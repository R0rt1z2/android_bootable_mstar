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

#include <MsUpgradeUtility.h>
#include <config.h>
#include <command.h>

#if(ENABLE_MODULE_DONT_OVERWRITE==1)
U_BOOT_CMD(dont_overwrite_init, 1, 0, do_dont_overwrite_init,
            "This serial apis are for protecting the specific partitions are overwrited. ex: customer\n"
            "This cmd is used to init this function\n",
            "\n");

U_BOOT_CMD(dont_overwrite, 2, 0, do_dont_overwrite,
            "This serial apis are for protecting the specific partitions are overwrited. ex: customer\n"
            "This cmd is used to register which partition should not be overwrite\n",
            "[partition name] \n");

U_BOOT_CMD(dont_overwrite_free, 1, 0, do_dont_overwrite_free,
            "This serial apis are for protecting the specific partitions are overwrited. ex: customer\n"
            "This cmd is used to free all registered partitions.\n",
            "\n");

U_BOOT_CMD(force_overwrite_init, 1, 0, do_force_overwirte_init,
            "This serial apis are for force write and re-create partitions.\n"
            "This cmd is used to register which partition should force overwrite\n",
            "\n");

U_BOOT_CMD(force_overwrite, 2, 0, do_force_overwrite,
            "This serial apis are for force write and re-create partitions.\n"
            "This cmd is used to register which partition should force overwrite\n",
            "[partition name] \n");

U_BOOT_CMD(dont_overwrite_switch, 2, 0, do_dont_overwrite_switch,
            "This serial apis are for force write and re-create partitions.\n"
            "This cmd is used to disable dont_overwrite function.\n",
            "[enable/disable] \n");
#endif

U_BOOT_CMD
(
    update_mode, 3, 0, do_rw_en_update_mode,
    "get/set update_mode \n",
    "<g/s> [EN_UPDATE_MODE]\n"
    "EN_UPDATE_MODE:\n'EN_UPDATE_MODE_NONE' 'EN_UPDATE_MODE_TFTP' 'EN_UPDATE_MODE_USB' 'EN_UPDATE_MODE_OAD'\n'EN_UPDATE_MODE_NET' 'EN_UPDATE_MODE_USB_WITH_SEG_DECRYPTED' 'EN_UPDATE_MODE_NET_WITH_SEG_DECRYPTED'\n" 
);

U_BOOT_CMD(filepartload, 5, 0, do_file_part_load,
    "load part of a file to RAM",
    "<addr> <filename> [bytes]\n"
    "    - load file 'filename' to address 'addr'\n");

U_BOOT_CMD(filepartloadSeg, 8, 0, do_file_part_load_with_segment,
    "load a seg part of file to RAM",
    "filepartloadSeg interface(hex) device(hex) addr(hex) filename(hex) offset(hex) bytes(hex)\n"
);

#if CONFIG_MINIUBOOT
#else
U_BOOT_CMD
(
    checkfile, CONFIG_SYS_MAXARGS, 0, do_check_file_partition,
    "check file exist in u disk,and set the partition.",
    "<Dir>\n"
    "    - check a file in U disk Dir exist or not\n"
);
#endif

#if defined (CONFIG_R2_BRINGUP) || defined (CONFIG_AMP_TRUSTZONE_ENABLE)
U_BOOT_CMD
(
    UpdateNuttx, CONFIG_SYS_MAXARGS, 0, do_update_nuttx,
    "upgrade nuttx.",
    "<Dir>\n"
    "    - upgrade nuttx\n"
);
#endif
#ifdef HB_CUS
U_BOOT_CMD(
    cusid ,  6,    0,     do_checkOUIValid,
    "cusid check the image is release by the valid guys\n",
    "command cusid <oui> <swModel> <swVer> <ChipVersion> <BoardSel>\n"
);
#endif

