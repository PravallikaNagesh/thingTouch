/*******************************************************************************
* File Name: Triac4.h  
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

#if !defined(CY_PINS_Triac4_H) /* Pins Triac4_H */
#define CY_PINS_Triac4_H

#include "cytypes.h"
#include "cyfitter.h"
#include "Triac4_aliases.h"


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
} Triac4_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   Triac4_Read(void);
void    Triac4_Write(uint8 value);
uint8   Triac4_ReadDataReg(void);
#if defined(Triac4__PC) || (CY_PSOC4_4200L) 
    void    Triac4_SetDriveMode(uint8 mode);
#endif
void    Triac4_SetInterruptMode(uint16 position, uint16 mode);
uint8   Triac4_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void Triac4_Sleep(void); 
void Triac4_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(Triac4__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define Triac4_DRIVE_MODE_BITS        (3)
    #define Triac4_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - Triac4_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the Triac4_SetDriveMode() function.
         *  @{
         */
        #define Triac4_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define Triac4_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define Triac4_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define Triac4_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define Triac4_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define Triac4_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define Triac4_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define Triac4_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define Triac4_MASK               Triac4__MASK
#define Triac4_SHIFT              Triac4__SHIFT
#define Triac4_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Triac4_SetInterruptMode() function.
     *  @{
     */
        #define Triac4_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define Triac4_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define Triac4_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define Triac4_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(Triac4__SIO)
    #define Triac4_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(Triac4__PC) && (CY_PSOC4_4200L)
    #define Triac4_USBIO_ENABLE               ((uint32)0x80000000u)
    #define Triac4_USBIO_DISABLE              ((uint32)(~Triac4_USBIO_ENABLE))
    #define Triac4_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define Triac4_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define Triac4_USBIO_ENTER_SLEEP          ((uint32)((1u << Triac4_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << Triac4_USBIO_SUSPEND_DEL_SHIFT)))
    #define Triac4_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << Triac4_USBIO_SUSPEND_SHIFT)))
    #define Triac4_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << Triac4_USBIO_SUSPEND_DEL_SHIFT)))
    #define Triac4_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(Triac4__PC)
    /* Port Configuration */
    #define Triac4_PC                 (* (reg32 *) Triac4__PC)
#endif
/* Pin State */
#define Triac4_PS                     (* (reg32 *) Triac4__PS)
/* Data Register */
#define Triac4_DR                     (* (reg32 *) Triac4__DR)
/* Input Buffer Disable Override */
#define Triac4_INP_DIS                (* (reg32 *) Triac4__PC2)

/* Interrupt configuration Registers */
#define Triac4_INTCFG                 (* (reg32 *) Triac4__INTCFG)
#define Triac4_INTSTAT                (* (reg32 *) Triac4__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define Triac4_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(Triac4__SIO)
    #define Triac4_SIO_REG            (* (reg32 *) Triac4__SIO)
#endif /* (Triac4__SIO_CFG) */

/* USBIO registers */
#if !defined(Triac4__PC) && (CY_PSOC4_4200L)
    #define Triac4_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define Triac4_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define Triac4_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define Triac4_DRIVE_MODE_SHIFT       (0x00u)
#define Triac4_DRIVE_MODE_MASK        (0x07u << Triac4_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins Triac4_H */


/* [] END OF FILE */
