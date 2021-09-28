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

/*! \defgroup Audio Audio interface

     \brief
    Audio Module is used for:

    - 1. Audio Input source handling
    - support input source as VGA, ATV, CVBS, YPbPr, SCART, HDMI, DTV, MM ...

    - 2. Audio Output source handling
    - support stereo audio analog output ,SPDIF digital output, I2S, HDMI 1.4 ARC output

    - 3. Basic Sound Effect support
    - support Volume Control , Bass, Treble, GEQ, PEQ, Surround, AVC ...

    - 4. Digital audio format decoding
    - support MPEG-1, MPEG-2(Layer I/II), MP3, AC-3,E-AC-3, WMA, HE-AAC, DTS, FLAC, XPCM

    - 5. Advanced Sound Effect support
    - support SRS-TSHD , SRS-TSXT, DBX....

    - All audio related features , architecture and bolck diagram are collected in
      MStar Audio Application Note(Chip dependent).

 *! \defgroup Audio_System System
 *  \ingroup Audio

 *! \defgroup Audio_Decoder Decoder
 *  \ingroup Audio

 *! \defgroup Audio_ATV_SPDIF_HDMI ATV SPDIF HDMI
 *  \ingroup Audio

 *! \defgroup Audio_BASIC_SOUNDEFFECT Basic Sound Effect
 *  \ingroup Audio

 *! \defgroup Audio_ADVANCED_SOUNDEFFECT Advanced Sound Effect
 *  \ingroup Audio

 *! \defgroup Audio_ToBeMerged To Be Merged
 *  \ingroup Audio

 *! \defgroup Audio_ToBeModified To Be Modified
 *  \ingroup Audio

 *! \defgroup Audio_ToBeMoved To Be Moved
 *  \ingroup Audio

 *! \defgroup Audio_ToBeRemove To Be Removed
 *  \ingroup Audio
 */

#ifndef _API_AUDIO_H_
#define _API_AUDIO_H_

#include "MsCommon.h"
#include "drvAUDIO_if.h"
#include "UFO.h"


