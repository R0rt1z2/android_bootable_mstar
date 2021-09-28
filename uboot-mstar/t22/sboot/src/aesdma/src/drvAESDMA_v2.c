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

#if defined(CONFIG_MSTAR_NS_UBOOT) || defined(CONFIG_TEE_LOADER)
static const U32 u8Padding1[] = { 0x05000420, 0x03040201, 0x86480165, 0x0d060960, 0x00303130 };
static const U32 u8Padding2[] = { 0x0001ffff };

void MDrv_AESDMA_SecureAuthen_v2(U32 u32SHAInBuf, U32 u32Size, U32 u32RSAInBuf, U32 u32SHAOutBuf, U32 u32RSAOutBuf, U8 u8RSA_Key[260])
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


    ptr_RSAextE = (U32 *)(u8RSA_Key+256);
    ptr_RSAextN = (U32 *)u8RSA_Key;

//    ptr_RSAextE = (U32 *)(CONFIG_EMMC_BASE_ADDRESS+((U32)RIU[(POR_STATUS_BASE_ADDR+(0xA<<1))]));
//    ptr_RSAextN = (U32 *)(CONFIG_EMMC_BASE_ADDRESS+((U32)RIU[(POR_STATUS_BASE_ADDR+(0xB<<1))]));

    //SECURE_DEBUG(SECURE_DEBUG_REG, 0x0003);

    HAL_RSA_ClearInt();

    HAL_RSA_Reset();

    while (( HAL_RSA_GetStatus() & RSA_STATUS_RSA_BUSY ) != 0);

    //SECURE_DEBUG(SECURE_DEBUG_REG, 0x0004);

    HAL_RSA_Ind32Ctrl(1);//ind32_ctrl=0xE0

    HAL_RSA_LoadSiganature(ptr_RSAIn);

    HAL_RSA_LoadKeyE_v2(ptr_RSAextE);
    HAL_RSA_LoadKeyN_v2(ptr_RSAextN);

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

    HAL_AESDMA_SetFileinAddr(u32SHAInBuf);

    HAL_AESDMA_Start(0);
    HAL_AESDMA_Start(1);

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

    HAL_SHA_Reset();
    //SECURE_DEBUG(SECURE_DEBUG_REG, 0x000B);
}

U32 MDrv_AESDMA_SecureMain_v2(U32 u32PlaintextAddr, U32 u32Size, U32 u32SignatureAddr, U8 u8RSA_Key[260])
{
    U32 i, j;
    U32 u32RSAInBuf;

    U32 *ptr_SHAOut = (U32 *)(SECURE_SHA_OUT_SRAM_ADDRESS);
    U32 *ptr_RSAOut = (U32 *)(SECURE_RSA_OUT_SRAM_ADDRESS);
    U32 ret = 0;

    u32RSAInBuf = u32SignatureAddr + RSA_SIGNATURE_LEN - 4;

    //SECURE_DEBUG(SECURE_DEBUG_REG, 0x0001);

    HAL_AESDMA_DisableXIUSelectCA9();
    HAL_AESDMA_WB2DMADisable();

    HAL_AESDMA_Disable();

    HAL_AESDMA_ShaFromInput();

    //SECURE_DEBUG(SECURE_DEBUG_REG, 0x0002);

    MDrv_AESDMA_SecureAuthen_v2(u32PlaintextAddr,u32Size,u32RSAInBuf,SECURE_SHA_OUT_SRAM_ADDRESS,SECURE_RSA_OUT_SRAM_ADDRESS, u8RSA_Key);

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

    ret |= (ptr_RSAOut[i] ^ u8Padding2[0]);

    return ret == 0 ? TRUE : FALSE;
}

U32 MDrv_AESDMA_Decrypt(U32 addr, U32 len, U8* key, U32 engine, E_AESDMA_KEYTYPE keytype)
{
    HAL_AESDMA_Reset();
    HAL_AESDMA_WB2DMADisable();
    HAL_AESDMA_SetEngine(engine);
    HAL_AESDMA_SetDecrypt(1);
    HAL_AESDMA_SetKeyIndex(keytype, (U32*)key);
    //HAL_AESDMA_SetKey((U32*)key);
    HAL_AESDMA_SetFileInAddr(addr);
    HAL_AESDMA_SetFileOutAddr(addr);
    HAL_AESDMA_SetFileSize(len);
    HAL_AESDMA_FileOutEnable(1);
    HAL_AESDMA_Start(1);
    while (!HAL_AESDMA_IsFinished());
    HAL_AESDMA_Reset();
    return TRUE;
}

