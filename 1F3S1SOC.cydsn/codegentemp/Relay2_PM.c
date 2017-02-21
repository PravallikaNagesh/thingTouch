/*******************************************************************************
* File Name: Relay2.c  
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
#include "Relay2.h"

static Relay2_BACKUP_STRUCT  Relay2_backup = {0u, 0u, 0u};


/*******************************************************************************
* Function Name: Relay2_Sleep
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
*  \snippet Relay2_SUT.c usage_Relay2_Sleep_Wakeup
*******************************************************************************/
void Relay2_Sleep(void)
{
    #if defined(Relay2__PC)
        Relay2_backup.pcState = Relay2_PC;
    #else
        #if (CY_PSOC4_4200L)
            /* Save the regulator state and put the PHY into suspend mode */
            Relay2_backup.usbState = Relay2_CR1_REG;
            Relay2_USB_POWER_REG |= Relay2_USBIO_ENTER_SLEEP;
            Relay2_CR1_REG &= Relay2_USBIO_CR1_OFF;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(Relay2__SIO)
        Relay2_backup.sioState = Relay2_SIO_REG;
        /* SIO requires unregulated output buffer and single ended input buffer */
        Relay2_SIO_REG &= (uint32)(~Relay2_SIO_LPM_MASK);
    #endif  
}


/*******************************************************************************
* Function Name: Relay2_Wakeup
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
*  Refer to Relay2_Sleep() for an example usage.
*******************************************************************************/
void Relay2_Wakeup(void)
{
    #if defined(Relay2__PC)
        Relay2_PC = Relay2_backup.pcState;
    #else
        #if (CY_PSOC4_4200L)
            /* Restore the regulator state and come out of suspend mode */
            Relay2_USB_POWER_REG &= Relay2_USBIO_EXIT_SLEEP_PH1;
            Relay2_CR1_REG = Relay2_backup.usbState;
            Relay2_USB_POWER_REG &= Relay2_USBIO_EXIT_SLEEP_PH2;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(Relay2__SIO)
        Relay2_SIO_REG = Relay2_backup.sioState;
    #endif
}


/* [] END OF FILE */
