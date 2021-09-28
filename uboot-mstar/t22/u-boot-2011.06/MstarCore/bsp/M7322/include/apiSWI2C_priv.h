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

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @brief  SWI2C Driver Interface
///
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _APISWI2C_PRIV_H_
#define _APISWI2C_PRIV_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "MsTypes.h"
#include "apiSWI2C.h"

#if defined (CONFIG_MSTAR_SWI2C_KERNEL_MODIIC_DEVICE_ENABLE)
typedef struct DLL_PACKED _SWI2C_IIC_Param
{
    MS_U8 u8IICType;         /// IICType: 0:SWI2C 1:HWI2C
    MS_U8 u8IdIIC;           /// IIC ID: Channel 1~7
    MS_U8 u8ClockIIC;        /// IIC clock speed
    MS_U8 u8SlaveIdIIC;      /// Device slave ID
    MS_U8 u8AddrSizeIIC;     /// Address length in bytes
    MS_U8 *u8AddrIIC;        /// Starting address inside the device
    MS_U8 *u8pbufIIC;        /// buffer
    MS_U32 u32DataSizeIIC;   /// size of buffer
} SWI2C_IIC_Param, *PSWI2C_IIC_Param;
#endif

typedef struct DLL_PACKED _SWI2C_PRIVATE_Init_PARAM
{
    SWI2C_BusCfg* SWI2CBusCfg;
#if !defined (__aarch64__)
    MS_U32 SWI2CBusCfgDummy;
#endif
    MS_U8 u8CfgBusNum;
} SWI2C_PRIVATE_Init_PARAM, *PSWI2C_PRIVATE_Init_PARAM;

typedef struct DLL_PACKED _SWI2C_PRIVATE_PARAM_WriteBytes
{
    MS_U16 u16BusNumSlaveID;
    MS_U8 u8addrcount;
    MS_U8* pu8addr;
    MS_U16 u16size;
    MS_U8* pu8data;
} SWI2C_PRIVATE_WriteBytes_PARAM, *PSWI2C_PRIVATE_WriteBytes_PARAM;

typedef struct DLL_PACKED _SWI2C_PRIVATE_WriteBytesStop_PARAM
{
    MS_U16 u16BusNumSlaveID;
    MS_U8 AddrCnt;
    MS_U8* pu8addr;
    MS_U16 u16size;
    MS_U8* pBuf;
    MS_BOOL bGenStop;
} SWI2C_PRIVATE_WriteBytesStop_PARAM, *PSWI2C_PRIVATE_WriteBytesStop_PARAM;

typedef struct DLL_PACKED _SWI2C_PRIVATE_ReadBytes_PARAM
{
    MS_U16 u16BusNumSlaveID;
    MS_U8 u8AddrNum;
    MS_U8* paddr;
    MS_U16 u16size;
    MS_U8* pu8data;
} SWI2C_PRIVATE_ReadBytes_PARAM, *PSWI2C_PRIVATE_ReadBytes_PARAM;

typedef struct DLL_PACKED _SWI2C_PRIVATE_ReadBytes_ThruMode_PARAM
{
    MS_U16 u16BusNumSlaveID;
    MS_U8 ucSubAdr;
    MS_U8* paddr;
    MS_U16 ucBufLen;
    MS_U8* pBuf;
} SWI2C_PRIVATE_ReadBytes_ThruMode_PARAM, *PSWI2C_PRIVATE_ReadBytes_ThruMode_PARAM;

typedef struct DLL_PACKED _SWI2C_PRIVATE_ReadByte_PARAM
{
    MS_U16 u16BusNumSlaveID;
    MS_U8 u8RegAddr;
    MS_U8 *pu8Data;
} SWI2C_PRIVATE_ReadByte_PARAM, *PSWI2C_PRIVATE_ReadByte_PARAM;

typedef struct DLL_PACKED _SWI2C_PRIVATE_WriteByte_PARAM
{
    MS_U16 u16BusNumSlaveID;
    MS_U8 u8RegAddr;
    MS_U8 u8Data;
} SWI2C_PRIVATE_WriteByte_PARAM, *PSWI2C_PRIVATE_WriteByte_PARAM;

typedef struct DLL_PACKED _SWI2C_PRIVATE_Write2Bytes_PARAM
{
    MS_U16 u16BusNumSlaveID;
    MS_U8 u8addr;
    MS_U16 u16data;
} SWI2C_PRIVATE_Write2Bytes_PARAM, *PSWI2C_PRIVATE_Write2Bytes_PARAM;

