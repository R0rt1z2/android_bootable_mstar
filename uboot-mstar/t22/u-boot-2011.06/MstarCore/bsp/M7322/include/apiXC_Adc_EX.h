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
/// @brief  MStar ADC API
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _API_XC_ADC_EX_H_
#define _API_XC_ADC_EX_H_

// Common
#include "MsCommon.h"

//#include "drvXC_IOPort_EX.h"
#include "apiXC_EX.h"

#ifdef __cplusplus
extern "C"
{
#endif

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

/// Struct for store ADC Gain and Offset
typedef struct
{
    MS_U16 u16RedGain;      ///< ADC red gain
    MS_U16 u16GreenGain;    ///< ADC green gain
    MS_U16 u16BlueGain;     ///< ADC blue gain
    MS_U16 u16RedOffset;    ///< ADC red offset
    MS_U16 u16GreenOffset;  ///< ADC green offset
    MS_U16 u16BlueOffset;   ///< ADC blue offset
} XC_ADC_EX_AdcGainOffsetSetting;

/// ADC output internal voltage type
typedef enum
{
    E_XC_ADC_EX_Internal_0V,      ///< Output   0V
    E_XC_ADC_EX_Internal_0_1V,    ///< Output 0.1V
    E_XC_ADC_EX_Internal_0_6V,    ///< Output 0.6V
    E_XC_ADC_EX_Internal_None,    ///< OFF
} XC_ADC_EX_Internal_Voltage;

/// ADC output Clock type
typedef enum
{
    E_XC_ADC_EX_Gen_480P_Clk,     ///< ADC output 480P
    E_XC_ADC_EX_Gen_720P_Clk,     ///< ADC output 720P
    E_XC_ADC_EX_Gen_1080P_Clk,    ///< ADC output 1080P
} XC_ADC_EX_Gen_Clock_Type;

/// ADC V Clamping type
typedef enum
{
    E_XC_ADC_EX_VClamp_0_85V,     ///< Clamp to 0.85V
    E_XC_ADC_EX_VClamp_0_9V,      ///< Clamp to 0.9V
    E_XC_ADC_EX_VClamp_0_95V,     ///< Clamp to 0.95V
    E_XC_ADC_EX_VClamp_1_0V,      ///< Clamp to 1.0V
    E_XC_ADC_EX_VClamp_1_05V,     ///< Clamp to 1.05V
    E_XC_ADC_EX_VClamp_1_2V,      ///< Clamp to 1.2V
    E_XC_ADC_EX_VClamp_1_5V,      ///< Clamp to 1.5V
} XC_ADC_EX_VClamp_Level_Type;

/// Update Gain / Offset for specific channel. This is used in MApi_XC_ADC_EX_SetGain or MApi_XC_ADC_EX_SetOffset
typedef enum
{
    E_XC_ADC_EX_CHR = 0,    ///< Channel R
    E_XC_ADC_EX_CHG,        ///< Channel G
    E_XC_ADC_EX_CHB,        ///< Channel B
    E_XC_ADC_EX_ALL,        ///< All Channel
} XC_ADC_EX_CHANNEL_t;

/// Software reset for ADCDVIPLL block
typedef enum
{
    E_XC_ADC_EX_REST_ATOP        = _XC_EX_BIT(7),     ///< Reset ATOP
    E_XC_ADC_EX_REST_HDMI        = _XC_EX_BIT(6),
    E_XC_ADC_EX_REST_HDCP        = _XC_EX_BIT(5),
    E_XC_ADC_EX_REST_DVI         = _XC_EX_BIT(4),
    E_XC_ADC_EX_REST_PLL_DIG_B   = _XC_EX_BIT(3),
    E_XC_ADC_EX_REST_ADC_DIG_B   = _XC_EX_BIT(2),
    E_XC_ADC_EX_REST_PLL_DIG_A   = _XC_EX_BIT(1),
    E_XC_ADC_EX_REST_ADC_DIG_A   = _XC_EX_BIT(0),
    E_XC_ADC_EX_REST_PLL         = ( E_XC_ADC_EX_REST_PLL_DIG_B | E_XC_ADC_EX_REST_PLL_DIG_A ), ///< Reset PLL
    E_XC_ADC_EX_REST_ADC         = ( E_XC_ADC_EX_REST_ADC_DIG_B | E_XC_ADC_EX_REST_ADC_DIG_A ), ///< Reset All ADC
} XC_ADC_EX_RESET;


//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
//******************************************************************************
/// Enable and Disable CVBS out. This will setup ADC part for CVBS out (monitor out).
/// @param bEnable \b IN Enable CVBS Out
/// @param enInputPortType \b IN The source need to monitor out.
/// @param isDigitalSource \b IN Tell the driver current source is digital source or not. (ex. If internal VIF used in ATV, it will be digital source)
//******************************************************************************
DLL_PUBLIC void    MApi_XC_ADC_EX_SetCVBSOut(XC_DEVICE_ID *pDeviceId, MS_BOOL bEnable, XC_EX_DEST_TYPE enOutputType , XC_EX_INPUT_SOURCE_TYPE enInputPortType, MS_BOOL isDigitalSource);
DLL_PUBLIC MS_BOOL MApi_XC_ADC_EX_IsCVBSOutEnabled(XC_DEVICE_ID *pDeviceId, XC_EX_DEST_TYPE enOutputType);

/******************************************************************************/
/// Set ADC Sample clock
/// @param u16Value \b IN value need to be set.
/******************************************************************************/
DLL_PUBLIC void    MApi_XC_ADC_EX_SetPcClock(XC_DEVICE_ID *pDeviceId, MS_U16 u16Clock);

/******************************************************************************/
/// Set ADC Phase, pls use MApi_XC_ADC_SetPhaseEx instead
/// @param u8Value \b IN The adc phase need to be update
/******************************************************************************/
DLL_PUBLIC void    MApi_XC_ADC_EX_SetPhase(XC_DEVICE_ID *pDeviceId, MS_U8 u8Value);

/******************************************************************************/
/// Set ADC Phase Ex
/// @param u16Value \b IN The adc phase need to be update
/******************************************************************************/
DLL_PUBLIC void    MApi_XC_ADC_EX_SetPhaseEx(XC_DEVICE_ID *pDeviceId, MS_U16 u16Value);

//******************************************************************************
/// Get current ADC Phase set
/// @return Current Phase
//******************************************************************************
DLL_PUBLIC MS_U16 	MApi_XC_ADC_EX_GetPhaseRange( XC_DEVICE_ID *pDeviceId );

//******************************************************************************
/// Get current ADC Phase set
/// @return Current Phase
//******************************************************************************
DLL_PUBLIC MS_U8    MApi_XC_ADC_EX_GetPhase( XC_DEVICE_ID *pDeviceId );
DLL_PUBLIC MS_U16   MApi_XC_ADC_EX_GetPhaseEx( XC_DEVICE_ID *pDeviceId );

DLL_PUBLIC MS_BOOL MApi_XC_ADC_EX_IsScartRGB(XC_DEVICE_ID *pDeviceId);
DLL_PUBLIC MS_U16  MApi_XC_ADC_EX_GetPcClock(XC_DEVICE_ID *pDeviceId );

/******************************************************************************/
///Get SOG level range
///@param u32Min \b OUT: min of SOG level
///@param u32Max \b OUT: max of SOG level
///@param u32Recommend_value \b OUT: recommend value
/******************************************************************************/
DLL_PUBLIC void    MApi_XC_ADC_EX_GetSoGLevelRange(XC_DEVICE_ID *pDeviceId, MS_U32 *u32min, MS_U32 *u32max, MS_U32 *u32Recommend_value);

/******************************************************************************/
///Set SOG level
///@param u32Min \b IN: set SOG level
/******************************************************************************/
DLL_PUBLIC void    MApi_XC_ADC_EX_SetSoGLevel(XC_DEVICE_ID *pDeviceId, MS_U32 u32Value);

// power
//******************************************************************************
/// Turn off ADC
//******************************************************************************
DLL_PUBLIC void    MApi_XC_ADC_EX_PowerOff(XC_DEVICE_ID *pDeviceId);

// ADC calibration
//******************************************************************************
/// Get ADC Default gain / offset by referring input structure.
/// @param eSource \b IN The input source
/// @param pstADCSetting \b IN gain / offset need to be set
//******************************************************************************
DLL_PUBLIC void    MApi_XC_ADC_EX_GetDefaultGainOffset(XC_DEVICE_ID *pDeviceId, XC_EX_INPUT_SOURCE_TYPE eSource , XC_ADC_EX_AdcGainOffsetSetting* pstADCSetting);

//******************************************************************************
/// get maximal value of offset
/// @param
/// @return maximal value of offset
//******************************************************************************
DLL_PUBLIC MS_U16  MApi_XC_ADC_EX_GetMaximalOffsetValue(XC_DEVICE_ID *pDeviceId);

//******************************************************************************
/// get maximal value of gain
/// @param
/// @return maximal value of gain
//******************************************************************************
DLL_PUBLIC MS_U16  MApi_XC_ADC_EX_GetMaximalGainValue(XC_DEVICE_ID *pDeviceId);

//******************************************************************************
/// get center value of gain
/// @param
/// @return center value of gain
//******************************************************************************
DLL_PUBLIC MS_U16 MApi_XC_ADC_EX_GetCenterGain(XC_DEVICE_ID *pDeviceId);

//******************************************************************************
/// get center value of offset
/// @param
/// @return center value of offset
//******************************************************************************
DLL_PUBLIC MS_U16 MApi_XC_ADC_EX_GetCenterOffset(XC_DEVICE_ID *pDeviceId);

// Set Gain & Offset
// u8Color is one of ADC_CHANNEL_t.
//******************************************************************************
/// Set Gain value
/// @param u8color \b IN The channel type need to be set
/// @param u16value \b IN The value need to be set.
//******************************************************************************
DLL_PUBLIC void 	MApi_XC_ADC_EX_SetGain(XC_DEVICE_ID *pDeviceId, MS_U8 u8color, MS_U16 u16value);

//******************************************************************************
/// Set offset value
/// @param u8color \b IN The channel type need to be set
/// @param u16value \b IN The value need to be set.
//******************************************************************************
DLL_PUBLIC void 	MApi_XC_ADC_EX_SetOffset(XC_DEVICE_ID *pDeviceId, MS_U8 u8color, MS_U16 u16value);

//******************************************************************************
/// Set ADC gain / offset by referring input structure.
/// @param pstADCSetting \b IN gain / offset need to be set
//******************************************************************************
DLL_PUBLIC void    MApi_XC_ADC_EX_AdjustGainOffset(XC_DEVICE_ID *pDeviceId, XC_ADC_EX_AdcGainOffsetSetting* pstADCSetting);

//******************************************************************************
/// Set ADC Internal calibration. (need to be set during set mode)
/// @param enInputSourceType \b IN current input source.
//******************************************************************************
DLL_PUBLIC void    MApi_XC_ADC_EX_Source_Calibrate(XC_DEVICE_ID *pDeviceId, XC_EX_INPUT_SOURCE_TYPE enInputSourceType);

/******************************************************************************/
///Set SOG level
///@param u32Min \b IN: set SOG calibration
/******************************************************************************/
DLL_PUBLIC void    MApi_XC_ADC_EX_SetSoGCal(XC_DEVICE_ID *pDeviceId);

//Set SCART RGB PIPE Delay
DLL_PUBLIC void MApi_XC_ADC_EX_SetRGB_PIPE_Delay(XC_DEVICE_ID *pDeviceId, MS_U8 u8Value);

//Set Scart RGB Sync on green clamp delay value
DLL_PUBLIC void MApi_XC_ADC_EX_ScartRGB_SOG_ClampDelay(XC_DEVICE_ID *pDeviceId, MS_U16 u16Clpdly, MS_U16 u16Caldur);

DLL_PUBLIC void MApi_XC_ADC_EX_Set_YPbPrLooseLPF(XC_DEVICE_ID *pDeviceId, MS_BOOL benable);

/******************************************************************************/
///Set SOG BW
///@param u16value \b IN: set SOG BW
/******************************************************************************/
DLL_PUBLIC void MApi_XC_ADC_EX_Set_SOGBW(XC_DEVICE_ID *pDeviceId, MS_U16 u16value);

// Set Clamp Duration
DLL_PUBLIC void MApi_XC_ADC_EX_SetClampDuration(XC_DEVICE_ID *pDeviceId, MS_U16 u16Value);

/******************************************************************************/
///Enable HW calibration, will load hw adc table when changing source
///@param bEnable \b IN: enable hw calibration
/******************************************************************************/
DLL_PUBLIC MS_BOOL MApi_XC_ADC_EX_EnableHWCalibration(XC_DEVICE_ID *pDeviceId, MS_BOOL bEnable);

#ifdef __cplusplus
}
#endif

#endif  // _API_XC_ADC_EX_H_

