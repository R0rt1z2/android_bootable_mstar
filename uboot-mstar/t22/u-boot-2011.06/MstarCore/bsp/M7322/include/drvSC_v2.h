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
/// @brief  SC Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_SC_V2_H_
#define _DRV_SC_V2_H_

#include "drvSC.h"

#ifdef __cplusplus
extern "C"
{
#endif

//-------------------------------------------------------------------------------------------------
// Defines
//-------------------------------------------------------------------------------------------------
// below for utopia20 structure

typedef enum
{
    E_MDRV_CMD_SC_Init,
    E_MDRV_CMD_SC_Exit,
    E_MDRV_CMD_SC_Open,
    E_MDRV_CMD_SC_Activate,
    E_MDRV_CMD_SC_Deactivate,
    E_MDRV_CMD_SC_Close,
    E_MDRV_CMD_SC_Reset,
    E_MDRV_CMD_SC_ClearState,
    E_MDRV_CMD_SC_GetATR,
    E_MDRV_CMD_SC_Conig,
    E_MDRV_CMD_SC_Send,
    E_MDRV_CMD_SC_Recv,
    E_MDRV_CMD_SC_GetCaps,
    E_MDRV_CMD_SC_SetPPS,
    E_MDRV_CMD_SC_PPS,
    E_MDRV_CMD_SC_GETSTATUS,
    E_MDRV_CMD_SC_SETDBGLEVEL,
    E_MDRV_CMD_SC_RAWEXCHANGE,
    E_MDRV_CMD_SC_CARDVOLTAGE_CONFIG,
    E_MDRV_CMD_SC_KDRVKERNELMODEPOLL,
    E_MDRV_CMD_SC_RESET_ATR,
    E_MDRV_CMD_SC_T0_SENDRECV,
    E_MDRV_CMD_SC_T1_SENDRECV,
    E_MDRV_CMD_SC_T14_SENDRECV,
    E_MDRV_CMD_SC_GETINFO,
    E_MDRV_CMD_SC_POWEROFF,
    E_MDRV_CMD_SC_SETGUARDTIME,
    E_MDRV_CMD_SC_SETPOWERSTATE
} eScIoctlOpt;

//-------------------------------------------------------------------------------------------------
// Type and Structure Declaration
//-------------------------------------------------------------------------------------------------
// below are for utopia20 cmd and parameters structure

typedef MS_U8 SC_GENERIC_PARAM;

typedef struct DLL_PACKED _SC_OPEN_PARAM
{
    MS_U8 u8SCID;
    MS_U8 u8Protocol;
    SC_Param* pParam;
    P_SC_Callback pfSmartNotify;
}SC_OPEN_PARAM, *PSC_OPEN_PARAM;

typedef struct DLL_PACKED _SC_RESET_PARAM
{
    MS_U8 u8SCID;
    SC_Param* pParam;
}SC_RESET_PARAM, *PSC_RESET_PARAM;

typedef struct DLL_PACKED _SC_GETATR_PARAM
{
    MS_U8 u8SCID;
    MS_U32 u32TimeOut;
    MS_U8 *pu8Atr;
    MS_U16 *pu16AtrLen;
    MS_U8 *pu8His;
    MS_U16 *pu16HisLen;
}SC_GETATR_PARAM, *PSC_GETATR_PARAM;

typedef struct DLL_PACKED _SC_CONFIG_PARAM
{
    MS_U8 u8SCID;
    SC_Param* pParam;
}SC_CONFIG_PARAM, *PSC_CONFIG_PARAM;

typedef struct DLL_PACKED _SC_SEND_PARAM
{
    MS_U8 u8SCID;
    MS_U8 *pu8SendData;
    MS_U16 u16SendDataLen;
    MS_U32 u32TimeoutMs;
}SC_SEND_PARAM, *PSC_SEND_PARAM;

typedef struct DLL_PACKED _SC_RECV_PARAM
{
    MS_U8 u8SCID;
    MS_U8 *pu8ReadData;
    MS_U16 *pu16ReadDataLen;
    MS_U32 u32TimeoutMs;
}SC_RECV_PARAM, *PSC_RECV_PARAM;

typedef struct DLL_PACKED _SC_GETCAPS_PARAM
{
    SC_Caps *pstCaps;
}SC_GETCAPS_PARAM, *PSC_GETCAPS_PARAM;

typedef struct DLL_PACKED _SC_SETPPS_PARAM
{
    MS_U8 u8SCID;
    MS_U8 u8SCProtocol;
    MS_U8 u8Di;
    MS_U8 u8Fi;
}SC_SETPPS_PARAM, *PSC_SETPPS_PARAM;

typedef struct DLL_PACKED _SC_GETSTATUS_PARAM
{
    MS_U8 u8SCID;
    SC_Status *pstStatus;
}SC_GETSTATUS_PARAM, *PSC_GETSTATUS_PARAM;

typedef SC_DbgLv SC_SETDBGLEVEL_PARAM;

typedef struct DLL_PACKED _SC_RAWEXCHANGE_PARAM
{
    MS_U8 u8SCID;
    MS_U8 *pu8SendData;
    MS_U16 *u16SendDataLen;
    MS_U8 *pu8ReadData;
    MS_U16 *u16ReadDataLen;
}SC_RAWEXCHANGE_PARAM, *PSC_RAWEXCHANGE_PARAM;

typedef struct DLL_PACKED _SC_CARDVOLTAGE_CONFIG_PARAM
{
    MS_U8 u8SCID;
    SC_VoltageCtrl eVoltage;
}SC_CARDVOLTAGE_CONFIG_PARAM, *PSC_CARDVOLTAGE_CONFIG_PARAM;

typedef struct DLL_PACKED _SC_KDRVKERNELMODEPOLL_PARAM
{
    MS_U8 u8SCID;
    MS_U32 *pu32Event;
    MS_U32 u32TimeoutMs;
}SC_KDRVKERNELMODEPOLL_PARAM, *PSC_KDRVKERNELMODEPOLL_PARAM;

typedef struct DLL_PACKED _SC_RESET_ATR_PARAM
{
    MS_U8 u8SCID;
    SC_Param* pParam;
    MS_U8 *pu8Atr;
    MS_U16 *pu16AtrLen;
    MS_U8 *pu8His;
    MS_U16 *pu16HisLen;
}SC_RESET_ATR_PARAM, *PSC_RESET_ATR_PARAM;

typedef struct DLL_PACKED _SC_T0_SENDRECV_PARAM
{
    MS_U8 u8SCID;
    MS_U8 *pu8SendData;
    MS_U16 u16SendLen;
    MS_U8 *pu8RecvData;
    MS_U16 *pu16RecvLen;
}SC_T0_SENDRECV_PARAM, *PSC_T0_SENDRECV_PARAM;

typedef struct DLL_PACKED _SC_T1_SENDRECV_PARAM
{
    MS_U8 u8SCID;
    MS_U8 *pu8SendData;
    MS_U16 *u16SendDataLen;
    MS_U8 *pu8ReadData;
    MS_U16 *u16ReadDataLen;
}SC_T1_SENDRECV_PARAM, *PSC_T1_SENDRECV_PARAM;

typedef struct DLL_PACKED _SC_T14_SENDRECV_PARAM
{
    MS_U8 u8SCID;
    MS_U8 *pu8SendData;
    MS_U16 u16SendLen;
    MS_U8 *pu8RecvData;
    MS_U16 *pu16RecvLen;
}SC_T14_SENDRECV_PARAM, *PSC_T14_SENDRECV_PARAM;

typedef struct DLL_PACKED _SC_GETINFO_PARAM
{
    MS_U8 u8SCID;
    SC_Info *pstInfo;
}SC_GETINFO_PARAM, *PSC_GETINFO_PARAM;

typedef struct DLL_PACKED _SC_SETGUARDTIME_PARAM
{
    MS_U8 u8SCID;
    MS_U8 u8GuardTime;
}SC_SETGUARDTIME_PARAM, *PSC_SETGUARDTIME_PARAM;

typedef EN_POWER_MODE SC_SETPOWERSTATE_PARAM;

#ifdef __cplusplus
}
#endif

#endif // _DRV_SC_V2_H_
