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

#ifndef _MS_API_GEGOP_
#define _MS_API_GEGOP_
#include <apiGFX.h>
#include <apiGOP.h>
#include <ShareType.h>

typedef enum
{
   eTextAlignLeft,
   eTextAlignMiddle,
   eTextAlignRight

} TextAttrib;

typedef enum 
{
	font32X32 = 0,
	font16X16,
	font_max = 7
}FontSize;

typedef struct
{
	MS_U8 font_width;
	MS_U8 font_height;
	MS_U8 space_width;
	MS_U8 interval_width;
	unsigned char *pCharTable;
	unsigned char *pFontWidthTable;
	
}FontInfo;

void MsApiSelectFont(MS_U8 font_choose);
void MsApiDisplay_Logo(MS_U8 u8logoGopIdx, GFX_DrawRect *rect,GFX_BufferInfo *src_info,GFX_BufferInfo *dst_info, MS_BOOL bHorStretch, MS_BOOL bVerStretch,MS_U16 u16DispX,MS_U16 u16DispY);

void MsApiOsdCreate(MS_U8 u8logoGopIdx, GFX_Block* pBlk, MS_PHYADDR u32GopBuffer);

void MsApiOsdDestroy(void);

void MsApiFlushCanvas2Screen(void);

void MsApiDrawRect(GFX_Block* pBlk, GFX_RgbColor color);

void MsApiDrawPixel(GFX_Point p, GFX_RgbColor color);

void MsApiDrawStrText(const char* pStrText, MS_U16 X, MS_U16 Y, GFX_RgbColor color, TextAttrib eTextAttrib);

void MsApiDrawProgress(MS_U16 X, MS_U16 Y, GFX_RgbColor fcolor, MS_U8 per);

void MsApiDrawJPG(GFX_BufferInfo src_info,GFX_Block Blk);

#endif