typedef struct DLL_PACKED _SWI2C_PRIVATE_Read2Bytes_PARAM
{
    MS_U16 u16BusNumSlaveID;
    MS_U8 u8addr;
} SWI2C_PRIVATE_Read2Bytes_PARAM, *PSWI2C_PRIVATE_Read2Bytes_PARAM;

typedef struct DLL_PACKED _SWI2C_PRIVATE_Write4Bytes_PARAM
{
    MS_U16 u16BusNumSlaveID;
    MS_U32 u32Data;
    MS_U8 u8EndData;
} SWI2C_PRIVATE_Write4Bytes_PARAM, *PSWI2C_PRIVATE_Write4Bytes_PARAM;

typedef struct DLL_PACKED _SWI2C_PRIVATE_WriteGroupBytes_PARAM
{
    MS_U16 u16BusNumSlaveID;
    MS_U8 u8SubGroup;
    MS_U16 u16Addr;
    MS_U16 u16Data;
} SWI2C_PRIVATE_WriteGroupBytes_PARAM, *PSWI2C_PRIVATE_WriteGroupBytes_PARAM;

typedef struct DLL_PACKED _SWI2C_PRIVATE_ReadGroupBytes_PARAM
{
    MS_U16 u16BusNumSlaveID;
    MS_U8 u8SubGroup;
    MS_U16 u16Addr;
} SWI2C_PRIVATE_ReadGroupBytes_PARAM, *PSWI2C_PRIVATE_ReadGroupBytes_PARAM;

typedef struct DLL_PACKED _SWI2C_PRIVATE_PARAM_Speed_Setting_PARAM
{
    MS_U8 u8BusNum;
    MS_U32 u32Speed_K;
} SWI2C_PRIVATE_Speed_Setting_PARAM, *PSWI2C_PRIVATE_Speed_Setting_PARAM;


typedef struct DLL_PACKED _SWI2C_PRIVATE_SetReadMode_PARAM
{
    SWI2C_ReadMode eReadMode;
} SWI2C_PRIVATE_SetReadMode_PARAM, *PSWI2C_PRIVATE_SetReadMode_PARAM;

typedef struct DLL_PACKED _SWI2C_PRIVATE_SetBusReadMode_PARAM
{
    MS_U8 u8BusNum;
    SWI2C_ReadMode eReadMode;
} SWI2C_PRIVATE_SetBusReadMode_PARAM, *PSWI2C_PRIVATE_SetBusReadMode_PARAM;

typedef struct DLL_PACKED _SWI2C_PRIVATE_GetLibVer_PARAM
{
    const MSIF_Version **ppVersion;
#if !defined (__aarch64__)
    MS_U32 ppVersionDummy;
#endif
} SWI2C_PRIVATE_GetLibVer_PARAM, *PSWI2C_PRIVATE_GetLibVer_PARAM;

typedef struct DLL_PACKED _SWI2C_PRIVATE_SetDbgLevel_PARAM
{
    SWI2C_DbgLvl eLevel;
} SWI2C_PRIVATE_SetDbgLevel_PARAM, *PSWI2C_PRIVATE_SetDbgLevel_PARAM;

#if 0
typedef struct _SWI2C_PRIVATE_PARAM_SetPowerState
{
// one pointer memberEN_POWER_MODE u16PowerState
} SWI2C_PRIVATE_PARAM_SetPowerState;
#endif

