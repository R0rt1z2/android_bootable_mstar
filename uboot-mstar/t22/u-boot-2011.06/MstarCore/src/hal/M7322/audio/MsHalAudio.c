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
#include <MsOS.h>
#include <MsDebug.h>
#include <MsSystem.h>

#include <apiAUDIO.h>
#include <drvAUDIO_if.h>
#include <drvAUDIO.h>

#define AUDIO_DELAY1MS(x)    mdelay(x)

//=============================================
// Set audio related input & output port connectivity
//=============================================
void MsHalAudio_SetInternalPath(void)
{
        MDrv_AUDIO_SetInputPath(AUDIO_DSP1_DVB_INPUT, AUDIO_PATH_MAIN);      //Set input source to decoder 1

        MDrv_AUDIO_SetInternalPath(INTERNAL_CH8, AUDIO_I2S_OUTPUT);       // TV:2R1D use internal path D
        MDrv_AUDIO_SetInternalPath(INTERNAL_CH8, AUDIO_AUOUT0_OUTPUT);
        MDrv_AUDIO_SetInternalPath(INTERNAL_CH8, AUDIO_AUOUT1_OUTPUT);
        MDrv_AUDIO_SetInternalPath(INTERNAL_CH8, AUDIO_AUOUT2_OUTPUT);
        MDrv_AUDIO_SetInternalPath(INTERNAL_CH8, AUDIO_AUOUT3_OUTPUT);
        MDrv_AUDIO_SetInternalPath(INTERNAL_CH8, AUDIO_SPDIF_OUTPUT);
}

//=============================================
// Set volume for all output port
//=============================================
void MsHalAudio_SetOutputVolume(MS_U16 u16Vol)
{
    MS_U8 u8VolHi = 0 , u8VolLo = 0 ;

    u8VolHi = (MS_U8)(u16Vol>>8);    
    u8VolLo = (MS_U8)(u16Vol&0xFF);

    MApi_AUDIO_SetAbsoluteVolume( AUDIO_T3_PATH_I2S, u8VolHi, u8VolLo );
    MApi_AUDIO_SetAbsoluteVolume( AUDIO_T3_PATH_AUOUT0, u8VolHi, u8VolLo );   //Set volume
    MApi_AUDIO_SetAbsoluteVolume( AUDIO_T3_PATH_AUOUT1, u8VolHi, u8VolLo );   //Set volume
    MApi_AUDIO_SetAbsoluteVolume( AUDIO_T3_PATH_AUOUT2, u8VolHi, u8VolLo );   //Set volume
    MApi_AUDIO_SetAbsoluteVolume( AUDIO_T3_PATH_AUOUT3, u8VolHi, u8VolLo );   //Set volume
    MApi_AUDIO_SetAbsoluteVolume( AUDIO_T3_PATH_SPDIF, u8VolHi, u8VolLo );   //Set volume//AUDIO_T3_PATH_I2S    
}

//=============================================
// Play Melody in this function 
//=============================================
void MsHalAudio_MelodyPlayStart(void)
{
  //======================================================
  // New SRC arch chip , play Music with MM new mode
  //======================================================
    MApi_AUDIO_SetSystem(MSAPI_AUD_DVB_MP3);   // Set Decoder type = MP3
    MApi_AUDIO_MM2_initAesInfo(AU_DEC_ID1);
    AUDIO_DELAY1MS(5);
    MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_FREE_RUN);
    MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_PLAYMM_FILE2);
    AUDIO_DELAY1MS(5);
    MApi_AUDIO_MM2_inputAesFinished(AU_DEC_ID1, 0x10000-0x20, 0, 0);
}

//=============================================
// Mute/unmute all audio output port
//=============================================
void MsHalAudio_SetOutputMute(MS_BOOL bMute)
{
    if(bMute == 1)
    {
        MApi_AUDIO_SetMute(AUDIO_PATH_MAIN, 1);                     
        MApi_AUDIO_SetMute(AUDIO_T3_PATH_AUOUT0, 1);                     
        MApi_AUDIO_SetMute(AUDIO_T3_PATH_AUOUT1, 1);                     
        MApi_AUDIO_SetMute(AUDIO_T3_PATH_AUOUT2, 1);                     
        MApi_AUDIO_SetMute(AUDIO_T3_PATH_AUOUT3, 1);                     
        MApi_AUDIO_SetMute(AUDIO_T3_PATH_I2S, 1);
        MApi_AUDIO_SetMute(AUDIO_T3_PATH_SPDIF, 1);  
    }  
    else
    {
        MApi_AUDIO_SetMute(AUDIO_PATH_MAIN, 0);                     
        MApi_AUDIO_SetMute(AUDIO_T3_PATH_AUOUT0, 0);                     
        MApi_AUDIO_SetMute(AUDIO_T3_PATH_AUOUT1, 0);                     
        MApi_AUDIO_SetMute(AUDIO_T3_PATH_AUOUT2, 0);                    
        MApi_AUDIO_SetMute(AUDIO_T3_PATH_AUOUT3, 0);                     
        MApi_AUDIO_SetMute(AUDIO_T3_PATH_I2S, 0);
        MApi_AUDIO_SetMute(AUDIO_T3_PATH_SPDIF, 0);  
    }
}

//=============================================
// Get ES buffer abs start address
//=============================================
MS_U32 MsHalAudio_GetEsStartAddr(void)
{
    MS_U32 u32EsAddress = 0;

    u32EsAddress = MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_UNI_ES_Base);

    return u32EsAddress;
}


