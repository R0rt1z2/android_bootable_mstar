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

#ifndef _DRV_DVBT_H_
#define _DRV_DVBT_H_

#include "MsCommon.h"
#include "drvDMD_common.h"
#include "MsOS.h"
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
#define MSIF_DMD_DVBT_INTERN_LIB_CODE           {'D','V', 'B','T'} //Lib code
#define MSIF_DMD_DVBT_INTERN_LIBVER             {'0','1'}      //LIB version
#define MSIF_DMD_DVBT_INTERN_BUILDNUM           {'2','1' }    //Build Number
#define MSIF_DMD_DVBT_INTERN_CHANGELIST         {'0','0','0','0','0','0','0','0'} //P4 ChangeList Number

#define DMD_DVBT_INTERN_VER                 /* Character String for DRV/API version             */  \
    MSIF_TAG,                           /* 'MSIF'                                           */  \
    MSIF_CLASS,                         /* '00'                                             */  \
    MSIF_CUS,                           /* 0x0000                                           */  \
    MSIF_MOD,                           /* 0x0000                                           */  \
    MSIF_CHIP,                                                                                  \
    MSIF_CPU,                                                                                   \
    MSIF_DMD_DVBT_INTERN_LIB_CODE,      /* IP__                                             */  \
    MSIF_DMD_DVBT_INTERN_LIBVER,        /* 0.0 ~ Z.Z                                        */  \
    MSIF_DMD_DVBT_INTERN_BUILDNUM,      /* 00 ~ 99                                          */  \
    MSIF_DMD_DVBT_INTERN_CHANGELIST,    /* CL#                                              */  \
    MSIF_OS

#define IS_BITS_SET(val, bits)                  (((val)&(bits)) == (bits))

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
#ifdef UFO_DEMOD_DVBT_T2_MERGE_FW_DETECT_FLAG
typedef enum
{
  T_DVBT2_NOCHAN_Flag = 0x3B1,
  T_DVBT_NOCHAN_Flag = 0x3B2,
  T_DETECT_DONE_FLAG = 0x3B3,
} DVT_FLAG;
#else
typedef enum
{
  T_DVBT2_NOCHAN_Flag = 0xF1,
  T_DVBT_NOCHAN_Flag = 0xF2,
  T_DETECT_DONE_FLAG = 0xF3,
} DVT_FLAG;
#endif

typedef enum
{
    DMD_DBGLV_NONE,    // disable all the debug message
    DMD_DBGLV_INFO,    // information
    DMD_DBGLV_NOTICE,  // normal but significant condition
    DMD_DBGLV_WARNING, // warning conditions
    DMD_DBGLV_ERR,     // error conditions
    DMD_DBGLV_CRIT,    // critical conditions
    DMD_DBGLV_ALERT,   // action must be taken immediately
    DMD_DBGLV_EMERG,   // system is unusable
    DMD_DBGLV_DEBUG,   // debug-level messages
} DMD_DbgLv;

typedef enum
{
    E_DMD_LOCK,
    E_DMD_CHECKING,
    E_DMD_CHECKEND,
    E_DMD_UNLOCK,
    E_DMD_NULL,
} DMD_LOCK_STATUS;

typedef enum
{
    E_DMD_DMD_DVBT_GETLOCK,
    E_DMD_COFDM_FEC_LOCK,
    E_DMD_COFDM_PSYNC_LOCK,
    E_DMD_COFDM_TPS_LOCK,
    E_DMD_COFDM_DCR_LOCK,
    E_DMD_COFDM_AGC_LOCK,
    E_DMD_COFDM_MODE_DET,
    E_DMD_COFDM_TPS_EVER_LOCK,
    E_DMD_COFDM_NO_CHANNEL,
    E_DMD_COFDM_NO_CHANNEL_IFAGC,
    E_DMD_COFDM_ATV_DETECT,
    E_DMD_COFDM_BER_LOCK,
    E_DMD_COFDM_SNR_LOCK,
    E_DMD_COFDM_TR_LOCK,
} DMD_DVBT_GETLOCK_TYPE;

typedef enum
{
    E_DMD_RF_CH_BAND_6MHz = 0x01,
    E_DMD_RF_CH_BAND_7MHz = 0x02,
    E_DMD_RF_CH_BAND_8MHz = 0x03,
    E_DMD_RF_CH_BAND_INVALID
} DMD_RF_CHANNEL_BANDWIDTH;

