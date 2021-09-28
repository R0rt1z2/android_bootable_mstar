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

/******************************************************************************/
/*                    Header Files                                            */
/* ****************************************************************************/
#include <common.h>
#include <MsMath.h>
#include <ShareType.h>

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @Functino Name :  rRand
/// @brief         :  This function is implement for get random value.
/// @author        :  MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////
int rRand(unsigned int seed0, unsigned int seed1)
{
    static const unsigned short para_16=0x8000;
    static const unsigned char para_8=0x43;
    static unsigned short valu_16=0x0111;
    static unsigned char valu_8=0x10;   

    unsigned short t_16;
    unsigned char t_8;
    int i,next_8,next_16;

    if(seed0>0)
        valu_16=(seed0 & 0xffff);
    if(seed1>0)
        valu_8=(seed1 & 0xff);     

    t_16=para_16&valu_16;
    t_8=para_8&valu_8;
    next_8=0;
    for(i=0;i<8;++i)
    {
      if(t_8%2)
       ++next_8;
      t_8/=2;
    }
    for(;i<24;++i)
    {
      if(t_16%2)
       ++next_8;
      t_16/=2;
    }
    next_8%=2;
    next_16=valu_8/0x7f;
    valu_8=valu_8-next_16*0x7f;
    valu_8=valu_8*2+next_8;
    valu_16=valu_16-(valu_16/0x7fff)*0x7fff;
    valu_16=valu_16*2+next_16;
    return valu_16*0x7f*2+valu_8;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @Functino Name :  ascii_to_hex
/// @brief         :  This function is implement for ascii_to_hex
/// @author        :  MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////
unsigned short ascii_to_hex(const char *buffer, int length)
{
    unsigned short u16temp;
    U8 tempbuffer[4];
    U8 i = 0;

    while(i<4)
    {
        //printf("%c ", *buffer);
        if((*buffer >= 0x30)&&(*buffer <= 0x39)) // 0 ~ 9
            tempbuffer[i] = *buffer - 0x30;
        else if((*buffer >= 0x41)&&(*buffer <= 0x46)) // A ~ E
            tempbuffer[i] = *buffer - 0x37;
        else if((*buffer >= 0x61)&&(*buffer <= 0x66)) // a ~ e
            tempbuffer[i] = *buffer - 0x57;
        else
            printf("Error> wrong character\n");

        //printf("0x%x \n", tempbuffer[i]);
        i++;
        buffer++;
    }

    u16temp = (U16)(tempbuffer[0]<<12|tempbuffer[1]<<8|tempbuffer[2]<<4|tempbuffer[3]);
    //printf("%04x\n", u16temp);

    return u16temp;
}
