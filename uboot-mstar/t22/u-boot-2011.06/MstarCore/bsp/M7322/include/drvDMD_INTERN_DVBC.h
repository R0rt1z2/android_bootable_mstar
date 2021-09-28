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

#ifndef _DRV_DVBC_H_
#define _DRV_DVBC_H_

#include "MsTypes.h"

#include "MsCommon.h"
#include "drvDMD_common.h" 
#include "UFO.h"
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
#define MSIF_DMD_DVBC_INTERN_LIB_CODE           {'D','V', 'B','C'} //Lib code
#define MSIF_DMD_DVBC_INTERN_LIBVER             {'0','1'}      //LIB version
#define MSIF_DMD_DVBC_INTERN_BUILDNUM           {'2','2' }    //Build Number
#define MSIF_DMD_DVBC_INTERN_CHANGELIST         {'0','0','0','0','0','0','0','0'} //P4 ChangeList Number
// oga	
#define	INDEX_C_TS_PHASE_EN			23
#define	INDEX_C_TS_PHASE_NUM		24

#define DMD_DVBC_INTERN_VER                 /* Character String for DRV/API version             */  \
    MSIF_TAG,                           /* 'MSIF'                                           */  \
    MSIF_CLASS,                         /* '00'                                             */  \
    MSIF_CUS,                           /* 0x0000                                           */  \
    MSIF_MOD,                           /* 0x0000                                           */  \
    MSIF_CHIP,                                                                                  \
    MSIF_CPU,                                                                                   \
    MSIF_DMD_DVBC_INTERN_LIB_CODE,      /* IP__                                             */  \
    MSIF_DMD_DVBC_INTERN_LIBVER,        /* 0.0 ~ Z.Z                                        */  \
    MSIF_DMD_DVBC_INTERN_BUILDNUM,      /* 00 ~ 99                                          */  \
    MSIF_DMD_DVBC_INTERN_CHANGELIST,    /* CL#                                              */  \
    MSIF_OS

#define IS_BITS_SET(val, bits)                  (((val)&(bits)) == (bits))

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
typedef enum
{
    DMD_DVBC_DBGLV_NONE,    // disable all the debug message
    DMD_DVBC_DBGLV_INFO,    // information
    DMD_DVBC_DBGLV_NOTICE,  // normal but significant condition
    DMD_DVBC_DBGLV_WARNING, // warning conditions
    DMD_DVBC_DBGLV_ERR,     // error conditions
    DMD_DVBC_DBGLV_CRIT,    // critical conditions
    DMD_DVBC_DBGLV_ALERT,   // action must be taken immediately
    DMD_DVBC_DBGLV_EMERG,   // system is unusable
    DMD_DVBC_DBGLV_DEBUG,   // debug-level messages
} DMD_DVBC_DbgLv;

typedef enum
{
    GET_INFO_TR_EVER_LOCK,
} DMD_DVBC_GET_INFO_FUNC;



typedef enum
{
    DMD_DVBC_LOCK,
    DMD_DVBC_CHECKING,
    DMD_DVBC_CHECKEND,
    DMD_DVBC_UNLOCK,
    DMD_DVBC_NULL,
} DMD_DVBC_LOCK_STATUS;

typedef enum
{
    DMD_DVBC_GETLOCK,
    DMD_DVBC_GETLOCK_FEC_LOCK,
    DMD_DVBC_GETLOCK_PSYNC_LOCK,
    DMD_DVBC_GETLOCK_TPS_LOCK,
    DMD_DVBC_GETLOCK_DCR_LOCK,
    DMD_DVBC_GETLOCK_AGC_LOCK,
    DMD_DVBC_GETLOCK_MODE_DET,
    DMD_DVBC_GETLOCK_NO_CHANNEL,
    DMD_DVBC_GETLOCK_ATV_DETECT,
    DMD_DVBC_GETLOCK_TR_LOCK,
    DMD_DVBC_GETLOCK_TR_EVER_LOCK,
//    DMD_DVBC_GETLOCK_FEC_STABLE_LOCK,
} DMD_DVBC_GETLOCK_TYPE;

