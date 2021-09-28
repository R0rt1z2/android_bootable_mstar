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
/// file   msAPI_XC.h
/// @brief  VE High Layer API Interface
///////////////////////////////////////////////////////////////////////////////////////////////////
//
//****************************************************
// msAPI_VE SW Version: 1.12
// msAPI_VE update date: 2011/05/05 16:10
//****************************************************

#ifndef _MSAPI_VE_H_
#define _MSAPI_VE_H_

#ifdef __cplusplus
extern "C" {
#endif

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define MSAPI_VE_H_OFFSET     (9+6)
#define MSAPI_VE_V_OFFSET     (0+0)

//-------------------------------------------------------------------------------------------------
//  Enum Types
//-------------------------------------------------------------------------------------------------
typedef enum
{
    EN_WSS_4x3_FULL                    = 0x08,
    EN_WSS_14x9_LETTERBOX_CENTER       = 0x01,
    EN_WSS_14x9_LETTERBOX_TOP          = 0x02,
    EN_WSS_16x9_LETTERBOX_CENTER       = 0x0B,
    EN_WSS_16x9_LETTERBOX_TOP          = 0x04,
    EN_WSS_ABOVE16x9_LETTERBOX_CENTER  = 0x0D,
    EN_WSS_14x9_FULL_CENTER            = 0x0E,
    EN_WSS_16x9_ANAMORPHIC             = 0x07,
}EN_VE_WSS_TYPE;

/// Define MPEG video AFD (Active Format Description) information
typedef enum
{
    /// Unknow AFD.
    E_MVD_AFD_UNKNOWN           = 0,
    /// LetterBox 16:9, top posistion.
    E_MVD_AFD_BOX_16X9_TOP      = 2,
    /// LetterBox 14:9, top posistion.
    E_MVD_AFD_BOX_14X9_TOP      = 3,
    /// LetterBox 16:9, cnetre posistion.
    E_MVD_AFD_BOX_16X9          = 4,
    /// Full frame.
    E_MVD_AFD_FULL_FRAME        = 8,
    /// Centre 4:3
    E_MVD_AFD_CENTRE_4x3        = 9,
    /// Centre 16:9
    E_MVD_AFD_CENTRE_16X9       = 10,
    /// Centre 14:9
    E_MVD_AFD_CENTRE_14X9       = 11,
    /// 4:3 (with shoot and protect 14:9 centre).
    E_MVD_AFD_CENTRE_4X3_14X9   = 13,
    /// 16:9 (with shoot and protect 14:9 centre).
    E_MVD_AFD_CENTRE_16X9_14X9  = 14,
    /// 16:9 (with shoot and protect 4:3 centre).
    E_MVD_AFD_CENTRE_16X9_4X3   = 15
} MVD_AFD, *pMVD_AFD;

typedef enum
{
    MVD_FORBIDDEN=0,
    MVD_ASP_1TO1,       //    1 : 1
    MVD_ASP_4TO3,       //    4 : 3
    MVD_ASP_16TO9,      //   16 : 9
    MVD_ASP_221TO100,   // 2.21 : 1
    MVD_ASP_MAXNUM,
} MVD_ASPECT_RATIO_CODE;

typedef enum
{
    E_MSAPI_VE_ASPECT_RATIO_NORMAL           = 0,
    E_MSAPI_VE_ASPECT_RATIO_4_3              = 1,
    E_MSAPI_VE_ASPECT_RATIO_16_9             = 2,
    E_MSAPI_VE_ASPECT_RATIO_CUT_4_3          = 3,
    E_MSAPI_VE_ASPECT_RATIO_CUT_16_9         = 4,
    E_MSAPI_VE_ASPECT_RATIO_FULL             = 5,
}E_MSAPI_VE_ASPECT_RATIO_TYPE;

typedef enum
{
    E_MSAPI_VE_SOURCE_AUTO = 0,
    E_MSAPI_VE_SOURCE_SCALER_OP2,
    E_MSAPI_VE_SOURCE_SCALER_DI2VE,
    E_MSAPI_VE_SOURCE_EXT_BT656,
    E_MSAPI_VE_SOURCE_SCALER_DNR2VE,
    E_MSAPI_VE_SOURCE_MAXNUM,
}E_MSAPI_VE_SOURCE_TYPE;

typedef enum
{
    E_MSAPI_VE_DBG_LEVEL_NONE = 0,
    E_MSAPI_VE_DBG_LEVEL_API,
    E_MSAPI_VE_DBG_LEVEL_DRV,
}E_MSAPI_VE_DBG_LEVEL_TYPE;

typedef enum
{
    /// 3D input type is None
    E_MSAPI_VE_3D_INPUT_MODE_NONE,
    /// 3D input type is Frame Packing
    E_MSAPI_VE_3D_INPUT_FRAME_PACKING,
    /// 3D input type is Frame Alternative
    E_MSAPI_VE_3D_INPUT_FRAME_ALTERNATIVE,
    /// 3D input type is Field Alternative
    E_MSAPI_VE_3D_INPUT_FIELD_ALTERNATIVE,
    /// 3D input type is Line Alternativ
    E_MSAPI_VE_3D_INPUT_LINE_ALTERNATIVE,
    /// 3D input type is Side by Side Half
    E_MSAPI_VE_3D_INPUT_SIDE_BY_SIDE_HALF,
    /// 3D input type is Side by Side Full
    E_MSAPI_VE_3D_INPUT_SIDE_BY_SIDE_FULL,
    /// 3D input type is Top and Bottom
    E_MSAPI_VE_3D_INPUT_TOP_BOTTOM,
    /// 3D input type is Check board
    E_MSAPI_VE_3D_INPUT_CHECK_BORAD,
    /// 3D input type is Pixel Alternative
    E_MSAPI_VE_3D_INPUT_PIXEL_ALTERNATIVE,
} E_MSAPI_VE_3D_INPUT_MODE;

//-------------------------------------------------------------------------------------------------
//  Structures
//-------------------------------------------------------------------------------------------------
/// Define the initial data for XC_Sys init
typedef struct
{
    MS_U32 u32VEMemAddress;                                 ///<msAPI VE Memory Address
    MS_U32 u32VEMemSize;                                    ///<msAPI VE Memory Size
    MS_VE_VIDEOSYS eOutputVideoStd;                         ///<msAPI VE Output Standard
    MS_VE_INPUT_SRC_TYPE eInputSrcType;                     ///<msAPI VE Input Source Type
    MS_VE_OUTPUT_DEST_TYPE eOutputDstType;                  ///<msAPI VE Output Destination Type
    MS_BOOL bBootLogoEnable;                                ///<Flag to control if Boot Logo is Enable and to Bypass some init flow
    MS_BOOL bEnableDacSD;                                   ///<Flag to turn on SD DAC after VE initialization
} MSAPI_VE_INITDATA;

typedef struct
{
    //SD
    MS_VE_WINDOW_TYPE SD_SrcWin;
    MS_VE_WINDOW_TYPE SD_DestWin;
    E_MSAPI_VE_ASPECT_RATIO_TYPE SD_AS;
}MSAPI_VE_DISP_INFO;

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
//init
extern MS_BOOL msAPI_VE_Init(MSAPI_VE_INITDATA stAPI_VE_InitData);

//---------------------------------------------------------------------------------------------
/// @brief: //Enable or Disable SD Output CVBS/S-Video
//---------------------------------------------------------------------------------------------
extern MS_BOOL msAPI_VE_EnableMiscOutput(MS_BOOL bEnable);

//---------------------------------------------------------------------------------------------
/// @brief: Enable/Disable Bluescreen of SD Output
//---------------------------------------------------------------------------------------------
extern void msAPI_VE_SetVideoMute(MS_BOOL bEnable);

//---------------------------------------------------------------------------------------------
/// @brief: VE setmode for (1)video path (2)timing change (3)output mode change
//---------------------------------------------------------------------------------------------
extern MS_BOOL msAPI_VE_SetMode(void);

//---------------------------------------------------------------------------------------------
/// @brief: VE Set Output Standard
//---------------------------------------------------------------------------------------------
extern MS_BOOL msAPI_VE_SetOutputVideoStd(MS_VE_VIDEOSYS eVideoSystem);

//---------------------------------------------------------------------------------------------
/// @brief: Check if msAPI_VE_Init have been invoked
//---------------------------------------------------------------------------------------------
extern MS_BOOL msAPI_VE_CheckInitStatus(void);

//---------------------------------------------------------------------------------------------
/// @brief: msAPI VE enable/disable VE Capture Mode
//---------------------------------------------------------------------------------------------
extern MS_BOOL msAPI_VE_EnableCaptureMode(MS_BOOL bEnable);

//---------------------------------------------------------------------------------------------
/// @brief: VE Set WSS Info
//---------------------------------------------------------------------------------------------
extern void msAPI_VE_SetAFDToVEWSS(MS_BOOL bEn, MVD_AFD u8AFD, MVD_ASPECT_RATIO_CODE u8SrcARC);

//---------------------------------------------------------------------------------------------
/// @brief: //Set VE capture/display window
//---------------------------------------------------------------------------------------------
extern MS_BOOL msAPI_VE_SetWin(MS_VE_WINDOW_TYPE *pstSrcWin, MS_VE_WINDOW_TYPE *pstDstWin);

//---------------------------------------------------------------------------------------------
/// @brief: msAPI VE set Debug Message Level
//---------------------------------------------------------------------------------------------
extern MS_BOOL msAPI_VE_SetDebugLevel(E_MSAPI_VE_DBG_LEVEL_TYPE eDBGLevel);

//---------------------------------------------------------------------------------------------
/// @brief: exit VE
//---------------------------------------------------------------------------------------------
extern void msAPI_VE_Exit(void);

//---------------------------------------------------------------------------------------------
/// @brief: set SD DAC Output Control
//---------------------------------------------------------------------------------------------
extern MS_BOOL msAPI_VE_SetDACOutCTL(E_DAC_SD_ENABLE_CTRL eDacCtrl);

//---------------------------------------------------------------------------------------------
/// @brief: set VE Source Type
//---------------------------------------------------------------------------------------------
extern MS_BOOL msAPI_VE_SetSourceType(E_MSAPI_VE_SOURCE_TYPE eSrcType);

//---------------------------------------------------------------------------------------------
/// @brief: set information of input CCIR
//---------------------------------------------------------------------------------------------
extern MS_BOOL msAPI_VE_SetSourceCCIR656Info(MS_BOOL bIsInterlace, MS_U16 u16FrameRate);

//---------------------------------------------------------------------------------------------
/// @brief: //Set VE Output Control to TVEncoder or CCIR656
//---------------------------------------------------------------------------------------------
extern MS_BOOL msAPI_VE_SetOutputCtrl(MS_VE_OUT_TYPE eOutputType);

//---------------------------------------------------------------------------------------------
/// @brief: set VE Source Type
//---------------------------------------------------------------------------------------------
E_MSAPI_VE_SOURCE_TYPE msAPI_VE_GetSourceType(void);

//---------------------------------------------------------------------------------------------
/// @brief: msAPI VE enable/disable VE Frame Lock
//---------------------------------------------------------------------------------------------
extern MS_BOOL msAPI_VE_EnableFrameLock(MS_BOOL bEnable);

//---------------------------------------------------------------------------------------------
/// @brief: msAPI VE enable/disable VE Force Set Mode
//---------------------------------------------------------------------------------------------
extern MS_BOOL msAPI_VE_ForceSetMode(MS_BOOL bEnable);

//---------------------------------------------------------------------------------------------
/// @brief: msAPI VE Get XC 3D Mode
//---------------------------------------------------------------------------------------------
extern MS_BOOL msAPI_VE_Get3DMode(E_MSAPI_VE_3D_INPUT_MODE en3DInMode);

#ifdef __cplusplus
}
#endif

#endif
