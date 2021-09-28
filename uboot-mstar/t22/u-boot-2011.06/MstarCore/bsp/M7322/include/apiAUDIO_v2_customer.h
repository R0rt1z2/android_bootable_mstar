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
/// @brief  AUDIO API
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _API_AUDIO_CUSTOMER_H_
#define _API_AUDIO_CUSTOMER_H_

#include "MsCommon.h"


#ifdef __cplusplus
extern "C"
{
#endif

//-------------------------------------------------------------------------------------------------------------------------------------
/* Initialize, STR */
//-------------------------------------------------------------------------------------------------------------------------------------
#if defined (__aarch64__)
typedef MS_U64 AU_PHY;
#else
typedef MS_U32 AU_PHY;
#endif //#if defined (__aarch64__)

//need to be consistent with AUDIO_OUTPUT_TYPE of drvAudio_if.h
typedef enum
{
    API_AUDIO_CUSTOMER_OUTPUT_TYPE_INVALID = -1,
    API_AUDIO_CUSTOMER_LINEOUT0_OUTPUT,              ///< 0: DAC0 OUTPUT
    API_AUDIO_CUSTOMER_LINEOUT1_OUTPUT,              ///< 1: T2=>AA0 OUTPUT    T3=>HP OUTPUT
    API_AUDIO_CUSTOMER_LINEOUT2_OUTPUT,              ///< 2: T2=>AA1 OUTPUT    T3=>DAC2 OUTPUT
    API_AUDIO_CUSTOMER_LINEOUT3_OUTPUT,              ///< 3: T2=>NULL          T3=>AA OUTPUT
    API_AUDIO_CUSTOMER_SPDIF_OUTPUT,                 ///< 4: SPDIF OUTPUT
    API_AUDIO_CUSTOMER_I2S_OUTPUT,                   ///< 5: I2S OUTPUT
    API_AUDIO_CUSTOMER_HP_OUTPUT,                    ///< 6: OUTPUT with headphone driver
    API_AUDIO_CUSTOMER_I2S2_OUTPUT,                  ///< 7: I2S2 OUTPUT
    API_AUDIO_CUSTOMER_HDMI_ARC_OUTPUT,              ///< 8: HDMI ARC OUTPUT(HDMI_Rx,TV)
    API_AUDIO_CUSTOMER_HDMI_OUTPUT,                  ///< 9: HDMI Tx OUTPUT(HDMI_Tx,STB)
    API_AUDIO_CUSTOMER_NULL_OUTPUT = 0xFF,           ///< 0xFF: Null output
    API_AUDIO_CUSTOMER_OUTPUT_MAX,
} API_AUDIO_CUSTOMER_OUTPUT_TYPE;

//need to be consistent with AUDIO_OUT_INFO of drvAudio_if.h
typedef struct
{
    API_AUDIO_CUSTOMER_OUTPUT_TYPE   SpeakerOut;  ///< Audio output port for Speaker
    API_AUDIO_CUSTOMER_OUTPUT_TYPE   HpOut;       ///< Audio output port for HP
    API_AUDIO_CUSTOMER_OUTPUT_TYPE   MonitorOut;  ///< Audio output port for Monitor out
    API_AUDIO_CUSTOMER_OUTPUT_TYPE   ScartOut;    ///< Audio output port for Scart out
    API_AUDIO_CUSTOMER_OUTPUT_TYPE   SpdifOut;    ///< Audio output port for S/PDIF out
    API_AUDIO_CUSTOMER_OUTPUT_TYPE   ArcOut;      ///< Audio output port for ARC(HDMI_Rx) out
    API_AUDIO_CUSTOMER_OUTPUT_TYPE   HDMIOut;     ///< Audio output port for HDMI_Tx out
} API_AUDIO_CUSTOMER_OUTPUT_INFO;

typedef enum
{
    API_AUDIO_CUSTOMER_CHIP_PLATFORM_INVALID = -1,
    API_AUDIO_CUSTOMER_CHIP_PLATFORM_TV,
    API_AUDIO_CUSTOMER_CHIP_PLATFORM_STB,
} API_AUDIO_CUSTOMER_CHIP_PLATFORM;

//-------------------------------------------------------------------------------------------------------------------------------------
/* Connect & Disconnect */
//-------------------------------------------------------------------------------------------------------------------------------------
typedef enum
{
    API_AUDIO_CUSTOMER_PARSER_MODE_INVALID = -1,
    API_AUDIO_CUSTOMER_PARSER_MODE_0,
    API_AUDIO_CUSTOMER_PARSER_MODE_1,
    API_AUDIO_CUSTOMER_PARSER_MODE_2,
    API_AUDIO_CUSTOMER_PARSER_MODE_3,
    API_AUDIO_CUSTOMER_PARSER_MODE_MAX,
} API_AUDIO_CUSTOMER_PARSER_MODE;

typedef enum
{
    API_AUDIO_CUSTOMER_PARSER_INPUT_INVALID = -1,
    API_AUDIO_CUSTOMER_PARSER_INPUT_AFIFO0,
    API_AUDIO_CUSTOMER_PARSER_INPUT_AFIFO1,
    API_AUDIO_CUSTOMER_PARSER_INPUT_AFIFO2,
    API_AUDIO_CUSTOMER_PARSER_INPUT_AFIFO3,
    API_AUDIO_CUSTOMER_PARSER_INPUT_MAX,
} API_AUDIO_CUSTOMER_PARSER_INPUT;

typedef enum
{
    API_AUDIO_CUSTOMER_ADEC_INDEX_INVALID   = -1,
    API_AUDIO_CUSTOMER_ADEC0                = 0,
    API_AUDIO_CUSTOMER_ADEC1                = 1,
    API_AUDIO_CUSTOMER_ADEC2                = 3,
    API_AUDIO_CUSTOMER_ADEC3                = 4, //<--MAX is here!!!
    API_AUDIO_CUSTOMER_ADEC_ATV             = 2,
    API_AUDIO_CUSTOMER_ADEC_MAX             = API_AUDIO_CUSTOMER_ADEC3 + 1,
} API_AUDIO_CUSTOMER_ADEC_INDEX;

typedef enum
{
    API_AUDIO_CUSTOMER_ADEC_INPUT_INVALID   = -1,
    API_AUDIO_CUSTOMER_ADEC_INPUT_DTV       = 0,
    API_AUDIO_CUSTOMER_ADEC_INPUT_HDMI      = 1,
    API_AUDIO_CUSTOMER_ADEC_INPUT_SPDIF     = 7,
    API_AUDIO_CUSTOMER_ADEC_INPUT_MM        = 2,
    API_AUDIO_CUSTOMER_ADEC_INPUT_MM_UNI    = 6,
    API_AUDIO_CUSTOMER_ADEC_INPUT_MM_TS     = 8, //<--MAX is here!!!
    API_AUDIO_CUSTOMER_ADEC_INPUT_CLIP      = 3,
    API_AUDIO_CUSTOMER_ADEC_INPUT_ATV       = 4,
    API_AUDIO_CUSTOMER_ADEC_INPUT_NONE      = 5,
    API_AUDIO_CUSTOMER_ADEC_INPUT_MAX       = API_AUDIO_CUSTOMER_ADEC_INPUT_MM_TS + 1,
} API_AUDIO_CUSTOMER_ADEC_INPUT;

typedef enum
{
    API_AUDIO_CUSTOMER_ADEC_PCM_PATH_INVALID = -1,
    API_AUDIO_CUSTOMER_ADEC_PCM_PATH_DSP_SRC,
    API_AUDIO_CUSTOMER_ADEC_PCM_PATH_MCU,
    API_AUDIO_CUSTOMER_ADEC_PCM_PATH_MAX,
} API_AUDIO_CUSTOMER_ADEC_PCM_PATH;

typedef enum
{
    API_AUDIO_CUSTOMER_ADC_INDEX_INVALID   = -1,
    API_AUDIO_CUSTOMER_ADC0,
    API_AUDIO_CUSTOMER_ADC1,
    API_AUDIO_CUSTOMER_ADC_MAX,
} API_AUDIO_CUSTOMER_ADC_INDEX;

typedef enum
{
    API_AUDIO_CUSTOMER_ADC_IN_PORT_INVALID  = -1,
    API_AUDIO_CUSTOMER_ADC0_IN_PORT_0       = 0x02,
    API_AUDIO_CUSTOMER_ADC0_IN_PORT_1       = 0x12,
    API_AUDIO_CUSTOMER_ADC0_IN_PORT_2       = 0x22,
    API_AUDIO_CUSTOMER_ADC0_IN_PORT_3       = 0x32,
    API_AUDIO_CUSTOMER_ADC0_IN_PORT_4       = 0xA2,
    API_AUDIO_CUSTOMER_ADC0_IN_PORT_5       = 0xB2,
    API_AUDIO_CUSTOMER_ADC0_MIC_IN          = 0x72,
    API_AUDIO_CUSTOMER_ADC1_IN_PORT_0       = 0x09,
    API_AUDIO_CUSTOMER_ADC1_IN_PORT_1       = 0x19,
    API_AUDIO_CUSTOMER_ADC1_IN_PORT_2       = 0x29,
    API_AUDIO_CUSTOMER_ADC1_IN_PORT_3       = 0x39,
    API_AUDIO_CUSTOMER_ADC1_IN_PORT_4       = 0xA9,
    API_AUDIO_CUSTOMER_ADC1_IN_PORT_5       = 0xB9,
    API_AUDIO_CUSTOMER_ADC1_MIC_IN          = 0x79,
} API_AUDIO_CUSTOMER_ADC_IN_PORT;

typedef enum
{
    API_AUDIO_CUSTOMER_PCM_MIXER_INDEX_INVALID = -1,
    API_AUDIO_CUSTOMER_PCM_MIXER0,
    API_AUDIO_CUSTOMER_PCM_MIXER_MAX,
} API_AUDIO_CUSTOMER_PCM_MIXER_INDEX;

typedef enum
{
    API_AUDIO_CUSTOMER_PCM_MIXER_INPUT_INVALID = -1,
    API_AUDIO_CUSTOMER_PCM_MIXER_INPUT_AMIX0,
    API_AUDIO_CUSTOMER_PCM_MIXER_INPUT_AMIX1,
    API_AUDIO_CUSTOMER_PCM_MIXER_INPUT_AMIX2,
    API_AUDIO_CUSTOMER_PCM_MIXER_INPUT_AMIX3,
    API_AUDIO_CUSTOMER_PCM_MIXER_INPUT_AMIX4,
    API_AUDIO_CUSTOMER_PCM_MIXER_INPUT_AMIX5,
    API_AUDIO_CUSTOMER_PCM_MIXER_INPUT_AMIX6,
    API_AUDIO_CUSTOMER_PCM_MIXER_INPUT_AMIX7,
    API_AUDIO_CUSTOMER_PCM_MIXER_INPUT_MAX,
} API_AUDIO_CUSTOMER_PCM_MIXER_INPUT;

typedef enum
{
    API_AUDIO_CUSTOMER_CH_SOUND_INVALID = -1,
    API_AUDIO_CUSTOMER_CH1_SOUND        = 4,
    API_AUDIO_CUSTOMER_CH2_SOUND        = 5,
    API_AUDIO_CUSTOMER_CH3_SOUND        = 6,
    API_AUDIO_CUSTOMER_CH4_SOUND        = 7, //<--MAX is here!!!
    API_AUDIO_CUSTOMER_CH5_SOUND        = 0,
    API_AUDIO_CUSTOMER_CH6_SOUND        = 1,
    API_AUDIO_CUSTOMER_CH7_SOUND        = 2,
    API_AUDIO_CUSTOMER_CH8_SOUND        = 3,
    API_AUDIO_CUSTOMER_CH_SOUND_MAX     = API_AUDIO_CUSTOMER_CH4_SOUND + 1,
} API_AUDIO_CUSTOMER_CH_SOUND;

typedef enum
{
    API_AUDIO_CUSTOMER_CH_INPUT_INVALID     = -1,
    API_AUDIO_CUSTOMER_CH_INPUT_ADEC0       = 0,
    API_AUDIO_CUSTOMER_CH_INPUT_ADEC1       = 1,
    API_AUDIO_CUSTOMER_CH_INPUT_ADEC2       = 12,
    API_AUDIO_CUSTOMER_CH_INPUT_ADEC3       = 13, //<--MAX is here!!!
    API_AUDIO_CUSTOMER_CH_INPUT_ADEC_ATV    = 2,
    API_AUDIO_CUSTOMER_CH_INPUT_HDMI        = 3,
    API_AUDIO_CUSTOMER_CH_INPUT_ADC0        = 4,
    API_AUDIO_CUSTOMER_CH_INPUT_ADC1        = 5,
    API_AUDIO_CUSTOMER_CH_INPUT_SPDIF       = 6,
    API_AUDIO_CUSTOMER_CH_INPUT_R2DMA1      = 7,
    API_AUDIO_CUSTOMER_CH_INPUT_R2DMA2      = 8,
    API_AUDIO_CUSTOMER_CH_INPUT_SWDMA1      = 9,
    API_AUDIO_CUSTOMER_CH_INPUT_HWDMA1      = 10,
    API_AUDIO_CUSTOMER_CH_INPUT_NONE        = 11,
    API_AUDIO_CUSTOMER_CH_INPUT_MAX         = API_AUDIO_CUSTOMER_CH_INPUT_ADEC3 + 1,
} API_AUDIO_CUSTOMER_CH_INPUT;

typedef  enum
{
    API_AUDIO_CUSTOMER_FWM_INVALID = -1,
    API_AUDIO_CUSTOMER_FWM0,
    API_AUDIO_CUSTOMER_FWM1,
    API_AUDIO_CUSTOMER_FWM2,
    API_AUDIO_CUSTOMER_FWM_MAX,
} API_AUDIO_CUSTOMER_FWM_INDEX;

typedef  enum
{
    API_AUDIO_CUSTOMER_FWM_INPUT_INVALID = -1,
    API_AUDIO_CUSTOMER_FWM_INPUT_CH1     = 4,
    API_AUDIO_CUSTOMER_FWM_INPUT_CH2     = 5,
    API_AUDIO_CUSTOMER_FWM_INPUT_CH3     = 6,
    API_AUDIO_CUSTOMER_FWM_INPUT_CH4     = 7, //<--MAX is here!!!
    API_AUDIO_CUSTOMER_FWM_INPUT_CH5     = 0,
    API_AUDIO_CUSTOMER_FWM_INPUT_CH6     = 1,
    API_AUDIO_CUSTOMER_FWM_INPUT_CH7     = 2,
    API_AUDIO_CUSTOMER_FWM_INPUT_CH8     = 3,
    API_AUDIO_CUSTOMER_FWM_INPUT_MAX     = API_AUDIO_CUSTOMER_FWM_INPUT_CH4 + 1,
} API_AUDIO_CUSTOMER_FWM_INPUT;

typedef  enum
{
    API_AUDIO_CUSTOMER_SE_INVALID = -1,
    API_AUDIO_CUSTOMER_SE_PRER2,
    API_AUDIO_CUSTOMER_SE_DSPSE,
    API_AUDIO_CUSTOMER_SE_POSTR2,
    API_AUDIO_CUSTOMER_SE_MAX,
} API_AUDIO_CUSTOMER_SE_INDEX;

typedef  enum
{
    API_AUDIO_CUSTOMER_SE_INPUT_INVALID = -1,
    API_AUDIO_CUSTOMER_SE_INPUT_DLY0,
    API_AUDIO_CUSTOMER_SE_INPUT_DLY1,
    API_AUDIO_CUSTOMER_SE_INPUT_MAX,
} API_AUDIO_CUSTOMER_SE_INPUT;

typedef enum
{
    API_AUDIO_CUSTOMER_SOUNDOUT_INVALID     = -1,
    API_AUDIO_CUSTOMER_SOUNDOUT_I2S         = 0, //SD0
    API_AUDIO_CUSTOMER_SOUNDOUT_I2S2        = 8, //SD1 //<--MAX is here!!!
    API_AUDIO_CUSTOMER_SOUNDOUT_LINEOUT0    = 1,
    API_AUDIO_CUSTOMER_SOUNDOUT_LINEOUT1    = 2,
    API_AUDIO_CUSTOMER_SOUNDOUT_LINEOUT2    = 3,
    API_AUDIO_CUSTOMER_SOUNDOUT_LINEOUT3    = 4,
    API_AUDIO_CUSTOMER_SOUNDOUT_SPDIF       = 5,
    API_AUDIO_CUSTOMER_SOUNDOUT_HDMI_ARC    = 6,
    API_AUDIO_CUSTOMER_SOUNDOUT_HDMI_TX     = 7,
    API_AUDIO_CUSTOMER_SOUNDOUT_MAX         = API_AUDIO_CUSTOMER_SOUNDOUT_I2S2 + 1,
} API_AUDIO_CUSTOMER_SOUNDOUT_INDEX;

typedef enum
{
    API_AUDIO_CUSTOMER_SOUNDOUT_INPUT_INVALID        = -1,
    API_AUDIO_CUSTOMER_SOUNDOUT_INPUT_NULL           = 0,
    API_AUDIO_CUSTOMER_SOUNDOUT_INPUT_CH7            = 1,
    API_AUDIO_CUSTOMER_SOUNDOUT_INPUT_MCH_SWDMA_LR   = 12,
    API_AUDIO_CUSTOMER_SOUNDOUT_INPUT_MCH_SWDMA_LsRs = 13, //<--MAX is here!!!
    API_AUDIO_CUSTOMER_SOUNDOUT_INPUT_FWM0           = 2,
    API_AUDIO_CUSTOMER_SOUNDOUT_INPUT_DLY0           = 3,
    API_AUDIO_CUSTOMER_SOUNDOUT_INPUT_FWM1           = 4,
    API_AUDIO_CUSTOMER_SOUNDOUT_INPUT_DLY1           = 5,
    API_AUDIO_CUSTOMER_SOUNDOUT_INPUT_FWM2           = 6,
    API_AUDIO_CUSTOMER_SOUNDOUT_INPUT_DLY2           = 7,
    API_AUDIO_CUSTOMER_SOUNDOUT_INPUT_AUDIO_DELAY    = 11,
    API_AUDIO_CUSTOMER_SOUNDOUT_INPUT_PRER2          = 8,
    API_AUDIO_CUSTOMER_SOUNDOUT_INPUT_DSPSE          = 9,
    API_AUDIO_CUSTOMER_SOUNDOUT_INPUT_POSTR2         = 10,
    API_AUDIO_CUSTOMER_SOUNDOUT_INPUT_MAX            = API_AUDIO_CUSTOMER_SOUNDOUT_INPUT_MCH_SWDMA_LsRs + 1,
} API_AUDIO_CUSTOMER_SOUNDOUT_INPUT;

typedef enum
{
    API_AUDIO_CUSTOMER_PCM_CAPTURE_INVALID = -1,
    API_AUDIO_CUSTOMER_PCM_CAPTURE0,
    API_AUDIO_CUSTOMER_PCM_CAPTURE1,
    API_AUDIO_CUSTOMER_PCM_CAPTURE2,
    API_AUDIO_CUSTOMER_PCM_CAPTURE_MAX,
} API_AUDIO_CUSTOMER_PCM_CAPTURE_INDEX;

typedef enum
{
    API_AUDIO_CUSTOMER_PCM_CAPTURE_INPUT_INVALID     = -1,
    API_AUDIO_CUSTOMER_PCM_CAPTURE_INPUT_CH1         = 11,
    API_AUDIO_CUSTOMER_PCM_CAPTURE_INPUT_CH2         = 12,
    API_AUDIO_CUSTOMER_PCM_CAPTURE_INPUT_CH3         = 13,
    API_AUDIO_CUSTOMER_PCM_CAPTURE_INPUT_CH4         = 14,
    API_AUDIO_CUSTOMER_PCM_CAPTURE_INPUT_CH5         = 0,
    API_AUDIO_CUSTOMER_PCM_CAPTURE_INPUT_CH6         = 1,
    API_AUDIO_CUSTOMER_PCM_CAPTURE_INPUT_CH7         = 2,
    API_AUDIO_CUSTOMER_PCM_CAPTURE_INPUT_CH8         = 3,
    API_AUDIO_CUSTOMER_PCM_CAPTURE_INPUT_PCM         = 4,
    API_AUDIO_CUSTOMER_PCM_CAPTURE_INPUT_PCM_DELAY   = 5,
    API_AUDIO_CUSTOMER_PCM_CAPTURE_INPUT_PCM_SE      = 6,
    API_AUDIO_CUSTOMER_PCM_CAPTURE_INPUT_MIXER       = 7,
    API_AUDIO_CUSTOMER_PCM_CAPTURE_INPUT_FWM0        = 8,
    API_AUDIO_CUSTOMER_PCM_CAPTURE_INPUT_FWM1        = 9,
    API_AUDIO_CUSTOMER_PCM_CAPTURE_INPUT_FWM2        = 10,
    API_AUDIO_CUSTOMER_PCM_CAPTURE_INPUT_I2S_RX0     = 15,
    API_AUDIO_CUSTOMER_PCM_CAPTURE_INPUT_I2S_RX1     = 16,
    API_AUDIO_CUSTOMER_PCM_CAPTURE_INPUT_I2S_RX0_RX1 = 17, //<--MAX is here!!!
    API_AUDIO_CUSTOMER_PCM_CAPTURE_INPUT_MAX         = API_AUDIO_CUSTOMER_PCM_CAPTURE_INPUT_I2S_RX0_RX1 + 1,
} API_AUDIO_CUSTOMER_PCM_CAPTURE_INPUT;

typedef enum
{
    API_AUDIO_CUSTOMER_MP3_ENC_INVALID = -1,
    API_AUDIO_CUSTOMER_MP3_ENC0,
    API_AUDIO_CUSTOMER_MP3_ENC_MAX,
} API_AUDIO_CUSTOMER_MP3_ENC_INDEX;

typedef enum
{
    API_AUDIO_CUSTOMER_MP3_ENC_INPUT_INVALID = -1,
    API_AUDIO_CUSTOMER_MP3_ENC_INPUT_CH5,
    API_AUDIO_CUSTOMER_MP3_ENC_INPUT_CH6,
    API_AUDIO_CUSTOMER_MP3_ENC_INPUT_MAX,
} API_AUDIO_CUSTOMER_MP3_ENC_INPUT;

typedef enum
{
    API_AUDIO_CUSTOMER_AAC_ENC_INVALID = -1,
    API_AUDIO_CUSTOMER_AAC_ENC0,
    API_AUDIO_CUSTOMER_AAC_ENC_MAX,
} API_AUDIO_CUSTOMER_AAC_ENC_INDEX;

typedef enum
{
    API_AUDIO_CUSTOMER_AAC_ENC_INPUT_INVALID = -1,
    API_AUDIO_CUSTOMER_AAC_ENC_INPUT_PCM_CAPTURE0,
    API_AUDIO_CUSTOMER_AAC_ENC_INPUT_PCM_CAPTURE1,
    API_AUDIO_CUSTOMER_AAC_ENC_INPUT_MAX,
} API_AUDIO_CUSTOMER_AAC_ENC_INPUT;

//-------------------------------------------------------------------------------------------------------------------------------------
/* Start & Stop */
//-------------------------------------------------------------------------------------------------------------------------------------
typedef enum
{
    API_AUDIO_CUSTOMER_CODEC_TYPE_INVALID           = -1,
    API_AUDIO_CUSTOMER_CODEC_TYPE_UNKNOWN           = 0,
    API_AUDIO_CUSTOMER_CODEC_TYPE_PCM               = 1, // for HDMI PCM
    API_AUDIO_CUSTOMER_CODEC_TYPE_AC3               = 2,
    API_AUDIO_CUSTOMER_CODEC_TYPE_EAC3              = 3,
    API_AUDIO_CUSTOMER_CODEC_TYPE_MPEG              = 4,
    API_AUDIO_CUSTOMER_CODEC_TYPE_AAC               = 5,
    API_AUDIO_CUSTOMER_CODEC_TYPE_HEAAC             = 6,
    API_AUDIO_CUSTOMER_CODEC_TYPE_DRA               = 7,
    API_AUDIO_CUSTOMER_CODEC_TYPE_MP3               = 8,
    API_AUDIO_CUSTOMER_CODEC_TYPE_DTS               = 9,
    API_AUDIO_CUSTOMER_CODEC_TYPE_SIF               = 10,
    API_AUDIO_CUSTOMER_CODEC_TYPE_SIF_BTSC          = 11,
    API_AUDIO_CUSTOMER_CODEC_TYPE_SIF_A2            = 12,
    API_AUDIO_CUSTOMER_CODEC_TYPE_DEFAULT           = 13,
    API_AUDIO_CUSTOMER_CODEC_TYPE_NONE              = 14,
    API_AUDIO_CUSTOMER_CODEC_TYPE_DTS_HD_MA         = 15,
    API_AUDIO_CUSTOMER_CODEC_TYPE_DTS_EXPRESS       = 16,
    API_AUDIO_CUSTOMER_CODEC_TYPE_DTS_CD            = 17,
    API_AUDIO_CUSTOMER_CODEC_TYPE_WMA               = 18,
    API_AUDIO_CUSTOMER_CODEC_TYPE_WMA_PRO           = 19,
    API_AUDIO_CUSTOMER_CODEC_TYPE_XPCM              = 20,
    API_AUDIO_CUSTOMER_CODEC_TYPE_RA8LBR            = 21,
    API_AUDIO_CUSTOMER_CODEC_TYPE_FLAC              = 22,
    API_AUDIO_CUSTOMER_CODEC_TYPE_VORBIS            = 23,
    API_AUDIO_CUSTOMER_CODEC_TYPE_AMR_NB            = 24,
    API_AUDIO_CUSTOMER_CODEC_TYPE_AMR_WB            = 25,
    API_AUDIO_CUSTOMER_CODEC_TYPE_DolbyTrueHDBypass = 26,
    API_AUDIO_CUSTOMER_CODEC_TYPE_DVI               = 27, // for HDMI DVI
    API_AUDIO_CUSTOMER_CODEC_TYPE_ESBypass          = 28,
    API_AUDIO_CUSTOMER_CODEC_TYPE_OPUS              = 29,
    API_AUDIO_CUSTOMER_CODEC_TYPE_MPEG_H            = 30,
    API_AUDIO_CUSTOMER_CODEC_TYPE_AC4               = 31,
    API_AUDIO_CUSTOMER_CODEC_TYPE_MAT               = 32,
} API_AUDIO_CUSTOMER_CODEC_TYPE;

typedef enum
{
    API_AUDIO_CUSTOMER_UPPER_PLAYCMD_INVALID = -1,
    API_AUDIO_CUSTOMER_UPPER_PLAY            = 0,
    API_AUDIO_CUSTOMER_UPPER_STOP            = 1,
    API_AUDIO_CUSTOMER_UPPER_PAUSE           = 2,
} API_AUDIO_CUSTOMER_UPPER_PLAYCMD;

//-------------------------------------------------------------------------------------------------------------------------------------
/* SPDIF */
//-------------------------------------------------------------------------------------------------------------------------------------
typedef enum
{
    API_AUDIO_CUSTOMER_SPDIF_TX_MODE_INVALID   = -1,
    API_AUDIO_CUSTOMER_SPDIF_TX_NONE           = 0,
    API_AUDIO_CUSTOMER_SPDIF_TX_PCM            = 1,
    API_AUDIO_CUSTOMER_SPDIF_TX_AUTO           = 2,
    API_AUDIO_CUSTOMER_SPDIF_TX_BYPASS         = 3,
    API_AUDIO_CUSTOMER_SPDIF_TX_TRANSCODE      = 4,        /* Netflix */
    API_AUDIO_CUSTOMER_SPDIF_TX_DD             = 5,
} API_AUDIO_CUSTOMER_SPDIF_TX_MODE;

typedef enum
{
    API_AUDIO_CUSTOMER_SPDIF_COPYRIGHT_INVALID  = -1,
    API_AUDIO_CUSTOMER_SPDIF_COPY_FREE          = 0,    /* cp-bit : 1, L-bit : 0 */
    API_AUDIO_CUSTOMER_SPDIF_COPY_NO_MORE       = 1,    /* cp-bit : 0, L-bit : 1 */
    API_AUDIO_CUSTOMER_SPDIF_COPY_ONCE          = 2,    /* cp-bit : 0, L-bit : 0 */
    API_AUDIO_CUSTOMER_SPDIF_COPY_NEVER         = 3,    /* cp-bit : 0, L-bit : 1 */
} API_AUDIO_CUSTOMER_SPDIF_COPYRIGHT;

//====== SPDIF OUTPUT CS TYPE =========
typedef enum
{
    API_AUDIO_CUSTOMER_SPDIF_CS_CategoryCode               = 0x0,      ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_CopyRight                  = 0x1,      ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_SourceNumber               = 0x2,      ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_ChannelNumber              = 0x3,      ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_SmpFreq                    = 0x4,      ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_ClockAcc                   = 0x5,      ///<
    API_AUDIO_CUSTOMER_SPDIF_ChannelAB                     = 0x6,      ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_CopyProtect                = 0x7,      ///<
} API_AUDIO_CUSTOMER_SPDIF_CS_TYPE;

//====== SPDIF OUTPUT CS Status =======
typedef enum
{
    API_AUDIO_CUSTOMER_SPDIF_CS_Category_BroadCast         = 0x00,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_Category_General           = 0x01,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_Category_BroadCast_JP      = 0x02,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_Category_BroadCast_EU      = 0x03,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_Category_BroadCast_USA     = 0x04,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_Category_Rev4              = 0x05,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_Category_Rev5              = 0x06,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_Category_Rev6              = 0x07,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_CopyRight_CP               = 0x08,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_CopyRight_NonCP            = 0x09,     ///<SPDIF_CS_ChannelNumber_Right
    API_AUDIO_CUSTOMER_SPDIF_CS_SourceNumber_2             = 0x0A,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_SourceNumber_5             = 0x0B,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_SourceNumber_6             = 0x0C,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_SourceNumber_Rev1          = 0x0D,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_SourceNumber_Rev2          = 0x0E,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_SourceNumber_Rev3          = 0x0F,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_ChannelNumber_A_Left       = 0x10,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_ChannelNumber_B_Right      = 0x11,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_ChannelNumber_C            = 0x12,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_ChannelNumber_D            = 0x13,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_ChannelNumber_E            = 0x14,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_ChannelNumber_F            = 0x15,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_ChannelNumber_G            = 0x16,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_ChannelNumber_H            = 0x17,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_ChannelNumber_I            = 0x18,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_ChannelNumber_J            = 0x19,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_ChannelNumber_K            = 0x1A,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_SmpFreq_192                = 0x1B,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_SmpFreq_176                = 0x1C,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_SmpFreq_128                = 0x1D,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_SmpFreq_96                 = 0x1E,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_SmpFreq_88                 = 0x1F,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_SmpFreq_64                 = 0x20,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_SmpFreq_48                 = 0x21,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_SmpFreq_44                 = 0x22,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_SmpFreq_32                 = 0x23,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_SmpFreq_24                 = 0x24,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_SmpFreq_22                 = 0x25,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_SmpFreq_16                 = 0x26,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_SmpFreq_Rev1               = 0x27,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_SmpFreq_Rev2               = 0x28,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_SmpFreq_Rev3               = 0x29,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_SmpFreq_Rev4               = 0x2A,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_SmpFreq_Rev5               = 0x2B,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_ClockAcc_Lev1              = 0x2C,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_ClockAcc_Lev2              = 0x2D,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_ClockAcc_Lev3              = 0x2E,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_ClockAcc_NotMatch          = 0x2F,     ///<
    API_AUDIO_CUSTOMER_SPDIF_ChannelAB_Invalid_ON          = 0x30,     ///<
    API_AUDIO_CUSTOMER_SPDIF_ChannelAB_Invalid_OFF         = 0x31,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_CopyProtect_CopyFree       = 0x32,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_CopyProtect_CopyOnce       = 0x33,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_CopyProtect_CopyNever      = 0x34,     ///<
} API_AUDIO_CUSTOMER_SPDIF_CS_TYPE_STATUS;

//-------------------------------------------------------------------------------------------------------------------------------------
/* HDMI */
//-------------------------------------------------------------------------------------------------------------------------------------
typedef enum
{
    API_AUDIO_CUSTOMER_HDMI_TX_OUTPUT_INVALID    = -1,
    API_AUDIO_CUSTOMER_HDMI_TX_OUTPUT_PCM,                       ///< HDMI OUTPUT PCM
    API_AUDIO_CUSTOMER_HDMI_TX_OUTPUT_NONPCM,                    ///< HDMI OUTPUT non-PCM (DD)
    API_AUDIO_CUSTOMER_HDMI_TX_OUTPUT_EAC3_BYPASS,               ///< HDMI OUTPUT DD/DDP Bypass
    API_AUDIO_CUSTOMER_HDMI_TX_OUTPUT_EAC3,                      ///< HDMI OUTPUT DDP Encode (MS12)
} API_AUDIO_CUSTOMER_HDMI_TX_OUTPUT_TYPE;

//-------------------------------------------------------------------------------------------------------------------------------------
/* ATV */
//-------------------------------------------------------------------------------------------------------------------------------------
typedef enum
{
    API_AUDIO_CUSTOMER_SIF_INPUT_INVALID    = -1,
    API_AUDIO_CUSTOMER_SIF_INPUT_EXTERNAL   = 0,
    API_AUDIO_CUSTOMER_SIF_INPUT_INTERNAL   = 1,
} API_AUDIO_CUSTOMER_SIF_INPUT;

typedef enum
{
    API_AUDIO_CUSTOMER_SIF_TYPE_INVALID         = -1,
    API_AUDIO_CUSTOMER_SIF_TYPE_NONE            = 0x0000,       ///< INIT TYPE : NONE
    API_AUDIO_CUSTOMER_SIF_ATSC_SELECT          = 0x0001,       ///< INIT TYPE : TV Systems for A2 enabled in default ATSC system
    API_AUDIO_CUSTOMER_SIF_KOREA_A2_SELECT      = 0x0002,       ///< INIT TYPE : TV Systems for A2 enabled in Korea A2 system
    API_AUDIO_CUSTOMER_SIF_BTSC_SELECT          = 0x0004,       ///< INIT TYPE : TV Systems for BTSC enabled in ATSC(CO, CF) or DVB(Taiwan) system
    API_AUDIO_CUSTOMER_SIF_BTSC_BR_SELECT       = 0x0008,       ///< INIT TYPE : TV Systems for BTSC enabled in ATSC(Brazil) system
    API_AUDIO_CUSTOMER_SIF_BTSC_US_SELECT       = 0x0010,       ///< INIT TYPE : TV Systems for BTSC enabled in ATSC(US) system
    API_AUDIO_CUSTOMER_SIF_DVB_SELECT           = 0x0020,       ///< INIT TYPE : TV Systems for EU in default DVB system
    API_AUDIO_CUSTOMER_SIF_DVB_ID_SELECT        = 0x0040,       ///< INIT TYPE : TV Systems for ID(Indonesia) in DVB(PAL B/G) system
    API_AUDIO_CUSTOMER_SIF_DVB_IN_SELECT        = 0x0080,       ///< INIT TYPE : TV Systems for IN(India) in DVB(PAL B) system
    API_AUDIO_CUSTOMER_SIF_DVB_CN_SELECT        = 0x0100,       ///< INIT TYPE : TV Systems for CN(China, Hong Kone) in DVB system
    API_AUDIO_CUSTOMER_SIF_DVB_AJJA_SELECT      = 0x0200,       ///< INIT TYPE : TV Systems for AJ(Asia JooDong), JA(JooAang Asia) in DVB system
    API_AUDIO_CUSTOMER_SIF_TYPE_MAX             = 0x0FFF,       ///< INIT TYPE : MAX
} API_AUDIO_CUSTOMER_SIF_TYPE;

typedef enum
{
    API_AUDIO_CUSTOMER_SIF_STANDARD_INVALID = -1,
    API_AUDIO_CUSTOMER_SIF_BG               = 0x00,         ///< Audio standard BG
    API_AUDIO_CUSTOMER_SIF_BG_A2            = 0x01,         ///< Audio standard BG A2
    API_AUDIO_CUSTOMER_SIF_BG_NICAM         = 0x02,         ///< Audio standard BG NICAM
    API_AUDIO_CUSTOMER_SIF_I                = 0x03,         ///< Audio standard I
    API_AUDIO_CUSTOMER_SIF_DK               = 0x04,         ///< Audio standard DK
    API_AUDIO_CUSTOMER_SIF_DK1_A2           = 0x05,         ///< Audio standard DK1 A2
    API_AUDIO_CUSTOMER_SIF_DK2_A2           = 0x06,         ///< Audio standard DK2 A2
    API_AUDIO_CUSTOMER_SIF_DK3_A2           = 0x07,         ///< Audio standard DK3 A2
    API_AUDIO_CUSTOMER_SIF_DK_NICAM         = 0x08,         ///< Audio standard DK NICAM
    API_AUDIO_CUSTOMER_SIF_L                = 0x09,         ///< Audio standard L
    API_AUDIO_CUSTOMER_SIF_M                = 0x0A,         ///< Audio standard M
    API_AUDIO_CUSTOMER_SIF_M_BTSC           = 0x0B,         ///< Audio standard M BTSC
    API_AUDIO_CUSTOMER_SIF_M_A2             = 0x0C,         ///< Audio standard M A2
    API_AUDIO_CUSTOMER_SIF_M_EIA_J          = 0x0D,         ///< Audio standard M EIA J
    API_AUDIO_CUSTOMER_SIF_NOTSTANDARD      = 0x0F          ///< Not Audio standard
} API_AUDIO_CUSTOMER_SIF_STANDARD;

typedef enum
{
    API_AUDIO_CUSTOMER_SIF_MODE_SET_INVALID             =     -1,
    API_AUDIO_CUSTOMER_SIF_SET_PAL_MONO                 =   0x00,   // PAL Mono
    API_AUDIO_CUSTOMER_SIF_SET_PAL_MONO_FORCED          =   0x01,   // PAL Mono Force Mono
    API_AUDIO_CUSTOMER_SIF_SET_PAL_STEREO               =   0x02,   // PAL Stereo
    API_AUDIO_CUSTOMER_SIF_SET_PAL_STEREO_FORCED        =   0x03,   // PAL Stereo Force Mono
    API_AUDIO_CUSTOMER_SIF_SET_PAL_DUALI                =   0x04,   // PAL Dual I
    API_AUDIO_CUSTOMER_SIF_SET_PAL_DUALII               =   0x05,   // PAL Dual II
    API_AUDIO_CUSTOMER_SIF_SET_PAL_DUALI_II             =   0x06,   // PAL Dual I+II
    API_AUDIO_CUSTOMER_SIF_SET_PAL_NICAM_MONO           =   0x07,   // PAL NICAM Mono
    API_AUDIO_CUSTOMER_SIF_SET_PAL_NICAM_MONO_FORCED    =   0x08,   // PAL NICAM Mono Force Mono
    API_AUDIO_CUSTOMER_SIF_SET_PAL_NICAM_STEREO         =   0x09,   // PAL NICAM Stereo
    API_AUDIO_CUSTOMER_SIF_SET_PAL_NICAM_STEREO_FORCED  =   0x0A,   // PAL NICAM Stereo Force Mono
    API_AUDIO_CUSTOMER_SIF_SET_PAL_NICAM_DUALI          =   0x0B,   // PAL NICAM Dual I
    API_AUDIO_CUSTOMER_SIF_SET_PAL_NICAM_DUALII         =   0x0C,   // PAL NICAM Dual II
    API_AUDIO_CUSTOMER_SIF_SET_PAL_NICAM_DUALI_II       =   0x0D,   // PAL NICAM Dual I+II
    API_AUDIO_CUSTOMER_SIF_SET_PAL_NICAM_DUAL_FORCED    =   0x0E,   // PAL NICAM Dual Forced Mono(Not Supported)
    API_AUDIO_CUSTOMER_SIF_SET_PAL_UNKNOWN              =   0x0F,   // PAL Unkown State
    API_AUDIO_CUSTOMER_SIF_SET_NTSC_A2_MONO             =   0x10,   // NTSC(A2) Mono
    API_AUDIO_CUSTOMER_SIF_SET_NTSC_A2_STEREO           =   0x11,   // NTSC(A2) Stereo
    API_AUDIO_CUSTOMER_SIF_SET_NTSC_A2_SAP              =   0x12,   // NTSC(A2) SAP
    API_AUDIO_CUSTOMER_SIF_SET_NTSC_A2_UNKNOWN          =   0x13,   // NTSC(A2) Unkown State
    API_AUDIO_CUSTOMER_SIF_SET_NTSC_BTSC_MONO           =   0x14,   // NTSC(BTSC) Mono
    API_AUDIO_CUSTOMER_SIF_SET_NTSC_BTSC_STEREO         =   0x15,   // NTSC(BTSC) Stereo
    API_AUDIO_CUSTOMER_SIF_SET_NTSC_BTSC_SAP_MONO       =   0x16,   // NTSC(BTSC) SAP Mono
    API_AUDIO_CUSTOMER_SIF_SET_NTSC_BTSC_SAP_STEREO     =   0x17,   // NTSC(BTSC) SAP Stereo
    API_AUDIO_CUSTOMER_SIF_SET_NTSC_BTSC_UNKNOWN        =   0x18,   // NTSC(BTSC) Unkown State
} API_AUDIO_CUSTOMER_SIF_MODE_SET;

typedef enum
{
    API_AUDIO_CUSTOMER_SIF_HHIDEV_FILTER_BW1        = 0x10, // 220kHz~315kHz
    API_AUDIO_CUSTOMER_SIF_HHIDEV_FILTER_BW2        = 0x20, // 320kHz~415kHz
    API_AUDIO_CUSTOMER_SIF_HHIDEV_FILTER_BW3        = 0x30, // 400kHz~495kHz
} API_AUDIO_CUSTOMER_SIF_HIDEV_BW;

///< Audio mode type
typedef enum
{
    API_AUDIO_CUSTOMER_SIF_AUDIOMODE_INVALID        = 0x00,           ///< Audio Mode Invalid
    API_AUDIO_CUSTOMER_SIF_AUDIOMODE_MONO           = 0x01,           ///< Audio Mode MONO
    API_AUDIO_CUSTOMER_SIF_AUDIOMODE_FORCED_MONO    = 0x02,           ///< Audio Mode Forced MONO
    API_AUDIO_CUSTOMER_SIF_AUDIOMODE_G_STEREO       = 0x03,           ///< Audio Mode G Stereo
    API_AUDIO_CUSTOMER_SIF_AUDIOMODE_K_STEREO       = 0x04,           ///< Audio Mode K Stereo
    API_AUDIO_CUSTOMER_SIF_AUDIOMODE_MONO_SAP       = 0x05,           ///< Audio Mode MONO SAP
    API_AUDIO_CUSTOMER_SIF_AUDIOMODE_STEREO_SAP     = 0x06,           ///< Audio Mode Stereo SAP
    API_AUDIO_CUSTOMER_SIF_AUDIOMODE_DUAL_A         = 0x07,           ///< Audio Mode Dual A
    API_AUDIO_CUSTOMER_SIF_AUDIOMODE_DUAL_B         = 0x08,           ///< Audio Mode Dual B
    API_AUDIO_CUSTOMER_SIF_AUDIOMODE_DUAL_AB        = 0x09,           ///< Audio Mode Dual AB
    API_AUDIO_CUSTOMER_SIF_AUDIOMODE_NICAM_MONO     = 0x0A,           ///< Audio Mode NICAM MONO
    API_AUDIO_CUSTOMER_SIF_AUDIOMODE_NICAM_STEREO   = 0x0B,           ///< Audio Mode NICAM Stereo
    API_AUDIO_CUSTOMER_SIF_AUDIOMODE_NICAM_DUAL_A   = 0x0C,           ///< Audio Mode NICAM DUAL A
    API_AUDIO_CUSTOMER_SIF_AUDIOMODE_NICAM_DUAL_B   = 0x0D,           ///< Audio Mode NICAM DUAL B
    API_AUDIO_CUSTOMER_SIF_AUDIOMODE_NICAM_DUAL_AB  = 0x0E,           ///< Audio Mode NICAM DUAL AB
    API_AUDIO_CUSTOMER_SIF_AUDIOMODE_HIDEV_MONO     = 0x0F,           ///< Audio Mode HIDEV MONO
    API_AUDIO_CUSTOMER_SIF_AUDIOMODE_LEFT_LEFT      = 0x10,           ///< Audio Mode Left left
    API_AUDIO_CUSTOMER_SIF_AUDIOMODE_RIGHT_RIGHT    = 0x11,           ///< Audio Mode right right
    API_AUDIO_CUSTOMER_SIF_AUDIOMODE_LEFT_RIGHT     = 0x12            ///< Audio Mode left right
} API_AUDIO_CUSTOMER_SIF_AUDIOMODE_TYPE;

///< Threshold table structure
typedef struct
{
    MS_U8  HiByteValue;                 ///< Threshold high-byte
    MS_U8  LowByteValue;                ///< Threshold low-byte
} API_AUDIO_CUSTOMER_SIF_THR_TBL_TYPE;

typedef enum
{
    API_AUDIO_CUSTOMER_SIF_PAL_MONO,
    API_AUDIO_CUSTOMER_SIF_PAL_HIDEV,
    API_AUDIO_CUSTOMER_SIF_PAL_A2,
    API_AUDIO_CUSTOMER_SIF_PAL_NICAM
}API_AUDIO_CUSTOMER_SIF_PAL_TYPE;

///< ATV SIF Command
typedef enum
{
    API_AUDIO_CUSTOMER_SIF_CMD_SET_STOP                  = 0x00,
    API_AUDIO_CUSTOMER_SIF_CMD_SET_PLAY                  = 0x01,
    API_AUDIO_CUSTOMER_SIF_CMD_ENABLE_HIDEV              = 0x02,
    API_AUDIO_CUSTOMER_SIF_CMD_SET_HIDEV_FILTER_BW_LEVEL = 0x03,
    API_AUDIO_CUSTOMER_SIF_CMD_RESET_FC_TRACKING         = 0x04,
    API_AUDIO_CUSTOMER_SIF_CMD_ENABLE_FC_TRACKING        = 0x05,
    API_AUDIO_CUSTOMER_SIF_CMD_SET_ADC_FROM_VIF_PATH     = 0x06,
    API_AUDIO_CUSTOMER_SIF_CMD_ENABLE_AUTO_MUTE          = 0x07,
    API_AUDIO_CUSTOMER_SIF_CMD_ENABLE_BEEPER_FUNCTION    = 0x08,
    API_AUDIO_CUSTOMER_SIF_CMD_SET_BEEPER_TONE           = 0x09,
    API_AUDIO_CUSTOMER_SIF_CMD_ENABLE_AGC                = 0x0A,
    API_AUDIO_CUSTOMER_SIF_CMD_RESET_AGC                 = 0x0B,
    API_AUDIO_CUSTOMER_SIF_STANDARD_RESET                = 0x0C,
    API_AUDIO_CUSTOMER_SIF_CMD_DETECT_MAIN_STD_ONLY      = 0x0D,
    API_AUDIO_CUSTOMER_SIF_CMD_ENABLE_AUTO_SOUNDMODE     = 0x0E,
    API_AUDIO_CUSTOMER_SIF_CMD_FMTX_SET_PREEMP           = 0x0F,
    API_AUDIO_CUSTOMER_SIF_CMD_SET_ENC_CMD               = 0x10,
    API_AUDIO_CUSTOMER_SIF_CMD_SET_ENC_Input_attenuation = 0x11,
    API_AUDIO_CUSTOMER_SIF_CMD_SET_ENC_Output_scaling    = 0x12,
    API_AUDIO_CUSTOMER_SIF_CMD_SET_BTSC_Enc_M_Gain       = 0x13,
    API_AUDIO_CUSTOMER_SIF_CMD_SET_BTSC_Enc_D_Gain       = 0x14,
    API_AUDIO_CUSTOMER_SIF_CMD_SET_BTSC_Enc_SAP_Gain     = 0x15,
    API_AUDIO_CUSTOMER_SIF_CMD2_ADC_VIA_VIF_PIN          = 0x01
} API_AUDIO_CUSTOMER_SIF_CmdType;

///< sif carrier status define
typedef enum
{
    // Never change the value of enumeration. Because each bit has weight.
    API_AUDIO_CUSTOMER_SIF_E_STATE_AUDIO_NO_CARRIER         = 0x00, ///< No carrier detect
    API_AUDIO_CUSTOMER_SIF_E_STATE_AUDIO_PRIMARY_CARRIER    = 0x01, ///< Carrier 1 exist
    API_AUDIO_CUSTOMER_SIF_E_STATE_AUDIO_SECONDARY_CARRIER  = 0x02, ///< Carrier 2 exist
    API_AUDIO_CUSTOMER_SIF_E_STATE_AUDIO_NICAM              = 0x04, ///< Nicam lock state
    API_AUDIO_CUSTOMER_SIF_E_STATE_AUDIO_STEREO             = 0x08, ///< A2 Stereo exist
    API_AUDIO_CUSTOMER_SIF_E_STATE_AUDIO_BILINGUAL          = 0x10, ///< A2 Dual exist
    API_AUDIO_CUSTOMER_SIF_E_STATE_AUDIO_PILOT              = 0x20, ///< A2 Pilot exist
    API_AUDIO_CUSTOMER_SIF_E_STATE_AUDIO_DK2                = 0x40, ///< Sound standard is DK2
    API_AUDIO_CUSTOMER_SIF_E_STATE_AUDIO_DK3                = 0x80  ///< Sound standard is DK3
} API_AUDIO_CUSTOMER_SIF_AUDIOSTATUS;

//======SIF Gain Type========
typedef enum
{
    API_AUDIO_CUSTOMER_SIF_PRESCALE_STEP_ONE_DB         = 0x04,
    API_AUDIO_CUSTOMER_SIF_SET_PRESCALE_A2_FM           = 0x00,
    API_AUDIO_CUSTOMER_SIF_SET_PRESCALE_NICAM           = 0x01,
    API_AUDIO_CUSTOMER_SIF_SET_PRESCALE_AM              = 0x02,
    API_AUDIO_CUSTOMER_SIF_SET_PRESCALE_HIDEV           = 0x03,
    API_AUDIO_CUSTOMER_SIF_SET_PRESCALE_FM_M            = 0x04,
    API_AUDIO_CUSTOMER_SIF_SET_PRESCALE_HIDEV_M         = 0x05,

    API_AUDIO_CUSTOMER_SIF_SET_PRESCALE_BTSC            = 0x00,
    API_AUDIO_CUSTOMER_SIF_SET_PRESCALE_BTSC_MONO       = 0x01,
    API_AUDIO_CUSTOMER_SIF_SET_PRESCALE_BTSC_STEREO     = 0x02,
    API_AUDIO_CUSTOMER_SIF_SET_PRESCALE_BTSC_SAP        = 0x03,

    API_AUDIO_CUSTOMER_SIF_SET_PRESCALE_FM_RADIO        = 0x00,
    API_AUDIO_CUSTOMER_SIF_SET_PRESCALE_FM_RADIO_MONO   = 0x01,
    API_AUDIO_CUSTOMER_SIF_SET_PRESCALE_FM_RADIO_STEREO = 0x02

}API_AUDIO_CUSTOMER_SIF_GAIN_TYPE;

//-------------------------------------------------------------------------------------------------------------------------------------
/* Decoder */
//-------------------------------------------------------------------------------------------------------------------------------------
typedef enum
{
    API_AUDIO_CUSTOMER_DOLBY_DRC_MODE_INVALID = -1,
    API_AUDIO_CUSTOMER_DOLBY_LINE_MODE,
    API_AUDIO_CUSTOMER_DOLBY_RF_MODE,
} API_AUDIO_CUSTOMER_DOLBY_DRC_MODE;

typedef enum
{
    API_AUDIO_CUSTOMER_DOWNMIX_MODE_INVALID = -1,
    API_AUDIO_CUSTOMER_LTRT_MODE,
    API_AUDIO_CUSTOMER_LORO_MODE,
} API_AUDIO_CUSTOMER_DOWNMIX_MODE;

typedef enum
{
    API_AUDIO_CUSTOMER_DUALMONO_MODE_INVALID = -1,
    API_AUDIO_CUSTOMER_DUALMONO_MODE_LR,
    API_AUDIO_CUSTOMER_DUALMONO_MODE_LL,
    API_AUDIO_CUSTOMER_DUALMONO_MODE_RR,
    API_AUDIO_CUSTOMER_DUALMONO_MODE_MIX,
} API_AUDIO_CUSTOMER_DUALMONO_MODE;

typedef enum
{
    API_AUDIO_CUSTOMER_IN_PORT_INVALID  = -1,
    API_AUDIO_CUSTOMER_IN_PORT_NONE     =  0,
    API_AUDIO_CUSTOMER_IN_PORT_TP       =  1,   // From TPA Stream Input
    API_AUDIO_CUSTOMER_IN_PORT_SPDIF    =  2,   // From SERIAL INTERFACE 0
    API_AUDIO_CUSTOMER_IN_PORT_SIF      =  3,   // From Analog Front End (SIF)
    API_AUDIO_CUSTOMER_IN_PORT_ADC      =  4,   // Fron ADC Input
    API_AUDIO_CUSTOMER_IN_PORT_HDMI     =  5,   // From HDMI
    API_AUDIO_CUSTOMER_IN_PORT_I2S      =  6,   // From I2S
    API_AUDIO_CUSTOMER_IN_PORT_SYSTEM   =  7,   // From System
} API_AUDIO_CUSTOMER_IN_PORT;

typedef enum
{
    API_AUDIO_CUSTOMER_SAMPLING_FREQ_INVALID    =       -1,
    API_AUDIO_CUSTOMER_SAMPLING_FREQ_NONE       =        0,
    API_AUDIO_CUSTOMER_SAMPLING_FREQ_4_KHZ      =     4000,
    API_AUDIO_CUSTOMER_SAMPLING_FREQ_8_KHZ      =     8000,
    API_AUDIO_CUSTOMER_SAMPLING_FREQ_11_025KHZ  =    11025,
    API_AUDIO_CUSTOMER_SAMPLING_FREQ_12_KHZ     =    12000,
    API_AUDIO_CUSTOMER_SAMPLING_FREQ_16_KHZ     =    16000,
    API_AUDIO_CUSTOMER_SAMPLING_FREQ_22_05KHZ   =    22050,
    API_AUDIO_CUSTOMER_SAMPLING_FREQ_24_KHZ     =    24000,
    API_AUDIO_CUSTOMER_SAMPLING_FREQ_32_KHZ     =    32000,
    API_AUDIO_CUSTOMER_SAMPLING_FREQ_44_1KHZ    =    44100,
    API_AUDIO_CUSTOMER_SAMPLING_FREQ_48_KHZ     =    48000,
    API_AUDIO_CUSTOMER_SAMPLING_FREQ_64_KHZ     =    64000,
    API_AUDIO_CUSTOMER_SAMPLING_FREQ_88_2KHZ    =    88200,
    API_AUDIO_CUSTOMER_SAMPLING_FREQ_96_KHZ     =    96000,
    API_AUDIO_CUSTOMER_SAMPLING_FREQ_128_KHZ    =   128000,
    API_AUDIO_CUSTOMER_SAMPLING_FREQ_176_4KHZ   =   176400,
    API_AUDIO_CUSTOMER_SAMPLING_FREQ_192_KHZ    =   192000,
    API_AUDIO_CUSTOMER_SAMPLING_FREQ_768_KHZ    =   768000,
    API_AUDIO_CUSTOMER_SAMPLING_FREQ_DEFAULT    =   999000,
} API_AUDIO_CUSTOMER_SAMPLING_FREQ;

typedef enum
{
    API_AUDIO_CUSTOMER_HDMI_INDEX_INVALID   = -1,
    API_AUDIO_CUSTOMER_HDMI0                = 0,
    API_AUDIO_CUSTOMER_HDMI1                = 1,
    API_AUDIO_CUSTOMER_HDMI2                = 2,
    API_AUDIO_CUSTOMER_HDMI3                = 3,
    API_AUDIO_CUSTOMER_HDMI_SWITCH          = 4,
    API_AUDIO_CUSTOMER_HDMI_MAX             = API_AUDIO_CUSTOMER_HDMI_SWITCH + 1,
} API_AUDIO_CUSTOMER_HDMI_INDEX;

typedef enum
{
    API_AUDIO_CUSTOMER_CHANNEL_MODE_INVALID = -1,
    API_AUDIO_CUSTOMER_CH_MODE_DUAL_MONO    = 0,  //MPEG/Dolby
    API_AUDIO_CUSTOMER_CH_MODE_MONO         = 1,  //MPEG/Dolby
    API_AUDIO_CUSTOMER_CH_MODE_STEREO       = 2,  //MPEG/Dolby
    API_AUDIO_CUSTOMER_CH_MODE_3_0_MODE     = 3,  //     Dolby
    API_AUDIO_CUSTOMER_CH_MODE_2_1_MODE     = 4,  //     Dolby
    API_AUDIO_CUSTOMER_CH_MODE_3_1_MODE     = 5,  //     Dolby
    API_AUDIO_CUSTOMER_CH_MODE_2_2_MODE     = 6,  //     Dolby
    API_AUDIO_CUSTOMER_CH_MODE_3_2_MODE     = 7,  //     Dolby
    API_AUDIO_CUSTOMER_CH_MODE_JOINT_STEREO = 8,  //MPEG/
    API_AUDIO_CUSTOMER_CH_MODE_3_3_MODE     = 9,  //DRA
    API_AUDIO_CUSTOMER_CH_MODE_3_2_2_MODE   = 10, //DRA /Dolby
    API_AUDIO_CUSTOMER_CH_MODE_3_2_3_MODE   = 11, //DRA
    API_AUDIO_CUSTOMER_CH_MODE_NONE         = 12,
} API_AUDIO_CUSTOMER_CHANNEL_MODE;

typedef struct
{
    MS_U8 version;              /* AAC = 0x0, HE-AACv1 = 0x1, HE-AACv2 = 0x2 */
    MS_U8 transmissionformat;   /* LOAS/LATM = 0x0, ADTS = 0x1*/
    MS_U8 channelNum;
    API_AUDIO_CUSTOMER_CHANNEL_MODE channeMode;
} API_AUDIO_CUSTOMER_HEAAC_ES_INFO;

typedef struct
{
    MS_U8 bitRate;
    MS_U8 sampleRate;
    MS_U8 layer;
    MS_U8 channelNum;
    API_AUDIO_CUSTOMER_CHANNEL_MODE channeMode;
} API_AUDIO_CUSTOMER_MPEG_ES_INFO;

typedef struct
{
    MS_U8 bitRate;
    MS_U8 sampleRate;
    MS_U8 channelNum;
    MS_U8 EAC3; /* AC3 0x0, EAC3 0x1*/
    API_AUDIO_CUSTOMER_CHANNEL_MODE channeMode;
} API_AUDIO_CUSTOMER_AC3_ES_INFO;

//need to be consistent with XPCM_TYPE of drvAudio_if.h
typedef enum
{
    API_AUDIO_CUSTOMER_XPCM_LPCM            = 1,
    API_AUDIO_CUSTOMER_XPCM_MS_ADPCM        = 2,
    API_AUDIO_CUSTOMER_XPCM_G711_A_LAW      = 6,
    API_AUDIO_CUSTOMER_XPCM_G711_u_LAW      = 7,
    API_AUDIO_CUSTOMER_XPCM_DVD_LPCM        = 8,
    API_AUDIO_CUSTOMER_XPCM_PRIVATE1_LPCM   = 9,
    API_AUDIO_CUSTOMER_XPCM_IMA_ADPCM       = 17,
    API_AUDIO_CUSTOMER_XPCM_IMA_ADPCM_APPLE = 18, // apple IMA4
} API_AUDIO_CUSTOMER_XPCM_TYPE;

//need to be consistent with Audio_XPCM_Param of drvAudio_if.h
typedef struct
{
    API_AUDIO_CUSTOMER_XPCM_TYPE audioType;
    MS_U32 sampleRate;
    MS_U16 blockSize;
    MS_U16 samplePerBlock;
    MS_U8  channels;
    MS_U8  bitsPerSample;
    MS_U32 dwchannelMask;   //  #define SPEAKER_FRONT_LEFT             0x1
                            //  #define SPEAKER_FRONT_RIGHT            0x2
                            //  #define SPEAKER_FRONT_CENTER           0x4
                            //  #define SPEAKER_LOW_FREQUENCY          0x8
                            //  #define SPEAKER_BACK_LEFT              0x10
                            //  #define SPEAKER_BACK_RIGHT             0x20 // <--- only support to here
                            //  #define SPEAKER_FRONT_LEFT_OF_CENTER   0x40
                            //  #define SPEAKER_FRONT_RIGHT_OF_CENTER  0x80
                            //  #define SPEAKER_BACK_CENTER            0x100
                            //  #define SPEAKER_SIDE_LEFT              0x200
                            //  #define SPEAKER_SIDE_RIGHT             0x400
                            //  #define SPEAKER_TOP_CENTER             0x800
                            //  #define SPEAKER_TOP_FRONT_LEFT         0x1000
                            //  #define SPEAKER_TOP_FRONT_CENTER       0x2000
                            //  #define SPEAKER_TOP_FRONT_RIGHT        0x4000
                            //  #define SPEAKER_TOP_BACK_LEFT          0x8000
                            //  #define SPEAKER_TOP_BACK_CENTER        0x10000
                            //  #define SPEAKER_TOP_BACK_RIGHT         0x20000
                            //  #define SPEAKER_RESERVED               0x80000000
} API_AUDIO_CUSTOMER_XPCM_PARAM;

//need to be consistent with Audio_COOK_Param of drvAudio_if.h
typedef struct
{
    #define COOK_MAX_NUM_CODECS 5
    MS_U16 Channels[COOK_MAX_NUM_CODECS];
    MS_U16 Regions[COOK_MAX_NUM_CODECS];
    MS_U16 cplStart[COOK_MAX_NUM_CODECS];
    MS_U16 cplQbits[COOK_MAX_NUM_CODECS];
    MS_U16 FrameSize[COOK_MAX_NUM_CODECS];
    MS_U16 mNumCodecs;
    MS_U16 mSamples;
    MS_U16 mSampleRate;
} API_AUDIO_CUSTOMER_COOK_PARM;

typedef enum
{
    API_AUDIO_CUSTOMER_TRICK_MODE_INVALID       = -1,
    API_AUDIO_CUSTOMER_TRICK_NONE               = 0,    ///<  rate : None, TP Live Play
    API_AUDIO_CUSTOMER_TRICK_PAUSE              = 1,    ///<  rate : Pause, DVR Play
    API_AUDIO_CUSTOMER_TRICK_NORMAL_PLAY        = 2,    ///<  rate : 1.00 Play
    API_AUDIO_CUSTOMER_TRICK_SLOW_MOTION_0P25X  = 3,    ///<  rate : 0.25 Play
    API_AUDIO_CUSTOMER_TRICK_SLOW_MOTION_0P50X  = 4,    ///<  rate : 0.50 Play
    API_AUDIO_CUSTOMER_TRICK_FAST_FORWARD_1P25X = 5,    ///<  rate : 1.25 Play
    API_AUDIO_CUSTOMER_TRICK_FAST_FORWARD_1P50X = 6,    ///<  rate : 1.50 Play
    API_AUDIO_CUSTOMER_TRICK_FAST_FORWARD_2P00X = 7,    ///<  rate : 2.00 Play
} API_AUDIO_CUSTOMER_TRICK_MODE;

typedef enum
{
    API_AUDIO_CUSTOMER_MPEG_LAYER_INVALID             = -1,
    API_AUDIO_CUSTOMER_MPEG_LAYER_RESERVED            = 0,
    API_AUDIO_CUSTOMER_MPEG_LAYER_1                   = 1,
    API_AUDIO_CUSTOMER_MPEG_LAYER_2                   = 2,
    API_AUDIO_CUSTOMER_MPEG_LAYER_3                   = 3,
    API_AUDIO_CUSTOMER_MPEG_LAYER_NOT_DETERMINE_YET   = 0xFF,
} API_AUDIO_CUSTOMER_MPEG_LAYER;

//need to be consistent with AUDIO_AAC_TYPE of drvAudio_if.h
typedef enum
{
    API_AUDIO_CUSTOMER_AAC_TYPE_GENERIC_AAC,
    API_AUDIO_CUSTOMER_AAC_TYPE_GENERIC_HEAACv1,
    API_AUDIO_CUSTOMER_AAC_TYPE_GENERIC_HEAACv2,
    API_AUDIO_CUSTOMER_AAC_TYPE_DOLBY_AAC,
    API_AUDIO_CUSTOMER_AAC_TYPE_DOLBY_HEAACv1,
    API_AUDIO_CUSTOMER_AAC_TYPE_DOLBY_HEAACv2,
} API_AUDIO_CUSTOMER_AAC_TYPE;

//need to be consistent with Buffer_Info_t of drvAudio_if.h
typedef struct
{
    MS_U32 u32Size;
    MS_U64 u64Pts;
    MS_U64 u64MetadataIndex;
    MS_U8  *pBuffer;
} API_AUDIO_CUSTOMER_FRAMEDECODE_BUFFER;

//-------------------------------------------------------------------------------------------------------------------------------------
/* Common Decoder */
//-------------------------------------------------------------------------------------------------------------------------------------
//need to be consistent with AUDIO_DECODER_INFO_VERSION of drvAudio_if.h
#define API_AUDIO_CUSTOMER_COMMON_DECODER_INFO_VERSION 0x00010001

//need to be consistent with AUDIO_DECODER_Command_t of drvAudio_if.h
typedef struct
{
    MS_U8  CmdName[64];
    MS_U32 CmdLen;
} API_AUDIO_CUSTOMER_COMMON_DECODER_COMMAND;

//need to be consistent with AUDIO_DECODER_SOURCE_TYPE of drvAudio_if.h
typedef enum
{
    API_AUDIO_CUSTOMER_COMMON_DECODER_SOURCE_INVALID = -1,
    API_AUDIO_CUSTOMER_COMMON_DECODER_SOURCE_NONE,
    API_AUDIO_CUSTOMER_COMMON_DECODER_SOURCE_DTV,
    API_AUDIO_CUSTOMER_COMMON_DECODER_SOURCE_ATV,
    API_AUDIO_CUSTOMER_COMMON_DECODER_SOURCE_HDMI,
    API_AUDIO_CUSTOMER_COMMON_DECODER_SOURCE_ADC,
    API_AUDIO_CUSTOMER_COMMON_DECODER_SOURCE_MM,
    API_AUDIO_CUSTOMER_COMMON_DECODER_SOURCE_SPDIF,
} API_AUDIO_CUSTOMER_COMMON_DECODER_SOURCE;

//need to be consistent with AUDIO_DECODER_PLAY_MODE of drvAudio_if.h
typedef enum
{
    API_AUDIO_CUSTOMER_COMMON_DECODER_PLAY_MODE_INVALID = -1,
    API_AUDIO_CUSTOMER_COMMON_DECODER_PLAY_MODE_FRAME_HAND_SHAKE,   // Gstreamer , OMX
    API_AUDIO_CUSTOMER_COMMON_DECODER_PLAY_MODE_FILE_TS,            // Send data to HW demux
    API_AUDIO_CUSTOMER_COMMON_DECODER_PLAY_MODE_ES_WITH_TS,         // MM new mode
    API_AUDIO_CUSTOMER_COMMON_DECODER_PLAY_MODE_ES,                 // MM old mode
    API_AUDIO_CUSTOMER_COMMON_DECODER_PLAY_MODE_DTV,                //
    API_AUDIO_CUSTOMER_COMMON_DECODER_PLAY_MODE_OTHER,              // HDMI
} API_AUDIO_CUSTOMER_COMMON_DECODER_PLAY_MODE;

//need to be consistent with AUDIO_DECODER_AV_SYNC_MODE of drvAudio_if.h
typedef enum
{
    API_AUDIO_CUSTOMER_COMMON_DECODER_SYNC_MODE_INVALID = -1,
    API_AUDIO_CUSTOMER_COMMON_DECODER_SYNC_MODE_AV_SYNC,
    API_AUDIO_CUSTOMER_COMMON_DECODER_SYNC_MODE_FREE_RUN,
} API_AUDIO_CUSTOMER_COMMON_DECODER_SYNC_MODE;

//need to be consistent with AUDIO_DECODER_CODECTYPE of drvAudio_if.h
typedef enum
{
    API_AUDIO_CUSTOMER_COMMON_DECODER_CODECTYPE_INVALID = -1,
    API_AUDIO_CUSTOMER_COMMON_DECODER_CODECTYPE_PCM,
    API_AUDIO_CUSTOMER_COMMON_DECODER_CODECTYPE_XPCM,
    API_AUDIO_CUSTOMER_COMMON_DECODER_CODECTYPE_MPEG,
    API_AUDIO_CUSTOMER_COMMON_DECODER_CODECTYPE_MP3,
    API_AUDIO_CUSTOMER_COMMON_DECODER_CODECTYPE_AC3,
    API_AUDIO_CUSTOMER_COMMON_DECODER_CODECTYPE_AC3P,
    API_AUDIO_CUSTOMER_COMMON_DECODER_CODECTYPE_AAC_LC,
    API_AUDIO_CUSTOMER_COMMON_DECODER_CODECTYPE_HEAAC_V1,
    API_AUDIO_CUSTOMER_COMMON_DECODER_CODECTYPE_HEAAC_V2,
    API_AUDIO_CUSTOMER_COMMON_DECODER_CODECTYPE_DOLBYPULSE,
    API_AUDIO_CUSTOMER_COMMON_DECODER_CODECTYPE_WMA,
    API_AUDIO_CUSTOMER_COMMON_DECODER_CODECTYPE_WMAPRO,
    API_AUDIO_CUSTOMER_COMMON_DECODER_CODECTYPE_RA8,
    API_AUDIO_CUSTOMER_COMMON_DECODER_CODECTYPE_DTS,
    API_AUDIO_CUSTOMER_COMMON_DECODER_CODECTYPE_FLAC,
    API_AUDIO_CUSTOMER_COMMON_DECODER_CODECTYPE_VORBIS,
    API_AUDIO_CUSTOMER_COMMON_DECODER_CODECTYPE_AMR_NB,
    API_AUDIO_CUSTOMER_COMMON_DECODER_CODECTYPE_AMR_WB,
    API_AUDIO_CUSTOMER_COMMON_DECODER_CODECTYPE_DRA,
    API_AUDIO_CUSTOMER_COMMON_DECODER_CODECTYPE_DTSLBR,
    API_AUDIO_CUSTOMER_COMMON_DECODER_CODECTYPE_DTSXLL,
    API_AUDIO_CUSTOMER_COMMON_DECODER_CODECTYPE_NULL = 0xFFFFFFFF
} API_AUDIO_CUSTOMER_COMMON_DECODER_CODECTYPE;

//need to be consistent with Audio_Ringbuf of drvAudio_if.h
typedef struct
{
    MS_U32 u32BufStartAddr;
    MS_U32 u32BufEndAddr;
    MS_U32 u32BufWrPtr;
    MS_U32 u32BufRdPtr;
    MS_U32 u32BufTotalSize;
    MS_U32 u32BufOffset;
    MS_U32 u32Size;
    MS_U32 u32CurLevel;
} API_AUDIO_CUSTOMER_COMMON_DECODER_RINGBUFFER;

//need to be consistent with AUDIO_DECODER_MM_Info_t of drvAudio_if.h
typedef struct
{
    API_AUDIO_CUSTOMER_COMMON_DECODER_RINGBUFFER  EsCtrl;  /* ES  buf control */
    API_AUDIO_CUSTOMER_COMMON_DECODER_RINGBUFFER  PcmCtrl; /* PCM buf control */

    /*Input ES related*/
    MS_U32 u32NeedDecodeFrmCnt;  /* Need decode frame count */
    MS_U64 u64TimeStamp;         /* Time stamp */

    /*Output PCM related*/
    MS_U32 u32OutputFrameCnt;    /* Decoder decode done frame count */
    MS_U32 u32OutputFrameCntAck; /* Inform Decoder output frame count */
} API_AUDIO_CUSTOMER_COMMON_DECODER_MM_INFO;

//need to be consistent with AUDIO_DECODER_Info_t of drvAudio_if.h
typedef struct
{
    char u8Name[32];            /* The DECODER name requested (only 32 bytes, if change this length, must change u8Name of API_AUDIO_CUSTOMER_COMMON_DECODER_COMMAND as well!) */
    MS_U32 u32StructVersion;    /* version number of this data structure */
    MS_U32 u32StructSize;       /* size of this data structure */

    MS_U32 u32Connect;          /* current connect status*/
    MS_U32 u32CodecType;        /* Set Decoder format ex, AC3 MP3 AAC DTS etc. please refer to API_AUDIO_CUSTOMER_COMMON_DECODER_CODECTYPE */
    MS_U32 u32SourceType;       /* Set Audio Source, ex, DTV / HDMI /MM etc. */
    MS_U32 u32OutputGroup;      /* Set Decoder Output Group */
    MS_U32 u32PlayMode;         /* Decoder Playback mode, please refer to API_AUDIO_CUSTOMER_COMMON_DECODER_PLAY_MODE */
    MS_U32 u32AfifoSource;      /* Set AUDIO FIFO */
    MS_U32 u32AdEnable;         /* Enable AD */
    MS_U32 u32AvSyncMode;       /* DECODER AV SYNC MODE, please refer to API_AUDIO_CUSTOMER_COMMON_DECODER_SYNC_MODE */

    MS_U32 u32MadBaseAddr;
    MS_U32 u32Endian;
    MS_U32 u32Channels;
    MS_U32 u32BitsPerSample;
    MS_U32 u32SamplingRate;
    MS_U32 u32BitRate;
    MS_U32 u32DecodeFrameCnt;

    API_AUDIO_CUSTOMER_COMMON_DECODER_MM_INFO tDecoderMMInfo;
    MS_U32 *pCodecExtraData;        /* Extra data pointer if necessary */
    MS_U32 u32CodecExtraDataSize;   /* Extra data size */
} API_AUDIO_CUSTOMER_COMMON_DECODER_INFO;

//-------------------------------------------------------------------------------------------------------------------------------------
/* Common Cmd */
//-------------------------------------------------------------------------------------------------------------------------------------
//need to be consistent with AUDIO_COMMON_CMD_t of drvAudio_if.h
typedef struct
{
    MS_U8 u8StrCmd[64];
    void *pData;
} API_AUDIO_CUSTOMER_COMMON_CMD;

//need to be consistent with AUDIO_DATA_IO_INFO_t of drvAudio_if.h
typedef struct
{
    void *pData;
    MS_U32 u32Size;
} API_AUDIO_CUSTOMER_DATA_IO_INFO;

//need to be consistent with AUDIO_SPDIF_PLAYBACK_CONFIG_t of drvAudio_if.h
typedef struct
{
    MS_U32 u32StructVersion; //AUDIO_SPDIF_PLAYBACK_CONFIG_VERSION
    MS_U32 u32StructSize;    //sizeof(AUDIO_SPDIF_PLAYBACK_CONFIG_t)
    MS_U8 u8PreanbleFlag;
    MS_U32 u32SampleRate;
} API_AUDIO_CUSTOMER_SPDIF_PLAYBACK_CONFIG;

//need to be consistent with AUDIO_SPDIF_PLAYBACK_CONFIG_VERSION of drvAudio_if.h
#define API_AUDIO_CUSTOMER_SPDIF_PLAYBACK_CONFIG_VERSION    0x00010001 //[31:16]major version; [15:0]minor version

//-------------------------------------------------------------------------------------------------------------------------------------
/* Common */
//-------------------------------------------------------------------------------------------------------------------------------------
typedef enum
{
    API_AUDIO_CUSTOMER_PARAM_TYPE_INVALID                       = -1,

    //--------------------------------------------------------------------------------------
    // Decoder (API_AUDIO_CUSTOMER_"ADEC"_PARAM_XXXXXXXX)
    //--------------------------------------------------------------------------------------
    //---MPEG---
    API_AUDIO_CUSTOMER_ADEC_PARAM_MPEG_SOUND_MODE               = 0,

    //---Dolby AC3P---
    API_AUDIO_CUSTOMER_ADEC_PARAM_AC3P_DRC_MODE                 = 1,
    API_AUDIO_CUSTOMER_ADEC_PARAM_AC3P_DOWNMIX_MODE             = 2,
    API_AUDIO_CUSTOMER_ADEC_PARAM_AC3P_DOLBY_BULLETIN11         = 3,
    API_AUDIO_CUSTOMER_ADEC_PARAM_AC3P_HDMITX_BYPASS_ENABLE     = 4,
    API_AUDIO_CUSTOMER_ADEC_PARAM_AC3P_DRC_HIGHCUT_SCALE        = 5,
    API_AUDIO_CUSTOMER_ADEC_PARAM_AC3P_DRC_LOWBOOT_SCALE        = 6,
    API_AUDIO_CUSTOMER_ADEC_PARAM_AC3P_1PID_AD_ENABLE           = 37,

    //---AAC---
    API_AUDIO_CUSTOMER_ADEC_PARAM_AAC_DRC_MODE                  = 7,
    API_AUDIO_CUSTOMER_ADEC_PARAM_AAC_DOWNMIX_MODE              = 8,

    //---WMA---
    API_AUDIO_CUSTOMER_ADEC_PARAM_WMA_VERSION                   = 9,
    API_AUDIO_CUSTOMER_ADEC_PARAM_WMA_CHANNELS                  = 10,
    API_AUDIO_CUSTOMER_ADEC_PARAM_WMA_SAMPLERATE                = 11,
    API_AUDIO_CUSTOMER_ADEC_PARAM_WMA_BYTERATE                  = 12,
    API_AUDIO_CUSTOMER_ADEC_PARAM_WMA_BLOCKALIGN                = 13,
    API_AUDIO_CUSTOMER_ADEC_PARAM_WMA_ENCOPT                    = 14,
    API_AUDIO_CUSTOMER_ADEC_PARAM_WMA_PARSINGBYAPP              = 15,
    API_AUDIO_CUSTOMER_ADEC_PARAM_WMA_BITS_PER_SAMPLE           = 16,
    API_AUDIO_CUSTOMER_ADEC_PARAM_WMA_CHANNELMASK               = 17,
    API_AUDIO_CUSTOMER_ADEC_PARAM_WMA_DRC_PARAM_EXIST           = 18,
    API_AUDIO_CUSTOMER_ADEC_PARAM_WMA_DRC_RMS_AMP_REF           = 19,
    API_AUDIO_CUSTOMER_ADEC_PARAM_WMA_DRC_RMS_AMP_TARGET        = 20,
    API_AUDIO_CUSTOMER_ADEC_PARAM_WMA_DRC_PEAK_AMP_REF          = 21,
    API_AUDIO_CUSTOMER_ADEC_PARAM_WMA_DRC_PEAK_AMP_TARGET       = 22,
    API_AUDIO_CUSTOMER_ADEC_PARAM_WMA_MAX_PACKET_SIZE           = 23,

    //---XPCM---
    API_AUDIO_CUSTOMER_ADEC_PARAM_XPCM                          = 24,

    //---Underrun threshold in ms, Debounce Cnt---
    API_AUDIO_CUSTOMER_ADEC_PARAM_UNDERRUN_THRESHOLD_IN_MS      = 26,
    API_AUDIO_CUSTOMER_ADEC_PARAM_UNDERRUN_DEBOUNCE_CNT         = 27,

    //---Decoder Setting---
    API_AUDIO_CUSTOMER_ADEC_PARAM_AVDLY                         = 38,

    //--------------------------------------------------------------------------------------
    // ASND (API_AUDIO_CUSTOMER_"ASND"_PARAM_XXXXXXXX)
    //--------------------------------------------------------------------------------------
    //---atmos lock---
    API_AUDIO_CUSTOMER_ASND_PARAM_ATMOS_LOCK                    = 39, //<--MAX is here!!!

    //--------------------------------------------------------------------------------------
    // Common (API_AUDIO_CUSTOMER_"COMMON"_PARAM_XXXXXXXX)
    //--------------------------------------------------------------------------------------
    //---Audio Dump Info---
    API_AUDIO_CUSTOMER_COMMON_PARAM_DUMP_INFO_Off               = 25,
    API_AUDIO_CUSTOMER_COMMON_PARAM_DUMP_INFO_AVSYNC            = 28,
    API_AUDIO_CUSTOMER_COMMON_PARAM_DUMP_INFO_ADEC0             = 29,
    API_AUDIO_CUSTOMER_COMMON_PARAM_DUMP_INFO_IO_INFO           = 30,
    API_AUDIO_CUSTOMER_COMMON_PARAM_DUMP_INFO_MM_FILE_AVSYNC    = 31,
    API_AUDIO_CUSTOMER_COMMON_PARAM_DUMP_INFO_ALL               = 32,
    API_AUDIO_CUSTOMER_COMMON_PARAM_DUMP_INFO_AUTOTEST          = 33,

    //---Common Cmd---
    API_AUDIO_CUSTOMER_COMMON_PARAM_COMMON_CMD                  = 34,

    //---OMX---
    API_AUDIO_CUSTOMER_COMMON_PARAM_OMX_SPDIF_MODE              = 35,
    API_AUDIO_CUSTOMER_COMMON_PARAM_OMX_SPDIF_PLAY              = 36,

    //---MAX---
    API_AUDIO_CUSTOMER_PARAM_MAX = API_AUDIO_CUSTOMER_ADEC_PARAM_AC3P_1PID_AD_ENABLE + 1,
} API_AUDIO_CUSTOMER_PARAM_TYPE;

typedef enum
{
    API_AUDIO_CUSTOMER_ADEC_INFO_TYPE_INVALID                       = -1,

    //--------------------------------------------------------------------------------------
    // Decoder (API_AUDIO_CUSTOMER_"ADEC"_INFO_XXXXXXXX)
    //--------------------------------------------------------------------------------------
    //---MPEG---
    API_AUDIO_CUSTOMER_ADEC_INFO_MPEG_SOUND_MODE                    = 0,
    API_AUDIO_CUSTOMER_ADEC_INFO_MPEG_LAYER                         = 25, //<--MAX is here!!!

    //---Dolby AC3P---
    API_AUDIO_CUSTOMER_ADEC_INFO_AC3P_DRC_MODE                      = 1,
    API_AUDIO_CUSTOMER_ADEC_INFO_AC3P_DOWNMIX_MODE                  = 2,
    API_AUDIO_CUSTOMER_ADEC_INFO_AC3P_IS_ATMOS_STREAM               = 18,

    //---AAC---
    API_AUDIO_CUSTOMER_ADEC_INFO_AAC_DRC_MODE                       = 3,
    API_AUDIO_CUSTOMER_ADEC_INFO_AAC_DOWNMIX_MODE                   = 4,
    API_AUDIO_CUSTOMER_ADEC_INFO_AAC_TYPE                           = 23,

    //---Decode Info---
    API_AUDIO_CUSTOMER_ADEC_INFO_SAMPLERATE                         = 9,
    API_AUDIO_CUSTOMER_ADEC_INFO_ACMODE                             = 10,
    API_AUDIO_CUSTOMER_ADEC_INFO_AVSYNC_STATE                       = 11,

    //--------------------------------------------------------------------------------------
    // Common (API_AUDIO_CUSTOMER_"COMMON"_INFO_XXXXXXXX)
    //--------------------------------------------------------------------------------------
    //---ES---
    API_AUDIO_CUSTOMER_COMMON_INFO_ES_BUFFER_BASE                   = 5,
    API_AUDIO_CUSTOMER_COMMON_INFO_ES_BUFFER_SIZE                   = 6,

    //---AD_ES (Audio description)---
    API_AUDIO_CUSTOMER_COMMON_INFO_AD_ES_BUFFER_BASE                = 14,
    API_AUDIO_CUSTOMER_COMMON_INFO_AD_ES_BUFFER_SIZE                = 15,

    //---PCM---
    API_AUDIO_CUSTOMER_COMMON_INFO_PCM_BUFFER_CURRENT_LEVEL         = 7,
    API_AUDIO_CUSTOMER_COMMON_INFO_PCM_BUFFER_CURRENT_LEVEL_IN_MS   = 12,

    //---PTS---
    API_AUDIO_CUSTOMER_COMMON_INFO_33BIT_PTS                        = 8,
    API_AUDIO_CUSTOMER_COMMON_INFO_AD_33BIT_PTS                     = 24,

    //---OK, ERR Frame Cnt, IS_MAD_LOCK---
    API_AUDIO_CUSTOMER_COMMON_INFO_OK_FRAME_CNT                     = 13,
    API_AUDIO_CUSTOMER_COMMON_INFO_ERR_FRAME_CNT                    = 16,
    API_AUDIO_CUSTOMER_COMMON_INFO_IS_MAD_LOCK                      = 21,

    //---Delay---
    API_AUDIO_CUSTOMER_COMMON_INFO_AUDIO_DELAY                      = 17,
    API_AUDIO_CUSTOMER_COMMON_INFO_SEDSP_MS12_DDE_DELAY             = 19,
    API_AUDIO_CUSTOMER_COMMON_INFO_SNDR2_MS12_DDPE_DELAY            = 20,
    API_AUDIO_CUSTOMER_COMMON_INFO_SNDR2_MS12_DAP_DELAY             = 22,

    //---MAX---
    API_AUDIO_CUSTOMER_INFO_MAX = API_AUDIO_CUSTOMER_ADEC_INFO_MPEG_LAYER + 1,
} API_AUDIO_CUSTOMER_INFO_TYPE;

//-------------------------------------------------------------------------------------------------------------------------------------
/* Customized patch */
//-------------------------------------------------------------------------------------------------------------------------------------
typedef enum
{
    API_AUDIO_CUSTOMIZED_PATCH_PARAM_TYPE_INVALID = -1,

    //--------------------------------------------------------------------------------------
    // (API_AUDIO_"CUSTOMIZED_PATCH"_PARAM_XXXXXXXX), PARAM: Set
    //--------------------------------------------------------------------------------------
    //---ES Passthrough---
    API_AUDIO_CUSTOMIZED_PATCH_PARAM_ES_PASSTHROUGH_DELAY_SAMPLE                = 0,
    API_AUDIO_CUSTOMIZED_PATCH_PARAM_ES_PASSTHROUGH_PCM_SAMPLERATE              = 1,
    API_AUDIO_CUSTOMIZED_PATCH_PARAM_ES_PASSTHROUGH_HDMI_SAMPLERATE             = 2,

    //---PCM Capture set rptr---
    API_AUDIO_CUSTOMIZED_PATCH_PARAM_PCM_CAPTURE1_RPTR                          = 3,
    API_AUDIO_CUSTOMIZED_PATCH_PARAM_PCM_CAPTURE2_RPTR                          = 4,

    //--- HDMI RX Bypass---
    API_AUDIO_CUSTOMIZED_PATCH_PARAM_HDMI_RX_BYPASS_ENABLE                      = 5,

    //---Low Latency Limiter---
    API_AUDIO_CUSTOMIZED_PATCH_PARAM_LOW_LATENCY_LIMITER_ENABLE                 = 6,
    API_AUDIO_CUSTOMIZED_PATCH_PARAM_LOW_LATENCY_LIMITER_ES_THRESHOLD           = 7,
    API_AUDIO_CUSTOMIZED_PATCH_PARAM_LOW_LATENCY_LIMITER_PCM_THRESHOLD          = 8,
    API_AUDIO_CUSTOMIZED_PATCH_PARAM_LOW_LATENCY_LIMITER_PCM_SKIP_TARGET        = 20,

    //---DOLBY_ATMOS_CONTROL---
    API_AUDIO_CUSTOMIZED_PATCH_PARAM_DOLBY_ATMOS_ENABLE                         = 17,
    API_AUDIO_CUSTOMIZED_PATCH_PARAM_DOLBY_ATMOS_DISABLE                        = 18,

    //---MCU Dump R2 Log---
    API_AUDIO_CUSTOMIZED_PATCH_PARAM_MCU_DUMP_R2_LOG_ENABLE                     = 21,
    API_AUDIO_CUSTOMIZED_PATCH_PARAM_MCU_DUMP_R2_LOG_OPTION                     = 22,

    //---ES Repeat Play---
    API_AUDIO_CUSTOMIZED_PATCH_PARAM_ES_REPLAY_PLAY_ENABLE                      = 23,

    //---HDMI_TX / SPDIF_TX SE-DSP Force DD/DDP Encode without Main sound---
    API_AUDIO_CUSTOMIZED_PATCH_PARAM_HDMI_TX_SEDSP_DDE_WO_MAIN_SND_ENABLE       = 24,
    API_AUDIO_CUSTOMIZED_PATCH_PARAM_HDMI_TX_SEDSP_DDPE_WO_MAIN_SND_ENABLE      = 25,
    API_AUDIO_CUSTOMIZED_PATCH_PARAM_SPDIF_TX_SEDSP_DDE_WO_MAIN_SND_ENABLE      = 26,
    API_AUDIO_CUSTOMIZED_PATCH_PARAM_SEDSP_MS12_ENC_WO_MAIN_SND_DISABLE         = 27,

    //---HDMI_ARC / SPDIF_TX SNDR2 Force DD/DDP Encode without Main sound---
    API_AUDIO_CUSTOMIZED_PATCH_PARAM_HDMI_ARC_SEDSP_DDE_WO_MAIN_SND_ENABLE      = 29,
    API_AUDIO_CUSTOMIZED_PATCH_PARAM_HDMI_ARC_SNDR2_DDPE_WO_MAIN_SND_ENABLE     = 30,
    API_AUDIO_CUSTOMIZED_PATCH_PARAM_SNDR2_MS12_ENC_WO_MAIN_SND_DISABLE         = 31,

    //---HDMI_RX_SWITCH_TO_PCM_DEBOUNCE---
    API_AUDIO_CUSTOMIZED_PATCH_PARAM_HDMI_RX_SWITCH_DEBOUNCE                    = 19,

    //---HDMI_RX_MONITOR_EVENT_CALLBACK_REGISTER---
    API_AUDIO_CUSTOMIZED_PATCH_PARAM_HDMI_RX_MONITOR_EVENT_CALLBACK_REGISTER    = 28,

    //---Frame Decode only cmd---
    API_AUDIO_CUSTOMIZED_PATCH_PARAM_FRAMEDECODE_CMD                            = 32,

    //---SDM L/R swap---
    API_AUDIO_CUSTOMIZED_PATCH_PARAM_SDM_SWAP_LINEOUT0                          = 33,
    API_AUDIO_CUSTOMIZED_PATCH_PARAM_SDM_SWAP_LINEOUT1                          = 34,
    API_AUDIO_CUSTOMIZED_PATCH_PARAM_SDM_SWAP_LINEOUT2                          = 35,
    API_AUDIO_CUSTOMIZED_PATCH_PARAM_SDM_SWAP_LINEOUT3                          = 36,

    //--------------------------------------------------------------------------------------
    // (API_AUDIO_"CUS*SE*"_PARAM_XXXXXXXX), PARAM: Set
    //--------------------------------------------------------------------------------------
    //---CUS1SE1---
    API_AUDIO_CUS1SE1_ENABLE                                                    = 9,
    API_AUDIO_CUS1SE1_AUTO_VOLUME_CTRL                                          = 10,
    API_AUDIO_CUS1SE1_WIDE_STEREO_CTRL                                          = 11,
    API_AUDIO_CUS1SE1_WALL_FILTER_CTRL                                          = 12,
    API_AUDIO_CUS1SE1_HEADROOM_GAIN_CTRL                                        = 13,
    API_AUDIO_CUS1SE1_GAIN_ADJUSTMENT_CTRL                                      = 14,
    API_AUDIO_CUS1SE1_HARD_LIMITER_CTRL                                         = 15,
    API_AUDIO_CUS1SE1_MODE_IDENTIFICATION_CTRL                                  = 16,
    //---CUS2SE1---
    API_AUDIO_CUS2SE1_PARAM_FN_MODE_REGISTER                                    = 37,
    API_AUDIO_CUS2SE1_PARAM_RESET                                               = 38,
    API_AUDIO_CUS2SE1_PARAM_INIT_FLAG                                           = 39,
    API_AUDIO_CUS2SE1_PARAM_FN_NUMBER_OF_OUTPUTS                                = 40,
    API_AUDIO_CUS2SE1_PARAM_FN_OUT_SAMPLING_FREQUENCY                           = 41,
    API_AUDIO_CUS2SE1_PARAM_FNMAIN_INIT                                         = 42,
    API_AUDIO_CUS2SE1_PARAM_FNMAIN_VARIABLES                                    = 43,
    API_AUDIO_CUS2SE1_PARAM_FN000_INIT                                          = 44,
    API_AUDIO_CUS2SE1_PARAM_FN000_VARIABLES                                     = 45,
    API_AUDIO_CUS2SE1_PARAM_FN001_INIT                                          = 46,
    API_AUDIO_CUS2SE1_PARAM_FN001_VARIABLES                                     = 47,
    API_AUDIO_CUS2SE1_PARAM_FN004_MODE1_VARIABLES                               = 48,
    API_AUDIO_CUS2SE1_PARAM_FN004_MODE2_VARIABLES                               = 49,
    API_AUDIO_CUS2SE1_PARAM_FN008_INIT                                          = 50,
    API_AUDIO_CUS2SE1_PARAM_FN008_VARIABLES                                     = 51,
    API_AUDIO_CUS2SE1_PARAM_FN009_INIT                                          = 52,
    API_AUDIO_CUS2SE1_PARAM_FN009_VARIABLES                                     = 53,
    API_AUDIO_CUS2SE1_PARAM_FN010_INIT                                          = 54,
    API_AUDIO_CUS2SE1_PARAM_FN010_VARIABLES                                     = 55,
    API_AUDIO_CUS2SE1_PARAM_FN010_OUT                                           = 56,
    API_AUDIO_CUS2SE1_PARAM_FN011_INIT                                          = 57,
    API_AUDIO_CUS2SE1_PARAM_FN011_VARIABLES                                     = 58,
    API_AUDIO_CUS2SE1_PARAM_FN014_INIT                                          = 59,
    API_AUDIO_CUS2SE1_PARAM_FN014_VARIABLES                                     = 60,
    API_AUDIO_CUS2SE1_PARAM_FN014_OUT                                           = 61,
    API_AUDIO_CUS2SE1_PARAM_FN016_INIT                                          = 62,
    API_AUDIO_CUS2SE1_PARAM_FN016_VARIABLES                                     = 63,
    API_AUDIO_CUS2SE1_PARAM_FN017_VARIABLES                                     = 64,
    API_AUDIO_CUS2SE1_PARAM_FN018_INIT                                          = 65,
    API_AUDIO_CUS2SE1_PARAM_FN018_VARIABLES                                     = 66,
    API_AUDIO_CUS2SE1_PARAM_FN019_INIT                                          = 67,
    API_AUDIO_CUS2SE1_PARAM_FN019_VARIABLES                                     = 68,
    API_AUDIO_CUS2SE1_PARAM_FN020_VARIABLES                                     = 69,
    API_AUDIO_CUS2SE1_PARAM_FN022_INIT                                          = 70,
    API_AUDIO_CUS2SE1_PARAM_FN022_VARIABLES                                     = 71,
    API_AUDIO_CUS2SE1_PARAM_FN023_INIT                                          = 72,
    API_AUDIO_CUS2SE1_PARAM_FN023_VARIABLES                                     = 73,
    API_AUDIO_CUS2SE1_PARAM_FN024_INIT                                          = 74,
    API_AUDIO_CUS2SE1_PARAM_FN024_VARIABLES                                     = 75,
    API_AUDIO_CUS2SE1_PARAM_FN026_INIT                                          = 76,
    API_AUDIO_CUS2SE1_PARAM_FN026_VARIABLES                                     = 77,
    API_AUDIO_CUS2SE1_PARAM_FN027_INIT                                          = 78,
    API_AUDIO_CUS2SE1_PARAM_FN027_VARIABLES                                     = 79,
    API_AUDIO_CUS2SE1_PARAM_FN028_VARIABLES                                     = 80,
    API_AUDIO_CUS2SE1_PARAM_FN029_VARIABLES                                     = 81, //<--MAX is here!!!

    //---MAX---
    API_AUDIO_CUSTOMIZED_PATCH_PARAM_MAX = API_AUDIO_CUS2SE1_PARAM_FN_OUT_SAMPLING_FREQUENCY + 1,
} API_AUDIO_CUSTOMIZED_PATCH_PARAM_TYPE;

typedef enum
{
    API_AUDIO_CUSTOMIZED_PATCH_INFO_TYPE_INVALID = -1,

    //--------------------------------------------------------------------------------------
    // (API_AUDIO_"CUSTOMIZED_PATCH"_INFO_XXXXXXXX), INFO: Get
    //--------------------------------------------------------------------------------------
    //---SPDIF npcm buffer get wptr---
    API_AUDIO_CUSTOMIZED_PATCH_INFO_SPDIF_NONPCM_WPTR           = 0,

    //---PCM Capture get wptr---
    API_AUDIO_CUSTOMIZED_PATCH_INFO_PCM_CAPTURE1_WPTR           = 1,
    API_AUDIO_CUSTOMIZED_PATCH_INFO_PCM_CAPTURE2_WPTR           = 2,

    //---R2 UART info---
    API_AUDIO_CUSTOMIZED_PATCH_INFO_R2_UART_BUF_BASE            = 3,
    API_AUDIO_CUSTOMIZED_PATCH_INFO_R2_UART_BUF_SIZE            = 4,
    API_AUDIO_CUSTOMIZED_PATCH_INFO_R2_UART_BUF_WPTR            = 5,

    //---ADEC PCM buffer info---
    API_AUDIO_CUSTOMIZED_PATCH_INFO_ADEC_PCM_BUF_BASE           = 6,
    API_AUDIO_CUSTOMIZED_PATCH_INFO_ADEC_PCM_BUF_SIZE           = 7,
    API_AUDIO_CUSTOMIZED_PATCH_INFO_ADEC_PCM_BUF_WPTR           = 8,

    //---SPDIF NPCM buffer info---
    API_AUDIO_CUSTOMIZED_PATCH_INFO_SPDIF_NPCM_BUF_BASE         = 22,
    API_AUDIO_CUSTOMIZED_PATCH_INFO_SPDIF_NPCM_BUF_SIZE         = 23,
    API_AUDIO_CUSTOMIZED_PATCH_INFO_SPDIF_NPCM_BUF_WPTR         = 24,

    //---HDMI NPCM buffer info---
    API_AUDIO_CUSTOMIZED_PATCH_INFO_HDMI_NPCM_BUF_BASE          = 9,
    API_AUDIO_CUSTOMIZED_PATCH_INFO_HDMI_NPCM_BUF_SIZE          = 10,
    API_AUDIO_CUSTOMIZED_PATCH_INFO_HDMI_NPCM_BUF_RPTR          = 11,
    API_AUDIO_CUSTOMIZED_PATCH_INFO_HDMI_NPCM_BUF_WPTR          = 12,

    //---MS12 META DATA buffer info---
    API_AUDIO_CUSTOMIZED_PATCH_INFO_MS12_DDPE_METADATA_BUF_BASE = 13,
    API_AUDIO_CUSTOMIZED_PATCH_INFO_MS12_DDPE_METADATA_BUF_SIZE = 14,
    API_AUDIO_CUSTOMIZED_PATCH_INFO_MS12_DDPE_METADATA_BUF_WPTR = 15,

    //---ADEC ES buffer info---
    API_AUDIO_CUSTOMIZED_PATCH_INFO_ADEC_ES_BUF_BASE            = 16,
    API_AUDIO_CUSTOMIZED_PATCH_INFO_ADEC_ES_BUF_SIZE            = 17,
    API_AUDIO_CUSTOMIZED_PATCH_INFO_ADEC_ES_BUF_RPTR            = 18,
    API_AUDIO_CUSTOMIZED_PATCH_INFO_ADEC_ES_BUF_WPTR            = 19,

    //---ADEC AC3 ES Dump---
    API_AUDIO_CUSTOMIZED_PATCH_INFO_ADEC_AC3_ES_DUMP_BASE       = 30,
    API_AUDIO_CUSTOMIZED_PATCH_INFO_ADEC_AC3_ES_DUMP_SIZE       = 31,
    API_AUDIO_CUSTOMIZED_PATCH_INFO_ADEC_AC3_ES_DUMP_PTR        = 32,

    //---Pipeline Delay---
    API_AUDIO_CUSTOMIZED_PATCH_INFO_SPDIFTX_PIPELINE_DELAY      = 20,
    API_AUDIO_CUSTOMIZED_PATCH_INFO_HDMITX_PIPELINE_DELAY       = 21,

    //---SW DMA1 info---
    API_AUDIO_CUSTOMIZED_PATCH_INFO_SWDMA1_SAMPLE_PERCHANNEL    = 28,

    //---MCH SW DMA info---
    API_AUDIO_CUSTOMIZED_PATCH_INFO_MCH_SWDMA_SAMPLE_PERCHANNEL = 25,

    //---Frame decode pcm info---
    API_AUDIO_CUSTOMIZED_PATCH_INFO_FRAMEDECODE_PCMSIZE         = 33,
    API_AUDIO_CUSTOMIZED_PATCH_INFO_FRAMEDECODE_BUFFER          = 34,

    //--------------------------------------------------------------------------------------
    // (API_AUDIO_"CUS*SE*"_INFO_XXXXXXXX), PARAM: Set
    //--------------------------------------------------------------------------------------
    //---CUS2SE1---
    API_AUDIO_CUS2SE1_INFO_FN_MODE_REGISTER                     = 35,
    API_AUDIO_CUS2SE1_INFO_RESET                                = 36,
    API_AUDIO_CUS2SE1_INFO_INIT_FLAG                            = 37,
    API_AUDIO_CUS2SE1_INFO_FN_NUMBER_OF_OUTPUTS                 = 38,
    API_AUDIO_CUS2SE1_INFO_FN_OUT_SAMPLING_FREQUENCY            = 39,
    API_AUDIO_CUS2SE1_INFO_FNMAIN_INIT                          = 40,
    API_AUDIO_CUS2SE1_INFO_FNMAIN_VARIABLES                     = 41,
    API_AUDIO_CUS2SE1_INFO_FN000_INIT                           = 42,
    API_AUDIO_CUS2SE1_INFO_FN000_VARIABLES                      = 43,
    API_AUDIO_CUS2SE1_INFO_FN001_INIT                           = 44,
    API_AUDIO_CUS2SE1_INFO_FN001_VARIABLES                      = 45,
    API_AUDIO_CUS2SE1_INFO_FN004_MODE1_VARIABLES                = 46,
    API_AUDIO_CUS2SE1_INFO_FN004_MODE2_VARIABLES                = 47,
    API_AUDIO_CUS2SE1_INFO_FN008_INIT                           = 48,
    API_AUDIO_CUS2SE1_INFO_FN008_VARIABLES                      = 49,
    API_AUDIO_CUS2SE1_INFO_FN009_INIT                           = 50,
    API_AUDIO_CUS2SE1_INFO_FN009_VARIABLES                      = 51,
    API_AUDIO_CUS2SE1_INFO_FN010_INIT                           = 52,
    API_AUDIO_CUS2SE1_INFO_FN010_VARIABLES                      = 53,
    API_AUDIO_CUS2SE1_INFO_FN010_OUT                            = 54,
    API_AUDIO_CUS2SE1_INFO_FN011_INIT                           = 55,
    API_AUDIO_CUS2SE1_INFO_FN011_VARIABLES                      = 56,
    API_AUDIO_CUS2SE1_INFO_FN014_INIT                           = 57,
    API_AUDIO_CUS2SE1_INFO_FN014_VARIABLES                      = 58,
    API_AUDIO_CUS2SE1_INFO_FN014_OUT                            = 59,
    API_AUDIO_CUS2SE1_INFO_FN016_INIT                           = 60,
    API_AUDIO_CUS2SE1_INFO_FN016_VARIABLES                      = 61,
    API_AUDIO_CUS2SE1_INFO_FN017_VARIABLES                      = 62,
    API_AUDIO_CUS2SE1_INFO_FN018_INIT                           = 63,
    API_AUDIO_CUS2SE1_INFO_FN018_VARIABLES                      = 64,
    API_AUDIO_CUS2SE1_INFO_FN019_INIT                           = 65,
    API_AUDIO_CUS2SE1_INFO_FN019_VARIABLES                      = 66,
    API_AUDIO_CUS2SE1_INFO_FN020_VARIABLES                      = 67,
    API_AUDIO_CUS2SE1_INFO_FN022_INIT                           = 68,
    API_AUDIO_CUS2SE1_INFO_FN022_VARIABLES                      = 69,
    API_AUDIO_CUS2SE1_INFO_FN023_INIT                           = 70,
    API_AUDIO_CUS2SE1_INFO_FN023_VARIABLES                      = 71,
    API_AUDIO_CUS2SE1_INFO_FN024_INIT                           = 72,
    API_AUDIO_CUS2SE1_INFO_FN024_VARIABLES                      = 73,
    API_AUDIO_CUS2SE1_INFO_FN026_INIT                           = 74,
    API_AUDIO_CUS2SE1_INFO_FN026_VARIABLES                      = 75,
    API_AUDIO_CUS2SE1_INFO_FN027_INIT                           = 76,
    API_AUDIO_CUS2SE1_INFO_FN027_VARIABLES                      = 77,
    API_AUDIO_CUS2SE1_INFO_FN028_VARIABLES                      = 78,
    API_AUDIO_CUS2SE1_INFO_FN029_VARIABLES                      = 79, //<--MAX is here!!! (26,27,29 available)

    //---MAX---
    API_AUDIO_CUSTOMIZED_PATCH_INFO_MAX = API_AUDIO_CUSTOMIZED_PATCH_INFO_FRAMEDECODE_BUFFER + 1,
} API_AUDIO_CUSTOMIZED_PATCH_INFO_TYPE;

//---CUS2SE1---
typedef struct
{
    MS_U32 u32Data;
    MS_U32 *pData;
    MS_U16 u16Data;
} API_AUDIO_CUS2SE1_DATA;

//-------------------------------------------------------------------------------------------------------------------------------------
/* Clip Play for ES */
//-------------------------------------------------------------------------------------------------------------------------------------
typedef struct
{
    API_AUDIO_CUSTOMER_CODEC_TYPE   clipType;
    MS_U32                          repeatNumber;       /* The play number of audio clip. */
} API_AUDIO_CUSTOMER_CLIP_DEC_PARAM;

typedef enum
{
    API_AUDIO_CUSTOMER_CLIP_STATUS_INVALID  = -1,
    API_AUDIO_CUSTOMER_CLIP_NONE            = 0,
    API_AUDIO_CUSTOMER_CLIP_PLAY            = 1,
    API_AUDIO_CUSTOMER_CLIP_STOP            = 2,
    API_AUDIO_CUSTOMER_CLIP_RESUME          = 3,
    API_AUDIO_CUSTOMER_CLIP_PAUSE           = 4,
    API_AUDIO_CUSTOMER_CLIP_DONE            = 5,
} API_AUDIO_CUSTOMER_CLIP_STATUS;

//-------------------------------------------------------------------------------------------------------------------------------------
/* Clip Play for PCM */
//-------------------------------------------------------------------------------------------------------------------------------------
typedef enum
{
    API_AUDIO_CUSTOMER_MIXER_INDEX_INVALID  = -1,
    API_AUDIO_CUSTOMER_MIXER0               = 0,
    API_AUDIO_CUSTOMER_MIXER1               = 1,
    API_AUDIO_CUSTOMER_MIXER2               = 2,
    API_AUDIO_CUSTOMER_MIXER3               = 3,
    API_AUDIO_CUSTOMER_MIXER4               = 4,
    API_AUDIO_CUSTOMER_MIXER5               = 5,
    API_AUDIO_CUSTOMER_MIXER6               = 6,
    API_AUDIO_CUSTOMER_MIXER7               = 7,
    API_AUDIO_CUSTOMER_MIXER_MAX            = API_AUDIO_CUSTOMER_MIXER7 + 1,
} API_AUDIO_CUSTOMER_MIXER_INDEX;

typedef enum
{
    API_AUDIO_CUSTOMER_PCM_ENDIAN_INVALID   = -1,
    API_AUDIO_CUSTOMER_PCM_LITTLE_ENDIAN    = 0,
    API_AUDIO_CUSTOMER_PCM_BIG_ENDIAN       = 1,
} API_AUDIO_CUSTOMER_PCM_ENDIAN;

typedef enum
{
    API_AUDIO_CUSTOMER_PCM_SIGNED_INVALID   = -1,
    API_AUDIO_CUSTOMER_PCM_SIGNED           = 0,
    API_AUDIO_CUSTOMER_PCM_UNSIGNED         = 1,
} API_AUDIO_CUSTOMER_PCM_SIGNED_STATUS;

typedef struct
{
    MS_U32                                  numOfChannel;       /* 2  : stereo, 1 : mono,  8 : 8 channel */
    MS_U32                                  bitPerSample;       /* 16 : 16 bit, 8 : 8 bit 24 : 24bit */
    API_AUDIO_CUSTOMER_SAMPLING_FREQ        samplingFreq;       /* 48000 : 48Khz, 44100 : 44.1Khz */
    API_AUDIO_CUSTOMER_PCM_ENDIAN           endianType;         /* 0  : little endian, 1 : big endian */
    API_AUDIO_CUSTOMER_PCM_SIGNED_STATUS    signedType;         /* 0  : signed PCM, 1 : unsigned PCM */
    MS_U32                                  repeatNumber;       /* The play number of audio clip. */
} API_AUDIO_CUSTOMER_CLIP_MIX_PARAM;

//-------------------------------------------------------------------------------------------------------------------------------------
/* Gain, Mute & Delay */
//-------------------------------------------------------------------------------------------------------------------------------------
typedef enum
{
    API_AUDIO_CUSTOMER_ADMIX_GAIN_IDX_INVALID = -1,
    API_AUDIO_CUSTOMER_ADMIX_MD_GAIN          = 0,
    API_AUDIO_CUSTOMER_ADMIX_AD_GAIN          = 1,
    API_AUDIO_CUSTOMER_ADMIX_GAIN_IDX_MAX     = API_AUDIO_CUSTOMER_ADMIX_AD_GAIN + 1,
} API_AUDIO_CUSTOMER_ADMIX_GAIN_IDX;

typedef enum
{
    API_AUDIO_CUSTOMER_LINE_OUT_INVALID    = -1,
    API_AUDIO_CUSTOMER_LINE_OUT_0          = 0,
    API_AUDIO_CUSTOMER_LINE_OUT_1          = 1,
    API_AUDIO_CUSTOMER_LINE_OUT_2          = 2,
    API_AUDIO_CUSTOMER_LINE_OUT_3          = 3,
    API_AUDIO_CUSTOMER_LINE_OUT_MAX        = API_AUDIO_CUSTOMER_LINE_OUT_3 + 1,
} API_AUDIO_CUSTOMER_LINE_OUT_IDX;

//-------------------------------------------------------------------------------------------------------------------------------------
/* SPK Out LR Mode */
//-------------------------------------------------------------------------------------------------------------------------------------
typedef enum
{
    API_AUDIO_CUSTOMER_SNDOUT_LRMODE_INVALID    = -1,
    API_AUDIO_CUSTOMER_SNDOUT_LRMODE_LR         = 0,
    API_AUDIO_CUSTOMER_SNDOUT_LRMODE_LL         = 1,
    API_AUDIO_CUSTOMER_SNDOUT_LRMODE_RR         = 2,
    API_AUDIO_CUSTOMER_SNDOUT_LRMODE_MIX        = 3,
} API_AUDIO_CUSTOMER_SNDOUT_LRMODE;

//-------------------------------------------------------------------------------------------------------------------------------------
/* AENC */
//-------------------------------------------------------------------------------------------------------------------------------------
typedef enum
{
    API_AUDIO_CUSTOMER_AENC_INDEX_INVALID   = -1,
    API_AUDIO_CUSTOMER_AENC0                = 0,
    API_AUDIO_CUSTOMER_AENC1                = 1,
    API_AUDIO_CUSTOMER_AENC_MAX             = API_AUDIO_CUSTOMER_AENC1 + 1,
} API_AUDIO_CUSTOMER_AENC_INDEX;

typedef enum
{
    API_AUDIO_CUSTOMER_AENC_STATUS_INVALID      = -1,
    API_AUDIO_CUSTOMER_AENC_STATUS_STOP         = 0,
    API_AUDIO_CUSTOMER_AENC_STATUS_PLAY         = 1,
    API_AUDIO_CUSTOMER_AENC_STATUS_ABNORMAL     = 2,
} API_AUDIO_CUSTOMER_AENC_STATUS;

typedef enum
{
    API_AUDIO_CUSTOMER_AENC_ENCODING_FORMAT_INVALID = -1,
    API_AUDIO_CUSTOMER_AENC_ENCODE_MP3              = 0,    /* Encode MP3 format */
    API_AUDIO_CUSTOMER_AENC_ENCODE_AAC              = 1,    /* Encode AAC format */
} API_AUDIO_CUSTOMER_AENC_ENCODING_FORMAT;

typedef enum
{
    API_AUDIO_CUSTOMER_AENC_CHANNEL_INVALID = -1,
    API_AUDIO_CUSTOMER_AENC_MONO            = 0,
    API_AUDIO_CUSTOMER_AENC_STEREO          = 1,
} API_AUDIO_CUSTOMER_AENC_CHANNEL;

typedef enum
{
    API_AUDIO_CUSTOMER_AENC_BITRATE_INVALID = -1,
    API_AUDIO_CUSTOMER_AENC_BIT_48K         = 0,
    API_AUDIO_CUSTOMER_AENC_BIT_56K         = 1,
    API_AUDIO_CUSTOMER_AENC_BIT_64K         = 2,
    API_AUDIO_CUSTOMER_AENC_BIT_80K         = 3,
    API_AUDIO_CUSTOMER_AENC_BIT_112K        = 4,
    API_AUDIO_CUSTOMER_AENC_BIT_128K        = 5,
    API_AUDIO_CUSTOMER_AENC_BIT_160K        = 6,
    API_AUDIO_CUSTOMER_AENC_BIT_192K        = 7,
    API_AUDIO_CUSTOMER_AENC_BIT_224K        = 8,
    API_AUDIO_CUSTOMER_AENC_BIT_256K        = 9,
    API_AUDIO_CUSTOMER_AENC_BIT_320K        = 10,
} API_AUDIO_CUSTOMER_AENC_BITRATE;

typedef struct
{
    //Get Info for debugging on DDI
    API_AUDIO_CUSTOMER_AENC_STATUS            status;   // current ENC Status
    API_AUDIO_CUSTOMER_AENC_ENCODING_FORMAT   codec;    // current ENC Codec

    MS_U32  errorCount;                                 // current ENC error counter
    MS_U32  inputCount;                                 // current ENC input counter - we distinguish whether or not enter data from input.
    MS_U32  underflowCount;                             // current ENC underflowCnt in kernel space - we distinguish which module have problem between muxer and encdoer
    MS_U32  overflowCount;                              // current ENC overflowCnt - we distinguish

    //Set Info - it is applied realtime, no matter stop&start
    API_AUDIO_CUSTOMER_AENC_CHANNEL   channel;          // number of channel
    API_AUDIO_CUSTOMER_AENC_BITRATE   bitrate;          // bitrate
} API_AUDIO_CUSTOMER_AENC_INFO;

//-------------------------------------------------------------------------------------------------------------------------------------
/* PCM(Sound Bar Buletooth, PCM Capture) */
//-------------------------------------------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------------------------------------------
/* PCM IO */
//-------------------------------------------------------------------------------------------------------------------------------------
typedef struct
{
    MS_U8 u8Name[32];           /* The PCM name requested (only 32 bytes, if change this length, must change u8Name of AUDIO_PCM_INFO_t as well!) */
    MS_U8 u8MultiChFlag;        /* it is TRUE when request for a multi-channel PCM IO */
    MS_U32 u32BufferDuration;   /* PCM buffer size in mini-second unit, and up to a maximun size */
    MS_U32 u32Channel;          /* PCM channel number */
    MS_U32 u32SampleRate;       /* PCM sample rate */
    MS_U32 u32BitWidth;         /* PCM sample's bit width of each channel */
    MS_U32 u32BigEndian;        /* PCM endien, TRUE means Big Endien, FALSE means Little Endien */
} API_AUDIO_CUSTOMER_PCMIO_PARAM;

typedef enum
{
    API_AUDIO_CUSTOMER_PCM_CMD_ALL,             /*       GET      */
    API_AUDIO_CUSTOMER_PCM_CMD_NONBLOCKING,     /* SET / GET      */
    API_AUDIO_CUSTOMER_PCM_CMD_MULTICH,         /*       GET      */
    API_AUDIO_CUSTOMER_PCM_CMD_MIXING,          /*       GET      */
    API_AUDIO_CUSTOMER_PCM_CMD_MIXINGGROUP,     /* SET / GET      */
    API_AUDIO_CUSTOMER_PCM_CMD_BUFFER,          /*       GET only */
    API_AUDIO_CUSTOMER_PCM_CMD_BUFFERDURATION,  /* SET / GET      */
    API_AUDIO_CUSTOMER_PCM_CMD_READPTR,         /*       GET only */
    API_AUDIO_CUSTOMER_PCM_CMD_WRITEPTR,        /*       GET only */
    API_AUDIO_CUSTOMER_PCM_CMD_CHANNEL,         /* SET / GET      */
    API_AUDIO_CUSTOMER_PCM_CMD_SAMPLERATE,      /* SET / GET      */
    API_AUDIO_CUSTOMER_PCM_CMD_BITWIDTH,        /* SET / GET      */
    API_AUDIO_CUSTOMER_PCM_CMD_BIGENDIAN,       /* SET / GET      */
    API_AUDIO_CUSTOMER_PCM_CMD_TIMESTAMP,       /* SET / GET      */
    API_AUDIO_CUSTOMER_PCM_CMD_WEIGHTING,       /* SET / GET      */
    API_AUDIO_CUSTOMER_PCM_CMD_VOLUME,          /* SET / GET      */
    API_AUDIO_CUSTOMER_PCM_CMD_BUFFERLEVEL,     /*       GET only */
    API_AUDIO_CUSTOMER_PCM_CMD_MUTE,            /* SET / GET      */
    API_AUDIO_CUSTOMER_PCM_CMD_BUFFERSIZE,      /*       GET only */
} API_AUDIO_CUSTOMER_PCM_COMMAND;

//-------------------------------------------------------------------------------------------------------------------------------------
/* MM New Mode */
//-------------------------------------------------------------------------------------------------------------------------------------
typedef enum
{
    API_AUDIO_CUSTOMER_MEMORY_BASE = 0x0, ///< for audio DDR Base
    API_AUDIO_CUSTOMER_MEMORY_SIZE = 0x1, ///< for audio DDR Size
} API_AUDIO_CUSTOMER_DDRINFO;

typedef struct
{
    MS_U32 aes_base_addr;       ///<  base address of ES buffer
    MS_U32 aes_end_addr;        ///<  end address of ES buffer
    MS_U32 aes_BufSize;         ///<  ES buffer size
    MS_U32 aes_write_addr;      ///<  write pointer
    MS_U32 aes_read_addr;       ///<  read pointer
    MS_U32 aes_freeSpace;       ///<  free space of ES buffer
    MS_U32 aes_level;           ///<  level of ES buffer
    MS_U32 aes_ptsCnt;          ///<  count of total received PTS
    MS_U64 aes_lastPTS;         ///<  last received PTS
} API_AUDIO_CUSTOMER_AES_INFO;

//-------------------------------------------------------------------------------------------------------------------------------------
/* Mstar Sound Effect */
//-------------------------------------------------------------------------------------------------------------------------------------
typedef enum
{
    API_AUDIO_CUSTOMER_GEQ,
    API_AUDIO_CUSTOMER_PEQ,
    API_AUDIO_CUSTOMER_HPF,
    API_AUDIO_CUSTOMER_DRC,
    API_AUDIO_CUSTOMER_AVC,
    API_AUDIO_CUSTOMER_BALANCE,
    API_AUDIO_CUSTOMER_MSTAR_SE_MAX,
} API_AUDIO_CUSTOMER_MSTAR_SE_TYPE;

typedef struct
{
    MS_U8   band;   ///< band
    MS_U32  a0;     ///< a0
    MS_U32  a1;     ///< a1
    MS_U32  a2;     ///< a2
    MS_U32  b1;     ///< b1
    MS_U32  b2;     ///< b2
    MS_BOOL enable; ///< PEQ band enable
    MS_U8   scale;  ///< scale
} API_AUDIO_CUSTOMER_MSTAR_PEQ_COEF;

typedef struct
{
    MS_U32  a0;     ///< a0
    MS_U32  a1;     ///< a1
    MS_U32  a2;     ///< a2
    MS_U32  b1;     ///< b1
    MS_U32  b2;     ///< b2
} API_AUDIO_CUSTOMER_MSTAR_HPF_COEF;

typedef enum
{
    API_AUDIO_CUSTOMER_AVC_LINEAR,
    API_AUDIO_CUSTOMER_AVC_SHIFT,
    API_AUDIO_CUSTOMER_AVC_NORMAL,
    API_AUDIO_CUSTOMER_AVC_R,
} API_AUDIO_CUSTOMER_MSTAR_AVC_MODE;

//-------------------------------------------------------------------------------------------------------------------------------------
/* Advanced Sound Effect */
//-------------------------------------------------------------------------------------------------------------------------------------

                                                                                                            // Description:
typedef enum                                                                                                // Enable SRS Main function, Only when the hashkey is correct,
{                                                                                                           // the SRS main function will be activated sucessfully.
    API_AUDIO_CUSTOMER_DTS_SE_INVALID           = -1,                                                       // ============================================================================
    API_AUDIO_CUSTOMER_DTS_SE_TSXT              = 0,                                                        // for SRS TSXT Hashkey
    API_AUDIO_CUSTOMER_DTS_SE_TSHD              = 1,                                                        // for SRS TSHD Hashkey
    API_AUDIO_CUSTOMER_DTS_SE_THEATERSOUND      = 2,                                                        // for SRS TheaterSound HD Hashkey
    API_AUDIO_CUSTOMER_DTS_SE_PURESND           = 3,                                                        // for SRS Pure Sound Hashkey or for DTS StudioSound 3D Hashkey.
    API_AUDIO_CUSTOMER_DTS_SE_STUDIOSOUND_3D    = 4,                                                        // for DTS StudioSound 3D Hashkey
    API_AUDIO_CUSTOMER_DTS_SE_DISABLE           = 5, /*<-MAX*/                                              // for DISABLE all DTS SE
    API_AUDIO_CUSTOMER_DTS_SE_MAX               = API_AUDIO_CUSTOMER_DTS_SE_DISABLE + 1,
} API_AUDIO_CUSTOMER_DTS_SE_TYPE;

                                                                                                            //  ____________________________________________________________________________________________________________
                                                                                                            // |Hashkey:                                                 |Description:
                                                                                                            // |_____________________________________ ___________________|__________________________________________________
typedef enum                                                                                                // |SRS_TSXT|SRS_TSHD|SRS_TheaterSound_HD|SRS_Puresound_Only |Enable/Disable
{                                                                                                           // |        |        |DTS_StudioSound_3D |(SE-DSP)           |for the following:
    API_AUDIO_CUSTOMER_DTS_SE_UNIT_INVALID                                          = -1,                   // |________|________|___________________|___________________|
                                                                                                            // |           TSHD                                          |
                                                                                                            // |________ ________ ___________________ ___________________|__________________________________________________
    API_AUDIO_CUSTOMER_DTS_SE_UNIT_THEATERSOUND_TOTAL                               = 0,                    // |   V    |   V    |        V          |                   |DTS StudioSound main function
    API_AUDIO_CUSTOMER_DTS_SE_UNIT_THEATERSOUND_TSHD                                = 1,                    // |   V    |   V    |        V          |                   |TruSurround HD
    API_AUDIO_CUSTOMER_DTS_SE_UNIT_THEATERSOUND_TSHD_DEFINITION                     = 2,                    // |        |   V    |        V          |                   |Definition processing component of TruSurround HD
    API_AUDIO_CUSTOMER_DTS_SE_UNIT_THEATERSOUND_TSHD_DC                             = 3,                    // |   V    |   V    |        V          |                   |Dialog Clarity processing component of TruSurround HD
    API_AUDIO_CUSTOMER_DTS_SE_UNIT_THEATERSOUND_TSHD_SURR                           = 4,                    // |   V    |   V    |        V          |                   |Surround processing component of TruSurround HD
    API_AUDIO_CUSTOMER_DTS_SE_UNIT_THEATERSOUND_TSHD_TRUBASS                        = 5,                    // |   V    |   V    |        V          |                   |TruBass processing component of TruSurround HD
    API_AUDIO_CUSTOMER_DTS_SE_UNIT_THEATERSOUND_TSHD_TRUBASS_LVL_INDPT              = 6,                    // |   V    |   V    |        V          |                   |Level Independent processing component of TruBass
                                                                                                            // |________|________|___________________|___________________|
                                                                                                            // |     CC3D Controls (CC3D must enable)                    |
                                                                                                            // |________ ________ ___________________ ___________________|
    API_AUDIO_CUSTOMER_DTS_SE_UNIT_THEATERSOUND_CC3D                                = 7,                    // |   V    |   V    |        V          |                   |CC3D processing
    API_AUDIO_CUSTOMER_DTS_SE_UNIT_THEATERSOUND_CC3D_DEPTH_PROCESS                  = 8,                    // |        |        |        V          |                   |Depth processing in CC3D
                                                                                                            // |________|________|___________________|___________________|
                                                                                                            // |       TRUVOLUME HD                                      |
                                                                                                            // |________ ________ ___________________ ___________________|
    API_AUDIO_CUSTOMER_DTS_SE_UNIT_THEATERSOUND_TRUVOLUME                           = 9,                    // |        |        |        V          |                   |TruVolume processing
    API_AUDIO_CUSTOMER_DTS_SE_UNIT_THEATERSOUND_TRUVOLUME_NORMALIZER                = 10,                   // |        |        |        V          |                   |it will detect the level of the incoming signal and if necessary, dynamically boosts the signal level to ensure that it is always within the operational range of TruVolume.
                                                                                                            // |________|________|___________________|___________________|
                                                                                                            // |         TRUDIALOG                                       |
                                                                                                            // |_____________________________________ ___________________|
    API_AUDIO_CUSTOMER_DTS_SE_UNIT_THEATERSOUND_TRUDIALOG                           = 11,                   // |        |        |        V          |                   |TruDialog processing
                                                                                                            // |________|________|___________________|___________________|
                                                                                                            // |           TBHDX                                         |
                                                                                                            // |________ ________ ___________________ ___________________|
    API_AUDIO_CUSTOMER_DTS_SE_UNIT_THEATERSOUND_CC3D_TBHDX                          = 12,                   // |        |        |        V          |                   |TruBass HDX low frequency compensation processing
                                                                                                            // |________|________|___________________|___________________|
                                                                                                            // |            GEQ                                          |
                                                                                                            // |________ ________ ___________________ ___________________|
    API_AUDIO_CUSTOMER_DTS_SE_UNIT_THEATERSOUND_GEQ                                 = 13,                   // |        |        |        V          |                   |Graphic EQ processing
                                                                                                            // |________|________|___________________|___________________|
                                                                                                            // |    PURESOUND HRADLIMITER                                |
                                                                                                            // |________ ________ ___________________ ___________________|
    API_AUDIO_CUSTOMER_DTS_SE_UNIT_PURESOUND_HL                                     = 14,                   // |        |   V    |        V          |        V          |HardLimiter processing
                                                                                                            // |________|________|___________________|___________________|
                                                                                                            // |       PURESOUND AEQ                                     |
                                                                                                            // |________ ________ ___________________ ___________________|
    API_AUDIO_CUSTOMER_DTS_SE_UNIT_PURESOUND_AEQ                                    = 15,                   // |        |        |        V          |        V          |Aective EQ processing
                                                                                                            // |________|________|___________________|___________________|
                                                                                                            // |       PURESOUND HPF                                     |
                                                                                                            // |________ ________ ___________________ ___________________|
    API_AUDIO_CUSTOMER_DTS_SE_UNIT_PURESOUND_HPF                                    = 16,                   // |        |        |                   |                   |High Pass Filter processing
                                                                                                            // |________|________|___________________|___________________|
                                                                                                            // |       PURESOUND TBHD                                    |
                                                                                                            // |________ ________ ___________________ ___________________|
    API_AUDIO_CUSTOMER_DTS_SE_UNIT_PURESOUND_TBHD                                   = 17, /*<-MAX*/         // |        |        |                   |        V          |TruBass in SE-DSP
                                                                                                            // |________|________|___________________|___________________|

    API_AUDIO_CUSTOMER_DTS_SE_UNIT_MAX = API_AUDIO_CUSTOMER_DTS_SE_UNIT_PURESOUND_TBHD + 1,
} API_AUDIO_CUSTOMER_DTS_SE_UNIT_TYPE;

                                                                                                            //  _________________________________________________________
                                                                                                            // |Hashkey:                                                 |
                                                                                                            // |________ ________ ___________________ ___________________|__________________________________________________________________________________________________________
typedef enum                                                                                                // |SRS_TSXT|SRS_TSHD|SRS_TheaterSound_HD|SRS_Puresound_Only |
{                                                                                                           // |        |        |DTS_StudioSound_3D |(SE-DSP)           |Default               Range                               Description
    API_AUDIO_CUSTOMER_DTS_SE_PARAM_INVALID                                             = -1,               // |________|________|___________________|___________________|__________________________________________________________________________________________________________
                                                                                                            // |           TSHD                                          |
                                                                                                            // |________ ________ ___________________ ___________________|
    API_AUDIO_CUSTOMER_DTS_SE_PARAM_THEATERSOUND_TSHD_INPUT_GAIN_FXP24                  = 0,                // |   V    |   V    |        V          |                   |1.0                   0x0 ~ 0x7FFFFF (0.0 ~ 1.0)          Trusurround input gain. Input Gain. Adjusts the level of the signal at the input to TSHD processing.
    API_AUDIO_CUSTOMER_DTS_SE_PARAM_THEATERSOUND_TSHD_OUTPUT_GAIN_FXP24                 = 50,               // |   V    |   V    |        V          |                   |1.0                   0x0 ~ 0x7FFFFF (0.0 ~ 1.0)          Trusurround output gain. Output Gain. Adjusts the level of the signal at the output to TSHD processing.
    API_AUDIO_CUSTOMER_DTS_SE_PARAM_THEATERSOUND_TSHD_DEFINITION_CTRL_FXP24             = 1,                // |        |   V    |        V          |                   |0.6                   0x0 ~ 0x7FFFFF (0.0 ~ 1.0)          Trusurround definition control. Definition Level. Controls the level of high frequency enhancement.
    API_AUDIO_CUSTOMER_DTS_SE_PARAM_THEATERSOUND_TSHD_DC_LEVEL_CTRL_FXP24               = 2,                // |   V    |   V    |        V          |                   |0.5                   0x0 ~ 0x7FFFFF (0.0 ~ 1.0)          Trusurround dialog clarity control. Dialog Clarity Level. Establishes the amount of dialog clarity enhancement that is applied to the audio signal.
    API_AUDIO_CUSTOMER_DTS_SE_PARAM_THEATERSOUND_TSHD_SURR_LEVEL_CTRL_FXP24             = 3,                // |   V    |   V    |        V          |                   |0.6                   0x0 ~ 0x7FFFFF (0.0 ~ 1.0)          Trusurround surround level. Surround Level. Controls the overall mix level of the surround channels.
    API_AUDIO_CUSTOMER_DTS_SE_PARAM_THEATERSOUND_TSHD_TRUBASS_CTRL_FXP24                = 4,                // |   V    |   V    |        V          |                   |0.3                   0x0 ~ 0x7FFFFF (0.0 ~ 1.0)          Trubass HD front level control
    API_AUDIO_CUSTOMER_DTS_SE_PARAM_THEATERSOUND_TSHD_TRUBASS_COMPR_CTRL_FXP24          = 5,                // |   V    |   V    |        V          |                   |0.25                  0x0 ~ 0x7FFFFF (0.0 ~ 1.0)          Trubass HD compressor control
    API_AUDIO_CUSTOMER_DTS_SE_PARAM_THEATERSOUND_TSHD_TRUBASS_PROCESS_MODE              = 6,                // |   V    |   V    |        V          |                   |1:stereo              0:mono  1:stereo                    Trubass HD process mode
    API_AUDIO_CUSTOMER_DTS_SE_PARAM_THEATERSOUND_TSHD_TRUBASS_SPKSIZE                   = 7,                // |   V    |   V    |        V          |                   |8:Custom              0 ~ 8                               Trubass HD speaker size. 0:40Hz, 1:60Hz, 2:100Hz, 3:150Hz, 4:200Hz, 5:250Hz, 6:300Hz, 7:400Hz, 8:Custom (default), Trubass HD speaker size
    API_AUDIO_CUSTOMER_DTS_SE_PARAM_THEATERSOUND_TSHD_TRUBASS_CUS_SPKSIZE               = 8,                // |   V    |   V    |        V          |                   |13:150Hz              0 ~ 38 (20Hz ~ 400 Hz, step:10Hz)   Trubass HD customer audio speaker size
    API_AUDIO_CUSTOMER_DTS_SE_PARAM_THEATERSOUND_TSHD_TRUBASS_CUS_SPKSIZE_ANALYSIS      = 9,                // |   V    |   V    |        V          |                   |10:120Hz              0 ~ 38 (20Hz ~ 400 Hz, step:10Hz)   Trubass HD customer analysis frequency
                                                                                                            // |________|________|___________________|___________________|
                                                                                                            // |       Main Controls                                     |
                                                                                                            // |________ ________ ___________________ ___________________|
    API_AUDIO_CUSTOMER_DTS_SE_PARAM_THEATERSOUND_MAIN_CTRL_HEADROOM_GAIN_FXP24          = 10,               // |        |        |        V          |                   |1.0                   0x0 ~ 0x7FFFFF (0.0 ~ 1.0)          Headroom Gain. Provides headroom for intermediate calculations so as to avoid clipping.The value is typically set to the inverse of the setting for the Boost Gain control of the HardLimiter. Note that the value of this control must differ for TVOL on versus off.
    API_AUDIO_CUSTOMER_DTS_SE_PARAM_THEATERSOUND_MAIN_CTRL_BYPASS_GAIN_FXP24            = 11,               // |        |        |        V          |                   |1.0                   0x0 ~ 0x7FFFFF (0.0 ~ 1.0)          Bypass Gain. Adjusts the signal level when TheaterSound HD/DTS StudioSound 3D processing is turned off.
    API_AUDIO_CUSTOMER_DTS_SE_PARAM_THEATERSOUND_MAIN_CTRL_CC3D_PROCESS_PATH            = 12,               // |        |        |        V          |                   |2:PATH_1              0 ~ 2                               Processing Path. Selects the processing path: PATH_NONE:NONE, PATH_0:csdecoder + trudiag + tshd, PATH_1:trudiag + cc3d (depth + stereo tshd). 0:SRS_TS3D_PROCESSINGPATH_NONE, 1:SRS_TS3D_PROCESSINGPATH_0; 2:SRS_TS3D_PROCESSINGPATH_1.
                                                                                                            // |________|________|___________________|___________________|
                                                                                                            // |       CC3D Controls                                     |
                                                                                                            // |________ ________ ___________________ ___________________|
    API_AUDIO_CUSTOMER_DTS_SE_PARAM_THEATERSOUND_CC3D_INPUT_GAIN_FXP24                  = 13,               // |        |        |        V          |                   |1.0                   0x0 ~ 0x7FFFFF (0.0 ~ 1.0)          Input Gain. Adjusts the level of the signal at the input to CC3D processing.
                                                                                                            // |________|________|___________________|___________________|
                                                                                                            // |       TRUVOLUME HD                                      |
                                                                                                            // |________ ________ ___________________ ___________________|
    API_AUDIO_CUSTOMER_DTS_SE_PARAM_THEATERSOUND_TRUVOLUME_MODE                         = 14,               // |        |        |        V          |                   |1:SRS_TVOLHD_NORM     0 ~ 1                               TruVolume HD Mode. Selects the mode of operation for TruVolume HD.There are two modes supported:Light and Normal.The two modes represent different tunings of the algorithm from lighter to more aggressive. 0:SRS_TVOLHD_LIGHT, 1:SRS_TVOLHD_NORM.
    API_AUDIO_CUSTOMER_DTS_SE_PARAM_THEATERSOUND_TRUVOLUME_REF_LEVEL_FXP24              = 15,               // |        |        |        V          |                   |0.125                 0x2C04 ~ 0x7FFFFF (1.0e-3 ~ 1.0)    LKFS. Sets the desired absolute loudness level of the signal
    API_AUDIO_CUSTOMER_DTS_SE_PARAM_THEATERSOUND_TRUVOLUME_MAX_GAIN_FXP24               = 16,               // |        |        |        V          |                   |2.0                   0x1FFFFF ~ 0xFFFFFE0 (0.25 ~ 32.00) Max Gain. Controls the maximum amount that the signal may be increased.
    API_AUDIO_CUSTOMER_DTS_SE_PARAM_THEATERSOUND_TRUVOLUME_NORMALIZE_THRESH_FXP24       = 17,               // |        |        |        V          |                   |0.25                  0x346 ~ 0x3FFFFF (1.0e-4 ~ 0.5)     Norm Threshold. Determines if the gain boost value is greater than 1 or not.
                                                                                                            // |________|________|___________________|___________________|
                                                                                                            // |         TRUDIALOG                                       |
                                                                                                            // |________ ________ ___________________ ___________________|
    API_AUDIO_CUSTOMER_DTS_SE_PARAM_THEATERSOUND_TRUDIALOG_INPUT_GAIN_FXP24             = 18,               // |        |        |        V          |                   |1.0                   0x0 ~ 0x7FFFFF (0.0 ~ 1.0)          Input Gain. Adjusts the level of the signal at the input.
    API_AUDIO_CUSTOMER_DTS_SE_PARAM_THEATERSOUND_TRUDIALOG_CLARITY_GAIN_FXP24           = 19,               // |        |        |        V          |                   |0.5                   0x0 ~ 0x7FFFFF (0.0 ~ 1.0)          Clarity Gain. Adjusts the amount of vocal enhancement that is applied to the audio signal.
    API_AUDIO_CUSTOMER_DTS_SE_PARAM_THEATERSOUND_TRUDIALOG_PROCESS_GAIN_FXP24           = 20,               // |        |        |        V          |                   |2.82                  0x3FFFFF ~ 0x17FFFFD (0.5 ~ 3.0)    Process Gain. Controls how much of the calculated final outout gain is applied to the output signal, effectively controlling how much the process will raise the dialog signal.
                                                                                                            // |________|________|___________________|___________________|
                                                                                                            // |          TBHDX                                          |
                                                                                                            // |________ ________ ___________________ ___________________|
    API_AUDIO_CUSTOMER_DTS_SE_PARAM_THEATERSOUND_CC3D_TBHDX_INPUT_GAIN_FXP24            = 21,               // |        |        |        V          |                   |1.0                   0x0 ~ 0x7FFFFF (0.0 ~ 1.0)          Input Gain. Adjusts the amount of vocal enhancement that is applied to the audio signal.
    API_AUDIO_CUSTOMER_DTS_SE_PARAM_THEATERSOUND_CC3D_TBHDX_BASSLEVEL_FXP24             = 22,               // |        |        |        V          |                   |0.333521432           0x0 ~ 0x7FFFFF (0.0 ~ 1.0)          Bass Level. Determines the level of psychoacoustic bass enhancement.
    API_AUDIO_CUSTOMER_DTS_SE_PARAM_THEATERSOUND_CC3D_TBHDX_SPEAKERSIZE                 = 23,               // |        |        |        V          |                   |2:100Hz               0 ~ 10                              Speaker Size. The range of low frequency limitations in the speakers that TruBass HDX is compensating for. 0:40Hz, 1:60Hz, 2:100Hz, 3:120HZ, 4:150Hz, 5:200Hz, 6:250Hz, 7:300Hz, 8:400Hz, 9:500Hz, 10:600Hz.
    API_AUDIO_CUSTOMER_DTS_SE_PARAM_THEATERSOUND_CC3D_TBHDX_HP_ORDER                    = 24,               // |        |        |        V          |                   |8                     1 ~ 8                               High Pass Order. The slope of the filter that removes the low frequencies that cannot be replicated by the speaker is adjusted with this control.
    API_AUDIO_CUSTOMER_DTS_SE_PARAM_THEATERSOUND_CC3D_TBHDX_DYNAMICS_FXP24              = 25,               // |        |        |        V          |                   |0.3                   0x0 ~ 0x7FFFFF (0.0 ~ 1.0)          Dynamics. Modifies the amount of dynamics processing applied to the signal.The ratio/range of the compressor is dynamically adjusted as the incoming signal is monitored.
    API_AUDIO_CUSTOMER_DTS_SE_PARAM_THEATERSOUND_CC3D_TBHDX_MODE                        = 26,               // |        |        |        V          |                   |1                     0:MONO 1:STEREO                     Mode. Determines if the left and right signals are summed and processed then sent to both channels or if the left and right channels are processed by TruBass HDX separetely. 0:SRS_TBHDX_MODE_MONO, 1:SRS_TBHDX_MODE_STEREO
                                                                                                            // |________|________|___________________|___________________|
                                                                                                            // |        CS DECODER                                       |
                                                                                                            // |________ ________ ___________________ ___________________|
    API_AUDIO_CUSTOMER_DTS_SE_PARAM_THEATERSOUND_CS_INPUT_GAIN_FXP24                    = 27,               // |        |        |        V          |                   |1.0                   0x0 ~ 0x7FFFFF (0.0 ~ 1.0)          Input Gain. Adjusts the level of the signal at the input to CS Decoder.
    API_AUDIO_CUSTOMER_DTS_SE_PARAM_THEATERSOUND_CS_PROCESS_MODE                        = 28,               // |        |        |        V          |                   |1:music               0:cinema, 1:music                   Mode. Specifies the format of the source material.
    API_AUDIO_CUSTOMER_DTS_SE_PARAM_THEATERSOUND_CS_LR_OUTPUT_GAIN_FXP24                = 29,               // |        |        |        V          |                   |2.0                   0xB645A ~ 0x1FFFFFC (0.089 ~ 4.0)   L/R Output Gain. Controls the output level of the front channel of CS Decoder.
    API_AUDIO_CUSTOMER_DTS_SE_PARAM_THEATERSOUND_CS_LsRs_OUTPUT_GAIN_FXP24              = 30,               // |        |        |        V          |                   |1.0                   0xB645A ~ 0x1FFFFFC (0.089 ~ 4.0)   Ls/Rs Output Gain. Controls the output level of the surround channel of CS Decoder.
    API_AUDIO_CUSTOMER_DTS_SE_PARAM_THEATERSOUND_CS_Center_OUTPUT_GAIN_FXP24            = 31,               // |        |        |        V          |                   |2.0                   0xB645A ~ 0x1FFFFFC (0.089 ~ 4.0)   Center Output Gain. Controls the output level of the center channel of CS Decoder.
                                                                                                            // |________|________|___________________|___________________|
                                                                                                            // |            GEQ                                          |
                                                                                                            // |________ ________ ___________________ ___________________|
    API_AUDIO_CUSTOMER_DTS_SE_PARAM_THEATERSOUND_GEQ_INPUT_GAIN_FXP24                   = 32,               // |        |        |        V          |                   |1.0                   0x0 ~ 0x7FFFFF (0.0 ~ 1.0)          Input Gain. Adjusts the level of the signal at the input to Graphic EQ processing.
    API_AUDIO_CUSTOMER_DTS_SE_PARAM_THEATERSOUND_GEQ_BAND0_GAIN_FXP24                   = 33,               // |        |        |        V          |                   |SRS_GEQ_PLUS_05DB     0x16A7EF ~ 0x2D0A3D1(0.177 ~ 5.63)  Band Gain[5]. Adjusts the gain for each band.A different control is available for each band.
    API_AUDIO_CUSTOMER_DTS_SE_PARAM_THEATERSOUND_GEQ_BAND1_GAIN_FXP24                   = 34,               // |        |        |        V          |                   |SRS_GEQ_PLUS_03DB     0x16A7EF ~ 0x2D0A3D1(0.177 ~ 5.63)  SRS_GEQ_MINUS_15DB:0.177, SRS_GEQ_MINUS_11DB:0.282, SRS_GEQ_MINUS_07DB:0.45, SRS_GEQ_MINUS_03DB:0.71, SRS_GEQ_PLUS_01DB:1.12, SRS_GEQ_PLUS_05DB:1.78, SRS_GEQ_PLUS_09DB 2.82, SRS_GEQ_PLUS_13DB:4.47
    API_AUDIO_CUSTOMER_DTS_SE_PARAM_THEATERSOUND_GEQ_BAND2_GAIN_FXP24                   = 35,               // |        |        |        V          |                   |SRS_GEQ_0DB           0x16A7EF ~ 0x2D0A3D1(0.177 ~ 5.63)  SRS_GEQ_MINUS_14DB:0.20 , SRS_GEQ_MINUS_10DB:0.316, SRS_GEQ_MINUS_06DB:0.50, SRS_GEQ_MINUS_02DB:0.79, SRS_GEQ_PLUS_02DB:1.26, SRS_GEQ_PLUS_06DB:2.00, SRS_GEQ_PLUS_10DB 3.16, SRS_GEQ_PLUS_14DB:5.01
    API_AUDIO_CUSTOMER_DTS_SE_PARAM_THEATERSOUND_GEQ_BAND3_GAIN_FXP24                   = 36,               // |        |        |        V          |                   |SRS_GEQ_PLUS_03DB     0x16A7EF ~ 0x2D0A3D1(0.177 ~ 5.63)  SRS_GEQ_MINUS_13DB:0.224, SRS_GEQ_MINUS_09DB:0.355, SRS_GEQ_MINUS_05DB:0.56, SRS_GEQ_MINUS_01DB:0.89, SRS_GEQ_PLUS_03DB:1.41, SRS_GEQ_PLUS_07DB:2.24, SRS_GEQ_PLUS_11DB 3.55, SRS_GEQ_PLUS_15DB:5.63
    API_AUDIO_CUSTOMER_DTS_SE_PARAM_THEATERSOUND_GEQ_BAND4_GAIN_FXP24                   = 37,               // |        |        |        V          |                   |SRS_GEQ_PLUS_05DB     0x16A7EF ~ 0x2D0A3D1(0.177 ~ 5.63)  SRS_GEQ_MINUS_12DB:0.251, SRS_GEQ_MINUS_08DB:0.40 , SRS_GEQ_MINUS_04DB:0.63, SRS_GEQ_0DB:1.00       , SRS_GEQ_PLUS_04DB:1.58, SRS_GEQ_PLUS_08DB:2.51, SRS_GEQ_PLUS_12DB 4.00,
                                                                                                            // |________|________|___________________|___________________|
                                                                                                            // |     PURESOUND HRADLIMITER                               |
                                                                                                            // |________ ________ ___________________ ___________________|
    API_AUDIO_CUSTOMER_DTS_SE_PARAM_PURESOUND_HL_INPUT_GAIN_FXP24                       = 38,               // |        |   V    |        V          |        V          |1.0                   0x0 ~ 0x1FFFFFC (0.0 ~ 4.0)         HardLimiter Input Gain. Adjusts the value used for adjusting the audio level after the HardLimiter process
    API_AUDIO_CUSTOMER_DTS_SE_PARAM_PURESOUND_HL_OUTPUT_GAIN_FXP24                      = 39,               // |        |   V    |        V          |        V          |1.0                   0x0 ~ 0x1FFFFFC (0.0 ~ 4.0)         HardLimiter Output Gain. Adjusts the value used for adjusting the audio level before the HardLimiter process.
    API_AUDIO_CUSTOMER_DTS_SE_PARAM_PURESOUND_HL_BYPASS_GAIN_FXP24                      = 40,               // |        |   V    |        V          |        V          |1.0                   0x0 ~ 0x7FFFFF  (0.0 ~ 1.0)         HardLimiter Bypass Gain. Adjusts the value used for adjusting the audio level when disable the HardLimiter process.
    API_AUDIO_CUSTOMER_DTS_SE_PARAM_PURESOUND_HL_LIMITERBOOST_FXP24                     = 41,               // |        |   V    |        V          |        V          |2.5                   0x7FFFFF ~ 0xFFFFFE0 (1.0 ~ 32.0)   (20*log(v/32))dB, v is 1.0 ~ 32.0. so 32.0 is 0dB (MAX). HardLimiter BoostGain. Modifies the signal level within the context of the HardLimiter, so that the signal will not exceed the Hard Limit Level parameter
    API_AUDIO_CUSTOMER_DTS_SE_PARAM_PURESOUND_HL_HARDLIMIT_CTRL_FXP24                   = 42,               // |        |   V    |        V          |        V          |1.0                   0x0 ~ 0x7FFFFF  (0.0 ~ 1.0)         HardLimiter limit level.
    API_AUDIO_CUSTOMER_DTS_SE_PARAM_PURESOUND_HL_DELAY                                  = 43,               // |        |   V    |        V          |        V          |14                    6 ~ 48                              HardLimiter Delay Length. Specifies the length of the look-ahead delay line utilized in the HardLimiter algorithm.
                                                                                                            // |________|________|___________________|___________________|
                                                                                                            // |       PURESOUND AEQ                                     |
                                                                                                            // |________ ________ ___________________ ___________________|
    API_AUDIO_CUSTOMER_DTS_SE_PARAM_PURESOUND_AEQ_INPUT_GAIN_FXP24                      = 44,               // |        |        |        V          |        V          |1.0                   0x0 ~ 0x7FFFFF (0.0 ~ 1.0)          AEQ Input Gain. Adjusts the value used for adjusting the audio level after the AEQ process.
    API_AUDIO_CUSTOMER_DTS_SE_PARAM_PURESOUND_AEQ_OUTPUT_GAIN_FXP24                     = 45,               // |        |        |        V          |        V          |1.0                   0x0 ~ 0x7FFFFF (0.0 ~ 1.0)          AEQ Output Gain. Adjusts the value used for adjusting the audio level before the AEQ process.
    API_AUDIO_CUSTOMER_DTS_SE_PARAM_PURESOUND_AEQ_BYPASS_GAIN_FXP24                     = 46,               // |        |        |        V          |        V          |1.0                   0x0 ~ 0x7FFFFF (0.0 ~ 1.0)          AEQ Bypass Gain. Adjusts the value used for adjusting the audio level when disable the AEQ process.
                                                                                                            // |________|________|___________________|___________________|
                                                                                                            // |       PURESOUND HPF                                     |
                                                                                                            // |________ ________ ___________________ ___________________|
    API_AUDIO_CUSTOMER_DTS_SE_PARAM_PURESOUND_HPF_FREQUENCY                             = 47,               // |        |        |        V          |        V          |1                     0 ~ 3                               HPF Frequency. 0:60Hz, 1:80Hz, 2:100Hz, 3:120Hz
                                                                                                            // |________|________|___________________|___________________|
                                                                                                            // |      PURESOUND Input/output gain                        |
                                                                                                            // |________ ________ ___________________ ___________________|
    API_AUDIO_CUSTOMER_DTS_SE_PARAM_PURESOUND_INPUT_GAIN_FXP24                          = 48,               // |        |        |                   |        V          |                      0x0 ~ 0x7FFFFF (0.0 ~ 1.0)
    API_AUDIO_CUSTOMER_DTS_SE_PARAM_PURESOUND_OUTPUT_GAIN_FXP24                         = 49,               // |        |        |                   |        V          |                      0x0 ~ 0x7FFFFF (0.0 ~ 1.0)
                                                                                                            // |________|________|___________________|___________________|
                                                                                                            // |      PURESOUND TBHD                                     |
                                                                                                            // |________ ________ ___________________ ___________________|
    API_AUDIO_CUSTOMER_DTS_SE_PARAM_PURESOUND_TBHD_TRUBASS_LEVEL_FXP24                  = 51,               // |        |        |                   |        V          |1.0                   0x0 ~ 0x7FFFFF (0.0 ~ 1.0)
    API_AUDIO_CUSTOMER_DTS_SE_PARAM_PURESOUND_TBHD_LEVEL_INDEPENDENT_EN                 = 52,               // |        |        |                   |        V          |1                     0 ~ 1
    API_AUDIO_CUSTOMER_DTS_SE_PARAM_PURESOUND_TBHD_COMPRESSOR_LEVEL_FXP24               = 53,               // |        |        |                   |        V          |0.8                   0x0 ~ 0x7FFFFF (0.0 ~ 1.0)
    API_AUDIO_CUSTOMER_DTS_SE_PARAM_PURESOUND_TBHD_SPEAKER_AUDIO                        = 54,               // |        |        |                   |        V          |0                     0 ~ 8                               0:40Hz, 1:60Hz, 2:100Hz, 3:120Hz, 4:150Hz, 5:200Hz, 6:250Hz, 7:300Hz, 8:400Hz
    API_AUDIO_CUSTOMER_DTS_SE_PARAM_PURESOUND_TBHD_SPEAKER_ANALYSIS                     = 55, /*<-MAX*/     // |        |        |                   |        V          |1                     0 ~ 8                               0:40Hz, 1:60Hz, 2:100Hz, 3:120Hz, 4:150Hz, 5:200Hz, 6:250Hz, 7:300Hz, 8:400Hz
                                                                                                            // |________|________|___________________|___________________|

    API_AUDIO_CUSTOMER_DTS_SE_PARAM_MAX = API_AUDIO_CUSTOMER_DTS_SE_PARAM_PURESOUND_TBHD_SPEAKER_ANALYSIS + 1,
} API_AUDIO_CUSTOMER_DTS_SE_PARAM_TYPE;

typedef struct
{
    MS_U32 u32IirBandNum;
    MS_S32 s32ScrambleIirCoefs[50];
    MS_U32 u32IirOrder;
    MS_U32 u32FirTapNum;
    MS_S32 s32ScrambleFirCoefs[67];
    MS_U32 u32FirOrder;
} API_AUDIO_CUSTOMER_DTS_SE_AEQ_COEF;

//For Dolby DAP's parameter setting structure.
#define DOLBY_DAP_MAX_BANDS          (20)
#define DOLBY_DAP_IEQ_MAX_BANDS      DOLBY_DAP_MAX_BANDS
#define DOLBY_DAP_GEQ_MAX_BANDS      DOLBY_DAP_MAX_BANDS
#define DOLBY_DAP_REG_MAX_BANDS      DOLBY_DAP_MAX_BANDS
#define DOLBY_DAP_OPT_MAX_BANDS      DOLBY_DAP_MAX_BANDS
#define DOLBY_DAP_MAX_CHANNELS       6

                                                                            //  ________________________________________________________
                                                                            // |
                                                                            // |Range               Default
                                                                            // |________________________________________________________
typedef struct                                                              // |
{                                                                           // |
    int pregain;                                                            // |-2080 ~ 480         0
    int postgain;                                                           // |-2080 ~ 480         0
    int systemgain;                                                         // |-2080 ~ 480         0
    int surround_decoder_enable;                                            // |    0 ~ 1           1
    int virtualizer_enable;                                                 // |    0 ~ 1           1
    int headphone_reverb;                                                   // |-2080 ~ 192         0
    int speaker_angle;                                                      // |    0 ~ 27          5
    int speaker_start;                                                      // |   20 ~ 20000       200
    int surround_boost;                                                     // |    0 ~ 96          96
    int mi_ieq_enable;                                                      // |    0 ~ 1           0
    int mi_dv_enable;                                                       // |    0 ~ 1           0
    int mi_de_enable;                                                       // |    0 ~ 1           0
    int mi_surround_enable;                                                 // |    0 ~ 1           0
                                                                            // |
    int calibration_boost;                                                  // |    0 ~ 192         0
                                                                            // |
    int leveler_amount;                                                     // |    0 ~ 10          7
    int leveler_input;                                                      // | -640 ~ 0           -496
    int leveler_output;                                                     // | -640 ~ 0           -496
    int leveler_enable;                                                     // |    0 ~ 1           0
                                                                            // |
    int modeler_enable;                                                     // |    0 ~ 1           0
    int modeler_calibration;                                                // | -320 ~ 320         0
                                                                            // |
    int ieq_enable;                                                         // |    0 ~ 1           0
    int ieq_amount;                                                         // |    0 ~ 16          10
    int ieq_nb_bands;                                                       // |    1 ~ 20          20
    int a_ieq_band_center[DOLBY_DAP_IEQ_MAX_BANDS];                         // |   20 ~ 20000       {65,  136, 223, 332, 467, 634, 841, 1098, 1416, 1812, 2302, 2909, 3663, 4598, 5756, 7194, 8976, 11186, 13927, 17326}
    int a_ieq_band_target[DOLBY_DAP_IEQ_MAX_BANDS];                         // | -480 ~ 480         {117, 133, 188, 176, 141, 149, 175, 185,  185,  200,  236,  242,  228,  213,  182,  132,  110,  68,    -27,   -240}
                                                                            // |
    int de_enable;                                                          // |    0 ~ 1           1
    int de_amount;                                                          // |    0 ~ 16          9
    int de_ducking;                                                         // |    0 ~ 16          0
                                                                            // |
    int volmax_boost;                                                       // |    0 ~ 192
                                                                            // |
    int geq_enable;                                                         // |    0 ~ 1           0
    int geq_nb_bands;                                                       // |    1 ~ 20          0
    int a_geq_band_center[DOLBY_DAP_GEQ_MAX_BANDS];                         // |   20 ~ 20000       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    int a_geq_band_target[DOLBY_DAP_GEQ_MAX_BANDS];                         // | -576 ~ 576         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
                                                                            // |
    int optimizer_enable;                                                   // |    0 ~ 1           1
    int optimizer_nb_bands;                                                 // |    1 ~ 20          20
    int a_opt_band_center_freq[DOLBY_DAP_OPT_MAX_BANDS];                    // |   20 ~ 20000       {47, 141, 234, 328, 469, 656, 844, 1031, 1313, 1688,2250, 3000, 3750, 4688, 5813, 7125, 9000, 11250, 13875, 19688}
    int a_opt_band_gain[DOLBY_DAP_MAX_CHANNELS][DOLBY_DAP_OPT_MAX_BANDS];   // | -480 ~ 480         {...} show in AU_CUS_AQ_Tuning_Dolby_DAP_SetParam(void)
                                                                            // |
    int bass_enable;                                                        // |    0 ~ 1           1
    int bass_boost;                                                         // |    0 ~ 384         177
    int bass_cutoff;                                                        // |   20 ~ 2000        140
    int bass_width;                                                         // |    2 ~ 64          9
                                                                            // |
    int reg_nb_bands;                                                       // |    1 ~ 20          20
    int a_reg_band_center[DOLBY_DAP_REG_MAX_BANDS];                         // |   20 ~ 20000       {47,   141,  234,  328,  469,  656,  844,  1031, 1313, 1688, 2250, 3000, 3750, 4688, 5813, 7125, 9000, 11250, 13875, 19688}
    int a_reg_low_thresholds[DOLBY_DAP_REG_MAX_BANDS];                      // |-2080 ~ 0           {-496, -192, -448, -384, -192, -192, -192, -192, -192, -192, -192, -192, -192, -192, -192, -192, -192, -192,  -192,  -192}
    int a_reg_high_thresholds[DOLBY_DAP_REG_MAX_BANDS];                     // |-2080 ~ 0           {-304, 0, -256, -192, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    int a_reg_isolated_bands[DOLBY_DAP_REG_MAX_BANDS];                      // |    0 ~ 1           {1,    0, 1,    1,    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
                                                                            // |
    int regulator_overdrive;                                                // |    0 ~ 192         0
    int regulator_timbre;                                                   // |    0 ~ 16          12
    int regulator_distortion;                                               // |    0 ~ 144         96
    int regulator_mode;                                                     // |    0 ~ 1           1
    int regulator_enable;                                                   // |    0 ~ 1           1
    int virtual_bass_mode;                                                  // |    0 ~ 3           3
    int virtual_bass_low_src_freq;                                          // |   30 ~ 90          55
    int virtual_bass_high_src_freq;                                         // |   90 ~ 270         110
    int virtual_bass_overall_gain;                                          // | -480 ~ 0           0
    int virtual_bass_slope_gain;                                            // |   -3 ~ 0           -2
    int virtual_bass_subgain[3];                                            // | -480 ~ 0           {-80, -240, -400}
    int virtual_bass_mix_low_freq;                                          // |    0 ~ 375         110
    int virtual_bass_mix_high_freq;                                         // |  281 ~ 938         281
    int virtual_bass_enable;                                                // |    0 ~ 1           1

    int a_opt_band_gain2[2][DOLBY_DAP_OPT_MAX_BANDS];
    int height_filter_mode;
    int leveler_ignore_il;
    int lfe_to_lr_gain;
    int b_lfe_to_lr_enable;
    int b_lfe_10db_boost_enable;
    int b_bass_extraction;
    int bass_extraction_cutoff_freq;
} API_AUDIO_CUSTOMER_DOLBY_DAP_PARAM;

//For SonicEmotion ABS3D's parameter setting structure.
                                                    //  ________________________________________________________________________________________________
                                                    // |
                                                    // |    Range                       Default
typedef struct InputCtrl_Tuning                     // |________________________________________________________________________________________________
{                                                   // |
    int Vol_dB;                                     // |    [-80 ~ 12] * 1000           -12 * 1000
} InputCtrl_Tuning;
                                                    //  ________________________________________________________________________________________________
                                                    // |
                                                    // |    Range                       Default
typedef struct CrossOverIIR_Tuning                  // |________________________________________________________________________________________________
{                                                   // |
    int Fc_Hz;                                      // |    [1 ~ 1000] * 1000           200 * 1000
    int Enable;                                     // |    [0 ~ 1]    * 1000           1   * 1000
} CrossOverIIR_Tuning;
                                                    //  ________________________________________________________________________________________________
                                                    // |
                                                    // |    Range                       Default
typedef struct AnalyzerTD_V2_Tuning                 // |________________________________________________________________________________________________
{                                                   // |
    int Ambience;                                   // |    [0 ~ 1] * 1000              0.9  * 1000
    int Balance;                                    // |    [0 ~ 1] * 1000              0.5  * 1000
    int CenterFocus;                                // |    [0 ~ 1] * 1000              0.75 * 1000
    int Enable;                                     // |    [0 ~ 1] * 1000              1    * 1000
} AnalyzerTD_V2_Tuning;
                                                    //  ________________________________________________________________________________________________
                                                    // |
                                                    // |    Range                       Default
typedef struct WCMatrix_V3_Tuning                   // |________________________________________________________________________________________________
{                                                   // |
    int AnalyzerLatency;                            // |    [0 ~ 256]  * 1000           256 * 1000
    int Enable;                                     // |    [0 ~ 1]    * 1000           1   * 1000
                                                    // |
    int LRWCVol_dB;                                 // |    [-80 ~ 12] * 1000           0   * 1000
    int ClCrVol_dB;                                 // |    [-80 ~ 12] * 1000           0   * 1000
    int C51Vol_dB;                                  // |    [-80 ~ 12] * 1000           0   * 1000
    int LsRsVol_dB;                                 // |    [-80 ~ 12] * 1000           0   * 1000
    int CSDiffVol_dB;                               // |    [-80 ~ 12] * 1000           0   * 1000
                                                    // |
    int LRWCwidth;                                  // |    [0 ~ 1]    * 1000           1   * 1000
    int ClCrFocus;                                  // |    [0 ~ 1]    * 1000           0.5 * 1000
    int C51Focus;                                   // |    [0 ~ 1]    * 1000           1   * 1000
                                                    // |
    int C51Delay_ms;                                // |    [0 ~ 20]   * 1000           0   * 1000
    int LsRsDelay_ms;                               // |    [0 ~ 20]   * 1000           0   * 1000
                                                    // |
    int CS_Dist;                                    // |    [0 ~ 1]    * 1000           1   * 1000
    int LRWCFocusCSDist;                            // |    [0 ~ 1]    * 1000           0.5 * 1000
    int ClCrFocusCSDist;                            // |    [0 ~ 1]    * 1000           0.6 * 1000
    int C51FocusCSDist;                             // |    [0 ~ 1]    * 1000           0.5 * 1000
    int LsRsFocusCSDist;                            // |    [0 ~ 1]    * 1000           0.7 * 1000
} WCMatrix_V3_Tuning;
                                                    //  ________________________________________________________________________________________________
                                                    // |
                                                    // |    Range                       Default
typedef struct LevelEqualizer_V3_Tuning             // |________________________________________________________________________________________________
{                                                   // |
    int Enable;                                     // |    [0 ~ 1]     * 1000          1             * 1000
    int Threshold_dB;                               // |    [-40 ~ -3]  * 1000          -30           * 1000
    int LevelRange_dB;                              // |    [3 ~ 40]    * 1000          20            * 1000
    int MinGain_dB[2];                              // |    [-20 ~ 20]  * 1000          {0,0}         * 1000
    int MaxGain_dB[2];                              // |    [-20 ~ 20]  * 1000          {0,0}         * 1000
    int Frequency_Hz[2];                            // |    [20 ~ 20000]* 1000          {2500,5500}   * 1000
    int Q[2];                                       // |    [0.01 ~ 10] * 1000          {0.707,0.707} * 1000
    int AttackTime_ms;                              // |    [0.01 ~ 1000]*1000          500           * 1000
    int ReleaseTime_ms;                             // |    [0.01 ~ 1000]*1000          500           * 1000
} LevelEqualizer_V3_Tuning;
                                                    //  ________________________________________________________________________________________________
                                                    // |
                                                    // |    Range                       Default
typedef struct ParametricEqualizer_V1_Tuning        // |________________________________________________________________________________________________
{                                                   // |
    int Enable;                                     // |    [0 ~ 1]      * 1000         1                         * 1000
    int GlobalMakeUpGain_dB;                        // |    [-80 ~ 12]   * 1000         0                         * 1000
                                                    // |
    int EQAllChannels_Q[4];                         // |    [0.5 ~ 20]   * 1000         {0.707,0.707,0.707,0.707} * 1000
    int EQAllChannels_Frequency_Hz[4];              // |    [10 ~ 20000] * 1000         {125,250,500,1000}        * 1000
    int EQAllChannels_Gain_dB[4];                   // |    [-12 ~ 12]   * 1000         {0,0,0,0}                 * 1000
    int EQAllChannels_FilterType[4];                // |    [0 ~ 4]      * 1000         {0,0,0,0}                 * 1000
    int EQAllChannels_Enable[4];                    // |    [0 ~ 1]      * 1000         {1,1,1,1}                 * 1000
} ParametricEqualizer_V1_Tuning;
                                                    //  ________________________________________________________________________________________________
                                                    // |
                                                    // |    Range                       Default
typedef struct Abs3DRenderer_V3_Tuning              // |________________________________________________________________________________________________
{                                                   // |
    int Spacing_m;                                  // |    [0.1 ~ 1] * 1000            0.5      * 1000
    int Angle_deg[2];                               // |    [10 ~ 90],[270 ~ 350]*1000  {50,310} * 1000
                                                    // |
    int CTC_Enable;                                 // |    [0 ~ 1]   * 1000            1        * 1000
    int Binaural_Enable;                            // |    [0 ~ 1]   * 1000
                                                    // |
    int FilterCTC_FileIndex;                        // |    [0]       * 1000            0        * 1000
    int FilterBinaural_FileIndex;                   // |    [0]       * 1000            0        * 1000
    int DelayBinaural_FileIndex;                    // |    [0]       * 1000            0        * 1000
} Abs3DRenderer_V3_Tuning;
                                                    //  ________________________________________________________________________________________________
                                                    // |
                                                    // |    Range                       Default
typedef struct BassEnhancer_V3_Tuning               // |________________________________________________________________________________________________
{                                                   // |
    int Eq_Enable;                                  // |    [0 ~ 1]     * 1000          1     * 1000
    int Compressor_Enable;                          // |    [0 ~ 1]     * 1000          1     * 1000
                                                    // |
    int Ratio;                                      // |    [1 ~ 20]    * 1000          1.7   * 1000
    int LevelAttackTime_ms;                         // |    [0 ~ 200]   * 1000          1     * 1000
    int LevelReleaseTime_ms;                        // |    [0 ~ 200]   * 1000          20    * 1000
    int SmoothingAttackTime_ms;                     // |    [0 ~ 200]   * 1000          20    * 1000
    int SmoothingReleaseTime_ms;                    // |    [0 ~ 200]   * 1000          1     * 1000
    int Threshold_dB;                               // |    [-40 ~ 0]   * 1000          -25   * 1000
    int MakeupGain_dB;                              // |    [-20 ~ 20]  * 1000          -3    * 1000
                                                    // |
    int HighPass_Enable;                            // |    [0 ~ 1]     * 1000          1     * 1000
    int HighPassFc_Hz;                              // |    [10 ~ 1000] * 1000          50    * 1000
    int HighPassDecay_dB;                           // |    [-12 ~ -24] * 1000          -24   * 1000
                                                    // |
    int PeakFilter1_Enable;                         // |    [0 ~ 1]     * 1000          1     * 1000
    int Peakfilter1Gain_dB;                         // |    [-20 ~ 20]  * 1000          0     * 1000
    int Peakfilter1Fc_Hz;                           // |    [10 ~ 1000] * 1000          140   * 1000
    int Peakfilter1Q;                               // |    [0.5 ~ 20]  * 1000          0.707 * 1000
                                                    // |
    int PeakFilter2_Enable;                         // |    [0 ~ 1]     * 1000          1     * 1000
    int Peakfilter2Gain_dB;                         // |    [-20 ~ 20]  * 1000          0     * 1000
    int Peakfilter2Fc_Hz;                           // |    [10 ~ 1000] * 1000          200   * 1000
    int Peakfilter2Q;                               // |    [0.5 ~ 20]  * 1000          0.707 * 1000
                                                    // |
    int Bwe_Enable;                                 // |    [0 ~ 1]     * 1000          1     * 1000
    int Bwe_PostNLDLowPassShapingFilterFc_Hz;       // |    [0 ~ 1]     * 1000          520   * 1000
    int Bwe_OrigGain_dB;                            // |    [-20 ~ 0]   * 1000          0     * 1000
    int Bwe_NLDGain_dB;                             // |    [-6 ~ 20]   * 1000          6     * 1000
    int Bwe_AttackRemoverTime_ms;                   // |    [100 ~ 500] * 1000          100   * 1000
    int Bwe_AttackRemoverShortTermTime_ms;          // |    [0 ~ 50]    * 1000          0     * 1000
    int Bwe_GainSmooAttackTime_ms;                  // |    [0 ~ 20]    * 1000          5     * 1000
    int Bwe_GainSmooReleaseTime_ms;                 // |    [0 ~ 200]   * 1000          15    * 1000
    int Bwe_HarmonicDecay_dB;                       // |    [-10 ~ -24] * 1000          -10   * 1000
    int Bwe_H1Level_dB;                             // |    [-80 ~ 20]  * 1000          -80   * 1000
    int Bwe_H2Level_dB;                             // |    [-20 ~ 20]  * 1000          6     * 1000
    int Bwe_Compressor_Enable;                      // |    [0 ~ 1]     * 1000          1     * 1000
    int Bwe_PostNLDHighPass_Enable;                 // |    [0 ~ 1]     * 1000          0     * 1000
    int Bwe_PostNLDLowPassShapingFilter_Enable;     // |    [1 ~ 4xcrossOverFc]*1000    0     * 1000
                                                    // |
    int LevelEstimatorBass_Enable;                  // |    [0 ~ 1]     * 1000          1     * 1000
    int LevelEstimatorBass_Threshold_dB;            // |    [-40 ~ 0]   * 1000          -30   * 1000
    int LevelEstimatorBass_LevelRange_dB;           // |    [0 ~ 20]    * 1000          20    * 1000
    int LevelEstimatorBass_MinGain_dB;              // |    [-10 ~ 3]   * 1000          0     * 1000
    int LevelEstimatorBass_MaxGain_dB;              // |    [0 ~ 20]    * 1000          15    * 1000
    int LevelEstimatorBass_AttackTime_ms;           // |    [0 ~ 5000]  * 1000          500   * 1000
    int LevelEstimatorBass_ReleaseTime_ms;          // |    [0 ~ 5000]  * 1000          500   * 1000
    int LevelEqualizer_Q;                           // |    [0.01 ~ 10] * 1000          1     * 1000
    int LevelEqualizer_Fc_Hz;                       // |    [1 ~ crossOverFc]*1000      110   * 1000
} BassEnhancer_V3_Tuning;
                                                    //  ________________________________________________________________________________________________
                                                    // |
                                                    // |    Range                       Default
typedef struct BassRenderer_V2_Tuning               // |________________________________________________________________________________________________
{                                                   // |
    int ManualDelay_ms;                             // |    [0 ~ 20]   * 1000           0   * 1000
    int LatencyLf_Samples;                          // |    [0 ~ 256]  * 1000           116 * 1000
    int LatencyHf_Samples;                          // |    [0 ~ 256]  * 1000           256 * 1000
    int DeviceMode;                                 // |    [0,3]      * 1000           2   * 1000
} BassRenderer_V2_Tuning;
                                                    //  ________________________________________________________________________________________________
                                                    // |
                                                    // |    Range                       Default
typedef struct ECompressor_V3_Tuning                // |________________________________________________________________________________________________
{                                                   // |
    int LevelAttackTime_ms;                         // |    [0 ~ 200]  * 1000           0.5  * 1000
    int LevelReleaseTime_ms;                        // |    [0 ~ 200]  * 1000           2    * 1000
    int Threshold_dB;                               // |    [-40 ~ 0]  * 1000           -3   * 1000
    int Ratio;                                      // |    [1 ~ 20]   * 1000           20   * 1000
    int MakeupGain_dB;                              // |    [-20 ~ 20] * 1000           0    * 1000
    int SmoothingAttackTime_ms;                     // |    [0 ~ 200]  * 1000           10   * 1000
    int SmoothingReleaseTime_ms;                    // |    [0 ~ 200]  * 1000           100  * 1000
    int Enable;                                     // |    {0 ~ 1}    * 1000           1    * 1000
} ECompressor_V3_Tuning;

typedef struct
{
    InputCtrl_Tuning InputCtrl;
    CrossOverIIR_Tuning CrossOverIIR;
    AnalyzerTD_V2_Tuning AnalyzerTD_V2;
    WCMatrix_V3_Tuning WCMatrix_V3;
    LevelEqualizer_V3_Tuning LevelEqualizer_V3[2];
    ParametricEqualizer_V1_Tuning ParametricEqualizer_V1[5];
    Abs3DRenderer_V3_Tuning Abs3DRenderer_V3;
    BassEnhancer_V3_Tuning BassEnhancer_V3;
    BassRenderer_V2_Tuning BassRenderer_V2;
    ECompressor_V3_Tuning ECompressor_V3[2];
} API_AUDIO_CUSTOMER_SONICEMOTION_ABS3D_PARAM;

// For DTS VirtualX's parameter setting structure.
#define kTSXbitMskFront     ( 0x003 )   /**< Channel mask type for Front                    */
#define kTSXbitMskCtr       ( 0x004 )   /**< Channel mask type for Center                   */
#define kTSXbitMskLfe       ( 0x008 )   /**< Channel mask type for Low Frequency Effects    */
#define kTSXbitMskRear      ( 0x030 )   /**< Channel mask type for Rear                     */
#define kTSXbitMskSide      ( 0x0C0 )   /**< Channel mask type for Side                     */
#define kTSXbitMskFrntHgt   ( 0x300 )   /**< Channel mask type for Front Height             */
#define kTSXbitMskRearHgt   ( 0xC00 )   /**< Channel mask type for Rear Front               */

// VirtualXLib1 Input channel layout
typedef enum
{
    kTSXIn_Stereo = kTSXbitMskFront,                                                                                          /**< Represents 2.0 Ch   */
    kTSXIn_5p1    = kTSXbitMskFront | kTSXbitMskCtr | kTSXbitMskLfe | kTSXbitMskRear,                                         /**< Represents 5.1 Ch   */
    kTSXIn_7p1    = kTSXbitMskFront | kTSXbitMskCtr | kTSXbitMskLfe | kTSXbitMskRear | kTSXbitMskSide,                        /**< Represents 7.1 Ch   */
    kTSXIn_5p1p2  = kTSXbitMskFront | kTSXbitMskCtr | kTSXbitMskLfe | kTSXbitMskRear | kTSXbitMskFrntHgt,                     /**< Represents 5.1.2 Ch */
    kTSXIn_5p1p4  = kTSXbitMskFront | kTSXbitMskCtr | kTSXbitMskLfe | kTSXbitMskRear | kTSXbitMskFrntHgt | kTSXbitMskRearHgt, /**< Represents 5.1.4 Ch */
} DTS_TRUSRNDX_INPUT_MODE;

// VirtualXLib1 Output channel layout
typedef enum
{
    kTSXOut_Stereo = kTSXbitMskFront,
} DTS_TRUSRNDX_OUTPUT_MODE;

// VirtualXLib1 Horizontal plane effect controls
typedef enum
{
    kTSX_HorizEffct_Default,  /**< Default Horizontal plane effect control */
    kTSX_HorizEffct_Mild,     /**< Mild Horizontal plane effect control    */
} DTS_TSX_HORIZN_EFFECT_CTRL;
                                                            // VirtualX API control params:
                                                            // Structure containing VirtualX API specific control params.
                                                            //  ____________________________________________________________________________________________________________________________________________________
                                                            // |
                                                            // |    Default             Range                                                                   Description
typedef struct dtsVXApiCtrl_t                               // |____________________________________________________________________________________________________________________________________________________
{                                                           // |
    int                       nVXLib1Enable;                // |    0x1                 0 ~ 1                                                                   Control to enable or disable VirtualX Lib1 processing
    DTS_TRUSRNDX_INPUT_MODE   nInputMode;                   // |    0x3                 3:"2.0",63:"5.1",255:"7.1",831:"5.1.2",3903:"5.1.4"                     Control to select VirtualX input channel layout
    DTS_TRUSRNDX_OUTPUT_MODE  nOutputMode;                  // |    0x3                 3:"2.0"                                                                 Control to select VirtualX output channel layout
    int                       nHeadRoom;                    // |    0x40000000(1.0)     0x8000000 ~ 0x40000000(normalized:         0.125 ~ 1.0)                 Control to specify Headroom gain
    int                       nProOutputGain;               // |    0x10000000(1.0)     0x8000000 ~ 0x40000000(normalized:           0.5 ~ 4.0)                 Control to specify processing output gain
    int                       nReferenceLevel;              // |    0x4000000(-24 dB)   0xCCCCD   ~ 0x4000000 (normalized:7.8e-4(-62 dB) ~ 0.0625(-24 dB))      Control for virtualxLib1 reference level( Read-only )
} dtsVXApiCtrl_t;
                                                            // TruSrndX API control params:
                                                            // Structure containing TruSrndX API specific control params.
                                                            //  ____________________________________________________________________________________________________________________________________________________
                                                            // |
                                                            // |    Default             Range                                                                   Description
typedef struct dtsTSXApiCtrl_t                              // |____________________________________________________________________________________________________________________________________________________
{                                                           // |
    int                         nTsxEnable;                 // |    0x1                          0 ~ 1                                                          Control to enable or disable TruSurroundX processing
    int                         nPassiveMtrxEnable;         // |    0x1                          0 ~ 1                                                          Control to enable or disable TruSurroundX Passive Matrix
    int                         nHeightUpmixEnable;         // |    0x1                          0 ~ 1                                                          Control to enable or disable TruSurroundX Height Upmix
    DTS_TSX_HORIZN_EFFECT_CTRL  nHorizEffectStrength;       // |    0x0                     0:Mild ~ 1:Default                                                  Control to specify TruSurroundX Horizontal plane effect strength
    int                         nLpRtoCtrMixGain;           // |    0x20000000(1.0)            0x0 ~ 0x40000000(normalized: 0.0 ~ 2.0)                          Control to specify TruSurroundX LpR to Center Mix Gain
    int                         nCtrGain;                   // |    0x20000000(1.0)     0x20000000 ~ 0x40000000(normalized: 1.0 ~ 2.0)                          Control to specify TruSurroundX Center gain
    int                         nHeightMixCoeff;            // |    0x20000000(1.0)     0x10000000 ~ 0x40000000(normalized: 0.5 ~ 2.0)                          Control to specify TruSurroundX Height Mix Coefficient
    int                         nTsxProcessDiscrad;         // |    0x0                          0 ~ 1                                                          Control to discard VirtualX Lib1 processing completely including headroom
    int                         nFrontControl;              // |    0x20000000(1.0)     0x10000000 ~ 0x40000000(normalized: 0.5 ~ 2.0)                          Control the Front widening effect gain
    int                         nSurroundControl;           // |    0x20000000(1.0)     0x10000000 ~ 0x40000000(normalized: 0.5 ~ 2.0)                          Control the Surround effect gain
} dtsTSXApiCtrl_t;
                                                            // SubComponent control params:
                                                            // Structure containing control params for Dialog Clarity & Definition functionalities.
                                                            //  ____________________________________________________________________________________________________________________________________________________
                                                            // |
                                                            // |    Default             Range                                                                   Description
typedef struct dtsVirtualXLib1SubCompApiCtrl_t              // |____________________________________________________________________________________________________________________________________________________
{                                                           // |
    int nEnable;                                            // |    0x0                   0 ~ 1                                                                 Control to enable Dialog Clarity or Definition
    int nControl;                                           // |    0x199999A0(0.4)     0x0 ~ 0x40000000(normalized: 0.0 ~ 1.0)                                 Represents Dialog Clarity control or Definition control
} dtsVirtualXLib1SubCompApiCtrl_t;
                                                            // VirtualXLib1 API Control structure :
                                                            // Master structure containing control params for VirtualXLib1, TrusrndX, Definition and Dialog Clarity.
                                                            //  ____________________________________________________________________________________________________________________________________________________
                                                            // |
                                                            // |    Description
typedef struct                                              // |____________________________________________________________________________________________________________________________________________________
{                                                           // |
    dtsVXApiCtrl_t                  vxApiControls;          // |    VirtualX controls.       Refer #dtsVXApiCtrl_t
    dtsTSXApiCtrl_t                 tsxApiControls;         // |    TrusrndX controls.       Refer #dtsTSXApiCtrl_t
    dtsVirtualXLib1SubCompApiCtrl_t focusApiControls;       // |    Dialog Clarity controls. Refer #dtsVirtualXLib1SubCompApiCtrl_t
    dtsVirtualXLib1SubCompApiCtrl_t defApiControls;         // |    Definition controls.     Refer #dtsVirtualXLib1SubCompApiCtrl_t
} dtsVirtualXLib1ApiCtrl_t;
                                                            // MBHL common API control structure
                                                            //  ____________________________________________________________________________________________________________________________________________________
                                                            // |
                                                            // |    Default             Range                                                                   Description
typedef struct dtsMbhlCommonApiCtrl                         // |____________________________________________________________________________________________________________________________________________________
{                                                           // |
    int nMbhlEnable;                                        // |    0x1                 0(bypass) ~ 1(enable)                                                   Control to enable Dialog Clarity or Definition
    int nMbhlBypassGain;                                    // |    0x40000000(1.0)           0x0 ~ 0x40000000(normalized:   0.0 ~ 1.0)                         The bypass gain is only applied when the enable control is adjusted to Bypass setting
    int nMbhlReferenceLevel;                                // |    0x40000000(1.0)      0x10624E ~ 0x40000000(normalized: 0.001 ~ 1.0)                         Set this control to the amount of external headroom applied prior to MBHL processing
    int nMbhlVolume;                                        // |    0x40000000(1.0)           0x0 ~ 0x40000000(normalized:   0.0 ~ 1.0)                         Adjust the volume level based on the master volume level of the audio system
    int nMbhlVolumeStep;                                    // |    0x64                      0x0 ~ 0x64      (normalized:     0 ~ 100)                         Adjust the volume level based on the master volume step of the audio system displayed to the user
    int nMbhlBalanceStep;                                   // |    0x0                       -10 ~ 10        (normalized:   -10 ~ 10)                          Adjust the stereo balance or panning based on the user selected value
    int nMbhlOutputGain;                                    // |    0x40000000(1.0)           0x0 ~ 0x40000000(normalized:   0.0 ~ 1.0)                         This is a straight gain after the Hard Limiter and should only be used in cases
    int nMbhlMode;                                          // |    0x0                 0:Limiter,1:         Compressor Limiter,2:Low   Band Compressor Limiter The Mode control determines the dynamics processing applied to the signal
                                                            // |                                  3:Two Band Compressor Limiter,4:Three Band Compressor Limiter
    int nMbhlProcessDiscrad;                                // |    0x0                         0 ~ 1                                                           Control to discard MBHL processing completely including application of bypass gain
} dtsMbhlCommonApiCtrl;

                                                            // MBHL compressor API Control structure
                                                            //  ____________________________________________________________________________________________________________________________________________________
                                                            // |
                                                            // |    Default             Range                                                                   Description
typedef struct dtsMbhlCompressorApiCtrl                     // |____________________________________________________________________________________________________________________________________________________
{                                                           // |
    int nMbhlCrossLow;                                      // |    0x7                  0:40   Hz,  1:60 Hz,    2:100 Hz,   3:120 Hz                           The Low frequency cutoff of the crossover for multi-band copression modes
                                                            // |                         4:150  Hz,  5:200 Hz,   6:250 Hz,   7:300 Hz
                                                            // |                         8:400  Hz,  9:500 Hz,  10:600 Hz,  11:800 Hz
                                                            // |                        12:1000 Hz, 13:1600 Hz, 14:2000 Hz, 15:3000 Hz
    int nMbhlCrossMid;                                      // |    0xF                 16:4000 Hz, 17:5000 Hz, 18:6000 Hz, 19:8000 Hz, 20:CUSTOM Hz            The Mid frequency cutoff of the crossover for multi-band copression modes
    int nMbhlCompAttack;                                    // |    0x5                       0x0 ~ 0x64      (normalized:     0 ~ 100)                         The Compressor Attack setting adjusts the attack time constant of all employed compressors for the current mode
    int nMbhlCompLowRelease;                                // |    0xFA(250)                0x32 ~ 0x7D0     (normalized:    50 ~ 2000)                        A time of 500ms is recommended for a more subtle effect. Going below 250ms is not recommended
    int nMbhlCompLowRatio;                                  // |    0x10000000(4.0)     0x4000000 ~ 0x50000000(normalized:   1.0 ~ 20.0)                        Ratio of 4 is recommended for a more subtle effect. Higher ratios increase the amount of compression
    int nMbhlCompLowThresh;                                 // |    0x4026808(0.501)    0x83126F  ~ 0x7EC8B400(normalized: 0.064 ~ 15.848)                      Each of the sub bands in the MBHL has a separate control for this
    int nMbhlCompLowMakeup;                                 // |    0x8000000(1.0)      0x83126F  ~ 0x7EC8B400(normalized: 0.064 ~ 15.848)                      There is a separate Make Up Gain for each of the sub bands
    int nMbhlCompMidRelease;                                // |    0xFA(250)               0x32  ~ 0x7D0     (normalized:    50 ~ 2000)                        A time of 500ms is recommended for a more subtle effect. Going below 250ms is not recommended
    int nMbhlCompMidRatio;                                  // |    0x10000000(4.0)     0x4000000 ~ 0x50000000(normalized:   1.0 ~ 20.0)                        Ratio of 4 is recommended for a more subtle effect.
    int nMbhlCompMidThresh;                                 // |    0x4026808(0.501)    0x83126F  ~ 0x7EC8B400(normalized: 0.064 ~ 15.848)                      Each of the sub bands in the MBHL has a separate control for this
    int nMbhlCompMidMakeup;                                 // |    0x8000000(1.0)      0x83126F  ~ 0x7EC8B400(normalized: 0.064 ~ 15.848)                      There is a separate Make Up Gain for each of the sub bands
    int nMbhlCompHighRelease;                               // |    0xFA(250)               0x32  ~ 0x7D0     (normalized:    50 ~ 2000)                        A time of 500ms is recommended for a more subtle effect. Going below 250ms is not recommended
    int nMbhlCompHighRatio;                                 // |    0x10000000(4.0)     0x4000000 ~ 0x50000000(normalized:   1.0 ~ 20.0)                        Ratio of 4 is recommended for a more subtle effect
    int nMbhlCompHighThresh;                                // |    0x4026808(0.501)    0x83126F  ~ 0x7EC8B400(normalized: 0.064 ~ 15.848)                      Each of the sub bands in the MBHL has a separate control for this
    int nMbhlCompHighMakeup;                                // |    0x8000000(1.0)      0x83126F  ~ 0x7EC8B400(normalized: 0.064 ~ 15.848)                      There is a separate Make Up Gain for each of the sub bands
    int nMbhlCompGainReduction[3];                          // |    0x8000000(1.0)      0x83126F  ~ 0x7EC8B400(normalized: 0.064 ~ 15.848)                      Realtime compressor gain
} dtsMbhlCompressorApiCtrl;
                                                            // MBHL limiter API Control structure
                                                            //  ____________________________________________________________________________________________________________________________________________________
                                                            // |
                                                            // |    Default             Range                                                                   Description
typedef struct dtsMbhlLimiterApiCtrl                        // |____________________________________________________________________________________________________________________________________________________
{                                                           // |
    int nMbhlBoost;                                         // |    0x200000(1.0)           0x831 ~ 0x7D000000(normalized: 0.001 ~ 1000.0)                      Adjusts the amount of boost that is applied by the limiter
    int nMbhlThreshold;                                     // |    0x40000000(1.0)     0x4189378 ~ 0x40000000(normalized: 0.064 ~ 1.0)                         The limiter threshold controls the level at which the final limiter will begin to apply its dynamic processing
    int nMbhlSlowOffset;                                    // |    0x20000000(1.0)     0xA24DD30 ~ 0x652F1A80(normalized: 0.317 ~ 3.162)                       This control allows the threshold of the slow gain smoother
    int nMbhlFastAttack;                                    // |    0x28000000(5.0)           0x0 ~ 0x50000000(normalized:   0.0 ~ 10.0)                        The fast attack ballistic of the final limiter determining the rate at which the dynamic gain will be reduced
    int nMbhlFastRelease;                                   // |    0x32(50)                  0xA ~ 0x1F4     (normalized:    10 ~ 500)                         The fast release ballistic of the final limiter for quickly recovery from short-term transient events
    int nMbhlSlowAttack;                                    // |    0x1F4(500)               0x64 ~ 0x3E8     (normalized:   100 ~ 1000)                        The slow attack ballistic of the final limiter
    int nMbhlSlowRelease;                                   // |    0x1F4(500)               0x64 ~ 0x3E8     (normalized:   100 ~ 1000)                        The slow release ballistic of the final limiter
    int nMbhlDelay;                                         // |    0x8(5P3MS)           1:0P6MS, 2:1P3MS, 3:1P9MS, 4:2P6MS                                     The amount of limiter look-ahead delay employed.
                                                            // |                         5:3P3MS, 6:3P9MS, 7:4P6MS, 8:5P3MS                                     More look-ahead will result in better sound quality but will
                                                            // |                         9:5P9MS,10:6P6MS,11:7P3MS,12:7P9MS                                     increase the processing delay
                                                            // |                        13:8P6MS,14:9P3MS,15:9P9MS,16:10P6MS
    int nMbhlEnvelopeFrequency;                             // |    0x14(20)                  0x5 ~ 0x1F4     (normalized:     5 ~ 500)                         The Peak Hold Frequency control will adjust the analysis window of time
    int nMbhlGainReduction;                                 // |    0x40000000(1.0)     0x4189378 ~ 0x40000000(normalized: 0.064 ~ 1.0)                         Realtime final limiter gain reduction
} dtsMbhlLimiterApiCtrl;
                                                            // Multi-band Hard Limiter API Control structure
                                                            //  ____________________________________________________________________________________________________________________________________________________
                                                            // |
                                                            // |    Description
typedef struct dtsMbhlApiCtrl                               // |____________________________________________________________________________________________________________________________________________________
{                                                           // |
    dtsMbhlCommonApiCtrl     mbhlCommonApiControls;         // |    VirtualX controls.          Refer #dtsMbhlCommonApiCtrl
    dtsMbhlCompressorApiCtrl mbhlCompApiControls;           // |    MBHL Compressor controls.   Refer #dtsMbhlCompressorApiCtrl
    dtsMbhlLimiterApiCtrl    mbhlLimApiControls;            // |    MBHL limiter controls.      Refer #dtsMbhlLimiterApiCtrl
} dtsMbhlApiCtrl;
                                                            // TBHDX API control params:
                                                            // Structure containing TBHDX API specific control params.
                                                            //  ____________________________________________________________________________________________________________________________________________________
                                                            // |
                                                            // |    Default             Range                                                                   Description
typedef struct _dtsTBHDxApiCtrl                             // |____________________________________________________________________________________________________________________________________________________
{                                                           // |
    int nEnable;                                            // |    0x0(disable)               0 ~ 1                                                            Enable/Disable TruBass HDx processing
    int nProcMode;                                          // |    0x0(disable)               0 ~ 1                                                            TruBass HDX processing mode
    int nSpkSize;                                           // |    0x2(80Hz)           0:40Hz , 1:60Hz , 2:80Hz , 3:100Hz, 4:120Hz, 5:150Hz, 6:200Hz,          Sets the speaker size setting for tbhdx
                                                            // |                        7:250Hz, 8:300Hz, 9:400Hz,10:500Hz,11:600Hz,12:Custom
    int nDynamics;                                          // |    0x13333333(0.3)          0x0 ~ 0x40000000(normalized:   0.0 ~ 1.0)                          TruBass HDX dynamic range
    int nHPEnable;                                          // |    0x1(enable)                0 ~ 1                                                            Enables/Disables the high-pass filter of the process
    int nHpOrder;                                           // |    0x4                        1 ~ 8                                                            Sets the order of the high-pass filter
    int nBassLvl;                                           // |    0x15586a47(0.334)   0x10624E ~ 0x40000000(normalized: 0.001 ~ 1.0)                          TruBass HDX bass level
    int nTbhdxProcessDiscrad;                               // |    0x0(disable)               0 ~ 1                                                            Control to discard TBHDX processing completely including application of bypass gain
} dtsTBHDxApiCtrl;

typedef struct
{
    dtsVirtualXLib1ApiCtrl_t VirtualxLib1APICtrls;
    dtsMbhlApiCtrl           MbhlAPICtrls;
    dtsTBHDxApiCtrl          TBHDxAPICtrls;
} API_AUDIO_CUSTOMER_DTS_VIRTUALX_PARAM;
                                                            // TBHDX APP control params:
                                                            // Structure containing TBHDX APP specific control params.
                                                            //  ____________________________________________________________________________________________________________________________________________________
                                                            // |
                                                            // |    Default             Range                                                                   Description
                                                            // |____________________________________________________________________________________________________________________________________________________
typedef struct                                              // |
{                                                           // |
    int     spkSize;                                        // |    80                        40 ~ 600                                                          Sets the speaker size setting for tbhdx
    float   bassLvl;                                        // |    0.333521432f        0.333521432f                                                            TruBass HDX bass level
    float   hpRatio;                                        // |    0.5                        0 ~ 1.0                                                          Sets the cut-off frequency of the high-pass filter as a function of the speaker size
    float   extBass;                                        // |    0.8                        0 ~ 1.0                                                          Controls the amount of bass added to the sub-speaker region
} dtsTBHDxAppCtrl;
                                                            // Multi-band Hard Limiter APP control params:
                                                            // Structure containing Multi-band Hard Limiter APP specific control params.
                                                            //  ____________________________________________________________________________________________________________________________________________________
                                                            // |
                                                            // |    Default             Range                                                                   Description
                                                            // |____________________________________________________________________________________________________________________________________________________
typedef struct                                              // |
{                                                           // |
    float   lowCrossFreq;                                   // |    300                       40 ~ 10000                                                        MBHL Low Cross Freq
    float   midCrossFreq;                                   // |    5000                      40 ~ 10000                                                        MBHL Mid Cross Freq
} dtsMbhlAppCtrl;

typedef struct
{
    dtsTBHDxAppCtrl TBHDxAPPCtrls;
    dtsMbhlAppCtrl  MbhlAPPCtrls;
} API_AUDIO_CUSTOMER_DTS_VIRTUALX_APPCTRLS;

//-------------------------------------------------------------------------------------------------------------------------------------
/* callback functions */
//-------------------------------------------------------------------------------------------------------------------------------------
typedef struct
{
    MS_U8   index;      // PCM index
    MS_U32  pts;        // PTS(unit : 90Khz clock base, max value : 0xFFFFFFFF)
    MS_U8   *pData;     // pointer to Audio Data
    MS_U32  dataLen;    // Audio Data Length
} API_AUDIO_CUSTOMER_PCM_DATA;

typedef struct
{
    MS_U32  encFormat;  // Encode format, 0:MP3, 1:AAC
    MS_U64  pts;        // PTS
    MS_U8   *pData;     // pointer to Audio Data
    MS_U32  dataLen;    // Audio Data Length
    MS_U8   *pRStart;   // start pointer of buffer
    MS_U8   *pREnd;     // end pointer of buffer
} API_AUDIO_CUSTOMER_AENC_DATA;

typedef enum
{
    API_AUDIO_CUSTOMER_HDMI_MONITOR_EVENT_INVALID      = -1,
    API_AUDIO_CUSTOMER_HDMI_MONITOR_EVENT_IS_HDMI      = 0,
    API_AUDIO_CUSTOMER_HDMI_MONITOR_EVENT_IS_NONPCM    = 1,
    API_AUDIO_CUSTOMER_HDMI_MONITOR_EVENT_CODEC_CHANGE = 2,
    API_AUDIO_CUSTOMER_HDMI_MONITOR_EVENT_MAX,
} API_AUDIO_CUSTOMER_HDMI_MONITOR_EVENT;

typedef enum
{
    API_AUDIO_CUSTOMER_HDMI_MONITOR_EVENT_INFO_INVALID   = -1,
    API_AUDIO_CUSTOMER_HDMI_MONITOR_EVENT_INFO_DVI_MODE  = 0,
    API_AUDIO_CUSTOMER_HDMI_MONITOR_EVENT_INFO_HDMI_MODE = 1,
    API_AUDIO_CUSTOMER_HDMI_MONITOR_EVENT_INFO_PCM       = 2,
    API_AUDIO_CUSTOMER_HDMI_MONITOR_EVENT_INFO_NONPCM    = 3,
    API_AUDIO_CUSTOMER_HDMI_MONITOR_EVENT_INFO_DD        = 4,
    API_AUDIO_CUSTOMER_HDMI_MONITOR_EVENT_INFO_DTS       = 5,
    API_AUDIO_CUSTOMER_HDMI_MONITOR_EVENT_INFO_DTS_HD_MA = 10, //<--MAX is here!!!
    API_AUDIO_CUSTOMER_HDMI_MONITOR_EVENT_INFO_DDP       = 7,
    API_AUDIO_CUSTOMER_HDMI_MONITOR_EVENT_INFO_DP        = 8,
    API_AUDIO_CUSTOMER_HDMI_MONITOR_EVENT_INFO_OTHER     = 9,
    API_AUDIO_CUSTOMER_HDMI_MONITOR_EVENT_INFO_MAX,
} API_AUDIO_CUSTOMER_HDMI_MONITOR_EVENT_INFO;

typedef MS_BOOL (*pfnAudioCustomerAdecoderClipDone)(API_AUDIO_CUSTOMER_ADEC_INDEX adecIndex);
typedef MS_BOOL (*pfnAudioCustomerAmixerClipDone)(API_AUDIO_CUSTOMER_MIXER_INDEX amixIndex);
typedef MS_BOOL (*pfnAudioCustomerAENCDataHandling)(API_AUDIO_CUSTOMER_AENC_DATA *pMsg);
typedef MS_BOOL (*pfnAudioCustomerPCMDataHandling)(API_AUDIO_CUSTOMER_PCM_DATA *pMsg);
typedef MS_BOOL (*pfnAudioCustomerPCMSending)(MS_U8 *pBuf, MS_U16 length);
typedef MS_BOOL (*pfnAudioCustomerMP3EncodeDone)(MS_U8 *pCopyBuffer, MS_U16 copyLenth);
typedef MS_BOOL (*pfnAudioCustomerAdecDecodeDone)(API_AUDIO_CUSTOMER_ADEC_INDEX adecIndex, MS_U32 copyLenth);
typedef MS_BOOL (*pfnAudioCustomerAdecEndOfStream)(API_AUDIO_CUSTOMER_ADEC_INDEX adecIndex);
typedef MS_BOOL (*pfnAudioCustomerAdecUnderrun)(API_AUDIO_CUSTOMER_ADEC_INDEX adecIndex);
typedef MS_BOOL (*pfnAudioCustomerHDMIMonitorEvent)(API_AUDIO_CUSTOMER_HDMI_MONITOR_EVENT eventType, API_AUDIO_CUSTOMER_HDMI_MONITOR_EVENT_INFO info);

/******************************************************************************
    (Function Declaration)
******************************************************************************/
/* Initialize, STR */
MS_BOOL API_AUDIO_CUSTOMER_InitializeModule(MS_U32 u32MiuNo, AU_PHY mad_va, API_AUDIO_CUSTOMER_OUTPUT_INFO OutputInfo, API_AUDIO_CUSTOMER_CHIP_PLATFORM chipPlatform);
MS_BOOL API_AUDIO_CUSTOMER_InitializeModule_IsFinish(void);
MS_BOOL API_AUDIO_CUSTOMER_Suspend(void);
MS_BOOL API_AUDIO_CUSTOMER_Resume(void);

/* Connect & Disconnect */
MS_BOOL API_AUDIO_CUSTOMER_SetParserMode(API_AUDIO_CUSTOMER_PARSER_MODE ParserMode);
MS_BOOL API_AUDIO_CUSTOMER_ADEC_Connect(API_AUDIO_CUSTOMER_ADEC_INDEX currentConnect, API_AUDIO_CUSTOMER_ADEC_INPUT inputConnect);
MS_BOOL API_AUDIO_CUSTOMER_ADEC_Disconnect(API_AUDIO_CUSTOMER_ADEC_INDEX currentConnect);
MS_BOOL API_AUDIO_CUSTOMER_ADC_Connect(API_AUDIO_CUSTOMER_ADC_INDEX currentConnect, API_AUDIO_CUSTOMER_ADC_IN_PORT portNum);
MS_BOOL API_AUDIO_CUSTOMER_ADC_Disconnect(API_AUDIO_CUSTOMER_ADC_INDEX currentConnect, API_AUDIO_CUSTOMER_ADC_IN_PORT portNum);
MS_BOOL API_AUDIO_CUSTOMER_PCM_Mixer_Connect(API_AUDIO_CUSTOMER_PCM_MIXER_INDEX currentConnect, API_AUDIO_CUSTOMER_PCM_MIXER_INPUT inputConnect);
MS_BOOL API_AUDIO_CUSTOMER_PCM_Mixer_Disconnect(API_AUDIO_CUSTOMER_PCM_MIXER_INDEX currentConnect, API_AUDIO_CUSTOMER_PCM_MIXER_INPUT inputConnect);
MS_BOOL API_AUDIO_CUSTOMER_CH_Sound_Connect(API_AUDIO_CUSTOMER_CH_SOUND currentConnect, API_AUDIO_CUSTOMER_CH_INPUT inputConnect);
MS_BOOL API_AUDIO_CUSTOMER_CH_Sound_Disconnect(API_AUDIO_CUSTOMER_CH_SOUND currentConnect, API_AUDIO_CUSTOMER_CH_INPUT inputConnect);
MS_BOOL API_AUDIO_CUSTOMER_FW_MIXER_Connect(API_AUDIO_CUSTOMER_FWM_INDEX currentConnect, API_AUDIO_CUSTOMER_FWM_INPUT inputConnect);
MS_BOOL API_AUDIO_CUSTOMER_FW_MIXER_Disconnect(API_AUDIO_CUSTOMER_FWM_INDEX currentConnect, API_AUDIO_CUSTOMER_FWM_INPUT inputConnect);
MS_BOOL API_AUDIO_CUSTOMER_SE_Connect(API_AUDIO_CUSTOMER_SE_INDEX currentConnect, API_AUDIO_CUSTOMER_SE_INPUT inputConnect);
MS_BOOL API_AUDIO_CUSTOMER_SE_Disconnect(API_AUDIO_CUSTOMER_SE_INDEX currentConnect, API_AUDIO_CUSTOMER_SE_INPUT inputConnect);
MS_BOOL API_AUDIO_CUSTOMER_SNDOUT_Connect(API_AUDIO_CUSTOMER_SOUNDOUT_INDEX currentConnect, API_AUDIO_CUSTOMER_SOUNDOUT_INPUT inputConnect);
MS_BOOL API_AUDIO_CUSTOMER_SNDOUT_Disconnect(API_AUDIO_CUSTOMER_SOUNDOUT_INDEX currentConnect, API_AUDIO_CUSTOMER_SOUNDOUT_INPUT inputConnect);
MS_BOOL API_AUDIO_CUSTOMER_PCM_CAPTURE_Connect(API_AUDIO_CUSTOMER_PCM_CAPTURE_INDEX currentConnect, API_AUDIO_CUSTOMER_PCM_CAPTURE_INPUT inputConnect);
MS_BOOL API_AUDIO_CUSTOMER_PCM_CAPTURE_Disconnect(API_AUDIO_CUSTOMER_PCM_CAPTURE_INDEX currentConnect, API_AUDIO_CUSTOMER_PCM_CAPTURE_INPUT inputConnect);
MS_BOOL API_AUDIO_CUSTOMER_MP3_ENC_Connect(API_AUDIO_CUSTOMER_MP3_ENC_INPUT inputConnect);
MS_BOOL API_AUDIO_CUSTOMER_MP3_ENC_Disconnect(API_AUDIO_CUSTOMER_MP3_ENC_INPUT inputConnect);
MS_BOOL API_AUDIO_CUSTOMER_AAC_ENC_Connect(API_AUDIO_CUSTOMER_AAC_ENC_INPUT inputConnect);
MS_BOOL API_AUDIO_CUSTOMER_AAC_ENC_Disconnect(API_AUDIO_CUSTOMER_AAC_ENC_INPUT inputConnect);

/* Start & Stop */
MS_BOOL API_AUDIO_CUSTOMER_SetCodecType(API_AUDIO_CUSTOMER_ADEC_INDEX adecIndex, API_AUDIO_CUSTOMER_CODEC_TYPE audioType);
MS_BOOL API_AUDIO_CUSTOMER_StartDecoding(API_AUDIO_CUSTOMER_ADEC_INDEX adecIndex);
MS_BOOL API_AUDIO_CUSTOMER_StopDecoding(API_AUDIO_CUSTOMER_ADEC_INDEX adecIndex);
MS_BOOL API_AUDIO_CUSTOMER_PauseDecoding(API_AUDIO_CUSTOMER_ADEC_INDEX adecIndex);
MS_BOOL API_AUDIO_CUSTOMER_SetMainDecoderOutput(API_AUDIO_CUSTOMER_ADEC_INDEX adecIndex);

/* SPDIF */
MS_BOOL API_AUDIO_CUSTOMER_SPDIF_TX_SetOutputType(API_AUDIO_CUSTOMER_SPDIF_TX_MODE eSPDIFMode);
API_AUDIO_CUSTOMER_SPDIF_TX_MODE API_AUDIO_CUSTOMER_SPDIF_TX_GetOutputType(void);
MS_BOOL API_AUDIO_CUSTOMER_SPDIF_TX_SetCopyInfo(API_AUDIO_CUSTOMER_SPDIF_COPYRIGHT copyInfo);
MS_BOOL API_AUDIO_CUSTOMER_SPDIF_TX_SetCategoryCode(MS_U8 categoryCode);
MS_BOOL API_AUDIO_CUSTOMER_SPDIF_TX_SetLightOnOff(MS_BOOL bOnOff);
MS_BOOL API_AUDIO_CUSTOMER_SPDIF_TX_SetMonitorOnOff(MS_BOOL bOnOff);
MS_BOOL API_AUDIO_CUSTOMER_SPDIF_TX_ChannelStatus_CTRL(API_AUDIO_CUSTOMER_SPDIF_CS_TYPE cs_mode, API_AUDIO_CUSTOMER_SPDIF_CS_TYPE_STATUS status);
MS_BOOL API_AUDIO_CUSTOMER_SPDIF_RX_GetAudioMode(API_AUDIO_CUSTOMER_CODEC_TYPE *pSpdifRxMode);
MS_BOOL API_AUDIO_CUSTOMER_SPDIF_RX_SetMonitorOnOff(MS_BOOL bOnOff);

/* HDMI */
MS_BOOL API_AUDIO_CUSTOMER_HDMI_RX_GetAudioMode(API_AUDIO_CUSTOMER_CODEC_TYPE *pHDMIMode);
MS_BOOL API_AUDIO_CUSTOMER_HDMI_RX_GetCopyInfo(API_AUDIO_CUSTOMER_SPDIF_COPYRIGHT *pCopyInfo);
MS_BOOL API_AUDIO_CUSTOMER_HDMI_RX_SetAudioReturnChannel(MS_BOOL bOnOff);
MS_BOOL API_AUDIO_CUSTOMER_HDMI_RX_SetMonitorOnOff(MS_BOOL bOnOff);
MS_BOOL API_AUDIO_CUSTOMER_HDMI_TX_SetOutputType(API_AUDIO_CUSTOMER_HDMI_TX_OUTPUT_TYPE eHDMIMode);
MS_BOOL API_AUDIO_CUSTOMER_HDMI_TX_SetMonitorOnOff(MS_BOOL bOnOff);

/* ATV */
MS_BOOL API_AUDIO_CUSTOMER_SIF_SetInputSource(API_AUDIO_CUSTOMER_SIF_INPUT sifSource);
MS_BOOL API_AUDIO_CUSTOMER_SIF_SetHighDevMode(API_AUDIO_CUSTOMER_SIF_HIDEV_BW bandwidth, MS_BOOL bOnOff);
MS_BOOL API_AUDIO_CUSTOMER_SIF_SetAudioStandard(API_AUDIO_CUSTOMER_SIF_STANDARD sifStandard);
MS_BOOL API_AUDIO_CUSTOMER_SIF_SetSoundMode(API_AUDIO_CUSTOMER_SIF_AUDIOMODE_TYPE u8SifSoundMode);
API_AUDIO_CUSTOMER_SIF_AUDIOMODE_TYPE API_AUDIO_CUSTOMER_SIF_GetSoundMode(void);
MS_BOOL API_AUDIO_CUSTOMER_SIF_SetMonitorOnOff(MS_BOOL bOnOff);
MS_BOOL API_AUDIO_CUSTOMER_SIF_SetAutoMute(MS_BOOL bOnOff);
API_AUDIO_CUSTOMER_SIF_STANDARD API_AUDIO_CUSTOMER_SIF_StartAutoStandardDetection(void);
MS_BOOL API_AUDIO_CUSTOMER_SIF_SetThreshold(API_AUDIO_CUSTOMER_SIF_THR_TBL_TYPE *ThrTbl);
MS_BOOL API_AUDIO_CUSTOMER_SIF_SetPALType(API_AUDIO_CUSTOMER_SIF_PAL_TYPE pal_type);
MS_BOOL API_AUDIO_CUSTOMER_SIF_SendCmd(API_AUDIO_CUSTOMER_SIF_CmdType enAudSifCommand, MS_U8 comm_arg1, MS_U8 comm_arg2);
MS_BOOL API_AUDIO_CUSTOMER_SIF_GetAudioStatus(API_AUDIO_CUSTOMER_SIF_AUDIOSTATUS *eCurrentAudioStatus);
MS_BOOL API_AUDIO_CUSTOMER_SIF_IsPALType(API_AUDIO_CUSTOMER_SIF_PAL_TYPE pal_type);
MS_BOOL API_AUDIO_CUSTOMER_SIF_SetPrescale(API_AUDIO_CUSTOMER_SIF_GAIN_TYPE gain_type, MS_S32 db_value); // db_value: The prescale value, unit is 0.25dB/Step.
                                                                                                         // 0: 0db, 1: 0.25dB,  2: 0.5dB, ...,  4: 1.0dB, ...,  8: 2dB
                                                                                                         //        -1:-0.25dB, -2:-0.5dB, ..., -4:-1.0dB, ..., -8:-2dB


/* Decoder */
MS_BOOL API_AUDIO_CUSTOMER_SetSyncMode(API_AUDIO_CUSTOMER_ADEC_INDEX adecIndex, MS_BOOL bOnOff);
MS_BOOL API_AUDIO_CUSTOMER_GetDecodingType(API_AUDIO_CUSTOMER_ADEC_INDEX adecIndex, API_AUDIO_CUSTOMER_CODEC_TYPE *pAudioType);
MS_BOOL API_AUDIO_CUSTOMER_SetDualMonoOutMode(API_AUDIO_CUSTOMER_ADEC_INDEX adecIndex, API_AUDIO_CUSTOMER_DUALMONO_MODE outputMode);
MS_BOOL API_AUDIO_CUSTOMER_GetESInfo(API_AUDIO_CUSTOMER_ADEC_INDEX adecIndex, void * pAudioESInfo);
MS_BOOL API_AUDIO_CUSTOMER_IsESExist(API_AUDIO_CUSTOMER_ADEC_INDEX adecIndex);
MS_BOOL API_AUDIO_CUSTOMER_SetAudioDescription(API_AUDIO_CUSTOMER_ADEC_INDEX adecIndex, MS_BOOL bOnOff);
MS_BOOL API_AUDIO_CUSTOMER_SetTrickMode(API_AUDIO_CUSTOMER_ADEC_INDEX adecIndex, API_AUDIO_CUSTOMER_TRICK_MODE eTrickMode);
MS_BOOL API_AUDIO_CUSTOMER_GetBufferStatus(API_AUDIO_CUSTOMER_ADEC_INDEX adecIndex, MS_U32 *pMaxSize, MS_U32 *pFreeSize);
MS_BOOL API_AUDIO_CUSTOMER_SetAdecPcmPath(API_AUDIO_CUSTOMER_ADEC_INDEX adecIndex, API_AUDIO_CUSTOMER_ADEC_PCM_PATH pcmPath);
MS_BOOL API_AUDIO_CUSTOMER_AdecPcmReady(API_AUDIO_CUSTOMER_ADEC_INDEX adecIndex);
MS_U32  API_AUDIO_CUSTOMER_AdecPcmGet(API_AUDIO_CUSTOMER_ADEC_INDEX adecIndex, void * pOutPcm, MS_U32 u32Size);
MS_BOOL API_AUDIO_CUSTOMER_AdecRegisterDecodeDoneCallback(API_AUDIO_CUSTOMER_ADEC_INDEX adecIndex, pfnAudioCustomerAdecDecodeDone pfnDecodeDoneCallBack);
MS_BOOL API_AUDIO_CUSTOMER_AdecRegisterEndOfStreamCallback(API_AUDIO_CUSTOMER_ADEC_INDEX adecIndex, pfnAudioCustomerAdecEndOfStream pfnEndOfStreamCallBack);
MS_BOOL API_AUDIO_CUSTOMER_AdecRegisterUnderrunCallback(API_AUDIO_CUSTOMER_ADEC_INDEX adecIndex, pfnAudioCustomerAdecUnderrun pfnUnderrunCallBack);
MS_BOOL API_AUDIO_CUSTOMER_Auto_Recovery_SetMonitorOnOff(MS_BOOL bOnOff);

/* Common Decoder */
MS_S32 API_AUDIO_CUSTOMER_COMMON_DECODER_Open(void * pData);
MS_S32 API_AUDIO_CUSTOMER_COMMON_DECODER_Close(MS_S32 s32DeviceID);
MS_S32 API_AUDIO_CUSTOMER_COMMON_DECODER_Start(MS_S32 s32DeviceID);
MS_S32 API_AUDIO_CUSTOMER_COMMON_DECODER_Stop(MS_S32 s32DeviceID);
MS_S32 API_AUDIO_CUSTOMER_COMMON_DECODER_Set(MS_S32 s32DeviceID, API_AUDIO_CUSTOMER_COMMON_DECODER_COMMAND Cmd, void* pData);
MS_S32 API_AUDIO_CUSTOMER_COMMON_DECODER_Get(MS_S32 s32DeviceID, API_AUDIO_CUSTOMER_COMMON_DECODER_COMMAND Cmd, void* pData);
MS_U32 API_AUDIO_CUSTOMER_COMMON_DECODER_Read(MS_S32 s32DeviceID, void* pBuf, MS_U32 u32Size);
MS_U32 API_AUDIO_CUSTOMER_COMMON_DECODER_Write(MS_S32 s32DeviceID, void* pBuf, MS_U32 u32Size);
MS_S32 API_AUDIO_CUSTOMER_COMMON_DECODER_Flush(MS_S32 s32DeviceID);

/* Common */
MS_BOOL API_AUDIO_CUSTOMER_SetAudioParam(API_AUDIO_CUSTOMER_ADEC_INDEX adecIndex, API_AUDIO_CUSTOMER_PARAM_TYPE paramType, void *pParam);
MS_BOOL API_AUDIO_CUSTOMER_GetAudioInfo(API_AUDIO_CUSTOMER_ADEC_INDEX  adecIndex, API_AUDIO_CUSTOMER_INFO_TYPE  infoType,  void *pInfo);

/* Customized patch */
MS_BOOL API_AUDIO_CUSTOMIZED_PATCH_SetAudioParam(API_AUDIO_CUSTOMER_ADEC_INDEX adecIndex, API_AUDIO_CUSTOMIZED_PATCH_PARAM_TYPE paramType, void *pParam);
MS_BOOL API_AUDIO_CUSTOMIZED_PATCH_GetAudioInfo(API_AUDIO_CUSTOMER_ADEC_INDEX  adecIndex, API_AUDIO_CUSTOMIZED_PATCH_INFO_TYPE  infoType,  void *pInfo);

/* Customized Unit control */
MS_BOOL API_AUDIO_CUSTOMIZED_Unit_Ctrl(char *pStrCmd, MS_U32 u32StrCmdLength, void *pData, MS_U32 u32DataLength);

/* Clip Play for ES */
MS_BOOL API_AUDIO_CUSTOMER_PlayClipDecoder(API_AUDIO_CUSTOMER_ADEC_INDEX adecIndex, API_AUDIO_CUSTOMER_CLIP_DEC_PARAM clipInfo, MS_U32 bufSize, void *pBufClip, pfnAudioCustomerAdecoderClipDone pfnCallBack);
MS_BOOL API_AUDIO_CUSTOMER_StopClipDecoder(API_AUDIO_CUSTOMER_ADEC_INDEX adecIndex);
MS_BOOL API_AUDIO_CUSTOMER_PauseClipDecoder(API_AUDIO_CUSTOMER_ADEC_INDEX adecIndex);
MS_BOOL API_AUDIO_CUSTOMER_ResumeClipDecoder(API_AUDIO_CUSTOMER_ADEC_INDEX adecIndex);

/* Clip Play for PCM */
MS_BOOL API_AUDIO_CUSTOMER_PlayClipMixer(API_AUDIO_CUSTOMER_MIXER_INDEX mixerIndex, API_AUDIO_CUSTOMER_CLIP_MIX_PARAM clipInfo, MS_U32 bufSize, void *pBufClip, pfnAudioCustomerAmixerClipDone pfnCallBack);
MS_BOOL API_AUDIO_CUSTOMER_StopClipMixer(API_AUDIO_CUSTOMER_MIXER_INDEX mixerIndex);
MS_BOOL API_AUDIO_CUSTOMER_PauseClipMixer(API_AUDIO_CUSTOMER_MIXER_INDEX mixerIndex);
MS_BOOL API_AUDIO_CUSTOMER_ResumeClipMixer(API_AUDIO_CUSTOMER_MIXER_INDEX mixerIndex);

/* Gain, Mute & Delay */
//---Gain---
MS_BOOL API_AUDIO_CUSTOMER_SetAudioDescriptionGain(API_AUDIO_CUSTOMER_ADEC_INDEX adecIndex, API_AUDIO_CUSTOMER_ADMIX_GAIN_IDX adMixGainIdx, MS_U32 gain);
MS_BOOL API_AUDIO_CUSTOMER_SetPCMMixerInputGain(API_AUDIO_CUSTOMER_MIXER_INDEX mixerIndex, MS_U32 gain);
MS_BOOL API_AUDIO_CUSTOMER_SetChannelGain(API_AUDIO_CUSTOMER_CH_SOUND ch, MS_U32 gain);
MS_BOOL API_AUDIO_CUSTOMER_SetFWMixerChannelGain(API_AUDIO_CUSTOMER_FWM_INDEX currentConnect, API_AUDIO_CUSTOMER_FWM_INPUT inputConnect, MS_U32 gain);
MS_BOOL API_AUDIO_CUSTOMER_SetI2SOutGain(MS_U32 gain);
MS_BOOL API_AUDIO_CUSTOMER_SetLineOutGain(API_AUDIO_CUSTOMER_LINE_OUT_IDX lineIndex, MS_U32 gain);
MS_BOOL API_AUDIO_CUSTOMER_SetSPDIFOutGain(MS_U32 gain);
MS_BOOL API_AUDIO_CUSTOMER_SetHDMIOutGain(MS_U32 gain);
//---Mute---
MS_BOOL API_AUDIO_CUSTOMER_SetADECMute(API_AUDIO_CUSTOMER_ADEC_INDEX adecIndex, MS_BOOL bOnOff);
MS_BOOL API_AUDIO_CUSTOMER_SetPCMMixerInputMute(API_AUDIO_CUSTOMER_MIXER_INDEX mixerIndex, MS_BOOL bOnOff);
MS_BOOL API_AUDIO_CUSTOMER_SetChannelMute(API_AUDIO_CUSTOMER_CH_SOUND ch, MS_BOOL bOnOff);
MS_BOOL API_AUDIO_CUSTOMER_SetFWMixerChannelMute(API_AUDIO_CUSTOMER_FWM_INDEX currentConnect, API_AUDIO_CUSTOMER_FWM_INPUT inputConnect, MS_BOOL bOnOff);
MS_BOOL API_AUDIO_CUSTOMER_SetI2SOutMute(MS_BOOL bOnOff);
MS_BOOL API_AUDIO_CUSTOMER_SetLineOutMute(API_AUDIO_CUSTOMER_LINE_OUT_IDX lineIndex, MS_BOOL bOnOff);
MS_BOOL API_AUDIO_CUSTOMER_SetSPDIFOutMute(MS_BOOL bOnOff);
MS_BOOL API_AUDIO_CUSTOMER_SetHDMIOutMute(MS_BOOL bOnOff);
MS_BOOL API_AUDIO_CUSTOMER_GetI2SOutMuteStatus(MS_BOOL *pOnOff);
MS_BOOL API_AUDIO_CUSTOMER_GetLineOutMuteStatus(API_AUDIO_CUSTOMER_LINE_OUT_IDX lineIndex, MS_BOOL *pOnOff);
MS_BOOL API_AUDIO_CUSTOMER_GetSPDIFOutMuteStatus(MS_BOOL *pOnOff);
MS_BOOL API_AUDIO_CUSTOMER_MuteDuringLimitedTime_Input(MS_U32 per_50ms, API_AUDIO_CUSTOMER_FWM_INDEX eFWM, API_AUDIO_CUSTOMER_FWM_INPUT eCh);
MS_BOOL API_AUDIO_CUSTOMER_MuteDuringLimitedTime_Output(MS_U32 per_50ms, API_AUDIO_CUSTOMER_OUTPUT_TYPE ePort);
//---Delay---
MS_BOOL API_AUDIO_CUSTOMER_SetADECDelay(API_AUDIO_CUSTOMER_ADEC_INDEX adecIndex, MS_U32 delay);
MS_BOOL API_AUDIO_CUSTOMER_SetChannelDelay(API_AUDIO_CUSTOMER_CH_SOUND ch, MS_U32 delay);
MS_BOOL API_AUDIO_CUSTOMER_SetAudioDelay(MS_U32 delay);
MS_BOOL API_AUDIO_CUSTOMER_SetSpdifDelay(MS_U32 delay);
MS_BOOL API_AUDIO_CUSTOMER_SetHdmiDelay(MS_U32 delay);

/* AENC */
MS_BOOL API_AUDIO_CUSTOMER_AENC_Start(API_AUDIO_CUSTOMER_AENC_ENCODING_FORMAT encFormat);
MS_BOOL API_AUDIO_CUSTOMER_AENC_Stop(API_AUDIO_CUSTOMER_AENC_ENCODING_FORMAT encFormat);
MS_BOOL API_AUDIO_CUSTOMER_AENC_RegisterCallback(API_AUDIO_CUSTOMER_AENC_ENCODING_FORMAT encFormat, pfnAudioCustomerAENCDataHandling pfnCallBack);
MS_BOOL API_AUDIO_CUSTOMER_AENC_SetInfo(API_AUDIO_CUSTOMER_AENC_ENCODING_FORMAT encFormat, API_AUDIO_CUSTOMER_AENC_INFO info);
MS_BOOL API_AUDIO_CUSTOMER_AENC_GetInfo(API_AUDIO_CUSTOMER_AENC_ENCODING_FORMAT encFormat, API_AUDIO_CUSTOMER_AENC_INFO *pInfo);
MS_BOOL API_AUDIO_CUSTOMER_AENC_SetGain(API_AUDIO_CUSTOMER_AENC_ENCODING_FORMAT encFormat, MS_U32 gain);
MS_BOOL API_AUDIO_CUSTOMER_AENC_CopyData(API_AUDIO_CUSTOMER_AENC_ENCODING_FORMAT encFormat, MS_U8 *pDest, MS_U8 *pBufAddr, MS_U32 datasize, MS_U8 *pRStart, MS_U8 *pREnd);
MS_BOOL API_AUDIO_CUSTOMER_AENC_ReleaseData(API_AUDIO_CUSTOMER_AENC_ENCODING_FORMAT encFormat, MS_U8 *pBufAddr, MS_U32 datasize);

/* PCM Capture */
MS_BOOL API_AUDIO_CUSTOMER_PCM_StartUpload(API_AUDIO_CUSTOMER_PCM_CAPTURE_INDEX currentConnect);
MS_BOOL API_AUDIO_CUSTOMER_PCM_StopUpload(API_AUDIO_CUSTOMER_PCM_CAPTURE_INDEX currentConnect);
MS_BOOL API_AUDIO_CUSTOMER_PCM_RegisterSendPCMCallback(API_AUDIO_CUSTOMER_PCM_CAPTURE_INDEX currentConnect, pfnAudioCustomerPCMSending pfnCallBack);
MS_BOOL API_AUDIO_CUSTOMER_PCM_SetGain(API_AUDIO_CUSTOMER_PCM_CAPTURE_INDEX currentConnect, MS_U32 gain);
MS_BOOL API_AUDIO_CUSTOMER_PCM_SetMute(API_AUDIO_CUSTOMER_PCM_CAPTURE_INDEX currentConnect, MS_BOOL bOnOff);
MS_BOOL API_AUDIO_CUSTOMER_PCM_SetRequestSizeInMs(MS_U32 request_ms);

/* PCM IO Control */
MS_S32  API_AUDIO_CUSTOMER_PCM_Open(API_AUDIO_CUSTOMER_PCMIO_PARAM *pData);
MS_S32  API_AUDIO_CUSTOMER_PCM_Close(MS_S32 s32DeviceId);
MS_S32  API_AUDIO_CUSTOMER_PCM_Start(MS_S32 s32DeviceId);
MS_S32  API_AUDIO_CUSTOMER_PCM_Stop(MS_S32 s32DeviceId);
MS_S32  API_AUDIO_CUSTOMER_PCM_Set(MS_S32 s32DeviceId, MS_U32 u32Cmd, const void *pData);
MS_S32  API_AUDIO_CUSTOMER_PCM_Get(MS_S32 s32DeviceId, MS_U32 u32Cmd, void *pData);
MS_U32  API_AUDIO_CUSTOMER_PCM_Read(MS_S32 s32DeviceId, void *pBuf, MS_U32 u32Size);
MS_U32  API_AUDIO_CUSTOMER_PCM_Write(MS_S32 s32DeviceId, const void *pBuf, MS_U32 u32Size);
MS_S32  API_AUDIO_CUSTOMER_PCM_Flush(MS_S32 s32DeviceId);

/* MM New Mode */
MS_PHY  API_AUDIO_CUSTOMER_GetDDRInfo(API_AUDIO_CUSTOMER_ADEC_INDEX adecIndex, API_AUDIO_CUSTOMER_DDRINFO DDRInfo);
MS_BOOL API_AUDIO_CUSTOMER_MM2_initAesInfo(API_AUDIO_CUSTOMER_ADEC_INDEX adecIndex);
MS_BOOL API_AUDIO_CUSTOMER_MM2_checkAesInfo(API_AUDIO_CUSTOMER_ADEC_INDEX adecIndex, API_AUDIO_CUSTOMER_AES_INFO *aes_info);
MS_BOOL API_AUDIO_CUSTOMER_MM2_inputAesFinished(API_AUDIO_CUSTOMER_ADEC_INDEX adecIndex, MS_U32 es_size, MS_BOOL ptsExist, MS_U64 pts);
MS_BOOL API_AUDIO_CUSTOMER_MM2_AD_initAesInfo(API_AUDIO_CUSTOMER_ADEC_INDEX adecIndex);
MS_BOOL API_AUDIO_CUSTOMER_MM2_AD_checkAesInfo(API_AUDIO_CUSTOMER_ADEC_INDEX adecIndex, API_AUDIO_CUSTOMER_AES_INFO *aes_info);
MS_BOOL API_AUDIO_CUSTOMER_MM2_AD_inputAesFinished(API_AUDIO_CUSTOMER_ADEC_INDEX adecIndex, MS_U32 es_size, MS_BOOL ptsExist, MS_U64 pts, MS_U8 fade, MS_U8 pan); //fade: 0x0~0xFE, pan: 0xEB~0x15

/* Mstar Sound Effect */
MS_BOOL API_AUDIO_CUSTOMER_MSTAR_SE_Enable(API_AUDIO_CUSTOMER_MSTAR_SE_TYPE Type, MS_BOOL bOnOff);

///@brief \b Function \b Name: API_AUDIO_CUSTOMER_MSTAR_SE_SET_GEQ()
///@brief \b Function \b Description: This routine is used to set the the 5-band GEQ u8Level
///@param <IN> \b u8band    :  GEQ band  0~4 // 0:120Hz, 1:500Hz, 2:1500Hz, 3:5000Hz, 4:10000Hz
///@param <IN> \b s8level   :  GEQ level -48~48(-12dB~12dB, step 0.25dB)
MS_BOOL API_AUDIO_CUSTOMER_MSTAR_SE_SET_GEQ(MS_U8 u8band, MS_S8 s8level);

///@brief \b Function \b Name: API_AUDIO_CUSTOMER_MSTAR_SE_SET_PEQCoef()
///@brief \b Function \b Description: This routine is used to set the PEQ parameters
///@param <IN> \b peq_coef->band    :  PEQ band select ( 0 ~ 7 )
///@param <IN> \b peq_coef->enable  :  PEQ band enable/disable ( 0:disable  1:enable )
///@param <IN> \b peq_coef->a0      :  PEQ coefficient a0 (check below sampele code to calculate coefficient)
///@param <IN> \b peq_coef->a1      :  PEQ coefficient a1 (check below sampele code to calculate coefficient)
///@param <IN> \b peq_coef->a2      :  PEQ coefficient a2 (check below sampele code to calculate coefficient)
///@param <IN> \b peq_coef->b1      :  PEQ coefficient b1 (check below sampele code to calculate coefficient)
///@param <IN> \b peq_coef->b2      :  PEQ coefficient b2 (check below sampele code to calculate coefficient)
///
/// @@@ Here are sample codes to calculate PEQ Coefficents @@@
/// @param <IN> \b Gain: -12dB ~ 12dB
/// @param <IN> \b Fc (Hz)
/// @param <IN> \b Q : 0.5~16.0 (band width: wide to narrow)
///void PEQ_Coefficent_Calculation(const float Gain, const int Fc, const float Q)
///{
///    float coef;
///    float G, fc, Q;
///    float fb,d,v0,H0,aBC,fm,fz,kb;
///    unsigned int a0, a1, a2, b1, b2;
///
///    fb = Fc / Q;
///    d = - cos( 2 * 3.1415926 * Fc / 48000);
///    v0 = powf(10.0, (Gain / 20.0));
///    H0 = v0 -1;
///    aBC=0;
///    fm=0;
///    fz=1;
///    kb = tan ( 3.1415926 * fb / 48000);
///
///    if (Gain >=0)
///    {
///        fz = kb - 1;
///        fm = kb + 1;
///    }
///    else
///    {
///        fz = kb - v0;
///        fm = kb + v0;
///    }
///
///    aBC = fz / fm ;
///    coef = 1 + (1 + aBC) * H0 / 2;
///    a0 = (long)(coef * 4194304);
///    coef = d * (1 - aBC);
///    a1 = (long)(coef * 4194304);
///    coef = -aBC - (1 + aBC) * H0 /2;
///    a2 = (long)(coef * 4194304);
///    coef = d * (1 - aBC);
///    b1 = (long)(-coef * 4194304);
///    coef = -aBC;
///    b2 = (long)(-coef * 4194304);
///
///    printf("PEQ Coefficient:\n");
///    printf("a0:%lx \n",a0));
///    printf("a1:%lx \n",a1));
///    printf("a2:%lx \n",a2));
///    printf("b1:%lx \n",b1));
///    printf("b2:%lx \n",b2));
///}
MS_BOOL API_AUDIO_CUSTOMER_MSTAR_SE_SET_PEQCoef(API_AUDIO_CUSTOMER_MSTAR_PEQ_COEF *peq_coef);

///@brief \b Function \b Name: API_AUDIO_CUSTOMER_MSTAR_SE_SET_HPFCoef()
///@brief \b Function \b Description: This routine is used to set the HPF parameters
///@param <IN> \b hpf_coef->a0      :  HPF coefficient a0 (check below sampele code to calculate coefficient)
///@param <IN> \b hpf_coef->a1      :  HPF coefficient a1 (check below sampele code to calculate coefficient)
///@param <IN> \b hpf_coef->a2      :  HPF coefficient a2 (check below sampele code to calculate coefficient)
///@param <IN> \b hpf_coef->b1      :  HPF coefficient b1 (check below sampele code to calculate coefficient)
///@param <IN> \b hpf_coef->b2      :  HPF coefficient b2 (check below sampele code to calculate coefficient)
///
/// @@@ Here are sample codes to calculate HPF Coefficents @@@
/// @param <IN> \b Fc (Hz): 50Hz~200Hz, step 1Hz
///void HPF_Coefficent_Calculation( const int Fc )
///{
///    float coef[5];
///    float dem;
///    float Q;
///    float k;
///    float kpow2;
///    unsigned int a0, a1, a2, b1, b2;
///
///    k = tan((3.1415926*Fc)/48000);
///    Q = 1/sqrt(2);
///
///    kpow2 = k*k;
///
///    dem = 1 + k/Q + kpow2;
///    coef[0] = 1/dem;
///    coef[1] = -2/dem;
///    coef[2] = 1/dem;
///    coef[3] = 2*(kpow2-1)/dem;
///    coef[4] = (1 - k/Q + kpow2)/dem;
///
///    a0 = (long)(coef[0] * 8388608/2);
///    a1 = (long)(coef[1] * 8388608/2);
///    a2 = (long)(coef[2] * 8388608/2);
///    b1 = (long)(-coef[3] * 8388608/2);
///    b2 = (long)(-coef[4] * 8388608/2);
///    printf("HPF Coefficient:\n");
///    printf("a0:%lx \n",a0));
///    printf("a1:%lx \n",a1));
///    printf("a2:%lx \n",a2));
///    printf("b1:%lx \n",b1));
///    printf("b2:%lx \n",b2));
///}
MS_BOOL API_AUDIO_CUSTOMER_MSTAR_SE_SET_HPFCoef(API_AUDIO_CUSTOMER_MSTAR_HPF_COEF *hpf_coef);

MS_BOOL API_AUDIO_CUSTOMER_MSTAR_SE_SET_AVC_MODE(API_AUDIO_CUSTOMER_MSTAR_AVC_MODE mode);

///@brief \b Function \b Name: API_AUDIO_CUSTOMER_MSTAR_SE_SET_AVC_LEVEL()
///@brief \b Function \b Description: This routine is used to set the AVC clipping level
///@param <IN> \b u16level    :  AVC clipping u16level
///                              0x00--  0      dBFS
///                              0x01--  -0.5   dBFS
///                              0x20--  -16    dBFS
///                              0x50--  -40    dBFS
MS_BOOL API_AUDIO_CUSTOMER_MSTAR_SE_SET_AVC_LEVEL(MS_U16 u16level);

///@brief \b Function \b Name: API_AUDIO_CUSTOMER_MSTAR_SE_SET_AVC_R_MODE_Begin()
///@brief \b Function \b Description: This routine is used to set the AVC R_Mode begin
///@param <IN> \b u16Begin    : default: 50 (-50dB), range: 0~60 (0dB ~ -60dB), End Must larger than Begin
MS_BOOL API_AUDIO_CUSTOMER_MSTAR_SE_SET_AVC_R_MODE_Begin(MS_U16 u16Begin);

///@brief \b Function \b Name: API_AUDIO_CUSTOMER_MSTAR_SE_SET_AVC_R_MODE_End()
///@brief \b Function \b Description: This routine is used to set the AVC R_Mode End
///@param <IN> \b u16End    : default: 20 (-20dB), range: 0~60 (0dB ~ -60dB), End Must larger than Begin
MS_BOOL API_AUDIO_CUSTOMER_MSTAR_SE_SET_AVC_R_MODE_End(MS_U16 u16End);

///@brief \b Function \b Name: API_AUDIO_CUSTOMER_MSTAR_SE_SET_AVC_R_MODE_Slope()
///@brief \b Function \b Description: This routine is used to set the AVC R_Mode offset
///@param <IN> \b u16Slope    : range: 1 ~ 3 (default: 2)
///                       - u16Slope = 1 --> 0.75
///                       - u16Slope = 2 --> 0.5
///                       - u16Slope = 3 --> 0.25
MS_BOOL API_AUDIO_CUSTOMER_MSTAR_SE_SET_AVC_R_MODE_Slope(MS_U16 u16Slope);

///@brief \b Function \b Name: API_AUDIO_CUSTOMER_MSTAR_SE_SET_AVC_S_MODE_OFFSET()
///@brief \b Function \b Description: This routine is used to set the AVC L_Mode offset (min value)
///@param <IN> \b u16offset    : range: +0 ~ +12 dB
MS_BOOL API_AUDIO_CUSTOMER_MSTAR_SE_SET_AVC_S_MODE_OFFSET(MS_U16 u16offset);

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: API_AUDIO_CUSTOMER_MSTAR_SE_SET_AVC_AttackTime()
/// @brief \b Function \b Description: This routine is used to set the AVC attack time.
/// @param AvcAT       \b :  AVC attack time
///                       - AvcAT = 0 --> 20  ms
///                       - AvcAT = 1 --> 100 ms
///                       - AvcAT = 2 --> 200 ms
///                       - AvcAT = 3 --> 1 sec
////////////////////////////////////////////////////////////////////////////////
MS_BOOL API_AUDIO_CUSTOMER_MSTAR_SE_SET_AVC_AttackTime(MS_U8 AvcAT);

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: API_AUDIO_CUSTOMER_MSTAR_SE_SET_AVC_ReleaseTime()
/// @brief \b Function \b Description: This routine is used to set the AVC  release time .
/// @param AvcRT       \b :  AVC release time
///                       - AvcRT = 0 --> 1 sec
///                       - AvcRT = 1 --> 2 sec
///                       - AvcRT = 2 --> 100 ms
///                       - AvcRT = 3 --> 200 ms
////////////////////////////////////////////////////////////////////////////////
MS_BOOL API_AUDIO_CUSTOMER_MSTAR_SE_SET_AVC_ReleaseTime(MS_U8 AvcRT);

///@brief \b Function \b Name: API_AUDIO_CUSTOMER_MSTAR_SE_SET_DRC_LEVEL()
///@brief \b Function \b Description: This routine is used to set the DRC clipping level
///@param <IN> \b u16level    :  DRC clipping u16level
///                              0x00--  0      dBFS
///                              0x01--  -0.5   dBFS
///                              0x20--  -16    dBFS
///                              0x50--  -40    dBFS
MS_BOOL API_AUDIO_CUSTOMER_MSTAR_SE_SET_DRC_LEVEL(MS_U16 u16level);

///@brief \b Function \b Name: API_AUDIO_CUSTOMER_MSTAR_SE_SET_BALANCE()
///@brief \b Function \b Description: This routine is used to set the volume balance
///@param <IN> \b Lbalance    :  balance L  0x00(0dB)~0xFF(mute)  step: -0.25dB
///@param <IN> \b Lbalance    :  balance R  0x00(0dB)~0xFF(mute)  step: -0.25dB
MS_BOOL API_AUDIO_CUSTOMER_MSTAR_SE_SET_BALANCE(MS_U16 Lbalance, MS_U16 Rbalance);

///@brief \b Function \b Name: API_AUDIO_CUSTOMER_MSTAR_SE_SET_NR_Threshold()
///@brief \b Function \b Description: This routine is used to set noise reduction threshold
///@param <IN> \b u16level    :  NR level
MS_BOOL API_AUDIO_CUSTOMER_MSTAR_SE_SET_NR_Threshold(MS_U16 u16level);

///@brief \b Function \b Name: API_AUDIO_CUSTOMER_MSTAR_SE_GET_NR_Status()
///@brief \b Function \b Description: This routine is used to get noise reduction status
///@param <IN> \b u16level    :  NR level
MS_S32 API_AUDIO_CUSTOMER_MSTAR_SE_GET_NR_Status(void);

/* Advanced Sound Effect */
MS_BOOL API_AUDIO_CUSTOMER_DTS_SE_Enable(API_AUDIO_CUSTOMER_DTS_SE_TYPE seType);
MS_BOOL API_AUDIO_CUSTOMER_DTS_SE_ProcessUnit_Enable(API_AUDIO_CUSTOMER_DTS_SE_UNIT_TYPE seUnit, MS_BOOL bOnOff);
MS_BOOL API_AUDIO_CUSTOMER_DTS_SE_SetParam(API_AUDIO_CUSTOMER_DTS_SE_PARAM_TYPE param, MS_U32 u32value);
MS_BOOL API_AUDIO_CUSTOMER_DTS_SE_SetAEQCoef(API_AUDIO_CUSTOMER_DTS_SE_AEQ_COEF *aeq_coef);
MS_BOOL API_AUDIO_CUSTOMER_Dolby_DAP_Enable(MS_BOOL bOnOff);
MS_BOOL API_AUDIO_CUSTOMER_Dolby_DAP_ProcessUnit_Enable(MS_BOOL bOnOff);
MS_BOOL API_AUDIO_CUSTOMER_Dolby_DAP_SetParam(API_AUDIO_CUSTOMER_DOLBY_DAP_PARAM dap_params);
MS_BOOL API_AUDIO_CUSTOMER_SonicEmotion_ABS3D_Enable(MS_BOOL bOnOff);
MS_BOOL API_AUDIO_CUSTOMER_SonicEmotion_ABS3D_ProcessUnit_Enable(MS_BOOL bOnOff);
MS_BOOL API_AUDIO_CUSTOMER_SonicEmotion_ABS3D_SetParam(API_AUDIO_CUSTOMER_SONICEMOTION_ABS3D_PARAM abs3d_params);
MS_BOOL API_AUDIO_CUSTOMER_DTS_VirtualX_Enable(MS_BOOL bOnOff);
MS_BOOL API_AUDIO_CUSTOMER_DTS_VirtualX_ProcessUnit_Enable(MS_BOOL bOnOff);
MS_BOOL API_AUDIO_CUSTOMER_DTS_VirtualX_SetParam(API_AUDIO_CUSTOMER_DTS_VIRTUALX_PARAM dtsVx_params);
MS_BOOL API_AUDIO_CUSTOMER_DTS_VirtualX_SetAppCtrls(API_AUDIO_CUSTOMER_DTS_VIRTUALX_APPCTRLS dtsVx_AppCtrls);

/* Debug */
void API_AUDIO_CUSTOMER_DebugMenu(void * pVoid, MS_BOOL scanf_bSupport);
void API_AUDIO_CUSTOMER_DebugMenu_Non_Scanf_ParseCommand(char *paramsStr);


//-----------------------------------------------------------------------------------
// [PURPOSE] for SET
//-----------------------------------------------------------------------------------
typedef struct
{
    MS_U32 Dbg_Param;
    MS_U32 Dbg_Param2;
    MS_U32 Dbg_Param3;
    MS_U32 Dbg_Param4;
    MS_U32 Dbg_Param5;
    MS_U32 Dbg_Param6;
    MS_U32 Dbg_Param7;
    MS_U32 Dbg_Param8;
    MS_U32 Dbg_Param9;
    MS_U32 Dbg_Param10;
} API_AUDIO_CUSTOMER_DEBUG_PARAM;

//-----------------------------------------------------------------------------------
// [PURPOSE] for GET
//-----------------------------------------------------------------------------------
typedef struct
{
    MS_U32 Dbg_Info;
    MS_U32 Dbg_Info2;
    MS_U32 Dbg_Info3;
    MS_U32 Dbg_Info4;
    MS_U32 Dbg_Info5;
    MS_U32 Dbg_Info6;
    MS_U32 Dbg_Info7;
    MS_U32 Dbg_Info8;
    MS_U32 Dbg_Info9;
    MS_U32 Dbg_Info10;
} API_AUDIO_CUSTOMER_DEBUG_INFO;

//Header-free api structure
typedef struct
{
    const char *name;
    void *pdata;
} audio_tunnel;

#ifdef __cplusplus
}
#endif

#endif // _API_AUDIO_CUSTOMER_H_
