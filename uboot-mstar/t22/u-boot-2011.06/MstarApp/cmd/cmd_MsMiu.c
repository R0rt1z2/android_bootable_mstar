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

#include <common.h>
#include <command.h>

#include <MsApiMiu.h>

#if CONFIG_MINIUBOOT
#else
U_BOOT_CMD(
    kernelProtect, 1, 1, do_MsApi_kernelProtect,
    "kernelProtect  - Protect kernel\n",
    "command: kernelProtect\n"
);

U_BOOT_CMD(
    pmProtect, 1, 1, do_MsApi_RunTimePmProtect,
    "runtime pm Protect  - Protect runtime PM\n",
    "command: pmProtect\n"
);

U_BOOT_CMD(
    kernelProtectBist, 1, 1, do_MsApi_kernelProtectBist,
    "kernelProtectBist  - Protect kernel bist\n",
    "command: kernelProtectBist\n"
);

U_BOOT_CMD(
    miuProtect, 1, 1, do_MsApi_MiuProtect,
    "miuProtect  - Protect miu\n",
    "command: miuProtect\n"
);


U_BOOT_CMD(
    nuttxProtect, 3, 1, do_MsApi_NuttxProtect,
    "nuttx Protect  - Protect nuttx\n",
    "command: nuttxProtect [start addr] [end addr]\n"
);
#endif

#ifdef CONFIG_MSTAR_STR_MINISIZE
U_BOOT_CMD_STR(
#else
U_BOOT_CMD(
#endif
    updatemiureg ,    CONFIG_SYS_MAXARGS,    1,     do_set_miu,
    "Call driver to update miu setting",
    "    - By Env MIU0(1)_GROUP_PRIORITY \n"
);