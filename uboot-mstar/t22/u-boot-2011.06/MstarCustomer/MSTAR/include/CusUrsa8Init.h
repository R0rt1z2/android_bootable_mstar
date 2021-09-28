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

#ifndef __CUS_ENVIRONMENT_H__
#define __CUS_ENVIRONMENT_H__
#include <MsTypes.h>

#define UINSIDE "uinside"

#ifdef CONFIG_ANDROID_BOOT
#define CONFIG   "tvservice"
#else
#define CONFIG   ""
#endif

#define CONFIG_PATH   "/config"
#define CSOT_BIN_PATH "ukulele/MERGE_VB1.bin"
#define CMI_BIN_PATH "ukulele/MERGE_4CH_LVDS.bin"
#define FULLHD_BIN_PATH "ukulele/MERGE_LVDS.bin"
#define FULLHD_PANEL_NAME "FullHD_CMO216_H1L01"
#define CSOT_PANEL_NAME "FullHD_CMO216_H1L01_NIKEU_CSOT"
#define CMI_PANEL_NAME "FullHD_CMO216_H1L01_NIKEU_CMI"
#define Dram_Addr (CONFIG_UBOOT_LOADADDR-CONFIG_MIU0_BUSADDR)  //0x300000
#define HEADER_LEN 0xA080
#define UKULELE_AP_START_ADDR 0x10000 //AP addr in ursa miu
#define UKULELE_BIN_START_ADDR 0x100000 //bin file addr in ursa miu
#define UKULELE_BIN_FILE_ADDR_IN_NIKE_MIU (Dram_Addr+CONFIG_MIU0_BUSADDR+0xA00C) //0x2030A00C //bin file aaddr in nike miu, file header A00C~A00E is bin file addr,0x20000000 is nike VA,0x300000 is Dram_Addr, 0x20000000+0x300000+A00C=0x2030A00C
#define UKULELE_BIN_GROUP_ADDR_IN_NIKE_MIU (Dram_Addr+CONFIG_MIU0_BUSADDR+0xA018)//18~1B: BIN_GROUP_MEM_ADDR

#define RIU_RT1_CMD    0x11
#define RIU_W1_CMD     0x1D
#define RIU_R1T_CMD      0x1C
#define MIU_W_CMD      0x25
#define MIU_R_CMD   0x20
#define MIU_ST_CMD       0x21
#define SPI_DEVICE_BUFFER_SIZE           256

#define BIT0  0x000001
#define BIT1  0x000002
#define BIT2  0x000004
#define BIT3  0x000008
#define BIT4  0x000010
#define BIT5  0x000020
#define BIT6  0x000040
#define BIT7  0x000080
#define BIT8  0x000100
#define BIT9  0x000200
#define BIT10 0x000400
#define BIT11 0x000800
#define BIT12 0x001000
#define BIT13 0x002000
#define BIT14 0x004000
#define BIT15 0x008000
#define BIT16 0x010000


MS_BOOL MDrv_SS_RIU_Write8(MS_U16 u16Addr, MS_U8 data);
MS_BOOL MDrv_SS_RIU_Read8(MS_U16 u16Addr, MS_U8 *pdata);
MS_BOOL MDrv_SS_RIU_Write16(MS_U16 u16Addr, MS_U16 data);
MS_BOOL MDrv_SS_MIU_Reads(MS_U32 u32Addr, MS_U8 *pdata, MS_U16 u16Size);
MS_BOOL MDrv_SS_MIU_Writes(MS_U32 u32Addr, MS_U8 *pdata, MS_U32 u16Size);
MS_BOOL MDrv_SS_RIU_Write8_Mask(MS_U16 u16Addr, MS_U8 data, MS_U8 mask);

void InitialSystemPowerUp( void );
void MDrv_Macaw12_MiuInitial(void);
void MDrv_MIU_SimpleBist(void);



int do_cusUrsa8Init( cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);



#endif

