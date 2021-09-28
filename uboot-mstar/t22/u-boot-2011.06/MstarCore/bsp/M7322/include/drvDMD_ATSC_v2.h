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
/// @brief  DMD ATSC Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_ATSC_V2_H_
#define _DRV_ATSC_V2_H_

#include "drvDMD_ATSC.h"

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
    DMD_ATSC_DRV_CMD_SetDbgLevel,
    DMD_ATSC_DRV_CMD_GetInfo,
    DMD_ATSC_DRV_CMD_GetLibVer,
    DMD_ATSC_DRV_CMD_Init,
    DMD_ATSC_DRV_CMD_Exit,
    DMD_ATSC_DRV_CMD_GetConfig,   
    DMD_ATSC_DRV_CMD_SetConfig,
    DMD_ATSC_DRV_CMD_SetReset,
    DMD_ATSC_DRV_CMD_Set_QAM_SR,
    DMD_ATSC_DRV_CMD_SetActive,
    DMD_ATSC_DRV_CMD_SetPowerState,
    DMD_ATSC_DRV_CMD_GetLock,
    DMD_ATSC_DRV_CMD_GetModulationMode,
    DMD_ATSC_DRV_CMD_GetSignalStrength,
    DMD_ATSC_DRV_CMD_GetSignalQuality,
    DMD_ATSC_DRV_CMD_GetSNRPercentage,
    DMD_ATSC_DRV_CMD_GET_QAM_SNR,
    DMD_ATSC_DRV_CMD_Read_uCPKT_ERR,
    DMD_ATSC_DRV_CMD_GetPreViterbiBer,
    DMD_ATSC_DRV_CMD_GetPostViterbiBer,
    DMD_ATSC_DRV_CMD_ReadFrequencyOffset,
    DMD_ATSC_DRV_CMD_SetSerialControl,
    DMD_ATSC_DRV_CMD_IIC_BYPASS_MODE,
    DMD_ATSC_DRV_CMD_SWITCH_SSPI_GPIO,
    DMD_ATSC_DRV_CMD_GPIO_GET_LEVEL,
    DMD_ATSC_DRV_CMD_GPIO_SET_LEVEL,
    DMD_ATSC_DRV_CMD_GPIO_OUT_ENABLE,
    DMD_ATSC_DRV_CMD_DoIQSwap,
    DMD_ATSC_DRV_CMD_GetReg,
    DMD_ATSC_DRV_CMD_SetReg,
    DMD_ATSC_DRV_CMD_SET_DIGRF_FREQ,
    DMD_ATSC_DRV_CMD_MD_Init,
    DMD_ATSC_DRV_CMD_MD_Exit,
    DMD_ATSC_DRV_CMD_MD_GetConfig,    
    DMD_ATSC_DRV_CMD_MD_SetConfig,
    DMD_ATSC_DRV_CMD_MD_SetReset,
    DMD_ATSC_DRV_CMD_MD_Set_QAM_SR,
    DMD_ATSC_DRV_CMD_MD_SetActive,
    DMD_ATSC_DRV_CMD_MD_SetPowerState,
    DMD_ATSC_DRV_CMD_MD_GetLock,
    DMD_ATSC_DRV_CMD_MD_GetModulationMode,
    DMD_ATSC_DRV_CMD_MD_GetSignalStrength,
    DMD_ATSC_DRV_CMD_MD_GetSignalQuality,
    DMD_ATSC_DRV_CMD_MD_GetSNRPercentage,
    DMD_ATSC_DRV_CMD_MD_GET_QAM_SNR,
    DMD_ATSC_DRV_CMD_MD_Read_uCPKT_ERR,
    DMD_ATSC_DRV_CMD_MD_GetPreViterbiBer,
    DMD_ATSC_DRV_CMD_MD_GetPostViterbiBer,
    DMD_ATSC_DRV_CMD_MD_ReadFrequencyOffset,
    DMD_ATSC_DRV_CMD_MD_SetSerialControl,
    DMD_ATSC_DRV_CMD_MD_IIC_BYPASS_MODE,
    DMD_ATSC_DRV_CMD_MD_SWITCH_SSPI_GPIO,
    DMD_ATSC_DRV_CMD_MD_GPIO_GET_LEVEL,
    DMD_ATSC_DRV_CMD_MD_GPIO_SET_LEVEL,
    DMD_ATSC_DRV_CMD_MD_GPIO_OUT_ENABLE,
    DMD_ATSC_DRV_CMD_MD_DoIQSwap,
    DMD_ATSC_DRV_CMD_MD_GetReg,
    DMD_ATSC_DRV_CMD_MD_SetReg,
    DMD_ATSC_DRV_CMD_MD_SET_DIGRF_FREQ,
} DMD_ATSC_DRV_COMMAND;

