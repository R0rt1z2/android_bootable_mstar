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

////////////////////////////////////////////////////////////////////////////////
//
/// @brief Demod default Nll function
//
////////////////////////////////////////////////////////////////////////////////




#ifndef _DEMODNULL_C_
#define _DEMODNULL_C_

#include "MsCommon.h"
#include "Board.h"
#include "drvDemod.h"
#include "drvDemodNull.h"



MS_BOOL MDrv_Demod_null_init(MS_U8 u8DemodIndex)
{
    printf("%s, %d \n", __FUNCTION__,__LINE__);
    return TRUE;
}

MS_BOOL MDrv_Demod_null_open(MS_U8 u8DemodIndex)
{
    printf("%s, %d \n", __FUNCTION__,__LINE__);
    return TRUE;
}


MS_BOOL MDrv_Demod_null_Close(MS_U8 u8DemodIndex)
{
    printf("%s, %d \n", __FUNCTION__,__LINE__);
    return TRUE;
}


MS_BOOL MDrv_Demod_null_Reset(MS_U8 u8DemodIndex)
{
    printf("%s, %d \n", __FUNCTION__,__LINE__);
    return TRUE;
}


MS_BOOL MDrv_Demod_null_TsOut(MS_U8 u8DemodIndex,MS_BOOL bEnable)
{
    printf("%s, %d \n", __FUNCTION__,__LINE__);
    return TRUE;
}

MS_BOOL MDrv_Demod_null_PowerOnOff(MS_U8 u8DemodIndex,MS_BOOL bPowerOn)
{
    printf("%s, %d \n", __FUNCTION__,__LINE__);
    return TRUE;
}


MS_BOOL MDrv_Demod_null_SetBW(MS_U8 u8DemodIndex,MS_U32 u32BW)
{
    printf("%s, %d \n", __FUNCTION__,__LINE__);
    return TRUE;
}


MS_BOOL MDrv_Demod_null_GetBW(MS_U8 u8DemodIndex,MS_U32 *pu32BW)
{  
    printf("%s, %d \n", __FUNCTION__,__LINE__);
    return TRUE;
}

MS_BOOL MDrv_Demod_null_GetLock(MS_U8 u8DemodIndex,EN_LOCK_STATUS *peLockStatus)
{
    printf("%s, %d \n", __FUNCTION__,__LINE__);
    return TRUE;
}

MS_BOOL MDrv_Demod_null_GetSNR(MS_U8 u8DemodIndex,MS_U32 *pu32SNR)
{
    printf("%s, %d \n", __FUNCTION__,__LINE__);
    return TRUE;
}


MS_BOOL MDrv_Demod_null_GetBER(MS_U8 u8DemodIndex,float *pfBER)
{
    printf("%s, %d \n", __FUNCTION__,__LINE__);
    return TRUE;
}

MS_BOOL MDrv_Demod_null_GetPWR(MS_U8 u8DemodIndex,MS_S32 *ps32Signal)
{
    printf("%s, %d \n", __FUNCTION__,__LINE__);
    return TRUE;
}

MS_BOOL MDrv_Demod_null_Config(MS_U8 u8DemodIndex,MS_U8 *pRegParam)
{
    printf("%s, %d \n", __FUNCTION__,__LINE__);
    return TRUE;
}


MS_BOOL MDrv_Demod_null_GetParam(MS_U8 u8DemodIndex,DEMOD_MS_FE_CARRIER_PARAM* pParam)
{
    printf("%s, %d \n", __FUNCTION__,__LINE__);
    return TRUE;
}

MS_BOOL MDrv_Demod_null_Restart(MS_U8 u8DemodIndex,DEMOD_MS_FE_CARRIER_PARAM* pParam)
{
    printf("%s, %d \n", __FUNCTION__,__LINE__);
    return TRUE;
}

MS_BOOL MDrv_Demod_null_SetMode(MS_U8 u8DemodIndex,Demod_Mode* pMode)
{
    printf("%s, %d \n", __FUNCTION__,__LINE__);
    return TRUE;
}


MS_BOOL MDrv_Demod_null_SetOutoutPath(MS_U8 u8DemodIndex,DEMOD_INTERFACE_MODE path)
{
    printf("%s, %d \n", __FUNCTION__,__LINE__);
    return TRUE;
}

DEMOD_INTERFACE_MODE MDrv_Demod_null_GetOutoutPath(MS_U8 u8DemodIndex)
{
    printf("%s, %d \n", __FUNCTION__,__LINE__);
    return DEMOD_INTERFACE_SERIAL;
}

