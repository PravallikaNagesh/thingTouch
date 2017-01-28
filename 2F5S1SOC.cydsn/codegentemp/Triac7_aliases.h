/*******************************************************************************
* File Name: Triac7.h  
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

#if !defined(CY_PINS_Triac7_ALIASES_H) /* Pins Triac7_ALIASES_H */
#define CY_PINS_Triac7_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define Triac7_0			(Triac7__0__PC)
#define Triac7_0_PS		(Triac7__0__PS)
#define Triac7_0_PC		(Triac7__0__PC)
#define Triac7_0_DR		(Triac7__0__DR)
#define Triac7_0_SHIFT	(Triac7__0__SHIFT)
#define Triac7_0_INTR	((uint16)((uint16)0x0003u << (Triac7__0__SHIFT*2u)))

#define Triac7_INTR_ALL	 ((uint16)(Triac7_0_INTR))


#endif /* End Pins Triac7_ALIASES_H */


/* [] END OF FILE */
