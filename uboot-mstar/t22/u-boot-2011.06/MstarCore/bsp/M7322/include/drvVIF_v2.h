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


#ifndef _DRVVIF_V2_H_
#define _DRVVIF_V2_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "MsCommon.h"
#include "MsIRQ.h"
#include "MsOS.h"
#include "MsTypes.h"
#include "drvVIF.h"

//****************************************************************************
// Public attributes.
//****************************************************************************

#ifndef BOOL
#define BOOL MS_BOOL
#endif
#ifndef BYTE
#define BYTE MS_U8
#endif
#ifndef WORD
#define WORD MS_U16
#endif
#ifndef DWORD
#define DWORD MS_U32
#endif
#ifndef U8
#define U8 MS_U8
#endif
#ifndef U16
#define U16 MS_U16
#endif
#ifndef U32
#define U32 MS_U32
#endif
#ifndef VIRT
#define VIRT MS_VIRT
#endif

typedef enum 
{
    Drv_CMD_VIF_Version,
    Drv_CMD_VIF_SetClock,
    Drv_CMD_VIF_Init,
    Drv_CMD_VIF_Exit,
    Drv_CMD_VIF_Reset,
    Drv_CMD_VIF_Handler,
    Drv_CMD_VIF_SetSoundSystem,
    Drv_CMD_VIF_SetIfFreq,
    Drv_CMD_VIF_Read_CR_FOE,
    Drv_CMD_VIF_Read_CR_LOCK_STATUS,
    Drv_CMD_VIF_BypassDBBAudioFilter,
    Drv_CMD_VIF_SetFreqBand,

    Drv_CMD_VIF_GetInputLevelIndicator,
    Drv_CMD_VIF_SetParameter,
    Drv_CMD_VIF_ShiftClk,
    Drv_CMD_VIF_SetPowerState,
    Drv_CMD_VIF_WriteByte,
    Drv_CMD_VIF_ReadByte,
    
} eVifIoctlOpt;

typedef struct DLL_PACKED _VIF_INIT
{
    VIFInitialIn *pVIF_InitData;
    DWORD u32InitDataLen;
}eVIF_INIT, *PVIF_INIT;

typedef struct DLL_PACKED _VIF_SETREGVALUE
{
    U32 u32Addr;
    U8 u8Value;
}VIF_SETREGVALUE, *PVIF_SETREGVALUE;

typedef struct DLL_PACKED _VIF_SET_PARA
{
    BOOL bRet;
    VIF_PARA_GROUP ParaGroup;
    void *pVIF_Para;
    DWORD u32DataLen;
}VIF_SETPARA, *PVIF_SETPARA;

typedef struct DLL_PACKED _VIF_COPYTOUSER
{
    BOOL bEnable;
    BYTE u8Value;
    WORD u16Value;
    DWORD u32Value;
    EN_POWER_MODE u16PowerState;
}VIF_COPYTOUSER, *PVIF_COPYTOUSER;

typedef struct DLL_PACKED _VIF_RESOURCE_PRIVATE
{
    VIFInitialIn sDMD_VIF_InitData;
}VIF_RESOURCE_PRIVATE, *PVIF_RESOURCE_PRIVATE;


#ifdef __cplusplus
}
#endif

#endif //_DRVVIF_V2_H_

