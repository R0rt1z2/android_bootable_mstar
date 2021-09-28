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

#ifndef __MS_DECOMPRESS_PRIV_H__
#define __MS_DECOMPRESS_PRIV_H__

#if 0

#define OFFSET_BITS     12
#define LENGTH_BITS     4

#define WINDOW_SIZE     (1 << OFFSET_BITS)

#define MAX_UNCODED     2
#define MAX_CODED       ((1 << LENGTH_BITS) + MAX_UNCODED)

#endif

#define UNCOMPRESSED_LENGTH     (14336UL)

#ifdef __mips__

#define BOOT_LOADER_LENGTH      (0x30000)
#define COMPRESSED_LENGTH_POS   (0xFCAUL)

#if defined(CONFIG_MSTAR_EMERALD)\
   ||defined(CONFIG_MSTAR_MILAN)\
   ||defined(CONFIG_MSTAR_MARLON)\
   ||defined(CONFIG_MSTAR_M5321)
    #define DECOMPRESS_BUF              (0xA2000000UL) // 32M
    #define DECOMPRESS_BUF_CACHEABLE    (0x82000000UL) // 32M
    #define MEMORY_POOL_CACHEABLE       (0x83000000UL) // 40M
#else
    #define DECOMPRESS_BUF              (0xA1100000UL) // 17M
    #define DECOMPRESS_BUF_CACHEABLE    (0x81100000UL) // 17M
    #define MEMORY_POOL_CACHEABLE       (0x82100000UL) // 31M
#endif

#define NON_CACHEABLE_TO_CACHEABLE_MASK (0xDFFFFFFF)

#elif defined(__arm__)
    #define MS_BUS_MIU0_0000M     CONFIG_MIU0_BUSADDR
    #define BOOT_LOADER_LENGTH    (0x30000)
    #define COMPRESSED_LENGTH_POS    (0xFCAUL)

    //jc_db: need to fix cache problem
    #define DECOMPRESS_BUF                 (MS_BUS_MIU0_0000M+(CONFIG_DRAM_SIZE_MB_RELOCATE-19)*0x100000)
    #define DECOMPRESS_BUF_CACHEABLE       (DECOMPRESS_BUF)
    #define MEMORY_POOL_CACHEABLE          (MS_BUS_MIU0_0000M+(CONFIG_DRAM_SIZE_MB_RELOCATE-13)*0x100000)
    #define NON_CACHEABLE_TO_CACHEABLE_MASK (0xFFFFFFFF)
#else

    #define COMPRESSED_LENGTH_POS   (0x114AUL)

    #define DECOMPRESS_BUF              (0x01100000UL)
    #define DECOMPRESS_BUF_CACHEABLE    (0x01100000UL)
    #define MEMORY_POOL_CACHEABLE       (0x01800000UL)

    #define NON_CACHEABLE_TO_CACHEABLE_MASK (0xFFFFFFFF)
#endif
#define CRC32_POS	(0x14)

#define MEMORY_POOL_SIZE			(0x0A00000UL)


#if 0

#define ENCODED         0       // encoded string
#define UNCODED         1       // unencoded character

typedef struct encoded_string_t
{
    unsigned int offset;    // offset to start of longest match
    unsigned int length;    // length of longest match
} encoded_string_t;

#define Wrap(value, limit)      (value&0xFFF) //(((value) < (limit)) ? (value) : ((value) - (limit)))

#endif

#endif /* __MS_DECOMPRESS_PRIV_H__ */
