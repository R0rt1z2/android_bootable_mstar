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
int demura_layer_distance_div(const int pre, const int nxt);
int register_shift(int mask, int value);
void part_bit15_00(const int input_value, const int total_bits, int *Bit15_08, int *Bit07_00);
void part_bit23_00(const int input_value, const int total_bits, int *Bit23_16, int *Bit15_08, int *Bit07_00);
void demura_table_normalization(int lut_size,double *lut_in,int *lut_out,int *layer_gain_out);
void demura_table_normalization_inv(int lut_size, int layer_gain,int *lut_in,double *lut_out);
int GETBITS( const int Value , const int MSB_bit, const int LSB_bit);
int register_08bits_inv( _08bits reg_tmp, int total_bits);
int register_16bits_inv( _16bits reg_tmp, int MSB_total_bits, int LSB_total_bits);
int twos_complement_to_integer(const int input_value, const int total_bits);
double DMC_LUT_Fix_To_Double(const int Layer_offset_2s, const int Layer_gain, const int Lut_value);

//ver:00
extern MS_BOOL demura_core_ver00(const int base_address, const int lut_h_size, const int lut_v_size, 
                              const int block_h_size, const int block_v_size, const int y_mode_en, 
                              interface_info *pInfo, m_vector *pComRegVector, m_vector *pRegVector, const int packet_size);
#endif