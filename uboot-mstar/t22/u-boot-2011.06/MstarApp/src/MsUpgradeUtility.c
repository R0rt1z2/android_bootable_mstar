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
#include <MsUpgradeUtility.h>
#include <CusUpgrade.h>
#include <MsUpgrade.h>
#include <config.h>
#include <MsTypes.h>
#include <CusConfig.h>
#include <MsUboot.h>
#include <MsVfs.h>
#include <MsDebug.h>
#include <MsUtility.h>
#include <MsBoot.h>
#include <MsAppCB.h>
#if defined (CONFIG_R2_BRINGUP) || defined (CONFIG_AMP_TRUSTZONE_ENABLE)
#include <MsSecureBoot.h>
#include <drvBDMA.h>
#include <MsSecureTee.h>
#endif
#include <CusConfig.h>
//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#ifdef CONFIG_UPGRADE_WITHOUT_FAIL_HANDLER
#define DEFAULT_SCRIPT_SIZE_FOR_UPGRADE_IMAGE 0x4000
#else // #ifdef CONFIG_UPGRADE_WITH_FAIL_HANDLER
#define DEFAULT_SCRIPT_SIZE_FOR_UPGRADE_IMAGE 0x10000
#endif
#define MAX_DONTWRITE_COUNT 20
#define MAX_FORCEWRIE_COUNT MAX_DONTWRITE_COUNT
#ifdef CONFIG_UPGRADE_WITHOUT_FAIL_HANDLER
#define SCRIPT_FILE_CRC_OFFSET 24
#else // #ifdef CONFIG_UPGRADE_WITH_FAIL_HANDLER
#define SCRIPT_FILE_CRC_OFFSET 32
#endif
#define FULL_FILE_CRC_OFFSET 20
#define DEFAULT_FULL_FILE_CRC_SEGMENT_SIZE 0x200000
#define COMPARE_LENGTH 16 //OAD bin might be attach the first 16 bytes to the end of file. Decide it is encrypted or not by comparison.
#define UPGRADE_FILE_MAGIC_STRING_LENGYTH 8

#define MAX_DEVICE 3
#define MAX_PARTITION 5



#define FULL_AP_BIN_PATH        "/MstarUpgrade_full.bin" // always fo full upgrade


#if defined (CONFIG_R2_BRINGUP) || defined (CONFIG_AMP_TRUSTZONE_ENABLE)
#ifdef CONFIG_UBI
#define TEE_PART_OFFSET 0x400000
#define TEE_PART_SIZE 0x400000
#define MMC_VADDR 0x84800000
#else
#define TEE_PART_OFFSET 0x600000
#define TEE_PART_SIZE 0x600000
#define MMC_VADDR 0x51100000
#endif
#define TEE_HEADER_LEN 0x80
#define CRC_LEN 4
#endif

#define DONT_OVERWIRTE_COMMAND_LEN 32
//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
#if(ENABLE_MODULE_DONT_OVERWRITE==1)
static BOOLEAN dont_overwrite_flag=0;
static U8 dont_overwrite_count=0;
static char *pdont_overwrite_partition[MAX_DONTWRITE_COUNT];
static BOOLEAN force_overwrite_flag=FALSE;
static U8 force_overwrite_count=0;
static char *pforce_overwrite_partition[MAX_FORCEWRIE_COUNT];
static IsStorageEmpty fpIsStorageEmpty=NULL;
#endif
static int g_device = 0;
static int g_partition = 0;
static unsigned int scriptSizeOfUpgradeImage=0;
static EN_UPDATE_MODE s_UpdateMode = EN_UPDATE_MODE_NONE;
static char s_UpgradeFsMount[32]=OAD;

#if defined (CONFIG_R2_BRINGUP) || defined (CONFIG_AMP_TRUSTZONE_ENABLE)
char TeeMagicID[MAGIC_ID_LEN]="MSTARUPGRADEBIN";
#endif

//the first item uses the full compare
//so the 'nand erase' needs to be the first one.
#if(ENABLE_MODULE_DONT_OVERWRITE==1)
char dont_overwrite_default_table[][DONT_OVERWIRTE_COMMAND_LEN]={
    "nand erase",
    "nand erase.chip",
    "mmc rmgpt",
    "ftl rmgpt",
    "create",
    "remove",
#ifdef CONFIG_USB_DISK_UPGRADE_LOADER
    "reset",
#endif
    "certificate"
};
#endif
//-------------------------------------------------------------------------------------------------
//  Extern Functions
//-------------------------------------------------------------------------------------------------
extern int snprintf(char *str, size_t size, const char *fmt, ...);

//-------------------------------------------------------------------------------------------------
//  Private Functions
//-------------------------------------------------------------------------------------------------
#if(ENABLE_MODULE_DONT_OVERWRITE==1)
static int check_not_empty(void);
static BOOLEAN get_dont_overwrite_flag(void);
static BOOLEAN is_dont_overwrite_cmd(char *str);
static BOOLEAN get_force_overwrite_flag(void);
static BOOLEAN is_force_overwrite_cmd(char *str);
#endif

//-------------------------------------------------------------------------------------------------
//  Public Functions
//-------------------------------------------------------------------------------------------------
#if(ENABLE_MODULE_DONT_OVERWRITE==1)
int do_force_overwirte_init(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    UBOOT_TRACE("IN\n");
    force_overwrite_count=MAX_DONTWRITE_COUNT;
    while(force_overwrite_count--){
        pforce_overwrite_partition[force_overwrite_count]=NULL;
    }
    force_overwrite_count=0;
    UBOOT_TRACE("OK\n");
    return 0;
}
int do_force_overwrite(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    UBOOT_TRACE("IN\n");

    if(argc!=2){
        cmd_usage(cmdtp);
        return -1;
    }

    if(argv[1]==NULL)
    {
        cmd_usage(cmdtp);
        return -1;
    }

    UBOOT_INFO("argv[1]=%s\n",argv[1]);

    if(force_overwrite_count==MAX_FORCEWRIE_COUNT){
        UBOOT_ERROR("The number os 'fore overwrite' is up to max vaulue:%d\n",MAX_FORCEWRIE_COUNT);
        return -1;
    }

    if(force_overwrite_flag!=TRUE){
        UBOOT_DEBUG("The function of 'force overwrite' is not enable\n");
        UBOOT_TRACE("OK\n");
        return 0;
    }

    pforce_overwrite_partition[force_overwrite_count]=malloc(strlen(argv[1])+1);
    if(pforce_overwrite_partition[force_overwrite_count]==NULL){
        UBOOT_INFO("malloc for %s fail\n",argv[1]);
        return -1;
    }
    memset(pforce_overwrite_partition[force_overwrite_count],0,strlen(argv[1])+1);
    strncpy(pforce_overwrite_partition[force_overwrite_count],argv[1], strlen(argv[1]));

    force_overwrite_count++;
        UBOOT_INFO("force_overwrite_count %d \n",force_overwrite_count);

    UBOOT_TRACE("OK\n");

    return 0;
}

int do_force_overwrite_free(void)
{
    UBOOT_TRACE("IN\n");
    if(force_overwrite_flag!=TRUE){
        UBOOT_DEBUG("The function of 'force overwrite' is not enable\n");
        UBOOT_TRACE("OK\n");
        return 0;
    }
    force_overwrite_flag=FALSE;
    while(force_overwrite_count--){
        if(pforce_overwrite_partition[force_overwrite_count]!=NULL){
         UBOOT_DEBUG("count=%d, %s\n",(unsigned int)force_overwrite_count,pforce_overwrite_partition[force_overwrite_count]);
         free(pforce_overwrite_partition[force_overwrite_count]);
        }
    }
    UBOOT_TRACE("OK\n");
    return 0;
}

void do_force_overwrite_disable(void)
{
    UBOOT_TRACE("IN\n");
    force_overwrite_flag=FALSE;
    UBOOT_TRACE("OK\n");
}

void do_force_overwrite_enable(void)
{
    UBOOT_TRACE("IN\n");
    force_overwrite_flag=TRUE;
    UBOOT_TRACE("OK\n");
}

static BOOLEAN get_force_overwrite_flag(void)
{
    UBOOT_TRACE("IN\n");
    UBOOT_TRACE("OK\n");
    return force_overwrite_flag;
}

