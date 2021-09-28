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

#ifndef MSAPI_TUNER_H
#define MSAPI_TUNER_H

//#include "debug.h"
//#include <drvVIF.h>
#include <tuner/Tuner.h>
#include <ShareType.h>
#include <apiDMX.h>
#if 1//((CONFIG_COMMANDS&(CFG_CMD_TUNER | CFG_CMD_DEMOD)) == (CFG_CMD_TUNER | CFG_CMD_DEMOD))

/******************************************************************************/
/*                 Macro                                                      */
/******************************************************************************/
#define FE_NOT_LOCK     0
#define FE_LOCK         1
#define FE_AGC_NOT_LOCK 2

//typedef U8 BOOLEAN;

typedef enum
{
    E_RESULT_SUCCESS = 0,                 ///< Result Success
    E_RESULT_FAILURE,                     ///< Result Failure
    E_RESULT_INVALID_PARAMETER,         ///< Result Invalid Parameter
    E_RESULT_OUTOF_MEMORY,               ///< Result Out of memory
    E_RESULT_OUTOF_EEPROM,               ///< Result Out of EEPROM
    E_RESULT_COMMUNICATION_ERROR,       ///< Result Communication Error
    E_RESULT_SAVE_TO_EEPROM_FAIL,       ///< Result Save to EEPROM fail
    E_RESULT_NOTHING,                    ///< Result Nothing
    E_RESULT_ILLEGAL_ACCESS,            ///< Result Illegal access
    E_RESULT_UNSUPPORTED                ///< Result unsupported
} FUNCTION_RESULT;


typedef enum
{
    DEMOD_MODE_PAL,
    DEMOD_MODE_DVB,
    DEMOD_MODE_NUM
} EN_DEMOD_MODE;

typedef enum
{
    PAL_SCAN,
    DVB_SCAN,
} EN_SCAN_MODE;

typedef enum
{
    DEMOD_SPECTRUM_NORMAL,
    DEMOD_SPECTRUM_INVERTER,
    DEMOD_SPECTRUM_AUTO
} EN_DEMOD_SPECTRUM_MODE;

typedef enum
{
    SIGNAL_NO = 0,          // little or no input power detected
    SIGNAL_WEAK,            // some power detected.
    SIGNAL_MODERATE,        // lock achieved, SNR < 15 dB (approx)
    SIGNAL_STRONG,          // lock achieved, SNR < 24 dB (approx)
    SIGNAL_VERY_STRONG,     // lock achieved, SNR > 24 dB (approx)
} EN_SIGNAL_CONDITION;

typedef enum
{
    STATE_VSB_CHECK_INIT,
    STATE_VSB_CHECK_CR_LOCK,
    STATE_VSB_CHECK_WAIT_INIT,
    STATE_VSB_CHECK_SYNC_LOCK,
    STATE_VSB_CHECK_SNR,
} EN_VSB_CHECK_LOCK_STATE;

typedef enum
{
    STATE_QAM_CHECK_INIT,
    STATE_QAM_CHECK_WAIT_INIT,
    STATE_QAM_CHANGE_MODE,
    STATE_QAM_CHECK_FEC_LOCK,
    STATE_QAM_CHECK_MPEG_LOCK,
} EN_QAM_CHECK_LOCK_STATE;

