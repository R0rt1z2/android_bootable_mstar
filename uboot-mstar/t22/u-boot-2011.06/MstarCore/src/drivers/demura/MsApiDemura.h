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
#ifndef _MS_API_DEMURA_H_
#define _MS_API_DEMURA_H_

#define DEMURA_BDM_STRING         "DMR"
#define DEMURA_BUFFER_DESC_SIZE   0x400  // 1024 Byte
#define DEMURA_BIN_ALIGN_UNIT     0x400
#define DEMURA_BIN_MAX_NUM        10
#define DEMURA_MSTAR_IP           "mstar demura"
#define DEMURA_FCIC_IP            "fcic demura"

typedef struct
{
    char    ip_string[16];
    MS_U64  phy_addr;
    MS_U64  virt_addr;
    MS_U32  length;
    MS_U32  len_align;
    MS_U32  id_num;
    MS_U32  reserve[5];
} demura_bin_desc;             // 64 Byte


typedef struct
{
    MS_U8   magic_str[4];      // "DMR"
    MS_U32  bin_cur_num;
    MS_U64  mmap_phy_addr;
    MS_U64  mmap_virt_addr;    // 32 Byte
    MS_U32  mmap_length;
    MS_U32  next_bin_addr;     // 36 Byte
    MS_U32  left_space;        // 40 Byte
    MS_U32  reserve[14];       // 96 Byte

    // Always be the last item, offset --> 96
    demura_bin_desc bdesc[DEMURA_BIN_MAX_NUM];
} demura_buf_desc;

MS_BOOL MApi_Demura_Init(PanelType* panel_data);

MS_BOOL push_demura_bin(demura_bin_desc *pbin);
MS_U64  add_dbin_buf(const char *ip_string, MS_U32 filesize, MS_U32 id_num);
void    pop_demura_bin(void);
MS_U32  get_dbuf_length(void);

demura_bin_desc *get_demura_bin(char *ip_string, MS_U32 id_num);

#endif