U32 MDrv_AESDMA_Encrypt(U32 addr, U32 len, U8* key, U32 engine, E_AESDMA_KEYTYPE keytype)
{
    HAL_AESDMA_Reset();
    HAL_AESDMA_WB2DMADisable();
    HAL_AESDMA_SetEngine(engine);
    HAL_AESDMA_SetDecrypt(0);
    HAL_AESDMA_SetKeyIndex(keytype, (U32*)key);
    //HAL_AESDMA_SetKey((U32*)key);
    HAL_AESDMA_SetFileInAddr(addr);
    HAL_AESDMA_SetFileOutAddr(addr);
    HAL_AESDMA_SetFileSize(len);
    HAL_AESDMA_FileOutEnable(1);
    HAL_AESDMA_Start(1);
    while (!HAL_AESDMA_IsFinished());
    HAL_AESDMA_Reset();
    return TRUE;
}

static void array_reverse(char* ary, unsigned int len)
{
    int i, j;
    char tmp;
    for (i=0, j=len-1; i<j; ++i, --j)
    {
        tmp = ary[i];
        ary[i] = ary[j];
        ary[j] = tmp;
    }
}

unsigned int MDrv_SYS_Query(E_SYS_QUERY query, unsigned int* result)
{
    return HAL_SYS_Query(query, result);
}


U32 MDrv_AESDMA_STR_CBCMAC(U32 addr, U32 len, U8* result)
{
    int i;

    if ( len % 16 != 0 )
    {
        return FALSE;
    }

    HAL_AESDMA_Reset();
    HAL_AESDMA_WB2DMADisable();
    HAL_AESDMA_SetEngine(AESDMA_ENGINE_CBC);
    HAL_AESDMA_SetDecrypt(0);
    HAL_AESDMA_EnableRNGKey();
    HAL_AESDMA_SetIV(NULL);
    HAL_AESDMA_FileOutEnable(1);

    for ( i=0 ; i<len ; i+=16 )
    {
        HAL_AESDMA_SetFileInAddr(addr + i);
        HAL_AESDMA_SetFileSize(16);
        HAL_AESDMA_SetFileOutAddr(result);
        HAL_AESDMA_Start(1);
        while (!HAL_AESDMA_IsFinished());
        HAL_AESDMA_Reset();
        HAL_AESDMA_WB2DMADisable();
        HAL_AESDMA_SetEngine(AESDMA_ENGINE_CBC);
        HAL_AESDMA_SetDecrypt(0);
        HAL_AESDMA_EnableRNGKey();

        array_reverse(result + CONFIG_MIU0_BUSADDR, 16);
        HAL_AESDMA_SetIV(result + CONFIG_MIU0_BUSADDR);
        HAL_AESDMA_FileOutEnable(1);
    }

    HAL_AESDMA_Reset();
    HAL_AESDMA_DisableRNGKey();
    HAL_AESDMA_ClearRNGKey();

    array_reverse(result + CONFIG_MIU0_BUSADDR, 16);

    return TRUE;
}
#endif

#if ENABLE_SUPPORT_AESDMA_HMAC
U32 MDrv_AESDMA_GetSHA (U32 u32SrcPA, U32 u32Size, U8 eMode, U32 u32ShaOutPA)
{
    HAL_SHA_Clear_Reset();
    HAL_SHA_SetAddress (u32SrcPA);
    HAL_SHA_SetLength(u32Size);

    HAL_SHA_SelMode (eMode);

    HAL_SHA_SetByPassTable(TRUE);

    HAL_SHA_Start();

    while(( HAL_SHA_GetStatus() & SHARNG_CTRL_SHA_READY ) == 0);
    HAL_SHA_Out(u32ShaOutPA);

    HAL_SHA_Clear_Reset();
    return 0;
}

