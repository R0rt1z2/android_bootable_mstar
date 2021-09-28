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

#ifndef _MS_DRV_ROCKET2_
#define _MS_DRV_ROCKET2_
#include <common.h>
#include <MsTypes.h>

typedef enum
{
    E_MAPI_HDMITX_TIMING_480_60I,
    E_MAPI_HDMITX_TIMING_480_60P,
    E_MAPI_HDMITX_TIMING_576_50I,
    E_MAPI_HDMITX_TIMING_576_50P,
    E_MAPI_HDMITX_TIMING_720_50P,
    E_MAPI_HDMITX_TIMING_720_60P,
    E_MAPI_HDMITX_TIMING_1080_50I,
    E_MAPI_HDMITX_TIMING_1080_50P,
    E_MAPI_HDMITX_TIMING_1080_60I,
    E_MAPI_HDMITX_TIMING_1080_60P,
    E_MAPI_HDMITX_TIMING_1080_30P,
    E_MAPI_HDMITX_TIMING_1080_25P,
    E_MAPI_HDMITX_TIMING_1080_24P,
    E_MAPI_HDMITX_TIMING_4K2K_30P,
    E_MAPI_HDMITX_TIMING_1440_50P,
    E_MAPI_HDMITX_TIMING_1440_60P,
    E_MAPI_HDMITX_TIMING_1440_24P,
    E_MAPI_HDMITX_TIMING_1440_30P,
    E_MAPI_HDMITX_TIMING_1470_50P,
    E_MAPI_HDMITX_TIMING_1470_60P,
    E_MAPI_HDMITX_TIMING_1470_24P,
    E_MAPI_HDMITX_TIMING_1470_30P,
    E_MAPI_HDMITX_TIMING_2160_24P,
    E_MAPI_HDMITX_TIMING_2160_30P,
    E_MAPI_HDMITX_TIMING_4K2K_25P,
    E_MAPI_HDMITX_TIMING_4K1K_60P,
    E_MAPI_HDMITX_TIMING_2K2K_60P,
    E_MAPI_HDMITX_TIMING_2560x1440_60P,
    E_MAPI_HDMITX_TIMING_4K2K_24P,
    E_MAPI_HDMITX_TIMING_4K2K_50P,
    E_MAPI_HDMITX_TIMING_4K2K_60P,
    E_MAPI_HDMITX_TIMING_4K2K_60P_RGB10BIT,
    E_MAPI_HDMITX_TIMING_4096x2160_24P,
    E_MAPI_HDMITX_TIMING_MAX,
}EN_MAPI_HDMITX_TIMING_TYPE;

typedef enum
{
    E_MAPI_ROCKY_RES_640x480p        =0,
    E_MAPI_ROCKY_RES_720x480i         = 1,
    E_MAPI_ROCKY_RES_720x576i         = 2,
    E_MAPI_ROCKY_RES_720x480p         = 3,
    E_MAPI_ROCKY_RES_720x576p         = 4,
    E_MAPI_ROCKY_RES_1280x720p_50Hz   = 5,
    E_MAPI_ROCKY_RES_1280x720p_60Hz   = 6,
    E_MAPI_ROCKY_RES_1920x1080i_50Hz  = 7,
    E_MAPI_ROCKY_RES_1920x1080i_60Hz  = 8,
    E_MAPI_ROCKY_RES_1920x1080p_24Hz  = 9,
    E_MAPI_ROCKY_RES_1920x1080p_25Hz  = 10,
    E_MAPI_ROCKY_RES_1920x1080p_30Hz  = 11,
    E_MAPI_ROCKY_RES_1920x1080p_50Hz  = 12,
    E_MAPI_ROCKY_RES_1920x1080p_60Hz  = 13,
    E_MAPI_ROCKY_RES_4K2Kp_30Hz       = 14,
    E_MAPI_ROCKY_RES_1280x1470p_50Hz  = 15,
    E_MAPI_ROCKY_RES_1280x1470p_60Hz  = 16,
    E_MAPI_ROCKY_RES_1280x1470p_24Hz  = 17,
    E_MAPI_ROCKY_RES_1280x1470p_30Hz  = 18,
    E_MAPI_ROCKY_RES_1920x2205p_24Hz  = 19,
    E_MAPI_ROCKY_RES_1920x2205p_30Hz  = 20,
    E_MAPI_ROCKY_RES_4K2Kp_25Hz       = 21,
    E_MAPI_ROCKY_RES_4K2Kp_24Hz       = 22,
    E_MAPI_ROCKY_RES_4K2Kp_50Hz       = 23,
    E_MAPI_ROCKY_RES_4K2Kp_60Hz       = 24,
    E_MAPI_ROCKY_RES_4K1Kp_60Hz       = 25,
    E_MAPI_ROCKY_RES_2K2Kp_60Hz       = 26,
    E_MAPI_ROCKY_RES_2560x1440p_60Hz  = 27,
    E_MAPI_ROCKY_RES_4096x2160p_24Hz  = 28,
    E_MAPI_ROCKY_RES_4K2Kp_60Hz_RB    = 29,
    E_MAPI_ROCKY_RES_MAX              = 30,
} EN_MAPI_DEVICE_ROCKY_VIDEO_TIMING;//sync to apiHDMITx.h in device rocket

typedef enum
{
    E_MAPI_RAPTORS_GET_EDID_FAILED = 1,
    E_MAPI_RAPTORS_RESOLUTION_NOT_SUPPORT = 2,
    E_MAPI_RAPTORS_RESOLUTION_SUPPORT_SUCCESS = 3,
    E_MAPI_RAPTORS_RESOLUTION_BESTSUPPORT_SUCCESS = 4,
}E_MAPI_RAPTORS_SUPPORT_FORMAT;

MS_BOOL msHdmitx_Rocket_Init(EN_MAPI_DEVICE_ROCKY_VIDEO_TIMING outPutType );



#endif

