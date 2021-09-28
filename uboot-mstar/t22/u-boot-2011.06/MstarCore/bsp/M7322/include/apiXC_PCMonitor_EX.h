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
/// @brief header file
///
////////////////////////////////////////////////////////////////////////////////
#ifndef _API_XC_PC_MONITOR_EX_H_
#define _API_XC_PC_MONITOR_EX_H_

#include "MsCommon.h"

// Driver
//#include "drvXC_IOPort.h"

// Api
#include "apiXC_EX.h"

#ifdef __cplusplus
extern "C"
{
#endif

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------

#define XC_PCMONITOR_EX_ENABLE_IP_AUTO_COAST    (0)

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

/// Indecate the status of pc monitor.
typedef enum
{
    E_XC_PCMONITOR_EX_STABLE_NOSYNC = 0,       ///< Input timing stable, no input sync detected
    E_XC_PCMONITOR_EX_STABLE_SYNC ,            ///< Input timing stable, no input sync detected
    E_XC_PCMONITOR_EX_UNSTABLE,                ///< Timing change, has to wait InfoFrame if HDMI input
} XC_PCMONITOR_EX_STATUS;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

DLL_PUBLIC MS_BOOL MApi_XC_PCMonitor_EX_Init(XC_DEVICE_ID *pDeviceId, MS_U8 u8MaxWindowNum);
DLL_PUBLIC void MApi_XC_PCMonitor_EX_Restart(XC_DEVICE_ID *pDeviceId, XC_EX_SCALER_WIN eWindow);
DLL_PUBLIC void MApi_XC_PCMonitor_EX_SetTimingCount(XC_DEVICE_ID *pDeviceId, MS_U16 u16TimingStableCounter, MS_U16 u16TimingNosyncCounter);

DLL_PUBLIC XC_PCMONITOR_EX_STATUS MApi_XC_PCMonitor_EX(XC_DEVICE_ID *pDeviceId, XC_EX_INPUT_SOURCE_TYPE src, XC_EX_SCALER_WIN eWindow);

// get input timing information
DLL_PUBLIC XC_PCMONITOR_EX_STATUS MApi_XC_PCMonitor_EX_GetCurrentState(XC_DEVICE_ID *pDeviceId, XC_EX_SCALER_WIN eWindow);
DLL_PUBLIC MS_U8 MApi_XC_PCMonitor_EX_GetSyncStatus(XC_DEVICE_ID *pDeviceId, XC_EX_SCALER_WIN eWindow);

//-------------------------------------------------------------------------------------------------
/// Get horizontal frequency * 10
/// @param  eWindow                 \b IN: which window we are going to get
/// @return MS_U16 horizontal frequency * 10
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_U16 MApi_XC_PCMonitor_EX_Get_HFreqx10(XC_DEVICE_ID *pDeviceId, XC_EX_SCALER_WIN eWindow);

//-------------------------------------------------------------------------------------------------
/// Get horizontal frequency * 1000
/// @param  eWindow                 \b IN: which window we are going to get
/// @return MS_U32 horizontal frequency * 1000
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_U32 MApi_XC_PCMonitor_EX_Get_HFreqx1K(XC_DEVICE_ID *pDeviceId, XC_EX_SCALER_WIN eWindow);

//-------------------------------------------------------------------------------------------------
/// Get vertical frequency * 10
/// @param  eWindow                 \b IN: which window we are going to get
/// @return MS_U16 vertical frequency * 10
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_U16 MApi_XC_PCMonitor_EX_Get_VFreqx10(XC_DEVICE_ID *pDeviceId, XC_EX_SCALER_WIN eWindow);

//-------------------------------------------------------------------------------------------------
/// Get vertical frequency * 1000
/// @param  eWindow                 \b IN: which window we are going to get
/// @return MS_U32 vertical frequency * 1000
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_U32 MApi_XC_PCMonitor_EX_Get_VFreqx1K(XC_DEVICE_ID *pDeviceId, XC_EX_SCALER_WIN eWindow);
DLL_PUBLIC MS_U16 MApi_XC_PCMonitor_EX_Get_Vtotal(XC_DEVICE_ID *pDeviceId, XC_EX_SCALER_WIN eWindow);
DLL_PUBLIC void MApi_XC_PCMonitor_EX_Get_Dvi_Hdmi_De_Info(XC_DEVICE_ID *pDeviceId, XC_EX_SCALER_WIN eWindow,XC_EX_WINDOW_TYPE* msWin);
DLL_PUBLIC MS_BOOL MApi_XC_PCMonitor_EX_SyncLoss(XC_DEVICE_ID *pDeviceId, XC_EX_SCALER_WIN eWindow);
DLL_PUBLIC MS_BOOL MApi_XC_PCMonitor_EX_InvalidTimingDetect(XC_DEVICE_ID *pDeviceId, MS_BOOL bPollingOnly, XC_EX_SCALER_WIN eWindow);
DLL_PUBLIC XC_EX_RETURN_VALUE MApi_XC_PCMonitor_EX_SetTimingCountEx( XC_DEVICE_ID *pDeviceId, XC_EX_INPUT_SOURCE_TYPE eCurrentSrc, MS_U16 u16TimingStableCounter, MS_U16 u16TimingNosyncCounter);

#ifdef __cplusplus
}
#endif

#endif /* _API_XC_PC_MONITOR_EX_H_ */
