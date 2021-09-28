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
#include <command.h>
#include <config.h>
#include <ShareType.h>
#include <MsTypes.h>
#include <MsDebug.h>
#include <exports.h>
#include <MsVfs.h>
#include <MsMmap.h>
#include <CusConfig.h>
#include <MsUtility.h>
#include <ShareType.h>
#include <MsEnvironment.h>
#include <miu/MsDrvMiu.h>
#include <secure/MsSecureCommon.h>
#include <secure/crypto_aes.h>
#include <secure/MsSecureBoot.h>

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define MMAP_FILE_NAME "mmap.ini"
//12 ??? For a example
//#define E_MMAP_ID_PM51_USAGE_MEM_LEN                           0x0000200000
//'0x0000200000' has 12 characters.
#define NUM_LEN 12
#define ENV_MAP_PARTITION "MI_MAP_PARTITION"
#define ENV_MAP_PATH "MI_MAP_PATH"
//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static U8 *mmap_buffer=NULL;
static U32 mmap_buffer_size=0;


BOOLEAN bIsLoadMmapSuccess = TRUE;
static BOOLEAN is_mmap_ready=FALSE;
static U64 miu_interval=0;
static U64 miu_interval2=0;
//static U64 miu_interval3=0;
static MS_U64 dram_length=0;
static MS_U64 dram_length0=0;
static MS_U64 dram_length1=0;
static MS_U64 dram_length2=0;
//static U32 dram_length3=0;



//-------------------------------------------------------------------------------------------------
//  Extern Functions
//-------------------------------------------------------------------------------------------------
extern int snprintf(char *str, size_t size, const char *fmt, ...);

//-------------------------------------------------------------------------------------------------
//  Private Functions
//-------------------------------------------------------------------------------------------------
int init_mmap(void);
int LoadMMAP(void);
int _get_mmap(U8 *buffer, char *id, MMapInfo_s *mmapInfo);
int get_mmap(char *id, MMapInfo_s *mmapInfo);
int list_mmap_id(void);
int save_mmapInfo_to_env(char *id, U64 value, U8 bIsAddrInfo);
int del_mmapInfo_from_env(char *id,U8 bIsAddrInfo);

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------


int do_get_mmap(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    MS_PHYADDR addr=0;
    U32 len=0;
    int ret=0;
    UBOOT_TRACE("IN\n");

    if(argc!=2)
    {
        cmd_usage(cmdtp);
        return -1;
    }
    if(strncmp(argv[1], "-l", 2) == 0)
    {

        ret=list_mmap_id();
    }
    else
    {
        ret=get_addr_from_mmap(argv[1],&addr);
        if(ret!=0)
        {
            UBOOT_ERROR("get addresss fail\n");
            return -1;
        }
        ret=get_length_from_mmap(argv[1],&len);
        if(ret!=0)
        {
            UBOOT_ERROR("get addresss fail\n");
            return -1;
        }
        UBOOT_INFO("addr=0x%llx\n",(MS_PHYADDR)addr);
        UBOOT_INFO("len=0x%x\n",(unsigned int)len);
    }


    if(ret!=0)
    {
        UBOOT_ERROR("get_mmap fail\n");
    }
    else
    {
        UBOOT_TRACE("OK\n");
    }

    return 0;
}

int get_dram_length(U32 *length)
{
    UBOOT_TRACE("IN\n");
    if(length==NULL)
    {
        UBOOT_ERROR("The 'length' is a null pointer\n");
        return -1;
    }

    if(init_mmap()!=0)
    {
        UBOOT_ERROR("init mmap fail\n");
        return -1;
    }
    UBOOT_DEBUG("dram_length=0x%x\n",dram_length);
    *length=dram_length;
    UBOOT_TRACE("OK\n");
    return 0;


}

MS_U64 get_specific_dram_length(int miu_number)
{
    MS_U64 miu_length=0;
    UBOOT_TRACE("IN\n");
    if(init_mmap()!=0)
    {
        UBOOT_ERROR("init mmap fail\n");
        return -1;
    }

    if(miu_number == 0)
    {
        miu_length=dram_length0;
    }
    else if(miu_number == 1)
    {
        miu_length=dram_length1;
    }
    else if(miu_number == 2)
    {
        miu_length=dram_length2;
    }
    // else if(miu_number == 3)
    //{
    //    miu_length=dram_length3;
    //}
    else
    {
        miu_length=0;
    }

    UBOOT_TRACE("OK\n");
    return miu_length;
}

int save_addr_to_env(char *id, U64 addr)
{
        return save_mmapInfo_to_env(id,addr,TRUE);
}
int del_addr_from_env(char *id)
{
        return del_mmapInfo_from_env(id,TRUE);
}
int save_len_to_env(char *id, U32 len)
{
        return save_mmapInfo_to_env(id,(U64)len,FALSE);
}
int del_len_from_env(char *id)
{
        return del_mmapInfo_from_env(id,FALSE);
}

