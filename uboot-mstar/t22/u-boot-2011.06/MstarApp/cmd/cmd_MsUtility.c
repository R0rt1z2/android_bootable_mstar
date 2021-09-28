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
#include <MsUtility.h>

#if(ENABLE_MODULE_COMPRESS==1)
U_BOOT_CMD(
	mscompress7,	 6, 0, do_mscompress7,
	"Compress or decompress lzma files",
	"\n"
	"Usage:  lzma <e|d> <index> Src_Address Src_Length Dst_Address\n"
	"      e: encode file\n"
	"      d: decode file\n"
	"      <index> start to (de)compress position\n"
);
#endif

U_BOOT_CMD(
	delay,	 6, 0, do_mdelay,
	"delay time, time unit is ms\n",
	"Usage: delay [time]."
);

#if ((ENABLE_MODULE_USB == 1)&&(ENABLE_MODULE_FAT==1))
#if (ENABLE_MODULE_FAT_WRITE==1)
U_BOOT_CMD(
    spi2usb, 4, 0, do_spi2usb,
    "Read data from spi to usb",
    "[spi offset] [length] [output file name]\n"
);
#endif

#if CONFIG_MINIUBOOT
#else
U_BOOT_CMD(
    usb2spi, 4, 0, do_usb2spi,
    "write data from usb to spi",
    "[spi offset] [length] [intput file name]\n"
);
#endif

#endif

#if ((ENABLE_MODULE_USB == 1)&&(ENABLE_MODULE_EEPROM==1))
U_BOOT_CMD(eeprom2usb, 4, 0, do_eeprom2usb,
    "dump data from eeprom to usb",
    "<eeprom offset> <len> <output file name>\n"
    );


U_BOOT_CMD(usb2eeprom, 4, 0, do_usb2eeprom,
    "write data from usb to eeprom",
    "<eeprom offset> <len> <intput file name>\n"
    );

#endif

#if defined(CONFIG_MSTAR_TOOL_ROM_PROGRAM_NAND_BIN) && defined (CONFIG_MSTAR_TOOL_PROGRAM)
U_BOOT_CMD(nandprogramforrom, 1, 0, do_nandprogramforrom,
    "program nand for rom boot",
    "\n"
    );
#endif

#if defined(CONFIG_MSTAR_TOOL_ROM_PROGRAM_NAND_BIN) && defined (CONFIG_MSTAR_TOOL_CMDLINE)
U_BOOT_CMD(skipnandprogramforrom, 1, 0, do_skipnandprogramforrom,
    "enter cmd line when use tv tool for nand program",
    "\n"
    );
#endif
#if (ENABLE_MINIUBOOT == 0)
U_BOOT_CMD(showversion, 1, 0, do_showVersion,
    "Show version",
    "\n"
    );
#endif

#if ((CONFIG_TV_CHIP == 1) || (ENABLE_RESERVED_CHUNK_HEADER == 1))
#if (ENABLE_MINIUBOOT == 0)
U_BOOT_CMD(showboard, 1, 0, do_showBoard,
    "Show board information",
    "\n"
    );
#endif
#endif

#if defined(CONFIG_MSTAR_TOOL_CHECK_TEE)
U_BOOT_CMD(checktee, 2, 0, do_checkTEE,
    "Check TEE binary",
    "Usage:  checktee <BINAddr>\n"
    "\n"
    );
#endif
