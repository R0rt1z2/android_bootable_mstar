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
/*! \defgroup G_HWI2C HWI2C interface
    \ingroup  G_PERIPHERAL
	
    \brief 
 
	IIC is a multi-master, multi-slave, single-ended, serial computer bus. It is typically used for attaching lower-speed peripheral 
	ICs to processors and micro-controllers. Alternatively I2C is spelled I2C (pronounced I-two-C) or IIC (pronounced I-I-C).
    
    <b>Features</b>
    - IIC Initialize & De-initialize function
    - IIC Send Data
    - IIC Receive Data

    <b> Operation Code Flow: </b> \n
    -# I2C Start Condition.
    -# Send data.
    -# Receive data.
	-# I2C End Condition.

    \defgroup G_HWI2C_INIT Initialization Task relative
    \ingroup  G_HWI2C
    \defgroup G_HWI2C_COMMON Common Task relative
    \ingroup  G_HWI2C
    \defgroup G_HWI2C_CONTROL Control relative
    \ingroup  G_HWI2C
    \defgroup G_HWI2C_INT  Interrupt relative
    \ingroup  G_HWI2C
    \defgroup G_HWI2C_ToBeModified GPIO api to be modified
    \ingroup  G_HWI2C
    \defgroup G_HWI2C_ToBeRemove GPIO api to be removed
    \ingroup  G_HWI2C
*/

#ifndef _DRV_HWI2C_H_
#define _DRV_HWI2C_H_

