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
#ifndef _DRVMIU_PRIV_H_
#define _DRVMIU_PRIV_H_

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
#include "utopia_dapi.h"
#include "drvMIU.h"

typedef MS_BOOL (*IOCTL_MIU_INIT)(void);
typedef void (*IOCTL_MIU_MASK_IP)(MS_U8 , MS_U8);
typedef void (*IOCTL_MIU_MASK_REQ)(MS_U8 , eMIUClientID);
typedef MS_BOOL (*IOCTL_MIU_SET_SSC)(MS_U16, MS_U16, MS_BOOL);
typedef MS_BOOL (*IOCTL_MIU_SET_SSCVALUE)(MS_U8, MS_U16, MS_U16, MS_BOOL);
typedef MS_BOOL (*IOCTL_MIU_PROTECT)(MS_U8, MS_U8*, MS_PHY, MS_PHY, MS_BOOL);
typedef MS_BOOL (*IOCTL_MIU_PROTECTEx)(MS_U8, MS_U32*, MS_PHY, MS_PHY, MS_BOOL);
typedef MS_BOOL (*IOCTL_MIU_SELMIU)(eMIUClientID, eMIU_SelType);
typedef MS_BOOL (*IOCTL_MIU_GETPROTECTINFO)(MS_U8, MIU_PortectInfo*);
typedef MS_BOOL (*IOCTL_MIU_SETGROUPPRIORITY)(MS_U8, MIU_GroupPriority);
typedef MS_BOOL (*IOCTL_MIU_SETHIGHPRIORITY)(MS_U8, eMIUClientID, MS_BOOL);
typedef void (*IOCTL_MIU_PRINTPROTECTINFO)(void);
typedef MS_BOOL (*IOCTL_MIU_DRAM_SIZE)(MS_U8, MS_U8);
typedef MS_BOOL (*IOCTL_MIU_PROTECTALIGN)(MS_U32 *);
typedef MS_BOOL (*IOCTL_MIU_GETDRAMTYPE)(MIU_ID eMiu, MIU_DDR_TYPE* pType);
typedef MS_BOOL (*IOCTL_MIU_ISSUPPORTMIU1)(void);
typedef MS_BOOL (*IOCTL_MIU_BWMONITOR)(MS_U8, MIU_BandwidthMonitorInfo *);
typedef MS_BOOL (*IOCTL_MIU_BWMONITORRESET)(MS_U8);
typedef MS_BOOL (*IOCTL_MIU_ConfigHWAttr)(MS_U8, eMIU_ATTR_TYPE, MS_U16*, MS_U16);

typedef struct _MIU_RESOURCE_PRIVATE
{
    MS_U32 Dummy;
}MIU_RESOURCE_PRIVATE;

typedef struct _MIU_INSTANT_PRIVATE
{
    IOCTL_MIU_INIT fpMIU_Init;
    IOCTL_MIU_MASK_IP fpMIU_Mask_Req_OPM_R;
    IOCTL_MIU_MASK_IP fpMIU_Mask_Req_DNRB_R;
    IOCTL_MIU_MASK_IP fpMIU_Mask_Req_DNRB_W;
    IOCTL_MIU_MASK_IP fpMIU_Mask_Req_DNRB_RW;
    IOCTL_MIU_MASK_IP fpMIU_Mask_Req_SC_RW;
    IOCTL_MIU_MASK_IP fpMIU_Mask_Req_MVOP_R;
    IOCTL_MIU_MASK_IP fpMIU_Mask_Req_MVD_R;
    IOCTL_MIU_MASK_IP fpMIU_Mask_Req_MVD_W;
    IOCTL_MIU_MASK_IP fpMIU_Mask_Req_MVD_RW;
    IOCTL_MIU_MASK_IP fpMIU_Mask_Req_AUDIO_RW;
    IOCTL_MIU_MASK_REQ fpMIU_Mask_Req;
    IOCTL_MIU_MASK_REQ fpMIU_UnMask_Req;
    IOCTL_MIU_SET_SSC fpMIU_SetSsc;
    IOCTL_MIU_SET_SSCVALUE fpMIU_SetSscValue;
    IOCTL_MIU_PROTECT fpMIU_Protect;
    IOCTL_MIU_PROTECTEx fpMIU_ProtectEx;
    IOCTL_MIU_SELMIU fpMIU_SelMIU;
    IOCTL_MIU_GETPROTECTINFO fpMIU_GetProtectInfo;
    IOCTL_MIU_SETGROUPPRIORITY fpMIU_SetGroupPriority;
    IOCTL_MIU_SETHIGHPRIORITY fpMIU_SetHighPriority;
    IOCTL_MIU_PRINTPROTECTINFO fpMIU_PrintProtectInfo;
    IOCTL_MIU_DRAM_SIZE fpMIU_Dram_Size;
    IOCTL_MIU_PROTECTALIGN fpMIU_ProtectAlign;
    IOCTL_MIU_GETDRAMTYPE fpMIU_GetDramType;
    IOCTL_MIU_ISSUPPORTMIU1 fpMIU_IsSupportMIU1;
    IOCTL_MIU_BWMONITOR fpMIU_SetBWMonitor;
    IOCTL_MIU_BWMONITOR fpMIU_DelBWMonitor;
    IOCTL_MIU_BWMONITOR fpMIU_QueryBWMonitor;
    IOCTL_MIU_BWMONITOR fpMIU_GetBWMonitor;
    IOCTL_MIU_BWMONITORRESET fpMIU_ResetBWMonitor;
    IOCTL_MIU_ConfigHWAttr fpMIU_ConfigHWAttr;
}MIU_INSTANT_PRIVATE;

