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
/// @brief  DVBS Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_DVBS_V2_H_
#define _DRV_DVBS_V2_H_

//#include "MsCommon.h"
//#include "drvDMD_common.h"
#include "drvDMD_INTERN_DVBS.h"
#ifdef __cplusplus
extern "C"
{
#endif

#ifndef DLL_PUBLIC
#define DLL_PUBLIC
#endif
//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------
typedef enum {
    DMD_DVBS_DRV_CMD_Init,
    DMD_DVBS_DRV_CMD_Exit,
    DMD_DVBS_DRV_CMD_SetDbgLevel,
    DMD_DVBS_DRV_CMD_GetInfo,
    DMD_DVBS_DRV_CMD_GetLibVer,
    DMD_DVBS_DRV_CMD_GetFWVer,
    DMD_DVBS_DRV_CMD_GetDSPReg,
    DMD_DVBS_DRV_CMD_SetDSPReg,
    DMD_DVBS_DRV_CMD_GetReg,
    DMD_DVBS_DRV_CMD_SetReg,
    DMD_DVBS_DRV_CMD_SetSerialControl,
    DMD_DVBS_DRV_CMD_BlindScan_Config,
    DMD_DVBS_DRV_CMD_BlindScan_Config_symbol_rate_list,
    DMD_DVBS_DRV_CMD_SetConfig,
    DMD_DVBS_DRV_CMD_SetConfig_symbol_rate_list,
    DMD_DVBS_DRV_CMD_SetActive,
    DMD_DVBS_DRV_CMD_GetLockWithRFPower,
    DMD_DVBS_DRV_CMD_GetSignalStrengthWithRFPower,
    //DMD_DVBS_DRV_CMD_GetSignalQualityWithRFPower,
    DMD_DVBS_DRV_CMD_ActiveDmdSwitch,
    DMD_DVBS_DRV_CMD_GetSNR,
    DMD_DVBS_DRV_CMD_GetPostViterbiBer,
    DMD_DVBS_DRV_CMD_GetPacketErr,
    DMD_DVBS_DRV_CMD_GetCellID,
    DMD_DVBS_DRV_CMD_GetStatus,
    DMD_DVBS_DRV_CMD_SetPowerState,
    DMD_DVBS_DRV_CMD_Demod_Restart,
    DMD_DVBS_DRV_CMD_Get_FreqOffset,
    DMD_DVBS_DRV_CMD_BlindScan_Start,
    DMD_DVBS_DRV_CMD_BlindScan_NextFreq,
    DMD_DVBS_DRV_CMD_BlindScan_Cancel,
    DMD_DVBS_DRV_CMD_BlindScan_End,
    DMD_DVBS_DRV_CMD_BlindScan_GetChannel,
    DMD_DVBS_DRV_CMD_BlindScan_GetCurrentFreq,
    DMD_DVBS_DRV_CMD_BlindScan_WaitCurFreqFinished,
    DMD_DVBS_DRV_CMD_BlindScan_GetTunerFreq,
    DMD_DVBS_DRV_CMD_DiSEqC_Init,
    DMD_DVBS_DRV_CMD_DiSEqC_SetLNBOut,
    DMD_DVBS_DRV_CMD_DiSEqC_GetLNBOut,
    DMD_DVBS_DRV_CMD_DiSEqC_Set22kOnOff,
    DMD_DVBS_DRV_CMD_DiSEqC_Get22kOnOff,
    DMD_DVBS_DRV_CMD_DiSEqC_SendCmd,
    DMD_DVBS_DRV_CMD_DiSEqC_SetTxToneMode,
    DMD_DVBS_DRV_CMD_DiSEqC_SetTone,
    DMD_DVBS_DRV_CMD_UnicableAGCCheckPower,
    DMD_DVBS_DRV_CMD_GetIFAGC,

    //Leo add
    DMD_DVBS_DRV_CMD_GetTsDivNum,
    DMD_DVBS_DRV_CMD_GetCurrentDemodCodeRate,
    DMD_DVBS_DRV_CMD_GetCurrentModulationType,
    DMD_DVBS_DRV_CMD_GetTunrSignalLevel_PWR,
    DMD_DVBS_DRV_CMD_TS_Enable,
    DMD_DVBS_DRV_CMD_GetBCHEflagWindow,
    DMD_DVBS_DRV_CMD_Customized_DiSEqC_SendCmd,

    DMD_DVBS_DRV_CMD_Set_IS_ID,
    DMD_DVBS_DRV_CMD_Set_Default_IS_ID,
    DMD_DVBS_DRV_CMD_Get_IS_ID_INFO,
    DMD_DVBS_DRV_CMD_VCM_Init,
    DMD_DVBS_DRV_CMD_VCM_Check,
    DMD_DVBS_DRV_CMD_VCM_Enabled,
    DMD_DVBS_DRV_CMD_VCM_Mode,
    DMD_DVBS_DRV_CMD_INT_MODE,
    DMD_DVBS_DRV_CMD_GetParam,
    DMD_DVBS_DRV_CMD_GetAGCInfo,
    DMD_DVBS_DRV_CMD_GetCurrentDemodType
} DMD_DVBS_DRV_COMMAND;


typedef struct DLL_PACKED _DMD_DVBS_InitData_Transform
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
    MS_U8 *u8DMD_DVBS_DSPRegInitExt; // TODO use system variable type
    MS_U8 u8DMD_DVBS_DSPRegInitSize;
    MS_U8 *u8DMD_DVBS_InitExt; // TODO use system variable type
    MS_U32  u32FwStartAddr;
    MS_U32  u32DjbStartAddr;
} DMD_DVBS_InitData_Transform;

