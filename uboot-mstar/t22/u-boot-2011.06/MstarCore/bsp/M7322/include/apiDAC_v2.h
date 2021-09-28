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

#ifndef _DAC_V2_H_
#define _DAC_V2_H_

#ifdef __cplusplus
extern "C"
{
#endif
#include "UFO.h"
//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------

/// DAC CMD
typedef enum
{
    //DAC command
    E_DAC_GET_LIBVER,
    E_DAC_GET_INFO,
    E_DAC_GET_STATUS,
    E_DAC_SET_DBG_LEVEL,
    E_DAC_INIT,
    E_DAC_ENABLE,
    E_DAC_SET_CLKINV,
    E_DAC_SET_YPBPR_OUTPUTTIMING,
    E_DAC_SET_OUTPUT_SOURCE,
    E_DAC_SET_OUTPUT_LEVEL,
    E_DAC_SET_OUTPUT_SWAPSEL,
    E_DAC_ONOFF_SD,
    E_DAC_ONOFF_HD,
    E_DAC_CLKSEL,
    E_DAC_DUMP_TABLE,
    E_DAC_EXIT,
    E_DAC_SET_IHALF_OUTPUT,
    E_DAC_SET_QUART_OUTPUT,
    E_DAC_SET_DAC_STATE,
    E_DAC_HOTPLUG_DETECT,
    E_DAC_SET_POWER_STATE,
    E_DAC_GET_GETOUTPUTINTERLACETIMING,
    E_DAC_SET_WSSONOFF,
    E_DAC_RESET_WSSDATA,
    E_DAC_SET_WSSOUTPUT,
    E_DAC_GET_SD_STATUS,
    E_DAC_GET_HD_STATUS,
    E_DAC_GET_WSSSTATUS,
    E_DAC_ENABLE_ICT,
    E_DAC_SET_VGA_HSYNCVSYNC,
    E_DAC_GET_CHIPCAPS,
    E_DAC_SKIP_DUMPDACTABLE,
}E_DAC_IOCTL_CMDS;;

typedef struct DLL_PACKED
{
    const MSIF_Version **ppVersion;
    APIDAC_Result eReturnValue;
} stDAC_GET_LIBVER,*pstDAC_GET_LIBVER;

typedef struct DLL_PACKED
{
    const DAC_ApiInfo* stReturnValue;
} stDAC_GET_INFO, *pstDAC_GET_INFO;

#ifdef UFO_XC_GETOUTPUTINTELACETIMING
typedef struct
{
    MS_BOOL bReturnValue;
} stDAC_GET_OUTPUTINTERLACETIMING, *pstDAC_GET_OUTPUTINTERLACETIMING;
#endif

typedef struct DLL_PACKED
{
    DAC_ApiStatus* pDacStatus;
    MS_BOOL bReturnValue;
} stDAC_GET_STATUS,*pstDAC_GET_STATUS;

typedef struct
{
    MS_U16 u16DbgSwitch;
    MS_BOOL bReturnValue;
} stDAC_SET_DBG_LEVEL,*pstDAC_SET_DBG_LEVEL;

typedef struct
{
    MS_BOOL bReturnValue;
} stDAC_INIT,*pstDAC_INIT;

typedef struct
{
    MS_BOOL bEnable;
    MS_BOOL bIsYPbPr;
} stDAC_ENABLE, *pstDAC_ENABLE;

typedef struct
{
    MS_BOOL bEnable;
    MS_BOOL bIsYPbPr;
} stDAC_SET_CLKINV, *pstDAC_SET_CLKINV;

typedef struct
{
    E_OUTPUT_VIDEO_TIMING_TYPE eTiming;
} stDAC_SET_YPBPR_OUTPUTTIMING,*pstDAC_SET_YPBPR_OUTPUTTIMING;

typedef struct
{
    E_DAC_OUTPUT_TYPE enOutputType;
    MS_BOOL bIsYPbPr;
} stDAC_SET_OUTPUT_SOURCE,*pstDAC_SET_OUTPUT_SOURCE;

typedef struct
{
    E_DAC_MAX_OUTPUT_LEVEL enLevel;
    MS_BOOL bIsYPbPr;
} stXC_SET_OUTPUT_LEVEL, *pstXC_SET_OUTPUT_LEVEL;

typedef struct
{
    E_DAC_MAX_OUTPUT_LEVEL enLevel;
    MS_BOOL bIsYPbPr;
} stDAC_SET_OUTPUT_LEVEL, *pstDAC_SET_OUTPUT_LEVEL;

typedef struct
{
    E_DAC_SWAP_SEL enSwap;
    MS_BOOL bIsYPbPr;
} stDAC_SET_OUTPUT_SWAPSEL, *pstDAC_SET_OUTPUT_SWAPSEL;

typedef struct
{
    E_DAC_SD_ENABLE_CTRL enBit;
} stDAC_ONOFF_SD,*pstDAC_ONOFF_SD;

typedef struct
{
    E_DAC_CAPS eCaptype;
    MS_U32* pRet;
    MS_U32 ret_size;
    MS_BOOL bReturnValue;
} stDAC_GET_CHIPCAPS,*pstDAC_GET_CHIPCAPS;

typedef struct
{
    E_DAC_SD_ENABLE_CTRL enBit;
} stDAC_GET_SD_STATUS,*pstDAC_GET_SD_STATUS;

typedef struct
{
    E_DAC_HD_ENABLE_CTRL enBit;
} stDAC_ONOFF_HD, *pstDAC_ONOFF_HD;

typedef struct
{
    E_DAC_HD_ENABLE_CTRL enBit;
} stDAC_GET_HD_STATUS,*pstDAC_GET_HD_STATUS;

typedef struct
{
    E_OUTPUT_VIDEO_TIMING_TYPE eTiming;
    E_OUTPUT_BIT_TYPE ebits;
} stDAC_CLKSEL, *pstDAC_CLKSEL;

typedef struct DLL_PACKED
{
    MS_U8 *pDACTable;
    MS_U8 u8DACtype;
} stDAC_DUMP_TABLE, *pstDAC_DUMP_TABLE;

typedef struct
{
    MS_BOOL bEnable;
    MS_BOOL bIsYPbPr;
} stDAC_SET_IHALF_OUTPUT, *pstDAC_SET_IHALF_OUTPUT;

typedef struct
{
    MS_BOOL bEnable;
    MS_BOOL bIsYPbPr;
} stDAC_SET_QUART_OUTPUT, *pstDAC_SET_QUART_OUTPUT;

typedef struct
{
    MS_BOOL bEnabled;
    MS_BOOL bIsYPbPr;
} stDAC_SET_DAC_STATE, *pstDAC_SET_DAC_STATE;

typedef struct DLL_PACKED
{
    E_DAC_DETECT SelDAC;
    E_DAC_DETECT_TYPE DetectType;
    MS_BOOL *State;
    MS_BOOL bReturnValue;
} stDAC_HOTPLUG_DETECT, *pstDAC_HOTPLUG_DETECT;

typedef struct
{
    E_DAC_POWER_MODE ePowerState;
    MS_U32 stReturnValue;
} stDAC_POWER_MODE, *pstDAC_POWER_MODE;

typedef struct
{
    MS_BOOL bEnable;
    MS_BOOL bIsYPbPr;
    MS_BOOL bReturnValue;
} stDAC_SET_WSSONOFF, *pstDAC_SET_WSSONOFF;

typedef struct
{
    MS_BOOL bIsYPbPr;
    MS_BOOL *pEnable;
    DAC_SETWSS_INFO *pSetWSS_Data;
    MS_BOOL bReturnValue;
} stDAC_GET_WSSSTATUS, *pstDAC_GET_WSSSTATUS;

typedef struct
{
    MS_BOOL bIsYPbPr;
    MS_BOOL bReturnValue;
} stDAC_RESET_WSSDATA, *pstDAC_RESET_WSSDATA;

typedef struct
{
    MS_BOOL bEnable;
    MS_BOOL bIsYPbPr;
    DAC_SETWSS_INFO SetWSS_Data;
    MS_BOOL bReturnValue;
} stDAC_SET_WSSOUTPUT, *pstDAC_SET_WSSOUTPUT;

typedef struct
{
    MS_BOOL bEnable;
    MS_BOOL bReturnValue;
} stDAC_ENABLE_ICT, *pstDAC_ENABLE_ICT;

typedef struct
{
    MS_BOOL bEnable;
} stDAC_SET_VGA_HsyncVsync, *pstDAC_SET_VGA_HSYNCVSYNC;

typedef struct
{
    MS_BOOL bEnable;
    APIDAC_Result eReturnValue;
} stDAC_SKIP_DUMP_DACTABLE, *pstDAC_SKIP_DUMP_DACTABLE;


#ifdef __cplusplus
}
#endif

#endif // _API_DAC_H_

