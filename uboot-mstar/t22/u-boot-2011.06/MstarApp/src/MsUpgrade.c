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
#include <stdio.h>
#include <MsUpgrade.h>
#include <MsUpgradeUtility.h>
#include <MsUtility.h>
#include <config.h>
#include <MsTypes.h>
#include <ShareType.h>
#include <MsVfs.h>
#include <MsString.h>
#include <MsDebug.h>
#include <MsRawIO.h>
#include <MsAppCB.h>
#include <CusConfig.h>
#include <MsSystem.h>
#ifdef CONFIG_USB_PC_UPGRADE_LOADER
#include "ms_msd_global.h"
#include "drvUSBDwld.h"
#endif
#if defined (CONFIG_URSA_6M60)
#include <ursa/ursa_6m60.h>
#endif

#ifdef CONFIG_AN_FASTBOOT_ENABLE
#include <sparse_format.h>
#ifdef CONFIG_FASTBOOT_FLASH_MMC_DEV
#include <fb_mmc.h>
#endif
#endif

#if defined(CONFIG_SECURE_UPGRADE_WITH_FAIL_HANDLER)
#include <secure/MsSecureUpgrade.h>
#endif

#if defined(UFBL_FEATURE_ONETIME_UNLOCK)
#include <amzn_onetime_unlock.h>
#endif
//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define SCRIPT_BUF_FOR_DO_MSTAR 4096
#define DEFAULT_BIN_PATH             "/MstarUpgrade.bin"
#define DEFAULT_SCRIPT_FILE_NAME    "auto_update.txt"   // script file name
#define DEFAULT_DEBUG_SCRIPT_PATH             "/MstarDebug.script"

#define ARG_NUM_SCRIPT_FILE         1
#define IS_ARG_AVAILABLE_SCRIPT_FILE(x) ((x) > ARG_NUM_SCRIPT_FILE)
#define ARG_SCRIPT_FILE(x)              (x)[ARG_NUM_SCRIPT_FILE]
#define MAX_SCRIPT_SIZE 0x20000

#if defined(CONFIG_KENYA)
#include <configs/kenya.h>
#ifdef HB_CUS
#include "usb.h"
#include "fat.h"
#include "common.h"
#endif
#endif

#ifdef CONFIG_BOOT_SEQUENCE_FORCE_USB_UPGRADE_LOADER
//MBOOT_LOADER_HEADER
#define LOADER_HEADER_SIZE_OFFSET       0
#define LOADER_HEADER_VERSION_OFFSET    1
#define LOADER_HEADER_CMD_OFFSET        2   // 2 bytes
#define LOADER_HEADER_DATA_SIZE_OFFSET  4   // 4 bytes
#define LOADER_HEADER_ADDR_OFFSET       8   // 4 bytes

#define USB_UPGRADE_HEADER_SIZE 0x20

#ifdef CONFIG_USB_VAR_MEM_ADDR
#define LOADER_USB_MEM_ADDR CONFIG_USB_VAR_MEM_ADDR
#else
#define LOADER_USB_MEM_ADDR 0x30280000
#endif

#define LOAD_ADDR LOADER_USB_MEM_ADDR + 0x80000 //need to alignment 32 bytes
#define OTP_USB_MEM_ADDR 0x20280000
#define MAX_DATA_BUFFER 0x10000 // 64KB
#define USB_LOG_BUFFER_SIZE 0x1000

typedef enum
{
    COMMAND_TYPE_SCRIPT   =0x8080,
    COMMAND_TYPE_COPYDATA =0x8181,
    COMMAND_TYPE_NOP      =0x8888,
    COMMAND_TYPE_RESET    =0x8989
}USB_Upgrade_COMMAND_TYPE;

typedef struct _USB_Upgrade_Header
{
    U32 HeaderSize;
    U32 Version;
    U32 Command;
    U32 DataSize;
    U32 Address;
}USB_Upgrade_Header;

typedef struct logbuffer
{
    char logbuffer[USB_LOG_BUFFER_SIZE];
    int start;
    int end;
    int count;
}USB_LogBuffer;

USB_LogBuffer g_usb_log;

#define USB_LOG_DEBUG(_usb_buf_, msg...)\
    do{\
        sprintf(_usb_buf_, msg);\
        UBOOT_DEBUG(_usb_buf_);\
        usb_push_log(_usb_buf_, strlen(_usb_buf_));\
    }while(0)

#define USB_LOG_INFO(_usb_buf_, msg...)\
    do{\
        sprintf(_usb_buf_, msg);\
        UBOOT_INFO(_usb_buf_);\
        usb_push_log(_usb_buf_, strlen(_usb_buf_));\
    }while(0)
#endif

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
#ifdef CONFIG_UPGRADE_WITH_FAIL_HANDLER
ST_IMAGE_OFFSET_INFO g_st_image_offset_info;
ST_CHUNK_INFO g_st_REE_chunk_info;
ST_CHUNK_INFO g_st_TEE_chunk_info;
#endif

// the tftp address is set if flag = 1, not set yet if flag = 0
int g_tftp_set_flag = 0;

// for auto-calculate tftp address
unsigned int g_tftp_start_address;
unsigned int g_tftp_end_address;
unsigned int g_lzo_start_address;
unsigned int g_lzo_len;

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
#if ((ENABLE_MODULE_OAD == 1) || (ENABLE_MODULE_USB == 1) || (ENABLE_MODULE_NETUPDATE == 1) || (CONFIG_RESCUE_ENV))
static show_Error_cb            fpShow_Error = NULL;
static show_Finish_cb           fpShow_Finish = NULL;
static show_LoadData_cb         fpShow_LoadData = NULL;
static show_StartUpgrading_cb   fpShow_StartUpgrading = NULL;
static show_Upgrading_cb        fpShow_upgrading = NULL;
#endif

#ifdef CONFIG_USB_PC_UPGRADE_LOADER
static U8 usb_data_buf[MAX_DATA_BUFFER] __attribute__((aligned(32)));
static U8 usb_ack_buf[4] __attribute__((aligned(32)));
static U8 usb_ack[4] __attribute__((aligned(32)));
static U8 usb_log_buf[USB_LOG_BUFFER_SIZE] __attribute__((aligned(32)));

static U8 *usb_load_addr = (U8 *)(LOAD_ADDR-USB_UPGRADE_HEADER_SIZE);
static USB_VAR *g_usb_var = (USB_VAR *)LOADER_USB_MEM_ADDR;
static vendorCmdStruct g_scsiVendorCmdVar;
static USB_Upgrade_Header Header;
#endif

#ifdef CONFIG_USB_DISK_UPGRADE_LOADER
static MS_U32 g_check_timer_id;
static enum USB_LED {LED_ON, LED_OFF};
static enum USB_LED led_status;
#endif

static int g_USB_partition = 0;
static int g_USB_device = 0;
static unsigned char gfileSizeReady=FALSE;
static unsigned int gfileSize=0;

#if defined(CONFIG_NET) && defined(CONFIG_TFTP_MAX_USAGE_MEMORY_SIZE)
//static const unsigned int MAX_TFTP_SIZE = 0x7800000; // 0x7800000=120MB, 0xC800000=200MB
#define MAX_TFTP_SIZE CONFIG_TFTP_MAX_USAGE_MEMORY_SIZE
#else
#define MAX_TFTP_SIZE 0x7800000
#endif

#if defined(CONFIG_LZO) && defined(CONFIG_UNLZO_MAX_USAGE_MEMORY_SIZE)
#define MAX_LZO_SIZE CONFIG_UNLZO_MAX_USAGE_MEMORY_SIZE
#else
//static const unsigned int MAX_LZO_SIZE = 0x1400000; // 0x1400000=20MB, 0x3200000=50MB
#define MAX_LZO_SIZE 0x1400000
#endif
//-------------------------------------------------------------------------------------------------
//  Extern Functions
//-------------------------------------------------------------------------------------------------
extern int snprintf(char *str, size_t size, const char *fmt, ...);
#ifdef CONFIG_USB_PC_UPGRADE_LOADER
extern void MDrv_USB_CmdLoop(void *pUsbStruct);
#endif

//-------------------------------------------------------------------------------------------------
//  Private Functions
//-------------------------------------------------------------------------------------------------
#if (ENABLE_MODULE_USB == 1)
#ifdef CONFIG_UPGRADE_WITHOUT_FAIL_HANDLER
static int LoadScript2DramByUsb(char *upgradeFile, char *downloadBuf);
#endif

#ifdef CONFIG_UPGRADE_WITH_FAIL_HANDLER
static int read_firmware_image_offset_info(char *upgradeFile);
static int checkUpgradeFileAvailable(char *upgradeFile);
static int LoadREEScript2DramByUsb(char *upgradeFile, char *downloadBuf);
static int LoadTEEScript2DramByUsb(char *upgradeFile, char *downloadBuf);
#endif
#endif

#if (ENABLE_MODULE_OAD == 1) || (ENABLE_MODULE_NETUPDATE == 1 || (CONFIG_RESCUE_ENV))
#if defined (CONFIG_OAD_IN_MBOOT)
static int LoadScript2DramByOAD(void);
#endif
static int LoadScript2DramByFS(char *upgradeFile, char *downloadBuf);
static int LoadScript2DramByFSFromVolume(char *upgradeFile, char *downloadBuf, char *volume);
#endif

#if (ENABLE_MODULE_OAD == 1) || (ENABLE_MODULE_NETUPDATE == 1)||(ENABLE_MODULE_USB == 1 || (CONFIG_RESCUE_ENV))
static int _show_Error(unsigned int var);
static int _show_Finish(unsigned int var);
static int _show_LoadData(unsigned int var);
static int _show_StartUpgrading(unsigned int var);
static int _show_Upgrading(unsigned int var1, unsigned int var2);
static unsigned int _check_upgrade_script_cmd_count(char *buffer);
#endif

#ifdef CONFIG_USB_DISK_UPGRADE_LOADER
u8 readb(const volatile void *addr)
{
    return *(const volatile u8 *) addr;
}

void writeb(u8 b, volatile void *addr)
{
    *(volatile u8 *) addr = b;
}
#endif

//-------------------------------------------------------------------------------------------------
//  Public Functions
//-------------------------------------------------------------------------------------------------
unsigned int GetfileSizeforUpgrade(const char *Interface,const char *device,char *file)
{
    char *buffer=NULL;
    int ret=0;
    UBOOT_TRACE("IN\n");

    if(gfileSizeReady==TRUE)
    {
        UBOOT_TRACE("OK\n");
        return gfileSize;
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
#if (ENABLE_MODULE_VFS == 1)
        gfileSize=vfs_getsize(file);
#else
        return -1;
#endif
    }
    else if(getUpgradeMode()==EN_UPDATE_MODE_USB)
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
    UBOOT_TRACE("OK\n");
    gfileSizeReady=TRUE;
    return gfileSize;
}

#if (ENABLE_MODULE_NET == 1)
int do_mstar (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    char* buffer = NULL;
    char *script_buf = NULL;
    char *next_line = NULL;
    char *ScriptFile = NULL;
    unsigned long iFilesize = 0;
    char *tmp = NULL;
    int BeAbort = 1;
    UBOOT_TRACE("IN\n");
    setUpgradeMode(EN_UPDATE_MODE_TFTP);

    ScriptFile= IS_ARG_AVAILABLE_SCRIPT_FILE(argc) ? ARG_SCRIPT_FILE(argv) : DEFAULT_SCRIPT_FILE_NAME;
    UBOOT_DEBUG("ScriptFile=%s\n",ScriptFile);

    buffer=(char *)malloc(SCRIPT_BUF_FOR_DO_MSTAR);
    if(buffer==NULL)
    {
        UBOOT_ERROR("no memory for command string!!\n");
        return -1;
    }

    // Set mstar commmand abort or not when cmd error.
    tmp = getenv ("CmdAbort");
    if (tmp != NULL)
    {
        BeAbort = (int)simple_strtol(tmp, NULL, 10);
        UBOOT_DEBUG("BeAbort=%d\n",BeAbort);
    }

    //Enable emac
    if(-1 == run_command("estart", 0))
    {
        UBOOT_ERROR("estart Fail \n");
        free(buffer);
        return -1;
    }

    // Set DHCP
    if(getenv("ipaddr") == NULL)
    {
        if(-1 == run_command("dhcp",0))
        {
            UBOOT_ERROR("please use the legal mac address.\n");
            free(buffer);// Fix CID:147674
            return -1;
        }
    }
    else
    {
        UBOOT_DEBUG("ipaddr=%s\n",getenv("ipaddr") );
    }
    // Load script
    memset(buffer, 0 , CMD_BUF);
    snprintf(buffer, CMD_BUF, "tftp %X %s", buffer,ScriptFile);
    UBOOT_DEBUG("cmd=%s\n",buffer);
    if(-1 == run_command(buffer, 0))
    {
        UBOOT_ERROR("cmd: %s\n",buffer);
        free(buffer);
        return -1;
    }

    // because application member is lazy, so we must set "sync_mmap 1" after burn code,
    run_command("setenv sync_mmap 1;saveenv", 0);

    // Run script
    iFilesize = (unsigned long)simple_strtol(getenv("filesize"), NULL, 16);
    UBOOT_DEBUG("iFilesize=0x%x\n",(unsigned int)iFilesize);
    script_buf = buffer;
    script_buf[iFilesize]='\n'; script_buf[iFilesize+1]='%';
    while ((next_line = get_script_next_line(&script_buf)) != NULL)
    {
        next_line = ParaReplace(next_line, (argc-2), &argv[1]);
        UBOOT_INFO ("\n>> %s \n", next_line);
        if(-1 == run_command(next_line, 0))
        {
            UBOOT_ERROR("command error!\n");
            if(BeAbort == 1)
            {
                free(buffer);
                return -1;
            }
        }
    }
    free(buffer);
    UBOOT_TRACE("OK\n");
    return 0;
}
#endif


