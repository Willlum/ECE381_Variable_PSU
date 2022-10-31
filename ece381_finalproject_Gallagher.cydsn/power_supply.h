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
#ifndef _POWER_SUPPLY_H
#define _POWER_SUPPLY_H
    
#define INA260_I2C_ADDR 0x40       // INA260 default i2c address
#define INA260_REG_CONFIG 0x00     // Configuration register
#define INA260_REG_MFG_UID 0xFE     // Manufacturer ID Register
#define INA260_MFG_UID 0x5449     // Manufacturer ID 
    
    
#define INA260_REG_CURRENT 0x01    // Current measurement register (signed) in mA
#define INA260_REG_BUSVOLTAGE 0x02 // Bus voltage measurement register in mV
#define INA260_REG_POWER 0x03      // Power calculation register in mW

#define INA260_MODE_CONTINUOUS 0x07 

typedef enum alert_type {
  INA260_ALERT_CONVERSION_READY = 0x1, // Trigger on conversion ready
  INA260_ALERT_OVERPOWER = 0x2,        // Trigger on power over limit
  INA260_ALERT_UNDERVOLTAGE = 0x4,     // Trigger on bus voltage under limit
  INA260_ALERT_OVERVOLTAGE = 0x8,      // Trigger on bus voltage over limit
  INA260_ALERT_UNDERCURRENT = 0x10,    // Trigger on current under limit
  INA260_ALERT_OVERCURRENT = 0x20,     // Trigger on current over limit
  INA260_ALERT_NONE = 0x0,             // Do not trigger alert pin (Default)
} INA260_AlertType;


#endif
    

/* [] END OF FILE */