typedef enum
{
    STATE_VSB_STABLE_INIT,
    STATE_VSB_STABLE_WAIT_INIT,
    STATE_VSB_STABLE_SYNC_LOCK,
} EN_VSB_CHECK_STABLE_STATE;
typedef enum
{
    EN_DVB_T_TYPE=0,
    EN_DVB_C_TYPE
} EN_DVB_TYPE;
/// TP Setting
#if ((FRONTEND_DEMOD_TYPE == MSTAR_MSB1245_DEMOD)||(FRONTEND_DEMOD_TYPE == EMBEDDED_DVBS_DEMOD))
typedef struct
{
	U32 u32Frequency;
    RF_CHANNEL_BANDWIDTH enBandWidth;
    U8 u8PLPID;
    U8 u8HpLp;
    U32 u32Symbol_rate;
} MS_TP_SETTING;
#else
typedef struct
{
    U32 u32Frequency; ///< Frequency, 50,000 ~ 860,000 Khz
    RF_CHANNEL_BANDWIDTH enBandWidth;   ///< Bandwidth
    U8 u8PLPID;
    U8 u8HpLp;
#if (DVB_C_ENABLE || (FRONTEND_DEMOD_TYPE == EMBEDDED_DVBC_DEMOD))
    U8 u8Modulation;
    U32 u32Symbol_rate;
    BOOLEAN bAutoSRFlag;
    BOOLEAN bAutoQamFlag;
#endif
} MS_TP_SETTING;
#endif

#if ((FRONTEND_DEMOD_TYPE == MSTAR_MSB123x_DEMOD) || (FRONTEND_DEMOD_TYPE == MSTAR_MSB1233c_DEMOD)||(FRONTEND_DEMOD_TYPE == EMBEDDED_DVBC_DEMOD)||(FRONTEND_DEMOD_TYPE == EMBEDDED_DVBT2_DEMOD)||(FRONTEND_DEMOD_TYPE == EMBEDDED_DVBS_DEMOD)||(FRONTEND_DEMOD_TYPE == MSTAR_MSB1238_DEMOD)||(FRONTEND_DEMOD_TYPE == MSTAR_MSB124x_DEMOD)||(FRONTEND_DEMOD_TYPE == MSTAR_MSB1245_DEMOD)  \
    || (FRONTEND_DEMOD_TYPE == MSTAR_MSB123xc_DEMOD) || (FRONTEND_DEMOD_TYPE == MSTAR_MSB1400_DEMOD))
typedef enum
{
    E_DEVICE_DEMOD_NULL,
    E_DEVICE_DEMOD_DVB_T,
    E_DEVICE_DEMOD_DVB_T2,
    E_DEVICE_DEMOD_DVB_C,
    E_DEVICE_DEMOD_DVB_S,
    E_DEVICE_DEMOD_ISDB
}EN_DEVICE_DEMOD_TYPE;
#endif

#if ((FRONTEND_DEMOD_TYPE == MSTAR_MSB123x_DEMOD) || (FRONTEND_DEMOD_TYPE == MSTAR_MSB123xc_DEMOD) || (FRONTEND_DEMOD_TYPE == MSTAR_MSB1400_DEMOD))
typedef enum
{
    // operation mode settings
    T_OPMODE_RFAGC_EN  = 0x20,   // 0x20
    T_OPMODE_HUMDET_EN,
    T_OPMODE_DCR_EN,
    T_OPMODE_IIS_EN,
    T_OPMODE_CCI_EN,
    T_OPMODE_ACI_EN,
    T_OPMODE_IQB_EN,
    T_OPMODE_AUTO_IQ,
    T_OPMODE_AUTO_RFMAX,    // 0x28
    T_OPMODE_AUTO_ACI,
    T_OPMODE_FIX_MODE_CP,
    T_OPMODE_FIX_TPS,
    T_OPMODE_AUTO_SCAN,
    T_OPMODE_RSV_0X2D,
    T_OPMODE_RSV_0X2E,
    T_OPMODE_RSV_0X2F,

    // channel config param
    T_CONFIG_RSSI,    // 0x30
    T_CONFIG_ZIF,
    T_CONFIG_FREQ,
    T_CONFIG_FC_L,
    T_CONFIG_FC_H,
    T_CONFIG_FS_L,
    T_CONFIG_FS_H,
    T_CONFIG_BW,
    T_CONFIG_MODE,    // 0x38
    T_CONFIG_CP,
    T_CONFIG_LP_SEL,
    T_CONFIG_CSTL,
    T_CONFIG_HIER,
    T_CONFIG_HPCR,
    T_CONFIG_LPCR,
    T_CONFIG_IQ_SWAP,
    T_CONFIG_RFMAX,    // 0x40
    T_CONFIG_CCI,
    T_CONFIG_ICFO_RANGE,
    T_CONFIG_RFAGC_REF,
    T_CONFIG_IFAGC_REF_2K,
    T_CONFIG_IFAGC_REF_8K,
    T_CONFIG_IFAGC_REF_ACI,
    T_CONFIG_IFAGC_REF_IIS_2K,
    T_CONFIG_IFAGC_REF_IIS_8K,    // 0x48
    T_CONFIG_ACI_DET_TH_L,
    T_CONFIG_ACI_DET_TH_H,
    T_CONFIG_TS_SERIAL,
    T_CONFIG_TS_CLK_RATE,
    T_CONFIG_TS_OUT_INV,
    T_CONFIG_TS_DATA_SWAP,
    T_CONFIG_2K_SFO_H,
    T_CONFIG_2K_SFO_L,    // 0x50
    T_CONFIG_8K_SFO_H,
    T_CONFIG_8K_SFO_L,
    T_CONFIG_CHECK_CHANNEL,
    T_CONFIG_SLICER_SNR_POS,
    T_CONFIG_TDP_CCI_KP,
    T_CONFIG_CCI_FSWEEP,
    T_CONFIG_TS_CLK_RATE_AUTO,

    /**********************
     * crc =
     *    ~(T_OPMODE_RFAGC_EN^T_OPMODE_HUMDET_EN^....^T_CONFIG_TS_CLK_RATE_AUTO)
     ************************/
    T_PARAM_CHECK_SUM,

    DVBT_PARAM_LEN,
} DVBT_Param;

