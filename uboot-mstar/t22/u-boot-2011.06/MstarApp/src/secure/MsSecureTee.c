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

//-------------------------------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------------------------------
#include <exports.h>
#include <ShareType.h>
#include <MsDebug.h>
#include <MsSecureTee.h>
#include <MsMmap.h>
#include <MsUtility.h>
#include <MsSystem.h>
#include <MsRawIO.h>
#include <secure/MsSecureBoot.h>
#include <MsSetupTee.h>
#include <MsEnvironment.h>
#include <CusConfig.h>
#include <ShareType.h>
#include <MsSysUtility.h>
#include <miu/MsDrvMiu.h>
#include <MsStr.h>
//#define CONFIG_NUTTX_BRINGUP_TEST 1

#if defined (CONFIG_NCONFIG_STORE_IN_SPI_FLASH)
#include <MsApiSpi.h>
#elif defined (CONFIG_NCONFIG_STORE_IN_NAND_FLASH)
//wait for implement
#elif defined (CONFIG_NCONFIG_STORE_IN_EMMC_FLASH)
#include <MsMmc.h>
#elif defined (CONFIG_NCONFIG_STORE_IN_UFS_FLASH)
#include <MsUfs.h>
#else
#error "please set the correct security storage!!\n"
#endif //#if defined (CONFIG_NCONFIG_STORE_IN_SPI_FLASH)

#include <MsSecureBoot.h>
#include <CusConfig.h>

#include "drvSERFLASH.h" //for flash copy fw from flash to dram
#include "drvBDMA.h" //for bdma copy fw from flash to dram


//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

#if defined (CONFIG_NCONFIG_STORE_IN_SPI_FLASH)
#define FLASH_DEFAULT_TARGET  E_RAW_DATA_IN_SPI
#define FLASH_DEFAULT_PARTITION SPI_DEFAULT_PARTITION
#define FLASH_DEFAULT_VOLUME SPI_DEFAULT_VOLUME
#define SECTOR_SIZE   0x10000 //64KBytes
#elif defined (CONFIG_NCONFIG_STORE_IN_NAND_FLASH)
extern  int ubi_get_volume_size(char *, size_t* );
extern int ubi_get_leb_size(void);
#define FLASH_DEFAULT_TARGET  E_RAW_DATA_IN_NAND
#define FLASH_DEFAULT_PARTITION NAND_DEFAULT_PARTITION
#define FLASH_DEFAULT_VOLUME NAND_DEFAULT_VOLUME
#define SECTOR_SIZE   ubi_get_leb_size()
#define SECURE_INFOR_BACK_OFFSET 1 // unit is sectore, so the total offset is 128*512=64KBytes
#elif defined  (CONFIG_NCONFIG_STORE_IN_EMMC_FLASH)
#define FLASH_DEFAULT_TARGET  E_RAW_DATA_IN_MMC
#define FLASH_DEFAULT_PARTITION MMC_DEFAULT_PARTITION
#define FLASH_DEFAULT_VOLUME MMC_DEFAULT_VOLUME
#define SECTOR_SIZE   0x200 //512Bytes
#define SECURE_INFOR_BACK_OFFSET 128 // unit is sectore, so the total offset is 128*512=64KBytes
#elif defined  (CONFIG_NCONFIG_STORE_IN_UFS_FLASH)
#define FLASH_DEFAULT_TARGET  E_RAW_DATA_IN_UFS
#define FLASH_DEFAULT_PARTITION UFS_DEFAULT_PARTITION
#define FLASH_DEFAULT_VOLUME UFS_DEFAULT_VOLUME
#define SECTOR_SIZE   0x1000
#define SECURE_INFOR_BACK_OFFSET 128 // unit is sectore, so the total offset is 128*4096=512KBytes
#else
#error "please set the correct security storage!!\n"
#endif

#define TEE_DYNAMIC_LOAD_MAGIC_NUM  0xFA16
#define NUTTX_HEADER_LEN 0x80
#define ALIGN_BYTES 16
#define NUTTX_CONFIG_OFFSET 8
#define FOUR_BIT_SHIFT 4
#ifdef CONFIG_MSTAR_R2_STR
#define STR_FLAG_RESUME_FINISH              0XFFFF9999
#define TEESTORESIZE     0X100000
#endif

#if defined(CONFIG_R2TEE_TEELOADER)
#ifndef UART_DBG_SECR2
#if (CONFIG_TV_CHIP==0)
#define UART_DBG_SECR2          0x00
#else
#define UART_DBG_SECR2          0x0D
#endif
#endif
#endif //#if defined(CONFIG_R2TEE_TEELOADER)

typedef struct
{
    U32 Length;
    U32 Offset;
}ST_HEADER_INFO;

typedef struct
{
    ST_HEADER_INFO stSignature;
    ST_HEADER_INFO stConfig;
}ST_SIGN_CONFIG;

typedef enum
{
    EN_TEE_DEBUG_LEVEL_DISABLE=0,
    EN_TEE_DEBUG_LEVEL_ERROR=0x01,
    EN_TEE_DEBUG_LEVEL_INFO=0x02,
    EN_TEE_DEBUG_LEVEL_TRACE=0x04,
    EN_TEE_DEBUG_LEVEL_DEBUG=0x08,
    EN_TEE_DEBUG_LEVEL_DEBUG_L2=0x10,
    EN_TEE_DEBUG_LEVEL_INVALID,
    EN_TEE_DEBUG_LEVEL_MAX=EN_TEE_DEBUG_LEVEL_INVALID
}EN_TEE_DEBUG_LEVEL;

#define NUTTX_BOOT_CONFIG_SIZE 0x1000
#define TEE_BOOT_CONFIG_ID "TEE_BOOT_CONFIG"
typedef struct
{
    U8  u8BootConfigID[16];
    EN_TEE_DEBUG_LEVEL  enDebugLevel;
    U8  u8FastBootMode;
    U8  u8Reserved[64];
}ST_TEE_BOOT_CONFIG;

#ifdef CONFIG_MSTAR_FAST_STR
typedef struct
{
    U32 u32Nuttx_size;
    U32 u32Start_data_addr;
    U32 u32End_data_addr;
    U32 u32Start_ta_addr;
    U32 u32End_ta_addr;
}ST_BACKUP_HEADER_INFO;
#define NUTTX_HEADER_OFFSET 0x168
#endif


#define _PA2VA(x)   (MS_U32)MsOS_PA2KSEG0((x))
#define _PA2NVA(x)  (MS_U32)MsOS_PA2KSEG1((x))
#define _VA2PA(x)   (MS_U32)MsOS_VA2PA((x))

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
U32 gu32TeeBootConfigAddr=0;
U32 gu32TeeHandShakeAddr=0;

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------

static char* pNuttxAlignMallocAddr=NULL;
static char* pNuttxOriMallocAddr=NULL;
static U32 NuttxAlignSize=0;
static U8  u8DynamicLoadNuttxMode = 0;

#define ENV_TEE_DEBUG_LEVEL          "teeDebugLevel"

#define ENV_NCONFIG_INDEX            "nconfig_index"
#define ENV_NCONFIG_SIZE             "nconfig_size"
#define DEF_NCONFIG_BANK_SIZE        0x4000


//-------------------------------------------------------------------------------------------------
//  Extern Functions
//-------------------------------------------------------------------------------------------------
extern int LoadCustomerKeyBank(void);
extern int do_Authenticate_RawData(char *KLName, int CacheBufAddr);
extern EN_SECUREMERGE_TYPE GetSecureMergeType(void);
extern int GetSecureImage(unsigned int imageAddr, char * volume, U32 offset, U32 size, EN_SECUREMERGE_DATA secureMergeData);
extern int GetImageSize(char *volume, U8 num, U32 *size);

//-------------------------------------------------------------------------------------------------
//  Private Functions
//-------------------------------------------------------------------------------------------------
int get_nconfig_offset(unsigned int *u32NConfigOffset,unsigned int *u32NConfigBakOffset);
int GetTEEImageSize(EN_SECUREMERGE_TYPE secureMergeType);
int NConfigSave(U8* pBufferAddr, U32 u32NConfigLen);
int NConfigLoad(U32 u32HwAesVABufAddr);
int LoadNConfig(U32 u32HwAesVABufAddr);
int get_nconfig_size(unsigned int *u32NConfigSize);
void _CheckNuttxAddr(unsigned int uitee_start_paddr);
void SetNuttxAddrToDummyReg(void);
//-------------------------------------------------------------------------------------------------
//  Public Functions
//-------------------------------------------------------------------------------------------------
#if defined(CONFIG_R2TEE_TEELOADER)
int do_uart2R2(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    char *pEnv=NULL;
    pEnv=getenv("uart2R2");
    if(pEnv!=NULL)
    {
        if(strcmp(pEnv,"1")==0)
        {
            printf("switch uart message to R2\n");
            WriteByte(0x101EA6, UART_DBG_SECR2);
        }
    }
    return 0;
}
#endif

#if defined (CONFIG_R2_BRINGUP)

void MApi_R2_FwCopy(MS_U32 u32FlashAddr, MS_U32 u32DramAddr, MS_U32 u32Len)
{
    printf("Copy R2 Fw from Flash 0x%08x to Ram 0x%08x, len = 0x%08x ... ",
        (unsigned int)u32FlashAddr, (unsigned int)u32DramAddr, (unsigned int)u32Len);

    MDrv_BDMA_SetDbgLevel(E_BDMA_DBGLV_ALL);

    MDrv_BDMA_Init(0x20000000);

    MDrv_BDMA_FlashCopy2Dram((MS_PHYADDR)u32FlashAddr, (MS_PHYADDR)u32DramAddr, u32Len);

    printf("Done!\n");
}

void SetHKtoGo(U32 address)
{
    char str[]="HK is going";
    UBOOT_TRACE("IN\n");

    UBOOT_DEBUG("address=0x%x\n",address);
    memset((void *)address,0 , sizeof(str)+1);
    memcpy((void *)address,str,sizeof(str));
    UBOOT_DEBUG("Mboot Starts to go\n");

    UBOOT_TRACE("OK\n");

    return;
}

int IsSecureMemDone(U32 address)
{
    char str[]="Secure memory is done";
    char *s=NULL;
    U32 timeOut=0;
    U32 count=0;
    UBOOT_TRACE("IN\n");
    UBOOT_DEBUG("address=0x%x\n",address);
    s = getenv("timeOut");
    if(s==NULL)
    {
        timeOut=60000;//60s
    }
    else
    {
        timeOut=simple_strtoul(s, NULL, 16);
    }
    UBOOT_DEBUG("timeOut=%d(ms)\n",timeOut);


    while(1){
            Chip_Flush_Memory();
        if(memcmp((void *)address,str,sizeof(str))==0)
        {
            UBOOT_TRACE("OK\n");
            return 0;
        }

        //MsOS_DelayTaskUs(100);
        MsOS_DelayTask(1);
        count++;
        if(count==timeOut){
            UBOOT_DEBUG("Time out\n");
            break;
        }
    }

    UBOOT_ERROR("fail\n");
    return -1;
}

