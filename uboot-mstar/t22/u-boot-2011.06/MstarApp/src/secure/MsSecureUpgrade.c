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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    cmd_secure.c
/// @brief  SCS Main Function
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include <common.h>
#include <command.h>
#include <MsTypes.h>
#include <secure/MsSecureBoot.h>
#include <secure/MsSecureUpgrade.h>
#include <secure/MsSecureCommon.h>
#include <secure/crypto_rsa.h>
#include <secure/crypto_sha.h>
#include <secure/crypto_func.h>
#include <secure/crypto_aes.h>
#include <secure/crypto_auth.h>
#include <secure/apiSecureBoot.h>
#include <MsVfs.h>
#include <MsSystem.h>
#include <exports.h>
#include <ShareType.h>
#include <MsDebug.h>
#include <MsUtility.h>
#include <MsUpgradeUtility.h>
#include <MsEnvironment.h>
#include <MsUpgrade.h>

//-------------------------------------------------------------------------------------------------
//  Debug
//-------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define DEFAULT_SEGMENT_SIZE 8192
#define CERTIFICATE_LOCATION        "certificate"
#define RSA_PUBLIC_KEY_PATH "/RSAPublicKeyAPP.bin"
#define AES_KEY_PATH "/Kcust.bin"
#define UPGRADE_IMAGE_BUFFER_ADDRESS 0x20280000
#define UPGRADE_IMAGE_FRAGMENT_SIZE 0x200000

//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
#ifdef CONFIG_UPGRADE_WITH_FAIL_HANDLER
extern ST_IMAGE_OFFSET_INFO g_st_image_offset_info;
extern ST_CHUNK_INFO g_st_REE_chunk_info;
extern ST_CHUNK_INFO g_st_TEE_chunk_info;
#endif

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static unsigned char aesKey[0x10];
static unsigned int segmentSize=0;
static unsigned char fileSizeReady=FALSE;
static unsigned int gfileSize=0;

static const MS_U8 RSA_Padding[] = {
        0x00, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x30, 0x31, 0x30,
        0x0D, 0x06, 0x09, 0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x02, 0x01, 0x05, 0x00, 0x04, 0x20,
    };

//-------------------------------------------------------------------------------------------------
//  extern function
//-------------------------------------------------------------------------------------------------
extern int snprintf(char *str, size_t size, const char *fmt, ...);

//-------------------------------------------------------------------------------------------------
//  inline
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local function
//-------------------------------------------------------------------------------------------------
U32 Secure_SegmentSize (void);
int GetfileSizeforAESUsbUpgrade(const char *Interface,const char *device,char *file);

/// chunk file size in all-in-one segment rsa mode
#define SEGMENT_RSA_CHUNK_FILE_SIZE 128
#define SIGNATURE_SIZE 256
#define SHA256_DIGEST_SIZE (256/8)

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

