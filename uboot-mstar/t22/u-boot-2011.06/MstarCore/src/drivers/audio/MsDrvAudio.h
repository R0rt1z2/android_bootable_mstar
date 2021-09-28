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


#ifndef _MS_DRV_AUDIO_
#define _MS_DRV_AUDIO_

//------------------------------TV_SYSTEM---------------------------------------
#define TV_NTSC                 0
#define TV_PAL                  1
#define TV_CHINA                2

//------------------------------AUDIO_SYSTEM_SEL--------------------------------
#define AUDIO_SYSTEM_BTSC                       0
#define AUDIO_SYSTEM_A2         1
#define AUDIO_SYSTEM_EIAJ      

/// Stream type
typedef enum
{
    ///Live stream
    E_MSDRV_AUDIO_STREAM_LIVE = 0,
    ///TS file
    E_MSDRV_AUDIO_STREAM_TSFILE,

    ///Stream type number
    E_MSDRV_AUDIO_STREAM_MAX
} MsDrv_AUDIO_StreamType_e;

typedef enum
{
    E_MSDRV_AUDIO_CODEC_TYPE_NONE = 0x0,
    E_MSDRV_AUDIO_CODEC_TYPE_MPEG,
    E_MSDRV_AUDIO_CODEC_TYPE_AC3,
    E_MSDRV_AUDIO_CODEC_TYPE_AC3P,
    E_MSDRV_AUDIO_CODEC_TYPE_AAC,
    E_MSDRV_AUDIO_CODEC_TYPE_MP3,
    E_MSDRV_AUDIO_CODEC_TYPE_WMA,
    E_MSDRV_AUDIO_CODEC_TYPE_CDLPCM,
    E_MSDRV_AUDIO_CODEC_TYPE_RA8LBR,
    E_MSDRV_AUDIO_CODEC_TYPE_XPCM,
    E_MSDRV_AUDIO_CODEC_TYPE_DTS,
    E_MSDRV_AUDIO_CODEC_TYPE_WMA_PRO,
    E_MSDRV_AUDIO_CODEC_TYPE_FLAC,
    E_MSDRV_AUDIO_CODEC_TYPE_VORBIS,
    E_MSDRV_AUDIO_CODEC_TYPE_DTSLBR,
    E_MSDRV_AUDIO_CODEC_TYPE_AMR_NB,
    E_MSDRV_AUDIO_CODEC_TYPE_AMR_WB,
    E_MSDRV_AUDIO_CODEC_TYPE_DRA,
    E_MSDRV_AUDIO_CODEC_TYPE_MAX,
} MsDrv_AUDIO_CodecType_e;

void MsDrv_PlayMelody(void);
int MsDrv_AudioPreInit(void);
MS_U32 MsDrv_GetEsStartAddr(void);

int MsDrv_AudioStart(MsDrv_AUDIO_StreamType_e eStreamType, MsDrv_AUDIO_CodecType_e eCodecType);
int MsDrv_AudioStop(void);
void MsDrv_AudioUnMute(MS_U8 u8Volume);
void MsDrv_AudioMute(void);
void MsDrv_AudioShowDecInfo(void);
#endif
