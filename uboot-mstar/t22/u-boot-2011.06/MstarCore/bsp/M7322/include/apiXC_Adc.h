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


/*! \defgroup Video Video modules

 * \defgroup XC_SRC XC_SRC modules  
 *  \ingroup Video 
 
	ADC is used to control ADC relative setting for YPbPr, VGA, SCART, AV, SV, ATV.\n
	For more information,see \link ADC ADC interface (apiXC_ADC.h)  \endlink

 *  \defgroup ADC ADC interface (apiXC_ADC.h) 
 *  \ingroup XC_SRC     
 
    ADC is used to control ADC relative setting for YPbPr, VGA, SCART, AV, SV, ATV, including
    - CVBS out setting
    - Control ADC PC clock
    - Control ADC phase
    - Control SoG level
    - Control ADC gain and offset
    - ADC efuse setting( bandgap and iDac)
    - ADC calibration
    - Control PIPE delay
    - Control ADC clamp delay

    <b> Operation Code Flow: </b> \n
    check flow chart directly.
    \image html ADC.png

 *  \defgroup ADC_FEATURE ADC feature operation
 *  \ingroup ADC     

 *! \defgroup ADC_INFO ADC Infomation pool
 *  \ingroup ADC     

 *! \defgroup ADC_ToBeModified ADC api to be modified 
 *  \ingroup ADC     

 *! \defgroup ADC_ToBeRemove ADC api to be removed 
 *  \ingroup ADC 
 */

#ifndef _API_XC_ADC_H_
#define _API_XC_ADC_H_

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
typedef struct DLL_PACKED
{
    MS_U16 u16RedGain;      ///< ADC red gain
    MS_U16 u16GreenGain;    ///< ADC green gain
    MS_U16 u16BlueGain;     ///< ADC blue gain
    MS_U16 u16RedOffset;    ///< ADC red offset
    MS_U16 u16GreenOffset;  ///< ADC green offset
    MS_U16 u16BlueOffset;   ///< ADC blue offset
} APIXC_AdcGainOffsetSetting;

/// ADC output internal voltage type
typedef enum
{
    E_ADC_Internal_0V,      ///< Output   0V
    E_ADC_Internal_0_1V,    ///< Output 0.1V
    E_ADC_Internal_0_6V,    ///< Output 0.6V
    E_ADC_Internal_None,    ///< OFF
}ADC_Internal_Voltage;

/// ADC output Clock type
typedef enum
{
    E_ADC_Gen_480P_Clk,     ///< ADC output 480P
    E_ADC_Gen_720P_Clk,     ///< ADC output 720P
    E_ADC_Gen_1080P_Clk,    ///< ADC output 1080P
}ADC_Gen_Clock_Type;

/// ADC V Clamping type
typedef enum
{
    E_ADC_VClamp_0_85V,     ///< Clamp to 0.85V
    E_ADC_VClamp_0_9V,      ///< Clamp to 0.9V
    E_ADC_VClamp_0_95V,     ///< Clamp to 0.95V
    E_ADC_VClamp_1_0V,      ///< Clamp to 1.0V
    E_ADC_VClamp_1_05V,     ///< Clamp to 1.05V
    E_ADC_VClamp_1_2V,      ///< Clamp to 1.2V
    E_ADC_VClamp_1_5V,      ///< Clamp to 1.5V
}ADC_VClamp_Level_Type;

/// Update Gain / Offset for specific channel. This is used in MApi_XC_ADC_SetGain or MApi_XC_ADC_SetOffset
typedef enum
{
    ADC_CHR = 0,    ///< Channel R
    ADC_CHG,        ///< Channel G
    ADC_CHB,        ///< Channel B
    ADC_ALL,        ///< All Channel
}ADC_CHANNEL_t;

/// Software reset for ADCDVIPLL block
typedef enum
{
    REST_ATOP        = _BIT(7),     ///< Reset ATOP
    REST_HDMI        = _BIT(6),
    REST_HDCP        = _BIT(5),
    REST_DVI         = _BIT(4),
    REST_PLL_DIG_B   = _BIT(3),
    REST_ADC_DIG_B   = _BIT(2),
    REST_PLL_DIG_A   = _BIT(1),
    REST_ADC_DIG_A   = _BIT(0),
    REST_PLL         = ( REST_PLL_DIG_B | REST_PLL_DIG_A ), ///< Reset PLL
    REST_ADC         = ( REST_ADC_DIG_B | REST_ADC_DIG_A ), ///< Reset All ADC
} ADC_RESET;

