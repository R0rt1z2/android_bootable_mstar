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
/// @brief  DVBC Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_DVBC_V2_H_
#define _DRV_DVBC_V2_H_

//#include "MsCommon.h"
//#include "drvDMD_common.h"
#include "drvDMD_INTERN_DVBC.h"
#ifdef __cplusplus
extern "C"
{
#endif

#ifndef DLL_PUBLIC 
#define DLL_PUBLIC 
#endif


#define DMD_DVBC_MAX_DEMOD_NUM          1



//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------
typedef struct DLL_PACKED
{
	MS_BOOL b_DCOFF_IsDVBC;   
	MS_BOOL b_IsInit_DVBCCreat;   
	EN_POWER_MODE eLastState;
} DMD_DVBC_SHARE_MEMORY_InitData;

typedef struct DLL_PACKED
{
    MS_U16 u16Version;
    MS_U16 u16SymbolRate;
    DMD_DVBC_MODULATION_TYPE eQamMode;
    MS_U32 u32IFFreq;
    MS_BOOL bSpecInv;
    MS_BOOL bSerialTS;
    MS_U8 u8SarValue;
    MS_U32 u32ChkScanTimeStart;
    DMD_DVBC_LOCK_STATUS eLockStatus;
    MS_U16 u16Strength;
    MS_U16 u16Quality;
    MS_U32 u32Intp; 	
    MS_U32 u32FcFs; 	
    MS_U8 u8Qam;
    MS_U16 u16SymbolRateHal; 	
} DMD_DVBC_SHARE_MEMORY_PreviousData;

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
    MS_U8 *u8DMD_DVBC_DSPRegInitExt; // TODO use system variable type
    MS_U8 u8DMD_DVBC_DSPRegInitSize;
    MS_U8 *u8DMD_DVBC_InitExt; // TODO use system variable type
} DMD_DVBC_InitData_Transform;

typedef struct DLL_PACKED
{
    DMD_DVBC_SHARE_MEMORY_InitData  sDMD_DVBC_SHARE_MEMORY_InitData;
	DMD_DVBC_SHARE_MEMORY_PreviousData sDMD_DVBC_SHARE_MEMORY_PreviousData;
	DMD_DVBC_InitData_Transform sDMD_DVBC_InitData_Transform;
} DMD_DVBC_ResData;






typedef enum {
    DMD_DVBC_DRV_CMD_Dual_Public_Init,
    DMD_DVBC_DRV_CMD_Dual_Individual_Init,
    DMD_DVBC_DRV_CMD_Init,
    DMD_DVBC_DRV_CMD_Exit,
    DMD_DVBC_DRV_CMD_SetDbgLevel,
    DMD_DVBC_DRV_CMD_GetInfo,
    DMD_DVBC_DRV_CMD_GetLibVer,
    DMD_DVBC_DRV_CMD_GetFWVer,
    DMD_DVBC_DRV_CMD_GetDSPReg,
    DMD_DVBC_DRV_CMD_SetDSPReg,
    DMD_DVBC_DRV_CMD_GetReg,
    DMD_DVBC_DRV_CMD_SetReg,
    DMD_DVBC_DRV_CMD_SetSerialControl,
    DMD_DVBC_DRV_CMD_SetConfig,
    DMD_DVBC_DRV_CMD_SetSetConfig_symbol_rate_list,
    DMD_DVBC_DRV_CMD_SetActive,
#ifdef UFO_DEMOD_DVBC_SUPPORT_DMD_INT
    DMD_DVBC_DRV_CMD_Get_Info_Generalization,
#endif
    DMD_DVBC_DRV_CMD_GetLockWithRFPower,
    DMD_DVBC_DRV_CMD_GetSignalStrengthWithRFPower,
    DMD_DVBC_DRV_CMD_GetSignalQualityWithRFPower,
    DMD_DVBC_DRV_CMD_ActiveDmdSwitch,
    DMD_DVBC_DRV_CMD_GetSNR,
    DMD_DVBC_DRV_CMD_GetPostViterbiBer,
    DMD_DVBC_DRV_CMD_GetPacketErr,
    DMD_DVBC_DRV_CMD_GetCellID,
    DMD_DVBC_DRV_CMD_GetStatus,
    DMD_DVBC_DRV_CMD_SetPowerState,
    
    //waiting add
    DMD_DVBC_DRV_CMD_GetIFAGC,
    DMD_DVBC_DRV_CMD_GetCurrentSymbolRateOffset,
    
    #ifdef UFO_DEMOD_DVBC_GET_AGC_INFO
    DMD_DVBC_DRV_CMD_GetAGCInfo,
    #endif
} DMD_DVBC_DRV_COMMAND;





