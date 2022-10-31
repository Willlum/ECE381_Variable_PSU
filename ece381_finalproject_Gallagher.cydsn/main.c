/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
#include "power_supply.h"
#include <stdio.h>
#define PWM_UPPER_BOUND 719
#define PWM_LOWER_BOUND 15
volatile uint16 enc_value;
volatile int8 enc_isr_flag;
volatile uint8 sw1_isr_flag;
volatile uint8 timer_isr_flag=0;
uint16 pwm_set_point=1;

void I2C_Reg_Read(uint8, uint8*, uint8);
uint16 value_feedback(float, float, uint16);
int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    LCD_Start();
    I2C_Start();
    ENC_ISR_Start();
    TIMER_Start();
    REFRESH_ISR_Start();
    SW1_ISR_Start();
    PWM_Start();
    
    
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    char buffer [16];
    uint8 data_buffer[2];
    uint8 setting=0;
   
    float value=0, set_point=0;
    int16 connected;

    I2C_Reg_Read(INA260_REG_MFG_UID, data_buffer, 1);
    connected = (int16)((data_buffer[0] << 8) | data_buffer[1]);
    
//    PWM_WriteCompare(PWM_UPPER_BOUND/2);
//    while(1){
//        for(int i =0; i < PWM_UPPER_BOUND;i++){
//            PWM_WriteCompare(i);
//            CyDelay(50);
//         }
//    }
//    
//    
    
    LCD_Position(0,1);
    snprintf (buffer, sizeof buffer,"P-ID: 0h%x", connected);
    LCD_PrintString(buffer);
    LCD_Position(1,1);
    if(connected == INA260_MFG_UID){
        snprintf (buffer, sizeof buffer,"Connected :)");
        LCD_PrintString(buffer);
    }else{
        snprintf (buffer, sizeof buffer,"No device :(");
        LCD_PrintString(buffer);
    }
        
    CyDelay(1000);
    LCD_ClearDisplay();
        
    for(;;)
    {   
// START CC or CV SELECT MENU //
        enc_value = 0;
        sw1_isr_flag = 0;
        setting = 0;
        PWM_WriteCompare(PWM_UPPER_BOUND);
        while(!sw1_isr_flag){
            if(timer_isr_flag){   
                timer_isr_flag = 0;
                if(enc_value % 2 == 0){
                    LCD_ClearDisplay();
                    LCD_Position(0,0);
                    snprintf (buffer, sizeof buffer,">CV");
                    LCD_PrintString(buffer);
                    LCD_Position(1,0);
                    snprintf (buffer, sizeof buffer," CC");
                    LCD_PrintString(buffer);
                    setting = 0;
                }
                else{
                    LCD_ClearDisplay();
                    LCD_Position(0,0);
                    snprintf (buffer, sizeof buffer," CV");
                    LCD_PrintString(buffer);
                    LCD_Position(1,0);
                    snprintf (buffer, sizeof buffer,">CC");
                    LCD_PrintString(buffer);
                    setting = 1;
                }
            }
        }
// END CC or CV SELECT MENU //
    LCD_ClearDisplay(); 
    sw1_isr_flag = 0;
    enc_value = 50;
// START Value SELECT MENU //    
    while(!sw1_isr_flag){
        if(setting == 0){
            if(timer_isr_flag){
                timer_isr_flag = 0;
                if(enc_value > 1 && enc_value < 150) set_point = (enc_value) / 10.0;
                else enc_value = 50;
                LCD_ClearDisplay(); 
                LCD_Position(0,0);
                snprintf (buffer, sizeof buffer,"Voltage Mode");
                LCD_PrintString(buffer);
                LCD_Position(1,0);
                snprintf (buffer, sizeof buffer,"Set-Pnt:%.1fV", set_point);
                LCD_PrintString(buffer);
            }
        }
        else{
            if(timer_isr_flag){
                timer_isr_flag = 0;
                if(enc_value > 1 && enc_value < 100) set_point = (enc_value);
                else enc_value = 20;
                LCD_ClearDisplay(); 
                LCD_Position(0,0);
                snprintf (buffer, sizeof buffer,"Current Mode");
                LCD_PrintString(buffer);
                LCD_Position(1,0);
                snprintf (buffer, sizeof buffer,"SP:%.2fmA", set_point);
                LCD_PrintString(buffer);
                }
            }
        }
// END Value SELECT MENU //   
    pwm_set_point = 1;
    sw1_isr_flag = 0;
    LCD_ClearDisplay();
// START PWM Feedback //       
    while(!sw1_isr_flag){
        if(setting == 0){
            I2C_Reg_Read(INA260_REG_BUSVOLTAGE, data_buffer,1); 
            value = (float)((((data_buffer[0] << 8) | data_buffer[1]) * 1.25) / 1000.0);
            pwm_set_point = value_feedback(set_point,value,pwm_set_point);
            if(timer_isr_flag){
                timer_isr_flag = 0;
                LCD_ClearDisplay(); 
                LCD_Position(0,0);
                snprintf (buffer, sizeof buffer,"Set-Pnt:%.2fV", set_point);
                LCD_PrintString(buffer);
                LCD_Position(1,0);
                snprintf (buffer, sizeof buffer,"Value:%.2fV", value);
                //snprintf (buffer, sizeof buffer,"D.C.:%d", pwm_set_point);
                LCD_PrintString(buffer);
            }
        }
        else{
            I2C_Reg_Read(INA260_REG_CURRENT, data_buffer,1); 
            value = (float)(((data_buffer[0] << 8) | data_buffer[1]) * 1.25);
            pwm_set_point = value_feedback(set_point,value,pwm_set_point);
            if(timer_isr_flag){
                timer_isr_flag = 0;
                LCD_ClearDisplay(); 
                LCD_Position(0,0);
                snprintf (buffer, sizeof buffer,"Set-Pnt:%.2fmA", set_point);
                LCD_PrintString(buffer);
                LCD_Position(1,0);
                snprintf (buffer, sizeof buffer,"Value:%.2fmA", value);
                //snprintf (buffer, sizeof buffer,"D.C.:%.2fV", pwm_set_point);
                LCD_PrintString(buffer);
            }
        }
    }
    sw1_isr_flag = 0;
    LCD_ClearDisplay(); 
// END PWM Feedback //  
   }
}
void I2C_Reg_Read(uint8 register_pointer, uint8 *data_buffer, uint8 is_first){
    if(is_first){
        //set register pointer  
        I2C_MasterWriteBuf(INA260_I2C_ADDR, &register_pointer, 1, I2C_MODE_COMPLETE_XFER);
        while (!(I2C_MasterStatus() & I2C_MSTAT_WR_CMPLT));
        I2C_MasterClearStatus();
    }
        //read data from register into data buffer
        I2C_MasterReadBuf(INA260_I2C_ADDR, data_buffer, 2, I2C_MODE_COMPLETE_XFER);
        while (!(I2C_MasterStatus() & I2C_MSTAT_RD_CMPLT));
        I2C_MasterClearStatus();
}
uint16 value_feedback(float set_point, float value, uint16 pwm_set_point){
    if(pwm_set_point < PWM_UPPER_BOUND){
        if(value > set_point) pwm_set_point++;
    }
    if(pwm_set_point > PWM_LOWER_BOUND){
        if(value < set_point) pwm_set_point--;
    }
    PWM_WriteCompare(pwm_set_point);
    return pwm_set_point;
}
/* [] END OF FILE */
