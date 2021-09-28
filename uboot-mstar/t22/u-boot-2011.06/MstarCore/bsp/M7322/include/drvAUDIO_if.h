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

#ifndef _DRV_AUDIO_IF_H_
#define _DRV_AUDIO_IF_H_

#include "MsTypes.h"
#include "UFO.h"

#ifdef __cplusplus
extern "C"
{
#endif

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#if defined(UFO_PUBLIC_HEADER_212)
#define code
#endif

#if defined(MSOS_TYPE_ECOS)
#define ATTRIBUTE_PACKED
#else
#define ATTRIBUTE_PACKED __attribute__((packed))
#endif

#define AUDIO_STRCMD_LENGTH    64
#define AUDIO_STRNAME_LENGTH    64
#define AUDIO_COMMON_PCM_IO_NAME_LENGTH    32

#define AUDIO_MAJOR_VERSION_MASK    0xFFFF0000
#define AUDIO_MINOR_VERSION_MASK    0x0000FFFF

#define A_EPERM    1 /* Operation not permitted */
#define A_ENOMEM    12 /* Out of memory */
#define A_EFAULT    14 /* Bad address */
#define A_EBUSY    16 /* Device or resource busy */
#define A_ENODEV    19 /* No such device */
#define A_EINVAL    22 /* Invalid argument */

//CathyRemove #define drvMAD_STOP                     0x0

#define AUD_CAP_VERSION                 1

#define AUDIO_BIFROST_TEST_SUPPORT_VERSION  2   // 2:  Some first basic module test cases supported. 4: SonicEmotion ABS3D supported.

#define En_DVB_advsndType_Compare(A, B) (strcmp(A, B) == 0)
#define ADVSND_TYPE_Compare(A, B) (strcmp(A, B) == 0)
#define ADVFUNC_Compare(A, B) (strcmp(A, B) == 0)
#define ADVSND_PARAM_Compare(A, B) (strcmp(A, B) == 0)

#define HDMI_Tx_HD                      1
#define HDMI_Tx_DTS_HD                  1

#define KDRIVER_AUDIO_VERSION 0x000D

//================================================================
//  HDMI PC Define
//================================================================
#define  AU_HDMI_AC3                    0x01
#define  AU_HDMI_AC3P                   0x15
#define  AU_HDMI_DTS_TYPE1              0x0B
#define  AU_HDMI_DTS_TYPE2              0x0C
#define  AU_HDMI_DTS_TYPE3              0x0D
#define  AU_HDMI_DTS_TYPE4              0x11
#define  AU_HDMI_AAC                    0x07
#define  AU_HDMI_MAT                    0x16

//================================================================
//  Common PCM I/O Define
//================================================================
#define AUDIO_HW_DMA_READER1    "HW DMA Reader1"
#define AUDIO_HW_DMA_READER2    "HW DMA Reader2"
#define AUDIO_SW_DMA_READER1    "SW DMA Reader1"
#define AUDIO_R2_DMA_READER1    "R2 DMA Reader1"
#define AUDIO_R2_DMA_READER2    "R2 DMA Reader2"
#define AUDIO_PCM_CAPTURE1      "PCM Capture1"
#define AUDIO_PCM_CAPTURE2      "PCM Capture2"
#define AUDIO_PCM_CAPTURE3      "PCM Capture3"
#define AUDIO_HW_DMA_WRITER1    "HW DMA Writer1"
#define AUDIO_DMIC_CAPTURE      "DMIC Capture"

#define AUDIO_SW_MIXER1    "SW MIXER1"
#define AUDIO_SW_MIXER2    "SW MIXER2"
#define AUDIO_SW_MIXER3    "SW MIXER3"
#define AUDIO_SW_MIXER4    "SW MIXER4"
#define AUDIO_SW_MIXER5    "SW MIXER5"
#define AUDIO_SW_MIXER6    "SW MIXER6"
#define AUDIO_SW_MIXER7    "SW MIXER7"
#define AUDIO_SW_MIXER8    "SW MIXER8"

#define AUDIO_PCM_SWMIXER_GROUP_NONE    0x00000000
#define AUDIO_PCM_SWMIXER_GROUP_ALL    0xFFFFFFFF
#define AUDIO_PCM_SWMIXER_GROUP_1    0x00000001
#define AUDIO_PCM_SWMIXER_GROUP_2    0x00000002
#define AUDIO_PCM_SWMIXER_GROUP_3    0x00000004
#define AUDIO_PCM_SWMIXER_GROUP_4    0x00000008
#define AUDIO_PCM_SWMIXER_GROUP_5    0x00000010
#define AUDIO_PCM_SWMIXER_GROUP_6    0x00000020
#define AUDIO_PCM_SWMIXER_GROUP_7    0x00000040
#define AUDIO_PCM_SWMIXER_GROUP_8    0x00000080

//================================================================
//  Output Device Selection Define
//================================================================
#define AUDIO_OUTPUT_DEVICE_NONE    0x00000000
#define AUDIO_OUTPUT_DEVICE_SPEAKER    0x00000001
#define AUDIO_OUTPUT_DEVICE_SPDIF    0x00000002
#define AUDIO_OUTPUT_DEVICE_HDMI    0x00000004
#define AUDIO_OUTPUT_DEVICE_ALL    0xFFFFFFFF

//================================================================
//  Define for CodecType in AUDIO_CODEC_CAPABILITY_t
//================================================================
#define AUDIO_CODEC_MPEG    0x00000001
#define AUDIO_CODEC_AC3    0x00000002
#define AUDIO_CODEC_AC3P    0x00000003
#define AUDIO_CODEC_AAC    0x00000004
#define AUDIO_CODEC_DTS    0x00000005
#define AUDIO_CODEC_DTS_LBR    0x00000006
#define AUDIO_CODEC_DTS_HD    0x00000007
#define AUDIO_CODEC_WMA    0x00000008
#define AUDIO_CODEC_DRA    0x00000009
#define AUDIO_CODEC_COOK    0x0000000A
#define AUDIO_CODEC_XPCM    0x0000000B
#define AUDIO_CODEC_VORBIS    0x0000000C
#define AUDIO_CODEC_AC4    0x0000000D
#define AUDIO_CODEC_MPEGH    0x0000000E
#define AUDIO_CODEC_OPUS    0x0000000F
//================================================================
//  Structure
//================================================================

typedef struct
{
    MS_U32 u32Addr;
    MS_U32 u32Size;
    MS_U64 u64Idx;
}AU_DVB_ENC_FRAME_INFO, *PAU_DVB_ENC_FRAME_INFO;

typedef struct
{
    MS_U32 u32Addr;
    MS_U32 u32Size;
    MS_U32 u32Idx;
}AUDIO_UPLOAD_INFO, *PAUDIO_UPLOAD_INFO;

typedef enum
{
    AUDIO_COEF_PREVER_PEQ, //for compatible with PEQ of previous version
    AUDIO_COEF_PEQ,
    AUDIO_COEF_HPF,
    AUDIO_COEF_BASS,
    AUDIO_COEF_TREBLE,
}AUDIO_COEF_TYPE;

typedef enum
{
    AUDIO_SINGLE_PRECISION,
    AUDIO_DOUBLE_PRECISION,
}AUDIO_PRECISION_TYPE;

typedef struct
{
    MS_U8   band;                       ///< band
    MS_U8   sfs;                        ///< fs
    MS_U8   scale;                      ///< scale
    MS_U32  a0;                         ///< a0
    MS_U32  a1;                         ///< a1
    MS_U32  a2;                         ///< a2
    MS_U32  b1;                         ///< b1
    MS_U32  b2;                         ///< b2
    AUDIO_COEF_TYPE   type;             /// PEQ, HPF or tone
    MS_BOOL enable;                     /// PEQ band enable
    AUDIO_PRECISION_TYPE precision;     /// PEQ filter precision
} AUDIO_PEQ_COEF, *PAUDIO_PEQ_COEF;

typedef enum
{
    AUDIO_SOUNDEFFECT_SPEAKER,
    AUDIO_SOUNDEFFECT_HEADPHONE,
}AUDIO_SOUNDEFFECT_TYPE;

typedef struct ATTRIBUTE_PACKED
{
    MS_U32 u32AudioCapsVersion;
    MS_U32 u32AudioCapsStructSize;
    MS_U8 u8MaxDecoderNum;
    MS_U8 u8MaxDmaReaderNum;
    MS_BOOL bIsADSupported;
    MS_BOOL bIsPusiDetectSupported;
    MS_BOOL bIsMpegSupported;
    MS_BOOL bIsAc3Supported;
    MS_BOOL bIsAc3pSupported;
    MS_BOOL bIsAacSupported;
    MS_BOOL bIsMp3Supported;
    MS_BOOL bIsWmaSupported;
    MS_BOOL bIsRa8LbrSupported;
    MS_BOOL bIsXpcmSupported;
    MS_BOOL bIsDtsSupported;
    MS_BOOL bIsWmaProSupported;
    MS_BOOL bIsFlacSupported;
    MS_BOOL bIsVorbisSupported;
    MS_BOOL bIsDtsLbrSupported;
    MS_BOOL bIsAmrNbSupported;
    MS_BOOL bIsAmrWbSupported;
    MS_BOOL bIsDraSupported;

    /*
     * Note!!!
     *
     * 1. No matter what kind of modification in this structure, "AUD_CAP_VERSION" must be updated as well!!!
     *
     * 2. The element in this structure can only be added, never removed!!!
     *
     */
} AUDIO_CAPABILITIES;

#if defined(UFO_PUBLIC_HEADER_300)
typedef struct
#else
typedef struct ATTRIBUTE_PACKED
#endif
{
    MS_U32 u32StructVersion; /* version number of this data structure */
    MS_U32 u32StructSize; /* size of this data structure */
    MS_U8 u8ConnectFlag; /* it is TRUE when opened */
    MS_U8 u8StartFlag; /* it is TRUE when started and opened */
    MS_U8 u8Name[AUDIO_COMMON_PCM_IO_NAME_LENGTH]; /* The PCM name requested */
    MS_U8 u8NonBlockingFlag; /* it is TRUE when request for nonblocking mode */
    MS_U8 u8MultiChFlag; /* it is TRUE when request for a multi-channel PCM IO */
    MS_U8 u8MixingFlag; /* it is TRUE when request for a mixing PCM IO */
    MS_U32 u32MixingGroup; /* PCM mixing group, i.e. AUDIO_PCMMIXER_GROUP_ALL */
#if defined(UFO_PUBLIC_HEADER_212) || defined(UFO_PUBLIC_HEADER_500_3)
    MS_S8 *pBuffer; /* PCM buffer address */
    MS_S8 *pReadPtr; /* PCM read pointer */
    MS_S8 *pWritePtr; /* PCM write pointer */
#endif
    MS_U32 u32BufferDuration; /* PCM buffer size in mini-second unit, and up to a maximun size */
    MS_U32 u32Channel; /* PCM channel number */
    MS_U32 u32SampleRate; /* PCM sample rate */
    MS_U32 u32BitWidth; /* PCM sample's bit width of each channel */
    MS_U32 u32BigEndian; /* PCM endian, TRUE means Big Endian, FALSE means Little Endian */
    MS_U32 u32Timestamp; /* PCM timestamp */
    MS_U32 u32Weighting; /* PCM weighting for mixing case, differs from volume it's an extra attenuation  for PCM level, the range is 0~100, and 100 means no any attenuation */
    MS_U32 u32Volume; /* PCM volume, the range is 0 ~ 1016, and 96 means 0dB */
    MS_U32 u32BufferLevel; /* PCM bufffer level in byte unit */
    MS_U8 u8CaptureFlag; /* it is TRUE when request for a PCM Capture IO  */

    /*
     * Note!!!
     *
     * 1. No matter what kind of modification in this structure, "AUDIO_PCM_INFO_VERSION" must be updated as well!!!
     *
     * 2. The element in this structure can only be added, never removed!!!
     *
     */
} AUDIO_PCM_INFO_t;
#if defined(UFO_PUBLIC_HEADER_212) || defined(UFO_PUBLIC_HEADER_500_3)
#define AUDIO_PCM_INFO_VERSION    0x00010001
#else
#define AUDIO_PCM_INFO_VERSION    0x00020001
#endif

//================================================================
//  Enum
//================================================================

//================================================================
//  AuCommon
//================================================================
typedef enum
{
    AUDIO_PATH_GROUP_1,
    AUDIO_PATH_GROUP_2,
    AUDIO_PATH_GROUP_3,
}AUDIO_PATH_GROUP_TYPE;


///<Audio source info type
typedef enum
{
    E_AUDIO_INFO_DTV_IN,
    E_AUDIO_INFO_ATV_IN,
    E_AUDIO_INFO_HDMI_IN,
    E_AUDIO_INFO_ADC_IN,
    E_AUDIO_INFO_MM_IN,
    E_AUDIO_INFO_SPDIF_IN,
    E_AUDIO_INFO_KTV_IN,
    E_AUDIO_INFO_GAME_IN,
    E_AUDIO_INFO_KTV_STB_IN         ///< Iuput source type for STB KTV mode
}AUDIO_SOURCE_INFO_TYPE;

///<Enum for Audio data capture source selection
typedef enum
{
    E_CAPTURE_CH1,                   ///< Data from DIG1 input port
    E_CAPTURE_CH2,                   ///< Data from DIG2 input port
    E_CAPTURE_CH3,                   ///< Data from DIG3 input port
    E_CAPTURE_CH4,                   ///< Data from DIG4 input port
    E_CAPTURE_CH5,                   ///< Data from DIG5 input port
    E_CAPTURE_CH6,                   ///< Data from DIG6 input port
    E_CAPTURE_CH7,                   ///< Data from DIG7 input port
    E_CAPTURE_CH8,                   ///< Data from DIG8 input port
    E_CAPTURE_PCM,                   ///< Pure PCM out =Data CH5
    E_CAPTURE_PCM_DELAY,             ///< PCM + Delay out
    E_CAPTURE_PCM_SE,                ///< PCM + Delay +SE out
    E_CAPTURE_ADC,                   ///< Data from ADC1 input port
    E_CAPTURE_AUOUT0,                ///< Data for DAC0 output port
    E_CAPTURE_AUOUT1,                ///< Data for DAC1 output port
    E_CAPTURE_AUOUT2,                ///< Data for DAC2 output port
    E_CAPTURE_AUOUT3,                ///< Data for DAC3 output port
    E_CAPTURE_I2S_OUT,               ///< Data for I2S output port
    E_CAPTURE_SPDIF_OUT,             ///< Data for SPDIF output port
    E_CAPTURE_MIXER,                 ///< Data for audio data Mixer
    E_CAPTURE_ADC2,                  ///< Data from ADC2 input port
    E_CAPTURE_ADC1_AUIN0_INPUT,      /// ADC1-Line-in[0]
    E_CAPTURE_ADC1_AUIN1_INPUT,      /// ADC1-Line-in[1]
    E_CAPTURE_ADC1_AUIN2_INPUT,      /// ADC1-Line-in[2]
    E_CAPTURE_ADC1_AUIN3_INPUT,      /// ADC1-Line-in[3]
    E_CAPTURE_ADC1_AUIN4_INPUT,      /// ADC1-Line-in[4]
    E_CAPTURE_ADC1_AUIN5_INPUT,      /// ADC1-Line-in[5]
    E_CAPTURE_ADC1_AUMIC_INPUT,      /// ADC1-Micphone-in
    E_CAPTURE_ADC2_AUIN0_INPUT,      /// ADC2-Line-in[0]
    E_CAPTURE_ADC2_AUIN1_INPUT,      /// ADC2-Line-in[1]
    E_CAPTURE_ADC2_AUIN2_INPUT,      /// ADC2-Line-in[2]
    E_CAPTURE_ADC2_AUIN3_INPUT,      /// ADC2-Line-in[3]
    E_CAPTURE_ADC2_AUIN4_INPUT,      /// ADC2-Line-in[4]
    E_CAPTURE_ADC2_AUIN5_INPUT,      /// ADC2-Line-in[5]
    E_CAPTURE_ADC2_AUMIC_INPUT,      /// ADC2-Micphone-in
    E_CAPTURE_SPEAKER,               ///< Speaker out (Delay +SE +VolumeBalance)
    E_CAPTURE_PCM_DECIM,             ///< PCM + Delay + Decimation out
    E_CAPTURE_I2S_RX0,               ///< I2S RX0 (SD0)
    E_CAPTURE_I2S_RX1,               ///< I2S RX1 (SD1)
    E_CAPTURE_I2S_RX0_RX1,           ///< I2S RX0 + RX1 (SD0 + SD1) - Customized
    E_CAPTURE_MAX_NUM,               ///< MAX capture source number
    E_CAPTURE_NULL                   = 0xFF,  ///< No caupure source
}AUDIO_CAPTURE_SOURCE_TYPE;          // Audio capture type selection

///<Enum for Multi-Audio Device
typedef enum
{
    E_DEVICE0,    ///< 1st audio device
    E_DEVICE1,    ///< 2nd audio device
    E_DEVICE2,    ///< 3rd audio device
    E_DEVICE3,    ///< 4th audio device
    E_DEVICE4,    ///< 5th audio device
    E_DEVICE5,    ///< 6th audio device
}AUDIO_DEVICE_TYPE;   // Audio device type

///====== Audio Path Mode TYPE========
enum
{
    AUDIO_DSP1_INPUT,                   ///< 0: DSP Decoder1 Input
    AUDIO_DSP2_INPUT,                   ///< 1: DSP Decoder2 Input
    AUDIO_ADC_INPUT,                    ///< 2: ADC Input
    AUDIO_DSP3_INPUT                    = 0x06,     ///< 6: DSP Decoder3 Input
    AUDIO_DSP4_INPUT                    = 0x08,     ///< 8: DSP Decoder4 Input
    AUDIO_ADC2_INPUT                    = 0x09,     ///< 9: ADC2 Input
    AUDIO_DMA_INPUT                     = 0x0E,     ///< E: DMA Input
};

typedef enum
{
    AUDIO_PATH_0,                       ///< 0: PATH 0
    AUDIO_PATH_1,                       ///< 1: PATH 1
    AUDIO_PATH_2,                       ///< 2: PATH 2
    AUDIO_PATH_3,                       ///< 3: PATH 3
    AUDIO_PATH_4,
    AUDIO_PATH_5,
    AUDIO_PATH_6,
    AUDIO_PATH_7,                       ///< 7: New add in T3
    AUDIO_PATH_MAIN                     = 4,

    AUDIO_T3_PATH_AUOUT0                = 0x30,      ///< T3 volume path : AUOUT0
    AUDIO_T3_PATH_AUOUT1                = 0x31,      ///< T3 volume path : AUOUT1
    AUDIO_T3_PATH_AUOUT2                = 0x32,      ///< T3 volume path : AUOUT2
    AUDIO_T3_PATH_AUOUT3                = 0x33,      ///< T3 volume path : AUOUT3
    AUDIO_T3_PATH_I2S                   = 0x34,      ///< T3 volume path : I2S
    AUDIO_T3_PATH_SPDIF                 = 0x35,      ///< T3 volume path : SPDIF
    AUDIO_T3_PATH_I2S2                  = 0x36,      ///< T3 volume path : SPDIF
    AUDIO_T3_PATH_HDMI                  = 0x37,      ///< T3 volume path : HDMI
    AUDIO_T3_PATH_LR                    = 0x38,
    AUDIO_T3_PATH_PCM_CAPTURE1          = 0x39,      ///< T3 volume path : PCM Capture1
    AUDIO_T3_PATH_PCM_CAPTURE2          = 0x3A,      ///< T3 volume path : PCM Capture2
    AUDIO_T3_PATH_DMA_WRITER1           = 0x3B,      ///< T3 volume path : HW DMA Writer1
    AUDIO_T3_PATH_PCM_CAPTURE3          = 0x3C,      ///< T3 volume path : PCM Capture3
    AUDIO_T3_PATH_ARC                   = 0x3D,
    AUDIO_T3_PATH_MIXER_MAIN            = 0x40,      ///< Internal volume path for Mixer 1st port (CH5 in)
    AUDIO_T3_PATH_MIXER_SECONDARY       = 0x41,      ///< Internal volume path for Mixer 2nd port (CH6 in)
    AUDIO_T3_PATH_MIXER_DMA_IN          = 0x42,      ///< Internal volume path for Mixer 3rd port (CH8 in)
    AUDIO_T3_PATH_MIXER_MCH_IN          = 0x43,      ///< Internal volume path forMulti-channel ports (CH1~3 in)

    AUDIO_PATH_SW_DMA_0                 = 0x50,
    AUDIO_PATH_SW_DMA_1                 = 0x51,
    AUDIO_PATH_HW_DMA_0                 = 0x52,
    AUDIO_PATH_HW_DMA_1                 = 0x53,

    AUDIO_PATH_NULL                     = 0xFF
}AUDIO_PATH_TYPE;

typedef enum
{
    INTERNAL_MULTI_1,                   ///< 0: For Multi-channel only
    INTERNAL_MULTI_2,                   ///< 1  For Multi-channel only
    INTERNAL_MULTI_3,                   ///< 2: For Multi-channel only
    INTERNAL_MULTI_4,                   ///< 3: For Multi-channel only
    INTERNAL_PCM,                       ///< 4: Pure PCM out
    INTERNAL_PCM_DELAY,                 ///< 5: PCM + Delay out
    INTERNAL_PCM_SE,                    ///< 6: PCM + Delay +SE out
    INTERNAL_SCART,                     ///< 7: SCART data
    INTERNAL_MIXER,                     ///< 8: SCART data
    INTERNAL_CH6,                       ///< 9: CH6 data
    INTERNAL_TEST,                      ///< A: Test data
    INTERNAL_LRDIV2,                    ///< B: I2S (L+R)/2 data
    INTERNAL_TONE,                      ///< C: 1KHz Tone
    INTERNAL_CH8,                       ///< D: Data from CH8
    INTERNAL_ATVENC,                    ///< E: Data from ATV Encoder.
    INTERNAL_MAX_NUM,                   ///< MAX Internal path number
    INTERNAL_PATH_NULL                  = 0xFF,
}AUDIO_INTERNAL_PATH_TYPE;              // for T3 only

//====== Audio input Mode TYPE========
typedef enum
{
    AUDIO_SPDIF_INPUT                   = 3,        ///< 3: SPDIF INPUT
    AUDIO_I2S_INPUT                     = 4,        ///< 4: I2S INPUT
    AUDIO_HDMI_INPUT                    = 5,        ///< 5: HDMI INPUT
    AUDIO_SRC_INPUT                     = 7,

    AUDIO_DSP1_DVB_INPUT                = 0x00,
    AUDIO_DSP1_DVB1_INPUT               = 0x10,
    AUDIO_DSP1_SPDIF_INPUT              = 0x20,
    AUDIO_DSP1_SPDIFx1_INPUT            = 0x30,
    AUDIO_DSP1_HDMI_INPUT               = 0x40,
    AUDIO_DSP1_HDMIx1_INPUT             = 0x50,
    AUDIO_DSP1_CardRD_INPUT             = 0x60,
    AUDIO_DSP1_SIF_INPUT                = 0x70,
    AUDIO_DSP1_MM_INPUT                 = 0x80,

    AUDIO_DSP2_DVB_INPUT                = 0x01,
    AUDIO_DSP2_DVB1_INPUT               = 0x11,
    AUDIO_DSP2_SPDIF_INPUT              = 0x21,
    AUDIO_DSP2_SPDIFx1_INPUT            = 0x31,
    AUDIO_DSP2_HDMI_INPUT               = 0x41,
    AUDIO_DSP2_HDMIx1_INPUT             = 0x51,
    AUDIO_DSP2_CardRD_INPUT             = 0x61,
    AUDIO_DSP2_SIF_INPUT                = 0x71,

    AUDIO_DSP3_DVB_INPUT                = 0x06,
    AUDIO_DSP3_DVB1_INPUT               = 0x16,
    AUDIO_DSP3_SPDIF_INPUT              = 0x26,
    AUDIO_DSP3_SPDIFx1_INPUT            = 0x36,
    AUDIO_DSP3_HDMI_INPUT               = 0x46,
    AUDIO_DSP3_HDMIx1_INPUT             = 0x56,
    AUDIO_DSP3_CardRD_INPUT             = 0x66,
    AUDIO_DSP3_SIF_INPUT                = 0x76,

    AUDIO_DSP4_DVB_INPUT                = 0x08,
    AUDIO_DSP4_DMARD_INPUT              = 0x58,     /// New add in T3
    AUDIO_DSP4_SIF_INPUT                = 0x78,     /// For T3 SIF input

    AUDIO_AUIN0_INPUT                   = 0x02,     /// Line-in[0]
    AUDIO_AUIN1_INPUT                   = 0x12,     /// Line-in[1]
    AUDIO_AUIN2_INPUT                   = 0x22,     /// Line-in[2]
    AUDIO_AUIN3_INPUT                   = 0x32,     /// Line-in[3]
    AUDIO_AUIN4_INPUT                   = 0xA2,     /// Line-in[4]
    AUDIO_AUIN5_INPUT                   = 0xB2,     /// Line-in[5]
    AUDIO_AUMIC_INPUT                   = 0x72,     /// Micphone-in

    AUDIO_ADC2_AUIN0_INPUT              = 0x09,     /// Line-in[0]
    AUDIO_ADC2_AUIN1_INPUT              = 0x19,     /// Line-in[1]
    AUDIO_ADC2_AUIN2_INPUT              = 0x29,     /// Line-in[2]
    AUDIO_ADC2_AUIN3_INPUT              = 0x39,     /// Line-in[3]
    AUDIO_ADC2_AUIN4_INPUT              = 0xA9,     /// Line-in[4]
    AUDIO_ADC2_AUIN5_INPUT              = 0xB9,     /// Line-in[5]
    AUDIO_ADC2_AUMIC_INPUT              = 0x79,     /// Micphone-in

    AUDIO_DMA_READER_INPUT              = 0x0E,       ///<0x0E : DMA Reader INPUT

    AUDIO_NULL_INPUT                    = 0xFF,
} AUDIO_INPUT_TYPE;

typedef enum
{
    AUDIO_DEBUG_LEVEL_FATAL,
    AUDIO_DEBUG_LEVEL_ERROR,
    AUDIO_DEBUG_LEVEL_WARN,
    AUDIO_DEBUG_LEVEL_INFO,
    AUDIO_DEBUG_LEVEL_DEBUG,
    AUDIO_DEBUG_LEVEL_VERBOSE,
}AUDIO_DEBUG_LEVEL;

//================================================================
//  HDMI ENUM
//================================================================

//====== HDMI Rx Mode TYPE========
typedef enum
{
    HDMI_RX_PCM                         = 0x0,      ///< HDMI RX PCM Mode
    HDMI_RX_DD                          = 0x1,      ///< HDMI RX Dolby Mode
    HDMI_RX_DTS                         = 0x2,      ///< HDMI RX DTS Mode
    HDMI_RX_DDP                         = 0x3,      ///< HDMI RX DDP Mode
    HDMI_RX_DP                          = 0x4,      ///< HDMI RX DP Mode
    HDMI_RX_Other                       = 0x5,      ///< HDMI RX Other Mode
    HDMI_RX_MAT                         = 0x6,      ///< HDMI RX MAT Mode
    HDMI_RX_DTS_HD                      = 0x7,      ///< HDMI RX DTS HD Mode
} AUDIO_HDMI_RX_TYPE;

typedef enum
{
    CODEC_NONE                          = 0x0,
    CODEC_LPCM                          = 0x1,      ///< Support LPCM
    CODEC_DD                            = 0x2,      ///< Support DD
    CODEC_MPEG1                         = 0x3,      ///< Support MPEG1
    CODEC_MP3                           = 0x4,      ///< Support MP3
    CODEC_MPEG2                         = 0x5,      ///< Support MPEG2
    CODEC_AAC                           = 0x6,      ///< Support AAC
    CODEC_DTS                           = 0x7,      ///< Support DTS
    CODEC_ATRAC                         = 0x8,      ///< Support ATRAC
    CODEC_ONEBITAUDIO                   = 0x9,      ///< Support One-Bit Audio
    CODEC_DDP                           = 0xA,      ///< Support DDP
    CODEC_DTSHD                         = 0xB,      ///< Support DTSHD
    CODEC_TRUEHD                        = 0xC,      ///< Support MLP/TRUE-HD
    CODEC_DST                           = 0xD,      ///< Support DST
    CODEC_WMAPRO                        = 0xE,      ///< Support WMA-Pro
    CODEC_MAX                           = 0xF,
} AUDIO_FORMAT_CODE;

//====== HDMI Tx Mode TYPE========
typedef enum
{
    HDMI_OUT_PCM,                       ///< HDMI OUTPUT PCM
    HDMI_OUT_NONPCM,                    ///< HDMI OUTPUT non-PCM
    HDMI_OUT_8CH_NONPCM,                ///< HDMI OUTPUT non-PCM (8 Channel transmit, mainly in DTS-HD, Dolby True-HD and so on)
    HDMI_OUT_BYPASS,                    ///< HDMI OUTPUT non-PCM (By-Pass)
    HDMI_OUT_TRANSCODE,                 ///< HDMI OUTPUT TRANSCODE
} HDMI_TX_OUTPUT_TYPE;

//====== Audio output Mode TYPE========
typedef enum
{
    AUDIO_AUOUT0_OUTPUT,                ///< 0: DAC0 OUTPUT
    AUDIO_AUOUT1_OUTPUT,                ///< 1: T2=>AA0 OUTPUT    T3=>HP OUTPUT
    AUDIO_AUOUT2_OUTPUT,                ///< 2: T2=>AA1 OUTPUT    T3=>DAC2 OUTPUT
    AUDIO_AUOUT3_OUTPUT,                ///< 3: T2=>NULL          T3=>AA OUTPUT
    AUDIO_SPDIF_OUTPUT,                 ///< 4: SPDIF OUTPUT
    AUDIO_I2S_OUTPUT,                   ///< 5: I2S OUTPUT
    AUDIO_HP_OUTPUT,                    ///< 6: OUTPUT with headphone driver
    AUDIO_I2S2_OUTPUT,                  ///< 7: I2S2 OUTPUT
    AUDIO_HDMI_ARC_OUTPUT,              ///< 8: HDMI ARC OUTPUT(HDMI_Rx,TV)
    AUDIO_HDMI_OUTPUT,                  ///< 9: HDMI Tx OUTPUT(HDMI_Tx,STB)
    AUDIO_I2S3_OUTPUT,                  ///< A: I2S3 OUTPUT
    AUDIO_I2S4_OUTPUT,                  ///< B: I2S4 OUTPUT
    AUDIO_NULL_OUTPUT                   = 0xFF,
} AUDIO_OUTPUT_TYPE;

typedef enum
{
    DIGITAL_SPDIF_OUTPUT,                 ///< 0: SPDIF
    DIGITAL_HDMI_ARC_OUTPUT,          ///< 1: HDMI ARC
    DIGITAL_HDMI_OUTPUT,                  ///< 2: HDMI
    DIGITAL_I2S_OUTPUT,                     ///< 3: I2S
    DIGITAL_MAX_OUTPUT,
} DIGITAL_OUTPUT_TYPE;

typedef enum
{
    DIGITAL_OUTPUT_CODEC_NOT_SPECIFIED,     ///< 0: not specified
    DIGITAL_OUTPUT_CODEC_AC3,               ///< 1: specifies to output AC3
    DIGITAL_OUTPUT_CODEC_AC3P,              ///< 2: specifies to output AC3P
    DIGITAL_OUTPUT_CODEC_AAC,               ///< 3: specifies to output AAC
    DIGITAL_OUTPUT_CODEC_DTS,               ///< 4: specifies to output DTS
    DIGITAL_OUTPUT_CODEC_ATMOS,             ///< 5: specifies to output ATMOS
    DIGITAL_OUTPUT_CODEC_MAX,
} DIGITAL_OUTPUT_SPECIFIED_CODEC;

typedef enum
{
    DECODER_PCM_OUTPUT_TYPE1_DMX, /* Downmix, target channel = 2 */
    DECODER_PCM_OUTPUT_TYPE2_6CH, /* Multi-Channel(5.1), target channel = 6 */
    DECODER_PCM_OUTPUT_TYPE3_8CH, /* Multi-Channel(7.1), target channel = 8 */
    DECODER_PCM_OUTPUT_TYPE4_DMX_8CH, /* Downmix, Multi-Channel(7.1), target channel = 10 */
    DECODER_PCM_OUTPUT_TYPE5_DMX_8CH_PTR, /* Downmix, Multi-Channel(7.1), nonPcmPtr, target channel = 12 */
 } DECODER_PCM_OUTPUT_TYPE;

//================================================================
//  I2S ENUM
//================================================================
//====== I2S OUTPUT Mode TYPE========
typedef enum
{
    AUDIO_I2S_MCLK,                     ///< I2S MCLK
    AUDIO_I2S_WORD_WIDTH,               ///< I2S Word-width
    AUDIO_I2S_SOURCE_CH,                ///< I2S Source
    AUDIO_I2S_FORMAT,                    ///< I2S format
    AUDIO_I2S_RXMODE,                     ///< I2S RxMode
    AUDIO_I2S_TXMODE,                     ///< I2S TxMode
} AUDIO_I2S_MODE_TYPE;


typedef enum
{
    I2S_MCLK_64FS                       = 0x04,
    I2S_MCLK_128FS                      = 0x05,
    I2S_MCLK_256FS                      = 0x06,     ///< 256 Fs ==>default setting
    I2S_MCLK_384FS                      = 0x07,

    I2S_WORD_WIDTH_16BIT                = 0x4,
    I2S_WORD_WIDTH_24BIT                = 0x5,
    I2S_WORD_WIDTH_32BIT                = 0x6,      ///< 32-bit ==> default setting

    I2S_FORMAT_STANDARD                 = 0,
    I2S_FORMAT_LEFT_JUSTIFIED           = 1,

    I2S_MASTER_MODE                       = 0,      ///< MASTER_MODE ==> default setting
    I2S_SLAVE_MODE                       = 1,
} AUDIO_I2S_MODE_VALUE;

//================================================================
//  SPDIF ENUM
//================================================================
//====== SPDIF OUTPUT PATH========
typedef enum
{
    MPEG_ENCODE,
    DTS_ENCODE_2CH,
    DTS_ENCODE_MULTI,
    MP3_ENCODE,
    DD_DDENCODE,
    DD_DDCO,
}AUDIO_ENCODE_TYPE;

//====== DIGITAL OUTPUT Mode TYPE========
typedef enum
{
    DIGITAL_OUT_PCM,
    DIGITAL_OUT_DOLBYDIGITAL,
    DIGITAL_OUT_DTS,
    DIGITAL_OUT_AAC_LC,
    DIGITAL_OUT_NONE,
} AUDIO_DIGITAL_OUTPUT_TYPE;

//====== SPDIF OUTPUT Mode TYPE========
typedef enum
{
    SPDIF_OUT_PCM                       = 0x0,      ///< S/PDIF always output PCM
    SPDIF_OUT_AUTO                      = 0x2,      ///< S/PDIF OUTPUT PCM or non-PCM automatic
    SPDIF_OUT_NONPCM                    = 0x2,      ///< S/PDIF OUTPUT non-PCM
    SPDIF_OUT_BYPASS,                   ///< S/PDIF OUTPUT by-pass from HDMI
    SPDIF_OUT_TRANSCODE,                ///< S/PDIF OUTPUT always transcode
    SPDIF_OUT_NONE,                     ///< S/PDIF OUTPUT nothing
} AUDIO_SPDIF_OUTPUT_TYPE;

//====== SPDIF OUTPUT CS TYPE =========
typedef enum
{
    SPDIF_CS_CategoryCode               = 0x0,      ///<
    SPDIF_CS_CopyRight                  = 0x1,      ///<
    SPDIF_CS_SourceNumber               = 0x2,      ///<
    SPDIF_CS_ChannelNumber              = 0x3,      ///<
    SPDIF_CS_SmpFreq                    = 0x4,      ///<
    SPDIF_CS_ClockAcc                   = 0x5,      ///<
    SPDIF_ChannelAB                     = 0x6,      ///<
    SPDIF_CS_CopyProtect                = 0x7,      ///<
} AUDIO_SPDIF_CS_TYPE;

//====== SPDIF OUTPUT CS Status =======
typedef enum
{
    SPDIF_CS_Category_BroadCast         = 0x00,     ///<
    SPDIF_CS_Category_General           = 0x01,     ///<
    SPDIF_CS_Category_BroadCast_JP      = 0x02,     ///<
    SPDIF_CS_Category_BroadCast_EU      = 0x03,     ///<
    SPDIF_CS_Category_BroadCast_USA     = 0x04,     ///<
    SPDIF_CS_Category_Rev4              = 0x05,     ///<
    SPDIF_CS_Category_Rev5              = 0x06,     ///<
    SPDIF_CS_Category_Rev6              = 0x07,     ///<
    SPDIF_CS_CopyRight_CP               = 0x08,     ///<
    SPDIF_CS_CopyRight_NonCP            = 0x09,     ///<SPDIF_CS_ChannelNumber_Right
    SPDIF_CS_SourceNumber_2             = 0x0A,     ///<
    SPDIF_CS_SourceNumber_5             = 0x0B,     ///<
    SPDIF_CS_SourceNumber_6             = 0x0C,     ///<
    SPDIF_CS_SourceNumber_Rev1          = 0x0D,     ///<
    SPDIF_CS_SourceNumber_Rev2          = 0x0E,     ///<
    SPDIF_CS_SourceNumber_Rev3          = 0x0F,     ///<
    SPDIF_CS_ChannelNumber_A_Left       = 0x10,     ///<
    SPDIF_CS_ChannelNumber_B_Right      = 0x11,     ///<
    SPDIF_CS_ChannelNumber_C            = 0x12,     ///<
    SPDIF_CS_ChannelNumber_D            = 0x13,     ///<
    SPDIF_CS_ChannelNumber_E            = 0x14,     ///<
    SPDIF_CS_ChannelNumber_F            = 0x15,     ///<
    SPDIF_CS_ChannelNumber_G            = 0x16,     ///<
    SPDIF_CS_ChannelNumber_H            = 0x17,     ///<
    SPDIF_CS_ChannelNumber_I            = 0x18,     ///<
    SPDIF_CS_ChannelNumber_J            = 0x19,     ///<
    SPDIF_CS_ChannelNumber_K            = 0x1A,     ///<
    SPDIF_CS_SmpFreq_192                = 0x1B,     ///<
    SPDIF_CS_SmpFreq_176                = 0x1C,     ///<
    SPDIF_CS_SmpFreq_128                = 0x1D,     ///<
    SPDIF_CS_SmpFreq_96                 = 0x1E,     ///<
    SPDIF_CS_SmpFreq_88                 = 0x1F,     ///<
    SPDIF_CS_SmpFreq_64                 = 0x20,     ///<
    SPDIF_CS_SmpFreq_48                 = 0x21,     ///<
    SPDIF_CS_SmpFreq_44                 = 0x22,     ///<
    SPDIF_CS_SmpFreq_32                 = 0x23,     ///<
    SPDIF_CS_SmpFreq_24                 = 0x24,     ///<
    SPDIF_CS_SmpFreq_22                 = 0x25,     ///<
    SPDIF_CS_SmpFreq_16                 = 0x26,     ///<
    SPDIF_CS_SmpFreq_Rev1               = 0x27,     ///<
    SPDIF_CS_SmpFreq_Rev2               = 0x28,     ///<
    SPDIF_CS_SmpFreq_Rev3               = 0x29,     ///<
    SPDIF_CS_SmpFreq_Rev4               = 0x2A,     ///<
    SPDIF_CS_SmpFreq_Rev5               = 0x2B,     ///<
    SPDIF_CS_ClockAcc_Lev1              = 0x2C,     ///<
    SPDIF_CS_ClockAcc_Lev2              = 0x2D,     ///<
    SPDIF_CS_ClockAcc_Lev3              = 0x2E,     ///<
    SPDIF_CS_ClockAcc_NotMatch          = 0x2F,     ///<
    SPDIF_ChannelAB_Invalid_ON          = 0x30,     ///<
    SPDIF_ChannelAB_Invalid_OFF         = 0x31,     ///<
    SPDIF_CS_CopyProtect_CopyFree       = 0x32,     ///<
    SPDIF_CS_CopyProtect_CopyOnce       = 0x33,     ///<
    SPDIF_CS_CopyProtect_CopyNever      = 0x34,     ///<
} AUDIO_SPDIF_CS_TYPE_STATUS;

//====== SPDIF OUTPUT CS Mode TYPE========
typedef enum
{
    SPDIF_CHANNEL_STATUS_FS,            ///< Set sampling rate in CS3
    SPDIF_CHANNEL_STATUS_CATEGORY,      ///< Set Category code in CS1
    SPDIF_CHANNEL_STATUS_PCM_FORMAT,    ///< Set PCM/NonPCM mode in CS0
    SPDIF_CHANNEL_STATUS_SOURCE_NUMBER,    ///< Set source number in CS2
    SPDIF_CHANNEL_STATUS_CHANNEL_NUMBER,    ///< Set Channel number in CS2
} SPDIF_CS_MODE_TYPE;

typedef enum
{
    SPDIF_CS_FS_32K                     = 0xC0,
    SPDIF_CS_FS_44K                     = 0x00,
    SPDIF_CS_FS_48K                     = 0x40,     ///< ==>default setting
    SPDIF_CS_CATEGORY_NONE              = 0x00,
    SPDIF_CS_CATEGORY_DVB               = 0x30,     ///Broadcast reception of digital audio with europe
    SPDIF_CS_CATEGORY_ATSC              = 0x26,     ///Broadcast reception of digital audio with america
    SPDIF_CS_CATEGORY_ISDB              = 0x20,     ///Broadcast reception of digital audio with japen
    SPDIF_CS_FORMAT_PCM                 = 0x00,
    SPDIF_CS_FORMAT_NONPCM              = 0x40,
}SPDIF_CS_MODE_VALUE;

//================================================================
//  SIF ENUM
//================================================================
//======SIF Standard========
typedef enum
{
    AU_SYS_NOT_READY,                   // 0
    AU_SYS_M_BTSC,                      // 1
    AU_SYS_M_EIAJ,                      // 2
    AU_SYS_M_A2,                        // 3

    AU_SYS_BG_A2,                       // 4
    AU_SYS_DK1_A2,                      // 5
    AU_SYS_DK2_A2,                      // 6
    AU_SYS_DK3_A2,                      // 7

    AU_SYS_BG_NICAM,                    // 8
    AU_SYS_DK_NICAM,                    // 9
    AU_SYS_I_NICAM,                     // 10
    AU_SYS_L_NICAM,                     // 11
    AU_SYS_FM_RADIO,                    // 12
    AU_SYS_HI_DEV                       = 0x10,
    AU_SYS_BUSY                         = 0x80,
    STD_SEL_AUTO                        = 0xE0
}AUDIO_SIF_SET_STANDARD;

//======SIF System Type========
typedef enum
{
    A2_M_STANDARD                       = 0x00,
    A2_BG_STANDARD,
    A2_DK_STANDARD,
    A2_I_STANDARD,
    AM_STANDARD,
    NICAM_BG_STANDARD,
    NICAM_I_STANDARD,
    HIDEV_M_STANDARD,
    HIDEV_BG_STANDARD,
    HIDEV_DK_STANDARD,
    HIDEV_I_STANDARD,
    BTSC_STANDARD,
    FM_RADIO_STANDARD,
    WRITE_THRESHOLD                     = 0x10
}AUDIO_SIF_SYSTEM_TYPE;


//======SIF Threshold Type========
typedef enum
{
    CARRIER1_ON_AMP                     = 0x00,
    CARRIER1_OFF_AMP,
    CARRIER1_ON_NSR,
    CARRIER1_OFF_NSR,
    CARRIER2_ON_AMP,
    CARRIER2_OFF_AMP,
    CARRIER2_ON_NSR,
    CARRIER2_OFF_NSR,
    A2_PILOT_ON_AMP,
    A2_PILOT_OFF_AMP,

    NICAM_ON_SIGERR                     = 0x0,
    NICAM_OFF_SIGERR                    = 0x1,

    BTSC_MONO_ON_NSR_RATIO              = 0x0,
    BTSC_MONO_OFF_NSR_RATIO             = 0x1,
    BTSC_PILOT_AMPLITUDE_ON             = 0x2,
    BTSC_PILOT_AMPLITUDE_OFF            = 0x3,
    BTSC_SAP_ON_NSR_RATIO               = 0x4,
    BTSC_SAP_OFF_NSR_RATIO              = 0x5,
    BTSC_STEREO_ON_RATIO                = 0x6,
    BTSC_STEREO_OFF_NSR_RATIO           = 0x7,
    BTSC_SAP_AMPLITUDE_ON               = 0x8,
    BTSC_SAP_AMPLITUDE_OFF              = 0x9,
    BTSC_HIDEV_ON_NSR_RATIO             = 0xA,
    BTSC_HIDEV_OFF_NSR_RATIO            = 0xB
}AUDIO_SIF_THRESHOLD_TYPE;

//======SIF Gain Type========
typedef enum
{
    PRESCALE_STEP_ONE_DB                = 0x04,
    SET_PRESCALE_A2_FM                  = 0x00,
    SET_PRESCALE_NICAM                  = 0x01,
    SET_PRESCALE_AM                     = 0x02,
    SET_PRESCALE_HIDEV                  = 0x03,
    SET_PRESCALE_FM_M                   = 0x04,
    SET_PRESCALE_HIDEV_M                = 0x05,

    SET_PRESCALE_BTSC                   = 0x00,
    SET_PRESCALE_BTSC_MONO              = 0x01,
    SET_PRESCALE_BTSC_STEREO            = 0x02,
    SET_PRESCALE_BTSC_SAP               = 0x03,

    SET_PRESCALE_FM_RADIO               = 0x00,
    SET_PRESCALE_FM_RADIO_MONO          = 0x01,
    SET_PRESCALE_FM_RADIO_STEREO        = 0x02
}AUDIO_SIF_GAIN_TYPE;

enum
{
    AU_STATUS_MUTE                      = 0x00,
    AU_STATUS_MONO                      = 0x01,    // Primary Mono
    AU_STATUS_STEREO                    = 0x02,    // Primary Stereo
    AU_STATUS_SAP                       = 0x04,    // Second Audio Program
    AU_STATUS_NICAM                     = 0x08,    // Second Audio Program
    AU_STATUS_NICAM_MONO                = 0x10
};

//  enum for sif command
typedef enum
{
    // need to compatible with En_AUD_SIF_CmdType at apiAUDIO.h
    AU_SIF_CMD_SET_STOP                 = 0x00,
    AU_SIF_CMD_SET_PLAY                 = 0x01,
    AU_SIF_CMD_ENABLE_HIDEV             = 0x02,
    AU_SIF_CMD_SET_HIDEV_FILTER_BW_LEVEL= 0x03,
    AU_SIF_CMD_RESET_FC_TRACKING        = 0x04,
    AU_SIF_CMD_ENABLE_FC_TRACKING       = 0x05,
    AU_SIF_CMD_SET_ADC_FROM_VIF_PATH    = 0x06,
    AU_SIF_CMD_ENABLE_AUTO_MUTE         = 0x07,
    AU_SIF_CMD_ENABLE_BEEPER_FUNCTION   = 0x08,
    AU_SIF_CMD_SET_BEEPER_TONE          = 0x09,
    AU_SIF_CMD_ENABLE_AGC               = 0x0A,
    AU_SIF_CMD_RESET_AGC                = 0x0B,
    AU_SIF_CMD_STANDARD_RESET           = 0x0C,
    AU_SIF_CMD_DETECT_MAIN_STD_ONLY     = 0x0D,
    AU_SIF_CMD_ENABLE_AUTO_SOUNDMODE    = 0x0E,
    AU_SIF_CMD_SET_FM_RADIO_DeEmp       = 0x0F,
    AU_SIF_CMD_SET_FM_RADIO_FC_TRACK    = 0x10,
    AU_SIF_CMD_FM_RADIO_RESET           = 0x11,
    AU_SIF_CMD_FMTX_SET_PREEMP          = 0x12,
    AU_SIF_CMD_SET_ENC_CMD              = 0x13,
    AU_SIF_CMD_SET_ENC_Input_attenuation                 = 0x14,
    AU_SIF_CMD_SET_ENC_Output_scaling                = 0x15,
    AU_SIF_CMD_SET_BTSC_Enc_M_Gain                 =0x16,
    AU_SIF_CMD_SET_BTSC_Enc_D_Gain                 = 0x17,
    AU_SIF_CMD_SET_BTSC_Enc_SAP_Gain                 = 0x18,

    // internal used at sif driver.
    AU_SIF_CMD_SET_STD                  = 0x40,
    AU_SIF_CMD_SET_SUB_CARRIER_STD      = 0x41,
    AU_SIF_CMD_SET_CARRIER_DEBOUNCE     = 0x42,
    AU_SIF_CMD_ENABLE_SIF_SYNTHESIZER   = 0x43,
    AU_SIF_CMD_ENABLE_CHANNEL           = 0x44,
    AU_SIF_CMD_SET_SIFFIFO_MODE         = 0x45,
    AU_SIF_CMD_DK123_MONITOR            = 0x46,

    // internal used at sif driver.
    AU_SIF_CMD_GET_HIDEV_ENABLE         = 0x80,
    AU_SIF_CMD_GET_HIDEV_FILTER_BW_LEVEL= 0x81,
    AU_SIF_CMD_GET_ADC_FROM_VIF_PATH    = 0x82,
    AU_SIF_CMD_GET_FM_A2_MODE_STATUS    = 0x83,
    AU_SIF_CMD_GET_BTSC_MODE_STATUS     = 0x84,
    AU_SIF_CMD_GET_NICAM_MODE_STATUS    = 0x85,
    AU_SIF_CMD_GET_STANDARD_STATUS      = 0x86,
    AU_SIF_CMD_GET_STANDARD_CMD         = 0x87,
    AU_SIF_CMD_GET_NSR_CMD              = 0x88,
    AU_SIF_CMD_GET_AMP_CMD              = 0x89,
    AU_SIF_CMD_GET_FM_RADIO_MODE_STATUS = 0x8A,
    AU_SIF_CMD_GET_FM_RADIO_DC_AMP      = 0x8B,
    AU_SIF_CMD_GET_FM_RADIO_NSR         = 0x8C,
    AU_SIF_CMD_GET_VIF_PATCH_ENABLE     = 0x90,
    AU_SIF_CMD_SET_SIF_SHIFT_CLK        = 0x91,
    AU_SIF_CMD_SET_FM_CH2_FILTER_BW_LEVEL= 0x92,

    AU_SIF_CMD2_ADC_VIA_VIF_PIN = 0x01
} AU_SIF_CMD;

#if defined(UFO_PUBLIC_HEADER_212)
//====== DSP code Type========
typedef enum
{
    AU_DVB_STANDARD_INVALID             = 0xFF,
    AU_DVB_STANDARD_DEC_DSP             = 0x00,    // define for dsp load code
    AU_DVB_STANDARD_DEC_DSP_EXT         = 0x10,    // define for dsp load code
    AU_DVB_NONE                         = 0x00,     //0x00
    AU_DVB_DEC_NONE,                    //0x01
    AU_DVB_ENC_NONE,                    //0x02
    AU_DVB_STANDARD_MPEG,               //0x03
    AU_DVB_STANDARD_AC3,                //0x04
    AU_DVB_STANDARD_AC3P,               //0x05
    AU_DVB_STANDARD_AAC,                //0x06
    AU_DVB_STANDARD_MP3,                //0x07
    AU_DVB_STANDARD_WMA,                //0x08
    AU_DVB_STANDARD_RESERVE3,           //0x09
    AU_DVB_STANDARD_RA8LBR,             //0x0A
    AU_DVB_STANDARD_XPCM,               //0x0B
    AU_DVB_STANDARD_TONE,               //0x0C
    AU_DVB_STANDARD_DTS,                //0x0D
    AU_DVB_STANDARD_MS10_DDT,           //0x0E
    AU_DVB_STANDARD_MS10_DDC,           //0x0F
    AU_DVB_STANDARD_WMA_PRO,            //0x10
    AU_DVB_STANDARD_FLAC,               //0x11
    AU_DVB_STANDARD_VORBIS,             //0x12
    AU_DVB_STANDARD_DTSLBR,             //0x13
    AU_DVB_STANDARD_AMR_NB,             //0x14
    AU_DVB_STANDARD_AMR_WB,             //0x15
    AU_DVB_STANDARD_DRA,                //0x16
    AU_DVB_STANDARD_MPEG_AD,            //0x17
    AU_DVB_STANDARD_DolbyTrueHD_Bypass, //0x18
    AU_DVB_STANDARD_DTSHD_ADO, //0x19
    AU_DVB_STANDARD_AC4,                //0x1A
    AU_DVB_STANDARD_MPEG_H,             //0x1B
    AU_DVB_STANDARD_OPUS,               //0x1C
    AU_DVB_STANDARD_DTSXLL,             //0x1D
    AU_DVB_STANDARD_MAX,

    AU_DVB_STANDARD_ENC                 = 0x20,    // define for dsp load code
    AU_DVB_STANDARD_MPEG_EN             = 0x20,

    AU_DVB2_STANDARD_SE_DSP             = 0x30,    // define for dsp load code
    //AU_DVB2_STANDARD_SE_DSP_EXT         = 0x40,    // need to refine ADV judgement first.
    AU_DVB2_NONE                        = 0x30,
    AU_DVB2_STANDARD_MPEG_AD ,
    AU_DVB2_STANDARD_AC3_AD,
    AU_DVB2_STANDARD_AC3P_AD,
    AU_DVB2_STANDARD_AAC_AD,
    AU_DVB2_STANDARD_DDE,
    AU_DVB2_STANDARD_GAAC_AD,
    AU_DVB2_STANDARD_MP3,
    AU_DVB2_STANDARD_MPEG_EN,
    AU_DVB2_STANDARD_XPCM,
    AU_DVB2_STANDARD_KTV,
    AU_DVB2_STANDARD_KTV2,
    AU_DVB2_STANDARD_DTS,
    AU_DVB2_STANDARD_TONEGEN,
    AU_DVB2_STANDARD_DolbyTrueHD_Bypass,
    AU_DVB2_STANDARD_DTSHD_ADO,
    AU_DVB2_STANDARD_AC4,
    AU_DVB2_STANDARD_MPEG_H,
    AU_DVB2_STANDARD_MAX,

    AU_DVB2_ADVSND                      = 0x40,    // define for dsp load code
    AU_DVB2_ADVSND_PL2                  = 0x40,
    AU_DVB2_ADVSND_BBE,
    AU_DVB2_ADVSND_SRS,
    AU_DVB2_ADVSND_VDS,
    AU_DVB2_ADVSND_VSPK,
    AU_DVB2_ADVSND_SUPV,
    AU_DVB2_ADVSND_TSHD,
    AU_DVB2_ADVSND_DTS_ULTRA_TV,
    AU_DVB2_ADVSND_TSHDVIQ,
    AU_DVB2_ADVSND_AUDSY,
    AU_DVB2_ADVSND_DBX,
    AU_DVB2_ADVSND_THEATERSOUND,
    AU_DVB2_ADVSND_PURESND,
    AU_DVB2_ADVSND_STUDIOSOUND_3D,
    AU_DVB2_ADVSND_MAX,

    AU_DEC_SYSTEM                       = 0x50,
    AU_SE_SYSTEM                        = 0x60,

    AU_KTV_ENC                          = 0x70,
    AU_KTV_FUNC                         = 0x80,
    AU_KTV_SYSTEM                       = 0x90,

    AU_SND_EFFECT                       = 0xa0,

    AU_STANDARD_SIF_TYPE                = 0xb0,
    AU_SIF_NONE                         = 0xb0,
    AU_SIF_BTSC ,
    AU_SIF_EIAJ,
    AU_SIF_PALSUM,
    AU_SIF_FM_RADIO,

    AU_ATVENC_NONE                      = 0xc0,
    AU_ATVENC_FMTx,
    AU_ATVENC_BTSC,

    AU_DVB2_ENCODE_NONE                 = 0xd0,    // define for snd_dsp encode code for because 0x30 has no free enum
    AU_DVB2_ENCODE_DTSE        = 0xd1,
    AU_DVB2_ENCODE_MS10_DDE    = 0xd2,
    AU_DVB2_ENCODE_DDE    = 0xd3,
    AU_DVB2_ENCODE_MAX,

    AU_DEC_ES_BYPASS,

}AUDIO_DSP_CODE_TYPE;
#endif

typedef enum
{
    AU_SIF_PAL_MONO,
    AU_SIF_PAL_HIDEV,
    AU_SIF_PAL_A2,
    AU_SIF_PAL_NICAM
}AUDIO_SIF_PAL_TYPE;

typedef enum
{
    AU_MODE_MONO,                       ///< MONO
    AU_MODE_STEREO,                     ///< STEREO
    AU_MODE_SAP,                        ///< SAP
    AU_MODE_LANG_A,                     ///< LANG_A
    AU_MODE_LANG_B,                     ///< LANG_B
    AU_MODE_LANG_AB,                    ///< LANG_AB
    AU_MODE_FORCE_NICAM,                ///< FORCE_NICAM
    AU_MODE_NICAM_MONO,
    AU_MODE_AUTO                        = 0x80      ///< auto switch to mono & mute
} AUDIO_SIF_SOUND_MODE;


//================================================================
//  DVB/MAD ENUM
//================================================================
//====== DSP Running Counter Type========
typedef enum
{
    DSP_DEC_MAIN_COUNTER,               /// Main counter for 1st DSP
    DSP_DEC_TIMER_COUNTER,              /// Timer counter for 1st DSP
    DSP_SE_MAIN_COUNTER,                /// Main counter for 2nd DSP
    DSP_SE_TIMER_COUNTER,               /// Timer counter for 2nd DSP
    DSP_SE_ISR_COUNTER,                 /// ISR counter for 2nd DSP
    DEC_R2_MAIN_COUNTER,                /// Main counter for DEC R2
    DEC_R2_TIMER_COUNTER,               /// Timer counter for DEC R2
    SND_R2_MAIN_COUNTER,                /// Main counter for SND R2
    SND_R2_TIMER_COUNTER                /// Timer counter for SND R2
} AUDIO_DSP_COUNTER_TYPE;

///< Audio decoder system (1st & 2nd) type
typedef enum
{
    MSAPI_AUD_DVB_INVALID               = 0xFF,     ///< 0xFF: System=Invalid

    //DTV / MM Audio main decoder
    MSAPI_AUD_DVB_NONE                  = 0x00,     ///< 0x00: System=None
    MSAPI_AUD_DVB_MPEG,                 ///< 0x01: System=MPEG
    MSAPI_AUD_DVB_AC3,                  ///< 0x02: System=AC3
    MSAPI_AUD_DVB_AC3P,                 ///< 0x03: System=AC3+
    MSAPI_AUD_DVB_AAC,                  ///< 0x04: System=AAC
    MSAPI_AUD_DVB_MP3,                  ///< 0x05: System=MP3
    MSAPI_AUD_DVB_WMA,                  ///< 0x06: System=WMA
    MSAPI_AUD_DVB_CDLPCM,               ///< 0x07: System=LPCM
    MSAPI_AUD_DVB_RA8LBR,               ///< 0x08: System=RA8 LBR
    MSAPI_AUD_DVB_XPCM,                 ///< 0x09: System=XPCM
    MSAPI_AUD_DVB_TONE,                 ///< 0x0A: System=TONE
    MSAPI_AUD_DVB_DTS,                  ///< 0x0B: System=DTS
    MSAPI_AUD_DVB_MS10_DDT,             ///< 0x0C: System=MS10 DDT
    MSAPI_AUD_DVB_MS10_DDC,             ///< 0x0D: System=MS10 DDC
    MSAPI_AUD_DVB_WMA_PRO,              ///< 0x0E: System=WMA PRO
    MSAPI_AUD_DVB_FLAC,                 ///< 0x0F: System=FLAC
    MSAPI_AUD_DVB_VORBIS,               // 0x10
    MSAPI_AUD_DVB_DTSLBR,               // 0x11
    MSAPI_AUD_DVB_AMR_NB,               // 0x12
    MSAPI_AUD_DVB_AMR_WB,               // 0x13
    MSAPI_AUD_DVB_DRA,                  // 0x14
    MSAPI_AUD_DVB_DolbyTrueHDBypass,    ///< 0x15: Dolby TruHD bypass
    MSAPI_AUD_DVB_DTSHDADO,             //0x16:
    MSAPI_AUD_DVB_DTSXLL,               // 0x17
    MSAPI_AUD_DVB_ES_Bypass,            //0x18:   System=ES Bypass
    MSAPI_AUD_DVB_AC4,                  //0x19
    MSAPI_AUD_DVB_MPEG_H,               //0x1A
    MSAPI_AUD_DVB_OPUS,                 //0x1B
    MSAPI_AUD_DVB_MAT,                  //0x1C
    MSAPI_AUD_DVB_MAX,

    //DTV Audio decoder 2 (for AD)
    MSAPI_AUD_DVB2_NONE                 = 0x30,     ///< 0x30: System=None
    MSAPI_AUD_DVB2_MPEG,                ///< 0x31: System=MPEG AD
    MSAPI_AUD_DVB2_AC3,                 ///< 0x32: System=AC3 AD
    MSAPI_AUD_DVB2_MP3,                 ///< 0x33: System=2nd MP3
    MSAPI_AUD_DVB2_AAC,                 ///< 0x34: System=AAC AD
    MSAPI_AUD_DVB2_GAAC,                ///< 0x35: System=GAAC AD
    MSAPI_AUD_DVB2_AC3P,                ///< 0x36: System=AC3P AD
    MSAPI_AUD_DVB2_DDE,                 ///< 0x37: System=AC3 Encoder
    MSAPI_AUD_DVB2_DTSE,                ///< 0x38: System=DTS Encoder
    MSAPI_AUD_DVB2_XPCM,
    MSAPI_AUD_DVB2_KTV,
    MSAPI_AUD_DVB2_KTV2,
    MSAPI_AUD_DVB2_DTS,
    MSAPI_AUD_DVB2_TONEGEN,
    MSAPI_AUD_DVB2_DolbyTrueHDBypass,    ///< 0x3E: Dolby TruHD bypass
    MSAPI_AUD_DVB2_DTSHDADO,             //0x3F:
    MSAPI_AUD_DVB2_AC4,                  //0x40:
    MSAPI_AUD_DVB2_MPEG_H,               //0x41
    MSAPI_AUD_DVB2_MAT,
    MSAPI_AUD_DVB2_MAX,

    //ATV SIF
    MSAPI_AUD_ATV_NONE                  = 0xB0,     ///< 0xB0: System=None
    MSAPI_AUD_ATV_BTSC,                 ///< 0xB1: System=BTSC
    MSAPI_AUD_ATV_EIAJ,
    MSAPI_AUD_ATV_PAL,                  ///< 0xB3: System=PALSUM
    MSAPI_AUD_ATV_FM_RADIO,             ///< 0xB4: System=FM_RADIO
    MSAPI_AUD_ATV_MAX,

    //ATV ENC
    MSAPI_AUD_ATVENC_NONE               = 0xC0,     ///< 0xC0: System=None
    MSAPI_AUD_ATVENC_FMTX,              ///< 0xC1: System=FM_Tx
    MSAPI_AUD_ATVENC_BTSC,              ///< 0xC2: System=BTSC_Enc
    MSAPI_AUD_ATVENC_MAX
} En_DVB_decSystemType;

typedef enum
{
    Audio_DEC_SYS                       = 0x00000000,
    Audio_DEC_PCM                       = 0x00000001,
    Audio_DEC_XPCM                      = 0x00000002,
    Audio_DEC_MPEG                      = 0x00000004,
    Audio_DEC_MP3                       = 0x00000008,
    Audio_DEC_AC3                       = 0x00000010,
    Audio_DEC_AC3P                      = 0x00000020,
    Audio_DEC_AAC_LC                    = 0x00000040,
    Audio_DEC_HEAAC_V1                  = 0x00000080,
    Audio_DEC_HEAAC_V2                  = 0x00000100,
    Audio_DEC_DOLBYPULSE                = 0x00000200,
    Audio_DEC_WMA                       = 0x00000400,
    Audio_DEC_WMAPRO                    = 0x00000800,
    Audio_DEC_RA8                       = 0x00001000,
    Audio_DEC_DTS                       = 0x00002000,
    Audio_DEC_FLAC                      = 0x00004000,
    Audio_DEC_VORBIS                    = 0x00008000,
    Audio_DEC_KTV                       = 0x00010000,
    Audio_DEC_KTV2                      = 0x00020000,
    Audio_DEC_AMR_NB                    = 0x00040000,
    Audio_DEC_AMR_WB                    = 0x00080000,
    Audio_DEC_DRA                       = 0x00100000,
    Audio_DEC_DTSLBR                    = 0x00200000,
    Audio_DEC_Dolby_TrueHD_Bypass_R2    = 0x00400000,
    Audio_DEC_DTS_HD_ADO_R2             = 0x00800000,
    Audio_DEC_DTSXLL                    = 0x01000000,
    Audio_DEC_AC4                       = 0x02000000,
    Audio_DEC_MPEG_H                    = 0x04000000,
    Audio_DEC_OPUS                      = 0x08000000,
    Audio_DEC_NULL                      = 0xFFFFFFFF

} MMA_AudioType;

//====== MMA ========
typedef enum
{
    MMA_STOP,
    MMA_PLAY,
    MMA_PLAY_FILETSP,
    MMA_RESYNC,
    MMA_PLAY_FILE,
    MMA_BROWSE,
    MMA_PAUSE,
    MMA_PLAY_FRAME,
    MMA_PLAY_FRAME_GS,
    MMA_PLAY_AD,
    MMA_STOP_AD,
    MMA_AVSYNC,
    MMA_FREERUN,
} MMA_AUDIO_CONTROL;

typedef enum
{
  /*********  New Interface ********/
    Audio_Comm_infoType_MMA_init = 0x60,
    Audio_Comm_infoType_MMA_finish,
    Audio_Comm_infoType_STC_Select,
    Audio_Comm_infoType_HDMI_ChannelStatus_1st_64bits,
    Audio_Comm_infoType_HDMI_ChannelStatus_2nd_64bits,
    Audio_Comm_infoType_HDMI_ChannelStatus_3rd_64bits,
    Audio_Comm_infoType_HDMI_UserData_1st_64bits,
    Audio_Comm_infoType_HDMI_UserData_2nd_64bits,
    Audio_Comm_infoType_HDMI_UserData_3rd_64bits,
    Audio_Comm_infoType_HDMI_HWbypassmode,
    Audio_Comm_infoType_setHdmiDelay,
    Audio_Comm_infoType_Is_Audio_Sync,
    Audio_comm_infoType_Get_Main_Input_Source_Info,
    Audio_comm_infoType_Get_Sub_Input_Source_Info,
    Audio_comm_infoType_Set_Input_Path,
    Audio_Comm_infoType_Get_Audio_Total_DDR_Size,

    /*******************************************************************/
    /* If you want add new cmd below,                                  */
    /* Please make sure ADEC 1 / 2/ 3 have "same" cmd in "same" order" */
    /*******************************************************************/
    Audio_Comm_infoType_ADEC1_capability = 0x70,
    Audio_Comm_infoType_ADEC1_setAudioDecoder,
    Audio_Comm_infoType_ADEC1_currAudDecStatus,
    Audio_Comm_infoType_ADEC1_AudioSyncStatus,   // 1 : AV-Sync  0: Free-run
    Audio_Comm_infoType_ADEC1_sampleRate,
    Audio_Comm_infoType_ADEC1_bitRate,
    Audio_Comm_infoType_ADEC1_acmod,
    Audio_Comm_infoType_ADEC1_soundmod,
    Audio_Comm_infoType_ADEC1_reqDataSize,
    Audio_Comm_infoType_ADEC1_okFrmCnt,
    Audio_Comm_infoType_ADEC1_errFrmCnt,
    Audio_Comm_infoType_ADEC1_1ms_timeStamp,
    Audio_Comm_infoType_ADEC1_33bit_PTS,
    Audio_Comm_infoType_ADEC1_33bit_STCPTS_DIFF,
    Audio_Comm_infoType_ADEC1_esBuf_reqFlag,       //new
    Audio_Comm_infoType_ADEC1_esBuf_reqAddr,
    Audio_Comm_infoType_ADEC1_esBuf_reqSize,
    Audio_Comm_infoType_ADEC1_esBuf_informDataRdy, //new
    Audio_Comm_infoType_ADEC1_esBuf_currLevel,
    Audio_Comm_infoType_ADEC1_pcmBuf_currLevel,
    Audio_Comm_infoType_ADEC1_playControl,
    Audio_Comm_infoType_ADEC1_FF2X,
    Audio_Comm_infoType_ADEC1_mute,
    Audio_Comm_infoType_syncSTC_in_mmTs,
    Audio_Comm_infoType_ADEC1_play_wo_output,
    Audio_Comm_infoType_ADEC1_audio_unsupported,
    Audio_Comm_infoType_ADEC1_UNI_PCMOutCnt,       // UNI_DEC1_PCM1 -> MIPS out size
    Audio_Comm_infoType_ADEC1_UNI_pcm_wptr,        // DEC1 -> UNI_DEC1_PCM1 wptr
    Audio_Comm_infoType_ADEC1_UNI_pcm_decSize,     // DEC1 -> UNI_DEC1_PCM1 w_size
    Audio_Comm_infoType_ADEC1_UNI_PCMInput_Addr,   // MISP -> UNI_DEC1_PCM3 wptr
    Audio_Comm_infoType_ADEC1_UNI_OutPCMLevel,     // UNI_DEC1_PCM3 level count
    Audio_Comm_infoType_ADEC1_avSyncOffset,
    Audio_Comm_infoType_ADEC1_Set_play_smpRate = 0x1000,
    Audio_Comm_infoType_ADEC1_Get_PTS_latency,
    Audio_Comm_infoType_ADEC1_Get_PCM_bufferEmpty,

    /********************************************************************/
    /* Please Add ADEC 1 cmd above                                      */
    /* Please make sure ADEC 1 / 2/ 3 have "same" cmd in "same" order"  */
    /********************************************************************/
    Audio_Comm_infoType_ADEC1_setDecParam = 0x8F,
    Audio_Comm_infoType_ADEC1_getDecInfo1 = 0x90,
    Audio_Comm_infoType_ADEC1_getDecInfo2,
    Audio_Comm_infoType_ADEC1_getDecInfo3,
    Audio_Comm_infoType_ADEC1_getDecInfo4,
    Audio_Comm_infoType_ADEC1_getDecInfo5,
    Audio_Comm_infoType_ADEC1_getDecInfo6,
    Audio_Comm_infoType_ADEC1_getDecInfo7,
    Audio_Comm_infoType_ADEC1_getDecInfo8,
    Audio_Comm_infoType_ADEC1_getDecChannelInfo,
    Audio_Comm_infoType_ADEC1_getlatest_PTS,
    Audio_Comm_infoType_ADEC1_avsync_delay,
    Audio_Comm_infoType_ADEC1_avsync_delay_upperlimit,

    /********************************************************************/
    /* If you want add new cmd below,                                   */
    /* Please make sure ADEC 1 / 2/ 3 have "same" cmd in "same" order"  */
    /********************************************************************/
    Audio_Comm_infoType_ADEC2_capability = 0xA0,
    Audio_Comm_infoType_ADEC2_setAudioDecoder,
    Audio_Comm_infoType_ADEC2_currAudDecStatus,
    Audio_Comm_infoType_ADEC2_AudioSyncStatus,   // 1 : AV-Sync  0: Free-run
    Audio_Comm_infoType_ADEC2_sampleRate,
    Audio_Comm_infoType_ADEC2_bitRate,
    Audio_Comm_infoType_ADEC2_acmod,
    Audio_Comm_infoType_ADEC2_soundmod,
    Audio_Comm_infoType_ADEC2_reqDataSize,
    Audio_Comm_infoType_ADEC2_okFrmCnt,
    Audio_Comm_infoType_ADEC2_errFrmCnt,
    Audio_Comm_infoType_ADEC2_1ms_timeStamp,
    Audio_Comm_infoType_ADEC2_33bit_PTS,
    Audio_Comm_infoType_ADEC2_33bit_STCPTS_DIFF,
    Audio_Comm_infoType_ADEC2_esBuf_reqFlag,
    Audio_Comm_infoType_ADEC2_esBuf_reqAddr,
    Audio_Comm_infoType_ADEC2_esBuf_reqSize,
    Audio_Comm_infoType_ADEC2_esBuf_informDataRdy,
    Audio_Comm_infoType_ADEC2_esBuf_currLevel,
    Audio_Comm_infoType_ADEC2_pcmBuf_currLevel,
    Audio_Comm_infoType_ADEC2_playControl,
    Audio_Comm_infoType_ADEC2_FF2X,
    Audio_Comm_infoType_ADEC2_mute,
    Audio_Comm_infoType_ADEC2_syncSTC_in_mmTs,
    Audio_Comm_infoType_ADEC2_play_wo_output,
    Audio_Comm_infoType_ADEC2_audio_unsupported,
    Audio_Comm_infoType_ADEC2_UNI_PCMOutCnt,       // UNI_DEC2_PCM1 -> MIPS out size
    Audio_Comm_infoType_ADEC2_UNI_pcm_wptr,        // DEC2 -> UNI_DEC2_PCM1 wptr
    Audio_Comm_infoType_ADEC2_UNI_pcm_decSize,     // DEC2 -> UNI_DEC2_PCM1 w_size
    Audio_Comm_infoType_ADEC2_UNI_PCMInput_Addr,   // MISP -> UNI_DEC2_PCM3 wptr
    Audio_Comm_infoType_ADEC2_UNI_OutPCMLevel,     // UNI_DEC2_PCM3 level count
    Audio_Comm_infoType_ADEC2_avSyncOffset,

    /********************************************************************/
    /* Please Add ADEC 2 cmd above                                      */
    /* Please make sure ADEC 1 / 2/ 3 have "same" cmd in "same" order"  */
    /********************************************************************/
    Audio_Comm_infoType_ADEC2_setDecParam = 0xBF,
    Audio_Comm_infoType_ADEC2_getDecInfo1 = 0xC0,
    Audio_Comm_infoType_ADEC2_getDecInfo2,
    Audio_Comm_infoType_ADEC2_getDecInfo3,
    Audio_Comm_infoType_ADEC2_getDecInfo4,
    Audio_Comm_infoType_ADEC2_getDecInfo5,
    Audio_Comm_infoType_ADEC2_getDecInfo6,
    Audio_Comm_infoType_ADEC2_getDecInfo7,
    Audio_Comm_infoType_ADEC2_getDecInfo8,
    Audio_Comm_infoType_ADEC2_getDecChannelInfo,

    /**********************************************************************/
    /* If you want add new cmd below,                                     */
    /* Please make sure ADEC 1 / 2/ 3 have "same" cmd in "same" order"    */
    /**********************************************************************/
    Audio_Comm_infoType_ADEC3_capability = 0xD0,
    Audio_Comm_infoType_ADEC3_setAudioDecoder,
    Audio_Comm_infoType_ADEC3_currAudDecStatus,
    Audio_Comm_infoType_ADEC3_AudioSyncStatus,   // 1 : AV-Sync  0: Free-run
    Audio_Comm_infoType_ADEC3_sampleRate,
    Audio_Comm_infoType_ADEC3_bitRate,
    Audio_Comm_infoType_ADEC3_acmod,
    Audio_Comm_infoType_ADEC3_soundmod,
    Audio_Comm_infoType_ADEC3_reqDataSize,
    Audio_Comm_infoType_ADEC3_okFrmCnt,
    Audio_Comm_infoType_ADEC3_errFrmCnt,
    Audio_Comm_infoType_ADEC3_1ms_timeStamp,
    Audio_Comm_infoType_ADEC3_33bit_PTS,
    Audio_Comm_infoType_ADEC3_33bit_STCPTS_DIFF,
    Audio_Comm_infoType_ADEC3_esBuf_reqFlag,
    Audio_Comm_infoType_ADEC3_esBuf_reqAddr,
    Audio_Comm_infoType_ADEC3_esBuf_reqSize,
    Audio_Comm_infoType_ADEC3_esBuf_informDataRdy,
    Audio_Comm_infoType_ADEC3_esBuf_currLevel,
    Audio_Comm_infoType_ADEC3_pcmBuf_currLevel,
    Audio_Comm_infoType_ADEC3_playControl,
    Audio_Comm_infoType_ADEC3_FF2X,
    Audio_Comm_infoType_ADEC3_mute,
    Audio_Comm_infoType_ADEC3_syncSTC_in_mmTs,
    Audio_Comm_infoType_ADEC3_play_wo_output,
    Audio_Comm_infoType_ADEC3_audio_unsupported,
    Audio_Comm_infoType_ADEC3_UNI_PCMOutCnt,       // UNI_DEC3_PCM1 -> MIPS out size
    Audio_Comm_infoType_ADEC3_UNI_pcm_wptr,        // DEC3 -> UNI_DEC3_PCM1 wptr
    Audio_Comm_infoType_ADEC3_UNI_pcm_decSize,     // DEC3 -> UNI_DEC3_PCM1 w_size
    Audio_Comm_infoType_ADEC3_UNI_PCMInput_Addr,   // MISP -> UNI_DEC3_PCM3 wptr
    Audio_Comm_infoType_ADEC3_UNI_OutPCMLevel,     // UNI_DEC3_PCM3 level count
    Audio_Comm_infoType_ADEC3_avSyncOffset,

    /**********************************************************************/
    /* Please Add ADEC 3 cmd above                                        */
    /* Please make sure ADEC 1 / 2/ 3 have "same" cmd in "same" order"    */
    /**********************************************************************/
    Audio_Comm_infoType_ADEC3_setDecParam = 0xEF,
    Audio_Comm_infoType_ADEC3_getDecInfo1 = 0xF0,
    Audio_Comm_infoType_ADEC3_getDecInfo2,
    Audio_Comm_infoType_ADEC3_getDecInfo3,
    Audio_Comm_infoType_ADEC3_getDecInfo4,
    Audio_Comm_infoType_ADEC3_getDecInfo5,
    Audio_Comm_infoType_ADEC3_getDecInfo6,
    Audio_Comm_infoType_ADEC3_getDecInfo7,
    Audio_Comm_infoType_ADEC3_getDecInfo8,
    Audio_Comm_infoType_ADEC3_getDecChannelInfo,
    Audio_Comm_infoType_ADEC3_getlatest_PTS,
    Audio_Comm_infoType_ADEC3_avsync_delay,
    Audio_Comm_infoType_ADEC3_avsync_delay_upperlimit,

    /****************************************************/
    /*  General audio cmd  */
    /****************************************************/
    Audio_Comm_infoType_DecStatus = 0x00,
    Audio_Comm_infoType_SampleRate,
    Audio_Comm_infoType_SoundMode,
    Audio_Comm_infoType_DecOutMode,
    Audio_Comm_infoType_ChannelMode,
    Audio_Comm_infoType_MMFileSize,
    Audio_Comm_infoType_33Bit_PTS,
    Audio_Comm_infoType_33Bit_STCPTS_DIFF,
    Audio_Comm_infoType_1ms_PTS,
    Audio_Comm_infoType_DEC1_BufferSize,
    Audio_Comm_infoType_DEC1_BufferAddr,
    Audio_Comm_infoType_DEC1_MMTag,
    Audio_Comm_infoType_DEC1_MMResidualPCM,
    Audio_Comm_infoType_DEC1_ESBufferSize,
    Audio_Comm_infoType_DEC1_PCMBufferSize,
    Audio_Comm_infoType_DEC2_BufferSize,
    Audio_Comm_infoType_DEC2_BufferAddr = 0x10,
    Audio_Comm_infoType_DEC2_MMTag,
    Audio_Comm_infoType_DEC2_MMResidualPCM,
    Audio_Comm_infoType_DecodeErrorCnt,
    Audio_Comm_infoType_MM_FFx2,
    Audio_Comm_infoType_setBypassSPDIF_PAPB_chk,
    Audio_Comm_infoType_CompressBin_LoadCode,
    Audio_Comm_infoType_CompressBin_DDRAddress,
    Audio_Comm_infoType_DMAReader_BufferLevel,
    Audio_Comm_infoType_DMAReader_Command,
    Audio_Comm_infoType_SetSCMS,
    Audio_Comm_infoType_GetSCMS,
    Audio_Comm_infoType_ADC_InputGain,
    Audio_Comm_infoType_KTV_SetType,
    Audio_Comm_infoType_getSignal_Energy,
    Audio_Comm_infoType_getNR_Status,
    Audio_Comm_infoType_setNR_Threshold = 0x20,
    Audio_Comm_infoType_setSPDIF_FS,
    Audio_Comm_infoType_getSPDIF_FS,
    Audio_Comm_infoType_setSpdifDelay,
    Audio_Comm_infoType_ReadByte,
    Audio_Comm_infoType_WriteByte,
    Audio_Comm_infoType_hdmiTx_outType,
    Audio_Comm_infoType_hdmiTx_outFreq,
    Audio_Comm_infoType_PTS_info,
    Audio_Comm_infoType_RTSP_Mem,
    Audio_Comm_infoType_setSpdif_BufferProcess,
    Audio_Comm_infoType_DEC1_setBufferProcess,
    Audio_Comm_infoType_setES_REQ_SZ,
    Audio_Comm_infoType_AD_OutputStyle,
    Audio_Comm_infoType_getHDMI_CopyRight_C_Bit,
    Audio_Comm_infoType_getHDMI_CopyRight_L_Bit,
    Audio_Comm_infoType_Set_UNI_NEED_DECODE_FRMCNT = 0x30,
    Audio_Comm_infoType_Set_UNI_ES_Wptr,
    Audio_Comm_infoType_Get_UNI_ES_MEMCNT,
    Audio_Comm_infoType_Set_MENU_WT_PTR,
    Audio_Comm_infoType_Get_MENU_WT_PTR,
    Audio_Comm_infoType_Get_MENU_KEY_CNT,
    Audio_Comm_infoType_Get_CurSynthRate,
    Audio_Comm_infoType_ADC1_InputGain,
    Audio_Comm_infoType_mainpath_mute_status,
    Audio_Comm_infoType_spdif_mute_status,
    Audio_Comm_infoType_hdmiTX_mute_status,
    Audio_Comm_infoType_SRC_Compensation,
    Audio_Comm_infoType_Dump_Dsp_Info,
    Audio_Comm_infoType_Set_Video_STCPTS_DIFF,
    Audio_Comm_infoType_Get_UNI_ES_Base,
    Audio_Comm_infoType_Get_UNI_ES_Size,
    Audio_Comm_infoType_Get_UNI_PCM_Base = 0x40,
    Audio_Comm_infoType_Get_UNI_PCM_Size,
    Audio_Comm_infoType_Get_AccumEsCnt,
    Audio_Comm_infoType_DSP_PLL_Setting,
    Audio_Comm_infoType_Set_CallBackFunc,
    Audio_Comm_infoType_Set_ALSA_Mode,  /* For OMX Dual Mono Setting */
    Audio_Comm_infoType_HDMI_Unstable_Protect,
    Audio_Comm_infoType_HDMI_Unstable_Threshold,
    Audio_Comm_infoType_Set_TEE_Capibility,
    Audio_Comm_infoType_Parser_Spec,  /* Support ISO 11172-1 for MM */
    Audio_Comm_infoType_Get_MultiPlayer_Capability,
    Audio_Comm_infoType_Get_Parser_Architecture,
    Audio_Comm_infoType_Set_CfgCtrlRegs_Lock,
    Audio_Comm_infoType_DMAReader_BufferSize,
    Audio_Comm_infoType_PUSI_disable_Info,  /*For STB only PES file in tsp.(need disable PUSI)*/
    Audio_Comm_infoType_Set_Parser,
    Audio_Comm_infoType_Parser_Info = 0x50,
    Audio_Comm_infoType_Get_Dolby_TrueHD_Bypass_Capability,
    Audio_Comm_infoType_Set_DFS_PowerSave,   /*DFS setting for saving power*/
    Audio_Comm_infoType_Is_Decoder_In_R2,  /*For IC with R2 Decoder*/
    Audio_comm_infoType_Get_audio_crash_status,
    Audio_Comm_infoType_mainAudio_setBufferProcess,
    Audio_comm_infoType_Get_Main_Input_Type,
    Audio_comm_infoType_Get_Sub_Input_Type,
    Audio_Comm_infoType_Multi_Channel,
    Audio_Comm_infoType_Vorbis_header_size = 0x5F,

    Audio_Comm_infoType_ES_PassThrough_DelaySmp = 0x2000,
    Audio_Comm_infoType_ES_PassThrough_PcmSmpRate,
    Audio_Comm_infoType_ES_PassThrough_HdmiSmpRate,

    Audio_Comm_infoType_HDMITx_Status,

    Audio_Comm_infoType_set_PAL_M_Type = 0x4000,
    Audio_Comm_infoType_set_PAL_N_Type,
    Audio_Comm_infoType_set_BTSC_M_Type,

    Audio_Comm_infoType_Get_SE_Input_Main_BufferBase = 0x8000,
    Audio_Comm_infoType_Get_SE_Input_Main_BufferSize,
    Audio_Comm_infoType_Get_SE_Output_Main_BufferBase,
    Audio_Comm_infoType_Get_SE_Output_Main_BufferSize,
    Audio_Comm_infoType_Get_SE_PROCESS_FRAME_SMP_UNIT,
    Audio_Comm_infoType_Get_SE_PROCESS_FETCH_CHANNELS,
    Audio_Comm_infoType_Get_SE_PROCESS_STORE_CHANNELS,
    Audio_Comm_infoType_Get_SE_PROCESS_FETCH_FRAME_BYTE_SIZE,
    Audio_Comm_infoType_Get_SE_PROCESS_STORE_FRAME_BYTE_SIZE,
    Audio_Comm_infoType_Set_SE_Input_Main_Buffer_Feed = 0x8100,
    Audio_Comm_infoType_Set_SE_Output_Main_Buffer_Consume,
    Audio_Comm_infoType_Set_SE_Test_Init,
    Audio_Comm_infoType_Set_SE_Test_End,
    Audio_Comm_infoType_Set_SE_TONE_Test_Begin,
    Audio_Comm_infoType_Set_SE_PEQ_Test_Begin,
    Audio_Comm_infoType_Set_SE_GEQ_Test_Begin,
    Audio_Comm_infoType_Set_SE_AVC_Test_Begin,
    Audio_Comm_infoType_Set_SE_SRS_THEATER_SOUND_Begin,
    Audio_Comm_infoType_Set_SE_SONICEMOTION_ABS3D_Begin,
    Audio_Comm_infoType_Set_SE_BONGIOVI_DPS_Begin,
    Audio_Comm_infoType_Set_Debug_LOG_Level,
    Audio_Comm_infoType_Get_Dra_ChannelMode,
    Audio_Comm_infoType_Get_Max_DelayTime,
    Audio_Comm_infoType_Get_Min_DelayTime,
    Audio_Comm_infoType_Set_SE_DTS_STUDIOSOUND_3D_Begin,

    Audio_Comm_infoType_AUDIO_Get_ADEC0_R2CodeType,
    Audio_Comm_infoType_AUDIO_Get_ADEC1_R2CodeType,
    Audio_Comm_infoType_AUDIO_GetDsp2CodeType,
    Audio_Comm_infoType_LFE_Channel_Info,
    Audio_Comm_infoType_DAC_Clock_OnOff,
    Audio_Comm_infoType_MMTS_KEEP_33bit_PTS_FLAG,
    Audio_Comm_infoType_AUDIO_CHUNK_PTS,
    Audio_Comm_infoType_Get_AD_Cabability,

    Audio_Comm_infoType_Set_1st_PreferLang,
    Audio_Comm_infoType_Set_2nd_PreferLang,
    Audio_Comm_infoType_Set_AC4_DE_Gain,
    Audio_Comm_infoType_Set_AC4_MD_PresentationIndex,
    Audio_Comm_infoType_Set_AC4_AD_PresentationIndex,

    Audio_Comm_infoType_ADEC1_Get_mpegH_audioSceneConfig_valid,
    Audio_Comm_infoType_ADEC1_Get_mpegH_audioSceneConfig_bufAddr,
    Audio_Comm_infoType_ADEC1_Get_mpegH_audioSceneConfig_bufSize,
    Audio_Comm_infoType_ADEC1_Set_mpegH_actionEvent,

    Audio_Comm_infoType_Get_SE_DTS_SRS_Version,
    Audio_Comm_infoType_Set_HDMI_Rx2Tx_HwPassThrough,
    Audio_Comm_infoType_Set_HDMI_Rx_HighBitRate,
    Audio_Comm_infoType_ADEC1_set_AC4_1th_pref_lang,
    Audio_Comm_infoType_ADEC1_set_AC4_2th_pref_lang,
    Audio_Comm_infoType_ADEC1_set_AC4_de_gain,
    Audio_Comm_infoType_ADEC1_set_AC4_MD_pres_index,
    Audio_Comm_infoType_ADEC1_set_AC4_AD_pres_index,
    Audio_Comm_infoType_ADEC1_Decoder_Output_Fading,
    Audio_Comm_infoType_ADEC2_Decoder_Output_Fading,

    Audio_Comm_infoType_Main_Pcm_Level,
    Audio_Comm_infoType_Hdmi_Nonpcm_Level,
    Audio_Comm_infoType_Spdif_Nonpcm_Level,
    Audio_Comm_infoType_Cvbs_Volume,
    Audio_Comm_infoType_Hdmi_Volume,
    Audio_Comm_infoType_Spdif_Volume,
    Audio_Comm_infoType_Hdmi_Pcm_Mode,
    Audio_Comm_infoType_Spdif_Pcm_Mode,
    Audio_Comm_infoType_SE_Buffer_Empty_Count,

    Audio_Comm_infoType_Get_SE_SONICEMOTION_ABS3D_Version,
    Audio_Comm_infoType_Get_SE_Number_Of_PEQ_Band,
    Audio_Comm_infoType_Get_SE_DOLBY_DAP_Version,

    Audio_Comm_infoType_Set_Speaker_Configuration,
    Audio_Comm_infoType_Set_AC4_Language_Type,
} Audio_COMM_infoType;

//================================================================
//  Common PCM I/O ENUM
//================================================================
typedef enum
{
    AUDIO_PCM_CMD_ALL, /* GET */
    AUDIO_PCM_CMD_NONBLOCKING, /* SET/GET */
    AUDIO_PCM_CMD_MULTICH, /* GET */
    AUDIO_PCM_CMD_MIXING, /* GET */
    AUDIO_PCM_CMD_MIXINGGROUP, /* SET/GET */
    AUDIO_PCM_CMD_BUFFER, /* GET only */
    AUDIO_PCM_CMD_BUFFERDURATION, /* SET/GET */
    AUDIO_PCM_CMD_READPTR, /* GET only */
    AUDIO_PCM_CMD_WRITEPTR, /* GET only */
    AUDIO_PCM_CMD_CHANNEL, /* SET/GET */
    AUDIO_PCM_CMD_SAMPLERATE, /* SET/GET */
    AUDIO_PCM_CMD_BITWIDTH, /* SET/GET */
    AUDIO_PCM_CMD_BIGENDIAN, /* SET/GET */
    AUDIO_PCM_CMD_TIMESTAMP, /* SET/GET */
    AUDIO_PCM_CMD_WEIGHTING, /* SET/GET */
    AUDIO_PCM_CMD_VOLUME, /* SET/GET */
    AUDIO_PCM_CMD_BUFFERLEVEL, /* GET only */
    AUDIO_PCM_CMD_MUTE, /* SET/GET */
    AUDIO_PCM_CMD_BUFFERSIZE, /* GET only */
    AUDIO_PCM_CMD_DATAPORT, /* SET */
    AUDIO_PCM_CMD_AUTORUN, /* SET/GET */
} AUDIO_PCM_COMMAND;

// MPEG
typedef enum
{   //info Type name                    //Attibtue       //Parameter
    Audio_MPEG_infoType_DecStatus,      //R              //0:unlock  1:lock
    Audio_MPEG_infoType_SampleRate,     //R              //
    Audio_MPEG_infoType_BitRate,        //R              //
    Audio_MPEG_infoType_FrameNum,       //R              //
    Audio_MPEG_infoType_SoundMode,      //W              //0: LR / 1:LL / 2:RR
    Audio_MPEG_infoType_stereoMode,     //R              //0: Stereo / 1: / 2:RR
    Audio_MPEG_infoType_Layer,          //R              //1 / 2 / 3
    Audio_MPEG_infoType_Header,         //R              //
    Audio_MPEG_infoType_FFSpeed,        //W              //
    Audio_MPEG_infoType_MMFileSize,     //W              //enum FILE_SIZE
    Audio_MPEG_infoType_Emphasis,       //R              //
    Audio_MPEG_DTV_AVSYNC_OFFSET,       //W, unit= 33bits offset
    Audio_MPEG_infoType_PCMBufLev,      //RW
    Audio_MPEG_infoType_BufferDelay,    //RW
    Audio_MPEG_DTV_AVSYNC_DISABLE_SETTING,
    Audio_MPEG_MuteFrm_PcmHighLevel_Threshold_NumTimeOut,
    Audio_MPEG_MuteFrm_PcmHighLevel_MuteNum,
    Audio_MPEG_infoType_PcmHighLevel_Pcmdata
} Audio_MPEG_infoType;

typedef enum
{
    AUD_MPEG_SOUNDMODE_LL,
    AUD_MPEG_SOUNDMODE_RR,
    AUD_MPEG_SOUNDMODE_LR,
    AUD_MPEG_SOUNDMODE_MIX_LR
}AUD_MPEG_SOUNDMODE;

typedef enum
{
    MPEG_SOUND_MODE_STEREO =0,
    MPEG_SOUND_MODE_JOINT_STEREO,
    MPEG_SOUND_MODE_DUAL_CHAN,
    MPEG_SOUND_MODE_MONO
}MPEG_SOUND_MODE;

/// MP3 Layer
typedef enum
{
    Audio_MP3_LAYER_RESERVED            = 0x0,
    Audio_MP3_LAYER_3                   = 0x1,
    Audio_MP3_LAYER_2                   = 0x2,
    Audio_MP3_LAYER_1                   = 0x3,
    Audio_MP3_LAYER_NOT_DETERMINE_YET   = 0xFF,
} Audio_MP3_LAYER;

// Dolby AC3
typedef enum
{   //info Type name                    //Attribute      //Parameter
    Audio_AC3_infoType_DecStatus,       //R              //0:unlock  1:lock
    Audio_AC3_infoType_SampleRate,      //R              //
    Audio_AC3_infoType_BitRate,         //R              //
    Audio_AC3_infoType_Dialnorm,        //R              //
    Audio_AC3_infoType_Acmod,           //R              //
    Audio_AC3_infoType_DrcMode,         //RW             //
    Audio_AC3_infoType_DownmixMode,     //RW             //
    Audio_AC3_infoType_MMFileSize,      //Not supported  //
    Audio_AC3_infoType_PCMBufLevel,     //R              //
    Audio_AC3_infoType_BufferDelay,     //RW             //
    Audio_AC3_infoType_SyncwordFound_Count,
    Audio_AC3_infoType_AD_Mixer_Balance,
} Audio_AC3_infoType;

// Dolby AC3P
typedef enum
{   //info Type name                     //Attribute     //Parameter
    Audio_AC3P_infoType_DecStatus,       //R             //
    Audio_AC3P_infoType_SampleRate,      //R             //
    Audio_AC3P_infoType_BitRate,         //R             //
    Audio_AC3P_infoType_Dialnorm,        //R             //
    Audio_AC3P_infoType_Acmod,           //R             //
    Audio_AC3P_infoType_DrcMode,         //RW            //
    Audio_AC3P_infoType_DownmixMode,     //RW            //
    Audio_AC3P_infoType_MMFileSize,      //W             //
    Audio_AC3P_infoType_syncSTC_in_mmTs, //W             //
    Audio_AC3P_infoType_hdmiTxBypass_enable,
    Audio_AC3P_infoType_AC3Type,
    Audio_AC3P_infoType_FrameSize,
    Audio_AC3P_infoType_BSMode,
    Audio_AC3P_infoType_BYTESWAP,
    Audio_AC3P_infoType_enableDolbyBulletin11,//W       // True or False          //default is enabled
    Audio_AC3P_DTV_AVSYNC_OFFSET,        //W, unit= 33bits offset
    Audio_AC3P_infoType_hdmiAC3inSE,
    Audio_AC3P_DTV_AVSYNC_DISABLE_SETTING,
    Audio_AC3P_infoType_hdmi_in_support_DDP,
    Audio_AC3P_infoType_Drc_HighCutScale,
    Audio_AC3P_infoType_Drc_LowBootScale,
    Audio_AC3P_infoType_LoudnessMatching,
    Audio_AC3P_infoType_ATMOS_support,
    Audio_AC3P_infoType_Mixer_Balance,
    Audio_AC3P_infoType_DDP_AD_Substream,
    Audio_AC3P_infoType_AD_Mixer_Balance,
    Audio_AC3P_infoType_DDPEnc_Mode,
    Audio_AC3P_infoType_DDP_71_Stream,
    Audio_AC3P_infoType_SyncwordFound_Count,
    Audio_AC3P_infoType_Reset_Atmos_EVO_Index_Buffer,
} Audio_AC3P_infoType;

 typedef enum
 {
    Audio_DDPEnc_DDP,
    Audio_DDPEnc_DD,
} Audio_DDPEnc_Mode;

typedef enum
{
    AUD_Dolby_SOUNDMODE_LR              = 0x0,
    AUD_Dolby_SOUNDMODE_LL              = 0x1,
    AUD_Dolby_SOUNDMODE_RR              = 0x2,
    AUD_Dolby_SOUNDMODE_MIXED_LR        = 0x3,
}AUD_Dolby_SOUNDMODE;

typedef enum
{
    DOLBY_MONO,
    DOLBY_STEREO,
    DOLBY_MULTI_CHAN
}DOLBY_AC_MODE;

typedef enum
{
    DOLBY_DOWNMIX_MODE_LTRT,
    DOLBY_DOWNMIX_MODE_LORO,
    DOLBY_DOWNMIX_MODE_AUTO,
    DOLBY_DOWNMIX_MODE_ARIB,
}DOLBY_DOWNMIX_MODE;

typedef enum
{
    LINE_MODE,
    RF_MODE,
}DOLBY_DRC_MODE;

typedef enum
{
    DVB_MODE,
    ATSC_MODE,
    OTHER_MODE
}DOLBY_LOUDNESS_MODE;

typedef enum
{
    BALANCE_EQUAL = 0,
    BALANCE_MAIN_0 = 1,
    BALANCE_MAIN_25 = 2,
    BALANCE_MAIN_75 = 3,
    BALANCE_MAIN_100 = 4,
}DOLBY_MIXER_BALANCE_MODE;

typedef enum
{
    AC3_ENABLE,
    AC3_DISABLE
}AC3_DIS_EN;


// DTS
typedef enum
{   //info Type name                     //Attribute    //Parameter
    Audio_DTS_infoType_DecStatus,        //R            //
    Audio_DTS_infoType_KernelVersion,    //R            //
    Audio_DTS_infoType_SampleRate,       //R            //
    Audio_DTS_infoType_BitRate,          //R            //
    Audio_DTS_infoType_CD_MODE,          //R            //
    Audio_DTS_infoType_Amode,            //R            //
    Audio_DTS_infoType_DMIX_LFE_EN,      //RW           //
    Audio_DTS_infoType_DrcMode,          //RW           //
    Audio_DTS_infoType_DownmixMode,      //RW           //
    Audio_DTS_infoType_Dialnorm_EN,      //RW           //
    Audio_DTS_infoType_ESDataServiceMode,//RW           //
    Audio_DTS_infoType_MMFileSize,       //W            //
    Audio_DTS_infoType_syncSTC_in_mmTs,  //W            //
    Audio_DTS_infoType_ESData_ByteSwap,  //W            //
    Audio_DTS_infoType_CertMode,         //W            //
    Audio_DTS_infoType_HDMI_DSP_SEL,
    Audio_DTS_infoType_DTSHD_HDMITxBypass_Enable,  //W            //
} Audio_DTS_infoType;

typedef enum
{   //ctrl Type name                     //Attribute
    Audio_DTS_Ctrl_DMIX_LFE_OFF         = 0x0,
    Audio_DTS_Ctrl_DMIX_LFE_ON          = 0x1,
    Audio_DTS_Ctrl_Drc_OFF              = 0x0,
    Audio_DTS_Ctrl_Drc_50               = 0x1,
    Audio_DTS_Ctrl_Drc_100              = 0x2,
    Audio_DTS_Ctrl_Downmix_LoRo         = 0x0,
    Audio_DTS_Ctrl_Downmix_LtRt         = 0x1,
    Audio_DTS_Ctrl_Dialnorm_OFF         = 0x0,
    Audio_DTS_Ctrl_Dialnorm_ON          = 0x1,
    Audio_DTS_Ctrl_POLLINGMode          = 0x0,
    Audio_DTS_Ctrl_INTRPTMode           = 0x1,
    Audio_DTS_Ctrl_ByteSwap_OFF         = 0x0,
    Audio_DTS_Ctrl_ByteSwap_ON          = 0x1,
    Audio_DTS_Ctrl_CertMode_OFF         = 0x0,
    Audio_DTS_Ctrl_CertMode_ON          = 0x1,
    Audio_DTS_Ctrl_DTSHDBypass_OFF      = 0x0,
    Audio_DTS_Ctrl_DTSHDBypass_ON       = 0x1,
} Audio_DTS_CtrlType;


typedef enum
{
    Audio_WMA_infoType_DecStatus,
    Audio_WMA_infoType_SampleRate,
    Audio_WMA_infoType_BitRate,
    Audio_WMA_infoType_Asf_Param,
    Audio_WMA_infoType_FFSpeed,
    Audio_WMA_infoType_MMFileSize
} Audio_WMA_infoType;

typedef enum
{   //info Type name                    //Attibtue      //Parameter
    Audio_AAC_error_count,              //R             //
    Audio_AAC_infoType_DrcMode,         //RW            //
    Audio_AAC_infoType_DownmixMode,     //RW            //
    Audio_AAC_dec_unsupported,          //R             //
    Audio_AAC_ts_file_end_ntfi,         //R
    Audio_AAC_infoType_MMFileSize,      //W
    Audio_AAC_DTV_AVSYNC_OFFSET,        //W, unit= 33bits offset
    Audio_AAC_DTV_AVSYNC_DISABLE_SETTING,
    Audio_AAC_mm_single_frame_mode,
    Audio_AAC_type,                      //0: AAC-LC, 1: HEAAC_V1, 2: HEAAC_V2
    Audio_AAC_AD_Mixer_Balance,
    Audio_AAC_infoType_Bypass_Enable,
} Audio_AAC_infoType;

typedef enum
{   //info Type name                    //Attibtue      //Parameter
    Audio_RA8LBR_NumCodes,              //w
    Audio_RA8LBR_Samples,               //w
    Audio_RA8LBR_SampleRate,            //w

    Audio_RA8LBR_Channels1,             //w
    Audio_RA8LBR_Regions1,              //w
    Audio_RA8LBR_cplStart1,             //w
    Audio_RA8LBR_cplQbits1,             //w
    Audio_RA8LBR_FrameSize1,            //w

    Audio_RA8LBR_Channels2,             //w
    Audio_RA8LBR_Regions2,              //w
    Audio_RA8LBR_cplStart2,             //w
    Audio_RA8LBR_cplQbits2,             //w
    Audio_RA8LBR_FrameSize2,            //w

    Audio_RA8LBR_Channels3,             //w
    Audio_RA8LBR_Regions3,              //w
    Audio_RA8LBR_cplStart3,             //w
    Audio_RA8LBR_cplQbits3,             //w
    Audio_RA8LBR_FrameSize3,            //w

    Audio_RA8LBR_Channels4,             //w
    Audio_RA8LBR_Regions4,              //w
    Audio_RA8LBR_cplStart4,             //w
    Audio_RA8LBR_cplQbits4,             //w
    Audio_RA8LBR_FrameSize4,            //w

    Audio_RA8LBR_Channels5,             //w
    Audio_RA8LBR_Regions5,              //w
    Audio_RA8LBR_cplStart5,             //w
    Audio_RA8LBR_cplQbits5,             //w
    Audio_RA8LBR_FrameSize5,            //w

    Audio_RA8LBR_infoType_DecStatus,    //R
} Audio_RA8LBR_infoType;

typedef enum
{
    DRA_Mono,
    DRA_Stereo,
    DRA_2_1,
    DRA_3_0,
    DRA_3_1,
    DRA_4_0,
    DRA_4_1,
    DRA_5_0,
    DRA_5_1,
    DRA_6_0,
    DRA_6_1,
    DRA_7_0,
    DRA_7_1,
    DRA_CH_MODE_NONE,
} DRA_CH_MODE;

typedef enum
{
    LPCM                                = 1,
    MS_ADPCM                            = 2,
    G711_A_LAW                          = 6,
    G711_u_LAW                          = 7,
    DVD_LPCM                            = 8,
    PRIVATE1_LPCM                       = 9,
    IMA_ADPCM                           = 17,
    IMA_ADPCM_APPLE                     = 18, // apple IMA4
}XPCM_TYPE;

typedef enum
{
    SAMPLE_RATE_48000,
    SAMPLE_RATE_44100,
    SAMPLE_RATE_32000,
    SAMPLE_RATE_24000,
    SAMPLE_RATE_22050,
    SAMPLE_RATE_16000,
    SAMPLE_RATE_12000,
    SAMPLE_RATE_11025,
    SAMPLE_RATE_8000,
    SAMPLE_RATE_4000,
    SAMPLE_RATE_96000,
}SAMPLE_RATE;

// AD & Mixer
typedef enum
{
    MAIN_MIX_AD,
    AD_MIX_MAIN,
}AD_MIX_MODE;

typedef enum
{
    MIX_OFF,
    MIX_ON,
}MIX_ONOFF;

typedef enum
{
    AUDIO_DOLBY_TYPE_DD,
    AUDIO_DOLBY_TYPE_DDP,
    AUDIO_DOLBY_TYPE_ATMOS,
} AUDIO_DOLBY_TYPE;

typedef enum
{
    AUDIO_AC3_TYPE_DD,
    AUDIO_AC3_TYPE_DDP,
    AUDIO_AC3_TYPE_ATMOS,
} AUDIO_AC3_TYPE;

typedef enum
{
    AUDIO_AAC_TYPE_GENERIC_AAC,
    AUDIO_AAC_TYPE_GENERIC_HEAACv1,
    AUDIO_AAC_TYPE_GENERIC_HEAACv2,
    AUDIO_AAC_TYPE_DOLBY_AAC,
    AUDIO_AAC_TYPE_DOLBY_HEAACv1,
    AUDIO_AAC_TYPE_DOLBY_HEAACv2,
} AUDIO_AAC_TYPE;

typedef enum
{
    AUDIO_DTS_TYPE_CORE,
    AUDIO_DTS_TYPE_LBR,
    AUDIO_DTS_TYPE_HD,
} AUDIO_DTS_TYPE;

typedef enum
{
    FILE_SIZE_1KB,
    FILE_SIZE_2KB,
    FILE_SIZE_4KB,
    FILE_SIZE_8KB,
    FILE_SIZE_16KB,
    FILE_SIZE_32KB,
    FILE_SIZE_64KB,
    FILE_SIZE_256,
    FILE_SIZE_512,
    FILE_SIZE_64,
    FILE_SIZE_128
}FILE_SIZE;


typedef enum
{
    AU_DVB_MODE_STEREO,                 ///< 0:STEREO
    AU_DVB_MODE_LL,                     ///< 1: LL
    AU_DVB_MODE_RR,                     ///< 2: RR
    AU_DVB_MODE_MIXED_LR                ///< 3:(L+R)/2
}AU_DVB_MODE;

//  enum for dvb decoder command
typedef enum
{
    AU_DVB_DECCMD_STOP                  = 0,
    AU_DVB_DECCMD_PLAY                  = 1,
    AU_DVB_DECCMD_PLAYFILETSP           = 2,
    AU_DVB_DECCMD_RESYNC                = 3,
    AU_DVB_DECCMD_PLAYFILE              = 4,
    AU_DVB_DECCMD_STARTBROWSE           = 5,
    AU_DVB_DECCMD_PAUSE                 = 6,
    AU_DVB_DECCMD_TSP_FRAMEDECODE       = 7,
    AU_DVB_DECCMD_PLAYFRAME             = 8,            // Uniplayer, Mstreamer for TSP input
    AU_DVB_DECCMD_PLAYFRAME_STAGE       = 9,            // Stagecraft
    AU_DVB_DECCMD_PLAYFRAME_GS          = 10,           // Uniplayer, Gstreamer for ES copy
    AU_DVB_DECCMD_PLAYFILE2_PTS         = 11,
    AU_DVB_DECCMD_PLAY_AD               = 12,
    AU_DVB_DECCMD_STOP_AD               = 13,
    AU_DVB_DECCMD_SKIP                  = 15,
    AU_DVB_DECCMD_AVSYNC                = 0x10,
    AU_DVB_DECCMD_FREERUN               = 0x20,
} AU_DVB_DECCMD;

typedef enum
{
    AU_DVB_ENCCMD_STOP,   // 0
    AU_DVB_ENCCMD_START   // 1
} AU_DVB_ENCCMD;

typedef enum
{
    AU_DVB_ENCBR_192K,                  // 0
    AU_DVB_ENCBR_384K,                  // 1
    AU_DVB_ENCBR_128K,                  // 2
    AU_DVB_ENCBR_64K                    // 3
} AU_DVB_ENCBR;

typedef enum
{
    WMA_PARAMTYPE_VERSION,
    WMA_PARAMTYPE_CHANNELS,
    WMA_PARAMTYPE_SAMPLERATE,
    WMA_PARAMTYPE_BYTERATE,
    WMA_PARAMTYPE_BLOCKALIGN,
    WMA_PARAMTYPE_ENCOPT,
    WMA_PARAMTYPE_PARSINGBYAPP,
    WMA_PARAMTYPE_BITS_PER_SAMPLE,
    WMA_PARAMTYPE_CHANNELMASK,
    WMA_PARAMTYPE_DRC_PARAM_EXIST,
    WMA_PARAMTYPE_DRC_RMS_AMP_REF,
    WMA_PARAMTYPE_DRC_RMS_AMP_TARGET,
    WMA_PARAMTYPE_DRC_PEAK_AMP_REF,
    WMA_PARAMTYPE_DRC_PEAK_AMP_TARGET,
    WMA_PARAMTYPE_MAX_PACKET_SIZE,
    WMA_PARAMTYPE_ADVANCEDENCODEOPT,
    WMA_PARAMTYPE_ADVANCEDENCODEOPT2,
}WMA_ASF_PARMTYPE;

enum
{
    DSP_INIT_FINISH_ACK,
    DSP_READY_FOR_RELOAD_ACK,
    DSP_RELOAD_FINISH_ACK,
    MCU_START_TO_RUN_CMD,
    MCU_START_RELOAD_CMD,
    MCU_RELOAD_FINISH_CMD
};

typedef struct
{
    MS_U8 u8StrCmd[64];
    void *pData;
} AUDIO_COMMON_CMD_t;

//--------------------------Start of SetAudioOutputDeviceSelection Command------------------------

typedef struct ATTRIBUTE_PACKED
{
    MS_U32 u32StructVersion;    //AUDIO_OUTPUT_DEVICE_SELECTION_VERSION
    MS_U32 u32StructSize;    //sizeof(AUDIO_OUTPUT_DEVICE_SELECTION_t)
    MS_U32 u32OutputDeviceSelection;    //AUDIO OUTPUT DEVICE SELECTION
} AUDIO_OUTPUT_DEVICE_SELECTION_t;
#define AUDIO_OUTPUT_DEVICE_SELECTION_VERSION    0x00010001    //[31:16]major version; [15:0]minor version

//--------------------------End of SetAudioOutputDeviceSelection Command-------------------------

//--------------------------Start of SpecifyDigitalOutputCodec Command------------------------

typedef struct ATTRIBUTE_PACKED
{
    MS_U32 u32StructVersion;    //AUDIO_SPECIFIED_DIGITAL_OUTPUT_VERSION
    MS_U32 u32StructSize;   //sizeof(AUDIO_SPECIFIED_DIGITAL_OUTPUT_t)
    MS_U32 u32SpdifOutputCodec;    //SPECIFIED CODEC OF SPDIF
    MS_U32 u32HdmiArcOutputCodec;    //SPECIFIED CODEC OF HDMI ARC
    MS_U32 u32HdmiTxOutputCodec;    //SPECIFIED CODEC OF HDMI TX
} AUDIO_SPECIFIED_DIGITAL_OUTPUT_t;
#define AUDIO_SPECIFIED_DIGITAL_OUTPUT_VERSION    0x00010001    //[31:16]major version; [15:0]minor version

//--------------------------End of SpecifyDigitalOutputCodec Command-------------------------

//--------------------------Start of DapControl Command------------------------

typedef struct ATTRIBUTE_PACKED
{
    MS_U32 u32StructVersion;    //AUDIO_DAP_CONTROL_VERSION
    MS_U32 u32StructSize;   //sizeof(AUDIO_DAP_CONTROL_t)
    MS_U32 u32McuCtrlEnable;    //DAP control by MCU enable
    MS_U32 u32MultiChannelEnable;    //multi-channel input enable
} AUDIO_DAP_CONTROL_t;
#define AUDIO_DAP_CONTROL_VERSION    0x00010001    //[31:16]major version; [15:0]minor version

//--------------------------End of DapControl Command-------------------------
#define AC4_MAX_PRESENTATION_NUM     64
typedef struct
{
    MS_U32 presentation_num;
    MS_U32 presentation_lang[AC4_MAX_PRESENTATION_NUM];
    MS_U8 presentation_type[AC4_MAX_PRESENTATION_NUM];
} UT_ac4_toc_info;

typedef struct ATTRIBUTE_PACKED
{
    MS_U32 u32StructVersion;    //AUDIO_DECODER_INFO_VERSION
    MS_U32 u32StructSize;    //sizeof(AUDIO_DECODER_INFO_t)
    MS_U32 DecId;    //decoder id
    MS_U32 buf_size;
    UT_ac4_toc_info ac4_toc;
} AUDIO_AC4_TOC_t;
#define AUDIO_AC4_TOC_VERSION    0x00010001    //[31:16]major version; [15:0]minor version

//--------------------------Start of GetDecoderInfo Command------------------------

typedef struct ATTRIBUTE_PACKED
{
    MS_U32 u32StructVersion;    //AUDIO_DECODER_INFO_VERSION
    MS_U32 u32StructSize;    //sizeof(AUDIO_DECODER_INFO_t)
    MS_U8 u8DecId;    //decoder id
    MS_BOOL bForceMchOutput;    //to use force multi-channel output mode or not
    MS_U32 u32PcmOutputType;    //the pcm output type
    MS_U32 u32ChannelMapping;   //channel mapping
} AUDIO_DECODER_INFO_t;
#define AUDIO_DECODER_INFO_VERSION    0x00010002    //[31:16]major version; [15:0]minor version

//--------------------------End of GetDecoderInfo Command-------------------------

typedef struct
{
    MS_U32 u32StructVersion;     //AUDIO_CODEC_CAPABILITY_VERSION
    MS_U32 u32StructSize;       //sizeof(AUDIO_CODEC_CAPABILITY_t)
    MS_U32 CodecType;       //use the Define for CodecType in AUDIO_CODEC_CAPABILITY_t
    MS_U32 SupportChannel;
} AUDIO_CODEC_CAPABILITY_t;
#define AUDIO_CODEC_CAPABILITY_VERSION    0x00010001    //[31:16]major version; [15:0]minor version

//================================================================
//  MAD2
//================================================================
enum
{
    DSP_DEC,                            //0
    DSP_SE,                             //1
    DSP_ADV,                              //2
    DSP_MAD                             //3
};

// DTV decoder sample rate definition
typedef enum
{
    AUDIO_FS_NONE,
    AUDIO_FS_8KHZ,
    AUDIO_FS_11KHZ,
    AUDIO_FS_12KHZ,
    AUDIO_FS_16KHZ,
    AUDIO_FS_22KHZ,
    AUDIO_FS_24KHZ,
    AUDIO_FS_32KHZ,
    AUDIO_FS_44KHZ,
    AUDIO_FS_48KHZ,
    AUDIO_FS_64KHZ,
    AUDIO_FS_88KHZ,
    AUDIO_FS_96KHZ,
    AUDIO_FS_176KHZ,
    AUDIO_FS_192KHZ,
    AUDIO_FS_UNSUPPORT                  = 0xFF,
} AUDIO_FS_TYPE;

typedef struct
{
    AUDIO_HDMI_RX_TYPE      audio_type;
    AUDIO_FS_TYPE           sampleRate;
} ST_HDMI_AUDIO_STATUS;
// DTV decoder sound mode type definition
typedef enum
{
    AUD_CH_MODE_DUAL_MONO                       = 0x0,              //MPEG/Dolby
    AUD_CH_MODE_MONO                            = 0x1,              //MPEG/Dolby
    AUD_CH_MODE_STEREO                          = 0x2,              //MPEG/Dolby
    AUD_CH_MODE_3_0_MODE                        = 0x3,              //     Dolby
    AUD_CH_MODE_2_1_MODE                        = 0x4,              //     Dolby
    AUD_CH_MODE_3_1_MODE                        = 0x5,              //     Dolby
    AUD_CH_MODE_2_2_MODE                        = 0x6,              //     Dolby
    AUD_CH_MODE_3_2_MODE                        = 0x7,              //     Dolby
    AUD_CH_MODE_JOINT_STEREO                    = 0x8,              //MPEG/
    AUD_CH_MODE_3_3_MODE                        = 0x9,              //DRA
    AUD_CH_MODE_3_2_2_MODE                      = 0xA,              //Dolby/DRA
    AUD_CH_MODE_3_2_3_MODE                      = 0xB,              //DRA
    AUD_CH_MODE_NONE,
} AUD_CH_MODE_TYPE;

typedef enum
{
    AUD_CH_MAP_C                                = 0x1,
    AUD_CH_MAP_L                                = 0x2,
    AUD_CH_MAP_R                                = 0x4,
    AUD_CH_MAP_LS                               = 0x8,
    AUD_CH_MAP_RS                               = 0x10,
    AUD_CH_MAP_LFE                              = 0x20,
    AUD_CH_MAP_LRS                              = 0x40,
    AUD_CH_MAP_RRS                              = 0x80,
    AUD_CH_MAP_LTS                              = 0x100,
    AUD_CH_MAP_RTS                              = 0x200,
    AUD_CH_MAP_CR                               = 0x400,
    AUD_CH_MAP_CT                               = 0x800,
    AUD_CH_MAP_NONE                             = 0x0,
} AUD_CH_MAP_TYPE;

//================================================================
//  KTV
//================================================================
typedef enum
{
    AUD_KTV_MODEL1,
    AUD_KTV_MODEL2,
    AUD_KTV_MODEL3,
    AUD_KTV_MODEL4,
}AUDIO_KTV_MODEL;

typedef enum
{   /*  Model               AUD_KTV_MODEL1    AUD_KTV_MODEL2    AUD_KTV_MODEL3  *
     *  Chip                T3-iSing99        U3-Multak                         *
     *--------------------------------------------------------------------------*/
    AUD_KTV_infoType1,                  // ScoringCommand
    AUD_KTV_infoType2,                  // EchoLevel
    AUD_KTV_infoType3,                  // EchoDeep
    AUD_KTV_infoType4,                  // MICGain
    AUD_KTV_infoType5,                  // (TBD)
    AUD_KTV_infoType6,                  // (TBD)
    AUD_KTV_infoType7,                  // (TBD)
    AUD_KTV_infoType8,                  // (TBD)
    AUD_KTV_infoType9,                  // (TBD)
    AUD_KTV_infoType10,                 // (TBD)
    AUD_KTV_infoType11,                 // (TBD)
    AUD_KTV_infoType12,                 // (TBD)
    AUD_KTV_infoType13,                 // (TBD)
    AUD_KTV_infoType14,                 // (TBD)
    AUD_KTV_infoType15,                 // (TBD)
    AUD_KTV_infoType16,                 // (TBD)
}AUDIO_KTV_INFOTYPE;

typedef enum
{   /*  Model             AUD_KTV_MODEL1    AUD_KTV_MODEL2    AUD_KTV_MODEL3    *
     *  Chip              T3-iSing99        U3-Multak                           *
     *--------------------------------------------------------------------------*/
    AUD_KTV_ParamType1,                 // AvMICVol
    AUD_KTV_ParamType2,                 // PitchCnt
    AUD_KTV_ParamType3,                 // Pitch
    AUD_KTV_ParamType4,                 // Energy
    AUD_KTV_ParamType5,                 // (TBD)
    AUD_KTV_ParamType6,                 // (TBD)
    AUD_KTV_ParamType7,
    AUD_KTV_ParamType8,
    AUD_KTV_ParamType9,
    AUD_KTV_ParamType10,
    AUD_KTV_ParamType11,
    AUD_KTV_ParamType12,
    AUD_KTV_ParamType13,
    AUD_KTV_ParamType14,
    AUD_KTV_ParamType15,
    AUD_KTV_ParamType16,
}AUDIO_KTV_PARAMTYPE;

typedef enum
{
    PCM_VOL,
    MIC_VOL,
    MP3_VOL,
    GAME1_VOL,
    GAME2_VOL,
    ECHO1_VOL,
    ECHO2_VOL,
}AUDIO_MIX_VOL_TYPE;

///< Threshold table structure
typedef struct
{
    MS_U8  HiByteValue;                 ///< Threshold high-byte
    MS_U8  LowByteValue;                ///< Threshold low-byte
} THR_TBL_TYPE;

//================================================================
//  Sound Enum
//================================================================
// ADC Input Gain Type Definition
typedef enum
{
    AUD_ADC_GAIN_0dB = 0,               //  0dB
    AUD_ADC_GAIN_NEG_3dB,               // -3dB
    AUD_ADC_GAIN_NEG_6dB,               // -6dB
    AUD_ADC_GAIN_3dB,                   //  3dB
    AUD_ADC_GAIN_6dB,                   //  6dB
    AUD_ADC_GAIN_9dB,                   //  9dB
    AUD_ADC_GAIN_12dB,                  // 12dB
    AUD_ADC_GAIN_15dB,                  // 15dB
} AUD_ADC_GAIN_TYPE;

typedef struct
{
    MS_U8  param1;                      ///< param1
    MS_U8  param2;                      ///< param2
    MS_U8  param3;                      ///< param1
    MS_U8  param4;                      ///< param2
} Sound_SET_Param;

typedef struct
{
    MS_U8  AvcThreshold;                ///< AvcThreshold
    MS_U8  AvcMode;                     ///< AvcMode
    MS_U8  AvcAT;                       ///< AvcAT
    MS_U8  AvcRT;                       ///< AvcRT
} Sound_AVC_param;

typedef struct
{
    MS_U8  SurroundXA;                  ///< SurroundXA
    MS_U8  SurroundXB;                  ///< SurroundXB
    MS_U8  SurroundXK;                  ///< SurroundXK
    MS_U8  SurroundLPFGain;             ///< SurroundLPFGain
} Sound_SURR_param;

typedef enum
{
    Sound_SET_Type_SetPreScale,
    Sound_SET_Type_SetMute,
    Sound_SET_Type_SetBalance,
    Sound_SET_Type_EnableEQ,
    Sound_SET_Type_EnablePEQ,
    Sound_SET_Type_EnableTone,
    Sound_SET_Type_EnableAutoVolume,
    Sound_SET_Type_EnableSurround,
    Sound_SET_Type_SetBass,
    Sound_SET_Type_EnableBalance,
    Sound_SET_Type_AbsoluteBass,
    Sound_SET_Type_SetTreble,
    Sound_SET_Type_SetEq,
    Sound_SET_Type_SetEq_7band,
    Sound_SET_Type_SetAvc,
    Sound_SET_Type_SetAudioDelay,
    Sound_SET_Type_SetSurround,
    Sound_SET_Type_ConvertVolumeUnit,
    Sound_SET_Type_SetDCOffet,
    Sound_SET_Type_EnableSineTone,
} Sound_SET_Type;

typedef enum
{
    Sound_SET_PARAM_PreScale,
    Sound_SET_PARAM_Balance,
    Sound_SET_PARAM_EQ,
    Sound_SET_PARAM_Surround_XA,
    Sound_SET_PARAM_Surround_XB,
    Sound_SET_PARAM_Surround_XK,
    Sound_SET_PARAM_Surround_LPFGAIN,
    Sound_SET_PARAM_Treble,
    Sound_SET_PARAM_Bass,
    Sound_SET_PARAM_AbsoluteBass,
    Sound_SET_PARAM_Avc_Mode,
    Sound_SET_PARAM_NR_Threshold,
    Sound_SET_PARAM_Avc_Threshold,
    Sound_SET_PARAM_Avc_AT,
    Sound_SET_PARAM_Avc_RT,
    Sound_SET_PARAM_AudioDelay,
    Sound_SET_PARAM_DCOffet,
    Sound_SET_PARAM_PEQ_48K_A0,
    Sound_SET_PARAM_PEQ_48K_A1,
    Sound_SET_PARAM_PEQ_48K_A2,
    Sound_SET_PARAM_PEQ_48K_B1,
    Sound_SET_PARAM_PEQ_48K_B2,
    Sound_SET_PARAM_PEQ_32K_A0,
    Sound_SET_PARAM_PEQ_32K_A1,
    Sound_SET_PARAM_PEQ_32K_A2,
    Sound_SET_PARAM_PEQ_32K_B1,
    Sound_SET_PARAM_PEQ_32K_B2,
    Sound_SET_PARAM_PEQ_SCALE,
    Sound_SET_PARAM_AbsoluteEQ,
    Sound_SET_PARAM_Drc_Threshold,
    Sound_SET_PARAM_DMAReader,
    Sound_SET_PARAM_AVC_S_Offset,
    Sound_SET_PARAM_Tone_Test_Fs,
    Sound_SET_PARAM_Tone_Test_Gain,
    Sound_SET_PARAM_AbsoluteBalance,
    Sound_SET_PARAM_AbsoluteTreble,
    Sound_SET_PARAM_Ease_Type_A,
    Sound_SET_PARAM_Ease_Duration_A,
    Sound_SET_PARAM_Ease_Gain_A,
    Sound_SET_PARAM_Ease_Target_Channel_A,
    Sound_SET_PARAM_Ease_Type_B,
    Sound_SET_PARAM_Ease_Duration_B,
    Sound_SET_PARAM_Ease_Gain_B,
    Sound_SET_PARAM_Ease_Target_Channel_B,
    Sound_SET_PARAM_Drc_Entry,
    Sound_SET_PARAM_Mixer_AD_Volume,
    Sound_SET_PARAM_Mixer_Main_Volume,
    Sound_SET_PARAM_LG_EQ,
    Sound_SET_PARAM_LG_Balance_L,
    Sound_SET_PARAM_LG_Balance_R,
    Sound_SET_PARAM_VolFadingStep,
    Sound_SET_PARAM_VolFadingCtrl,
    Sound_SET_PARAM_BalanceMask,
    Sound_SET_PARAM_PhaseShiftMask,
    Sound_SET_PARAM_InternalDelay_Ch1,
    Sound_SET_PARAM_InternalDelay_Ch2,
    Sound_SET_PARAM_InternalDelay_Ch3,
    Sound_SET_PARAM_InternalDelay_Ch4,
    Sound_SET_PARAM_InternalDelay_Ch5,
    Sound_SET_PARAM_InternalDelay_Ch6,
    Sound_SET_PARAM_InternalDelay_Ch7,
    Sound_SET_PARAM_InternalDelay_Ch8,
    Sound_SET_PARAM_KTVEcho_XA,
    Sound_SET_PARAM_KTVEcho_XB,
    Sound_SET_PARAM_KTVEcho_ReverbStep,
    Sound_SET_PARAM_Avc_R_Begin,
    Sound_SET_PARAM_Avc_R_End,
    Sound_SET_PARAM_Avc_R_Slope,
    Sound_SET_PARAM_Sinetone_StartFreq,
    Sound_SET_PARAM_Sinetone_EndFreq,
    Sound_SET_PARAM_Sinetone_Duration,
    Sound_SET_PARAM_Sinetone_Mode,
    Sound_SET_PARAM_Sinetone_Ctrl,
    Sound_SET_PARAM_Sinetone_Gain,
    Sound_SET_PARAM_Pcm_Capture_Balence,
    Sound_SET_PARAM_Drc_AT,
    Sound_SET_PARAM_Drc_RT,
    Sound_SET_PARAM_Surround_Phase,
    Sound_SET_PARAM_VoiceEngineRef_PcmSource,
    Sound_SET_PARAM_VoiceEngineRef_PcmSampleRate,
    Sound_SET_PARAM_Limiter_Mode,
    Sound_SET_PARAM_Limiter_Threshold,
    Sound_SET_PARAM_ARCDelay,
} Sound_SET_PARAM_Type;

typedef enum
{
    Sound_GET_PARAM_PreScale,
    Sound_GET_PARAM_Balance_L,
    Sound_GET_PARAM_Balance_R,
    Sound_GET_PARAM_EQ,
    Sound_GET_PARAM_Surround_XA,
    Sound_GET_PARAM_Surround_XB,
    Sound_GET_PARAM_Surround_XK,
    Sound_GET_PARAM_Surround_LPFGAIN,
    Sound_GET_PARAM_Treble,
    Sound_GET_PARAM_Bass,
    Sound_GET_PARAM_Avc_Mode,
    Sound_GET_PARAM_Avc_Threshold,
    Sound_GET_PARAM_Avc_AT,
    Sound_GET_PARAM_Avc_RT,
    Sound_GET_PARAM_AudioDelay,
    Sound_GET_PARAM_DCOffet,
    Sound_GET_PARAM_NR_Threshold,
    Sound_GET_PARAM_NR_Status,
    Sound_GET_PARAM_getSignal_Energy,
    Sound_GET_PARAM_EQ_Status,
    Sound_GET_PARAM_PEQ_Status,
    Sound_GET_PARAM_Tone_Status,
    Sound_GET_PARAM_AutoVolume_Status,
    Sound_GET_PARAM_Surround_Status,
    Sound_GET_PARAM_HPF_Status,
    Sound_GET_PARAM_Drc_Threshold,
    Sound_GET_PARAM_DMAReader_Buffer_Addr,
    Sound_GET_PARAM_ToneTest_LEVEL,
    Sound_GET_PARAM_FPC_Lpeak,
    Sound_GET_PARAM_FPC_Rpeak,
    Sound_GET_PARAM_Ease_Type_A,
    Sound_GET_PARAM_Ease_Duration_A,
    Sound_GET_PARAM_Ease_Gain_A,
    Sound_GET_PARAM_Ease_Type_B,
    Sound_GET_PARAM_Ease_Duration_B,
    Sound_GET_PARAM_Ease_Gain_B,
    Sound_GET_PARAM_InternalDelay_Ch1,
    Sound_GET_PARAM_InternalDelay_Ch2,
    Sound_GET_PARAM_InternalDelay_Ch3,
    Sound_GET_PARAM_InternalDelay_Ch4,
    Sound_GET_PARAM_InternalDelay_Ch5,
    Sound_GET_PARAM_InternalDelay_Ch6,
    Sound_GET_PARAM_InternalDelay_Ch7,
    Sound_GET_PARAM_InternalDelay_Ch8,
    Sound_GET_PARAM_KTVEcho_XA,
    Sound_GET_PARAM_KTVEcho_XB,
    Sound_GET_PARAM_KTVEcho_ReverbStep,
    Sound_GET_PARAM_Avc_R_Begin,
    Sound_GET_PARAM_Avc_R_End,
    Sound_GET_PARAM_Avc_R_Slope,
    Sound_GET_PARAM_Sinetone_StartFreq,
    Sound_GET_PARAM_Sinetone_EndFreq,
    Sound_GET_PARAM_Sinetone_Duration,
    Sound_GET_PARAM_Sinetone_Mode,
    Sound_GET_PARAM_Sinetone_Ctrl,
    Sound_GET_PARAM_Ease_Gain_Max,
    Sound_GET_PARAM_Ease_Gain_Min,
    Sound_GET_PARAM_Sinetone_Gain,
    Sound_GET_PARAM_Drc_AT,
    Sound_GET_PARAM_Drc_RT,
    Sound_GET_PARAM_Surround_Phase,
    Sound_GET_PARAM_ARCDelay,
} Sound_GET_PARAM_Type;

typedef enum
{
//  Sound_ENABL_Type_PreScale,
    Sound_ENABL_Type_EQ,
    Sound_ENABL_Type_PEQ,
    Sound_ENABL_Type_Tone,               //treble/bass
    Sound_ENABL_Type_AutoVolume,
    Sound_ENABL_Type_Surround,
    Sound_ENABL_Type_Balance,
    Sound_ENABL_Type_DRC,
    Sound_ENABL_Type_KTVEcho,
    Sound_ENABL_Type_Hpf,
    Sound_ENABL_Type_FPC,
    Sound_ENABL_Type_DcRemove,
    Sound_ENABL_Type_EaseVol,
    Sound_ENABL_Type_HeadPhoneSoundEffect,
    Sound_ENABL_Type_DRC2,
    Sound_ENABL_Type_SineTone,
    Sound_ENABL_Type_VoiceEngineRef,
    Sound_ENABL_Type_Limiter,
    Sound_ENABL_Type_AllSoundEffect,
} Sound_ENABLE_Type;

typedef enum
{
    AdvSound_SET_Type_VDS_Enable,
    AdvSound_SET_Type_VSPK_Enable,
    AdvSound_SET_Type_VSPK_SetWMod,
    AdvSound_SET_Type_VSPK_SetSMod,
    AdvSound_SET_Type_SRS_Enable,
    AdvSound_SET_Type_SRS_DC,
    AdvSound_SET_Type_SRS_TruBass,
    AdvSound_SET_Type_SRS_SetTsxtPara,
    AdvSound_SET_Type_TSHD_Enable,
    AdvSound_SET_Type_TSHD_TruBass,
    AdvSound_SET_Type_TSHD_Definition,
    AdvSound_SET_Type_TSHD_Clarity,
    AdvSound_SET_Type_TSHD_TruSurr,
    AdvSound_SET_Type_TSHD_Limiter,
    AdvSound_SET_Type_TSHD_SRS3D,
    AdvSound_SET_Type_SRS_SetTshdPara,
    AdvSound_SET_Type_TruVolume_Enable,
    AdvSound_SET_Type_BBE_SetBbeMode,
    AdvSound_SET_CV2_Enable,
    AdvSound_SET_CV2_SetPara,
    AdvSound_SET_CV_SetPara,
    AdvSound_SET_AV_SetPara,
    AdvSound_SET_SetEQPara,
    AdvSound_SET_CV_SetLevel,
    AdvSound_SET_ADSY_SetPara,
} AdvSound_SET_Type;

typedef enum
{
    ADVSOUND_EN_TYPE,
    ADVSOUND_INFO_DV258_ENABLE,
    ADVSOUND_INFO_DV258_LCL_ENABLE,
    ADVSOUND_INFO_DV258_LCL_LEVELER_ENABLE,
    ADVSOUND_INFO_DV258_LCL_MODELER_ENABLE,
    ADVSOUND_INFO_DV258_LCL_MODELER_HALF_MODE,
    ADVSOUND_INFO_DV258_LCL_AMOUNT,
    ADVSOUND_INFO_DV258_LCL_AVM,
    ADVSOUND_INFO_DV258_LCL_DVM,
    ADVSOUND_INFO_DV258_LCL_IRL,
    ADVSOUND_INFO_DV258_LCL_ORL,
    ADVSOUND_INFO_DV258_LCL_PREGAIN,
    ADVSOUND_INFO_DV258_LCL_GAIN,
    ADVSOUND_INFO_DV258_LIMITER_ENABLE,
    ADVSOUND_INFO_DV258_LIMITER_PEAK,
    ADVSOUND_INFO_DV258_LIMITER_BOOST,
    ADVSOUND_INFO_DV258_LIMITER_RMS,
} AdvSound_GET_Type;


//================================================================
//  Advance Sound Enum
//================================================================
typedef enum
{
    ADVSND_INPUT_CHANNEL_2_0 = 0, // 2.0ch
    ADVSND_INPUT_CHANNEL_5_1, // 5.1ch
} ADVSND_INPUT_CHANNEL_MODE;

typedef enum
{
    ADVSND_OUTPUT_CHANNEL_2_0 = 0, // 2.0ch
} ADVSND_OUTPUT_CHANNEL_MODE;

//SRS APOLLO
typedef enum
{
    AU_SRS_APOLLO_EN,
    AU_SRS_TSHD_EN,
    AU_SRS_TRUBASS_FRONT_EN,
    AU_SRS_DEFINITION_FRONT_EN,
    AU_SRS_DIALOG_CLARITY_EN,
    AU_SRS_SURROUND_LEVEL_CTRL,
    AU_SRS_TSHD_INPUT_GAIN,
    AU_SRS_DEFINITION_FRONT_CTRL,
    AU_SRS_DIALOG_CLARITY_CTRL,
    AU_SRS_TRUBASS_FRONT_CTRL,
    AU_SRS_LOW_END_FRONT_SPEAKER_FREQ,
    AU_SRS_HL_EN,
    AU_SRS_HL_LIMIT_LEVEL,
    AU_SRS_HL_BOOST_GAIN,
    AU_SRS_APOLLO_HEADROOM_GAIN,
    AU_SRS_AEQ_PEQ_EN,
    AU_SRS_AEQ_HPF_EN,
    AU_SRS_AEQ_GEQ_EN,
    AU_SRS_VIQ_EN,
    AU_SRS_VIQ_MODE,
    AU_SRS_VIQ_VOL_REF_LEVEL,
    AU_SRS_VIQ_MAX_GAIN,
    AU_SRS_VIQ_NOISE_MNGR_THLD,
    AU_SRS_VIQ_CALIBRATE,
    AU_SRS_VIQ_NOISE_MNGR_EN,
    AU_SRS_AEQ_HPF_FC,
    AU_SRS_VIQ_INPUT_GAIN,
    AU_SRS_VIQ_OUTPUT_GAIN,


    AU_SRS_FTB_LVL_INDPDT_ENABLE,
    AU_SRS_FTB_COMPRESSOR_CONTROL,
    AU_SRS_FTB_MODE,
    AU_SRS_FTB_SPEAKER_AUDIO,
    AU_SRS_FTB_SPEAKER_ANALYSIS,
    AU_SRS_TVOLHD_EN,
    AU_SRS_TVOLHD_OUTPUT_GAIN,
    AU_SRS_TVOLHD_INPUT_GAIN,
    AU_SRS_TVOLHD_BYPASS_GAIN,
    AU_SRS_TVOLHD_MODE,
    AU_SRS_TVOLHD_MAX_GAIN,
    AU_SRS_TVOLHD_NORMALIZER_EN,
    AU_SRS_TVOLHD_NORMTHRESH,
    AU_SRS_TVOLHD_SMOOTH_EN,
    AU_SRS_TVOLHD_LKFS,
    AU_SRS_AEQ_PEQ_INPUT_GAIN,
} AU_SRS_APOLLO_TYPE;


////////////////////////////////////////////////////////
/** Total Sonics modes for DBXTV_SetMode() */
typedef enum _DBXTV_TotSonMode
{
    E_TOTSON_ON        = 0,
    E_TOTSON_OFF,
} EN_DBXTV_TotSonMode;

/** Total Volume modes for DBXTV_SetMode() */
typedef enum _DBXTV_TotVolMode
{
    E_TOTVOL_NORMAL     = 0,
    E_TOTVOL_NIGHT,
    E_TOTVOL_OFF,
} EN_DBXTV_TotVolMode;

/** Total Surround modes for DBXTV_SetMode() */
typedef enum _DBXTV_TotSurMode
{
    E_TOTSUR_ON     = 0,
    E_TOTSUR_OFF,
} EN_DBXTV_TotSurMode;

/** Total Surround modes for HAL_DBXTV_SetOnOff() */
typedef enum _DBXTV_TurnOnOff
{
    E_DBXTV_TurnOff     = 0,
    E_DBXTV_TurnOn,
} EN_DBXTV_TurnOnOff;

////////////////////////////////////////////////////////


typedef enum
{
    AU_SRS_THEATER_EN,
    AU_SRS_THEATER_HPF_EN,
    AU_SRS_THEATER_VIQ_EN,
    AU_SRS_THEATER_CS_EN,
    AU_SRS_THEATER_TRUDIALOG_EN,
    AU_SRS_THEATER_INPUT_GAIN,
    AU_SRS_THEATER_OUTPUT_GAIN,
    AU_SRS_THEATER_BYPASS_GAIN,
    AU_SRS_THEATER_HEADROOM_GAIN,
    AU_SRS_THEATER_INPUT_MODE,
    AU_SRS_THEATER_VIQ_INPUT_GAIN,
    AU_SRS_THEATER_VIQ_OUTPUT_GAIN,
    AU_SRS_THEATER_VIQ_BYPASS_GAIN,
    AU_SRS_THEATER_VIQ_VOLUME_MODE,
    AU_SRS_THEATER_VOLHD_MAXGAIN_CONTROL,
    AU_SRS_THEATER_VOLHD_NORMALIZER_EN,
    AU_SRS_THEATER_VOLHD_NORMTHRESH,
    AU_SRS_THEATER_VOLHD_SMOOTH_EN,
    AU_SRS_THEATER_VOLHD_LKFS,
    AU_SRS_THEATER_CS_INPUT_GAIN,
    AU_SRS_THEATER_CS_PROCESSING_MODE,
    AU_SRS_THEATER_CS_LR_OUTPUT_GAIN,
    AU_SRS_THEATER_CS_LSRS_OUTPUT_GAIN,
    AU_SRS_THEATER_CS_CENTER_OUTPUT_GAIN,
    AU_SRS_THEATER_TRUDIALOG_INPUT_GAIN,
    AU_SRS_THEATER_TRUDIALOG_OUTPUT_GAIN,
    AU_SRS_THEATER_TRUDIALOG_BYPASS_GAIN,
    AU_SRS_THEATER_TRUDIALOG_PROCESS_GAIN,
    AU_SRS_THEATER_TRUDIALOG_CLARITY_GAIN,
    AU_SRS_THEATER_HPF_ORDER,
    AU_SRS_THEATER_HPF_COEFS,
    AU_SRS_THEATER_TSHD_EN,
    AU_SRS_THEATER_TSHD_SURR_EN,
    AU_SRS_THEATER_TSHD_TRUBASS_FRONT_EN,
    AU_SRS_THEATER_TSHD_DEFINITION_FRONT_EN,
    AU_SRS_THEATER_TSHD_DIALOG_CLARITY_EN,
    AU_SRS_THEATER_TSHD_TRUBASS_LEVEL_INDEPENDENT_EN,
    AU_SRS_THEATER_HPF_END_EN,
    AU_SRS_THEATER_HL_EN,
    AU_SRS_THEATER_TSHD_SURROUND_LEVEL_CTRL,
    AU_SRS_THEATER_TSHD_INPUT_GAIN,
    AU_SRS_THEATER_TSHD_OUTPUT_GAIN,
    AU_SRS_THEATER_TSHD_DEFINITION_FRONT_CTRL,
    AU_SRS_THEATER_TSHD_DIALOG_CLARITY_CTRL,
    AU_SRS_THEATER_TSHD_TRUBASS_FRONT_CTRL,
    AU_SRS_THEATER_TSHD_TRUBASS_SPEAKER_SIZE,
    AU_SRS_THEATER_TSHD_TRUBASS_COMPRESSOR_CTRL,
    AU_SRS_THEATER_TSHD_TRUBASS_PROCESS_MODE,
    AU_SRS_THEATER_TSHD_TRUBASS_SPEAKER_AUDIO,
    AU_SRS_THEATER_TSHD_TRUBASS_SPEAKER_ANALYSIS,
    AU_SRS_THEATER_HL_LIMIT_LEVEL_CTRL,
    AU_SRS_THEATER_HL_BOOST_GAIN,
    AU_SRS_THEATER_HPF_CUTOFF,
    AU_SRS_THEATER_HPF_END_CUTOFF,
    AU_SRS_THEATER_CC3D_EN,//param54
    AU_SRS_THEATER_CC3D_DEPTH_PROCESS_EN,//param55
    AU_SRS_THEATER_CC3D_3D_SURR_BOOST_EN,//param56
    AU_SRS_THEATER_CC3D_FADE_EN,//param57
    AU_SRS_THEATER_CC3D_TSHD_MIX_EN,//param58
    AU_SRS_THEATER_CC3D_TBHDX_EN,//param59
    AU_SRS_THEATER_CC3D_INPUT_GAIN,//param60
    AU_SRS_THEATER_CC3D_OUTPUT_GAIN,//param61
    AU_SRS_THEATER_CC3D_BYPASS_GAIN,//param62
    AU_SRS_THEATER_CC3D_APERTURE,//param63
    AU_SRS_THEATER_CC3D_GAINLIMIT,//param64
    AU_SRS_THEATER_CC3D_FF_DEPTH,//param65
    AU_SRS_THEATER_CC3D_NF_DEPTH,//param66

    AU_SRS_THEATER_CC3D_TSHD_SURR_MODE,//param67
    AU_SRS_THEATER_CC3D_TSHD_MIX_FADE_CTRL,//param68

    AU_SRS_THEATER_CC3D_TBHDX_INPUT_GAIN,//param69
    AU_SRS_THEATER_CC3D_TBHDX_BASSLEVEL,//param70
    AU_SRS_THEATER_CC3D_TBHDX_SPEAKERSIZE,//param71
    AU_SRS_THEATER_CC3D_TBHDX_MODE,//param72
    AU_SRS_THEATER_CC3D_TBHDX_DYNAMICS,//param73
    AU_SRS_THEATER_CC3D_TBHDX_HP_ORDER,//param74
    AU_SRS_THEATER_CC3D_TBHDX_CUSTOM_FILTER,
    AU_SRS_THEATER_CC3D_GEQ_EN,//param75
    AU_SRS_THEATER_CC3D_GEQ_INPUT_GAIN,
    AU_SRS_THEATER_CC3D_GEQ_BAND0_GAIN,
    AU_SRS_THEATER_CC3D_GEQ_BAND1_GAIN,
    AU_SRS_THEATER_CC3D_GEQ_BAND2_GAIN,
    AU_SRS_THEATER_CC3D_GEQ_BAND3_GAIN,
    AU_SRS_THEATER_CC3D_GEQ_BAND4_GAIN,
    AU_SRS_THEATER_CC3D_PROCESS_PATH,
    AU_SRS_THEATER_TSHD_BYPASS_GAIN,

    AU_SRS_R2_MAX=100,

    AU_SRS_PURESOUND_HL_EN,//param101
    AU_SRS_PURESOUND_HL_INPUT_GAIN,//param102
    AU_SRS_PURESOUND_HL_OUTPUT_GAIN,//param103
    AU_SRS_PURESOUND_HL_BYPASS_GAIN,//param104
    AU_SRS_PURESOUND_HL_LIMITERBOOST,//param105
    AU_SRS_PURESOUND_HL_HARDLIMIT,//param106
    AU_SRS_PURESOUND_HL_DELAYLEN,//param107

    AU_SRS_PURESOUND_AEQ_EN,//param108
    AU_SRS_PURESOUND_AEQ_INPUT_GAIN,//param109
    AU_SRS_PURESOUND_AEQ_OUTPUT_GAIN,//param110
    AU_SRS_PURESOUND_AEQ_BYPASS_GAIN,//param111
    AU_SRS_PURESOUND_FILTER_CONFIG,//param112

    AU_SRS_PURESOUND_HPF_EN,//param113
    AU_SRS_PURESOUND_HPF_FREQUENCY,//param114
    AU_SRS_PURESOUND_TBHD_EN,
    AU_SRS_PURESOUND_TBHD_TRUBASS_LEVEL,
    AU_SRS_PURESOUND_TBHD_SPEAKER_SIZE,
    AU_SRS_PURESOUND_TBHD_LEVEL_INDEPENDENT_EN,
    AU_SRS_PURESOUND_TBHD_COMPRESSOR_LEVEL,
    AU_SRS_PURESOUND_TBHD_MODE,
    AU_SRS_PURESOUND_TBHD_SPEAKER_AUDIO,
    AU_SRS_PURESOUND_TBHD_SPEAKER_ANALYSIS,

    AU_SRS_PURESOUND_INPUT_GAIN,
    AU_SRS_PURESOUND_OUTPUT_GAIN,
    AU_SRS_PURESOUND_AEQ_COEFFICIENT,

} AU_SRS_THEATERSOUND_TYPE;

#if defined(UFO_PUBLIC_HEADER_500) || defined(UFO_PUBLIC_HEADER_700)

typedef char* ADVSND_TYPE;
#define DOLBY_PL2VDS                                   ((ADVSND_TYPE)"DOLBY_PL2VDS")
#define DOLBY_PL2VDPK                                  ((ADVSND_TYPE)"DOLBY_PL2VDPK")
#define BBE                                            ((ADVSND_TYPE)"BBE")
#define SRS_TSXT                                       ((ADVSND_TYPE)"SRS_TSXT")
#define SRS_TSHD                                       ((ADVSND_TYPE)"SRS_TSHD")
#define SRS_THEATERSOUND                               ((ADVSND_TYPE)"SRS_THEATERSOUND")
#define DTS_ULTRATV                                    ((ADVSND_TYPE)"DTS_ULTRATV")
#define AUDYSSEY                                       ((ADVSND_TYPE)"AUDYSSEY")
#define SUPER_VOICE                                    ((ADVSND_TYPE)"SUPER_VOICE")
#define DBX                                            ((ADVSND_TYPE)"DBX")
#define SRS_PURESND                                    ((ADVSND_TYPE)"SRS_PURESND")
#define DTS_STUDIOSOUND_3D                             ((ADVSND_TYPE)"DTS_STUDIOSOUND_3D")  // DTS_STUDIOSOUND_3D = DTS_STUDIOSOUND_I
#define BONGIOVI_DPS                                   ((ADVSND_TYPE)"BONGIOVI_DPS")
#define SONICMOTION_ABS3D                              ((ADVSND_TYPE)"SONICMOTION_ABS3D")
#define DOLBY_DAP                                      ((ADVSND_TYPE)"DOLBY_DAP")
#define DTS_VIRTUALX                                   ((ADVSND_TYPE)"DTS_VIRTUALX")
#define DTS_STUDIOSOUND_II                             ((ADVSND_TYPE)"DTS_STUDIOSOUND_II")
#define ADV_NONE                                       ((ADVSND_TYPE)"ADV_NONE")

typedef char* ADVFUNC;
//SRS_TSXT
#define SRS_TSXT_TRUBASS                               ((ADVFUNC)"TSXT_TRUBASS")
#define SRS_TSXT_DC                                    ((ADVFUNC)"TSXT_DC")
//SRS_TSHD
#define SRS_TSHD_TRUBASS                               ((ADVFUNC)"TSHD_TRUBASS")
#define SRS_TSHD_DC                                    ((ADVFUNC)"TSHD_DC")
#define SRS_TSHD_DEFINITION                            ((ADVFUNC)"TSHD_DEFINITION")
#define SRS_TSHD_SRS3D                                 ((ADVFUNC)"TSHD_SRS3D")
//SRS_THEATERSOUND
#define SRS_THEATERSOUND_TSHD                          ((ADVFUNC)"THSND_TSHD")
#define SRS_THEATERSOUND_TRUBASS                       ((ADVFUNC)"THSND_TRUBASS")
#define SRS_THEATERSOUND_DC                            ((ADVFUNC)"THSND_DC")
#define SRS_THEATERSOUND_DEFINITION                    ((ADVFUNC)"THSND_DEFINITION")
#define SRS_THEATERSOUND_TRUVOLUME                     ((ADVFUNC)"THSND_TRUVOLUME")
#define SRS_THEATERSOUND_HARDLIMITER                   ((ADVFUNC)"THSND_HARDLIMITER")
#define SRS_THEATERSOUND_HPF                           ((ADVFUNC)"THSND_HPF")
#define SRS_THEATERSOUND_TRUEQ                         ((ADVFUNC)"THSND_TRUEQ")
#define SRS_THEATERSOUND_TRUVOLUME_NOISE_MNGR          ((ADVFUNC)"THSND_TRUVOLUME_NOISE_MNGR")
//DTS_ULTRATV
#define DTS_ULTRATV_EVO                                ((ADVFUNC)"ULTRATV_EVO")
#define DTS_ULTRATV_SYM                                ((ADVFUNC)"ULTRATV_SYM")
//AUDYSSEY
#define AUDYSSEY_DYNAMICVOL                            ((ADVFUNC)"AUDYSSEY_DYNAMICVOL")
#define AUDYSSEY_DYNAMICEQ                             ((ADVFUNC)"AUDYSSEY_DYNAMICEQ")
#define AUDYSSEY_EQ                                    ((ADVFUNC)"AUDYSSEY_EQ")
#define AUDYSSEY_ABX                                   ((ADVFUNC)"AUDYSSEY_ABX")
//SRS_TSHD
#define SRS_THEATERSOUND_TSHD_SURR                     ((ADVFUNC)"THSND_TSHD_SURR")
#define SRS_THEATERSOUND_TSHD_LEVEL_INDPT              ((ADVFUNC)"THSND_TSHD_LEVEL_INDPT")
#define SRS_THEATERSOUND_CS                            ((ADVFUNC)"THSND_CS")
#define SRS_THEATERSOUND_TRUDIALOG                     ((ADVFUNC)"THSND_TRUDIALOG")
#define SRS_THEATERSOUND_TRUVOLUME_NORMALIZER          ((ADVFUNC)"THSND_TRUVOLUME_NORMALIZER")
#define SRS_THEATERSOUND_TRUVOLUME_SMOOTH              ((ADVFUNC)"THSND_TRUVOLUME_SMOOTH")
#define SRS_THEATERSOUND_HPF_END                       ((ADVFUNC)"THSND_HPF_END")
#define SRS_THEATERSOUND_TOTAL                         ((ADVFUNC)"THSND_TOTAL")
// SRS_PURESOUND
#define SRS_PURESOUND_HL                               ((ADVFUNC)"PURESND_HL")
#define SRS_PURESOUND_AEQ                              ((ADVFUNC)"PURESND_AEQ")
#define SRS_PURESOUND_HPF                              ((ADVFUNC)"PURESND_HPF")
#define SRS_PURESOUND_TBHD                             ((ADVFUNC)"PURESND_TBHD")
#define SRS_THEATERSOUND_CC3D_EN                       ((ADVFUNC)"THSND_CC3D_EN")
#define SRS_THEATERSOUND_CC3D_DEPTH_PROCESS_EN         ((ADVFUNC)"THSND_CC3D_DEPTH_PROCESS_EN")
#define SRS_THEATERSOUND_CC3D_3D_SURR_BOOST_EN         ((ADVFUNC)"THSND_CC3D_3D_SURR_BOOST_EN")
#define SRS_THEATERSOUND_CC3D_FADE_EN                  ((ADVFUNC)"THSND_CC3D_FADE_EN")
#define SRS_THEATERSOUND_CC3D_TSHD_MIX_EN              ((ADVFUNC)"THSND_CC3D_TSHD_MIX_EN")
#define SRS_THEATERSOUND_CC3D_TBHDX_EN                 ((ADVFUNC)"THSND_CC3D_TBHDX_EN")
#define SRS_THEATERSOUND_GEQ_EN                        ((ADVFUNC)"THSND_GEQ_EN")
#define SRS_THEATERSOUND_3D_EN                         ((ADVFUNC)"THSND_3D_EN")
// BONGIOVI DPS
#define BONGIOVI_DPS_SURR_EN                           ((ADVFUNC)"DPS_SURR_EN")
// SONICEMOTION_ABS3D
#define SONICEMOTION_ABS3D_EN                          ((ADVFUNC)"ABS3D_EN")
// DOLBY DAP
#define DOLBY_DAP_EN                                   ((ADVFUNC)"DAP_EN")
#define DOLBY_DAP_ATMOS_EN                             ((ADVFUNC)"DAP_ATMOS_EN")
// DTS VIRTUALX
// DTS VIRTUALX: Totally disable DTS VirtualX function.
#define DTS_VIRTUALX_EN                                ((ADVFUNC)"VIRTUALX_EN")
// DTS VIRTUALX: Not totally disable DTS VirtualX function; used to On/Off VirtualX's Sound Effect (TRUSRNDX, TBHDX, MBHL), headromm gain and downmixing keep working.
#define DTS_VIRTUALX_SOUNDEFFECT_EN                    ((ADVFUNC)"VIRTUALX_SOUNDEFFECT_EN")
// DTS VIRTUALX: On/Off TruSurroundX(TRUSRNDX)
#define DTS_VIRTUALX_SOUNDEFFECT_TRUSRNDX_EN           ((ADVFUNC)"VIRTUALX_SOUNDEFFECT_TRUSRNDX_EN")
// DTS VIRTUALX: On/Off TruBassHDX (TBHDX)
#define DTS_VIRTUALX_SOUNDEFFECT_TBHDX_EN              ((ADVFUNC)"VIRTUALX_SOUNDEFFECT_TBHDX_EN")
// DTS VIRTUALX: On/Off MultiBand Hard Limilter (MBHL)
#define DTS_VIRTUALX_SOUNDEFFECT_MBHL_EN               ((ADVFUNC)"VIRTUALX_SOUNDEFFECT_MBHL_EN")
// DTS TRUVOLUMEHD
#define DTS_TRUVOLUMEHD_EN                             ((ADVFUNC)"DTS_TRUVOLUMEHD_EN")
// DTS STUDIOSOUND II
#define DTS_STUDIOSOUND_II_EN                          ((ADVFUNC)"DTS_STUDIOSOUND_II_EN")

typedef char* ADVSND_PARAM;
//BBE
#define BBE_MODE_SET                                  ((ADVSND_PARAM)"BBE_MODE_SET")
// DOLBY
#define DOLBY_PL2VDPK_SMOD                            ((ADVSND_PARAM)"PL2VDPK_SMOD")
#define DOLBY_PL2VDPK_WMOD                            ((ADVSND_PARAM)"PL2VDPK_WMOD")
//SRS TSXT
#define SRS_TSXT_SET_INPUT_GAIN                       ((ADVSND_PARAM)"TSXT_SET_INPUT_GAIN")
#define SRS_TSXT_SET_DC_GAIN                          ((ADVSND_PARAM)"TSXT_SET_DC_GAIN")
#define SRS_TSXT_SET_TRUBASS_GAIN                     ((ADVSND_PARAM)"TSXT_SET_TRUBASS_GAIN")
#define SRS_TSXT_SET_SPEAKERSIZE                      ((ADVSND_PARAM)"TSXT_SET_SPEAKERSIZE")
#define SRS_TSXT_SET_INPUT_MODE                       ((ADVSND_PARAM)"TSXT_SET_INPUT_MODE")
#define SRS_TSXT_SET_OUTPUT_GAIN                      ((ADVSND_PARAM)"TSXT_SET_OUTPUT_GAIN")

//SRS TSHD in SE-DSP
#define SRS_TSHD_SET_INPUT_MODE                       ((ADVSND_PARAM)"TSHD_SET_INPUT_MODE")
#define SRS_TSHD_SET_OUTPUT_MODE                      ((ADVSND_PARAM)"TSHD_SET_OUTPUT_MODE")
#define SRS_TSHD_SET_SPEAKERSIZE                      ((ADVSND_PARAM)"TSHD_SET_SPEAKERSIZE")
#define SRS_TSHD_SET_TRUBASS_CONTROL                  ((ADVSND_PARAM)"TSHD_SET_TRUBASS_CONTROL")
#define SRS_TSHD_SET_DEFINITION_CONTROL               ((ADVSND_PARAM)"TSHD_SET_DEFINITION_CONTROL")
#define SRS_TSHD_SET_DC_CONTROL                       ((ADVSND_PARAM)"TSHD_SET_DC_CONTROL")
#define SRS_TSHD_SET_SURROUND_LEVEL                   ((ADVSND_PARAM)"TSHD_SET_SURROUND_LEVEL")
#define SRS_TSHD_SET_INPUT_GAIN                       ((ADVSND_PARAM)"TSHD_SET_INPUT_GAIN")
#define SRS_TSHD_SET_WOWSPACE_CONTROL                 ((ADVSND_PARAM)"TSHD_SET_WOWSPACE_CONTROL")
#define SRS_TSHD_SET_WOWCENTER_CONTROL                ((ADVSND_PARAM)"TSHD_SET_WOWCENTER_CONTROL")
#define SRS_TSHD_SET_WOWHDSRS3DMODE                   ((ADVSND_PARAM)"TSHD_SET_WOWHDSRS3DMODE")
#define SRS_TSHD_SET_LIMITERCONTROL                   ((ADVSND_PARAM)"TSHD_SET_LIMITERCONTROL")
#define SRS_TSHD_SET_OUTPUT_GAIN                      ((ADVSND_PARAM)"TSHD_SET_OUTPUT_GAIN")
// SRS_THEATERSOUND
#define SRS_THEATERSOUND_INPUT_GAIN                   ((ADVSND_PARAM)"THSND_INPUT_GAIN")
#define SRS_THEATERSOUND_DEFINITION_CONTROL           ((ADVSND_PARAM)"THSND_DEFINITION_CONTROL")
#define SRS_THEATERSOUND_DC_CONTROL                   ((ADVSND_PARAM)"THSND_DC_CONTROL")
#define SRS_THEATERSOUND_TRUBASS_CONTROL              ((ADVSND_PARAM)"THSND_TRUBASS_CONTROL")
#define SRS_THEATERSOUND_SPEAKERSIZE                  ((ADVSND_PARAM)"THSND_SPEAKERSIZE")
#define SRS_THEATERSOUND_HARDLIMITER_LEVEL            ((ADVSND_PARAM)"THSND_HARDLIMITER_LEVEL")
#define SRS_THEATERSOUND_HARDLIMITER_BOOST_GAIN       ((ADVSND_PARAM)"THSND_HARDLIMITER_BOOST_GAIN")
#define SRS_THEATERSOUND_HEADROOM_GAIN                ((ADVSND_PARAM)"THSND_HEADROOM_GAIN")
#define SRS_THEATERSOUND_TRUVOLUME_MODE               ((ADVSND_PARAM)"THSND_TRUVOLUME_MODE")

#define SRS_THEATERSOUND_TRUVOLUME_REF_LEVEL          ((ADVSND_PARAM)"THSND_TRUVOLUME_REF_LEVEL")
#define SRS_THEATERSOUND_TRUVOLUME_MAX_GAIN           ((ADVSND_PARAM)"THSND_TRUVOLUME_MAX_GAIN")
#define SRS_THEATERSOUND_TRUVOLUME_NOISE_MNGR_THLD    ((ADVSND_PARAM)"THSND_TRUVOLUME_NOISE_MNGR_THLD")
#define SRS_THEATERSOUND_TRUVOLUME_CALIBRATE          ((ADVSND_PARAM)"THSND_TRUVOLUME_CALIBRATE")
#define SRS_THEATERSOUND_TRUVOLUME_INPUT_GAIN         ((ADVSND_PARAM)"THSND_TRUVOLUME_INPUT_GAIN")
#define SRS_THEATERSOUND_TRUVOLUME_OUTPUT_GAIN        ((ADVSND_PARAM)"THSND_TRUVOLUME_OUTPUT_GAIN")
#define SRS_THEATERSOUND_HPF_FC                       ((ADVSND_PARAM)"THSND_HPF_FC")
#define SRS_THEATERSOUND_TSHD_INPUT_GAIN              ((ADVSND_PARAM)"THSND_TSHD_INPUT_GAIN")
#define SRS_THEATERSOUND_TSHD_OUTPUT_GAIN             ((ADVSND_PARAM)"THSND_TSHD_OUTPUT_GAIN")
#define SRS_THEATERSOUND_SURR_LEVEL_CONTROL           ((ADVSND_PARAM)"THSND_SURR_LEVEL_CONTROL")
#define SRS_THEATERSOUND_TRUBASS_COMPRESSOR_CONTROL   ((ADVSND_PARAM)"THSND_TRUBASS_COMPRESSOR_CONTROL")
#define SRS_THEATERSOUND_TRUBASS_PROCESS_MODE         ((ADVSND_PARAM)"THSND_TRUBASS_PROCESS_MODE")
#define SRS_THEATERSOUND_TRUBASS_SPEAKER_AUDIO        ((ADVSND_PARAM)"THSND_TRUBASS_SPEAKER_AUDIO")
#define SRS_THEATERSOUND_TRUBASS_SPEAKER_ANALYSIS     ((ADVSND_PARAM)"THSND_TRUBASS_SPEAKER_ANALYSIS")
#define SRS_THEATERSOUND_OUTPUT_GAIN                  ((ADVSND_PARAM)"THSND_OUTPUT_GAIN")
#define SRS_THEATERSOUND_BYPASS_GAIN                  ((ADVSND_PARAM)"THSND_BYPASS_GAIN")
#define SRS_THEATERSOUND_INPUT_MODE                   ((ADVSND_PARAM)"THSND_INPUT_MODE")
#define SRS_THEATERSOUND_TRUVOLUME_BYPASS_GAIN        ((ADVSND_PARAM)"THSND_TRUVOLUME_BYPASS_GAIN")
#define SRS_THEATERSOUND_CS_INPUT_GAIN                ((ADVSND_PARAM)"THSND_CS_INPUT_GAIN")

// DTS_ULTRATV
#define DTS_ULTRATV_EVO_MONOINPUT                     ((ADVSND_PARAM)"ULTRATV_EVO_MONOINPUT")
#define DTS_ULTRATV_EVO_WIDENINGON                    ((ADVSND_PARAM)"ULTRATV_EVO_WIDENINGON")
#define DTS_ULTRATV_EVO_ADD3DBON                      ((ADVSND_PARAM)"ULTRATV_EVO_ADD3DBON")
#define DTS_ULTRATV_EVO_PCELEVEL                      ((ADVSND_PARAM)"ULTRATV_EVO_PCELEVEL")
#define DTS_ULTRATV_EVO_VLFELEVEL                     ((ADVSND_PARAM)"ULTRATV_EVO_VLFELEVEL")
#define DTS_ULTRATV_SYM_DEFAULT                       ((ADVSND_PARAM)"ULTRATV_SYM_DEFAULT")
#define DTS_ULTRATV_SYM_MODE                          ((ADVSND_PARAM)"ULTRATV_SYM_MODE")
#define DTS_ULTRATV_SYM_LEVEL                         ((ADVSND_PARAM)"ULTRATV_SYM_LEVEL")
#define DTS_ULTRATV_SYM_RESET                         ((ADVSND_PARAM)"ULTRATV_SYM_RESET")
// AUDYSSEY
#define AUDYSSEY_DYNAMICVOL_COMPRESS_MODE             ((ADVSND_PARAM)"AUDYSSEY_DYNAMICVOL_COMPRESS_MODE")
#define AUDYSSEY_DYNAMICVOL_GC                        ((ADVSND_PARAM)"AUDYSSEY_DYNAMICVOL_GC")
#define AUDYSSEY_DYNAMICVOL_VOLSETTING                ((ADVSND_PARAM)"AUDYSSEY_DYNAMICVOL_VOLSETTING")
#define AUDYSSEY_DYNAMICEQ_EQOFFSET                   ((ADVSND_PARAM)"AUDYSSEY_DYNAMICEQ_EQOFFSET")
#define AUDYSSEY_ABX_GWET                             ((ADVSND_PARAM)"AUDYSSEY_ABX_GWET")
#define AUDYSSEY_ABX_GDRY                             ((ADVSND_PARAM)"AUDYSSEY_ABX_GDRY")
#define AUDYSSEY_ABX_FILSET                           ((ADVSND_PARAM)"AUDYSSEY_ABX_FILSET")
// DBX
#define DBX_MODE_SET                                  ((ADVSND_PARAM)"DBX_MODE_SET")
#define DBX_LOAD_PM                                   ((ADVSND_PARAM)"DBX_LOAD_PM")
#define DBX_LOAD_DM                                   ((ADVSND_PARAM)"DBX_LOAD_DM")
#define DBX_COPY_TotSonDM                             ((ADVSND_PARAM)"DBX_COPY_TotSonDM")
#define DBX_COPY_TotSonPM                             ((ADVSND_PARAM)"DBX_COPY_TotSonPM")
#define DBX_COPY_TotVolDM                             ((ADVSND_PARAM)"DBX_COPY_TotVolDM")
#define DBX_COPY_TotVolPM                             ((ADVSND_PARAM)"DBX_COPY_TotVolPM")
#define DBX_COPY_TotSurDM                             ((ADVSND_PARAM)"DBX_COPY_TotSurDM")
#define DBX_COPY_TotSurPM                             ((ADVSND_PARAM)"DBX_COPY_TotSurPM")
// SRS THEATERSOUND
#define SRS_THEATERSOUND_CS_PROCESS_MODE              ((ADVSND_PARAM)"THSND_CS_PROCESS_MODE")
#define SRS_THEATERSOUND_CS_LR_OUTPUT_GAIN            ((ADVSND_PARAM)"THSND_CS_LR_OUTPUT_GAIN")
#define SRS_THEATERSOUND_CS_LsRs_OUTPUT_GAIN          ((ADVSND_PARAM)"THSND_CS_LsRs_OUTPUT_GAIN")
#define SRS_THEATERSOUND_CS_Center_OUTPUT_GAIN        ((ADVSND_PARAM)"THSND_CS_Center_OUTPUT_GAIN")
#define SRS_THEATERSOUND_TRUDIALOG_INPUT_GAIN         ((ADVSND_PARAM)"THSND_TRUDIALOG_INPUT_GAIN")
#define SRS_THEATERSOUND_TRUDIALOG_OUTPUT_GAIN        ((ADVSND_PARAM)"THSND_TRUDIALOG_OUTPUT_GAIN")
#define SRS_THEATERSOUND_TRUDIALOG_BYPASS_GAIN        ((ADVSND_PARAM)"THSND_TRUDIALOG_BYPASS_GAIN")
#define SRS_THEATERSOUND_TRUDIALOG_PROCESS_GAIN       ((ADVSND_PARAM)"THSND_TRUDIALOG_PROCESS_GAIN")
#define SRS_THEATERSOUND_TRUDIALOG_CLARITY_GAIN       ((ADVSND_PARAM)"THSND_TRUDIALOG_CLARITY_GAIN")
#define SRS_THEATERSOUND_TRUVOLUME_NORMALIZE_THRESH   ((ADVSND_PARAM)"THSND_TRUVOLUME_NORMALIZE_THRESH")
#define SRS_THEATERSOUND_HPF_CUTOFF                   ((ADVSND_PARAM)"THSND_HPF_CUTOFF")
#define SRS_THEATERSOUND_HPF_END_CUTOFF               ((ADVSND_PARAM)"THSND_HPF_END_CUTOFF")
// SRS PURESOUND
#define SRS_PURESOUND_HL_INPUT_GAIN                   ((ADVSND_PARAM)"PURESND_HL_INPUT_GAIN")
#define SRS_PURESOUND_HL_OUTPUT_GAIN                  ((ADVSND_PARAM)"PURESND_HL_OUTPUT_GAIN")
#define SRS_PURESOUND_HL_BYPASS_GAIN                  ((ADVSND_PARAM)"PURESND_HL_BYPASS_GAIN")
#define SRS_PURESOUND_HL_LIMITERBOOST                 ((ADVSND_PARAM)"PURESND_HL_LIMITERBOOST")
#define SRS_PURESOUND_HL_HARDLIMIT                    ((ADVSND_PARAM)"PURESND_HL_HARDLIMIT")
#define SRS_PURESOUND_HL_DELAYLEN                     ((ADVSND_PARAM)"PURESND_HL_DELAYLEN")
#define SRS_PURESOUND_AEQ_INPUT_GAIN                  ((ADVSND_PARAM)"PURESND_AEQ_INPUT_GAIN")
#define SRS_PURESOUND_AEQ_OUTPUT_GAIN                 ((ADVSND_PARAM)"PURESND_AEQ_OUTPUT_GAIN")
#define SRS_PURESOUND_AEQ_BYPASS_GAIN                 ((ADVSND_PARAM)"PURESND_AEQ_BYPASS_GAIN")
#define SRS_PURESOUND_FILTER_CONFIG                   ((ADVSND_PARAM)"PURESND_FILTER_CONFIG")
#define SRS_PURESOUND_AEQ_COEFFICIENT                 ((ADVSND_PARAM)"PURESND_AEQ_COEFFICIENT")
#define SRS_PURESOUND_HPF_FREQUENCY                   ((ADVSND_PARAM)"PURESND_HPF_FREQUENCY")
#define SRS_PURESOUND_TBHD_TRUBASS_LEVEL              ((ADVSND_PARAM)"PURESND_TBHD_TRUBASS_LEVEL")
#define SRS_PURESOUND_TBHD_SPEAKER_SIZE               ((ADVSND_PARAM)"PURESND_TBHD_SPEAKER_SIZE")
#define SRS_PURESOUND_TBHD_LEVEL_INDEPENDENT_EN       ((ADVSND_PARAM)"PURESND_TBHD_LEVEL_INDEPENDENT_EN")
#define SRS_PURESOUND_TBHD_COMPRESSOR_LEVEL           ((ADVSND_PARAM)"PURESND_TBHD_COMPRESSOR_LEVEL")
#define SRS_PURESOUND_TBHD_MODE                       ((ADVSND_PARAM)"PURESND_TBHD_MODE")
#define SRS_PURESOUND_TBHD_SPEAKER_AUDIO              ((ADVSND_PARAM)"PURESND_TBHD_SPEAKER_AUDIO")
#define SRS_PURESOUND_TBHD_SPEAKER_ANALYSIS           ((ADVSND_PARAM)"PURESND_TBHD_SPEAKER_ANALYSIS")
#define SRS_PURESOUND_INPUT_GAIN                      ((ADVSND_PARAM)"PURESND_INPUT_GAIN")
#define SRS_PURESOUND_OUTPUT_GAIN                     ((ADVSND_PARAM)"PURESND_OUTPUT_GAIN")
//SRS_THEATERSOUND_3D
#define SRS_THEATERSOUND_CC3D_INPUT_GAIN              ((ADVSND_PARAM)"THSND_CC3D_INPUT_GAIN")
#define SRS_THEATERSOUND_CC3D_OUTPUT_GAIN             ((ADVSND_PARAM)"THSND_CC3D_OUTPUT_GAIN")
#define SRS_THEATERSOUND_CC3D_BYPASS_GAIN             ((ADVSND_PARAM)"THSND_CC3D_BYPASS_GAIN")
#define SRS_THEATERSOUND_CC3D_APERTURE                ((ADVSND_PARAM)"THSND_CC3D_APERTURE")
#define SRS_THEATERSOUND_CC3D_GAINLIMIT               ((ADVSND_PARAM)"THSND_CC3D_GAINLIMIT")
#define SRS_THEATERSOUND_CC3D_FF_DEPTH                ((ADVSND_PARAM)"THSND_CC3D_FF_DEPTH")
#define SRS_THEATERSOUND_CC3D_NF_DEPTH                ((ADVSND_PARAM)"THSND_CC3D_NF_DEPTH")
#define SRS_THEATERSOUND_CC3D_TSHD_SURR_MODE          ((ADVSND_PARAM)"THSND_CC3D_TSHD_SURR_MODE")
#define SRS_THEATERSOUND_CC3D_TSHD_MIX_FADE_CTRL      ((ADVSND_PARAM)"THSND_CC3D_TSHD_MIX_FADE_CTRL")
#define SRS_THEATERSOUND_CC3D_TBHDX_INPUT_GAIN        ((ADVSND_PARAM)"THSND_CC3D_TBHDX_INPUT_GAIN")
#define SRS_THEATERSOUND_CC3D_TBHDX_BASSLEVEL         ((ADVSND_PARAM)"THSND_CC3D_TBHDX_BASSLEVEL")
#define SRS_THEATERSOUND_CC3D_TBHDX_SPEAKERSIZE       ((ADVSND_PARAM)"THSND_CC3D_TBHDX_SPEAKERSIZE")
#define SRS_THEATERSOUND_CC3D_TBHDX_MODE              ((ADVSND_PARAM)"THSND_CC3D_TBHDX_MODE")
#define SRS_THEATERSOUND_CC3D_TBHDX_DYNAMICS          ((ADVSND_PARAM)"THSND_CC3D_TBHDX_DYNAMICS")
#define SRS_THEATERSOUND_CC3D_TBHDX_HP_ORDER          ((ADVSND_PARAM)"THSND_CC3D_TBHDX_HP_ORDER")
#define SRS_THEATERSOUND_CC3D_TBHDX_CUSTOM_FILTER     ((ADVSND_PARAM)"THSND_CC3D_TBHDX_CUSTOM_FILTER")
#define SRS_THEATERSOUND_GEQ_INPUT_GAIN               ((ADVSND_PARAM)"THSND_GEQ_INPUT_GAIN")
#define SRS_THEATERSOUND_GEQ_BAND0_GAIN               ((ADVSND_PARAM)"THSND_GEQ_BAND0_GAIN")
#define SRS_THEATERSOUND_GEQ_BAND1_GAIN               ((ADVSND_PARAM)"THSND_GEQ_BAND1_GAIN")
#define SRS_THEATERSOUND_GEQ_BAND2_GAIN               ((ADVSND_PARAM)"THSND_GEQ_BAND2_GAIN")
#define SRS_THEATERSOUND_GEQ_BAND3_GAIN               ((ADVSND_PARAM)"THSND_GEQ_BAND3_GAIN")
#define SRS_THEATERSOUND_GEQ_BAND4_GAIN               ((ADVSND_PARAM)"THSND_GEQ_BAND4_GAIN")
#define SRS_THEATERSOUND_CC3D_PROCESS_PATH            ((ADVSND_PARAM)"THSND_CC3D_PROCESS_PATH")
#define SRS_THEATERSOUND_TSHD_BYPASS_GAIN             ((ADVSND_PARAM)"THSND_TSHD_BYPASS_GAIN")
// BONGIOVI_DPS
#define BONGIOVI_DPS_SET_PARAMS                       ((ADVSND_PARAM)"DPS_SET_PARAMS")
#define BONGIOVI_DPS_SET_GLOBAL_INPUT_GAIN            ((ADVSND_PARAM)"DPS_SET_GLOBAL_INPUT_GAIN")
#define BONGIOVI_DPS_SET_GLOBAL_OUTPUT_GAIN           ((ADVSND_PARAM)"DPS_SET_GLOBAL_OUTPUT_GAIN")
#define BONGIOVI_DPS_SET_STEREO_WIDENER_MGAIN         ((ADVSND_PARAM)"DPS_SET_STEREO_WIDENER_MGAIN")
#define BONGIOVI_DPS_SET_STEREO_WIDENER_SGAIN         ((ADVSND_PARAM)"DPS_SET_STEREO_WIDENER_SGAIN")
// SONICEMOTION ABS3D
#define SONICEMOTION_ABS3D_SET_PARAMS                 ((ADVSND_PARAM)"ABS3D_SET_PARAMS")
// DOLBY DAP
#define DOLBY_DAP_SET_PARAMS                          ((ADVSND_PARAM)"DAP_SET_PARAMS")
// DTS VIRAUTLX
#define DTS_VIRTUALX_SET_PARAMS                       ((ADVSND_PARAM)"VIRTUALX_SET_PARAMS")  // To be removed, replaced by DTS_VIRTUALX_SET_API_PARAMS.
#define DTS_VIRTUALX_GET_PARAMS                       ((ADVSND_PARAM)"VIRTUALX_GET_PARAMS")  // To be removed, replaced by DTS_VIRTUALX_GET_API_PARAMS.
#define DTS_VIRTUALX_SET_INPUT_CHANNEL_MODE           ((ADVSND_PARAM)"VIRTUALX_SET_INPUT_CHANNEL_MODE")
#define DTS_VIRTUALX_SET_API_PARAMS                   ((ADVSND_PARAM)"DTS_VIRTUALX_SET_API_PARAMS")
#define DTS_VIRTUALX_GET_API_PARAMS                   ((ADVSND_PARAM)"DTS_VIRTUALX_GET_API_PARAMS")
#define DTS_VIRTUALX_SET_APP_PARAMS                   ((ADVSND_PARAM)"DTS_VIRTUALX_SET_APP_PARAMS")
#define DTS_VIRTUALX_GET_APP_PARAMS                   ((ADVSND_PARAM)"DTS_VIRTUALX_GET_APP_PARAMS")
#define DTS_VIRTUALX_SET_SAMPLERATE                   ((ADVSND_PARAM)"DTS_VIRTUALX_SET_SAMPLERATE")
#define DTS_VIRTUALX_SET_PARAMS_BY_CLI                ((ADVSND_PARAM)"DTS_VIRTUALX_SET_PARAMS_BY_CLI")
// DTS TRUVOLUMEHD
#define DTS_TRUVOLUMEHD_SET_INPUT_CHANNEL_MODE        ((ADVSND_PARAM)"DTS_TRUVOLUMEHD_SET_INPUT_CHANNEL_MODE")
#define DTS_TRUVOLUMEHD_SET_API_PARAMS                ((ADVSND_PARAM)"DTS_TRUVOLUMEHD_SET_API_PARAMS")
#define DTS_TRUVOLUMEHD_GET_API_PARAMS                ((ADVSND_PARAM)"DTS_TRUVOLUMEHD_GET_API_PARAMS")
#define DTS_TRUVOLUMEHD_SET_SAMPLERATE                ((ADVSND_PARAM)"DTS_TRUVOLUMEHD_SET_SAMPLERATE")
#define DTS_TRUVOLUMEHD_SET_PARAMS_BY_CLI             ((ADVSND_PARAM)"DTS_TRUVOLUMEHD_SET_PARAMS_BY_CLI")
//DBXTV
#define DBXTV_SET_MODE                                ((ADVSND_PARAM)"DBXTV_SET_MODE")

//DV258 (Dolby Volume 258)
#define DV258_ENABLE                                  ((ADVSND_PARAM)"DV258_ENABLE")
#define DV258_LCL_ENABLE                              ((ADVSND_PARAM)"DV258_LCL_ENABLE")
#define DV258_LCL_LEVELER_ENABLE                      ((ADVSND_PARAM)"DV258_LCL_LEVELER_ENABLE")
#define DV258_LCL_MODELER_ENABLE                      ((ADVSND_PARAM)"DV258_LCL_MODELER_ENABLE")
#define DV258_LCL_MODELER_HALF_MODE                   ((ADVSND_PARAM)"DV258_LCL_MODELER_HALF_MODE")
#define DV258_LCL_AMOUNT                              ((ADVSND_PARAM)"DV258_LCL_AMOUNT")
#define DV258_LCL_AVM                                 ((ADVSND_PARAM)"DV258_LCL_AVM")
#define DV258_LCL_DVM                                 ((ADVSND_PARAM)"DV258_LCL_DVM")
#define DV258_LCL_IRL                                 ((ADVSND_PARAM)"DV258_LCL_IRL")
#define DV258_LCL_ORL                                 ((ADVSND_PARAM)"DV258_LCL_ORL")
#define DV258_LCL_PREGAIN                             ((ADVSND_PARAM)"DV258_LCL_PREGAIN")
#define DV258_LCL_GAIN                                ((ADVSND_PARAM)"DV258_LCL_GAIN")
#define DV258_LIMITER_ENABLE                          ((ADVSND_PARAM)"DV258_LIMITER_ENABLE")
#define DV258_LIMITER_PEAK                            ((ADVSND_PARAM)"DV258_LIMITER_PEAK")
#define DV258_LIMITER_BOOST                           ((ADVSND_PARAM)"DV258_LIMITER_BOOST")
#define DV258_LIMITER_RMS                             ((ADVSND_PARAM)"DV258_LIMITER_RMS")

// THESE ARE DUMMY DEFINITIONS WHICH ARE DEFINED TO PREVENT SN COMPILE ERROR AND THESE WOULD BE DELETED IN THE FUTURE.
#define ADVSND_PARAM_DUMMY   "ADVSND_PARAM_DUMMY"
#define SRS_TSXT_RESERVE1                             ((ADVSND_PARAM)ADVSND_PARAM_DUMMY)
#define SRS_TSXT_RESERVE2                             ((ADVSND_PARAM)ADVSND_PARAM_DUMMY)
#define SRS_TSXT_RESERVE3                             ((ADVSND_PARAM)ADVSND_PARAM_DUMMY)
#define SRS_TSHD_RESERVE1                             ((ADVSND_PARAM)ADVSND_PARAM_DUMMY)
#define SRS_TSHD_RESERVE2                             ((ADVSND_PARAM)ADVSND_PARAM_DUMMY)
#define SRS_TSHD_RESERVE3                             ((ADVSND_PARAM)ADVSND_PARAM_DUMMY)
#define DTS_ULTRATV_RESERVE1                          ((ADVSND_PARAM)ADVSND_PARAM_DUMMY)
#define DTS_ULTRATV_RESERVE2                          ((ADVSND_PARAM)ADVSND_PARAM_DUMMY)
#define DTS_ULTRATV_RESERVE3                          ((ADVSND_PARAM)ADVSND_PARAM_DUMMY)
#define DTS_ULTRATV_RESERVE4                          ((ADVSND_PARAM)ADVSND_PARAM_DUMMY)
#define DTS_ULTRATV_RESERVE5                          ((ADVSND_PARAM)ADVSND_PARAM_DUMMY)
#define DTS_ULTRATV_RESERVE6                          ((ADVSND_PARAM)ADVSND_PARAM_DUMMY)
#define DTS_ULTRATV_RESERVE7                          ((ADVSND_PARAM)ADVSND_PARAM_DUMMY)
#define DTS_ULTRATV_RESERVE8                          ((ADVSND_PARAM)ADVSND_PARAM_DUMMY)
#define DTS_ULTRATV_RESERVE9                          ((ADVSND_PARAM)ADVSND_PARAM_DUMMY)
#define AUDYSSEY_RESERVE1                             ((ADVSND_PARAM)ADVSND_PARAM_DUMMY)
#define AUDYSSEY_RESERVE2                             ((ADVSND_PARAM)ADVSND_PARAM_DUMMY)
#define AUDYSSEY_RESERVE3                             ((ADVSND_PARAM)ADVSND_PARAM_DUMMY)
#define AUDYSSEY_RESERVE4                             ((ADVSND_PARAM)ADVSND_PARAM_DUMMY)
#define AUDYSSEY_RESERVE5                             ((ADVSND_PARAM)ADVSND_PARAM_DUMMY)
#define AUDYSSEY_RESERVE6                             ((ADVSND_PARAM)ADVSND_PARAM_DUMMY)
#define OTHERS_RESERVE9                               ((ADVSND_PARAM)ADVSND_PARAM_DUMMY)
#define OTHERS_RESERVE10                              ((ADVSND_PARAM)ADVSND_PARAM_DUMMY)
#define OTHERS_RESERVE11                              ((ADVSND_PARAM)ADVSND_PARAM_DUMMY)
#define OTHERS_RESERVE12                              ((ADVSND_PARAM)ADVSND_PARAM_DUMMY)
#define OTHERS_RESERVE13                              ((ADVSND_PARAM)ADVSND_PARAM_DUMMY)
#define OTHERS_RESERVE14                              ((ADVSND_PARAM)ADVSND_PARAM_DUMMY)
#define OTHERS_RESERVE15                              ((ADVSND_PARAM)ADVSND_PARAM_DUMMY)
#define OTHERS_RESERVE16                              ((ADVSND_PARAM)ADVSND_PARAM_DUMMY)
#define OTHERS_RESERVE17                              ((ADVSND_PARAM)ADVSND_PARAM_DUMMY)
#define OTHERS_RESERVE18                              ((ADVSND_PARAM)ADVSND_PARAM_DUMMY)
#define OTHERS_RESERVE19                              ((ADVSND_PARAM)ADVSND_PARAM_DUMMY)
#define OTHERS_RESERVE20                              ((ADVSND_PARAM)ADVSND_PARAM_DUMMY)
#define OTHERS_RESERVE21                              ((ADVSND_PARAM)ADVSND_PARAM_DUMMY)
#define OTHERS_RESERVE22                              ((ADVSND_PARAM)ADVSND_PARAM_DUMMY)
#define OTHERS_RESERVE23                              ((ADVSND_PARAM)ADVSND_PARAM_DUMMY)
#define OTHERS_RESERVE24                              ((ADVSND_PARAM)ADVSND_PARAM_DUMMY)
#define OTHERS_RESERVE25                              ((ADVSND_PARAM)ADVSND_PARAM_DUMMY)
#define OTHERS_RESERVE26                              ((ADVSND_PARAM)ADVSND_PARAM_DUMMY)
#define OTHERS_RESERVE27                              ((ADVSND_PARAM)ADVSND_PARAM_DUMMY)
#define OTHERS_RESERVE28                              ((ADVSND_PARAM)ADVSND_PARAM_DUMMY)
#define OTHERS_RESERVE29                              ((ADVSND_PARAM)ADVSND_PARAM_DUMMY)
#define OTHERS_RESERVE30                              ((ADVSND_PARAM)ADVSND_PARAM_DUMMY)

#else

typedef enum
{
    DOLBY_PL2VDS,
    DOLBY_PL2VDPK,
    BBE,
    SRS_TSXT,
    SRS_TSHD,
    SRS_THEATERSOUND,
    DTS_ULTRATV,
    AUDYSSEY,
    SUPER_VOICE,
    DBX,
    SRS_PURESND,
    DTS_STUDIOSOUND_3D, // DTS_STUDIOSOUND_3D = DTS_STUDIOSOUND_I
    BONGIOVI_DPS,
    CV3,
    SONICMOTION_ABS3D,
    DOLBY_DAP,
    DTS_VIRTUALX,
    DTS_STUDIOSOUND_II,
    ADV_NONE,
} ADVSND_TYPE;

typedef enum
{
    //SRS_TSXT
    SRS_TSXT_TRUBASS,
    SRS_TSXT_DC,

    //SRS_TSHD
    SRS_TSHD_TRUBASS,
    SRS_TSHD_DC,
    SRS_TSHD_DEFINITION,
    SRS_TSHD_SRS3D,

    //SRS_THEATERSOUND
    SRS_THEATERSOUND_TSHD,
    SRS_THEATERSOUND_TRUBASS,
    SRS_THEATERSOUND_DC,
    SRS_THEATERSOUND_DEFINITION,
    SRS_THEATERSOUND_TRUVOLUME,
    SRS_THEATERSOUND_HARDLIMITER,
    SRS_THEATERSOUND_HPF,
    SRS_THEATERSOUND_TRUEQ,
    SRS_THEATERSOUND_TRUVOLUME_NOISE_MNGR,

    //DTS_ULTRATV
    DTS_ULTRATV_EVO,
    DTS_ULTRATV_SYM,

    //AUDYSSEY
    AUDYSSEY_DYNAMICVOL,
    AUDYSSEY_DYNAMICEQ,
    AUDYSSEY_EQ,
    AUDYSSEY_ABX,

    SRS_THEATERSOUND_TSHD_SURR,
    SRS_THEATERSOUND_TSHD_LEVEL_INDPT,
    SRS_THEATERSOUND_CS,
    SRS_THEATERSOUND_TRUDIALOG,
    SRS_THEATERSOUND_TRUVOLUME_NORMALIZER,
    SRS_THEATERSOUND_TRUVOLUME_SMOOTH,
    SRS_THEATERSOUND_HPF_END,
    SRS_THEATERSOUND_TOTAL,

    SRS_PURESOUND_HL,
    SRS_PURESOUND_AEQ,
    SRS_PURESOUND_HPF,
    SRS_PURESOUND_TBHD,
    SRS_THEATERSOUND_CC3D_EN,
    SRS_THEATERSOUND_CC3D_DEPTH_PROCESS_EN,
    SRS_THEATERSOUND_CC3D_3D_SURR_BOOST_EN,
    SRS_THEATERSOUND_CC3D_FADE_EN,
    SRS_THEATERSOUND_CC3D_TSHD_MIX_EN,
    SRS_THEATERSOUND_CC3D_TBHDX_EN,
    SRS_THEATERSOUND_GEQ_EN,
    SRS_THEATERSOUND_3D_EN,

    // BONGIOVI DPS
    BONGIOVI_DPS_SURR_EN,

    // SONICEMOTOIN_ABS3D
    SONICEMOTION_ABS3D_EN,

    // DOLBY_DAP
    DOLBY_DAP_EN,
    DOLBY_DAP_ATMOS_EN,

    // DTS_VIRTUALX
    DTS_VIRTUALX_EN,
    DTS_VIRTUALX_SOUNDEFFECT_EN,
    DTS_VIRTUALX_SOUNDEFFECT_TRUSRNDX_EN,
    DTS_VIRTUALX_SOUNDEFFECT_TBHDX_EN,
    DTS_VIRTUALX_SOUNDEFFECT_MBHL_EN,

    // DTS_TRUVOLUMEHD
    DTS_TRUVOLUMEHD_EN,

    // DTS_STUDIOSOUND_II
    DTS_STUDIOSOUND_II_EN,
} ADVFUNC;

typedef enum
{
    //BBE
    BBE_MODE_SET,

    //DOLBY
    DOLBY_PL2VDPK_SMOD,
    DOLBY_PL2VDPK_WMOD,

    //SRS TSXT
    SRS_TSXT_SET_INPUT_GAIN,
    SRS_TSXT_SET_DC_GAIN,
    SRS_TSXT_SET_TRUBASS_GAIN,
    SRS_TSXT_SET_SPEAKERSIZE,
    SRS_TSXT_SET_INPUT_MODE,
    SRS_TSXT_SET_OUTPUT_GAIN,
    SRS_TSXT_RESERVE1,
    SRS_TSXT_RESERVE2,
    SRS_TSXT_RESERVE3,

    //SRS TSHD in SE-DSP
    SRS_TSHD_SET_INPUT_MODE,
    SRS_TSHD_SET_OUTPUT_MODE,
    SRS_TSHD_SET_SPEAKERSIZE,
    SRS_TSHD_SET_TRUBASS_CONTROL,
    SRS_TSHD_SET_DEFINITION_CONTROL,
    SRS_TSHD_SET_DC_CONTROL,
    SRS_TSHD_SET_SURROUND_LEVEL,
    SRS_TSHD_SET_INPUT_GAIN,
    SRS_TSHD_SET_WOWSPACE_CONTROL,
    SRS_TSHD_SET_WOWCENTER_CONTROL,
    SRS_TSHD_SET_WOWHDSRS3DMODE,
    SRS_TSHD_SET_LIMITERCONTROL,
    SRS_TSHD_SET_OUTPUT_GAIN,
    SRS_TSHD_RESERVE1,
    SRS_TSHD_RESERVE2,
    SRS_TSHD_RESERVE3,

    //SRS_THEATERSOUND
    SRS_THEATERSOUND_INPUT_GAIN,
    SRS_THEATERSOUND_DEFINITION_CONTROL,
    SRS_THEATERSOUND_DC_CONTROL,
    SRS_THEATERSOUND_TRUBASS_CONTROL,
    SRS_THEATERSOUND_SPEAKERSIZE,
    SRS_THEATERSOUND_HARDLIMITER_LEVEL,
    SRS_THEATERSOUND_HARDLIMITER_BOOST_GAIN,
    SRS_THEATERSOUND_HEADROOM_GAIN,
    SRS_THEATERSOUND_TRUVOLUME_MODE,
    SRS_THEATERSOUND_TRUVOLUME_REF_LEVEL,
    SRS_THEATERSOUND_TRUVOLUME_MAX_GAIN,
    SRS_THEATERSOUND_TRUVOLUME_NOISE_MNGR_THLD,
    SRS_THEATERSOUND_TRUVOLUME_CALIBRATE,
    SRS_THEATERSOUND_TRUVOLUME_INPUT_GAIN,
    SRS_THEATERSOUND_TRUVOLUME_OUTPUT_GAIN,
    SRS_THEATERSOUND_HPF_FC,
    SRS_THEATERSOUND_TSHD_INPUT_GAIN,
    SRS_THEATERSOUND_TSHD_OUTPUT_GAIN,
    SRS_THEATERSOUND_SURR_LEVEL_CONTROL,
    SRS_THEATERSOUND_TRUBASS_COMPRESSOR_CONTROL,
    SRS_THEATERSOUND_TRUBASS_PROCESS_MODE,
    SRS_THEATERSOUND_TRUBASS_SPEAKER_AUDIO,
    SRS_THEATERSOUND_TRUBASS_SPEAKER_ANALYSIS,
    SRS_THEATERSOUND_OUTPUT_GAIN,
    SRS_THEATERSOUND_BYPASS_GAIN,
    SRS_THEATERSOUND_INPUT_MODE,
    SRS_THEATERSOUND_TRUVOLUME_BYPASS_GAIN,
    SRS_THEATERSOUND_CS_INPUT_GAIN,

    //DTS_ULTRATV
    DTS_ULTRATV_EVO_MONOINPUT,
    DTS_ULTRATV_EVO_WIDENINGON,
    DTS_ULTRATV_EVO_ADD3DBON,
    DTS_ULTRATV_EVO_PCELEVEL,
    DTS_ULTRATV_EVO_VLFELEVEL,
    DTS_ULTRATV_RESERVE1,
    DTS_ULTRATV_RESERVE2,
    DTS_ULTRATV_RESERVE3,
    DTS_ULTRATV_RESERVE4,
    DTS_ULTRATV_RESERVE5,
    DTS_ULTRATV_RESERVE6,

    DTS_ULTRATV_SYM_DEFAULT,
    DTS_ULTRATV_SYM_MODE,
    DTS_ULTRATV_SYM_LEVEL,
    DTS_ULTRATV_SYM_RESET,
    DTS_ULTRATV_RESERVE7,
    DTS_ULTRATV_RESERVE8,
    DTS_ULTRATV_RESERVE9,

    //AUDYSSEY
    AUDYSSEY_DYNAMICVOL_COMPRESS_MODE,
    AUDYSSEY_DYNAMICVOL_GC,
    AUDYSSEY_DYNAMICVOL_VOLSETTING,
    AUDYSSEY_DYNAMICEQ_EQOFFSET,
    AUDYSSEY_ABX_GWET,
    AUDYSSEY_ABX_GDRY,
    AUDYSSEY_ABX_FILSET,
    AUDYSSEY_RESERVE1,
    AUDYSSEY_RESERVE2,
    AUDYSSEY_RESERVE3,
    AUDYSSEY_RESERVE4,
    AUDYSSEY_RESERVE5,
    AUDYSSEY_RESERVE6,

    //DBX
    DBX_MODE_SET,
    DBX_LOAD_PM,
    DBX_LOAD_DM,
    DBX_COPY_TotSonDM,
    DBX_COPY_TotSonPM,
    DBX_COPY_TotVolDM,
    DBX_COPY_TotVolPM,
    DBX_COPY_TotSurDM,
    DBX_COPY_TotSurPM,
    OTHERS_RESERVE9,
    OTHERS_RESERVE10,
    OTHERS_RESERVE11,
    OTHERS_RESERVE12,
    OTHERS_RESERVE13,
    OTHERS_RESERVE14,
    OTHERS_RESERVE15,
    OTHERS_RESERVE16,
    OTHERS_RESERVE17,
    OTHERS_RESERVE18,
    OTHERS_RESERVE19,
    OTHERS_RESERVE20,
    OTHERS_RESERVE21,
    OTHERS_RESERVE22,
    OTHERS_RESERVE23,
    OTHERS_RESERVE24,
    OTHERS_RESERVE25,
    OTHERS_RESERVE26,
    OTHERS_RESERVE27,
    OTHERS_RESERVE28,
    OTHERS_RESERVE29,
    OTHERS_RESERVE30,

    SRS_THEATERSOUND_CS_PROCESS_MODE,
    SRS_THEATERSOUND_CS_LR_OUTPUT_GAIN,
    SRS_THEATERSOUND_CS_LsRs_OUTPUT_GAIN,
    SRS_THEATERSOUND_CS_Center_OUTPUT_GAIN,
    SRS_THEATERSOUND_TRUDIALOG_INPUT_GAIN,
    SRS_THEATERSOUND_TRUDIALOG_OUTPUT_GAIN,
    SRS_THEATERSOUND_TRUDIALOG_BYPASS_GAIN,
    SRS_THEATERSOUND_TRUDIALOG_PROCESS_GAIN,
    SRS_THEATERSOUND_TRUDIALOG_CLARITY_GAIN,
    SRS_THEATERSOUND_TRUVOLUME_NORMALIZE_THRESH,
    SRS_THEATERSOUND_HPF_CUTOFF,
    SRS_THEATERSOUND_HPF_END_CUTOFF,


    //SRS PURESOUND
    SRS_PURESOUND_HL_INPUT_GAIN,
    SRS_PURESOUND_HL_OUTPUT_GAIN,
    SRS_PURESOUND_HL_BYPASS_GAIN,
    SRS_PURESOUND_HL_LIMITERBOOST,
    SRS_PURESOUND_HL_HARDLIMIT,
    SRS_PURESOUND_HL_DELAYLEN,
    SRS_PURESOUND_AEQ_INPUT_GAIN,
    SRS_PURESOUND_AEQ_OUTPUT_GAIN,
    SRS_PURESOUND_AEQ_BYPASS_GAIN,
    SRS_PURESOUND_FILTER_CONFIG,
    SRS_PURESOUND_HPF_FREQUENCY,
    SRS_PURESOUND_TBHD_TRUBASS_LEVEL,
    SRS_PURESOUND_TBHD_SPEAKER_SIZE,
    SRS_PURESOUND_TBHD_LEVEL_INDEPENDENT_EN,
    SRS_PURESOUND_TBHD_COMPRESSOR_LEVEL,
    SRS_PURESOUND_TBHD_MODE,
    SRS_PURESOUND_TBHD_SPEAKER_AUDIO,
    SRS_PURESOUND_TBHD_SPEAKER_ANALYSIS,
    SRS_PURESOUND_INPUT_GAIN,
    SRS_PURESOUND_OUTPUT_GAIN,

    //SRS_THEATERSOUND_3D
    SRS_THEATERSOUND_CC3D_INPUT_GAIN,
    SRS_THEATERSOUND_CC3D_OUTPUT_GAIN,
    SRS_THEATERSOUND_CC3D_BYPASS_GAIN,
    SRS_THEATERSOUND_CC3D_APERTURE,
    SRS_THEATERSOUND_CC3D_GAINLIMIT,
    SRS_THEATERSOUND_CC3D_FF_DEPTH,
    SRS_THEATERSOUND_CC3D_NF_DEPTH,

    SRS_THEATERSOUND_CC3D_TSHD_SURR_MODE,
    SRS_THEATERSOUND_CC3D_TSHD_MIX_FADE_CTRL,

    SRS_THEATERSOUND_CC3D_TBHDX_INPUT_GAIN,
    SRS_THEATERSOUND_CC3D_TBHDX_BASSLEVEL,
    SRS_THEATERSOUND_CC3D_TBHDX_SPEAKERSIZE,
    SRS_THEATERSOUND_CC3D_TBHDX_MODE,
    SRS_THEATERSOUND_CC3D_TBHDX_DYNAMICS,
    SRS_THEATERSOUND_CC3D_TBHDX_HP_ORDER,
    SRS_THEATERSOUND_CC3D_TBHDX_CUSTOM_FILTER,
    SRS_THEATERSOUND_GEQ_INPUT_GAIN,
    SRS_THEATERSOUND_GEQ_BAND0_GAIN,
    SRS_THEATERSOUND_GEQ_BAND1_GAIN,
    SRS_THEATERSOUND_GEQ_BAND2_GAIN,
    SRS_THEATERSOUND_GEQ_BAND3_GAIN,
    SRS_THEATERSOUND_GEQ_BAND4_GAIN,
    SRS_THEATERSOUND_CC3D_PROCESS_PATH,
    SRS_THEATERSOUND_TSHD_BYPASS_GAIN,

    //Dolby Volume
    DV258_ENABLE,
    DV258_LCL_ENABLE,
    DV258_LCL_LEVELER_ENABLE,
    DV258_LCL_MODELER_ENABLE,
    DV258_LCL_MODELER_HALF_MODE,
    DV258_LCL_AMOUNT,
    DV258_LCL_AVM,
    DV258_LCL_DVM,
    DV258_LCL_IRL,
    DV258_LCL_ORL,
    DV258_LCL_PREGAIN,
    DV258_LCL_GAIN,

    DV258_LIMITER_ENABLE,
    DV258_LIMITER_PEAK,
    DV258_LIMITER_BOOST,
    DV258_LIMITER_RMS,

    // BONGIOVI_DPS
    BONGIOVI_DPS_SET_PARAMS,
    BONGIOVI_DPS_SET_GLOBAL_INPUT_GAIN,
    BONGIOVI_DPS_SET_GLOBAL_OUTPUT_GAIN,
    BONGIOVI_DPS_SET_STEREO_WIDENER_MGAIN,
    BONGIOVI_DPS_SET_STEREO_WIDENER_SGAIN,

    // SONICEMOTION ABS3D
    SONICEMOTION_ABS3D_SET_PARAMS,

    // DOLBY_DAP
    DOLBY_DAP_SET_PARAMS,

    // DBX
    DBXTV_SET_MODE,

    // SRS PURESOUND
    SRS_PURESOUND_AEQ_COEFFICIENT,

    // DTS VIRAUTLX
    DTS_VIRTUALX_SET_INPUT_CHANNEL_MODE,
    DTS_VIRTUALX_SET_API_PARAMS,
    DTS_VIRTUALX_GET_API_PARAMS,
    DTS_VIRTUALX_SET_APP_PARAMS,
    DTS_VIRTUALX_GET_APP_PARAMS,
    DTS_VIRTUALX_SET_SAMPLERATE,
    DTS_VIRTUALX_SET_PARAMS_BY_CLI,

    // DTS TRUVOLUMEHD
    DTS_TRUVOLUMEHD_SET_INPUT_CHANNEL_MODE,
    DTS_TRUVOLUMEHD_SET_API_PARAMS,
    DTS_TRUVOLUMEHD_GET_API_PARAMS,
    DTS_TRUVOLUMEHD_SET_SAMPLERATE,
    DTS_TRUVOLUMEHD_SET_PARAMS_BY_CLI,

} ADVSND_PARAM;

#endif

//====== Certification Mode TYPE========
typedef enum
{
    AUDIO_DOLBY_DD_Cert,                ///<
    AUDIO_DOLBY_DDP_Cert,               ///<
    AUDIO_DOLBY_PULSE_Cert,             ///<
    AUDIO_DOLBY_DDCO_Cert,              ///<
    AUDIO_DOLBY_MS10_Cert,              ///<
    AUDIO_DOLBY_RESERVE1,               ///<
    AUDIO_DOLBY_RESERVE2,               ///<
    AUDIO_DOLBY_RESERVE3,               ///<
    AUDIO_DTS_DMP_Cert,                 ///<
    AUDIO_DTS_BC_Cert,                  ///<
    AUDIO_DTS_PremiumTV_Cert,           ///<
    AUDIO_DTS_SYMMETRY_Cert,            ///<
    AUDIO_DTS_ENVELO_Cert,              ///<
    AUDIO_DTS_RESERVE1,                 ///<
    AUDIO_DTS_RESERVE2,                 ///<
    AUDIO_DTS_RESERVE3,                 ///<
    AUDIO_SRS_XT_Cert,                  ///<
    AUDIO_SRS_RESERVE1,                 ///<
    AUDIO_SRS_RESERVE2,                 ///<
    AUDIO_SRS_RESERVE3,                 ///<
} Audio_CertMode;

typedef enum
{
    AUDIO_CERT_Enable,
    AUDIO_CERT_Disable
}Audio_CertEnable;

//====== VoIP Applications ========
typedef enum
{
    /* VoIP Application CallBack */
    AUDIO_VoIP_CONFIG_REGISTER_DEC_CB,
    AUDIO_VoIP_CONFIG_REGISTER_SE_CB,

    /* VoIP Application Audio In/Out */
    AUDIO_VoIP_CONFIG_ALSA_PLAYBACK,

    /* Voip Application setup pcm upload and config SRC */
    AUDIO_VOIP_CONFIG_PCM_UPLOAD,

}AUDIO_VoIP_CONFIG_TYPE;


//====== STAOS PIP START 2012/02/01 ========

typedef struct
{
    XPCM_TYPE audioType;
#if defined(UFO_PUBLIC_HEADER_212) ||defined(UFO_PUBLIC_HEADER_500_3)
    MS_U32    sampleRate;
#else
    MS_U16    sampleRate;
#endif
    MS_U16    blockSize;
    MS_U16    samplePerBlock;
    MS_U8     channels;
    MS_U8     bitsPerSample;

} Audio_XPCM_Param;

typedef struct
{
    #define  MAX_NUM_CODECS  5

    MS_U16 Channels[MAX_NUM_CODECS];
    MS_U16 Regions[MAX_NUM_CODECS];
    MS_U16 cplStart[MAX_NUM_CODECS];
    MS_U16 cplQbits[MAX_NUM_CODECS];
    MS_U16 FrameSize[MAX_NUM_CODECS];

    MS_U16 mNumCodecs;
    MS_U16 mSamples;
    MS_U16 mSampleRate;

} Audio_COOK_Param;

typedef struct
{
    MS_U32    u32Version;
    MS_U32    u32Channels;
    MS_U32    u32SampleRate;
    MS_U32    u32ByteRate;
    MS_U32    u32BlockAlign;
    MS_U32    u32Encopt;
    MS_U32    u32ParsingByApp;
    MS_U32    u32BitsPerSample;
    MS_U32    u32ChannelMask;
    MS_U32    u32DrcParamExist;
    MS_U32    u32DrcRmsAmpRef;
    MS_U32    u32DrcRmsAmpTarget;
    MS_U32    u32DrcPeakAmpRef;
    MS_U32    u32DrcPeakAmpTarget;
    MS_U32    u32MaxPacketSize;

} Audio_ASF_Param;

typedef struct
{
    MS_U32 U32WrtAddr;
    MS_U32 U32WrtBytes;

} Audio_MM_Data_Request;

typedef struct
{
    MS_U32  U32DecodeFrame;
    MS_U32  U32WrtAddr;
    MS_U32  U32WrtBytes;
    /* TVOS2.0 DTV */
    MS_S64  S64CPTS;
    MS_U32  U32FrameSize;
    MS_U32  U32ReadAddr;
    MS_U32  U32Channels;
    MS_U32  U32Bps;
    MS_U32  U32SampleRate;
    MS_U32  U32WriteAddr;
} Audio_MM_PCM_Info;

typedef struct
{
    MS_U32  U32Name;
    MS_U32  U32IsHashkeySupported;

} Audio_Hashkey_Info;

/// Audio Decoder ID Information
typedef enum
{
    /// Audio Decoder ID invalid
    AU_DEC_INVALID                       = -1,
    /// Audio 1-Decoder ID
    AU_DEC_ID1                           =  0,
    /// Audio 2-Decoder ID
    AU_DEC_ID2                           =  1,
    /// Audio 3-Decoder ID
    AU_DEC_ID3                           =  2,
    /// Audio 4-Decoder ID
    AU_DEC_ID4                           =  3,
    /// Audio 5-Decoder ID
    AU_DEC_ID5                           =  4,
    AU_DEC_MAX                           =  5

} AUDIO_DEC_ID;

typedef enum
{
    AUDIO_DSP_ID_DEC = 0x00,
    AUDIO_DSP_ID_SND,
    AUDIO_DSP_ID_R2,

    AUDIO_DSP_ID_ALL = 0xFF
} AUDIO_DSP_ID;

typedef enum
{
    AUDIO_MM_OMX = 0x00,
    AUDIO_MM_VD,
    AUDIO_MM_INJECT,
    AUDIO_MM_FRAME_DECODE,
    AUDIO_MM_MAX = 0xFF
} AUDIO_MM_TYPE;

typedef enum
{
    FILTER_TYPE_AUDIO_1 = 0,
    FILTER_TYPE_AUDIO_2,
    FILTER_TYPE_AUDIO_INVALID,

} AUDIO_FILTER_TYPE;

typedef enum
{
    Audio_ParamType_SoundMode = 0,
    Audio_ParamType_syncSTC,
    Audio_ParamType_PTS_info,
    Audio_ParamType_setAudioDecoder,
    Audio_ParamType_acMode,
    Audio_ParamType_MM_FFx2,
    Audio_ParamType_reqDataSize,
    Audio_ParamType_esBuf_informDataRdy,      // new
    Audio_ParamType_playControl,
    Audio_ParamType_mute,
    Audio_ParamType_predecode,                // for KTV
    Audio_ParamType_setWMADecParam,
    Audio_ParamType_setCOOKDecParam,
    Audio_ParamType_setXPCMDecParam,
    Audio_ParamType_UNI_PCM3_Input_Addr,
    Audio_ParamType_UNI_NEED_DECODE_FRMCNT,
    Audio_ParamType_UNI_ES_Wptr,
    Audio_ParamType_UNI_PCMOutCnt,
    Audio_ParamType_AllocDecoder,
    Audio_ParamType_FreeDecoder,
    Audio_ParamType_omx_spdif_mode,
    Audio_ParamType_omx_spdif_play,
    Audio_ParamType_omx_spdif_pcmLevel,
    Audio_ParamType_setExtSynth,
    Audio_ParamType_trueHD_bypass_enable,
    Audio_ParamType_tvos2_PCM_rdPtr,     //  byte pointer        // update when change       //range : 0 ~ PCM1_DRAM_SIZE
    Audio_ParamType_tvos2_DTV_SyncType,  // 0: Normal type, 1: TVOS2.0
    Audio_ParamType_CommonCmd,
    Audio_ParamType_Multi_Channel,
    Audio_ParamType_omx_spdif_pause,
    Audio_ParamType_omx_spdif_flush,
    Audio_ParamType_omx_spdif_allowed_sampleSize,
    Audio_R2_SHM_PARAM_DOLBY_DRC_MODE,
    Audio_R2_SHM_PARAM_DOLBY_DMX_MODE,
    Audio_R2_SHM_PARAM_DOLBY_HIGH_CUT,
    Audio_R2_SHM_PARAM_DOLBY_LOW_BOOST,
    Audio_R2_SHM_PARAM_AD_Volume,
    Audio_R2_Set_NonPcm_Sel,
    Audio_ParamType_Abs_ReadReg_Target,
    Audio_ParamType_Abs_WriteReg_Target,
    Audio_ParamType_Abs_WriteReg_Value,
    Audio_ParamType_ReadDM_Target,
    Audio_ParamType_WriteDM_Target,
    Audio_ParamType_WriteDM_Value,
    Audio_ParamType_ReadPM_Target,
    Audio_ParamType_WritePM_Target,
    Audio_ParamType_WritePM_Value,
    Audio_ParamType_Abs_ReadXbox_Target,
    Audio_ParamType_Abs_WriteXbox_Target,
    Audio_ParamType_Abs_WriteXbox_Value,
    Audio_ParamType_Audio_AAC_mm_single_frame_mode,

    Audio_R2_SHM_PARAM_ES_Limiter_EN,
    Audio_R2_SHM_PARAM_ES_Limiter_Threshold,
    Audio_R2_SHM_PARAM_PCM_Limiter_EN,
    Audio_R2_SHM_PARAM_PCM_Limiter_Threshold,
    Audio_SE_Set_PCM_Capture_Volume,
    Audio_SE_Set_PCM_Capture2_Volume,
    Audio_infoType_Abs_ReadReg_Target,
    Audio_infoType_Abs_WriteReg_Target,
    Audio_infoType_Abs_WriteReg_Value,
    Audio_infoType_ReadDM_Target,
    Audio_infoType_WriteDM_Target,
    Audio_infoType_WriteDM_Value,
    Audio_infoType_ReadPM_Target,
    Audio_infoType_WritePM_Target,
    Audio_infoType_WritePM_Value,
    Audio_ParamType_UNI_TrickMode,
    Audio_ParamType_MPG_AVOFFSET,
    Audio_ParamType_AC3_AVOFFSET,
    Audio_ParamType_AAC_AVOFFSET,
    Audio_ParamType_SE_Set_I2S_OutNumber,
    Audio_ParamType_SE_Set_HP_Delay,
    Audio_ParamType_SE_Set_ADEC0_Input_delay,
    Audio_ParamType_SE_Set_ADEC1_Input_delay,
    Audio_ParamType_R2_Set_NonPcm_Sel,
    Audio_ParamType_Low_Latency_Limiter_ES_enable,
    Audio_ParamType_Low_Latency_Limiter_ES_Threshold,
    Audio_ParamType_Low_Latency_Limiter_PCM_Threshold,
    Audio_ParamType_Low_Latency_Limiter_PCM_Skip_Target,
    Audio_ParamType_ES2_IsUsedBy_Dual,
    Audio_ParamType_ddp_disable_atmos,
    Audio_ParamType_Decoder_RepeatPlay,
    Audio_ParamType_SetEDIDOutputInterface,
    Audio_ParamType_FreerunMode_Pcm_Output_Threshold_Enable,
    Audio_ParamType_FreerunMode_Pcm_Output_Threshold_Value,  //ms

    Audio_ParamType_Downmix_Mode,
    Audio_ParamType_DolbyDrc_Mode,
    Audio_ParamType_DolbyDrc_HighCutScale,
    Audio_ParamType_DolbyDrc_LowBootScale,

    Audio_ParamType_Decoder_FlushPcmFrom_PTS,
    Audio_ParamType_Decoder_FlushPcmFrom_Immediately,
    Audio_ParamType_Decoder_FlushPcmFrom_Cancel,
    Audio_ParamType_Decoder_FlushPcmUntil_PTS,
    Audio_ParamType_Decoder_FlushPcmUntil_Immediately,
    Audio_ParamType_Decoder_FlushPcmUntil_Cancel,
    Audio_ParamType_Decoder_FlushPcm_PTS_Tolerance,

    Audio_ParamType_PUSI_Enable,

    Audio_ParamType_Spdif_RDptr,
    Audio_ParamType_omx_pcm3_pcmLevel,
    Audio_ParamType_omx_spdif_flush_en,

    Audio_ParamType_MPEG_Decoder_Delay,
    Audio_ParamType_AAC_Decoder_Delay,
    Audio_ParamType_AC3_Decoder_Delay,
    Audio_ParamType_AC4_Decoder_Delay,

    Audio_ParamType_Tempo_Enable,
    Audio_ParamType_Tempo_PlayRate,

    Audio_ParamType_DTS_Cert_Seamless_Mode,

    Audio_ParamType_omx_atmos_valid,

    Audio_ParamType_Speaker_With_AD_Enable,
    Audio_ParamType_Headphone_With_AD_Enable,

    Audio_ParamType_Decoder_Delay,

    Audio_ParamType_Decoder_ApplyPanFade_Enable,

    Audio_ParamType_Low_Latency_Limiter_PCM_Enable,
    Audio_ParamType_PTS_OFFSET,       /* Media Sync */
} Audio_ParamType;

typedef enum
{
    Audio_infoType_Unknow = -1,
    Audio_infoType_DecStatus = 0,
    Audio_infoType_SampleRate,
    Audio_infoType_SoundMode,
    Audio_infoType_capability,
    Audio_infoType_AudioSyncStatus,   // 1 : AV-Sync  0: Free-run
    Audio_infoType_bitRate,
    Audio_infoType_acmod,
    Audio_infoType_okFrmCnt,
    Audio_infoType_errFrmCnt,
    Audio_infoType_1ms_timeStamp,
    Audio_infoType_33bit_PTS          = 10,
    Audio_infoType_33bit_STCPTS_DIFF,
    Audio_infoType_esBuf_req,
    Audio_infoType_BufferSize,
    Audio_infoType_BufferAddr,
    Audio_infoType_esBuf_currLevel,
    Audio_infoType_pcmBuf_currLevel,
    Audio_infoType_get_audio_filter,
    Audio_infoType_audio_unsupported,
    Audio_infoType_getDecInfo1,
    Audio_infoType_UNI_PCM1_Info      = 20,
    Audio_infoType_UNI_PCM3_Level,
    Audio_infoType_UNI_ES_Level,
    Audio_infoType_UNI_ES_Base,
    Audio_infoType_UNI_ES_Size,
    Audio_infoType_UNI_PCM_Base,
    Audio_infoType_UNI_PCM_Size,
    Audio_infoType_Decoder_ID,
    Audio_infoType_AccumEsCnt,
    Audio_infoType_MadBaseAddr,
    Audio_infoType_hashkey,
    Audio_infoType_UNI_PCM3_Base,
    Audio_infoType_UNI_PCM3_Size,
    Audio_infoType_AC3_BSID,
    Audio_infoType_get_trueHD_bypass_enable,
    Audio_infoType_syncSTC_in_mmTs,
    Audio_infoType_Pcm_Output_Channel,
    Audio_R2_SHM_INFO_PCM_LEVEL,
    Audio_R2_SHM_INFO_DEC_STAUS,
    Audio_R2_SHM_INFO_CURR_PTS,
    Audio_R2_SHM_INFO_TP_ES_FreeSize,
    Audio_infoType_Abs_ReadReg,
    Audio_infoType_ReadDM_Value,
    Audio_infoType_ReadPM_Value,
    Audio_infoType_Abs_ReadXbox,
    Audio_infoType_omx_no_mixing,
    Audio_infoType_omx_ES_bypass,
    Audio_infoType_Decoder_Type,
    Audio_infoType_33bit_latestPTS,
    Audio_infoType_tvos2_PCM1_Info,
    Audio_infoType_tvos2_pcm_rdpt,
    Audio_infoType_tvos2_pcm_wrpt,
    Audio_infoType_tvos2_cpts,
    Audio_infoType_tvos2_pcm_framesize,
    Audio_infoType_Dec_Channel_Mode,
    Audio_infoType_DRA_Channel_Mapping,
    Audio_infoType_Smp_Rate,
    Audio_infoType_Bit_Rate,
    Audio_infoType_DOLBY_BS_Mode,
    Audio_infoType_DOLBY_AAC_type,
    Audio_infoType_DTS_CD_Mode,
    Audio_infoType_DTS_PCM_Frame_Size,
    Audio_infoType_DTS_ES_Type,
    Audio_infoType_SPDIF_info_flag,
    Audio_infoType_MPEG_Layer,
    Audio_infoType_MPEG_Stereo_Mode,
    Audio_infoType_MPEG_Header,
    Audio_infoType_SE_Decoder_Delay,
    Audio_infoType_SE_HpOut_Delay,
    Audio_infoType_R2_Dec_Type,
    Audio_infoType_get_parser_info,
    Audio_infoType_Is_Audio_Sync,
    Audio_infoType_R2_uart_buf_base,
    Audio_infoType_R2_uart_buf_size,
    Audio_infoType_R2_uart_buf_wptr,
    Audio_infoType_Get_DDPEnc_Status,
    Audio_infoType_Is_DDP_8ch_Stream,

    Audio_infoType_Decoder_FlushPcm_PTS_Tolerance,
    Audio_Comm_infoType_ADEC1_Ok_Frame_Count,
    Audio_Comm_infoType_ADEC1_Error_Frame_Count,
    Audio_Comm_infoType_ADEC1_Skip_Frame_Count,
    Audio_Comm_infoType_ADEC1_Repeat_Frame_Count,
    Audio_Comm_infoType_ADEC1_Play_Cmd,
    Audio_Comm_infoType_ADEC1_Codec_Type,

    Audio_infoType_Decoder_Pcm_Size,
    Audio_infoType_Decoder_Pcm_Pts,
    Audio_infoType_Decoder_Pcm_Buffer_Info,

    Audio_infoType_Is_Atmos_Stream,
    Audio_infoType_Decoder_Delay,

    Audio_infoType_Decoder_Es_Size,
    Audio_infoType_Decoder_Es_Pts,
    Audio_infoType_Decoder_Es_Buffer_Info,

    Audio_infoType_Spdif_WRptr,

    Audio_infoType_Decoder_Channel_Mapping,
} Audio_InfoType;

typedef MS_BOOL (*pFuncPtr_SetAudioParam2)(AUDIO_DEC_ID DecId, Audio_ParamType paramType, MS_VIRT Param);
typedef MS_BOOL (*pFuncPtr_GetAudioInfo2)(AUDIO_DEC_ID DecId, Audio_InfoType paramType, void * pInfo);

//====== STAOS PIP END 2012/02/01 ========

typedef enum
{
    Audio_DumpInfo_Off          = 0x0,
    Audio_DumpInfo_Avsync       = 0x1,
    Audio_DumpInfo_Dec1         = 0x2,
    Audio_DumpInfo_IoInfo       = 0x4,
    Audio_DumpInfo_mmFileAVsync = 0x8,
    Audio_DumpInfo_All          = 0x15,
    Audio_DumpInfo_AutoTest     = 0x80,
}Audio_DumpInfo_Type;

typedef enum
{
    Audio_OMX_SPDIF_OFF,
    Audio_OMX_SPDIF_ON,
    Audio_OMX_SPDIF_STOP,
    Audio_OMX_SPDIF_PLAY,

} Audio_OMX_SPDIF_Param;

typedef enum
{
    AUDIO_USAGE_TYPE_NONE,
    AUDIO_USAGE_TYPE_DTV,
    AUDIO_USAGE_TYPE_ATV,
    AUDIO_USAGE_TYPE_HDMI,
    AUDIO_USAGE_TYPE_ADC,
    AUDIO_USAGE_TYPE_MM,
    AUDIO_USAGE_TYPE_SPDIF,
    AUDIO_USAGE_TYPE_KTV,
    AUDIO_USAGE_TYPE_GAME,

} AUDIO_USAGE_TYPE;

///<Audio output port source type
typedef enum
{
    E_CONNECT_MAIN,                   ///< output connect to main source
    E_CONNECT_SUB,                     ///< output connect to sub source
    E_CONNECT_SCART,                 ///< output connect to scart source
    E_CONNECT_DATA_CAPTURE,  ///< output connect to data capture source
    E_CONNECT_KTV,                     ///< output connect to KTV source
    E_CONNECT_NULL,                   ///< Disable output port
} AUDIO_OUTPORT_SOURCE_TYPE;

// TEE Capability
typedef enum
{
    Audio_TEE_None                            = 0x00,
    Audio_TEE_Setup,
    Audio_TEE_Set_DSP_Secure_Accessed,
    Audio_TEE_Set_DSP_Normal_Accessed,

    Audio_TEE_Setup_and_ALL_DSP_Secure_Accessed   = 0xFF
} Audio_TEE_Capability;

///<Enum for Audio Ease Type
typedef enum
{
    E_AUDIO_EASE_TYPE_LINEAR,
    E_AUDIO_EASE_TYPE_INCUBIC,
    E_AUDIO_EASE_TYPE_OUTCUBIC,
    E_AUDIO_EASE_TYPE_INOUTCUBIC,
}AUDIO_EASE_TYPE;

///<Enum for Audio Menu Sound Control
typedef enum
{
    E_AUDIO_SINETONE_CTRL_PAUSE,
    E_AUDIO_SINETONE_CTRL_PLAY,
    E_AUDIO_SINETONE_CTRL_LEFTMUTE,
    E_AUDIO_SINETONE_CTRL_RIGHTMUTE,
}AUDIO_SINETONE_CTRL;

typedef enum
{
    E_AUDIO_SINETONE_MODE_MONO,
    E_AUDIO_SINETONE_MODE_SWEEP,
    E_AUDIO_SINETONE_MODE_300TO800,
    E_AUDIO_SINETONE_MODE_800TO300,
}AUDIO_SINETONE_MODE;

//=======================================
/// Init structure for audio
typedef struct ATTRIBUTE_PACKED
{
    MS_U8      miu;               ///< MIU
    MS_U8      tv_system;        ///< TV system
    MS_U8      dtv_system;       ///< DTV system
    MS_U8      au_system_sel;   ///< Audio system type

    //sif threshold
    AUDIO_SIF_SET_STANDARD        standard_u8Type;   ///< SIF standard type
    AUDIO_SIF_THRESHOLD_TYPE      threshold_u8Type;  ///< SIF Threshold type
    MS_U16                        threshold_value;   ///< SIF Threshold value

} AUDIO_INIT_INFO, *PAUDIO_INIT_INFO;

//=======================================
/// structure for audio output setting
typedef struct ATTRIBUTE_PACKED
{
    AUDIO_OUTPUT_TYPE   SpeakerOut;  ///< Audio output port for Speaker
    AUDIO_OUTPUT_TYPE   HpOut;       ///< Audio output port for HP
    AUDIO_OUTPUT_TYPE   MonitorOut;  ///< Audio output port for Monitor out
    AUDIO_OUTPUT_TYPE   ScartOut;    ///< Audio output port for Scart out
    AUDIO_OUTPUT_TYPE   SpdifOut;    ///< Audio output port for S/PDIF out
    AUDIO_OUTPUT_TYPE   ArcOut;      ///< Audio output port for ARC(HDMI_Rx) out
    AUDIO_OUTPUT_TYPE   HDMIOut;     ///< Audio output port for HDMI_Tx out
} AUDIO_OUT_INFO, *PAUDIO_OUT_INFO;

//=======================================
/// structure for audio DSP path setting
typedef struct ATTRIBUTE_PACKED
{
    AUDIO_PATH_TYPE   SpeakerOut;     ///< Audio DSP path for Speaker
    AUDIO_PATH_TYPE   HpOut;          ///< Audio DSP path for HP
    AUDIO_PATH_TYPE   MonitorOut;     ///< Audio DSP path for Monitor out
    AUDIO_PATH_TYPE   ScartOut;       ///< Audio DSP path for Scart out
    AUDIO_PATH_TYPE   SpdifOut;       ///< Audio DSP path for Spdif
    AUDIO_PATH_TYPE   ArcOut;         ///< Audio output port for ARC(HDMI_Rx) out
    AUDIO_PATH_TYPE   HDMIOut;        ///< Audio output port for HDMI out
} AUDIO_PATH_INFO, *PAUDIO_PATH_INFO;

//=======================================
/// structure for output port connection setting
typedef struct ATTRIBUTE_PACKED
{
    AUDIO_OUTPORT_SOURCE_TYPE   SpeakerOut;     ///<  for Speaker
    AUDIO_OUTPORT_SOURCE_TYPE   HpOut;          ///<  for HP
    AUDIO_OUTPORT_SOURCE_TYPE   MonitorOut;     ///<  for Monitor out
    AUDIO_OUTPORT_SOURCE_TYPE   ScartOut;       ///<  for Scart out
    AUDIO_OUTPORT_SOURCE_TYPE   SpdifOut;       ///<  for Spdif
    AUDIO_OUTPORT_SOURCE_TYPE   ArcOut;         ///<  for ARC(HDMI_Rx)
    AUDIO_OUTPORT_SOURCE_TYPE   HDMIOut;        ///<  for HDMI_Tx
} OUTPUT_SOURCE_INFO, *POUTPUT_SOURCE_INFO;

//=======================================
/// structure for AES info in new MM mode
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
} AES_INFO;


