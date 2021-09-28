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
#include <exports.h>
#include <command.h>
#include <MsSystem.h>

#include <secure/MsSecureBoot.h>
#include <secure/MsSignature.h>
#include <secure/MsSecureCommon.h>
#include <secure/crypto_rsa.h>
#include <secure/crypto_sha.h>
#include <secure/crypto_func.h>
#include <secure/crypto_aes.h>
#include <secure/crypto_auth.h>
#include <secure/apiSecureBoot.h>
#include <MsEnvironment.h>
#include <MsTypes.h>
#include <MsSystem.h>
#include <ShareType.h>
#include <MsSystem.h>
#include <MsDebug.h>
#include <MsUtility.h>
#include <MsRawIO.h>
#include <MsMmap.h>
#include <MsBoot.h>
#include <drvMBX.h>
#include <drvAESDMA.h>
#include <CusConfig.h>
#if (CONFIG_URSA_UNION) || defined(CONFIG_URSA20_VB1)
#include <ursa/ursa_common.h>
#endif
#if defined(CONFIG_ANDROID_AVB_ENABLE)
#include <libavb_ab_uboot.h>
#endif
#include <android_image.h>
//-------------------------------------------------------------------------------------------------
//  Debug
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define CUSTOMER_KEY_BANK_LEN 0x1000

#ifndef MAX_IMAGE_SIZE
#define MAX_IMAGE_SIZE (25*1024*1024)
#endif

#define WITH_PADDING_SIZE(size, page)    (((size)+(page)-1) & (~((page)-1)))

#if(ENABLE_MMC==1)
#define FLASH_RAW_DATA_TARGET E_RAW_DATA_IN_MMC
#elif (ENABLE_NAND_FLASH==1)
#define FLASH_RAW_DATA_TARGET E_RAW_DATA_IN_NAND
#else
#error
#endif

#define ENV_BOOT_CMD "bootcmd"
#define ENV_PROTECTION_KEY "PROTECTION_KEY"
#define ENV_PARTAIL_AUTHENTICATION "partial_partition"
#define DID_LEN 16

#if defined(__ARM__)
#define CONFIG_SRAM_BASE_ADDRESS    0x1FC00000
#elif defined(CONFIG_MSTAR_KIWI)
#define CONFIG_SRAM_BASE_ADDRESS    0xB2000000
#else
#define CONFIG_SRAM_BASE_ADDRESS    0xBF400000
#endif
#define POR_STATUS_BASE_ADDR (0x100500)

extern unsigned long MS_RIU_MAP;
#define RIU     ((unsigned short volatile *) MS_RIU_MAP)

//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------
#define ENV_KEYBANK_DONE "keybank_done"
#if (ENABLE_MODULE_ANDROID_BOOT == 1 )
    #define KERNEL "boot"
    #define SYSTEM "system"
#else
    #define KERNEL "KL"
#endif


#ifndef CONFIG_RAMDISK_ADDR
#define CONFIG_RAMDISK_ADDR 0x22000000
#endif

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
#if defined (CONFIG_SEC_SYSTEM)
extern int ENABLE_SECOND_SYSTEM;
#endif
extern unsigned long gOpteeFailFlag;

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static SECURITY_INFO stSecureInfo;
static SUB_SECURE_INFO stSubSecureInfo;
static EN_SECUREMERGE_TYPE enSecureMergeType = E_SECUREMERGE_NONE;
static CUSTOMER_KEY_BANK stCusKey;
static U8 const u8MstarDefRSAImagePublicKey[RSA_PUBLIC_KEY_LEN]={
    0x84 ,0x62 ,0x76 ,0xCC ,0xBD ,0x5A ,0x5A ,0x40 ,0x30 ,0xC0 ,0x96 ,0x40 ,0x87 ,0x28 ,0xDB ,0x85 ,
    0xED ,0xED ,0x9F ,0x3E ,0xDE ,0x4E ,0x65 ,0xE6 ,0x7B ,0x1B ,0x78 ,0x17 ,0x87 ,0x9D ,0xF6 ,0x16 ,
    0xC3 ,0xD3 ,0x27 ,0xBC ,0xB4 ,0x5A ,0x3  ,0x13 ,0x35 ,0xB0 ,0x96 ,0x5A ,0x96 ,0x41 ,0x74 ,0x4E ,
    0xB9 ,0xD1 ,0x77 ,0x96 ,0xF7 ,0x8D ,0xE2 ,0xE7 ,0x15 ,0x9  ,0x65 ,0x9C ,0x46 ,0x79 ,0xEA ,0xF0 ,
    0x91 ,0x67 ,0x35 ,0xFA ,0x69 ,0x4C ,0x83 ,0xF7 ,0xDC ,0xCF ,0x97 ,0x20 ,0xF2 ,0xA5 ,0xBA ,0x72 ,
    0x80 ,0x9D ,0x55 ,0x79 ,0x17 ,0xDC ,0x6E ,0x60 ,0xA5 ,0xE7 ,0xE  ,0x9E ,0x89 ,0x9B ,0x46 ,0x6  ,
    0x52 ,0xFC ,0x64 ,0x56 ,0x2  ,0x8  ,0x9A ,0x96 ,0x41 ,0xE2 ,0x4F ,0xDB ,0xB6 ,0x60 ,0xC3 ,0x38 ,
    0xDF ,0xF4 ,0x97 ,0x81 ,0x5D ,0x12 ,0x2  ,0xAE ,0x2B ,0x9F ,0x9  ,0x29 ,0xB9 ,0x9D ,0x51 ,0x45 ,
    0xD2 ,0x9E ,0x2B ,0xAF ,0x64 ,0xCA ,0x9A ,0x6  ,0x4E ,0x94 ,0x35 ,0x67 ,0xF7 ,0x8E ,0x4  ,0x7B ,
    0x24 ,0x38 ,0xA0 ,0xDF ,0xE7 ,0x5F ,0x1E ,0x6D ,0x29 ,0x8E ,0x30 ,0xD7 ,0x83 ,0x8C ,0xB4 ,0x41 ,
    0xD2 ,0xFD ,0xBF ,0x5B ,0x18 ,0xCA ,0x50 ,0xD1 ,0x27 ,0xD1 ,0xF6 ,0x7D ,0x54 ,0x3E ,0x80 ,0x5F ,
    0x20 ,0xDC ,0x88 ,0x82 ,0xCF ,0xBE ,0xE1 ,0x46 ,0x2A ,0xD6 ,0x63 ,0xB9 ,0xB9 ,0x9D ,0xA3 ,0xC7 ,
    0x68 ,0x3E ,0x48 ,0xCE ,0x6A ,0x62 ,0x6F ,0xD1 ,0x6A ,0xC3 ,0xB6 ,0xDE ,0xF3 ,0x39 ,0x25 ,0xEC ,
    0xF6 ,0x79 ,0x20 ,0xB5 ,0xF2 ,0x30 ,0x25 ,0x6E ,0x99 ,0xAE ,0x39 ,0x56 ,0xDA ,0xAF ,0x83 ,0xD6 ,
    0xB8 ,0x49 ,0x15 ,0x78 ,0x81 ,0xCC ,0x3C ,0x4F ,0x66 ,0x5D ,0x95 ,0x7E ,0x31 ,0xD4 ,0x37 ,0x2A ,
    0xBE ,0xFC ,0xB4 ,0x66 ,0xF8 ,0x91 ,0x1  ,0xA  ,0x53 ,0x3C ,0x3C ,0xAB ,0x86 ,0xB9 ,0x80 ,0xB7 ,
    0x0  ,0x1  ,0x0  ,0x1};
static char idForContinueMode[]={'S','E','C','U','R','I','T','Y'};
static char idForInterleaveMode[]={'I','N','T','E','R','L','V','E'};
static U8 const u8AESBootHashKey[AES_KEY_LEN]={0xD2 ,0x9E ,0x2B ,0xAF ,0x64 ,0xCA ,0x9A ,0x6  ,0x4E ,0x94 ,0x35 ,0x67 ,0xF7 ,0x8E ,0x4  ,0x7B};

static char* RawData_partition[]={KL_PARTITION,
                                  TEE_PARTITION,
                                  "recovery",
                                  #if defined (CONFIG_SEC_SYSTEM)
                                  SECKL_PARTITION,
                                  #endif
                                  #if defined (CONFIG_ARMv8_ARM_TRUSTED_FIRMWARE) && defined (CONFIG_SECURITY_BOOT)
                                  "optee",
                                  "armfw",
                                  #endif
                                  #if defined (CONFIG_SECURITY_BOOT) && (ENABLE_STB_BOOT == 1)
                                  "RTPM",
                                  #endif
                                  "RFS",
                                  #if(ENABLE_MODULE_ANDROID_BOOT != 1)
                                  "cacert",
                                  #endif
                                  NULL};

#if (ENABLE_STB_BOOT == 1)
    static char* filesystem_partition[]={ "RFS","MWLIB","APP","LIB","CON",NULL };
#else
    static char* filesystem_partition[]={
                                     #if(ENABLE_MODULE_ANDROID_BOOT != 1)
                                     "MSLIB","APP","CONFIG",
                                     #endif
                                     NULL};
#endif

#ifdef CONFIG_TFTP_SECURE_CHECK
static char utest_err_msg[128];
#endif
static MS_BOOL secure_init_ok = FALSE;


static U8 u8MagicID[16]={0x4D,0x73,0x74,0x61,0x72,0x2E,0x4B,0x65,0x79,0x2E,0x42,0x61,0x6E,0x6B,0x2E,0x2E};
#define ENV_NCONFIG_INDEX            "nconfig_index"
#define ENV_NCONFIG_SIZE             "nconfig_size"
#define DEF_NCONFIG_BANK_SIZE        0x4000

//-------------------------------------------------------------------------------------------------
//  extern function
//-------------------------------------------------------------------------------------------------
extern int snprintf(char *str, size_t size, const char *fmt, ...);
extern int strcmp(const char * cs,const char * ct);
extern int ubi_get_leb_size(void);
//-------------------------------------------------------------------------------------------------
//  inline
//-------------------------------------------------------------------------------------------------

void Secure_HaltCPU(void)
{
    //show error UI
    run_command("destroy_logo",0);
    run_command("bootlogo 0 0 1 1 boot_error.jpg", 0);

#if defined(CONFIG_URSA6_VB1) || defined(CONFIG_URSA_UNION)
#if (CONFIG_URSA_UNION)
    if (MDrv_URSA_COM_Get_URSA_Type() == URSA_COMMON_U6)
#endif
    {
        run_command("send_I2C_cmd_to_ursa6" , 0);
    }
#endif

#if defined(CONFIG_URSA9_VB1) || defined(CONFIG_URSA20_VB1) || defined(CONFIG_URSA_UNION)
#if (CONFIG_URSA_UNION)
    if ((MDrv_URSA_COM_Get_URSA_Type() == URSA_COMMON_U9) || (MDrv_URSA_COM_Get_URSA_Type() == URSA_COMMON_U11) || (MDrv_URSA_COM_Get_URSA_Type() == URSA_COMMON_U20))
#endif
    {
#if defined(CONFIG_URSA20_VB1)
        run_command("oledLGOnRFDoneGPIO" , 0);
        run_command("send_I2C_cmd_to_ursa20" , 0);
#else
        run_command("send_I2C_cmd_to_ursa9" , 0);
#endif

    }
#endif

    run_command("panel_post_init",0);

#if (CONFIG_LOCAL_DIMMING)
    run_command("local_dimming", 0);
#endif
    //halt cpu
    while(1){;}
}


//-------------------------------------------------------------------------------------------------
//  Local function
//-------------------------------------------------------------------------------------------------
int ImageIOInit(char *partition);
int _ImageIOInitForEMMC(char *partition);
int _ImageIOInitForNand(char *partition);
int ImageRead(char *volume, unsigned int addr, unsigned int offset, unsigned len);
int _ImageRead(char *volume, unsigned int addr, unsigned int offset, unsigned len);
int ImageWrite(char *volume, unsigned int addr, unsigned int offset, unsigned len);
U32 GetBlockSize(void);
int GetSubImageNum(char *volume, U32 *num);
int GetImageFromStorage(char *volume, unsigned int addr, unsigned int number);
int _GetImageFromStorage(char *volume, unsigned int addr, unsigned int number, BOOLEAN bIsInterleaveMode);
int DecryptKernelImageByHouseKeeping(char *dataBuf, unsigned int len);
int DecryptKernelImageBySecureProcessor(char *dataBuf, unsigned int len);
int EnablePartialAuthentication(void);
int DisablePartialAuthentication(void);
BOOLEAN IsDoingPartialAuthentication(void);
int _RawData_Authentication(char *KLName,unsigned int u32kernelAddr);

int do_Authenticate_RawData(char *KLName, int CacheBufAddr);
int do_Authenticate_FileSystem(char *fileSystemName);

int IsImageRight(char *volume);
int CheckSignature(SECURITY_INFO *sign);
int _LoadCustomerKeyBank(U32 u32CusKeyOffset,U32 u32CusKeySize,U32 u32HwAesVABufAddr);
int RestoreCustomerKeyBank(U32 u32CusKeyOffset,U32 u32CusKeySize,U32 tempDRAMAddr);
BOOLEAN isCustomerKeyBankCipher(U32 keyBankOffset,U32 keyBankSize,U32 tempDRAMAddr);
void EnableAESDAMClock(void);

int Secure_Drv_Init(void);
int LoadSignature(void);
int LoadCustomerKeyBank(void);

void PopUpProtectedKey(void);
#if defined(CONFIG_SECURE_SPI_PROTECT)
void spi_write_protect_process_init(void);
#endif
BOOLEAN IsInterleaveMode(void);
EN_SECUREMERGE_TYPE GetSecureMergeType(void);
int GetSecureImage(unsigned int imageAddr, char * partition, U32 offset, U32 size, EN_SECUREMERGE_DATA secureMergeData);
void CheckSecureMerge(void);
int _EncryptCustomerKeyBank(U32 u32CusKeyOffset,U32 u32CusKeySize,U32 tempDRAMAddr);
int _DecryptCustomerKeyBank(U32 u32HwAesVABufAddr,U32 u32CusKeySize);


int EncryptCustomerKeyBank(U32 u32CusKeyOffset,U32 u32CusKeyBackUpOffset, U32 u32CusKeySize,U32 tempDRAMAddr);
int copyKeyBank(U32 destOffset, U32 srcOffset, U32 size, U32 tempDRAMAddr);
int setKeyBankProcessStatusDone(void);
int clearKeyBankProcessStatus(void);
int _setKeyBankProcessStatus(char value);

int IsKeyBankProcessDone(char *env, BOOLEAN*pDone);
int IsRawDataPartition(char* partition);
int IsFileSystemPartition(char* partition);
_SUB_SECURE_INFO*  getSubSecureInfo(char* partition,BOOLEAN bIsInterleaveMode);
void getSecIdentify(char* pSecIdentify,SECURITY_INFO *pBufferAddr,BOOLEAN bIsInterleaveMode);
U32 *getHash0PublicKeyN(void);
U32 *getHash0PublicKeyE(void);
BOOLEAN isCustomerKeyBankValid(CUSTOMER_KEY_BANK *pCusKey);

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

int IsRawDataPartition(char* partition)
{
    UBOOT_TRACE("IN\n");
    int ret = -1;
    int i;
    for(i = 0;RawData_partition[i] != NULL;i++)
    {
        if(0 == strcmp(partition,RawData_partition[i]))
        {
            ret = 0;
            break;
        }
    }
    if(0 == ret)
    {
        UBOOT_DEBUG("%s is RawData Partition\n",partition);
    }
    else
    {
        UBOOT_DEBUG("%s is not RawData Partition\n",partition);
    }
    UBOOT_TRACE("OK\n");
    return ret;
}

int IsFileSystemPartition(char* partition)
{
    UBOOT_TRACE("IN\n");
    int ret = -1;
    int i;
    for(i = 0;filesystem_partition[i] != NULL;i++)
    {
        if(0 == strcmp(partition,filesystem_partition[i]))
        {
            ret = 0;
            break;
        }
    }
    if(0 == ret)
    {
        UBOOT_DEBUG("%s is FileSystem Partition\n",partition);
    }
    else
    {
        UBOOT_DEBUG("%s is not FileSystem Partition\n",partition);
    }
    UBOOT_TRACE("OK\n");
    return ret;
}

