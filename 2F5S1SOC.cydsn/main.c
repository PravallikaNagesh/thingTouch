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

/* WDT counter configuration */
#define WDT_COUNT0_MATCH    (0x0FFFu)
#define WDT_COUNT1_MATCH    (0x0008u)

/* Prototype of WDT ISR */
CY_ISR_PROTO(WdtIsrHandler);

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
uint16 curPos1, oldCurPos1, curPos2, oldCurPos2;
uint32 sock1, but1, but2, but3, but4, but5, oldSock1, oldBut1, oldBut2, oldBut3, oldBut4, oldBut5;
uint8 temp1,temp2,temp3,temp4,temp5,temp6;

/* I2C slave read and write buffers */
//uint8 i2cReadBuffer [BUFFER_SIZE] = {PACKET_SOP, STS_CMD_FAIL, PACKET_EOP};
uint8 i2cReadBuffer [READ_BUFFER_SIZE]; // we need to change this value according to our pwm buffer
uint8 i2cWriteBuffer[WRITE_BUFFER_SIZE];

uint8 compare[2]={0x52,0x44};
                  /*R ,D*/

/*******************************************************************************
* Function Name: main
********************************************************************************
* Summary:
*  Main function performs following functions:
*   1. Enable global interrupts.
*   2. Initialize CapSense CSD and Start the sensor scanning loop.
*   3. Process scanning results and display it on with LEDs.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
int main()
{   uint8 i;
    
    /* Start UART(SCB mode)*/ 
    UART_Start();
    
    /* Start I2C slave (SCB mode) */
    I2C_Slave_I2CSlaveInitReadBuf(i2cReadBuffer, READ_BUFFER_SIZE);
    I2C_Slave_I2CSlaveInitWriteBuf(i2cWriteBuffer, WRITE_BUFFER_SIZE);
    I2C_Slave_Start();
   
    /* Enable global interrupts */
    CyGlobalIntEnable;

    /* Start PWM and CapSense components */
    LED_CONTROL1_Start();
    LED_CONTROL2_Start();

    CapSense_Start();

    /* Initialize baselines */
    CapSense_InitializeAllBaselines();
   
    //UART_UartPutString("UPDATING THE STATUS \n\r");
    
    /* Reading from the Flash memory and setting up the widgets to previous status(status before the power down(reset))*/
    for(i=0;i<8;i++){
        update[i]=(*((uint8 *) (CY_TEST_FLASH_ADDR + i)));
    }
      
    for(i=0;i<8;i++){
       //UART_UartPutChar(update[i]); 
       //UART_UartPutString("\n\r");
       ExecuteCommand(i,update[i]);
       CyDelay(50);
    }
    //UART_UartPutString("update finihsed \n\r");
    
    
    //Capsense Loop
    //UART_UartPutString("Main Loop Started \n\r");
    while(1u)
    {   //CyDelay(100);
        //UART_UartPutString("before I2C_Slave_I2CSlaveStatus \n\r");
        
        /* Write complete: parse command packet */
        if (0u != (I2C_Slave_I2CSlaveStatus() & I2C_Slave_I2C_SSTAT_WR_CMPLT))
        {
           // UART_UartPutString("inside I2C_Slave_I2CSlaveStatus \n\r");
            
            /* Check packet length */
            if (1u <= I2C_Slave_I2CSlaveGetWriteBufSize())
            {
                //UART_UartPutString("inside I2C_Slave_I2CSlaveGetWriteBufSize \n\r");
                //UART_UartPutString("before ExecuteCommand \n\r");
                //UART_UartPutString("String cmp pass \n\r");
                
                /*Sendind master a ready ack*/
                char8 temp = i2cWriteBuffer[0];
                char8 temp2 = i2cWriteBuffer[1];
                    UART_UartPutString(&temp);
                    UART_UartPutString(&temp2);
                
                if(i2cWriteBuffer[0]==0x52 && i2cWriteBuffer[1]==0x53 )
                {   
                   // UART_UartPutString("reset\r\n");
                   /* Setup ISR for interrupts at WDT counter 0 events. */
                    WdtIsr_StartEx(WdtIsrHandler);

	                /* Set WDT counter 0 to generate interrupt on match */
	                CySysWdtWriteMode(CY_SYS_WDT_COUNTER0, CY_SYS_WDT_MODE_INT);
	                CySysWdtWriteMatch(CY_SYS_WDT_COUNTER0, WDT_COUNT0_MATCH);
	                CySysWdtWriteClearOnMatch(CY_SYS_WDT_COUNTER0, 1u);
	
	                /* Enable WDT counters 0 and 1 cascade */
	                CySysWdtWriteCascade(CY_SYS_WDT_CASCADE_01);
    
	                /* Set WDT counter 1 to generate reset on match */
	                CySysWdtWriteMatch(CY_SYS_WDT_COUNTER1, WDT_COUNT1_MATCH);
	                CySysWdtWriteMode(CY_SYS_WDT_COUNTER1, CY_SYS_WDT_MODE_RESET);
                    CySysWdtWriteClearOnMatch(CY_SYS_WDT_COUNTER1, 1u);
	
	                /* Enable WDT counters 0 and 1 */
	                CySysWdtEnable(CY_SYS_WDT_COUNTER0_MASK | CY_SYS_WDT_COUNTER1_MASK);
	
	                /* Lock WDT registers and try to disable WDT counters 0 and 1 */
	                CySysWdtLock();
	                CySysWdtDisable(CY_SYS_WDT_COUNTER1_MASK);
	                CySysWdtUnlock();
                    
                }
                
                
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
                        case FAN2: i2cReadBuffer[1]=update[FAN2];
                                    break;
                        case SWITCH1:i2cReadBuffer[1]=update[SWITCH1];
                                    break;
                        case SWITCH2:i2cReadBuffer[1]=update[SWITCH2];
                                    break;
                        case SWITCH3:i2cReadBuffer[1]=update[SWITCH3];
                                    break;
                        case SWITCH4:i2cReadBuffer[1]=update[SWITCH4];
                                    break;
                        case SWITCH5:i2cReadBuffer[1]=update[SWITCH5];
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
       
        {   // UART_UartPutString("Capsense");
            /* Update all baselines */
            CapSense_UpdateEnabledBaselines();

            /* Start scanning all enabled sensors */
            CapSense_ScanEnabledWidgets();
        }

        /* Display CapSense state using LEDs */
        //UART_UartPutString("touch on the panel \n\r");
        CapSense_DisplayState();
       
    }
}

/*******************************************************************************
* Function Name: CapSense_DisplayState
********************************************************************************
* Summary:
*  Changes LEDs brightness by changing the duty cycle of PWM signals.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void CapSense_DisplayState(void)
{
    /* Find Radial Slider Position */
    curPos1 = CapSense_GetRadialCentroidPos(CapSense_RADIALSLIDER0__RS);
    curPos2 = CapSense_GetRadialCentroidPos(CapSense_RADIALSLIDER1__RS);
    
    /* Switch 1 - 5 and Socket 1Control Code */
    but1 = CapSense_CheckIsSensorActive(CapSense_BUTTON0__BTN);
    but2 = CapSense_CheckIsWidgetActive(CapSense_BUTTON1__BTN);
    but3 = CapSense_CheckIsWidgetActive(CapSense_BUTTON2__BTN);
    but4 = CapSense_CheckIsWidgetActive(CapSense_BUTTON3__BTN);
    but5 = CapSense_CheckIsWidgetActive(CapSense_BUTTON4__BTN);
    sock1= CapSense_CheckIsWidgetActive(CapSense_BUTTON5__BTN);
    
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
        
   /* Switch 4 Control Code */      
  if(but4==1 && temp4==1)
        {
            if(oldBut4==0)
            {
               // UART_UartPutString("in CapSense_DisplayState :: Switch 4, value 100 \n\r");
                UpdateSwitchLeds(SWITCH4, 100);
                i2cReadBuffer[1] = 100u;
                oldBut4=1;
            }
            
            else
            {
               // UART_UartPutString("in CapSense_DisplayState :: Switch 4, value 0  \n\r");
                UpdateSwitchLeds(SWITCH4, 0);
                i2cReadBuffer[1] = 0u;
                oldBut4=0;
                
            }
        temp4=0;
        i2cReadBuffer[0] = SWITCH4;
    
        InterPin_Write(~InterPin_Read());
        CyDelay(50);
        InterPin_Write(~InterPin_Read());
         
        }          
   else if (but4==0)
    {
      temp4=1;   
    }

    /* Switch 5 Control Code */
    
   if(but5==1 && temp5==1)
   {
            if(oldBut5==0)
            {
               // UART_UartPutString("in CapSense_DisplayState :: Switch 5, value 0  \n\r");
                UpdateSwitchLeds(SWITCH5, 100);
                i2cReadBuffer[1] = 100u;
                oldBut5=1;
            }
            
            else
            {
               // UART_UartPutString("in CapSense_DisplayState :: Switch 5, value 0  \n\r");
                UpdateSwitchLeds(SWITCH5, 0);
                i2cReadBuffer[1] = 0u;
                oldBut5=0;
                
            }
        temp5=0;
        i2cReadBuffer[0] = SWITCH5;
        
        InterPin_Write(~InterPin_Read());
        CyDelay(50);
        InterPin_Write(~InterPin_Read());
        }          
   else if (but5==0)
   {
         temp5=1;   
   }

   /* Socket 1 Control Code */
   if(sock1==1 && temp6==1)
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
        temp6=0;
        i2cReadBuffer[0] = SOCKET1;
        
        InterPin_Write(~InterPin_Read());
        CyDelay(50);
        InterPin_Write(~InterPin_Read());
        }          
   else if (sock1==0)
    {
         temp6=1;   
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
       //UART_UartPutString("in CapSense_DisplayState :: Fan 1\n");
        
        oldCurPos1 = curPos1;
        /* Display Slider bargraph */
        if (curPos1 != 0u)
        {
            LED_CONTROL1_WriteCompare((uint32)curPos1 << SLIDER_POS_TO_COMPARE_SHIFT);       
            i2cReadBuffer[0] = FAN1;
            i2cReadBuffer[1] = curPos1;
            UpdateLedsFan1(curPos1);
            InterPin_Write(~InterPin_Read());
            CyDelay(50);
            InterPin_Write(~InterPin_Read());
        }
        else{
           //UART_UartPutString("in CapSense_DisplayState :: Fan 1\n");
        }
    }
    
    /* Fan 2 Control Code */ 
    
    /* Reset position - FAN2*/
    if(curPos2 == 0xFFFFu)
    {
        curPos2 = 0u;
    }

    /* Move bargraph */
    if (curPos2 != oldCurPos2)
    {
        //UART_UartPutString("in CapSense_DisplayState :: Fan 1\n");
        oldCurPos2 = curPos2;
        /* Display Slider bargraph */
        if (curPos2 != 0u)
        {
            LED_CONTROL2_WriteCompare((uint32)curPos2 << SLIDER_POS_TO_COMPARE_SHIFT);
            UpdateLedsFan2(curPos2);
           
            i2cReadBuffer[0] = FAN2;
            i2cReadBuffer[1] = curPos2;
            InterPin_Write(~InterPin_Read());
            CyDelay(50);
            InterPin_Write(~InterPin_Read());
        }
    }
   
}

