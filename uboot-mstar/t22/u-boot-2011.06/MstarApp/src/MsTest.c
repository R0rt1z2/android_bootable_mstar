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


#include <exports.h>
#include <MsTypes.h>
#include <MsUboot.h>
#include <MsTest.h>
#include <linux/string.h>
#include <MsSystem.h>
#include <MsDebug.h>

#if defined(CONFIG_SECURITY_BOOT) && (defined(CONFIG_SECURE_HW_IP) || defined(CONFIG_SECURE_SMC_HW_IP))
#include <MsSecureBoot.h>
#include <secure/MsSecureCommon.h>
#include <secure/crypto_aes.h>
#include <secure/crypto_rsa.h>
#include <secure/crypto_sha.h>

// input data for AES decryption
static unsigned char inputDataAES[]={0x67, 0x1a, 0xac, 0xc1, 0xc6, 0xcd, 0xd3, 0xae, 0xd9, 0xa5, 0xe8, 0xc8, 0xd5, 
                                     0x51, 0xd8, 0xa0, 0xe0, 0x04, 0xfa, 0x37, 0x0f, 0x1b, 0x60, 0x3d, 0x98, 0x0d, 
                                     0xdd, 0xa3, 0xf4, 0x28, 0x26, 0x4b};
// input data for SHA
static unsigned char inputDataSHA[]={'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t',
                                     'u','v','w','x','y','z','a','a','a','a','a','a'};
// Hash value of inputDataSHA[]
static unsigned char inputDataHash[]={0x7c, 0x36, 0x8f, 0x04, 0x4c, 0x3a, 0x02, 0xe7, 0x48, 0xac, 0x97, 0xec, 0x94,
                                      0xdc, 0x36, 0x22, 0x3c, 0x49, 0xea, 0xa7, 0x13, 0x37, 0x93, 0xe2, 0xc7, 0xf1,
                                      0x29, 0x50, 0x1c, 0x42, 0x66, 0xbd};

// Hash value of rsaSig[] by RSA decryption 
static unsigned char shaHash[]={0x62, 0xA2, 0x2A, 0x79, 0x29, 0x04, 0x16, 0x8F, 0xAE, 0xF0, 0x13, 0x5B, 0x68, 0x9C,
                                0x3C, 0xB4, 0x32, 0x08, 0x8A, 0xEE, 0xEC, 0x71, 0xFD, 0x29, 0x8F, 0x64, 0x77, 0xFE,
                                0xAD, 0x75, 0xE0, 0x08};
