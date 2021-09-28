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

#ifndef _EXTERN_DVBS_H
#include "MsCommon.h"
#include "drvDMD_common.h"

#define _EXTERN_DVBS_H

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum
{
    Pre_LDPC = 0x00,
    Post_Viterbi,
    Post_LDPC,
}E_BER_type;

typedef enum
{
/*Fundmental Info*/
    E_S2_TS_SERIAL= 0x00,
    E_S2_FW_VERSION_L,
    E_S2_FW_VERSION_H,
    E_S2_CHIP_VERSION,
    E_S2_MANUAL_TUNE_SYMBOLRATE_0,
    E_S2_MANUAL_TUNE_SYMBOLRATE_1,
    E_S2_MANUAL_TUNE_SYMBOLRATE_2,

/*DIRECTV Info*/    
    SCR_X_SEED_0,
    SCR_X_SEED_1,
    SCR_X_SEED_2,
    PLHeader_Seq_0,
    PLHeader_Seq_1,
    PLHeader_Seq_2,
    PLHeader_Seq_3,
    PLHeader_Seq_4,
    PLHeader_Seq_5,
    PLHeader_Seq_6,
    PLHeader_Seq_7,
    PLHeader_Seq_8,
    PLHeader_Seq_9,
    PLHeader_Seq_10,
    PLHeader_Seq_11,

/*Diseqc*/
    E_S2_DISEQC_RX_LENGTH,
    E_S2_DISEQC_INTERRUPT_FLAG,
    E_S2_DISEQC_RX_FLAG,
    E_S2_DISEQC_INTERRUPT_STATUS,
    E_S2_DISEQC_STATUS_FLAG,

/*TP Info*/
    E_S2_SYSTEM_TYPE,    
    E_S2_MOD_TYPE,    
    E_S2_CODE_RATE,
    E_S2_FEC_TYPE,
    E_S2_PILOT_FLAG,
    E_S2_SNR_0,
    E_S2_SNR_1,

/*Blind Scan*/
    E_S2_NARROW_STEP_FLAG,
    E_S2_NO_SIGNAL_FLAG,    
    E_S2_BLINDSCAN_CHECK,
    E_S2_BLINDSCAN_DBG,    
    E_S2_TF_LOCKED0,
    E_S2_TF_LOCKED1,
    E_S2_TF_LOCKED2,
    E_S2_SR_LOCKED0,
    E_S2_SR_LOCKED1,
    E_S2_SR_LOCKED2,        
}E_DVBS2_PARAM;

typedef enum
{
    E_DMD_DVBS_DEMOD_I2C_DYNAMIC_SLAVE_ID_1,
    E_DMD_DVBS_DEMOD_I2C_DYNAMIC_SLAVE_ID_2,
    E_DMD_DVBS_DEMOD_I2C_DYNAMIC_SLAVE_ID_3,
    E_DMD_DVBS_DEMOD_I2C_DYNAMIC_SLAVE_ID_4
} eDMD_DVBS_DemodI2CSlaveID;

typedef enum
{
    E_DMD_DVBS_DEMOD_I2C_READ_BYTES,
    E_DMD_DVBS_DEMOD_I2C_WRITE_BYTES
} eDMD_DVBS_DemodI2CMethod;

/// For demod init
typedef struct
{
    MS_BOOL bPreloadDSPCodeFromMainCHIPI2C;
    MS_BOOL (*fpDVBS_I2C_Access)(eDMD_DVBS_DemodI2CSlaveID eSlaveID, eDMD_DVBS_DemodI2CMethod eMethod, MS_U8 u8AddrSize, MS_U8 *pu8Addr, MS_U16 u16Size, MS_U8 *pu8Data);
    MS_BOOL bEnableSPILoadCode;
    void (*fpDVBS_SPIPAD_En)(MS_BOOL bOnOff);
} sDMD_EXTERN_DVBS_InitData;

#if 0
/*================================================
DVBS External DEMOD fundmental function
=================================================*/
#endif
MS_BOOL MDrv_DMD_EXTERN_DVBS_Init(sDMD_EXTERN_DVBS_InitData *pDMD_DVBS_EXTERN_InitData, MS_U32 u32InitDataLen);
MS_BOOL MDrv_DMD_EXTERN_DVBS_Exit(void);
MS_BOOL MDrv_DMD_EXTERN_DVBS_LoadDSPCodeToSram(void);
MS_BOOL MDrv_DMD_EXTERN_DVBS_AGC_PAD_SEL(MS_BOOL sel);
MS_BOOL MDrv_DMD_EXTERN_DVBS_SetCurrentDemodulatorType(void);
MS_BOOL MDrv_DMD_EXTERN_DVBS_Get_Lock(MS_U8 *lock_type, MS_BOOL *lock_status);
MS_BOOL MDrv_DMD_EXTERN_DVBS_TS_Enable(MS_BOOL bTsEnable);
MS_BOOL MDrv_DMD_EXTERN_DVBS_DEMOD_Restart(MS_U32 u32SymbolRate);
MS_BOOL MDrv_DMD_EXTERN_DVBS_Driving_Control(MS_BOOL bEnable);
MS_BOOL MDrv_DMD_EXTERN_DVBS_Serial_Control(MS_BOOL serial);
MS_BOOL MDrv_DMD_EXTERN_DVBS_IIC_Bypass_Mode(MS_BOOL enable);
MS_BOOL MDrv_DMD_EXTERN_DVBS_Demod_ClearStatus(void);

