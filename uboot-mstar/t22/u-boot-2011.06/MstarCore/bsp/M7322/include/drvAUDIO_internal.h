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

#ifndef _DRV_AUDIO_INTERNAL_H_
#define _DRV_AUDIO_INTERNAL_H_

#include "MsTypes.h"
#include "ULog.h"
#include "utopia.h"
#include "apiAUDIO.h"
#include "drvAUDIO_if.h"

#ifdef __cplusplus
extern "C"
{
#endif

//=====================================================
//  Macro and Define
//=====================================================
#define AUDIO_DO_ALIGNMENT(Value, AlignmentSize) \
    do { \
        Value = (Value /AlignmentSize) * AlignmentSize; \
    } while(0)

#define AUDIO_INVALID_SHM_THEN_RETURN_ERROR_VALUE(bRet) \
    do { \
        if (g_AudioVars2 == NULL) \
        { \
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s() : Warning! g_AudioVars2 should not be NULL !!!\n", __FUNCTION__); \
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s() : Perform SHM Init here !!!\n", __FUNCTION__); \
            MDrv_AUDIO_SHM_Init(); \
            if (g_AudioVars2 == NULL) \
            { \
                return bRet; \
            } \
        } \
    } while(0)

#define AUDIO_INVALID_SHM_THEN_RETURN \
    do { \
        if (g_AudioVars2 == NULL) \
        { \
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s() : Warning! g_AudioVars2 should not be NULL !!!\n", __FUNCTION__); \
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s() : Perform SHM Init here !!!\n", __FUNCTION__); \
            MDrv_AUDIO_SHM_Init(); \
            if (g_AudioVars2 == NULL) \
            { \
                return; \
            } \
        } \
    } while(0)

#define AUDIO_ADVSND_AUTO_RESTORE    1

//
#define STRING_TO_ENUM(string)             { #string, string }
#define ARRAY_SIZE(a)                      (sizeof(a) / sizeof((a)[0]))
#define MONITOR_MSG_TIME                   5000 // in monitor function print mesage interval time
//

//====== DSP code Type========
typedef enum
{
    AU_DVB_STANDARD_INVALID             = -1,

    AU_DVB_NONE                         = 0x0,
    AU_DVB_STANDARD_MPEG,
    AU_DVB_STANDARD_AC3,
    AU_DVB_STANDARD_AC3P,
    AU_DVB_STANDARD_AAC,
    AU_DVB_STANDARD_MP3,
    AU_DVB_STANDARD_WMA,
    AU_DVB_STANDARD_RA8LBR,
    AU_DVB_STANDARD_XPCM,
    AU_DVB_STANDARD_TONE,
    AU_DVB_STANDARD_DTS,
    AU_DVB_STANDARD_MS10_DDT,
    AU_DVB_STANDARD_MS10_DDC,
    AU_DVB_STANDARD_WMA_PRO,
    AU_DVB_STANDARD_FLAC,
    AU_DVB_STANDARD_VORBIS,
    AU_DVB_STANDARD_DTSLBR,
    AU_DVB_STANDARD_AMR_NB,
    AU_DVB_STANDARD_AMR_WB,
    AU_DVB_STANDARD_DRA,
    AU_DVB_STANDARD_MPEG_AD,
    AU_DVB_STANDARD_DolbyTrueHD_Bypass,
    AU_DVB_STANDARD_DTSHD_ADO,
    AU_DVB_STANDARD_AC4,
    AU_DVB_STANDARD_MPEG_H,
    AU_DVB_STANDARD_DTSXLL,
    AU_DVB_STANDARD_OPUS,
    AU_DVB_STANDARD_MAT,
    AU_DVB_MAX,

    AU_DVB_STANDARD_ENC,
    AU_DVB_STANDARD_MPEG_EN,

    AU_DVB2_NONE,
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
    AU_DVB2_MAX,

    AU_DVB2_ADVSND_NONE,
    AU_DVB2_ADVSND_PL2,
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
    AU_DVB2_ADVSND_BONGIOVI_DPS,
    AU_DVB2_ADVSND_SONICMOTION_ABS3D,
    AU_DVB2_ADVSND_DOLBY_DAP,
    AU_DVB2_ADVSND_DTS_VIRTUALX,
    AU_DVB2_ADVSND_DTS_StudioSound_II,
    AU_DVB2_ADVSND_MAX,

    AU_DEC_SYSTEM,
    AU_SE_SYSTEM,

    AU_KTV_ENC,
    AU_KTV_FUNC,
    AU_KTV_SYSTEM,

    AU_SND_EFFECT,

    AU_SIF_NONE,
    AU_SIF_BTSC,
    AU_SIF_EIAJ,
    AU_SIF_PALSUM,
    AU_SIF_FM_RADIO,
    AU_STANDARD_SIF_TYPE,
    AU_SIF_MAX,

    AU_ATVENC_NONE,
    AU_ATVENC_FMTx,
    AU_ATVENC_BTSC,

    AU_DVB2_ENCODE_NONE,    // define for snd_dsp encode code for because 0x30 has no free enum
    AU_DVB2_ENCODE_DTSE,
    AU_DVB2_ENCODE_MS10_DDE,
    AU_DVB2_ENCODE_DDE,
    AU_DVB2_ENCODE_MS12_DDPE,
    AU_DVB2_ENCODE_MAX,

    AU_DEC_ES_BYPASS,

}AUDIO_DSP_CODE_TYPE;

#define IS_AUDIO_DSP_CODE_TYPE_DEC1_SYSTEM(dspCodeType)  ((dspCodeType >= AU_DVB_NONE) && (dspCodeType < AU_DVB_MAX))
#define IS_AUDIO_DSP_CODE_TYPE_DEC2_SYSTEM(dspCodeType)  ((dspCodeType >= AU_DVB2_NONE) && (dspCodeType < AU_DVB2_MAX))
#define IS_AUDIO_DSP_CODE_TYPE_ADVSND(dspCodeType)       ((dspCodeType >= AU_DVB2_ADVSND_NONE) && (dspCodeType < AU_DVB2_ADVSND_MAX))
#define IS_AUDIO_DSP_CODE_TYPE_DEC_SYSTEM(dspCodeType)   (dspCodeType == AU_DEC_SYSTEM)
#define IS_AUDIO_DSP_CODE_TYPE_SE_SYSTEM(dspCodeType)    (dspCodeType == AU_SE_SYSTEM)
#define IS_AUDIO_DSP_CODE_TYPE_SIF(dspCodeType)          ((dspCodeType >= AU_SIF_NONE) && (dspCodeType < AU_SIF_MAX))
#define IS_AUDIO_DSP_CODE_TYPE_SE_ENC(dspCodeType)       ((dspCodeType >= AU_DVB2_ENCODE_NONE) && (dspCodeType < AU_DVB2_ENCODE_MAX))


//=====================================================
// Define
//=====================================================
#define AUDIO_DRIVER_VERSION_MAJOR    11 /* API change or new feature */
#define AUDIO_DRIVER_VERSION_MINOR    12 /* defect */
#define AUDIO_DRIVER_VERSION_REVISION    16 /* refine */

#define MAX_AUDIO_BASE_ADDRESS_NUM2    3
#define AUDIO_PCM_MAXNUM    32
#define AUDIO_ENCODER_MAXNUM    2
#define AUDIO_CHANNEL_MAXNUM    24
#define AUDIO_VOLUME_ARRAY_MAXNUM    101
#define AUDIO_DEBUG_CMDSTR_LENGTH    256
#define AUDIO_DEBUG_CODEC_NAME_LENGTH    30
#define AUDIO_DEBUG_SOURCE_INFO_NAME_LENGTH    30
#define AUDIO_DEBUG_TEMP_STRING_LENGTH    1024

#define DEFAULT_PCM_LIMITER_THRESHOLD    150 // ms
#define PCM_LIMITER_THRESHOLD_UPPER_BOUND    800 // ms
#define PCM_LIMITER_THRESHOLD_LOWER_BOUND    70 // ms