typedef enum
{
    E_TSP_0,
    E_TSP_1,
    E_TSP_2,
    E_TSP_3,
    E_TSP_4,
    E_TSP_5,
    E_TSP_6,
    E_TSP_7,
    E_TSP_MAX,
} AUDIO_STC_SOURCE;

typedef enum
{
    E_AUD_3_PARSER = 0x0,
    E_AUD_4_PARSER_type1 = 0x1,    ///<K3S_type (2 AFIFO & 4 audio parser arch)
    E_AUD_4_PARSER_type2 = 0x2,    ///<eiffle_type (4 AFIFO & 4 audio parser arch)
    E_AUD_1_PARSER = 0x3,
    E_AUD_2_PARSER = 0x4,
    E_AUD_MAX_PARSER = 0x5,
}AUDIO_PARSER_Architecture;

typedef enum
{
    E_AFIFO_0,
    E_AFIFO_1,
    E_AFIFO_2,
    E_AFIFO_3,
    E_AFIFO_4,
    E_AFIFO_5,
    E_AFIFO_MAX,
} AUDIO_AFIFO_SOURCE;

typedef enum
{
    AUDIO_FORCE_INVALID = -1,
    AUDIO_FORCE_ID1 = 0,
    AUDIO_FORCE_ID2 = 1,
    AUDIO_FORCE_ID3 = 2,
    AUDIO_FORCE_ID4 = 3,
    AUDIO_FORCE_ID5 = 4,
    AUDIO_FORCE_MAX = 5,
} AUDIO_FORCE_ADEC_ID;

