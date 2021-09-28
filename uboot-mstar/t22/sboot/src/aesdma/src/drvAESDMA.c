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


#ifndef _DRV_AESDMA_H_
#include "drvAESDMA.h"
//#include "halAESDMA.h"
#include "../../mmc/inc/api/drv_eMMC.h"
#endif

static const U32 u8Padding1[] = { 0x05000420, 0x03040201, 0x86480165, 0x0d060960, 0x00303130 };
static const U32 u8Padding2 =  0x0001ffff;

#define MAGIC_UBOOT (0x544f4255)


void MDrv_AESDMA_SecureAuthen(U32 u32SHAInBuf, U32 u32Size, U32 u32RSAInBuf, U32 u32SHAOutBuf, U32 u32RSAOutBuf, U32 u32SramSel)
{
    U32 i;
    U32 *ptr_SHAIn = (U32 *)(u32SHAInBuf);
    U32 *ptr_RSAext = (U32 *)(SECURE_WB_FIFO_OUT_ADDRESS);
    U32 *ptr_RSAIn = (U32 *)(u32RSAInBuf);
    U32 *ptr_RSAOut=(U32 *)(u32RSAOutBuf);
    U32 *ptr_RSAextE=0;
    U32 *ptr_RSAextN=0;
    U32 WFIFO_times = u32Size / 16;//the cycle of XIU read/write
    U32 RFIFO_times = u32Size / 16;
    U32 WFIFO_len,RFIFO_len;
    U32 WFIFO_offset=0;
    U32 RFIFO_offset=0;


#ifdef CONFIG_MSTAR_MUJI  //RSA_KEY TO DRAM
    if (u32SramSel == 0)
	{
        //ptr_RSAextE = (U32 *)(0x20208100);
        //ptr_RSAextN = (U32 *)(0x20208000);
        ptr_RSAextE = (U32 *)(0x20010100);
        ptr_RSAextN = (U32 *)(0x20010000);
    }
    else
	{
        ptr_RSAextE = (U32 *)(CONFIG_EMMC_BASE_ADDRESS+((U32)RIU[(POR_STATUS_BASE_ADDR+(0xA<<1))]));
        ptr_RSAextN = (U32 *)(CONFIG_EMMC_BASE_ADDRESS+((U32)RIU[(POR_STATUS_BASE_ADDR+(0xB<<1))]));
    }
#else
    ptr_RSAextE = (U32 *)(CONFIG_EMMC_BASE_ADDRESS+((U32)RIU[(POR_STATUS_BASE_ADDR+(0xA<<1))]));
    ptr_RSAextN = (U32 *)(CONFIG_EMMC_BASE_ADDRESS+((U32)RIU[(POR_STATUS_BASE_ADDR+(0xB<<1))]));
#endif

    //SECURE_DEBUG(SECURE_DEBUG_REG, 0x0003);

    HAL_RSA_ClearInt();

    HAL_RSA_Reset();

    while (( HAL_RSA_GetStatus() & RSA_STATUS_RSA_BUSY ) != 0);

    //SECURE_DEBUG(SECURE_DEBUG_REG, 0x0004);

    HAL_RSA_Ind32Ctrl(1);//ind32_ctrl=0xE0

    HAL_RSA_LoadSiganature(ptr_RSAIn);

    HAL_RSA_LoadKeyE(ptr_RSAextE);
#ifdef CONFIG_MSTAR_MUJI //RSA_KEY TO DRAM
    if (u32SramSel == 0)
	{
        HAL_RSA_LoadKeyNInverse(ptr_RSAextN);
    }
    else
    {
        HAL_RSA_LoadKeyN(ptr_RSAextN);
    }
#else
    HAL_RSA_LoadKeyN(ptr_RSAextN);
#endif



    HAL_RSA_SetKeyLength(0x3F);

    HAL_RSA_SetKeyType(0,1);//sw key, public key

    HAL_RSA_ExponetialStart();

    //SECURE_DEBUG(SECURE_DEBUG_REG, 0x0005);

    HAL_SHA_Reset();

    HAL_SHA_SetLength(u32Size);

    HAL_AESDMA_SetXIULength(u32Size);

    HAL_SHA_SelMode(1);//sha256

    RFIFO_times = 0x0;
    HAL_AESDMA_FileOutEnable(0);

    if(u32SramSel==0)
    {
        HAL_AESDMA_SetFileinAddr(u32SHAInBuf);
    }

    HAL_AESDMA_Start(1);

    if(u32SramSel==1)
    {
        //read XIU status
        while((XIU[0x8] & XIU_STATUS_W_RDY) == 0);

        //SECURE_DEBUG(SECURE_DEBUG_REG, 0x0006);

        while( (WFIFO_times != 0) || (RFIFO_times != 0) )
        {
            WFIFO_len = (XIU[0x8]&XIU_STATUS_W_LEN)>>4;

            if((XIU[0x8] & XIU_STATUS_W_RDY) == 1)
            {
                for( i = 0; i < WFIFO_len; i++ )
                {
                    XIU[0x0] = *(ptr_SHAIn + ((i+WFIFO_offset)*0x4) + 0x0);
                    XIU[0x1] = *(ptr_SHAIn + ((i+WFIFO_offset)*0x4) + 0x1);
                    XIU[0x2] = *(ptr_SHAIn + ((i+WFIFO_offset)*0x4) + 0x2);
                    XIU[0x3] = *(ptr_SHAIn + ((i+WFIFO_offset)*0x4) + 0x3);
                }
                WFIFO_offset += WFIFO_len;
                WFIFO_times -= WFIFO_len;
            }

            RFIFO_len = (XIU[0x8]&XIU_STATUS_R_LEN)>>12;

            //pop FIFO out
            if(((XIU[0x8] & XIU_STATUS_R_RDY)>>8) == 1)
            {
                for( i = 0; i < RFIFO_len; i++)
                {
                    *(ptr_RSAext + ((i+RFIFO_offset)*0x4) + 0x0) = XIU[0x4];
                    *(ptr_RSAext + ((i+RFIFO_offset)*0x4) + 0x1) = XIU[0x5];
                    *(ptr_RSAext + ((i+RFIFO_offset)*0x4) + 0x2) = XIU[0x6];
                    *(ptr_RSAext + ((i+RFIFO_offset)*0x4) + 0x3) = XIU[0x7];
                }
                RFIFO_offset += RFIFO_len;
                RFIFO_times -= RFIFO_len;
            }
        }
    }
    //SECURE_DEBUG(SECURE_DEBUG_REG, 0x0007);

    while(( HAL_AESDMA_GetStatus() & AESDMA_CTRL_DMA_DONE ) == 0);

    while(( HAL_SHA_GetStatus() & SHARNG_CTRL_SHA_READY ) == 0);

    //SECURE_DEBUG(SECURE_DEBUG_REG, 0x0008);

    HAL_SHA_Out(u32SHAOutBuf);//write sha_out to sram

    HAL_SHA_Clear();

    //SECURE_DEBUG(SECURE_DEBUG_REG, 0x0009);

    while (( HAL_SHA_GetStatus() & SHARNG_CTRL_SHA_BUSY ) != 0);

    while (( HAL_RSA_GetStatus() & RSA_STATUS_RSA_BUSY ) != 0);

    HAL_RSA_Ind32Ctrl(0);//ind32_ctrl=0xC0

    //SECURE_DEBUG(SECURE_DEBUG_REG, 0x000A);

//    for( i = 0; i<(SHA256_DIGEST_SIZE/4); i++)
    for( i = 0; i<(RSA_SIGNATURE_LEN/4); i++)
    {
        HAL_RSA_SetFileOutAddr(i);
        HAL_RSA_FileOutStart();
        *(ptr_RSAOut+i) = HAL_RSA_FileOut();//write rsa_out to sram
    }

    HAL_RSA_Reset();

    //SECURE_DEBUG(SECURE_DEBUG_REG, 0x000B);
}

