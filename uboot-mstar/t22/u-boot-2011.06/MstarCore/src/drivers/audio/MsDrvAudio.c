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

#include <common.h>
#include <command.h>
#include <config.h>
#include <MsTypes.h>
#include <bootlogo/MsPoolDB.h>
#include <MsOS.h>
#include <MsMmap.h>
#include <MsDebug.h>
#include <MsSystem.h>
#include <MsEnvironment.h>
#if defined (CONFIG_ONESN_ENABLE)
#include <MsHalAudio_onesn.h>
#endif
#if ((ENABLE_POWER_MUSIC == 1) || (CONFIG_BOOTVIDEO == 1))
#define DEFAULT_VOL  30
#define MUSIC_VOL_MAP_NUM 11

#include <apiAUDIO.h>
#include <drvAUDIO_if.h>
#include <drvAUDIO.h>
#include <drvGPIO.h>
#endif  // #if ((ENABLE_POWER_MUSIC == 1) || (CONFIG_BOOTVIDEO == 1))

#if (ENABLE_POWER_MUSIC == 1)
#include <audio/Audio_amplifier.h>
#include <MsApiPM.h>
#include <MsHalAudio.h>

#define DEFAULT_ABSVOL  0x0C00
#define debug(fmt,args...)
unsigned int  MELODY_VOLUME = DEFAULT_VOL;
#endif  // #if (ENABLE_POWER_MUSIC == 1)

#if (CONFIG_BOOTVIDEO == 1)
#include <apiDMX.h>
#include <apiHDMITx.h>
#include "MsDrvAudio.h"

// Dolby bulletin11
#define MSDRV_AUDIO_ENABLE_DOLBY_BULLETIN11         TRUE
// HDMITx pass through DDP
#define MSDRV_AUDIO_ENABLE_HDMITX_PASS_THROUGH_DDP  TRUE
// DTV decoder delay for av sync
#define MSDRV_AUDIO_ENABLE_DTV_AVSYNC_OFFSET_TUNING FALSE
// MPEG
#define MSDRV_AUDIO_MPEG_DTV_AVSYNC_OFFSET          0x3018
// AC3
#define MSDRV_AUDIO_AC3P_DTV_AVSYNC_OFFSET          0x3400
// AAC
#define MSDRV_AUDIO_AAC_DTV_AVSYNC_OFFSET           0x367E

#define MSDRV_AUDIO_MMAP_ID_MAD_DEC         "E_MMAP_ID_MAD_DEC"
#define MSDRV_AUDIO_MMAP_ID_MAD_SE          "E_MMAP_ID_MAD_SE"

static MS_BOOL _bIsMsDrvAudioInit = FALSE;
#endif  // #if (CONFIG_BOOTVIDEO == 1)

#if ((ENABLE_POWER_MUSIC == 1) || (CONFIG_BOOTVIDEO == 1))
/*
     change user power on music vol (range from 0~100) to Abs audio vol(range from 0x7F00 ~ 0x0D01)
*/
const MS_U16 music_vol_map[MUSIC_VOL_MAP_NUM] =             // UI:  Mute
//           Volume  Table     (High Byte : Integer part  ; Low Byte : Fraction part)
{
    //   1       2       3       4       5       6       7       8       9       10
    0x7F00, //  00
    0x4200, //  10
    0x3000, //  20
    0x2500, //  30
    0x1D00, //  40
    0x1800, //  50
    0x1402, //  60
    0x1106, //  70
    0x0F05, //  80
    0x0E03, //  90
    0x0D01, //  100
};

MS_U16 GetNonlinearAbsVol(MS_U8 u8vol)
{
    MS_U16 ret = 0;
    MS_U8  step = u8vol/10;

    if(u8vol>100)
    {
        u8vol = DEFAULT_VOL;
        step = u8vol/10;
        ret = music_vol_map[step] - (double)(u8vol%10)/10*(music_vol_map[step] - music_vol_map[step+1]);
    }
    else if(!(u8vol%10))
        ret =  music_vol_map[step];
    else
        ret = music_vol_map[step] - (double)(u8vol%10)/10*(music_vol_map[step] - music_vol_map[step+1]);

    return (MS_U16)ret;
}
#endif  // #if ((ENABLE_POWER_MUSIC == 1) || (CONFIG_BOOTVIDEO == 1))