static BOOLEAN is_force_overwrite_cmd(char *str)
{
    U8 count=0;
    char *p=NULL;
    UBOOT_TRACE("IN\n");
    if(str==NULL)
    {
        UBOOT_ERROR("The input parameter 'str' is a null pointer\n");
        return FALSE;
    }

    if(get_force_overwrite_flag()==TRUE)
    {
        for(count=0;count<force_overwrite_count;count++)
        {
            p=strstr(str,pforce_overwrite_partition[count]);
            if(p!=NULL)
            {
                UBOOT_DEBUG("parttion[%d]: %s\n",(unsigned int)count, pforce_overwrite_partition[count]);
                UBOOT_DEBUG("input string : %s\n",str);
                UBOOT_INFO("\033[0;33m force to execute the cmd:%s\033[0m\n",str);            
                return TRUE;
            }
        }
    }    
    UBOOT_DEBUG("don't force to execute this cmd:%s\n",str);
    UBOOT_TRACE("OK\n");
    return FALSE; //don't force this command to be executed
}

int do_dont_overwrite_init(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    UBOOT_TRACE("IN\n");
    int i;
    if(argc!=1){
        cmd_usage(cmdtp);
        return -1;
    }

    dont_overwrite_count=MAX_DONTWRITE_COUNT;
    while(dont_overwrite_count--){
        pdont_overwrite_partition[dont_overwrite_count]=NULL;
    }
    dont_overwrite_count=0;

    for (i=0;i<(sizeof(dont_overwrite_default_table)/DONT_OVERWIRTE_COMMAND_LEN);i++)
    {
        pdont_overwrite_partition[dont_overwrite_count]=malloc(strlen(dont_overwrite_default_table[i])+1);
        if(pdont_overwrite_partition[dont_overwrite_count]==NULL){
            UBOOT_ERROR("malloc for '%s' fail\n",dont_overwrite_default_table[i]);
            return -1;
        }
        memset(pdont_overwrite_partition[dont_overwrite_count],0,strlen(dont_overwrite_default_table[i])+1);
        strncpy(pdont_overwrite_partition[dont_overwrite_count],dont_overwrite_default_table[i], strlen(dont_overwrite_default_table[i])+1);
        dont_overwrite_count++;
    }

    UBOOT_TRACE("OK\n");

    return 0;
}

int do_dont_overwrite_switch(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    UBOOT_TRACE("IN\n");

    if(argc!=2)
    {
        cmd_usage(cmdtp);
        return -1;
    }

    extern int SKIP_DONT_OVERWRITE;
    if(strcmp(argv[1], "disable")==0)
    {
        SKIP_DONT_OVERWRITE = 1;
        UBOOT_INFO("\n\nDisable dont overwrite function\n\n");
    }
    else
    {
        SKIP_DONT_OVERWRITE = 0;
        UBOOT_INFO("\n\nEnable dont overwrite function\n\n");
    }

    UBOOT_TRACE("OK\n");
    return 0;
}

int do_dont_overwrite(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    UBOOT_TRACE("IN\n");

    if(argc!=2){
        cmd_usage(cmdtp);
        return -1;
    }

    if(argv[1]==NULL)
    {
        cmd_usage(cmdtp);
        return -1;
    }

    UBOOT_DEBUG("argv[1]=%s\n",argv[1]);

    if(dont_overwrite_count==MAX_DONTWRITE_COUNT){
        UBOOT_ERROR("The number os 'dont overwrite' is up to max vaulue:%d\n",MAX_DONTWRITE_COUNT);
        return -1;
    }

    if(dont_overwrite_flag!=TRUE){
        UBOOT_DEBUG("The function of 'dont overwrite' is not enable\n");
        UBOOT_TRACE("OK\n");
        return 0;
    }

    pdont_overwrite_partition[dont_overwrite_count]=malloc(strlen(argv[1])+1);
    if(pdont_overwrite_partition[dont_overwrite_count]==NULL){
        UBOOT_ERROR("malloc for %s fail\n",argv[1]);
        return -1;
    }
    memset(pdont_overwrite_partition[dont_overwrite_count],0,strlen(argv[1])+1);
    strncpy(pdont_overwrite_partition[dont_overwrite_count],argv[1], strlen(argv[1]));

    dont_overwrite_count++;

    UBOOT_TRACE("OK\n");

    return 0;
}

int do_dont_overwrite_free(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    UBOOT_TRACE("IN\n");

    if(argc!=1){
        cmd_usage(cmdtp);
        return -1;
    }

    if(dont_overwrite_flag!=TRUE){
        UBOOT_DEBUG("The function of 'dont overwrite' is not enable\n");
        return -1;
    }
    dont_overwrite_flag=FALSE;
    while(dont_overwrite_count--){
        if(pdont_overwrite_partition[dont_overwrite_count]!=NULL){
         UBOOT_DEBUG("count=%d, %s\n",(unsigned int)dont_overwrite_count,pdont_overwrite_partition[dont_overwrite_count]);
         free(pdont_overwrite_partition[dont_overwrite_count]);
        }
    }
    UBOOT_TRACE("OK\n");
    return 0;
}


static BOOLEAN get_dont_overwrite_flag(void)
{
    UBOOT_TRACE("IN\n");
    UBOOT_TRACE("OK\n");
    return dont_overwrite_flag;
}

static BOOLEAN is_dont_overwrite_cmd(char *str)
{
    U8 count=0;
    char *p=NULL;
    UBOOT_TRACE("IN\n");
    if(str==NULL)
    {
        UBOOT_ERROR("input parameter 'str' is a null pointer\n");
        return FALSE;
    }

    if(get_dont_overwrite_flag()==TRUE)
    {
        for(count=0;count<dont_overwrite_count;count++)
        {
            if(count==0)//first item uses the full compare
            {
                if(strcmp(str,pdont_overwrite_partition[count])==0)
                {
                    UBOOT_INFO("\033[0;33m skip cmd %s\033[0m\n",str);
                    UBOOT_TRACE("OK\n");
                    return TRUE; //skip this cmd
                }
            }
            else
            {
                p=strstr(str,pdont_overwrite_partition[count]);
                if(p!=NULL)
                {
                    UBOOT_DEBUG("parttion[%d]: %s\n",(unsigned int)count, pdont_overwrite_partition[count]);
                    UBOOT_DEBUG("input string : %s\n",str);            
                    UBOOT_INFO("\033[0;33m skip cmd:%s\033[0m\n",str);
                    UBOOT_TRACE("OK\n");
                    return TRUE;
                }
            }
        }
    }
    UBOOT_TRACE("OK\n");
    return FALSE; // don't skip this cmd
}

void dont_overwrite_disable(void)
{
    UBOOT_TRACE("IN\n");
    dont_overwrite_flag=FALSE;
    UBOOT_TRACE("OK\n");
}
void dont_overwrite_enable(void)
{
    UBOOT_TRACE("IN\n");    
    dont_overwrite_flag=TRUE;
    run_command("dont_overwrite_init",0);
    UBOOT_TRACE("OK\n");
}

void RegisterCBIsStorageEmpty(IsStorageEmpty cb)
{
    UBOOT_TRACE("IN\n");
    fpIsStorageEmpty=cb;
    UBOOT_TRACE("OK\n");
}

static int check_not_empty(void)
{
    UBOOT_TRACE("IN\n");
    int ret=0;
    if(fpIsStorageEmpty==NULL)
    {
        #if (ENABLE_MODULE_MMC==1)
        UBOOT_DEBUG("storage is mmc\n");
        ret=run_command("mmc look certificate",0);
        #elif (ENABLE_MODULE_UBI_FS==1)
        UBOOT_DEBUG("storage is nand, and use ubifs\n");
        ret=vfs_mount(CHECK_FLASH_EMPTY);
        #else
        UBOOT_DEBUG("unknown storage\n");
        ret=-1;
        #endif
        if(ret==-1)
        {
            UBOOT_INFO("storage is empty\n");
            UBOOT_TRACE("OK\n");
            return -1;
        }
    }
    else
    {
        UBOOT_DEBUG("Detect by custmoer's call back function\n");
        if(fpIsStorageEmpty()!=0)
        {
            UBOOT_INFO("storage is empty\n");
            UBOOT_TRACE("OK\n");
            return -1;
        }
    }
    UBOOT_TRACE("OK\n");
    return 0; //flash is not empty
}