typedef struct DLL_PACKED _DVBC_Dual_Public_Init_PARAM
{
	MS_U8 u8AGC_Tristate_Ctrl;
	MS_U8 u8Sar_Channel;
	MS_BOOL ret;
} DVBC_Dual_Public_Init_PARAM,*PDVBC_Dual_Public_Init_PARAM;

typedef struct DLL_PACKED _DVBC_Dual_Individual_Init_PARAM
{
	DMD_DVBC_InitData_Transform *pDMD_DVBC_InitData;
	MS_U32 u32InitDataLen;
	MS_BOOL ret;
}DVBC_Dual_Individual_Init_PARAM,*PDVBC_Dual_Individual_Init_PARAM;

typedef struct DLL_PACKED _DVBC_Init_PARAM
{
	DMD_DVBC_InitData_Transform *pDMD_DVBC_InitData;
	MS_U32 u32InitDataLen;
	MS_BOOL ret;
}DVBC_Init_PARAM,*PDVBC_Init_PARAM;

typedef struct DLL_PACKED _DVBC_SetDbgLevel_PARAM
{
	DMD_DVBC_DbgLv u8DbgLevel;
	MS_BOOL ret;
}DVBC_SetDbgLevel_PARAM,*PDVBC_SetDbgLevel_PARAM;

typedef struct DLL_PACKED _DVBC_EXIT_PARAM_PARAM
{
	MS_BOOL ret;
}DVBC_EXIT_PARAM_PARAM,*PDVBC_EXIT_PARAM_PARAM;

typedef struct DLL_PACKED _DVBC_GetInfo_PARAM
{
	const DMD_DVBC_Info* ret_info;
}DVBC_GetInfo_PARAM,*PDVBC_GetInfo_PARAM;

typedef struct _DVBC_GetLibVer_PARAM
{
	const MSIF_Version **ppVersion;
	MS_BOOL ret;
}DVBC_GetLibVer_PARAM,*PDVBC_GetLibVer_PARAM;

typedef struct DLL_PACKED _DVBC_GetFWVer_PARAM
{
	MS_U16 *ver;
	MS_BOOL ret;
}DVBC_GetFWVer_PARAM,*PDVBC_GetFWVer_PARAM;


typedef struct DLL_PACKED _DVBC_GetDSPReg_PARAM
{
	MS_U16 u16Addr;
	MS_U8 *pu8Data;
	MS_BOOL ret;
}DVBC_GetDSPReg_PARAM,*PDVBC_GetDSPReg_PARAM;

typedef struct DLL_PACKED _DVBC_SetDSPReg_PARAM
{
	MS_U16 u16Addr;
	MS_U8 pu8Data;
	MS_BOOL ret;
}DVBC_SetDSPReg_PARAM,*PDVBC_SetDSPReg_PARAM;


typedef struct DLL_PACKED _DVBC_GetReg_PARAM
{
	MS_U16 u16Addr;
	MS_U8 *pu8Data;
	MS_BOOL ret;
}DVBC_GetReg_PARAM,*PDVBC_GetReg_PARAM;

