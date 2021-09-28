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
/// @brief  GPD Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _API_GPD_V2_H_
#define _API_GPD_V2_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "MsTypes.h"
//-------------------------------------------------------------------------------------------------
// Defines
//-------------------------------------------------------------------------------------------------
// below for utopia20 structure

typedef enum {
    MApi_CMD_GPD_Init,
    MApi_CMD_GPD_InputSource,
    MApi_CMD_GPD_OutputDecode,
    MApi_CMD_GPD_OutputDecodeROI,
    MApi_CMD_GPD_OutputDecodeMGIF,
    MApi_CMD_GPD_GetDuration,
    MApi_CMD_GPD_SetGIFMode,
    MApi_CMD_GPD_ScalingEnable,
    MApi_CMD_GPD_ScalingDisable,
    MApi_CMD_GPD_SetControl,
    MApi_CMD_GPD_GetControl,
    MApi_CMD_GPD_GetCRCResult
} E_GPD_V2_IOCTL_CMD;


typedef struct DLL_PACKED _GPD_INIT_PARAM
{
    MS_PHY SrcBuf;
    void* retVal;
}GPD_INIT_PARAM, *PGPD_INIT_PARAM;

typedef struct DLL_PACKED _GPD_INPUTSOURCE_PARAM
{
    gpd_pic_info* param;
    MS_PHY InputBuf;
    MS_U32 Size;
    MS_S32 retVal;
}GPD_INPUTSOURCE_PARAM, *PGPD_INPUTSOURCE_PARAM;

typedef struct DLL_PACKED _GPD_OUTPUTDECODE_PARAM
{
    MS_PHY DecodeBuf;
    MS_U32 ocolor;
    MS_U32 Size;
    MS_S32 retVal;
}GPD_OUTPUTDECODE_PARAM, *PGPD_OUTPUTDECODE_PARAM;

typedef struct DLL_PACKED _GPD_OUTPUTDECODEROI_PARAM
{
    MS_PHY DecodeBuf;
    MS_U32 ocolor;
    MS_U32 Size;
    gpd_roi_info *roi;
    MS_S32 retVal;
}GPD_OUTPUTDECODEROI_PARAM, *PGPD_OUTPUTDECODEROI_PARAM;

typedef struct DLL_PACKED _GPD_GETDURATION_PARAM
{
    MS_S32 retVal;
}GPD_GETDURATION_PARAM, *PGPD_GETDURATION_PARAM;

typedef struct DLL_PACKED _GPD_SETGIFMODE_PARAM
{
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
    int mode;
#else
   MS_U32 mode;
#endif
}GPD_SETGIFMODE_PARAM, *PGPD_SETGIFMODE_PARAM;

typedef struct DLL_PACKED _GPD_SCALINGENABLE_PARAM
{
    MS_U32 mode;
}GPD_SCALINGENABLE_PARAM, *PGPD_SCALINGENABLE_PARAM;


typedef struct DLL_PACKED _GPD_CONTROL_PARAM
{
    gpd_user_cmd cmd_id;
    MS_VIRT param;
    MS_S32 retVal;
}GPD_CONTROL_PARAM, *PGPD_CONTROL_PARAM;

typedef struct DLL_PACKED _GPD_GET_CONTROL_PARAM
{
    gpd_get_cmd cmd_id;
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
    U32* param;
#else
    MS_VIRT param;
#endif
    MS_S32 retVal;
}GPD_GET_CONTROL_PARAM, *PGPD_GET_CONTROL_PARAM;

typedef struct DLL_PACKED _GPD_GETCRCRESULT_PARAM
{
    const MS_U8 *pu8Data;
    MS_U32 u32Size;
    MS_U32 u32Width;
    MS_U32 ocolor;
    MS_U32 retVal;
}GPD_GETCRCRESULT_PARAM, *PGPD_GETCRCRESULT_PARAM;


void GPDRegisterToUtopia(FUtopiaOpen ModuleType);
MS_U32 GPD_Open(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute);
MS_U32 GPD_Close(void* pInstance);
MS_U32 GPD_Ioctl(void* pInstance, MS_U32 u32Cmd, void* pArgs);


#ifdef __cplusplus
}
#endif

#endif // _API_GPD_V2_H_


