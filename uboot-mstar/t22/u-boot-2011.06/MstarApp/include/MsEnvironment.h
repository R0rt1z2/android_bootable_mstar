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

#ifndef __MS_ENVIRONMENT_H__
#define __MS_ENVIRONMENT_H__

//#include <environment.h>
#include "MsTypes.h"

typedef enum
{
  E_LX = 0,
  E_LX2,
  E_LX3,
  E_LX4,
  E_MAX,
}LX_NUM;

typedef enum
{
    E_ID_NULL=0,

    E_LX_MEM,
    E_LX_MEM2,
    E_LX_MEM3,
    E_LX_MEM4,
    E_EMAC_MEM,//5

    E_GMAC_MEM,
    E_NUTTX_MEM,
    E_HW_AES_BUF,
    E_PM51_MEM,
    E_MAD_R2,//10

    E_MAD_SE,
    E_MAD_DEC,
    E_JPD_READ,
    E_JPD_WRITE,
    E_PHOTO_INTER,//15

    E_DFB_FRAMEBUFFER,
    E_XC_MAIN_FB,
    E_XC_FRC_PQ,
    E_XC_FRC_R,
    E_XC_FRC_L,//20

    E_XC1_MAIN_FB,
    E_XC_SELF,
    E_VE,
    E_BOOTLOGO_BUFFER,
    E_VDEC_BITSTREAM,//25

    E_VDEC_SUB_BITSTREAM,
    E_MBOOT_MEM_USAGE,
    E_RECOVERY_BUFFER,
    E_LZO_MEM_USAGE,
    E_LOCAL_DIMMING, //30

    E_DEMURA,
    E_FRC_R2,
    E_FAST_STR_BUFFER,
    E_NUTTX_STR_STATUS,
    E_DMX_SECBUF, //35

    E_VDEC_AEON,
    E_VDEC_FRAME_BUF,
    E_XC_MENULOAD_BUF,
    E_VIDEO_FILE_IN,
    E_TEE_RAMLOG, //40

    E_VDEC_CPU,
    E_OAD_DOWNLOAD,
    E_ID_XC_FRC_L_MEDS,
    E_ID_XC_FRC_R_MEDS,
    E_ID_XC_AUTODOWNLOAD,//45
    E_PM51_RT_MEM,
    E_ID_OPTEE_MEM_SHM,
    E_ID_DISPOUT_DEMURA,
    E_ID_HWC_GOP_BUF,
    E_MAP_MAX=0xFF,
}UBOOT_MMAP_ID;


typedef struct
{
    UBOOT_MMAP_ID     u8MapID;
    char *                      snMapID;
    char *                      miMapID;
}MMAP_MAPING;

#define MAP_TYPE_SN   1
#define MAP_TYPE_MI3  2

#if defined(ENABLE_MODULE_ENV_IN_SERIAL)
# if defined(CONFIG_SECURITY_BOOT)
#  ifndef  SEC_INFOR_SIZE
#   define SEC_INFOR_SIZE    0x10000
#  endif
#  ifndef  SEC_INFOR_ADDR
#   define SEC_INFOR_ADDR    (CONFIG_ENV_ADDR - SEC_INFOR_SIZE)
#  endif
#  ifndef  SEC_INFOR_ADDR_BAK
#   define SEC_INFOR_ADDR_BAK    (SEC_INFOR_ADDR - SEC_INFOR_SIZE)
#  endif
#endif


#if  defined(CONFIG_ENV_IS_IN_NAND)
# if  defined(CONFIG_SECURITY_BOOT)
#  ifndef  SEC_INFOR_BLOCK_NUM
#   define SEC_INFOR_BLOCK_NUM    0x3
#  endif
#  ifndef  SEC_INFOR_ADDR
#   define SEC_INFOR_ADDR    (CONFIG_ENV_ADDR - (SEC_INFOR_BLOCK_NUM * nand->erasesize))
#  endif
#  ifndef  SEC_INFOR_ADDR_BAK
#   define SEC_INFOR_ADDR_BAK    (SEC_INFOR_ADDR - (SEC_INFOR_BLOCK_NUM * nand->erasesize))
#  endif
# endif
# endif
#endif

#define MAX_MMAP_ID_LEN 35
#define ENV_CFG_PREFIX  "ENV="
#if (ENABLE_MODULE_ENV_IN_NAND==1)
#define ENV_CFG         "ENV=NAND"
#elif (ENABLE_MODULE_ENV_IN_UBI == 1)
#define ENV_CFG         "ENV=UBI"
#elif (ENABLE_MODULE_ENV_IN_SERIAL==1)
#define ENV_CFG         "ENV=SERIAL"
//#elif (CONFIG_COMMANDS & CONFIG_ENV_IN_SERIAL_512K)
//#define ENV_CFG         "ENV=SERIAL_512K"
#elif (ENABLE_MODULE_ENV_IN_MMC == 1)
#define ENV_CFG         "ENV=EMMC"
#elif (ENABLE_MODULE_ENV_IN_UFS == 1)
#define ENV_CFG         "ENV=UFS"
#else
#error "Wrong ENV_CFG settings!"
#endif