// set dont_overwrite setting by file name. if name equal FULL_AP_BIN_PATH,just don't overwrite.
void set_dont_overwrite_byname(char *upgradeFile)
{
    UBOOT_TRACE("IN\n");
    if(strcmp(upgradeFile,FULL_AP_BIN_PATH)==0)
    {
        UBOOT_DEBUG("dont_overwrite force disable");
        dont_overwrite_disable();
		do_force_overwrite_disable();
    }
    else
    {
        if(check_not_empty()==0)
        {
            UBOOT_DEBUG("dont_overwrite force enable");
            dont_overwrite_enable();
			do_force_overwrite_enable();
        }
        else
        {
            UBOOT_DEBUG("dont_overwrite force disable");
            dont_overwrite_disable();
			do_force_overwrite_disable();
        }
    }
    UBOOT_TRACE("OK\n");
}
#endif

BOOLEAN check_osd_active(void)
{
    #define OSD_ACTIVE "osd_active"
    if((getenv(OSD_ACTIVE)!=NULL) && (strcmp(getenv(OSD_ACTIVE),"off")==0))
    {
        setenv(OSD_ACTIVE,NULL);
        saveenv();
        return FALSE;
    }
    else 
    {
        return TRUE;
    }       
}
BOOLEAN check_skip_cmd(char *next_line)
{
    char *pLine = NULL;
    UBOOT_TRACE("IN\n");

    if(next_line==NULL)
    {
        UBOOT_ERROR("The input parameter 'next_line' is a null pointer\n");
        return FALSE;
    }

    //reset command must be skipped when force_upgrade.
    if((getenv(ENV_FORCEUPGRADE)!= NULL) && (strncmp(next_line,"reset",5)==0))
    {
        UBOOT_INFO("Reset command is skipped\n");
        UBOOT_TRACE("OK\n");
        return TRUE;
    }

// if is_dont_overwrite_cmd return TRUE, we skip this line.
// if is_dont_overwrite_cmd return FALSE, we execute this line.
#if(ENABLE_MODULE_DONT_OVERWRITE==1)
    extern int SKIP_DONT_OVERWRITE;
    if(SKIP_DONT_OVERWRITE == 0)
    {
        if(is_dont_overwrite_cmd(next_line)==TRUE)
        {
            if(is_force_overwrite_cmd(next_line)==TRUE)
            {
                goto END;
            }

            UBOOT_TRACE("OK\n");
            return TRUE;
        }
    }
END:
#endif

    //This part is for filter command.
    pLine=strstr(next_line,"#");
    if(pLine!=NULL){
        *pLine='\0';
    }
    UBOOT_TRACE("OK\n");
    return FALSE;
}

int do_rw_en_update_mode (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    int ret=0;
    EN_UPDATE_MODE enMode = EN_UPDATE_MODE_NONE;
    UBOOT_TRACE("IN\n");
    if(argc < 2)
    {
        cmd_usage(cmdtp);
        return -1;
    }
    
    if(argv[1][0] == 'g')
    {
        switch(getUpgradeMode())
        {
            case EN_UPDATE_MODE_NONE:
                UBOOT_INFO("EN_UPDATE_MODE_NONE\n");
                break;
            case EN_UPDATE_MODE_TFTP:
                UBOOT_INFO("EN_UPDATE_MODE_TFTP\n");
                break;
            case EN_UPDATE_MODE_USB:
                UBOOT_INFO("EN_UPDATE_MODE_USB\n");
                break;
            case EN_UPDATE_MODE_OAD:
                UBOOT_INFO("EN_UPDATE_MODE_OAD\n");
                break;
            case EN_UPDATE_MODE_NET:
                UBOOT_INFO("EN_UPDATE_MODE_NET\n");
                break;
            case EN_UPDATE_MODE_USB_WITH_SEG_DECRYPTED:
                UBOOT_INFO("EN_UPDATE_MODE_USB_WITH_SEG_DECRYPTED\n");
                break;
            case EN_UPDATE_MODE_NET_WITH_SEG_DECRYPTED:
                UBOOT_INFO("EN_UPDATE_MODE_NET_WITH_SEG_DECRYPTED\n");
                break;
            default:
                break;
        }
    } 
    else if (argv[1][0] == 's')
    {
        if(0 == strncmp("EN_UPDATE_MODE_NONE", argv[2], strlen("EN_UPDATE_MODE_NONE")))
            enMode = EN_UPDATE_MODE_NONE;
        else if (0 == strncmp("EN_UPDATE_MODE_TFTP", argv[2], strlen("EN_UPDATE_MODE_TFTP")))
            enMode = EN_UPDATE_MODE_TFTP;
        else if (0 == strncmp("EN_UPDATE_MODE_USB", argv[2], strlen("EN_UPDATE_MODE_USB")))
            enMode = EN_UPDATE_MODE_USB;
        else if (0 == strncmp("EN_UPDATE_MODE_OAD", argv[2], strlen("EN_UPDATE_MODE_OAD")))
            enMode = EN_UPDATE_MODE_OAD;
        else if (0 == strncmp("EN_UPDATE_MODE_NET", argv[2], strlen("EN_UPDATE_MODE_NET")))
            enMode = EN_UPDATE_MODE_NET;
        else if (0 == strncmp("EN_UPDATE_MODE_USB_WITH_SEG_DECRYPTED", argv[2], strlen("EN_UPDATE_MODE_USB_WITH_SEG_DECRYPTED")))
            enMode = EN_UPDATE_MODE_USB_WITH_SEG_DECRYPTED;
        else if (0 == strncmp("EN_UPDATE_MODE_NET_WITH_SEG_DECRYPTED", argv[2], strlen("EN_UPDATE_MODE_NET_WITH_SEG_DECRYPTED")))
            enMode = EN_UPDATE_MODE_NET_WITH_SEG_DECRYPTED;
        else
            cmd_usage(cmdtp);
           
        setUpgradeMode(enMode);
    }
    else
    {
        cmd_usage(cmdtp);
    }
    
    
    UBOOT_TRACE("OK\n");
    return ret;
}

#if ENABLE_DISPLAY_PICTURE_UPDATE
void check_filepartload_cmd(char *next_line, char *volume)
{
    char *pLine = NULL;
    UBOOT_TRACE("IN\n");
    if(next_line==NULL)
    {
        UBOOT_ERROR("The input parameter 'next_line' is a null pointer\n");
    }
    //filepartload command need to mount
    if(strncmp(next_line,"filepartload",12)==0)
    {
#if (ENABLE_MODULE_VFS == 1)
        vfs_mount(volume);
#endif
    }
    UBOOT_TRACE("OK\n");
}
#endif

