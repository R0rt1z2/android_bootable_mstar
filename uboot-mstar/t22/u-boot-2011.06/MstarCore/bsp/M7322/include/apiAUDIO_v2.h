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
//  File name: apiAUDIO_v2.h
//  Description: Audio api header file
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _AUDIO_H_
#define _AUDIO_H_

//#include "MsDevice.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum {
    //  AUDIO_SYSTEM RELATIONAL API FUNCTION                    //*** Args of API  ***
    MApi_CMD_AUDIO_Initialize = 0,                              // void                                                              #0
    MApi_CMD_AUDIO_SetPowerOn,                                  // pointer to (U32_bEnable)
    MApi_CMD_AUDIO_SetDSPBaseAddr,                              // pointer to (MS_U32)
    MApi_CMD_AUDIO_GetDSPBaseAddr,                              // pointer to (MS_U32)
    MApi_CMD_AUDIO_WriteDecMailBox,                             // pointer to (MS_U32)
    MApi_CMD_AUDIO_TriggerSIFPLL,                               // void
    MApi_CMD_AUDIO_Monitor,                                     // void
    MApi_CMD_AUDIO_SetSystemInfo,                               // pointer to (AUDIO_INIT_INFO*)
    MApi_CMD_AUDIO_ASF_Data_To_DDR,                             // pointer to (MS_BOOL)
    MApi_CMD_AUDIO_R2_DEBUG_MSG,                                // pointer to (MS_BOOL)
    MApi_CMD_AUDIO_R2_DDR_LETENCY_MSG,                          // 10 pointer to (MS_BOOL)
    MApi_CMD_AUDIO_SetOutConnectivity,                          // void
    MApi_CMD_AUDIO_SetPathInfo,                                 // pointer to (AUDIO_PATH_INFO*)
    MApi_CMD_AUDIO_GetPathInfo,                                 // pointer to (AUDIO_PATH_INFO*)
    MApi_CMD_AUDIO_SetOutputInfo,                               // pointer to (AUDIO_OUT_INFO*)
    MApi_CMD_AUDIO_GetOutputInfo,                               // pointer to (AUDIO_OUT_INFO*)
    MApi_CMD_AUDIO_SetSourceInfo,                               // pointer to (AUDIO_SOURCE_INFO_TYPE)
    MApi_CMD_AUDIO_InputSwitch,                                 // pointer to (AUDIO_INPUT_TYPE, AUDIO_SWITCH_GROUP)
    MApi_CMD_AUDIO_SetInternalPath,                             // pointer to (AUDIO_INTERNAL_PATH_TYPE, AUDIO_OUTPUT_TYPE)
    MApi_CMD_AUDIO_Set_Input_Scale,                             // pointer to (int)
    MApi_CMD_AUDIO_Set_Scart_Scale,                             // pointer to (int)
    //MApi_CMD_AUDIO_AUDYSSEY_VXT_SET_THRESH_LEVEL_TABLE,       // pointer to (int *)
    MApi_CMD_AUDIO_SPDIF_CHANNELSTATUS,                         // pointer to (SPDIF_CS_MODE_TYPE eType, SPDIF_CS_MODE_VALUE eValue)

    MApi_CMD_AUDIO_SetDataCaptureSource,                        // pointer to (AUDIO_DEVICE_TYPE, AUDIO_CAPTURE_SOURCE_TYPE)    ##20
    //MApi_CMD_AUDIO_SetOutputSourceInfo,                       // pointer to (OUTPUT_SOURCE_INFO *)
    //MApi_CMD_AUDIO_GetOutputSourceInfo,                       // pointer to (OUTPUT_SOURCE_INFO *)
    MApi_CMD_AUDIO_OutputSourceInfo,                            // 20 pointer to (AUDIO_ST_OUTPUT_SOURCE)
    MApi_CMD_AUDIO_ReadMailBox,                                 // pointer to (MS_BOOL, MS_U8)
    MApi_CMD_AUDIO_WriteMailBox,                                // pointer to (MS_BOOL, MS_U8, MS_U16)
    MApi_CMD_AUDIO_FwTriggerDSP,                                // pointer to (MS_U16)
    MApi_CMD_AUDIO_SetFwStatus,                                 // pointer to (MS_BOOL)
    MApi_CMD_AUDIO_ExitAudioSystem,                             // void
    MApi_CMD_AUDIO_RebootDSP,                                   // pointer to (MS_BOOL)
    MApi_CMD_AUDIO_SendIntrupt,                                 // pointer to (MS_BOOL, MS_U8)
    MApi_CMD_AUDIO_I2S_SetMode,                                 // pointer to (AUDIO_I2S_MODE_TYPE, AUDIO_I2S_MODE_VALUE)
    MApi_CMD_AUDIO_WritePreInitTable,                           // void
    MApi_CMD_AUDIO_EnaEarphone_LowPower_Stage,                  // 30 void
    MApi_CMD_AUDIO_EnaEarphone_HighDriving_Stage,               // void
    MApi_CMD_AUDIO_Suspend,                                     // pointer to (void *)
    MApi_CMD_AUDIO_Resume,                                      // pointer to (void *)


    // AUDIO_SOUND RELATIONAL API FUNCTION
    MApi_CMD_SND_ProcessEnable,                                 // pointer to (Sound_ENABLE_Type, MS_BOOL)
    MApi_CMD_SND_SetParam1,                                     // pointer to (Sound_SET_PARAM_Type, MS_U16, MS_U16)
    MApi_CMD_SND_GetParam1,                                     // pointer to (Sound_GET_PARAM_Type, MS_U16)
    MApi_CMD_AUDIO_Sound_SetParam,                              // pointer to (AUDIO_ST_SOUND_SETPARAM*)
    MApi_CMD_AUDIO_SetAbsoluteVolume,                           // pointer to (MS_U8, MS_U8, MS_U8)
    //MApi_CMD_AUDIO_SetPreScale,                                 // pointer to (MS_U8, MS_U8)
    //MApi_CMD_AUDIO_SetMute,                                     // pointer to (MS_U8, MS_BOOL)
    MApi_CMD_AUDIO_SourceConnect,                               // pointer to (AUDIO_OUTPORT_SOURCE_TYPE, MS_BOOL)
    //MApi_CMD_AUDIO_SetBalance,                                  // pointer to (MS_U8)
    //MApi_CMD_AUDIO_EnableEQ,                                    // pointer to (MS_BOOL)
    //MApi_CMD_AUDIO_EnableTone,                                  // pointer to (MS_BOOL)
    //MApi_CMD_AUDIO_EnableAutoVolume,                            // pointer to (MS_BOOL)
    //MApi_CMD_AUDIO_EnableSurround,                              // pointer to (MS_BOOL)
    //MApi_CMD_AUDIO_EnableBalance,                               // pointer to (MS_BOOL)
    MApi_CMD_AUDIO_SetDynamicBass,                              // 40 pointer to (MS_U8)
    //MApi_CMD_AUDIO_SetBass,                                     // pointer to (MS_U8)
    //MApi_CMD_AUDIO_AbsoluteBass,                                // pointer to (MS_U8)
    //MApi_CMD_AUDIO_SetTreble,                                   // pointer to (MS_U8)
    MApi_CMD_AUDIO_AbsoluteTreble,                              // pointer to (MS_U8)                                                ##40
    //MApi_CMD_AUDIO_SetEq,                                       // pointer to (MS_U8, MS_U8)
    MApi_CMD_AUDIO_SetEq_7band,                                 // pointer to (MS_U8, MS_U8)
    MApi_CMD_AUDIO_SetAvcThreshold,                             // pointer to (MS_U8)
    MApi_CMD_AUDIO_SetAvcMode,                                  // pointer to (MS_U8)
    MApi_CMD_AUDIO_SetAvcAT,                                    // pointer to (MS_U8)
    MApi_CMD_AUDIO_SetAvcRT,                                    // pointer to (MS_U8)
    MApi_CMD_AUDIO_SetBufferProcess,                            // pointer to (MS_U8)
    MApi_CMD_AUDIO_SetSurroundXA,                               // pointer to (MS_U8)
    MApi_CMD_AUDIO_SetSurroundXB,                               // pointer to (MS_U8)
    MApi_CMD_AUDIO_SetSurroundXK,                               // 50 pointer to (MS_U8)
    MApi_CMD_AUDIO_SetSurroundLPFGain,                           // pointer to (MS_U8)
    MApi_CMD_AUDIO_ConvertVolumeUnit,                           // pointer to (MS_U8)
    MApi_CMD_AUDIO_SetPEQCoef,                                  // pointer to (AUDIO_PEQ_COEF*)
    //MApi_CMD_AUDIO_EnablePEQ,                                   // pointer to (MS_U8)
    MApi_CMD_AUDIO_EnableDcRemove,                              // pointer to (MS_U8)


    // AUDIO_DTV RELATIONAL API FUNCTION
    MApi_CMD_AUDIO_DecodeSystem,                                // pointer to (AUDIO_ST_SET_DECODESYSTEM)
    MApi_CMD_AUDIO_ReleaseDecodeSystem,                         // pointer to (AUDIO_ST_RELEASE_DECODESYSTEM*)
    MApi_CMD_AUDIO_SetDecodeSystem,                             // pointer to (AUDIO_ST_SET_DECODESYSTEM*)
    MApi_CMD_AUDIO_GetDecodeSystem,                             // pointer to (AUDIO_ST_GET_DECODESYSTEM*)
    MApi_CMD_AUDIO_SetDecodeCmd,                                // pointer to (AUDIO_DEC_ID, En_DVB_decCmdType)
    MApi_CMD_AUDIO_OpenDecodeSystem,                            // 60 pointer to (AudioDecStatus_t*)
    MApi_CMD_AUDIO_SetSystem,                                   // pointer to (En_DVB_decSystemType)
    MApi_CMD_AUDIO_GetDecSysSupportStatus,                      // pointer to (En_DVB_decSystemType)                                ##60
    MApi_CMD_AUDIO_IsMadLock,                                   // void
    MApi_CMD_AUDIO_GetDecStatus,                                // void
    MApi_CMD_AUDIO_Decode_Command,                              // pointer to (AUDIO_ST_DECODER_COMMAND)
    //MApi_CMD_AUDIO_GetCommand,                                  // pointer to (En_DVB_AudioDecoder)
    //MApi_CMD_AUDIO_SetCommand,                                  // pointer to (En_DVB_decCmdType)
    MApi_CMD_AUDIO_GetMAD_Lock,                                 // void
    MApi_CMD_AUDIO_SETAD,                                       // pointer to (AUDIO_ST_SET_AD)
    //MApi_CMD_AUDIO_SetADOutputMode,                             // pointer to (MS_U8)
    //MApi_CMD_AUDIO_SetADAbsoluteVolume,                         // pointer to (MS_U8)
    //MApi_CMD_AUDIO_ADSetMute,                                   // pointer to (MS_U8)

    // AUDIO_PVR RELATED FUNCTIONS
    MApi_CMD_AUDIO_SetEncInit,                                  // pointer to (AU_DVB_ENCBR, MS_U8)
    MApi_CMD_AUDIO_SetEncCommand,                               // pointer to (AU_DVB_ENCCMD)
    MApi_CMD_AUDIO_GetEncodeFrameInfo,                          // 70 pointer to (AU_DVB_ENC_FRAME_INFO*)
    MApi_CMD_AUDIO_GetEncodeDoneFlag,                           // void

    // AUDIO_ATV RELATIONAL API FUNCTION
    MApi_CMD_AUDIO_SIF_SetStandard,                             // pointer to (AUDIOSTANDARD_TYPE)
    MApi_CMD_AUDIO_SIF_GetSoundMode,                            // void
    MApi_CMD_AUDIO_SIF_SetSoundMode,                            // pointer to (MS_U8)
    MApi_CMD_AUDIO_SIF_GetAudioStatus,                          // pointer to (AUDIO_ST_SIF_GET_AUDIO_STATUS)
    MApi_CMD_AUDIO_SIF_StartAutoStandardDetection,              // void
    MApi_CMD_AUDIO_SIF_GetResultOfAutoStandardDetection,        // void
    MApi_CMD_AUDIO_SIF_ConvertToBasicAudioStandard,             // pointer to (AUDIOSTANDARD_TYPE)
    MApi_CMD_AUDIO_SIF_SetThreshold,                            // pointer to (THR_TBL_TYPE code*)
    MApi_CMD_AUDIO_SIF_SetPrescale,                             // 80 pointer to (MS_U8, int)
    MApi_CMD_AUDIO_SIF_IsPALType,                               // pointer to (AUDIO_SIF_PAL_TYPE)
    MApi_CMD_AUDIO_SIF_SetPALType,                              // pointer to (AUDIO_SIF_PAL_TYPE)                                  ##80
    MApi_CMD_AUDIO_SIF_SendCmd,                                 // pointer to (En_AUD_SIF_CmdType, MS_U8, MS_U8)
    MApi_CMD_AUDIO_SIF_Shift,                                   // pointer to (En_AUD_VIF_Type)

