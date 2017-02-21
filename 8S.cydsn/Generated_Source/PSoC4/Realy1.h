/*******************************************************************************
* File Name: Realy1.h  
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

#if !defined(CY_PINS_Realy1_H) /* Pins Realy1_H */
#define CY_PINS_Realy1_H

#include "cytypes.h"
#include "cyfitter.h"
#include "Realy1_aliases.h"


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
} Realy1_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   Realy1_Read(void);
void    Realy1_Write(uint8 value);
uint8   Realy1_ReadDataReg(void);
#if defined(Realy1__PC) || (CY_PSOC4_4200L) 
    void    Realy1_SetDriveMode(uint8 mode);
#endif
void    Realy1_SetInterruptMode(uint16 position, uint16 mode);
uint8   Realy1_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void Realy1_Sleep(void); 
void Realy1_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(Realy1__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define Realy1_DRIVE_MODE_BITS        (3)
    #define Realy1_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - Realy1_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the Realy1_SetDriveMode() function.
         *  @{
         */
        #define Realy1_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define Realy1_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define Realy1_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define Realy1_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define Realy1_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define Realy1_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define Realy1_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define Realy1_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define Realy1_MASK               Realy1__MASK
#define Realy1_SHIFT              Realy1__SHIFT
#define Realy1_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Realy1_SetInterruptMode() function.
     *  @{
     */
        #define Realy1_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define Realy1_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define Realy1_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define Realy1_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(Realy1__SIO)
    #define Realy1_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(Realy1__PC) && (CY_PSOC4_4200L)
    #define Realy1_USBIO_ENABLE               ((uint32)0x80000000u)
    #define Realy1_USBIO_DISABLE              ((uint32)(~Realy1_USBIO_ENABLE))
    #define Realy1_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define Realy1_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define Realy1_USBIO_ENTER_SLEEP          ((uint32)((1u << Realy1_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << Realy1_USBIO_SUSPEND_DEL_SHIFT)))
    #define Realy1_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << Realy1_USBIO_SUSPEND_SHIFT)))
    #define Realy1_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << Realy1_USBIO_SUSPEND_DEL_SHIFT)))
    #define Realy1_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(Realy1__PC)
    /* Port Configuration */
    #define Realy1_PC                 (* (reg32 *) Realy1__PC)
#endif
/* Pin State */
#define Realy1_PS                     (* (reg32 *) Realy1__PS)
/* Data Register */
#define Realy1_DR                     (* (reg32 *) Realy1__DR)
/* Input Buffer Disable Override */
#define Realy1_INP_DIS                (* (reg32 *) Realy1__PC2)

/* Interrupt configuration Registers */
#define Realy1_INTCFG                 (* (reg32 *) Realy1__INTCFG)
#define Realy1_INTSTAT                (* (reg32 *) Realy1__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define Realy1_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(Realy1__SIO)
    #define Realy1_SIO_REG            (* (reg32 *) Realy1__SIO)
#endif /* (Realy1__SIO_CFG) */

/* USBIO registers */
#if !defined(Realy1__PC) && (CY_PSOC4_4200L)
    #define Realy1_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define Realy1_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define Realy1_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define Realy1_DRIVE_MODE_SHIFT       (0x00u)
#define Realy1_DRIVE_MODE_MASK        (0x07u << Realy1_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins Realy1_H */


/* [] END OF FILE */
