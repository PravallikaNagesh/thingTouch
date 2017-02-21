/*******************************************************************************
* File Name: zeroCrossing_intr.h  
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

#if !defined(CY_PINS_zeroCrossing_intr_ALIASES_H) /* Pins zeroCrossing_intr_ALIASES_H */
#define CY_PINS_zeroCrossing_intr_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define zeroCrossing_intr_0			(zeroCrossing_intr__0__PC)
#define zeroCrossing_intr_0_PS		(zeroCrossing_intr__0__PS)
#define zeroCrossing_intr_0_PC		(zeroCrossing_intr__0__PC)
#define zeroCrossing_intr_0_DR		(zeroCrossing_intr__0__DR)
#define zeroCrossing_intr_0_SHIFT	(zeroCrossing_intr__0__SHIFT)
#define zeroCrossing_intr_0_INTR	((uint16)((uint16)0x0003u << (zeroCrossing_intr__0__SHIFT*2u)))

#define zeroCrossing_intr_INTR_ALL	 ((uint16)(zeroCrossing_intr_0_INTR))


#endif /* End Pins zeroCrossing_intr_ALIASES_H */


/* [] END OF FILE */