typedef enum
{
    E_ADC_IDAC_FULL_MODE,
    E_ADC_IDAC_HALF_MODE,
    E_ADC_IDAC_QUART_MODE,
    E_ADC_IDAC_MODE_MAX,
} ADC_DacCurrentMode_t;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/// Enable and Disable CVBS out. This will setup ADC part for CVBS out (monitor out).
/// @ingroup ADC_FEATURE
/// @param bEnable \b IN Enable CVBS Out
/// @param enInputPortType \b IN The source need to monitor out.
/// @param isDigitalSource \b IN Tell the driver current source is digital source or not. (ex. If internal VIF used in ATV, it will be digital source)
//------------------------------------------------------------------------------
void    MApi_XC_ADC_SetCVBSOut(MS_BOOL bEnable, E_DEST_TYPE enOutputType , INPUT_SOURCE_TYPE_t enInputPortType, MS_BOOL isDigitalSource);
//------------------------------------------------------------------------------
/// Check the CVBS out enable status. 
/// @ingroup ADC_INFO
/// @param bEnable \b IN Enable CVBS Out
/// @return @ref MS_BOOL 
//------------------------------------------------------------------------------
MS_BOOL MApi_XC_ADC_IsCVBSOutEnabled(E_DEST_TYPE enOutputType);

//------------------------------------------------------------------------------
/// Set ADC Sample clock
/// @ingroup ADC_FEATURE
/// @param u16Value \b IN value need to be set.
//------------------------------------------------------------------------------
void    MApi_XC_ADC_SetPcClock(MS_U16 u16Clock);

//------------------------------------------------------------------------------
/// Set ADC Phase, pls use MApi_XC_ADC_SetPhaseEx instead
/// @ingroup ADC_ToBeRemove
/// @param u8Value \b IN The adc phase need to be update
//------------------------------------------------------------------------------
void    MApi_XC_ADC_SetPhase(MS_U8 u8Value);

//------------------------------------------------------------------------------
/// Set ADC Phase for extend bit
/// @ingroup ADC_ToBeModified
/// @param u16Value \b IN The adc phase need to be update
//------------------------------------------------------------------------------
void    MApi_XC_ADC_SetPhaseEx(MS_U16 u16Value);

//------------------------------------------------------------------------------
/// Get current ADC Phase set
/// @ingroup ADC_INFO
/// @return @ref MS_U16  \b Current Phase
//------------------------------------------------------------------------------
MS_U16     MApi_XC_ADC_GetPhaseRange( void );

//------------------------------------------------------------------------------
/// Get current ADC Phase set
/// @ingroup ADC_ToBeRemove
/// @return @ref MS_U8  \b Current Phase
//------------------------------------------------------------------------------
MS_U8    MApi_XC_ADC_GetPhase( void );

//------------------------------------------------------------------------------
/// Get current ADC Phase set for extend bit
/// @ingroup ADC_ToBeModified
/// @return @ref MS_U16  \b Current Phase
//------------------------------------------------------------------------------
MS_U16   MApi_XC_ADC_GetPhaseEx( void );

//------------------------------------------------------------------------------
/// Get the status for Scart RGB
/// @ingroup ADC_INFO
/// @return @ref MS_BOOL
//------------------------------------------------------------------------------
MS_BOOL MApi_XC_ADC_IsScartRGB(void);

//------------------------------------------------------------------------------
/// Get the status for PC clock
/// @ingroup ADC_INFO
/// @return @ref MS_U16
//------------------------------------------------------------------------------
MS_U16  MApi_XC_ADC_GetPcClock(void );

//------------------------------------------------------------------------------
///Get SOG level range
/// @ingroup ADC_INFO
///@param u32Min \b OUT: min of SOG level
///@param u32Max \b OUT: max of SOG level
///@param u32Recommend_value \b OUT: recommend value
//------------------------------------------------------------------------------
void    MApi_XC_ADC_GetSoGLevelRange(MS_U32 *u32min, MS_U32 *u32max, MS_U32 *u32Recommend_value);

//------------------------------------------------------------------------------
///Set SOG level
/// @ingroup ADC_FEATURE
///@param u32Min \b IN: set SOG level
//------------------------------------------------------------------------------
void    MApi_XC_ADC_SetSoGLevel(MS_U32 u32Value);

// power
//------------------------------------------------------------------------------
/// Turn off ADC
/// @ingroup ADC_FEATURE
//------------------------------------------------------------------------------
void    MApi_XC_ADC_PowerOff(void);

// ADC calibration
//------------------------------------------------------------------------------
/// Get ADC Default gain / offset by referring input structure.
/// @ingroup ADC_INFO
/// @param eSource \b IN The input source
/// @param pstADCSetting \b IN gain / offset need to be set
//------------------------------------------------------------------------------
void MApi_XC_ADC_GetDefaultGainOffset(INPUT_SOURCE_TYPE_t eSource , APIXC_AdcGainOffsetSetting* pstADCSetting);

//------------------------------------------------------------------------------
/// get maximal value of offset
/// @ingroup ADC_INFO
/// @param
/// @return maximal value of offset
//------------------------------------------------------------------------------
MS_U16 MApi_XC_ADC_GetMaximalOffsetValue(void);

//------------------------------------------------------------------------------
/// get maximal value of gain
/// @ingroup ADC_INFO
/// @param
/// @return @ref MS_U16  \b maximal value of gain
//------------------------------------------------------------------------------
MS_U16  MApi_XC_ADC_GetMaximalGainValue(void);

