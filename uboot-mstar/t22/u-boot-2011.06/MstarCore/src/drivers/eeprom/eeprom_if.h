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

#ifndef EEPROM_IF_H
#define EEPROM_IF_H

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------
//#include "wrap.h"
#include <MsTypes.h>
#include <command.h>

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

typedef enum __EEPDTV_Offset_T{
    EEPDTV_DTVCFG_MAGIC1 = 0,
} EEPDTV_Offset_T;

// DTVCFG_T Offset 0.
typedef struct __DtvCfg {
    MS_U8 u1Magic1;         //< MTK fix value = 0x53.
    MS_U8 u1Magic2;         //< MTK fix value = 0x81.
    MS_U8 u1Upgrade;
    MS_U8 u1Upgrade_Ext;
    MS_U8 u1Flags;			//< MTK internal setting value.
    MS_U8 u1Flags2;
    MS_U8 u1Checksum;
} DTVCFG_T;

// DTVCFG parameter macro.
#define DTVCFG_MAGIC_CHAR1                  (0x53)
#define DTVCFG_MAGIC_CHAR2                  (0x81)

// DTVCFG flag for system configuration.
#define DTVCFG_FLAG_POWERON_BOOT            (1U << 0)   /* Map to PC_MISC_CTRL_NO_STANDBY, if 1, just boot, otherwise standby if ext_status on */
#define DTVCFG_FLAG_FACTORY_MODE            (1U << 1)   /* Map to PC_MISC_CTRL_FACTORY_MODE */
#define DTVCFG_FLAG_POWERON_LAST            (1U << 2)   /* Map to PC_MISC_CTRL_NO_STANDBY_2, if 1, read system_state_on to decide boot or standby */
#define DTVCFG_FLAG_8280_UPGRADE_ON         (1U << 3)   /* Map to PC_MISC_CTRL_COMPANION_CHIP_UPGRADE */
#define DTVCFG_FLAG_POWER_BTN_ON            (1U << 4)   /* Use to record power button count, 1 is on, 0 is off */
#define DTVCFG_FLAG_SYSTEM_STATE_ON         (1U << 5)   /* Use to record previous system state, 1 is on, 0 is off. */
#define DTVCFG_FLAG_POWER_LEDOFF            (1U << 6)   /* Map to PC_MISC_CTRL_POWER_LED_OFF */
#define DTVCFG_FLAG_USB_UPGRADE_ON          (1U << 7)   /* Map to PC_MISC_CTRL_FIRMWARE_UPGRADE */

// DTVCFG flags2 for system configuration.
#define DTVCFG_FLAG2_IMAGE_BANK             (1U << 0)
#define DTVCFG_FLAG2_8032UP_BANK            (1U << 1)
#define DTVCFG_FLAG2_QUIET_BOOT             (1U << 2)   /* Map to PC_MISC_CTRL_RTC_QUIET_BOOT, if 1, set quiet boot. */
#define DTVCFG_FLAG2_DISABLE_LOGO           (1U << 3)   /* Use to record logo display state, if 1, disable logo. otherwise show logo */
#define DTVCFG_FLAG2_FLIP_ON                (1U << 4)   /* FLIP_ENABLE */
#define DTVCFG_FLAG2_MIRROR_ON              (1U << 5)   /* MIRROR_ENABLE*/
#define DTVCFG_FLAG2_FP_LOCK                (1U << 6)   /* FP_LOCK, default 0. if 1 keypad cannot wakeup from standby. */
#define DTVCFG_FLAG2_UPGRADE_GO	            (1U << 7)   /* Use to record the usb upgrade status. */
#if 0
#define EEPROM_DTV_DRIVER_SIZE              (DRVCUST_InitGet(eEepromDtvDriverSize))
#define EEPROM_DTV_DRIVER_OFFSET            (DRVCUST_InitGet(eEepromDtvDriverOffset))

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
extern INT32 EEPROM_Read(UINT64 u8Offset, unsigned long u4MemPtr, UINT32 u4MemLen);
extern INT32 EEPROM_Write(UINT64 u8Offset, unsigned long u4MemPtr, UINT32 u4MemLen);

extern INT32 EEPDTV_Read(UINT64 u8Offset, UPTR u4MemPtr, UINT32 u4MemLen);
extern INT32 EEPDTV_Write(UINT64 u8Offset, UPTR u4MemPtr, UINT32 u4MemLen);

extern INT32 EEPDTV_GetCfg(DTVCFG_T* prDtvCfg);
extern INT32 EEPDTV_SetCfg(DTVCFG_T* prDtvCfg);
#endif // if 0
MS_BOOL EEPRON_IsQuiteHotBootMode(void) ;

#endif

