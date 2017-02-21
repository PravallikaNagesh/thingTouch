/*******************************************************************************
* File Name: Relay6.h  
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

#if !defined(CY_PINS_Relay6_ALIASES_H) /* Pins Relay6_ALIASES_H */
#define CY_PINS_Relay6_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define Relay6_0			(Relay6__0__PC)
#define Relay6_0_PS		(Relay6__0__PS)
#define Relay6_0_PC		(Relay6__0__PC)
#define Relay6_0_DR		(Relay6__0__DR)
#define Relay6_0_SHIFT	(Relay6__0__SHIFT)
#define Relay6_0_INTR	((uint16)((uint16)0x0003u << (Relay6__0__SHIFT*2u)))

#define Relay6_INTR_ALL	 ((uint16)(Relay6_0_INTR))


#endif /* End Pins Relay6_ALIASES_H */


/* [] END OF FILE */