void* AllocateNuttxbuffer(int BufSize)
{
    UBOOT_TRACE("IN\n");
    U8* pu8R2MallocAddr=NULL;
    UBOOT_DEBUG("BufSize =%x \n",BufSize);
    pu8R2MallocAddr=malloc(BufSize);
    //pu8R2MallocAddr=(U8*)((U32)((U32)pu8R2MallocAddr+16))>>4<<4;

    if(pu8R2MallocAddr==NULL)
    {
        UBOOT_ERROR("\033[0;31m malloc FAIL\033[0m\n");
        return NULL;
    }

    UBOOT_DEBUG("pu8R2MallocAddr =%x \n",(U32)pu8R2MallocAddr);
    UBOOT_TRACE("OK\n");

    return pu8R2MallocAddr;
}

void DeAllocateNuttxbuffer(void* BufAddr)
{
    UBOOT_TRACE("IN\n");

    if(BufAddr!=NULL)
    {
        free(BufAddr);
        BufAddr=NULL;
    }

    UBOOT_TRACE("OK\n");
    return;
}

int CalculateNuttxAlignSize(char* pu8R2HeaderAddr)
{
    ST_TEE_HEADER* pstTeeHd=NULL;
    U32 NuttxSize=0;

    UBOOT_TRACE("IN\n");

    pstTeeHd=(ST_TEE_HEADER*)pu8R2HeaderAddr;
    if(memcmp(pstTeeHd->ucMagicID,TeeMagicID,strlen(TeeMagicID))==0)
    {
        UBOOT_DEBUG("pstTeeHd->_sthd.uitee_image_size =%x \n",pstTeeHd->_sthd.uitee_image_size);

        if(pstTeeHd->_sthd.uitee_image_size==0){
            UBOOT_ERROR(" error size\n");
            return 0;
        }


        if(pstTeeHd->_sthd.uitee_image_size%ALIGN_BYTES){
            NuttxSize=(((pstTeeHd->_sthd.uitee_image_size+ALIGN_BYTES)>>FOUR_BIT_SHIFT)<<FOUR_BIT_SHIFT)+NUTTX_HEADER_LEN;
        }
        else{
            NuttxSize=pstTeeHd->_sthd.uitee_image_size+NUTTX_HEADER_LEN;
        }
    }

    UBOOT_TRACE("OK\n");

    return NuttxSize;
}

void _CheckNuttxAddr(unsigned int uitee_start_paddr)
{
    char *puEnvNuttxAddr=NULL;
    unsigned int u32EnvNuttxAddr=0;
    puEnvNuttxAddr = getenv("E_MMAP_ID_NUTTX_MEM");
    if(get_map_addr_from_env(E_NUTTX_MEM, NO_DEFAULT_MMAP_VALUE, &u32EnvNuttxAddr)==0)
    {
        if(uitee_start_paddr!=u32EnvNuttxAddr)
        {
            UBOOT_ERROR("TEE Header Address and MMAP is mismatch!!!\n");
            UBOOT_ERROR("TEE Header Address:0x%x\n",uitee_start_paddr);
            UBOOT_ERROR("MMAP Address:0x%x\n",u32EnvNuttxAddr);
        }
    }
}


#ifdef CONFIG_MSTAR_FAST_STR

int TEE_BACKUP(MS_PHYADDR NuttxPhyAddr)
{
    #define STR_BootFlagSize    0x4
    #define NConfigBKSize       0x10000
    #define KeyBankBKSize       0x450
    #define TEEStoreSize        0x100000

    // TEE BACK UP
    UBOOT_DEBUG("*****_PA2NVA(NUTTX_BACKUP_HEADER) =%lx*****\n",_PA2NVA(NuttxPhyAddr + 0x100 - 0x20));
    UBOOT_DUMP((_PA2NVA(NuttxPhyAddr + NUTTX_HEADER_OFFSET)), 0x30);

    ST_BACKUP_HEADER_INFO BackUpInfo;

    memcpy((void*)&BackUpInfo,(void*)_PA2NVA(NuttxPhyAddr + NUTTX_HEADER_OFFSET),sizeof(ST_BACKUP_HEADER_INFO));

    UBOOT_DEBUG("BackUpInfo = 0x%x\n",BackUpInfo.u32Nuttx_size);
    UBOOT_DEBUG("BackUpInfo.u32Nuttx_size = 0x%x\n",BackUpInfo.u32Nuttx_size);
    UBOOT_DEBUG("BackUpInfo.u32Start_data_addr = 0x%x\n",BackUpInfo.u32Start_data_addr);
    UBOOT_DEBUG("BackUpInfo.u32End_data_addr = 0x%x\n",BackUpInfo.u32End_data_addr);
    UBOOT_DEBUG("BackUpInfo.u32Start_ta_addr = 0x%x\n",BackUpInfo.u32Start_ta_addr);
    UBOOT_DEBUG("BackUpInfo.u32End_ta_addr = 0x%x\n",BackUpInfo.u32End_ta_addr);

    MS_PHYADDR Nuttx_End_Addr = 0;
    U32 u32DataSectionStoreAddr = 0;
    U32 u32DataSectionLength = BackUpInfo.u32End_data_addr - BackUpInfo.u32Start_data_addr;
    U32 u32TASectionLength = BackUpInfo.u32End_ta_addr - BackUpInfo.u32Start_ta_addr;
    MS_PHYADDR NuttxStart = 0;
    U32 u32NuttxLen= 0;
    //get_nconfig_size(&u32NConfigSize);


    if (get_map_addr_from_env(E_NUTTX_MEM, NO_DEFAULT_MMAP_VALUE, &NuttxStart) != 0)
    {
        UBOOT_ERROR("get %s mmap fail\n",E_MMAP_ID_NUTTX_MEM_ADR);
        return -1;
    }
    if (get_map_size_from_env(E_NUTTX_MEM, NO_DEFAULT_MMAP_VALUE, &u32NuttxLen) != 0)
    {
        UBOOT_ERROR("get %s mmap fail\n",E_MMAP_ID_NUTTX_MEM_LEN);
        return -1;
    }
    Nuttx_End_Addr = NuttxStart + u32NuttxLen - TEEStoreSize - NUTTX_BOOT_CONFIG_SIZE;//Nuttx Dram End

    U32 u32TEEBackUpSize =  STR_BootFlagSize/*STR Boot Magic ID*/ + NConfigBKSize/*backup*/ + NUTTX_BOOT_CONFIG_SIZE/*backup*/+ KeyBankBKSize/*backup*/;

    UBOOT_DEBUG("Nuttx_End_Addr = 0x%llx\n",Nuttx_End_Addr);
    UBOOT_DEBUG("u32TEEBackUpSize = 0x%x\n",u32TEEBackUpSize);
    UBOOT_DEBUG("u32DataSectionLength = 0x%x\n",u32DataSectionLength);
    UBOOT_DEBUG("u32TASectionLength = 0x%x\n",u32TASectionLength);
#ifndef CONFIG_MSTAR_FAST_STR_PHASE2
    if(TEEStoreSize > (u32TEEBackUpSize + u32DataSectionLength + u32TASectionLength))//Backup Data Section
    {
        //backup data section address
        u32DataSectionStoreAddr = Nuttx_End_Addr + u32TEEBackUpSize;
        UBOOT_DEBUG("STR Boot Magic ID Address = 0x%llx\n",           Nuttx_End_Addr);
        UBOOT_DEBUG("Back Up Boot Config Address = 0x%llx\n",         Nuttx_End_Addr + STR_BootFlagSize + NConfigBKSize);
        UBOOT_DEBUG("Back Up Customer Key Bank Address  = 0x%llx\n",  Nuttx_End_Addr + STR_BootFlagSize + NConfigBKSize + NUTTX_BOOT_CONFIG_SIZE);
        //TA section range
        UBOOT_DEBUG("DataSectionStoreAddr:Start = 0x%x\n",          u32DataSectionStoreAddr);
        UBOOT_DEBUG("DataSectionStoreAddr:End = 0x%x\n",            u32DataSectionStoreAddr + u32DataSectionLength);
        UBOOT_DEBUG("TASectionStoreAddr:Start = 0x%x\n",            u32DataSectionStoreAddr + u32DataSectionLength + 1);
        UBOOT_DEBUG("TASectionStoreAddr:End = 0x%x\n",              u32DataSectionStoreAddr + u32DataSectionLength + u32TASectionLength);
        memcpy(((void*)_PA2VA(u32DataSectionStoreAddr)),((void*)_PA2NVA(BackUpInfo.u32Start_data_addr)),u32DataSectionLength);
        flush_cache(_PA2VA(u32DataSectionStoreAddr),u32DataSectionLength);
        if(0 == memcmp((void*)_PA2VA(u32DataSectionStoreAddr),(void*)_PA2NVA(BackUpInfo.u32Start_data_addr),u32DataSectionLength))
        {
            UBOOT_DUMP((_PA2NVA(u32DataSectionStoreAddr)), 0x40);
        }
        //backup ta section
        memcpy(((void*)_PA2NVA(u32DataSectionStoreAddr+u32DataSectionLength+1)),((void*)_PA2NVA(BackUpInfo.u32Start_ta_addr)),u32TASectionLength);
        flush_cache(_PA2NVA(u32DataSectionStoreAddr+u32DataSectionLength+1),u32TASectionLength);
        if(0 == memcmp((void*)_PA2NVA(u32DataSectionStoreAddr+u32DataSectionLength+1),(void*)_PA2NVA(BackUpInfo.u32Start_ta_addr),u32TASectionLength))
        {
            UBOOT_DUMP((_PA2NVA(u32DataSectionStoreAddr+u32DataSectionLength+1)), u32TASectionLength);
        }
    }
    else
    {
        UBOOT_ERROR("TEE back up space not enough!!\n");
    }
#endif
    return 0;
}