typedef enum
{
    E_DMD_DVBT_N_PARAM_VERSION = 0x00,    //0x00
    E_DMD_DVBT_N_OP_RFAGC_EN,
    E_DMD_DVBT_N_OP_HUMDET_EN,
    E_DMD_DVBT_N_OP_DCR_EN,
    E_DMD_DVBT_N_OP_IIS_EN,
    E_DMD_DVBT_N_OP_CCI_EN,
    E_DMD_DVBT_N_OP_ACI_EN,
    E_DMD_DVBT_N_OP_IQB_EN,
    E_DMD_DVBT_N_OP_AUTO_IQ_SWAP_EN,      //0x08
    E_DMD_DVBT_N_OP_AUTO_RF_MAX_EN,
    E_DMD_DVBT_N_OP_FORCE_ACI_EN,
    E_DMD_DVBT_N_OP_FIX_MODE_CP_EN,
    E_DMD_DVBT_N_OP_FIX_TPS_EN,
    E_DMD_DVBT_N_OP_AUTO_SCAN_MODE_EN,
    E_DMD_DVBT_N_CFG_RSSI,
    E_DMD_DVBT_N_CFG_ZIF,                 //0x0F

    E_DMD_DVBT_N_CFG_NIF,                 //0x10
    E_DMD_DVBT_N_CFG_LIF,
    E_DMD_DVBT_N_CFG_SAWLESS,
    E_DMD_DVBT_N_CFG_FS_L,
    E_DMD_DVBT_N_CFG_FS_H,
    E_DMD_DVBT_N_CFG_FIF_L,
    E_DMD_DVBT_N_CFG_FIF_H,
    E_DMD_DVBT_N_CFG_FC_L,
    E_DMD_DVBT_N_CFG_FC_H,                //0x18
    E_DMD_DVBT_N_CFG_BW,
    E_DMD_DVBT_N_CFG_MODE,
    E_DMD_DVBT_N_CFG_CP,
    E_DMD_DVBT_N_CFG_LP_SEL,
    E_DMD_DVBT_N_CFG_CSTL,
    E_DMD_DVBT_N_CFG_HIER,
    E_DMD_DVBT_N_CFG_HPCR,                //0x1F

    E_DMD_DVBT_N_CFG_LPCR,                //0x20
    E_DMD_DVBT_N_CFG_IQ_SWAP,
    E_DMD_DVBT_N_CFG_RFMAX,
    E_DMD_DVBT_N_CFG_CCI,
    E_DMD_DVBT_N_CFG_ICFO_RANGE,
    E_DMD_DVBT_N_CFG_RFAGC_REF,
    E_DMD_DVBT_N_CFG_IFAGC_REF_2K,
    E_DMD_DVBT_N_CFG_IFAGC_REF_8K,
    E_DMD_DVBT_N_CFG_IFAGC_REF_ACI,       //0x28
    E_DMD_DVBT_N_CFG_IFAGC_REF_IIS,
    E_DMD_DVBT_N_CFG_IFAGC_REF_2K_H,
    E_DMD_DVBT_N_CFG_IFAGC_REF_8K_H,
    E_DMD_DVBT_N_CFG_IFAGC_REF_ACI_H,
    E_DMD_DVBT_N_CFG_IFAGC_REF_IIS_H,
    E_DMD_DVBT_N_CFG_TS_SERIAL,
    E_DMD_DVBT_N_CFG_TS_CLK_INV,          //0x2F

    E_DMD_DVBT_N_CFG_TS_DATA_SWAP,        //0x30
    E_DMD_DVBT_N_CFG_8M_DACI_DET_TH_L,
    E_DMD_DVBT_N_CFG_8M_DACI_DET_TH_H,
    E_DMD_DVBT_N_CFG_8M_ANM1_DET_TH_L,
    E_DMD_DVBT_N_CFG_8M_ANM1_DET_TH_H,
    E_DMD_DVBT_N_CFG_8M_ANP1_DET_TH_L,
    E_DMD_DVBT_N_CFG_8M_ANP1_DET_TH_H,
    E_DMD_DVBT_N_CFG_7M_DACI_DET_TH_L,
    E_DMD_DVBT_N_CFG_7M_DACI_DET_TH_H,    //0x38
    E_DMD_DVBT_N_CFG_7M_ANM1_DET_TH_L,
    E_DMD_DVBT_N_CFG_7M_ANM1_DET_TH_H,
    E_DMD_DVBT_N_CFG_7M_ANP1_DET_TH_L,
    E_DMD_DVBT_N_CFG_7M_ANP1_DET_TH_H,

    E_DMD_DVBT_N_CFO10K_L,
    E_DMD_DVBT_N_CFO10K_H,
    E_DMD_DVBT_N_SNR100_L,           //0x3F
    E_DMD_DVBT_N_SNR100_H,           //0x40

    E_DMD_DVBT_N_AGC_LOCK_TH,
    E_DMD_DVBT_N_AGC_LOCK_NUM,
    E_DMD_DVBT_N_ADC_PGA_GAIN_I,
    E_DMD_DVBT_N_ADC_PGA_GAIN_Q,
    E_DMD_DVBT_N_PWDN_ADCI,
    E_DMD_DVBT_N_PWDN_ADCQ,
    E_DMD_DVBT_N_MPLL_ADC_DIV_SEL,
    E_DMD_DVBT_N_DCR_LOCK,                //0x48
    E_DMD_DVBT_N_MIXER_IQ_SWAP_MODE,
    E_DMD_DVBT_N_CCI_BYPASS,
    E_DMD_DVBT_N_CCI_LOCK_DET,
    E_DMD_DVBT_N_CCI_FSWEEP_L,
    E_DMD_DVBT_N_CCI_FSWEEP_H,
    E_DMD_DVBT_N_CCI_KPKI,
    E_DMD_DVBT_N_INTP_RATEM1_0,           //0x4F

    E_DMD_DVBT_N_INTP_RATEM1_1,           //0x50
    E_DMD_DVBT_N_INTP_RATEM1_2,
    E_DMD_DVBT_N_INTP_RATEM1_3,
    E_DMD_DVBT_N_8K_MC_MODE,
    E_DMD_DVBT_N_8K_MC_CP,
    E_DMD_DVBT_N_8K_MC_CPOBS_NUM,
    E_DMD_DVBT_N_8K_MODECP_DET,
    E_DMD_DVBT_N_2K_MC_MODE,
    E_DMD_DVBT_N_2K_MC_CP,                //0x58
    E_DMD_DVBT_N_2K_MC_CPOBS_NUM,
    E_DMD_DVBT_N_2K_MODECP_DET,
    E_DMD_DVBT_N_ICFO_SCAN_WINDOW_L,
    E_DMD_DVBT_N_ICFO_SCAN_WINDOW_H,
    E_DMD_DVBT_N_ICFO_MAX_OFFSET_L,
    E_DMD_DVBT_N_ICFO_MAX_OFFSET_H,
    E_DMD_DVBT_N_ICFO_DONE,               //0x5F

    E_DMD_DVBT_N_TPS_SYNC_LOCK,           //0x60
    E_DMD_DVBT_N_CONSTELLATION,
    E_DMD_DVBT_N_HIERARCHY,
    E_DMD_DVBT_N_HP_CODE_RATE,
    E_DMD_DVBT_N_LP_CODE_RATE,
    E_DMD_DVBT_N_GUARD_INTERVAL,
    E_DMD_DVBT_N_TRANSMISSION_MODE,
    E_DMD_DVBT_N_OFDM_SYMBOL_NUMBER,
    E_DMD_DVBT_N_LENGTH_INDICATOR,        //0x68
    E_DMD_DVBT_N_FRAME_NUMBER,
    E_DMD_DVBT_N_CELL_IDENTIFIER,
    E_DMD_DVBT_N_DVBH_SIGNALLING,
    E_DMD_DVBT_N_SNR_2K_ALPHA,
    E_DMD_DVBT_N_SNR_8K_ALPHA,
    E_DMD_DVBT_N_TS_EN,
    E_DMD_DVBT_N_2K_DAGC1_REF,            //0x6F

    E_DMD_DVBT_N_8K_DAGC1_REF,            //0x70
    E_DMD_DVBT_N_2K_8K_DAGC2_REF,
    E_DMD_DVBT_N_IF_INV_PWM_OUT_EN,
    E_DMD_DVBT_N_RESERVE_0,
    E_DMD_DVBT_N_RESERVE_1,
    E_DMD_DVBT_N_RESERVE_2,
    E_DMD_DVBT_N_RESERVE_3,
    E_DMD_DVBT_N_RESERVE_4,
    E_DMD_DVBT_N_RESERVE_5,               //0x78
    E_DMD_DVBT_N_RESERVE_6,
    E_DMD_DVBT_N_RESERVE_7,
    E_DMD_DVBT_N_RESERVE_8,
    E_DMD_DVBT_N_RESERVE_9,
    E_DMD_DVBT_N_RESERVE_10,
    E_DMD_DVBT_N_RESERVE_11,              //0x7E

    E_DMD_T_CHANNEL_SWITCH   = 0x123,

    //dvbt auto ts data rate
    E_DMD_T_TS_DATA_RATE_0       = 0x130,
    E_DMD_T_TS_DATA_RATE_1       = 0x131,
    E_DMD_T_TS_DATA_RATE_2       = 0x132,
    E_DMD_T_TS_DATA_RATE_3       = 0x133,
    E_DMD_T_TS_DATA_RATE_CHANGE_IND       = 0x134,
    E_DMD_T_TS_DIV_172           = 0x135,
    E_DMD_T_TS_DIV_288           = 0x136,
    E_DMD_T_TS_DIV_432           = 0x137,
}DVBT_N_Param;

