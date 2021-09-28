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
#include <MsRawIO.h>

#ifdef CONFIG_MSTAR_STR_MINISIZE
U_BOOT_CMD_STR(
#else
U_BOOT_CMD(
#endif
    init_raw_io, CONFIG_SYS_MAXARGS, 0, do_raw_io_init,
    "init raw_io module",
    " \n"
);

#ifdef CONFIG_MSTAR_STR_MINISIZE
U_BOOT_CMD_STR(
#else
U_BOOT_CMD(
#endif
    config_raw_io, CONFIG_SYS_MAXARGS, 0, do_raw_io_config,
    "Config the target device for raw I/O",
    "SPI \n"
	"config_raw_io NAND [partition] [volume]\n"
	"config_raw_io MMC [volume]\n"	
	"If you don't input any parameters, uboot would use the default settings\n"
);

#if CONFIG_MINIUBOOT
#else
U_BOOT_CMD(
    raw_io_status, CONFIG_SYS_MAXARGS, 0, do_raw_io_status,
    "get raw_io status",
    " \n"
);

U_BOOT_CMD(
    push_raw_io_config, CONFIG_SYS_MAXARGS, 0, do_raw_io_config_push,
    "push raw_io current config",
    " \n"
);

U_BOOT_CMD(
    pop_raw_io_config, CONFIG_SYS_MAXARGS, 0, do_raw_io_config_pop,
    "pop raw_io last config",
    " \n"
);

U_BOOT_CMD(
    raw_read, CONFIG_SYS_MAXARGS, 0, do_raw_read,
    "Read the raw datas that store in the target device\n"
    "Yo have to execute 'config_raw_io' before using this cmd\n",
    "[dram addr] [offset] [len] \n"
	"unit:bytes\n"
);

U_BOOT_CMD(
    raw_write, CONFIG_SYS_MAXARGS, 0, do_raw_write,
    "Write the raw datas that store in the target device"
    "Yo have to execute 'config_raw_io' before using this cmd\n",    
    "[dram addr] [offset] [len] \n"
	"unit:bytes\n"
);

#endif

