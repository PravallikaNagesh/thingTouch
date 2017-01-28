/*******************************************************************************
* File Name: LED_CONTROL1_PM.c
* Version 2.10
*
* Description:
*  This file contains the setup, control, and status commands to support
*  the component operations in the low power mode.
*
* Note:
*  None
*
********************************************************************************
* Copyright 2013-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "LED_CONTROL1.h"

static LED_CONTROL1_BACKUP_STRUCT LED_CONTROL1_backup;


/*******************************************************************************
* Function Name: LED_CONTROL1_SaveConfig
********************************************************************************
*
* Summary:
*  All configuration registers are retention. Nothing to save here.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void LED_CONTROL1_SaveConfig(void)
{

}


/*******************************************************************************
* Function Name: LED_CONTROL1_Sleep
********************************************************************************
*
* Summary:
*  Stops the component operation and saves the user configuration.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void LED_CONTROL1_Sleep(void)
{
    if(0u != (LED_CONTROL1_BLOCK_CONTROL_REG & LED_CONTROL1_MASK))
    {
        LED_CONTROL1_backup.enableState = 1u;
    }
    else
    {
        LED_CONTROL1_backup.enableState = 0u;
    }

    LED_CONTROL1_Stop();
    LED_CONTROL1_SaveConfig();
}


/*******************************************************************************
* Function Name: LED_CONTROL1_RestoreConfig
********************************************************************************
*
* Summary:
*  All configuration registers are retention. Nothing to restore here.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void LED_CONTROL1_RestoreConfig(void)
{

}


/*******************************************************************************
* Function Name: LED_CONTROL1_Wakeup
********************************************************************************
*
* Summary:
*  Restores the user configuration and restores the enable state.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void LED_CONTROL1_Wakeup(void)
{
    LED_CONTROL1_RestoreConfig();

    if(0u != LED_CONTROL1_backup.enableState)
    {
        LED_CONTROL1_Enable();
    }
}


/* [] END OF FILE */
