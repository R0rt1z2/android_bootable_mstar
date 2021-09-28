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

#ifndef _DRV_DVBT2_H_
#define _DRV_DVBT2_H_

#include "MsTypes.h"

#include "MsCommon.h"
#include "drvDMD_common.h"
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
#define MSIF_DMD_DVBT2_INTERN_LIB_CODE           {'D','V', 'B','T'} //Lib code
#define MSIF_DMD_DVBT2_INTERN_LIBVER             {'0','1'}      //LIB version
#define MSIF_DMD_DVBT2_INTERN_BUILDNUM           {'2','1' }    //Build Number
#define MSIF_DMD_DVBT2_INTERN_CHANGELIST         {'0','0','0','0','0','0','0','0'} //P4 ChangeList Number

#define DMD_DVBT2_INTERN_VER                 /* Character String for DRV/API version             */  \
    MSIF_TAG,                           /* 'MSIF'                                           */  \
    MSIF_CLASS,                         /* '00'                                             */  \
    MSIF_CUS,                           /* 0x0000                                           */  \
    MSIF_MOD,                           /* 0x0000                                           */  \
    MSIF_CHIP,                                                                                  \
    MSIF_CPU,                                                                                   \
    MSIF_DMD_DVBT2_INTERN_LIB_CODE,      /* IP__                                             */  \
    MSIF_DMD_DVBT2_INTERN_LIBVER,        /* 0.0 ~ Z.Z                                        */  \
    MSIF_DMD_DVBT2_INTERN_BUILDNUM,      /* 00 ~ 99                                          */  \
    MSIF_DMD_DVBT2_INTERN_CHANGELIST,    /* CL#                                              */  \
    MSIF_OS

#define IS_BITS_SET(val, bits)                  (((val)&(bits)) == (bits))

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
typedef enum
{
    DMD_T2_DBGLV_NONE,    // disable all the debug message
    DMD_T2_DBGLV_INFO,    // information
    DMD_T2_DBGLV_NOTICE,  // normal but significant condition
    DMD_T2_DBGLV_WARNING, // warning conditions
    DMD_T2_DBGLV_ERR,     // error conditions
    DMD_T2_DBGLV_CRIT,    // critical conditions
    DMD_T2_DBGLV_ALERT,   // action must be taken immediately
    DMD_T2_DBGLV_EMERG,   // system is unusable
    DMD_T2_DBGLV_DEBUG,   // debug-level messages
} DMD_T2_DbgLv;

typedef enum
{
    E_DMD_T2_LOCK,
    E_DMD_T2_CHECKING,
    E_DMD_T2_CHECKEND,
    E_DMD_T2_UNLOCK,
    E_DMD_T2_NULL,
} DMD_T2_LOCK_STATUS;

typedef enum
{
    E_DMD_DVBT2_GETLOCK,
    E_DMD_DVBT2_FEC_LOCK,
    E_DMD_DVBT2_P1_LOCK,
    E_DMD_DVBT2_DCR_LOCK,
    E_DMD_DVBT2_AGC_LOCK,
    E_DMD_DVBT2_MODE_DET,
    E_DMD_DVBT2_P1_EVER_LOCK,
    E_DMD_DVBT2_L1_CRC_LOCK,
    E_DMD_DVBT2_NO_CHANNEL,
    E_DMD_DVBT2_NO_CHANNEL_IFAGC,
    E_DMD_DVBT2_ATV_DETECT,
    E_DMD_DVBT2_BER_LOCK,
    E_DMD_DVBT2_SNR_LOCK,
} DMD_DVBT2_GETLOCK_TYPE;

typedef enum
{
    E_DMD_T2_RF_BAND_5MHz = 0x01,
    E_DMD_T2_RF_BAND_1p7MHz = 0x00,
    E_DMD_T2_RF_BAND_6MHz = 0x02,
    E_DMD_T2_RF_BAND_7MHz = 0x03,
    E_DMD_T2_RF_BAND_8MHz = 0x04,
    E_DMD_T2_RF_BAND_10MHz = 0x05,
    E_DMD_T2_RF_BAND_INVALID
} DMD_DVBT2_RF_CHANNEL_BANDWIDTH;

