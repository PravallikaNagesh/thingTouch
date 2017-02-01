/*******************************************************************************
* File Name: main.c
*
* Version: 1.30
*
* Description:
*  The project explains the usage of CapSense CSD component. The 2 buttons and
*  linear sliders are used as sensing elements. LED displays buttons active
*  state and slider position is shown on LCD.
*
********************************************************************************
* Copyright 2013, Cypress Semiconductor Corporation. All rights reserved.
* This software is owned by Cypress Semiconductor Corporation and is protected
* by and subject to worldwide patent and copyright laws and treaties.
* Therefore, you may use this software only as provided in the license agreement
* accompanying the software package from which you obtained this software.
* CYPRESS AND ITS SUPPLIERS MAKE NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* WITH REGARD TO THIS SOFTWARE, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT,
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*******************************************************************************/

#include <main.h>
#include <CyFlash.h>

/* CapSense Slider resoluton is set to 127 (7 bits).
*  Since PWM has 16 bit resolution, slider position value should be shifted left
*  by 9 bits.
*/
#define SLIDER_POS_TO_COMPARE_SHIFT    (9u)



/*Defining a Flash Row ,that is to be used for caching the latest status*/
/* Defines second ROW from the last ROW */
#define CY_TEST_FLASH_ROW       (CY_FLASH_NUMBER_ROWS - 2u)

/* Defines absolut address of ROW */
#define CY_TEST_FLASH_ADDR      (CY_TEST_FLASH_ROW * CY_FLASH_SIZEOF_ROW)

/* Defines ID of the array to write */
#define ARRAY_ID                ((uint8)(CY_TEST_FLASH_ROW / CY_FLASH_SIZEOF_ROW))

/* Defines second ROW from the last ROW in the defined array */
#define ARRAY_ID_ROW_ADDR   ((uint16)(CY_FLASH_SIZEOF_ROW - 2u))

uint8 cache[CY_FLASH_SIZEOF_ROW]={0};
uint8 update[CY_FLASH_SIZEOF_ROW]={0};
uint16 curPos1, oldCurPos1;
uint32 sock1, but1, but2, but3, oldSock1, oldBut1, oldBut2, oldBut3;
uint8 temp1,temp2,temp3,temp4;

/* I2C slave read and write buffers */
//uint8 i2cReadBuffer [BUFFER_SIZE] = {PACKET_SOP, STS_CMD_FAIL, PACKET_EOP};
uint8 i2cReadBuffer [READ_BUFFER_SIZE]; // we need to change this value according to our pwm buffer
uint8 i2cWriteBuffer[BUFFER_SIZE];

uint8 compare[2]={0x52,0x44};
                  /*R ,D*/