int STR_BACKUP(MS_PHYADDR NuttxPhyAddr)
{
    ST_FSTR_BACKUP *stFstrBackUp=NULL;
    MS_PHYADDR AesAddr=0;
    MS_PHYADDR NuttxAddr=0;
    stFstrBackUp=(ST_FSTR_BACKUP *)0x201F0000;
    memset(stFstrBackUp, 0x0, sizeof(stFstrBackUp));
#ifdef CONFIG_MSTAR_FAST_STR_PHASE2
    if(get_map_addr_from_env(E_NUTTX_STR_STATUS, NO_DEFAULT_MMAP_VALUE, &AesAddr)!=0)
    {
        UBOOT_ERROR("E_NUTTX_STR_STATUS failed\n");
        return -1;
    }
#else
    if(get_map_addr_from_env(E_HW_AES_BUF, NO_DEFAULT_MMAP_VALUE, &AesAddr)!=0)
    {
        UBOOT_ERROR("E_HW_AES_BUF failed\n");
        return -1;
    }
#endif
    if(get_map_addr_from_env(E_NUTTX_MEM, NO_DEFAULT_MMAP_VALUE, &NuttxAddr)!=0)
    {
        UBOOT_ERROR("E_NUTTX_MEM failed\n");
        return -1;
    }
    //Address Back Up
    stFstrBackUp->u32NuttxAddr=NuttxAddr;
    stFstrBackUp->u32AESAddr=AesAddr;
    MSDrv_FSTR_MiuBackup(&stFstrBackUp->stMiuInfo);
    //Miu information Back Up
    UBOOT_DEBUG("*****u32NuttxAddr=[0x%x] u32AESAddr [0x%x] \n",stFstrBackUp->u32NuttxAddr,stFstrBackUp->u32AESAddr);
    UBOOT_DEBUG("*****u32MiuPriority[0]=[0x%x] u32MiuPriority[1] [0x%x] \n",stFstrBackUp->stMiuInfo.u32MiuPriority[0],stFstrBackUp->stMiuInfo.u32MiuPriority[1]);
    UBOOT_DEBUG("*****MIU 0 u32MiuSel[0]=[0x%x] u32MiuSel[1] [0x%x] \n",stFstrBackUp->stMiuInfo.u32MiuSel[0][0],stFstrBackUp->stMiuInfo.u32MiuSel[0][1]);
    UBOOT_DEBUG("*****MIU 1 u32MiuSel[0]=[0x%x] u32MiuSel[1] [0x%x] \n",stFstrBackUp->stMiuInfo.u32MiuSel[1][0],stFstrBackUp->stMiuInfo.u32MiuSel[1][1]);
    // TEE BACK UP
    if (TEE_BACKUP(NuttxPhyAddr) != 0)
    {
        UBOOT_ERROR("TEE_BACKUP failed\n");
        return -1;
    }
    return 0;
}

#endif


U32 MoveNuttx2Dest(U32 NuttxVirtMallocAddr, U32 NuttxSize)
{
    ST_TEE_HEADER* pstTeeHd=NULL;
    MS_PHYADDR ulTempAddr = 0;
    unsigned long ulImageSize = 0;
    unsigned long ulHeaderLen = 0;
    MS_PHYADDR NuttxPhyAddr = 0;
    MS_PHYADDR NuttxPhyMallocAddr=0;
    UBOOT_TRACE("IN\n");

    UBOOT_DUMP(_PA2NVA(_VA2PA(NuttxVirtMallocAddr)), 0x100);

    pstTeeHd=(ST_TEE_HEADER*)NuttxVirtMallocAddr;
    UBOOT_DEBUG("NuttxVirtMallocAddr=0x%x\n",(U32)NuttxVirtMallocAddr);
    UBOOT_DEBUG("pstTeeHd->ucMagicID=%s\n",pstTeeHd->ucMagicID);

    UBOOT_DEBUG("*****compare magic id*****\n");
    if(memcmp(pstTeeHd->ucMagicID,TeeMagicID,strlen(TeeMagicID))==0)
    {

        if( pstTeeHd->_sthd.uitee_start_paddr == 0 )
        {
            if (get_map_addr_from_env(E_NUTTX_MEM, NO_DEFAULT_MMAP_VALUE, &pstTeeHd->_sthd.uitee_start_paddr) != 0)
            {
                UBOOT_ERROR("get %s mmap fail\n",E_MMAP_ID_NUTTX_MEM_ADR);
                // when NuttxPhyAddr = 0, MoveNuttx2Dest is thought to be failed
                return 0;
            }
            UBOOT_DEBUG("get nuttx addr from env\n");
            u8DynamicLoadNuttxMode = 1;
        }

        UBOOT_DEBUG("pstTeeHd->uitee_header_len=%x\n",pstTeeHd->_sthd.uitee_header_len);
        UBOOT_DEBUG("pstTeeHd->uitee_start_paddr=0x%x\n",pstTeeHd->_sthd.uitee_start_paddr);
        UBOOT_DEBUG("pstTeeHd->uitee_start_vaddr=0x%x\n",pstTeeHd->_sthd.uitee_start_vaddr);
        UBOOT_DEBUG("pstTeeHd->uitee_image_size=0x%x\n",pstTeeHd->_sthd.uitee_image_size);
        UBOOT_DEBUG("pstTeeHd->ulltime=%llx\n",pstTeeHd->_sthd.ulltime);
        UBOOT_DEBUG("pstTeeHd->ullTeeBinVersion=%llx\n",pstTeeHd->_sthd.ullTeeBinVersion);
        UBOOT_DEBUG("pstTeeHd->uiHeaderVersion=%d\n",pstTeeHd->_sthd.uiHeaderVersion);
        UBOOT_DEBUG("pstTeeHd->uicrc_value=%d\n",pstTeeHd->_sthd.uicrc_value);

        if((pstTeeHd->_sthd.uitee_start_paddr==0)&&(pstTeeHd->_sthd.uitee_start_vaddr==0)&&(pstTeeHd->_sthd.uitee_image_size==0))
        {
            UBOOT_ERROR("header info in wrong, please check again");
            return 0;
        }

        //check TEE Header Address and MMAP
        _CheckNuttxAddr(pstTeeHd->_sthd.uitee_start_paddr);

        ulTempAddr = pstTeeHd->_sthd.uitee_start_paddr;
        ulImageSize = pstTeeHd->_sthd.uitee_image_size;
        ulHeaderLen = pstTeeHd->_sthd.uitee_header_len;
        NuttxPhyAddr = _VA2PA(NuttxVirtMallocAddr);
        UBOOT_DEBUG("NuttxPhyAddr=%x\n",NuttxPhyAddr);

#if defined (CONFIG_AMP_TRUSTZONE_ENABLE)
        ulTempAddr+=0x8000;
#endif
        UBOOT_DUMP(_PA2NVA(_VA2PA(NuttxVirtMallocAddr)), 0x100);
        //UBOOT_DUMP((unsigned long)((u32R2BaseAddr|CONFIG_MIU0_BUSADDR|0x20000000)+ulImageSize+0x80-0x200), 0x200);

        UBOOT_DEBUG("*****MDrv_BDMA_MemCopy*****\n");
        #if !defined (CONFIG_NUTTX_BRINGUP_TEST)
        MDrv_BDMA_MemCopy((U32)NuttxPhyAddr+ulHeaderLen, ulTempAddr, ulImageSize);
        #else
        memcpy((U32)NuttxVirtMallocAddr+ulHeaderLen, PA2VA(ulTempAddr), ulImageSize);
        #endif
#ifdef CONFIG_MSTAR_FAST_STR
        if (STR_BACKUP(NuttxPhyAddr) != 0)
        {
            UBOOT_ERROR("STR_BACKUP failed!\n");
            return 0;
        }
                //TEE_BACKUP(NuttxPhyAddr);
#endif
        NuttxPhyAddr=ulTempAddr;
        UBOOT_DEBUG("phdinfo->ultee_start_paddr=%x\n",pstTeeHd->_sthd.uitee_start_paddr);
        UBOOT_DEBUG("NuttxPhyAddr=%x\n",NuttxPhyAddr);
        UBOOT_DEBUG("_PA2NVA(NuttxPhyAddr)=%lx\n",_PA2NVA(NuttxPhyAddr));

        UBOOT_DEBUG("*****_PA2VA(NuttxPhyAddr) =%lx*****\n",_PA2VA(NuttxPhyAddr));
        UBOOT_DUMP((_PA2VA(NuttxPhyAddr+0x100)), 0x110);
        UBOOT_DEBUG("*****_PA2NVA(NuttxPhyAddr) =%lx*****\n",_PA2NVA(NuttxPhyAddr));
        UBOOT_DUMP((_PA2NVA(NuttxPhyAddr+0x100)), 0x110);
        //UBOOT_DUMP((unsigned long)((NuttxPhyAddr|CONFIG_MIU0_BUSADDR|0x20000000)+ulImageSize-0x100), 0x100);
    }
    else
    {
        UBOOT_DEBUG("*****Without Header*****\n");
        NuttxPhyMallocAddr = _VA2PA(NuttxVirtMallocAddr);
        UBOOT_DEBUG("NuttxPhyMallocAddr=%llx\n",NuttxPhyMallocAddr);

        if (get_map_addr_from_env(E_NUTTX_MEM, 0xa200000, &NuttxPhyAddr) != 0)
        {
            UBOOT_ERROR("get %s mmap fail\n",E_MMAP_ID_NUTTX_MEM_ADR);
            return 0;
        }

        #if !defined (CONFIG_NUTTX_BRINGUP_TEST)
        MDrv_BDMA_MemCopy(NuttxPhyMallocAddr, NuttxPhyAddr, NuttxSize);
        #else
        memcpy(NuttxPhyMallocAddr, NuttxPhyAddr, NuttxSize);
        #endif
    }

    UBOOT_TRACE("OK\n");
    return NuttxPhyAddr;
}

MS_PHYADDR get_hw_aes_addr_from_tee_hd(U32 u32NuttxVirtAddr)
{
    ST_TEE_HEADER* pstTeeHd=NULL;
    UBOOT_TRACE("IN\n");

    pstTeeHd=(ST_TEE_HEADER*)u32NuttxVirtAddr;

    UBOOT_TRACE("OK\n");
    return (MS_PHYADDR)pstTeeHd->_sthd.uihw_aes_paddr;
}

U32 get_hw_aes_len_from_tee_hd(U32 u32NuttxVirtAddr)
{
    ST_TEE_HEADER* pstTeeHd=NULL;
    UBOOT_TRACE("IN\n");
    pstTeeHd=(ST_TEE_HEADER*)u32NuttxVirtAddr;

    UBOOT_TRACE("OK\n");

    return pstTeeHd->_sthd.uihw_aes_buffer_len;
}