#if defined(CONFIG_UPGRADE_WITHOUT_FAIL_HANDLER)
int do_file_segment_rsa_authendication(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    //"filSegRSA usb %d:%d %s",device, partition, upgradeFile);
    #define TARGET_INTERFACE argv[1]
    #define TARGET_DEVICE  argv[2]
    #define TARGET_FILE  argv[3]
    /// chunk file size in all-in-one segment rsa mode
    #define SEGMENT_RSA_CHUNK_FILE_SIZE 128
    #define SEGMENT_RSA_CHUNK_SIGN_SIZE 256
    /// CHUNK file's start signature
    #define CHUNK_ID "MSTAR..."
    /// CHUNK file's end signature
    #define CHUNK_END "...mstar"
    #define RSA_PUBLIC_KEN_N_LEN 256
    #define RSA_PUBLIC_KEN_E_LEN 4
    #undef RSA_PUBLIC_KEY_LEN
    #define RSA_PUBLIC_KEY_LEN          (RSA_PUBLIC_KEN_N_LEN+RSA_PUBLIC_KEN_E_LEN)
    #define SHA256_DIGEST_SIZE ( 256 / 8)
    
    unsigned int i=0;
    unsigned int fileDataOffset=0;
    unsigned int fileDataLen=0;
    unsigned int fileHashOffset=0;
    unsigned int fileHashLen=0;
    unsigned int fileSignatureOffset=0;
    unsigned int fileSignatureLen=0;
    unsigned int *pReadChunk=NULL;
    char buffer[CMD_BUF]="\0";
    int ret=0;
    unsigned int readLen=0;
    unsigned char rsaOutBuf[RSA_KEY_DIGI_LEN*4+15]="\0";
    unsigned char rsaPublicKeyN[RSA_PUBLIC_KEN_N_LEN]="\0";
    unsigned char rsaPublicKeyE[RSA_PUBLIC_KEN_E_LEN]="\0";
    unsigned char hashOutBuf[SHA256_DIGEST_SIZE+15]="\0";

    unsigned char *pt=NULL;
    unsigned char chunkFileBuf[SEGMENT_RSA_CHUNK_FILE_SIZE+15]="\0";
    unsigned char chunkFileRSABuf[SEGMENT_RSA_CHUNK_SIGN_SIZE+15]="\0";
    unsigned int  *segmentBuf = NULL;
    int checkSegNums = 0;

    // for aligned 16
    unsigned char *ptrChunkFileBuf = ((MS_U32)chunkFileBuf+15) & ~0x0F;
    unsigned char *ptrChunkFileRSABuf = ((MS_U32)chunkFileRSABuf+15) & ~0x0F;
    unsigned char *ptrHashOutBuf = ((MS_U32)hashOutBuf+15) & ~0x0F;
    unsigned char *ptrRsaOutBuf = ((MS_U32)rsaOutBuf+15) & ~0x0F;
    unsigned int  *ptrSegmentBuf = NULL;
   
    UBOOT_TRACE("IN\n");
    if(argc<4)
        return -1;

    ret=GetfileSizeforAESUsbUpgrade(TARGET_INTERFACE,TARGET_DEVICE,TARGET_FILE);
    if(ret<0)
    {
        UBOOT_ERROR("GetfileSizeforAESUsbUpgrade fail\n");
        return -1;
    }

    //Get rsa upgrade public key
    ret = GetPublicKeyN(E_RSA_UPGRADE_PUBLIC_KEY,rsaPublicKeyN,RSA_PUBLIC_KEN_N_LEN);
    if(ret!=0)
    {
        UBOOT_ERROR("Get public key N fail\n");
        return -1;
    }
    ret = GetPublicKeyE(E_RSA_UPGRADE_PUBLIC_KEY,rsaPublicKeyE,RSA_PUBLIC_KEN_E_LEN);
    if(ret!=0)
    {
        UBOOT_ERROR("Get public key E fail\n");
        return -1;
    }

    //Read last segment , because we want to get chunk file.
    snprintf(buffer, CMD_BUF, "filepartloadSegAES %s %s %x %s %x %x", TARGET_INTERFACE, TARGET_DEVICE, (U32)ptrChunkFileBuf, TARGET_FILE, gfileSize-SEGMENT_RSA_CHUNK_FILE_SIZE, SEGMENT_RSA_CHUNK_FILE_SIZE);
    if(run_command(buffer, 0)!=0)
    {
        UBOOT_ERROR("Cmd:'%s' fail!!\n",buffer);
        return -1;
    }
    
    if(memcmp((const void *)((U32)ptrChunkFileBuf),CHUNK_ID,sizeof(CHUNK_ID))!=0){
        UBOOT_ERROR("[ERROR] CHUNK_ID compare fail\n");
        return -1;
    }

    if(memcmp((const void *)((U32)ptrChunkFileBuf+SEGMENT_RSA_CHUNK_FILE_SIZE-strlen(CHUNK_END)),CHUNK_END,strlen(CHUNK_END))!=0){
        UBOOT_ERROR("[ERROR] CHUNK_END compare fail\n");
        return -1;
    }

#if defined(CONFIG_CHUNK_VERIFY)
    UBOOT_INFO("Do Chunk Signature Verify...\n");
    if(CommonSHA((U32)ptrChunkFileBuf,(MS_U32)ptrHashOutBuf, SEGMENT_RSA_CHUNK_FILE_SIZE)!=0) {
        UBOOT_ERROR("CommonSHA Fail!!\n");
        return -1;
    }

    //Read last-1 segment (chunkid signature)
    snprintf(buffer, CMD_BUF, "filepartloadSegAES %s %s %x %s %x %x",
        TARGET_INTERFACE, TARGET_DEVICE, (U32)ptrChunkFileRSABuf, TARGET_FILE, gfileSize-(SEGMENT_RSA_CHUNK_SIGN_SIZE+SEGMENT_RSA_CHUNK_FILE_SIZE), SEGMENT_RSA_CHUNK_SIGN_SIZE);
    if(run_command(buffer, 0)!=0)
    {
        UBOOT_ERROR("Cmd:'%s' fail!!\n",buffer);
        return -1;
    }

    rsa_main((unsigned char *)ptrChunkFileRSABuf,rsaPublicKeyN,rsaPublicKeyE,ptrRsaOutBuf);

    UBOOT_DEBUG("Compare Chunk Signature...\n");
    for(i = 0; i<SHA256_DIGEST_SIZE; i++)
    {
        if(ptrHashOutBuf[i] != ptrRsaOutBuf[i+224])
        {
           UBOOT_ERROR("ptrHashOutBuf[%d]=0x%x, ptrRsaOutBuf[%d]=0x%x \n",i,ptrHashOutBuf[i],i,ptrRsaOutBuf[i+224]);
           UBOOT_DUMP((unsigned int)ptrRsaOutBuf, SHA256_DIGEST_SIZE);
           UBOOT_DUMP((unsigned int)ptrHashOutBuf, SHA256_DIGEST_SIZE);
           return -1;
        }
    }
#endif
    //Get hash, signature infor from chunk data
    pReadChunk=(unsigned int *)((U32)ptrChunkFileBuf);
    pReadChunk+=2;
    segmentSize=*pReadChunk;
    pReadChunk+=1;
    fileDataOffset=*pReadChunk;
    pReadChunk++;
    fileDataLen=*pReadChunk;
    pReadChunk++;

    fileHashOffset=*pReadChunk;
    pReadChunk++;
    fileHashLen=*pReadChunk;
    pReadChunk++;
    fileSignatureOffset=*pReadChunk;
    pReadChunk++;
    fileSignatureLen=*pReadChunk;
    pReadChunk++;
    
    UBOOT_DEBUG("segment size=0x%x\n",segmentSize);
    UBOOT_DEBUG("fileDataOffset=0x%x\n",fileDataOffset);
    UBOOT_DEBUG("fileDataLen=0x%x\n",fileDataLen);
    UBOOT_DEBUG("fileHashOffset=0x%x\n",fileHashOffset);
    UBOOT_DEBUG("fileHashLen=0x%x\n",fileHashLen);
    UBOOT_DEBUG("fileSignatureOffset=0x%x\n",fileSignatureOffset);
    UBOOT_DEBUG("fileSignatureLen=0x%x\n",fileSignatureLen);

    // Check whether chunk info is valid or not
    UBOOT_INFO("Do Chunk Info Verify...\n");
    if ( (segmentSize > 0) && (fileDataLen > 0) && (fileHashLen > 0) )
    {
        if((fileDataLen%segmentSize)==0)
            checkSegNums  = fileDataLen/segmentSize;
        else
            checkSegNums  = fileDataLen/segmentSize+1;

        // Calculate the fileHashLen, fileHashLen = checkSegNums * (256/8)
        if((checkSegNums*32) != fileHashLen)
        {
            UBOOT_ERROR("The parameter in chunk info is not valid");
            return -1;
        }
    }
    else
    {
        UBOOT_ERROR("The parameter in chunk info is not valid");
        return -1;
    }

    segmentBuf=(unsigned int*)malloc(segmentSize+15);

    if(segmentBuf==NULL)
    {
        UBOOT_ERROR("[ERROR] segmentBuf memroy allocate fail\n");
        return -1;        
    }

    ptrSegmentBuf = ((MS_U32)segmentBuf+15) & ~0x0F;

    //Read ***.hash.signature, and then do rsa decod
    memset(buffer,0,CMD_BUF);
    snprintf(buffer, CMD_BUF, "filepartloadSegAES %s %s %x %s %x %x", TARGET_INTERFACE,TARGET_DEVICE,(U32)ptrSegmentBuf, TARGET_FILE, fileSignatureOffset, fileSignatureLen);
    if(run_command(buffer, 0)!=0)
    {          
        UBOOT_ERROR("Cmd:'%s' fail!!\n",buffer);
        free(segmentBuf);
        return -1;
    }

    // clean buffer for next round of auth.
    memset(rsaOutBuf, 0, RSA_KEY_DIGI_LEN*4);
    memset(hashOutBuf, 0, SHA256_DIGEST_SIZE+15);
    rsa_main((unsigned char *)ptrSegmentBuf,rsaPublicKeyN,rsaPublicKeyE,ptrRsaOutBuf);

    memset(buffer,0,CMD_BUF);        
    snprintf(buffer, CMD_BUF, "filepartloadSegAES %s %s %x %s %x %x", TARGET_INTERFACE,TARGET_DEVICE,(U32)ptrSegmentBuf, TARGET_FILE, fileHashOffset, fileHashLen);
    if(run_command(buffer, 0)!=0)
    {
        UBOOT_ERROR("Cmd:'%s' fail!!\n",buffer);    
        free(segmentBuf);
        return -1;
    }

    if(CommonSHA((U32)ptrSegmentBuf,(MS_U32)ptrHashOutBuf,(MS_U32)fileHashLen)!=0)
    {
        UBOOT_ERROR("CommonSHA Fail!!\n");    
        free(segmentBuf);
        return -1;
    }

    for(i = 0; i<SHA256_DIGEST_SIZE; i++)
    {
        if(ptrHashOutBuf[i] != ptrRsaOutBuf[i+224])
        {
           UBOOT_ERROR("ptrHashOutBuf[%d]=0x%x, ptrRsaOutBuf[%d]=0x%x \n",i,ptrHashOutBuf[i],i,ptrRsaOutBuf[i+224]);
           UBOOT_DUMP((unsigned int)ptrRsaOutBuf, SHA256_DIGEST_SIZE);
           UBOOT_DUMP((unsigned int)ptrHashOutBuf, SHA256_DIGEST_SIZE);
           free(segmentBuf);
           return -1;
        }
    }

    if( memcmp(RSA_Padding, ptrRsaOutBuf, sizeof(RSA_Padding)) != 0 )
    {
        UBOOT_ERROR("RSA Padding is not matched\n");
        UBOOT_DUMP((unsigned int)RSA_Padding, 32);
        UBOOT_DUMP((unsigned int)ptrRsaOutBuf, 32);
        free(segmentBuf);
        return -1;
    }

    // Do rsa authentication for ***.hash
    UBOOT_INFO("Segment RSA Authentication ....\n");
    while(fileDataLen)
    {
        UBOOT_INFO("*");
        readLen=(fileDataLen>=segmentSize)?segmentSize:fileDataLen;

        memset(buffer,0,CMD_BUF);
        snprintf(buffer, CMD_BUF, "filepartloadSegAES %s %s %x %s %x %x", TARGET_INTERFACE,TARGET_DEVICE,(U32)ptrSegmentBuf, TARGET_FILE, fileDataOffset, readLen);
        if(run_command(buffer, 0)!=0)
        {
            UBOOT_ERROR("Cmd:'%s' fail!!\n",buffer);  
            free(segmentBuf);
            return -1;
        }


        if(CommonSHA((U32)ptrSegmentBuf,(MS_U32)ptrHashOutBuf,(MS_U32)readLen)!=0)
        {
            UBOOT_ERROR("CommonSHA Fail!!\n"); 
            free(segmentBuf);
            return -1;
        }

        memset(buffer,0,CMD_BUF);
        snprintf(buffer, CMD_BUF, "filepartloadSegAES %s %s %x %s %x %x", TARGET_INTERFACE,TARGET_DEVICE,(U32)ptrSegmentBuf, TARGET_FILE, fileHashOffset, SHA256_DIGEST_SIZE*2);
        if(run_command(buffer, 0)!=0)
        {
            UBOOT_ERROR("Cmd:'%s' fail!!\n",buffer);  
            free(segmentBuf);
            return -1;
        }

        pt=(unsigned char *)(ptrSegmentBuf);//+SHA256_DIGEST_SIZE-1);
        UBOOT_DUMP((unsigned int)pt, SHA256_DIGEST_SIZE);
        UBOOT_DUMP((unsigned int)ptrHashOutBuf, SHA256_DIGEST_SIZE);
        for(i = 0; i<SHA256_DIGEST_SIZE; i++)
        {
            if(ptrHashOutBuf[i] != *pt)
             {
                UBOOT_ERROR("ptrHashOutBuf[%d]=0x%x, *pt=0x%x \n",i,ptrHashOutBuf[i],*pt);
                free(segmentBuf);
                return -1;
             }
             pt++;
        }

        fileDataLen-=readLen;
        fileDataOffset+=readLen;
        fileHashOffset+=SHA256_DIGEST_SIZE;

        UBOOT_DEBUG("readLen=0x%x\n",readLen);
        UBOOT_DEBUG("fileDataLen=0x%x\n",fileDataLen);
        UBOOT_DEBUG("fileDataOffset=0x%x\n",fileDataOffset);
        UBOOT_DEBUG("fileHashOffset=0x%x\n",fileHashOffset);
    }

    free(segmentBuf);
    UBOOT_TRACE("OK\n");
    return 0;             
}

