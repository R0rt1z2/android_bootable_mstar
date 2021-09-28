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

#ifndef _DRV_DVBS_H_
#define _DRV_DVBS_H_
#include "UFO.h"
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
#define MSIF_DMD_DVBS_INTERN_LIB_CODE           {'D','V', 'B','S'} //Lib code
#define MSIF_DMD_DVBS_INTERN_LIBVER             {'0','1'}      //LIB version
#define MSIF_DMD_DVBS_INTERN_BUILDNUM           {'2','2' }    //Build Number
#define MSIF_DMD_DVBS_INTERN_CHANGELIST         {'0','0','0','0','0','0','0','0'} //P4 ChangeList Number

#define DMD_DVBS_INTERN_VER             /* Character String for DRV/API version             */  \
    MSIF_TAG,                           /* 'MSIF'                                           */  \
    MSIF_CLASS,                         /* '00'                                             */  \
    MSIF_CUS,                           /* 0x0000                                           */  \
    MSIF_MOD,                           /* 0x0000                                           */  \
    MSIF_CHIP,                                                                                  \
    MSIF_CPU,                                                                                   \
    MSIF_DMD_DVBS_INTERN_LIB_CODE,      /* IP__                                             */  \
    MSIF_DMD_DVBS_INTERN_LIBVER,        /* 0.0 ~ Z.Z                                        */  \
    MSIF_DMD_DVBS_INTERN_BUILDNUM,      /* 00 ~ 99                                          */  \
    MSIF_DMD_DVBS_INTERN_CHANGELIST,    /* CL#                                              */  \
    MSIF_OS

#define IS_BITS_SET(val, bits)                  (((val)&(bits)) == (bits))

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
typedef enum
{
    DMD_DVBS_DBGLV_NONE,    // disable all the debug message
    DMD_DVBS_DBGLV_INFO,    // information
    DMD_DVBS_DBGLV_NOTICE,  // normal but significant condition
    DMD_DVBS_DBGLV_WARNING, // warning conditions
    DMD_DVBS_DBGLV_ERR,     // error conditions
    DMD_DVBS_DBGLV_CRIT,    // critical conditions
    DMD_DVBS_DBGLV_ALERT,   // action must be taken immediately
    DMD_DVBS_DBGLV_EMERG,   // system is unusable
    DMD_DVBS_DBGLV_DEBUG,   // debug-level messages
} DMD_DVBS_DbgLv;

typedef enum
{
    DMD_DVBS_LOCK,
    DMD_DVBS_CHECKING,
    DMD_DVBS_CHECKEND,
    DMD_DVBS_UNLOCK,
    DMD_DVBS_NULL,
} DMD_DVBS_LOCK_STATUS;

typedef enum
{
    DMD_DVBS_GETLOCK,
    DMD_DVBS_GETLOCK_TR_EVER_LOCK,
    DMD_DVBS_GETLOCK_NO_CHANNEL,
} DMD_DVBS_GETLOCK_TYPE;

typedef enum
{
    DMD_DVBS_QPSK = 0,
    DMD_DVBS_8PSK = 1,
    DMD_DVBS_16APSK = 2,
    DMD_DVBS_32APSK = 3,
    DMD_DVBS_8APSK = 4,
    DMD_DVBS_8_8APSK = 5,
    DMD_DVBS_4_8_4_16APSK = 6
} DMD_DVBS_MODULATION_TYPE;

typedef enum
{
    DMD_SAT_DVBS  = 0,
    DMD_SAT_DVBS2 = 1,
} DMD_DVBS_DEMOD_TYPE;

#ifdef UFO_DEMOD_DVBS_BERINFO
typedef enum
{
   DMD_SAT_DVBS_PREVITERBI_DVBS2_PRELDPC = 0,
   DMD_SAT_DVBS_PREVITERBI_DVBS2_POSTLDPC = 1,
   DMD_SAT_DVBS_POSTVITERBI_DVBS2_PRELDPC = 2,
   DMD_SAT_DVBS_POSTVITERBI_DVBS2_POSTLDPC = 3,
   DMD_SAT_DVBS_BER_TYPE_MAX
}DMD_DVBS_DEMOD_BER_TYPE;
#endif

