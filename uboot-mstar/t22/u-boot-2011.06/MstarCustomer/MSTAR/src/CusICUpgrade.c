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
#include <common.h>
#include <malloc.h>
#include <MsTypes.h>
#include <MsSystem.h>
#include <MsUtility.h>
#include <MsDebug.h>
#include <MsVfs.h>
#include <MsBoot.h>
#include <MsAppCB.h>
#include <MsOS.h>
#include <apiSWI2C.h>
#include <CusICUpgrade.h>
#include <CusConfig.h>
#include <bootlogo/iniparser.h>
#include "MsEeprom.h"

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define I2C_BUS_NUM         (1)
#define SW_IIC_READ          0
#define SW_IIC_WRITE         1

typedef struct IC_INFO_s
{
    int Bus;
    int Addr;
    int Offset;
    int Size;
}IC_INFO_t;

MS_U8 g_bus = 0;
MS_U8 g_slave_addr = 0x0;
char gPMICBinPath[BUFFER_SIZE];
char gPGAMMAICBinPath[BUFFER_SIZE];

//-------------------------------------------------------------------------------------------------
//  Private Functions
//-------------------------------------------------------------------------------------------------
static SWI2C_BusCfg I2cBusCfg[I2C_BUS_NUM]=
{
    // Bus-0
    {PAD_TGPIO0, PAD_TGPIO1, 100}, //IS_SW_I2C  /SCL_PAD /SDA_PAD /Delay

    // Bus-1
    //{PAD_GPIO28, PAD_GPIO29, 100}, //IS_SW_I2C  /SCL_PAD /SDA_PAD /Delay

    // Others, add other bus here
};

static void SWI2C_WriteDatas(MS_U8 *u8RegOffset, MS_U8 addrSize, MS_U8 *pdatabuf, MS_U32 dataSize)
{
     MS_U8 k;
     MApi_SWI2C_UseBus(g_bus);

    if(MApi_SWI2C_AccessStart(g_slave_addr, SW_IIC_WRITE)==FALSE)
    {
        UBOOT_DEBUG("\n I2cWriteDatas->IIC_AccessStart Error");
    }

    for(k=0;k<addrSize;k++)
    {
        if(MApi_SWI2C_SendByte(u8RegOffset[k])==FALSE)
        {
            UBOOT_DEBUG("\n register error");
        }
    }

    for(k=0;k<dataSize;k++)
    {
        if(MApi_SWI2C_SendByte(pdatabuf[k])==FALSE)
        {
            UBOOT_DEBUG("\n pdatabuf Error");
        }
    }

    MApi_SWI2C_Stop();
}

static int do_pm_ic_upgrade(IC_INFO_t *sPMIC, MS_U8 *pu8Buffer)
{
    UBOOT_INFO("Start upgrade PM IC bin file!~\n");
    MS_U8 u8RegOffset = sPMIC->Offset;
    MS_U32 u32Size = sPMIC->Size-3;

    g_bus = sPMIC->Bus;
    if(sPMIC->Addr != 0)
    {
        g_slave_addr = sPMIC->Addr;
        UBOOT_INFO("g_slave_addr:%x\n",g_slave_addr);
    }
    else
    {
        UBOOT_DEBUG("g_slave_addr is zero!!");
        return -1;
    }

    if(u32Size == 0)
    {
        UBOOT_DEBUG("Write data size is zero!!\n");
        return -1;
    }
    else
    {
        UBOOT_INFO("Write data size:%x\n",u32Size);
    }

    UBOOT_INFO("I2cBusCfg: padSCL=%d, padSDA=%d\n", I2cBusCfg[0].padSCL, I2cBusCfg[0].padSDA);
    MApi_SWI2C_Init(I2cBusCfg, I2C_BUS_NUM);

    SWI2C_WriteDatas(&u8RegOffset,1,pu8Buffer,u32Size);

    UBOOT_TRACE("OK\n");
    return 0;
}

static int do_pm_ic_burn(void)
{
    MS_U8 u8WriteEEPROM = 0x80;
    MS_U8 u8RegOffset = 0xFF;

    UBOOT_INFO("Start burn PM IC bin file!~\n");
    UBOOT_INFO("u8RegOffset:%x u8WriteEEPROM:%x\n",u8RegOffset,u8WriteEEPROM);
    SWI2C_WriteDatas(&u8RegOffset,1,&u8WriteEEPROM,1);

    UBOOT_TRACE("OK\n");
    return 0;
}