int main()
{  uint8 i;
    
    /* Start UART(SCB mode)*/ 
    UART_Start();
   
    
    /* Start I2C slave (SCB mode) */
    I2C_Slave_I2CSlaveInitReadBuf(i2cReadBuffer,  3);
    I2C_Slave_I2CSlaveInitWriteBuf(i2cWriteBuffer, BUFFER_SIZE);
    I2C_Slave_Start();
   
    
    /* Enable global interrupts */
    CyGlobalIntEnable;

    /* Start PWM and CapSense components */
    LED_CONTROL1_Start();
    

    CapSense_Start();

    /* Initialize baselines */
    CapSense_InitializeAllBaselines();
    
    
    //UART_UartPutString("UPDATING THE STATUS \n\r");
    
    /* Reading from the Flash memory and setting up the widgets to previous status(status before the power down(reset))*/
    for(i=0;i<4;i++){
        update[i]=(*((uint8 *) (CY_TEST_FLASH_ADDR + i)));
    }
      
    for(i=0;i<4;i++){
       //UART_UartPutChar(update[i]); 
       //UART_UartPutString("\n\r");
       ExecuteCommand(i,update[i]);
       CyDelay(50);
    }
    //UART_UartPutString("update finihsed \n\r");
    
    //Capsense Loop
    UART_UartPutString("Main Loop Started \n");
    while(1u)
    {
        //UART_UartPutString("before I2C_Slave_I2CSlaveStatus \n");
        /* Write complete: parse command packet */
        if (0u != (I2C_Slave_I2CSlaveStatus() & I2C_Slave_I2C_SSTAT_WR_CMPLT))
        {
            UART_UartPutString("inside I2C_Slave_I2CSlaveStatus \n");
            /* Check packet length */
            if (1u <= I2C_Slave_I2CSlaveGetWriteBufSize())
            {
                UART_UartPutString("inside I2C_Slave_I2CSlaveGetWriteBufSize \n");
                UART_UartPutString("before ExecuteCommand \n");
                UART_UartPutString("String cmp pass \n");
              
                
                /*Sendind master a ready ack*/
                if(i2cWriteBuffer[0]==0x52 && i2cWriteBuffer[1]==0x44 )
                {   
                    //UART_UartPutString("ready ack sent \n\r");
                    
                    i2cReadBuffer[0] = 0x52;
                    i2cReadBuffer[1] = 0x44;
                    
                    InterPin_Write(~InterPin_Read());
                    CyDelay(100);
                    InterPin_Write(~InterPin_Read());
                    
                }
                /*Sending data according to master request*/
                else if( i2cWriteBuffer[1]==0x72)
                {   i2cReadBuffer[0] = i2cWriteBuffer[0];
                    switch(i2cWriteBuffer[1])
                    {
                        case FAN1: i2cReadBuffer[1]=update[FAN1];
                                    break;
                        case SWITCH1: i2cReadBuffer[1]=update[SWITCH1];
                                    break;
                        case SWITCH2:i2cReadBuffer[1]=update[SWITCH2];
                                    break;
                        case SWITCH3:i2cReadBuffer[1]=update[SWITCH3];
                                    break;
                        case SOCKET1:i2cReadBuffer[1]=update[SOCKET1];
                                    break;
                    }
                    InterPin_Write(~InterPin_Read());
                    CyDelay(50);
                    InterPin_Write(~InterPin_Read());
                    
                }
               else
                {
                //UART_UartPutString("excecuting the master commands \n\r");
                ExecuteCommand(i2cWriteBuffer[0], i2cWriteBuffer[1]);
                }
            }

            /* Clear slave write buffer and status */
            I2C_Slave_I2CSlaveClearWriteBuf();
            (void) I2C_Slave_I2CSlaveClearWriteStatus();
        }


        /* Read complete: expose buffer to master */
        if (0u != (I2C_Slave_I2CSlaveStatus() & I2C_Slave_I2C_SSTAT_RD_CMPLT))
        {
            /* Clear slave read buffer and status */
            I2C_Slave_I2CSlaveClearReadBuf();
            (void) I2C_Slave_I2CSlaveClearReadStatus();
        }
        
        /* Check whether the scanning of all enabled widgets is completed. */
        if(0u == CapSense_IsBusy())
        {
            /* Update all baselines */
            CapSense_UpdateEnabledBaselines();

            /* Start scanning all enabled sensors */
            CapSense_ScanEnabledWidgets();
        }

        /* Display CapSense state using LEDs */
        CapSense_DisplayState();
    }
}