int do_file_part_load_with_segment(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    #define DOWNLOAD_INTERFACE argv[1]
    #define DOWNLOAD_DEVICE argv[2]
    #define DOWNLOAD_ADDR argv[3]
    #define DOWNLOAD_FILE argv[4]
    #define DOWNLOAD_OFFSET argv[5]
    #define DOWNLOAD_LENGTH argv[6]
    #define SEGMENT_SIZE argv[7]

    unsigned int downloadOffset=0;
    unsigned int downloadLen=0;
    unsigned int pdownloadAddr=0;

    unsigned int readSegmentAddr=0;
    unsigned int readOffsetInSeg=0;
    unsigned int readLen=0;
    unsigned int readSegmentLen=0;
    unsigned int segmentNum=0;
    unsigned int _segmentSize=0;
    unsigned int segmentSize=0;
    unsigned int *ptrBufAddr=NULL;
    unsigned int upgrade_fileSize=0;
    unsigned int u32HwAesBufAddr=0;

    char buffer[CMD_BUF]="\0";
    UBOOT_TRACE("IN\n");

    if(argc<7)
    {
        return -1;
    }

    downloadOffset = (unsigned int)simple_strtoul(DOWNLOAD_OFFSET, NULL, 16);
    downloadLen = (unsigned int)simple_strtoul(DOWNLOAD_LENGTH, NULL, 16);
    pdownloadAddr = (unsigned int)simple_strtoul(DOWNLOAD_ADDR, NULL, 16);
    segmentSize = (unsigned int)simple_strtoul(SEGMENT_SIZE, NULL, 16);

    //Get total file size
    upgrade_fileSize = GetfileSizeforUpgrade(DOWNLOAD_INTERFACE,DOWNLOAD_DEVICE,DOWNLOAD_FILE);
    if(upgrade_fileSize<0)
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
    u32HwAesBufAddr=UPGRADE_IMAGE_BUFFER_ADDRESS;
    ptrBufAddr = (unsigned int*)malloc(_segmentSize);
    if(ptrBufAddr==NULL)
    {
        UBOOT_ERROR("[ERROR] ptrBufAddr memroy allocate fail\n");
        return -1;
    }

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

        readSegmentLen=((readSegmentAddr+_segmentSize)>=upgrade_fileSize)?upgrade_fileSize-readSegmentAddr:_segmentSize;
        UBOOT_DEBUG("downloadOffset=0x%x\n",downloadOffset);
        UBOOT_DEBUG("downloadLen=0x%x\n",downloadLen);
        UBOOT_DEBUG("pdownloadAddr=0x%x\n",pdownloadAddr);
        UBOOT_DEBUG("readSegmentAddr=0x%x\n",readSegmentAddr);
        UBOOT_DEBUG("readOffsetInSeg=0x%x\n",readOffsetInSeg);
        UBOOT_DEBUG("readLen=0x%x\n",readLen);
        UBOOT_DEBUG("readSegmentLen=0x%x\n",readSegmentLen);
        UBOOT_DEBUG("segmentNum=0x%x\n",segmentNum);
        UBOOT_DEBUG("_segmentSize=0x%x\n",_segmentSize);


        if(getUpgradeMode()==EN_UPDATE_MODE_NET)
        {
#if (ENABLE_MODULE_VFS == 1)
            if(vfs_read((void*)ptrBufAddr,DOWNLOAD_FILE,readSegmentAddr,readSegmentLen)!=0)
            {
                free(ptrBufAddr);
                return -1;
            }
#else
            return -1;
#endif
        }
        else if(getUpgradeMode()==EN_UPDATE_MODE_USB)
        {
            memset(buffer, 0, CMD_BUF);
            snprintf(buffer, CMD_BUF, "fatpartload %s %s %x %s %x %x", DOWNLOAD_INTERFACE,DOWNLOAD_DEVICE,ptrBufAddr, DOWNLOAD_FILE, readSegmentAddr, readSegmentLen);
            UBOOT_DEBUG("cmd=%s\n",buffer);
            if(run_command(buffer, 0)!=0)
            {
                free(ptrBufAddr);
                return -1;
            }
        }
        else
        {
            UBOOT_ERROR("Wrong Upgrade Mode!!\n");
            free(ptrBufAddr);
            return -1;
        }

        // copy to target address
        memcpy((void*)pdownloadAddr,(char *)(ptrBufAddr)+readOffsetInSeg,readLen);
        //memcpy((void*)pdownloadAddr,(void*)(ptrBufAddr+readOffsetInSeg),readLen);
        UBOOT_DUMP(pdownloadAddr, 0x50);

        downloadLen-=readLen;
        pdownloadAddr+=readLen;
        downloadOffset+=readLen;
        segmentNum++;
    }

    // set the download length for next command
    setenv("filesize",DOWNLOAD_LENGTH);
    free(ptrBufAddr);
    UBOOT_TRACE("OK\n");
    return 0;
}

int do_file_part_load (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    char buffer[CMD_BUF] = "\0";
    int ret=0;
    unsigned int addrs = 0;
    unsigned int offset = 0;
    unsigned int len = 0;
    UBOOT_TRACE("IN\n");
    if(argc < 5)
    {
        cmd_usage(cmdtp);
        return -1;
    }
    switch(getUpgradeMode())
    {
        case EN_UPDATE_MODE_USB:
            snprintf(buffer, CMD_BUF, "fatpartload usb %d:%d %s %s %s %s",g_device,g_partition, argv[1], argv[2], argv[3], argv[4]);
            UBOOT_DEBUG("cmd=%s\n",buffer);
            ret = run_command(buffer, 0);
            break;
        case EN_UPDATE_MODE_USB_WITH_SEG:
            if(argc == 5)
                snprintf(buffer, CMD_BUF, "filepartloadSeg usb %d:%d %s %s %s %s 0",g_device,g_partition, argv[1], argv[2], argv[3], argv[4]);
            else if(argc == 6)
                snprintf(buffer, CMD_BUF, "filepartloadSeg usb %d:%d %s %s %s %s %s",g_device,g_partition, argv[1], argv[2], argv[3], argv[4], argv[5]);
            else
            {
                UBOOT_ERROR("filepartloadSeg parameter error!\n");
                break;
            }
            UBOOT_DEBUG("cmd=%s\n",buffer);
            ret = run_command(buffer, 0);
            break;
#if (ENABLE_NET == 1)
        case EN_UPDATE_MODE_NET_WITH_SEG_DECRYPTED:
#if (ENABLE_MODULE_VFS == 1)
            vfs_mount(getUpgradeFsMountName());
#endif
            snprintf(buffer, CMD_BUF, "filepartloadSegAES net %d:%d %s %s %s %s",g_device,g_partition, argv[1], argv[2], argv[3], argv[4]);
            UBOOT_DEBUG("cmd=%s\n",buffer);
            ret = run_command(buffer, 0);
            break;
        case EN_UPDATE_MODE_USB_WITH_SEG_DECRYPTED:
            snprintf(buffer, CMD_BUF, "filepartloadSegAES usb %d:%d %s %s %s %s",g_device,g_partition, argv[1], argv[2], argv[3], argv[4]);
            UBOOT_DEBUG("cmd=%s\n",buffer);
            ret = run_command(buffer, 0);
            break;
        case EN_UPDATE_MODE_TFTP:
            addrs = simple_strtol(argv[1], NULL, 16);
            addrs = addrs - simple_strtol(argv[3], NULL, 16); // for skip the script content
            snprintf(buffer, CMD_BUF, "tftp 0x%X %s",addrs, argv[2]);
            UBOOT_DEBUG("cmd=%s\n",buffer);
            ret = run_command(buffer, 0);
            break;
#endif
#if (ENABLE_NET == 1)||(ENABLE_OAD == 1)
        case EN_UPDATE_MODE_OAD:
        case EN_UPDATE_MODE_NET:
            UBOOT_DEBUG("vfs_read: %s,%s,%s,%s \n",argv[1], argv[2], argv[3], argv[4]);
            addrs=simple_strtoul(argv[1], NULL, 16);
            offset=simple_strtoul(argv[3], NULL, 16);
            len=simple_strtoul(argv[4], NULL, 16);
#if (ENABLE_MODULE_VFS == 1)
            vfs_mount(getUpgradeFsMountName());
            ret = vfs_read((void *)addrs, argv[2], offset, len);
#endif
            break;
#endif
        default:
            break;
    }
    if (ret != -1)
    {
      ret =0;
    }
    UBOOT_TRACE("OK\n");
    return ret;
}

