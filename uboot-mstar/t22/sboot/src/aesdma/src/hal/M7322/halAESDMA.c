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

#ifndef _HAL_AESDMA_H_
#include "../../drvAESDMA.h"
#include "halAESDMA.h"
#endif

void HAL_AESDMA_DisableXIUSelectCA9(void)
{
    //disable AESDMA XIU select CA9
    RIU[(AESDMA_BASE_ADDR+(0x5F<<1))]= ((RIU[(AESDMA_BASE_ADDR+(0x5F<<1))])&(~AESDMA_CTRL_XIU_SEL_CA9));
}

void HAL_AESDMA_WB2DMADisable(void)
{
    // WB2DMA read & write disable
    RIU[(DMA_SECURE_BASE_ADDR+(0x79<<1))]= ((RIU[(DMA_SECURE_BASE_ADDR+(0x79<<1))])&(~(DMA_SECURE_CTRL_WB2DMA_R_EN|DMA_SECURE_CTRL_WB2DMA_W_EN)));
}

void HAL_AESDMA_ShaFromOutput(void)
{
    // SHA from AESDMA output(XIU)
    RIU[(AESDMA_BASE_ADDR+(0x5F<<1))]= ((RIU[(AESDMA_BASE_ADDR+(0x5F<<1))])|(AESDMA_CTRL_SHA_FROM_OUT));
}

void HAL_AESDMA_ShaFromInput(void)
{
    // SHA from AESDMA input(XIU)
    RIU[(AESDMA_BASE_ADDR+(0x5F<<1))]= ((RIU[(AESDMA_BASE_ADDR+(0x5F<<1))])|(AESDMA_CTRL_SHA_FROM_IN));
}

void HAL_AESDMA_SetXIULength(U32 u32Size)
{
    // AESDMA XIU length (byte):54~55
    RIU[(AESDMA_BASE_ADDR+(0x54<<1))]= (U16)((0x0000ffff)&(u32Size));
    RIU[(AESDMA_BASE_ADDR+(0x55<<1))]= (U16)(((0xffff0000)&(u32Size))>>16);
}

void HAL_AESDMA_UseHwKey(void)
{
    // AESDMA using HW key
    RIU[(AESDMA_BASE_ADDR+(0x5e<<1))]= ((RIU[(AESDMA_BASE_ADDR+(0x5e<<1))])|AESDMA_CTRL_USE_SECRET_KEY);
}

void HAL_AESDMA_CipherDecrypt(void)
{
    // AESDMA ctrl(decrypt/aes_en)
    RIU[(AESDMA_BASE_ADDR+(0x51<<1))]= ((RIU[(AESDMA_BASE_ADDR+(0x51<<1))])|(AESDMA_CTRL_CIPHER_DECRYPT));
}

void HAL_AESDMA_Disable(void)
{
    // AESDMA ctrl(decrypt/aes_en)
    RIU[(AESDMA_BASE_ADDR+(0x51<<1))]= ((RIU[(AESDMA_BASE_ADDR+(0x51<<1))])&(~AESDMA_CTRL_AES_EN));
}

void HAL_AESDMA_FileOutEnable(U8 u8FileOutEnable)
{
    // AESDMA fout_en
    if(u8FileOutEnable==1)
    {
        RIU[(AESDMA_BASE_ADDR+(0x50<<1))]= ((RIU[(AESDMA_BASE_ADDR+(0x50<<1))])|(AESDMA_CTRL_FOUT_EN));
    }
    else
    {
        RIU[(AESDMA_BASE_ADDR+(0x50<<1))]= ((RIU[(AESDMA_BASE_ADDR+(0x50<<1))])&(~AESDMA_CTRL_FOUT_EN));
    }
}

void HAL_AESDMA_SetFileinAddr(U32 u32addr)
{
    //SHA_SetLength:5c~5d(sha_message_length)
    RIU[(AESDMA_BASE_ADDR+(0x52<<1))]= (U16)((0x0000ffff)&(u32addr));
    RIU[(AESDMA_BASE_ADDR+(0x53<<1))]= (U16)((((0xffff0000)&(u32addr)) - CONFIG_MIU0_BUSADDR)>>16);
}

void HAL_AESDMA_Start(U8 u8AESDMAStart)
{
    // AESDMA file start
    if(u8AESDMAStart==1)
    {
        RIU[(AESDMA_BASE_ADDR+(0x50<<1))]= ((RIU[(AESDMA_BASE_ADDR+(0x50<<1))])|(AESDMA_CTRL_FILE_ST));
        RIU[(AESDMA_BASE_ADDR+(0x50<<1))]= ((RIU[(AESDMA_BASE_ADDR+(0x50<<1))])&(~AESDMA_CTRL_FILE_ST));
    }
    else
    {
        RIU[(AESDMA_BASE_ADDR+(0x50<<1))]= ((RIU[(AESDMA_BASE_ADDR+(0x50<<1))])&(~AESDMA_CTRL_FILE_ST));
    }
}

U16 HAL_AESDMA_GetStatus(void)
{
    return RIU[(AESDMA_BASE_ADDR+(0x7F<<1))];
}

