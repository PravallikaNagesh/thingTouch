/*******************************************************************************
* File Name: LED15.c  
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
#include "LED15.h"

static LED15_BACKUP_STRUCT  LED15_backup = {0u, 0u, 0u};


/*******************************************************************************
* Function Name: LED15_Sleep
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
*  \snippet LED15_SUT.c usage_LED15_Sleep_Wakeup
*******************************************************************************/
void LED15_Sleep(void)
{
    #if defined(LED15__PC)
        LED15_backup.pcState = LED15_PC;
    #else
        #if (CY_PSOC4_4200L)
            /* Save the regulator state and put the PHY into suspend mode */
            LED15_backup.usbState = LED15_CR1_REG;
            LED15_USB_POWER_REG |= LED15_USBIO_ENTER_SLEEP;
            LED15_CR1_REG &= LED15_USBIO_CR1_OFF;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(LED15__SIO)
        LED15_backup.sioState = LED15_SIO_REG;
        /* SIO requires unregulated output buffer and single ended input buffer */
        LED15_SIO_REG &= (uint32)(~LED15_SIO_LPM_MASK);
    #endif  
}


/*******************************************************************************
* Function Name: LED15_Wakeup
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
*  Refer to LED15_Sleep() for an example usage.
*******************************************************************************/
void LED15_Wakeup(void)
{
    #if defined(LED15__PC)
        LED15_PC = LED15_backup.pcState;
    #else
        #if (CY_PSOC4_4200L)
            /* Restore the regulator state and come out of suspend mode */
            LED15_USB_POWER_REG &= LED15_USBIO_EXIT_SLEEP_PH1;
            LED15_CR1_REG = LED15_backup.usbState;
            LED15_USB_POWER_REG &= LED15_USBIO_EXIT_SLEEP_PH2;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(LED15__SIO)
        LED15_SIO_REG = LED15_backup.sioState;
    #endif
}


/* [] END OF FILE */
