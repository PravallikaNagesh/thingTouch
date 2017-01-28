/*******************************************************************************
* File Name: LED15.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_LED15_H) /* Pins LED15_H */
#define CY_PINS_LED15_H

#include "cytypes.h"
#include "cyfitter.h"
#include "LED15_aliases.h"


/***************************************
*     Data Struct Definitions
***************************************/

/**
* \addtogroup group_structures
* @{
*/
    
/* Structure for sleep mode support */
typedef struct
{
    uint32 pcState; /**< State of the port control register */
    uint32 sioState; /**< State of the SIO configuration */
    uint32 usbState; /**< State of the USBIO regulator */
} LED15_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   LED15_Read(void);
void    LED15_Write(uint8 value);
uint8   LED15_ReadDataReg(void);
#if defined(LED15__PC) || (CY_PSOC4_4200L) 
    void    LED15_SetDriveMode(uint8 mode);
#endif
void    LED15_SetInterruptMode(uint16 position, uint16 mode);
uint8   LED15_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void LED15_Sleep(void); 
void LED15_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(LED15__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define LED15_DRIVE_MODE_BITS        (3)
    #define LED15_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - LED15_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the LED15_SetDriveMode() function.
         *  @{
         */
        #define LED15_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define LED15_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define LED15_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define LED15_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define LED15_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define LED15_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define LED15_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define LED15_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define LED15_MASK               LED15__MASK
#define LED15_SHIFT              LED15__SHIFT
#define LED15_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in LED15_SetInterruptMode() function.
     *  @{
     */
        #define LED15_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define LED15_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define LED15_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define LED15_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(LED15__SIO)
    #define LED15_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(LED15__PC) && (CY_PSOC4_4200L)
    #define LED15_USBIO_ENABLE               ((uint32)0x80000000u)
    #define LED15_USBIO_DISABLE              ((uint32)(~LED15_USBIO_ENABLE))
    #define LED15_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define LED15_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define LED15_USBIO_ENTER_SLEEP          ((uint32)((1u << LED15_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << LED15_USBIO_SUSPEND_DEL_SHIFT)))
    #define LED15_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << LED15_USBIO_SUSPEND_SHIFT)))
    #define LED15_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << LED15_USBIO_SUSPEND_DEL_SHIFT)))
    #define LED15_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(LED15__PC)
    /* Port Configuration */
    #define LED15_PC                 (* (reg32 *) LED15__PC)
#endif
/* Pin State */
#define LED15_PS                     (* (reg32 *) LED15__PS)
/* Data Register */
#define LED15_DR                     (* (reg32 *) LED15__DR)
/* Input Buffer Disable Override */
#define LED15_INP_DIS                (* (reg32 *) LED15__PC2)

/* Interrupt configuration Registers */
#define LED15_INTCFG                 (* (reg32 *) LED15__INTCFG)
#define LED15_INTSTAT                (* (reg32 *) LED15__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define LED15_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(LED15__SIO)
    #define LED15_SIO_REG            (* (reg32 *) LED15__SIO)
#endif /* (LED15__SIO_CFG) */

/* USBIO registers */
#if !defined(LED15__PC) && (CY_PSOC4_4200L)
    #define LED15_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define LED15_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define LED15_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define LED15_DRIVE_MODE_SHIFT       (0x00u)
#define LED15_DRIVE_MODE_MASK        (0x07u << LED15_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins LED15_H */


/* [] END OF FILE */