int check_full_usb_crc(char * upgradeFile, char *scriptBuf)
{
    MS_U32 CRC_result=0;
    MS_U32 CRC_value=0;
    MS_U32 u32CRCOffset=0;
    MS_U32 u32FileSize=0;
    MS_U32 u32SegmentSize=0;
    MS_U32 u32SegmentCnt=0;
    MS_U32 u32SegmentOffset=0;
    char *tempBuf=NULL;
    char *pTr=scriptBuf;
    char buffer[CMD_BUF]= "\0";
    UBOOT_TRACE("IN\n");

    if(strstr(pTr, "USB Check Integrity") != NULL)
    {
        //do usb upgrade bin full check

        UBOOT_DEBUG("USB Check Integrity Start...\n");
        pTr = strstr(pTr, "CRC Segment Size : ");
        if(pTr != NULL)
        {
            pTr += strlen("CRC Segment Size : ");
            u32SegmentSize = simple_strtol(pTr, NULL, 16);
            pTr = NULL;
        }
        else
        {
            u32SegmentSize = DEFAULT_FULL_FILE_CRC_SEGMENT_SIZE;
        }
        UBOOT_DEBUG("u32SegmentSize = 0x%lx\n", u32SegmentSize);

        if(getUpgradeMode() == EN_UPDATE_MODE_USB)
        {
            snprintf(buffer, CMD_BUF, "fatfilesize usb %d:%d %s",g_device, g_partition, upgradeFile);
            UBOOT_DEBUG("cmd=%s\n",buffer);
            if (-1 == run_command(buffer, 0))
            {
                UBOOT_ERROR("Error: cmd %s failed\n", buffer);
                return -1;
            }
            u32FileSize = simple_strtol(getenv("filesize"), NULL, 16);
            if((u32FileSize <= 4) || (u32FileSize%4 != 0))
            {
                UBOOT_ERROR("wrong file size: 0x%lx\n", u32FileSize);
                return -1;
            }

            #define TEMP_SEGMENT_SIZE ((u32FileSize&0x1FFF)+0x1000)
            if(u32SegmentSize >= TEMP_SEGMENT_SIZE)
            {
                tempBuf = malloc(u32SegmentSize + 16);
            }
            else
            {
                tempBuf = malloc(TEMP_SEGMENT_SIZE + 16);
            }

            if(tempBuf == NULL)
            {
                UBOOT_ERROR("[ERROR] tempBuf memroy allocate fail\n");
                return -1;
            }

            memset(tempBuf, 0, TEMP_SEGMENT_SIZE);
            snprintf(buffer, CMD_BUF, "filepartload %x %s %x %x", tempBuf, upgradeFile, u32FileSize - TEMP_SEGMENT_SIZE, TEMP_SEGMENT_SIZE);
            UBOOT_DEBUG("cmd=%s\n",buffer);
            if (-1 == run_command(buffer, 0))
            {
                UBOOT_ERROR("Error: cmd %s failed\n", buffer);
                free(tempBuf);
                return -1;
            }

            CRC_value = *((U32*)(tempBuf + TEMP_SEGMENT_SIZE - FULL_FILE_CRC_OFFSET));
            UBOOT_INFO("Full Usb Upgrade Bin CRC_value is 0x%lx\n", CRC_value);
            if (CRC_value == 0)
            {
                UBOOT_ERROR("Error: CRC_value is 0x%lx\n", CRC_value);
                free(tempBuf);
                return -1;
            }

            u32FileSize -= FULL_FILE_CRC_OFFSET;
            u32SegmentCnt = u32FileSize/u32SegmentSize;
            u32SegmentOffset = u32FileSize%u32SegmentSize;
            UBOOT_DEBUG("u32SegmentCnt = 0x%lx\n", u32SegmentCnt);
            UBOOT_INFO("Full Upgrade File CRC Check Start...\n");
            while(u32SegmentCnt)
            {
                UBOOT_INFO("*");
                snprintf(buffer, CMD_BUF, "filepartload %x %s %x %x", tempBuf, upgradeFile, u32CRCOffset, u32SegmentSize);
                UBOOT_DEBUG("cmd=%s\n",buffer);
                if (-1 == run_command(buffer, 0))
                {
                    UBOOT_ERROR("Error: cmd %s failed\n", buffer);
                    free(tempBuf);
                    return -1;
                }

                CRC_result = crc32(CRC_result, (U8*)tempBuf, u32SegmentSize);
                u32CRCOffset += u32SegmentSize;
                --u32SegmentCnt;
            }

            U32 u32readLen = 0;
            u32readLen=(u32SegmentOffset>=TEMP_SEGMENT_SIZE)?u32SegmentOffset:TEMP_SEGMENT_SIZE;
            snprintf(buffer, CMD_BUF, "filepartload %x %s %x %x", tempBuf, upgradeFile, u32CRCOffset, u32readLen);
            UBOOT_DEBUG("cmd=%s\n",buffer);
            if (-1 == run_command(buffer, 0))
            {
                UBOOT_ERROR("Error: cmd %s failed\n", buffer);
                free(tempBuf);
                return -1;
            }

            CRC_result = crc32(CRC_result, (U8*)tempBuf, u32SegmentOffset);
            if(CRC_result != CRC_value)
            {
                UBOOT_ERROR("\033[1;31m Error: CRC error! \033[0m \n");
                UBOOT_ERROR("CRC_result=0x%lx, CRC_expect=0x%lx, size=0x%lx\n", CRC_result, CRC_value, u32FileSize);
                free(tempBuf);
                return -1;
            }
            UBOOT_INFO("CRC_result=0x%lx, CRC_expect=0x%lx, size=0x%lx\n", CRC_result, CRC_value, u32FileSize);
            free(tempBuf);
            #undef TEMP_SEGMENT_SIZE
        }
    }

    UBOOT_TRACE("OK\n");
    return 0;
}

int check_script_crc(char * upgradeFile, char *scriptBuf)
{
    MS_U32 CRC_result=0;
    MS_U32 CRC_value=0;
    MS_U32 u32FileSize=0;
    char *tempBuf=NULL;
    char buffer[CMD_BUF]= "\0";
    BOOLEAN bIsEncrypted = TRUE;
    UBOOT_TRACE("IN\n");
    if((upgradeFile==NULL)||(scriptBuf==NULL))
    {
        UBOOT_ERROR("input is a null pointer.upgradeFile=0x%x,scriptBuf=0x%x\n",(unsigned int)upgradeFile,(unsigned int)scriptBuf);
        return -1;
    }
    // Get filesize
    if(getUpgradeMode()==EN_UPDATE_MODE_USB)
    {
        snprintf(buffer, CMD_BUF, "fatfilesize usb %d:%d %s",g_device, g_partition, upgradeFile);
        UBOOT_DEBUG("cmd=%s\n",buffer);
        if (-1 == run_command(buffer, 0))
        {
            UBOOT_ERROR("Error: cmd %s failed\n", buffer);
            return -1;
        }
        u32FileSize = simple_strtol(getenv("filesize"), NULL, 16);
    }
    else
    {
        //vfs_mount("oad");
#if (ENABLE_MODULE_VFS == 1)
        u32FileSize=vfs_getsize(upgradeFile);
#endif
    }
    if((u32FileSize <= 4) || (u32FileSize%4 != 0))
    {
        UBOOT_ERROR("wrong file size: 0x%lx\n", u32FileSize);
        return -1;
    }
    // crc check
    UBOOT_DEBUG("script file size=0x%x\n",getScritSizeOfUpgradeImage());
    memset(buffer, 0 , CMD_BUF);
    //u32FileSize-(u32FileSize&0x1FF)-0x1000 ???
    // (u32FileSize&0x1FF) : make sure offset alignmet 4096 bytes
    // -0x1000 : make sure length is over 4096 bytes.
    // ubifs_part_load should align to page size(0x1000)
    #define TEMP_SIZE ((u32FileSize&0x1FFF)+0x1000)
    tempBuf=malloc(TEMP_SIZE);
    if(tempBuf==NULL)
    {
        UBOOT_ERROR("malloc for tempBuf fail\n");
        return -1;
    }
    if(getUpgradeMode()==EN_UPDATE_MODE_USB)
    {
        memset(tempBuf,0,TEMP_SIZE);
        snprintf(buffer, CMD_BUF, "filepartload %x %s %x %x", tempBuf,upgradeFile,u32FileSize-TEMP_SIZE,TEMP_SIZE);
        UBOOT_DEBUG("cmd=%s\n",buffer);
        if (-1 == run_command(buffer, 0))
        {
            UBOOT_ERROR("Error: cmd %s failed\n", buffer);
            free(tempBuf);
            return -1;
        }
    }
    else
    {
#if (ENABLE_MODULE_VFS == 1)
        vfs_read(tempBuf,upgradeFile,u32FileSize-TEMP_SIZE,TEMP_SIZE);


        // compare the first 16 bytes and the last 16bytes
        // if the same, it means the CRC value is at the 24th from the end.
        // if different, it means the CRC value is at the 8th from the end.
        MS_U8 k = 0;
        bIsEncrypted = TRUE;
        char * pu8DataHead;
        pu8DataHead = malloc(0x1000);
        memset(pu8DataHead,0,0x1000);
        vfs_read(pu8DataHead, upgradeFile, 0, 0x1000);
        for(k = 0 ; k < COMPARE_LENGTH ; k++)
        {
            //printf("[%02x] v.s. [%02x] \n", (char)*(pu8DataHead+k), (char)*(tempBuf+TEMP_SIZE-COMPARE_LENGTH+k));
            if(((char)*(pu8DataHead+k)) != ((char)*(tempBuf+TEMP_SIZE-COMPARE_LENGTH+k)))
            {
                UBOOT_DEBUG("This bin is not encrypted. So the CRC value offset has to be minus 16.\n");
                bIsEncrypted = FALSE;
                break;
            }
        }

        free(pu8DataHead);
#endif
    }

    if(bIsEncrypted)
        CRC_value=*((U32*)((U32)tempBuf+TEMP_SIZE-SCRIPT_FILE_CRC_OFFSET));
    else
        CRC_value=*((U32*)((U32)tempBuf+TEMP_SIZE-(SCRIPT_FILE_CRC_OFFSET-COMPARE_LENGTH)));

    CRC_result=crc32( 0x00, (U8*)scriptBuf, getScritSizeOfUpgradeImage());

    if(CRC_result!= CRC_value)
    {
        UBOOT_ERROR("\033[1;31m Error: CRC error! \033[0m \n");
        UBOOT_ERROR("CRC_resule=0x%x, CRC_expect=0x%x, size=0x%x\n",(unsigned int)CRC_result,(unsigned int)CRC_value,getScritSizeOfUpgradeImage());
        free(tempBuf);
        return -1;
    }
    UBOOT_DEBUG("CRC_resule=0x%x, CRC_expect=0x%x, size=0x%x\n",(unsigned int)CRC_result,(unsigned int)CRC_value,getScritSizeOfUpgradeImage());
#if defined(CONFIG_USB_UPGRADE_MAGIC_STRING)
    char *magic_string=NULL;
    if(bIsEncrypted)
        magic_string=(tempBuf+TEMP_SIZE-SCRIPT_FILE_CRC_OFFSET-UPGRADE_FILE_MAGIC_STRING_LENGYTH);
    else
        magic_string=(tempBuf+TEMP_SIZE-(SCRIPT_FILE_CRC_OFFSET+UPGRADE_FILE_MAGIC_STRING_LENGYTH-COMPARE_LENGTH));
    if (strncmp(CONFIG_MAGIC_STRING, magic_string,8) != 0) {
        UBOOT_ERROR("check MAGIC_STRING fail\n");
        return -1;
    }
    else{
        printf("check MAGIC_STRING ok\n");
    }
#endif
    free(tempBuf);
    UBOOT_TRACE("OK\n");
    return 0;
}

