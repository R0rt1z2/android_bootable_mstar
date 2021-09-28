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

#ifndef __DRV_VBI_V2_H__
#define __DRV_VBI_V2_H__

#include "drvVBI.h"
#include "drvDMX_TTX.h"
#include "UFO.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum
{
    VBI_POOL_ID_VBI0 = 0,
    VBI_POOL_ID_VBI1 = 1,
} eVBIPoolID;

typedef enum {
    // VBI Teletext
    MDrv_CMD_VBI_RingBuffer_Reset = 0,
    MDrv_CMD_VBI_RegisterCB,
    MDrv_CMD_VBI_InitializeTTXSlicer,
    MDrv_CMD_VBI_EnableTTXSlicer,
    MDrv_CMD_VBI_IsVPS_Ready,
    MDrv_CMD_VBI_IsTTX_Ready,
    MDrv_CMD_VBI_IsWSS_Ready,
    MDrv_CMD_VBI_GetWSS_Data,
    MDrv_CMD_VBI_GetVPS_Data,
    MDrv_CMD_VBI_SetVideoStandard,
    MDrv_CMD_VBI_TTX_PacketBufferIsEmpty,
    MDrv_CMD_VBI_TTX_CheckCircuitReady,
    MDrv_CMD_VBI_TTX_GetPacketCount,
    MDrv_CMD_VBI_TTX_GetPackets,
    MDrv_CMD_VBI_TTX_GetPacket,
    MDrv_CMD_VBI_TTX_PacketBufferIsOverflow,
    MDrv_CMD_VBI_TTX_PacketBufferGetNoOfOverflows,
    MDrv_CMD_VBI_TTX_EnableLine,
    MDrv_CMD_VBI_WSS_SetVpsByteNum,
    MDrv_CMD_VBI_GetCompleteVPS_Data,
    MDrv_CMD_VBI_GetRawVPS_Data,
    MDrv_CMD_VBI_TTX_GetInfo,
    
    // VBI CC
    MDrv_CMD_VBI_CC_InitSlicer = 100,
    MDrv_CMD_VBI_CC_InitYPbYr,
    MDrv_CMD_VBI_CC_SetDataRate,
    MDrv_CMD_VBI_CC_GetInfo,
    MDrv_CMD_VBI_CC_SetFrameCnt,
    MDrv_CMD_VBI_CC_EnableSlicer,
    MDrv_CMD_VBI_CC_EnableLine,
    MDrv_CMD_VBI_CC_SetSCWindowLen,
    MDrv_CMD_VBI_CC_SetVideoStandard,
    
    // DMX Teletext
    MDrv_CMD_DMX_TTX_RingBuffer_Reset = 200,
    MDrv_CMD_DMX_TTX_Init,
    MDrv_CMD_DMX_TTX_SetFilterID,
    MDrv_CMD_DMX_TTX_SetCB,
    MDrv_CMD_DMX_TTX_PacketBufferIsEmpty,
    MDrv_CMD_DMX_TTX_GetPackets,
    MDrv_CMD_DMX_TTX_GetPacket,
    MDrv_CMD_DMX_TTX_PacketBufferIsOverflow,
    MDrv_CMD_DMX_TTX_PacketBufferGetNoOfOverflows,
    MDrv_CMD_DMX_TTX_Exit,
    
    // MISC
    MDrv_CMD_VBI_ObtainEng = 300,
    MDrv_CMD_VBI_ReleaseEng,
    MDrv_CMD_VBI_QueryCap,
    MDrv_CMD_VBI_GetLibVer,
    MDrv_CMD_VBI_GetInfo,
    MDrv_CMD_VBI_GetStatus,
    MDrv_CMD_VBI_SetDbgLevel,
    MDrv_CMD_VBI_Init,
    MDrv_CMD_VBI_SyncMemory,
    MDrv_CMD_VBI_Exit,    
    MDrv_CMD_VBI_ProtectMemory,

    MDrv_CMD_VBI_Suspend,
    MDrv_CMD_VBI_Resume,
    MDrv_CMD_VBI_SetPowerState,

    MDrv_CMD_DMX_TTX_Suspend,
    MDrv_CMD_DMX_TTX_Resume,
    MDrv_CMD_DMX_TTX_SetPowerState,
} eVBIIoctlOpt;

