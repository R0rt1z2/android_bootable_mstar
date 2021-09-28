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

    //************************************************************
    //***             RIU Base address of each IP              ***
    //***                (Byte address [23:0])                 ***
    //************************************************************

#define    RIUBASE_FRC_RIU_DBG         0x300100    //0x300100 ~ 0x3001ff
#define    RIUBASE_FRC_INTR_CPUINT     0x300580    //0x300580 ~ 0x30059f
#define    RIUBASE_FRC_REG_HW_SEMAPH   0x300600    //0x300600 ~ 0x3006ff
#define    RIUBASE_LDM_DMA0            0x300900    //0x300900 ~ 0x3009ff
#define    RIUBASE_LDM_DMA1            0x300a00    //0x300a00 ~ 0x300aff
#define    RIUBASE_LDM_DMA2            0x300b00    //0x300b00 ~ 0x300bff
#define    RIUBASE_LDM_DMA3            0x300c00    //0x300c00 ~ 0x300cff
#define    RIUBASE_FRC_MAU0            0x300d00    //0x300d00 ~ 0x300d7f
#define    RIUBASE_FRC_R2              0x301000    //0x301000 ~ 0x3010ff
#define    RIUBASE_FRC_INTR_CTRL       0x301100    //0x301100 ~ 0x3011ff
#define    RIUBASE_FRC_MIU             0x301200    //0x301200 ~ 0x3012ff
#define    RIUBASE_PRAM                0x301500    //0x301500 ~ 0x3015ff
#define    RIUBASE_SC24_2D3D           0x301600    //0x301600 ~ 0x3016ff
#define    RIUBASE_ECC                 0x301800    //0x301800 ~ 0x3018ff
#define    RIUBASE_MSPI0               0x301900    //0x301900 ~ 0x3019ff
#define    RIUBASE_MSPI1               0x301a00    //0x301a00 ~ 0x301aff
#define    RIUBASE_MSPI2               0x301b00    //0x301b00 ~ 0x301bff
#define    RIUBASE_MSPI3               0x301c00    //0x301c00 ~ 0x301cff
#define    RIUBASE_FRC_CHIP            0x301e00    //0x301e00 ~ 0x301eff
#define    RIUBASE_FRC_VD_IRQ          0x301f00    //0x301f00 ~ 0x301fff
#define    RIUBASE_SC0_IP              0x302000    //0x302000 ~ 0x3020ff
#define    RIUBASE_SC1_VBI             0x302100    //0x302100 ~ 0x3021ff
#define    RIUBASE_SC2_LVDSRX          0x302200    //0x302200 ~ 0x3022ff
#define    RIUBASE_SC3_TCON            0x302300    //0x302300 ~ 0x3023ff
#define    RIUBASE_SC4_SCTOP           0x302400    //0x302400 ~ 0x3024ff
#define    RIUBASE_SC5_PWM             0x302500    //0x302500 ~ 0x3025ff
#define    RIUBASE_SC6_MCPLUS          0x302600    //0x302600 ~ 0x3026ff
#define    RIUBASE_SC7_MC_3FRAME       0x302700    //0x302700 ~ 0x3027ff
#define    RIUBASE_SC8_OD              0x302800    //0x302800 ~ 0x3028ff
#define    RIUBASE_SC9_MC2D            0x302900    //0x302900 ~ 0x3029ff
#define    RIUBASE_SC10_DISP_LPLL      0x302a00    //0x302a00 ~ 0x302aff
#define    RIUBASE_SC16_MC2D           0x302c00    //0x302c00 ~ 0x302cff
#define    RIUBASE_SC17_IP             0x302d00    //0x302d00 ~ 0x302dff
#define    RIUBASE_SC11_SNR            0x302e00    //0x302e00 ~ 0x302eff
#define    RIUBASE_SC12_DISP_TGEN      0x302f00    //0x302f00 ~ 0x302fff
#define    RIUBASE_SC13_VOP2           0x303000    //0x303000 ~ 0x3030ff
#define    RIUBASE_SC14_MFT            0x303100    //0x303100 ~ 0x3031ff
#define    RIUBASE_SC15_MISC           0x303200    //0x303200 ~ 0x3032ff
#define    RIUBASE_SC18_LD             0x303300    //0x303300 ~ 0x3033ff
#define    RIUBASE_SC19_OD_LOCAL_GAIN  0x303800    //0x303800 ~ 0x3038ff
#define    RIUBASE_SC20_MC2D           0x303a00    //0x303a00 ~ 0x303aff
#define    RIUBASE_SC21_NEW_FILM       0x303b00    //0x303b00 ~ 0x303bff
#define    RIUBASE_SC22_ADL            0x303d00    //0x303d00 ~ 0x303dff
#define    RIUBASE_SC25_HVSP           0x303e00    //0x303e00 ~ 0x303eff
#define    RIUBASE_FRC_EFUSE           0x303f00    //0x303f00 ~ 0x303fff
#define    RIUBASE_LGD_IP1             0x312000    //0x312000 ~ 0x3120ff
#define    RIUBASE_SC26_3DD            0x313000    //0x313000 ~ 0x3130ff
