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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @brief  MStar Graphics Output Path API
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _API_GOPSC_EX_H_
#define _API_GOPSC_EX_H_

#ifdef __cplusplus
extern "C"
{
#endif

#define MSIF_GOPSC_EX_LIBVER              {'0','0'}            //LIB version
#define MSIF_GOPSC_EX_BUILDNUM            {'1','1'}            //Build Number
#define MSIF_GOPSC_EX_CHANGELIST          {'0','0','3','9','1','8','8','7'} //P4 ChangeList Number

typedef enum
{
    EN_Bilinear = 0,
    EN_Cubic
}EN_GOPSC_VScalingMode;

typedef struct
{
    MS_U16 srcWidth;
    MS_U16 srcHeight;
    MS_U16 destWidth;
    MS_U16 destHeight;
    MS_U8 bEnableScaleX;
    MS_U8 bEnableScaleY;
}GOPSC_SizeInfo;

typedef struct
{
    MS_U16 xStart;
    MS_U16 xEnd;
    MS_U16 yStart;
    MS_U16 yEnd;
    MS_U8 bEnableSkipX;
    MS_U8 bEnableSkipY;
}GOPSC_SkipPixelInfo;

typedef struct
{
    MS_U32 u32Version;
    MS_U32 u32Id;
}GOPSC_DeviceInfo;

typedef enum
{
    ///failed
    En_GOPSC_FAIL = 0,
    ///success
    En_GOPSC_OK,
    ///invalid parameter
    En_GOPSC_INVALID_PARAM,
    ///access not allow
    En_GOPSC_NOT_INITIALIZE,
    ///access not allow
    En_GOPSC_NotSupport,
}GOPSC_Result;

typedef enum
{
    None = 0x0,
    SetOutputRateControl = 0x1,
    EnableP2I = 0x2,
    HalfMode = 0x4,
}EN_GOPSCCmd;

typedef enum
{
    EN_Source_GOP,
    EN_Source_OP,
}EN_GOPSC_Source;

typedef struct
{
    EN_GOPSC_VScalingMode en_VScalingMode;
    EN_GOPSC_Source en_Source;
    MS_U16 dlyLineNumber; //(unit : 2-line)
    MS_BOOL P2IEnable;
    MS_BOOL HalfModeEnable; //output frame rate is half input frame rate
    MS_BOOL OutputRateControlEnable;
	MS_BOOL UseP2IFormula; //If GOP use GOPSC result is P2I, scaling factor formula must use change
}GOPSC_Info;

GOPSC_Result MApi_GOPSC_Init(GOPSC_DeviceInfo* DevInfo);
GOPSC_Result MApi_GOPSC_Enable(GOPSC_DeviceInfo* DevInfo, MS_BOOL enable);
GOPSC_Result MApi_GOPSC_GetGOPSCInfo(GOPSC_DeviceInfo* DevInfo, GOPSC_Info* gopscInfo);
GOPSC_Result MApi_GOPSC_SetGOPSCInfo(GOPSC_DeviceInfo* DevInfo, GOPSC_Info* gopscInfo);
GOPSC_Result MApi_GOPSC_SetHVSPSize(GOPSC_DeviceInfo* DevInfo, GOPSC_SizeInfo* sizeInfo, MS_U32 flag);
GOPSC_Result MApi_GOPSC_SetSkipPixel(GOPSC_DeviceInfo* DevInfo, GOPSC_SkipPixelInfo* skippixelInfo, MS_U32 flag);
GOPSC_Result MApi_GOPSC_ScalingDownOnce(GOPSC_DeviceInfo* DevInfo, MS_U8 FrameNum);

//MS_BOOL MApi_GOPSC_SetCommand(EN_GOPSCCmd cmds);
//EN_GOPSCCmd MApi_GOPSC_GetCommand(void);

#ifdef __cplusplus
}
#endif

#endif
