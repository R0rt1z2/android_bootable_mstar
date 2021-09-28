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

#ifndef _Ms_UPGRADEUTILITY_H_
#define _Ms_UPGRADEUTILITY_H_
#include <MsTypes.h>
#include <ShareType.h>
#include <common.h>
#include <command.h>
#include <exports.h>

#define ENV_FORCEUPGRADE "force_upgrade"
#define ENV_UPGRADE_NAME        "UpgradeImage"
#define USB_DEVICE           "usb_device"
#define USB_PARTITION        "usb_partition"

typedef enum
{
    EN_UPDATE_MODE_NONE,
    EN_UPDATE_MODE_TFTP,
    EN_UPDATE_MODE_USB,
    EN_UPDATE_MODE_OAD,
    EN_UPDATE_MODE_NET,
    EN_UPDATE_MODE_USB_WITH_SEG,
    EN_UPDATE_MODE_USB_WITH_SEG_DECRYPTED,
    EN_UPDATE_MODE_NET_WITH_SEG_DECRYPTED,
}EN_UPDATE_MODE;

int check_script_encrypt(char *script_buf);
int check_file_partition(char *upgradeFile,int *device, int *partition);

void set_dont_overwrite_byname(char *upgradeFile);

int do_rw_en_update_mode (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_file_part_load (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_file_part_load_with_segment(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_check_file_partition(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);

void setUpgradeFsMountName(char* pFsMount);
char* getUpgradeFsMountName(void);
#ifdef HB_CUS
int do_checkOUIValid(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[]);
#endif
//---------------------------------------------------------------- ---------------------------------
/// Check if this cmd should be executed or not.
/// @param next_line  \b IN: cmd that will be executed later.
/// @return BOOLEAN   \b OUT: TRUE: This cmd should be skipped. FALSE:This cmd should be executed.
//-------------------------------------------------------------------------------------------------
BOOLEAN check_skip_cmd(char *next_line);

//---------------------------------------------------------------- ---------------------------------
/// This serial apis are for force write and re-create partitions ex: disable
/// This cmd is used to disable dont_overwrite function.
//-------------------------------------------------------------------------------------------------
int do_dont_overwrite_switch(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);

//---------------------------------------------------------------- ---------------------------------    
/// This serial apis are for protecting the specific partitions are overwrited. ex: customer
/// This cmd is used to init this function
//-------------------------------------------------------------------------------------------------
int do_dont_overwrite_init(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);

//---------------------------------------------------------------- ---------------------------------    
/// This serial apis are for protecting the specific partitions are overwrited. ex: customer
/// This cmd is used to register which partition should not be overwrite
//-------------------------------------------------------------------------------------------------
int do_dont_overwrite(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);

//---------------------------------------------------------------- ---------------------------------    
/// This serial apis are for protecting the specific partitions are overwrited. ex: customer
/// This cmd is used to free all registered partitions.
//-------------------------------------------------------------------------------------------------
int do_dont_overwrite_free(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);

//---------------------------------------------------------------- ---------------------------------    
/// This serial apis are for protecting the specific partitions are overwrited. ex: customer
/// This api is used to enable this function
//-------------------------------------------------------------------------------------------------
void dont_overwrite_enable(void);

//---------------------------------------------------------------- ---------------------------------    
/// This serial apis are for protecting the specific partitions are overwrited. ex: customer
/// This api is used to disable this function
//-------------------------------------------------------------------------------------------------
void dont_overwrite_disable(void);


//---------------------------------------------------------------- ---------------------------------    
/// These serial apis are for forcing to upgrade the specific partitions, even these partitions should not be overwrite. ex:customer
/// The priority of these serial apis are higher than do_dont_overwrite_xxxx
/// This api is used to init this function.
/// @return int                                 \b OUT: 0:success, -1: fail
//-------------------------------------------------------------------------------------------------
int do_force_overwirte_init(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);

//---------------------------------------------------------------- ---------------------------------    
/// These serial apis are for forcing to upgrade the specific partitions, even these partitions should not be overwrite. ex:customer
/// The priority of these serial apis are higher than do_dont_overwrite_xxxx
/// This api is used to register which partition must overwrite
/// @param partitionName.                 \b IN: partition name
/// @return int                                 \b OUT: 0:success, -1: fail
//-------------------------------------------------------------------------------------------------
int do_force_overwrite(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);

//---------------------------------------------------------------- ---------------------------------    
/// These serial apis are for forcing to upgrade the specific partitions, even these partitions should not be overwrite. ex:customer
/// The priority of these serial apis are higher than do_dont_overwrite_xxxx
/// This api is used to free all registered partitions.
/// @return int                                 \b OUT: 0:success, -1: fail
//-------------------------------------------------------------------------------------------------
int do_force_overwrite_free(void);

//---------------------------------------------------------------- ---------------------------------    
/// These serial apis are for forcing to upgrade the specific partitions, even these partitions should not be overwrite. ex:customer
/// The priority of these serial apis are higher than do_dont_overwrite_xxxx
/// This api is used to disable this function
//-------------------------------------------------------------------------------------------------
void do_force_overwrite_disable(void);

//---------------------------------------------------------------- ---------------------------------    
/// These serial apis are for forcing to upgrade the specific partitions, even these partitions should not be overwrite. ex:customer
/// The priority of these serial apis are higher than do_dont_overwrite_xxxx
/// This api is used to enable this function
//-------------------------------------------------------------------------------------------------
void do_force_overwrite_enable(void);

//-------------------------------------------------------------------------------------------------    
/// Check if full usb bin file is right or not.
/// @param  upgradeFile                 \b IN: The file name of upgrade image
/// @param  scriptBuf                     \b IN: The dram address of scriptBuf
/// @return  int                             \b OUT: 0: full file crc check is right. -1: full file crc is wrong
//-------------------------------------------------------------------------------------------------
int check_full_usb_crc(char * upgradeFile, char *scriptBuf);

//-------------------------------------------------------------------------------------------------
/// Check if the script file is right or not. This script file is in frot of the upgrade image.
/// @param  upgradeFile                 \b IN: The file name of upgrade image   
/// @param  scriptBuf                     \b IN: The dram address of scriptBuf   
/// @return  int                             \b OUT: 0: script file is right. -1: script file is wrong
//-------------------------------------------------------------------------------------------------
int check_script_crc(char * upgradeFile, char *scriptBuf);

//-------------------------------------------------------------------------------------------------    
/// Set the file name of upgrade image to uboot's environment variable
/// @param  str                             \b IN: The file name of upgrade image   
/// @return  int                             \b OUT: 0: set file name successfully. -1: set file name fail
//-------------------------------------------------------------------------------------------------
int setUpgradeFileName(char *str);

//-------------------------------------------------------------------------------------------------    
/// delete an environment variable, and this environmet recall the file name of upgrade image
/// @return  int                             \b OUT: 0: delete file name successfully. -1: delete file name fail
//-------------------------------------------------------------------------------------------------
int deleteUpgradeFileName(void);

//-------------------------------------------------------------------------------------------------    
/// setup the script file's size, and this script file is stored in front of the upgrade image.
/// The default size is 0x4000
/// @param  size                            \b IN: The file size of upgrade script file
/// @return  int                             \b OUT: 0: setup size successfully. -1: setup size fail
//-------------------------------------------------------------------------------------------------
int setScritSizeOfUpgradeImage(unsigned int size);

//-------------------------------------------------------------------------------------------------    
/// clear the script file's size to zero, and this script file is stored in front of the upgrade image.
/// @return  int                             \b OUT: 0: clear size successfully. -1: clear size fail
//-------------------------------------------------------------------------------------------------
int clearScritSizeOfUpgradeImage(void);

//-------------------------------------------------------------------------------------------------    
/// get the script file's size, and this script file is stored in front of the upgrade image.
/// If no setting, this function will return the default value. Default value is 0x4000 now.
/// @return  unsigned int                             \b OUT: 
//-------------------------------------------------------------------------------------------------
unsigned int getScritSizeOfUpgradeImage(void);

//-------------------------------------------------------------------------------------------------    
/// setup current upgrade mode
/// @return  unsigned int                             \b OUT: 0: setup successfully, -1: setup fail
//-------------------------------------------------------------------------------------------------
int setUpgradeMode(EN_UPDATE_MODE enMode);

//-------------------------------------------------------------------------------------------------    
/// get current upgrade mode
/// @return  EN_UPDATE_MODE                      \b OUT:
//-------------------------------------------------------------------------------------------------
EN_UPDATE_MODE getUpgradeMode(void);

//-------------------------------------------------------------------------------------------------    
/// calculate the total amount of commands in the script file
/// PS: This function will destory the datas that are stored in the buffer.
/// @param  script_buf                                  \b IN: the buffer for the script file
/// @return  unsigned int                               \b OUT: the total amount of commands
//-------------------------------------------------------------------------------------------------
unsigned int check_script_cmd_count(char *script_buf);

//---------------------------------------------------------------- ---------------------------------
/// Check whether OSD execution or not.
/// @return BOOLEAN   \b OUT: TRUE: This OSD should be run. FALSE:This OSD should be disable
//-------------------------------------------------------------------------------------------------
BOOLEAN check_osd_active(void);

//---------------------------------------------------------------- ---------------------------------
/// Print usb upgrade bin info.
/// @param  downloadBuf                                  \b IN: the buffer for the script file
/// @return  int                             \b OUT: 0: print info successfully. -1: print info fail
//-------------------------------------------------------------------------------------------------
int Print_UsbInfo(char *downloadBuf);

#if defined (CONFIG_R2_BRINGUP) || defined (CONFIG_AMP_TRUSTZONE_ENABLE)

//-------------------------------------------------------------------------------------------------    
/// This api are for upgrading nuttx 
//-------------------------------------------------------------------------------------------------
int do_update_nuttx(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
#endif
#endif
