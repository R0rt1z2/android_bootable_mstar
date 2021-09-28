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

#ifndef __MS_SECURE_TEE_H__
#define __MS_SECURE_TEE_H__


#if defined (CONFIG_R2_BRINGUP) || defined (CONFIG_AMP_TRUSTZONE_ENABLE)
#define HEADER_SIZE 128
#define MAGIC_ID_LEN 16
#define BOOT_CONFIG_FLAG_SIZE 1
#define ONE_G_MIU_INTERVAL_FLAG_SIZE 1

#define DEBUG_INFO_SIZE 0x200

extern char TeeMagicID[MAGIC_ID_LEN];

typedef struct
{
    unsigned int uitee_header_len;
    unsigned int uitee_start_paddr;
    unsigned int uitee_start_vaddr;
    unsigned int uitee_image_size;
    unsigned long long ulltime;
    unsigned long long ullTeeBinVersion;
    unsigned int uitee_mem_len;
    unsigned int uihw_aes_paddr;
    unsigned int uihw_aes_buffer_len;
    unsigned int uiHeaderVersion;
    unsigned int uicrc_value;
}_ST_TEE_HEADER;


typedef struct
{
    _ST_TEE_HEADER _sthd;
    unsigned char reserved[HEADER_SIZE-sizeof(_ST_TEE_HEADER)-ONE_G_MIU_INTERVAL_FLAG_SIZE-BOOT_CONFIG_FLAG_SIZE-MAGIC_ID_LEN];
    unsigned char one_g_miu_interval[ONE_G_MIU_INTERVAL_FLAG_SIZE];
    unsigned char bootConfigFlag[BOOT_CONFIG_FLAG_SIZE];
    unsigned char ucMagicID[MAGIC_ID_LEN];
}ST_TEE_HEADER;
#endif

#if defined(CONFIG_R2TEE_TEELOADER)
int do_uart2R2(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
#else
int do_readNuttx (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_bootNuttx(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_load_nconfig(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_wait_tee_ready(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
#ifdef CONFIG_MSTAR_R2_STR
int do_r2_resume(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
#endif
#endif // #if defined(CONFIG_R2TEE_TEELOADER)

int do_set_tee_debug_level (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_save_nconfig(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
#endif