#if ENABLE_POWER_MUSIC
void Mboot_Set_Melody_Play(void)
{
    MS_U8 u8Volume = DEFAULT_VOL;
    MS_U16 u16AbsVolume = DEFAULT_ABSVOL;

    #if (!CONFIG_SIM_Amplifier)
    //    mdrv_gpio_init();
    drvAudio_AMP_Init();
    #endif

    char * p_vol = NULL;
    if((p_vol =getenv ("music_vol"))!=NULL)
    {
        MELODY_VOLUME = (int)simple_strtol(p_vol, NULL, 10);
    }
    else
    {
        st_sys_misc_setting misc_setting;
        Read_MiscSetting_ToFlash(&misc_setting);
        MELODY_VOLUME = misc_setting.m_u32MusicVol;
    }
    if(MELODY_VOLUME == 0)
    {
        u8Volume = 0;
    }
    else
    {
        u8Volume = MELODY_VOLUME;
    }

    u16AbsVolume = GetNonlinearAbsVol(u8Volume);
    UBOOT_DEBUG("music u16AbsVolume = %u \n", u16AbsVolume); //need this for Delay
  
  // Set volume for all audio output port
    MsHalAudio_SetOutputVolume(u16AbsVolume);

  // Set audio internal->output port data path   
    MsHalAudio_SetInternalPath();
    
  // unmute all audio output port
    MsHalAudio_SetOutputMute(0);

  //check env music_mute
    char * p_mute = NULL;
    if ((p_mute = getenv ("music_mute")) != NULL)
    {
        int mute_index = (int)simple_strtol(p_mute, NULL, 10);
        UBOOT_DEBUG("music_mute = 0x%x\n", mute_index);
        //low 8 bit is 0 mute spdif
        if((mute_index & 0xFF) == 0)
        {
            UBOOT_DEBUG("Mute spdif !!\n");
            MApi_AUDIO_SetMute(AUDIO_T3_PATH_SPDIF, 1);
        }
        //high 8 bit is 0 mute hdmi
        if(((mute_index >> 8) & 0xFF) == 0)
        {
            UBOOT_DEBUG("Mute hdmi !!\n");
            MApi_AUDIO_SetMute(AUDIO_T3_PATH_HDMI, 1);
        }
    }

  // unmute all audio output port
    MsHalAudio_MelodyPlayStart();

    // Do Audio_Amplifier_ON after melody set play command
    if(FALSE == pm_check_back_ground_active())
    {
#if defined (CONFIG_ONESN_ENABLE)
        OneSN_Adj_Volume_On();
        OneSN_Audio_Amplifier_ON();
#else
        Adj_Volume_On();
        Audio_Amplifier_ON();
#endif
    }
}

void MsDrv_PlayMelody(void)
{
    UBOOT_DEBUG("music debug %s %s %u\n",__FILE__,__FUNCTION__,__LINE__);

    Mboot_Set_Melody_Play();

    printf("[AT][MB][Play Melody][%lu]\n", MsSystemGetBootTime());
}

int MsDrv_AudioPreInit(void)
{
    MS_PHYADDR R2Addr =0 , SeAddr =0 , DecAddr =0 ;

    UBOOT_DEBUG("music debug %s %s %u\n",__FILE__,__FUNCTION__,__LINE__);
    Audio_Amplifier_OFF();  // Do Audio_Amplifier_OFF before audio init

    MApi_AUDIO_WritePreInitTable();

    if(get_map_addr_from_env(E_MAD_R2,NO_DEFAULT_MMAP_VALUE, &R2Addr)!=0)
    {
        UBOOT_ERROR("get E_MMAP_ID_MAD_R2 mmap fail\n");
        return -1;
    }
    if(get_map_addr_from_env(E_MAD_SE,NO_DEFAULT_MMAP_VALUE, &SeAddr)!=0)
    {
        UBOOT_ERROR("get E_MMAP_ID_MAD_SE mmap fail\n");
        return -1;
    }
    if(get_map_addr_from_env(E_MAD_DEC,NO_DEFAULT_MMAP_VALUE, &DecAddr)!=0)
    {
        UBOOT_ERROR("get E_MMAP_ID_MAD_DEC mmap fail\n");
        return -1;
    }
    if(R2Addr==0xFFFF || SeAddr==0xFFFF ||DecAddr==0xFFFF)
    {
        UBOOT_ERROR("Get Mmap for Audio Fail Skip Power Music !!! \n");
        return -1;
    }

    MDrv_AUDIO_SetDspBaseAddr(DSP_DEC, 0, DecAddr);
    MDrv_AUDIO_SetDspBaseAddr(DSP_SE, 0,  SeAddr);
    MDrv_AUDIO_SetDspBaseAddr(DSP_ADV, 0, R2Addr);

    MApi_AUDIO_Initialize();
    
    return 1;
}

