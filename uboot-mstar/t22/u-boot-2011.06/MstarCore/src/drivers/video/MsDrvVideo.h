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


#ifndef _MS_DRV_VIDEO_
#define _MS_DRV_VIDEO_

/// Stream type
typedef enum
{
    ///Live stream
    E_MSDRV_VIDEO_STREAM_LIVE = 0,
    ///TS file
    E_MSDRV_VIDEO_STREAM_TSFILE,
    ///file
    E_MSDRV_VIDEO_STREAM_FILE,
    ///Stream type number
    E_MSDRV_VIDEO_STREAM_MAX
} MsDrv_VIDEO_StreamType_e;

typedef enum
{
    ///unsupported codec type
    E_MSDRV_VIDEO_CODEC_TYPE_NONE = 0,
    ///MPEG 1/2
    E_MSDRV_VIDEO_CODEC_TYPE_MPEG2,
    ///H263 (short video header)
    E_MSDRV_VIDEO_CODEC_TYPE_H263,
    ///MPEG4 (default)
    E_MSDRV_VIDEO_CODEC_TYPE_MPEG4,
    ///MPEG4 (Divx311)
    E_MSDRV_VIDEO_CODEC_TYPE_DIVX311,
    ///MPEG4 (Divx412)
    E_MSDRV_VIDEO_CODEC_TYPE_DIVX412,
    ///FLV
    E_MSDRV_VIDEO_CODEC_TYPE_FLV,
    ///VC1 advanced profile (VC1)
    E_MSDRV_VIDEO_CODEC_TYPE_VC1_ADV,
    ///VC1 main profile (RCV)
    E_MSDRV_VIDEO_CODEC_TYPE_VC1_MAIN,
    ///Real Video version 8
    E_MSDRV_VIDEO_CODEC_TYPE_RV8,
    ///Real Video version 9 and 10
    E_MSDRV_VIDEO_CODEC_TYPE_RV9,
    ///H264
    E_MSDRV_VIDEO_CODEC_TYPE_H264,
    ///AVS
    E_MSDRV_VIDEO_CODEC_TYPE_AVS,
    ///MJPEG
    E_MSDRV_VIDEO_CODEC_TYPE_MJPEG,
    ///MVC
    E_MSDRV_VIDEO_CODEC_TYPE_MVC,
    ///VP8
    E_MSDRV_VIDEO_CODEC_TYPE_VP8,
    ///HEVC
    E_MSDRV_VIDEO_CODEC_TYPE_HEVC,
    E_MSDRV_VIDEO_CODEC_TYPE_MAX
} MsDrv_VIDEO_CodecType_e;

int MsDrv_VideoStart(MsDrv_VIDEO_StreamType_e eStreamType, MsDrv_VIDEO_CodecType_e eCodecType);
int MsDrv_IFrameStart(MsDrv_VIDEO_StreamType_e eStreamType, MsDrv_VIDEO_CodecType_e eCodecType, char * FileName);
MS_BOOL MsDrv_VideoSetXcMvopCfg(void);
int MsDrv_VideoStop(void);
MS_U32 MsDrv_VideoShowDecInfo(void);
void MsDrv_VideoRoutinePolling(void);
MS_U32 MsDrv_VideoGetDecFrmCnt(void);
MS_PHYADDR MsDrv_VideoGetPtsAddr(MsDrv_VIDEO_CodecType_e eCodecType, MS_U32 *pu32PtsKhzUnit);
MS_BOOL MsDrv_VideoGetFrameCountAddr(MsDrv_VIDEO_CodecType_e eCodecType, MS_PHYADDR *pu32FrameCntAddr, MS_U32 *pu32SkipFrameCntAddr);
MS_BOOL MsDrv_VideoIsSyncOn(void);
MS_BOOL MsDrv_VideoIsReachSync(MS_BOOL bWaitSync);
void MsDrv_VideoUnMute(void);

MS_U32 MsDrv_VideoGetCurPts(void);

MS_BOOL MsDrv_VideoPause(void);
MS_BOOL MsDrv_VideoResume(void);
MS_U32 MsDrv_VideoGetDecFrmCnt(void);
MS_U32 MsDrv_VideoGetDispCnt(void);


#endif  // #ifndef _MS_DRV_VIDEO_