typedef enum
{
    // fw version, check sum
    E_DMD_T2_CHECK_SUM_L      = 0x00,
    E_DMD_T2_CHECK_SUM_H,
    E_DMD_T2_FW_VER_0,
    E_DMD_T2_FW_VER_1,
    E_DMD_T2_FW_VER_2,

    // operation mode
    E_DMD_T2_ZIF_EN           = 0x20,
    E_DMD_T2_RF_AGC_EN,
    E_DMD_T2_HUM_DET_EN,
    E_DMD_T2_DCR_EN,
    E_DMD_T2_IQB_EN,
    E_DMD_T2_IIS_EN,
    E_DMD_T2_CCI_EN,
    E_DMD_T2_LOW_PWR_DET_EN,
    E_DMD_T2_ACI_DET_EN,
    E_DMD_T2_TD_MOTION_EN,
    E_DMD_T2_FD_MOTION_EN,

    // channel tuning param
    E_DMD_T2_BW               = 0x40,
    E_DMD_T2_FC_L             = 0x41,
    E_DMD_T2_FC_H             = 0x42,
    E_DMD_T2_FS_L,
    E_DMD_T2_FS_H,
    E_DMD_T2_ZIF,
    E_DMD_T2_GI,
    E_DMD_T2_ACI_DET_TYPE,
    E_DMD_T2_AGC_REF,         //0x48
    E_DMD_T2_RSSI_REF,
    E_DMD_T2_SNR_TIME_L,
    E_DMD_T2_SNR_TIME_H,
    E_DMD_T2_BER_CMP_TIME_L,
    E_DMD_T2_BER_CMP_TIME_H,
    E_DMD_T2_SFO_CFO_NUM,
    E_DMD_T2_CCI,
    E_DMD_T2_ACI_DET_TH_L,    //0x50
    E_DMD_T2_ACI_DET_TH_H,
    E_DMD_T2_TS_SERIAL     = 0x52,
    E_DMD_T2_TS_CLK_RATE   = 0x53,
    E_DMD_T2_TS_OUT_INV    = 0x54,
    E_DMD_T2_TS_DATA_SWAP  = 0x55,
    E_DMD_T2_TDP_CCI_KP,
    E_DMD_T2_CCI_FSWEEP,      //0x57
    E_DMD_T2_TS_ERR_POL,      //0x58
    E_DMD_T2_IF_AGC_INV_PWM_EN, // 0x59
    E_DMD_T2_CCI_TYPE,		       //0x5A
    E_DMD_T2_LITE,                       //0x5B

    E_DMD_T2_TOTAL_CFO_0      = 0x85,
    E_DMD_T2_TOTAL_CFO_1,

    // EQ of SDRAM arrangement
    E_T2EQ_START_ADDR_0 = 0x90,
    E_T2EQ_START_ADDR_1,
    E_T2EQ_START_ADDR_2,
    E_T2EQ_START_ADDR_3,

    // TDI of SDRAM arrangement
    E_T2TDI_START_ADDR_0,
    E_T2TDI_START_ADDR_1,
    E_T2TDI_START_ADDR_2,
    E_T2TDI_START_ADDR_3,

    // DJB of SDRAM arrangement
    E_T2DJB_START_ADDR_0,
    E_T2DJB_START_ADDR_1,
    E_T2DJB_START_ADDR_2,
    E_T2DJB_START_ADDR_3,

    // DJB of SDRAM arrangement
    E_T2FW_START_ADDR_0,
    E_T2FW_START_ADDR_1,
    E_T2FW_START_ADDR_2,
    E_T2FW_START_ADDR_3,

    // dvbt2 lock history
    E_DMD_T2_DVBT2_LOCK_HIS   = 0xF0,
    E_DMD_T2_FEF_DET_IND,
    E_DMD_T2_MPLP_NO_COMMON_IND,
    E_DMD_T2_SNR_L,             // 0xf3
    E_DMD_T2_SNR_H,             // 0xf4
    E_DMD_T2_DOPPLER_DET_FLAG,  // 0xf5
    E_DMD_T2_DOPPLER_DET_TH_L,  // 0xf6
    E_DMD_T2_DOPPLER_DET_TH_H,  // 0xf7

    // splp, mplp releted
    E_DMD_T2_PLP_ID_ARR       = 0x100,
    E_DMD_T2_L1_FLAG          = 0x120,
    E_DMD_T2_PLP_ID,
    E_DMD_T2_GROUP_ID,
    E_DMD_T2_CHANNEL_SWITCH,

    //dvbt2 auto ts data rate
    E_DMD_T2_TS_DATA_RATE_0       = 0x130,
    E_DMD_T2_TS_DATA_RATE_1       = 0x131,
    E_DMD_T2_TS_DATA_RATE_2       = 0x132,
    E_DMD_T2_TS_DATA_RATE_3       = 0x133,
    E_DMD_T2_TS_DATA_RATE_CHANGE_IND       = 0x134,
    E_DMD_T2_TS_DIV_172           = 0x135,
    E_DMD_T2_TS_DIV_288           = 0x136,
    E_DMD_T2_TS_DIV_432           = 0x137,
    E_DMD_T2_PARAM_NUM,
} DVBT2_PARAM;