typedef struct DLL_PACKED _ATSC_DBG_LEVEL_PARAM
{
    DMD_ATSC_DbgLv u8DbgLevel;
} ATSC_DBG_LEVEL_PARAM, *PATSC_DBG_LEVEL_PARAM;

typedef struct DLL_PACKED _ATSC_GET_INFO_PARAM
{
    DMD_ATSC_Info* pInfo;
} ATSC_GET_INFO_PARAM, *PATSC_GET_INFO_PARAM;

typedef struct DLL_PACKED _ATSC_GET_LIB_VER_PARAM
{
    const MSIF_Version **ppVersion;
} ATSC_GET_LIB_VER_PARAM, *PATSC_GET_LIB_VER_PARAM;

typedef struct DLL_PACKED _ATSC_INIT_PARAM
{
    MS_U8 id;
    DMD_ATSC_InitData *pDMD_ATSC_InitData;
    MS_U32             u32InitDataLen;
} ATSC_INIT_PARAM, *PATSC_INIT_PARAM;

typedef struct DLL_PACKED _ATSC_ID_PARAM
{
    MS_U8 id;
} ATSC_ID_PARAM, *PATSC_ID_PARAM;

typedef struct DLL_PACKED _ATSC_SET_CONFIG_PARAM
{
    MS_U8 id;
    DMD_ATSC_DEMOD_TYPE eType;
    MS_BOOL             bEnable;
} ATSC_SET_CONFIG_PARAM, *PATSC_SET_CONFIG_PARAM;

typedef struct DLL_PACKED _ATSC_SET_QAM_SR_PARAM
{
    MS_U8 id;
    DMD_ATSC_DEMOD_TYPE eType;
    MS_U16              symbol_rate;
} ATSC_SET_QAM_SR_PARAM, *PATSC_SET_QAM_SR_PARAM;

typedef struct DLL_PACKED _ATSC_SET_ACTIVE_PARAM
{
    MS_U8 id;
    MS_BOOL bEnable;
} ATSC_SET_ACTIVE_PARAM, *PATSC_SET_ACTIVE_PARAM;

typedef struct DLL_PACKED _ATSC_SET_POWER_STATE_PARAM
{
    MS_U8 id;
    EN_POWER_MODE u16PowerState;
} ATSC_SET_POWER_STATE_PARAM, *PATSC_SET_POWER_STATE_PARAM;

typedef struct DLL_PACKED _ATSC_GET_LOCK_PARAM
{
    MS_U8 id;
    DMD_ATSC_GETLOCK_TYPE eType;
    DMD_ATSC_LOCK_STATUS  status;
} ATSC_GET_LOCK_PARAM, *PATSC_GET_LOCK_PARAM;

typedef struct DLL_PACKED _ATSC_GET_MODULATION_MODE_PARAM
{
    MS_U8 id;
    DMD_ATSC_DEMOD_TYPE eType;
} ATSC_GET_MODULATION_MODE_PARAM, *PATSC_GET_MODULATION_MODE_PARAM;

typedef struct DLL_PACKED _ATSC_GET_SIGNAL_STRENGTH_PARAM
{
    MS_U8 id;
    MS_U16 u16Strength;
} ATSC_GET_SIGNAL_STRENGTH_PARAM, *PATSC_GET_SIGNAL_STRENGTH_PARAM;

typedef struct DLL_PACKED _ATSC_GET_SIGNAL_QUALITY_PARAM
{
    MS_U8 id;
    DMD_ATSC_SIGNAL_CONDITION eQuality;
} ATSC_GET_SIGNAL_QUALITY_PARAM, *PATSC_GET_SIGNAL_QUALITY_PARAM;

