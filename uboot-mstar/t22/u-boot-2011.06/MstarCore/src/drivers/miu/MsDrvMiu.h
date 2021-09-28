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

#ifndef __DRVMIU__
#define __DRVMIU__

#include <common.h>
#include <malloc.h>
#include <asm/io.h>
#include <configs/uboot_board_config.h>
#include <ShareType.h>
#include <drvMIU.h>
#include <MsStr.h>
typedef enum
{
    E_MIU_ID_0=0x00,
    E_MIU_ID_1,
    E_MIU_ID_2,
    E_MIU_ID_MAX
}EN_MIUID;

#define	MIU_MAX_GP_NUM		        (16)         //Max MIU Group number
#define MIU_MAX_GP_PRIORITY_NUM     (16)         //Max MIU Group Priority number

typedef struct
{
    MIU_ID miu_id;
    MS_U8 u8Group;
    char *c_Mmap_Id;
    MS_PHYADDR u32Start;
    MS_PHYADDR u32End;
    MS_U8 clientID[16];
} stMIU_PortectInfo;


U8 MDrv_MIU_SelectMIU_UBoot(U8 bMIU1, U16 *au16SelMiu, U8 u8GpNum);
U8 MDrv_MIU_SetGroupPriority_UBoot(U8 bMIU1, U8 *au8GpPriority, U8 u8GpNum);
MS_BOOL MsDrv_MIU_Protect(MS_U8  u8Blockx, MS_U8  *pu8ProtectId, MS_U64 u64Start, MS_U64 u64End, MS_BOOL bSetFlag);
MS_BOOL MsDrv_MIU_ProtectEx(MS_U8  u8Blockx, MS_U32  *pu32ProtectId, MS_U64 u64Start, MS_U64 u64End, MS_BOOL bSetFlag);
void MsDrv_kernelProtect(MS_U8  u8Blockx,MS_U64 u64Lx1Start, MS_U64 u64Lx1End);
void MsDrv_PMProtect(MS_U8  u8Blockx,MS_U64 u64Lx1Start, MS_U64 u64Lx1End);
void MsDrv_NuttxProtect(MS_U8  u8Blockx,MS_U64 u64NuttxStart, MS_U64 u64NuttxEnd);
void MsDrv_MiuInit(void);
MIU_DDR_SIZE dram_size_to_enum(MS_U64 u64DramSize);
char* MsDrv_GetMIUSize(MS_U8 MiuID);
MS_U64 BA2PA(MS_U64 u64usAddr);
MS_U64 PA2BA(MS_U64 u64PhyAddr);
void MSDrv_FSTR_MiuBackup(ST_MIU_INFO * stMiuInfo);
void MSDrv_MIU_ProtectDummyDRAM(void);
//-------------------------------------------------------------------------------------------------
/// get MIU_ID by input uPhyAddrs
/// @INPUT:  uPhyAddrs                 \b OUT: MIU_ID
//-------------------------------------------------------------------------------------------------
EN_MIUID MsApi_Miu_GetID(MS_U64 uPhyAddrs);


#endif	//__DRVMIU__