int do_set_tee_debug_level (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    int ret=0;
    char buffer[CMD_BUF]={0};
    if(argc!=2)
    {
        cmd_usage(cmdtp);
        return -1;
    }
    UBOOT_TRACE("IN\n");

    snprintf((char *)buffer,CMD_BUF,"setenv %s %s",ENV_TEE_DEBUG_LEVEL,argv[1]);
    UBOOT_DEBUG("%s\n",buffer);
    ret=run_command(buffer,0);
    if(ret==-1)
    {
        UBOOT_ERROR("set %s to env fail.\n",ENV_TEE_DEBUG_LEVEL);
        return -1;
    }

    snprintf((char *)buffer,CMD_BUF,"saveenv");
    UBOOT_DEBUG("%s\n",buffer);
    ret=run_command(buffer,0);
    if(ret==-1)
    {
        UBOOT_ERROR("save %s to env fail\n",ENV_TEE_DEBUG_LEVEL);
        return -1;
    }

    UBOOT_TRACE("OK\n");
    return 0;
}

void _set_tee_debug_mode(ST_TEE_BOOT_CONFIG *pstTeeBootConfig)
{
    EN_TEE_DEBUG_LEVEL dbgLevel=(EN_TEE_DEBUG_LEVEL_ERROR+EN_TEE_DEBUG_LEVEL_INFO);
    char *pEnv=NULL;
    UBOOT_TRACE("IN\n");

    pEnv=getenv(ENV_TEE_DEBUG_LEVEL);
    if(pEnv==NULL)
    {
        dbgLevel=EN_TEE_DEBUG_LEVEL_INFO;
        UBOOT_DEBUG("tee debug leve: DEFAULT\n");
    }
    else if(strcmp(pEnv,"ERROR")==0)
    {
        dbgLevel=EN_TEE_DEBUG_LEVEL_ERROR;
        UBOOT_DEBUG("tee debug leve: ERROR\n");
    }
    else if(strcmp(pEnv,"INFO")==0)
    {
        //dbgLevel=(EN_TEE_DEBUG_LEVEL_INFO+EN_TEE_DEBUG_LEVEL_ERROR);
        dbgLevel=EN_TEE_DEBUG_LEVEL_INFO;
        UBOOT_DEBUG("tee debug leve: INFO\n");
    }
    else if(strcmp(pEnv,"TRACE")==0)
    {
        //dbgLevel=(EN_TEE_DEBUG_LEVEL_INFO+EN_TEE_DEBUG_LEVEL_ERROR+EN_TEE_DEBUG_LEVEL_TRACE);
        dbgLevel=EN_TEE_DEBUG_LEVEL_TRACE;
        UBOOT_DEBUG("tee debug leve: TRACE\n");
    }
    else if(strcmp(pEnv,"DEBUG")==0)
    {
        //dbgLevel=(EN_TEE_DEBUG_LEVEL_INFO+EN_TEE_DEBUG_LEVEL_ERROR+EN_TEE_DEBUG_LEVEL_TRACE+EN_TEE_DEBUG_LEVEL_DEBUG);
        dbgLevel=EN_TEE_DEBUG_LEVEL_DEBUG;
        UBOOT_DEBUG("tee debug leve: DEBUG\n");
    }
    else if(strcmp(pEnv,"DEBUG_L2")==0)
    {
        //dbgLevel=(EN_TEE_DEBUG_LEVEL_INFO+EN_TEE_DEBUG_LEVEL_ERROR+EN_TEE_DEBUG_LEVEL_TRACE+EN_TEE_DEBUG_LEVEL_DEBUG+EN_TEE_DEBUG_LEVEL_DEBUG_L2);
        dbgLevel=EN_TEE_DEBUG_LEVEL_DEBUG_L2;
        UBOOT_DEBUG("tee debug leve: DEBUG_L2\n");
    }
    else
    {
        dbgLevel=EN_TEE_DEBUG_LEVEL_INFO;
        UBOOT_DEBUG("tee debug leve: DEFAULT\n");
    }

    pstTeeBootConfig->enDebugLevel=dbgLevel;
    UBOOT_TRACE("OK\n");
}

void _set_tee_boot_mode(ST_TEE_BOOT_CONFIG *pstTeeBootConfig)
{
    pstTeeBootConfig->u8FastBootMode=1;
}

void set_tee_boot_config(U32 u32BootConfigAddr)
{
  ST_TEE_BOOT_CONFIG stTeeBootConfig;

  memset(&stTeeBootConfig,0,sizeof(stTeeBootConfig));

  strncpy((void *)stTeeBootConfig.u8BootConfigID,TEE_BOOT_CONFIG_ID,strlen(TEE_BOOT_CONFIG_ID)+1);

  _set_tee_debug_mode(&stTeeBootConfig);

  _set_tee_boot_mode(&stTeeBootConfig);

  memcpy((void *)u32BootConfigAddr,&stTeeBootConfig,sizeof(ST_TEE_BOOT_CONFIG));
  flush_cache(u32BootConfigAddr,(U32)sizeof(ST_TEE_BOOT_CONFIG));

  UBOOT_DEBUG("Dump boot config\n");
  UBOOT_DUMP(u32BootConfigAddr,sizeof(ST_TEE_BOOT_CONFIG));
}

int supportTeeBootConfig(U32 u32NuttxVirtAddr)
{
    ST_TEE_HEADER* pstTeeHd=NULL;
    UBOOT_TRACE("IN\n");

    UBOOT_DEBUG("u32NuttxVirtAddr : %x \n",u32NuttxVirtAddr);
    UBOOT_DUMP(u32NuttxVirtAddr, sizeof(ST_TEE_HEADER));
    pstTeeHd=(ST_TEE_HEADER*)u32NuttxVirtAddr;

    if(pstTeeHd->bootConfigFlag[0]==0x11)
    {
        UBOOT_DEBUG("The tee.bin support the boot config function \n");
        UBOOT_TRACE("OK\n");
          return 0;
    }
    else
    {
         UBOOT_ERROR("The tee.bin doesn't support the boot config function\n");
         return -1;
    }
}
int show_tee_version(U32 u32NuttxVirtAddr)
{
    ST_TEE_HEADER* pstTeeHd=NULL;
    U8 v0[3]={0};
    U8 v1[3]={0};
    U8 v2[3]={0};
    U8 v3[3]={0};
    UBOOT_TRACE("IN\n");

    UBOOT_DEBUG("u32NuttxVirtAddr : %x \n",u32NuttxVirtAddr);
    UBOOT_DUMP(u32NuttxVirtAddr, sizeof(ST_TEE_HEADER));
    pstTeeHd=(ST_TEE_HEADER*)u32NuttxVirtAddr;
    memcpy(v0,&(((char *)&pstTeeHd->_sthd.ullTeeBinVersion)[0]),2);
    memcpy(v1,&(((char *)&pstTeeHd->_sthd.ullTeeBinVersion)[2]),2);
    memcpy(v2,&(((char *)&pstTeeHd->_sthd.ullTeeBinVersion)[4]),2);
    memcpy(v3,&(((char *)&pstTeeHd->_sthd.ullTeeBinVersion)[6]),2);
    UBOOT_DEBUG("\033[0;33mThe version of tee.bin is %s:%s:%s:%s\n\033[0m",v0,v1,v2,v3);
    UBOOT_TRACE("OK\n");
    return 0;
}


int support_tee_1g1g_miu_interval(U32 u32NuttxVirtAddr)
{
    ST_TEE_HEADER* pstTeeHd=NULL;
    UBOOT_TRACE("IN\n");

    UBOOT_DEBUG("u32NuttxVirtAddr : %x \n",u32NuttxVirtAddr);
    UBOOT_DUMP(u32NuttxVirtAddr, sizeof(ST_TEE_HEADER));
    pstTeeHd=(ST_TEE_HEADER*)u32NuttxVirtAddr;

    if((pstTeeHd->one_g_miu_interval[0]==0x1)&&(R2_CACHE_ADDR==0))
    {
        UBOOT_DEBUG("The tee.bin supports 1G1G Miu Interval \n");
        UBOOT_TRACE("OK\n");
        return 0;
    }
    else if((pstTeeHd->one_g_miu_interval[0]==0xff)&&(R2_CACHE_ADDR==0x40000000))
    {
        UBOOT_DEBUG(" The tee.bin does ''NOT'' support 1G1G Miu Interval \n");
        UBOOT_TRACE("OK\n");
        return 0;
    }
    else
    {
        UBOOT_ERROR("Please check the version of tee.bin and mboot \n");
        return -1;
    }
}

#if !defined(CONFIG_R2TEE_TEELOADER)
int do_readNuttx (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    int ret = 0;

    UBOOT_TRACE("IN\n");
    UBOOT_DEBUG("TOTAL_MALLOC_LEN=%x \n",TOTAL_MALLOC_LEN);

    if(GetTEEImageSize(GetSecureMergeType()) == -1)
    {
        UBOOT_ERROR("get TEE image size error\n");
        return -1;
    }
    UBOOT_DEBUG("NuttxAlignSize =%x \n",NuttxAlignSize);
    if(NuttxAlignSize!=0)
    {
        pNuttxOriMallocAddr=AllocateNuttxbuffer(NuttxAlignSize+16);
        if(pNuttxOriMallocAddr==NULL)
        {
            UBOOT_ERROR("malloc fail\n");
            return -1;
        }
        pNuttxAlignMallocAddr=(char*)((((U32)(((U32)pNuttxOriMallocAddr)+16))>>4)<<4);
        UBOOT_DEBUG("pNuttxAlignMallocAddr =%x \n",(U32)pNuttxAlignMallocAddr);
        if(GetSecureImage((unsigned int)pNuttxAlignMallocAddr, TEE_PARTITION, 0, NuttxAlignSize, E_SECUREIMAGE) == -1)
        {
            free(pNuttxOriMallocAddr);
            return -1;
        }
    }
    else if (GetSecureMergeType() == E_SECUREMERGE_NONE)
    {
        #define TEE_PART_SIZE 0x600000
        NuttxAlignSize=TEE_PART_SIZE;
        pNuttxAlignMallocAddr=AllocateNuttxbuffer(TEE_PART_SIZE);
        if(pNuttxAlignMallocAddr==NULL)
        {
            UBOOT_ERROR("malloc fail\n");
            return -1;
        }
        UBOOT_DEBUG("pNuttxAlignMallocAddr =%x \n",(U32)pNuttxAlignMallocAddr);
        if(GetSecureImage((unsigned int)pNuttxAlignMallocAddr, TEE_PARTITION, 0, NuttxAlignSize, E_SECUREIMAGE) == -1)
        {
            free(pNuttxAlignMallocAddr);
            return -1;
        }
    }
    else
    {
        UBOOT_ERROR("Cannot get NuttxAlignSize from tee partition\n");
        return -1;
    }

    UBOOT_TRACE("OK\n");
    return ret;
}
#endif

#if defined(CONFIG_MSTAR_MOBF_ENABLE) || defined(CONFIG_DRAM_MOBF)
#include <drvSEAL.h>
typedef struct{
    U32 addr;
    U32 length;
} ST_MMAP_GROUP_ITEM;