int read_dynamic_script_size(char *ScriptBuf)
{
    UBOOT_TRACE("IN\n");
    // read dynamic script size
    unsigned int tmp_script_size=0;
    sscanf(ScriptBuf, "# Script Size = 0x%x #", &tmp_script_size);

    if(tmp_script_size==0)
    {
        UBOOT_DEBUG("no support dynamic script size\n");
        return -1;
    }

    UBOOT_DEBUG("# Script Size = 0x%x #",tmp_script_size);
    setScritSizeOfUpgradeImage(tmp_script_size);

    UBOOT_TRACE("OK\n");
    return 0;
}

#if (ENABLE_MODULE_USB == 1)
#ifdef CONFIG_UPGRADE_WITHOUT_FAIL_HANDLER
static int LoadScript2DramByUsb(char *upgradeFile, char *downloadBuf)
{
    char *buffer=NULL;
    int partition = 0;
    int device = 0;
    UBOOT_TRACE("IN\n");
    if(check_file_partition(upgradeFile,&device,&partition)!=0)
    {
        UBOOT_ERROR("fail : no file in usb\n");
        return -1;
    }
    buffer=malloc(CMD_BUF);
    if(buffer==NULL)
    {
        UBOOT_ERROR("malloc fail\n");
        return -1;
    }
    memset(buffer,0,CMD_BUF);

    //Load Script
    snprintf(buffer, CMD_BUF, "fatload usb %d:%d  %X %s %X",device, partition, downloadBuf, upgradeFile, getScritSizeOfUpgradeImage());
    UBOOT_DEBUG("cmd=%s\n",buffer);
    if(-1 == run_command(buffer, 0))
    {

        UBOOT_ERROR("Error: cmd: %s\n",buffer);
        free(buffer);//Coverity CID:149588
        return -1;
    }


    /*Check script file is ciphertext or not(start)*/
#if (CONFIG_SECURE_USB_UPGRADE==1)
     if(check_script_encrypt(downloadBuf) == TRUE)
     {
        setUpgradeMode(EN_UPDATE_MODE_USB_WITH_SEG_DECRYPTED);
        memset(buffer, 0 , CMD_BUF);
        snprintf(buffer, CMD_BUF, "fileSegRSA usb %d:%d %s",device, partition, upgradeFile);
        UBOOT_DEBUG("cmd=%s\n",buffer);
        if(-1 == run_command(buffer, 0))
        {
            UBOOT_ERROR("Error: cmd: %s\n",buffer);
            free(buffer);
            Reset_System();
        }

        memset(buffer, 0 , CMD_BUF);
        snprintf(buffer, CMD_BUF, "filepartload %x %s %x %x",downloadBuf, upgradeFile, 0, getScritSizeOfUpgradeImage());
        UBOOT_DEBUG("cmd=%s\n",buffer);
        if(-1 == run_command(buffer, 0))
        {
            UBOOT_ERROR("Error: cmd: %s\n",buffer);
            free(buffer);
            Reset_System();
        }

        // read dynamic script size
        if(read_dynamic_script_size(downloadBuf) == 0)
        {
            memset(buffer, 0 , CMD_BUF);
            snprintf(buffer, CMD_BUF, "filepartload %x %s %x %x",downloadBuf, upgradeFile, 0, getScritSizeOfUpgradeImage());
            UBOOT_DEBUG("cmd=%s\n",buffer);
            if(-1 == run_command(buffer, 0))
            {
                UBOOT_ERROR("Error: cmd: %s\n",buffer);
                free(buffer);
                Reset_System();
            }
        }
     }
     else
     {
         //Reset system if #define of secure upgrade is enable, but image is not encrypted
         UBOOT_ERROR("Error: Image format is not for secure upgrade!!\n");
         Reset_System();
         return -1;
     }
#else
     {
        // read dynamic script size
        if(read_dynamic_script_size(downloadBuf) == 0)
        {
            //load script again
            memset(buffer, 0 , CMD_BUF);
            snprintf(buffer, CMD_BUF, "fatload usb %d:%d  %X %s %X",device, partition, downloadBuf, upgradeFile, getScritSizeOfUpgradeImage());
            UBOOT_DEBUG("cmd=%s\n",buffer);
            if(-1 == run_command(buffer, 0))
            {
                UBOOT_ERROR("Error: cmd: %s\n",buffer);
                free(buffer);//Coverity CID:149588
                return -1;
            }
        }

        //check script crc
        if(check_script_crc(upgradeFile,downloadBuf)==-1)
        {
            UBOOT_ERROR("Error: check_script_crc Fail !!\n");
            free(buffer);
            return -1;
        }

        if(check_full_usb_crc(upgradeFile,downloadBuf)==-1)
        {
            UBOOT_ERROR("Error: check_full_usb_crc Fail !!\n");
            free(buffer);
            return -1;
        }
     }
#endif
#if (ENABLE_MINIUBOOT==0)
     Print_UsbInfo(downloadBuf);
#endif
     free(buffer);
     UBOOT_TRACE("OK\n");
     return 0;
}

#if defined(UFBL_FEATURE_ONETIME_UNLOCK)
int do_onetimeunlock(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    int ret = -1;
    unsigned char *b64;
    unsigned int out_len;

    if (argc < 2) {
		ret = -1;
		goto done;
    }

    if (!strcmp(argv[1], "getcode")) {
        unsigned char one_tu_code[ONETIME_UNLOCK_CODE_LEN + 1] = {0};
        unsigned int unlock_code_len = sizeof(one_tu_code);

        if (amzn_get_one_tu_code(one_tu_code, &unlock_code_len)) {
	    printf("cannot get onetime unlock code\n");
	    ret = -2;
            goto done;
         } else {
	    printf("%s\n", one_tu_code);
         }
    } else if (!strcmp(argv[1], "setcode")) {

        if(argc < 3) {
	    ret = -3;
            goto done;
        }
        b64 = (unsigned char *)argv[2];
	out_len = strlen(b64);

        if (amzn_set_onetime_unlock_code(b64, out_len)) {
            printf("set onetime unlock code error\n");
	    ret = -4;
            goto done;
        } else {
            printf("set onetime unlock code OKAY\n");
        }
    } else if (!strcmp(argv[1], "setcert")) {

        if(argc < 3) {
	    ret = -5;
            goto done;
        }
        b64 = (unsigned char *)argv[2];
	out_len = strlen(b64);

        if (amzn_set_onetime_unlock_cert(b64, out_len)) {
            printf("set onetime unlock cert error\n");
	    ret = -6;
            goto done;
        } else {
            printf("set onetime unlock cert OKAY\n");
        }
    } else {
	ret = -10;
	goto done;
    }
	ret = 0;
done:
    if (ret)
	printf("do_onetimeunlock fail: %d\n", ret);
    else
	printf("do_onetimeunlock pass\n");
    return 0;
}
#endif

int do_ustar(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    char *script_buf = NULL;
    char *_script_buf = NULL;
    char *next_line = NULL;
    char *upgradeFile = NULL;
    unsigned int scriptFileSize=0;
    unsigned int progress_total_cnt = 0;
    unsigned int progress_cnt = 0;

    UBOOT_TRACE("IN\n");
    setUpgradeMode(EN_UPDATE_MODE_USB); // set the Upgrade mode
    /* Load Script */
    _show_LoadData(0);

    #ifdef CONFIG_BOOTLOGO_LITE
    char progress_cmd[64] = {0};
    run_command("bootupgrade_init", 0);
    run_command("bootupgrade_clean", 0);
    run_command("bootupgrade_ui", 0);
    #endif
    upgradeFile = IS_ARG_AVAILABLE_SCRIPT_FILE(argc) ? ARG_SCRIPT_FILE(argv) : DEFAULT_BIN_PATH;

    scriptFileSize=MAX_SCRIPT_SIZE;
    if(scriptFileSize==0)
    {
        UBOOT_ERROR("The size of script file is zero \n");
        #ifdef CONFIG_BOOTLOGO_LITE
        run_command("bootupgrade_clean", 0);
        #endif
        return -1;
    }

    _script_buf=malloc(scriptFileSize);
    if(_script_buf==NULL)
    {
        UBOOT_ERROR("malloc fail \n");
        UBOOT_ERROR("The size of script file is zero \n");
        #ifdef CONFIG_BOOTLOGO_LITE
        run_command("bootupgrade_clean", 0);
        #endif
        return -1;
    }
    script_buf=_script_buf;
    memset(script_buf,0,scriptFileSize);


    if(setUpgradeFileName(upgradeFile)==-1)
    {
        setUpgradeMode(EN_UPDATE_MODE_NONE);
        free(_script_buf);
        UBOOT_ERROR("Error> set upgrade fail fail\n");
        _show_Error(0);
        UBOOT_ERROR("The size of script file is zero \n");
        #ifdef CONFIG_BOOTLOGO_LITE
        run_command("bootupgrade_clean", 0);
        #endif
        return -1;
    }
    if(LoadScript2DramByUsb(upgradeFile,script_buf) == -1)
    {
        deleteUpgradeFileName();
        free(_script_buf);
        setUpgradeMode(EN_UPDATE_MODE_NONE);
        _show_Error(0);
        #ifdef CONFIG_BOOTLOGO_LITE
        run_command("bootupgrade_clean", 0);
        #endif
        return -1;
    }
    #if(ENABLE_MODULE_DONT_OVERWRITE==1)
    extern int SKIP_DONT_OVERWRITE;
    if(SKIP_DONT_OVERWRITE == 0)
        set_dont_overwrite_byname(upgradeFile);
    #endif
    //Run Script

    //script_buf = (char *)UPGRADE_BUFFER_ADDR;

    progress_total_cnt=_check_upgrade_script_cmd_count(script_buf);
    if(progress_total_cnt==0)
    {
        deleteUpgradeFileName();
        setUpgradeMode(EN_UPDATE_MODE_NONE);
        free(_script_buf);
        UBOOT_ERROR("There are no any commands in the upgrade script file\n");
        _show_Error(0);
        #ifdef CONFIG_BOOTLOGO_LITE
        run_command("bootupgrade_clean", 0);
        #endif
        return -1;
    }
    UBOOT_DEBUG("progress_total_cnt=%d\n",progress_total_cnt);

    _show_StartUpgrading(0);
    #ifdef CONFIG_BOOTLOGO_LITE
    memset((void*)progress_cmd,0,64);
    sprintf(progress_cmd, "bootupgrade_progress %d",progress_cnt%4);
    run_command(progress_cmd, 0);
    #endif

    // because application member is lazy, so we must set "sync_mmap 1" after burn code,
    run_command("setenv sync_mmap 1;saveenv", 0);

    while ((next_line = get_script_next_line(&script_buf)) != NULL)
    {

        UBOOT_INFO("\n>> %s \n", next_line);
        progress_cnt++;
        UBOOT_DEBUG("progress_cnt=%d,progress_total_cnt=%d\n",progress_cnt,progress_total_cnt);
        _show_Upgrading(progress_cnt,progress_total_cnt);
        if(check_skip_cmd(next_line)== FALSE)//filterCmd
        {
            if(run_command(next_line, 0) < 0)
            {
                deleteUpgradeFileName();
                setUpgradeMode(EN_UPDATE_MODE_NONE);
                free(_script_buf);
                UBOOT_ERROR("Error> cmd: %s Fail !!\n", next_line);
                _show_Error(0);
                #ifdef CONFIG_BOOTLOGO_LITE
                run_command("bootupgrade_clean", 0);
                #endif
                return -1;

            }
        }
        #ifdef CONFIG_BOOTLOGO_LITE
        memset((void*)progress_cmd,0,64);
        sprintf(progress_cmd, "bootupgrade_progress %d",progress_cnt%4);
        run_command(progress_cmd, 0);
        #endif
    }

    if(deleteUpgradeFileName()==-1)
    {
        UBOOT_ERROR("delete upgrade name fail\n");
    }

    #if(ENABLE_MODULE_DONT_OVERWRITE==1)
    extern int SKIP_DONT_OVERWRITE;
    SKIP_DONT_OVERWRITE = 0;
    #endif

    free(_script_buf);
    _show_Finish(0);
    #ifdef CONFIG_BOOTLOGO_LITE
    run_command("bootupgrade_clean", 0);
    #endif
    UBOOT_TRACE("OK\n");
    return 0;
}

