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
#define IR_MODE_SEL             IR_TYPE_FULLDECODE_MODE
#define IR_CHANNEL_USE_AS_UPDOWN   0
#define IR_VOLUME_USE_AS_LEFTRIGHT      0
// IR Header code define
#define IR_HEADER_CODE0         0x80    // Custom 0     //0x04
#define IR_HEADER_CODE1         0x7F    // Custom 1     //0xFB

// IR Timing define
#define IR_HEADER_CODE_TIME     9000    // us
#define IR_OFF_CODE_TIME        4500    // us
#define IR_OFF_CODE_RP_TIME     2500    // us
#define IR_LOGI_01H_TIME        560     // us
#define IR_LOGI_0_TIME          1120    // us
#define IR_LOGI_1_TIME          2240    // us
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
#define IRKEY_DUMY              0xFF
#define IRDA_KEY_MAPPING_POWER  IRKEY_POWER

#define IR_LEADER_CODE_CHECKING_OPTION 0xBF
#define ENABLE_IR_MSTAR_SOURCE_HOTKEY   0
typedef enum _IrCommandType
{
    IRKEY_TV_ANTENNA        = 0x0C,
#if (ENABLE_DMP == DISABLE)
    IRKEY_TV_RADIO          = 0x03,//same as IRKEY_PAGE_UP
#endif
    IRKEY_CHANNEL_LIST      = 0x10,
    IRKEY_CHANNEL_FAV_LIST  = 0x08,
    IRKEY_CHANNEL_RETURN    = 0x5C,
    IRKEY_CHANNEL_PLUS      = 0x1F,
    IRKEY_CHANNEL_MINUS     = 0x19,

    IRKEY_AUDIO             = 0x44,
    IRKEY_VOLUME_PLUS       = 0x16,
    IRKEY_VOLUME_MINUS      = 0x15,

    IRKEY_UP                = 0x52,
    IRKEY_POWER             = 0x46,
    IRKEY_EXIT              = 0x1B,
    IRKEY_MENU              = 0x07,
    IRKEY_DOWN              = 0x13,
    IRKEY_LEFT              = 0x06,
    IRKEY_SELECT            = 0x0F,
    IRKEY_RIGHT             = 0x1A,

    IRKEY_NUM_0             = 0x50,
    IRKEY_NUM_1             = 0x49,
    IRKEY_NUM_2             = 0x55,
    IRKEY_NUM_3             = 0x59,
    IRKEY_NUM_4             = 0x4D,
    IRKEY_NUM_5             = 0x51,
    IRKEY_NUM_6             = 0x5D,
    IRKEY_NUM_7             = 0x48,
    IRKEY_NUM_8             = 0x54,
    IRKEY_NUM_9             = 0x58,

    IRKEY_MUTE              = 0x5A,
    IRKEY_FREEZE            = 0x4E,
#if ENABLE_DMP
    IRKEY_PAGE_UP           = 0x03,
#endif
    IRKEY_PAGE_DOWN         = 0x05,
    IRKEY_CLOCK             = 0x5F,

    IRKEY_INFO              = 0x14,
    IRKEY_RED               = 0x47,
    IRKEY_GREEN             = 0x4B,
    IRKEY_YELLOW            = 0x57,
    IRKEY_BLUE              = 0x5B,
    IRKEY_MTS               = 0x41,
    IRKEY_NINE_LATTICE      = IRKEY_DUMY,
    IRKEY_TTX               = 0x0A,
    IRKEY_CC                = 0x09,
    IRKEY_INPUT_SOURCE      = 0x04,
    IRKEY_CRADRD            = IRKEY_DUMY-1,
    IRKEY_PICTURE           = 0x40,
    IRKEY_ZOOM              = 0x4C,
#if ENABLE_IR_MSTAR_SOURCE_HOTKEY
    IRKEY_AV                = 0x4E,
    IRKEY_HDMI              = 0x02,
    IRKEY_SCART             = 0x1E,
    IRKEY_PC                = 0x01,
    IRKEY_DTV               = 0x0D,
    IRKEY_TV                = 0x11,
    IRKEY_COMPONENT         = 0x1D,
    IRKEY_SV                = 0X17,

    IRKEY_DASH              = IRKEY_DUMY-7,  // no use
    IRKEY_BACKWARD          = IRKEY_DUMY-8,  // no use
    IRKEY_FORWARD           = IRKEY_DUMY-9, // no use
    IRKEY_PLAY              = IRKEY_DUMY-10, // no use
    IRKEY_RECORD            = IRKEY_DUMY-11, // no use
    IRKEY_STOP              = IRKEY_DUMY-12, // no use
    IRKEY_PAUSE             = IRKEY_DUMY-13, // no use
    IRKEY_MEMORY_RECALL     = IRKEY_DUMY-14, // no use
#else
    IRKEY_DASH              = IRKEY_DUMY-15,
    IRKEY_BACKWARD          = 0x02,
    IRKEY_FORWARD           = 0x1E,
    IRKEY_PLAY              = 0x01,
    IRKEY_RECORD            = 0x0D,
    IRKEY_STOP              = 0x11,
    IRKEY_PAUSE             = 0x1D,
    IRKEY_MEMORY_RECALL       = 0X17,

    IRKEY_AV                = IRKEY_DUMY-7, // no use
    IRKEY_HDMI              = IRKEY_DUMY-8, // no use
    IRKEY_SCART             = IRKEY_DUMY-9, // no use
    IRKEY_PC                = IRKEY_DUMY-10, // no use
    IRKEY_DTV               = IRKEY_DUMY-11, // no use
    IRKEY_TV                = IRKEY_DUMY-12, // no use
    IRKEY_COMPONENT  = IRKEY_DUMY-13, // no use
    IRKEY_SV                = IRKEY_DUMY-14, // no use
#endif
    IRKEY_SLEEP             = 0x45,
    IRKEY_EPG               = 0x4A,

    IRKEY_P_CHECK           = 0x57,
    IRKEY_S_CHECK           = 0x5b,

      IRKEY_MIX               = 0x1C,
    IRKEY_INDEX             = 0x18,
    IRKEY_HOLD              = 0x00,

    IRKEY_PREVIOUS          = 0x12,
    IRKEY_NEXT              = 0x0E,
  //IRKEY_BRIGHTNESS_PLUS   = 0x0E,
  //IRKEY_BRIGHTNESS_MINUS  = 0x12,

    IRKEY_POWERONLY         = 0xFE,

    IRKEY_TTX_MODE          = 0x22,
    IRKEY_RED2              = 0x72,
    IRKEY_GREEN2            = 0x71,
    IRKEY_UPDATE            = 0x0C,
    IRKEY_SUBTITLE          = 0x53,
//  IRKEY_SUBCODE           = 0x53,
    IRKEY_TIME                 = 0x26,

    IRKEY_SIZE              = 0x43,
    IRKEY_REVEAL            = 0x4F,
    IRKEY_RETURN            = 0x00,
    IRKEY_ADJUST            = IRKEY_DUMY-2,
    IRKEY_TV_INPUT          = IRKEY_DUMY-3,
    IRKEY_KEY_DISABLE_KEYPAD = IRKEY_DUMY-4,
    IRKEY_BACK              = IRKEY_DUMY-5,
    IRKEY_SUBPAGE   =   IRKEY_DUMY-6,

    IRKEY_SUBCODE       = 0x5E,
    IRKEY_PHOTO           = 0x56,
    IRKEY_MINU             = 0x0B,
    IRKEY_GUIDE            = 0x42,


}IrCommandType;

#endif
