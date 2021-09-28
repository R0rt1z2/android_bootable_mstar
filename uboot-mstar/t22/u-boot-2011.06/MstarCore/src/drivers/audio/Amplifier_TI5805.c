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
 //wjq20100112>>>
#include <common.h>
#include <audio/Amplifier_TI5805.h>
#include <linux/types.h>
//#define code

#ifndef I2C_BUS_IDX
#define I2C_BUS_IDX         0
#endif

#define TAS5805_ADDR     0x58
#define TAS5805_BUSID     (((MS_U16) I2C_BUS_IDX << 8) | TAS5805_ADDR)

MAPI_U8  g_ucTas5805RegDate[20];
 MAPI_U8  g_ucTas5805RegDateread[20];

MAPI_U8  g_ucTas5805RegAddr;

#define AmpInitRegListNum  20

MAPI_U8 AmpInitRegList[]=
{
    1    ,0x00,0x00,
    1    ,0x7f,0x00,
    1    ,0x03,0x03,
    1    ,0x78,0x80,
    0x00,
};



MAPI_BOOL Amplifier_TI5805_Init(void)
{

    MAPI_U8 * Pstr=NULL;
    MAPI_U8 DataLength = 0;
    printf("*****Amplifier_Init****\n");
    Adj_Volume_On();
    mdelay(20);

    Pstr = AmpInitRegList;
    do
    {
        DataLength = *Pstr;
        if(DataLength > AmpInitRegListNum)
        {
            printf("** TI5805 set command error!!\n");
            break;
        }
	     printf(" TI5805 mboot write\n");
        g_ucTas5805RegAddr = *(++Pstr);
        Pstr++;
        if (MApi_SWI2C_WriteBytes(TAS5805_BUSID,1,&g_ucTas5805RegAddr,DataLength, Pstr) == FALSE)
        {
            printf("************ TI5808 write fail = %d\n",g_ucTas5805RegAddr);
        }
        mdelay(2);

        Pstr = Pstr + DataLength;
    }while(*Pstr != 0);


    return MAPI_TRUE;
}