MS_PHYADDR MsDrv_GetEsStartAddr(void)
{
    return MsHalAudio_GetEsStartAddr();
}
#endif

#if (CONFIG_BOOTVIDEO == 1)

static MS_BOOL _MsDrv_AudioGetMmap(char *pszMmapId, MS_U32 *pu32MmapAddr, MS_U32 *pu32MmapLen)
{
    U32 u32MmapAddr = 0;
    U32 u32MmapLen = 0;

    if(pu32MmapAddr) *pu32MmapAddr = 0;
    if(pu32MmapLen) *pu32MmapLen = 0;

    if(pu32MmapAddr)
    {
        if(get_addr_from_mmap(pszMmapId, &u32MmapAddr) != 0)
        {
            UBOOT_ERROR("failed to get mmap addr of \"%s\" \n", pszMmapId);
            return FALSE;
        }

        *pu32MmapAddr = u32MmapAddr;
    }

    if(pu32MmapLen)
    {
        if(get_length_from_mmap(pszMmapId, &u32MmapLen) != 0)
        {
            UBOOT_ERROR("failed to get mmap length of \"%s\" \n", pszMmapId);
            return FALSE;
        }

        *pu32MmapLen = u32MmapLen;
    }

    return TRUE;
}

static MS_BOOL _MsDrv_AudioInit(void)
{
    if(_bIsMsDrvAudioInit == TRUE)
    {
        UBOOT_INFO("%s: Module has been initialized!\n", __FUNCTION__);
        return TRUE;
    }

    UBOOT_INFO("%s(%d): Single decode case.\n", __FUNCTION__, __LINE__);

    AUDIO_INIT_INFO SystemInfo;
    AUDIO_OUT_INFO OutputInfo;
    AUDIO_PATH_INFO PathInfo;
    MS_PHYADDR R2Addr = 0 , SeAddr = 0 , DecAddr = 0 ;
    MS_BOOL bR2Enable = TRUE;
    MS_BOOL bSeEnable = TRUE;
    MS_BOOL bDecEnable = TRUE;

    MApi_AUDIO_WritePreInitTable();

    SystemInfo.tv_system = TV_PAL;
    SystemInfo.au_system_sel = AUDIO_SYSTEM_A2;
    SystemInfo.dtv_system = 0;
    MApi_AUDIO_SetSystemInfo(&SystemInfo);

    //======================================
    // Set output info
    //======================================
    OutputInfo.SpeakerOut = AUDIO_OUTPUT_MAIN_SPEAKER;
    OutputInfo.HpOut = AUDIO_OUTPUT_HP;
    OutputInfo.ScartOut = AUDIO_OUTPUT_SCART1;
    //OutputInfo.MonitorOut = AUDIO_OUTPUT_LINEOUT;
    OutputInfo.MonitorOut = AUDIO_NULL_OUTPUT;
    MApi_AUDIO_SetOutputInfo(&OutputInfo);

    //======================================
    // Set path info
    //======================================
    PathInfo.SpeakerOut = AUDIO_PATH_MAIN_SPEAKER;
    PathInfo.HpOut = AUDIO_PATH_HP;
    //PathInfo.MonitorOut = AUDIO_PATH_LINEOUT;
    PathInfo.MonitorOut = AUDIO_NULL_OUTPUT;
    PathInfo.SpdifOut = AUDIO_PATH_SPDIF;
    PathInfo.ScartOut = AUDIO_PATH_SCART1;
    MApi_AUDIO_SetPathInfo(&PathInfo);

    //======================================
    // Set audio buffer
    //======================================
/*    u32MmapAddr = u32MmapLen = 0;
    if(_MsDrv_AudioGetMmap(MSDRV_AUDIO_MMAP_ID_MAD_DEC, &u32MmapAddr, &u32MmapLen) == FALSE)
    {
        UBOOT_ERROR("failed to _MsDrv_AudioGetMmap(MSDRV_AUDIO_MMAP_ID_MAD_DEC)!\n");
        goto AUDIO_INIT_FAILED;
    }
    MDrv_AUDIO_SetDspBaseAddr(DSP_DEC, 0, u32MmapAddr);
    UBOOT_INFO("ADEC DEC add = 0x%lx   Size = 0x%lx\n", u32MmapAddr, u32MmapLen);

    u32MmapAddr = u32MmapLen = 0;
    if(_MsDrv_AudioGetMmap(MSDRV_AUDIO_MMAP_ID_MAD_SE, &u32MmapAddr, &u32MmapLen))
    {
        if(u32MmapLen > 0)
            MDrv_AUDIO_SetDspBaseAddr(DSP_SE,  0, u32MmapAddr);
    }
    UBOOT_INFO("ADEC SE add = 0x%lx   Size = 0x%lx\n", u32MmapAddr, u32MmapLen);
*/

    if((get_map_addr_from_env(E_MAD_R2,NO_DEFAULT_MMAP_VALUE, &R2Addr) == 0) && (R2Addr != 0xFFFF))
    {
        MDrv_AUDIO_SetDspBaseAddr(DSP_ADV, 0, R2Addr);
        bR2Enable = TRUE;
    }
    else
    {
        bR2Enable = FALSE;
        UBOOT_ERROR("get E_MMAP_ID_MAD_R2 mmap fail\n");
    }

    if((get_map_addr_from_env(E_MAD_SE,NO_DEFAULT_MMAP_VALUE, &SeAddr) == 0) && (SeAddr != 0xFFFF))
    {
        bSeEnable = TRUE;
        MDrv_AUDIO_SetDspBaseAddr(DSP_SE, 0,  SeAddr);
    }
    else
    {
        bSeEnable = FALSE;
        UBOOT_ERROR("get E_MMAP_ID_MAD_SE mmap fail\n");
    }

    if((get_map_addr_from_env(E_MAD_DEC,NO_DEFAULT_MMAP_VALUE, &DecAddr) == 0) && (DecAddr != 0xFFFF))
    {
        bDecEnable = TRUE;
        MDrv_AUDIO_SetDspBaseAddr(DSP_DEC, 0, DecAddr);
    }
    else
    {
        bDecEnable = FALSE;
        UBOOT_ERROR("get E_MMAP_ID_MAD_DEC mmap fail\n");
    }

    if(bR2Enable == FALSE && bDecEnable == FALSE && bSeEnable == FALSE)
    {
        UBOOT_ERROR("Get Mmap for Audio Fail Skip Power Music !!! \n");
        return -1;
    }

    MApi_AUDIO_Initialize();

    MApi_AUDIO_SetSourceInfo(E_AUDIO_INFO_GAME_IN);

    // Set Dolby DRC mode
    // DTV: RF mode
    // MM: Line mode
    MApi_AUDIO_SetAC3PInfo(Audio_AC3P_infoType_DrcMode, RF_MODE, 0);

    MApi_AUDIO_InputSwitch(AUDIO_DSP1_DVB_INPUT, E_AUDIO_GROUP_MAIN);
    // Set HDMI Tx DD+ bypass
    // Enable:  DDP will pass through HDMI path
    // Disable: DDP will be transcoded to DD, and then pass through HDMI path
    MApi_AUDIO_SetAC3PInfo(Audio_AC3P_infoType_hdmiTxBypass_enable, MSDRV_AUDIO_ENABLE_HDMITX_PASS_THROUGH_DDP, 0);

    // Set Dolby Bulletin11, attenuate by 11dB in HDMI/SPDIF output at decoded PCM level. Default is enabled.
    MApi_AUDIO_SetAC3PInfo(Audio_AC3P_infoType_enableDolbyBulletin11, MSDRV_AUDIO_ENABLE_DOLBY_BULLETIN11, 0);

    // DTV av sync decoder delay setting
    if(MSDRV_AUDIO_ENABLE_DTV_AVSYNC_OFFSET_TUNING)
    {
        MApi_AUDIO_SetMpegInfo(Audio_MPEG_DTV_AVSYNC_OFFSET, MSDRV_AUDIO_MPEG_DTV_AVSYNC_OFFSET, 0);    //default:(0x3018) 180ms
        MApi_AUDIO_SetAACInfo(Audio_AAC_DTV_AVSYNC_OFFSET, MSDRV_AUDIO_AAC_DTV_AVSYNC_OFFSET, 0);       //default:(0x367E) 180ms
        MApi_AUDIO_SetAC3PInfo(Audio_AC3P_DTV_AVSYNC_OFFSET, MSDRV_AUDIO_AC3P_DTV_AVSYNC_OFFSET, 0);    //default:(0x3400) 180ms
    }
    else    // use default value
    {
        MApi_AUDIO_SetMpegInfo(Audio_MPEG_DTV_AVSYNC_DISABLE_SETTING, 0, 0);    //default:(0x3018) 180ms
        MApi_AUDIO_SetAACInfo(Audio_AAC_DTV_AVSYNC_DISABLE_SETTING, 0, 0);      //default:(0x367E) 180ms
        MApi_AUDIO_SetAC3PInfo(Audio_AC3P_DTV_AVSYNC_DISABLE_SETTING, 0, 0);    //default:(0x3400) 180ms
    }

    MsDrv_AudioMute();

    _bIsMsDrvAudioInit = TRUE;

    return TRUE;

AUDIO_INIT_FAILED:

    return FALSE;
}