#if 1
    // AUDIO_FM_RADIO RELATIONAL API FUNCTION
    MApi_CMD_AUDIO_FM_RADIO_GetSoundMode,                       // void                                                             #90
    MApi_CMD_AUDIO_FM_RADIO_SetSoundMode,                       // pointer to (MS_U8)
    MApi_CMD_AUDIO_FM_RADIO_DeEmphassisOption,                  // pointer to (MS_BOOL)
    MApi_CMD_AUDIO_FM_RADIO_GetDC_Amp,                          // void
    MApi_CMD_AUDIO_FM_RADIO_Get_NSR,                            // void
    MApi_CMD_AUDIO_FM_RADIO_Reset,                              // 90 void
#endif

    // AUDIO_SPDIF RELATIONAL API FUNCTION
    MApi_CMD_AUDIO_SPDIF_OP,                                     // pointer to (AUDIO_ST_SPDIF_OP)
    //MApi_CMD_AUDIO_SPDIF_HWEN,                                  // pointer to (MS_BOOL)
    //MApi_CMD_AUDIO_SPDIF_SetMute,                               // pointer to (MS_BOOL)
    //MApi_CMD_AUDIO_SPDIF_SetMode,                               // pointer to (MS_BOOL)
    //MApi_CMD_AUDIO_SPDIF_GetMode,                               // void
    MApi_CMD_AUDIO_SPDIF_SCMS,                                  // pointer to (AUDIO_ST_SPDIF_SCMS)
    //MApi_CMD_AUDIO_SPDIF_SetSCMS,                               // pointer to (MS_U8, MS_U8)
    //MApi_CMD_AUDIO_SPDIF_GetSCMS,                               // void
    MApi_CMD_AUDIO_SPDIF_Monitor,                               // void
    MApi_CMD_AUDIO_SPDIF_ChannelStatus_CTRL,                    // pointer to (AUDIO_SPDIF_CS_TYPE, AUDIO_SPDIF_CS_TYPE_STATUS)
    MApi_CMD_AUDIO_DigitalOut_SetChannelStatus,
    MApi_CMD_AUDIO_DigitalOut_SetDeviceCapability,              // pointer to (DIGITAL_OUTPUT_TYPE, AUDIO_FORMAT_CODE, Digital_Out_Device_Capability_t*)

    // AUDIO_HDMI RELATIONAL API FUNCTION
    MApi_CMD_AUDIO_HDMI_Tx_SetMute,                             // pointer to (MS_BOOL)
    MApi_CMD_AUDIO_HDMI_Tx_GetStatus,                           // pointer to (MS_BOOL*, AUDIO_FS_TYPE*, HDMI_TX_OUTPUT_TYPE*)
    MApi_CMD_AUDIO_HDMI_Monitor,                                // void
    MApi_CMD_AUDIO_HDMI_GetNonpcmFlag,                          // 100 void
    MApi_CMD_AUDIO_HDMI_SetNonpcm,                              // pointer to (MS_BOOL)
    MApi_CMD_AUDIO_HDMI_Nonpcm_OP,                              // pointer to (AUDIO_ST_HDMI_NONPCM_OP)
    //MApi_CMD_AUDIO_HDMI_Rx_SetNonPcm,                           // pointer to (MS_U8)
    //MApi_CMD_AUDIO_HDMI_RX_GetNonpcm,                           // void
    MApi_CMD_AUDIO_DTV_HDMI_CFG,                                // pointer to (MS_U8)
    MApi_CMD_AUDIO_HDMI_GetSynthFreq,                           // void
    MApi_CMD_AUDIO_HDMI_SetDownSample,                          // pointer to (MS_U8)
    MApi_CMD_AUDIO_HDMI_TX_SetMode,                             // pointer to (HDMI_TX_OUTPUT_TYPE)
    MApi_CMD_AUDIO_HDMI_RX_GetHdmiInAudioStatus,                // pointer to (AUDIO_ST_RX_HDMI_AUDIO_STATUS*)

    // AUDIO_ADVANCE_SOUND RELATIONAL API FUNCTION
    MApi_CMD_AUDIO_ADVSOUND_ProcessEnable,                      // pointer to (ADVSND_TYPE)
    MApi_CMD_AUDIO_ADVSOUND_SubProcessEnable,                   // pointer to (ADVFUNC, MS_BOOL)                                    ##100
    MApi_CMD_AUDIO_ADVSOUND_SetParam,                           // 110 pointer to (ADVSND_PARAM, MS_U16, MS_U16)
    MApi_CMD_AUDIO_ADVSND_SetParam,                             // pointer to (ADVSND_PARAM, MS_U32, MS_U16)
    //MApi_CMD_AUDIO_ADVSOUND_GetParam,                           // pointer to (ADVSND_GET_PARAM)
    MApi_CMD_AUDIO_ADVSOUND_SET_ASF_PARAM,                      // pointer to (ADVSND_PARAM, MS_S16, MS_S16)
    //MApi_CMD_AUDIO_ADVSOUND_GET_ASF_PARAM,                      // pointer to (ADVSND_GET_PARAM)
    MApi_CMD_AUDIO_ADVSOUND_GetInfo,                            // pointer to (AdvSound_GET_Type)
    MApi_CMD_AUDIO_DBXTV_SetMode,                               // pointer to (EN_DBXTV_TotSonMode, EN_DBXTV_TotVolMode , EN_DBXTV_TotSurMode,MS_U32)
    MApi_CMD_AUDIO_SeInit,                                      // void
    MApi_CMD_AUDIO_SetAdvSndSys,                                // pointer to (MS_U8)
    //MApi_CMD_AUDIO_ADV_SOUND_SET,                               // pointer to (AUDIO_ST_HDMI_NONPCM_OP)
    MApi_CMD_AUDIO_SetVDS,                                      // pointer to (MS_U8)
    MApi_CMD_AUDIO_SetVSPK,                                     // pointer to (MS_U8)
    MApi_CMD_AUDIO_SetSRS,                                      // pointer to (MS_U8)
    MApi_CMD_AUDIO_SetBBE,                                      // 120 pointer to (MS_U8, MS_U8)
    MApi_CMD_AUDIO_VSPK_WMod,                                   // pointer to (MS_U8)
    MApi_CMD_AUDIO_VSPK_SMod,                                   // pointer to (MS_U8)
    MApi_CMD_AUDIO_SRS_DC,                                      // pointer to (MS_U8)
    MApi_CMD_AUDIO_SRS_TruBass,                                 // pointer to (MS_U8)
    MApi_CMD_AUDIO_SRSTsxtPara,                                 // pointer to (MS_U8, MS_U8)
    MApi_CMD_AUDIO_SetSRSTSHD,                                  // pointer to (MS_U8)
    MApi_CMD_AUDIO_TSHD_TruBass,                                // pointer to (MS_U8)
    MApi_CMD_AUDIO_TSHD_Definition,                             // pointer to (MS_U8)
    MApi_CMD_AUDIO_TSHD_Clarity,                                // pointer to (MS_U8)
    MApi_CMD_AUDIO_SRSSetTshdPara,                              // 130 pointer to (MS_U8, MS_U8)
    MApi_CMD_AUDIO_Copy_Parameter,                              // pointer to (AUDIO_ST_COPY_PARAMETER*)

    // AUDIO_KTV RELATIONAL API FUNCTION
    MApi_CMD_AUDIO_KTV_OP,                                      // pointer to ()
    //MApi_CMD_AUDIO_SetKTVInfo,                                  // pointer to (AUDIO_KTV_MODEL, AUDIO_KTV_PARAMTYPE, MS_U32, MS_U32)
    //MApi_CMD_AUDIO_GetKTVInfo,                                  // pointer to (AUDIO_KTV_MODEL, AUDIO_KTV_INFOTYPE)
    //MApi_CMD_AUDIO_SetMixModeVolume,                            // pointer to (AUDIO_SOURCE_INFO_TYPE, AUDIO_MIX_VOL_TYPE, MS_U8, MS_U8)
    //MApi_CMD_AUDIO_SetMixModeMute,                              // pointer to (AUDIO_SOURCE_INFO_TYPE, AUDIO_MIX_VOL_TYPE, MS_BOOL)
    MApi_CMD_AUDIO_PlayMenuSound,                               // void

    // AUDIO_MM RELATIONAL API FUNCTION
    MApi_CMD_AUDIO_SetCommAudioInfo,                            // pointer to (Audio_COMM_infoType, MS_U32, MS_U32)
    MApi_CMD_AUDIO_SetDecodeParam,                              // pointer to (AUDIO_ST_SETDECODEPARAM)
    MApi_CMD_AUDIO_SetMpegInfo,                                 // pointer to (Audio_MPEG_infoType, MS_U32, MS_U32)
    MApi_CMD_AUDIO_SetAC3Info,                                  // pointer to (Audio_AC3_infoType, MS_U32, MS_U32)
    MApi_CMD_AUDIO_SetAC3PInfo,                                 // pointer to (Audio_AC3P_infoType, MS_U32, MS_U32)
    MApi_CMD_AUDIO_SetAACInfo,                                  // pointer to (Audio_AAC_infoType, MS_U32, MS_U32)
    MApi_CMD_AUDIO_SetWMAInfo,                                  // 140 pointer to (Audio_WMA_infoType, MS_U32, MS_U32)
    MApi_CMD_AUDIO_SetDTSCommonCtrl,                            // pointer to (Audio_DTS_infoType , Audio_DTS_CtrlType )
    MApi_CMD_AUDIO_GetCommAudioInfo,                            // pointer to (Audio_COMM_infoType)                                 ##120
    MApi_CMD_AUDIO_GetMpegInfo,                                 // pointer to (Audio_MPEG_infoType)
    MApi_CMD_AUDIO_GetAC3Info,                                  // pointer to (Audio_AC3P_infoType)
    MApi_CMD_AUDIO_GetAC3PInfo,                                 // pointer to (Audio_AC3P_infoType)
    MApi_CMD_AUDIO_GetAACInfo,                                  // pointer to (Audio_AAC_infoType)
    MApi_CMD_AUDIO_GetWmaInfo,                                  // pointer to (Audio_WMA_infoType)
    MApi_CMD_AUDIO_GetDTSInfo,                                  // pointer to (Audio_DTS_infoType)
    MApi_CMD_AUDIO_XPCM_Param,                                  // pointer to (XPCM_TYPE, MS_U8, MS_U16, MS_U8 , MS_U16, MS_U16)
    MApi_CMD_AUDIO_XPCM2_Param,                                 // 150 pointer to (XPCM_TYPE, MS_U8, MS_U16, MS_U8 , MS_U16, MS_U16)
    MApi_CMD_AUDIO_XPCM2_CheckIntStatus,                        // void
    MApi_CMD_AUDIO_RA8_Param,                                   // pointer to (AUDIO_ST_RA8_Param)
    MApi_CMD_AUDIO_Init,                                        // pointer to (En_DVB_decSystemType)
    MApi_CMD_AUDIO_StartDecode,                                 // void
    MApi_CMD_AUDIO_StartBroswe,                                 // void
    MApi_CMD_AUDIO_StopDecode,                                  // void
    MApi_CMD_AUDIO_PauseDecode,                                 // void
    MApi_CMD_AUDIO_CheckPlayDone,                               // void
    MApi_CMD_AUDIO_GetResidualBufferSize,                       // void
    MApi_CMD_AUDIO_GetPCMBufferSize,                            // 160 pointer to (MS_U16)
    MApi_CMD_AUDIO_GetPCMBufferSize2,                           // pointer to (MS_U16, MS_U16)
    MApi_CMD_AUDIO_GetCurrentFrameNumber,                       // void
    MApi_CMD_AUDIO_GetSampleRates,                              // void
    MApi_CMD_AUDIO_GetBitRates,                                 // void                                                             ##140
    MApi_CMD_AUDIO_GetLayer,                                    // void
    MApi_CMD_AUDIO_CheckInputRequest,                           // pointer to (AUDIO_ST_CHECK_INPUT_REQ)
    MApi_CMD_AUDIO_SetInput,                                    // void
    MApi_CMD_AUDIO_SetSampleRateIndex,                          // pointer to (MS_U16)
    MApi_CMD_AUDIO_SetXPCMParam,                                // pointer to (XPCM_TYPE, MS_U8, MS_U16, MS_U8, MS_U16, MS_U16)
    MApi_CMD_AUDIO_FileEndNotification,                         // 170 void
    MApi_CMD_AUDIO_FileEndDataHandle,                           // pointer to (MS_U32)
    MApi_CMD_AUDIO_GetPlayTick,                                 // void
    MApi_CMD_AUDIO_GetEsMEMCnt,                                 // void
    MApi_CMD_AUDIO_SetASFParm,                                  // pointer to (WMA_ASF_PARMTYPE, MS_U32)
    MApi_CMD_AUDIO_MM_SetInput,                                 // pointer to (En_DVB_AudioDecoder, MS_U8)
    MApi_CMD_AUDIO_MM_CheckPlayDone,                            // pointer to (En_DVB_AudioDecoder)
    MApi_CMD_AUDIO_MM_CheckInputRequest,                        // pointer to (AUDIO_ST_MM_CHECK_INPUT_REQ)
    MApi_CMD_AUDIO_DmaReader_Init,                              // pointer to (SAMPLE_RATE)
    MApi_CMD_AUDIO_DmaReader_AllInput_Init,                     // void
    MApi_CMD_AUDIO_DmaReader_WritePCM,                          // 180 pointer to (AUDIO_ST_DMAREADER_WRITEPCM)
    MApi_CMD_AUDIO_DmaWrite_Init,                               // void
    MApi_CMD_AUDIO_USBPCM_Enable,                               // pointer to (MS_BOOL)
    MApi_CMD_AUDIO_USBPCM_SetFlag,                              // pointer to (MS_BOOL)
    MApi_CMD_AUDIO_USBPCM_GetFlag,                              // void
    MApi_CMD_AUDIO_USBPCM_GetMemInfo,                           // pointer to (AUDIO_UPLOAD_INFO*)
    MApi_CMD_AUDIO_PCMCapture_Init,                             // pointer to (const AUDIO_DEVICE_TYPE , const AUDIO_CAPTURE_SOURCE_TYPE)
    MApi_CMD_AUDIO_PCMCapture_Start,                            // pointer to (cAUDIO_ST_PCMCAPTURE_INIT)
    MApi_CMD_AUDIO_PCMCapture_Stop,                             // pointer to (AUDIO_ST_PCMCAPTURE_INIT)
    MApi_CMD_AUDIO_PCMCapture_Read,                             // pointer to (AUDIO_ST_PCMCAPTURE_READ)
    MApi_CMD_AUDIO_VoIP_Config,                                 // 190 pointer to (AUDIO_VoIP_CONFIG_TYPE, MS_U32, MS_U32)
    MApi_CMD_AUDIO_ALSA_Check,                                  // void
    MApi_CMD_AUDIO_ALSA_Enable,                                 // pointer to (MS_BOOL)
    MApi_CMD_AUDIO_UNI_CheckDecodeDone,                         // pointer to (MS_U32*, MS_U32*)
    MApi_CMD_AUDIO_UNI_SetOutput,                               // pointer to (MS_U32)
    MApi_CMD_AUDIO_UNI_Set_PCMInputWriteAddr,                   // pointer to (MS_U32)
    MApi_CMD_AUDIO_UNI_Get_OutPCMLevel,                         // void
    MApi_CMD_AUDIO_RingTask,                                    // pointer to (audioRingTask*)
    MApi_CMD_AUDIO_Ring_DataTransfer,                           // pointer to (audioRingTask*, audioRingTask*)
    MApi_CMD_AUDIO_MM2_InitAesInfo,                             // pointer to (AUDIO_DEC_ID)
    MApi_CMD_AUDIO_MM2_CheckAesInfo,                            // 200 pointer to (ADUIO_DEC_ID, AES_INFO*)
    MApi_CMD_AUDIO_MM2_InputAesFinished,                        // pointer to (AUDIO_DEC_ID , MS_U32 , MS_BOOL , MS_U64  )
    MApi_CMD_AUDIO_SetAudioParam2,                              // pointer to (AUDIO_DEC_ID , Audio_ParamType, MS_U32 )
    MApi_CMD_AUDIO_GetAudioInfo2,                               // pointer to (AUDIO_DEC_ID, Audio_InfoType, void* )
    MApi_CMD_AUDIO_GetDDRInfo,                                  // pointer to (AUDIO_DEC_ID , EN_AUDIO_DDRINFO)
    MApi_CMD_AUDIO_DigitalOut_GetChannelStatus,                   //
    MApi_CMD_AUDIO_GetCaps,
    MApi_CMD_AUDIO_PcmOpen,
    MApi_CMD_AUDIO_PcmClose,
    MApi_CMD_AUDIO_PcmStart,
    MApi_CMD_AUDIO_PcmStop,
    MApi_CMD_AUDIO_PcmSet,
    MApi_CMD_AUDIO_PcmGet,
    MApi_CMD_AUDIO_PcmRead,
    MApi_CMD_AUDIO_PcmWrite,
    MApi_CMD_AUDIO_PcmFlush,
    MApi_CMD_AUDIO_EncoderOpen,
    MApi_CMD_AUDIO_EncoderClose,
    MApi_CMD_AUDIO_EncoderStart,
    MApi_CMD_AUDIO_EncoderStop,
    MApi_CMD_AUDIO_EncoderIoctl,
    MApi_CMD_AUDIO_EncoderRead,
    MApi_CMD_AUDIO_EncoderFlush,
    MApi_CMD_AUDIO_DebugCmdRead,
    MApi_CMD_AUDIO_DebugCmdWrite,
    MApi_CMD_AUDIO_SystemControl,
    MApi_CMD_AUDIO_ChannelOpen,
    MApi_CMD_AUDIO_ChannelOpenByID,
    MApi_CMD_AUDIO_ChannelClose,
    MApi_CMD_AUDIO_ChannelIoctl,
    MApi_CMD_AUDIO_Sound_GetParam,
    MApi_CMD_AUDIO_SoundEffect_SetParam,
} eAudioIoctlOpt;

