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

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @brief  RF Channel Frequency High-Level API
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _API_RF_CHANNEL_H_
#define _API_RF_CHANNEL_H_

//---------------------------------------------------------------------------
//  Include file
//---------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
// COUNTRY_TYPE_CHINA
#define FREQ_CHINA_MIN              99000                               // KHz
#define FREQ_CHINA_MAX              866000                              // KHz
//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
#if 1//AUSTRALIA_FREQTBL_NEW
#define FREQ_TAL_SIZE   7
typedef struct  _FreqTblType
{
       MS_U8 u8CHNo;   //channel no.
       MS_U32 u32Freq;   //frequency
       MS_S8 u8CHNoOffset;  //No. offset ----->for displaying 0:= No.  +X:No.-X and add "A"   -X:No.-X
}FreqTblType;

#endif

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

typedef enum _EN_COUNTRY_TYPE
{
    COUNTRY_TYPE_EUROPE,
    COUNTRY_TYPE_GERMANY,
    COUNTRY_TYPE_TAIWAN,
    COUNTRY_TYPE_ITALY,
    COUNTRY_TYPE_FRANCE,
    COUNTRY_TYPE_CHINA,
    COUNTRY_TYPE_AUSTRALIA,
    COUNTRY_TYPE_BRAZIL,
    COUNTRY_TYPE_CHILE,
    COUNTRY_TYPE_HONGKONG,
    COUNTRY_TYPE_ARGENTINA,
    COUNTRY_TYPE_PERU,
    COUNTRY_TYPE_UK,
    COUNTRY_TYPE_OTHER,
    COUNTRY_TYPE_AUSTRALIA_NEW,
    COUNTRY_TYPE_NUM,

} EN_COUNTRY_TYPE;


//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

extern MS_BOOL MApi_RfChannel_GetTPSetting(MS_U8 u8RF_CH, MS_U8 u8Country, MS_U16 *u16BandWidth, MS_U32 *u32Frequency);
extern MS_U8   MApi_RfChannel_Freq2ChannelNumber(MS_U8 u8Country, MS_U32 u32Frequency);
extern MS_U8   MApi_RfChannel_GetMaxRfChNum(MS_U8 u8Country);
extern MS_U8   MApi_RfChannel_GetMinRfChNum(MS_U8 u8Country);
extern MS_U8   MApi_RfChannel_GetNextRfChNum(MS_U8 u8Country, MS_U8 u8RF_CH);
extern MS_U8   MApi_RfChannel_GetPrevtRfChNum(MS_U8 u8Country, MS_U8 u8RF_CH);

#endif // _API_RF_CHANNEL_H_

