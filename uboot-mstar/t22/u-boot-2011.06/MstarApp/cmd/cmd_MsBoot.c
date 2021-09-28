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

////////////////////////////////////////////////////
///    Header File
////////////////////////////////////////////////////
#include <command.h>
#include <MsBoot.h>
U_BOOT_CMD(
    bootcheck, CONFIG_SYS_MAXARGS, 1,    do_bootcheck,
    "bootcheck   - Do boot check\n",
    NULL
);
#ifdef CONFIG_SECARM_OPTEE
extern do_sec_test();
U_BOOT_CMD(
    sectest, CONFIG_SYS_MAXARGS, 1,    do_sec_test,
    "bootcheck   - Do boot check\n",
    NULL
);
#endif

#if ENABLE_STB_ECOS_BOOT
U_BOOT_CMD(
    read_boot_info, CONFIG_SYS_MAXARGS, 1,    do_read_boot_info,
    "read_boot_info - read eCos boot info\n",
    NULL
);
#endif

#if defined(CONFIG_SEC_SYSTEM)
U_BOOT_CMD(
    Boot2secKernel, CONFIG_SYS_MAXARGS, 1,    do_Boot2secKernel,
    "Boot2secKernel   - jump to non-TEE second kernel\n",
    NULL
);
#endif

#ifdef CONFIG_ARMv8_ARM_TRUSTED_FIRMWARE
U_BOOT_CMD(
    smc, CONFIG_SYS_MAXARGS, 1,    do_smc,
    "smc   - jump to arm_trusted_firmware\n",
    NULL
);
#endif

#if defined (CONFIG_ARMv8_ARM_TRUSTED_FIRMWARE)|| defined(CONFIG_ARMv7_OPTEE)

#ifdef CONFIG_MSTAR_STR_MINISIZE
U_BOOT_CMD_STR(
#else
U_BOOT_CMD(
#endif
    readArmFw, CONFIG_SYS_MAXARGS, 1,    do_read_arm_fw,
    "readArmFw   - read Nuttx to malloc buffer \n",
    NULL
);

#ifdef CONFIG_MSTAR_STR_MINISIZE
U_BOOT_CMD_STR(
#else
U_BOOT_CMD(
#endif
    readOptee, CONFIG_SYS_MAXARGS, 1,    do_read_optee,
    "readOptee   - core1 run at Nuttx \n",
    NULL
);
#endif


#if defined (CONFIG_DUAL_SYSTEM)
U_BOOT_CMD(
    bootflag_read, CONFIG_SYS_MAXARGS, 1,    do_bootflag_read,
    "bootflag_read   - Read bootflag.\n",
    NULL
);

U_BOOT_CMD(
    bootflag_set, CONFIG_SYS_MAXARGS, 1,    do_bootflag_set,
    "bootflag_set   - set bootflag to choose system0 or system1.\n",
    NULL
);

U_BOOT_CMD(
    sboot_verify_check, CONFIG_SYS_MAXARGS, 1,    do_sboot_verify_check,
    "sboot_verify_check   - Do sboot_verify_check\n",
    NULL
);

U_BOOT_CMD(
    copy_mpool, CONFIG_SYS_MAXARGS, 1,    do_copy_mpool,
    "copy_mpool   - Do bootflag check\n",
    NULL
);

U_BOOT_CMD(
    mboot_verify_check, CONFIG_SYS_MAXARGS, 1,    do_mboot_verify_check,
    "mboot_verify_check   - Do mboot_verify_check\n",
    NULL
);
#endif

U_BOOT_CMD(
    boot_led_init, CONFIG_SYS_MAXARGS, 1,    do_boot_led_init,
    "boot_led_init   - Do boot_led_init\n",
    NULL
);