#if !((CONFIG_MINIUBOOT) || (CONFIG_SECURE_UPGRADE_WITHOUT_FAIL_HANDLER))
int do_udstar (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    char *cmdBuf=NULL;
    char *script_buf=NULL;
    char *pBuf=NULL;
    char *next_line=NULL;
    char *upgradeFile=NULL;
    char *env=NULL;
    int ret=0;
    unsigned int u32FileSize=0;
    int partition = 0;
    int device = 0;
    UBOOT_TRACE("IN\n");

    upgradeFile = IS_ARG_AVAILABLE_SCRIPT_FILE(argc) ? ARG_SCRIPT_FILE(argv) : DEFAULT_DEBUG_SCRIPT_PATH;

    if(check_file_partition(upgradeFile,&device,&partition)!=0)
    {
        UBOOT_ERROR("no file in usb\n");
        return -1;
    }

    UBOOT_DEBUG("upgradeFile=%s\n",upgradeFile);

    cmdBuf=malloc(CMD_BUF);
    if(cmdBuf==NULL)
    {
        UBOOT_ERROR("malloc fail\n");
        return -1;
    }
    memset(cmdBuf, 0 , CMD_BUF);
    snprintf(cmdBuf, CMD_BUF, "fatload usb %d:%d %X %s 1", device,partition,cmdBuf, upgradeFile);
    UBOOT_DEBUG("cmd=%s\n",cmdBuf);
    ret=run_command(cmdBuf, 0);
    if(ret!=0)
    {
        free(cmdBuf);
        UBOOT_ERROR("No script file exist\n");
        return -1;
    }

    memset(cmdBuf, 0, CMD_BUF);
    snprintf(cmdBuf, CMD_BUF, "fatfilesize usb %d:%d %s", device,partition, upgradeFile);
    UBOOT_DEBUG("cmd=%s\n",cmdBuf);
    ret = run_command(cmdBuf, 0);
    if(ret!=0)
    {
        free(cmdBuf);
        UBOOT_ERROR("get script file's size fail\n");
        return -1;
    }
    // Get file size. If larger then DRAM, don't load the whole. Just load its script.
    env=getenv("filesize");
    if(env==NULL)
    {
       free(cmdBuf);
       UBOOT_ERROR("get env 'filesize' fail\n");
       return -1;
    }
    u32FileSize = simple_strtol(env, NULL, 16);
    pBuf=malloc(u32FileSize);
    if(pBuf==NULL)
    {
        free(cmdBuf);
        UBOOT_ERROR("malloc for script file fail\n");
        return -1;
    }
    script_buf=pBuf;
    memset(script_buf,0,u32FileSize);

    memset(cmdBuf, 0, CMD_BUF);
    snprintf(cmdBuf, CMD_BUF, "fatload usb %d:%d %X %s %X",device,partition,script_buf,upgradeFile,u32FileSize);
    UBOOT_DEBUG("cmd=%s\n",cmdBuf);
    ret = run_command(cmdBuf, 0);
    if(ret!=0)
    {
        free(pBuf);
        free(cmdBuf);
        return -1;
    }

    // because application member is lazy, so we must set "sync_mmap 1" after burn code,
    run_command("setenv sync_mmap 1;saveenv", 0);

    while ((next_line = get_script_next_line(&script_buf)) != NULL)
    {
        UBOOT_INFO ("\n>>1 %s \n", next_line);

        if(strstr((char *)next_line,(char *)"run")!=NULL)
        {
            UBOOT_INFO ("skip cmd 'run' \n");
            continue;
        }

        ret=run_command(next_line, 0);
        if(ret!=0)
        {
            free(pBuf);
            free(cmdBuf);
            UBOOT_ERROR("run cmd '%s' fail\n",next_line);
            return -1;
        }
    }

    free(cmdBuf);
    free(pBuf);
    UBOOT_TRACE("OK\n");
    return 0;
}
#endif //#ifndef CONFIG_MINIUBOOT && CONFIG_UPGRADE_WITHOUT_FAIL_HANDLER
#endif // #ifdef CONFIG_UPGRADE_WITHOUT_FAIL_HANDLER

#ifdef CONFIG_UPGRADE_WITH_FAIL_HANDLER
static int checkUpgradeFileAvailable(char *upgradeFile)
{
    UBOOT_TRACE("IN\n");
    int partition = 0;
    int device = 0;
    char device_partition[10] = {0};
    unsigned int upgrade_fileSize = 0;

    if(check_file_partition(upgradeFile,&device,&partition)!=0)
    {
        UBOOT_ERROR("fail : no file in usb\n");
        return -1;
    }

    g_USB_device = device;
    g_USB_partition = partition;

    memset(device_partition, 0, 10);
    sprintf(device_partition, "%d:%d", g_USB_device, g_USB_partition);
    upgrade_fileSize = GetfileSizeforUpgrade("usb", device_partition, upgradeFile);
    if( upgrade_fileSize <= 0 )
    {
        UBOOT_ERROR("file size of upgrade image not valid\n");
        return -1;
    }

    UBOOT_TRACE("OK\n");
    return 0;
}

static int read_firmware_image_offset_info(char *upgradeFile)
{
    UBOOT_TRACE("IN\n");
    char header[PACK_HEADER_SIZE] = {0};
    char *ptrHeader = header;
    char *buffer=NULL;

    buffer=malloc(CMD_BUF);
    if(buffer==NULL)
    {
        UBOOT_ERROR("malloc fail\n");
        return -1;
    }
    memset(buffer,0,CMD_BUF);

    //Load header from offset 0 with size PACK_HEADER_SIZE
    snprintf(buffer, CMD_BUF, "filepartload %x %s %x %x",ptrHeader, upgradeFile, 0, PACK_HEADER_SIZE);
    UBOOT_DEBUG("cmd=%s\n",buffer);
    if(-1 == run_command(buffer, 0))
    {
        UBOOT_ERROR("Error: cmd: %s\n",buffer);
        free(buffer);
        return -1;
    }

    U32 u32TEEImageOffsetStart = 0;
    U32 u32TEEImageOffsetLEN = 0;
    U32 u32REEImageOffsetStart = 0;
    U32 u32REEImageOffsetLEN = 0;

    // shift offset to image offset info part
    ptrHeader+=SIGNATURE_SIZE;
    UBOOT_DUMP(ptrHeader, 0x200);

    sscanf(ptrHeader, "# TEE_OFFSET_START = 0x%x #\n"\
                   "# TEE_OFFSET_LEN = 0x%x #\n"\
                   "# REE_OFFSET_START = 0x%x #\n"\
                   "# REE_OFFSET_LEN = 0x%x #\n",\
           &u32TEEImageOffsetStart, &u32TEEImageOffsetLEN, &u32REEImageOffsetStart, &u32REEImageOffsetLEN);

    g_st_image_offset_info.u32TEEImageOffsetStart = u32TEEImageOffsetStart;
    g_st_image_offset_info.u32TEEImageOffsetLEN = u32TEEImageOffsetLEN;
    g_st_image_offset_info.u32REEImageOffsetStart = u32REEImageOffsetStart;
    g_st_image_offset_info.u32REEImageOffsetLEN = u32REEImageOffsetLEN;

    UBOOT_DEBUG("# TEE_OFFSET_START = 0x%x #\n", g_st_image_offset_info.u32TEEImageOffsetStart);
    UBOOT_DEBUG("# TEE_OFFSET_LEN = 0x%x #\n", g_st_image_offset_info.u32TEEImageOffsetLEN);
    UBOOT_DEBUG("# REE_OFFSET_START = 0x%x #\n", g_st_image_offset_info.u32REEImageOffsetStart);
    UBOOT_DEBUG("# REE_OFFSET_LEN = 0x%x #\n", g_st_image_offset_info.u32REEImageOffsetLEN);

    // pre-define offset for firmware image header
    g_st_image_offset_info.signOfHeaderSize = SIGNATURE_SIZE;
    g_st_image_offset_info.imageOffsetSize = HEADER_IMAGE_OFFSET_SIZE;
    g_st_image_offset_info.chunkInfoSize = HEADER_CHUNKINFO_SIZE;
    g_st_image_offset_info.signOfhashSetSize = SIGNATURE_SIZE;

    free(buffer);
    UBOOT_TRACE("OK\n");
    return 0;
}

static int LoadREEScript2DramByUsb(char *upgradeFile, char *downloadBuf)
{
    char *buffer=NULL;
    int upgrade_fileSize=0;
    int ret = 0;
    int load_offset = 0;
    char device_partition[10] = {0};
    UBOOT_TRACE("IN\n");
    setUpgradeMode(EN_UPDATE_MODE_USB);
    setScritSizeOfUpgradeImage(REE_SCRIPT_SIZE);

    memset(device_partition, 0, 10);
    sprintf(device_partition, "%d:%d", g_USB_device, g_USB_partition);

    buffer=malloc(CMD_BUF);
    if(buffer==NULL)
    {
        UBOOT_ERROR("malloc fail\n");
        return -1;
    }

    /*Check script file is ciphertext or not(start)*/
#ifdef CONFIG_SECURE_UPGRADE_WITH_FAIL_HANDLER
    memset(buffer,0,CMD_BUF);
    // Read REE Image Start Offset
    load_offset = g_st_image_offset_info.u32REEImageOffsetStart;
    //snprintf(buffer, CMD_BUF, "filepartload %x %s %x %x",downloadBuf, upgradeFile, load_offset, REE_SCRIPT_SIZE);
    snprintf(buffer, CMD_BUF, "filepartloadSeg %s %s %x %s %x %x", "usb",device_partition,(U32)downloadBuf, upgradeFile, load_offset, REE_SCRIPT_SIZE);
    UBOOT_DEBUG("cmd=%s\n",buffer);
    if(-1 == run_command(buffer, 0))
    {
        UBOOT_ERROR("Error: cmd: %s\n",buffer);
        free(buffer);
        return -1;
    }

    if(check_script_encrypt(downloadBuf) == TRUE)
    {
        setUpgradeMode(EN_UPDATE_MODE_USB_WITH_SEG_DECRYPTED);
        memset(buffer, 0 , CMD_BUF);
        load_offset = g_st_image_offset_info.u32REEImageOffsetStart;
        snprintf(buffer, CMD_BUF, "filepartloadSegAES %s %s %x %s %x %x", "usb",device_partition,(U32)downloadBuf, upgradeFile, load_offset, REE_SCRIPT_SIZE);
        UBOOT_DEBUG("cmd=%s\n",buffer);
        if(-1 == run_command(buffer, 0))
        {
            UBOOT_ERROR("Error: cmd: %s\n",buffer);
            free(buffer);
            Reset_System();
        }
    }
    else
    {
        //Reset system if #define of secure upgrade is enable, but image is not encrypted
        UBOOT_ERROR("Error: Image format is not for secure upgrade!!\n");
        Reset_System();
        return -1;
    }
#else
     {
        memset(buffer,0,CMD_BUF);
        // Read REE Image Start Offset
        load_offset = g_st_image_offset_info.u32REEImageOffsetStart;

        if(load_offset%0x200 != 0)
        {
            UBOOT_ERROR("Upgrade Image format is not vaild\n",buffer);
            free(buffer);
            return -1;
        }

        snprintf(buffer, CMD_BUF, "filepartload %x %s %x %x",downloadBuf, upgradeFile, load_offset, REE_SCRIPT_SIZE);
        //snprintf(buffer, CMD_BUF, "filepartloadSeg %s %s %x %s %x %x", "usb",device_partition,(U32)downloadBuf, upgradeFile, load_offset, REE_SCRIPT_SIZE);
        UBOOT_DEBUG("cmd=%s\n",buffer);
        if(-1 == run_command(buffer, 0))
        {
            UBOOT_ERROR("Error: cmd: %s\n",buffer);
            free(buffer);
            return -1;
        }
        //check script crc
        if(check_script_crc(upgradeFile,downloadBuf)==-1)
        {
            UBOOT_ERROR("Error: check_script_crc Fail !!\n");
            free(buffer);
            return -1;
        }

        if(check_full_usb_crc(upgradeFile,downloadBuf)==-1)
        {
            UBOOT_ERROR("Error: check_full_usb_crc Fail !!\n");
            free(buffer);
            return -1;
        }
     }
#endif
#if (ENABLE_MINIUBOOT==0)
     Print_UsbInfo(downloadBuf);
#endif
     free(buffer);
     UBOOT_TRACE("OK\n");
     return 0;
}

static int LoadTEEScript2DramByUsb(char *upgradeFile, char *downloadBuf)
{
    char *buffer=NULL;
    int upgrade_fileSize=0;
    int ret = 0;
    int load_offset = 0;
    char device_partition[10] = {0};
    UBOOT_TRACE("IN\n");

    if (g_st_image_offset_info.u32TEEImageOffsetLEN == 0)
    {
        UBOOT_DEBUG("No TEE image in firmware upgrade image, skip load TEE scripts!\n");
        UBOOT_TRACE("OK\n");
        return 0;
    }

    setUpgradeMode(EN_UPDATE_MODE_USB);
    setScritSizeOfUpgradeImage(TEE_SCRIPT_SIZE);

    memset(device_partition, 0, 10);
    sprintf(device_partition, "%d:%d", g_USB_device, g_USB_partition);

    buffer=malloc(CMD_BUF);
    if(buffer==NULL)
    {
        UBOOT_ERROR("malloc fail\n");
        return -1;
    }

#ifdef CONFIG_SECURE_UPGRADE_WITH_FAIL_HANDLER
    memset(buffer,0,CMD_BUF);
    // Read TEE Image Start Offset
    load_offset = g_st_image_offset_info.u32TEEImageOffsetStart;
    //snprintf(buffer, CMD_BUF, "filepartload %x %s %x %x",downloadBuf, upgradeFile, load_offset, TEE_SCRIPT_SIZE);
    snprintf(buffer, CMD_BUF, "filepartloadSeg %s %s %x %s %x %x", "usb",device_partition,(U32)downloadBuf, upgradeFile, load_offset, TEE_SCRIPT_SIZE);
    UBOOT_DEBUG("cmd=%s\n",buffer);
    if(-1 == run_command(buffer, 0))
    {
        UBOOT_ERROR("Error: cmd: %s\n",buffer);
        free(buffer);
        return -1;
    }
    if(check_script_encrypt(downloadBuf) == TRUE)
    {
        //Reset system if the script is encrypted
        UBOOT_ERROR("Error: TEE Script should not be encrypted!!\n");
        Reset_System();
        return -1;
    }
#else
    // Read TEE Image Start Offset
    load_offset = g_st_image_offset_info.u32TEEImageOffsetStart;
    snprintf(buffer, CMD_BUF, "filepartload %x %s %x %x",downloadBuf, upgradeFile, load_offset, TEE_SCRIPT_SIZE);
    UBOOT_DEBUG("cmd=%s\n",buffer);
    if(-1 == run_command(buffer, 0))
    {
        UBOOT_ERROR("Error: cmd: %s\n",buffer);
        free(buffer);
        return -1;
    }
#endif
    UBOOT_TRACE("OK\n");
    return 0;
}