MS_BOOL MDrv_Demod_null_I2C_ByPass(MS_U8 u8DemodIndex,MS_BOOL bOn)
{
    printf("%s, %d \n", __FUNCTION__,__LINE__);
    return TRUE;
}


MS_BOOL MDrv_Demod_null_SetCurrentDemodType(MS_U8 u8DemodIndex, MS_U8 type)
{
    printf("%s, %d \n", __FUNCTION__,__LINE__);
    return TRUE;
}

MS_U8 MDrv_Demod_null_GetCurrentDemodType(MS_U8 u8DemodIndex)
{
    printf("%s, %d \n", __FUNCTION__,__LINE__);
    return TRUE;
}

MS_BOOL MDrv_Demod_null_GetPlpBitMap(MS_U8 u8DemodIndex, MS_U8* u8PlpBitMap)
{
    printf("%s, %d \n", __FUNCTION__,__LINE__);
    return TRUE;
}

MS_BOOL MDrv_Demod_null_GetPlpGroupID(MS_U8 u8DemodIndex, MS_U8 u8PlpID, MS_U8* u8GroupID)
{
    printf("%s, %d \n", __FUNCTION__,__LINE__);
    return TRUE;

}

MS_BOOL MDrv_Demod_null_SetPlpGroupID(MS_U8 u8DemodIndex, MS_U8 u8PlpID, MS_U8 u8GroupID)
{
    printf("%s, %d \n", __FUNCTION__,__LINE__);
    return TRUE;

}

MS_BOOL MDrv_Demod_null_SetTsSerial(MS_U8 u8DemodIndex,MS_BOOL bSerial)
{
    printf("%s, %d \n", __FUNCTION__,__LINE__);
    return TRUE;
}

MS_BOOL MDrv_Demod_null_TPSGetLock(MS_U8 u8DemodIndex,MS_BOOL *pbLock)
{
    printf("%s, %d \n", __FUNCTION__,__LINE__);
    return TRUE;
}

MS_BOOL MDrv_Demod_null_MPEGGetLock(MS_U8 u8DemodIndex,MS_BOOL *pbLock)
{
    printf("%s, %d \n", __FUNCTION__,__LINE__);
    return TRUE;
}

MS_BOOL MDrv_Demod_null_GetRollOff(MS_U8 u8DemodIndex,MS_U8 *pRollOff)
{
    printf("%s, %d \n", __FUNCTION__,__LINE__);
    return TRUE;
}

MS_BOOL MDrv_Demod_null_GetRFOffset(MS_U8 u8DemodIndex,MS_S16 *ps16RFOff)
{
    printf("%s, %d \n", __FUNCTION__,__LINE__);
    return TRUE;
}

MS_BOOL MDrv_Demod_null_BlindScan_Start(MS_U8 u8DemodIndex,MS_U16 u16StartFreq,MS_U16 u16EndFreq)
{
    printf("%s, %d \n", __FUNCTION__,__LINE__);
    return TRUE;
}

MS_BOOL MDrv_Demod_null_BlindScan_NextFreq(MS_U8 u8DemodIndex,MS_BOOL* bBlindScanEnd)
{
    printf("%s, %d \n", __FUNCTION__,__LINE__);
    return TRUE;
}

MS_BOOL MDrv_Demod_null_BlindScan_WaitCurFreqFinished(MS_U8 u8DemodIndex,MS_U8* u8Progress,MS_U8 *u8FindNum)
{
    printf("%s, %d \n", __FUNCTION__,__LINE__);
    return TRUE;
}

MS_BOOL MDrv_Demod_null_BlindScan_Cancel(MS_U8 u8DemodIndex)
{
    printf("%s, %d \n", __FUNCTION__,__LINE__);
    return TRUE;
}

MS_BOOL MDrv_Demod_null_BlindScan_End(MS_U8 u8DemodIndex)
{
    printf("%s, %d \n", __FUNCTION__,__LINE__);
    return TRUE;
}

MS_BOOL MDrv_Demod_null_BlindScan_GetChannel(MS_U8 u8DemodIndex, MS_U16 u16ReadStart,MS_U16* u16TPNum,DEMOD_MS_FE_CARRIER_PARAM *pTable)
{
    printf("%s, %d \n", __FUNCTION__,__LINE__);
    return TRUE;
}

MS_BOOL MDrv_Demod_null_BlindScan_GetCurrentFreq(MS_U8 u8DemodIndex, MS_U32 *u32CurrentFeq)
{
    printf("%s, %d \n", __FUNCTION__,__LINE__);
    return TRUE;
}

MS_BOOL MDrv_Demod_null_DiSEqC_SetTone(MS_U8 u8DemodIndex, MS_BOOL bTone1)
{
    printf("%s, %d \n", __FUNCTION__,__LINE__);
    return TRUE;
}

