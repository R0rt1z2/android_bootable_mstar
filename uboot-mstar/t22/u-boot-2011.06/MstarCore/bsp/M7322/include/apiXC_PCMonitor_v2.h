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

#ifndef _XC_PCMONITOR_H_
#define _XC_PCMONITOR_H_

#ifdef __cplusplus
extern "C"
{
#endif



typedef struct
{
    MS_U8 u8MaxWindowNum;
    MS_BOOL bReturnValue;
} stXC_PCMONITOR_INIT, *pstXC_PCMONITOR_INIT;

typedef struct
{
    SCALER_WIN eWindow;
} stXC_PCMONITOR_RESTART, *pstXC_PCMONITOR_RESTART;

typedef struct
{
    MS_U16 u16TimingStableCounter;
    MS_U16 u16TimingNosyncCounter;
} stXC_PCMONITOR_SETTIMINGCOUNT, *pstXC_PCMONITOR_SETTIMINGCOUNT;

typedef struct
{
    INPUT_SOURCE_TYPE_t eCurrentSrc;
    SCALER_WIN eWindow;
    XC_PCMONITOR_STATUS eReturnValue;
} stXC_PCMONITOR, *pstXC_PCMONITOR;

typedef struct
{
    SCALER_WIN eWindow;
    XC_PCMONITOR_STATUS eReturnValue;
} stXC_PCMONITOR_GETCURRENTSTATE, *pstXC_PCMONITOR_GETCURRENTSTATE;

typedef struct
{
    SCALER_WIN eWindow;
    MS_U8 u8ReturnValue;
} stXC_PCMONITOR_GETSYNCSTATUS, *pstXC_PCMONITOR_GETSYNCSTATUS;

typedef struct
{
    SCALER_WIN eWindow;
    MS_U16 u16ReturnValue;
} stXC_PCMONITOR_GET_HFREQX10, *pstXC_PCMONITOR_GET_HFREQX10;

typedef struct
{
    SCALER_WIN eWindow;
    MS_U32 u32ReturnValue;
} stXC_PCMONITOR_GET_HFREQX1K, *pstXC_PCMONITOR_GET_HFREQX1K;

typedef struct
{
    SCALER_WIN eWindow;
    MS_U16 u16ReturnValue;
} stXC_PCMONITOR_GET_VFREQX10, *pstXC_PCMONITOR_GET_VFREQX10;

typedef struct
{
    SCALER_WIN eWindow;
    MS_U32 u32ReturnValue;
} stXC_PCMONITOR_GET_VFREQX1K, *pstXC_PCMONITOR_GET_VFREQX1K;

typedef struct
{
    SCALER_WIN eWindow;
    MS_U16 u16ReturnValue;
} stXC_PCMONITOR_GET_VTOTAL, *pstXC_PCMONITOR_GET_VTOTAL;

typedef struct DLL_PACKED
{
    SCALER_WIN eWindow;
    MS_WINDOW_TYPE* msWin;
} stXC_PCMONITOR_GET_DVI_HDMI_DE_INFO, *pstXC_PCMONITOR_GET_DVI_HDMI_DE_INFO;

typedef struct
{
    SCALER_WIN eWindow;
    MS_BOOL bReturnValue;
} stXC_PCMONITOR_SYNCLOSS, *pstXC_PCMONITOR_SYNCLOSS;

typedef struct
{
    MS_BOOL bPollingOnly;
    SCALER_WIN eWindow;
    MS_BOOL bReturnValue;
} stXC_PCMONITOR_INVALIDTIMINGDETECT, *pstXC_PCMONITOR_INVALIDTIMINGDETECT;

typedef struct
{
    INPUT_SOURCE_TYPE_t eCurrentSrc;
    MS_U16 u16TimingStableCounter;
    MS_U16 u16TimingNosyncCounter;
    E_APIXC_ReturnValue eReturnValue;
} stXC_PCMONITOR_SETTIMINGCOUNTEX, *pstXC_PCMONITOR_SETTIMINGCOUNTEX;

#ifdef __cplusplus
}
#endif

#endif //_XC_PCMONITOR_H_