int save_mmapInfo_to_env(char *id, U64 value, U8 bIsAddrInfo)
{
        char cmdBuf[CMD_BUF];
        UBOOT_TRACE("IN\n");
        if(id==NULL)
        {
                UBOOT_ERROR("id is a null pointer\n");
                return -1;
        }
        memset(cmdBuf,0,CMD_BUF);
        if(bIsAddrInfo==TRUE)
        {
            snprintf(cmdBuf,CMD_BUF,"setenv %s_ADR 0x%llx",id,value);
        }
        else
        {
            snprintf(cmdBuf,CMD_BUF,"setenv %s_LEN 0x%x",id,value);
        }
        UBOOT_DEBUG("cmdBuf=%s\n",cmdBuf);
        if(0!=run_command(cmdBuf,0))
        {
            UBOOT_ERROR("fail\n");
            return -1;
        }
        UBOOT_TRACE("OK\n");
        return 0;
}
int del_mmapInfo_from_env(char *id,U8 bIsAddrInfo)
{
        char cmdBuf[CMD_BUF];
        UBOOT_TRACE("IN\n");
        if(id==NULL)
        {
                UBOOT_ERROR("id is a null pointer\n");
                return -1;
        }
        memset(cmdBuf,0,CMD_BUF);
        if(bIsAddrInfo==TRUE)
        {
            snprintf(cmdBuf,CMD_BUF,"setenv %s_ADR",id);
        }
        else
        {
            snprintf(cmdBuf,CMD_BUF,"setenv %s_LEN",id);
        }
        UBOOT_DEBUG("cmdBuf=%s\n",cmdBuf);
        if(0!=run_command(cmdBuf,0))
        {
            UBOOT_ERROR("fail\n");
            return -1;
        }
        UBOOT_TRACE("OK\n");
        return 0;
}

int setMiu2env(char *miuID)
{
    char cmdBuf[CMD_BUF]="\0";
    char *ptr= NULL;
    int i=0;
    UBOOT_DEBUG("miuID : %s\n",miuID);
    ptr=pattern_search((char *)mmap_buffer,mmap_buffer_size,miuID);
    if(ptr==NULL)
    {
        UBOOT_INFO("no miu_setting information in mmap.ini\n");
        return 0;
    }
    ptr+=strlen(miuID);//ignore miuID
    while(*ptr==' ')// skip space
    {
        ptr++;
    }

    while(*(ptr+i)!='\n')
    {
        cmdBuf[i]=*(ptr+i);
        i++;
    }
    UBOOT_DEBUG("setenv %s %s\n",miuID,cmdBuf);
    setenv(miuID,cmdBuf);
    return 0;
}


int set_mmap2env(char *id, U64 *addr, U32 *len)
{
    MMapInfo_s mmapInfo;
    UBOOT_TRACE("IN\n");
    if((id==NULL)||(addr==NULL))
    {
        UBOOT_ERROR("One of the parameters is a null pointer\n");
        return -1;
    }
    UBOOT_DEBUG("id=%s\n",id);

    if(get_mmap(id,&mmapInfo)!=0)
    {
        UBOOT_DEBUG("get mmap fail\n");
        return -1;
    }
    if(mmapInfo.u32miuID==0)
    {
        UBOOT_DEBUG("In MIU0\n");
        *addr=mmapInfo.u64Addr;
    }
    else if(mmapInfo.u32miuID==1)
    {
        UBOOT_DEBUG("In MIU1\n");
        *addr=mmapInfo.u64Addr+miu_interval;

    }
    else //if(mmapInfo.u32miuID==2)
    {
        UBOOT_DEBUG("In MIU2\n");
        *addr=mmapInfo.u64Addr+miu_interval2;
    }
    //else //if(mmapInfo.u32miuID==3)
    //{
    //    UBOOT_DEBUG("In MIU3\n");
    //    *addr=mmapInfo.u64Addr+miu_interval3;
    //}
    *len=mmapInfo.u32Size;
    UBOOT_DEBUG("addr=0x%llx\n",*addr);
    UBOOT_DEBUG("len=0x%x\n",*len);
    UBOOT_TRACE("OK\n");
    return 0;
}

int get_64bit_addr_from_mmap(char *id, U64 *addr)
{
    char cmdBuf[CMD_BUF]="\0";
    char *pEnv=NULL;
    UBOOT_TRACE("IN\n");
    if((id==NULL)||(addr==NULL))
    {
        UBOOT_ERROR("One of the parameters is a null pointer\n");
        return -1;
    }
    UBOOT_DEBUG("id=%s\n",id);
    snprintf(cmdBuf,CMD_BUF,"%s_ADR",id);
    UBOOT_DEBUG("cmdBuf=%s\n",cmdBuf);
    pEnv=getenv(cmdBuf);
    if(pEnv==NULL)
    {
        UBOOT_DEBUG("Can not get %s environment\n",cmdBuf);
        return -1;
    }
    else
    {
        *addr=simple_strtoull(pEnv, NULL, 16);
        UBOOT_DEBUG("*addr=0x%llx\n",*addr);
        if (is_overlap_mboot_mmap(cmdBuf) != TRUE)
        {
            UBOOT_TRACE("OK\n");
            return -1;
        }
        UBOOT_TRACE("OK\n");
    }
    return 0;
}

