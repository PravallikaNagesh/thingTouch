/*******************************************************************************
* File Name: zeroCrossing_intr.h  
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

#if !defined(CY_PINS_zeroCrossing_intr_H) /* Pins zeroCrossing_intr_H */
#define CY_PINS_zeroCrossing_intr_H

#include "cytypes.h"
#include "cyfitter.h"
#include "zeroCrossing_intr_aliases.h"


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
} zeroCrossing_intr_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   zeroCrossing_intr_Read(void);
void    zeroCrossing_intr_Write(uint8 value);
uint8   zeroCrossing_intr_ReadDataReg(void);
#if defined(zeroCrossing_intr__PC) || (CY_PSOC4_4200L) 
    void    zeroCrossing_intr_SetDriveMode(uint8 mode);
#endif
void    zeroCrossing_intr_SetInterruptMode(uint16 position, uint16 mode);
uint8   zeroCrossing_intr_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void zeroCrossing_intr_Sleep(void); 
void zeroCrossing_intr_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(zeroCrossing_intr__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define zeroCrossing_intr_DRIVE_MODE_BITS        (3)
    #define zeroCrossing_intr_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - zeroCrossing_intr_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the zeroCrossing_intr_SetDriveMode() function.
         *  @{
         */
        #define zeroCrossing_intr_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define zeroCrossing_intr_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define zeroCrossing_intr_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define zeroCrossing_intr_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define zeroCrossing_intr_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define zeroCrossing_intr_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define zeroCrossing_intr_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define zeroCrossing_intr_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define zeroCrossing_intr_MASK               zeroCrossing_intr__MASK
#define zeroCrossing_intr_SHIFT              zeroCrossing_intr__SHIFT
#define zeroCrossing_intr_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in zeroCrossing_intr_SetInterruptMode() function.
     *  @{
     */
        #define zeroCrossing_intr_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define zeroCrossing_intr_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define zeroCrossing_intr_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define zeroCrossing_intr_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(zeroCrossing_intr__SIO)
    #define zeroCrossing_intr_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(zeroCrossing_intr__PC) && (CY_PSOC4_4200L)
    #define zeroCrossing_intr_USBIO_ENABLE               ((uint32)0x80000000u)
    #define zeroCrossing_intr_USBIO_DISABLE              ((uint32)(~zeroCrossing_intr_USBIO_ENABLE))
    #define zeroCrossing_intr_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define zeroCrossing_intr_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define zeroCrossing_intr_USBIO_ENTER_SLEEP          ((uint32)((1u << zeroCrossing_intr_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << zeroCrossing_intr_USBIO_SUSPEND_DEL_SHIFT)))
    #define zeroCrossing_intr_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << zeroCrossing_intr_USBIO_SUSPEND_SHIFT)))
    #define zeroCrossing_intr_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << zeroCrossing_intr_USBIO_SUSPEND_DEL_SHIFT)))
    #define zeroCrossing_intr_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(zeroCrossing_intr__PC)
    /* Port Configuration */
    #define zeroCrossing_intr_PC                 (* (reg32 *) zeroCrossing_intr__PC)
#endif
/* Pin State */
#define zeroCrossing_intr_PS                     (* (reg32 *) zeroCrossing_intr__PS)
/* Data Register */
#define zeroCrossing_intr_DR                     (* (reg32 *) zeroCrossing_intr__DR)
/* Input Buffer Disable Override */
#define zeroCrossing_intr_INP_DIS                (* (reg32 *) zeroCrossing_intr__PC2)

/* Interrupt configuration Registers */
#define zeroCrossing_intr_INTCFG                 (* (reg32 *) zeroCrossing_intr__INTCFG)
#define zeroCrossing_intr_INTSTAT                (* (reg32 *) zeroCrossing_intr__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define zeroCrossing_intr_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(zeroCrossing_intr__SIO)
    #define zeroCrossing_intr_SIO_REG            (* (reg32 *) zeroCrossing_intr__SIO)
#endif /* (zeroCrossing_intr__SIO_CFG) */

/* USBIO registers */
#if !defined(zeroCrossing_intr__PC) && (CY_PSOC4_4200L)
    #define zeroCrossing_intr_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define zeroCrossing_intr_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define zeroCrossing_intr_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define zeroCrossing_intr_DRIVE_MODE_SHIFT       (0x00u)
#define zeroCrossing_intr_DRIVE_MODE_MASK        (0x07u << zeroCrossing_intr_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins zeroCrossing_intr_H */


/* [] END OF FILE */