typedef enum
{
    _T2_QPSK    = 0x0,
    _T2_16QAM   = 0x1,
    _T2_64QAM   = 0x2,
    _T2_256QAM    = 0x3,
    _T2_QAM_UNKNOWN     = 0xFF,
} DMD_T2_CONSTEL;

typedef enum
{
    _T2_CR1Y2   = 0x0,
    _T2_CR3Y5   = 0x1,
    _T2_CR2Y3   = 0x2,
    _T2_CR3Y4   = 0x3,
    _T2_CR4Y5   = 0x4,
    _T2_CR5Y6   = 0x5,
    _T2_CR_UNKNOWN     = 0xFF,
} DMD_T2_CODERATE;

typedef struct
{
    DMD_T2_CONSTEL         constel;
    DMD_T2_CODERATE        code_rate;
    float               p_ref;
}DMD_T2_SSI_DBM_NORDIGP1;

typedef struct
{
    DMD_T2_CONSTEL   constel;
    DMD_T2_CODERATE   code_rate;
    float   cn_ref;
}DMD_T2_SQI_CN_NORDIGP1;


typedef struct
{
    DMD_T2_CONSTEL   constel;
    DMD_T2_CODERATE   code_rate;
    MS_S16   cn_ref;  //X10 scalep
}DMD_T2_SQI_CN_NORDIGP1_KERNEL;



// For Interrupt mode
typedef enum
{
    E_DMD_DVBT2_INT_UNKNOWN=0,
    E_DMD_DVBT2_INT_LOCK,
    E_DMD_DVBT2_INT_UNLOCK
} DMD_DVBT2_INT_TYPE;

#ifdef UFO_DEMOD_DVBT2_SUPPORT_DMD_INT
typedef void (*fpIntCallBack)(MS_U8 u8arg);
#endif


/// For demod init
typedef struct
{
    // tuner parameter
    MS_U8 u8SarChannel;
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
    DMD_T2_SQI_CN_NORDIGP1 *pSqiCnNordigP1;
    MS_U16 u16SqiCnNordigP1_Size;

    // register init
    MS_U8 *u8DMD_DVBT2_DSPRegInitExt; // TODO use system variable type
    MS_U8 u8DMD_DVBT2_DSPRegInitSize;
    MS_U8 *u8DMD_DVBT2_InitExt; // TODO use system variable type
    MS_U32  u32EqStartAddr;
    MS_U32  u32TdiStartAddr;
    MS_U32  u32DjbStartAddr;
    MS_U32  u32FwStartAddr;
} DMD_DVBT2_InitData;



typedef struct
{
    // tuner parameter
    MS_U8 u8SarChannel;
    DMD_RFAGC_SSI_KERNEL *pTuner_RfagcSsi;
    MS_U16 u16Tuner_RfagcSsi_Size;
    DMD_IFAGC_SSI_KERNEL *pTuner_IfagcSsi_LoRef;
    MS_U16 u16Tuner_IfagcSsi_LoRef_Size;
    DMD_IFAGC_SSI_KERNEL *pTuner_IfagcSsi_HiRef;
    MS_U16 u16Tuner_IfagcSsi_HiRef_Size;
    DMD_IFAGC_ERR_KERNEL *pTuner_IfagcErr_LoRef;
    MS_U16 u16Tuner_IfagcErr_LoRef_Size;
    DMD_IFAGC_ERR_KERNEL *pTuner_IfagcErr_HiRef;
    MS_U16 u16Tuner_IfagcErr_HiRef_Size;
    DMD_T2_SQI_CN_NORDIGP1_KERNEL *pSqiCnNordigP1;
    MS_U16 u16SqiCnNordigP1_Size;

    // register init
    MS_U8 *u8DMD_DVBT2_DSPRegInitExt; // TODO use system variable type
    MS_U8 u8DMD_DVBT2_DSPRegInitSize;
    MS_U8 *u8DMD_DVBT2_InitExt; // TODO use system variable type
    MS_U32  u32EqStartAddr;
    MS_U32  u32TdiStartAddr;
    MS_U32  u32DjbStartAddr;
    MS_U32  u32FwStartAddr;
} DMD_DVBT2_InitData_KERNEL;