//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
// Type and Structure Declaration
//-------------------------------------------------------------------------------------------------
/// Audio status indicator
typedef enum
{
    AUDIO_STATUS_OK,               ///< Audio status OK
    AUDIO_STATUS_ERROR,            ///< Audio status ERROR
} AUDIO_STATUS;

typedef enum
{
    DECODE_SYS_RELEASE,
    DECODE_SYS_OPEN,
    DECODE_SYS_SET,
    DECODE_SYS_GET,
}AUDIO_DECODE_TYPE;

typedef enum
{
    AD_OUTPUT_MODE,
    AD_ABSOLUTE_VOLUME,
    AD_SET_MUTE,
    AD_SET_MAIN_VOL,
}AUDIO_SET_AD_TYPE;

typedef enum
{
    SPDIF_HWEN,
    SPDIF_SETMUTE,
    SPDIF_SETMODE,
    SPDIF_GETMODE,
}AUDIO_SPDIF_OP;

typedef enum
{
    KTV_SET_INFO,
    KTV_GET_INFO,
    KTV_SET_MIX_VOL,
    KTV_SET_MIX_MUTE,
}AUDIO_KTV_OP;

typedef struct __attribute__((packed))
{
    MS_BOOL                     bStatus;
    ST_HDMI_AUDIO_STATUS*       pHdmi_status;
}AUDIO_ST_RX_HDMI_AUDIO_STATUS, *PAUDIO_ST_RX_HDMI_AUDIO_STATUS;

