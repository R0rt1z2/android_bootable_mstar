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

#ifndef _XC_AUTO_H_
#define _XC_AUTO_H_

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct DLL_PACKED
{
    XC_Auto_TuneType enAutoTuneType;
    XC_Auto_Signal_Info *ActiveInfo;
    XC_Auto_Signal_Info *StandardInfo;
    SCALER_WIN eWindow;
    MS_BOOL bReturnValue;
} stXC_AUTO_GEOMETRY, *pstXC_AUTO_GEOMETRY;

typedef struct DLL_PACKED
{
    XC_Auto_TuneType enAutoTuneType;
    XC_Auto_Signal_Info_Ex *ActiveInfo;
    XC_Auto_Signal_Info_Ex *StandardInfo;
    SCALER_WIN eWindow;
    MS_BOOL bReturnValue;
} stXC_AUTO_GEOMETRY_EX, *pstXC_AUTO_GEOMETRY_EX;

typedef struct
{
    MS_BOOL bReturnValue;
} stXC_AUTO_STOPAUTOGEOMETRY, *pstXC_AUTO_STOPAUTOGEOMETRY;

typedef struct DLL_PACKED
{
    XC_Auto_CalibrationType type;
    XC_Auto_TuneType enAutoTuneType;
    APIXC_AdcGainOffsetSetting *pstADCSetting;
    SCALER_WIN eWindow;
    MS_BOOL bReturnValue;
} stXC_AUTO_GAINOFFSET, *pstXC_AUTO_GAINOFFSET;

typedef struct DLL_PACKED
{
    XC_Auto_TuneType enAutoTuneType;
    APIXC_AdcGainOffsetSetting *pstADCSetting;
    MS_BOOL bReturnValue;
} stXC_AUTO_GETHWFIXEDGAINOFFSET, *pstXC_AUTO_GETHWFIXEDGAINOFFSET;

typedef struct
{
    MS_U8 Value;
    MS_BOOL bReturnValue;
} stXC_AUTO_SETVALIDDATA, *pstXC_AUTO_SETVALIDDATA;

typedef struct
{
    MS_BOOL bEnable;
    MS_BOOL bIsYPbPrFlag;
} stXC_AUTO_AUTOOFFSET, *pstXC_AUTO_AUTOOFFSET;

typedef struct DLL_PACKED
{
    XC_Auto_Signal_Info_Ex *pSignalInfo;
    SCALER_WIN eWindow;
    MS_BOOL bReturnValue;
} stXC_AUTO_DETECTWIDTH, *pstXC_AUTO_DETECTWIDTH;

typedef struct
{
    XC_Auto_CalibrationMode eMode;
    MS_BOOL bReturnValue;
} stXC_AUTO_SETCALIBRATIONMODE, *pstXC_AUTO_SETCALIBRATIONMODE;

typedef struct DLL_PACKED
{
    XC_Auto_CalibrationMode* eMode;
} stXC_AUTO_GETCALIBRATIONMODE, *pstXC_AUTO_GETCALIBRATIONMODE;

typedef struct DLL_PACKED
{
    XC_AUTO_SYNC_INFO* pSyncInfo;
} stXC_AUTO_GETSYNCINFO, *pstXC_AUTO_GETSYNCINFO;

#ifdef __cplusplus
}
#endif

#endif //_XC_AUTO_H_

