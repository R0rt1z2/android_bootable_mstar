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
#include <stdio.h>
#include <assert.h>
#include "datatype.h"
#include "mhal_arch.h"

#define ALIGN_16(_x)            ((_x + 15) & ~15)           // No data type specified, optimized by complier
#define KSEG0_BASE	            ((void *)0x80000000)
#define KSEG1_BASE	            ((void *)0xa0000000)

// Data cache

#define NOS_HAL_DCACHE_SIZE                 4096                        // Size of data cache in bytes
#define NOS_HAL_DCACHE_LINE_SIZE            16                          // Size of a data cache line
#define NOS_HAL_DCACHE_WAYS                 2                           // Associativity of the cache

#define NOS_HAL_MIPS_CACHE_HIT_WRITEBACK_INV_D   0x15 /* 5 1 */

#define NOS_HAL_DCACHE_START_ADDRESS(_addr_) \
(((U32)(_addr_)) & ~(NOS_HAL_DCACHE_LINE_SIZE-1))
#define NOS_HAL_DCACHE_END_ADDRESS(_addr_, _asize_) \
((U32)((_addr_) + (_asize_)))
#define NOS_HAL_DCACHE_IS_ENABLED(_state_) (_state_) = 1;
#define _nos_hal_asm_mips_cpp_stringize( _x_ ) #_x_
#define _NOS_HAL_ASM_SET_MIPS_ISA( _isal_ ) asm volatile ( \
      ".set mips" _nos_hal_asm_mips_cpp_stringize(_isal_) )
#define _NOS_DWAY(_n_) ((_n_)*NOS_HAL_DCACHE_SIZE/NOS_HAL_DCACHE_WAYS)
#define _NOS_HAL_ASM_DCACHE_ALL_WAYS( _cmd_ , _addr_ )      \
    asm volatile ("cache %0,0(%1);"                     \
                  "cache %0,%2(%1);"                    \
                    : : "I" ((_cmd_) | 1), "r"(_addr_), \
                        "I" (_NOS_DWAY(1)))

extern void   MAsm_CPU_Sync(void);

void nos_hal_dcache_flush( U32 base , U32 asize )
{
    register U32 addr  = NOS_HAL_DCACHE_START_ADDRESS(base);
    register U32 eaddr = NOS_HAL_DCACHE_END_ADDRESS(base, asize);
    register U32 state;
    NOS_HAL_DCACHE_IS_ENABLED( state );
    if( state ) {
        _NOS_HAL_ASM_SET_MIPS_ISA(3);
        for( ; addr < eaddr; addr += NOS_HAL_DCACHE_LINE_SIZE )
        { _NOS_HAL_ASM_DCACHE_ALL_WAYS(NOS_HAL_MIPS_CACHE_HIT_WRITEBACK_INV_D, addr); }
        _NOS_HAL_ASM_SET_MIPS_ISA(0);
    }
}
void mhal_dcache_flush( U32 u32Base, U32 u32Size )
{
    U32 u32Addr;

    u32Addr = u32Base & 0xFFFFFFF0;
    u32Size+= (u32Base - u32Addr);
    u32Size = ALIGN_16(u32Size);

    //U32 u32OldIntr;
    if ( (u32Addr % NOS_HAL_DCACHE_LINE_SIZE) || ( (u32Addr & (U32)KSEG1_BASE) != (U32)KSEG0_BASE) )
    {
        return;
    }
    //u32OldIntr = MsOS_DisableAllInterrupts();
    nos_hal_dcache_flush( u32Addr, u32Size);
    //flush EC's write FIFO
    MAsm_CPU_Sync();
    //MsOS_RestoreAllInterrupts(u32OldIntr);
    //return TRUE;
}