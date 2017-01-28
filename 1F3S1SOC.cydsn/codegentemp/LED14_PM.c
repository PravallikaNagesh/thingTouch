/*******************************************************************************
* File Name: LED14.c  
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
#include "LED14.h"

static LED14_BACKUP_STRUCT  LED14_backup = {0u, 0u, 0u};


/*******************************************************************************
* Function Name: LED14_Sleep
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
*  \snippet LED14_SUT.c usage_LED14_Sleep_Wakeup
*******************************************************************************/
void LED14_Sleep(void)
{
    #if defined(LED14__PC)
        LED14_backup.pcState = LED14_PC;
    #else
        #if (CY_PSOC4_4200L)
            /* Save the regulator state and put the PHY into suspend mode */
            LED14_backup.usbState = LED14_CR1_REG;
            LED14_USB_POWER_REG |= LED14_USBIO_ENTER_SLEEP;
            LED14_CR1_REG &= LED14_USBIO_CR1_OFF;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(LED14__SIO)
        LED14_backup.sioState = LED14_SIO_REG;
        /* SIO requires unregulated output buffer and single ended input buffer */
        LED14_SIO_REG &= (uint32)(~LED14_SIO_LPM_MASK);
    #endif  
}


/*******************************************************************************
* Function Name: LED14_Wakeup
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
*  Refer to LED14_Sleep() for an example usage.
*******************************************************************************/
void LED14_Wakeup(void)
{
    #if defined(LED14__PC)
        LED14_PC = LED14_backup.pcState;
    #else
        #if (CY_PSOC4_4200L)
            /* Restore the regulator state and come out of suspend mode */
            LED14_USB_POWER_REG &= LED14_USBIO_EXIT_SLEEP_PH1;
            LED14_CR1_REG = LED14_backup.usbState;
            LED14_USB_POWER_REG &= LED14_USBIO_EXIT_SLEEP_PH2;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(LED14__SIO)
        LED14_SIO_REG = LED14_backup.sioState;
    #endif
}


/* [] END OF FILE */
