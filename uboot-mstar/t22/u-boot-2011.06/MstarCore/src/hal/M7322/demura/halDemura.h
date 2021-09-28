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
#ifndef _HAL_DEMURA_H_
#define _HAL_DEMURA_H_

#define DEMURA_MAX_LAYER             8
#define MSTAR_DEMURA_DAT_BIT_LEN     10
#define MSTAR_CHIP_ID                0x00E8     // For Merlin

#define REG_DEMURA_BASE              0x037700  // --> Bank_0x1377 (Main 0x102F, Sub 0x77)
#define REG_DEMURA_DMA_BASE          0x036700  // --> Bank_0x1367 (Main 0x102F, Sub 0x67)
#define DEMURA_DMA_ADDR_UNIT         32
#define DEMURA_DMA_REQ_LENGTH        4

#define REG_DMC_DGA_EN_BIT           BIT7
#define REG_DMC_W_EN_BIT             BIT6


#define BK_REG_L( x, y )            ((x) | (((y) << 1)))
#define BK_REG_H( x, y )            (((x) | (((y) << 1))) + 1)

// Demura info
#define L_BK_DEMURA(x)               BK_REG_L(REG_DEMURA_BASE, x)
#define H_BK_DEMURA(x)               BK_REG_H(REG_DEMURA_BASE, x)
#define L_BK_DEMURA_DMA(x)           BK_REG_L(REG_DEMURA_DMA_BASE, x)
#define H_BK_DEMURA_DMA(x)           BK_REG_H(REG_DEMURA_DMA_BASE, x)


// RBGW Panel Register info
#define REG_CHPI_CLKEN           (0x003200 + (0x2F<<1))  // --> 0x1032_0x2f[6] = 1 (16 bit mode)
#define REG_CHPI_BYPASS          (0x232600 + (0x01<<1))  // --> 0x3326_0x01[0] = 0 (16 bit mode)

#define REG_CEDS_CLKEN           (0x003200 + (0x2F<<1))  // --> 0x1032_0x2f[7] = 1 (16 bit mode)
#define REG_CEDS_BYPASS          (0x232600 + (0x01<<1))  // --> 0x3326_0x01[0] = 0 (16 bit mode)

#define REG_EPI_CLKEN            (0x003200 + (0x2F<<1))  // --> 0x1032_0x2f[7] = 1 (16 bit mode)
#define REG_EPI_BYPASS           (0x230300 + (0x01<<1))  // --> 0x3303_0x01[5] = 0 (16 bit mode)


MS_U16 HAL_DEMURA_Get_PanelWidth(DeMuraBinHeader *pHeader);
MS_U16 HAL_DEMURA_Get_PanelHeight(DeMuraBinHeader *pHeader);

void HAL_DEMURA_SetDL_BaseAddr(MS_U32 u32Addr);
void HAL_DEMURA_SetDL_SramIni_Addr(MS_U16 offset);
void HAL_DEMURA_SetDL_Depth(MS_U16 HNodeNum);
void HAL_DEMURA_SetDL_ReqLenth(MS_U16 ReqLenth);
void HAL_DEMURA_SetDL_TriggerMode(AUTODOWNLOAD_TRIG_e eTrigMode);
void HAL_DEMURA_Enable_DL(MS_BOOL bEnable);
void HAL_DEMURA_EnableDemura(MS_BOOL bEnable, DEMURA_PANEL_TYPE ptype);
void HAL_DEMURA_Dump_Layerlevel(void);

MS_U32 HAL_DEMURA_Cal_LutSize(MS_U32 levelCnt, MS_U32 hNode, MS_U32 vNode, MS_U32 sepType);
MS_U32 HAL_DEMURA_Max_RegSize(void);
MS_U32 HAL_DEMURA_Max_RegCount(void);
MS_U32 HAL_DEMURA_Get_PacketType(void);
DEMURA_PANEL_TYPE HAL_DEMURA_Get_PanelType(void);


#endif