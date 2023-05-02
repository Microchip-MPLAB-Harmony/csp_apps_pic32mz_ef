/*******************************************************************************
  SPI PLIB

  Company:
    Microchip Technology Inc.

  File Name:
    plib_spi3_master.c

  Summary:
    SPI3 Master Source File

  Description:
    This file has implementation of all the interfaces provided for particular
    SPI peripheral.

*******************************************************************************/

/*******************************************************************************
* Copyright (C) 2018-2019 Microchip Technology Inc. and its subsidiaries.
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

#include "plib_spi3_master.h"
#include "interrupts.h"

// *****************************************************************************
// *****************************************************************************
// Section: SPI3 Implementation
// *****************************************************************************
// *****************************************************************************


#define SPI3_CON_MSTEN                      (1UL << _SPI3CON_MSTEN_POSITION)
#define SPI3_CON_CKP                        (0UL << _SPI3CON_CKP_POSITION)
#define SPI3_CON_CKE                        (1UL << _SPI3CON_CKE_POSITION)
#define SPI3_CON_MODE_32_MODE_16            (0UL << _SPI3CON_MODE16_POSITION)
#define SPI3_CON_ENHBUF                     (1UL << _SPI3CON_ENHBUF_POSITION)
#define SPI3_CON_MCLKSEL                    (0UL << _SPI3CON_MCLKSEL_POSITION)
#define SPI3_CON_MSSEN                      (1UL << _SPI3CON_MSSEN_POSITION)
#define SPI3_CON_SMP                        (0UL << _SPI3CON_SMP_POSITION)

void SPI3_Initialize ( void )
{
    uint32_t rdata = 0U;

    /* Disable SPI3 Interrupts */
    IEC4CLR = 0x4000000;
    IEC4CLR = 0x8000000;
    IEC4CLR = 0x10000000;

    /* STOP and Reset the SPI */
    SPI3CON = 0;

    /* Clear the Receiver buffer */
    rdata = SPI3BUF;
    rdata = rdata;

    /* Clear SPI3 Interrupt flags */
    IFS4CLR = 0x4000000;
    IFS4CLR = 0x8000000;
    IFS4CLR = 0x10000000;

    /* BAUD Rate register Setup */
    SPI3BRG = 49;

    /* CLear the Overflow */
    SPI3STATCLR = _SPI3STAT_SPIROV_MASK;

    /*
    MSTEN = 1
    CKP = 0
    CKE = 1
    MODE<32,16> = 0
    ENHBUF = 1
    MSSEN = 1
    MCLKSEL = 0
    */
    SPI3CONSET = (SPI3_CON_MSSEN | SPI3_CON_MCLKSEL | SPI3_CON_ENHBUF | SPI3_CON_MODE_32_MODE_16 | SPI3_CON_CKE | SPI3_CON_CKP | SPI3_CON_MSTEN | SPI3_CON_SMP);

    /* Enable transmit interrupt when transmit buffer is completely empty (STXISEL = '01') */
    /* Enable receive interrupt when the receive buffer is not empty (SRXISEL = '01') */
    SPI3CONSET = 0x00000005;


    /* Enable SPI3 */
    SPI3CONSET = _SPI3CON_ON_MASK;
}

bool SPI3_TransferSetup (SPI_TRANSFER_SETUP* setup, uint32_t spiSourceClock )
{
    uint32_t t_brg;
    uint32_t baudHigh;
    uint32_t baudLow;
    uint32_t errorHigh;
    uint32_t errorLow;

    if ((setup == NULL) || (setup->clockFrequency == 0U))
    {
        return false;
    }

    if(spiSourceClock == 0U)
    {
        // Use Master Clock Frequency set in GUI
        spiSourceClock = 100000000;
    }

    t_brg = (((spiSourceClock / (setup->clockFrequency)) / 2u) - 1u);
    baudHigh = spiSourceClock / (2u * (t_brg + 1u));
    baudLow = spiSourceClock / (2u * (t_brg + 2u));
    errorHigh = baudHigh - setup->clockFrequency;
    errorLow = setup->clockFrequency - baudLow;

    if (errorHigh > errorLow)
    {
        t_brg++;
    }

    if(t_brg > 8191U)
    {
        return false;
    }

    SPI3BRG = t_brg;
    SPI3CON = (SPI3CON & (~(_SPI3CON_MODE16_MASK | _SPI3CON_MODE32_MASK | _SPI3CON_CKP_MASK | _SPI3CON_CKE_MASK))) |
                            ((uint32_t)setup->clockPolarity | (uint32_t)setup->clockPhase | (uint32_t)setup->dataBits);

    return true;
}

bool SPI3_Write(void* pTransmitData, size_t txSize)
{
    return(SPI3_WriteRead(pTransmitData, txSize, NULL, 0));
}

bool SPI3_Read(void* pReceiveData, size_t rxSize)
{
    return(SPI3_WriteRead(NULL, 0, pReceiveData, rxSize));
}

bool SPI3_IsTransmitterBusy (void)
{
    return ((SPI3STAT & _SPI3STAT_SRMT_MASK) == 0U)? true : false;
}

