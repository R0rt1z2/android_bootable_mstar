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
/// @brief  MStar OS Wrapper
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __HB_COMMON_H__
#define __HB_COMMON_H__

// #ifdef MS_DEBUG
#if 0

extern void MAsm_CPU_SwDbgBp(void);

#define HB_ASSERT(_bool_)                                                                                       \
{                                                                                                               \
    if ( ! ( _bool_ ) )                                                                                         \
    {                                                                                                           \
        printf("[%s][%d] HB_ASSERT FAIL\n", __FUNCTION__, __LINE__);                                            \
        MAsm_CPU_SwDbgBp();                                                                                     \
    }                                                                                                           \
}

#define HB_printf(...)                                                                                          \
({                                                                                                              \
    if (MsOS_In_Interrupt())                                                                                    \
    {                                                                                                           \
        printf("[%s][%d] printf in ISR\n", __FUNCTION__, __LINE__);                                             \
        MAsm_CPU_SwDbgBp();                                                                                     \
    }                                                                                                           \
    printf(__VA_ARGS__);                                                                                        \
})

#define HB_AllocateMemory(u32Size, s32PoolId)                                                                   \
({                                                                                                              \
    void* temp = NULL;                                                                                          \
    if (MsOS_In_Interrupt())                                                                                    \
    {                                                                                                           \
        printf("[%s][%d] allocate in ISR\n", __FUNCTION__, __LINE__);                                           \
        HB_ASSERT(0);                                                                                           \
    }                                                                                                           \
    temp = MsOS_AllocateMemory ((u32Size), (s32PoolId));                                                        \
    printf("[%s][%d] allocate 0x%08x 0x%08x %d\n", __FUNCTION__, __LINE__, (s32PoolId), (int)temp, (u32Size));  \
    (temp);                                                                                                     \
})


#define HB_FreeMemory(pAddress, s32PoolId)                                                                      \
({                                                                                                              \
    MS_BOOL ret;                                                                                                \
    if (MsOS_In_Interrupt())                                                                                    \
    {                                                                                                           \
        printf("[%s][%d] free in ISR\n", __FUNCTION__, __LINE__);                                               \
        HB_ASSERT(0);                                                                                           \
    }                                                                                                           \
    ret = MsOS_FreeMemory ((pAddress), (s32PoolId));                                                            \
    printf("[%s][%d] free 0x%08x 0x%08x\n", __FUNCTION__, __LINE__, (s32PoolId), (int)(pAddress));              \
    (ret);                                                                                                      \
})

#define HB_ObtainMutex(id, time)                                                                                \
({                                                                                                              \
    MS_BOOL ret;                                                                                                \
    if ((id & 0xFFFF0000)!= 0x76540000)                                                                         \
    {                                                                                                           \
        printf("[%s][%d] invalid mutex id 0x%08x\n", __FUNCTION__, __LINE__, id);                               \
        MAsm_CPU_SwDbgBp();                                                                                     \
    }                                                                                                           \
    if (MsOS_In_Interrupt())                                                                                    \
    {                                                                                                           \
        printf("[%s][%d] obtain mutex in ISR\n", __FUNCTION__, __LINE__);                                       \
        MAsm_CPU_SwDbgBp();                                                                                     \
    }                                                                                                           \
    ret = MsOS_ObtainMutex(id, time);                                                                           \
    (ret);                                                                                                      \
})

#define HB_ReleaseMutex(id)                                                                                     \
({                                                                                                              \
    MS_BOOL ret;                                                                                                \
    if ((id & 0xFFFF0000)!= 0x76540000)                                                                         \
    {                                                                                                           \
        printf("[%s][%d] invalid mutex id 0x%08x\n", __FUNCTION__, __LINE__, id);                               \
        MAsm_CPU_SwDbgBp();                                                                                     \
    }                                                                                                           \
    if (MsOS_In_Interrupt())                                                                                    \
    {                                                                                                           \
        printf("[%s][%d] obtain mutex in ISR\n", __FUNCTION__, __LINE__);                                       \
        MAsm_CPU_SwDbgBp();                                                                                     \
    }                                                                                                           \
    ret = MsOS_ReleaseMutex(id);                                                                                \
    (ret);                                                                                                      \
})

#else // #ifdef MS_DEBUG

#define HB_ASSERT                       MS_ASSERT
#define HB_printf                       printf
#if 0
#ifdef MS_DEBUG
#define HB_printf(fmt, args...)         printf("[EPGDB][%06d]  " fmt, __LINE__, ## args)
#else
#define HB_printf(fmt, args...)
#endif
#endif
#define HB_AllocateMemory               MsOS_AllocateMemory
#define HB_FreeMemory                   MsOS_FreeMemory
#define HB_ObtainMutex                  MsOS_ObtainMutex
#define HB_ReleaseMutex                 MsOS_ReleaseMutex

#endif // #ifdef MS_DEBUG

#endif // __MEMORY_DEF_H__
