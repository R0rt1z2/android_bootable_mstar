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

#define CRYPTO_AES_C
#include <secure/crypto_aes.h>
#include <secure/MsSecureCommon.h>
#include <common.h>
#include <MsSystem.h>
#include <MsString.h>
#include <MsDebug.h>
#include <drvAESDMA.h>
#include <drvSYS.h>
#if defined(CONFIG_FOXCONN_ENABLE)
#include <MsSysUtility.h>
#endif
#include <MsUtility.h>
#include <efuse_key/TEE_SetKey.h>
#include "drvDSCMB.h"

//SW Efuse Key to encrypt CustomerKeyBank
static U8 hwKey[0x10]={0xE0, 0x10, 0x01, 0xFF, 0x0F, 0xAA, 0x55, 0xFC, \
                       0x92, 0x4D, 0x53, 0x54, 0x41, 0xFF, 0x07, 0x00}; // temporary

// for test
static uint8_t u8TestKL_3L_AES_to_DMA_RootKey[16] = {
    0xa3, 0x05, 0xc3, 0x94, 0x4f, 0x9f, 0x83, 0x00, 0x22, 0x1c, 0x68, 0x19, 0x8e, 0xe5, 0xef, 0x9b};
// for test
static uint8_t u8TestKL_3L_AES_to_DMA_Input[48] = {
    //Input Key 0
    0xda, 0x1d, 0xe9, 0xb9, 0xff, 0xf1, 0x2b, 0x8a, 0xff, 0xa2, 0x14, 0x0d, 0xfa, 0x99, 0xfc, 0x41,
    //Input Key 1
    0x95, 0x73, 0x93, 0x5f, 0x96, 0x8d, 0xec, 0xb6, 0x51, 0x85, 0xc3, 0xcd, 0xaa, 0x5b, 0x64, 0x18,
    //Input Key 2 (Encrypted Control Word)
    0x48, 0xd0, 0xcf, 0xaa, 0x00, 0x23, 0x43, 0x6b, 0x09, 0xd9, 0x4b, 0xe2, 0xe3, 0x6e, 0x07, 0x2f
    };


#if defined(CONFIG_SECURE_HW_IP)
#define BIT(bits)                   (1<<bits)
typedef struct
{
    MS_U32 pChiperBuf;
    MS_U32 pPlainBuf;
    MS_U32 *pKey;
    MS_U32 *pIV;
    MS_U32 u32Len;
    MS_BOOL bIsDecrypt;
    DrvAESDMA_CipherMode eMode;
}AES_ParamStruct;

static int CommonAES128(AES_ParamStruct *AESPara)
{
    DRVAESDMA_RESULT result = DRVAESDMA_OK;
    MS_U32 u32AESInBuf, u32AESOutBuf;
    UBOOT_TRACE("IN\n");
    if((AESPara->pChiperBuf == 0)||(AESPara->pPlainBuf == 0))
    {
        UBOOT_ERROR("The input parameters are not correct\n");
        return DRVAESDMA_INVALID_PARAM;
    }

    flush_cache(AESPara->pChiperBuf, AESPara->u32Len);

    u32AESInBuf=VA2PA(AESPara->pChiperBuf);
    u32AESOutBuf=VA2PA(AESPara->pPlainBuf);
    MDrv_AESDMA_Init(0x00, 0x20000000, 2); // wait the AESDMA.a
    MDrv_AESDMA_Reset();
    MDrv_AESDMA_SelEng(AESPara->eMode, AESPara->bIsDecrypt);
    result=MDrv_AESDMA_SetFileInOut(u32AESInBuf, AESPara->u32Len, u32AESOutBuf, u32AESOutBuf+AESPara->u32Len-1);
    if (result == DRVAESDMA_MIU_ADDR_ERROR)
    {
        UBOOT_ERROR("CommonAES128 execte MDrv_AESDMA_SetFileInOut fail\n");
        return -1;
    }

    if(AESPara->eMode == E_DRVAESDMA_CIPHER_CBC)
    {
        MDrv_AESDMA_SetIV(AESPara->pIV);
    }
    char key[AES_KEY_LEN] = "\0";
    if(AESPara->pKey!=NULL)
    {
        memcpy(key,AESPara->pKey,AES_KEY_LEN);
        MDrv_AESDMA_SetKey((MS_U32*)key);
        //MDrv_AESDMA_SetKey(AESPara->pKey);
    }
    else
    {
#if defined(CONFIG_FOXCONN_ENABLE)

        MS_U16 dev_id[8]={0};

        dev_id[0]=Read2Byte(0x3800);
        dev_id[1]=Read2Byte(0x3802);
        dev_id[2]=Read2Byte(0x3804);
        //efuse key is disable, use device id as key

        UBOOT_DEBUG("device id key\n");
        UBOOT_DUMP("sizeof(dev_id)=%d\n",sizeof(dev_id));
        MDrv_AESDMA_SetKey((MS_U32*)dev_id);
#else
        //key is NULL ,we use Efuse key
        UBOOT_DEBUG("Use EFUSE Key\n");
        TEE_SetKey();
#endif

    }

    UBOOT_DEBUG("AESDMA Start\n");
    MDrv_AESDMA_Start(TRUE);
    UBOOT_DEBUG("AESDMA polling finish bits\n");

    while(MDrv_AESDMA_IsFinished() != DRVAESDMA_OK);

    flush_cache(AESPara->pPlainBuf, AESPara->u32Len);
    UBOOT_TRACE("OK\n");
    return 0;
}