static En_DVB_decSystemType _MsDrv_AudioGetAdecCodecType(MsDrv_AUDIO_CodecType_e eCodecType)
{
    En_DVB_decSystemType eAdecCodecType = MSAPI_AUD_DVB_NONE;
    switch(eCodecType)
    {
        case E_MSDRV_AUDIO_CODEC_TYPE_MPEG:
            eAdecCodecType = MSAPI_AUD_DVB_MPEG;
            break;

        case E_MSDRV_AUDIO_CODEC_TYPE_AC3:
            eAdecCodecType = MSAPI_AUD_DVB_AC3;
            break;

        case E_MSDRV_AUDIO_CODEC_TYPE_AC3P:
            eAdecCodecType = MSAPI_AUD_DVB_AC3P;
            break;

        case E_MSDRV_AUDIO_CODEC_TYPE_AAC:
            eAdecCodecType = MSAPI_AUD_DVB_AAC;
            break;

        case E_MSDRV_AUDIO_CODEC_TYPE_MP3:
            eAdecCodecType = MSAPI_AUD_DVB_MP3;
            break;

        case E_MSDRV_AUDIO_CODEC_TYPE_XPCM:
            eAdecCodecType = MSAPI_AUD_DVB_XPCM;
            break;

        case E_MSDRV_AUDIO_CODEC_TYPE_DTS:
            eAdecCodecType = MSAPI_AUD_DVB_DTS;
            break;

        default:
            UBOOT_ERROR("Unknown codec type = %d\n", eCodecType);
            break;
    }

    return eAdecCodecType;
}

