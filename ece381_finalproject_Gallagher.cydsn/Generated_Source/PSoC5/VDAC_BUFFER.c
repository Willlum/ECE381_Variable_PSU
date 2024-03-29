/*******************************************************************************
* File Name: VDAC_BUFFER.c
* Version 1.90
*
* Description:
*  This file provides the source code to the API for OpAmp (Analog Buffer) 
*  Component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "VDAC_BUFFER.h"

uint8 VDAC_BUFFER_initVar = 0u;


/*******************************************************************************   
* Function Name: VDAC_BUFFER_Init
********************************************************************************
*
* Summary:
*  Initialize component's parameters to the parameters set by user in the 
*  customizer of the component placed onto schematic. Usually called in 
*  VDAC_BUFFER_Start().
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void VDAC_BUFFER_Init(void) 
{
    VDAC_BUFFER_SetPower(VDAC_BUFFER_DEFAULT_POWER);
}


/*******************************************************************************   
* Function Name: VDAC_BUFFER_Enable
********************************************************************************
*
* Summary:
*  Enables the OpAmp block operation
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void VDAC_BUFFER_Enable(void) 
{
    /* Enable negative charge pumps in ANIF */
    VDAC_BUFFER_PUMP_CR1_REG  |= (VDAC_BUFFER_PUMP_CR1_CLKSEL | VDAC_BUFFER_PUMP_CR1_FORCE);

    /* Enable power to buffer in active mode */
    VDAC_BUFFER_PM_ACT_CFG_REG |= VDAC_BUFFER_ACT_PWR_EN;

    /* Enable power to buffer in alternative active mode */
    VDAC_BUFFER_PM_STBY_CFG_REG |= VDAC_BUFFER_STBY_PWR_EN;
}


/*******************************************************************************
* Function Name:   VDAC_BUFFER_Start
********************************************************************************
*
* Summary:
*  The start function initializes the Analog Buffer with the default values and 
*  sets the power to the given level. A power level of 0, is same as 
*  executing the stop function.
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  VDAC_BUFFER_initVar: Used to check the initial configuration, modified 
*  when this function is called for the first time.
*
*******************************************************************************/
void VDAC_BUFFER_Start(void) 
{
    if(VDAC_BUFFER_initVar == 0u)
    {
        VDAC_BUFFER_initVar = 1u;
        VDAC_BUFFER_Init();
    }

    VDAC_BUFFER_Enable();
}


/*******************************************************************************
* Function Name: VDAC_BUFFER_Stop
********************************************************************************
*
* Summary:
*  Powers down amplifier to lowest power state.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void VDAC_BUFFER_Stop(void) 
{
    /* Disable power to buffer in active mode template */
    VDAC_BUFFER_PM_ACT_CFG_REG &= (uint8)(~VDAC_BUFFER_ACT_PWR_EN);

    /* Disable power to buffer in alternative active mode template */
    VDAC_BUFFER_PM_STBY_CFG_REG &= (uint8)(~VDAC_BUFFER_STBY_PWR_EN);
    
    /* Disable negative charge pumps for ANIF only if all ABuf is turned OFF */
    if(VDAC_BUFFER_PM_ACT_CFG_REG == 0u)
    {
        VDAC_BUFFER_PUMP_CR1_REG &= (uint8)(~(VDAC_BUFFER_PUMP_CR1_CLKSEL | VDAC_BUFFER_PUMP_CR1_FORCE));
    }
}


/*******************************************************************************
* Function Name: VDAC_BUFFER_SetPower
********************************************************************************
*
* Summary:
*  Sets power level of Analog buffer.
*
* Parameters: 
*  power: PSoC3: Sets power level between low (1) and high power (3).
*         PSoC5: Sets power level High (0)
*
* Return:
*  void
*
**********************************************************************************/
void VDAC_BUFFER_SetPower(uint8 power) 
{
    #if (CY_PSOC3 || CY_PSOC5LP)
        VDAC_BUFFER_CR_REG &= (uint8)(~VDAC_BUFFER_PWR_MASK);
        VDAC_BUFFER_CR_REG |= power & VDAC_BUFFER_PWR_MASK;      /* Set device power */
    #else
        CYASSERT(VDAC_BUFFER_HIGHPOWER == power);
    #endif /* CY_PSOC3 || CY_PSOC5LP */
}


/* [] END OF FILE */