typedef struct __attribute__((packed))
{
    MS_BOOL                     bStatus;
    Sound_SET_Type              eType;
    MS_U32*                     pParam;
}AUDIO_ST_SOUND_SETPARAM, *PAUDIO_ST_SOUND_SETPARAM;

typedef struct __attribute__((packed))
{
    MS_BOOL                     bStatus;
    AUDIO_DECODE_TYPE           eType;
    AUDIO_DEC_ID                DecId;
    AudioDecStatus_t*           p_AudioDecStatus;
}AUDIO_ST_DECODESYSTEM, *PAUDIO_ST_DECODESYSTEM;

typedef struct __attribute__((packed))
{
    MS_BOOL                     bStatus;
    MS_S8*                      pStrCmd;
    MS_U32                      u32StrCmdLength;
    MS_S8*                      pData;
    MS_U32                      u32DataSize;
}AUDIO_ST_SOUND_GETPARAM, *PAUDIO_ST_SOUND_GETPARAM;

typedef struct __attribute__((packed))
{
    MS_BOOL                     bStatus;
    MS_S8*                      pStrCmd;
    MS_U32                      u32StrCmdLength;
    MS_S8*                      pData;
    MS_U32                      u32DataSize;
}AUDIO_ST_SOUNDEFFECT_SETPARAM, *PAUDIO_ST_SOUNDEFFECT_SETPARAM;

typedef struct __attribute__((packed))
{
    AUDIO_DEC_ID                DecId;
    AudioDecStatus_t*           p_AudioDecStatus;
}AUDIO_ST_OPEN_DECODESYSTEM,  *PAUDIO_ST_OPEN_DECODESYSTEM;

typedef struct __attribute__((packed))
{
    MS_BOOL                     bStatus;
    AUDIO_DEC_ID                DecId;
}AUDIO_ST_RELEASE_DECODESYSTEM, *PAUDIO_ST_RELEASE_DECODESYSTEM;

typedef struct __attribute__((packed))
{
    MS_BOOL                     bStatus;
    AUDIO_DEC_ID                DecId;
    AudioDecStatus_t*           p_AudioDecStatus;
}AUDIO_ST_GET_DECODESYSTEM, *PAUDIO_ST_GET_DECODESYSTEM;

typedef struct __attribute__((packed))
{
    MS_BOOL                     bStatus;
    AUDIO_DEC_ID                DecId;
    AudioDecStatus_t*           p_AudioDecStatus;
}AUDIO_ST_SET_DECODESYSTEM, *PAUDIO_ST_SET_DECODESYSTEM;

typedef struct __attribute__((packed))
{
    MS_BOOL                     bStatus;
    AUDIOSTATUS*                p_eAudioStatus;
}AUDIO_ST_SIF_GET_AUDIO_STATUS, *PAUDIO_ST_SIF_GET_AUDIO_STATUS;

typedef struct __attribute__((packed))
{
    MS_BOOL                         bStatus;
    DIGITAL_OUTPUT_TYPE             ePath;
    Digital_Out_Channel_Status_t*   stChannelStatus;
}AUDIO_ST_DIGITALOUT_SETCHANNELSTATUS, *PAUDIO_ST_DIGITALOUT_SETCHANNELSTATUS;

