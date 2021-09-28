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
#ifndef _DRVSAR_PRIV_H_
#define _DRVSAR_PRIV_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "MsTypes.h"
#include "drvSAR.h"

#ifdef CONFIG_MSTAR_UTPA2K_SAR_ENABLE
typedef SAR_KpdResult (*IOCTL_SAR_Kpd_GetKeyCode)(MS_U8 *pu8Key, MS_U8 *pu8Repeat);
typedef SAR_KpdResult (*IOCTL_SAR_Kpd_Config)( SAR_KpdRegCfg* sarChInfo);

//SAR_Private

typedef struct _SAR_RESOURCE_SHARED
{
    void* pSARResource;
}SAR_RESOURCE_SHARED;

typedef struct _SAR_INSTANT_PRIVATE
{
    IOCTL_SAR_Kpd_GetKeyCode fpSARKpdGetKeyCode;
    IOCTL_SAR_Kpd_Config  fpSARKpdConfig;
}SAR_INSTANT_PRIVATE;

typedef struct _SAR_PRIVATE_PARAM_Kpd_GetKeyCode
{
   MS_U8 u8Key;
   MS_U8 u8Repeat;
   MS_U32 u32Retn;
} SAR_PRIVATE_PARAM_Kpd_GetKeyCode, *PSAR_PRIVATE_PARAM_Kpd_GetKeyCode ;

typedef struct  _SAR_PRIVATE_PARAM_KeyConfig
{
   SAR_KpdRegCfg *pKeyRegCfg;
} SAR_PRIVATE_PARAM_KeyConfig, *PSAR_PRIVATE_PARAM_KeyConfig;

typedef struct _SAR_RESOURCE_PRIVATE
{
    SAR_PRIVATE_PARAM_KeyConfig  privateKpd_Config;
}SAR_RESOURCE_PRIVATE;

typedef union _SAR_PRIVATE_PARAM
{
    SAR_PRIVATE_PARAM_Kpd_GetKeyCode  privateKpd_GetKeyCode;
    SAR_PRIVATE_PARAM_KeyConfig  privateKpd_Config;
}SAR_PRIVATE_PARAM;

void SARRegisterToUtopia(FUtopiaOpen ModuleType);
MS_U32 SAROpen(void** pInstance, MS_U32 u32ModuleVersion, void* pAttribute);
MS_U32 SARClose(void* pInstance);
MS_U32 SARIoctl(void* pInstance, MS_U32 u32Cmd, void *pArgs);
MS_U32 SARStr(MS_U32 u32PowerState, void* pModule);
#endif

#ifdef __cplusplus
}
#endif
#endif // _DRVSAR_PRIV_H_
