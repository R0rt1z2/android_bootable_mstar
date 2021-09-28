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
/// @brief Subroutine for sound effect
////////////////////////////////////////////////////////////////////////////////

#ifndef _DRVSOUND_H_
#define _DRVSOUND_H_

#include "MsTypes.h"
#include "drvAUDIO_if.h"

#ifdef __cplusplus
extern "C"
{
#endif


enum
{
    SUBWOOFER=0x00,
    VOLUME_BALANCE=0x01,
    SPATIAL=0x02,
    u8BaSS_TREBLE=0x03,
    AUTO_VOLUME=0x04,
    DAC_OUTPUT=0x05,
    SOUND_EFFECT=0x06,
    STEREO_MODE=0x07,
    SOUND_MODE=0x08
};

//=====================================================
// Declare audio function here.
//=====================================================
void    MDrv_SOUND_Init(void);
void    MDrv_SOUND_SetMute(MS_U8 u8Path, MS_BOOL bEnable);
void    MDrv_SOUND_AbsoluteVolume(MS_U8 u8Path, MS_U8 u8Vol1, MS_U8 u8Vol2);
void    MDrv_SOUND_SetPreScale(MS_U8 u8Path, MS_U8 u8Prescale);
void    MDrv_SOUND_SetBalance(MS_U8 u8Balance);
void    MDrv_SOUND_SetBalance_L(MS_U8 u8Balance_L);
void    MDrv_SOUND_SetBalance_R(MS_U8 u8Balance_R);
void    MDrv_SOUND_SetBass(MS_U8 u8Level);
void    MDrv_SOUND_SetDynamicBass(MS_U8 u8Level);
void    MDrv_SOUND_AbsoluteBass(MS_U8 u8Bass);
void    MDrv_SOUND_SetTreble(MS_U8 u8Level);
void    MDrv_SOUND_AbsoluteTreble(MS_U8 u8Treble);
void    MDrv_SOUND_SetEq(MS_U8 u8Band, MS_U8 u8Level);
void    MDrv_SOUND_SetEq7(MS_U8 u8Band, MS_U8 u8Level);

void    MDrv_SOUND_SetADCThreshold(MS_U8 u8Threshold);
void    MDrv_SOUND_SetNRAttenuate(MS_U8 u8Mode);
void    MDrv_SOUND_SetAVCThreshold(MS_U8 u8Level);
void    MDrv_SOUND_SetAvcMode( MS_U8 u8AvcMode );
void    MDrv_SOUND_SetAvcAT( MS_U8 u8AvcAT );
void    MDrv_SOUND_SetAvcRT( MS_U8 u8AvcRT );
void    MDrv_SOUND_PowerDown_Wait(MS_BOOL bEnable);
void    MDrv_SOUND_SetCH1AudioDelay(MS_U8 u8Delay);
void    MDrv_SOUND_PlayMenuSound(void);
void    MDrv_SOUND_SetSurroundXA(MS_U8 u8Mode);
void    MDrv_SOUND_SetSurroundXB(MS_U8 u8Mode);
void    MDrv_SOUND_SetSurroundXK(MS_U8 u8Mode);
void    MDrv_SOUND_SetSurroundLPFGain(MS_U8 u8Mode);

//======================================================
void    MDrv_SOUND_EnableAutoVolume(MS_BOOL bEnAVC);
void    MDrv_SOUND_EnableTone(MS_BOOL bEnTone);
void    MDrv_SOUND_EnableEQ(MS_BOOL bEnEQ);
void    MDrv_SOUND_EnableSurround(MS_BOOL bEnSurround);
void    MDrv_SOUND_EnableNR(MS_U8 u8Level);
void    MDrv_SOUND_EnableBalance(MS_U8 u8Enable);
//================From DrvAudioProcess=======================
void    MDrv_SOUND_SetAdAbsoluteVolume(MS_U8 u8Vol1, MS_U8 u8Vol2);
void    MDrv_SOUND_SetAdMute(MS_BOOL bEnable);
void    MDrv_SOUND_SetPEQCoef(AUDIO_PEQ_COEF *peq_coef);
void    MDrv_SOUND_EnablePEQ(MS_BOOL Enpeq);

void    MDrv_SOUND_SetMixModeMute(AUDIO_SOURCE_INFO_TYPE eSourceType, AUDIO_MIX_VOL_TYPE VolType, MS_BOOL EnMute );
void    MDrv_SOUND_SetMixModeVolume(AUDIO_SOURCE_INFO_TYPE eSourceType, AUDIO_MIX_VOL_TYPE VolType, MS_U8 u8Vol1, MS_U8 u8Vol2  );

MS_BOOL MDrv_SOUND_EnableDcRemove(MS_BOOL EnDcRemove);
MS_BOOL MDrv_SOUND_SetParam(Sound_SET_Type Type, MS_U32 *p_param );
MS_BOOL MDrv_SOUND_GetParam(const MS_S8 *pStrCmd, MS_U32 u32StrCmdLength, MS_S8 *pData, MS_U32 u32DataSize);
MS_BOOL MDrv_SOUND_SoundEffect_SetParam(const MS_S8 *pStrCmd, MS_U32 u32StrCmdLength, MS_S8 *pData, MS_U32 u32DataSize);

MS_BOOL MDrv_SND_ProcessEnable(Sound_ENABLE_Type Type, MS_BOOL enable);
MS_BOOL MDrv_SND_SetParam( Sound_SET_PARAM_Type Type, MS_U16 param1, MS_U16 param2);
MS_U16  MDrv_SND_GetParam( Sound_GET_PARAM_Type Type, MS_U16 param1);

#ifdef __cplusplus
}
#endif


#endif //_DRVSOUND_H_
