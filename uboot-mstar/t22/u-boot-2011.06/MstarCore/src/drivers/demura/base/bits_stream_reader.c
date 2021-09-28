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
#include <MsTypes.h>
#include <stdio.h>
#include <linux/string.h>
#include <asm/byteorder.h>
#include "bits_stream_reader.h"


int Init_BSReader(BitsStreamReader * pBSR, MS_U32 *pdata, MS_U32 size_in_bytes)
{
    memset(pBSR, 0, sizeof(BitsStreamReader));
    if (size_in_bytes & 0xE0000000)
    {
        printf("%s Error : data_size (0x%x) is too large !", __FUNCTION__, size_in_bytes);
        return -1;
    }
    if ( (((unsigned int)pdata) & 3) != 0 )
    {
        // For efficiency
        printf("pdata(%p) should align 4 Byte", pdata);
        return -1;
    }

    pBSR->pdata = pdata;
    pBSR->total_bytes = size_in_bytes;
    pBSR->total_bits  = (size_in_bytes << 3);
    pBSR->cur_bit_pos = 0;
    return 0;
}


MS_U32 BSR_ReadData (BitsStreamReader * pBSR, MS_U8 bit_num)
{
    MS_U32 result    = 0;
    MS_U32 *pbuf     = pBSR->pdata;
    MS_U32 cur_bpos  = pBSR->cur_bit_pos;

    if ((bit_num > 32) || (bit_num <= 0))
    {
        printf("BSR_ReadData : bit_num(%d) is too large/small !\n", bit_num);
        return 0;
    }
    if ((cur_bpos + bit_num) > pBSR->total_bits)
    {
        printf("BSR_ReadData : Not enough data (0x%x < 0x%x)\n", (cur_bpos + bit_num), pBSR->total_bits);
        return 0;
    }

    MS_U32 left_data_pos = (cur_bpos >> 5);
    MS_U32 right_data_pos = ((cur_bpos + bit_num) >> 5);
    
    if (left_data_pos != right_data_pos)
    {
        MS_U64 cal_val = 0;
        MS_U32 left_data  = __le32_to_cpu( pbuf[left_data_pos] );
        MS_U64 right_data = __le32_to_cpu( pbuf[right_data_pos] );
        cal_val = left_data | (right_data << 32);
        cal_val = cal_val >> (cur_bpos & 31);
        cal_val = cal_val & ((MS_U64)((1 << bit_num) - 1));
        result  = (MS_U32)cal_val;
    }
    else
    {
        if (((cur_bpos & 31) == 0) && (bit_num == 32))
        {
            // Unlikely
            result = __le32_to_cpu( pbuf[(cur_bpos >> 5)] );
        }
        else
        {
            MS_U32 cal_val = 0;
            cal_val = __le32_to_cpu( pbuf[(cur_bpos >> 5)] );
            cal_val = cal_val >> (cur_bpos & 31);
            cal_val = cal_val & ((1 << bit_num) - 1);
            result  = cal_val;
        }
    }
    pBSR->cur_bit_pos += bit_num;
    return result;
}


void BSR_ShiftBits(BitsStreamReader * pBSR, MS_U32 bit_num)
{
    if ((pBSR->cur_bit_pos + bit_num) > pBSR->total_bits)
    {
        printf("Warning : BSR_ShiftBits bit_num(0x%x + 0x%x) exceed total_bits(0x%x), Trunk !\n", \
                pBSR->cur_bit_pos, bit_num, pBSR->total_bits);
        pBSR->cur_bit_pos = pBSR->total_bits;
    }
    else
    {
        pBSR->cur_bit_pos += bit_num;
    }
}