static int _MsDrv_AudioSetVolume(MS_U8 u8Volume)
{
    MS_U8 u8VolHi,u8VolLo;
    MS_BOOL bMute = FALSE;

    if (u8Volume == 0)
        bMute = TRUE;

    u8VolHi = (MS_U8)(GetNonlinearAbsVol(u8Volume)>>8);
    u8VolLo = (MS_U8)(GetNonlinearAbsVol(u8Volume)&0xFF);

    UBOOT_INFO("Audio u8VolHi=%u,u8VolLo=%u",u8VolHi,u8VolLo);

    MApi_AUDIO_SetAbsoluteVolume(AUDIO_PATH_MAIN_SPEAKER, u8VolHi, u8VolLo);
    MApi_AUDIO_SetAbsoluteVolume(AUDIO_PATH_HDMI, u8VolHi, u8VolLo);
    MApi_AUDIO_SetAbsoluteVolume(AUDIO_PATH_SPDIF, u8VolHi, u8VolLo);
    MApi_AUDIO_SetAbsoluteVolume(AUDIO_PATH_I2S, u8VolHi, u8VolLo);

    MApi_AUDIO_SetMute(AUDIO_PATH_MAIN_SPEAKER, bMute);
    MApi_AUDIO_SetMute(AUDIO_PATH_HDMI, bMute);
    MApi_AUDIO_SetMute(AUDIO_PATH_SPDIF, bMute);
    MApi_AUDIO_SetMute(AUDIO_PATH_I2S, bMute);
    MApi_AUDIO_SPDIF_SetMute(bMute);
    MApi_AUDIO_HDMI_Tx_SetMute(bMute);

    return 0;
}

