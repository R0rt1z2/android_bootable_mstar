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
/// @brief XC api for Auto
///
////////////////////////////////////////////////////////////////////////////////
#ifndef _APIXC_AUTO_EX_H_
#define _APIXC_AUTO_EX_H_

// Common Definition
#include "MsCommon.h"
#include "MsOS.h"

// Driver
//#include "drvXC_IOPort_EX.h"

// Api
#include "apiXC_EX.h"
#include "apiXC_Adc_EX.h"


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
#define XC_AUTO_EX_SIGNAL_INFO_VERSION 1

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
typedef enum
{
    E_XC_AUTO_EX_TUNE_NULL = 0x00,

    // Basic auto-tune
    E_XC_AUTO_EX_TUNE_VALID_DATA    =   0x01,
    E_XC_AUTO_EX_TUNE_POSITION      =   0x02,
    E_XC_AUTO_EX_TUNE_FREQ          =   0x04,
    E_XC_AUTO_EX_TUNE_PHASE         =   0x08,
    E_XC_AUTO_EX_TUNE_BASIC         =   E_XC_AUTO_EX_TUNE_VALID_DATA | E_XC_AUTO_EX_TUNE_POSITION | E_XC_AUTO_EX_TUNE_FREQ | E_XC_AUTO_EX_TUNE_PHASE,

    // Auto-tune RGB Color
    E_XC_AUTO_EX_TUNE_RGB_OFFSET    =   0x10,
    E_XC_AUTO_EX_TUNE_RGB_GAIN      =   0X20,
    E_XC_AUTO_EX_TUNE_RGB_COLOR     =   E_XC_AUTO_EX_TUNE_RGB_OFFSET | E_XC_AUTO_EX_TUNE_RGB_GAIN,

    // Auto-tune YUV Color
    E_XC_AUTO_EX_TUNE_YUV_COLOR     =   0X40,

    E_XC_AUTO_EX_TUNE_SCART_RGB_GAIN =  0x80,

    // Advance auto-tune
    E_XC_AUTO_EX_TUNE_ADVANCE       =   E_XC_AUTO_EX_TUNE_BASIC | E_XC_AUTO_EX_TUNE_RGB_COLOR,
} XC_Auto_EX_TuneType;

typedef enum
{
    E_XC_AUTO_EX_INTERNAL_CALIBRATION,
    E_XC_AUTO_EX_EXTERNAL_CALIBRATION,
} XC_Auto_EX_CalibrationType;

typedef enum
{
    E_XC_AUTO_EX_HW_CALIBRATION,
    E_XC_AUTO_EX_SW_CALIBRATION,
} XC_Auto_EX_CalibrationMode;

typedef struct
{
    MS_U16 u16HorizontalStart;      ///< horizontal start
    MS_U16 u16VerticalStart;        ///< vertical start
    MS_U16 u16HorizontalTotal;      ///< ADC horizontal total
    MS_U16 u16HResolution;          ///< H size
    MS_U8  u8Phase;                 ///< obsolete
} XC_Auto_EX_Signal_Info;

typedef struct
{
    MS_U16 u16Version;              ///< version
    MS_U16 u16HorizontalStart;      ///< horizontal start
    MS_U16 u16VerticalStart;        ///< vertical start
    MS_U16 u16HorizontalTotal;      ///< ADC horizontal total
    MS_U16 u16HResolution;          ///< H size
    MS_U16 u16Phase;                ///< ADC phase
} XC_Auto_EX_Signal_Info_Ex;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
//*************************************************************************
//Function name: MApi_XC_Auto_EX_Geometry
//Passing parameter:
//  enAutoTuneType: Auto function select
//  ActiveInfo: Current PC mode setting
//  StandardInfo: Standard mode. (This is singal information of stardard)
//Return parameter:
//  MS_BOOL: Success status. (If faile, return FALSE.)
//Description: Auto-tune for PC mode
//*************************************************************************
DLL_PUBLIC MS_BOOL MApi_XC_Auto_EX_Geometry(XC_DEVICE_ID *pDeviceId, XC_Auto_EX_TuneType enAutoTuneType, XC_Auto_EX_Signal_Info *ActiveInfo, XC_Auto_EX_Signal_Info *StandardInfo, XC_EX_SCALER_WIN eWindow);

