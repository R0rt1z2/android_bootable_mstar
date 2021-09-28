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

#ifndef _API_MFE_FRMFORMAT_H_
#define _API_MFE_FRMFORMAT_H_

#include "MsTypes.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum
{
MFE_API_MFE_COLORFORMAT_MSTTILE = 0,
MFE_API_MFE_COLORFORMAT_YUYV = 1,
MFE_API_MFE_COLORFORMAT_YVYU = 2,
MFE_API_MFE_COLORFORMAT_UYVY = 3,
MFE_API_MFE_COLORFORMAT_VYUY = 4,
} MFE_API_MFE_COLORFORMAT;

typedef struct _MS_MfeFrameFormat
{
    MS_PHY Cur_PhyY0;
    MS_U8* Cur_VirY0;
    MS_PHY Cur_PhyC0;
    MS_U8* Cur_VirC0;
    MS_PHY Cur_PhyY1;
    MS_U8* Cur_VirY1;
    MS_PHY Cur_PhyC1;
    MS_U8* Cur_VirC1;

} MS_MfeFrameFormat_t;

typedef struct _MS_MfeEXIFInfo
{
    MS_U32 tmp;
} MS_MfeEXIFInfo_t;

typedef struct _MS_MfeAdvInfo
{
    MS_U32 frame_mode;
    MS_U32 has_exif;
    MS_U32 framerate_change;
    MS_U32 new_framerate;
    MFE_API_MFE_COLORFORMAT colorformat;
} MS_MfeAdvInfo_t;

typedef struct _MS_MfeFrameInfo
{
    MS_U8 starcode[5]; //must be 0x00,0x00,0x00,0x01,0x47
    MS_MfeFrameFormat_t inbuf;
    MS_MfeAdvInfo_t     advbuf;
    //MS_MfeEXIFInfo_t exif_info;
} MS_MfeFrameInfo_t;

#ifdef __cplusplus
}
#endif

#endif //_API_MFE_FRMFORMAT_H_