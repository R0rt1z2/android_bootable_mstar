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
#ifndef __USBCOMMON_H
#define __USBCOMMON_H

#include "asmCPU.h" // ASSERT debug

typedef void* (*mem_Alloc)(int size);
typedef void (*mem_Free)(void* pBuf);
typedef MS_U32 (*mem_VA2PA)(MS_U32 addr);
typedef MS_U32 (*mem_PA2VA)(MS_U32 addr, int bCached);
typedef void* (*mem_Cached2Noncached)(MS_U32 addr);
typedef void* (*mem_NonCached2Cached)(MS_U32 addr);


extern mem_Alloc    pfnAllocCachedMem, pfnAllocNoncachedMem;
extern mem_Free     pfnFreeCachedMem, pfnFreeNoncachedMem;
extern mem_VA2PA    pfnVA2PA;
extern mem_PA2VA    pfnPA2VA;
extern mem_Cached2Noncached     pfnCached2Noncached;
extern mem_NonCached2Cached     pfnNoncached2Cached;

#if 1 // eCos lib
#define Usb_AllocateNonCachedMemory(x)  pfnAllocNoncachedMem(x)
#define Usb_FreeNonCachedMemory(x)      pfnFreeNoncachedMem(x)
#define Usb_AllocateCachedMemory(x)     pfnAllocCachedMem(x)
#define Usb_FreeCachedMemory(x)         pfnFreeCachedMem(x)


//extern void* (*kmalloc)(int size, int flag); 
//extern void  (*kfree)(void* pBuf); 
#define kmalloc(size, flag)             pfnAllocCachedMem(size)
#define kfree(pBuf)                     pfnFreeCachedMem(pBuf)

#define USB_VA2PA(addr)                 pfnVA2PA(addr)
#else
#define Usb_AllocateNonCachedMemory(x)  kmalloc(x, 0)
#define Usb_FreeNonCachedMemory(x)      kfree(x)
#define Usb_AllocateCachedMemory(x)     kmalloc(x, 0)
#define Usb_FreeCachedMemory(x)         kfree(x)
#endif

#if 1 // eCos lib
#define KSEG02KSEG1(addr)       pfnCached2Noncached((MS_U32)addr)  //cached -> unchched, 20111017 for K2
#define KSEG12KSEG0(addr)       pfnNoncached2Cached((MS_U32)addr) //unchched -> cached
#else
#define KSEG02KSEG1(addr)       ((void *)((MS_U32)(addr)|0x20000000))  //cached -> unchched
#define KSEG12KSEG0(addr)       ((void *)((MS_U32)(addr)&~0x20000000)) //unchched -> cached
#endif

#ifndef U32
#define U32 MS_U32
#endif

#ifndef U16
#define U16 MS_U16
#endif

#ifndef U8
#define U8 MS_U8
#endif

#ifndef S32
#define S32 MS_S32
#endif

#ifndef S16
#define S16 MS_S16
#endif

#ifndef S8
#define S8 MS_S8
#endif

#ifndef __u32
#define __u32 MS_U32
#endif

#ifndef __u16
#define __u16 MS_U16
#endif

#ifndef __u8
#define __u8 MS_U8
#endif

#ifndef __s32
#define __s32 MS_S32
#endif

#ifndef __s16
#define __s16 MS_S16
#endif

#ifndef __s8
#define __s8 MS_S8
#endif

#ifndef BOOL
#define BOOL MS_BOOL
#endif

#ifndef BOOLEAN
#define BOOLEAN MS_BOOL
#endif