// RSA encryption data
static U8 rsaSig[]={0x65, 0xd5, 0x2a, 0x5d, 0xc3, 0x44, 0xd9, 0x90, 0xba, 0x0d, 0x07, 0x37, 0xd1, 0xde, 0xa6, 0x32,
                    0x17, 0xee, 0x62, 0x58, 0xef, 0x18, 0xa1, 0x55, 0x9e, 0x75, 0x04, 0x44, 0x82, 0xa3, 0x61, 0xc0,
                    0xa7, 0x58, 0x40, 0xdf, 0x15, 0x97, 0xbf, 0x52, 0xe3, 0xc7, 0xb1, 0x39, 0x24, 0x69, 0x9f, 0x9d,
                    0x92, 0x10, 0xf5, 0xb4, 0x73, 0xf8, 0x71, 0xc3, 0xae, 0x77, 0x0e, 0xb2, 0x0c, 0xec, 0x61, 0xec,
                    0x2b, 0x2c, 0x36, 0x36, 0x97, 0x44, 0xa4, 0xdb, 0xc0, 0x7e, 0x5d, 0xd6, 0x57, 0xd8, 0x79, 0x2d,
                    0x3b, 0x05, 0xf3, 0xf0, 0xe0, 0x93, 0x9c, 0x16, 0x11, 0x51, 0xaf, 0x47, 0xa9, 0x23, 0xc4, 0x56,
                    0xe6, 0x88, 0x9b, 0xe3, 0x2e, 0xb0, 0xda, 0xeb, 0x20, 0x41, 0x12, 0x9e, 0x2f, 0xd5, 0x0a, 0xc2,
                    0x20, 0xc3, 0x04, 0x68, 0x49, 0x69, 0x91, 0x0e, 0x33, 0xde, 0x5a, 0xd6, 0x7d, 0xe1, 0x41, 0x8a,
                    0x13, 0x0a, 0x21, 0xcf, 0x2b, 0x46, 0x89, 0x28, 0xaf, 0xf3, 0x12, 0x45, 0xca, 0x92, 0xec, 0xe1,
                    0x4d, 0x7c, 0x9d, 0xd3, 0x61, 0x8a, 0x84, 0x0a, 0x0d, 0x93, 0x08, 0x01, 0xb2, 0x5c, 0xab, 0x69,
                    0x21, 0xd0, 0xe9, 0x7b, 0xd8, 0xdf, 0x3b, 0x40, 0x95, 0xc4, 0x87, 0xc1, 0x61, 0x37, 0xf6, 0x8b,
                    0xa4, 0xc0, 0xe7, 0x82, 0x52, 0x2c, 0x2a, 0x92, 0x7b, 0x22, 0xfe, 0xe7, 0x1f, 0xb5, 0x68, 0xb3,
                    0x1d, 0xa2, 0x82, 0x16, 0xaa, 0xd2, 0x32, 0x8b, 0x0d, 0x71, 0xb8, 0xe8, 0x4b, 0x8a, 0xd0, 0xa3,
                    0x26, 0x35, 0xca, 0x37, 0xfe, 0xd0, 0x8e, 0xcf, 0x41, 0x52, 0x86, 0x14, 0x80, 0xf5, 0xae, 0x59,
                    0x8a, 0x10, 0xb5, 0x09, 0xb0, 0x20, 0x5e, 0xbc, 0x56, 0xe5, 0xfe, 0x63, 0x76, 0x5e, 0x23, 0xc7,
                    0x58, 0x0e, 0xea, 0x19, 0xe2, 0xd5, 0xce, 0xfe, 0xfe, 0x0d, 0x80, 0xd4, 0x42, 0x6b, 0xc2, 0x10};

int verify_aes_driver(void)
{
    int i=0;
    int runTime=1;
    U8 u8Key[AES_KEY_LEN]="\0";
    unsigned long u32Len=sizeof(inputDataAES);
    unsigned char *tempinput;
    unsigned long executeTime=0;
    unsigned long initialTime=0;

    tempinput=malloc(sizeof(inputDataAES));
    memcpy(tempinput,inputDataAES,sizeof(inputDataAES));

    GetAESKey(E_AES_BOOT_KEY,u8Key);
    #if defined(CONFIG_SECURE_SMC_HW_IP)
    Secure_AES_ECB_Decrypt_SMC_HW((unsigned int)tempinput, u32Len, (U8 *)u8Key,SWKEY);
    #else
    Secure_AES_ECB_Decrypt_HW((unsigned int)tempinput, u32Len, (U8 *)u8Key);
    #endif
    if(memcmp((unsigned char*)inputDataSHA,tempinput,sizeof(inputDataAES))!=0)
    {
        UBOOT_DUMP((unsigned int)inputDataSHA, SHA256_DIGEST_SIZE);
        UBOOT_DUMP((unsigned int)tempinput, SHA256_DIGEST_SIZE);
        UBOOT_ERROR("Error: AES Decrypt Fail by AESBootKey!!\n");
        free(tempinput);
        return -1;
    }

    initialTime=MsSystemGetBootTime();
    while(i<runTime)
    {
    #if defined(CONFIG_SECURE_SMC_HW_IP)
        Secure_AES_ECB_Decrypt_SMC_HW((unsigned int)tempinput, u32Len, (U8 *)u8Key,SWKEY);
    #else
        Secure_AES_ECB_Decrypt_HW((unsigned int)tempinput, u32Len, (U8 *)u8Key);
    #endif
      i++;
    }
    executeTime=MsSystemGetBootTime()-initialTime;
    UBOOT_INFO("<AES Decrypt>\n");
    UBOOT_INFO("Data size: %d byte\n",sizeof(inputDataAES));
    UBOOT_INFO("Performance: %lu (ms) (total %d time)\n",executeTime,runTime);
  
    UBOOT_INFO("Result: pass by AESBootKey\n\n");

#if defined(CONFIG_SECURE_HW_IP)
    // Verify efuse key encrypt and decrypt
    memset(tempinput,'\0',sizeof(inputDataSHA));
    memcpy(tempinput,inputDataSHA,sizeof(inputDataSHA));
    Secure_AES_ECB_Encrypt_HW((unsigned int)tempinput, u32Len, NULL);
    if (memcmp(inputDataSHA,tempinput,sizeof(inputDataSHA))==0)
    {
        UBOOT_DUMP((unsigned int)inputDataSHA, SHA256_DIGEST_SIZE);
        UBOOT_DUMP((unsigned int)tempinput, SHA256_DIGEST_SIZE);
        UBOOT_INFO("Error: AES Encrypt fail by Efuse key!!");
        free(tempinput);
        return -1;
    }
    
    Secure_AES_ECB_Decrypt_HW((unsigned int)tempinput, u32Len, NULL); 
    if(memcmp((unsigned char*)inputDataSHA,tempinput,sizeof(inputDataSHA))!=0)
    {
        UBOOT_DUMP((unsigned int)inputDataSHA, SHA256_DIGEST_SIZE);
        UBOOT_DUMP((unsigned int)tempinput, SHA256_DIGEST_SIZE);
        UBOOT_ERROR("Error: AES Decrypt Fail!!\n");
        free(tempinput);
        return -1;
    }
    UBOOT_INFO("<AES Encrypt and Decrypt>\n");
    UBOOT_INFO("Result: pass by Efuse Key\n\n");
#endif

    
    free(tempinput);  
    return 0; 
}

