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
#ifndef _DRVMSPI_PRIV_H_
#define _DRVMSPI_PRIV_H_

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
#include "drvMSPI.h"


typedef MSPI_ErrorNo (*IOCTL_MSPI_Init_Ext)(MS_U8);
typedef MSPI_ErrorNo (*IOCTL_MSPI_Init)(MSPI_config*, MS_U8);
typedef MSPI_ErrorNo (*IOCTL_MSPI_Read)(MS_U8*, MS_U16);
typedef MSPI_ErrorNo (*IOCTL_MSPI_Write)(MS_U8*, MS_U16);
typedef MSPI_ErrorNo (*IOCTL_MSPI_DCConfig)(MSPI_DCConfig*);
typedef MSPI_ErrorNo (*IOCTL_MSPI_CLKConfig)(MSPI_CLKConfig*);
typedef MSPI_ErrorNo (*IOCTL_MSPI_FRAMEConfig)(MSPI_FrameConfig*);
typedef void (*IOCTL_MSPI_SlaveEnable)(MS_BOOL);
typedef MS_U32 (*IOCTL_MSPI_SetPowerState)(EN_POWER_MODE);
typedef MS_BOOL (*IOCTL_MSPI_SetDbgLevel)(MS_U8);
//==============================================================================
//   NEW API for MSPI
//===============================================================================
typedef MSPI_ErrorNo (*IOCTL_MasterSPI_Init)(MSPI_config*, MS_U8);
typedef MSPI_ErrorNo (*IOCTL_MasterSPI_Read)(MS_U32, MS_U8*, MS_U16);
typedef MSPI_ErrorNo (*IOCTL_MasterSPI_Write)(MS_U32, MS_U8*, MS_U16);
typedef MSPI_ErrorNo (*IOCTL_MasterSPI_DCConfig)(MS_U32, MSPI_DCConfig*);
typedef MSPI_ErrorNo (*IOCTL_MasterSPI_CLKConfig)(MS_U32, MSPI_CLKConfig*);
typedef MSPI_ErrorNo (*IOCTL_MasterSPI_FRAMEConfig)(MS_U32, MSPI_FrameConfig*);
typedef MSPI_ErrorNo (*IOCTL_MasterSPI_SlaveEnable)(MS_U32, MS_BOOL);
typedef MS_BOOL (*IOCTL_MasterSPI_CSPadConfig)(MS_U32, MS_U32);
typedef MS_BOOL (*IOCTL_MasterSPI_MaxClkConfig)(MS_U32, MS_U32);

typedef struct _MSPI_RESOURCE_PRIVATE
{
    MS_U32 Dummy;
}MSPI_RESOURCE_PRIVATE;

typedef struct _MSPI_INSTANT_PRIVATE
{
    IOCTL_MSPI_Init_Ext                                 fpMSPI_Init_Ext;
    IOCTL_MSPI_Init                                     fpMSPI_Init;
    IOCTL_MSPI_Read                                     fpMSPI_Read;
    IOCTL_MSPI_Write                                    fpMSPI_Write;
    IOCTL_MSPI_DCConfig                                 fpMSPI_DCConfig;
    IOCTL_MSPI_CLKConfig                                fpMSPI_CLKConfig;
    IOCTL_MSPI_FRAMEConfig                              fpMSPI_FRAMEConfig;
    IOCTL_MSPI_SlaveEnable                              fpMSPI_SlaveEnable;
    IOCTL_MSPI_SetPowerState                            fpMSPI_SetPowerState;
    IOCTL_MSPI_SetDbgLevel                              fpMSPI_SetDbgLevel;
    IOCTL_MasterSPI_Init                                fpMasterSPI_Init;
    IOCTL_MasterSPI_Read                                fpMasterSPI_Read;
    IOCTL_MasterSPI_Write                               fpMasterSPI_Write;
    IOCTL_MasterSPI_DCConfig                            fpMasterSPI_DCConfig;
    IOCTL_MasterSPI_CLKConfig                           fpMasterSPI_CLKConfig;
    IOCTL_MasterSPI_FRAMEConfig                         fpMasterSPI_FrameConfig;
    IOCTL_MasterSPI_SlaveEnable                         fpMasterSPI_SlaveEnable;
    IOCTL_MasterSPI_CSPadConfig                         fpMasterSPI_CsPadConfig;
    IOCTL_MasterSPI_MaxClkConfig                        fpMasterSPI_MaxClkConfig;
}MSPI_INSTANT_PRIVATE;

void MSPIRegisterToUtopia(FUtopiaOpen ModuleType);
MS_U32 MSPIOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute);
MS_U32 MSPIClose(void* pInstance);
MS_U32 MSPIIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs);
#if defined(CONFIG_UTOPIA_PROC_DBG_SUPPORT)
#define MSPI_CMD_STRING_LENGTH    20
#define MSPI_CMD_HELP_STRING "help"
#define MSPI_CMD_SET_CLK        "SetClk"
#define MSPI_CMD_LOGLEVEL_STRING "LogLevel"
#define MSPI_CMD_LOOPBACK        "MspiLoopback"
#define SCAN_PAD_MAX 0x5
#define TEST_PATTEN  0xAA
#define NOT_MSPIPAD  0xFF
typedef enum
{
    MSPI_CMD_TYPE_HELP,
    MSPI_CMD_TYPE_SET_CLK,
    MSPI_CMD_TYPE_LOGLEVEL,
    MSPI_CMD_TYPE_LOOPBACK,
    MSPI_CMD_TYPE_MAX
} MSPI_CMD_TYPE_DEF;