typedef struct DLL_PACKED _DVBC_SetReg_PARAM
{
	MS_U16 u16Addr;
	MS_U8 u8Data;
	MS_BOOL ret;
}DVBC_SetReg_PARAM,*PDVBC_SetReg_PARAM;


typedef struct DLL_PACKED _DVBC_SetSerialControl_PARAM
{
	MS_BOOL bEnable;
	MS_BOOL ret;
}DVBC_SetSerialControl_PARAM,*PDVBC_SetSerialControl_PARAM;


typedef struct DLL_PACKED _DVBC_SetConfig_Symbol_rate_list_PARAM
{
	MS_U16 u16SymbolRate;
	DMD_DVBC_MODULATION_TYPE eQamMode;
	MS_U32 u32IFFreq; 
	MS_BOOL bSpecInv;
	MS_BOOL bSerialTS; 
	MS_U16 *pu16_symbol_rate_list;
	MS_U8 u8_symbol_rate_list_num;
	MS_BOOL ret;
}DVBC_SetConfig_Symbol_rate_list_PARAM,*PDVBC_SetConfig_Symbol_rate_list_PARAM;

typedef struct DLL_PACKED _DVBC_SetActive_PARAM
{
	MS_BOOL bEnable;
	MS_BOOL ret;
}DVBC_SetActive_PARAM,*PDVBC_SetActive_PARAM;

/*
typedef struct DLL_PACKED _DVBC_GetLockWithRFPower_Transform
{
	DMD_DVBC_GETLOCK_TYPE eType;
	DMD_DVBC_LOCK_STATUS *eLockStatus;
	MS_BOOL ret;
}DVBC_GetLockWithRFPower_Transform,*PDVBC_GetLockWithRFPower_Transform;
*/
#ifdef UFO_DEMOD_DVBC_SUPPORT_DMD_INT
typedef struct DLL_PACKED _DVBC_GET_INFO_Generalization
{

	DMD_DVBC_GET_INFO_FUNC InfoType;
	void* pin;
	void* pout;
	MS_BOOL ret;
	
} DVBC_Get_Info_Generalization_PARAM, *PDVBC_Get_Info_Generalization_PARAM; 



typedef struct DLL_PACKED _DVBC_GET_INFO_TR_EVER_LOCK_IN
{
	MS_U8 demod_no;
	DMD_DVBC_GETLOCK_TYPE eType;


} DVBC_GET_INFO_TR_EVER_LOCK_IN, *P_DVBC_GET_INFO_TR_EVER_LOCK_IN; 

typedef struct DLL_PACKED _DVBC_GET_INFO_TR_EVER_LOCK_OUT
{
	DMD_DVBC_LOCK_STATUS *peLockStatus;
	MS_BOOL ret; 

} DVBC_GET_INFO_TR_EVER_LOCK_OUT, *P_DVBC_GET_INFO_TR_EVER_LOCK_OUT; 


#endif

typedef struct DLL_PACKED _DVBC_GetLockWithRFPower
{
        DMD_DVBC_GETLOCK_TYPE eType;
        union{
            MS_U32 u32CurrRFPowerDbm;
            MS_S32 s32CurrRFPowerDbm;
        };
        union{
            MS_U32 u32NoChannelRFPowerDbm;
            MS_S32 s32NoChannelRFPowerDbm;
        };
        MS_U32 u32TimeInterval;
        DMD_DVBC_LOCK_STATUS *eLockStatus;
        MS_BOOL ret;
}DVBC_GetLockWithRFPower,*PDVBC_GetLockWithRFPower;

/*
typedef struct _DVBC_GetSignalStrengthWithRFPower_PARAM
{
	MS_U16 *u16Strength;
	MS_BOOL ret;
}DVBC_GetSignalStrengthWithRFPower_PARAM,*PDVBC_GetSignalStrengthWithRFPower_PARAM;


typedef struct _DVBC_GetSignalQualityWithRFPower_PARAM
{
	MS_U16 *u16Quality;
	MS_BOOL ret;
}DVBC_GetSignalQualityWithRFPower_PARAM,*PDVBC_GetSignalQualityWithRFPower_PARAM;
*/