int check_script_encrypt(char *script_buf)
{
    char *_argv[CONFIG_SYS_MAXARGS + 1];
    char *next_line = NULL;
    char *u8TempBuf = NULL;
    char *u8TempPtr = NULL;
    unsigned int lineNum = 0;
    UBOOT_TRACE("IN\n");

    // duplicate script for check_script_encrypt
    u8TempBuf=(char *)malloc(getScritSizeOfUpgradeImage());
    u8TempPtr=u8TempBuf;
    if(u8TempBuf == NULL)
    {
        UBOOT_ERROR("no memory for check_script_encrypt!!\n");
        return -1;
    }
    memcpy(u8TempBuf,script_buf,getScritSizeOfUpgradeImage());
    memset(_argv, 0, sizeof(char*)*(CONFIG_SYS_MAXARGS + 1));

    for(lineNum=0;lineNum<2;lineNum++)//Check two lines
    {
        next_line = get_script_next_line(&u8TempBuf);

        if((next_line==NULL) || (strlen(next_line)>MAX_LINE_SIZE))
        {
            UBOOT_DEBUG("%dth line not found\n",lineNum);
            free(u8TempPtr);
            return TRUE;
        }
        else
        {      /* parse command to argv */
            if ( parse_line (next_line, _argv) == 0)
            {
                  UBOOT_DEBUG("%dth line doesn't find any argument \n",lineNum);
                  free(u8TempPtr);
                  return TRUE;
            }
            else
            {   /* Look up command in command table */
                if (find_cmd(_argv[0]) == NULL)
                {
                      UBOOT_DEBUG("%dth line doesn't find any cmd \n",lineNum);
                      free(u8TempPtr);
                      return TRUE;
                }
            }
        }
    }
    free(u8TempPtr);

    UBOOT_TRACE("OK\n");
    return FALSE;
}
int do_check_file_partition(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    char *cFile =NULL;
    int partition=0;
    int device=0;
    int ret=-1;
    UBOOT_TRACE("IN\n");
     if(argc < 2)
    {
        cmd_usage(cmdtp);
        return -1;
    }
    cFile=argv[1];
    if(check_file_partition(cFile,&partition,&device)!= 0)
    {
        UBOOT_INFO("FAIL: no file in usb or Usb no init!\n");
        ret =-1;
    }
    else
    {
        UBOOT_INFO("file:%s is existed.[device:%d partition:%d]\n",cFile,device,partition);
        ret = 0;
    }
    UBOOT_TRACE("OK\n");
    return ret;
}

int check_file_partition(char *upgradeFile,int *device, int *partition)
{
    char buffer[CMD_BUF] = "\0";
    char *pdev = NULL;
    char *ppart = NULL;
    int g_device_e = 0;
    int g_partition_e = 0;

    UBOOT_TRACE("IN\n");

    pdev=getenv(USB_DEVICE);
    if(pdev)
    {
        g_device = (int)simple_strtoul(pdev, NULL, 16);
        if(g_device >= MAX_DEVICE)
        {
            UBOOT_DEBUG("invalid USB specified specified [%d]\n", g_device);
            return -1;
        }
        g_device_e = g_device + 1;
        UBOOT_INFO("USB device specified [%d]\n", g_device);
    }
    else
    {
        g_device = 0;
        g_device_e = MAX_DEVICE;
    }

    ppart=getenv(USB_PARTITION);
    if(ppart)
    {
        g_partition = (int)simple_strtoul(ppart, NULL, 16);
        if(g_partition >= MAX_PARTITION)
        {
            UBOOT_DEBUG("invalid USB partition specified [%d]\n", g_partition);
            return -1;
        }
        g_partition_e = g_partition+1;
        UBOOT_INFO("USB partition specified [%d]\n", g_partition);
    }
    else
    {
        g_partition = 0;
        g_partition_e = MAX_PARTITION;
    }

    for( ; g_device < g_device_e ;g_device++)
    {
        for( ; g_partition < g_partition_e ; g_partition++)
        {
            snprintf(buffer, CMD_BUF, "fatload usb %d:%d  %X %s 1",g_device, g_partition, UPGRADE_BUFFER_ADDR, upgradeFile);
            UBOOT_DEBUG("cmd: %s \n",buffer);
            if(run_command(buffer, 0) == 0)
            {
               if((device != NULL) && (partition != NULL))
               {
                   *device=g_device;
                   *partition=g_partition;
               }
               UBOOT_TRACE("OK\n");
               return 0;
            }
        }
    }
    UBOOT_TRACE("OK\n");
    return -1;
}

int setUpgradeFileName(char *str)
{
    char *buffer=NULL;
    char *pname=NULL;
    int ret=0;
    UBOOT_TRACE("IN\n");
    if(str==NULL)
    {
       UBOOT_ERROR("The input parameter 'str' is a null pointer\n");
       return -1;
    }

    buffer=malloc(CMD_BUF);
    if(buffer==NULL)
    {
        UBOOT_ERROR("malloc fail \n");
        return -1;
    }
    memset(buffer,0,CMD_BUF);
    pname=strrchr(str,'/');
    if(pname==NULL)
    {
        UBOOT_DEBUG("There are some '/' in the file name\n");
        pname=str;
    }
    else
    {
        pname++;
    }

    snprintf(buffer,CMD_BUF, "setenv %s %s",ENV_UPGRADE_NAME,pname);

    UBOOT_DEBUG("cmd: %s \n",buffer);
    ret=run_command(buffer,0);
    if(ret==-1)
    {
        free(buffer);
        UBOOT_ERROR("setenv ENV_UPGRADE_NAME fail\n");
    }
    else
    {
        free(buffer);
        UBOOT_TRACE("OK\n");
    }
    return ret;
}

int deleteUpgradeFileName(void)
{
    char *buffer=NULL;
    int ret=0;
    UBOOT_TRACE("IN\n");
    buffer=malloc(CMD_BUF);
    if(buffer==NULL)
    {
        UBOOT_ERROR("malloc fail \n");
        return -1;
    }
    memset(buffer,0,CMD_BUF);

    snprintf(buffer,CMD_BUF, "setenv %s ",ENV_UPGRADE_NAME);
    UBOOT_DEBUG("cmd: %s \n",buffer);
    ret=run_command(buffer,0);
    if(ret==-1)
    {
        free(buffer);
        UBOOT_ERROR("delete ENV_UPGRADE_NAME fail\n");
    }
    else
    {
        free(buffer);
        UBOOT_TRACE("OK\n");
    }
    return ret;

}