typedef enum
{
    DMD_DVBC_QAM16 = 0,
    DMD_DVBC_QAM32 = 1,
    DMD_DVBC_QAM64 = 2,
    DMD_DVBC_QAM128 = 3,
    DMD_DVBC_QAM256 = 4,
    DMD_DVBC_QAMAUTO = 128,
} DMD_DVBC_MODULATION_TYPE;

typedef enum
{
    DMD_DVBC_RF_CH_BAND_6MHz = 0x01,
    DMD_DVBC_RF_CH_BAND_7MHz = 0x02,
    DMD_DVBC_RF_CH_BAND_8MHz = 0x03,
    DMD_DVBC_RF_CH_BAND_INVALID
} DMD_DVBC_RF_CHANNEL_BANDWIDTH;

typedef enum
{
    E_DMD_DVBC_PARAM_VERSION,           //0x00
    E_DMD_DVBC_OP_RFAGC_EN,
    E_DMD_DVBC_OP_HUMDET_EN,
    E_DMD_DVBC_OP_DCR_EN,
    E_DMD_DVBC_OP_IQB_EN,
    E_DMD_DVBC_OP_AUTO_IQ,
    E_DMD_DVBC_OP_AUTO_RFMAX,
    E_DMD_DVBC_OP_AUTO_ACI,
    E_DMD_DVBC_OP_AUTO_SCAN,
    E_DMD_DVBC_OP_AUTO_SCAN_SYM_RATE,
    E_DMD_DVBC_OP_AUTO_SCAN_QAM,
    E_DMD_DVBC_OP_ATV_DETECTOR_EN,
    E_DMD_DVBC_OP_ZIF_EN,                       //ZIF
    E_DMD_DVBC_OP_NORMALIF_EN,            //NIF
    E_DMD_DVBC_OP_LIF_EN,                       //LIF
    E_DMD_DVBC_OP_SAWLESS_EN,             //SAWLESS
    E_DMD_DVBC_IF_INV_PWM_OUT_EN,     //Sony Tuner , 0x10
    E_DMD_DVBC_CFG_RSSI,
    E_DMD_DVBC_CFG_ZIF,
    E_DMD_DVBC_CFG_FS_L,                        //FS
    E_DMD_DVBC_CFG_FS_H,                        //FS
    E_DMD_DVBC_CFG_FIF_L,                       //IF
    E_DMD_DVBC_CFG_FIF_H,                       //IF
    E_DMD_DVBC_CFG_FC_L,                //FC
    E_DMD_DVBC_CFG_FC_H,                //FC
    E_DMD_DVBC_CFG_BW0_L,
    E_DMD_DVBC_CFG_BW0_H,
    E_DMD_DVBC_CFG_BW1_L,
    E_DMD_DVBC_CFG_BW1_H,
    E_DMD_DVBC_CFG_BW2_L,
    E_DMD_DVBC_CFG_BW2_H,
    E_DMD_DVBC_CFG_BW3_L,
    E_DMD_DVBC_CFG_BW3_H,               //0x20
    E_DMD_DVBC_CFG_BW4_L,
    E_DMD_DVBC_CFG_BW4_H,
    E_DMD_DVBC_CFG_BW5_L,
    E_DMD_DVBC_CFG_BW5_H,
    E_DMD_DVBC_CFG_BW6_L,
    E_DMD_DVBC_CFG_BW6_H,
    E_DMD_DVBC_CFG_BW7_L,
    E_DMD_DVBC_CFG_BW7_H,
    E_DMD_DVBC_CFG_BW8_L,
    E_DMD_DVBC_CFG_BW8_H,
    E_DMD_DVBC_CFG_BW9_L,
    E_DMD_DVBC_CFG_BW9_H,
    E_DMD_DVBC_CFG_BW10_L,
    E_DMD_DVBC_CFG_BW10_H,
    E_DMD_DVBC_CFG_BW11_L,
    E_DMD_DVBC_CFG_BW11_H,              //0x30
    E_DMD_DVBC_CFG_RFMAX,
    E_DMD_DVBC_CFG_QAM,
    E_DMD_DVBC_CFG_IQ_SWAP,
    E_DMD_DVBC_CFG_CCI,
    E_DMD_DVBC_CFG_TS_SERIAL,
    E_DMD_DVBC_CFG_TS_CLK_RATE,
    E_DMD_DVBC_CFG_TS_CLK_INV,
    E_DMD_DVBC_CFG_TS_DATA_SWAP,
    E_DMD_DVBC_CFG_TS_EN,
    E_DMD_DVBC_AGC_REF_L,
    E_DMD_DVBC_AGC_REF_H,
    E_DMD_DVBC_AGC_K,
    E_DMD_DVBC_AGC_LOCK_TH,
    E_DMD_DVBC_AGC_LOCK_NUM,
    E_DMD_DVBC_ADC_PGA_GAIN_I,
    E_DMD_DVBC_ADC_PGA_GAIN_Q,                  //0x40
    E_DMD_DVBC_ADC_PWDN_I,
    E_DMD_DVBC_ADC_PWDN_Q,
    E_DMD_DVBC_ADC_MPLL_DIV_SEL,
    E_DMD_DVBC_CCI_BYPASS,
    E_DMD_DVBC_CCI_LOCK_DET,
    E_DMD_DVBC_CCI_FREQN_OUT_0,
    E_DMD_DVBC_CCI_FREQN_OUT_1,
    E_DMD_DVBC_RF_GAIN_MIN,
    E_DMD_DVBC_RF_GAIN_MAX,
    E_DMD_DVBC_IF_GAIN_MIN,
    E_DMD_DVBC_IF_GAIN_MAX,
    E_DMD_DVBC_NO_SIGNAL_NUM_TH,
    E_DMD_DVBC_NO_SIGNAL_GAIN_TH_L,
    E_DMD_DVBC_NO_SIGNAL_GAIN_TH_H,
    E_DMD_DVBC_NO_SIGNAL_ERR_TH_L,
    E_DMD_DVBC_NO_SIGNAL_ERR_TH_H,          //0x50
    E_DMD_DVBC_TUNER_NUM,
    E_DMD_DVBC_UNCRT_PKT_NUM_7_0,
    E_DMD_DVBC_UNCRT_PKT_NUM_8_15,
    E_DMD_DVBC_STATE,
    E_DMD_DVBC_ILL_LOCK,
    E_DMD_DVBC_DAGC1_REF,
    E_DMD_DVBC_DAGC2_REF,
    E_DMD_DVBC_EQ_KP3_16QAM,
    E_DMD_DVBC_EQ_KP3_32QAM,
    E_DMD_DVBC_EQ_KP3_64QAM,
    E_DMD_DVBC_EQ_KP3_128QAM,
    E_DMD_DVBC_EQ_KP3_256QAM,
    E_DMD_DVBC_EQ_KP4_16QAM,
    E_DMD_DVBC_EQ_KP4_32QAM,
    E_DMD_DVBC_EQ_KP4_64QAM,
    E_DMD_DVBC_EQ_KP4_128QAM,                   //0x60
    E_DMD_DVBC_EQ_KP4_256QAM,
    E_DMD_DVBC_SNR100_L,
    E_DMD_DVBC_SNR100_H,
    E_DMD_DVBC_CFO10_L,
    E_DMD_DVBC_CFO10_H,
    E_DMD_DVBC_TR_LOCK,
    E_DMD_DVBC_CR_LOCK,
    E_DMD_DVBC_EQ_DMA1_LOCK,
    E_DMD_DVBC_EQ_DMA_LOCK,
    E_DMD_DVBC_EQ_DD1_LOCK,
    E_DMD_DVBC_EQ_DD_LOCK,
    E_DMD_DVBC_FEC_LOCK,                //0x6C
    E_DMD_DVBC_CHIP_VERSION,
} DVBC_Param_2;

