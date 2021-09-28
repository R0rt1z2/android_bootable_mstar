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

#ifndef __Ms_UPGRADE_H__
#define __Ms_UPGRADE_H__

#include <MsTypes.h>
#include <ShareType.h>
#include <common.h>

#define UPGRADE_IMAGE_BUFFER_ADDRESS 0x20280000
#define DEFAULT_SEGMENT_SIZE 8192

#ifdef CONFIG_UPGRADE_WITH_FAIL_HANDLER
#define HEADER_CHUNKINFO_SIZE 0x80
#define HEADER_IMAGE_OFFSET_SIZE 0x200
#define SIGNATURE_SIZE 0x100
#define PACK_HEADER_SIZE 0x1000
#define TEE_SCRIPT_SIZE 0x4000
#define REE_SCRIPT_SIZE 0x10000

#define CHUNK_ID "MSTAR..."
#define CHUNK_END "...mstar"

typedef struct image_offset_info
{
    // offset info of TEE image and REE image
    U32 u32TEEImageOffsetStart;
    U32 u32TEEImageOffsetLEN;
    U32 u32REEImageOffsetStart;
    U32 u32REEImageOffsetLEN;

    // offset for firmware image header
    int signOfhashSetSize;
    int chunkInfoSize;
    int imageOffsetSize;
    int signOfHeaderSize;
}ST_IMAGE_OFFSET_INFO;

typedef struct Chunk_Info_Structure
{
    unsigned int segmentSize;
    unsigned int fileDataOffset;
    unsigned int fileDataLen;
    unsigned int fileHashOffset;
    unsigned int fileHashLen;
}ST_CHUNK_INFO;
#endif

unsigned int GetfileSizeforUpgrade(const char *Interface,const char *device,char *file);

#if(ENABLE_MODULE_NET==1)
int do_mstar (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
#endif

#ifdef HB_CUS
int do_ustar_hb(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[]);
int do_check_usb_upgrade(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_auto_update (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
#endif

#if(ENABLE_MODULE_USB == 1)
int do_ustar(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_udstar (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
#endif
#if ((ENABLE_MODULE_NETUPDATE == 1)||(ENABLE_MODULE_OAD== 1))
int do_nstar(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_ostar(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
#endif
#if CONFIG_RESCUE_ENV
int do_bstar(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
#endif

#if (ENABLE_MODULE_ENV_UPGRADE_FROM_BANK == 1)
int do_rstar(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
#endif

#if ((ENABLE_MODULE_OAD == 1) || (ENABLE_MODULE_USB == 1)||(ENABLE_MODULE_NETUPDATE == 1))
int do_mboot_upgrade(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
#endif

#if defined(CONFIG_MSTAR_NS_UBOOT) || defined(CONFIG_OPTEE_TEELOADER)
int do_keybank_upgrade(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
#endif

#if defined(UFBL_FEATURE_ONETIME_UNLOCK)
int do_onetimeunlock(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
#endif

int read_dynamic_script_size(char *ScriptBuf);

#ifdef CONFIG_URSA_6M60
int do_terra2_fast_upgrade(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
#endif

#ifdef CONFIG_AN_FASTBOOT_ENABLE
int do_sparse_write(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
#endif

int do_auto_tftp(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_auto_flash(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);

#endif