#define SIG_CFG_HEADER_LEN 256

MS_BOOL DoesSignatureExist(U32 SignBaseAddr)
{
    U8 ID[]={'S','E','C','U','R','I','T','Y'};
    UBOOT_TRACE("IN\n");
    if(memcmp((void *)SignBaseAddr,ID,sizeof(ID))!=0)
    {
        UBOOT_DEBUG("Signature doesn't exist\n");
        UBOOT_TRACE("OK\n");
        return FALSE;
    }
    else
    {
        UBOOT_DEBUG("Signature exist\n");
        UBOOT_TRACE("OK\n");
        return TRUE;
    }
}

void _set_SeucreRange(U32 u32NConfigAddr)
{
    int i=0;
    U8  MIUID[2]="\0";
    int MIUIndex=0;
    U8* pTmpPtr=NULL;
    ST_HEADER_INFO* pstHeaderInfo=NULL;
    ST_MMAP_GROUP_ITEM* pstMmapGroupInfo=NULL;
    U32 u32NconfigOffset=0;
    U32 u32MmapOffset=0;
    U32 u32MmapGroupCount=0;
    U32 u32MiuProtectAdr =0;
    U32 u32MiuProtectLen =0;

    pTmpPtr=(U8*)u32NConfigAddr;
    if(DoesSignatureExist(u32NConfigAddr+SIG_CFG_HEADER_LEN)==TRUE)
    {
        //get nuttx_config offset
        pstHeaderInfo=(ST_HEADER_INFO*)u32NConfigAddr;
        u32NconfigOffset=pstHeaderInfo[1].Offset;
        UBOOT_DEBUG("u32NconfigOffset=0x%x\n",u32NconfigOffset);
        pTmpPtr+=u32NconfigOffset;//0x320

    }
    //get mmap info offset
    u32MmapOffset = ((ST_HEADER_INFO*)pTmpPtr)->Offset;
    UBOOT_DEBUG("u32MmapOffset=0x%x\n",u32MmapOffset);

    //get MmapGroupCount & mmap Groupinfo start address
    pTmpPtr+=u32MmapOffset;//0x100
    u32MmapGroupCount=(U32)(*((U32*)pTmpPtr));
    UBOOT_DEBUG("u32MmapGrountCount: 0x%x\n",u32MmapGroupCount);
    pTmpPtr+=8;//8byte for mmapgrounp info & mmap info


    //start to set secure range
    MDrv_SEAL_Init();
    pstMmapGroupInfo=(ST_MMAP_GROUP_ITEM*)pTmpPtr;
    for(i=0;i <(u32MmapGroupCount-1);i++)
    {
        if(pstMmapGroupInfo[i].length!=0)
        {
            if(i==0) // i == 0 is for Nuttx
            {
                MIUIndex = (pstMmapGroupInfo[i].addr >= CONFIG_SYS_MIU_INTERVAL)?1:0;
                UBOOT_DEBUG("pstMmapGroupInfo[%d].addr   = 0x%x ; miu(%d)\n",i,pstMmapGroupInfo[i].addr,MIUIndex);
                UBOOT_DEBUG("pstMmapGroupInfo[%d].length = 0x%x\n",i,(pstMmapGroupInfo[i].length-0x1000)); // 0x1000 is for boot config & handshake
                MDrv_Seal_SecureRangeSet(MIUID[MIUIndex]++,pstMmapGroupInfo[i].addr,(pstMmapGroupInfo[i].addr+pstMmapGroupInfo[i].length -0x1000),0xF);//0xF is all R/W
            }
            else if(i==2) // i == 2 is for VDEC
            {
                //E_MMAP_ID_VDEC_CPU
                if (get_map_addr_from_env(E_VDEC_CPU, NO_DEFAULT_MMAP_VALUE, &u32MiuProtectAdr) == 0)
                {
                    if (get_map_size_from_env(E_VDEC_CPU, NO_DEFAULT_MMAP_VALUE, &u32MiuProtectLen) == 0)
                    {
                        MIUIndex = (u32MiuProtectAdr >= CONFIG_SYS_MIU_INTERVAL)?1:0;
                        u32MiuProtectLen=simple_strtoul(pTmpPtr,NULL,16);
                        UBOOT_DEBUG("u32MiuProtectAdr  = 0x%x ; miu(%d)\n",u32MiuProtectAdr,MIUIndex);
                        UBOOT_DEBUG("pstMmapGroupInfo = 0x%x\n",u32MiuProtectLen);
                        MDrv_Seal_SecureRangeSet(MIUID[MIUIndex]++,u32MiuProtectAdr+0x1000,u32MiuProtectAdr+u32MiuProtectLen,0xF);
                    }
                }
                else
                {
                    UBOOT_ERROR("get E_VDEC_CPU mmap fail\n" );
                }

                //E_MMAP_ID_VDEC_BITSTREAM
                 if (get_map_addr_from_env(E_VDEC_BITSTREAM, NO_DEFAULT_MMAP_VALUE, &u32MiuProtectAdr) == 0)
                {
                    if (get_map_size_from_env(E_VDEC_BITSTREAM, NO_DEFAULT_MMAP_VALUE, &u32MiuProtectLen) == 0)
                    {
                        MIUIndex = (u32MiuProtectAdr >= CONFIG_SYS_MIU_INTERVAL)?1:0;
                        u32MiuProtectLen=simple_strtoul(pTmpPtr,NULL,16);
                        UBOOT_DEBUG("u32MiuProtectAdr  = 0x%x ; miu(%d)\n",u32MiuProtectAdr,MIUIndex);
                        UBOOT_DEBUG("pstMmapGroupInfo = 0x%x\n",u32MiuProtectLen);

                        MDrv_Seal_SecureRangeSet(MIUID[MIUIndex]++,u32MiuProtectAdr+0xA000,u32MiuProtectAdr+u32MiuProtectLen,0xF);
                    }
                }
                else
                {
                    UBOOT_ERROR("get E_VDEC_BITSTREAM mmap fail\n" );
                }
                //E_MMAP_ID_VDEC_SUB_BITSTREAM
                 if (get_map_addr_from_env(E_VDEC_SUB_BITSTREAM, NO_DEFAULT_MMAP_VALUE, &u32MiuProtectAdr) == 0)
                {
                    if (get_map_size_from_env(E_VDEC_SUB_BITSTREAM, NO_DEFAULT_MMAP_VALUE, &u32MiuProtectLen) == 0)
                    {
                        MIUIndex = (u32MiuProtectAdr >= CONFIG_SYS_MIU_INTERVAL)?1:0;
                        u32MiuProtectLen=simple_strtoul(pTmpPtr,NULL,16);
                        UBOOT_DEBUG("u32MiuProtectAdr  = 0x%x ; miu(%d)\n",u32MiuProtectAdr,MIUIndex);
                        UBOOT_DEBUG("pstMmapGroupInfo = 0x%x\n",u32MiuProtectLen);

                        MDrv_Seal_SecureRangeSet(MIUID[MIUIndex]++,u32MiuProtectAdr+0xA000,u32MiuProtectAdr+u32MiuProtectLen,0xF);
                    }
                }
                else
                {
                    UBOOT_ERROR("get E_VDEC_SUB_BITSTREAM mmap fail\n" );
                }
            }
            else
            {
                MIUIndex = (pstMmapGroupInfo[i].addr >= CONFIG_SYS_MIU_INTERVAL)?1:0;
                UBOOT_DEBUG("pstMmapGroupInfo[%d].addr   = 0x%x ; miu(%d)\n",i,pstMmapGroupInfo[i].addr,MIUIndex);
                UBOOT_DEBUG("pstMmapGroupInfo[%d].length = 0x%x\n",i,pstMmapGroupInfo[i].length);
                MDrv_Seal_SecureRangeSet(MIUID[MIUIndex]++,pstMmapGroupInfo[i].addr,pstMmapGroupInfo[i].addr+pstMmapGroupInfo[i].length,0xF);//0xF is all R/W
            }
        }
    }
}

#endif


void SetNuttxAddrToDummyReg(void)
{
    #define DummyReg1   (0x103300+(0x60<<1))
    #define DummyReg2   (0x103300+(0x61<<1))
    #define DummyReg3   (0x103300+(0x62<<1))

    char *p_str;
    MS_U32 nuttx_start = 0;
    MS_U32 nuttx_len = 0;
    UBOOT_TRACE("IN\n");

    if( !u8DynamicLoadNuttxMode )
    {
        Write2Byte(DummyReg1, 0);
        Write2Byte(DummyReg2, 0);
        Write2Byte(DummyReg3, 0);
        UBOOT_TRACE("OK\n");
        return;
    }

    // set R2 start address and len to DummyReg
    if (get_map_addr_from_env(E_NUTTX_MEM, NO_DEFAULT_MMAP_VALUE, &nuttx_start) == 0)
    {

        MS_U16 nuttx_start_H = 0;
        nuttx_start_H = (MS_U16)(nuttx_start >> 16);
        Write2Byte(DummyReg1, nuttx_start_H);
        //MS_U16 tmp = 0;
        //tmp = Read2Byte(DummyReg1);
        //UBOOT_DEBUG("DummyReg1(0x%08x)=0x%x\n",DummyReg1,tmp);
    }
    if (get_map_size_from_env(E_NUTTX_MEM, NO_DEFAULT_MMAP_VALUE, &nuttx_len) == 0)
    {
        MS_U16 nuttx_len_H = 0;
        nuttx_len_H = (MS_U16)(nuttx_len >> 16);
        Write2Byte(DummyReg2, nuttx_len_H);
        //MS_U16 tmp = 0;
        //tmp = Read2Byte(DummyReg2);
        //UBOOT_DEBUG("DummyReg2(0x%08x)=0x%x\n",DummyReg2,tmp);
    }

    Write2Byte(DummyReg3, TEE_DYNAMIC_LOAD_MAGIC_NUM);

    UBOOT_TRACE("OK\n");
}

