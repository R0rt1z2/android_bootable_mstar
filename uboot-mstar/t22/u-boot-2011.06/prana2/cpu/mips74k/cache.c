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
#define NOS_HAL_DCACHE_WAYS                 2

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

#if (CHIP_FAMILY_TYPE == CHIP_FAMILY_A5 || CHIP_FAMILY_TYPE == CHIP_FAMILY_A5P || CHIP_FAMILY_TYPE == CHIP_FAMILY_A3 || CHIP_FAMILY_TYPE == CHIP_FAMILY_EDISON || CHIP_FAMILY_TYPE == CHIP_FAMILY_EINSTEIN || CHIP_FAMILY_TYPE == CHIP_FAMILY_S8 || CHIP_FAMILY_TYPE==CHIP_FAMILY_J2 || CHIP_FAMILY_TYPE == CHIP_FAMILY_T11 || CHIP_FAMILY_TYPE == CHIP_FAMILY_K2)
#define MIU_INTERVAL    0x20000000 // MIU1 base address= MIU0 max size
#define MIU_DRAM_LEN0   0x20000000 // MIU0 DRAM cell size ??? check with MMAP file
#define MIU_DRAM_LEN1   0x20000000 // MIU1 DRAM cell size ??? check with MMAP file

void L2_Flush_Memory(void)   // write back
{
    U32 u32temp;

    // L2 disable
    if( (*(volatile U32*)(0xbf000000+0x203100)) & BIT1 )  // H40
    {
        return;
    }

    // disbale INT
    mhal_interrupt_lock();

    // flush all and keep valid
    *(volatile U32*)(0xbf000000+0x203104) = 3; // H41 all
    *(volatile U32*)(0xbf000000+0x203104) |= BIT4; // H41
    do
    {
        u32temp = *(volatile U32*)(0xbf000000+0x203104);
        u32temp &= BIT10;
    }while(u32temp == 0);
    *(volatile U32*)(0xbf000000+0x203104) &= BIT4; // H41

    // enable INT
    mhal_interrupt_unlock();
}

inline void* Chip_mphy_noncachevirt( unsigned long mphyaddr )
{
    if( mphyaddr < (MIU_INTERVAL>>1)  )
    {
        return (void*)(mphyaddr|0xA0000000);
    }
    else if( mphyaddr < MIU_INTERVAL  )
    {
        return (void*)((mphyaddr -(MIU_INTERVAL>>1) ) |0xC0000000);
    }
    else if( mphyaddr < (MIU_INTERVAL +  (MIU_INTERVAL>>1)   ) )
    {
        return (void*)((mphyaddr - MIU_INTERVAL) |0xD0000000);
    }
    else
    {
        return (void*)(NULL);
    }
}

