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

/* CapSense Slider resoluton is set to 127 (7 bits).
*  Since PWM has 16 bit resolution, slider position value should be shifted left
*  by 9 bits.
*/
#define SLIDER_POS_TO_COMPARE_SHIFT    (9u)

void CapSense_DisplayState(void);
void UpdateLeds(uint16);

uint16 curPos, oldPos, but0, but1;

/* I2C slave read and write buffers */
//uint8 i2cReadBuffer [BUFFER_SIZE] = {PACKET_SOP, STS_CMD_FAIL, PACKET_EOP};
uint8 i2cReadBuffer [2]; // we need to change this value according to our pwm buffer
uint8 i2cWriteBuffer[BUFFER_SIZE];

// PWM Array defined - Amit
// interupt pin no = P0.0
struct masterData{
    int pos;
    int posData;
};

struct masterData data;
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
{
   uint8 status = STS_CMD_FAIL;
    UART_Start();
    /* Start I2C slave (SCB mode) */
    I2C_Slave_I2CSlaveInitReadBuf(i2cReadBuffer,  2);
    I2C_Slave_I2CSlaveInitWriteBuf(i2cWriteBuffer, BUFFER_SIZE);
    I2C_Slave_Start();
    
    /* Enable global interrupts */
    CyGlobalIntEnable;

    /* Start PWM and CapSense components */
    LED_CONTROL_Start();

    CapSense_Start();

    /* Initialize baselines */
    CapSense_InitializeAllBaselines();
    
    // PWM Array initazlied with zero - Amit
    data.pos = 0;
    data.posData = 0;
    //Capsense Loop
    while(1u)
    {
 
        /* Write complete: parse command packet */
        if (0u != (I2C_Slave_I2CSlaveStatus() & I2C_Slave_I2C_SSTAT_WR_CMPLT))
        {
            UART_UartPutString("inside I2C_Slave_I2CSlaveStatus \n");
            /* Check packet length */
            //if (PACKET_SIZE == I2C_Slave_I2CSlaveGetWriteBufSize())
            if (1u <= I2C_Slave_I2CSlaveGetWriteBufSize())
            {
                UART_UartPutString("inside I2C_Slave_I2CSlaveGetWriteBufSize \n");
                /* Check start and end of packet markers */
//                if ((i2cWriteBuffer[PACKET_SOP_POS] == PACKET_SOP) &&
//                    (i2cWriteBuffer[PACKET_EOP_POS] == PACKET_EOP))
                {
                    UART_UartPutString("before ExecuteCommand \n");
                    
                        status = ExecuteCommand(i2cWriteBuffer[0], i2cWriteBuffer[1]);
                }
            }

            /* Clear slave write buffer and status */
            I2C_Slave_I2CSlaveClearWriteBuf();
            (void) I2C_Slave_I2CSlaveClearWriteStatus();

            /* Update read buffer */
//            i2cReadBuffer[PACKET_STS_POS] = status;
            status = STS_CMD_FAIL;
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
    /* Find Slider Position */
    curPos = CapSense_GetCentroidPos(CapSense_LINEARSLIDER0__LS);
    but0 = CapSense_GetCentroidPos(CapSense_BUTTON0__BTN);
    if( but0 < 1000)
    {
        LED_CONTROL_WriteCompare(32767);
        UpdateLeds(0xFFFF);
        
        data.pos = 1;
        data.posData = 50;
        
        i2cReadBuffer[0] = data.pos;
        i2cReadBuffer[1] = data.posData;
        
        InterPin_Write(~InterPin_Read());
        //CyDelay(2000);
        UpdateLeds(0x0000);
    }
    
    but1 = CapSense_GetCentroidPos(CapSense_BUTTON1__BTN);
    if (but1 < 1000)
    {
        LED_CONTROL_WriteCompare(16383);
        UpdateLeds(0xFFFF);
        
        data.pos = 2;
        data.posData = 75;
        i2cReadBuffer[0] = data.pos;
        i2cReadBuffer[1] = data.posData;
        
        
        InterPin_Write(~InterPin_Read());
        //CyDelay(2000);
        UpdateLeds(0x0000);
    }
    
    /* Reset position */
    if(curPos == 0xFFFFu)
    {
        curPos = 0u;
    }

    /* Move bargraph */
    if (curPos != oldPos)
    {
        oldPos = curPos;
        /* Display Slider bargraph */
        if (curPos != 0u)
        {
            LED_CONTROL_WriteCompare((uint32)curPos << SLIDER_POS_TO_COMPARE_SHIFT);
            data.pos = 0;
            data.posData = curPos;
           
            i2cReadBuffer[0] = data.pos;
            i2cReadBuffer[1] = data.posData;
            InterPin_Write(~InterPin_Read());
            UpdateLeds(curPos);
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
    UART_UartPutString("in ExecuteCommand \n");
    uint8 status;
    status = STS_CMD_DONE;

    /* Execute received command */
    switch (cmd)
    {
        case FAN1:
            UART_UartPutString("in ExecuteCommand :: FAN1\n");
            UpdateFanLed1(value);
            break;
        case FAN2:
            UART_UartPutString("in ExecuteCommand :: FAN2\n");
            UpdateFanLed2(value);
            break;
        case SWITCH1:
            UART_UartPutString("in ExecuteCommand :: SWITCH1\n");
            UpdateLightLeds(value);
            break;
        case SWITCH2:
            UART_UartPutString("in ExecuteCommand :: SWITCH2\n");
            UpdateLightLeds(value);
            break;
        case SWITCH3:
            UART_UartPutString("in ExecuteCommand :: SWITCH3\n");
            UpdateLightLeds(value);
            break;
        case SWITCH4:
            UART_UartPutString("in ExecuteCommand :: SWITCH4\n");
            UpdateLightLeds(value);
            break;
        case SWITCH5:
            UART_UartPutString("in ExecuteCommand :: SWITCH5\n");
            UpdateLightLeds(value);
            break;
        case SOCKET1:
            UART_UartPutString("in ExecuteCommand :: SOCKET1\n");
            UpdateSocketLeds(value);
            break;
        default:
            UART_UartPutString("in ExecuteCommand :: default\n");
            status = STS_CMD_FAIL;
            break;
    }

    return (status);
}

void UpdateSocketLeds(uint16 curPos)
{
    /* Turn of all LEDs first */
    UART_UartPutString("in UpdateSocketLeds\n");
       
	case 0:
	    LED1_Write(0);
	    LED2_Write(0);
	    LED3_Write(0);
	    LED4_Write(0);
	    LED5_Write(0);
	    break;

	case 1:
	    LED1_Write(1);
	    LED2_Write(1);
	    LED3_Write(1);
	    LED4_Write(1);
	    LED5_Write(1);

	    break;

	default:
	    break;
}

void UpdateLightLeds(uint16 lightNo)
{
    /* Turn of all LEDs first */
    LED1_Write(0);
    LED2_Write(0);
    LED3_Write(0);
    LED4_Write(0);
    LED5_Write(0);
    UART_UartPutString("in UpdateLightLeds\n");

	case 1:
	    LED1_Write(1);
	    break;
	case 2:
	    LED1_Write(1);
	    break;

	case 3:
	    LED1_Write(1);
	    break;

	case 4:
	    LED1_Write(1);
	    break;

	case 5:
	    LED1_Write(1);
	    break;

	default:
	    break;
}
// Fan 1
void UpdateFanLed1(uint16 curPos)
{
    /* Turn of all LEDs first */
    LED1_Write(0);
    LED2_Write(0);
    LED3_Write(0);
    LED4_Write(0);
    LED5_Write(0);
    UART_UartPutString("in UpdateFanLed1\n");
       
    if (curPos > 10 || curPos <= 10)
    {
        UART_UartPutString("in UpdateFanLed1 :: range 0-10\n");
        LED1_Write(1);
    }
    
    if (curPos > 20)
    {
        UART_UartPutString("in UpdateFanLed1 :: range >20\n");
        LED2_Write(1);
    }
    
    if (curPos > 40)
    {
        UART_UartPutString("in UpdateFanLed1 :: range >40\n");
        LED3_Write(1);
    }
    
    if (curPos > 60)
    {
        UART_UartPutString("in UpdateFanLed1 :: range > 60\n");
        LED4_Write(1);
    }
    
    if (curPos > 80)
    {
        UART_UartPutString("in UpdateFanLed1 :: range > 80\n");
        LED5_Write(1);
    }
}
// Fan 2
void UpdateFanLed2(uint16 curPos)
{
    /* Turn of all LEDs first */
    LED1_Write(0);
    LED2_Write(0);
    LED3_Write(0);
    LED4_Write(0);
    LED5_Write(0);
    UART_UartPutString("in UpdateFanLed2\n");
       
    if (curPos > 10 || curPos <= 10)
    {
        UART_UartPutString("in UpdateFanLed2 :: range 0-10\n");
        LED1_Write(1);
    }
    
    if (curPos > 20)
    {
        UART_UartPutString("in UpdateFanLed2 :: range >20\n");
        LED2_Write(1);
    }
    
    if (curPos > 40)
    {
        UART_UartPutString("in UpdateFanLed2 :: range >40\n");
        LED3_Write(1);
    }
    
    if (curPos > 60)
    {
        UART_UartPutString("in UpdateFanLed2 :: range > 60\n");
        LED4_Write(1);
    }
    
    if (curPos > 80)
    {
        UART_UartPutString("in UpdateFanLed2 :: range > 80\n");
        LED5_Write(1);
    }
}
/* [] END OF FILE */

