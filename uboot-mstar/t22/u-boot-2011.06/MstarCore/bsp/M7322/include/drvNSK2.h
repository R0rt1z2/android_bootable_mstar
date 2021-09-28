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
#ifndef _DRVNSK2_H_
#define _DRVNSK2_H_

////////////////////////////////////////////////////////////////////////////////
/// @brief
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C"
{
#endif

#include "MsTypes.h"
//#include "ndstypes.h"
////////////////////////////////////////////////////////////////////////////////
// Define & data type
////////////////////////////////////////////////////////////////////////////////


typedef void (*NSK2_IntNotify)(MS_U32 reason, MS_U32 acpuError);


////////////////////////////////////////////////////////////////////////////////
// Extern Function
////////////////////////////////////////////////////////////////////////////////
MS_U32 MDrv_NSK2_Init(void);
MS_U32 MDrv_NSK2_ColdReset(void);
MS_U32 MDrv_NSK2_EndSubtest(void);
MS_U32 MDrv_NSK2_Compare(MS_U32 StartAddr, MS_U32 CompareLens, MS_U32 CompareSim,
                         MS_U32 Mask, MS_U32 ExpectResult);
MS_U32 MDrv_NSK2_CompareMem(MS_U32 reserved, MS_U32 StartAddr, MS_U32 CompareLens,
                            MS_U32 CompareSim, MS_U32 ExpectResult, void *pGolden);

MS_U32 MDrv_NSK2_CompareKTE(MS_U32 reserved_1, MS_U32 reserved_2, void *pLabel);
MS_U32 MDrv_NSK2_CompareOut(MS_U32 reserved_1, MS_U32 reserved_2, MS_U32 HighDWord, MS_U32 LowDWord);

MS_U32 MDrv_NSK2_WriteMem(MS_U32 reserved, MS_U32 StartAddr, MS_U32 WriteLens, void *pWriteData);
MS_U32 MDrv_NSK2_WriteSFR(MS_U32 StartAddr, MS_U32 Data);
MS_U32 MDrv_NSK2_WriteESA(MS_U8 ESASelect, MS_U8 ESASubSelect, MS_U8 pid_no);

MS_U32 MDrv_NSK2_WriteTransportKey( MS_U8 SCB, MS_U8 ForceSCB, void *pLabel, MS_U8 pid_no );
MS_U32 MDrv_NSK2_WriteM2MKey(void *pIV, MS_U8 SubAlgo);
MS_U32 MDrv_NSK2_WriteSCPUKey(void);
MS_U32 MDrv_NSK2_WriteReservedKey(void);
MS_U32 MDrv_NSK2_GetReserveKeyNum(void);

MS_U32 MDrv_NSK2_SetRNG(MS_U32 reserved_1, MS_U16 RNG_Value);
MS_U32 MDrv_NSK2_BasicInitializationComplete(void);
MS_U32 MDrv_NSK2_DriveKteAck(void);
MS_U32 MDrv_NSK2_FillJTagPswd(void);
MS_U32 MDrv_NSK2_CheckPubOTPConfig(void *pCheck);
MS_U32 MDrv_NSK2_UnlockOTPCtrl(void);

MS_U32 MDrv_NSK2_ReadData(MS_U32 addr_offset, MS_U32 data_size, MS_U8 *data);
MS_U32 MDrv_NSK2_WriteData(MS_U32 addr_offset, MS_U32 data_size, MS_U8 *data);
MS_U32 MDrv_NSK2_ReadData8(MS_U32 addr_offset, MS_U32 data_size, MS_U8 *data);
MS_U32 MDrv_NSK2_WriteData8(MS_U32 addr_offset, MS_U32 data_size, MS_U8 *data);
MS_U32 MDrv_NSK2_ReadData32(MS_U32 addr_offset, MS_U32 data_size, MS_U32 *data);
MS_U32 MDrv_NSK2_WriteData32(MS_U32 addr_offset, MS_U32 data_size, MS_U32 *data);

MS_U32 MDrv_NSK2_SetIntNotify(NSK2_IntNotify IntNotify);
MS_U32 MDrv_NSK2_ExcuteCmd(MS_U32 command, MS_U32 control,
                           MS_U32 register_offset, MS_U32 data_size,
                           const MS_U8    *data);

MS_U32 MDrv_NSK2_GetOTPProperties(MS_U32 *desc_size, MS_U8 *desc);
MS_U32 MDrv_NSK2_GetCMProperties(MS_U32 *desc_size, MS_U8 *desc);
MS_U32 MDrv_NSK2_GetM2MProperties(MS_U32 *desc_size, MS_U8 *desc);
MS_U32 MDrv_NSK2_GetDMAProperties(MS_U32 *desc_size, MS_U8 *desc);


MS_U32 MDrv_NSK2_GetMaxXConn(void);
MS_U32 MDrv_NSK2_GetOTPValue(MS_U32 u32Offset);
MS_U32 MDrv_NSK2_CMChannelNum(void);
MS_U32 MDrv_NSK2_GetOTPFieldValue(MS_U8 u8Tag, MS_U8 *pLength, MS_U32 *pRetValue);
void   MDrv_NSK2_SetDbgLevel(MS_U32 u32Level);
void   MDrv_NSK2_SetPollingCnt(MS_U32 u32Cnt);
void   MDrv_NSK2_GetRNGThroughPut(void *pRngData, MS_U32 u32DataSize, MS_BOOL bDump);
void   MDrv_NSK2_RunFree(MS_BOOL bRunFree);
void   MDrv_NSK2_PushSlowClock ( MS_BOOL HaltClk, MS_U32 TenSecond);
MS_U32 MDrv_NSK2_BasicInitializationComplete(void);

MS_U32 MDrv_NSK21_InvalidCmChannel(MS_U16 PidSlot);
MS_U32 MDrv_NSK21_CfgCmChannel(MS_U16 PidSlot, MS_U16 LocalDAlgo, MS_U16  ESAAlgo, MS_U16 LocalSAlgo, MS_U16 SysKeyIndex);
MS_U32 MDrv_NSK21_WriteTransportKey(MS_U8 SCB, MS_U8 ForceSCB, void *pLocalDIV1, void *pLocalDIV2,
                                    void *pESAIV1, void *pESAIV2, void *pLocalSIV1, void *pLocalSIV2, MS_U16 PidSlot );


//void   MDrv_NSK2_BurstLen(MS_U32 u32PVREng, MS_U32 u32BurstMode);
MS_U32 MDrv_NSK21_WriteM2MKey(MS_U32 M2MAlgo);
MS_U32 MDrv_NSK21_ModifyGenIn(MS_U32 MaskVal,MS_U32 XorVal);
MS_U32 MDrv_NSK21_WriteJTagKey(MS_U32 OverrideOid, MS_U32 Select);
MS_U32 MDrv_NSK2_CompareIRQ(MS_U32 reserved_1, MS_U32 reserved_2, MS_U32 ExpectedValue);
MS_U32 MDrv_NSK21_IncrementNvCounter(void);
MS_U32 MDrv_NSK2_WriteOtpKey(void);

#ifdef __cplusplus
}
#endif
#endif//_DRVNSK2_H_