_SUB_SECURE_INFO* getSubSecureInfo(char* partition,BOOLEAN bIsInterleaveMode)
{
    UBOOT_TRACE("IN\n");
    if(NULL == partition)
    {
        UBOOT_ERROR("args error\n");
        return NULL;
    }
    _SUB_SECURE_INFO *pSubAddr = NULL;

    if((GetSecureMergeType() == E_SECUREMERGE_NONE) || (IsFileSystemPartition(partition) == 0))
    {
#if (ENABLE_MODULE_ANDROID_BOOT == 1)
        if(0 == strcmp("boot",partition))
        {
            if(TRUE == bIsInterleaveMode)
            {
                pSubAddr = &(stSecureInfo.data_interleave.Boot);
            }
            else
            {
                pSubAddr = &(stSecureInfo.data.Boot);
            }

        }
#else
        if(0 == strcmp("KL",partition))
        {
            if(TRUE == bIsInterleaveMode)
            {
                pSubAddr = &(stSecureInfo.data_interleave.Kernel);
            }
            else
            {
                pSubAddr = &(stSecureInfo.data.Kernel);
            }
        }
        else if(0 == strcmp("RFS",partition))
        {
            if(TRUE == bIsInterleaveMode)
            {
                pSubAddr = &(stSecureInfo.data_interleave.RootFs);
            }
            else
            {
                pSubAddr = &(stSecureInfo.data.RootFs);
            }
        }
        else if(0 == strcmp("MSLIB",partition))
        {
            if(TRUE == bIsInterleaveMode)
            {
                pSubAddr = &(stSecureInfo.data_interleave.MsLib[0]);
            }
            else
            {
                pSubAddr = &(stSecureInfo.data.MsLib[0]);
            }
        }
        else if(0 == strcmp("MWLIB",partition))
        {
            if(TRUE == bIsInterleaveMode)
            {
                pSubAddr = &(stSecureInfo.data_interleave.MwLib[0]);
            }
            else
            {
                pSubAddr = &(stSecureInfo.data.MwLib[0]);
            }
        }
        else if(0 == strcmp("LIB",partition))
        {
            if(TRUE == bIsInterleaveMode)
            {
                pSubAddr = &(stSecureInfo.data_interleave.Lib[0]);
            }
            else
            {
                pSubAddr = &(stSecureInfo.data.Lib[0]);
            }
        }
        else if((0 == strcmp("CON",partition)) || (0 == strcmp("CONFIG",partition)))
        {
            if(TRUE == bIsInterleaveMode)
            {
                pSubAddr = &(stSecureInfo.data_interleave.Config[0]);
            }
            else
            {
                pSubAddr = &(stSecureInfo.data.Config[0]);
            }
        }
        else if(0 == strcmp("APP",partition))
        {
            if(TRUE == bIsInterleaveMode)
            {
                pSubAddr = &(stSecureInfo.data_interleave.App[0]);
            }
            else
            {
                pSubAddr = &(stSecureInfo.data.App[0]);
            }
        }
        else if(0 == strcmp("cacert",partition))
        {
            if(TRUE == bIsInterleaveMode)
            {
                pSubAddr = &(stSecureInfo.data_interleave.Cacert[0]);
            }
            else
            {
                pSubAddr = &(stSecureInfo.data.Cacert[0]);
            }
        }
#endif
#if defined (CONFIG_ARMv8_ARM_TRUSTED_FIRMWARE) && defined (CONFIG_SECURITY_BOOT)
        else if(0 == strcmp("armfw",partition))
        {
            if(TRUE == bIsInterleaveMode)
            {
                pSubAddr = &(stSecureInfo.data_interleave.ArmFw);
            }
            else
            {
                pSubAddr = &(stSecureInfo.data.ArmFw);
            }
        }
#endif
        else if(0 == strcmp(TEE_PARTITION,partition))
        {
            if(TRUE == bIsInterleaveMode)
            {
                pSubAddr = &(stSecureInfo.data_interleave.tee);
            }
            else
            {
                pSubAddr = &(stSecureInfo.data.tee);
            }
        }
#if (ENABLE_STB_BOOT == 1) && defined (CONFIG_SECURITY_BOOT)
        else if(0 == strcmp("RTPM",partition))
        {
            if(TRUE == bIsInterleaveMode)
            {
                pSubAddr = &(stSecureInfo.data_interleave.RTPM);
            }
            else
            {
                pSubAddr = &(stSecureInfo.data.RTPM);
            }
        }
#endif
        else if(0 == strcmp("recovery",partition))
        {
            if(TRUE == bIsInterleaveMode)
            {
                pSubAddr = &(stSecureInfo.data_interleave.Recovery);
            }
            else
            {
                pSubAddr = &(stSecureInfo.data.Recovery);
            }
        }
#if defined (CONFIG_SEC_SYSTEM)
        else if (0 == strcmp("secKL",partition))
        {
            UBOOT_DEBUG("Get secKL secure info\n");
            if(TRUE == bIsInterleaveMode)
            {
                pSubAddr = &(stSecureInfo.data_interleave.secKernel);
            }
            else
            {
                pSubAddr = &(stSecureInfo.data.secKernel);
            }
        }
#endif
        else
        {
            UBOOT_ERROR("invalid partiton name:%s\n",partition);
            return NULL;
        }
    }
    else
    {
        if(GetSecureImage((unsigned int)&stSubSecureInfo, partition, 0, sizeof(SUB_SECURE_INFO), E_SECUREINFO) == -1)
        {
            return NULL;
        }

        if(TRUE == bIsInterleaveMode)
        {
            pSubAddr = &(stSubSecureInfo.sInfo_Interleave);
        }
        else
        {
            pSubAddr = &(stSubSecureInfo.sInfo);
        }
    }
    UBOOT_TRACE("OK\n");
    return pSubAddr;
}

void getSecIdentify(char* pSecIdentify,SECURITY_INFO *pBufferAddr,BOOLEAN bIsInterleaveMode)
{
    UBOOT_TRACE("IN\n");
    #if (ENABLE_MODULE_ANDROID_BOOT == 1)
    if(TRUE == bIsInterleaveMode)
    {
        memcpy(pSecIdentify,pBufferAddr->data_interleave.Boot.u8SecIdentify,sizeof(pBufferAddr->data_interleave.Boot.u8SecIdentify));
    }
    else
    {
        memcpy(pSecIdentify,pBufferAddr->data.Boot.u8SecIdentify,sizeof(pBufferAddr->data.Boot.u8SecIdentify));
    }
    #else
    if(TRUE == bIsInterleaveMode)
    {
        memcpy(pSecIdentify,pBufferAddr->data_interleave.Kernel.u8SecIdentify,sizeof(pBufferAddr->data_interleave.Kernel.u8SecIdentify));
    }
    else
    {
        memcpy(pSecIdentify,pBufferAddr->data.Kernel.u8SecIdentify,sizeof(pBufferAddr->data.Kernel.u8SecIdentify));
    }
    #endif
    UBOOT_TRACE("OK\n");
    return;
}

int FileSystem_Authentication(void)
{
    int ret=0;
    UBOOT_TRACE("IN\n");

    ret=ImageIOInit("UBI");
    if(ret==-1)
    {
        UBOOT_ERROR("Image IO init fail\n");
        Secure_HaltCPU();
    }
    int i;
    for(i = 0;filesystem_partition[i] != NULL;i++){
        ret=IsImageRight(filesystem_partition[i]);
        if(-1 == ret)
        {
            UBOOT_ERROR("Authentication is fail in %s\n",filesystem_partition[i]);
            Secure_HaltCPU();
        }
    }

    EnablePartialAuthentication();

    UBOOT_TRACE("OK\n");
    return 0;
}

int IsImageRight(char *volume)
{
    UBOOT_TRACE("IN\n");
    U32 numOfSubImage=0;
    U32 size=0;
    U8 i=0;
    U8 u8ImagePublicKeyN[RSA_PUBLIC_KEY_N_LEN];
    U8 u8ImagePublicKeyE[RSA_PUBLIC_KEY_E_LEN];
    U8 u8ImageSignature[SIGNATURE_LEN];
    U32 partition_num;
    int ret=-1;
    MS_BOOL skipPartialAuthen=FALSE;
    UBOOT_TRACE("IN\n");

    uint32_t auth_addr = FILESYSTEM_AUTHENTICATION_BUFFER_ADDR;
    get_temp_used_addr(&auth_addr);
    UBOOT_DEBUG("auth_addr=0x%08x\n", auth_addr);

    if(volume==0)
    {
        UBOOT_ERROR("The input of parameter is a null pointer\n");
        return ret;
    }
    ret=GetSubImageNum(volume,&numOfSubImage);
    if(ret!=0)
    {
        UBOOT_ERROR("get sub image number fail\n");
        return -1;
    }
    partition_num=getRandNum();
    partition_num=(partition_num)%numOfSubImage;
    UBOOT_INFO("partition_num=%d\n",partition_num);
    skipPartialAuthen=IsDoingPartialAuthentication();

    for(i=0;i<numOfSubImage;i++){

        if(skipPartialAuthen==TRUE){
            if(i!=partition_num){
                continue;
            }
        }
        ret=GetImageSize(volume,i, &size);
        if(ret!=0)
        {
            UBOOT_ERROR("Get image size fail\n");
            return ret;
        }
        ret=GetImageFromStorage(volume,auth_addr,i);
        if(ret!=0)
        {
            UBOOT_ERROR("Get image fail\n");
            return ret;
        }
        ret=GetPublicKeyN(E_RSA_IMAGE_PUBLIC_KEY, u8ImagePublicKeyN, sizeof(u8ImagePublicKeyN));
        if(ret!=0)
        {
            UBOOT_ERROR("Get public key N fail\n");
            return ret;
        }
        ret=GetPublicKeyE(E_RSA_IMAGE_PUBLIC_KEY, u8ImagePublicKeyE, sizeof(u8ImagePublicKeyE));
        if(ret!=0)
        {
            UBOOT_ERROR("Get public key E fail\n");
            return ret;
        }
        ret=GetSignature(volume, i, u8ImageSignature, sizeof(u8ImageSignature));
        if(ret!=0)
        {
            UBOOT_ERROR("Get signature fail\n");
            return ret;
        }
        if (-1 == Secure_doAuthentication(u8ImagePublicKeyN, u8ImagePublicKeyE,u8ImageSignature, (U8 *)auth_addr, (U32)size))
        {
            UBOOT_ERROR("Auth. %s FAIL\n",volume);
            DisablePartialAuthentication();

            UBOOT_DEBUG("\033[0;31m Image public-N key=\033[0m\n");
            UBOOT_DUMP((U32)u8ImagePublicKeyN, RSA_PUBLIC_KEY_N_LEN);

            UBOOT_DEBUG("\033[0;31m Image public-E key=\033[0m\n");
            UBOOT_DUMP((U32)u8ImagePublicKeyE, RSA_PUBLIC_KEY_E_LEN);

            UBOOT_DEBUG("\033[0;31m %s[%d] signature=\033[0m\n",volume, i);
            UBOOT_DUMP((U32)u8ImageSignature, SIGNATURE_LEN);

            UBOOT_DEBUG("\033[0;31m %s[%d] image=\033[0m\n",volume, i);
            UBOOT_DUMP((U32)auth_addr, 0x100);
            return -1;

        }
        else
        {
            #if defined(CONFIG_SECURE_DEBUG_MESSAGE)
            UBOOT_INFO("\033[0;34m*****************************\033[0m\n");
            UBOOT_INFO("\033[0;34m* %s Authentication SUCCESS !! *\033[0m\n",volume);
            UBOOT_INFO("\033[0;34m*****************************\033[0m\n");
            #endif
        }
    }
    UBOOT_TRACE("OK\n");
    return 0;
}

int do_AN_Authenticate(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    int ret = -1;
    U32 addr=0;
    UBOOT_TRACE("IN\n");

    if(argc!=2)
    {
        cmd_usage(cmdtp);
        return -1;
    }

    addr=simple_strtoul(argv[1], NULL, 16);
    if(isBootToRecovery())
    {
        ret=RawData_Authentication("recovery",addr);
    }
    else
    {
        ret=RawData_Authentication("boot",addr);
    }

    if(-1 == ret)
    {
        UBOOT_ERROR("authenticate %s fail\n",argv[1]);
        return -1;
    }
    else
    {
        UBOOT_TRACE("OK\n");
        return 0;
    }

}


#if (CONFIG_VERIFY_SYSTEM == 1)
int do_Verify_System(char* partition, int CacheBufAddr)
{
    #define SYSTEM_SIGNED_AREA_BEGIN     1024
    #define SYSTEM_SIGNED_AREA_SZIE      0x200000

    typedef struct
    {
       unsigned char u8CrcIdentify[4];
       unsigned int  u32CrcValue;
    }CRC_INFO;

    U32 ret, offset, image_size, crc;
    U8  *VirtWorkBuf = NULL;
    U8  tmp_buf[256];

    // step1. Get verify image size
    UBOOT_TRACE("IN\n");
    image_size = SYSTEM_SIGNED_AREA_SZIE - SYSTEM_SIGNED_AREA_BEGIN;
    if(CacheBufAddr == 0){
        VirtWorkBuf = (U8 *)FILESYSTEM_AUTHENTICATION_BUFFER_ADDR;
    }
    else{
        VirtWorkBuf = (U8 *)CacheBufAddr;
    }

    // step2. Read image into buffer
    raw_io_config_push();
    ret = raw_io_config(FLASH_RAW_DATA_TARGET, partition, UNNECESSARY_VOLUME);
    if(ret != 0){
        UBOOT_ERROR("raw_io_config setting fail!\n");
        return -1;
    }
    ret = raw_read((U32)tmp_buf, 0, 256);
    if(ret != 0){
        UBOOT_ERROR("raw_read %s secure image fail!\n", partition);
        return -1;
    }
    CRC_INFO *pcrc = (CRC_INFO *)tmp_buf;

    offset = SYSTEM_SIGNED_AREA_BEGIN;
    ret = raw_read((U32)VirtWorkBuf, offset, image_size);
    if(ret != 0){
        UBOOT_ERROR("raw_read %s secure image fail!\n", partition);
        return -1;
    }
    raw_io_config_pop();

    // step3. Verify system partition
    crc = crc32(0, VirtWorkBuf, image_size);
    UBOOT_DEBUG("Calculate crc : 0x%x , Image crc : 0x%x\n", crc, pcrc->u32CrcValue);
    if (pcrc->u32CrcValue != crc)
    {
        UBOOT_ERROR("\033[0;31m Image CRC Block =\033[0m\n");
        UBOOT_ERRDUMP((U32)tmp_buf,     0x10);
        UBOOT_ERROR("\033[0;31m Image Suprer Block =\033[0m\n");
        UBOOT_ERRDUMP((U32)VirtWorkBuf, 0x100);
        Secure_HaltCPU();
        return -1;
    }
    UBOOT_TRACE("OK\n");
    return 0;
}
#endif

int do_Authenticate(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    UBOOT_TRACE("IN\n");
    int ret = -1;
    uint32_t CacheBufAddr =0;

    if((argc<2)||(argc>3))
    {
        cmd_usage(cmdtp);
        return -1;
    }

    if(argc==3)
    {
        CacheBufAddr=simple_strtoul(argv[2], NULL, 16);
    }

    if(0 == IsRawDataPartition(argv[1]))
    {
        ret = do_Authenticate_RawData(argv[1],CacheBufAddr);
    }
    else if(0 == IsFileSystemPartition(argv[1]))
    {
        ret = do_Authenticate_FileSystem(argv[1]);
    }
    else
    {
         UBOOT_ERROR("Invalid volume name:%s.\n",argv[1]);
         ret = -1;
    }

    if(ret!=0)
    {
        UBOOT_ERROR("authenticate %s fail\n",argv[1]);
        #if defined (CONFIG_DUAL_SYSTEM)
        run_command("mboot_verify_check", 0);
        #endif
        Secure_HaltCPU();
    }

    UBOOT_TRACE("OK\n");
    return 0;
}

int do_Authenticate_RawData(char* KLName, int CacheBufAddr)
{
    UBOOT_TRACE("IN\n");
    U32 kernelSize = 0;
    int ret=0;
    int VirtWorkBuf=0;

    uint32_t auth_addr = FILESYSTEM_AUTHENTICATION_BUFFER_ADDR;
    get_temp_used_addr(&auth_addr);
    UBOOT_DEBUG("auth_addr=0x%08x\n", auth_addr);

    ret = GetImageSize(KLName,0, &kernelSize);
    if (0 != ret)
    {
        UBOOT_ERROR("get %s size fail\n",KLName);
        return -1;
    }

    if(CacheBufAddr==0){
        VirtWorkBuf =auth_addr;
    }
    else{
        VirtWorkBuf=CacheBufAddr;
    }

    if(-1 == GetSecureImage((unsigned int)VirtWorkBuf, KLName, 0, kernelSize, E_SECUREIMAGE))
    {
        UBOOT_ERROR("read kernel fail\n");
        return -1;
    }
#if !defined (CONFIG_SECURE_BY_PASS_VERIFICATION)// && (!defined(CONFIG_SECURE_SMC_HW_IP))
    ret = _RawData_Authentication(KLName,(unsigned int)VirtWorkBuf);
    if(-1 == ret)
    {
        UBOOT_ERROR("Authenticate %s fail\n",KLName);
        #if defined (CONFIG_DUAL_SYSTEM)
        run_command("mboot_verify_check", 0);
        #endif
        Secure_HaltCPU();
        return -1;
    }
#endif
    UBOOT_TRACE("OK\n");
    return 0;
}

int do_Authenticate_FileSystem(char *fileSystemName)
{
    int ret=0;
    UBOOT_TRACE("IN\n");

    if(fileSystemName==NULL)
    {
        UBOOT_ERROR("The input parameter is a null pointer\n");
        return -1;
    }

    ret=ImageIOInit("UBI");
    if(ret==-1)
    {
        UBOOT_ERROR("Image IO init fail\n");
        return -1;
    }

    ret=IsImageRight(fileSystemName);
    if(ret==-1)
    {
        UBOOT_ERROR("Authenticate %s fail\n",fileSystemName);
        return -1;
    }

    UBOOT_TRACE("OK\n");
    return 0;
}



int do_verify_sha256(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    unsigned int intAddr;
    unsigned int intLen;
    unsigned int outAddr;
    unsigned int count;
    unsigned int i,j;
    int ret=0;
    UBOOT_TRACE("IN\n");
    if(argc<5) {
        cmd_usage(cmdtp);
        return -1;
    }

    intAddr=simple_strtoul(argv[1], NULL, 16);
    outAddr=simple_strtoul(argv[2], NULL, 16);
    intLen=simple_strtoul(argv[3], NULL, 16);
    count=simple_strtoul(argv[4], NULL, 16);
    UBOOT_DEBUG("intAddr=0x%x,outAddr=0x%x,intLen=0x%x,count=0x%x\n",intAddr,outAddr,intLen,count);

    j=1;
    while(count--)
    {
        intAddr=simple_strtoul(argv[1], NULL, 16);
        outAddr=simple_strtoul(argv[2], NULL, 16);
        intLen=simple_strtoul(argv[3], NULL, 16);

        ret=CommonSHA(intAddr, outAddr, intLen);
        UBOOT_INFO("count=%d\n",j);
        for(i=0;i<32;i++,outAddr++){
            UBOOT_INFO("0x%x ",*(unsigned char *)outAddr);
        }

        UBOOT_INFO("\n");
        j++;
        if(ret!=0)
        {
            break;
        }
    }

    if(ret==0)
    {
        UBOOT_TRACE("OK\n");
    }
    else
    {
        UBOOT_ERROR("verify fail\n");
    }
    return 0;
}


