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
#include "CommonDataType.h"
#include <dmalloc.h>
#include <ms_vector.h>
#include <demura.h>
#include <command.h>
#include <MsDebug.h>
#include "ms_array.h"

#ifdef CONFIG_DEMURA_VENDOR_LGD
MS_BOOL bit_mode_LPF(int plane_idx, int block_width, int block_height, strgb_structInfo *layer_blk_img_out);
#endif


int register_shift(int mask, int value)
{
    int shift_bit = 0;
    int shift_bit_tmp = 0;
    int mask_tmp = mask;
    int count_i;
    for (count_i = 1; count_i < 8; count_i++)
    {
        shift_bit_tmp = (mask_tmp&0x1);
        mask_tmp = (mask_tmp>>1);

        if(shift_bit_tmp==0)
            shift_bit = count_i;
        else
            break;

    }

    int adjust_value = ((value<<shift_bit)&mask);

    return adjust_value;
}

void part_bit15_00(int input_value, int total_bits, int *Bit15_08, int *Bit07_00)
{
    int msb_mask = 0;
    int msb_bit = total_bits - 8;

    int mask_count;
    for (mask_count = 0; mask_count < msb_bit; mask_count++)
    {
        msb_mask = (msb_mask<<1) + 1;
    }

    *Bit07_00 = (input_value&0xFF);
    *Bit15_08 = ((input_value>>8)&msb_mask);
}

void part_bit23_00(int input_value, int total_bits, int *Bit23_16, int *Bit15_08, int *Bit07_00)
{
    int msb_mask = 0;
    int msb_bit = total_bits - 16;

    int mask_count;
    for (mask_count = 0; mask_count < msb_bit; mask_count++)
    {
        msb_mask = (msb_mask<<1) + 1;
    }

    *Bit07_00 = (input_value&0xFF);
    *Bit15_08 = ((input_value>>8)&0xFF);
    *Bit23_16 = ((input_value>>16)&msb_mask);
}


void coeff_and_ks(int diff, int *coeff, int *ks)
{
    double target = (double)(1.0/(double)diff);

    int inv_20 = (int)((double)(1<<20)/(double)diff + 0.5);
    int inv_22 = (int)((double)(1<<22)/(double)diff + 0.5);

    inv_20 = minmax_( inv_20, 0x0, 0x3FFF);
    inv_22 = minmax_( inv_22, 0x0, 0x3FFF);

    double inv_20_value = (double)((double)inv_20/(double)(1<<20));
    double inv_22_value = (double)((double)inv_22/(double)(1<<22));

    double target1 = (target > inv_20_value) ? (target - inv_20_value) : (inv_20_value - target);
    double target2 = (target > inv_22_value) ? (target - inv_22_value) : (inv_22_value - target);

    if(target1<target2)
    {
        *ks = 0;
        *coeff = inv_20;
    }
    else
    {
        *ks = 1;
        *coeff = inv_22;
    }

}


void FIND_MIN_AND_MAX(int RGB_channel_idx, int block_width, int block_height, strgb_structInfo *layer_blk_img, double *rgb_max, double *rgb_min)
{

    // input lut value is base on 10bit data path 
    double rgb_max_tmp = 0.0;
    double rgb_min_tmp = 1023.0;

    int CurV, CurH;
    for (CurV = 0; CurV < block_height; CurV++)
    {
        for (CurH = 0; CurH < block_width; CurH++)
        {

            double val = 0;
            switch (RGB_channel_idx)
            {
            case 0:  // R channel
                val = layer_blk_img[posi_blk(CurV,CurH)].dbr;
                break;
            case 1:  // G channel
                val = layer_blk_img[posi_blk(CurV,CurH)].dbg;
                break;
            case 2:  // B channel
                val = layer_blk_img[posi_blk(CurV,CurH)].dbb;
                break;
            }

            rgb_max_tmp = max_(rgb_max_tmp, val);
            rgb_min_tmp = min_(rgb_min_tmp, val);

        }
    }

    *rgb_max = rgb_max_tmp;
    *rgb_min = rgb_min_tmp;
}