void MDrv_AESDMA_Hmac_sha256(const U8 *text, U32 text_len, const U8 *key, U32 key_len, void *digest)
{
#define SHA256_DIGEST_LENGTH 32
    U8 k_pad[65];   /* padding -
                     * key XORd with ipad
                     */

    U8 *pTemp;
    U8 tk[SHA256_DIGEST_LENGTH];
    U8 buffer[65 + SHA256_DIGEST_LENGTH];
    U32           i,j;

    /*
     * the HMAC_SHA256 transform looks like:
     *
     * SHA256(K XOR opad, SHA256(K XOR ipad, text))
     *
     * where K is an n byte key
     * ipad is the byte 0x36 repeated 64 times
     * opad is the byte 0x5c repeated 64 times
     * and text is the data being protected
     */

    /* start out by storing key in pads */
    memset( k_pad, 0, sizeof k_pad );
    memcpy( k_pad, key, key_len );

    /* XOR key with ipad and opad values */
    for ( i = 0; i < 64; i++ ) {
        k_pad[i] ^= 0x36;
    }

    /*
     * perform inner SHA256
     */

    //Store a 64-Byte space
    pTemp = text - 64;
    memcpy( buffer, pTemp, 64 );
    memcpy( pTemp, k_pad, 64 );
    MDrv_AESDMA_GetSHA ((U32)pTemp, 64 + text_len, 1, (U32)tk);

    //Restore the 64-byte data
    memcpy( pTemp, buffer, 64 );

    /*
     * perform outer SHA256
     */
    memset( k_pad, 0, sizeof k_pad );
    memcpy( k_pad, key, key_len );
    /* XOR key with out pad values */
    for ( i = 0; i < 64; i++ ) {
        k_pad[i] ^= 0x5c;
    }

    pTemp = text;
    memcpy( pTemp, k_pad, 64 );

    pTemp = text + 64;
    //Reorder the hash value and append to the XOR Key.
    for (i = 15, j=0; j<16; i--, j++) {
        ((U16*)pTemp)[j] =  (((U16*)tk)[i] >> 8) & 0x00FF;
        ((U16*)pTemp)[j] |= (((U16*)tk)[i] << 8) & 0xFF00;
    }

    MDrv_AESDMA_GetSHA ((U32)text, 64 + SHA256_DIGEST_LENGTH, 1, (U32)tk);

    //Reordering the hash value
    pTemp = digest;
    for (i = 15, j=0; j<16; i--, j++) {
        ((U16*)pTemp)[j] =  (((U16*)tk)[i] >> 8) & 0x00FF;
        ((U16*)pTemp)[j] |= (((U16*)tk)[i] << 8) & 0xFF00;
    }
}

U32 GetHmacKey(U32 u32KeyBufBA, U32 u32key_len, U8* key)
{
    U32 i;
    for ( i = 0; i < u32key_len; i++ ) {
        *((U8 *)(u32KeyBufBA+i)) = 0x0;
    }

    //
    // Get eMMC CID
    //
    eMMC_GetCID((U8 *)u32KeyBufBA);
    //
    // Encrypt CID
    //
#if defined(CONFIG_MSTAR_NS_UBOOT) || defined(CONFIG_TEE_LOADER)
    HAL_AESDMA_Reset();
    HAL_AESDMA_WB2DMADisable();
#endif

    //Input Addr
    HAL_AESDMA_SetFileinAddr (u32KeyBufBA);

    //Output Addr
    HAL_AESDMA_SetFileOutAddr (u32KeyBufBA - CONFIG_MIU0_BUSADDR);

    //Data Size
    HAL_AESDMA_SetFileSize (u32key_len);

    if(key)  //use sw key
        HAL_AESDMA_SetKeyIndex(E_AESDMA_SWKEY, (U32 *)key);
    else    //Use HW Key
        HAL_AESDMA_SetKeyIndex(E_AESDMA_EFUSEKEY0, 0);

    //Engine: AES Encrypt
    HAL_AESDMA_SetDecrypt(0);
    HAL_AESDMA_SetEngine (AESDMA_ENGINE_ECB);

    //Trigger
    HAL_AESDMA_Start_FileOutEn(1);

    while(( HAL_AESDMA_GetStatus() & AESDMA_CTRL_DMA_DONE ) == 0);

#if defined(CONFIG_MSTAR_NS_UBOOT) || defined(CONFIG_TEE_LOADER)
    HAL_AESDMA_Reset();
    HAL_AESDMA_WB2DMADisable();
#endif
    return TRUE;
}
#endif //#if ENABLE_SUPPORT_AESDMA_HMAC

void MDrv_AESDMA_DisableHwKey(void);
void MDrv_AESDMA_DisableHwKey(void)
{
    //Disable use HW Key on normal bank
    HAL_AESDMA_DisableHwKey();
}
