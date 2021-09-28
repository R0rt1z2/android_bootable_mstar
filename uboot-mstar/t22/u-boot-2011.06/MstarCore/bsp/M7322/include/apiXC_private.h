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
#ifndef _API_XC_PRIVATE_H_
#define _API_XC_PRIVATE_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "apiXC.h"

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

typedef enum
{
    E_XC_2DMLOAD_MAIN_XC,
    E_XC_2DMLOAD_MAIN_HDR,
    E_XC_2DMLOAD_SUB_XC,
    E_XC_2DMLOAD_SUB_HDR,

    E_XC_2DMLOAD_MAX,
} EN_XC_2DMLOAD_CLIENT;

typedef enum
{
    E_XC_2DMLOAD_TRIGGER_BY_OP_SYNC,
    E_XC_2DMLOAD_TRIGGER_BY_IP_MAIN_SYNC,
    E_XC_2DMLOAD_TRIGGER_BY_IP_SUB_SYNC,
    E_XC_2DMLOAD_TRIGGER_BY_SW,
    E_XC_2DMLOAD_TRIGGER_BY_OP2_SYNC,

    E_XC_2DMLOAD_TRIGGER_MAX,
}EN_XC_2DMLOAD_TRIGGER;

typedef struct __attribute__((packed))
{
    /// Reg Bank (IN)
    MS_U32 u32Bank;
    /// Reg Address (IN)
    MS_U32 u32Addr;
    /// Reg Data (IN)
    MS_U16 u16Data;
    ///Reg Mask (IN)
    MS_U16 u16Mask;
}ST_XC_REGISTER;

typedef struct __attribute__((packed))
{
    EN_XC_2DMLOAD_CLIENT enMloadClient;
    EN_XC_2DMLOAD_TRIGGER enMloadTrigger;

    ST_XC_REGISTER * pstReg;
#if !defined (__aarch64__)
    MS_U32 u32AlignDummy; ///Dummy data
#endif
    MS_U32 u32Count;
}ST_XC_MENULOAD_INFO,*pstXC_MENULOAD_INFO;

typedef enum
{
    E_DS_IP,
    E_DS_OP,
}EN_DS_REG_IP_OP_SEL;

typedef enum
{
    E_DS_XC,
    E_DS_MVOP,
    E_DS_GOP,
}EN_DS_REG_SOURCE_SEL;

typedef struct
{
    MS_U32  u32Version;                          //Structure version
    MS_U16  u16Length;                           //Structure length

    SCALER_WIN eWindow;
    MS_U32 u32CmdRegAddr;
    MS_U16 u16CmdRegValue;
    EN_DS_REG_IP_OP_SEL IPOP_Sel;
    EN_DS_REG_SOURCE_SEL Source_Select;
    MS_BOOL bDsEnable;
}ST_XC_WRITE_CMD;

typedef struct DLL_PACKED
{
    ST_XC_WRITE_CMD *pstXCWriteCmd;
    E_APIXC_ReturnValue eReturnValue;
} stXC_WRITE_CMD, *pstXC_WRITE_CMD;

typedef struct
{
    SCALER_WIN eWindow;
    E_APIXC_ReturnValue eReturnValue;
} stXC_SWDSFIRE, *pstXC_SWDSFIRE;

typedef struct
{
    SCALER_WIN eWindow;
    MS_U32 *pu32BufferSize;
    E_APIXC_ReturnValue eReturnValue;
} stXC_GETDNRBUFFERSIZE, *pstXC_GETDNRBUFFERSIZE;

typedef struct DLL_PACKED
{
    XC_HDR_OTT_SHARE_MEMORY_INFO *pstShareMemInfo;
    E_APIXC_ReturnValue eReturnValue;
} stXC_GETOTTSHMINFO, *pstXC_GETOTTSHMINFO;

typedef struct
{
    ST_XC_MENULOAD_INFO *pstMenuloadInfo;
    SCALER_WIN enWindow;
    E_APIXC_ReturnValue eReturnValue;
} stXC_WRITEANDFIREREGINVSYNCBLANK, *pstXC_WRITEANDFIREREGINVSYNCBLANK;

E_APIXC_ReturnValue MApi_XC_WriteCommand(ST_XC_WRITE_CMD *pstXCWriteCmd);
E_APIXC_ReturnValue MApi_XC_SWDS_Fire(SCALER_WIN eWindow);
E_APIXC_ReturnValue MApi_XC_GetDNRBufferSize(SCALER_WIN eWindow, MS_U32 *pu32BufferSize);
E_APIXC_ReturnValue MApi_XC_EnableCaptureMode(MS_BOOL bEnable);
E_APIXC_ReturnValue MApi_XC_GetOttShareMemoryInfo(XC_HDR_OTT_SHARE_MEMORY_INFO *pstShareMemInfo);
E_APIXC_ReturnValue MApi_XC_WriteAndFireReginVsyncBlank(ST_XC_MENULOAD_INFO *pstMenuloadInfo,SCALER_WIN enWindow);
#ifdef __cplusplus
}
#endif

#endif // _API_XC_PRIV_H_
