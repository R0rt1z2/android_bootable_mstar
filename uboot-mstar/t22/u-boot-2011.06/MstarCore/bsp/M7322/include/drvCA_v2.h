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
#ifndef __DRV_CA_V2_H__
#define __DRV_CA_V2_H__

#include "drvCA.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum {
    E_MDRV_CMD_CA_Init,
    E_MDRV_CMD_CA_OTP_EnableSecureBoot,
    E_MDRV_CMD_CA_OTP_IsSecureBootEnabled,
    E_MDRV_CMD_CA_OTP_SetBlockLock,
    E_MDRV_CMD_CA_OTP_GetBlockLock,
    E_MDRV_CMD_CA_OTP_IsBlank,
    E_MDRV_CMD_CA_OTP_Read,
    E_MDRV_CMD_CA_OTP_Write,
    E_MDRV_CMD_CA_Locked,
    E_MDRV_CMD_CA_OTP_Lock,
    E_MDRV_CMD_CA_OTP_SetRSAextID,
    E_MDRV_CMD_CA_OTP_GetRSAextID,
    E_MDRV_CMD_CA_OTP_SetHASH1_REF_VER,
    E_MDRV_CMD_CA_OTP_GetHASH1_REF_VER,
    E_MDRV_CMD_CA_OTP_SetHASH_REF_VER,
    E_MDRV_CMD_CA_OTP_GetHASH_REF_VER,
    E_MDRV_CMD_CA_OTP_SetDebugPortMode,
    E_MDRV_CMD_CA_OTP_GetDebugPortMode,
    E_MDRV_CMD_CA_OTP_SetDeviceId,
    E_MDRV_CMD_CA_OTP_GetDeviceId,
    E_MDRV_CMD_CA_MaxDeviceIdSize,
    E_MDRV_CMD_CA_OTP_EnableSecureCWMode,
    E_MDRV_CMD_CA_OTP_IsSecureCWMode,
    E_MDRV_CMD_CA_Random,
    E_MDRV_CMD_CA_OTP_ReadCAVendor,
    E_MDRV_CMD_CA_OTP_SetFunctionConfig,
    E_MDRV_CMD_CA_OTP_GetFunctionConfig,
    E_MDRV_CMD_CA_CustomDataWrite,
    E_MDRV_CMD_CA_CustomDataRead,
    E_MDRV_CMD_CA_SecureWrite,
    E_MDRV_CMD_CA_SecureRead,
    E_MDRV_CMD_CA_BGCInit,
    E_MDRV_CMD_CA_OTP_SetBootDevice,
    E_MDRV_CMD_CA_OTP_GetBootDevice,
} eCAIoctlOpt;


typedef struct _CA_OTP_SETBLOCKLOCK
{
    MS_U32 u32Start;
    MS_U32 u32End;
    CA_LOCK_TYPE eLockType;
} CA_OTP_SETBLOCKLOCK;

typedef struct _CA_OTP_GETBLOCKLOCK
{
    MS_U32 *pu32Start;
    MS_U32 *pu32End;
    CA_LOCK_TYPE *peLockType;
} CA_OTP_GETBLOCKLOCK;

typedef struct _CA_OTP_ISBLANK
{
    MS_U32 u32addr;
} CA_OTP_ISBLANK;

typedef struct _CA_OTP_READ
{
    MS_U32 u32Addr;
    MS_U32 u32OTPValue;
} CA_OTP_READ;

typedef struct _CA_OTP_WRITE
{
    MS_U32 u32Addr;
    MS_U32 u32value;
} CA_OTP_WRITE;

typedef struct _CA_OTP_LOCK20
{
    CA_OTP_LOCK eLock;
} CA_OTP_LOCK20;

typedef struct _CA_OTP_SETRSAEXTID
{
    MS_U32 u32value;
} CA_OTP_SETRSAEXTID;

typedef struct _CA_OTP_GETRSAEXTID
{
    MS_U32 u32value;
} CA_OTP_GETRSAEXTID;

typedef struct _CA_OTP_SETHASH1REFVER
{
    MS_U32 u32HASH1_REF_VER;
} CA_OTP_SETHASH1REFVER;

