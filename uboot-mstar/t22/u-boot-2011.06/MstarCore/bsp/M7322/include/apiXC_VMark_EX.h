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
/// @brief VMark Ex header file
///
////////////////////////////////////////////////////////////////////////////////
#ifndef _API_XC_VMARK_EX_H_
#define _API_XC_VMARK_EX_H_

#include "MsCommon.h"

// Driver
#include "drvXC_IOPort_EX.h"

// Api
#include "apiXC_EX.h"

#ifdef __cplusplus
extern "C"
{
#endif

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------

#define XC_EX_VMARK_PARAMETER_VERSION                   1

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

typedef struct __attribute__((packed))
{
    //versioning
    MS_U32 u32XC_VMark_version;                   ///<Version of current structure.
    MS_U32 u32XC_VMarK_Length;                   ///<Length of this structure, u32XC_VMarK_Length=sizeof(XC_VMARK_PARAMETER)
    //embedding part
    MS_U8 u8WatermarkOn;
    MS_U8 u8FrequencyDistance[3][3];
    MS_U8 u8BackgroundEmbeddingOn;
    MS_U16 u16EmbeddingStrengthThreshold[48];
    MS_U16 u16EmbeddingStrengthThresholdBG[48];
    MS_U16 u16DirectionMax;
    MS_S8 s8StrengthMultiply;
    //rendering part
    MS_U8 u8PayloadSymbols[1920];
    MS_U8 u8SymbolsRows;
    MS_U8 u8SymbolsCols;
    MS_U8 u8SymbolsXpos;
    MS_U8 u8SymbolsYpos;
    MS_U8 u8SymbolSize;
    MS_U16 u16SpacingVert;
    MS_U16 u16SpacingHorz;
    MS_U8 u8SymbolScaleControl;
}XC_EX_VMARK_PARAMETER;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

#ifdef UFO_XC_VMARK
//-------------------------------------------------------------------------------------------------
/// MApi_XC_VMark_SetParameters
/// @param  u8ServiceIndex                       \b IN: Identifies uniquely a video stream inside the decoder. May not be supported if the support of a single decode is sufficient.
/// @param  pstXC_VMarkParameters           \b IN: VMark parameters for HW embedding
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC XC_EX_RETURN_VALUE MApi_XC_EX_VMark_SetParameters(XC_DEVICE_ID *pDeviceId, MS_U8 u8ServiceIndex, XC_EX_VMARK_PARAMETER *pstXC_VMarkParameters);
#endif

#ifdef __cplusplus
}
#endif

#endif /* _API_XC_VMARK_EX_H_ */
