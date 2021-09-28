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


#ifndef __MDRV_VOICE_
#define __MDRV_VOICE_
#include "MsHalCm4.h"

typedef struct
{
    int mic_num;
    MBX_AudBitWidth_e mic_bitwidth;
    MBX_AudRate_e mic_audRate;
}MIC_CONFIG_S;

typedef struct
{
    int nScale;
}VP_CONFIG_S;

typedef struct
{
    int nMode;
}VQ_CONFIG_S;

void MDrv_voice_Halt(void);
void MDrv_voice_Run(void);
void MDrv_voice_test_Init(void);
void MDrv_voice_VQ_ONOFF(void);
MS_S32 MDrv_voice_EnableHwAEC(int nEn);
MS_S32 MDrv_voice_EnableI2S(int nEn);
MS_S32 MDrv_voice_ConfigMIC(MIC_CONFIG_S sConfig);
MS_S32 MDrv_voice_ConfigVp(VP_CONFIG_S sConfig);
MS_S32 MDrv_voice_EnableVp(int nEn);
void MDrv_voice_EnableUart(int nEn);
MS_S32 MDrv_voice_ConfigHpf(int nConfig);
MS_S32 MDrv_voice_EnableHpf(int nStage);
MS_S32 MDrv_voice_EnableDa(int nEn);
MS_S32 MDrv_voice_EnableVq(int nEn);
MS_S32 MDrv_voice_ConfigVq(VQ_CONFIG_S sConfig);
MS_S32 MDrv_voice_EnableSinegen(int bEn);
MS_S32 MDrv_voice_Gain(int nGain);
void MDrv_voice_DcOff(void);
int MDrv_voice_updateFw(char* pBufferAddr, unsigned int binsize);
#endif
