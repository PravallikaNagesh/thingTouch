/*******************************************************************************
* File Name: InterPin.h  
* Version 2.20
*
* Description:
*  This file contains the Alias definitions for Per-Pin APIs in cypins.h. 
*  Information on using these APIs can be found in the System Reference Guide.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_InterPin_ALIASES_H) /* Pins InterPin_ALIASES_H */
#define CY_PINS_InterPin_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define InterPin_0			(InterPin__0__PC)
#define InterPin_0_PS		(InterPin__0__PS)
#define InterPin_0_PC		(InterPin__0__PC)
#define InterPin_0_DR		(InterPin__0__DR)
#define InterPin_0_SHIFT	(InterPin__0__SHIFT)
#define InterPin_0_INTR	((uint16)((uint16)0x0003u << (InterPin__0__SHIFT*2u)))

#define InterPin_INTR_ALL	 ((uint16)(InterPin_0_INTR))


#endif /* End Pins InterPin_ALIASES_H */


/* [] END OF FILE */