int Secure_AES_ECB_Decrypt_HW(MS_U32 u32Addr, MS_U32 u32Len, MS_U8 *bKey)
{
    int ret=0;
    UBOOT_TRACE("IN\n");
    //check u32Len align 16
    if(0 != u32Len%16)
    {
         UBOOT_ERROR("u32InLen should align 16\n");
         return -1;
    }
	AES_ParamStruct AESParam;
    AESParam.eMode = E_DRVAESDMA_CIPHER_ECB;
    AESParam.pChiperBuf = u32Addr;
    AESParam.pPlainBuf = u32Addr;
    AESParam.pKey = (MS_U32 *)bKey;
    AESParam.u32Len = u32Len;
    AESParam.pIV= (MS_U32 *)NULL;
    AESParam.bIsDecrypt = TRUE;
    ret=CommonAES128(&AESParam);
    if(ret==0)
    {
        UBOOT_TRACE("OK\n");
    }
    else
    {
        UBOOT_ERROR("Secure_AES_Decrypt fail\n");
    }

    return ret;
}
int Secure_AES_ECB_Encrypt_HW(MS_U32 u32Addr, MS_U32 u32Len, MS_U8 *bKey)
{
    int ret=0;
    UBOOT_TRACE("IN\n");
    //check u32Len align 16
    if(0 != u32Len%16)
    {
         UBOOT_ERROR("u32InLen should align 16\n");
         return -1;
    }
	AES_ParamStruct AESParam;
    AESParam.eMode = E_DRVAESDMA_CIPHER_ECB;
    AESParam.pChiperBuf = u32Addr;
    AESParam.pPlainBuf = u32Addr;
    AESParam.pKey = (MS_U32 *)bKey;
    AESParam.u32Len = u32Len;
    AESParam.pIV= (MS_U32 *)NULL;
    AESParam.bIsDecrypt = FALSE;
    ret=CommonAES128(&AESParam);
    if(ret==0)
    {
        UBOOT_TRACE("OK\n");
    }
    else
    {
        UBOOT_ERROR("Secure_AES_Encrypt Error\n");
    }

    return ret;
}
#endif

#if defined(CONFIG_SECURE_SMC_HW_IP)

//int SetAESKeyByKeyladder()
//{
//    DSCMB_KLCfg_All KLConfigAll;
//    uint32_t u32Status = 0;
//    uint8_t ACPU_Out[16] = {0x00};
//    UBOOT_TRACE("IN\n");
//    /// KLCfg
//    /// From TEE_Loader
//    KLConfigAll.eAlgo      = E_DSCMB_KL_AES;
//    KLConfigAll.eSrc       = E_DSCMB_KL_SRC_ACPU;
//    KLConfigAll.eDst       = E_DSCMB_KL_DST_DMA_SK0;
//    /// From User
//    KLConfigAll.eOutsize   = E_DSCMB_KL_128_BITS;
//    KLConfigAll.eKeyType   = 0;  // Don't care when dst is Crypto DMA
//    KLConfigAll.u32Level   = 3;  // 1-layer
//    KLConfigAll.u32EngID   = 0;  //Set to zero in STB chip, Don't care when dst is Crypto DMA
//    KLConfigAll.u32DscID   = 0;  // Don't care when dst is Crypto DMA
//    KLConfigAll.u8KeyACPU  = u8TestKL_3L_AES_to_DMA_RootKey;    //root key
//    KLConfigAll.pu8KeyKLIn = u8TestKL_3L_AES_to_DMA_Input;     //input
//    KLConfigAll.bDecrypt   = TRUE;
//    KLConfigAll.bInverse   = FALSE;
//    KLConfigAll.eKLSel     = E_DSCMB_KL_SEL_PVR;
//    KLConfigAll.u32CAVid   = 0xF;    // 3 for VIACCESS, F for test
//
//    flush_cache(&KLConfigAll, sizeof(DSCMB_KLCfg_All));
//    flush_cache(&KLConfigAll.u8KeyACPU, sizeof(KLConfigAll.u8KeyACPU));
//    flush_cache(&KLConfigAll.pu8KeyKLIn, sizeof(KLConfigAll.pu8KeyKLIn));
//
//    __asm__ __volatile__(
//        "ldr r0,=0xb200585a \n\t"
//        "mov  r1,%0  \n\t"
//        "mov  r2,%1  \n\t"
//        "mov  r3,%2  \n\t"
//        "smc #0  \n\t"
//        :
//        : "r"(&KLConfigAll),"r"(ACPU_Out),"r"(&u32Status)
//        : "r0","r1","r2","r3"
//    );
//
//    UBOOT_TRACE("OK\n");
//    return u32Status;
//}