int do_ustar(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    char *REE_script_buf = NULL;
    char *TEE_script_buf = NULL;
    char *next_line = NULL;
    char *upgradeFile = NULL;
    unsigned int scriptFileSize=0;
    unsigned int progress_total_cnt = 0;
    unsigned int progress_cnt = 0;
    int ret = 0;

    UBOOT_TRACE("IN\n");
    setUpgradeMode(EN_UPDATE_MODE_USB); // set the Upgrade mode
    /* Load Script */
    _show_LoadData(0);

    upgradeFile = IS_ARG_AVAILABLE_SCRIPT_FILE(argc) ? ARG_SCRIPT_FILE(argv) : DEFAULT_BIN_PATH;

    scriptFileSize=MAX_SCRIPT_SIZE;
    if(scriptFileSize==0)
    {
        UBOOT_ERROR("The size of script file is zero \n");
        return -1;
    }

    REE_script_buf=malloc(scriptFileSize);
    if(REE_script_buf==NULL)
    {
        UBOOT_ERROR("malloc fail \n");
        return -1;
    }

    TEE_script_buf=malloc(scriptFileSize);
    if(TEE_script_buf==NULL)
    {
        UBOOT_ERROR("malloc fail \n");
        return -1;
    }

    memset(REE_script_buf,0,scriptFileSize);
    memset(TEE_script_buf,0,scriptFileSize);

    if( setUpgradeFileName(upgradeFile) == -1 )
    {
        setUpgradeMode(EN_UPDATE_MODE_NONE);
        free(REE_script_buf);
        free(TEE_script_buf);
        UBOOT_ERROR("Error> set upgrade fail\n");
        _show_Error(0);
        return -1;
    }

    if( checkUpgradeFileAvailable(upgradeFile) == -1 )
    {
        setUpgradeMode(EN_UPDATE_MODE_NONE);
        free(REE_script_buf);
        free(TEE_script_buf);
        UBOOT_ERROR("Error> Check UpgradeFile Available fail\n");
        _show_Error(0);
        return -1;
    }

#if (CONFIG_SECURE_UPGRADE_WITH_FAIL_HANDLER==1)
    // Do firmware image header authendication
    if ( firmware_image_header_authendication(upgradeFile) != 0 )
    {
        UBOOT_ERROR("Firmware image header authendication FAIL!\n");
        free(REE_script_buf);
        free(TEE_script_buf);
        Reset_System();
        return -1;
    }
#endif

    if( read_firmware_image_offset_info(upgradeFile) == -1 )
    {
        setUpgradeMode(EN_UPDATE_MODE_NONE);
        free(REE_script_buf);
        free(TEE_script_buf);
        UBOOT_ERROR("Error> Read Firmware Image Offset Info fail\n");
        _show_Error(0);
        return -1;
    }

#if (CONFIG_SECURE_UPGRADE_WITH_FAIL_HANDLER==1)
    // Do image authendication & check the integrity of image
    if (firmware_image_authendication(upgradeFile, g_USB_device, g_USB_partition) != 0)
    {
        setUpgradeMode(EN_UPDATE_MODE_NONE);
        free(REE_script_buf);
        free(TEE_script_buf);
        UBOOT_ERROR("Error> Firmware Image Authendication fail\n");
        _show_Error(0);
        return -1;
    }
#endif

    if( LoadREEScript2DramByUsb(upgradeFile,REE_script_buf) == -1 )
    {
        deleteUpgradeFileName();
        free(REE_script_buf);
        free(TEE_script_buf);
        setUpgradeMode(EN_UPDATE_MODE_NONE);
        _show_Error(0);
        return -1;
    }
    if( LoadTEEScript2DramByUsb(upgradeFile,TEE_script_buf) == -1 )
    {
        deleteUpgradeFileName();
        free(REE_script_buf);
        free(TEE_script_buf);
        setUpgradeMode(EN_UPDATE_MODE_NONE);
        _show_Error(0);
        return -1;
    }
    #if(ENABLE_MODULE_DONT_OVERWRITE==1)
    set_dont_overwrite_byname(upgradeFile);
    #endif

    //setUpgradeMode(EN_UPDATE_MODE_USB_WITH_SEG);

    // Run REE Script
#if defined(CONFIG_SECURE_UPGRADE_WITH_FAIL_HANDLER)
    setUpgradeMode(EN_UPDATE_MODE_USB_WITH_SEG_DECRYPTED);
#endif
    setScritSizeOfUpgradeImage(REE_SCRIPT_SIZE);
    progress_total_cnt=_check_upgrade_script_cmd_count(REE_script_buf);
    if(progress_total_cnt==0)
    {
        deleteUpgradeFileName();
        setUpgradeMode(EN_UPDATE_MODE_NONE);
        free(REE_script_buf);
        free(TEE_script_buf);
        UBOOT_ERROR("There are no any commands in the upgrade script file\n");
        _show_Error(0);
        return -1;
    }
    UBOOT_DEBUG("progress_total_cnt=%d\n",progress_total_cnt);

    // Add TEE part's script counts
    progress_total_cnt += _check_upgrade_script_cmd_count(TEE_script_buf);

    _show_StartUpgrading(0);

    // because application member is lazy, so we must set "sync_mmap 1" after burn code,
    run_command("setenv sync_mmap 1;saveenv", 0);

    while ((next_line = get_script_next_line(&REE_script_buf)) != NULL)
    {

        UBOOT_INFO("\n>> %s \n", next_line);
        progress_cnt++;
        UBOOT_DEBUG("progress_cnt=%d,progress_total_cnt=%d\n",progress_cnt,progress_total_cnt);
        _show_Upgrading(progress_cnt,progress_total_cnt);
        if(check_skip_cmd(next_line)== FALSE)//filterCmd
        {
            if(run_command(next_line, 0) < 0)
            {
                deleteUpgradeFileName();
                setUpgradeMode(EN_UPDATE_MODE_NONE);
                free(REE_script_buf);
                free(TEE_script_buf);
                UBOOT_ERROR("Error> cmd: %s Fail !!\n", next_line);
                _show_Error(0);
                return -1;

            }
        }
    }

    // Run TEE Script
    if (g_st_image_offset_info.u32TEEImageOffsetLEN > 0)
    {
        setUpgradeMode(EN_UPDATE_MODE_USB);
        setScritSizeOfUpgradeImage(TEE_SCRIPT_SIZE);
        int check_tee_progress_total_cnt=_check_upgrade_script_cmd_count(TEE_script_buf);
        if(check_tee_progress_total_cnt==0)
        {
            deleteUpgradeFileName();
            setUpgradeMode(EN_UPDATE_MODE_NONE);
            free(REE_script_buf);
            free(TEE_script_buf);
            UBOOT_ERROR("There are no any commands in the upgrade script file\n");
            _show_Error(0);
            return -1;
        }
        UBOOT_DEBUG("check_tee_progress_total_cnt=%d\n",check_tee_progress_total_cnt);

        //_show_StartUpgrading(0);

        while ((next_line = get_script_next_line(&TEE_script_buf)) != NULL)
        {
            UBOOT_INFO("\n>> %s \n", next_line);
            progress_cnt++;
            UBOOT_DEBUG("progress_cnt=%d,progress_total_cnt=%d\n",progress_cnt,progress_total_cnt);
            _show_Upgrading(progress_cnt,progress_total_cnt);
            if(check_skip_cmd(next_line)== FALSE)//filterCmd
            {
                if(run_command(next_line, 0) < 0)
                {
                    deleteUpgradeFileName();
                    setUpgradeMode(EN_UPDATE_MODE_NONE);
                    free(REE_script_buf);
                    free(TEE_script_buf);
                    UBOOT_ERROR("Error> cmd: %s Fail !!\n", next_line);
                    _show_Error(0);
                    return -1;
                }
            }
        }
    }

    if(deleteUpgradeFileName()==-1)
    {
        UBOOT_ERROR("delete upgrade name fail\n");
    }

    free(REE_script_buf);
    free(TEE_script_buf);
    _show_Finish(0);
    UBOOT_TRACE("OK\n");
    return 0;
}

#if !((CONFIG_MINIUBOOT) || (CONFIG_SECURE_UPGRADE_WITH_FAIL_HANDLER))
int do_udstar (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    // not implenment
    return 0;
}
#endif //#ifndef CONFIG_MINIUBOOT && CONFIG_SECURE_UPGRADE_WITH_FAIL_HANDLER
#endif // CONFIG_UPGRADE_WITH_FAIL_HANDLER

#endif //#if (ENABLE_MODULE_USB == 1)

#if (ENABLE_MODULE_OAD == 1) || (ENABLE_MODULE_NETUPDATE == 1 || (CONFIG_RESCUE_ENV))
#if defined (CONFIG_OAD_IN_MBOOT)
static int LoadScript2DramByOAD(void)
{
    char buffer[CMD_BUF]="\0";
    char* p_str = NULL;
    unsigned int  u32Freq = 0;
    unsigned int  u32Pid  = 0;
    unsigned char u8Band = 0;
    unsigned char u8DVBC = 0;
    unsigned char u8DVBS = 0;
    unsigned int  u32QAM = 0;
    unsigned int  u16SYMBOLRATE = 0;
    unsigned char u8SDTT_OAD  = 0;
    unsigned int  u32GroupId = 0;
    unsigned int  u16Plp  = 0xffff;
    UBOOT_TRACE("IN\n");

    // Frequency
    p_str = getenv("oad_upgrade_freq");
    if(p_str == NULL)
    {
        return -1;
    }
    u32Freq = simple_strtoul (p_str, NULL, 10);
    // PID
    p_str = getenv("oad_upgrade_pid");
    if(p_str == NULL)
    {
        return -1;
    }
    u32Pid = simple_strtoul (p_str, NULL, 10);
    // Group ID
    p_str = getenv("oad_group_pid");
    if(p_str != NULL)
    {
        u32GroupId = simple_strtoul (p_str, NULL, 10);
    }
    // OAD_DVBC
    p_str = getenv("oad_upgrade_dvbc");
    if(p_str != NULL)
    {
        u8DVBC = simple_strtoul (p_str, NULL, 10);
    }
    // OAD DVBS
    p_str = getenv("oad_upgrade_dvbs");
    if(p_str != NULL)
    {
        u8DVBS = simple_strtoul (p_str, NULL, 10);
    }

    if(u8DVBC == 1)
    {
        p_str = getenv("oad_upgrade_qam");
        u32QAM = simple_strtoul (p_str, NULL, 10);
        if(p_str == NULL)
        {
            return -1;
        }
        p_str = getenv("oad_upgrade_sym");
        u16SYMBOLRATE = simple_strtoul (p_str, NULL, 10);
        if(p_str == NULL)
        {
            return -1;
        }
        snprintf(buffer, CMD_BUF, "oaddvbc %d %d %d 0x%x 0x%x", u32Freq, u32QAM, u16SYMBOLRATE, u32Pid, u32GroupId);
    }
    else if(u8DVBS == 1)
    {
        p_str = getenv("oad_upgrade_sym");
        u16SYMBOLRATE = simple_strtoul (p_str, NULL, 10);
        if(p_str == NULL)
        {
            return -1;
        }
        snprintf(buffer, CMD_BUF, "oaddvbs %d %d 0x%x 0x%x", u32Freq, u16SYMBOLRATE, u32Pid, u32GroupId);
    }
    else // DVBT
    {
        p_str = getenv("oad_upgrade_band");
        if(p_str == NULL)
        {
            return -1;
        }

        u8Band = simple_strtoul (p_str, NULL, 10);
        if((u8Band < 1) || (u8Band > 3))
        {
            UBOOT_ERROR("wrong band: %d \n",u8Band);
            return FALSE;
        }
        p_str = getenv("oad_is_sdtt");
        if(p_str != NULL)
        {
            u8SDTT_OAD = simple_strtoul (p_str, NULL, 10);
        }
        p_str = getenv("oad_upgrade_plp");
        if(p_str != NULL)
        {
            u16Plp = simple_strtoul (p_str, NULL, 10);
        }
        snprintf(buffer, CMD_BUF, "oad %d %d 0x%x 0x%x %d 0x%x", u32Freq, u8Band, u32Pid, u32GroupId, u8SDTT_OAD, u16Plp); //do OAD: download from ts to mamory
        printf("Get CMD_BUF %d %x %x\n",u32Freq,u32Pid,u32GroupId);
    }
     //do OAD: download from ts to mamory
    if(run_command(buffer,0)!=0)
    {
        UBOOT_ERROR("do %s fail\n",buffer);
        return -1;
    }
    UBOOT_TRACE("OK\n");

    return 0;
}
#endif

