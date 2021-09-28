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
#ifndef _DRVBDMA_PRIV_H_
#define _DRVBDMA_PRIV_H_

////////////////////////////////////////////////////////////////////////////////
/// @brief Byte DMA control driver
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C"
{
#endif

#include "MsTypes.h"
#include "drvBDMA.h"

typedef MS_U32 (*IOCTL_BDMA_MEMCPY)(MS_PHY, MS_PHY, MS_U32);
typedef MS_U32 (*IOCTL_BDMA_SEARCH)(MS_PHY, MS_U32, MS_U32, MS_U32, BDMA_SrcDev);
typedef MS_U32 (*IOCTL_BDMA_CRC32)(MS_PHY, MS_U32, MS_U32, MS_U32, BDMA_SrcDev, MS_BOOL);
typedef MS_U32 (*IOCTL_BDMA_PATTERN_FILL)(MS_PHY, MS_U32, MS_U32, BDMA_DstDev);
typedef MS_U32 (*IOCTL_BDMA_FLASHCOPY2DRAM)(MS_PHY, MS_PHY, MS_U32);
#ifdef MOBF_ENABLE
typedef MS_U32 (*IOCTL_BDMA_MOBFSEARCH)(MS_PHY, MS_U32, BDMA_MOBF_PS*, BDMA_SrcDev);
#endif
typedef void (*IOCTL_BDMA_GETSTATUS)(BDMA_Status*);
typedef BDMA_Info* (*IOCTL_BDMA_GETINFO)(void);
typedef BDMA_Result (*IOCTL_BDMA_GETLIBVER)(const MSIF_Version **);
typedef MS_U32 (*IOCTL_BDMA_GETMINSIZE)(void);
typedef BDMA_Result (*IOCTL_BDMA_SETDBGLEVEL)(BDMA_DbgLv);
typedef BDMA_Result (*IOCTL_BDMA_WAITFLASHDONE)(void);
typedef BDMA_Result (*IOCTL_BDMA_INIT)(MS_PHY);
typedef BDMA_Result (*IOCTL_BDMA_COPYHND)(MS_PHY, MS_PHY, MS_U32, BDMA_CpyType, MS_U8);
typedef MS_U32 (*IOCTL_BDMA_SETPOWERSTATE)(EN_POWER_MODE u16PowerState);

typedef struct _BDMA_RESOURCE_PRIVATE
{
    MS_U32 Dummy;
}BDMA_RESOURCE_PRIVATE;

typedef struct _BDMA_INSTANT_PRIVATE
{
    IOCTL_BDMA_MEMCPY           fpBDMAMemCpy;
    IOCTL_BDMA_SEARCH           fpBDMASearch;
    IOCTL_BDMA_CRC32            fpBDMACRC32;
    IOCTL_BDMA_PATTERN_FILL     fpBDMAPatternFill;
    IOCTL_BDMA_FLASHCOPY2DRAM   fpBDMAFlashCopy2Dram;
#ifdef MOBF_ENABLE
    IOCTL_BDMA_MOBFSEARCH       fpBDMAMOBFSearch;
#endif
    IOCTL_BDMA_GETSTATUS        fpBDMAGetStatus;
    IOCTL_BDMA_GETINFO              fpBDMAGetInfo;
    IOCTL_BDMA_GETLIBVER            fpBDMAGetLibVer;
    IOCTL_BDMA_GETMINSIZE           fpBDMAGetMinSize;
    IOCTL_BDMA_SETDBGLEVEL          fpBDMASetDbgLevel;
    IOCTL_BDMA_WAITFLASHDONE        fpBDMAWaitFlashDone;
    IOCTL_BDMA_INIT                 fpBDMAInit;
    IOCTL_BDMA_COPYHND              fpBDMACopyHnd;
    IOCTL_BDMA_SETPOWERSTATE        fpBDMASetPowerState;
}BDMA_INSTANT_PRIVATE;

void BDMARegisterToUtopia(FUtopiaOpen ModuleType);
MS_U32 BDMAOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute);
MS_U32 BDMAClose(void* pInstance);
MS_U32 BDMAIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs);
MS_U32 BDMAStr(MS_U32 u32PowerState, void* pModule);

BDMA_Result _MDrv_BDMA_MemCopy(MS_PHYADDR u32SrcAddr, MS_PHYADDR u32DstAddr, MS_U32 u32Len);
MS_U32 _MDrv_BDMA_Search(MS_PHYADDR u32Addr, MS_U32 u32Len, MS_U32 u32Pattern, MS_U32 u32ExcluBit, BDMA_SrcDev eDev);
MS_U32 _MDrv_BDMA_CRC32(MS_PHYADDR u32Addr, MS_U32 u32Len, MS_U32 u32Poly, MS_U32 u32Seed, BDMA_SrcDev eDev, MS_BOOL bReflect);
BDMA_Result _MDrv_BDMA_PatternFill(MS_PHYADDR u32Addr, MS_U32 u32Len, MS_U32 u32Pattern, BDMA_DstDev eDev);
BDMA_Result _MDrv_BDMA_FlashCopy2Dram(MS_PHYADDR u32FlashAddr, MS_PHYADDR u32DramAddr, MS_U32 u32Len);
#ifdef MOBF_ENABLE
MS_U32 _MDrv_BDMA_MOBFSearch(MS_PHYADDR u32Addr, MS_U32 u32Len, BDMA_MOBF_PS* pMobfPsCfg, BDMA_SrcDev eDev);
#endif
void _MDrv_BDMA_GetStatus(BDMA_Status *pStatus);
BDMA_Result _MDrv_BDMA_WaitFlashDone(void);
BDMA_Result _MDrv_BDMA_Init(MS_PHY phy64Miu1Base);
BDMA_Result _MDrv_BDMA_CopyHnd (MS_PHY phy64SrcAddr, MS_PHY phy64DstAddr, MS_U32 u32Len, BDMA_CpyType eCpyType, MS_U8 u8OpCfg);
MS_U32 _MDrv_BDMA_SetPowerState(EN_POWER_MODE u16PowerState);

#ifdef CONFIG_UTOPIA_PROC_DBG_SUPPORT
void MDrv_BDMA_Debug_echo(MS_PHY Src_addr, MS_PHY Dst_Addr, MS_PHY Src_Device, MS_PHY Dst_Device, MS_PHY Length, MS_U64 *handler);
void MDrv_BDMA_Debug_cat(MS_U64 *handler);
#endif

#ifdef __cplusplus
}
#endif
#endif // _DRVBDMA_PRIV_H_
