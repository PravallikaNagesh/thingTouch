/*******************************************************************************
* File Name: Realy1.h  
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

#if !defined(CY_PINS_Realy1_ALIASES_H) /* Pins Realy1_ALIASES_H */
#define CY_PINS_Realy1_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define Realy1_0			(Realy1__0__PC)
#define Realy1_0_PS		(Realy1__0__PS)
#define Realy1_0_PC		(Realy1__0__PC)
#define Realy1_0_DR		(Realy1__0__DR)
#define Realy1_0_SHIFT	(Realy1__0__SHIFT)
#define Realy1_0_INTR	((uint16)((uint16)0x0003u << (Realy1__0__SHIFT*2u)))

#define Realy1_INTR_ALL	 ((uint16)(Realy1_0_INTR))


#endif /* End Pins Realy1_ALIASES_H */


/* [] END OF FILE */