static int do_pgamma_ic_upgrade(IC_INFO_t *sPGAMMAIC, MS_U8 *pu8Buffer)
{
    UBOOT_INFO("Start upgrade PGAMMA IC bin file!~\n");
    MS_U8 u8RegOffset = sPGAMMAIC->Offset;
    MS_U8 u8WriteBit = 0x1;
    MS_U32 u32Size = sPGAMMAIC->Size-26;

    g_bus = sPGAMMAIC->Bus;
    if(sPGAMMAIC->Addr != 0)
    {
        g_slave_addr = sPGAMMAIC->Addr;
        UBOOT_INFO("g_slave_addr:%x\n",g_slave_addr);
    }
    else
    {
        UBOOT_DEBUG("g_slave_addr is zero!!");
        return -1;
    }

    if(u32Size == 0){
        UBOOT_DEBUG("Write data size is zero!!\n");
        return -1;
    }
    else
    {
        UBOOT_INFO("Write data size:%x\n",u32Size);
    }

    UBOOT_INFO("I2cBusCfg: padSCL=%d, padSDA=%d\n", I2cBusCfg[0].padSCL, I2cBusCfg[0].padSDA);
    MApi_SWI2C_Init(I2cBusCfg, I2C_BUS_NUM);

    pu8Buffer[u32Size-2] |= (u8WriteBit<<6);
    SWI2C_WriteDatas(&u8RegOffset,1,pu8Buffer,u32Size);

    UBOOT_TRACE("OK\n");
    return 0;
}

static int Read_FlashDataToAddr(IC_INFO_t *sICInfo, U32 *pu32Addr, char *BinPath)
{
    UBOOT_TRACE("IN\n");

    vfs_mount(CONFIG);
    if(sICInfo->Size == 0)
    {
        UBOOT_ERROR("vfs_getsize error: %s !\n", BinPath);
        return -1;
    }

    if(pu32Addr == NULL)
    {
        UBOOT_DEBUG("malloc pu32Addr memory error !");
        return -1;
    }
    memset((void*)pu32Addr,0,sICInfo->Size);

    if (vfs_read((void *)pu32Addr,BinPath,0,sICInfo->Size) != 0)
    {
        UBOOT_DEBUG("vfs_read data fail ...>>>\n");
    }

    UBOOT_TRACE("OK\n");
    return 0;
}

static int Read_ICBinFileToMem(IC_INFO_t *sICInfo, U32 *pu32Addr, char *BinPath)
{
    int ret = 0;
    UBOOT_TRACE("IN\n");
    ret = Read_FlashDataToAddr(sICInfo, pu32Addr, BinPath);
    UBOOT_TRACE("OK\n");
    return ret;
}

unsigned char* _load_ICBinFile(IC_INFO_t *sICInfo, char *BinPath)
{
    void* Dram_Addr = NULL;

    UBOOT_TRACE("IN\n");
    UBOOT_INFO("BinPath:%s\n",BinPath);
    Dram_Addr=malloc(sICInfo->Size);
    if(Dram_Addr==NULL)
    {
        UBOOT_ERROR("malloc fail!!\n");
    }
    if(Read_ICBinFileToMem(sICInfo, (U32)Dram_Addr, BinPath)!=0) //read logo from spi/emmc to dram
    {
        UBOOT_ERROR("Error: Read_PMICAddr Fail\n");
        free((void*)Dram_Addr);

        return NULL;
    }
    UBOOT_TRACE("OK\n");

    return (unsigned char*)Dram_Addr;
}