typedef enum
{
    E_DMD_DVBT2_FAIL=0,
    E_DMD_DVBT2_OK=1
} DMD_DVBT2_Result;


typedef enum
{
    E_DMD_DVBT2_MODULATION_INFO,
    E_DMD_DVBT2_DEMOD_INFO,
    E_DMD_DVBT2_LOCK_INFO,
    E_DMD_DVBT2_PRESFO_INFO,
    E_DMD_DVBT2_LOCK_TIME_INFO,
    E_DMD_DVBT2_BER_INFO,
    E_DMD_DVBT2_AGC_INFO,
} DMD_DVBT2_INFO_TYPE;

typedef struct
{
    MS_U16 u16Version;
    MS_U8 u16DemodState;	//
	float SfoValue; //
	float TotalCfo; //
    MS_U16 u16ChannelLength; //
	MS_U8 u8Fft; //
	MS_U8 u8Constel; //
	MS_U8 u8Gi; //
	MS_U8 u8HpCr; //
	MS_U8 u8LpCr; //
	MS_U8 u8Hiearchy; //
	MS_U8 u8Fd; //
	MS_U8 u8ChLen; //
	MS_U8 u8SnrSel;	//
	MS_U8 u8PertoneNum; //
	MS_U8 u8DigAci; //
	MS_U8 u8FlagCi;	//
	MS_U8 u8TdCoef;	//
} DMD_DVBT2_Info;

/********************************************************
*Constellation (b2 ~ b0)  : 0~3 => QPSK, 16QAM, 64QAM, 256QAM
*Code Rate (b5 ~ b3)   : 0~5 => 1/2, 3/5, 2/3, 3/4, 4/5, 5/6
*GI (b8 ~ b6)           : 0~6 => 1/32, 1/16, 1/8, 1/4, 1/128, 19/128, 19/256
*FFT (b11 ~ b9)        : 0~7 => 2K, 8K, 4K, 1K, 16K, 32K, 8KE, 32KE
*Preamble(b12)      : 0~1 => mixed, not_mixed
*S1_Signaling(b14~b13)   : 0~3 => t2_siso, t2_miso, "non_t2, reserved
*pilot_pattern(b18~b15)    : 0~8 => PP1, PP2, PP3, PP4, PP5, PP6, PP7, PP8
*BW_Extend(b19)             : 0~1 => normal, extension
*PAPR(b22~b20)              : 0~4 => none, ace, tr, tr_and_ace, reserved
*OFDM_SYMBOLS_PER_FRAME	: 4~2098
*PLP_ROTATION	: 0/1 => none, rotated constellation
*PLP_FEC_TYPE	: 0/1 => LDPC_16K, LDPC_64K
*NUM_PLP	: 1~255
*PLP_TYPE	: 0~2 => common, Type 1, Type 2
*PLP_TIME_IL_TYPE	: 0/1
*PLP_TIME_IL_LENGTH	: 0~3
*DAT_ISSY	: 0/1
*T2_PLP_MODE	: 0/1: Normal mode /High Efficiency Mode
*L1_MODULATION	: 0~3 => BPSK, QPSK, 16QAM, 64QAM
*T2_NUM_T2_FRAMES	: 2 ~ 255
*T2_PLP_NUM_BLOCKS_MAX :
*FEF_ENABLE	: 0/1
 ********************************/
typedef enum
{
    T2_MODUL_MODE,
    T2_FFT_VALUE,
    T2_GUARD_INTERVAL,
    T2_CODE_RATE,
    T2_PREAMBLE,
    T2_S1_SIGNALLING,
    T2_PILOT_PATTERN,
    T2_BW_EXT,
    T2_PAPR_REDUCTION,
    T2_OFDM_SYMBOLS_PER_FRAME,
    T2_PLP_ROTATION,
    T2_PLP_FEC_TYPE,
    T2_NUM_PLP,
	T2_PLP_TYPE,
	T2_PLP_TIME_IL_TYPE,
	T2_PLP_TIME_IL_LENGTH,
	T2_DAT_ISSY,
	T2_PLP_MODE,
	T2_L1_MODULATION,
	T2_NUM_T2_FRAMES,
	T2_PLP_NUM_BLOCKS_MAX,
	T2_FEF_ENABLE,
    T2_PARAM_MAX_NUM,
} DMD_DVBT2_SIGNAL_INFO;