int get_addr_from_mmap(char *id, MS_PHYADDR *addr)
{
    char cmdBuf[CMD_BUF]="\0";
    char *pEnv=NULL;
    UBOOT_TRACE("IN\n");
    if((id==NULL)||(addr==NULL))
    {
        UBOOT_ERROR("One of the parameters is a null pointer\n");
        return -1;
    }
    UBOOT_DEBUG("id=%s\n",id);
    snprintf(cmdBuf,CMD_BUF,"%s_ADR",id);
    UBOOT_DEBUG("cmdBuf=%s\n",cmdBuf);
    pEnv=getenv(cmdBuf);
    if(pEnv==NULL)
    {
        UBOOT_DEBUG("Can not get %s environment\n",cmdBuf);
        return -1;
    }
    else
    {
        *addr=simple_strtoull(pEnv, NULL, 16);
        UBOOT_DEBUG("*addr=0x%llx\n",(MS_U64)*addr);
        if (is_overlap_mboot_mmap(cmdBuf) != TRUE)
        {
            UBOOT_TRACE("OK\n");
            return -1;
        }
        UBOOT_TRACE("OK\n");
    }
    return 0;
}

int get_length_from_mmap(char *id, U32 *len)
{
    char cmdBuf[CMD_BUF]="\0";
    char *pEnv=NULL;
    UBOOT_TRACE("IN\n");
    if((id==NULL)||(len==NULL))
    {
        UBOOT_ERROR("One of the parameters is a null pointer\n");
        return -1;
    }
    UBOOT_DEBUG("id=%s\n",id);
    snprintf(cmdBuf,CMD_BUF,"%s_LEN",id);
    UBOOT_DEBUG("cmdBuf=%s\n",cmdBuf);
    pEnv=getenv(cmdBuf);
    if(pEnv==NULL)
    {
        UBOOT_DEBUG("Can not get %s environment\n",cmdBuf);
        return -1;
    }
    else
    {
        *len=simple_strtoul(pEnv, NULL, 16);
        UBOOT_DEBUG("*len=0x%x\n",*len);
        UBOOT_TRACE("OK\n");
    }
    return 0;
}



