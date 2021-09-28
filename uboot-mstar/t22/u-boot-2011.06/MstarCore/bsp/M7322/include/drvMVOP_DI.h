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
/// @brief  MVOP Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_MVOP_DI_H_
#define _DRV_MVOP_DI_H_

#ifdef __cplusplus
extern "C"
{
#endif
#include "MsDevice.h"
#include "UFO.h"

//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define ENABLE_UTOPIA2_INTERFACE   1

#define ST_MVOPDI_MEM_INFO_VERSION                  0
#define ST_MVOPDI_DATA_SIZE_VERSION                 0
//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
typedef enum
{
    E_MVOPDI_OK              = 0,
    E_MVOPDI_FAIL            = 1,
    E_MVOPDI_INVALID_PARAM  = 2,
    E_MVOPDI_NOT_INIT        = 3,
    E_MVOPDI_ALREADY_EXIT    = 4,
    E_MVOPDI_UNSUPPORTED     = 5
} MVOPDI_Result;

typedef enum
{
    E_MVOPDI_MFD_ROTATE_DI   = 0,
    E_MVOPDI_ABF              = 1,
    E_MVOPDI_DEFAULT
} EN_MVOPDI_Usage;

typedef enum
{
    E_MVOPDI_CMD_SET_TYPE                   = 0x100,
    E_MVOPDI_CMD_SET_CONFIG,
    E_MVOPDI_CMD_SET_SIZE,
    E_MVOPDI_CMD_SET_DIPR_BASE_ADDR,
    E_MVOPDI_CMD_SET_TRICK_MD,
    E_MVOPDI_CMD_SET_AUTO_SW_TRIG,
    E_MVOPDI_CMD_SET_SW_TRIG,
    E_MVOPDI_CMD_RESET_BOB_CNT,

    E_MVOPDI_CMD_GET_TYPE                   = 0x400,
    E_MVOPDI_CMD_GET_DIPW_BASE_ADDR
} EN_MVOPDI_Command;

typedef enum
{
    E_MVOPDI_MEM_MFD_ROTATE_DI,
    E_MVOPDI_MEM_ABF,
    E_MVOPDI_MEM_NONE
}EN_MVOPDI_MEMORY_TYPE;

typedef struct DLL_PACKED
{
    MS_U32 u32Version;                /// Structure version
    MS_U32 u32Length;                 /// Structure length
    EN_MVOPDI_MEMORY_TYPE enType;
    MS_PHY phyBaseAddr;
    MS_U32 u32Size;
} ST_MVOPDI_MEMORY_INFO;

typedef enum
{
    E_MVOPDI_420_16x32  = 0,
    E_MVOPDI_420_32x16  = 1,
    E_MVOPDI_420_32x32  = 2,
    E_MVOPDI_422         = 3,
    E_MVOPDI_RGB_565    = 4,
    E_MVOPDI_RGB_888    = 5,
    E_MVOPDI_FORMAT_NONE
} EN_MVOPDI_DataFormat;

typedef struct DLL_PACKED
{
    MS_U32 u32Version;                /// Structure version
    MS_U32 u32Length;                 /// Structure length
    EN_MVOPDI_Usage eUsage;
    MS_U16 u16Width;
    MS_U16 u16Height;
} ST_MVOPDI_DATA_SIZE;

typedef struct DLL_PACKED
{
    MS_U32 u32Version;                /// Structure version
    MS_U32 u32Length;                 /// Structure length
    MS_PHY phyYAddr;
    MS_PHY phyUVAddr;
    MS_U16 u16PitchR;                 /// pitch of DIPR
    MS_U16 u16PitchW;                 /// pitch of DIPW
    MS_U8  u8BottomFieldFirst;        /// bottom first
    EN_MVOPDI_DataFormat  inputFmt;   /// tile format
    //EN_MVOPDI_DataFormat  outputFmt;  /// tile format. (same as inputFmt in M7622 rotate)

} ST_MVOPDI_FRAME_INFO;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

void MDrv_MVOPDI_Init(void);
void MDrv_MVOPDI_Exit(void);
void MDrv_MVOPDI_Enable(EN_MVOPDI_Usage eUsage , MS_BOOL bEnable);
MVOPDI_Result MDrv_MVOPDI_SetCommand(EN_MVOPDI_Usage eUsage, EN_MVOPDI_Command eCmd, void* pPara);
MVOPDI_Result MDrv_MVOPDI_GetCommand(EN_MVOPDI_Usage eUsage, EN_MVOPDI_Command eCmd, void* pPara, MS_U32 u32ParaSize);
MVOPDI_Result MDrv_MVOPDI_SetDIBufferAddr(ST_MVOPDI_MEMORY_INFO stMVOPDIMemInfo);
#ifdef __cplusplus
}
#endif

#endif // _DRV_MVOP_H_
