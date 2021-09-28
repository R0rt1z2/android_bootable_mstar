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

#include <common.h>
#include <MsMath.h>
#include <MsRawIO.h>
#include <stdio.h>
#include <MsTypes.h>
#include <MsOS.h>
#include <drvBDMA.h>
#include <drvMIU.h>
#include <MsDebug.h>
#include <miu/MsDrvMiu.h>
#include <ShareType.h>
#include <bootlogo/MsPoolDB.h>
#include <MsMmap.h>
#include <MsApiMiu.h>
#include <MsEnvironment.h>


MS_U32 u32Lx1Start = 0, u32Lx1Size = 0;
MS_U32 u32Lx2Start = 0, u32Lx2Size = 0;
MS_U32 u32Lx3Start = 0, u32Lx3Size = 0;
MS_U32 u32Block = 0;
#if (ENABLE_MINIUBOOT == 0)
static int Show_Dram_Information(MS_U16 miu_type, MS_U16 miu_speed, MS_U16 miu_vendor)
{
    MS_U16 ddr_t, ddr_e, ddr_x, ddr_s;

    // 1 : 4x, 0 : 8x
    ddr_x = miu_type & 0x000f;

    // 1 : EXT, 0 : MCP
    ddr_e = (miu_type & 0x00f0) >> 4;

    // 1 : DDR4 0 : DDR3
    ddr_t = (miu_type & 0x0f00) >> 8;

    // DRAM Size
    ddr_s = (miu_type & 0xf000) >> 12;
 
    if (ddr_e == 1)
    {
        printf("EXT");
    }
    else
    {
        printf("MCP");
        // 1 : Samsung
        // 2 : Micron
        // 3 : Hynix
        // 4 : Nanya
        // 5 : Winbond
        // 6 : ETRON
        if (miu_vendor == 1)
        {
            printf("_S");
        }
        else if (miu_vendor == 2)
        {
            printf("_M");
        }
        else if (miu_vendor == 3)
        {
            printf("_H");
        }
        else if (miu_vendor == 4)
        {
            printf("_N");
        }
        else if (miu_vendor == 5)
        {
            printf("_W");
        }
        else if (miu_vendor == 6)
        {
            printf("_E");
        }
        else
        {
            printf("_X");
        }
    }

    if (ddr_t == 1)
    {
        printf("_D4");
    }
    else
    {
        printf("_D3");
    }

    printf("_%d_%dG_%dx\r\n", miu_speed, ddr_s, ddr_x);
    return 1;
}

int Get_DRAM_Information(void)
{
    MS_U16 miu_type = 0, miu_speed = 0, miu_vendor = 0;
    if (Read2Byte(0x1033A0) == 0xBABE)
    {
        printf("0:");
        miu_type = Read2Byte(0x1033A2);
        miu_speed = Read2Byte(0x1033A4);
        miu_vendor = Read2Byte(0x1033A6) & 0x00ff;
        Show_Dram_Information(miu_type, miu_speed, miu_vendor);
    }
    if (Read2Byte(0x1033B0) == 0xBABE)
    {
        printf("1:");
        miu_type = Read2Byte(0x1033B2);
        miu_speed = Read2Byte(0x1033B4);
        miu_vendor = Read2Byte(0x1033B6) & 0x00ff;
        Show_Dram_Information(miu_type, miu_speed, miu_vendor);
    }
    if (Read2Byte(0x1033C0) == 0xBABE)
    {
        printf("2:");
        miu_type = Read2Byte(0x1033C2);
        miu_speed = Read2Byte(0x1033C4);
        miu_vendor = Read2Byte(0x1033C6) & 0x00ff;
        Show_Dram_Information(miu_type, miu_speed, miu_vendor);
    }
    return 1;
}

