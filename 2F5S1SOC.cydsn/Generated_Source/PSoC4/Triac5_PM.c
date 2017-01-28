/*******************************************************************************
* File Name: Triac5.c  
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
#include "Triac5.h"

static Triac5_BACKUP_STRUCT  Triac5_backup = {0u, 0u, 0u};


/*******************************************************************************
* Function Name: Triac5_Sleep
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
*  \snippet Triac5_SUT.c usage_Triac5_Sleep_Wakeup
*******************************************************************************/
void Triac5_Sleep(void)
{
    #if defined(Triac5__PC)
        Triac5_backup.pcState = Triac5_PC;
    #else
        #if (CY_PSOC4_4200L)
            /* Save the regulator state and put the PHY into suspend mode */
            Triac5_backup.usbState = Triac5_CR1_REG;
            Triac5_USB_POWER_REG |= Triac5_USBIO_ENTER_SLEEP;
            Triac5_CR1_REG &= Triac5_USBIO_CR1_OFF;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(Triac5__SIO)
        Triac5_backup.sioState = Triac5_SIO_REG;
        /* SIO requires unregulated output buffer and single ended input buffer */
        Triac5_SIO_REG &= (uint32)(~Triac5_SIO_LPM_MASK);
    #endif  
}


/*******************************************************************************
* Function Name: Triac5_Wakeup
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
*  Refer to Triac5_Sleep() for an example usage.
*******************************************************************************/
void Triac5_Wakeup(void)
{
    #if defined(Triac5__PC)
        Triac5_PC = Triac5_backup.pcState;
    #else
        #if (CY_PSOC4_4200L)
            /* Restore the regulator state and come out of suspend mode */
            Triac5_USB_POWER_REG &= Triac5_USBIO_EXIT_SLEEP_PH1;
            Triac5_CR1_REG = Triac5_backup.usbState;
            Triac5_USB_POWER_REG &= Triac5_USBIO_EXIT_SLEEP_PH2;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(Triac5__SIO)
        Triac5_SIO_REG = Triac5_backup.sioState;
    #endif
}


/* [] END OF FILE */