int do_sha256(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    unsigned int intAddr;
    unsigned int intLen;
    unsigned int outAddr;
    unsigned int i;
    UBOOT_TRACE("IN\n");
    if(argc<4) {
        cmd_usage(cmdtp);
        return -1;
    }
    intAddr=simple_strtoul(argv[1], NULL, 16);
    outAddr=simple_strtoul(argv[2], NULL, 16);
    intLen=simple_strtoul(argv[3], NULL, 16);

    CommonSHA(intAddr, outAddr, intLen);

    for(i=0;i<32;i++,outAddr++){
        UBOOT_INFO("0x%x ",*(unsigned char *)outAddr);
    }

    UBOOT_INFO("\n");
    UBOOT_TRACE("OK\n");
    return 0;
}


int do_rsa(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{

    unsigned int signAddr;
    unsigned int keyAddr;
    unsigned int outputAddr;
    unsigned int i;
    UBOOT_TRACE("IN\n");
    if(argc<4) {
        cmd_usage(cmdtp);
        return -1;
    }
    UBOOT_INFO("\n---- rsa 256 cacluate -----\n");
    signAddr=simple_strtoul(argv[1], NULL, 16);
    keyAddr=simple_strtoul(argv[2], NULL, 16);
    outputAddr=simple_strtoul(argv[3], NULL, 16);

    rsa_main((unsigned char *)signAddr, (unsigned char *)keyAddr, (unsigned char *)(keyAddr+256), (unsigned char *)outputAddr);
    for(i=0;i<32;i++,outputAddr++){
        UBOOT_INFO("0x%x ",*(unsigned char *)outputAddr);
    }
    UBOOT_INFO("\n");
    UBOOT_TRACE("OK\n");
    return 0;

}

U32 *getHash0PublicKeyN(void)
{
    UBOOT_TRACE("IN\n");
    UBOOT_DEBUG("MS_RIU_MAP is 0x%x\n",(U32)MS_RIU_MAP);
    UBOOT_DEBUG("(U32)RIU[(POR_STATUS_BASE_ADDR+(0xB<<1))]=0x%x\n",((U32)RIU[(POR_STATUS_BASE_ADDR+(0xB<<1))]));
    UBOOT_TRACE("OK\n");
    return (U32 *)(CONFIG_SRAM_BASE_ADDRESS+((U32)RIU[(POR_STATUS_BASE_ADDR+(0xB<<1))]));
}

U32 *getHash0PublicKeyE(void)
{
    UBOOT_TRACE("IN\n");

    UBOOT_DEBUG("MS_RIU_MAP is 0x%x\n",(U32)MS_RIU_MAP);
    UBOOT_DEBUG("(U32)RIU[(POR_STATUS_BASE_ADDR+(0xA<<1))]=0x%x\n",((U32)RIU[(POR_STATUS_BASE_ADDR+(0xA<<1))]));
    UBOOT_TRACE("OK\n");
    return (U32 *)(CONFIG_SRAM_BASE_ADDRESS+((U32)RIU[(POR_STATUS_BASE_ADDR+(0xA<<1))]));
}

#if defined(CONFIG_AUTH_KEYBANK_RSA_KEY_FROM_HASH2)
/*
 * 'CONFIG_AUTH_KEYBANK_RSA_KEY_FROM_HASH2'
 * is defined in
 * MBoot/MstarCore/src/hal/${CHIP}/${CHIP}.h
 */

U32 *getHash2PublicKeyN(void)
{
    U32 rsa_pub_keyN = 0;
    stb_hash2_header *hash2_header = NULL;

    hash2_header = (stb_hash2_header *)CONFIG_HASH2_START_ADDRESS;
    rsa_pub_keyN = hash2_header->rsa_pub_dram_addr;
    UBOOT_TRACE("IN\n");
    UBOOT_DEBUG("CONFIG_HASH2_START_ADDRESS is 0x%x\n", CONFIG_HASH2_START_ADDRESS);
    UBOOT_DEBUG("The DRAM_ADDR of rsa_pub_keyN is 0x%x\n", rsa_pub_keyN);
    UBOOT_TRACE("OK\n");
    return (U32 *)rsa_pub_keyN;
}

U32 *getHash2PublicKeyE(void)
{
    U32 rsa_pub_keyE = 0;
    stb_hash2_header *hash2_header = NULL;

    hash2_header = (stb_hash2_header *)CONFIG_HASH2_START_ADDRESS;
    rsa_pub_keyE = (hash2_header->rsa_pub_dram_addr + RSA_PUBLIC_KEY_N_LEN);
    UBOOT_TRACE("IN\n");
    UBOOT_DEBUG("CONFIG_HASH2_START_ADDRESS is 0x%x\n", CONFIG_HASH2_START_ADDRESS);
    UBOOT_DEBUG("The DRAM_ADDR of rsa_pub_keyE is 0x%x\n", rsa_pub_keyE);
    UBOOT_TRACE("OK\n");
    return (U32 *)rsa_pub_keyE;
}
#endif


BOOLEAN isCustomerKeyBankValid(CUSTOMER_KEY_BANK *pCusKey)
{
    #define SIZE (sizeof(CUSTOMER_KEY_BANK)-sizeof(_SUB_SECURE_INFO))

    UBOOT_TRACE("IN\n");

    U32  *pHash0E = NULL;
    U32  *pHash0N = NULL;
    U32 publicKeyN[(RSA_PUBLIC_KEY_N_LEN/4)];
    U32 publicKeyE[(RSA_PUBLIC_KEY_E_LEN/4)];
    U32 idx=0;

    if(pCusKey==NULL)
    {
        UBOOT_ERROR("pCusKey is a null pointer\n");
        return FALSE;
    }

#if defined(CONFIG_AUTH_KEYBANK_RSA_KEY_FROM_HASH2)
    U32  *pHash2N = getHash2PublicKeyN();
    U32  *pHash2E = getHash2PublicKeyE();
    memcpy((U8 *)publicKeyN, pHash2N, RSA_PUBLIC_KEY_N_LEN);
    memcpy((U8 *)publicKeyE, pHash2E, RSA_PUBLIC_KEY_E_LEN);
#else
    pHash0E = getHash0PublicKeyE();
    pHash0N = getHash0PublicKeyN();
    UBOOT_DEBUG("pHash0E is at 0x%x\n",(U32)pHash0E);
    UBOOT_DEBUG("pHash0N is at 0x%x\n",(U32)pHash0N);
    for(idx=0;idx<(RSA_PUBLIC_KEY_N_LEN/4);idx++)
    {
        publicKeyN[(RSA_PUBLIC_KEY_N_LEN/4)-1-idx]=*pHash0N;
        pHash0N++;
    }

    for(idx=0;idx<(RSA_PUBLIC_KEY_E_LEN/4);idx++)
    {
        publicKeyE[(RSA_PUBLIC_KEY_E_LEN/4)-1-idx]=*pHash0E;
        pHash0E++;
    }
#endif

    UBOOT_DUMP((U32)publicKeyE,0x100);
    UBOOT_DUMP((U32)publicKeyN,0x100);

    if (-1 == Secure_doAuthentication((U8 *)publicKeyN, (U8 *)publicKeyE, pCusKey->customer.u8Signature, (U8 *)pCusKey->u8RSABootPublicKey, SIZE))
    {
        UBOOT_ERROR("\033[0;31m Auth CustomerKeyBank FAIL\033[0m\n");
        UBOOT_ERROR("\033[0;31m RSA image public-N key=\033[0m\n");
        UBOOT_ERRDUMP((U32)publicKeyN, RSA_PUBLIC_KEY_N_LEN);
        UBOOT_ERROR("\033[0;31m RSA image public-E key=\033[0m\n");
        UBOOT_ERRDUMP((U32)publicKeyE, RSA_PUBLIC_KEY_E_LEN);
        UBOOT_ERROR("\033[0;31m CustomerKeyBank signature=\033[0m\n");
        UBOOT_ERRDUMP((U32)pCusKey->customer.u8Signature, SIGNATURE_LEN);
        UBOOT_ERROR("\033[0;31m clear CustomerKeyBank image=\033[0m\n");
        UBOOT_ERRDUMP((U32)pCusKey->u8RSABootPublicKey,SIZE);//dump key bank
        #if defined (CONFIG_DUAL_SYSTEM)
        UBOOT_ERROR("isCustomerKeyBankValid fail\n");
        run_command("mboot_verify_check", 0);
        #endif
        jump_to_console();
        return FALSE;
    }
    UBOOT_DEBUG("Customer keybank is valid\n");
    UBOOT_TRACE("OK\n");
    return TRUE;

    #undef SIZE
}
BOOLEAN isCustomerKeyBankCipher(U32 keyBankOffset,U32 keyBankSize,U32 tempDRAMAddr)
{
    CUSTOMER_KEY_BANK *pCusKey = NULL;
    UBOOT_TRACE("IN\n");
    UBOOT_DEBUG("keyBankOffset=0x%x\n",keyBankOffset);
    UBOOT_DEBUG("keyBankSize=0x%x\n",keyBankSize);
    pCusKey = (CUSTOMER_KEY_BANK *)tempDRAMAddr;
    #if defined (CONFIG_DOUBLE_MBOOT) && !defined (CONFIG_DUAL_SYSTEM)
    mbootbak_raw_io_Config();
    #endif
    raw_read(tempDRAMAddr,keyBankOffset,keyBankSize);
    if(0 != memcmp(u8MagicID,pCusKey->u8MagicID,sizeof(u8MagicID)))
    {
         UBOOT_DEBUG("Customer KeyBank is cipher text \n");
         UBOOT_TRACE("OK\n");
         return TRUE;
    }

    UBOOT_DEBUG("Customer KeyBank is plain text \n");
    UBOOT_TRACE("OK\n");
    return FALSE;
}


int _EncryptCustomerKeyBank(U32 u32CusKeyOffset,U32 u32CusKeySize,U32 tempDRAMAddr)
{
    U8 pu8Key[AES_KEY_LEN]="\0";
    UBOOT_TRACE("IN\n");

    //Encrpt the Customer key bank, and
    //isCustomerKeyBankCipher had already loaded the CustomerKeyBank into stCusKey
    if(IsHouseKeepingBootingMode()==TRUE)
    {
        Secure_Get_DeviceKey(pu8Key);
        Secure_AES_ECB_Encrypt(tempDRAMAddr,u32CusKeySize,pu8Key);
    }
    else
    {
        //In this case, customer keybank is encrypted by secure processor(8051),
        //and secure processor encrypt 64KBytes data one time.
        u32CusKeySize=CUSTOMER_KEY_BANK_LEN;
        SECUREBOOT_MbxRetStruct SecureBootMbxRetStruct;
        raw_read(tempDRAMAddr,u32CusKeyOffset,u32CusKeySize);
        flush_cache(tempDRAMAddr,u32CusKeySize);
        Secure_MailBox_ASSIGN_DECRYPTED_KEY(&SecureBootMbxRetStruct, E_AES_EFUSE_KEY, AES_KEY_LEN,E_KEY_NO_SWAP);
        Secure_MailBox_AESEncode(&SecureBootMbxRetStruct, VA2PA(tempDRAMAddr),u32CusKeySize , SECURE_ENG_AES_MODE_ECB);
    }

    UBOOT_TRACE("OK\n");
    return 0;
}

int _DecryptCustomerKeyBank(U32 u32HwAesVABufAddr,U32 u32CusKeySize)
{
    U8 pu8Key[AES_KEY_LEN]="\0";
    UBOOT_TRACE("IN\n");

    Secure_Get_DeviceKey(pu8Key);
    Secure_AES_ECB_Decrypt(u32HwAesVABufAddr,u32CusKeySize,pu8Key);

    UBOOT_DUMP(u32HwAesVABufAddr,32);
    UBOOT_TRACE("OK\n");
    return 0;
}


int EncryptCustomerKeyBank(U32 u32CusKeyOffset,U32 u32CusKeyBackUpOffset, U32 u32CusKeySize,U32 tempDRAMAddr)
{
    UBOOT_TRACE("IN\n");
    if(_EncryptCustomerKeyBank(u32CusKeyBackUpOffset,u32CusKeySize,tempDRAMAddr)==-1)
    {
        UBOOT_ERROR("encrypt key bank fail\n");
        return  -1;
    }

    #if defined (CONFIG_DOUBLE_MBOOT) && !defined (CONFIG_DUAL_SYSTEM)
    mbootbak_raw_io_Config();
    #endif
    if(raw_write(tempDRAMAddr,u32CusKeyOffset,u32CusKeySize)!=0)
        {
            UBOOT_ERROR("fail\n");
            return -1;
        }

    if(raw_write(tempDRAMAddr,u32CusKeyBackUpOffset,u32CusKeySize)!=0)
    {
         UBOOT_ERROR("encrypt key bank backup fail\n");
         return  -1;
    }

#if defined (CONFIG_MSTAR_ROM_BOOT_WITH_NAND_FLASH)
    // Because system has double mboot in rom boot with nand,
    // so we mustdecrypt another keybank, too.
        raw_io_config_push();

        raw_io_config(E_RAW_DATA_IN_NAND,"MBOOTBAK",UNNECESSARY_VOLUME);
        if(raw_write(tempDRAMAddr,u32CusKeyOffset,u32CusKeySize)!=0)
        {
            UBOOT_ERROR("encrypt key bank backup fail in MBOOTBAK\n");
            return -1;
        }

        if(raw_write(tempDRAMAddr,u32CusKeyBackUpOffset,u32CusKeySize)!=0)
        {
             UBOOT_ERROR("encrypt key bank backup fail in MBOOTBAK\n");
             return  -1;
        }

        raw_io_config_pop();
#endif

    UBOOT_TRACE("OK\n");
    return 0;
}

int copyKeyBank(U32 destOffset, U32 srcOffset, U32 size, U32 tempDRAMAddr)
{
    UBOOT_TRACE("IN\n");
    if(raw_read(tempDRAMAddr,srcOffset,size)!=0)
    {
        UBOOT_ERROR("fail\n");
        return -1;
    }

    if(raw_write(tempDRAMAddr,destOffset,size)!=0)
    {
        UBOOT_ERROR("fail\n");
        return -1;
    }

    UBOOT_TRACE("OK\n");
    return 0;
}

int setKeyBankProcessStatusDone(void)
{
    UBOOT_TRACE("IN\n");

    if(_setKeyBankProcessStatus(1)!=0)
    {
         UBOOT_ERROR("sete down\n");
         return -1;
    }
    UBOOT_TRACE("OK\n");
    return 0;
}

int clearKeyBankProcessStatus(void)
{
    UBOOT_TRACE("IN\n");

    if(_setKeyBankProcessStatus(0)!=0)
    {
         UBOOT_ERROR("sete down\n");
         return -1;
    }
    UBOOT_TRACE("OK\n");
    return 0;
}


int _setKeyBankProcessStatus(char value)
{
    UBOOT_TRACE("IN\n");
    char buf[CMD_BUF];

    raw_io_config_push();
    snprintf(buf,CMD_BUF,"setenv %s %x;saveenv",ENV_KEYBANK_DONE,value);
    if(run_command(buf,0)!=0)
    {
         UBOOT_ERROR("setenv fail\n");
         return -1;
    }
    raw_io_config_pop();
    UBOOT_TRACE("OK\n");
    return 0;
}

int IsKeyBankProcessDone(char *env, BOOLEAN *pDone)
{
    UBOOT_TRACE("IN\n");
    char *ptr=NULL;
    if(env==NULL)
    {
        *pDone=FALSE;
        UBOOT_ERROR("env is a null pointer\n");
        return -1;
    }
    if(pDone==NULL)
    {
        UBOOT_ERROR("pDone is a null pointer\n");
        return -1;
    }
    ptr=getenv(env);
    if(ptr==NULL)
    {
       *pDone=FALSE;

    }
    else
    {
        if(strcmp(ptr,"1")==0)
        {
            *pDone=TRUE;
        }
        else
        {
            *pDone=FALSE;
        }
    }
    UBOOT_TRACE("OK\n");
    return 0;

}


int RestoreCustomerKeyBank(U32 u32CusKeyOffset,U32 u32CusKeySize,U32 tempDRAMAddr)
{
// Here,there are 3 cases  that will be happened on customer keybank.
// case 1: KeyBank, and KeyBankBackup are both plaintext.
// case 2: keyBank is not plaintext, and KeyBankBackup is plaintext
// case 3: keyBank and KeyBankBackup aren't both plaintext, but env doesn't appear that the status is done.
// In case 1, uboot will encrypt these two keybanks, and setup the status on env.
// In case 2, uboot asssumes that something happened during writting keyBank, and uboot recover keyBank by keyBankBackup.
// In case 3, uboot asssumes that something happened during writting keyBankBackup, and uboot recover keyBankBackup by KeyBank.
#define KEY_BANK_OFFSET u32CusKeyOffset
#define KEY_BANK_BACKUP_OFFSET (u32CusKeyOffset+CUSTOMER_KEY_BANK_LEN)
    U8 u8Encrypt=0;
    U8 u8Restore=0;
    BOOLEAN IsKeyBankDone=FALSE;

    UBOOT_TRACE("IN\n");
    UBOOT_DEBUG("u32CusKeyOffset=0x%x\n",u32CusKeyOffset);

    if(isCustomerKeyBankCipher(KEY_BANK_OFFSET,u32CusKeySize,tempDRAMAddr)== FALSE)
    {
        u8Encrypt++;
        UBOOT_DEBUG("Keybank doesn't encrypt\n");
    }

    if(isCustomerKeyBankCipher(KEY_BANK_BACKUP_OFFSET,u32CusKeySize,tempDRAMAddr)== FALSE)
    {
        u8Encrypt++;
        UBOOT_DEBUG("KeybankBackup doesn't encrypt\n");
    }
    //"IsKeyBankProcessDone" must be behind of "isCustomerKeyBankCipher"
    if(IsKeyBankProcessDone(ENV_KEYBANK_DONE,&IsKeyBankDone)==-1)
    {
        UBOOT_ERROR("fail\n");
        return -1;
    }

    if(IsKeyBankDone==FALSE)
    {
        u8Restore++;
    }



    if(u8Encrypt>0)
    {
        UBOOT_INFO("Customer key bank is not encrypted\n");
        UBOOT_DUMP((unsigned int)tempDRAMAddr,32);
        clearKeyBankProcessStatus();
        if(EncryptCustomerKeyBank(KEY_BANK_OFFSET,KEY_BANK_BACKUP_OFFSET,u32CusKeySize,tempDRAMAddr)==-1)
        {
            UBOOT_ERROR("Encrypt key bank fail\n");
            return  -1;
        }

        if( (isCustomerKeyBankCipher(KEY_BANK_OFFSET,u32CusKeySize,tempDRAMAddr)== FALSE)
          ||(isCustomerKeyBankCipher(KEY_BANK_BACKUP_OFFSET,u32CusKeySize,tempDRAMAddr)== FALSE) )
        {
            printf(" KeyBank is still plant data! Boot-Up is forbidden!\n");
            /***************************************************
            If:
            1. KeyBank is plant data
            2. After encrypt(or, re-encrypt) the KeyBank, it's still plant data
            then the secure booting is stopped due to KeyBank cannot be trusted
            ****************************************************/
            jump_to_console();
            return -1;
        }

        IsKeyBankDone = FALSE;
    }
    else
    {
        if(u8Restore>0)
        {
            if(copyKeyBank(KEY_BANK_BACKUP_OFFSET, KEY_BANK_OFFSET, u32CusKeySize, tempDRAMAddr)==-1)
            {
                UBOOT_ERROR("copy key bank fail\n");
                return -1;
            }
        }
    }

    if(FALSE == IsKeyBankDone)
    {
        if(setKeyBankProcessStatusDone()==-1)
        {
            UBOOT_ERROR("fail\n");
            return -1;
        }
    }


    UBOOT_TRACE("OK\n");
    return 0;
}



int Secure_Drv_Init(void)
{
    BOOLEAN ret=TRUE;
    UBOOT_TRACE("IN\n");

    if(IsHouseKeepingBootingMode()==FALSE)
    {
        ret=Secure_MailBox_Init();
    }

    EnableAESDAMClock();

    if(ret==FALSE)
    {
        UBOOT_ERROR("Secure_Drv_Init fail\n");
        return -1;
    }

    UBOOT_TRACE("OK\n");
    return 0;
}

int LoadSignature(void)
{
    UBOOT_TRACE("IN\n");
#if !defined (CONFIG_SECURE_BY_PASS_VERIFICATION)
    if(SignatureLoad((SECURITY_INFO *)&stSecureInfo)!=EN_SUCCESS)
    {
        UBOOT_ERROR("Load signature fail\n");
        return -1;
    }

    if(CheckSignature((SECURITY_INFO *)&stSecureInfo) == -1)
    {
        UBOOT_ERROR("Check signature fail\n");
        return -1;
    }
#else
    UBOOT_INFO("\n\nCONFIG_SECURE_BY_PASS_VERIFICATION MODE!!\n\n");
#endif
    UBOOT_TRACE("OK\n");
    return 0;
    }
int CopyCustomerKeyBank(U32 u32DramAddr)
{

    UBOOT_TRACE("IN\n");
    if(0 != memcmp(u8MagicID,stCusKey.u8MagicID,sizeof(u8MagicID)))
    {
        UBOOT_ERROR("Customer keybank is not ready");
        return -1;
    }
    UBOOT_DEBUG("u32DramAddr =%x\n",u32DramAddr);
    memcpy((void *)u32DramAddr,&stCusKey,sizeof(CUSTOMER_KEY_BANK));
    flush_cache(u32DramAddr,(U32)sizeof(CUSTOMER_KEY_BANK));

    UBOOT_TRACE("OK\n");
    return 0;
}

int LoadCustomerKeyBank(void)
{
    U32 u32CusKeyOffset = 0;
    U32 u32HwAesVABufAddr=0;
    U32 u32HwAesBufLen=0;
    U32 *temp_read_CustomerKeyBank_ADDR = NULL;

    UBOOT_TRACE("IN\n");
    raw_io_config_push();
    if(mboot_raw_io_Config() != 0)
    {
        UBOOT_ERROR("raw_io_config setting fail!\n");
        return -1;
    }

    temp_read_CustomerKeyBank_ADDR = (U32 *)malloc(sizeof(stCusKey)*sizeof(U32) + 64);
    if (temp_read_CustomerKeyBank_ADDR == NULL)
    {
        UBOOT_ERROR("malloc 0x%x bytes for HW_AES_BUF failed!\n", sizeof(stCusKey)*sizeof(U32) + 64);
        return -1;
    }

    u32HwAesVABufAddr = ((U32)temp_read_CustomerKeyBank_ADDR + 64) & (0xFFFFFFC0);
    u32HwAesBufLen    = sizeof(stCusKey);
    if(u32HwAesVABufAddr == 0xFFFFFFFF)
    {
        UBOOT_ERROR("PA2VA(u32HwAesPABufAddr) fail, u32HwAesVABufAddr=%x \n",u32HwAesVABufAddr);
        free(temp_read_CustomerKeyBank_ADDR);
        temp_read_CustomerKeyBank_ADDR = NULL;
        u32HwAesVABufAddr = 0;
        return -1;
    }
    UBOOT_DEBUG("u32HwAesVABufAddr=%x \n", u32HwAesVABufAddr);
    UBOOT_DEBUG("u32HwAesBufLen=%x \n", u32HwAesBufLen);

#if defined (CONFIG_R2TEE_TEELOADER)
    EN_CHUNK_HEADER_ITEM eChunkHeaderOffset = CH_CUSTOMER_KEY_BANK_RAM_OFFSET;
#else
    EN_CHUNK_HEADER_ITEM eChunkHeaderOffset = CH_CUSTOMER_KEY_BANK_ROM_OFFSET;
#endif

    if(MsApiChunkHeader_GetValue(eChunkHeaderOffset,&u32CusKeyOffset)== -1)
    {
        UBOOT_ERROR("Secure_Drv_Init fail\n");
        free(temp_read_CustomerKeyBank_ADDR);
        temp_read_CustomerKeyBank_ADDR = NULL;
        u32HwAesVABufAddr = 0;
        return -1;
    }

#if defined (CONFIG_R2TEE_TEELOADER)
    memset(&stCusKey, 0, sizeof(CUSTOMER_KEY_BANK));
    memcpy(&stCusKey, u32CusKeyOffset, sizeof(CUSTOMER_KEY_BANK));
    UBOOT_DEBUG("DUMP CUSTOMER KEY BANK :\n");
    UBOOT_DUMP(&stCusKey, sizeof(stCusKey));
#else
#if !defined (CONFIG_SECURE_BY_PASS_VERIFICATION)
    if(RestoreCustomerKeyBank(u32CusKeyOffset,sizeof(stCusKey),u32HwAesVABufAddr)==-1)
    {
        UBOOT_ERROR("restore key bank fail\n");
        free(temp_read_CustomerKeyBank_ADDR);
        temp_read_CustomerKeyBank_ADDR = NULL;
        u32HwAesVABufAddr = 0;
        return -1;
    }
#endif

    ///Warning: "_LoadCustomerKeyBank" must behind of "SignatureLoad".
    ///why??
    ///Becase we have a simple secure booting solution on MIPS booting mode.
    ///In this case, we store both the sensitive keys and signatures in "SECURITY_INFO" in SPI.
    ///So, we have to do "SignatureLoad" first, and then copy the sensitive keys from stSecureInfo.
    if(_LoadCustomerKeyBank(u32CusKeyOffset,sizeof(stCusKey),u32HwAesVABufAddr) == -1)
    {
       UBOOT_ERROR("Load rsa public key fail\n");
       free(temp_read_CustomerKeyBank_ADDR);
       temp_read_CustomerKeyBank_ADDR = NULL;
       u32HwAesVABufAddr = 0;
       return -1;
    }
#endif

    PopUpProtectedKey();
    raw_io_config_pop();

    free(temp_read_CustomerKeyBank_ADDR);
    temp_read_CustomerKeyBank_ADDR = NULL;
    u32HwAesVABufAddr = 0;
    UBOOT_TRACE("OK\n");
    return 0;
}


int do_Secure_Init(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    int ret1=0,ret2=0;
    UBOOT_TRACE("IN\n");

    secure_init_ok = TRUE;
    #if (defined(CONFIG_MSTAR_NS_UBOOT) || defined(CONFIG_OPTEE_TEELOADER) || defined(CONFIG_SECURE_SMC_HW_IP))
    if(gOpteeFailFlag != 0)
    {
        secure_init_ok = FALSE;
        UBOOT_ERROR("Optee OS is not running, Skip do_Secure_Init\n");
        return -1;
    }
    #endif

    if(Secure_Drv_Init()==-1)
    {
        UBOOT_ERROR("Secure_drv_Init fail\n");
        #if defined (CONFIG_DUAL_SYSTEM)
        run_command("mboot_verify_check", 0);
        #endif
        secure_init_ok = FALSE;
        jump_to_console();
    }

#if !defined (CONFIG_ANDROID_AVB_ENABLE)
    CheckSecureMerge();
    if(GetSecureMergeType() == E_SECUREMERGE_NONE)
    {
        ret1=LoadSignature();
    }
#endif

    ret2=LoadCustomerKeyBank();
    if (ret2 != 0)
    {
        secure_init_ok = FALSE;
    }

    if((ret1!=0)||(ret2!=0))
    {
        #if defined (CONFIG_DUAL_SYSTEM)
        UBOOT_ERROR("do_Secure_Init fail\n");
        run_command("mboot_verify_check", 0);
        #endif
        jump_to_console();
    }

#if defined(CONFIG_SECURE_SPI_PROTECT)
    spi_write_protect_process_init();
#endif

   UBOOT_TRACE("OK\n");
   return 0;
}

//WARNING: This function shoule be revomed from MstarApp.This kind of behavior must be in MstarCore
#if defined(__ARM__)
#define TSPCLK_MAP  0x1F2016A8
#else
#define TSPCLK_MAP  0xBF2016A8
#endif
#define TSP      ((unsigned char volatile *) TSPCLK_MAP)
void EnableAESDAMClock(void)
{
    TSP[0]=0x0;
}

int do_DumpSignature(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    UBOOT_TRACE("IN\n");

    if(argc!=2)
    {
        cmd_usage(cmdtp);
        return -1;
    }
    UBOOT_INFO("data.%s\n",argv[1]);
    _SUB_SECURE_INFO * pSubAddr = getSubSecureInfo(argv[1],FALSE);
    if( NULL == pSubAddr)
    {
        UBOOT_ERROR("get _SUB_SECURE_INFO error\n");
        return -1;
    }
    if(0 == IsFileSystemPartition(argv[1]))
    {
        _dump((unsigned int)pSubAddr,sizeof(_SUB_SECURE_INFO)*FRAGMENT_NUM);
    }
    else
    {
        _dump((unsigned int)pSubAddr,sizeof(_SUB_SECURE_INFO));
    }
    if(TRUE == IsInterleaveMode())
    {
        UBOOT_INFO("data_interleave.%s\n",argv[1]);
        pSubAddr = getSubSecureInfo(argv[1],TRUE);
        if( NULL == pSubAddr)
        {
            UBOOT_ERROR("get _SUB_SECURE_INFO error\n");
            return -1;
        }
        if(0 == IsFileSystemPartition(argv[1]))
        {
            _dump((unsigned int)pSubAddr,sizeof(_SUB_SECURE_INFO)*FRAGMENT_NUM);
        }
        else
        {
            _dump((unsigned int)pSubAddr,sizeof(_SUB_SECURE_INFO));
        }
    }
    UBOOT_TRACE("OK\n");
    return 0;

}

int CheckSignature(SECURITY_INFO *sign)
{
    unsigned int i;

    UBOOT_TRACE("IN\n");
    if(sign==NULL)
    {
        UBOOT_ERROR("The input parameter 'sign' is a null pointer\n");
        return -1;
    }
    char u8SecIdentify[8] = "\0";
    getSecIdentify(u8SecIdentify,sign,FALSE);
    if(0 != memcmp((const void *)u8SecIdentify,(const void *)idForContinueMode, sizeof(idForContinueMode)))
    {
        UBOOT_ERROR("\033[0;31m********************************\033[0m\n");
        UBOOT_ERROR("\033[0;31m !!SecureInfo ID compare fail   \033[0m\n");
        UBOOT_ERROR("\033[0;31m Please do sw upgrade           \033[0m\n");
        UBOOT_ERROR("\033[0;31m********************************\033[0m\n");
        return -1;
    }

    if(IsHouseKeepingBootingMode()==FALSE)
    {
        for(i=0;i<RSA_PUBLIC_KEY_E_LEN;i++)
        {
            if(sign->data.Key.u8RSAKernelPublicKey[RSA_PUBLIC_KEY_N_LEN+i]!=0xFF)
                break;
        }

        if(i!=RSA_PUBLIC_KEY_E_LEN){
            UBOOT_ERROR("\x1b[37;41m ===== [%s:%s:%d] Wrong Data in Secure Info !!! ===== \x1b[0m\n",__FILE__,__FUNCTION__,__LINE__);
            return -1;
        }
    }
    UBOOT_TRACE("OK\n");
    return 0;
}

int RawData_Authentication(char *KLName,unsigned int u32kernelAddr)
{
    UBOOT_TRACE("IN\n");
    if(-1 == _RawData_Authentication(KLName,u32kernelAddr))
    {
        #if defined (CONFIG_DUAL_SYSTEM)
        UBOOT_ERROR("_RawData_Authentication fail\n");
        run_command("mboot_verify_check", 0);
        #endif
        Secure_HaltCPU();
    }
    UBOOT_TRACE("OK\n");
    return 0;
}
int _RawData_Authentication(char* KLName,unsigned int u32kernelAddr)
{
    U32 kernelSize=0;
    U8 u8ImagePublicKeyN[RSA_PUBLIC_KEY_N_LEN];
    U8 u8ImagePublicKeyE[RSA_PUBLIC_KEY_E_LEN];
    U8 u8ImageSignature[SIGNATURE_LEN];
    int ret=0;
    UBOOT_TRACE("IN\n");
    ret = IsRawDataPartition(KLName);
    if( 0 != ret)
    {
        UBOOT_ERROR("Invalid KLName :%s\n",KLName);
        return -1;
    }
    ret = GetImageSize(KLName,0,&kernelSize);
    if (0 != ret)
    {
        UBOOT_ERROR("get %s image size fail\n",KLName);
        return -1;
    }
    #if defined(CONFIG_SECURE_ENCRYPT_RAWDATA)
    if(strcmp(KLName,"RFS")!=0 && strcmp(KLName,"cacert")!=0)
    {
        ret = DecryptKernelImage((char *)u32kernelAddr,kernelSize);
        if(-1 == ret)
        {
            UBOOT_ERROR("\033[0;31m Dercypt %s FAIL\033[0m\n",KLName);
            return -1;
        }
    }
    #endif//#if defined(CONFIG_SECURE_ENCRYPT_RAWDATA)
    flush_cache(u32kernelAddr,(U32)kernelSize);
    GetPublicKeyN(E_RSA_IMAGE_PUBLIC_KEY, u8ImagePublicKeyN, sizeof(u8ImagePublicKeyN));
    GetPublicKeyE(E_RSA_IMAGE_PUBLIC_KEY, u8ImagePublicKeyE, sizeof(u8ImagePublicKeyE));
    GetSignature(KLName, 0, u8ImageSignature, sizeof(u8ImageSignature));
    if (-1 == Secure_doAuthentication(u8ImagePublicKeyN, u8ImagePublicKeyE, u8ImageSignature, (U8 *)u32kernelAddr, kernelSize))
    {
        UBOOT_ERROR("\033[0;31m Auth  %s FAIL\033[0m\n",KLName);
        UBOOT_ERROR("\033[0;31m RSA image public-N key=\033[0m\n");
        UBOOT_ERRDUMP((U32)u8ImagePublicKeyN, RSA_PUBLIC_KEY_N_LEN);

        UBOOT_ERROR("\033[0;31m RSA image public-E key=\033[0m\n");
        UBOOT_ERRDUMP((U32)u8ImagePublicKeyE, RSA_PUBLIC_KEY_E_LEN);

        UBOOT_ERROR("\033[0;31m %s signature=\033[0m\n",KLName);
        UBOOT_ERRDUMP((U32)u8ImageSignature, SIGNATURE_LEN);

        UBOOT_ERROR("\033[0;31m clear %s image=\033[0m\n",KLName);
        UBOOT_ERRDUMP((U32)u32kernelAddr, 0x100);
        return -1;
    }
    else
    {
        #if defined(CONFIG_SECURE_DEBUG_MESSAGE)
        UBOOT_INFO("\033[0;34m*******************************\033[0m\n");
        UBOOT_INFO("\033[0;34m* %s Authentication SUCCESS !! *\033[0m\n",KLName);
        UBOOT_INFO("\033[0;34m*******************************\033[0m\n");
        #endif
        UBOOT_TRACE("OK\n");
        return 0;
    }
}


int ImageIOInit(char *partition)
{
    int ret=0;
    UBOOT_TRACE("IN\n");
    if(partition==NULL)
    {
        UBOOT_ERROR("partition is a null pointer\n");
        return -1;
    }

    #if (ENABLE_MODULE_NAND_FLASH==1) || (ENABLE_MODULE_SPI_NAND_FLASH==1)
    ret=_ImageIOInitForNand(partition);
    #endif

    #if(ENABLE_MODULE_MMC==1)
    ret=_ImageIOInitForEMMC(partition);
    #endif

    UBOOT_TRACE("OK\n");
    return ret;
}

int _ImageIOInitForEMMC(char *partition)
{
    UBOOT_TRACE("IN\n");
    UBOOT_TRACE("OK\n");
    return 0;
}

int _ImageIOInitForNand(char *partition)
{
    char *buffer=NULL;
    int ret=0;

    UBOOT_TRACE("IN\n");

    if(partition==NULL)
    {
        UBOOT_ERROR("partition is a null pointer\n");
        return -1;
    }

    buffer=(char *)malloc(CMD_BUF);
    if(buffer==NULL)
    {
        UBOOT_ERROR("malloc fail\n");
        return -1;
    }
    memset(buffer,0,CMD_BUF);

    UBOOT_DEBUG("partition=%s\n",partition);

    snprintf(buffer,CMD_BUF,"ubi part %s",partition);
    UBOOT_DEBUG("cmd=%s\n",buffer);
    ret=run_command(buffer,0);
    if(ret==-1)
    {
        free(buffer);
        UBOOT_ERROR("ubi part %s fail\n",partition);
        return -1;
    }


    free(buffer);
    UBOOT_TRACE("OK\n");
    return 0;
}

int ImageRead(char *volume, unsigned int addr, unsigned int offset, unsigned len)
{
    int ret=0;
    UBOOT_TRACE("IN\n");
    ret=_ImageRead(volume, addr, offset, len);
    UBOOT_TRACE("OK\n");
    return 0;
}
int _ImageRead(char *volume, unsigned int addr, unsigned int offset, unsigned len)
{
    char *buffer=NULL;
    int ret=0;

    UBOOT_TRACE("IN\n");

    if(volume==NULL)
    {
        UBOOT_ERROR("volume is a null pointer\n");
        return -1;
    }
    buffer=(char *)malloc(CMD_BUF);
    if(buffer==NULL)
    {
        UBOOT_ERROR("malloc fail\n");
        return -1;
    }
    memset(buffer,0,CMD_BUF);

    UBOOT_DEBUG("volume=%s\n",volume);
    UBOOT_DEBUG("addr=0x%x\n",(unsigned int)addr);
    UBOOT_DEBUG("offset=0x%x\n",(unsigned int)offset);
    UBOOT_DEBUG("len=0x%x\n",(unsigned int)len);

    flush_cache(addr,(U32)len);
    memset(buffer,0,CMD_BUF);

    #if (ENABLE_MODULE_NAND_FLASH==1) || (ENABLE_MODULE_SPI_NAND_FLASH==1)
    snprintf(buffer,CMD_BUF,"ubi partial_read %x %s %x %x",addr ,volume ,len, offset);
    #endif
    #if(ENABLE_MODULE_MMC==1)
    snprintf(buffer,CMD_BUF,"mmc read.p.continue %x %s %x %x", addr, volume,offset>>9, len);
    #endif

    UBOOT_DEBUG("cmd=%s\n",buffer);
    ret=run_command(buffer,0);
    if(ret==-1)
    {
        free(buffer);
        UBOOT_ERROR("ubi read %s fail\n",volume);
        return -1;
    }
    flush_cache(addr,(U32)len);
    free(buffer);
    UBOOT_TRACE("OK\n");
    return 0;
}

int ImageWrite(char *volume, unsigned int addr, unsigned int offset, unsigned len)
{
    UBOOT_TRACE("IN\n");
    UBOOT_ERROR("This function is not implement\n");
    UBOOT_TRACE("OK\n");
    return -1;
}

BOOLEAN IsDoingPartialAuthentication(void)
{
    char *ptr=NULL;
    UBOOT_TRACE("IN\n");
    ptr=getenv(ENV_PARTAIL_AUTHENTICATION);
    if(ptr!=NULL){
       if(0==strcmp((const char *)ptr,"OFF"))
       {
         UBOOT_DEBUG("env '%s' is OFF\n",ENV_PARTAIL_AUTHENTICATION);
         UBOOT_TRACE("OK\n");
         return FALSE;
       }
    }
    else
    {
        UBOOT_DEBUG("No env '%s'\n",ENV_PARTAIL_AUTHENTICATION);
        UBOOT_TRACE("OK\n");
        return FALSE;
    }
    UBOOT_TRACE("OK\n");
    return TRUE;
}

int DisablePartialAuthentication(void)
{
    int ret=0;
    char *pEnv=0;
    UBOOT_TRACE("IN\n");
    pEnv=getenv(ENV_PARTAIL_AUTHENTICATION);
    if(pEnv!=NULL)
    {
        if(strstr(pEnv,"OFF")!=0)
        {
            UBOOT_DEBUG("This function is already disable\n");
            UBOOT_TRACE("OK\n");
            return 0;
        }
    }
    ret=setenv(ENV_PARTAIL_AUTHENTICATION, "OFF");
    if(ret==0)
    {
        ret=saveenv();
        if(ret==0)
        {
            UBOOT_TRACE("OK\n");
        }
        else
        {
           UBOOT_ERROR("DisablePartialAuthentication executes saveenv fail\n");
        }
    }
    else
    {
        UBOOT_ERROR("DisablePartialAuthentication executes setenv fail\n");
    }

    return ret;
}

int EnablePartialAuthentication(void)
{
    int ret=0;
    char *pEnv=NULL;
    UBOOT_TRACE("IN\n");

    pEnv=getenv(ENV_PARTAIL_AUTHENTICATION);
    if(pEnv!=NULL)
    {
        if(strstr(pEnv,"ON")!=0)
        {
            UBOOT_DEBUG("This function is already enable\n");
            UBOOT_TRACE("OK\n");
            return 0;
        }
    }
    ret=setenv(ENV_PARTAIL_AUTHENTICATION, "ON");
    if(ret==0)
    {
        ret=saveenv();
        if(ret==0)
        {
            UBOOT_TRACE("OK\n");
        }
        else
        {
           UBOOT_ERROR("EnablePartialAuthentication executes saveenv fail\n");
        }
    }
    else
    {
        UBOOT_ERROR("EnablePartialAuthentication executes setenv fail\n");
    }
    return ret;
}

int GetAESKey(KEY_INDEX enKey,U8 *keyBuf)
{
    UBOOT_TRACE("IN\n");
    if(keyBuf==NULL)
    {
        UBOOT_ERROR("The input parameter 'keyBuf' is a null pointer\n");
        return -1;
    }
    if(enKey == E_AES_BOOT_KEY)
    {
        int i;
        for(i=0;i<AES_KEY_LEN;i++)
        {
            U8 tempKey=0;
            #if defined(CONFIG_ENCRYPT_AESBOOT_KEY)
                tempKey=stCusKey.u8AESBootKey[i]^u8AESBootHashKey[i];
            #else
                tempKey=stCusKey.u8AESBootKey[i];
            #endif

            #if defined(CONFIG_SECURE_HW_IP)
                //if call AESDMA ,need to reverse AES key
                keyBuf[AES_KEY_LEN-1-i]=tempKey;
            #else
                keyBuf[i]=tempKey;
            #endif
        }
    }
    else if(enKey == E_AES_UPGRADE_KEY)
    {
        #if defined(CONFIG_SECURE_HW_IP)
        int i;
        for(i=0;i<AES_KEY_LEN;i++)
        {
            keyBuf[AES_KEY_LEN-1-i]=stCusKey.u8AESUpgradeKey[i];
        }
        #else
        memcpy(keyBuf,stCusKey.u8AESUpgradeKey,AES_KEY_LEN);
        #endif
    }
    else
    {
        UBOOT_ERROR("Wrong AES Key Index !!\n");
        return -1;
    }

    UBOOT_TRACE("OK\n");
    return 0;
}

int GetPublicKeyN(KEY_INDEX enKey, U8 *buffer, U32 len)
{
    UBOOT_TRACE("IN\n");
    if(buffer==NULL)
    {
        UBOOT_ERROR("The input parameter 'buffer' is a null pointer\n");
        return -1;
    }

    if(len<RSA_PUBLIC_KEY_N_LEN)
    {
        UBOOT_ERROR("The len is smaller than the lenght of public-N(%d bytes).\n",RSA_PUBLIC_KEY_N_LEN);
        return -1;
    }

    if(enKey == E_RSA_IMAGE_PUBLIC_KEY)
    {
        memcpy(buffer,stCusKey.u8RSAImagePublicKey,RSA_PUBLIC_KEY_N_LEN);
    }
    else if(enKey == E_RSA_UPGRADE_PUBLIC_KEY)
    {
        memcpy(buffer,stCusKey.u8RSAUpgradePublicKey,RSA_PUBLIC_KEY_N_LEN);
    }
    else if(enKey == E_RSA_BOOT_PUBLIC_KEY)
    {
        memcpy(buffer,stCusKey.u8RSABootPublicKey,RSA_PUBLIC_KEY_N_LEN);
    }
    else
    {
        UBOOT_ERROR("Wrong RSA Key Index !!\n");
        return -1;
    }

    UBOOT_TRACE("OK\n");
    return 0;
}

int GetPublicKeyE(KEY_INDEX enKey, U8 *buffer, U32 len)
{
    UBOOT_TRACE("IN\n");

    if(buffer==NULL)
    {
        UBOOT_ERROR("The input parameter 'buffer' is a null pointer\n");
        return -1;
    }

    if(len<RSA_PUBLIC_KEY_E_LEN)
    {
        UBOOT_ERROR("The len is smaller than the lenght of public-E(%d bytes).\n",RSA_PUBLIC_KEY_E_LEN);
        return -1;
    }

    if(enKey == E_RSA_IMAGE_PUBLIC_KEY)
    {
        memcpy(buffer,&stCusKey.u8RSAImagePublicKey[RSA_PUBLIC_KEY_N_LEN],RSA_PUBLIC_KEY_E_LEN);
    }
    else if(enKey == E_RSA_UPGRADE_PUBLIC_KEY)
    {
        memcpy(buffer,&stCusKey.u8RSAUpgradePublicKey[RSA_PUBLIC_KEY_N_LEN],RSA_PUBLIC_KEY_E_LEN);
    }
    else if(enKey == E_RSA_BOOT_PUBLIC_KEY)
    {
        memcpy(buffer,&stCusKey.u8RSABootPublicKey[RSA_PUBLIC_KEY_N_LEN],RSA_PUBLIC_KEY_E_LEN);

    }
    else
    {
        UBOOT_ERROR("Wrong Key Index !!\n");
        return -1;
    }

    UBOOT_TRACE("OK\n");
    return 0;
}

int GetSignature(char *volume, U8 num, U8 *buffer, U32 len)
{
    UBOOT_TRACE("IN\n");
    if(volume==NULL)
    {
        UBOOT_ERROR("The input parameter 'volume' is a null pointer\n");
        return -1;
    }

    if(buffer==NULL)
    {
        UBOOT_ERROR("The input parameter 'buffer' is a null pointer\n");
        return -1;
    }

    if(len<SIGNATURE_LEN)
    {
        UBOOT_ERROR("The len is smaller than the lenght of signature(%d bytes).\n",SIGNATURE_LEN);
        return -1;
    }
    UBOOT_DEBUG("get %s[%d] signature\n",volume,num);
    _SUB_SECURE_INFO * pSubAddr = getSubSecureInfo(volume,IsInterleaveMode());
    if( NULL == pSubAddr)
    {
        UBOOT_ERROR("get _SUB_SECURE_INFO error\n");
        return -1;
    }
    if(0 == IsFileSystemPartition(volume))
    {
        pSubAddr += num;
    }
    memcpy(buffer,pSubAddr->u8Signature,SIGNATURE_LEN);
    UBOOT_TRACE("OK\n");
    return 0;
}

int GetImageSize(char *volume, U8 num, U32 *size)
{

    UBOOT_TRACE("IN\n");
    if(volume==NULL)
    {
        UBOOT_ERROR("The input parameter 'volume' is a null pointer\n");
        return -1;
    }

    if(size==NULL)
    {
        UBOOT_ERROR("The input parameter 'size' is a null pointer\n");
        return -1;
    }
    UBOOT_DEBUG("get %s[%d] size\n",volume,num);
    _SUB_SECURE_INFO * pSubAddr = getSubSecureInfo(volume,IsInterleaveMode());
    if( NULL == pSubAddr)
    {
        UBOOT_ERROR("get _SUB_SECURE_INFO error\n");
        return -1;
    }
    if(0 == IsFileSystemPartition(volume))
    {
        pSubAddr += num;
    }
    *size = pSubAddr->info.u32Size;
    UBOOT_DEBUG("size=0x%x\n",*size);
    if(*size==0)
    {
        UBOOT_ERROR("wrong image size\n");
        return -1;
    }
    UBOOT_TRACE("OK\n");
    return 0;
}

int GetSubImageNum(char *volume, U32 *num)
{
    UBOOT_TRACE("IN\n");
    if(volume==NULL)
    {
        UBOOT_ERROR("The input parameter 'volume' is a null pointer\n");
        return -1;
    }

    if(num==NULL)
    {
        UBOOT_ERROR("The input parameter 'num' is a null pointer\n");
        return -1;
    }

    UBOOT_DEBUG("get the sub image number of %s\n",volume);
    _SUB_SECURE_INFO * pSubAddr = getSubSecureInfo(volume,IsInterleaveMode());
    if( NULL == pSubAddr)
    {
        UBOOT_ERROR("get _SUB_SECURE_INFO error\n");
        return -1;
    }
    *num = pSubAddr->info.u32Num;
    UBOOT_DEBUG("num=0x%x\n",*num);
    if(*num > FRAGMENT_NUM)
    {
        UBOOT_ERROR("wrong sub image num,larger than FRAGMENT_NUM(%d)\n",FRAGMENT_NUM);
        return -1;
    }
    UBOOT_TRACE("OK\n");
    return 0;
}

int DecryptKernelImage(char *dataBuf, unsigned int len)
{
    int ret=-1;

    UBOOT_TRACE("IN\n");

    if(IsHouseKeepingBootingMode()==FALSE)
    {
        ret=DecryptKernelImageBySecureProcessor(dataBuf, len);
    }
    else
    {
        ret=DecryptKernelImageByHouseKeeping(dataBuf, len);
    }

    if(ret==-1)
    {
        UBOOT_ERROR("Decrypte kernel image fail\n");
    }
    else
    {
        UBOOT_TRACE("OK\n");
    }

    return ret;
}

int DecryptKernelImageBySecureProcessor(char *dataBuf, unsigned int len)
{
    SECUREBOOT_MbxRetStruct SecureBootMbxRetStruct;
    SECUREBOOT_Result Result;
    int ret=-1;
    UBOOT_TRACE("IN\n");

    flush_cache((U32)dataBuf,(U32)len);
    Chip_Flush_Memory();

    Result=Secure_MailBox_ASSIGN_DECRYPTED_KEY(&SecureBootMbxRetStruct,E_AES_BOOT_KEY,AES_KEY_LEN,E_KEY_SWAP);
    if(Result==E_SECUREBOOT_OK)
    {
        Result=Secure_MailBox_AESDecode(&SecureBootMbxRetStruct, VA2PA((U32)dataBuf), len, SECURE_ENG_AES_MODE_ECB);
        if(Result==E_SECUREBOOT_OK)
        {
            ret=0;
        }
        else
        {
            UBOOT_DEBUG("aes decrypte fail\n");
            ret=-1;
        }
    }
    else
    {
        UBOOT_DEBUG("Assign aes decrypted key fail\n");
        ret=-1;
    }


    if(ret==-1)
    {
        UBOOT_ERROR("Decrypte kernel image by secure processor fail\n");
    }
    else
    {
        UBOOT_TRACE("OK\n");
    }

    return ret;
}

int DecryptKernelImageByHouseKeeping(char *dataBuf, unsigned int len)
{
    int ret=-1;
    UBOOT_TRACE("IN\n");
    flush_cache((U32)dataBuf,(U32)len);
    Chip_Flush_Memory();
    U8 u8Key[AES_KEY_LEN]="\0";
    GetAESKey(E_AES_BOOT_KEY,u8Key);
    UBOOT_DUMP((unsigned int)u8Key,AES_KEY_LEN);
    ret=Secure_AES_ECB_Decrypt((U32)(dataBuf), len, (U8 *)u8Key);
    if(-1 == ret)
    {
        UBOOT_ERROR("Decrypte kernel image by hostkeeping fail\n");
    }
    else
    {
        UBOOT_TRACE("OK\n");
    }

    return ret;
}


int _LoadCustomerKeyBank(U32 u32CusKeyOffset,U32 u32CusKeySize,U32 u32HwAesVABufAddr)
{
    UBOOT_TRACE("IN\n");
    if(IsHouseKeepingBootingMode()==FALSE)
    {
        //In this case, secure processor holds all clean customer's key. And it only allow us to get the rsa public key.
        SECUREBOOT_MbxRetStruct SecureBootMbxRetStruct;
        UBOOT_DEBUG("flush memory\n");
        flush_cache((U32)u32HwAesVABufAddr,RSA_PUBLIC_KEY_LEN); //Chip_Flush_Memory();
        // get RSA_IMAGE_PUBLIC_KEY
        if(E_SECUREBOOT_OK!=Secure_MailBox_GetKey(&SecureBootMbxRetStruct,(U32)u32HwAesVABufAddr,E_RSA_IMAGE_PUBLIC_KEY,RSA_PUBLIC_KEY_LEN))
        {
            return -1;
        }
        memcpy((void *)stCusKey.u8RSAImagePublicKey,(void *)u32HwAesVABufAddr,RSA_PUBLIC_KEY_LEN);

        // get RSA_UPGRADE_PUBLIC_KEY
        flush_cache((U32)u32HwAesVABufAddr,RSA_PUBLIC_KEY_LEN); //Chip_Flush_Memory();
        if(E_SECUREBOOT_OK!=Secure_MailBox_GetKey(&SecureBootMbxRetStruct,(U32)u32HwAesVABufAddr,E_RSA_UPGRADE_PUBLIC_KEY,RSA_PUBLIC_KEY_LEN))
        {
            return -1;
        }
        memcpy((void *)stCusKey.u8RSAUpgradePublicKey,(void *)u32HwAesVABufAddr,RSA_PUBLIC_KEY_LEN);
    }
    else
    {
        #if defined (CONFIG_DOUBLE_MBOOT) && !defined (CONFIG_DUAL_SYSTEM)
        mbootbak_raw_io_Config();
        #endif
        if(raw_read(u32HwAesVABufAddr,u32CusKeyOffset,u32CusKeySize)!= 0)
        {
            UBOOT_ERROR("Read CustomerKeyBank Fail\n");
        }
        #if !defined (CONFIG_SECURE_BY_PASS_VERIFICATION)
        _DecryptCustomerKeyBank(u32HwAesVABufAddr,u32CusKeySize);
        #endif
        memcpy(&stCusKey,(void*)u32HwAesVABufAddr,u32CusKeySize);
        #if defined(CONFIG_MSTAR_AUTHEN_ENABLE) && !defined(CONFIG_SECURE_BY_PASS_VERIFICATION)
        isCustomerKeyBankValid((CUSTOMER_KEY_BANK *)u32HwAesVABufAddr);
        #endif
    }

    if(memcmp(stCusKey.u8RSAImagePublicKey,u8MstarDefRSAImagePublicKey,RSA_PUBLIC_KEY_LEN)==0)
    {
        UBOOT_DEBUG("\033[0;36m Mstar's default keys inside\033[0m\n");
    }
    else
    {
        UBOOT_DEBUG("\033[0;36m Customer's keys inside\033[0m\n");
    }
    UBOOT_TRACE("OK\n");
    return 0;
}

#if defined(CONFIG_BOOTING_FROM_EXT_SPI_WITH_PM51)
void _DoSecureEngUnitTestCase1(void)
{
   /*
      *  Test encoding and decodeing basically.
      */
    SECUREBOOT_MbxRetStruct SecureBootMbxRetStruct;

    U8 u8Plaintext[] = {0x4E,0x43,0x43,0x50,0x32,0x2F,0x63,0x54,0x69,0x63,0x6B,0x65,0x74,0x45,0x78,0x70,
                        0x69,0x72,0x61,0x74,0x69,0x6F,0x6E,0x0A,0x31,0x32,0x38,0x31,0x39,0x35,0x35,0x34};



    const U8 u8PlaintextBackup[] = {0x4E,0x43,0x43,0x50,0x32,0x2F,0x63,0x54,0x69,0x63,0x6B,0x65,0x74,0x45,0x78,0x70,
                                   0x69,0x72,0x61,0x74,0x69,0x6F,0x6E,0x0A,0x31,0x32,0x38,0x31,0x39,0x35,0x35,0x34};

    UBOOT_TRACE("IN\n");
    if(IsHouseKeepingBootingMode()==TRUE)
    {
        UBOOT_INFO("This function only supports in secure processer booting mode\n");
        return;
    }
    Secure_Drv_Init();
    Secure_MailBox_ASSIGN_DECRYPTED_KEY(&SecureBootMbxRetStruct,E_AES_EFUSE_KEY,AES_KEY_LEN,E_KEY_SWAP);


    memcpy((void *)SECURITY_COMMON_BUFFER_ADDR,u8Plaintext,sizeof(u8Plaintext));
    flush_cache((U32)SECURITY_COMMON_BUFFER_ADDR,sizeof(u8Plaintext));
    Chip_Flush_Memory();
    Secure_MailBox_AESEncode(&SecureBootMbxRetStruct,VA2PA((U32)SECURITY_COMMON_BUFFER_ADDR),sizeof(u8Plaintext),SECURE_ENG_AES_MODE_ECB);
    UBOOT_DUMP(SECURITY_COMMON_BUFFER_ADDR,sizeof(u8Plaintext));


    flush_cache((U32)SECURITY_COMMON_BUFFER_ADDR,sizeof(u8Plaintext));
    Chip_Flush_Memory();
    Secure_MailBox_AESDecode(&SecureBootMbxRetStruct,VA2PA((U32)SECURITY_COMMON_BUFFER_ADDR),sizeof(u8Plaintext),SECURE_ENG_AES_MODE_ECB);
    UBOOT_DUMP(SECURITY_COMMON_BUFFER_ADDR,sizeof(u8Plaintext));

    if(0!=memcmp((void *)SECURITY_COMMON_BUFFER_ADDR,(void *)u8PlaintextBackup,sizeof(u8PlaintextBackup))){
        UBOOT_INFO("\033[0;31m %s fail \033[0m\n",__FUNCTION__);

    }
    else{
        UBOOT_INFO("\033[0;31m %s pass \033[0m\n",__FUNCTION__);
    }
    UBOOT_TRACE("OK\n");

}

void _DoSecureEngUnitTestCase2(void)
{
    /*
       *  We use a cipher key to decrypt u8ENCRYPTED. So we have to command PM51 to decrypte u8cipherKey first.
       *  PS: This cipher key has 32 bytes, but only first 16 bytes is for aes decryption. (Here, key offset is o)
       */

    SECUREBOOT_MbxRetStruct SecureBootMbxRetStruct;

    //u8ENCRYPTED comes from using u8cleanKey to encrypte u8DECRYPTED.
    U8 u8ENCRYPTED[] = {0x76,0x1F,0x47,0x1B,0x3B,0x93,0x21,0x46,0x90,0xB5,0xD5,0x6F,0x2C,0x67,0x77,0xFE,
                        0x79,0x05,0x06,0xB5,0xAE,0xF8,0xAB,0xDF,0x29,0x27,0xE0,0x43,0x2E,0x1E,0x00,0x37};

    const U8 u8DECRYPTED[] = {0x4E,0x43,0x43,0x50,0x32,0x2F,0x63,0x54,0x69,0x63,0x6B,0x65,0x74,0x45,0x78,0x70,
                                       0x36,0x2B,0x2D,0xDF,0x0C,0xDF,0xED,0x4D,0x96,0xBF,0xA7,0x0C,0x00,0x2E,0x7E,0x9C};

    //U8 u8cleanKey[]={0x02,0x44,0xB7,0x16,0x7F,0x65,0x98,0x04,0xC5,0x75,0xC4,0x9A,0x81,0xDF,0xE0,0x00};
    // u8ChiperKey comes from using efse key to encrypte u8cleanKey.(Here, efuse key are all zero)
    U8 u8cipherKey[]={0xDE,0x5D,0x16,0x31,0x48,0xA8,0x12,0x47,0xCB,0x64,0x7A,0x18,0x60,0x24,0x8D,0xAC, //Key
                      0xDE,0x5D,0x16,0x31,0x48,0xA8,0x12,0x47,0xCB,0x64,0x7A,0x18,0x60,0x24,0x8D,0xAC}; //Data
    UBOOT_TRACE("IN\n");
    if(IsHouseKeepingBootingMode()==TRUE)
    {
        UBOOT_INFO("This function only supports in secure processer booting mode\n");
        return;
    }
    Secure_Drv_Init();
    Secure_MailBox_ASSIGN_DECRYPTED_KEY(&SecureBootMbxRetStruct,E_AES_EFUSE_KEY,AES_KEY_LEN,E_KEY_SWAP);
    memcpy((void *)SECURITY_COMMON_BUFFER_ADDR,u8cipherKey,sizeof(u8cipherKey));
    flush_cache((U32)SECURITY_COMMON_BUFFER_ADDR,sizeof(u8cipherKey));
    Chip_Flush_Memory();
    Secure_MailBox_SET_KEY_WITH_DECRYPTION(&SecureBootMbxRetStruct,VA2PA((U32)SECURITY_COMMON_BUFFER_ADDR),sizeof(u8cipherKey),0,SECURE_ENG_AES_MODE_ECB);


    memcpy((void *)SECURITY_COMMON_BUFFER_ADDR,u8ENCRYPTED,sizeof(u8ENCRYPTED));
    flush_cache((U32)SECURITY_COMMON_BUFFER_ADDR,sizeof(u8ENCRYPTED));
    Chip_Flush_Memory();
    Secure_MailBox_AESDecode(&SecureBootMbxRetStruct,VA2PA((U32)SECURITY_COMMON_BUFFER_ADDR),sizeof(u8ENCRYPTED),SECURE_ENG_AES_MODE_ECB);
    UBOOT_DUMP(SECURITY_COMMON_BUFFER_ADDR,sizeof(u8ENCRYPTED));

    if(0!=memcmp((void *)SECURITY_COMMON_BUFFER_ADDR,(void *)u8DECRYPTED,sizeof(u8DECRYPTED)))
    {
        UBOOT_INFO("\033[0;31m %s fail \033[0m\n",__FUNCTION__);

    }
    else{
        UBOOT_INFO("\033[0;31m %s pass \033[0m\n",__FUNCTION__);
    }
    UBOOT_TRACE("OK\n");
}


void _DoSecureEngUnitTestCase3(void)
{
    /*
       *  This cipher key has 48 bytes, but only  16 bytes is for aes decryption. (Here, key offset is 21)
       *   The cipher keys encrypte in ECB mode
       */

    SECUREBOOT_MbxRetStruct SecureBootMbxRetStruct;

    // u8ChiperKey comes from using efse key to encrypte u8cleanKey.(Here, efuse key are all zero)
    U8 u8cipherKey[]={0xDE,0x5D,0x16,0x31,0x48,0xA8,0x12,0x47,0xCB,0x64,0x7A,0x18,0x60,0x24,0x8D,0xAC,
                      0xDE,0x5D,0x16,0x31,0x48,//Data
                      0xA8,0x12,0x47,0xCB,0x64,0x7A,0x18,0x60,0x24,0x8D,0xAC,0xDE,0x5D,0x16,0x31,0x48, //Key
                      0xA8,0x12,0x47,0xCB,0x64,0x7A,0x18,0x60,0x24,0x8D,0xAC}; //Data

    const U8 u8plaintKey[]={0x02,0x44,0xB7,0x16,0x7F,0x65,0x98,0x04,0xC5,0x75,0xC4,0x9A,0x81,0xDF,0xE0,0x00,
                      0x02,0x44,0xB7,0x16,0x7F, // Data
                      0xA8,0x12,0x47,0xCB,0x64,0x7A,0x18,0x60,0x24,0x8D,0xAC,0xDE,0x5D,0x16,0x31,0x48, //Key
                      0x65,0x98,0x04,0xC5,0x75,0xC4,0x9A,0x81,0xDF,0xE0,0x00}; //Data

    UBOOT_TRACE("IN\n");
    if(IsHouseKeepingBootingMode()==TRUE)
    {
        UBOOT_INFO("This function only supports in secure processer booting mode\n");
        return;
    }
    Secure_Drv_Init();
    Secure_MailBox_ASSIGN_DECRYPTED_KEY(&SecureBootMbxRetStruct,E_AES_EFUSE_KEY,AES_KEY_LEN,E_KEY_SWAP);
    memcpy((void *)SECURITY_COMMON_BUFFER_ADDR,u8cipherKey,sizeof(u8cipherKey));
    flush_cache((U32)SECURITY_COMMON_BUFFER_ADDR,sizeof(u8cipherKey));
    Chip_Flush_Memory();
    Secure_MailBox_SET_KEY_WITH_DECRYPTION(&SecureBootMbxRetStruct,VA2PA((U32)SECURITY_COMMON_BUFFER_ADDR),sizeof(u8cipherKey),21,SECURE_ENG_AES_MODE_ECB);
    UBOOT_DUMP(SECURITY_COMMON_BUFFER_ADDR,sizeof(u8cipherKey));

    if(0!=memcmp((void *)(0xA0000000|(SECURITY_COMMON_BUFFER_ADDR)),(void *)u8plaintKey,sizeof(u8plaintKey)))
    {
        UBOOT_INFO("\033[0;31m %s fail \033[0m\n",__FUNCTION__);

    }
    else{
        UBOOT_INFO("\033[0;31m %s pass \033[0m\n",__FUNCTION__);
    }
    UBOOT_TRACE("OK\n");
}


void _DoSecureEngUnitTestCase4(void)
{
    /*
       *  This u82thChiphertext has 36 bytes, but only  16 bytes is for aes decryption. (Here, key offset is 5)
       *   The cipher keys encrypte in CBC mode
       */

    SECUREBOOT_MbxRetStruct SecureBootMbxRetStruct;

    //--> u82thCiphertext is encrypted by u81thPlaintKey in CBC mode
    U8 u82thChiphertext[]={0x2c,0x82,0x9f,0xbe, //Data
                           0xa9,0x83,0x0d,0x64,0x31,0xd2,0x1d,0x50,0x5f,0x58,0x38,0x79,0xcb,0xa0,0x35,0x49, //Key
                           0x64,0x5c,0x20,0x9c,0xfc,0x7e,0xa0,0xd8,0xb7,0x25,0xec,0xfe}; //Data

    // --> u82thPlaintext is real plaintext
    //U8 u82thPlaintext[]={0x62,0x27,0xdc,0xd1, //Data
    //                             0x5e,0x59,0x79,0x97,0x4e,0xd7,0x2a,0x8a,0x24,0x0a,0x05,0xb0, 0x97,0x96,0xa8,0x7e, //Key
    //                             0x4f,0x0b,0x33,0xb5,0x6a,0x89,0xac,0x33,0x7f,0xab,0xe8,0x9a}; //Data
    // --> u82thOutputtext is output data, becuase the part of key will be stored in RIU only.
    const U8 u82thOutputtext[]={0x62,0x27,0xdc,0xd1, //Data
                          0xa9,0x83,0x0d,0x64,0x31,0xd2,0x1d,0x50,0x5f,0x58,0x38,0x79,0xcb,0xa0,0x35,0x49, //Key
                          0x4f,0x0b,0x33,0xb5,0x6a,0x89,0xac,0x33,0x7f,0xab,0xe8,0x9a}; //Data

    // --> u81thCiphertKey comes from using efse key to encrypte u81thPlaintKey.(Here, efuse key are all zero)
    //U8 u81thPlaintKey[]={0x23,0xb2,0xdb,0xea,0xb4,0x0b,0x8c,0xc0,0xd1,0xca,0xaa,0xe9,0xe8,0x65,0xca,0x3c};
    U8 u81thCiphertKey[]={0x56,0xa5,0xc2,0x1b,0x30,0xd4,0x3c,0xc6,0x10,0x1f,0x93,0xe3,0xb5,0x78,0xc9,0xa9};

    UBOOT_TRACE("IN\n");
    if(IsHouseKeepingBootingMode()==TRUE)
    {
        UBOOT_INFO("This function only supports in secure processer booting mode\n");
        return;
    }
    Secure_Drv_Init();
    Secure_MailBox_ASSIGN_DECRYPTED_KEY(&SecureBootMbxRetStruct,E_AES_EFUSE_KEY,AES_KEY_LEN,E_KEY_SWAP);

    memcpy((void *)SECURITY_COMMON_BUFFER_ADDR,u81thCiphertKey,sizeof(u81thCiphertKey));
    flush_cache((U32)SECURITY_COMMON_BUFFER_ADDR,sizeof(u81thCiphertKey));
    Chip_Flush_Memory();
    Secure_MailBox_SET_KEY_WITH_DECRYPTION(&SecureBootMbxRetStruct,VA2PA((U32)SECURITY_COMMON_BUFFER_ADDR),sizeof(u81thCiphertKey),0,SECURE_ENG_AES_MODE_ECB);
    UBOOT_DUMP(SECURITY_COMMON_BUFFER_ADDR,sizeof(u81thCiphertKey));

    memcpy((void *)SECURITY_COMMON_BUFFER_ADDR,u82thChiphertext,sizeof(u82thChiphertext));

    flush_cache((U32)SECURITY_COMMON_BUFFER_ADDR,sizeof(u82thChiphertext));
    Chip_Flush_Memory();
    Secure_MailBox_SET_KEY_WITH_DECRYPTION(&SecureBootMbxRetStruct,VA2PA((U32)SECURITY_COMMON_BUFFER_ADDR),sizeof(u82thChiphertext),4,SECURE_ENG_AES_MODE_CBC);

    UBOOT_DUMP(SECURITY_COMMON_BUFFER_ADDR,sizeof(u82thChiphertext));

    if(0!=memcmp((void *)SECURITY_COMMON_BUFFER_ADDR,(void *)u82thOutputtext,sizeof(u82thOutputtext)))
    {
        UBOOT_INFO("\033[0;31m %s fail \033[0m\n",__FUNCTION__);

    }
    else{
        UBOOT_INFO("\033[0;31m %s pass \033[0m\n",__FUNCTION__);
    }
    UBOOT_TRACE("OK\n");


}

void _DoSecureEngUnitTestCase5(void)
{
    SECUREBOOT_MbxRetStruct SecureBootMbxRetStruct;
    U8 DID[DID_LEN];
    U8 i=0;
    UBOOT_TRACE("IN\n");
    if(IsHouseKeepingBootingMode()==TRUE)
    {
        UBOOT_INFO("This function only supports in secure processer booting mode\n");
        return;
    }

    memset((void *)DID,0,DID_LEN);
    flush_cache((U32)DID,DID_LEN);
    Secure_MailBox_GETDID(&SecureBootMbxRetStruct,(U32)DID,DID_LEN);
    UBOOT_INFO("DID[]={");
    for(i=0;i<DID_LEN;i++)
    {
        UBOOT_INFO("0x%02x ",DID[i]);
    }
    UBOOT_INFO("}\n");
    UBOOT_TRACE("OK\n");
}

int do_DoSecureEngUnitTestCase1(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    UBOOT_TRACE("IN\n");
    _DoSecureEngUnitTestCase1();
    UBOOT_TRACE("OK\n");
    return 0;

}
int do_DoSecureEngUnitTestCase2(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    UBOOT_TRACE("IN\n");
    _DoSecureEngUnitTestCase2();
    UBOOT_TRACE("OK\n");
    return 0;

}
int do_DoSecureEngUnitTestCase3(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    UBOOT_TRACE("IN\n");
    _DoSecureEngUnitTestCase3();
    UBOOT_TRACE("OK\n");
    return 0;

}
int do_DoSecureEngUnitTestCase4(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    UBOOT_TRACE("IN\n");
    _DoSecureEngUnitTestCase4();
    UBOOT_TRACE("OK\n");
    return 0;

}

int do_DoSecureEngUnitTestCase5(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    UBOOT_TRACE("IN\n");
    _DoSecureEngUnitTestCase5();
    UBOOT_TRACE("OK\n");
    return 0;

}
#endif

void PopUpProtectedKey(void)
{
    char *pEnv=NULL;
    UBOOT_TRACE("IN\n");
    pEnv=getenv(ENV_PROTECTION_KEY);
    if(pEnv!=NULL)
    {
        if(0==strcmp((const char *)pEnv,"MSTAR"))
        {
            UBOOT_DEBUG("\033[0;34m The application bins are protected by MSTAR default key \033[0m\n");
        }
        else
        {
            UBOOT_DEBUG("\033[0;34m The application bins are protected by Customer key \033[0m\n");
        }
    }
    else
    {
        UBOOT_DEBUG("\033[0;34m The application bins are protected by MSTAR default key \033[0m\n");
    }
    UBOOT_TRACE("OK\n");
}

#if defined(CONFIG_SECURE_SPI_PROTECT)

#if defined(CONFIG_DISPLAY_LOGO)
#define INTERNAL_SPI_NON_PROTECT_SPACE (CONFIG_ENV_SIZE+CONFIG_ENV_SIZE+SECURITY_BOOT_SIGNATURE_SIZE+CONFIG_DISPLAY_LOGO_SPACE)
#else
#define INTERNAL_SPI_NON_PROTECT_SPACE (CONFIG_ENV_SIZE+CONFIG_ENV_SIZE+SECURITY_BOOT_SIGNATURE_SIZE)
#endif


void spi_write_protect_process_init(void)
{
    char *p_active = getenv("active_spi_wp");
    UBOOT_INFO("IN\n");

    if(p_active && (0 == strncmp(p_active,"on",2)))
    {
        int ret = 1;
        char spi_wp_active[256];
        UBOOT_DEBUG("active_spi_wp=on\n");
        memset(spi_wp_active, 0 , 256);
        snprintf(spi_wp_active, sizeof(spi_wp_active), "spi_in");
        UBOOT_DEBUG("cmd=%s\n",spi_wp_active);
        ret = run_command(spi_wp_active, 0);
        ASSERT((0 == ret));
#if defined(CONFIG_SECURE_SPI_PARTIAL_PROTECT)//partial protect
        uint ProtectLength      = CONFIG_SECURE_SPI_PROTECT_SPACE;
        uint NonProtectLength   = CONFIG_SECURE_SPI_NON_PROTECT_SPACE + INTERNAL_SPI_NON_PROTECT_SPACE;
        snprintf(spi_wp_active, sizeof(spi_wp_active), "spi_bwp 0x%x 0x%x", ProtectLength, NonProtectLength);
#elif defined(CONFIG_SECURE_SPI_FULL_PROTECT)//full protect
        snprintf(spi_wp_active, sizeof(spi_wp_active), "spi_wp %u", 1);
#else
        ASSERT(0);
#endif
        UBOOT_DEBUG("cmd=%s\n",spi_wp_active);
        ret = run_command(spi_wp_active, 0);
        ASSERT((0 == ret));
        snprintf(spi_wp_active, sizeof(spi_wp_active), "setenv active_spi_wp done");
        UBOOT_DEBUG("cmd=%s\n",spi_wp_active);
        ret = run_command(spi_wp_active, 0);
        ASSERT((0 == ret));
        snprintf(spi_wp_active, sizeof(spi_wp_active), "saveenv");
        UBOOT_DEBUG("cmd=%s\n",spi_wp_active);
        ret = run_command(spi_wp_active, 0);
        ASSERT((0 == ret));
    }
    UBOOT_INFO("OK\n");
}
#endif

void CheckSecureMerge(void)
{
    UBOOT_TRACE("IN\n");
    char *KL_buffer=NULL;
    KL_buffer=malloc(sizeof(SUB_SECURE_INFO));
    if(KL_buffer==NULL)
    {
        UBOOT_ERROR("malloc buffer fail\n");
        return;
    }

    if(-1 == GetSecureImage((U32)KL_buffer, KERNEL, 0, sizeof(SUB_SECURE_INFO), E_SECUREINFO))
    {
        UBOOT_ERROR("read kernel fail\n");
        free(KL_buffer);
        return;
    }

    if(0 == memcmp((const void *)(KL_buffer+sizeof(_SUB_SECURE_INFO)),(const void *)idForInterleaveMode,sizeof(idForInterleaveMode)))
    {
        UBOOT_DEBUG("Secure info and secure image is merge and Interleavemode\n");
        enSecureMergeType = E_SECUREMERGE_INTERLEAVE;
    }
    else if(0 == memcmp((const void *)KL_buffer,(const void *)idForContinueMode,sizeof(idForContinueMode)))
    {
        UBOOT_DEBUG("Secure info and secure image is merge and ContinueMode\n");
        enSecureMergeType = E_SECUREMERGE_CONTINUE;
    }
    else
    {
        UBOOT_DEBUG("secure info store in MPOOL\n");
        enSecureMergeType = E_SECUREMERGE_NONE;
    }

    free(KL_buffer);
    UBOOT_TRACE("OK\n");
}

EN_SECUREMERGE_TYPE GetSecureMergeType(void)
{
    return enSecureMergeType;
}

int GetSecureImage(unsigned int imageAddr, char * partition, U32 offset, U32 size, EN_SECUREMERGE_DATA secureMergeData)
{
    #define NUTTX_HEADER_LEN 0x80
    #define NUTTX_CONFIG_OFFSET 2
    int ret = 0;

    U32  u32NConfigIndex = 0;
    U32  u32NConfigBankSize = 0;
    char *pChr;


    UBOOT_TRACE("IN\n");

    raw_io_config_push();
    ret = raw_io_config(FLASH_RAW_DATA_TARGET, partition, UNNECESSARY_VOLUME);

    if(ret != 0){
        UBOOT_ERROR("raw_io_config setting fail!\n");
        return -1;
    }

    switch(secureMergeData)
    {
        case E_SECUREINFO :
            ret = raw_read((U32)imageAddr, offset, size);
            if(ret != 0){
                UBOOT_ERROR("raw_read %s secure info fail!\n", partition);
                return -1;
            }
            break;
        case E_SECUREIMAGE :
            if(GetSecureMergeType() != E_SECUREMERGE_NONE)
            {
                offset += sizeof(SUB_SECURE_INFO);
            }
            ret = raw_read((U32)imageAddr, offset, size);
            if(ret != 0){
                UBOOT_ERROR("raw_read %s secure image fail!\n", partition);
                return -1;
            }
            break;
        case E_NUTTXCONFIG :
            pChr = getenv(ENV_NCONFIG_INDEX);
            if( pChr != NULL )
            {
                u32NConfigIndex = simple_strtoul(pChr, NULL, 16);

                UBOOT_DEBUG("Switch Nconfig bank to number %d\n",u32NConfigIndex);
                if(u32NConfigIndex == 0)
                {
                    UBOOT_ERROR("Invail NConfig index\n");
                    return -1;
                }

                pChr = getenv(ENV_NCONFIG_SIZE);

                if( pChr != NULL )
                    u32NConfigBankSize = simple_strtoul(pChr, NULL, 16);
                else
                    u32NConfigBankSize = DEF_NCONFIG_BANK_SIZE;

                UBOOT_DEBUG("Nconfig bank size %d\n",u32NConfigBankSize);

                offset = offset + (u32NConfigIndex-1)*u32NConfigBankSize;
            }
            else
            {
                ret = raw_read((U32)imageAddr, offset, size);
                if(ret != 0)
                {
                    UBOOT_ERROR("raw_read nuttxconfig header fail!\n");
                    return -1;
                }

                u32NConfigBankSize += ((U32 *)imageAddr)[NUTTX_CONFIG_OFFSET] + ((U32 *)imageAddr)[NUTTX_CONFIG_OFFSET + 1];
                UBOOT_DEBUG("NuttxConfigSize =%x \n",(U32)u32NConfigBankSize);
            }

            ret = raw_read((U32)imageAddr,offset, u32NConfigBankSize);
            if(ret != 0){
                UBOOT_ERROR("raw_read nuttxconfig fail!\n");
                return -1;
            }
            break;
    }
    raw_io_config_pop();
    UBOOT_TRACE("OK\n");
    return ret;
}

BOOLEAN IsInterleaveMode(void)
{
    UBOOT_TRACE("IN\n");
    char u8SecIdentify[8] = "\0";
    if(GetSecureMergeType() == E_SECUREMERGE_NONE)
    {
        getSecIdentify(u8SecIdentify,&stSecureInfo,TRUE);
        if(0 == memcmp((const void *)u8SecIdentify,(const void *)idForInterleaveMode,sizeof(idForInterleaveMode)))
        {
            UBOOT_DEBUG("Interleave mode\n");
            UBOOT_TRACE("OK\n");
            return TRUE;
        }
        else
        {
            UBOOT_DEBUG("Continue mode\n");
            UBOOT_TRACE("OK\n");
            return FALSE;
        }
    }
    else if(GetSecureMergeType() == E_SECUREMERGE_INTERLEAVE)
    {
         UBOOT_DEBUG("Interleave mode\n");
         UBOOT_TRACE("OK\n");
         return TRUE;
    }
    else
    {
         UBOOT_DEBUG("Continue mode\n");
         UBOOT_TRACE("OK\n");
         return FALSE;
    }
}

int GetImageFromStorage(char *volume, unsigned int addr, unsigned int number)
{
    int ret=0;
    UBOOT_TRACE("IN\n");
    if(volume==NULL)
    {
        UBOOT_ERROR("The volume is a null pointer\n");
        return -1;
    }
    ret=_GetImageFromStorage(volume,addr,number,IsInterleaveMode());

    if(ret==0)
    {
        UBOOT_TRACE("OK\n");
    }
    else
    {
        UBOOT_ERROR("read image fail\n");
    }

    return ret;

}

int _GetImageFromStorage(char *volume, unsigned int addr, unsigned int number, BOOLEAN bIsInterleaveMode)
{
    int ret=0;
    unsigned int i=0;
    unsigned int size=0;
    unsigned int rsize=0;
    unsigned int offset=0;
    unsigned int blockSize=0;
    UBOOT_TRACE("IN\n");
    if(volume==NULL)
    {
        UBOOT_ERROR("The volume is a null pointer\n");
        return -1;
    }
    blockSize=GetBlockSize();
    ret=GetImageSize(volume, number, &size);
    if(ret!=0)
    {
        UBOOT_ERROR("get image %s size fail\n",volume);
        return -1;
    }
    if(bIsInterleaveMode==TRUE)
    {
        UBOOT_DEBUG("Read by interleave mode\n");
        while(size>0)
        {
            rsize=(size>blockSize)?blockSize:size;
            ret=ImageRead(volume,addr, blockSize*(i*FRAGMENT_NUM+number),rsize);
            size-=rsize;
            addr=addr+rsize;
            i++;
            if(ret==-1)
            {
                break;
            }
        }
    }
    else
    {
        UBOOT_DEBUG("Read by continue mode\n");
        //Get 0th size as unit.
        ret=GetImageSize(volume, 0, &offset);
        if(ret!=0)
        {
            UBOOT_ERROR("Get 0th of %s size fail\n",volume);
            return -1;
        }
        offset=offset*number;
        ret=ImageRead(volume,addr,offset,(U32)size);
    }

    if(ret==0)
    {
        UBOOT_TRACE("OK\n");
    }
    else
    {
        UBOOT_ERROR("Get image fail\n");
    }
    return ret;

}

static int read_from_partition(const char* partition,
                               int64_t offset,
                               size_t num_bytes,
                               void* buffer,
                               size_t* out_num_read) {
    int ret1;
    raw_io_config_push();
    ret1 = raw_io_config(FLASH_RAW_DATA_TARGET, partition, UNNECESSARY_VOLUME);

    if(ret1 != 0)
    {
        printf("raw_io_config setting fail!\n");
        return -1;
    }

    ret1 = raw_read((unsigned int)buffer, offset, num_bytes);
    if(ret1 != 0)
    {
        printf("%s:%d:%s: Error, raw_read %s fail!\n",__FILE__,__LINE__,__FUNCTION__,partition);
        return -1;
    }
    raw_io_config_pop();
    return num_bytes;

}

static int get_size_of_partition(const char* partition, uint64_t* out_size_in_bytes) {
    struct mtd_device *dev;
    struct part_info *partInfo;
    uint64_t* partSize = 0;
    uint64_t* partOffset = 0;
    uint8_t partNum;
    int ret = 0;

    if (get_mmc_part_size_and_offset(partition, (unsigned int*)partSize, (unsigned int*)partOffset) == 0)
    {
        *out_size_in_bytes = (*partSize)*512;
        ret = 0;
    }
    else
    {
        ret = -1;
    }
    return ret;
}

#if defined(CONFIG_SUPPORT_DTO)
static int emmc_load_dtbo(uint8_t *image_addr, size_t image_size)
{
    ulong ram_addr = CONFIG_DTBO_ADDR;
    int ret = 0;

    if(image_addr != NULL)
    {
        #ifdef CONFIG_SUPPORT_VERIFY_DTBO_BY_VTS
        struct dt_table_header * hdr = (struct dt_table_header *)ram_addr;
        #endif

        memcpy(ram_addr,image_addr,image_size);

        #ifdef CONFIG_SUPPORT_VERIFY_DTBO_BY_VTS
        if(be32_to_cpu(hdr->magic) != DT_TABLE_MAGIC)
        {
            printf("%s:%d:%s: Error, it is not a valid dtbo image.\n",__FILE__,__LINE__,__FUNCTION__);
            return 1;
        }
        #endif
    }
    else
    {
        size_t image_size = 0,read_size = 0;
        uint8_t pnum;
        #ifdef CONFIG_SUPPORT_VERIFY_DTBO_BY_VTS
        struct dt_table_header * hdr = (struct dt_table_header *)ram_addr;
        #else
        image_header_t *hdr = (image_header_t *)ram_addr;
        #endif

    #if 0 // CC_SUPPORT_AB_UPDATE
        char dtbo_partition[32] = "dtbo";
        char slot_suffix[8] = {0};
        extern uint8_t env_get_bootargs(const char* key, char* value);

        if(env_get_bootargs("androidboot.slot_suffix", slot_suffix))
        {
            strcat(dtbo_partition, slot_suffix);
        }
        else
        {
            printf("%s:%d:%s: Error, there is no bootable slot!\n",__FILE__,__LINE__,__FUNCTION__);
            return 1;
        }
        if ((mtdparts_init() == 0) &&
            (find_dev_and_part(dtbo_partition, &dev, &pnum, &part) == 0))
    #else
        char dtbo_partition[32] = "dtbo";

        if (get_size_of_partition(dtbo_partition, &image_size) == 0)
    #endif
        {
            printf("## %s: Load dtbo image from partition: %s\n",__FUNCTION__,dtbo_partition);

            image_size = 2048;
            //read_size = emmc_read(mmc_offset, ram_addr, image_size);
            read_size = read_from_partition(dtbo_partition, 0, image_size, ram_addr, NULL);
            if (read_size != image_size)
            {
                printf("%s:%d:%s: MMC Read %s partition error.\n",__FILE__,__LINE__,__FUNCTION__, dtbo_partition);
                return 1;
            }

            #ifdef CONFIG_SUPPORT_VERIFY_DTBO_BY_VTS
            if(be32_to_cpu(hdr->magic) != DT_TABLE_MAGIC)
            #else
            if(fdt_magic(image_get_data(hdr)) != FDT_MAGIC)
            #endif
            {
                printf("%s:%d:%s: Error, it is not a valid dtbo image.\n",__FILE__,__LINE__,__FUNCTION__);
                _dump(hdr, 0x100);
                printf("\n");
                return 1;
            }

            #ifdef CONFIG_SUPPORT_VERIFY_DTBO_BY_VTS
            image_size = be32_to_cpu(hdr->total_size);
            #else
            image_size = image_get_image_size(hdr);
            #endif
            if(image_size > read_size)
            {
                //read_size = emmc_read(mmc_offset, ram_addr, image_size);
                read_size = read_from_partition(dtbo_partition, 0, image_size, ram_addr, NULL);
                if (read_size != image_size)
                {
                    printf("%s:%d:%s: MMC Read %s partition error.\n",__FILE__,__LINE__,__FUNCTION__, dtbo_partition);
                    return 1;
                }
            }

            #ifndef CONFIG_SUPPORT_VERIFY_DTBO_BY_VTS
            printf("Verifying dtbo image Checksum ......");
            if (!image_check_dcrc(hdr))
            {
                printf(" Bad Data CRC !\n");
                return 1;
            }
            else
            {
                printf(" OK !\n");
            }
            #endif

            printf("## Load dtbo image done, image size: %d Byte !\n",image_size);
        }
        else
        {
            printf("%s:%d:%s: Can not find the dtbo partition: %s\n",__FILE__,__LINE__,__FUNCTION__, dtbo_partition);
            return 1;
        }
    }
    return 0;
}

static int readKL_dtb(uint32_t auth_addr, char* volume)
{
    struct andr_img_hdr ahdr;
    int magic_found, ret;
    int cnt, decomp_size;
    cnt = 2048;
    extern ulong ramdisk_start,ramdisk_len;
    extern uImage_dtb_size;

    read_from_partition(volume, 0, cnt, auth_addr, NULL);

    memcpy((void *)&ahdr, (void *)auth_addr, sizeof(andr_img_hdr));
    magic_found = !strncmp((char *)ahdr.magic, ANDR_BOOT_MAGIC, ANDR_BOOT_MAGIC_SIZE);

    if (magic_found)
    {
        printf("bootimg: ");
        // 1. load boot.img
        auth_addr -= ahdr.page_size;
        cnt = WITH_PADDING_SIZE(ahdr.kernel_size, ahdr.page_size) + ahdr.ramdisk_size + 3 * ahdr.page_size;
        printf("cnt=0x%x\n", cnt);
        if (cnt > MAX_IMAGE_SIZE)
        {   //limit size of boot.img
            cnt = MAX_IMAGE_SIZE;
        }

        read_from_partition(volume, 0, cnt, auth_addr, NULL);

        // 2. load ramdisk
        auth_addr += ahdr.page_size;
        memcpy(CONFIG_RAMDISK_ADDR, auth_addr + WITH_PADDING_SIZE(ahdr.kernel_size, ahdr.page_size), ahdr.ramdisk_size);
        ramdisk_start = CONFIG_RAMDISK_ADDR;
        ramdisk_len = ahdr.ramdisk_size;
    }
    else
    {
        UBOOT_ERROR("ERROR: android header format not found!!!\n");
    }
}
#endif

int do_ReadKL(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    char *volume = NULL;
    extern int IS_BOOTFLOW_FINISH;
    UBOOT_TRACE("IN\n");

    uint32_t auth_addr = BOOT_MODE_TMP_BUF_ADDR;
    get_temp_used_addr(&auth_addr);
    UBOOT_DEBUG("auth_addr=0x%08x\n", auth_addr);

    if (argc == 1)
    {
        volume = KERNEL;
        IS_BOOTFLOW_FINISH = EN_BOOT_MODE_NORMAL;
    }
    else if (argc == 2)
    {
        // argv[1] will be recovery for now
        volume = argv[1];
        IS_BOOTFLOW_FINISH = EN_BOOT_MODE_RECOVERY;
    }

    #if (CONFIG_VERIFY_SYSTEM == 1)
    if (strcmp(volume, "recovery"))
    {
        if(do_Verify_System(SYSTEM, (uint32_t)auth_addr) != 0)
        {
            UBOOT_ERROR("do_Verify_System %s fail\n", SYSTEM);
            return -1;
        }
    }
    #endif

    #if defined(CONFIG_ANDROID_AVB_ENABLE)
    char boot_partition_name[32] = {0};
    memset(boot_partition_name, 0, 32);
    strcpy(boot_partition_name, volume);
    avb_verify_boot_start(boot_partition_name, (uint32_t)auth_addr);
    #endif

    #if defined(CONFIG_SUPPORT_DTO)
    readKL_dtb(auth_addr, volume);
    emmc_load_dtbo(NULL, 0);
    #endif

    #if !defined (CONFIG_SECURE_BY_PASS_VERIFICATION) && !defined(CONFIG_ANDROID_AVB_ENABLE)
    // Authenticate Kernel/Recovery
    if(do_Authenticate_RawData(volume,(uint32_t)auth_addr)!=0)
    {
        UBOOT_ERROR("do_Authenticate_RawData %s fail\n",volume);
        IS_BOOTFLOW_FINISH = EN_BOOT_MODE_UNKNOWN;
        Secure_HaltCPU();
    }
    #endif

    // set dtb information
    #if !defined(CONFIG_SUPPORT_DTO)
    #if defined (CONFIG_ARMv8_64BIT_KERNEL) || (CONFIG_LOAD_DTB==1)
    #if defined(CONFIG_MMC)
    if(run_command("mmc read.p 0x23000000 dtb 0x0100000", 0)!=0)
    #elif defined(CONFIG_NAND_FLASH)
    if(run_command("nand read.e 0x23000000 dtb 0x0100000", 0)!=0)
    #endif
    {
        UBOOT_ERROR("do_SecureKLBoot fail\n");
        IS_BOOTFLOW_FINISH = EN_BOOT_MODE_UNKNOWN;
        Secure_HaltCPU();
    }
    #endif
    #endif

    return 0;
}

int do_BootKL(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    char buffer[CMD_BUF]="\0";
    UBOOT_TRACE("IN\n");

    uint32_t auth_addr = BOOT_MODE_TMP_BUF_ADDR;
    get_temp_used_addr(&auth_addr);
    UBOOT_DEBUG("auth_addr=0x%08x\n", auth_addr);

    //Boot Kernel
    snprintf(buffer, CMD_BUF, "bootm 0x%x",(int)auth_addr);
    UBOOT_INFO("[AT][MBoot][Boot Nuttx][%lu]\n", MsSystemGetBootTime());
    if(run_command(buffer, 0)!=0)
    {
        UBOOT_ERROR("do_BootKL fail\n");
        return -1;
    }
    return 0;
}

int do_SecureBootCmd(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    UBOOT_TRACE("IN\n");


#if defined (CONFIG_ARMv8_ARM_TRUSTED_FIRMWARE)|| defined(CONFIG_ARMv7_OPTEE)

    if(run_command("readArmFw;readOptee", 0)==-1)
    {
        UBOOT_ERROR("verify tee fail\n");
        return -1;
    }

    // read KL
    if(run_command("readKL", 0)==-1)
    {
        UBOOT_ERROR("verify KL fail\n");
        return -1;
    }

#else
    // lanuch TEE
    if(run_command("readNuttx;bootNuttx", 0)==-1)
    {
        UBOOT_ERROR("verify tee fail\n");
        return -1;
    }

    // read KL
    if(run_command("readKL", 0)==-1)
    {
        UBOOT_ERROR("verify KL fail\n");
        return -1;
    }

    // REE & TEE handshake
    if(run_command("wait_tee_ready", 0)==-1)
    {
        UBOOT_ERROR("fail to wait tee\n");
        return -1;
    }
#endif

#if (CONFIG_PANEL_INIT)// Init Panel
    if(run_command("panel_post_init", 0)==-1)
    {
        UBOOT_ERROR("panel_post_init fail\n");
        return -1;
    }
#endif

#if (CONFIG_LOCAL_DIMMING)
    run_command("local_dimming", 0);
#endif

    // Lanuch Kernel
    if(run_command("bootKL", 0)==-1)
    {
        UBOOT_ERROR("boot KL fail\n");
        return -1;
    }

    UBOOT_TRACE("OK\n");
    return 0;
}


int do_performance_test (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    int ret=0;
    UBOOT_TRACE("IN\n");
    if(argc!=3)
    {
        cmd_usage(cmdtp);
        return -1;
    }
    if((argv[1]==NULL)||(argv[2]==NULL))
    {
        UBOOT_ERROR("One of the parameters is a null pointer\n");
        return -1;
    }

    ret=ImageIOInit("UBI");



    if(ret!=-1)
    {
        if(strncmp(argv[1], "-c", 2) == 0)
        {
            UBOOT_INFO("==>start\n");
            ret=_GetImageFromStorage(argv[2],UPGRADE_BUFFER_ADDR,0,FALSE);
            UBOOT_INFO("==>end\n");
        }
        else if(strncmp(argv[1], "-i", 2) == 0)
        {
            UBOOT_INFO("==>start\n");
            ret=_GetImageFromStorage(argv[2],UPGRADE_BUFFER_ADDR,0,TRUE);
            UBOOT_INFO("==>end\n");
        }
        else
        {
            cmd_usage(cmdtp);
            return -1;
        }
    }

    if(ret!=-1)
    {
        UBOOT_TRACE("OK\n");
    }
    else
    {
        UBOOT_ERROR("Image IO init fail\n");

    }

    return 0;
}

U32 GetBlockSize(void)
{
    #define EMMC_READ_LEN_OF_CMD18 (512*4096) //CM18 can read 4096 blocks in one time.
    unsigned int size=0;
    UBOOT_TRACE("IN\n");
#if (ENABLE_MODULE_NAND_FLASH==1) || (ENABLE_MODULE_SPI_NAND_FLASH==1)
    size=ubi_get_leb_size();
#endif
#if(ENABLE_MODULE_MMC==1)
    size=EMMC_READ_LEN_OF_CMD18;
#endif

    UBOOT_DEBUG("block size=0x%x\n",size);
    UBOOT_TRACE("OK\n");
    return size;

}


#ifdef CONFIG_TFTP_SECURE_CHECK

char * get_utest_err_msg(void)
{
    return utest_err_msg;
}

void set_utest_err_msg(char * msg)
{
    memset(utest_err_msg, 0, sizeof(utest_err_msg));
    strncpy(utest_err_msg, msg, sizeof(utest_err_msg)-1);
}


int do_secure_image_check(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    UBOOT_TRACE("IN\n");
    U32 image_addr;
    U32 image_size;
    U8 cnt=0;
    SUB_SECURE_INFO stSubSecureInfo;
    U32  *pHash0E=NULL;
    U32  *pHash0N=NULL;
    U32 publicKeyN[(RSA_PUBLIC_KEY_N_LEN/4)];
    U32 publicKeyE[(RSA_PUBLIC_KEY_E_LEN/4)];
    U32 idx=0;
    #define OBJECT_DRAM_ADDR argv[1]
    #define OBJECT_SIZE argv[2]
    #define OBJECT_NAME argv[3]
    #define CHECK_LENGTH 32
    #define CHUNK_HEADER_SECURE_OFFSET 0x2F0
    #define CHUNK_HEADER_SIZE          0x400
    #define HASH1_SIG_ADDR 0x6F00
    #define HASH1_ADDR 0x3800
#if(ENABLE_MSTAR_MARLON == 1 ||ENABLE_MSTAR_MILAN == 1)
    #define HASH1_SIZE 0x3000
#else
    #define HASH1_SIZE 0x3600
#endif


    if(argc < 3)
    {
        cmd_usage(cmdtp);
        return -1;
    }

    if(argv[1] == NULL)
    {
        UBOOT_ERROR("One of the parameters is a null pointer\n");
        return -1;
    }
    image_addr = simple_strtoul(OBJECT_DRAM_ADDR, NULL, 16);
    image_size = simple_strtoul(OBJECT_SIZE, NULL, 16);

    if(((strcmp(OBJECT_NAME, "boot") == 0) || (strcmp(OBJECT_NAME, "recovery") == 0)) && (GetSecureMergeType() != E_SECUREMERGE_NONE))
    {
        if((0 != memcmp((const void *)image_addr, (const void *)idForContinueMode, sizeof(idForContinueMode))))
        {
            set_utest_err_msg("Image is not secure image!");
            UBOOT_ERROR("Image %s is non-secure image\n", OBJECT_NAME);
            return -1;
        }
        else
        {
            GetPublicKeyN(E_RSA_IMAGE_PUBLIC_KEY, publicKeyN, sizeof(publicKeyN));
            GetPublicKeyE(E_RSA_IMAGE_PUBLIC_KEY, publicKeyE, sizeof(publicKeyE));
            memcpy((void *)(image_addr + image_size), (const void *)image_addr, image_size);
            memcpy(&stSubSecureInfo, (void *)(image_addr + image_size), sizeof(SUB_SECURE_INFO));
            #if defined(CONFIG_SECURE_ENCRYPT_RAWDATA)
            if(-1 == DecryptKernelImage((char *)image_addr + image_size + sizeof(SUB_SECURE_INFO), stSubSecureInfo.sInfo.info.u32Size))
            {
                set_utest_err_msg("Image decrypt fail !");
                UBOOT_ERROR("\033[0;31m Dercypt %s FAIL\033[0m\n", OBJECT_NAME);
                return -1;
            }
            #endif//#if defined(CONFIG_SECURE_ENCRYPT_RAWDATA)
            if (-1 == Secure_doAuthentication((U8 *)publicKeyN, (U8 *)publicKeyE, (U8 *)(stSubSecureInfo.sInfo.u8Signature), (U8 *)image_addr + image_size + sizeof(SUB_SECURE_INFO), stSubSecureInfo.sInfo.info.u32Size))
            {
                set_utest_err_msg("Image secure key error");
                UBOOT_ERROR("Image %s secure key error\n", OBJECT_NAME);
                return -1;
            }
        }
    }
    else
    {
    #if defined(CONFIG_AUTH_KEYBANK_RSA_KEY_FROM_HASH2)
        U32 uboot_entry, uboot_size;
        _SUB_SECURE_INFO sInfo;
        U8 *pHash2N = getHash2PublicKeyN();
        U8 *pHash2E = getHash2PublicKeyE();
        memcpy((U8 *)publicKeyN, pHash2N, RSA_PUBLIC_KEY_N_LEN);
        memcpy((U8 *)publicKeyE, pHash2E, RSA_PUBLIC_KEY_E_LEN);

        if(strcmp(OBJECT_NAME, "MBOOT") == 0)
        {
            uboot_entry = image_addr + CONFIG_APP_HEADER_SIZE + CHUNK_HEADER_SIZE;
            uboot_size  = *(U32 *)(uboot_entry - 0x100 + 0x8) - *(U32 *)(uboot_entry - 0x100 + 0x4) - sizeof(_SUB_SECURE_INFO);

            memcpy(&sInfo, (void *)(uboot_entry + uboot_size), sizeof(_SUB_SECURE_INFO));
            if (strncmp(sInfo.u8SecIdentify, idForContinueMode, 8) != 0)
            {
                set_utest_err_msg("Image is not secure image!");
                UBOOT_ERROR("Can not find SecIdentify\n");
                return -1;
            }

            if (-1 == Secure_doAuthentication((U8 *)publicKeyN, (U8 *)publicKeyE, (U8 *)(sInfo.u8Signature), \
                                              (U8 *)uboot_entry, uboot_size))
            {
                set_utest_err_msg("Image MBOOT secure check error");
                UBOOT_ERROR("Image MBOOT secure check error\n");
                return -1;
            }
        }
    #else
        // For TV_type image (eg. Clipper)
        pHash0E = getHash0PublicKeyE();
        pHash0N = getHash0PublicKeyN();

        for(idx=0; idx<(RSA_PUBLIC_KEY_N_LEN/4); idx++)
        {
            publicKeyN[(RSA_PUBLIC_KEY_N_LEN/4)-1-idx] = *pHash0N;
            pHash0N++;
        }

        for(idx=0; idx<(RSA_PUBLIC_KEY_E_LEN/4); idx++)
        {
            publicKeyE[(RSA_PUBLIC_KEY_E_LEN/4)-1-idx] = *pHash0E;
            pHash0E++;
        }

        if(strcmp(OBJECT_NAME, "MBOOT") == 0)
        {
            memcpy(&stSubSecureInfo, (void *)(image_addr + CHUNK_HEADER_SECURE_OFFSET), sizeof(SUB_SECURE_INFO));
            if (-1 == Secure_doAuthentication((U8 *)publicKeyN, (U8 *)publicKeyE, (U8 *)(stSubSecureInfo.sInfo.u8Signature), (U8 *)image_addr, CHUNK_HEADER_SECURE_OFFSET))
            {
                set_utest_err_msg("Image MBOOT secure check error");
                UBOOT_ERROR("Image MBOOT secure check error\n");
                return -1;
            }
        }
        else if(strcmp(OBJECT_NAME, "sboot") == 0)
        {
            memcpy((void *)stSubSecureInfo.sInfo.u8Signature, (void *)(image_addr + HASH1_SIG_ADDR), SIGNATURE_LEN);
            if (-1 == Secure_doAuthentication((U8 *)publicKeyN, (U8 *)publicKeyE, (U8 *)(stSubSecureInfo.sInfo.u8Signature), (U8 *)(image_addr + HASH1_ADDR), HASH1_SIZE))
            {
                set_utest_err_msg("Image ROM BOOT secure check error");
                UBOOT_ERROR("Image ROM BOOT secure check error\n");
                return -1;
            }
        }
    #endif
    }

    UBOOT_DEBUG("Image %s is secure image\n", OBJECT_NAME);
    UBOOT_TRACE("OK\n");
    return 0;
}
#endif


MS_BOOL get_secure_init_status(void)
{
    return secure_init_ok;
}
