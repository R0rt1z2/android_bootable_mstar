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

#ifndef _MS_DRV_GOP_
#define _MS_DRV_GOP_
#include <apiGFX.h>
#include <apiGOP.h>
#include <MsTypes.h>

typedef struct
{
    MS_U8   m_u8SettingType;//0:mux, 1:layer
    MS_U16  m_u16SettingCounts;
    MS_U16  m_u16Setting0GopIndex;
    MS_U16  m_u16Setting1GopIndex;
    MS_U16  m_u16Setting2GopIndex;
    MS_U16  m_u16Setting3GopIndex;
    MS_U16  m_u16Setting4GopIndex;
}st_gopmux_para;

typedef struct
{
    MS_U32  m_u32PWMPeriod;
    MS_U16  m_u16DivPWM;
    MS_U32  m_u32PWMDuty;
    MS_U8   m_bPolPWM;
    MS_U16  m_u16maxPWM;
    MS_U16  m_u16maxPWMMsb;
    MS_U16  m_u16minPWM;
    MS_U16  m_u16minPWMMsb;
    MS_U16  m_u16PWMPort;
}st_pwm_setting;

typedef struct
{
    MS_U8 topRate;
    MS_U8 bottomRate;
    MS_U8 leftRate;
    MS_U8 rightRate;
    MS_U8 mode;       // 0 : unit_pixel;  1 : unit_percent;
}st_reproduce_para;

void MsDrvGFX_Init(void);
void MsDrvGOP_Init(MS_U8 u8logoGopIdx);
void MsDrvGOP_Show(MS_U8 u8logoGopIdx, GFX_BufferInfo *dst_info,MS_BOOL bHorStretch, MS_BOOL bVerStretch,MS_U16 u16DispX,MS_U16 u16DispY);
void MsDrvGOP_Show1(MS_U8 u8logoGopIdx,GFX_BufferInfo *dst_info , MS_U8 u8HorStretch, MS_U8 u8VerStretch,MS_U16 u16DispX,MS_U16 u16DispY,MS_U32 u32HorStretch_After_scaling , MS_U32 u32VorStretch_After_scaling );
int MsDrvGFX_Blt(GFX_DrawRect *rect,GFX_BufferInfo *src_info, GFX_BufferInfo *dst_info);
void MsDrvGOP_Hide(void);

void MsDrvGE_SetColorFmt(GFX_Buffer_Format *ColorFmt);
int MsDrvGE_GetPixelByte(GFX_Buffer_Format ColorFmt);
MS_U32 MsDrvGE_GetDstPitch(MS_U32 src_pitch, GFX_Buffer_Format ColorFmt);
MS_U32 MsDrvGE_GetBlackColor(GFX_Buffer_Format ColorFmt);

#if ((CONFIG_TV_CHIP == 0) && (ENABLE_ANDROID_BOOT == 1))
MS_BOOL Load_ReproduceRate(st_reproduce_para *reproduce);
MS_BOOL If_Reproduce_By_GWIN(st_reproduce_para *reproduce);
MS_BOOL Set_Rerpoduce_Rect(MS_U16 *posX, MS_U16 *posY, MS_U32 *rect_width, MS_U32 *rect_height, st_reproduce_para *reproduce);
#endif

#endif