int verify_sha_driver(void)
{
    int i=0;
    int runTime=1;
    unsigned char *tempu8AuthData=NULL;
    unsigned char *u8AuthData=NULL;
    unsigned char *digest=NULL;
    unsigned char tempdigest[SHA256_DIGEST_SIZE+16]= "\0";
    unsigned long u32AuthDataLen=sizeof(tempdigest);
    unsigned long executeTime=0;
    unsigned long initialTime=MsSystemGetBootTime();
    unsigned int *reverseBuf=NULL;
    

    if( (MS_U32)tempdigest % 0x10 != 0)
        digest = (MS_U32)tempdigest + (0x10-((MS_U32)tempdigest % 0x10));
    else
        digest = tempdigest;

    tempu8AuthData=malloc(u32AuthDataLen);
    if(tempu8AuthData==NULL)
    {
        UBOOT_ERROR("Error: malloc SHA input buffer Fail!!\n");
        return -1;
    }
    if( (MS_U32)tempu8AuthData % 0x10 != 0)
        u8AuthData = (MS_U32)tempu8AuthData + (0x10-((MS_U32)tempu8AuthData % 0x10));
    else
        u8AuthData = tempu8AuthData;

    memcpy(u8AuthData,&inputDataSHA[0],sizeof(inputDataSHA));
    while(i<runTime)
    {

        CommonSHA((unsigned int)u8AuthData, (unsigned int)digest,32);
        i++;
    }

    free(tempu8AuthData);
    executeTime=MsSystemGetBootTime()-initialTime;
    UBOOT_INFO("<SHA Decrypt>\n");
    UBOOT_INFO("Data size: %d byte\n",sizeof(inputDataSHA));
    UBOOT_INFO("Performance: %lu (ms) per time (total %d time)\n",executeTime,runTime);  

    if(memcmp(inputDataHash,digest,sizeof(digest))!=0)
    {
        UBOOT_DUMP((unsigned int)inputDataHash, SHA256_DIGEST_SIZE);
        UBOOT_DUMP((unsigned int)digest, SHA256_DIGEST_SIZE);
        UBOOT_ERROR("Error: SHA256 Authentication Fail!!\n");
        return -1;
    }
    UBOOT_INFO("Result: pass\n\n");
  return 0;
}