typedef enum
{
    // Operation Mode Settings
    e_opmode_rfagc_en, //0
    e_opmode_humdet_en,
    e_opmode_dcr_en,
    e_opmode_iqb_en,
    e_opmode_auto_iq,
    e_opmode_auto_rfmax,
    e_opmode_auto_aci,
    e_opmode_auto_scan,
    e_opmode_auto_scan_sym_rate,
    e_opmode_auto_scan_qam,
    e_opmode_atv_detector_en,
    e_opmode_no_sig_if_gain_th_l,
    e_opmode_no_sig_if_gain_th_h,
    e_opmode_no_sig_if_err_th_l,
    e_opmode_no_sig_if_err_th_h,
    e_opmode_rsv_0x0F, //15
    // Config Params
    e_config_rssi,
    e_config_zif,
    e_config_freq,
    e_config_fc_l,
    e_config_fc_h,//20
    e_config_fs_l,
    e_config_fs_h,
    e_config_bw_l,
    e_config_bw_h,
    e_config_bw1_l,//25
    e_config_bw1_h,
    e_config_bw2_l,
    e_config_bw2_h,
    e_config_bw3_l,
    e_config_bw3_h,//30
    e_config_rsv_0x1F,
    e_config_rfmax,
    e_config_qam,
    e_config_iq_swap,
    e_config_cci,
    e_config_ts_serial,
    e_config_ts_clk_rate,
    e_config_ts_out_inv,
    e_config_ts_data_swap,
    e_config_lif,
    e_config_fif_l,
    e_config_fif_h,
    e_config_tuner_sawless,
    DVBC_PARAM_LEN,
} DVBC_Param;