void MIURegisterToUtopia(FUtopiaOpen ModuleType);
MS_U32 MIUOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute);
MS_U32 MIUClose(void* pInstance);
MS_U32 MIUIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs);
MS_BOOL _MDrv_MIU_Init(void);
void _MDrv_MIU_Mask_Req_OPM_R(MS_U8 u8Mask, MS_U8 u8Miu);
void _MDrv_MIU_Mask_Req_DNRB_R(MS_U8 u8Mask, MS_U8 u8Miu);
void _MDrv_MIU_Mask_Req_DNRB_W(MS_U8 u8Mask, MS_U8 u8Miu);
void _MDrv_MIU_Mask_Req_DNRB_RW(MS_U8 u8Mask, MS_U8 u8Miu);
void _MDrv_MIU_Mask_Req_SC_RW(MS_U8 u8Mask, MS_U8 u8Miu);
void _MDrv_MIU_Mask_Req_MVOP_R(MS_U8 u8Mask, MS_U8 u8Miu);
void _MDrv_MIU_Mask_Req_MVD_R(MS_U8 u8Mask, MS_U8 u8Miu);
void _MDrv_MIU_Mask_Req_MVD_W(MS_U8 u8Mask, MS_U8 u8Miu);
void _MDrv_MIU_Mask_Req_MVD_RW(MS_U8 u8Mask, MS_U8 u8Miu);
void _MDrv_MIU_Mask_Req_AUDIO_RW(MS_U8 u8Mask, MS_U8 u8Miu);
void _MDrv_MIU_MaskReq(MS_U8 u8Miu, eMIUClientID eClientID);
void _MDrv_MIU_UnMaskReq(MS_U8 u8Miu, eMIUClientID eClientID);
MS_BOOL _MDrv_MIU_SetSsc(MS_U16 u16Fmodulation, MS_U16 u16FDeviation, MS_BOOL bEnable);
MS_BOOL _MDrv_MIU_SetSscValue(MS_U8 u8MiuDev, MS_U16 u16Fmodulation, MS_U16 u16FDeviation, MS_BOOL bEnable);
MS_BOOL _MDrv_MIU_Protect(MS_U8 u8Blockx, MS_U8 *pu8ProtectId, MS_PHY u32Start, MS_PHY u32End, MS_BOOL  bSetFlag);
MS_BOOL _MDrv_MIU_ProtectEx(MS_U8 u8Blockx, MS_U32 *pu32ProtectId, MS_PHY u32Start, MS_PHY u32End, MS_BOOL  bSetFlag);
MS_BOOL _MDrv_MIU_SelMIU(eMIUClientID eClientID, eMIU_SelType eType);
MS_BOOL _MDrv_MIU_GetProtectInfo(MS_U8 u8MiuDev, MIU_PortectInfo *pInfo);
MS_BOOL _MDrv_MIU_SetGroupPriority(MS_U8 u8MiuDev, MIU_GroupPriority sPriority);
MS_BOOL _MDrv_MIU_SetHPriorityMask(MS_U8 u8MiuDev, eMIUClientID eClientID, MS_BOOL bMask);
void _MDrv_MIU_PrintProtectInfo(void);
MS_BOOL _MDrv_MIU_Dram_Size(MS_U8 MiuID, MS_U8 DramSize);
MS_BOOL _MDrv_MIU_ProtectAlign(MS_U32 *u32PageShift);
MS_BOOL _MDrv_MIU_GetDramType(MIU_ID eMiu, MIU_DDR_TYPE* pType);
MS_BOOL _MDrv_MIU_IsSupportMIU1(void);
MS_BOOL _MDrv_MIU_Set_BW_Monitor(MS_U8 u8MiuDev, MIU_BandwidthMonitorInfo *pBWInfo);
MS_BOOL _MDrv_MIU_Del_BW_Monitor(MS_U8 u8MiuDev, MIU_BandwidthMonitorInfo *pBWInfo);
MS_BOOL _MDrv_MIU_Query_BW_Monitor(MS_U8 u8MiuDev, MIU_BandwidthMonitorInfo *pBWInfo);
MS_BOOL _MDrv_MIU_Get_BW(MS_U8 u8MiuDev, MIU_BandwidthMonitorInfo *pBWInfo);
MS_BOOL _MDrv_MIU_Reset_All_BW_Monitor(MS_U8 u8MiuDev);
MS_BOOL _MDrv_MIU_ConfigHWAttr(MS_U8 u8MiuDev, eMIU_ATTR_TYPE eAttrType, MS_U16 *u16Value, MS_U16 u16Length);

