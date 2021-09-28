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

#ifndef IR_FORMAT_H
#define IR_FORMAT_H

//-------------------------------------------------------------------------------------------
// Customer IR Specification parameter define (Please modify them by IR SPEC)
//-------------------------------------------------------------------------------------------
#define IR_MODE_SEL             IR_TYPE_SWDECODE_RCA_MODE//IR_TYPE_SWDECODE_RC5_MODE//IR_TYPE_SWDECODE_MODE//IR_TYPE_SWDECODE_RCA_MODE//IR_TYPE_SWDECODE_MODE
// IR Header code define
#define IR_HEADER_CODE0         0x00    // Custom 0
#define IR_HEADER_CODE1         0x0F    // Custom 1
//--------------------------------------------------
// [Function] IR NEC 2nd customer code support
// [Support] Chip series after T8/T9 included can support 
// (CODE20!=0xFF) || (CODE21!=0xFF) ==> enable
// (CODE20==0xFF) && (CODE21==0xFF) ==> disable
// default set to disable
#define IR_HEADER_CODE20        0xFF    // NEC 2nd Custom 0 
#define IR_HEADER_CODE21        0xFF    // NEC 2nd Custom 1

// IR Timing define
#define IR_HEADER_CODE_TIME     4150//4000    // us
#define IR_OFF_CODE_TIME        4000    // us
#define IR_OFF_CODE_RP_TIME     10800   // us
#define IR_LOGI_01H_TIME        500     // us
#define IR_LOGI_0_TIME          2500    // us
#define IR_LOGI_1_TIME          1300//1500    // us
#define IR_TIMEOUT_CYC          140000  // us

#define IR_HEADER_CODE_TIME_UB  20
#define IR_HEADER_CODE_TIME_LB  -20
#define IR_OFF_CODE_TIME_UB  20
#define IR_OFF_CODE_TIME_LB  -20
#define IR_OFF_CODE_RP_TIME_UB  20
#define IR_OFF_CODE_RP_TIME_LB  -20
#define IR_LOGI_01H_TIME_UB  35
#define IR_LOGI_01H_TIME_LB  -30
#define IR_LOGI_0_TIME_UB  20
#define IR_LOGI_0_TIME_LB  -20
#define IR_LOGI_1_TIME_UB  20
#define IR_LOGI_1_TIME_LB  -20
// IR Format define
#define IRKEY_DUMY              0x00
#define IRDA_KEY_MAPPING_POWER  IRKEY_POWER

