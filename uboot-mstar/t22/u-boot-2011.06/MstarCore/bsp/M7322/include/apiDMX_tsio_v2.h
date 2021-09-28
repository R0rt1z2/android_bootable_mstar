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

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  File name: apiDMX_tsio_v2.h
//  Description: Demux (dmx) api header file
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DMX_TSIO_V2_H_
#define _DMX_TSIO_V2_H_

#include "MsDevice.h"

#ifdef __cplusplus
extern "C"
{
#endif

//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
// Type and Structure Declaration
//-------------------------------------------------------------------------------------------------

typedef struct DLL_PACKED _DMX_TSIO_Service_SetDMAoutVC_PARAM
{
    MS_U32              u32Res;
    MS_U16              u16Handle;
    TSIO_DMAOUT_SET*    eSet;
}DMX_TSIO_Service_SetDMAoutVC_PARAM,*PDMX_TSIO_Service_SetDMAoutVC_PARAM;

typedef struct DLL_PACKED _DMX_TSIO_Service_DMAout_WriteGet_PARAM
{
    MS_U32              u32Res;
    MS_U16              u16Handle;
    MS_PHY*             pphyWrite;
}DMX_TSIO_Service_DMAout_WriteGet_PARAM, *PDMX_TSIO_Service_DMAout_WriteGet_PARAM;

typedef struct DLL_PACKED _DMX_TSIO_Service_SetDestination_PARAM
{
    MS_U32              u32Res;
    MS_U16              u16Handle;
    TSIO_SERVICEDEST    eDest;
}DMX_TSIO_Service_SetDestination_PARAM,*PDMX_TSIO_Service_SetDestination_PARAM;

typedef struct DLL_PACKED _DMX_TSIO_Service_SetDMAinVC_PARAM
{
    MS_U32              u32Res;
    MS_U16              u16Handle;
    TSIO_DMAIN_SET*     eSet;
}DMX_TSIO_Service_SetDMAinVC_PARAM,*PDMX_TSIO_Service_SetDMAinVC_PARAM;

typedef struct DLL_PACKED _DMX_TSIO_Service_DMAinVC_Start_PARAM
{
    MS_U32              u32Res;
    MS_U16              u16Handle;
    MS_U32              u32Pacing;
}DMX_TSIO_Service_DMAinVC_Start_PARAM,*PDMX_TSIO_Service_DMAinVC_Start_PARAM;

typedef struct DLL_PACKED _DMX_TSIO_Service_SetLocdecKey_PARAM
{
    MS_U32              u32Res;
    MS_U16              u16Handle;
    TSIO_LOCKEY_SET*    eSet;
}DMX_TSIO_Service_SetLocdecKey_PARAM,*PDMX_TSIO_Service_SetLocdecKey_PARAM;

typedef struct DLL_PACKED _DMX_TSIO_Service_LocdecKeyEnable_PARAM
{
    MS_U32              u32Res;
    MS_U16              u16Handle;
    MS_BOOL             bEnable;
}DMX_TSIO_Service_LocdecKeyEnable_PARAM,*PDMX_TSIO_Service_LocdecKeyEnable_PARAM;

typedef struct DLL_PACKED _DMX_TSIO_Service_PidOpen_PARAM
{
    MS_U32              u32Res;
    MS_U16              u16Handle;
    TSIO_FLT_CFG*       pstFltCfg;
}DMX_TSIO_Service_PidOpen_PARAM,*PDMX_TSIO_Service_PidOpen_PARAM;

typedef struct DLL_PACKED _DMX_TSIO_Service_PidClose_PARAM
{
    MS_U32              u32Res;
    MS_U16              u16Handle;
    MS_U32              u32DmxId;
}DMX_TSIO_Service_PidClose_PARAM,*PDMX_TSIO_Service_PidClose_PARAM;

typedef struct DLL_PACKED _DMX_TSIO_Service_Alloc_PARAM
{
    MS_U32              u32Res;
    MS_U8               u8Sid;
    MS_U16*             u16Handle;
}DMX_TSIO_Service_Alloc_PARAM,*PDMX_TSIO_Service_Alloc_PARAM;

typedef struct DLL_PACKED _DMX_TSIO_Service_Free_PARAM
{
    MS_U32              u32Res;
    MS_U16              u16Handle;
}DMX_TSIO_Service_Free_PARAM,*PDMX_TSIO_Service_Free_PARAM;

typedef struct DLL_PACKED _DMX_TSIO_RegisterIntCb_PARAM
{
    MS_U32              u32Res;
    TSIOIntTypeCB       efn;
}DMX_TSIO_RegisterIntCb_PARAM,*PDMX_TSIO_RegisterIntCb_PARAM;

typedef struct DLL_PACKED _DMX_TSIO_Init_PARAM
{
    MS_U32              u32Res;
    TSIO_Init_Set*      eParm;
}DMX_TSIO_Init_PARAM,*PDMX_TSIO_Init_PARAM;

typedef struct DLL_PACKED _DMX_TSIO_Open_PARAM
{
    MS_U32              u32Res;
    TSIO_CLK            eClk;
}DMX_TSIO_Open_PARAM, *PDMX_TSIO_Open_PARAM;

typedef struct DLL_PACKED _DMX_TSIO_Cmd_PARAM
{
    MS_U32              u32Res;
    TSIO_CMD            eCmd;
    MS_U32              u32Value;
}DMX_TSIO_Cmd_PARAM,*PDMX_TSIO_Cmd_PARAM;

typedef struct DLL_PACKED _DMX_TSIO_CC_PARAM
{
    MS_U32              u32Res;
    TSIO_CC_Set*        Parm;
    TSIO_CC_RESULT*     pResult;
}DMX_TSIO_CC_PARAM,*PDMX_TSIO_CC_PARAM;

typedef struct DLL_PACKED _DMX_TSIO_GetInfo_PARAM
{
    MS_U32              u32Res;
    TSIO_INFO           eInfo;
    MS_U32              u32Arg;
    MS_U32*             u32Retinfo;
}DMX_TSIO_GetInfo_PARAM,*PDMX_TSIO_GetInfo_PARAM;

#ifdef __cplusplus
}
#endif


#endif //_DMX_TSIO_V2_H_
