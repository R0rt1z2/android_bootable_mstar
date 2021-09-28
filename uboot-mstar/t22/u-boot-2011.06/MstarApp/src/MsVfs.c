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
#include <watchdog.h>
#include <command.h>
#include <malloc.h>   /* for free() prototype */
#include <stdio.h>
#include <post.h>
#include <ShareType.h>
#include <MsDebug.h>
#include <MsVfs.h>
#include <MsUtility.h>
#include <CusConfig.h>
#include <MsTypes.h>

#if (ENABLE_MODULE_VFS == 1)
#define VOLUME_NAME_LEN     32

//-------------------------------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
static char volName[VOLUME_NAME_LEN] = {0};

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static BOOLEAN bVfsReady=FALSE;
//-------------------------------------------------------------------------------------------------
//  Extern Functions
//-------------------------------------------------------------------------------------------------
extern int snprintf(char *str, size_t size, const char *fmt, ...);
extern void ubi_get_part_name(char *name);
extern void ubifs_get_sb_name(char *name);

//-------------------------------------------------------------------------------------------------
//  Private Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

#if (defined(CONFIG_UBI) && !defined(CONFIG_NAND_FTL))
int vfs_mount(char *volume)
{
    char buffer[CMD_BUF]="\0";
    char last_ubi_partname[32];
    char last_sb_name[32];
    int ret=-1;
    UBOOT_TRACE("IN\n");

    if(volume==NULL)
    {
        bVfsReady=FALSE;
        UBOOT_ERROR("Input parameter 'volume' is a null pointer\n");
        return -1;
    }

        
	memset(last_ubi_partname, 0, 32);
	memset(last_sb_name,0,32);
	ubi_get_part_name(last_ubi_partname);
	ubifs_get_sb_name(last_sb_name);

	if( strcmp(last_sb_name,volume) == 0)
    {
		UBOOT_DEBUG("%s is already mount!!\n",volume);
		return 0;
	}
     
    ret=vfs_umount();
    if(ret==-1)
    {
        bVfsReady=FALSE;   
        UBOOT_ERROR("umount previous filesystem fail\n");
        return -1;
    }
    
    //Assume this volume is in UBI partition
    if(strcmp(last_ubi_partname, UBIPART) != 0)
	{
        memset(buffer,0,CMD_BUF);    
        snprintf(buffer,CMD_BUF,"ubi part %s",UBIPART);
        ret=run_command(buffer,0);
    }
    else
        ret = 0;

    if(ret!=-1)
    {
        memset(buffer,0,CMD_BUF);
        snprintf(buffer,CMD_BUF,"ubifsmount %s",volume);
        UBOOT_DEBUG("cmd=%s\n",buffer);
        ret=run_command(buffer,0);
        if(ret==-1)
        {
            UBOOT_ERROR("ubifsmount fail (Assume this volume is in UBI(or ubi) partition)\n");
        }
        else
        {
            memset(volName, 0, sizeof(volName));
            strncpy(volName, volume, sizeof(volName)-1);
            bVfsReady=TRUE;              
            UBOOT_TRACE("OK\n");
            return ret;
        }
    } 
    return ret;
}

int vfs_umount(void)
{
    int ret = 0;
    UBOOT_TRACE("IN\n");
    if(bVfsReady==TRUE)
    {
        ret = run_command("ubifsumount",0);
    }
    if(ret==0)
    {
        UBOOT_TRACE("OK\n");
    }
    else
    {
        UBOOT_ERROR("unmount fail\n");
    }
    memset(volName, 0, sizeof(volName));
    bVfsReady=FALSE;  
  	return ret;
}