#define IR_LEADER_CODE_CHECKING_OPTION 0xBF
typedef enum _IrCommandType
{
    IRKEY_TV_RADIO          = 0x5F,
    IRKEY_CHANNEL_LIST      = 0xD7,
    IRKEY_CHANNEL_FAV_LIST  = 0x2F,
    IRKEY_CHANNEL_RETURN    = 0xD8,
    IRKEY_CHANNEL_PLUS      = 0xD2,
    IRKEY_CHANNEL_MINUS     = 0xD3,

    IRKEY_AUDIO             = 0xA5,
    IRKEY_VOLUME_PLUS       = 0xD0,
    IRKEY_VOLUME_MINUS      = 0xD1,

    IRKEY_UP                = 0xA6,
    IRKEY_POWER             = 0xD5,
    IRKEY_EXIT              = 0xF9,
    IRKEY_MENU              = 0xF7,
    IRKEY_DOWN              = 0xA7,
    IRKEY_LEFT              = 0xA9,
    IRKEY_SELECT            = 0x0B,
    IRKEY_RIGHT             = 0xA8,

    IRKEY_NUM_0             = 0xCF,
    IRKEY_NUM_1             = 0xCE,
    IRKEY_NUM_2             = 0xCD,
    IRKEY_NUM_3             = 0xCC,
    IRKEY_NUM_4             = 0xCB,
    IRKEY_NUM_5             = 0xCA,
    IRKEY_NUM_6             = 0xC9,
    IRKEY_NUM_7             = 0xC8,
    IRKEY_NUM_8             = 0xC7,
    IRKEY_NUM_9             = 0xC6,

    IRKEY_MUTE              = 0xC0,
    IRKEY_PAGE_UP           = IRKEY_DUMY+1,
    IRKEY_PAGE_DOWN         = IRKEY_DUMY+2,
    IRKEY_CLOCK             = IRKEY_DUMY+3,

    IRKEY_INFO              = 0xC3,
    IRKEY_RED               = IRKEY_DUMY+4,
    IRKEY_GREEN             = IRKEY_DUMY+5,
    IRKEY_YELLOW            = IRKEY_DUMY+6,
    IRKEY_BLUE              = IRKEY_DUMY+7,
    IRKEY_MTS               = 0x1B,
    IRKEY_NINE_LATTICE      = IRKEY_DUMY+8,
    IRKEY_TTX               = IRKEY_DUMY+9,
    IRKEY_CC                = IRKEY_DUMY+10,
    IRKEY_INPUT_SOURCE      = 0x5C,
    IRKEY_CRADRD            = IRKEY_DUMY+12,
//    IRKEY_PICTURE           = 0x40,
    IRKEY_ZOOM              = 0x6F,
    IRKEY_DASH              = IRKEY_DUMY+13,
    IRKEY_SLEEP             = 0xF8,
    IRKEY_EPG               = 0xE5,
    IRKEY_PIP               = IRKEY_DUMY+14,

  	IRKEY_MIX               = IRKEY_DUMY+15,
    IRKEY_INDEX             = IRKEY_DUMY+16,
    IRKEY_HOLD              = IRKEY_DUMY+17,
    IRKEY_PREVIOUS          = 0x45,
    IRKEY_NEXT              = 0xAC,
    IRKEY_BACKWARD          = 0xE2,
    IRKEY_FORWARD           = 0xE3,
    IRKEY_PLAY              = 0xEA,
    IRKEY_RECORD            = 0xE8,
    IRKEY_STOP              = 0xE0,
    IRKEY_PAUSE             = IRKEY_DUMY+18,

    IRKEY_SIZE              = 0x8A,
    IRKEY_REVEAL            = IRKEY_DUMY+19,
    IRKEY_SUBCODE           = IRKEY_DUMY+20,
    IRKEY_LED_CONTROL_FLAG1 = 0xFD,//this virtual ir for pm_sleep led on.off flag
    IRKEY_LED_CONTROL_FLAG2 = 0xFE,//this virtual ir for pm_sleep led on.off flag
    IRKEY_RESERVE0          = 0x01,
    IRKEY_RESERVE1          = 0x02,
    IRKEY_RESERVE2          = 0x03,
    IRKEY_RESERVE3          = 0x04,
    IRKEY_RESERVE4          = 0x05,
    IRKEY_RESERVE5          = 0x06,
}IrCommandType;
//-------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------
// IR system parameter define for H/W setting (Please don't modify them)
//-------------------------------------------------------------------------------------------

#define BIU_CLOCK               24000000
#define IR_CKDIV_NUM            ((BIU_CLOCK + 500000UL) / 1000000UL)
#define IR_CKDIV_NUM_BOOT       13

//#define IR_CLK_BOOT             (BIU_CLOCK_BOOT / 1000000.0)
#define IR_CLK                  (BIU_CLOCK / 1000000.0)
#define irGetMinCnt_BOOT(time, tolerance) (((double)time*((double)IR_CLK_BOOT)/(IR_CKDIV_NUM_BOOT+1))*((double)1-tolerance))
#define irGetMaxCnt_BOOT(time, tolerance) (((double)time*((double)IR_CLK_BOOT)/(IR_CKDIV_NUM_BOOT+1))*((double)1+tolerance))
#define irGetMinCnt(time, tolerance) (((double)time*((double)IR_CLK)/(IR_CKDIV_NUM+1))*((double)1-tolerance))
#define irGetMaxCnt(time, tolerance) (((double)time*((double)IR_CLK)/(IR_CKDIV_NUM+1))*((double)1+tolerance))