#ifdef MS_DEBUG
#define ASSERT(_bool_)                                                                                      \
        {                                                                                                   \
            if ( ! ( _bool_ ) )                                                                             \
            {                                                                                               \
                diag_printf("ASSERT FAIL: %s, %s %s %d\n", #_bool_, __FILE__, __PRETTY_FUNCTION__, __LINE__);\
                MAsm_CPU_SwDbgBp();                                                                       \
            }                                                                                               \
        }
#else
#define ASSERT(_bool_) MS_ASSERT(_bool_)
#endif


//Chip ID deifinition
#define CHIPID_NEPTUNE     0x2
#define CHIPID_ERIS        0x3
#define CHIPID_TITANIA     0x4
#define CHIPID_PLUTO       0x5
#define CHIPID_TRITON      0x6
#define CHIPID_TITANIA2    0xB
#define CHIPID_TITANIA3    0xF
#define CHIPID_EUCLID      0x15
#define CHIPID_TITANIA4    0x18
#define CHIPID_URANUS4     0x1B
#define CHIPID_TITANIA7    0x1C
#define CHIPID_JANUS       0x1D
#define CHIPID_TITANIA8    0x1F
#define CHIPID_TITANIA9    0x23
#define CHIPID_KRONUS      0x2F
#define CHIPID_KAISERIN    0x41
#define CHIPID_KAPPA       0x75
#define CHIPID_KELTIC      0x72
#define CHIPID_KENYA       0x7C
#define CHIPID_KRITI       0x87
#define CHIPID_KAISER      0x56
#define CHIPID_KERES       0x7E
#define CHIPID_KIRIN       0x98
#define CHIPID_KRIS        0x8B
#define CHIPID_KRATOS      0x9F
#define CHIPID_KIWI        0xA7
#define CHIPID_CLIPPERS    0x7F
#define CHIPID_KAYLA       0xB1
#define CHIPID_KANO        0x92
#define CHIPID_K6          0xA8
#define CHIPID_CURRY       0xB2
#define CHIPID_K6LITE      0xBE
#define CHIPID_K5TN        0xC1
#define CHIPID_K7U         0xCB
#define CHIPID_C2P         0xD4
#define CHIPID_K1C         0xDB
#define CHIPID_K5AP        0xD0


#if defined(CHIP_U4)
    #define USB_LIB_CHIPID        CHIPID_URANUS4
#elif defined(CHIP_K1)
    #define USB_LIB_CHIPID        CHIPID_KRONUS
#elif defined(CHIP_K2)
    #define USB_LIB_CHIPID        CHIPID_KAISERIN
#elif defined(CHIP_KAPPA)
    #define USB_LIB_CHIPID        CHIPID_KAPPA
#elif defined(CHIP_KELTIC)
    #define USB_LIB_CHIPID        CHIPID_KELTIC
#elif defined(CHIP_KENYA)
    #define USB_LIB_CHIPID        CHIPID_KENYA
#elif defined(CHIP_KRITI)
    #define USB_LIB_CHIPID        CHIPID_KRITI
#elif defined(CHIP_KRATOS)
    #define USB_LIB_CHIPID        CHIPID_KRATOS
#elif defined(CHIP_KAISER)
    #define USB_LIB_CHIPID        CHIPID_KAISER
#elif defined(CHIP_KERES)
    #define USB_LIB_CHIPID        CHIPID_KERES
#elif defined(CHIP_KIRIN)
    #define USB_LIB_CHIPID        CHIPID_KIRIN
#elif defined(CHIP_KRIS)
    #define USB_LIB_CHIPID        CHIPID_KRIS
#elif defined(CHIP_KIWI)
    #define USB_LIB_CHIPID        CHIPID_KIWI
#elif defined(CHIP_CLIPPERS)
    #define USB_LIB_CHIPID        CHIPID_CLIPPERS
#elif defined(CHIP_KAYLA)
    #define USB_LIB_CHIPID        CHIPID_KAYLA
#elif defined(CHIP_KANO)
    #define USB_LIB_CHIPID        CHIPID_KANO
#elif defined(CHIP_K6)
    #define USB_LIB_CHIPID        CHIPID_K6
#elif defined(CHIP_CURRY)
    #define USB_LIB_CHIPID        CHIPID_CURRY
#elif defined(CHIP_K6LITE)
    #define USB_LIB_CHIPID        CHIPID_K6LITE
#elif defined(CHIP_K5TN)
    #define USB_LIB_CHIPID        CHIPID_K5TN
#elif defined(CHIP_K7U)
    #define USB_LIB_CHIPID        CHIPID_K7U
#elif defined(CHIP_C2P)
    #define USB_LIB_CHIPID        CHIPID_C2P
#elif defined(CHIP_K1C)
    #define USB_LIB_CHIPID        CHIPID_K1C
#elif defined(CHIP_K5AP)
    #define USB_LIB_CHIPID        CHIPID_K5AP
#else
    #error No USB Driver Supported
#endif

#endif
