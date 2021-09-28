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
#include <MsCommon.h>
#include <command.h>
#include <MsDebug.h>
#include <apiPNL.h>
#include <linux/string.h>
#include <halRegOp.h>
#include <MDrvDemura.h>
#include "halDemura.h"


MS_U16 HAL_DEMURA_Get_PanelWidth(DeMuraBinHeader *pHeader)
{
    MS_U16 panel_width;
    MS_U16 packet_nodes;

    if (pHeader->nPacketSize > 1)
    {
        DEBUG_DEMURA("Do not support nPacketSize = %d\n", pHeader->nPacketSize);
        return 0;
    }

    if (pHeader->u4EndLayer > 4)
    {
        packet_nodes = 1;
        if (pHeader->nPacketSize == 0)
        {
            DEBUG_DEMURA("This PacketSize(%d) can not contain Layers(%d)\n",
                         DEMURA_PACKET_SIZE(pHeader->nPacketSize), pHeader->u4EndLayer - pHeader->u4StartLayer);
            return 0;
        }
    }
    else if (pHeader->u4EndLayer > 2)
    {
        packet_nodes = (2 >> (1 - pHeader->nPacketSize));
    }
    else
    {
        packet_nodes = (4 >> (1 - pHeader->nPacketSize));
    }

    panel_width = packet_nodes * (1 << (pHeader->nHBlockSize)) * (pHeader->u16HNodeCount - 1);
    if (pHeader->bSeperateRGB == 0)
    {
        // Mono mode: R/G/B channel used the same table
        panel_width = panel_width * 4;
    }

    DEBUG_DEMURA("The panel_width in LUT is %d\n", panel_width);
    return panel_width;
}


MS_U16 HAL_DEMURA_Get_PanelHeight(DeMuraBinHeader *pHeader)
{
    MS_U16 panel_height;
    panel_height = (1 << (pHeader->nVBlockSize)) * (pHeader->u16VNodeCount - 1);
    DEBUG_DEMURA("The panel_height in LUT is %d\n", panel_height);
    return panel_height;
}


void HAL_DEMURA_SetDL_BaseAddr(MS_U32 u32Addr)
{
    MS_U32 base_addr = u32Addr / DEMURA_DMA_ADDR_UNIT;

    HAL_DEMURA_Write2Byte(L_BK_DEMURA_DMA(0x06), (MS_U16)(base_addr & 0xFFFF));
    HAL_DEMURA_Write2ByteMask(L_BK_DEMURA_DMA(0x07), (base_addr >> 16), 0x07FF);
}


void HAL_DEMURA_SetDL_SramIni_Addr(MS_U16 offset)
{
    HAL_DEMURA_Write2Byte(L_BK_DEMURA_DMA(0x10), offset);
}


void HAL_DEMURA_SetDL_Depth(MS_U16 HNodeNum)
{
    HAL_DEMURA_Write2Byte(L_BK_DEMURA_DMA(0x0A), HNodeNum);
}


void HAL_DEMURA_SetDL_ReqLenth(MS_U16 ReqLenth)
{
    HAL_DEMURA_Write2Byte(L_BK_DEMURA_DMA(0x0D), ReqLenth);
}

void HAL_DEMURA_SetDL_TriggerMode(AUTODOWNLOAD_TRIG_e eTrigMode)
{
    HAL_DEMURA_Write2ByteMask((L_BK_DEMURA_DMA(0x01)), (eTrigMode << 5), BIT5);
}

void HAL_DEMURA_Enable_DL(MS_BOOL bEnable)
{
    HAL_DEMURA_Write2ByteMask((L_BK_DEMURA_DMA(0x01)), (bEnable << 2), BIT2);
}