typedef enum                            // BW: 0->1.7M, 1->5M, 2->6M, 3->7M, 4->8M, 5->10M
{
    E_DEMOD_BW_17M    = 0x0,
    E_DEMOD_BW_5M      = 0x1,
    E_DEMOD_BW_6M      = 0x2,
    E_DEMOD_BW_7M      = 0x3,
    E_DEMOD_BW_8M      = 0x4,
    E_DEMOD_BW_10M    = 0x5,
}E_DEMOD_CHANNEL_BANDWIDTH;

typedef enum
{
    E_DEMOD_LOCK,
    E_DEMOD_CHECKING,
    E_DEMOD_CHECKEND,
    E_DEMOD_UNLOCK,
    E_DEMOD_NULL,
} EN_LOCK_STATUS;

typedef enum
{
    E_FE_SIGNAL_NO = 0,
    E_FE_SIGNAL_WEAK,
    E_FE_SIGNAL_MODERATE,
    E_FE_SIGNAL_STRONG,
    E_FE_SIGNAL_VERY_STRONG,
} EN_FRONTEND_SIGNAL_CONDITION;
#endif

#if (FRONTEND_DEMOD_TYPE == MSTAR_MSB122x_DEMOD)
typedef enum
{
    E_DEMOD_LOCK,
    E_DEMOD_CHECKING,
    E_DEMOD_CHECKEND,
    E_DEMOD_UNLOCK,
    E_DEMOD_NULL,
} EN_LOCK_STATUS;
#endif

#ifdef MSAPI_TUNER_C
#define INTERFACE
#else
#define INTERFACE extern
#endif