typedef enum
{
//Parameter version
E_DMD_DVBT_PARAM_VERSION	= 0x00,//	0x00
//System
E_DMD_DVBT_OP_AUTO_SCAN_MODE_EN,//
E_DMD_DVBT_CFG_FREQ,//
E_DMD_DVBT_CFG_BW,//
E_DMD_DVBT_CFG_MODE,//
E_DMD_DVBT_CFG_CP,//
E_DMD_DVBT_CFG_LP_SEL,//
E_DMD_DVBT_CFG_CSTL,//
E_DMD_DVBT_CFG_HIER,	//0x08
E_DMD_DVBT_CFG_HPCR,//
E_DMD_DVBT_CFG_LPCR,//

//AGC
E_DMD_DVBT_OP_RFAGC_EN,//
E_DMD_DVBT_OP_HUMDET_EN,//
E_DMD_DVBT_OP_AUTO_RF_MAX_EN,//
E_DMD_DVBT_CFG_RFMAX,//
E_DMD_DVBT_CFG_ZIF,//
E_DMD_DVBT_CFG_RSSI,	//0x10
E_DMD_DVBT_CFG_RFAGC_REF,//
E_DMD_DVBT_AGC_K,//
E_DMD_DVBT_CFG_IFAGC_REF_2K,//
E_DMD_DVBT_CFG_IFAGC_REF_8K,//
E_DMD_DVBT_CFG_IFAGC_REF_ACI,//
E_DMD_DVBT_CFG_IFAGC_REF_IIS,//
E_DMD_DVBT_AGC_REF,
E_DMD_DVBT_AGC_LOCK_TH,	//0x18
E_DMD_DVBT_AGC_LOCK_NUM,	//
E_DMD_DVBT_AGC_GAIN_LOCK,//

//ADC
E_DMD_DVBT_PWDN_ADCI,//
E_DMD_DVBT_PWDN_ADCQ,//
E_DMD_DVBT_MPLL_ADC_DIV_SEL,//

//DCR
E_DMD_DVBT_OP_DCR_EN,//
E_DMD_DVBT_DCR_LOCK,//
E_DMD_DVBT_DCR_LEAKY_I_FF_0,//0x20
E_DMD_DVBT_DCR_LEAKY_I_FF_1,//0x20
E_DMD_DVBT_DCR_LEAKY_I_FF_2,//0x20
E_DMD_DVBT_DCR_LEAKY_Q_FF_0,	//
E_DMD_DVBT_DCR_LEAKY_Q_FF_1,	//
E_DMD_DVBT_DCR_LEAKY_Q_FF_2,	//

//IIS
E_DMD_DVBT_OP_IIS_EN,//

//Mixer
E_DMD_DVBT_CFG_FC_L,//
E_DMD_DVBT_CFG_FC_H,//
E_DMD_DVBT_CFG_FS_L,//
E_DMD_DVBT_CFG_FS_H,//
E_DMD_DVBT_MIXER_IQ_SWAP_MODE,//
E_DMD_DVBT_MIXER_IQ_SWAP_OUT_0,//0x28
E_DMD_DVBT_MIXER_IQ_SWAP_OUT_1,
E_DMD_DVBT_MIXER_IQ_SWAP_OUT_2,
E_DMD_DVBT_MIXER_IQ_SWAP_OUT_3,
E_DMD_DVBT_MIXER_IQ_DBG_SEL,//

//IQ Swap
E_DMD_DVBT_OP_IQB_EN,//
E_DMD_DVBT_OP_AUTO_IQ_SWAP_EN,//
E_DMD_DVBT_CFG_IQ_SWAP,//
E_DMD_DVBT_IQB_PHASE_COARSE_0,//
E_DMD_DVBT_IQB_PHASE_COARSE_1,//
E_DMD_DVBT_IQB_GAIN_COARSE_0,//
E_DMD_DVBT_IQB_GAIN_COARSE_1,//

//ACI
E_DMD_DVBT_OP_ACI_EN,//0x30
E_DMD_DVBT_OP_AUTO_ACI_EN,	//
E_DMD_DVBT_CFG_ACI_DET_TH_L,//
E_DMD_DVBT_CFG_ACI_DET_TH_H,//

//CCI
E_DMD_DVBT_OP_CCI_EN,//
E_DMD_DVBT_CFG_CCI,//
E_DMD_DVBT_CCI_BYPASS,//
E_DMD_DVBT_CCI_TRACK_SW_RST,//
E_DMD_DVBT_CCI_LOCK_DET,//0x38
E_DMD_DVBT_CCI_FREQN_OUT_0,	//
E_DMD_DVBT_CCI_FREQN_OUT_1, //

//Interpolator
E_DMD_DVBT_CFG_2K_SFO_DELAY_TIME_H,//
E_DMD_DVBT_CFG_2K_SFO_DELAY_TIME_L,//
E_DMD_DVBT_CFG_8K_SFO_DELAY_TIME_H,//
E_DMD_DVBT_CFG_8K_SFO_DELAY_TIME_L,//
E_DMD_DVBT_INTP_RATEM1_0,//
E_DMD_DVBT_INTP_RATEM1_1,//
E_DMD_DVBT_INTP_RATEM1_2,//
E_DMD_DVBT_INTP_RATEM1_3,//

//ModeCP
E_DMD_DVBT_OP_FIX_MODE_CP_EN,//
E_DMD_DVBT_8K_MC_MODE,//
E_DMD_DVBT_8K_MC_CP,//
E_DMD_DVBT_8K_MC_CPOBS_NUM,//
E_DMD_DVBT_8K_MODECP_DET, //
E_DMD_DVBT_2K_MC_MODE, //
E_DMD_DVBT_2K_MC_CP, //
E_DMD_DVBT_2K_MC_CPOBS_NUM, //
E_DMD_DVBT_2K_MODECP_DET, //

//ICFO
E_DMD_DVBT_CFG_ICFO_RANGE,//
//E_DMD_DVBT_ICFO_RANGE,//0x40
E_DMD_DVBT_ICFO_SCAN_WINDOW_0,	//
E_DMD_DVBT_ICFO_SCAN_WINDOW_1,	//
E_DMD_DVBT_ICFO_MAX_OFFSET_0,//
E_DMD_DVBT_ICFO_MAX_OFFSET_1,//
E_DMD_DVBT_ICFO_DONE,//

//TPS
E_DMD_DVBT_OP_FIX_TPS_EN,//
E_DMD_DVBT_TPS_SYNC_LOCK,//
E_DMD_DVBT_CONSTELLATION,//
E_DMD_DVBT_HIERARCHY,//
E_DMD_DVBT_HP_CODE_RATE,//0x48
E_DMD_DVBT_LP_CODE_RATE,	//
E_DMD_DVBT_GUARD_INTERVAL,//
E_DMD_DVBT_TRANSMISSION_MODE,//
E_DMD_DVBT_OFDM_SYMBOL_NUMBER,//
E_DMD_DVBT_LENGTH_INDICATOR,//
E_DMD_DVBT_FRAME_NUMBER,//
E_DMD_DVBT_CELL_IDENTIFIER,//
E_DMD_DVBT_DVBH_SIGNALLING,//0x50

//SNR
E_DMD_DVBT_SNR_ACCU_0,	//
E_DMD_DVBT_SNR_ACCU_1,	//
E_DMD_DVBT_SNR_ACCU_2,	//
E_DMD_DVBT_SNR_ACCU_3,	//
E_DMD_DVBT_BIT_ERR_NUM_7_0,//
E_DMD_DVBT_BIT_ERR_NUM_15_8,//
E_DMD_DVBT_BIT_ERR_NUM_23_16,//
E_DMD_DVBT_BIT_ERR_NUM_31_24,//
E_DMD_DVBT_UNCRT_PKT_NUM_7_0,//
E_DMD_DVBT_UNCRT_PKT_NUM_15_8,//

//TS
E_DMD_DVBT_CFG_TS_SERIAL,//0x58
E_DMD_DVBT_CFG_TS_CLK_RATE,//
E_DMD_DVBT_CFG_TS_CLK_INV,//
E_DMD_DVBT_CFG_TS_DATA_SWAP,//
E_DMD_DVBT_TS_EN,//
E_DMD_DVBT_TS_SOURCE_SEL,//
E_DMD_DVBT_DVBTM_TS_CLK_POL,//
E_DMD_DVBT_DVBTM_TS_CLK_DIVNUM,//0x60
E_DMD_DVBT_EN_TS_PAD,	//
E_DMD_DVBT_IF_INV_PWM_OUT_EN,

//Reserve
E_DMD_DVBT_RESERVE_1,
E_DMD_DVBT_RESERVE_2,
E_DMD_DVBT_RESERVE_3,
E_DMD_DVBT_RESERVE_4,

//Debug
E_DMD_DVBT_CHECKSUM,//0x62
} DVBT_Param;

