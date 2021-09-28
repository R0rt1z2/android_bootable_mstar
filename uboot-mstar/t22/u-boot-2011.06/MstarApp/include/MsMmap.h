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

#ifndef __MS_MMAP_H__
#define __MS_MMAP_H__
#include <ShareType.h>
#include <MsTypes.h>
#if defined(CONFIG_ONESN_ENABLE)
#include <MsProjectID.h>
#endif

typedef struct 
{
    U8     u8Name[32];
    U32    u32gID;
    U64    u64Addr;
    U32    u32Size;
    U8     u8Layer;
    U32    u32Align;
    U32    u32miuID;
    U32    u32CMA_HID;    
}MMapInfo_s;

int do_get_mmap(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int set_mmap2env(char *id, U64 *addr, U32 *len);
int setMiu2env(char *miuID);

//-------------------------------------------------------------------------------------------------    
/// get address from supernova's mmap
/// @param id                                  \b IN: id.
/// @param addr                               \b OUT: The address of the id
/// @return int                                  \b OUT: 0:success, -1:fail
/// example:
/// #define E_MMAP_ID_COPROCESSOR_AVAILABLE                        0x0000000000
/// E_MMAP_ID_COPROCESSOR_AVAILABLE is defined in supernova's mmap.
//   If we want to get the address of this item, we have input id "E_MMAP_ID_COPROCESSOR"
///  get_addr_from_mmap("E_MMAP_ID_COPROCESSOR", &addr);
//------------------------------------------------------------------------------------------------- 
int get_addr_from_mmap(char *id, MS_PHYADDR *addr);
int get_64bit_addr_from_mmap(char *id, U64 *addr);

//-------------------------------------------------------------------------------------------------    
/// get length from supernova's mmap
/// @param id                                  \b IN: id.
/// @param len                               \b OUT: The length of the id
/// @return int                                  \b OUT: 0:success, -1:fail
/// example:
/// #define E_MMAP_ID_COPROCESSOR_LEN                              0x0000200000
/// E_MMAP_ID_COPROCESSOR_LEN is defined in supernova's mmap.
//   If we want to get the address of this item, we have input id "E_MMAP_ID_COPROCESSOR"
///  get_length_from_mmap("E_MMAP_ID_COPROCESSOR", &len);
//------------------------------------------------------------------------------------------------- 
int get_length_from_mmap(char *id, U32 *len);

//-------------------------------------------------------------------------------------------------    
/// get dram length from SN's mmap
/// @param length                                  \b OUT: The length of the dram
/// @return int                                  \b OUT: 0:success, -1:fail
//------------------------------------------------------------------------------------------------- 
int get_dram_length(U32 *length);
MS_U64 get_specific_dram_length(int miu_number);


int save_addr_to_env(char *id, U64 addr);

int del_addr_from_env(char *id);

int save_len_to_env(char *id, U32 len);

int del_len_from_env(char *id);

int get_mmap(char *id, MMapInfo_s *mmapInfo);

void setCMA2bootargs(void);

#ifdef CONFIG_UMA_WITH_NARROW
void setDMAzoneargs(void);
#endif

int isDetectMmap(void);

void del_bootargs_CMA(MS_BOOL bDontSaveEnv);

#endif
