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

#define    RIUBASE_PM_RIU_DBG          0x000100    //0x000100 ~ 0x0001ff
#define    RIUBASE_MENULOAD            0x000200    //0x000200 ~ 0x00021f
#define    RIUBASE_GDMA                0x000300    //0x000300 ~ 0x0003ff
#define    RIUBASE_DDC                 0x000400    //0x000400 ~ 0x0004ff
#define    RIUBASE_ISP                 0x000800    //0x000800 ~ 0x0008bf
#define    RIUBASE_FSP                 0x0008c0    //0x0008c0 ~ 0x0008df
#define    RIUBASE_QSPI                0x0008e0    //0x0008e0 ~ 0x0008ff
#define    RIUBASE_PM_SLEEP            0x000e00    //0x000e00 ~ 0x000eff
#define    RIUBASE_PM_GPIO             0x000f00    //0x000f00 ~ 0x000fff
#define    RIUBASE_MCU                 0x001000    //0x001000 ~ 0x0010ff
#define    RIUBASE_PM_CEC              0x001100    //0x001100 ~ 0x0011ff
#define    RIUBASE_PM_RTC              0x001200    //0x001200 ~ 0x0012ff
#define    RIUBASE_PM_RTC2             0x001300    //0x001300 ~ 0x0013ff
#define    RIUBASE_PM_SAR              0x001400    //0x001400 ~ 0x0014ff
#define    RIUBASE_PM_AV_LNK           0x001500    //0x001500 ~ 0x0015ff
#define    RIUBASE_PM_TOP              0x001e00    //0x001e00 ~ 0x001eff
#define    RIUBASE_EFUSE               0x002000    //0x002000 ~ 0x0020ff
#define    RIUBASE_IRQ                 0x002b00    //0x002b00 ~ 0x002b7f
#define    RIUBASE_CACHE               0x002b80    //0x002b80 ~ 0x002bbf
#define    RIUBASE_XDMIU               0x002bc0    //0x002bc0 ~ 0x002bff
#define    RIUBASE_PM_MISC             0x002e00    //0x002e00 ~ 0x002eff
#define    RIUBASE_WDT                 0x003000    //0x003000 ~ 0x00301f
#define    RIUBASE_TIMER0              0x003020    //0x003020 ~ 0x00303f
#define    RIUBASE_TIMER1              0x003040    //0x003040 ~ 0x00305f
#define    RIUBASE_SEC_KEY             0x003100    //0x003100 ~ 0x0031ff
#define    RIUBASE_DID_KEY             0x003800    //0x003800 ~ 0x0038ff
#define    RIUBASE_REG_PIU_MISC_0      0x003c00    //0x003c00 ~ 0x003cff
#define    RIUBASE_IR                  0x003d00    //0x003d00 ~ 0x003dff
#define    RIUBASE_PM_SPARE0           0x003e00    //0x003e00 ~ 0x003eff
#define    RIUBASE_PM_SPARE1           0x003f00    //0x003f00 ~ 0x003fff