typedef enum
{
/*
    E_DMD_S2_ZIF_EN = 0x00,
    E_DMD_S2_RF_AGC_EN,
    E_DMD_S2_DCR_EN,
    E_DMD_S2_IQB_EN,
    E_DMD_S2_IIS_EN,
    E_DMD_S2_CCI_EN,
    E_DMD_S2_FORCE_ACI_SELECT,
    E_DMD_S2_IQ_SWAP,    //For DVBS2
    E_DMD_S2_AGC_REF_EXT_0,
    E_DMD_S2_AGC_REF_EXT_1,
    E_DMD_S2_AGC_K,
    E_DMD_S2_ADCI_GAIN,
    E_DMD_S2_ADCQ_GAIN,
    E_DMD_S2_SRD_SIG_SRCH_RNG,
    E_DMD_S2_SRD_DC_EXC_RNG,
    E_DMD_S2_FORCE_CFO_0,    //0FH
    E_DMD_S2_FORCE_CFO_1,
    E_DMD_S2_DECIMATION_NUM,
    E_DMD_S2_PSD_SMTH_TAP,
    E_DMD_S2_CCI_FREQN_0_L,
    E_DMD_S2_CCI_FREQN_0_H,
    E_DMD_S2_CCI_FREQN_1_L,
    E_DMD_S2_CCI_FREQN_1_H,
    E_DMD_S2_CCI_FREQN_2_L,
    E_DMD_S2_CCI_FREQN_2_H,
    E_DMD_S2_TR_LOPF_KP,
    E_DMD_S2_TR_LOPF_KI,
    E_DMD_S2_FINEFE_KI_SWITCH_0,
    E_DMD_S2_FINEFE_KI_SWITCH_1,
    E_DMD_S2_FINEFE_KI_SWITCH_2,
    E_DMD_S2_FINEFE_KI_SWITCH_3,
    E_DMD_S2_FINEFE_KI_SWITCH_4,    //1FH
    E_DMD_S2_PR_KP_SWITCH_0,
    E_DMD_S2_PR_KP_SWITCH_1,
    E_DMD_S2_PR_KP_SWITCH_2,
    E_DMD_S2_PR_KP_SWITCH_3,
    E_DMD_S2_PR_KP_SWITCH_4,
    E_DMD_S2_FS_GAMMA,
    E_DMD_S2_FS_ALPHA0,
    E_DMD_S2_FS_ALPHA1,
    E_DMD_S2_FS_ALPHA2,
    E_DMD_S2_FS_ALPHA3,
    E_DMD_S2_FS_H_MODE_SEL,
    E_DMD_S2_FS_OBSWIN,
    E_DMD_S2_FS_PEAK_DET_TH_L,
    E_DMD_S2_FS_PEAK_DET_TH_H,
    E_DMD_S2_FS_CONFIRM_NUM,
    E_DMD_S2_EQ_MU_FFE_DA,    //2FH
    E_DMD_S2_EQ_MU_FFE_DD,
    E_DMD_S2_EQ_ALPHA_SNR_DA,
    E_DMD_S2_EQ_ALPHA_SNR_DD,
    E_DMD_S2_FEC_ALFA,    //For DVBS2
    E_DMD_S2_FEC_BETA,    //For DVBS2
    E_DMD_S2_FEC_SCALING_LLR,    //For DVBS2
*/
    E_DMD_S2_TS_SERIAL=0x00,
    E_DMD_S2_TS_CLK_RATE,
    E_DMD_S2_TS_OUT_INV,
    E_DMD_S2_TS_DATA_SWAP,
    //------------------------------------------
    E_DMD_S2_FW_VERSION_L,
    E_DMD_S2_FW_VERSION_H,
    E_DMD_S2_CHIP_VERSION,
    E_DMD_S2_FS_L,//Frequency
    E_DMD_S2_FS_H,//Frequency
    E_DMD_S2_MANUAL_TUNE_SYMBOLRATE_L,//0x3F
    E_DMD_S2_MANUAL_TUNE_SYMBOLRATE_H,//
    E_DMD_S2_SYSTEM_TYPE,//DVBS/S2
    E_DMD_S2_MODULATION_TYPE,//QPSK/8PSK
    E_DMD_S2_BLINDSCAN_CHECK,
    E_DMD_S2_UNCRT_PKT_NUM_7_0,
    E_DMD_S2_UNCRT_PKT_NUM_8_15,
    E_DMD_S2_STATE_FLAG,//0x10
    E_DMD_S2_SUBSTATE_FLAG,
    E_DMD_S2_HUM_DETECT_FLAG,
    E_DMD_S2_CCI_DETECT_FLAG,
    E_DMD_S2_IIS_DETECT_FLAG,
    E_DMD_S2_OPEN_HUM_VLD_IRQ_FLAG,
    E_DMD_S2_SRD_COARSE_DONE_FLAG,
    E_DMD_S2_SRD_FINE_DONE_FLAG,
    E_DMD_S2_FINEFE_DONE_FLAG,
    E_DMD_S2_REV_FRAME_FLAG,
    E_DMD_S2_DUMMY_FRAME_FLAG,
    E_DMD_S2_PLSC_DONE_FLAG,
    E_DMD_S2_GET_INFO_FROM_FRAME_LENGTH_DONE_FLAG,
    E_DMD_S2_IQ_SWAP_DETECT_FLAG,
    E_DMD_S2_FRAME_ACQUISITION_DONE_FLAG,
    E_DMD_S2_OLCFE_DONE_FLAG,
    E_DMD_S2_FSYNC_FOUND_FLAG,//0x20
    E_DMD_S2_FSYNC_FAIL_SEARCH_FLAG,
    E_DMD_S2_FALSE_ALARM_FLAG,
    E_DMD_S2_VITERBI_IN_SYNC_FLAG,
    E_DMD_S2_INT_CODE_RATE_SEARCH_FAIL_FLAG,
    E_DMD_S2_VITERBI_INT_PRE_FLAG,
    E_DMD_S2_BER_WINDOW_END_FLAG,
    E_DMD_S2_PASS_WRONG_INT_FLAG,
    E_DMD_S2_CLK_CNT_OVER_FLAG,
    E_DMD_S2_UNCRT_OVER_FLAG,
    E_DMD_S2_DISEQC_RX_LENGTH,
    E_DMD_S2_DISEQC_INTERRUPT_FLAG,
    E_DMD_S2_DISEQC_RX_FLAG,
    E_DMD_S2_DISEQC_INTERRUPT_STATUS,
    E_DMD_S2_DISEQC_STATUS_FLAG,
    E_DMD_S2_ACI_FIR_SELECTED,
    //LOCK
    E_DMD_S2_AGC_LOCK_FLAG,//0x30
    E_DMD_S2_DCR_LOCK_FLAG,
    E_DMD_S2_DAGC0_LOCK_FLAG,
    E_DMD_S2_DAGC1_LOCK_FLAG,
    E_DMD_S2_DAGC2_LOCK_FLAG,
    E_DMD_S2_DAGC3_LOCK_FLAG,
    E_DMD_S2_TR_LOCK_FLAG,
    E_DMD_S2_CLCFE_LOCK_FLAG,
    E_DMD_S2_EQ_LOCK_FLAG,
    E_DMD_S2_PR_LOCK_FLAG,
    E_DMD_S2_FSYNC_LOCK_FLAG,
    E_DMD_S2_FSYNC_FAIL_LOCK_FLAG,
    E_DMD_S2_MB_SWUSE12L,
    E_DMD_S2_MB_SWUSE12H,
    E_DMD_S2_MB_SWUSE13L,
    E_DMD_S2_MB_SWUSE13H,
    E_DMD_S2_MB_SWUSE14L,//0x40
    E_DMD_S2_MB_SWUSE14H,
    E_DMD_S2_MB_SWUSE15L,
    E_DMD_S2_MB_SWUSE15H,
    E_DMD_S2_MB_SWUSE16L,
    E_DMD_S2_MB_SWUSE16H,
    E_DMD_S2_MB_SWUSE17L,
    E_DMD_S2_MB_SWUSE17H,
    E_DMD_S2_MB_SWUSE18L,
    E_DMD_S2_MB_SWUSE18H,
    E_DMD_S2_MB_SWUSE19L,
    E_DMD_S2_MB_SWUSE19H,
    E_DMD_S2_MB_SWUSE1AL,
    E_DMD_S2_MB_SWUSE1AH,
    E_DMD_S2_MB_SWUSE1BL,
    E_DMD_S2_MB_SWUSE1BH,
    E_DMD_S2_MB_SWUSE1CL,//0x50
    E_DMD_S2_MB_SWUSE1CH,
    E_DMD_S2_MB_SWUSE1DL,
    E_DMD_S2_MB_SWUSE1DH,
    E_DMD_S2_MB_SWUSE1EL,
    E_DMD_S2_MB_SWUSE1EH,
    E_DMD_S2_MB_SWUSE1FL,
    E_DMD_S2_MB_SWUSE1FH,
    E_DMD_S2_MB_DMDTOP_DBG_0,
    E_DMD_S2_MB_DMDTOP_DBG_1,
    E_DMD_S2_MB_DMDTOP_DBG_2,
    E_DMD_S2_MB_DMDTOP_DBG_3,
    E_DMD_S2_MB_DMDTOP_DBG_4,
    E_DMD_S2_MB_DMDTOP_DBG_5,
    E_DMD_S2_MB_DMDTOP_DBG_6,
    E_DMD_S2_MB_DMDTOP_DBG_7,
    E_DMD_S2_MB_DMDTOP_DBG_8,//0x60
    E_DMD_S2_MB_DMDTOP_DBG_9,
    E_DMD_S2_MB_DMDTOP_DBG_A,
    E_DMD_S2_MB_DMDTOP_DBG_B,
    E_DMD_S2_MB_DMDTOP_SWUSE00L,
    E_DMD_S2_MB_DMDTOP_SWUSE00H,
    E_DMD_S2_MB_DMDTOP_SWUSE01L,
    E_DMD_S2_MB_DMDTOP_SWUSE01H,
    E_DMD_S2_MB_DMDTOP_SWUSE02L,
    E_DMD_S2_MB_DMDTOP_SWUSE02H,
    E_DMD_S2_MB_DMDTOP_SWUSE03L,
    E_DMD_S2_MB_DMDTOP_SWUSE03H,
    E_DMD_S2_MB_DMDTOP_SWUSE04L,
    E_DMD_S2_MB_DMDTOP_SWUSE04H,
    E_DMD_S2_MB_DMDTOP_SWUSE05L,
    E_DMD_S2_MB_DMDTOP_SWUSE05H,
    E_DMD_S2_MB_DMDTOP_SWUSE06L,//0x70
    E_DMD_S2_MB_DMDTOP_SWUSE06H,
    E_DMD_S2_MB_DMDTOP_SWUSE07L,
    E_DMD_S2_MB_DMDTOP_SWUSE07H,
    E_DMD_S2_MB_TOP_WR_DBG_90,
    E_DMD_S2_MB_TOP_WR_DBG_91,
    E_DMD_S2_MB_TOP_WR_DBG_92,
    E_DMD_S2_MB_TOP_WR_DBG_93,
    E_DMD_S2_MB_TOP_WR_DBG_94,
    E_DMD_S2_MB_TOP_WR_DBG_95,
    E_DMD_S2_MB_TOP_WR_DBG_96,
    E_DMD_S2_MB_TOP_WR_DBG_97,
    E_DMD_S2_MB_TOP_WR_DBG_98,
    E_DMD_S2_MB_TOP_WR_DBG_99,
    E_DMD_S2_MB_DUMMY_REG_0,
    E_DMD_S2_MB_DUMMY_REG_1,
    E_DMD_S2_MB_DUMMY_REG_2,//0x80
    E_DMD_S2_MB_DUMMY_REG_3,
    E_DMD_S2_MB_DUMMY_REG_4,
    E_DMD_S2_MB_DUMMY_REG_5,
    E_DMD_S2_MB_DUMMY_REG_6,
    E_DMD_S2_MB_DUMMY_REG_7,
    E_DMD_S2_MB_DUMMY_REG_8,
    E_DMD_S2_MB_DUMMY_REG_9,
    E_DMD_S2_MB_DUMMY_REG_A,
    E_DMD_S2_MB_DUMMY_REG_B,
    E_DMD_S2_MB_DUMMY_REG_C,
    E_DMD_S2_MB_DUMMY_REG_D,
    E_DMD_S2_MB_DUMMY_REG_E,
    E_DMD_S2_MB_DUMMY_REG_F,
    E_DMD_S2_MB_DUMMY_REG_10,
    E_DMD_S2_MB_DUMMY_REG_11,
    E_DMD_S2_MB_DMDTOP_INFO_01,//0x90
    E_DMD_S2_MB_DMDTOP_INFO_02,
    E_DMD_S2_MB_DMDTOP_INFO_03,
    E_DMD_S2_MB_DMDTOP_INFO_04,
    E_DMD_S2_MB_DMDTOP_INFO_05,
    E_DMD_S2_MB_DMDTOP_INFO_06,
    E_DMD_S2_MB_DMDTOP_INFO_07,
    E_DMD_S2_MB_DMDTOP_INFO_08,
    E_DMD_S2_IDLE_STATE_UPDATED,
    E_DMD_S2_LOG_FLAG,
    E_DMD_S2_LOG_SKIP_INDEX,
    E_DMD_S2_LOCK_COUNT,
    E_DMD_S2_NARROW_STEP_FLAG,
    E_DMD_S2_UNCORRECT_PKT_COUNT,
    E_DMD_S2_DISEQC_INIT_MODE,
    E_DMD_S2_DECIMATE_FORCED,
    E_DMD_S2_SRD_MAX_SRG_FLAG,//0xA0
    E_DMD_S2_DVBS_OUTER_RETRY,
    E_DMD_S2_FORCED_DECIMATE_FLAG,
    E_DMD_S2_NO_SIGNAL_FLAG,
    E_DMD_S2_SPECTRUM_TRACK_FLAG,
    E_DMD_S2_SRD_LOCAL_SEARCH_FLAG,
    E_DMD_S2_NO_SIGNAL_RATIO_CHECK_FLAG,
    E_DMD_S2_LOW_SR_ACI_FLAG,
    E_DMD_S2_SPECTRUM_TRACKER_TIMEOUT,
    E_DMD_S2_TR_TIMEOUT,
    E_DMD_S2_BALANCE_TRACK,
    E_DMD_S2_GAIN_TILT_FLAG,
    E_DMD_S2_SIS_EN,
    E_DMD_S2_ISSY_ACTIVE,
    E_DMD_S2_CODE_RATE,
    E_DMD_S2_PILOT_FLAG,
    E_DMD_S2_FEC_TYPE,//0xB0
    E_DMD_S2_MOD_TYPE,
    E_DMD_S2_VCM_OPT,
    E_DMD_S2_OPPRO_FLAG,
    E_DMD_S2_IS_ID,
    E_DMD_S2_CHECK_EVER_UNLOCK,
    E_DMD_S2_IS_ID_TABLE = 0x100, // use 32bytes length
    DVBS2_PARAM_LEN = 120
} DVBS_Param_2;



