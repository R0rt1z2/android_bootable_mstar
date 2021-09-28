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

#ifndef _DLC_V2_H_
#define _DLC_V2_H_

#ifdef _DLC_C_
#define INTERFACE
#else
#define INTERFACE extern
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#include "UFO.h"
#include "MsTypes.h"
//******** ENUM DEFINITIONS ********//
INTERFACE enum
{
    E_DLC_POOL_ID_INTERNAL = 0, // just for test, please refine.
} E_DLC_POOL_ID;


typedef enum
{
    E_DLC_CMD_INIT_EX,
    E_DLC_CMD_SET_SETTING_EX,
    E_DLC_CMD_SET_CURVE,
    E_DLC_CMD_SET_BLE_SLOP_POINT,
#if defined(UFO_PUBLIC_HEADER_500_3) || defined(UFO_PUBLIC_HEADER_500) || defined(UFO_PUBLIC_HEADER_700)
    E_DLC_CMD_SET_HDR_INFO,
#endif
    E_DLC_CMD_EXIT,
    E_DLC_CMD_SET_ON_OFF,
    E_DLC_CMD_SET_HANDLER_ON_OFF,
    E_DLC_CMD_SET_BLE_ON_OFF,
    E_DLC_CMD_ENABLE_MAIN_SUB_CURVE_SYNCHRONIZATION,
    E_DLC_CMD_HANDLER,
    E_DLC_CMD_GET_HISTOGRAM_HANDLER,
    E_DLC_CMD_GET_AVERAGE_VALUE,
    E_DLC_CMD_GET_AVERAGE_VALUE_EX,
    E_DLC_CMD_INIT_CURVE,
    E_DLC_CMD_SPEEDUP_TRIGGER,
    E_DLC_CMD_GET_LUMA_CURVE_STATUS,
    E_DLC_CMD_CGC_RESET_C_GAIN,
    E_DLC_CMD_CGC_CHECK_C_GAIN_IN_PQ_COM,
    E_DLC_CMD_CGC_RESET_Y_GAIN,
    E_DLC_CMD_CGC_CHECK_Y_GAIN_IN_PQ_COM,
    E_DLC_CMD_CGC_RESET,
    E_DLC_CMD_CGC_INIT,
    E_DLC_CMD_CGC_REINIT,
    E_DLC_CMD_CGC_HANDLER,
    E_DLC_CMD_GET_LIB_VER,
    E_DLC_CMD_GET_INFO,
    E_DLC_CMD_GET_STATUS_EX,
    E_DLC_CMD_SET_DBG_LEVEL,
    E_DLC_CMD_WRITE_CURVE,
    E_DLC_CMD_WRITE_CURVE_SUB,
    E_DLC_CMD_GET_HISTOGRAM,
    E_DLC_CMD_DECODE_EXT_CMD,
    E_DLC_CMD_SET_CAPTURE_RANGE,
    E_DLC_CMD_SET_SETTING,
    E_DLC_CMD_INIT,
    E_DLC_CMD_GET_STATUS,
    E_DLC_CMD_SET_POWERSTATE,
#if defined(UFO_PUBLIC_HEADER_500_3) || defined(UFO_PUBLIC_HEADER_500) || defined(UFO_PUBLIC_HEADER_700)
    E_DLC_CMD_SET_TMOINFO,
#endif
    E_DLC_CMD_SET_MEMORY_ADDRESS,
    E_DBC_CMD_DLC_INIT,
    E_DBC_CMD_INIT,
    E_DBC_CMD_SET_STATUS,
    E_DBC_CMD_GET_STATUS,
    E_DBC_CMD_SET_READY,
    E_DBC_CMD_SET_DEBUG_MODE,
    E_DBC_CMD_GET_DEBUG_MODE,
    E_DBC_CMD_UPDATE_PWM,
    E_DBC_CMD_HANDLER,
    E_DBC_CMD_ADJUST_YC_GAIN,
    E_DBC_CMD_YC_GAIN_INIT,
    E_DBC_CMD_RESET,
    E_DBC_CMD_DECODE_EXT_CMD,
    E_DLC_CMD_GET_EOTF_INFO,
    E_DBC_CMD_NUM,
    E_DLC_CMD_MAX = E_DBC_CMD_NUM,
} E_DLC_IOCTL_CMDS;


typedef struct DLL_PACKED _stDLC_SetHDRInitData
{
    XC_DLC_HDRinit *pstDLC_HDRInitData;
    MS_BOOL bReturn;
} stDLC_SetHDRInitData, *pstDLC_HDRInitData;

#if defined(UFO_XC_HDR_VERSION) && (UFO_XC_HDR_VERSION == 2)
typedef struct DLL_PACKED _stDLC_SetTMOInfo
{
    ST_XC_DLC_TMO_INFO *pstDLC_TMOInfo;
    MS_BOOL bReturn;
} stDLC_SetTMOInfo, *pstDLC_SetTMOInfo;
#endif

