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
#ifndef _DRVBDHWI2C_PRIV_H_
#define _DRVBDHWI2C_PRIV_H_


////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C"
{
#endif

#include "MsTypes.h"
#include "drvHWI2C.h"

typedef MS_BOOL  (*IOCTL_MDrv_HWI2C_Init)(HWI2C_UnitCfg *);
typedef MS_BOOL  (*IOCTL_MDrv_HWI2C_Start)(void);
typedef MS_BOOL  (*IOCTL_MDrv_HWI2C_Stop)(void);
typedef MS_BOOL  (*IOCTL_MDrv_HWI2C_GetPortIndex)(HWI2C_PORT , MS_U8 *);
typedef MS_BOOL  (*IOCTL_MDrv_HWI2C_SelectPort)(HWI2C_PORT );
typedef MS_BOOL  (*IOCTL_MDrv_HWI2C_SetClk)(HWI2C_CLKSEL );
typedef MS_BOOL  (*IOCTL_MDrv_HWI2C_SetReadMode)(HWI2C_ReadMode  );
typedef MS_BOOL  (*IOCTL_MDrv_HWI2C_WriteByte)(MS_U16 ,MS_U8 ,MS_U8  );
typedef MS_BOOL  (*IOCTL_MDrv_HWI2C_WriteBytes)(MS_U16 ,MS_U32 ,MS_U8 *,MS_U32 ,MS_U8 * );
typedef MS_BOOL  (*IOCTL_MDrv_HWI2C_ReadByte)(MS_U16 ,MS_U8 ,MS_U8 * );
typedef MS_BOOL  (*IOCTL_MDrv_HWI2C_ReadBytes)(MS_U16 ,MS_U32 ,MS_U8 *,MS_U32 ,MS_U8 * );
typedef MS_BOOL  (*IOCTL_MDrv_HWI2C_SelectPort1)(HWI2C_PORT  );
typedef MS_BOOL  (*IOCTL_MDrv_HWI2C_SetClkP1)(HWI2C_CLKSEL );
typedef MS_BOOL  (*IOCTL_MDrv_HWI2C_SetReadModeP1)(HWI2C_ReadMode  );
typedef MS_BOOL  (*IOCTL_MDrv_HWI2C_WriteByteP1)(MS_U16 , MS_U8 , MS_U8 );
typedef MS_BOOL  (*IOCTL_MDrv_HWI2C_WriteBytesP1)(MS_U16 , MS_U32 , MS_U8 *, MS_U32 , MS_U8 *);
typedef MS_BOOL  (*IOCTL_MDrv_HWI2C_ReadByteP1)(MS_U16 , MS_U8 , MS_U8 *);
typedef MS_BOOL  (*IOCTL_MDrv_HWI2C_ReadBytesP1)(MS_U16 , MS_U32 , MS_U8 *, MS_U32 , MS_U8 *);
typedef MS_BOOL  (*IOCTL_MDrv_HWI2C_SetDbgLevel)(HWI2C_DbgLv );
typedef MS_BOOL  (*IOCTL_MDrv_HWI2C_GetLibVer)(const MSIF_Version **);
typedef void     (*IOCTL_MDrv_HWI2C_GetStatus)(HWI2C_Status *);
typedef const HWI2C_Info*   (*IOCTL_MDrv_HWI2C_GetInfo)(void );
//typedef MS_U16   (*IOCTL_MDrv_HWI2C_SetPowerState)(EN_POWER_MODE );


typedef struct DLL_PACKED _HWI2C_PRIVATE_PARAM_Init
{
    HWI2C_UnitCfg *psCfg;
#if !defined (__aarch64__)
    MS_U32 psCfgDummy;
#endif
} HWI2C_PRIVATE_PARAM_Init, *PHWI2C_PRIVATE_PARAM_Init;

typedef struct DLL_PACKED _HWI2C_PRIVATE_PARAM_GetPortIndex
{
    HWI2C_PORT ePort;
    MS_U8* pu8Port;
} HWI2C_PRIVATE_PARAM_GetPortIndex, *PHWI2C_PRIVATE_PARAM_GetPortIndex;

typedef struct _HWI2C_PRIVATE_PARAM_SelectPort
{
    HWI2C_PORT ePort;
} HWI2C_PRIVATE_PARAM_SelectPort, *PHWI2C_PRIVATE_PARAM_SelectPort;

typedef struct _HWI2C_PRIVATE_PARAM_SetClk
{
    HWI2C_CLKSEL eClk;
} HWI2C_PRIVATE_PARAM_SetClk, *PHWI2C_PRIVATE_PARAM_SetClk;

typedef struct _HWI2C_PRIVATE_PARAM_SetReadMode
{
    HWI2C_ReadMode eReadMode;
} HWI2C_PRIVATE_PARAM_SetReadMode, *PHWI2C_PRIVATE_PARAM_SetReadMode;

typedef struct _HWI2C_PRIVATE_PARAM_WriteByte
{
    MS_U16 u16SlaveCfg;
    MS_U8 u8RegAddr;
    MS_U8 u8Data;
    MS_BOOL bReturn;
} HWI2C_PRIVATE_PARAM_WriteByte, *PHWI2C_PRIVATE_PARAM_WriteByte;

typedef struct DLL_PACKED _HWI2C_PRIVATE_PARAM_WriteBytes
{
    MS_U16 u16SlaveCfg;
    MS_U32 uAddrCnt;
    MS_U8 *pRegAddr;
    MS_U32 uSize;
    MS_U8 *pData;
    MS_BOOL bReturn;
} HWI2C_PRIVATE_PARAM_WriteBytes, *PHWI2C_PRIVATE_PARAM_WriteBytes;

typedef struct DLL_PACKED _HWI2C_PRIVATE_PARAM_ReadByte
{
    MS_U16 u16SlaveCfg;
    MS_U8 u8RegAddr;
    MS_U8 *pData;
    MS_BOOL bReturn;
} HWI2C_PRIVATE_PARAM_ReadByte, *PHWI2C_PRIVATE_PARAM_ReadByte;

typedef struct DLL_PACKED _HWI2C_PRIVATE_PARAM_ReadBytes
{
    MS_U16 u16SlaveCfg;
    MS_U32 uAddrCnt;
    MS_U8 *pRegAddr;
    MS_U32 uSize;
    MS_U8 *pData;
    MS_BOOL bReturn;
} HWI2C_PRIVATE_PARAM_ReadBytes, *PHWI2C_PRIVATE_PARAM_ReadBytes;

typedef struct _HWI2C_PRIVATE_PARAM_SelectPort1
{
    HWI2C_PORT ePort;
} HWI2C_PRIVATE_PARAM_SelectPort1, *PHWI2C_PRIVATE_PARAM_SelectPort1;

typedef struct _HWI2C_PRIVATE_PARAM_SetClkP1
{
    HWI2C_CLKSEL eClk;
} HWI2C_PRIVATE_PARAM_SetClkP1, *PHWI2C_PRIVATE_PARAM_SetClkP1;

typedef struct _HWI2C_PRIVATE_PARAM_SetReadModeP1
{
    HWI2C_ReadMode eReadMode;
} HWI2C_PRIVATE_PARAM_SetReadModeP1, *PHWI2C_PRIVATE_PARAM_SetReadModeP1;

typedef struct _HWI2C_PRIVATE_PARAM_WriteByteP1
{
    MS_U16 u16SlaveCfg;
    MS_U8 u8RegAddr;
    MS_U8 u8Data;
} HWI2C_PRIVATE_PARAM_WriteByteP1, *PHWI2C_PRIVATE_PARAM_WriteByteP1;

typedef struct DLL_PACKED _HWI2C_PRIVATE_PARAM_WriteBytesP1
{
    MS_U16 u16SlaveCfg;
    MS_U32 uAddrCnt;
    MS_U8 *pRegAddr;
    MS_U32 uSize;
    MS_U8 *pData;
} HWI2C_PRIVATE_PARAM_WriteBytesP1, *PHWI2C_PRIVATE_PARAM_WriteBytesP1;


typedef struct DLL_PACKED _HWI2C_PRIVATE_PARAM_ReadByteP1
{
    MS_U16 u16SlaveCfg;
    MS_U8 u8RegAddr;
    MS_U8 *pData;
} HWI2C_PRIVATE_PARAM_ReadByteP1, *PHWI2C_PRIVATE_PARAM_ReadByteP1;

typedef struct DLL_PACKED _HWI2C_PRIVATE_PARAM_ReadBytesP1
{
    MS_U16 u16SlaveCfg;
    MS_U32 uAddrCnt;
    MS_U8 *pRegAddr;
    MS_U32 uSize;
    MS_U8 *pData;
} HWI2C_PRIVATE_PARAM_ReadBytesP1, *PHWI2C_PRIVATE_PARAM_ReadBytesP1;

typedef struct _HWI2C_PRIVATE_PARAM_SetDbgLevel
{
    HWI2C_DbgLv eLevel;
} HWI2C_PRIVATE_PARAM_SetDbgLevel, *PHWI2C_PRIVATE_PARAM_SetDbgLevel;

typedef struct DLL_PACKED _HWI2C_PRIVATE_PARAM_GetLibVer
{
    const MSIF_Version **ppVersion;
} HWI2C_PRIVATE_PARAM_GetLibVer, *PHWI2C_PRIVATE_PARAM_GetLibVer;

typedef struct DLL_PACKED _HWI2C_PRIVATE_PARAM_GetStatus
{
    HWI2C_Status *pStatus;
} HWI2C_PRIVATE_PARAM_GetStatus, *PHWI2C_PRIVATE_PARAM_GetStatus;


typedef struct _HWI2C_RESOURCE_PRIVATE
{
    HWI2C_PRIVATE_PARAM_Init privateInit;
}HWI2C_RESOURCE_PRIVATE;

typedef union _HWI2C_PRIVATE_PARAM
{
    HWI2C_PRIVATE_PARAM_Init privateInit;
    HWI2C_PRIVATE_PARAM_GetPortIndex  privateGetPortIndex;
    HWI2C_PRIVATE_PARAM_SelectPort privateSelectPort;
    HWI2C_PRIVATE_PARAM_SetClk privateSetClk;
    HWI2C_PRIVATE_PARAM_SetReadMode privateSetReadMode;
    HWI2C_PRIVATE_PARAM_WriteByte privateWriteByte;
    HWI2C_PRIVATE_PARAM_WriteBytes privateWriteBytes;
    HWI2C_PRIVATE_PARAM_ReadByte privateReadByte;
    HWI2C_PRIVATE_PARAM_ReadBytes privateReadBytes;
    HWI2C_PRIVATE_PARAM_SetReadModeP1 privateSetReadModeP1;
    HWI2C_PRIVATE_PARAM_WriteByteP1 privateWriteByteP1;
    HWI2C_PRIVATE_PARAM_WriteBytesP1 privateWriteBytesP1;
    HWI2C_PRIVATE_PARAM_ReadByteP1 privateReadByteP1;
    HWI2C_PRIVATE_PARAM_ReadBytesP1 privateReadBytesP1;
    HWI2C_PRIVATE_PARAM_SetDbgLevel privateSetDbgLevel;
    HWI2C_PRIVATE_PARAM_GetStatus privateGetStatus;
} HWI2C_PRIVATE_PARAM;

typedef struct _HWI2C_INSTANT_PRIVATE
{
    IOCTL_MDrv_HWI2C_Init               fpHWI2CInit;
    IOCTL_MDrv_HWI2C_Start              fpHWI2CStart;
    IOCTL_MDrv_HWI2C_Stop               fpHWI2CStop;
    IOCTL_MDrv_HWI2C_GetPortIndex       fpHWI2CGetPortIndex;
    IOCTL_MDrv_HWI2C_SelectPort         fpHWI2CSelectPort;

    IOCTL_MDrv_HWI2C_SetClk             fpHWI2CSetClk;
    IOCTL_MDrv_HWI2C_SetReadMode        fpHWI2CSetReadMode;
    IOCTL_MDrv_HWI2C_WriteByte          fpHWI2CWriteByte;
    IOCTL_MDrv_HWI2C_WriteBytes         fpHWI2CWriteBytes ;
    IOCTL_MDrv_HWI2C_ReadByte           fpHWI2CReadByte;
    IOCTL_MDrv_HWI2C_ReadBytes          fpHWI2CReadBytes;
    IOCTL_MDrv_HWI2C_SelectPort1        fpHWI2CSelectPort1;
    IOCTL_MDrv_HWI2C_SetClkP1           fpHWI2CSetClkP1;
    IOCTL_MDrv_HWI2C_SetReadModeP1      fpHWI2CSetReadModeP1;
    IOCTL_MDrv_HWI2C_WriteByteP1        fpHWI2CWriteByteP1;
    IOCTL_MDrv_HWI2C_WriteBytesP1       fpHWI2CWriteBytesP1;
    IOCTL_MDrv_HWI2C_ReadByteP1         fpHWI2CReadByteP1 ;
    IOCTL_MDrv_HWI2C_ReadBytesP1        fpHWI2CReadBytesP1;
    IOCTL_MDrv_HWI2C_SetDbgLevel        fpHWI2CSetDbgLevel;
    IOCTL_MDrv_HWI2C_GetLibVer          fpHWI2CGetLibVer ;
    IOCTL_MDrv_HWI2C_GetStatus          fpHWI2CGetStatus ;
    IOCTL_MDrv_HWI2C_GetInfo            fpHWI2CGetInfo;
    //IOCTL_MDrv_HWI2C_SetPowerState      fpHWI2CSetPowerState;
}HWI2C_INSTANT_PRIVATE;


void HWI2CRegisterToUtopia(void);
MS_U32 HWI2COpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute);
//MS_U32 HWI2COpen(void** pInstantTmp, void* pAttribute);
MS_U32 HWI2CClose(void* pInstantTmp);
MS_U32 HWI2CIoctl(void* pInstantTmp, MS_U32 u32Cmd, void* pArgs);

#ifdef __cplusplus
}
#endif
#endif // _DRVHWI2C_PRIV_H_


