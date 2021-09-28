/* SPDX-License-Identifier: GPL-2.0-only OR BSD-3-Clause */
/******************************************************************************
 *
 * This file is provided under a dual license.  When you use or
 * distribute this software, you may choose to be licensed under
 * version 2 of the GNU General Public License ("GPLv2 License")
 * or BSD License.
 *
 * GPLv2 License
 *
 * Copyright(C) 2019 MediaTek Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See http://www.gnu.org/licenses/gpl-2.0.html for more details.
 *
 * BSD LICENSE
 *
 * Copyright(C) 2019 MediaTek Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *  * Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *****************************************************************************/

#ifndef _MSKEYPAD_H_
#define _MSKEYPAD_H_

#if 1//(CONFIG_COMMANDS & CFG_CMD_KEYPAD)

#ifdef  _MSKEYPAD_C_
#define INTERFACE
#else
#define INTERFACE extern
#endif

//#define KeypadRepeatTimerCount 1 //100ms based

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#if ENABLE_DISPLAY_PICTURE_UPDATE
#define KEYPAD_USE_ISR  1
#else
#define KEYPAD_USE_ISR  0
#endif

#if 0
#define BIT0	                    0x00000001
#define BIT1	                    0x00000002
#define BIT2	                    0x00000004
#define BIT3	                    0x00000008
#define BIT4	                    0x00000010
#define BIT5	                    0x00000020
#define BIT6	                    0x00000040
#define BIT7	                    0x00000080
#endif
/// MS Return Error
#define MSRET_ERROR     0
/// MS Return OK
#define MSRET_OK        1
/// MS Return Other
#define MSRET_OTHER     2

#ifndef UNUSED
#define UNUSED(x) ((x)=(x))
#endif

// definition for TRUE
#define TRUE            1
// definition for FALSE
#define FALSE           0

//typedef unsigned char  U8;
//typedef unsigned int   U16;
//typedef unsigned long  U32;
//typedef U8      BOOLEAN;   ///< BOOLEAN
#define BOOL    BOOLEAN
#define ENABLE          1
#define DISABLE         0
#ifndef NULL
#define NULL	((void *) 0)
#endif

//-------------------------------------------------------------------------------------------------
//  enum
//-------------------------------------------------------------------------------------------------
enum KEYPAD_ADC_CHANNEL
{
    KEYPAD_ADC_CHANNEL_1 = 0,
    KEYPAD_ADC_CHANNEL_2,
    KEYPAD_ADC_CHANNEL_3,
    KEYPAD_ADC_CHANNEL_4,
    KEYPAD_ADC_CHANNEL_5,
    KEYPAD_ADC_CHANNEL_6,
    KEYPAD_ADC_CHANNEL_7,
    KEYPAD_ADC_CHANNEL_8,
};

#define SAR_SIGNAL_CH                   KEYPAD_ADC_CHANNEL_5

#ifndef  CUS_IR_HEAD_FILE//Please define it in board file for customization
#include "IR_MSTAR_DTV.h"
#else
#include CUS_IR_HEAD_FILE
#endif

//-------------------------------------------------------------------------------------------------
//  register define
//-------------------------------------------------------------------------------------------------

//#define RIU8    ((unsigned char  volatile *) RIU_MAP)
#define RIUBASE_PM_SAR              0x001400    //0x001400 ~ 0x0014ff

//#define RIU      ((unsigned short volatile *) RIU_MAP)
#define MDrv_WriteByte(u32Reg, u8Value)     RIU8[(u32Reg << 1) - (u32Reg & 1)] = u8Value
#define MDrv_ReadByte(u32Reg)                   RIU8[(u32Reg << 1) - (u32Reg & 1)]
#define MDrv_WriteRegBit( u32Reg, bEnable, u8Mask ) RIU8[(u32Reg <<1) - ((u32Reg) & 1)] = (bEnable) ? (RIU8[(u32Reg <<1) - ((u32Reg) & 1)] | (u8Mask)) : (RIU8[(u32Reg <<1) - ((u32Reg) & 1)] & ~(u8Mask))

//------------------------------------------------------------------------------

