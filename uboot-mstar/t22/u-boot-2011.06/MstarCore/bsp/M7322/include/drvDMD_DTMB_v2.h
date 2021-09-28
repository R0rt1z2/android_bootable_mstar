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
/// @brief  DMD DTMB Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_DTMB_V2_H_
#define _DRV_DTMB_V2_H_

#include "drvDMD_DTMB.h"

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
    DMD_DTMB_DRV_CMD_SetDbgLevel,
    DMD_DTMB_DRV_CMD_GetInfo,
    DMD_DTMB_DRV_CMD_GetLibVer,
    DMD_DTMB_DRV_CMD_Init,
    DMD_DTMB_DRV_CMD_Exit,
    DMD_DTMB_DRV_CMD_GetConfig,
    DMD_DTMB_DRV_CMD_SetConfig,
    DMD_DTMB_DRV_CMD_SetReset,
    DMD_DTMB_DRV_CMD_Set_QAM_SR,
    DMD_DTMB_DRV_CMD_SetActive,
    DMD_DTMB_DRV_CMD_SetPowerState,
    DMD_DTMB_DRV_CMD_GetLock,
    DMD_DTMB_DRV_CMD_GetModulationMode,
    DMD_DTMB_DRV_CMD_GetSignalStrength,
    DMD_DTMB_DRV_CMD_ReadFrequencyOffset,
    DMD_DTMB_DRV_CMD_GetSignalQuality,
    DMD_DTMB_DRV_CMD_GetPreLdpcBer,
    DMD_DTMB_DRV_CMD_GetPreViterbiBer,
    DMD_DTMB_DRV_CMD_GetPostViterbiBer,
    DMD_DTMB_DRV_CMD_GetSNR,
    DMD_DTMB_DRV_CMD_SetSerialControl,
    DMD_DTMB_DRV_CMD_IIC_BYPASS_MODE,
    DMD_DTMB_DRV_CMD_SWITCH_SSPI_GPIO,
    DMD_DTMB_DRV_CMD_GPIO_GET_LEVEL,
    DMD_DTMB_DRV_CMD_GPIO_SET_LEVEL,
    DMD_DTMB_DRV_CMD_GPIO_OUT_ENABLE,
    DMD_DTMB_DRV_CMD_DoIQSwap,
    DMD_DTMB_DRV_CMD_GetReg,
    DMD_DTMB_DRV_CMD_SetReg,
    DMD_DTMB_DRV_CMD_MD_Init,
    DMD_DTMB_DRV_CMD_MD_Exit,
    DMD_DTMB_DRV_CMD_MD_GetConfig,
    DMD_DTMB_DRV_CMD_MD_SetConfig,
    DMD_DTMB_DRV_CMD_MD_SetReset,
    DMD_DTMB_DRV_CMD_MD_Set_QAM_SR,
    DMD_DTMB_DRV_CMD_MD_SetActive,
    DMD_DTMB_DRV_CMD_MD_SetPowerState,
    DMD_DTMB_DRV_CMD_MD_GetLock,
    DMD_DTMB_DRV_CMD_MD_GetModulationMode,
    DMD_DTMB_DRV_CMD_MD_GetSignalStrength,
    DMD_DTMB_DRV_CMD_MD_ReadFrequencyOffset,
    DMD_DTMB_DRV_CMD_MD_GetSignalQuality,
    DMD_DTMB_DRV_CMD_MD_GetPreLdpcBer,
    DMD_DTMB_DRV_CMD_MD_GetPreViterbiBer,
    DMD_DTMB_DRV_CMD_MD_GetPostViterbiBer,
    DMD_DTMB_DRV_CMD_MD_GetSNR,
    DMD_DTMB_DRV_CMD_MD_SetSerialControl,
    DMD_DTMB_DRV_CMD_MD_IIC_BYPASS_MODE,
    DMD_DTMB_DRV_CMD_MD_SWITCH_SSPI_GPIO,
    DMD_DTMB_DRV_CMD_MD_GPIO_GET_LEVEL,
    DMD_DTMB_DRV_CMD_MD_GPIO_SET_LEVEL,
    DMD_DTMB_DRV_CMD_MD_GPIO_OUT_ENABLE,
    DMD_DTMB_DRV_CMD_MD_DoIQSwap,
    #ifdef CONFIG_UTOPIA_PROC_DBG_SUPPORT
    DMD_DTMB_DRV_CMD_MD_DBG_GetModuleInfo,
    DMD_DTMB_DRV_CMD_MD_DBG_EchoCmd,
    #endif
    DMD_DTMB_DRV_CMD_MD_GetReg,
    DMD_DTMB_DRV_CMD_MD_SetReg
} DMD_DTMB_DRV_COMMAND;

