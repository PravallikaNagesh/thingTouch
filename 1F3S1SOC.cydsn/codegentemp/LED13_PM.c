/*******************************************************************************
* File Name: LED13.c  
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
#include "LED13.h"

static LED13_BACKUP_STRUCT  LED13_backup = {0u, 0u, 0u};


/*******************************************************************************
* Function Name: LED13_Sleep
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
*  \snippet LED13_SUT.c usage_LED13_Sleep_Wakeup
*******************************************************************************/
void LED13_Sleep(void)
{
    #if defined(LED13__PC)
        LED13_backup.pcState = LED13_PC;
    #else
        #if (CY_PSOC4_4200L)
            /* Save the regulator state and put the PHY into suspend mode */
            LED13_backup.usbState = LED13_CR1_REG;
            LED13_USB_POWER_REG |= LED13_USBIO_ENTER_SLEEP;
            LED13_CR1_REG &= LED13_USBIO_CR1_OFF;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(LED13__SIO)
        LED13_backup.sioState = LED13_SIO_REG;
        /* SIO requires unregulated output buffer and single ended input buffer */
        LED13_SIO_REG &= (uint32)(~LED13_SIO_LPM_MASK);
    #endif  
}


/*******************************************************************************
* Function Name: LED13_Wakeup
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
*  Refer to LED13_Sleep() for an example usage.
*******************************************************************************/
void LED13_Wakeup(void)
{
    #if defined(LED13__PC)
        LED13_PC = LED13_backup.pcState;
    #else
        #if (CY_PSOC4_4200L)
            /* Restore the regulator state and come out of suspend mode */
            LED13_USB_POWER_REG &= LED13_USBIO_EXIT_SLEEP_PH1;
            LED13_CR1_REG = LED13_backup.usbState;
            LED13_USB_POWER_REG &= LED13_USBIO_EXIT_SLEEP_PH2;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(LED13__SIO)
        LED13_SIO_REG = LED13_backup.sioState;
    #endif
}


/* [] END OF FILE */