int setScritSizeOfUpgradeImage(unsigned int size)
{
    UBOOT_TRACE("IN\n");
    scriptSizeOfUpgradeImage=size;
    UBOOT_TRACE("OK\n");
    return 0;
}

int clearScritSizeOfUpgradeImage(void)
{
    UBOOT_TRACE("IN\n");
    scriptSizeOfUpgradeImage=0;
    UBOOT_TRACE("OK\n");
    return 0;
}

unsigned int getScritSizeOfUpgradeImage(void)
{
    UBOOT_TRACE("IN\n");
    if(scriptSizeOfUpgradeImage==0)
    {
        UBOOT_TRACE("OK\n");
        return DEFAULT_SCRIPT_SIZE_FOR_UPGRADE_IMAGE;
    }
    else
    {
        UBOOT_TRACE("OK\n");
        return scriptSizeOfUpgradeImage;
    }
}

void setUpgradeFsMountName(char* pFsMount)
{
    UBOOT_TRACE("IN\n");
    memcpy(s_UpgradeFsMount,pFsMount,strlen(pFsMount)+1);
    UBOOT_TRACE("OK\n");
}

char* getUpgradeFsMountName(void)
{
    UBOOT_TRACE("IN\n");
    UBOOT_TRACE("OK\n");
    return s_UpgradeFsMount;
}


int setUpgradeMode(EN_UPDATE_MODE enMode)
{
    UBOOT_TRACE("IN\n");
    s_UpdateMode=enMode;
    UBOOT_TRACE("OK\n");
    return 0;
}

EN_UPDATE_MODE getUpgradeMode(void)
{
    UBOOT_TRACE("IN\n");
    UBOOT_TRACE("OK\n");
    return s_UpdateMode;
}

unsigned int check_script_cmd_count(char *script_buf)
{
    char *_argv[CONFIG_SYS_MAXARGS + 1];
    char *next_line=NULL;
    unsigned int line = 0;

    memset(_argv, 0, sizeof(char*)*(CONFIG_SYS_MAXARGS + 1));

    next_line = get_script_next_line(&script_buf);
    UBOOT_DEBUG("The address of script_buf is 0x%x\n",(unsigned int)script_buf);
    while (next_line != NULL)
    {
        if((strlen(next_line)>MAX_LINE_SIZE))
        {
            UBOOT_DEBUG("The size of %dth line is over %d bytes. \n",line, MAX_LINE_SIZE);
            return line;
        }
        else
        {      /* parse command to argv */
            if ( parse_line (next_line, _argv) == 0)
            {
                  UBOOT_DEBUG("%dth line doesn't find any argument \n",line);
                  return line;
            }
            else
            {   /* Look up command in command table */
                if (find_cmd(_argv[0]) == NULL)
                {
                      UBOOT_DEBUG("%dth line doesn't find any cmd, _argv[0]=%s \n",line,_argv[0]);
                      return line;
                }
            }
        }
        line += 1;
        next_line = get_script_next_line(&script_buf);
    }
    return line;
}

#if defined (CONFIG_R2_BRINGUP) || defined (CONFIG_AMP_TRUSTZONE_ENABLE)

int DecryptUsbImageToOstPst(unsigned long ulVAddr ,unsigned long ulTeePartOffset ,unsigned long ulTeeImageSize)
{
    UBOOT_TRACE("IN\n");

    UBOOT_DEBUG("ulVAddr(hex) =%lx\n",ulVAddr);
    UBOOT_DUMP(ulVAddr, 0x200);

    MDrv_BDMA_MemCopy((ulVAddr^CONFIG_MIU0_BUSADDR), (ulVAddr^CONFIG_MIU0_BUSADDR)+ulTeePartOffset, ulTeeImageSize);

    UBOOT_DEBUG("ulVAddr+0x600000(hex) =%lx\n",(ulVAddr+ulTeePartOffset));
    UBOOT_DUMP(ulVAddr, 0x200);
    UBOOT_DEBUG("ulVAddr^CONFIG_MIU0_BUSADDR(hex)+0x600000 =%lx\n",(ulVAddr^CONFIG_MIU0_BUSADDR)+ulTeePartOffset);
    UBOOT_DUMP((ulVAddr+ulTeePartOffset), 0x200);

    DecryptKernelImage((char*)(ulVAddr+ulTeePartOffset), ulTeeImageSize);

    UBOOT_DUMP((ulVAddr+ulTeePartOffset), 0x200);
    UBOOT_TRACE("OK\n");

    return 1;
}

int ReadTeeImagefrMmc(unsigned long ulMmcVAddr, unsigned long ulTeePartSize)
{
    unsigned char IsMmcImageCorrect=TRUE;
    char buffer[CMD_BUF]={0};
    UBOOT_TRACE("IN\n");
#if defined(CONFIG_SECURITY_BOOT)
// read tee from mmc
    memset(buffer,0, CMD_BUF);

#ifdef CONFIG_UBI
    snprintf(buffer,CMD_BUF, "nand read.e %x %s %x",ulMmcVAddr,TEE_PARTITION,ulTeePartSize);
#else
    snprintf(buffer,CMD_BUF, "mmc read.p %x %s %x",ulMmcVAddr,TEE_PARTITION,ulTeePartSize);
#endif
    UBOOT_DEBUG("cmd: %s \n",buffer);
    run_command(buffer,0);

    UBOOT_DUMP(ulMmcVAddr, 0x100);
    extern int _RawData_Authentication(char *,unsigned int);
    if(-1==_RawData_Authentication(TEE_PARTITION,ulMmcVAddr))
    {
        UBOOT_ERROR("TEE Authentication fail\n");
        IsMmcImageCorrect=FALSE;
    }

    UBOOT_DUMP(ulMmcVAddr, 0x100);

#else
    memset(buffer,0,CMD_BUF);
    #ifdef CONFIG_UBI
    snprintf(buffer,CMD_BUF, "nand read.e %x %s %x",ulMmcVAddr,TEE_PARTITION,TEE_HEADER_LEN);
    #else
    snprintf(buffer,CMD_BUF, "mmc read.p %x %s %x",ulMmcVAddr,TEE_PARTITION, TEE_HEADER_LEN);
    #endif
    UBOOT_DEBUG("cmd: %s \n",buffer);
    run_command(buffer,0);

    UBOOT_DUMP(ulMmcVAddr, TEE_HEADER_LEN);
#endif
    UBOOT_TRACE("OK\n");
    return IsMmcImageCorrect;
}

