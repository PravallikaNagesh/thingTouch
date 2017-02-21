/*******************************************************************************
* File Name: Intr.h
* Version 1.70
*
*  Description:
*   Provides the function definitions for the Interrupt Controller.
*
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/
#if !defined(CY_ISR_Intr_H)
#define CY_ISR_Intr_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void Intr_Start(void);
void Intr_StartEx(cyisraddress address);
void Intr_Stop(void);

CY_ISR_PROTO(Intr_Interrupt);

void Intr_SetVector(cyisraddress address);
cyisraddress Intr_GetVector(void);

void Intr_SetPriority(uint8 priority);
uint8 Intr_GetPriority(void);

void Intr_Enable(void);
uint8 Intr_GetState(void);
void Intr_Disable(void);

void Intr_SetPending(void);
void Intr_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the Intr ISR. */
#define Intr_INTC_VECTOR            ((reg32 *) Intr__INTC_VECT)

/* Address of the Intr ISR priority. */
#define Intr_INTC_PRIOR             ((reg32 *) Intr__INTC_PRIOR_REG)

/* Priority of the Intr interrupt. */
#define Intr_INTC_PRIOR_NUMBER      Intr__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable Intr interrupt. */
#define Intr_INTC_SET_EN            ((reg32 *) Intr__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the Intr interrupt. */
#define Intr_INTC_CLR_EN            ((reg32 *) Intr__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the Intr interrupt state to pending. */
#define Intr_INTC_SET_PD            ((reg32 *) Intr__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the Intr interrupt. */
#define Intr_INTC_CLR_PD            ((reg32 *) Intr__INTC_CLR_PD_REG)



#endif /* CY_ISR_Intr_H */


/* [] END OF FILE */
