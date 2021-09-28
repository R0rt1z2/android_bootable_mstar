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
/// @brief  DMD ISDBT Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_ISDBT_V2_H_
#define _DRV_ISDBT_V2_H_

#include "drvDMD_ISDBT.h"

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

#ifndef DLL_PUBLIC 
#define DLL_PUBLIC 
#endif

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

typedef enum {
    DMD_ISDBT_DRV_CMD_SetDbgLevel,
    DMD_ISDBT_DRV_CMD_GetInfo,
    DMD_ISDBT_DRV_CMD_GetLibVer,
    DMD_ISDBT_DRV_CMD_Init,
    DMD_ISDBT_DRV_CMD_Exit,
    DMD_ISDBT_DRV_CMD_GetConfig,
    DMD_ISDBT_DRV_CMD_AdvSetConfig,
    DMD_ISDBT_DRV_CMD_SetPowerState,
    DMD_ISDBT_DRV_CMD_GetLock,
    DMD_ISDBT_DRV_CMD_GetModulationMode,
    DMD_ISDBT_DRV_CMD_GetSignalStrength,
    DMD_ISDBT_DRV_CMD_GetFreqOffset,
    DMD_ISDBT_DRV_CMD_GetSignalQuality,
    DMD_ISDBT_DRV_CMD_GetSignalQualityOfLayerA,
    DMD_ISDBT_DRV_CMD_GetSignalQualityOfLayerB,
    DMD_ISDBT_DRV_CMD_GetSignalQualityOfLayerC,
    DMD_ISDBT_DRV_CMD_GetSignalQualityCombine,
    DMD_ISDBT_DRV_CMD_GetSNR,
    DMD_ISDBT_DRV_CMD_GetPreViterbiBer,
    DMD_ISDBT_DRV_CMD_GetPostViterbiBer,
    DMD_ISDBT_DRV_CMD_Read_PKT_ERR,
    DMD_ISDBT_DRV_CMD_SetSerialControl,
    DMD_ISDBT_DRV_CMD_IIC_BYPASS_MODE,
    DMD_ISDBT_DRV_CMD_SWITCH_SSPI_GPIO,
    DMD_ISDBT_DRV_CMD_GPIO_GET_LEVEL,
    DMD_ISDBT_DRV_CMD_GPIO_SET_LEVEL,
    DMD_ISDBT_DRV_CMD_GPIO_OUT_ENABLE,
    DMD_ISDBT_DRV_CMD_DoIQSwap,
    DMD_ISDBT_DRV_CMD_GetReg,
    DMD_ISDBT_DRV_CMD_SetReg,
    DMD_ISDBT_DRV_CMD_MD_Init,
    DMD_ISDBT_DRV_CMD_MD_Exit,
    DMD_ISDBT_DRV_CMD_MD_GetConfig,   
    DMD_ISDBT_DRV_CMD_MD_AdvSetConfig,
    DMD_ISDBT_DRV_CMD_MD_SetActive,
    DMD_ISDBT_DRV_CMD_MD_SetPowerState,
    DMD_ISDBT_DRV_CMD_MD_GetLock,
    DMD_ISDBT_DRV_CMD_MD_GetModulationMode,
    DMD_ISDBT_DRV_CMD_MD_GetSignalStrength,
    DMD_ISDBT_DRV_CMD_MD_GetFreqOffset,
    DMD_ISDBT_DRV_CMD_MD_GetSignalQuality,
    DMD_ISDBT_DRV_CMD_MD_GetSignalQualityOfLayerA,
    DMD_ISDBT_DRV_CMD_MD_GetSignalQualityOfLayerB,
    DMD_ISDBT_DRV_CMD_MD_GetSignalQualityOfLayerC,
    DMD_ISDBT_DRV_CMD_MD_GetSignalQualityCombine,
    DMD_ISDBT_DRV_CMD_MD_GetSNR,
    DMD_ISDBT_DRV_CMD_MD_GetPreViterbiBer,
    DMD_ISDBT_DRV_CMD_MD_GetPostViterbiBer,
    DMD_ISDBT_DRV_CMD_MD_Read_PKT_ERR,
    DMD_ISDBT_DRV_CMD_MD_SetSerialControl,
    DMD_ISDBT_DRV_CMD_MD_IIC_BYPASS_MODE,
    DMD_ISDBT_DRV_CMD_MD_SWITCH_SSPI_GPIO,
    DMD_ISDBT_DRV_CMD_MD_GPIO_GET_LEVEL,
    DMD_ISDBT_DRV_CMD_MD_GPIO_SET_LEVEL,
    DMD_ISDBT_DRV_CMD_MD_GPIO_OUT_ENABLE,
    DMD_ISDBT_DRV_CMD_MD_DoIQSwap,
    DMD_ISDBT_DRV_CMD_MD_GetReg,
    DMD_ISDBT_DRV_CMD_MD_SetReg
} DMD_ISDBT_DRV_COMMAND;