typedef struct _stDLC_SetMemoryAddress
{
    ST_XC_DLC_SET_MEMORY_INFO *pstDLC_Set_Memory_Address;
    MS_BOOL bReturn;
} stDLC_SetMemoryAddress, *pstDLC_SetMemoryAddress;

typedef struct _stDLC_Init_Ex
{
    XC_DLC_init *pstXC_DLC_InitData;
    MS_U32 u32InitDataLen;
    MS_BOOL bReturn;
} stDLC_Init_Ex, *pstDLC_Init_Ex;

typedef struct _stDLC_SetSetting_Ex
{
    XC_DLC_MFinit_Ex *DLC_MFinit_Ex;
} stDLC_SetSetting_Ex, *pstDLC_SetSetting_Ex;


typedef struct _stDLC_SetCurve
{
    MS_U8 *pNormal;
    MS_U8 *pLight;
    MS_U8 *pDark;
} stDLC_SetCurve, *pstDLC_SetCurve;


typedef struct _stDLC_SetBleSlopPoint
{
    MS_U16 *pBLESlopPoint;
} stDLC_SetBleSlopPoint, *pstDLC_SetBleSlopPoint;


typedef struct _stDLC_Exit
{
    MS_BOOL bReturn;
} stDLC_Exit, *pstDLC_Exit;


typedef struct _stDLC_SetOnOff
{
    MS_BOOL bSwitch;
    MS_BOOL bWindow;
} stDLC_SetOnOff, *pstDLC_SetOnOff;


typedef struct _stDLC_SetDlcHandlerOnOff
{
    MS_BOOL bDlcEnable;
    MS_BOOL bReturn;
} stDLC_SetDlcHandlerOnOff, *pstDLC_SetDlcHandlerOnOff;


typedef struct _stDLC_SetBleOnOff
{
    MS_BOOL bSwitch;
    MS_BOOL bReturn;
} stDLC_SetBleOnOff, *pstDLC_SetBleOnOff;


typedef struct _stDLC_EnableMainSubCurveSynchronization
{
    MS_BOOL bEnable;
} stDLC_EnableMainSubCurveSynchronization, *pstDLC_EnableMainSubCurveSynchronization;


typedef struct _stDLC_Handler
{
    MS_BOOL bWindow;
} stDLC_Handler, *pstDLC_Handler;


typedef struct _stDLC_GetHistogramHandler
{
    MS_BOOL bWindow;
    MS_BOOL bReturn;
} stDLC_GetHistogramHandler, *pstDLC_GetHistogramHandler;


typedef struct _stDLC_GetAverageValue
{
    MS_U8 u8Return;
} stDLC_GetAverageValue, *pstDLC_GetAverageValue;


typedef struct _stDLC_GetAverageValue_Ex
{
    MS_U16 u16Return;
} stDLC_GetAverageValue_Ex, *pstDLC_GetAverageValue_Ex;


typedef struct _stDLC_InitCurve
{
    MS_U16 u16HStart;
    MS_U16 u16HEnd;
    MS_U16 u16VStart;
    MS_U16 u16VEnd;
} stDLC_InitCurve, *pstDLC_InitCurve;


typedef struct _stDLC_SpeedupTrigger
{
    MS_U8 u8Loop;
} stDLC_SpeedupTrigger, *pstDLC_SpeedupTrigger;


typedef struct _stDLC_GetLumaCurveStatus
{
    MS_BOOL bReturn;
} stDLC_GetLumaCurveStatus, *pstDLC_GetLumaCurveStatus;


typedef struct _stDLC_CGC_ResetCGain
{

} stDLC_CGC_ResetCGain, *pstDLC_CGC_ResetCGain;


typedef struct _stDLC_CGC_CheckCGainInPQCom
{

} stDLC_CGC_CheckCGainInPQCom, *pstDLC_CGC_CheckCGainInPQCom;


typedef struct _stDLC_CGC_ResetYGain
{

} stDLC_CGC_ResetYGain, *pstDLC_CGC_ResetYGain;


typedef struct _stDLC_CGC_CheckYGainInPQCom
{

} stDLC_CGC_CheckYGainInPQCom, *pstDLC_CGC_CheckYGainInPQCom;


typedef struct _stDLC_CGC_Reset
{

} stDLC_CGC_Reset, *pstDLC_CGC_Reset;


typedef struct _stDLC_CGC_Init
{

} stDLC_CGC_Init, *pstDLC_CGC_Init;


typedef struct _stDLC_CGC_ReInit
{

} stDLC_CGC_ReInit, *pstDLC_CGC_ReInit;


typedef struct _stDLC_CGC_Handler
{

} stDLC_CGC_Handler, *pstDLC_CGC_Handler;


typedef struct DLL_PACKED  _stDLC_GetLibVer
{
    const MSIF_Version **ppVersion;
    E_XC_DLC_RESULT eResult;
} stDLC_GetLibVer, *pstDLC_GetLibVer;


typedef struct _stDLC_GetInfo
{
    const XC_DLC_ApiInfo * eResult;
} stDLC_GetInfo, *pstDLC_GetInfo;


