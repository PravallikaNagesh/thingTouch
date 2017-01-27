/*******************************************************************************
* File Name: LED12.c  
* Version 2.20
*
* Description:
*  This file contains APIs to set up the Pins component for low power modes.
*
* Note:
*
********************************************************************************
* Copyright 2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "cytypes.h"
#include "LED12.h"

static LED12_BACKUP_STRUCT  LED12_backup = {0u, 0u, 0u};


/*******************************************************************************
* Function Name: LED12_Sleep
****************************************************************************//**
*
* \brief Stores the pin configuration and prepares the pin for entering chip 
*  deep-sleep/hibernate modes. This function must be called for SIO and USBIO
*  pins. It is not essential if using GPIO or GPIO_OVT pins.
*
* <b>Note</b> This function is available in PSoC 4 only.
*
* \return 
*  None 
*  
* \sideeffect
*  For SIO pins, this function configures the pin input threshold to CMOS and
*  drive level to Vddio. This is needed for SIO pins when in device 
*  deep-sleep/hibernate modes.
*
* \funcusage
*  \snippet LED12_SUT.c usage_LED12_Sleep_Wakeup
*******************************************************************************/
void LED12_Sleep(void)
{
    #if defined(LED12__PC)
        LED12_backup.pcState = LED12_PC;
    #else
        #if (CY_PSOC4_4200L)
            /* Save the regulator state and put the PHY into suspend mode */
            LED12_backup.usbState = LED12_CR1_REG;
            LED12_USB_POWER_REG |= LED12_USBIO_ENTER_SLEEP;
            LED12_CR1_REG &= LED12_USBIO_CR1_OFF;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(LED12__SIO)
        LED12_backup.sioState = LED12_SIO_REG;
        /* SIO requires unregulated output buffer and single ended input buffer */
        LED12_SIO_REG &= (uint32)(~LED12_SIO_LPM_MASK);
    #endif  
}


/*******************************************************************************
* Function Name: LED12_Wakeup
****************************************************************************//**
*
* \brief Restores the pin configuration that was saved during Pin_Sleep().
*
* For USBIO pins, the wakeup is only triggered for falling edge interrupts.
*
* <b>Note</b> This function is available in PSoC 4 only.
*
* \return 
*  None
*  
* \funcusage
*  Refer to LED12_Sleep() for an example usage.
*******************************************************************************/
void LED12_Wakeup(void)
{
    #if defined(LED12__PC)
        LED12_PC = LED12_backup.pcState;
    #else
        #if (CY_PSOC4_4200L)
            /* Restore the regulator state and come out of suspend mode */
            LED12_USB_POWER_REG &= LED12_USBIO_EXIT_SLEEP_PH1;
            LED12_CR1_REG = LED12_backup.usbState;
            LED12_USB_POWER_REG &= LED12_USBIO_EXIT_SLEEP_PH2;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(LED12__SIO)
        LED12_SIO_REG = LED12_backup.sioState;
    #endif
}


/* [] END OF FILE */
