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

//*************************************************************************
// Customer IR Specification parameter define (Please modify them by IR SPEC)
//*************************************************************************
#define IR_MODE_SEL             IR_TYPE_SWDECODE_KON_MODE//IR_TYPE_HWDECODE_FULL_MODE
#define IR_LG_MAP               0
#define IR_CHANNEL_USE_AS_UPDOWN   0
#define IR_VOLUME_USE_AS_LEFTRIGHT      0
// IR Header code define

#define IR_HEADER_CODE0     0x02    // Custom 0
#define IR_HEADER_CODE1     0x00    // Custom 1
//--------------------------------------------------
// [Function] IR NEC 2nd customer code support
// [Support] Chip series after T8/T9 included can support 
// (CODE20!=0xFF) || (CODE21!=0xFF) ==> enable
// (CODE20==0xFF) && (CODE21==0xFF) ==> disable
// default set to disable
#define IR_HEADER_CODE20        0xFF    // NEC 2nd Custom 0 
#define IR_HEADER_CODE21        0xFF    // NEC 2nd Custom 1 

// IR Timing define
#define IR_HEADER_CODE_TIME     3000    // us
#define IR_OFF_CODE_TIME        3000    // us
#define IR_OFF_CODE_RP_TIME     4500    // us
#define IR_LOGI_01H_TIME        500     // us
#define IR_LOGI_0_TIME          2000    // us
#define IR_LOGI_1_TIME          3000    // us
#define IR_TIMEOUT_CYC          70000  // us

#define IR_HEADER_CODE_TIME_UB  20
#define IR_HEADER_CODE_TIME_LB  -20
#define IR_OFF_CODE_TIME_UB  20
#define IR_OFF_CODE_TIME_LB  -20
#define IR_OFF_CODE_RP_TIME_UB  20
#define IR_OFF_CODE_RP_TIME_LB  -20
#define IR_LOGI_01H_TIME_UB  35
#define IR_LOGI_01H_TIME_LB  -30
#define IR_LOGI_0_TIME_UB  10
#define IR_LOGI_0_TIME_LB  -10
#define IR_LOGI_1_TIME_UB  20
#define IR_LOGI_1_TIME_LB  -20

#define IR_KON_PWS3_HEADER_CNT_LB       5000
#define IR_KON_PWS3_HEADER_CNT_UB       7000
#define IR_KON_PWS3_LOGIC0_CNT_LB       1500
#define IR_KON_PWS3_LOGIC0_CNT_UB       2500
#define IR_KON_PWS3_LOGIC1_CNT_LB       2500
#define IR_KON_PWS3_LOGIC1_CNT_UB       3500
#define IR_KON_PWS3_STOP_CNT_LB       4000
#define IR_KON_PWS3_STOP_CNT_UB       5000
#define IR_KON_PWS3_REPEATE_TIMEOUT     150

// IR Format define
#define IRKEY_DUMY              0xFF
#define IRDA_KEY_MAPPING_POWER  IRKEY_POWER
#define IR_LEADER_CODE_CHECKING_OPTION 0xBF


