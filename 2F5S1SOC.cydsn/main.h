/*******************************************************************************
* File Name: main.h
*
* Version: 1.20
*
* Description:
*  This file provides function prototypes, constants and macros for the example
*  project.
*
********************************************************************************
* Copyright 2014-2015, Cypress Semiconductor Corporation. All rights reserved.
* This software is owned by Cypress Semiconductor Corporation and is protected
* by and subject to worldwide patent and copyright laws and treaties.
* Therefore, you may use this software only as provided in the license agreement
* accompanying the software package from which you obtained this software.
* CYPRESS AND ITS SUPPLIERS MAKE NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* WITH REGARD TO THIS SOFTWARE, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT,
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*******************************************************************************/

#if !defined(CY_MAIN_H)
#define CY_MAIN_H

#include <project.h>

struct masterData{
    int pos;
    int posData;
};
/***************************************
*         Function Prototypes
****************************************/

//uint8 ExecuteCommand(uint32 cmd);
uint8 ExecuteCommand(uint32 no, uint32 value);
void UpdateSwitchLeds(uint16 res, uint16 curPos);
void CapSense_DisplayState(void);
void UpdateLedsFan1(uint16);
void UpdateLedsFan2(uint16);
void UpdateLedsSocketSwitch(uint16, uint16);
/***************************************
*            Constants
****************************************/

#define  FQ_50      1 // in case of 50Hz
#define  FQ_60      0 // in case of 60Hz

/* Buffer and packet size */
#define WRITE_BUFFER_SIZE   (4u)
#define READ_BUFFER_SIZE    (2u)
//#define PACKET_SIZE     (BUFFER_SIZE)
#define PACKET_SIZE     (4u)

/* Packet positions */
#define PACKET_SOP_POS  (0u)
#define PACKET_CMD_POS  (1u)
#define PACKET_STS_POS  (PACKET_CMD_POS)
#define PACKET_EOP_POS  (2u)

/* Start and end of packet markers */
#define PACKET_SOP      (0x80u)
#define PACKET_EOP      (0x17u)

/* Command valid status */
#define STS_CMD_DONE    (0x00u)
#define STS_CMD_FAIL    (0xFFu)

/* Commands set */
#define CMD_SET_OFF     (0u)
#define CMD_SET_RED     (1u)
#define CMD_SET_GREEN   (2u)
//#define CMD_SET_BLUE    (3u)

// Commands for PSOC
#define FAN1            (0u)
#define FAN2            (1u)
#define SWITCH1         (2u)
#define SWITCH2         (3u)
#define SWITCH3         (4u)
#define SWITCH4         (5u)
#define SWITCH5         (6u)
#define SOCKET1         (7u)
/***************************************
*               Macros
****************************************/

/* Set LED RED color */

#define RGB_LED_ON_RED  \
                do{     \
                    LED_1_Write(0u); \
                    LED_2_Write(1u); \
                }while(0)

/* Set LED GREEN color */
#define RGB_LED_ON_GREEN \
                do{      \
                    LED_1_Write(1u); \
                    LED_2_Write(0u); \
                }while(0)

/* Set LED BLUE color */
/*
#define RGB_LED_ON_BLUE \
                do{     \
                    LED_RED_Write  (1u); \
                    LED_GREEN_Write(1u); \
                    LED_BLUE_Write (0u); \
                }while(0)
*/
/* Set LED TURN OFF */

#define RGB_LED_OFF \
                do{ \
                    LED_1_Write(1u); \
                    LED_2_Write(1u); \
                }while(0)

#endif /* (CY_MAIN_H) */


/* [] END OF FILE */