U32 vfs_getsize(char *filedir)
{
    int ret = -1;
	U32 filesize = 0;
    char *buffer=NULL;
    UBOOT_TRACE("IN\n"); 
    if(filedir==NULL)
    {
        UBOOT_ERROR("filedir is a null pointer\n");
        return FALSE;
    }
    if(bVfsReady==FALSE)
    {
        UBOOT_ERROR("filesystem is not mounted\n");
        return FALSE;
    }
    buffer=malloc(CMD_BUF);
    if(buffer==NULL)
    {
        UBOOT_ERROR("malloc buffer fail\n");
        return FALSE;
    }
    memset(buffer,0,CMD_BUF);
    
    snprintf(buffer, CMD_BUF, "ubifsfilesize %s 0x%x",filedir,&filesize);
    UBOOT_DEBUG("cmd=%s\n",buffer);
    ret = run_command(buffer, 0);
    if(ret==-1)
    {
        UBOOT_ERROR("ubifsfilesize %s fail\n",filedir);
        filesize=0;
    }
    else
    {
        UBOOT_TRACE("OK\n");     
    }
    free(buffer);
	return filesize;
}
int vfs_read(void* addr,char* filedir,unsigned int offset,unsigned int size )
{
    int ret = -1;
    char *buffer=NULL;
    UBOOT_TRACE("IN\n");
    
    if(addr==NULL)
    {
        UBOOT_ERROR("addr is a null pointer\n");
        return -1;
    }
    
    if(filedir==NULL)
    {
        UBOOT_ERROR("filedir is a null pointer\n");
        return -1;
    }

    if(bVfsReady==FALSE)
    {
        UBOOT_ERROR("filesystem is not mounted\n");
        return -1;
    }
    buffer=malloc(CMD_BUF);
    if(buffer==NULL)
    {
        UBOOT_ERROR("malloc buffer fail\n");
        return -1;
    }
    memset(buffer,0,CMD_BUF);
    snprintf(buffer, CMD_BUF, "ubifspartload %x %s %x %x", (unsigned int)addr, filedir, offset,size);
    UBOOT_DEBUG("cmd=%s\n",buffer);
    ret = run_command(buffer, 0);
    if(ret==-1)
    {
        UBOOT_ERROR("ubifspartload %s fail\n",filedir);
    }
    else
    {
        UBOOT_TRACE("OK\n"); 
    }
    free(buffer);
	return ret;  
}

int vfs_write(void* addr,char* filedir,unsigned int offset,unsigned int size )
{
    UBOOT_TRACE("IN\n");
    
    if(addr==NULL)
    {
        UBOOT_ERROR("addr is a null pointer\n");
        return -1;
    }
    if(filedir==NULL)
    {
        UBOOT_ERROR("filedir is a null pointer\n");
        return -1;
    }

    if(bVfsReady==FALSE)
    {
        UBOOT_ERROR("filesystem is not mounted\n");
        return -1;
    }
    UBOOT_ERROR("Not implement now\n");
    return -1;
}


#else

#if defined(CONFIG_DUAL_SYSTEM)
#include <MsBoot.h>

int vfs_mount(char *volume)
{
    int ret = 0;
    char vfs_volName[32] = {0};
    UBOOT_TRACE("IN\n");

    memset(vfs_volName, 0, sizeof(vfs_volName));
    strncpy(vfs_volName, volume, 31);
    if (0 != read_bootflag())
    {
        strcat(vfs_volName, DUAL_SYSTEM_PARTITION_SUFFIX);
    }

    memset(volName, 0, sizeof(volName));
    strncpy(volName, vfs_volName, sizeof(volName)-1);
    UBOOT_INFO("mmc change mount : %s\n", volName);
    bVfsReady=TRUE;
    UBOOT_TRACE("OK\n");
    return ret;
}
#else
int vfs_mount(char *volume)
{
    int ret = 0;
    UBOOT_TRACE("IN\n");
    if (strlen(volume) >= VOLUME_NAME_LEN)
    {
        UBOOT_ERROR("invalid volume name : %s\n", volume);
        return -1;
    }
    memset(volName, 0, sizeof(volName));
    strncpy(volName, volume, sizeof(volName)-1);
    UBOOT_INFO("mmc change mount : %s\n",volName);
    bVfsReady=TRUE;
    UBOOT_TRACE("OK\n");
	return ret;
}
#endif

int vfs_umount(void)
{
    int ret = 0;
    UBOOT_TRACE("IN\n");
    memset(volName, 0, sizeof(volName));
    bVfsReady=FALSE;
    UBOOT_TRACE("OK\n");
	return ret;
}

U32 vfs_getsize(char *filedir)
{
	U32 filesize = 0;
    int ret = 0;
    char *buffer=NULL;
    
    UBOOT_TRACE("IN\n"); 

    if(filedir==NULL)
    {
        UBOOT_ERROR("filedir is a null pointer\n");
        return FALSE;
    }

    if(bVfsReady==FALSE)
    {
        UBOOT_ERROR("filesystem is not mounted\n");
        return FALSE;
    }
    
    buffer=malloc(CMD_BUF);
    if(buffer==NULL)
    {
        UBOOT_ERROR("malloc buffer fail\n");
        return FALSE;
    }
    memset(buffer,0,CMD_BUF);    
    //snprintf(cmd, CMD_BUF, "ext4filesize mmc 0:%s %s",mmc_partition,filedir);
    #if(defined(CONFIG_NAND_FTL))
    snprintf(buffer, CMD_BUF, "ext4filesize nand 0:%s %s %x",volName,filedir,(U32)&filesize);
    #else
    snprintf(buffer, CMD_BUF, "ext4filesize mmc 0:%s %s %x", volName,filedir,(U32)&filesize);
    #endif
    UBOOT_DEBUG("cmd=%s\n",buffer);    
    ret=run_command(buffer, 0);
    if(ret == -1)
    {
        UBOOT_ERROR("ext4filesize mmc fail, try again! \n");
        ret=run_command(buffer, 0);
        if(ret == -1)
        {
            UBOOT_ERROR("ext4filesize mmc fail\n");
            filesize=0;
        }
    }
    else
    {
        UBOOT_TRACE("OK\n");
    }
    free(buffer);
	return filesize;
}

