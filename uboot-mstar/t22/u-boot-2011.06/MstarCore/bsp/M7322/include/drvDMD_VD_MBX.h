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
/// @brief  System Control Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_SYS_DMD_VD_MBX_H_
#define _DRV_SYS_DMD_VD_MBX_H_

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


//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
typedef enum
{
    E_DMD_VD_MBX_TYPE_ATV,
    E_DMD_VD_MBX_TYPE_DVBT,
    E_DMD_VD_MBX_TYPE_DVBC,
    E_DMD_VD_MBX_TYPE_ATSC,
    E_DMD_VD_MBX_TYPE_DVBS,
    E_DMD_VD_MBX_TYPE_ISDBT,
    E_DMD_VD_MBX_TYPE_DTMB,
    E_DMD_VD_MBX_TYPE_DVBT2,
    E_DMD_VD_MBX_TYPE_MAX
}DMD_VD_MBX_Type;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SYS_DMD_VD_MBX_Init(void);
void MDrv_SYS_DMD_VD_MBX_SetType(DMD_VD_MBX_Type eType);
MS_BOOL MDrv_SYS_DMD_VD_MBX_ReadReg(MS_U32 u32Reg, MS_U8 *u8Value);
MS_BOOL MDrv_SYS_DMD_VD_MBX_ReadDSPReg(MS_U32 u32Reg, MS_U8 *u8Value);
MS_BOOL MDrv_SYS_DMD_VD_MBX_DBG_ReadReg(MS_U32 u32Reg, MS_U8 *u8Value);
MS_BOOL MDrv_SYS_DMD_VD_MBX_WriteReg(MS_U32 u32Reg, MS_U8 u8Value);
MS_BOOL MDrv_SYS_DMD_VD_MBX_WriteDSPReg(MS_U32 u32Reg, MS_U8 u8Value);
MS_BOOL MDrv_SYS_DMD_VD_MBX_DBG_WriteReg(MS_U32 u32Reg, MS_U8 u8Value);
#ifdef __cplusplus
}
#endif
#endif // _DRV_SYS_DMD_VD_MBX_H_