//typedef void(*P_DMD_ISR_Proc)(MS_U8 u8DMDID);


//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBT2_Init
////////////////////////////////////////////////////////////////////////////////
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBT2_Init(DMD_DVBT2_InitData *pDMD_DVBT2_InitData, MS_U32 u32InitDataLen);
////////////////////////////////////////////////////////////////////////////////
/// Should be called when exit VD input source
////////////////////////////////////////////////////////////////////////////////
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBT2_Exit(void);
//------------------------------------------------------------------------------
/// Set detailed level of DVBT2 driver debug message
/// u8DbgLevel : debug level for Parallel Flash driver\n
/// AVD_DBGLV_NONE,    ///< disable all the debug message\n
/// AVD_DBGLV_INFO,    ///< information\n
/// AVD_DBGLV_NOTICE,  ///< normal but significant condition\n
/// AVD_DBGLV_WARNING, ///< warning conditions\n
/// AVD_DBGLV_ERR,     ///< error conditions\n
/// AVD_DBGLV_CRIT,    ///< critical conditions\n
/// AVD_DBGLV_ALERT,   ///< action must be taken immediately\n
/// AVD_DBGLV_EMERG,   ///< system is unusable\n
/// AVD_DBGLV_DEBUG,   ///< debug-level messages\n
/// @return TRUE : succeed
/// @return FALSE : failed to set the debug level
//------------------------------------------------------------------------------
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBT2_SetDbgLevel(DMD_T2_DbgLv u8DbgLevel);
//-------------------------------------------------------------------------------------------------
/// Get the information of DVBT2 driver\n
/// @return the pointer to the driver information
//-------------------------------------------------------------------------------------------------
extern DLL_PUBLIC DMD_DVBT2_Info* MDrv_DMD_DVBT2_GetInfo(DMD_DVBT2_INFO_TYPE eInfoType);
//-------------------------------------------------------------------------------------------------
/// Get DVBT2 driver version
/// when get ok, return the pointer to the driver version
//-------------------------------------------------------------------------------------------------
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBT2_GetLibVer(const MSIF_Version **ppVersion);
////////////////////////////////////////////////////////////////////////////////
/// To get DVBT2's register  value, only for special purpose.\n
/// u16Addr       : the address of DVBT2's register\n
/// return the value of AFEC's register\n
////////////////////////////////////////////////////////////////////////////////
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBT2_GetReg(MS_U16 u16Addr, MS_U8 *pu8Data);
////////////////////////////////////////////////////////////////////////////////
/// To set DVBT2's register value, only for special purpose.\n
/// u16Addr       : the address of DVBT2's register\n
/// u8Value        : the value to be set\n
////////////////////////////////////////////////////////////////////////////////
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBT2_SetReg(MS_U16 u16Addr, MS_U8 u8Data);
////////////////////////////////////////////////////////////////////////////////
/// Get DVBT2 FW version
/// u16Addr       : the address of DVBT's register\n
////////////////////////////////////////////////////////////////////////////////
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBT2_GetFWVer(MS_U16 *ver);