int get_mmap(char *id, MMapInfo_s *mmapInfo)
{
    UBOOT_TRACE("IN\n");

    if((id==NULL)||(mmapInfo==NULL))
    {
        UBOOT_ERROR("One of the parameters is a null pointer\n");
        return -1;
    }

    if(init_mmap()!=0)
    {
        UBOOT_ERROR("init mmap fail\n");
        return -1;
    }

    if(0!=_get_mmap(mmap_buffer,id,mmapInfo))
    {
        UBOOT_DEBUG("_get_mmap fail\n");
        return -1;
    }

    UBOOT_TRACE("OK\n");
    return 0;
}
#if (ENABLE_MODULE_DISPLAY_OSD==1)
extern int display_osd;
#endif
int init_mmap(void)
{
    char *str=NULL;
    char *ptr=NULL;
    UBOOT_TRACE("IN\n");
    if(is_mmap_ready==TRUE)
    {
        UBOOT_TRACE("OK\n");
        return 0;
    }

    if(LoadMMAP()!=0)
    {
        #if (ENABLE_MODULE_DISPLAY_OSD==1)
        display_osd = -1;
        #endif
        UBOOT_ERROR("load mmap fail\n");
        return -1;
    }

    str=malloc(CMD_BUF);
    if(str==NULL)
    {
        UBOOT_ERROR("malloc for str fail\n");
        return -1;
    }
    memset(str,0,CMD_BUF);

    //#define MIU_INTERVAL                0x0020000000
    ptr=strstr((char *)mmap_buffer,"MIU_INTERVAL");
    if(ptr!=NULL)
    {
        ptr=strstr(ptr,"0x");
        snprintf(str,NUM_LEN,"%s\n",ptr);
        UBOOT_DEBUG("str=%s\n",str);
        miu_interval=simple_strtoull(str, NULL, 16);
        UBOOT_DEBUG("miu_interval=0x%llx\n",miu_interval);
    }
    //#define MIU_INTERVAL2                0x0020000000
    ptr=strstr((char *)mmap_buffer,"MIU_INTERVAL2");
    if(ptr!=NULL)
    {
        ptr=strstr(ptr,"0x");
        snprintf(str,NUM_LEN,"%s\n",ptr);
        UBOOT_DEBUG("str=%s\n",str);
        miu_interval2=simple_strtoull(str, NULL, 16);
        UBOOT_DEBUG("miu_interval2=0x%llx\n",miu_interval2);
    }
    //#define MIU_INTERVAL3                0x0020000000
    //ptr=strstr((char *)mmap_buffer,"MIU_INTERVAL3");
    //if(ptr!=NULL)
    //{
    //    ptr=strstr(ptr,"0x");
    //    snprintf(str,NUM_LEN,"%s\n",ptr);
    //    UBOOT_DEBUG("str=%s\n",str);
    //    miu_interval3=simple_strtoull(str, NULL, 16);
    //    UBOOT_DEBUG("miu_interval3=0x%llx\n",miu_interval3);
    //}
    //#define MIU_DRAM_LEN                0x0020000000
    ptr=strstr((char *)mmap_buffer,"MIU_DRAM_LEN");
    if(ptr!=NULL)
    {
        ptr=strstr(ptr,"0x");
        snprintf(str,NUM_LEN,"%s\n",ptr);
        UBOOT_DEBUG("str=%s\n",str);
        dram_length=simple_strtoull(str, NULL, 16);
        UBOOT_DEBUG("dram_length=0x%llx\n",dram_length);
    }
    //#define MIU_DRAM_LEN0                0x0010000000
    ptr=strstr((char *)mmap_buffer,"MIU_DRAM_LEN0");
    if(ptr!=NULL)
    {
        ptr=strstr(ptr,"0x");
        snprintf(str,NUM_LEN,"%s\n",ptr);
        UBOOT_DEBUG("str=%s\n",str);
        dram_length0=simple_strtoull(str, NULL, 16);
        UBOOT_DEBUG("dram_length0=0x%llx\n",dram_length0);
    }
    //#define MIU_DRAM_LEN1                0x0010000000
    ptr=strstr((char *)mmap_buffer,"MIU_DRAM_LEN1");
    if(ptr!=NULL)
    {
        ptr=strstr(ptr,"0x");
        snprintf(str,NUM_LEN,"%s\n",ptr);
        UBOOT_DEBUG("str=%s\n",str);
        dram_length1=simple_strtoull(str, NULL, 16);
        UBOOT_DEBUG("dram_length1=0x%llx\n",dram_length1);
    }
    //#define MIU_DRAM_LEN2                0x0010000000
    ptr=strstr((char *)mmap_buffer,"MIU_DRAM_LEN2");
    if(ptr!=NULL)
    {
        ptr=strstr(ptr,"0x");
        snprintf(str,NUM_LEN,"%s\n",ptr);
        UBOOT_DEBUG("str=%s\n",str);
        dram_length2=simple_strtoull(str, NULL, 16);
        UBOOT_DEBUG("dram_length2=0x%llx\n",dram_length2);
    }
    //#define MIU_DRAM_LEN3                0x0010000000
    //ptr=strstr((char *)mmap_buffer,"MIU_DRAM_LEN3");
    //if(ptr!=NULL)
    //{
    //    ptr=strstr(ptr,"0x");
    //    snprintf(str,NUM_LEN,"%s\n",ptr);
    //    UBOOT_DEBUG("str=%s\n",str);
    //    dram_length3=simple_strtoul(str, NULL, 16);
    //    UBOOT_DEBUG("dram_length2=0x%x\n",dram_length3);
    //}

    is_mmap_ready=TRUE;
    free(str);

    UBOOT_TRACE("OK\n");
    return 0;
}

int isDetectMmap(void)
{
    int ret=0;
    if ( (getenv("DETECT_MMAP") != NULL) && (strcmp(getenv("DETECT_MMAP"),"1")==0))
    {
        ret=1;
    }
    return ret;
}



