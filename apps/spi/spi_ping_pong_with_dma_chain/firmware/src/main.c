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

// DOM-IGNORE-BEGIN
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
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "definitions.h"                // SYS function prototypes
#include "string.h"

/* Macro definitions */
#define LED_SUCCESS_On      LED3_Set
#define LED_SUCCESS_Off     LED3_Clear

#define LED_FAIL_On      LED1_Set
#define LED_FAIL_Off     LED1_Clear

#define DMA_CHANNEL_RECEIVE_PING         DMAC_CHANNEL_3
#define DMA_CHANNEL_RECEIVE_PONG         DMAC_CHANNEL_4
#define DMA_CHANNEL_TRANSMIT_PING        DMAC_CHANNEL_0
#define DMA_CHANNEL_TRANSMIT_PONG        DMAC_CHANNEL_1

typedef enum
{
    APP_STATE_INITIALIZE,
	APP_STATE_SPI_IDLE,
    APP_STATE_VERIFY_AND_UPDATE_PING_BUFFER,
    APP_STATE_VERIFY_AND_UPDATE_PONG_BUFFER,
    APP_STATE_SPI_XFER_SUCCESSFUL,
    APP_STATE_SPI_XFER_ERROR

} APP_STATES;

typedef enum
{
    BUFFER_TYPE_PING,
    BUFFER_TYPE_PONG
}BUFFER_TYPE;

static uint8_t __attribute__((coherent, aligned(32))) txPingBuffer[] = "INITIAL_DMA_DATA_FROM_PING_BUFFER";
static uint8_t __attribute__((coherent, aligned(32))) rxPingBuffer[sizeof(txPingBuffer)] = {1};

static uint8_t __attribute__((coherent, aligned(32))) txPongBuffer[] = "initial_dma_data_from_pong_buffer";
static uint8_t __attribute__((coherent, aligned(32))) rxPongBuffer[sizeof(txPongBuffer)] = {2};

volatile static APP_STATES state = APP_STATE_INITIALIZE;

static void APP_DMA_TxCallbackHandler(DMAC_TRANSFER_EVENT event, uintptr_t context)
{
    if(event == DMAC_TRANSFER_EVENT_ERROR)
    {
        state = APP_STATE_SPI_XFER_ERROR;
    }
}

static void APP_DMA_RxPingCallbackHandler(DMAC_TRANSFER_EVENT event, uintptr_t context)
{
    if(event == DMAC_TRANSFER_EVENT_COMPLETE)
    {
        state = APP_STATE_VERIFY_AND_UPDATE_PING_BUFFER;
    }
    else
    {
        /* It means XDMAC_TRANSFER_ERROR event */
        state = APP_STATE_SPI_XFER_ERROR;
    }
}

static void APP_DMA_RxPongCallbackHandler(DMAC_TRANSFER_EVENT event, uintptr_t context)
{
    if(event == DMAC_TRANSFER_EVENT_COMPLETE)
    {
        state = APP_STATE_VERIFY_AND_UPDATE_PONG_BUFFER;
    }
    else
    {
        /* It means XDMAC_TRANSFER_ERROR event */
        state = APP_STATE_SPI_XFER_ERROR;
    }
}
// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************

int main ( void )
{
    uint8_t pingData=0, pongData=0x44;
    uint8_t i=0;

    /* Initialize all modules */
    SYS_Initialize ( NULL );

    while ( true )
    {
        switch (state)
        {
            case APP_STATE_INITIALIZE:
            {
                LED_SUCCESS_Off();
                LED_FAIL_Off();
                
                /* Setup the callbacks */
                DMAC_ChannelCallbackRegister(DMA_CHANNEL_TRANSMIT_PING, APP_DMA_TxCallbackHandler, (uintptr_t)NULL);
                DMAC_ChannelCallbackRegister(DMA_CHANNEL_TRANSMIT_PONG, APP_DMA_TxCallbackHandler, (uintptr_t)NULL);
                DMAC_ChannelCallbackRegister(DMA_CHANNEL_RECEIVE_PING, APP_DMA_RxPingCallbackHandler, (uintptr_t)NULL);
                DMAC_ChannelCallbackRegister(DMA_CHANNEL_RECEIVE_PONG, APP_DMA_RxPongCallbackHandler, (uintptr_t)NULL);

                /* Setup the DMA Chained channels */
                DMAC_ChainTransferSetup(DMA_CHANNEL_TRANSMIT_PONG, (const void *)&txPongBuffer, sizeof(txPongBuffer), (const void *)&SPI1BUF, 1, 1);
                DMAC_ChainTransferSetup(DMA_CHANNEL_RECEIVE_PONG, (const void *)&SPI1BUF, 1, (const void *)&rxPongBuffer, sizeof(rxPongBuffer), 1);
                
                /* Start the DMA in Chained mode */
                DMAC_ChannelTransfer(DMA_CHANNEL_TRANSMIT_PING, (const void *)&txPingBuffer, sizeof(txPingBuffer), (const void *)&SPI1BUF, 1, 1);
                DMAC_ChannelTransfer(DMA_CHANNEL_RECEIVE_PING, (const void *)&SPI1BUF, 1, (const void *)&rxPingBuffer, sizeof(rxPingBuffer), 1);
                state = APP_STATE_SPI_IDLE;
                break;
            }
            case APP_STATE_SPI_IDLE:
            {
                /* Application can do other task here */
                break;
            }
            case APP_STATE_VERIFY_AND_UPDATE_PING_BUFFER:
            {
                if (memcmp(&txPingBuffer[0], &rxPingBuffer[0], sizeof(txPingBuffer)) != 0)
                {
                    /* It means received data is not same as transmitted data */
                    state = APP_STATE_SPI_XFER_ERROR;
                }
                else
                {
                    /* It means received data is same as transmitted data.
                     * Change the data for next transfer */
                    for(i=0; i<sizeof(txPingBuffer); i++)
                    {
                        txPingBuffer[i] = pingData++;
                    }
                    /* Do not go to successful state before pong buffer also passes */
                    state = APP_STATE_SPI_IDLE;
                }
                break;
            }
            case APP_STATE_VERIFY_AND_UPDATE_PONG_BUFFER:
            {
                if (memcmp(&txPongBuffer[0], &rxPongBuffer[0], sizeof(txPongBuffer)) != 0)
                {
                    /* It means received data is not same as transmitted data */
                    state = APP_STATE_SPI_XFER_ERROR;
                }
                else
                {
                    /* It means received data is same as transmitted data.
                     * Change the data for next transfer */
                    for(i=0; i<sizeof(txPongBuffer); i++)
                    {
                        txPongBuffer[i] = pongData++;
                    }
                    state = APP_STATE_SPI_XFER_SUCCESSFUL;
                }
                break;
            }
            case APP_STATE_SPI_XFER_SUCCESSFUL:
            {
                LED_SUCCESS_On();
                break;
            }
            case APP_STATE_SPI_XFER_ERROR:
            {
                /* Abort DMA */
                DMAC_ChannelDisable(DMA_CHANNEL_TRANSMIT_PING);
                DMAC_ChannelDisable(DMA_CHANNEL_TRANSMIT_PONG);
                DMAC_ChannelDisable(DMA_CHANNEL_RECEIVE_PING);
                DMAC_ChannelDisable(DMA_CHANNEL_RECEIVE_PONG);

                /* Turn on the LED for failure */
                LED_FAIL_On();
                break;
            }
            default:
                break;
        }
    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}


/*******************************************************************************
 End of File
*/