typedef struct __attribute__((packed))
{
    MS_BOOL                         bStatus;
    DIGITAL_OUTPUT_TYPE             eoutType;
    AUDIO_FORMAT_CODE               ecodecType;
    Digital_Out_Device_Capability_t* p_codecCapability;
}AUDIO_ST_DIGITALOUT_SETDEVICECAP, *PAUDIO_ST_DIGITALOUT_SETDEVICECAP;

typedef struct __attribute__((packed))
{
    DIGITAL_OUTPUT_TYPE             ePath;
    Digital_Out_Channel_Status_t*   stChannelStatus;
}AUDIO_ST_DIGITALOUT_GETCHANNELSTATUS, *PAUDIO_ST_DIGITALOUT_GETCHANNELSTATUS;

typedef struct __attribute__((packed))
{
    MS_BOOL                     onOff;
    AUDIO_FS_TYPE               hdmi_smpFreq;
    HDMI_TX_OUTPUT_TYPE         outType;
}AUDIO_HDMI_TX_GETSTATUS, *PAUDIO_HDMI_TX_GETSTATUS;

typedef struct __attribute__((packed))
{
    MS_U32                      parameter_size;
    MS_U32                      type_size;
    void *                      Parameter_ptr;
}AUDIO_ST_COPY_PARAMETER, *PAUDIO_ST_COPY_PARAMETER;

typedef struct __attribute__((packed))
{
    MS_U8                       u8Status;
    MS_U16                      mNumCodecs;
    MS_U16                      mSamples;
    MS_U16                      mSampleRate;
    MS_U16*                     pChannels;
    MS_U16*                     pRegions;
    MS_U16*                     pcplStart;
    MS_U16*                     pcplQbits;
    MS_U16*                     pFrameSize;
}AUDIO_ST_RA8_Param, *PAUDIO_ST_RA8_Param;

typedef struct __attribute__((packed))
{
    MS_U8                       u8Status;
    MS_PHY*                     pPHYWrtAddr;
    MS_U32*                     pU32WrtBytes;
}AUDIO_ST_CHECK_INPUT_REQ, *PAUDIO_ST_CHECK_INPUT_REQ;

typedef struct __attribute__((packed))
{
    MS_U8                       u8Status;
    En_DVB_AudioDecoder         AUDDecoderNo;
    MS_PHY*                     pPHYWrtAddr;
    MS_U32*                     pU32WrtBytes;
}AUDIO_ST_MM_CHECK_INPUT_REQ, *PAUDIO_ST_MM_CHECK_INPUT_REQ;

typedef struct __attribute__((packed))
{
    MS_BOOL                     bStatus;
    MS_U32                      bytes;
    union
    {
        void*                   pBuffer;
        MS_U64                  dummy1;
    };
}AUDIO_ST_DMAREADER_WRITEPCM, *PAUDIO_ST_DMAREADER_WRITEPCM;

typedef struct __attribute__((packed))
{
    MS_BOOL                     bStatus;
    AUDIO_DEVICE_TYPE           eID;
    AUDIO_CAPTURE_SOURCE_TYPE   eSource;
}AUDIO_ST_PCMCAPTURE_INIT, *PAUDIO_ST_PCMCAPTURE_INIT;

typedef struct __attribute__((packed))
{
    MS_BOOL                     bStatus;
    AUDIO_DEVICE_TYPE           eID;
    MS_U32                      bytes;
    void*                       pBuffer;
}AUDIO_ST_PCMCAPTURE_READ, *PAUDIO_ST_PCMCAPTURE_READ;

typedef struct __attribute__((packed))
{
    MS_BOOL                     bStatus;
    MS_PHY*                     pPHYWrtAddr;
    MS_U32*                     pU32WrtBytes;
}AUDIO_ST_UNI_CHECK_DECODEDONE, *PAUDIO_ST_UNI_CHECK_DECODEDONE;

typedef struct __attribute__((packed))
{
    audioRingTask*              pauRingTask;
    audioRingTask*              pauRingTask_1;
}AUDIO_ST_RING_DATA_TRANS, *PAUDIO_ST_RING_DATA_TRANS;

typedef struct __attribute__((packed))
{
    AES_INFO*                   paes_info;
    AUDIO_DEC_ID                dec_id;
}AUDIO_ST_MM2_CHECKAESINFO, *PAUDIO_ST_MM2_CHECKAESINFO;

typedef struct __attribute__((packed))
{
    MS_BOOL                     bStatus;
    AUDIO_DEC_ID                dec_id;
    MS_U32                      es_size;
    MS_BOOL                     ptsExist;
    MS_U64                      pts;
}AUDIO_ST_MM2_INPUTAES_FINISH, *PAUDIO_ST_MM2_INPUTAES_FINISH;

typedef struct __attribute__((packed))
{
    MS_BOOL                     bStatus;
    AUDIO_DEC_ID                DecId;
    Audio_InfoType              infoType;
    union {
        void*                   pInfo;
        MS_U64                  dummy1;
    };
}AUDIO_ST_GETAUDIOINFO2, *PAUDIO_ST_GETAUDIOINFO2;


typedef struct __attribute__((packed))
{
    AdvSound_SET_Type           eType;
    MS_U32*                     pParam;
}AUDIO_ST_ADV_SOUND_SETPARAM, *PAUDIO_ST_ADV_SOUND_SETPARAM;

typedef struct __attribute__((packed))
{
    MS_BOOL                     bSet;
    OUTPUT_SOURCE_INFO*         stOutputSourceInfo;
}AUDIO_ST_OUTPUT_SOURCE, *PAUDIO_ST_OUTPUT_SOURCE;

typedef struct __attribute__((packed))
{
    AUDIO_SET_AD_TYPE           eType;
    MS_U8                       u8Data;
    MS_U8                       u8Data2;
    MS_BOOL                     bMute;
}AUDIO_ST_SET_AD, *PAUDIO_ST_SET_AD;

typedef struct __attribute__((packed))
{
    MS_BOOL                     bSet;
    AU_DVB_DECCMD               eDecCmd;
    En_DVB_AudioDecoder         eAdoDecoderNo;
    En_DVB_decCmdType           enCommandType;
}AUDIO_ST_DECODER_COMMAND, *PAUDIO_ST_DECODER_COMMAND;

typedef struct __attribute__((packed))
{
    AUDIO_SPDIF_OP              eType;
    MS_BOOL                     spdif_hwen;
    MS_BOOL                     mute_en;
    MS_U8                       spdif_mode;
}AUDIO_ST_SPDIF_OP, *PAUDIO_ST_SPDIF_OP;

typedef struct __attribute__((packed))
{
    MS_BOOL                     bSetSCMS;
    MS_U8                       C_bit_en;
    MS_U8                       L_bit_en;
    MS_U8                       Ret_bit;
}AUDIO_ST_SPDIF_SCMS, *PAUDIO_ST_SPDIF_SCMS;

typedef struct __attribute__((packed))
{
    MS_BOOL                     bSet;
    MS_U8                       nonPCM_en;
    AUDIO_HDMI_RX_TYPE          eRet;
}AUDIO_ST_HDMI_NONPCM_OP, *PAUDIO_ST_HDMI_NONPCM_OP;


typedef struct __attribute__((packed))
{
    AUDIO_KTV_OP                eType;
    AUDIO_KTV_MODEL             modelType;
    AUDIO_KTV_PARAMTYPE         paramType;
    AUDIO_KTV_INFOTYPE          infoType;
    AUDIO_SOURCE_INFO_TYPE      eSourceType;
    AUDIO_MIX_VOL_TYPE          VolType;
    MS_U8                       u8Vol1;
    MS_U8                       u8Vol2;
    MS_U32                      param1;
    MS_U32                      param2;
    MS_BOOL                     EnMute;
    MS_BOOL                     bRet;
    MS_U32                      u32Ret;
}AUDIO_ST_KTV_OP, *PAUDIO_ST_KTV_OP;

typedef struct __attribute__((packed))
{
    MS_BOOL                     bStatus;
    AUDIO_DEC_ID                DecID;
    Audio_ParamType             paramType;
    MS_U32*                     Param;
}AUDIO_ST_SETDECODEPARAM, *PAUDIO_ST_SETDECODEPARAM;

#if 0
typedef struct __attribute__((packed))
{
    AUDIO_INIT_INFO             *pau_info;
}AUDIO_ST_AUDIO_INIT_INFO, *PAUDIO_ST_AUDIO_INIT_INFO;

typedef struct __attribute__((packed))
{
    AUDIO_PATH_INFO             *path_info;
}AUDIO_ST_AUDIO_PATH_INFO, *PAUDIO_ST_AUDIO_PATH_INFO;

typedef struct __attribute__((packed))
{
    AUDIO_OUT_INFO              *pout_info;
}AUDIO_ST_AUDIO_OUT_INFO, *PAUDIO_ST_AUDIO_OUT_INFO;