//Interrupt callback
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
    MS_U8 *u8DMD_DVBS_DSPRegInitExt; // TODO use system variable type
    MS_U8 u8DMD_DVBS_DSPRegInitSize;
    MS_U8 *u8DMD_DVBS_InitExt; // TODO use system variable type

    //add for Kris: Since we have to borrow 3MByte DRAM for DJB
    MS_U32  u32FwStartAddr;
    MS_U32  u32DjbStartAddr;
} DMD_DVBS_InitData;

typedef struct
{
    // tuner parameter
    MS_U8 u8SarChannel;
    DMD_RFAGC_SSI_KERNEL*pTuner_RfagcSsi;
    MS_U16 u16Tuner_RfagcSsi_Size;
    DMD_IFAGC_SSI_KERNEL*pTuner_IfagcSsi_LoRef;
    MS_U16 u16Tuner_IfagcSsi_LoRef_Size;
    DMD_IFAGC_SSI_KERNEL*pTuner_IfagcSsi_HiRef;
    MS_U16 u16Tuner_IfagcSsi_HiRef_Size;
    DMD_IFAGC_ERR_KERNEL*pTuner_IfagcErr_LoRef;
    MS_U16 u16Tuner_IfagcErr_LoRef_Size;
    DMD_IFAGC_ERR_KERNEL*pTuner_IfagcErr_HiRef;
    MS_U16 u16Tuner_IfagcErr_HiRef_Size;
    DMD_SQI_CN_NORDIGP1_KERNEL*pSqiCnNordigP1;
    MS_U16 u16SqiCnNordigP1_Size;

    // register init
    MS_U8 *u8DMD_DVBS_DSPRegInitExt; // TODO use system variable type
    MS_U8 u8DMD_DVBS_DSPRegInitSize;
    MS_U8 *u8DMD_DVBS_InitExt; // TODO use system variable type

    //add for Kris: Since we have to borrow 3MByte DRAM for DJB
    MS_U32  u32FwStartAddr;
    MS_U32  u32DjbStartAddr;
} DMD_DVBS_InitData_KERNEL;