INTERFACE void msAPI_Tuner_InintCurrentTPSetting(void);
INTERFACE int msAPI_Tuner_Initialization(BOOLEAN bSrcChg);
INTERFACE void msAPI_Tuner_InitExternDemod(void);
INTERFACE void msAPI_Tuner_ZigZagScan(void);
INTERFACE void msAPI_Tuner_Tune2RfCh(MS_TP_SETTING *pstTPSetting);
INTERFACE BOOLEAN msAPI_Tuner_IsSameRfChannel(MS_TP_SETTING *pstTPSetting);
INTERFACE void msAPI_Tuner_PowerOnOff(BOOLEAN bPower);
INTERFACE U8 msAPI_Tuner_GetSingalSNRPercentage(void);
INTERFACE MS_U16 msAPI_Tuner_GetSignalQualityPercentage(void);
INTERFACE BOOLEAN msAPI_Tuner_GetBER(float *ber);
INTERFACE BOOLEAN msAPI_Tuner_GetSignalModulMode(MS_U16 *wModul_Mode);
INTERFACE void msAPI_Check_Lock_State_Init(void);
INTERFACE BOOLEAN msAPI_Tuner_CheckLock(BOOLEAN *CheckLockResult,BOOLEAN fScan);
INTERFACE void msAPI_Demodulator_Exit(void);
INTERFACE BOOLEAN msAPI_Demodulator_Reset(void);
INTERFACE void msAPI_Tuner_Serial_Control(BOOLEAN bParallelMode);
INTERFACE FUNCTION_RESULT msAPI_Tuner_CheckSignalStrength(MS_U16 *wStrength);
INTERFACE BOOLEAN msAPI_Tuner_Get_CELL_ID( MS_U16 * cell_id);
INTERFACE BOOLEAN msAPI_Tuner_Get_PLP_ID( U8 * pu8plp_id);
INTERFACE U8 msAPI_Tuner_Get_HpLp( void);
INTERFACE BOOLEAN msAPI_Tuner_Is_HierarchyOn(void);
INTERFACE FUNCTION_RESULT msAPI_Tuner_GetCurTPSetting(MS_TP_SETTING *pstCurTPSetting);



INTERFACE void msAPI_Tuner_SwitchSource(EN_DVB_TYPE etype, BOOLEAN bInit);
INTERFACE EN_DVB_TYPE msAPI_Tuner_GetSourceType(void);
INTERFACE void msAPI_Tuner_SetAntenna(BOOLEAN bIsCATV);
INTERFACE DMX_FILTER_STATUS msAPI_Tuner_SetByPassMode(BOOLEAN bByPass, BOOLEAN bPVRMode);
INTERFACE BOOLEAN msAPI_Tuner_IsParallelMode(void);
INTERFACE BOOLEAN msAPI_Tuner_IsByPassMode(void);
INTERFACE U8 msAPI_Tuner_GetDspStatus(void);
#if (DVB_C_ENABLE)
INTERFACE void msAPI_Tuner_UpdateTPSetting(void);
#endif
#if ((FRONTEND_DEMOD_TYPE == MSTAR_MSB123x_DEMOD) || (FRONTEND_DEMOD_TYPE == MSTAR_MSB1233c_DEMOD) \
    ||(FRONTEND_DEMOD_TYPE == MSTAR_MSB1233c_DEMOD) || (FRONTEND_DEMOD_TYPE == MSTAR_MSB123xc_DEMOD))
INTERFACE void msAPI_GetPlpBitMap(U8* u8PlpBitMap);
INTERFACE BOOLEAN msAPI_GetPlpGroupID(U8 u8PlpID, U8* u8GroupID);
INTERFACE BOOLEAN msAPI_SetPlpGroupID(U32 u32PlpID, U32 u32GroupID);
INTERFACE BOOLEAN msAPI_SetDemodType(EN_DEVICE_DEMOD_TYPE enDemodType);
INTERFACE EN_DEVICE_DEMOD_TYPE msAPI_GetDemodType(void);
#endif

#if ENABLE_DVB_T2_SYS
INTERFACE MS_U8 msAPI_GetPlpIDList(void);
INTERFACE MS_U8 msAPI_GetNextPlpID(MS_U8 u8Index);
INTERFACE MS_BOOL msAPI_SetPlpID(MS_U8 u8PlpID);
#endif

INTERFACE BOOLEAN msAPI_InitFrontend(EN_DEVICE_DEMOD_TYPE enDemodType, MS_U32 u32Frequency, MS_U8 u8BandWidth);
#undef INTERFACE

#define ENABLE_SCAN_ONELINE_MSG 0
#endif
#endif // MSAPI_TUNER_H