int do_file_part_load_with_segment_aes_decrypted(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    #define DOWNLOAD_INTERFACE argv[1]
    #define DOWNLOAD_DEVICE argv[2]
    #define DOWNLOAD_ADDR argv[3]
    #define DOWNLOAD_FILE argv[4]
    #define DOWNLOAD_OFFSET argv[5]
    #define DOWNLOAD_LENGTH argv[6]
        
    unsigned int downloadOffset=0;
    unsigned int downloadLen=0;
    unsigned int pdownloadAddr=0;

    unsigned int readSegmentAddr=0;
    unsigned int readOffsetInSeg=0;
    unsigned int readLen=0;
    unsigned int aesDecodeLen=0;
    unsigned int segmentNum=0;
    unsigned int _segmentSize=0;
    unsigned int u32HwAesBufAddr=0;     
    char buffer[CMD_BUF]="\0";
    SECUREBOOT_MbxRetStruct SecureBootMbxRetStruct;
    int aes_result=0;
    SECUREBOOT_Result secureBootResult;
    U32 *temp_u32HwAesBufAddr = NULL;
    UBOOT_TRACE("IN\n");

    if(argc<7)
    {
        return -1;
    }

    downloadOffset = (unsigned int)simple_strtoul(DOWNLOAD_OFFSET, NULL, 16);
    downloadLen = (unsigned int)simple_strtoul(DOWNLOAD_LENGTH, NULL, 16);
    pdownloadAddr = (unsigned int)simple_strtoul(DOWNLOAD_ADDR, NULL, 16);

    //Get total file size
    if(GetfileSizeforAESUsbUpgrade(DOWNLOAD_INTERFACE,DOWNLOAD_DEVICE,DOWNLOAD_FILE)<0)
    {
        UBOOT_ERROR("GetfileSizeforAESUsbUpgrade fail\n");
        return -1;
    }

    // get _segmentSize and segmentNum
    if(segmentSize==0)
    {
        _segmentSize=DEFAULT_SEGMENT_SIZE;
    }
    else
    {
        _segmentSize=segmentSize;
    }
    segmentNum=downloadOffset/_segmentSize;

    //Get AES decode key
    if(IsHouseKeepingBootingMode()==FALSE)
    {
        Secure_MailBox_ASSIGN_DECRYPTED_KEY(&SecureBootMbxRetStruct,E_AES_UPGRADE_KEY,AES_KEY_LEN,E_KEY_SWAP);
    }
    else
    {
        GetAESKey(E_AES_UPGRADE_KEY,aesKey);
    }

    temp_u32HwAesBufAddr = (U32 *)malloc(UPGRADE_IMAGE_FRAGMENT_SIZE + 64);
    if (temp_u32HwAesBufAddr == NULL)
    {
        UBOOT_ERROR("malloc 0x%x bytes for HW_AES_BUF failed!\n", UPGRADE_IMAGE_FRAGMENT_SIZE + 64);
        return -1;
    }

    u32HwAesBufAddr = ((U32)temp_u32HwAesBufAddr + 64) & (0xFFFFFFC0);

    if(u32HwAesBufAddr == 0xFFFFFFFF)
    {
        UBOOT_ERROR("PA2VA(u32HwAesPABufAddr) fail, u32HwAesVABufAddr=%x \n",u32HwAesBufAddr);
        free(temp_u32HwAesBufAddr);
        temp_u32HwAesBufAddr = NULL;
        u32HwAesBufAddr = 0;
        return -1;
    }

    UBOOT_DEBUG("u32HwAesBufAddr=%x \n", u32HwAesBufAddr);

    while(downloadLen)
    {
        readSegmentAddr=segmentNum*_segmentSize;
        if(downloadOffset%_segmentSize)//Only first time
        { 
            readOffsetInSeg=downloadOffset-readSegmentAddr;
            readLen=((readOffsetInSeg+downloadLen)>=_segmentSize)?(_segmentSize-readOffsetInSeg):downloadLen;

        }
        else
        {
            readOffsetInSeg=0;
            readLen=(downloadLen>=_segmentSize)?_segmentSize:downloadLen;

        }
        aesDecodeLen=((readSegmentAddr+_segmentSize)>=gfileSize)?gfileSize-readSegmentAddr:_segmentSize;

        UBOOT_DEBUG("downloadOffset=0x%x\n",downloadOffset);
        UBOOT_DEBUG("downloadLen=0x%x\n",downloadLen);
        UBOOT_DEBUG("pdownloadAddr=0x%x\n",pdownloadAddr);
        UBOOT_DEBUG("readSegmentAddr=0x%x\n",readSegmentAddr);
        UBOOT_DEBUG("readOffsetInSeg=0x%x\n",readOffsetInSeg);
        UBOOT_DEBUG("readLen=0x%x\n",readLen);
        UBOOT_DEBUG("aesDecodeLen=0x%x\n",aesDecodeLen);
        UBOOT_DEBUG("segmentNum=0x%x\n",segmentNum);

        if(getUpgradeMode()==EN_UPDATE_MODE_NET_WITH_SEG_DECRYPTED)
        {
            if(vfs_read((void*)u32HwAesBufAddr,DOWNLOAD_FILE,readSegmentAddr,aesDecodeLen)!=0)
            {
                free(temp_u32HwAesBufAddr);
                temp_u32HwAesBufAddr = NULL;
                u32HwAesBufAddr = 0;
                return -1;
            }
        }
        else if(getUpgradeMode()==EN_UPDATE_MODE_USB_WITH_SEG_DECRYPTED)
        {
            memset(buffer, 0, CMD_BUF);
            snprintf(buffer, CMD_BUF, "fatpartload %s %s %x %s %x %x", DOWNLOAD_INTERFACE,DOWNLOAD_DEVICE,u32HwAesBufAddr, DOWNLOAD_FILE, readSegmentAddr, aesDecodeLen);
            if(run_command(buffer, 0)!=0)
            {
                free(temp_u32HwAesBufAddr);
                temp_u32HwAesBufAddr = NULL;
                u32HwAesBufAddr = 0;
                return -1;
            }
        }
        else
        {
            UBOOT_ERROR("Wrong Upgrade Mode!!\n");
            free(temp_u32HwAesBufAddr);
            temp_u32HwAesBufAddr = NULL;
            u32HwAesBufAddr = 0;
            return -1;
        }

        // AES decode
        if(IsHouseKeepingBootingMode()==FALSE)
        {
            flush_cache(u32HwAesBufAddr,aesDecodeLen);
            Chip_Flush_Memory();
            secureBootResult = Secure_MailBox_AESDecode(&SecureBootMbxRetStruct, MsOS_VA2PA(u32HwAesBufAddr), aesDecodeLen, SECURE_ENG_AES_MODE_ECB);
            if(secureBootResult == E_SECUREBOOT_FAIL)
            {
                free(temp_u32HwAesBufAddr);
                temp_u32HwAesBufAddr = NULL;
                u32HwAesBufAddr = 0;
                return -1;
            }
        }
        else
        {
            aes_result = Secure_AES_ECB_Decrypt(u32HwAesBufAddr, aesDecodeLen, (U8 *)aesKey);
            if(aes_result != 0)
            {
                free(temp_u32HwAesBufAddr);
                temp_u32HwAesBufAddr = NULL;
                u32HwAesBufAddr = 0;
                return -1;
            }
        }

        // copy to target address
        memcpy((void*)pdownloadAddr,(char *)(u32HwAesBufAddr+readOffsetInSeg),readLen);

        downloadLen-=readLen;
        pdownloadAddr+=readLen;
        downloadOffset+=readLen;
        segmentNum++;
    }

    // set the download length for next command
    setenv("filesize",DOWNLOAD_LENGTH);
    free(temp_u32HwAesBufAddr);
    temp_u32HwAesBufAddr = NULL;
    u32HwAesBufAddr = 0;

    UBOOT_TRACE("OK\n");
    return 0;
}

