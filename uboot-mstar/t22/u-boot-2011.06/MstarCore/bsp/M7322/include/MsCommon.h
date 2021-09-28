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


///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @brief  MStar Common Interface Header File
/// @note   MsCommon.h includes most command header files including basic data type, macro(MsTypes.h),\n
/// board configuration(MsBoard.h), and OS related API(MsOS.h).\n
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _MS_COMMON_H_
#define _MS_COMMON_H_

#ifndef MSOS_TYPE_LINUX_KERNEL
#include <stdio.h>
#include <stdlib.h>
#else
#endif

#ifdef MSOS_TYPE_ECOS
#include <cyg/infra/diag.h>
#endif

#include <stdarg.h>

#ifdef CONFIG_ENABLE_MENUCONFIG
    #include "autoconf.h"
#endif

#ifdef MSOS_TYPE_LINUX
    #include <limits.h>
#elif defined(MSOS_TYPE_ECOS)
    #include <limits.h>
#elif defined(MSOS_TYPE_OPTEE)
    #include <limits.h>
#else
    #ifndef MSOS_TYPE_LINUX_KERNEL
	#ifndef MSOS_TYPE_OPTEE
		#define ULONG_MAX 0xffffffff
    #else
		//#define ULONG_MAX 0xffffffff
    #endif
    #endif
#endif

//#include <setjmp.h>

#include "MsOS.h"                                                       // Plan to be obsoleted in next generation.
#include "MsTypes.h"
#include "MsIRQ.h"
//#include "MsVersion.h"
#include "MsDevice.h"

#ifdef CONFIG_ENABLE_MENUCONFIG
    #include "autoconf.h"
#endif
//-------------------------------------------------------------------------------------------------
// Defines
//-------------------------------------------------------------------------------------------------
#ifndef DLL_PUBLIC
#define DLL_PUBLIC __attribute__ ((visibility ("default")))
#endif
#ifndef DLL_LOCAL
#define DLL_LOCAL __attribute__ ((visibility ("hidden")))
#endif

#ifndef MSOS_TYPE_LINUX_KERNEL
#define SYMBOL_WEAK __attribute__((weak))
#else
#define SYMBOL_WEAK
#endif

#if defined (__GNUC__)
    #define UNUSED_PARA(x) UNUSED_ ## x __attribute__ ((__unused__))
#else
    #define UNUSED_PARA(x) x
#endif

#ifdef __GNUC__
    #define UNUSED_FUNCTION(x) __attribute__((__unused__)) UNUSED_ ## x
#else
    #define UNUSED_FUNCTION(x) UNUSED_ ## x
#endif


//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------
#define GEN_EXCEP   { while(1); }

//#define REG(addr) (*(volatile U32 *)(addr))
#if 0
#ifdef __aeon__

//-------------------------------------------------------------------------------------------------
//  In order to keep the compatiblity of the source code from Venus,
//  keep these memory address translation for a while.
//  They will be removed in the future.
//-------------------------------------------------------------------------------------------------
#define CACHED_BASE            ((void *)0x00000000)
#define UNCACHED_BASE       ((void *)0x80000000)

#define CACHED_SIZE         ((void *)0x20000000)
#define UNCACHED_SIZE        ((void *)0x20000000)
//  0xA0000000~0xA000FFFF belongs to RIU
//  0xA1000000~           belongs to SPI

//cached/unchched segment
#define KSEG0_BASE        CACHED_BASE
#define KSEG1_BASE        UNCACHED_BASE
#define KSEG0_SIZE        CACHED_SIZE
#define KSEG1_SIZE        UNCACHED_SIZE

//cached <-> uncached
#define KSEG02KSEG1(addr)  ((void *)((U32)(addr)|0x80000000))
#define KSEG12KSEG0(addr)  ((void *)((U32)(addr)&~0x80000000))

//virtual <-> physical
#define VA2PA(addr)         ((void *)(((U32)addr) & 0x1fffffff))
#define PA2KSEG0(addr)         ((void *)(((U32)addr) | 0x00000000))
#define PA2KSEG1(addr)         ((void *)(((U32)addr) | 0x80000000))
#endif

#if defined(__mips__)

