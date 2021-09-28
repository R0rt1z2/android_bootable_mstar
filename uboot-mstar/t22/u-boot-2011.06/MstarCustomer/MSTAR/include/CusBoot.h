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

#ifndef __CUS_BOOT_H__
#define __CUS_BOOT_H__

#ifndef  CUS_IR_HEAD_FILE//Please define it in board file for customization
#include "IR_MSTAR_DTV.h" 
#else
#include CUS_IR_HEAD_FILE
#endif

#if (ENABLE_MODULE_BOOT_IR == 1)
#if (EANBLE_Customer)
#if (ENABLE_CUS18)
#define IR_RECOVERY_KEY             IRKEY_MENU
#define IR_UPGRADEBOOTLOADER_KEY    IRKEY_EXIT
#define IR_FORCEUGRADE_KEY          IRKEY_HOME
#define IR_UPGRADEUSBOTA_KEY        IRKEY_VOLUME_PLUS
#define IR_RECOVERYWIPEDATA_KEY     IRKEY_CHANNEL_PLUS
#define IR_RECOVERYWIPECACHE_KEY    IRKEY_CHANNEL_MINUS
#elif (ENABLE_CUS19)
#define IR_RECOVERY_KEY             IRKEY_RESERVE0
#define IR_UPGRADEBOOTLOADER_KEY    IRKEY_RESERVE1
#define IR_FORCEUGRADE_KEY          IRKEY_RESERVE2
#define IR_UPGRADEUSBOTA_KEY        IRKEY_RESERVE3
#define IR_RECOVERYWIPEDATA_KEY     IRKEY_RESERVE4
#define IR_RECOVERYWIPECACHE_KEY    IRKEY_RESERVE5
#elif (ENABLE_CUS23)
#define IR_RECOVERY_KEY             IRKEY_EXIT
#define IR_UPGRADEBOOTLOADER_KEY    IRKEY_VOLUME_PLUS
#define IR_FORCEUGRADE_KEY          IRKEY_MENU
#elif (ENABLE_CUS28)
#define IR_RECOVERY_KEY             IRKEY_INPUT_SOURCE
#define IR_UPGRADEBOOTLOADER_KEY    IRKEY_VOLUME_MINUS
#define IR_FORCEUGRADE_KEY          IRKEY_POWER
#else
#define IR_RECOVERY_KEY             IRKEY_EXIT
#define IR_UPGRADEUSBOTA_KEY        IRKEY_MENU  
#define IR_UPGRADEBOOTLOADER_KEY    IRKEY_VOLUME_MINUS
#define IR_FORCEUGRADE_KEY          IRKEY_VOLUME_PLUS
#define IR_RECOVERYWIPEDATA_KEY     IRKEY_CHANNEL_PLUS
#define IR_RECOVERYWIPECACHE_KEY    IRKEY_CHANNEL_MINUS
#endif
#else
#define IR_RECOVERY_KEY             IRKEY_EXIT
#define IR_UPGRADEUSBOTA_KEY        IRKEY_MENU  
#define IR_UPGRADEBOOTLOADER_KEY    IRKEY_VOLUME_MINUS
#define IR_FORCEUGRADE_KEY          IRKEY_VOLUME_PLUS
#define IR_RECOVERYWIPEDATA_KEY     IRKEY_CHANNEL_PLUS
#define IR_RECOVERYWIPECACHE_KEY    IRKEY_CHANNEL_MINUS
#if defined (CONFIG_AN_FASTBOOT_ENABLE)
#define IR_FASTBOOT_KEY             IRKEY_SELECT
#endif
#endif

#if CONFIG_RESCUE_ENV
#define IR_BRICK_TERMINATOR_RECOVERY_KEY1             IRKEY_RED
#define IR_BRICK_TERMINATOR_RECOVERY_KEY2             IRKEY_GREEN
#define IR_BRICK_TERMINATOR_RECOVERY_KEY_REPEAT_REQUIRED 4
#endif