U32 Secure_SegmentSize (void)
{
    UBOOT_TRACE("IN\n");
    UBOOT_TRACE("OK\n");
    return DEFAULT_SEGMENT_SIZE;
}

int GetfileSizeforAESUsbUpgrade(const char *Interface,const char *device,char *file)
{
    char* buffer=NULL;
    int ret=0;
    UBOOT_TRACE("IN\n");
    if(fileSizeReady==TRUE)
    {
        UBOOT_TRACE("OK\n");
        return ret;
    }

    buffer=(char *)malloc(CMD_BUF);
    if(buffer == NULL)
    {
       UBOOT_ERROR("Error: out of memory\n");
       return -1;
    }
    //Get total file size
    memset(buffer,0,CMD_BUF);
    if(getUpgradeMode()==EN_UPDATE_MODE_NET_WITH_SEG_DECRYPTED)
    {
        gfileSize=vfs_getsize(file);

    }
    else if(getUpgradeMode()==EN_UPDATE_MODE_USB_WITH_SEG_DECRYPTED)
    {
        snprintf(buffer, CMD_BUF, "fatfilesize %s %s %s 0x%08x",Interface,device, file, &gfileSize);
        UBOOT_DEBUG("cmd:%s\n",buffer);
        ret=run_command(buffer, 0);
        if(ret<0)
        {
            free(buffer);
            return -1;
        }
    }
    else
    {
        UBOOT_ERROR("Wrong Upgrade Mode!!\n");
        free(buffer);
        return -1;
    }

    free(buffer);
    fileSizeReady=TRUE;
    UBOOT_TRACE("OK\n");
    return ret;
}
#endif

#ifdef CONFIG_UPGRADE_WITH_FAIL_HANDLER
int get_and_check_chunk_info(char *ptrChunkFileBuf, unsigned int *pReadChunk, unsigned int offset, ST_CHUNK_INFO *ptr_chunk_info)
{
    unsigned int load_offset = 0;
    int checkSegNums = 0;
    UBOOT_TRACE("IN\n");
    //Get hash, signature infor from chunk data
    pReadChunk=(unsigned int *)((U32)ptrChunkFileBuf);
    pReadChunk+=2;
    ptr_chunk_info->segmentSize=*pReadChunk;
    pReadChunk+=1;
    ptr_chunk_info->fileDataOffset=*pReadChunk;
    ptr_chunk_info->fileDataOffset += offset;
    pReadChunk++;
    ptr_chunk_info->fileDataLen=*pReadChunk;
    pReadChunk++;

    ptr_chunk_info->fileHashOffset=*pReadChunk;
    ptr_chunk_info->fileHashOffset += offset;
    pReadChunk++;
    ptr_chunk_info->fileHashLen=*pReadChunk;
    pReadChunk++;

    // Check whether chunk info is valid or not
    if(memcmp((const void *)((U32)ptrChunkFileBuf),CHUNK_ID,sizeof(CHUNK_ID))!=0){
        UBOOT_ERROR("[ERROR] CHUNK_ID compare fail\n");
        return -1;
    }

    load_offset = HEADER_CHUNKINFO_SIZE - strlen(CHUNK_END);
    if(memcmp((const void *)((U32)ptrChunkFileBuf+load_offset),CHUNK_END,strlen(CHUNK_END))!=0){
        UBOOT_ERROR("[ERROR] CHUNK_END compare fail\n");
        return -1;
    }

    if ( (ptr_chunk_info->segmentSize > 0) && (ptr_chunk_info->fileDataLen > 0) && (ptr_chunk_info->fileHashLen > 0) )
    {
        if((ptr_chunk_info->fileDataLen%ptr_chunk_info->segmentSize)==0)
            checkSegNums  = ptr_chunk_info->fileDataLen/ptr_chunk_info->segmentSize;
        else
            checkSegNums  = ptr_chunk_info->fileDataLen/ptr_chunk_info->segmentSize+1;

        // Calculate the fileHashLen, fileHashLen = checkSegNums * (256/8)
        if((checkSegNums*32) != ptr_chunk_info->fileHashLen)
        {
            UBOOT_ERROR("The parameter in chunk info is not valid");
            return -1;
        }
    }
    else
    {
        UBOOT_ERROR("The parameter in chunk info is not valid");
        return -1;
    }

    UBOOT_TRACE("OK\n");
    return 0;
}

int firmware_image_header_authendication(char *upgradeFile)
{
    UBOOT_TRACE("IN\n");
    int ret=0;
    U32 u32_temp_header[PACK_HEADER_SIZE+64] = {0x00};
    unsigned char header_signature[SIGNATURE_SIZE]={0};
    unsigned char rsaPublicKeyN[RSA_PUBLIC_KEY_N_LEN]="\0";
    unsigned char rsaPublicKeyE[RSA_PUBLIC_KEY_E_LEN]="\0";
    U32 *u32_ptr_header = ((U32)u32_temp_header + 64) & (0xFFFFFFC0);
    char headerBuf[PACK_HEADER_SIZE] = {0};
    char buffer[CMD_BUF]={0};
    unsigned int load_offset = 0;
    // the auth part of header is IMAGE_OFFSET, TEE_CHUNK_INFO, REE_CHUNK_INFO, TEE's SIGN_OF_HASH_SET and REE's SIGN_OF_HASH_SET
    int header_len = HEADER_IMAGE_OFFSET_SIZE + HEADER_CHUNKINFO_SIZE + HEADER_CHUNKINFO_SIZE + SIGNATURE_SIZE + SIGNATURE_SIZE;

    //Load header with PACK_HEADER_SIZE
    memset(buffer,0,CMD_BUF);
    load_offset = 0;
    snprintf(buffer, CMD_BUF, "filepartload %x %s %x %x",headerBuf, upgradeFile, load_offset, PACK_HEADER_SIZE);
    UBOOT_DEBUG("cmd=%s\n",buffer);
    if(-1 == run_command(buffer, 0))
    {
        UBOOT_ERROR("Error: cmd: %s\n",buffer);
        return -1;
    }

    memcpy(header_signature, headerBuf, SIGNATURE_SIZE);
    memcpy(u32_ptr_header, (headerBuf+SIGNATURE_SIZE), header_len);

    //Get rsa upgrade public key
    ret = GetPublicKeyN(E_RSA_UPGRADE_PUBLIC_KEY,rsaPublicKeyN,RSA_PUBLIC_KEY_N_LEN);
    if(ret!=0)
    {
        UBOOT_ERROR("Get public key N fail\n");
        return -1;
    }
    ret = GetPublicKeyE(E_RSA_UPGRADE_PUBLIC_KEY,rsaPublicKeyE,RSA_PUBLIC_KEY_E_LEN);
    if(ret!=0)
    {
        UBOOT_ERROR("Get public key E fail\n");
        return -1;
    }

    ret = Secure_doAuthentication(rsaPublicKeyN, rsaPublicKeyE, header_signature, u32_ptr_header, header_len);
    if(ret!=0)
    {
        UBOOT_ERROR("\033[0;31m RSA upgrade public-N key=\033[0m\n");
        UBOOT_ERRDUMP((U32)rsaPublicKeyN, RSA_PUBLIC_KEY_N_LEN);

        UBOOT_ERROR("\033[0;31m RSA upgrade public-E key=\033[0m\n");
        UBOOT_ERRDUMP((U32)rsaPublicKeyE, RSA_PUBLIC_KEY_E_LEN);

        UBOOT_ERROR("\033[0;31m signature=\033[0m\n");
        UBOOT_ERRDUMP((U32)header_signature, SIGNATURE_LEN);

        UBOOT_ERROR("\033[0;31m clear image=\033[0m\n");
        UBOOT_ERRDUMP((U32)u32_ptr_header, header_len);
        Secure_HaltCPU();
        return -1;
    }
    else
    {
        #if defined(CONFIG_SECURE_DEBUG_MESSAGE)
        UBOOT_INFO("\033[0;34m******************************************\033[0m\n");
        UBOOT_INFO("\033[0;34m* Image Header Authentication SUCCESS !! *\033[0m\n");
        UBOOT_INFO("\033[0;34m******************************************\033[0m\n");
        #endif
    }

    UBOOT_TRACE("OK\n");
    return 0;
}