typedef enum
{
    E_DMD_DVBS_FAIL=0,
    E_DMD_DVBS_OK=1
} DMD_DVBS_Result;


typedef struct
{
    MS_U16 u16Version;
    MS_U32 u32SymbolRate;
    DMD_DVBS_MODULATION_TYPE eQamMode;
    MS_U32 u32IFFreq;
    MS_BOOL bSpecInv;
    MS_BOOL bSerialTS;
    MS_U8 u8SarValue;
    MS_U32 u32ChkScanTimeStart;
    DMD_DVBS_LOCK_STATUS eLockStatus;
    MS_U16 u16Strength;
    MS_U16 u16Quality;
    MS_U32 u32Intp; //
    MS_U32 u32FcFs; //
    MS_U8 u8Qam; //
    MS_U16 u16SymbolRateHal; //
} DMD_DVBS_Info;

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
/// Define converlution code rate for DVB-T and DVB-S
typedef enum
{
    HAL_DEMOD_CONV_CODE_RATE_1_2,///< Code rate = 1/2
    HAL_DEMOD_CONV_CODE_RATE_1_3,///< Code rate = 1/3
    HAL_DEMOD_CONV_CODE_RATE_2_3,///< Code rate = 2/3
    HAL_DEMOD_CONV_CODE_RATE_1_4,///< Code rate = 1/4
    HAL_DEMOD_CONV_CODE_RATE_3_4,///< Code rate = 3/4
    HAL_DEMOD_CONV_CODE_RATE_2_5,///< Code rate = 2/5
    HAL_DEMOD_CONV_CODE_RATE_3_5,///< Code rate = 3/5
    HAL_DEMOD_CONV_CODE_RATE_4_5,///< Code rate = 4/5
    HAL_DEMOD_CONV_CODE_RATE_5_6,///< Code rate = 5/6
    HAL_DEMOD_CONV_CODE_RATE_7_8,///< Code rate = 7/8
    HAL_DEMOD_CONV_CODE_RATE_8_9,///< Code rate = 8/9
    HAL_DEMOD_CONV_CODE_RATE_9_10,///< Code rate = 9/10

    HAL_DEMOD_DVBS2_CODE_RATE_2_9,
    HAL_DEMOD_DVBS2_CODE_RATE_13_45,
    HAL_DEMOD_DVBS2_CODE_RATE_9_20,
    HAL_DEMOD_DVBS2_CODE_RATE_90_180,
    HAL_DEMOD_DVBS2_CODE_RATE_96_180,
    HAL_DEMOD_DVBS2_CODE_RATE_11_20,
    HAL_DEMOD_DVBS2_CODE_RATE_100_180,
    HAL_DEMOD_DVBS2_CODE_RATE_104_180,
    HAL_DEMOD_DVBS2_CODE_RATE_26_45_L,
    HAL_DEMOD_DVBS2_CODE_RATE_18_30,
    HAL_DEMOD_DVBS2_CODE_RATE_28_45,
    HAL_DEMOD_DVBS2_CODE_RATE_23_36,
    HAL_DEMOD_DVBS2_CODE_RATE_116_180,
    HAL_DEMOD_DVBS2_CODE_RATE_20_30,
    HAL_DEMOD_DVBS2_CODE_RATE_124_180,
    HAL_DEMOD_DVBS2_CODE_RATE_25_36,
    HAL_DEMOD_DVBS2_CODE_RATE_128_180,
    HAL_DEMOD_DVBS2_CODE_RATE_13_18,
    HAL_DEMOD_DVBS2_CODE_RATE_132_180,
    HAL_DEMOD_DVBS2_CODE_RATE_22_30,
    HAL_DEMOD_DVBS2_CODE_RATE_135_180,
    HAL_DEMOD_DVBS2_CODE_RATE_140_180,
    HAL_DEMOD_DVBS2_CODE_RATE_7_9,
    HAL_DEMOD_DVBS2_CODE_RATE_154_180,
    HAL_DEMOD_DVBS2_CODE_RATE_11_45,
    HAL_DEMOD_DVBS2_CODE_RATE_4_15,
    HAL_DEMOD_DVBS2_CODE_RATE_14_45,
    HAL_DEMOD_DVBS2_CODE_RATE_7_15,
    HAL_DEMOD_DVBS2_CODE_RATE_8_15,
    HAL_DEMOD_DVBS2_CODE_RATE_26_45_S,
    HAL_DEMOD_DVBS2_CODE_RATE_32_45
} HAL_DEMOD_EN_CONV_CODE_RATE_TYPE;

/// Define terrestrial band width
typedef enum
{
    HAL_DEMOD_BW_MODE_6MHZ = 0,                                                   ///< 6 MHz
    HAL_DEMOD_BW_MODE_7MHZ,                                                       ///< 7 MHz
    HAL_DEMOD_BW_MODE_8MHZ                                                        ///< 8 MHz
} HAL_DEMOD_EN_TER_BW_MODE;


/// Define terrestrial constellation type
typedef enum
{
    HAL_DEMOD_TER_QPSK,                                                           ///< QPSK type
    HAL_DEMOD_TER_QAM16,                                                          ///< QAM 16 type
    HAL_DEMOD_TER_QAM64                                                           ///< QAM 64 type
} HAL_DEMOD_EN_TER_CONSTEL_TYPE;

/// Define terrestrial hierarchy information
typedef enum
{
    HAL_DEMOD_TER_HIE_NONE,                                                       ///< Non-hierarchy
    HAL_DEMOD_TER_HIE_ALPHA_1,                                                    ///< Hierarchy alpha = 1
    HAL_DEMOD_TER_HIE_ALPHA_2,                                                    ///< Hierarchy alpha = 2
    HAL_DEMOD_TER_HIE_ALPHA_4                                                     ///< Hierarchy alpha = 4
} HAL_DEMOD_EN_TER_HIE_TYPE;

/// Define terrestrial guard interval
typedef enum
{
    HAL_DEMOD_TER_GI_1_32,                                                        ///< Guard interval value = 1/32
    HAL_DEMOD_TER_GI_1_16,                                                        ///< Guard interval value = 1/16
    HAL_DEMOD_TER_GI_1_8,                                                         ///< Guard interval value = 1/8
    HAL_DEMOD_TER_GI_1_4                                                          ///< Guard interval value = 1/4
} HAL_DEMOD_EN_TER_GI_TYPE;

