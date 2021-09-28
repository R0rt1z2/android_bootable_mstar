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
/// file drvMAD.h
/// @brief Subroutine for DVB
////////////////////////////////////////////////////////////////////////////////

#ifndef _DRVMAD_H_
#define _DRVMAD_H_

////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////
#include "drvAUDIO_if.h"
#include "../internal/drvAUDIO_internal.h"

#ifdef __cplusplus
extern "C"
{
#endif

////////////////////////////////////////////////////////////////////////////////
// Define & data type
////////////////////////////////////////////////////////////////////////////////

#define IS_MADGOOD() ( MDrv_MAD_Rpt_DTVES() == 0x41 || MDrv_MAD_Rpt_DTVES() == 0x81 )

//=====================================================
// AUDIO_MAD System Relational Driver Function
//=====================================================
//MS_U32  MDrv_MAD_Read_DSP_sram(MS_U16 dsp_addr,AUDIO_DSP_MEMORY_TYPE dsp_memory_type);
//MS_BOOL MDrv_MAD_Write_DSP_sram(MS_U16 dsp_addr, MS_U32 value, AUDIO_DSP_MEMORY_TYPE dsp_memory_type);
void    MDrv_MAD_Init(void);
void    MDrv_MAD_SetMemInfo(void);
void    MDrv_MAD_DisEn_MIUREQ(void);
void    MDrv_MAD_Dis_MIUREQ(void);
MS_BOOL MDrv_MAD_LoadCode(AUDIO_DSP_CODE_TYPE dspCodeType);
void    MDrv_MAD_SetAdMixMode(AD_MIX_MODE mix_mode, MIX_ONOFF en_mix);

void    MDrv_AUD_Monitor_SPDIF_NONPCM_SmpRate(void);
MS_BOOL MDrv_AUDIO_SetCertMode( Audio_CertMode CertType, Audio_CertEnable enCert );
MS_BOOL MDrv_AUDIO_MM2_initAesInfo( AUDIO_DEC_ID  dec_id );
MS_BOOL MDrv_AUDIO_MM2_checkAesInfo( AUDIO_DEC_ID  dec_id, AES_INFO *aes_info );
MS_BOOL MDrv_AUDIO_MM2_inputAesFinished( AUDIO_DEC_ID dec_id, MS_U32 es_size, MS_BOOL ptsExist, MS_U64 pts );


//=====================================================
// AUDIO_MAD Common decoder Relational Driver Function
//=====================================================
//Multi Player
MS_BOOL MDrv_MAD_DecoderCmd(AUDIO_DEC_ID DecId, AU_DVB_DECCMD u8DecCmd);
MS_BOOL MDrv_MAD_GetAudioCapability(AUDIO_DEC_ID eDecId, AudioDecStatus_t * p_AudioDecStatus);
MS_BOOL MDrv_AUDIO_SetCommAudioInfo( Audio_COMM_infoType infoType, MS_VIRT param1, MS_VIRT param2 );
MS_U64 MDrv_AUDIO_GetCommAudioInfo( Audio_COMM_infoType infoType );

void    MDrv_MAD_SetDecCmd(AU_DVB_DECCMD decCmd);
AU_DVB_DECCMD   MDrv_MAD_GetDecCmd(En_DVB_AudioDecoder AdoDecoderNo);

MS_U8   MDrv_MAD_Rpt_DTVES(void);
//=====================================================
// AUDIO_MAD MEPG Relational Driver Function
//=====================================================
MS_BOOL MDrv_AUDIO_SetMpegInfo( Audio_MPEG_infoType infoType, MS_VIRT param1, MS_VIRT param2 );
MS_U32  MDrv_AUDIO_GetMpegInfo( Audio_MPEG_infoType infoType );

//=====================================================
// AUDIO_MAD Dolby Relational Driver Function
//=====================================================
MS_BOOL MDrv_AUDIO_SetAC3Info( Audio_AC3_infoType infoType, MS_VIRT param1, MS_VIRT param2 );
MS_BOOL MDrv_AUDIO_SetAC3PInfo( Audio_AC3P_infoType infoType, MS_VIRT param1, MS_VIRT param2 );
MS_U32  MDrv_AUDIO_GetAC3Info( Audio_AC3_infoType infoType );
MS_U32  MDrv_AUDIO_GetAC3PInfo( Audio_AC3P_infoType infoType );

//=====================================================
// AUDIO_MAD DTS Relational Driver Function
//=====================================================
MS_BOOL MDrv_AUDIO_SetDTSCommonCtrl( Audio_DTS_infoType infoType, Audio_DTS_CtrlType ctrl_type );
MS_U32  MDrv_AUDIO_GetDTSInfo( Audio_DTS_infoType infoType );

//=====================================================
// AUDIO_MAD WMA Relational Hal Function
//=====================================================
MS_BOOL MDrv_AUDIO_SetWmaInfo( Audio_WMA_infoType infoType, MS_VIRT param1, MS_VIRT param2 );
MS_U32  MDrv_AUDIO_GetWmaInfo( Audio_WMA_infoType infoType );

//=====================================================
// AUDIO_MAD LPCM/XPCM Relational Driver Function
//=====================================================
MS_U8   MDrv_MAD_XPCM_setParam( XPCM_TYPE audioType, MS_U8  channels, MS_U16 sampleRate,
                                MS_U8  bitsPerSample, MS_U16 blockSize, MS_U16 samplePerBlock);

//=====================================================
// AUDIO_MAD RA Relational Driver Function
//=====================================================
MS_U8   MDrv_MAD_RA8_setParam(MS_U16 mNumCodecs, MS_U16 mSamples, MS_U16 mSampleRate,
                                MS_U16* Channels, MS_U16* Regions, MS_U16* cplStart, MS_U16* cplQbits,
                                MS_U16* FrameSize);

//=====================================================
// AUDIO_MAD MPEG Encoder Related Driver Function
//=====================================================
void    MDrv_MAD_SetEncCmd(AU_DVB_ENCCMD u8EncCmd);
void    MDrv_AUDIO_SetEncodeInit(AU_DVB_ENCBR u8EncBitrate, MS_U8 u8EncFrameNum);
void    MDrv_AUDIO_GetEncodeFrameInfo(AU_DVB_ENC_FRAME_INFO *FrameInfo);

//=====================================================
// AUDIO_MAD AAC Relational Driver Function
//=====================================================
MS_U32  MDrv_AUDIO_GetAACInfo( Audio_AAC_infoType infoType );
MS_U32  MDrv_AUDIO_SetAACInfo( Audio_AAC_infoType infoType, MS_VIRT param1, MS_VIRT param2 );

//====== STAOS PIP START 2012/02/02 ========
MS_BOOL MDrv_AUDIO_GetAudioInfo2( AUDIO_DEC_ID DecId, Audio_InfoType infoType, void * pInfo );

//====== STAOS PIP END 2012/02/02 ========

#ifdef __cplusplus
}
#endif

#endif   //#ifndef _DRVMAD_H_