void CapSense_DisplayState(void)
{
    /* Find Radial Slider Position */
    curPos1 = CapSense_GetRadialCentroidPos(CapSense_RADIALSLIDER0__RS);
  
    
    /* Switch 1 - 5 and Socket 1Control Code */
    but1 = CapSense_CheckIsSensorActive(CapSense_BUTTON0__BTN);
    but2 = CapSense_CheckIsWidgetActive(CapSense_BUTTON1__BTN);
    but3 = CapSense_CheckIsWidgetActive(CapSense_BUTTON2__BTN);
  
    sock1 = CapSense_CheckIsWidgetActive(CapSense_BUTTON3__BTN);
    
    //CyDelay(50);
    /* Switch 1 Control Code */
  if(but1==1 && temp1==1)
    {
            if(oldBut1==0)
            {
                //UART_UartPutString("in CapSense_DisplayState :: Switch 1, value 100 \n\r");
                UpdateSwitchLeds(SWITCH1, 100);
                i2cReadBuffer[1] = 100u;
                oldBut1=1;
            }
            
            else
            {
                //UART_UartPutString("in CapSense_DisplayState :: Switch 1, value 0  \n\r");
                UpdateSwitchLeds(SWITCH1, 0);
                i2cReadBuffer[1] = 0u;
                oldBut1=0;
            }
        temp1=0;
        i2cReadBuffer[0] = SWITCH1;
       
        InterPin_Write(~InterPin_Read());
        CyDelay(50);
        InterPin_Write(~InterPin_Read());
            
    }          
    else if (but1==0)
    {
        temp1=1;   
    }
        
    /* Switch 2 Control Code */ 
    if(but2==1 && temp2==1)
   {
            if(oldBut2==0)
            {
               // UART_UartPutString("in CapSense_DisplayState :: Switch 2, value 100  \n\r");
                UpdateSwitchLeds(SWITCH2, 100);
                i2cReadBuffer[1] = 100u;
                oldBut2=1;
            }
            else
            {
                //UART_UartPutString("in CapSense_DisplayState :: Switch 2, value 0  \n\r");
                UpdateSwitchLeds(SWITCH2, 0);
                i2cReadBuffer[1] = 0u;
                oldBut2=0;
                
            }
             
        temp2=0;
        i2cReadBuffer[0] = SWITCH2;
       
        InterPin_Write(~InterPin_Read());
        CyDelay(50);
        InterPin_Write(~InterPin_Read());
    }          
    else if (but2==0)
    {
        temp2=1;   
    }
    
    /* Switch 3 Control Code */
   if(but3==1 && temp3==1)
        {
            if(oldBut3==0)
            {
               // UART_UartPutString("in CapSense_DisplayState :: Switch 3, value 100   A\n\r");
                UpdateSwitchLeds(SWITCH3, 100);
                i2cReadBuffer[1] = 1u;
                oldBut3=1;
            }
            
            else
            {
               // UART_UartPutString("in CapSense_DisplayState :: Switch 3, value 0   A\n\r");
                UpdateSwitchLeds(SWITCH3, 0);
                i2cReadBuffer[1] = 0u;
                oldBut3=0;
                
            }
        temp3=0;
        i2cReadBuffer[0] = SWITCH3;
       
        InterPin_Write(~InterPin_Read());
        CyDelay(50);
        InterPin_Write(~InterPin_Read());
        }          
   else if (but3==0)
   {
    temp3=1;   
   }

/* Socket 1 Control Code */
   if(sock1==1 && temp4==1)
        {
            if(oldSock1==0)
            {
               // UART_UartPutString("in CapSense_DisplayState :: Switch 5, value 0  \n\r");
                UpdateSwitchLeds(SOCKET1, 100);
                i2cReadBuffer[1] = 100u;
                oldSock1=1;
            }
            
            else
            {
               // UART_UartPutString("in CapSense_DisplayState :: Switch 5, value 0  \n\r");
                UpdateSwitchLeds(SOCKET1, 0);
                i2cReadBuffer[1] = 0u;
                oldSock1=0;
                
            }
        temp4=0;
        i2cReadBuffer[0] = SOCKET1;
        
        InterPin_Write(~InterPin_Read());
        CyDelay(50);
        InterPin_Write(~InterPin_Read());
        }          
   else if (sock1==0)
    {
         temp4=1;   
    }
    
    /* Fan 1 Control Code */
    /* Reset position - FAN1 */
    if(curPos1 == 0xFFFFu)
    {
        curPos1 = 0u;
    }
    
    /* Move bargraph */
    if (curPos1 != oldCurPos1)
    {
       // UART_UartPutString("in CapSense_DisplayState :: Fan 1\n");
        oldCurPos1 = curPos1;
        /* Display Slider bargraph */
        if (curPos1 != 0u)
        {
            LED_CONTROL1_WriteCompare((uint32)curPos1 << SLIDER_POS_TO_COMPARE_SHIFT);       
            i2cReadBuffer[0] = FAN1;
            i2cReadBuffer[1] = curPos1;
            UpdateLedsFan1(curPos1);
            InterPin_Write(~InterPin_Read());
        }
    }
    

}


uint8 ExecuteCommand(uint32 cmd, uint32 value)
{
   // UART_UartPutString("in ExecuteCommand \n");
    uint8 status;
    status = STS_CMD_DONE;

    /* Execute received command */
    switch (cmd)
    {
        case FAN1:
           // UART_UartPutString("in ExecuteCommand :: FAN1\n");
            UpdateLedsFan1(value);
            break;
      
        case SWITCH1:
        case SWITCH2:
        case SWITCH3:
        case SOCKET1:
           // UART_UartPutString("in ExecuteCommand :: Switch & Socket\n");
            UpdateSwitchLeds(cmd, value);
            break;
        default:
           // UART_UartPutString("in ExecuteCommand :: default\n");
            status = STS_CMD_FAIL;
            break;
    }

    return (status);
}