void LUT_SETTING_SUBFUNCTION(int RGB_idx, int plane_idx, int block_width, int block_height, int reg_dmc_plane_level, registers *preg, strgb_structInfo *layer_blk_img, strgb_structInfo *layer_blk_img_out)
{
    // input lut value is base on 10bit data path
    double rgb_max = 0.0;
    double rgb_min = 1023.0;
    FIND_MIN_AND_MAX( RGB_idx, block_width, block_height, layer_blk_img, &rgb_max, &rgb_min);

    double rgb_max_diff_min_half = (double)(rgb_max - rgb_min)/2.0;

    double rgb_max_min_avg = (double)(rgb_max + rgb_min)/2.0;

    int rgb_gain = 0; // x1
    int rgb_dmc_data_mag = 6;
    if(rgb_max_diff_min_half<=31.75/4.0)      // base on 10bit data path , lut value format T3.4 bit
    {
        rgb_gain = 0;
        rgb_dmc_data_mag = 6;
    }
    else if(rgb_max_diff_min_half<=63.5/4.0)  // base on 10bit data path , lut value format T4.3 bit
    {
        rgb_gain = 1;
        rgb_dmc_data_mag = 7;
    }
    else if(rgb_max_diff_min_half<=128/4.0)   // base on 10bit data path , lut value format T5.2 bit
    {
        rgb_gain = 2;
        rgb_dmc_data_mag = 0;
    }
    else if(rgb_max_diff_min_half<=256/4.0)   // base on 10bit data path , lut value format T6.1 bit
    {
        rgb_gain = 3;
        rgb_dmc_data_mag = 1;
    }
    else if(rgb_max_diff_min_half<=512/4.0)   // base on 10bit data path , lut value format T7.0 bit
    {
        rgb_gain = 4;
        rgb_dmc_data_mag = 2;
    }
    else if(rgb_max_diff_min_half<=1024/4.0)  // base on 10bit data path , lut value format T8.0 bit
    {
        rgb_gain = 5;
        rgb_dmc_data_mag = 3;
    }
    else //if(rgb_max_diff_min_half<=1016)    // base on 10bit data path , lut value format T9.0 bit
    {
        rgb_gain = 6;
        rgb_dmc_data_mag = 4;
    }

    // input data is 10 bit , but reg_dmc_plane_level is 12 bit , so shift 2 bit to 10 bit
    double reg_dmc_data_rgb_offset = rgb_max_min_avg - (double)((double)reg_dmc_plane_level/4.0);

    int reg_dmc_data_rgb_offset_int = (int)((reg_dmc_data_rgb_offset*4)*4); // 10 bit -> 12 bit -> 12.2bit

    reg_dmc_data_rgb_offset_int = minmax_(reg_dmc_data_rgb_offset_int, -8192, 8191);

    int reg_dmc_data_rgb_offset_int_2s = (reg_dmc_data_rgb_offset_int < 0) ? (16384 + reg_dmc_data_rgb_offset_int) : reg_dmc_data_rgb_offset_int; //T11.2


    //------------------------ mag and offset register start ------------------------//
    int Bit15_08, Bit07_00;


    switch(plane_idx)
    {
    case 0:
        switch (RGB_idx)
        {
        case 0: // plane 0 , R channel
            (*preg).reg_dmc_data_r_mag1.Bit07_00.value               = register_shift((*preg).reg_dmc_data_r_mag1.Bit07_00.mask, rgb_dmc_data_mag);

            part_bit15_00( reg_dmc_data_rgb_offset_int_2s, 14, &Bit15_08, &Bit07_00);
            (*preg).reg_dmc_data_r_offset1.Bit07_00.value             = register_shift((*preg).reg_dmc_data_r_offset1.Bit07_00.mask, Bit07_00);
            (*preg).reg_dmc_data_r_offset1.Bit15_08.value             = register_shift((*preg).reg_dmc_data_r_offset1.Bit15_08.mask, Bit15_08);
            break;
        case 1: // plane 0 , G channel
            (*preg).reg_dmc_data_g_mag1.Bit07_00.value               = register_shift((*preg).reg_dmc_data_g_mag1.Bit07_00.mask, rgb_dmc_data_mag);

            part_bit15_00( reg_dmc_data_rgb_offset_int_2s, 14, &Bit15_08, &Bit07_00);
            (*preg).reg_dmc_data_g_offset1.Bit07_00.value             = register_shift((*preg).reg_dmc_data_g_offset1.Bit07_00.mask, Bit07_00);
            (*preg).reg_dmc_data_g_offset1.Bit15_08.value             = register_shift((*preg).reg_dmc_data_g_offset1.Bit15_08.mask, Bit15_08);
            break;
        case 2: // plane 0 , B channel
            (*preg).reg_dmc_data_b_mag1.Bit07_00.value               = register_shift((*preg).reg_dmc_data_b_mag1.Bit07_00.mask, rgb_dmc_data_mag);

            part_bit15_00( reg_dmc_data_rgb_offset_int_2s, 14, &Bit15_08, &Bit07_00);
            (*preg).reg_dmc_data_b_offset1.Bit07_00.value             = register_shift((*preg).reg_dmc_data_b_offset1.Bit07_00.mask, Bit07_00);
            (*preg).reg_dmc_data_b_offset1.Bit15_08.value             = register_shift((*preg).reg_dmc_data_b_offset1.Bit15_08.mask, Bit15_08);
            break;
        }    
        break;
    case 1:
        switch (RGB_idx)
        {
        case 0: // plane 1 , R channel
            (*preg).reg_dmc_data_r_mag2.Bit07_00.value               = register_shift((*preg).reg_dmc_data_r_mag2.Bit07_00.mask, rgb_dmc_data_mag);

            part_bit15_00( reg_dmc_data_rgb_offset_int_2s, 14, &Bit15_08, &Bit07_00);
            (*preg).reg_dmc_data_r_offset2.Bit07_00.value             = register_shift((*preg).reg_dmc_data_r_offset2.Bit07_00.mask, Bit07_00);
            (*preg).reg_dmc_data_r_offset2.Bit15_08.value             = register_shift((*preg).reg_dmc_data_r_offset2.Bit15_08.mask, Bit15_08);
            break;
        case 1: // plane 1 , G channel
            (*preg).reg_dmc_data_g_mag2.Bit07_00.value               = register_shift((*preg).reg_dmc_data_g_mag2.Bit07_00.mask, rgb_dmc_data_mag);

            part_bit15_00( reg_dmc_data_rgb_offset_int_2s, 14, &Bit15_08, &Bit07_00);
            (*preg).reg_dmc_data_g_offset2.Bit07_00.value             = register_shift((*preg).reg_dmc_data_g_offset2.Bit07_00.mask, Bit07_00);
            (*preg).reg_dmc_data_g_offset2.Bit15_08.value             = register_shift((*preg).reg_dmc_data_g_offset2.Bit15_08.mask, Bit15_08);
            break;
        case 2: // plane 1 , B channel
            (*preg).reg_dmc_data_b_mag2.Bit07_00.value               = register_shift((*preg).reg_dmc_data_b_mag2.Bit07_00.mask, rgb_dmc_data_mag);

            part_bit15_00( reg_dmc_data_rgb_offset_int_2s, 14, &Bit15_08, &Bit07_00);
            (*preg).reg_dmc_data_b_offset2.Bit07_00.value             = register_shift((*preg).reg_dmc_data_b_offset2.Bit07_00.mask, Bit07_00);
            (*preg).reg_dmc_data_b_offset2.Bit15_08.value             = register_shift((*preg).reg_dmc_data_b_offset2.Bit15_08.mask, Bit15_08);
            break;
        }        
        break;
    case 2:
        switch (RGB_idx)
        {
        case 0: // plane 2 , R channel
            (*preg).reg_dmc_data_r_mag3.Bit07_00.value               = register_shift((*preg).reg_dmc_data_r_mag3.Bit07_00.mask, rgb_dmc_data_mag);

            part_bit15_00( reg_dmc_data_rgb_offset_int_2s, 14, &Bit15_08, &Bit07_00);
            (*preg).reg_dmc_data_r_offset3.Bit07_00.value             = register_shift((*preg).reg_dmc_data_r_offset3.Bit07_00.mask, Bit07_00);
            (*preg).reg_dmc_data_r_offset3.Bit15_08.value             = register_shift((*preg).reg_dmc_data_r_offset3.Bit15_08.mask, Bit15_08);
            break;
        case 1: // plane 2 , G channel
            (*preg).reg_dmc_data_g_mag3.Bit07_00.value               = register_shift((*preg).reg_dmc_data_g_mag3.Bit07_00.mask, rgb_dmc_data_mag);

            part_bit15_00( reg_dmc_data_rgb_offset_int_2s, 14, &Bit15_08, &Bit07_00);
            (*preg).reg_dmc_data_g_offset3.Bit07_00.value             = register_shift((*preg).reg_dmc_data_g_offset3.Bit07_00.mask, Bit07_00);
            (*preg).reg_dmc_data_g_offset3.Bit15_08.value             = register_shift((*preg).reg_dmc_data_g_offset3.Bit15_08.mask, Bit15_08);
            break;
        case 2: // plane 2 , B channel
            (*preg).reg_dmc_data_b_mag3.Bit07_00.value               = register_shift((*preg).reg_dmc_data_b_mag3.Bit07_00.mask, rgb_dmc_data_mag);

            part_bit15_00( reg_dmc_data_rgb_offset_int_2s, 14, &Bit15_08, &Bit07_00);
            (*preg).reg_dmc_data_b_offset3.Bit07_00.value             = register_shift((*preg).reg_dmc_data_b_offset3.Bit07_00.mask, Bit07_00);
            (*preg).reg_dmc_data_b_offset3.Bit15_08.value             = register_shift((*preg).reg_dmc_data_b_offset3.Bit15_08.mask, Bit15_08);
            break;
        }        
        break;
    case 3:
        switch (RGB_idx)
        {
        case 0: // plane 3 , R channel
            (*preg).reg_dmc_data_r_mag4.Bit07_00.value               = register_shift((*preg).reg_dmc_data_r_mag4.Bit07_00.mask, rgb_dmc_data_mag);

            part_bit15_00( reg_dmc_data_rgb_offset_int_2s, 14, &Bit15_08, &Bit07_00);
            (*preg).reg_dmc_data_r_offset4.Bit07_00.value             = register_shift((*preg).reg_dmc_data_r_offset4.Bit07_00.mask, Bit07_00);
            (*preg).reg_dmc_data_r_offset4.Bit15_08.value             = register_shift((*preg).reg_dmc_data_r_offset4.Bit15_08.mask, Bit15_08);
            break;
        case 1: // plane 3 , G channel
            (*preg).reg_dmc_data_g_mag4.Bit07_00.value               = register_shift((*preg).reg_dmc_data_g_mag4.Bit07_00.mask, rgb_dmc_data_mag);

            part_bit15_00( reg_dmc_data_rgb_offset_int_2s, 14, &Bit15_08, &Bit07_00);
            (*preg).reg_dmc_data_g_offset4.Bit07_00.value             = register_shift((*preg).reg_dmc_data_g_offset4.Bit07_00.mask, Bit07_00);
            (*preg).reg_dmc_data_g_offset4.Bit15_08.value             = register_shift((*preg).reg_dmc_data_g_offset4.Bit15_08.mask, Bit15_08);
            break;
        case 2: // plane 3 , B channel
            (*preg).reg_dmc_data_b_mag4.Bit07_00.value               = register_shift((*preg).reg_dmc_data_b_mag4.Bit07_00.mask, rgb_dmc_data_mag);

            part_bit15_00( reg_dmc_data_rgb_offset_int_2s, 14, &Bit15_08, &Bit07_00);
            (*preg).reg_dmc_data_b_offset4.Bit07_00.value             = register_shift((*preg).reg_dmc_data_b_offset4.Bit07_00.mask, Bit07_00);
            (*preg).reg_dmc_data_b_offset4.Bit15_08.value             = register_shift((*preg).reg_dmc_data_b_offset4.Bit15_08.mask, Bit15_08);
            break;
        }
        break;
    case 4:
        switch (RGB_idx)
        {
        case 0: // plane 4 , R channel
            (*preg).reg_dmc_data_r_mag5.Bit07_00.value               = register_shift((*preg).reg_dmc_data_r_mag5.Bit07_00.mask, rgb_dmc_data_mag);

            part_bit15_00( reg_dmc_data_rgb_offset_int_2s, 14, &Bit15_08, &Bit07_00);
            (*preg).reg_dmc_data_r_offset5.Bit07_00.value             = register_shift((*preg).reg_dmc_data_r_offset5.Bit07_00.mask, Bit07_00);
            (*preg).reg_dmc_data_r_offset5.Bit15_08.value             = register_shift((*preg).reg_dmc_data_r_offset5.Bit15_08.mask, Bit15_08);
            break;
        case 1: // plane 4 , G channel
            (*preg).reg_dmc_data_g_mag5.Bit07_00.value               = register_shift((*preg).reg_dmc_data_g_mag5.Bit07_00.mask, rgb_dmc_data_mag);

            part_bit15_00( reg_dmc_data_rgb_offset_int_2s, 14, &Bit15_08, &Bit07_00);
            (*preg).reg_dmc_data_g_offset5.Bit07_00.value             = register_shift((*preg).reg_dmc_data_g_offset5.Bit07_00.mask, Bit07_00);
            (*preg).reg_dmc_data_g_offset5.Bit15_08.value             = register_shift((*preg).reg_dmc_data_g_offset5.Bit15_08.mask, Bit15_08);
            break;
        case 2: // plane 4 , B channel
            (*preg).reg_dmc_data_b_mag5.Bit07_00.value               = register_shift((*preg).reg_dmc_data_b_mag5.Bit07_00.mask, rgb_dmc_data_mag);

            part_bit15_00( reg_dmc_data_rgb_offset_int_2s, 14, &Bit15_08, &Bit07_00);
            (*preg).reg_dmc_data_b_offset5.Bit07_00.value             = register_shift((*preg).reg_dmc_data_b_offset5.Bit07_00.mask, Bit07_00);
            (*preg).reg_dmc_data_b_offset5.Bit15_08.value             = register_shift((*preg).reg_dmc_data_b_offset5.Bit15_08.mask, Bit15_08);
            break;
        }    
        break;
    case 5:
        switch (RGB_idx)
        {
        case 0: // plane 5 , R channel
            (*preg).reg_dmc_data_r_mag6.Bit07_00.value               = register_shift((*preg).reg_dmc_data_r_mag6.Bit07_00.mask, rgb_dmc_data_mag);

            part_bit15_00( reg_dmc_data_rgb_offset_int_2s, 14, &Bit15_08, &Bit07_00);
            (*preg).reg_dmc_data_r_offset6.Bit07_00.value             = register_shift((*preg).reg_dmc_data_r_offset6.Bit07_00.mask, Bit07_00);
            (*preg).reg_dmc_data_r_offset6.Bit15_08.value             = register_shift((*preg).reg_dmc_data_r_offset6.Bit15_08.mask, Bit15_08);
            break;
        case 1: // plane 5 , G channel
            (*preg).reg_dmc_data_g_mag6.Bit07_00.value               = register_shift((*preg).reg_dmc_data_g_mag6.Bit07_00.mask, rgb_dmc_data_mag);

            part_bit15_00( reg_dmc_data_rgb_offset_int_2s, 14, &Bit15_08, &Bit07_00);
            (*preg).reg_dmc_data_g_offset6.Bit07_00.value             = register_shift((*preg).reg_dmc_data_g_offset6.Bit07_00.mask, Bit07_00);
            (*preg).reg_dmc_data_g_offset6.Bit15_08.value             = register_shift((*preg).reg_dmc_data_g_offset6.Bit15_08.mask, Bit15_08);
            break;
        case 2: // plane 5 , B channel
            (*preg).reg_dmc_data_b_mag6.Bit07_00.value               = register_shift((*preg).reg_dmc_data_b_mag6.Bit07_00.mask, rgb_dmc_data_mag);

            part_bit15_00( reg_dmc_data_rgb_offset_int_2s, 14, &Bit15_08, &Bit07_00);
            (*preg).reg_dmc_data_b_offset6.Bit07_00.value             = register_shift((*preg).reg_dmc_data_b_offset6.Bit07_00.mask, Bit07_00);
            (*preg).reg_dmc_data_b_offset6.Bit15_08.value             = register_shift((*preg).reg_dmc_data_b_offset6.Bit15_08.mask, Bit15_08);
            break;
        }    
        break;
    case 6:
        switch (RGB_idx)
        {
        case 0: // plane  , R channel
            (*preg).reg_dmc_data_r_mag7.Bit07_00.value               = register_shift((*preg).reg_dmc_data_r_mag7.Bit07_00.mask, rgb_dmc_data_mag);

            part_bit15_00( reg_dmc_data_rgb_offset_int_2s, 14, &Bit15_08, &Bit07_00);
            (*preg).reg_dmc_data_r_offset7.Bit07_00.value             = register_shift((*preg).reg_dmc_data_r_offset7.Bit07_00.mask, Bit07_00);
            (*preg).reg_dmc_data_r_offset7.Bit15_08.value             = register_shift((*preg).reg_dmc_data_r_offset7.Bit15_08.mask, Bit15_08);
            break;
        case 1: // plane  , G channel
            (*preg).reg_dmc_data_g_mag7.Bit07_00.value               = register_shift((*preg).reg_dmc_data_g_mag7.Bit07_00.mask, rgb_dmc_data_mag);

            part_bit15_00( reg_dmc_data_rgb_offset_int_2s, 14, &Bit15_08, &Bit07_00);
            (*preg).reg_dmc_data_g_offset7.Bit07_00.value             = register_shift((*preg).reg_dmc_data_g_offset7.Bit07_00.mask, Bit07_00);
            (*preg).reg_dmc_data_g_offset7.Bit15_08.value             = register_shift((*preg).reg_dmc_data_g_offset7.Bit15_08.mask, Bit15_08);
            break;
        case 2: // plane  , B channel
            (*preg).reg_dmc_data_b_mag7.Bit07_00.value               = register_shift((*preg).reg_dmc_data_b_mag7.Bit07_00.mask, rgb_dmc_data_mag);

            part_bit15_00( reg_dmc_data_rgb_offset_int_2s, 14, &Bit15_08, &Bit07_00);
            (*preg).reg_dmc_data_b_offset7.Bit07_00.value             = register_shift((*preg).reg_dmc_data_b_offset7.Bit07_00.mask, Bit07_00);
            (*preg).reg_dmc_data_b_offset7.Bit15_08.value             = register_shift((*preg).reg_dmc_data_b_offset7.Bit15_08.mask, Bit15_08);
            break;
        }    
        break;
    case 7:
        switch (RGB_idx)
        {
        case 0: // plane  , R channel
            (*preg).reg_dmc_data_r_mag8.Bit07_00.value               = register_shift((*preg).reg_dmc_data_r_mag8.Bit07_00.mask, rgb_dmc_data_mag);

            part_bit15_00( reg_dmc_data_rgb_offset_int_2s, 14, &Bit15_08, &Bit07_00);
            (*preg).reg_dmc_data_r_offset8.Bit07_00.value             = register_shift((*preg).reg_dmc_data_r_offset8.Bit07_00.mask, Bit07_00);
            (*preg).reg_dmc_data_r_offset8.Bit15_08.value             = register_shift((*preg).reg_dmc_data_r_offset8.Bit15_08.mask, Bit15_08);
            break;
        case 1: // plane  , G channel
            (*preg).reg_dmc_data_g_mag8.Bit07_00.value               = register_shift((*preg).reg_dmc_data_g_mag8.Bit07_00.mask, rgb_dmc_data_mag);

            part_bit15_00( reg_dmc_data_rgb_offset_int_2s, 14, &Bit15_08, &Bit07_00);
            (*preg).reg_dmc_data_g_offset8.Bit07_00.value             = register_shift((*preg).reg_dmc_data_g_offset8.Bit07_00.mask, Bit07_00);
            (*preg).reg_dmc_data_g_offset8.Bit15_08.value             = register_shift((*preg).reg_dmc_data_g_offset8.Bit15_08.mask, Bit15_08);
            break;
        case 2: // plane  , B channel
            (*preg).reg_dmc_data_b_mag8.Bit07_00.value               = register_shift((*preg).reg_dmc_data_b_mag8.Bit07_00.mask, rgb_dmc_data_mag);

            part_bit15_00( reg_dmc_data_rgb_offset_int_2s, 14, &Bit15_08, &Bit07_00);
            (*preg).reg_dmc_data_b_offset8.Bit07_00.value             = register_shift((*preg).reg_dmc_data_b_offset8.Bit07_00.mask, Bit07_00);
            (*preg).reg_dmc_data_b_offset8.Bit15_08.value             = register_shift((*preg).reg_dmc_data_b_offset8.Bit15_08.mask, Bit15_08);
            break;
        }    
        break;
    }
    //------------------------ mag and offset register end ------------------------//


    //----------------------- LUT start -----------------------//
    int CurV, CurH;
    for (CurV = 0; CurV < block_height; CurV++)
    {
        for (CurH = 0; CurH < block_width; CurH++)
        {        
            double rgb_tmp = 0;

            switch (RGB_idx)
            {
            case 0:  // R channel
                rgb_tmp = layer_blk_img[posi_blk(CurV,CurH)].dbr;
                break;
            case 1:  // G channel
                rgb_tmp = layer_blk_img[posi_blk(CurV,CurH)].dbg;
                break;
            case 2:  // B channel
                rgb_tmp = layer_blk_img[posi_blk(CurV,CurH)].dbb;
                break;
            }
            rgb_tmp = rgb_tmp - rgb_max_min_avg;  // double 10bit - double 10bit        

            switch(rgb_gain)
            {
            case 0: // format T3.4 (T00xxx.xxxx)
                rgb_tmp = ((double)rgb_tmp*(double)16);
                break;
            case 1: // format T4.3 (T0xxxx.xxx)
                rgb_tmp = ((double)rgb_tmp*(double)8);
                break;
            case 2: // format T5.2 (Txxxxx.xx)
                rgb_tmp = ((double)rgb_tmp*(double)4);
                break;
            case 3: // format T6.1 (Txxxxxx.x)
                rgb_tmp = ((double)rgb_tmp*(double)2);
                break;
            case 4: // format T7.0 (Txxxxxxx.0)
                rgb_tmp = rgb_tmp;
                break;
            case 5: // format T8.0 (Txxxxxxx0.0)
                rgb_tmp = ((double)rgb_tmp/(double)2);
            case 6: // format T9.0 (Txxxxxxx00.0)
                rgb_tmp = ((double)rgb_tmp/(double)4);
                break;
            }

            double rgb_db = (double)((double)((int)(rgb_tmp*4))/4.0);

            int rgb_int = (int)(rgb_tmp);

            rgb_int = minmax_(rgb_int,-128,127);

            switch (RGB_idx)
            {
            case 0:  // R channel
                layer_blk_img_out[posi_blk(CurV,CurH)].dbr = rgb_db;
                layer_blk_img_out[posi_blk(CurV,CurH)].r   = rgb_int;
                break;
            case 1:  // G channel
                layer_blk_img_out[posi_blk(CurV,CurH)].dbg = rgb_db;
                layer_blk_img_out[posi_blk(CurV,CurH)].g   = rgb_int;
                break;
            case 2:  // B channel
                layer_blk_img_out[posi_blk(CurV,CurH)].dbb = rgb_db;
                layer_blk_img_out[posi_blk(CurV,CurH)].b   = rgb_int;
                break;
            }            
        }
    }
    //----------------------- LUT end -----------------------//
}


