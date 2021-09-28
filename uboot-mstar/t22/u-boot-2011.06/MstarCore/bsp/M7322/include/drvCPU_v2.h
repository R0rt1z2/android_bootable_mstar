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

//////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @brief  MIU Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_CPU_V2_H_
#define _DRV_CPU_V2_H_

#ifdef __cplusplus
extern "C"
{
#endif

//-------------------------------------------------------------------------------------------------
// Defines
//-------------------------------------------------------------------------------------------------
// below for utopia20 structure

typedef enum {
    MDrv_CMD_CPU_InitFront,
    MDrv_CMD_CPU_InitEnd,
    MDrv_CMD_CPU_Disable,
    MDrv_CMD_CPU_Enable,
    MDrv_CMD_CPU_SetHost,
    MDrv_CMD_CPU_GetInfo,
    MDrv_CMD_CPU_GetLibVer,
    MDrv_CMD_CPU_GetStatus,
    MDrv_CMD_CPU_SetDbgLevel,
    MDrv_CMD_CPU_GetBase,
    MDrv_CMD_CPU_QueryClock,
} eCPUIoctlOpt;

typedef enum
{
	CPU_MODULE_V1,
	CPU_MODULE_V2,
} eCPUModuleVersion;

//////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
// Type and Structure Declaration
//-------------------------------------------------------------------------------------------------
typedef struct _CPU_API_V1
{
}CPU_API_V1;

typedef struct _CPU_API_V2
{
}CPU_API_V2;

// below are for utopia20 cmd and parameters structure
// for cmd MDrv_COPRO_Init_End
typedef struct _CPU_INITEND_PARAM
{
    MS_U32 u32_ADR;
}CPU_INITEND_PARAM, *PCPU_INITEND_PARAM;

// for cmd MDrv_COPRO_Enable
typedef struct _CPU_ENABLE_PARAM
{
    MS_U32 u32_ADR;
}CPU_ENABLE_PARAM, *PCPU_ENABLE_PARAM;

// for cmd MDrv_SetBEON_Host
typedef struct _CPU_SETHOST_PARAM
{
    MS_BOOL bHost;
}CPU_SETHOST_PARAM, *PCPU_SETHOST_PARAM;

// for cmd MDrv_COPRO_GetLibVer
typedef struct _CPU_GETLIBVER_PARAM
{
    const MSIF_Version **ppVersion;
}CPU_GETLIBVER_PARAM, *PCPU_GETLIBVER_PARAM;

// for cmd MDrv_COPRO_GetStatus
typedef struct _CPU_GETSTATUS_PARAM
{
    COPRO_DrvStatus *pStatus;
}CPU_GETSTATUS_PARAM, *PCPU_GETSTATUS_PARAM;

// for cmd MDrv_COPRO_SetDbgLevel
typedef struct _CPU_SETDBGLEVEL_PARAM
{
    MS_U8 u8Level;
}CPU_SETDBGLEVEL_PARAM, *PCPU_SETDBGLEVEL_PARAM;

// for cmd MDrv_COPRO_GetInfo
typedef struct _CPU_GETINFO_PARAM
{
    MS_BOOL     bInit;
    MS_U32      u32Addr;
}CPU_GETINFO_PARAM, *PCPU_GETINFO_PARAM;

//-------------------------------------------------------------------------------------------------
// Extern Global Variabls
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Extern Functions
//-------------------------------------------------------------------------------------------------

//////////////////////////////////////////////////////////////

#ifdef __cplusplus
}
#endif

#endif // _DRV_CPU_V2_H_