#if 0 // ian test sample code
int do_bootNuttx (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    MS_PHYADDR NuttxPhyAddr = 0x48008000;
    if(argc==2)
       NuttxPhyAddr = simple_strtoul(argv[1], NULL, 16);
    UBOOT_DEBUG("[ARM TRUSTZONE Nuttx] boot to 0x%lx\n",NuttxPhyAddr);
    *((volatile unsigned int *)(CONFIG_KERNEL_START_ADDRESS + 0x8000 - 0x4)) = NuttxPhyAddr; //Nuttx entry address
    *((volatile unsigned int *)(CONFIG_KERNEL_START_ADDRESS + 0x8000 - 0x8)) = 0xfac1; //call cpu1

    flush_cache((CONFIG_KERNEL_START_ADDRESS + 0x8000 - 0x8),32);
    return 0;
}
#else
#if !defined(CONFIG_R2TEE_TEELOADER)
int do_bootNuttx (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    //I marked 'nand erad' behavior here, and add this behavior in script '[[nuttx'
    //ret = run_command ("nand read.e 0x58000000 NUTTX 0x200000", 0 );
    int ret = 0;
    MS_PHYADDR NuttxPhyAddr = 0;
    U32 u32NuttxVirtAddr = 0;
    MS_PHYADDR HwAesPABufAddr=0;
    U32 u32HwAesBufSize=0;
    U32 u32CustomerKeyBankAddr=0;
    U32 u32NConfigAddr=0;
    U32 u32NConfigSize=0;

    UBOOT_TRACE("IN\n");

    if(argc==2)
    {
        UBOOT_DEBUG("physical working buffer is from external parameter \n");
        NuttxPhyAddr  = (MS_PHYADDR)simple_strtoull(argv[1], NULL, 16);
        u32NuttxVirtAddr = _PA2VA(NuttxPhyAddr);
    }
    else
    {
        u32NuttxVirtAddr = (U32)pNuttxAlignMallocAddr;
    }

#if !defined (CONFIG_SECURE_BY_PASS_VERIFICATION)
    if(-1==RawData_Authentication(TEE_PARTITION,u32NuttxVirtAddr))
    {
        UBOOT_ERROR("TEE Authentication fail\n");
        return -1;
    }
#endif

    UBOOT_DEBUG("u32NuttxVirtAddr = 0x%x \n",u32NuttxVirtAddr);

	  HwAesPABufAddr=get_hw_aes_addr_from_tee_hd(u32NuttxVirtAddr);
    u32HwAesBufSize=get_hw_aes_len_from_tee_hd(u32NuttxVirtAddr);
    if((HwAesPABufAddr==0)||(HwAesPABufAddr==0xffffffff)||(u32HwAesBufSize==0)||(u32HwAesBufSize==0xffffffff))
    {
        UBOOT_DEBUG("tee header doesn't have hw aes info, using hw aes info in mmap\n");
        if (get_map_addr_from_env(E_HW_AES_BUF, NO_DEFAULT_MMAP_VALUE, &HwAesPABufAddr) != 0)
        {
            UBOOT_ERROR("get %s mmap fail\n",E_MMAP_ID_HW_AES_BUF_ADR);
            return -1;
        }
        if (get_map_size_from_env(E_HW_AES_BUF, NO_DEFAULT_MMAP_VALUE, &u32HwAesBufSize) != 0)
        {
            UBOOT_ERROR("get %s mmap fail\n",E_MMAP_ID_HW_AES_BUF_LEN);
            return -1;
        }
    }

    u32CustomerKeyBankAddr=_PA2NVA(HwAesPABufAddr);
    u32NConfigAddr=u32CustomerKeyBankAddr+sizeof(CUSTOMER_KEY_BANK);
    gu32TeeBootConfigAddr=_PA2NVA(HwAesPABufAddr)-NUTTX_BOOT_CONFIG_SIZE;
    gu32TeeHandShakeAddr=gu32TeeBootConfigAddr+sizeof(ST_TEE_BOOT_CONFIG);

    get_nconfig_size(&u32NConfigSize);

    UBOOT_DEBUG("HwAesPABufAddr=%llx \n",HwAesPABufAddr);
    UBOOT_DEBUG("u32HwAesBufSize=%x \n",u32HwAesBufSize);
    UBOOT_DEBUG("u32NConfigSize =%x \n",u32NConfigSize);


    UBOOT_DEBUG("u32CustomerKeyBankAddr=%x \n",u32CustomerKeyBankAddr);
    UBOOT_DEBUG("sizeof(CUSTOMER_KEY_BANK)=%x \n",sizeof(CUSTOMER_KEY_BANK));
    if((sizeof(CUSTOMER_KEY_BANK)+u32NConfigSize)> u32HwAesBufSize )
    {
        UBOOT_ERROR("Data is too big\n");
        UBOOT_ERROR("sizeof(CUSTOMER_KEY_BANK) : %x , u32NConfigSize : %x \n",sizeof(CUSTOMER_KEY_BANK),u32NConfigSize);
        UBOOT_ERROR("u32HwAesBufSize %x \n",u32HwAesBufSize);
        DeAllocateNuttxbuffer(pNuttxOriMallocAddr);
        return -1;
    }


    #if !defined (CONFIG_NUTTX_BRINGUP_TEST)
    UBOOT_DUMP(_PA2NVA(_VA2PA(u32NuttxVirtAddr)), 0x100);
    #else
    UBOOT_DUMP(u32NuttxVirtAddr, 0x100);
    #endif

    #if !defined (CONFIG_NUTTX_BRINGUP_TEST)
    LoadNConfig(u32NConfigAddr);

    #if defined(CONFIG_MSTAR_MOBF_ENABLE) || defined(CONFIG_DRAM_MOBF)
    //for MOBF patch
    U8* pu8NConfigAddrTmp=NULL;
    pu8NConfigAddrTmp=malloc(u32NConfigSize);
    // Move Nuttx config to another ram space.
    // after set Secure Range, copy back.
    memcpy(pu8NConfigAddrTmp,u32NConfigAddr,u32NConfigSize);
    _set_SeucreRange(pu8NConfigAddrTmp);
    memcpy(u32NConfigAddr,pu8NConfigAddrTmp,u32NConfigSize);
    #endif

    NuttxPhyAddr=MoveNuttx2Dest(u32NuttxVirtAddr, NuttxAlignSize);
    #endif

    if(NuttxPhyAddr==0)
    {
        UBOOT_ERROR("MoveNuttx2Dest fail\n");
        DeAllocateNuttxbuffer(pNuttxOriMallocAddr);
        return -1;
    }

    #if !defined (CONFIG_NUTTX_BRINGUP_TEST)
    if(-1==supportTeeBootConfig(u32NuttxVirtAddr))
    {
            jump_to_console();
            return -1;
    }

    if(-1==support_tee_1g1g_miu_interval(u32NuttxVirtAddr))
    {
            jump_to_console();
            return -1;
    }

    show_tee_version(u32NuttxVirtAddr);

    if(-1==CopyCustomerKeyBank(u32CustomerKeyBankAddr))
    {
        UBOOT_ERROR("Load NConfig fail\n");
        DeAllocateNuttxbuffer(pNuttxOriMallocAddr);
        return -1;
    }
    UBOOT_DUMP(u32CustomerKeyBankAddr, 0x100);
    UBOOT_DUMP(u32NConfigAddr, 0x100);

    set_tee_boot_config(gu32TeeBootConfigAddr);
    #endif

    // set R2 start address and end address
    SetNuttxAddrToDummyReg();

    #if defined (CONFIG_NUTTX_BRINGUP_TEST)
    MApi_R2_StartFrDRAM((MS_PHYADDR)simple_strtoull(argv[1], NULL, 16));
    #else
    #if defined(CONFIG_AMP_TRUSTZONE_ENABLE)
    printf("[ARM TRUSTZONE Nuttx]\n");

    U32 u32NuttxBusAddr = _PA2VA(NuttxPhyAddr);

    UBOOT_DEBUG("[ARM TRUSTZONE Nuttx] boot NuttxPhyAddr 0x%llx\n",NuttxPhyAddr);
    UBOOT_DEBUG("[ARM TRUSTZONE Nuttx] boot to u32NuttxBusAddr : 0x%x\n",u32NuttxBusAddr);

    *((volatile unsigned int *)(CONFIG_KERNEL_START_ADDRESS + 0x8000 - 0x4)) = u32NuttxBusAddr; //Nuttx entry address
    *((volatile unsigned int *)(CONFIG_KERNEL_START_ADDRESS + 0x8000 - 0x8)) = 0xfac1; //call cpu1

    flush_cache((CONFIG_KERNEL_START_ADDRESS + 0x8000 - 0x8),32);

    #else
    MApi_R2_StartFrDRAM(NuttxPhyAddr);
    #endif
    #endif

    DeAllocateNuttxbuffer(pNuttxOriMallocAddr);

    UBOOT_TRACE("OK\n");
    return ret;
}
#endif
#endif

#if !defined(CONFIG_R2TEE_TEELOADER)
int do_wait_tee_ready(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    UBOOT_TRACE("IN\n");
    UBOOT_DEBUG("Wait for secure memory done\n");
    if(-1==IsSecureMemDone(gu32TeeHandShakeAddr))
    {
        UBOOT_ERROR("SecureOS is dead\n");
        UBOOT_ERROR("SecureOS is dead\n");
#if defined (CONFIG_R2_BRINGUP)
        MApi_Disable_R2();
#else
        while(1);
#endif
    }
    else
    {
        UBOOT_DEBUG("secure memory setting is done\n");
        SetHKtoGo(gu32TeeHandShakeAddr);
    }
    UBOOT_TRACE("OK\n");
    return 0;
}
#endif

#if !defined(CONFIG_R2TEE_TEELOADER)
#ifdef CONFIG_MSTAR_R2_STR
static int IsTEE_Resume_Done(U32 address)
{
    U32 timeOut = 0;
    U32 count = 0;
    timeOut = 600;
    while (1)
    {
        if ( *((U32*)address) == STR_FLAG_RESUME_FINISH)
        {
            printf("TEE Resume OK\n");
            return 0;
        }
        mdelay(1);
        count++;
        if (count == timeOut)
        {
            break;
        }
    }
    printf("TEE Resume Fail [0x%x] \n",*((U32*)address));
    return -1;
}