static int LoadScript2DramByFS(char *upgradeFile, char *downloadBuf)
{
    char buffer[CMD_BUF]="\0";
    UBOOT_TRACE("IN\n");
    memset(buffer, 0 , CMD_BUF);
    //load script
    if(0 != LoadScript2DramByFSFromVolume(upgradeFile, downloadBuf, OAD))
    {
        UBOOT_ERROR("Error: LoadScript2DramByFSFromVolume Fail !!\n");
        return -1;
    }
#if (CONFIG_SECURE_USB_UPGRADE==1)
    if(check_script_encrypt(downloadBuf) == TRUE)
    {
        setUpgradeMode(EN_UPDATE_MODE_NET_WITH_SEG_DECRYPTED);
        snprintf(buffer, CMD_BUF, "fileSegRSA FS NULL %s",upgradeFile);
        UBOOT_DEBUG("cmd=%s\n",buffer);
        if(0 != run_command(buffer, 0))
        {
            UBOOT_ERROR("Error: cmd: %s\n",buffer);
            return -1;
        }

        memset(buffer, 0 , CMD_BUF);
        snprintf(buffer, CMD_BUF, "filepartload %x %s %x %x",downloadBuf, upgradeFile, 0, getScritSizeOfUpgradeImage());
        UBOOT_DEBUG("cmd=%s\n",buffer);
        if(0 != run_command(buffer, 0))
        {
            UBOOT_ERROR("Error: cmd: %s\n",buffer);
            return -1;
        }

        // read dynamic script size
        if(read_dynamic_script_size(downloadBuf) == 0)
        {
            memset(buffer, 0 , CMD_BUF);
            snprintf(buffer, CMD_BUF, "filepartload %x %s %x %x",downloadBuf, upgradeFile, 0, getScritSizeOfUpgradeImage());
            UBOOT_DEBUG("cmd=%s\n",buffer);
            if(-1 == run_command(buffer, 0))
            {
                UBOOT_ERROR("Error: cmd: %s\n",buffer);
                free(buffer);
                return -1;
            }
        }
    }
    else
    {
        //Jump to console if #define of secure upgrade is enable, but image is not encrypted
        UBOOT_ERROR("Error: Image format is not for secure upgrade!!\n");
        jump_to_console();
        return -1;
    }
#else
    {
        // read dynamic script size
        if(read_dynamic_script_size(downloadBuf) == 0)
        {
            //load script again
            if(0 != LoadScript2DramByFSFromVolume(upgradeFile, downloadBuf, OAD))
            {
                UBOOT_ERROR("Error: LoadScript2DramByFSFromVolume Fail !!\n");
                return -1;
            }
        }

        //check script crc
        if(check_script_crc(upgradeFile,downloadBuf)==-1)
        {
            UBOOT_ERROR("Error: check_script_crc Fail !!\n");
            return -1;
        }
    }
#endif
    UBOOT_TRACE("OK\n");
    return 0;
}

static int LoadScript2DramByFSFromVolume(char *upgradeFile, char *downloadBuf, char *volume)
{
    UBOOT_TRACE("IN\n");

    if(upgradeFile==NULL || downloadBuf==NULL)
    {
        UBOOT_ERROR("Input NULL\n");
        return -1;
    }

    if(NULL == volume)
    {
        UBOOT_ERROR("volume is NULL\n");
        return -1;
    }
    if(vfs_mount(volume) == -1)
    {
        UBOOT_ERROR("Error: vfs_mount Fail !!\n");
        return -1;
    }
    setUpgradeFsMountName(volume);
    if(vfs_read((void *)downloadBuf,upgradeFile,0,getScritSizeOfUpgradeImage()) == -1)
    {
        UBOOT_ERROR("Error: vfs_read Fail !!\n");
        return -1;
    }

    UBOOT_TRACE("OK\n");
    return 0;
}

int do_nstar(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    char *script_buf  = NULL;
    char *_script_buf = NULL;
    char *next_line   = NULL;
    char *upgradeFile = NULL;
    unsigned int scriptFileSize=0;
    unsigned int progress_total_cnt = 0;
    unsigned int progress_cnt = 0;

    UBOOT_TRACE("IN\n");
    setUpgradeMode(EN_UPDATE_MODE_OAD); // set the Upgrade mode

    upgradeFile = IS_ARG_AVAILABLE_SCRIPT_FILE(argc) ? ARG_SCRIPT_FILE(argv) : DEFAULT_BIN_PATH;
    UBOOT_DEBUG("upgradeFile=%s\n",upgradeFile);
    if(setUpgradeFileName(upgradeFile)==-1)
    {
        UBOOT_ERROR("Error> set upgrade fail fail\n");
        _show_Error(0);
        setUpgradeMode(EN_UPDATE_MODE_NONE);
        return -1;
    }

    scriptFileSize=MAX_SCRIPT_SIZE;
    if(scriptFileSize==0)
    {
        UBOOT_ERROR("The size of script file is zero \n");
        return -1;
    }

    _script_buf=malloc(scriptFileSize+512);
    if(_script_buf==NULL)
    {
        UBOOT_ERROR("malloc fail \n");
        return -1;
    }
    script_buf=_script_buf;
    memset(script_buf,0,scriptFileSize);
    _show_LoadData(0);
    // load script
    if(LoadScript2DramByFS(upgradeFile,script_buf) != 0)
    {
        goto OAD_FAIL;
    }

    //set dont overwrite
    #if(ENABLE_MODULE_DONT_OVERWRITE==1)
    extern int SKIP_DONT_OVERWRITE;
    if(SKIP_DONT_OVERWRITE == 0)
    {
        dont_overwrite_enable();
        do_force_overwrite_enable();
    }
    #endif
    _show_StartUpgrading(0);

    //run script
    progress_total_cnt=_check_upgrade_script_cmd_count(script_buf);
    if(progress_total_cnt==0)
    {
       UBOOT_ERROR("There are no any commands in the upgrade script file\n");
       goto OAD_FAIL;
    }
    UBOOT_DEBUG("progress_total_cnt=%d\n",progress_total_cnt);

    // because application member is lazy, so we must set "sync_mmap 1" after burn code,
    run_command("setenv sync_mmap 1;saveenv", 0);
    while ((next_line = get_script_next_line(&script_buf)) != NULL)
    {
        UBOOT_INFO ("\n>> %s \n", next_line);

        progress_cnt++;
        UBOOT_DEBUG("progress_cnt=%d,progress_total_cnt=%d\n",progress_cnt,progress_total_cnt);
        _show_Upgrading(progress_cnt,progress_total_cnt);
        if(check_skip_cmd(next_line)== FALSE)
        {
#if ENABLE_DISPLAY_PICTURE_UPDATE
            check_filepartload_cmd(next_line, OAD);
#endif
            if(run_command(next_line, 0) < 0)
            {
                UBOOT_ERROR("Error> cmd: %s Fail !!\n", next_line);
                goto OAD_FAIL;
            }
        }

    }

    if(deleteUpgradeFileName()==-1)
    {
        UBOOT_ERROR("delete upgrade name fail\n");
    }

    #if(ENABLE_MODULE_DONT_OVERWRITE==1)
    extern int SKIP_DONT_OVERWRITE;
    SKIP_DONT_OVERWRITE = 0;
    #endif

    if(_script_buf!=NULL)
    {
        free(_script_buf);
    }

    _show_Finish(0);
    UBOOT_TRACE("OK\n");
    return 0;

OAD_FAIL:
   _show_Error(0);
    deleteUpgradeFileName();
    setUpgradeMode(EN_UPDATE_MODE_NONE);
    if(_script_buf!=NULL)
    {
        free(_script_buf);
    }
    return -1;
}

#define BRICK_BACKUP_VOLUME "brickbackup"
int do_bstar(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    char *script_buf  = NULL;
    char *_script_buf = NULL;
    char *next_line   = NULL;
    char *upgradeFile = NULL;
    unsigned int scriptFileSize=0;
    unsigned int progress_total_cnt = 0;
    unsigned int progress_cnt = 0;

    UBOOT_TRACE("IN\n");
    setUpgradeMode(EN_UPDATE_MODE_OAD); // set the Upgrade mode

    upgradeFile = IS_ARG_AVAILABLE_SCRIPT_FILE(argc) ? ARG_SCRIPT_FILE(argv) : DEFAULT_BIN_PATH;
    UBOOT_DEBUG("upgradeFile=%s\n",upgradeFile);
    if(setUpgradeFileName(upgradeFile)==-1)
    {
        UBOOT_ERROR("Error> set upgrade fail fail\n");
        _show_Error(0);
        setUpgradeMode(EN_UPDATE_MODE_NONE);
        return -1;
    }

    scriptFileSize=MAX_SCRIPT_SIZE;
    if(scriptFileSize==0)
    {
        UBOOT_ERROR("The size of script file is zero \n");
        return -1;
    }

    _script_buf=malloc(scriptFileSize+512);
    if(_script_buf==NULL)
    {
        UBOOT_ERROR("malloc fail \n");
        return -1;
    }
    script_buf=_script_buf;
    memset(script_buf,0,scriptFileSize);
    _show_LoadData(0);

    // load script
    if(LoadScript2DramByFSFromVolume(upgradeFile,script_buf, BRICK_BACKUP_VOLUME) != 0)
    {
        goto OAD_FAIL;
    }

    //set dont overwrite
#if(ENABLE_MODULE_DONT_OVERWRITE==1)
    extern int SKIP_DONT_OVERWRITE;
    if(SKIP_DONT_OVERWRITE == 0)
    {
        dont_overwrite_enable();
        do_force_overwrite_enable();
    }
#endif
    _show_StartUpgrading(0);

    //run script
    progress_total_cnt=_check_upgrade_script_cmd_count(script_buf);
    if(progress_total_cnt==0)
    {
       UBOOT_ERROR("There are no any commands in the upgrade script file\n");
       goto OAD_FAIL;
    }
    UBOOT_DEBUG("progress_total_cnt=%d\n",progress_total_cnt);

    while ((next_line = get_script_next_line(&script_buf)) != NULL)
    {
        UBOOT_INFO ("\n>> %s \n", next_line);

        progress_cnt++;
        UBOOT_DEBUG("progress_cnt=%d,progress_total_cnt=%d\n",progress_cnt,progress_total_cnt);
        _show_Upgrading(progress_cnt,progress_total_cnt);
        if(check_skip_cmd(next_line)== FALSE)
        {
#if ENABLE_DISPLAY_PICTURE_UPDATE
            check_filepartload_cmd(next_line, BRICK_BACKUP_VOLUME);
#endif
            if(run_command(next_line, 0) < 0)
            {
                UBOOT_ERROR("Error> cmd: %s Fail !!\n", next_line);
                goto OAD_FAIL;
            }
        }
    }

    if(deleteUpgradeFileName()==-1)
    {
        UBOOT_ERROR("delete upgrade name fail\n");
    }

    #if(ENABLE_MODULE_DONT_OVERWRITE==1)
    extern int SKIP_DONT_OVERWRITE;
    SKIP_DONT_OVERWRITE = 0;
    #endif

    if(_script_buf!=NULL)
    {
        free(_script_buf);
    }

    _show_Finish(0);
    UBOOT_TRACE("OK\n");
    return 0;

OAD_FAIL:
    _show_Error(0);
    deleteUpgradeFileName();
    setUpgradeMode(EN_UPDATE_MODE_NONE);
    if(_script_buf!=NULL)
    {
        free(_script_buf);
    }
    return -1;
}

int do_ostar(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    char *script_buf  = NULL;
    char *_script_buf = NULL;
    char *next_line   = NULL;
    char *upgradeFile = NULL;
    unsigned int scriptFileSize=0;
    unsigned int progress_total_cnt = 0;
    unsigned int progress_cnt = 0;

    UBOOT_TRACE("IN\n");
    setUpgradeMode(EN_UPDATE_MODE_OAD); // set the Upgrade mode

#if defined (CONFIG_OAD_IN_MBOOT)
    scriptFileSize = 0;
    _script_buf = NULL;
    upgradeFile = NULL;
    script_buf=(char *)UPGRADE_BUFFER_ADDR;
    if(LoadScript2DramByOAD()!= 0)
    {
         UBOOT_ERROR("OAD Download in Mboot Fail!!\n");
         return -1;
    }
#else
    upgradeFile = IS_ARG_AVAILABLE_SCRIPT_FILE(argc) ? ARG_SCRIPT_FILE(argv) : DEFAULT_BIN_PATH;
    UBOOT_DEBUG("upgradeFile=%s\n",upgradeFile);
    if(setUpgradeFileName(upgradeFile)==-1)
    {
        UBOOT_ERROR("Error> set upgrade fail fail\n");
        _show_Error(0);
        setUpgradeMode(EN_UPDATE_MODE_NONE);
        return -1;
    }

    scriptFileSize=MAX_SCRIPT_SIZE;
    if(scriptFileSize==0)
    {
        UBOOT_ERROR("The size of script file is zero \n");
        return -1;
    }

    _script_buf=malloc(scriptFileSize+512);
    if(_script_buf==NULL)
    {
        UBOOT_ERROR("malloc fail \n");
        return -1;
    }
    script_buf=_script_buf;
    memset(script_buf,0,scriptFileSize);
    _show_LoadData(0);
    // load script
    if(LoadScript2DramByFS(upgradeFile,script_buf) != 0)
    {
        goto OAD_FAIL;
    }
#endif

    //set dont overwrite
    #if(ENABLE_MODULE_DONT_OVERWRITE==1)
    extern int SKIP_DONT_OVERWRITE;
    if(SKIP_DONT_OVERWRITE == 0)
    {
        dont_overwrite_enable();
        do_force_overwrite_enable();
    }
    #endif
    _show_StartUpgrading(0);

    //run script
    progress_total_cnt=_check_upgrade_script_cmd_count(script_buf);
    if(progress_total_cnt==0)
    {
       UBOOT_ERROR("There are no any commands in the upgrade script file\n");
       goto OAD_FAIL;
    }
    UBOOT_DEBUG("progress_total_cnt=%d\n",progress_total_cnt);

    while ((next_line = get_script_next_line(&script_buf)) != NULL)
    {
        UBOOT_INFO ("\n>> %s \n", next_line);

        progress_cnt++;
        UBOOT_DEBUG("progress_cnt=%d,progress_total_cnt=%d\n",progress_cnt,progress_total_cnt);
        _show_Upgrading(progress_cnt,progress_total_cnt);
        if(check_skip_cmd(next_line)== FALSE)
        {
#if ENABLE_DISPLAY_PICTURE_UPDATE
            check_filepartload_cmd(next_line, OAD);
#endif
            if(run_command(next_line, 0) < 0)
            {
                UBOOT_ERROR("Error> cmd: %s Fail !!\n", next_line);
                goto OAD_FAIL;
            }
        }

    }

    if(deleteUpgradeFileName()==-1)
    {
        UBOOT_ERROR("delete upgrade name fail\n");
    }

    #if(ENABLE_MODULE_DONT_OVERWRITE==1)
    extern int SKIP_DONT_OVERWRITE;
    SKIP_DONT_OVERWRITE = 0;
    #endif

    if(_script_buf!=NULL)
    {
        free(_script_buf);
    }

    // because application member is lazy, so we must set "sync_mmap 1" after burn code,
    run_command("setenv sync_mmap 1;saveenv", 0);

    _show_Finish(0);
    UBOOT_TRACE("OK\n");
    return 0;

OAD_FAIL:
   _show_Error(0);
    deleteUpgradeFileName();
    setUpgradeMode(EN_UPDATE_MODE_NONE);
    if(_script_buf!=NULL)
    {
        free(_script_buf);
    }
    return -1;
}