int verify_rsa_driver(void)
{
    int i=0;
    int runTime=1;
    U8 *u8Signature=&rsaSig[0];
    unsigned char signature[RSA_KEY_DIGI_LEN*4] = "\0";
    U8 u8ImagePublicKeyN[RSA_PUBLIC_KEY_N_LEN]="\0";
    U8 u8ImagePublicKeyE[RSA_PUBLIC_KEY_E_LEN]="\0";
    unsigned long executeTime=0;
    unsigned long initialTime=0;  
    GetPublicKeyN(E_RSA_IMAGE_PUBLIC_KEY, u8ImagePublicKeyN, sizeof(u8ImagePublicKeyN));
    GetPublicKeyE(E_RSA_IMAGE_PUBLIC_KEY, u8ImagePublicKeyE, sizeof(u8ImagePublicKeyE));
    initialTime=MsSystemGetBootTime();
    unsigned int *reverseBuf=NULL;
    
    while(i<runTime)  
    {
        rsa_main(u8Signature, u8ImagePublicKeyN, u8ImagePublicKeyE, signature);
        i++;
    }

    executeTime=MsSystemGetBootTime()-initialTime;
    UBOOT_INFO("<RSA Decrypt>\n");
    UBOOT_INFO("Data size: %d byte\n",sizeof(rsaSig));
    UBOOT_INFO("Performance: %lu (ms) per time (total %d time)\n",executeTime,runTime);  

    if(memcmp(signature+224,shaHash,sizeof(shaHash))!=0)
    {
        UBOOT_DUMP((unsigned int)signature, 0x100);
        UBOOT_DUMP((unsigned int)shaHash, SHA256_DIGEST_SIZE);
        UBOOT_ERROR("Error: RSA Authentication Fail!!\n");
        return -1;
    }
    UBOOT_INFO("Result: pass\n\n");
    return 0;
}


int do_secure_driver_check(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    verify_aes_driver();
    verify_sha_driver();
    verify_rsa_driver();
    
    return 0;
}
#endif

int i_adding(void)
{
    int i=0;

    i++;
    i++;
    i++;
    i++;
    i++;
    return i;
}

// for ReadWrite Testing
int miu0_write_test( void ){
    int err=0;
#if (ENABLE_MODULE_MS_TEST == 1)
    int tsize = 64*0x100000 ;
    //int i, seed, r, err ;

    unsigned int i=0, seed=0,r=0;
    volatile unsigned char* vp ;

    seed = 0x183687 ;
    err = 0 ;

    vp = (volatile unsigned char*)0x84000000 ;
    printf( "test %d MB from 0x%08X\n", tsize/0x100000, (unsigned int)vp ) ;

    r = seed ;
    for( i=0; i<tsize; i++ ){
        vp[i] = (unsigned char)r ;
        r = 110017 + 2360483*r ;
    }
    r = seed ;
    for( i=0; i<tsize; i++ ){
        if( vp[i]!=(r&0xFF) ){
            printf( "#1 error idx=%d, vp[i]=0x%02X, r=0x%02X\n", i, vp[i], (r&0xFF) ) ;
            printf( "#2 error idx=%d, vp[i]=0x%02X, r=0x%02X\n", i, vp[i], (r&0xFF) ) ;
            printf( "#3 error idx=%d, vp[i]=0x%02X, r=0x%02X\n", i, vp[i], (r&0xFF) ) ;
            printf( "#4 error idx=%d, vp[i]=0x%02X, r=0x%02X\n", i, vp[i], (r&0xFF) ) ;
            err = 1 ;
        }
        r = 110017 + 2360483*r ;
    }

    vp = (volatile unsigned char*)0x84000000 ;
    printf( "test %d MB from 0x%08X\n", tsize/0x100000, (unsigned int)vp ) ;

    r = seed ;
    for( i=0; i<tsize; i++ ){
        vp[i] = (unsigned char)r ;
        r = 110017 + 2360483*r ;
    }
    r = seed ;
    for( i=0; i<tsize; i++ ){
        if( vp[i]!=(r&0xFF) ){
            printf( "#1 error idx=%d, vp[i]=0x%02X, r=0x%02X\n", i, vp[i], (r&0xFF) ) ;
            printf( "#2 error idx=%d, vp[i]=0x%02X, r=0x%02X\n", i, vp[i], (r&0xFF) ) ;
            printf( "#3 error idx=%d, vp[i]=0x%02X, r=0x%02X\n", i, vp[i], (r&0xFF) ) ;
            printf( "#4 error idx=%d, vp[i]=0x%02X, r=0x%02X\n", i, vp[i], (r&0xFF) ) ;
            err = 1 ;
        }
        r = 110017 + 2360483*r ;
    }

    r = 0xFF ;
    for( i=0; i<tsize; i++ ){
        vp[i] = (unsigned char)r ;
    }
    for( i=0; i<tsize; i++ ){
        if( vp[i]!=(r&0xFF) ){
            printf( "#1 error idx=%d, vp[i]=0x%02X, r=0x%02X\n", i, vp[i], (r&0xFF) ) ;
            printf( "#2 error idx=%d, vp[i]=0x%02X, r=0x%02X\n", i, vp[i], (r&0xFF) ) ;
            printf( "#3 error idx=%d, vp[i]=0x%02X, r=0x%02X\n", i, vp[i], (r&0xFF) ) ;
            printf( "#4 error idx=%d, vp[i]=0x%02X, r=0x%02X\n", i, vp[i], (r&0xFF) ) ;
            err = 1 ;
        }
    }

    r = 0x00 ;
    for( i=0; i<tsize; i++ ){
        vp[i] = (unsigned char)r ;
    }
    for( i=0; i<tsize; i++ ){
        if( vp[i]!=(r&0xFF) ){
            printf( "#1 error idx=%d, vp[i]=0x%02X, r=0x%02X\n", i, vp[i], (r&0xFF) ) ;
            printf( "#2 error idx=%d, vp[i]=0x%02X, r=0x%02X\n", i, vp[i], (r&0xFF) ) ;
            printf( "#3 error idx=%d, vp[i]=0x%02X, r=0x%02X\n", i, vp[i], (r&0xFF) ) ;
            printf( "#4 error idx=%d, vp[i]=0x%02X, r=0x%02X\n", i, vp[i], (r&0xFF) ) ;
            err = 1 ;
        }
    }
#endif
    return(!err) ;
}