typedef struct
{
    MSPI_CMD_TYPE_DEF u8CmdType;
    MS_U32 u32SettingValue[3];
} MSPI_CMD_INFO;

void MDrv_MSPI_Atoi(char *s, MS_U32 *u32MspiCmd);
void MDrv_MSPI_CmdParsing(MS_U64 *u64Hdl, char *s, MSPI_CMD_INFO *pstCmdInfo);
void MDrv_MSPI_CmdExecute(MS_U64 *u64Hdl, MSPI_CMD_INFO *pstCmdInfo);

void MDrv_MSPI_Debug_Cmd(MS_U64 *u64Hdl, char *cmd);
void MDrv_MSPI_Debug_Cat(MS_U64 *u64Hdl);
#endif
#if (MSPI_UTOPIA20)
//------------------------------------------------------------------------------
/// Description : Set detailed level of Parallel Flash driver debug message
/// @param u8DbgLevel    \b IN  debug level for Serial Flash driver
/// @return TRUE : succeed
/// @return FALSE : failed to set the debug level
//------------------------------------------------------------------------------
MS_BOOL _MDrv_MSPI_SetDbgLevel(MS_U8 u8DbgLevel);

//------------------------------------------------------------------------------
/// Description : MSPI Init
/// @param u8HwNum
/// @return TRUE : succeed
/// @return FALSE : failed to set the WP pin info
//------------------------------------------------------------------------------
MSPI_ErrorNo  _MDrv_MSPI_Init_Ext(MS_U8 u8HWNum);

//------------------------------------------------------------------------------
/// Description : MSPI read data
/// @param pData
/// @param u16Size
/// @return TRUE : succeed
/// @return NULL :
//------------------------------------------------------------------------------
MSPI_ErrorNo _MDrv_MSPI_Read(MS_U8 *pData, MS_U16 u16Size);

//-------------------------------------------------------------------------------------------------
/// Description : MSPI write data
/// @param pData
/// @param u16Size
/// @return TRUE : succeed
/// @return FALSE : unknown flash type
/// @note   Not allowed in interrupt context
//-------------------------------------------------------------------------------------------------
MSPI_ErrorNo _MDrv_MSPI_Write(MS_U8 *pData, MS_U16 u16Size);

//-------------------------------------------------------------------------------------------------
/// Description : MSPI timing config
/// @param  ptDCConfig \b IN: config MSPI timing
/// @return TRUE : succeed
/// @return FALSE : unknown flash size
/// @note   Not allowed in interrupt context
//-------------------------------------------------------------------------------------------------
MSPI_ErrorNo _MDrv_MSPI_DCConfig(MSPI_DCConfig *ptDCConfig);

//-------------------------------------------------------------------------------------------------
/// Description : MSPI clock config
/// @param  ptCLKConfig    \b IN: clock config
/// @return TRUE : succeed
/// @return FALSE : not succeed
/// @note   Please ref. sprc. to confirm Flash support or not
//-------------------------------------------------------------------------------------------------
MSPI_ErrorNo _MDrv_MSPI_CLKConfig(MSPI_CLKConfig *ptCLKConfig);

//-------------------------------------------------------------------------------------------------
/// Description : Config MSPI frame size
/// @param  ptFrameConfig
/// @return TRUE : succeed
/// @return FALSE : not succeed
/// @note   Please ref. sprc. to confirm Flash support or not. It is safty to run at 43M (Default).
//-------------------------------------------------------------------------------------------------
MSPI_ErrorNo _MDrv_MSPI_FRAMEConfig(MSPI_FrameConfig *ptFrameConfig);
//-------------------------------------------------------------------------------------------------
/// Description : set CS of MSPI
/// @param  bEnable  \b IN: set CS HIGH/LOW
/// @return TRUE : succeed
/// @return FALSE : not succeed
/// @note
//-------------------------------------------------------------------------------------------------
void _MDrv_MSPI_SlaveEnable(MS_BOOL bEnable);

//-------------------------------------------------------------------------------------------------
/// Description : Set MSPI power state
/// @param  enPowerState \b IN: power state of MSPI
/// @return TRUE : succeed
/// @return FALSE : not succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL _MDrv_MSPI_SetPowerState(EN_POWER_MODE enPowerState);

//-------------------------------------------------------------------------------------------------
/// Description : Set MSPI debug message level
/// @param  u8DbgLevel /b IN: debug message level
/// @return TRUE : succeed
/// @return FALSE : illegal parameters
/// @note   Not allowed in interrupt context
//-------------------------------------------------------------------------------------------------
MS_BOOL _MDrv_MSPI_SetDbgLevel(MS_U8 u8DbgLevevl);