#endif

#if (ENABLE_MODULE_ENV_UPGRADE_FROM_BANK == 1)
int do_rstar(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
//=========================================================
    #define ENV_RECOVERY "env_recovery"
    #define SCRIPT_SIZE 0x1000

    char *cEnvRecoveryOffset = getenv(ENV_RECOVERY);
    char *script_buf = NULL;
    char *_script_buf = NULL;
    char *next_line = NULL;

    unsigned int progress_total_cnt = 0;
    unsigned int progress_cnt = 0;
    unsigned int u32RecoveryOffset = 0;

    UBOOT_TRACE("IN\n");

    if (cEnvRecoveryOffset == NULL)
    {
        UBOOT_ERROR("Recovery offset not found\n");
       //_show_Error(0);
       return -1;
    }

    u32RecoveryOffset = (unsigned int)strtol(cEnvRecoveryOffset, NULL, 16);

    _script_buf=malloc(SCRIPT_SIZE);

    if(_script_buf==NULL){
        return -1;
    }

    script_buf = _script_buf;
    memset(script_buf ,0 ,SCRIPT_SIZE);
    raw_read((unsigned int)script_buf, u32RecoveryOffset, SCRIPT_SIZE);

    //Run Script

    progress_total_cnt=_check_upgrade_script_cmd_count(script_buf);
    if(progress_total_cnt==0)
    {
        setUpgradeMode(EN_UPDATE_MODE_NONE);
        free(_script_buf);
        UBOOT_ERROR("There are no any commands in the upgrade script file\n");
       //_show_Error(0);
       return -1;
    }
    UBOOT_DEBUG("progress_total_cnt=%d\n",progress_total_cnt);


    while ((next_line = get_script_next_line(&script_buf)) != NULL)
    {
        UBOOT_INFO("\n>> %s \n", next_line);

        progress_cnt++;
        UBOOT_DEBUG("progress_cnt=%d,progress_total_cnt=%d\n",progress_cnt,progress_total_cnt);
        //_show_Upgrading(progress_cnt,progress_total_cnt);
        if(check_skip_cmd(next_line)== FALSE)//filterCmd
        {
            if(run_command(next_line, 0) < 0)
            {
                free(_script_buf);
                UBOOT_ERROR("Error> cmd: %s Fail !!\n", next_line);
                //_show_Error(0);
                return -1;

            }
        }
    }
#if 0
    if(deleteUpgradeFileName()==-1)
    {
        UBOOT_ERROR("delete upgrade name fail\n");
    }
#endif
    free(_script_buf);
    UBOOT_TRACE("OK\n");
    return 0;

}
#endif

#if ((ENABLE_MODULE_OAD == 1) || (ENABLE_MODULE_USB == 1) || (ENABLE_MODULE_NETUPDATE == 1))
int do_mboot_upgrade(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    #define MBOOT_BUF_ADDR argv[1]
    #define MBOOT_SIZE argv[2]
    char ciscmd[CMD_BUF]="\0";
    UBOOT_TRACE("IN\n");
    unsigned int u32DramBuf = simple_strtoul(MBOOT_BUF_ADDR, NULL, 16);
    unsigned int u32MbootSize = simple_strtoul(MBOOT_SIZE, NULL, 16);

    UBOOT_DEBUG("u32DramBuf   : 0x%x\n",u32DramBuf);
    UBOOT_DEBUG("u32MbootSize : 0x%x\n",u32MbootSize);

    if(mboot_raw_io_Config()!= 0)
    {
        UBOOT_ERROR("raw_io_config setting fail!\n");
        return -1;
    }

    if(get_raw_status()==E_RAW_DATA_IN_SPI)
    {
        UBOOT_INFO("Upgrading Mboot.bin into SPI....\n");
        if(raw_write(u32DramBuf,0,u32MbootSize)!= 0)
        {
            UBOOT_ERROR("raw_write fail!\n");
            return -1;
        }
    }
    else if(get_raw_status()==E_RAW_DATA_IN_NAND)
    {
        UBOOT_INFO("Upgrading RomBoot.bin(MBOOT) into NAND....\n");

      #if (ENABLE_EAGLE) || (ENABLE_EMERALD) || (ENABLE_NIKON) || (ENABLE_KENYA)
    	sprintf(ciscmd,"ncisbl 0x%X 0x%X 0x%X 0x%X 0x%X", \
    	        u32DramBuf,u32DramBuf+0x10000,u32DramBuf+0x20000,u32DramBuf+0x30000,u32MbootSize);
      #elif (ENABLE_NUGGET)
    	sprintf(ciscmd,"ncishash 0x%X 0x%X 0x%X 0x%X 0x3800 0x%X 0x%X 0x3200", \
                u32DramBuf,u32DramBuf+0x10000,u32DramBuf+0x20000,u32DramBuf+0x30000,u32DramBuf+0x33800,u32MbootSize-0x3800);
      #elif (ENABLE_MILAN) || (ENABLE_MARLON) || (ENABLE_M5321)
    	sprintf(ciscmd,"ncishash 0x%X 0x%X 0x%X 0x%X 0x3800 0x%X 0x%X", \
                u32DramBuf,u32DramBuf+0x10000,u32DramBuf+0x20000,u32DramBuf+0x30000,u32DramBuf+0x33800,u32MbootSize-0x3800);
      #else //(ENABLE_EIFFEL) || (ENABLE_NIKE) || (ENABLE_MADISON) || (ENABLE_CLIPPERS) || (ENABLE_MIAMI) || (ENABLE_NADAL) || (ENABLE_EINSTEIN)
    	sprintf(ciscmd,"ncishash 0x%X 0x%X 0x%X 0x%X 0x2800 0x%X 0x%X", \
    	        u32DramBuf,u32DramBuf+0x10000,u32DramBuf+0x20000,u32DramBuf+0x30000,u32DramBuf+0x32800,u32MbootSize-0x2800);
      #endif
        if(run_command(ciscmd,0)!=0)
        {
            UBOOT_ERROR("CMD: ncisbl/ncishash  fail!\n");
            return -1;
        }
        setenv("CISRefresh",NULL);
        saveenv();
    }
    else if(get_raw_status()==E_RAW_DATA_IN_MMC)
    {
        UBOOT_INFO("Upgrading Mboot.bin into EMMC....\n");
        //todo
    }
    else
    {
        UBOOT_ERROR("wrong raw_io_setting !!\n");
        return -1;
    }

    UBOOT_TRACE("OK\n");
    return 0;
}

void RegisterCBShowError(show_Error_cb cb)
{
    UBOOT_TRACE("IN\n");
    fpShow_Error=cb;
    UBOOT_TRACE("OK\n");
}

void RegisterCBShowFinish(show_Finish_cb cb)
{
    UBOOT_TRACE("IN\n");
    fpShow_Finish=cb;
    UBOOT_TRACE("OK\n");
}

void RegisterCBShowLoadData(show_LoadData_cb cb)
{
    UBOOT_TRACE("IN\n");
    fpShow_LoadData=cb;
    UBOOT_TRACE("OK\n");
}

void RegisterCBShowStartUpgrading(show_StartUpgrading_cb cb)
{
    UBOOT_TRACE("IN\n");
    fpShow_StartUpgrading=cb;
    UBOOT_TRACE("OK\n");
}
void RegisterCBShowUpgrading(show_Upgrading_cb cb)
{
    UBOOT_TRACE("IN\n");
    fpShow_upgrading=cb;
    UBOOT_TRACE("OK\n");
}

void UnRegisterShowCb(void)
{
    UBOOT_TRACE("IN\n");
    fpShow_Error=NULL;
    fpShow_Finish=NULL;
    fpShow_LoadData=NULL;
    fpShow_StartUpgrading=NULL;
    fpShow_upgrading=NULL;
    UBOOT_TRACE("OK\n");
}


static int _show_Error(unsigned int var)
{
#if (ENABLE_MODULE_DISPLAY_OSD==1)
    UBOOT_TRACE("IN\n");
    if(fpShow_Error!=NULL)
    {
        UBOOT_TRACE("OK\n");
        return fpShow_Error(var);
    }
    else
    {
        UBOOT_DEBUG("No call back function for 'error' phase\n");
        UBOOT_TRACE("OK\n");
        return 0;
    }
#else
    return 0;
#endif
}

static int _show_Finish(unsigned int var)
{
#if (ENABLE_MODULE_DISPLAY_OSD==1)
    UBOOT_TRACE("IN\n");
    if(fpShow_Finish!=NULL)
    {
        UBOOT_TRACE("OK\n");
        return fpShow_Finish(var);
    }
    else
    {
        UBOOT_DEBUG("No call back function for 'finish' phase\n");
        UBOOT_TRACE("OK\n");
        return 0;
    }
#else
    return 0;
#endif
}

static int _show_LoadData(unsigned int var)
{
#if (ENABLE_MODULE_DISPLAY_OSD==1)
    UBOOT_TRACE("IN\n");
    if(fpShow_LoadData!=NULL)
    {
        UBOOT_TRACE("OK\n");
        return fpShow_LoadData(var);
    }
    else
    {
        UBOOT_DEBUG("No call back function for 'LoadData' phase\n");
        UBOOT_TRACE("OK\n");
        return 0;
    }
#else
    return 0;
#endif
}
static int _show_StartUpgrading(unsigned int var)
{
#if (ENABLE_MODULE_DISPLAY_OSD==1)
    UBOOT_TRACE("IN\n");
    if(fpShow_StartUpgrading!=NULL)
    {
        UBOOT_TRACE("OK\n");
        return fpShow_StartUpgrading(var);
    }
    else
    {
        UBOOT_DEBUG("No call back function for 'StartUpgrading' phase\n");
        UBOOT_TRACE("OK\n");
        return 0;
    }
#else
    return 0;
#endif
}

static int _show_Upgrading(unsigned int var1, unsigned int var2)
{
    int ret = 0;

#if (ENABLE_MODULE_DISPLAY_OSD==1)
    UBOOT_TRACE("IN\n");
    if(fpShow_upgrading!=NULL)
    {
        char filesize[CMD_BUF]="\0";
        char *env = NULL;
        memset(filesize, 0 , CMD_BUF);
        env = getenv("filesize");
        if(env)
        {
            snprintf(filesize, CMD_BUF, "%s", env);
        }
        ret = fpShow_upgrading(var1, var2);
        setenv("filesize", filesize);
        UBOOT_TRACE("OK\n");
    }
    else
    {
        UBOOT_DEBUG("No call back function for 'Upgrading' phase\n");
        UBOOT_TRACE("OK\n");
    }
#endif
    return ret;
}

static unsigned int _check_upgrade_script_cmd_count(char *buffer)
{
    unsigned int scriptFileSize=0;
    unsigned int amount=0;
    char *tempBuf=NULL;

    UBOOT_TRACE("IN\n");
    scriptFileSize=getScritSizeOfUpgradeImage();
    if(scriptFileSize==0)
    {
        UBOOT_ERROR("The size of script file is zero \n");
        return 0;
    }

    tempBuf=malloc(scriptFileSize);
    if(tempBuf==NULL)
    {
        UBOOT_ERROR("malloc fail \n");
        return 0;
    }
    memcpy(tempBuf,buffer,scriptFileSize);
    amount=check_script_cmd_count(tempBuf);
    free(tempBuf);

    UBOOT_TRACE("OK\n");
    return amount;
}

#endif

#ifdef CONFIG_BOOT_SEQUENCE_FORCE_USB_UPGRADE_LOADER

#ifdef CONFIG_USB_DISK_UPGRADE_LOADER
void USB_LED_Setting(void)
{
#if defined(USB_DISK_UPGRADE_LOADER_LED_GPIO)
    printf("\nLED SETTING\n");
    MDrv_PWM_Init(0);
    MDrv_PM_PWM_Enable_Ex(0,0);
    mdrv_gpio_init();
#endif
}

void USB_LED_ON(void)
{
#if defined(USB_DISK_UPGRADE_LOADER_LED_GPIO)
    mdrv_gpio_set_high(USB_DISK_UPGRADE_LOADER_LED_GPIO);
    led_status = LED_ON;
#endif
}

void USB_LED_OFF(void)
{
#if defined(USB_DISK_UPGRADE_LOADER_LED_GPIO)
    mdrv_gpio_set_low(USB_DISK_UPGRADE_LOADER_LED_GPIO);
    led_status = LED_OFF;
#endif
}

void USB_LED_Toggle(void)
{
    if(led_status == LED_ON)
    {
       USB_LED_OFF();
    }
    else
    {
       USB_LED_ON();
    }
}