/// Define terrestrial transmission mode
typedef enum
{
    HAL_DEMOD_TER_FFT_2K,                                                         ///< 2k FFT mode
    HAL_DEMOD_TER_FFT_8K                                                          ///< 8k FFT mode
} HAL_DEMOD_EN_TER_FFT_MODE;

/// Define terrestrial transmission mode
typedef enum
{
    HAL_DEMOD_TER_HP_SEL,                                                         ///< High priority level selection
    HAL_DEMOD_TER_LP_SEL                                                          ///< Low priority level selection
} HAL_DEMOD_EN_TER_LEVEL_SEL;

/// Define DVB-C modulation scheme
typedef enum
{
    HAL_DEMOD_CAB_QAM16,                                                          ///< QAM 16
    HAL_DEMOD_CAB_QAM32,                                                          ///< QAM 32
    HAL_DEMOD_CAB_QAM64,                                                          ///< QAM 64
    HAL_DEMOD_CAB_QAM128,                                                         ///< QAM 128
    HAL_DEMOD_CAB_QAM256
} HAL_DEMOD_EN_CAB_CONSTEL_TYPE;


/// Define DVB-S IQ tuning mode
typedef enum
{
    HAL_DEMOD_CAB_IQ_NORMAL,                                                      ///< Normal
    HAL_DEMOD_CAB_IQ_INVERT                                                       ///< Inverse
} HAL_DEMOD_EN_CAB_IQ_MODE;


/// Define DVB-S modulatiopn scheme
typedef enum
{
    HAL_DEMOD_SAT_DVBS,
    HAL_DEMOD_SAT_DVBS2                                                          ///< DVBS2                                                           ///< DVBS
} HAL_DEMOD_EN_SAT_MOD_TYPE;

typedef enum
{
    HAL_DEMOD_SAT_QPSK,                                                           ///< QPSK
    HAL_DEMOD_SAT_8PSK,                                                           ///< 8PSK
    HAL_DEMOD_SAT_QAM16,                                                           ///< QAM16
    //HAL_DEMOD_SAT_16APSK,
    HAL_DEMOD_SAT_32APSK,
    HAL_DEMOD_SAT_8APSK,
    HAL_DEMOD_SAT_8_8APSK,
    HAL_DEMOD_SAT_4_8_4_16APSK
} HAL_DEMOD_EN_SAT_CONSTEL_TYPE;

/// Define DVB-S Roll-Off factor
typedef enum
{
    HAL_DEMOD_SAT_RO_35,                                                          ///< roll-off factor = 0.35
    HAL_DEMOD_SAT_RO_25,                                                          ///< roll-off factor = 0.25
    HAL_DEMOD_SAT_RO_20                                                           ///< roll-off factor = 0.20
} HAL_DEMOD_EN_SAT_ROLL_OFF_TYPE;

/// Define DVB-S IQ tuning mode
typedef enum
{
    HAL_DEMOD_SAT_IQ_NORMAL,                                                      ///< Normal
    HAL_DEMOD_SAT_IQ_INVERSE                                                      ///< Inverse
} HAL_DEMOD_EN_SAT_IQ_MODE;

/// Define Bit Error Rate range measure from signal
typedef enum
{
    HAL_DEMOD_BIT_ERR_RATIO_LOW ,                                                 ///< Low BER
    HAL_DEMOD_BIT_ERR_RATIO_MEDIUM ,                                              ///< Medium BER
    HAL_DEMOD_BIT_ERR_RATIO_HIGH                                                  ///< High BER
} HAL_DEMOD_EN_BIT_ERR_RATIO;

/// Define lock status of front end
typedef enum
{
    HAL_DEMOD_FE_UNLOCKED = 0,                                                    ///< Frontend is unlocked
    HAL_DEMOD_FE_LOCKED                                                           ///< Frontend is locked
} HAL_DEMOD_EN_FE_LOCK_STATUS;


/// Define tuning mode
/// NOTE: When this typedef is modified, the apiChScan should be rebuild.
typedef enum
{
    HAL_DEMOD_FE_TUNE_MANUAL,                                                     ///< Manual tuning to carrier
    HAL_DEMOD_FE_TUNE_AUTO,                                                       ///< Auto tuning to carrier
} HAL_DEMOD_EN_FE_TUNE_MODE;

/// Define output mode
/// NOTE: When this typedef is modified, the apiChScan should be rebuild.
typedef enum
{
    HAL_DEMOD_INTERFACE_SERIAL = 0,                                                   ///< Serial interface
    HAL_DEMOD_INTERFACE_PARALLEL                                                  ///< Parallel interface
} HAL_DEMOD_INTERFACE_MODE;

/// Define tuning paramter of DVB-T front-end
typedef struct
{
    HAL_DEMOD_EN_TER_BW_MODE                  eBandWidth;                         ///< Band width
    HAL_DEMOD_EN_TER_CONSTEL_TYPE             eConstellation;                     ///< Constellation type
    HAL_DEMOD_EN_TER_HIE_TYPE                 eHierarchy;                         ///< Hierarchy
    HAL_DEMOD_EN_TER_GI_TYPE                  eGuardInterval;                     ///< Guard interval
    HAL_DEMOD_EN_TER_FFT_MODE                 eFFT_Mode;                          ///< Transmission mode
    HAL_DEMOD_EN_CONV_CODE_RATE_TYPE          eHPCodeRate;                        ///< HP code rate
    HAL_DEMOD_EN_CONV_CODE_RATE_TYPE          eLPCodeRate;                        ///< LP code rate
    HAL_DEMOD_EN_TER_LEVEL_SEL                eLevelSel;                          ///< Select HP or LP level
} HAL_DEMOD_MS_TER_CARRIER_PARAM;

/// Define tuning paramter of DVB-C front-end
typedef struct
{
    HAL_DEMOD_EN_CAB_CONSTEL_TYPE   eConstellation;                     ///< Constellation type
    MS_U16                          u16SymbolRate;                      ///< Symbol rate (Ksym/sec)

    HAL_DEMOD_EN_CAB_IQ_MODE        eIQMode;                            ///< IQ Mode
    MS_U8                           u8TapAssign;                        ///< Tap assign
    MS_U32                          u32FreqOffset;                      ///< Carrier frequency offset
    MS_U8                           u8TuneFreqOffset;                       ///< Requeset tuner freq offset
} HAL_DEMOD_MS_CAB_CARRIER_PARAM;

/// Define tuning paramter of DVB-S front-end
typedef struct
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

} HAL_DEMOD_MS_SAT_CARRIER_PARAM;

/// Define carrier paramter of digital tuner
/// NOTE: When this typedef is modified, the apiChScan should be rebuild.
typedef struct
{
    MS_U32                            u32Frequency;
    union
    {
        HAL_DEMOD_MS_TER_CARRIER_PARAM        TerParam;                           ///< Paramters for DVB-T front-end
        HAL_DEMOD_MS_CAB_CARRIER_PARAM        CabParam;                           ///< Paramters for DVB-C front-end
        HAL_DEMOD_MS_SAT_CARRIER_PARAM        SatParam;                           ///< Paramters for DVB-S front-end
    };
} HAL_DEMOD_MS_FE_CARRIER_PARAM;