typedef struct DLL_PACKED _DMD_DVBS_InitData_STR
{
    MS_BOOL b_DCOFF_IsDVBS;
    MS_BOOL b_IsInit_DVBSSuspend;
    EN_POWER_MODE eLastState;
} DMD_DVBS_InitData_STR;

typedef struct DLL_PACKED _DMD_DVBS_PreviousData
{
    MS_U16 u16Version;
    MS_U16 u16SymbolRate;
    DMD_DVBS_MODULATION_TYPE eQamMode;
    MS_U32 u32IFFreq;
    MS_BOOL bSpecInv;
    MS_BOOL bSerialTS;
    MS_U8 u8SarValue;
    MS_U32 u32ChkScanTimeStart;
    DMD_DVBS_LOCK_STATUS eLockStatus;
    MS_U16 u16Strength;
    MS_U16 u16Quality;
    MS_U32 u32Intp;
    MS_U32 u32FcFs;
    MS_U8 u8Qam;
    MS_U32 u32SymbolRate;
} DMD_DVBS_PreviousData;

typedef struct DLL_PACKED _DMD_DVBS_ResData
{
    DMD_DVBS_InitData_STR  sDMD_DVBS_InitData_STR;
    DMD_DVBS_PreviousData sDMD_DVBS_PreviousData;
    DMD_DVBS_InitData_Transform sDMD_DVBS_InitData_Transform;
} DMD_DVBS_ResData;

typedef struct DLL_PACKED _DVBS_Init_PARAM
{
    DMD_DVBS_InitData_Transform *pDMD_DVBS_InitData;
    MS_U32 u32InitDataLen;
    MS_BOOL ret;
}DVBS_Init_PARAM,*PDVBS_Init_PARAM;

typedef struct DLL_PACKED _DVBS_SetDbgLevel_PARAM
{
    DMD_DVBS_DbgLv u8DbgLevel;
    MS_BOOL ret;
}DVBS_SetDbgLevel_PARAM,*PDVBS_SetDbgLevel_PARAM;

typedef struct DLL_PACKED _DVBS_EXIT_PARAM_PARAM
{
    MS_BOOL ret;
}DVBS_EXIT_PARAM_PARAM,*PDVBS_EXIT_PARAM_PARAM;

