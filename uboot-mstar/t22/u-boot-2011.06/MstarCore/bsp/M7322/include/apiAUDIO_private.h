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
//
//  File name: apiDMX_private.h
//  Description: Demux  (DMX) API private header file
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _AUDIO_PRIV_H_
#define _AUDIO_PRIV_H_

#include "apiAUDIO.h"
#include "MsTypes.h"

#ifdef __cplusplus
extern "C"
{
#endif

//============================================================
// AUDIO_SYSTEM RELATIONAL API FUNCTION
//============================================================
typedef void                (*IOCTL_AUDIO_INITIALIZE)(void);
typedef void                (*IOCTL_AUDIO_SET_POWER_ON)(MS_BOOL);
typedef void                (*IOCTL_AUDIO_SETDSPBASEADDR)(MS_U8, MS_PHY, MS_PHY );
typedef MS_PHY              (*IOCTL_AUDIO_GETDSPMADBASEADDR)(MS_U8);
typedef void                (*IOCTL_AUDIO_WRITEDECMAILBOX)(MS_U8, MS_U16);
typedef void                (*IOCTL_AUDIO_TRIGGERSIFPLL)(void);
typedef void                (*IOCTL_AUDIO_MONITOR)(void);
typedef void                (*IOCTL_AUDIO_SETSYSTEMINFO)(AUDIO_INIT_INFO*);
typedef void                (*IOCTL_AUDIO_ASF_DATA_TO_DDR)(MS_BOOL);
typedef void                (*IOCTL_AUDIO_R2_DEBUG_MSG)(MS_BOOL);
typedef void                (*IOCTL_AUDIO_R2_DDR_LETENCY_MSG)(MS_BOOL);
typedef void                (*IOCTL_AUDIO_SETOUTCONNECTIVITY)(void);
typedef void                (*IOCTL_AUDIO_SETPATHINFO)(AUDIO_PATH_INFO*);
typedef void                (*IOCTL_AUDIO_GETPATHINFO)(AUDIO_PATH_INFO*);
typedef void                (*IOCTL_AUDIO_SETOUTPUTINFO)(AUDIO_OUT_INFO *);
typedef void                (*IOCTL_AUDIO_GETOUTPUTINFO)(AUDIO_OUT_INFO *);
typedef void                (*IOCTL_AUDIO_SETSOURCEINFO)(AUDIO_SOURCE_INFO_TYPE);
typedef void                (*IOCTL_AUDIO_INPUTSWITCH)(AUDIO_INPUT_TYPE, AUDIO_SWITCH_GROUP);
typedef void                (*IOCTL_AUDIO_SETINTERNALPATH)(AUDIO_INTERNAL_PATH_TYPE, AUDIO_OUTPUT_TYPE);
typedef void                (*IOCTL_AUDIO_SETINPUTSCALE)(int);
typedef void                (*IOCTL_AUDIO_SETSCARTSCALE)(int Step);
typedef MS_BOOL             (*IOCTL_AUDIO_AUDYSSEY_VXT_SET_THRESH_LEVEL_TABLE)(int*);
typedef void                (*IOCTL_AUDIO_SPDIF_SETCHANNELSTATUS)(SPDIF_CS_MODE_TYPE, SPDIF_CS_MODE_VALUE);
typedef MS_U16              (*IOCTL_AUDIO_SPDIF_GETCHANNELSTATUS)(SPDIF_CS_MODE_TYPE);

typedef MS_BOOL             (*IOCTL_AUDIO_SETDATACAPTURESOURCE)(AUDIO_DEVICE_TYPE, AUDIO_CAPTURE_SOURCE_TYPE);
typedef void                (*IOCTL_AUDIO_SETOUTPUTSOURCEINFO)(OUTPUT_SOURCE_INFO *);
typedef void                (*IOCTL_AUDIO_GETOUTPUTSOURCEINFO)(OUTPUT_SOURCE_INFO *);
typedef MS_U16              (*IOCTL_AUDIO_READMAILBOX)(MS_BOOL, MS_U8);
typedef void                (*IOCTL_AUDIO_WRITEMAILBOX)(MS_BOOL, MS_U8, MS_U16);
typedef void                (*IOCTL_AUDIO_FWTRIGGERDSP)(MS_U16);
typedef void                (*IOCTL_AUDIO_SETFWSTATUS)(MS_BOOL);
typedef void                (*IOCTL_AUDIO_EXITAUDIOSYSTEM)(void);
typedef void                (*IOCTL_AUDIO_REBOOTDSP)(MS_BOOL);
typedef void                (*IOCTL_AUDIO_SENDINTRUPT)(MS_BOOL, MS_U8);
typedef void                (*IOCTL_AUDIO_I2SSETMODE)(AUDIO_I2S_MODE_TYPE, AUDIO_I2S_MODE_VALUE);
typedef void                (*IOCTL_AUTIO_WRITEPREINITTABLE)(void);
typedef void                (*IOCTL_AUDIO_ENAEARPHONE_LOWPOWER_STAGE)(void);
typedef void                (*IOCTL_AUDIO_ENAEARPHONE_HIGHDRIVING_STAGE)(void);
typedef MS_BOOL             (*IOCTL_AUDIO_SUSPEND)(void);
typedef MS_BOOL             (*IOCTL_AUDIO_RESUME)(void);

//=============================================================
// AUDIO_SOUND RELATIONAL API FUNCTION
//=============================================================
typedef MS_BOOL             (*IOCTL_SND_PROCESSENABLE)(Sound_ENABLE_Type, MS_BOOL);
typedef MS_BOOL             (*IOCTL_SND_SETPARAM1)(Sound_SET_PARAM_Type, MS_U16, MS_U16);
typedef MS_U16              (*IOCTL_SND_GETPARAM1)(Sound_GET_PARAM_Type, MS_U16);
typedef MS_BOOL             (*IOCTL_AUDIO_SOUND_SETPARAM)(Sound_SET_Type, MS_U32*);
typedef MS_BOOL             (*IOCTL_AUDIO_SOUND_GETPARAM)(const MS_S8*, MS_U32, MS_S8*, MS_U32);
typedef MS_BOOL             (*IOCTL_AUDIO_SOUNDEFFECT_SETPARAM)(const MS_S8*, MS_U32, MS_S8*, MS_U32);
typedef void                (*IOCTL_AUDIO_SETABSOLUTEVOLUME)(MS_U8, MS_U8, MS_U8);
//typedef void                (*IOCTL_AUDIO_SETPRESCALE)(MS_U8, MS_U8);
//typedef void                (*IOCTL_AUDIO_SETMUTE)(MS_U8, MS_BOOL);
typedef void                (*IOCTL_AUDIO_SOURCECONNECT)(AUDIO_OUTPORT_SOURCE_TYPE, MS_BOOL);
//typedef void                (*IOCTL_AUDIO_SETBALANCE)(MS_U8);
//typedef void                (*IOCTL_AUDIO_ENABLEEQ)(MS_BOOL);
//typedef void                (*IOCTL_AUDIO_ENABLETONE)(MS_BOOL);
//typedef void                (*IOCTL_AUDIO_ENABLEAUTOVOLUME)(MS_BOOL);
//typedef void                (*IOCTL_AUDIO_ENABLESURROUND)(MS_BOOL);
//typedef void                (*IOCTL_AUDIO_ENABLEBALANCE)(MS_BOOL);
typedef void                (*IOCTL_AUDIO_SETDYNAMICBASS)(MS_U8);
//typedef void                (*IOCTL_AUDIO_SETBASS)(MS_U8);
//typedef void                (*IOCTL_AUDIO_ABSOLUTEBASS)(MS_U8);
//typedef void                (*IOCTL_AUDIO_SETTREBLE)(MS_U8);
typedef void                (*IOCTL_AUDIO_ABSOLUTETREBLE)(MS_U8);
//typedef void                (*IOCTL_AUDIO_SETEQ)(MS_U8, MS_U8);
typedef void                (*IOCTL_AUDIO_SETEQ_7BAND)(MS_U8, MS_U8);
typedef void                (*IOCTL_AUDIO_SETAVCTHRESHOLD)(MS_U8);
typedef void                (*IOCTL_AUDIO_SETAVCMODE)(MS_U8);
typedef void                (*IOCTL_AUDIO_SETAVCAT)(MS_U8);
typedef void                (*IOCTL_AUDIO_SETAVCRT)(MS_U8);
typedef void                (*IOCTL_AUDIO_SETBUFFERPROCESS)(MS_U8);
typedef void                (*IOCTL_AUDIO_SETSURROUNDXA)(MS_U8);
typedef void                (*IOCTL_AUDIO_SETSURROUNDXB)(MS_U8);
typedef void                (*IOCTL_AUDIO_SETSURROUNDXK)(MS_U8);
typedef void                (*IOCTL_AUDIO_SETSURROUNDLPFGAIN)(MS_U8);
typedef MS_U8               (*IOCTL_AUDIO_CONVERTVOLUMEUNIT)(MS_U8);
typedef void                (*IOCTL_AUDIO_SETPEQCOEF)(AUDIO_PEQ_COEF*);
//typedef void                (*IOCTL_AUDIO_ENABLEPEQ)(MS_BOOL);
typedef MS_BOOL             (*IOCTL_AUDIO_ENABLEDCREMOVE)(MS_BOOL);

//============================================================
// AUDIO_DTV RELATIONAL API FUNCTION
//============================================================
typedef MS_BOOL             (*IOCTL_AUDIO_RELEASEDECODESYSTEM)(AUDIO_DEC_ID);
typedef MS_BOOL             (*IOCTL_AUDIO_SETDECODESYSTEM)(AUDIO_DEC_ID, AudioDecStatus_t*);
typedef MS_BOOL             (*IOCTL_AUDIO_GETDECODESYSTEM)(AUDIO_DEC_ID, AudioDecStatus_t*);
typedef MS_BOOL             (*IOCTL_AUDIO_SETDECODECMD)(AUDIO_DEC_ID, En_DVB_decCmdType);
typedef AUDIO_DEC_ID        (*IOCTL_AUDIO_OPENDECODESYSTEM)(AudioDecStatus_t *);
typedef MS_BOOL             (*IOCTL_AUDIO_SETSYSTEM)(En_DVB_decSystemType);
typedef MS_BOOL             (*IOCTL_AUDIO_GETDECSYSSUPPORTSTATUS)(En_DVB_decSystemType);
typedef MS_BOOL             (*IOCTL_AUDIO_ISMADLOCK)(void);
typedef En_DVB_decCmdType   (*IOCTL_AUDIO_GETDECSTATUS)(void);
typedef AU_DVB_DECCMD       (*IOCTL_AUDIO_GETCOMMAND)(En_DVB_AudioDecoder);
typedef void                (*IOCTL_AUDIO_SETCOMMAND)(En_DVB_decCmdType);
typedef MS_BOOL             (*IOCTL_AUDIO_GETMAD_LOCK)(void);
typedef void                (*IOCTL_AUDIO_SETADOUTPUTMODE)(MS_U8);
typedef void                (*IOCTL_AUDIO_SETADABSOLUTEVOLUME)(MS_U8);
typedef void                (*IOCTL_AUDIO_SETMAINVOLUMEFORAD)(MS_U8 u8Vol1, MS_U8 u8Vol2);
typedef void                (*IOCTL_AUDIO_ADSETMUTE)(MS_BOOL);

//============================================================
// AUDIO_PVR RELATED FUNCTIONS
//============================================================
typedef void                (*IOCTL_AUDIO_SETENCINIT)(AU_DVB_ENCBR, MS_U8);
typedef void                (*IOCTL_AUDIO_SETENCCOMMAND)(AU_DVB_ENCCMD);
typedef void                (*IOCTL_AUDIO_GETENCODEFRAMEINFO)(AU_DVB_ENC_FRAME_INFO *);
typedef MS_BOOL             (*IOCTL_AUDIO_GETENCODEDONEFLAG)(void);

//============================================================
// AUDIO_ATV RELATIONAL API FUNCTION
//============================================================
typedef void                (*IOCTL_AUDIO_SIF_SETSTANDARD)(AUDIOSTANDARD_TYPE);
typedef MS_U8               (*IOCTL_AUDIO_SIF_GETSOUNDMODE)(void);
typedef MS_BOOL             (*IOCTL_AUDIO_SIF_SETSOUNDMODE)(MS_U8);
typedef MS_BOOL             (*IOCTL_AUDIO_SIF_GETAUDIOSTATUS)(AUDIOSTATUS *);
typedef void                (*IOCTL_AUDIO_SIF_STARTAUTOSTANDARDDETECTION)(void);
typedef AUDIOSTANDARD_TYPE  (*IOCTL_AUDIO_SIF_GETRESULTOFAUTOSTANDARDDETECTION)(void);
typedef AUDIOSTANDARD_TYPE  (*IOCTL_AUDIO_SIF_CONVERTTOBASICAUDIOSTANDARD)(AUDIOSTANDARD_TYPE);
typedef void                (*IOCTL_AUDIO_SIF_SETTHRESHOLD)(THR_TBL_TYPE *);
typedef void                (*IOCTL_AUDIO_SIF_SETPRESCALE)(MS_U8, MS_S32);
typedef MS_BOOL             (*IOCTL_AUDIO_SIF_ISPALTYPE)(AUDIO_SIF_PAL_TYPE);
typedef void                (*IOCTL_AUDIO_SIF_SETPALTYPE)(AUDIO_SIF_PAL_TYPE);
typedef void                (*IOCTL_AUDIO_SIF_SENDCMD)(En_AUD_SIF_CmdType, MS_U8, MS_U8);
typedef void                (*IOCTL_AUDIO_SIF_SHIFT)(En_AUD_VIF_Type);

#if 1
//============================================================
// AUDIO_FM_RADIO RELATIONAL API FUNCTION
//============================================================
typedef MS_U8               (*IOCTL_AUDIO_FM_RADIO_GETSOUNDMODE)(void);
typedef void                (*IOCTL_AUDIO_FM_RADIO_SETSOUNDMODE)(MS_U8);
typedef void                (*IOCTL_AUDIO_FM_RADIO_DEEMPHASSISOPTION)(MS_BOOL);
typedef MS_S8               (*IOCTL_AUDIO_FM_RADIO_GET_DC_AMP)(void);
typedef MS_U8               (*IOCTL_AUDIO_FM_RADIO_GET_NSR)(void);
typedef void                (*IOCTL_AUDIO_FM_RADIO_RESET)(void);
#endif
//=============================================================
// AUDIO_SPDIF RELATIONAL API FUNCTION
//=============================================================
typedef void                (*IOCTL_AUDIO_SPDIF_HWEN)(MS_BOOL);
typedef void                (*IOCTL_AUDIO_SPDIF_SETMUTE)(MS_BOOL);
typedef void                (*IOCTL_AUDIO_SPDIF_SETMODE)(MS_U8);
typedef MS_U8               (*IOCTL_AUDIO_SPDIF_GETMODE)(void);
typedef void                (*IOCTL_AUDIO_SPDIF_SETSCMS)(MS_U8, MS_U8);
typedef MS_U8               (*IOCTL_AUDIO_SPDIF_GETSCMS)(void);
typedef void                (*IOCTL_AUDIO_SPDIF_MONITOR)(void);
typedef MS_BOOL             (*IOCTL_AUDIO_SPDIF_CHANNELSTATUS_CTRL)(AUDIO_SPDIF_CS_TYPE, AUDIO_SPDIF_CS_TYPE_STATUS);
typedef MS_BOOL             (*IOCTL_AUDIO_DIGITALOUT_SETCHANNELSTATUS)(DIGITAL_OUTPUT_TYPE, Digital_Out_Channel_Status_t*);
typedef void                (*IOCTL_AUDIO_DIGITALOUT_GETCHANNELSTATUS)(DIGITAL_OUTPUT_TYPE, Digital_Out_Channel_Status_t*);
typedef MS_BOOL             (*IOCTL_AUDIO_DIGITALOUT_SETDEVICECAPABILITY)(DIGITAL_OUTPUT_TYPE, AUDIO_FORMAT_CODE, Digital_Out_Device_Capability_t*);

//=============================================================
// AUDIO_HDMI RELATIONAL API FUNCTION
//=============================================================
typedef void                (*IOCTL_AUDIO_HDMI_TX_SETMUTE)(MS_BOOL);
typedef void                (*IOCTL_AUDIO_HDMI_TX_GETSTATUS)(MS_BOOL*, AUDIO_FS_TYPE*, HDMI_TX_OUTPUT_TYPE*);
typedef MS_U8               (*IOCTL_AUDIO_HDMI_MONITOR)(void);
typedef MS_BOOL             (*IOCTL_AUDIO_HDMI_GETNONPCMFLAG)(void);
typedef void                (*IOCTL_AUDIO_HDMI_SETNONPCM)(MS_U8);
typedef AUDIO_HDMI_RX_TYPE  (*IOCTL_AUDIO_HDMI_RX_SETNONPCM)(MS_U8);
typedef AUDIO_HDMI_RX_TYPE  (*IOCTL_AUDIO_HDMI_RX_GETNONPCM)(void);
typedef void                (*IOCTL_AUDIO_DTV_HDMI_CFG)(MS_U8);
typedef MS_U16              (*IOCTL_AUDIO_HDMI_GETSYNTHFREG)(void);
typedef void                (*IOCTL_AUDIO_HDMI_SETDOWNSAMPLE)(MS_U8);
typedef void                (*IOCTL_AUDIO_HDMI_TX_SETMODE)(HDMI_TX_OUTPUT_TYPE);
typedef MS_BOOL             (*IOCTL_AUDIO_HDMI_RX_GetHDMIINAUDIOSTATUS)(ST_HDMI_AUDIO_STATUS*);

//=============================================================
// AUDIO_ADVANCE_SOUND RELATIONAL API FUNCTION
//=============================================================
typedef MS_BOOL             (*IOCTL_AUDIO_ADVSOUND_PROCESSENABLE)(ADVSND_TYPE);
typedef MS_BOOL             (*IOCTL_AUDIO_ADVSOUND_SUBPROCESSENABLE)(ADVFUNC, MS_BOOL);
typedef MS_BOOL             (*IOCTL_AUDIO_ADVSOUND_SETPARAM)(ADVSND_PARAM, MS_U16, MS_U16);
typedef MS_BOOL             (*IOCTL_AUDIO_ADVSND_SETPARAM)(ADVSND_PARAM, MS_U32, MS_U16);
//typedef MS_U32              (*IOCTL_AUDIO_ADVSOUND_GETPARAM)(ADVSND_GET_PARAM);
typedef MS_BOOL             (*IOCTL_AUDIO_ADVSOUND_SETASFPARAM)(ADVSND_PARAM, MS_S16, MS_S16);
//typedef MS_U32              (*IOCTL_AUDIO_ADVSOUND_GETASFPARAM)(ADVSND_GET_PARAM param);

typedef MS_U32              (*IOCTL_AUDIO_ADVSOUND_GETINFO)(AdvSound_GET_Type);
typedef void                (*IOCTL_AUDIO_DBXTV_SETMODE)(EN_DBXTV_TotSonMode, EN_DBXTV_TotVolMode, EN_DBXTV_TotSurMode,MS_U32);
typedef void                (*IOCTL_AUDIO_SEINIT)(void);
typedef void                (*IOCTL_AUDIO_SETADVSNDSYS)(En_DVB_advsndType);
//typedef void                (*IOCTL_AUDIO_ADVSND_SET_SOUND_PARAM)(AdvSound_SET_Type, MS_U32*);
typedef void                (*IOCTL_AUDIO_SETVDS)(MS_U8);
typedef void                (*IOCTL_AUDIO_SETVSPK)(MS_U8);
typedef void                (*IOCTL_AUDIO_SETSRS)(MS_U8);
typedef void                (*IOCTL_AUDIO_SETBBE)(MS_U8, MS_U8);
typedef void                (*IOCTL_AUDIO_VSPK_WMOD)(MS_U8);
typedef void                (*IOCTL_AUDIO_VSPK_SMOD)(MS_U8);
typedef void                (*IOCTL_AUDIO_SRS_DC)(MS_U8);
typedef void                (*IOCTL_AUDIO_SRS_TRUBASS)(MS_U8);
typedef void                (*IOCTL_AUDIO_SRS_SETTSXTPARA)(MS_U8, MS_U8);
typedef void                (*IOCTL_AUDIO_SETSRSTSHD)(MS_U8);
typedef void                (*IOCTL_AUDIO_TSHD_TruBass)(MS_U8);
typedef void                (*IOCTL_AUDIO_TSHD_DEFINITION)(MS_U8);
typedef void                (*IOCTL_AUDIO_TSHD_CLARITY)(MS_U8);
typedef void                (*IOCTL_AUDIO_SRS_SETTSHDPARA)(MS_U8, MS_U8);
typedef void                (*IOCTL_AUDIO_COPY_PARAMETER)(ADVSND_PARAM, void*, MS_U32);

//=============================================================
// AUDIO_KTV RELATIONAL API FUNCTION
//=============================================================
typedef MS_BOOL             (*IOCTL_AUDIO_SETKTVINFO)(AUDIO_KTV_MODEL, AUDIO_KTV_PARAMTYPE, MS_U32, MS_U32);
typedef MS_U32              (*IOCTL_AUDIO_GETKTVINFO)(AUDIO_KTV_MODEL,AUDIO_KTV_INFOTYPE);
typedef void                (*IOCTL_AUDIO_SETMIXMODEVOLUME)(AUDIO_SOURCE_INFO_TYPE, AUDIO_MIX_VOL_TYPE, MS_U8, MS_U8);
typedef void                (*IOCTL_AUDIO_SETMIXMODEMUTE)(AUDIO_SOURCE_INFO_TYPE, AUDIO_MIX_VOL_TYPE, MS_BOOL);
typedef void                (*IOCTL_AUDIO_PLAYMENUSOUND)(void);

//=============================================================
// AUDIO_MM RELATIONAL API FUNCTION
//=============================================================
// Set Info
typedef MS_BOOL             (*IOCTL_AUDIO_SETCOMMAUDIOINFO)(Audio_COMM_infoType, MS_VIRT, MS_VIRT);
typedef MS_BOOL             (*IOCTL_AUDIO_SETDECODEPARAM)(AUDIO_DEC_ID DecId, Audio_ParamType paramType, MS_U32* Param );
typedef MS_BOOL             (*IOCTL_AUDIO_SETMPEGINFO)(Audio_MPEG_infoType, MS_VIRT, MS_VIRT);
typedef MS_BOOL             (*IOCTL_AUDIO_SETAC3INFO)(Audio_AC3_infoType , MS_VIRT, MS_VIRT);
typedef MS_BOOL             (*IOCTL_AUDIO_SETAC3PINFO)(Audio_AC3P_infoType , MS_VIRT, MS_VIRT);
typedef MS_BOOL             (*IOCTL_AUDIO_SETAACINFO)(Audio_AAC_infoType, MS_VIRT, MS_VIRT );
typedef MS_BOOL             (*IOCTL_AUDIO_SETWMAINFO)(Audio_WMA_infoType, MS_VIRT, MS_VIRT );
typedef MS_BOOL             (*IOCTL_AUDIO_SETDTSCOMMONCTRL)(Audio_DTS_infoType , Audio_DTS_CtrlType);
// Get Info
typedef MS_U64              (*IOCTL_AUDIO_GETCOMMAUDIOINFO)(Audio_COMM_infoType);
typedef MS_U32              (*IOCTL_AUDIO_GETMPEGINFO)(Audio_MPEG_infoType);
typedef MS_U32              (*IOCTL_AUDIO_GETAC3INFO)(Audio_AC3_infoType);
typedef MS_U32              (*IOCTL_AUDIO_GETAC3PINFO)(Audio_AC3P_infoType);
typedef MS_U32              (*IOCTL_AUDIO_GETAACINFO)(Audio_AAC_infoType);
typedef MS_U32              (*IOCTL_AUDIO_GETWMAINFO)(Audio_WMA_infoType);
typedef MS_U32              (*IOCTL_AUDIO_GETDTSINFO)(Audio_DTS_infoType);
typedef MS_U8               (*IOCTL_AUDIO_XPCM_PARAM)(XPCM_TYPE, MS_U8 , MS_U16, MS_U8 , MS_U16, MS_U16);
typedef void                (*IOCTL_AUDIO_XPCM2_PARAM)(XPCM_TYPE, MS_U8 , MS_U16, MS_U8 , MS_U16, MS_U16);
typedef MS_U16              (*IOCTL_AUDIO_XPCM2_CHECKINTSTATUS)(void);
// RA8
typedef MS_U8               (*IOCTL_AUDIO_RA8_PARAM)(MS_U16, MS_U16, MS_U16, MS_U16*, MS_U16*, MS_U16*, MS_U16*, MS_U16*);

typedef void                (*IOCTL_AUDIO_INIT)(En_DVB_decSystemType);
typedef void                (*IOCTL_AUDIO_STARTDECODE)(void);
typedef void                (*IOCTL_AUDIO_STARTBROWSE)(void);
typedef void                (*IOCTL_AUDIO_STOPDECODE)(void);
typedef void                (*IOCTL_AUDIO_PAUSEDECODE)(void);
typedef MS_U8               (*IOCTL_AUDIO_CHECKPLAYDONE)(void);
typedef MS_U16              (*IOCTL_AUDIO_GETRESIDUALBUFFERSIZE)(void);
typedef MS_U16              (*IOCTL_AUDIO_GETPCMBUFFERSIZE)(MS_U16);
typedef MS_U16              (*IOCTL_AUDIO_GETPCMBUFFERSIZE2)(MS_U16, MS_U16);
typedef MS_U32              (*IOCTL_AUDIO_GETCURRENTFRAMENUMBER)(void);
typedef MS_U16              (*IOCTL_AUDIO_GETSAMPLERATE)(void);
typedef MS_U32              (*IOCTL_AUDIO_GETBITRATE)(void);
typedef Audio_MP3_LAYER     (*IOCTL_AUDIO_GETLAYER)(void);
typedef MS_U8               (*IOCTL_AUDIO_CHECKINPUTREQUEST)(MS_PHY*, MS_U32*);
typedef void                (*IOCTL_AUDIO_SETINPUT)(void);
typedef void                (*IOCTL_AUDIO_SETSAMPLERATEINDEX)(MS_U16);
typedef void                (*IOCTL_AUDIO_SETXPCMPARAM)(XPCM_TYPE, MS_U8, MS_U16, MS_U8, MS_U16, MS_U16);
typedef void                (*IOCTL_AUDIO_FILEENDNOTIFICATION)(void);
typedef void                (*IOCTL_AUDIO_FILEENDDATAHANDLE)(MS_U32);
typedef MS_U32              (*IOCTL_AUDIO_GETPLAYTICK)(void);
typedef MS_U16              (*IOCTL_AUDIO_GETESMEMCNT)(void);
typedef void                (*IOCTL_AUDIO_SETASFPARM)(WMA_ASF_PARMTYPE, MS_U32);
typedef void                (*IOCTL_AUDIO_MM_SETINPUT)(En_DVB_AudioDecoder, MS_U8);
typedef MS_U32              (*IOCTL_AUDIO_MM_CHECKPLAYDONE)(En_DVB_AudioDecoder);
typedef MS_U8               (*IOCTL_AUDIO_MM_CHECKINPUTREQUEST)(En_DVB_AudioDecoder, MS_PHY*, MS_U32*);
typedef void                (*IOCTL_AUDIO_DMAREADER_INIT)(SAMPLE_RATE);
typedef void                (*IOCTL_AUDIO_DMAREADER_ALLINPUT_INIT)(void);
typedef MS_BOOL             (*IOCTL_AUDIO_DMAREADER_WRITEPCM)(void* , MS_U32 );
typedef void                (*IOCTL_AUDIO_DMAWRITER_INIT)(void);
typedef void                (*IOCTL_AUDIO_USBPCM_ENABLE)(MS_BOOL);
typedef void                (*IOCTL_AUDIO_USBPCM_SETFLAG)(MS_BOOL);
typedef MS_BOOL             (*IOCTL_AUDIO_USBPCM_GETFLAG)(void);
typedef void                (*IOCTL_AUDIO_USBPCM_GETMEMINFO)(AUDIO_UPLOAD_INFO *);
typedef MS_BOOL             (*IOCTL_AUDIO_PCMCAPTURE_INIT)(const AUDIO_DEVICE_TYPE , const AUDIO_CAPTURE_SOURCE_TYPE);
typedef MS_BOOL             (*IOCTL_AUDIO_PCMCAPTURE_START)(const AUDIO_DEVICE_TYPE);
typedef MS_BOOL             (*IOCTL_AUDIO_PCMCAPTURE_STOP)(const AUDIO_DEVICE_TYPE);
typedef MS_BOOL             (*IOCTL_AUDIO_PCMCAPTURE_READ)(const AUDIO_DEVICE_TYPE , void *, const MS_U32);
typedef MS_BOOL             (*IOCTL_AUDIO_VOIP_CONFIG)(AUDIO_VoIP_CONFIG_TYPE, MS_U32, MS_U32);
typedef MS_BOOL             (*IOCTL_AUDIO_ALSA_CHECK)(void);
typedef MS_BOOL             (*IOCTL_AUDIO_ALSA_ENABLE)(MS_BOOL);
typedef MS_BOOL             (*IOCTL_AUDIO_UNI_CHECKDECODEDONE)(MS_PHY*, MS_U32*);
typedef void                (*IOCTL_AUDIO_UNI_SETOUTPUT)(MS_U32);
typedef void                (*IOCTL_AUDIO_UNI_SET_PCMINPUTWRITEADDR)(MS_U32);
typedef MS_U32              (*IOCTL_AUDIO_UNI_GET_OUTPCMLEVEL)(void);
typedef void                (*IOCTL_AUDIO_RINGTASK)(audioRingTask*);
typedef void                (*IOCTL_AUDIO_RING_DATATRANSFER)(audioRingTask* , audioRingTask* );
typedef MS_BOOL             (*IOCTL_AUDIO_MM2_INITAESINFO)(AUDIO_DEC_ID);
typedef MS_BOOL             (*IOCTL_AUDIO_MM2_CHECKAESINFO)(AUDIO_DEC_ID, AES_INFO*);
typedef MS_BOOL             (*IOCTL_AUDIO_MM2_INPUTAESFINISHED)( AUDIO_DEC_ID, MS_U32, MS_BOOL, MS_U64);
typedef MS_BOOL             (*IOCTL_AUDIO_SETAUDIOPARAM2)(AUDIO_DEC_ID, Audio_ParamType, MS_VIRT);
typedef MS_BOOL             (*IOCTL_AUDIO_GETAUDIOINFO2)(AUDIO_DEC_ID, Audio_InfoType, void*);
typedef MS_PHY              (*IOCTL_AUDIO_GETDDRINFO)(AUDIO_DEC_ID, EN_AUDIO_DDRINFO);
typedef MS_U32              (*IOCTL_AUDIO_GETCAPS)(AUDIO_DEC_ID, MS_U32 *);
typedef MS_S32              (*IOCTL_AUDIO_PCMOPEN)(void*);
typedef MS_S32              (*IOCTL_AUDIO_PCMCLOSE)(MS_S32);
typedef MS_S32              (*IOCTL_AUDIO_PCMSTART)(MS_S32);
typedef MS_S32              (*IOCTL_AUDIO_PCMSTOP)(MS_S32);
typedef MS_S32              (*IOCTL_AUDIO_PCMSET)(MS_S32, MS_U32, const void*);
typedef MS_S32              (*IOCTL_AUDIO_PCMGET)(MS_S32, MS_U32, void*);
typedef MS_U32              (*IOCTL_AUDIO_PCMREAD)(MS_S32, void*, MS_U32);
typedef MS_U32              (*IOCTL_AUDIO_PCMWRITE)(MS_S32, const void*, MS_U32);
typedef MS_S32              (*IOCTL_AUDIO_PCMFLUSH)(MS_S32);
typedef MS_S32              (*IOCTL_AUDIO_ENCODEROPEN)(const char*);
typedef MS_S32              (*IOCTL_AUDIO_ENCODERCLOSE)(MS_S32);
typedef MS_S32              (*IOCTL_AUDIO_ENCODERSTART)(MS_S32);
typedef MS_S32              (*IOCTL_AUDIO_ENCODERSTOP)(MS_S32);
typedef MS_S32              (*IOCTL_AUDIO_ENCODERIOCTL)(MS_S32, const char*, void*);
typedef MS_U32              (*IOCTL_AUDIO_ENCODEREAD)(MS_S32, void*, MS_U32);
typedef MS_S32              (*IOCTL_AUDIO_ENCODERFLUSH)(MS_S32);
typedef MS_S32              (*IOCTL_AUDIO_DEBUGCMDREAD)(MS_U64*, const char*, MS_U32);
typedef MS_S32              (*IOCTL_AUDIO_DEBUGCMDWRITE)(MS_U64*, const char*, MS_U32);
typedef MS_S32              (*IOCTL_AUDIO_SYSTEM_CONTROL)(MS_U8*, MS_U32);
typedef MS_S32              (*IOCTL_AUDIO_CHANNEL_OPEN)(void);
typedef MS_S32              (*IOCTL_AUDIO_CHANNEL_OPEN_BYID)(MS_S32);
typedef MS_S32              (*IOCTL_AUDIO_CHANNEL_CLOSE)(MS_S32);
typedef MS_S32              (*IOCTL_AUDIO_CHANNEL_IOCTL)(MS_S32, const MS_U8*, MS_U32, MS_U32*);
// to add...

typedef struct _AUDIO_INSTANT_PRIVATE
{
    //============================================================
    // AUDIO_SYSTEM RELATIONAL API FUNCTION
    //============================================================

    IOCTL_AUDIO_INITIALIZE                          fpAUDIOInitialize;
    IOCTL_AUDIO_SET_POWER_ON                        fpAUDIOSetPowerOn;
    IOCTL_AUDIO_SETDSPBASEADDR                      fpAUDIOSetDSPBaseAddr;
    IOCTL_AUDIO_GETDSPMADBASEADDR                   fpAUDIOGetDSPMadBaseAddr;
    IOCTL_AUDIO_WRITEDECMAILBOX                     fpAUDIOWriteDecMailBox;
    IOCTL_AUDIO_TRIGGERSIFPLL                       fpAUDIOTriggerSifPLL;
    IOCTL_AUDIO_MONITOR                             fpAUDIOMonitor;
    IOCTL_AUDIO_SETSYSTEMINFO                       fpAUDIOSetSystemInfo;
    IOCTL_AUDIO_ASF_DATA_TO_DDR                     fpAUDIOASFDataToDDR;
    IOCTL_AUDIO_R2_DEBUG_MSG                        fpAUDIOR2DebugMSG;
    IOCTL_AUDIO_R2_DDR_LETENCY_MSG                  fpAUDIOR2DDRLetencyMSG;
    IOCTL_AUDIO_SETOUTCONNECTIVITY                  fpAUDIOSetOutConnectivity;
    IOCTL_AUDIO_SETPATHINFO                         fpAUDIOSetPathInfo;
    IOCTL_AUDIO_GETPATHINFO                         fpAUDIOGetPathInfo;
    IOCTL_AUDIO_SETOUTPUTINFO                       fpAUDIOSetOutputInfo;
    IOCTL_AUDIO_GETOUTPUTINFO                       fpAUDIOGetOutputInfo;
    IOCTL_AUDIO_SETSOURCEINFO                       fpAUDIOSetSourceInfo;
    IOCTL_AUDIO_INPUTSWITCH                         fpAUDIOInputSwitch;
    IOCTL_AUDIO_SETINTERNALPATH                     fpAUDIOSetInternalPath;
    IOCTL_AUDIO_SETINPUTSCALE                       fpAUDIOSetInputScale;
    IOCTL_AUDIO_SETSCARTSCALE                       fpAUDIOSetScartScale;
    IOCTL_AUDIO_AUDYSSEY_VXT_SET_THRESH_LEVEL_TABLE fpAUDIOAudysseyVXTSetThreshLevelTable;
    IOCTL_AUDIO_SPDIF_SETCHANNELSTATUS              fpAUDIOSPDIFSetChannelStauts;
    IOCTL_AUDIO_SPDIF_GETCHANNELSTATUS              fpAUDIOSPDIFGetChannelStatus;

    IOCTL_AUDIO_SETDATACAPTURESOURCE                fpAUDIOSetDataCaptureSource;
    IOCTL_AUDIO_SETOUTPUTSOURCEINFO                 fpAUDIOSetOutputSourceInfo;
    IOCTL_AUDIO_GETOUTPUTSOURCEINFO                 fpAUDIOGetOutputSourceInfo;
    IOCTL_AUDIO_READMAILBOX                         fpAUDIOReadMailBox;
    IOCTL_AUDIO_WRITEMAILBOX                        fpAUDIOWriteMailBox;
    IOCTL_AUDIO_FWTRIGGERDSP                        fpAUDIOFwTriggerDsp;
    IOCTL_AUDIO_SETFWSTATUS                         fpAUDIOSetFwStatus;
    IOCTL_AUDIO_EXITAUDIOSYSTEM                     fpAUDIOExitAudioSystem;
    IOCTL_AUDIO_REBOOTDSP                           fpAUDIORebootDSP;
    IOCTL_AUDIO_SENDINTRUPT                         fpAUDIOSendIntrupt;
    IOCTL_AUDIO_I2SSETMODE                          fpAUDIOI2SSetMode;
    IOCTL_AUTIO_WRITEPREINITTABLE                   fpAUDIOWritePreInitTable;
    IOCTL_AUDIO_ENAEARPHONE_LOWPOWER_STAGE          fpAUDIOEnaEarphoneLowPowerStage;
    IOCTL_AUDIO_ENAEARPHONE_HIGHDRIVING_STAGE       fpAUDIOEnaEarphoneHighDrivingStage;
    IOCTL_AUDIO_SUSPEND                             fpAUDIOSuspend;
    IOCTL_AUDIO_RESUME                              fpAUDIOResume;


    //=============================================================
    // AUDIO_SOUND RELATIONAL API FUNCTION
    //=============================================================
    IOCTL_SND_PROCESSENABLE                         fpSNDProcessEnable;
    IOCTL_SND_SETPARAM1                             fpSNDSetParam1;
    IOCTL_SND_GETPARAM1                             fpSNDGetParam1;
    IOCTL_AUDIO_SOUND_SETPARAM                      fpAUDIOSoundSetParam;
    IOCTL_AUDIO_SETABSOLUTEVOLUME                   fpAUDIOSetAbsoluteVolume;
    //IOCTL_AUDIO_SETPRESCALE                         fpAUDIOSetPreScale;
    //IOCTL_AUDIO_SETMUTE                             fpAUDIOSetMute;
    IOCTL_AUDIO_SOURCECONNECT                       fpAUDIOSourceConnect;
    //IOCTL_AUDIO_SETBALANCE                          fpAUDIOSetBalance;
    //IOCTL_AUDIO_ENABLEEQ                            fpAUDIOEnableEQ;
    //IOCTL_AUDIO_ENABLETONE                          fpAUDIOEnableTone;
    //IOCTL_AUDIO_ENABLEAUTOVOLUME                    fpAUDIOEnableAutoVolume;
    //IOCTL_AUDIO_ENABLESURROUND                      fpAUDIOEnableSurround;
    //IOCTL_AUDIO_ENABLEBALANCE                       fpAUDIOEnableBalance;
    IOCTL_AUDIO_SETDYNAMICBASS                      fpAUDIOSetDynamicBass;
    //IOCTL_AUDIO_SETBASS                             fpAUDIOSetBass;
    //IOCTL_AUDIO_ABSOLUTEBASS                        fpAUDIOAbsoluteBass;
    //IOCTL_AUDIO_SETTREBLE                           fpAUDIOSetTreble;
    IOCTL_AUDIO_ABSOLUTETREBLE                      fpAUDIOAbsoluteTreble;
    //IOCTL_AUDIO_SETEQ                               fpAUDIOSetEq;
    IOCTL_AUDIO_SETEQ_7BAND                         fpAUDIOSetEq7band;
    IOCTL_AUDIO_SETAVCTHRESHOLD                     fpAUDIOSetAvcThreshold;
    IOCTL_AUDIO_SETAVCMODE                          fpAUDIOSetAvcMode;
    IOCTL_AUDIO_SETAVCAT                            fpAUDIOSetAvcAT;
    IOCTL_AUDIO_SETAVCRT                            fpAUDIOSetAvcRT;
    IOCTL_AUDIO_SETBUFFERPROCESS                    fpAUDIOSetBufferProcess;
    IOCTL_AUDIO_SETSURROUNDXA                       fpAUDIOSetSurroundXA;
    IOCTL_AUDIO_SETSURROUNDXB                       fpAUDIOSetSurroundXB;
    IOCTL_AUDIO_SETSURROUNDXK                       fpAUDIOSetSurroundXK;
    IOCTL_AUDIO_SETSURROUNDLPFGAIN                  fpAUDIOSetSurroundLPFGain;
    IOCTL_AUDIO_CONVERTVOLUMEUNIT                   fpAUDIOConvertVolumeUnit;
    IOCTL_AUDIO_SETPEQCOEF                          fpAUDIOSetPEQCoef;
    //IOCTL_AUDIO_ENABLEPEQ                           fpAUDIOEnablePEQ;
    IOCTL_AUDIO_ENABLEDCREMOVE                      fpAUDIOEnableDcRemove;

    //============================================================
    // AUDIO_DTV RELATIONAL API FUNCTION
    //============================================================
    IOCTL_AUDIO_RELEASEDECODESYSTEM                 fpAUDIOReleaseDecodeSystem;
    IOCTL_AUDIO_SETDECODESYSTEM                     fpAUDIOSetDecodeSystem;
    IOCTL_AUDIO_GETDECODESYSTEM                     fpAUDIOGetDecodeSystem;
    IOCTL_AUDIO_SETDECODECMD                        fpAUDIOSetDecodeCmd;
    IOCTL_AUDIO_OPENDECODESYSTEM                    fpAUDIOOpenDecodeSystem;
    IOCTL_AUDIO_SETSYSTEM                           fpAUDIOSetSystem;
    IOCTL_AUDIO_GETDECSYSSUPPORTSTATUS              fpAUDIOGetDecSysSupportStatus;
    IOCTL_AUDIO_ISMADLOCK                           fpAUDIOIsMadLock;
    IOCTL_AUDIO_GETDECSTATUS                        fpAUDIOGetDecStatus;
    IOCTL_AUDIO_GETCOMMAND                          fpAUDIOGetCommand;
    IOCTL_AUDIO_SETCOMMAND                          fpAUDIOSetCommand;
    IOCTL_AUDIO_GETMAD_LOCK                         fpAUDIOGetMADLock;
    IOCTL_AUDIO_SETADOUTPUTMODE                     fpAUDIOSetADOutputMode;
    IOCTL_AUDIO_SETADABSOLUTEVOLUME                 fpAUDIOSetADAbsoluteVoluem;
    IOCTL_AUDIO_SETMAINVOLUMEFORAD                  fpAUDIOSetMainVolumeForAD;
    IOCTL_AUDIO_ADSETMUTE                           fpAUDIOADSetMute;

    //============================================================
    // AUDIO_PVR RELATED FUNCTIONS
    //============================================================
    IOCTL_AUDIO_SETENCINIT                          fpAUDIOSetEncInit;
    IOCTL_AUDIO_SETENCCOMMAND                       fpAUDIOSetEncCommand;
    IOCTL_AUDIO_GETENCODEFRAMEINFO                  fpAUDIOGetEncodeFrameInfo;
    IOCTL_AUDIO_GETENCODEDONEFLAG                   fpAUDIOGetEncodeDoneFlag;

    //============================================================
    // AUDIO_ATV RELATIONAL API FUNCTION
    //============================================================
    IOCTL_AUDIO_SIF_SETSTANDARD                     fpAUDIOSIFSetStandard;
    IOCTL_AUDIO_SIF_GETSOUNDMODE                    fpAUDIOSIFGetSoundMode;
    IOCTL_AUDIO_SIF_SETSOUNDMODE                    fpAUDIOSIFSetSoundMode;
    IOCTL_AUDIO_SIF_GETAUDIOSTATUS                  fpAUDIOSIFGetAudioStatus;
    IOCTL_AUDIO_SIF_STARTAUTOSTANDARDDETECTION      fpAUDIOSIFStartAutoStandardDetection;
    IOCTL_AUDIO_SIF_GETRESULTOFAUTOSTANDARDDETECTION fpAUDIOSIFGetResultOfAutoStandardDetection;
    IOCTL_AUDIO_SIF_CONVERTTOBASICAUDIOSTANDARD     fpAUDIOSIFConvertToBasicAudioStandard;
    IOCTL_AUDIO_SIF_SETTHRESHOLD                    fpAUDIOSIFSetThreshold;
    IOCTL_AUDIO_SIF_SETPRESCALE                     fpAUDIOSIFSetPrescale;
    IOCTL_AUDIO_SIF_ISPALTYPE                       fpAUDIOSIFIsPALType;
    IOCTL_AUDIO_SIF_SETPALTYPE                      fpAUDIOSIFSetPALType;
    IOCTL_AUDIO_SIF_SENDCMD                         fpAUDIOSIFSendCmd;
    IOCTL_AUDIO_SIF_SHIFT                           fpAUDIOSIFShift;

#if 1
    //============================================================
    // AUDIO_FM_RADIO RELATIONAL API FUNCTION
    //============================================================
    IOCTL_AUDIO_FM_RADIO_GETSOUNDMODE               fpAUDIOFMRADIOGetSoundMode;
    IOCTL_AUDIO_FM_RADIO_SETSOUNDMODE               fpAUDIOFMRADIOSetSoundMode;
    IOCTL_AUDIO_FM_RADIO_DEEMPHASSISOPTION          fpAUDIOFMRADIODeEmphassisOption;
    IOCTL_AUDIO_FM_RADIO_GET_DC_AMP                 fpAUDIOFMRADIOGetDcAmp;
    IOCTL_AUDIO_FM_RADIO_GET_NSR                    fpAUDIOFMRADIOGetNSR;
    IOCTL_AUDIO_FM_RADIO_RESET                      fpAUDIOFMRADIOReset;
#endif

    //=============================================================
    // AUDIO_SPDIF RELATIONAL API FUNCTION
    //=============================================================
    IOCTL_AUDIO_SPDIF_HWEN                          fpAUDIOSPDIFHWEN;
    IOCTL_AUDIO_SPDIF_SETMUTE                       fpAUDIOSPDIFSetMute;
    IOCTL_AUDIO_SPDIF_SETMODE                       fpAUDIOSPDIFSetMode;
    IOCTL_AUDIO_SPDIF_GETMODE                       fpAUDIOSPDIFGetMode;
    IOCTL_AUDIO_SPDIF_SETSCMS                       fpAUDIOSPDIFSetSCMS;
    IOCTL_AUDIO_SPDIF_GETSCMS                       fpAUDIOSPDIFGetSCMS;
    IOCTL_AUDIO_SPDIF_MONITOR                       fpAUDIOSPDIFMonitor;
    IOCTL_AUDIO_SPDIF_CHANNELSTATUS_CTRL            fpAUDIOSPDIFChannelStatusCTRL;
    IOCTL_AUDIO_DIGITALOUT_SETCHANNELSTATUS         fpAUDIODigitalOut_SetChannelStatus;
    IOCTL_AUDIO_DIGITALOUT_GETCHANNELSTATUS         fpAUDIODigitalOut_GetChannelStatus;
    IOCTL_AUDIO_DIGITALOUT_SETDEVICECAPABILITY      fpAUDIODigitalOut_SetDeviceCapabilty;

    //=============================================================
    // AUDIO_HDMI RELATIONAL API FUNCTION
    //=============================================================
    IOCTL_AUDIO_HDMI_TX_SETMUTE                     fpAUDIOHDMITxSetMute;
    IOCTL_AUDIO_HDMI_TX_GETSTATUS                   fpAUDIOHDMITxGetStatus;
    IOCTL_AUDIO_HDMI_MONITOR                        fpAUDIOHDMIMonitor;
    IOCTL_AUDIO_HDMI_GETNONPCMFLAG                  fpAUDIOHDMIGetNonpcmFlag;
    IOCTL_AUDIO_HDMI_SETNONPCM                      fpAUDIOHDMISetNonpcm;
    IOCTL_AUDIO_HDMI_RX_SETNONPCM                   fpAUDIOHDMIRxSetNonPCM;
    IOCTL_AUDIO_HDMI_RX_GETNONPCM                   fpAUDIOHDMIRxGetNonPCM;
    IOCTL_AUDIO_DTV_HDMI_CFG                        fpAUDIODTVHDMICFG;
    IOCTL_AUDIO_HDMI_GETSYNTHFREG                   fpAUDIOHDMIGetSynthFreg;
    IOCTL_AUDIO_HDMI_SETDOWNSAMPLE                  fpAUDIOHDMISetDownSample;
    IOCTL_AUDIO_HDMI_TX_SETMODE                     fpAUDIOHDMITxSetMode;
    IOCTL_AUDIO_HDMI_RX_GetHDMIINAUDIOSTATUS        fpAUDIOHDMIRxGetHDMIInAUDIOStatus;

    //=============================================================
    // AUDIO_ADVANCE_SOUND RELATIONAL API FUNCTION
    //=============================================================
    IOCTL_AUDIO_ADVSOUND_PROCESSENABLE              fpAUDIOADVSOUNDProcessEnable;
    IOCTL_AUDIO_ADVSOUND_SUBPROCESSENABLE           fpAUDIOADVSOUNDSubProcessEnable;
    IOCTL_AUDIO_ADVSOUND_SETPARAM                   fpAUDIOADVSOUNDSetParam;
    IOCTL_AUDIO_ADVSND_SETPARAM                     fpAUDIOADVSNDSetParam;
    //IOCTL_AUDIO_ADVSOUND_GETPARAM                   fpAUDIOADVSOUNDGetParam;
    IOCTL_AUDIO_ADVSOUND_SETASFPARAM                fpAUDIOADVSOUNDSetASFParam;
    //IOCTL_AUDIO_ADVSOUND_GETASFPARAM                fpAUDIOADVSOUNDGetASFParam;

    IOCTL_AUDIO_ADVSOUND_GETINFO                    fpAUDIOADVSOUNDGetInfo;
    IOCTL_AUDIO_DBXTV_SETMODE                       fpAUDIODBXTV_SetMode;
    IOCTL_AUDIO_SEINIT                              fpAUDIOSeInit;
    IOCTL_AUDIO_SETADVSNDSYS                        fpAUDIOSetAdvSndSys;
    //IOCTL_AUDIO_ADVSND_SET_SOUND_PARAM              fpAUDIOAdvSndSetSoundParam;
    IOCTL_AUDIO_SETVDS                              fpAUDIOSetVDS;
    IOCTL_AUDIO_SETVSPK                             fpAUDIOSetVSPK;
    IOCTL_AUDIO_SETSRS                              fpAUDIOSetSRS;
    IOCTL_AUDIO_SETBBE                              fpAUDIOSetBBE;
    IOCTL_AUDIO_VSPK_WMOD                           fpAUDIOVSPKWMod;
    IOCTL_AUDIO_VSPK_SMOD                           fpAUDIOVSPKSMod;
    IOCTL_AUDIO_SRS_DC                              fpAUDIOSRSDC;
    IOCTL_AUDIO_SRS_TRUBASS                         fpAUDIOSRSTruBass;
    IOCTL_AUDIO_SRS_SETTSXTPARA                     fpAUDIOSRSSetTsxtPara;
    IOCTL_AUDIO_SETSRSTSHD                          fpAUDIOSetSRSTSHD;
    IOCTL_AUDIO_TSHD_TruBass                        fpAUDIOTSHDTruBass;
    IOCTL_AUDIO_TSHD_DEFINITION                     fpAUDIOTSHDDefinition;
    IOCTL_AUDIO_TSHD_CLARITY                        fpAUDIOTSHDClarity;
    IOCTL_AUDIO_SRS_SETTSHDPARA                     fpAUDIOSRSSetTshdPara;
    IOCTL_AUDIO_COPY_PARAMETER                      fpAUDIOCopyParameter;

    //=============================================================
    // AUDIO_KTV RELATIONAL API FUNCTION
    //=============================================================
    IOCTL_AUDIO_SETKTVINFO                          fpAUDIOSetKTVInfo;
    IOCTL_AUDIO_GETKTVINFO                          fpAUDIOGetKTVInfo;
    IOCTL_AUDIO_SETMIXMODEVOLUME                    fpAUDIOSetMixModeVolume;
    IOCTL_AUDIO_SETMIXMODEMUTE                      fpAUDIOSetMixModeMute;
    IOCTL_AUDIO_PLAYMENUSOUND                       fpAUDIOPlayMenuSound;

    //=============================================================
    // AUDIO_MM RELATIONAL API FUNCTION
    //=============================================================
    IOCTL_AUDIO_SETCOMMAUDIOINFO                    fpAUDIOSetCommAudioInfo;
    IOCTL_AUDIO_SETDECODEPARAM                      fpAUDIOSetDecodeParam;
    IOCTL_AUDIO_SETMPEGINFO                         fpAUDIOSetMpegInfo;
    IOCTL_AUDIO_SETAC3INFO                          fpAUDIOSetAC3Info;
    IOCTL_AUDIO_SETAC3PINFO                         fpAUDIOSetAC3PInfo;
    IOCTL_AUDIO_SETAACINFO                          fpAUDIOSetAACInfo;
    IOCTL_AUDIO_SETWMAINFO                          fpAUDIOSetWMAInfo;
    IOCTL_AUDIO_SETDTSCOMMONCTRL                    fpAUDIOSetDTSCommonCtrl;
    IOCTL_AUDIO_GETCOMMAUDIOINFO                    fpAUDIOGetCommAudioInfo;
    IOCTL_AUDIO_GETMPEGINFO                         fpAUDIOGetMpegInfo;
    IOCTL_AUDIO_GETAC3INFO                          fpAUDIOGetAC3Info;
    IOCTL_AUDIO_GETAC3PINFO                         fpAUDIOGetAC3PInfo;
    IOCTL_AUDIO_GETAACINFO                          fpAUDIOGetAACInfo;
    IOCTL_AUDIO_GETWMAINFO                          fpAUDIOGetWmaInfo;
    IOCTL_AUDIO_GETDTSINFO                          fpAUIDOGetDTSInfo;
    IOCTL_AUDIO_XPCM_PARAM                          fpAUDIOXPCMParam;
    IOCTL_AUDIO_XPCM2_PARAM                         fpAUDIOXPCM2Param;
    IOCTL_AUDIO_XPCM2_CHECKINTSTATUS                fpAUDIOXPCM2CheckIntStatus;
    IOCTL_AUDIO_RA8_PARAM                           fpAUDIORA8Param;
    IOCTL_AUDIO_INIT                                fpAUDIOInit;
    IOCTL_AUDIO_STARTDECODE                         fpAUDIOStartDecode;
    IOCTL_AUDIO_STARTBROWSE                         fpAUDIOStartBrower;
    IOCTL_AUDIO_STOPDECODE                          fpAUDIOStopDecode;
    IOCTL_AUDIO_PAUSEDECODE                         fpAUDIOPauseDecode;
    IOCTL_AUDIO_CHECKPLAYDONE                       fpAUDIOCheckPlayDone;
    IOCTL_AUDIO_GETRESIDUALBUFFERSIZE               fpAUDIOGetResidualBufferSize;
    IOCTL_AUDIO_GETPCMBUFFERSIZE                    fpAUDIOGetPCMBufferSize;
    IOCTL_AUDIO_GETPCMBUFFERSIZE2                   fpAUDIOGetPCMBufferSize2;
    IOCTL_AUDIO_GETCURRENTFRAMENUMBER               fpAUDIOGetCurrentFrameNumber;
    IOCTL_AUDIO_GETSAMPLERATE                       fpAUDIOGetSampleRate;
    IOCTL_AUDIO_GETBITRATE                          fpAUDIOGetBitRate;
    IOCTL_AUDIO_GETLAYER                            fpAUDIOGetLayer;
    IOCTL_AUDIO_CHECKINPUTREQUEST                   fpAUDIOCheckInputRequest;
    IOCTL_AUDIO_SETINPUT                            fpAUDIOSetInput;
    IOCTL_AUDIO_SETSAMPLERATEINDEX                  fpAUDIOSetSampleRateIndex;
    IOCTL_AUDIO_SETXPCMPARAM                        fpAUDIOSetXPCMParam;
    IOCTL_AUDIO_FILEENDNOTIFICATION                 fpAUDIOFileEndNotification;
    IOCTL_AUDIO_FILEENDDATAHANDLE                   fpAUDIOFileEndDataHandle;
    IOCTL_AUDIO_GETPLAYTICK                         fpAUDIOGetPlayTick;
    IOCTL_AUDIO_GETESMEMCNT                         fpAUDIOGetEsMEMCnt;
    IOCTL_AUDIO_SETASFPARM                          fpAUDIOSetASFParm;
    IOCTL_AUDIO_MM_SETINPUT                         fpAUDIOMMSetInput;
    IOCTL_AUDIO_MM_CHECKPLAYDONE                    fpAUDIOMMCheckPlayDone;
    IOCTL_AUDIO_MM_CHECKINPUTREQUEST                fpAUDIOMMCheckInputRequest;
    IOCTL_AUDIO_DMAREADER_INIT                      fpAUDIODmaReaderInit;
    IOCTL_AUDIO_DMAREADER_ALLINPUT_INIT             fpAUDIODmaReaderAllInputInit;
    IOCTL_AUDIO_DMAREADER_WRITEPCM                  fpAUDIODmaReaderWritePCM;
    IOCTL_AUDIO_DMAWRITER_INIT                      fpAUDIODmaWriterInit;
    IOCTL_AUDIO_USBPCM_ENABLE                       fpAUDIOUSBPCMEnable;
    IOCTL_AUDIO_USBPCM_SETFLAG                      fpAUDIOUSBPCMSetFlag;
    IOCTL_AUDIO_USBPCM_GETFLAG                      fpAUDIOUSBPCMGetFlag;
    IOCTL_AUDIO_USBPCM_GETMEMINFO                   fpAUDIOUSBPCMGetMemInfo;
    IOCTL_AUDIO_PCMCAPTURE_INIT                     fpAUDIOPCMCaptureInit;
    IOCTL_AUDIO_PCMCAPTURE_START                    fpAUDIOPCMCaptureStart;
    IOCTL_AUDIO_PCMCAPTURE_STOP                     fpAUDIOPCMCaptureStop;
    IOCTL_AUDIO_PCMCAPTURE_READ                     fpAUDIOPCMCaptureRead;
    IOCTL_AUDIO_VOIP_CONFIG                         fpAUDIOVoIPConfig;
    IOCTL_AUDIO_ALSA_CHECK                          fpAUDIOALSACheck;
    IOCTL_AUDIO_ALSA_ENABLE                         fpAUDIOALSAEnable;
    IOCTL_AUDIO_UNI_CHECKDECODEDONE                 fpAUDIOUNICheckDecodeDone;
    IOCTL_AUDIO_UNI_SETOUTPUT                       fpAUDIOUNISetOutput;
    IOCTL_AUDIO_UNI_SET_PCMINPUTWRITEADDR           fpAUDIOUNISetPCMInputWriteAddr;
    IOCTL_AUDIO_UNI_GET_OUTPCMLEVEL                 fpAUDIOUNIGetOutPCMLevel;
    IOCTL_AUDIO_RINGTASK                            fpAUDIORingTask;
    IOCTL_AUDIO_RING_DATATRANSFER                   fpAUDIORingDataTransfer;
    IOCTL_AUDIO_MM2_INITAESINFO                     fpAUDIOMM2InitAesInfo;
    IOCTL_AUDIO_MM2_CHECKAESINFO                    fpAUDIOMM2CheckAesInfo;
    IOCTL_AUDIO_MM2_INPUTAESFINISHED                fpAUDIOMM2InputAesFinished;
    IOCTL_AUDIO_SETAUDIOPARAM2                      fpAUDIOSetAudioParam2;
    IOCTL_AUDIO_GETAUDIOINFO2                       fpAUDIOGetAudioInfo2;
    IOCTL_AUDIO_GETDDRINFO                          fpAUDIOGetDDRInfo;
    IOCTL_AUDIO_GETCAPS                             fpAUDIOGetCaps;
    IOCTL_AUDIO_PCMOPEN                             fpAUDIOPcmOpen;
    IOCTL_AUDIO_PCMCLOSE                            fpAUDIOPcmClose;
    IOCTL_AUDIO_PCMSTART                            fpAUDIOPcmStart;
    IOCTL_AUDIO_PCMSTOP                             fpAUDIOPcmStop;
    IOCTL_AUDIO_PCMSET                              fpAUDIOPcmSet;
    IOCTL_AUDIO_PCMGET                              fpAUDIOPcmGet;
    IOCTL_AUDIO_PCMREAD                             fpAUDIOPcmRead;
    IOCTL_AUDIO_PCMWRITE                            fpAUDIOPcmWrite;
    IOCTL_AUDIO_PCMFLUSH                            fpAUDIOPcmFlush;
    IOCTL_AUDIO_ENCODEROPEN                         fpAUDIOEncoderOpen;
    IOCTL_AUDIO_ENCODERCLOSE                        fpAUDIOEncoderClose;
    IOCTL_AUDIO_ENCODERSTART                        fpAUDIOEncoderStart;
    IOCTL_AUDIO_ENCODERSTOP                         fpAUDIOEncoderStop;
    IOCTL_AUDIO_ENCODERIOCTL                        fpAUDIOEncoderIoctl;
    IOCTL_AUDIO_ENCODEREAD                          fpAUDIOEncoderRead;
    IOCTL_AUDIO_ENCODERFLUSH                        fpAUDIOEncoderFlush;
    IOCTL_AUDIO_DEBUGCMDREAD                        fpAUDIODebugCmdRead;
    IOCTL_AUDIO_DEBUGCMDWRITE                       fpAUDIODebugCmdWrite;
    IOCTL_AUDIO_SYSTEM_CONTROL                      fpAUDIOSystemControl;
    IOCTL_AUDIO_CHANNEL_OPEN                        fpAUDIOChannelOpen;
    IOCTL_AUDIO_CHANNEL_OPEN_BYID                   fpAUDIOChannelOpenByID;
    IOCTL_AUDIO_CHANNEL_CLOSE                       fpAUDIOChannelClose;
    IOCTL_AUDIO_CHANNEL_IOCTL                       fpAUDIOChannelIoctl;
    IOCTL_AUDIO_SOUND_GETPARAM                      fpAUDIOSoundGetParam;
    IOCTL_AUDIO_SOUNDEFFECT_SETPARAM                fpAUDIOSoundEffectSetParam;
}AUDIO_INSTANT_PRIVATE;


typedef struct _AUDIO_RESOURCE_PRIVATE
{
    MS_U32 Dummy;
}AUDIO_RESOURCE_PRIVATE;

void    AUDIORegisterToUtopia(FUtopiaOpen ModuleType);
MS_U32  AUDIOOpen(void **pInstance, void *pAttribute);
MS_U32  AUDIOClose(void *pInstance);
MS_U32  AUDIOIoctl(void *pInstance, MS_U32 u32Cmd, void *pArgs);
MS_U32  AUDIOStr(MS_U32 u32PowerState, void *pModule);
MS_U32 AUDIOMdbIoctl(MS_U32 cmd, const void* const pArgs);


//----------------------
// Utopia 1.0 body API
//----------------------

//============================================================
// AUDIO_SYSTEM RELATIONAL API FUNCTION
//============================================================

void                _MApi_AUDIO_Initialize(void);
void                _MApi_AUDIO_SetPowerOn(MS_BOOL bFlag);
void                _MApi_AUDIO_SetDspBaseAddr(MS_U8 u8Index, MS_U32 u32Bin_Base_Address, MS_U32 u32Mad_Base_Buffer_Adr);
MS_U32              _MApi_AUDIO_GetDspMadBaseAddr(MS_U8 u8Index);
void                _MApi_AUDIO_WriteDecMailBox(MS_U8 u8ParamNum, MS_U16 u16Data);
void                _MApi_AUDIO_TriggerSifPLL(void);
//// **** PATCH GROUP ****////
void                _MDrv_AUDIO_SetDspBaseAddr(MS_U8 u8Index, MS_PHY u32Bin_Base_Address, MS_PHY u32Mad_Base_Buffer_Adr);
MS_PHY              _MDrv_AUDIO_GetDspMadBaseAddr(MS_U8 u8Index);
void                _MDrv_AUDIO_WriteDecMailBox(MS_U8 u8ParamNum, MS_U16 u16Data);
void                _MDrv_AUDIO_TriggerSifPLL(void);
//// ** PATCH GROUP END **////
void                _MApi_Audio_Monitor(void);
void                _MApi_AUDIO_SetSystemInfo(AUDIO_INIT_INFO *pau_info);
//void                _MApi_AUDIO_ASF_Data_To_DDR(MS_BOOL bEnable);
//void                _MApi_AUDIO_R2_DEBUG_MSG(MS_BOOL bEnable);
//void                _MApi_AUDIO_R2_DDR_LATENCY_MSG(MS_BOOL bEnable);

void                _MApi_AUDIO_SetOutConnectivity(void);
void                _MApi_AUDIO_SetPathInfo(AUDIO_PATH_INFO *path_info);
void                _MApi_AUDIO_GetPathInfo(AUDIO_PATH_INFO *path_info);
void                _MApi_AUDIO_SetOutputInfo(AUDIO_OUT_INFO *pout_info);
void                _MApi_AUDIO_GetOutputInfo(AUDIO_OUT_INFO *pout_info);
void                _MApi_AUDIO_SetSourceInfo(AUDIO_SOURCE_INFO_TYPE eSourceType);
void                _MApi_AUDIO_InputSwitch(AUDIO_INPUT_TYPE enSource, AUDIO_SWITCH_GROUP enGroup);
void                _MApi_AUDIO_SetInternalPath(AUDIO_INTERNAL_PATH_TYPE u8Path,  AUDIO_OUTPUT_TYPE u8Output);
//void                _MApi_AUDIO_SetInputScale(int Step);
//void                _MApi_AUDIO_SetScartScale(int Step);
//MS_BOOL             _MApi_AUDIO_Audyssey_VXT_Set_THRESH_LEVEL_TABLE(int *thresh_level_table);
//void                _MApi_AUDIO_SPDIF_SetChannelStatus(SPDIF_CS_MODE_TYPE eType, SPDIF_CS_MODE_VALUE eValue);
//MS_U16              _MApi_AUDIO_SPDIF_GetChannelStatus(SPDIF_CS_MODE_TYPE eType);

MS_BOOL             _MApi_AUDIO_SetDataCaptureSource(AUDIO_DEVICE_TYPE eID, AUDIO_CAPTURE_SOURCE_TYPE eSource);
void                _MApi_AUDIO_SetOutputSourceInfo(OUTPUT_SOURCE_INFO *pSourceInfo);
void                _MApi_AUDIO_GetOutputSourceInfo(OUTPUT_SOURCE_INFO *pSourceInfo);
MS_U16              _MApi_AUDIO_ReadMailBox(MS_BOOL bDspType, MS_U8 u8ParamNum);
void                _MApi_AUDIO_WriteMailBox(MS_BOOL bDspType, MS_U8 u8ParamNum, MS_U16 u16Data);
void                _MApi_AUDIO_FwTriggerDSP(MS_U16 u16Cmd);
void                _MApi_AUDIO_SetFwStatus(MS_BOOL bBootOnDDR);
void                _MApi_AUDIO_ExitAudioSystem(void);
void                _MApi_AUDIO_RebootDsp(MS_BOOL bDspType);
void                _MApi_AUDIO_SendIntrupt(MS_BOOL bDspType,MS_U8 u8Cmd);
void                _MApi_AUDIO_I2S_SetMode(AUDIO_I2S_MODE_TYPE u8Mode, AUDIO_I2S_MODE_VALUE  u8Val);
void                _MApi_AUDIO_WritePreInitTable(void);
void                _MApi_AUDIO_EnaEarphone_LowPower_Stage(void);
void                _MApi_AUDIO_EnaEarphone_HighDriving_Stage(void);

MS_BOOL             _MApi_AUDIO_Suspend(void);
MS_BOOL             _MApi_AUDIO_Resume(void);

//=============================================================
// AUDIO_SOUND RELATIONAL API FUNCTION
//=============================================================
MS_BOOL             _MApi_SND_ProcessEnable(Sound_ENABLE_Type Type, MS_BOOL enable);
MS_BOOL             _MApi_SND_SetParam1(Sound_SET_PARAM_Type Type, MS_U16 param1, MS_U16 param2);
MS_U16              _MApi_SND_GetParam1(Sound_GET_PARAM_Type Type, MS_U16 param1);
MS_BOOL             _MApi_AUDIO_SOUND_SetParam(Sound_SET_Type Type, MS_U32 *p_param);
MS_BOOL             _MApi_AUDIO_SOUND_GetParam(const MS_S8 *pStrCmd, MS_U32 u32StrCmdLength, MS_S8 *pData, MS_U32 u32DataSize);
MS_BOOL             _MApi_AUDIO_SoundEffect_SetParam(const MS_S8 *pStrCmd, MS_U32 u32StrCmdLength, MS_S8 *pData, MS_U32 u32DataSize);
void                _MApi_AUDIO_SetAbsoluteVolume(MS_U8 u8Path, MS_U8 u8Vol1, MS_U8 u8Vol2);
void                _MApi_AUDIO_SetPreScale(MS_U8 u8Path, MS_U8 u8PreScale);
void                _MApi_AUDIO_SetMute(MS_U8 u8Path, MS_BOOL EnMute );
void                _MApi_AUDIO_SourceConnect(AUDIO_OUTPORT_SOURCE_TYPE eConnectSourceType, MS_BOOL bConnect );
void                _MApi_AUDIO_SetBalance(MS_U8 u8Balance);
void                _MApi_AUDIO_EnableEQ(MS_BOOL EnEQ);
void                _MApi_AUDIO_EnableTone(MS_BOOL EnTone);
void                _MApi_AUDIO_EnableAutoVolume(MS_BOOL EnAVC);
void                _MApi_AUDIO_EnableSurround(MS_BOOL EnSurround);
void                _MApi_AUDIO_EnableBalance(MS_BOOL EnBalance);
void                _MApi_AUDIO_SetDynamicBass(MS_U8 u8DynamicBass);
void                _MApi_AUDIO_SetBass(MS_U8 u8Bass);
void                _MApi_AUDIO_AbsoluteBass(MS_U8 u8Bass);
void                _MApi_AUDIO_SetTreble(MS_U8 u8Treble);
void                _MApi_AUDIO_AbsoluteTreble(MS_U8 u8Treble);
void                _MApi_AUDIO_SetEq(MS_U8 u8band, MS_U8 u8level);
void                _MApi_AUDIO_SetEq_7band(MS_U8 u8band, MS_U8 u8level);
void                _MApi_AUDIO_SetAvcThreshold(MS_U8 Level);
void                _MApi_AUDIO_SetAvcMode(MS_U8 AvcMode);
void                _MApi_AUDIO_SetAvcAT(MS_U8 AvcAT);
void                _MApi_AUDIO_SetAvcRT(MS_U8 AvcRT);
void                _MApi_AUDIO_SetBufferProcess(MS_U8 DelayTime);
void                _MApi_AUDIO_SetSurroundXA(MS_U8 mode);
void                _MApi_AUDIO_SetSurroundXB(MS_U8 mode);
void                _MApi_AUDIO_SetSurroundXK( MS_U8 mode);
void                _MApi_AUDIO_SetSurroundLPFGain(MS_U8 mode);
MS_U8               _MApi_AUDIO_ConvertVolumeUnit(MS_U8 UiVolume);
void                _MApi_AUDIO_SetPEQCoef(AUDIO_PEQ_COEF *peq_coef);
void                _MApi_AUDIO_EnablePEQ(MS_BOOL Enpeq);
MS_BOOL             _MApi_AUDIO_EnableDcRemove(MS_BOOL EnDcRemove);

//============================================================
// AUDIO_DTV RELATIONAL API FUNCTION
//============================================================
MS_BOOL             _MApi_AUDIO_ReleaseDecodeSystem(AUDIO_DEC_ID DecId);
MS_BOOL             _MApi_AUDIO_SetDecodeSystem(AUDIO_DEC_ID DecId,  AudioDecStatus_t * p_AudioDecStatus);
MS_BOOL             _MApi_AUDIO_GetDecodeSystem(AUDIO_DEC_ID DecId,  AudioDecStatus_t * p_AudioDecStatus);
MS_BOOL             _MApi_AUDIO_SetDecodeCmd(AUDIO_DEC_ID DecId, En_DVB_decCmdType enDecComamnd);
AUDIO_DEC_ID        _MApi_AUDIO_OpenDecodeSystem(AudioDecStatus_t * p_AudioDecStatus);
MS_BOOL             _MApi_AUDIO_SetSystem(En_DVB_decSystemType enDecSystem);
MS_BOOL             _MApi_AUDIO_GetDecSysSupportStatus(En_DVB_decSystemType enDecSystem);
MS_BOOL             _MApi_AUDIO_IsMadLock(void);
En_DVB_decCmdType   _MApi_AUDIO_GetDecStatus(void);
AU_DVB_DECCMD       _MApi_AUDIO_GetCommand(En_DVB_AudioDecoder AdoDecoderNo);
void                _MApi_AUDIO_SetCommand(En_DVB_decCmdType enDecComamnd);
MS_BOOL             _MApi_AUDIO_GetMAD_LOCK(void);
void                _MApi_AUDIO_SetADOutputMode (MS_U8 out_mode);
void                _MApi_AUDIO_SetADAbsoluteVolume (MS_U8 AbsVolume);
//void                _MApi_AUDIO_SetMainVolumeForAD (MS_U8 u8Vol1, MS_U8 u8Vol2);
void                _MApi_AUDIO_ADSetMute(MS_BOOL EnMute);

//============================================================
// AUDIO_PVR RELATED FUNCTIONS
//============================================================
void                _MApi_AUDIO_SetEncInit(AU_DVB_ENCBR BitRate, MS_U8 u8EncFrameNum);
void                _MApi_AUDIO_SetEncCommand(AU_DVB_ENCCMD enEncComamnd);
void                _MApi_AUDIO_GetEncodeFrameInfo(AU_DVB_ENC_FRAME_INFO *EncFrameInfo);
MS_BOOL             _MApi_AUDIO_GetEncodeDoneFlag(void);

//============================================================
// AUDIO_ATV RELATIONAL API FUNCTION
//============================================================
void                _MApi_AUDIO_SIF_SetStandard(AUDIOSTANDARD_TYPE standard_type);
MS_U8               _MApi_AUDIO_SIF_GetSoundMode(void);
MS_BOOL             _MApi_AUDIO_SIF_SetSoundMode(MS_U8 u8SifSoundMode);
MS_BOOL             _MApi_AUDIO_SIF_GetAudioStatus(AUDIOSTATUS * eAudioStatus);
void                _MApi_AUDIO_SIF_StartAutoStandardDetection(void);
AUDIOSTANDARD_TYPE  _MApi_AUDIO_SIF_GetResultOfAutoStandardDetection(void);
AUDIOSTANDARD_TYPE  _MApi_AUDIO_SIF_ConvertToBasicAudioStandard(AUDIOSTANDARD_TYPE eStandard);
void                _MApi_AUDIO_SIF_SetThreshold(THR_TBL_TYPE *ThrTbl);
void                _MApi_AUDIO_SIF_SetPrescale(MS_U8 gain_type, MS_S32 db_value);
MS_BOOL             _MApi_AUDIO_SIF_IsPALType(AUDIO_SIF_PAL_TYPE pal_type);
void                _MApi_AUDIO_SIF_SetPALType(AUDIO_SIF_PAL_TYPE pal_type);
void                _MApi_AUDIO_SIF_SendCmd(En_AUD_SIF_CmdType enAudSifCommand, MS_U8 comm_arg1, MS_U8 comm_arg2);
void                _MApi_AUDIO_SIF_Shift(En_AUD_VIF_Type type);

#if 1
//============================================================
// AUDIO_FM_RADIO RELATIONAL API FUNCTION
//============================================================
MS_U8               _MApi_AUDIO_FM_RADIO_GetSoundMode(void);
void                _MApi_AUDIO_FM_RADIO_SetSoundMode(MS_U8 u8FMRadioSoundMode);
void                _MApi_AUDIO_FM_RADIO_DeEmphassisOption(MS_BOOL Is_Europe);
MS_S8               _MApi_AUDIO_FM_RADIO_GET_DC_AMP(void);
MS_U8               _MApi_AUDIO_FM_RADIO_GET_NSR(void);
void                _MApi_AUDIO_FM_RADIO_RESET(void);
#endif

//=============================================================
// AUDIO_SPDIF RELATIONAL API FUNCTION
//=============================================================
void                _MApi_AUDIO_SPDIF_HWEN(MS_BOOL spdif_en);
void                _MApi_AUDIO_SPDIF_SetMute(MS_BOOL mute_en);
void                _MApi_AUDIO_SPDIF_SetMode(MS_U8 spdif_mode);
MS_U8               _MApi_AUDIO_SPDIF_GetMode(void);
void                _MApi_AUDIO_SPDIF_SetSCMS(MS_U8 C_bit_en, MS_U8 L_bit_en);
MS_U8               _MApi_AUDIO_SPDIF_GetSCMS(void);
void                _MApi_Audio_SPDIF_Monitor(void);
MS_BOOL             _MApi_AUDIO_SPDIF_ChannelStatus_CTRL(AUDIO_SPDIF_CS_TYPE cs_mode, AUDIO_SPDIF_CS_TYPE_STATUS status);
MS_BOOL             _MApi_AUDIO_DigitalOut_SetChannelStatus(DIGITAL_OUTPUT_TYPE ePath, Digital_Out_Channel_Status_t *stChannelStatus);
MS_BOOL             _MApi_AUDIO_DigitalOut_SetDeviceCapability(DIGITAL_OUTPUT_TYPE eoutType, AUDIO_FORMAT_CODE ecodecType, Digital_Out_Device_Capability_t *p_codecCapability);

//void                _MApi_AUDIO_DigitalOut_GetChannelStatus(DIGITAL_OUTPUT_TYPE ePath, Digital_Out_Channel_Status_t *stChannelStatus);

//=============================================================
// AUDIO_HDMI RELATIONAL API FUNCTION
//=============================================================
MS_BOOL             _MApi_AUDIO_HDMI_RX_GetHdmiInAudioStatus(ST_HDMI_AUDIO_STATUS *p_hdmiAudioSts);
void                _MApi_AUDIO_HDMI_Tx_GetStatus(MS_BOOL *onOff, AUDIO_FS_TYPE *hdmi_SmpFreq, HDMI_TX_OUTPUT_TYPE *outType);
MS_U8               _MApi_AUDIO_HDMI_Monitor(void);
MS_BOOL             _MApi_AUDIO_HDMI_GetNonpcmFlag(void);
void                _MApi_AUDIO_HDMI_SetNonpcm(MS_U8 nonPCM_en);
AUDIO_HDMI_RX_TYPE  _MApi_AUDIO_HDMI_RX_SetNonpcm(MS_U8 nonPCM_en);
AUDIO_HDMI_RX_TYPE  _MApi_AUDIO_HDMI_RX_GetNonPCM(void);
void                _MApi_AUDIO_DTV_HDMI_CFG(MS_U8 ctrl);
MS_U16              _MApi_AUDIO_HDMI_GetSynthFreq(void);
void                _MApi_AUDIO_HDMI_SetDownSample(MS_U8 ratio);
void                _MApi_AUDIO_HDMI_TX_SetMode(HDMI_TX_OUTPUT_TYPE outType);
void                _MApi_AUDIO_HDMI_Tx_SetMute(MS_BOOL mute_en);

//=============================================================
// AUDIO_ADVANCE_SOUND RELATIONAL API FUNCTION
//=============================================================
MS_BOOL             _MApi_AUDIO_ADVSOUND_ProcessEnable(ADVSND_TYPE type);
MS_BOOL             _MApi_AUDIO_ADVSOUND_SubProcessEnable(ADVFUNC proc, MS_BOOL enable);
MS_BOOL             _MApi_AUDIO_ADVSOUND_SetParam(ADVSND_PARAM param, MS_U16 u16value1, MS_U16 u16value2);
MS_BOOL             _MApi_AUDIO_ADVSND_SetParam(ADVSND_PARAM param, MS_U32 u32value1, MS_U16 u16value2);
//MS_U32              _MApi_AUDIO_ADVSOUND_GetParam(ADVSND_GET_PARAM param);
//MS_BOOL             _MApi_AUDIO_ADVSOUND_SetASFParam(ADVSND_PARAM param, MS_S16 s16value1, MS_S16 s16value2);
//MS_U32              _MApi_AUDIO_ADVSOUND_GetASFParam(ADVSND_GET_PARAM param);
MS_U32              _MApi_AUDIO_ADVSOUND_GetInfo(AdvSound_GET_Type Type);
void                _MApi_DBXTV_SetMode(EN_DBXTV_TotSonMode totsonmode, EN_DBXTV_TotVolMode totvolmode, EN_DBXTV_TotSurMode totsurmode,MS_U32 enable);
void                _MApi_AUDIO_SeInit(void);
void                _MApi_AUDIO_SetAdvSndSys(En_DVB_advsndType u8AdvSurrMod);
//void                _MApi_AUDIO_ADVSND_Set_Sound_Param(AdvSound_SET_Type Type, MS_U32 *pParam);
void                _MApi_AUDIO_SetVDS(MS_U8 u8enVDS);
void                _MApi_AUDIO_SetVSPK(MS_U8 u8enVSPK);
void                _MApi_AUDIO_SetSRS(MS_U8 u8enSRS);
void                _MApi_AUDIO_SetBBE(MS_U8 u8enBBE , MS_U8 u8BBEMode);
void                _MApi_AUDIO_VSPK_WMod(MS_U8 u8VSPK_WMode);
void                _MApi_AUDIO_VSPK_SMod(MS_U8 u8VSPK_SMode);
void                _MApi_AUDIO_SRS_DC(MS_U8 u8SRSenDC);
void                _MApi_AUDIO_SRS_TruBass(MS_U8 u8SRSenTruBass);
void                _MApi_AUDIO_SRS_SetTsxtPara(MS_U8 u8mode, MS_U8 u8value);
void                _MApi_AUDIO_SetSRSTSHD(MS_U8 u8enTSHD);
void                _MApi_AUDIO_TSHD_TruBass(MS_U8 u8TSHDenTruBass);
void                _MApi_AUDIO_TSHD_Definition(MS_U8 u8TSHDenDefinition);
void                _MApi_AUDIO_TSHD_Clarity(MS_U8 u8TSHDenClarity);
void                _MApi_AUDIO_SRS_SetTshdPara(MS_U8 u8mode, MS_U8 u8value);
void                _MApi_AUDIO_COPY_Parameter(ADVSND_PARAM type, void* Parameter_ptr, MS_U32 size);

//=============================================================
// AUDIO_KTV RELATIONAL API FUNCTION
//=============================================================
MS_BOOL             _MApi_AUDIO_SetKTVInfo (AUDIO_KTV_MODEL modelType, AUDIO_KTV_PARAMTYPE paramType, MS_U32 param1, MS_U32 param2);
MS_U32              _MApi_AUDIO_GetKTVInfo(AUDIO_KTV_MODEL modelType, AUDIO_KTV_INFOTYPE infoType);
void                _MApi_AUDIO_SetMixModeVolume(AUDIO_SOURCE_INFO_TYPE eSourceType, AUDIO_MIX_VOL_TYPE VolType, MS_U8 u8Vol1, MS_U8 u8Vol2);
void                _MApi_AUDIO_SetMixModeMute(AUDIO_SOURCE_INFO_TYPE eSourceType, AUDIO_MIX_VOL_TYPE VolType, MS_BOOL EnMute);
void                _MApi_AUDIO_PlayMenuSound(void);

//=============================================================
// AUDIO_MM RELATIONAL API FUNCTION
//=============================================================
// Each Decoder functions
// Set Info
MS_BOOL             _MApi_AUDIO_SetCommAudioInfo(Audio_COMM_infoType infoType, MS_VIRT param1, MS_VIRT param2);
//MS_BOOL             _MApi_ADEC_SetDecodeParam( AUDIO_DEC_ID DecId, Audio_ParamType paramType, MS_U32* Param );
MS_BOOL             _MApi_AUDIO_SetMpegInfo( Audio_MPEG_infoType infoType, MS_VIRT param1, MS_VIRT param2 );
MS_BOOL             _MApi_AUDIO_SetAC3Info(Audio_AC3_infoType infoType, MS_VIRT param1, MS_VIRT param2 );
MS_BOOL             _MApi_AUDIO_SetAC3PInfo(Audio_AC3P_infoType infoType, MS_VIRT param1, MS_VIRT param2 );
MS_BOOL             _MApi_AUDIO_SetAACInfo( Audio_AAC_infoType infoType, MS_VIRT param1, MS_VIRT param2 );
MS_BOOL             _MApi_AUDIO_SetWmaInfo(Audio_WMA_infoType infoType, MS_VIRT param1, MS_VIRT param2 );
MS_BOOL             _MApi_AUDIO_SetDTSCommonCtrl( Audio_DTS_infoType infoType, Audio_DTS_CtrlType ctrl_type );
//Get Info
MS_U64             _MApi_AUDIO_GetCommAudioInfo( Audio_COMM_infoType infoType );
MS_U32              _MApi_AUDIO_GetMpegInfo( Audio_MPEG_infoType infoType );
MS_U32              _MApi_AUDIO_GetAC3Info(Audio_AC3_infoType infoType );
MS_U32              _MApi_AUDIO_GetAC3PInfo( Audio_AC3P_infoType infoType );
MS_U32              _MApi_AUDIO_GetAACInfo( Audio_AAC_infoType infoType );
MS_U32              _MApi_AUDIO_GetWmaInfo( Audio_WMA_infoType infoType );
MS_U32              _MApi_AUDIO_GetDTSInfo( Audio_DTS_infoType infoType );
MS_U8               _MApi_AUDIO_XPCM_Param(XPCM_TYPE audioType, MS_U8  channels, MS_U16 sampleRate, MS_U8  bitsPerSample, MS_U16 blockSize, MS_U16 samplePerBlock);
void                _MApi_AUDIO_XPCM2_Param(XPCM_TYPE audioType, MS_U8  channels, MS_U16 sampleRate, MS_U8  bitsPerSample, MS_U16 blockSize, MS_U16 samplePerBlock);
MS_U16              _MApi_AUDIO_XPCM2_CheckIntStatus(void);
// RM
MS_U8               _MApi_AUDIO_RA8_Param(MS_U16 mNumCodecs, MS_U16 mSamples, MS_U16 mSampleRate, MS_U16* Channels, MS_U16* Regions, MS_U16* cplStart, MS_U16* cplQbits, MS_U16* FrameSize);

void                _MApi_AUDIO_Init(En_DVB_decSystemType enDecSystem);
void                _MApi_AUDIO_StartDecode(void);
void                _MApi_AUDIO_StartBrowse(void);
void                _MApi_AUDIO_StopDecode(void);
void                _MApi_AUDIO_PauseDecode(void);
MS_U8               _MApi_AUDIO_CheckPlayDone(void);
MS_U16              _MApi_AUDIO_GetResidualBufferSize(void);
MS_U16              _MApi_AUDIO_GetPCMBufferSize(MS_U16 u16BitRate);
MS_U16              _MApi_AUDIO_GetPCMBufferSize2(MS_U16 u16BitRate,MS_U16 u16SampleRate);
MS_U32              _MApi_AUDIO_GetCurrentFrameNumber(void);
MS_U16              _MApi_AUDIO_GetSampleRate(void);
MS_U32              _MApi_AUDIO_GetBitRate(void);
Audio_MP3_LAYER     _MApi_AUDIO_GetLayer(void);
MS_U8               _MApi_AUDIO_CheckInputRequest(MS_PHY *pPHYWrtAddr, MS_U32 *pU32WrtBytes);
void                _MApi_AUDIO_SetInput(void);
void                _MApi_AUDIO_SetSampleRateIndex(MS_U16 u16Index);
void                _MApi_AUDIO_SetXPCMParam(XPCM_TYPE audioType, MS_U8 channels, MS_U16 sampleRate, MS_U8 bitsPerSample, MS_U16 blockSize, MS_U16 samplePerBlock);
void                _MApi_AUDIO_FileEndNotification(void);
void                _MApi_AUDIO_FileEndDataHandle(MS_U32 u32DataLeft);
MS_U32              _MApi_AUDIO_GetPlayTick(void);
MS_U16              _MApi_AUDIO_GetEsMEMCnt(void);
void                _MApi_AUDIO_SetASFParm(WMA_ASF_PARMTYPE parm_type, MS_U32 value);
void                _MApi_AUDIO_MM_SetInput (En_DVB_AudioDecoder AUDDecoderNo, MS_U8 u8IntTag);
MS_U32              _MApi_AUDIO_MM_CheckPlayDone (En_DVB_AudioDecoder AUDDecoderNo);
MS_U8               _MApi_AUDIO_MM_CheckInputRequest(En_DVB_AudioDecoder AUDDecoderNo, MS_PHY *pPHYWrtAddr, MS_U32 *pU32WrtBytes);
void                _MApi_AUDIO_DmaReader_Init(SAMPLE_RATE sampleRate);
void                _MApi_AUDIO_DmaReader_AllInput_Init(void);
MS_BOOL             _MApi_AUDIO_DmaReader_WritePCM(void* buffer, MS_U32 bytes);
void                _MApi_AUDIO_DmaWriter_Init(void);
void                _MApi_AUDIO_USBPCM_Enable(MS_BOOL bEnable);
void                _MApi_AUDIO_USBPCM_SetFlag(MS_BOOL bEnable);
MS_BOOL             _MApi_AUDIO_USBPCM_GetFlag(void);
void                _MApi_AUDIO_USBPCM_GetMemInfo(AUDIO_UPLOAD_INFO *uploadInfo);
MS_BOOL             _MApi_AUDIO_PCMCapture_Init(const AUDIO_DEVICE_TYPE eID, const AUDIO_CAPTURE_SOURCE_TYPE eSource);
MS_BOOL             _MApi_AUDIO_PCMCapture_Start(const AUDIO_DEVICE_TYPE eID);
MS_BOOL             _MApi_AUDIO_PCMCapture_Stop(const AUDIO_DEVICE_TYPE eID);
MS_BOOL             _MApi_AUDIO_PCMCapture_Read(const AUDIO_DEVICE_TYPE eID, void *buffer, const MS_U32 bytes);
MS_BOOL             _MApi_AUDIO_VoIP_Config(AUDIO_VoIP_CONFIG_TYPE configType, MS_U32 param1, MS_U32 param2);
MS_BOOL             _MApi_AUDIO_ALSA_Check(void);
MS_BOOL             _MApi_AUDIO_ALSA_Enable(MS_BOOL bEnable);
MS_BOOL             _MApi_AUDIO_UNI_CheckDecodeDone(MS_PHY *pPHYWrtAddr, MS_U32 *pU32WrtBytes);
void                _MApi_AUDIO_UNI_SetOutput (MS_U32 PCMOutCnt);
void                _MApi_AUDIO_UNI_Set_PCMInputWriteAddr (MS_U32 PCMIn_Wptr);
MS_U32              _MApi_AUDIO_UNI_Get_OutPCMLevel (void);
void                _MApi_AUDIO_RingTask(audioRingTask* auRingTask);        // this function is used for auto ring address
void                _MApi_AUDIO_Ring_DataTransfer(audioRingTask* auRingTask, audioRingTask* auRingTask_1);
MS_BOOL             _MApi_AUDIO_MM2_initAesInfo( AUDIO_DEC_ID dec_id );
MS_BOOL             _MApi_AUDIO_MM2_checkAesInfo( AUDIO_DEC_ID  dec_id, AES_INFO *aes_info );
MS_BOOL             _MApi_AUDIO_MM2_inputAesFinished( AUDIO_DEC_ID dec_id, MS_U32 es_size, MS_BOOL ptsExist, MS_U64 pts );
MS_BOOL             _MApi_AUDIO_SetAudioParam2( AUDIO_DEC_ID DecId, Audio_ParamType paramType, MS_VIRT Param );
MS_BOOL             _MApi_AUDIO_GetAudioInfo2(AUDIO_DEC_ID DecId, Audio_InfoType infoType, void * pInfo );
MS_PHY              _MApi_AUDIO_GetDDRInfo(AUDIO_DEC_ID DecId, EN_AUDIO_DDRINFO DDRInfo);
MS_U32              _MApi_AUDIO_GetCaps(AUDIO_DEC_ID DecId, MS_U32 *pCaps);
MS_S32              _MApi_AUDIO_PCM_Open(void *pData);
MS_S32              _MApi_AUDIO_PCM_Close(MS_S32 s32DeviceId);
MS_S32              _MApi_AUDIO_PCM_Start(MS_S32 s32DeviceId);
MS_S32              _MApi_AUDIO_PCM_Stop(MS_S32 s32DeviceId);
MS_S32              _MApi_AUDIO_PCM_Set(MS_S32 s32DeviceId, MS_U32 u32Cmd, const void *pData);
MS_S32              _MApi_AUDIO_PCM_Get(MS_S32 s32DeviceId, MS_U32 u32Cmd, void *pData);
MS_U32              _MApi_AUDIO_PCM_Read(MS_S32 s32DeviceId, void *pBuf, MS_U32 u32Size);
MS_U32              _MApi_AUDIO_PCM_Write(MS_S32 s32DeviceId, const void *pBuf, MS_U32 u32Size);
MS_S32              _MApi_AUDIO_PCM_Flush(MS_S32 s32DeviceId);
MS_S32              _MApi_AUDIO_ENCODER_Open(const char *pName);
MS_S32              _MApi_AUDIO_ENCODER_Close(MS_S32 s32DeviceId);
MS_S32              _MApi_AUDIO_ENCODER_Start(MS_S32 s32DeviceId);
MS_S32              _MApi_AUDIO_ENCODER_Stop(MS_S32 s32DeviceId);
MS_S32              _MApi_AUDIO_ENCODER_Ioctl(MS_S32 s32DeviceId, const char *pCmd, void *pData);
MS_U32              _MApi_AUDIO_ENCODER_Read(MS_S32 s32DeviceId, void *pBuf, MS_U32 u32Size);
MS_S32              _MApi_AUDIO_ENCODER_Flush(MS_S32 s32DeviceId);
MS_S32              _MApi_AUDIO_Debug_Cmd_Read(MS_U64 *pHandle, const char *pStr, MS_U32 u32Size);
MS_S32              _MApi_AUDIO_Debug_Cmd_Write(MS_U64 *pHandle, const char *pStr, MS_U32 u32Size);
MS_S32              _MApi_AUDIO_SYSTEM_Control(MS_U8 *pStrCmd, MS_U32 u32StrCmdLength);
MS_S32              _MApi_AUDIO_CHANNEL_Open(void);
MS_S32              _MApi_AUDIO_CHANNEL_Open_ByID(MS_S32 s32ChannelId);
MS_S32              _MApi_AUDIO_CHANNEL_Close(MS_S32 s32ChannelId);
MS_S32              _MApi_AUDIO_CHANNEL_Ioctl(MS_S32 s32ChannelId, const MS_U8 *pStrCmd, MS_U32 u32StrCmdLength, MS_U32 *pData);


#ifdef __cplusplus
}
#endif

#endif //_DMX_PRIV_H_