void USB_Process_LED_ON(int led_toggle_time)
{
#if defined(USB_DISK_UPGRADE_LOADER_LED_GPIO)
    g_check_timer_id=0;
    g_check_timer_id = MsOS_CreateTimer( (TimerCb)USB_LED_Toggle,
                                        0,
                                        led_toggle_time,
                                        TRUE,
                                        "USB LOADER LED");
#endif
}

void USB_Process_LED_OFF(void)
{
#if defined(USB_DISK_UPGRADE_LOADER_LED_GPIO)
     MsOS_StopTimer(g_check_timer_id);
     MsOS_DeleteTimer(g_check_timer_id);
     mdrv_gpio_set_low(USB_DISK_UPGRADE_LOADER_LED_GPIO);
     printf("Stop timer !!\n");
#endif
}
#endif // #ifdef CONFIG_USB_DISK_UPGRADE_LOADER

#ifdef CONFIG_USB_PC_UPGRADE_LOADER
void usb_push_log(char *data, int len)
{
    strncat(g_usb_log.logbuffer, data, (len%USB_LOG_BUFFER_SIZE));
}

int usb_get_log(char *data, int len)
{
    int buf_len = strlen(g_usb_log.logbuffer);
    int get_len = (buf_len-len) > 0 ? len : buf_len;
    strncpy(data, g_usb_log.logbuffer, get_len);
    return get_len;
}

int USB_GET_LOG(U32 u32ReqLen)
{
    int ret = 0;
    ret=usb_get_log(usb_log_buf, u32ReqLen);
    memset(g_usb_log.logbuffer, 0, USB_LOG_BUFFER_SIZE);
    return ret;
}

void USB_Upgrade_Active_USB(void)
{
    UBOOT_TRACE("IN\n");
    USB_DRC_Interrupt(g_usb_var);
    MDrv_USB_CmdLoop((void*)(g_usb_var));
    UBOOT_TRACE("OK\n");
}

void USB_Upgrade_Write_Ack(void)
{
    char tempBuf[200] = {0};
    memset(tempBuf, 0, 200);
    UBOOT_TRACE("IN\n");
    usb_ack[0] = 0xd0;
    usb_ack[1] = 0xd0;
    usb_ack[2] = 0xd0;
    usb_ack[3] = 0xd0;

    MDrv_USB_Write(usb_ack, 4, g_usb_var);
    while(1)
    {
        USB_Upgrade_Active_USB();
        if(g_scsiVendorCmdVar.ptrPPB2Buf[0] == 0)
           break;
    }
    USB_LOG_INFO(tempBuf, "USB ack OK\n\n");
}

void USB_Upgrade_Write_NACK(void)
{
    char tempBuf[200] = {0};
    memset(tempBuf, 0, 200);
    UBOOT_TRACE("IN\n");
    usb_ack[0] = 0xee;
    usb_ack[1] = 0xee;
    usb_ack[2] = 0xee;
    usb_ack[3] = 0xee;

    MDrv_USB_Write(usb_ack, 4, g_usb_var);
    while(1)
    {
        USB_Upgrade_Active_USB();
        if(g_scsiVendorCmdVar.ptrPPB2Buf[0] == 0)
           break;
    }
    USB_LOG_INFO(tempBuf, "USB nack OK\n\n");
}

U32 USB_Upgrade_Receive_Packet(void)
{
    U32 PacketLen = 0;
    char tempBuf[200] = {0};
    memset(tempBuf, 0, 200);

    USB_LOG_DEBUG(tempBuf, "USB Receive Packet\n");
    memset(&Header, 0, sizeof(USB_Upgrade_Header));
    while(1)
    {
        USB_Upgrade_Active_USB();
        PacketLen += MDrv_USB_Read((U8*)(usb_load_addr), MAX_DATA_BUFFER, g_usb_var);

        if(PacketLen >= 16)
            break;
    }
    USB_LOG_DEBUG(tempBuf, "first packet len    = 0x%x\n", PacketLen);
    Header.HeaderSize = usb_load_addr[LOADER_HEADER_SIZE_OFFSET];
    if(Header.HeaderSize != USB_UPGRADE_HEADER_SIZE)
    {
        USB_LOG_DEBUG(tempBuf, "Unsupport Header Type : 0x%x\n", Header.HeaderSize);
        return -1;
    }
    Header.Version = usb_load_addr[LOADER_HEADER_VERSION_OFFSET];
    Header.Command = (U32)(usb_load_addr[LOADER_HEADER_CMD_OFFSET])
               + (U32)(usb_load_addr[LOADER_HEADER_CMD_OFFSET+1] <<  8);
    Header.DataSize = (U32)(usb_load_addr[LOADER_HEADER_DATA_SIZE_OFFSET])
               + (U32)(usb_load_addr[LOADER_HEADER_DATA_SIZE_OFFSET+1] <<  8)
               + (U32)(usb_load_addr[LOADER_HEADER_DATA_SIZE_OFFSET+2] << 16)
               + (U32)(usb_load_addr[LOADER_HEADER_DATA_SIZE_OFFSET+3] << 24);
    Header.Address = (U32)(usb_load_addr[LOADER_HEADER_ADDR_OFFSET])
               + (U32)(usb_load_addr[LOADER_HEADER_ADDR_OFFSET+1] <<  8)
               + (U32)(usb_load_addr[LOADER_HEADER_ADDR_OFFSET+2] << 16)
               + (U32)(usb_load_addr[LOADER_HEADER_ADDR_OFFSET+3] << 24);

    USB_LOG_DEBUG(tempBuf, "Header.HeaderSize   = 0x%x\n", Header.HeaderSize);
    USB_LOG_DEBUG(tempBuf, "Header.Version      = 0x%x\n", Header.Version);
    USB_LOG_DEBUG(tempBuf, "Header.Command      = 0x%x\n", Header.Command);
    USB_LOG_DEBUG(tempBuf, "Header.DataSize     = 0x%x\n", Header.DataSize);
    USB_LOG_DEBUG(tempBuf, "Header.Address      = 0x%x\n", Header.Address);

    if(Header.DataSize + Header.HeaderSize > PacketLen)
    {
        while(1)
        {
            USB_DRC_Interrupt(g_usb_var);
            MDrv_USB_CmdLoop((void*)(g_usb_var));
            PacketLen += MDrv_USB_Read((MS_U8*)(usb_load_addr + PacketLen), MAX_DATA_BUFFER, g_usb_var);

            if(PacketLen)
            {
                if(PacketLen >= Header.DataSize + Header.HeaderSize)
                    break;
            }
        }
    }
    USB_LOG_DEBUG(tempBuf, "final packet len    = 0x%x\n", PacketLen);
    UBOOT_TRACE("OK\n");
    return 0;
}

void USB_Upgrade_Process(void)
{
    char buf[CMD_BUF];
    int ret = 0;
    char tempBuf[200] = {0};
    memset(tempBuf, 0, 200);
    UBOOT_TRACE("IN\n");
    switch (Header.Command)
    {
        case COMMAND_TYPE_SCRIPT:
            // ignore the command start from "#" and "%"
            if(strncmp((const char *)&usb_load_addr[Header.HeaderSize], "#", 1)==0 ||
                strncmp((const char *)&usb_load_addr[Header.HeaderSize], "%", 1)==0)
            {
                USB_Upgrade_Active_USB();
                usb_load_addr[Header.HeaderSize + Header.DataSize] = 0x00;
                USB_LOG_INFO(tempBuf, ">> Ignore Run command : %s\n", &usb_load_addr[Header.HeaderSize]);
                break;
            }
            if(strncmp((const char *)&usb_load_addr[Header.HeaderSize], "reset", 5)==0)
            {
                USB_Upgrade_Write_Ack();
            }
            usb_load_addr[Header.HeaderSize + Header.DataSize] = 0x00;
            USB_LOG_INFO(tempBuf, ">> Run command : %s\n", &usb_load_addr[Header.HeaderSize]);
            USB_Upgrade_Active_USB();
            ret = run_command((const char *)&usb_load_addr[Header.HeaderSize], 0);
            if( ret == -1 )
            {
                USB_Upgrade_Write_NACK();
            }
            USB_Upgrade_Active_USB();
            break;
        case COMMAND_TYPE_COPYDATA:
            memset(buf, 0, CMD_BUF);
            sprintf(buf, "%x", Header.DataSize);
            setenv("filesize", buf);
            memcpy ((void *)Header.Address, &usb_load_addr[Header.HeaderSize], Header.DataSize);
            USB_LOG_INFO(tempBuf, ">> Copy Data To Dram, addr=0x%x, filesize=0x%s\n", Header.Address, getenv("filesize"));
            break;
        case COMMAND_TYPE_NOP:
            USB_LOG_INFO(tempBuf, ">> NOP : Do nothing\n");
            break;
        case COMMAND_TYPE_RESET:
            USB_LOG_INFO(tempBuf, ">> Run command : reset\n");
            USB_Upgrade_Write_Ack();
            run_command("reset", 0);
            break;
        default:
            USB_LOG_INFO(tempBuf, ">> Unknow Command Type: 0x%x\n", Header.Command);
            USB_Upgrade_Write_NACK();
            break;
    }
    USB_LOG_INFO(tempBuf, "USB Upgrade Process OK\n");
}
#endif

void USB_Upgrade_Init(void)
{
    UBOOT_TRACE("IN\n");

// LED setting is for TV chip
#if (CONFIG_TV_CHIP == 1)
#if defined(CONFIG_USB_DISK_UPGRADE_LOADER)
    USB_LED_Setting();
#endif
#endif

#ifdef CONFIG_USB_PC_UPGRADE_LOADER
#if (CONFIG_TV_CHIP == 1)
    // copy otg structure
    memcpy((void *)LOADER_USB_MEM_ADDR, (const void *)CONFIG_USB_LOADER_OTG_MEM_ADDR, CONFIG_USB_LOADER_OTG_MEM_SIZE);
#endif

    #if defined(ENABLE_PC_UPGRADE_EMI_SWITCH)
    USB_EMI_Switch();
    #endif

    otgInitMassBuffer(g_usb_var);

    g_scsiVendorCmdVar.ucptrDataBuf = (U8 *)(usb_data_buf);
    g_scsiVendorCmdVar.ptrPPB2Buf= (U8 *)(usb_ack_buf);
    g_scsiVendorCmdVar.ptrTmpHifBuf= (U8 *)(usb_log_buf);
    g_usb_var->ptrVCmdStruct = &g_scsiVendorCmdVar;

    memset(g_scsiVendorCmdVar.ptrPPB2Buf, 0x0, 0x4);
    memset(&g_usb_log, 0, sizeof(USB_LOG_BUFFER_SIZE));
    memset(g_usb_log.logbuffer, 0, USB_LOG_BUFFER_SIZE);
    g_usb_log.start = 0;
    g_usb_log.end = 0;
    g_usb_log.count = 0;
    USB_Upgrade_Write_Ack();
#endif // #ifdef CONFIG_USB_PC_UPGRADE_LOADER

    UBOOT_TRACE("OK\n");
}

void USB_Upgrade_Func(void)
{
    char tempBuf[200] = {0};
    memset(tempBuf, 0, 200);
    USB_Upgrade_Init();
#ifdef CONFIG_USB_PC_UPGRADE_LOADER
    USB_LOG_INFO(tempBuf, "\n");
    USB_LOG_INFO(tempBuf, "===========================================\n");
    USB_LOG_INFO(tempBuf, "========USB PC Upgrade mode start==========\n");
    USB_LOG_INFO(tempBuf, "===========================================\n");
    USB_LOG_INFO(tempBuf, "\n");

    while(1)
    {
        if(-1 != USB_Upgrade_Receive_Packet())
        {
            USB_Upgrade_Process();
        }
        USB_Upgrade_Write_Ack();
    }
#else // CONFIG_USB_DISK_UPGRADE_LOADER
    UBOOT_INFO("\n");
    UBOOT_INFO("===========================================\n");
    UBOOT_INFO("========USB DISK Upgrade mode start========\n");
    UBOOT_INFO("===========================================\n");
    UBOOT_INFO("\n");
    #define LED_NORMAL_TOGGLE_TIME 500
    #define LED_ERROR_TOGGLE_TIME 100
    led_status = LED_OFF;
    USB_Process_LED_ON(LED_NORMAL_TOGGLE_TIME);
    int ret = 0;
    ret = run_command("usb start 0", 0);
    if(ret != 0)
    {
        UBOOT_ERROR("usb start fail\n");
        USB_Process_LED_OFF();
        USB_Process_LED_ON(LED_ERROR_TOGGLE_TIME);
        hang();
    }

    ret = run_command("ustar", 0);
    if(ret != 0)
    {
        UBOOT_ERROR("ustar fail\n");
        USB_Process_LED_OFF();
        USB_Process_LED_ON(LED_ERROR_TOGGLE_TIME);
        hang();
    }
    else
    {
        USB_Process_LED_OFF();
        USB_LED_ON();
    }
    UBOOT_INFO("Upgrade Finish, skip the error message below!!!\n\n");
    hang();
#endif
}
#endif

#if defined(CONFIG_MSTAR_NS_UBOOT)
#define TEE_KEY_BANK_OFFSET       0x2000
#define REE_KEY_BANK_OFFSET       0x3000
#define KEYLADDER_KEY_BANK_OFFSET 0x4000
#define KEE_TYPE_OFFSET           0x6000

#define KEY_BANK_ROM_SIZE         0x1000