#if CONFIG_SEC_SYSTEM
#define IR_SECOND_SYSTEM_KEY       IRKEY_YELLOW
#endif
#endif


#if (ENABLE_MODULE_BOOT_KEYPAD == 1)
#if (ENABLE_Customer)
#if (ENABLE_CUS18)
#define KEYPAD_RECOVERY_KEY             IRKEY_INPUT_SOURCE
#define KEYPAD_UPGRADEBOOTLOADER_KEY    IRKEY_MENU
#define KEYPAD_FORCEUGRADE_KEY          IRKEY_KEYPAD_VOLUME_PLUS //HIS USB Update
#define KEYPAD_UART_DEBUG_KEY		    IRKEY_VOLUME_MINUS
#elif (ENABLE_CUS19)
#define KEYPAD_RECOVERY_KEY             IRKEY_RESERVE0
#define KEYPAD_UPGRADEBOOTLOADER_KEY    IRKEY_RESERVE1
#define KEYPAD_FORCEUGRADE_KEY          IRKEY_POWER
#define KEYPAD_UART_DEBUG_KEY			IRKEY_RESERVE2
#elif (ENABLE_CUS23)
#define KEYPAD_RECOVERY_KEY             IRKEY_INPUT_SOURCE
#define KEYPAD_UPGRADEBOOTLOADER_KEY    IRKEY_VOLUME_PLUS
#define KEYPAD_FORCEUGRADE_KEY          IRKEY_MENU
#define KEYPAD_UART_DEBUG_KEY			IRKEY_VOLUME_MINUS
#elif (ENABLE_CUS28)
#define KEYPAD_RECOVERY_KEY             IRKEY_INPUT_SOURCE
#define KEYPAD_UPGRADEBOOTLOADER_KEY    IRKEY_VOLUME_MINUS
#define KEYPAD_FORCEUGRADE_KEY          IRKEY_POWER
#define KEYPAD_UART_DEBUG_KEY			IRKEY_MENU
#else
#define KEYPAD_RECOVERY_KEY             IRKEY_INPUT_SOURCE
#define KEYPAD_UPGRADEBOOTLOADER_KEY    IRKEY_MENU
#define KEYPAD_FORCEUGRADE_KEY          IRKEY_VOLUME_PLUS
#define KEYPAD_UART_DEBUG_KEY			IRKEY_VOLUME_MINUS
#if (ENABLE_MODULE_SYSTEM_RESTORE == 1)
#define KEYPAD_SYSTEM_RESTORE_KEY		IRKEY_CHANNEL_MINUS
#endif // #if (ENABLE_MODULE_SYSTEM_RESTORE == 1)
#endif // #if (ENABLE_CUS18)
#else
#ifndef KEYPAD_RECOVERY_KEY
#define KEYPAD_RECOVERY_KEY             IRKEY_INPUT_SOURCE
#endif // #ifndef KEYPAD_RECOVERY_KEY
#define KEYPAD_UPGRADEBOOTLOADER_KEY    IRKEY_MENU
#define KEYPAD_FORCEUGRADE_KEY          IRKEY_VOLUME_PLUS
#define KEYPAD_HOLD_VALUE               1100  // 1100 -> 1.1 seconds
#define KEYPAD_UART_DEBUG_KEY			IRKEY_VOLUME_MINUS
#if (ENABLE_MODULE_SYSTEM_RESTORE == 1)
#define KEYPAD_SYSTEM_RESTORE_KEY		IRKEY_CHANNEL_MINUS
#endif // #if (ENABLE_MODULE_SYSTEM_RESTORE == 1)
#if defined (CONFIG_AN_FASTBOOT_ENABLE)
#define KEYPAD_FASTBOOT_KEY             IRKEY_CHANNEL_PLUS
#endif // #if defined (CONFIG_AN_FASTBOOT_ENABLE)
#endif // #if (ENABLE_Customer)
#endif // #if (ENABLE_MODULE_BOOT_KEYPAD == 1)


#endif

