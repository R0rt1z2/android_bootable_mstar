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

//////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @brief  MIU Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_MIU_V2_H_
#define _DRV_MIU_V2_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "MsTypes.h"
#include "UFO.h"

//-------------------------------------------------------------------------------------------------
// Defines
//-------------------------------------------------------------------------------------------------
typedef enum _eMIUIoctlCmd
{
    MDrv_CMD_MIU_Init,
    MDrv_CMD_MIU_Mask_Req_OPM_R,
    MDrv_CMD_MIU_Mask_Req_DNRB_R,
    MDrv_CMD_MIU_Mask_Req_DNRB_W,
    MDrv_CMD_MIU_Mask_Req_DNRB_RW,
    MDrv_CMD_MIU_Mask_Req_SC_RW,
    MDrv_CMD_MIU_Mask_Req_MVOP_R,
    MDrv_CMD_MIU_Mask_Req_MVD_R,
    MDrv_CMD_MIU_Mask_Req_MVD_W,
    MDrv_CMD_MIU_Mask_Req_MVD_RW,
    MDrv_CMD_MIU_Mask_Req_AUDIO_RW,
    MDrv_CMD_MIU_Mask_Req,
    MDrv_CMD_MIU_UnMask_Req,
    MDrv_CMD_MIU_GetLoading,
    MDrv_CMD_MIU_SetSsc,
    MDrv_CMD_MIU_SetSscValue,
    MDrv_CMD_MIU_Protect,
    MDrv_CMD_MIU_ProtectEx,
    MDrv_CMD_MIU_SelMIU,
    MDrv_CMD_MIU_GetProtectInfo,
    MDrv_CMD_MIU_SetGroupPriority,
    MDrv_CMD_MIU_SetHighPriority,
    MDrv_CMD_MIU_PrintProtectInfo,
    MDrv_CMD_MIU_Dram_Size,
    MDrv_CMD_MIU_ProtectAlign,
    MDrv_CMD_MIU_GetDramType,
    MDrv_CMD_MIU_IsSupportMIU1,
    MDrv_CMD_MIU_SetBWMonitor,
    MDrv_CMD_MIU_DelBWMonitor,
    MDrv_CMD_MIU_QueryBWMonitor,
    MDrv_CMD_MIU_GetBW,
    MDrv_CMD_MIU_ResetAllBWMonitor,
    MDrv_CMD_MIU_ConfigHWAttr,
}eMIUIoctlCmd;

//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
// Type and Structure Declaration
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Extern Global Variabls
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Extern Functions
//-------------------------------------------------------------------------------------------------

//////////////////////////////////////////////////////////////
// below are for utopia20 cmd and parameters structure
// for below cmds
// MDrv_CMD_MIU_Mask_Req_OPM_R,
// MDrv_CMD_MIU_Mask_Req_DNRB_R,
// MDrv_CMD_MIU_Mask_Req_DNRB_W,
// MDrv_CMD_MIU_Mask_Req_DNRB_RW,
// MDrv_CMD_MIU_Mask_Req_SC_RW,
// MDrv_CMD_MIU_Mask_Req_MVOP_R,
// MDrv_CMD_MIU_Mask_Req_MVD_R,
// MDrv_CMD_MIU_Mask_Req_MVD_W,
// MDrv_CMD_MIU_Mask_Req_MVD_RW,
// MDrv_CMD_MIU_Mask_Req_AUDIO_RW,

typedef struct _MIU_RET
{
    MS_BOOL bRet;
}MIU_RET, *PMIU_RET;

typedef struct _MIU_MASK
{
    MS_U8 u8Mask;
    MS_U8 u8Miu;
}MIU_MIU_MASK, *PMIU_MIU_MASK;

// for below cmds MDrv_CMD_MIU_Mask_Req, MDrv_CMD_MIU_UnMask_Req
typedef struct _MIU_MIUREQ_MASK
{
    MS_U8 u8Miu;
    eMIUClientID eClientID;
}MIU_MIUREQ_MASK, *PMIU_MIUREQ_MASK;

// MDrv_MIU_GetLoading,
typedef struct _MIU_GET_LOADING
{
    MS_U8 u8MiuDev;
    MS_U32 *u32Loading;
    MS_BOOL bRet;
}MIU_GET_LOADING, *PMIU_MIU_GET_LOADING;

// for below cmds MDrv_CMD_MIU_SetSsc
typedef struct _MIU_SET_SSC
{
    MS_U16 u16Fmodulation;
    MS_U16 u16FDeviation;
    MS_BOOL bEnable;
    MS_BOOL bRet;
}MIU_SET_SSC, *PMIU_SET_SSC;

// for below cmds MDrv_CMD_MIU_SetSscValue
typedef struct _MIU_SET_SSCVALUE
{
    MS_U8 u8MiuDev;
    MS_U16 u16Fmodulation;
    MS_U16 u16FDeviation;
    MS_BOOL bEnable;
    MS_BOOL bRet;
}MIU_SET_SSCVALUE, *PMIU_SET_SSCVALUE;

