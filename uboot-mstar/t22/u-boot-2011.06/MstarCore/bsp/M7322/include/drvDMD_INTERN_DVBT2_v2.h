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

#ifndef _DRV_DVBT2_V2_H_
#define _DRV_DVBT2_V2_H_

//#include "MsCommon.h"
//#include "drvDMD_common.h"
#include "drvDMD_INTERN_DVBT2.h"
#ifdef __cplusplus
extern "C"
{
#endif

#define DMD_DVBT2_MAX_DEMOD_NUM          1
//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------
typedef struct DLL_PACKED
{
	MS_BOOL b_DCOFF_IsDVBT2;
	MS_BOOL b_IsInit_DVBT2Creat;
	EN_POWER_MODE eLastState;
} DMD_DVBT2_SHARE_MEMORY_InitData;

typedef struct DLL_PACKED
{
    MS_U16 u16Version;
    DMD_DVBT2_RF_CHANNEL_BANDWIDTH BW;
    MS_BOOL bSerialTS;
    MS_U8 u8PlpID;
    MS_U8 u8SarValue;
    MS_U32 u32ChkScanTimeStart;
    DMD_T2_LOCK_STATUS eLockStatus;
    MS_U16 u16Strength;
    MS_U16 u16Quality;
    MS_U32 u32Intp;
    MS_U32 u32FcFs;
    MS_U8 u8Qam;
    MS_U16 u16SymbolRateHal;
    MS_U8 u8GroupID;
} DMD_DVBT2_SHARE_MEMORY_PreviousData;

typedef struct DLL_PACKED _DMD_DVBT2_InitData_Transform
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
    MS_U8 *u8DMD_DVBT2_DSPRegInitExt; // TODO use system variable type
    MS_U8 u8DMD_DVBT2_DSPRegInitSize;
    MS_U8 *u8DMD_DVBT2_InitExt; // TODO use system variable type

    MS_U32  u32EqStartAddr;
    MS_U32  u32TdiStartAddr;
    MS_U32  u32DjbStartAddr;
    MS_U32  u32FwStartAddr;
} DMD_DVBT2_InitData_Transform;

typedef struct DLL_PACKED
{
  DMD_DVBT2_SHARE_MEMORY_InitData  sDMD_DVBT2_SHARE_MEMORY_InitData;
	DMD_DVBT2_SHARE_MEMORY_PreviousData sDMD_DVBT2_SHARE_MEMORY_PreviousData;
	DMD_DVBT2_InitData_Transform sDMD_DVBT2_InitData_Transform;
} DMD_DVBT2_ResData;

typedef enum {
    DMD_DVBT2_DRV_CMD_Init                        ,
    DMD_DVBT2_DRV_CMD_Exit                        ,
    DMD_DVBT2_DRV_CMD_SetDbgLevel                 ,
    DMD_DVBT2_DRV_CMD_GetInfo                     ,
    DMD_DVBT2_DRV_CMD_GetLibVer                   ,
    DMD_DVBT2_DRV_CMD_GetFWVer                    ,
    DMD_DVBT2_DRV_CMD_GetReg                      ,
    DMD_DVBT2_DRV_CMD_SetReg                      ,
    DMD_DVBT2_DRV_CMD_SetSerialControl            ,
    DMD_DVBT2_DRV_CMD_SetReset                    ,
    DMD_DVBT2_DRV_CMD_SetConfig                   ,
    DMD_DVBT2_DRV_CMD_SetActive                   ,
    DMD_DVBT2_DRV_CMD_GetLock                     ,
//    DMD_DVBT2_DRV_CMD_GetSignalStrengthWithRFPower,
//    DMD_DVBT2_DRV_CMD_GetSignalQualityWithRFPower ,
    DMD_DVBT2_DRV_CMD_GetSNR                      ,
    DMD_DVBT2_DRV_CMD_GetPostLdpcBer              ,
    DMD_DVBT2_DRV_CMD_GetPreLdpcBer               ,
    DMD_DVBT2_DRV_CMD_GetPacketErr                ,
    DMD_DVBT2_DRV_CMD_GetL1Info                   ,
    DMD_DVBT2_DRV_CMD_GetFreqOffset               ,
    DMD_DVBT2_DRV_CMD_NORDIG_SSI_Table_Write      ,
    DMD_DVBT2_DRV_CMD_NORDIG_SSI_Table_Read       ,
    DMD_DVBT2_DRV_CMD_SetPowerState               ,
    DMD_DVBT2_DRV_CMD_GetPlpBitMap                ,
    DMD_DVBT2_DRV_CMD_GetPlpGroupID								,
    DMD_DVBT2_DRV_CMD_SetPlpID                    ,

} DMD_DVBT2_DRV_COMMAND;
/// For demod init



typedef struct DLL_PACKED _DVBT2_INIT_PARAM
{
	DMD_DVBT2_InitData_Transform *pDMD_DVBT2_InitData;
	MS_U32 u32InitDataLen;
	MS_BOOL ret;
}DVBT2_INIT_PARAM, *PDVBT2_INIT_PARAM;