static void parse_customer_profile(char* path,IC_INFO_t *sPMIC, IC_INFO_t *sPGAMMAIC)
{
    char **ptr;
    char str[BUFFER_SIZE];
    char *script = NULL;
    U32 filesize = 0;

    UBOOT_TRACE("IN %s\n",CONFIG);
    vfs_mount(CONFIG);
    script = loadscript(path,&filesize);
    if(script == NULL)
    {
        while (*path== '/') path++;
        path=strstr(path,"/");
        UBOOT_INFO("filter /cusdata or /config, and find file again!!\n");
        script = loadscript(path,&filesize);
        if(script == NULL)
        {
            return ;
        }
    }
    UBOOT_INFO("sizeof( model ini script ) --------> %u bytes\n",filesize);
    Profile_Init((char *)script,filesize);

    if(Profile_GetString("TCON_BIN", "PMIC_FILE","", str, sizearray(str)))
    {
        UBOOT_TRACE("PMIC start\n");
        char* tmpPMICBinPath;
        tmpPMICBinPath=str;
        if(FALSE==vfs_getsize(tmpPMICBinPath))
        {
            while (*tmpPMICBinPath== '/') tmpPMICBinPath++;
            tmpPMICBinPath=strstr(tmpPMICBinPath,"/");
            UBOOT_ERROR("filter /cusdata or /config, and find file again!!\n");
            UBOOT_ERROR("tmpPMICBinPath:%s\n",tmpPMICBinPath);

            if(FALSE==vfs_getsize(tmpPMICBinPath))
            {
                tmpPMICBinPath=str;
            }
        }
        snprintf(gPMICBinPath,BUFFER_SIZE,"%s",tmpPMICBinPath);
        sPMIC->Size = vfs_getsize(gPMICBinPath);
        UBOOT_INFO("PMIC path --> %s\n",gPMICBinPath);
        UBOOT_INFO("PMIC size:%x\n",sPMIC->Size);
        if(sPMIC->Size != 0)
        {
            if(Profile_GetString("TCON_BIN", "PMIC_I2C_bus","", str, sizearray(str)))
            {
                sPMIC->Bus = (unsigned int)strtol(str,ptr,16);
                UBOOT_INFO("sPMIC->Bus:%x\n",sPMIC->Bus);
            }
            if(Profile_GetString("TCON_BIN", "PMIC_I2C_addr","", str, sizearray(str)))
            {
                sPMIC->Addr = (unsigned int)strtol(str,ptr,16);
                UBOOT_INFO("sPMIC->Addr:%x\n",sPMIC->Addr);
            }
            if(Profile_GetString("TCON_BIN", "PMIC_I2C_offset","", str, sizearray(str)))
            {
                sPMIC->Offset = (unsigned int)strtol(str,ptr,16);
                UBOOT_INFO("sPMIC->Offset:%x\n",sPMIC->Offset);
            }
        }
    }

    if(Profile_GetString("TCON_BIN", "Pgamma_FILE","", str, sizearray(str)))
    {
        UBOOT_ERROR("PMIC start\n");
        char* tmpPgammaBinPath;
        tmpPgammaBinPath=str;
        if(FALSE==vfs_getsize(tmpPgammaBinPath))
        {
            while (*tmpPgammaBinPath== '/') tmpPgammaBinPath++;
            tmpPgammaBinPath=strstr(tmpPgammaBinPath,"/");
            UBOOT_ERROR("filter /cusdata or /config, and find file again!!\n");
            UBOOT_ERROR("tmpPgammaBinPath:%s\n",tmpPgammaBinPath);

            if(FALSE==vfs_getsize(tmpPgammaBinPath))
            {
                tmpPgammaBinPath=str;
            }
        }
        snprintf(gPGAMMAICBinPath,BUFFER_SIZE,"%s",tmpPgammaBinPath);
        sPGAMMAIC->Size = vfs_getsize(gPGAMMAICBinPath);
        UBOOT_INFO("PMIC path --> %s\n",gPGAMMAICBinPath);
        UBOOT_INFO("PGAMMAIC size:%x\n",sPGAMMAIC->Size);
        if(sPGAMMAIC->Size != 0)
        {
            if(Profile_GetString("TCON_BIN", "Pgamma_I2C_bus","", str, sizearray(str)))
            {
                sPGAMMAIC->Bus = (unsigned int)strtol(str,ptr,16);
                UBOOT_INFO("sPGAMMAIC->Bus:%x\n",sPGAMMAIC->Bus);
            }
            if(Profile_GetString("TCON_BIN", "Pgamma_I2C_addr","", str, sizearray(str)))
            {
                sPGAMMAIC->Addr = (unsigned int)strtol(str,ptr,16);
                UBOOT_INFO("sPGAMMAIC->Addr:%x\n",sPGAMMAIC->Addr);
            }
            if(Profile_GetString("TCON_BIN", "Pgamma_I2C_offset","", str, sizearray(str)))
            {
                sPGAMMAIC->Offset = (unsigned int)strtol(str,ptr,16);
                UBOOT_INFO("sPGAMMAIC->Offset:%x\n",sPGAMMAIC->Offset);
            }
        }
    }
    UBOOT_TRACE("OK");
}

//-------------------------------------------------------------------------------------------------
//  Public Functions
//-------------------------------------------------------------------------------------------------
int Burn_CustomerBinToExternalFlash(char* path)
{
    IC_INFO_t sPMIC,sPGAMMAIC;
    unsigned char *memaddr;

    UBOOT_TRACE("IN\n");
    memset(&sPMIC, 0, sizeof(IC_INFO_t));
    memset(&sPGAMMAIC, 0, sizeof(IC_INFO_t));
    parse_customer_profile(path,&sPMIC,&sPGAMMAIC);
    if(sPMIC.Size > 0)
    {
        memaddr = _load_ICBinFile(&sPMIC,gPMICBinPath);
        do_pm_ic_upgrade(&sPMIC,memaddr);
        do_pm_ic_burn();
    }

    if(sPGAMMAIC.Size > 0)
    {
        memaddr = _load_ICBinFile(&sPGAMMAIC,gPGAMMAICBinPath);
        do_pgamma_ic_upgrade(&sPGAMMAIC,memaddr);
    }
    UBOOT_TRACE("OK\n");
    return 0;
}