static int SMC_CommonAES128(FastCryptoCalParam64 *AESPara)
{
    //TEE_Result result = 0;
    int j=0;
    MS_U32 u32AESInBuf, u32AESOutBuf;

    UBOOT_TRACE("IN\n");
    if((AESPara->input_address == 0)||(AESPara->output_address== 0))
    {
        UBOOT_ERROR("The input parameters are not correct\n");
        return -1;
    }

    UBOOT_DEBUG("AES:AESPara->input_address=%llx\n", AESPara->input_address);
    UBOOT_DEBUG("AES:AESPara->output_address=%llx\n", AESPara->output_address);

    flush_cache(AESPara, sizeof(FastCryptoCalParam64));
    flush_cache(AESPara->input_address, AESPara->input_size);
    flush_cache(AESPara->output_address, AESPara->output_size);
    flush_cache(AESPara->swkey, AESPara->swkey_len);

    struct smc_param aes_smc_params;
    aes_smc_params.a0 = 0xb2005860;
    aes_smc_params.a1 = AESPara;
    tee_smc_call(&aes_smc_params);

    /*
    __asm__ __volatile__(
        "ldr r0, =0xb2005860 \n\t"
        "mov r1,%0 \n\t"
        "smc #0 \n\t"
        :
        : "r"(AESPara)
        : "r0","r1"
    );
    */
    UBOOT_TRACE("OK\n");
    return 0;
}

int Secure_AES_ECB_Decrypt_SMC_HW(MS_U32 u32Addr, MS_U32 u32Len, MS_U8 *bKey)
{
    int ret=0;
    int i = 0;

    UBOOT_TRACE("IN\n");
    //check u32Len align 16
    if(0 != u32Len%16)
    {
         UBOOT_ERROR("u32InLen should align 16\n");
         return -1;
    }

    FastCryptoCalParam64 AESParam;
    memset(&AESParam, 0, sizeof(FastCryptoCalParam64));
    AESParam.mode = MODE_DECRYPT;
    AESParam.secure_algo = ALG_AES_ECB_NOPAD;
    AESParam.input_address = (uint64_t)u32Addr;
    AESParam.input_size = u32Len;
    AESParam.output_address = (uint64_t)u32Addr;
    AESParam.output_size = u32Len;
    AESParam.iv = (uint64_t) bKey;
    AESParam.iv_len = AES_KEY_LEN;
    AESParam.swkey = (uint64_t) bKey;
    AESParam.swkey_len= AES_KEY_LEN;

    #if defined(CONFIG_SECURE_KEYLADDER_SMC_HW_IP)
    SetAESKeyByKeyladder();
    AESParam.key_type = HWKEY_SLOT;
    AESParam.key_index = E_DSCMB_KL_DST_DMA_SK0;
    #else
#if 0
    // ree key bank
    AESParam.key_type = KEYBANK_INDEX;
    AESParam.key_index = 0;
#else
    // use SW key
    UBOOT_DEBUG("USE SWKEY\n");
    AESParam.key_type = SWKEY;
    AESParam.key_index = 0;
#endif
    #endif

    UBOOT_DEBUG("\nBefore\n");
    //UBOOT_DUMP((unsigned int)u32Addr, 32);
    //UBOOT_DUMP((unsigned int)u32OutBuf, 32);
    UBOOT_DUMP((unsigned int)AESParam.input_address, 32);
    UBOOT_DUMP((unsigned int)AESParam.output_address, 32);
    UBOOT_DEBUG("\nKEY=\n");
    UBOOT_DUMP((unsigned int)AESParam.swkey, 16);

    ret=SMC_CommonAES128(&AESParam);

    UBOOT_DEBUG("\nAfter\n");
    //UBOOT_DUMP((unsigned int)u32Addr, 32);
    UBOOT_DUMP((unsigned int)AESParam.input_address, 32);
    UBOOT_DUMP((unsigned int)AESParam.output_address, 32);

    if(ret==0)
    {
        UBOOT_TRACE("OK\n");
    }
    else
    {
        UBOOT_ERROR("Secure_AES_Decrypt fail\n");
    }
    return ret;
}

#endif

#define AES_ENCRYPT     0
#define AES_DECRYPT     1

/**
 * \brief          AES context structure
 */
typedef struct
{
    int nr;                     /*!<  number of rounds  */
    unsigned long *rk;          /*!<  AES round keys    */
    unsigned long buf[68];      /*!<  unaligned data    */
}
aes_context;


