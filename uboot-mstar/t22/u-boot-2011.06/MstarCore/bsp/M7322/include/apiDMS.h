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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @brief  DMS Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _API_DMS_H_
#define _API_DMS_H_

#include "MsCommon.h"
#include "apiDMS_Type.h"

#ifdef __cplusplus
extern "C" {
#endif
#include "UFO.h"

#define DMS_XC_PIPEID_DEFAULT 0
//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
EN_DMS_RESULT SYMBOL_WEAK MApi_DMS_Init(ST_DMS_INITDATA *pstDMS_InitData);
EN_DMS_RESULT SYMBOL_WEAK MApi_DMS_SetMemory(ST_DMS_SET_MEMORY_TYPE* pstDMS_SetMemType);

EN_DMS_RESULT SYMBOL_WEAK MApi_DMS_CreateWindow(ST_DMS_WINDOW *pstOutputWin, ST_DMS_CREATE_WIN_INFO *pstCreateWin_Info, MS_U32 *pu32WindowID);

EN_DMS_RESULT SYMBOL_WEAK MApi_DMS_SetDigitalDecodeSignalInfo(MS_U32 u32WindowID, ST_DMS_DISPFRAMEFORMAT *pstDispFrameFormat);

EN_DMS_RESULT SYMBOL_WEAK MApi_DMS_ClearDigitalDecodeSignalInfo(MS_U32 u32WindowID);

EN_DMS_RESULT SYMBOL_WEAK MApi_DMS_DestroyWindow(MS_U32 u32WindowID);

EN_DMS_RESULT SYMBOL_WEAK MApi_DMS_SetWindow(MS_U32 u32WindowID, ST_DMS_SETWIN_INFO *pstDMS_SetWin_Info);

EN_DMS_RESULT SYMBOL_WEAK MApi_DMS_GetWindowInfo(MS_U32 u32WindowID, ST_DMS_WINDOW_INFO *pstDMS_GetWin_Info);

EN_DMS_RESULT SYMBOL_WEAK MApi_DMS_Video_Flip(MS_U32 u32WindowID, ST_DMS_DISPFRAMEFORMAT* pstDispFrameFormat);

EN_DMS_RESULT SYMBOL_WEAK MApi_DMS_SetZOrder(MS_U32 u32WindowID, ST_DMS_ZORDER_INFO *pstZorderInfo);

EN_DMS_RESULT SYMBOL_WEAK MApi_DMS_Video_Freeze(MS_U32 u32WindowID, ST_DMS_FREEZE_WINDOW_INFO *pstWindowFreeze);

EN_DMS_RESULT SYMBOL_WEAK MApi_DMS_Video_Mute(MS_U32 u32WindowID, ST_DMS_MUTE_WINDOW_INFO *pstWindowMute);

EN_DMS_RESULT SYMBOL_WEAK MApi_DMS_Set_MuteColor(MS_U32 u32WindowID, ST_DMS_MUTE_COLOR_INFO *pstMuteColor);

EN_DMS_RESULT SYMBOL_WEAK MApi_DMS_GetStatus(EN_DMS_STATUS *peStatus);
EN_DMS_RESULT SYMBOL_WEAK MApi_DMS_GetCapability(EN_DMS_CAPABILITY *peCapability);

EN_DMS_RESULT SYMBOL_WEAK MApi_DMS_GetExistedWindows(MS_U32 *pu32WindowNums);
EN_DMS_RESULT SYMBOL_WEAK MApi_DMS_SetOutputLayer(ST_DMS_WINDOW *pstLayer);
EN_DMS_RESULT SYMBOL_WEAK MApi_DMS_Set_3D_Mode(MS_U32 u32WindowID, ST_DMS_3D_INFO *pst3DInfo);

EN_DMS_RESULT SYMBOL_WEAK MApi_DMS_Set_CaptureInfo(MS_U32 u32WindowID, ST_DMS_SET_CAPTURE_INFO *pstSetCaptureInfo);
EN_DMS_RESULT SYMBOL_WEAK MApi_DMS_Get_CaptureBuffer(MS_U32 u32WindowID, ST_DMS_CAPTURE_INFO *pstCaptureInfo);
EN_DMS_RESULT SYMBOL_WEAK MApi_DMS_Release_CaptureBuffer(MS_U32 u32WindowID, ST_DMS_CAPTURE_INFO *pstCaptureInfo);
EN_DMS_RESULT SYMBOL_WEAK MApi_DMS_GetWindowID(ST_DMS_GET_ID_INFO *pstGetIDInfo, MS_U32* pu32WindowID);
EN_DMS_RESULT SYMBOL_WEAK MApi_DMS_Security_Control(EN_DMS_SECURITY_ACTION enAction, ST_DMS_SECURITY_HANDLER* pstDMSHandler);
EN_DMS_RESULT SYMBOL_WEAK MApi_DMS_SetMetaData(MS_U32 u32WindowID, ST_DMS_METADATA_INFO *pstMetaDataInfo);
#ifndef UFO_DMS_OLD_VERSION
EN_DMS_RESULT SYMBOL_WEAK MApi_DMS_DynamicScaling_Enable(MS_U32 u32WindowID, MS_BOOL bEnable);
EN_DMS_RESULT SYMBOL_WEAK MApi_DMS_RegisterFrameReleaseFun(MS_U32 u32WindowID, FUNC_DMS_SOURCEFRAMERELEASE pfFunVdecFrameRelease);
EN_DMS_RESULT SYMBOL_WEAK MApi_DMS_RegisterFirstDisplayFun(MS_U32 u32WindowID, FUNC_DMS_DISPLAYINFO pfFunDisplayInfo);
EN_DMS_RESULT SYMBOL_WEAK MApi_DMS_RegisterCallBackFun(MS_U32 u32WindowID, ST_DMS_CALLBACK_INFO *pstCallBackInfo);
EN_DMS_RESULT SYMBOL_WEAK MApi_DMS_GetVideoDelay(MS_U32 u32WindowID, ST_DMS_VIDEO_DELAY_INFO *pstVideoDelayInfo);
#endif
#ifdef __cplusplus
}
#endif

#endif
