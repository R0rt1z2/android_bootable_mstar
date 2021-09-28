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

#ifndef DRV_GLOBAL_H
#define DRV_GLOBAL_H

#include "datatype.h"

/********************************************************************************/
/*                           Macro                                              */
/********************************************************************************/

typedef U8 MS_REG_INIT;

#define _RV1(addr, value)   (((addr) >> 8) & 0x3F), (U8)(addr), (U8)(value)
#define _RV2(addr, value)   0x40 + _RV1(addr, value), (U8)((value) >> 8)
#define _RV3(addr, value)   0x40 + _RV2(addr, value), (U8)((value) >> 16)
#define _RV4(addr, value)   0x40 + _RV3(addr, value), (U8)((value) >> 24)


#define _RV32_1(addr, value)    (U8)(((addr) >> 16) & 0xFF),(U8)(((addr) >> 8) & 0xFF), (U8)(addr), (U8)(value)
#define _RV32_2(addr, value)   0x20 + _RV32_1(addr, value), (U8)((value) >> 8)
#define _RV32_3(addr, value)   0x20 + _RV32_2(addr, value), (U8)((value) >> 16)
#define _RV32_4(addr, value)   0x20 + _RV32_3(addr, value), (U8)((value) >> 24)
#define _END_OF_TBL32_         0xFF, 0xFF, 0xFF


#define _RVM1(addr, value, mask) (((addr) >> 8) & 0x3F), (U8)(addr), (U8)(value), (U8)(mask)
#define _RVM2(addr, value, mask) 0x40 + _RVM1(addr, value, mask), (U8)((value) >> 8), (U8)((mask) >> 8)
#define _RVM3(addr, value, mask) 0x40 + _RVM2(addr, value, mask), (U8)((value) >> 16), (U8)((mask) >> 16)
#define _RVM4(addr, value, mask) 0x40 + _RVM3(addr, value, mask), (U8)((value) >> 24), (U8)((mask) >> 24)

#define _END_OF_TBL2_       0xFF, 0xFF

/******************************************************************************/
/*                   Function Prototypes                                      */
/******************************************************************************/

#ifdef DRV_GLOBAL_C
#define INTERFACE
#else
#define INTERFACE extern
#endif

#define MDrv_ReadByte( u32Reg )                                                         \
    (__builtin_constant_p( u32Reg ) ?                                                   \
        (((u32Reg) & 0x01) ? RIU8[(u32Reg) * 2 - 1] : RIU8[(u32Reg) * 2]) :             \
        __MDrv_ReadByte( u32Reg ))

#define MDrv_Read2Byte( u32Reg )                                                        \
    (__builtin_constant_p( u32Reg ) && !((u32Reg) & 0x01) ?                             \
        RIU[u32Reg] : __MDrv_Read2Byte( u32Reg ))

#define MDrv_Read3Byte( u32Reg )    __MDrv_Read3Byte( u32Reg )

#define MDrv_Read4Byte( u32Reg )                                                        \
    ({                                                                                  \
    U32 value;                                                                          \
    if ( __builtin_constant_p( u32Reg ) && !((u32Reg) & 0x01) )                         \
    {                                                                                   \
        value = ((U32)RIU[(u32Reg) + 2] << 16) | RIU[u32Reg];                           \
    }                                                                                   \
    else                                                                                \
    {                                                                                   \
        value = __MDrv_Read4Byte( u32Reg );                                             \
    }                                                                                   \
    value;                                                                              \
    })

#define MDrv_ReadRegBit( u32Reg, u8Mask )                                               \
        (RIU8[(u32Reg) * 2 - ((u32Reg) & 1)] & (u8Mask))

#define MDrv_WriteRegBit( u32Reg, bEnable, u8Mask )                                     \
    MST_MACRO_START                                                                     \
    U32 u32Reg8 = ((u32Reg) * 2) - ((u32Reg) & 1);                                      \
    RIU8[u32Reg8] = (bEnable) ? (RIU8[u32Reg8] |  (u8Mask)) :                           \
                                (RIU8[u32Reg8] & ~(u8Mask));                            \
    MST_MACRO_END

#define MDrv_WriteByte( u32Reg, u8Val )                                                 \
    MST_MACRO_START                                                                     \
    if ( __builtin_constant_p( u32Reg ) )                                               \
    {                                                                                   \
        RIU8[((u32Reg) * 2) - ((u32Reg) & 1)] = u8Val;                                  \
    }                                                                                   \
    else                                                                                \
    {                                                                                   \
        __MDrv_WriteByte( u32Reg, u8Val );                                              \
    }                                                                                   \
    MST_MACRO_END

#define MDrv_Write2Byte( u32Reg, u16Val )                                               \
    MST_MACRO_START                                                                     \
    if ( __builtin_constant_p( u32Reg ) )                                               \
    {                                                                                   \
        if ( ((u32Reg) & 0x01) )                                                        \
        {                                                                               \
            RIU8[((u32Reg) * 2) - 1] = (U8)((u16Val));                                  \
            RIU8[((u32Reg) + 1) * 2] = (U8)((u16Val) >> 8);                             \
        }                                                                               \
        else                                                                            \
        {                                                                               \
            RIU[u32Reg] = u16Val;                                                       \
        }                                                                               \
    }                                                                                   \
    else                                                                                \
    {                                                                                   \
        __MDrv_Write2Byte( u32Reg, u16Val );                                            \
    }                                                                                   \
    MST_MACRO_END

#define MDrv_Write3Byte( u32Reg, u32Val )   __MDrv_Write3Byte( u32Reg, u32Val )

#define MDrv_Write4Byte( u32Reg, u32Val )                                               \
    MST_MACRO_START                                                                     \
    if ( __builtin_constant_p( u32Reg ) && !((u32Reg) & 0x01) )                         \
    {                                                                                   \
        RIU[u32Reg] = (U16)(u32Val);                                                    \
        RIU[(u32Reg) + 2] = (U16)((u32Val) >> 16);                                      \
    }                                                                                   \
    else                                                                                \
    {                                                                                   \
        __MDrv_Write4Byte( u32Reg, u32Val );                                            \
    }                                                                                   \
    MST_MACRO_END

#define MDrv_WriteByteMask( u32Reg, u8Val, u8Msk )                                      \
    MST_MACRO_START                                                                     \
    U32 u32Reg8 = ((u32Reg) * 2) - ((u32Reg) & 1);                                      \
    RIU8[u32Reg8] = (RIU8[u32Reg8] & ~(u8Msk)) | ((u8Val) & (u8Msk));                   \
    MST_MACRO_END

INTERFACE U8   __MDrv_ReadByte( U32 u32Reg );
INTERFACE U16  __MDrv_Read2Byte( U32 u32Reg );
INTERFACE U32  __MDrv_Read3Byte( U32 u32Reg );
INTERFACE U32  __MDrv_Read4Byte( U32 u32Reg );

INTERFACE void __MDrv_WriteByte( U32 u32Reg, U8 u8Val );
INTERFACE void __MDrv_Write2Byte( U32 u32Reg, U16 u16Val );
INTERFACE void __MDrv_Write3Byte( U32 u32Reg, U32 u32Val );
INTERFACE void __MDrv_Write4Byte( U32 u32Reg, U32 u32Val );

INTERFACE void MDrv_WriteRegTbl ( const MS_REG_INIT *pRegTable );
INTERFACE void MDrv_WriteRegTbl_32 ( const MS_REG_INIT *pRegTable );

#define MDrv_WriteRegTbl2( pRegTable )  MDrv_WriteRegTbl( pRegTable )

#undef INTERFACE

#endif /* DRV_GLOBAL_H */