void GetMmapPath(char *strCommand)
{
#define MMAP_PATH "MMAP_PATH"

    char *pMiu0Size=NULL;
    char *pMiu1Size=NULL;
    char *pMiu2Size=NULL;
    //char *pMiu3Size=NULL;
    char s8TmpBuf[CMD_BUF]="\0";
    char s8MmapPath[CMD_BUF]="\0";
    char * pEnv=NULL;
    UBOOT_TRACE("IN\n");


    if(isDetectMmap()==0)
    {
#if defined(CONFIG_ONESN_ENABLE)
        mstar_project_ID_t ProjectID=GetProjectID();
        snprintf(strCommand,CMD_BUF,"config/onebin/model_%d/%s",ProjectID.Model_ID,MMAP_FILE_NAME);
#else
        pEnv=getenv(ENV_MAP_PATH);
        if (pEnv !=NULL)
        {
            snprintf(strCommand,CMD_BUF,"/%s",pEnv);
            UBOOT_DEBUG("Get MAP path from env =%s \n",strCommand);
        }
        else
        {
            snprintf(strCommand,CMD_BUF,"%s/%s",CONFIG_PATH,MMAP_FILE_NAME);
            UBOOT_DEBUG("Get MAP path from cus ini =%s \n",strCommand);
        }
#endif
        del_bootargs_cfg(MMAP_PATH,0);
    }
    else
    {
        pMiu0Size=MsDrv_GetMIUSize(0); //miu0
        pMiu1Size=MsDrv_GetMIUSize(1); //miu1
        pMiu2Size=MsDrv_GetMIUSize(2); //miu2
        //pMiu3Size=MsDrv_GetMIUSize(3); //miu3
        //if(pMiu3Size!=NULL)//mmap_1024_1024_512.ini
        //{
        //    snprintf(s8TmpBuf,CMD_BUF,"mmap_%s_%s_%s_%s.ini",
        //    pMiu0Size,pMiu1Size,pMiu2Size,pMiu3Size);
        //}
        //else
        if(pMiu2Size!=NULL)//mmap_1024_1024_512.ini
        {
            snprintf(s8TmpBuf,CMD_BUF,"mmap_%s_%s_%s.ini",
                     pMiu0Size,pMiu1Size,pMiu2Size);
        }
        else if(pMiu1Size!=NULL)//mmap_1024_1024.ini
        {
            snprintf(s8TmpBuf,CMD_BUF,"mmap_%s_%s.ini",
                     pMiu0Size,pMiu1Size);
        }
        else if(pMiu0Size!=NULL)//mmap_1024.ini
        {
            snprintf(s8TmpBuf,CMD_BUF,"mmap_%s.ini",pMiu0Size);
        }
        else
        {
#if defined(CONFIG_ONESN_ENABLE)
            mstar_project_ID_t ProjectID=GetProjectID();
            snprintf(strCommand,CMD_BUF,"config/onebin/model_%d/%s",ProjectID.Model_ID,MMAP_FILE_NAME);
#else
        pEnv=getenv(ENV_MAP_PATH);
        if (pEnv !=NULL)
        {
            UBOOT_DEBUG("Get MAP path from env =%s \n",pEnv);
            snprintf(strCommand,CMD_BUF,CONFIG_PATH,pEnv);
        }
        else
        {
            snprintf(strCommand,CMD_BUF,"%s/%s",CONFIG_PATH,MMAP_FILE_NAME);
        }
#endif
            UBOOT_ERROR("MDrv_MIU_Dram_ReadSize() is wrong!!\n");
            UBOOT_ERROR("Use the default path(%s)!!\n",strCommand);
            return ;
        }
        //update to bootargs
        snprintf(s8MmapPath,CMD_BUF,"%s=%s",MMAP_PATH,s8TmpBuf);
        set_bootargs_cfg(MMAP_PATH,s8MmapPath,0);
        //get mmap path for mboot
        //snprintf(strCommand,CMD_BUF,"%s/%s",CONFIG_PATH,s8TmpBuf);
    }
    UBOOT_DEBUG("MmapPath : %s\n",strCommand);
    UBOOT_TRACE("OK\n");
}

void decrypt_mmap(void)
{
#if (ENABLE_MODULE_SECURITY_BOOT==1)
    U8 u8Key[AES_KEY_LEN]="\0";
    if(strstr((char *)mmap_buffer,"#define")==NULL) //check cipher or plain
    {
        UBOOT_DEBUG("mmap.ini is cipher!!\n");
        GetAESKey(E_AES_BOOT_KEY,u8Key);
        if(Secure_AES_ECB_Decrypt((U32)mmap_buffer,mmap_buffer_size,u8Key)!=0)
        {
            UBOOT_ERROR("Decrypt Mmap fail\n");
        }
    }
#endif
    return;
}



int LoadMMAP(void)
{
    char strCommand[CMD_BUF]="\0";
    char * pEnv=NULL;
    UBOOT_TRACE("IN\n");

    if(bIsLoadMmapSuccess == FALSE)
    {
        UBOOT_ERROR("LoadMMAP fail last time, skip it!\n");
        return -1;
    }
    pEnv=getenv(ENV_MAP_PARTITION);
    if (pEnv !=NULL)
    {
        UBOOT_DEBUG("Get MAP partition from env =%s \n",pEnv);
        if(vfs_mount(pEnv)!=0)
        {
            UBOOT_ERROR("mount %s fail\n",pEnv);
            bIsLoadMmapSuccess = FALSE;
            return -1;
        }
    }
    else
    {
        if(vfs_mount(CONFIG)!=0)
        {
            UBOOT_ERROR("mount %s fail\n",CONFIG);
            bIsLoadMmapSuccess = FALSE;
            return -1;
        }
    }


    GetMmapPath(strCommand);
    mmap_buffer_size=vfs_getsize(strCommand);
    if(mmap_buffer_size==0)
    {
        UBOOT_ERROR("get the file size of %s fail\n",strCommand);
        bIsLoadMmapSuccess = FALSE;
        return -1;
    }

    mmap_buffer=malloc(mmap_buffer_size);
    UBOOT_DEBUG("mmmap_buffer at 0x%x\n",(unsigned int)mmap_buffer);
    if(mmap_buffer==NULL)
    {
        UBOOT_ERROR("malloc for mmap_buffer fail\n");
        return NULL;
    }

    if(vfs_read((char *)mmap_buffer,strCommand,0,mmap_buffer_size)!=0)
    {
        free(mmap_buffer);
        mmap_buffer=NULL;
        UBOOT_ERROR("read %s fail\n",strCommand);
        bIsLoadMmapSuccess = FALSE;
        return -1;
    }

    // if mmap is cipher , decrypt it.
    decrypt_mmap();

    UBOOT_TRACE("OK\n");
    return 0;
}