//--------------------------------------------------------------------
//--------------------------------------------------------------------
#ifdef UFO_PUBLIC_HEADER_212
typedef struct
{
    MS_U8        cmd_code;
    MS_U8        param[80];
} S_CMDPKTREG;

typedef enum
{
    TS_MODUL_MODE,
    TS_FFX_VALUE,
    TS_GUARD_INTERVAL,
    TS_CODE_RATE,

    TS_PARAM_MAX_NUM
}E_SIGNAL_TYPE;

typedef enum
{
    CMD_SYSTEM_INIT = 0,
    CMD_DAC_CALI,
    CMD_DVBT_CONFIG,
    CMD_DVBC_CONFIG,
    CMD_VIF_CTRL,
    CMD_FSM_CTRL,
    CMD_INDIR_RREG,
    CMD_INDIR_WREG,
    CMD_GET_INFO,
    CMD_TS_CTRL,
    CMD_TUNED_VALUE,

    CMD_MAX_NUM
}E_CMD_CODE;

typedef enum
{
    TS_PARALLEL = 0,
    TS_SERIAL = 1,

    TS_MODE_MAX_NUM
}E_TS_MODE;

typedef enum
{
    E_SYS_UNKOWN = -1,
    E_SYS_DVBT,
    E_SYS_DVBC,
    E_SYS_ATSC,
    E_SYS_VIF,
    E_SYS_DVBS,

    E_SYS_NUM
}E_SYSTEM;
#endif

/// Define the quality report
typedef struct
{
    HAL_DEMOD_EN_FE_LOCK_STATUS           eLock;                              ///< Lock
    HAL_DEMOD_EN_BIT_ERR_RATIO            eBER;                               ///< Bit error rate
    float                                 fSNR;                              	///< SNR
    float                                 fSignalLevel;                       ///< Signal Level=1~100
    MS_U16                                u16SignalStrength;                  ///< Signal Strength[dBm],mick
    MS_U8                                 u8SignalQuality;                    ///< Signal Quality,mick
    float                                 fPreBER;                            ///< xxE-xx,mick
    float                                 fPostBerTSBER;
    MS_U32                                u32LockTime;                        ///< LockTime
    MS_U16                                u16TSpacketError;                   ///< TS Packet Error
} HAL_DEMOD_MS_FE_CARRIER_STATUS;

typedef struct
{
        MS_BOOL                           bLNBPowerOn;                                              ///< Power On/Off
        MS_BOOL                           b22kOn;                                                           ///< LNB 22k On/Off
        MS_BOOL                           bLNBOutLow;                                                   ///< LNB 13/18V

} HAL_DEMOD_MS_FE_CARRIER_DISEQC;

/// Define the carrier information
typedef struct
{
    HAL_DEMOD_MS_FE_CARRIER_PARAM             Param;                              ///< Carrier parameter
    HAL_DEMOD_MS_FE_CARRIER_STATUS            Status;                             ///< Quality report
    HAL_DEMOD_MS_FE_CARRIER_DISEQC            DiSEqCp;                                                      ///< DiSEqC
} HAL_DEMOD_MS_FE_CARRIER_INFO;

typedef struct
{
    // Demodulator option
    MS_BOOL                         bX4CFE_en;                          ///< Carrier frequency estimation
    MS_BOOL                         bPPD_en;                            ///< Tap assign estimation
    MS_BOOL                         bIQAutoSwap_en;                     ///< IQ mode auto swap
    MS_BOOL                         bQAMScan_en;                        ///< QAM type auto scan
    MS_BOOL                         bFHO_en;                            ///< FHO
    MS_BOOL                         (*fptTunerSet)(MS_U32);             ///< Tuner set freq function pointer
} Hal_Demod_Mode;

 //d0: 1/4, d1: 1/3, d2: 2/5, d3: 1/2, d4: 3/5, d5: 2/3, d6: 3/4, d7: 4/5, d8: 5/6, d9: 8/9, d10: 9/10,
 //d11: 2/9, d12: 13/45, d13: 9/20, d14: 90/180, d15: 96/180, d16: 11/20, d17: 100/180, d18: 104/180, d19: 26/45
 //d20: 18/30, d21: 28/45, d22: 23/36, d23: 116/180, d24: 20/30, d25: 124/180, d26: 25/36, d27: 128/180,, d28: 13/18
 //d29: 132/180, d30: 22/30, d31: 135/180, d32: 140/180, d33: 7/9, d34: 154/180
 //d35: 11/45, d36: 4/15, d37: 14/45, d38: 7/15, d39: 8/15, d40: 26/45, d41: 32/45

typedef enum
{
    DMD_CONV_CODE_RATE_1_2,                                                 ///< Code rate = 1/2
    DMD_CONV_CODE_RATE_1_3,                                                 ///< Code rate = 1/3
    DMD_CONV_CODE_RATE_2_3,                                                 ///< Code rate = 2/3
    DMD_CONV_CODE_RATE_1_4,                                                 ///< Code rate = 1/4
    DMD_CONV_CODE_RATE_3_4,                                                 ///< Code rate = 3/4
    DMD_CONV_CODE_RATE_2_5,                                                 ///< Code rate = 2/5
    DMD_CONV_CODE_RATE_3_5,                                                 ///< Code rate = 3/5
    DMD_CONV_CODE_RATE_4_5,                                                 ///< Code rate = 4/5
    DMD_CONV_CODE_RATE_5_6,                                                 ///< Code rate = 5/6
    DMD_CONV_CODE_RATE_7_8,                                                 ///< Code rate = 7/8
    DMD_CONV_CODE_RATE_8_9,                                                 ///< Code rate = 8/9
    DMD_CONV_CODE_RATE_9_10,                                                 ///< Code rate = 9/10

    // DVBS2X

    DMD_DVBS2_CODE_RATE_2_9,
    DMD_DVBS2_CODE_RATE_13_45,
    DMD_DVBS2_CODE_RATE_9_20,
    DMD_DVBS2_CODE_RATE_90_180,
    DMD_DVBS2_CODE_RATE_96_180,
    DMD_DVBS2_CODE_RATE_11_20,
    DMD_DVBS2_CODE_RATE_100_180,
    DMD_DVBS2_CODE_RATE_104_180,
    DMD_DVBS2_CODE_RATE_26_45_L,
    DMD_DVBS2_CODE_RATE_18_30,
    DMD_DVBS2_CODE_RATE_28_45,
    DMD_DVBS2_CODE_RATE_23_36,
    DMD_DVBS2_CODE_RATE_116_180,
    DMD_DVBS2_CODE_RATE_20_30,
    DMD_DVBS2_CODE_RATE_124_180,
    DMD_DVBS2_CODE_RATE_25_36,
    DMD_DVBS2_CODE_RATE_128_180,
    DMD_DVBS2_CODE_RATE_13_18,
    DMD_DVBS2_CODE_RATE_132_180,
    DMD_DVBS2_CODE_RATE_22_30,
    DMD_DVBS2_CODE_RATE_135_180,
    DMD_DVBS2_CODE_RATE_140_180,
    DMD_DVBS2_CODE_RATE_7_9,
    DMD_DVBS2_CODE_RATE_154_180,
    DMD_DVBS2_CODE_RATE_11_45,
    DMD_DVBS2_CODE_RATE_4_15,
    DMD_DVBS2_CODE_RATE_14_45,
    DMD_DVBS2_CODE_RATE_7_15,
    DMD_DVBS2_CODE_RATE_8_15,
    DMD_DVBS2_CODE_RATE_26_45_S,
    DMD_DVBS2_CODE_RATE_32_45
} DMD_DVBS_CODE_RATE_TYPE;