#ifdef UFO_DEMOD_DVBC_SUPPORT_DMD_INT
/// For demod init
typedef enum
{
    E_DMD_DVBC_INT_UNKNOWN=0,
    E_DMD_DVBC_INT_LOCK,
    E_DMD_DVBC_INT_UNLOCK,
    E_DMD_DVBC_INT_CHECKING
} DMD_DVBC_INT_TYPE;

typedef void (*fpIntCallBack)(MS_U8 u8arg);
#endif

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
    DMD_SQI_CN_NORDIGP1 *pSqiCnNordigP1;
    MS_U16 u16SqiCnNordigP1_Size;

    // register init
    MS_U8 *u8DMD_DVBC_DSPRegInitExt; // TODO use system variable type
    MS_U8 u8DMD_DVBC_DSPRegInitSize;
    MS_U8 *u8DMD_DVBC_InitExt; // TODO use system variable type

    //Interrupt callback
    //fpIntCallBack fpCB;
} DMD_DVBC_InitData;


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
    DMD_SQI_CN_NORDIGP1_KERNEL *pSqiCnNordigP1;
    MS_U16 u16SqiCnNordigP1_Size;

    // register init
    MS_U8 *u8DMD_DVBC_DSPRegInitExt; // TODO use system variable type
    MS_U8 u8DMD_DVBC_DSPRegInitSize;
    MS_U8 *u8DMD_DVBC_InitExt; // TODO use system variable type
} DMD_DVBC_InitData_KERNEL;


typedef enum
{
    E_DMD_DVBC_FAIL=0,
    E_DMD_DVBC_OK=1
} DMD_DVBC_Result;


typedef struct
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
    MS_U32 u32Intp; //	
    MS_U32 u32FcFs; //	
    MS_U8 u8Qam; //
    MS_U16 u16SymbolRateHal; //		
} DMD_DVBC_Info;

//typedef void(*P_DMD_ISR_Proc)(MS_U8 u8DMDID);


//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBT_Init
////////////////////////////////////////////////////////////////////////////////
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBC_Init(DMD_DVBC_InitData *pDMD_DVBC_InitData, MS_U32 u32InitDataLen);
////////////////////////////////////////////////////////////////////////////////
/// Should be called when exit VD input source
////////////////////////////////////////////////////////////////////////////////
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBC_Exit(void);
////////////////////////////////////////////////////////////////////////////////
/// Get Initial Data
/// @ingroup DVBC_BASIC
/// @param pDMD_DVBC_InitData    \b IN: DVBC initial parameters 
/// @return TRUE : succeed 
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
extern DLL_PUBLIC MS_U32 SYMBOL_WEAK MDrv_DMD_DVBC_GetConfig(DMD_DVBC_InitData *pDMD_DVBC_InitData);  
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/// Set detailed level of DVBT driver debug message
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
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBC_SetDbgLevel(DMD_DVBC_DbgLv u8DbgLevel);
//-------------------------------------------------------------------------------------------------
/// Get the information of DVBT driver\n
/// @return the pointer to the driver information
//-------------------------------------------------------------------------------------------------
extern DLL_PUBLIC const DMD_DVBC_Info* MDrv_DMD_DVBC_GetInfo(void);
//-------------------------------------------------------------------------------------------------
/// Get DVBT driver version
/// when get ok, return the pointer to the driver version
//-------------------------------------------------------------------------------------------------