typedef struct DLL_PACKED _DTMB_DBG_LEVEL_PARAM
{
    DMD_DTMB_DbgLv u8DbgLevel;
} DTMB_DBG_LEVEL_PARAM, *PDTMB_DBG_LEVEL_PARAM;

typedef struct DLL_PACKED _DTMB_GET_INFO_PARAM
{
    DMD_DTMB_Info* pInfo;
} DTMB_GET_INFO_PARAM, *PDTMB_GET_INFO_PARAM;

typedef struct DLL_PACKED _DTMB_GET_LIB_VER_PARAM
{
    const MSIF_Version **ppVersion;
} DTMB_GET_LIB_VER_PARAM, *PDTMB_GET_LIB_VER_PARAM;

typedef struct DLL_PACKED _DTMB_INIT_PARAM
{
    MS_U8 id;
    DMD_DTMB_InitData *pDMD_DTMB_InitData;
    MS_U32             u32InitDataLen;
} DTMB_INIT_PARAM, *PDTMB_INIT_PARAM;

typedef struct DLL_PACKED _DTMB_ID_PARAM
{
    MS_U8 id;
} DTMB_ID_PARAM, *PDTMB_ID_PARAM;

typedef struct DLL_PACKED _DTMB_SET_CONFIG_PARAM
{
    MS_U8 id;
    DMD_DTMB_DEMOD_TYPE eType;
    MS_BOOL             bEnable;
} DTMB_SET_CONFIG_PARAM, *PDTMB_SET_CONFIG_PARAM;

typedef struct DLL_PACKED _DTMB_SET_QAM_SR_PARAM
{
    MS_U8 id;
    DMD_DTMB_DEMOD_TYPE eType;
    MS_U16              symbol_rate;
} DTMB_SET_QAM_SR_PARAM, *PDTMB_SET_QAM_SR_PARAM;

typedef struct DLL_PACKED _DTMB_SET_ACTIVE_PARAM
{
    MS_U8 id;
    MS_BOOL bEnable;
} DTMB_SET_ACTIVE_PARAM, *PDTMB_SET_ACTIVE_PARAM;

typedef struct DLL_PACKED _DTMB_SET_POWER_STATE_PARAM
{
    MS_U8 id;
    EN_POWER_MODE u16PowerState;
} DTMB_SET_POWER_STATE_PARAM, *PDTMB_SET_POWER_STATE_PARAM;

typedef struct DLL_PACKED _DTMB_GET_LOCK_PARAM
{
    MS_U8 id;
    DMD_DTMB_GETLOCK_TYPE eType;
    DMD_DTMB_LOCK_STATUS  status;
} DTMB_GET_LOCK_PARAM, *PDTMB_GET_LOCK_PARAM;

typedef struct DLL_PACKED _DTMB_GET_MODULATION_MODE_PARAM
{
    MS_U8 id;
    DMD_DTMB_MODULATION_INFO info;
} DTMB_GET_MODULATION_MODE_PARAM, *PDTMB_GET_MODULATION_MODE_PARAM;

typedef struct DLL_PACKED _DTMB_GET_SIGNAL_STRENGTH_PARAM
{
    MS_U8 id;
    MS_U16 u16Strength;
} DTMB_GET_SIGNAL_STRENGTH_PARAM, *PDTMB_GET_SIGNAL_STRENGTH_PARAM;

typedef struct DLL_PACKED _DTMB_READ_FREQ_OFFSET_PARAM
{
    MS_U8 id;
    #ifdef UTPA2
    DMD_DTMB_CFO_DATA cfo;
    #else
    MS_S16 cfo;
    #endif
} DTMB_READ_FREQ_OFFSET_PARAM, *PDTMB_READ_FREQ_OFFSET_PARAM;

