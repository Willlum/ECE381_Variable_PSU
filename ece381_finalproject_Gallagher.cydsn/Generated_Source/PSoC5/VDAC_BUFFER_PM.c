/*******************************************************************************
* File Name: VDAC_BUFFER_PM.c
* Version 1.90
*
* Description:
*  This file provides the power management source code to the API for the 
*  OpAmp (Analog Buffer) component.
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

static VDAC_BUFFER_BACKUP_STRUCT  VDAC_BUFFER_backup;


/*******************************************************************************  
* Function Name: VDAC_BUFFER_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the current user configuration registers.
* 
* Parameters:
*  void
* 
* Return:
*  void
*
*******************************************************************************/
void VDAC_BUFFER_SaveConfig(void) 
{
    /* Nothing to save as registers are System reset on retention flops */
}


/*******************************************************************************  
* Function Name: VDAC_BUFFER_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration registers.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void VDAC_BUFFER_RestoreConfig(void) 
{
    /* Nothing to restore */
}


/*******************************************************************************   
* Function Name: VDAC_BUFFER_Sleep
********************************************************************************
*
* Summary:
*  Disables block's operation and saves its configuration. Should be called 
*  just prior to entering sleep.
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  VDAC_BUFFER_backup: The structure field 'enableState' is modified 
*  depending on the enable state of the block before entering to sleep mode.
*
*******************************************************************************/
void VDAC_BUFFER_Sleep(void) 
{
    /* Save OpAmp enable state */
    if((VDAC_BUFFER_PM_ACT_CFG_REG & VDAC_BUFFER_ACT_PWR_EN) != 0u)
    {
        /* Component is enabled */
        VDAC_BUFFER_backup.enableState = 1u;
         /* Stops the component */
         VDAC_BUFFER_Stop();
    }
    else
    {
        /* Component is disabled */
        VDAC_BUFFER_backup.enableState = 0u;
    }
    /* Saves the configuration */
    VDAC_BUFFER_SaveConfig();
}


/*******************************************************************************  
* Function Name: VDAC_BUFFER_Wakeup
********************************************************************************
*
* Summary:
*  Enables block's operation and restores its configuration. Should be called
*  just after awaking from sleep.
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  VDAC_BUFFER_backup: The structure field 'enableState' is used to 
*  restore the enable state of block after wakeup from sleep mode.
*
*******************************************************************************/
void VDAC_BUFFER_Wakeup(void) 
{
    /* Restore the user configuration */
    VDAC_BUFFER_RestoreConfig();

    /* Enables the component operation */
    if(VDAC_BUFFER_backup.enableState == 1u)
    {
        VDAC_BUFFER_Enable();
    } /* Do nothing if component was disable before */
}


/* [] END OF FILE */
