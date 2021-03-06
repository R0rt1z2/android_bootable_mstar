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
/// @brief  SWI2C interface
/// @version 1.0
/////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \defgroup G_PERIPHERAL Peripheral modules
	\defgroup G_SWI2C SWI2C interface 
    \ingroup  G_PERIPHERAL

    \brief 
 
	IIC is a multi-master, multi-slave, single-ended, serial computer bus. It is typically used for attaching lower-speed peripheral 
	ICs to processors and micro-controllers. Alternatively I2C is spelled I2C (pronounced I-two-C) or IIC (pronounced I-I-C).
	
	apiSWI2C using GPIOs(SDA and SCL) to emulate IIC protocol.
    
    <b>Features</b>
    - IIC Initialize & De-initialize function
    - IIC Send Data
    - IIC Receive Data

    <b> Operation Code Flow: </b> \n
    -# I2C Start Condition.
    -# Send data.
    -# Receive data.
	-# I2C End Condition.

    \defgroup G_SWI2C_INIT Initialization Task relative
    \ingroup  G_SWI2C
    \defgroup G_SWI2C_COMMON read/write Task relative
    \ingroup  G_SWI2C
    \defgroup G_SWI2C_BUS I2C Bus relative
    \ingroup  G_SWI2C
    \defgroup G_SWI2C_OTHER  other relative
    \ingroup  G_SWI2C
    \defgroup G_SWI2C_ToBeModified SWI2C api to be modified
    \ingroup  G_SWI2C
    \defgroup G_SWI2C_ToBeRemove SWI2C api to be removed
    \ingroup  G_SWI2C
*/

#ifndef _API_SWI2C_H_
#define _API_SWI2C_H_

#include "MsCommon.h"

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
/// define IR library version
#define MSIF_SWI2C_LIB_CODE             {'S','I','2','C'}
#define MSIF_SWI2C_LIBVER               {'0','5'}
#define MSIF_SWI2C_BUILDNUM             {'0','5' }
#define MSIF_SWI2C_CHANGELIST           {'0','0','4','9','7','5','7','8'}

#define SWI2C_API_VERSION                 /* Character String for DRV/API version */  \
      MSIF_TAG,                           /* 'MSIF' */  \
      MSIF_CLASS,                         /* '00' */  \
      MSIF_CUS,                           /* 0x0000 */  \
      MSIF_MOD,                           /* 0x0000 */  \
      MSIF_CHIP,                                       \
      MSIF_CPU,                                        \
      MSIF_SWI2C_LIB_CODE,                /* IP__ */  \
      MSIF_SWI2C_LIBVER,                  /* 0.0 ~ Z.Z */  \
      MSIF_SWI2C_BUILDNUM,                /* 00 ~ 99 */  \
      MSIF_SWI2C_CHANGELIST,              /* CL# */  \
      MSIF_OS

// Speed Mapping: Base on the Driver loading, maybe you must to use the oscilloscope to measure this Value
// Speed Mapping has been refined, and you can specify the IIC speed X (KHz) driectly by X.
#define SWI2C_SPEED_MAPPING_400K          400
#define SWI2C_SPEED_MAPPING_350K          350
#define SWI2C_SPEED_MAPPING_300K          300
#define SWI2C_SPEED_MAPPING_250K          250
#define SWI2C_SPEED_MAPPING_200K          200
#define SWI2C_SPEED_MAPPING_150K          150
#define SWI2C_SPEED_MAPPING_100K          100
#define SWI2C_SPEED_MAPPING_50K           50

/// emurate SWI2C fucntion return result
typedef enum
{
    /// SWI2C result for failure
    E_SWI2C_FAIL =0,
    /// SWI2C result for OK
    E_SWI2C_OK = 1,

} SWI2C_Result;

///Define SWI2C Bus Configuration
typedef struct
{
    MS_U16 padSCL;
    MS_U16 padSDA;
    MS_U16 defDelay;
} SWI2C_BusCfg;

///Define SWI2C read mode
typedef enum {
    E_SWI2C_READ_MODE_DIRECT,                 ///< first transmit slave address + reg address and then start receive the data */
    E_SWI2C_READ_MODE_DIRECTION_CHANGE,       ///< slave address + reg address in write mode, direction change to read mode, repeat start slave address in read mode, data from device
    E_SWI2C_READ_MODE_DIRECTION_CHANGE_STOP_START,  ///< slave address + reg address in write mode + stop, direction change to read mode, repeat start slave address in read mode, data from device
    E_SWI2C_READ_MODE_MAX
} SWI2C_ReadMode;

///Define SWI2C debug level
typedef enum _SWI2C_DbgLvl
{
    E_SWI2C_DBGLVL_NONE = 0,      /// no debug message
    E_SWI2C_DBGLVL_WARNING,       /// show warning only
    E_SWI2C_DBGLVL_ERROR,         /// show error only
    E_SWI2C_DBGLVL_INFO,          /// show error & informaiton
    E_SWI2C_DBGLVL_ALL,           /// show error, information & funciton name
}SWI2C_DbgLvl;