typedef struct DLL_PACKED _ATSC_GET_SNR_PERCENTAGE_PARAM
{
    MS_U8 id;
    MS_U8 u8Percentage;
} ATSC_GET_SNR_PERCENTAGE_PARAM, *PATSC_GET_SNR_PERCENTAGE_PARAM;

typedef struct DLL_PACKED _ATSC_GET_SNR_PARAM 
{
    MS_U8 id;
    #ifdef UTPA2
    DMD_ATSC_SNR_DATA snr;
    #else
    float snr;
    #endif
} ATSC_GET_SNR_PARAM, *PATSC_GET_SNR_PARAM;

typedef struct DLL_PACKED _ATSC_GET_UCPKT_ERR_PARAM
{
    MS_U8 id;
    MS_U16 u16PacketErr;
} ATSC_GET_UCPKT_ERR_PARAM, *PATSC_GET_UCPKT_ERR_PARAM;

typedef struct DLL_PACKED _ATSC_GET_BER_PARAM
{
    MS_U8 id;
    #ifdef UTPA2
    DMD_ATSC_BER_DATA ber;
    #else
    float ber;
    #endif
} ATSC_GET_BER_PARAM, *PATSC_GET_BER_PARAM;

typedef struct DLL_PACKED _ATSC_READ_FREQ_OFFSET_PARAM
{
    MS_U8 id;
    #ifdef UTPA2
    DMD_ATSC_CFO_DATA cfo;
    #else
    MS_S16 cfo;
    #endif
} ATSC_READ_FREQ_OFFSET_PARAM, *PATSC_READ_FREQ_OFFSET_PARAM;

typedef struct DLL_PACKED _ATSC_SET_SERIAL_CONTROL_PARAM
{
    MS_U8 id;
    MS_U8 u8TsConfigData;
} ATSC_SET_SERIAL_CONTROL_PARAM, *PATSC_SET_SERIAL_CONTROL_PARAM;

typedef struct DLL_PACKED _ATSC_IIC_BYPASS_MODE_PARAM
{
    MS_U8 id;
    MS_BOOL bEnable;
} ATSC_IIC_BYPASS_MODE_PARAM, *PATSC_IIC_BYPASS_MODE_PARAM;

typedef struct DLL_PACKED _ATSC_SWITCH_SSPI_GPIO_PARAM
{
    MS_U8 id;
    MS_BOOL bEnable;
} ATSC_SWITCH_SSPI_GPIO_PARAM, *PATSC_SWITCH_SSPI_GPIO_PARAM;

typedef struct DLL_PACKED _ATSC_GPIO_LEVEL_PARAM
{
    MS_U8 id;
    MS_U8 u8Pin;
    MS_BOOL bLevel;
} ATSC_GPIO_LEVEL_PARAM, *PATSC_GPIO_LEVEL_PARAM;

typedef struct DLL_PACKED _ATSC_GPIO_OUT_ENABLE_PARAM
{
    MS_U8 id;
    MS_U8 u8Pin;
    MS_BOOL bEnableOut;
} ATSC_GPIO_OUT_ENABLE_PARAM, *PATSC_GPIO_OUT_ENABLE_PARAM;

typedef struct DLL_PACKED _ATSC_DO_IQ_SWAP_PARAM
{
    MS_U8 id;
    MS_BOOL bIsQPad;
} ATSC_DO_IQ_SWAP_PARAM, *PATSC_DO_IQ_SWAP_PARAM;

typedef struct DLL_PACKED _ATSC_REG_PARAM
{
    MS_U8 id;
    MS_U16 u16Addr;
    MS_U8 u8Data;
} ATSC_REG_PARAM, *PATSC_REG_PARAM;

typedef struct DLL_PACKED _ATSC_SET_DIGRF_PARAM
{
    MS_U8 id;
    MS_U32 u32RfFreq;
} ATSC_SET_DIGRF_PARAM, *PATSC_SET_DIGRF_PARAM;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

DLL_PUBLIC extern void ATSCRegisterToUtopia(FUtopiaOpen ModuleType);
DLL_PUBLIC extern MS_U32 ATSCOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute);
DLL_PUBLIC extern MS_U32 ATSCClose(void* pInstance);
DLL_PUBLIC extern MS_U32 ATSCIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs);

#ifdef __cplusplus
}
#endif

#endif // _DRV_ATSC_V2_H_

