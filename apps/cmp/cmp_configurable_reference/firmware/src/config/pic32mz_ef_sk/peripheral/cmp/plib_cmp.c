/*******************************************************************************
  Comparator (CMP) Peripheral Library (PLIB)

  Company:
    Microchip Technology Inc.

  File Name:
    plib_cmp.c

  Summary:
    CMP Source File

  Description:
    None

*******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2019 Microchip Technology Inc. and its subsidiaries.
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

#include "plib_cmp.h"
#include "interrupts.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data
// *****************************************************************************
// *****************************************************************************

volatile static CMP_OBJECT cmp1Obj;

// *****************************************************************************
// *****************************************************************************
// Section: CMP Implementation
// *****************************************************************************
// *****************************************************************************

void CMP_Initialize(void)
{
    /*  Setup CM1CON    */
    /*  CCH     = 2    */
    /*  CREF    = 1    */
    /*  EVPOL   = 1    */
    /*  CPOL    = false    */
    /*  COE     = false    */
    CM1CON = 0x52;

    /* Enable Comparator 1 Interrupt */
    IEC4SET = _IEC4_CMP1IE_MASK;

    /*  Setup CM2CON    */
    /*  CCH     = 0    */
    /*  CREF    = 0    */
    /*  EVPOL   = 0    */
    /*  CPOL    = false    */
    /*  COE     = false    */
    CM2CON = 0x0;

    /* Clear Idle Control */
    CMSTATCLR = _CMSTAT_SIDL_MASK;
}

bool CMP_StatusGet(CMP_STATUS_SOURCE ch_status)
{
    return ((CMSTAT & ch_status) != 0U);
}

void CMP_1_CompareEnable(void)
{
    CM1CONSET = _CM1CON_ON_MASK;
}

void CMP_1_CompareDisable(void)
{
    CM1CONCLR = _CM1CON_ON_MASK;
}

void CMP_1_CallbackRegister(CMP_CALLBACK callback, uintptr_t context)
{
    cmp1Obj.callback = callback;

    cmp1Obj.context = context;
}

void __attribute__((used)) COMPARATOR_1_InterruptHandler(void)
{
    IFS4CLR = _IFS4_CMP1IF_MASK; //Clear IRQ flag

    if(cmp1Obj.callback != NULL)
    {
        uintptr_t context = cmp1Obj.context;
        cmp1Obj.callback(context);
    }
}

void CMP_2_CompareEnable(void)
{
    CM2CONSET = _CM2CON_ON_MASK;
}

void CMP_2_CompareDisable(void)
{
    CM2CONCLR = _CM2CON_ON_MASK;
}