int MsDrv_AudioStart(MsDrv_AUDIO_StreamType_e eStreamType, MsDrv_AUDIO_CodecType_e eCodecType)
{
    if(_MsDrv_AudioInit() == FALSE)
    {
        UBOOT_ERROR("failed to _MsDrv_AudioInit()!\n");
        goto AUDIO_START_FAILED;
    }
    En_DVB_decSystemType AudioDecoder = _MsDrv_AudioGetAdecCodecType(eCodecType);
    UBOOT_DEBUG("AudioDecoder = %d\n", AudioDecoder);

    // Load audio decoder
    MApi_AUDIO_SetSystem(AudioDecoder);

    switch(eStreamType)
    {
        case E_MSDRV_AUDIO_STREAM_LIVE:
            MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_AVSYNC);
            MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_PLAY);
            break;

        case E_MSDRV_AUDIO_STREAM_TSFILE:
            MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_FREE_RUN);
            MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_PLAYFILETSP);
            break;

        default:
            UBOOT_ERROR("Unknown stream type = %d\n", eStreamType);
            break;
    }

    // Enable PUSI detect
    MApi_AUDIO_SetCommAudioInfo(Audio_Comm_infoType_PUSI_disable_Info, 0, 0);

    MsOS_DelayTask(30); // for wait audio decoder ready for plabyack. After the AV fifo reset disabled, the AV fifo may be full due to audio decoder is not ready.

    MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE_AUDIO, FALSE);

    UBOOT_INFO("Audio stream type = %d and codec type = %d start succeeded!\n", eStreamType, eCodecType);

    return 0;

AUDIO_START_FAILED:


    return -1;
}

int MsDrv_AudioStop(void)
{
    MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE_AUDIO, TRUE);
    MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_STOP);

    return 0;
}

void MsDrv_AudioShowDecInfo(void)
{
    MS_U32 u32EsBuf;
    MS_U32 u32PcmBuf;
    MS_U32 u32DecFrmCnt;
    MS_U32 u32DecErrCnt;

    u32EsBuf = (MS_U32)MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_ADEC1_esBuf_currLevel);
    u32PcmBuf = (MS_U32)MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_ADEC1_pcmBuf_currLevel);
    u32DecFrmCnt = (MS_U32)MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_ADEC1_okFrmCnt);
    u32DecErrCnt = (MS_U32)MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_ADEC1_errFrmCnt);

    printf("[ADEC] ES:%04lx, PCM:%04lx, Dec:%ld, Err:%ld\n", u32EsBuf, u32PcmBuf, u32DecFrmCnt, u32DecErrCnt);
}

void MsDrv_AudioUnMute(MS_U8 u8Volume)
{
    MApi_HDMITx_DisableRegWrite(FALSE);

    // Volume control
    _MsDrv_AudioSetVolume(u8Volume);

    // HDMITx audio configuration
    if(u8Volume == 0)
        MApi_HDMITx_SetAudioOnOff(FALSE);
    else
        MApi_HDMITx_SetAudioOnOff(TRUE);
    MApi_HDMITx_SetAudioConfiguration(HDMITX_AUDIO_48K, HDMITX_AUDIO_CH_2, HDMITX_AUDIO_PCM);

    // GPIO demute
#ifdef AUDIO_MUTE_GPIO
    AUDIO_MUTE_SET_GPIO_UP(AUDIO_MUTE_GPIO);
#endif
}

void MsDrv_AudioMute(void)
{
    MApi_HDMITx_DisableRegWrite(FALSE);
    // Volume control
    _MsDrv_AudioSetVolume(0);
    MApi_HDMITx_SetAudioOnOff(FALSE);
}

int MsDrv_AudioGetCurPts(void)
{
    MS_U32 u32Afpts = 0xFFFFFFFF;

    u32Afpts = (MApi_AUDIO_GetCommAudioInfo((Audio_COMM_infoType)Audio_Comm_infoType_ADEC1_33bit_PTS) & 0xFFFFFFFF);

    return u32Afpts;
}
#endif  // #if (CONFIG_BOOTVIDEO == 1)
