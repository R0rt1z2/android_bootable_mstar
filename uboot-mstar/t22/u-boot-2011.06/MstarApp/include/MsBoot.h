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

#ifndef _Mstar_Boot_Header_
#define _Mstar_Boot_Header_
#include <CusBoot.h>
#include <ShareType.h>

#if defined (CONFIG_SECURITY_BOOT)
#define BOOT_COMMAND "authenticate recovery 0x25000000; bootm 0x25000000"
#else
  #if (ENABLE_MODULE_MMC == 1)
  #define BOOT_COMMAND "mmc read.p 0x45000000 recovery 0x00800000; bootm   0x45000000"
  #else
  #define BOOT_COMMAND "nand read.e 0x45000000 recovery 0x00800000; bootm 0x45000000"
  #endif
#endif

typedef enum
{
    EN_BOOT_MODE_UNKNOWN,
    EN_BOOT_MODE_NORMAL,
    EN_BOOT_MODE_RECOVERY,
    EN_BOOT_MODE_FORCEUGRADE,
    EN_BOOT_MODE_USB_UPGRADE,
    EN_BOOT_MODE_NET_UPGRADE,
    EN_BOOT_MODE_OAD_UPGRADE,
    EN_BOOT_MODE_UPDATELOGIC_UPGRADE,
#if (ENABLE_MODULE_ENV_UPGRADE_FROM_BANK == 1)
    EN_BOOT_MODE_ENV_UPGRADE,
#endif
    EN_BOOT_MODE_USB_RECOVRY_UPGRADE,
    EN_BOOT_MODE_OTA_UPGRADE,
    EN_BOOT_MODE_RECOVRY_WIPE_DATA,
    EN_BOOT_MODE_RECOVRY_WIPE_CACHE,
    EN_BOOT_MODE_UART_DEBUG,
#if CONFIG_RESCUE_ENV
    EN_BOOT_MODE_BRICK_TERMINATOR,
#endif
#if (ENABLE_MODULE_SYSTEM_RESTORE == 1)
    EN_BOOT_MODE_SYSTEM_RESTORE,
#endif
#if defined (CONFIG_SEC_SYSTEM)
    EN_BOOT_MODE_SECOND_SYSTEM,
#endif
#if defined (CONFIG_AN_FASTBOOT_ENABLE)
   EN_BOOT_MODE_FASTBOOT,
#endif
    EN_BOOT_MODE_UPDATE_BOOTLOGO,
    EN_BOOT_MODE_RECOVERY_IN_SECONDARY,
    EN_BOOT_MODE_FASTBOOT_RECOVERY, //Try fastboot, then USB upgrade, then recovery mode
}EN_BOOT_MODE;

#if (CONFIG_DYNAMIC_IR)
///----------------------------------------------------------------------//
//--------------------------IR Bootcheck -----------------------------------//

#define IR_MAX 5

#define IR_KEY_MAX 16

typedef enum
{
    IR_BOOTCASE_INVAILUE=0x00,
    IR_BOOTCASE_RECOVERY,
    IR_BOOTCASE_PGRADEUSBOTA,
    IR_BOOTCASE_FORCEUPGRADE,
    IR_BOOTCASE_UPGRADEBOOTLOADER,
    IR_BOOTCASE_RECOVERYWIPEDATA,
    IR_BOOTCASE_RECOVERYWIPECACHE,
    IR_BOOTCASE_SECOND_SYSTEM,
    IR_BOOTCASE_FASTBOOT,
    IR_BOOTCASE_BRICK_TERMINATOR_RECOVERY,
    IR_BOOTCASE_BOOTCASE_MAX
} eIRBootCase;

typedef struct {
    U8 u8Protocol;
    U32 u32Headcode;
    U16 u16Key[IR_KEY_MAX];
}IRBootCheckInfo_t;

typedef IRBootCheckInfo_t st_IRBootCheck_t[IR_MAX];
///----------------------------------------------------------------------//
//--------------------------IR wakeup info  -----------------------------------//

typedef struct {
    U8 u8Protocol;
    U32 u32Headcode;
    U16 u16Key;
}IRWakeup_t;

typedef IRWakeup_t st_IRWakeup_t[IR_MAX];

#endif
#if defined(CONFIG_DATA_SEPARATION)
#define KEYPAD_MAX  5
typedef char KeypadWakeup_t;
typedef KeypadWakeup_t st_KeypadWakeup_t[KEYPAD_MAX];
#endif

#if defined (CONFIG_DUAL_SYSTEM)
#define DUAL_SYSTEM_PARTITION_SUFFIX "1"
#define DUAL_MBOOT_PARTITION_SUFFIX "BAK"

unsigned char read_bootflag(void);
int do_bootflag_read (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_bootflag_set(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_sboot_verify_check(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_copy_mpool(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_mboot_verify_check(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
#endif
int do_bootcheck (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int isBootToRecovery(void);
int is_quiescent_mode(void);
int do_boot_led_init(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);

#if ENABLE_STB_ECOS_BOOT
int do_read_boot_info (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
#endif

#if defined(CONFIG_SEC_SYSTEM)
int do_Boot2secKernel (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
#endif

#ifdef CONFIG_ARMv8_ARM_TRUSTED_FIRMWARE
int do_smc (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
#endif
#if defined (CONFIG_ARMv8_ARM_TRUSTED_FIRMWARE)|| defined(CONFIG_ARMv7_OPTEE)
int do_read_arm_fw (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_read_optee (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
#endif

#endif