typedef struct __attribute__((packed))
{
    AUDIO_PEQ_COEF              *peq_coef;
}AUDIO_ST_AUDIO_PEQ_COEF, *PAUDIO_ST_AUDIO_PEQ_COEF;
#endif

typedef struct __attribute__((packed))
{
    AU_DVB_ENC_FRAME_INFO       *EncFrameInfo;
}AUDIO_ST_AU_DVB_ENC_FRAME_INFO, *PAUDIO_ST_AU_DVB_ENC_FRAME_INFO;

#if 0
typedef struct __attribute__((packed))
{
    THR_TBL_TYPE code           *ThrTbl;
}AUDIO_ST_THR_TBLL_TYPE, *PAUDIO_ST_THR_TBLL_TYPE;

typedef struct __attribute__((packed))
{
    AUDIO_UPLOAD_INFO           *uploadInfo;
}AUDIO_ST_UPLOAD_INFO, *PAUDIO_ST_UPLOAD_INFO;
#endif
typedef struct __attribute__((packed))
{
    audioRingTask               *auRingTask;
}AUDIO_ST_AUDIORINGTASK, *PAUDIO_ST_AUDIORINGTASK;

typedef struct __attribute__((packed))
{
    MS_BOOL                     bStatus;
    int*                        pthresh_level_table;
}AUDIO_ST_AUDYSSEY_VXT_SET_THRESH_LEVEL_TABLE, *PAUDIO_ST_AUDYSSEY_VXT_SET_THRESH_LEVEL_TABLE;

typedef struct __attribute__((packed))
{
    MS_BOOL                     bSet;
    SPDIF_CS_MODE_TYPE          eType;
    SPDIF_CS_MODE_VALUE         eValue;
    MS_U32                      u32Ret;
}AUDIO_ST_SPDIF_CHANNELSTATUS, *PAUDIO_ST_SPDIF_CHANNELSTATUS;

typedef struct __attribute__((packed))
{
    MS_U32                      u32Ret;
    AUDIO_DEC_ID                eDecId;
    MS_U32*                     pu32Caps;
}AUDIO_ST_GETCAPS, *PAUDIO_ST_GETCAPS;

typedef struct __attribute__((packed))
{
    AU_DVB_ENCCMD               enEncComamnd;
    void                        *pCB;
    MS_U8                       u8Param;
}AUDIO_ST_SETENCCOMMAND, *PAUDIO_ST_SETENCCOMMAND;

typedef struct __attribute__((packed))
{
    MS_U8                       u8Index;
    union
    {
        MS_PHY                  u32Bin_Base_Address;
        MS_U64                  dummy1;
    };
    union
    {
        MS_PHY                  u32Mad_Base_Buffer_Adr;
        MS_U64                  dummy2;
    };
}AUDIO_ST_SET_DSPBASEADDR, *PAUDIO_ST_SET_DSPBASEADDR;

typedef struct __attribute__((packed))
{
    union
    {
        MS_PHY                  u32Addr;
        MS_U64                  dummy1;
    };
    MS_U8                       u8Index;
}AUDIO_ST_GET_DSPBASEADDR, *PAUDIO_ST_GET_DSPBASEADDR;

typedef struct __attribute__((packed))
{
    MS_U8                       u8ParamNum;
    MS_U16                      u16Data;
}AUDIO_ST_WRITE_DECMAILBOX, *PAUDIO_ST_WRITE_DECMAILBOX;

typedef struct __attribute__((packed))
{
    AUDIO_INPUT_TYPE            eSource;
    AUDIO_SWITCH_GROUP          eGroup;
}AUDIO_ST_INPUTSWITCH, *PAUDIO_ST_INPUTSWITCH;

typedef struct __attribute__((packed))
{
    MS_BOOL                     bRet;
    AUDIO_DEVICE_TYPE           eID;
    AUDIO_CAPTURE_SOURCE_TYPE   eSource;
}AUDIO_ST_SET_DATACAPTURESOURCE, *PAUDIO_ST_SET_DATACAPTURESOURCE;

typedef struct __attribute__((packed))
{
    MS_U16                      u16Ret;
    MS_BOOL                     bDspType;
    MS_U8                       u8ParamNum;
}AUDIO_ST_READ_MAILBOX, *PAUDIO_ST_READ_MAILBOX;

typedef struct __attribute__((packed))
{
    MS_BOOL                     bDspType;
    MS_U8                       u8ParamNum;
    MS_U16                      u16Data;
}AUDIO_ST_WRITE_MAILBOX, *PAUDIO_ST_WRITE_MAILBOX;

typedef struct __attribute__((packed))
{
    MS_BOOL                     bDspType;
    MS_U8                       u8Cmd;
}AUDIO_ST_SEND_INTERRUPT, *PAUDIO_ST_SEND_INTERRUPT;

typedef struct __attribute__((packed))
{
    AUDIO_I2S_MODE_TYPE         eMode;
    AUDIO_I2S_MODE_VALUE        eVal;
}AUDIO_ST_I2S_SETMODE, *PAUDIO_ST_I2S_SETMODE;

typedef struct __attribute__((packed))
{
    MS_BOOL                     bRet;
    Sound_ENABLE_Type           eType;
    MS_BOOL                     bEnable;
}AUDIO_ST_SND_PROCESSENABLE, *PAUDIO_ST_SND_PROCESSENABLE;

typedef struct __attribute__((packed))
{
    MS_BOOL                     bRet;
    Sound_SET_PARAM_Type        eType;
    MS_U16                      u16Param1;
    MS_U16                      u16Param2;
}AUDIO_ST_SND_SETPARAM1, *PAUDIO_ST_SND_SETPARAM1;

typedef struct __attribute__((packed))
{
    MS_U16                      u16Ret;
    Sound_GET_PARAM_Type        eType;
    MS_U16                      u16Param1;
}AUDIO_ST_SND_GETPARAM1, *PAUDIO_ST_SND_GETPARAM1;

typedef struct __attribute__((packed))
{
    MS_U8                       u8Band;
    MS_U8                       u8Level;
}AUDIO_ST_SETEQ_7BAND, *PAUDIO_ST_SETEQ_7BAND;

typedef struct __attribute__((packed))
{
    MS_U8                       u8Path;
    MS_U8                       u8Vol1;
    MS_U8                       u8Vol2;
}AUDIO_ST_SET_ABSOLUTEVOLUME, *PAUDIO_ST_SET_ABSOLUTEVOLUME;

typedef struct __attribute__((packed))
{
    AUDIO_OUTPORT_SOURCE_TYPE   eConnectSourceType;
    MS_BOOL                     bConnect;
}AUDIO_ST_SOURCECONNECT, *PAUDIO_ST_SOURCECONNECT;

typedef struct __attribute__((packed))
{
    MS_U8                       u8Ret;
    MS_U8                       u8Uivolume;
}AUDIO_ST_CONVERT_VOLUMEUNIT, *PAUDIO_ST_CONVERT_VOLUMEUNIT;

typedef struct __attribute__((packed))
{
    MS_BOOL                     bRet;
    MS_BOOL                     bEndcremove;
}AUDIO_ST_ENABLE_DCREMOVE, *PAUDIO_ST_ENABLE_DCREMOVE;

typedef struct __attribute__((packed))
{
    MS_BOOL                     bRet;
    AUDIO_DEC_ID                eDecId;
    En_DVB_decCmdType           eDecComamnd;
}AUDIO_ST_SET_DECCMD, *PAUDIO_ST_SET_DECCMD;

typedef struct __attribute__((packed))
{
    MS_BOOL                     bRet;
    En_DVB_decSystemType        eDecSystem;
}AUDIO_ST_SET_SYSTEM, *PAUDIO_ST_SET_SYSTEM;

typedef struct __attribute__((packed))
{
    MS_BOOL                     bRet;
    En_DVB_decSystemType        eDecSystem;
}AUDIO_ST_GET_DECSYSSUPPORTSTATUS, *PAUDIO_ST_GET_DECSYSSUPPORTSTATUS;

typedef struct __attribute__((packed))
{
    AU_DVB_ENCBR                eBitRate;
    MS_U8                       u8EncFrameNum;
}AUDIO_ST_SET_ENCINIT, *PAUDIO_ST_SET_ENCINIT;

typedef struct __attribute__((packed))
{
    MS_BOOL                     bRet;
    MS_U8                       u8SifSoundMode;
}AUDIO_ST_SIF_SET_SOUNDMODE, *PAUDIO_ST_SIF_SET_SOUNDMODE;

typedef struct __attribute__((packed))
{
    AUDIOSTANDARD_TYPE          eRet;
    AUDIOSTANDARD_TYPE          eStandard;
}AUDIO_ST_SIF_SIF_CONVERTTOBASICAUDIOSTANDARD, *PAUDIO_ST_SIF_CONVERTTOBASICAUDIOSTANDARD;