U32 MDrv_AESDMA_SecureMain(U32 u32PlaintextAddr, U32 u32Size, U32 u32SignatureAddr, U32 u32SramSel)
{
    U32 i, j;
    U32 u32RSAInBuf;

    U32 *ptr_SHAOut = (U32 *)(SECURE_SHA_OUT_SRAM_ADDRESS);
    U32 *ptr_RSAOut = (U32 *)(SECURE_RSA_OUT_SRAM_ADDRESS);
    U32 ret = 0;

    u32RSAInBuf = u32SignatureAddr + RSA_SIGNATURE_LEN - 4;

    //SECURE_DEBUG(SECURE_DEBUG_REG, 0x0001);

    if(u32SramSel==0)
    {
        HAL_AESDMA_DisableXIUSelectCA9();
        HAL_AESDMA_WB2DMADisable();
    }

    HAL_AESDMA_Disable();

    HAL_AESDMA_ShaFromInput();

    //SECURE_DEBUG(SECURE_DEBUG_REG, 0x0002);

    MDrv_AESDMA_SecureAuthen(u32PlaintextAddr,u32Size,u32RSAInBuf,SECURE_SHA_OUT_SRAM_ADDRESS,SECURE_RSA_OUT_SRAM_ADDRESS,u32SramSel);

    // compare sha256
    for(i = 0; i<(SHA256_DIGEST_SIZE/4); ++i)
    {
        ret |= (ptr_RSAOut[i] ^ ptr_SHAOut[i]);
    }

    // compare pkcs1.5 padding
    for(j = 0; j<5; ++i, ++j)
    {
        ret |= (ptr_RSAOut[i] ^ u8Padding1[j]);
    }
    for(j = 0; j<50; ++i, ++j)
    {
        ret |= (ptr_RSAOut[i] ^ 0xffffffff);
    }

    ret |= (ptr_RSAOut[i] ^ u8Padding2);

    return ret == 0 ? TRUE : FALSE;
}

U32 CompareMagic(U32 data, U32 stage);
U32 CompareMagic(U32 data, U32 stage)
{
    U32 magic = 0;
    if (stage==1)
        magic = MAGIC_BOOT1;
    else if (stage==2)
        magic = MAGIC_BOOT2;
    else if (stage==3)
        magic = MAGIC_BOOT3;
    else if (stage==4)
        magic = MAGIC_BOOT4;
    else if (stage==0xC)
        magic = MAGIC_CHUNK;
    else if (stage==0xF)
        magic = MAGIC_UBOOT;
    else
        return FALSE;

    return data==magic ? TRUE : FALSE;
}

static U32 UbootRamEnd = 0;
void Save_UbootRamEnd(U32 addr);
void Save_UbootRamEnd(U32 addr)
{
    UbootRamEnd = addr;
}

U32 Get_UbootRamEnd(void)
{
    return UbootRamEnd;
}