typedef struct DLL_PACKED _DTMB_GET_SIGNAL_QUALITY_PARAM
{
    MS_U8 id;
    MS_U8 u8Percentage;
} DTMB_GET_SIGNAL_QUALITY_PARAM, *PDTMB_GET_SIGNAL_QUALITY_PARAM;

typedef struct DLL_PACKED _DTMB_GET_BER_PARAM
{
    MS_U8 id;
    #ifdef UTPA2
    DMD_DTMB_BER_DATA ber;
    #else
    float ber;
    #endif
} DTMB_GET_BER_PARAM, *PDTMB_GET_BER_PARAM;

typedef struct DLL_PACKED _DTMB_GET_SNR_PARAM
{
    MS_U8 id;
    #ifdef UTPA2
    DMD_DTMB_SNR_DATA snr;
    #else
    float snr;
    #endif
} DTMB_GET_SNR_PARAM, *PDTMB_GET_SNR_PARAM;

typedef struct DLL_PACKED _DTMB_SET_SERIAL_CONTROL_PARAM
{
    MS_U8 id;
    MS_U8 u8TsConfigData;
} DTMB_SET_SERIAL_CONTROL_PARAM, *PDTMB_SET_SERIAL_CONTROL_PARAM;

typedef struct DLL_PACKED _DTMB_IIC_BYPASS_MODE_PARAM
{
    MS_U8 id;
    MS_BOOL bEnable;
} DTMB_IIC_BYPASS_MODE_PARAM, *PDTMB_IIC_BYPASS_MODE_PARAM;

typedef struct DLL_PACKED _DTMB_SWITCH_SSPI_GPIO_PARAM
{
    MS_U8 id;
    MS_BOOL bEnable;
} DTMB_SWITCH_SSPI_GPIO_PARAM, *PDTMB_SWITCH_SSPI_GPIO_PARAM;

typedef struct DLL_PACKED _DTMB_GPIO_LEVEL_PARAM
{
    MS_U8 id;
    MS_U8 u8Pin;
    MS_BOOL bLevel;
} DTMB_GPIO_LEVEL_PARAM, *PDTMB_GPIO_LEVEL_PARAM;

typedef struct DLL_PACKED _DTMB_DO_IQ_SWAP_PARAM
{
    MS_U8 id;
    MS_BOOL bIsQPad;
} DTMB_DO_IQ_SWAP_PARAM, *PDTMB_DO_IQ_SWAP_PARAM;

typedef struct DLL_PACKED _DTMB_GPIO_OUT_ENABLE_PARAM
{
    MS_U8 id;
    MS_U8 u8Pin;
    MS_BOOL bEnableOut;
} DTMB_GPIO_OUT_ENABLE_PARAM, *PDTMB_GPIO_OUT_ENABLE_PARAM;

typedef struct DLL_PACKED _DTMB_DO_IQSWAP_PARAM
{
    MS_U8 id;
    MS_BOOL bIsQPad;
} DTMB_DO_IQSWAP_PARAM, *PDTMB_DO_IQSWAP_PARAM;

typedef struct DLL_PACKED _DTMB_REG_PARAM
{
    MS_U8 id;
    MS_U16 u16Addr;
    MS_U8 u8Data;
} DTMB_REG_PARAM, *PDTMB_REG_PARAM;

#ifdef CONFIG_UTOPIA_PROC_DBG_SUPPORT
typedef struct DLL_PACKED _DTMB_MDB_PARAM
{
    MS_U8 id;
    MS_U32 u32Data;
    MS_U64* u64Data;
    MS_S8* chData;
} DTMB_MDB_PARAM, *PDTMB_MDB_PARAM;
#endif

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

DLL_PUBLIC extern void DTMBRegisterToUtopia(FUtopiaOpen ModuleType);
DLL_PUBLIC extern MS_U32 DTMBOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute);
DLL_PUBLIC extern MS_U32 DTMBClose(void* pInstance);
DLL_PUBLIC extern MS_U32 DTMBIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs);
#ifdef CONFIG_UTOPIA_PROC_DBG_SUPPORT
DLL_PUBLIC extern MS_U32  DTMBMdbIoctl(MS_U32 u32Cmd, const void* const pArgs);
#endif

#ifdef __cplusplus
}
#endif

#endif // _DRV_ATSC_V2_H_