typedef struct DLL_PACKED _DVBT2_EXIT_PARAM
{
	MS_BOOL ret;
}DVBT2_EXIT_PARAM, *PDVBT2_EXIT_PARAM;


typedef struct DLL_PACKED _DVBT2_SETDBGLEVEL_PARAM
{
	DMD_T2_DbgLv u8DbgLevel;
	MS_BOOL ret;
}DVBT2_SETDBGLEVEL_PARAM, *PDVBT2_SETDBGLEVEL_PARAM;

typedef struct DLL_PACKED _DVBT2_GETINFO_PARAM
{
	DMD_DVBT2_INFO_TYPE eInfoType;
	DMD_DVBT2_Info* pInfo;
	MS_BOOL ret;
}DVBT2_GETINFO_PARAM, *PDVBT2_GETINFO_PARAM;


typedef struct DLL_PACKED _DVBT2_GETLIBVER_PARAM
{
	const MSIF_Version **ppVersion;
	MS_BOOL ret;
}DVBT2_GETLIBVER_PARAM, *PDVBT2_GETLIBVER_PARAM;


typedef struct DLL_PACKED _DVBT2_GETFWVER_PARAM
{
	MS_U16 *ver;
	MS_BOOL ret;
}DVBT2_GETFWVER_PARAM, *PDVBT2_GETFWVER_PARAM;

typedef struct DLL_PACKED _DVBT2_GETREG_PARAM
{
	MS_U16 u16Addr;
	MS_U8 *pu8Data;
	MS_BOOL ret;
}DVBT2_GETREG_PARAM, *PDVBT2_GETREG_PARAM;

typedef struct DLL_PACKED _DVBT2_SETREG_PARAM
{
	MS_U16 u16Addr;
	MS_U8 u8Data;
	MS_BOOL ret;
}DVBT2_SETREG_PARAM, *PDVBT2_SETREG_PARAM;

typedef struct DLL_PACKED _DVBT2_SETSERIALCONTROL_PARAM
{
	MS_BOOL bEnable;
	MS_BOOL ret;
}DVBT2_SETSERIALCONTROL_PARAM,*PDVBT2_SETSERIALCONTROL_PARAM;

typedef struct DLL_PACKED _DVBT2_SETCONFIG_PARAM
{
	DMD_DVBT2_RF_CHANNEL_BANDWIDTH BW;
	MS_BOOL bSerialTS;
       MS_U8 u8PlpID;
	MS_BOOL ret;
}DVBT2_SETCONFIG_PARAM,*PDVBT2_SETCONFIG_PARAM;

typedef struct DLL_PACKED _DVBT2_SETACTIVE_PARAM
{
	MS_BOOL bEnable;
	MS_BOOL ret;
}DVBT2_SETACTIVE_PARAM,*PDVBT2_SETACTIVE_PARAM;

typedef struct DLL_PACKED _DVBT2_GETLOCK_PARAM
{
	DMD_DVBT2_GETLOCK_TYPE eType;
	DMD_T2_LOCK_STATUS *eLockStatus;
	MS_BOOL ret;
}DVBT2_GETLOCK_PARAM, *PDVBT2_GETLOCK_PARAM;

#if 0
typedef struct DLL_PACKED _DVBT2_GETSIGNALSTRENGTHWITHRFPOWER_PARAM
{
	MS_U16 *u16Strength;
	float fRFPowerDbm	;
	MS_BOOL ret;
}DVBT2_GETSIGNALSTRENGTHWITHRFPOWER_PARAM,*PDVBT2_GETSIGNALSTRENGTHWITHRFPOWER_PARAM;

typedef struct DLL_PACKED _DVBT2_GETSIGNALQUALITYWITHRFPOWER_PARAM
{
	MS_U16 *u16Quality;
	float fRFPowerDbm;
	MS_BOOL ret;
}DVBT2_GETSIGNALQUALITYWITHRFPOWER_PARAM,*PDVBT2_GETSIGNALQUALITYWITHRFPOWER_PARAM;
#endif

#if 0
typedef struct DLL_PACKED _DVBT2_GETSNR_PARAM
{
	float *fSNR;
	MS_BOOL ret;
}DVBT2_GETSNR_PARAM,*PDVBT2_GETSNR_PARAM;
#else
typedef struct DLL_PACKED _DVBT2_GETSNR_PARAM
{
    MS_U16 *u16_snr100;
    MS_U8 *snr_cali;
    MS_U8 *u8_gi;
    MS_BOOL ret;
}DVBT2_GETSNR_PARAM,*PDVBT2_GETSNR_PARAM;
#endif

