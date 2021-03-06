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

uint32 but1, but2, but3, but4, but5, but6, but7, but8;
uint8  butPress1,  butPress2,  butPress3,  butPress4,  butPress5,  butPress6,  butPress7,  butPress8 ;

/* I2C slave read and write buffers */
uint8 i2cReadBuffer [READ_BUFFER_SIZE]; 
uint8 i2cWriteBuffer[BUFFER_SIZE];

int main()
{   uint8 i;
    
    /* Start UART(SCB mode)*/ 
    UART_Start();
    
    /* Start I2C slave (SCB mode) */
    I2C_Slave_I2CSlaveInitReadBuf(i2cReadBuffer,  3);
    I2C_Slave_I2CSlaveInitWriteBuf(i2cWriteBuffer, BUFFER_SIZE);
    I2C_Slave_Start();
   
    /* Enable global interrupts */
    CyGlobalIntEnable;
    
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
       CyDelay(20);
    }
    //UART_UartPutString("update finihsed \n\r");
    
    //Capsense Loop
    UART_UartPutString("Main Loop Started \n");
    while(1u)
    {
        /* Write complete: parse command packet */
        if (0u != (I2C_Slave_I2CSlaveStatus() & I2C_Slave_I2C_SSTAT_WR_CMPLT))
        {
            UART_UartPutString("inside I2C_Slave_I2CSlaveStatus \n");
            /* Check packet length */
            if (1u <= I2C_Slave_I2CSlaveGetWriteBufSize())
            {
                UART_UartPutString("inside I2C_Slave_I2CSlaveGetWriteBufSize \n");
                //UART_UartPutString("before ExecuteCommand \n");
                //UART_UartPutString("String cmp pass \n");
                
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
                {   
                    i2cReadBuffer[0] = i2cWriteBuffer[0];
                    for(i=0;i<8;i++){
                        update[i]=(*((uint8 *) (CY_TEST_FLASH_ADDR + i)));
                    }
                    switch(i2cWriteBuffer[1])
                    {
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
                        case SWITCH6:i2cReadBuffer[1]=update[SWITCH6];
                                    break;
                        case SWITCH7:i2cReadBuffer[1]=update[SWITCH7];
                                    break;
                        case SWITCH8:i2cReadBuffer[1]=update[SWITCH8];
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

uint8 ExecuteCommand(uint32 cmd, uint32 value)
{
    //UART_UartPutString("in ExecuteCommand \n");
    uint8 status;
    status = STS_CMD_DONE;

    /* Execute received command */
    if(cmd)
    {
       UpdateSwitchLeds(cmd,value);
    }
    else
    {
        //UART_UartPutString("in ExecuteCommand :: default\n");
        status = STS_CMD_FAIL;
    }
    return (status);
}



void CapSense_DisplayState(void)
{
    //UART_UartPutString("inside capsense display\n\r");
    /* Switch 1 - 8 */
    but1 = CapSense_CheckIsSensorActive(CapSense_BUTTON0__BTN);
    but2 = CapSense_CheckIsWidgetActive(CapSense_BUTTON1__BTN);
    but3 = CapSense_CheckIsWidgetActive(CapSense_BUTTON2__BTN);
    but4 = CapSense_CheckIsWidgetActive(CapSense_BUTTON3__BTN);
    but5 = CapSense_CheckIsWidgetActive(CapSense_BUTTON4__BTN);
    but6 = CapSense_CheckIsWidgetActive(CapSense_BUTTON5__BTN);
    but7 = CapSense_CheckIsWidgetActive(CapSense_BUTTON6__BTN);
    but8 = CapSense_CheckIsWidgetActive(CapSense_BUTTON7__BTN);
       
    // Switch 1 Control Code  
    if(but1==1 && butPress1==1)
        {
            LED1_Write(~LED1_Read());
            i2cReadBuffer[0] = SWITCH1;
            if(LED1_Read())
            {     
                Relay1_Write(1);
                i2cReadBuffer[1] =100u;
                cache[0]=100;
                CySysFlashWriteRow(CY_TEST_FLASH_ROW, cache); 
            }
            else
            { 
                Relay1_Write(0);
                i2cReadBuffer[1] = 0u;
                cache[0]=0;
                CySysFlashWriteRow(CY_TEST_FLASH_ROW, cache); 
        }
        butPress1 =0;
         
        InterPin_Write(~InterPin_Read());
        CyDelay(50);
        InterPin_Write(~InterPin_Read());
    }          
    else if (but1==0 && butPress1==0 )
    {
        butPress1=1;   
    }
  
    // Switch 2 Control Code  
    if(but2==1 && butPress2==1)
    {
        LED2_Write(~LED2_Read());
        i2cReadBuffer[0] = SWITCH2;
        if(LED2_Read())
        {       
            Relay2_Write(1);
            i2cReadBuffer[1] =100u;
            cache[1]=100;
            CySysFlashWriteRow(CY_TEST_FLASH_ROW, cache); 
        }
        else
        { 
            Relay2_Write(0);
            i2cReadBuffer[1] = 0u;
            cache[1]=0;
            CySysFlashWriteRow(CY_TEST_FLASH_ROW, cache); 
        }
        butPress2 =0;
        InterPin_Write(~InterPin_Read());
        CyDelay(50);
        InterPin_Write(~InterPin_Read());
    }          
    else if (but2==0 && butPress2==0)
    {
        butPress2=1;   
    }
    
    // Switch 3 Control Code 
    if(but3==1 && butPress3==1)
    {
        LED3_Write(~LED3_Read());
        i2cReadBuffer[0] = SWITCH3;
        if(LED3_Read())
        {     
            Relay3_Write(1);
            i2cReadBuffer[1] =100u;
            cache[2]=100;
            CySysFlashWriteRow(CY_TEST_FLASH_ROW, cache); 
        }
        else
        { 
            Relay3_Write(0);
            i2cReadBuffer[1] = 0u;
            cache[2]=0;
            CySysFlashWriteRow(CY_TEST_FLASH_ROW, cache); 
        }
        butPress3 =0;
         
        InterPin_Write(~InterPin_Read());
        CyDelay(50);
        InterPin_Write(~InterPin_Read());
    }          
    else if (but3==0 && butPress3==0)
    {
        butPress3=1;   
    }
        
    // Switch 4 Control Code     
    if(but4==1 && butPress4==1)
    {
        LED4_Write(~LED4_Read());
        i2cReadBuffer[0] = SWITCH4;
        if(LED4_Read())
        {     
            Relay4_Write(1);
            i2cReadBuffer[1] =100u;
            cache[3]=100;
            CySysFlashWriteRow(CY_TEST_FLASH_ROW, cache); 
        }
        else
        {
            Relay4_Write(0);
            i2cReadBuffer[1] = 0u;
            cache[3]=0;
            CySysFlashWriteRow(CY_TEST_FLASH_ROW, cache); 
        }
         butPress4=0;
         
        InterPin_Write(~InterPin_Read());
        CyDelay(50);
        InterPin_Write(~InterPin_Read());
    }          
    else if (but4==0 && butPress4==0 )
    {
        butPress4=1;   
    }
    
    //Switch 5 Control Code 
    if(but5==1 && butPress5==1)
    {
        LED5_Write(~LED5_Read());
        i2cReadBuffer[0] = SWITCH5;
        if(LED5_Read())
        {     
            Relay5_Write(1);
            i2cReadBuffer[1] =100u;
            cache[4]=100;
            CySysFlashWriteRow(CY_TEST_FLASH_ROW, cache); 
        }
        else
        { 
            Relay5_Write(0);
            i2cReadBuffer[1] = 0u;
            cache[4]=0;
            CySysFlashWriteRow(CY_TEST_FLASH_ROW, cache); 
        }
        butPress5 =0;
         
        InterPin_Write(~InterPin_Read());
        CyDelay(50);
        InterPin_Write(~InterPin_Read());
        }          
    else if (but5==0  && butPress5==0 )
    {
        butPress5=1;   
    }
    
    //Switch 6 Control Code 
    if(but6==1 && butPress6==1)
    {
        LED6_Write(~LED6_Read());
        i2cReadBuffer[0] = SWITCH6;
        if(LED6_Read())
        {   
            Relay6_Write(1);
            i2cReadBuffer[1] =100u;
            cache[5]=100;
            CySysFlashWriteRow(CY_TEST_FLASH_ROW, cache); 
        }
        else
        { 
            Relay6_Write(0);
            i2cReadBuffer[1] = 0u;
            cache[5]=0;
            CySysFlashWriteRow(CY_TEST_FLASH_ROW, cache); 
            
        }
        butPress6 =0;
         
        InterPin_Write(~InterPin_Read());
        CyDelay(50);
        InterPin_Write(~InterPin_Read());
    }          
    else if (but6==0 && butPress6==0 )
    {
        butPress6=1;   
    }
    
    // Switch 7 Control Code 
    if(but7==1 && butPress7==1)
    {
        LED7_Write(~LED7_Read());
        i2cReadBuffer[0] = SWITCH7;
        if(LED7_Read())
        {     
            Triac1_Write(1);
            i2cReadBuffer[1] =100u;
            cache[6]=100;
            CySysFlashWriteRow(CY_TEST_FLASH_ROW, cache); 
        }
        else
        { 
            Triac1_Write(0);
            i2cReadBuffer[1] = 0u;
             cache[6]=0;
             CySysFlashWriteRow(CY_TEST_FLASH_ROW, cache); 
            
        }
        butPress1 =0;
         
        InterPin_Write(~InterPin_Read());
        CyDelay(50);
        InterPin_Write(~InterPin_Read());
        }          
    else if (but7==0 && butPress7==0)
    {
        butPress7=1;   
    }
    
    // Switch 8 Control Code 
    if(but8==1 && butPress8==1)
    {
        LED8_Write(~LED8_Read());
        i2cReadBuffer[0] = SWITCH8;
        if(LED8_Read())
        {   
            Triac2_Write(1);
            i2cReadBuffer[1] =100u;
            cache[7]=100;
            CySysFlashWriteRow(CY_TEST_FLASH_ROW, cache); 
        }
        else
        { 
			Triac2_Write(0);
			i2cReadBuffer[1] = 0u;
			cache[7]=0;
			CySysFlashWriteRow(CY_TEST_FLASH_ROW, cache); 
        }
        butPress8 =0;
         
        InterPin_Write(~InterPin_Read());
        CyDelay(50);
        InterPin_Write(~InterPin_Read());
        }          
    else if (but8==0 && butPress8==0 )
    {
        butPress8=1;   
    }
}

void UpdateSwitchLeds(uint16 res, uint16 curPos)
{
    /* Turn of all LEDs first */
    //UART_UartPutString("in UpdateSwitchLeds\n");
    switch(res)
    {
        case SWITCH1:
                if(curPos == 100)
                {
                    //UART_UartPutString("in UpdateSwitchLeds :: switch1 :: on \n");
                    LED1_Write(1);
                    Relay1_Write(1);
                    cache[0]=curPos;
                    CySysFlashWriteRow(CY_TEST_FLASH_ROW, cache);
                }
                else
                {
                    //UART_UartPutString("in UpdateSwitchLeds :: switch1 :: off \n");
                    LED1_Write(0);
                    Relay1_Write(0);
                    cache[0]=curPos;
                    CySysFlashWriteRow(CY_TEST_FLASH_ROW, cache);
                    
                }
                break;
        case SWITCH2:
                if(curPos == 100 )
                {
                    //UART_UartPutString("in UpdateSwitchLeds :: switch2 :: on \n");
                    LED2_Write(1);
                    Relay2_Write(1);
                    cache[1]=curPos;
                    CySysFlashWriteRow(CY_TEST_FLASH_ROW, cache);
                  
                }
                else
                {
                    //UART_UartPutString("in UpdateSwitchLeds :: switch2 :: off \n");
                    LED2_Write(0);
                    Relay2_Write(0);
                    cache[1]=curPos;
                    CySysFlashWriteRow(CY_TEST_FLASH_ROW, cache);                   
                }
                break;
        case SWITCH3:
                if(curPos == 100 )
                {
                    //UART_UartPutString("in UpdateSwitchLeds :: switch3 :: on \n");
                    LED3_Write(1);
                    Relay3_Write(1);
                    cache[2]=curPos;
                    CySysFlashWriteRow(CY_TEST_FLASH_ROW, cache);
                }
                else
                {
                    //UART_UartPutString("in UpdateSwitchLeds :: switch3 :: off \n");
                    LED3_Write(0);
                    Relay3_Write(0);
                    cache[2]=curPos;
                    CySysFlashWriteRow(CY_TEST_FLASH_ROW, cache);
                }
                break;
        case SWITCH4:
                if(curPos == 100 )
                {
                    //UART_UartPutString("in UpdateSwitchLeds :: switch4 :: on \n");
                    LED4_Write(1);
                    Relay4_Write(1);
                    cache[3]=curPos;
                    CySysFlashWriteRow(CY_TEST_FLASH_ROW, cache);
                }
                else
                {
                    //UART_UartPutString("in UpdateSwitchLeds :: switch4 :: off \n");
                    LED4_Write(0);
                    Relay4_Write(0);
                    cache[3]=curPos;
                    CySysFlashWriteRow(CY_TEST_FLASH_ROW, cache);
                }
                break;
        case SWITCH5:
                if(curPos == 100 )
                {
                    //UART_UartPutString("in UpdateSwitchLeds :: switch5 :: on \n");
                    LED5_Write(1);
                    Relay5_Write(1);
                    cache[4]=curPos;
                    CySysFlashWriteRow(CY_TEST_FLASH_ROW, cache);
                }
                else
                {
                    //UART_UartPutString("in UpdateSwitchLeds :: switch5 :: off \n");
                    LED5_Write(0);
                    Relay5_Write(0);
                    cache[4]=curPos;
                    CySysFlashWriteRow(CY_TEST_FLASH_ROW, cache);
                    
           }
                break;
                
             case SWITCH6:
                if(curPos == 100 )
                {
                    //UART_UartPutString("in UpdateSwitchLeds :: switch1 :: on \n");
                    LED6_Write(1);
                    Relay6_Write(1);
                    cache[5]=curPos;
                    CySysFlashWriteRow(CY_TEST_FLASH_ROW, cache);
                   
                }
                else
                {
                    //UART_UartPutString("in UpdateSwitchLeds :: switch1 :: off \n");
                    LED6_Write(0);
                    Relay6_Write(0);
                    cache[5]=curPos;
                    CySysFlashWriteRow(CY_TEST_FLASH_ROW, cache);
                  
                }
                break;
            case SWITCH7:
                if(curPos == 100)
                {
                    //UART_UartPutString("in UpdateSwitchLeds :: switch2 :: on \n");
                    LED7_Write(1);
                    Triac1_Write(1);
                    cache[6]=curPos;
                    CySysFlashWriteRow(CY_TEST_FLASH_ROW, cache);
                   
                }
                else
                {
                    //UART_UartPutString("in UpdateSwitchLeds :: switch2 :: off \n");
                    LED7_Write(0);
                    Triac1_Write(0);
                    cache[6]=curPos;
                    CySysFlashWriteRow(CY_TEST_FLASH_ROW, cache);
                   
                }
                break; 
               
            case SWITCH8:
                if(curPos == 100)
                {
                    //UART_UartPutString("in UpdateSwitchLeds :: switch2 :: on \n");
                    LED8_Write(1);
                    Triac2_Write(1);
                    cache[7]=curPos;
                    CySysFlashWriteRow(CY_TEST_FLASH_ROW, cache);
                   
                }
                else
                {
                    //UART_UartPutString("in UpdateSwitchLeds :: switch2 :: off \n");
                    LED8_Write(0);
                    Triac2_Write(0);
                    cache[7]=curPos;
                    CySysFlashWriteRow(CY_TEST_FLASH_ROW, cache);
                    
                }
                break;
           
            default:
               // UART_UartPutString("in UpdateSwitchLeds :: All :: on \n");
                LED1_Write(1);
                LED2_Write(1);
                LED3_Write(1);
                LED4_Write(1);
                LED5_Write(1);
                LED6_Write(1);
                LED7_Write(1);
                LED8_Write(1);
                Relay1_Write(1);
                Relay2_Write(1);
                Relay3_Write(1);
                Relay4_Write(1);
                Relay5_Write(1);
                Relay6_Write(1);
                Triac1_Write(1);
                Triac2_Write(1);
                break;
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
