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
/// @brief  DLC API layer Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

/*! \defgroup Video Video modules

 *  \defgroup XC_BE XC_BE modules 
 *  \ingroup Video
 
	XC_BE is used for\n
	1. Dynamic Backlight Curve : Calculate the DBC base on average value.\n
	For more information,see \link DBC DBC interface (apiXC_DBC.h) \endlink

 *  \defgroup DBC DBC interface (apiXC_DBC.h) 
 *  \ingroup XC_BE     
 
    - Dynamic Backlight Curve : Calculate the DBC base on average value.

    <b> Operation Code Flow: </b> \n
    check flow chart directly.
    \image html apiXC_DLC_DBC_init_flow.png

 *  \defgroup DBC_INIT DBC init control 
 *  \ingroup DBC      

 *! \defgroup DBC_FEATURE DBC feature operation
 *  \ingroup DBC     

 *! \defgroup DBC_INFO DBC Infomation pool
 *  \ingroup DBC     

 *! \defgroup DBC_ToBeModified DBC api to be modified 
 *  \ingroup DBC     

 *! \defgroup DBC_ToBeRemove DBC api to be removed 
 *  \ingroup DBC 
 */


#ifndef _API_XC_DBC_H_
#define _API_XC_DBC_H_

#ifdef __cplusplus
extern "C"
{
#endif

//-------------------------------------------------------------------------------------------------
/// DLC DBC initilization
/// @ingroup DBC_INIT
// @param DLC_DBC_MFinit               \b IN: TBD
//-------------------------------------------------------------------------------------------------
void    MApi_XC_DLC_DBC_Init(XC_DLC_DBC_MFinit DLC_DBC_MFinit);

//-------------------------------------------------------------------------------------------------
/// DBC initilization
/// @ingroup DBC_INIT
//-------------------------------------------------------------------------------------------------
void MApi_XC_DBC_Init(void);

//-------------------------------------------------------------------------------------------------
/// DLC DBC set status
/// @ingroup DBC_FEATURE
// @param bDBCEnable               \b IN: TBD
//-------------------------------------------------------------------------------------------------
void    MApi_XC_DLC_DBC_Setstatus(MS_BOOL bDBCEnable);

//-------------------------------------------------------------------------------------------------
/// DLC DBC get status
/// @ingroup DBC_INFO
/// @return @ref MS_BOOL
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_DLC_DBC_Getstatus(void);

//-------------------------------------------------------------------------------------------------
/// Set DBC Ready Mode 
/// @ingroup DBC_FEATURE
// @param bDBCReady               \b IN: TBD
//-------------------------------------------------------------------------------------------------
void MApi_XC_DLC_DBC_SetReady(MS_BOOL bDBCReady);

//-------------------------------------------------------------------------------------------------
/// Set DLC DBC Debug Mode 
/// @ingroup DBC_FEATURE
// @param ucDBC_DebugMode               \b IN: TBD
//-------------------------------------------------------------------------------------------------
void MApi_XC_DLC_DBC_SetDebugMode(MS_U8 ucDBC_DebugMode);

//-------------------------------------------------------------------------------------------------
/// DLC DBC Debug Mode Status
/// @ingroup DBC_INFO
/// @return @ref MS_U8
//-------------------------------------------------------------------------------------------------
MS_U8 MApi_XC_DLC_DBC_GetDebugMode(void);

//-------------------------------------------------------------------------------------------------
/// DLC DBC update PWM
/// @ingroup DBC_FEATURE
/// @param   u8PWMvalue           \b IN: PWM value to set the lumming
//-------------------------------------------------------------------------------------------------
void    MApi_XC_DLC_DBC_UpdatePWM(MS_U8 u8PWMvalue);

//-------------------------------------------------------------------------------------------------
/// DLC DBC handler
/// @ingroup DBC_FEATURE
/// @return @ref MS_U16
//-------------------------------------------------------------------------------------------------
MS_U16  MApi_XC_DLC_DBC_Handler(void);

//-------------------------------------------------------------------------------------------------
/// DLC DBC adjsut YC gain
/// @ingroup DBC_FEATURE
//-------------------------------------------------------------------------------------------------
void    MApi_XC_DLC_DBC_AdjustYCGain(void);

//-------------------------------------------------------------------------------------------------
/// DLC DBC YC gain initilization
/// @ingroup DBC_INIT
// @param u8YGain_M               \b IN: TBD
// @param u8YGain_L               \b IN: TBD
// @param u8CGain_M               \b IN: TBD
// @param u8CGain_L               \b IN: TBD
//-------------------------------------------------------------------------------------------------
void    MApi_XC_DLC_DBC_YCGainInit(MS_U8 u8YGain_M, MS_U8 u8YGain_L, MS_U8 u8CGain_M, MS_U8 u8CGain_L);

//-------------------------------------------------------------------------------------------------
/// DLC DBC reset
/// @ingroup DBC_FEATURE
//-------------------------------------------------------------------------------------------------
void    MApi_XC_DLC_DBC_Reset(void);

//-------------------------------------------------------------------------------------------------
/// @brief callback functions & struct used to support debug command.
//-------------------------------------------------------------------------------------------------
typedef void   (*MApi_XC_DBC_OnOff_Callback)(MS_BOOL/*On/Off*/);

//-------------------------------------------------------------------------------------------------
/// @brief callback functions & struct used to support debug command.
//-------------------------------------------------------------------------------------------------
typedef void   (*MApi_XC_DBC_AdjustBackLight_Callback)(MS_U32 /*PWM_VALUE*/);

//-------------------------------------------------------------------------------------------------
/// @brief structure used to support debug command.
//-------------------------------------------------------------------------------------------------
typedef struct DLL_PACKED {
    MS_U8 *pCmdBuff;                                            ///<1. command buffer from UART
    MApi_XC_DBC_OnOff_Callback           fnDBC_AP_OnOff;        ///<2. Function to control DBC on/off
    MApi_XC_DBC_AdjustBackLight_Callback fnDBC_AdjustBacklight; ///<3. Functoin to control backlight
    MApi_XC_DLC_Print_Callback           fnDBC_Putchar;         ///<4. User putchar function
}tDBC_CONTROL_PARAMS;

//-------------------------------------------------------------------------------------------------
/// @brief uart debug command decoder for Dynamic Backlight Control.
/// @ingroup DBC_FEATURE
/// @param params   passin params.  
//-------------------------------------------------------------------------------------------------
void MApi_XC_DBC_DecodeExtCmd(tDBC_CONTROL_PARAMS *params );

#ifdef __cplusplus
}
#endif

#endif // _API_XC_DBC_H_