DLL_PUBLIC MS_BOOL MApi_XC_Auto_EX_Geometry_Ex(XC_DEVICE_ID *pDeviceId, XC_Auto_EX_TuneType enAutoTuneType, XC_Auto_EX_Signal_Info_Ex *ActiveInfo, XC_Auto_EX_Signal_Info_Ex *StandardInfo, XC_EX_SCALER_WIN eWindow);

DLL_PUBLIC MS_BOOL MApi_XC_Auto_EX_StopAutoGeometry(XC_DEVICE_ID *pDeviceId); // Call MApi_XC_Auto_EX_Geometry to restart auto geometry again.

DLL_PUBLIC MS_BOOL MApi_XC_Auto_EX_GainOffset(XC_DEVICE_ID *pDeviceId, XC_Auto_EX_CalibrationType type, XC_Auto_EX_TuneType enAutoTuneType,  XC_ADC_EX_AdcGainOffsetSetting *pstADCSetting, XC_EX_SCALER_WIN eWindow);

//***************************************************************************
///-This function is to get fixed gain for HW calibration
/// @param  enAutoTuneType      \b IN:tune type
/// @param  pstADCSetting       \b OUT:gain value
/// @return @ref MS_BOOL
//***************************************************************************
DLL_PUBLIC MS_BOOL MApi_XC_Auto_EX_GetHWFixedGainOffset(XC_DEVICE_ID *pDeviceId, XC_Auto_EX_TuneType enAutoTuneType, XC_ADC_EX_AdcGainOffsetSetting *pstADCSetting);

//***************************************************************************
///-This function is to set ValidDataThreshold by customer
/// @param  Value        \b IN:ValidDataThreshold
/// @return @ref MS_BOOL
//***************************************************************************
DLL_PUBLIC MS_BOOL MApi_XC_Auto_EX_SetValidData(XC_DEVICE_ID *pDeviceId, MS_U8 Value);

/******************************************************************************/
/// -This function will recalibration ADC offset, must be called when mode change.
/// @param bEnable \b IN:
/// - 0: Disable offset recalibration
/// - 1: Enable offset recalibration
/// @param bIsYPbPrFlag \b IN: to tell if YPbPr in
/// - 0: Vga mode
/// - 1: YPbPr mode
/******************************************************************************/
DLL_PUBLIC void MApi_XC_Auto_EX_AutoOffset(XC_DEVICE_ID *pDeviceId, MS_BOOL bEnable, MS_BOOL bIsYPbPrFlag);

/******************************************************************************/
/// -This function will try to get the actual mode width for VGA
/// @param  eWindow          \b IN: Window
/// @param  pSignalInfo      \b IN: Input signal info for distinguish
/// Note: The result of "MApi_XC_Auto_EX_DetectWidth"  will highly depends on the HTT value of pSignalInfo
/******************************************************************************/
DLL_PUBLIC MS_BOOL MApi_XC_Auto_EX_DetectWidth(XC_DEVICE_ID *pDeviceId, XC_Auto_EX_Signal_Info_Ex *pSignalInfo , XC_EX_SCALER_WIN eWindow);

//***************************************************************************
///-This function is to set HW or Sw calibration
/// @param  eMode        \b IN: HW or SW mode for calibration
/// @return @ref MS_BOOL
//***************************************************************************
DLL_PUBLIC MS_BOOL MApi_XC_Auto_EX_SetCalibrationMode(XC_DEVICE_ID *pDeviceId, XC_Auto_EX_CalibrationMode eMode);

//***************************************************************************
///-This function is to get HW or Sw calibration
/// @param  eMode        \b OUT: HW or SW mode for calibration
//***************************************************************************
DLL_PUBLIC void MApi_XC_Auto_EX_GetCalibrationMode(XC_DEVICE_ID *pDeviceId, XC_Auto_EX_CalibrationMode *eMode);


#ifdef __cplusplus
}
#endif

#endif /* _APIXC_AUTO_EX_H_ */