// for ReadWrite Testing
int miu0_write_test_cache_first( void ){
    int err=0;
#if (ENABLE_MODULE_MS_TEST == 1)
    int tsize = 64*0x100000 ;
    //int i, seed, r, err ;

    unsigned int i=0, seed=0,r=0;
    volatile unsigned char* vp ;

    seed = 0x183687 ;
    err = 0 ;

    vp = (volatile unsigned char*)0x84000000 ;
    printf( "test %d MB from 0x%08X\n", tsize/0x100000, (unsigned int)vp ) ;

    r = seed ;
    for( i=0; i<tsize; i++ ){
        //vp[i] = r ;
        vp[i] = (unsigned char)r ;
        r = 110017 + 2360483*r ;
    }
    r = seed ;
    for( i=0; i<tsize; i++ ){
        if( vp[i]!=(r&0xFF) ){
            printf( "#1 error idx=%d, vp[i]=0x%02X, r=0x%02X\n", i, vp[i], (r&0xFF) ) ;
            printf( "#2 error idx=%d, vp[i]=0x%02X, r=0x%02X\n", i, vp[i], (r&0xFF) ) ;
            printf( "#3 error idx=%d, vp[i]=0x%02X, r=0x%02X\n", i, vp[i], (r&0xFF) ) ;
            printf( "#4 error idx=%d, vp[i]=0x%02X, r=0x%02X\n", i, vp[i], (r&0xFF) ) ;
            err = 1 ;
        }
        r = 110017 + 2360483*r ;
    }

    vp = (volatile unsigned char*)0x84000000 ;
    printf( "test %d MB from 0x%08X\n", tsize/0x100000, (unsigned int)vp ) ;

    r = seed ;
    for( i=0; i<tsize; i++ ){
        vp[i] = (unsigned char)r ;
        r = 110017 + 2360483*r ;
    }
    r = seed ;
    for( i=0; i<tsize; i++ ){
        if( vp[i]!=(r&0xFF) ){
            printf( "#1 error idx=%d, vp[i]=0x%02X, r=0x%02X\n", i, vp[i], (r&0xFF) ) ;
            printf( "#2 error idx=%d, vp[i]=0x%02X, r=0x%02X\n", i, vp[i], (r&0xFF) ) ;
            printf( "#3 error idx=%d, vp[i]=0x%02X, r=0x%02X\n", i, vp[i], (r&0xFF) ) ;
            printf( "#4 error idx=%d, vp[i]=0x%02X, r=0x%02X\n", i, vp[i], (r&0xFF) ) ;
            err = 1 ;
        }
        r = 110017 + 2360483*r ;
    }

    r = 0xFF ;
    for( i=0; i<tsize; i++ ){
        vp[i] = (unsigned char)r ;
    }
    for( i=0; i<tsize; i++ ){
        if( vp[i]!=(r&0xFF) ){
            printf( "#1 error idx=%d, vp[i]=0x%02X, r=0x%02X\n", i, vp[i], (r&0xFF) ) ;
            printf( "#2 error idx=%d, vp[i]=0x%02X, r=0x%02X\n", i, vp[i], (r&0xFF) ) ;
            printf( "#3 error idx=%d, vp[i]=0x%02X, r=0x%02X\n", i, vp[i], (r&0xFF) ) ;
            printf( "#4 error idx=%d, vp[i]=0x%02X, r=0x%02X\n", i, vp[i], (r&0xFF) ) ;
            err = 1 ;
        }
    }

    r = 0x00 ;
    for( i=0; i<tsize; i++ ){
        vp[i] = (unsigned char)r ;
    }
    for( i=0; i<tsize; i++ ){
        if( vp[i]!=(r&0xFF) ){
            printf( "#1 error idx=%d, vp[i]=0x%02X, r=0x%02X\n", i, vp[i], (r&0xFF) ) ;
            printf( "#2 error idx=%d, vp[i]=0x%02X, r=0x%02X\n", i, vp[i], (r&0xFF) ) ;
            printf( "#3 error idx=%d, vp[i]=0x%02X, r=0x%02X\n", i, vp[i], (r&0xFF) ) ;
            printf( "#4 error idx=%d, vp[i]=0x%02X, r=0x%02X\n", i, vp[i], (r&0xFF) ) ;
            err = 1 ;
        }
    }
#endif
    return(!err) ;
}