/*
 * 32-bit integer manipulation macros (little endian)
 */
#ifndef GET_ULONG_LE
#define GET_ULONG_LE(n,b,i)                             \
{                                                       \
    (n) = ( (unsigned long) (b)[(i)    ]       )        \
        | ( (unsigned long) (b)[(i) + 1] <<  8 )        \
        | ( (unsigned long) (b)[(i) + 2] << 16 )        \
        | ( (unsigned long) (b)[(i) + 3] << 24 );       \
}
#endif

#ifndef PUT_ULONG_LE
#define PUT_ULONG_LE(n,b,i)                             \
{                                                       \
    (b)[(i)    ] = (unsigned char) ( (n)       );       \
    (b)[(i) + 1] = (unsigned char) ( (n) >>  8 );       \
    (b)[(i) + 2] = (unsigned char) ( (n) >> 16 );       \
    (b)[(i) + 3] = (unsigned char) ( (n) >> 24 );       \
}
#endif


/*
 * Forward S-box & tables
 */
static unsigned char FSb[256];
static unsigned long FT0[256];
static unsigned long FT1[256];
static unsigned long FT2[256];
static unsigned long FT3[256];

/*
 * Reverse S-box & tables
 */
static unsigned char RSb[256];
static unsigned long RT0[256];
static unsigned long RT1[256];
static unsigned long RT2[256];
static unsigned long RT3[256];

/*
 * Round constants
 */
static unsigned long RCON[10];

/*
 * Tables generation code
 */
#define ROTL8(x) ( ( x << 8 ) & 0xFFFFFFFF ) | ( x >> 24 )
#define XTIME(x) ( ( x << 1 ) ^ ( ( x & 0x80 ) ? 0x1B : 0x00 ) )
#define MUL(x,y) ( ( x && y ) ? pow[(log[x]+log[y]) % 255] : 0 )

static int aes_init_done = 0;

static void aes_gen_tables( void )
{
    int i, x, y, z;
    int pow[256];
    int log[256];

    /*
     * compute pow and log tables over GF(2^8)
     */
    for( i = 0, x = 1; i < 256; i++ )
    {
        pow[i] = x;
        log[x] = i;
        x = ( x ^ XTIME( x ) ) & 0xFF;
    }

    /*
     * calculate the round constants
     */
    for( i = 0, x = 1; i < 10; i++ )
    {
        RCON[i] = (unsigned long) x;
        x = XTIME( x ) & 0xFF;
    }

    /*
     * generate the forward and reverse S-boxes
     */
    FSb[0x00] = 0x63;
    RSb[0x63] = 0x00;

    for( i = 1; i < 256; i++ )
    {
        x = pow[255 - log[i]];

        y  = x; y = ( (y << 1) | (y >> 7) ) & 0xFF;
        x ^= y; y = ( (y << 1) | (y >> 7) ) & 0xFF;
        x ^= y; y = ( (y << 1) | (y >> 7) ) & 0xFF;
        x ^= y; y = ( (y << 1) | (y >> 7) ) & 0xFF;
        x ^= y ^ 0x63;

        FSb[i] = (unsigned char) x;
        RSb[x] = (unsigned char) i;
    }

    /*
     * generate the forward and reverse tables
     */
    for( i = 0; i < 256; i++ )
    {
        x = FSb[i];
        y = XTIME( x ) & 0xFF;
        z =  ( y ^ x ) & 0xFF;

        FT0[i] = ( (unsigned long) y       ) ^
                 ( (unsigned long) x <<  8 ) ^
                 ( (unsigned long) x << 16 ) ^
                 ( (unsigned long) z << 24 );

        FT1[i] = ROTL8( FT0[i] );
        FT2[i] = ROTL8( FT1[i] );
        FT3[i] = ROTL8( FT2[i] );

        x = RSb[i];

        RT0[i] = ( (unsigned long) MUL( 0x0E, x )       ) ^
                 ( (unsigned long) MUL( 0x09, x ) <<  8 ) ^
                 ( (unsigned long) MUL( 0x0D, x ) << 16 ) ^
                 ( (unsigned long) MUL( 0x0B, x ) << 24 );

        RT1[i] = ROTL8( RT0[i] );
        RT2[i] = ROTL8( RT1[i] );
        RT3[i] = ROTL8( RT2[i] );
    }
}


/*
 * AES key schedule (encryption)
 */
