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


#define DMA_CHANNEL_RECEIVE         DMAC_CHANNEL_1
#define DMA_CHANNEL_TRANSMIT        DMAC_CHANNEL_0

typedef enum
{
    APP_STATE_INITIALIZE,
	APP_STATE_SPI_IDLE,
    APP_STATE_VERIFY_AND_UPDATE_PING_BUFFER,
    APP_STATE_VERIFY_AND_UPDATE_PONG_BUFFER,
    APP_STATE_SPI_XFER_SUCCESSFUL,
    APP_STATE_SPI_XFER_ERROR

} APP_STATES;

/* first half of array is considered as ping buffer and 2nd half as pong buffer */
static uint8_t __attribute__((coherent, aligned(32))) txBuffer[] = "INITIAL_DMA_DATA_FROM_PING_BUFFERinitial_dma_data_from_pong_buffer";
static uint8_t __attribute__((coherent, aligned(32))) rxBuffer[sizeof(txBuffer)] = {1};


volatile static APP_STATES state = APP_STATE_INITIALIZE;

static void APP_DMA_TxCallbackHandler(DMAC_TRANSFER_EVENT event, uintptr_t context)
{
    if(event == DMAC_TRANSFER_EVENT_ERROR)
    {
        state = APP_STATE_SPI_XFER_ERROR;
    }
}

static void APP_DMA_RxCallbackHandler(DMAC_TRANSFER_EVENT event, uintptr_t context)
{
    if(event == DMAC_TRANSFER_EVENT_HALF_COMPLETE)
    {
        state = APP_STATE_VERIFY_AND_UPDATE_PING_BUFFER;
    }
    else if(event == DMAC_TRANSFER_EVENT_COMPLETE)
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
                DMAC_ChannelCallbackRegister(DMA_CHANNEL_TRANSMIT, APP_DMA_TxCallbackHandler, (uintptr_t)NULL);
                DMAC_ChannelCallbackRegister(DMA_CHANNEL_RECEIVE, APP_DMA_RxCallbackHandler, (uintptr_t)NULL);

                /* Start the DMA in Chained mode */
                DMAC_ChannelTransfer(DMA_CHANNEL_TRANSMIT, (const void *)&txBuffer, sizeof(txBuffer), (const void *)&SPI1BUF, 1, 1);
                DMAC_ChannelTransfer(DMA_CHANNEL_RECEIVE, (const void *)&SPI1BUF, 1, (const void *)&rxBuffer, sizeof(rxBuffer), 1);
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
                if (memcmp(&txBuffer[0], &rxBuffer[0], sizeof(txBuffer)/2) != 0)
                {
                    /* It means received data is not same as transmitted data */
                    state = APP_STATE_SPI_XFER_ERROR;
                }
                else
                {
                    /* It means received data is same as transmitted data.
                     * Change the data for next transfer */
                    for(i=0; i<sizeof(txBuffer)/2; i++)
                    {
                        txBuffer[i] = pingData++;
                    }
                    /* Do not go to successful state before pong buffer also passes */
                    state = APP_STATE_SPI_IDLE;
                }
                break;
            }
            case APP_STATE_VERIFY_AND_UPDATE_PONG_BUFFER:
            {
                if (memcmp(&txBuffer[sizeof(txBuffer)/2], &rxBuffer[sizeof(txBuffer)/2], sizeof(txBuffer)/2) != 0)
                {
                    /* It means received data is not same as transmitted data */
                    state = APP_STATE_SPI_XFER_ERROR;
                }
                else
                {
                    /* It means received data is same as transmitted data.
                     * Change the data for next transfer */
                    for(i=sizeof(txBuffer)/2; i<sizeof(txBuffer); i++)
                    {
                        txBuffer[i] = pongData++;
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
                DMAC_ChannelDisable(DMA_CHANNEL_TRANSMIT);
                DMAC_ChannelDisable(DMA_CHANNEL_RECEIVE);

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