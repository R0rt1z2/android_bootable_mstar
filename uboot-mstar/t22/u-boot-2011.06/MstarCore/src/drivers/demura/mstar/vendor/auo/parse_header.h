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
#ifndef _PARSE_HEADER_H_
#define _PARSE_HEADER_H_

typedef struct
{
#if __BYTE_ORDER == __LITTLE_ENDIAN
    unsigned high     : 4;
    unsigned reserve  : 4;
#else /* __BIG_ENDIAN */
    unsigned reserve  : 4;
    unsigned high     : 4;
#endif
    unsigned char low;

} __attribute__((packed)) AUO_BIN_Panel_Level;


typedef struct
{
    unsigned char dmc_par_checksum[2];

#if __BYTE_ORDER == __LITTLE_ENDIAN
    unsigned dmc_panel_num     :  4;
    unsigned dmc_block_size    :  2;
    unsigned byte_1_reserve    :  1;
    unsigned rgb_mode          :  1;
#else /* __BIG_ENDIAN */
    unsigned rgb_mode          :  1;
    unsigned byte_1_reserve    :  1;
    unsigned dmc_block_size    :  2;
    unsigned dmc_panel_num     :  4;
#endif

#if __BYTE_ORDER == __LITTLE_ENDIAN
    unsigned dmc_v_block       :  3;
    unsigned byte_2_reserve1   :  1;
    unsigned dmc_h_block       :  3;
    unsigned byte_2_reserve    :  1;
#else /* __BIG_ENDIAN */
    unsigned byte_2_reserve    :  1;
    unsigned dmc_h_block       :  3;
    unsigned byte_2_reserve1   :  1;
    unsigned dmc_v_block       :  3;
#endif

#if __BYTE_ORDER == __LITTLE_ENDIAN
    unsigned char dmc_h_lut_num_h;
    unsigned dmc_v_lut_num_h   :  2;
    unsigned byte_3_reserve    :  2;
    unsigned dmc_h_lut_num_l   :  4;
    unsigned char dmc_v_lut_num_l;
#else /* __BIG_ENDIAN */
    unsigned char dmc_h_lut_num_h;
    unsigned dmc_h_lut_num_l   :  4;
    unsigned byte_3_reserve    :  2;
    unsigned dmc_v_lut_num_h   :  2;
    unsigned char dmc_v_lut_num_l;
#endif

#if __BYTE_ORDER == __LITTLE_ENDIAN
    unsigned dmc_panel_78_ks22 :  1;
    unsigned dmc_panel_67_ks22 :  1;
    unsigned dmc_panel_56_ks22 :  1;
    unsigned dmc_panel_45_ks22 :  1;
    unsigned dmc_panel_34_ks22 :  1;
    unsigned dmc_panel_23_ks22 :  1;
    unsigned dmc_panel_12_ks22 :  1;
    unsigned dmc_panel_b1_ks22 :  1;
#else /* __BIG_ENDIAN */
    unsigned dmc_panel_b1_ks22 :  1;
    unsigned dmc_panel_12_ks22 :  1;
    unsigned dmc_panel_23_ks22 :  1;
    unsigned dmc_panel_34_ks22 :  1;
    unsigned dmc_panel_45_ks22 :  1;
    unsigned dmc_panel_56_ks22 :  1;
    unsigned dmc_panel_67_ks22 :  1;
    unsigned dmc_panel_78_ks22 :  1;
#endif

#if __BYTE_ORDER == __LITTLE_ENDIAN
    unsigned byte_4_reserve    :  7;
    unsigned dmc_panel_8w_ks22 :  1;
#else /* __BIG_ENDIAN */
    unsigned dmc_panel_8w_ks22 :  1;
    unsigned byte_4_reserve    :  7;
#endif

#if __BYTE_ORDER == __LITTLE_ENDIAN
    unsigned dmc_black_limit_h :  4;
    unsigned byte_5_reserve    :  4;
    unsigned char dmc_black_limit_l;
#else /* __BIG_ENDIAN */
    unsigned byte_5_reserve    :  4;
    unsigned dmc_black_limit_h :  4;
    unsigned char dmc_black_limit_l;
#endif

    AUO_BIN_Panel_Level panel_levels[8];

#if __BYTE_ORDER == __LITTLE_ENDIAN
    unsigned dmc_white_limit_h :  4;
    unsigned byte_7_reserve    :  4;
    unsigned char dmc_white_limit_l;
#else /* __BIG_ENDIAN */
    unsigned byte_7_reserve    :  4;
    unsigned dmc_white_limit_h :  4;
    unsigned char dmc_white_limit_l;
#endif
    
    unsigned int reverse[8];

} __attribute__((packed)) AUO_BIN_Header;

#endif  // _PARSE_HEADER_H_