typedef struct _CA_OTP_GETHASH1REFVER
{
    MS_U32 *pu32HASH1_REF_VER;
} CA_OTP_GETHASH1REFVER;

typedef struct _CA_OTP_SETHASHREFVER
{
    MS_U32 u32HASH0_REF_VER;
    MS_U32 u32HASH2_REF_VER;
} CA_OTP_SETHASHREFVER;

typedef struct _CA_OTP_GETHASHREFVER
{
    MS_U32 *pu32HASH0_REF_VER;
    MS_U32 *pu32HASH2_REF_VER;
} CA_OTP_GETHASHREFVER;

typedef struct _CA_OTP_SETDEBUGPORTMODE
{
    CA_DEBUG_PORT eDebugPort;
    CA_DEBUG_PORT_MODE eMode;
} CA_OTP_SETDEBUGPORTMODE;

typedef struct _CA_OTP_GETDEBUGPORTMODE
{
    CA_DEBUG_PORT eDebugPort;
    CA_DEBUG_PORT_MODE *eMode;
} CA_OTP_GETDEBUGPORTMODE;

typedef struct _CA_OTP_SETDEVICEID
{
    const MS_U8 *pu8Did;
    MS_U32 u32Size;
} CA_OTP_SETDEVICEID;

typedef struct _CA_OTP_GETDEVICEID
{
    MS_U8 *pu8Did;
    MS_U32 u32Size;
} CA_OTP_GETDEVICEID;

typedef struct _CA_MAXDEVICEIDSIZE
{
    MS_U32 u32MaxDeviceIdSize;
} CA_MAXDEVICEIDSIZE;

typedef struct _CA_RANDOM
{
    MS_U32 u32Random;
} CA_RANDOM;

typedef struct _CA_OTP_READCAVENDOR
{
    MS_U32 u32CAVendor;
} CA_OTP_READCAVENDOR;

typedef struct _CA_OTP_SETFUNCTIONCONFIG
{
    CA_OTP_FUNCATION eFunction;
    CA_OTP_CONFIG_TYPE eActive;
} CA_OTP_SETFUNCTIONCONFIG;

typedef struct _CA_OTP_GETFUNCTIONCONFIG
{
    CA_OTP_FUNCATION eFunction;
    CA_OTP_CONFIG_TYPE *peActive;
    MS_BOOL *pbDefault;
} CA_OTP_GETFUNCTIONCONFIG;

typedef struct _CA_CUSTOMDATAWRITE
{
    CA_CUSTOM_DATA eData;
    MS_U32 u32Data;
} CA_CUSTOMDATAWRITE;

typedef struct _CA_CUSTOMDATAREAD
{
    CA_CUSTOM_DATA eData;
    MS_U32 *pu32Data;
} CA_CUSTOMDATAREAD;

typedef struct _CA_SECUREWRITE
{
    CA_SECURE_ITEM eItem;
    MS_U32 u32Data;
    MS_U32 u32Size;
} CA_SECUREWRITE;

typedef struct _CA_SECUREREAD
{
    CA_SECURE_ITEM eItem;
    MS_U32 *pu32Data;
    MS_U32 u32Size;
} CA_SECUREREAD;

typedef struct _CA_BGCINIT
{
    CA_BGC_CFG strCfg;
} CA_BGCINIT;

typedef struct _CA_SET_BOOTDEVICE
{
    CA_BOOT_DEVICE eDevice;
} CA_SET_BOOTDEVICE;

typedef struct _CA_GET_BOOTDEVICE
{
    CA_BOOT_DEVICE *peBootDevice;
} CA_GET_BOOTDEVICE;

MS_U32 CAOpen(void** pInstantTmp, MS_U32 u32ModuleVersion, void* pAttribute);
MS_U32 CAClose(void* pInstantTmp);
MS_U32 CAIoctl(void* pInstantTmp, MS_U32 u32Cmd, void* pu32Args);

#ifdef __cplusplus
}
#endif

#endif // __DRV_CA_V2_H__