//ioctl function pointer type
typedef MS_BOOL (*IOCTL_SWI2C_INIT)(SWI2C_BusCfg[], MS_U8 u8CfgBusNum);
typedef MS_BOOL (*IOCTL_SWI2C_WRITEBYTES)(MS_U16, MS_U8, MS_U8*, MS_U16, MS_U8*);
typedef MS_BOOL (*IOCTL_SWI2C_WRITEBYTESSTOP)(MS_U16, MS_U8, MS_U8*, MS_U16, MS_U8*,MS_BOOL);
typedef MS_BOOL (*IOCTL_SWI2C_READBYTES)(MS_U16, MS_U8, MS_U8*, MS_U16, MS_U8*);
typedef MS_BOOL (*IOCTL_SWI2C_READBYTESTHRUMODE)(MS_U16, MS_U8, MS_U8*, MS_U16, MS_U8*);
typedef MS_BOOL (*IOCTL_SWI2C_READBYTE)(MS_U16, MS_U8, MS_U8*);
typedef MS_BOOL (*IOCTL_SWI2C_WRITEBYTE)(MS_U16, MS_U8, MS_U8);
typedef MS_BOOL (*IOCTL_SWI2C_WRITE2BYTES)(MS_U16, MS_U8, MS_U16);
typedef MS_BOOL (*IOCTL_SWI2C_READ2BYTES)(MS_U16, MS_U8);
typedef MS_BOOL (*IOCTL_SWI2C_WRITE4BYTES)(MS_U16, MS_U32, MS_U8);
typedef MS_BOOL (*IOCTL_SWI2C_WRITEGROUPBYTES)(MS_U16, MS_U8, MS_U16, MS_U16);
typedef MS_BOOL (*IOCTL_SWI2C_READGROUPBYTES)(MS_U16, MS_U8, MS_U16);
typedef MS_BOOL (*IOCTL_SWI2C_GETMAXBUSES)(void);
typedef MS_BOOL (*IOCTL_SWI2C_SPEEDSETTING)(MS_U8, MS_U32);
typedef MS_BOOL (*IOCTL_SWI2C_SETREADMODE)(SWI2C_ReadMode);
typedef MS_BOOL (*IOCTL_SWI2C_SETBUSREADMODE)(MS_U8, SWI2C_ReadMode);
typedef MS_BOOL (*IOCTL_SWI2C_GETLIBVER)(const MSIF_Version**);
typedef MS_BOOL (*IOCTL_SWI2C_SETDBGLEVEL)(SWI2C_DbgLvl);

typedef struct _SWI2C_RESOURCE_PRIVATE
{
    MS_U32 Dummy;
}SWI2C_RESOURCE_PRIVATE;

typedef struct _SWI2C_INSTANT_PRIVATE
{
    IOCTL_SWI2C_INIT fpSWI2CInit;
    IOCTL_SWI2C_WRITEBYTES fpSWI2CWriteBytes;
    IOCTL_SWI2C_WRITEBYTESSTOP fpSWI2CWriteBytesStop;
    IOCTL_SWI2C_READBYTES fpSWI2CReadBytes;
    IOCTL_SWI2C_READBYTESTHRUMODE fpSWI2CReadBytesThruMode;
    IOCTL_SWI2C_READBYTE fpSWI2CReadByte;
    IOCTL_SWI2C_WRITEBYTE fpSWI2CWriteByte;
    IOCTL_SWI2C_WRITE2BYTES fpSWI2CWrite2Bytes;
    IOCTL_SWI2C_READ2BYTES fpSWI2CRead2Bytes;
    IOCTL_SWI2C_WRITE4BYTES fpSWI2CWrite4Bytes;
    IOCTL_SWI2C_WRITEGROUPBYTES fpSWI2CWriteGroupBytes;
    IOCTL_SWI2C_READGROUPBYTES fpSWI2CReadGroupBytes;
    IOCTL_SWI2C_GETMAXBUSES fpSWI2CGetMaxBuses;
    IOCTL_SWI2C_SPEEDSETTING fpSWI2CSpeedSetting;
    IOCTL_SWI2C_SETREADMODE fpSWI2CSetReadMode;
    IOCTL_SWI2C_SETBUSREADMODE fpSWI2CSetBusReadMode;
    IOCTL_SWI2C_GETLIBVER fpSWI2CGetLibVer;
    IOCTL_SWI2C_SETDBGLEVEL fpSWI2CSetDbgLevel;
}SWI2C_INSTANT_PRIVATE;

typedef enum {
    MApi_CMD_SWI2C_Init,
    MApi_CMD_SWI2C_WriteBytes,
    MApi_CMD_SWI2C_WriteBytesStop,
    MApi_CMD_SWI2C_ReadBytes,
    MApi_CMD_SWI2C_ReadBytes_ThruMode,
    MApi_CMD_SWI2C_ReadByte,
    MApi_CMD_SWI2C_WriteByte,
    MApi_CMD_SWI2C_Write2Bytes,
    MApi_CMD_SWI2C_Read2Bytes,
    MApi_CMD_SWI2C_Write4Bytes,
    MApi_CMD_SWI2C_WriteGroupBytes,
    MApi_CMD_SWI2C_ReadGroupBytes,
    MApi_CMD_SWI2C_GetMaxBuses,
    MApi_CMD_SWI2C_Speed_Setting,
    MApi_CMD_SWI2C_SetReadMode,
    MApi_CMD_SWI2C_SetBusReadMode,
    MApi_CMD_SWI2C_GetLibVer,
    MApi_CMD_SWI2C_SetDbgLevel,
    //MApi_CMD_SWI2C_SetPowerState,
} eSWI2CIoctlOpt;
//////////////////////////////////////
// function for utopia2.0 ////////////
//////////////////////////////////////
void SWI2CRegisterToUtopia(void);
MS_U32 SWI2COpen(void** pInstance, MS_U32 u32ModuleVersion, void* pAttribute);
MS_U32 SWI2CClose(void* pInstance);
MS_U32 SWI2CIoctl(void* pInstance, MS_U32 u32Cmd, void *pArgs);