//cached/unchched segment
#define KSEG0_BASE                ((void *)0x80000000)
#define KSEG1_BASE                ((void *)0xa0000000)
#define KSEG0_SIZE                0x20000000
#define KSEG1_SIZE                0x20000000

//cached addr <-> unchched addr
#define KSEG02KSEG1(addr)       ((void *)((MS_U32)(addr)|0x20000000))  //cached -> unchched
#define KSEG12KSEG0(addr)       ((void *)((MS_U32)(addr)&~0x20000000)) //unchched -> cached

//virtual addr <-> physical addr
#define VA2PA(addr)             ((void *)(((MS_U32)addr) & 0x1fffffff)) //virtual -> physical
#define PA2KSEG0(addr)             ((void *)(((MS_U32)addr) | 0x80000000)) //physical -> cached
#define PA2KSEG1(addr)             ((void *)(((MS_U32)addr) | 0xa0000000)) //physical -> unchched
#endif
#endif

#if 0
//cached/unchched segment
#define KSEG0_BASE        ((void *)0x80000000)
#define KSEG1_BASE        ((void *)0xa0000000)
#define KSEG0_SIZE        0x20000000
#define KSEG1_SIZE        0x20000000

//cached <-> unchched
#define KSEG02KSEG1(addr)  ((void *)((U32)(addr)|0x20000000))
#define KSEG12KSEG0(addr)  ((void *)((U32)(addr)&~0x20000000))

//virtual <-> physical
#define VA2PA(addr)         ((void *)(((U32)addr) & 0x1fffffff))
#define PA2KSEG0(addr)         ((void *)(((U32)addr) | 0x80000000))
#define PA2KSEG1(addr)         ((void *)(((U32)addr) | 0xa0000000))
#endif