int _get_mmap(U8 *buffer, char *id, MMapInfo_s *mmapInfo)
{
    char *str=NULL;
    char *ptr=NULL;
    UBOOT_TRACE("IN\n");
    if((buffer==NULL)||(id==NULL)||(mmapInfo==NULL))
    {
        UBOOT_ERROR("One of the input parameters is null pointer\n");
        return -1;
    }
    UBOOT_DEBUG("@@id=%s\n",id);
    str=malloc(CMD_BUF);
    if(str==NULL)
    {
        UBOOT_ERROR("malloc for str fail\n");
        return -1;
    }

    memset(str,0,CMD_BUF);
    snprintf(str,CMD_BUF,"%s_ADR",id);
    UBOOT_DEBUG("str=%s\n",str);
    ptr=pattern_search((char *)mmap_buffer,mmap_buffer_size,str);
    if(ptr!=NULL)
    {
        ptr=pattern_search(ptr,mmap_buffer_size-((unsigned int)ptr-(unsigned int)mmap_buffer+1),"0x");
    }
    else
    {
        UBOOT_DEBUG("get addr from mmap fail\n");
        free(str);
        return -1;
    }
    memset(str,0,CMD_BUF);
    snprintf(str,NUM_LEN,"%s\n",ptr);
    UBOOT_DEBUG("str=%s\n",str);
    mmapInfo->u64Addr=(U64)simple_strtoull(str,NULL,16);

    memset(str,0,CMD_BUF);
    snprintf(str,CMD_BUF,"%s_LEN",id);
    UBOOT_DEBUG("str=%s\n",str);
    ptr=pattern_search((char *)mmap_buffer,mmap_buffer_size,str);
    if(ptr!=NULL)
    {
        ptr=pattern_search(ptr,mmap_buffer_size-((unsigned int)ptr-(unsigned int)mmap_buffer+1),"0x");
    }
    if(ptr==NULL)
    {
        UBOOT_ERROR("get length from mmap fail\n");
        free(str);
        return -1;
    }
    memset(str,0,CMD_BUF);
    snprintf(str,NUM_LEN,"%s\n",ptr);
    UBOOT_DEBUG("str=%s\n",str);
    mmapInfo->u32Size=(U32)simple_strtol(str,NULL,16);

    //For an example
    //#define E_MMAP_ID_PM51_USAGE_MEM_MEMORY_TYPE                   (MIU0 | TYPE_NONE | UNCACHE)
    memset(str,0,CMD_BUF);
    snprintf(str,CMD_BUF,"%s_MEMORY_TYPE",id);
    UBOOT_DEBUG("str=%s \n",str);
    ptr=pattern_search((char *)mmap_buffer,mmap_buffer_size,str);
    if(ptr!=NULL)
    {
        ptr=pattern_search(ptr,mmap_buffer_size-((unsigned int)ptr-(unsigned int)mmap_buffer+1),"(");
    }
    else
    {
        UBOOT_ERROR("get memory type from mmap fail\n");
        free(str);
        return -1;
    }
    ptr++;
    if(memcmp(ptr, "MIU0", strlen("MIU0"))==0)
    {
        UBOOT_DEBUG("In MIU0\n");
        mmapInfo->u32miuID=0;
    }
    else if(memcmp(ptr, "MIU1", strlen("MIU1"))==0)
    {
        UBOOT_DEBUG("In MIU1\n");
        mmapInfo->u32miuID=1;
    }
    else //if(memcmp(ptr, "MIU2", strlen("MIU2"))==0)
    {
        UBOOT_DEBUG("In MIU2\n");
        mmapInfo->u32miuID=2;
    }
    //else
    //{
    //    UBOOT_DEBUG("In MIU3\n");
    //    mmapInfo->u32miuID=3;
    //}

    mmapInfo->u32Align=0;
    mmapInfo->u32gID=0;
    mmapInfo->u8Layer=0;

    free(str);
    UBOOT_TRACE("OK\n");
    return 0;
}

