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

#define RIU_MAP                         0x1F200000
#define LOCAL_BDMA_REG_BASE             0x900
#define RIU                             ((unsigned short volatile *) RIU_MAP)
#define RIU8                            ((unsigned char volatile *) RIU_MAP)
#define __loader_2nd
#define __loader_2nd_bss

typedef unsigned char                   U8;
typedef unsigned int                    U16;
typedef unsigned long                   U32;
#ifndef BOOL
typedef unsigned int                    BOOL;
#endif

void __BDMA_FlashCopy2MIU0(U32 FlashPA, U32 DRAMPA, U32 Len);
void __loader_2nd __BDMA_FlashCopy2MIU0(U32 FlashPA, U32 DRAMPA, U32 Len)
{
    while (RIU8[(LOCAL_BDMA_REG_BASE + 0x02)*2] & 0x01)
    {
        // do nothing
    }
    //set source address
    RIU[(LOCAL_BDMA_REG_BASE + 0x08 )] = (U16)(FlashPA & 0x0000ffff);
    RIU[(LOCAL_BDMA_REG_BASE + 0x0A )] = (U16)(FlashPA >> 16);
    //set destination address
    RIU[(LOCAL_BDMA_REG_BASE + 0x0C )] = (U16)(DRAMPA & 0x0000ffff);
    RIU[(LOCAL_BDMA_REG_BASE + 0x0E )] = (U16)(DRAMPA >> 16);
    // set length
    RIU[(LOCAL_BDMA_REG_BASE + 0x10 )] = (U16)(Len & 0x0000ffff);
    RIU[(LOCAL_BDMA_REG_BASE + 0x12 )] = (U16)(Len >> 16);
    //increasing
    RIU8[(LOCAL_BDMA_REG_BASE + 0x06)*2] = 0x0;   //increasing

    //((DstDataWidth|Dsttype)  <<8) |(SrcDataWidth|srctype)
    RIU[(LOCAL_BDMA_REG_BASE+0x04)] = 0x4045;

    //clear status
    RIU8[(LOCAL_BDMA_REG_BASE+0x02)*2] = RIU8[(LOCAL_BDMA_REG_BASE+0x02)*2] | 0x1c;
    //fire once
    RIU8[ (LOCAL_BDMA_REG_BASE*2)] = 0x01 ;
    while(1)
    {
        if((RIU8[(LOCAL_BDMA_REG_BASE+0x02)*2] & 0x08) == 0x08)
            break;
    }
}

void __loader_2nd __BDMA_MIU0Copy2MIU0(U32 SRCPA, U32 DSTPA, U32 Len)
{
    while (RIU8[(LOCAL_BDMA_REG_BASE + 0x02)*2] & 0x01)
    {
        // do nothing
    }
    //set source address
    RIU[(LOCAL_BDMA_REG_BASE + 0x08 )] = (U16)(SRCPA & 0x0000ffff);
    RIU[(LOCAL_BDMA_REG_BASE + 0x0A )] = (U16)(SRCPA >> 16);

    //set destination address
    RIU[(LOCAL_BDMA_REG_BASE + 0x0C )] = (U16)(DSTPA & 0x0000ffff);
    RIU[(LOCAL_BDMA_REG_BASE + 0x0E )] = (U16)(DSTPA >> 16);

    // set length
    RIU[(LOCAL_BDMA_REG_BASE + 0x10 )] = (U16)(Len & 0x0000ffff);
    RIU[(LOCAL_BDMA_REG_BASE + 0x12 )] = (U16)(Len >> 16);
    //increasing
    RIU8[(LOCAL_BDMA_REG_BASE + 0x06)*2] = 0x0;   //increasing

    //((DstDataWidth|Dsttype)  <<8) |(SrcDataWidth|srctype)
    RIU[(LOCAL_BDMA_REG_BASE+0x04)] = 0x4040;

    //clear status
    RIU8[(LOCAL_BDMA_REG_BASE+0x02)*2] = RIU8[(LOCAL_BDMA_REG_BASE+0x02)*2] | 0x1c;
    //fire once
    RIU8[ (LOCAL_BDMA_REG_BASE*2)] = 0x01 ;
    while(1)
    {
        if((RIU8[(LOCAL_BDMA_REG_BASE+0x02)*2] & 0x08) == 0x08)
            break;
    }
}