int Set_DRAM_Size(void)
{
    MS_U64 u64DramSize1 = 0, u64DramSize2 = 0,u64DramSize3 = 0,u64DramSize4 = 0;
    char *start=0;
    char dram_size_tmp[32];
    char cEnvBuffer[512]= {0};
    char * tok;
    char *s = getenv("KERNEL_PROTECT");
    memset(cEnvBuffer, 0, sizeof(cEnvBuffer));
    if (s != NULL)
    {
        MsDrv_MiuInit();
        strncpy(cEnvBuffer, s,255);
        tok = strtok (cEnvBuffer," ");
        if(isDetectMmap()==0)
        {
            while (tok != NULL)
            {
                if((start = strstr (tok, "DRAM_SIZE1=")) != NULL)
                {
                    memset(dram_size_tmp, 0, sizeof(dram_size_tmp));
                    strncpy(dram_size_tmp, tok+11, strlen(dram_size_tmp)-8);
                    u64DramSize1=(simple_strtoull(dram_size_tmp, NULL, 16));
                    UBOOT_DEBUG("DRAM_SIZE1 0x%llx \n",u64DramSize1);
                    MDrv_MIU_Dram_Size(E_MIU_0, dram_size_to_enum(u64DramSize1));
                }
                else if((start = strstr (tok, "DRAM_SIZE2=")) != NULL)
                {
                    memset(dram_size_tmp,0,sizeof(dram_size_tmp));
                    strncpy(dram_size_tmp, tok+11, strlen(dram_size_tmp)-8);
                    u64DramSize2=(simple_strtoull(dram_size_tmp, NULL, 16));
                    UBOOT_DEBUG("DRAM_SIZE2 0x%llx \n",u64DramSize2);
                    if(u64DramSize2!=0)
                    {
                        MDrv_MIU_Dram_Size(E_MIU_1, dram_size_to_enum(u64DramSize2));
                    }
                }
                else if((start = strstr (tok, "DRAM_SIZE3=")) != NULL)
                {
                    memset(dram_size_tmp,0,sizeof(dram_size_tmp));
                    strncpy(dram_size_tmp, tok+11, strlen(dram_size_tmp)-8);
                    u64DramSize3=(simple_strtoull(dram_size_tmp, NULL, 16));
                    UBOOT_DEBUG("DRAM_SIZE3 0x%llx \n",u64DramSize3);
                    if(u64DramSize3!=0)
                    {
                        MDrv_MIU_Dram_Size(2, dram_size_to_enum(u64DramSize3));// 2:E_MIU_2
                    }
                }
                else if((start = strstr (tok, "DRAM_SIZE4=")) != NULL)
                {
                    memset(dram_size_tmp,0,sizeof(dram_size_tmp));
                    strncpy(dram_size_tmp, tok+11, strlen(dram_size_tmp)-8);
                    u64DramSize4=(simple_strtoull(dram_size_tmp, NULL, 16));
                    UBOOT_DEBUG("DRAM_SIZE4 0x%llx \n",u64DramSize3);
                    if(u64DramSize4!=0)
                    {
                        MDrv_MIU_Dram_Size(3, dram_size_to_enum(u64DramSize4));// 2:E_MIU_3
                    }
                }
                tok = strtok (NULL, " ");
            }
#if ENABLE_NIKON || ENABLE_MILAN || ENABLE_MARLON || ENABLE_KERES || ENABLE_KIRIN || ENABLE_KRIS || ENABLE_KAYLA || ENABLE_K5TN || ENABLE_K1C
            if (u64DramSize1 == 0)
            {
                UBOOT_DEBUG("#######################################################################\n");
                if (u64DramSize1 == 0)
                    UBOOT_DEBUG("# [PROTECT WARNING], miu kernel protect is not enabled on first dram  #\n");
                UBOOT_DEBUG("#######################################################################\n");
                return 0;
            }
#else
            if (u64DramSize1 == 0 || u64DramSize2 == 0 || u64DramSize3 == 0)
            {
                UBOOT_DEBUG("#######################################################################\n");
                if (u64DramSize1 == 0)
                    UBOOT_DEBUG("# [PROTECT WARNING], miu kernel protect is not enabled on first dram  #\n");
                if (u64DramSize2 == 0)
                    UBOOT_DEBUG("# [PROTECT WARNING], miu kernel protect is not enabled on second dram #\n");
                #if ENABLE_MONACO
                if (u64DramSize3 == 0)
                    UBOOT_DEBUG("# [PROTECT WARNING], miu kernel protect is not enabled on 3rd dram #\n");
                #endif
                    UBOOT_DEBUG("#######################################################################\n");
                if (u64DramSize1 == 0 || u64DramSize2 == 0)
                {
                    return 0;
                }
            }
#endif
        }//if(isDetectMmap()==0)
    }

    //Protect no use but define DRAM space. Support for M7221 and M7322
    MSDrv_MIU_ProtectDummyDRAM();

    return 0;
}