int list_mmap_id(void)
{
    #define PATTERN_HEAD "/* E_MMAP_ID"
    #define PATTERN_TAIL "*/"
    char *pS=NULL,*pE=NULL;
    char *pBuf=NULL;
    char *strBuf=NULL;
    unsigned int buf_size=mmap_buffer_size;
    UBOOT_TRACE("IN\n");
    if(init_mmap()!=0)
    {
        UBOOT_ERROR("init mmap fail\n");
        return -1;
    }
    strBuf=malloc(CMD_BUF);
    if(strBuf==NULL)
    {
        UBOOT_ERROR("malloc for string buf fail\n");
        return -1;
    }
    memset(strBuf,0,CMD_BUF);

    pS=(char *)mmap_buffer;
    pBuf=pS;
    while(buf_size)
    {
        pS=pattern_search((char *)pBuf,buf_size,PATTERN_HEAD);
        if(pS!=NULL)
        {
            buf_size=buf_size-((unsigned int)pS-(unsigned int)pBuf+1);
            pE=pattern_search((char *)pS,buf_size,PATTERN_TAIL);
            buf_size=buf_size-((unsigned int)pE-(unsigned int)pS+1);
        }
        if((pS==NULL)||(pE==NULL))
        {
            break;
        }
        pS+=3;//+3? skip  "/* "
        pBuf=pE+strlen(PATTERN_TAIL);
        snprintf(strBuf,((unsigned int)pE-(unsigned int)pS),"%s",pS);
        UBOOT_INFO("%s\n",strBuf);
    }
    free(strBuf);
    UBOOT_TRACE("OK\n");
    return 0;

}

char * CMA_parsing(char *buffer,MMapInfo_s *mmapInfo,MS_BOOL bIsMItype)
{
    char *ptr=buffer;
    char str[CMD_BUF]={0};
    char *tmpPtr=NULL;
    int line=10;

    UBOOT_TRACE("IN\n");
    // get the CMA case
    ptr=strstr(ptr,"| CMA");
    if(ptr!=NULL)
    {
        // back to   E_MMAP_ID_  MI_ start
        while(line)
        {
            ptr--;
            if(*ptr==10) //LF Backspace
            {
                line--;
            }
        }
        if(bIsMItype==TRUE)
        {
            ptr=strstr(ptr,"/* MI_");
            if(ptr!=NULL)
            {
                ptr+=6; // skip  "/* MI_"
            }
            else
            {
                UBOOT_ERROR("can not get MI_ !\n");
                return NULL;
            }
        }
        else
        {
            ptr=strstr(ptr,"/* E_MMAP_ID_");
            if(ptr!=NULL)
            {
                ptr+=13; // skip "/* E_MMAP_ID_"
            }
            else
            {
                UBOOT_ERROR("can not get E_MMAP_ID_ !\n");
                return NULL;
            }
        }

        //get mmap Name
        tmpPtr=strchr(ptr,' ');
        memcpy(mmapInfo->u8Name,ptr,tmpPtr-ptr);

        // get mmap layer
        if(ptr!=NULL)
        {
            ptr=strstr(ptr,"_LAYER");
            if(ptr!=NULL)
            {
                ptr+=6;//skip "_LAYER"
                while(*ptr==' ')
                {
                    ptr++;
                }
                mmapInfo->u8Layer=(U8)(*ptr-'0');
            }
        }

        //get mmap address
        if(ptr!=NULL)
        {
            ptr=strstr(ptr,"_ADR");
            if(ptr!=NULL)
            {
                ptr=strstr(ptr,"0x");
                snprintf(str,NUM_LEN,"%s\n",ptr);
                UBOOT_DEBUG("str=%s\n",str);
                mmapInfo->u64Addr= simple_strtoull(str, NULL, 16);
            }
        }

        //get mmap length
        if(ptr!=NULL)
        {
            ptr=strstr(ptr,"_LEN");
            if(ptr!=NULL)
            {
                ptr=strstr(ptr,"0x");
                snprintf(str,NUM_LEN,"%s\n",ptr);
                UBOOT_DEBUG("str=%s\n",str);
                mmapInfo->u32Size= simple_strtoul(str, NULL, 16);
            }
        }

        //get mmap miuID
        if(ptr!=NULL)
        {
            ptr=strchr(ptr,'(');
            ptr++;//skip '('
        }
        if(ptr!=NULL)
        {
            if(memcmp(ptr, "MIU0", strlen("MIU0"))==0)
            {
                UBOOT_DEBUG("In MIU0\n");
                mmapInfo->u32miuID=0;
            }
            else if(memcmp(ptr, "MIU1", strlen("MIU1"))==0)
            {
                UBOOT_DEBUG("In MIU1\n");
                mmapInfo->u32miuID=1;
            }
            else if(memcmp(ptr, "MIU2", strlen("MIU2"))==0)
            {
                UBOOT_DEBUG("In MIU2\n");
                mmapInfo->u32miuID=2;
            }
            //else if(memcmp(ptr, "MIU3", strlen("MIU3"))==0)
            //{
            //    UBOOT_DEBUG("In MIU3\n");
            //    mmapInfo->u32miuID=3;
            //}
            else
            {
                UBOOT_ERROR("can not get miuID!\n");
                return NULL;
            }
        }

        //get mmap CMA HID
        if(ptr!=NULL)
        {
            ptr=strstr(ptr,"HID");
            if(ptr!=NULL)
            {
                ptr+=3; //skip "HID"
                while(*ptr==' ')
                {
                    ptr++;
                }
            }
        }
        memset(str,0,sizeof(str));
        snprintf(str,3,"%s\n",ptr);
        UBOOT_DEBUG("str : %s\n",str);
        mmapInfo->u32CMA_HID= simple_strtoul(str, NULL, 10);

        // print mmap information
        UBOOT_DEBUG("Name    = %s\n",mmapInfo->u8Name);
        UBOOT_DEBUG("Layer    = %d\n",mmapInfo->u8Layer);
        UBOOT_DEBUG("Addr    = 0x%llx\n",mmapInfo->u64Addr);
        UBOOT_DEBUG("Size    = 0x%x\n",mmapInfo->u32Size);
        UBOOT_DEBUG("miuID   = %d\n",mmapInfo->u32miuID);
        UBOOT_DEBUG("CMA HID = %d\n",mmapInfo->u32CMA_HID);
    }
    UBOOT_TRACE("OK\n");
    return ptr;
}