#ifdef __cplusplus
extern "C"
{
#endif
//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
//-----------------------------------------------------------
#define  MSAPI_AUD_SPDIF_PCM    0
#define  MSAPI_AUD_SPDIF_OFF    1
#define  MSAPI_AUD_SPDIF_NONPCM 2

#define  MSAPI_AUD_DTV_IN       0
#define  MSAPI_AUD_ATV_IN       1
#define  MSAPI_AUD_HDMI_IN      2
#define  MSAPI_AUD_ADC_IN       3
#define  MSAPI_AUD_CardRD_IN    4
#define  MSAPI_AUD_MM_IN    5

#define  MSAPI_AUD_AVsync       0
#define  MSAPI_AUD_FreeRun      1

/// A2/FM/_SOUND_MOD Report info
#define _STATUS_MOD_MONO_EXIST                  0x01
#define _STATUS_MOD_STEREO_EXIST                0x02
#define _STATUS_MOD_DUAL_EXIST                  0x04
#define _STATUS_MOD_A2_PILOT_EXIST             0x08
#define _STATUS_MOD_A2_PRIMARY_EXIST           0x10
#define _STATUS_MOD_A2_SECONDARY_EXIST         0x20
#define _STATUS_MOD_A2_DK2_EXIST                0x40
#define _STATUS_MOD_A2_DK3_EXIST                0x80

/// NICAM_SOUND_MOD Report info
#define _MASK_NICAM_STATUS_INFO                 0x0F
#define _NICAM_SOUND_MODE_LOCK_STATE            0x05
#define _MASK_NICAM_SOUND_MODE_INFO             0xF0
#define _NICAM_SOUND_MOD_MONO_NOT_EXIST         0x00
#define _NICAM_SOUND_MOD_NICAM_MONO             0x10
#define _NICAM_SOUND_MOD_NICAM_STEREO           0x20
#define _NICAM_SOUND_MOD_NICAM_DUAL             0x30
#define _NICAM_SOUND_MOD_MONO_EXIST             0x80
#define _NICAM_SOUND_MOD_MONO_n_NICAM_MONO     0x90
#define _NICAM_SOUND_MOD_MONO_n_NICAM_STEREO   0xA0
#define _NICAM_SOUND_MOD_MONO_n_NICAM_DUAL     0xB0

/********************************
*   SIF Standard
********************************/
#define _STD_SEL_INVALID    0x00
#define _STD_SEL_M_BTSC     0x01
#define _STD_SEL_M_EIAJ     0x02
#define _STD_SEL_M_A2       0x03
#define _STD_SEL_BG_A2      0x04
#define _STD_SEL_DK1_A2     0x05
#define _STD_SEL_DK2_A2     0x06
#define _STD_SEL_DK3_A2     0x07
#define _STD_SEL_BG_NICAM   0x08
#define _STD_SEL_DK_NICAM   0x09
#define _STD_SEL_I_NICAM    0x0A
#define _STD_SEL_L_NICAM    0x0B
#define _STD_SEL_AUTO       0xE0

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
///< Decoder command type
typedef enum
{
    MSAPI_AUD_DVB_DECCMD_STOP = 0x0,   ///< 1st DSP stop
    MSAPI_AUD_DVB_DECCMD_PLAY,         ///< 1st DSP play
    MSAPI_AUD_DVB_DECCMD_RESYNC,
    MSAPI_AUD_DVB_DECCMD_FREE_RUN,
    MSAPI_AUD_DVB_DECCMD_AVSYNC,
    MSAPI_AUD_DVB_DECCMD_PLAYFILE,     ///< 1st DSP play file
    MSAPI_AUD_DVB_DECCMD_PAUSE,        ///< 1st DSP pause
    MSAPI_AUD_DVB_DECCMD_PLAYFILETSP,  ///< 1st DSP play TS file
    MSAPI_AUD_DVB_DECCMD_STARTBROWSE,
    MSAPI_AUD_DVB_DECCMD_PLAYFRAME,    ///<1st DSP play MM (for uniplayer/Mstreamer)
    MSAPI_AUD_DVB_DECCMD_PLAYFRAME_STAGE, ////<1st DSP play MM (for stagecraft)
    MSAPI_AUD_DVB_DECCMD_PLAYFRAME_GS,  ///<1st DSP play MM (for uniplayer/Gstreamer ES copy)
    MSAPI_AUD_DVB_DECCMD_PLAYMM_FILE2,
    MSAPI_AUD_DVB_DECCMD_PLAY_AD,
    MSAPI_AUD_DVB_DECCMD_STOP_AD,
    MSAPI_AUD_DVB_DECCMD_SKIP,

    // SE DSP
    MSAPI_AUD_DVB2_DECCMD_STOP=0x10,   ///< 2nd DSP stop
    MSAPI_AUD_DVB2_DECCMD_PLAY,        ///< 2nd DSP play
    MSAPI_AUD_DVB2_DECCMD_RESYNC,
    MSAPI_AUD_DVB2_DECCMD_FREE_RUN,
    MSAPI_AUD_DVB2_DECCMD_AVSYNC,
    MSAPI_AUD_DVB2_DECCMD_PLAYFILE,    ///< 2nd DSP play file
    MSAPI_AUD_DVB2_DECCMD_PAUSE,       ///< 2nd DSP pause
    MSAPI_AUD_DVB2_DECCMD_PLAYFILETSP,  ///< 2nd DSP play TS file

    MSAPI_AUD_DVB_DECCMD_TSP_FRAMEDECODE, //ES from HW TSP, after decoding, won't output PCM to DSP, let user get PCM from driver

} En_DVB_decCmdType;

///< Encoder Command
typedef enum
{
    MSAPI_AUD_DVB_ENCCMD_STOP = 0x0,   ///< stop encode
    MSAPI_AUD_DVB_ENCCMD_START         ///<  start encode
}En_DVB_EncCmdType;

///< ATV SIF Command
typedef enum
{
    MSAPI_AUD_SIF_CMD_SET_STOP = 0x00,
    MSAPI_AUD_SIF_CMD_SET_PLAY = 0x01,
    MSAPI_AUD_SIF_CMD_ENABLE_HIDEV = 0x02,
    MSAPI_AUD_SIF_CMD_SET_HIDEV_FILTER_BW_LEVEL = 0x03,
    MSAPI_AUD_SIF_CMD_RESET_FC_TRACKING = 0x04,
    MSAPI_AUD_SIF_CMD_ENABLE_FC_TRACKING = 0x05,
    MSAPI_AUD_SIF_CMD_SET_ADC_FROM_VIF_PATH = 0x06,
    MSAPI_AUD_SIF_CMD_ENABLE_AUTO_MUTE = 0x07,
    MSAPI_AUD_SIF_CMD_ENABLE_BEEPER_FUNCTION = 0x08,
    MSAPI_AUD_SIF_CMD_SET_BEEPER_TONE = 0x09,
    MSAPI_AUD_SIF_CMD_ENABLE_AGC = 0x0A,
    MSAPI_AUD_SIF_CMD_RESET_AGC = 0x0B,
    MSAPI_AUD_SIF_STANDARD_RESET = 0x0C,
    MSAPI_AUD_SIF_CMD_DETECT_MAIN_STD_ONLY = 0x0D,
    MSAPI_AUD_SIF_CMD_ENABLE_AUTO_SOUNDMODE = 0x0E,
    MSAPI_AUD_SIF_CMD_FMTX_SET_PREEMP = 0x0F,
    MSAPI_AUD_SIF_CMD_SET_ENC_CMD = 0x10,
    MSAPI_AUD_SIF_CMD_SET_ENC_Input_attenuation= 0x11,
    MSAPI_AUD_SIF_CMD_SET_ENC_Output_scaling= 0x12,
    MSAPI_AUD_SIF_CMD_SET_BTSC_Enc_M_Gain=0x13,
    MSAPI_AUD_SIF_CMD_SET_BTSC_Enc_D_Gain= 0x14,
    MSAPI_AUD_SIF_CMD_SET_BTSC_Enc_SAP_Gain= 0x15,
    MSAPI_AUD_SIF_CMD_SET_FM_CH2_FILTER_BW_LEVEL=0x16,
    MSAPI_AUD_SIF_CMD2_ADC_VIA_VIF_PIN = 0x01
} En_AUD_SIF_CmdType;

typedef enum
{
   MSAPI_AUD_SIF_42M,
   MSAPI_AUD_SIF_43M,
   MSAPI_AUD_SIF_44M,
} En_AUD_VIF_Type;

enum
{
    MSAPI_AUD_SIF_HIDEV_FILTER_BW_DEFAULT = 0x00, // equal middle filter bandwidth
    MSAPI_AUD_SIF_HIDEV_FILTER_BW_L1 = 0x10,    // narrowest filter bandwidth
    MSAPI_AUD_SIF_HIDEV_FILTER_BW_L2 = 0x20,    // middle filter bandwidth
    MSAPI_AUD_SIF_HIDEV_FILTER_BW_L3 = 0x30     // widest filter bandwidth
};

///< Decoder Number
typedef enum
{
   DVB_Audio_Decoder1,             ///<Audio decoder1
   DVB_Audio_Decoder2              ///<Audio decoder2
} En_DVB_AudioDecoder;

#if defined(UFO_PUBLIC_HEADER_500) || defined(UFO_PUBLIC_HEADER_700)

///< Advance sound effect type
typedef char* En_DVB_advsndType;
#define MSAPI_AUD_ADVSND_NONE          ((En_DVB_advsndType)"ADVSND_NONE")
#define MSAPI_AUD_ADVSND_PL2           ((En_DVB_advsndType)"ADVSND_PL2")
#define MSAPI_AUD_ADVSND_BBE           ((En_DVB_advsndType)"ADVSND_BBE")
#define MSAPI_AUD_ADVSND_SRS           ((En_DVB_advsndType)"ADVSND_SRS")
#define MSAPI_AUD_ADVSND_VDS           ((En_DVB_advsndType)"ADVSND_VDS")
#define MSAPI_AUD_ADVSND_VSPK          ((En_DVB_advsndType)"ADVSND_VSPK")
#define MSAPI_AUD_ADVSND_SUPV          ((En_DVB_advsndType)"ADVSND_SUPV")
#define MSAPI_AUD_ADVSND_TSHD          ((En_DVB_advsndType)"ADVSND_TSHD")
#define MSAPI_AUD_ADVSND_DTS_ULTRATV   ((En_DVB_advsndType)"ADVSND_DTS_ULTRATV")
#define MSAPI_AUD_ADVSND_TSHDVIQ       ((En_DVB_advsndType)"ADVSND_TSHDVIQ")
#define MSAPI_AUD_ADVSND_AUDYSSEY      ((En_DVB_advsndType)"ADVSND_AUDYSSEY")
#define MSAPI_AUD_ADVSND_DBX           ((En_DVB_advsndType)"ADVSND_DBX")
#define MSAPI_AUD_ADVSND_SSHD_TSHD     ((En_DVB_advsndType)"ADVSND_SSHD_TSHD")
#define MSAPI_AUD_ADVSND_PURESND       ((En_DVB_advsndType)"ADVSND_PURESND")
#define MSAPI_AUD_ADVSND_BONGIOVI_DPS  ((En_DVB_advsndType)"ADVSND_BONGIOVI_DPS")
#define MSAPI_AUD_ADVSND_DOLBY_DAP     ((En_DVB_advsndType)"ADVSND_DOLBY_DAP")

#else

///< Advance sound effect type
typedef enum
{
    MSAPI_AUD_ADVSND_PL2,             ///<ID = 0
    MSAPI_AUD_ADVSND_BBE,             ///<ID = 1
    MSAPI_AUD_ADVSND_SRS,             ///<ID = 2
    MSAPI_AUD_ADVSND_VDS,             ///<ID = 3
    MSAPI_AUD_ADVSND_VSPK,            ///<ID = 4
    MSAPI_AUD_ADVSND_SUPV,            ///<ID = 5
    MSAPI_AUD_ADVSND_TSHD,            ///<ID = 6
    MSAPI_AUD_ADVSND_DTS_ULTRATV,     ///<ID = 7
    MSAPI_AUD_ADVSND_TSHDVIQ,         ///<ID = 8
    MSAPI_AUD_ADVSND_AUDYSSEY,        ///<ID = 9
    MSAPI_AUD_ADVSND_DBX,             ///<ID = 10
    MSAPI_AUD_ADVSND_SSHD_TSHD,       ///<ID = 55
    MSAPI_AUD_ADVSND_PURESND,         ///<ID = 66
    MSAPI_AUD_ADVSND_BONGIOVI_DPS,   ///<ID = 93
    MSAPI_AUD_ADVSND_CV3,
} En_DVB_advsndType;

#endif

///< Select audio output mode type
typedef enum
{
    MSAPI_AUD_DVB_SOUNDMODE_STEREO,
    MSAPI_AUD_DVB_SOUNDMODE_LEFT,
    MSAPI_AUD_DVB_SOUNDMODE_RIGHT,
    MSAPI_AUD_DVB_SOUNDMODE_NONE,
} En_DVB_soundModeType;

///< Melody play command type
typedef enum
{
    MSAPI_AUD_MELODY_START,
    MSAPI_AUD_MELODY_STOP,
    MSAPI_AUD_MELODY_RESUME,
    MSAPI_AUD_MELODY_CHK_STATUS
}En_DVB_melodyPlayCmdType;

typedef struct
{
    MS_U32 u32BufStartAddr;
    MS_U32 u32BufEndAddr;
    MS_U32 u32BufWrPtr;
    MS_U32 u32BufTotalSize;
    MS_U32 u32BufferedPCMSize;
}audioBufMonitor;

typedef struct
{
    audioBufMonitor _stBufInfo;
    MS_U32 u32ReqAddr;
    MS_U32 u32ReqSize;
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
    MS_U32 u32RetAddr1;
#else
    MS_PHY phyRetAddr1;
#endif
    MS_U32 u32RetSize1;
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
    MS_U32 u32RetAddr2;
#else
    MS_PHY phyRetAddr2;
#endif
    MS_U32 u32RetSize2;
    MS_U8 u8RingStatus;
}audioRingTask;


///< Melody play status
typedef enum
{
    MSAPI_AUD_MELODY_NEED_MORE_DATA,
    MSAPI_AUD_MELODY_PLAY_INIT,
    MSAPI_AUD_MELODY_PLAY_ING,
    MSAPI_AUD_MELODY_PLAY_DONE,
    MSAPI_AUD_MELODY_PLAY_STOP
}En_DVB_melodyPlayStatus;

///< Select AD output type
enum
{
    AD_OUT_SPEAKER, ///< AD out in speaker only
    AD_OUT_HP,      ///< AD out in HP only
    AD_OUT_BOTH,    ///< AD out both in speaker & HP
    AD_OUT_NONE     ///< no AD out
};

///< Audio System Type
typedef enum
{
    E_AUDIO_SYSTEM_BTSC,
    E_AUDIO_SYSTEM_A2,
    E_AUDIO_SYSTEM_EIAJ
}AUDIO_SYSTEM_TYPE;

///< Audio SIF Standard Type
typedef enum
{
    E_AUDIOSTANDARD_BG          = 0x00,         ///< Audio standard BG
    E_AUDIOSTANDARD_BG_A2       = 0x01,         ///< Audio standard BG A2
    E_AUDIOSTANDARD_BG_NICAM    = 0x02,         ///< Audio standard BG NICAM
    E_AUDIOSTANDARD_I           = 0x03,         ///< Audio standard I
    E_AUDIOSTANDARD_DK          = 0x04,         ///< Audio standard DK
    E_AUDIOSTANDARD_DK1_A2      = 0x05,         ///< Audio standard DK1 A2
    E_AUDIOSTANDARD_DK2_A2      = 0x06,         ///< Audio standard DK2 A2
    E_AUDIOSTANDARD_DK3_A2      = 0x07,         ///< Audio standard DK3 A2
    E_AUDIOSTANDARD_DK_NICAM    = 0x08,         ///< Audio standard DK NICAM
    E_AUDIOSTANDARD_L           = 0x09,         ///< Audio standard L
    E_AUDIOSTANDARD_M           = 0x0A,         ///< Audio standard M
    E_AUDIOSTANDARD_M_BTSC      = 0x0B,         ///< Audio standard M BTSC
    E_AUDIOSTANDARD_M_A2        = 0x0C,         ///< Audio standard M A2
    E_AUDIOSTANDARD_M_EIA_J     = 0x0D,         ///< Audio standard M EIA J
    E_AUDIOSTANDARD_NOTSTANDARD = 0x0F          ///< Not Audio standard
} AUDIOSTANDARD_TYPE;

///< Audio Stream type
typedef enum
{
    E_AUDIOSTREAM_INVALID       = 0x00,
    E_AUDIOSTREAM_MPEG          = 0x01,
    E_AUDIOSTREAM_AC3           = 0x02,
    E_AUDIOSTREAM_MPEG_AD       = 0x03,
    E_AUDIOSTREAM_MPEG4         = 0x04,
    E_AUDIOSTREAM_AAC           = 0x05,
    E_AUDIOSTREAM_AC3P          = 0x06,
    E_AUDIOSTREAM_AC3_AD        = 0x07,
    E_AUDIOSTREAM_AC3P_AD       = 0x08,
    E_AUDIOSTREAM_AACP_AD       = 0x09,
    E_AUDIOSTREAM_DRA           = 0x0A
} AUDIOSTREAM_TYPE;

///< Audio mode type
typedef enum
{
    E_AUDIOMODE_INVALID      = 0x00,            ///< Audio Mode Invalid
    E_AUDIOMODE_MONO         = 0x01,           ///< Audio Mode MONO
    E_AUDIOMODE_FORCED_MONO  = 0x02,           ///< Audio Mode Forced MONO
    E_AUDIOMODE_G_STEREO     = 0x03,           ///< Audio Mode G Stereo
    E_AUDIOMODE_K_STEREO     = 0x04,           ///< Audio Mode  K Stereo
    E_AUDIOMODE_MONO_SAP     = 0x05,           ///< Audio Mode MONO SAP
    E_AUDIOMODE_STEREO_SAP   = 0x06,           ///< Audio Mode Stereo SAP
    E_AUDIOMODE_DUAL_A       = 0x07,           ///< Audio Mode Dual A
    E_AUDIOMODE_DUAL_B       = 0x08,           ///< Audio Mode Dual B
    E_AUDIOMODE_DUAL_AB      = 0x09,           ///< Audio Mode Dual AB
    E_AUDIOMODE_NICAM_MONO   = 0x0A,           ///< Audio Mode NICAM MONO
    E_AUDIOMODE_NICAM_STEREO = 0x0B,           ///< Audio Mode NICAM Stereo
    E_AUDIOMODE_NICAM_DUAL_A = 0x0C,           ///< Audio Mode NICAM DUAL A
    E_AUDIOMODE_NICAM_DUAL_B = 0x0D,           ///< Audio Mode NICAM DUAL B
    E_AUDIOMODE_NICAM_DUAL_AB = 0x0E,           ///< Audio Mode NICAM DUAL AB
    E_AUDIOMODE_HIDEV_MONO   = 0x0F,           ///< Audio Mode HIDEV MONO
    E_AUDIOMODE_LEFT_LEFT    = 0x10,           ///< Audio Mode Left left
    E_AUDIOMODE_RIGHT_RIGHT  = 0x11,           ///< Audio Mode right right
    E_AUDIOMODE_LEFT_RIGHT   = 0x12           ///< Audio Mode left right
} AUDIOMODE_TYPE;

///< sif carrier status define
typedef enum
{ // Never change the value of enumeration. Because each bit has weight.
    E_STATE_AUDIO_NO_CARRIER           = 0x00, ///< No carrier detect
    E_STATE_AUDIO_PRIMARY_CARRIER      = 0x01, ///< Carrier 1 exist
    E_STATE_AUDIO_SECONDARY_CARRIER    = 0x02, ///< Carrier 2 exist
    E_STATE_AUDIO_NICAM                = 0x04, ///< Nicam lock state
    E_STATE_AUDIO_STEREO               = 0x08, ///< A2 Stereo exist
    E_STATE_AUDIO_BILINGUAL            = 0x10, ///< A2 Dual exist
    E_STATE_AUDIO_PILOT                = 0x20, ///< A2 Pilot exist
    E_STATE_AUDIO_DK2                  = 0x40, ///< Sound standard is DK2
    E_STATE_AUDIO_DK3                  = 0x80 ///< Sound standard is DK3
} AUDIOSTATUS;

///< EQ band define
typedef enum
{
    E_EQUALIZER_BAND_1    = 0x00, ///< EQ Band1
    E_EQUALIZER_BAND_2    = 0x01, ///< EQ Band2
    E_EQUALIZER_BAND_3    = 0x02, ///< EQ Band3
    E_EQUALIZER_BAND_4    = 0x03, ///< EQ Band4
    E_EQUALIZER_BAND_5    = 0x04, ///< EQ Band5
    E_EQUALIZER_BAND_6    = 0x05, ///< EQ Band6
    E_EQUALIZER_BAND_7    = 0x06  ///< EQ Band7
} EQUALIZER_BAND;

///< Audio command type
typedef enum
{
    E_AUDIO_COM_PLAY,
    E_AUDIO_COM_STOP,
    E_AUDIO_COM_RESYNC,
    E_AUDIO_COM_FREE_RUN,
    E_AUDIO_COM_LOCK_AVSYNC,
    E_AUDIO_COM_FILE_PLAY,      // start playing file based audio in MHEG5
    E_AUDIO_COM_FILE_PAUSE,      // pause playing file based audio in MHEG5
    E_AUDIO_DVB2_COM_PLAY,
    E_AUDIO_DVB2_COM_STOP,
    E_AUDIO_DVB2_COM_RESYNC,
    E_AUDIO_DVB2_COM_FREE_RUN,
    E_AUDIO_DVB2_COM_LOCK_AVSYNC,
    E_AUDIO_DVB2_COM_FILE_PLAY,      // start playing file based audio in MHEG5
    E_AUDIO_DVB2_COM_FILE_PAUSE      // pause playing file based audio in MHEG5
} DTV_AUDIO_COMMAND;

enum
{
    MSAPI_AUD_MODE_STEREO,  ///< 0:STEREO
    MSAPI_AUD_MODE_LL,      ///< 1: LL
    MSAPI_AUD_MODE_RR,      ///< 2: RR
    MSAPI_AUD_MODE_MUTE     ///< 3:MUTE
};

/// TSXT mode
enum
{
    MSAPI_TSXT_OFF     = 0,
    MSAPI_TSXT_STEREO,
    MSAPI_TSXT_3D_MONO,
    MSAPI_TSXT_MAX,
    MSAPI_TSXT_INVALID = -1
};

typedef enum
{
    E_TSXT_MOD_STEREO,         ///< 0:STEREO
    E_TSXT_MOD_3D_MONO,        ///< 0:MONO
    E_TSXT_MOD_MAX             ///< 0:PASSIVE MATRIX
} TSXT_MODETYPE;

///< TSXT parameter
typedef enum
{
    E_TSXT_PARA_TS_GAIN,
    E_TSXT_PARA_DC_GAIN,
    E_TSXT_PARA_TB_GAIN,
    E_TSXT_PARA_SPK,
    E_TSXT_PARA_INPUT_MOD,
    E_TSXT_PARA_OUTPUT_GAIN
} TSXT_PARATYPE;

///< Audio DSP system type
typedef enum
{
    E_AUDIO_DSP_MPEG,                       ///< AUDIO DSP = MPEG
    E_AUDIO_DSP_AC3,                        ///< AUDIO DSP = AC3
    E_AUDIO_DSP_AC3_AD,                     ///< AUDIO DSP = AC3 AD
    E_AUDIO_DSP_SIF,                        ///< AUDIO DSP = SIF
    E_AUDIO_DSP_AACP,                       ///< AUDIO DSP = HE-AAC
    E_AUDIO_DSP_MPEG_AD,                    ///< AUDIO DSP = MPEG AD
    E_AUDIO_DSP_AC3P,                       ///< AUDIO DSP = AC3+
    E_AUDIO_DSP_AC3P_AD,                    ///< AUDIO DSP = AC3P AD
    E_AUDIO_DSP_AACP_AD,                    ///< AUDIO DSP = HE-AAC AD
    E_AUDIO_DSP_DTS,                        ///< AUDIO DSP = DTS
    E_AUDIO_DSP_MS10_DDT,                   ///< AUDIO DSP = MS10 DDT
    E_AUDIO_DSP_MS10_DDC,                   ///< AUDIO DSP = MS10 DDC
    E_AUDIO_DSP_FLAC,                       ///< AUDIO DSP = FLAC
    E_AUDIO_DSP_FM_RADIO,                   ///< AUDIO DSP = FM RADIO
    E_AUDIO_DSP_KTV,
    E_AUDIO_DSP_RESERVE3,
    E_AUDIO_DSP_RESERVE2,
    E_AUDIO_DSP_RESERVE1,
    E_AUDIO_DSP_MAX,
    E_AUDIO_DSP_INVALID=0xFF                ///< AUDIO DSP = Invalid
} AUDIO_DSP_SYSTEM;

///<Audio source switch group
typedef enum
{
    E_AUDIO_GROUP_MAIN,      ///< Main speaker, HP, Line-out , S/PDIF PCM mode
    E_AUDIO_GROUP_SCART,    ///< SCART
    E_AUDIO_GROUP_SPDIF,    ///< SPDIF non-pcm
    E_AUDIO_GROUP_SUB,       ///< SUB source for PIP application
    E_AUDIO_GROUP_INVALID
} AUDIO_SWITCH_GROUP;

#define TBL_INDEX  MS_U8
#define TBL_LENGTH MS_U8

#define FM_RADIO_TBL_LENGTH    6   ///< FM Radio Threshold table length
#define BTSC_TBL_LENGTH       10   ///< BTSC Threshold table length
#define A2_M_TBL_LENGTH       10   ///< A2_M Threshold table length
#define A2_BG_TBL_LENGTH      10   ///< A2_BG Threshold table length
#define A2_DK_TBL_LENGTH      10   ///< A2_DK Threshold table length
#define FM_I_TBL_LENGTH        4   ///< FM_I Threshold table length
#define AM_TBL_LENGTH          2   ///< AM Threshold table length
#define NICAM_BG_TBL_LENGTH    2   ///< NICAM_BG Threshold table length
#define NICAM_I_TBL_LENGTH     2   ///< NICAM_I Threshold table length
#define HIDEV_M_TBL_LENGTH     4   ///< HIDEV_M Threshold table length
#define HIDEV_BG_TBL_LENGTH    4   ///< HIDEV_BG Threshold table length
#define HIDEV_DK_TBL_LENGTH    4   ///< HIDEV_DK Threshold table length
#define HIDEV_I_TBL_LENGTH     4   ///< HIDEV_I Threshold table length

#define FM_RADIO_TBL_INDEX      0
#define BTSC_TBL_INDEX          0
#define A2_M_TBL_INDEX          0
#define A2_BG_TBL_INDEX      A2_M_TBL_INDEX + A2_M_TBL_LENGTH
#define A2_DK_TBL_INDEX      A2_BG_TBL_INDEX + A2_BG_TBL_LENGTH
#define FM_I_TBL_INDEX       A2_DK_TBL_INDEX + A2_DK_TBL_LENGTH
#define AM_TBL_INDEX         FM_I_TBL_INDEX + FM_I_TBL_LENGTH
#define NICAM_BG_TBL_INDEX  AM_TBL_INDEX + AM_TBL_LENGTH
#define NICAM_I_TBL_INDEX   NICAM_BG_TBL_INDEX + NICAM_BG_TBL_LENGTH
#define HIDEV_M_TBL_INDEX   NICAM_I_TBL_INDEX + NICAM_I_TBL_LENGTH
#define HIDEV_BG_TBL_INDEX  HIDEV_M_TBL_INDEX + HIDEV_M_TBL_LENGTH
#define HIDEV_DK_TBL_INDEX  HIDEV_BG_TBL_INDEX + HIDEV_BG_TBL_LENGTH
#define HIDEV_I_TBL_INDEX   HIDEV_DK_TBL_INDEX + HIDEV_DK_TBL_LENGTH

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

//============================================================
// AUDIO_SYSTEM RELATIONAL API FUNCTION
//============================================================

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_System
/// @brief \b Function \b Name: MApi_AUDIO_SetPowerOn()
/// @brief \b Function \b Description: This routine is used to execute DSP power on/down setting.
/// @param bFlag        \b : TRUE --power on;
///                         FALSE--power off
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_SetPowerOn(MS_BOOL bFlag);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_System
/// @brief \b Function \b Name: MApi_AUD_Initialize()
/// @brief \b Function \b Description: Audio initialize function incluing DSP power on and load code.
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_Initialize(void);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_System
/// @brief \b Function \b Name: MApi_AUDIO_Suspend()
/// @brief \b Function \b Description: Suspend Audio system and backup audio system status
////////////////////////////////////////////////////////////////////////////////
 MS_BOOL MApi_AUDIO_Suspend(void);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_System
/// @brief \b Function \b Name: MApi_AUDIO_Resume()
/// @brief \b Function \b Description: Resume Audio system and restore status
////////////////////////////////////////////////////////////////////////////////
 MS_BOOL MApi_AUDIO_Resume(void);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_System
/// @brief \b Function \b Name: MApi_AUDIO_SetSystemInfo()
/// @brief \b Function \b Description:  Function for Driver layer to get the system compile option
///                                                  (TV system , Audio system ....)
/// @param pau_info    \b : Audio output path information structure pointer
///                                    Please refer  AUDIO_INIT_INFO   in header file
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_SetSystemInfo(AUDIO_INIT_INFO *pau_info);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_System
/// @brief \b Function \b Name: MApi_Audio_Monitor()
/// @brief \b Function \b Description: This function is used to monitor DSP while-loop and ISR running counter.
///                                  If these counters stop longer than a certain period of time in any abnormal condition, DSP reboot will be launched to alive DSP process again.
////////////////////////////////////////////////////////////////////////////////
 void    MApi_Audio_Monitor(void);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_System
/// @brief \b Function \b Name: MApi_AUDIO_SetOutConnectivity()
/// @brief \b Function \b Description:  Set the TV output connectivity information from APP layer
///                                                   to audio driver .  Then audio driver will set the audio output
///                                                   port with these information automatically.
///////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_SetOutConnectivity(void);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_System
/// @brief \b Function \b Name: MApi_AUDIO_SetPathInfo()
/// @brief \b Function \b Description:  Get audio output path information from APP
/// @param path_info   \b : path information structure pointer
///                                    for Speaker , Head phone , Monitor out , Scart out and SPDIF out
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_SetPathInfo(AUDIO_PATH_INFO *path_info);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_System
/// @brief \b Function \b Name: MApi_AUDIO_GetPathInfo()
/// @brief \b Function \b Description:  Get audio output path information from APP
/// @param path_info   \b : path information structure pointer
///                                    for Speaker , Head phone , Monitor out , Scart out and SPDIF out
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_GetPathInfo(AUDIO_PATH_INFO *path_info);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_System
/// @brief \b Function \b Name: MApi_AUDIO_SetOutputInfo()
/// @brief \b Function \b Description:  Get audio output information from APP
/// @param pout_info   \b : Audio output path information structure pointer.
///                                    Please refer  AUDIO_OUT_INFO   in header file
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_SetOutputInfo(AUDIO_OUT_INFO *pout_info);

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_GetOutputInfo()
/// @brief \b Function \b Description:  Get audio output information from APP
/// @param pout_info   \b : Audio output path information structure pointer.
///                                    Please refer  AUDIO_OUT_INFO   in header file
////////////////////////////////////////////////////////////////////////////////
void MApi_AUDIO_GetOutputInfo(AUDIO_OUT_INFO *pout_info);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_System
/// @brief \b Function \b Name: MApi_AUDIO_SetSourceInfo()
/// @brief \b Function \b Description:  For driver layer to get audio source information from APP
/// @param eSourceType   \b : information structure pointer ==>AUDIO_SOURCE_INFO_TYPE
///                         - E_AUDIO_INFO_DTV_IN,
///                         - E_AUDIO_INFO_ATV_IN,
///                         - E_AUDIO_INFO_HDMI_IN,
///                         - E_AUDIO_INFO_ADC_IN,
///                         - E_AUDIO_INFO_MM_IN,
///                         - E_AUDIO_INFO_SPDIF_IN,
///                         - E_AUDIO_INFO_KTV_IN,
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_SetSourceInfo(AUDIO_SOURCE_INFO_TYPE eSourceType);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_System
/// @brief \b Function \b Name: MApi_AUDIO_InputSwitch()
/// @brief \b Function \b Description:  Set audio Input connectivity (for S7 serious only)
/// @param enSource   \b :  Please refer  AUDIO_INPUT_TYPE  enum in header file
///@param  enGroup  \b : Please refer  AUDIO_SWITCH_GROUP  enum in header file
///////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_InputSwitch(AUDIO_INPUT_TYPE enSource, AUDIO_SWITCH_GROUP enGroup);

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetInternalPath()    @@VVV
/// @brief \b Function \b Description: This routine is used to set the topalogy for Audio Output.
/// @param <IN>        \b u8Path    : Audio internal path
/// @param <IN>        \b output    : Audio output type
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b  NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void MApi_AUDIO_SetInternalPath(AUDIO_INTERNAL_PATH_TYPE u8Path,  AUDIO_OUTPUT_TYPE u8Output);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeRemove
/// @brief \b Function \b Name: MApi_AUDIO_SetDataCaptureSource()
/// @brief \b Function \b Description:  Select source for data capture
/// @param <IN>        \b AUDIO_DEVICE_TYPE : select 1st or 2nd data capture
/// @param <IN>        \b AUDIO_CAPTURE_SOURCE_TYPE : Data Source
/// @param <OUT>       \b MS_BOOL    : return TRUE if success, else return FALSE
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
 MS_BOOL MApi_AUDIO_SetDataCaptureSource(AUDIO_DEVICE_TYPE eID, AUDIO_CAPTURE_SOURCE_TYPE eSource);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_System
/// @brief \b Function \b Name: MApi_AUDIO_SetOutputSourceInfo()
/// @brief \b Function \b Description:  Set audio output source information
/// @param path_info   \b : output port source information structure pointer
///                                    for Speaker , Head phone , Monitor out , Scart out and SPDIF out
////////////////////////////////////////////////////////////////////////////////
 void   MApi_AUDIO_SetOutputSourceInfo(OUTPUT_SOURCE_INFO *pSourceInfo);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_System
/// @brief \b Function \b Name: MApi_AUDIO_GetOutputSourceInfo()
/// @brief \b Function \b Description:  Get audio output path information from APP
/// @param pSourceInfo   \b : output port source information structure pointer
///                                    for Speaker , Head phone , Monitor out , Scart out and SPDIF out
////////////////////////////////////////////////////////////////////////////////
 void   MApi_AUDIO_GetOutputSourceInfo(OUTPUT_SOURCE_INFO *pSourceInfo);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeRemove
/// @brief \b Function \b Name: MApi_AUDIO_ReadMailBox()
/// @brief \b Function \b Description:  This routine is used to read the DSP mail box value
/// @param bDspType    \b : Select audio DEC/SE DSP
/// @param u8ParamNum  \b : Mail box address
/// @return MS_U16     \b : Mail Box value
////////////////////////////////////////////////////////////////////////////////
 MS_U16  MApi_AUDIO_ReadMailBox(MS_BOOL bDspType, MS_U8 u8ParamNum);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeRemove
/// @brief \b Function \b Name: MDrv_AUDIO_WriteDecMailBox()
/// @brief \b Function \b Description:  This routine is used to write DSP mail box
/// @param bDspType    \b : Select audio DEC/SE DSP
/// @param u8ParamNum  \b : Mail box address
/// @param u16Data     \b : value
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_WriteMailBox(MS_BOOL bDspType, MS_U8 u8ParamNum, MS_U16 u16Data);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeRemove
/// @brief \b Function \b Name: MApi_AUDIO_FwTriggerDSP()
/// @brief \b Function \b Description:  This routine send a PIO interrupt to DSP with a command .
/// @param u16Cmd      \b :    0xF0-- for MHEG5 file protocol
////////////////////////////////////////////////////////////////////////////////
  void    MApi_AUDIO_FwTriggerDSP(MS_U16 u16Cmd);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_System
/// @brief \b Function \b Name: MApi_AUDIO_SetFwStatus()
/// @brief \b Function \b Description:  FW set DSP load/reload code from DDR or Flash
/// @param bBootOnDDR  \b :
///                     - TRUE==> Boot from DDR;
///                     - FALSE==> Boot from Flash
////////////////////////////////////////////////////////////////////////////////
  void    MApi_AUDIO_SetFwStatus(MS_BOOL en);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_System
/// @brief \b Function \b Name: MApi_AUDIO_ExitAudioSystem()
/// @brief \b Function \b Description: This routine is used to exit audio system
///                                  by performing DSP power down setting.
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_ExitAudioSystem(void);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_System
/// @brief \b Function \b Name: MApi_AUDIO_RebootDsp(void)
/// @brief \b Function \b Description:  This routine reboot DSP.
/// @param bDspType    \b : 0- DEC DSP; 1- SNDEff DSP
////////////////////////////////////////////////////////////////////////////////
  void    MApi_AUDIO_RebootDsp(MS_BOOL bDspType);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_System
/// @brief \b Function \b Name: MApi_AUDIO_SendIntrupt()
/// @brief \b Function \b Description: This routine send a PIO8 interrupt to DSP.
/// @param bDspType    \b :  0- DEC DSP; 1- SNDEff DSP
/// @param u8Cmd       \b :  0xE0, for MHEG5 file protocol
////////////////////////////////////////////////////////////////////////////////
  void    MApi_AUDIO_SendIntrupt(MS_BOOL bDspType,MS_U8 u8Cmd);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_System
/// @brief \b Function \b Name: MApi_AUDIO_I2S_SetMode()
/// @brief \b Function \b Description:  This routine is used to Set the I2S output mode.
/// @param u8Mode      \b : I2S mode select
///                       - AUDIO_I2S_MCLK               ==>Set I2S MCLK
///                       - AUDIO_I2S_WORD_WIDTH, ==>Set I2S Word-width
///                       - AUDIO_I2S_SOURCE_CH,    ==>Set I2S channel Source
///                       - AUDIO_I2S_FORMAT            ==>Set  I2S format
/// @param u8Val      \b : I2S mode value
///                      - I2S_MCLK_64FS=0x04,               ==> MCLK=64 Fs ;
///                      - I2S_MCLK_128FS=0x05,             ==> MCLK=128 Fs ;
///                      - I2S_MCLK_256FS=0x06,             ==> MCLK=256 Fs ; default setting
///                      - I2S_MCLK_384FS=0x07,             ==> MCLK=384 Fs ;
///                      - I2S_WORD_WIDTH_16BIT=0x4,  ==>Word width=16bit;
///                      - I2S_WORD_WIDTH_24BIT=0x5,  ==>Word width=24bit;
///                      - I2S_WORD_WIDTH_32BIT=0x6,  ==>Word width=32bit; default setting
///                      - I2S_FORMAT_STANDARD=0,       ==>I2S  in Standard format
///                      - I2S_FORMAT_LEFT_JUSTIFIED=1 ==> I2S in Left-justify format
///////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_I2S_SetMode(AUDIO_I2S_MODE_TYPE u8Mode, AUDIO_I2S_MODE_VALUE  u8Val);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_System
/// @brief \b Function \b Name: MApi_AUDIO_WritePreInitTable()
/// @brief \b Function \b Description: This routine is to set power on DAC sequence before setting init table.
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_WritePreInitTable(void);            //Refine power on sequence for earphone & DAC pop noise issue

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeRemove
/// @brief \b Function \b Name: MApi_AUDIO_EnaEarphone_LowPower_Stage()
/// @brief \b Function \b Description: This routine is used to enable earphone low power stage.
////////////////////////////////////////////////////////////////////////////////
  void    MApi_AUDIO_EnaEarphone_LowPower_Stage(void);   //Refine power on sequence for earphone & DAC pop noise issue

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeRemove
/// @brief \b Function \b Name: MApi_AUDIO_EnaEarphone_HighDriving_Stage()
/// @brief \b Function \b Description: This routine is used to enable earphone high driving stage.
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_EnaEarphone_HighDriving_Stage(void);//Refine power on sequence for earphone & DAC pop noise issue

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetDspBaseAddr()
/// @brief \b Function \b Description:  This function is used to set bin file base address in flash for Audio module.
/// @param u8Index     \b :
/// @param u32Bin_Base_Address    \b :
/// @param u32Mad_Base_Buffer_Adr \b :
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_SetDspBaseAddr(MS_U8 u8Index, MS_U32 u32Bin_Base_Address, MS_U32 u32Mad_Base_Buffer_Adr);

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_GetDspMadBaseAddr()
/// @brief \b Function \b Description:  This function is used to get the MAD base address.
/// @param u8Index     \b     :
/// @return MS_U32     \b     :
////////////////////////////////////////////////////////////////////////////////
 MS_U32  MApi_AUDIO_GetDspMadBaseAddr(MS_U8 u8Index);

//=============================================================
// AUDIO_SOUND RELATIONAL API FUNCTION
//=============================================================

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_BASIC_SOUNDEFFECT
/// @brief \b Function \b Name: MApi_SND_ProcessEnable()
/// @brief \b Function \b Description: This routine is used for adjust Common Sound parameters
////////////////////////////////////////////////////////////////////////////////
 MS_BOOL MApi_SND_ProcessEnable(Sound_ENABLE_Type Type, MS_BOOL enable);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeRemove
/// @brief \b Function \b Name: MApi_SND_SetParam1()
/// @brief \b Function \b Description: This routine is used for adjust Common Sound parameters
/// @return MS_BOOL     \b : True / False
////////////////////////////////////////////////////////////////////////////////
 MS_BOOL MApi_SND_SetParam1( Sound_SET_PARAM_Type Type, MS_U16 param1, MS_U16 param2);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeRemove
/// @brief \b Function \b Name: HAL_AUDIO_SOUND_SetParam()
/// @brief \b Function \b Description: This routine is used for adjust Common Sound parameters
/// @return MS_BOOL     \b : True / False
////////////////////////////////////////////////////////////////////////////////
 MS_U16  MApi_SND_GetParam1( Sound_GET_PARAM_Type Type, MS_U16 param1);

//******************************************************************************
/// @ingroup Audio_BASIC_SOUNDEFFECT
/// @brief \b Function \b Name: MApi_AUDIO_SOUND_SetParam()
/// @brief \b Function \b Description: This routine is used for adjust Common Sound parameters
/// @return MS_BOOL     \b : True / False
//******************************************************************************
  MS_BOOL MApi_AUDIO_SOUND_SetParam( Sound_SET_Type Type, MS_U32 *p_param );

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_BASIC_SOUNDEFFECT
/// @brief \b Function \b Name: MApi_AUDIO_SOUND_GetParam()
/// @brief \b Function \b Description: This routine is used for get Common Sound parameters
/// @param <IN>        \b pStrCmd : pointer to string command
/// @param <IN>        \b u32StrCmdLength : the string command size
/// @param <IN>        \b pData : pointer to data buffer. Each parameter size is U32 in data buffer.
/// @param <IN>        \b u32DataSize : the data buffer size
/// @param <RET>       \b MS_BOOL : On success, one is returned. On error, zero is returned.
/// @param <GLOBAL>    \b NONE :
///
/// Usage of *pStrCmd :
///
/// "GetSpectrumSize"
///     - Description : Get the request data buffer size of the spectrum parameters
///     - i.e.  "GetSpectrumSize"
///
/// "GetSpectrumParam"
///     - Description : Get the spectrum parameters
///     - i.e.  "GetSpectrum"
///
//******************************************************************************
  MS_BOOL MApi_AUDIO_SOUND_GetParam(const MS_S8 *pStrCmd, MS_U32 u32StrCmdLength, MS_S8 *pData, MS_U32 u32DataSize);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_BASIC_SOUNDEFFECT
/// @brief \b Function \b Name: MApi_AUDIO_SoundEffect_SetParam()
/// @brief \b Function \b Description: This routine is used for set Sound effect parameters
/// @param <IN>        \b pStrCmd : A signed char pointer to string command.
/// @param <IN>        \b u32StrCmdLength : the string command size
/// @param <IN>        \b pData : A signed integer pointer to a block of memory with specific configuration to deliver specific configuration.
/// @param <IN>        \b u32DataSize : the data buffer size
/// @param <RET>       \b MS_BOOL : On success, one is returned. On error, zero is returned.
/// @param <GLOBAL>    \b NONE :
///
/// Usage of *pStrCmd :
///
/// "SetPeq"
///     - Description : Set PEQ parameters
///     - i.e.  "SetPeq"
///
/// "SetDrc"
///     - Description : Set multi band DRC parameters
///     - i.e.  "SetDrc"
///
///
/// Usage of *pData :
///
/// *pStrCmd="SetPeq"
///     - Description : PEQ parameters
///     - parameter number : 6
///        -pData[0] :  [band]  0~15
///        -pData[1] :  [enable]  0/1 (disable/enable)
///        -pData[2] :  [type]  0~5 (peak/high pass/low pass/HSHELF/LSHELF)
///        -pData[3] :  [gain]  -180~+180 (-18dB~18dB)
///        -pData[4] :  [fc]  50~20000 (50Hz~20000Hz)
///        -pData[5] :  [Q]  5~160
///
/// *pStrCmd="SetDrc"
///     - Description : DRC parameters
///     - parameter number : 7
///        -pData[0] :  [band]  0~3
///        -pData[1] :  [target level]  0x1000~0x7FFFFF (-48~+18 dB)
///        -pData[2] :  [attack step]  0x1~0xC000 (0.0039~192 dB/ms)
///        -pData[3] :  [release step]  0x1~0xC000 (0.00098~48 dB/ms)
///        -pData[4] :  [ratio]  0x0~0x7FFFFF (infinite:1~1:1)
///        -pData[5] :  [hysteresis]  0x0~0xFF (0~1 dB)
///        -pData[6] :  [hold]  0x0~0x100 (0~1364 ms)
///
//******************************************************************************
  MS_BOOL MApi_AUDIO_SoundEffect_SetParam(const MS_S8 *pStrCmd, MS_U32 u32StrCmdLength, MS_S8 *pData, MS_U32 u32DataSize);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_BASIC_SOUNDEFFECT
/// @brief \b Function \b Name: MDrv_SOUND_AbsoluteVolume()
/// @brief \b Function \b Description: This routine is used to set the absolute u8Volume of audio u8Path.
/// @param u8Path      \b : for audio u8Path0 ~ u8Path6
/// @param u8Vol1      \b : MSB 7-bit register value of 10-bit u8Volume
///                         range from 0x00 to 0x7E , gain: +12db to   -114db (-1 db per step)
/// @param u8Vol2      \b : LSB 3-bit register value of 10-bit u8Volume
///                         range from 0x00 to 0x07 , gain:  -0db to -0.875db (-0.125 db per step)
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_SetAbsoluteVolume( MS_U8 u8Path, MS_U8 u8Vol1, MS_U8 u8Vol2 );

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_BASIC_SOUNDEFFECT
/// @brief \b Function \b Name: MApi_AUDIO_SetPreScale()
/// @brief \b Function \b Description: This routine is used to set the prescale of audio u8Path.
/// @param u8Path      \b : for audio u8Path0 ~ u8Path5
/// @param u8PreScale  \b : range from 0x01 to 0xFF , gain: -13.75db to +18db (0.125 db per step)
///                       - 0x00: disable pre-scale
///                       - 0x6F: gain = 0db
///                       - 0xFF: gain = +18db
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_SetPreScale ( MS_U8 u8Path, MS_U8 u8PreScale );

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_BASIC_SOUNDEFFECT
/// @brief \b Function \b Name: MApi_AUDIO_SetMute()
/// @brief \b Function \b Description: This routine is used to set audio path S/W mute.
/// @param u8Path      \b : for audio path0 ~ path6
/// @param EnMute      \b : TRUE --Mute;
///                         FALSE--Unmute
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_SetMute( MS_U8 u8Path, MS_BOOL EnMute);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_BASIC_SOUNDEFFECT
/// @brief \b Function \b Name: MApi_AUDIO_SourceConnect()
/// @brief \b Function \b Description: This routine is used to set audio mute by processor
/// @param u8Path      \b : eProcessor: Set Main/Sub processor,
/// @param EnMute      \b : TRUE --Mute;
///                                    FALSE--Unmute
////////////////////////////////////////////////////////////////////////////////
 void MApi_AUDIO_SourceConnect(AUDIO_OUTPORT_SOURCE_TYPE eConnectSourceType, MS_BOOL bIsConnect );

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_BASIC_SOUNDEFFECT
/// @brief \b Function \b Name: MApi_AUDIO_SetBalance()
/// @brief \b Function \b Description: This routine is used to set  balance value
/// @param u8Balance   \b : balance (0~100)
///                       - 0    :  R channel is muted ...
///                       - 50   :  L & R channel output the same level...
///                       -  100 :  L channel is muted .
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_SetBalance ( MS_U8 u8Balance );

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_BASIC_SOUNDEFFECT
/// @brief \b Function \b Name: MApi_AUDIO_EnableEQ()
/// @brief \b Function \b Description: This routine is used to enable EQ function.
/// @param EnEQ        \b : TRUE --Enable EQ;
///                                   FALSE--Disable EQ
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_EnableEQ ( MS_BOOL EnEQ );

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_BASIC_SOUNDEFFECT
/// @brief \b Function \b Name: MApi_AUDIO_EnableTone()
/// @brief \b Function \b Description: This routine is used to enable Bass/Treble function.
/// @param EnTone      \b : TRUE --Enable Bass/Treble;
///                         FALSE--Disable Bass/Treble
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_EnableTone ( MS_BOOL EnTone );

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_BASIC_SOUNDEFFECT
/// @brief \b Function \b Name: MApi_AUDIO_EnableAutoVolume()
/// @brief \b Function \b Description: This routine is used to enable auto volume(AVC) function.
/// @param EnAVC       \b : TRUE --Enable AVC;
///                        FALSE--Disable AVC
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_EnableAutoVolume ( MS_BOOL EnAVL );

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_BASIC_SOUNDEFFECT
/// @brief \b Function \b Name: MApi_AUDIO_EnableSurround()
/// @brief \b Function \b Description: This routine is used to enable surround function.
/// @param EnSurround   \b : TRUE --Enable Surround;
///                          FALSE--Disable Surround
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_EnableSurround ( MS_BOOL EnSurround );

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_BASIC_SOUNDEFFECT
/// @brief \b Function \b Name: MApi_AUDIO_EnableBalance()
/// @brief \b Function \b Description: This routine is used to enable Balance function.
/// @param EnBalance   \b : Balance Enable bits
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_EnableBalance( MS_U8 EnBalance );

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_BASIC_SOUNDEFFECT
/// @brief \b Function \b Name: MApi_AUDIO_SetDynamicBass()
/// @brief \b Function \b Description: This routine is used to set DynamicBass level.
/// @param u8Bass      \b :
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_SetDynamicBass( MS_U8 u8DynamicBass );

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_BASIC_SOUNDEFFECT
/// @brief \b Function \b Name: MApi_AUDIO_SetBass()
/// @brief \b Function \b Description: This routine is used to set Bass level.
/// @param u8Bass      \b :Bass Level (0~100)
///                       - CHIP_FAMILY_U3 & S4LE==>mapping to -16 ~ +15 dB Gain
///                       - CHIP_FAMILY_S7 ==>mapping to -12 ~ +12 dB Gain
///                       - CHIP_FAMILY_S7J       ==> mapping to -16 ~ +15 dB Gain
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_SetBass( MS_U8 u8Bass );

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_BASIC_SOUNDEFFECT
/// @brief \b Function \b Name: MApi_AUDIO_AbsoluteBass()
/// @brief \b Function \b Description: This routine is used to set abs. Bass value.
/// @param u8Bass      \b : register value
///                       - CHIP_FAMILY_U3 & S4LE ==>mapping to -16 ~ +15 dB Gain
///                       - CHIP_FAMILY_S7 ==>mapping to -12 ~ +12 dB Gain
///                       - CHIP_FAMILY_S7J       ==> mapping to -16 ~ +15 dB Gain
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_AbsoluteBass( MS_U8 u8Bass );

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_BASIC_SOUNDEFFECT
/// @brief \b Function \b Name: MApi_AUDIO_SetTreble()
/// @brief \b Function \b Description: This routine is used to set Treble level.
/// @param u8Treble    \b :Treble Level (0~100)
///                       - CHIP_FAMILY_U3 & S4LE ==>mapping to -16 ~ +15 dB Gain
///                       - CHIP_FAMILY_S7 ==>mapping to -12 ~ +12 dB Gain
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_SetTreble( MS_U8 u8Treble );

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_BASIC_SOUNDEFFECT
/// @brief \b Function \b Name: MApi_AUDIO_AbsoluteTreble()
/// @brief \b Function \b Description: This routine is used to set abs. Treble value.
/// @param u8Treble    \b : register value
///                       - CHIP_FAMILY_U3 & S4LE ==> mapping to -16 ~ +15 dB Gain
///                       - CHIP_FAMILY_S7        ==> mapping to -12 ~ +12 dB Gain
///                       - CHIP_FAMILY_S7J       ==> mapping to -16 ~ +15 dB Gain
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_AbsoluteTreble( MS_U8 u8Treble );

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_BASIC_SOUNDEFFECT
/// @brief \b Function \b Name: MApi_AUDIO_SetEq()
/// @brief \b Function \b Description: This routine is used to set the the 5-band EQ level  .
/// @param u8band      \b :  EQ band 0~4
/// @param u8level     \b :  Absolute EQ register value  ;
///                       - 0011-0000: +12.00db. (Max)
///                       - 0000-0001: +0.25db.
///                       - 0000-0000: +0.00db.
///                       - 1111-1111: -0.25db.
///                       - 1101-0000: -12.00db. (Min)
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_SetEq(MS_U8 u8band, MS_U8 u8level);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_BASIC_SOUNDEFFECT
/// @brief \b Function \b Name: MApi_AUDIO_SetEq_7band()
/// @brief \b Function \b Description: This routine is used to set the the 7-band EQ level  .
/// @param u8band      \b :  EQ band 0~6
/// @param u8level     \b :  Absolute EQ register value  ;
///                          0011-0000: +12.00db. (Max)
///                          0000-0001: +0.25db.
///                          0000-0000: +0.00db.
///                          1111-1111: -0.25db.
///                          1101-0000: -12.00db. (Min)
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_SetEq_7band(MS_U8 u8band, MS_U8 u8level);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_BASIC_SOUNDEFFECT
/// @brief \b Function \b Name: MApi_AUDIO_SetAvcThreshold()
/// @brief \b Function \b Description: This routine is used to set the AVC  threshold level.
/// @param Level       \b :  AVC threshold level
///                       - 0x00--  0    dBFS
///                       - 0x01--  -0.5    dBFS
///                       - 0x20--  -16    dBFS
///                       - 0x50--  -40    dBFS
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_SetAvcThreshold(MS_U8 Level);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_BASIC_SOUNDEFFECT
/// @brief \b Function \b Name: MApi_AUDIO_SetAvcMode()
/// @brief \b Function \b Description: This routine is used to set the AVC mode.
/// @param <IN>        \b u8AvcMode    :  AVC u8Mode
///                                    - 0: L mode
///                                    - 1: S mode
///                                    - 2: M mode
/// @image html L_mode.JPG "L mode"
/// @image html S_mode.JPG "S mode"
/// @image html M_mode.JPG "M mode"
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_SetAvcMode( MS_U8 AvcMode );

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_BASIC_SOUNDEFFECT
/// @brief \b Function \b Name: MApi_AUDIO_SetAvcAT()
/// @brief \b Function \b Description: This routine is used to set the AVC  attack time .
/// @param AvcAT       \b :  AVC attack time
///                       - AvcAT = 0 --> 20  ms
///                       - AvcAT = 1 --> 100 ms
///                       - AvcAT = 2 --> 200 ms
///                       - AvcAT = 3 --> 1 sec
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_SetAvcAT( MS_U8 AvcAT );

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_BASIC_SOUNDEFFECT
/// @brief \b Function \b Name: MApi_AUDIO_SetAvcRT()
/// @brief \b Function \b Description: This routine is used to set the AVC  release time .
/// @param AvcRT       \b :  AVC release time
///                       - AvcRT = 0 --> 1 sec
///                       - AvcRT = 1 --> 2 sec
///                       - AvcRT = 2 --> 100 ms
///                       - AvcRT = 3 --> 200 ms
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_SetAvcRT( MS_U8 AvcRT );

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_BASIC_SOUNDEFFECT
/// @brief \b Function \b Name: MApi_AUDIO_SetBufferProcess()
/// @brief \b Function \b Description: sets the value of audio delay time (ms)..
/// @param DelayTime   \b : 0x00~0xFA(0~250), each step=1 ms , 0~250 ms
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_SetBufferProcess(MS_U8 DelayTime);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_BASIC_SOUNDEFFECT
/// @brief \b Function \b Name: MApi_AUDIO_SetSurroundXA()
/// @brief \b Function \b Description: This routine is used to set the Surround xA echo Gain.
/// @param <IN>        \b mode    :     A Gain : register SURROUND[3:2]
///                       - 0 :    x0.1
///                       - 1 :    x0.15
///                       - 2 :    x0.2
///                       - 3 :    x0.25
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_SetSurroundXA( MS_U8 mode );

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_BASIC_SOUNDEFFECT
/// @brief \b Function \b Name: MApi_AUDIO_SetSurroundXB()
/// @brief \b Function \b Description: This routine is used to set the Surround xB echo Gain.
/// @param <IN>        \b mode    :   B Gain : register SURROUND[5:4]
///                       - 0 :    x0.25
///                       - 1 :    x0.3
///                       - 2 :    x0.35
///                       - 3 :    x0.45
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_SetSurroundXB( MS_U8 mode );

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_BASIC_SOUNDEFFECT
/// @brief \b Function \b Name: MApi_AUDIO_SetSurroundXK()
/// @brief \b Function \b Description: This routine is used to set the Surround xK feedback Gain.
/// @param <IN>        \b mode    :     K Gain : register SURROUND[10:8]
///                       - 0 :    x0.1
///                       - 1 :    x0.2
///                       - 2 :    x0.3
///                       - 3 :    x0.4
///                       - 4 :    x0.5
///                       - 5 :    x0.6
///                       - 6 :    x0.7
///                       - 7 :    x0.8
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_SetSurroundXK( MS_U8 mode );

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_BASIC_SOUNDEFFECT
/// @brief \b Function \b Name: MApi_AUDIO_SetSurroundLPFGain()
/// @brief \b Function \b Description: This routine is used to set the Surround LPF Gain.
/// @image html surround.JPG "Surround block diagram"
/// @param <IN>        \b mode    : LPF Gain : register SURROUND[7:6]
///                       - 0 :    0 dB
///                       - 1 :    2 dB
///                       - 2 :    4 dB
///                       - 3 :    un-support
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_SetSurroundLPFGain( MS_U8 mode );

/////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeRemove
/// @brief \b Function \b Name: MApi_AUDIO_ConvertVolumeUnit()
/// @brief \b Function \b Description: Convert UI Volume to Absolute Volume.
///                                                  valid value of UiVolume is between 0 and 100
/// @param UiVolume    \b :    UI Volume 0 -> 62, 50 -> 12, 100 -> 7
/// @param MS_U8       \b :    register value
////////////////////////////////////////////////////////////////////////////////
 MS_U8   MApi_AUDIO_ConvertVolumeUnit(MS_U8 UiVolume);

// void  MApi_AUDIO_SetMaxClipping( MS_U8 MaxClippingValue, MS_U16 Volume );

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_BASIC_SOUNDEFFECT
/// @brief \b Function \b Name: MApi_AUDIO_SetPEQCoef()
/// @brief \b Function \b Description: This routine is used to set PEQ Coefficient.
/// @param <IN>        \b peq_coef
/// @param <OUT>       \b NONE  :
/// @param <RET>       \b  NONE :
/// @param <GLOBAL>    \b NONE
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_SetPEQCoef(AUDIO_PEQ_COEF *peq_coef);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_BASIC_SOUNDEFFECT
/// @brief \b Function \b Name: MApi_AUDIO_EnablePEQ()
/// @brief \b Function \b Description: This routine is used to eable PEQ.
/// @param <IN>        \b Enpeq
/// @param <OUT>       \b NONE  :
/// @param <RET>       \b  NONE :
/// @param <GLOBAL>    \b NONE
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_EnablePEQ(MS_BOOL Enpeq);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_BASIC_SOUNDEFFECT
/// @brief \b Function \b Name: MApi_AUDIO_EnableDcRemove()
/// @brief \b Function \b Description: This routine is used to eable PEQ.
/// @param <IN>        \b EnDcRemove
/// @param <OUT>       \b True / False
/// @param <RET>       \b  NONE :
/// @param <GLOBAL>    \b NONE
////////////////////////////////////////////////////////////////////////////////
  MS_BOOL MApi_AUDIO_EnableDcRemove(MS_BOOL EnDcRemove);

//============================================================
// AUDIO_DTV RELATIONAL API FUNCTION
//============================================================

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_System
/// @brief \b Function \b Name: MApi_AUDIO_ReleaseDecodeSystem()
/// @brief \b Function \b Description:  This function will release/close decode system
/// @param DecId  \b : Decoder ID to be released
/// @return\b MS_BOOL  \b : set decoder system process status FALSE-> fail TRUE-> Success
////////////////////////////////////////////////////////////////////////////////
 MS_BOOL MApi_AUDIO_ReleaseDecodeSystem(AUDIO_DEC_ID DecId);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_System
/// @brief \b Function \b Name: MApi_AUDIO_SetDecodeSystem()
/// @brief \b Function \b Description:  This function will set DecId decoder as  structure p_AudioDecStatus
/// @param\b DecId  \b : Decoder ID to be controled
/// @param\b p_AudioDecStatus  \b : decoder status structure pointer
/// @return\b MS_BOOL  \b : set decoder system process status FALSE-> fail TRUE-> Success
////////////////////////////////////////////////////////////////////////////////
 MS_BOOL MApi_AUDIO_SetDecodeSystem(AUDIO_DEC_ID DecId, AudioDecStatus_t * p_AudioDecStatus);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_System
/// @brief \b Function \b Name: MApi_AUDIO_GetDecodeSystem()
/// @brief \b Function \b Description:  This function will get DecId decoder status
/// @param DecId  \b : Decoder ID to get status
/// @param p_AudioDecStatus  \b : return status to p_AudioDecStatus pointer
/// @return\b MS_BOOL  \b : set decoder system process status FALSE-> fail TRUE-> Success
////////////////////////////////////////////////////////////////////////////////
 MS_BOOL MApi_AUDIO_GetDecodeSystem(AUDIO_DEC_ID DecId, AudioDecStatus_t * p_AudioDecStatus);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_System
/// @brief \b Function \b Name: MApi_AUDIO_SetDecodeCmd()
/// @brief \b Function \b Description:  This function will send decode command to DecId decoder
/// @param DecId  \b : Decoder ID to be controled
/// @param enDecComamnd  \b : decoder command
/// @return\b MS_BOOL  \b : set decoder system process status FALSE-> fail TRUE-> Success
////////////////////////////////////////////////////////////////////////////////
 MS_BOOL MApi_AUDIO_SetDecodeCmd(AUDIO_DEC_ID DecId, En_DVB_decCmdType enDecComamnd);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_System
/// @brief \b Function \b Name: MApi_AUDIO_OpenDecodeSystem()
/// @brief \b Function \b Description:  This function will open decode system according audio dec status
/// @param p_AudioDecStatus  \b : decoder status stucture
/// @return\b AUDIO_DEC_ID  \b : return available decoder ID
////////////////////////////////////////////////////////////////////////////////
 AUDIO_DEC_ID MApi_AUDIO_OpenDecodeSystem(AudioDecStatus_t * p_AudioDecStatus);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeRemove
/// @brief \b Function \b Name: MApi_AUDIO_SetSystem()
/// @brief \b Function \b Description:  This function will load
///                                     different audio decoder to audio system.
///
///                                     In Mstar audio system, we can support 1 or 2 decoders.
///                                     One is for main DTV audio or Multimedia audio player.
///                                     Another is for second audio decode (ex: used for Audio Description purpose)
///
/// @param enDecSystem  \b : (En_DVB_decSystemType) decoder type
///                        - parameter type "MSAPI_AUD_DVB_XXXX" is decoder type for main audio decoder
///                        - parameter type "MSAPI_AUD_DVB2_XXXX" is decoder type for second audio decoder
////////////////////////////////////////////////////////////////////////////////
 MS_BOOL MApi_AUDIO_SetSystem(En_DVB_decSystemType enDecSystem);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_System
/// @brief \b Function \b Name: MApi_AUDIO_GetDecSysSupportStatus()
/// @brief \b Function \b Description: This function will report Audio Decoder support feature status when giving DecSystem type
/// @param <IN>        \b   enDecSystem : Decoder type
/// @param <RET>       \b   BOOL        : True or False
////////////////////////////////////////////////////////////////////////////////
 MS_BOOL MApi_AUDIO_GetDecSysSupportStatus(En_DVB_decSystemType enDecSystem);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeRemove
/// @brief \b Function \b Name: MApi_AUDIO_SetH264StreamID_Mod()
/// @brief \b Function \b Description: This routine is used to AC3 Stream ID mod.
/// @param stream_id_mod  \b : DTV stream ID
///                          - stream_id_mod = 0, video MPEG mode;
///                          - stream_id_mod = 1, video H264 mode
////////////////////////////////////////////////////////////////////////////////
  void    MApi_AUDIO_SetH264StreamID_Mod(MS_U8 stream_id_mod);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_System
/// @brief \b Function \b Name: MApi_AUDIO_IsMadLock()
/// @brief \b Function \b Description: This function show DTV Audio stream lock status
/// @return MS_BOOL    \b :       1 : lock  ; 0 : unlock
////////////////////////////////////////////////////////////////////////////////
 MS_BOOL MApi_AUDIO_IsMadLock(void);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeRemove
/// @brief \b Function \b Name: MApi_AUDIO_GetDecStatus()
/// @brief \b Function \b Description: This routine is used to get the decoder's status (paly or stop)
/// @return En_DVB_decCmdType \b : Decoder status (paly or stop)
////////////////////////////////////////////////////////////////////////////////
 En_DVB_decCmdType MApi_AUDIO_GetDecStatus ( void );

///////////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_System
/// @brief \b Function \b Name: MApi_AUDIO_GetCommand()
/// @brief \b Function \b Description: This routine is to get  the DVB1 decode command.
/// @return AU_DVB_DECCMD \b :
///                          - AU_DVB_DECCMD_STOP,        //0
///                         - AU_DVB_DECCMD_PLAY,        //1
///                         - AU_DVB_DECCMD_PLAYFILETSP, //2
///                          - AU_DVB_DECCMD_RESYNC,
///                                 ....etc
///////////////////////////////////////////////////////////////////////////////////////
 AU_DVB_DECCMD MApi_AUDIO_GetCommand(En_DVB_AudioDecoder AdoDecoderNo);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_System
/// @brief \b Function \b Name: MApi_AUDIO_SetCommand()
/// @brief \b Function \b Description: Set decoder Command for Digital Audio module
/// @param enDecComamnd \b : deccoder command for DVB Audio
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_SetCommand(En_DVB_decCmdType enDecComamnd);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_System
/// @brief \b Function \b Name: MApi_AUDIO_GetMAD_LOCK()
/// @brief \b Function \b Description: This routine is used to report decoder lock information.
/// @retun MS_U8      \b the decoder status
///////////////////////////////////////////////////////////////////////////////
 MS_BOOL MApi_AUDIO_GetMAD_LOCK(void);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_System
/// @brief \b Function \b Name: MApi_AUDIO_SetADOutputMode()
/// @brief \b Function \b Description: Set AD output mode
/// @param out_mode    \b : out_mode:
///                       - AD_OUT_SPEAKER
///                       - AD_OUT_HP
///                       - AD_OUT_BOTH
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_SetADOutputMode (MS_U8 out_mode);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_System
/// @brief \b Function \b Name: MApi_AUDIO_SetADAbsoluteVolume()
/// @brief \b Function \b Description: This routine is used to set the absolute u8Volume of AD.
/// @param AbsVolume   \b : MSB 7-bit register value of 10-bit u8Volume
///                         range from 0x00 to 0x7E , gain: +12db to   -114db (-1 db per step)
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_SetADAbsoluteVolume (MS_U8 AbsVolume);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_System
/// @brief \b Function \b Name: MApi_AUDIO_ADSetMute()
/// @brief \b Function \b Description: This routine is used to set AD mute.
/// @param EnMute      \b :     TRUE --Mute AD;
///                             FALSE--Unmute AD
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_ADSetMute(MS_BOOL EnMute);

//============================================================
// AUDIO_PVR RELATED FUNCTIONS
//============================================================

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeRemove
/// @brief \b Function \b Name: MDrv_AUDIO_SetEncodeInit()
/// @brief \b Function \b Description:  This function is used to set Encod init
/// @param BitRate \b :   Encode Output Bitrate
///                      -  AU_DVB_ENCBR_192K,    // 0
///                      -  AU_DVB_ENCBR_384K,    // 1
///                      -  AU_DVB_ENCBR_128K,    // 2
///                      -  AU_DVB_ENCBR_64K      // 3
/// @param EncFrameNum \b :   Frame number per interrupt
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_SetEncInit(AU_DVB_ENCBR BitRate, MS_U8 u8EncFrameNum);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeRemove
/// @brief \b Function   \b Name: MApi_AUDIO_SetEncCommand()
/// @brief \b Function   \b Description: Set Encoder Command for Digital Audio module
/// @param enEnccComamnd \b : Encoder command for DVB Audio
///                                       -   AU_DVB_ENCCMD_STOP,      //0
///                                       -   AU_DVB_ENCCMD_START      // 1
////////////////////////////////////////////////////////////////////////////////
  void    MApi_AUDIO_SetEncCommand(AU_DVB_ENCCMD enEncComamnd);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeRemove
/// @brief \b Function \b Name: MDrv_AUDIO_GetEncodeFrameInfo()
/// @brief \b Function \b Description:  This function is used to get the Encoded frame info
/// @param input       \b EncFrameInfo:   structure to store encoded frame info
////////////////////////////////////////////////////////////////////////////////
  void    MApi_AUDIO_GetEncodeFrameInfo(AU_DVB_ENC_FRAME_INFO *EncFrameInfo);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeRemove
/// @brief \b Function \b Name: MApi_AUDIO_GetEncodeDoneFlag()
/// @brief \b Function \b Description:  This function is used to get check encode done flag
/// @param input       \b None
/// @param output      \b BOOL  TRUE/FALSE
////////////////////////////////////////////////////////////////////////////////
 MS_BOOL MApi_AUDIO_GetEncodeDoneFlag(void);

//============================================================
// AUDIO_ATV RELATIONAL API FUNCTION
//============================================================

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ATV_SPDIF_HDMI
/// @brief \b Function \b Name: MApi_AUDIO_SIF_SetStandard()
/// @brief \b Function \b Description: Set SIF standard
/// @param system_type \b : standard type
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_SIF_SetStandard(AUDIOSTANDARD_TYPE standard_type);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ATV_SPDIF_HDMI
/// @brief \b Function \b Name: MApi_AUDIO_SIF_GetSoundMode()
/// @brief \b Function \b Description: Get SIF BTSC sound mode type
/// @return MS_U8      \b : Sound mode type
///                       - MS_U8 = 0x0 : Mute
///                       - bit 0 : Mono
///                       - bit 1 : Stereo
///                       - bit 2 : SAP
////////////////////////////////////////////////////////////////////////////////
 MS_U8   MApi_AUDIO_SIF_GetSoundMode(void);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ATV_SPDIF_HDMI
/// @brief \b Function \b Name: MApi_AUDIO_SIF_SetSoundMode()
/// @brief \b Function \b Description: Set sound mode for ATV source.
/// @param u8SifSoundMode  \b : sound mode type
///                           - AU_MODE_MONO
///                           - AU_MODE_STEREO
///                           - AU_MODE_SAP
///                           - AU_MODE_LANG_A
///                           - AU_MODE_LANG_B
///                           - AU_MODE_NICAM_MONO, ...etc
/// @return MS_BOOL    \b : success / fail
////////////////////////////////////////////////////////////////////////////////
 MS_BOOL MApi_AUDIO_SIF_SetSoundMode(MS_U8 u8SifSoundMode);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ATV_SPDIF_HDMI
/// @brief \b Function \b Name: MApi_AUDIO_SIF_GetAudioStatus()
/// @brief \b Function \b Description: Get the current audio status.
///                                    It can be used for getting available sound modes and real-time audio standard monitor.
/// @param eAudioStatus \b : audio status pointer
/// @return MS_BOOL     \b : TRUE/FALSE
////////////////////////////////////////////////////////////////////////////////
 MS_BOOL MApi_AUDIO_SIF_GetAudioStatus(AUDIOSTATUS * eAudioStatus);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ATV_SPDIF_HDMI
/// @brief \b Function \b Name: MApi_AUDIO_SIF_StartAutoStandardDetection()
/// @brief \b Function \b Description: To start automatic detection about input audio standard
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_SIF_StartAutoStandardDetection(void);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ATV_SPDIF_HDMI
/// @brief \b Function \b Name: MApi_AUDIO_SIF_GetResultOfAutoStandardDetection()
/// @brief \b Function \b Description: To get result of audio standard after automatic detection called by MDrv_ASP_StartAutoStandardDetection.
/// @return AUDIOSTANDARD_TYPE \b :
////////////////////////////////////////////////////////////////////////////////
 AUDIOSTANDARD_TYPE MApi_AUDIO_SIF_GetResultOfAutoStandardDetection(void);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ATV_SPDIF_HDMI
/// @brief \b Function \b Name: MApi_AUDIO_SIF_ConvertToBasicAudioStandard()
/// @brief \b Function \b Description: Convert the audio standard to the basic standard
/// @param eStandard   \b : Please refer AUDIOSTANDARD_TYPE enum in header file
/// @return AUDIOSTANDARD_TYPE \b : basic standard
///                               - E_AUDIOSTANDARD_BG
///                               - E_AUDIOSTANDARD_I
///                               - E_AUDIOSTANDARD_DK
///                               - E_AUDIOSTANDARD_L
///                               - E_AUDIOSTANDARD_M
///                               - E_AUDIOSTANDARD_NOTSTANDARD
////////////////////////////////////////////////////////////////////////////////
 AUDIOSTANDARD_TYPE MApi_AUDIO_SIF_ConvertToBasicAudioStandard(AUDIOSTANDARD_TYPE eStandard);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ATV_SPDIF_HDMI
/// @brief \b Function \b Name: MApi_AUDIO_SIF_SetThreshold()
/// @brief \b Function \b Description:  This routine is used to set SIF threshold
/// @param ThrTbl  \b : SIF thresholds Table
////////////////////////////////////////////////////////////////////////////////
#if defined(UFO_PUBLIC_HEADER_212)
 void    MApi_AUDIO_SIF_SetThreshold(THR_TBL_TYPE code *ThrTbl);
#else
 void    MApi_AUDIO_SIF_SetThreshold(THR_TBL_TYPE *ThrTbl);
#endif
////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ATV_SPDIF_HDMI
/// @brief \b Function \b Name: MApi_AUDIO_SIF_SetPrescale()
/// @brief \b Function \b Description: SIF output level = (original level add db_value gain)
////////////////////////////////////////////////////////////////////////////////
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
 void    MApi_AUDIO_SIF_SetPrescale(MS_U8, int);
#else
 void    MApi_AUDIO_SIF_SetPrescale(MS_U8, MS_S32);
#endif

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ATV_SPDIF_HDMI
/// @brief \b Function \b Name: MApi_AUDIO_SIF_IsPALType()
/// @brief \b Function \b Description: Check the DSP type is pal_type or not
/// @return MS_BOOL    \b : TRUE/FALSE
////////////////////////////////////////////////////////////////////////////////
 MS_BOOL MApi_AUDIO_SIF_IsPALType(AUDIO_SIF_PAL_TYPE pal_type);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ATV_SPDIF_HDMI
/// @brief \b Function \b Name: MApi_AUDIO_SIF_SetPALType()
/// @brief \b Function \b Description: Set DSP type to pal_type
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_SIF_SetPALType(AUDIO_SIF_PAL_TYPE pal_type);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ATV_SPDIF_HDMI
/// @brief \b Function \b Name: MApi_AUDIO_SIF_SetCommand()
/// @brief \b Function \b Description: Set decoder Command for ATV module
/// @param enDecComamnd \b : deccoder command for ATV AUDIO
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_SIF_SendCmd(En_AUD_SIF_CmdType enAudSifCommand, MS_U8 comm_arg1, MS_U8 comm_arg2);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ATV_SPDIF_HDMI
/// @brief \b Function \b Name: MApi_AUDIO_SIF_Shift()
/// @brief \b Function \b Description: This function is used to provide a API for application to choose what kind of VIF mdoe.
///
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_SIF_Shift(En_AUD_VIF_Type type);

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_TriggerSifPLL()
/// @brief \b Function \b Description:  This function is used to initialize SIF analog part .
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_TriggerSifPLL(void);

//============================================================
// AUDIO_FM_RADIO RELATIONAL API FUNCTION
//============================================================

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ATV_SPDIF_HDMI
/// @brief \b Function \b Name: MApi_AUDIO_FM_RADIO_GetSoundMode()
/// @brief \b Function \b Description: Getting FM Radio Sound Mode
/// @param <IN>        \b NONE :
/// @param <OUT>       \b MS_U8   : return E_AUDIOMODE_MONO, E_AUDIOMODE_K_STEREO or E_AUDIOMODE_INVALID
////////////////////////////////////////////////////////////////////////////////
  MS_U8 MApi_AUDIO_FM_RADIO_GetSoundMode(void);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ATV_SPDIF_HDMI
/// @brief \b Function \b Name: MApi_AUDIO_FM_RADIO_SetSoundMode()
/// @brief \b Function \b Description: Setting FM Radio Sound Mode
/// @param <IN>        \b MS_U8 : u8FMRadioSoundMode
/// @param <OUT>       \b NONE :
////////////////////////////////////////////////////////////////////////////////
  void MApi_AUDIO_FM_RADIO_SetSoundMode(MS_U8 u8FMRadioSoundMode);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ATV_SPDIF_HDMI
/// @brief \b Function \b Name: MApi_AUDIO_FM_RADIO_DeEmphassisOption()
/// @brief \b Function \b Description: FM Radio De-emphassis Option
/// @param <IN>        \b MS_BOOL : Is_Europe
/// @param <OUT>       \b NONE :
////////////////////////////////////////////////////////////////////////////////
 void MApi_AUDIO_FM_RADIO_DeEmphassisOption(MS_BOOL Is_Europe);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ATV_SPDIF_HDMI
/// @brief \b Function \b Name: MApi_AUDIO_FM_RADIO_GET_DC_AMP()
/// @brief \b Function \b Description: FM Radio get DC amp
/// @param <IN>        \b NONE
/// @param <OUT>       \b MS_S8 DC amp
////////////////////////////////////////////////////////////////////////////////
 MS_S8 MApi_AUDIO_FM_RADIO_GET_DC_AMP(void);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ATV_SPDIF_HDMI
/// @brief \b Function \b Name: MApi_AUDIO_FM_RADIO_GET_NSR()
/// @brief \b Function \b Description: FM Radio get NSR
/// @param <IN>        \b NONE
/// @param <OUT>       \b MS_U8 NSR
////////////////////////////////////////////////////////////////////////////////
  MS_U8 MApi_AUDIO_FM_RADIO_GET_NSR(void);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ATV_SPDIF_HDMI
/// @brief \b Function \b Name: MApi_AUDIO_FM_RADIO_RESET()
/// @brief \b Function \b Description: FM Radio reset
/// @param <IN>        \b NONE
/// @param <OUT>       \b NONE
////////////////////////////////////////////////////////////////////////////////
 void MApi_AUDIO_FM_RADIO_RESET(void);

//=============================================================
// AUDIO_SPDIF RELATIONAL API FUNCTION
//=============================================================

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ATV_SPDIF_HDMI
/// @brief \b Function \b Name: MApi_AUDIO_SPDIF_HWEN()
/// @brief \b Function \b Description:  This routine is used to enable S/PDIF output (Hardware)
/// @param spdif_en    \b :    0--Disable S/PDIF out;
///                            1--Enable S/PDIF out
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_SPDIF_HWEN(MS_BOOL spdif_hwen);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ATV_SPDIF_HDMI
/// @brief \b Function \b Name: MApi_AUDIO_SPDIF_SetMute()
/// @brief \b Function \b Description:  This routine is used to enable S/PDIF output (Hardware)
/// @param mute_en     \b :    0--Disable mute;
///                            1--Enable mute
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_SPDIF_SetMute(MS_BOOL mute_en);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ATV_SPDIF_HDMI
/// @brief \b Function \b Name: MApi_AUDIO_SPDIF_SetMode()
/// @brief \b Function \b Description:  This routine is used to set S/PDIF mode.
/// @param spdif_mode  \b : PCM or Non-PCM mode
///                - spdif_mode = 0x0 : SPDIF enable, PCM mode
///                - spdif_mode = 0x1 : SPDIF off
///                - spdif_mode = 0x2 : SPDIF enable, nonPCM mode
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_SPDIF_SetMode(MS_U8 spdif_mode);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ATV_SPDIF_HDMI
/// @brief \b Function \b Name: MApi_AUDIO_SPDIF_GetMode()
/// @brief \b Function \b Description:  This routine is used to get S/PDIF mode.
/// @return MS_U8      \b :  register SPDIF_CTRL value
///                       - 0x0 : SPDIF enable, PCM mode
///                       - 0x1 : SPDIF OFF
///                       - 0x2 : SPDIF enable, non-PCM mode
////////////////////////////////////////////////////////////////////////////////
  MS_U8   MApi_AUDIO_SPDIF_GetMode(void);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ATV_SPDIF_HDMI
/// @brief \b Function \b Name: MApi_AUDIO_SPDIF_SetSCMS()
/// @brief \b Function \b Description:  This routine is used to set SPDIF SCMS.
/// @param C_bit_en  \b : copy right control bit
/// @param L_bit_en  \b : generation bit
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_SPDIF_SetSCMS(MS_U8 C_bit_en, MS_U8 L_bit_en);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ATV_SPDIF_HDMI
/// @brief \b Function \b Name: MApi_AUDIO_SPDIF_GetSCMS()
/// @brief \b Function \b Description:  This routine is used to get SPDIF SCMS.
/// @return MS_U8      \b :  SCMS[0] = C bit status, SCMS[1] = L bit status
////////////////////////////////////////////////////////////////////////////////
 MS_U8   MApi_AUDIO_SPDIF_GetSCMS(void);
 void    MApi_AUDIO_SPDIF_SetOutputType(AUDIO_SPDIF_OUTPUT_TYPE eType); // New API for S/PDIF output auto/PCM mode switching

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ATV_SPDIF_HDMI
/// @brief \b Function \b Name: MApi_Audio_SPDIF_Monitor()
/// @brief \b Function \b Description:
/// @param pau_info    \b :  AUDIO INIT STRUCTURE
////////////////////////////////////////////////////////////////////////////////
void    MApi_Audio_SPDIF_Monitor(void);
 void    MApi_Audio_Monitor_SPDIF_NONPCM_SmpRate(void);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ATV_SPDIF_HDMI
/// @brief \b Function \b Name: MApi_AUDIO_SPDIF_ChannelStatus_CTRL()
/// @brief \b Function \b Description:
/// @param pau_info    \b :  AUDIO INIT STRUCTURE
////////////////////////////////////////////////////////////////////////////////
 MS_BOOL MApi_AUDIO_SPDIF_ChannelStatus_CTRL(AUDIO_SPDIF_CS_TYPE cs_mode, AUDIO_SPDIF_CS_TYPE_STATUS status);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ATV_SPDIF_HDMI
/// @brief \b Function \b Name: HAL_AUDIO_Digital_Out_SetChannelStatus()
/// @brief \b Function \b Description: This routine is used to set SPDIF/HDMI/ARC channel status.
/// @param <IN>        \b   eType   :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
////////////////////////////////////////////////////////////////////////////////
 MS_BOOL MApi_AUDIO_DigitalOut_SetChannelStatus(DIGITAL_OUTPUT_TYPE ePath, Digital_Out_Channel_Status_t *stChannelStatus);


////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ATV_SPDIF_HDMI
/// @brief \b Function \b Name: MApi_AUDIO_DigitalOut_SetDeviceCapability()
/// @brief \b Function \b Description: This routine is used to set output device capability.
/// @param <IN>        \b eoutType: output type (ARC_OUTPUT or HDMI_OUTPUT)
/// @param <IN>        \b ecodecType: kind of codec
/// @param <IN>        \b *p_codecCapability: codec support capability structure
/// @param <RET>       \b bRet: return api setting success or fail
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MApi_AUDIO_DigitalOut_SetDeviceCapability(DIGITAL_OUTPUT_TYPE eoutType, AUDIO_FORMAT_CODE ecodecType, Digital_Out_Device_Capability_t *p_codecCapability);

//=============================================================
// AUDIO_HDMI RELATIONAL API FUNCTION
//=============================================================

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ATV_SPDIF_HDMI
/// @brief \b Function \b Name: MApi_AUDIO_HDMI_Tx_SetMute()
/// @brief \b Function \b Description:  This routine is used to enable HDMI Tx output (Software)
/// @param mute_en     \b :    0--Disable mute;
///                            1--Enable mute
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_HDMI_Tx_SetMute(MS_BOOL mute_en);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ATV_SPDIF_HDMI
/// @brief \b Function \b Name: MApi_AUDIO_HDMI_Tx_GetStatus()
/// @brief \b Function \b Description:  This routine is used to get HDMI Tx status
/// @param onOff     \b : HDMI tx function is availible in this chip ?
/// @param hdmi_SmpFreq \b : Audio current sample freq for tx
/// @param outType \b   : PCM mode or nonPCM mode
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_HDMI_Tx_GetStatus(MS_BOOL *onOff, AUDIO_FS_TYPE *hdmi_SmpFreq, HDMI_TX_OUTPUT_TYPE *outType );

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ATV_SPDIF_HDMI
/// @brief \b Function \b Name: MApi_AUDIO_HDMI_Monitor()
/// @brief \b Function \b Description: This function report HDMI non-PCM or PCM format (register STATUS_INPUT)
/// @return MS_U8      \b :
///                     - non-PCM --> register STATUS_INPUT[7:6]= 0x1;
///                     - PCM   --> register STATUS_INPUT[7:6]= 0x0
////////////////////////////////////////////////////////////////////////////////
  MS_U8   MApi_AUDIO_HDMI_Monitor(void);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ATV_SPDIF_HDMI
/// @brief \b Function \b Name: MApi_AUDIO_HDMI_GetNonpcmFlag()
/// @brief \b Function \b Description: This function report HDMI non-PCM or PCM format (register STATUS_INPUT)
/// @return MS_BOOL    \b :
///                     - TRUE : non-PCM --> register STATUS_INPUT[7:6]= 0x1 ;
///                     - FALSE : PCM     --> register STATUS_INPUT[7:6]= 0x0
////////////////////////////////////////////////////////////////////////////////
 MS_BOOL MApi_AUDIO_HDMI_GetNonpcmFlag(void);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ATV_SPDIF_HDMI
/// @brief \b Function \b Name: MApi_AUDIO_HDMI_SetNonpcm()
/// @brief \b Function \b Description: This function setting HDMI non-PCM or PCM relational register
/// @param nonPCM_en   \b :    1: nonPCM; 0: PCM
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_HDMI_SetNonpcm(MS_U8 nonPCM_en);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ATV_SPDIF_HDMI
/// @brief \b Function \b Name: MApi_AUDIO_HDMI_RX_SetNonpcm()
/// @brief \b Function \b Description: This function setting HDMI non-PCM or PCM relational register
/// @param nonPCM_en   \b :    1: nonPCM; 0: PCM
/// @return AUDIO_HDMI_RX_TYPE \b : Return HDMI RX Type
////////////////////////////////////////////////////////////////////////////////
 AUDIO_HDMI_RX_TYPE MApi_AUDIO_HDMI_RX_SetNonpcm(MS_U8 nonPCM_en);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ATV_SPDIF_HDMI
/// @brief \b Function \b Name: MApi_AUDIO_HDMI_RX_SetNonpcm()
/// @brief \b Function \b Description: This function setting HDMI non-PCM or PCM relational register
/// @param nonPCM_en   \b :    1: nonPCM; 0: PCM
/// @return AUDIO_HDMI_RX_TYPE \b : Return HDMI RX Type
////////////////////////////////////////////////////////////////////////////////
 AUDIO_HDMI_RX_TYPE MApi_AUDIO_HDMI_RX_GetNonPCM(void);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ATV_SPDIF_HDMI
/// @brief \b Function \b Name: MApi_AUDIO_DTV_HDMI_CFG()
/// @brief \b Function \b Description:  Set DVB/HDMI AC3 path configure
/// @param ctrl    \b : 0: DVB2_AD path;
///                     1: HDMI path
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_DTV_HDMI_CFG(MS_U8 ctrl);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ATV_SPDIF_HDMI
/// @brief \b Function \b Name: MApi_AUDIO_HDMI_GetSynthFreq()
/// @brief \b Function \b Description:  Audio HDMI CTS-N synthesizer input signal detect.
/// @return MS_U16     \b : return freq. If no signal, return 0
////////////////////////////////////////////////////////////////////////////////
 MS_U16  MApi_AUDIO_HDMI_GetSynthFreq(void);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ATV_SPDIF_HDMI
/// @brief \b Function \b Name: MApi_AUDIO_HDMI_SetDownSample()
/// @brief \b Function \b Description:  set HDMI downsample rate
/// @param ratio       \b : donwsample ratio
///                       - 00: Normal (from 1x to 1x).
///                       - 01: Down sample from 2x to 1x.
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_HDMI_SetDownSample(MS_U8 ratio);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ATV_SPDIF_HDMI
/// @brief \b Function \b Name: MApi_AUDIO_HDMI_SetOutputType()
/// @brief \b Function \b Description:  To set HDMI output type independently
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_HDMI_TX_SetMode(HDMI_TX_OUTPUT_TYPE outType);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_System
/// @brief \b Function \b Name: MApi_AUDIO_HDMI_RX_GetHdmiInAudioStatus()
/// @brief \b Function \b Description:  Return audio status.
/// @return MS_U16     \b : return structure which include pcm, non pcm, sampling rate.
////////////////////////////////////////////////////////////////////////////////
 MS_BOOL MApi_AUDIO_HDMI_RX_GetHdmiInAudioStatus( ST_HDMI_AUDIO_STATUS *p_hdmiAudioSts);

//=============================================================
// AUDIO_ADVANCE_SOUND RELATIONAL API FUNCTION
//=============================================================

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ADVANCED_SOUNDEFFECT
/// @brief \b Function \b Name: MApi_AUDIO_ADVSOUND_ProcessEnable
/// @brief \b Function \b Description: This routine is used to enable process of advanced sound effect.
/// @param <IN>        \b ADVSND_TYPE type
/// @param <OUT>       \b NONE  :
/// @param <RET>       \b  NONE :
/// @param <GLOBAL>    \b NONE
////////////////////////////////////////////////////////////////////////////////
 MS_BOOL MApi_AUDIO_ADVSOUND_ProcessEnable(ADVSND_TYPE type);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ADVANCED_SOUNDEFFECT
/// @brief \b Function \b Name: MApi_AUDIO_ADVSOUND_SubProcessEnable(ADVFUNC proc, BOOL enable)
/// @brief \b Function \b Description: This routine is used to enable subprocess of advanced sound effect.
/// @param <IN>        \b proc, enable
/// @param <OUT>       \b NONE  :
/// @param <RET>       \b  NONE :
/// @param <GLOBAL>    \b NONE
////////////////////////////////////////////////////////////////////////////////
 MS_BOOL MApi_AUDIO_ADVSOUND_SubProcessEnable(ADVFUNC proc, MS_BOOL enable);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ADVANCED_SOUNDEFFECT
/// @brief \b Function \b Name: MApi_AUDIO_ADVSOUND_SetParam(ADVSND_PARAM param, U16 value1, U16 value2)
/// @brief \b Function \b Description: This routine is used to set paramters advanced sound effect.
/// @param <IN>        \b param, value1, value2
/// @param <OUT>       \b NONE  :
/// @param <RET>       \b  NONE :
/// @param <GLOBAL>    \b NONE
////////////////////////////////////////////////////////////////////////////////
 MS_BOOL MApi_AUDIO_ADVSOUND_SetParam(ADVSND_PARAM param, MS_U16 u16value1, MS_U16 u16value2);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeRemove
/// @brief \b Function \b Name: MApi_AUDIO_ADVSOUND_SetParam(ADVSND_PARAM param, U32 value1, U16 value2)
/// @brief \b Function \b Description: This routine is used to set paramters advanced sound effect.
/// @param <IN>        \b param, u32value1, u16value2
///                              \b Parameter u32value1: range 0~0x7FFFFF
///                              \b Parameter u16value2: 0/1,
///                              - 0 : if u32value1 is u16, none fix point 24 bit parameter
///                              - 1 : if u32value1 is u32, fix point 24 bit parameter
/// @param <OUT>       \b NONE  :
/// @return <OUT>       \b FALSE : Setting Fail
///                               \b TRUE  : Setting OK
/// @param <GLOBAL>    \b NONE
////////////////////////////////////////////////////////////////////////////////
 MS_BOOL MApi_AUDIO_ADVSND_SetParam(ADVSND_PARAM param, MS_U32 u32value1, MS_U16 u16value2);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ADVANCED_SOUNDEFFECT
/// @brief \b Function \b Name: MApi_AUDIO_ADVSOUND_GetInfo()
/// @brief \b Function \b Description: This routine is used for Get ADV Sound info
/// @return MS_BOOL     \b : True / False
////////////////////////////////////////////////////////////////////////////////
 MS_U32  MApi_AUDIO_ADVSOUND_GetInfo( AdvSound_GET_Type Type);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeRemove
/// @brief \b Function \b Name: MApi_DBXTV_SetMode(DBXTV_TotSonMode TotSonMode, DBXTV_TotVolMode TotVolMode, DBXTV_TotSurMode TotSurMode)
/// @brief \b Function \b Description: This routine is used to set dbx TV mode
/// @param <IN>        \b param, TotSonMode, TotVolMode, TotSurMode
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b  NONE    :
/// @param <GLOBAL>    \b NONE
////////////////////////////////////////////////////////////////////////////////
void MApi_DBXTV_SetMode(EN_DBXTV_TotSonMode totsonmode, EN_DBXTV_TotVolMode totvolmode, EN_DBXTV_TotSurMode totsurmode,MS_U32 enable);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_BASIC_SOUNDEFFECT
/// @brief \b Function \b Name: MApi_AUDIO_SeInit()
/// @brief \b Function \b Description: This routine is the initialization for Audio sound effect module.
////////////////////////////////////////////////////////////////////////////////
  void    MApi_AUDIO_SeInit(void);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeMerged
/// @brief \b Function \b Name: MApi_AUDIO_SetAdvSndSys()
/// @brief \b Function \b Description: Check Advance sound mod and reload code
/// @param u8AdvSurrMod \b : Audio Advance Sound system mod
////////////////////////////////////////////////////////////////////////////////
#if defined(UFO_PUBLIC_HEADER_500) || defined(UFO_PUBLIC_HEADER_700)
 void    MApi_AUDIO_SetAdvSndSys(En_DVB_advsndType u8AdvSurrMod);
#else
 void    MApi_AUDIO_SetAdvSndSys(MS_U8 u8AdvSurrMod);
#endif

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeMerged
/// @brief \b Function \b Name: MApi_AUDIO_VDS()
/// @brief \b Function \b Description: Dolby Virtual Surround
/// @param u8enVDS     \b :   1: enable;  0: disable
////////////////////////////////////////////////////////////////////////////////
  void    MApi_AUDIO_SetVDS(MS_U8 u8enVDS);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeMerged
/// @brief \b Function \b Name: MApi_AUDIO_SetVSPK()
/// @brief \b Function \b Description: Dolby Virtual Speaker
/// @param u8enVSPK    \b :   1: enable;  0: disable
////////////////////////////////////////////////////////////////////////////////
  void    MApi_AUDIO_SetVSPK(MS_U8 u8enVSPK);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeMerged
/// @brief \b Function \b Name: MApi_AUDIO_SetSRS()
/// @brief \b Function \b Description: SRS TruSurr
/// @param u8enSRS     \b :   1: enable;  0: disable
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_SetSRS(MS_U8 u8enSRS);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeMerged
/// @brief \b Function \b Name: MApi_AUDIO_SetBBE()
/// @brief \b Function \b Description: BBE Surround
/// @param u8enBBE     \b :   1: enable;  0: disable
/// @param u8BBEMode   \b :
////////////////////////////////////////////////////////////////////////////////
  void    MApi_AUDIO_SetBBE(MS_U8 u8enBBE,MS_U8 u8BBEMode);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeMerged
/// @brief \b Function \b Name: MApi_AUDIO_VSPK_WMod()
/// @brief \b Function \b Description: Dolby Virtual Speaker Wide Mod
/// @param u8VSPK_WMode \b : 0: reference mod; 1: wide mod
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_VSPK_WMod(MS_U8 u8VSPK_WMode);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeMerged
/// @brief \b Function \b Name: MApi_AUDIO_VSPK_SMod()
/// @brief \b Function \b Description: Dolby Virtual Speaker Surround Mod
/// @param u8VSPK_SMode \b : 0: movie mod; 1: music mod
////////////////////////////////////////////////////////////////////////////////
  void    MApi_AUDIO_VSPK_SMod(MS_U8 u8VSPK_SMode);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeMerged
/// @brief \b Function \b Name: MApi_AUDIO_SRS_DC()
/// @brief \b Function \b Description: SRS Dialog Clarity
/// @param u8SRSenDC   \b :   1: enable;  0: disable
////////////////////////////////////////////////////////////////////////////////
  void    MApi_AUDIO_SRS_DC(MS_U8 u8SRSenDC);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeMerged
/// @brief \b Function \b Name: MApi_AUDIO_SRS_TruBass()
/// @brief \b Function \b Description:  SRS TruBass
/// @param u8SRSenTruBass \b :   1: enable;  0: disable
////////////////////////////////////////////////////////////////////////////////
  void    MApi_AUDIO_SRS_TruBass(MS_U8 u8SRSenTruBass);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeMerged
/// @brief \b Function \b Name: MApi_SOUND_SRS_SetTsxtPara()
/// @brief \b Function \b Description:  This function is used to set paramters of SRS-TSXT
/// @param u8mode      \b : parameter mode
/// @param u8value     \b : parameter value
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_SRS_SetTsxtPara(MS_U8 u8mode, MS_U8 u8value);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeMerged
/// @brief \b Function \b Name: MApi_AUDIO_SetSRSTSHD()
/// @brief \b Function \b Description: SRS TruSurround HD
/// @param u8enSRS     \b :   1: enable;  0: disable
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_SetSRSTSHD(MS_U8 u8enTSHD);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeRemove
/// @brief \b Function \b Name: Mapi_SOUND_TSHD_TruBass()
/// @brief \b Function \b Description:  TSHD TruBass
/// @param u8SRSenTruBass \b :   1: enable;  0: disable
////////////////////////////////////////////////////////////////////////////////
  void    Mapi_SOUND_TSHD_TruBass(MS_U8 u8TSHDenTruBass);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeMerged
/// @brief \b Function \b Name: MApi_AUDIO_TSHD_Definition()
/// @brief \b Function \b Description:  This function is used to enable/disable SRS-HD Definition
/// @param u8TSHDenDefinition \b :  1: enable, 0: disable
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_TSHD_Definition(MS_U8 u8TSHDenDefinition);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeRemove
/// @brief \b Function \b Name: Mapi_SOUND_TSHD_Definition()
/// @brief \b Function \b Description:  This function is used to enable/disable SRS-HD Definition
/// @param u8TSHDenDefinition \b :  1: enable, 0: disable
////////////////////////////////////////////////////////////////////////////////
  void    Mapi_SOUND_TSHD_Definition(MS_U8 u8TSHDenDefinition);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeMerged
/// @brief \b Function \b Name: MApi_SOUND_SRS_SetTshdPara()
/// @brief \b Function \b Description:  This function is used to set paramters of SRS-TSHD
/// @param u8mode      \b : parameter mode
/// @param u8value     \b : parameter value
////////////////////////////////////////////////////////////////////////////////
  void    MApi_AUDIO_SRS_SetTshdPara(MS_U8 u8mode, MS_U8 u8value);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_BASIC_SOUNDEFFECT
/// @brief \b Function \b Name: MApi_AUDIO_COPY_Parameter()
/// @brief \b Function \b Description: Copy parameter from APP level
/// @param u8enVSPK    \b :
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_COPY_Parameter(ADVSND_PARAM type, void* Parameter_ptr,MS_U32 size);

//=============================================================
// AUDIO_KTV RELATIONAL API FUNCTION
//=============================================================

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_BASIC_SOUNDEFFECT
/// @brief \b Function \b Name: MApi_AUDIO_SetKTVInfo()
/// @brief \b Function \b Description: Set KTV parameter
/// @param modelType   \b : KTV model type
/// @param infoType    \b : KTV infomation type
/// @param param1      \b :
/// @param param2      \b :
////////////////////////////////////////////////////////////////////////////////
 MS_BOOL MApi_AUDIO_SetKTVInfo (AUDIO_KTV_MODEL modelType, AUDIO_KTV_PARAMTYPE paramType, MS_U32 param1, MS_U32 param2);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_BASIC_SOUNDEFFECT
/// @brief \b Function \b Name: MApi_AUDIO_GetKTVInfo()
/// @brief \b Function \b Description: Get KTV parameter
/// @param modelType   \b : KTV model type
/// @param infoType    \b : KTV infomation type
/// @return MS_U32     \b : return KTV value
////////////////////////////////////////////////////////////////////////////////
 MS_U32  MApi_AUDIO_GetKTVInfo (AUDIO_KTV_MODEL modelType, AUDIO_KTV_INFOTYPE infoType);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_BASIC_SOUNDEFFECT
/// @brief \b Function \b Name: MApi_AUDIO_SetMixModeVolume()
/// @brief \b Function \b Description: This routine is used to set the absolute u8Volume of mix mode.
/// @param eSourceType \b : for audio source
/// @param VolType     \b : for audio vol type
/// @param u8Vol1      \b : MSB 7-bit register value of 10-bit u8Volume
///                         range from 0x00 to 0x7E , gain: +12db to   -114db (-1 db per step)
/// @param u8Vol2      \b : LSB 3-bit register value of 10-bit u8Volume
///                         range from 0x00 to 0x07 , gain:  -0db to -0.875db (-0.125 db per step)
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_SetMixModeVolume(AUDIO_SOURCE_INFO_TYPE eSourceType, AUDIO_MIX_VOL_TYPE VolType, MS_U8 u8Vol1, MS_U8 u8Vol2);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_BASIC_SOUNDEFFECT
/// @brief \b Function \b Name: MApi_AUDIO_SetMixModeMute()
/// @brief \b Function \b Description: This routine is used to set audio mix mode mute
/// @param eSourceType \b : for audio source
/// @param VolType     \b : for audio vol type
/// @param EnMute      \b : TRUE --Mute;
///                         FALSE--Unmute
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_SetMixModeMute(AUDIO_SOURCE_INFO_TYPE eSourceType, AUDIO_MIX_VOL_TYPE VolType, MS_BOOL EnMute);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeRemove
/// @brief \b Function \b Name: MApi_AUDIO_PlayMenuSound()
/// @brief \b Function \b Description: This routine is used to Play Menu Sound.
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_PlayMenuSound(void);

//=============================================================
// AUDIO_OTHERS FUNCTION
//=============================================================
//Cathy0511 void MApi_AUDIO_SetAudioSource(AUDIOSOURCE_TYPE eSource);
//Certification

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeRemove
/// @brief \b Function \b Name: MApi_AUDIO_SetCertMode()
/// @brief \b Function \b Description:
/// @param CertType    \b :
/// @param enable      \b :
///
////////////////////////////////////////////////////////////////////////////////
  MS_BOOL MApi_AUDIO_SetCertMode( Audio_CertMode CertType,Audio_CertEnable  enable);

//=============================================================
// AUDIO_MM RELATIONAL API FUNCTION
//=============================================================
// Each Decoder functions
// Set Info

//******************************************************************************
/// @ingroup Audio_Decoder
/// @brief \b Function \b Name: MApi_AUDIO_SetCommAudioInfo()
/// @brief \b Function \b Description: This routine is used for adjust Common Audio decoder parameters
/// @return MS_BOOL     \b : True / False
//******************************************************************************
 MS_BOOL MApi_AUDIO_SetCommAudioInfo( Audio_COMM_infoType infoType, MS_VIRT param1, MS_VIRT param2 );
//******************************************************************************
/// @ingroup Audio_ToBeRemove
/// @brief \b Function \b Name: MApi_AUDIO_SetMpegInfo()
/// @brief \b Function \b Description: This routine is used for adjust MPEG Audio decoder parameters
/// @return MS_BOOL     \b : True / False
//******************************************************************************
 MS_BOOL MApi_AUDIO_SetMpegInfo( Audio_MPEG_infoType infoType, MS_VIRT param1, MS_VIRT param2 );
 //******************************************************************************
/// @ingroup Audio_ToBeRemove
/// @brief \b Function \b Name: MApi_AUDIO_SetAC3Info()
/// @brief \b Function \b Description: This routine is used for adjust AC3 Audio decoder parameters
/// @return MS_BOOL     \b : True / False
//******************************************************************************
MS_BOOL MApi_AUDIO_SetAC3Info( Audio_AC3_infoType infoType, MS_VIRT param1, MS_VIRT param2 );

//******************************************************************************
/// @ingroup Audio_ToBeRemove
/// @brief \b Function \b Name: MApi_AUDIO_SetAC3PInfo()
/// @brief \b Function \b Description: This routine is used for adjust AC3+ Audio decoder parameters
/// @return MS_BOOL     \b : True / False
//******************************************************************************
 MS_BOOL MApi_AUDIO_SetAC3PInfo( Audio_AC3P_infoType infoType, MS_VIRT param1, MS_VIRT param2 );
 //******************************************************************************
/// @ingroup Audio_ToBeRemove
/// @brief \b Function \b Name: MApi_AUDIO_SetAACInfo()
/// @brief \b Function \b Description: This routine is used for adjust AAC Audio decoder parameters
/// @return MS_BOOL     \b : True / False
//******************************************************************************
MS_BOOL MApi_AUDIO_SetAACInfo( Audio_AAC_infoType infoType, MS_VIRT param1, MS_VIRT param2 );
//******************************************************************************
/// @ingroup Audio_ToBeRemove
/// @brief \b Function \b Name: MApi_AUDIO_SetWmaInfo()
/// @brief \b Function \b Description: This routine is used for adjust WMA Audio decoder parameters
/// @return MS_BOOL     \b : True / False
//******************************************************************************
MS_BOOL MApi_AUDIO_SetWmaInfo( Audio_WMA_infoType infoType, MS_VIRT param1, MS_VIRT param2 );
//******************************************************************************
/// @ingroup Audio_ToBeRemove
/// @brief \b Function \b Name: MApi_AUDIO_SetDTSCommonCtrl()
/// @brief \b Function \b Description: This routine is used for adjust DTS Audio decoder control
/// @return MS_BOOL     \b : True / False
//******************************************************************************
 MS_BOOL MApi_AUDIO_SetDTSCommonCtrl( Audio_DTS_infoType infoType, Audio_DTS_CtrlType ctrl_type );

//******************************************************************************
/// @ingroup Audio_Decoder
/// @brief \b Function \b Name: MApi_AUDIO_GetCommAudioInfo()
/// @brief \b Function \b Description: This routine will return the 64bit Common info according to info type
/// @return long long     \b : 64bit info
//******************************************************************************
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
 long long MApi_AUDIO_GetCommAudioInfo( Audio_COMM_infoType infoType );
#else
 MS_U64 MApi_AUDIO_GetCommAudioInfo( Audio_COMM_infoType infoType );
#endif

//******************************************************************************
/// @ingroup Audio_ToBeRemove
/// @brief \b Function \b Name: MApi_AUDIO_GetMpegInfo()
/// @brief \b Function \b Description: This routine will return the 32bit mpeg info according to info type
/// @return MS_U32     \b : 32bit info
//******************************************************************************
 MS_U32  MApi_AUDIO_GetMpegInfo( Audio_MPEG_infoType infoType );

//******************************************************************************
/// @ingroup Audio_ToBeRemove
/// @brief \b Function \b Name: MApi_AUDIO_GetAC3Info()
/// @brief \b Function \b Description: This routine will return the 32bit ac3 info according to info type
/// @return MS_U32     \b : 32bit info
//******************************************************************************
 MS_U32  MApi_AUDIO_GetAC3Info( Audio_AC3_infoType infoType );

//******************************************************************************
/// @ingroup Audio_ToBeRemove
/// @brief \b Function \b Name: MApi_AUDIO_GetAC3PInfo()
/// @brief \b Function \b Description: This routine will return the 32bit ac3+ info according to info type
/// @return MS_U32     \b : 32bit info
//******************************************************************************
 MS_U32  MApi_AUDIO_GetAC3PInfo( Audio_AC3P_infoType infoType );

//******************************************************************************
/// @ingroup Audio_ToBeRemove
/// @brief \b Function \b Name: MApi_AUDIO_GetAACInfo()
/// @brief \b Function \b Description: This routine will return the 32bit AAC info according to info type
/// @return MS_U32     \b : 32bit info
//******************************************************************************
 MS_U32  MApi_AUDIO_GetAACInfo( Audio_AAC_infoType infoType );

//******************************************************************************
/// @ingroup Audio_ToBeRemove
/// @brief \b Function \b Name: MApi_AUDIO_GetWmaInfo()
/// @brief \b Function \b Description: This routine will return the 32bit wma info according to info type
/// @return MS_U32     \b : 32bit info
//******************************************************************************
 MS_U32  MApi_AUDIO_GetWmaInfo( Audio_WMA_infoType infoType );

//******************************************************************************
/// @ingroup Audio_ToBeRemove
/// @brief \b Function \b Name: MApi_AUDIO_GetDTSInfo()
/// @brief \b Function \b Description: This routine will return the 32bit DTS info according to info type
/// @return MS_U32     \b : 32bit info
//******************************************************************************
  MS_U32  MApi_AUDIO_GetDTSInfo( Audio_DTS_infoType infoType );

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeRemove
/// @brief \b Function \b Name: MApi_AUDIO_XPCM_Param()
/// @brief \b Function \b Description: This routine configures necessary parameters when playing XPCM data
/// @param audioType      \b :    1--lpcm;
///                               2--MS-ADPCM;
///                               17--IMA-ADPCM
/// @param channels       \b :    0-- mono;
///                               1-- stereo
/// @param sampleRate     \b :    8, 11.025, 16, 22.05, 44.1 and 48 (KHz)
/// @param bitsPerSample  \b :    8--8 bits;
///                               16--16 bits
/// @param blockSize      \b :    block size
/// @param samplePerBlock \b :    samples per block
/// @return MS_U8         \b :    TRUE--parameters are correct;
///                               FALSE--parameters are incorrect
////////////////////////////////////////////////////////////////////////////////
#if defined(UFO_PUBLIC_HEADER_212) ||defined(UFO_PUBLIC_HEADER_500_3)
MS_U8   MApi_AUDIO_XPCM_Param(XPCM_TYPE audioType, MS_U8  channels, MS_U32 sampleRate,
                                MS_U8  bitsPerSample, MS_U16 blockSize, MS_U16 samplePerBlock);
#else
 MS_U8   MApi_AUDIO_XPCM_Param(XPCM_TYPE audioType, MS_U8  channels, MS_U16 sampleRate,
                                MS_U8  bitsPerSample, MS_U16 blockSize, MS_U16 samplePerBlock);
#endif

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeRemove
/// @brief \b Function \b Name: MApi_AUDIO_XPCM2_Param()
/// @brief \b Function \b Description: This routine configures necessary parameters when playing XPCM data
/// @param audioType      \b :    1--lpcm;
///                               2--MS-ADPCM;
///                               17--IMA-ADPCM
/// @param channels       \b :    0-- mono;
///                               1-- stereo
/// @param sampleRate     \b :    8, 11.025, 16, 22.05, 44.1 and 48 (KHz)
/// @param bitsPerSample  \b :    8--8 bits;
///                               16--16 bits
/// @param blockSize      \b :    block size
/// @param samplePerBlock \b :    samples per block
/// @return MS_U8         \b :    TRUE--parameters are correct;
///                               FALSE--parameters are incorrect
////////////////////////////////////////////////////////////////////////////////
#if defined(UFO_PUBLIC_HEADER_212) ||defined(UFO_PUBLIC_HEADER_500_3)
 void    MApi_AUDIO_XPCM2_Param(XPCM_TYPE audioType, MS_U8  channels, MS_U32 sampleRate,
                                MS_U8  bitsPerSample, MS_U16 blockSize, MS_U16 samplePerBlock);

#else
void    MApi_AUDIO_XPCM2_Param(XPCM_TYPE audioType, MS_U8  channels, MS_U16 sampleRate,
                                MS_U8  bitsPerSample, MS_U16 blockSize, MS_U16 samplePerBlock);
#endif

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeRemove
/// @brief \b Function \b Name: MApi_AUDIO_XPCM2_CheckIntStatus()
/// @brief \b Function \b Description: This routine is used to Read DSP internal sram value for int status by IDMA
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_U16  :     sram value for interrupt status
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
  MS_U16  MApi_AUDIO_XPCM2_CheckIntStatus(void);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeRemove
/// @brief \b Function \b Name: MApi_AUDIO_RA8_Param()
/// @brief \b Function \b Description: This routine configures necessary parameters when playing RA8 LBR data
/// @param mNumCodecs  \b :    1--stereo or mono
///                            2~5-- multi channels
/// @param mSamples    \b :    output samples--256, 512, 1024
/// @param mSampleRate \b :    8, 11.025, 16, 22.05, 44.1 (KHz)
/// @param Channels    \b :    1-- mono
///                                    2-- stereo
/// @param cplStart    \b :    0 ~ 51
/// @param Regions     \b :    0 ~ 31
/// @param cplQbits    \b :    0, 2, 3, 4, 5, 6
/// @param FrameSize   \b :    bitstream size of every NumCodes (bytes)
///                                    Range--0 ~ 65535
/// @return MS_U8      \b :    TRUE--parameters are correct;
///                            FALSE--parameters are incorrect
////////////////////////////////////////////////////////////////////////////////
 MS_U8   MApi_AUDIO_RA8_Param(MS_U16 mNumCodecs, MS_U16 mSamples, MS_U16 mSampleRate,
                                MS_U16* Channels, MS_U16* Regions, MS_U16* cplStart, MS_U16* cplQbits,
                                MS_U16* FrameSize);

  En_DVB_melodyPlayStatus MApi_AUDIO_PlayMelody(En_DVB_melodyPlayCmdType eMelodyCmd, MS_VIRT* u32FileAddr,MS_U32 u32FileSize, MS_U32 u32DspReqAddr);
////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeRemove
/// @brief \b Function \b Name: MApi_AUDIO_Init()
/// @brief \b Function \b Description: MM Music mode Initial Audio decoder
/// @param <IN>        \b enDecSystem: Audio decoder type
/// @param <OUT>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_Init(En_DVB_decSystemType enDecSystem);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeRemove
/// @brief \b Function \b Name: MApi_AUDIO_StartDecode()
/// @brief \b Function \b Description: start play MM file
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_StartDecode(void);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeRemove
/// @brief \b Function \b Name: MApi_AUDIO_StartBrowse()
/// @brief \b Function \b Description: start browse file
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_StartBrowse(void);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeRemove
/// @brief \b Function \b Name: MApi_AUDIO_StopDecode()
/// @brief \b Function \b Description: Stop decoder
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_StopDecode(void);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeRemove
/// @brief \b Function \b Name: MApi_AUDIO_PauseDecode()
/// @brief \b Function \b Description: pause decoder
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_PauseDecode(void);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeRemove
/// @brief \b Function \b Name: MApi_AUDIO_CheckPlayDone()
/// @brief \b Function \b Description: Check file play done. If file play done, will return TRUE
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b MS_U8   : TRUE or FALSE
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
 MS_U8   MApi_AUDIO_CheckPlayDone(void);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeRemove
/// @brief \b Function \b Name: MApi_AUDIO_GetResidualBufferSize()
/// @brief \b Function \b Description: Get ES buffer level.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b MS_U16 : ES buffer level
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
 MS_U16  MApi_AUDIO_GetResidualBufferSize(void);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeRemove
/// @brief \b Function \b Name: MApi_AUDIO_GetPCMBufferSize()
/// @brief \b Function \b Description: Get MP3 PCM buffer level.
/// @param <IN>        \b u16BitRate    : Bit Rate
/// @param <OUT>       \b MS_U16 : MP3 PCM buffer level
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
 MS_U16  MApi_AUDIO_GetPCMBufferSize(MS_U16 u16BitRate);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeRemove
/// @brief \b Function \b Name: MApi_AUDIO_GetPCMBufferSize2()
/// @brief \b Function \b Description: Get PCM buffer level.
/// @param <IN>        \b u16BitRate : Bit Rate;
/// @param <IN>        \b u16SampleRate : Sample Rate
/// @param <OUT>       \b MS_U16 : PCM buffer level
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
 MS_U16  MApi_AUDIO_GetPCMBufferSize2(MS_U16 u16BitRate,MS_U16 u16SampleRate);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeRemove
/// @brief \b Function \b Name: MApi_AUDIO_GetCurrentFrameNumber()
/// @brief \b Function \b Description: Get MP3 Current Frame Number
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b MS_U32  : Current Frame Number
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
 MS_U32  MApi_AUDIO_GetCurrentFrameNumber(void);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeMerged
/// @brief \b Function \b Name: MApi_AUDIO_GetSampleRate()
/// @brief \b Function \b Description: Get Sample Rate
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b MS_U16 : Sample Rate
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
 MS_U16  MApi_AUDIO_GetSampleRate(void);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeMerged
/// @brief \b Function \b Name: MApi_AUDIO_GetBitRate()
/// @brief \b Function \b Description: Get Bit Rate
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b MS_U32  : Bit Rate
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
 MS_U32  MApi_AUDIO_GetBitRate(void);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeRemove
/// @brief \b Function \b Name: MApi_AUDIO_GetLayer()
/// @brief \b Function \b Description: Get MP3 Layer
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b Audio_MP3_LAYER : MPEG Layer
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
 Audio_MP3_LAYER MApi_AUDIO_GetLayer(void);
////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_Decoder
/// @brief \b Function \b Name: MApi_AUDIO_CheckInputRequest()
/// @brief \b Function \b Description: If MCU receive DSP interrupt, this function will return TRUE and report requested ES data buffer address and size
/// @param <IN>        \b pU32WrtAddr    :
/// @param <IN>        \b pU32WrtBytes   :
/// @param <OUT>       \b MS_U8          : TRUE or FALSE
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
 MS_U8   MApi_AUDIO_CheckInputRequest(MS_U32 *pU32WrtAddr, MS_U32 *pU32WrtBytes);
#else
 MS_U8   MApi_AUDIO_CheckInputRequest(MS_PHY *pPHYWrtAddr, MS_U32 *pU32WrtBytes);
#endif

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_Decoder
/// @brief \b Function \b Name: MApi_AUDIO_SetInput()
/// @brief \b Function \b Description: This function will inform DSP that MCU already write data to ES buffer by interrupt.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_SetInput(void);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeRemove
/// @brief \b Function \b Name: MApi_AUDIO_SetSampleRateIndex()
/// @brief \b Function \b Description: Set AAC sample rate index.
/// @param <IN>        \b u16Index    :
/// @param <OUT>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_SetSampleRateIndex(MS_U16 u16Index);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeRemove
/// @brief \b Function \b Name: MApi_AUDIO_SetXPCMParam()
/// @brief \b Function \b Description: Set XPCM paramaters
/// @param <IN>        \b audioType :
/// @param <IN>        \b channels :
/// @param <IN>        \b sampleRate :
/// @param <IN>        \b bitsPerSample :
/// @param <IN>        \b samplePerBlock :
/// @param <OUT>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
#if defined(UFO_PUBLIC_HEADER_212)
void    MApi_AUDIO_SetXPCMParam(XPCM_TYPE audioType, MS_U8 channels, MS_U32 sampleRate, MS_U8 bitsPerSample, MS_U16 blockSize, MS_U16 samplePerBlock);
 #else
void    MApi_AUDIO_SetXPCMParam(XPCM_TYPE audioType, MS_U8 channels, MS_U16 sampleRate, MS_U8 bitsPerSample, MS_U16 blockSize, MS_U16 samplePerBlock);
#endif

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeRemove
/// @brief \b Function \b Name: MApi_AUDIO_FileEndNotification()
/// @brief \b Function \b Description: This function will inform DSP that file end
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_FileEndNotification(void);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeRemove
/// @brief \b Function \b Name: MApi_AUDIO_FileEndDataHandle()
/// @brief \b Function \b Description: Info DSP that the last ES data left size.
/// @param <IN>        \b u32DataLeft    :
/// @param <OUT>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_FileEndDataHandle(MS_U32 u32DataLeft);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeRemove
/// @brief \b Function \b Name: MApi_AUDIO_GetPlayTick()
/// @brief \b Function \b Description: Get Play Time stamp
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
 MS_U32  MApi_AUDIO_GetPlayTick(void);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeMerged
/// @brief \b Function \b Name: MApi_AUDIO_GetEsMEMCnt()
/// @brief \b Function \b Description: Get ES Bufffer size
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b MS_U16  : ES Bufffer size
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
 MS_U16  MApi_AUDIO_GetEsMEMCnt(void);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeMerged
/// @brief \b Function \b Name: MApi_AUDIO_SetASFParm()
/// @brief \b Function \b Description: Set WMA ASF Paramaters
/// @param <IN>        \b parm_type  :
/// @param <IN>        \b value   :
/// @param <OUT>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_SetASFParm(WMA_ASF_PARMTYPE parm_type, MS_U32 value);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeRemove
/// @brief \b Function \b Name: MApi_AUDIO_MM_SetInput()
/// @brief \b Function \b Description: MCU send interrupt to decoder1/2 in MM mode
/// @param <IN>        \b AUDDecoderNo  : DVB_Audio_Decoder1 / DVB_Audio_Decoder2
/// @param <IN>        \b u8IntTag   : Interrupt tag, increase by every time
/// @param <OUT>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_MM_SetInput (En_DVB_AudioDecoder AUDDecoderNo, MS_U8 u8IntTag);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeRemove
/// @brief \b Function \b Name: MApi_AUDIO_MM_CheckPlayDone()
/// @brief \b Function \b Description: MCU check audio play end in MM mode
/// @param <IN>        \b AUDDecoderNo  : DVB_Audio_Decoder1 / DVB_Audio_Decoder2
/// @param <OUT>       \b MS_U16    : Value of residual PCM level for file end stop
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
 MS_U32  MApi_AUDIO_MM_CheckPlayDone (En_DVB_AudioDecoder AUDDecoderNo);
////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeRemove
/// @brief \b Function \b Name: MApi_AUDIO_MM_CheckInputRequest()
/// @brief \b Function \b Description  : Decoder request input addr & size in MM mode
/// @param <IN>        \b AUDDecoderNo : DVB_Audio_Decoder1 / DVB_Audio_Decoder2
/// @param <IN>        \b pU32WrtAddr  : Request input address in ES1 buffer
/// @param <IN>        \b pU32WrtBytes : Request input size
/// @param <OUT>       \b MS_U8   : return TRUE if ok, else return FALSE
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
 MS_U8   MApi_AUDIO_MM_CheckInputRequest(En_DVB_AudioDecoder AUDDecoderNo, MS_U32 *pU32WrtAddr, MS_U32 *pU32WrtBytes);
#else
 MS_U8   MApi_AUDIO_MM_CheckInputRequest(En_DVB_AudioDecoder AUDDecoderNo, MS_PHY *pPHYWrtAddr, MS_U32 *pU32WrtBytes);
#endif

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeRemove
/// @brief \b Function \b Name: MApi_AUDIO_DmaReader_Init()
/// @brief \b Function \b Description:
/// @param <IN>        \b   eType   : sampleRate
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_DmaReader_Init(SAMPLE_RATE sampleRate);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeRemove
/// @brief \b Function \b Name: MApi_AUDIO_DmaReader_AllInput_Init()
/// @brief \b Function \b Description: Used DMA reader in all audio source application
///                    \b              (ex: Key sound) (CHIP_T8/T12) (48KHz only)
/// @param <IN>        \b   eType   : sampleRate
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_DmaReader_AllInput_Init(void);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeRemove
/// @brief \b Function \b Name: MApi_AUDIO_DmaReader_WritePCM()
/// @brief \b Function \b Description: This routine is used to write PCM data into DMA reader Buffer
/// @param <IN>        \b   eType   : buffer size (must be multiple of 8 Byte)
/// @param <OUT>       \b MS_BOOL    : TRUE or FALSE
/// @param <RET>       \b NONE    :
////////////////////////////////////////////////////////////////////////////////
 MS_BOOL MApi_AUDIO_DmaReader_WritePCM(void* buffer, MS_U32 bytes);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeRemove
/// @brief \b Function \b Name: MApi_AUDIO_DmaWriter_Init()
/// @brief \b Function \b Description:
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
////////////////////////////////////////////////////////////////////////////////
  void    MApi_AUDIO_DmaWriter_Init(void);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeRemove
/// @brief \b Function \b Name: MApi_AUDIO_USBPCM_Enable()  @@Need_Modify
/// @brief \b Function \b Description:  Enable/ Disable the path of USB PCM
/// @param <IN>        \b bEnable : on: TRUE, off: FALSE
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_USBPCM_Enable(MS_BOOL bEnable);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeRemove
/// @brief \b Function \b Name: MApi_AUDIO_USBPCM_SetFlag()  @@Need_Modify
/// @brief \b Function \b Description:  clear interrupt flag for USBPCM function
///                    \b               (Encoder path)
/// @param <IN>        \b bEnable :  set false to clean interrupt flag
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_USBPCM_SetFlag(MS_BOOL bEnable);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeRemove
/// @brief \b Function \b Name: MApi_AUDIO_USBPCM_GetFlag()  @@Need_Modify
/// @brief \b Function \b Description:  Get interrupt flag for USBPCM function
///                    \b               (Encoder path)
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b BOOL    :   interrupt flag
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
 MS_BOOL MApi_AUDIO_USBPCM_GetFlag(void);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeRemove
/// @brief \b Function \b Name: MApi_AUDIO_USBPCM_GetMemInfo()  @@Need_Modify
/// @brief \b Function \b Description:  Get memory address and size for USBPCM function
///                    \b               (Encoder path)
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b MS_U32  : address, size
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_USBPCM_GetMemInfo(AUDIO_UPLOAD_INFO *uploadInfo);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeRemove
/// @brief \b Function \b Name: MApi_AUDIO_PCMCapture_Init()
/// @brief \b Function \b Description:  Select source for pcm capture
/// @param <IN>        \b AUDIO_DEVICE_TYPE : select 1st or 2nd data capture
/// @param <IN>        \b AUDIO_CAPTURE_SOURCE_TYPE : Data Source
/// @param <OUT>       \b MS_BOOL    : return TRUE if success, else return FALSE
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
 MS_BOOL MApi_AUDIO_PCMCapture_Init(const AUDIO_DEVICE_TYPE eID, const AUDIO_CAPTURE_SOURCE_TYPE eSource);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeRemove
/// @brief \b Function \b Name: MApi_AUDIO_PCMCapture_Start()
/// @brief \b Function \b Description:  captrue pcm data to DDR
/// @param <IN>        \b AUDIO_DEVICE_TYPE : select 1st or 2nd data capture
/// @param <OUT>       \b MS_BOOL    : return TRUE if success, else return FALSE
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
 MS_BOOL MApi_AUDIO_PCMCapture_Start(const AUDIO_DEVICE_TYPE eID);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeRemove
/// @brief \b Function \b Name: MApi_AUDIO_PCMCapture_Stop()
/// @brief \b Function \b Description:  stop captrue pcm data from DDR
/// @param <IN>        \b AUDIO_DEVICE_TYPE : select 1st or 2nd data capture
/// @param <OUT>       \b MS_BOOL    : return TRUE if success, else return FALSE
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
 MS_BOOL MApi_AUDIO_PCMCapture_Stop(const AUDIO_DEVICE_TYPE eID);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeRemove
/// @brief \b Function \b Name: MApi_AUDIO_PCMCapture_Read()
/// @brief \b Function \b Description:  captrue pcm data from DDR to device
/// @param <IN>        \b AUDIO_DEVICE_TYPE : select 1st or 2nd data capture
/// @param <IN>        \b void* : destination buffer pointer
/// @param <IN>        \b MS_U32 : buffer size need transfered in byte
/// @param <OUT>       \b MS_BOOL    : return TRUE if success, else return FALSE
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
 MS_BOOL MApi_AUDIO_PCMCapture_Read(const AUDIO_DEVICE_TYPE eID,  void* buffer, const MS_U32 bytes);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeRemove
/// @brief \b Function \b Name: MApi_AUDIO_VoIP_Config()
/// @brief \b Function \b Description:  This routine is used for VoIP applications
/// @param <IN>        \b AUDIO_VoIP_CONFIG_TYPE : configType
/// @param <IN>        \b MS_U32 : param1
/// @param <IN>        \b MS_U32 : param2
/// @param <OUT>       \b MS_BOOL    : return TRUE if success, else return FALSE
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
 MS_BOOL MApi_AUDIO_VoIP_Config(AUDIO_VoIP_CONFIG_TYPE configType, MS_U32 param1, MS_U32 param2);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeRemove
/// @brief \b Function \b Name: MApi_AUDIO_ALSA_Check()
/// @brief \b Function \b Description:  Check if ALSA Interface is supported
/// @param <IN>        \b bEnable : on: TRUE, off: FALSE
/// @param <OUT>       \b MS_BOOL    : return TRUE if it's supported, else return FALSE
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
  MS_BOOL MApi_AUDIO_ALSA_Check(void);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeRemove
/// @brief \b Function \b Name: MApi_AUDIO_ALSA_Enable()
/// @brief \b Function \b Description:  Enable/ Disable the path of ALSA
/// @param <IN>        \b bEnable : on: TRUE, off: FALSE
/// @param <OUT>       \b MS_BOOL    : return TRUE if ok, else return FALSE
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
  MS_BOOL MApi_AUDIO_ALSA_Enable(MS_BOOL bEnable);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeRemove
/// @brief \b Function \b Name: MApi_AUDIO_UNI_CheckDecodeDone()
/// @brief \b Function \b Description  : Decoder report pcm out addr & size in Uniplayer mode
/// @param <IN>        \b pU32WrtAddr  : Request input address in ES1 buffer
/// @param <IN>        \b pU32WrtBytes : Request input size
/// @param <OUT>       \b MS_U8   : return TRUE if ok, else return FALSE
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
 MS_BOOL MApi_AUDIO_UNI_CheckDecodeDone(MS_U32 *pU32WrtAddr, MS_U32 *pU32WrtBytes);
#else
 MS_BOOL MApi_AUDIO_UNI_CheckDecodeDone(MS_PHY *pPHYWrtAddr, MS_U32 *pU32WrtBytes);
#endif

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeRemove
/// @brief \b Function \b Name: MApi_AUDIO_UNI_SetOutput()
/// @brief \b Function \b Description: MCU send interrupt to decoder1 in Uniplayer mode
/// @param <IN>        \b PCMOutCnt   : Get output PCM count by MCU
/// @param <OUT>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
 void    MApi_AUDIO_UNI_SetOutput (MS_U32 PCMOutCnt);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeRemove
/// @brief \b Function \b Name: MApi_AUDIO_UNI_Set_PCMInputWriteAddr()
/// @brief \b Function \b Description: MCU send DEC1-PCM3 wptr to decoder1 in Uniplayer mode
/// @param <IN>        \b PCMIn_Wptr   : PCM3 write pointer to DSP
/// @param <OUT>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
  void    MApi_AUDIO_UNI_Set_PCMInputWriteAddr (MS_U32 PCMIn_Wptr);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeRemove
/// @brief \b Function \b Name: MApi_AUDIO_UNI_Get_OutPCMLevel()
/// @brief \b Function \b Description: Decoder1 report DEC1-PCM3 buffer level in Uniplayer mode
/// @param <OUT>       \b MS_U32  : DEC1-PCM3 buffer level (byte unit)
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
  MS_U32  MApi_AUDIO_UNI_Get_OutPCMLevel (void);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeRemove
/// @brief \b Function \b Name: MApi_AUDIO_RingTask()
/// @brief \b Function \b Description :
/// @param <IN>        \b auRingTask  :
/// @param <GLOBAL>    \b NONE        :
////////////////////////////////////////////////////////////////////////////////
  void    MApi_AUDIO_RingTask(audioRingTask* auRingTask) ;

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeRemove
/// @brief \b Function \b Name: MApi_AUDIO_Ring_DataTransfer()
/// @brief \b Function \b Description :
/// @param <IN>        \b auRingTask  :
/// @param <GLOBAL>    \b NONE        :
////////////////////////////////////////////////////////////////////////////////
  void    MApi_AUDIO_Ring_DataTransfer(audioRingTask* auRingTask, audioRingTask* auRingTask_1);

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeRemove
/// @brief \b Function \b Name: MApi_AUDIO_MM2_initAesInfo()
/// @brief \b Function \b Description  : Initialize AES info
/// @param <IN>        \b dec_id  : Decoder ID
/// @param <OUT>       \b MS_BOOL : return TRUE if ok, else return FALSE
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
 MS_BOOL MApi_AUDIO_MM2_initAesInfo( AUDIO_DEC_ID dec_id );

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeRemove
/// @brief \b Function \b Name: MApi_AUDIO_MM2_checkAesInfo()
/// @brief \b Function \b Description  : Get AES info from audio decoder
/// @param <IN>        \b dec_id       : Decoder ID
/// @param <IN>        \b aes_info     : AES info structure pointer
/// @param <OUT>       \b MS_BOOL : return TRUE if ok, else return FALSE
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
 MS_BOOL MApi_AUDIO_MM2_checkAesInfo( AUDIO_DEC_ID  dec_id, AES_INFO *aes_info );

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeRemove
/// @brief \b Function \b Name: MApi_AUDIO_MM2_inputAesFinished()
/// @brief \b Function \b Description  : Inform DSP how much ES data is sent
/// @param <IN>        \b dec_id       : Decoder ID
/// @param <IN>        \b es_size  : Size of ES data
/// @param <IN>        \b ptsExist : Whether ES data contains PTS or not
/// @param <IN>        \b pts       : PTS
/// @param <OUT>       \b MS_BOOL  : return TRUE if ok, else return FALSE
/// @param <GLOBAL>    \b NONE     :
////////////////////////////////////////////////////////////////////////////////
 MS_BOOL MApi_AUDIO_MM2_inputAesFinished( AUDIO_DEC_ID dec_id, MS_U32 es_size, MS_BOOL ptsExist, MS_U64 pts );


//====== STAOS PIP START 2012/02/01 ========

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_Decoder
/// @brief \b Function \b Name: MApi_AUDIO_SetAudioParam2()
////////////////////////////////////////////////////////////////////////////////
 MS_BOOL MApi_AUDIO_SetAudioParam2( AUDIO_DEC_ID DecId, Audio_ParamType paramType, MS_VIRT Param );
////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_Decoder
/// @brief \b Function \b Name: MApi_AUDIO_GetAudioInfo2()
////////////////////////////////////////////////////////////////////////////////
 MS_BOOL MApi_AUDIO_GetAudioInfo2( AUDIO_DEC_ID DecId, Audio_InfoType infoType, void * pInfo );

//====== STAOS PIP END 2012/02/01 ========
////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_ToBeRemove
/// @brief \b Function \b Name: MApi_AUDIO_GetDDRInfo()
/// @brief \b Function \b Description:  Return Audio DDR info
/// @param <IN>        \b AUDIO_DEC_ID : select audio processor
/// @param <IN>        \b EN_AUDIO_DDRINFO : DDR info
/// @param <OUT>       \b MS_U32  : return DDR info
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_PHY MApi_AUDIO_GetDDRInfo(AUDIO_DEC_ID DecId, EN_AUDIO_DDRINFO DDRInfo);

MS_U32 MApi_AUDIO_GetCaps(AUDIO_DEC_ID DecId, MS_U32 *pCaps);

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_PCM_Open()
/// @brief \b Function \b Description : Open a PCM interface according to specific capabilities.
/// @param <IN>        \b void : Pointer to a block of memory with PCM information.
/// @param <OUT>       \b NONE :
/// @param <RET>       \b MS_S32 : On success a nonnegative value is returned, and this value is the chosen device-id. On error, a negative value is returned.
/// @param <GLOBAL>    \b NONE :
////////////////////////////////////////////////////////////////////////////////
 MS_S32   MApi_AUDIO_PCM_Open(void *pData);

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_PCM_Close()
/// @brief \b Function \b Description : Close an opened PCM interface.
/// @param <IN>        \b MS_S32 : The chosen device-id.
/// @param <OUT>       \b NONE :
/// @param <RET>       \b MS_S32 : On success zero is returned. On error, a negative value is returned.
/// @param <GLOBAL>    \b NONE :
////////////////////////////////////////////////////////////////////////////////
 MS_S32   MApi_AUDIO_PCM_Close(MS_S32 s32DeviceId);

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_PCM_Start()
/// @brief \b Function \b Description : Start an opened PCM interface.
/// @param <IN>        \b MS_S32 : The chosen device-id.
/// @param <OUT>       \b NONE :
/// @param <RET>       \b MS_S32 : On success zero is returned. On error, a negative value is returned.
/// @param <GLOBAL>    \b NONE :
////////////////////////////////////////////////////////////////////////////////
 MS_S32   MApi_AUDIO_PCM_Start(MS_S32 s32DeviceId);

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_PCM_Stop()
/// @brief \b Function \b Description : Stop a started PCM interface.
/// @param <IN>        \b MS_S32 : The chosen device-id.
/// @param <OUT>       \b NONE :
/// @param <RET>       \b MS_S32 : On success zero is returned. On error, a negative value is returned.
/// @param <GLOBAL>    \b NONE :
////////////////////////////////////////////////////////////////////////////////
 MS_S32   MApi_AUDIO_PCM_Stop(MS_S32 s32DeviceId);

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_PCM_Set()
/// @brief \b Function \b Description : Set configuration to an opened PCM interface.
/// @param <IN>        \b MS_S32 : The chosen device-id.
/// @param <IN>        \b MS_U32 : The command type.
/// @param <IN>        \b void : Pointer to a block of memory with specific configuration, and this memory is not allowed to be updated by this API.
/// @param <OUT>       \b NONE :
/// @param <RET>       \b MS_S32 : On success zero is returned. On error, a negative value is returned.
/// @param <GLOBAL>    \b NONE :
////////////////////////////////////////////////////////////////////////////////
 MS_S32   MApi_AUDIO_PCM_Set(MS_S32 s32DeviceId, MS_U32 u32Cmd, const void *pData);

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_PCM_Get()
/// @brief \b Function \b Description : Get configuration from an opened PCM interface.
/// @param <IN>        \b MS_S32 : The chosen device-id.
/// @param <IN>        \b MS_U32 : The command type.
/// @param <IN>        \b void : Pointer to a block of memory for receiving specific configuration.
/// @param <OUT>       \b NONE :
/// @param <RET>       \b MS_S32 : On success zero is returned. On error, a negative value is returned.
/// @param <GLOBAL>    \b NONE :
////////////////////////////////////////////////////////////////////////////////
 MS_S32   MApi_AUDIO_PCM_Get(MS_S32 s32DeviceId, MS_U32 u32Cmd, void *pData);

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_PCM_Read()
/// @brief \b Function \b Description : Read a specific amount of PCM data from a started PCM interface.
/// @param <IN>        \b MS_S32 : The chosen device-id.
/// @param <IN>        \b void : Pointer to a block of memory with PCM data.
/// @param <IN>        \b MS_U32 : The size of PCM data.
/// @param <OUT>       \b NONE :
/// @param <RET>       \b MS_U32 : The total number of PCM data successfully written is returned.
/// @param <GLOBAL>    \b NONE :
////////////////////////////////////////////////////////////////////////////////
 MS_U32   MApi_AUDIO_PCM_Read(MS_S32 s32DeviceId, void *pBuf, MS_U32 u32Size);

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_PCM_Write()
/// @brief \b Function \b Description : Write a specific amount of PCM data to a started PCM interface.
/// @param <IN>        \b MS_S32 : The chosen device-id.
/// @param <IN>        \b void : Pointer to a block of memory with PCM data, and this memory is not allowed to be updated by this API.
/// @param <IN>        \b MS_U32 : The size of PCM data.
/// @param <OUT>       \b NONE :
/// @param <RET>       \b MS_U32 : The total number of PCM data successfully written is returned.
/// @param <GLOBAL>    \b NONE :
////////////////////////////////////////////////////////////////////////////////
 MS_U32   MApi_AUDIO_PCM_Write(MS_S32 s32DeviceId, const void *pBuf, MS_U32 u32Size);

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_PCM_Flush()
/// @brief \b Function \b Description : Flush an opened PCM interface.
/// @param <IN>        \b MS_S32 : The chosen device-id.
/// @param <OUT>       \b NONE :
/// @param <RET>       \b MS_S32 : On success zero is returned. On error, a negative value is returned.
/// @param <GLOBAL>    \b NONE :
////////////////////////////////////////////////////////////////////////////////
 MS_S32   MApi_AUDIO_PCM_Flush(MS_S32 s32DeviceId);

#if defined(UFO_PUBLIC_HEADER_700)
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_ENCODER_Open()
/// @brief \b Function \b Description : Open an encoder interface according to specific device name.
/// @param <IN>		\b char : Pointer to a string with name.
/// @param <OUT>		\b NONE :
/// @param <RET>		\b MS_S32 : On success a nonnegative value is returned, and this value is the chosen device-id. On error, a negative value is returned.
/// @param <GLOBAL>	\b NONE :
///
/// Device Name :
///     "MP3_ENCODER1" : 1st MP3 Encoder
///     "MP3_ENCODER2" : 2nd MP3 Encoder
 ////////////////////////////////////////////////////////////////////////////////
 MS_S32 MApi_AUDIO_ENCODER_Open(const char *pName);

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_ENCODER_Close()
/// @brief \b Function \b Description : Close an opened encoder interface.
/// @param <IN>		\b MS_S32 : The chosen device-id.
/// @param <OUT>		\b NONE :
/// @param <RET>		\b MS_S32 : On success zero is returned. On error, a negative value is returned.
/// @param <GLOBAL>	\b NONE :
////////////////////////////////////////////////////////////////////////////////
 MS_S32 MApi_AUDIO_ENCODER_Close(MS_S32 s32DeviceId);

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_ENCODER_Start()
/// @brief \b Function \b Description : Start an opened encoder interface.
/// @param <IN>		\b MS_S32 : The chosen device-id.
/// @param <OUT>		\b NONE :
/// @param <RET>		\b MS_S32 : On success zero is returned. On error, a negative value is returned.
/// @param <GLOBAL>	\b NONE :
////////////////////////////////////////////////////////////////////////////////
 MS_S32 MApi_AUDIO_ENCODER_Start(MS_S32 s32DeviceId);

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_ENCODER_Stop()
/// @brief \b Function \b Description : Stop a started encoder interface.
/// @param <IN>		\b MS_S32 : The chosen device-id.
/// @param <OUT>		\b NONE :
/// @param <RET>		\b MS_S32 : On success zero is returned. On error, a negative value is returned.
/// @param <GLOBAL>	\b NONE :
////////////////////////////////////////////////////////////////////////////////
 MS_S32 MApi_AUDIO_ENCODER_Stop(MS_S32 s32DeviceId);

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_ENCODER_Ioctl()
/// @brief \b Function \b Description : Set/Get configuration to an opened encoder interface.
/// @param <IN>		\b MS_S32 : The chosen device-id.
/// @param <IN>		\b char : Pointer to a string with command name.
/// @param <IN>		\b void : Pointer to a block of memory with specific configuration to deliver, or to receive specific configuration.
/// @param <OUT>		\b NONE :
/// @param <RET>		\b MS_S32 : On success zero is returned. On error, a negative value is returned.
/// @param <GLOBAL>	\b NONE :
///
/// Command String Name :
///     "GetFrameSize" : Get the size of current encoded frame.
///     "GetAvailableSize" : Get the size of total available encoded frames.
///     "GetTimestampHigh32Bit" : Get high 32 bits of timestamp from current encoded frame.
///     "GetTimestampLow32Bit" : Get low 32 bits of timestamp from current encoded frame.
////////////////////////////////////////////////////////////////////////////////
 MS_S32 MApi_AUDIO_ENCODER_Ioctl(MS_S32 s32DeviceId, const char *pCmd, void *pData);

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_ENCODER_Read()
/// @brief \b Function \b Description : Read a specific amount of encoded data from a started encoder interface.
/// @param <IN>		\b MS_S32 : The chosen device-id.
/// @param <IN>		\b void : Pointer to a block of memory with encoded data.
/// @param <IN>		\b MS_U32 : The size of encoded data.
/// @param <OUT>		\b NONE :
/// @param <RET>		\b MS_U32 : The total number of encoded data successfully written is returned.
/// @param <GLOBAL>	\b NONE :
////////////////////////////////////////////////////////////////////////////////
 MS_U32 MApi_AUDIO_ENCODER_Read(MS_S32 s32DeviceId, void *pBuf, MS_U32 u32Size);

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_ENCODER_Flush()
/// @brief \b Function \b Description : Flush an opened encoder interface.
/// @param <IN>		\b MS_S32 : The chosen device-id.
/// @param <OUT>		\b NONE :
/// @param <RET>		\b MS_S32 : On success zero is returned. On error, a negative value is returned.
/// @param <GLOBAL>	\b NONE :
////////////////////////////////////////////////////////////////////////////////
 MS_S32 MApi_AUDIO_ENCODER_Flush(MS_S32 s32DeviceId);
#endif

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Audio_System
/// @brief \b Function \b Name: MApi_AUDIO_SYSTEM_Control()
/// @brief \b Function \b Description: audio system control
/// @param <IN>        \b pStrCmd : pointer to string command
/// @param <IN>        \b u32StrCmdLength : string command length
/// @param <RET>       \b MS_S32 : On success zero is returned. On error, a negative value is returned.
/// @param <GLOBAL>    \b NONE :
///
/// Usage of *pStrCmd :
///
/// "SetSpdifOutputType"
///     - Description : Set spdif output type.
///     - Parameter : AC3, AAC, DTS.
///     - i.e. "SetSpdifOutputType=AC3"
///
/// "SetHdmiArcOutputType"
///     - Description : Set hdmi arc output type.
///     - Parameter : AC3, AC3P, AAC, DTS.
///     - i.e. "SetHdmiArcOutputType=AC3"
///
/// "SetHdmiTxOutputType"
///     - Description : Set hdmi tx output type.
///     - Parameter : AC3, AC3P, AAC, DTS.
///     - i.e. "SetHdmiTxOutputType=AC3"
///
/// "SetSpdifOutputMode"
///     - Description : Set spdif output mode.
///     - Parameter : PCM, AUTO, BYPASS, TRANSCODE.
///     - i.e. "SetSpdifOutputMode=AUTO"
///
/// "SetHdmiArcOutputMode"
///     - Description : Set hdmi arc output mode.
///     - Parameter : PCM, AUTO, BYPASS, TRANSCODE.
///     - i.e. "SetHdmiArcOutputMode=AUTO"
///
/// "SetHdmiTxOutputMode"
///     - Description : Set hdmi tx output mode.
///     - Parameter : PCM, AUTO, BYPASS, TRANSCODE.
///     - i.e. "SetHdmiTxOutputMode=AUTO"
////////////////////////////////////////////////////////////////////////////////
MS_S32 MApi_AUDIO_SYSTEM_Control(MS_U8 *pStrCmd, MS_U32 u32StrCmdLength);

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_CHANNEL_Open()
/// @brief \b Function \b Description : Open a channel with automatically allocation.
/// @param <IN>        \b NONE :
/// @param <OUT>       \b NONE :
/// @param <RET>       \b MS_S32 : On success a nonnegative value is returned, and this value is the chosen channel-id. On error, a negative value is returned.
/// @param <GLOBAL>    \b NONE :
////////////////////////////////////////////////////////////////////////////////
MS_S32 MApi_AUDIO_CHANNEL_Open(void);

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_CHANNEL_Open_ByID()
/// @brief \b Function \b Description : Open a channel with a specified channel-id.
/// @param <IN>        \b MS_S32 : The specified channel-id, and this must be a nonnegative value.
/// @param <OUT>       \b NONE :
/// @param <RET>       \b MS_S32 : On success a nonnegative value is returned, and this value is the chosen channel-id. On error, a negative value is returned.
/// @param <GLOBAL>    \b NONE :
////////////////////////////////////////////////////////////////////////////////
MS_S32 MApi_AUDIO_CHANNEL_Open_ByID(MS_S32 s32ChannelId);

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_CHANNEL_Close()
/// @brief \b Function \b Description : Close an opened channel.
/// @param <IN>        \b MS_S32 : The chosen channel-id.
/// @param <OUT>       \b NONE :
/// @param <RET>       \b MS_S32 : On success zero is returned. On error, a negative value is returned.
/// @param <GLOBAL>    \b NONE :
////////////////////////////////////////////////////////////////////////////////
MS_S32 MApi_AUDIO_CHANNEL_Close(MS_S32 s32ChannelId);

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_CHANNEL_Ioctl()
/// @brief \b Function \b Description : Set/Get configuration to an opened channel.
/// @param <IN>        \b MS_S32 : The chosen channel-id.
/// @param <IN>        \b char : Pointer to a string with command name.
/// @param <IN>        \b MS_U32 : The length of command name.
/// @param <IN>        \b MS_U32 : Pointer to a memory with specific configuration to deliver, or to receive specific configuration.
/// @param <OUT>       \b NONE :
/// @param <RET>       \b MS_S32 : On success zero is returned. On error, a negative value is returned.
/// @param <GLOBAL>    \b NONE :
///
/// Command String Name :
////////////////////////////////////////////////////////////////////////////////
MS_S32 MApi_AUDIO_CHANNEL_Ioctl(MS_S32 s32ChannelId, const MS_U8 *pStrCmd, MS_U32 u32StrCmdLength, MS_U32 *pData);

#ifdef __cplusplus
}
#endif

#endif // _API_AUDIO_H_
