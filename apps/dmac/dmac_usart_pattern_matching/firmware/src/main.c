/*******************************************************************************
  Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This file contains the "main" function for a project.

  Description:
    This file contains the "main" function for a project.  The
    "main" function calls the "SYS_Initialize" function to initialize the state
    machines of all modules in the system
 *******************************************************************************/

/*******************************************************************************
* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "definitions.h"                // SYS function prototypes

static void TransmitCompleteCallback(DMAC_TRANSFER_EVENT event, uintptr_t contextHandle);
static void ReceiveCompleteCallback(DMAC_TRANSFER_EVENT event, uintptr_t contextHandle);

#define RX_BUFFER_SIZE 10

#define LED_ON    LED_Set
#define LED_OFF   LED_Clear

#define UART_TRANSMIT_CHANNEL   DMAC_CHANNEL_0
#define UART_RECEIVE_CHANNEL   DMAC_CHANNEL_1

static char __attribute__((coherent)) messageStart[] = "**** USART Echo Using DMA Pattern Matching ****\r\n\
**** Type a buffer of less than 10 characters followed ****\r\n\
**** by 'Enter' key and observe it echo back using DMA ****\r\n\
**** LED toggles each time the buffer is echoed ****\r\n";
static char __attribute__((coherent)) receiveBuffer[RX_BUFFER_SIZE] = {};
static char __attribute__((coherent)) echoBuffer[RX_BUFFER_SIZE] = {};

static char __attribute__((coherent)) endChar[2]="\r\n";

volatile static bool writeStatus = false;
volatile static bool readStatus = false;

// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************

int main ( void )
{
    /* Initialize all modules */
    SYS_Initialize ( NULL );
    DMAC_ChannelCallbackRegister(UART_TRANSMIT_CHANNEL, TransmitCompleteCallback,0);
    DMAC_ChannelCallbackRegister(UART_RECEIVE_CHANNEL, ReceiveCompleteCallback,0);
    
    DMAC_ChannelTransfer(UART_TRANSMIT_CHANNEL, (const void *)&messageStart, sizeof(messageStart), (const void *)&U2TXREG, 1, 1);
    
    /* data reception from user will stop as soon as "Enter" key is pressed (pattern is matched) */
    DMAC_ChannelPatternMatchSetup(UART_RECEIVE_CHANNEL, DMAC_DATA_PATTERN_SIZE_1_BYTE, (uint16_t)(0x0D));    

    while ( true )
    {
        /* Maintain state machines of all polled MPLAB Harmony modules. */
        SYS_Tasks ( );
        if(readStatus == true)
        {
            /* Echo back received buffer and Toggle LED */
            readStatus = false;

            memcpy(echoBuffer, receiveBuffer,sizeof(receiveBuffer));
            
            /* echoBuffer may have few stale data which we do not want to print, so stop printing when "Carriage Return" is found */
            DMAC_ChannelPatternMatchSetup(UART_TRANSMIT_CHANNEL, DMAC_DATA_PATTERN_SIZE_1_BYTE, (uint16_t)(0x0D));
            DMAC_ChannelTransfer(UART_TRANSMIT_CHANNEL, &echoBuffer, sizeof(echoBuffer), (const void *)&U2TXREG, 1, 1);
            LED_Toggle();
        }
        else if(writeStatus == true)
        {   
            /* Ensure to add "Carriage Return" and "Line Feed" on the console before receiving the next data from user */
            DMAC_ChannelPatternMatchDisable(UART_TRANSMIT_CHANNEL);
            DMAC_ChannelTransfer(UART_TRANSMIT_CHANNEL, &endChar, sizeof(endChar), (const void *)&U2TXREG, 1, 1);
            while(DMAC_ChannelIsBusy (UART_TRANSMIT_CHANNEL));
            
            /* Submit buffer to read user data */
            writeStatus = false;
            DMAC_ChannelTransfer(UART_RECEIVE_CHANNEL, (const void *)&U2RXREG, 1, &receiveBuffer, sizeof(receiveBuffer), 1);            
        }
        else
        {
            /* Repeat the loop */
            ;
        }        
    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}

static void TransmitCompleteCallback(DMAC_TRANSFER_EVENT event, uintptr_t contextHandle)
{
    writeStatus = true;    
}

static void ReceiveCompleteCallback(DMAC_TRANSFER_EVENT event, uintptr_t contextHandle)
{
    readStatus = true;    
}
/*******************************************************************************
 End of File
*/