typedef struct DLL_PACKED  _stDLC_GetStatus_Ex
{
    XC_DLC_ApiStatus *pDrvStatus;
    MS_BOOL bWindow;
    MS_BOOL bReturn;
} stDLC_GetStatus_Ex, *pstDLC_GetStatus_Ex;


typedef struct _stDLC_SetDbgLevel
{
    MS_U16 u16DbgSwitch;
    MS_BOOL bReturn;
} stDLC_SetDbgLevel, *pstDLC_SetDbgLevel;


typedef struct DLL_PACKED  _stDLC_WriteCurve
{
    MS_U8 *pu8Table;
    MS_BOOL bReturn;
} stDLC_WriteCurve, *pstDLC_WriteCurve;


typedef struct DLL_PACKED  _stDLC_WriteCurve_Sub
{
    MS_U8 *pu8Table;
    MS_BOOL bReturn;
} stDLC_WriteCurve_Sub, *pstDLC_WriteCurve_Sub;


typedef struct DLL_PACKED  _stDLC_GetHistogram
{
    MS_U16 *pu16Histogram;
    E_XC_DLC_HISTOGRAM_TYPE enHistogramType;
    MS_BOOL bReturn;
} stDLC_GetHistogram, *pstDLC_GetHistogram;


typedef struct DLL_PACKED  _stDLC_DecodeExtCmd
{
    tDLC_CONTROL_PARAMS *params;
} stDLC_DecodeExtCmd, *pstDLC_DecodeExtCmd;


typedef struct DLL_PACKED  _stDLC_SetCaptureRange
{
    XC_DLC_CAPTURE_Range *pu16_Range;
} stDLC_SetCaptureRange, *pstDLC_SetCaptureRange;


typedef struct _stDLC_SetSetting
{
    XC_DLC_MFinit DLC_MFinit;
} stDLC_SetSetting, *pstDLC_SetSetting;


typedef struct _stDLC_Init
{
    XC_DLC_init *pstXC_DLC_InitData;
    MS_U32 u32InitDataLen;
    MS_BOOL bReturn;
} stDLC_Init, *pstDLC_Init;


typedef struct _stDLC_GetStatus
{
    XC_DLC_ApiStatus *pDrvStatus;
    MS_BOOL bWindow;
    MS_BOOL bReturn;
} stDLC_GetStatus, *pstDLC_GetStatus;

typedef struct _stDLC_SetPowerState
{
    EN_POWER_MODE enPowerState;
    MS_U32 u32Return;
} stDLC_SetPowerState, *pstDLC_SetPowerState;


typedef struct _stDBC_DLC_Init
{
    XC_DLC_DBC_MFinit DLC_DBC_MFinit;
} stDBC_DLC_Init, *pstDBC_DLC_Init;


typedef struct _stDBC_Init
{

} stDBC_Init, *pstDBC_Init;


typedef struct _stDBC_Setstatus
{
    MS_BOOL bDBCEnable;
} stDBC_Setstatus, *pstDBC_Setstatus;


typedef struct _stDBC_Getstatus
{
    MS_BOOL bReturn;
} stDBC_Getstatus, *pstDBC_Getstatus;


typedef struct _stDBC_SetReady
{
    MS_BOOL bDBCReady;
} stDBC_SetReady, *pstDBC_SetReady;


typedef struct _stDBC_SetDebugMode
{
    MS_U8 ucDBC_DebugMode;
} stDBC_SetDebugMode, *pstDBC_SetDebugMode;


typedef struct _stDBC_GetDebugMode
{
    MS_U8 u8Return;
} stDBC_GetDebugMode, *pstDBC_GetDebugMode;


typedef struct _stDBC_UpdatePWM
{
    MS_U8 u8PWMvalue;
} stDBC_UpdatePWM, *pstDBC_UpdatePWM;


typedef struct _stDBC_Handler
{
    MS_U16 u16Return;
} stDBC_Handler, *pstDBC_Handler;


typedef struct _stDBC_AdjustYCGain
{

} stDBC_AdjustYCGain, *pstDBC_AdjustYCGain;


typedef struct _stDBC_YCGainInit
{
    MS_U8 u8YGain_M;
    MS_U8 u8YGain_L;
    MS_U8 u8CGain_M;
    MS_U8 u8CGain_L;
} stDBC_YCGainInit, *pstDBC_YCGainInit;


typedef struct _stDBC_Reset
{

} stDBC_Reset, *pstDBC_Reset;


typedef struct DLL_PACKED  _stDBC_DecodeExtCmd
{
    tDBC_CONTROL_PARAMS *params;
} stDBC_DecodeExtCmd, *pstDBC_DecodeExtCmd;

typedef struct _stDLC_GetEOTFInfo
{
    MS_U32 u32SupportType;
} stDLC_GetEOTFInfo, *pstDLC_GetEOTFInfo;

#undef INTERFACE

#ifdef __cplusplus
}
#endif

#undef _DLC_V2_H_
#endif //_DLC_V2_H_

