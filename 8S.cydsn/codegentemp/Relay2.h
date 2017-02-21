/*******************************************************************************
* File Name: Relay2.h  
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

#if !defined(CY_PINS_Relay2_H) /* Pins Relay2_H */
#define CY_PINS_Relay2_H

#include "cytypes.h"
#include "cyfitter.h"
#include "Relay2_aliases.h"


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
} Relay2_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   Relay2_Read(void);
void    Relay2_Write(uint8 value);
uint8   Relay2_ReadDataReg(void);
#if defined(Relay2__PC) || (CY_PSOC4_4200L) 
    void    Relay2_SetDriveMode(uint8 mode);
#endif
void    Relay2_SetInterruptMode(uint16 position, uint16 mode);
uint8   Relay2_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void Relay2_Sleep(void); 
void Relay2_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(Relay2__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define Relay2_DRIVE_MODE_BITS        (3)
    #define Relay2_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - Relay2_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the Relay2_SetDriveMode() function.
         *  @{
         */
        #define Relay2_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define Relay2_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define Relay2_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define Relay2_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define Relay2_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define Relay2_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define Relay2_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define Relay2_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define Relay2_MASK               Relay2__MASK
#define Relay2_SHIFT              Relay2__SHIFT
#define Relay2_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Relay2_SetInterruptMode() function.
     *  @{
     */
        #define Relay2_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define Relay2_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define Relay2_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define Relay2_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(Relay2__SIO)
    #define Relay2_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(Relay2__PC) && (CY_PSOC4_4200L)
    #define Relay2_USBIO_ENABLE               ((uint32)0x80000000u)
    #define Relay2_USBIO_DISABLE              ((uint32)(~Relay2_USBIO_ENABLE))
    #define Relay2_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define Relay2_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define Relay2_USBIO_ENTER_SLEEP          ((uint32)((1u << Relay2_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << Relay2_USBIO_SUSPEND_DEL_SHIFT)))
    #define Relay2_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << Relay2_USBIO_SUSPEND_SHIFT)))
    #define Relay2_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << Relay2_USBIO_SUSPEND_DEL_SHIFT)))
    #define Relay2_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(Relay2__PC)
    /* Port Configuration */
    #define Relay2_PC                 (* (reg32 *) Relay2__PC)
#endif
/* Pin State */
#define Relay2_PS                     (* (reg32 *) Relay2__PS)
/* Data Register */
#define Relay2_DR                     (* (reg32 *) Relay2__DR)
/* Input Buffer Disable Override */
#define Relay2_INP_DIS                (* (reg32 *) Relay2__PC2)

/* Interrupt configuration Registers */
#define Relay2_INTCFG                 (* (reg32 *) Relay2__INTCFG)
#define Relay2_INTSTAT                (* (reg32 *) Relay2__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define Relay2_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(Relay2__SIO)
    #define Relay2_SIO_REG            (* (reg32 *) Relay2__SIO)
#endif /* (Relay2__SIO_CFG) */

/* USBIO registers */
#if !defined(Relay2__PC) && (CY_PSOC4_4200L)
    #define Relay2_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define Relay2_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define Relay2_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define Relay2_DRIVE_MODE_SHIFT       (0x00u)
#define Relay2_DRIVE_MODE_MASK        (0x07u << Relay2_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins Relay2_H */


/* [] END OF FILE */