// SAR define
#define REG_SAR_CTRL0                   (RIUBASE_PM_SAR+0x00)
#define SAR_SINGLE_CH_MSK           (BIT2|BIT1|BIT0)//[2:0] //select channel for single channel modech0~ch7
#define SAR_LEVEL_TRIGGER           (BIT3) //keypad level trigger enable. 0:  use edge trigger, 1:  use level trigger
#define SAR_SINGLE_CH_EN            (BIT4) //enable single channel mode. 0: disable1: enable
#define SAR_MODE_FREERUN            (BIT5) //select sar digital operation mode. 0: one-shot, 1: freerun
#define SAR_DIGITAL_PWRDN           (BIT6) //sar digital power down
#define SAR_START                   (BIT7) //sar start signal
#define REG_SAR_CTRL1                   (RIUBASE_PM_SAR+0x01)
#define SAR_ADC_PWRDN               (BIT0)
#define SAR_ADC_FREERUN             (BIT1)
#define REG_SAR_SELCH                   (RIUBASE_PM_SAR+0x01)
#define SAR_SEL                     (BIT2)
#define SAR_NCH_EN                  (BIT3)
#define REG_SAR_CKSAMP_PRD              (RIUBASE_PM_SAR+0x02)
#define REG_SAR1_UPB                    (RIUBASE_PM_SAR+0x0A)
#define REG_SAR1_LOB                    (RIUBASE_PM_SAR+0x0B)
#define REG_SAR2_UPB                    (RIUBASE_PM_SAR+0x0C)
#define REG_SAR2_LOB                    (RIUBASE_PM_SAR+0x0D)
#define REG_SAR3_UPB                    (RIUBASE_PM_SAR+0x0E)
#define REG_SAR3_LOB                    (RIUBASE_PM_SAR+0x0F)
#define REG_SAR4_UPB                    (RIUBASE_PM_SAR+0x10)
#define REG_SAR4_LOB                    (RIUBASE_PM_SAR+0x11)
#define REG_SAR5_UPB                    (RIUBASE_PM_SAR+0x12)
#define REG_SAR5_LOB                    (RIUBASE_PM_SAR+0x13)
#define REG_SAR6_UPB                    (RIUBASE_PM_SAR+0x14)
#define REG_SAR6_LOB                    (RIUBASE_PM_SAR+0x15)
#define REG_SAR7_UPB                    (RIUBASE_PM_SAR+0x16)
#define REG_SAR7_LOB                    (RIUBASE_PM_SAR+0x17)
#define REG_SAR8_UPB                    (RIUBASE_PM_SAR+0x18)
#define REG_SAR8_LOB                    (RIUBASE_PM_SAR+0x19)
#define REG_SAR_ADCOUT1                 (RIUBASE_PM_SAR+0x1A)
#define REG_SAR_ADCOUT2                 (RIUBASE_PM_SAR+0x1B)
#define REG_SAR_ADCOUT3                 (RIUBASE_PM_SAR+0x1C)
#define REG_SAR_ADCOUT4                 (RIUBASE_PM_SAR+0x1D)
#define REG_SAR_ADCOUT5                 (RIUBASE_PM_SAR+0x1E)
#define REG_SAR_ADCOUT6                 (RIUBASE_PM_SAR+0x1F)
#define REG_SAR_ADCOUT7                 (RIUBASE_PM_SAR+0x20)
#define REG_SAR_ADCOUT8                 (RIUBASE_PM_SAR+0x21)
#define MASK_SAR_ADCOUT             (0xFF)
#define REG_SAR_AISEL                   (RIUBASE_PM_SAR+0x22)
#define SAR_AISEL_MSK               (0x1F)
#define SAR_AISEL_CH0_MSK           (BIT0)
#define SAR_AISEL_CH1_MSK           (BIT1)
#define SAR_AISEL_CH2_MSK           (BIT2)
#define SAR_AISEL_CH3_MSK           (BIT3)

#define REG_SAR_GPIOOEN                 (RIUBASE_PM_SAR+0x23)
#define SAR_GPIOOEN_MSK             (0x1F)
#define SAR_GPIOOEN_CH0_MSK         (BIT0)
#define SAR_GPIOOEN_CH1_MSK         (BIT1)
#define SAR_GPIOOEN_CH2_MSK         (BIT2)
#define SAR_GPIOOEN_CH3_MSK         (BIT3)

#define REG_SAR_TEST0                           (RIUBASE_PM_SAR+0x26)
#define REG_SAR_TEST1                           (RIUBASE_PM_SAR+0x27)

#if KEYPAD_USE_ISR
#define REG_SAR_INT_MASK           (RIUBASE_PM_SAR +0x14*2 )
    #define SAR_INT_CH0_MASK        (BIT0)
    #define SAR_INT_CH1_MASK        (BIT1)
    #define SAR_INT_CH2_MASK        (BIT2)
    #define SAR_INT_CH3_MASK        (BIT3)

#define REG_SAR_INT_CLR             (RIUBASE_PM_SAR + 0x15*2 )
    #define SAR_INT_CLR_ALL             (0xFF)//BIT7~BIT0 : SAR7~SAR0
#else
#define REG_SAR_INT_MASK                (RIUBASE_PM_SAR+0x28)
#define SAR_INT_MASK                (BIT0)
#endif

#if KEYPAD_USE_ISR
//PM_SLEEP
#define RIUBASE_PM_SLEEP            0x000e00    //0x000e00 ~ 0x000eff
//irq force for level wake-up source
#define REG_PM_WK_IRQ_MSK           (RIUBASE_PM_SLEEP+ 0x8*2 )
    #define PM_WK_IRQ_CEC               (BIT0)
    #define PM_WK_IRQ_SAR               (BIT1)
    #define PM_WK_IRQ_WOL              (BIT2)
    #define PM_WK_IRQ_VGA_SYNC     (BIT3)
    #define PM_WK_IRQ_MHL_CBUS     (BIT4)
    #define PM_WK_IRQ_DDC              (BIT5)
    #define PM_WK_IRQ_AVLINK          (BIT6)
    #define PM_WK_IRQ_RTC              (BIT7)
#endif

INTERFACE void  msKeypad_Init(void);
INTERFACE unsigned char msKeypad_GetKey(unsigned char *pkey, unsigned char *pflag);

#undef INTERFACE

#endif
#endif