/// For demod init
typedef void (*fpIntCallBack)(MS_U8 u8arg);
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
    MS_U8 *u8DMD_DVBT_DSPRegInitExt; // TODO use system variable type
    MS_U8 u8DMD_DVBT_DSPRegInitSize;
    MS_U8 *u8DMD_DVBT_InitExt; // TODO use system variable type

    // for T/T2 merged FW
    MS_U32  u32EqStartAddr;
    MS_U32  u32TdiStartAddr;
    MS_U32  u32DjbStartAddr;
    MS_U32  u32FwStartAddr;
} DMD_DVBT_InitData;




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
    MS_U8 *u8DMD_DVBT_DSPRegInitExt; // TODO use system variable type
    MS_U8 u8DMD_DVBT_DSPRegInitSize;
    MS_U8 *u8DMD_DVBT_InitExt; // TODO use system variable type

    // for T/T2 merged FW
    MS_U32  u32EqStartAddr;
    MS_U32  u32TdiStartAddr;
    MS_U32  u32DjbStartAddr;
    MS_U32  u32FwStartAddr;
} DMD_DVBT_InitData_KERNEL;

typedef enum
{
    E_DMD_DVBT_FAIL=0,
    E_DMD_DVBT_OK=1
} DMD_DVBT_Result;


