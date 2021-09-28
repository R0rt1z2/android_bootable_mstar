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
#ifndef _DEMURA_COMMON_H_
#define _DEMURA_COMMON_H_

// Function declare
int demura_layer_distance_div(int pre, int nxt);
int register_shift(int mask, int value);
void part_bit15_00(int input_value, int total_bits, int *Bit15_08, int *Bit07_00);
void part_bit23_00(int input_value, int total_bits, int *Bit23_16, int *Bit15_08, int *Bit07_00);
void demura_table_normalization(int lut_size, double *lut_in, int *lut_out,int *layer_gain_out);
void demura_table_normalization1(int lut_size, double *lut_in, int *lut_out, int *layer_gain_out);
void demura_table_normalization_ROI(int lut_h_size, int lut_v_size, int ROI_h_start_offset, int ROI_h_end_offset, int ROI_v_start_offset, int ROI_v_end_offset, double *lut_in, int *lut_out, int *layer_gain_out);
void demura_table_normalization_inv(int lut_size, int layer_gain,int *lut_in,double *lut_out);
void coeff_and_ks(int diff, int *coeff, int *ks);
void FIND_MIN_AND_MAX(int RGB_channel_idx, int block_width, int block_height, strgb_structInfo *layer_blk_img, double *rgb_max, double *rgb_min);
void LUT_SETTING_SUBFUNCTION(int RGB_idx, int plane_idx, int block_width, int block_height, int reg_dmc_plane_level, registers *preg, strgb_structInfo *layer_blk_img, strgb_structInfo *layer_blk_img_out);
void LUT_AND_SETTING_PROCESSOR( int plane_idx, int block_width, int block_height, int reg_dmc_plane_level, registers *preg , strgb_structInfo *layer_blk_img_in, strgb_structInfo *layer_blk_img_out);
void write_demuraLUT_to_HW_DRAM_FORMAT(stlayer_info_structInfo *layer_info, strgb_structInfo *layer_blk_img[8], m_vector *pLut_out_vector);

int GETBITS( const int Value , const int MSB_bit, const int LSB_bit);
int register_08bits_inv( _08bits reg_tmp, int total_bits);
int register_16bits_inv( _16bits reg_tmp, int MSB_total_bits, int LSB_total_bits);
int twos_complement_to_integer(const int input_value, const int total_bits);
double DMC_LUT_Fix_To_Double(const int Layer_offset_2s, const int Layer_gain, const int Lut_value);

// extern function
extern MS_BOOL demura_core_ver10(int base_address, int lut_h_size, int lut_v_size, interface_info *pInfo, m_vector *pRegVector, m_vector *Lut_out);

#endif