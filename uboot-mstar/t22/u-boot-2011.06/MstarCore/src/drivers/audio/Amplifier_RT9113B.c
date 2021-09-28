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
#include <audio/Amplifier_RT9113B.h>
#include <linux/types.h>
#include <apiSWI2C.h>
#include <MsUboot.h>
#include <drvIIC.h>
#if defined (CONFIG_ONESN_ENABLE)
#include <MsHalAudio_onesn.h>
#endif

#define RT9113B_ADDR        0x36

#ifndef I2C_BUS_IDX
#define I2C_BUS_IDX         0
#endif

#define RT9113B_BUSID     (((MS_U16) I2C_BUS_IDX << 8) | RT9113B_ADDR)
#define  MAX_DATA_LENGTH   20

MAPI_U8  g_ucRT9113BRegAddr;


/****************************
        amp init regs
****************************/
MAPI_U8 AmpReadTbl[] =
{
    //DataLength        Address     DataN       DataN+1...
    1, 0x0a, 0xff,
    0x00,
};

MAPI_U8 AmpInitTblRa188b[] =
{
    1, 0x80, 0x80,
    1, 0x12, 0xD9,
    1, 0x13, 0x06,
    1, 0x84, 0x90,
    1, 0x16, 0x06,
    1, 0x18, 0x13,
    1, 0x81, 0x14,
    1, 0x19, 0x56,     // SW Volume default +16 dB
    1, 0x13, 0x02,
    1, 0x05, 0x02,
    1, 0x74, 0x00,
    1, 0x73, 0x8c,
    1, 0x73, 0x8c,
    1, 0x15, 0x44,
    2, 0x07, 0x01, 0x10,
    0x00,
};

MAPI_U8 AmpInitTblTka188e[] =
{
    //DataLength        Address     DataN       DataN+1...
    1, 0x05, 0x40,
    1, 0x80, 0x80,
    1, 0x11, 0xA9,
    1, 0x12, 0xD8,
    1, 0x13, 0x02,
    1, 0x15, 0x04,
    1, 0x16, 0x05,
    1, 0x18, 0x12,
    1, 0x81, 0x14,
    1, 0x82, 0x39,
    1, 0x05, 0x02,
    1, 0x15, 0xC4,
    1, 0x19, 0x56,
    1, 0x07, 0x10,     // SW Volume default +16 dB
    4, 0x57, 0x00, 0x01, 0xCC, 0x00,
    1, 0x03, 0xB0,
    4, 0x21, 0x3F, 0x7F, 0x00, 0xA9,
    4, 0x22, 0x07, 0xE5, 0x01, 0xFF,
    1, 0x70, 0xF1,
    4, 0x45, 0x00, 0x08, 0x5A, 0x8B,
    8, 0x3A, 0x00, 0x18, 0x12, 0x78, 0x00, 0x7F, 0xFF, 0x51,
    8, 0x3D, 0x00, 0x70, 0x0F, 0xE8, 0x00, 0x7F, 0xF9, 0x2C,
    8, 0x47, 0x00, 0x70, 0x0F, 0xE8, 0x00, 0x7F, 0xE0, 0x69,
    8, 0xAC, 0x00, 0x70, 0x0F, 0xE8, 0x00, 0x7F, 0xFF, 0x3E,
    0x00,
};

MAPI_U8 AmpmasterSWMute[] =
{
    1, 0x06, 0x03,
    0x00,
};

MAPI_U8 AmpmasterSWUnMute[] =
{
    1, 0x06, 0x00,
    0x00,
};

MAPI_BOOL Amplifier_RT9113_Init(void)
{
    MAPI_U8 * Pstr=NULL;
    MAPI_U8 DataLength = 0;
    printf("*****Amplifier_Init****\n");
#if defined (CONFIG_ONESN_ENABLE)
    OneSN_Audio_Amplifier_ON();
    mdelay(2);
    OneSN_Adj_Volume_On();
#else
    Audio_Amplifier_ON();
    mdelay(2);
    Adj_Volume_On();
#endif
    mdelay(40);

    Pstr = AmpReadTbl;
    DataLength = *Pstr;
    g_ucRT9113BRegAddr = *(++Pstr);
    Pstr++;
    if(MApi_SWI2C_ReadBytes(RT9113B_BUSID, 1, &g_ucRT9113BRegAddr, DataLength, Pstr) != TRUE)
    {
        printf("MApi_SWI2C_ReadBytes FAIL!!\n");
        return MAPI_FALSE;
    }
    printf("==========RT9113B chip id is:%d==========\n", AmpReadTbl[2]);

    if(AmpReadTbl[2] == 0x00)       //chip revision ID(0x00-TKA188E,0xff-RA188B)
    {
        Pstr = AmpInitTblTka188e;
    }
    else
    {
        Pstr = AmpInitTblRa188b;
    }

    do
    {
        DataLength = *Pstr;
        if(DataLength>MAX_DATA_LENGTH)
        {
            printf("** RT9113 set command error!!\n");
            break;
        }
        g_ucRT9113BRegAddr = *(++Pstr);
        Pstr++;
        if (MApi_SWI2C_WriteBytes(RT9113B_BUSID,1,&g_ucRT9113BRegAddr,DataLength, Pstr) == FALSE)
        {
            printf("** RT9113 write fail = %d\n",g_ucRT9113BRegAddr);
        }
        mdelay(2);

        Pstr = Pstr + DataLength;
    }while(*Pstr != 0);

    return MAPI_TRUE;
}


MAPI_BOOL Amplifier_RT9113_SWMute(MAPI_BOOL bMute)
{
    MAPI_U8 * Pstr;
    MAPI_U8 DataLength = 0;

    printf("******Amplifier_RT9113_SWMute(%d)\n",bMute);
    if(bMute)
    {
        Pstr=AmpmasterSWMute;
        do
        {
            DataLength = *Pstr;
            if(DataLength>20)
            {
                printf("9113 set command error!!\n");
                break;
            }
            g_ucRT9113BRegAddr = *(++Pstr);
            Pstr++;

            if (MApi_SWI2C_WriteBytes(RT9113B_BUSID,1, &g_ucRT9113BRegAddr, DataLength, Pstr) == FALSE)
            {
                printf("9113write fail = %d\n",g_ucRT9113BRegAddr);
            }
            mdelay(2);
            Pstr = Pstr + DataLength;
        }while(*Pstr != 0);
    }
    else
    {

        Pstr=AmpmasterSWUnMute;
        do
        {
            DataLength = *Pstr;
            if(DataLength>20)
            {
                printf("9113 set command error!!\n");
                break;
            }
            g_ucRT9113BRegAddr = *(++Pstr);
            Pstr++;

            if (MApi_SWI2C_WriteBytes(RT9113B_BUSID,1, &g_ucRT9113BRegAddr, DataLength, Pstr) == FALSE)
            {
                printf("9113write fail = %d\n",g_ucRT9113BRegAddr);
            }
            mdelay(2);

            Pstr = Pstr + DataLength;
        }while(*Pstr != 0);
    }
    return TRUE;
}

MAPI_BOOL Amplifier_RT9113_Mute(MAPI_BOOL bMute)
{
    // TODO: HW no use
    return TRUE;
}


