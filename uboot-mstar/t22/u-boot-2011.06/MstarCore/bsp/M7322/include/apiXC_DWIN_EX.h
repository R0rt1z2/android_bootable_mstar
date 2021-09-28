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
//==============================================================================
// [apiXC_DWIN_EX.h]
// Date: 20120221
// Descriptions: structure pool
//==============================================================================
#ifndef APIXC_DWIN_EX_H
#define APIXC_DWIN_EX_H

// Common Definition
#include "MsCommon.h"

// Api
#include "apiXC_EX.h"

#ifdef __cplusplus
extern "C"
{
#endif

//#include "mvideo_context_ex.h"
//==============================================================================
//==============================================================================
#ifdef APIXC_DWIN_EX_C//MVIDEO_CONTEXT_C
#define INTERFACE
#else
#define INTERFACE extern
#endif

#define XC_DIP_EX_MAX_BUFF_CNT 8UL
#define XC_DIP_EX_AUTO_BUFF_CNT 0UL
/// Define window type for DIP
typedef enum
{
    E_XC_DIP_EX_DIP_WINDOW = 0,         ///< DIP window
    E_XC_DIP_EX_MAX_WINDOW          /// The max support window
} XC_DIP_EX_WIN;

typedef enum
{
    E_XC_DIP_EX_RGB_SWAPTO_BGR = 0,         ///< RGB Swap to BGR
    E_XC_DIP_EX_RGB_SWAP_MAX                ///< The max support format
} XC_DIP_EX_RGB_SWAP_TYPE;

/// DIP source data format
typedef enum
{
    /// CrYCb domain.
    E_XC_DIP_EX_DATA_FMT_YUV422 = 0,
    /// CrYCb domain.
    E_XC_DIP_EX_DATA_FMT_RGB565,
    /// RGB domain (CSC bypass).
    E_XC_DIP_EX_DATA_FMT_ARGB8888,
    /// YUV420
    E_XC_DIP_EX_DATA_FMT_YUV420,
    E_XC_DIP_EX_DATA_FMT_MAX
} XC_DIP_EX_DATA_FMT;

/// DIP scan mode
typedef enum
{
    /// progessive.
    E_XC_DIP_EX_SCAN_MODE_PROGRESSIVE,
    /// interlace.
    E_XC_DIP_EX_SCAN_MODE_extern,
} XC_DIP_EX_SCAN_MODE;

typedef enum
{
	E_XC_DIP_EX_CAP_NOT_EXIST = 0x0,
	E_XC_DIP_EX_CAP_EXIST = 	0x1,
	E_XC_DIP_EX_CAP_420TILE = 	0x2,
	E_XC_DIP_EX_CAP_SCALING_DOWN = 	0x4,
	E_XC_DIP_EX_CAP_MIRROR = 	0x8,
	E_XC_DIP_EX_CAP_DIPR 	= 	0x10,
	E_XC_DIP_EX_CAP_R2Y 	= 	0x20,
    E_XC_DIP_EX_CAP_OP1_CAPTURE  =   0x40,
} XC_DIP_EX_CAP;

typedef enum
{
    E_XC_DIP_EX_VOP2   =0x00,
    E_XC_DIP_EX_OP2    =0x01,
    E_XC_DIP_EX_VIP    =0x02,
    E_XC_DIP_EX_BRI    =0x03,
} XC_DIP_EX_OP_CAPTURE;

typedef struct
{
    MS_U8               u8BufCnt;
    MS_PHY              u32YBuf[XC_DIP_EX_MAX_BUFF_CNT];
    MS_PHY              u32CBuf[XC_DIP_EX_MAX_BUFF_CNT];
} XC_DIP_EX_BUFFER_INFO;

typedef struct
{
    XC_DIP_EX_WIN      eWindow;
    MS_U32              u32DipChipCaps;
} XC_DIP_EX_CHIPCAPS;

typedef struct
{
    MS_U8  u8BufCnt;
    MS_U16 u16Width;
    MS_U16 u16Height;
	MS_U16 u16Pitch;
    MS_PHY u32BufStart;
    MS_PHY u32BufEnd;
    XC_DIP_EX_SOURCE_TYPE enSource;
} XC_DIP_EX_WINPROPERTY;

typedef struct
{
    MS_U16 u16Width;
    MS_U16 u16Height;
	MS_U16 u16Pitch;
    MS_PHY u32YBufAddr;
    MS_PHY u32CBufAddr;
    XC_DIP_EX_DATA_FMT enDataFmt;
}XC_DIP_EX_DIPR_PROPERTY;

XC_EX_RETURN_VALUE MApi_XC_DIP_EX_InitByDIP(XC_DIP_EX_WIN eWindow);
XC_EX_RETURN_VALUE MApi_XC_DIP_EX_SetFrameBufferAddress(MS_PHY u32FBAddress, MS_PHY u32FBSize, XC_DIP_EX_WIN eWindow);
XC_EX_RETURN_VALUE MApi_XC_DIP_EX_SetInputSource(XC_EX_INPUT_SOURCE_TYPE enInputSourceType, XC_DIP_EX_WIN eWindow);
XC_EX_RETURN_VALUE MApi_XC_DIP_EX_GetInputSource(XC_EX_INPUT_SOURCE_TYPE *enInputSourceType, XC_DIP_EX_WIN eWindow);
XC_EX_RETURN_VALUE MApi_XC_DIP_EX_DisableInputSource(MS_BOOL bDisable, XC_DIP_EX_WIN eWindow);
XC_EX_RETURN_VALUE MApi_XC_DIP_EX_SetWindow(XC_EX_SETWIN_INFO *pstXC_SetWin_Info, MS_U32 u32InitDataLen, XC_DIP_EX_WIN eWindow);
XC_EX_RETURN_VALUE MApi_XC_DIP_EX_GetDEWindow(XC_EX_WINDOW_TYPE *psWin, XC_DIP_EX_WIN eWindow);
XC_EX_RETURN_VALUE MApi_XC_DIP_EX_SetCaptureWindowVstart(MS_U16 u16Vstart, XC_DIP_EX_WIN eWindow);
XC_EX_RETURN_VALUE MApi_XC_DIP_EX_SetCaptureWindowHstart(MS_U16 u16Hstart, XC_DIP_EX_WIN eWindow);
XC_EX_RETURN_VALUE MApi_XC_DIP_EX_SetCaptureWindowVsize(MS_U16 u16Vsize, XC_DIP_EX_WIN eWindow);
XC_EX_RETURN_VALUE MApi_XC_DIP_EX_SetCaptureWindowHsize(MS_U16 u16Hsize, XC_DIP_EX_WIN eWindow);
XC_EX_RETURN_VALUE MApi_XC_DIP_EX_GetCaptureWindow(XC_EX_WINDOW_TYPE* capture_win, XC_DIP_EX_WIN eWindow);
XC_EX_RETURN_VALUE MApi_XC_DIP_EX_EnableR2Y(MS_BOOL benable, XC_DIP_EX_WIN eWindow);

//==============Set===================
XC_EX_RETURN_VALUE MApi_XC_DIP_EX_SWReset(XC_DIP_EX_WIN eWindow);
XC_EX_RETURN_VALUE MApi_XC_DIP_EX_FrameRateCtrl(MS_BOOL bEnable,MS_U16 u16In,MS_U16 u16Out, XC_DIP_EX_WIN eWindow);
XC_EX_RETURN_VALUE MApi_XC_DIP_EX_CapOneFrame(XC_DIP_EX_WIN eWindow);
XC_EX_RETURN_VALUE MApi_XC_DIP_EX_CapOneFrameFast(XC_DIP_EX_WIN eWindow);
XC_EX_RETURN_VALUE MApi_XC_DIP_EX_SetPinpon(MS_BOOL bPinpon,MS_PHY u32PinponAddr,XC_DIP_EX_WIN eWindow);
XC_EX_RETURN_VALUE MApi_XC_DIP_EX_ClearInt(MS_U16 u8mask, XC_DIP_EX_WIN eWindow);
XC_EX_RETURN_VALUE MApi_XC_DIP_EX_Ena(MS_BOOL bEnable, XC_DIP_EX_WIN eWindow);
XC_EX_RETURN_VALUE MApi_XC_DIP_EX_EnaInt(MS_U16 u8mask, MS_BOOL bEnable, XC_DIP_EX_WIN eWindow);
XC_EX_RETURN_VALUE MApi_XC_DIP_EX_SetSourceScanType(XC_DIP_EX_SCAN_MODE enScanMode, XC_DIP_EX_WIN eWindow);
XC_EX_RETURN_VALUE MApi_XC_DIP_EX_EnaInterlaceWrite(MS_BOOL bEnable, XC_DIP_EX_WIN eWindow);
XC_EX_RETURN_VALUE MApi_XC_DIP_EX_SetOutputDataFmt(XC_DIP_EX_DATA_FMT fmt, XC_DIP_EX_WIN eWindow);
XC_EX_RETURN_VALUE MApi_XC_DIP_EX_SetDIPWinProperty(XC_DIP_EX_WINPROPERTY *pstDIPWinProperty, XC_DIP_EX_WIN eWindow);
XC_EX_RETURN_VALUE MApi_XC_DIP_EX_EnableY2R(MS_BOOL bEnable, XC_DIP_EX_WIN eWindow);
XC_EX_RETURN_VALUE MApi_XC_DIP_EX_SetAlpha(MS_U8 u8AlphaVal, XC_DIP_EX_WIN eWindow);
XC_EX_RETURN_VALUE MApi_XC_DIP_EX_SwapUV(MS_BOOL bEnable, XC_DIP_EX_WIN eWindow);
XC_EX_RETURN_VALUE MApi_XC_DIP_EX_SwapYC(MS_BOOL bEnable, XC_DIP_EX_WIN eWindow);
XC_EX_RETURN_VALUE MApi_XC_DIP_EX_SwapRGB(MS_BOOL bEnable, XC_DIP_EX_RGB_SWAP_TYPE enSwapType, XC_DIP_EX_WIN eWindow);
XC_EX_RETURN_VALUE MApi_XC_DIP_EX_SetWinProperty_Ex(MS_U8 u8BufCnt ,MS_U16 u16Width, MS_U16 u16Height, MS_PHY u32BufStart ,MS_PHY u32BufEnd, XC_DIP_EX_SOURCE_TYPE eSource,XC_DIP_EX_WIN eWindow);
XC_EX_RETURN_VALUE MApi_XC_DIP_EX_SetOutputCapture(MS_BOOL bEnable, XC_DIP_EX_OP_CAPTURE eOpCapture, XC_DIP_EX_WIN eWindow);
XC_EX_RETURN_VALUE MApi_XC_DIP_EX_SetMirror(MS_BOOL bHMirror, MS_BOOL bVMirror, XC_DIP_EX_WIN eWindow);
XC_EX_RETURN_VALUE MApi_XC_DIP_EX_SetDIPRProperty(XC_DIP_EX_DIPR_PROPERTY *pstDIPRProperty, XC_DIP_EX_WIN eWindow);
XC_EX_RETURN_VALUE MApi_XC_DIP_EX_InterruptAttach(InterruptCb pIntCb,XC_DIP_EX_WIN eWindow);
XC_EX_RETURN_VALUE MApi_XC_DIP_EX_InterruptDetach(XC_DIP_EX_WIN eWindow);
//==============Get===================
MS_U16 MApi_XC_DIP_EX_GetIntStatus(XC_DIP_EX_WIN eWindow);
XC_DIP_EX_BUFFER_INFO MApi_XC_DIP_EX_GetBufInfo(XC_DIP_EX_WIN eWindow);

#undef INTERFACE

#ifdef __cplusplus
}
#endif
#endif /* APIXC_DWIN_H */