typedef struct DLL_PACKED _ISDBT_DBG_LEVEL_PARAM
{
    DMD_ISDBT_DbgLv u8DbgLevel;
} ISDBT_DBG_LEVEL_PARAM, *PISDBT_DBG_LEVEL_PARAM;

typedef struct DLL_PACKED _ISDBT_GET_INFO_PARAM
{
    DMD_ISDBT_Info* pInfo;
} ISDBT_GET_INFO_PARAM, *PISDBT_GET_INFO_PARAM;

typedef struct DLL_PACKED _ISDBT_GET_LIB_VER_PARAM
{
    const MSIF_Version **ppVersion;
} ISDBT_GET_LIB_VER_PARAM, *PISDBT_GET_LIB_VER_PARAM;

typedef struct DLL_PACKED _ISDBT_INIT_PARAM
{
    MS_U8 id;
    DMD_ISDBT_InitData *pDMD_ISDBT_InitData;
    MS_U32             u32InitDataLen;
} ISDBT_INIT_PARAM, *PISDBT_INIT_PARAM;

typedef struct DLL_PACKED _ISDBT_SET_CONFIG_PARAM
{
    MS_U8 id;
    DMD_ISDBT_DEMOD_TYPE eType;
    MS_BOOL             bEnable;
} ISDBT_SET_CONFIG_PARAM, *PISDBT_SET_CONFIG_PARAM;

typedef struct DLL_PACKED _ISDBT_ID_PARAM
{
    MS_U8 id;
} ISDBT_ID_PARAM, *PISDBT_ID_PARAM;

typedef struct DLL_PACKED _ISDBT_SET_POWER_STATE_PARAM
{
    MS_U8 id;
    EN_POWER_MODE u16PowerState;
} ISDBT_SET_POWER_STATE_PARAM, *PISDBT_SET_POWER_STATE_PARAM;

typedef struct DLL_PACKED _ISDBT_GET_LOCK_PARAM
{
    MS_U8 id;
    DMD_ISDBT_GETLOCK_TYPE eType;
    DMD_ISDBT_LOCK_STATUS  status;
} ISDBT_GET_LOCK_PARAM, *PISDBT_GET_LOCK_PARAM;

typedef struct DLL_PACKED _ISDBT_GET_MODULATION_MODE_PARAM
{
    MS_U8 id;
    EN_ISDBT_Layer eLayerIndex;
    sISDBT_MODULATION_MODE IsdbtModulationMode;
} ISDBT_GET_MODULATION_MODE_PARAM, *PISDBT_GET_MODULATION_MODE_PARAM;

typedef struct DLL_PACKED _ISDBT_GET_SIGNAL_STRENGTH_PARAM
{
    MS_U8 id;
    MS_U16 u16Strength;
} ISDBT_GET_SIGNAL_STRENGTH_PARAM, *PISDBT_GET_SIGNAL_STRENGTH_PARAM;

