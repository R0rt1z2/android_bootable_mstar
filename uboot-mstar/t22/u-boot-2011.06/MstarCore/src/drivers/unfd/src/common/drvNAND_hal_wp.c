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

//#include <linux/string.h>
#include "drvNAND.h"
#include "drvNAND_utl.h"

#if defined (CONFIG_NAND_SECURITY_GEN) && CONFIG_NAND_SECURITY_GEN==1

#define TZPC_REG_BASE_ADDR    (((0x1239*0x80)<<REG_OFFSET_SHIFT_BITS)+RIU_PM_BASE)

#define SECURE_CH_CNT         4

#define reg_tee_fcie_wp1_startaddr_3116    GET_REG_ADDR(TZPC_REG_BASE_ADDR, 0x35)
#define reg_tee_fcie_wp1_startaddr_1500    GET_REG_ADDR(TZPC_REG_BASE_ADDR, 0x34)
#define reg_tee_fcie_wp1_endaddr_3116      GET_REG_ADDR(TZPC_REG_BASE_ADDR, 0x37)
#define reg_tee_fcie_wp1_endaddr_1500      GET_REG_ADDR(TZPC_REG_BASE_ADDR, 0x36)

#define reg_tee_fcie_wp2_startaddr_3116    GET_REG_ADDR(TZPC_REG_BASE_ADDR, 0x39)
#define reg_tee_fcie_wp2_startaddr_1500    GET_REG_ADDR(TZPC_REG_BASE_ADDR, 0x38)
#define reg_tee_fcie_wp2_endaddr_3116      GET_REG_ADDR(TZPC_REG_BASE_ADDR, 0x3b)
#define reg_tee_fcie_wp2_endaddr_1500      GET_REG_ADDR(TZPC_REG_BASE_ADDR, 0x3a)

#define reg_tee_fcie_wp3_startaddr_3116    GET_REG_ADDR(TZPC_REG_BASE_ADDR, 0x3d)
#define reg_tee_fcie_wp3_startaddr_1500    GET_REG_ADDR(TZPC_REG_BASE_ADDR, 0x3c)
#define reg_tee_fcie_wp3_endaddr_3116      GET_REG_ADDR(TZPC_REG_BASE_ADDR, 0x3f)
#define reg_tee_fcie_wp3_endaddr_1500      GET_REG_ADDR(TZPC_REG_BASE_ADDR, 0x3e)

#define reg_tee_fcie_wp0_startaddr_3116    GET_REG_ADDR(TZPC_REG_BASE_ADDR, 0x61)
#define reg_tee_fcie_wp0_startaddr_1500    GET_REG_ADDR(TZPC_REG_BASE_ADDR, 0x60)
#define reg_tee_fcie_wp0_endaddr_3116      GET_REG_ADDR(TZPC_REG_BASE_ADDR, 0x63)
#define reg_tee_fcie_wp0_endaddr_1500      GET_REG_ADDR(TZPC_REG_BASE_ADDR, 0x62)

#define reg_tee_fcie_wp_ctrl               GET_REG_ADDR(TZPC_REG_BASE_ADDR, 0x64)
#define bit_tee_fcie_wp_enbase             0
#define bit_tee_fcie_wp0_en                BIT0
#define bit_tee_fcie_wp1_en                BIT1
#define bit_tee_fcie_wp2_en                BIT2
#define bit_tee_fcie_wp3_en                BIT3
#define bit_tee_fcie_wp_lockbase           8
#define bit_tee_fcie_wp0_lock              BIT8
#define bit_tee_fcie_wp1_lock              BIT9
#define bit_tee_fcie_wp2_lock              BIT10
#define bit_tee_fcie_wp3_lock              BIT11

void drvNAND_SetSecureRange(U32 StartRow, U32 EndRow, U8 PortNo)
{
    switch(PortNo)
    {
        case 0:
            REG_WRITE_UINT16(reg_tee_fcie_wp0_startaddr_3116, StartRow >> 16);
            REG_WRITE_UINT16(reg_tee_fcie_wp0_startaddr_1500, StartRow & 0xFFFF);
		    REG_WRITE_UINT16(reg_tee_fcie_wp0_endaddr_3116, EndRow >> 16);
            REG_WRITE_UINT16(reg_tee_fcie_wp0_endaddr_1500, EndRow & 0xFFFF);
            break;
        case 1:
            REG_WRITE_UINT16(reg_tee_fcie_wp1_startaddr_3116, StartRow >> 16);
            REG_WRITE_UINT16(reg_tee_fcie_wp1_startaddr_1500, StartRow & 0xFFFF);
		    REG_WRITE_UINT16(reg_tee_fcie_wp1_endaddr_3116, EndRow >> 16);
            REG_WRITE_UINT16(reg_tee_fcie_wp1_endaddr_1500, EndRow & 0xFFFF);
            break;
        case 2:
            REG_WRITE_UINT16(reg_tee_fcie_wp2_startaddr_3116, StartRow >> 16);
            REG_WRITE_UINT16(reg_tee_fcie_wp2_startaddr_1500, StartRow & 0xFFFF);
		    REG_WRITE_UINT16(reg_tee_fcie_wp2_endaddr_3116, EndRow >> 16);
            REG_WRITE_UINT16(reg_tee_fcie_wp2_endaddr_1500, EndRow & 0xFFFF);
            break;
        case 3:
            REG_WRITE_UINT16(reg_tee_fcie_wp3_startaddr_3116, StartRow >> 16);
            REG_WRITE_UINT16(reg_tee_fcie_wp3_startaddr_1500, StartRow & 0xFFFF);
		    REG_WRITE_UINT16(reg_tee_fcie_wp3_endaddr_3116, EndRow >> 16);
            REG_WRITE_UINT16(reg_tee_fcie_wp3_endaddr_1500, EndRow & 0xFFFF);
            break;            
    }
}

void drvNAND_EnSecureRange(U8 PortNo)
{
    if(PortNo > SECURE_CH_CNT-1) // Ch.0~3
        return;

    REG_SET_BITS_UINT16(reg_tee_fcie_wp_ctrl, 1<<(PortNo+bit_tee_fcie_wp_enbase));
}

void drvNAND_DisSecureRange(U8 PortNo)
{
    if(PortNo > SECURE_CH_CNT-1) // Ch.0~3
        return;

    REG_CLR_BITS_UINT16(reg_tee_fcie_wp_ctrl, 1<<(PortNo+bit_tee_fcie_wp_enbase));    
}

void drvNAND_LockSecureRange(U8 PortNo)
{
    if(PortNo > SECURE_CH_CNT-1) // Ch.0~3
        return;

    REG_SET_BITS_UINT16(reg_tee_fcie_wp_ctrl, 1<<(PortNo+bit_tee_fcie_wp_lockbase));
}


#elif defined (CONFIG_NAND_SECURITY_GEN) && CONFIG_NAND_SECURITY_GEN==2




#endif 