int firmware_image_authendication(char *upgradeFile, int device, int partition)
{
    char buffer[CMD_BUF]={0};
    UBOOT_TRACE("IN\n");

    memset(buffer, 0 , CMD_BUF);
    snprintf(buffer, CMD_BUF, "fileSegRSA usb %d:%d %s", device, partition, upgradeFile);
    UBOOT_DEBUG("cmd=%s\n",buffer);
    if(-1 == run_command(buffer, 0))
    {
        UBOOT_ERROR("Error: cmd: %s\n",buffer);
        Reset_System();
    }
    UBOOT_TRACE("OK\n");
    return 0;
}

int check_image_segement_SHA(ST_CHUNK_INFO *ptr_chunk_info, char *target_interface, char *target_device, char *target_file, int is_descrypted)
{
    unsigned char *pt=NULL;
    unsigned char *segmentBuf = NULL;
    unsigned char *ptrSegmentBuf = NULL;
    unsigned int readLen=0;
    unsigned int fileDataLen=0;
    unsigned int fileDataOffset=0;
    unsigned int fileHashOffset=0;
    int i=0;

    char buffer[CMD_BUF]="\0";
    unsigned char hashOutBuf[SHA256_DIGEST_SIZE+64]={0x0};
    unsigned char *ptrHashOutBuf = ((MS_U32)hashOutBuf + 64) & (0xFFFFFFC0);
    UBOOT_TRACE("IN\n");

    // segmentSize is global variable, set it for partload segment used.
    segmentSize = ptr_chunk_info->segmentSize;

    fileDataLen = ptr_chunk_info->fileDataLen;
    fileDataOffset = ptr_chunk_info->fileDataOffset;
    fileHashOffset = ptr_chunk_info->fileHashOffset;

    UBOOT_DEBUG("segmentSize=%x\n", segmentSize);
    UBOOT_DEBUG("fileDataLen=%x\n", fileDataLen);
    UBOOT_DEBUG("fileDataOffset=%x\n", fileDataOffset);
    UBOOT_DEBUG("fileHashOffset=%x\n", fileHashOffset);

    segmentBuf = (unsigned int*)malloc(segmentSize+64);
    if(segmentBuf==NULL)
    {
        UBOOT_ERROR("[ERROR] segmentBuf memroy allocate fail\n");
        return -1;
    }

    ptrSegmentBuf = ((MS_U32)segmentBuf+64) & (0xFFFFFFC0);

    while(fileDataLen)
    {
        UBOOT_INFO("*");
        readLen=(fileDataLen>=segmentSize)?segmentSize:fileDataLen;

        memset(buffer,0,CMD_BUF);
        if ( is_descrypted == 1 )
            snprintf(buffer, CMD_BUF, "filepartloadSegAES %s %s %x %s %x %x", target_interface,target_device,(U32)ptrSegmentBuf, target_file, fileDataOffset, readLen);
        else
            snprintf(buffer, CMD_BUF, "filepartloadSeg %s %s %x %s %x %x %x", target_interface,target_device,(U32)ptrSegmentBuf, target_file, fileDataOffset, readLen, segmentSize);
        UBOOT_DEBUG("cmd=%s\n", buffer);
        if(run_command(buffer, 0)!=0)
        {
            UBOOT_ERROR("Cmd:'%s' fail!!\n",buffer);
            free(segmentBuf);
            return -1;
        }

        if(CommonSHA((U32)ptrSegmentBuf,(MS_U32)ptrHashOutBuf,(MS_U32)readLen)!=0)
        {
            UBOOT_ERROR("CommonSHA Fail!!\n");
            free(segmentBuf);
            return -1;
        }

        memset(buffer,0,CMD_BUF);
        if ( is_descrypted == 1 )
            snprintf(buffer, CMD_BUF, "filepartloadSegAES %s %s %x %s %x %x", target_interface,target_device,(U32)ptrSegmentBuf, target_file, fileHashOffset, SHA256_DIGEST_SIZE*2);
        else
            snprintf(buffer, CMD_BUF, "filepartloadSeg %s %s %x %s %x %x %x", target_interface,target_device,(U32)ptrSegmentBuf, target_file, fileHashOffset, SHA256_DIGEST_SIZE*2, segmentSize);
        UBOOT_DEBUG("cmd=%s\n", buffer);
        if(run_command(buffer, 0)!=0)
        {
            UBOOT_ERROR("Cmd:'%s' fail!!\n",buffer);
            free(segmentBuf);
            return -1;
        }

        pt=(unsigned char *)(ptrSegmentBuf);//+SHA256_DIGEST_SIZE-1);
        UBOOT_DUMP((unsigned int)pt, SHA256_DIGEST_SIZE);
        UBOOT_DUMP((unsigned int)ptrHashOutBuf, SHA256_DIGEST_SIZE);
        for(i = 0; i<SHA256_DIGEST_SIZE; i++)
        {
            if(ptrHashOutBuf[i] != *pt)
             {
                UBOOT_ERROR("ptrHashOutBuf[%d]=0x%x, *pt=0x%x \n",i,ptrHashOutBuf[i],*pt);
                free(segmentBuf);
                return -1;
             }
             pt++;
        }

        fileDataLen-=readLen;
        fileDataOffset+=readLen;
        fileHashOffset+=SHA256_DIGEST_SIZE;

        UBOOT_DEBUG("readLen=0x%x\n",readLen);
        UBOOT_DEBUG("fileDataLen=0x%x\n",fileDataLen);
        UBOOT_DEBUG("fileDataOffset=0x%x\n",fileDataOffset);
        UBOOT_DEBUG("fileHashOffset=0x%x\n",fileHashOffset);
    }

    free(segmentBuf);
    UBOOT_INFO("\n");
    UBOOT_TRACE("OK\n");
    return 0;
}