////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C"
{
#endif

#include "MsTypes.h"
#include "MsDevice.h"

#define HWI2C_UTOPIA20 (0)
////////////////////////////////////////////////////////////////////////////////
// Define & data type
////////////////////////////////////////////////////////////////////////////////
#define MSIF_HWI2C_LIB_CODE                     {'H','I','2','C'}    //Lib code
#define MSIF_HWI2C_LIBVER                       {'0','6'}            //LIB version
#define MSIF_HWI2C_BUILDNUM                     {'0','6'}            //Build Number
#define MSIF_HWI2C_CHANGELIST                   {'0','0','5','6','2','3','3','3'} //P4 ChangeList Number

#define HWI2C_DRV_VERSION               /* Character String for DRV/API version             */  \
    MSIF_TAG,                           /* 'MSIF'                                           */  \
    MSIF_CLASS,                         /* '00'                                             */  \
    MSIF_CUS,                           /* 0x0000                                           */  \
    MSIF_MOD,                           /* 0x0000                                           */  \
    MSIF_CHIP,                                                                                  \
    MSIF_CPU,                                                                                   \
    MSIF_HWI2C_LIB_CODE,                /* IP__                                             */  \
    MSIF_HWI2C_LIBVER,                  /* 0.0 ~ Z.Z                                        */  \
    MSIF_HWI2C_BUILDNUM,                /* 00 ~ 99                                          */  \
    MSIF_HWI2C_CHANGELIST,              /* CL#                                              */  \
    MSIF_OS

/// debug level
typedef enum _HWI2C_DbgLv
{
    E_HWI2C_DBGLV_NONE,          /// no debug message
    E_HWI2C_DBGLV_ERR_ONLY,      /// show error only
    E_HWI2C_DBGLV_INFO,          /// show error & informaiton
    E_HWI2C_DBGLV_ALL            /// show error, information & funciton name
}HWI2C_DbgLv;

/// I2C select master port
typedef enum _HWI2C_PORT
{
    E_HWI2C_PORT_0 = 0, /// port 0_0 //disable port 0
    E_HWI2C_PORT0_1,    /// port 0_1
    E_HWI2C_PORT0_2,    /// port 0_2
    E_HWI2C_PORT0_3,    /// port 0_3
    E_HWI2C_PORT0_4,    /// port 0_4
    E_HWI2C_PORT0_5,    /// port 0_5
    E_HWI2C_PORT0_6,    /// port 0_6
    E_HWI2C_PORT0_7,    /// port 0_7

    E_HWI2C_PORT_1 = 8, /// port 1_0 //disable port 1
    E_HWI2C_PORT1_1,    /// port 1_1
    E_HWI2C_PORT1_2,    /// port 1_2
    E_HWI2C_PORT1_3,    /// port 1_3
    E_HWI2C_PORT1_4,    /// port 1_4
    E_HWI2C_PORT1_5,    /// port 1_5
    E_HWI2C_PORT1_6,    /// port 1_6
    E_HWI2C_PORT1_7,    /// port 1_7

    E_HWI2C_PORT_2 = 16,/// port 2_0 //disable port 2
    E_HWI2C_PORT2_1,    /// port 2_1
    E_HWI2C_PORT2_2,    /// port 2_2
    E_HWI2C_PORT2_3,    /// port 2_3
    E_HWI2C_PORT2_4,    /// port 2_4
    E_HWI2C_PORT2_5,    /// port 2_5
    E_HWI2C_PORT2_6,    /// port 2_6
    E_HWI2C_PORT2_7,    /// port 2_7

    E_HWI2C_PORT_3 = 24,/// port 3_0 //disable port 3
    E_HWI2C_PORT3_1,    /// port 3_1
    E_HWI2C_PORT3_2,    /// port 3_2
    E_HWI2C_PORT3_3,    /// port 3_3
    E_HWI2C_PORT3_4,    /// port 3_4
    E_HWI2C_PORT3_5,    /// port 3_5
    E_HWI2C_PORT3_6,    /// port 3_6
    E_HWI2C_PORT3_7,    /// port 3_7

    E_HWI2C_PORT_4 = 32,/// port 4_0 //disable port 4
    E_HWI2C_PORT4_1,    /// port 4_1
    E_HWI2C_PORT4_2,    /// port 4_2
    E_HWI2C_PORT4_3,    /// port 4_3
    E_HWI2C_PORT4_4,    /// port 4_4
    E_HWI2C_PORT4_5,    /// port 4_5
    E_HWI2C_PORT4_6,    /// port 4_6
    E_HWI2C_PORT4_7,    /// port 4_7

    E_HWI2C_PORT_5 = 40,/// port 5_0 //disable port 5
    E_HWI2C_PORT5_1,    /// port 5_1
    E_HWI2C_PORT5_2,    /// port 5_2
    E_HWI2C_PORT5_3,    /// port 5_3
    E_HWI2C_PORT5_4,    /// port 5_4
    E_HWI2C_PORT5_5,    /// port 5_5
    E_HWI2C_PORT5_6,    /// port 5_6
    E_HWI2C_PORT5_7,    /// port 5_7

    E_HWI2C_PORT_6 = 48,/// port 6_0 //disable port 6
    E_HWI2C_PORT6_1,    /// port 6_1
    E_HWI2C_PORT6_2,    /// port 6_2
    E_HWI2C_PORT6_3,    /// port 6_3
    E_HWI2C_PORT6_4,    /// port 6_4
    E_HWI2C_PORT6_5,    /// port 6_5
    E_HWI2C_PORT6_6,    /// port 6_6
    E_HWI2C_PORT6_7,    /// port 6_7

    E_HWI2C_PORT_NOSUP  /// non-support port
}HWI2C_PORT;

/// I2C clock speed select
typedef enum _HWI2C_CLKSEL
{
    E_HWI2C_HIGH = 0,  /// high speed
    E_HWI2C_NORMAL,    /// normal speed
    E_HWI2C_SLOW,      /// slow speed
    E_HWI2C_VSLOW,     /// very slow
    E_HWI2C_USLOW,     /// ultra slow
    E_HWI2C_UVSLOW,    /// ultra-very slow
    E_HWI2C_NOSUP      /// non-support speed
}HWI2C_CLKSEL;

/// I2C state
typedef enum _HWI2C_State
{
    E_HWI2C_IDLE,      /// idle state
    E_HWI2C_READ_DATA, /// read data state
    E_HWI2C_WRITE_DATA, /// write data state
    E_HWI2C_DMA_READ_DATA, /// DMA read data state
    E_HWI2C_DMA_WRITE_DATA /// DMA write data state
}HWI2C_State;

typedef enum {
    E_HWI2C_READ_MODE_DIRECT,                 ///< first transmit slave address + reg address and then start receive the data */
    E_HWI2C_READ_MODE_DIRECTION_CHANGE,       ///< slave address + reg address in write mode, direction change to read mode, repeat start slave address in read mode, data from device
    E_HWI2C_READ_MODE_DIRECTION_CHANGE_STOP_START,  ///< slave address + reg address in write mode + stop, direction change to read mode, repeat start slave address in read mode, data from device
    E_HWI2C_READ_MODE_MAX
} HWI2C_ReadMode;

typedef enum _HWI2C_DMA_ADDRMODE
{
    E_HWI2C_DMA_ADDR_NORMAL = 0,
    E_HWI2C_DMA_ADDR_10BIT,
    E_HWI2C_DMA_ADDR_MAX,
}HWI2C_DMA_ADDRMODE;

typedef enum _HWI2C_DMA_READMODE
{
    E_HWI2C_DMA_READ_NOSTOP = 0,
    E_HWI2C_DMA_READ_STOP,
    E_HWI2C_DMA_READ_MAX,
}HWI2C_DMA_READMODE;

typedef enum _HWI2C_DMA_MIUPRI
{
    E_HWI2C_DMA_PRI_LOW = 0,
    E_HWI2C_DMA_PRI_HIGH,
    E_HWI2C_DMA_PRI_MAX,
}HWI2C_DMA_MIUPRI;

typedef enum _HWI2C_DMA_MIUCH
{
    E_HWI2C_DMA_MIU_CH0 = 0,
    E_HWI2C_DMA_MIU_CH1,
    E_HWI2C_DMA_MIU_MAX,
}HWI2C_DMA_MIUCH;

/// I2C master pin config
typedef struct _HWI2C_PinCfg
{
    MS_U32  u32Reg;    /// register
    MS_U8   u8BitPos;  /// bit position
    MS_BOOL bEnable;   /// enable or disable
}HWI2C_PinCfg;

/// I2C port config
typedef struct _HWI2C_PortCfg
{
    MS_U32              u32DmaPhyAddr;  /// DMA physical address
    HWI2C_DMA_ADDRMODE  eDmaAddrMode;   /// DMA address mode
    HWI2C_DMA_MIUPRI    eDmaMiuPri;     /// DMA miu priroity
    HWI2C_DMA_MIUCH     eDmaMiuCh;      /// DMA miu channel
    MS_BOOL             bDmaEnable;     /// DMA enable

    HWI2C_PORT          ePort;          /// number
    HWI2C_CLKSEL        eSpeed;         /// clock speed
    HWI2C_ReadMode      eReadMode;      /// read mode
    MS_BOOL             bEnable;        /// enable

}HWI2C_PortCfg;

/// I2C Configuration for initialization
typedef struct DLL_PACKED _HWI2C_UnitCfg
{
    HWI2C_PortCfg   sCfgPort[8];    /// port cfg info
    HWI2C_PinCfg    sI2CPin;        /// pin info
    HWI2C_CLKSEL    eSpeed;         /// speed
    HWI2C_PORT      ePort;          /// port
    HWI2C_ReadMode  eReadMode;      /// read mode

}HWI2C_UnitCfg;

typedef struct DLL_PACKED _HWI2C_UnitCfg_Sub
{
    HWI2C_PinCfg    sI2CPin;        /// pin info
    HWI2C_CLKSEL    eSpeed;         /// speed
    HWI2C_PORT      ePort;          /// port
    HWI2C_ReadMode  eReadMode;      /// read mode

}HWI2C_UnitCfg_Sub;

/// I2C information
typedef struct _HWI2C_Info
{
    MS_U32          u32IOMap; /// base address
    HWI2C_UnitCfg   sUnitCfg; /// configuration
}HWI2C_Info;

/// I2C status
typedef struct DLL_PACKED _HWI2C_Status
{
    MS_U8       u8DbgLevel;   /// debug level
    MS_BOOL     bIsInit;      /// initialized
    MS_BOOL     bIsMaster;    /// master
    HWI2C_State eState;       /// state
}HWI2C_Status;

typedef struct _HWI2C_PortStatus
{
    MS_U8       u8DbgLevel;   /// debug level
    MS_BOOL     bIsInit;      /// initialized
    MS_BOOL     bIsMaster;    /// master
    HWI2C_State eState;       /// state
    MS_U8       u8Port;
}HWI2C_PortStatus;

typedef struct _HWI2C_DelayConfig
{
    MS_U8 u8Port;
	MS_U32 u32StartDelay;
	MS_U32 u32StopDelay;
}HWI2C_DelayConfig;

////////////////////////////////////////////////////////////////////////////////
// Extern Function
////////////////////////////////////////////////////////////////////////////////
#include "drvHWI2C_v2.h"

//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_HWI2C_INIT
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//------------------------------------------------------------------------------------------------- 
MS_BOOL MDrv_HWI2C_Init(HWI2C_UnitCfg *psCfg);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_HWI2C_CONTROL
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//------------------------------------------------------------------------------------------------- 
MS_BOOL MDrv_HWI2C_Start(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_HWI2C_CONTROL
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//------------------------------------------------------------------------------------------------- 
MS_BOOL MDrv_HWI2C_Stop(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_HWI2C_CONTROL
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//------------------------------------------------------------------------------------------------- 
MS_BOOL MDrv_HWI2C_GetPortIndex(HWI2C_PORT ePort, MS_U8* pu8Port);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_HWI2C_CONTROL
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//------------------------------------------------------------------------------------------------- 
MS_BOOL MDrv_HWI2C_SelectPort(HWI2C_PORT ePort);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_HWI2C_CONTROL
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//------------------------------------------------------------------------------------------------- 
MS_BOOL MDrv_HWI2C_SetClk(HWI2C_CLKSEL eClk);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_HWI2C_CONTROL
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//------------------------------------------------------------------------------------------------- 
MS_BOOL MDrv_HWI2C_SetReadMode(HWI2C_ReadMode eReadMode);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_HWI2C_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//------------------------------------------------------------------------------------------------- 
MS_BOOL MDrv_HWI2C_WriteByte(MS_U16 u16SlaveCfg, MS_U8 u8RegAddr, MS_U8 u8Data);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_HWI2C_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//------------------------------------------------------------------------------------------------- 
MS_BOOL MDrv_HWI2C_WriteBytes(MS_U16 u16SlaveCfg, MS_U32 uAddrCnt, MS_U8 *pRegAddr, MS_U32 uSize, MS_U8 *pData);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_HWI2C_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//------------------------------------------------------------------------------------------------- 
MS_BOOL MDrv_HWI2C_ReadByte(MS_U16 u16SlaveCfg, MS_U8 u8RegAddr, MS_U8 *pData);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_HWI2C_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//------------------------------------------------------------------------------------------------- 
MS_BOOL MDrv_HWI2C_ReadBytes(MS_U16 u16SlaveCfg, MS_U32 uAddrCnt, MS_U8 *pRegAddr, MS_U32 uSize, MS_U8 *pData);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_HWI2C_CONTROL
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//------------------------------------------------------------------------------------------------- 
MS_BOOL MDrv_HWI2C_SelectPort1(HWI2C_PORT ePort);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_HWI2C_CONTROL
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//------------------------------------------------------------------------------------------------- 
MS_BOOL MDrv_HWI2C_SetClkP1(HWI2C_CLKSEL eClk);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_HWI2C_CONTROL
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//------------------------------------------------------------------------------------------------- 
MS_BOOL MDrv_HWI2C_SetReadModeP1(HWI2C_ReadMode eReadMode);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_HWI2C_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//------------------------------------------------------------------------------------------------- 
MS_BOOL MDrv_HWI2C_WriteByteP1(MS_U16 u16SlaveCfg, MS_U8 u8RegAddr, MS_U8 u8Data);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_HWI2C_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//------------------------------------------------------------------------------------------------- 
MS_BOOL MDrv_HWI2C_WriteBytesP1(MS_U16 u16SlaveCfg, MS_U32 uAddrCnt, MS_U8 *pRegAddr, MS_U32 uSize, MS_U8 *pData);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_HWI2C_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//------------------------------------------------------------------------------------------------- 
MS_BOOL MDrv_HWI2C_ReadByteP1(MS_U16 u16SlaveCfg, MS_U8 u8RegAddr, MS_U8 *pData);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_HWI2C_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//------------------------------------------------------------------------------------------------- 
MS_BOOL MDrv_HWI2C_ReadBytesP1(MS_U16 u16SlaveCfg, MS_U32 uAddrCnt, MS_U8 *pRegAddr, MS_U32 uSize, MS_U8 *pData);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_HWI2C_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//------------------------------------------------------------------------------------------------- 
MS_BOOL MDrv_HWI2C_SetDbgLevel(HWI2C_DbgLv eLevel);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_HWI2C_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//------------------------------------------------------------------------------------------------- 
MS_BOOL MDrv_HWI2C_GetLibVer(const MSIF_Version **ppVersion);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_HWI2C_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//------------------------------------------------------------------------------------------------- 
void MDrv_HWI2C_GetStatus(HWI2C_Status *pStatus);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_HWI2C_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//------------------------------------------------------------------------------------------------- 
const HWI2C_Info* MDrv_HWI2C_GetInfo(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_HWI2C_CONTROL
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//------------------------------------------------------------------------------------------------- 
MS_BOOL  MDrv_HWI2C_ConfigDelay(MS_U32 u32StartDelay, MS_U32 u32StopDelay);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_HWI2C_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//------------------------------------------------------------------------------------------------- 
MS_U16 MDrv_HWI2C_SetPowerState(EN_POWER_MODE u16PowerState);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_HWI2C_CONTROL
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//------------------------------------------------------------------------------------------------- 
MS_BOOL  MDrv_HWI2C_ConfigDelay_EX(void *pParam_Config);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_HWI2C_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//------------------------------------------------------------------------------------------------- 
void MDrv_HWI2C_GetStatus_EX(void *pParam_Status);

// For Utopia2K
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_HWI2C_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//------------------------------------------------------------------------------------------------- 
MS_BOOL MDrv_HWI2C_Init_U2K(HWI2C_UnitCfg *psCfg);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_HWI2C_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//------------------------------------------------------------------------------------------------- 
MS_BOOL MDrv_HWI2C_Start_U2K(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_HWI2C_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//------------------------------------------------------------------------------------------------- 
MS_BOOL MDrv_HWI2C_Stop_U2K(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_HWI2C_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//------------------------------------------------------------------------------------------------- 
MS_BOOL MDrv_HWI2C_WriteBytes_U2K(MS_U16 u16SlaveCfg, MS_U32 uAddrCnt, MS_U8 *pRegAddr, MS_U32 uSize, MS_U8 *pData);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_HWI2C_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//------------------------------------------------------------------------------------------------- 
MS_BOOL MDrv_HWI2C_ReadBytes_U2K(MS_U16 u16SlaveCfg, MS_U32 uAddrCnt, MS_U8 *pRegAddr, MS_U32 uSize, MS_U8 *pData);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_HWI2C_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//------------------------------------------------------------------------------------------------- 
MS_BOOL MDrv_HWI2C_SelectPort_U2K(HWI2C_PORT ePort);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_HWI2C_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//------------------------------------------------------------------------------------------------- 
MS_BOOL MDrv_HWI2C_SetClk_U2K(HWI2C_CLKSEL eClk);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_HWI2C_CONTROL
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void MDrv_HWI2C_SetStopBit(MS_U8 u8Port, MS_BOOL bGenStop);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_HWI2C_CONTROL
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_HWI2C_GetStopBit(MS_U8 u8Port);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_HWI2C_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_HWI2C_WriteBytes_CUST(MS_U16 u16SlaveCfg, MS_U32 uAddrCnt, MS_U8 *pRegAddr, MS_U32 uSize, MS_U8 *pData, HWI2C_PORT ePort, HWI2C_CLKSEL eClk);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_HWI2C_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_HWI2C_ReadBytes_CUST(MS_U16 u16SlaveCfg, MS_U32 uAddrCnt, MS_U8 *pRegAddr, MS_U32 uSize, MS_U8 *pData, HWI2C_PORT ePort, HWI2C_CLKSEL eClk);

void  MDrv_HWI2C_EnableRetry(MS_BOOL bEnable);

#ifdef __cplusplus
}
#endif


#endif