//-------------------------------------------------------------------------------------------------
/// Description : MSPI Init
/// @param  ptMSPIConfig \b IN: slave device config of mspi
/// @param  u8HWNum      \b IN: mspi ip number
/// @return error numbero of driver E_MSPI_OK : success \others fail
//-------------------------------------------------------------------------------------------------
MSPI_ErrorNo _MDrv_MasterSPI_Init(MSPI_config *ptMSPIConfig, MS_U8 u8HWNum);

//-------------------------------------------------------------------------------------------------
/// Description : MSPI READ DATA
/// @param  u32DevID    \b IN: device id of slave device
/// @param  *pu8Data      \b IN: pointer of buffer to get data
/// @param  u16Size     \b IN: size of read data
/// @return E_MSPI_OK : succeed
/// @return others: fail
/// @note   Not allowed in interrupt context
//-------------------------------------------------------------------------------------------------
MSPI_ErrorNo _MDrv_MasterSPI_Read(MS_U32 u32DevID, MS_U8 *pu8Data, MS_U16 u16Size);

//-------------------------------------------------------------------------------------------------
/// Description : MSPI write data
/// @param  u32DevID    \b IN: device id of slave device
/// @param  *pu8Data    \b IN: pointer of buffer to send data
/// @param  u16Size     \b IN: size of write data
/// @return E_MSPI_OK : succeed
/// @return others : fail
/// @note   Not allowed in interrupt context
//-------------------------------------------------------------------------------------------------
MSPI_ErrorNo _MDrv_MasterSPI_Write(MS_U32 u32DevID, MS_U8 *pu8Data, MS_U16 u16Size);

//-------------------------------------------------------------------------------------------------
/// Description : Set MSPI timming
/// @param  u32DevID      \b IN: device id of slave device
/// @param  ptDCConfig    \b IN: poniter of struct config slave timming
/// @return E_MSPI_OK : succeed
/// @return others: fail
/// @note   Not allowed in interrupt context
//-------------------------------------------------------------------------------------------------
MSPI_ErrorNo _MDrv_MasterSPI_DCConfig(MS_U32 u32DevID, MSPI_DCConfig *ptDCConfig);

//-------------------------------------------------------------------------------------------------
/// Description : Set MSPI clock
/// @param u32DevID    \b IN: device id of slave device
/// @param ptCLKConfig \b IN: pointer of struct to config mspi clock
/// @return E_MSPI_OK : success
/// @return others : fail
/// @note   Not allowed in interrupt context
//-------------------------------------------------------------------------------------------------
MSPI_ErrorNo _MDrv_MasterSPI_CLKConfig(MS_U32 u32DevID, MSPI_CLKConfig *ptCLKConfig);

//-------------------------------------------------------------------------------------------------
/// Description : Set MSPI fram size
/// @param  u32DevID         \b IN: device id of slave device
/// @param  ptFrameConfig    \b IN: pointer of frame size
/// @return E_MSPI_OK : succeed
/// @return others: fail before timeout or illegal parameters
/// @note   Not allowed in interrupt context
/// [NONOS_SUPPORT]
//-------------------------------------------------------------------------------------------------
MSPI_ErrorNo _MDrv_MasterSPI_FRAMEConfig(MS_U32 u32DevID, MSPI_FrameConfig *ptFrameConfig);

//-------------------------------------------------------------------------------------------------
/// Description : MSPI slave enable/disable
/// @param  u32DevID    \b IN: device id of slave id
/// @param  bEnable     \b IN: set CS high /low to enable /disable slave device
/// @return NONE
/// @note   Not allowed in interrupt context
/// [NONOS_SUPPORT]
//-------------------------------------------------------------------------------------------------
MSPI_ErrorNo _MDrv_MasterSPI_SlaveEnable(MS_U32 u32DevID, MS_BOOL bEnable);

//-------------------------------------------------------------------------------------------------
/// Description : Set MSPI CS PAD number
/// @param u32DevID \b IN: slave device id of slave device
/// @param u32CsPad \b IN: pad number of cs pin
/// @return TRUE : succeed
/// @return FALSE : fail
/// @note   Not allowed in interrupt context
//-------------------------------------------------------------------------------------------------
MS_BOOL _MDrv_MasterSPI_CsPadConfig(MS_U32 u32DevID, MS_U32 u32CsPad);

//-------------------------------------------------------------------------------------------------
/// Description : Set MSPI Slave Device Support Max Clock
/// @param u32DevID  \b IN: slave device id of slave device
/// @param u32MaxCLk \b IN: max clock of slave device support
/// @return TRUE : succeed
/// @return FALSE : fail
/// @note   Not allowed in interrupt contextb//-------------------------------------------------------------------------------------------------
MS_BOOL _MDrv_MasterSPI_MaxClkConfig(MS_U32 u32DevID, MS_U32 u32MaxClk);
#endif

#ifdef __cplusplus
}
#endif
#endif // _DRVBDMA_PRIV_H_
