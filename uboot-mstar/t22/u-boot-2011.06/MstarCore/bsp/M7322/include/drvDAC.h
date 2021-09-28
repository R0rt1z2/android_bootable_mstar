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
/// @brief  DAC Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_DAC_H_
#define _DRV_DAC_H_


#ifdef __cplusplus
extern "C"
{
#endif
#include "UFO.h"
#include "utopia_dapi.h"
//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
typedef enum
{
    RES_720x480I_60Hz = 0,
    RES_720x480P_60Hz,
    RES_720x576I_50Hz,
    RES_720x576P_50Hz,
    RES_1280x720P_50Hz,
    RES_1280x720P_60Hz,
    RES_1920x1080I_50Hz,
    RES_1920x1080I_60Hz,
    RES_1920x1080P_50Hz,
    RES_1920x1080P_60Hz,
    RES_1920x1080P_24Hz,
    RES_1920x1080P_25Hz,
    RES_1920x1080P_30Hz,
    RES_640x480P_60Hz,
    RES_1920x2205P_24Hz,
    RES_1280x1470P_50Hz,
    RES_1280x1470P_60Hz,
    RES_VGA640x480P_60Hz,
    RES_TTL480X272P_60Hz,
    RES_3840x2160P_24Hz,
    RES_3840x2160P_25Hz,
    RES_3840x2160P_30Hz,
    RES_3840x2160P_50Hz,
    RES_3840x2160P_60Hz,
    RES_4096x2160P_24Hz,
    RES_4096x2160P_25Hz,
    RES_4096x2160P_30Hz,
    RES_4096x2160P_50Hz,
    RES_4096x2160P_60Hz,
    RES_3840x2160P420_30Hz,
    RES_3840x2160P420_50Hz,
    RES_3840x2160P420_60Hz,
    RES_4096x2160P420_30Hz,
    RES_4096x2160P420_50Hz,
    RES_4096x2160P420_60Hz,
    RES_1600x1200P_60Hz,
    RES_1440x900P_60Hz,
    RES_1280x1024P_60Hz,
    RES_1024x768P_60Hz,
    RES_1280x720P_24Hz,
    RES_1280x720P_25Hz,
    RES_1280x720P_30Hz,
    RES_1080x1920p_59D246Hz,
    RES_1080x1920p_60Hz,
    RES_1440x2560p_70Hz,
} EN_OUTPUT_VIDEO_TIMING_TYPE;

typedef enum
{
    COLOR_8_BIT,
    COLOR_10_BIT,
    COLOR_12_BIT,
}EN_OUTPUT_BIT_TYPE;

typedef enum
{
    DAC_CLK_FROM_DISP,
    DAC_CLK_FROM_VEDAC,
    DAC_CLK_FROM_DVBC_INN,
    DAC_CLK_FROM_DVBC_EQ,
    DAC_CLK_FROM_ADCD_D,
    DAC_CLK_FROM_DFT,
} EN_DAC_CLK_TYPE;

typedef enum
{
    DAC_TO_HDGEN = 0,   //HD Gen output
    DAC_TO_VE = 1,      //VE output (CVBS + S-Video)
    DAC_TO_VE_YPBPR =3  //VE output (Y/Pb/Pr)
} EN_DAC_OUTPUT_TYPE;

typedef enum
{
    DAC_R_G_B = 0,
    DAC_R_B_G = 1,
    DAC_G_R_B = 2,
    DAC_B_G_R = 4,
    DAC_G_B_R = 5,
    DAC_B_R_G = 6,
    DAC_R_R_R = 7,
} EN_DAC_SWAP_SEL;

typedef enum
{
    DAC_OUTPUT_LEVEL_HIGH = 0,  //1.3V
    DAC_OUTPUT_LEVEL_LOW = 1    //1V
} EN_DAC_MAX_OUTPUT_LEVEL;

typedef enum
{
    DAC_SD_OFF =0,
    DAC_SD_X = 1,
    DAC_SD_Y = 2,
    DAC_SD_XY = 3,
    DAC_SD_C = 4,
    DAC_SD_XC = 5,
    DAC_SD_YC = 6,
    DAC_SD_XYC = 7,

}EN_DAC_SD_ENABLE_CTRL;

typedef enum
{
    DAC_HD_OFF =0,
    DAC_HD_R = 1,
    DAC_HD_G = 2,
    DAC_HD_RG = 3,
    DAC_HD_B = 4,
    DAC_HD_RB = 5,
    DAC_HD_GB = 6,
    DAC_HD_RGB = 7,

}EN_DAC_HD_ENABLE_CTRL;

typedef enum
{
    DAC_DETECT_PLUGIN,       //Detect  Plug_IN
    DAC_DETECT_PLUGOUT,     // Detect Plug_OUT
}EN_DAC_DETECT_TYPE;

typedef enum
{
    DAC_DETECT_HD,       //Detect HD_DAC
    DAC_DETECT_SD,     // Detect SD_DAC
}EN_DAC_DETECT;

typedef enum
{
    DAC_VIDEO_VBI_WSS_START = 0x0000,

    DAC_VIDEO_VBI_WSS_AS_4x3_FULL,
    DAC_VIDEO_VBI_WSS_AS_14x9_LETTERBOX_CENTER,
    DAC_VIDEO_VBI_WSS_AS_14x9_LETTERBOX_TOP,
    DAC_VIDEO_VBI_WSS_AS_16x9_LETTERBOX_CENTER,
    DAC_VIDEO_VBI_WSS_AS_16x9_LETTERBOX_TOP,
    DAC_VIDEO_VBI_WSS_AS_ABOVE16x9_LETTERBOX_CENTER,
    DAC_VIDEO_VBI_WSS_AS_14x9_FULL_CENTER,
    DAC_VIDEO_VBI_WSS_AS_16x9_ANAMORPHIC,

    DAC_VIDEO_VBI_WSS_CGMS_COPYRIGHT,
    DAC_VIDEO_VBI_WSS_CGMS_COPY_PROTECTION,

    DAC_VIDEO_VBI_WSS_CGMS_COPY_FREELY,       //No Copyright Issue
    DAC_VIDEO_VBI_WSS_CGMS_COPY_ONCE,         //VBI_WSS_COPYRIGHT
    DAC_VIDEO_VBI_WSS_CGMS_COPY_NO_MORE,      //VBI_WSS_COPY_PROTECTION
    DAC_VIDEO_VBI_WSS_CGMS_COPY_NEVER,        //VBI_WSS_COPYRIGHT | VBI_WSS_COPY_PROTECTION

    DAC_VIDEO_VBI_WSS525_A_START = 0x0100,

    DAC_VIDEO_VBI_WSS525_A_AS_4x3_NORMAL,
    DAC_VIDEO_VBI_WSS525_A_AS_4x3_LETTERBOX,
    DAC_VIDEO_VBI_WSS525_A_AS_16x9_ANAMORPHIC,                                          //Using in CVBS
    DAC_VIDEO_VBI_WSS525_A_AS_16x9_NORMAL = DAC_VIDEO_VBI_WSS525_A_AS_16x9_ANAMORPHIC,    //Using in YPbPr
    DAC_VIDEO_VBI_WSS525_A_AS_RESERVED,

    DAC_VIDEO_VBI_WSS525_A_CGMS_COPY_FREELY,
    DAC_VIDEO_VBI_WSS525_A_CGMS_COPY_ONCE,
    DAC_VIDEO_VBI_WSS525_A_CGMS_COPY_NO_MORE,
    DAC_VIDEO_VBI_WSS525_A_CGMS_COPY_NEVER,

    DAC_VIDEO_VBI_WSS525_A_APS_NO_APS,
    DAC_VIDEO_VBI_WSS525_A_APS_PSP_CS_OFF,
    DAC_VIDEO_VBI_WSS525_A_APS_PSP_2_LINE_CS,
    DAC_VIDEO_VBI_WSS525_A_APS_PSP_4_LINE_CS,

    DAC_VIDEO_VBI_WSS525_B_START = 0x0200,
    DAC_VIDEO_VBI_WSS525_B_CGMS_RCI,
} EN_DAC_VIDEO_VBI_WSS_TYPE;

typedef enum
{
    VIDEO_VBI_WSS525_A_AS_4x3_NORMAL               = 0x0,
    VIDEO_VBI_WSS525_A_AS_4x3_LETTERBOX            = 0x2,
    VIDEO_VBI_WSS525_A_AS_16x9_ANAMORPHIC          = 0x1,
    VIDEO_VBI_WSS525_A_AS_16x9_NORMAL              = VIDEO_VBI_WSS525_A_AS_16x9_ANAMORPHIC,
    VIDEO_VBI_WSS525_A_AS_RESERVED                 = 0x3,
}EN_VIDEO_VBI_WSS525_A_AS_TYPE;

typedef enum
{
    VIDEO_VBI_WSS_AS_4x3_FULL                    = 0x8,
    VIDEO_VBI_WSS_AS_14x9_LETTERBOX_CENTER       = 0x1,
    VIDEO_VBI_WSS_AS_14x9_LETTERBOX_TOP          = 0x2,
    VIDEO_VBI_WSS_AS_16x9_LETTERBOX_CENTER       = 0xB,
    VIDEO_VBI_WSS_AS_16x9_LETTERBOX_TOP          = 0x4,
    VIDEO_VBI_WSS_AS_ABOVE16x9_LETTERBOX_CENTER  = 0xD,
    VIDEO_VBI_WSS_AS_14x9_FULL_CENTER            = 0xE,
    VIDEO_VBI_WSS_AS_16x9_ANAMORPHIC             = 0x7,
}EN_VIDEO_VBI_WSS_AS_TYPE;

typedef enum
{
    VIDEO_VBI_WSS525_A_CGMS_COPY_FREELY                = 0x0,
    VIDEO_VBI_WSS525_A_CGMS_COPY_ONCE                  = 0x1,   //COPYRIGHT
    VIDEO_VBI_WSS525_A_CGMS_COPY_NO_MORE               = 0x2,   //COPY_PROTECTION
    VIDEO_VBI_WSS525_A_CGMS_COPY_NEVER                 = (VIDEO_VBI_WSS525_A_CGMS_COPY_ONCE|VIDEO_VBI_WSS525_A_CGMS_COPY_NO_MORE),
}EN_VIDEO_VBI_WSS525_A_CGMS_TYPE;

typedef enum
{
    VIDEO_VBI_WSS525_B_CGMS_COPY_FREELY         = 0x0,
    VIDEO_VBI_WSS525_B_CGMS_COPY_ONCE           = 0x1,
    VIDEO_VBI_WSS525_B_CGMS_NO_MORE             = 0x2,
    VIDEO_VBI_WSS525_B_CGMS_NEVER               = (VIDEO_VBI_WSS525_B_CGMS_COPY_ONCE|VIDEO_VBI_WSS525_B_CGMS_NO_MORE),
    VIDEO_VBI_WSS525_B_CGMS_RCI                 = 0x10,
}EN_VIDEO_VBI_WSS525_B_TYPE;

typedef enum
{
    VIDEO_VBI_WSS_CGMS_COPYRIGHT                = 0x1,
    VIDEO_VBI_WSS_CGMS_COPY_PROTECTION          = 0x2,

    VIDEO_VBI_WSS_CGMS_COPY_FREELY              = 0x0,
    VIDEO_VBI_WSS_CGMS_COPY_ONCE                = VIDEO_VBI_WSS_CGMS_COPYRIGHT,
    VIDEO_VBI_WSS_CGMS_COPY_NO_MORE             = VIDEO_VBI_WSS_CGMS_COPY_PROTECTION,
    VIDEO_VBI_WSS_CGMS_COPY_NEVER               = (VIDEO_VBI_WSS_CGMS_COPYRIGHT|VIDEO_VBI_WSS_CGMS_COPY_PROTECTION),
}EN_VIDEO_VBI_WSS_CGMS_TYPE;

typedef enum
{
    VIDEO_VBI_WSS525_A_APS_NO_APS               = 0x0,
    VIDEO_VBI_WSS525_A_APS_PSP_CS_OFF           = 0x2,
    VIDEO_VBI_WSS525_A_APS_PSP_2_LINE_CS        = 0x1,
    VIDEO_VBI_WSS525_A_APS_PSP_4_LINE_CS        = 0x3,
}EN_VIDEO_VBI_WSS525_A_APS_TYPE;

//-------------------------------------------------------------------------------------------------
//  Function Prototype
//-------------------------------------------------------------------------------------------------
#ifdef MDRV_DAC_C
#define MDRV_INTERFACE
#else
#define MDRV_INTERFACE extern
#endif

MDRV_INTERFACE void MDrv_DAC_SetIOMapBase(MS_VIRT virtNPMBase, MS_VIRT virtPMBase);
MDRV_INTERFACE void MDrv_DAC_Init(void);
MDRV_INTERFACE void MDrv_DAC_Enable(MS_BOOL bEnable, MS_BOOL bIsYPbPr);
MDRV_INTERFACE void MDrv_DAC_SetClkInv(MS_BOOL bEnable, MS_BOOL bIsYPbPr);
MDRV_INTERFACE void MDrv_DAC_SetOutputSource(EN_DAC_OUTPUT_TYPE enOutputType, MS_BOOL bIsYPbPr);
MDRV_INTERFACE void MDrv_DAC_SetOutputLevel(EN_DAC_MAX_OUTPUT_LEVEL enLevel, MS_BOOL bIsYPbPr);
MDRV_INTERFACE void MDrv_DAC_SetOutputSwapSel(EN_DAC_SWAP_SEL enSwap,MS_BOOL bIsYPbPr);
MDRV_INTERFACE void MDrv_DAC_SetOutputTiming(EN_OUTPUT_VIDEO_TIMING_TYPE enTiming);
MDRV_INTERFACE void MDrv_ClkSel_Set(EN_OUTPUT_VIDEO_TIMING_TYPE enTiming, EN_OUTPUT_BIT_TYPE enBits);
MDRV_INTERFACE void MDrv_DAC_OnOffSD(EN_DAC_SD_ENABLE_CTRL enBit);
MDRV_INTERFACE EN_DAC_SD_ENABLE_CTRL MDrv_DAC_GetSDStatus(void);
MDRV_INTERFACE void MDrv_DAC_OnOffHD(EN_DAC_HD_ENABLE_CTRL enBit);
MDRV_INTERFACE EN_DAC_HD_ENABLE_CTRL MDrv_DAC_GetHDStatus(void);
MDRV_INTERFACE void MDrv_DAC_Exit(void);
MDRV_INTERFACE void MDrv_DAC_DumpTable(MS_U8 * pTCONTable, MS_U8 u8Tcontype);
MDRV_INTERFACE void MDrv_DAC_SetWSSOnOff(MS_BOOL bEnable);
MDRV_INTERFACE MS_BOOL MDrv_DAC_GetWSSStatus(void);
MDRV_INTERFACE void MDrv_DAC_ResetWSSData(void);
MDRV_INTERFACE MS_BOOL MDrv_DAC_WSS_SetAS(MS_BOOL bEnable, EN_OUTPUT_VIDEO_TIMING_TYPE eVideo_Timing, EN_DAC_VIDEO_VBI_WSS_TYPE eWSS_TYPE);
MDRV_INTERFACE MS_BOOL MDrv_DAC_WSS_SetCGMS(MS_BOOL bEnable, EN_OUTPUT_VIDEO_TIMING_TYPE eVideo_Timing, EN_DAC_VIDEO_VBI_WSS_TYPE eWSS_TYPE);
MDRV_INTERFACE MS_BOOL MDrv_DAC_WSS_SetAPS(MS_BOOL bEnable, EN_OUTPUT_VIDEO_TIMING_TYPE eVideo_Timing, EN_DAC_VIDEO_VBI_WSS_TYPE eWSS_TYPE);

#ifdef UFO_XC_GETOUTPUTINTELACETIMING
//----------------------------------------------------------------
// MDrv_DAC_GetOutputInterlaceTiming - Get InterlaceTiming
// @return: TRUE is InterlaceTiming
//----------------------------------------------------------------
MDRV_INTERFACE MS_BOOL MDrv_DAC_GetOutputInterlaceTiming(void);
#endif
//----------------------------------------------------------------
// DAC - set half output current
// @return none
//----------------------------------------------------------------
MDRV_INTERFACE void MDrv_DAC_SetIHalfOutput(MS_BOOL bEnable, MS_BOOL bIsYPbPr);

//----------------------------------------------------------------
// DAC - set half output current
// @return none
//----------------------------------------------------------------
MDRV_INTERFACE void MDrv_DAC_SetQuartOutput(MS_BOOL bEnable, MS_BOOL bIsYPbPr);

//----------------------------------------------------------------
// MDrv_DAC_SetDACState - Set DAC init state
// @param: bEnable: TRUE for DAC is initialized, FALSE for not initialized
// @param: bIsYPbPr: TRUE for YPbPr, FALSE for CVBS
// @return none
//----------------------------------------------------------------
MDRV_INTERFACE void MDrv_DAC_SetDACState(MS_BOOL bEnable, MS_BOOL bIsYPbPr);

//----------------------------------------------------------------
// MDrv_DAC_GetDACState - Get DAC init state
// @param: bIsYPbPr: TRUE for YPbPr, FALSE for CVBS
// @return: TRUE is DAC is initialized
//----------------------------------------------------------------
MDRV_INTERFACE MS_BOOL MDrv_DAC_GetDACState(MS_BOOL bIsYPbPr);

//----------------------------------------------------------------
// MDrv_DAC_HotPlugDetect - Get DAC HotPlug state
// @param: SelDAC: DAC_DETECT_HD, DAC_DETECT_SD
// @param: DetectType: DAC_DETECT_PLUGIN, DAC_DETECT_PLUGOUT
// @param: bIsSignIn: Report signal is in/out
// @return: TRUE is working successful
//----------------------------------------------------------------
MDRV_INTERFACE MS_BOOL MDrv_DAC_HotPlugDetect(EN_DAC_DETECT SelDAC, EN_DAC_DETECT_TYPE DetectType, MS_BOOL *State);

//----------------------------------------------------------------
// MDrv_DAC_EnableICT - Enable ICT (Image Constraint Token) function
// @bEnable: TRUE: enable, FALSE: disable
// @return: TRUE is working successful
//----------------------------------------------------------------
MDRV_INTERFACE MS_BOOL MDrv_DAC_EnableICT(MS_BOOL bEnable);

//----------------------------------------------------------------
// DAC - set VGA Hsync Vsync
// @return none
//----------------------------------------------------------------
MDRV_INTERFACE void MDrv_DAC_SetVGAHsyncVsync(MS_BOOL bEnable);

#ifdef CONFIG_UTOPIA_PROC_DBG_SUPPORT
//----------------------------------------------------------------
// DAC - Mdebug Echo command
// @return none
//----------------------------------------------------------------
MDRV_INTERFACE void MDrv_DACMdbIoctl_EchoCmd(MDBCMD_CMDLINE_PARAMETER *paraCmdLine);

//----------------------------------------------------------------
// DAC - Mdebug catch log
// @return none
//----------------------------------------------------------------
MDRV_INTERFACE void MDrv_DACMdbIoctl_Cat(MDBCMD_GETINFO_PARAMETER *paraGetInfo);
#endif

#ifdef __cplusplus
}
#endif

#endif // _DRV_HDMITX_H_