int MsApi_kernelProtect(void)
{
    MS_U64 u64LxStart=0,u64Size=0;
    MS_U8 u8BlockMiu[E_MIU_ID_MAX]="\0";
    MS_U8 u8MiuIndex = 0;
    MS_U8 LX_ID=0;

    if ( getenv("KERNEL_PROTECT") != NULL)
    {
        for(LX_ID=E_LX;LX_ID<E_MAX;LX_ID++)
        {
            if(Get_LX_MemInfo(LX_ID,&u64LxStart,&u64Size)==TRUE)
            {
                u8MiuIndex=MsApi_Miu_GetID(u64LxStart);
                UBOOT_DEBUG("Lx%d start:[0x%llx] Size[0x%llx] in MIU(%d)\n",(LX_ID+1),u64LxStart,u64Size,u8MiuIndex);
                if(u64Size>0)
                {
                    MsDrv_kernelProtect(u8BlockMiu[u8MiuIndex],u64LxStart,(MS_U64)((MS_U64)u64LxStart + (MS_U64)u64Size));
                    u8BlockMiu[u8MiuIndex]++;
                }
            }
        }

    }
    return 0;
}


/*
setenv DynMmapID E_LX_MEM2 E_LX_MEM2 E_LX_MEM2 E_LX_MEM2 E_LX_MEM2 E_LX_MEM2 E_LX_MEM2
setenv MIUPRO0 0 0 E_LX_MEM2 0x00000000 0x00200000 0 43 57 82 08 11 12 13 15 24 26 28 29 0 0 0 0
setenv MIUPRO1 0 1 E_LX_MEM2 0x00300000 0x00500000 0 03 06 07 08 11 12 13 15 24 26 28 29 0 0 0 0
setenv MIUPRO2 0 2 E_LX_MEM2 0x00600000 0x00700000 0 03 06 07 08 11 12 13 15 24 26 28 29 0 0 0 0
setenv MIUPRO3 0 3 E_LX_MEM2 0x00600000 0x00700000 0 03 06 07 08 11 12 13 15 24 26 28 29 0 0 0 0
setenv MIUPRO4 1 0 E_LX_MEM2 0x00600000 0x00700000 0 03 06 07 08 11 12 13 15 24 26 28 29 0 0 0 0
setenv MIUPRO5 1 1 E_LX_MEM2 0x00600000 0x00700000 0 03 06 07 08 11 12 13 15 24 26 28 29 0 0 0 0
setenv MIUPRO6 1 2 E_LX_MEM2 0x00600000 0x00700000 0 03 06 07 08 11 12 13 15 24 26 28 29 0 0 0 0
setenv MIUPRO7 1 3 E_LX_MEM2 0x00600000 0x00700000 0 03 06 07 08 11 12 13 15 24 26 28 29 0 0 0 0


*/
int MsApi_MiuProtect(void)
{
    char cTempBuffer[BUFFER_SIZE]= {0};
    char cEnvBuffer[BUFFER_SIZE]= {0};
    char * p_str = NULL;
    char * tok=NULL;
    int index=0;
    int client_index=0;
    MS_PHYADDR temp_start=0;
    U32 temp_length=0;
    MS_BOOL checkIndex=TRUE;
    UBOOT_TRACE("IN\n");
    do
    {
        memset(cTempBuffer, 0, BUFFER_SIZE);
        memset(cEnvBuffer, 0, BUFFER_SIZE);
        p_str = NULL;
        tok = NULL;
        snprintf(cTempBuffer, BUFFER_SIZE, "MIUPRO%d", index);
        UBOOT_DEBUG(" getenv[%s] \n",cTempBuffer);
        p_str = getenv (cTempBuffer);
        if(p_str != NULL)
        {
            strncpy(cEnvBuffer, p_str,BUFFER_SIZE-1);
            stMIU_PortectInfo portectInfo;
            memset(&portectInfo, 0, sizeof(stMIU_PortectInfo));
            tok = strtok (cEnvBuffer," ");
            portectInfo.miu_id=(MIU_ID)((int)simple_strtol(tok, NULL, 16));
            tok = strtok (NULL," ");
            portectInfo.u8Group=(int)simple_strtol(tok, NULL, 16);
            tok = strtok (NULL," ");
            portectInfo.c_Mmap_Id=tok;
            UBOOT_DEBUG(" MMAP ID =%s",portectInfo.c_Mmap_Id);
            checkIndex=TRUE;

            if( NULL!= strstr (tok, "E_") )
            {

                tok = strtok (NULL," ");//start not used
                tok = strtok (NULL," ");//end not used
                if(get_addr_from_mmap(portectInfo.c_Mmap_Id, &temp_start)==0)
                {
                    portectInfo.u32Start=temp_start;
                }
                else
                {
                    UBOOT_DEBUG(" index[%d]%s get mmap fail %s !!!\n",index,cTempBuffer,portectInfo.c_Mmap_Id);
                    checkIndex=FALSE;

                }
                if(get_length_from_mmap(portectInfo.c_Mmap_Id, &temp_length)==0)
                {
                    portectInfo.u32End=temp_start+temp_length;
                }
                else
                {
                    UBOOT_DEBUG(" index[%d]%s get mmap fail %s !!!\n",index,cTempBuffer,portectInfo.c_Mmap_Id);
                    checkIndex=FALSE;
                }
            }
            else
            {

                tok = strtok (NULL," ");
                portectInfo.u32Start=simple_strtoull(tok, NULL, 16);
                tok = strtok (NULL," ");
                portectInfo.u32End=simple_strtoull(tok, NULL, 16);
                if(portectInfo.u32End==0)
                {
                    UBOOT_DEBUG(" index[%d] %s end of address is 0  !!!\n",index,cTempBuffer);

                    if(portectInfo.u32Start==0)
                    {
                        index=index+1;
                        continue;
                    }
                    else
                        checkIndex=FALSE;
                }
            }
            if(checkIndex==FALSE)
            {
                UBOOT_ERROR(" %s parse Fail  !!!\n",cTempBuffer);
                index=index+1;
                continue;
            }
            tok = strtok (NULL," ");// do nothing for enable/disable flag
            for(client_index=0;client_index<16;client_index++)
            {
                tok = strtok (NULL," ");
                if(tok==NULL)
                {
                    UBOOT_DEBUG(" index[%d] Client ID end of index [%d] !!!\n",index,client_index);
                    break;
                }
                portectInfo.clientID[client_index]=(MS_U8)simple_strtol(tok, NULL, 10);
            }
            if(dbgLevel>=EN_DEBUG_LEVEL_DEBUG)
            {
                printf(" Block[%d]start[%llx]end[%llx]\n",portectInfo.u8Group,portectInfo.u32Start,portectInfo.u32End);
                printf("portect IDs \n");
                for(client_index=0;client_index<16;client_index++)
                {
                    printf("%d[0x%x] ",client_index,portectInfo.clientID[client_index]);
                    if(client_index==7)
                        printf("\n");
                }
                printf("\n");
            }

            MsDrv_MIU_Protect(portectInfo.u8Group,portectInfo.clientID,portectInfo.u32Start,portectInfo.u32End,true);

        }
        else
        {
            UBOOT_DEBUG("No Portect Infor !!!\n");
            return -1;
        }
        index=index+1;
    }
    while (p_str!=NULL);


    UBOOT_TRACE("OK\n");
    return 0;

}

