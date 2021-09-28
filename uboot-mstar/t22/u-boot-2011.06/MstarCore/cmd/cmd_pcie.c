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

/*
 * PCIE test Utilities
 */
#if (ENABLE_MODULE_PCIE== 1)
#include <common.h>
#include <command.h>
#include <exports.h>
#include "MsTypes.h"

#define readb(addr)			*((unsigned char volatile *)(addr))
#define writeb(val, addr)	(*((unsigned char volatile *)(addr)) = (unsigned char)val)
#define readw(addr) 		*((volatile u16 *)(addr))
#define writew(val, addr)	(*((volatile u16 *)(addr)) = (u16)val)
#define readl(addr) 		*((volatile u32 *)(addr))
#define writel(val, addr)	(*((volatile u32 *)(addr)) = (u32)val)

#define BIT0			0x01
#define BIT1			0x02
#define BIT2			0x04
#define BIT3			0x08
#define BIT4			0x10
#define BIT5			0x20
#define BIT6			0x40
#define BIT7			0x80

int do_pcietest(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	if (argc < 2)
		return cmd_usage(cmdtp);

	if (strncmp(argv[1], "read32", 6) == 0)
	{
		u32 	addr, addr_input, offset;

		addr_input = simple_strtol(argv[2], NULL, 16);
		if (addr_input == 0)
		{
			printf("Invalid addr...\n");
			return 1;
		}

		offset = addr_input & 0xFFFF;
		addr = ((addr_input & 0xFFFF0000) << 1) | 0x1F000000;    //Bank address
		addr |= offset;

		printf("0x%x: %x\n", addr_input, readl((void*) addr));
	}
	else if (strncmp(argv[1], "write32", 7) == 0)
	{
		u32 	addr, offset, uVal;

		addr = simple_strtol(argv[2], NULL, 16);
		if (addr == 0)
		{
			printf("Invalid addr...\n");
			return 1;
		}
		uVal = simple_strtol(argv[3], NULL, 16);
		if (uVal == 0)
		{
			printf("Invalid value...\n");
			return 1;
		}

		offset = addr & 0xFFFF;
		addr = ((addr & 0xFFFF0000) << 1) | 0x1F000000;    //Bank address
		addr |= offset;

		writel((u32) uVal, (void*) addr);
	}
	else if (strncmp(argv[1], "read", 4) == 0)
	{
		u32 	addr;
		u8	data_len = 32;

		addr = simple_strtol(argv[2], NULL, 16);
		if (addr == 0)
		{
			printf("Invalid addr...\n");
			return 1;
		}
		if (strncmp(argv[3], "8", 1) == 0)
			data_len = 8;
		else if (strncmp(argv[3], "16", 2) == 0)
			data_len = 16;

		if (data_len == 8)
		{
			printf("0x%x: %x\n", addr, readb((void*) addr));
		}
		else if (data_len == 16)
		{
			printf("0x%x: %x\n", addr, readw((void*) addr));
		}
		else if (data_len == 32)
		{
			printf("0x%x: %x\n", addr, readl((void*) addr));
		}
	}
	else if (strncmp(argv[1], "write", 5) == 0)
	{
		u32 	addr, uVal;
		u8	data_len = 32;

		addr = simple_strtol(argv[2], NULL, 16);
		if (addr == 0)
		{
			printf("Invalid addr...\n");
			return 1;
		}

		if (strncmp(argv[3], "8", 1) == 0)
			data_len = 8;
		else if (strncmp(argv[3], "16", 2) == 0)
			data_len = 16;

		uVal = simple_strtol(argv[4], NULL, 16);
		if (uVal == 0)
		{
			printf("Invalid value...\n");
			return 1;
		}

		if (data_len == 8)
			writeb((u8) uVal, (void*) addr);
		else if (data_len == 16)
			writew((u16) uVal, (void*) addr);
		else if (data_len == 32)
			writel((u32) uVal, (void*) addr);
	}

	return 1;
}


U_BOOT_CMD(
	pcietest,	5,	1,	do_pcietest,
	"PCIE test system",
	"read32 addr - read 32bits data from addr \n"
	"write32 addr value - write 32bits value to addr \n"
);
#endif