void L2_Read_Memory_Range(unsigned long Address , unsigned long  size)
{
    volatile unsigned char* ptr= NULL;
    unsigned long tmp =0;
    unsigned long EndAddr=0, i=0;
    unsigned long CacheLineSize=0;
    signed long s32Increase=0;

    // L2 disabled
    if( (*(volatile unsigned int *)(0xbf000000+0x203100)) & BIT1 )  // H40
    {
        return;
    }

    if(  size >= 0x8000  )
    {
        unsigned long temp =0;

        // disbale INT
        mhal_interrupt_lock();

        // flush all and keep valid
        *(volatile unsigned int *)(0xbf000000+0x203104) = 3; // H41 all
        *(volatile unsigned int *)(0xbf000000+0x203104) |= BIT4; // H41
        *(volatile unsigned int *)(0xbf000000+0x203104) &= ~(BIT4); // H41
        do
        {
            temp = *(volatile unsigned int *)(0xbf000000+0x203104);
            temp &= BIT10;//all
        }while(temp == 0);

        // invalid all
        *(volatile unsigned int *)(0xbf000000+0x203104) = 0; // H41 all
        *(volatile unsigned int *)(0xbf000000+0x203104) |= BIT4; // H41
        *(volatile unsigned int *)(0xbf000000+0x203104) &= ~(BIT4); // H41
        do
        {
            temp = *(volatile unsigned int *)(0xbf000000+0x203104);
            temp &= BIT8;//all
        }while(temp == 0);

        // enable INT
        mhal_interrupt_unlock();

        return;
    }

    CacheLineSize=((*(volatile unsigned int *)(0xbf000000+0x203100))&(BIT4|BIT5))>>4;  // H40 L2 cache line size 00: 32-bytes    01: 64-bytes    1x: 128-bytes

    //printk("Read_Memory_Range: %lx \n"  , CacheLineSize);
    switch(CacheLineSize  )
    {
     case 0:
        CacheLineSize=32;
        break;
     case 1:
        CacheLineSize=64;
        break;
     case 2:
     case 3:
        CacheLineSize=128;
        break;
    default:
        CacheLineSize=32;
        break;
    }

    // check MIU upper boundary
    if( Address  < MIU_INTERVAL   ) // MIU0
    {
        if((Address  + 0xa0000) < MIU_DRAM_LEN0 ) // 128KB *5
        {
            s32Increase=0x20000;
        }
        else
        {
            s32Increase=-0x20000;
        }
    }
    else // MIU1
    {
        if((Address  + 0xa0000) < (MIU_INTERVAL + MIU_DRAM_LEN1 ))
        {
            s32Increase=0x20000;
        }
        else
        {
            s32Increase=-0x20000;
        }
    }
    //printf("Read_Memory_Range: direct: %ld %lu\n" , s32Increase,CacheLineSize );
    for(  i=0 ; i<4;i++ )
    {
        Address += s32Increase;
        //*************************************************
        EndAddr = Address +size;
        if(  size  > 0x8000   ) // 32KB
        {
            EndAddr = Address+0x8000;
        }
        // TODO: miss one almost impossible case: 255.9MB~256.1MB >> diff virtual address
        EndAddr=(unsigned long)Chip_mphy_noncachevirt( EndAddr );
        ptr= (volatile unsigned char*)Chip_mphy_noncachevirt(Address);
        if( (ptr == NULL) || (EndAddr == 0))
        {
            continue;
        }
        //printk("Read_Memory_Range: read start:%lx end:%lx diff:%lx %lx\n" , (U32)(ptr),EndAddr ,EndAddr-Address,(U32)(volatile U8*)((U32)ptr+ CacheLineSize));

        do
        {
            tmp =(unsigned long)( *ptr);
            ptr= (volatile unsigned char*)((unsigned long)ptr+ CacheLineSize);
            //printk("c*ceach: %lx %lx\n" ,(U32)(ptr)   ,(U32)tmp  );
        }while( (unsigned long)(ptr) <  EndAddr);
    }

    return ;
}

void L2_Read_Memory(void)
{
    L2_Read_Memory_Range(0, 0x8000); //32KB(L2 1 way size)
}
#endif

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

    #if (CHIP_FAMILY_TYPE == CHIP_FAMILY_A5 || CHIP_FAMILY_TYPE == CHIP_FAMILY_A5P || CHIP_FAMILY_TYPE == CHIP_FAMILY_A3 || CHIP_FAMILY_TYPE == CHIP_FAMILY_EDISON || CHIP_FAMILY_TYPE == CHIP_FAMILY_S8 || CHIP_FAMILY_TYPE==CHIP_FAMILY_J2 || CHIP_FAMILY_TYPE == CHIP_FAMILY_T11 || CHIP_FAMILY_TYPE == CHIP_FAMILY_K2)
    if((u32Addr % NOS_HAL_DCACHE_LINE_SIZE) || \
       ((u32Addr >= 0x10000000) && (u32Addr < 0x20000000)) || \
       ((u32Addr >= 0x30000000) && (u32Addr < 0x40000000)) || \
       ((u32Addr >= 0xA0000000) && (u32Addr < 0xB0000000)) || \
       ((u32Addr >= 0xD0000000) && (u32Addr < 0xE0000000)))
    {
        return;
    }
    #else
    //U32 u32OldIntr;
    if ( (u32Addr % NOS_HAL_DCACHE_LINE_SIZE) || ( (u32Addr & (U32)KSEG1_BASE) != (U32)KSEG0_BASE) )
    {
        return;
    }
    #endif

    //u32OldIntr = MsOS_DisableAllInterrupts();
    nos_hal_dcache_flush( u32Addr, u32Size);
    //flush EC's write FIFO
    MAsm_CPU_Sync();
    //MsOS_RestoreAllInterrupts(u32OldIntr);

    //return TRUE;
}

#if (CHIP_FAMILY_TYPE == CHIP_FAMILY_A5 || CHIP_FAMILY_TYPE == CHIP_FAMILY_A5P || CHIP_FAMILY_TYPE == CHIP_FAMILY_A3 || CHIP_FAMILY_TYPE == CHIP_FAMILY_EDISON || CHIP_FAMILY_TYPE == CHIP_FAMILY_S8 || CHIP_FAMILY_TYPE==CHIP_FAMILY_J2 || CHIP_FAMILY_TYPE == CHIP_FAMILY_T11 || CHIP_FAMILY_TYPE == CHIP_FAMILY_K2)
void mhal_L2Cache_Flush(void)
{
    L2_Flush_Memory(); // write back
}

void mhal_L2Cache_Read(void)
{
    L2_Read_Memory();  // invalid all
}
#endif



