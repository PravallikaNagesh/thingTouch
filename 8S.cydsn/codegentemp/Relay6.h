/*******************************************************************************
* File Name: Relay6.h  
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

#if !defined(CY_PINS_Relay6_H) /* Pins Relay6_H */
#define CY_PINS_Relay6_H

#include "cytypes.h"
#include "cyfitter.h"
#include "Relay6_aliases.h"


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
} Relay6_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   Relay6_Read(void);
void    Relay6_Write(uint8 value);
uint8   Relay6_ReadDataReg(void);
#if defined(Relay6__PC) || (CY_PSOC4_4200L) 
    void    Relay6_SetDriveMode(uint8 mode);
#endif
void    Relay6_SetInterruptMode(uint16 position, uint16 mode);
uint8   Relay6_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void Relay6_Sleep(void); 
void Relay6_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(Relay6__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define Relay6_DRIVE_MODE_BITS        (3)
    #define Relay6_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - Relay6_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the Relay6_SetDriveMode() function.
         *  @{
         */
        #define Relay6_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define Relay6_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define Relay6_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define Relay6_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define Relay6_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define Relay6_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define Relay6_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define Relay6_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define Relay6_MASK               Relay6__MASK
#define Relay6_SHIFT              Relay6__SHIFT
#define Relay6_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Relay6_SetInterruptMode() function.
     *  @{
     */
        #define Relay6_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define Relay6_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define Relay6_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define Relay6_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(Relay6__SIO)
    #define Relay6_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(Relay6__PC) && (CY_PSOC4_4200L)
    #define Relay6_USBIO_ENABLE               ((uint32)0x80000000u)
    #define Relay6_USBIO_DISABLE              ((uint32)(~Relay6_USBIO_ENABLE))
    #define Relay6_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define Relay6_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define Relay6_USBIO_ENTER_SLEEP          ((uint32)((1u << Relay6_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << Relay6_USBIO_SUSPEND_DEL_SHIFT)))
    #define Relay6_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << Relay6_USBIO_SUSPEND_SHIFT)))
    #define Relay6_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << Relay6_USBIO_SUSPEND_DEL_SHIFT)))
    #define Relay6_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(Relay6__PC)
    /* Port Configuration */
    #define Relay6_PC                 (* (reg32 *) Relay6__PC)
#endif
/* Pin State */
#define Relay6_PS                     (* (reg32 *) Relay6__PS)
/* Data Register */
#define Relay6_DR                     (* (reg32 *) Relay6__DR)
/* Input Buffer Disable Override */
#define Relay6_INP_DIS                (* (reg32 *) Relay6__PC2)

/* Interrupt configuration Registers */
#define Relay6_INTCFG                 (* (reg32 *) Relay6__INTCFG)
#define Relay6_INTSTAT                (* (reg32 *) Relay6__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define Relay6_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(Relay6__SIO)
    #define Relay6_SIO_REG            (* (reg32 *) Relay6__SIO)
#endif /* (Relay6__SIO_CFG) */

/* USBIO registers */
#if !defined(Relay6__PC) && (CY_PSOC4_4200L)
    #define Relay6_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define Relay6_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define Relay6_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define Relay6_DRIVE_MODE_SHIFT       (0x00u)
#define Relay6_DRIVE_MODE_MASK        (0x07u << Relay6_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins Relay6_H */


/* [] END OF FILE */