void HAL_RSA_ClearInt(void)
{
    //RSA interrupt clear
    RIU[(RSA_BASE_ADDR+(0x27<<1))]= ((RIU[(RSA_BASE_ADDR+(0x27<<1))])|(RSA_INT_CLR));
}

void HAL_RSA_Reset(void)
{
    //RSA Rst
    RIU[(RSA_BASE_ADDR+(0x28<<1))]= ((RIU[(RSA_BASE_ADDR+(0x28<<1))])|(RSA_CTRL_RSA_RST));
    RIU[(RSA_BASE_ADDR+(0x28<<1))]= ((RIU[(RSA_BASE_ADDR+(0x28<<1))])&(~RSA_CTRL_RSA_RST));
}

void HAL_RSA_Ind32Ctrl(U8 u8dirction)
{
    //[1] reg_ind32_direction 0: Read. 1: Write
    if(u8dirction==1)
    {
        RIU[(RSA_BASE_ADDR+(0x21<<1))]= ((RIU[(RSA_BASE_ADDR+(0x21<<1))])|(RSA_IND32_CTRL_DIRECTION_WRITE));
    }
    else
    {
        RIU[(RSA_BASE_ADDR+(0x21<<1))]= ((RIU[(RSA_BASE_ADDR+(0x21<<1))])&(~RSA_IND32_CTRL_DIRECTION_WRITE));
    }
    //[2] reg_addr_auto_inc : Set 1 to enable address auto-increment after finishing read/write
    RIU[(RSA_BASE_ADDR+(0x21<<1))]= ((RIU[(RSA_BASE_ADDR+(0x21<<1))])|(RSA_IND32_CTRL_ADDR_AUTO_INC));

    //[3] Set 1 to enable access auto-start after writing Data[31:16]
    RIU[(RSA_BASE_ADDR+(0x21<<1))]= ((RIU[(RSA_BASE_ADDR+(0x21<<1))])|(RSA_IND32_CTRL_ACCESS_AUTO_START));


}

void HAL_RSA_LoadSiganature(U32 *ptr_Sign)
{

    U32 i;

    RIU[(RSA_BASE_ADDR+(0x22<<1))]= RSA_A_BASE_ADDR; //RSA A addr
    RIU[(RSA_BASE_ADDR+(0x20<<1))]= ((RIU[(RSA_BASE_ADDR+(0x20<<1))])|(RSA_IND32_START)); //RSA start

    for( i = 0; i < 64; i++ )
    {
        RIU[(RSA_BASE_ADDR+(0x23<<1))]= (U16)(((*(ptr_Sign-i))>>8)&0xFF00)|(((*(ptr_Sign-i))>>24)&0xFF);
        RIU[(RSA_BASE_ADDR+(0x24<<1))]= (U16)(((*(ptr_Sign-i))>>8)&0xFF)|(((*(ptr_Sign-i))<<8)&0xFF00);
    }

}

void HAL_RSA_LoadKeyE(U32 *ptr_E)
{

    U32 i;

    RIU[(RSA_BASE_ADDR+(0x22<<1))]= RSA_E_BASE_ADDR; //RSA E addr
    RIU[(RSA_BASE_ADDR+(0x20<<1))]= ((RIU[(RSA_BASE_ADDR+(0x20<<1))])|(RSA_IND32_START)); //RSA start

    //RIU[(POR_STATUS_BASE_ADDR+(0xA<<1))]=(U16)((0x0000ffff)&(U32)(ptr_E)); //write ptr_E addr to por_status(0x10050A)

    for( i = 0; i < 64; i++ )
    {
        RIU[(RSA_BASE_ADDR+(0x23<<1))]= (U16)(((*(ptr_E+i))>>8)&0xFF00)|(((*(ptr_E+i))>>24)&0xFF);
        RIU[(RSA_BASE_ADDR+(0x24<<1))]= (U16)(((*(ptr_E+i))>>8)&0xFF)|(((*(ptr_E+i))<<8)&0xFF00);
    }

}

void HAL_RSA_LoadKeyN(U32 *ptr_N)
{

    U32 i;

    RIU[(RSA_BASE_ADDR+(0x22<<1))]= RSA_N_BASE_ADDR; //RSA N addr
    RIU[(RSA_BASE_ADDR+(0x20<<1))]= ((RIU[(RSA_BASE_ADDR+(0x20<<1))])|(RSA_IND32_START)); //RSA start

    //RIU[(POR_STATUS_BASE_ADDR+(0xB<<1))]=(U16)((0x0000ffff)&(U32)(ptr_N)); //write ptr_N addr to por_status(0x10050B)

    for( i = 0; i < 64; i++ )
    {
        RIU[(RSA_BASE_ADDR+(0x23<<1))]= (U16)(((*(ptr_N+i))>>8)&0xFF00)|(((*(ptr_N+i))>>24)&0xFF);
        RIU[(RSA_BASE_ADDR+(0x24<<1))]= (U16)(((*(ptr_N+i))>>8)&0xFF)|(((*(ptr_N+i))<<8)&0xFF00);
    }

}