typedef struct DLL_PACKED _DVBS_GetInfo_PARAM
{
    const DMD_DVBS_Info* ret_info;
}DVBS_GetInfo_PARAM,*PDVBS_GetInfo_PARAM;

typedef struct _DVBS_GetLibVer_PARAM
{
    const MSIF_Version **ppVersion;
    MS_BOOL ret;
}DVBS_GetLibVer_PARAM,*PDVBS_GetLibVer_PARAM;

typedef struct DLL_PACKED _DVBS_GetFWVer_PARAM
{
    MS_U16 *ver;
    MS_BOOL ret;
}DVBS_GetFWVer_PARAM,*PDVBS_GetFWVer_PARAM;


typedef struct DLL_PACKED _DVBS_GetDSPReg_PARAM
{
    MS_U16 u16Addr;
    MS_U8 *pu8Data;
    MS_BOOL ret;
}DVBS_GetDSPReg_PARAM,*PDVBS_GetDSPReg_PARAM;

typedef struct DLL_PACKED _DVBS_SetDSPReg_PARAM
{
    MS_U16 u16Addr;
    MS_U8 pu8Data;
    MS_BOOL ret;
}DVBS_SetDSPReg_PARAM,*PDVBS_SetDSPReg_PARAM;


typedef struct DLL_PACKED _DVBS_GetReg_PARAM
{
    MS_U16 u16Addr;
    MS_U8 *pu8Data;
    MS_BOOL ret;
}DVBS_GetReg_PARAM,*PDVBS_GetReg_PARAM;

typedef struct DLL_PACKED _DVBS_SetReg_PARAM
{
    MS_U16 u16Addr;
    MS_U8 u8Data;
    MS_BOOL ret;
}DVBS_SetReg_PARAM,*PDVBS_SetReg_PARAM;


typedef struct DLL_PACKED _DVBS_SetSerialControl_PARAM
{
    MS_BOOL bEnable;
    MS_BOOL ret;
}DVBS_SetSerialControl_PARAM,*PDVBS_SetSerialControl_PARAM;

typedef struct DLL_PACKED _DVBS_BlindScan_Config_Symbol_rate_list_PARAM
{
    MS_U32 u32SymbolRate;
    DMD_DVBS_MODULATION_TYPE eQamMode;
    MS_U32 u32IFFreq;
    MS_BOOL bSpecInv;
    MS_BOOL bSerialTS;
    MS_U16 *pu16_symbol_rate_list;
    MS_U8 u8_symbol_rate_list_num;
    MS_BOOL ret;
}DVBS_BlindScan_Config_Symbol_rate_list_PARAM,*PDVBS_BlindScan_Config_Symbol_rate_list_PARAM;

typedef struct DLL_PACKED _DVBS_SetConfig_Symbol_rate_list_PARAM
{
    MS_U32 u32SymbolRate;
    MS_U16 u16SymbolRate;
    DMD_DVBS_MODULATION_TYPE eQamMode;
    MS_U32 u32IFFreq;
    MS_BOOL bSpecInv;
    MS_BOOL bSerialTS;
    MS_U16 *pu16_symbol_rate_list;
    MS_U8 u8_symbol_rate_list_num;
    MS_BOOL ret;
}DVBS_SetConfig_Symbol_rate_list_PARAM,*PDVBS_SetConfig_Symbol_rate_list_PARAM;

typedef struct DLL_PACKED _DVBS_SetActive_PARAM
{
    MS_BOOL bEnable;
    MS_BOOL ret;
}DVBS_SetActive_PARAM,*PDVBS_SetActive_PARAM;

typedef struct DLL_PACKED _DVBS_GetLockWithRFPower
{
    DMD_DVBS_GETLOCK_TYPE eType;
    DMD_DVBS_LOCK_STATUS *eLockStatus;
    MS_U32 u32CurrRFPowerDbm;
    MS_U32 u32NoChannelRFPowerDbm;
    MS_BOOL ret;
}DVBS_GetLockWithRFPower,*PDVBS_GetLockWithRFPower;