int do_keybank_upgrade(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    char *KeyBankName = argv[1];
    U32 u32Addr = simple_strtoul(argv[2], NULL, 16);
    U32 u32KeyOffset = 0;

    UBOOT_TRACE("IN\n");
    if(argc!=3)
    {
        cmd_usage(cmdtp);
        return -1;
    }

    if(MsApiChunkHeader_GetValue(CH_CUSTOMER_KEY_BANK_ROM_OFFSET,&u32KeyOffset)== -1)
    {
        UBOOT_ERROR("Secure_Drv_Init fail\n");
        return -1;
    }
    if(0==strncmp(KeyBankName, "TEE", 2))
    {
        u32KeyOffset = u32KeyOffset + TEE_KEY_BANK_OFFSET;
    }
    else if(0==strncmp(KeyBankName, "REE", 2))
    {
        u32KeyOffset = u32KeyOffset + REE_KEY_BANK_OFFSET;
    }
    else if(0==strncmp(KeyBankName, "KL", 2))
    {
        u32KeyOffset = u32KeyOffset + KEYLADDER_KEY_BANK_OFFSET;
    }
    else if(0==strncmp(KeyBankName, "KEYTYPE", 2))
    {
        u32KeyOffset = u32KeyOffset + KEE_TYPE_OFFSET;
    }
    else
    {
        UBOOT_DEBUG("Unknow key bank name\n");
        return -1;
    }

    UBOOT_DEBUG("KeyBankName:%s\n", KeyBankName);
    UBOOT_DEBUG("u32Addr:0x%x\n", u32Addr);
    UBOOT_DEBUG("u32KeyOffset:0x%x\n", u32KeyOffset);

    if(raw_write(u32Addr, u32KeyOffset, KEY_BANK_ROM_SIZE)!= 0)
    {
        UBOOT_ERROR("Write KeyBank Fail\n");
        return -1;
    }
    UBOOT_TRACE("OK\n");
    return 0;
}
#endif


#if defined (CONFIG_URSA_6M60)
int terra2_fast_upgrade_program(void) {
    MS_U8 retry_time = 0;

    // program from SoC dram -> ursa dram -> dram flash with maximum 3 times retry in case data corruption during transfer via I2C
    do
    {
        _show_Upgrading(1, 6); //show upgrade progress  (current_cnt, total_cnt)
        if (FALSE == set_sw_upgrade_init())
        {
            UBOOT_INFO("set_sw_upgrade_init fail, retry_time = %d\n", retry_time);
            if (retry_time == 2)
                return FastUpgradeStatusInitFail;
            else
                continue;
        }

        _show_Upgrading(2, 6);

        if (FALSE == terra2_uart_burst_write())
        {
            UBOOT_INFO("terra2_uart_burst_write fail, retry_time = %d\n", retry_time);
            if (retry_time == 2)
                return FastUpgradeStatusBurstWriteFail;
            else
                continue;
        }
        _show_Upgrading(3, 6);

        // check dram crc32
        if (FALSE == get_sw_upgrade_crc32(CRC32_TYPE_DRAM))
        {
            UBOOT_INFO("get_sw_upgrade_crc32 dram fail, retry_time = %d\n", retry_time);
            if (retry_time == 2)
                return FastUpgradeStatusChkDRAMFail;
            else
                continue;
        }
        _show_Upgrading(4, 6);

        // invoke write data from register to dram
        UBOOT_INFO("Terra2 begin write flash!! [%lu]\n", MsSystemGetBootTime());
        if (FALSE == set_sw_upgrade_write_flash())
        {
            UBOOT_INFO("set_sw_upgrade_write_flash fail, retry_time = %d\n", retry_time);
            if (retry_time == 2)
                return FastUpgradeStatusWriteFlashFail;
            else
                continue;
        }
        UBOOT_INFO("Terra2 write flash OK!! [%lu]\n ", MsSystemGetBootTime());
        _show_Upgrading(5, 6);

        // check flash crc32
        if (FALSE == get_sw_upgrade_crc32(CRC32_TYPE_FLASH))
        {
            UBOOT_INFO("get_sw_upgrade_crc32 flash fail, retry_time = %d\n", retry_time);

            if (retry_time == 2)
                return FastUpgradeStatusChkFLASHFail;
            else
                continue;
        }
        else
        {
            break;
        }
    } while (retry_time++ < 3);
    _show_Upgrading(6, 6);
    UBOOT_INFO("Terra2 fast upgrade success! [%lu]\n", MsSystemGetBootTime());

    return FastUpgradeStatusOk;
}


int do_terra2_fast_upgrade(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    MS_U32 u32Addr = 0;
    MS_U32 u32FileSize = 0;

    if(3 == argc) //tftp case: get terra2.bin from DRAM
    {
        u32Addr = simple_strtoul(argv[1], NULL, 16);
        u32FileSize = simple_strtoul(argv[2], NULL, 16);
    }
    else          //usb case: get terra2.bin from UsbDisk
    {
        _show_LoadData(0);

        if(0 != getUpgradeBinFromUsb(&u32Addr, &u32FileSize))
        {
           UBOOT_ERROR("Get Upgrade File from USB fail!\n");
           _show_Error(0);
           return -1;
        }
    }

    if(0 != checkTerra2Bin(u32Addr, u32FileSize))
    {
       UBOOT_ERROR("Check terra2 file structure fail!\n");
         _show_Error(0);
       return -1;
    }

    Terra2_Upgrade_Init(u32Addr, u32FileSize);
    if (TRUE != is_support_get_spi_header_by_ddc())
    {
        UBOOT_ERROR("Terra2 don't support ddc commands!\n");
          _show_Error(0);
        return -1;
    }
    if (TRUE != get_sw_upgrade_spi_header())
    {
        UBOOT_ERROR("Get SPI Header failed!\n");
          _show_Error(0);
        return -1;
    }

    UBOOT_INFO("Start Upgrade Fast Terra2! [%lu]\n", MsSystemGetBootTime());

    _show_StartUpgrading(0);
    int status = terra2_fast_upgrade_program();
    if (status != 0)
    {
        UBOOT_ERROR("Terra2 Fast Upgrading fail, status(%d)!!!\n", status);
          _show_Error(0);
        return -1;
    }

    _show_Finish(0);

    return 0;
}
#endif


#ifdef CONFIG_AN_FASTBOOT_ENABLE
int do_sparse_write(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    if(argc < 5)
        return cmd_usage(cmdtp);
    // argv[1]: flash type
    // argv[2]: DRAM Address
    // argv[3]: partition name
    // argv[4]: image size
    char *flash_type = argv[1];
    void *addr = (void *)simple_strtoul(argv[2], NULL, 16);
    char *partition_name = argv[3];
    unsigned int sparse_image_size = simple_strtoul(argv[4], NULL, 16);

    sparse_header_t * sparse_header = (sparse_header_t*)addr;
    if (sparse_header->magic == SPARSE_HEADER_MAGIC)
    {
        printf("Starting Writing Sparse Image...\n");
        if ( strcmp(flash_type, "mmc") == 0 )
            fb_mmc_flash_write(partition_name, addr, sparse_image_size);
        else if ( strcmp(flash_type, "nand") == 0 )
            ; // not implement yet
        else
        {
            UBOOT_ERROR("unknow flash type\n");
            return -1;
        }
    }
    else
    {
        UBOOT_ERROR("Image is not a sparse format image!!!\n");
        return -1;
    }

    return 0;
}
#endif
#if (ENABLE_MODULE_NET == 1)
int Set_tftp_addr_info(unsigned int tftp_start_address, unsigned int tftp_end_address)
{
    UBOOT_TRACE("IN\n");
    // first time to set the tftp address
    if( g_tftp_set_flag == 0 )
    {
        g_tftp_start_address = tftp_start_address;
        g_tftp_end_address = tftp_end_address;
        UBOOT_DEBUG("g_tftp_start_address=%x\n", g_tftp_start_address);
        UBOOT_DEBUG("g_tftp_end_address=%x\n", g_tftp_end_address);
        g_tftp_set_flag = 1;
    }
    else
    {
        UBOOT_DEBUG("tftp_address is already set!\n");
        UBOOT_DEBUG("g_tftp_start_address=%x\n", g_tftp_start_address);
        UBOOT_DEBUG("g_tftp_end_address=%x\n", g_tftp_end_address);
    }
    UBOOT_TRACE("OK\n");
    return 0;
}

int do_auto_tftp(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    UBOOT_TRACE("IN\n");
    int ret = 0;
    char *cmd = NULL;
    if(argc < 2)
        return cmd_usage(cmdtp);
    // argv[1]: image name

    char *image_name = argv[1];

    // Just for initialize, these values may be change after calculate
    unsigned int tftp_start_address = 0x20200000;
    unsigned int tftp_end_address = 0x20200000;
    unsigned int lzo_start_address = 0;
    unsigned int lzo_len = 0;

    // We define the max tftp buffer size is 200MB, LZO buffer size is 50MB
    // Need to reserve total 250MB for tftp + lzo
    const unsigned int MIU_OFFSET = 0x20000000;
    unsigned int mboot_address = (CONFIG_DRAM_SIZE_MB_RELOCATE*1024*1024) + MIU_OFFSET;

    // check if the mboot address < 256MB, ex. 137MB
    // use 256MB~512MB
    if( mboot_address < (MAX_TFTP_SIZE + MAX_LZO_SIZE + MIU_OFFSET) )
    {
        tftp_start_address = 0x10000000+MIU_OFFSET;
        tftp_end_address = tftp_start_address + MAX_TFTP_SIZE - 1;
    }
    else
    {
        // if the mboot address > 256MB, ex. 305MB
        // use 2MB~256MB
        tftp_start_address = 0x200000+MIU_OFFSET;
        tftp_end_address = tftp_start_address + MAX_TFTP_SIZE - 1;
    }

    Set_tftp_addr_info(tftp_start_address, tftp_end_address);

    UBOOT_DEBUG("tftp_start_address=%x\n", tftp_start_address);
    UBOOT_DEBUG("tftp_end_address=%x\n", tftp_end_address);

    // first step, use tftp load image to DRAM
    cmd = (char *)malloc(CMD_BUF);
    snprintf(cmd, CMD_BUF, "tftp 0x%x %s", g_tftp_start_address, image_name);
    UBOOT_DEBUG("cmd:%s\n",cmd);
    ret=run_command(cmd, 0);
    if(ret<0)
    {
        free(cmd);
        return -1;
    }

    free(cmd);
    UBOOT_TRACE("OK\n");
    return ret;
}

int do_auto_flash(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    UBOOT_TRACE("IN\n");
    int ret = 0;
    char *cmd = NULL;
    if(argc < 4)
        return cmd_usage(cmdtp);
    // argv[1]: flash type
    // argv[2]: flash command
    // argv[3]: partition name
    // argv[4]: empty_skip

    char *flash_type = argv[1];
    char *flash_command = argv[2];
    char *partition_name = argv[3];
    char *empty_skip = NULL;

    if(argc == 5)
        empty_skip = argv[4];

    // check tftp address is available or not
    // if the address is not available, stop flash image
    if( g_tftp_set_flag == 0 )
    {
        UBOOT_ERROR("TFTP address is not available!!!\n");
        return -1;
    }

#ifdef CONFIG_LZO
    unsigned int lzo_start_address = 0;
    unsigned int lzo_len = 0;
    lzo_start_address = g_tftp_end_address + 1;
    lzo_len = MAX_LZO_SIZE;
    Set_lzo_addr_Info(lzo_start_address, lzo_len);

    UBOOT_DEBUG("lzo_start_address=%x\n", Get_lzo_addr_Info());
    UBOOT_DEBUG("lzo_len=%x\n", g_lzo_len);
#endif

    cmd = (char *)malloc(CMD_BUF);

    if ( strcmp(flash_type, "mmc") == 0 )
    {
        //printf("test1\n");
        if ( strcmp(flash_command, "write.boot") == 0 )
        {
            //printf("test2\n");
            snprintf(cmd, CMD_BUF, "mmc write.boot 1 0x%x 0 $(filesize)", g_tftp_start_address);
            UBOOT_DEBUG("cmd:%s\n",cmd);
            ret=run_command(cmd, 0);
            if(ret<0)
            {
                free(cmd);
                return -1;
            }
        }
        else if ( strcmp(flash_command, "write.p") == 0 )
        {
            if( empty_skip == NULL )
                snprintf(cmd, CMD_BUF, "mmc write.p 0x%x %s $(filesize)", g_tftp_start_address, partition_name);
            else
                snprintf(cmd, CMD_BUF, "mmc write.p 0x%x %s $(filesize) %s", g_tftp_start_address, partition_name, empty_skip);

            UBOOT_DEBUG("cmd:%s\n",cmd);
            ret=run_command(cmd, 0);
            if(ret<0)
            {
                free(cmd);
                return -1;
            }
        }
#ifdef CONFIG_LZO
        else if ( strcmp(flash_command, "unlzo") == 0 )
        {
            snprintf(cmd, CMD_BUF, "mmc unlzo 0x%x $(filesize) %s 1", g_tftp_start_address, partition_name);
            UBOOT_DEBUG("cmd:%s\n",cmd);
            ret=run_command(cmd, 0);
            if(ret<0)
            {
                free(cmd);
                return -1;
            }
        }
        else if ( strcmp(flash_command, "unlzo.cont") == 0 )
        {
            snprintf(cmd, CMD_BUF, "mmc unlzo.cont 0x%x $(filesize) %s 1", g_tftp_start_address, partition_name);
            UBOOT_DEBUG("cmd:%s\n",cmd);
            ret=run_command(cmd, 0);
            if(ret<0)
            {
                free(cmd);
                return -1;
            }
        }
#endif
        else
        {
            UBOOT_ERROR("Unavailable mmc flash command\n");
            free(cmd);
            return -1;
        }
    }
    else
    {
        UBOOT_ERROR("Only Support EMMC now!!!\n");
    }

    free(cmd);
    UBOOT_TRACE("OK\n");
    return ret;
}
#endif