void HAL_RSA_SetKeyLength(U16 u16keylen)
{
    //[13:8] n_len_e: key length, if hardware key set, this register is ignored and hardware internal using 3f
    RIU[(RSA_BASE_ADDR+(0x28<<1))] = ((RIU[(RSA_BASE_ADDR+(0x28<<1))])|(u16keylen<<8));
}

void HAL_RSA_SetKeyType(U8 u8hwkey, U8 u8pubkey)
{
    //[1] hw_key_e : 0 : software key, 1: hardware key
    //[2] e_pub_e : 0: pvivate key, 1: public key
    if(u8hwkey==1)
    {
        RIU[(RSA_BASE_ADDR+(0x28<<1))] = ((RIU[(RSA_BASE_ADDR+(0x28<<1))])|(RSA_SEL_HW_KEY));
    }
    else
    {
        RIU[(RSA_BASE_ADDR+(0x28<<1))] = ((RIU[(RSA_BASE_ADDR+(0x28<<1))])&(~RSA_SEL_HW_KEY));
    }

    if(u8pubkey==1)
    {
        RIU[(RSA_BASE_ADDR+(0x28<<1))] = ((RIU[(RSA_BASE_ADDR+(0x28<<1))])|(RSA_SEL_PUBLIC_KEY));
    }
    else
    {
        RIU[(RSA_BASE_ADDR+(0x28<<1))] = ((RIU[(RSA_BASE_ADDR+(0x28<<1))])&(~RSA_SEL_PUBLIC_KEY));
    }

}

void HAL_RSA_ExponetialStart(void)
{
    //RSA exp start
    RIU[(RSA_BASE_ADDR+(0x27<<1))] = ((RIU[(RSA_BASE_ADDR+(0x27<<1))])|(RSA_EXP_START));
}

U16 HAL_RSA_GetStatus(void)
{
    return RIU[(RSA_BASE_ADDR+(0x29<<1))];
}

void HAL_RSA_FileOutStart(void)
{
    //RSA ind32_start
    RIU[(RSA_BASE_ADDR+(0x20<<1))]= ((RIU[(RSA_BASE_ADDR+(0x20<<1))])|(RSA_IND32_START));
}

void HAL_RSA_SetFileOutAddr(U32 u32offset)
{
    //RSA ind32_addr
    RIU[(RSA_BASE_ADDR+(0x22<<1))] = (U16)(RSA_Z_BASE_ADDR + u32offset);
}

U32 HAL_RSA_FileOut(void)
{
    return (U32)(RIU[(RSA_BASE_ADDR+(0x25<<1))]|(RIU[(RSA_BASE_ADDR+(0x26<<1))]<<16));
}

void HAL_SHA_Reset(void)
{
    //SHA_Reset
    RIU[SHARNG_BASE_ADDR+(0x08<<1)]= (RIU[SHARNG_BASE_ADDR+(0x08<<1)]|(SHARNG_CTRL_SHA_RST));
    RIU[SHARNG_BASE_ADDR+(0x08<<1)]= (RIU[SHARNG_BASE_ADDR+(0x08<<1)]&(~SHARNG_CTRL_SHA_RST));
}

void HAL_SHA_SetLength(U32 u32Size)
{
    //SHA_SetLength:5c~5d(sha_message_length)
    RIU[(SHARNG_BASE_ADDR+(0x0C<<1))]= (U16)((0x0000ffff)&(u32Size));
    RIU[(SHARNG_BASE_ADDR+(0x0D<<1))]= (U16)(((0xffff0000)&(u32Size))>>16);
}

void HAL_SHA_SelMode(U8 u8sha256)
{
    //SHA_SelMode:58~59(sha_ctrl & sha_scattergather_size)
    if(u8sha256==1)
    {
        RIU[(SHARNG_BASE_ADDR+(0x08<<1))]= ((RIU[(SHARNG_BASE_ADDR+(0x08<<1))])|(SHARNG_CTRL_SHA_SEL_SHA256));
    }
    else
    {
        RIU[(SHARNG_BASE_ADDR+(0x08<<1))]= ((RIU[(SHARNG_BASE_ADDR+(0x08<<1))])&(~SHARNG_CTRL_SHA_SEL_SHA256));
    }
}

U16 HAL_SHA_GetStatus(void)
{
    return (RIU[(SHARNG_BASE_ADDR+(0x0F<<1))]);
}

void HAL_SHA_Clear(void)
{
    //Set "1" to idle state after reg_read_sha_ready = 1
    RIU[(SHARNG_BASE_ADDR+(0x08<<1))]= ((RIU[(SHARNG_BASE_ADDR+(0x08<<1))])&(~SHARNG_CTRL_SHA_CLR));
    RIU[(SHARNG_BASE_ADDR+(0x08<<1))]= ((RIU[(SHARNG_BASE_ADDR+(0x08<<1))])|(SHARNG_CTRL_SHA_CLR));
}

void HAL_SHA_Out(U32 u32Buf)
{
    U32 index;

    //SHA_Out
    for( index = 0; index < 16; index++ )
    {
        *((U16 *)u32Buf + index) = (RIU[(SHARNG_BASE_ADDR+(0x10<<1)+index*2)]);
    }
}