int vfs_read(void* addr,char* filedir,unsigned int offset,unsigned int size )
{
    int ret = -1;
    char *buffer=NULL;
    
    UBOOT_TRACE("IN\n");

    if(addr==NULL)
    {
        UBOOT_ERROR("addr is a null pointer\n");
        return -1;
    }
    
    if(filedir==NULL)
    {
        UBOOT_ERROR("filedir is a null pointer\n");
        return -1;
    }

    if(bVfsReady==FALSE)
    {
        UBOOT_ERROR("filesystem is not mounted\n");
        return -1;
    }
    
    buffer=malloc(CMD_BUF);
    if(buffer==NULL)
    {
        UBOOT_ERROR("malloc buffer fail\n");
        return -1;
    }
    memset(buffer,0,CMD_BUF);
    #if(defined(CONFIG_NAND_FTL))
    snprintf(buffer, CMD_BUF, "ext4partload nand 0:%s %x %s %x %x",volName, (unsigned int)addr, filedir, offset,size);
    #else
    snprintf(buffer, CMD_BUF, "ext4partload mmc 0:%s %x %s %x %x",volName, (unsigned int)addr, filedir, offset,size);
    #endif
    UBOOT_DEBUG("cmd=%s\n",buffer); 
    ret = run_command(buffer, 0);
    if(ret==-1)
    {
        UBOOT_ERROR("ext4load mmc fail, try again!\n");
        ret = run_command(buffer, 0);
        if(ret==-1)
        {
          UBOOT_ERROR("ext4load mmc fail\n");
        }
    }
    else
    {
      UBOOT_TRACE("OK\n");
    }
    free(buffer);
	return ret;  
}

int vfs_write(void* addr,char* filedir,unsigned int offset,unsigned int size )
{
    UBOOT_TRACE("IN\n");
    
    if(addr==NULL)
    {
        UBOOT_ERROR("addr is a null pointer\n");
        return -1;
    }
    
    if(filedir==NULL)
    {
        UBOOT_ERROR("filedir is a null pointer\n");
        return -1;
    }

    if(bVfsReady==FALSE)
    {
        UBOOT_ERROR("filesystem is not mounted\n");
        return -1;
    }
    UBOOT_ERROR("Not implement now\n");
    return -1;
}

#endif


int vfs_mount_by_abspath(char *abspath, char *subdir_buf, int buf_size)
{
    int ret = 0, plen;
    char *path, *partition, *partition_s;

    UBOOT_TRACE("IN\n");
    if ((abspath == NULL) || (subdir_buf == NULL) || (buf_size <= 0))
    {
        return -1;
    }
    UBOOT_DEBUG("abspath   = %s\n", abspath);

    path = abspath;
    if (path[0] != '/')
    {
        UBOOT_ERROR("invalid abspath(%s), should start with '/'\n", abspath);
        return -1;
    }
    while (*path== '/')
    {
        path++;
    }
    partition_s = path;
    memset(subdir_buf, 0, buf_size);

    path = strstr(path,"/");
    if (path != NULL)
    {
        // Case : abspath --> /tvconfig/boot0.jpg
        plen = path - partition_s;
        partition = malloc(plen + 1);
        if (partition == NULL)
        {
            UBOOT_ERROR("can not get 0x%x byte from malloc\n", plen+1);
            return -1;
        }
        memset(partition, 0, plen+1);
        strncpy(partition, partition_s, plen);
    
        path += 1;  // skip '/'
        plen  = strlen(path);
        if (plen >= buf_size)
        {
            UBOOT_ERROR("subdir(%s) is too long, (len:%d >= buf_size:%d)\n", path, plen, buf_size);
            free(partition);
            return -1;
        }
        strncpy(subdir_buf, path, plen);

        if (strncmp(partition, "firetv_odm", 10) == 0) {
            ret = vfs_mount(ODM);
            UBOOT_DEBUG("partition = %s\n", ODM);
        } else {
            ret = vfs_mount(partition);
            UBOOT_DEBUG("partition = %s\n", partition);
        }

        UBOOT_DEBUG("subdir    = %s\n", subdir_buf);

        free(partition);
    }
    else
    {
        // Case : abspath --> /tvconfig
        ret = vfs_mount(partition_s);
        UBOOT_DEBUG("partition = %s\n", partition_s);
    }

    UBOOT_TRACE("OK\n");
    return ret;
}


char *vfs_get_volname(void)
{
    return volName;
}
#endif

