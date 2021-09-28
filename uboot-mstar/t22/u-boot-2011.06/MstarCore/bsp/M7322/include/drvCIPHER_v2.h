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
/// @brief  CIPHER Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_CIPHER_V2_H_
#define _DRV_CIPHER_V2_H_

#include "drvCIPHER.h"

#ifdef __cplusplus
extern "C"
{
#endif

//-------------------------------------------------------------------------------------------------
// Defines
//-------------------------------------------------------------------------------------------------
// below for utopia20 structure


typedef enum {
    E_MDRV_CMD_CIPHER_Init,
    E_MDRV_CMD_CIPHER_Reset,
    E_MDRV_CMD_CIPHER_ResetKey,
    E_MDRV_CMD_CIPHER_DbgLevel,
    E_MDRV_CMD_CIPHER_DMACalc,
    E_MDRV_CMD_CIPHER_HASH,
    E_MDRV_CMD_CIPHER_IsDMADone,
    E_MDRV_CMD_CIPHER_IsHASHDone,
    E_MDRV_CMD_CIPHER_SetPowerState,
    E_MDRV_CMD_CIPHER_KeySlot_Alloc,
    E_MDRV_CMD_CIPHER_KeySlot_Free,
    E_MDRV_CMD_CIPHER_KeySlot_SetKey,
    E_MDRV_CMD_CIPHER_HASH_Manual,
    E_MDRV_CMD_CIPHER_HMAC,
} eCipherIoctlOpt;

//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Type and Structure Declaration
//-------------------------------------------------------------------------------------------------
typedef struct DLL_PACKED _CIPHER_RESETKEY
{
    MS_U32          u32CAVid;
    MS_U32          u32KeyIdx;
    DRV_CIPHER_RET  ret;
} CIPHER_RESETKEY;

typedef struct DLL_PACKED _CIPHER_DBGLEVEL
{
    CIPHER_DBGMSG_LEVEL eDBGMsgLevel;
} CIPHER_DBGLEVEL;

typedef struct DLL_PACKED _CIPHER_DMACALC
{
    DRV_CIPHER_DMACFG   stCfg;
    MS_U32              *pu32CmdId;
    DRV_CIPHER_RET      ret;
} CIPHER_DMACALC, *PCIPHER_DMACALC;

typedef struct DLL_PACKED _CIPHER_HASHCALC
{
    DRV_CIPHER_HASHCFG  stCfg;
    MS_U32              *pu32CmdId;
    DRV_CIPHER_RET      ret;
} CIPHER_HASHCALC;

typedef struct DLL_PACKED _CIPHER_HASHCALC_MANUAL
{
    DRV_CIPHER_HASHCFG_MANUAL  stCfg;
    MS_U32                    *pu32CmdId;
    DRV_CIPHER_RET            ret;
} CIPHER_HASHCALC_MANUAL;

typedef struct DLL_PACKED _CIPHER_HMACCALC
{
    DRV_CIPHER_HMACCFG        stCfg;
    MS_U32                    *pu32CmdId;
    DRV_CIPHER_RET            ret;
} CIPHER_HMACCALC;

typedef struct DLL_PACKED _CIPHER_ISDMADONE
{
    MS_U32 u32CmdId;
    MS_U32 *pu32Exception;
} CIPHER_ISDMADONE, *PCIPHER_ISDMADONE;

typedef struct DLL_PACKED _CIPHER_ISHASHDONE
{
    MS_U32 u32CmdId;
    MS_U32 *pu32Exception;
} CIPHER_ISHASHDONE;

typedef struct DLL_PACKED _CIPHER_ALLOC
{
    MS_U32         *pu32CipherId;
    DRV_CIPHER_RET ret;
} CIPHER_ALLOC;

typedef struct DLL_PACKED _CIPHER_FREE
{
    MS_U32         u32CipherId;
    DRV_CIPHER_RET ret;
} CIPHER_FREE;

typedef struct DLL_PACKED _CIPHER_KEYSLOT_ALLOC
{
    MS_U32               *pu32KeySlotIdx;
    CIPHER_KS_ALLOW_HOST  eKeyAllowHost;
    DRV_CIPHER_RET        ret;
} CIPHER_KEYSLOT_ALLOC;

typedef struct DLL_PACKED _CIPHER_KEYSLOT_FREE
{
    MS_U32               u32KeySlotIdx;
    DRV_CIPHER_RET       ret;
} CIPHER_KEYSLOT_FREE;

typedef struct DLL_PACKED _CIPHER_KEYSLOT_SETKEY
{
    DRV_CIPHER_KS_CFG    stKSCfg;
    DRV_CIPHER_RET       ret;
} CIPHER_KEYSLOT_SETKEY;

typedef struct DLL_PACKED _CIPHER_GCMTag
{
    MS_U8                *pu8OutputBuff;
    MS_U32               u32OutputBuffSize;
    MS_U32               u32RetTagSize;
} CIPHER_GCMTAG, *PCIPHER_GCMTAG;

//-------------------------------------------------------------------------------------------------
// Extern Global Variabls
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Extern Functions
//-------------------------------------------------------------------------------------------------
MS_U32 CIPHEROpen(void** pInstantTmp, MS_U32 u32ModuleVersion, void* pAttribute);
MS_U32 CIPHERClose(void* pInstantTmp);
MS_U32 CIPHERIoctl(void* pInstantTmp, MS_U32 u32Cmd, void* pu32Args);

//////////////////////////////////////////////////////////////

#ifdef __cplusplus
}
#endif

#endif // _DRV_CIPHER_V2_H_