void del_bootargs_CMA(MS_BOOL bDontSaveEnv)
{
    char CMAInfo[CMD_BUF]={0};
    int index=0;
    UBOOT_TRACE("IN\n");

    while(1)
    {
        sprintf(CMAInfo,"CMA%d",index++);
        if(del_bootargs_cfg(CMAInfo,bDontSaveEnv)!=0)
        {
            break;
        }
    }
    UBOOT_TRACE("OK\n");
}
#ifdef CONFIG_UMA_WITH_NARROW
void setDMAzoneargs(void)
{
    char *mmap_ptr = (char*)mmap_buffer;
    char *dma_ptr = NULL;
    char DMAzone_info[CMD_BUF]={0};
    char DMA_info[10] ={0};
    unsigned long long dma_length = 0;
    dma_ptr = strstr(mmap_ptr,"E_MMAP_ID_DMA_ZONE_MIU0");
    if(dma_ptr == NULL)
        return ;
    dma_ptr = strstr(mmap_ptr,"E_MMAP_ID_DMA_ZONE_MIU0_LEN");
    if(dma_ptr != NULL)
    {
        dma_ptr = strstr(dma_ptr,"0x");
        if(dma_ptr != NULL)
        {
            dma_length = simple_strtoull(dma_ptr,NULL,16);
            sprintf(DMAzone_info,"DMA_SIZE=0x%llx",dma_length);
            memcpy(DMA_info,DMAzone_info,8);
            del_bootargs_cfg(DMAzone_info,0);
            set_bootargs_cfg(DMA_info,DMAzone_info,1);
        }
    }

}
#endif
void setCMA2bootargs(void)
{
#define CMA_LAYER_CHECK "MIU_INTERVAL64"
    int index=0;
    char CMAInfo[CMD_BUF]={0};
    char CMAIndex[10]={0};
    char u8TargetLayer=0;
    char * pEnv=NULL;
    MMapInfo_s mmapInfo;
    MS_BOOL bIsMItype=FALSE;
    char *buffer = (char*)mmap_buffer;
    UBOOT_TRACE("IN\n");
    pEnv = getenv("MAP_TYPE");
    if(pEnv!=NULL && strcmp(pEnv, "MI")==0) //mi3.0 format
   {
        bIsMItype=TRUE;
    }
    //clean the CMA
    del_bootargs_CMA(0);

    //check target CMA layer
    u8TargetLayer=(strstr(buffer,CMA_LAYER_CHECK)!=NULL)?0:2;
    UBOOT_DEBUG("Target CMA Layer : %d\n",u8TargetLayer);

    //set CMA from mmap.ini
    while(1)
    {
        memset(&mmapInfo,0,sizeof(mmapInfo));
        buffer = CMA_parsing(buffer,&mmapInfo,bIsMItype);
        if(buffer != NULL)
        {
            if(mmapInfo.u8Layer==u8TargetLayer)
            {
                sprintf(CMAInfo,"CMA%d=%s,miu=%d,hid=%d,sz=0x%x,st=0x%llx",index++,mmapInfo.u8Name,
                                 mmapInfo.u32miuID,mmapInfo.u32CMA_HID,mmapInfo.u32Size,mmapInfo.u64Addr);
                memcpy(CMAIndex,CMAInfo,5);
                UBOOT_DEBUG("%s\n",CMAInfo);
                UBOOT_DEBUG("====================\n");
                set_bootargs_cfg(CMAIndex,CMAInfo,1);
            }
        }
        else
        {
            break;
        }
    }
    UBOOT_TRACE("OK\n");
}
