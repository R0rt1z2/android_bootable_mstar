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

#include <command.h>
#include <CusUpgrade.h>
#include <config.h>


#if (ENABLE_MODULE_USB == 1)
#if defined(CONFIG_INSTALL_CHECK_ENABLE) && (CONFIG_INSTALL_CHECK_ENABLE==1)
U_BOOT_CMD(
    installcheck, CONFIG_SYS_MAXARGS, 1,    do_installcheck,
    "installcheck - Do installation check\n",
    NULL
);
#endif

U_BOOT_CMD(
    custar ,    CONFIG_SYS_MAXARGS,    1,     do_custar,
    "do usb update from the specified file that is in usb.\n",
    "    -  \n"
);
#if (ENABLE_MINIUBOOT==0)
U_BOOT_CMD(
    ota_zip_check ,    CONFIG_SYS_MAXARGS,    1,     do_ota_zip_check,
    "do OTA zip package check.\n",
    "ota_zip_check  -  check 'update_signed.zip' in usb_disk\n"
    "ota_zip_check emmc abspath  -  check abspath in emmc\n"
    "    -  \n"
);

U_BOOT_CMD(
    usb_bin_check ,    CONFIG_SYS_MAXARGS,    1,     do_usb_bin_check,
    "do usb bin document check.\n",
    "    -  \n"
);
#endif

#if defined(CONFIG_MMC)
U_BOOT_CMD(
    usb_partial_upgrade_to_emmc ,    CONFIG_SYS_MAXARGS,    1,     do_usb_partial_upgrade_to_emmc,
    "partial upgrade from USB to eMMC\n",
    "    -  \n"
);
#endif

#if (ENABLE_UTEST == 1)
U_BOOT_CMD(
    utest ,    CONFIG_SYS_MAXARGS,    1,     do_utest,
    "do usb update from the specified file that is in usb.\n",
    "    -  \n"
);
#endif

#if defined(CONFIG_CMD_CUSTOMER_UDSTAR) && (CONFIG_CMD_CUSTOMER_UDSTAR == 1)
U_BOOT_CMD(
    cudstar ,    CONFIG_SYS_MAXARGS,    1,     do_cudstar,
    "Execute the script file that is stored in usb disk\n",
    "    -  \n"
);
#endif


#endif

#if (ENABLE_MODULE_OAD == 1)
U_BOOT_CMD(
    costar ,    CONFIG_SYS_MAXARGS,    1,     do_costar,
    "do oad update from the specified file that is in oad partition\n",
    "    -  \n"
);
#endif

#if (ENABLE_MODULE_NETUPDATE== 1)
U_BOOT_CMD(
    cnstar ,    CONFIG_SYS_MAXARGS,    1,     do_cnstar,
    "do net update from the specified file that is in oad partition\n",
    "    -  \n"
);
#endif

U_BOOT_CMD(
    recovery_wipe_partition ,    CONFIG_SYS_MAXARGS,    1,     do_wipe_datacache_check,
    "do recovery wipe data or cache.\n",
    "    -  \n"
);
#if (ENABLE_MODULE_SYSTEM_RESTORE == 1)
U_BOOT_CMD(
    SystemRestore ,    CONFIG_SYS_MAXARGS,    1,     do_systemrestore,
    "do restore system by OTA upgrade package in systembackup partition.\n",
    "    -  \n"
);
#endif


#if (defined (CONFIG_URSA_6M40) || defined (CONFIG_UPGRADE_URSA))
U_BOOT_CMD(
    ursa_upgrade_mmc_check_update ,    CONFIG_SYS_MAXARGS,    1,     do_ursa_upgrade_mmc_check_update,
    "Check if a different ursa version occurs on eMMC\n",
    "    -  \n"
);
U_BOOT_CMD(
    ursa_upgrade ,    CONFIG_SYS_MAXARGS,    1,     do_ursa_upgrade,
    "ursa upgrade from USB\n",
    "    -  \n"
);

U_BOOT_CMD(
    ursa_reset ,    CONFIG_SYS_MAXARGS,    1,     do_ursa_reset,
    "ursa upgrade from USB\n",
    "    -  \n"
);
#endif

#if defined(CONFIG_CMD_WRITE_DATA_TO_RAW_PARTITION_ENABLE) && (CONFIG_CMD_WRITE_DATA_TO_RAW_PARTITION_ENABLE==1)
U_BOOT_CMD(
    checkcrc32,    CONFIG_SYS_MAXARGS,    1,     do_crc32check,
    "calculates crc32 of the file from given RAM address.\n",
    "    -  \n"
);
U_BOOT_CMD(
    writeDataToRawPartition,    CONFIG_SYS_MAXARGS,    1,     do_writeDataToRawPartition,
    "To write data to the specific raw partition.\n",
    "    -  \n"
);
#endif

/* this command is used for sec flashing in factory */
/* it won't run if sec flashing is not enabled */
U_BOOT_CMD(
    uupdate ,    CONFIG_SYS_MAXARGS,    1,     do_uupdate,
    "usb update\n",
    "    -  \n"
);
