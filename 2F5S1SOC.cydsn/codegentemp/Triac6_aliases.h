/*******************************************************************************
* File Name: Triac6.h  
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

#if !defined(CY_PINS_Triac6_ALIASES_H) /* Pins Triac6_ALIASES_H */
#define CY_PINS_Triac6_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define Triac6_0			(Triac6__0__PC)
#define Triac6_0_PS		(Triac6__0__PS)
#define Triac6_0_PC		(Triac6__0__PC)
#define Triac6_0_DR		(Triac6__0__DR)
#define Triac6_0_SHIFT	(Triac6__0__SHIFT)
#define Triac6_0_INTR	((uint16)((uint16)0x0003u << (Triac6__0__SHIFT*2u)))

#define Triac6_INTR_ALL	 ((uint16)(Triac6_0_INTR))


#endif /* End Pins Triac6_ALIASES_H */


/* [] END OF FILE */
