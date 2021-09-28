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
#ifndef _DRVEMMFLT_H_
#define _DRVEMMFLT_H_

////////////////////////////////////////////////////////////////////////////////
/// @brief to filter EMM packet
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C"
{
#endif

#include "MsTypes.h"
#include "UFO.h"

////////////////////////////////////////////////////////////////////////////////
// Define & data type
////////////////////////////////////////////////////////////////////////////////
#define MSIF_EMMFLT_LIB_CODE	{'E','M','M','F'}    //Lib code
#define MSIF_EMMFLT_LIBVER		{'0','1'}            //LIB version
#define MSIF_EMMFLT_BUILDNUM    {'0','1'}            //Build Number
#define MSIF_EMMFLT_CHANGELIST  {'0','0','3','4','8','0','3','3'} //P4 ChangeList Number

#define EMMRASP_SHAREINT

#define EMMFLT_DRV_VERSION                /* Character String for DRV/API version             */  \
    MSIF_TAG,                           /* 'MSIF'                                           */  \
    MSIF_CLASS,                         /* '00'                                             */  \
    MSIF_CUS,                           /* 0x0000                                           */  \
    MSIF_MOD,                           /* 0x0000                                           */  \
    MSIF_CHIP,                                                                                  \
    MSIF_CPU,                                                                                   \
    MSIF_EMMFLT_LIB_CODE,                  /* IP__                                             */  \
    MSIF_EMMFLT_LIBVER,                    /* 0.0 ~ Z.Z                                        */  \
    MSIF_EMMFLT_BUILDNUM,                  /* 00 ~ 99                                          */  \
    MSIF_EMMFLT_CHANGELIST,                /* CL#                                              */  \
    MSIF_OS

typedef MS_U32                         EMMFLT_RESULT;

typedef void (*EMMFLT_Notify)(MS_U32 conn, MS_U32 reason, void *pData);


#define DRV_EMMFLT_OK                   TRUE
#define DRV_EMMFLT_FAIL                 FALSE
#define DRV_EMMFLT_INVALID_REQUEST      3



#define EMM_SRC_FILEIN      0
#define EMM_SRC_LIVEIN      1

typedef enum
{
    EMM_ENGINE = 0,
    RASP_ENGINE,
    MAX_ENGINE,
}EMMRASP_e;

typedef enum _EMM_EVENT_
{
    EMM_EVENT_ARRIVE = 0,
    EMM_EVENT_OVERFLOW,
} EMM_EVENT_e;


#if defined(UFO_PUBLIC_HEADER_500_3)
typedef enum _EMM_SrcType_
{
    EMM_SRC_TS0             = 0,
    EMM_SRC_TS1,
    EMM_SRC_TS2,            // 2
    EMM_SRC_TS3,
    EMM_SRC_TS4,
    EMM_SRC_TS5,            // 5
    EMM_SRC_TS6,            // 6
    EMM_SRC_TSO,            // 7
    EMM_SRC_INTERDEMOD_0,   // 8
    EMM_SRC_INTERDEMOD_1,   // 9
    EMM_SRC_MEMORY,         // 10
}EMM_SRCTYPE_e;
#else
typedef enum _EMM_SrcType_
{
    EMM_SRC_TS0 = 0,
    EMM_SRC_TS1,
    EMM_SRC_TS2,
    EMM_SRC_TS3,
    EMM_SRC_INTERDEMOD_0,
    EMM_SRC_INTERDEMOD_1,
    EMM_SRC_MEMORY,
}EMM_SRCTYPE_e;
#endif

typedef enum
{
    EMM_TIDMODE_INGORE = 0x0,
    EMM_TIDMODE_ACCEPT_ALL,
    EMM_TIDMODE_ACCEPT_ADDRESS,
    EMM_TIDMODE_RESERVED,
} EMMDRV_TIDMODE_e;

typedef enum _EMM_OutType_
{
    EMM_OUT_NORMAL = 0,
    EMM_OUT_184BYTES,
    EMM_OUT_PACKETNUM,
}EMM_OUTTYPE_e;

typedef struct _EMMFLT_Require_
{
    MS_U32 u32DstBufSize;
    MS_U32 u32AlignBytes;
    MS_U8  u8EMMNumber;
} EMMFLT_Require_t;


typedef struct _EMM_FILTER_DEF
{
    MS_U8   filter_type;         //this can be set to LABEL_00, 01, 02, 03
    MS_U8   filter_address[4];   //
    MS_U8   filter_address_mask[4];

} EMM_FILTER_DEF_t;


////////////////////////////////////////////////////////////////////////////////
// Extern Function
////////////////////////////////////////////////////////////////////////////////

EMMFLT_RESULT MDrv_EMMFLT_Init(EMMFLT_Require_t *pRequire);
EMMFLT_RESULT MDrv_EMMFLT_GetHwBufCnt(MS_U32 *pHwBufCnt);
EMMFLT_RESULT MDrv_EMMFLT_SetDbgLevel(MS_U32 u32Level);
EMMFLT_RESULT MDrv_EMMFLT_SetNotify(EMMFLT_Notify Notify);
EMMFLT_RESULT MDrv_EMMFLT_SetEmmPID(MS_U32 connection,MS_U16 u16EmmPID);
EMMFLT_RESULT MDrv_EMMFLT_SetTidMode(MS_U32 connection, MS_U8 u8TidValue, EMMDRV_TIDMODE_e eTIDMODE);
EMMFLT_RESULT MDrv_EMMFLT_EnableEmmTID(MS_U32 connection, MS_BOOL bEnable);
EMMFLT_RESULT MDrv_EMMFLT_SetFilterCfg(MS_U32 connection, MS_U8 FilerIndex, EMM_FILTER_DEF_t *pFilter);
EMMFLT_RESULT MDrv_EMMFLT_Fire(MS_U32 connection);

EMMFLT_RESULT MDrv_EMMFLT_SetDstBufInfo(MS_U32 x_connection, MS_U32 u32BufAddr, MS_U32 u32BufSize);
EMMFLT_RESULT MDrv_EMMFLT_EmmProcessingFinished(MS_U8 *emm_buf/* RPC SIZECONST(256) */);
EMMFLT_RESULT MDrv_EMMFLT_Deinit(void);
EMMFLT_RESULT MDrv_EMMFLT_SrcSelect(MS_U32 x_connection, MS_U32 u32SrcFrom, EMM_SRCTYPE_e eSrcType);
EMMFLT_RESULT MDrv_EMMFLT_StopEmm(MS_U32 x_connection);
EMMFLT_RESULT MDrv_EMMFLT_EmmProcessingFinished(MS_U8 *emm_buf/* RPC SIZECONST(256) */);
EMMFLT_RESULT MDrv_EMMFLT_SetOutputType(MS_U32 x_connection, EMM_OUTTYPE_e eOutType);
EMMFLT_RESULT MDrv_EMMFLT_ConnectCheck(MS_U32 connection);

EMMFLT_RESULT MDrv_EMMFLT_SetExtendConfig(MS_U32 x_connection, MS_U16 type, MS_U16 extendbytes, MS_U16 syncbyte);
EMMFLT_RESULT MDrv_EMMFLT_InputMode(MS_U32 x_connection, MS_BOOL bSerial);

void EMMRASP_IntAttach(EMMRASP_e eEng, MS_BOOL Enable);

#ifdef __cplusplus
}
#endif
#endif//_DRVEMMFLT_H_