typedef struct __attribute__((packed))
{
    MS_U8                       u8GainType;
    MS_S32                      s32DbValue;
}AUDIO_ST_SIF_SETPRESCALE, *PAUDIO_ST_SIF_SETPRESCALE;

typedef struct __attribute__((packed))
{
    MS_BOOL                     bRet;
    AUDIO_SIF_PAL_TYPE          ePalType;
}AUDIO_ST_SIF_SIF_ISPALTYPE, *PAUDIO_ST_SIF_ISPALTYPE;

typedef struct __attribute__((packed))
{
    En_AUD_SIF_CmdType          eAudSifCommand;
    MS_U8                       u8CommArg1;
    MS_U8                       u8CommArg2;
}AUDIO_ST_SIF_SENDCMD, *PAUDIO_ST_SIF_SENDCMD;

typedef struct __attribute__((packed))
{
    MS_BOOL                     bRet;
    AUDIO_SPDIF_CS_TYPE         eCsMode;
    AUDIO_SPDIF_CS_TYPE_STATUS  eStatus;
}AUDIO_ST_SPDIF_CHANNELSTATUS_CTRL, *PAUDIO_ST_SPDIF_CHANNELSTATUS_CTRL;

typedef struct __attribute__((packed))
{
    MS_BOOL                     bRet;
    ADVSND_TYPE                 eType;
}AUDIO_ST_ADVSOUND_PROCESSENABLE, *PAUDIO_ST_ADVSOUND_PROCESSENABLE;

typedef struct __attribute__((packed))
{
    En_DVB_advsndType           eType;
}AUDIO_ST_SETADVSNDSYS, *PAUDIO_ST_SETADVSNDSYS;

typedef struct __attribute__((packed))
{
    MS_BOOL                     bRet;
    MS_BOOL                     bEnable;
    ADVFUNC                     eProc;
}AUDIO_ST_ADVSOUND_SUBPROCESSENABLE, *PAUDIO_ST_ADVSOUND_SUBPROCESSENABLE;

typedef struct __attribute__((packed))
{
    MS_BOOL                     bRet;
    MS_U16                      u16Value1;
    MS_U16                      u16Value2;
    ADVSND_PARAM                eParam;
}AUDIO_ST_ADVSOUND_SETPARAM, *PAUDIO_ST_ADVSOUND_SETPARAM;

typedef struct __attribute__((packed))
{
    MS_BOOL                     bRet;
    MS_U32                      u32Value1;
    MS_U16                      u16Value2;
    ADVSND_PARAM                eParam;
}AUDIO_ST_ADVSND_SETPARAM, *PAUDIO_ST_ADVSND_SETPARAM;

typedef struct __attribute__((packed))
{
    MS_BOOL                     bRet;
    MS_S16                      s16Value1;
    MS_S16                      s16Value2;
    ADVSND_PARAM                eParam;
}AUDIO_ST_ADVSND_SETASFPARAM, *PAUDIO_ST_ADVSND_SETASFPARAM;

typedef struct __attribute__((packed))
{
    MS_U8                       u8EnBBE;
    MS_U8                       u8BBEMode;
}AUDIO_ST_SETBBE, *PAUDIO_ST_SETBBE;

typedef struct __attribute__((packed))
{
    MS_U8                       u8Mode;
    MS_U8                       u8Value;
}AUDIO_ST_SRS_SETTSXTPARA, *PAUDIO_ST_SRS_SETTSXTPARA;

typedef struct __attribute__((packed))
{
    MS_U8                       u8Mode;
    MS_U8                       u8Value;
}AUDIO_ST_SRS_SETTSHDPARA, *PAUDIO_ST_SRS_SETTSHDPARA;

typedef struct __attribute__((packed))
{
    MS_U32                      u32Ret;
    AdvSound_GET_Type           eType;
}AUDIO_ST_SRS_ADVSOUND_GETINFO, *PAUDIO_ST_SRS_ADVSOUND_GETINFO;

typedef struct __attribute__((packed))
{
    EN_DBXTV_TotSonMode         eTotSonMode;
    EN_DBXTV_TotVolMode         eTotVolMode;
    EN_DBXTV_TotSurMode         eTotSurMode;
    MS_U32                      u32Enable;
}AUDIO_ST_DBXTV_SETMODE, *PAUDIO_ST_DBXTV_SETMODE;

typedef struct __attribute__((packed))
{
    MS_BOOL                     bRet;
    Audio_COMM_infoType         eInfoType;
    MS_VIRT                     Param1;
    MS_VIRT                     Param2;
}AUDIO_ST_SET_COMMAUDIOINFO, *PAUDIO_ST_SET_COMMAUDIOINFO;

typedef struct __attribute__((packed))
{
    MS_BOOL                     bRet;
    Audio_MPEG_infoType         eInfoType;
    MS_VIRT                     Param1;
    MS_VIRT                     Param2;
}AUDIO_ST_SET_MPEGINFO, *PAUDIO_ST_SET_MPEGINFO;

typedef struct __attribute__((packed))
{
    MS_BOOL                     bRet;
    Audio_AC3_infoType          eInfoType;
    MS_VIRT                     Param1;
    MS_VIRT                     Param2;
}AUDIO_ST_SET_AC3INFO, *PAUDIO_ST_SET_AC3INFO;

typedef struct __attribute__((packed))
{
    MS_BOOL                     bRet;
    Audio_AC3P_infoType         eInfoType;
    MS_VIRT                     Param1;
    MS_VIRT                     Param2;
}AUDIO_ST_SET_AC3PINFO, *PAUDIO_ST_SET_AC3PINFO;

typedef struct __attribute__((packed))
{
    MS_BOOL                     bRet;
    Audio_AAC_infoType          eInfoType;
    MS_VIRT                     Param1;
    MS_VIRT                     Param2;
}AUDIO_ST_SET_AACINFO, *PAUDIO_ST_SET_AACINFO;

typedef struct __attribute__((packed))
{
    MS_BOOL                     bRet;
    Audio_WMA_infoType          eInfoType;
    MS_VIRT                     Param1;
    MS_VIRT                     Param2;
}AUDIO_ST_SET_WMAINFO, *PAUDIO_ST_SET_WMAINFO;

typedef struct __attribute__((packed))
{
    MS_BOOL                     bRet;
    Audio_DTS_infoType          eInfoType;
    Audio_DTS_CtrlType          eCtrlType;
}AUDIO_ST_SET_DTSCOMMONCTRL, *PAUDIO_ST_SET_DTSCOMMONCTRL;

typedef struct __attribute__((packed))
{
    MS_BOOL                     bRet;
    AUDIO_DEC_ID                eDecId;
}AUDIO_ST_MM2_INITAESINFO, *PAUDIO_ST_MM2_INITAESINFO;

typedef struct __attribute__((packed))
{
    XPCM_TYPE                   eAudioType;
    MS_U8                       u8Channels;
    MS_U16                      u16SampleRate;
    MS_U8                       u8BitsPerSample;
    MS_U16                      u16BlockSize;
    MS_U16                      u16SamplePerBlock;
}AUDIO_ST_SET_XPCMPARAM, *PAUDIO_ST_SET_XPCMPARAM;

typedef struct __attribute__((packed))
{
    MS_U64                      u64Ret;
    Audio_COMM_infoType         eInfoType;
}AUDIO_ST_GET_COMMAUDIOINFO, *PAUDIO_ST_GET_COMMAUDIOINFO;

typedef struct __attribute__((packed))
{
    MS_U32                      u32Ret;
    Audio_MPEG_infoType         eInfoType;
}AUDIO_ST_GET_MPEGINFO, *PAUDIO_ST_GET_MPEGINFO;

typedef struct __attribute__((packed))
{
    MS_U32                      u32Ret;
    Audio_AC3_infoType          eInfoType;
}AUDIO_ST_GET_AC3INFO, *PAUDIO_ST_GET_AC3INFO;

typedef struct __attribute__((packed))
{
    MS_U32                      u32Ret;
    Audio_AC3P_infoType         eInfoType;
}AUDIO_ST_GET_AC3PINFO, *PAUDIO_ST_GET_AC3PINFO;

typedef struct __attribute__((packed))
{
    MS_U32                      u32Ret;
    Audio_AAC_infoType          eInfoType;
}AUDIO_ST_GET_AACINFO, *PAUDIO_ST_GET_AACINFO;

typedef struct __attribute__((packed))
{
    MS_U32                      u32Ret;
    Audio_WMA_infoType          eInfoType;
}AUDIO_ST_GET_WMAINFO, *PAUDIO_ST_GET_WMAINFO;

typedef struct __attribute__((packed))
{
    MS_U32                      u32Ret;
    Audio_DTS_infoType          eInfoType;
}AUDIO_ST_GET_DTSINFO, *PAUDIO_ST_GET_DTSINFO;

typedef struct __attribute__((packed))
{
    MS_U8                       u8Ret;
    XPCM_TYPE                   eAudioType;
    MS_U8                       u8Channels;
    MS_U16                      u16SampleRate;
    MS_U8                       u8BitsPerSample;
    MS_U16                      u16BlockSize;
    MS_U16                      u16SamplePerBlock;
}AUDIO_ST_XPCMPARAM, *PAUDIO_ST_XPCMPARAM;

