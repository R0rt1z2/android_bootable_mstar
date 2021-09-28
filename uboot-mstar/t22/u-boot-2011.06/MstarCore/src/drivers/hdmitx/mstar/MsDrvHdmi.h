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
#ifndef _MS_DRV_HDMI_H_
#define _MS_DRV_HDMI_H_

typedef enum E_MW_Picture_Dispout_Timing
{
    E_MW_PICTURE_DISPOUT_TIMING_480_60I         = 0,
    E_MW_PICTURE_DISPOUT_TIMING_480_60P         = 1,
    E_MW_PICTURE_DISPOUT_TIMING_576_50I         = 2,
    E_MW_PICTURE_DISPOUT_TIMING_576_50P         = 3,
    E_MW_PICTURE_DISPOUT_TIMING_720_50P         = 4,
    E_MW_PICTURE_DISPOUT_TIMING_720_60P         = 5,
    E_MW_PICTURE_DISPOUT_TIMING_1080_24P        = 6,
    E_MW_PICTURE_DISPOUT_TIMING_1080_25P        = 7,
    E_MW_PICTURE_DISPOUT_TIMING_1080_30P        = 8,
    E_MW_PICTURE_DISPOUT_TIMING_1080_50I        = 9,
    E_MW_PICTURE_DISPOUT_TIMING_1080_50P        = 10,
    E_MW_PICTURE_DISPOUT_TIMING_1080_60I        = 11,
    E_MW_PICTURE_DISPOUT_TIMING_1080_60P        = 12,
    E_MW_PICTURE_DISPOUT_TIMING_1440_24P        = 13,
    E_MW_PICTURE_DISPOUT_TIMING_1440_30P        = 14,
    E_MW_PICTURE_DISPOUT_TIMING_1440_50P        = 15,
    E_MW_PICTURE_DISPOUT_TIMING_1440_60P        = 16,
    E_MW_PICTURE_DISPOUT_TIMING_1470_24P        = 17,
    E_MW_PICTURE_DISPOUT_TIMING_1470_30P        = 18,
    E_MW_PICTURE_DISPOUT_TIMING_1470_50P        = 19,
    E_MW_PICTURE_DISPOUT_TIMING_1470_60P        = 20,
    E_MW_PICTURE_DISPOUT_TIMING_1920x2205_24P   = 21,
    E_MW_PICTURE_DISPOUT_TIMING_1920x2205_30P   = 22,
    E_MW_PICTURE_DISPOUT_TIMING_2205_24P        = 23,
    E_MW_PICTURE_DISPOUT_TIMING_4K1K_60P        = 24,
    E_MW_PICTURE_DISPOUT_TIMING_4K1K_120P       = 25,
    E_MW_PICTURE_DISPOUT_TIMING_4K2K_24P        = 26,
    E_MW_PICTURE_DISPOUT_TIMING_4K2K_25P        = 27,
    E_MW_PICTURE_DISPOUT_TIMING_4K2K_30P        = 28,
    E_MW_PICTURE_DISPOUT_TIMING_4K2K_50P        = 29,
    E_MW_PICTURE_DISPOUT_TIMING_4K2K_60P        = 30,
    E_MW_PICTURE_DISPOUT_TIMING_NOT_SUPPORT     = 0xFF,
} E_MW_Picture_Dispout_Timing_e;


#endif /* _MS_DRV_HDMI_H_ */