//user-defined warning
#ifdef MSOS_TYPE_ECOS
    #ifdef BLOCK_ASSERT
        #define MS_ASSERT(_bool_)                                                                                   		\
                {                                                                                                   		\
                    if ( ! ( _bool_ ) )                                                                             		\
                    {                                                                                               		\
                       diag_printf("\033[35m UTOPIA ASSERT: %s, %s %s %d\033[35m\n", #_bool_, __FILE__, __PRETTY_FUNCTION__, __LINE__);   	\
                        *(volatile unsigned int *)(ULONG_MAX) = 0;                                                         \
                    }                                                                                               		\
                }
    #else
        #define MS_ASSERT(_bool_)                                                                                   		\
                {                                                                                                   		\
                    if ( ! ( _bool_ ) )                                                                             		\
                    {                                                                                               		\
                       diag_printf("\033[35m UTOPIA ASSERT: %s, %s %s %d\033[35m\n", #_bool_, __FILE__, __PRETTY_FUNCTION__, __LINE__);   	\
                    }                                                                                               		\
                }
    #endif
#else
#ifdef MSOS_TYPE_LINUX_KERNEL
    #ifdef BLOCK_ASSERT
        #define MS_ASSERT(_bool_)                                                                                   \
                {                                                                                                   \
                    if (!(_bool_))                                                                                  \
                    {                                                                                               \
                        printk("\033[35m ASSERT FAIL: %s, %s %s %d\033[35m\n", #_bool_, __FILE__, __PRETTY_FUNCTION__, __LINE__);    \
                        panic("mstar panic\n");                                                                     \
                        *(volatile unsigned int *)(ULONG_MAX) = 0;                                                         \
                    }                                                                                               \
                }
    #else
        #define MS_ASSERT(_bool_)                                                                                   \
                {                                                                                                   \
                    if (!(_bool_))                                                                                  \
                    {                                                                                               \
                        printk("\033[35m ASSERT FAIL: %s, %s %s %d\033[35m\n", #_bool_, __FILE__, __PRETTY_FUNCTION__, __LINE__);    \
                        panic("mstar panic\n");                                                                     \
                    }                                                                                               \
                }
    #endif
#else
    #ifdef BLOCK_ASSERT
        #define MS_ASSERT(_bool_)                                                                                   	\
                {                                                                                                   	\
                    if ( ! ( _bool_ ) )                                                                             	\
                    {                                                                                               	\
                        printf("\033[35m UTOPIA ASSERT: %s, %s %s %d\033[35m\n", #_bool_, __FILE__, __PRETTY_FUNCTION__, __LINE__);   	\
                        *(volatile unsigned int *)(ULONG_MAX) = 0;                                                         \
                    }                                                                                               	\
                }
    #else
        #define MS_ASSERT(_bool_)                                                                                   	\
                {                                                                                                   	\
                    if ( ! ( _bool_ ) )                                                                             	\
                    {                                                                                               	\
                        printf("\033[35m UTOPIA ASSERT: %s, %s %s %d\033[35m\n", #_bool_, __FILE__, __PRETTY_FUNCTION__, __LINE__);   	\
                    }                                                                                               	\
                }
    #endif
#endif
#endif

//user-defined panic
//access null pointer 0xFFFFFFFF to make exception
#ifdef MSOS_TYPE_ECOS
#define MS_PANIC(_bool_)                                                                                   	    \
        {                                                                                                   		\
            if ( ! ( _bool_ ) )                                                                             		\
            {                                                                                               		\
                diag_printf("\033[35m UTOPIA ASSERT: %s %s %s %d\033[35m\n", #_bool_, __FILE__, __PRETTY_FUNCTION__, __LINE__);   	\
                *(volatile unsigned int *)(ULONG_MAX) = 0;                                                         \
            }                                                                                               		\
        }
#elif defined(MSOS_TYPE_OPTEE)
#define MS_PANIC(_bool_)                                                                                   	\
        {                                                                                                   	\
            if ( ! ( _bool_ ) )                                                                             	\
            {                                                                                               	\
                printf("\033[35m UTOPIA ASSERT: %s %s %s %d\033[35m\n", #_bool_, __FILE__, __PRETTY_FUNCTION__, __LINE__);   	\
                while(1);                                                                                   	\
            }                                                                                               	\
        }
#else
#define MS_PANIC(_bool_)                                                                                   	\
        {                                                                                                   	\
            if ( ! ( _bool_ ) )                                                                             	\
            {                                                                                               	\
                printf("\033[35m UTOPIA ASSERT: %s %s %s %d\033[35m\n", #_bool_, __FILE__, __PRETTY_FUNCTION__, __LINE__);   	\
                *(volatile unsigned int *)(ULONG_MAX) = 0;                                                         \
            }                                                                                               	\
        }
#endif


#ifndef MIN
#define MIN(_a_, _b_)               ((_a_) < (_b_) ? (_a_) : (_b_))
#endif
#ifndef MAX
#define MAX(_a_, _b_)               ((_a_) > (_b_) ? (_a_) : (_b_))
#endif


#define ALIGN_4(_x_)                (((_x_) + 3) & ~3)
#define ALIGN_8(_x_)                (((_x_) + 7) & ~7)
#define ALIGN_16(_x_)               (((_x_) + 15) & ~15)           // No data type specified, optimized by complier
#define ALIGN_32(_x_)               (((_x_) + 31) & ~31)           // No data type specified, optimized by complier

#define MASK(x)     (((1<<(x##_BITS))-1) << x##_SHIFT)
#ifndef BIT	//for Linux_kernel type, BIT redefined in <linux/bitops.h>
#define BIT(_bit_)                  (1 << (_bit_))
#endif
#define BIT_(x)                     BIT(x) //[OBSOLETED] //TODO: remove it later
#define BITS(_bits_, _val_)         ((BIT(((1)?_bits_)+1)-BIT(((0)?_bits_))) & (_val_<<((0)?_bits_)))
#define BMASK(_bits_)               (BIT(((1)?_bits_)+1)-BIT(((0)?_bits_)))
#define BIT_64(_bit_)                (1ULL << (_bit_))


#define READ_BYTE(_reg)             (*(volatile MS_U8*)(_reg))
#define READ_WORD(_reg)             (*(volatile MS_U16*)(_reg))
#define READ_LONG(_reg)             (*(volatile MS_U32*)(_reg))
#define WRITE_BYTE(_reg, _val)      { (*((volatile MS_U8*)(_reg))) = (MS_U8)(_val); }
#define WRITE_WORD(_reg, _val)      { (*((volatile MS_U16*)(_reg))) = (MS_U16)(_val); }
#define WRITE_LONG(_reg, _val)      { (*((volatile MS_U32*)(_reg))) = (MS_U32)(_val); }


#endif // _MS_COMMON_H_
