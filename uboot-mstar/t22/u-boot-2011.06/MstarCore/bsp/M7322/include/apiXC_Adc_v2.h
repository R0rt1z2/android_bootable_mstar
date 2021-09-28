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

#ifndef _XC_ADC_H_
#define _XC_ADC_H_

#ifdef __cplusplus
extern "C"
{
#endif


typedef struct
{
    MS_BOOL bEnable;
    E_DEST_TYPE enOutputType;
    INPUT_SOURCE_TYPE_t enInputPortType;
    MS_BOOL isDigitalSource;
} stXC_ADC_SETCVBSOUT, *pstXC_ADC_SETCVBSOUT;


typedef struct
{
    E_DEST_TYPE enOutputType;
    MS_BOOL bReturnValue;
} stXC_ADC_ISCVBSOUTENABLED, *pstXC_ADC_ISCVBSOUTENABLED;

typedef struct
{
    MS_U16 u16Value;
} stXC_ADC_SETPCCLOCK, *pstXC_ADC_SETPCCLOCK;

typedef struct
{
    MS_U8 u8Value;
} stXC_ADC_SETPHASE, *pstXC_ADC_SETPHASE;

typedef struct
{
    MS_U16 u16Value;
} stXC_ADC_SETPHASEEX, *pstXC_ADC_SETPHASEEX;

typedef struct
{
    MS_U16 u16ReturnValue;
} stXC_ADC_GETPHASERANGE, *pstXC_ADC_GETPHASERANGE;

typedef struct
{
    MS_U8 u8ReturnValue;
} stXC_ADC_GETPHASE, *pstXC_ADC_GETPHASE;

typedef struct
{
    MS_U16 u16ReturnValue;
} stXC_ADC_GETPHASEEX, *pstXC_ADC_GETPHASEEX;

typedef struct
{
    MS_BOOL bReturnValue;
} stXC_ADC_ISSCARTRGB, *pstXC_ADC_ISSCARTRGB;

typedef struct
{
    MS_U16 u16ReturnValue;
} stXC_ADC_GETPCCLOCK, *pstXC_ADC_GETPCCLOCK;

typedef struct DLL_PACKED
{
    MS_U32 *u32min;
    MS_U32 *u32max;
    MS_U32 *u32Recommend_value;
} stXC_ADC_GETSOGLEVELRANGE, *pstXC_ADC_GETSOGLEVELRANGE;

typedef struct
{
    MS_U32 u32Value;
} stXC_ADC_SETSOGLEVEL, *pstXC_ADC_SETSOGLEVEL;


typedef struct
{
    INPUT_SOURCE_TYPE_t enSource;
    APIXC_AdcGainOffsetSetting* pstADCSetting;
} stXC_ADC_GETDEFAULTGAINOFFSET, *pstXC_ADC_GETDEFAULTGAINOFFSET;

typedef struct
{
    MS_U16 u16ReturnValue;
} stXC_ADC_GETMAXIMALOFFSETVALUE, *pstXC_ADC_GETMAXIMALOFFSETVALUE;

typedef struct
{
    MS_U16 u16ReturnValue;
} stXC_ADC_GETMAXIMALGAINVALUE, *pstXC_ADC_GETMAXIMALGAINVALUE;

typedef struct
{
    MS_U16 u16ReturnValue;
} stXC_ADC_GETCENTERGAIN, *pstXC_ADC_GETCENTERGAIN;

typedef struct
{
    MS_U16 u16ReturnValue;
} stXC_ADC_GETCENTEROFFSET, *pstXC_ADC_GETCENTEROFFSET;

typedef struct
{
    MS_U8 u8Color;
    MS_U16 u16Value;
} stXC_ADC_SETGAIN, *pstXC_ADC_SETGAIN;

typedef struct
{
    MS_U8 u8Color;
    MS_U16 u16Value;
} stXC_ADC_SETOFFSET, *pstXC_ADC_SETOFFSET;

typedef struct DLL_PACKED
{
    APIXC_AdcGainOffsetSetting* pstADCSetting;
} stXC_ADC_ADJUSTGAINOFFSET, *pstXC_ADC_ADJUSTGAINOFFSET;

typedef struct
{
    INPUT_SOURCE_TYPE_t enInputSourceType;
} stXC_ADC_SOURCE_CALIBRATE, *pstXC_ADC_SOURCE_CALIBRATE;

typedef struct
{
    MS_U8 u8Value;
} stXC_ADC_SETRGB_PIPE_DELAY, *pstXC_ADC_SETRGB_PIPE_DELAY;

typedef struct
{
    MS_U16 u16Clpdly;
    MS_U16 u16Caldur;
} stXC_ADC_SCARTRGB_SOG_CLAMPDELAY, *pstXC_ADC_SCARTRGB_SOG_CLAMPDELAY;

typedef struct
{
    MS_BOOL bEnable;
} stXC_ADC_SET_YPBPRLOOSELPF, *pstXC_ADC_SET_YPBPRLOOSELPF;

typedef struct
{
    MS_U16 u16Value;
} stXC_ADC_SET_SOGBW, *pstXC_ADC_SET_SOGBW;

typedef struct
{
    MS_U16 u16Value;
} stXC_ADC_SETCLAMPDURATION, *pstXC_ADC_SETCLAMPDURATION;

typedef struct
{
    MS_BOOL bEnable;
    MS_BOOL bReturnValue;
} stXC_ADC_ENABLEHWCALIBRATION, *pstXC_ADC_ENABLEHWCALIBRATION;

typedef struct
{
    ADC_DacCurrentMode_t eMode;
    MS_BOOL bReturnValue;
} stXC_ADC_SETIDACCURRENTMODE, *pstXC_ADC_SETIDACCURRENTMODE;

typedef struct
{
    ADC_DacCurrentMode_t eReturnMode;
} stXC_ADC_GETIDACCURRENTMODE, *pstXC_ADC_GETIDACCURRENTMODE;


#ifdef __cplusplus
}
#endif

#endif //_XC_ADC_H_