#define SECURITY_ENV_CFG_PREFIX  "SECURITY="
#if defined(CONFIG_SECURITY_BOOT)
#define SECURITY_ENV_CFG         "SECURITY=ON"
#else
#define SECURITY_ENV_CFG         "SECURITY=OFF"
#endif

#define FAST_STR_ENV_CFG         "FSTR"

#define INFO_EXCHANGE_CFG           "info_exchange"
#if (ENABLE_MODULE_INFO_IN_UBI_FILE == 1)
#define INFO_EXCHANGE_STORAGE       "ubifile"
#elif (ENABLE_MODULE_INFO_IN_SERIAL_ENV == 1)
#define INFO_EXCHANGE_STORAGE       "spi"
#elif (ENABLE_MODULE_INFO_IN_MMC_ENV == 1)
#define INFO_EXCHANGE_STORAGE       "emmc"
#else
#define INFO_EXCHANGE_STORAGE       "unknow"
#endif


MS_U32 Get_lzo_addr_Info(void);
MS_U32 Set_lzo_addr_Info(unsigned int lzo_start_address, unsigned int lzo_len);

int do_testmode_set(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_set_bootargs( cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_del_boogargs_cfg (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_set_bootargs_cfg (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_sync_mmap_to_env( cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_mmap_to_env( cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_config2env( cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_add_bootcheckpoints (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
//-------------------------------------------------------------------------------------------------
/// get value from environment if available otherwise set value to default value
/// @param id                                  \b IN: environment variable name
/// @param defval                              \b IN: default value
/// @param addr                                \b IN: address of variable to hold the return value
/// @return char *                             \b OUT: -1:FAIL, 0:Sucess
//-------------------------------------------------------------------------------------------------
int get_value_from_env(char *id, unsigned int defval, MS_PHYADDR *addr);

//-------------------------------------------------------------------------------------------------
/// Delet the member that exist in bootargs
/// For example: del_bootargs_cfg("autotest",0)
/// @param delCfg                           \b IN: the member that exist in the env 'bootargs'
////@param bDontSaveEnv                      \b IN: 0, saveenv , 1, don't saveenv
/// @return  int                              \b OUT: 0, delete sucessfully , -1, delete fail
//-------------------------------------------------------------------------------------------------
int del_bootargs_cfg(char *delCfg,MS_BOOL bDontSaveEnv);

//-------------------------------------------------------------------------------------------------
/// Edit a new member to bootargs
/// For example: set_bootargs_cfg("autotest","autotest=ture",0);
/// @param prefix_cfg                           \b IN: the new member's prefix
/// @param setCfg                               \b IN: the new member's content
////@param bDontSaveEnv                           \b IN: 0, saveenv , 1, don't saveenv
/// @return  int                                   \b OUT: 0, edit sucessfully , -1, edit fail
//-------------------------------------------------------------------------------------------------
int set_bootargs_cfg(char * prefix_cfg,char *setCfg,MS_BOOL bDontSaveEnv);


//-------------------------------------------------------------------------------------------------
/// Restore the envirnoment variable from supernova's mmap
/// @param id_mmap                           \b IN: the id  in mmap
/// @param env_addr                               \b IN: the name of addr in env
/// @param env_len                               \b IN: the name of len in env
/// @return  int                                   \b OUT: 0, edit sucessfully , -1, edit fail
/// example:
/// #define E_MMAP_ID_PM51_USAGE_MEM_AVAILABLE                     0x0000200000
/// id = "E_MMAP_ID_PM51_USAG"
/// env_addr="PM51_ADDR"
/// env_leng="PM51_LEN"
//-------------------------------------------------------------------------------------------------
int setMmapInfo2Env(char *id_mmap, char *env_addr, char *env_len);

//-------------------------------------------------------------------------------------------------
/// get address and size from env by input LX_ID
/// @INPUT: LX_ID                  \b IN/OUT: addr and size.
//-------------------------------------------------------------------------------------------------
MS_BOOL Get_LX_MemInfo(LX_NUM LxMem, MS_U64 *addr, unsigned int *size);

//-------------------------------------------------------------------------------------------------
/// Set the boot time information to envirnoment variable 'bootargs'
/// @param none
/// @return  none
//-------------------------------------------------------------------------------------------------
void _boottime_set_to_env(void);

MS_BOOL is_overlap_mboot_mmap(char *mmap_id);
int get_map_addr_from_env(UBOOT_MMAP_ID id, unsigned int defval, MS_PHYADDR *addr);
int get_map_size_from_env(UBOOT_MMAP_ID id, unsigned int defval, MS_PHYADDR *size);
//-------------------------------------------------------------------------------------------------
/// Get the config_value from bootargs
/// @param prefix_cfg                   \b IN : the prefix config
/// @param cfg_val_buf                  \b OUT: the buffer to store the config value
/// @param cfg_val_max_len              \b IN : the max length of config value
/// @return  int                        \b OUT: 0, sucessfully; -1, failed
//-------------------------------------------------------------------------------------------------
int get_bootargs_cfg(char *prefix_cfg, char *cfg_val_buf, int cfg_val_max_len);

#if CONFIG_RESCUE_ENV
int    saveenv_rescue     (void);
#endif

#if !defined(CONFIG_ANDROID_BOOT) && !defined(CONFIG_MI_CONFIG_PARTITION)
int do_reload_MAC_to_env(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
#endif

#endif