///////////////////////////////////////////////////////////////////////////////////
///To get/set the DSP parameter table from demod MCU
//u16Addr     :the address of the demod MCU DSP parameter table
//////////////////////////////////////////////////////////////////////////////////
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBC_GetDSPReg(MS_U16 u16Addr, MS_U8 *pu8Data);
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBC_SetDSPReg(MS_U16 u16Addr, MS_U8 pu8Data);

extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBC_GetLibVer(const MSIF_Version **ppVersion);
////////////////////////////////////////////////////////////////////////////////
/// To get DVBT's register  value, only for special purpose.\n
/// u16Addr       : the address of DVBT's register\n
/// return the value of AFEC's register\n
////////////////////////////////////////////////////////////////////////////////
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBC_GetReg(MS_U16 u16Addr, MS_U8 *pu8Data);
////////////////////////////////////////////////////////////////////////////////
/// To set DVBT's register value, only for special purpose.\n
/// u16Addr       : the address of DVBT's register\n
/// u8Value        : the value to be set\n
////////////////////////////////////////////////////////////////////////////////
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBC_GetFWVer(MS_U16 *ver);
////////////////////////////////////////////////////////////////////////////////
/// Get DVBC FW version
/// u16Addr       : the address of DVBC's register\n
////////////////////////////////////////////////////////////////////////////////
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBC_SetReg(MS_U16 u16Addr, MS_U8 u8Data);
////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBT_SetSerialControl
////////////////////////////////////////////////////////////////////////////////
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBC_SetSerialControl(MS_BOOL bEnable);
////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBT_SetConfig
////////////////////////////////////////////////////////////////////////////////
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBC_SetConfig(MS_U16 u16SymbolRate, DMD_DVBC_MODULATION_TYPE eQamMode, MS_U32 u32IFFreq, MS_BOOL bSpecInv, MS_BOOL bSerialTS);
////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBT_SetConfig_symbol_rate_list
////////////////////////////////////////////////////////////////////////////////
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBC_SetConfig_symbol_rate_list(MS_U16 u16SymbolRate, DMD_DVBC_MODULATION_TYPE eQamMode, MS_U32 u32IFFreq, MS_BOOL bSpecInv, MS_BOOL bSerialTS, MS_U16 *pu16_symbol_rate_list,MS_U8 u8_symbol_rate_list_num);
////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBT_SetActive
////////////////////////////////////////////////////////////////////////////////
extern  DLL_PUBLIC MS_BOOL MDrv_DMD_DVBC_SetActive(MS_BOOL bEnable);


#ifdef UFO_DEMOD_DVBC_SUPPORT_DMD_INT
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBC_Get_Info_Generalization(DMD_DVBC_GET_INFO_FUNC InfoType, void *pin, void *pout);
#endif