#if 0
typedef struct DLL_PACKED _DVBT2_GETPOSTLDPCBER_PARAM
{
	float *ber;
	MS_BOOL ret;
}DVBT2_GETPOSTLDPCBER_PARAM,*PDVBT2_GETPOSTLDPCBER_PARAM;
#else
typedef struct DLL_PACKED _DVBT2_GETPOSTLDPCBER_PARAM
{
    MS_U32 *BitErr_reg;
    MS_U16 *BitErrPeriod_reg;
    MS_U16 *FecType;
    MS_BOOL ret;
}DVBT2_GETPOSTLDPCBER_PARAM,*PDVBT2_GETPOSTLDPCBER_PARAM;
#endif

#if 0
typedef struct DLL_PACKED _DVBT2_GETPRELDPCBERPARAM
{
	float *ber;
	MS_BOOL ret;
}DVBT2_GETPRELDPCBERPARAM,*PDVBT2_GETPRELDPCBERPARAM;
#else
typedef struct DLL_PACKED _DVBT2_GETPRELDPCBERPARAM
{
    MS_U32 *BitErr_reg;
    MS_U16 *BitErrPeriod_reg;
    MS_U16 *FecType;
    MS_BOOL ret;
}DVBT2_GETPRELDPCBERPARAM,*PDVBT2_GETPRELDPCBERPARAM;
#endif

typedef struct DLL_PACKED _DVBT2_GETPACKETERRPARAM
{
	MS_U16 *pktErr;
	MS_BOOL ret;
}DVBT2_GETPACKETERRPARAM,*PDVBT2_GETPACKETERRPARAM;


typedef struct DLL_PACKED _DVBT2_GETL1INFO_PARAM
{
	MS_U16 *u16Info;
	DMD_DVBT2_SIGNAL_INFO eSignalType;
	MS_BOOL ret;
}DVBT2_GETL1INFO_PARAM,*PDVBT2_GETL1INFO_PARAM;

#if 0
typedef struct DLL_PACKED _DVBT2_GETFREQOFFSET_PARAM
{
	float *pFreqOff;
	MS_BOOL ret;
}DVBT2_GETFREQOFFSET_PARAM,*PDVBT2_GETFREQOFFSET_PARAM;
#else
typedef struct DLL_PACKED _DVBT2_GETFREQOFFSET_PARAM
{
    MS_U32 *CfoTd_reg;
    MS_U32 *CfoFd_reg;
    MS_U32 *Icfo_reg;
    MS_U8 *fft_reg;
    MS_BOOL ret;
}DVBT2_GETFREQOFFSET_PARAM,*PDVBT2_GETFREQOFFSET_PARAM;
#endif

#if 0
typedef struct DLL_PACKED _DVBT2_NORDIG_SSI_TABLE_WRITE_PARAM
{
	DMD_T2_CONSTEL constel;
	DMD_T2_CODERATE code_rate;
	float write_value;
	MS_BOOL ret;
}DVBT2_NORDIG_SSI_TABLE_WRITE_PARAM,*PDVBT2_NORDIG_SSI_TABLE_WRITE_PARAM;

typedef struct DLL_PACKED _DVBT2_NORDIG_SSI_TABLE_READ
{
	DMD_T2_CONSTEL constel;
	DMD_T2_CODERATE code_rate;
	float *read_value;
	MS_BOOL ret;
}DVBT2_NORDIG_SSI_TABLE_READ_PARAM,*PDVBT2_NORDIG_SSI_TABLE_READ;
#endif

typedef struct DLL_PACKED _DVBT2_SETPOWERSTATE_PARAM
{
	EN_POWER_MODE u16PowerState;
	MS_U32 ret;
}DVBT2_SETPOWERSTATE_PARAM,*PDVBT2_SETPOWERSTATE_PARAM;

typedef struct DLL_PACKED _DVBT2_GETPLPBITMAP_PARAM
{
	MS_U8* u8PlpBitMap;
	MS_BOOL ret;
}DVBT2_GETPLPBITMAP_PARAM,*PDVBT2_GETPLPBITMAP_PARAM;

typedef struct DLL_PACKED _DVBT2_GETPLPGROUPID_PARAM
{
	MS_U8 u8PlpID;
	MS_U8* u8GroupID;
	MS_BOOL ret;
}DVBT2_GETPLPGROUPID_PARAM,*PDVBT2_GETPLPGROUPID_PARAM;

typedef struct DLL_PACKED _DMD_DVBT2_SETPLPID_PARAM
{
	MS_U8 u8PlpID;
	MS_U8 u8GroupID;
	MS_BOOL ret;
}DMD_DVBT2_SETPLPID_PARAM,*PDMD_DVBT2_SETPLPID_PARAM;










/*************************************************/


MS_U32 DVBT2Open(void** pInstantTmp, MS_U32 u32ModuleVersion, void* pAttribute);
MS_U32 DVBT2Close(void* pInstantTmp);
MS_U32 DVBT2Ioctl(void* pInstantTmp, MS_U32 u32Cmd, void* pArgs);



#ifdef __cplusplus
}
#endif

#endif // _DRV_DVBT2_H_