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
/// @brief  CH34 Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __DRV_CH34_H__
#define __DRV_CH34_H__

#ifdef __cplusplus
extern "C"
{
#endif


//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------

/// ATTRIBUTE_INFO_EX version of current CH34 lib
#define ATTRIBUTE_INFO_VERSION                          1
#define SET_DAC_OUTPUT_SWING_VERSION             1
#define SET_SIGMA_DELTA_GAIN_VERSION               1

/// Version string.
#define CH34_DRV_VERSION                /* Character String for DRV/API version             */  \
    MSIF_TAG,                           /* 'MSIF'                                           */  \
    MSIF_CLASS,                         /* '00'                                             */  \
    MSIF_CUS,                           /* 0x0000                                           */  \
    MSIF_MOD,                           /* 0x0000                                           */  \
    MSIF_CHIP,                                                                                  \
    MSIF_CPU,                                                                                   \
    {'C','H','3','4'},                  /* IP__                                             */  \
    {'0','1'},                          /* 0.0 ~ Z.Z                                        */  \
    {'0','1'},                          /* 00 ~ 99                                          */  \
    {'0','0','0','0','0','0','0','0'},  /* CL#                                              */  \
    MSIF_OS


//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

typedef enum
{
    ///failed
    En_Drv_CH34_FAIL = 0,
    ///success
    En_Drv_CH34_OK,
    ///invalid parameter
    En_Drv_CH34_INVALID_PARAM,
    ///access not allow
    En_Drv_CH34_NotSupport,
}CH34_Drv_Result;

typedef enum
{
    E_CH34_VIDEO_SYS_NTSC = 0,     ///< NTSC
    E_CH34_VIDEO_SYS_PAL,          ///< PAL
    E_CH34_VIDEO_SYS_NUM,
} E_CH34_VIDEO_SYS;

typedef enum
{
    E_CH34_AUDIO_SYS_MONO = 0,     ///< MONO
    E_CH34_AUDIO_SYS_BTSC,         ///< BTSC_STEREO
    E_CH34_AUDIO_SYS_BTSC_SAP,     ///< BTSC_SAP
    E_CH34_AUDIO_SYS_A2,           ///< A2
    E_CH34_AUDIO_SYS_NUM,
} E_CH34_AUDIO_SYS;

typedef enum
{
    E_CH34_CH3 = 0,     ///< ch3
    E_CH34_CH4,          ///< ch4
    E_CH34_NUM,
} E_CH34_CHANNEL;

typedef enum{
    E_CH34_SPEC_DEFAULT = 0,               ///< Default
    E_CH34_SPEC_ABNT_NBR_15604,       ///< ABNT_NBR_15604 spec
} E_CH34_SPEC_SELECT;

typedef enum{
    E_CH34_DAC_SWING_LEVEL0 = 0,     ///< Dac output swing level0
    E_CH34_DAC_SWING_LEVEL1,           ///< Dac output swing level1
    E_CH34_DAC_SWING_LEVEL2,           ///< Dac output swing level2
    E_CH34_DAC_SWING_LEVEL3,           ///< Dac output swing level3
} E_CH34_DAC_SWING_LEVEL;

typedef enum
{
    E_CH34_SIGMA_DELTA_GAIN_LEVEL0 = 0,     ///< Sigma Delta Gain level0, 0.9375
    E_CH34_SIGMA_DELTA_GAIN_LEVEL1,           ///< Sigma Delta Gain level1, 0.875
    E_CH34_SIGMA_DELTA_GAIN_LEVEL2,           ///< Sigma Delta Gain level2, 0.75
    E_CH34_SIGMA_DELTA_GAIN_LEVEL3,           ///< Sigma Delta Gain level3, 0.5
} E_CH34_SIGMA_DELTA_GAIN_LEVEL;

typedef enum
{
    CH34_CMD_NONE = 0,
    CH34_CMD_SET_DAC_OUTPUT_SWING,
    CH34_CMD_SET_SIGMA_DELTA_GAIN,
    CH34_CMD_NUMBER,
} CH34_CTRL_ID;

typedef struct{
    MS_U32 u32AttributeInfo_Version;   ///<Version of current structure. Please always set to "ATTRIBUTE_INFO_VERSION" as input
    MS_U16 u16AttributeInfo_Length;    ///<Length of this structure, u16AttributeInfo_Length=sizeof(CH34_ATTRIBUTE_INFO)
    E_CH34_CHANNEL eChannel;
    E_CH34_VIDEO_SYS eVideoSys;
    E_CH34_AUDIO_SYS eAudioSys;
    E_CH34_SPEC_SELECT eSpecSelect;
} CH34_ATTRIBUTE_INFO;

typedef struct{
    MS_U32 u32SetDACOutputSwing_Version;   ///<Version of current structure. Please always set to "SET_DAC_OUTPUT_SWING_VERSION" as input
    MS_U16 u16SetDACOutputSwing_Length;    ///<Length of this structure, u16SetDACOutputSwing_Length=sizeof(CH34_SET_DAC_OUTPUT_SWING)
    E_CH34_DAC_SWING_LEVEL eDACSwingLevel;
} CH34_SET_DAC_OUTPUT_SWING;

typedef struct{
    MS_U32 u32SetSigmaDeltaGain_Version;   ///<Version of current structure. Please always set to "SET_SIGMA_DELTA_GAIN_VERSION" as input
    MS_U16 u16SetSigmaDeltaGain_Length;    ///<Length of this structure, u16SetSigmaDeltaGain_Length=sizeof(CH34_SET_SIGMA_DELTA_GAIN)
    E_CH34_SIGMA_DELTA_GAIN_LEVEL eSigmaDeltaGainLevel;
} CH34_SET_SIGMA_DELTA_GAIN;

#define CH34_TABLE_OFFSET                  4
#define CH34_TABLE_MAX_DEPTH               2000

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

CH34_Drv_Result MDrv_Ch34_Init(void);
CH34_Drv_Result MDrv_Ch34_TurnOnOff(MS_BOOL bOn);
CH34_Drv_Result MDrv_Ch34_SetMode(E_CH34_CHANNEL channel, E_CH34_VIDEO_SYS videosys, E_CH34_AUDIO_SYS audiosys);
CH34_Drv_Result MDrv_Ch34_WriteCH34Table(MS_U8* pTable);
CH34_Drv_Result MDrv_Ch34_SetAttribute(CH34_ATTRIBUTE_INFO* pInfo);
MS_BOOL MDrv_Ch34_GeneralCtrl(MS_U32 u32Cmd, void* pBuf, MS_U32 u32BufSize);

#ifdef __cplusplus
}
#endif


#endif // __DRV_CH34_H__