#ifdef UFO_DEMOD_DVBS_SUPPORT_DISEQC_RX
typedef struct
{
    MS_U8 u8_delay_before_cmd; //unit: 1ms , add delay before diseqc command;  Recommended value is 0x0F
    MS_U8 u8_delay_after_cmd;   //unit: 1ms , add delay after diseqc command ; Recommended value is 0x02
    MS_U8 u8_fcar; //please set this to " 0x88 ", do not change it arbitrarily. Only for demod team used.; Recommended value is 0x88
    MS_U8  u8_dummy1;                  //future use
}HAL_DEMOD_DISEQC2_TX_SETTING;
#endif

typedef enum
{
    VCM_Disabled = 0,
    VCM_MODE,
    VCM_Forced_Mode
}DMD_DVBS_VCM_OPT;

//typedef void(*P_DMD_ISR_Proc)(MS_U8 u8DMDID);
//---------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBS_Init
////////////////////////////////////////////////////////////////////////////////

extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBS_Init_KERNEL(DMD_DVBS_InitData_KERNEL*pDMD_DVBS_InitData, MS_U32 u32InitDataLen);

extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBS_Init(DMD_DVBS_InitData *pDMD_DVBS_InitData, MS_U32 u32InitDataLen);
////////////////////////////////////////////////////////////////////////////////
/// Should be called when exit VD input source
////////////////////////////////////////////////////////////////////////////////
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBS_Exit(void);
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
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBS_SetDbgLevel(DMD_DVBS_DbgLv u8DbgLevel);
//-------------------------------------------------------------------------------------------------
/// Get the information of DVBT driver\n
/// @return the pointer to the driver information
//-------------------------------------------------------------------------------------------------
extern DLL_PUBLIC const DMD_DVBS_Info* MDrv_DMD_DVBS_GetInfo(void);
//-------------------------------------------------------------------------------------------------
/// Get DVBT driver version
/// when get ok, return the pointer to the driver version
//-------------------------------------------------------------------------------------------------

///////////////////////////////////////////////////////////////////////////////////
///To get/set the DSP parameter table from demod MCU
//u16Addr     :the address of the demod MCU DSP parameter table
//////////////////////////////////////////////////////////////////////////////////
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBS_GetDSPReg(MS_U16 u16Addr, MS_U8 *pu8Data);
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBS_SetDSPReg(MS_U16 u16Addr, MS_U8 pu8Data);

extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBS_GetLibVer(const MSIF_Version **ppVersion);

////////////////////////////////////////////////////////////////////////////////
/// To get DVBT's register  value, only for special purpose.\n
/// u16Addr       : the address of DVBT's register\n
/// return the value of AFEC's register\n
////////////////////////////////////////////////////////////////////////////////
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBS_GetReg(MS_U16 u16Addr, MS_U8 *pu8Data);
////////////////////////////////////////////////////////////////////////////////
/// To set DVBT's register value, only for special purpose.\n
/// u16Addr       : the address of DVBT's register\n
/// u8Value        : the value to be set\n
////////////////////////////////////////////////////////////////////////////////
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBS_GetFWVer(MS_U16 *ver);
////////////////////////////////////////////////////////////////////////////////
/// Get DVBS FW version
/// u16Addr       : the address of DVBS's register\n
////////////////////////////////////////////////////////////////////////////////
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBS_SetReg(MS_U16 u16Addr, MS_U8 u8Data);
////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBT_SetSerialControl
////////////////////////////////////////////////////////////////////////////////
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBS_SetSerialControl(MS_BOOL bEnable);
////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBT_SetConfig
////////////////////////////////////////////////////////////////////////////////
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBS_SetConfig(MS_U32 u32SymbolRate, DMD_DVBS_MODULATION_TYPE eQamMode, MS_U32 u32IFFreq, MS_BOOL bSpecInv, MS_BOOL bSerialTS);
////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBT_SetConfig_symbol_rate_list
////////////////////////////////////////////////////////////////////////////////
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBS_SetConfig_symbol_rate_list(MS_U32 u32SymbolRate, DMD_DVBS_MODULATION_TYPE eQamMode, MS_U32 u32IFFreq, MS_BOOL bSpecInv, MS_BOOL bSerialTS, MS_U16 *pu16_symbol_rate_list,MS_U8 u8_symbol_rate_list_num);
////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBS_BlindScanConfig
////////////////////////////////////////////////////////////////////////////////

extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBS_BlindScan_Config(MS_U32 u32SymbolRate, DMD_DVBS_MODULATION_TYPE eQamMode, MS_U32 u32IFFreq, MS_BOOL bSpecInv, MS_BOOL bSerialTS);
////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBS_BlindScanConfig_symbol_rate_list
////////////////////////////////////////////////////////////////////////////////
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBS_BlindScan_Config_symbol_rate_list(MS_U32 u32SymbolRate, DMD_DVBS_MODULATION_TYPE eQamMode, MS_U32 u32IFFreq, MS_BOOL bSpecInv, MS_BOOL bSerialTS, MS_U16 *pu16_symbol_rate_list,MS_U8 u8_symbol_rate_list_num);

////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBT_SetActive
////////////////////////////////////////////////////////////////////////////////
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBS_SetActive(MS_BOOL bEnable);
////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBT_Get_Lock
////////////////////////////////////////////////////////////////////////////////
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBS_GetLock(DMD_DVBS_GETLOCK_TYPE eType, DMD_DVBS_LOCK_STATUS *eLockStatus);
////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBS_GetParam
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBS_GetLockWithRFPower
////////////////////////////////////////////////////////////////////////////////
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBS_GetLockWithRFPower(DMD_DVBS_GETLOCK_TYPE eType, DMD_DVBS_LOCK_STATUS *eLockStatus, float fCurrRFPowerDbm, float fNoChannelRFPowerDbm);
////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBS_GetTunrSignalLevel_PWR
////////////////////////////////////////////////////////////////////////////////
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBS_GetTunrSignalLevel_PWR(float *fPowerLevel);
////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBT_GetSignalStrength
////////////////////////////////////////////////////////////////////////////////
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBS_GetSignalStrength(MS_U16 *u16Strength);
////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBS_GetSignalStrengthWithRFPower
////////////////////////////////////////////////////////////////////////////////
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBS_GetSignalStrengthWithRFPower(MS_U16 *u16Strength, float fRFPowerDbm);
////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBS_GetSignalQuality
////////////////////////////////////////////////////////////////////////////////
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBS_GetSignalQuality(MS_U16 *u16Quality);
////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBS_GetSignalQualityWithRFPower
////////////////////////////////////////////////////////////////////////////////
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBS_GetSignalQualityWithRFPower(MS_U16 *u16Quality, float fRFPowerDbm);
////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBS_GetSNR
////////////////////////////////////////////////////////////////////////////////
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBS_GetSNR(float *fSNR);
////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBS_GetPostViterbiBer
////////////////////////////////////////////////////////////////////////////////
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBS_GetPostViterbiBer(float *ber);
#ifdef UFO_DEMOD_DVBS_BERINFO
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBS_GetPreLDPCBer(float *ber);
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBS_GetPostLDPCBer(float *ber);
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBS_GetBer(DMD_DVBS_DEMOD_BER_TYPE eType, float *ber);
#endif

extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBS_TS_Enable(MS_BOOL bEnable);
////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBS_GetPacketErr
////////////////////////////////////////////////////////////////////////////////
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBS_GetPacketErr(MS_U16 *pktErr);
////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBT_GetCellID
////////////////////////////////////////////////////////////////////////////////
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBS_GetCellID(MS_U16 *u16CellID);

extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBS_GetParam(HAL_DEMOD_MS_SAT_CARRIER_PARAM *pParam);

////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBS_GetAGCInfo
////////////////////////////////////////////////////////////////////////////////
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBS_GetAGCInfo(MS_U8 u8dbg_mode, MS_U16 *pu16Data);
////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBS_GetStatus
////////////////////////////////////////////////////////////////////////////////
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBS_GetStatus(DMD_DVBS_MODULATION_TYPE *pQAMMode, MS_U32 *u32SymbolRate, float *pFreqOff);

extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBS_ActiveDmdSwitch(MS_U8 demod_no);

extern DLL_PUBLIC MS_U32 MDrv_DMD_DVBS_SetPowerState(EN_POWER_MODE u16PowerState);

extern DLL_PUBLIC MS_U32 MDrv_DMD_DVBS_Demod_Restart(MS_U32 u32SymbolRate, MS_U32 u32Frequency);

extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBS_Get_FreqOffset(float *pFreqOff, MS_U8 u8BW);
////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBS_BlindScan Function
////////////////////////////////////////////////////////////////////////////////
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBS_BlindScan_Start(MS_U16 u16StartFreq, MS_U16 u16EndFreq);

extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBS_BlindScan_NextFreq(MS_BOOL* bBlindScanEnd);

extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBS_BlindScan_Cancel(void);

extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBS_BlindScan_End(void);

extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBS_BlindScan_GetChannel(MS_U16 u16ReadStart, MS_U16* u16TPNum, HAL_DEMOD_MS_FE_CARRIER_PARAM *pTable);

extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBS_BlindScan_GetCurrentFreq(MS_U32 *u32CurrentFeq);

extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBS_BlindScan_WaitCurFreqFinished(MS_U8* u8Progress,MS_U8 *u8FindNum);

extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBS_BlindScan_GetTunerFreq_EX(MS_U16 *u16TunerCenterFreq, MS_U16 *u16TunerCutOffFreq);
#define Temp_func2(a,b,...) MDrv_DMD_DVBS_BlindScan_GetTunerFreq_EX(a,b)
#define Temp_func1(...) Temp_func2(__VA_ARGS__)
#define MDrv_DMD_DVBS_BlindScan_GetTunerFreq(...) Temp_func1(__VA_ARGS__,NULL)




////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBS_DiSEqC Function
////////////////////////////////////////////////////////////////////////////////
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBS_DiSEqC_Init(void);

extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBS_DiSEqC_SetLNBOut(MS_BOOL bLow);

extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBS_DiSEqC_GetLNBOut(MS_BOOL* bLNBOutLow);

extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBS_DiSEqC_Set22kOnOff(MS_BOOL b22kOn);

extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBS_DiSEqC_Get22kOnOff(MS_BOOL* b22kOn);

extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBS_DiSEqC_SendCmd(MS_U8* pCmd,MS_U8 u8CmdSize);


//extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBS_DiSEqC_ReceiveCmd(MS_U8* pCmd,MS_U8* u8CmdSize);

#ifdef UFO_DEMOD_DVBS_SUPPORT_DISEQC_RX
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBS_DiSEqC2_TX_SendCmd(MS_U8* pCmd,MS_U8 u8CmdSize, HAL_DEMOD_DISEQC2_TX_SETTING txsetting );
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBS_DiSEqC_ReceiveCmd_GetBoardinfo(MS_BOOL envelop_mode, MS_BOOL invers_mode);
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBS_DiSEqC_ReceiveCmd(MS_U8* pCmd,MS_U8* u8CmdSize);
#endif

#ifdef UFO_DEMOD_DVBS_CUSTOMIZED_DISEQC_SEND_CMD
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBS_Customized_DiSEqC_SendCmd(MS_U8* pCmd,MS_U8 u8CmdSize);
#endif

extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBS_DiSEqC_SetTxToneMode(MS_BOOL bTxTone22kOff);

extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBS_DiSEqC_SetTone(MS_BOOL bTone1);

extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBS_UnicableAGCCheckPower(MS_BOOL pbAGCCheckPower);


MS_BOOL MDrv_DMD_DVBS_Reg_INT_CB(fpIntCallBack fpCBReg);


//Terry add
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBS2_TS_DivNum_Calculation(void);

extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBS2_Set_IS_ID(MS_U8 u8IS_ID);

extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBS2_Set_Default_IS_ID(MS_U8 *u8IS_ID_table);

extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBS2_Get_IS_ID_INFO(MS_U8 *u8IS_ID, MS_U8 *u8IS_ID_table);

extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBS2_VCM_Init(DMD_DVBS_VCM_OPT u8VCM_OPT, MS_U8 u8IS_ID, MS_U32 u32DVBS2_DJB_START_ADDR);

extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBS2_VCM_Check(void);

extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBS2_VCM_Enabled(MS_U8 u8VCM_ENABLED);

extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBS2_VCM_Change_Mode(DMD_DVBS_VCM_OPT u8VCM_OPT);

////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBS_Kernel function
////////////////////////////////////////////////////////////////////////////////
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBS_GetSNR_Kernel(MS_FLOAT_ST *fSNR);

extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBS_GetBER_Kernel(MS_FLOAT_ST *ber);

extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBS_TS_DivNum_Calculation_Kernel(void);

#ifdef CONFIG_UTOPIA_PROC_DBG_SUPPORT
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBS_GetDEMODMdbInfo(MS_U64 *u64ReqHdl);
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBS_SetMdb(MS_U64* pu64ReqHdl, MS_U32 u32CmdSize, char* pcmd);
#endif
#ifdef __cplusplus
}
#endif


#endif // _DRV_DVBS_H_

