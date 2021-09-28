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
#ifndef _APIGPD_PRIV_H_
#define _APIGPD_PRIV_H_

////////////////////////////////////////////////////////////////////////////////
/// @brief GPD control driver
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C"
{
#endif

#include "MsTypes.h"
#include "apiGPD.h"

#define SUPPORT_GPD_NUM_MAX     1

typedef void* (*IOCTL_GPD_INIT)(MS_PHY);
typedef MS_S32 (*IOCTL_GPD_INPUT_SOURCE)(gpd_pic_info*, MS_PHY, MS_U32);
typedef MS_S32 (*IOCTL_GPD_OUTPUT_DECODE)(MS_PHY, MS_U32, MS_U32);
typedef MS_S32 (*IOCTL_GPD_OUTPUT_DECODE_ROI)(MS_PHY, MS_U32, MS_U32, gpd_roi_info*);
typedef MS_S32 (*IOCTL_GPD_OUTPUT_DECODE_MGIF)(MS_PHY, MS_U32, MS_U32);
typedef MS_S32 (*IOCTL_GPD_GET_DURATION)(void);
typedef void (*IOCTL_GPD_SET_GIF_MODE)(int);
typedef void (*IOCTL_GPD_SCALING_ENABLE)(MS_U32);
typedef void (*IOCTL_GPD_SCALING_DISABLE)(void);
typedef MS_U32 (*IOCTL_GPD_SET_CONTROL)(gpd_user_cmd, MS_VIRT);
typedef MS_U32 (*IOCTL_GPD_GET_CONTROL)(gpd_get_cmd, MS_VIRT);
typedef MS_U32 (*IOCTL_GPD_GET_CRC_RESULT)(const MS_U8 *, MS_U32);


typedef struct __attribute__((packed))
{
    MS_U8 u8DecoderInUse[SUPPORT_GPD_NUM_MAX];
}GPD_RESOURCE_PRIVATE;

typedef struct _GPD_INSTANCE_PRIVATE
{
    void* pResource;
    // instance variable
    MS_U8                                       Index_of_decoder;
    MS_BOOL                                     bGPDInUse;
    MS_BOOL                                     bGPDSuspend;

    IOCTL_GPD_INIT                              fpGPDInit;
    IOCTL_GPD_INPUT_SOURCE                      fpGPDInputSource;
    IOCTL_GPD_OUTPUT_DECODE                     fpGPDOutputDecode;
    IOCTL_GPD_OUTPUT_DECODE_ROI                 fpGPDOutputDecodeROI;
    IOCTL_GPD_OUTPUT_DECODE_MGIF                fpGPDOutputDecodeMGIF;
    IOCTL_GPD_GET_DURATION                      fpGPDGetDuration;
    IOCTL_GPD_SET_GIF_MODE                      fpGPDSetGIFMode;
    IOCTL_GPD_SCALING_ENABLE                    fpGPDScalingEnable;
    IOCTL_GPD_SCALING_DISABLE                   fpGPDScalingDisable;
    IOCTL_GPD_SET_CONTROL                       fpGPDSetControl;
    IOCTL_GPD_GET_CONTROL                       fpGPDGetControl;
    IOCTL_GPD_GET_CRC_RESULT                    fpGPDGetCRCResult;

}GPD_INSTANCE_PRIVATE;

void GPDRegisterToUtopia(FUtopiaOpen ModuleType);
MS_U32 GPDOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute);
MS_U32 GPDClose(void* pInstance);
MS_U32 GPDIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs);
MS_U32 GPDStr(MS_U32 u32PowerState, void* pModule);

#ifdef CONFIG_UTOPIA_PROC_DBG_SUPPORT
MS_U32          GPDMdbIoctl                         (MS_U32 u32Cmd, const void* const pArgs);
void _MApi_GPD_GetMdbInfo(MS_U64* u64ReqHdl);
#endif

void *_MApi_GPD_Init(MS_PHY SrcBuf);
MS_S32 _MApi_GPD_InputSource(gpd_pic_info* param, MS_PHY InputBuf, MS_U32 Size);
MS_S32 _MApi_GPD_OutputDecode(MS_PHY DecodeBuf, MS_U32 ocolor, MS_U32 Size);
MS_S32 _MApi_GPD_OutputDecodeROI(MS_PHY DecodeBuf, MS_U32 ocolor, MS_U32 Size, gpd_roi_info *roi);
MS_S32 _MApi_GPD_OutputDecodeMGIF(MS_PHY DecodeBuf, MS_U32 ocolor, MS_U32 Size);
MS_S32 _MApi_GPD_GetDuration(void);
void _MApi_GPD_SetGIFMode(MS_U32 mode);

void _MApi_GPD_ScalingEnable(MS_U32 mode);
void _MApi_GPD_ScalingDisable(void);
MS_U32 _MApi_GPD_SetControl(gpd_user_cmd cmd_id, MS_VIRT param);
MS_U32 _MApi_GPD_GetControl(gpd_get_cmd cmd_id, MS_VIRT param);
MS_U32 _MApi_GPD_GetCRCResult(const MS_U8 *pu8Data, MS_U32 u32Size, MS_U32, MS_U32);

#ifdef __cplusplus
}
#endif
#endif // _APIGPD_PRIV_H_