typedef struct __attribute__((packed))
{
    MS_U16                      u16Ret;
    MS_U16                      u16BitRate;
}AUDIO_ST_GET_PCMBUFFERSIZE, *PAUDIO_ST_GET_PCMBUFFERSIZE;

typedef struct __attribute__((packed))
{
    MS_U16                      u16Ret;
    MS_U16                      u16BitRate;
    MS_U16                      u16SampleRate;
}AUDIO_ST_GET_PCMBUFFERSIZE2, *PAUDIO_ST_GET_PCMBUFFERSIZE2;

typedef struct __attribute__((packed))
{
    WMA_ASF_PARMTYPE            eParmType;
    MS_U32                      u32Value;
}AUDIO_ST_SET_ASFPARM, *PAUDIO_ST_SET_ASFPARM;

typedef struct __attribute__((packed))
{
    En_DVB_AudioDecoder         eAudDecoderNo;
    MS_U8                       u8IntTag;
}AUDIO_ST_MM_SETINPUT, *PAUDIO_ST_MM_SETINPUT;

typedef struct __attribute__((packed))
{
    MS_U32                      u32Ret;
    En_DVB_AudioDecoder         eAudDecoderNo;
}AUDIO_ST_MM_CHECKPLAYDONE, *PAUDIO_ST_MM_CHECKPLAYDONE;

typedef struct __attribute__((packed))
{
    MS_BOOL                     bRet;
    AUDIO_VoIP_CONFIG_TYPE      eConfigType;
    MS_U32                      u32Param1;
    MS_U32                      u32Param2;
}AUDIO_ST_VOIP_CONFIG, *PAUDIO_ST_VOIP_CONFIG;

typedef struct __attribute__((packed))
{
    MS_BOOL                     bRet;
    MS_BOOL                     bEnable;
}AUDIO_ST_ALSA_ENABLE, *PAUDIO_ST_ALSA_ENABLE;

typedef struct __attribute__((packed))
{
    MS_BOOL                     bRet;
    AUDIO_DEC_ID                eDecId;
    Audio_ParamType             eParamType;
    MS_U64                      Param;
}AUDIO_ST_SET_AUDIOPARAM2, *PAUDIO_ST_SET_AUDIOPARAM2;

typedef struct __attribute__((packed))
{
    union
    {
        MS_PHY                  Ret;
        MS_U64                  dummy1;
    };
    AUDIO_DEC_ID                eDecId;
    EN_AUDIO_DDRINFO            eDdrInfo;
}AUDIO_ST_GET_DDRINFO, *PAUDIO_ST_GET_DDRINFO;

typedef struct __attribute__((packed))
{
    MS_S32                      s32Ret;
    void*                       pData;
}AUDIO_ST_PCM_OPEN, *PAUDIO_ST_PCM_OPEN;

typedef struct __attribute__((packed))
{
    MS_S32                      s32Ret;
    MS_S32                      s32DeviceId;
}AUDIO_ST_PCM_CLOSE, *PAUDIO_ST_PCM_CLOSE;

typedef struct __attribute__((packed))
{
    MS_S32                      s32Ret;
    MS_S32                      s32DeviceId;
}AUDIO_ST_PCM_START, *PAUDIO_ST_PCM_START;

typedef struct __attribute__((packed))
{
    MS_S32                      s32Ret;
    MS_S32                      s32DeviceId;
}AUDIO_ST_PCM_STOP, *PAUDIO_ST_PCM_STOP;

typedef struct __attribute__((packed))
{
    MS_S32                      s32Ret;
    MS_S32                      s32DeviceId;
    MS_U32                      u32Cmd;
    const void*                 pData;
}AUDIO_ST_PCM_SET, *PAUDIO_ST_PCM_SET;

typedef struct __attribute__((packed))
{
    MS_S32                      s32Ret;
    MS_S32                      s32DeviceId;
    MS_U32                      u32Cmd;
    void*                       pData;
}AUDIO_ST_PCM_GET, *PAUDIO_ST_PCM_GET;

typedef struct __attribute__((packed))
{
    MS_U32                      u32Ret;
    MS_S32                      s32DeviceId;
    MS_U32                      u32Size;
    void*                       pBuf;
}AUDIO_ST_PCM_READ, *PAUDIO_ST_PCM_READ;

typedef struct __attribute__((packed))
{
    MS_U32                      u32Ret;
    MS_S32                      s32DeviceId;
    MS_U32                      u32Size;
    const void*                 pBuf;
}AUDIO_ST_PCM_WRITE, *PAUDIO_ST_PCM_WRITE;

typedef struct __attribute__((packed))
{
    MS_S32                      s32Ret;
    MS_S32                      s32DeviceId;
}AUDIO_ST_PCM_FLUSH, *PAUDIO_ST_PCM_FLUSH;

typedef struct __attribute__((packed))
{
    MS_S32                      s32Ret;
    const char*                 pName;
}AUDIO_ST_ENCODER_OPEN, *PAUDIO_ST_ENCODER_OPEN;

typedef struct __attribute__((packed))
{
    MS_S32                      s32Ret;
    MS_S32                      s32DeviceId;
}AUDIO_ST_ENCODER_CLOSE, *PAUDIO_ST_ENCODER_CLOSE;

typedef struct __attribute__((packed))
{
    MS_S32                      s32Ret;
    MS_S32                      s32DeviceId;
}AUDIO_ST_ENCODER_START, *PAUDIO_ST_ENCODER_START;

typedef struct __attribute__((packed))
{
    MS_S32                      s32Ret;
    MS_S32                      s32DeviceId;
}AUDIO_ST_ENCODER_STOP, *PAUDIO_ST_ENCODER_STOP;

typedef struct __attribute__((packed))
{
    MS_S32                      s32Ret;
    MS_S32                      s32DeviceId;
    const char*                 pCmd;
    void*                       pData;
}AUDIO_ST_ENCODER_IOCTL, *PAUDIO_ST_ENCODER_IOCTL;

typedef struct __attribute__((packed))
{
    MS_U32                      u32Ret;
    MS_S32                      s32DeviceId;
    MS_U32                      u32Size;
    void*                       pBuf;
}AUDIO_ST_ENCODER_READ, *PAUDIO_ST_ENCODER_READ;

typedef struct __attribute__((packed))
{
    MS_S32                      s32Ret;
    MS_S32                      s32DeviceId;
}AUDIO_ST_ENCODER_FLUSH, *PAUDIO_ST_ENCODER_FLUSH;

typedef struct __attribute__((packed))
{
    MS_S32                      s32Ret;
    MS_U32                      u32CmdSize;
    MS_U64*                     pHandle;
    const char*                 pCmdLine;
}AUDIO_ST_DEBUG_CMD_READ, *PAUDIO_ST_DEBUG_CMD_READ;

typedef struct __attribute__((packed))
{
    MS_S32                      s32Ret;
    MS_U32                      u32CmdSize;
    MS_U64*                     pHandle;
    const char*                 pCmdLine;
}AUDIO_ST_DEBUG_CMD_WRITE, *PAUDIO_ST_DEBUG_CMD_WRITE;

typedef struct __attribute__((packed))
{
    MS_U32                      u32StrCmdLength;
    MS_S32                      s32Ret;
    MS_U8*                      pStrCmd;
}AUDIO_ST_SYSTEM_CONTROL, *PAUDIO_ST_SYSTEM_CONTROL;

typedef struct __attribute__((packed))
{
    MS_S32                      s32Ret;
}AUDIO_ST_CHANNEL_OPEN, *PAUDIO_ST_CHANNEL_OPEN;

typedef struct __attribute__((packed))
{
    MS_S32                      s32Ret;
    MS_S32                      s32ChannelId;
}AUDIO_ST_CHANNEL_OPEN_BYID, *PAUDIO_ST_CHANNEL_OPEN_BYID;

typedef struct __attribute__((packed))
{
    MS_S32                      s32Ret;
    MS_S32                      s32ChannelId;
}AUDIO_ST_CHANNEL_CLOSE, *PAUDIO_ST_CHANNEL_CLOSE;

typedef struct __attribute__((packed))
{
    MS_S32                      s32Ret;
    MS_S32                      s32ChannelId;
    const MS_U8*                      pStrCmd;
    MS_U32                      u32StrCmdLength;
    MS_U32*                     pData;
}AUDIO_ST_CHANNEL_IOCTL, *PAUDIO_ST_CHANNEL_IOCTL;

typedef struct __attribute__((packed))
{
    AUDIO_INTERNAL_PATH_TYPE    enPath;
    AUDIO_OUTPUT_TYPE           enOutput;
}AUDIO_ST_SET_INTERNALPATH, *PAUDIO_ST_SET_INTERNALPATH;

#ifdef __cplusplus
}
#endif


#endif //_AUDIO_H_