// for below cmds MDrv_CMD_MIU_Protect
    
typedef MS_U8 MIU_PROYECT_ID[16];

#ifdef BIFROST_32BIT_MODE
typedef struct DLL_PACKED _MIU_PROTECT
{
    MS_U8 u8Blockx;
    MS_U8 *pu8ProtectId;
#if defined(UFO_PUBLIC_HEADER_212)||defined(UFO_PUBLIC_HEADER_300)
    MS_U32 u32Start;
    MS_U32 u32End;
#else
    MS_U32 phy64Start;
    MS_U32 phy64End;
#endif
    MS_BOOL bSetFlag;
    MS_BOOL bRet;
}MIU_PROTECT, *PMIU_PROTECT;
#else
typedef struct DLL_PACKED _MIU_PROTECT
{
    MS_U8 u8Blockx;
    MS_U8 *pu8ProtectId;
#if defined(UFO_PUBLIC_HEADER_212)||defined(UFO_PUBLIC_HEADER_300)
    MS_U32 u32Start;
    MS_U32 u32End;
#else
    MS_PHY phy64Start;
    MS_PHY phy64End;
#endif
    MS_BOOL bSetFlag;
    MS_BOOL bRet;
}MIU_PROTECT, *PMIU_PROTECT;
#endif

// for below cmds MDrv_CMD_MIU_ProtectEx
typedef struct _MIU_PROTECTEx
{
    MS_U8 u8Blockx;
    MS_U32 *pu32ProtectId;
    MS_PHY phy64Start;
    MS_PHY phy64End;
    MS_BOOL bSetFlag;
}MIU_PROTECTEx, *PMIU_PROTECTEx;

// For HW Setting
typedef struct _MIU_ConfigHWAttr
{
    MS_U8 u8MiuDev;
    eMIU_ATTR_TYPE eAttrType;
    MS_U16 *u16Value;
    MS_U16 u16Length;
    MS_BOOL bRet;
}MIU_ConfigHWAttr, *PMIU_ConfigHWAttr;

// for below cmds MDrv_CMD_MIU_SelMIU
typedef struct _MIU_SELMIU
{
    eMIUClientID eClientID;
    eMIU_SelType eType;
    MS_BOOL bRet;
}MIU_SELMIU, *PMIU_SELMIU;

// for below cmds MDrv_CMD_MIU_GetDramType
typedef struct _MIU_GETDRAMTYPE
{
    MIU_ID eMiu;
    MIU_DDR_TYPE* pType;
}MIU_GETDRAMTYPE, *PMIU_GETDRAMTYPE;

// for below cmds MDrv_CMD_MIU_GetProtectInfo
typedef struct DLL_PACKED _MIU_GETPROTECTINFO
{
    MS_U8 u8MiuDev;
    MIU_PortectInfo *pInfo;
    MS_BOOL bRet;
}MIU_GETPROTECTINFO, *PMIU_GETPROTECTINFO;

// for below cmds MDrv_CMD_MIU_SetGroupPriority
typedef struct _MIU_SETGROUPPRIORITY
{
    MS_U8 u8MiuDev;
    MIU_GroupPriority sPriority;
    MS_BOOL bRet;
}MIU_SETGROUPPRIORITY, *PMIU_SETGROUPPRIORITY;


// for below cmds MDrv_CMD_MIU_SetHighPriority
typedef struct _MIU_SETHIGHPRIORITY
{
    MS_U8 u8MiuDev;
    eMIUClientID eClientID;
    MS_BOOL bMask;
    MS_BOOL bRet;
}MIU_SETHIGHPRIORITY, *PMIU_SETHIGHPRIORITY;

// for below cmds MDrv_CMD_MIU_Dram_Size
typedef struct _MIU_DRAM_SIZE
{
    MS_U8 u8MiuDev;
    MS_U8 DramSize;
    MS_BOOL bRet;
}MIU_DRAM_SIZE, *PMIU_DRAM_SIZE;

// for below cmds MDrv_CMD_MIU_ProtectAlign
typedef struct DLL_PACKED _MIU_PROTECT_ALIGN
{
    MS_U32 *u32PageShift;
    MS_BOOL bRet;
}MIU_PROTECT_ALIGN, *PMIU_PROTECT_ALIGN;

// for below cmds MDrv_CMD_MIU_SetBWMonitor
typedef struct DLL_PACKED _MIU_BWMONITOR
{
    MS_U8 u8MiuDev;
    MIU_BandwidthMonitorInfo *pBWInfo;
    MS_BOOL bRet;
}MIU_BWMONITOR, *PMIU_BWMONITOR;

#ifdef __cplusplus
}
#endif

#endif // _DRV_MIU_V2_H_