#define irGetCnt_BOOT(time) (((double)time*((double)IR_CLK_BOOT)/(IR_CKDIV_NUM_BOOT+1))+0x300000UL)
#define irGetCnt(time) (((double)time*((double)IR_CLK)/(IR_CKDIV_NUM+1))+0x300000UL)


// 12Mhz
#define IR_RP_TIMEOUT_BOOT      irGetCnt_BOOT(IR_TIMEOUT_CYC)
#define IR_HDC_UPB_BOOT         irGetMaxCnt_BOOT(IR_HEADER_CODE_TIME, 0.2)
#define IR_HDC_LOB_BOOT         irGetMinCnt_BOOT(IR_HEADER_CODE_TIME, 0.2)
#define IR_OFC_UPB_BOOT         irGetMaxCnt_BOOT(IR_OFF_CODE_TIME, 0.2)
#define IR_OFC_LOB_BOOT         irGetMinCnt_BOOT(IR_OFF_CODE_TIME, 0.2)
#define IR_OFC_RP_UPB_BOOT      irGetMaxCnt_BOOT(IR_OFF_CODE_RP_TIME, 0.2)
#define IR_OFC_RP_LOB_BOOT      irGetMinCnt_BOOT(IR_OFF_CODE_RP_TIME, 0.2)
#define IR_LG01H_UPB_BOOT       irGetMaxCnt_BOOT(IR_LOGI_01H_TIME, 0.35)
#define IR_LG01H_LOB_BOOT       irGetMinCnt_BOOT(IR_LOGI_01H_TIME, 0.3)
#define IR_LG0_UPB_BOOT         irGetMaxCnt_BOOT(IR_LOGI_0_TIME, 0.2)
#define IR_LG0_LOB_BOOT         irGetMinCnt_BOOT(IR_LOGI_0_TIME, 0.2)
#define IR_LG1_UPB_BOOT         irGetMaxCnt_BOOT(IR_LOGI_1_TIME, 0.2)
#define IR_LG1_LOB_BOOT         irGetMinCnt_BOOT(IR_LOGI_1_TIME, 0.2)

// 90Mhz
#define IR_RP_TIMEOUT           irGetCnt(IR_TIMEOUT_CYC)
#define IR_HDC_UPB              irGetMaxCnt(IR_HEADER_CODE_TIME, 0.2)
#define IR_HDC_LOB              irGetMinCnt(IR_HEADER_CODE_TIME, 0.2)
#define IR_OFC_UPB              irGetMaxCnt(IR_OFF_CODE_TIME, 0.2)
#define IR_OFC_LOB              irGetMinCnt(IR_OFF_CODE_TIME, 0.2)
#define IR_OFC_RP_UPB           irGetMaxCnt(IR_OFF_CODE_RP_TIME, 0.2)
#define IR_OFC_RP_LOB           irGetMinCnt(IR_OFF_CODE_RP_TIME, 0.2)
#define IR_LG01H_UPB            irGetMaxCnt(IR_LOGI_01H_TIME, 0.35)
#define IR_LG01H_LOB            irGetMinCnt(IR_LOGI_01H_TIME, 0.3)
#define IR_LG0_UPB              irGetMaxCnt(IR_LOGI_0_TIME, 0.2)
#define IR_LG0_LOB              irGetMinCnt(IR_LOGI_0_TIME, 0.2)
#define IR_LG1_UPB              irGetMaxCnt(IR_LOGI_1_TIME, 0.2)
#define IR_LG1_LOB              irGetMinCnt(IR_LOGI_1_TIME, 0.2)

//-------------------------------------------------------------------------------------------

#endif