int do_MsApi_kernelProtect( cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{

    return MsApi_kernelProtect();

}
int do_MsApi_MiuProtect( cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{

    return MsApi_MiuProtect();

}

void MsApi_RunTimePmProtect(MS_PHYADDR StartAddr, MS_PHYADDR EndAddr)
{
#if (CONFIG_ENABLE_RT_PM_PROTECT)
    MsDrv_PMProtect(3,StartAddr,EndAddr); //64K run time PM
#endif
}
int do_MsApi_RunTimePmProtect( cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    MS_PHYADDR StarAddr=(MS_PHYADDR)simple_strtoull(argv[1], NULL, 16);
    MS_PHYADDR EndAddr=(MS_PHYADDR)simple_strtoull(argv[2], NULL, 16);
    MsApi_RunTimePmProtect(StarAddr, EndAddr);
    return 0;
}

extern unsigned int u32UbootStart;
extern unsigned int u32UbootEnd;
#if 0 // by jh

// return 1 for overlap case, 0 for non-overlap case
static MS_U32 isOverlapUboot(MS_U32 u32Start, MS_U32 u32Size)
{
    if ((u32UbootStart <= u32Start) && (u32Start <= u32UbootEnd))
        return 1;
    if ((u32UbootStart <= (u32Start+u32Size)) && ((u32Start+u32Size) <= u32UbootEnd))
        return 1;
    return 0;
}

static MS_U32 return_or_block(MS_U32 u32RetVal)
{
    if (u32Block)
        while(1);

    return u32RetVal;
}


#define TEST_OFFSET (128*1024*1024) // set 128M offset
#define TEST_OFFSET_MASK (TEST_OFFSET-1)
#define MIU1_OFFSET CONFIG_SYS_MIU1_CACHE
#define TEST_RANGE CONFIG_SYS_MIU_INTERVAL
static MS_U32 write_test_to_lxmem(MS_U32 u32Start, MS_U32 u32Size)
{
    MS_U32 i;
    MS_U32 u32DramValue[u32Size/sizeof(MS_U32)];
    MS_U32* pu32Start = (MS_U32*)u32Start;
    MS_U32 failed = 0;
    MS_U32 u32DramSize = 0, u32MiuOffset = 0;
    MS_U32 u32BdmaStart = 0;
    BDMA_CpyType eBdmaCpyType;

    if (!u32Size)
    {
        printf("[PROTECT ERROR], lxmem size = 0\n");
        return return_or_block(-1);
    }

    // 1. decide dram parameters
    u32DramSize = (u32Start < MIU1_OFFSET) ? u32DramSize1 : u32DramSize2;
    u32MiuOffset = (u32Start < MIU1_OFFSET) ? 0 : MIU1_OFFSET;
    eBdmaCpyType = (u32Start < MIU1_OFFSET) ? E_BDMA_FLASH2SDRAM : E_BDMA_FLASH2SDRAM1;
    if (!u32DramSize)
    {
        printf("[PROTECT ERROR], dram size = 0\n");
        return return_or_block(-1);
    }

    // 2. backup parital values in protected region
    for (i = 0; i < sizeof(u32DramValue)/sizeof(MS_U32); i++)
        u32DramValue[i] = *(pu32Start + i);

    // 3. bdma attack
    u32BdmaStart = (u32Start & TEST_OFFSET_MASK) + u32MiuOffset;
    for (i = 0; i < (TEST_RANGE/TEST_OFFSET); i++)
    {
        u32BdmaStart += TEST_OFFSET;
        if (isOverlapUboot(u32BdmaStart, u32Size))
            continue;
        // bdma source is not important, just fill it with reasonable address
        MDrv_BDMA_CopyHnd(0x20000, u32BdmaStart, u32Size, eBdmaCpyType, 0);
    }
    flush_cache(u32Start, u32Size); // necessary, or the value would be kept in cache

    // 4. check whether the region is protected or not
    for (i = 0; i < sizeof(u32DramValue)/sizeof(MS_U32); i++)
    {
        if (u32DramValue[i] != *(pu32Start + i))
        {
            failed = 1;
            break;
        }
    }

    if (failed)
    {
        printf("[PROTECT ERROR], fail at address 0x%p \n", pu32Start + i);
        return return_or_block(-1);
    }
    else
    {
        printf("[PROTECT INFO], pass at address 0x%X, size 0x%X\n", (unsigned int)u32Start, (unsigned int)u32Size);
        return 0;
    }
}

static MS_U32 isDramExist(MS_U32 u32LxStart)
{
    if (u32LxStart < MIU1_OFFSET)
        return u32DramSize1 ? 1 : 0;
    else
        return u32DramSize2 ? 1 : 0;
}

#define TEST_SIZE (256) // it's enough to test 256 bytes on lx mem head & tail
#endif
void MsApi_kernelProtectBist(void)
{
    #if 0 //jh
    MIU_PortectInfo info;

    if (isDramExist(u32Lx1Start) && u32Lx1Size >= TEST_SIZE)
    {
        write_test_to_lxmem(u32Lx1Start, TEST_SIZE);
        write_test_to_lxmem(u32Lx1Start + u32Lx1Size - TEST_SIZE, TEST_SIZE);
    }
    if (isDramExist(u32Lx2Start) && u32Lx2Size >= TEST_SIZE)
    {
        write_test_to_lxmem(u32Lx2Start, TEST_SIZE);
        write_test_to_lxmem(u32Lx2Start + u32Lx2Size - TEST_SIZE, TEST_SIZE);
    }
    if (isDramExist(u32Lx3Start) && u32Lx3Size >= TEST_SIZE)
    {
        write_test_to_lxmem(u32Lx3Start, TEST_SIZE);
        write_test_to_lxmem(u32Lx3Start + u32Lx3Size - TEST_SIZE, TEST_SIZE);
    }

    MDrv_MIU_GetProtectInfo(0, &info);
    MDrv_MIU_GetProtectInfo(1, &info);
    #endif
}

int do_MsApi_kernelProtectBist(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    MsApi_kernelProtectBist();
    return 0;
}


void MsApi_NuttxProtect(MS_U32 u32StartAddr, MS_U32 u32EndAddr)
{
    UBOOT_DEBUG("IN\n");

    UBOOT_DEBUG("u32StartAddr=0x%x\n",(unsigned int)u32StartAddr);
    UBOOT_DEBUG("u32EndAddr=0x%x\n",(unsigned int)u32EndAddr);
    MsDrv_NuttxProtect(3,u32StartAddr,u32EndAddr);

    UBOOT_DEBUG("OK\n");
}

int do_MsApi_NuttxProtect(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
        MS_PHYADDR Start=0;
        MS_PHYADDR End=0;
        if(argc<3) {
        cmd_usage(cmdtp);
        return -1;
    }
    Start=(MS_PHYADDR)simple_strtoull(argv[1], NULL, 16);
    End=(MS_PHYADDR)simple_strtoull(argv[2], NULL, 16);
    UBOOT_DEBUG("Start=0x%llx\n",Start);
        UBOOT_DEBUG("End=0llx%x\n",End);
    MsApi_NuttxProtect(Start,End); //"protect address" and "protect lenth" must align to 64K
    return 0;
}
#endif
extern U8 MDrv_MIU_SelectMIU_UBoot(U8 bMIU1, U16 *au16SelMiu, U8 u8GpNum);
extern U8 MDrv_MIU_SetGroupPriority_UBoot(U8 bMIU1, U8 *au8GpPriority, U8 u8GpNum);

static void set_miu_group_priority(U8 u8MiuIdx, const char* cStr)
{
    if (cStr)
    {
        int i = 0;
        int j = 0;
        int length = 0;
        U8 au8GpPriority[MIU_MAX_GP_PRIORITY_NUM] = {0};

        for (;;)
        {
            if (cStr[i] != ':')
            {
                au8GpPriority[j++] = cStr[i] - 0x30;
            }
            else
            {
                ++length; // length is ':' number and priority number need add one more.
            }

            if ('\0' == cStr[i])
            {
                ++length;
                break;
            }

            ++i;
        }
        MDrv_MIU_SetGroupPriority_UBoot(u8MiuIdx, au8GpPriority, length);
    }
}

static void select_miu(U8 u8MiuIdx, const char* cStr)
{
    if (cStr)
    {
        int i = 0;
        int j = 0;
        int length = 0;

        for (;;)
        {
            if (':' == cStr[i])
            {
                ++length;
            }

            if ('\0' == cStr[i])
            {
                ++length; // length is ':' number and priority number need add one more.
                break;
            }

            ++i;
        }

        i = 0;
        j = 0;
        U16 au16SelMiu[MIU_MAX_GP_NUM] = {0};

        for (;;)
        {
            au16SelMiu[j++] = ascii_to_hex(&cStr[i], 4);
            i += 5;

            if (length == j)
            {
                break;
            }
        }

        MDrv_MIU_SelectMIU_UBoot (u8MiuIdx, au16SelMiu, length);
    }
}

static void set_miu(void)
{
    set_miu_group_priority(0, getenv("MIU0_GROUP_PRIORITY"));
    set_miu_group_priority(1, getenv("MIU1_GROUP_PRIORITY"));

#if 0
    set_miu_group_priority(2, getenv("MIU2_GROUP_PRIORITY"));
#endif
    select_miu(0, getenv("MIU0_GROUP_SELMIU"));
    select_miu(1, getenv("MIU1_GROUP_SELMIU"));

#if 0
    select_miu(2, getenv("MIU2_GROUP_SELMIU"));
#endif
}

int do_set_miu(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
#if (ENABLE_MINIUBOOT == 0)
    Get_DRAM_Information();
#endif
    set_miu();
#if (ENABLE_MINIUBOOT == 0)
    Set_DRAM_Size();
#endif
    return 0;
}