void LUT_AND_SETTING_PROCESSOR( int plane_idx, int block_width, int block_height, int reg_dmc_plane_level, registers *preg , strgb_structInfo *layer_blk_img_in, strgb_structInfo *layer_blk_img_out)
{
    int i;
    for (i = 0; i < 3; i++)  // R/G/B channel
    {
        LUT_SETTING_SUBFUNCTION( i, plane_idx, block_width, block_height, reg_dmc_plane_level, preg, layer_blk_img_in, layer_blk_img_out);
    }
    
    #ifdef CONFIG_DEMURA_VENDOR_LGD
    UBOOT_DEBUG("LGD to Mstar, Call bit_mode_LPF\n");
    bit_mode_LPF(plane_idx, block_width, block_height, layer_blk_img_out);
    #endif
}


void write_demuraLUT_to_HW_DRAM_FORMAT(stlayer_info_structInfo *layer_info, strgb_structInfo *layer_blk_img[8], m_vector *pLut_out_vector)
{
    int out_idx = 0; // For Lut_out arrary
    char *Lut_out = (char *)(pLut_out_vector->pbuf) + pLut_out_vector->dat_num;
    
    if(layer_info->dmc_rgb_mode == 1)
    {
        int H_offset = 1;
        switch(layer_info->dmc_plane_num)
        {
        case 1:
        case 2:
            H_offset = 4;
            break;
        case 3:
        case 4:
            H_offset = 2;
            break;
        case 5:
        case 6:
        case 7:
        case 8:
            H_offset = 1;
            break;
        }  


        int lut_vidx, lut_hidx;
        for (lut_vidx = 0; lut_vidx < layer_info->real_dram_v_size; lut_vidx++)
        {
            for (lut_hidx = 0; lut_hidx < layer_info->real_dram_h_size; lut_hidx++)
            {
                strgb_structInfo L[9];
                strgb_structInfo null_val;
                null_val.r = 0;
                null_val.g = 0;
                null_val.b = 0;
                null_val.dbr = 0.0;
                null_val.dbg = 0.0;
                null_val.dbb = 0.0;

                int lut_idx0 = MIN(lut_hidx*H_offset + 0, layer_info->dram_h_size);
                int lut_idx1 = MIN(lut_hidx*H_offset + 1, layer_info->dram_h_size);
                int lut_idx2 = MIN(lut_hidx*H_offset + 2, layer_info->dram_h_size);
                int lut_idx3 = MIN(lut_hidx*H_offset + 3, layer_info->dram_h_size);

                lut_idx0 = (lut_idx0 == layer_info->dram_h_size) ? -1 : (lut_vidx * layer_info->dram_h_size + lut_idx0);
                lut_idx1 = (lut_idx1 == layer_info->dram_h_size) ? -1 : (lut_vidx * layer_info->dram_h_size + lut_idx1);
                lut_idx2 = (lut_idx2 == layer_info->dram_h_size) ? -1 : (lut_vidx * layer_info->dram_h_size + lut_idx2);
                lut_idx3 = (lut_idx3 == layer_info->dram_h_size) ? -1 : (lut_vidx * layer_info->dram_h_size + lut_idx3);


                switch(layer_info->dmc_plane_num)
                {
                case 1:
                    L[1] = (lut_idx0 == -1) ? null_val : layer_blk_img[0][lut_idx0];
                    L[2] = null_val;
                    L[3] = (lut_idx1 == -1) ? null_val : layer_blk_img[0][lut_idx1];
                    L[4] = null_val;
                    L[5] = (lut_idx2 == -1) ? null_val : layer_blk_img[0][lut_idx2];
                    L[6] = null_val;
                    L[7] = (lut_idx3 == -1) ? null_val : layer_blk_img[0][lut_idx3];
                    L[8] = null_val;
                    break;
                case 2:
                    L[1] = (lut_idx0 == -1) ? null_val : layer_blk_img[0][lut_idx0];
                    L[2] = (lut_idx0 == -1) ? null_val : layer_blk_img[1][lut_idx0];
                    L[3] = (lut_idx1 == -1) ? null_val : layer_blk_img[0][lut_idx1];
                    L[4] = (lut_idx1 == -1) ? null_val : layer_blk_img[1][lut_idx1];
                    L[5] = (lut_idx2 == -1) ? null_val : layer_blk_img[0][lut_idx2];
                    L[6] = (lut_idx2 == -1) ? null_val : layer_blk_img[1][lut_idx2];
                    L[7] = (lut_idx3 == -1) ? null_val : layer_blk_img[0][lut_idx3];
                    L[8] = (lut_idx3 == -1) ? null_val : layer_blk_img[1][lut_idx3];
                    break;
                case 3:
                    L[1] = (lut_idx0 == -1) ? null_val : layer_blk_img[0][lut_idx0];
                    L[2] = (lut_idx0 == -1) ? null_val : layer_blk_img[1][lut_idx0];
                    L[3] = (lut_idx0 == -1) ? null_val : layer_blk_img[2][lut_idx0];
                    L[4] = null_val;
                    L[5] = (lut_idx1 == -1) ? null_val : layer_blk_img[0][lut_idx1];
                    L[6] = (lut_idx1 == -1) ? null_val : layer_blk_img[1][lut_idx1];
                    L[7] = (lut_idx1 == -1) ? null_val : layer_blk_img[2][lut_idx1];
                    L[8] = null_val;
                    break;
                case 4:
                    L[1] = (lut_idx0 == -1) ? null_val : layer_blk_img[0][lut_idx0];
                    L[2] = (lut_idx0 == -1) ? null_val : layer_blk_img[1][lut_idx0];
                    L[3] = (lut_idx0 == -1) ? null_val : layer_blk_img[2][lut_idx0];
                    L[4] = (lut_idx0 == -1) ? null_val : layer_blk_img[3][lut_idx0];
                    L[5] = (lut_idx1 == -1) ? null_val : layer_blk_img[0][lut_idx1];
                    L[6] = (lut_idx1 == -1) ? null_val : layer_blk_img[1][lut_idx1];
                    L[7] = (lut_idx1 == -1) ? null_val : layer_blk_img[2][lut_idx1];
                    L[8] = (lut_idx1 == -1) ? null_val : layer_blk_img[3][lut_idx1];
                    break;
                case 5:
                    L[1] = (lut_idx0 == -1) ? null_val : layer_blk_img[0][lut_idx0];
                    L[2] = (lut_idx0 == -1) ? null_val : layer_blk_img[1][lut_idx0];
                    L[3] = (lut_idx0 == -1) ? null_val : layer_blk_img[2][lut_idx0];
                    L[4] = (lut_idx0 == -1) ? null_val : layer_blk_img[3][lut_idx0];
                    L[5] = (lut_idx0 == -1) ? null_val : layer_blk_img[4][lut_idx0];
                    L[6] = null_val;
                    L[7] = null_val;
                    L[8] = null_val;
                    break;
                case 6:
                    L[1] = (lut_idx0 == -1) ? null_val : layer_blk_img[0][lut_idx0];
                    L[2] = (lut_idx0 == -1) ? null_val : layer_blk_img[1][lut_idx0];
                    L[3] = (lut_idx0 == -1) ? null_val : layer_blk_img[2][lut_idx0];
                    L[4] = (lut_idx0 == -1) ? null_val : layer_blk_img[3][lut_idx0];
                    L[5] = (lut_idx0 == -1) ? null_val : layer_blk_img[4][lut_idx0];
                    L[6] = (lut_idx0 == -1) ? null_val : layer_blk_img[5][lut_idx0];
                    L[7] = null_val;
                    L[8] = null_val;
                    break;
                case 7:
                    L[1] = (lut_idx0 == -1) ? null_val : layer_blk_img[0][lut_idx0];
                    L[2] = (lut_idx0 == -1) ? null_val : layer_blk_img[1][lut_idx0];
                    L[3] = (lut_idx0 == -1) ? null_val : layer_blk_img[2][lut_idx0];
                    L[4] = (lut_idx0 == -1) ? null_val : layer_blk_img[3][lut_idx0];
                    L[5] = (lut_idx0 == -1) ? null_val : layer_blk_img[4][lut_idx0];
                    L[6] = (lut_idx0 == -1) ? null_val : layer_blk_img[5][lut_idx0];
                    L[7] = (lut_idx0 == -1) ? null_val : layer_blk_img[6][lut_idx0];
                    L[8] = null_val;
                    break;
                case 8:
                    L[1] = (lut_idx0 == -1) ? null_val : layer_blk_img[0][lut_idx0];
                    L[2] = (lut_idx0 == -1) ? null_val : layer_blk_img[1][lut_idx0];
                    L[3] = (lut_idx0 == -1) ? null_val : layer_blk_img[2][lut_idx0];
                    L[4] = (lut_idx0 == -1) ? null_val : layer_blk_img[3][lut_idx0];
                    L[5] = (lut_idx0 == -1) ? null_val : layer_blk_img[4][lut_idx0];
                    L[6] = (lut_idx0 == -1) ? null_val : layer_blk_img[5][lut_idx0];
                    L[7] = (lut_idx0 == -1) ? null_val : layer_blk_img[6][lut_idx0];
                    L[8] = (lut_idx0 == -1) ? null_val : layer_blk_img[7][lut_idx0];
                    break;
                }

                int idx;
                for (idx = 1; idx < 9; idx++)
                {
                    char rr = (char)((L[idx].r < 0.0) ? (256 + L[idx].r) : (L[idx].r));
                    char gg = (char)((L[idx].g < 0.0) ? (256 + L[idx].g) : (L[idx].g));
                    char bb = (char)((L[idx].b < 0.0) ? (256 + L[idx].b) : (L[idx].b));
                    char zero = 0;
                    
                    Lut_out[out_idx++] = bb;
                    Lut_out[out_idx++] = gg;
                    Lut_out[out_idx++] = rr;

                    if((idx-1)%4==3)
                    {
                        int zero_idx;
                        for (zero_idx = 0; zero_idx < 4; zero_idx++)
                            Lut_out[out_idx++] = zero;
                    }
                }

            }
        }

    }
    else
    {
        int H_offset = 4;
        switch(layer_info->dmc_plane_num)
        {
        case 1:
        case 2:
            H_offset = 16;
            break;
        case 3:
        case 4:
            H_offset = 8;
            break;
        case 5:
        case 6:
        case 7:
        case 8:
            H_offset = 4;
            break;
        }  

        int lut_vidx, lut_hidx;
        for (lut_vidx = 0; lut_vidx < layer_info->real_dram_v_size; lut_vidx++)
        {
            for (lut_hidx = 0; lut_hidx < layer_info->real_dram_h_size; lut_hidx++)
            {
                int L[32];
                int null_val = 0;

                int lut_idx00 = MIN(lut_hidx*H_offset +  0, layer_info->dram_h_size );
                int lut_idx01 = MIN(lut_hidx*H_offset +  1, layer_info->dram_h_size );
                int lut_idx02 = MIN(lut_hidx*H_offset +  2, layer_info->dram_h_size );
                int lut_idx03 = MIN(lut_hidx*H_offset +  3, layer_info->dram_h_size );
                int lut_idx04 = MIN(lut_hidx*H_offset +  4, layer_info->dram_h_size );
                int lut_idx05 = MIN(lut_hidx*H_offset +  5, layer_info->dram_h_size );
                int lut_idx06 = MIN(lut_hidx*H_offset +  6, layer_info->dram_h_size );
                int lut_idx07 = MIN(lut_hidx*H_offset +  7, layer_info->dram_h_size );
                int lut_idx08 = MIN(lut_hidx*H_offset +  8, layer_info->dram_h_size );
                int lut_idx09 = MIN(lut_hidx*H_offset +  9, layer_info->dram_h_size );
                int lut_idx10 = MIN(lut_hidx*H_offset + 10, layer_info->dram_h_size );
                int lut_idx11 = MIN(lut_hidx*H_offset + 11, layer_info->dram_h_size );
                int lut_idx12 = MIN(lut_hidx*H_offset + 12, layer_info->dram_h_size );
                int lut_idx13 = MIN(lut_hidx*H_offset + 13, layer_info->dram_h_size );
                int lut_idx14 = MIN(lut_hidx*H_offset + 14, layer_info->dram_h_size );
                int lut_idx15 = MIN(lut_hidx*H_offset + 15, layer_info->dram_h_size );

                lut_idx00 = (lut_idx00 == layer_info->dram_h_size) ? -1 : (lut_vidx * layer_info->dram_h_size + lut_idx00);
                lut_idx01 = (lut_idx01 == layer_info->dram_h_size) ? -1 : (lut_vidx * layer_info->dram_h_size + lut_idx01);
                lut_idx02 = (lut_idx02 == layer_info->dram_h_size) ? -1 : (lut_vidx * layer_info->dram_h_size + lut_idx02);
                lut_idx03 = (lut_idx03 == layer_info->dram_h_size) ? -1 : (lut_vidx * layer_info->dram_h_size + lut_idx03);
                lut_idx04 = (lut_idx04 == layer_info->dram_h_size) ? -1 : (lut_vidx * layer_info->dram_h_size + lut_idx04);
                lut_idx05 = (lut_idx05 == layer_info->dram_h_size) ? -1 : (lut_vidx * layer_info->dram_h_size + lut_idx05);
                lut_idx06 = (lut_idx06 == layer_info->dram_h_size) ? -1 : (lut_vidx * layer_info->dram_h_size + lut_idx06);
                lut_idx07 = (lut_idx07 == layer_info->dram_h_size) ? -1 : (lut_vidx * layer_info->dram_h_size + lut_idx07);
                lut_idx08 = (lut_idx08 == layer_info->dram_h_size) ? -1 : (lut_vidx * layer_info->dram_h_size + lut_idx08);
                lut_idx09 = (lut_idx09 == layer_info->dram_h_size) ? -1 : (lut_vidx * layer_info->dram_h_size + lut_idx09);
                lut_idx10 = (lut_idx10 == layer_info->dram_h_size) ? -1 : (lut_vidx * layer_info->dram_h_size + lut_idx10);
                lut_idx11 = (lut_idx11 == layer_info->dram_h_size) ? -1 : (lut_vidx * layer_info->dram_h_size + lut_idx11);
                lut_idx12 = (lut_idx12 == layer_info->dram_h_size) ? -1 : (lut_vidx * layer_info->dram_h_size + lut_idx12);
                lut_idx13 = (lut_idx13 == layer_info->dram_h_size) ? -1 : (lut_vidx * layer_info->dram_h_size + lut_idx13);
                lut_idx14 = (lut_idx14 == layer_info->dram_h_size) ? -1 : (lut_vidx * layer_info->dram_h_size + lut_idx14);
                lut_idx15 = (lut_idx15 == layer_info->dram_h_size) ? -1 : (lut_vidx * layer_info->dram_h_size + lut_idx15);


                switch(layer_info->dmc_plane_num)
                {
                case 1:
                    L[ 0] = (lut_idx00 == -1) ? null_val : layer_blk_img[0][lut_idx00].g;
                    L[ 1] = null_val;
                    L[ 2] = (lut_idx01 == -1) ? null_val : layer_blk_img[0][lut_idx01].g;
                    L[ 3] = null_val;
                    L[ 4] = (lut_idx02 == -1) ? null_val : layer_blk_img[0][lut_idx02].g;
                    L[ 5] = null_val;
                    L[ 6] = (lut_idx03 == -1) ? null_val : layer_blk_img[0][lut_idx03].g;
                    L[ 7] = null_val;
                    L[ 8] = (lut_idx04 == -1) ? null_val : layer_blk_img[0][lut_idx04].g;
                    L[ 9] = null_val;
                    L[10] = (lut_idx05 == -1) ? null_val : layer_blk_img[0][lut_idx05].g;
                    L[11] = null_val;
                    L[12] = (lut_idx06 == -1) ? null_val : layer_blk_img[0][lut_idx06].g;
                    L[13] = null_val;
                    L[14] = (lut_idx07 == -1) ? null_val : layer_blk_img[0][lut_idx07].g;
                    L[15] = null_val;
                    L[16] = (lut_idx08 == -1) ? null_val : layer_blk_img[0][lut_idx08].g;
                    L[17] = null_val;
                    L[18] = (lut_idx09 == -1) ? null_val : layer_blk_img[0][lut_idx09].g;
                    L[19] = null_val;
                    L[20] = (lut_idx10 == -1) ? null_val : layer_blk_img[0][lut_idx10].g;
                    L[21] = null_val;
                    L[22] = (lut_idx11 == -1) ? null_val : layer_blk_img[0][lut_idx11].g;
                    L[23] = null_val;
                    L[24] = (lut_idx12 == -1) ? null_val : layer_blk_img[0][lut_idx12].g;
                    L[25] = null_val;
                    L[26] = (lut_idx13 == -1) ? null_val : layer_blk_img[0][lut_idx13].g;
                    L[27] = null_val;
                    L[28] = (lut_idx14 == -1) ? null_val : layer_blk_img[0][lut_idx14].g;
                    L[29] = null_val;
                    L[30] = (lut_idx15 == -1) ? null_val : layer_blk_img[0][lut_idx15].g;
                    L[31] = null_val;
                    break;
                case 2:
                    L[ 0] = (lut_idx00 == -1) ? null_val : layer_blk_img[0][lut_idx00].g;
                    L[ 1] = (lut_idx00 == -1) ? null_val : layer_blk_img[1][lut_idx00].g;
                    L[ 2] = (lut_idx01 == -1) ? null_val : layer_blk_img[0][lut_idx01].g;
                    L[ 3] = (lut_idx01 == -1) ? null_val : layer_blk_img[1][lut_idx01].g;
                    L[ 4] = (lut_idx02 == -1) ? null_val : layer_blk_img[0][lut_idx02].g;
                    L[ 5] = (lut_idx02 == -1) ? null_val : layer_blk_img[1][lut_idx02].g;
                    L[ 6] = (lut_idx03 == -1) ? null_val : layer_blk_img[0][lut_idx03].g;
                    L[ 7] = (lut_idx03 == -1) ? null_val : layer_blk_img[1][lut_idx03].g;
                    L[ 8] = (lut_idx04 == -1) ? null_val : layer_blk_img[0][lut_idx04].g;
                    L[ 9] = (lut_idx04 == -1) ? null_val : layer_blk_img[1][lut_idx04].g;
                    L[10] = (lut_idx05 == -1) ? null_val : layer_blk_img[0][lut_idx05].g;
                    L[11] = (lut_idx05 == -1) ? null_val : layer_blk_img[1][lut_idx05].g;
                    L[12] = (lut_idx06 == -1) ? null_val : layer_blk_img[0][lut_idx06].g;
                    L[13] = (lut_idx06 == -1) ? null_val : layer_blk_img[1][lut_idx06].g;
                    L[14] = (lut_idx07 == -1) ? null_val : layer_blk_img[0][lut_idx07].g;
                    L[15] = (lut_idx07 == -1) ? null_val : layer_blk_img[1][lut_idx07].g;
                    L[16] = (lut_idx08 == -1) ? null_val : layer_blk_img[0][lut_idx08].g;
                    L[17] = (lut_idx08 == -1) ? null_val : layer_blk_img[1][lut_idx08].g;
                    L[18] = (lut_idx09 == -1) ? null_val : layer_blk_img[0][lut_idx09].g;
                    L[19] = (lut_idx09 == -1) ? null_val : layer_blk_img[1][lut_idx09].g;
                    L[20] = (lut_idx10 == -1) ? null_val : layer_blk_img[0][lut_idx10].g;
                    L[21] = (lut_idx10 == -1) ? null_val : layer_blk_img[1][lut_idx10].g;
                    L[22] = (lut_idx11 == -1) ? null_val : layer_blk_img[0][lut_idx11].g;
                    L[23] = (lut_idx11 == -1) ? null_val : layer_blk_img[1][lut_idx11].g;
                    L[24] = (lut_idx12 == -1) ? null_val : layer_blk_img[0][lut_idx12].g;
                    L[25] = (lut_idx12 == -1) ? null_val : layer_blk_img[1][lut_idx12].g;
                    L[26] = (lut_idx13 == -1) ? null_val : layer_blk_img[0][lut_idx13].g;
                    L[27] = (lut_idx13 == -1) ? null_val : layer_blk_img[1][lut_idx13].g;
                    L[28] = (lut_idx14 == -1) ? null_val : layer_blk_img[0][lut_idx14].g;
                    L[29] = (lut_idx14 == -1) ? null_val : layer_blk_img[1][lut_idx14].g;
                    L[30] = (lut_idx15 == -1) ? null_val : layer_blk_img[0][lut_idx15].g;
                    L[31] = (lut_idx15 == -1) ? null_val : layer_blk_img[1][lut_idx15].g;
                    break;
                case 3:
                    L[ 0] = (lut_idx00==-1) ? null_val : layer_blk_img[0][lut_idx00].g;
                    L[ 1] = (lut_idx00==-1) ? null_val : layer_blk_img[1][lut_idx00].g;
                    L[ 2] = (lut_idx00==-1) ? null_val : layer_blk_img[2][lut_idx00].g;
                    L[ 3] = null_val;
                    L[ 4] = (lut_idx01==-1) ? null_val : layer_blk_img[0][lut_idx01].g;
                    L[ 5] = (lut_idx01==-1) ? null_val : layer_blk_img[1][lut_idx01].g;
                    L[ 6] = (lut_idx01==-1) ? null_val : layer_blk_img[2][lut_idx01].g;
                    L[ 7] = null_val;
                    L[ 8] = (lut_idx02==-1) ? null_val : layer_blk_img[0][lut_idx02].g;
                    L[ 9] = (lut_idx02==-1) ? null_val : layer_blk_img[1][lut_idx02].g;
                    L[10] = (lut_idx02==-1) ? null_val : layer_blk_img[2][lut_idx02].g;
                    L[11] = null_val;
                    L[12] = (lut_idx03==-1) ? null_val : layer_blk_img[0][lut_idx03].g;
                    L[13] = (lut_idx03==-1) ? null_val : layer_blk_img[1][lut_idx03].g;
                    L[14] = (lut_idx03==-1) ? null_val : layer_blk_img[2][lut_idx03].g;
                    L[15] = null_val;
                    L[16] = (lut_idx04==-1) ? null_val : layer_blk_img[0][lut_idx04].g;
                    L[17] = (lut_idx04==-1) ? null_val : layer_blk_img[1][lut_idx04].g;
                    L[18] = (lut_idx04==-1) ? null_val : layer_blk_img[2][lut_idx04].g;
                    L[19] = null_val;
                    L[20] = (lut_idx05==-1) ? null_val : layer_blk_img[0][lut_idx05].g;
                    L[21] = (lut_idx05==-1) ? null_val : layer_blk_img[1][lut_idx05].g;
                    L[22] = (lut_idx05==-1) ? null_val : layer_blk_img[2][lut_idx05].g;
                    L[23] = null_val;
                    L[24] = (lut_idx06==-1) ? null_val : layer_blk_img[0][lut_idx06].g;
                    L[25] = (lut_idx06==-1) ? null_val : layer_blk_img[1][lut_idx06].g;
                    L[26] = (lut_idx06==-1) ? null_val : layer_blk_img[2][lut_idx06].g;
                    L[27] = null_val;
                    L[28] = (lut_idx07==-1) ? null_val : layer_blk_img[0][lut_idx07].g;
                    L[29] = (lut_idx07==-1) ? null_val : layer_blk_img[1][lut_idx07].g;
                    L[30] = (lut_idx07==-1) ? null_val : layer_blk_img[2][lut_idx07].g;
                    L[31] = null_val;
                    break;            
                case 4:
                    L[ 0] = (lut_idx00==-1) ? null_val : layer_blk_img[0][lut_idx00].g;
                    L[ 1] = (lut_idx00==-1) ? null_val : layer_blk_img[1][lut_idx00].g;
                    L[ 2] = (lut_idx00==-1) ? null_val : layer_blk_img[2][lut_idx00].g;
                    L[ 3] = (lut_idx00==-1) ? null_val : layer_blk_img[3][lut_idx00].g;
                    L[ 4] = (lut_idx01==-1) ? null_val : layer_blk_img[0][lut_idx01].g;
                    L[ 5] = (lut_idx01==-1) ? null_val : layer_blk_img[1][lut_idx01].g;
                    L[ 6] = (lut_idx01==-1) ? null_val : layer_blk_img[2][lut_idx01].g;
                    L[ 7] = (lut_idx01==-1) ? null_val : layer_blk_img[3][lut_idx01].g;
                    L[ 8] = (lut_idx02==-1) ? null_val : layer_blk_img[0][lut_idx02].g;
                    L[ 9] = (lut_idx02==-1) ? null_val : layer_blk_img[1][lut_idx02].g;
                    L[10] = (lut_idx02==-1) ? null_val : layer_blk_img[2][lut_idx02].g;
                    L[11] = (lut_idx02==-1) ? null_val : layer_blk_img[3][lut_idx02].g;
                    L[12] = (lut_idx03==-1) ? null_val : layer_blk_img[0][lut_idx03].g;
                    L[13] = (lut_idx03==-1) ? null_val : layer_blk_img[1][lut_idx03].g;
                    L[14] = (lut_idx03==-1) ? null_val : layer_blk_img[2][lut_idx03].g;
                    L[15] = (lut_idx03==-1) ? null_val : layer_blk_img[3][lut_idx03].g;
                    L[16] = (lut_idx04==-1) ? null_val : layer_blk_img[0][lut_idx04].g;
                    L[17] = (lut_idx04==-1) ? null_val : layer_blk_img[1][lut_idx04].g;
                    L[18] = (lut_idx04==-1) ? null_val : layer_blk_img[2][lut_idx04].g;
                    L[19] = (lut_idx04==-1) ? null_val : layer_blk_img[3][lut_idx04].g;
                    L[20] = (lut_idx05==-1) ? null_val : layer_blk_img[0][lut_idx05].g;
                    L[21] = (lut_idx05==-1) ? null_val : layer_blk_img[1][lut_idx05].g;
                    L[22] = (lut_idx05==-1) ? null_val : layer_blk_img[2][lut_idx05].g;
                    L[23] = (lut_idx05==-1) ? null_val : layer_blk_img[3][lut_idx05].g;
                    L[24] = (lut_idx06==-1) ? null_val : layer_blk_img[0][lut_idx06].g;
                    L[25] = (lut_idx06==-1) ? null_val : layer_blk_img[1][lut_idx06].g;
                    L[26] = (lut_idx06==-1) ? null_val : layer_blk_img[2][lut_idx06].g;
                    L[27] = (lut_idx06==-1) ? null_val : layer_blk_img[3][lut_idx06].g;
                    L[28] = (lut_idx07==-1) ? null_val : layer_blk_img[0][lut_idx07].g;
                    L[29] = (lut_idx07==-1) ? null_val : layer_blk_img[1][lut_idx07].g;
                    L[30] = (lut_idx07==-1) ? null_val : layer_blk_img[2][lut_idx07].g;
                    L[31] = (lut_idx07==-1) ? null_val : layer_blk_img[3][lut_idx07].g;
                    break;
                case 5:
                    L[ 0] = (lut_idx00==-1) ? null_val : layer_blk_img[0][lut_idx00].g;
                    L[ 1] = (lut_idx00==-1) ? null_val : layer_blk_img[1][lut_idx00].g;
                    L[ 2] = (lut_idx00==-1) ? null_val : layer_blk_img[2][lut_idx00].g;
                    L[ 3] = (lut_idx00==-1) ? null_val : layer_blk_img[3][lut_idx00].g;
                    L[ 4] = (lut_idx00==-1) ? null_val : layer_blk_img[4][lut_idx00].g;
                    L[ 5] = null_val;
                    L[ 6] = null_val;
                    L[ 7] = null_val;
                    L[ 8] = (lut_idx01==-1) ? null_val : layer_blk_img[0][lut_idx01].g;
                    L[ 9] = (lut_idx01==-1) ? null_val : layer_blk_img[1][lut_idx01].g;
                    L[10] = (lut_idx01==-1) ? null_val : layer_blk_img[2][lut_idx01].g;
                    L[11] = (lut_idx01==-1) ? null_val : layer_blk_img[3][lut_idx01].g;
                    L[12] = (lut_idx01==-1) ? null_val : layer_blk_img[4][lut_idx01].g;
                    L[13] = null_val;
                    L[14] = null_val;
                    L[15] = null_val;
                    L[16] = (lut_idx02==-1) ? null_val : layer_blk_img[0][lut_idx02].g;
                    L[17] = (lut_idx02==-1) ? null_val : layer_blk_img[1][lut_idx02].g;
                    L[18] = (lut_idx02==-1) ? null_val : layer_blk_img[2][lut_idx02].g;
                    L[19] = (lut_idx02==-1) ? null_val : layer_blk_img[3][lut_idx02].g;
                    L[20] = (lut_idx02==-1) ? null_val : layer_blk_img[4][lut_idx02].g;
                    L[21] = null_val;
                    L[22] = null_val;
                    L[23] = null_val;
                    L[24] = (lut_idx03==-1) ? null_val : layer_blk_img[0][lut_idx03].g;
                    L[25] = (lut_idx03==-1) ? null_val : layer_blk_img[1][lut_idx03].g;
                    L[26] = (lut_idx03==-1) ? null_val : layer_blk_img[2][lut_idx03].g;
                    L[27] = (lut_idx03==-1) ? null_val : layer_blk_img[3][lut_idx03].g;
                    L[28] = (lut_idx03==-1) ? null_val : layer_blk_img[4][lut_idx03].g;
                    L[29] = null_val;
                    L[30] = null_val;
                    L[31] = null_val;
                    break;
                case 6:
                    L[ 0] = (lut_idx00==-1) ? null_val : layer_blk_img[0][lut_idx00].g;
                    L[ 1] = (lut_idx00==-1) ? null_val : layer_blk_img[1][lut_idx00].g;
                    L[ 2] = (lut_idx00==-1) ? null_val : layer_blk_img[2][lut_idx00].g;
                    L[ 3] = (lut_idx00==-1) ? null_val : layer_blk_img[3][lut_idx00].g;
                    L[ 4] = (lut_idx00==-1) ? null_val : layer_blk_img[4][lut_idx00].g;
                    L[ 5] = (lut_idx00==-1) ? null_val : layer_blk_img[5][lut_idx00].g;
                    L[ 6] = null_val;
                    L[ 7] = null_val;
                    L[ 8] = (lut_idx01==-1) ? null_val : layer_blk_img[0][lut_idx01].g;
                    L[ 9] = (lut_idx01==-1) ? null_val : layer_blk_img[1][lut_idx01].g;
                    L[10] = (lut_idx01==-1) ? null_val : layer_blk_img[2][lut_idx01].g;
                    L[11] = (lut_idx01==-1) ? null_val : layer_blk_img[3][lut_idx01].g;
                    L[12] = (lut_idx01==-1) ? null_val : layer_blk_img[4][lut_idx01].g;
                    L[13] = (lut_idx01==-1) ? null_val : layer_blk_img[5][lut_idx01].g;
                    L[14] = null_val;
                    L[15] = null_val;
                    L[16] = (lut_idx02==-1) ? null_val : layer_blk_img[0][lut_idx02].g;
                    L[17] = (lut_idx02==-1) ? null_val : layer_blk_img[1][lut_idx02].g;
                    L[18] = (lut_idx02==-1) ? null_val : layer_blk_img[2][lut_idx02].g;
                    L[19] = (lut_idx02==-1) ? null_val : layer_blk_img[3][lut_idx02].g;
                    L[20] = (lut_idx02==-1) ? null_val : layer_blk_img[4][lut_idx02].g;
                    L[21] = (lut_idx02==-1) ? null_val : layer_blk_img[5][lut_idx02].g;
                    L[22] = null_val;
                    L[23] = null_val;
                    L[24] = (lut_idx03==-1) ? null_val : layer_blk_img[0][lut_idx03].g;
                    L[25] = (lut_idx03==-1) ? null_val : layer_blk_img[1][lut_idx03].g;
                    L[26] = (lut_idx03==-1) ? null_val : layer_blk_img[2][lut_idx03].g;
                    L[27] = (lut_idx03==-1) ? null_val : layer_blk_img[3][lut_idx03].g;
                    L[28] = (lut_idx03==-1) ? null_val : layer_blk_img[4][lut_idx03].g;
                    L[29] = (lut_idx03==-1) ? null_val : layer_blk_img[5][lut_idx03].g;
                    L[30] = null_val;
                    L[31] = null_val;
                    break;
                case 7:
                    L[ 0] = (lut_idx00==-1) ? null_val : layer_blk_img[0][lut_idx00].g;
                    L[ 1] = (lut_idx00==-1) ? null_val : layer_blk_img[1][lut_idx00].g;
                    L[ 2] = (lut_idx00==-1) ? null_val : layer_blk_img[2][lut_idx00].g;
                    L[ 3] = (lut_idx00==-1) ? null_val : layer_blk_img[3][lut_idx00].g;
                    L[ 4] = (lut_idx00==-1) ? null_val : layer_blk_img[4][lut_idx00].g;
                    L[ 5] = (lut_idx00==-1) ? null_val : layer_blk_img[5][lut_idx00].g;
                    L[ 6] = (lut_idx00==-1) ? null_val : layer_blk_img[6][lut_idx00].g;
                    L[ 7] = null_val;
                    L[ 8] = (lut_idx01==-1) ? null_val : layer_blk_img[0][lut_idx01].g;
                    L[ 9] = (lut_idx01==-1) ? null_val : layer_blk_img[1][lut_idx01].g;
                    L[10] = (lut_idx01==-1) ? null_val : layer_blk_img[2][lut_idx01].g;
                    L[11] = (lut_idx01==-1) ? null_val : layer_blk_img[3][lut_idx01].g;
                    L[12] = (lut_idx01==-1) ? null_val : layer_blk_img[4][lut_idx01].g;
                    L[13] = (lut_idx01==-1) ? null_val : layer_blk_img[5][lut_idx01].g;
                    L[14] = (lut_idx01==-1) ? null_val : layer_blk_img[6][lut_idx01].g;
                    L[15] = null_val;
                    L[16] = (lut_idx02==-1) ? null_val : layer_blk_img[0][lut_idx02].g;
                    L[17] = (lut_idx02==-1) ? null_val : layer_blk_img[1][lut_idx02].g;
                    L[18] = (lut_idx02==-1) ? null_val : layer_blk_img[2][lut_idx02].g;
                    L[19] = (lut_idx02==-1) ? null_val : layer_blk_img[3][lut_idx02].g;
                    L[20] = (lut_idx02==-1) ? null_val : layer_blk_img[4][lut_idx02].g;
                    L[21] = (lut_idx02==-1) ? null_val : layer_blk_img[5][lut_idx02].g;
                    L[22] = (lut_idx02==-1) ? null_val : layer_blk_img[6][lut_idx02].g;
                    L[23] = null_val;
                    L[24] = (lut_idx03==-1) ? null_val : layer_blk_img[0][lut_idx03].g;
                    L[25] = (lut_idx03==-1) ? null_val : layer_blk_img[1][lut_idx03].g;
                    L[26] = (lut_idx03==-1) ? null_val : layer_blk_img[2][lut_idx03].g;
                    L[27] = (lut_idx03==-1) ? null_val : layer_blk_img[3][lut_idx03].g;
                    L[28] = (lut_idx03==-1) ? null_val : layer_blk_img[4][lut_idx03].g;
                    L[29] = (lut_idx03==-1) ? null_val : layer_blk_img[5][lut_idx03].g;
                    L[30] = (lut_idx03==-1) ? null_val : layer_blk_img[6][lut_idx03].g;
                    L[31] = null_val;
                    break;
                case 8:
                    L[ 0] = (lut_idx00==-1) ? null_val : layer_blk_img[0][lut_idx00].g;
                    L[ 1] = (lut_idx00==-1) ? null_val : layer_blk_img[1][lut_idx00].g;
                    L[ 2] = (lut_idx00==-1) ? null_val : layer_blk_img[2][lut_idx00].g;
                    L[ 3] = (lut_idx00==-1) ? null_val : layer_blk_img[3][lut_idx00].g;
                    L[ 4] = (lut_idx00==-1) ? null_val : layer_blk_img[4][lut_idx00].g;
                    L[ 5] = (lut_idx00==-1) ? null_val : layer_blk_img[5][lut_idx00].g;
                    L[ 6] = (lut_idx00==-1) ? null_val : layer_blk_img[6][lut_idx00].g;
                    L[ 7] = (lut_idx00==-1) ? null_val : layer_blk_img[7][lut_idx00].g;
                    L[ 8] = (lut_idx01==-1) ? null_val : layer_blk_img[0][lut_idx01].g;
                    L[ 9] = (lut_idx01==-1) ? null_val : layer_blk_img[1][lut_idx01].g;
                    L[10] = (lut_idx01==-1) ? null_val : layer_blk_img[2][lut_idx01].g;
                    L[11] = (lut_idx01==-1) ? null_val : layer_blk_img[3][lut_idx01].g;
                    L[12] = (lut_idx01==-1) ? null_val : layer_blk_img[4][lut_idx01].g;
                    L[13] = (lut_idx01==-1) ? null_val : layer_blk_img[5][lut_idx01].g;
                    L[14] = (lut_idx01==-1) ? null_val : layer_blk_img[6][lut_idx01].g;
                    L[15] = (lut_idx01==-1) ? null_val : layer_blk_img[7][lut_idx01].g;
                    L[16] = (lut_idx02==-1) ? null_val : layer_blk_img[0][lut_idx02].g;
                    L[17] = (lut_idx02==-1) ? null_val : layer_blk_img[1][lut_idx02].g;
                    L[18] = (lut_idx02==-1) ? null_val : layer_blk_img[2][lut_idx02].g;
                    L[19] = (lut_idx02==-1) ? null_val : layer_blk_img[3][lut_idx02].g;
                    L[20] = (lut_idx02==-1) ? null_val : layer_blk_img[4][lut_idx02].g;
                    L[21] = (lut_idx02==-1) ? null_val : layer_blk_img[5][lut_idx02].g;
                    L[22] = (lut_idx02==-1) ? null_val : layer_blk_img[6][lut_idx02].g;
                    L[23] = (lut_idx02==-1) ? null_val : layer_blk_img[7][lut_idx02].g;
                    L[24] = (lut_idx03==-1) ? null_val : layer_blk_img[0][lut_idx03].g;
                    L[25] = (lut_idx03==-1) ? null_val : layer_blk_img[1][lut_idx03].g;
                    L[26] = (lut_idx03==-1) ? null_val : layer_blk_img[2][lut_idx03].g;
                    L[27] = (lut_idx03==-1) ? null_val : layer_blk_img[3][lut_idx03].g;
                    L[28] = (lut_idx03==-1) ? null_val : layer_blk_img[4][lut_idx03].g;
                    L[29] = (lut_idx03==-1) ? null_val : layer_blk_img[5][lut_idx03].g;
                    L[30] = (lut_idx03==-1) ? null_val : layer_blk_img[6][lut_idx03].g;
                    L[31] = (lut_idx03==-1) ? null_val : layer_blk_img[7][lut_idx03].g;
                    break;
                }

                int idx;
                for (idx = 0; idx < 32; idx++)
                {
                    char rr = (char)((L[idx] < 0.0) ? (256.0 + L[idx]) : (L[idx]));

                    Lut_out[out_idx++]= rr;
                    
                }
            }
        }
    }
    
    pLut_out_vector->dat_num += out_idx;
    
    if (pLut_out_vector->dat_num > pLut_out_vector->max_num)
    {
        printf("\n\033[31;1mError !\033[0m\n");
        printf("Lut_out_vector data num  : 0x%x\n", pLut_out_vector->dat_num);
        printf("Lut_out_vector max  num  : 0x%x\n", pLut_out_vector->max_num);
    }
}