MS_BOOL MDrv_Demod_null_DiSEqC_SetLNBOut(MS_U8 u8DemodIndex, MS_BOOL bLow)
{
    printf("%s, %d \n", __FUNCTION__,__LINE__);
    return TRUE;
}

MS_BOOL MDrv_Demod_null_DiSEqC_GetLNBOut(MS_U8 u8DemodIndex, MS_BOOL* bLNBOutLow)
{
    printf("%s, %d \n", __FUNCTION__,__LINE__);
    return TRUE;
}

MS_BOOL MDrv_Demod_null_DiSEqC_Set22kOnOff(MS_U8 u8DemodIndex, MS_BOOL b22kOn)
{
    printf("%s, %d \n", __FUNCTION__,__LINE__);
    return TRUE;
}

MS_BOOL MDrv_Demod_null_DiSEqC_Get22kOnOff(MS_U8 u8DemodIndex, MS_BOOL* b22kOn)
{
    printf("%s, %d \n", __FUNCTION__,__LINE__);
    return TRUE;
}

MS_BOOL MDrv_Demod_null_DiSEqC_SendCmd(MS_U8 u8DemodIndex, MS_U8* pCmd,MS_U8 u8CmdSize)
{
    printf("%s, %d \n", __FUNCTION__,__LINE__);
    return TRUE;
}

MS_BOOL MDrv_Demod_null_Extension_Function(MS_U8 u8DemodIndex, DEMOD_EXT_FUNCTION_TYPE fuction_type, void *data)
{
    printf("%s, %d \n", __FUNCTION__,__LINE__);
    return TRUE;
}

MS_BOOL MDrv_Demod_null_CheckExist(MS_U8 u8DemodIndex)
{
    printf("%s, %d \n", __FUNCTION__,__LINE__);
    return TRUE;
}

DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_NULL) DDI_DRV_TABLE_ENTRY(demodtab) = 
{  
     .name                         = "DEMOD_NULL",
     .data                         = DEMOD_NULL,        
     .init                         = MDrv_Demod_null_init,
     .GetLock                      = MDrv_Demod_null_GetLock,
     .GetSNR                       = MDrv_Demod_null_GetSNR,
     .GetBER                       = MDrv_Demod_null_GetBER,
     .GetPWR                       = MDrv_Demod_null_GetPWR,
     .GetParam                     = MDrv_Demod_null_GetParam,
     .Restart                      = MDrv_Demod_null_Restart,
     .I2CByPass                    = MDrv_Demod_null_I2C_ByPass,
     .CheckExist                   = MDrv_Demod_null_CheckExist,
     .Extension_Function           = MDrv_Demod_null_Extension_Function,
#if MS_DVBT2_INUSE
     .SetCurrentDemodType          = MDrv_Demod_null_SetCurrentDemodType,
     .GetCurrentDemodType          = MDrv_Demod_null_GetCurrentDemodType,
     .GetPlpBitMap                 = MDrv_Demod_null_GetPlpBitMap,
     .GetPlpGroupID                = MDrv_Demod_null_GetPlpGroupID,
     .SetPlpGroupID                = MDrv_Demod_null_SetPlpGroupID,
#endif
#if MS_DVBS_INUSE
     .BlindScanStart               = MDrv_Demod_null_BlindScan_Start,
     .BlindScanNextFreq            = MDrv_Demod_null_BlindScan_NextFreq,
     .BlindScanWaitCurFreqFinished = MDrv_Demod_null_BlindScan_WaitCurFreqFinished,
     .BlindScanCancel              = MDrv_Demod_null_BlindScan_Cancel,
     .BlindScanEnd                 = MDrv_Demod_null_BlindScan_End,
     .BlindScanGetChannel          = MDrv_Demod_null_BlindScan_GetChannel,
     .BlindScanGetCurrentFreq      = MDrv_Demod_null_BlindScan_GetCurrentFreq,
     .DiSEqCSetTone                = MDrv_Demod_null_DiSEqC_SetTone,
     .DiSEqCSetLNBOut              = MDrv_Demod_null_DiSEqC_SetLNBOut,
     .DiSEqCGetLNBOut              = MDrv_Demod_null_DiSEqC_GetLNBOut,
     .DiSEqCSet22kOnOff            = MDrv_Demod_null_DiSEqC_Set22kOnOff,
     .DiSEqCGet22kOnOff            = MDrv_Demod_null_DiSEqC_Get22kOnOff,
     .DiSEqC_SendCmd               = MDrv_Demod_null_DiSEqC_SendCmd
#endif
};

#endif

