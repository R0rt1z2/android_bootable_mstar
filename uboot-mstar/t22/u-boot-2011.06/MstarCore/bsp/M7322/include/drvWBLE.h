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
#ifndef DRV_WBLE_H
#define DRV_WBLE_H

#ifdef __cplusplus
extern "C"
{
#endif


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
//  library information
#define MSIF_WBLE_LIB_CODE               {'W','B','L','E'}
#define MSIF_WBLE_LIBVER                 {'0','0'}
#define MSIF_WBLE_BUILDNUM               {'0','2'}
#define MSIF_WBLE_CHANGELIST             {'0','0','2','7','5','5','7','8'}
#define WBLE_API_VERSION                  /* Character String for DRV/API version             */  \
    MSIF_TAG,                           /* 'MSIF'                                           */  \
    MSIF_CLASS,                         /* '00'                                             */  \
    MSIF_CUS,                           /* 0x0000                                           */  \
    MSIF_MOD,                           /* 0x0000                                           */  \
    MSIF_CHIP,                                                                                  \
    MSIF_CPU,                                                                                   \
    MSIF_WBLE_LIB_CODE  ,                 /* IP__                                             */  \
    MSIF_WBLE_LIBVER    ,                 /* 0.0 ~ Z.Z                                        */  \
    MSIF_WBLE_BUILDNUM  ,                 /* 00 ~ 99                                          */  \
    MSIF_WBLE_CHANGELIST,                 /* CL#                                              */  \
    MSIF_OS

#define WBLE_INITDATA_VERSION                1


/// XC Return Value
typedef enum
{
    E_DRVWBLE_RET_FAIL = 0,   ///<Fail
    E_DRVWBLE_RET_OK,         ///<OK
} E_DRVWBLE_ReturnValue;



void MDrv_WBLE_Init(void);
void MDrv_WBLE_EnableBLE(MS_BOOL bEnable);
void MDrv_WBLE_EnableWLE(MS_BOOL bEnable);
void MDrv_WBLE_SetBLE(MS_U8 u8Start, MS_U8 u8Slop);
void MDrv_WBLE_SetWLE(MS_U8 u8Start, MS_U8 u8Slop);
void MDrv_WBLE_Handler_BLEAvgLuma(MS_U8 u8AvgY, MS_U16 u16YTotalSum);
void MDrv_WBLE_Set_SlopeValue(MS_U8 u8Slope);

#ifdef __cplusplus
}
#endif



#endif