void aes_setkey_enc( aes_context *ctx, unsigned char *key, int keysize )
{
    int i;
    unsigned long *RK;

    if( aes_init_done == 0 )
    {
        aes_gen_tables();
        aes_init_done = 1;
    }

    switch( keysize )
    {
        case 128: ctx->nr = 10; break;
        case 192: ctx->nr = 12; break;
        case 256: ctx->nr = 14; break;
        default : return;
    }

    ctx->rk = RK = ctx->buf;

    for( i = 0; i < (keysize >> 5); i++ )
    {
        GET_ULONG_LE( RK[i], key, i << 2 );
    }

    switch( ctx->nr )
    {
        case 10:

            for( i = 0; i < 10; i++, RK += 4 )
            {
                RK[4]  = RK[0] ^ RCON[i] ^
                    ( FSb[ ( RK[3] >>  8 ) & 0xFF ]       ) ^
                    ( FSb[ ( RK[3] >> 16 ) & 0xFF ] <<  8 ) ^
                    ( FSb[ ( RK[3] >> 24 ) & 0xFF ] << 16 ) ^
                    ( FSb[ ( RK[3]       ) & 0xFF ] << 24 );

                RK[5]  = RK[1] ^ RK[4];
                RK[6]  = RK[2] ^ RK[5];
                RK[7]  = RK[3] ^ RK[6];
            }
            break;

        case 12:

            for( i = 0; i < 8; i++, RK += 6 )
            {
                RK[6]  = RK[0] ^ RCON[i] ^
                    ( FSb[ ( RK[5] >>  8 ) & 0xFF ]       ) ^
                    ( FSb[ ( RK[5] >> 16 ) & 0xFF ] <<  8 ) ^
                    ( FSb[ ( RK[5] >> 24 ) & 0xFF ] << 16 ) ^
                    ( FSb[ ( RK[5]       ) & 0xFF ] << 24 );

                RK[7]  = RK[1] ^ RK[6];
                RK[8]  = RK[2] ^ RK[7];
                RK[9]  = RK[3] ^ RK[8];
                RK[10] = RK[4] ^ RK[9];
                RK[11] = RK[5] ^ RK[10];
            }
            break;

        case 14:

            for( i = 0; i < 7; i++, RK += 8 )
            {
                RK[8]  = RK[0] ^ RCON[i] ^
                    ( FSb[ ( RK[7] >>  8 ) & 0xFF ]       ) ^
                    ( FSb[ ( RK[7] >> 16 ) & 0xFF ] <<  8 ) ^
                    ( FSb[ ( RK[7] >> 24 ) & 0xFF ] << 16 ) ^
                    ( FSb[ ( RK[7]       ) & 0xFF ] << 24 );

                RK[9]  = RK[1] ^ RK[8];
                RK[10] = RK[2] ^ RK[9];
                RK[11] = RK[3] ^ RK[10];

                RK[12] = RK[4] ^
                    ( FSb[ ( RK[11]       ) & 0xFF ]       ) ^
                    ( FSb[ ( RK[11] >>  8 ) & 0xFF ] <<  8 ) ^
                    ( FSb[ ( RK[11] >> 16 ) & 0xFF ] << 16 ) ^
                    ( FSb[ ( RK[11] >> 24 ) & 0xFF ] << 24 );

                RK[13] = RK[5] ^ RK[12];
                RK[14] = RK[6] ^ RK[13];
                RK[15] = RK[7] ^ RK[14];
            }
            break;

        default:

            break;
    }
}

/*
 * AES key schedule (decryption)
 */
void aes_setkey_dec( aes_context *ctx, unsigned char *key, int keysize )
{
    int i, j;
    aes_context cty;
    unsigned long *RK;
    unsigned long *SK;

    switch( keysize )
    {
        case 128: ctx->nr = 10; break;
        case 192: ctx->nr = 12; break;
        case 256: ctx->nr = 14; break;
        default : return;
    }

    ctx->rk = RK = ctx->buf;

    aes_setkey_enc( &cty, key, keysize );
    SK = cty.rk + cty.nr * 4;

    *RK++ = *SK++;
    *RK++ = *SK++;
    *RK++ = *SK++;
    *RK++ = *SK++;

    for( i = ctx->nr, SK -= 8; i > 1; i--, SK -= 8 )
    {
        for( j = 0; j < 4; j++, SK++ )
        {
            *RK++ = RT0[ FSb[ ( *SK       ) & 0xFF ] ] ^
                    RT1[ FSb[ ( *SK >>  8 ) & 0xFF ] ] ^
                    RT2[ FSb[ ( *SK >> 16 ) & 0xFF ] ] ^
                    RT3[ FSb[ ( *SK >> 24 ) & 0xFF ] ];
        }
    }

    *RK++ = *SK++;
    *RK++ = *SK++;
    *RK++ = *SK++;
    *RK++ = *SK++;

    memset( &cty, 0, sizeof( aes_context ) );
}