typedef enum
{
    E_DMD_DVBT_MODULATION_INFO,
    E_DMD_DVBT_DEMOD_INFO,
    E_DMD_DVBT_LOCK_INFO,
    E_DMD_DVBT_PRESFO_INFO,
    E_DMD_DVBT_LOCK_TIME_INFO,
    E_DMD_DVBT_BER_INFO,
    E_DMD_DVBT_AGC_INFO,
} DMD_DVBT_INFO_TYPE;

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
} DMD_DVBT_Info;

//typedef void(*P_DMD_ISR_Proc)(MS_U8 u8DMDID);


//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBT_Init
////////////////////////////////////////////////////////////////////////////////
extern MS_BOOL MDrv_DMD_DVBT_Init(DMD_DVBT_InitData *pDMD_DVBT_InitData, MS_U32 u32InitDataLen);
////////////////////////////////////////////////////////////////////////////////
/// Should be called when exit VD input source
////////////////////////////////////////////////////////////////////////////////
extern MS_BOOL MDrv_DMD_DVBT_Exit(void);
////////////////////////////////////////////////////////////////////////////////
/// Get Initial Data
/// @ingroup DVBT_BASIC
/// @param pDMD_DVBT_InitData    \b IN: DVBT initial parameters
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
extern MS_U32 SYMBOL_WEAK MDrv_DMD_DVBT_GetConfig(DMD_DVBT_InitData *pDMD_DVBT_InitData);
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
extern MS_BOOL MDrv_DMD_DVBT_SetDbgLevel(DMD_DbgLv u8DbgLevel);
//-------------------------------------------------------------------------------------------------
/// Get the information of DVBT driver\n
/// @return the pointer to the driver information
//-------------------------------------------------------------------------------------------------
extern DMD_DVBT_Info* MDrv_DMD_DVBT_GetInfo(DMD_DVBT_INFO_TYPE eInfoType);
//-------------------------------------------------------------------------------------------------
/// Get DVBT driver version
/// when get ok, return the pointer to the driver version
//-------------------------------------------------------------------------------------------------
extern MS_BOOL MDrv_DMD_DVBT_GetLibVer(const MSIF_Version **ppVersion);
////////////////////////////////////////////////////////////////////////////////
/// To get DVBT's register  value, only for special purpose.\n
/// u16Addr       : the address of DVBT's register\n
/// return the value of AFEC's register\n
////////////////////////////////////////////////////////////////////////////////
extern MS_BOOL MDrv_DMD_DVBT_GetReg(MS_U16 u16Addr, MS_U8 *pu8Data);
////////////////////////////////////////////////////////////////////////////////
/// To set DVBT's register value, only for special purpose.\n
/// u16Addr       : the address of DVBT's register\n
/// u8Value        : the value to be set\n
////////////////////////////////////////////////////////////////////////////////
extern MS_BOOL MDrv_DMD_DVBTGetFWVer(MS_U16 *ver);
////////////////////////////////////////////////////////////////////////////////
/// Get DVBT FW version
/// u16Addr       : the address of DVBT's register\n
////////////////////////////////////////////////////////////////////////////////
extern MS_BOOL MDrv_DMD_DVBT_SetReg(MS_U16 u16Addr, MS_U8 u8Data);
////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBT_SetSerialControl
////////////////////////////////////////////////////////////////////////////////
extern MS_BOOL MDrv_DMD_DVBT_SetSerialControl(MS_BOOL bEnable);
////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBT_SetConfig
////////////////////////////////////////////////////////////////////////////////
extern MS_BOOL MDrv_DMD_DVBT_SetConfig(DMD_RF_CHANNEL_BANDWIDTH BW, MS_BOOL bSerialTS, MS_BOOL bPalBG);
////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBT_SetConfigHPLP
////////////////////////////////////////////////////////////////////////////////
extern MS_BOOL MDrv_DMD_DVBT_SetConfigHPLP(DMD_RF_CHANNEL_BANDWIDTH BW, MS_BOOL bSerialTS, MS_BOOL bPalBG, MS_BOOL bLPSel);
////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBT_SetConfigHPLPSetIF
////////////////////////////////////////////////////////////////////////////////
extern MS_BOOL MDrv_DMD_DVBT_SetConfigHPLPSetIF(DMD_RF_CHANNEL_BANDWIDTH BW, MS_BOOL bSerialTS, MS_BOOL bPalBG, MS_BOOL bLPSel, MS_U32 u32IFFreq, MS_U32 u32FSFreq, MS_U8 u8IQSwap);
///////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBT_SetActive
////////////////////////////////////////////////////////////////////////////////
extern MS_BOOL MDrv_DMD_DVBT_SetActive(MS_BOOL bEnable);
////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBT_Get_Lock
////////////////////////////////////////////////////////////////////////////////
extern MS_BOOL MDrv_DMD_DVBT_GetLock(DMD_DVBT_GETLOCK_TYPE eType, DMD_LOCK_STATUS *eLockStatus);
////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBT_GetSignalStrength
////////////////////////////////////////////////////////////////////////////////
extern MS_BOOL MDrv_DMD_DVBT_GetSignalStrength(MS_U16 *u16Strength);
////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBT_GetSignalStrengthWithRFPower
////////////////////////////////////////////////////////////////////////////////
extern MS_BOOL MDrv_DMD_DVBT_GetSignalStrengthWithRFPower(MS_U16 *u16Strength, float fRFPowerDbm);
////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBT_GetSignalQuality
////////////////////////////////////////////////////////////////////////////////
extern MS_BOOL MDrv_DMD_DVBT_GetSignalQuality(MS_U16 *u16Quality);
////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBT_GetSignalQualityWithRFPower
////////////////////////////////////////////////////////////////////////////////
extern MS_BOOL MDrv_DMD_DVBT_GetSignalQualityWithRFPower(MS_U16 *u16Quality, float fRFPowerDbm);
////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBT_GetSNR
////////////////////////////////////////////////////////////////////////////////
extern MS_BOOL MDrv_DMD_DVBT_GetSNR(float *fSNR);
////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBT_GetPostViterbiBer
////////////////////////////////////////////////////////////////////////////////
extern MS_BOOL MDrv_DMD_DVBT_GetPostViterbiBer(float *ber);
////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBT_GetPreViterbiBer
////////////////////////////////////////////////////////////////////////////////
extern MS_BOOL MDrv_DMD_DVBT_GetPreViterbiBer(float *ber);
////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBT_GetPacketErr
////////////////////////////////////////////////////////////////////////////////
extern MS_BOOL MDrv_DMD_DVBT_GetPacketErr(MS_U16 *pktErr);
////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBT_GetTPSInfo
////////////////////////////////////////////////////////////////////////////////
extern MS_BOOL MDrv_DMD_DVBT_GetTPSInfo(MS_U16 *u16Info);
////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBT_GetCellID
////////////////////////////////////////////////////////////////////////////////
extern MS_BOOL MDrv_DMD_DVBT_GetCellID(MS_U16 *u16CellID);
////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBT_GetFreqOffset
////////////////////////////////////////////////////////////////////////////////
extern MS_BOOL MDrv_DMD_DVBT_GetFreqOffset(float *pFreqOff);
////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBT_NORDIG_SSI_Table_Write
////////////////////////////////////////////////////////////////////////////////
extern MS_BOOL MDrv_DMD_DVBT_NORDIG_SSI_Table_Write(DMD_CONSTEL constel, DMD_CODERATE code_rate, float write_value);
////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBT_NORDIG_SSI_Table_Read
////////////////////////////////////////////////////////////////////////////////
extern MS_BOOL MDrv_DMD_DVBT_NORDIG_SSI_Table_Read(DMD_CONSTEL constel, DMD_CODERATE code_rate, float *read_value);

