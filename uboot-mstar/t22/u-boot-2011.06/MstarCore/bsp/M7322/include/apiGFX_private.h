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
#ifndef _APIGOP_PRIV_H_
#define _APIGOP_PRIV_H_

////////////////////////////////////////////////////////////////////////////////
/// @brief Byte DMA control driver
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C"
{
#endif

//Local define
#define PATCH_T3_SCK_FAIL 1 //[2009.07.02]T3/U3 GE HW issue:set source alpha is max for filtering the background color

#define GFX_DEBUGINFO_LEVEL(a, x)           if (g_apiGFXLocal.u32dbglvl <= a) {MS_DEBUG_MSG(x);}
#define GFX_DBG_HDR     "[GFXDBG]"

#define GFX_DEBUGINFO(x)                    //MS_DEBUG_MSG(x)

#undef GFX_ASSERT
#ifdef GFX_DEBUG
#define GFX_ASSERT(_bool, _f)                 if (!(_bool)) { MS_CRITICAL_MSG(_f); while (1); }
#else
#define GFX_ASSERT(_bool, _f)                 while (0)
#endif // #ifdef SCL_DEBUG

#include "ULog.h"

// Debug Logs, level form low(INFO) to high(FATAL, always show)
// Function information, ex function entry
#define GFX_INFO(x, args...) ULOGI("GE API", x, ##args)
// Warning, illegal paramter but can be self fixed in functions
#define GFX_WARN(x, args...) ULOGW("GE API", x, ##args)
//  Need debug, illegal paramter.
#define GFX_DBUG(x, args...) ULOGD("GE API", x, ##args)
// Error, function will be terminated but system not crash
#define GFX_ERR(x, args...) ULOGE("GE API", x, ##args)
// Critical, system crash. (ex. assert)
#define GFX_FATAL(x, args...) ULOGF("GE API", x, ##args)

typedef struct
{
    GFX_Set_Colorkey*           pColorKey;              //////////ColorKey
    GFX_Set_ROP*                pROP;                   //////////ROP
}GFX_Function;

typedef struct _GFX_SetConfig
{
    MS_BOOL                     bOnePixelMode;          //////////OnePixelMode
    MS_BOOL                     bDither;                //////////Dither
    MS_BOOL                     bVQEnable;              //////////Virual Cmd Queue
    MS_BOOL                     bABLEnable;             //////////Alpha Blending Enable
    GFX_RotateAngle             eRotate;                //////////Rotate
    GFX_Set_Mirror*             pMirror;                //////////Mirror
    MS_U32                      u32TagID;               //////////TagID
    GFX_Set_Clip_Property*      pClip;                  //////////Clip
}GFX_SetConfig;
#ifndef MSOS_TYPE_OPTEE
typedef struct
{
    GFX_FireInfo*               pFireInfo;
    GFX_OvalFillInfo*           pDrawOvalInfo;
}GFX_Set_DrawOvalInfo;

typedef struct
{
    GFX_FireInfo*               pFireInfo;
    MS_S32                      fHandle;
    MS_U8                       charArray[256];
    MS_U32                      u32StrWidth;
    GFX_TextOutInfo*            pTextInfo;
    GFX_FontInfo *              pFonttable;
}GFX_TextOut_V2;
#endif

void            GFXRegisterToUtopia                 (FUtopiaOpen ModuleType);
MS_U32          GFXOpen                             (void** ppInstance, const void* const pAttribute);
MS_U32          GFXClose                            (void* pInstance);
MS_U32          GFXIoctl                            (void* pInstance, MS_U32 u32Cmd, void* pArgs);
#ifdef CONFIG_GFX_UTOPIA10
MS_U16 Ioctl_GFX_Init(void* pInstance, void* pArgs);
MS_U16 Ioctl_GFX_GetCaps(void* pInstance, void* pArgs);
MS_U16 Ioctl_GFX_GetInfo(void* pInstance, void* pArgs);
MS_S32 Ioctl_GFX_LineDraw(void* pInstance, void* pArgs);
MS_S32 Ioctl_GFX_RectFill(void* pInstance, void* pArgs);
MS_S32 Ioctl_GFX_TriFill(void* pInstance, void* pArgs);
MS_S32 Ioctl_GFX_SpanFill(void* pInstance, void* pArgs);
MS_S32 Ioctl_GFX_BitBlt(void* pInstance, void* pArgs);
MS_S32 Ioctl_GFX_TwoSrcBitBlt(void* pInstance, void* pArgs);
MS_S32 Ioctl_GFX_SetABL(void* pInstance, void* pArgs);
MS_S32 Ioctl_GFX_SetConfig(void* pInstance, void* pArgs);
MS_U16 Ioctl_GFX_GetConfig(void* pInstance, void* pArgs);
MS_S32 Ioctl_GFX_Bitmap(void* pInstance, void* pArgs);
MS_S32 Ioctl_GFX_Font(void* pInstance, void* pArgs);
MS_S32 Ioctl_GFX_PowerState(void* pInstance, void* pArgs);
MS_S32 Ioctl_GFX_CB(void* pInstance, void* pArgs);
MS_S32 Ioctl_GFX_MISC(void* pInstance, void* pArgs);
MS_S32 Ioctl_GFX_TLB(void* pInstance, void* pArgs);
MS_S32 Ioctl_GFX_DrawOval(void* pInstance, void* pArgs);
#endif

#ifdef CONFIG_UTOPIA_PROC_DBG_SUPPORT
MS_U32          GFXMdbIoctl                         (MS_U32 u32Cmd, const void* const pArgs);
#endif

#ifdef __cplusplus
}
#endif
#endif