int GETBITS(const int Value , const int MSB_bit, const int LSB_bit)
{
    int Mask = (1<<(MSB_bit - LSB_bit + 1)) - 1;
    int Shift = LSB_bit;

    int step1 = (Value>>Shift);
    int step2 = (step1&Mask);

    return step2;
}


int register_08bits_inv( _08bits reg_tmp, int total_bits)
{
    int shift_bit = 0;
    int shift_bit_tmp = 0;
    int mask_tmp = reg_tmp.Bit07_00.mask;
    
    int count_i;
    for (count_i = 1; count_i < 8; count_i++)
    {
        shift_bit_tmp = (mask_tmp&0x1);
        mask_tmp = (mask_tmp>>1);

        if(shift_bit_tmp==0)
            shift_bit = count_i;
        else
            break;

    }

    int value = reg_tmp.Bit07_00.value;

    int LSB_bit = shift_bit;
    int MSB_bit = (LSB_bit + total_bits - 1);

    int final_value = GETBITS( value , MSB_bit, LSB_bit);

    return final_value;
}

int register_16bits_inv( _16bits reg_tmp, int MSB_total_bits, int LSB_total_bits)
{
    //------------- LSB process start -------------//
    int shift_bit = 0;
    int shift_bit_tmp = 0;
    int mask_tmp = reg_tmp.Bit07_00.mask;
    
    int count_i;
    for (count_i = 1; count_i < 8; count_i++)
    {
        shift_bit_tmp = (mask_tmp&0x1);
        mask_tmp = (mask_tmp>>1);

        if(shift_bit_tmp==0)
            shift_bit = count_i;
        else
            break;

    }

    int value = reg_tmp.Bit07_00.value;

    int LSB_bit = shift_bit;
    int MSB_bit = (LSB_bit + LSB_total_bits - 1);

    int LSB_value = GETBITS( value , MSB_bit, LSB_bit);
    //------------- LSB process start -------------//


    //------------- MSB process start -------------//
    shift_bit = 0;
    shift_bit_tmp = 0;
    mask_tmp = reg_tmp.Bit15_08.mask;
    for (count_i = 1; count_i < 8; count_i++)
    {
        shift_bit_tmp = (mask_tmp&0x1);
        mask_tmp = (mask_tmp>>1);

        if(shift_bit_tmp==0)
            shift_bit = count_i;
        else
            break;
    }

    value = reg_tmp.Bit15_08.value;

    LSB_bit = shift_bit;
    MSB_bit = (LSB_bit + MSB_total_bits - 1);

    int MSB_value = GETBITS( value , MSB_bit, LSB_bit);
    //------------- LSB process start -------------//

    int final_value = (MSB_value << LSB_total_bits) + LSB_value;

    return final_value;
}