int do_file_segment_rsa_authendication(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    #define TARGET_INTERFACE argv[1]
    #define TARGET_DEVICE  argv[2]
    #define TARGET_FILE  argv[3]

    #undef RSA_PUBLIC_KEY_LEN
    #define RSA_PUBLIC_KEY_LEN (RSA_PUBLIC_KEY_N_LEN+RSA_PUBLIC_KEY_E_LEN)

    unsigned int i=0;
    unsigned int *pReadTEEChunk=NULL;
    unsigned int *pReadREEChunk=NULL;
    char buffer[CMD_BUF]="\0";
    int ret=0;
    unsigned int hashSetSize=0;
    unsigned char rsaOutBuf[RSA_KEY_DIGI_LEN*4+64]={0x0};
    unsigned char rsaPublicKeyN[RSA_PUBLIC_KEY_N_LEN]={0x0};
    unsigned char rsaPublicKeyE[RSA_PUBLIC_KEY_E_LEN]={0x0};
    unsigned char hashOutBuf[SHA256_DIGEST_SIZE+64]={0x0};
    unsigned char headerBuf[PACK_HEADER_SIZE]={0x0};
    unsigned char header_signature[SIGNATURE_SIZE]={0x0};

    unsigned char chunkTEEFileBuf[SEGMENT_RSA_CHUNK_FILE_SIZE]={0x0};
    unsigned char chunkREEFileBuf[SEGMENT_RSA_CHUNK_FILE_SIZE]={0x0};
    unsigned char *hashSetBuf = NULL;
    unsigned char *ptrHashSetBuf = NULL;
    unsigned char *ptrREESegmentBuf = NULL;
    unsigned char *ptrREEHashSetBuf = NULL;
    unsigned char *ptrTEESegmentBuf = NULL;
    unsigned char *ptrTEEHashSetBuf = NULL;
    unsigned int load_offset = 0;
    int hasTEEImage = 1; // 1 == has Tee image, 0 == no Tee image

    unsigned char *ptrChunkTEEFileBuf = (MS_U32)chunkTEEFileBuf;
    unsigned char *ptrChunkREEFileBuf = (MS_U32)chunkREEFileBuf;
    unsigned char *ptrHeaderBuf = (MS_U32)headerBuf;

    // for aligned 64
    unsigned char *ptrHashOutBuf = ((MS_U32)hashOutBuf + 64) & (0xFFFFFFC0);
    unsigned char *ptrRsaOutBuf = ((MS_U32)rsaOutBuf + 64) & (0xFFFFFFC0);

    g_st_TEE_chunk_info.fileDataOffset = 0;
    g_st_TEE_chunk_info.fileDataLen = 0;
    g_st_TEE_chunk_info.fileHashOffset = 0;
    g_st_TEE_chunk_info.fileHashLen = 0;

    g_st_REE_chunk_info.fileDataOffset = 0;
    g_st_REE_chunk_info.fileDataLen = 0;
    g_st_REE_chunk_info.fileHashOffset = 0;
    g_st_REE_chunk_info.fileHashLen = 0;

    UBOOT_TRACE("IN\n");
    if(argc<4)
        return -1;

    //Get rsa upgrade public key
    ret = GetPublicKeyN(E_RSA_UPGRADE_PUBLIC_KEY,rsaPublicKeyN,RSA_PUBLIC_KEY_N_LEN);
    if(ret!=0)
    {
        UBOOT_ERROR("Get public key N fail\n");
        return -1;
    }
    UBOOT_DUMP(rsaPublicKeyN, RSA_PUBLIC_KEY_N_LEN);
    ret = GetPublicKeyE(E_RSA_UPGRADE_PUBLIC_KEY,rsaPublicKeyE,RSA_PUBLIC_KEY_E_LEN);
    if(ret!=0)
    {
        UBOOT_ERROR("Get public key E fail\n");
        return -1;
    }

    // Read image header
    load_offset = 0;
    memset(buffer, 0, CMD_BUF);
    snprintf(buffer, CMD_BUF, "filepartload %x %s %x %x",ptrHeaderBuf, TARGET_FILE, load_offset, PACK_HEADER_SIZE);
    UBOOT_DEBUG("cmd=%s\n",buffer);
    if(-1 == run_command(buffer, 0))
    {
        UBOOT_ERROR("Error: cmd: %s\n",buffer);
        return -1;
    }

    // if the TEE image len == 0, means there are no tee image in firmware upgrade image.
    if( g_st_image_offset_info.u32TEEImageOffsetLEN == 0 )
        hasTEEImage = 0;
    else
        hasTEEImage = 1;

    if( hasTEEImage == 1 )
    {
        // Read TEE chunk info from header
        // The offset of chunk info is 0x100+0x200 in header
        load_offset = SIGNATURE_SIZE + HEADER_IMAGE_OFFSET_SIZE;
        memcpy(ptrChunkTEEFileBuf, (ptrHeaderBuf+load_offset), HEADER_CHUNKINFO_SIZE);

        UBOOT_INFO("Do TEE Chunk Info Verify...\n");
        ret = get_and_check_chunk_info(ptrChunkTEEFileBuf, pReadTEEChunk, PACK_HEADER_SIZE, &g_st_TEE_chunk_info);
        if( ret != 0 )
        {
            return -1;
        }

        UBOOT_DEBUG("TEE segment size=0x%x\n",g_st_TEE_chunk_info.segmentSize);
        UBOOT_DEBUG("TEE fileDataOffset=0x%x\n",g_st_TEE_chunk_info.fileDataOffset);
        UBOOT_DEBUG("TEE fileDataLen=0x%x\n",g_st_TEE_chunk_info.fileDataLen);
        UBOOT_DEBUG("TEE fileHashOffset=0x%x\n",g_st_TEE_chunk_info.fileHashOffset);
        UBOOT_DEBUG("TEE fileHashLen=0x%x\n",g_st_TEE_chunk_info.fileHashLen);

        ptrTEEHashSetBuf = (unsigned int*)malloc(g_st_TEE_chunk_info.fileHashLen);
        if(ptrTEEHashSetBuf==NULL)
        {
            UBOOT_ERROR("[ERROR] ptrTEEHashSetBuf memroy allocate fail\n");
            return -1;
        }

        // Read hashset of TEE image to ptrTEEHashSetBuf
        segmentSize = g_st_TEE_chunk_info.segmentSize;
        memset(buffer, 0, CMD_BUF);
        memset(ptrTEEHashSetBuf, 0, g_st_TEE_chunk_info.fileHashLen);
        snprintf(buffer, CMD_BUF, "filepartload %x %s %x %x",ptrTEEHashSetBuf, TARGET_FILE, g_st_TEE_chunk_info.fileHashOffset, g_st_TEE_chunk_info.fileHashLen);
        UBOOT_DEBUG("cmd=%s\n",buffer);
        if(-1 == run_command(buffer, 0))
        {
            UBOOT_ERROR("Error: cmd: %s\n",buffer);
            free(ptrTEEHashSetBuf);
            return -1;
        }
        UBOOT_DEBUG("TEE HASHSET:\n");
        UBOOT_DUMP(ptrTEEHashSetBuf, g_st_TEE_chunk_info.fileHashLen);

        hashSetBuf=(unsigned int*)malloc(g_st_TEE_chunk_info.fileHashLen+64);
        ptrHashSetBuf = ((MS_U32)hashSetBuf+64) & (0xFFFFFFC0);
        memcpy(ptrHashSetBuf, ptrTEEHashSetBuf, g_st_TEE_chunk_info.fileHashLen);

        // Read signature of TEE hashset from header
        // The offset of signature of hashset is 0x100+0x200+0x80+0x80 in header
        load_offset = SIGNATURE_SIZE + HEADER_IMAGE_OFFSET_SIZE + HEADER_CHUNKINFO_SIZE + HEADER_CHUNKINFO_SIZE;
        memcpy(header_signature, (ptrHeaderBuf+load_offset), SIGNATURE_SIZE);

        memset(ptrRsaOutBuf, 0, RSA_KEY_DIGI_LEN*4);
        memset(ptrHashOutBuf, 0, SHA256_DIGEST_SIZE);

        // Do RSA for TEE's hashset signature
        rsa_main((unsigned char *)header_signature,rsaPublicKeyN,rsaPublicKeyE,ptrRsaOutBuf);

        // Calculate the SHA of TEE's hashset
        if(CommonSHA((U32)ptrHashSetBuf,(MS_U32)ptrHashOutBuf,(MS_U32)g_st_TEE_chunk_info.fileHashLen)!=0)
        {
            UBOOT_ERROR("CommonSHA Fail!!\n");
            free(hashSetBuf);
            free(ptrTEEHashSetBuf);
            hashSetBuf = NULL;
            ptrTEEHashSetBuf = NULL;
            return -1;
        }

        for(i = 0; i<SHA256_DIGEST_SIZE; i++)
        {
            if(ptrHashOutBuf[i] != ptrRsaOutBuf[i+224])
            {
               UBOOT_ERROR("ptrHashOutBuf[%d]=0x%x, ptrRsaOutBuf[%d]=0x%x \n",i,ptrHashOutBuf[i],i,ptrRsaOutBuf[i+224]);
               UBOOT_DUMP((unsigned int)ptrRsaOutBuf, SHA256_DIGEST_SIZE);
               UBOOT_DUMP((unsigned int)ptrHashOutBuf, SHA256_DIGEST_SIZE);
               free(hashSetBuf);
                free(ptrTEEHashSetBuf);
                hashSetBuf = NULL;
                ptrTEEHashSetBuf = NULL;
               return -1;
            }
        }

        if( memcmp(RSA_Padding, ptrRsaOutBuf, sizeof(RSA_Padding)) != 0 )
        {
            UBOOT_ERROR("RSA Padding is not matched\n");
            UBOOT_DUMP((unsigned int)RSA_Padding, 32);
            UBOOT_DUMP((unsigned int)ptrRsaOutBuf, 32);
            free(hashSetBuf);
            free(ptrTEEHashSetBuf);
            hashSetBuf = NULL;
            ptrTEEHashSetBuf = NULL;
            return -1;
        }

        free(ptrTEEHashSetBuf);
        ptrTEEHashSetBuf = NULL;

        // hashSetBuf will be reused for REE auth
        free(hashSetBuf);
        ptrHashSetBuf = NULL;
    }

    // Read REE chunk info from header
    // The offset of chunk info is 0x100+0x200+0x80 in header
    load_offset = SIGNATURE_SIZE + HEADER_IMAGE_OFFSET_SIZE + HEADER_CHUNKINFO_SIZE;
    memcpy(ptrChunkREEFileBuf, (ptrHeaderBuf+load_offset), HEADER_CHUNKINFO_SIZE);
    //UBOOT_DUMP(ptrChunkFileBuf, g_st_image_offset_info.chunkInfoSize);

    UBOOT_INFO("Do REE Chunk Info Verify...\n");
    load_offset = PACK_HEADER_SIZE + g_st_image_offset_info.u32TEEImageOffsetLEN;
    ret = get_and_check_chunk_info(ptrChunkREEFileBuf, pReadREEChunk, load_offset, &g_st_REE_chunk_info);
    if( ret != 0 )
    {
        return -1;
    }

    UBOOT_DEBUG("REE segment size=0x%x\n",g_st_REE_chunk_info.segmentSize);
    UBOOT_DEBUG("REE fileDataOffset=0x%x\n",g_st_REE_chunk_info.fileDataOffset);
    UBOOT_DEBUG("REE fileDataLen=0x%x\n",g_st_REE_chunk_info.fileDataLen);
    UBOOT_DEBUG("REE fileHashOffset=0x%x\n",g_st_REE_chunk_info.fileHashOffset);
    UBOOT_DEBUG("REE fileHashLen=0x%x\n",g_st_REE_chunk_info.fileHashLen);

    ptrREEHashSetBuf = (unsigned int*)malloc(g_st_REE_chunk_info.fileHashLen);
    if(ptrREEHashSetBuf==NULL)
    {
        UBOOT_ERROR("[ERROR] ptrREEHashSetBuf memroy allocate fail\n");
        return -1;
    }

    // set USB decrtpted mode for load ree image's hashset
    setUpgradeMode(EN_UPDATE_MODE_USB_WITH_SEG_DECRYPTED);
    segmentSize = g_st_REE_chunk_info.segmentSize;
    memset(buffer,0,CMD_BUF);
    memset(ptrREEHashSetBuf,0x0,g_st_REE_chunk_info.fileHashLen);
    snprintf(buffer, CMD_BUF, "filepartloadSegAES %s %s %x %s %x %x", \
        TARGET_INTERFACE,TARGET_DEVICE,(U32)ptrREEHashSetBuf, TARGET_FILE, g_st_REE_chunk_info.fileHashOffset, g_st_REE_chunk_info.fileHashLen);
    if(run_command(buffer, 0)!=0)
    {
        UBOOT_ERROR("Cmd:'%s' fail!!\n",buffer);
        free(ptrREEHashSetBuf);
        return -1;
    }
    UBOOT_DEBUG("REE HASHSET:\n");
    //UBOOT_DUMP(ptrREEHashSetBuf, g_st_REE_chunk_info.fileHashLen);

    hashSetBuf=(unsigned int*)malloc(g_st_REE_chunk_info.fileHashLen+64);
    if(hashSetBuf==NULL)
    {
        UBOOT_ERROR("[ERROR] hashSetBuf memroy allocate fail\n");
        free(ptrREEHashSetBuf);
        return -1;
    }

    ptrHashSetBuf = ((MS_U32)hashSetBuf+64) & (0xFFFFFFC0);
    memcpy(ptrHashSetBuf, ptrREEHashSetBuf, g_st_REE_chunk_info.fileHashLen);
    UBOOT_DUMP(ptrHashSetBuf, g_st_REE_chunk_info.fileHashLen);

    // release resource, no longer to use.
    free(ptrREEHashSetBuf);
    ptrREEHashSetBuf = NULL;

    // Read signature of REE's hashset from header
    // The offset of signature of hashset is 0x100+0x200+0x80+0x80+0x100 in header
    load_offset = SIGNATURE_SIZE + HEADER_IMAGE_OFFSET_SIZE + HEADER_CHUNKINFO_SIZE + HEADER_CHUNKINFO_SIZE + SIGNATURE_SIZE;
    memcpy(header_signature, (ptrHeaderBuf+load_offset), SIGNATURE_SIZE);
    UBOOT_DUMP(header_signature, SIGNATURE_SIZE);

    memset(ptrRsaOutBuf, 0, RSA_KEY_DIGI_LEN*4);
    memset(ptrHashOutBuf, 0, SHA256_DIGEST_SIZE);
    // Do RSA for REE's hashset signature
    rsa_main((unsigned char *)header_signature,rsaPublicKeyN,rsaPublicKeyE,ptrRsaOutBuf);

    // Calculate the SHA of REE's hashset
    if(CommonSHA((U32)ptrHashSetBuf,(MS_U32)ptrHashOutBuf,(MS_U32)g_st_REE_chunk_info.fileHashLen)!=0)
    {
        UBOOT_ERROR("CommonSHA Fail!!\n");
        free(hashSetBuf);
        return -1;
    }

    for(i = 0; i<SHA256_DIGEST_SIZE; i++)
    {
        if(ptrHashOutBuf[i] != ptrRsaOutBuf[i+224])
        {
           UBOOT_ERROR("ptrHashOutBuf[%d]=0x%x, ptrRsaOutBuf[%d]=0x%x \n",i,ptrHashOutBuf[i],i+224,ptrRsaOutBuf[i+224]);
           UBOOT_DUMP((unsigned int)ptrRsaOutBuf, 0x100);
           UBOOT_DUMP((unsigned int)ptrHashOutBuf, SHA256_DIGEST_SIZE);
           free(hashSetBuf);
           return -1;
        }
    }

    if( memcmp(RSA_Padding, ptrRsaOutBuf, sizeof(RSA_Padding)) != 0 )
    {
        UBOOT_ERROR("RSA Padding is not matched\n");
        UBOOT_DUMP((unsigned int)RSA_Padding, 32);
        UBOOT_DUMP((unsigned int)ptrRsaOutBuf, 32);
        free(hashSetBuf);
        return -1;
    }

    // Do rsa authentication for ***.hash
    if( hasTEEImage == 1 )
    {
        UBOOT_INFO("TEE Image Segment RSA Authentication ....\n");
        setUpgradeMode(EN_UPDATE_MODE_USB);
        ret = check_image_segement_SHA(&g_st_TEE_chunk_info, TARGET_INTERFACE, TARGET_DEVICE, TARGET_FILE, 0);
        if( ret != 0 )
        {
            free(hashSetBuf);
            return -1;
        }
        UBOOT_INFO("\n");
    }

    UBOOT_INFO("REE Image Segment RSA Authentication ....\n");
    setUpgradeMode(EN_UPDATE_MODE_USB_WITH_SEG_DECRYPTED);
    ret = check_image_segement_SHA(&g_st_REE_chunk_info, TARGET_INTERFACE, TARGET_DEVICE, TARGET_FILE, 1);
    if( ret != 0 )
    {
        free(hashSetBuf);
        return -1;
    }
    UBOOT_INFO("\n");

    free(hashSetBuf);
    UBOOT_TRACE("OK\n");
    return 0;
}

