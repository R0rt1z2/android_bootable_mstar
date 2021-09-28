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
///
/// @brief  Video Encoder API
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __APIVENC_H__
#define __APIVENC_H__

#ifdef __cplusplus
extern "C"
{
#endif


#define VENC_HANDLE MS_U32

typedef enum
{
    E_VENC_SUCCESS = 0,
    E_VENC_FAIL,
    E_VENC_INVALID_PARAM,
    E_VENC_INVALID_HANDLE,
    E_VENC_INVALID_STATE,
    E_VENC_UNSUPPORTED,
} VENC_RESULT;

typedef enum
{
    E_VENC_STATE_CLOSED = 0,
    E_VENC_STATE_IDLE,
    E_VENC_STATE_BUSY,
    E_VENC_STATE_UNKNOWN = 0xFF,
} VENC_STATE;

typedef enum
{
    E_VENC_CODEC_AVC = 0,
    E_VENC_CODEC_HEVC,
} VENC_CODEC;

typedef enum
{
    E_VENC_RATE_CONTROL_CQ = 0,
    E_VENC_RATE_CONTROL_CBR,
    E_VENC_RATE_CONTROL_CVBR,
    E_VENC_RATE_CONTROL_VBR,
} VENC_RATE_CONTROL;

typedef enum
{
    E_VENC_SCAN_TYPE_PROGRESSIVE = 0,
    E_VENC_SCAN_TYPE_INTERLACED,
} VENC_SCAN_TYPE;

typedef enum
{
    E_VENC_INPUT_FORMAT_YUVTILE = 0,  // OBSOLETE
    E_VENC_INPUT_FORMAT_MSTTILE,
    E_VENC_INPUT_FORMAT_YUYV,
    E_VENC_INPUT_FORMAT_YVYU,
    E_VENC_INPUT_FORMAT_UYVY,
    E_VENC_INPUT_FORMAT_VYUY,
    E_VENC_INPUT_FORMAT_EVDTILE,
    E_VENC_INPUT_FORMAT_32x32TILE,
    E_VENC_INPUT_FORMAT_NV12,
    E_VENC_INPUT_FORMAT_NV21,
} VENC_INPUT_FORMAT;

typedef enum
{
	E_VENC_PROFILE_BASELINE = 0,  // AVC only
    E_VENC_PROFILE_MAIN,
} VENC_PROFILE;

typedef enum
{
    E_VENC_LEVEL_1 = 0,
    E_VENC_LEVEL_1b,     // AVC only
    E_VENC_LEVEL_1_1,    // AVC only
    E_VENC_LEVEL_1_2,    // AVC only
    E_VENC_LEVEL_1_3,    // AVC only
    E_VENC_LEVEL_2,
    E_VENC_LEVEL_2_1,
    E_VENC_LEVEL_2_2,    // AVC only
    E_VENC_LEVEL_3,
    E_VENC_LEVEL_3_1,
    E_VENC_LEVEL_3_2,    // AVC only
    E_VENC_LEVEL_4,
    E_VENC_LEVEL_4_1,
    E_VENC_LEVEL_4_2,    // AVC only
    E_VENC_LEVEL_5,
    E_VENC_LEVEL_5_1,
    E_VENC_LEVEL_5_2,
    E_VENC_LEVEL_6,
    E_VENC_LEVEL_6_1,
    E_VENC_LEVEL_6_2,
} VENC_LEVEL;

typedef enum
{
    E_VENC_FRAME_TYPE_I = 0,
    E_VENC_FRAME_TYPE_P,
    E_VENC_FRAME_TYPE_B,
} VENC_FRAME_TYPE;

typedef enum
{
    E_VENC_CONFIG_CODEC = 0,            // VENC_CODEC
    E_VENC_CONFIG_DIMENSION,            // VENC_Dimension
    E_VENC_CONFIG_HW_BUFFER,            // VENC_Buffer
    E_VENC_CONFIG_OUTPUT_BUFFER,        // VENC_Buffer
    E_VENC_CONFIG_PROFILE,              // VENC_PROFILE
    E_VENC_CONFIG_LEVEL,                // VENC_LEVEL
    E_VENC_CONFIG_FRAME_RATE_X_100,     // MS_U32
    E_VENC_CONFIG_INTRA_PERIOD,         // MS_S32
    E_VENC_CONFIG_RATE_CONTROL,         // VENC_RATE_CONTROL
    E_VENC_CONFIG_CEILING_BIT_RATE,     // MS_U32
    E_VENC_CONFIG_AVERAGE_BIT_RATE,     // MS_U32
    E_VENC_CONFIG_SCAN_TYPE,            // VENC_SCAN_TYPE
    E_VENC_CONFIG_CABAC,                // MS_BOOL
    E_VENC_CONFIG_ISR,                  // MS_BOOL
    E_VENC_CONFIG_INPUT_FORMAT,         // VENC_INPUT_FORMAT
    E_VENC_CONFIG_CROP_INFO,            // VENC_CropInfo
    E_VENC_CONFIG_SAMPLE_ASPECT_RATIO,  // VENC_SampleAspectRatio
    E_VENC_CONFIG_STREAM_CONTEXT,       // MS_U64
    E_VENC_CONFIG_FRAME_DONE_CALLBACK,  // void *
    E_VENC_CONFIG_REDUCE_BW_MODE,       // MS_BOOL
    E_VENC_CONFIG_FAST_MODE,            // MS_BOOL
    E_VENC_CONFIG_DIP_MFE_HANDSHAKE,    // MS_BOOL
    E_VENC_CONFIG_GN_DBF_IMI,           // MS_BOOL
} VENC_CONFIG;


typedef struct DLL_PACKED
{
    MS_U32 u32Version;
    MS_S32 u32Size;
} VENC_VerCtl;

typedef struct DLL_PACKED
{
    VENC_VerCtl verCtrl;
    MS_U32 u32Width;
    MS_U32 u32Height;
} VENC_Dimension;

typedef struct DLL_PACKED
{
    VENC_VerCtl verCtrl;
    MS_BOOL bCropEnable;
    MS_U32 u32CropTop;
    MS_U32 u32CropBottom;
    MS_U32 u32CropLeft;
    MS_U32 u32CropRight;
} VENC_CropInfo;

typedef struct DLL_PACKED
{
    VENC_VerCtl verCtrl;
    MS_BOOL bSarInfoPresent;
    MS_U32 u32SarInfoIdc;
    MS_U32 u32SarWidth;
    MS_U32 u32SarHeight;
} VENC_SampleAspectRatio;

typedef struct DLL_PACKED
{
    VENC_VerCtl verCtrl;
    MS_U8 u8HwBufMiu;
    MS_PHY phyHwBufAddr;
    MS_U32 u32HwBufSize;
    MS_BOOL bHwBufCached;
} VENC_HwBuf;

typedef struct DLL_PACKED
{
    VENC_VerCtl verCtrl;
    MS_U64 u64OutBufAddr;
    MS_U32 u32OutBufSize;
} VENC_OutputBuf;


typedef void (*VENCFrameDoneCallback)(VENC_HANDLE handle,
                                      MS_U64 u64OutBuf,
                                      MS_S32 s32Len,
                                      MS_S32 frameType,
                                      MS_U64 u64StreamCtx,
                                      MS_U64 u64FrameCtx);


VENC_RESULT MApi_VENC_CreateTask(VENC_HANDLE *pHandle);

VENC_RESULT MApi_VENC_StartTask(VENC_HANDLE handle);

VENC_RESULT MApi_VENC_CloseTask(VENC_HANDLE handle);


VENC_STATE MApi_VENC_GetState(void);


VENC_RESULT MApi_VENC_SetConfig(VENC_HANDLE handle,
                                VENC_CONFIG config,
                                void *pParam,
                                MS_S32 s32ParamSize);


VENC_RESULT MApi_VENC_ReConfig(VENC_HANDLE handle,
                               VENC_CONFIG config,
                               void *pParam,
                               MS_S32 s32ParamSize);


VENC_RESULT MApi_VENC_GetConfig(VENC_HANDLE handle,
                                VENC_CONFIG config,
                                void *pParam,
                                MS_S32 s32ParamSize);


VENC_RESULT MApi_VENC_GetVpsSpsPps(VENC_HANDLE handle,
                                   MS_S32 *ps32VpsLen,
                                   MS_S32 *ps32SpsLen,
                                   MS_S32 *ps32PpsLen);


VENC_RESULT MApi_VENC_CompressOnePicture(VENC_HANDLE handle,
                                         MS_U8 u8InputMiu,
                                         MS_PHY phyLumaAddr,
                                         MS_PHY phyChromaAddr,
                                         MS_U32 u32Pitch,
                                         MS_BOOL bForceI,
                                         void *pFrameCtx);


VENC_RESULT MApi_VENC_GetPictureES(VENC_HANDLE handle,
                                   MS_U64 *pu64OutBuf,
                                   MS_S32 *ps32Len,
                                   MS_S32 *ps32FrameType,
                                   MS_U64 *pu64StreamCtx,
                                   MS_U64 *pu64FrameCtx);

#ifdef __cplusplus
}
#endif

#endif // __APIVENC_H__
