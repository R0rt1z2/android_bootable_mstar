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
#include <common.h>
#include <MsTest.h>
#include <MsEnvironment.h>
#include <MsApiPM.h>

#if CONFIG_MINIUBOOT
#else

#if defined (CONFIG_POINTS_HANDLER_ENABLE)
U_BOOT_CMD(
    add_bootcp, CONFIG_SYS_MAXARGS, 1,    do_add_bootcheckpoints,
    "add_bootcp   - Perform to enable the booting check-points(reboot 15 times, then auto disable)\n",
    NULL
);
#endif

U_BOOT_CMD(
    testmode , CONFIG_SYS_MAXARGS, 1,    do_testmode_set ,
    "set testmode into bootargs.",
    "testmode <mode>\n"
    "mode:autotest,moduletest,hsl,mdebugtest"
);

#endif

U_BOOT_CMD(
    bootargs_set,    CONFIG_SYS_MAXARGS,    1,     do_set_bootargs,
    "Set info exchange and set to boot args. ",
    "    -  \n"
);

#if CONFIG_MINIUBOOT
#else
U_BOOT_CMD(
    config2env,    CONFIG_SYS_MAXARGS,    1,     do_config2env,
    "Set config to environment.",
    "    -  \n"
);
#endif

#if (CONFIG_ENABLE_RTPM)
#ifdef CONFIG_MSTAR_STR_MINISIZE
U_BOOT_CMD_STR(
#else
U_BOOT_CMD(
#endif
    runtime_pm, CONFIG_SYS_MAXARGS, 1,    do_run_time_pm,
    "if boot to PM\n",
    NULL
);
#endif

#if CONFIG_MINIUBOOT
#else
U_BOOT_CMD(
    dc, 2, 1, do_del_boogargs_cfg,
    "delete the specific cofig that is in the bootargs\n",
    "[config]\n"
    "For example: dl autotest\n"    
);

U_BOOT_CMD(
    ac, 3, 1, do_set_bootargs_cfg,
    "set a new config to the bootargs\n",
    "[config][content]\n"
    "For example: add autotest true\n"    
);

#ifdef CONFIG_ENABLE_SYNC_MMAP
U_BOOT_CMD(
    sync_mmap, 1, 1, do_sync_mmap_to_env,
    "Sync the some id's address and len to env and bootargs from supernova's mmap\n",
    "\n"
);
#endif

U_BOOT_CMD(
    m2e, 4, 1, do_mmap_to_env,
    "Restore the address and len to env from supernova's mmap\n",
    "[ID] [n1] [n2]"
    "For example:"
    "ID:E_MMAP_ID_PM51_USAG"
    "n1: This is an name of env, this env store the addr for ID"
    "n2: This is an name of env, this env store the len for ID"
    "\n"
);

#if !defined(CONFIG_ANDROID_BOOT) && !defined(CONFIG_MI_CONFIG_PARTITION)
U_BOOT_CMD(
    reload_mac, 1, 1, do_reload_MAC_to_env,
    "Reload mac address from supernova to env\n",
    "\n"
);
#endif

#endif