/*******************************************************************************
* Function Name: ExecuteCommand
********************************************************************************
* Summary:
*  Executes received command to control the LED color and returns status.
*  If the command is unknown, the LED color is not changed.
*
* Parameters:
*  cmd: command to execute. Available commands:
*   - CMD_SET_RED:   set red color of the LED.
*   - CMD_SET_GREEN: set green color of the LED.
*   - CMD_SET_BLUE:  set blue color of the LED.
*   - CMD_SET_OFF:   turn off the LED.
*
* Return:
*  Returns status of command execution. There are two statuses
*  - STS_CMD_DONE: command is executed successfully.
*  - STS_CMD_FAIL: unknown command
*
*******************************************************************************/
uint8 ExecuteCommand(uint32 cmd, uint32 value)
{
    //UART_UartPutString("in ExecuteCommand \n");
    uint8 status;
    status = STS_CMD_DONE;

    /* Execute received command */
    switch (cmd)
    {
        case FAN1:
         //   UART_UartPutString("in ExecuteCommand :: FAN1\n");
            UpdateLedsFan1(value);
            break;
        case FAN2:
           // UART_UartPutString("in ExecuteCommand :: FAN2\n");
            UpdateLedsFan2(value);
            break;
        case SWITCH1:
        case SWITCH2:
        case SWITCH3:
        case SWITCH4:
        case SWITCH5:
        case SOCKET1:
            //UART_UartPutString("in ExecuteCommand :: Switch & Socket\n");
            UpdateSwitchLeds(cmd, value);
            break;
        default:
            //UART_UartPutString("in ExecuteCommand :: default\n");
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
                    //UART_UartPutString("in UpdateSwitchLeds :: switch1 :: on \n");
                    LED11_Write(1);
                    Triac3_Write(1);
                                      
                    cache[2]=curPos;
                    CySysFlashWriteRow(CY_TEST_FLASH_ROW, cache);
                    
                }
                else
                {
                    //UART_UartPutString("in UpdateSwitchLeds :: switch1 :: off \n");
                    LED11_Write(0);
                    Triac3_Write(0);
                                       
                    cache[2]=curPos;
                    CySysFlashWriteRow(CY_TEST_FLASH_ROW, cache);
                }
                break;
            case SWITCH2:
                if(curPos == 100 )
                {
                    //UART_UartPutString("in UpdateSwitchLeds :: switch2 :: on \n");
                    LED12_Write(1);
                    Triac4_Write(1);
                                        
                    cache[3]=curPos;
                    CySysFlashWriteRow(CY_TEST_FLASH_ROW, cache);
                    
                }
                else
                {
                    //UART_UartPutString("in UpdateSwitchLeds :: switch2 :: off \n");
                    LED12_Write(0);
                    Triac4_Write(0);
                                       
                    cache[3]=curPos;
                    CySysFlashWriteRow(CY_TEST_FLASH_ROW, cache);
                    
                }
                break;
            case SWITCH3:
                if(curPos == 0x100 )
                {
                    //UART_UartPutString("in UpdateSwitchLeds :: switch3 :: on \n");
                    LED13_Write(1);
                    Triac5_Write(1);
                                       
                    cache[4]=curPos;
                    CySysFlashWriteRow(CY_TEST_FLASH_ROW, cache);
                    
                }
                else
                {
                    //UART_UartPutString("in UpdateSwitchLeds :: switch3 :: off \n");
                    LED13_Write(0);
                    Triac5_Write(0);
                                       
                    cache[4]=curPos;
                    CySysFlashWriteRow(CY_TEST_FLASH_ROW, cache);
                   
                }
                break;
            case SWITCH4:
                if(curPos == 100 )
                {
                    //UART_UartPutString("in UpdateSwitchLeds :: switch4 :: on \n");
                    LED14_Write(1);
                    Triac6_Write(1);
                                      
                    cache[5]=curPos;
                    CySysFlashWriteRow(CY_TEST_FLASH_ROW, cache); 
                }
                   
                else
                {
                    //UART_UartPutString("in UpdateSwitchLeds :: switch4 :: off \n");
                    LED14_Write(0);
                    Triac6_Write(0);
                  
                    cache[5]=curPos;
                    CySysFlashWriteRow(CY_TEST_FLASH_ROW, cache);
                }   
                   
                break;
            case SWITCH5:
                if(curPos == 100)
                {
                   // UART_UartPutString("in UpdateSwitchLeds :: switch5 :: on \n");
                    LED15_Write(1);
                    Triac7_Write(1);
                                        
                    cache[6]=curPos;
                    CySysFlashWriteRow(CY_TEST_FLASH_ROW, cache);
                    
                }
                else
                {
                    //UART_UartPutString("in UpdateSwitchLeds :: switch5 :: off \n");
                    LED15_Write(0);
                    Triac7_Write(0);
                                       
                    cache[6]=curPos;
                    CySysFlashWriteRow(CY_TEST_FLASH_ROW, cache);
                    
                }
                break;
            case SOCKET1:
                if(curPos == 100 )
                {
                    //UART_UartPutString("in UpdateSwitchLeds :: socket1 :: on \n");
                    LED16_Write(1);
                    Triac8_Write(1);
                                       
                    cache[7]=curPos;
                    CySysFlashWriteRow(CY_TEST_FLASH_ROW, cache);
                   
                }
                else
                {
                    //UART_UartPutString("in UpdateSwitchLeds :: socket1 :: off \n");
                    LED16_Write(0);
                    Triac8_Write(0);
                                       
                    cache[7]=curPos;
                    CySysFlashWriteRow(CY_TEST_FLASH_ROW, cache);
                   
                }
                break;
            default:
                //UART_UartPutString("in UpdateSwitchLeds :: All :: on \n");
                LED11_Write(1);
                LED12_Write(1);
                LED13_Write(1);
                LED14_Write(1);
                LED15_Write(1);
                LED16_Write(1);
                Triac3_Write(1);
                Triac4_Write(1);
                Triac5_Write(1);
                Triac6_Write(1);
                Triac7_Write(1);
                Triac8_Write(1);
                break;
    }
}