int twos_complement_to_integer(const int input_value, const int total_bits)
{
    int sign_bit = 1<<(total_bits-1);
    int inverse_value = 1<<total_bits;

    int final_value = (input_value>=sign_bit) ? (input_value - inverse_value) : input_value;

    return final_value;
}

double DMC_LUT_Fix_To_Double(const int Layer_offset_2s, const int Layer_gain, const int Lut_value)
{
    int Layer_offset_2s_to_interger = twos_complement_to_integer( Layer_offset_2s, 14);
    double Layer_offset_double = (double)Layer_offset_2s_to_interger/(double)16;  // format s9.4 base on 10bit data path

    int Lut_value_2s_to_interger = twos_complement_to_integer( Lut_value, 8);
    
    double Lut_value_double = 0.0;
    switch(Layer_gain)
    {
    case 6:  // s3.4 base on 10bit data path
        Lut_value_double = (double)Lut_value_2s_to_interger/(double)16;
        break;
    case 7:  // s4.3 base on 10bit data path
        Lut_value_double = (double)Lut_value_2s_to_interger/(double)8;
        break;
    case 0:  // s5.2 base on 10bit data path
        Lut_value_double = (double)Lut_value_2s_to_interger/(double)4;
        break;
    case 1:  // s6.1 base on 10bit data path
        Lut_value_double = (double)Lut_value_2s_to_interger/(double)2;
        break;
    case 2:  // s7.0 base on 10bit data path
        Lut_value_double = (double)Lut_value_2s_to_interger/(double)1;
        break;
    }

    double final_value = Layer_offset_double + Lut_value_double;

    return final_value;

}