int do_file_part_load_with_segment_aes_decrypted(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    #define DOWNLOAD_INTERFACE argv[1]
    #define DOWNLOAD_DEVICE argv[2]
    #define DOWNLOAD_ADDR argv[3]
    #define DOWNLOAD_FILE argv[4]
    #define DOWNLOAD_OFFSET argv[5]
    #define DOWNLOAD_LENGTH argv[6]

    unsigned int downloadOffset=0;
    unsigned int downloadLen=0;
    unsigned int pdownloadAddr=0;

    unsigned int readSegmentAddr=0;
    unsigned int readOffsetInSeg=0;
    unsigned int readLen=0;
    unsigned int aesDecodeLen=0;
    unsigned int segmentNum=0;
    unsigned int _segmentSize=0;
    unsigned int u32HwAesBufAddr=0;
    char buffer[CMD_BUF]="\0";
    SECUREBOOT_MbxRetStruct SecureBootMbxRetStruct;
    int aes_result=0;
    SECUREBOOT_Result secureBootResult;
    U32 *temp_u32HwAesBufAddr = NULL;
    UBOOT_TRACE("IN\n");

    if(argc<7)
    {
        return -1;
    }

    downloadOffset = (unsigned int)simple_strtoul(DOWNLOAD_OFFSET, NULL, 16);
    downloadLen = (unsigned int)simple_strtoul(DOWNLOAD_LENGTH, NULL, 16);
    pdownloadAddr = (unsigned int)simple_strtoul(DOWNLOAD_ADDR, NULL, 16);

    //Get total file size
    if(GetfileSizeforAESUsbUpgrade(DOWNLOAD_INTERFACE,DOWNLOAD_DEVICE,DOWNLOAD_FILE)<0)
    {
        UBOOT_ERROR("GetfileSizeforAESUsbUpgrade fail\n");
        return -1;
    }

    // get _segmentSize and segmentNum
    if(segmentSize==0)
    {
        _segmentSize=DEFAULT_SEGMENT_SIZE;
    }
    else
    {
        _segmentSize=segmentSize;
    }
    segmentNum=downloadOffset/_segmentSize;

    //Get AES decode key
    if(IsHouseKeepingBootingMode()==FALSE)
    {
        Secure_MailBox_ASSIGN_DECRYPTED_KEY(&SecureBootMbxRetStruct,E_AES_UPGRADE_KEY,AES_KEY_LEN,E_KEY_SWAP);
    }
    else
    {
        GetAESKey(E_AES_UPGRADE_KEY,aesKey);
    }

    temp_u32HwAesBufAddr = (U32 *)malloc(UPGRADE_IMAGE_FRAGMENT_SIZE + 64);
    if (temp_u32HwAesBufAddr == NULL)
    {
        UBOOT_ERROR("malloc 0x%x bytes for HW_AES_BUF failed!\n", UPGRADE_IMAGE_FRAGMENT_SIZE + 64);
        return -1;
    }

    u32HwAesBufAddr = ((U32)temp_u32HwAesBufAddr + 64) & (0xFFFFFFC0);

    if(u32HwAesBufAddr == 0xFFFFFFFF)
    {
        UBOOT_ERROR("PA2VA(u32HwAesPABufAddr) fail, u32HwAesVABufAddr=%x \n",u32HwAesBufAddr);
        free(temp_u32HwAesBufAddr);
        temp_u32HwAesBufAddr = NULL;
        u32HwAesBufAddr = 0;
        return -1;
    }

    UBOOT_DEBUG("u32HwAesBufAddr=%x \n", u32HwAesBufAddr);

    while(downloadLen)
    {
        readSegmentAddr=segmentNum*_segmentSize;
        if(downloadOffset%_segmentSize)//Only first time
        {
            readOffsetInSeg=downloadOffset-readSegmentAddr;
            readLen=((readOffsetInSeg+downloadLen)>=_segmentSize)?(_segmentSize-readOffsetInSeg):downloadLen;

        }
        else
        {
            readOffsetInSeg=0;
            readLen=(downloadLen>=_segmentSize)?_segmentSize:downloadLen;

        }
        aesDecodeLen=((readSegmentAddr+_segmentSize)>=gfileSize)?gfileSize-readSegmentAddr:_segmentSize;

        UBOOT_DEBUG("_segmentSize=0x%x\n",_segmentSize);
        UBOOT_DEBUG("gfileSize=0x%x\n",gfileSize);
        UBOOT_DEBUG("downloadOffset=0x%x\n",downloadOffset);
        UBOOT_DEBUG("downloadLen=0x%x\n",downloadLen);
        UBOOT_DEBUG("pdownloadAddr=0x%x\n",pdownloadAddr);
        UBOOT_DEBUG("readSegmentAddr=0x%x\n",readSegmentAddr);
        UBOOT_DEBUG("readOffsetInSeg=0x%x\n",readOffsetInSeg);
        UBOOT_DEBUG("readLen=0x%x\n",readLen);
        UBOOT_DEBUG("aesDecodeLen=0x%x\n",aesDecodeLen);
        UBOOT_DEBUG("segmentNum=0x%x\n",segmentNum);

        if(getUpgradeMode()==EN_UPDATE_MODE_NET_WITH_SEG_DECRYPTED)
        {
            if(vfs_read((void*)u32HwAesBufAddr,DOWNLOAD_FILE,readSegmentAddr,aesDecodeLen)!=0)
            {
                free(temp_u32HwAesBufAddr);
                temp_u32HwAesBufAddr = NULL;
                u32HwAesBufAddr = 0;
                return -1;
            }
        }
        else if(getUpgradeMode()==EN_UPDATE_MODE_USB_WITH_SEG_DECRYPTED)
        {
            memset(buffer, 0, CMD_BUF);
            snprintf(buffer, CMD_BUF, "fatpartload %s %s %x %s %x %x", DOWNLOAD_INTERFACE,DOWNLOAD_DEVICE,u32HwAesBufAddr, DOWNLOAD_FILE, readSegmentAddr, aesDecodeLen);
            if(run_command(buffer, 0)!=0)
            {
                free(temp_u32HwAesBufAddr);
                temp_u32HwAesBufAddr = NULL;
                u32HwAesBufAddr = 0;
                return -1;
            }
        }
        else
        {
            UBOOT_ERROR("Wrong Upgrade Mode!!\n");
            free(temp_u32HwAesBufAddr);
            temp_u32HwAesBufAddr = NULL;
            u32HwAesBufAddr = 0;
            return -1;
        }

        // AES decode
        if(IsHouseKeepingBootingMode()==FALSE)
        {
            flush_cache(u32HwAesBufAddr,aesDecodeLen);
            Chip_Flush_Memory();
            secureBootResult = Secure_MailBox_AESDecode(&SecureBootMbxRetStruct, MsOS_VA2PA(u32HwAesBufAddr), aesDecodeLen, SECURE_ENG_AES_MODE_ECB);
            if(secureBootResult == E_SECUREBOOT_FAIL)
            {
                free(temp_u32HwAesBufAddr);
                temp_u32HwAesBufAddr = NULL;
                u32HwAesBufAddr = 0;
                return -1;
            }
        }
        else
        {
            aes_result = Secure_AES_ECB_Decrypt(u32HwAesBufAddr, aesDecodeLen, (U8 *)aesKey);
            if(aes_result != 0)
            {
                free(temp_u32HwAesBufAddr);
                temp_u32HwAesBufAddr = NULL;
                u32HwAesBufAddr = 0;
                return -1;
            }
        }

        // copy to target address
        memcpy((void*)pdownloadAddr,(char *)(u32HwAesBufAddr+readOffsetInSeg),readLen);
        //UBOOT_DUMP(u32HwAesBufAddr, 0x200);
        //UBOOT_DUMP(u32HwAesBufAddr+readOffsetInSeg, 0x200);
        downloadLen-=readLen;
        pdownloadAddr+=readLen;
        downloadOffset+=readLen;
        segmentNum++;
    }

    // set the download length for next command
    setenv("filesize",DOWNLOAD_LENGTH);
    free(temp_u32HwAesBufAddr);
    temp_u32HwAesBufAddr = NULL;
    u32HwAesBufAddr = 0;
    UBOOT_TRACE("OK\n");
    return 0;
}