void _MApi_SWI2C_Init(SWI2C_BusCfg SWI2CBusCfg[],MS_U8 u8CfgBusNum);
MS_BOOL _MApi_SWI2C_WriteBytes(MS_U16 u16BusNumSlaveID, MS_U8 u8addrcount, MS_U8* pu8addr, MS_U16 u16size, MS_U8* pu8data);
MS_BOOL _MApi_SWI2C_WriteBytesStop(MS_U16 u16BusNumSlaveID, MS_U8 AddrCnt, MS_U8* pu8addr, MS_U16 u16size, MS_U8* pBuf,MS_BOOL bGenStop);
MS_BOOL _MApi_SWI2C_ReadBytes(MS_U16 u16BusNumSlaveID, MS_U8 u8AddrNum, MS_U8* paddr, MS_U16 u16size, MS_U8* pu8data);
MS_BOOL _MApi_SWI2C_ReadBytes_ThruMode(MS_U16 u16BusNumSlaveID, MS_U8 ucSubAdr, MS_U8* paddr, MS_U16 ucBufLen, MS_U8* pBuf);
MS_BOOL _MApi_SWI2C_ReadByte(MS_U16 u16BusNumSlaveID, MS_U8 u8RegAddr, MS_U8 *pu8Data);
MS_BOOL _MApi_SWI2C_WriteByte(MS_U16 u16BusNumSlaveID, MS_U8 u8RegAddr, MS_U8 u8Data);
MS_BOOL _MApi_SWI2C_Write2Bytes(MS_U16 u16BusNumSlaveID, MS_U8 u8addr, MS_U16 u16data);
MS_U16 _MApi_SWI2C_Read2Bytes(MS_U16 u16BusNumSlaveID, MS_U8 u8addr);
MS_BOOL _MApi_SWI2C_Write4Bytes(MS_U16 u16BusNumSlaveID, MS_U32 u32Data, MS_U8 u8EndData);
//MS_BOOL _MApi_SWI2C_ReadByteDirectly(MS_U16 u16BusNumSlaveID, MS_U8 * pu8Data);
MS_BOOL _MApi_SWI2C_WriteGroupBytes(MS_U16 u16BusNumSlaveID, MS_U8 u8SubGroup, MS_U16 u16Addr, MS_U16 u16Data);
MS_U16 _MApi_SWI2C_ReadGroupBytes(MS_U16 u16BusNumSlaveID, MS_U8 u8SubGroup, MS_U16 u16Addr);
MS_U8 _MApi_SWI2C_GetMaxBuses(void);
MS_U32 _MApi_SWI2C_Speed_Setting(MS_U8 u8BusNum, MS_U32 u32Speed_K);
MS_BOOL _MApi_SWI2C_SetReadMode(SWI2C_ReadMode eReadMode);
MS_BOOL _MApi_SWI2C_SetBusReadMode(MS_U8 u8BusNum, SWI2C_ReadMode eReadMode);
MS_BOOL _MApi_SWI2C_GetLibVer(const MSIF_Version **ppVersion);
SWI2C_Result _MApi_SWI2C_SetDbgLevel(SWI2C_DbgLvl eLevel);

#if defined(MSOS_TYPE_LINUX_KERNEL)
extern int MDrv_SW_IIC_WriteBytes(u8 u8BusNum, u8 u8SlaveID, u8 u8addrcount, u8* pu8addr, u16 u16size, u8* pu8data);
extern int MDrv_SW_IIC_ReadBytes(u8 u8BusNum, u8 u8SlaveID, u8 u8AddrNum, u8* paddr, u16 u16size, u8* pu8data);
extern int MDrv_SW_IIC_WriteBytesStop(u8 u8BusNum, u8 u8SlaveID, u8 AddrCnt, u8* pu8addr, u16 u16size, u8* pBuf, int bGenStop);
#endif

#endif
