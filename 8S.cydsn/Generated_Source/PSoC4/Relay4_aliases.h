/*******************************************************************************
* File Name: Relay4.h  
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

#if !defined(CY_PINS_Relay4_ALIASES_H) /* Pins Relay4_ALIASES_H */
#define CY_PINS_Relay4_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define Relay4_0			(Relay4__0__PC)
#define Relay4_0_PS		(Relay4__0__PS)
#define Relay4_0_PC		(Relay4__0__PC)
#define Relay4_0_DR		(Relay4__0__DR)
#define Relay4_0_SHIFT	(Relay4__0__SHIFT)
#define Relay4_0_INTR	((uint16)((uint16)0x0003u << (Relay4__0__SHIFT*2u)))

#define Relay4_INTR_ALL	 ((uint16)(Relay4_0_INTR))


#endif /* End Pins Relay4_ALIASES_H */


/* [] END OF FILE */