#ifdef CONFIG_DEMURA_VENDOR_LGD

#define minmax(v,a,b) (((v)<(a))? (a) : ((v)>(b)) ? (b) : (v))

MS_BOOL bit_mode_LPF(int plane_idx, int block_width, int block_height, strgb_structInfo *layer_blk_img_out)
{
    int LPF[5][5] = {{ 0, 1, 1, 1, 0 },
                     { 1, 3, 7, 3, 1 },
                     { 1, 7, 12, 7, 1 },
                     { 1, 3, 7, 3, 1 },
                     { 0, 1, 1, 1, 0 } }; //sum=64

    DEFINE_ARRAY_DIM2(_tmp, sizeof(int), block_height, block_width);
    int ** tmp = (int **)PTR_ARRAY_DIM2(_tmp);

    //! We only use RGB"W" channel (i.e., channel 3)
    //! Shift according to bit_mode
    int CurH, CurV;
    for (CurV = 0; CurV < block_height; CurV++)
    {
        for (CurH = 0; CurH < block_width; CurH++)
        {              
            tmp[CurV][CurH] = (int)(layer_blk_img_out[posi_blk(CurV,CurH)].r);
        }
    }

    //! LPF
    for (CurV = 0; CurV < block_height; CurV++)
    {
        for (CurH = 0; CurH < block_width; CurH++)
        {
            int sum = 0;
            int x_sh, y_sh;
            for (x_sh = -2; x_sh < 3; x_sh++)
            {
                for (y_sh = -2; y_sh < 3; y_sh++)
                {
                    sum += tmp[minmax(CurV + y_sh, 0, (block_height-1))][minmax(CurH + x_sh, 0, (block_width-1))] * LPF[y_sh + 2][x_sh + 2];
                }
            }

            //double uuu = (double)minmax((double)sum / (double)64, -128, 127);
            
            // R channel
            layer_blk_img_out[posi_blk(CurV,CurH)].dbr = (double)minmax((double)sum / (double)64, -128, 127); // Clamp to s7
            layer_blk_img_out[posi_blk(CurV,CurH)].r   = (int)minmax(sum / 64, -128, 127); // Clamp to s7
            // G channel
            layer_blk_img_out[posi_blk(CurV,CurH)].dbg = (double)minmax((double)sum / (double)64, -128, 127); // Clamp to s7
            layer_blk_img_out[posi_blk(CurV,CurH)].g   = (int)minmax(sum / 64, -128, 127); // Clamp to s7
            // B channel
            layer_blk_img_out[posi_blk(CurV,CurH)].dbb = (double)minmax((double)sum / (double)64, -128, 127); // Clamp to s7
            layer_blk_img_out[posi_blk(CurV,CurH)].b   = (int)minmax(sum / 64, -128, 127); // Clamp to s7
        }
    }

    FREE_ARRAY_DIM2(_tmp);
    return TRUE;
}
#endif

