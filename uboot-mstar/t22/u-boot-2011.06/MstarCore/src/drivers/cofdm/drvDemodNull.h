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
#ifndef __DRV_DEMOD_NULL_H__
#define __DRV_DEMOD_NULL_H__

MS_BOOL MDrv_Demod_null_init(MS_U8 u8DemodIndex);
MS_BOOL MDrv_Demod_null_open(MS_U8 u8DemodIndex);
MS_BOOL MDrv_Demod_null_Close(MS_U8 u8DemodIndex);
MS_BOOL MDrv_Demod_null_Reset(MS_U8 u8DemodIndex);
MS_BOOL MDrv_Demod_null_TsOut(MS_U8 u8DemodIndex,MS_BOOL bEnable);
MS_BOOL MDrv_Demod_null_PowerOnOff(MS_U8 u8DemodIndex,MS_BOOL bPowerOn);
MS_BOOL MDrv_Demod_null_SetBW(MS_U8 u8DemodIndex,MS_U32 u32BW);
MS_BOOL MDrv_Demod_null_GetBW(MS_U8 u8DemodIndex,MS_U32 *pu32BW);
MS_BOOL MDrv_Demod_null_GetLock(MS_U8 u8DemodIndex,EN_LOCK_STATUS *peLockStatus);
MS_BOOL MDrv_Demod_null_GetSNR(MS_U8 u8DemodIndex,MS_U32 *pu32SNR);
MS_BOOL MDrv_Demod_null_GetBER(MS_U8 u8DemodIndex,float *pfBER);
MS_BOOL MDrv_Demod_null_GetPWR(MS_U8 u8DemodIndex,MS_S32 *ps32Signal);
MS_BOOL MDrv_Demod_null_Config(MS_U8 u8DemodIndex,MS_U8 *pRegParam);
MS_BOOL MDrv_Demod_null_GetParam(MS_U8 u8DemodIndex,DEMOD_MS_FE_CARRIER_PARAM* pParam);
MS_BOOL MDrv_Demod_null_Restart(MS_U8 u8DemodIndex,DEMOD_MS_FE_CARRIER_PARAM* pParam);
MS_BOOL MDrv_Demod_null_SetMode(MS_U8 u8DemodIndex,Demod_Mode* pMode);
MS_BOOL MDrv_Demod_null_SetOutoutPath(MS_U8 u8DemodIndex,DEMOD_INTERFACE_MODE path);
DEMOD_INTERFACE_MODE MDrv_Demod_null_GetOutoutPath(MS_U8 u8DemodIndex);
MS_BOOL MDrv_Demod_null_I2C_ByPass(MS_U8 u8DemodIndex,MS_BOOL bOn);

MS_BOOL MDrv_Demod_null_SetCurrentDemodType(MS_U8 u8DemodIndex, MS_U8 type);
MS_U8 MDrv_Demod_null_GetCurrentDemodType(MS_U8 u8DemodIndex);
MS_BOOL MDrv_Demod_null_GetPlpBitMap(MS_U8 u8DemodIndex, MS_U8* u8PlpBitMap);
MS_BOOL MDrv_Demod_null_GetPlpGroupID(MS_U8 u8DemodIndex, MS_U8 u8PlpID, MS_U8* u8GroupID);
MS_BOOL MDrv_Demod_null_SetPlpGroupID(MS_U8 u8DemodIndex, MS_U8 u8PlpID, MS_U8 u8GroupID);

MS_BOOL MDrv_Demod_null_SetTsSerial(MS_U8 u8DemodIndex,MS_BOOL bSerial);
MS_BOOL MDrv_Demod_null_TPSGetLock(MS_U8 u8DemodIndex,MS_BOOL *pbLock);
MS_BOOL MDrv_Demod_null_MPEGGetLock(MS_U8 u8DemodIndex,MS_BOOL *pbLock);
MS_BOOL MDrv_Demod_null_GetRollOff(MS_U8 u8DemodIndex,MS_U8 *pRollOff);
MS_BOOL MDrv_Demod_null_GetRFOffset(MS_U8 u8DemodIndex,MS_S16 *ps16RFOff);
MS_BOOL MDrv_Demod_null_BlindScan_Start(MS_U8 u8DemodIndex,MS_U16 u16StartFreq,MS_U16 u16EndFreq);
MS_BOOL MDrv_Demod_null_BlindScan_NextFreq(MS_U8 u8DemodIndex,MS_BOOL* bBlindScanEnd);
MS_BOOL MDrv_Demod_null_BlindScan_WaitCurFreqFinished(MS_U8 u8DemodIndex,MS_U8* u8Progress,MS_U8 *u8FindNum);
MS_BOOL MDrv_Demod_null_BlindScan_Cancel(MS_U8 u8DemodIndex);
MS_BOOL MDrv_Demod_null_BlindScan_End(MS_U8 u8DemodIndex);
MS_BOOL MDrv_Demod_null_BlindScan_GetChannel(MS_U8 u8DemodIndex, MS_U16 u16ReadStart,MS_U16* u16TPNum,DEMOD_MS_FE_CARRIER_PARAM *pTable);
MS_BOOL MDrv_Demod_null_BlindScan_GetCurrentFreq(MS_U8 u8DemodIndex, MS_U32 *u32CurrentFeq);
MS_BOOL MDrv_Demod_null_DiSEqC_SetTone(MS_U8 u8DemodIndex, MS_BOOL bTone1);
MS_BOOL MDrv_Demod_null_DiSEqC_SetLNBOut(MS_U8 u8DemodIndex, MS_BOOL bLow);
MS_BOOL MDrv_Demod_null_DiSEqC_GetLNBOut(MS_U8 u8DemodIndex, MS_BOOL* bLNBOutLow);
MS_BOOL MDrv_Demod_null_DiSEqC_Set22kOnOff(MS_U8 u8DemodIndex, MS_BOOL b22kOn);
MS_BOOL MDrv_Demod_null_DiSEqC_Get22kOnOff(MS_U8 u8DemodIndex, MS_BOOL* b22kOn);
MS_BOOL MDrv_Demod_null_DiSEqC_SendCmd(MS_U8 u8DemodIndex, MS_U8* pCmd,MS_U8 u8CmdSize);



#endif // #define __DRV_DEMOD_NULL_H__
