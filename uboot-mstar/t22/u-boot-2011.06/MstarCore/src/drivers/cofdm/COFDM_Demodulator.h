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

#ifndef __COFDM_DEMODULATOR_H__
#define __COFDM_DEMODULATOR_H__

#include <MsTypes.h>
#include <ShareType.h>
#include <msAPI_Tuner.h>

#if 1//((CONFIG_COMMANDS&(CFG_CMD_TUNER | CFG_CMD_DEMOD)) == (CFG_CMD_TUNER | CFG_CMD_DEMOD))


typedef enum
{
    E_SERIAL_INTERFACE,
    E_PARALLEL_INTERFACE
} TS_INTERFACE;

#if (FRONTEND_DEMOD_TYPE == MSTAR_MSB123x_DEMOD)
#include <cofdm/MSB123x.h>
#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB123xc_DEMOD)
#include <cofdm/MSB123xc.h>
#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB1400_DEMOD)
#include <cofdm/MSB1400.h>
#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB122x_DEMOD)
#include <cofdm/MSB122x.h>
#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB1233c_DEMOD)
#include <cofdm/MSB1233C.h>
#elif ((FRONTEND_DEMOD_TYPE == EMBEDDED_DVBC_DEMOD)||(FRONTEND_DEMOD_TYPE == EMBEDDED_DVBS_DEMOD)||(FRONTEND_DEMOD_TYPE == EMBEDDED_DVBT2_DEMOD))
#if (defined(CONFIG_KENYA)||defined(CONFIG_KERES)||defined(CONFIG_KIRIN)||defined(CONFIG_KRIS)||defined(CONFIG_KRATOS)||defined(CONFIG_KIWI)||defined(CONFIG_KAYLA)||defined(CONFIG_K5TN)||defined(CONFIG_K5AP)||defined(CONFIG_K1C))
typedef enum
{
	COFDM_FEC_LOCK,
	COFDM_PSYNC_LOCK,
	COFDM_TPS_LOCK,
	COFDM_DCR_LOCK,
	COFDM_AGC_LOCK,
	COFDM_MODE_DET,

} COFDM_LOCK_STATUS;
#endif
#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB1238_DEMOD)
#if defined(CONFIG_KENYA)
typedef enum
{
	COFDM_FEC_LOCK,
	COFDM_PSYNC_LOCK,
	COFDM_TPS_LOCK,
	COFDM_DCR_LOCK,
	COFDM_AGC_LOCK,
	COFDM_MODE_DET,

} COFDM_LOCK_STATUS;
#endif
#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB124x_DEMOD)
#if (defined(CONFIG_KERES)||defined(CONFIG_KIRIN)||defined(CONFIG_KRIS)||defined(CONFIG_KRATOS)||defined(CONFIG_KIWI)||defined(CONFIG_KAYLA)||defined(CONFIG_K1C))
typedef enum
{
    COFDM_FEC_LOCK,
    COFDM_PSYNC_LOCK,
    COFDM_TPS_LOCK,
    COFDM_TPS_LOCK_HISTORY,
    COFDM_DCR_LOCK,
    COFDM_AGC_LOCK,
    COFDM_MODE_DET,
    COFDM_LOCK_STABLE_DVBT,
    COFDM_SYNC_LOCK_DVBT,
    COFDM_FAST_LOCK_DVBT,
    COFDM_P1_LOCK,
    COFDM_P1_LOCK_HISTORY,
    COFDM_L1_CRC_LOCK,
    COFDM_DVBT2_NOCH_FLAG,
    COFDM_DVBT_NOCH_FLAG,
} COFDM_LOCK_STATUS;
#endif
#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB1245_DEMOD)
typedef enum
{
	COFDM_FEC_LOCK,
	COFDM_PSYNC_LOCK,
	COFDM_TPS_LOCK,
	COFDM_DCR_LOCK,
	COFDM_AGC_LOCK,
	COFDM_MODE_DET,
} COFDM_LOCK_STATUS;
#endif


//******************************************************************************
// Functions prototype
//******************************************************************************