#ifdef CONFIG_UTOPIA_PROC_DBG_SUPPORT
#define MIU_FLOWCONTROL_COUNT_NUM    (2)
#define MIU_ML_MAX_CLIENT    (16)
#define strMIUMdbCmd_NUM (10)
typedef enum
{
    MIU_GETPARABYCHIP_MIU_SELECT,
}EN_MIU_GETPARABYCHIP;

typedef enum
{
    MIU_MDBCMDLINE_NONE,
    MIU_MDBCMDLINE_HELP,
    MIU_MDBCMDLINE_MIU_PROTECT,
    MIU_MDBCMDLINE_MIU_SELECT,
    MIU_MDBCMDLINE_MIU_MASK,
    MIU_MDBCMDLINE_MIU_BANDWIDTH_LOADING,
    MIU_MDBCMDLINE_MIU_BANDWIDTH_BANKINFO,
    MIU_MDBCMDLINE_MIU_BANDWIDTH_MEMBERLIMIT,
    MIU_MDBCMDLINE_MIU_BANDWIDTH_FLOWCONRTOL,
    MIU_MDBCMDLINE_MIU_BANDWIDTH_HPMASK,
    MIU_MDBCMDLINE_MIU_DRAMSIZE,
    MIU_MDBCMDLINE_MIU_DRAMCLOCK,
    MIU_MDBCMDLINE_MIU_MSTV_MIU_PROTECT,
    MIU_MDBCMDLINE_MIU_BW_MONITOR_SET,
    MIU_MDBCMDLINE_MIU_BW_MONITOR_GET,
}EN_MIU_MDBCMDLINE;

typedef struct
{
    MIU_ID eMIU;
}GetParaByChipHandle;

typedef struct
{
    MS_U32 u32Period;
    MS_U32 u32clientID[MIU_FLOWCONTROL_COUNT_NUM];
}MIU_FlowControl_Count;

typedef struct
{
    MS_U32 u32Group;
    MIU_FlowControl_Count CountArray[MIU_FLOWCONTROL_COUNT_NUM];
}MIU_FlowControl;

typedef struct
{
    MS_U32 u32Group;
    MS_U32 u32MaxNum;
    MS_U32 u32MaskNum;
    MS_U32 u32MaskclientID[MIU_ML_MAX_CLIENT];
    MS_U32 u32clientID[MIU_ML_MAX_CLIENT];
}MIU_MemberLimit;

MS_BOOL MDrv_MIU_MdbCmdLine(MDBCMD_CMDLINE_PARAMETER *paraCmdLine, EN_MIU_MDBCMDLINE eMdbCmdLine);
#endif

#ifdef __cplusplus
}
#endif
#endif // _DRVMIU_PRIV_H_
