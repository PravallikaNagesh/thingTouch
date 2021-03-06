/*******************************************************************************
* File Name: Triac4.h  
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

#if !defined(CY_PINS_Triac4_ALIASES_H) /* Pins Triac4_ALIASES_H */
#define CY_PINS_Triac4_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define Triac4_0			(Triac4__0__PC)
#define Triac4_0_PS		(Triac4__0__PS)
#define Triac4_0_PC		(Triac4__0__PC)
#define Triac4_0_DR		(Triac4__0__DR)
#define Triac4_0_SHIFT	(Triac4__0__SHIFT)
#define Triac4_0_INTR	((uint16)((uint16)0x0003u << (Triac4__0__SHIFT*2u)))

#define Triac4_INTR_ALL	 ((uint16)(Triac4_0_INTR))


#endif /* End Pins Triac4_ALIASES_H */


/* [] END OF FILE */