void HAL_DEMURA_EnableDemura(MS_BOOL bEnable, DEMURA_PANEL_TYPE ptype)
{
    MS_U16 reg_val  = 0;
    MS_U16 reg_mask = 0;

    reg_mask = (REG_DMC_DGA_EN_BIT | REG_DMC_W_EN_BIT | BIT2|BIT1|BIT0);

    switch (ptype)
    {
        case E_DEMURA_RGB_PANEL:
            reg_val  = BIT2|BIT1|BIT0;
            break;
        case E_DEMURA_RGBW_PANEL:
            reg_val = reg_mask;
            break;
        default:
            break;
    }

    if (bEnable == TRUE)
    {
        HAL_DEMURA_Write2ByteMask((L_BK_DEMURA(0x2F)),  reg_val, reg_mask);
    }
    else
    {
        HAL_DEMURA_Write2ByteMask((L_BK_DEMURA(0x2F)),  0x00, reg_mask);
    }
}


void HAL_DEMURA_Dump_Layerlevel(void)
{
    MS_U32 i = 0;
    MS_U32 layer_num = 8;
    MS_U32 layer_reg_start = 0x11;

    printf("Layer level(8 bit): ");
    for (i = 0; i < layer_num; i++)
    {
        MS_U32 addr = layer_reg_start + i;
        MS_U32 val  = HAL_DEMURA_Read2ByteMask(L_BK_DEMURA(addr), 0x0FFF);
        printf("%d ", (val >> 4));  // 12bit --> 8bit
    }
    printf("\n");
}


/******************** Convert Part ********************/

MS_U32 HAL_DEMURA_Cal_LutSize(MS_U32 levelCnt, MS_U32 hNode, MS_U32 vNode, MS_U32 sepType)
{
    int  LevelCount;
    int  cal_packet_size;    // unit : bit
    int  packet_size;        // unit : bit
    MS_U32 layer_dat_size;   // unit : byte

    switch (levelCnt)
    {
        case 8:
        case 7:
        case 6:
        case 5:
            LevelCount = 8;
            break;
        case 4:
        case 3:
            LevelCount = 4;
            break;
        case 2:
        case 1:
            LevelCount = 2;
            break;
        default:
            LevelCount = 8;
    }

    if (sepType == 1)
    {
        cal_packet_size = LevelCount * 3 * 8;
    }
    else
    {
        cal_packet_size = LevelCount * 8;
    }

    if (cal_packet_size > 128)
    {
        packet_size = 256;
    }
    else
    {
        packet_size = 128;
    }

    layer_dat_size = hNode * vNode * (packet_size / 8);
    return layer_dat_size;
}


MS_U32 HAL_DEMURA_Max_RegSize(void)
{
   return (256 * 5);   // unit : byte
}


MS_U32 HAL_DEMURA_Max_RegCount(void)
{
   return   256;   // unit : byte
}

MS_U32 HAL_DEMURA_Get_PacketType(void)
{
    // 1 : 256bit
    return 1;
}


DEMURA_PANEL_TYPE HAL_DEMURA_Get_PanelType(void)
{
    DEMURA_PANEL_TYPE pret;

    if ((HAL_DEMURA_Read2ByteMask(REG_CHPI_CLKEN, BIT6) == BIT6) \
         && (HAL_DEMURA_Read2ByteMask(REG_CHPI_BYPASS, BIT0) == 0))
    {
        // Interface CHPI
        pret = E_DEMURA_RGBW_PANEL;
    }
    else if ((HAL_DEMURA_Read2ByteMask(REG_CEDS_CLKEN, BIT7) == BIT7) \
         && (HAL_DEMURA_Read2ByteMask(REG_CEDS_BYPASS, BIT0) == 0))
    {
        // Interface CEDS
        pret = E_DEMURA_RGBW_PANEL;
    }
    else if ((HAL_DEMURA_Read2ByteMask(REG_EPI_CLKEN, BIT7) == BIT7) \
         && (HAL_DEMURA_Read2ByteMask(REG_EPI_BYPASS, BIT5) == 0))
    {
        // Interface EPI
        pret = E_DEMURA_RGBW_PANEL;
    }
    else
    {
        pret = E_DEMURA_RGB_PANEL;
    }

    return pret;
}