#define IR_LEADER_CODE_CHECKING_OPTION 0xBF
typedef enum _IrCommandType
{
     IRKEY_TV_RADIO          = 0x33,
    IRKEY_CHANNEL_LIST      = 0x32,
    IRKEY_CHANNEL_FAV_LIST  =0x34,
    IRKEY_CHANNEL_RETURN    = 0x0E,  //
    IRKEY_CHANNEL_PLUS      = 0x11,  //
    IRKEY_CHANNEL_MINUS     = 0x10,  //

    IRKEY_AUDIO             = 0x0D,  // sound mode
    IRKEY_VOLUME_PLUS       = 0x13,  //
    IRKEY_VOLUME_MINUS      = 0x12,  //
    IRKEY_UP                = 0x2B,  //
    IRKEY_POWER             = 0x0B,  //
    IRKEY_EXIT              = 0x30,  //
    IRKEY_MENU              = 0x15,  //
    IRKEY_DOWN              = 0x2C,  //
    IRKEY_LEFT              = 0x2D,  //
    IRKEY_SELECT            = 0x2F,  //
    IRKEY_RIGHT             = 0x2E,  //

    IRKEY_TIME              = 0x0F,
    IRKEY_LIST              = 0x1B,
    IRKEY_NUM_0             = 0x00,  //
    IRKEY_NUM_1             = 0x01,  //
    IRKEY_NUM_2             = 0x02,  //
    IRKEY_NUM_3             = 0x03,  //
    IRKEY_NUM_4             = 0x04,  //
    IRKEY_NUM_5             = 0x05,  //
    IRKEY_NUM_6             = 0x06,  //
    IRKEY_NUM_7             = 0x07,  //
    IRKEY_NUM_8             = 0x08,  //
    IRKEY_NUM_9             = 0x09,  //

    IRKEY_MUTE              = 0x14,  //
    IRKEY_FREEZE            = 0x19,
    IRKEY_PAGE_UP           = 0x1A,  //
    IRKEY_PAGE_DOWN         = 0x1B,  //
    IRKEY_CLOCK             = IRKEY_DUMY,
    IRKEY_TV_INPUT           = IRKEY_DUMY-46,
    IRKEY_INFO              = 0x0a,  //

    IRKEY_RED               = 0x1A,
    IRKEY_GREEN             =  0x1F,
    IRKEY_YELLOW            =0x0C,
    IRKEY_BLUE              =0x16,
    IRKEY_MTS               =  0x17,//
    IRKEY_NINE_LATTICE      = IRKEY_DUMY-5,
    IRKEY_TTX               = 0x23,
    IRKEY_CC                = 0x20,  //
    IRKEY_INPUT_SOURCE      = 0x1c,  //
    IRKEY_CRADRD            = IRKEY_DUMY-7,
    IRKEY_PICTURE           = 0x1D,  // Picture Mode
    IRKEY_ZOOM              = 0x1E,  //
    IRKEY_DASH              = IRKEY_DUMY-33, //
    IRKEY_SLEEP             = 0x31,
    IRKEY_EPG               = 0x18,  //IRKEY_DUMY-30

    IRKEY_MIX               = 0x27,
    IRKEY_INDEX             = 0x25,
    IRKEY_HOLD              = 0x2A,
    IRKEY_PREVIOUS          = IRKEY_DUMY-11,
    IRKEY_NEXT              = IRKEY_DUMY-12,
    IRKEY_BACKWARD          = IRKEY_DUMY-13,
    IRKEY_FORWARD           = IRKEY_DUMY-14,
    IRKEY_PLAY              = IRKEY_DUMY-15,
    IRKEY_RECORD            = IRKEY_DUMY-16,
    IRKEY_STOP              = IRKEY_DUMY-17,
    IRKEY_PAUSE             = IRKEY_DUMY-18,

    IRKEY_SIZE              = 0x22,
    IRKEY_REVEAL            = 0x24,
    IRKEY_SUBCODE           =  0x21,
    IRKEY_ANTENNA           = IRKEY_DUMY-22,
     IRKEY_LOCK                = IRKEY_DUMY-23,
  IRKEY_BACK                 = IRKEY_DUMY-24,
  IRKEY_RED2                  = IRKEY_DUMY-25,
  IRKEY_GREEN2              = IRKEY_DUMY-26,
  IRKEY_SUBTITLE            = 0x26,
  IRKEY_UPDATE              =0x28,
  IRKEY_TTX_MODE         = IRKEY_DUMY-29,

  IRKEY_ADJUST               = IRKEY_DUMY-31,
  IRKEY_KEY_DISABLE_KEYPAD = IRKEY_DUMY-32,
   IRKEY_P_CHECK      = IRKEY_DUMY-33,
   IRKEY_S_CHECK   =   IRKEY_DUMY-34,


    // source switching hotkeys
    IRKEY_AV                 = IRKEY_DUMY-35,
    IRKEY_HDMI            = IRKEY_DUMY-36,
    IRKEY_SCART         = IRKEY_DUMY-37,
    IRKEY_PC                = IRKEY_DUMY-38,
    IRKEY_TV                = IRKEY_DUMY-39,
    IRKEY_DTV              = IRKEY_DUMY-40,
    IRKEY_COMPONENT  = IRKEY_DUMY-41,
    IRKEY_SV                = IRKEY_DUMY-42,

    IRKEY_SUBPAGE   =   IRKEY_DUMY-43,

}IrCommandType;
//*************************************************************************

//*************************************************************************
// IR system parameter define for H/W setting (Please don't modify them)
//*************************************************************************
#define IR_CKDIV_NUM             ((BIU_CLOCK + 500000UL) / 1000000UL)
#define IR_CKDIV_NUM_BOOT        13

#define IR_CLK_BOOT             (BIU_CLOCK_BOOT / 1000000.0)
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

// Power off mode
#define PM_IR_TH_VAL            (PM_TH0_VAL & 0x0F)
// PM_IR_TH_GRID specify the time (in us) for each threshold bit.
// So PM_IR_TH_GRID = (1/12M) * (2^14) * 1000000 = (2^14) / 12
#define PM_IR_TH_GRID           (16384/IR_CLK_BOOT)
#define PM_IR_HEADER_CODE_TIME  (IR_HEADER_CODE_TIME-(0x0F-PM_IR_TH_VAL)*PM_IR_TH_GRID)
#define PM_IR_HDC_UPB_BOOT      irGetMaxCnt_BOOT(PM_IR_HEADER_CODE_TIME, 0.6)
#define PM_IR_HDC_LOB_BOOT      irGetMinCnt_BOOT(PM_IR_HEADER_CODE_TIME, 0.6)
//*************************************************************************

#endif