#define AES_FROUND(X0,X1,X2,X3,Y0,Y1,Y2,Y3)     \
{                                               \
    X0 = *RK++ ^ FT0[ ( Y0       ) & 0xFF ] ^   \
                 FT1[ ( Y1 >>  8 ) & 0xFF ] ^   \
                 FT2[ ( Y2 >> 16 ) & 0xFF ] ^   \
                 FT3[ ( Y3 >> 24 ) & 0xFF ];    \
                                                \
    X1 = *RK++ ^ FT0[ ( Y1       ) & 0xFF ] ^   \
                 FT1[ ( Y2 >>  8 ) & 0xFF ] ^   \
                 FT2[ ( Y3 >> 16 ) & 0xFF ] ^   \
                 FT3[ ( Y0 >> 24 ) & 0xFF ];    \
                                                \
    X2 = *RK++ ^ FT0[ ( Y2       ) & 0xFF ] ^   \
                 FT1[ ( Y3 >>  8 ) & 0xFF ] ^   \
                 FT2[ ( Y0 >> 16 ) & 0xFF ] ^   \
                 FT3[ ( Y1 >> 24 ) & 0xFF ];    \
                                                \
    X3 = *RK++ ^ FT0[ ( Y3       ) & 0xFF ] ^   \
                 FT1[ ( Y0 >>  8 ) & 0xFF ] ^   \
                 FT2[ ( Y1 >> 16 ) & 0xFF ] ^   \
                 FT3[ ( Y2 >> 24 ) & 0xFF ];    \
}

#define AES_RROUND(X0,X1,X2,X3,Y0,Y1,Y2,Y3)     \
{                                               \
    X0 = *RK++ ^ RT0[ ( Y0       ) & 0xFF ] ^   \
                 RT1[ ( Y3 >>  8 ) & 0xFF ] ^   \
                 RT2[ ( Y2 >> 16 ) & 0xFF ] ^   \
                 RT3[ ( Y1 >> 24 ) & 0xFF ];    \
                                                \
    X1 = *RK++ ^ RT0[ ( Y1       ) & 0xFF ] ^   \
                 RT1[ ( Y0 >>  8 ) & 0xFF ] ^   \
                 RT2[ ( Y3 >> 16 ) & 0xFF ] ^   \
                 RT3[ ( Y2 >> 24 ) & 0xFF ];    \
                                                \
    X2 = *RK++ ^ RT0[ ( Y2       ) & 0xFF ] ^   \
                 RT1[ ( Y1 >>  8 ) & 0xFF ] ^   \
                 RT2[ ( Y0 >> 16 ) & 0xFF ] ^   \
                 RT3[ ( Y3 >> 24 ) & 0xFF ];    \
                                                \
    X3 = *RK++ ^ RT0[ ( Y3       ) & 0xFF ] ^   \
                 RT1[ ( Y2 >>  8 ) & 0xFF ] ^   \
                 RT2[ ( Y1 >> 16 ) & 0xFF ] ^   \
                 RT3[ ( Y0 >> 24 ) & 0xFF ];    \
}

/*
 * AES-ECB block encryption/decryption
 */