bool SPI3_WriteRead(void* pTransmitData, size_t txSize, void* pReceiveData, size_t rxSize)
{
    size_t txCount = 0;
    size_t rxCount = 0;
    size_t dummySize = 0;
    size_t dummyRxCntr = 0;
    size_t receivedData;
    bool isSuccess = false;

    /* Verify the request */
    if (((txSize > 0U) && (pTransmitData != NULL)) || ((rxSize > 0U) && (pReceiveData != NULL)))
    {
        if (pTransmitData == NULL)
        {
            txSize = 0;
        }
        if (pReceiveData == NULL)
        {
            rxSize = 0;
        }

        /* Clear the receive overflow error if any */
        SPI3STATCLR = _SPI3STAT_SPIROV_MASK;

        /* Flush out any unread data in SPI read buffer from the previous transfer */
        while ((SPI3STAT & _SPI3STAT_SPIRBE_MASK) == 0U)
        {
            (void)SPI3BUF;
        }

        if (rxSize > txSize)
        {
            dummySize = rxSize - txSize;
        }

        /* If dataBit size is 32 bits */
        if (_SPI3CON_MODE32_MASK == (SPI3CON & _SPI3CON_MODE32_MASK))
        {
            rxSize >>= 2;
            txSize >>= 2;
            dummySize >>= 2;
        }
        /* If dataBit size is 16 bits */
        else if (_SPI3CON_MODE16_MASK == (SPI3CON & _SPI3CON_MODE16_MASK))
        {
            rxSize >>= 1;
            txSize >>= 1;
            dummySize >>= 1;
        }
        else
        {
             /* Nothing to process */
        }

        while((SPI3STAT & _SPI3STAT_SPITBE_MASK) == 0U)
        {
            /* Wait for transmit buffer to be empty */
        }

        while ((txCount != txSize) || (dummySize != 0U))
        {
            if (txCount != txSize)
            {
                if((_SPI3CON_MODE32_MASK) == (SPI3CON & (_SPI3CON_MODE32_MASK)))
                {
                    SPI3BUF = ((uint32_t*)pTransmitData)[txCount];
                }
                else if((_SPI3CON_MODE16_MASK) == (SPI3CON & (_SPI3CON_MODE16_MASK)))
                {
                    SPI3BUF = ((uint16_t*)pTransmitData)[txCount];
                }
                else
                {
                    SPI3BUF = ((uint8_t*)pTransmitData)[txCount];
                }
                txCount++;
            }
            else if (dummySize > 0U)
            {
                SPI3BUF = 0xff;
                dummySize--;
            }
            else
            {
                 /* Nothing to process */
            }

            if (rxCount == rxSize)
            {
                /* If inside this if condition, then it means that txSize > rxSize and all RX bytes are received */

                /* For transmit only request, wait for buffer to become empty */
                while((SPI3STAT & _SPI3STAT_SPITBE_MASK) == 0U)
                {
                    /* Wait for buffer empty */
                }

                /* Read until the receive buffer is not empty */
                while ((SPI3STAT & _SPI3STAT_SPIRBE_MASK) == 0U)
                {
                    (void)SPI3BUF;
                    dummyRxCntr++;
                }
            }
            else
            {
                /* If data is read, wait for the Receiver Data the data to become available */
                while((SPI3STAT & _SPI3STAT_SPIRBE_MASK) == _SPI3STAT_SPIRBE_MASK)
                {
                  /* Do Nothing */
                }

                /* We have data waiting in the SPI buffer */
                receivedData = SPI3BUF;

                if (rxCount < rxSize)
                {
                    if((_SPI3CON_MODE32_MASK) == (SPI3CON & (_SPI3CON_MODE32_MASK)))
                    {
                        ((uint32_t*)pReceiveData)[rxCount]  = receivedData;
                        rxCount++;
                    }
                    else if((_SPI3CON_MODE16_MASK) == (SPI3CON & (_SPI3CON_MODE16_MASK)))
                    {
                        ((uint16_t*)pReceiveData)[rxCount]  = (uint16_t)receivedData;
                        rxCount++;
                    }
                    else
                    {
                        ((uint8_t*)pReceiveData)[rxCount]  = (uint8_t)receivedData;
                        rxCount++;
                    }
                }
            }
        }

        /* Make sure no data is pending in the shift register */
        while((SPI3STAT & _SPI3STAT_SRMT_MASK) == 0U)
        {
            /* Data pending in shift register */
        }
        /* Make sure for every character transmitted a character is also received back.
         * If this is not done, we may prematurely exit this routine with the last bit still being
         * transmitted out. As a result, the application may prematurely deselect the CS line and also
         * the next request can receive last character of previous request as its first character.
         */
        if (txSize > rxSize)
        {
            while (dummyRxCntr != (txSize - rxSize))
            {
                /* Wait for all the RX bytes to be received. */
                while ((bool)(SPI3STAT & _SPI3STAT_SPIRBE_MASK) == false)
                {
                    (void)SPI3BUF;
                    dummyRxCntr++;
                }
            }
        }
        isSuccess = true;
    }
    return isSuccess;
}