typedef struct DLL_PACKED _VBI_BOOL
{
    MS_BOOL bCheck;
} VBI_BOOL, *PVBI_BOOL;

typedef struct DLL_PACKED _VBI_GET_DATA
{
    MS_U16 u16data;
} VBI_GET_DATA, *PVBI_GET_DATA;

typedef struct _VBI_GET_RESULT
{
    EN_POWER_MODE u16PowerState;
    MS_U32 u32result;
} VBI_GET_RESULT, *PVBI_GET_RESULT;

//-------------------------------------------------------------

typedef struct _VBI_GETLIBVER
{
    const MSIF_Version **ppVersion;
} VBI_GETLIBVER_PARAM, *PVBI_GETLIBVER;

typedef struct DLL_PACKED _VBI_GETINFO
{
    MS_U8 u8NoInfo;
} VBI_GETINFO_PARAM, *PVBI_GETINFO_PARAM;

typedef struct DLL_PACKED _GETSTATUS
{
    VBI_DrvStatus *pDrvStatus;
} VBI_GETSTATUS_PARAM, *PVBI_GETSTATUS;

typedef struct DLL_PACKED _VBI_SETDBGLEVEL
{
    MS_U16 u16DbgSwitch;
} VBI_SETDBGLEVEL_PARAM, *PVBI_SETDBGLEVEL;

typedef struct DLL_PACKED _VBI_CMD
{
    EN_VBI_CMD cmd;
} VBI_CMD, *PVBI_CMD;

typedef struct DLL_PACKED _VBI_INIT_TYPE
{
    EN_VBI_CMD cmd;
    VBI_INIT_TYPE type;
} VBI_INIT_TYPE_PARAM, *PVBI_INIT_TYPE;

typedef struct DLL_PACKED _DMX_TTX_CMD
{
    EN_VBI_CMD cmd;
} DMX_TTX_CMD, *PDMX_TTX_CMD;

typedef struct DLL_PACKED _VBI_REGISTER_CB
{
    VBI_CB_FN pFN;
#if defined(UFO_PUBLIC_HEADER_300)    
    MS_PHYADDR bufferAddr;
#else
    MS_PHY     bufferAddr;
#endif    
    MS_U32 length;
} VBI_REGISTER_CB, *PVBI_REGISTER_CB;

typedef struct DLL_PACKED _VBI_INITIALIZER_TTX_SLICER
{
    EN_VBI_CMD cmd;
#if defined(UFO_PUBLIC_HEADER_300)    
    MS_PHYADDR bufferAddr;
#else
    MS_PHY     bufferAddr;
#endif    
    MS_U16 packetCount;
} VBI_INITIALIZER_TTX_SLICER, *PVBI_INITIALIZER_TTX_SLICER;

typedef struct DLL_PACKED _VBI_ENABLE_TTX_SLICER
{
    MS_BOOL bEnable;
} VBI_ENABLE_TTX_SLICER, *PVBI_ENABLE_TTX_SLICER, VBI_CC_ENABLE_SLICER, *PVBI_CC_ENABLE_SLICER;

typedef struct DLL_PACKED _VBI_GET_VPS_DATA
{
    MS_U8 *lowerWord;
    MS_U8 *higherWord;
} VBI_GET_VPS_DATA, *PVBI_GET_VPS_DATA;

typedef struct DLL_PACKED _VBI_GET_COMPLETE_VPS_DATA
{
    MS_U8  **pBuffer;
    MS_U32 *length;
} VBI_GET_COMPLETE_VPS_DATA, *PVBI_GET_COMPLETE_VPS_DATA;

typedef struct DLL_PACKED _VBI_GET_RAW_VPS_DATA
{
    MS_U8 byte0;
    MS_U8 byte1;
    MS_U8 byte2;
    MS_U8 byte3;
} VBI_GET_RAW_VPS_DATA, *PVBI_GET_RAW_VPS_DATA;

typedef struct DLL_PACKED _VBI_SET_VIDEO_STANDARD
{
    VBI_VIDEO_STANDARD eStandard;
    MS_BOOL bRet;
} VBI_SET_VIDEO_STANDARD, *PVBI_SET_VIDEO_STANDARD;