void aes_crypt_ecb( aes_context *ctx,
                    int mode,
                    unsigned char input[16],
                    unsigned char output[16] )
{
    int i;
    unsigned long *RK, X0, X1, X2, X3, Y0, Y1, Y2, Y3;


    RK = ctx->rk;

    GET_ULONG_LE( X0, input,  0 ); X0 ^= *RK++;
    GET_ULONG_LE( X1, input,  4 ); X1 ^= *RK++;
    GET_ULONG_LE( X2, input,  8 ); X2 ^= *RK++;
    GET_ULONG_LE( X3, input, 12 ); X3 ^= *RK++;

    if( mode == AES_ENCRYPT )
    {
        for( i = (ctx->nr >> 1); i > 1; i-- )
        {
            AES_FROUND( Y0, Y1, Y2, Y3, X0, X1, X2, X3 );
            AES_FROUND( X0, X1, X2, X3, Y0, Y1, Y2, Y3 );
        }

        AES_FROUND( Y0, Y1, Y2, Y3, X0, X1, X2, X3 );

        X0 = *RK++ ^ ( FSb[ ( Y0       ) & 0xFF ]       ) ^
                     ( FSb[ ( Y1 >>  8 ) & 0xFF ] <<  8 ) ^
                     ( FSb[ ( Y2 >> 16 ) & 0xFF ] << 16 ) ^
                     ( FSb[ ( Y3 >> 24 ) & 0xFF ] << 24 );

        X1 = *RK++ ^ ( FSb[ ( Y1       ) & 0xFF ]       ) ^
                     ( FSb[ ( Y2 >>  8 ) & 0xFF ] <<  8 ) ^
                     ( FSb[ ( Y3 >> 16 ) & 0xFF ] << 16 ) ^
                     ( FSb[ ( Y0 >> 24 ) & 0xFF ] << 24 );

        X2 = *RK++ ^ ( FSb[ ( Y2       ) & 0xFF ]       ) ^
                     ( FSb[ ( Y3 >>  8 ) & 0xFF ] <<  8 ) ^
                     ( FSb[ ( Y0 >> 16 ) & 0xFF ] << 16 ) ^
                     ( FSb[ ( Y1 >> 24 ) & 0xFF ] << 24 );

        X3 = *RK++ ^ ( FSb[ ( Y3       ) & 0xFF ]       ) ^
                     ( FSb[ ( Y0 >>  8 ) & 0xFF ] <<  8 ) ^
                     ( FSb[ ( Y1 >> 16 ) & 0xFF ] << 16 ) ^
                     ( FSb[ ( Y2 >> 24 ) & 0xFF ] << 24 );
    }
    else /* AES_DECRYPT */
    {
        for( i = (ctx->nr >> 1); i > 1; i-- )
        {
            AES_RROUND( Y0, Y1, Y2, Y3, X0, X1, X2, X3 );
            AES_RROUND( X0, X1, X2, X3, Y0, Y1, Y2, Y3 );
        }

        AES_RROUND( Y0, Y1, Y2, Y3, X0, X1, X2, X3 );

        X0 = *RK++ ^ ( RSb[ ( Y0       ) & 0xFF ]       ) ^
                     ( RSb[ ( Y3 >>  8 ) & 0xFF ] <<  8 ) ^
                     ( RSb[ ( Y2 >> 16 ) & 0xFF ] << 16 ) ^
                     ( RSb[ ( Y1 >> 24 ) & 0xFF ] << 24 );

        X1 = *RK++ ^ ( RSb[ ( Y1       ) & 0xFF ]       ) ^
                     ( RSb[ ( Y0 >>  8 ) & 0xFF ] <<  8 ) ^
                     ( RSb[ ( Y3 >> 16 ) & 0xFF ] << 16 ) ^
                     ( RSb[ ( Y2 >> 24 ) & 0xFF ] << 24 );

        X2 = *RK++ ^ ( RSb[ ( Y2       ) & 0xFF ]       ) ^
                     ( RSb[ ( Y1 >>  8 ) & 0xFF ] <<  8 ) ^
                     ( RSb[ ( Y0 >> 16 ) & 0xFF ] << 16 ) ^
                     ( RSb[ ( Y3 >> 24 ) & 0xFF ] << 24 );

        X3 = *RK++ ^ ( RSb[ ( Y3       ) & 0xFF ]       ) ^
                     ( RSb[ ( Y2 >>  8 ) & 0xFF ] <<  8 ) ^
                     ( RSb[ ( Y1 >> 16 ) & 0xFF ] << 16 ) ^
                     ( RSb[ ( Y0 >> 24 ) & 0xFF ] << 24 );
    }

    PUT_ULONG_LE( X0, output,  0 );
    PUT_ULONG_LE( X1, output,  4 );
    PUT_ULONG_LE( X2, output,  8 );
    PUT_ULONG_LE( X3, output, 12 );
}

int Secure_AES_ECB_Decrypt_SW(MS_U32 u32Addr, MS_U32 u32Len, MS_U8 *bKey)
{
    UBOOT_TRACE("IN\n");
    if(0 != u32Len%16)
    {
         UBOOT_ERROR("u32InLen should align 16\n");
         return -1;
    }
    aes_context aes_ctx;
    memset(&aes_ctx, 0, sizeof(aes_context));
    if(NULL == bKey)
    {
        // use hwKey to handle CustomerKeyBank
        UBOOT_DEBUG("Use SW EFUSE Key\n");
        aes_setkey_dec( &aes_ctx, hwKey, 128 );
    }
    else
    {
        aes_setkey_dec( &aes_ctx, bKey, 128 );
    }
    int offset;
    unsigned char *ptr = (unsigned char*)u32Addr;
    for( offset = 0; offset < u32Len; offset += 16 )
    {
        aes_crypt_ecb( &aes_ctx, AES_DECRYPT, ptr, ptr );
        ptr += 16;
    }
    UBOOT_TRACE("OK\n");
    return 0;
}
int Secure_AES_ECB_Encrypt_SW(MS_U32 u32Addr, MS_U32 u32Len, MS_U8 *bKey)
{
    UBOOT_TRACE("IN\n");
    if(0 != u32Len%16)
    {
         UBOOT_ERROR("u32InLen should align 16\n");
         return -1;
    }
    aes_context aes_ctx;
    memset(&aes_ctx, 0,sizeof(aes_context));
    if(NULL == bKey)
    {
        // use hwKey to handle CustomerKeyBank
        UBOOT_DEBUG("Use SW EFUSE Key\n");
        aes_setkey_enc( &aes_ctx, hwKey, 128 );
    }
    else
    {
        aes_setkey_enc( &aes_ctx, bKey, 128 );
    }
    int offset;
    unsigned char *ptr = (unsigned char*)u32Addr;
    for( offset = 0; offset < u32Len; offset += 16 )
    {
        aes_crypt_ecb( &aes_ctx, AES_ENCRYPT, ptr, ptr );
        ptr += 16;
    }
    UBOOT_TRACE("OK\n");
    return 0;
}