typedef struct DLL_PACKED _DVBS_ActiveDmdSwitch_PARAM
{
    MS_U8 demod_no;
    MS_BOOL ret;
}DVBS_ActiveDmdSwitch_PARAM,*PDVBS_ActiveDmdSwitch_PARAM;

typedef struct _DVBS_GetSignalStrengthWithRFPower_PARAM
{
    MS_U16 fRFPowerDbm;
    DMD_DVBS_DEMOD_TYPE *pDemodType;
    MS_U8  *u8_DVBS2_CurrentCodeRateLocal;
    MS_U8   *u8_DVBS2_CurrentConstellationLocal;
    MS_BOOL ret;
}DVBS_GetSignalStrengthWithRFPower_PARAM,*PDVBS_GetSignalStrengthWithRFPower_PARAM;

//Leo add
typedef struct _DVBS_GetIFAGC_PARAM
{
    MS_U8 *ifagc_reg;
    MS_U8 *ifagc_reg_lsb;
    MS_U16 *ifagc_err_reg;
    MS_BOOL ret;
}DVBS_GetIFAGC_PARAM,*PDVBS_GetIFAGC_PARAM;


//typedef struct DLL_PACKED _DVBS_GetPacketErr_PARAM

typedef struct _DVBS_GetSNR_PARAM
{
    MS_U32 *u32NDA_SNR_A;
    MS_U32 *u32NDA_SNR_AB;
    MS_BOOL ret;
}DVBS_GetSNR_PARAM,*PDVBS_GetSNR_PARAM;


typedef struct _DVBS_GetPostViterbiBer_PARAM
{
    MS_U32 *BitErr_reg;
    MS_U16 *BitErrPeriod_reg;
    MS_BOOL ret;
}DVBS_GetPostViterbiBer_PARAM,*PDVBS_GetPostViterbiBer_PARAM;


typedef struct DLL_PACKED _DVBS_GetPacketErr_PARAM
{
    MS_U16 *pktErr;
    MS_BOOL ret;
}DVBS_GetPacketErr_PARAM,*PDVBS_GetPacketErr_PARAM;

typedef struct DLL_PACKED _DVBS_GetCellID_PARAM
{
    MS_U16 *u16CellID;
    MS_BOOL ret;
}DVBS_GetCellID_PARAM,*PDVBS_GetCellID_PARAM;


typedef struct _DVBS_GetStatus_PARAM
{
    DMD_DVBS_MODULATION_TYPE *pQAMMode;
    MS_U32 *u32SymbolRate;
    MS_S16  *s16CFO;
    MS_BOOL ret;
}DVBS_GetStatus_PARAM,*PDVBS_GetStatus_PARAM;


typedef struct DLL_PACKED _DVBS_SetPowerState_PARAM
{
    EN_POWER_MODE u16PowerState;
    MS_U32 ret_U32;
}DVBS_SetPowerState_PARAM,*PDVBS_SetPowerState_PARAM;

typedef struct DLL_PACKED _DVBS_Get_FreqOffset_PARAM
{
    MS_S16  *s16CFO;
    MS_BOOL bRet;
}DVBS_Get_FreqOffset_PARAM,*PDVBS_Get_FreqOffset_PARAM;

typedef struct DLL_PACKED _DVBS_BlindScan_Start_PARAM
{
    MS_U16 u16StartFreq;
    MS_U16 u16EndFreq;
    MS_BOOL bRet;
}DVBS_BlindScan_Start_PARAM,*PDVBS_BlindScan_Start_PARAM;

typedef struct DLL_PACKED _DVBS_BlindScan_NextFreq_PARAM
{
    MS_BOOL *bBlindScanEnd;
    MS_BOOL bRet;
}DVBS_BlindScan_NextFreq_PARAM,*PDVBS_BlindScan_NextFreq_PARAM;


typedef struct DLL_PACKED _DVBS_BlindScan_Cancel_PARAM
{
    MS_BOOL bRet;
}DVBS_BlindScan_Cancel_PARAM,*PDVBS_BlindScan_Cancel_PARAM;