int devCOFDM_Init(void);
void devCOFDM_ControlPowerOnOff(BOOLEAN bPowerOn);
FUNCTION_RESULT devCOFDM_PowerSave(void);
void devCOFDM_SetFreqStep(FREQSTEP eFreqStep);
void devCOFDM_SetFrequency(MS_U32 dwFrequency, RF_CHANNEL_BANDWIDTH eBandWidth, RF_CHANNEL_HP_LP eHpLp, U8 u8PlpID);
BOOLEAN devCOFDM_GetLockStatus(COFDM_LOCK_STATUS eStatus);
U8 devCOFDM_GetSignalToNoiseRatio(void);
void devCOFDM_PassThroughI2C(BOOLEAN bEnable);
BOOLEAN devCOFDM_PassThroughI2C_WriteBytes(U8 u8SlaveID, U8 u8AddrNum, U8* paddr, U16 u16size, U8* pu8data);
BOOLEAN devCOFDM_PassThroughI2C_ReadBytes(U8 u8SlaveID, U8 u8AddrNum, U8* paddr, U16 u16size, U8* pu8data);
#if (FRONTEND_DEMOD_TYPE == ZARLINK_CE6355_DEMOD)
void devCOFDM_SwitchTransportSteamInferface(TS_INTERFACE interface, RF_CHANNEL_BANDWIDTH BandWidth);
#else
void devCOFDM_SwitchTransportSteamInferface(TS_INTERFACE interface);
#endif
#if DTV_SCAN_AUTO_FINE_TUNE_ENABLE
BOOLEAN devCOFDM_GetFrequencyOffset(float *pFreqOff, RF_CHANNEL_BANDWIDTH u8BW);
BOOLEAN devCOFDM_SetAfcEnable(BOOLEAN bEnable);
#endif
FUNCTION_RESULT devCOFDM_GetSignal_Vit( MS_U32 *vit );
FUNCTION_RESULT devCOFDM_GetSignalStrength(MS_U16 *strength);
BOOLEAN devCOFDM_Get_CELL_ID(MS_U16 *wCell_id);
FUNCTION_RESULT devCOFDM_GetSignalQuality(MS_U16 *quality);
BOOLEAN devCOFDM_GetBER(float *ber);
BOOLEAN devCOFDM_Get_Modul_Mode(MS_U16 *Modul_Mode);
BOOLEAN devCOFDM_Is_Hierarchy_On(void);
void devCOFDM_TunerProgramming(U8 *cTunerData);

#if (FRONTEND_DEMOD_TYPE == MSTAR_MSB1200_DEMOD || FRONTEND_DEMOD_TYPE == MSTAR_MSB1210_DEMOD || FRONTEND_DEMOD_TYPE == EMBEDDED_DVBT_DEMOD || FRONTEND_DEMOD_TYPE == TOSHIBA_TC90517FG_DEMOD)
U16 devCOFDM_ReadReg(U16 RegAddr);
BOOLEAN devCOFDM_WriteReg(U16 RegAddr, U16 RegData);
#endif
BOOLEAN devCOFDM_InitFrontend(EN_DEVICE_DEMOD_TYPE enDemodType, MS_U32 u32Frequency, MS_U8 u8BandWidth);
#if ((FRONTEND_DEMOD_TYPE == MSTAR_MSB123x_DEMOD) || (FRONTEND_DEMOD_TYPE == MSTAR_MSB122x_DEMOD) ||(FRONTEND_DEMOD_TYPE == MSTAR_MSB1233c_DEMOD) || (FRONTEND_DEMOD_TYPE == MSTAR_MSB123xc_DEMOD))
BOOLEAN devCOFDM_SetDemodType(EN_DEVICE_DEMOD_TYPE enDemodType);
EN_DEVICE_DEMOD_TYPE devCOFDM_GetDemodType(void);
#endif

#if ((FRONTEND_DEMOD_TYPE == MSTAR_MSB124x_DEMOD) ||(FRONTEND_DEMOD_TYPE == EMBEDDED_DVBT2_DEMOD))
BOOLEAN devCOFDM_SetDemodType(EN_DEVICE_DEMOD_TYPE enDemodType);
#endif

#if ((FRONTEND_DEMOD_TYPE == MSTAR_MSB123x_DEMOD) || (FRONTEND_DEMOD_TYPE == MSTAR_MSB1233c_DEMOD) || (FRONTEND_DEMOD_TYPE == MSTAR_MSB123xc_DEMOD))
BOOLEAN devCOFDM_DTV_Serial_Control(BOOLEAN bEnable);
#endif

#if ((FRONTEND_DEMOD_TYPE == MSTAR_MSB1238_DEMOD)||(FRONTEND_DEMOD_TYPE == MSTAR_MSB124x_DEMOD))
MS_BOOL MDrv_Tuner_Init(void);
MS_U32 MDrv_Tuner_SetTuner(MS_U32 dwFreq /*Khz*/, MS_U8 ucBw /*MHz*/);
#else
MS_BOOL MDrv_Tuner_Init(void);
#endif

#if ENABLE_DVB_T2_SYS
MS_U8 devCOFDM_GetPlpIDList(void);
MS_U8 devCOFDM_GetNextPlpID(MS_U8 u8Index);
MS_BOOL devCOFDM_SetPlpID(MS_U8 u8PlpID);
#endif

#endif
#endif // __COFDM_DEMODULATOR_H__

