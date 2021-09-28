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
/// @brief  DVBT Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_DVBT_V2_H_
#define _DRV_DVBT_V2_H_

//#include "MsCommon.h"
//#include "drvDMD_common.h"
#include "drvDMD_INTERN_DVBT.h"
#ifdef __cplusplus
extern "C"
{
#endif


#define DMD_DVBT_MAX_DEMOD_NUM          1
//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------
typedef struct DLL_PACKED
{
	MS_BOOL b_DCOFF_IsDVBT;   
	MS_BOOL b_IsInit_DVBTCreat;   
	EN_POWER_MODE eLastState;
} DMD_DVBT_SHARE_MEMORY_InitData;

typedef struct DLL_PACKED
{
    MS_U16 u16Version;
    DMD_RF_CHANNEL_BANDWIDTH BW;
    MS_BOOL bSerialTS;
    MS_BOOL bPalBG;
    MS_BOOL bLPSel;
    MS_U32 u32IFFreq;
    MS_U32 u32FSFreq;
    MS_U8 u8IQSwap;    
    MS_U8 u8SarValue;
    MS_U32 u32ChkScanTimeStart;
    DMD_LOCK_STATUS eLockStatus;
    MS_U16 u16Strength;
    MS_U16 u16Quality;
    MS_U32 u32Intp; 	
    MS_U32 u32FcFs; 	
    MS_U8 u8Qam;
    MS_U16 u16SymbolRateHal; 	
} DMD_DVBT_SHARE_MEMORY_PreviousData;

typedef struct DLL_PACKED
{
    // tuner parameter
    MS_U8 u8SarChannel;

/*    
    DMD_RFAGC_SSI *pTuner_RfagcSsi;
    MS_U16 u16Tuner_RfagcSsi_Size;
    DMD_IFAGC_SSI *pTuner_IfagcSsi_LoRef;
    MS_U16 u16Tuner_IfagcSsi_LoRef_Size;
    DMD_IFAGC_SSI *pTuner_IfagcSsi_HiRef;
    MS_U16 u16Tuner_IfagcSsi_HiRef_Size;
    DMD_IFAGC_ERR *pTuner_IfagcErr_LoRef;
    MS_U16 u16Tuner_IfagcErr_LoRef_Size;
    DMD_IFAGC_ERR *pTuner_IfagcErr_HiRef;
    MS_U16 u16Tuner_IfagcErr_HiRef_Size;
    DMD_SQI_CN_NORDIGP1 *pSqiCnNordigP1;
    MS_U16 u16SqiCnNordigP1_Size;
*/
    // register init
    MS_U8 *u8DMD_DVBT_DSPRegInitExt; // TODO use system variable type
    MS_U8 u8DMD_DVBT_DSPRegInitSize;
    MS_U8 *u8DMD_DVBT_InitExt; // TODO use system variable type

    // for T/T2 merged FW
    MS_U32  u32EqStartAddr;
    MS_U32  u32TdiStartAddr;
    MS_U32  u32DjbStartAddr;
    MS_U32  u32FwStartAddr;
} DMD_DVBT_InitData_Transform;

typedef struct DLL_PACKED
{
  DMD_DVBT_SHARE_MEMORY_InitData  sDMD_DVBT_SHARE_MEMORY_InitData;
	DMD_DVBT_SHARE_MEMORY_PreviousData sDMD_DVBT_SHARE_MEMORY_PreviousData;
	DMD_DVBT_InitData_Transform sDMD_DVBT_InitData_Transform;
} DMD_DVBT_ResData;


typedef enum {
    DMD_DVBT_DRV_CMD_Init,
    DMD_DVBT_DRV_CMD_Exit,
    DMD_DVBT_DRV_CMD_SetDbgLeve,
    DMD_DVBT_DRV_CMD_GetInfo,
    DMD_DVBT_DRV_CMD_GetLibVer,
    DMD_DVBT_DRV_CMD_GetFWVer,
    DMD_DVBT_DRV_CMD_GetReg,
    DMD_DVBT_DRV_CMD_SetReg,
    DMD_DVBT_DRV_CMD_SetSerialControl,
    DMD_DVBT_DRV_CMD_SetConfig,
    DMD_DVBT_DRV_CMD_SetConfigHPLP,
    DMD_DVBT_DRV_CMD_SetConfigHPLPSetIF,
    DMD_DVBT_DRV_CMD_SetActive,
    DMD_DVBT_DRV_CMD_GetLock,
    DMD_DVBT_DRV_CMD_GetSignalStrength,
    DMD_DVBT_DRV_CMD_GetSignalStrengthWithRFPower,
    DMD_DVBT_DRV_CMD_GetSignalQuality,
    DMD_DVBT_DRV_CMD_GetSignalQualityWithRFPower,
    DMD_DVBT_DRV_CMD_GetSNR,
    DMD_DVBT_DRV_CMD_GetPostViterbiBer,
    DMD_DVBT_DRV_CMD_GetPreViterbiBer,
    DMD_DVBT_DRV_CMD_GetPacketErr,
    DMD_DVBT_DRV_CMD_GetTPSInfo,
    DMD_DVBT_DRV_CMD_GetCellID,
    DMD_DVBT_DRV_CMD_GetFreqOffset,
    DMD_DVBT_DRV_CMD_NORDIGSSITableWrite,
    DMD_DVBT_DRV_CMD_NORDIGSSITableRead,
    DMD_DVBT_DRV_CMD_SetPowerState,

    DMD_DVBT_DRV_CMD_GetIFAGC
} DMD_DVBT_DRV_COMMAND;


typedef struct DLL_PACKED _DVBT_EXIT_PARAM
{
	MS_BOOL ret;
}DVBT_EXIT_PARAM, *PDVBT_EXIT_PARAM;

typedef struct DLL_PACKED _DVBT_SETDBG_LEVEL_PARAM
{
    DMD_DbgLv u8DbgLevel;
    MS_BOOL ret;
} DVBT_SETDBG_LEVEL_PARAM, *PDVBT_SETDBG_LEVEL_PARAM;

typedef struct DLL_PACKED _DVBT_INIT_PARAM
{
    DMD_DVBT_InitData_Transform *DMD_DVBT_InitData; 
    MS_U32 u32InitDataLen;
	MS_BOOL ret;
} DVBT_INIT_PARAM, *PDVBT_INIT_PARAM;

typedef struct DLL_PACKED _DVBT_GETINFO_PARAM
{
    DMD_DVBT_INFO_TYPE eInfoType;
	DMD_DVBT_Info* pInfo;
	MS_BOOL ret;
} DVBT_GETINFO_PARAM, *PDVBT_GETINFO_PARAM;

/*
typedef struct _DVBT_GETINFO_RETURN_PARAM
{
    DMD_DVBT_Info* pInfo;
} DVBT_GETINFO_RETURN_PARAM, *PDVBT_GETINFO_RETURN_PARAM;
*/
typedef struct DLL_PACKED _DVBT_GETLIBVER_PARAM
{
    const MSIF_Version **ppVersion;
    MS_BOOL ret;
} DVBT_GETLIBVER_PARAM, *PDVBT_GETLIBVER_PARAM;

typedef struct DLL_PACKED _DVBT_GETFWVER_PARAM
{
    MS_U16 *ver;
   MS_BOOL ret;
} DVBT_GETFWVER_PARAM, *PDVBT_GETFWVER_PARAM;

typedef struct DLL_PACKED _DVBT_GETREG_PARAM
{
    MS_U16 u16Addr;
    MS_U8 *pu8Data;
   MS_BOOL ret;
} DVBT_GETREG_PARAM, *PDVBT_GETREG_PARAM;

typedef struct DLL_PACKED _DVBT_SETREG_PARAM
{
    MS_U16 u16Addr;
    MS_U8 u8Data;
   MS_BOOL ret;
} DVBT_SETREG_PARAM, *PDVBT_SETREG_PARAM;

typedef struct DLL_PACKED _DVBT_SetSerialControl_PARAM
{
    MS_BOOL bEnable;
    MS_BOOL ret;
} DVBT_SetSerialControl_PARAM, *PDVBT_SetSerialControl_PARAM;

typedef struct DLL_PACKED _DVBT_SetConfig_PARAM
{
    DMD_RF_CHANNEL_BANDWIDTH BW; 
    MS_BOOL bSerialTS; 
    MS_BOOL bPalBG;
    MS_BOOL ret;
} DVBT_SetConfig_PARAM, *PDVBT_SetConfig_PARAM;

typedef struct DLL_PACKED _DVBT_SetConfigHPLP_PARAM
{
    DMD_RF_CHANNEL_BANDWIDTH BW; 
    MS_BOOL bSerialTS; 
    MS_BOOL bPalBG; 
    MS_BOOL bLPSel;
    MS_BOOL ret;
} DVBT_SetConfigHPLP_PARAM, *PDVBT_SetConfigHPLP_PARAM;

typedef struct DLL_PACKED _DVBT_SetConfigHPLPSetIF_PARAM
{
    DMD_RF_CHANNEL_BANDWIDTH BW; 
    MS_BOOL bSerialTS; 
    MS_BOOL bPalBG; 
    MS_BOOL bLPSel; 
    MS_U32 u32IFFreq; 
    MS_U32 u32FSFreq; 
    MS_U8 u8IQSwap;
    MS_BOOL ret;
} DVBT_SetConfigHPLPSetIF_PARAM, *PDVBT_SetConfigHPLPSetIF_PARAM;

typedef struct DLL_PACKED _DVBT_SetActive_PARAM
{
    MS_BOOL bEnable;
    MS_BOOL ret;
} DVBT_SetActive_PARAM, *PDVBT_SetActive_PARAM;

typedef struct DLL_PACKED _DVBT_GetLock_PARAM
{
    DMD_DVBT_GETLOCK_TYPE eType; 
    DMD_LOCK_STATUS *eLockStatus;
    MS_BOOL ret;
} DVBT_GetLock_PARAM, *PDVBT_GetLock_PARAM;

typedef struct DLL_PACKED _DVBT_GetSignalStrength_PARAM
{
    MS_U16 *u16Strength;
    MS_BOOL ret;
} DVBT_GetSignalStrength_PARAM, *PDVBT_GetSignalStrength_PARAM;

/*bryan temp mark*/
#if(0)
typedef struct _DVBT_GetSignalStrengthWithRFPower_PARAM
{
    MS_U16 *u16Strength; 
    float fRFPowerDbm;
    MS_BOOL ret;
} DVBT_GetSignalStrengthWithRFPower_PARAM, *PDVBT_GetSignalStrengthWithRFPower_PARAM;
#endif

typedef struct DLL_PACKED _DVBT_GetSignalQuality_PARAM
{
    MS_U16 *u16Quality;
    MS_BOOL ret;
} DVBT_GetSignalQuality_PARAM, *PDVBT_GetSignalQuality_PARAM;

typedef struct DLL_PACKED _DVBT_GetSignalQualityWithRFPower_PARAM
{
    MS_U16 *u16Quality; 
    float fRFPowerDbm;
    MS_BOOL ret;
} DVBT_GetSignalQualityWithRFPower_PARAM, *PDVBT_GetSignalQualityWithRFPower_PARAM;


typedef struct _DVBT_GetSNR_PARAM
{
    MS_U32 *noise_power_reg;
    MS_BOOL ret;
} DVBT_GetSNR_PARAM, *PDVBT_GetSNR_PARAM;


typedef struct _DVBT_GetIFAGC_PARAM
{
	MS_U8 *ifagc_reg;
	MS_U8 *ifagc_reg_lsb;
	MS_U16 *ifagc_err_reg;
	MS_BOOL ret;
}DVBT_GetIFAGC_PARAM,*PDVBT_GetIFAGC_PARAM;


typedef struct _DVBT_GetPostViterbiBer_PARAM
{
	MS_U32 *BitErr_reg;
	MS_U16 *BitErrPeriod_reg;
	MS_U16 *PktErr_reg;
	MS_BOOL ret;
} DVBT_GetPostViterbiBer_PARAM, *PDVBT_GetPostViterbiBer_PARAM;


#if(0)
typedef struct _DVBT_GetPreViterbiBer_PARAM
{
    float *ber;
    MS_BOOL ret;
} DVBT_GetPreViterbiBer_PARAM, *PDVBT_GetPreViterbiBer_PARAM;
#endif

typedef struct DLL_PACKED _DVBT_GetPacketErr_PARAM
{
    MS_U16 *pktErr;
    MS_BOOL ret;
} DVBT_GetPacketErr_PARAM, *PDVBT_GetPacketErr_PARAM;

typedef struct DLL_PACKED _DVBT_GetTPSInfo_PARAM
{
    MS_U16 *u16Info;
    MS_BOOL ret;
} DVBT_GetTPSInfo_PARAM, *PDVBT_GetTPSInfo_PARAM;

typedef struct DLL_PACKED _DVBT_GetCellID_PARAM
{
    MS_U16 *u16CellID;
    MS_BOOL ret;
} DVBT_GetCellID_PARAM, *PDVBT_GetCellID_PARAM;

/*bryan temp mark*/
#if(0)
typedef struct _DVBT_GetFreqOffset_PARAM
{
    float *pFreqOff;
    MS_BOOL ret;
} DVBT_GetFreqOffset_PARAM, *PDVBT_GetFreqOffset_PARAM;
#endif

/*bryan temp mark*/
#if(0)
typedef struct _DVBT_NORDIGSSITableWrite_PARAM
{
    DMD_CONSTEL constel;
    DMD_CODERATE code_rate;
    float write_value;
    MS_BOOL ret;
} DVBT_NORDIGSSITableWrite_PARAM, *PDVBT_NORDIGSSITableWrite_PARAM;

typedef struct DLL_PACKED _DVBT_NORDIGSSITableRead_PARAM
{
    DMD_CONSTEL constel;
    DMD_CODERATE code_rate;
    float *read_value;
    MS_BOOL ret;
} DVBT_NORDIGSSITableRead_PARAM, *PDVBT_NORDIGSSITableRead_PARAM;
#endif

typedef struct DLL_PACKED _DVBT_SetPowerState_PARAM
{
    EN_POWER_MODE u16PowerState;
    MS_BOOL ret;
} DVBT_SetPowerState_PARAM, *PDVBT_SetPowerState_PARAM;


/*******kernel mode transform structure***************/





/*************************************************/


MS_U32 DVBTOpen(void** pInstantTmp, MS_U32 u32ModuleVersion, void* pAttribute);
MS_U32 DVBTClose(void* pInstantTmp);
MS_U32 DVBTIoctl(void* pInstantTmp, MS_U32 u32Cmd, void* pArgs);
//MS_U32 DVBTStr(MS_U32 u32PowerState, void* pModule);


#ifdef __cplusplus
}
#endif

#endif // _DRV_DVBT_H_