int do_r2_resume(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    UBOOT_TRACE("IN\n");
    MS_PHYADDR NuttxStart = 0;
    MS_PHYADDR HandShakeAddr=0;
    MS_PHYADDR AesAddr=0;
    MS_PHYADDR NuttxAddr=0;
    U32 u32TEEStoreSize = TEESTORESIZE;
    U32 u32BootConfigSize = NUTTX_BOOT_CONFIG_SIZE;
    MS_BOOL isR2Resume=FALSE;

    if (get_map_addr_from_env(E_NUTTX_STR_STATUS, NO_DEFAULT_MMAP_VALUE, &HandShakeAddr) != 0)
    {
            UBOOT_ERROR("get E_MMAP_ID_NUTTX_STR_STATUS_ADR mmap fail\n");
            return -1;
    }
    if (get_map_addr_from_env(E_NUTTX_MEM, NO_DEFAULT_MMAP_VALUE, &NuttxAddr) != 0)
    {
            UBOOT_ERROR("get E_MMAP_ID_NUTTX_MEM_ADR mmap fail\n");
            return -1;
    }
    HandShakeAddr=MsOS_PA2KSEG1(HandShakeAddr);
    //HandShakeAddr=AesAddr-u32TEEStoreSize-u32BootConfigSize;
    //HandShakeAddr=PATCH_STR_HANDSHAKER_ADDR; //need fix this address
    printf("Restart R2 TEE u32HandShakeAddr =0x%x\n",HandShakeAddr);
    if(*((U32*)HandShakeAddr) == 0xFFFF8888)
    {
        *((U32*)HandShakeAddr)= 0x5A5A5A5A;
        isR2Resume=TRUE;
        UBOOT_DEBUG("Restart R2 TEE\n");
    }

    MApi_R2_StartFrDRAM(NuttxAddr);
    if(isR2Resume==TRUE)
    {
        UBOOT_DEBUG("Wait forTEE Resume done\n");
        if(-1==IsTEE_Resume_Done(HandShakeAddr))
        {
            UBOOT_ERROR("R2 TEE Resume is fail !!\n");
            UBOOT_ERROR("R2 TEE Resume is fail !!\n");
            jump_to_console();
        }
        else
        {
            UBOOT_DEBUG("secure memory setting is done\n");
            SetHKtoGo(gu32TeeHandShakeAddr);
        }
    }
    else
    {
        UBOOT_ERROR("R2 TEE Resume is fail !!\n");
        MsSystemReset();
    }
    UBOOT_TRACE("OK\n");
    return 0;
}
#endif
#endif

#if !defined(CONFIG_R2TEE_TEELOADER)
int do_load_nconfig(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    MS_PHYADDR HwAesPABufAddr=0;
    U32 u32HwAesVABufAddr=0;
    U32 u32HwAesBufLen=0;
    U32 u32NConfigAddr=0;
    if(argc!=1){
         cmd_usage(cmdtp);
         return -1;
     }

    UBOOT_TRACE("IN\n");

    if (get_map_addr_from_env(E_HW_AES_BUF, NO_DEFAULT_MMAP_VALUE, &HwAesPABufAddr) != 0)
    {
        UBOOT_ERROR("get %s mmap fail\n",E_MMAP_ID_HW_AES_BUF_ADR);
        return -1;
    }
    if (get_map_size_from_env(E_HW_AES_BUF, NO_DEFAULT_MMAP_VALUE, &u32HwAesBufLen) != 0)
    {
        UBOOT_ERROR("get %s mmap fail\n",E_MMAP_ID_HW_AES_BUF_LEN);
        return -1;
    }

    u32HwAesVABufAddr=_PA2NVA(HwAesPABufAddr);
    u32NConfigAddr=u32HwAesVABufAddr+sizeof(CUSTOMER_KEY_BANK);

    UBOOT_DEBUG("HwAesPABufAddr=%llx \n",HwAesPABufAddr);
    UBOOT_DEBUG("u32HwAesBufLen=%x \n",u32HwAesBufLen);
    UBOOT_DEBUG("u32HwAesVABufAddr=%x \n",u32HwAesVABufAddr);
    UBOOT_DEBUG("u32NConfigAddr=%x \n",u32NConfigAddr);

    if(NConfigLoad(u32NConfigAddr)!=0)
    {
        UBOOT_ERROR("Load NConfig fail\n");
        return -1;
    }

    UBOOT_TRACE("OK\n");
    return 0;

}
#endif

int do_save_nconfig(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    #define OBJECT_NAME argv[1]
    #define OBJECT_DRAM_ADDR argv[2]
    int ret=0;

    if(argc<3){
         cmd_usage(cmdtp);
         return -1;
     }

    if(strcmp(OBJECT_NAME,"NuttxConfig")!=0){
        return -1;
    }

    ST_SIGN_CONFIG* pstHeaderInfo = NULL;
    U32 u32SignConfigPhyAddr=simple_strtoul(OBJECT_DRAM_ADDR, NULL, 16);;
    U32 u32SignConfigLen=0;
    U32 u32SigPhyAddr=0;
    U32 u32ConfigPhyAddr=0;
    U32 u32SigLen=256;
    U32 u32ConfigLen=0;
    UBOOT_DEBUG("OBJECT_NAME : %s \n",OBJECT_NAME);
    UBOOT_DEBUG("u32SignConfigPhyAddr =%x \n",u32SignConfigPhyAddr);

    pstHeaderInfo=(ST_SIGN_CONFIG*)u32SignConfigPhyAddr;
    u32SigPhyAddr=u32SignConfigPhyAddr+pstHeaderInfo->stSignature.Offset;
    u32SigLen=pstHeaderInfo->stSignature.Length;
    u32ConfigPhyAddr=u32SignConfigPhyAddr+pstHeaderInfo->stConfig.Offset;
    u32ConfigLen=pstHeaderInfo->stConfig.Length;
    #define HEADER_LEN 256
    u32SignConfigLen=HEADER_LEN+u32SigLen+u32ConfigLen;

    UBOOT_DEBUG("****u32SignConfigPhyAddr : %x \n",u32SignConfigPhyAddr);
    UBOOT_DEBUG("****u32SigPhyAddr : %x \n",u32SigPhyAddr);
    UBOOT_DEBUG("****u32SigLen : %x \n",u32SigLen);
    UBOOT_DEBUG("****u32ConfigPhyAddr : %x \n",u32ConfigPhyAddr);
    UBOOT_DEBUG("****u32ConfigLen : %x \n",u32ConfigLen);

    ret = NConfigSave((U8*)u32SignConfigPhyAddr, u32SignConfigLen);
    if(EN_SUCCESS != ret){
        UBOOT_ERROR("NConfig Save fail\n");
        return -1;
    }

     UBOOT_TRACE("OK\n");

     return 0;
}
int get_nconfig_size(unsigned int *u32NConfigSize)
{
    int ret = 0;
    UBOOT_DEBUG("IN\n");

    ret = MsApiChunkHeader_GetValue(CH_NUTTX_CONFIG_SIZE,u32NConfigSize);
    if(ret != 0){
        UBOOT_ERROR("MsApiChunkHeader_GetValue fail!\n");
        return -1;
    }
    #if defined (CONFIG_NCONFIG_STORE_IN_NAND_FLASH)
    // nand flash block size range is very different.
    // ex: MLC - 2M, SLC - 128K.
    // In some MLC case, block size is 2M. 2M is larger than hw aes buffer size will cause problem.
    *u32NConfigSize=(*u32NConfigSize)*(0x10000);
    #else
    *u32NConfigSize=(*u32NConfigSize)*((unsigned int)SECTOR_SIZE);
    #endif
    UBOOT_DEBUG("*u32NConfigSize = %x\n",*u32NConfigSize);

    UBOOT_DEBUG("OK\n");
    return ret;
}

int get_nconfig_offset(unsigned int *u32NConfigOffset,unsigned int *u32NConfigBakOffset)
{
    int ret = 0;
    U32 u32Size=0;

    UBOOT_DEBUG("IN\n");

    ret = MsApiChunkHeader_GetValue(CH_NUTTX_CONFIG_OFFSET,u32NConfigOffset);
    if(ret != 0){
        UBOOT_ERROR("MsApiChunkHeader_GetValue fail!\n");
        return -1;
    }
    UBOOT_DEBUG("*u32NConfigOffset = %x\n",*u32NConfigOffset);
    #if defined (CONFIG_NCONFIG_STORE_IN_SPI_FLASH)
    ret = getSpiSize(&u32Size);
    *u32NConfigOffset = u32Size - ((*u32NConfigOffset)*SECTOR_SIZE);
    *u32NConfigBakOffset=*u32NConfigOffset+SECTOR_SIZE;
    UBOOT_DEBUG("u32NConfigOffset : 0x%x\n",*u32NConfigOffset);
    UBOOT_DEBUG("u32NConfigBkOffset : 0x%x\n",*u32NConfigBakOffset);

    #elif defined (CONFIG_NCONFIG_STORE_IN_NAND_FLASH)
    check_ubi_partition(NAND_DEFAULT_VOLUME,NAND_DEFAULT_PARTITION);
    ubi_get_volume_size(NAND_DEFAULT_VOLUME,&u32Size);
    UBOOT_DEBUG("u32Size : 0x%x\n",u32Size);
    UBOOT_DEBUG("ubi_get_leb_size : 0x%x\n",SECTOR_SIZE);
    *u32NConfigOffset = u32Size - ((*u32NConfigOffset)*SECTOR_SIZE);
    *u32NConfigBakOffset=*u32NConfigOffset+SECURE_INFOR_BACK_OFFSET*SECTOR_SIZE;
    UBOOT_DEBUG("u32NConfigOffset : 0x%x\n",*u32NConfigOffset);
    UBOOT_DEBUG("u32NConfigBkOffset : 0x%x\n",*u32NConfigBakOffset);

    #elif defined (CONFIG_NCONFIG_STORE_IN_EMMC_FLASH)
    ret = get_mmc_partsize(FLASH_DEFAULT_PARTITION,&u32Size);
    *u32NConfigOffset = u32Size - ((*u32NConfigOffset)*SECTOR_SIZE);
    *u32NConfigBakOffset=*u32NConfigOffset+SECURE_INFOR_BACK_OFFSET*SECTOR_SIZE;
    UBOOT_DEBUG("u32NConfigOffset : 0x%x\n",*u32NConfigOffset);
    UBOOT_DEBUG("u32NConfigBkOffset : 0x%x\n",*u32NConfigBakOffset);

    #elif defined (CONFIG_NCONFIG_STORE_IN_UFS_FLASH)
    ret = get_ufs_partsize(FLASH_DEFAULT_PARTITION,&u32Size);
    *u32NConfigOffset = u32Size - ((*u32NConfigOffset)*SECTOR_SIZE);
    *u32NConfigBakOffset=*u32NConfigOffset+SECURE_INFOR_BACK_OFFSET*SECTOR_SIZE;
    UBOOT_DEBUG("u32NConfigOffset : 0x%x\n",*u32NConfigOffset);
    UBOOT_DEBUG("u32NConfigBkOffset : 0x%x\n",*u32NConfigBakOffset);
    #else
    #error "error"
    #endif

    if(EN_SUCCESS == ret){
        ret=0;
        UBOOT_DEBUG("OK\n");
    }
    else{
        ret=-1;
        UBOOT_DEBUG("get_NConfig_offset fail\n");
    }

    UBOOT_DEBUG("OK\n");

    return ret;
}