typedef struct DLL_PACKED _DVBC_ActiveDmdSwitch_PARAM
{
	MS_U8 demod_no;
	MS_BOOL ret;
}DVBC_ActiveDmdSwitch_PARAM,*PDVBC_ActiveDmdSwitch_PARAM;

//waiting add
typedef struct _DVBC_GetIFAGC_PARAM
{
	MS_U8 *ifagc_reg;
	MS_U8 *ifagc_reg_lsb;
	MS_U16 *ifagc_err_reg;
	MS_BOOL ret;
}DVBC_GetIFAGC_PARAM,*PDVBC_GetIFAGC_PARAM;

#ifdef UFO_DEMOD_DVBC_GET_AGC_INFO
typedef struct _DVBC_GetAGCInfo_PARAM
{
	MS_U8 u8dbg_mode;
	MS_U16 *pu16Data;
	MS_BOOL ret;
}DVBC_GetAGCInfo_PARAM,*PDVBC_GetAGCInfo_PARAM;
#endif


//typedef struct DLL_PACKED _DVBC_GetPacketErr_PARAM

typedef struct _DVBC_GetSNR_PARAM
{
	MS_U16 *snr_reg;
	MS_BOOL ret;
}DVBC_GetSNR_PARAM,*PDVBC_GetSNR_PARAM;


typedef struct _DVBC_GetPostViterbiBer_PARAM
{
	MS_U32 *BitErr_reg;
	MS_U16 *BitErrPeriod_reg;
	MS_BOOL ret;
}DVBC_GetPostViterbiBer_PARAM,*PDVBC_GetPostViterbiBer_PARAM;


typedef struct DLL_PACKED _DVBC_GetPacketErr_PARAM
{
	MS_U16 *pktErr;
	MS_BOOL ret;
}DVBC_GetPacketErr_PARAM,*PDVBC_GetPacketErr_PARAM;

typedef struct DLL_PACKED _DVBC_GetCurrentSymbolRateOffset_PARAM
{
	MS_U32 *pData;
	MS_BOOL ret;
}DVBC_GetCurrentSymbolRateOffset_PARAM,*PDVBC_GetCurrentSymbolRateOffset_PARAM;

typedef struct DLL_PACKED _DVBC_GetCellID_PARAM
{
	MS_U16 *u16CellID;
	MS_BOOL ret;
}DVBC_GetCellID_PARAM,*PDVBC_GetCellID_PARAM;


typedef struct _DVBC_GetStatus_PARAM
{
	DMD_DVBC_MODULATION_TYPE *pQAMMode;
	MS_U16 *u16SymbolRate;
	MS_U32 *config_Fc_reg;
	MS_U32 *Fc_over_Fs_reg;
	MS_U16 *Cfo_offset_reg;
	MS_BOOL ret;
}DVBC_GetStatus_PARAM,*PDVBC_GetStatus_PARAM;


typedef struct DLL_PACKED _DVBC_SetPowerState_PARAM
{
	EN_POWER_MODE u16PowerState;
	MS_U32 ret_U32;
}DVBC_SetPowerState_PARAM,*PDVBC_SetPowerState_PARAM;

void DVBCRegisterToUtopia(void);




/*******kernel mode transform structure***************/





/*************************************************/


MS_U32 DVBCOpen(void** pInstantTmp, MS_U32 u32ModuleVersion, void* pAttribute);
MS_U32 DVBCClose(void* pInstantTmp);
MS_U32 DVBCIoctl(void* pInstantTmp, MS_U32 u32Cmd, void* pArgs);
//MS_U32 DVBCStr(MS_U32 u32PowerState, void* pModule);


#ifdef __cplusplus
}
#endif

#endif // _DRV_DVBC_H_

