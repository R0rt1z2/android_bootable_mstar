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

#ifndef __CMD_CUS_UPGRADE_H__
#define __CMD_CUS_UPGRADE_H__
#include <common.h>
#include <MsTypes.h>
#include <MsUboot.h>

#define URSA_VERSION_ENV             "ursa_version"

typedef enum
{
    E_MFC_FIRMWARE = 0,
    E_MFC_SYSTEM_BIN
} EN_MFC_UPGRADE_SEL_t;

int do_custar(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_costar(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_cnstar(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_ota_zip_check(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_usb_bin_check(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_wipe_datacache_check(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_systemrestore(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_ursa_upgrade_mmc_check_update(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_ursa_upgrade(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_ursa_reset(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_utest(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_uupdate(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
#if defined(CONFIG_URSA20_SYSBIN)
int do_Ursa_20_Upgrade_SysBin(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
#endif

#if defined(CONFIG_MMC)
int do_usb_partial_upgrade_to_emmc(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
#endif

#if defined(CONFIG_CMD_CUSTOMER_UDSTAR) && (CONFIG_CMD_CUSTOMER_UDSTAR == 1)
int do_cudstar (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
#endif

#if defined(CONFIG_CMD_WRITE_DATA_TO_RAW_PARTITION_ENABLE) && (CONFIG_CMD_WRITE_DATA_TO_RAW_PARTITION_ENABLE==1)
int do_crc32check(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_writeDataToRawPartition(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
#endif

#if defined(CONFIG_INSTALL_CHECK_ENABLE) && (CONFIG_INSTALL_CHECK_ENABLE==1)
int do_installcheck(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
#endif

#if defined(CONFIG_URSA20_SYSBIN)
int Ursa_20_Upgrade_SysBin();
#endif

int save_first_boot_status(void);
int get_first_boot_status(void);
void clear_first_boot_status(void);

#endif