int do_update_nuttx(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    unsigned long ulVAddr=0;
    unsigned char partname[10]={0};
    unsigned long ulTeeImageSize=0;
    int ret=0;
    char buffer[CMD_BUF]="\0";
    unsigned long ulOffset=0;
    unsigned long ulTeePartOffset=0;
    unsigned char IsMmcImageCorrect=TRUE;
    unsigned long ulMmcVAddr=0;
    unsigned long ulTeePartSize=0;
    
    ulVAddr=simple_strtoul(argv[1], NULL, 16);

    memcpy(partname, argv[2], strlen(argv[2]));
    ulTeeImageSize= simple_strtoul(argv[3], NULL, 16);
    ulOffset =ulVAddr+ulTeeImageSize;

    UBOOT_DEBUG("ulVAddr(hex): %lx \n",ulVAddr);
    UBOOT_DEBUG("ulTeeImageSize(dec): %ld \n",ulTeeImageSize);
    UBOOT_DEBUG("partname: %s \n",partname);

#if defined(CONFIG_SECURITY_BOOT)
// decrypt tee
    ulTeePartOffset=TEE_PART_OFFSET;
    DecryptUsbImageToOstPst(ulVAddr ,ulTeePartOffset ,ulTeeImageSize);
#endif

    ST_TEE_HEADER *pstUsbHder=NULL;
    pstUsbHder=(ST_TEE_HEADER*)(ulVAddr+ulTeePartOffset);

    UBOOT_DUMP((ulVAddr+ulTeePartOffset), TEE_HEADER_LEN);
    UBOOT_DEBUG("pstUsbHder uitee_header_len : %x \n",pstUsbHder->_sthd.uitee_header_len);
    UBOOT_DEBUG("pstUsbHder uitee_start_paddr : %x \n",pstUsbHder->_sthd.uitee_start_paddr);
    UBOOT_DEBUG("pstUsbHder uitee_start_vaddr : %x \n",pstUsbHder->_sthd.uitee_start_vaddr);
    UBOOT_DEBUG("pstUsbHder uitee_image_size : %x \n",pstUsbHder->_sthd.uitee_image_size);
    UBOOT_DEBUG("pstUsbHder uiHeaderVersion : %x \n",pstUsbHder->_sthd.uiHeaderVersion);
    UBOOT_DEBUG("pstUsbHder ulltime : %llx \n",pstUsbHder->_sthd.ulltime);
    UBOOT_DEBUG("pstUsbHder ullTeeBinVersion : %llx \n",pstUsbHder->_sthd.ullTeeBinVersion);
    UBOOT_DEBUG("pstUsbHder uicrc_value : %x \n",pstUsbHder->_sthd.uicrc_value);
    UBOOT_DEBUG("pstUsbHder ucMagicID : %s \n",pstUsbHder->ucMagicID);

    UBOOT_DEBUG("*****Compare Magic ID***** \n");
    ret=memcmp(pstUsbHder->ucMagicID, TeeMagicID, strlen(TeeMagicID));
    if(ret==0)
    {
        UBOOT_DEBUG("*****With header***** \n");
        ulMmcVAddr=MMC_VADDR;
        ulTeePartSize=TEE_PART_SIZE;
        IsMmcImageCorrect=ReadTeeImagefrMmc(ulMmcVAddr, ulTeePartSize);
        if(IsMmcImageCorrect==TRUE){
            ST_TEE_HEADER *pstMmcHder;
            pstMmcHder=(ST_TEE_HEADER*)ulMmcVAddr;

            UBOOT_DEBUG("pstMmcHder uitee_header_len : %x \n",pstMmcHder->_sthd.uitee_header_len);
            UBOOT_DEBUG("pstMmcHder uitee_start_paddr : %x \n",pstMmcHder->_sthd.uitee_start_paddr);
            UBOOT_DEBUG("pstMmcHder uitee_start_vaddr : %x \n",pstMmcHder->_sthd.uitee_start_vaddr);
            UBOOT_DEBUG("pstMmcHder uitee_image_size : %x \n",pstMmcHder->_sthd.uitee_image_size);
            UBOOT_DEBUG("pstMmcHder uiHeaderVersion : %x \n",pstMmcHder->_sthd.uiHeaderVersion);
            UBOOT_DEBUG("pstMmcHder ulltime : %llx \n",pstMmcHder->_sthd.ulltime);
            UBOOT_DEBUG("pstMmcHder ullTeeBinVersion : %llx \n",pstMmcHder->_sthd.ullTeeBinVersion);
            UBOOT_DEBUG("pstMmcHder uicrc_value : %x \n",pstMmcHder->_sthd.uicrc_value);
#if defined(CONFIG_ANTI_ROLLBACK)
            #define DEBUG_CL 0x393939393939
            //UBOOT_DEBUG("DEBUG_CL =%llx \n",DEBUG_CL);
            UBOOT_DEBUG("*****Check Usb's change list***** \n");
            if(pstUsbHder->_sthd.ullTeeBinVersion!=DEBUG_CL)
            {
                UBOOT_DEBUG("*****Compare Mmc's & Usb's Time***** \n");
                if(pstUsbHder->_sthd.ulltime < pstMmcHder->_sthd.ulltime)
                 {
                    UBOOT_DEBUG("usb version : %lld \n",pstUsbHder->_sthd.ulltime);
                    UBOOT_DEBUG("mmc version : %lld \n",pstMmcHder->_sthd.ulltime);
                    UBOOT_ERROR("*****usb version is too old, please use a new one\n");
                    UBOOT_ERROR("*****usb version is too old, please use a new one\n");
                    return -1;
                }
            }
#endif
        }
    }
    else{
        UBOOT_DEBUG("*****Without header***** \n");
    }

#ifdef CONFIG_UBI
    snprintf(buffer,CMD_BUF, "nand erase %s;nand write.e %x %s %x",partname,ulVAddr,partname,ulTeeImageSize);
#else
    snprintf(buffer,CMD_BUF, "mmc write.p.continue %x %s 0 %x",ulVAddr,partname,ulTeeImageSize);
#endif
    UBOOT_DEBUG("cmd: %s \n",buffer);
    ret=run_command(buffer,0);
    if(ret!=0)
    {
        UBOOT_ERROR("cmd: %s \n",buffer);
    }
    return ret;
}
#endif
#ifdef HB_CUS
static MS_BOOL MApi_LDR_CheckValidImage(unsigned int u32CusOUI, unsigned int u32SwModel, unsigned int u32SwVer, char* cChipVersion, char* cBoardSel)
{
    char *pTemp;
	unsigned int CustomerOUI = 0x169B;
    unsigned int AppSwModel=0;
    unsigned int AppSwVersion=0;
    char ChipVersion[10]="";
    char BoardSel[10]="";
    unsigned int BuildCL=0;

    pTemp = getenv("CUSTOMER_OUI");
    if (pTemp)
    {
        CustomerOUI = simple_strtoul(pTemp, NULL, 16);
    }

    pTemp = getenv("AP_SW_MODEL");
    if (pTemp)
    {
        AppSwModel = simple_strtoul(pTemp, NULL, 16);
    }

    pTemp = getenv("AP_SW_VERSION");
    if (pTemp)
    {
        AppSwVersion = simple_strtoul(pTemp, NULL, 16);
    }

    pTemp = getenv("CHIP_VERSION");
    if (pTemp)
    {
        strcpy(ChipVersion,pTemp);
    }

    pTemp = getenv("BOARD_TYPE_SEL");
    if (pTemp)
    {
        strcpy(BoardSel,pTemp);
    }

    if (u32CusOUI == CustomerOUI && u32SwModel == AppSwModel && u32SwVer >= AppSwVersion && strcmp(cChipVersion,ChipVersion) == 0 && strcmp(cBoardSel,BoardSel) == 0)
	{
        return TRUE;
    }

    else
    {
        printf("MApi_LDR_CheckValidImage(%x,%x,%x,%s,%s,%d), Invalid\n", CustomerOUI, AppSwModel, AppSwVersion,ChipVersion,BoardSel,BuildCL);
        return 0;
    }
}
int do_checkOUIValid(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    unsigned int oui;
    unsigned int swModel;
    unsigned int swVer;
    char chip_version[10];
    char board_sel[10];

    if(argc!=6)
    {
        printf("too few arguments %d to check, please check header in bin file!!\n",argc);
        return -1;
    }

    oui = simple_strtoul(argv[1], NULL, 16);
    swModel = simple_strtoul(argv[2], NULL, 16);
    swVer   = simple_strtoul(argv[3], NULL, 16);

    strcpy(chip_version,argv[4]);
    strcpy(board_sel,argv[5]);
    if (MApi_LDR_CheckValidImage(oui, swModel, swVer,chip_version,board_sel))
    {
        //gIsCusInfoMatched = 1;
        return 0;
    }
    return -1;
}

#endif
#if (ENABLE_MINIUBOOT==0)
int Print_UsbInfo(char *downloadBuf)
{
    #define USB_INFO_CNT 4
    int cnt = USB_INFO_CNT;
    char *line_buf=NULL;
    char *current_line=NULL;
    char *next_line=NULL;

    UBOOT_TRACE("IN\n");

    line_buf=malloc(CMD_BUF);
    if(line_buf==NULL)
    {
        UBOOT_ERROR("malloc fail\n");
        return -1;
    }
    memset(line_buf,0,CMD_BUF);

    if(strstr (downloadBuf, "USB Upgrade Bin Info") == NULL)
    {
        UBOOT_DEBUG("No USB Upgrade bin info in script\n");
        free(line_buf);
        UBOOT_TRACE("OK\n");
        return 0;
    }

    current_line = downloadBuf;
    while(cnt)
    {
        next_line = strstr (current_line, "\n");
        if((next_line - current_line) <= CMD_BUF)
        {
            snprintf(line_buf, next_line - current_line, "%s", current_line);
            current_line = next_line + 1;
            UBOOT_INFO("%s\n", line_buf);
            cnt --;
        }
        else
        {
            UBOOT_ERROR("cmd line size exceed %d\n", CMD_BUF);
            free(line_buf);
            return -1;
        }
    }
    free(line_buf);
    UBOOT_TRACE("OK\n");
    return 0;
}
#endif

