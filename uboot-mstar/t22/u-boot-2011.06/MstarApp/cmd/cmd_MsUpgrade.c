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

#include <MsUpgrade.h>
#include <MsUpgradeUtility.h>
#include <config.h>
#include <command.h>

#if (ENABLE_MODULE_NET==1)
U_BOOT_CMD(
    mstar ,    CONFIG_SYS_MAXARGS,    1,     do_mstar,
    "update kernal & root file system automatically by script file",
    "mstar [script_file_name]\n"
    "set CmdAbort = 0 in env to ignore the abort when cmd error\n"
);
#endif

#ifdef UFBL_FEATURE_ONETIME_UNLOCK
U_BOOT_CMD(
    onetimeunlock ,    CONFIG_SYS_MAXARGS,    1,     do_onetimeunlock,
    "onetimeunlock   - one time unlock\n",
	"[onetimeunlock getcode]\n"
	"[onetimeunlock setcode signed_code]\n"
	"[onetimeunlock setcert signed_cert]\n"
);
#endif

#if (ENABLE_MODULE_USB == 1)

U_BOOT_CMD(
    ustar ,    CONFIG_SYS_MAXARGS,    1,     do_ustar,
    "ustar   - update kernal & root file system automatically by script file\n",
    "[script_file_name] \n"
    "    - when use this command \n"
    "    - at first use usb start \n"
    "    - then use ustar \n"
    "    -  \n"
);

#if !((CONFIG_MINIUBOOT) || (CONFIG_SECURE_USB_UPGRADE))
U_BOOT_CMD(
    udstar ,    CONFIG_SYS_MAXARGS,    1,     do_udstar,
    "Execute the script file that is stored in usb disk\n",
    "[script_file_name] \n"
    "If you don't input a file name, udstar will use the defatult file name \n"    
);
#endif

#endif

#if (ENABLE_MODULE_NETUPDATE == 1)
U_BOOT_CMD(
    nstar ,    CONFIG_SYS_MAXARGS,    1,     do_nstar,
    "nstar   - update softwave system automatically via net\n",
   NULL
);
#endif

#if CONFIG_RESCUE_ENV
U_BOOT_CMD(
    bstar ,    CONFIG_SYS_MAXARGS,    1,     do_bstar,
    "bstar   - update softwave system automatically via backup\n",
    NULL
);
#endif

#if (ENABLE_MODULE_OAD == 1)
U_BOOT_CMD(
    ostar ,    CONFIG_SYS_MAXARGS,    1,     do_ostar,
    "ostar   - update softwave system automatically via oad\n",
    NULL
);
#endif

#if (ENABLE_MODULE_ENV_UPGRADE_FROM_BANK == 1)
U_BOOT_CMD(
    rstar ,    CONFIG_SYS_MAXARGS,    1,     do_rstar,
    "do env update from the lastest spi bank.\n",
    "    -  \n"
);
#endif

#if CONFIG_MINIUBOOT
#else
#if ( (ENABLE_MODULE_OAD == 1) || (ENABLE_MODULE_NETUPDATE == 1) || (ENABLE_MODULE_USB == 1))
U_BOOT_CMD(
    mbup ,    CONFIG_SYS_MAXARGS,    1,     do_mboot_upgrade,
    "mboot upgrade\n",
    "[mboot dram buffer addr] [mboot size] \n"
);
#endif
#endif

#if defined(CONFIG_MSTAR_NS_UBOOT)
U_BOOT_CMD(
    kbup ,    CONFIG_SYS_MAXARGS,    1,     do_keybank_upgrade,
    "kbup   - update key bank\n",
    "kbup [KeyBankName] [dram buffer addr]\n"
    "    - kbup TEE     0x25000000 (TEE KeyBank)\n"
    "    - kbup REE     0x25000000 (REE KeyBank)\n"
    "    - kbup KL      0x25000000 (Keyladder KeyBank)\n"
    "    - kbup KEYTYPE 0x25000000 (KeyType)\n"
);
#endif

#if defined (CONFIG_URSA_6M60)
U_BOOT_CMD(terra2_fast_upgrade, CONFIG_SYS_MAXARGS, 1, do_terra2_fast_upgrade,
    "terra2_fast_upgrade - update terra2 by uart1\n",
    "Usage:\n"
    "terra2_fast_upgrade - update bin file in USB, named Terra2Upgrade.bin\n"
    "terra2_fast_upgrade [dram_addr] [dram_size] - update bin file in dram\n"
);
#endif


#ifdef CONFIG_AN_FASTBOOT_ENABLE
U_BOOT_CMD(
    sparse_write ,    CONFIG_SYS_MAXARGS,    4,     do_sparse_write,
    "do sparse image writing to target flash type.\n",
    "Usage:\n"
    "sparse_write [flash_type] [dram_addr] [partitionname] [image_size]"
);
#endif
#if (ENABLE_MODULE_NET == 1)
U_BOOT_CMD(
    auto_tftp ,    CONFIG_SYS_MAXARGS,    4,     do_auto_tftp,
    "do auto-address tftp.\n",
    "Usage:\n"
    "auto_tftp [image_name]\n"
    "ex. auto_tftp mboot.bin\n"
);

U_BOOT_CMD(
    auto_flash ,    CONFIG_SYS_MAXARGS,    6,     do_auto_flash,
    "do auto flash image.\n",
    "Usage:\n"
    "auto_flash [flash_type] [flash_command] [partitionname] ([empty_skip]<0/1>)\n"
    "ex. auto_flash mmc write.p MBOOT\n"
);
#endif