typedef enum _SWI2C_CfgCmd
{
    E_SWI2C_CFGCMD_TRANSRTY=0,  /// do transcation retry
    E_SWI2C_CFGCMD_SNDBYRTY,  /// do send byte retry
    E_SWI2C_CFGCMD_NONE,                  /// do nothing
}SWI2C_CfgCmd;

///Define SWI2C bus configuration parameters
typedef struct
{
    MS_U32 u32CfgParam[8] ; /// configuration parameter list
    SWI2C_CfgCmd  cfgCmd;      /// configuration commands
    MS_U8 u8BusNum;               /// bus number to be configured
    MS_BOOL bByBus;                 ///0: configure by bus, 1: configure for all buses
    MS_BOOL bRecoveried;      ///0: set by specified value, 1: recoveried back to default value
} SWI2C_Config;
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SWI2C_INIT
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void MApi_SWI2C_Init(SWI2C_BusCfg SWI2CBusCfg[],MS_U8 u8CfgBusNum);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SWI2C_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_SWI2C_WriteBytes(MS_U16 u16BusNumSlaveID, MS_U8 u8addrcount, MS_U8* pu8addr, MS_U16 u16size, MS_U8* pu8data);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SWI2C_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_SWI2C_WriteBytesStop(MS_U16 u16BusNumSlaveID, MS_U8 AddrCnt, MS_U8* pu8addr, MS_U16 u16size, MS_U8* pBuf,MS_BOOL bGenStop);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SWI2C_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_SWI2C_ReadBytes(MS_U16 u16BusNumSlaveID, MS_U8 u8AddrNum, MS_U8* paddr, MS_U16 u16size, MS_U8* pu8data);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SWI2C_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_SWI2C_ReadBytes_ThruMode(MS_U16 u16BusNumSlaveID, MS_U8 ucSubAdr, MS_U8* paddr, MS_U16 ucBufLen, MS_U8* pBuf);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SWI2C_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_SWI2C_ReadByte(MS_U16 u16BusNumSlaveID, MS_U8 u8RegAddr, MS_U8 *pu8Data);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SWI2C_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_SWI2C_WriteByte(MS_U16 u16BusNumSlaveID, MS_U8 u8RegAddr, MS_U8 u8Data);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SWI2C_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_SWI2C_Write2Bytes(MS_U16 u16BusNumSlaveID, MS_U8 u8addr, MS_U16 u16data);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SWI2C_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_U16 MApi_SWI2C_Read2Bytes(MS_U16 u16BusNumSlaveID, MS_U8 u8addr);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SWI2C_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_SWI2C_Write4Bytes(MS_U16 u16BusNumSlaveID, MS_U32 u32Data, MS_U8 u8EndData);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SWI2C_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_SWI2C_ReadByteDirectly(MS_U16 u16BusNumSlaveID, MS_U8 * pu8Data);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SWI2C_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_SWI2C_WriteGroupBytes(MS_U16 u16BusNumSlaveID, MS_U8 u8SubGroup, MS_U16 u16Addr, MS_U16 u16Data);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SWI2C_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_U16 MApi_SWI2C_ReadGroupBytes(MS_U16 u16BusNumSlaveID, MS_U8 u8SubGroup, MS_U16 u16Addr);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SWI2C_BUS
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_U8 MApi_SWI2C_GetMaxBuses(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SWI2C_BUS
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_U32 MApi_SWI2C_Speed_Setting(MS_U8 u8BusNum, MS_U32 u32Speed_K);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SWI2C_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_SWI2C_SetReadMode(SWI2C_ReadMode eReadMode);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SWI2C_BUS
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_SWI2C_SetBusReadMode(MS_U8 u8BusNum, SWI2C_ReadMode eReadMode);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SWI2C_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_SWI2C_GetLibVer(const MSIF_Version **ppVersion);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SWI2C_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
SWI2C_Result MApi_SWI2C_SetDbgLevel(SWI2C_DbgLvl eLevel);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SWI2C_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_U16 MApi_SWI2C_SetPowerState(EN_POWER_MODE u16PowerState);

/******************************************************************************/
///
/// The following API functions are packed for special usage in ISP programming
///
/******************************************************************************/
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SWI2C_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void MApi_SWI2C_MutexLock(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SWI2C_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void MApi_SWI2C_MutexUnlock(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SWI2C_BUS
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void MApi_SWI2C_UseBus( MS_U8 u8BusChn );
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SWI2C_BUS
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void MApi_SWI2C_UnuseBus(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SWI2C_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_SWI2C_AccessStart(MS_U8 u8SlaveID, MS_U8 trans_t);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SWI2C_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void MApi_SWI2C_Stop(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SWI2C_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_SWI2C_Start(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SWI2C_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_SWI2C_SendByte(MS_U8 u8date);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SWI2C_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_U8 MApi_SWI2C_GetByte(MS_U16 u16Ack);

void MApi_SWI2C_EnableRetry(MS_BOOL bEnable);

#ifdef __cplusplus
}
#endif

#endif // _API_SWI2C_H_