int NConfigSave(U8* pBufferAddr, U32 u32NConfigLen)
{
   int ret=-1;
   int ret_bk =-1;
   unsigned int u32NConfigOffset = 0;
   unsigned int u32NConfigBkOffset = 0;
   unsigned int u32NConfigSize=0;
    UBOOT_DEBUG("IN\n");

   if(pBufferAddr==NULL)
   {
      UBOOT_ERROR("The input parameter pBufferAddr' is a null pointer\n");
      return -1;
   }

   ret = raw_io_config(FLASH_DEFAULT_TARGET,FLASH_DEFAULT_PARTITION,FLASH_DEFAULT_VOLUME);
   if(ret != 0)
   {
       UBOOT_ERROR("raw_io_config setting fail!\n");
       return -1;
   }

   ret = get_nconfig_offset(&u32NConfigOffset,&u32NConfigBkOffset);
   if(ret != 0)
   {
       UBOOT_ERROR("get_NConfig_offset fail!\n");
       return -1;
   }

   UBOOT_DEBUG("u32NConfigOffset : 0x%x\n",u32NConfigOffset);
   UBOOT_DEBUG("u32NConfigBkOffset : 0x%x\n",u32NConfigBkOffset);

   ret = get_nconfig_size(&u32NConfigSize);
   if(ret != 0)
   {
       UBOOT_ERROR("get_nconfig_max_size fail!\n");
       return -1;
   }

   #define NCONFIG_RAW_DATA_SIZE (u32NConfigSize-4)

   UBOOT_DEBUG("u32NConfigLen =%x\n",u32NConfigLen);
   if(u32NConfigLen>=NCONFIG_RAW_DATA_SIZE)
   {
        UBOOT_ERROR("NCofngi length is larger than %x\n",NCONFIG_RAW_DATA_SIZE);
        return -1;
   }

    unsigned long Crc32=0;
    // update CRC
    Crc32=crc32(0, pBufferAddr,NCONFIG_RAW_DATA_SIZE);
    UBOOT_DEBUG("Crc32 =%lx\n",Crc32);
    memcpy(pBufferAddr+NCONFIG_RAW_DATA_SIZE, &Crc32, sizeof(Crc32));
    UBOOT_DEBUG("Burn NConfig to blcok\n");
    ret = raw_write((unsigned int)pBufferAddr,u32NConfigOffset,u32NConfigSize);
    UBOOT_DEBUG("Burn NConfig to  backup blcok\n");
    ret_bk = raw_write((unsigned int)pBufferAddr,u32NConfigBkOffset,u32NConfigSize);

    if(EN_SUCCESS == ret || EN_SUCCESS == ret_bk){
        UBOOT_DEBUG("ret : %d , ret_bk : %d \n",ret,ret_bk);
        UBOOT_TRACE("OK\n");
        ret = 0;
    }
    else{
        ret= -1;
        UBOOT_ERROR("NConfig Save fail\n");
    }

    UBOOT_DEBUG("OK\n");
    return ret;
    #undef NCONFIG_RAW_DATA_SIZE
}

int LoadNConfig(U32 u32NConfigBufAddr)
{
    UBOOT_TRACE("IN\n");
    UBOOT_DEBUG("u32HwAesVABufAddr =%x\n",u32NConfigBufAddr);
    if(NConfigLoad(u32NConfigBufAddr)!=EN_SUCCESS){
        UBOOT_ERROR("Load signature fail\n");
        return -1;
    }

    UBOOT_DEBUG("OK\n");
    return 0;
}

int NConfigLoad(U32 u32NConfigBufAddr)
{
    int ret = -1;
    unsigned int u32NConfigOffset = 0;
    unsigned int u32NConfigBakOffset = 0;
    unsigned int u32NConfigSize=0;

    U32          u32NConfigIndex = 0;
    U32          u32NConfigBankSize;
    char        *pChr;


    UBOOT_DEBUG("IN\n");
    if(GetSecureMergeType() != E_SECUREMERGE_NONE)
    {
        ret = GetSecureImage(u32NConfigBufAddr, TEE_PARTITION, NuttxAlignSize + sizeof(SUB_SECURE_INFO), NUTTX_HEADER_LEN, E_NUTTXCONFIG);
        if(ret != 0){
            UBOOT_ERROR("get nuttxconfig fail!\n");
            return -1;
        }
    }
    else
    {

#define NCONFIG_RAW_DATA_SIZE (u32NConfigSize-4)


        ret = raw_io_config(FLASH_DEFAULT_TARGET,FLASH_DEFAULT_PARTITION,FLASH_DEFAULT_VOLUME);
        if(ret != 0){
            UBOOT_ERROR("raw_io_config setting fail!\n");
            return -1;
        }

        ret = get_nconfig_offset(&u32NConfigOffset,&u32NConfigBakOffset);
        if(ret != 0){
            UBOOT_ERROR("get_signature_offset fail!\n");
            return -1;
        }
        UBOOT_DEBUG("u32NConfigOffset =%x \n",u32NConfigOffset);
        UBOOT_DEBUG("u32NConfigBakOffset =%x \n",u32NConfigBakOffset);


        U8* pBufferAddr= NULL;
        int flag1=0, flag2=0;
        unsigned long Crc32=0;

        get_nconfig_size(&u32NConfigSize);
        pBufferAddr=malloc(u32NConfigSize);
        if(pBufferAddr==NULL){
            return -1;
        }

        UBOOT_DEBUG("u32NConfigSize =%x \n",u32NConfigSize);

        ret = raw_read((unsigned int)pBufferAddr,u32NConfigOffset,u32NConfigSize);
        memcpy(&Crc32, (pBufferAddr+NCONFIG_RAW_DATA_SIZE), sizeof(Crc32));
        UBOOT_DEBUG("Crc32 =%lx \n",Crc32);
        if( (EN_SUCCESS == ret) && (Crc32 == crc32(0, pBufferAddr,NCONFIG_RAW_DATA_SIZE)) ){
            UBOOT_DEBUG("NConfig data is ok \n");
            flag1=1;
        }

        ret = raw_read((unsigned int)pBufferAddr,u32NConfigBakOffset,u32NConfigSize);
        memcpy(&Crc32, (pBufferAddr+NCONFIG_RAW_DATA_SIZE), sizeof(Crc32));
        UBOOT_DEBUG("Crc32 =%lx \n",Crc32);
        if( (EN_SUCCESS == ret) && (Crc32== crc32(0, pBufferAddr,NCONFIG_RAW_DATA_SIZE)) ){
            UBOOT_DEBUG("Backup NConfig data is ok \n");
            flag2=1;
        }

        if( (flag2==0) && (flag1!=0) ){
            ret = raw_read((unsigned int)pBufferAddr,u32NConfigOffset,u32NConfigSize);
            memcpy(&Crc32, (pBufferAddr+NCONFIG_RAW_DATA_SIZE), sizeof(Crc32));
            if( (EN_SUCCESS == ret) && (Crc32 == crc32(0, pBufferAddr,NCONFIG_RAW_DATA_SIZE))){
                UBOOT_DEBUG("Backup NConfig data is fail, re-write backup NConfig \n");
                UBOOT_DEBUG("Crc32 =%lx \n",Crc32);

                ret = raw_write((unsigned int)pBufferAddr,u32NConfigBakOffset,u32NConfigSize);
            }
            else{
                free(pBufferAddr);
                UBOOT_ERROR("raw_read fail or caculate crc fail!\n");
                return -1;
            }
        }

        if((flag1==0)&&(flag2!=0)){
            UBOOT_DEBUG("SigNConfig data is fail, re write backup SigNConfig \n");
            ret = raw_write((unsigned int)pBufferAddr,u32NConfigOffset,sizeof(SECURITY_INFO));
        }

        if(EN_SUCCESS != ret){
            free(pBufferAddr);
            UBOOT_ERROR("Load NConfig fail\n");
            return -1;
        }


        pChr = getenv(ENV_NCONFIG_INDEX);

        if( pChr != NULL )
        {
            u32NConfigIndex = simple_strtoul(pChr, NULL, 16);
            if(u32NConfigIndex == 0)
            {
                UBOOT_ERROR("Invail NConfig index\n");
                return -1;
            }

            UBOOT_DEBUG("Switch Nconfig bank to number %d\n",u32NConfigIndex);

            pChr = getenv(ENV_NCONFIG_SIZE);

            if( pChr != NULL )
                u32NConfigBankSize = simple_strtoul(pChr, NULL, 16);
            else
                u32NConfigBankSize = DEF_NCONFIG_BANK_SIZE;

            UBOOT_DEBUG("Nconfig bank size %d\n",u32NConfigBankSize);

            u32NConfigOffset = u32NConfigOffset + (u32NConfigIndex-1)*u32NConfigBankSize;
            u32NConfigSize = u32NConfigBankSize;
        }

        flush_cache(u32NConfigBufAddr,u32NConfigSize);
        ret = raw_read((unsigned int)u32NConfigBufAddr,u32NConfigOffset,u32NConfigSize);
        if(EN_SUCCESS != ret){
            free(pBufferAddr);
            UBOOT_ERROR("Load NConfig fail\n");
            return -1;
        }

        free(pBufferAddr);
    }
        UBOOT_DEBUG("OK\n");
        return ret;
#undef NCONFIG_RAW_DATA_SIZE
}

int GetTEEImageSize(EN_SECUREMERGE_TYPE secureMergeType)
{
    char* pu8PreloadAddr=NULL;
    int ret = 0;
    UBOOT_TRACE("IN\n");
    if(secureMergeType != E_SECUREMERGE_NONE)
    {
        ret = GetImageSize(TEE_PARTITION, 0, &NuttxAlignSize);
        if (0 != ret)
        {
            UBOOT_ERROR("get tee image size fail\n");
            return -1;
        }
    }
    else
    {
        pu8PreloadAddr=malloc(NUTTX_HEADER_LEN);
        if(pu8PreloadAddr==NULL){
            UBOOT_ERROR("malloc fail\n");
            return -1;
        }
        GetSecureImage((unsigned int)pu8PreloadAddr,TEE_PARTITION, 0, NUTTX_HEADER_LEN, E_SECUREIMAGE);
        UBOOT_DUMP((U32)pu8PreloadAddr, NUTTX_HEADER_LEN);
#if !defined (CONFIG_SECURE_BY_PASS_VERIFICATION)
        ret = DecryptKernelImage((char *)pu8PreloadAddr, NUTTX_HEADER_LEN);
        if(-1 == ret){
            free(pu8PreloadAddr);
            UBOOT_ERROR("\033[0;31m Dercypt FAIL\033[0m\n");
            return -1;
        }
#endif

        UBOOT_DUMP((U32)pu8PreloadAddr, NUTTX_HEADER_LEN);

        NuttxAlignSize=CalculateNuttxAlignSize(pu8PreloadAddr);
    }

    free(pu8PreloadAddr);
    UBOOT_TRACE("OK\n");
    return ret;
}
#endif