typedef struct DLL_PACKED _DVBS_BlindScan_End_PARAM
{
    MS_BOOL bRet;
}DVBS_BlindScan_End_PARAM,*PDVBS_BlindScan_End_PARAM;

typedef struct DLL_PACKED _DVBS_BlindScan_GetChannel_PARAM
{
    MS_U16 u16ReadStart;
    MS_U16 *u16TPNum;
    HAL_DEMOD_MS_FE_CARRIER_PARAM *pTable;
    MS_BOOL bRet;
}DVBS_BlindScan_GetChannel_PARAM,*PDVBS_BlindScan_GetChannel_PARAM;

typedef struct DLL_PACKED _DVBS_BlindScan_GetCurrentFreq_PARAM
{
    MS_U32 *u32CurrentFreq;
    MS_BOOL bRet;
}DVBS_BlindScan_GetCurrentFreq_PARAM,*PDVBS_BlindScan_GetCurrentFreq_PARAM;

typedef struct DLL_PACKED _DVBS_BlindScan_WaitCurFreqFinished_PARAM
{
    MS_U8 *u8Progress;
    MS_U8 *u8FindNum;
    MS_U8 *substate_reg;
    MS_U32 *u32Data;
    MS_U16 *symbolrate_reg;
    MS_U16 *CFO_reg;
    MS_BOOL bRet;
}DVBS_BlindScan_WaitCurFreqFinished_PARAM,*PDVBS_BlindScan_WaitCurFreqFinished_PARAM;

typedef struct DLL_PACKED _DVBS_BlindScan_GetTunerFreq_PARAM
{
    MS_U16 *u16TunerCenterFreq;
    MS_U16 *u16TunerCutOffFreq;
    MS_BOOL bRet;
}DVBS_BlindScan_GetTunerFreq_PARAM,*PDVBS_BlindScan_GetTunerFreq_PARAM;

typedef struct DLL_PACKED _DVBS_DiSEqC_Init_PARAM
{
    MS_BOOL bRet;
}DVBS_DiSEqC_Init_PARAM,*PDVBS_DiSEqC_Init_PARAM;

typedef struct DLL_PACKED _DVBS_DiSEqC_SetLNBOut_PARAM
{
    MS_BOOL bLow;
    MS_BOOL bRet;
}DVBS_DiSEqC_SetLNBOut_PARAM,*PDVBS_DiSEqC_SetLNBOut_PARAM;

typedef struct DLL_PACKED _DVBS_DiSEqC_GetLNBOut_PARAM
{
    MS_BOOL *bLNBOutLow;
    MS_BOOL bRet;
}DVBS_DiSEqC_GetLNBOut_PARAM,*PDVBS_DiSEqC_GetLNBOut_PARAM;

typedef struct DLL_PACKED _DVBS_DiSEqC_Set22kOnOff_PARAM
{
    MS_BOOL b22kOn;
    MS_BOOL bRet;
}DVBS_DiSEqC_Set22kOnOff_PARAM,*PDVBS_DiSEqC_Set22kOnOff_PARAM;

typedef struct DLL_PACKED _DVBS_DiSEqC_Get22kOnOff_PARAM
{
    MS_BOOL *b22kOn;
    MS_BOOL bRet;
}DVBS_DiSEqC_Get22kOnOff_PARAM,*PDVBS_DiSEqC_Get22kOnOff_PARAM;

typedef struct DLL_PACKED _DVBS_DiSEqC_SendCmd_PARAM
{
    MS_U8 *pCmd;
    MS_U8 u8CmdSize;
    MS_BOOL bRet;
}DVBS_DiSEqC_SendCmd_PARAM,*PDVBS_DiSEqC_SendCmd_PARAM;

typedef struct DLL_PACKED _DVBS_DiSEqC_SetTxToneMode_PARAM
{
    MS_BOOL bTxTone22kOff;
    MS_BOOL bRet;
}DVBS_DiSEqC_SetTxToneMode_PARAM,*PDVBS_DiSEqC_SetTxToneMode_PARAM;