//==============================
//  IP Security Define
//==============================
#define  AU_DVB2_AUTHBIT_BBE            0
#define  AU_DVB2_AUTHBIT_SRS            2
#define  AU_DVB2_AUTHBIT_TSHD           3
#define  AU_DVB2_AUTHBIT_VIQ            4
#define  AU_DVB2_AUTHBIT_VDS            5
#define  AU_DVB2_AUTHBIT_VSPK           6
#define  AU_DVB2_AUTHBIT_DTS_StudioSound_II 6
#define  AU_DVB2_AUTHBIT_AUDSY          9
// Dolby DD
#define  AU_DVB_AUTHBIT_AC3             11
#define  AU_DVB2_AUTHBIT_AC3_AD         11
// Dolby DDP
#define  AU_DVB_AUTHBIT_AC3P            12
#define  AU_DVB2_AUTHBIT_AC3P_AD        12
// Dolby DDCO
#define  AU_DVB2_AUTHBIT_DDE            13
// Dolby Pulse(DDT)
#define  AU_DVB_AUTHBIT_MS10_DDT        14
// DTS DMP (Core 2.0)
#define  AU_DVB_AUTHBIT_DTSDEC          15
#define  AU_DVB2_AUTHBIT_DTSDEC         15
// DTS HD
#define  AU_DVB_AUTHBIT_DTSHD           18
// COOK(RA8-LBR) same with RM video
#define  AU_DVB_AUTHBIT_COOK            28
// WMA
#define  AU_DVB_AUTHBIT_WMA             30
#define  AU_DVB2_AUTHBIT_WMA            30
// SRS StudioSound
#define  AU_DVB2_AUTHBIT_SRS_StudioSND  55
// Dolby Volume
#define  AU_DVB2_AUTHBIT_DolbyVolume    56
// Dolby DD Plus Encode
#define  AU_DVB2_AUTHBIT_DDP_ENC        57
// DTS LBR
#define  AU_DVB_AUTHBIT_DTS_LBR         58
#define  AU_DVB2_AUTHBIT_DTS_LBR        58
// DTS XLL
#define  AU_DVB_AUTHBIT_DTS_XLL         59
#define  AU_DVB2_AUTHBIT_DTS_XLL        59
#define  AU_DVB2_AUTHBIT_DBX            64
#define  AU_DVB_AUTHBIT_DRA             65
// SRS PureSound
#define  AU_DVB2_AUTHBIT_SRS_PURESND    66
//DTS StudioSound 3D(=TheaterSound3D)
#define  AU_DVB2_AUTHBIT_DTS_StudioSound3D  69  // DTS_StudioSound3D = DTS_StudioSound_I
#define  AU_DVB_AUTHBIT_GAAC            70
#define  AU_DVB_AUTHBIT_DTSNeoUltra     72
#define  AU_DVB2_AUTHBIT_DTSNeoUltra    72
#define  AU_DVB_AUTHBIT_DTSE            73
#define  AU_DVB2_AUTHBIT_DTSE           73
#define  AU_DVB_AUTHBIT_DOLBY_MS12V2_WO_AAC      79
#define  AU_DVB_AUTHBIT_DOLBY_MS11      80
#define  AU_DVB2_AUTHBIT_DOLBY_MS11     80
#define  AU_DVB_AUTHBIT_DOLBY_MS12_B    81
#define  AU_DVB_AUTHBIT_DOLBY_MS12_D    82
#define  AU_DVB_AUTHBIT_DOLBY_MS12_LC   83
#define  AU_DVB2_AUTHBIT_SONICMOTION_ABS3D  92
#define  AU_DVB2_AUTHBIT_BONGIOVI_DPS   93
#define  AU_DVB2_AUTHBIT_NULL           126
#define  AU_DVB_AUTHBIT_DEMOMODE        127
#define  AU_DVB2_AUTHBIT_DOLBY_MS11_WO_AAC       115
#define  AU_DVB2_AUTHBIT_DOLBY_MS12_B_WO_AAC     116
#define  AU_DVB2_AUTHBIT_DOLBY_MS12_D_WO_AAC     117
#define  AU_DVB_AUTHBIT_DOLBY_MS12V1_AC4  121
#define  AU_DVB_AUTHBIT_MPEGH             124
#define  AU_DVB_AUTHBIT_DOLBY_MS12V2      125
#define  AU_DVB2_AUTHBIT_DTS_VIRTUALX     126

#ifdef CONFIG_UTOPIA_PROC_DBG_SUPPORT