typedef enum
{
    AUDIO_DOLBY_DAP_VERSION_NONE = 0,
    AUDIO_DOLBY_DAP_VERSION_V1,
    AUDIO_DOLBY_DAP_VERSION_V2,
    AUDIO_DOLBY_DAP_VERSION_MAX,
} AUDIO_DOLBY_DAP_VERSION;

typedef struct ATTRIBUTE_PACKED
{
    MS_BOOL bStart;
    MS_BOOL bConnect;
    MS_BOOL bAVSyncOn;
    MS_BOOL bIsAD;
    AUDIO_OUTPORT_SOURCE_TYPE eGroup;
    AUDIO_DSP_ID eDSPId;
    AUDIO_SOURCE_INFO_TYPE eSourceType;
    En_DVB_decSystemType eAudFormat;
    AUDIO_STC_SOURCE eStcSource;
    AUDIO_AFIFO_SOURCE eAfifoSource;
    AUDIO_MM_TYPE eMMType;
    AUDIO_FORCE_ADEC_ID eAudio_Force_ID;
    MS_S32 s32PTSOffset;
} AudioDecStatus_t;

typedef struct ATTRIBUTE_PACKED
{
    AUDIO_OUTPORT_SOURCE_TYPE eGroup;
    AUDIO_DSP_ID u8NonPcmPath;
    AUDIO_SOURCE_INFO_TYPE eSourceType;
    AUDIO_DIGITAL_OUTPUT_TYPE eDigitalOutfMode;
    MS_U8 u8R2NonPcmSetting;
} DigitalOutSetting_t;

