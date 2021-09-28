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
/// @brief  ACP Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _API_ACP_H_
#define _API_ACP_H_

#ifdef __cplusplus
extern "C" {
#endif

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
// library information
#define MSIF_ACP_LIB_CODE               {'A','C','P','_'}
#define MSIF_ACP_LIBVER                 {'0','0'}
#define MSIF_ACP_BUILDNUM               {'0','2'}
#define MSIF_ACP_CHANGELIST             {'0','0','6','1','4','0','9','2'}

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

// Result of VE function
typedef enum _ACP_Result
{
    E_ACP_NOT_SUPPORT = -1,
    E_ACP_FAIL = 0,
    E_ACP_OK = 1,
} ACP_Result;

//MV Type
typedef enum
{
    MS_VE_MV_NTSC_START = 0,
    MS_VE_MV_NTSC_TYPE1 = MS_VE_MV_NTSC_START,
    MS_VE_MV_NTSC_TYPE2,
    MS_VE_MV_NTSC_TYPE3,
    MS_VE_MV_NTSC_TEST_N01,
    MS_VE_MV_NTSC_TEST_N02,
    MS_VE_MV_NTSC_TYPE2_TTX,
    MS_VE_MV_NTSC_TEST_N01_TTX,
    MS_VE_MV_PAL_START = 0x10,
    MS_VE_MV_PAL_TYPE1 = MS_VE_MV_PAL_START,
    MS_VE_MV_PAL_TYPE2,
    MS_VE_MV_PAL_TYPE3,
    MS_VE_MV_PAL_TEST_P01,
    MS_VE_MV_PAL_TEST_P02,
    MS_VE_MV_PAL_TYPE2_TTX,
    MS_VE_MV_PAL_TEST_P01_TTX,

    MS_VE_MV_COMPONENT_MIN = 0x20,
    MS_VE_MV_COMPONENT_480I_TYPE2 = MS_VE_MV_COMPONENT_MIN,
    MS_VE_MV_COMPONENT_480I_TYPE3,
    MS_VE_MV_COMPONENT_480P_EIA_7701,
    MS_VE_MV_COMPONENT_480P_EIA_7702,
    MS_VE_MV_COMPONENT_576I_P01,
    MS_VE_MV_COMPONENT_576I_P02,
    MS_VE_MV_COMPONENT_576P,
    MS_VE_MV_TYPE_MAX,
} MS_VE_MV_TYPE;

typedef struct
{
    MS_U8   u8CPC[2];
    MS_U8   u8CPS[34];
}MS_ACP_MV_ByteInfo;

typedef struct
{
    //Versioning
    MS_U32  u32ACP_MV_BitControl_version;     ///<Version of current structure.
    MS_U32  u32ACP_MV_BitControl_Length;      ///<Length of this structure, u32ACP_MV_BitControl_Length=sizeof(MS_ACP_MV_BitControl)
    //Data
    MS_BOOL bIsYPbPr;
    union
    {
        MS_U8               u8MV_ByteInfo[36];
        MS_ACP_MV_ByteInfo  MV_ByteInfo;
    }__attribute__ ((__packed__))ByteInfo ;
}MS_ACP_MV_BitControl;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

//----------------------------------------------------------------
// MApi_ACP_SetProtection
// enable/disable protection for composite (CVBS) and component outputs
// @bEnabled: TRUE: enabled, FALSE: not enabled
// @bIsYPbPr: TRUE: component, FALSE: CVBS
// @u8Type: protection type
// @return ACP_Result
//----------------------------------------------------------------
ACP_Result SYMBOL_WEAK MApi_ACP_SetProtection(MS_BOOL bEnable, MS_BOOL bIsYPbPr, MS_U8 u8Type);

//----------------------------------------------------------------
// MApi_ACP_SetMV_BitControl
// enable/disable protection by Bit Control (CPC/CPS) for composite (CVBS) and component outputs
// @bEnabled: TRUE: enabled, FALSE: not enabled
// @MS_ACP_MV_BitControl: protection data structure
// @return ACP_Result
//----------------------------------------------------------------
ACP_Result MApi_ACP_SetMV_BitControl(MS_BOOL bEnable, MS_ACP_MV_BitControl MV_BitControl_Data);

//----------------------------------------------------------------
// MApi_DCS_SetProtection
// enable/disable DCS protection for composite (CVBS) and component outputs
// @bEnabled: TRUE: enabled, FALSE: not enabled
// @u8Type: protection type
// @return ACP_Result
//----------------------------------------------------------------
ACP_Result MApi_DCS_SetProtection(MS_BOOL bEnable, MS_U8 u8Type);

//----------------------------------------------------------------
// MApi_DCS_SetActivationKey
// DCS protection Activation Key Setting By External
// @pu8ActivationKeyTbl: External Activation Key Table
// @u8ActivationKeyTblSize: External Activation Key Table Size
// @return ACP_Result
//----------------------------------------------------------------
ACP_Result MApi_DCS_SetActivationKey(MS_U8 *pu8ActivationKeyTbl, MS_U8 u8ActivationKeyTblSize);

#ifdef __cplusplus
}
#endif

#endif

