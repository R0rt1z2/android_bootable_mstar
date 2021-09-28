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
#ifndef _DRVCPU_PRIV_H_
#define _DRVCPU_PRIV_H_

////////////////////////////////////////////////////////////////////////////////
/// @brief Byte DMA control driver
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C"
{
#endif

#include "MsTypes.h"
#include "drvCPU.h"

typedef void (*IOCTL_CPU_DISABLE)(void);
typedef void (*IOCTL_CPU_ENABLE)(MS_U32 u32_ADR);
typedef void (*IOCTL_CPU_SETHOST)(MS_BOOL bHost);
typedef void (*IOCTL_CPU_INITFRONT)(void);
typedef void (*IOCTL_CPU_INITEND)(MS_U32 u32_ADR);
typedef const COPRO_DrvInfo* (*IOCTL_CPU_GETINFO)(void);
typedef COPRO_Result (*IOCTL_CPU_GETLIBVER)(const MSIF_Version **ppVersion);
typedef COPRO_Result (*IOCTL_CPU_GETSTATUS)(COPRO_DrvStatus *pStatus);
typedef void (*IOCTL_CPU_SETDBGLEVEL)(MS_U8 u8Level);
typedef MS_U32 (*IOCTL_CPU_GETBASE)(void);
typedef MS_U32 (*IOCTL_CPU_QUERYCLOCK)(void);

//#################################
typedef struct _CPU_RESOURCE_PRIVATE
{
    MS_U32 Dummy;
}CPU_RESOURCE_PRIVATE;
//#################################

typedef struct _CPU_INSTANT_PRIVATE
{   
    IOCTL_CPU_DISABLE       fpCPUDisable;
    IOCTL_CPU_ENABLE        fpCPUEnable;
    IOCTL_CPU_SETHOST       fpCPUSetHost;
    IOCTL_CPU_INITFRONT     fpCPUInitFront;
    IOCTL_CPU_INITEND       fpCPUInitEnd;
    IOCTL_CPU_GETINFO       fpCPUGetInfo;
    IOCTL_CPU_GETLIBVER     fpCPUGetLibVer;
    IOCTL_CPU_GETSTATUS     fpCPUGetStatus;
    IOCTL_CPU_SETDBGLEVEL   fpCPUSetDbgLevel;
    IOCTL_CPU_GETBASE       fpCPUGetBase;
    IOCTL_CPU_QUERYCLOCK    fpCPUQueryClock;

}CPU_INSTANT_PRIVATE;

void CPURegisterToUtopia(FUtopiaOpen ModuleType);
MS_U32 CPUOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute);
MS_U32 CPUClose(void* pInstance);
MS_U32 CPUIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs);

void _MDrv_CPU_InitFront(void);
void _MDrv_CPU_InitEnd(MS_U32 u32_ADR);
void _MDrv_CPU_Disable(void);
void _MDrv_CPU_Enable(MS_U32 u32_ADR);
void _MDrv_CPU_SetHost(MS_BOOL bHost);
const COPRO_DrvInfo* _MDrv_CPU_GetInfo(void);
COPRO_Result _MDrv_CPU_GetLibVer(const MSIF_Version **ppVersion);
COPRO_Result _MDrv_CPU_GetStatus(COPRO_DrvStatus *pStatus);
void _MDrv_CPU_SetDbgLevel(MS_U8 u8Level);
MS_VIRT _MDrv_CPU_GetBase(void);
MS_U32 _MDrv_CPU_QueryClock(void);


#ifdef __cplusplus
}
#endif
#endif // _DRVCPU_PRIV_H_