typedef struct ATTRIBUTE_PACKED
{
    MS_BOOL   CBit;
    MS_BOOL   LBit;
} Digital_Out_Copy_Righr_t;

typedef struct ATTRIBUTE_PACKED
{
    MS_U8   u8PcmFormatControl  ;                     // byte 0, [0:7]
    MS_U8   u8Category;                                      // byte 1, [8:14]
    Digital_Out_Copy_Righr_t    stCopyRight;         // byte 0[1],  byte 1[15]

    MS_U8   u8SourceNumber;                            // byte 2, [16:19]
    MS_U8   u8ChannelNumber;                          // byte 2, [20:23]

    MS_U8   u8SamplingRate;                              // byte 3, bit[24:27]
    MS_U8   u8ClockPrecision;                            // byte 3, bit[28:31]

    MS_U8   u8WordLength;                                // byte 4, bit[32:35]
    MS_U8   u8OriginalSamplingRate;                // byte 4, bit[36:39]

    MS_U8   u8CGMS_A;                                      // byte 5, bit[40:41]
} Digital_Out_Channel_Status_t;

typedef struct ATTRIBUTE_PACKED
{
    MS_U8   support_codec;              // Short_Audio_Descriptor Byte1 [3:6], 0: default, 1: support, 2: not suppot
    MS_U8   support_ch;                 // Short_Audio_Descriptor Byte1 [0:2]
    MS_U8   support_samplerate;         // Short_Audio_Descriptor Byte2 [0:6]
    MS_U8   support_SAD_byte3;          // Short_Audio_Descriptor Byte3 [0:7]
} Digital_Out_Device_Capability_t;

//=======================================
/// enum for audio DDR Info
typedef enum
{
    E_AUD_MEMORY_BASE = 0x0, ///< for audio DDR Base
    E_AUD_MEMORY_SIZE = 0x1, ///< for audio DDR Size
} EN_AUDIO_DDRINFO;

typedef struct ATTRIBUTE_PACKED
{
    MS_U32 u32Size;
    MS_U64 u64Pts;
    MS_U64 u64MetadataIndex;
    MS_U8  *pBuffer;
} Buffer_Info_t;

typedef enum
{
   E_SPEAKER_2_0_0 = 0,
   E_SPEAKER_2_0_2 = 1,
   E_SPEAKER_2_1_2 = 2,
   E_SPEAKER_3_0_2 = 3,
   E_SPEAKER_3_1_2 = 4,
   E_SPEAKER_5_1_0 = 5,
   E_SPEAKER_7_1_0 = 6,
   E_SPEAKER_5_1_2 = 7,
   E_SPEAKER_MAXIMUM,
} E_SPEAKER_CONFIG;

#ifdef __cplusplus
}
#endif

#endif // _DRV_AUDIO_IF_H_
