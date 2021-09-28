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
//  File name:   apiAUDIO_v2_customer_config.h
//  Description: apiAUDIO_v2_customer_config.h
//
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef ROLLS_ROYCE

#ifndef _AUDIO_V2_CUSTOMER_CONFIG_H_
#define _AUDIO_V2_CUSTOMER_CONFIG_H_

#ifdef __cplusplus
extern "C"
{
#endif

//--------------------------------------------------------------------------------------
//config for different Linux Kernel Version
//--------------------------------------------------------------------------------------
#define LINUX_KERNEL_VERSION_4_4_3              0

//--------------------------------------------------------------------------------------
//config for different upper layer
//--------------------------------------------------------------------------------------
#define UPPER_USB_MOUNT_PATH_STRING             "/mnt/sda1/"
#define UPPER_PROJECT_NAME                      "Master"
#define UPPER_STRTOK                            ";"
#define DBG_COLOR                               "\033[1;32m"
#define DBG_COLOR_END                           "\033[0m"
#define MonitorDBG_COLOR                        "\033[0;33m"
#if defined(ANDROID)
#define UPPER_ANDROID                           1
#else
#define UPPER_ANDROID                           0
#endif //defined(ANDROID)
#if defined(MSOS_TYPE_LINUX_KERNEL) //Kernel Space
#define UPPER_MdbPrint                          0
#else //User Space
#define UPPER_MdbPrint                          0
#endif //User Space

//--------------------------------------------------------------------------------------
//config for internal used file path
//--------------------------------------------------------------------------------------
#define AU_CUS_NDBG_LOG_TO_FILE_PATH                "/tmp/AudioCUSnDBGLog.log"
#define AU_CUS_NDBG_MENULOG_TO_FILE_PATH            "/tmp/AudioCUSnDBGMenuLog.log"
#define AU_CUS_R2_LOG_TO_FILE_PATH                  "/tmp/AudioR2Log.log"
#define AU_CUS_DDR_BIN_PATH                         "/tmp/AUDIO_DDR.bin"
#define AU_CUS_ADEC_MCU_PCM_DUMP_PATH               "/tmp/AdecPcmDump.pcm"
#define AU_CUS_DEMO_MM_NEW_MODE_GET_MCU_PCM_PATH    "/tmp/Demo_Audio_AdecPcmDump.pcm"
#define AU_CUS_PCM_CAPTURE0_SOURCE_DUMP_PATH        "/tmp/PcmCapture0SourceDump.pcm"
#define AU_CUS_PCM_CAPTURE1_SOURCE_DUMP_PATH        "/tmp/PcmCapture1SourceDump.pcm"
#define AU_CUS_PCM_CAPTURE2_SOURCE_DUMP_PATH        "/tmp/PcmCapture2SourceDump.pcm"
#define AU_CUS_MP3_ENCODE_DUMP_PATH                 "/tmp/MP3EncDump.mp3"
#define AU_CUS_AAC_ENCODE_DUMP_PATH                 "/tmp/AACEncDump.aac"
#define AU_CUS_AAC_ENCODE_PCM_DUMP_PATH             "/tmp/AACEncPCMDump.pcm"
#define AU_CUS_MM_NEW_MODE_ADEC0_ES_DUMP_PATH       "/tmp/MM_Audio_ES1_Dump.bin"
#define AU_CUS_MM_NEW_MODE_ADEC0_AD_ES_DUMP_PATH    "/tmp/MM_Audio_ES1_AD_Dump.bin"
#define AU_CUS_GENERAL_DUMP_ADEC0_PCM_PATH          "/tmp/General_Buffer_Dump1_DEC_PCM1_Buffer.pcm"
#define AU_CUS_MM_NEW_MODE_ADEC1_ES_DUMP_PATH       "/tmp/MM_Audio_ES3_Dump.bin"
#define AU_CUS_MM_NEW_MODE_ADEC1_AD_ES_DUMP_PATH    "/tmp/MM_Audio_ES3_AD_Dump.bin"
#define AU_CUS_GENERAL_DUMP_ADEC1_PCM_PATH          "/tmp/General_Buffer_Dump1_DEC_PCM2_Buffer.pcm"
#define AU_CUS_GENERAL_DUMP_SPDIF_NPCM_PATH         "/tmp/General_Buffer_Dump5_SPDIF_NPCM_Buffer.bin"
#define AU_CUS_GENERAL_DUMP_HDMI_NPCM_PATH          "/tmp/General_Buffer_Dump2_HDMI_NPCM_Buffer.bin"
#define AU_CUS_GENERAL_DUMP_MS12_METADATA_PATH      "/tmp/General_Buffer_Dump3_MS12_MetaData_Buffer.bin"
#define AU_CUS_GENERAL_DUMP_HDMI_ADEC0_ES_PATH      "/tmp/HDMI_Audio_ES1_Dump.bin"
#define AU_CUS_GENERAL_DUMP_HDMI_ADEC1_ES_PATH      "/tmp/HDMI_Audio_ES3_Dump.bin"
#define AU_CUS_GENERAL_DUMP_ADEC0_AC3_ES_PATH       "/tmp/ADEC0_AC3_Dump.bin"
#define AU_CUS_GENERAL_DUMP_ADEC1_AC3_ES_PATH       "/tmp/ADEC1_AC3_Dump.bin"
#define AU_CUS_FRAME_DECODE_PCM_CH0_DUMP            "/tmp/Frame_Decode_PCM_CH0_Dump.pcm"
#define AU_CUS_FRAME_DECODE_PCM_CH1_DUMP            "/tmp/Frame_Decode_PCM_CH1_Dump.pcm"
#define AU_CUS_FRAME_DECODE_PCM_CH2_DUMP            "/tmp/Frame_Decode_PCM_CH2_Dump.pcm"
#define AU_CUS_FRAME_DECODE_PCM_CH3_DUMP            "/tmp/Frame_Decode_PCM_CH3_Dump.pcm"
#define AU_CUS_FRAME_DECODE_PCM_CH4_DUMP            "/tmp/Frame_Decode_PCM_CH4_Dump.pcm"
#define AU_CUS_FRAME_DECODE_PCM_CH5_DUMP            "/tmp/Frame_Decode_PCM_CH5_Dump.pcm"
#define AU_CUS_FRAME_DECODE_PCM_CH6_DUMP            "/tmp/Frame_Decode_PCM_CH6_Dump.pcm"
#define AU_CUS_FRAME_DECODE_PCM_CH7_DUMP            "/tmp/Frame_Decode_PCM_CH7_Dump.pcm"
#define AU_CUS_FRAME_DECODE_PCM_CH8_DUMP            "/tmp/Frame_Decode_PCM_CH8_Dump.pcm"
#define AU_CUS_FRAME_DECODE_PCM_CH9_DUMP            "/tmp/Frame_Decode_PCM_CH9_Dump.pcm"
#define AU_CUS_FRAME_DECODE_PTS_DUMP_PATH           "/tmp/Frame_Decode_PTS_Dump.txt"
#define AU_CUS_WMA_PARSER_INTERMEDIATE_ES_PATH      "/tmp/WmaIntermediate.es"

//--------------------------------------------------------------------------------------
//config for different UTPA
//--------------------------------------------------------------------------------------
#define UTPA_SUPPORT_NDBG_LOG_TO_FILE               0
#define UTPA_SUPPORT_AAC_ENCODE                     0
#define UTPA_SUPPORT_COMMON_DECODER_API             0
#define UTPA_SUPPORT_HDMI_TX_OUT_BYPASS             0
#define UTPA_SUPPORT_DOLBY_DAP                      1
#define UTPA_SUPPORT_SONICEMOTION_ABS3D             0
#define UTPA_SUPPORT_DTS_VIRTUALX                   0
#define UTPA_SUPPORT_VirtualX_Profile2_2_201_1_0    0
#define UTPA_SUPPORT_DTS_VIRTUALX_APPCTRLS          0
#define UTPA_SUPPORT_COMMON_CMD_API                 1
#define UTPA_SUPPORT_XPCM_PARAM_CHANNEL_MASK        0
#define UTPA_SUPPORT_HDMI_TX_STATUS                 0
#define UTPA_SUPPORT_SET_POWER_ON                   1
#define UTPA_SUPPORT_AVC_R_MODE                     0
#define UTPA_SUPPORT_SND_INTERNAL_DELAY             1
#define UTPA_SUPPORT_IS_ATMOS_STREAM                0
#define UTPA_SUPPORT_IS_ATMOS_STREAM_V2             1
#define UTPA_SUPPORT_AAC_TYPE                       1
#define UTPA_SUPPORT_AEQ_COEFFICIENT                0
#define UTPA_SUPPORT_AUDIO_UNIT_CTRL                0
#define UTPA_SUPPORT_AUDIO_JUNGLE                   0
#define UTPA_SUPPORT_AUDIO_SYSTEM_CONTROL           1
#ifdef MSOS_TYPE_LINUX_KERNEL //Kernel Space
#define UTPA_SUPPORT_CWRAP_BUILD_IN                 1
#else //User Space
#define UTPA_SUPPORT_CWRAP_BUILD_IN                 1
#endif //User Space
#define UTPA_SUPPORT_MM_INPUT_PAN_FADE              0
#define UTPA_SUPPORT_HDMI_RX_DTS_HD                 0
#define UTPA_SUPPORT_AC3P_1PID_AD_ENABLE            0
#define UTPA_SUPPORT_R2_SET_NONPCM_SEL              0
#define UTPA_SUPPORT_R2_SHM_PARAM_MD_Volume         0
#define UTPA_SUPPORT_HDMI_RX_SetNonpcm_PathPlay     1
#define UTPA_SUPPORT_DDPE_DDPBYPASS_INDEPENDENT     0
#define UTPA_SUPPORT_DIRECT_DIVISION                1
#define UTPA_SUPPORT_SPDIF_RX_MONITOR               0
#define UTPA_SUPPORT_SPDIF_RX_SetNonpcm_PathPlay    0
#define UTPA_SUPPORT_AUD_CH_MODE                    1
#define UTPA_SUPPORT_MsOS_SHM_GetId_VIRTaddr        1
#define UTPA_SUPPORT_R2_TEMPO                       1
#define UTPA_SUPPORT_CODECTYPE_OPUS                 1
#define UTPA_SUPPORT_CODECTYPE_MPEG_H               0
#define UTPA_SUPPORT_CODECTYPE_AC4                  0
#define UTPA_SUPPORT_CODECTYPE_MAT                  0
#define UTPA_SUPPORT_SW_MIXER                       0
#define UTPA_SUPPORT_DECODER_DELAY                  1
#define UTPA_SUPPORT_Ioctrl                         1
#define UTPA_SUPPORT_PCM_CAPTURE_I2S_RX             1

//---MAD Base addr(DSP_DEC,DSP_SE,DSP_ADV,DSP_MAD)---
#define UTPA_SUPPORT_GetDspMadBaseAddr  1
#define MAD_BASE_ADDR_DEC               0x0
#define MAD_BASE_ADDR_SE                0x0
#define MAD_BASE_ADDR_ADV               0x0
#define MAD_BASE_ADDR_MAD               0x0

//---DDPE Control by MCU---
#define UTPA_SUPPORT_M2S_MBOX_DDPE_ENCODE_CTRL  0
#define M2S_MBOX_DDPE_ENCODE_CTRL               0x112DC2

//---HDMI NPCM Control by MCU---
#define UTPA_SUPPORT_M2S_MBOX_MCU_HDMI_NONPCM_CTRL  0
#define M2S_MBOX_MCU_HDMI_NONPCM_CTRL               0x112DC8
        /*
        #define HDMI_MCU_FORCE_CTRL_BIT     MBOX_BIT0
        #define HDMI_NONPCM_FROM_SER2       MBOX_HDMI_NONPCM_FROM_ASND_R2_BIT       //MBOX_BIT4
        #define HDMI_NONPCM_FROM_SEDSP      MBOX_HDMI_NONPCM_FROM_ASND_DSP_BIT      //MBOX_BIT5
        */

//---[R2] MBOX_BYTE_STATUS_SELECT in decR2_proj.h---
#define MBOX_BYTE_STATUS_SELECT     0x112E9E

//---g_DDPBypassenable_1, g_DDPBypassenable_2 in halMAD.c---
#define UTPA_SUPPORT_AC3P_INFOTYPE_HDMITX_BYPASS_ENABLE 1
#define DDPBYPASSENABLE_1_DEFAULT                       0
#define DDPBYPASSENABLE_2_DEFAULT                       0

//---check ASND_DSP_DDR_SIZE in ddr_config.h MUST to be sync with _MAD_DSP2_DRAM_SIZE in mhal_audio.c (ALSA kernel)---
//                                                                _MAD_DMA_READER_BASE_OFFSET
//                                                                _MAD_PCM_PLAYBACK2_BASE_OFFSET
//                                                                _MAD_DMA_READER2_BASE_OFFSET
//                                                                _MAD_PCM_CAPTURE1_BASE_OFFSET
//                                                                _MAD_PCM_CAPTURE2_BASE_OFFSET
//                                                                _MAD_PCM_SWMIXER_CLIENT_INFO_BASE_OFFSET
#define UTPA_ASND_DSP_DDR_SIZE_SYNC_WITH_ALSA   1

//--------------------------------------------------------------------------------------
//config for different R2
//--------------------------------------------------------------------------------------
#define R2_SUPPORT_MS12_PCM_RENDER_ALWAYS_ENABLE    1
#define R2_SUPPORT_R2DMA_READER2                    1
#define R2_SUPPORT_R2_DEC_ISR2_EN                   0 // replace R2_DMA_READER2, R2(ADEC1) is using DEC4 for output (See R2_PCM2_DMX__L_OUT in audio_comm2.h)

//--------------------------------------------------------------------------------------
//config for different Chip //@chip bring up
//--------------------------------------------------------------------------------------
#define UTPA_AUDIO_CHIP_TYPE_3R1D           0   //DEC_R2_0:0x112E, DEC_R2_1:0x1603, SND_R2:0x153F
                                                //ADEC(R2)  ID  ES  CH_CFG
                                                //-------------------------
                                                //  1       1-+-1   0x80
                                                //            +-2
                                                //  2       2---3   0x8C
                                                //-------------------------
                                                //  3       3-+-4   0x82
                                                //            +-5
                                                //  4       4---6   0x83
                                                //-------------------------
                                                //ADEC_ATV  5       0x83

#define UTPA_AUDIO_CHIP_TYPE_2R1D           0   //DEC_R2:0x1603, SND_R2:0x112E
#define UTPA_AUDIO_CHIP_TYPE_2R1D_V2        1   //DEC_R2:0x112E, SND_R2:0x1603
#define UTPA_AUDIO_CHIP_TYPE_1R1D           0
#define UTPA_AUDIO_CHIP_TYPE_2D1R           0
#define UTPA_AUDIO_CHIP_TYPE_2D             0
#define UTPA_AUDIO_CHIP_TYPE_1D             0

//---DecID Mapping---
#define ADEC0_DecID         AU_DEC_ID1
#define ADEC0_AD_DecID      AU_DEC_ID2
#define ADEC1_DecID         AU_DEC_ID3
#define ADEC1_AD_DecID      AU_DEC_INVALID
#define ADEC2_DecID         AU_DEC_INVALID
#define ADEC2_AD_DecID      AU_DEC_INVALID
#define ADEC3_DecID         AU_DEC_INVALID
#define ADEC3_AD_DecID      AU_DEC_INVALID
#define ADEC_ATV_DecID      AU_DEC_ID2
#define ASND_DSP_DecID      AU_DEC_ID3

#define ADEC0_FORCE_ID      AUDIO_FORCE_ID1
#define ADEC1_FORCE_ID      AUDIO_FORCE_ID3
#define ADEC2_FORCE_ID      AUDIO_FORCE_INVALID
#define ADEC3_FORCE_ID      AUDIO_FORCE_INVALID
#define ADEC_ATV_FORCE_ID   AUDIO_FORCE_ID2

//---ADEC Array index---
#define ADEC0_INDEX     0
#define ADEC1_INDEX     1
#define ADEC2_INDEX     0
#define ADEC3_INDEX     0
#define ADEC_ATV_INDEX  2
#define ADEC_INDEX_MAX  3

//---ADC Array index---
#define ADC0_INDEX     0
#define ADC1_INDEX     1
#define ADC_INDEX_MAX  2

//---Channel Array index---
#define CH1_INDEX       0
#define CH2_INDEX       1
#define CH3_INDEX       2
#define CH4_INDEX       3
#define CH5_INDEX       4
#define CH6_INDEX       5
#define CH7_INDEX       6
#define CH8_INDEX       7
#define CH_INDEX_MAX    8

//---Channel Array index---
#define LINEOUT0_INDEX      0
#define LINEOUT1_INDEX      1
#define LINEOUT2_INDEX      2
#define LINEOUT3_INDEX      3
#define LINEOUT_INDEX_MAX   4

//---Pcm Capture index---
#define PCMCAPTURE0_INDEX       0
#define PCMCAPTURE1_INDEX       1
#define PCMCAPTURE2_INDEX       2
#define PCMCAPTURE_INDEX_MAX    3

//---REG_BANK---
#define REG_BANK_ADEC1_PLAY                 0x112E98
#define REG_BANK_ADEC1_CODEC                0x112E98
#define REG_BANK_ADEC2_PLAY                 0x112E9A
#define REG_BANK_ADEC2_CODEC                0x112E9A
#define REG_BANK_R2_UART                    0x101EA6
#define REG_BANK_R2_DEC_UART_ENABLE         0x0
#define REG_BANK_R2_SND_UART_ENABLE         0xCCCC
#define REG_BANK_R2_WFI                     0x112E8A
#define REG_BANK_R2_WFI_ENABLE              0x0800
#define REG_BANK_R2_LOG_DBG                 MBOX_BYTE_STATUS_SELECT
#define REG_BANK_R2_SYSTEM_DBG              0x112E8A
#define REG_BANK_ADEC_DSP_DBG               0x112DDC
#define REG_BANK_ADEC_DSP_RESULT1           0x112DFC
#define REG_BANK_ADEC_DSP_RESULT2           0x112DFE
#define REG_BANK_ASND_DSP_DBG               0x112DDC
#define REG_BANK_ASND_DSP_RESULT1           0x112DFC
#define REG_BANK_ASND_DSP_RESULT2           0x112DFE
#define REG_BANK_DEC_R2_VERSION             0x112EAA
#define REG_BANK_SND_R2_VERSION             0x1603AA
#define REG_BANK_SND_R2_SRS                 0x160394
#define REG_BANK_SND_R2_SRS_PARAM_LOG       0x80
#define REG_BANK_SND_R2_ABS3D               0x160392
#define REG_BANK_SND_R2_ABS3D_PARAM_LOG     0x40
#define REG_BANK_SND_R2_DTSVX               0x160394
#define REG_BANK_SND_R2_DTSVX_PARAM_LOG     0x80
#define REG_BANK_SND_R2_CUS2SE1             0x160392
#define REG_BANK_SND_R2_CUS2SE1_PARAM_LOG   0x80
#define REG_BANK_SNDOUT_INPUT_PATH_SEL1     0x112D50
#define REG_BANK_SNDOUT_INPUT_PATH_SEL2     0x112D52
#define REG_BANK_SNDOUT_INPUT_PATH_SEL3     0x112D54
#define REG_BANK_ADEC_DSP_IDMA              0x112A80
#define REG_BANK_ASND_DSP_IDMA              0x112A80

//--------------------------------------------------------------------------------------
//config for different audio common info
//--------------------------------------------------------------------------------------
#define AUDIO_COMMON_INFO_SEDSP_MS12_DDE_DELAY   60  //ms
#define AUDIO_COMMON_INFO_SNDR2_MS12_DDPE_DELAY  110 //ms
#define AUDIO_COMMON_INFO_SNDR2_MS12_DAP_DELAY   50  //ms

//--------------------------------------------------------------------------------------
//config for different customized behavior
//--------------------------------------------------------------------------------------
//---ES Passthrough---
#define CUS_PATCH_PARAM_ES_PASSTHROUGH      1

//---PCM Capture param rptr---
#define CUS_PATCH_PARAM_PCM_CAPTURE1_RPTR   0
#define M2S_MBOX_PCM_CAPTURE_DDR_RdPtr      0x112DD4

#define CUS_PATCH_PARAM_PCM_CAPTURE2_RPTR   0
#define M2S_MBOX_PCM_CAPTURE2_DDR_RdPtr     0x112D38

//---SPDIF info NPCM wptr---
#define CUS_PATCH_INFO_SPDIF_NPCM_WPTR      0
#define DSP2DmAddr_nonpcm_capture_wptr      0x3FAF

//---PCM Capture info wptr---
#define CUS_PATCH_INFO_PCM_CAPTURE1_WPTR    0
#define S2M_MBOX_PCM_CAPTURE_DDR_WrPtr      0x112DF0

#define CUS_PATCH_INFO_PCM_CAPTURE2_WPTR    0
#define S2M_MBOX_PCM_CAPTURE2_DDR_WrPtr     0x112DF4

//---HDMI RX BYPASS---
#define CUS_PATCH_PARAM_HDMI_RX_BYPASS      1
#define REG_AUDIO_SPDIF_OUT_CFG             0x112C8A
#define REG_AUDIO_SPDIF2_OUT_CFG            0x112C5E

//---Low Latency Limiter---
#define CUS_PATCH_PARAM_LOW_LATENCY         0

//---DOLBY_ATMOS_CONTROL---
#define CUS_PATCH_PARAM_DOLBY_ATMOS_CTRL    0

//---MCU dump R2 log---
#define CUS_PATCH_PARAM_MCU_DUMP_R2_LOG     0
#define CUS_PATCH_INFO_MCU_DUMP_R2_LOG      0

//---ES Repeat Play---
#define CUS_PATCH_PARAM_ES_REPEAT_PLAY      0

//---SDM L/R swap---
// Sigma Delta Modulation swap
// 0x112B_52[11:8], 0: Disable, 1: Enable
//           8:  lineout 0
//           9:  lineout 1
//           10: lineout 2
//           11: lineout 3
#define CUS_PATCH_PARAM_SDM_SWAP            0
#define REG_CODEC_SDM_SWAP                  0x112B52

//---ASND DSP DDR size---
#define ASND_DSP_DDR_SIZE                   0x300000  //@chip bring up

//---DEC PCM1 buffer info---
#define CUS_PATCH_INFO_ADEC0_PCM_BUFFER     1
#define OFFSET_ADEC0_PCM_DRAM_ADDR          0x01E0000 //OFFSET_PCM1_DRAM_ADDR //@chip bring up
#define ADEC0_PCM_DRAM_SIZE                 0x1FE000  //PCM1_DRAM_SIZE //@chip bring up

//---DEC PCM2 buffer info---
#define CUS_PATCH_INFO_ADEC1_PCM_BUFFER     0
#define OFFSET_ADEC1_PCM_DRAM_ADDR          0x0048000 //OFFSET_PCM2_DRAM_ADDR //@chip bring up
#define ADEC1_PCM_DRAM_SIZE                 0xFF000   //PCM2_DRAM_SIZE //@chip bring up

//---SPDIF NPCM buffer info---
#define CUS_PATCH_INFO_SPDIF_NPCM_BUFFER    1
#define OFFSET_SPDIF_NONPCM_DRAM_BASE       0x03DF000 //@chip bring up
#define SPDIF_NONPCM_DRAM_SIZE              0x66000   //@chip bring up
#define SPDIF_NPCM_BUFFER_RW_PTR_IN_MBOX    0
#define S2M_MBOX_SPDIF_NPCM_WPTR            0x112DEC  //@chip bring up
#define SPDIF_NPCM_BUFFER_RW_PTR_IN_DM      1
#define DSP2DmAddr_spdif_npcm_rdptr         0x0
#define DSP2DmAddr_spdif_npcm_wrptr         0x3E92    //@chip bring up

//---HDMI NPCM buffer info---
#define CUS_PATCH_INFO_HDMI_NPCM_BUFFER     1
#define OFFSET_HDMI_NONPCM_DRAM_BASE        0x0448000 //@chip bring up
#define HDMI_NONPCM_DRAM_SIZE               0x198000  //@chip bring up
#define HDMI_NPCM_BUFFER_RW_PTR_IN_MBOX     0
#define S2M_MBOX_HDMI_NPCM_RPTR             0x0
#define S2M_MBOX_HDMI_NPCM_WPTR             0x112DEA  //@chip bring up
#define HDMI_NPCM_BUFFER_RW_PTR_IN_DM       1
#define DSP2DmAddr_hdmi_npcm_rdptr          0x0
#define DSP2DmAddr_hdmi_npcm_wrptr          0x3E93    //@chip bring up

//---MS12 META DATA buffer info---
#define CUS_PATCH_INFO_MS12_DDPE_METADATA_BUFFER    0
#define OFFSET_DDPENC_METADATA_DRAM_ADDR            0x030C000
#define DDPENC_METADATA_DRAM_SIZE                   0x03100
#define S2M_MBOX_DDPE_METADATA_WPTR                 0x112DEC

//---ADEC0 ES buffer info---
#define CUS_PATCH_INFO_ADEC0_ES_BUFFER  1
#define ADEC0_ES_DRAM_SIZE              0x20000 //ES1_DRAM_SIZE //@chip bring up

//---ADEC1 ES buffer info---
#define CUS_PATCH_INFO_ADEC1_ES_BUFFER  1
#define ADEC1_ES_DRAM_SIZE              0x20000 //ES3_DRAM_SIZE //@chip bring up

//---ADEC AC3 ES dump info---
#define CUS_PATCH_INFO_ADEC_AC3_ES_DUMP  0

//---SE-DSP Force MS12 DD/DDP Encode without main sound---
#define CUS_PATCH_PARAM_SEDSP_MS12_ENC_WO_MAIN_SND   0

//---SND-R2 Force MS12 DD/DDP Encode without main sound---
#define CUS_PATCH_PARAM_SNDR2_MS12_ENC_WO_MAIN_SND   1

#define MBOX_BYTE_DEC_SPDIF_SEL 0x160396
        /*
        1R1D:
            [7:5] SPDIF      / HDMI owner (MCU inform R2 to decide who is encode owner)
            000:  DEC-R2
            001:  SND-R2
            010:  SE-DSP-DD  / SE-DSP-DD
            011:  SE-DSP-PCM / SE-DSP-DDP

            [4:0] raw decoder selct

        2R1D:
            [7:5] SPDIF      / HDMI owner (MCU inform R2 to decide who is encode owner)
            000:  DEC-R2
            001:  SND-R2
            010:  SE-DSP-DD  / SE-DSP-DD
            011:  SE-DSP-DD  / SND-R2-DDP

            [4:0] raw decoder selct
        */

//---HDMI RX swtich to PCM Debounce---
#define CUS_PATCH_PARAM_HDMI_RX_SWITCH_DEBOUNCE          1

//---HDMI RX Monitor Event CallBack---
#define CUS_PATCH_PARAM_HDMI_RX_MONITOR_EVENT_CALLBACK   1

//---Frame Decode cmd, PCM Buffer info---
#define CUS_PATCH_PARAM_FRAMEDECODE  0
#define CUS_PATCH_INFO_FRAMEDECODE   0

//---SPDIF_TX / HDMI_TX pipeline delay ---
#define CUS_PATCH_INFO_SPDIFTX_PIPELINE_DELAY   0
#define CUS_PATCH_INFO_HDMITX_PIPELINE_DELAY    0
#define DSP2XboxAddr_GetHdmiEncodeDelay         0xB86F
#define DSP2XboxAddr_mainAudioDelay             0xB801
#define DSP2XboxAddr_spdifDelay                 0xB802
#define DSP2XboxAddr_hdmiDelay                  0xB807

//---SW DMA info---
#define CUS_PATCH_INFO_SWDMA1_SAMPLE_PERCHANNEL      0
#define CUS_PATCH_INFO_MCH_SWDMA_SAMPLE_PERCHANNEL   0

//---UTPA_SUPPORT_SPDIF_RX_MONITOR---
#define REG_AUDIO_STATUS_SPDIF_IN_FREQ  0x112C0E //[15]:1/0, signal unlock/lock

//--------------------------------------------------------------------------------------
//config for different Cus SE
//--------------------------------------------------------------------------------------
//---CUS1SE1---
#define UTPA_SUPPORT_CUS1SE1                0
#define CUS1SE1_SOUND
#define CUS1SE1_AUTO_VOLUME_CTRL
#define CUS1SE1_WIDE_STEREO_CTRL
#define CUS1SE1_WALL_FILTER_CTRL
#define CUS1SE1_HEADROOM_GAIN_CTRL
#define CUS1SE1_GAIN_ADJUSTMENT_CTRL
#define CUS1SE1_HARD_LIMITER_CTRL
#define CUS1SE1_MODE_IDENTIFICATION_CTRL

//---CUS2SE1---
#define UTPA_SUPPORT_CUS2SE1                0
#define CUS2SE1_FN_MODE_REGISTER
#define CUS2SE1_PARAM_RESET
#define CUS2SE1_INIT_FLAG
#define CUS2SE1_FN_NUMBER_OF_OUTPUTS
#define CUS2SE1_FN_OUT_SAMPLING_FREQUENCY
#define CUS2SE1_FNMAIN_INIT
#define CUS2SE1_FNMAIN_VARIABLES
#define CUS2SE1_FN000_INIT
#define CUS2SE1_FN000_VARIABLES
#define CUS2SE1_FN001_INIT
#define CUS2SE1_FN001_VARIABLES
#define CUS2SE1_FN004_MODE1_VARIABLES
#define CUS2SE1_FN004_MODE2_VARIABLES
#define CUS2SE1_FN008_INIT
#define CUS2SE1_FN008_VARIABLES
#define CUS2SE1_FN009_INIT
#define CUS2SE1_FN009_VARIABLES
#define CUS2SE1_FN010_INIT
#define CUS2SE1_FN010_VARIABLES
#define CUS2SE1_FN010_OUT
#define CUS2SE1_FN011_INIT
#define CUS2SE1_FN011_VARIABLES
#define CUS2SE1_FN014_INIT
#define CUS2SE1_FN014_VARIABLES
#define CUS2SE1_FN014_OUT
#define CUS2SE1_FN016_INIT
#define CUS2SE1_FN016_VARIABLES
#define CUS2SE1_FN017_VARIABLES
#define CUS2SE1_FN018_INIT
#define CUS2SE1_FN018_VARIABLES
#define CUS2SE1_FN019_INIT
#define CUS2SE1_FN019_VARIABLES
#define CUS2SE1_FN020_VARIABLES
#define CUS2SE1_FN022_INIT
#define CUS2SE1_FN022_VARIABLES
#define CUS2SE1_FN023_INIT
#define CUS2SE1_FN023_VARIABLES
#define CUS2SE1_FN024_INIT
#define CUS2SE1_FN024_VARIABLES
#define CUS2SE1_FN026_INIT
#define CUS2SE1_FN026_VARIABLES
#define CUS2SE1_FN027_INIT
#define CUS2SE1_FN027_VARIABLES
#define CUS2SE1_FN028_VARIABLES
#define CUS2SE1_FN029_VARIABLES

//--------------------------------------------------------------------------------------
//config for different customized internal patch
//--------------------------------------------------------------------------------------
//---ADEC set mute---
#define SELF_PATCH_PARAM_ADEC_SET_MUTE_EN 1

//---MS12 HDMI TX PCM ouput Auto Delay (for sync with NPCM)---
#define SELF_PATCH_PARAM_MS12_HDMI_TX_PCM_AUTO_AUDIO_DELAY_ENABLE   0
#define REG_DSP_XBOX_MISC                                           0x112E36

//input: MS12 DD/DDP/DP, HDMI output type: DD
#define SELF_PATCH_MS12_HDMI_TX_PCM_DD_IN_AUTO_AUDIO_DELAY     100   //ms
#define SELF_PATCH_MS12_HDMI_TX_PCM_DDP_IN_AUTO_AUDIO_DELAY    100   //ms
#define SELF_PATCH_MS12_HDMI_TX_PCM_DP_IN_AUTO_AUDIO_DELAY     100   //ms

//---General Buffer Dump---
#define SELF_PATCH_PARAM_GENERAL_BUFFER_DUMP  1

//---SNDR2_MS12_PCMR_METADATA_SELECT---
#define SELF_PATCH_PARAM_SNDR2_MS12_PCMR_METADATA_SELECT_EN 1
#define SNDR2_MS12_MBOX_PCMR_METADATA_SELECT                0x112E92

//---CH set gain---
#define SELF_PATCH_PARAM_CH_SET_GAIN_EN 1

//---Start decoding with metadata select to DEC---
#define SELF_PATCH_ADEC_START_METADATA_SELECT_DEC    0

//---HW SRC Output Gain Compensate---
#define SELF_PATCH_HW_SRC_GAIN_COMPENSATE_I2S_OUT    13
#define SELF_PATCH_HW_SRC_GAIN_COMPENSATE_SPDIF_OUT  13
#define SELF_PATCH_HW_SRC_GAIN_COMPENSATE_LINE_0_OUT 13
#define SELF_PATCH_HW_SRC_GAIN_COMPENSATE_LINE_1_OUT 13
#define SELF_PATCH_HW_SRC_GAIN_COMPENSATE_LINE_2_OUT 13
#define SELF_PATCH_HW_SRC_GAIN_COMPENSATE_LINE_3_OUT 13
#define SELF_PATCH_HW_SRC_GAIN_COMPENSATE_HDMI_OUT   13

//---HW SRC Input Gain Compensate---
#define SELF_PATCH_HW_SRC_GAIN_COMPENSATE_CH5_IN     0
#define SELF_PATCH_HW_SRC_GAIN_COMPENSATE_CH6_IN     0
#define SELF_PATCH_HW_SRC_GAIN_COMPENSATE_CH8_IN     0

//---HW SRC PCM Cpature Gain Compensate---
#define SELF_PATCH_HW_SRC_GAIN_COMPENSATE_PCM_CAP    0

//---DDP_71 Auto Bypass---
#define SELF_PATCH_DDP_71_AUTO_BYPASS_ENABLE         1
#define SELF_PATCH_DDP_71_AUTO_BYPASS_BROWSE_CMD     0
#define SELF_PATCH_DDP_71_AUTO_BYPASS_STB_HDMI_TX    0
#define SELF_PATCH_DDP_71_AUTO_BYPASS_TV_HDMI_ARC    1

//---Avoid ADEC Pop Noise---
#define SELF_PATCH_AVOID_ADEC_STOP_POP_NOISE     1
#define SELF_PATCH_AVOID_ADEC_PAUSE_POP_NOISE    0

//---Play/pause/OpenDecodeSystem Cmd Delay---
#define SELF_PATCH_SET_PLAY_CMD_BY_ADEC_SOURCE_DELAY 1
#define SELF_PATCH_PAUSE_DECODING_DELAY              1
#define SELF_PATCH_OPEN_DECODE_SYSTEM_DELAY          1

//---DDPE SRS/DAP/ABS3D Auto Bypass---
#define SELF_PATCH_DDPE_SRS_AUTO_BYPASS_ENABLE   0
#define SELF_PATCH_DDPE_DAP_AUTO_BYPASS_ENABLE   0
#define SELF_PATCH_DDPE_ABS3D_AUTO_BYPASS_ENABLE 0

//---LIMITED AUDIO_DELAY for DDPE--- [SE-DSP] AUDIO_DELAY_LOWER_BOUND in audio_comm2.h
#define SELF_PATCH_LIMITED_AUDIO_DELAY_ENABLE   1
#define AUDIO_DELAY_LOWER_BOUND                 0x20

//---R2_Active_Monitor---
#define SELF_PATCH_R2_ACTIVE_MONITOR_ENABLE 0
#define REG_DECR2_ACK1                      0xFFFFFF
#define REG_DECR2_ACK1_RESPONSE             0xFF

//---Initial Codec to AC3---
#define SELF_PATCH_ADEC0_INITIAL_CODEC_TYPE_AC3_ENABLE   1

//---Initial channel mute---
#define SELF_PATCH_INITIAL_FWM0_INPUT_CH5_bMute   FALSE
#define SELF_PATCH_INITIAL_FWM0_INPUT_CH6_bMute   FALSE
#define SELF_PATCH_INITIAL_FWM0_INPUT_CH8_bMute   FALSE

//---Auto_Set_SPDIF_HDMI_TX_OutputType---
#define SELF_PATCH_AUTO_SET_SPDIF_HDMI_TX_OUTPUTTYPE    0

//---Initial Dolby DRC mode---
#define SELF_PATCH_ADEC0_INITIAL_DOLBY_DRC_LINE_MODE    1

//---ADC Fast charge---
#define SELF_PATCH_STR_RESUME_ADC_FAST_CHARGE_ENABLE    0
#define REG_ADC_FAST_CHARGE                             0x112CEE

//---STR Suspend control R2 & DSP---
#define SELF_PATCH_STR_SUSPEND_R2_DSP_ENABLE_CTRL       0
#define REG_AUDIO_SYNTH_EXPANDER    0x112CB2
#define REG_R2_0_CTRL               0x112980
#define REG_R2_1_CTRL               0x163080
#define REG_SE_IDMA_CTRL0           0x112A80

//---STR_RESUME_All_SE_PARAM_ENABLE---
#define SELF_PATCH_STR_RESUME_All_SE_PARAM_ENABLE       0

//---STR_RESUME_START_DECODE---
#define SELF_PATCH_STR_RESUME_START_DECODE              0

//---Register control for ADEC connect / CH connect---
#define SELF_PATCH_REG_CTRL_ADEC_CONNECT 1
#define SELF_PATCH_REG_CTRL_CH_CONNECT   1
#define REG_AUDIO_DECODER1_CFG  0x112C60 //high(0x112C61): REG_AUDIO_DECODER3_CFG   , low(0x112C60): REG_AUDIO_DECODER1_CFG
#define REG_AUDIO_DECODER2_CFG  0x112C62 //high(0x112C63): REG_AUDIO_DECODER4_CFG   , low(0x112C62): REG_AUDIO_DECODER2_CFG
#define REG_AUDIO_DECODER3_CFG  0x112C61
#define REG_AUDIO_DECODER4_CFG  0x112C63
#define REG_AUDIO_DECODER5_CFG  0x112C25 //high(0x112C25): REG_AUDIO_DECODER5_CFG   ,
#define REG_AUDIO_CH1_CFG       0x112C64 //high(0x112C65): REG_AUDIO_CH5_CFG        , low(0x112C64): REG_AUDIO_CH1_CFG
#define REG_AUDIO_CH2_CFG       0x112C66 //high(0x112C67): REG_AUDIO_CH6_CFG        , low(0x112C66): REG_AUDIO_CH2_CFG
#define REG_AUDIO_CH3_CFG       0x112C68 //high(0x112C69): REG_AUDIO_CH7_CFG        , low(0x112C68): REG_AUDIO_CH3_CFG
#define REG_AUDIO_CH4_CFG       0x112C6A //high(0x112C6B): REG_AUDIO_CH8_CFG        , low(0x112C6A): REG_AUDIO_CH4_CFG
#define REG_AUDIO_CH5_CFG       0x112C65
#define REG_AUDIO_CH6_CFG       0x112C67
#define REG_AUDIO_CH7_CFG       0x112C69
#define REG_AUDIO_CH8_CFG       0x112C6B

//---SW DMA Reader output---
#define REG_SWDMA1_OUTPUT       0x8C     //SW_DMA_READER1, output by "decoder 5 = 0x8C", disable when HW_DMA_READER2_SUPPORT

//---initial all threads in default---
#define SELF_PATCH_INIT_ALL_THREAD       0

//---Dynamic Thread using---
#define SELF_PATCH_DYNAMIC_THREAD_CTRL   1

//---AV sync info---
#define SELF_PATCH_AV_SYNC_INFO_MONITOR  0

//---set by memcpy---
#define SELF_PATCH_SET_BY_MEMCPY                 1

//---Device Capability support codec---
#define SELF_PATCH_DEVICE_CAP_SUPPORT_CODEC      1

//---HDMI Audio Mute event---
#define SELF_PATCH_HDMI_Audio_MUTE_Enable        0

//---SIF auto sound mode---
#define SELF_PATCH_SIF_BTSC_AutoMute_Enable      0

//---R2_SHM_PARAM_REPORT_MM_TS_PTS_WITH_MSB---
#define SELF_PATCH_REPORT_MM_TS_PTS_WITH_MSB     1

//---I2S_RX_FORMAT_CONFIG---
#define SELF_PATCH_I2S_RX_FORMAT_CONFIG  1
#define REG_AUDIO_INPUT_CFG              0x112C02  //[4]:1/0, Left-justified/I2S-justified

//--------------------------------------------------------------------------------------
//Re-define for all utopia functions
//--------------------------------------------------------------------------------------
//============================================================
// System
//============================================================
#define cwrap_aud_WritePreInitTable MApi_AUDIO_WritePreInitTable
#define cwrap_aud_SetDspBaseAddr    MApi_AUDIO_SetDspBaseAddr
#define cwrap_aud_Initialize        MApi_AUDIO_Initialize
#define cwrap_aud_Monitor           MApi_Audio_Monitor
#define cwrap_aud_GetDspMadBaseAddr MApi_AUDIO_GetDspMadBaseAddr
#define cwrap_aud_ExitAudioSystem   MApi_AUDIO_ExitAudioSystem
#define cwrap_aud_SetPowerOn        MApi_AUDIO_SetPowerOn

//============================================================
// path
//============================================================
#define cwrap_aud_SetOutputInfo     MApi_AUDIO_SetOutputInfo
#define cwrap_aud_InputSwitch       MApi_AUDIO_InputSwitch
#define cwrap_aud_SetSourceInfo     MApi_AUDIO_SetSourceInfo

//=============================================================
// volume
//=============================================================
#define cwrap_aud_SetMixModeMute    MApi_AUDIO_SetMixModeMute
#define cwrap_aud_SetMixModeVolume  MApi_AUDIO_SetMixModeVolume
#define cwrap_aud_SetAbsoluteVolume MApi_AUDIO_SetAbsoluteVolume
#define cwrap_aud_SetMute           MApi_AUDIO_SetMute
#define cwrap_aud_SPDIF_SetMute     MApi_AUDIO_SPDIF_SetMute

//============================================================
// Decoder
//============================================================
#define cwrap_aud_OpenDecodeSystem      MApi_AUDIO_OpenDecodeSystem
#define cwrap_aud_GetDecodeSystem       MApi_AUDIO_GetDecodeSystem
#define cwrap_aud_SetDecodeSystem       MApi_AUDIO_SetDecodeSystem
#define cwrap_aud_ReleaseDecodeSystem   MApi_AUDIO_ReleaseDecodeSystem
#define cwrap_aud_SetMpegInfo           MApi_AUDIO_SetMpegInfo
#define cwrap_aud_GetMpegInfo           MApi_AUDIO_GetMpegInfo
#define cwrap_aud_SetAC3PInfo           MApi_AUDIO_SetAC3PInfo
#define cwrap_aud_GetAC3PInfo           MApi_AUDIO_GetAC3PInfo
#define cwrap_aud_SetAACInfo            MApi_AUDIO_SetAACInfo
#define cwrap_aud_SetCommand            MApi_AUDIO_SetCommand
#define cwrap_aud_SetADOutputMode       MApi_AUDIO_SetADOutputMode
#define cwrap_aud_SetASFParm            MApi_AUDIO_SetASFParm
#define cwrap_aud_SetDecodeCmd          MApi_AUDIO_SetDecodeCmd
#define cwrap_aud_GetMAD_LOCK           MApi_AUDIO_GetMAD_LOCK

//============================================================
// Common Decoder
//============================================================
#if(UTPA_SUPPORT_COMMON_DECODER_API)
#define cwrap_aud_DECODER_Open          MApi_AUDIO_DECODER_Open
#define cwrap_aud_DECODER_Close         MApi_AUDIO_DECODER_Close
#define cwrap_aud_DECODER_Start         MApi_AUDIO_DECODER_Start
#define cwrap_aud_DECODER_Stop          MApi_AUDIO_DECODER_Stop
#define cwrap_aud_DECODER_Set           MApi_AUDIO_DECODER_Set
#define cwrap_aud_DECODER_Get           MApi_AUDIO_DECODER_Get
#define cwrap_aud_DECODER_Read          MApi_AUDIO_DECODER_Read
#define cwrap_aud_DECODER_Write         MApi_AUDIO_DECODER_Write
#define cwrap_aud_DECODER_Flush         MApi_AUDIO_DECODER_Flush
#endif //#if(UTPA_SUPPORT_COMMON_DECODER_API)

//============================================================
// Encode
//============================================================


//=============================================================
// Common Info
//=============================================================
#define cwrap_aud_SetCommAudioInfo      MApi_AUDIO_SetCommAudioInfo
#define cwrap_aud_GetCommAudioInfo      MApi_AUDIO_GetCommAudioInfo
#define cwrap_aud_SetAudioParam2        MApi_AUDIO_SetAudioParam2
#define cwrap_aud_GetAudioInfo2         MApi_AUDIO_GetAudioInfo2

//=============================================================
// Capture
//=============================================================
#define cwrap_aud_PCMCapture_Read       MApi_AUDIO_PCMCapture_Read
#define cwrap_aud_PCMCapture_Init       MApi_AUDIO_PCMCapture_Init
#define cwrap_aud_PCMCapture_Start      MApi_AUDIO_PCMCapture_Start
#define cwrap_aud_PCMCapture_Stop       MApi_AUDIO_PCMCapture_Stop

//============================================================
// ATV
//============================================================
#define cwrap_aud_SIF_SendCmd           MApi_AUDIO_SIF_SendCmd
#define cwrap_aud_SIF_SetBandSetup      MApi_AUDIO_SIF_SetBandSetup
#define cwrap_aud_SIF_SetStandard       MApi_AUDIO_SIF_SetStandard
#define cwrap_aud_SIF_IsPALType         MApi_AUDIO_SIF_IsPALType
#define cwrap_aud_SIF_SetPALType        MApi_AUDIO_SIF_SetPALType
#define cwrap_aud_SIF_SetSoundMode      MApi_AUDIO_SIF_SetSoundMode
#define cwrap_aud_TriggerSifPLL         MApi_AUDIO_TriggerSifPLL
#define cwrap_aud_SIF_GetAudioStatus    MApi_AUDIO_SIF_GetAudioStatus
#define cwrap_aud_SIF_GetSoundMode      MApi_AUDIO_SIF_GetSoundMode
#define cwrap_aud_SIF_StartAutoStandardDetection        MApi_AUDIO_SIF_StartAutoStandardDetection
#define cwrap_aud_SIF_GetResultOfAutoStandardDetection  MApi_AUDIO_SIF_GetResultOfAutoStandardDetection
#define cwrap_aud_SIF_SetThreshold      MApi_AUDIO_SIF_SetThreshold
#define cwrap_aud_SIF_SetPrescale       MApi_AUDIO_SIF_SetPrescale

//=============================================================
// SPDIF
//=============================================================
#define cwrap_aud_SPDIF_Monitor             MApi_Audio_SPDIF_Monitor
#define cwrap_aud_SPDIF_SetMode             MApi_AUDIO_SPDIF_SetMode
#define cwrap_aud_SPDIF_GetMode             MApi_AUDIO_SPDIF_GetMode
#define cwrap_aud_SPDIF_SetSCMS             MApi_AUDIO_SPDIF_SetSCMS
#define cwrap_aud_SPDIF_HWEN                MApi_AUDIO_SPDIF_HWEN
#define cwrap_aud_SPDIF_ChannelStatus_CTRL  MApi_AUDIO_SPDIF_ChannelStatus_CTRL
#if(UTPA_SUPPORT_SPDIF_RX_MONITOR)
#define cwrap_aud_SPDIF_Rx_GetNonpcmFlag    MApi_AUDIO_SPDIF_Rx_GetNonpcmFlag
#define cwrap_aud_SPDIF_RX_GetNonPCM        MApi_AUDIO_SPDIF_RX_GetNonPCM
#define cwrap_aud_SPDIF_RX_SetNonpcm        MApi_AUDIO_SPDIF_RX_SetNonpcm
#endif //#if(UTPA_SUPPORT_SPDIF_RX_MONITOR)

//=============================================================
// HDMI
//=============================================================
#ifdef CONFIG_API_HDMITX //TV dont have HDMI TX
#define cwrap_HDMITx_Init                   MApi_HDMITx_Init
#define cwrap_HDMITx_SetAudioConfiguration  MApi_HDMITx_SetAudioConfiguration
#define cwrap_HDMITx_SetAudioOnOff          MApi_HDMITx_SetAudioOnOff
#define cwrap_HDMITx_SetAudioFrequency      MApi_HDMITx_SetAudioFrequency
#define cwrap_HDMITx_SetAudioSourceFormat   MApi_HDMITx_SetAudioSourceFormat
#define cwrap_HDMITx_GetHDCPStatus          MApi_HDMITx_GetHDCPStatus
#endif //#ifdef CONFIG_API_HDMITX

#define cwrap_aud_DigitalOut_SetDeviceCapability    MApi_AUDIO_DigitalOut_SetDeviceCapability
#define cwrap_aud_HDMI_GetNonpcmFlag                MApi_AUDIO_HDMI_GetNonpcmFlag
#define cwrap_aud_HDMI_Tx_GetStatus                 MApi_AUDIO_HDMI_Tx_GetStatus
#define cwrap_aud_HDMI_TX_SetMode                   MApi_AUDIO_HDMI_TX_SetMode
#define cwrap_aud_HDMI_RX_SetNonpcm                 MApi_AUDIO_HDMI_RX_SetNonpcm
#define cwrap_aud_HDMI_RX_GetNonPCM                 MApi_AUDIO_HDMI_RX_GetNonPCM

//=============================================================
// Digital Output Type
//=============================================================
#define cwrap_aud_SYSTEM_Control  MApi_AUDIO_SYSTEM_Control

//=============================================================
// PCM IO
//=============================================================
#define cwrap_aud_PCM_Open  MApi_AUDIO_PCM_Open
#define cwrap_aud_PCM_Close MApi_AUDIO_PCM_Close
#define cwrap_aud_PCM_Start MApi_AUDIO_PCM_Start
#define cwrap_aud_PCM_Stop  MApi_AUDIO_PCM_Stop
#define cwrap_aud_PCM_Set   MApi_AUDIO_PCM_Set
#define cwrap_aud_PCM_Get   MApi_AUDIO_PCM_Get
#define cwrap_aud_PCM_Read  MApi_AUDIO_PCM_Read
#define cwrap_aud_PCM_Write MApi_AUDIO_PCM_Write
#define cwrap_aud_PCM_Flush MApi_AUDIO_PCM_Flush

//=============================================================
// Basic Sound Effect
//=============================================================
#define cwrap_snd_ProcessEnable MApi_SND_ProcessEnable
#define cwrap_snd_SetParam1     MApi_SND_SetParam1
#define cwrap_snd_GetParam1     MApi_SND_GetParam1
#define cwrap_aud_SetPEQCoef    MApi_AUDIO_SetPEQCoef
#define cwrap_aud_SetAvcAT      MApi_AUDIO_SetAvcAT
#define cwrap_aud_SetAvcRT      MApi_AUDIO_SetAvcRT

//=============================================================
// ADV Sound Effect
//=============================================================
#define cwrap_aud_ADVSOUND_ProcessEnable    MApi_AUDIO_ADVSOUND_ProcessEnable
#define cwrap_aud_ADVSOUND_SubProcessEnable MApi_AUDIO_ADVSOUND_SubProcessEnable
#define cwrap_aud_ADVSND_SetParam           MApi_AUDIO_ADVSND_SetParam
#define cwrap_aud_COPY_Parameter            MApi_AUDIO_COPY_Parameter
#if(UTPA_SUPPORT_CUS2SE1)
#define cwrap_aud_ADVSND_SetParam2          MApi_AUDIO_ADVSOUND_SetParam2
#define cwrap_aud_ADVSND_GetParam2          MApi_AUDIO_ADVSOUND_GetParam2
#endif //#if(UTPA_SUPPORT_CUS2SE1)

//=============================================================
// MM
//=============================================================
#define cwrap_aud_GetDDRInfo            MApi_AUDIO_GetDDRInfo
#define cwrap_aud_MM2_initAesInfo       MApi_AUDIO_MM2_initAesInfo
#define cwrap_aud_MM2_checkAesInfo      MApi_AUDIO_MM2_checkAesInfo
#define cwrap_aud_MM2_inputAesFinished  MApi_AUDIO_MM2_inputAesFinished

//=============================================================
// not audio, use for debug
//=============================================================
#define cwrap_VDEC_EX_GetControl    MApi_VDEC_EX_GetControl
#define cwrap_VDEC_EX_GetPTS        MApi_VDEC_EX_GetPTS
#define cwrap_DMX_Stc_Get           MApi_DMX_Stc_Get
#define cwrap_DMX_Stc_Set           MApi_DMX_Stc_Set

//--------------------------------------------------------------------------------------
//config for Cwrap functional block
//--------------------------------------------------------------------------------------
/* Initialize, STR */
#define BUILD_CWRAP_InitializeModule            1   //API_AUDIO_CUSTOMER_InitializeModule
#define BUILD_CWRAP_InitializeModule_IsFinish   1   //API_AUDIO_CUSTOMER_InitializeModule_IsFinish
#define BUILD_CWRAP_STR                         1   //API_AUDIO_CUSTOMER_Suspend
                                                    //API_AUDIO_CUSTOMER_Resume

/* Connect & Disconnect */
#define BUILD_CWRAP_CONNECTION 1    //API_AUDIO_CUSTOMER_SetParserMode
                                    //API_AUDIO_CUSTOMER_ADEC_Connect
                                    //API_AUDIO_CUSTOMER_ADEC_Disconnect
                                    //API_AUDIO_CUSTOMER_ADC_Connect
                                    //API_AUDIO_CUSTOMER_ADC_Disconnect
                                    //API_AUDIO_CUSTOMER_PCM_Mixer_Connect
                                    //API_AUDIO_CUSTOMER_PCM_Mixer_Disconnect
                                    //API_AUDIO_CUSTOMER_CH_Sound_Connect
                                    //API_AUDIO_CUSTOMER_CH_Sound_Disconnect
                                    //API_AUDIO_CUSTOMER_FW_MIXER_Connect
                                    //API_AUDIO_CUSTOMER_FW_MIXER_Disconnect
                                    //API_AUDIO_CUSTOMER_SE_Connect
                                    //API_AUDIO_CUSTOMER_SE_Disconnect
                                    //API_AUDIO_CUSTOMER_SNDOUT_Connect
                                    //API_AUDIO_CUSTOMER_SNDOUT_Disconnect
                                    //API_AUDIO_CUSTOMER_PCM_CAPTURE_Connect
                                    //API_AUDIO_CUSTOMER_PCM_CAPTURE_Disconnect
                                    //API_AUDIO_CUSTOMER_MP3_ENC_Connect
                                    //API_AUDIO_CUSTOMER_MP3_ENC_Disconnect
                                    //API_AUDIO_CUSTOMER_AAC_ENC_Connect
                                    //API_AUDIO_CUSTOMER_AAC_ENC_Disconnect

/* Start & Stop */
#define BUILD_CWRAP_SetCodecType            1   //API_AUDIO_CUSTOMER_SetCodecType
#define BUILD_CWRAP_StartDecoding           1   //API_AUDIO_CUSTOMER_StartDecoding
#define BUILD_CWRAP_StopDecoding            1   //API_AUDIO_CUSTOMER_StopDecoding
#define BUILD_CWRAP_PauseDecoding           1   //API_AUDIO_CUSTOMER_PauseDecoding
#define BUILD_CWRAP_SetMainDecoderOutput    1   //API_AUDIO_CUSTOMER_SetMainDecoderOutput

/* SPDIF */
#define BUILD_CWRAP_SPDIF_TX    1   //API_AUDIO_CUSTOMER_SPDIF_TX_SetOutputType
                                    //API_AUDIO_CUSTOMER_SPDIF_TX_GetOutputType
                                    //API_AUDIO_CUSTOMER_SPDIF_TX_SetCopyInfo
                                    //API_AUDIO_CUSTOMER_SPDIF_TX_SetCategoryCode
                                    //API_AUDIO_CUSTOMER_SPDIF_TX_SetLightOnOff
                                    //API_AUDIO_CUSTOMER_SPDIF_TX_SetMonitorOnOff
                                    //API_AUDIO_CUSTOMER_SPDIF_TX_ChannelStatus_CTRL
#define BUILD_CWRAP_SPDIF_RX    1   //API_AUDIO_CUSTOMER_SPDIF_RX_GetAudioMode
                                    //API_AUDIO_CUSTOMER_SPDIF_RX_SetMonitorOnOff

/* HDMI */
#define BUILD_CWRAP_HDMI_RX 1   //API_AUDIO_CUSTOMER_HDMI_RX_GetAudioMode
                                //API_AUDIO_CUSTOMER_HDMI_RX_GetCopyInfo
                                //API_AUDIO_CUSTOMER_HDMI_RX_SetAudioReturnChannel
                                //API_AUDIO_CUSTOMER_HDMI_RX_SetMonitorOnOff
#define BUILD_CWRAP_HDMI_TX 1   //API_AUDIO_CUSTOMER_HDMI_TX_SetOutputType
                                //API_AUDIO_CUSTOMER_HDMI_TX_SetMonitorOnOff

/* ATV */
#define BUILD_CWRAP_SIF 1   //API_AUDIO_CUSTOMER_SIF_SetInputSource
                            //API_AUDIO_CUSTOMER_SIF_SetHighDevMode
                            //API_AUDIO_CUSTOMER_SIF_SetAudioStandard
                            //API_AUDIO_CUSTOMER_SIF_SetSoundMode
                            //API_AUDIO_CUSTOMER_SIF_GetSoundMode
                            //API_AUDIO_CUSTOMER_SIF_SetMonitorOnOff
                            //API_AUDIO_CUSTOMER_SIF_SetAutoMute
                            //API_AUDIO_CUSTOMER_SIF_StartAutoStandardDetection
                            //API_AUDIO_CUSTOMER_SIF_SetThreshold
                            //API_AUDIO_CUSTOMER_SIF_SetPALType
                            //API_AUDIO_CUSTOMER_SIF_SendCmd
                            //API_AUDIO_CUSTOMER_SIF_GetAudioStatus
                            //API_AUDIO_CUSTOMER_SIF_IsPALType
                            //API_AUDIO_CUSTOMER_SIF_SetPrescale

/* Decoder */
#define BUILD_CWRAP_DECODER 1   //API_AUDIO_CUSTOMER_SetSyncMode
                                //API_AUDIO_CUSTOMER_GetDecodingType
                                //API_AUDIO_CUSTOMER_SetDualMonoOutMode
                                //API_AUDIO_CUSTOMER_GetESInfo
                                //API_AUDIO_CUSTOMER_IsESExist
                                //API_AUDIO_CUSTOMER_SetAudioDescription
                                //API_AUDIO_CUSTOMER_SetTrickMode
                                //API_AUDIO_CUSTOMER_GetBufferStatus
                                //API_AUDIO_CUSTOMER_SetAdecPcmPath
                                //API_AUDIO_CUSTOMER_AdecPcmReady
                                //API_AUDIO_CUSTOMER_AdecPcmGet
                                //API_AUDIO_CUSTOMER_AdecRegisterDecodeDoneCallback
                                //API_AUDIO_CUSTOMER_AdecRegisterEndOfStreamCallback
                                //API_AUDIO_CUSTOMER_AdecRegisterUnderrunCallback
                                //API_AUDIO_CUSTOMER_Auto_Recovery_SetMonitorOnOff

/* Common Decoder */
#define BUILD_CWRAP_COMMON_DECODER  1   //API_AUDIO_CUSTOMER_COMMON_DECODER_Open
                                        //API_AUDIO_CUSTOMER_COMMON_DECODER_Close
                                        //API_AUDIO_CUSTOMER_COMMON_DECODER_Start
                                        //API_AUDIO_CUSTOMER_COMMON_DECODER_Stop
                                        //API_AUDIO_CUSTOMER_COMMON_DECODER_Set
                                        //API_AUDIO_CUSTOMER_COMMON_DECODER_Get
                                        //API_AUDIO_CUSTOMER_COMMON_DECODER_Read
                                        //API_AUDIO_CUSTOMER_COMMON_DECODER_Write
                                        //API_AUDIO_CUSTOMER_COMMON_DECODER_Flush

/* Common */
#define BUILD_CWRAP_SetAudioParam   1   //API_AUDIO_CUSTOMER_SetAudioParam
#define BUILD_CWRAP_GetAudioInfo    1   //API_AUDIO_CUSTOMER_GetAudioInfo

/* Customized patch */
#define BUILD_CWRAP_PATCH_SetAudioParam 1   //API_AUDIO_CUSTOMIZED_PATCH_SetAudioParam
#define BUILD_CWRAP_PATCH_GetAudioInfo  1   //API_AUDIO_CUSTOMIZED_PATCH_GetAudioInfo

/* Customized Unit control */
#define BUILD_CWRAP_UNIT_CTRL   1   //API_AUDIO_CUSTOMIZED_Unit_Ctrl

/* Clip Play for ES */
#define BUILD_CWRAP_CLIP_ES     1   //API_AUDIO_CUSTOMER_PlayClipDecoder
                                    //API_AUDIO_CUSTOMER_StopClipDecoder
                                    //API_AUDIO_CUSTOMER_PauseClipDecoder
                                    //API_AUDIO_CUSTOMER_ResumeClipDecoder

/* Clip Play for PCM */
#define BUILD_CWRAP_CLIP_PCM    1   //API_AUDIO_CUSTOMER_PlayClipMixer
                                    //API_AUDIO_CUSTOMER_StopClipMixer
                                    //API_AUDIO_CUSTOMER_PauseClipMixer
                                    //API_AUDIO_CUSTOMER_ResumeClipMixer

/* Gain, Mute & Delay */
//---Gain---
#define BUILD_CWRAP_Gain    1   //API_AUDIO_CUSTOMER_SetAudioDescriptionGain
                                //API_AUDIO_CUSTOMER_SetPCMMixerInputGain
                                //API_AUDIO_CUSTOMER_SetChannelGain
                                //API_AUDIO_CUSTOMER_SetFWMixerChannelGain
                                //API_AUDIO_CUSTOMER_SetI2SOutGain
                                //API_AUDIO_CUSTOMER_SetLineOutGain
                                //API_AUDIO_CUSTOMER_SetSPDIFOutGain
                                //API_AUDIO_CUSTOMER_SetHDMIOutGain
//---Mute---
#define BUILD_CWRAP_Mute    1   //API_AUDIO_CUSTOMER_SetADECMute
                                //API_AUDIO_CUSTOMER_SetPCMMixerInputMute
                                //API_AUDIO_CUSTOMER_SetChannelMute
                                //API_AUDIO_CUSTOMER_SetFWMixerChannelMute
                                //API_AUDIO_CUSTOMER_SetI2SOutMute
                                //API_AUDIO_CUSTOMER_SetLineOutMute
                                //API_AUDIO_CUSTOMER_SetSPDIFOutMute
                                //API_AUDIO_CUSTOMER_SetHDMIOutMute
                                //API_AUDIO_CUSTOMER_GetI2SOutMuteStatus
                                //API_AUDIO_CUSTOMER_GetLineOutMuteStatus
                                //API_AUDIO_CUSTOMER_GetSPDIFOutMuteStatus
                                //API_AUDIO_CUSTOMER_MuteDuringLimitedTime_Input
                                //API_AUDIO_CUSTOMER_MuteDuringLimitedTime_Output
//---Delay---
#define BUILD_CWRAP_Delay   1   //API_AUDIO_CUSTOMER_SetADECDelay
                                //API_AUDIO_CUSTOMER_SetChannelDelay
                                //API_AUDIO_CUSTOMER_SetAudioDelay
                                //API_AUDIO_CUSTOMER_SetSpdifDelay
                                //API_AUDIO_CUSTOMER_SetHdmiDelay

/* AENC */
#define BUILD_CWRAP_AENC    1   //API_AUDIO_CUSTOMER_AENC_Start
                                //API_AUDIO_CUSTOMER_AENC_Stop
                                //API_AUDIO_CUSTOMER_AENC_RegisterCallback
                                //API_AUDIO_CUSTOMER_AENC_SetInfo
                                //API_AUDIO_CUSTOMER_AENC_GetInfo
                                //API_AUDIO_CUSTOMER_AENC_SetGain
                                //API_AUDIO_CUSTOMER_AENC_CopyData
                                //API_AUDIO_CUSTOMER_AENC_ReleaseData

/* PCM Capture */
#define BUILD_CWRAP_PCMCAPTURE  1   //API_AUDIO_CUSTOMER_PCM_StartUpload
                                    //API_AUDIO_CUSTOMER_PCM_StopUpload
                                    //API_AUDIO_CUSTOMER_PCM_RegisterSendPCMCallback
                                    //API_AUDIO_CUSTOMER_PCM_SetGain
                                    //API_AUDIO_CUSTOMER_PCM_SetMute
                                    //API_AUDIO_CUSTOMER_PCM_SetRequestSizeInMs

/* PCM IO Control */
#define BUILD_CWRAP_COMMON_PCM  1   //API_AUDIO_CUSTOMER_PCM_Open
                                    //API_AUDIO_CUSTOMER_PCM_Close
                                    //API_AUDIO_CUSTOMER_PCM_Start
                                    //API_AUDIO_CUSTOMER_PCM_Stop
                                    //API_AUDIO_CUSTOMER_PCM_Set
                                    //API_AUDIO_CUSTOMER_PCM_Get
                                    //API_AUDIO_CUSTOMER_PCM_Read
                                    //API_AUDIO_CUSTOMER_PCM_Write
                                    //API_AUDIO_CUSTOMER_PCM_Flush

/* MM New Mode */
#define BUILD_CWRAP_MMNewMode   1   //API_AUDIO_CUSTOMER_GetDDRInfo
                                    //API_AUDIO_CUSTOMER_MM2_initAesInfo
                                    //API_AUDIO_CUSTOMER_MM2_checkAesInfo
                                    //API_AUDIO_CUSTOMER_MM2_inputAesFinished
                                    //API_AUDIO_CUSTOMER_MM2_AD_initAesInfo
                                    //API_AUDIO_CUSTOMER_MM2_AD_checkAesInfo
                                    //API_AUDIO_CUSTOMER_MM2_AD_inputAesFinished

/* Mstar Sound Effect */
#define BUILD_CWRAP_MSTAR_SE    1   //API_AUDIO_CUSTOMER_MSTAR_SE_Enable
                                    //API_AUDIO_CUSTOMER_MSTAR_SE_SET_GEQ
                                    //API_AUDIO_CUSTOMER_MSTAR_SE_SET_PEQCoef
                                    //API_AUDIO_CUSTOMER_MSTAR_SE_SET_HPFCoef
                                    //API_AUDIO_CUSTOMER_MSTAR_SE_SET_AVC_MODE
                                    //API_AUDIO_CUSTOMER_MSTAR_SE_SET_AVC_LEVEL
                                    //API_AUDIO_CUSTOMER_MSTAR_SE_SET_AVC_R_MODE_Begin
                                    //API_AUDIO_CUSTOMER_MSTAR_SE_SET_AVC_R_MODE_End
                                    //API_AUDIO_CUSTOMER_MSTAR_SE_SET_AVC_R_MODE_Slope
                                    //API_AUDIO_CUSTOMER_MSTAR_SE_SET_AVC_S_MODE_OFFSET
                                    //API_AUDIO_CUSTOMER_MSTAR_SE_SET_AVC_AttackTime
                                    //API_AUDIO_CUSTOMER_MSTAR_SE_SET_AVC_ReleaseTime
                                    //API_AUDIO_CUSTOMER_MSTAR_SE_SET_DRC_LEVEL
                                    //API_AUDIO_CUSTOMER_MSTAR_SE_SET_BALANCE
                                    //API_AUDIO_CUSTOMER_MSTAR_SE_SET_NR_Threshold
                                    //API_AUDIO_CUSTOMER_MSTAR_SE_GET_NR_Status

/* Advanced Sound Effect */
#define BUILD_CWRAP_DTS_SE              1   //API_AUDIO_CUSTOMER_DTS_SE_Enable
                                            //API_AUDIO_CUSTOMER_DTS_SE_ProcessUnit_Enable
                                            //API_AUDIO_CUSTOMER_DTS_SE_SetParam
                                            //API_AUDIO_CUSTOMER_DTS_SE_SetAEQCoef
#define BUILD_CWRAP_Dolby_DAP           1   //API_AUDIO_CUSTOMER_Dolby_DAP_Enable
                                            //API_AUDIO_CUSTOMER_Dolby_DAP_ProcessUnit_Enable
                                            //API_AUDIO_CUSTOMER_Dolby_DAP_SetParam
#define BUILD_CWRAP_SonicEmotion_ABS3D  1   //API_AUDIO_CUSTOMER_SonicEmotion_ABS3D_Enable
                                            //API_AUDIO_CUSTOMER_SonicEmotion_ABS3D_ProcessUnit_Enable
                                            //API_AUDIO_CUSTOMER_SonicEmotion_ABS3D_SetParam
#define BUILD_CWRAP_DTS_VirtualX        1   //API_AUDIO_CUSTOMER_DTS_VirtualX_Enable
                                            //API_AUDIO_CUSTOMER_DTS_VirtualX_ProcessUnit_Enable
                                            //API_AUDIO_CUSTOMER_DTS_VirtualX_SetParam
                                            //API_AUDIO_CUSTOMER_DTS_VirtualX_SetAppCtrls

/* Debug */
#define BUILD_CWRAP_DBG_MSG             1   //API_AUDIO_CUSTOMER_DebugMenu (for debug messages)
                                            //API_AUDIO_CUSTOMER_DebugMenu_Non_Scanf_ParseCommand
#define BUILD_CWRAP_SINGLE_FUNTION_DBG  1   //AU_CUS_SingleFunctionDebugMenu
#define BUILD_CWRAP_BlockDiagram        1   //AU_CUS_ShowAudioBlockDiagram

//---Bifrost parser---
#if defined(CONFIG_MBOOT)
#define BUILD_CWRAP_BIFROST_DTS_PARSER      0
#define BUILD_CWRAP_BIFROST_WMA_PARSER      0
#define BUILD_CWRAP_BIFROST_XPCM_PARSER     0
#else
#define BUILD_CWRAP_BIFROST_DTS_PARSER      1
#define BUILD_CWRAP_BIFROST_WMA_PARSER      1
#define BUILD_CWRAP_BIFROST_XPCM_PARSER     1
#endif //#if defined(CONFIG_MBOOT)

//---Threads---
#define BUILD_CWRAP_ClipDecoderMonitor              1   //AU_CUS_Thread_ClipDecoderMonitor
#define BUILD_CWRAP_MP3EncodeMonitor                1   //AU_CUS_Thread_MP3EncodeMonitor
#define BUILD_CWRAP_HDMI_RX_Monitor                 1   //AU_CUS_Thread_HDMI_RX_Monitor
#define BUILD_CWRAP_MonitorService                  1   //AU_CUS_Thread_MonitorService
#define BUILD_CWRAP_AACEncodeMonitor                1   //AU_CUS_Thread_AACEncodeMonitor
#define BUILD_CWRAP_MuteDuringLimitedTime_Monitor   1   //AU_CUS_Thread_MuteDuringLimitedTime_Monitor
#define BUILD_CWRAP_AdecPcmOutMonitor               1   //AU_CUS_Thread_AdecPcmOutMonitor
#define BUILD_CWRAP_PCMCaptureMonitor               1   //AU_CUS_Thread_PCMCaptureMonitor
#define BUILD_CWRAP_EndOfStreamMonitor              1   //AU_CUS_Thread_EndOfStreamMonitor
#define BUILD_CWRAP_PCMIO_Play_Monitor              1   //AU_CUS_Thread_Demo_PCMIO_Play_Monitor
#define BUILD_CWRAP_MM_New_Mode_Play_Monitor        1   //AU_CUS_Thread_Demo_MM_New_Mode_Play_Monitor
#define BUILD_CWRAP_MM_New_Mode_AD_Play_Monitor     1   //AU_CUS_Thread_Demo_MM_New_Mode_AD_Play_Monitor
#define BUILD_CWRAP_AV_Sync_Info_Monitor            1   //AU_CUS_Thread_AV_Sync_Info_Monitor
#define BUILD_CWRAP_ADEC_Underrun_Monitor           1   //AU_CUS_Thread_ADEC_Underrun_Monitor
#define BUILD_CWRAP_Auto_Audio_Delay_Monitor        1   //AU_CUS_Thread_INTERNAL_PATCH_Auto_Audio_Delay_Monitor
#define BUILD_CWRAP_General_Dump_Buffer_Monitor     1   //AU_CUS_Thread_INTERNAL_PATCH_General_Dump_Buffer_Monitor
#define BUILD_CWRAP_DDP_71_Auto_Bypass_Monitor      1   //AU_CUS_Thread_INTERNAL_PATCH_DDP_71_Auto_Bypass_Monitor
#define BUILD_CWRAP_Dump_R2_Log_Monitor             1   //AU_CUS_Thread_PATCH_Dump_R2_Log_Monitor
#define BUILD_CWRAP_PrintMsg_Monitor                1   //AU_CUS_Thread_PrintMsg_Monitor
#define BUILD_CWRAP_R2_Active_Monitor               1   //AU_CUS_Thread_INTERNAL_PATCH_R2_Active_Monitor
#define BUILD_CWRAP_FrameDecode_Monitor             1   //AU_CUS_Thread_Demo_FrameDecode_Monitor
#define BUILD_CWRAP_DebugMenuThread                 1   //AU_CUS_CreateDebugMenuThread

#ifdef __cplusplus
}
#endif

#endif //_AUDIO_V2_CUSTOMER_CONFIG_H_

#endif // #ifdef ROLLS_ROYCE