////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBT_Get_Lock
////////////////////////////////////////////////////////////////////////////////
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBC_GetLock(DMD_DVBC_GETLOCK_TYPE eType, DMD_DVBC_LOCK_STATUS *eLockStatus);
////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBC_GetLockWithRFPower
////////////////////////////////////////////////////////////////////////////////
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBC_GetLockWithRFPower(DMD_DVBC_GETLOCK_TYPE eType, DMD_DVBC_LOCK_STATUS *eLockStatus, float fCurrRFPowerDbm, float fNoChannelRFPowerDbm);
////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBT_GetSignalStrength
////////////////////////////////////////////////////////////////////////////////
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBC_GetSignalStrength(MS_U16 *u16Strength);
////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBC_GetSignalStrengthWithRFPower
////////////////////////////////////////////////////////////////////////////////
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBC_GetSignalStrengthWithRFPower(MS_U16 *u16Strength, float fRFPowerDbm);
////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBC_GetSignalQuality
////////////////////////////////////////////////////////////////////////////////
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBC_GetSignalQuality(MS_U16 *u16Quality);
////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBC_GetSignalQualityWithRFPower
////////////////////////////////////////////////////////////////////////////////
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBC_GetSignalQualityWithRFPower(MS_U16 *u16Quality, float fRFPowerDbm);
////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBC_GetSNR
////////////////////////////////////////////////////////////////////////////////
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBC_GetSNR(float *fSNR);
////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBC_GetPostViterbiBer
////////////////////////////////////////////////////////////////////////////////
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBC_GetPostViterbiBer(float *ber);
////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBC_GetPacketErr
////////////////////////////////////////////////////////////////////////////////
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBC_GetPacketErr(MS_U16 *pktErr);
////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBT_GetCellID
////////////////////////////////////////////////////////////////////////////////
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBC_GetCellID(MS_U16 *u16CellID);
////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBC_GetStatus
////////////////////////////////////////////////////////////////////////////////
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBC_GetStatus(DMD_DVBC_MODULATION_TYPE *pQAMMode, MS_U16 *u16SymbolRate, float *pFreqOff);

extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBC_ActiveDmdSwitch(MS_U8 demod_no);

extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBC_Dual_Individual_Init(DMD_DVBC_InitData *pDMD_DVBC_InitData, MS_U32 u32InitDataLen);
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBC_Dual_Public_Init(MS_U8 u8AGC_Tristate_Ctrl,MS_U8 u8Sar_Channel);

extern DLL_PUBLIC MS_U32 MDrv_DMD_DVBC_SetPowerState(EN_POWER_MODE u16PowerState);

//Kernel mode replaced functions

//extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBC_GetPostViterbiBer(float *ber);
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBC_GetPostViterbiBer_KERNEL(MS_FLOAT_ST *msf_ber);
//extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBC_GetSNR(float *fSNR);
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBC_GetSNR_KERNEL(MS_FLOAT_ST *msf_SNR);
//extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBC_GetStatus(DMD_DVBC_MODULATION_TYPE *pQAMMode, MS_U16 *u16SymbolRate, float *pFreqOff);
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBC_GetStatus_KERNEL(DMD_DVBC_MODULATION_TYPE *pQAMMode, MS_U16 *u16SymbolRate, MS_FLOAT_ST *msf_pFreqOff);
//extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBC_GetSignalStrength(MS_U16 *u16Strength);
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBC_GetSignalStrength_KERNEL(MS_U16 *u16Strength);
//extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBC_GetSignalStrengthWithRFPower(MS_U16 *u16Strength, float fRFPowerDbm);
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBC_GetSignalStrengthWithRFPower_KERNEL(MS_U16 *u16Strength, MS_FLOAT_ST msf_RFPowerDbm);
//extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBC_GetSignalQuality(MS_U16 *u16Quality);
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBC_GetSignalQuality_KERNEL(MS_U16 *u16Quality);
//extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBC_Init(DMD_DVBC_InitData *pDMD_DVBC_InitData, MS_U32 u32InitDataLen);
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBC_Init_KERNEL(DMD_DVBC_InitData_KERNEL *pDMD_DVBC_InitData, MS_U32 u32InitDataLen);

#ifdef UFO_DEMOD_DVBC_SUPPORT_DMD_INT
MS_BOOL MDrv_DMD_DVBC_Reg_INT_CB(fpIntCallBack fpCBReg);
void MDrv_DMD_DVBC_Timer_CB(MS_U32 u32StTimer, MS_U32 u32TimerID);
#endif

#ifdef UFO_DEMOD_DVBC_GET_AGC_INFO
MS_BOOL MDrv_DMD_DVBC_GetAGCInfo(MS_U8 u8dbg_mode, MS_U16 *pu16Data);
#endif

#ifdef CONFIG_UTOPIA_PROC_DBG_SUPPORT // arthur add
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBC_GetDEMODMdbInfo(MS_U64 *u64ReqHdl);
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBC_SetMdb(MS_U64* pu64ReqHdl, MS_U32 u32CmdSize, char* pcmd);
#endif

#ifdef __cplusplus
}
#endif


#endif // _DRV_DVBC_H_