U32 Secure_SegmentSize (void)
{
    UBOOT_TRACE("IN\n");
    UBOOT_TRACE("OK\n");
    return DEFAULT_SEGMENT_SIZE;
}

int GetfileSizeforAESUsbUpgrade(const char *Interface,const char *device,char *file)
{
    char* buffer=NULL;
    int ret=0;
    UBOOT_TRACE("IN\n");
    if(fileSizeReady==TRUE)
    {
        UBOOT_TRACE("OK\n");
        return ret;
    }

    buffer=(char *)malloc(CMD_BUF);
    if(buffer == NULL)
    {
       UBOOT_ERROR("Error: out of memory\n");
       return -1;
    }
    //Get total file size
    memset(buffer,0,CMD_BUF);
    if(getUpgradeMode()==EN_UPDATE_MODE_NET_WITH_SEG_DECRYPTED)
    {
        gfileSize=vfs_getsize(file);

    }
    else if(getUpgradeMode()==EN_UPDATE_MODE_USB_WITH_SEG_DECRYPTED)
    {
        snprintf(buffer, CMD_BUF, "fatfilesize %s %s %s 0x%08x",Interface,device, file, &gfileSize);
        UBOOT_DEBUG("cmd:%s\n",buffer);
        ret=run_command(buffer, 0);
        if(ret<0)
        {
            free(buffer);
            return -1;
        }
    }
    else
    {
        UBOOT_ERROR("Wrong Upgrade Mode!!\n");
        free(buffer);
        return -1;
    }

    //gfileSize -= (PACK_HEADER_SIZE - g_st_image_offset_info.u32TEEImageOffsetLEN);
    free(buffer);
    fileSizeReady=TRUE;
    UBOOT_TRACE("OK\n");
    return ret;
}
#endif