////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBT2_SetSerialControl
////////////////////////////////////////////////////////////////////////////////
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBT2_SetSerialControl(MS_BOOL bEnable);
////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBT2_SetReset
////////////////////////////////////////////////////////////////////////////////
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBT2_SetReset(void);
////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBT2_SetConfig
////////////////////////////////////////////////////////////////////////////////
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBT2_SetConfig(DMD_DVBT2_RF_CHANNEL_BANDWIDTH BW, MS_BOOL bSerialTS, MS_U8 u8PlpID);
///////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBT2_SetActive
////////////////////////////////////////////////////////////////////////////////
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBT2_SetActive(MS_BOOL bEnable);
////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBT2_Get_Lock
////////////////////////////////////////////////////////////////////////////////
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBT2_GetLock(DMD_DVBT2_GETLOCK_TYPE eType, DMD_T2_LOCK_STATUS *eLockStatus);
////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBT2_GetSignalStrength
////////////////////////////////////////////////////////////////////////////////
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBT2_GetSignalStrength(MS_U16 *u16Strength);
////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBT2_GetSignalStrengthWithRFPower
////////////////////////////////////////////////////////////////////////////////
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBT2_GetSignalStrengthWithRFPower(MS_U16 *u16Strength, float fRFPowerDbm);
////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBT2_GetSignalQuality
////////////////////////////////////////////////////////////////////////////////
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBT2_GetSignalQuality(MS_U16 *u16Quality);
////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBT2_GetSignalQualityWithRFPower
////////////////////////////////////////////////////////////////////////////////
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBT2_GetSignalQualityWithRFPower(MS_U16 *u16Quality, float fRFPowerDbm);
////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBT2_GetSNR
////////////////////////////////////////////////////////////////////////////////
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBT2_GetSNR(float *fSNR);
////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBT2_GetPostLDPCBer
////////////////////////////////////////////////////////////////////////////////
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBT2_GetPostLdpcBer(float *ber);
////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBT2_GetPreLDPCBer
////////////////////////////////////////////////////////////////////////////////
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBT2_GetPreLdpcBer(float *ber);
////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBT2_GetPacketErr
////////////////////////////////////////////////////////////////////////////////
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBT2_GetPacketErr(MS_U16 *pktErr);
////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBT2_GetL1Info
////////////////////////////////////////////////////////////////////////////////
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBT2_GetL1Info(MS_U16 *u16Info, DMD_DVBT2_SIGNAL_INFO eSignalType);
////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBT2_GetFreqOffset
////////////////////////////////////////////////////////////////////////////////
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBT2_GetFreqOffset(float *pFreqOff);
////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBT2_NORDIG_SSI_Table_Write
////////////////////////////////////////////////////////////////////////////////
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBT2_NORDIG_SSI_Table_Write(DMD_T2_CONSTEL constel, DMD_T2_CODERATE code_rate, float write_value);
////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBT2_NORDIG_SSI_Table_Read
////////////////////////////////////////////////////////////////////////////////
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBT2_NORDIG_SSI_Table_Read(DMD_T2_CONSTEL constel, DMD_T2_CODERATE code_rate, float *read_value);

extern DLL_PUBLIC MS_U32 MDrv_DMD_DVBT2_SetPowerState(EN_POWER_MODE u16PowerState);

extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBT2_GetPlpBitMap(MS_U8* u8PlpBitMap);

extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBT2_GetPlpGroupID(MS_U8 u8PlpID, MS_U8* u8GroupID);

extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBT2_SetPlpID(MS_U8 u8PlpID, MS_U8 u8GroupID);

//Kernel mode replaced functions

//extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBT2_Init(DMD_DVBT2_InitData *pDMD_DVBT2_InitData, MS_U32 u32InitDataLen);
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBT2_Init_KERNEL(DMD_DVBT2_InitData_KERNEL *pDMD_DVBT2_InitData, MS_U32 u32InitDataLen);
//extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBT2_GetSignalStrengthWithRFPower(MS_U16 *u16Strength, float fRFPowerDbm);
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBT2_GetSignalStrengthWithRFPower_KERNEL(MS_U16 *u16Strength, MS_FLOAT_ST msf_RFPowerDbm);
//extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBT2_GetSignalStrength(MS_U16 *u16Strength);
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBT2_GetSignalStrength_KERNEL(MS_U16 *u16Strength);
//extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBT2_GetSignalQuality(MS_U16 *u16Quality);
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBT2_GetSignalQuality_KERNEL(MS_U16 *u16Quality);
//extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBT2_GetSignalQualityWithRFPower(MS_U16 *u16Quality, float fRFPowerDbm);
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBT2_GetSignalQualityWithRFPower_KERNEL(MS_U16 *u16Quality, MS_FLOAT_ST msf_RFPowerDbm);
//extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBT2_GetSNR(float *fSNR);
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBT2_GetSNR_KERNEL(MS_FLOAT_ST *msf_SNR);
//extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBT2_GetPostLdpcBer(float *ber);
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBT2_GetPostLdpcBer_KERNEL(MS_FLOAT_ST *msf_ber);
//extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBT2_GetPreLdpcBer(float *ber);
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBT2_GetPreLdpcBer_KERNEL(MS_FLOAT_ST *msf_ber);
//extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBT2_GetFreqOffset(float *pFreqOff);
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBT2_GetFreqOffset_KERNEL(MS_FLOAT_ST *msf_FreqOff);


#ifdef UFO_DEMOD_DVBT2_SUPPORT_DMD_INT
MS_BOOL MDrv_DMD_DVBT2_Reg_INT_CB(fpIntCallBack fpCBReg);
#endif

#ifdef __cplusplus
}
#endif


#endif // _DRV_DVBT_H_