int Secure_AES_ECB_Decrypt(MS_U32 u32Addr, MS_U32 u32Len, MS_U8 *bKey)
{
    int ret=0;
    UBOOT_TRACE("IN\n");

    // To get optee status
    extern unsigned long gOpteeFailFlag;

    // In optee, fastcall function is available if gOpteeFailFlag is 0
    // In R2TEE, gOpteeFailFlag is always 0
    if( gOpteeFailFlag == 0 )
    {
        #if defined(CONFIG_SECURE_SMC_HW_IP)
        // use fastcall
        UBOOT_DEBUG("\n\nUSE Secure_AES_ECB_Decrypt_SMC_HW\n\n");
        Secure_AES_ECB_Decrypt_SMC_HW(u32Addr,u32Len,bKey);
        #elif defined(CONFIG_SECURE_HW_IP)
        // use HW driver
        UBOOT_DEBUG("\n\nUSE Secure_AES_ECB_Decrypt_HW\n\n");
        ret = Secure_AES_ECB_Decrypt_HW(u32Addr,u32Len,bKey);
        #else
        #if defined(CONFIG_MCU_ARM)
        // use SW solution
        UBOOT_DEBUG("\n\nUSE Secure_AES_ECB_Decrypt_SW\n\n");
        ret = Secure_AES_ECB_Decrypt_SW(u32Addr,u32Len,bKey);
        #else
        UBOOT_DEBUG("\n\nNot Support Secure_AES_ECB_Decrypt_SW\n\n");
        ret = -1;
        #endif // #if defined(CONFIG_MCU_ARM)
        #endif
    }
    else
    {
        // Only optee case here, if optee auth fail, use HW driver
        // if HW driver also fail, use SW slution.
        UBOOT_ERROR("\n\nOptee auth failed, use HW driver\n\n");

        #if defined(CONFIG_SECURE_HW_IP)
        ret = Secure_AES_ECB_Decrypt_HW(u32Addr,u32Len,bKey);
        if(ret!=0)
        {
            UBOOT_INFO("\n\nSecure_AES_ECB_Decrypt_HW fail, Use SW solution!!\n\n");
            #if defined(CONFIG_MCU_ARM)
            ret = Secure_AES_ECB_Decrypt_SW(u32Addr,u32Len,bKey);
            #else
            UBOOT_DEBUG("\n\nNot Support Secure_AES_ECB_Decrypt_SW\n\n");
            ret = -1;
            #endif // #if defined(CONFIG_MCU_ARM)
        }
        #else
        UBOOT_DEBUG("\n\nNot Support Secure_AES_ECB_Decrypt_HW\n\n");
        ret = -1;
        #endif //#if defined(CONFIG_SECURE_HW_IP)
    }

    if(ret==0)
    {
        UBOOT_TRACE("OK\n");
    }
    else
    {
        UBOOT_ERROR("AES Decrypt Error\n");
    }

    return ret;
}


int Secure_AES_ECB_Encrypt(MS_U32 u32Addr, MS_U32 u32Len, MS_U8 *bKey)
{
    int ret=0;
    UBOOT_TRACE("IN\n");
    #if defined(CONFIG_SECURE_HW_IP)
    //AESDMA
    ret = Secure_AES_ECB_Encrypt_HW(u32Addr,u32Len,bKey);
    #else
    //SW
    ret = Secure_AES_ECB_Encrypt_SW(u32Addr,u32Len,bKey);
    #endif
    if(ret==0)
    {
        UBOOT_TRACE("OK\n");
    }
    else
    {
        UBOOT_ERROR("AES Encrypt fail\n");
    }

    return ret;
}

void Secure_Get_DeviceKey(U8 *pu8HwKey)
{
    //use efuse to encrypt puCommonTaString
    const static U8 puCommonTaString[AES_KEY_LEN] = {'M', 'S', 't', 'a', 'r', 'C', 'o', 'm', 'm', 'o', 'n', 'T', 'A', 'K', 'e', 'y'};

    UBOOT_TRACE("IN\n");

    if(MDrv_SYS_Query(E_SYS_QUERY_SECURED_IC_SUPPORTED)==TRUE)
    {
        UBOOT_DEBUG("use Device key!!\n");
        memcpy(pu8HwKey,puCommonTaString,AES_KEY_LEN);
        Secure_AES_ECB_Encrypt((U32)pu8HwKey,AES_KEY_LEN,NULL);
        array_reverse((char*)pu8HwKey,AES_KEY_LEN);
    }
    else
    {
         UBOOT_DEBUG("use SW key!!\n");
         memcpy(pu8HwKey,hwKey,AES_KEY_LEN);
    }

    UBOOT_TRACE("OK\n");
}

#undef CRYPTO_AES_C