typedef struct DLL_PACKED _ISDBT_GET_FREQ_OFFSET_PARAM
{
    MS_U8 id;
    #ifdef UTPA2
    DMD_ISDBT_CFO_DATA cfo;
    #else
    MS_S16 cfo;
    #endif
} ISDBT_GET_FREQ_OFFSET_PARAM, *PISDBT_GET_FREQ_OFFSET_PARAM;

typedef struct DLL_PACKED _ISDBT_GET_SIGNAL_QUALITY_PARAM
{
    MS_U8 id;
    MS_U16 u16SignalQuality;
} ISDBT_GET_SIGNAL_QUALITY_PARAM, *PISDBT_GET_SIGNAL_QUALITY_PARAM;

typedef struct DLL_PACKED _ISDBT_GET_SNR_PARAM
{
    MS_U8 id;
    #ifdef UTPA2
    DMD_ISDBT_SNR_DATA snr;
    #else
    float snr;
    #endif
} ISDBT_GET_SNR_PARAM, *PISDBT_GET_SNR_PARAM;

typedef struct DLL_PACKED _ISDBT_READ_PKT_ERR_PARAM
{
    MS_U8 id;
    EN_ISDBT_Layer eIsdbtLayer;
    MS_U16 u16PacketErr;
} ISDBT_READ_PKT_ERR_PARAM, *PISDBT_READ_PKT_ERR_PARAM;

typedef struct DLL_PACKED _ISDBT_GET_BER_PARAM
{
    MS_U8 id;
    DMD_ISDBT_GET_BER_VALUE ber;
} ISDBT_GET_BER_PARAM, *PISDBT_GET_BER_PARAM;

typedef struct DLL_PACKED _ISDBT_SET_SERIAL_CONTROL_PARAM
{
    MS_U8 id;
    MS_U8 u8TsConfigData;
} ISDBT_SET_SERIAL_CONTROL_PARAM, *PISDBT_SET_SERIAL_CONTROL_PARAM;

typedef struct DLL_PACKED _ISDBT_IIC_BYPASS_MODE_PARAM
{
    MS_U8 id;
    MS_BOOL bEnable;
} ISDBT_IIC_BYPASS_MODE_PARAM, *PISDBT_IIC_BYPASS_MODE_PARAM;

typedef struct DLL_PACKED _ISDBT_SWITCH_SSPI_GPIO_PARAM
{
    MS_U8 id;
    MS_BOOL bEnable;
} ISDBT_SWITCH_SSPI_GPIO_PARAM, *PISDBT_SWITCH_SSPI_GPIO_PARAM;

typedef struct DLL_PACKED _ISDBT_GPIO_LEVEL_PARAM
{
    MS_U8 id;
    MS_U8 u8Pin;
    MS_BOOL bLevel;
} ISDBT_GPIO_LEVEL_PARAM, *PISDBT_GPIO_LEVEL_PARAM;

typedef struct DLL_PACKED _ISDBT_GPIO_OUT_ENABLE_PARAM
{
    MS_U8 id;
    MS_U8 u8Pin;
    MS_BOOL bEnableOut;
} ISDBT_GPIO_OUT_ENABLE_PARAM, *PISDBT_GPIO_OUT_ENABLE_PARAM;

typedef struct DLL_PACKED _ISDBT_DO_IQ_SWAP_PARAM
{
    MS_U8 id;
    MS_BOOL bIsQPad;
} ISDBT_DO_IQ_SWAP_PARAM, *PISDBT_DO_IQ_SWAP_PARAM;

typedef struct DLL_PACKED _ISDBT_REG_PARAM
{
    MS_U8 id;
    MS_U16 u16Addr;
    MS_U8 u8Data;
} ISDBT_REG_PARAM, *PISDBT_REG_PARAM;


//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

DLL_PUBLIC extern void ISDBTRegisterToUtopia(FUtopiaOpen ModuleType);
DLL_PUBLIC extern MS_U32 ISDBTOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute);
DLL_PUBLIC extern MS_U32 ISDBTClose(void* pInstance);
DLL_PUBLIC extern MS_U32 ISDBTIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs);

#ifdef __cplusplus
}
#endif

#endif // _DRV_ISDBT_V2_H_