void UpdateSwitchLeds(uint16 res, uint16 curPos)
{
   //UART_UartPutString("in UpdateSwitchLeds\n");
   
    
    switch(res)
    {
            case SWITCH1:
                if(curPos == 100)
                {
                   // UART_UartPutString("in UpdateSwitchLeds :: switch1 :: on \n");
                    LED11_Write(1);
                    Triac2_Write(1);
                    cache[1]=curPos;
                    CySysFlashWriteRow(CY_TEST_FLASH_ROW, cache);
                }
                else
                {
                   // UART_UartPutString("in UpdateSwitchLeds :: switch1 :: off \n");
                    LED11_Write(0);
                    Triac2_Write(0);
                   
                    cache[1]=curPos;
                    CySysFlashWriteRow(CY_TEST_FLASH_ROW, cache);
                }
                break;
            case SWITCH2:
                if(curPos == 100)
                {
                   // UART_UartPutString("in UpdateSwitchLeds :: switch2 :: on \n");
                    LED12_Write(1);
                    Triac3_Write(1);
                    
                    cache[2]=curPos;
                    CySysFlashWriteRow(CY_TEST_FLASH_ROW, cache);
                }
                else
                {
                   // UART_UartPutString("in UpdateSwitchLeds :: switch2 :: off \n");
                    LED12_Write(0);
                    Triac3_Write(0);
                    
                    cache[2]=curPos;
                    CySysFlashWriteRow(CY_TEST_FLASH_ROW, cache);
                }
                break;
            case SWITCH3:
                if(curPos == 100)
                {
                  //  UART_UartPutString("in UpdateSwitchLeds :: switch3 :: on \n");
                    LED13_Write(1);
                    Triac4_Write(1);
                  
                    cache[3]=curPos;
                    CySysFlashWriteRow(CY_TEST_FLASH_ROW, cache);
                }
                else
                {
                   // UART_UartPutString("in UpdateSwitchLeds :: switch3 :: off \n");
                    LED13_Write(0);
                    Triac4_Write(0);
                  
                    cache[3]=curPos;
                    CySysFlashWriteRow(CY_TEST_FLASH_ROW, cache);
                }
                break;
            
            case SOCKET1:
                if(curPos == 100)
                {
                  //  UART_UartPutString("in UpdateSwitchLeds :: socket1 :: on \n");
                    LED14_Write(1);
                    Triac5_Write(1);
                    
                    cache[4]=curPos;
                    CySysFlashWriteRow(CY_TEST_FLASH_ROW, cache);
                }
                else
                {
                   // UART_UartPutString("in UpdateSwitchLeds :: socket1 :: off \n");
                    LED14_Write(0);
                    Triac5_Write(0);
                  
                    cache[4]=curPos;
                    CySysFlashWriteRow(CY_TEST_FLASH_ROW, cache);
                }
                break;
            default:
               // UART_UartPutString("in UpdateSwitchLeds :: All :: on \n");
                LED11_Write(1);
                LED12_Write(1);
                LED13_Write(1);
                LED14_Write(1);
            
                Triac2_Write(1);
                Triac3_Write(1);
                Triac4_Write(1);
                Triac5_Write(1);
              
                break;
    }
}

// FAN - updating LED
void UpdateLedsFan1(uint16 curPos)
{
    /* Turn of all LEDs first */
    LED1_Write(0);
    LED2_Write(0);
    LED3_Write(0);
    LED4_Write(0);
    LED5_Write(0);
    Triac1_Write(0);
   // UART_UartPutString("in UpdateLedsFan2\n");
    
    if(curPos<10)
    {
        cache[0]=curPos;
        CySysFlashWriteRow(CY_TEST_FLASH_ROW, cache);
    }
       
    if (curPos > 10)
    {
     //   UART_UartPutString("in UpdateLedsFan2 :: range >10\n");
        LED1_Write(1);
        Triac1_Write(1);
        cache[0]=curPos;
        CySysFlashWriteRow(CY_TEST_FLASH_ROW, cache);
                    
    }
    
    if (curPos > 20)
    {
      //  UART_UartPutString("in UpdateLedsFan2 :: range >20\n");
        LED2_Write(1);
        Triac1_Write(1);
        cache[0]=curPos;
        CySysFlashWriteRow(CY_TEST_FLASH_ROW, cache);
                    
    }
    
    if (curPos > 40)
    {
       // UART_UartPutString("in UpdateLedsFan2 :: range >40\n");
        LED3_Write(1);
        Triac1_Write(1);
        cache[0]=curPos;
        CySysFlashWriteRow(CY_TEST_FLASH_ROW, cache);
                    
    }
    
    if (curPos > 60)
    {
       // UART_UartPutString("in UpdateLedsFan2 :: range > 60\n");
        LED4_Write(1);
        Triac1_Write(1);
        cache[0]=curPos;
        CySysFlashWriteRow(CY_TEST_FLASH_ROW, cache);
    }
    
    if (curPos > 80)
    {
       // UART_UartPutString("in UpdateLedsFan2 :: range > 80\n");
        LED5_Write(1);
        Triac1_Write(1);
        cache[0]=curPos;
        CySysFlashWriteRow(CY_TEST_FLASH_ROW, cache);
    }
}
/* [] END OF FILE */
