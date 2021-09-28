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
#ifndef __DRV_AKL_V2_H__
#define __DRV_AKL_V2_H__

#include "drvAKL.h"
#include "UFO.h"

#ifdef UFO_AKL_CERT

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum {
    E_MDRV_CMD_AKL_Init,
    E_MDRV_CMD_AKL_Reset,
    E_MDRV_CMD_AKL_SetDbgLevel,
    E_MDRV_CMD_AKL_SetManualACK,
    E_MDRV_CMD_AKL_SetDMAKey,
    E_MDRV_CMD_AKL_SetDMAParserKey,
    E_MDRV_CMD_AKL_SetDSCMBKey,
    E_MDRV_CMD_AKL_SetTSIOKey,
    E_MDRV_CMD_AKL_SetManualACKMode,
    E_MDRV_CMD_AKL_CERTCmd,
} eAKLIoctlOpt;

typedef struct DLL_PACKED _AKL_DMA_KEY
{
    MS_U32 u32KeyIndex;
} AKL_DMA_KEY;

typedef struct DLL_PACKED _AKL_DMAPARSER_KEY
{
    MS_U32 u32OddKeyIndex;
    MS_U32 u32EvenKeyIndex;
} AKL_DMAPARSER_KEY;

typedef struct DLL_PACKED _AKL_DSCMB_KEY
{
    MS_U32 u32EngId;
    MS_U32 u32Index;
    AKL_Eng_Type eEngType;
    AKL_Key_Type eKeyType;
} AKL_DSCMB_KEY;

typedef struct DLL_PACKED _AKL_TSIO_KEY
{
    MS_U32 u32ServiceId;
    AKL_Key_Type eKeyType;
} AKL_TSIO_KEY;

typedef struct DLL_PACKED _AKL_DBGLEVEL
{
    AKL_DBGMSG_LEVEL eDBGMsgLevel;
} AKL_DBGLEVEL;

typedef struct DLL_PACKED _AKL_ACKMODE
{
    MS_BOOL bEnable;
} AKL_ACKMODE;

typedef struct DLL_PACKED _AKL_EXCHANGE
{
    AKL_CERT_Cmd eCmd;
    MS_U8 *pu8Data;
    MS_U32 u32DataSize;
} AKL_EXCHANGE;


#ifdef __cplusplus
}
#endif

#endif //UFO_AKL_CERT

#endif // __DRV_AKL_V2_H__