// Fan 1 - updating LED
void UpdateLedsFan1(uint16 curPos)
{
    /* Turn of all LEDs first */
    LED6_Write(0);
    LED7_Write(0);
    LED8_Write(0);
    LED9_Write(0);
    LED10_Write(0);
    Triac1_Write(0);
    cache[0]=0;
    CySysFlashWriteRow(CY_TEST_FLASH_ROW, cache);
    
    //UART_UartPutString("in UpdateLedsFan1\n");
    
    if(curPos<0)
    {
        cache[0]=0;
     CySysFlashWriteRow(CY_TEST_FLASH_ROW, cache);
    
    }
    if (curPos > 10)
    {
        //UART_UartPutString("in UpdateLedsFan1 :: range >10\n");
        LED6_Write(1);
        Triac1_Write(1);
        
        cache[0]=curPos;
        CySysFlashWriteRow(CY_TEST_FLASH_ROW, cache);
        
    }
    
    if (curPos > 20)
    {
        //UART_UartPutString("in UpdateLedsFan1 :: range >20\n");
        LED7_Write(1);
        Triac1_Write(1);
        
        cache[0]=curPos;
        CySysFlashWriteRow(CY_TEST_FLASH_ROW, cache);
    }
    
    if (curPos > 40)
    {
       // UART_UartPutString("in UpdateLedsFan1 :: range >40\n");
        LED8_Write(1);
        Triac1_Write(1);
       
        cache[0]=curPos;
        CySysFlashWriteRow(CY_TEST_FLASH_ROW, cache);
    }
    
    if (curPos > 60)
    {
       // UART_UartPutString("in UpdateLedsFan1 :: range > 60\n");
        LED9_Write(1);
        Triac1_Write(1);
        
        cache[0]=curPos;
        CySysFlashWriteRow(CY_TEST_FLASH_ROW, cache);
    }
    
    if (curPos > 80)
    {
        //UART_UartPutString("in UpdateLedsFan1 :: range > 80\n");
        LED10_Write(1);
        Triac1_Write(1);
        
        cache[0]=curPos;
        CySysFlashWriteRow(CY_TEST_FLASH_ROW, cache);
        
    }
}