// for ReadWrite Testing
void do_rw_test( char arg, int loop ){
#if (ENABLE_MODULE_MS_TEST == 1)
    int i, j ;

    for( j=0; j<loop; j++ ){
        switch(arg){
        case '0':
            i = i_adding() ;
            printf("i_adding = %d\n", i) ;
            break ;
        case '1':
            i = miu0_write_test() ;
            printf("MIU0 write and read testing = %s\n", i?"Pass":"Fail") ;
            break ;
        case '2':
            i = miu0_write_test_cache_first() ;
            printf("MIU0 write and read testing cache first = %s\n", i?"Pass":"Fail") ;
            break ;
        default:
            printf( "not defined test case\n" ) ;
            return ;
            break ;
        }
    }
#endif
}

void boot_rw_test(int argc, char * const argv[])
{
#if (ENABLE_MODULE_MS_TEST == 1)
        if( argc==3 )
            do_rw_test( argv[1][1], simple_strtoul(argv[2], NULL, 10) ) ;
        else
            do_rw_test( argv[1][1], 1 ) ;
#endif
}

int do_msg (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
//    printf("\n##############################\n");
    if( argc > 0 )
       printf("\n\033[0;34m##  Mstar : %s : ##\033[0m \n",argv[1]);
    else
       printf("\n##  Mstar ##\n");
  //  printf("##############################\n");
    return 0;
}
#ifdef CONFIG_DISPLAY_VERSIONINFO
int mversion (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    printf ("\nChangelist:\t%s\n", CHANGELIST_STRING);
    return (0);
}
#endif

int do_gettime(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
#if (ENABLE_MODULE_MS_TEST == 1)
  static unsigned long preTime = 0;
  unsigned long executeTime = 0;
  unsigned long currentTime = MsSystemGetBootTime();

  if (preTime == 0)
  	executeTime = 0;
  else
  	executeTime = currentTime - preTime;
  
  printf("[executeTime:%lu] [currentTime:%lu]\n", executeTime,currentTime);
  preTime=currentTime;
#endif
  return 0;
} 

int do_cmdperformancetest(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
#if (ENABLE_MODULE_MS_TEST == 1)
    int ret=0;
    unsigned long preTime=0;
    unsigned long currentTime=0;
        
    if(argc > 1)
    {
        preTime=MsSystemGetUSTime();
        ret=run_command(argv[1],0);
        currentTime=MsSystemGetUSTime();
        if (ret!=0)
        {
            printf("no \"%s\" command\n",argv[1]);
            return -1;
        }

        printf("\033[0;35m [command] %s [performance] %lu us\033[0m\n",argv[1],(currentTime-preTime));
    }
#endif
    return 0;
}