typedef struct DLL_PACKED _DVBS_DiSEqC_SetTone_PARAM
{
    MS_BOOL bTone1;
    MS_BOOL bRet;
}DVBS_DiSEqC_SetTone_PARAM,*PDVBS_DiSEqC_SetTone_PARAM;

typedef struct DLL_PACKED _DVBS_UnicableAGCCheckPower_PARAM
{
    MS_BOOL pbAGCCheckPower;
    MS_BOOL bRet;
}DVBS_UnicableAGCCheckPower_PARAM,*PDVBS_UnicableAGCCheckPower_PARAM;

typedef struct DLL_PACKED _DVBS_GetTsDivNum_PARAM
{
    MS_U32 *u32SymbolRate;
    MS_U8* system_type_reg;
    MS_U8 *code_rate_idx;
    MS_U8 *fec_type_idx;
    MS_U8 *pilot_flag;
    MS_U32 *u32temp;
    MS_U8 *code_rate_reg;
    MS_BOOL bRet;
}DVBS_GetTsDivNum_PARAM,*PDVBS_GetTsDivNum_PARAM;

typedef struct DLL_PACKED _DVBS_GetCurrentDemodCodeRate_PARAM
{
    DMD_DVBS_CODE_RATE_TYPE *pCodeRate;
    MS_BOOL bRet;
}DVBS_GetCurrentDemodCodeRate_PARAM,*PDVBS_GetCurrentDemodCodeRate_PARAM;

typedef struct DLL_PACKED _DVBS_GetCurrentModulationType_PARAM
{
    DMD_DVBS_MODULATION_TYPE *mod_type;
    MS_BOOL bRet;
}DVBS_GetCurrentModulationType_PARAM,*PDVBS_GetCurrentModulationType_PARAM;

typedef struct DLL_PACKED _DVBS_GetTunrSignalLevel_PWR_PARAM
{
    MS_U16 *u16Data;
    MS_BOOL bRet;
}DVBS_GetTunrSignalLevel_PWR_PARAM,*PDVBS_GetTunrSignalLevel_PWR_PARAM;

typedef struct DLL_PACKED _DVBS_SetDefaultVCM_PARAM
{
    MS_U8 *u8IS_ID;
    MS_U8 *u8IS_ID_table;
    MS_BOOL ret;
}DVBS_SetDefaultVCM_PARAM,*PDVBS_SetDefaultVCM_PARAM;

typedef struct DLL_PACKED _DVBS_SetVCM_PARAM
{
    MS_U8 u8IS_ID;
    MS_BOOL ret;
}DVBS_SetVCM_PARAM,*PDVBS_SetVCM_PARAM;

typedef struct DLL_PACKED _DVBS_GetVCM_PARAM
{
    MS_U8 *u8IS_ID;
    MS_U8 *u8IS_ID_table;
    MS_BOOL ret;
}DVBS_GetVCM_PARAM,*PDVBS_GetVCM_PARAM;

typedef struct DLL_PACKED _DVBS_VCM_INIT_PARAM
{
    DMD_DVBS_VCM_OPT u8VCM_OPT;
    MS_U8 u8IS_ID;
    MS_U32 u32DVBS2_DJB_START_ADDR;
    MS_BOOL ret;
}DVBS_VCM_INIT_PARAM,*PDVBS_VCM_INIT_PARAM;

typedef struct DLL_PACKED _DVBS_VCM_CHECK
{
    MS_BOOL ret;
}DVBS_VCM_CHECK,*PDVBS_VCM_CHECK;

typedef struct DLL_PACKED _DVBS_VCM_ENABLED
{
    MS_U8 u8VCM_ENABLED;
    MS_BOOL ret;
}DVBS_VCM_ENABLED,*PDVBS_VCM_ENABLED;

