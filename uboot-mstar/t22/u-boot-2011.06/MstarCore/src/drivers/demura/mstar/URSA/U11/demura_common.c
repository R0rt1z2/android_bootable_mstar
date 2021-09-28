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



int demura_layer_distance_div(const int pre, const int nxt)
{
    int distance = nxt - pre;

    distance = (distance==0) ? 1 : distance;

    int distance_div = (int)((double)0xFFFFF/(double)distance + 0.5);

    distance_div = minmax_(distance_div,0x0,0xFFFFF);

    return distance_div;
}

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


void part_bit15_00(const int input_value, const int total_bits, int *Bit15_08, int *Bit07_00)
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


void part_bit23_00(const int input_value, const int total_bits, int *Bit23_16, int *Bit15_08, int *Bit07_00)
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

void demura_table_normalization(int lut_size, double *lut_in, int *lut_out, int *layer_gain_out)
{
    //---------------- search range start ----------------//
    int tmpMax_value = -(1024*4);
    int tmpMin_value =  (1024*4);

    int CurIdx;
    for (CurIdx = 0; CurIdx < lut_size; CurIdx++)
    {                
        int sign = (lut_in[CurIdx]>=0) ? 1 : -1;
        
        int lut_value = (int)(sign*(sign*(lut_in[CurIdx]*4 + 0.5)));

        tmpMax_value = max_(lut_value,tmpMax_value);
        tmpMin_value = min_(lut_value,tmpMin_value);
    }
    //---------------- search range end ----------------//

    //---------------- decision normalize layer gain start ----------------//
    int Value_shift = 0;
    int rounding = 0;

    if((-64<=tmpMin_value)&&(tmpMax_value<=63))
    {
        // range -16.00 ~ 15.75
        *layer_gain_out = 0;
        Value_shift = 0;
        rounding = 0;
    }
    else if((-128<=tmpMin_value)&&(tmpMax_value<=126))
    {
        // range -32.0 ~ 31.5
        *layer_gain_out = 1;
        Value_shift = 1;
        rounding = 1;
    }
    else if((-256<=tmpMin_value)&&(tmpMax_value<=252))
    {
        // range -64 ~ 63
        *layer_gain_out = 2;
        Value_shift = 2;
        rounding = 2;
    }
    else if((-512<=tmpMin_value)&&(tmpMax_value<=508))
    {
        // range -128 ~ 127
        *layer_gain_out = 3;
        Value_shift = 3;
        rounding = 4;
    }
    else if((-1024<=tmpMin_value)&&(tmpMax_value<=1020))
    {
        // range -256 ~ 255
        *layer_gain_out = 4;
        Value_shift = 4;
        rounding = 8;
    }
    else if((-2048<=tmpMin_value)&&(tmpMax_value<=2044))
    {
        // range -512 ~ 511
        *layer_gain_out = 5;
        Value_shift = 5;
        rounding = 16;
    }
    else
    {
        // range -1024 ~ 1023
        *layer_gain_out = 6;
        Value_shift = 6;
        rounding = 32;
    }
    //---------------- decision normalize layer gain end ----------------//

    //---------------- normalize layer table value start ----------------//
    for (CurIdx = 0; CurIdx < lut_size; CurIdx++)
    {
        int sign = (lut_in[CurIdx]>=0) ? 1 : -1;
        int tmpValue = (int)(sign*(sign*lut_in[CurIdx]*4 + 0.5));
        int absValue = abs(tmpValue);
        
        tmpValue = (sign*( (absValue+rounding)>>Value_shift) );        

        tmpValue = minmax_(tmpValue,-64,63);  // keep entry format T6

        tmpValue = (tmpValue<0) ? (128 + tmpValue) : tmpValue;

        lut_out[CurIdx] = tmpValue;
    }
    //---------------- normalize layer table value end ----------------//
}

void demura_table_normalization_inv(int lut_size, int layer_gain,int *lut_in,double *lut_out)
{
    int CurIdx;
    for (CurIdx = 0; CurIdx < lut_size; CurIdx++)
    {
        int table_val = lut_in[CurIdx];

        int signValue = (table_val>=64) ? (table_val - 128) : table_val;

        double table_val_norm_inv = 0.0;

        switch (layer_gain)
        {
        case 0:
            table_val_norm_inv = (double)((double)signValue/4.0);
            break;
        case 1:
            table_val_norm_inv = (double)((double)signValue/2.0);
            break;
        case 2:
            table_val_norm_inv = (double)((double)signValue/1.0);
            break;
        case 3:
            table_val_norm_inv = (double)((double)signValue*2.0);
            break;
        case 4:
            table_val_norm_inv = (double)((double)signValue*4.0);
            break;
        case 5:
            table_val_norm_inv = (double)((double)signValue*8.0);
            break;
        case 6:
        default:
            table_val_norm_inv = (double)((double)signValue*16.0);
            break;
        }

        lut_out[CurIdx] = table_val_norm_inv;
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