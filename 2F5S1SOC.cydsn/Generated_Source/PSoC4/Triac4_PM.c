/*******************************************************************************
* File Name: Triac4.c  
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
#include "Triac4.h"

static Triac4_BACKUP_STRUCT  Triac4_backup = {0u, 0u, 0u};


/*******************************************************************************
* Function Name: Triac4_Sleep
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
*  \snippet Triac4_SUT.c usage_Triac4_Sleep_Wakeup
*******************************************************************************/
void Triac4_Sleep(void)
{
    #if defined(Triac4__PC)
        Triac4_backup.pcState = Triac4_PC;
    #else
        #if (CY_PSOC4_4200L)
            /* Save the regulator state and put the PHY into suspend mode */
            Triac4_backup.usbState = Triac4_CR1_REG;
            Triac4_USB_POWER_REG |= Triac4_USBIO_ENTER_SLEEP;
            Triac4_CR1_REG &= Triac4_USBIO_CR1_OFF;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(Triac4__SIO)
        Triac4_backup.sioState = Triac4_SIO_REG;
        /* SIO requires unregulated output buffer and single ended input buffer */
        Triac4_SIO_REG &= (uint32)(~Triac4_SIO_LPM_MASK);
    #endif  
}


/*******************************************************************************
* Function Name: Triac4_Wakeup
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
*  Refer to Triac4_Sleep() for an example usage.
*******************************************************************************/
void Triac4_Wakeup(void)
{
    #if defined(Triac4__PC)
        Triac4_PC = Triac4_backup.pcState;
    #else
        #if (CY_PSOC4_4200L)
            /* Restore the regulator state and come out of suspend mode */
            Triac4_USB_POWER_REG &= Triac4_USBIO_EXIT_SLEEP_PH1;
            Triac4_CR1_REG = Triac4_backup.usbState;
            Triac4_USB_POWER_REG &= Triac4_USBIO_EXIT_SLEEP_PH2;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(Triac4__SIO)
        Triac4_SIO_REG = Triac4_backup.sioState;
    #endif
}


/* [] END OF FILE */