extern MS_U32 MDrv_DMD_DVBT_SetPowerState(EN_POWER_MODE u16PowerState);

//Kernel mode replaced functions

//extern MS_BOOL MDrv_DMD_DVBT_GetSignalStrength(MS_U16 *u16Strength);
extern MS_BOOL MDrv_DMD_DVBT_GetSignalStrength_KERNEL(MS_U16 *u16Strength);
//extern MS_BOOL MDrv_DMD_DVBT_GetSignalStrengthWithRFPower(MS_U16 *u16Strength, float fRFPowerDbm);
extern MS_BOOL MDrv_DMD_DVBT_GetSignalStrengthWithRFPower_KERNEL(MS_U16	*u16Strength,MS_FLOAT_ST	msf_RFPowerDbm);
//extern MS_BOOL MDrv_DMD_DVBT_GetSignalQuality(MS_U16 *u16Quality);
extern MS_BOOL MDrv_DMD_DVBT_GetSignalQuality_KERNEL(MS_U16 *u16Quality);
//extern MS_BOOL MDrv_DMD_DVBT_GetSNR(float *fSNR);
extern MS_BOOL MDrv_DMD_DVBT_GetSNR_KERNEL(MS_FLOAT_ST *msf_SNR);
//extern MS_BOOL MDrv_DMD_DVBT_GetPostViterbiBer(float *ber);
extern MS_BOOL MDrv_DMD_DVBT_GetPostViterbiBer_KERNEL(MS_FLOAT_ST *msf_ber);
//extern MS_BOOL MDrv_DMD_DVBT_Init(DMD_DVBT_InitData *pDMD_DVBT_InitData, MS_U32 u32InitDataLen);
extern MS_BOOL MDrv_DMD_DVBT_Init_KERNEL(DMD_DVBT_InitData_KERNEL *pDMD_DVBT_InitData,	MS_U32 u32InitDataLen);



#ifdef UFO_DEMOD_DVBT_SUPPORT_DMD_INT
MS_BOOL MDrv_DMD_DVBT_Reg_INT_CB(fpIntCallBack fpCBReg);
#endif

#ifdef __cplusplus
}
#endif


#endif // _DRV_DVBT_H_