typedef struct DLL_PACKED _VBI_TTX_GET_PACKETS
{
#if defined(UFO_PUBLIC_HEADER_300)    
    MS_PHYADDR dataAddr;
#else
    MS_PHY     dataAddr;
#endif    
    MS_U32 *length;
} VBI_TTX_GET_PACKETS, *PVBI_TTX_GET_PACKETS;

typedef struct DLL_PACKED _VBI_TTX_GET_PACKET
{
#if defined(UFO_PUBLIC_HEADER_300)    
    MS_PHYADDR *packetAddress;
#else
    MS_PHY     *packetAddress;
#endif    
} VBI_TTX_GET_PACKET, *PVBI_TTX_GET_PACKET;

typedef struct DLL_PACKED _VBI_TTX_ENABLE_LINE
{
    MS_U16 StartLine;
    MS_U16 EndLine;
} VBI_TTX_ENABLE_LINE, *PVBI_TTX_ENABLE_LINE;

typedef struct DLL_PACKED _VBI_SYNC_MEMORY
{
    MS_U32 u32Start;
    MS_U32 u32Size;
} VBI_SYNC_MEMORY, *PVBI_SYNC_MEMORY;

typedef struct DLL_PACKED _VBI_CC_INIT_SLICER
{
#if defined(UFO_PUBLIC_HEADER_300)    
    MS_U32     u32RiuAddr;
    MS_PHYADDR bufferAddr;
#else
    MS_VIRT    u32RiuAddr;
    MS_PHY     bufferAddr;
#endif    
    MS_U16 packetCount;
} VBI_CC_INIT_SLICER, *PVBI_CC_INIT_SLICER;

typedef struct DLL_PACKED _VBI_CC_INIT_YPBYR
{
    MS_U8 cvbs_no;
} VBI_CC_INIT_YPBYR, *PVBI_CC_INIT_YPBYR;

typedef struct DLL_PACKED _VBI_CC_SETDATARATE
{
    MS_U8 *ptable;
} VBI_CC_SETDATARATE, *PVBI_CC_SETDATARATE;

typedef struct DLL_PACKED _VBI_CC_GETINFO
{
    MS_U32 selector;
    MS_U32 info;
} VBI_CC_GETINFO, *PVBI_CC_GETINFO;

typedef struct DLL_PACKED _VBI_CC_SET_FRAMECNT
{
    MS_U8 cnt;
} VBI_CC_SET_FRAMECNT, *PVBI_CC_SET_FRAMECNT, VBI_VBI_WSS_VPSBYTENUM, *PVBI_VBI_WSS_VPSBYTENUM;

typedef struct DLL_PACKED _VBI_CC_ENABLE_LINE
{
    MS_U16 StartLine;
    MS_U16 EndLine;
    MS_U8 mode;
} VBI_CC_ENABLE_LINE, *PVBI_CC_ENABLE_LINE;

typedef struct DLL_PACKED _VBI_CC_SET_SC_WND_LEN
{
    MS_U8 u8Len;
    MS_BOOL bRet;
} VBI_CC_SET_SC_WND_LEN, *PVBI_CC_SET_SC_WND_LEN;

typedef struct DLL_PACKED _VBI_PROTECT_MEMORY
{
    MS_BOOL bEnable;
    MS_PHY phyAddr;
    MS_U32 u32Size;
    MS_BOOL bRet;
} VBI_PROTECT_MEMORY, *PVBI_PROTECT_MEMORY;

typedef struct DLL_PACKED _DMX_TTX_SET_FILTERID
{
    MS_U8 fid;
} DMX_TTX_SET_FILTERID, *PDMX_TTX_SET_FILTERID;

typedef struct DLL_PACKED _DMX_TTX_SET_CB
{
    DMX_CB_FN fn;
} DMX_TTX_SET_CB, *PDMX_TTX_SET_CB;

typedef struct DLL_PACKED _VBI_TTX_GETINFO
{
    MS_U32 u32Info;
    MS_U32 u32Ret;
} VBI_TTX_GETINFO, *PVBI_TTX_GETINFO;

#ifdef __cplusplus
}
#endif

#endif //__DRV_VBI_V2_H__