#if 0
/*================================================
DVBS External DEMOD Get information function
=================================================*/
#endif
MS_BOOL MDrv_DMD_EXTERN_DVBS_Get_SNR(MS_FLOAT_ST *snr);
MS_BOOL MDrv_DMD_EXTERN_DVBS_Get_BER(E_BER_type ber_type, MS_FLOAT_ST *ber);
MS_BOOL MDrv_DMD_EXTERN_DVBS_Get_AGC_Info(MS_U8 u8dbg_mode, MS_U16 *pu16Data);
MS_BOOL MDrv_DMD_EXTERN_DVBS_Get_IQ_Mode(MS_BOOL *iq);
MS_BOOL MDrv_DMD_EXTERN_DVBS_Get_SYS_Type(MS_U8 *sys_type);
MS_BOOL MDrv_DMD_EXTERN_DVBS_Get_ModulationType(MS_U8 *mod_type);
MS_BOOL MDrv_DMD_EXTERN_DVBS_Get_CodeRate(MS_U8 *CR);
MS_BOOL MDrv_DMD_EXTERN_DVBS_Get_RollOff(MS_U8 *roll_off);

#if 0
/*================================================
DVBS External DEMOD Diseqc function
=================================================*/
#endif
MS_BOOL MDrv_DMD_EXTERN_DVBS_DiSEqC_Init(MS_BOOL IsMCPDMD, MS_BOOL Tx22kOff);
MS_BOOL MDrv_DMD_EXTERN_DVBS_DiSEqC_SetLNBOut(MS_BOOL bLow);
MS_BOOL MDrv_DMD_EXTERN_DVBS_DiSEqC_GetLNBOut(MS_BOOL* bLNBOutLow);
MS_BOOL MDrv_DMD_EXTERN_DVBS_DiSEqC_Set22kOnOff(MS_BOOL b22kOn);
MS_BOOL MDrv_DMD_EXTERN_DVBS_DiSEqC_Get22kOnOff(MS_BOOL* b22kOn);
MS_BOOL MDrv_DMD_EXTERN_DVBS_DiSEqC_SendCmd(MS_U8* pCmd,MS_U8 u8CmdSize);
MS_BOOL MDrv_DMD_EXTERN_DVBS_DiSEqC_SetTone(MS_BOOL bTone1);

#if 0
/*================================================
DVBS External DEMOD Blind Scan function
=================================================*/
#endif
MS_BOOL MDrv_DMD_EXTERN_DVBS_BlindScan_Start(MS_U16 u16StartFreq,MS_U16 u16EndFreq);
MS_BOOL MDrv_DMD_EXTERN_DVBS_BlindScan_NextFreq(MS_BOOL* bBlindScanEnd);
MS_BOOL MDrv_DMD_EXTERN_DVBS_BlindScan_GetTunerFreq(MS_U32 *u32TunerCenterFreq, MS_U32 *u32TunerCutOffFreq);
MS_BOOL MDrv_DMD_EXTERN_DVBS_BlindScan_WaitCurFreqFinished(MS_U8* u8Progress,MS_U8 *u8FindNum);
MS_BOOL MDrv_DMD_EXTERN_DVBS_BlindScan_Cancel(void);
MS_BOOL MDrv_DMD_EXTERN_DVBS_BlindScan_End(void);
MS_BOOL MDrv_DMD_EXTERN_DVBS_BlindScan_GetChannel(MS_U16 *u16ChannelInfoIndex, MS_U16 ptable[2][1000]);
MS_BOOL MDrv_DMD_EXTERN_DVBS_BlindScan_GetCurrentFreq(MS_U32 *u32CurrentFeq);

#if 0
/*================================================
DVBS External DEMOD read/write register
=================================================*/
#endif
MS_BOOL MDrv_DMD_EXTERN_DVBS_WriteReg(MS_U16 u16Addr, MS_U8 u8Data);
MS_BOOL MDrv_DMD_EXTERN_DVBS_WriteReg2Bytes(MS_U16 u16Addr, MS_U16 u16Data);
MS_BOOL MDrv_DMD_EXTERN_DVBS_ReadReg(MS_U16 u16Addr, MS_U8 *pu8Data);
MS_BOOL MDrv_DMD_EXTERN_DVBS_WriteDspReg(MS_U16 u16Addr, MS_U8 u8Data);
MS_BOOL MDrv_DMD_EXTERN_DVBS_ReadDspReg(MS_U16 u16Addr, MS_U8* pData);
#endif