//------------------------------------------------------------------------------
/// get center value of gain
/// @ingroup ADC_INFO
/// @param
/// @return @ref MS_U16  \b return center value of gain
//------------------------------------------------------------------------------
MS_U16 MApi_XC_ADC_GetCenterGain(void);

//------------------------------------------------------------------------------
/// get center value of offset
/// @ingroup ADC_INFO
/// @param
/// @return @ref MS_U16  \b return center value of offset
//------------------------------------------------------------------------------
MS_U16 MApi_XC_ADC_GetCenterOffset(void);

// Set Gain & Offset
// u8Color is one of ADC_CHANNEL_t.
//------------------------------------------------------------------------------
/// Set Gain value
/// @ingroup ADC_FEATURE
/// @param u8color \b IN The channel type need to be set
/// @param u16value \b IN The value need to be set.
//------------------------------------------------------------------------------
void MApi_XC_ADC_SetGain(MS_U8 u8Color, MS_U16 u16Value);

//------------------------------------------------------------------------------
/// Set offset value
/// @ingroup ADC_FEATURE
/// @param u8color \b IN The channel type need to be set
/// @param u16value \b IN The value need to be set.
//------------------------------------------------------------------------------
void MApi_XC_ADC_SetOffset(MS_U8 u8Color, MS_U16 u16Value);

//------------------------------------------------------------------------------
/// Set ADC gain / offset by referring input structure.
/// @ingroup ADC_FEATURE
/// @param pstADCSetting \b IN gain / offset need to be set
//------------------------------------------------------------------------------
void MApi_XC_ADC_AdjustGainOffset(APIXC_AdcGainOffsetSetting* pstADCSetting);

//------------------------------------------------------------------------------
/// Set ADC Internal calibration. (need to be set during set mode)
/// @ingroup ADC_FEATURE
/// @param enInputSourceType \b IN current input source.
//------------------------------------------------------------------------------
void MApi_XC_ADC_Source_Calibrate(INPUT_SOURCE_TYPE_t enInputSourceType);

//------------------------------------------------------------------------------
///Set SOG level
/// @ingroup ADC_FEATURE
///@param u32Min \b IN: set SOG calibration
//------------------------------------------------------------------------------
void MApi_XC_ADC_SetSoGCal(void);

//------------------------------------------------------------------------------
//Set SCART RGB PIPE Delay
/// @ingroup ADC_FEATURE
///@param u8Value \b IN: set PIP Delay
//------------------------------------------------------------------------------
void MApi_XC_ADC_SetRGB_PIPE_Delay(MS_U8 u8Value);

//------------------------------------------------------------------------------
//Set Scart RGB Sync on green clamp delay and duration value
/// @ingroup ADC_FEATURE
///@param u16Clpdly \b IN: set clamp Delay
///@param u16Caldur \b IN: set clamp duration
//------------------------------------------------------------------------------
void MApi_XC_ADC_ScartRGB_SOG_ClampDelay(MS_U16 u16Clpdly, MS_U16 u16Caldur);

//------------------------------------------------------------------------------
//Enable Loose LPF
/// @ingroup ADC_FEATURE
///@param benable \b IN: Enable Loose LPF
//------------------------------------------------------------------------------
void MApi_XC_ADC_Set_YPbPrLooseLPF(MS_BOOL benable);

//------------------------------------------------------------------------------
///Set SOG BW
/// @ingroup ADC_FEATURE
///@param u16value \b IN: set SOG BW
//------------------------------------------------------------------------------
void MApi_XC_ADC_Set_SOGBW(MS_U16 u16value);

//------------------------------------------------------------------------------
// Set Clamp Duration
/// @ingroup ADC_FEATURE
///@param u16Caldur \b IN: set clamp duration
//------------------------------------------------------------------------------
void MApi_XC_ADC_SetClampDuration(MS_U16 u16Value);

//------------------------------------------------------------------------------
///Enable HW calibration, will load hw adc table when changing source
/// @ingroup ADC_FEATURE
///@param bEnable \b IN: enable hw calibration
/// @return @ref MS_BOOL  \b return the status
//------------------------------------------------------------------------------
MS_BOOL MApi_XC_ADC_EnableHWCalibration(MS_BOOL bEnable);

//------------------------------------------------------------------------------
///This function set CVBS Out IDAC curret mode.
/// @ingroup ADC_FEATURE
///@param stMode \b IDAC current mode.
/// @return @ref MS_BOOL  \b return the status
//------------------------------------------------------------------------------
MS_BOOL MApi_XC_ADC_SetIdacCurrentMode(ADC_DacCurrentMode_t eMode);

//------------------------------------------------------------------------------
///This function get CVBS Out IDAC current mode.
/// @ingroup ADC_INFO
///@return IDAC current mode.
//------------------------------------------------------------------------------
ADC_DacCurrentMode_t MApi_XC_ADC_GetIdacCurrentMode(void);

#ifdef __cplusplus
}
#endif

#endif  // _API_XC_ADC_H_