#define AUDIO_BUG_PRINT(pHandle, fmt, args...) \
    do { \
        if (pHandle != NULL) \
        { \
            MdbPrint(pHandle, fmt, ## args); \
        } \
        else \
        { \
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_FATAL, fmt, ## args); \
        } \
    } while(0)

#else

#define AUDIO_BUG_PRINT(pHandle, fmt, args...)             AUDIO_PRINT(AUDIO_DEBUG_LEVEL_FATAL, fmt, ## args);

#endif

#define AUDIO_PRINT(level, fmt, args...) \
    do { \
        if ( g_AudioVars2 != NULL) \
        { \
            if ( level <= g_AudioVars2->g_debug_level) \
            { \
                switch(level) \
                { \
                    case AUDIO_DEBUG_LEVEL_FATAL: \
                        ULOGF("[AUDIO][FATAL]", fmt, ## args); \
                        break; \
                    case AUDIO_DEBUG_LEVEL_ERROR: \
                        ULOGE("[AUDIO][ERROR]", fmt, ## args); \
                        break; \
                    case AUDIO_DEBUG_LEVEL_WARN: \
                        ULOGW("[AUDIO][WARN]", fmt, ## args); \
                        break; \
                    case AUDIO_DEBUG_LEVEL_INFO: \
                        ULOGI("[AUDIO][INFO]", fmt, ## args); \
                        break; \
                    case AUDIO_DEBUG_LEVEL_DEBUG: \
                        ULOGD("[AUDIO][DEBUG]", fmt, ## args); \
                        break; \
                    case AUDIO_DEBUG_LEVEL_VERBOSE: \
                        ULOGD("[AUDIO][VERBOSE]", fmt, ## args); \
                        break; \
                    default : \
                        break; \
                } \
            } \
        } \
    } while(0)


//=====================================================
// Structure
//=====================================================


//=====================================================
// Enum
//=====================================================
typedef enum
{
    AU_CHIP_T2,
    AU_CHIP_U3,
    AU_CHIP_T3,
    AU_CHIP_T4,
    AU_CHIP_T7,                         //single DSP
    AU_CHIP_JANUS,                      //single DSP
    AU_CHIP_U4,
    AU_CHIP_T8,
    AU_CHIP_T9,
    AU_CHIP_T10,
    AU_CHIP_T11,
    AU_CHIP_T13,
    AU_CHIP_T12,
    AU_CHIP_J2,
    AU_CHIP_K1,
    AU_CHIP_K2,
    AU_CHIP_A1,
    AU_CHIP_A5,
    AU_CHIP_A6,
    AU_CHIP_A7,
    AU_CHIP_A3,
    AU_CHIP_A5P,
    AU_CHIP_AMETHYST,
    AU_CHIP_EAGLE,
    AU_CHIP_EDISON,
    AU_CHIP_EMERALD,
    AU_CHIP_M12,
    AU_CHIP_EINSTEIN,
    AU_CHIP_EIFFEL,
    AU_CHIP_NIKE,
    AU_CHIP_Euler,
    AU_CHIP_NUGGET,
    AU_CHIP_NIKON,
    AU_CHIP_EINSTEIN3,
    AU_CHIP_NAPOLI,
    AU_CHIP_MONACO,
    AU_CHIP_MADISON,
    AU_CHIP_MIAMI,
    AU_CHIP_CLIPPERS,
    AU_CHIP_MUJI,
    AU_CHIP_KERES,
    AU_CHIP_MUNICH,
    AU_CHIP_MUSTANG,
}AU_CHIP_TYPE;

typedef enum
{
    AUDIO_DSP_SND = 0x00,

    AUDIO_R2_DEC1 = 0x10,
    AUDIO_R2_DEC2 = 0x11,

    AUDIO_R2_SND1 = 0x20,
} AUDIO_IC_TYPE;

typedef enum
{
    AUDIO_ETMODE_DTV,     // DTV Mode
    AUDIO_ETMODE_KTV,     // KTV Mode 1
    AUDIO_ETMODE_GAME,  // GAME Mode
    AUDIO_ETMODE_KTV2,   // KTV Mode 2
    AUDIO_ETMODE_VOIP_ON,
    AUDIO_ETMODE_VOIP_OFF,
    AUDIO_ETMODE_KTV_STB, // STB KTV Mode, forward mix ch.6 and ch.7
}AUDIO_ENTERTAINMENT_MODE;

// for dvb2 decoder command
typedef enum
{
    AU_DVB2_DECCMD_STOP,                //0
    AU_DVB2_DECCMD_PLAY,                //1
    AU_DVB2_DECCMD_PLAYFILETSP          = 2,
    AU_DVB2_DECCMD_RESYNC,
    AU_DVB2_DECCMD_PLAYFILE             = 4,
    AU_DVB2_DECCMD_PAUSE                = 6
}AU_DVB2_DECCMD;

// for dvb2 free run mode
typedef enum
{
    AU_DVB2_FreeRunMode_AVsync,         //0
    AU_DVB2_FreeRunMode_FreeRun,        //1
}AU_DVB2_FreeRunMode;

typedef enum
{
    DSP_MEM_TYPE_PM,                    // DSP PM memory
    DSP_MEM_TYPE_DM                     // DSP DM memory
}AUDIO_DSP_MEMORY_TYPE;

typedef enum
{
    AUDIO_DTV_DVB,
    AUDIO_DTV_ATSC,
    AUDIO_DTV_ISDB,
    AUDIO_DTV_DMBT,
    AUDIO_DTV_OTHER = 15
}AUDIO_DTV_SYSTEM;

typedef enum
{
    AUDIO_MULTICH_PCM_DMXL,
    AUDIO_MULTICH_PCM_DMXR,
    AUDIO_MULTICH_PCM_L,
    AUDIO_MULTICH_PCM_C,
    AUDIO_MULTICH_PCM_R,
    AUDIO_MULTICH_PCM_LS,
    AUDIO_MULTICH_PCM_RS,
    AUDIO_MULTICH_PCM_LFE,
    AUDIO_MULTICH_PCM_LRS,
    AUDIO_MULTICH_PCM_RRS,
}AUDIO_MULTICH_PCM_TYPE;

typedef enum
{
    TRANSCODE_OWNER_DEC_R2,
    TRANSCODE_OWNER_SND_R2,
    TRANSCODE_OWNER_SND_DSP,
    /* add new enum above here !!! */
    TRANSCODE_OWNER_UNSPECIFIED = 0xFF,
} AUDIO_ALWAYS_TRANSCODE_OWNER;

typedef enum
{
    TRANSCODE_TYPE_DD,
    TRANSCODE_TYPE_DDP,
    TRANSCODE_TYPE_DTS,
    /* add new enum above here !!! */
    TRANSCODE_TYPE_UNSPECIFIED = 0xFF,
} AUDIO_ALWAYS_TRANSCODE_TYPE;

typedef enum
{
    E_AUDIO_PCM_RENDER_DECODER_AUTO,
    E_AUDIO_PCM_RENDER_MUTLICHANNEL,
    E_AUDIO_PCM_RENDER_STEREO,
    E_AUDIO_PCM_RENDER_TRANSCODE_6CH,
}E_AUDIO_PCM_RENDER_MODE;

typedef enum
{
    /* Set Cmd */
    AUDIO_ENCODER_IOCTL_SET_READ_PTR_OFFSET,

    /* Get Cmd */
    AUDIO_ENCODER_IOCTL_GET_WRITE_PTR_OFFSET,
    AUDIO_ENCODER_IOCTL_GET_THRESHOLD,
    AUDIO_ENCODER_IOCTL_GET_FRAME_SIZE,
    AUDIO_ENCODER_IOCTL_GET_AVAILABLE_SIZE,
    AUDIO_ENCODER_IOCTL_GET_FRAME_COUNT,
    AUDIO_ENCODER_IOCTL_GET_TIMESTAMP_HIGH32BIT,
    AUDIO_ENCODER_IOCTL_GET_TIMESTAMP_LOW32BIT,
}AUDIO_ENCODER_IOCTL_CMD;

typedef enum
{
    /* Set Cmd */
    AUDIO_CHANNEL_IOCTL_SET_MIXING_GROUP,
    AUDIO_CHANNEL_IOCTL_SET_BINDING_DECODER,
    AUDIO_CHANNEL_IOCTL_SET_BINDING_PCMIO,
    AUDIO_CHANNEL_IOCTL_SET_BINDING_HDMI_IN,
    AUDIO_CHANNEL_IOCTL_SET_BINDING_LINE_IN,
    AUDIO_CHANNEL_IOCTL_SET_BINDING_SPDIF_IN,

    /* Get Cmd */
    AUDIO_CHANNEL_IOCTL_GET_MIXING_GROUP,
    AUDIO_CHANNEL_IOCTL_GET_BINDING_DECODER,
    AUDIO_CHANNEL_IOCTL_GET_BINDING_PCMIO,
    AUDIO_CHANNEL_IOCTL_GET_BINDING_HDMI_IN,
    AUDIO_CHANNEL_IOCTL_GET_BINDING_LINE_IN,
    AUDIO_CHANNEL_IOCTL_GET_BINDING_SPDIF_IN,
}AUDIO_CHANNEL_IOCTL_CMD;

typedef enum
{
    AUDIO_HASHKEY_GROUP1,
    AUDIO_HASHKEY_GROUP2,
}AUDIO_HASHKEY_GROUP;

typedef enum
{
    AUDIO_HASHKEY_DOLBY_VERSION_NONE = 0,
    AUDIO_HASHKEY_DOLBY_VERSION_MS10,
    AUDIO_HASHKEY_DOLBY_VERSION_MS11,
    AUDIO_HASHKEY_DOLBY_VERSION_MS12V1,
    AUDIO_HASHKEY_DOLBY_VERSION_MS12V22,
} AUDIO_HASHKEY_DOLBY_VERSION;

typedef enum
{
    AUDIO_GROUP_MISX = 0,
    AUDIO_GROUP_1,
} AUDIO_GROUP;

typedef enum
{
    AUDIO_SOUND_GET_PARAM_SPECTRUM_SIZE,
    AUDIO_SOUND_GET_PARAM_SPECTRUM,
}AUDIO_SOUND_GET_PARAM_CMD;

typedef enum
{
    AUDIO_SOUND_EFFECT_SET_PARAM_PEQ,
    AUDIO_SOUND_EFFECT_SET_PARAM_DRC,
}AUDIO_SOUND_EFFECT_SET_PARAM_CMD;

typedef struct
{
    MS_S32 (*open)(void *pData);
    MS_S32 (*close)(void);
    MS_S32 (*start)(void);
    MS_S32 (*stop)(void);
    MS_S32 (*set)(MS_U32 u32Cmd, const void *pData);
    MS_S32 (*get)(MS_U32 u32Cmd, void *pData);
    MS_U32 (*read)(void *pBuf, MS_U32 u32Size);
    MS_U32 (*write)(const void *pBuf, MS_U32 u32Size);
    MS_S32 (*flush)(void);
} AUDIO_PCM_OPS_t;

typedef struct
{
    MS_U8 u8MultiChFlag;
    MS_U8 u8MixingFlag;
    MS_U8 u8CaptureFlag;
    MS_U32 u32Channel[16];
    MS_U32 u32SampleRate[32];
    MS_U32 u32BitWidth[8];
} AUDIO_PCM_CAPS_t;

typedef struct
{
    MS_S8 *pBuffer;
    MS_S8 *pReadPtr;
    MS_S8 *pWritePtr;
    MS_U32 u32BufferSize;
    MS_U32 u32BufferHighThreshold;
    MS_U32 u32RemainSize;
} AUDIO_PCM_BUFFER_INFO_t;

typedef struct
{
    MS_U8 u8Name[AUDIO_COMMON_PCM_IO_NAME_LENGTH]; /* PCM name (only 32 bytes, if change this length, must change u8Name of AUDIO_PCM_INFO_t as well!) */
    AUDIO_PCM_OPS_t tPcmOps;  /* PCM operators */
    AUDIO_PCM_CAPS_t tPcmCaps;  /* PCM operators */
    AUDIO_PCM_BUFFER_INFO_t tPcmBufferInfo;  /* PCM buffer information */
    AUDIO_PCM_INFO_t *pPcmInfo; /* PCM information */
    MS_U8 u8SettingChangeFlag;
    MS_U32 u32RemainSize;
    MS_U32 u32UpdateTime;
} AUDIO_PCM_t;

typedef struct
{
    MS_S32 (*open)(void);
    MS_S32 (*close)(void);
    MS_S32 (*start)(void);
    MS_S32 (*stop)(void);
    MS_S32 (*ioctl)(MS_U32 u32Cmd, void *pData);
    MS_U32 (*read)(void *pBuf, MS_U32 u32Size);
    MS_S32 (*flush)(void);
} AUDIO_ENCODER_OPS_t;

typedef struct
{
    MS_U8 u8SupportedFlag; /* it is TRUE when this channel is supported */
    MS_U8 u8ConnectFlag; /* it is TRUE when opened */
    MS_U8 strInputName[32]; /* the name of input source type */
    MS_S32 s32InputId; /* the ID of binding device */
    MS_U16 u16ConfigAddr; /* the address to configure setting */
} AUDIO_CHANNEL_INFO_t;

typedef struct __attribute__((packed))
{
    MS_BOOL          bDecInUsed;
    AUDIO_USAGE_TYPE eUsageType;
    MMA_AudioType    eAudioType;
} AUDIO_DSP_USAGE;

#if AUDIO_ADVSND_AUTO_RESTORE
typedef enum
{
    //SRS_TSXT
    SRS_TSXT_TRUBASS_BACK_UP,
    SRS_TSXT_DC_BACK_UP,

    //SRS_TSHD
    SRS_TSHD_TRUBASS_BACK_UP,
    SRS_TSHD_DC_BACK_UP,
    SRS_TSHD_DEFINITION_BACK_UP,
    SRS_TSHD_SRS3D_BACK_UP,

    //SRS_THEATERSOUND
    SRS_THEATERSOUND_TSHD_BACK_UP,
    SRS_THEATERSOUND_TRUBASS_BACK_UP,
    SRS_THEATERSOUND_DC_BACK_UP,
    SRS_THEATERSOUND_DEFINITION_BACK_UP,
    SRS_THEATERSOUND_TRUVOLUME_BACK_UP,
    SRS_THEATERSOUND_HARDLIMITER_BACK_UP,
    SRS_THEATERSOUND_HPF_BACK_UP,
    SRS_THEATERSOUND_TRUEQ_BACK_UP,
    SRS_THEATERSOUND_TRUVOLUME_NOISE_MNGR_BACK_UP,

    //DTS_ULTRATV
    DTS_ULTRATV_EVO_BACK_UP,
    DTS_ULTRATV_SYM_BACK_UP,

    //AUDYSSEY
    AUDYSSEY_DYNAMICVOL_BACK_UP,
    AUDYSSEY_DYNAMICEQ_BACK_UP,
    AUDYSSEY_EQ_BACK_UP,
    AUDYSSEY_ABX_BACK_UP,

    SRS_THEATERSOUND_TSHD_SURR_BACK_UP,
    SRS_THEATERSOUND_TSHD_LEVEL_INDPT_BACK_UP,
    SRS_THEATERSOUND_CS_BACK_UP,
    SRS_THEATERSOUND_TRUDIALOG_BACK_UP,
    SRS_THEATERSOUND_TRUVOLUME_NORMALIZER_BACK_UP,
    SRS_THEATERSOUND_TRUVOLUME_SMOOTH_BACK_UP,
    SRS_THEATERSOUND_HPF_END_BACK_UP,
    SRS_THEATERSOUND_TOTAL_BACK_UP,

    SRS_PURESOUND_HL_BACK_UP,
    SRS_PURESOUND_AEQ_BACK_UP,
    SRS_PURESOUND_HPF_BACK_UP,
    SRS_PURESOUND_TBHD_BACK_UP,
    SRS_THEATERSOUND_CC3D_EN_BACK_UP,
    SRS_THEATERSOUND_CC3D_DEPTH_PROCESS_EN_BACK_UP,
    SRS_THEATERSOUND_CC3D_3D_SURR_BOOST_EN_BACK_UP,
    SRS_THEATERSOUND_CC3D_FADE_EN_BACK_UP,
    SRS_THEATERSOUND_CC3D_TSHD_MIX_EN_BACK_UP,
    SRS_THEATERSOUND_CC3D_TBHDX_EN_BACK_UP,
    SRS_THEATERSOUND_GEQ_EN_BACK_UP,
    SRS_THEATERSOUND_3D_EN_BACK_UP,

    // BONGIOVI DPS
    BONGIOVI_DPS_SURR_EN_BACK_UP,

    // SONICEMOTOIN_ABS3D
    SONICEMOTION_ABS3D_EN_BACK_UP,

    // DOLBY_DAP
    DOLBY_DAP_EN_BACK_UP,

    ADV_SND_SUB_MODULE_BACK_UP_NUM,
} ADVFUNC_BACK_UP;

typedef enum
{
    //BBE
    BBE_MODE_SET_BACK_UP,

    //DOLBY
    DOLBY_PL2VDPK_SMOD_BACK_UP,
    DOLBY_PL2VDPK_WMOD_BACK_UP,

    //SRS TSXT
    SRS_TSXT_SET_INPUT_GAIN_BACK_UP,
    SRS_TSXT_SET_DC_GAIN_BACK_UP,
    SRS_TSXT_SET_TRUBASS_GAIN_BACK_UP,
    SRS_TSXT_SET_SPEAKERSIZE_BACK_UP,
    SRS_TSXT_SET_INPUT_MODE_BACK_UP,
    SRS_TSXT_SET_OUTPUT_GAIN_BACK_UP,
    SRS_TSXT_RESERVE1_BACK_UP,
    SRS_TSXT_RESERVE2_BACK_UP,
    SRS_TSXT_RESERVE3_BACK_UP,

    //SRS TSHD in SE-DSP
    SRS_TSHD_SET_INPUT_MODE_BACK_UP,
    SRS_TSHD_SET_OUTPUT_MODE_BACK_UP,
    SRS_TSHD_SET_SPEAKERSIZE_BACK_UP,
    SRS_TSHD_SET_TRUBASS_CONTROL_BACK_UP,
    SRS_TSHD_SET_DEFINITION_CONTROL_BACK_UP,
    SRS_TSHD_SET_DC_CONTROL_BACK_UP,
    SRS_TSHD_SET_SURROUND_LEVEL_BACK_UP,
    SRS_TSHD_SET_INPUT_GAIN_BACK_UP,
    SRS_TSHD_SET_WOWSPACE_CONTROL_BACK_UP,
    SRS_TSHD_SET_WOWCENTER_CONTROL_BACK_UP,
    SRS_TSHD_SET_WOWHDSRS3DMODE_BACK_UP,
    SRS_TSHD_SET_LIMITERCONTROL_BACK_UP,
    SRS_TSHD_SET_OUTPUT_GAIN_BACK_UP,
    SRS_TSHD_RESERVE1_BACK_UP,
    SRS_TSHD_RESERVE2_BACK_UP,
    SRS_TSHD_RESERVE3_BACK_UP,

    //SRS_THEATERSOUND
    SRS_THEATERSOUND_INPUT_GAIN_BACK_UP,
    SRS_THEATERSOUND_DEFINITION_CONTROL_BACK_UP,
    SRS_THEATERSOUND_DC_CONTROL_BACK_UP,
    SRS_THEATERSOUND_TRUBASS_CONTROL_BACK_UP,
    SRS_THEATERSOUND_SPEAKERSIZE_BACK_UP,
    SRS_THEATERSOUND_HARDLIMITER_LEVEL_BACK_UP,
    SRS_THEATERSOUND_HARDLIMITER_BOOST_GAIN_BACK_UP,
    SRS_THEATERSOUND_HEADROOM_GAIN_BACK_UP,
    SRS_THEATERSOUND_TRUVOLUME_MODE_BACK_UP,
    SRS_THEATERSOUND_TRUVOLUME_REF_LEVEL_BACK_UP,
    SRS_THEATERSOUND_TRUVOLUME_MAX_GAIN_BACK_UP,
    SRS_THEATERSOUND_TRUVOLUME_NOISE_MNGR_THLD_BACK_UP,
    SRS_THEATERSOUND_TRUVOLUME_CALIBRATE_BACK_UP,
    SRS_THEATERSOUND_TRUVOLUME_INPUT_GAIN_BACK_UP,
    SRS_THEATERSOUND_TRUVOLUME_OUTPUT_GAIN_BACK_UP,
    SRS_THEATERSOUND_HPF_FC_BACK_UP,
    SRS_THEATERSOUND_TSHD_INPUT_GAIN_BACK_UP,
    SRS_THEATERSOUND_TSHD_OUTPUT_GAIN_BACK_UP,
    SRS_THEATERSOUND_SURR_LEVEL_CONTROL_BACK_UP,
    SRS_THEATERSOUND_TRUBASS_COMPRESSOR_CONTROL_BACK_UP,
    SRS_THEATERSOUND_TRUBASS_PROCESS_MODE_BACK_UP,
    SRS_THEATERSOUND_TRUBASS_SPEAKER_AUDIO_BACK_UP,
    SRS_THEATERSOUND_TRUBASS_SPEAKER_ANALYSIS_BACK_UP,
    SRS_THEATERSOUND_OUTPUT_GAIN_BACK_UP,
    SRS_THEATERSOUND_BYPASS_GAIN_BACK_UP,
    SRS_THEATERSOUND_INPUT_MODE_BACK_UP,
    SRS_THEATERSOUND_TRUVOLUME_BYPASS_GAIN_BACK_UP,
    SRS_THEATERSOUND_CS_INPUT_GAIN_BACK_UP,

    //DTS_ULTRATV
    DTS_ULTRATV_EVO_MONOINPUT_BACK_UP,
    DTS_ULTRATV_EVO_WIDENINGON_BACK_UP,
    DTS_ULTRATV_EVO_ADD3DBON_BACK_UP,
    DTS_ULTRATV_EVO_PCELEVEL_BACK_UP,
    DTS_ULTRATV_EVO_VLFELEVEL_BACK_UP,
    DTS_ULTRATV_RESERVE1_BACK_UP,
    DTS_ULTRATV_RESERVE2_BACK_UP,
    DTS_ULTRATV_RESERVE3_BACK_UP,
    DTS_ULTRATV_RESERVE4_BACK_UP,
    DTS_ULTRATV_RESERVE5_BACK_UP,
    DTS_ULTRATV_RESERVE6_BACK_UP,

    DTS_ULTRATV_SYM_DEFAULT_BACK_UP,
    DTS_ULTRATV_SYM_MODE_BACK_UP,
    DTS_ULTRATV_SYM_LEVEL_BACK_UP,
    DTS_ULTRATV_SYM_RESET_BACK_UP,
    DTS_ULTRATV_RESERVE7_BACK_UP,
    DTS_ULTRATV_RESERVE8_BACK_UP,
    DTS_ULTRATV_RESERVE9_BACK_UP,

    //AUDYSSEY
    AUDYSSEY_DYNAMICVOL_COMPRESS_MODE_BACK_UP,
    AUDYSSEY_DYNAMICVOL_GC_BACK_UP,
    AUDYSSEY_DYNAMICVOL_VOLSETTING_BACK_UP,
    AUDYSSEY_DYNAMICEQ_EQOFFSET_BACK_UP,
    AUDYSSEY_ABX_GWET_BACK_UP,
    AUDYSSEY_ABX_GDRY_BACK_UP,
    AUDYSSEY_ABX_FILSET_BACK_UP,
    AUDYSSEY_RESERVE1_BACK_UP,
    AUDYSSEY_RESERVE2_BACK_UP,
    AUDYSSEY_RESERVE3_BACK_UP,
    AUDYSSEY_RESERVE4_BACK_UP,
    AUDYSSEY_RESERVE5_BACK_UP,
    AUDYSSEY_RESERVE6_BACK_UP,

    //DBX
    DBX_MODE_SET_BACK_UP,
    DBX_LOAD_PM_BACK_UP,
    DBX_LOAD_DM_BACK_UP,
    DBX_COPY_TotSonDM_BACK_UP,
    DBX_COPY_TotSonPM_BACK_UP,
    DBX_COPY_TotVolDM_BACK_UP,
    DBX_COPY_TotVolPM_BACK_UP,
    DBX_COPY_TotSurDM_BACK_UP,
    DBX_COPY_TotSurPM_BACK_UP,
    OTHERS_RESERVE9_BACK_UP,
    OTHERS_RESERVE10_BACK_UP,
    OTHERS_RESERVE11_BACK_UP,
    OTHERS_RESERVE12_BACK_UP,
    OTHERS_RESERVE13_BACK_UP,
    OTHERS_RESERVE14_BACK_UP,
    OTHERS_RESERVE15_BACK_UP,
    OTHERS_RESERVE16_BACK_UP,
    OTHERS_RESERVE17_BACK_UP,
    OTHERS_RESERVE18_BACK_UP,
    OTHERS_RESERVE19_BACK_UP,
    OTHERS_RESERVE20_BACK_UP,
    OTHERS_RESERVE21_BACK_UP,
    OTHERS_RESERVE22_BACK_UP,
    OTHERS_RESERVE23_BACK_UP,
    OTHERS_RESERVE24_BACK_UP,
    OTHERS_RESERVE25_BACK_UP,
    OTHERS_RESERVE26_BACK_UP,
    OTHERS_RESERVE27_BACK_UP,
    OTHERS_RESERVE28_BACK_UP,
    OTHERS_RESERVE29_BACK_UP,
    OTHERS_RESERVE30_BACK_UP,

    SRS_THEATERSOUND_CS_PROCESS_MODE_BACK_UP,
    SRS_THEATERSOUND_CS_LR_OUTPUT_GAIN_BACK_UP,
    SRS_THEATERSOUND_CS_LsRs_OUTPUT_GAIN_BACK_UP,
    SRS_THEATERSOUND_CS_Center_OUTPUT_GAIN_BACK_UP,
    SRS_THEATERSOUND_TRUDIALOG_INPUT_GAIN_BACK_UP,
    SRS_THEATERSOUND_TRUDIALOG_OUTPUT_GAIN_BACK_UP,
    SRS_THEATERSOUND_TRUDIALOG_BYPASS_GAIN_BACK_UP,
    SRS_THEATERSOUND_TRUDIALOG_PROCESS_GAIN_BACK_UP,
    SRS_THEATERSOUND_TRUDIALOG_CLARITY_GAIN_BACK_UP,
    SRS_THEATERSOUND_TRUVOLUME_NORMALIZE_THRESH_BACK_UP,
    SRS_THEATERSOUND_HPF_CUTOFF_BACK_UP,
    SRS_THEATERSOUND_HPF_END_CUTOFF_BACK_UP,

    //SRS PURESOUND
    SRS_PURESOUND_HL_INPUT_GAIN_BACK_UP,
    SRS_PURESOUND_HL_OUTPUT_GAIN_BACK_UP,
    SRS_PURESOUND_HL_BYPASS_GAIN_BACK_UP,
    SRS_PURESOUND_HL_LIMITERBOOST_BACK_UP,
    SRS_PURESOUND_HL_HARDLIMIT_BACK_UP,
    SRS_PURESOUND_HL_DELAYLEN_BACK_UP,
    SRS_PURESOUND_AEQ_INPUT_GAIN_BACK_UP,
    SRS_PURESOUND_AEQ_OUTPUT_GAIN_BACK_UP,
    SRS_PURESOUND_AEQ_BYPASS_GAIN_BACK_UP,
    SRS_PURESOUND_FILTER_CONFIG_BACK_UP,
    SRS_PURESOUND_HPF_FREQUENCY_BACK_UP,
    SRS_PURESOUND_TBHD_TRUBASS_LEVEL_BACK_UP,
    SRS_PURESOUND_TBHD_SPEAKER_SIZE_BACK_UP,
    SRS_PURESOUND_TBHD_LEVEL_INDEPENDENT_EN_BACK_UP,
    SRS_PURESOUND_TBHD_COMPRESSOR_LEVEL_BACK_UP,
    SRS_PURESOUND_TBHD_MODE_BACK_UP,
    SRS_PURESOUND_TBHD_SPEAKER_AUDIO_BACK_UP,
    SRS_PURESOUND_TBHD_SPEAKER_ANALYSIS_BACK_UP,
    SRS_PURESOUND_INPUT_GAIN_BACK_UP,
    SRS_PURESOUND_OUTPUT_GAIN_BACK_UP,

    //SRS_THEATERSOUND_3D
    SRS_THEATERSOUND_CC3D_INPUT_GAIN_BACK_UP,
    SRS_THEATERSOUND_CC3D_OUTPUT_GAIN_BACK_UP,
    SRS_THEATERSOUND_CC3D_BYPASS_GAIN_BACK_UP,
    SRS_THEATERSOUND_CC3D_APERTURE_BACK_UP,
    SRS_THEATERSOUND_CC3D_GAINLIMIT_BACK_UP,
    SRS_THEATERSOUND_CC3D_FF_DEPTH_BACK_UP,
    SRS_THEATERSOUND_CC3D_NF_DEPTH_BACK_UP,

    SRS_THEATERSOUND_CC3D_TSHD_SURR_MODE_BACK_UP,
    SRS_THEATERSOUND_CC3D_TSHD_MIX_FADE_CTRL_BACK_UP,

    SRS_THEATERSOUND_CC3D_TBHDX_INPUT_GAIN_BACK_UP,
    SRS_THEATERSOUND_CC3D_TBHDX_BASSLEVEL_BACK_UP,
    SRS_THEATERSOUND_CC3D_TBHDX_SPEAKERSIZE_BACK_UP,
    SRS_THEATERSOUND_CC3D_TBHDX_MODE_BACK_UP,
    SRS_THEATERSOUND_CC3D_TBHDX_DYNAMICS_BACK_UP,
    SRS_THEATERSOUND_CC3D_TBHDX_HP_ORDER_BACK_UP,
    SRS_THEATERSOUND_CC3D_TBHDX_CUSTOM_FILTER_BACK_UP,
    SRS_THEATERSOUND_GEQ_INPUT_GAIN_BACK_UP,
    SRS_THEATERSOUND_GEQ_BAND0_GAIN_BACK_UP,
    SRS_THEATERSOUND_GEQ_BAND1_GAIN_BACK_UP,
    SRS_THEATERSOUND_GEQ_BAND2_GAIN_BACK_UP,
    SRS_THEATERSOUND_GEQ_BAND3_GAIN_BACK_UP,
    SRS_THEATERSOUND_GEQ_BAND4_GAIN_BACK_UP,
    SRS_THEATERSOUND_CC3D_PROCESS_PATH_BACK_UP,
    SRS_THEATERSOUND_TSHD_BYPASS_GAIN_BACK_UP,

    //Dolby Volume
    DV258_ENABLE_BACK_UP,
    DV258_LCL_ENABLE_BACK_UP,
    DV258_LCL_LEVELER_ENABLE_BACK_UP,
    DV258_LCL_MODELER_ENABLE_BACK_UP,
    DV258_LCL_MODELER_HALF_MODE_BACK_UP,
    DV258_LCL_AMOUNT_BACK_UP,
    DV258_LCL_AVM_BACK_UP,
    DV258_LCL_DVM_BACK_UP,
    DV258_LCL_IRL_BACK_UP,
    DV258_LCL_ORL_BACK_UP,
    DV258_LCL_PREGAIN_BACK_UP,
    DV258_LCL_GAIN_BACK_UP,

    DV258_LIMITER_ENABLE_BACK_UP,
    DV258_LIMITER_PEAK_BACK_UP,
    DV258_LIMITER_BOOST_BACK_UP,
    DV258_LIMITER_RMS_BACK_UP,

    // BONGIOVI_DPS
    BONGIOVI_DPS_SET_PARAMS_BACK_UP,
    BONGIOVI_DPS_SET_GLOBAL_INPUT_GAIN_BACK_UP,
    BONGIOVI_DPS_SET_GLOBAL_OUTPUT_GAIN_BACK_UP,
    BONGIOVI_DPS_SET_STEREO_WIDENER_MGAIN_BACK_UP,
    BONGIOVI_DPS_SET_STEREO_WIDENER_SGAIN_BACK_UP,

    // SONICEMOTION ABS3D
    SONICEMOTION_ABS3D_SET_PARAMS_BACK_UP,

    // DOLBY_DAP
    DOLBY_DAP_SET_PARAMS_BACK_UP,

    // DBX
    DBXTV_SET_MODE_BACK_UP,

    // SRS PURESOUND
    SRS_PURESOUND_AEQ_COEFFICIENT_BACK_UP,

    ADV_PARAM_BACK_UP_NUM,
} ADVSND_PARAM_BACK_UP;


//================================================================
//  Structure
//================================================================

typedef struct
{
    const char *name;
    MS_U32 value;
}StringToEnum;

typedef struct
{
    ADVSND_PARAM          AdvParamType;
    MS_U32                u32Size;
    void*                 pParam_ptr;
} ADV_SND_PARAM_TABLE_t;

typedef struct
{
    ADVFUNC               eProc;
    MS_BOOL               bEnable;
    MS_BOOL               bSet_flag;
} ADV_SND_SUB_MODULE_t;

typedef struct
{
    ADVSND_PARAM          AdvParamType;
    MS_BOOL               bSet_flag;
    MS_U32                u32Value1;
    MS_U16                u16Value2;
} ADV_SND_PARAM_t;

typedef struct
{
    MS_U8                  type[AUDIO_STRNAME_LENGTH];
    AUDIO_DSP_CODE_TYPE    u8AdvSndEffType;
    ADV_SND_SUB_MODULE_t   stAdvSndSubModule[ADV_SND_SUB_MODULE_BACK_UP_NUM];
    ADV_SND_PARAM_t        stAdvSndParam[ADV_PARAM_BACK_UP_NUM];
    ADV_SND_PARAM_TABLE_t  stAdvSndParamTable;
} Audio_ADV_SND_BackUp_t;
#endif

typedef struct
{
    MS_U8 u8MultiChannelVol[2]; /* for Channel 1 ~ 4, [0] : vol1, [1] : vol2 */
    MS_U8 u8PcmCapture1Vol[2];  /* PCM Capture1, [0] : vol1, [1] : vol2 */
    MS_U8 u8PcmCapture2Vol[2];  /* PCM Capture2, [0] : vol1, [1] : vol2 */
    MS_U8 u8PcmCapture3Vol[2];  /* PCM Capture3, [0] : vol1, [1] : vol2 */
} Audio_FW_VOLUME_BACKUP_t;

typedef struct
{
    Audio_FW_VOLUME_BACKUP_t tFwVolumeBackup;
} Audio_STR_INFO_BACKUP_t;

typedef struct
{
    MS_U32  g_ShmID;

    MS_BOOL g_bDecPlayFileFlag;   // DEC MM interrupt flag
    MS_BOOL g_bSePlayFileFlag;    // SE  MM interrupt flag
    MS_BOOL g_bMMPlayFlag;        // MM  play flag
    MS_BOOL g_Audio_PreInitFlag;
    MS_BOOL g_Audio_InitFlag;
    MS_BOOL g_SPDIF_MuteStatus;
    MS_BOOL g_ARC_MuteStatus;
    AUDIO_SPDIF_OUTPUT_TYPE g_eUserSpdifMode;
    AUDIO_SPDIF_OUTPUT_TYPE g_eSpdifMode;
    AUDIO_SPDIF_OUTPUT_TYPE g_eHdmiArcOutputMode;
    HDMI_TX_OUTPUT_TYPE g_eHdmiTxOutputMode;
    HDMI_TX_OUTPUT_TYPE g_eUserHdmiTxOutputMode;
    MS_U8   g_Pre_R2_NonPcm_Sel;
    MS_U8   g_SpdifNpcmPath;
    MS_U8   g_PreMainDecodeId;
    AUDIO_DSP_CODE_TYPE g_PreMainDspCodeType;

    MS_BOOL g_dolbyLoudness_Enable;
    MS_U8   g_dolbyLoudness_DTVsystem;

    AUDIO_DSP_CODE_TYPE g_DspCodeType;
    AUDIO_DSP_CODE_TYPE g_Dsp2CodeType;

    MS_U16  u16DecodeFrmCnt[AU_DEC_MAX];

    AUDIO_DSP_CODE_TYPE g_AdvSndEffDspCodeType;
    MS_U8   g_u8DspAliveFlag;
    MS_U8   g_u8AudioCrashFlag;

    MS_U8   g_u8SifDspType;
    MS_U16  sif_gain_0[6];
    MS_U16  sif_shift_0[6];

    MS_U8   g_u8IntTag;   // DEC MM interrupt flag
    MS_U16  int_mb_cnt;

    MS_U8   CompressInfo;
    MS_U8   g_DSPMadMIUBank[MAX_AUDIO_BASE_ADDRESS_NUM2];
    MS_PHY  g_DSPBinBaseAddress[MAX_AUDIO_BASE_ADDRESS_NUM2];
    MS_PHY  g_DSPMadBaseBufferAdr[MAX_AUDIO_BASE_ADDRESS_NUM2];
    MS_PHY  g_DSPDDRBinBaseAddress[MAX_AUDIO_BASE_ADDRESS_NUM2];

    MS_U32  g_u32ClientCounter;

    AUDIO_SOURCE_INFO_TYPE eMainSourceType;
    AUDIO_SOURCE_INFO_TYPE ePrevSourceType;
    AUDIO_DSP_USAGE        DspUsage[AU_DEC_MAX];
    AudioDecStatus_t            AudioDecStatus[AU_DEC_MAX];

    AUDIO_PATH_TYPE     eMainAudioPath;

    AUDIO_SOURCE_INFO_TYPE eAudioSource;
    AUDIO_INPUT_TYPE       eMainAudioSource;
    AUDIO_INPUT_TYPE       eSubAudioSource;
    AUDIO_INPUT_TYPE       eScartAudioSource;

    AUDIO_INIT_INFO        AudioSystemInfo;
    AUDIO_OUT_INFO         AudioOutputInfo;             // Store the connectivity of audio output device
    AUDIO_PATH_INFO        AudioPathInfo;               // Store the connectivity of audio DSP & output path
    OUTPUT_SOURCE_INFO     AudioOutputSourceInfo;       // Store the output port source setting

    DigitalOutSetting_t stDigitalOutSetting[DIGITAL_MAX_OUTPUT];
    Digital_Out_Channel_Status_t stDigitalOutChannelStatus[DIGITAL_MAX_OUTPUT];

    MS_U16  mainAudioDelay;
    MS_U8   ChipRevision;

    AUDIO_CAPABILITIES AudioCaps[AU_DEC_MAX];
    Digital_Out_Device_Capability_t stDigitalOutCodecCapability[DIGITAL_MAX_OUTPUT][CODEC_MAX];

    MS_BOOL DolbyAACFlag;
    MS_BOOL DolbyEncFlag;

    AUDIO_PCM_INFO_t g_PcmInfo[AUDIO_PCM_MAXNUM];

    AUDIO_DEBUG_LEVEL g_debug_level;

    MS_U8   g_R2_NonPcm_Sel;
    MS_U8   g_DolbyDAPEnable;
    AUDIO_HASHKEY_DOLBY_VERSION g_eHashKeyDolbyVersion;

    AUDIO_OUTPUT_DEVICE_SELECTION_t g_AudioOutputDeviceSelection;

    AUDIO_SPECIFIED_DIGITAL_OUTPUT_t g_AudioSpecifiedDigitalOutput;

    MS_BOOL  g_hbr_bypass_enable;

    MS_U8   g_u8MADSkipResetFlag;
    MS_U8   g_u8MAD2SkipResetFlag;

    MS_BOOL  g_R2ShmInitFlag;

    MS_U8   g_HDMINonPcmPath;

    MS_BOOL g_bHashkeyFlag;

    DOLBY_DRC_MODE g_dolbyDrcMode;
    AUDIO_INPUT_TYPE AudioSrcType;
    AUDIO_INPUT_TYPE AudioSubSrcType;
    MS_U8   g_ddenc_owner;    // 0:R2 1:DSP

    DIGITAL_OUTPUT_TYPE g_EdidOutputInterfaceType;
    MS_BOOL g_EdidOutViaArc;
    MS_U8   g_u8AdvSndEffType;
    MS_U8   g_dolby_truehd_enable;

    MS_BOOL g_HDMIRx2TxHwPTFlag; //HDMI Rx to Tx HW Pass through Flag
    MS_BOOL g_HDMIRxHBRFlag; //HDMI Rx High Bit Rate Flag

    AUDIO_DSP_CODE_TYPE g_eSifCodeType;
    AUDIO_DSP_CODE_TYPE g_eDecoder1Type;
    AUDIO_DSP_CODE_TYPE g_eDecoder2Type;

    AUDIO_ALWAYS_TRANSCODE_OWNER g_AlwaysEncOwner;
    AUDIO_ALWAYS_TRANSCODE_TYPE g_AlwaysEncType;
    MS_BOOL g_AlwaysEnc_enable;
    E_AUDIO_PCM_RENDER_MODE g_ePcmRenderControl;

    //for procfs
    MS_U8   g_u8DecocderID;
    MS_U16  g_u16R2LogDbgOption;
    MS_BOOL g_bDumpR2LogMonitorEnable;
    MS_BOOL g_bDumpR2Log;
    MS_U16  g_u16Orignal_REG_BANK_R2_LOG_DBG;
    MS_U32  g_u32DumpR2LogMonitorInterval; //ms
    char    *g_pR2LogFilePath;
    MS_U32  g_u32MaskValue;
    MS_U32  g_u32RegValue;

    //Previous Digital OUTPUT_CODEC for HAL_AUDIO_SPDIF_ApplySetting
    DIGITAL_OUTPUT_SPECIFIED_CODEC g_PreSpdifOutputCodec;
    DIGITAL_OUTPUT_SPECIFIED_CODEC g_PreHdmiArcOutputCodec;
    DIGITAL_OUTPUT_SPECIFIED_CODEC g_PreHdmiTxOutputCodec;

#if AUDIO_ADVSND_AUTO_RESTORE
    Audio_ADV_SND_BackUp_t g_ADV_SND_Backup;
#endif

    MS_U32 g_FrameDecStatus[AU_DEC_MAX];

    MS_BOOL g_bDSPLoadCode;

    AUDIO_CHANNEL_INFO_t g_ChannelInfo[AUDIO_CHANNEL_MAXNUM];

    MS_U32  g_u32bPreDTSCD;
    MS_U32  g_u32PreHDMITxSamplingRate;
    MS_U32  g_u32PreSPDIFTxSamplingRate;

    MS_BOOL g_bAutoRecoveryEnable;
    MS_U32  g_dts_drc_percent;

    Audio_STR_INFO_BACKUP_t g_tStrInfoBackup;

    MS_U32 u32PcmLimiterEn;
    MS_U32 u32PcmLimiterThreshold;
    MS_BOOL bTempoEn;
} AUDIO_SHARED_VARS2;

typedef struct
{
    MS_U32 Address;
    MS_U8 Length;
    MS_U16 u16Data[8];
} Audio_MBOX_BackUp_t;

typedef enum
{
    AUDIO_DEBUG_CMD_SPEAKER_MUTE,
    AUDIO_DEBUG_CMD_HEADPHONE_MUTE,
    AUDIO_DEBUG_CMD_LINEOUT_MUTE,
    AUDIO_DEBUG_CMD_SPDIF_MUTE,
    AUDIO_DEBUG_CMD_SPEAKER_VOLUME,
    AUDIO_DEBUG_CMD_HEADPHONE_VOLUME,
    AUDIO_DEBUG_CMD_LINEOUT_VOLUME,
    AUDIO_DEBUG_CMD_SPDIF_VOLUME,
    AUDIO_DEBUG_CMD_SOUNDMODE,
    AUDIO_DEBUG_CMD_BALANCE,
    AUDIO_DEBUG_CMD_GEQ_ENABLE,
    AUDIO_DEBUG_CMD_GEQ_120HZ,
    AUDIO_DEBUG_CMD_GEQ_500HZ,
    AUDIO_DEBUG_CMD_GEQ_1500HZ,
    AUDIO_DEBUG_CMD_GEQ_5KHZ,
    AUDIO_DEBUG_CMD_GEQ_10KHZ,
    AUDIO_DEBUG_CMD_DRC_ENABLE,
    AUDIO_DEBUG_CMD_DRC_THRESHOLD,
    AUDIO_DEBUG_CMD_AVC_ENABLE,
    AUDIO_DEBUG_CMD_AVC_MODE,
    AUDIO_DEBUG_CMD_AVC_AT,
    AUDIO_DEBUG_CMD_AVC_RT,
    AUDIO_DEBUG_CMD_AVC_THRESHOLD,
    AUDIO_DEBUG_CMD_BASSTREBLE_ENABLE,
    AUDIO_DEBUG_CMD_BASS_LEVEL,
    AUDIO_DEBUG_CMD_TREBLE_LEVEL,
    AUDIO_DEBUG_CMD_PEQ_ENABLE,
    AUDIO_DEBUG_CMD_HPF_ENABLE,
    AUDIO_DEBUG_CMD_PRESCALE_LEVEL,
    AUDIO_DEBUG_CMD_SURROUND_ENABLE,
    AUDIO_DEBUG_CMD_SURROUND_A_GAIN,
    AUDIO_DEBUG_CMD_SURROUND_B_GAIN,
    AUDIO_DEBUG_CMD_SURROUND_K_GAIN,
    AUDIO_DEBUG_CMD_SURROUND_LPF,
    AUDIO_DEBUG_CMD_FW_VERSION,
    AUDIO_DEBUG_CMD_SPDIF_MODE,
    AUDIO_DEBUG_CMD_DECODER_NUMBER,
    AUDIO_DEBUG_CMD_SHOW_DECODER_STATUS,
    AUDIO_DEBUG_CMD_SHOW_ALL_DECODER_STATUS,
    AUDIO_DEBUG_CMD_DEC_R2_WFI,
    AUDIO_DEBUG_CMD_DEC_R2_CMD,
    AUDIO_DEBUG_CMD_START_DUMP_R2_LOG,
    AUDIO_DEBUG_CMD_STOP_DUMP_R2_LOG,
    AUDIO_DEBUG_CMD_SHOW_R2_LOG_DBG_OPTION,
    AUDIO_DEBUG_CMD_R2_LOG_DBG_OPTION,
    AUDIO_DEBUG_CMD_R2_LOG_FILE_PATH,
    AUDIO_DEBUG_CMD_R2_LOG_INTERVAL,
    AUDIO_DEBUG_CMD_READ_REGISTER_BANK,
    AUDIO_DEBUG_CMD_MASK_VALUE,
    AUDIO_DEBUG_CMD_REG_VALUE,
    AUDIO_DEBUG_CMD_WRITE_MASK_REGISTER,
    AUDIO_DEBUG_CMD_DUMP_ES,
    AUDIO_DEBUG_CMD_DUMP_PCM,
    AUDIO_DEBUG_CMD_Debug_Level,
    AUDIO_DEBUG_CMD_HDMI_TX_MODE,
    AUDIO_DEBUG_CMD_AUTO_RECOVERY,

    //===== add new debug command above =====//
    AUDIO_DEBUG_CMD_HELP,
    AUDIO_DEBUG_CMD_MAX,
} AUDIO_DEBUG_CMD_INDEX;

typedef enum
{
    AUDIO_DEBUG_CMD_PERMISSION_READ,
    AUDIO_DEBUG_CMD_PERMISSION_WRITE,
    AUDIO_DEBUG_CMD_PERMISSION_READ_WRITE,
}AUDIO_DEBUG_CMD_PERMISSION;

typedef struct
{
    char CmdStr[AUDIO_DEBUG_CMDSTR_LENGTH];
    AUDIO_DEBUG_CMD_INDEX eCmdIndex;
    AUDIO_DEBUG_CMD_PERMISSION eAccessRight;
    MS_S32 s32ParamMin;
    MS_S32 s32ParamMax;
    char DescriptionStr[AUDIO_DEBUG_CMDSTR_LENGTH];
} AUDIO_DEBUG_CMD_t;

//=====================================================
MS_BOOL MDrv_Write_DSP_sram(MS_U16 dsp_addr, MS_U32 value, AUDIO_DSP_MEMORY_TYPE dsp_memory_type);
MS_BOOL MDrv_Write_DSP_Xbox(MS_U16 dsp_addr, MS_U32 value);
void MDrv_AUDIO_SetEntertainmentMode(AUDIO_ENTERTAINMENT_MODE u16Mode);
void MDrv_AUDIO_SPDIF_SetOutputType (AUDIO_SOURCE_INFO_TYPE eSource, AUDIO_SPDIF_OUTPUT_TYPE eType);
MS_BOOL MDrv_AUDIO_SetDataCaptureSource(AUDIO_DEVICE_TYPE eID, AUDIO_CAPTURE_SOURCE_TYPE eSource);
AUDIO_DEC_ID MDrv_AUDIO_OpenDecodeSystem(AudioDecStatus_t * p_AudioDecStatus);
MS_BOOL MDrv_AUDIO_CheckPlayDone(void);
MS_S32 MDrv_AUDIO_SYSTEM_Control(MS_U8 *pStrCmd, MS_U32 u32StrCmdLength);
MS_U32 MDrv_AUDIO_ReadProgramCount(AUDIO_IC_TYPE eType);

//================================================================
//  For Audio initialize
//================================================================
MS_BOOL MDrv_AUDIO_SHM_Init(void);
void    MDrv_AUDIO_SetPowerOn(MS_BOOL bPowerOn);
void    MDrv_AUDIO_RebootDSP(MS_BOOL bDspType);
MS_BOOL MDrv_AUDIO_GetDecSysSupportStatus(En_DVB_decSystemType enDecSystem);
MS_BOOL MDrv_AUDIO_GetEncodeDoneFlag(void);
MS_BOOL MDrv_AUDIO_Check_AsndR2_SupportStatus(void);
MS_BOOL MDrv_AUDIO_CheckBootOnInitState(void);
void    MDrv_AUDIO_WriteStopDecTable(void);

//================================================================
// Audio I/O Config. function
//================================================================
void    MDrv_AUDIO_FwTriggerDSP(MS_U16 u16Cmd);
void    MDrv_AUDIO_SendIntrupt(MS_BOOL bDspType,MS_U16 u8Cmd);
void    MDrv_AUDIO_I2S_SetMode(MS_U8 u8Mode, MS_U8 u8Value);
void    MDrv_AUDIO_USBPCM_Enable(MS_BOOL bEnable);
void    MDrv_AUDIO_USBPCM_SetFlag(MS_BOOL bEnable);
MS_BOOL MDrv_AUDIO_USBPCM_GetFlag(void);
void    MDrv_AUDIO_USBPCM_GetMemInfo(AUDIO_UPLOAD_INFO *uploadInfo);
MS_BOOL MDrv_AUDIO_PCMCapture_Init(const AUDIO_DEVICE_TYPE eID, const AUDIO_CAPTURE_SOURCE_TYPE eSource);
MS_BOOL MDrv_AUDIO_PCMCapture_Start(const AUDIO_DEVICE_TYPE eID);
MS_BOOL MDrv_AUDIO_PCMCapture_Stop(const AUDIO_DEVICE_TYPE eID);
MS_BOOL MDrv_AUDIO_PCMCapture_Read(const AUDIO_DEVICE_TYPE eID, void* buffer, const MS_U32 bytes);

//=====================================================
// AUDIO SPDIF Relational Hal Function
//=====================================================
void    MDrv_AUDIO_SPDIF_HWEN(MS_U8 spdif_hwen);
MS_U8   MDrv_AUDIO_SPDIF_GetMode(void);
void    MDrv_AUDIO_SPDIF_SetSCMS(MS_U8 C_bit_en, MS_U8 L_bit_en);
MS_U8   MDrv_AUDIO_SPDIF_GetSCMS(void);
MS_BOOL MDrv_AUDIO_SPDIF_ChannelStatus_CTRL(AUDIO_SPDIF_CS_TYPE cs_mode, AUDIO_SPDIF_CS_TYPE_STATUS status);
MS_BOOL Mdrv_AUDIO_DigitalOut_SetChannelStatus(DIGITAL_OUTPUT_TYPE ePath, Digital_Out_Channel_Status_t *stChannelStatus);
//=====================================================
// AUDIO HDMI Relational Hal Function
//=====================================================
MS_BOOL MDrv_AUDIO_HDMI_RX_GetHdmiInAudioStatus( ST_HDMI_AUDIO_STATUS *p_hdmiAudioSts);
void    MDrv_AUDIO_HDMI_Tx_SetMute(MS_U8 bEnMute);
AUDIO_HDMI_RX_TYPE MDrv_AUDIO_HDMI_SetNonpcm(MS_U8 u8NonPCM_en);
MS_U16  MDrv_AUDIO_HDMI_GetSynthFreq(void);
void    MDrv_AUDIO_HDMI_SetDownSample(MS_U8 ratio);
MS_BOOL MDrv_AUDIO_HDMI_NonpcmMonitor(void);
//================================================================
// for audio driver internal use function
//================================================================
void    MDrv_AUDIO_DmaReader_Init(SAMPLE_RATE sampleRate);
void    MDrv_AUDIO_DmaReader_AllInput_Init(void);
MS_BOOL MDrv_AUDIO_DmaReader_WritePCM(void* buffer, MS_U32 bytes);
void    MDrv_AUDIO_DmaWriter_Init(void);
MS_U16  MDrv_AUDIO_AbsReadReg(MS_U32 u32RegAddr);
void    MDrv_AUDIO_AbsWriteReg(MS_U32 u32RegAddr, MS_U16 u16Val);
MS_S32  MDrv_AUDIO_InputSwitch(AUDIO_SWITCH_GROUP enGroup);
MS_S32  MDrv_AUDIO_Debug_Cmd_Write(MS_U64 *pHandle, const char *pCmdLine, MS_U32 u32CmdSize);
MS_S32  MDrv_AUDIO_Debug_Cmd_Read(MS_U64 *pHandle, const char *pCmdLine, MS_U32 u32CmdSize);
void    MDrv_AUDIO_SetDecCmd(AUDIO_DEC_ID DecId, AU_DVB_DECCMD u8DecCmd);
MS_S32  MDrv_AUDIO_Get_PassthroughPermit(MS_U8 *pStrCmd, MS_U32 u32StrCmdLength);

//================================================================
// Audio dump debug message
//================================================================
void    MDrv_AUDIO_ParserPES_Bypass(MS_BOOL bEnable);

//================================================================
// Audio VoIP Interface Relational Function
//================================================================
MS_BOOL MDrv_AUDIO_VoIP_Config(AUDIO_VoIP_CONFIG_TYPE configType, MS_VIRT param1, MS_VIRT param2);

//================================================================
// Audio ALSA Interface Relational Function
//================================================================
MS_BOOL MDrv_AUDIO_ALSA_Check(void);


MS_PHY MDrv_AUDIO_GetDDRInfo(AUDIO_DEC_ID DecId, EN_AUDIO_DDRINFO DDRInfo);

void MDrv_AUDIO_PCM_Init(void);
void MDrv_AUDIO_PCM_DeInit(void);
MS_S32 MDrv_AUDIO_PCM_Open(void *pData);
MS_S32 MDrv_AUDIO_PCM_Close(MS_S32 s32DeviceId);
MS_S32 MDrv_AUDIO_PCM_Start(MS_S32 s32DeviceId);
MS_S32 MDrv_AUDIO_PCM_Stop(MS_S32 s32DeviceId);
MS_S32 MDrv_AUDIO_PCM_Set(MS_S32 s32DeviceId, MS_U32 u32Cmd, const void *pData);
MS_S32 MDrv_AUDIO_PCM_Get(MS_S32 s32DeviceId, MS_U32 u32Cmd, void *pData);
MS_U32 MDrv_AUDIO_PCM_Read(MS_S32 s32DeviceId, void *pBuf, MS_U32 u32Size);
MS_U32 MDrv_AUDIO_PCM_Write(MS_S32 s32DeviceId, const void *pBuf, MS_U32 u32Size);
MS_S32 MDrv_AUDIO_PCM_Flush(MS_S32 s32DeviceId);

MS_S32 MDrv_AUDIO_ENCODER_Open(const char *pName);
MS_S32 MDrv_AUDIO_ENCODER_Close(MS_S32 s32DeviceId);
MS_S32 MDrv_AUDIO_ENCODER_Start(MS_S32 s32DeviceId);
MS_S32 MDrv_AUDIO_ENCODER_Stop(MS_S32 s32DeviceId);
MS_S32 MDrv_AUDIO_ENCODER_Ioctl(MS_S32 s32DeviceId, const char *pCmd, void *pData);
MS_U32 MDrv_AUDIO_ENCODER_Read(MS_S32 s32DeviceId, void *pBuf, MS_U32 u32Size);
MS_S32 MDrv_AUDIO_ENCODER_Flush(MS_S32 s32DeviceId);

MS_S32 MDrv_AUDIO_CHANNEL_Open(MS_S32 s32ChannelId);
MS_S32 MDrv_AUDIO_CHANNEL_Close(MS_S32 s32ChannelId);
MS_S32 MDrv_AUDIO_CHANNEL_Ioctl(MS_S32 s32ChannelId, const MS_U8 *pStrCmd, MS_U32 u32StrCmdLength, MS_U32 *pData);

MS_S32 MDrv_AUDIO_Get_Decoder_License(MS_U8 *pStrCmd, MS_U32 u32StrCmdLength);

void MDrv_AUDIO_InitOtherMMIO(void);

//================================================================
// Procfs Function
//================================================================
void MDrv_AUDIO_GetCodeTypeName(char *CodeTypeName, MS_U32 eCodeType);
void MDrv_AUDIO_GetSourceInfoName(char *SourceInfoName, AUDIO_SOURCE_INFO_TYPE eSourceType);
void MDrv_AUDIO_Dump_R2_Log_Monitor(MS_U64* pHandle);
void MDrv_AUDIO_Dump_RegBank(MS_U64* pHandle, MS_U32 u32bank);
#if defined(CONFIG_NOS) || defined(CONFIG_NUTTX)
#else
void MDrv_AUDIO_CreateThread(MS_U64 *pHandle);
#endif
void MDrv_AUDIO_Delay1MS(MS_U32 delay1MS);

#ifdef MSOS_TYPE_LINUX_KERNEL //Kernel Space
struct file * MDrv_AUDIO_FileOpen(const char * filename, const char *mode);
void MDrv_AUDIO_FileClose(struct file *fp);
void MDrv_AUDIO_FileWrite(const void *p, MS_SIZE szSize, MS_SIZE szNmemb, struct file * fp);
#else //User Space
FILE * MDrv_AUDIO_FileOpen(const char * filename, const char *mode);
MS_U32 MDrv_AUDIO_FileClose(FILE * fp);
void MDrv_AUDIO_FileWrite(const void *p, MS_SIZE szSize, MS_SIZE szNmemb, FILE *fp);
#endif

// DEBUG: enum to string
const char * _enumToString_SpdifOutputType(AUDIO_SPDIF_OUTPUT_TYPE eSpdifOutputType);
const char * _enumToString_CodecType(AUDIO_DSP_CODE_TYPE eCodecType);
const char * _enumToString_AudioSource(AUDIO_SOURCE_INFO_TYPE eAudioSource);
const char * _enumToString_HdmiTxOutputType(HDMI_TX_OUTPUT_TYPE eHDMITxOutputType);


#ifdef __cplusplus
}
#endif

#endif // _DRV_AUDIO_IF_H_