typedef struct DLL_PACKED _DVBS_VCM_MODE
{
    DMD_DVBS_VCM_OPT u8VCM_OPT;
    MS_BOOL ret;
}DVBS_VCM_MODE,*PDVBS_VCM_MODE;

#ifdef UFO_DEMOD_DVBS_SUPPORT_DMD_INT
typedef struct DLL_PACKED _DVBS_INT_MODE
{
    fpIntCallBack fpCB;
    MS_BOOL ret;
}DVBS_INT_MODE,*PDVBS_INT_MODE;
#endif

#ifdef UFO_DEMOD_GetParam_NEW_FLOW
    /// Define tuning paramter of DVB-S front-end
typedef struct DLL_PACKED _DVBS_GET_INFO
{
    //HAL_DEMOD_EN_SAT_MOD_TYPE             eDemod_Type;                        ///< Mode type
    HAL_DEMOD_EN_SAT_CONSTEL_TYPE           eConstellation;                     ///< Constellation type
    HAL_DEMOD_EN_SAT_ROLL_OFF_TYPE          eRollOff;                           ///< Roll-Off factor
    HAL_DEMOD_EN_SAT_IQ_MODE                eIQ_Mode;                           ///< IQ mode
    HAL_DEMOD_EN_CONV_CODE_RATE_TYPE        eCodeRate;                          ///< Converlution code rate
    MS_U32                                  u32SymbolRate;
    HAL_DEMOD_EN_SAT_MOD_TYPE               eDemodType;
    float                                   eFreqoff;
    //MS_U8                                 u8Polarity;                         // 0: Horizon; > 0(default 1): Vertical;
    //MS_S16                                s16FreqOffset;
    MS_BOOL ret;
} DVBS_GET_INFO, *PDVBS_GET_INFO;
#endif

#ifdef UFO_DEMOD_GET_AGC_INFO
typedef struct DLL_PACKED _DVBS_GET_AGC_INFO
{
    MS_U8 u8dbg_mode;
    MS_U16 *pu16Data;
    MS_BOOL ret;
} DVBS_GET_AGC_INFO, *PDVBS_GET_AGC_INFO;
#endif

typedef struct DLL_PACKED _DVBS_TS_Enable_PARAM
{
    MS_BOOL TS_Enable;
    MS_BOOL ret;
}DVBS_TS_Enable_PARAM, *PDVBS_TS_Enable_PARAM;

typedef struct DLL_PACKED _DVBS_GetBCHEflagWindow_PARAM
{
    MS_U16 *BCH_Eflag_Window;
    MS_BOOL ret;
}DVBS_GetBCHEflagWindow_PARAM, *PDVBS_GetBCHEflagWindow_PARAM;

#ifdef UFO_DEMOD_DVBS_CUSTOMIZED_DISEQC_SEND_CMD
typedef struct DLL_PACKED _DVBS_Customized_DiSEqC_SendCmd_PARAM
{
    MS_U8 *pCmd;
    MS_U8 u8CmdSize;
    MS_BOOL ret;
}DVBS_Customized_DiSEqC_SendCmd_PARAM, *PDVBS_Customized_DiSEqC_SendCmd_PARAM;
#endif

typedef struct DLL_PACKED _DVBS_GetCurrentDemodType_PARAM
{
    DMD_DVBS_DEMOD_TYPE *pDemodType;
    MS_BOOL bRet;
}DVBS_GetCurrentDemodType_PARAM,*PDVBS_GetCurrentDemodType_PARAM;

void DVBSRegisterToUtopia(void);

/*******kernel mode transform structure***************/

/*************************************************/

MS_U32 DVBSOpen(void** pInstantTmp, MS_U32 u32ModuleVersion, void* pAttribute);
MS_U32 DVBSClose(void* pInstantTmp);
MS_U32 DVBSIoctl(void* pInstantTmp, MS_U32 u32Cmd, void* pArgs);
//MS_U32 DVBSStr(MS_U32 u32PowerState, void* pModule);


#ifdef __cplusplus
}
#endif

#endif // _DRV_DVBS_H_