// FAN2 - updating LED
void UpdateLedsFan2(uint16 curPos)
{
    /* Turn of all LEDs first */
    LED1_Write(0);
    LED2_Write(0);
    LED3_Write(0);
    LED4_Write(0);
    LED5_Write(0);
    Triac2_Write(0);
    //UART_UartPutString("in UpdateLedsFan2\n");
       
    if (curPos > 10)
    {
      //  UART_UartPutString("in UpdateLedsFan2 :: range >10\n");
        LED1_Write(1);
        Triac2_Write(1);
        
        cache[1]=curPos;
        CySysFlashWriteRow(CY_TEST_FLASH_ROW, cache);
        
    }
    
    if (curPos > 20)
    {
      //  UART_UartPutString("in UpdateLedsFan2 :: range >20\n");
        LED2_Write(1);
        Triac2_Write(1);
        
        cache[1]=curPos;
        CySysFlashWriteRow(CY_TEST_FLASH_ROW, cache);
    }
    
    if (curPos > 40)
    {
      //  UART_UartPutString("in UpdateLedsFan2 :: range >40\n");
        LED3_Write(1);
        Triac2_Write(1);
        
        cache[1]=curPos;
        CySysFlashWriteRow(CY_TEST_FLASH_ROW, cache);
   }
    
    if (curPos > 60)
    {
       // UART_UartPutString("in UpdateLedsFan2 :: range > 60\n");
        LED4_Write(1);
        Triac2_Write(1);
        
        cache[1]=curPos;
        CySysFlashWriteRow(CY_TEST_FLASH_ROW, cache);
    }
    
    if (curPos > 80)
    {
       // UART_UartPutString("in UpdateLedsFan2 :: range > 80\n");
        LED5_Write(1);
        Triac2_Write(1);
        
        cache[1]=curPos;
        CySysFlashWriteRow(CY_TEST_FLASH_ROW, cache);
    }
}
CY_ISR(WdtIsrHandler)
{ 
    UART_UartPutString("in WDT ISR\n\r");
    /* Clear interrupts state */
	CySysWdtClearInterrupt(CY_SYS_WDT_COUNTER0_INT);
    WdtIsr_ClearPending();
}
/* [] END OF FILE */
