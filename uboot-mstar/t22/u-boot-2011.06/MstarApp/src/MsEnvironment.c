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
#include <environment.h>
#include <errno.h>
#include <MsTypes.h>
#include <MsDebug.h>
#include <MsRawIO.h>
#include <MsEnvironment.h>
#include <MsMmap.h>
#include <MsUtility.h>
#include <miu/MsDrvMiu.h>
#include <MsSystem.h>
#include <MsApiSpi.h>

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define MMAP_ID_LXMEM "E_LX_MEM"
#define MMAP_ID_EMAC "E_EMAC_MEM"
#define MMAP_ID_GMAC "E_GMAC_MEM"

#define MMAP_ID_GE "E_MST_GEVQ"
#define MMAP_ID_GOP "E_MST_GOP_REGDMA"
#define MMAP_ID_PM51_USAGE "E_MMAP_ID_PM51_USAGE_MEM"
#define MMAP_ID_FLUSH_BUFFER "E_MMAP_ID_FLUSH_BUFFER"
#define MMAP_ID_PM51_CODE "E_MMAP_ID_PM51_CODE_MEM"
#define MMAP_ID_XC_MAIN_FB "E_MMAP_ID_XC_MAIN_FB"
#define MMAP_ID_FRC_4K2K "E_MMAP_ID_FRC_4K2K"


#define ENV_DRAM_LEN      "DRAM_LEN"
#define ENV_LX_MEM_LEN    "LX_MEM"
#define ENV_EMAC_MEM_LEN  "EMAC_MEM"
#define ENV_GMAC_MEM      "GMAC_MEM"

#define ENV_LX_MEM2       "LX_MEM2"
#define ENV_LX_MEM3       "LX_MEM3"
#define ENV_LX_MEM4       "LX_MEM4"
#define ENV_PM51_ADDR     "PM51_ADDR"
#define ENV_PM51_LEN      "PM51_LEN"
#define ENV_PM51_RT_ADDR  "PM51_RT_ADDR"
#define ENV_PM51_RT_LEN   "PM51_RT_LEN"

#define ENV_MBOOT_ADDR   "E_MMAP_ID_MBOOT_MEM_USAGE_ADR"
#define ENV_MBOOT_LEN      "E_MMAP_ID_MBOOT_MEM_USAGE_LEN"

#define ENV_GE_MEM "GE_ADDR"
#define ENV_GE_LEN "GE_LEN"
#define ENV_GOP_MEM "GOP_ADDR"
#define ENV_GOP_LEN "GOP_LEN"
#define ENV_TEE_MODE "tee_mode"
#define ENV_SAR5 "SAR5"
#define ENV_PM_PATCH "pm_path"
#define ENV_SYNC_MMAP "sync_mmap"
#define ENV_RAM_LOG "RAMLOG"
#define ENV_RECOVER_BUFFER "recovery_fbaddr"
#define ENV_RT_PM "RTPM_INFO"
#define ENV_RT_FRC_R2 "FRC_R2_INFO"
#if defined(__ARM__)
#define MIU1_LOGIC_ADR_OFFSET 0xA0000000
#else
#define MIU1_LOGIC_ADR_OFFSET 0x60000000
#endif

#if (CONFIG_OAD_IN_MBOOT)
#define OAD_IN_MBOOT         "OAD_IN_MBOOT"
#endif

#define ENV_51ONRAM         "51OnRam"
#if ((ENABLE_BOOTING_FROM_EXT_SPI_WITH_PM51==1)||(ENABLE_BOOTING_FROM_OTP_WITH_PM51==1))
#define ENV_51ONRAM_CFG         "1"
#else
#define ENV_51ONRAM_CFG         "0"
#endif

#define BOOTARGS_ITEM_MAX_LEN   192
#define TEMP_BUFFER_SIZE 64

#define ANDROID_BOOT_STATE "androidboot.verifiedbootstate"

//This flag need to set to 0 if FAST_STR is disabled
#define ENABLE_ENABLE_RTPM 1

extern BOOLEAN bIsLoadMmapSuccess;
char miuID[][MAX_MMAP_ID_LEN]={
    "MIU0_GROUP_SELMIU",
    "MIU0_GROUP_PRIORITY",
    "MIU1_GROUP_SELMIU",
    "MIU1_GROUP_PRIORITY",
    "MIU2_GROUP_SELMIU",
    "MIU2_GROUP_PRIORITY"
};

char mmapID[][MAX_MMAP_ID_LEN]={
        "E_LX_MEM",
        "E_LX_MEM2",
        "E_LX_MEM3",
        "E_LX_MEM4",
        "E_EMAC_MEM",
        "E_GMAC_MEM",
        "E_MMAP_ID_KERNEL_RANGE0",
        "E_MMAP_ID_NUTTX_MEM",
        "E_MMAP_ID_HW_AES_BUF",
        "E_MMAP_ID_PM51_CODE_MEM",
        "E_MMAP_ID_PM51_USAGE_MEM",
        "E_MMAP_ID_MAD_R2",
        "E_MMAP_ID_MAD_SE",
        "E_MMAP_ID_XC_MLG",
        "E_MMAP_ID_MAD_DEC",
        "E_MMAP_ID_MAD_COMMON",
        "E_MMAP_ID_JPD_READ",
        "E_MMAP_ID_JPD_WRITE",
        "E_MMAP_ID_PHOTO_INTER",
        "E_DFB_FRAMEBUFFER",
        "E_MMAP_ID_GOP_FB",
        "E_MMAP_ID_XC_MAIN_FB",
        "E_MMAP_ID_FRC_4K2K",
        "E_MMAP_ID_VE",
        "E_MMAP_ID_VDEC_CPU",
        "E_MMAP_ID_VDEC_BITSTREAM",
        "E_MMAP_ID_VDEC_SUB_BITSTREAM",
        "E_MMAP_ID_VDEC_FRAMEBUFFER", // VDEC_FRAMEBUFFER
        "E_MMAP_ID_MBOOT_MEM_USAGE",
        "E_MMAP_ID_DIP_10M_2",
        "E_MMAP_ID_CAMERA",
        "E_MMAP_ID_BOOTLOGO_BUFFER",
        "E_MMAP_ID_RECOVERY_BUFFER",
        "E_MMAP_ID_LZO_MEM_USAGE",
        "E_MMAP_ID_XC_FRC_PQ",
        "E_MMAP_ID_XC_FRC_R",
        "E_MMAP_ID_XC_FRC_L",
        "E_MMAP_ID_XC1_MAIN_FB",
        "E_MMAP_ID_XC_SELF",
#if (CONFIG_LOCAL_DIMMING==1)
        "E_MMAP_ID_LOCAL_DIMMING", // local dimming_BUFFER
#endif
#if (CONFIG_ENABLE_DEMURA==1)
        "E_MMAP_ID_DEMURA",
#endif
#if (CONFIG_URSA12_VB1_FRC_BRINGUP)
        "E_MMAP_ID_FRC_R2",
#endif
#if (CONFIG_MSTAR_FAST_STR)
        "E_MMAP_ID_FAST_STR_BUFFER",
        "E_MMAP_ID_NUTTX_STR_STATUS",
#endif
#if (CONFIG_MSTAR_R2_STR)
        "E_MMAP_ID_NUTTX_STR_STATUS",
#endif
        "E_MMAP_ID_DMX_SECBUF",
#if (CONFIG_BOOTVIDEO == 1)
    "E_MMAP_ID_VDEC_AEON",
    "E_MMAP_ID_VDEC_FRAME_BUF",
    "E_MMAP_ID_VDEC_BITSTREAM",
    "E_MMAP_ID_XC_MENULOAD_BUF",
    "E_MMAP_ID_VIDEO_FILE_IN",
#endif
    "E_MMAP_ID_TEE_RAMLOG",
    "E_MMAP_ID_RECOVERY_BUFFER_ADR",
    "E_MMAP_ID_MBOOT_GOP_SPINNER_BUFFER",
    "E_MMAP_ID_XC_AUTODOWNLOAD",
    };

#if (ENABLE_MODULE_ENV_IN_SERIAL)
#define SECTOR_SIZE   0x10000
#define ENV_SECTOR_SIZE   SECTOR_SIZE
#elif (ENABLE_MODULE_ENV_IN_NAND)
extern  int ubi_get_volume_size(char *, size_t* );
extern int ubi_get_leb_size(void);
#define SECTOR_SIZE   ubi_get_leb_size()
#define ENV_SECTOR_SIZE   SECTOR_SIZE
#elif (ENABLE_MODULE_ENV_IN_MMC)
#define SECTOR_SIZE   0x200
#define ENV_SECTOR_SIZE   0x10000
#elif (ENABLE_MODULE_ENV_IN_UFS)
#include "MsUfs.h"
#define SECTOR_SIZE   0x1000
#define ENV_SECTOR_SIZE   0x10000
#endif

static MMAP_MAPING staUbootMapMaping[] = {
    { E_ID_NULL                , NULL                                                     , NULL},

    { E_LX_MEM                , "E_LX_MEM"                                           , "MI_KERNEL_POOL1"},
    { E_LX_MEM2              , "E_LX_MEM2"                                          , "MI_KERNEL_POOL2"},
    { E_LX_MEM3              , "E_LX_MEM3"                                          , "MI_KERNEL_POOL3"},
    { E_LX_MEM4              , "E_LX_MEM4"                                          , "MI_KERNEL_POOL4"},
    { E_EMAC_MEM           , "E_EMAC_MEM"                                       , "EMAC_MEM"},

    { E_GMAC_MEM          , "E_GMAC_MEM"                                       , "GMAC_MEM"},
    { E_NUTTX_MEM         , "E_MMAP_ID_NUTTX_MEM"                       , NULL},
    { E_HW_AES_BUF       , "E_MMAP_ID_HW_AES_BUF"                     , NULL},
    { E_PM51_MEM           , "E_MMAP_ID_PM51_USAGE_MEM"             , "MI_PM51_MEM"},
    { E_MAD_R2               , "E_MMAP_ID_MAD_R2"                             , "MI_MAD_ADV_BUF"},

    { E_MAD_SE                , "E_MMAP_ID_MAD_SE"                             , "MI_MAD_SE_BUF"},
    { E_MAD_DEC             , "E_MMAP_ID_MAD_DEC"                           , "MI_MAD_DEC_BUF"},
    { E_JPD_READ             , "E_MMAP_ID_JPD_READ"                                  , "MI_JPD_READ_BUF"},
    { E_JPD_WRITE            , "E_MMAP_ID_JPD_WRITE"                        , "MI_JPD_OUT"},
    { E_PHOTO_INTER        , "E_MMAP_ID_PHOTO_INTER"                    , "MI_JPD_INTER_BUF"},

    { E_DFB_FRAMEBUFFER , "E_DFB_FRAMEBUFFER"                          , "MI_GOP_GWIN_RB"},
    { E_XC_MAIN_FB          , "E_MMAP_ID_XC_MAIN_FB"                    , "MI_XC_MAIN_FRAME_BUF"},
    { E_XC_FRC_PQ           , "E_MMAP_ID_XC_FRC_PQ"                     , NULL},
    { E_XC_FRC_R             , "E_MMAP_ID_XC_FRC_R"                       , NULL},
    { E_XC_FRC_L              , "E_MMAP_ID_XC_FRC_L"                       , NULL},

    { E_XC1_MAIN_FB         , "E_MMAP_ID_XC1_MAIN_FB"                , "MI_XC1_MAIN_FRAME_BUF"},
    { E_XC_SELF                 , "E_MMAP_ID_XC_SELF"                        , NULL},
    { E_VE                          , "E_MMAP_ID_VE"                                 , "MI_VE_FRAME_BUF"},
    { E_BOOTLOGO_BUFFER , "E_MMAP_ID_BOOTLOGO_BUFFER"       , "MI_BOOTLOGO_BUF"},
    { E_VDEC_BITSTREAM   , "E_MMAP_ID_VDEC_BITSTREAM"          , "MI_VDEC_BITSTREAM"},

    { E_VDEC_SUB_BITSTREAM, "E_MMAP_ID_VDEC_SUB_BITSTREAM", "MI_VDEC1_BITSTREAM"},
    { E_MBOOT_MEM_USAGE     , "E_MMAP_ID_MBOOT_MEM_USAGE"   ,"MI_MBOOT_MEM_USAGE"},
    { E_RECOVERY_BUFFER       , "E_MMAP_ID_RECOVERY_BUFFER"     ,"MI_AN_RECOVERY_BUF"},
    { E_LZO_MEM_USAGE          , "E_MMAP_ID_LZO_MEM_USAGE"       , "MI_AN_LZO_MEM_USAGE"},
    { E_LOCAL_DIMMING           , "E_MMAP_ID_LOCAL_DIMMING"        , NULL},

    { E_DEMURA                      , "E_MMAP_ID_DEMURA"                       ,NULL},
    { E_FRC_R2                       , "E_MMAP_ID_FRC_R2"                        ,NULL},
    { E_FAST_STR_BUFFER       , "E_MMAP_ID_FAST_STR_BUFFER"        ,NULL},
    { E_NUTTX_STR_STATUS    , "E_MMAP_ID_NUTTX_STR_STATUS"     ,NULL},
    { E_DMX_SECBUF              , "E_MMAP_ID_DMX_SECBUF"                , "MI_TSP_SECT_BUF"},

    { E_VDEC_AEON                , "E_MMAP_ID_VDEC_AEON"                 ,"MI_VDEC_AEON"},
    { E_VDEC_FRAME_BUF       , "E_MMAP_ID_VDEC_FRAME_BUF"         ,"MI_VDEC_FRAME_BUF"},
    { E_XC_MENULOAD_BUF     , "E_MMAP_ID_XC_MENULOAD_BUF"      ,"MI_MENULOAD_BUF"},
    { E_VIDEO_FILE_IN            , "E_MMAP_ID_VIDEO_FILE_IN"             ,NULL},
    { E_TEE_RAMLOG               , "E_MMAP_ID_TEE_RAMLOG"                ,NULL},

    { E_VDEC_CPU                 , "E_MMAP_ID_VDEC_CPU"                     ,NULL},
    { E_OAD_DOWNLOAD        , "E_MMAP_ID_OAD_DOWNLOAD"           ,NULL},
    { E_ID_XC_FRC_L_MEDS        , "E_MMAP_ID_XC_FRC_L_MEDS"           ,NULL},
    { E_ID_XC_FRC_R_MEDS        , "E_MMAP_ID_XC_FRC_R_MEDS"           ,NULL},
    { E_ID_XC_AUTODOWNLOAD ,"E_MMAP_ID_XC_AUTODOWNLOAD", NULL},
    { E_PM51_RT_MEM, NULL, "MI_PM51_RT_MEM"},
    { E_ID_OPTEE_MEM_SHM ,"E_MMAP_ID_OPTEE_MEM_SHM", "E_MMAP_ID_OPTEE_MEM_SHM"},
    { E_ID_DISPOUT_DEMURA, NULL, "MI_DISPOUT_DEMURA"},
    { E_ID_HWC_GOP_BUF, NULL, "MI_AN_HWC_GOP_BUF"},
};
#define MMAP_PATH_ONESN "MMAP_PATH"

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
extern unsigned int cfg_env_offset;
extern unsigned long gOpteeFailFlag;
//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Extern Functions
//-------------------------------------------------------------------------------------------------
extern int snprintf(char *str, size_t size, const char *fmt, ...);
//-------------------------------------------------------------------------------------------------
//  Private Functions
//-------------------------------------------------------------------------------------------------
static int del_cfg(char *source,char *delCfg);
static int set_info_exchange_cfg(void);
static int set_security_cfg(void);
static int set_panelinit_cfg(void);
static void if_51OnRam_set(void);
static int set_env_cfg(void);
int syncMmap2bootargs(void);
static int _syncLen2bootargs(UBOOT_MMAP_ID id, char *envName);
static int _syncAddr2bootargs(UBOOT_MMAP_ID id, char *envName);
int delMmapInfoFromEnv(void);
int saveMmapInfoToEnv(void);
int syncMmapToEnv(void);
int processMmapInfo(BOOLEAN save2env);
int processMmapInfoOnEnv(char *ptr, BOOLEAN save2env);
int checkUbootFromEnv(void);
MS_BOOL is_overlap_mboot_mmap(char *mmap_id);

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

#if !defined(CONFIG_ANDROID_BOOT) && !defined(CONFIG_MI_CONFIG_PARTITION)
int do_reload_MAC_to_env(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    int ret = -1;
    MS_U8 u8FileBuffer[14]="\0";
    U32 u32fileSize=0;
    UBOOT_TRACE("IN\n");

    char *reload_mac = getenv("reload_mac");
    // check reload mac env, if reload_mac != true, skip this command
    if( reload_mac != NULL )
    {
        if( strcmp(reload_mac, "true") == 0 )
        {
            run_command("setenv reload_mac",0);
            saveenv();
        }
        else
        {
            return ret;
        }
    }
    else
    {
        return ret;
    }

    memset((void*)u8FileBuffer,0,14);

#ifdef CONFIG_UBI
    // mount UBIRO for read certificate
    ret=run_command("ubi part UBIRO",0);
    if(ret==-1)
    {
        UBOOT_ERROR("run_command=ubi part UBIRO failed!\n");
        return ret;
    }

    ret=run_command("ubifsmount certificate",0);
    if(ret==-1)
    {
        UBOOT_ERROR("run_command=ubifsmount certificate failed!\n");
        return ret;
    }
#else
    vfs_mount("certificate");
#endif
    u32fileSize=vfs_getsize("/MAC.bin");
    UBOOT_DEBUG("u32fileSize=0x%x\n",u32fileSize);

    if(u32fileSize == 0)
    {
        UBOOT_ERROR("MAC.bin read failed!\n");
        return -1;
    }

    ret = vfs_read((void *)u8FileBuffer,"/MAC.bin",0,u32fileSize);
    if (ret == 0)
    {
        char strCommand[256];
        snprintf(strCommand, 256, "%02X:%02X:%02X:%02X:%02X:%02X", u8FileBuffer[0], u8FileBuffer[1], u8FileBuffer[2], u8FileBuffer[3], u8FileBuffer[4], u8FileBuffer[5]);
        UBOOT_DEBUG("%s\n",strCommand);
        setenv("ethaddr", strCommand);
        setenv("macaddr", strCommand);
        saveenv();
    }
    else
    {
        UBOOT_ERROR("load MAC.bin file faile>>>>>>>>>>\n");
        return ret;
    }

#if CONFIG_RESCUE_ENV
    ret=run_command("BrickTerminator backup_env",0);
    UBOOT_DEBUG("BrickTerminator backup_env\n");
    if(ret==-1)
    {
        UBOOT_ERROR("run_command=BrickTerminator backup_env failed!\n");
        return ret;
    }
    ret=run_command("BrickTerminator backup_env_end",0);
    UBOOT_DEBUG("BrickTerminator backup_env_end\n");
    if(ret==-1)
    {
        UBOOT_ERROR("run_command=BrickTerminator backup_env failed!\n");
        return ret;
    }
#endif

    UBOOT_TRACE("OK\n");
    return ret;
}
#endif

int get_value_from_env(char *id, unsigned int defval, MS_PHYADDR *addr)
{
    char *pEnv=NULL;

    UBOOT_TRACE("IN\n");
    if((id==NULL))
    {
        UBOOT_ERROR("One of the parameters is a null pointer\n");
        return -1;
    }
    UBOOT_DEBUG("id=%s\n",id);

    pEnv = getenv(id);
    if(pEnv)
    {
        *addr = (MS_PHYADDR)simple_strtoull(pEnv, NULL, 16);
        UBOOT_DEBUG("*addr=0x%llx from environment\n",(MS_U64)*addr);

        if (is_overlap_mboot_mmap(id) != TRUE)
        {
            UBOOT_TRACE("OK\n");
            return -1;
        }
    }
    else
    {
        if(defval!=NO_DEFAULT_MMAP_VALUE)
        {
            *addr = defval;
	        UBOOT_DEBUG("*addr=0x%llx from defval\n",(MS_U64)*addr);
		}
		else
		{
	        UBOOT_DEBUG("no mmap(%s) information!!\n",id);
            return -1;
		}
    }

    UBOOT_TRACE("OK\n");
    return 0;
}

static int get_map_ID(UBOOT_MMAP_ID id, char *mapID,char * type)
{
    char *pEnv=NULL;
    char *tempID = NULL;

    UBOOT_TRACE("IN\n");
    if((id==NULL)||(type==NULL))
    {
        UBOOT_ERROR("One of the parameters is a null pointer\n");
        return -1;
    }
    pEnv = getenv("MAP_TYPE");
   if(pEnv!=NULL && strcmp(pEnv, "MI")==0) //mi3.0 format
    {
        UBOOT_DEBUG(" MI ID=%s\n",staUbootMapMaping[id].miMapID);
        if((staUbootMapMaping[id].miMapID)==NULL)
        {
            UBOOT_ERROR("Map ID maping to [%d] a null pointer !!\n",staUbootMapMaping[id].u8MapID);
            UBOOT_TRACE("OK\n");
            return -1;
        }
        tempID=staUbootMapMaping[id].miMapID;
    }
   else //SN AN other map format...
    {
        UBOOT_DEBUG(" SN ID=%s\n",staUbootMapMaping[id].snMapID);
        if((staUbootMapMaping[id].snMapID)==NULL)
        {
            UBOOT_ERROR("Map ID maping to [%d] a null pointer !!\n",staUbootMapMaping[id].u8MapID);
            UBOOT_TRACE("OK\n");
            return -1;
        }
        tempID=staUbootMapMaping[id].snMapID;
    }
    snprintf(mapID,TEMP_BUFFER_SIZE,"%s%s",tempID,type);
    UBOOT_DEBUG("newID: %s\n",mapID);

    UBOOT_TRACE("OK\n");
    return 0;
}
static int _get_new_map_format_from_env(UBOOT_MMAP_ID id, unsigned int defval, MS_PHYADDR *addr,char * type)
{
    char newID[TEMP_BUFFER_SIZE]="\0";
    int ret=0;

    UBOOT_TRACE("IN\n");
    ret=get_map_ID(id,newID,type);
    UBOOT_DEBUG("buffer: %s\n",newID);
    if(ret!=0)
    {
        UBOOT_ERROR("One of the parameters is a null pointer\n");
        return -1;
    }
    ret=get_value_from_env(newID,defval,addr);
    if(ret!=0)
    {
            UBOOT_ERROR("get %s fail\n",newID);
            return -1;
    }
    UBOOT_TRACE("OK\n");
    return 0;

}

int get_map_addr_from_env(UBOOT_MMAP_ID id, unsigned int defval, MS_PHYADDR *addr)
{

    int ret=0;
    UBOOT_TRACE("IN\n");
    ret=_get_new_map_format_from_env(id,defval,addr,"_ADR");
    UBOOT_TRACE("OK\n");
    return ret;

}
int get_map_size_from_env(UBOOT_MMAP_ID id, unsigned int defval, MS_PHYADDR *size)
{

    int ret=0;
    UBOOT_TRACE("IN\n");
    ret=_get_new_map_format_from_env(id,defval,size,"_LEN");
    UBOOT_TRACE("OK\n");
    return ret;

}
static void get_mmap_range(char *mmap_addr_str, char *mmap_len_str, MS_U64 *pmap_begin, MS_U64 *pmap_end)
{
    char *pEnv;
    MS_U64 temp;

    pEnv = getenv(mmap_addr_str);
    if(pEnv)
    {
        *pmap_begin = simple_strtoull(pEnv, NULL, 16);
    }
    pEnv = getenv(mmap_len_str);
    if(pEnv)
    {
        temp = simple_strtoull(pEnv, NULL, 16);
        *pmap_end = *pmap_begin + temp;
    }
}


static int get_mmap_range_by_id(char *mmap_id, MS_U64 *pmap_begin, MS_U64 *pmap_end)
{
#define  MMAP_ADR_STRING           "_ADR"
#define  MMAP_LEN_STRING           "_LEN"
#define  MMAP_ADR_STRING_LEN       4
#define  MMAP_ID_MIN_LEN           5
#define  MMAP_ID_MAX_LEN           512

    int len, sub_len;
    char str_buf[MMAP_ID_MAX_LEN];
    char *pstr, *paddr_str, *plen_str;

    len = strlen(mmap_id);
    if ((len < MMAP_ID_MIN_LEN) || (len > MMAP_ID_MAX_LEN - 1))
    {
        UBOOT_ERROR("%s is not a vaild mmap id\n", mmap_id);
        return -1;
    }
    sub_len = len - MMAP_ADR_STRING_LEN;
    pstr = mmap_id + sub_len;
    if (strcmp(pstr, MMAP_ADR_STRING) != 0)
    {
        // a mmap_id not endwith "_ADR"
        return -1;
    }

    paddr_str = mmap_id;
    // to generate mmap_id_len
    memcpy(str_buf, mmap_id, sub_len);
    memcpy(str_buf + sub_len, MMAP_LEN_STRING, MMAP_ADR_STRING_LEN);
    str_buf[sub_len + MMAP_ADR_STRING_LEN] = NULL;
    plen_str = str_buf;

    get_mmap_range(paddr_str, plen_str, pmap_begin, pmap_end);
    return 0;

#undef  MMAP_ADR_STRING
#undef  MMAP_ADR_STRING_LEN
#undef  MMAP_ID_MIN_LEN
#undef  MMAP_ID_MAX_LEN
}


MS_BOOL is_overlap_mboot_mmap(char *mmap_id)
{
    int i;
    static int init_flag = 0;
    MS_U64 mmap_a_begin = 0, mmap_a_end = 0;
    static MS_U64 mboot_mmap_begin = 0,  mboot_mmap_end = 0;

    UBOOT_TRACE("IN\n");
    UBOOT_MMAP_ID skip_id[] = { E_MBOOT_MEM_USAGE, E_LX_MEM, E_LX_MEM2, E_LX_MEM3, E_LX_MEM4 };

    for (i = 0; i < ARRAY_SIZE(skip_id); i++)
    {
        char MapID[TEMP_BUFFER_SIZE] = "\0";
        memset(MapID, 0, sizeof(MapID));
        if(get_map_ID(skip_id[i], MapID,"_ADR") == 0)
        {
            if (strcmp(mmap_id, MapID) == 0)
            {
                // No Need to check MBOOT/LX Memory
                UBOOT_TRACE("OK\n");
                return TRUE;
            }
        }
    }

    if (init_flag == 0)
    {
        init_flag = 1;
        get_mmap_range(ENV_MBOOT_ADDR, ENV_MBOOT_LEN, &mboot_mmap_begin, &mboot_mmap_end);
    }
    if (get_mmap_range_by_id(mmap_id, &mmap_a_begin, &mmap_a_end) != 0)
    {
        // When mmap_id endwith '_LEN', then no need to check it.
        UBOOT_TRACE("OK\n");
        return TRUE;
    }

    if (mmap_a_end <= mboot_mmap_begin)        // On the left side of mboot_mmap ?
    {
        UBOOT_TRACE("OK\n");
        return TRUE;
    }else if (mmap_a_begin >= mboot_mmap_end)  // On the right side of mboot_mmap ?
    {
        UBOOT_TRACE("OK\n");
        return TRUE;
    }

    // Neither on left or right sides, it should overlap mboot_mmap;
    UBOOT_ERROR("The range of \033[31;1m%s \033[33;1moverlaps \033[31;1m%s\033[0m\n", mmap_id, ENV_MBOOT_ADDR);
    UBOOT_INFO("%-40s --> begin : 0x%llx, end : 0x%llx\n", ENV_MBOOT_ADDR, (MS_U64)mboot_mmap_begin, (MS_U64)mboot_mmap_end);
    UBOOT_INFO("%-40s --> begin : 0x%llx, end : 0x%llx\n", mmap_id, (MS_U64)mmap_a_begin, (MS_U64)mmap_a_end);
    UBOOT_TRACE("OK\n");
    return FALSE;
}

static int get_64lx_addr_from_env(UBOOT_MMAP_ID id, MS_U64 defval, MS_U64 *addr)
{
    char *pEnv=NULL;
    char newID[TEMP_BUFFER_SIZE]="\0";

    UBOOT_TRACE("IN\n");
    if((id==NULL)||(addr==NULL))
    {
        UBOOT_ERROR("One of the parameters is a null pointer\n");
        return -1;
    }
    if(get_map_ID(id,newID,"_ADR")!=0)
    {
        UBOOT_ERROR("One of the parameters is a null pointer\n");
        return -1;
    }

    UBOOT_DEBUG("buffer: %s\n",newID);
    pEnv = getenv(newID);
    if(pEnv)
    {
        *addr = (MS_U64)simple_strtoull(pEnv, NULL, 16);
         UBOOT_DEBUG("*addr=0x%llx from environment\n",(MS_U64)*addr);
    }
    else
    {
        if(defval!=NO_DEFAULT_MMAP_VALUE)
        {
            *addr = defval;
	      UBOOT_DEBUG("*addr=0x%llx from defval\n",(MS_U64)*addr);
	    }
	    else
	    {
	        UBOOT_DEBUG("no mmap(%d) information!!\n",id);
               return -1;
            }
        }

    UBOOT_TRACE("OK\n");
    return 0;

}


static int del_cfg(char *source,char *delCfg)
{
    char *substr = NULL;
    char *pPreEnvEnd = NULL;

    UBOOT_TRACE("IN\n");
    if(source==NULL)
    {
        UBOOT_ERROR("The input parameter 'source' is a null pointer\n");
        return -1;
    }

    if(delCfg==NULL)
    {
        UBOOT_ERROR("The input parameter 'delCfg' is a null pointer\n");
        return -1;
    }

    if(strlen(source)==0)
    {
        UBOOT_ERROR("The length of source is zero\n");
        return -1;
    }


    substr=strstr(source,delCfg);
    if(substr == NULL)
    {
        return 1;
    }
    else
    {
        pPreEnvEnd = strchr(substr,' ');
        if(((unsigned int)pPreEnvEnd-(unsigned int)source+1)<strlen(source))
        {
            strcpy(substr,pPreEnvEnd+1); //the +1 is for skip space
        }
        else
        {
            UBOOT_DEBUG("This member is the last one in the bootargs\n");
            //clear the rst of size.
            *(substr-1)='\0';
            memset(substr,0,strlen(substr));
        }
    }
    UBOOT_TRACE("OK\n");
    return 0;
}

int do_del_boogargs_cfg (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
   int ret;
   UBOOT_TRACE("IN\n");
   if(argc!=2)
   {
        return cmd_usage(cmdtp);
   }

   ret = del_bootargs_cfg(argv[1],0);
   if(ret==0)
   {
        UBOOT_TRACE("OK\n");
   }
   else
   {
        UBOOT_ERROR("delete %s fail\n",argv[1]);
   }
   return ret;
}

int del_bootargs_cfg(char *delCfg,MS_BOOL bDontSaveEnv)
{
    int ret=0;
    char *bootarg=NULL;
    char *OriArg = NULL;
    UBOOT_TRACE("IN\n");
    if(delCfg==NULL)
    {
        UBOOT_ERROR("The input parameter 'delCfg' is a null pointer\n");
        return -1;
    }

    bootarg = getenv("bootargs");
    if(bootarg==NULL)
    {
        UBOOT_ERROR("No env 'bootargs'\n");
        return -1;
    }

    OriArg=malloc(strlen(bootarg)+1);
    if(OriArg==NULL)
    {
        UBOOT_ERROR("malloc for tempBuf fail\n");
        return -1;
    }
    UBOOT_DEBUG("delete [%s] in bootargs\n",delCfg);
    memset(OriArg,0,strlen(bootarg)+1);
    strcpy(OriArg,bootarg);

    ret=del_cfg(OriArg,delCfg);
    if(ret==0)
    {
        UBOOT_DEBUG("delete [%s] in bootargs\n",delCfg);
        UBOOT_DEBUG("OriArg=%s\n",OriArg);
        setenv("bootargs", OriArg);
        if(0 == bDontSaveEnv)
	    {
		    saveenv();
		}
        UBOOT_TRACE("OK\n");
    }
    else if(ret==1)
    {
        UBOOT_DEBUG("No[%s] in bootargs\n",delCfg);
    }
    else
    {
        UBOOT_ERROR("delete %s fail\n",delCfg);
    }
    free(OriArg);
    return ret;
}

int do_set_bootargs_cfg (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
   int ret;
   unsigned int prefixLen=0;
   unsigned int contentLen=0;
   char *buffer=NULL;
   UBOOT_TRACE("IN\n");
   if(argc!=3)
   {
        return cmd_usage(cmdtp);
   }
   prefixLen=strlen(argv[1]);
   contentLen=strlen(argv[2]);
   //2? for '=' and the end symbol
   #define BUFFER_SIZE (prefixLen+contentLen+2)
   buffer=malloc(BUFFER_SIZE);
   if(buffer==NULL)
   {
       UBOOT_ERROR("malloc fail\n");
       return -1;
   }
   snprintf(buffer,BUFFER_SIZE,"%s=%s",argv[1],argv[2]);
   UBOOT_DEBUG("buffer: %s\n",buffer);
   ret = set_bootargs_cfg(argv[1],buffer,0);
   if(ret==0)
   {
        UBOOT_TRACE("OK\n");
   }
   else
   {
        UBOOT_ERROR("delete %s fail\n",argv[1]);
   }
   free(buffer);
   #undef BUFFER_SIZE
   return ret;
}

int set_bootargs_cfg(char * prefix_cfg, char *set_cfg, MS_BOOL bDontSaveEnv)
{
    char *preCheck=NULL;
    char *bootargs = NULL;
    char *OriArg = NULL;
    char *NewArg = NULL;
    int  NewArgLen = 0;
    char setCfg[BOOTARGS_ITEM_MAX_LEN+1];
    char scanBuf[BOOTARGS_ITEM_MAX_LEN+1];
    UBOOT_TRACE("IN\n");

    if(prefix_cfg==NULL)
    {
        UBOOT_ERROR("The input parameter 'prefix_cfg' is a null pointer\n");
        return -1;
    }

    if(set_cfg == NULL)
    {
        UBOOT_ERROR("The input parameter 'set_cfg' is a null pointer\n");
        return -1;
    }

    // Format the set_cfg
    memset(setCfg,  0, BOOTARGS_ITEM_MAX_LEN+1);
    memset(scanBuf, 0, BOOTARGS_ITEM_MAX_LEN+1);
    strncpy(scanBuf, set_cfg, BOOTARGS_ITEM_MAX_LEN);
    sscanf(scanBuf, "%s", setCfg);
    UBOOT_DEBUG("setCfg = %s\n", setCfg);

    //get origin bootargs
    bootargs = getenv("bootargs");
    if (bootargs == NULL)
    {
        UBOOT_INFO("Warnning!!!!\nbootargs doesn't exist, add a new one!!\n");
        setenv("bootargs",setCfg);
        saveenv();
        return 0;
    }
    preCheck=strstr(bootargs,setCfg);
    if(preCheck!=0)
    {
        UBOOT_DEBUG("%s has already existed\n",setCfg);
        UBOOT_TRACE("OK\n");
        return 0;
    }

    OriArg = (char*)malloc(strlen(bootargs) + 1);
    if(OriArg == NULL)
    {
        UBOOT_ERROR("malloc for OriArg fail\n");
        return -1;
    }
    strncpy(OriArg, bootargs,strlen(bootargs)+1);
    UBOOT_DEBUG("OriArg=%s\n",OriArg);
    //if setCfg exist, delete it.
    del_cfg(OriArg,prefix_cfg);
    UBOOT_DEBUG("OriArg=%s\n",OriArg);
    //add the NewCfg to Arg's tail.
    NewArgLen = strlen(OriArg) + strlen(setCfg) + 2 ;
    if (NewArgLen >= CONFIG_SYS_CBSIZE)
    {
        UBOOT_DEBUG("The length of OriArg : %d\n", strlen(OriArg));
        UBOOT_DEBUG("The length of setCfg : %d\n", strlen(setCfg));
        UBOOT_ERROR("New bootargs( NewArgLen : %d) has exceeded the env_size( CONFIG_SYS_CBSIZE :%d )\n",
                     NewArgLen, CONFIG_SYS_CBSIZE);
        UBOOT_INFO("set_bootargs_cfg failed !\n");
        free(OriArg);
        return -1;
    }
    NewArg = malloc(NewArgLen);

    if(NewArg==NULL)
    {
        free(OriArg);
        UBOOT_ERROR("malloc for NewArg fail\n");
        return -1;
    }
    UBOOT_DEBUG("OriArg=%s\n",OriArg);

    snprintf(NewArg, NewArgLen, "%s %s", OriArg, setCfg);
    setenv("bootargs", NewArg);
    if(0 == bDontSaveEnv)
    {
        saveenv();
    }
    free(OriArg);
    free(NewArg);

    UBOOT_TRACE("OK\n");
    return 0;
}


int get_bootargs_cfg(char *prefix_cfg, char *cfg_val_buf, int cfg_val_max_len)
{
    char *bootargs = getenv("bootargs");

    UBOOT_TRACE("IN\n");
    if (bootargs == NULL)
    {
        UBOOT_DEBUG("bootargs is NULL\n");
        return -1;
    }
    if ((cfg_val_buf == NULL) || (cfg_val_max_len <= 0))
    {
        UBOOT_DEBUG("cfg_val_buf = NULL or cfg_val_max_len(%d) <= 0", cfg_val_max_len);
        return -1;
    }
    memset(cfg_val_buf, 0, cfg_val_max_len);

    char *ptr = strstr(bootargs, prefix_cfg);
    if (ptr == NULL)
    {
        UBOOT_DEBUG("%s not in bootargs\n", prefix_cfg);
        return -1;
    }
    else
    {
        char *pval_s = (char *)(ptr + strlen(prefix_cfg));
        int  val_len = strlen(pval_s);
        char *pval_e = strchr(pval_s, ' ');
        if (pval_e != NULL)
        {
            val_len = (int)(pval_e - pval_s);
        }
        if (val_len >= cfg_val_max_len)
        {
            UBOOT_INFO("cfg(%s) has exceed the max_len(%d)", pval_s, cfg_val_max_len-1);
            val_len = cfg_val_max_len - 1;
        }
        strncpy(cfg_val_buf, pval_s, val_len);
    }
    UBOOT_DEBUG("cfg_val = %s\n", cfg_val_buf);
    UBOOT_TRACE("OK\n");
    return 0;
}


#if defined (CONFIG_POINTS_HANDLER_ENABLE)
#define INFO_EN_CHECKPOINTS_CFG     "en_chk_p"
int do_add_bootcheckpoints (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]){
    int ret = 0;
    char str[2];
    UBOOT_TRACE("IN\n");

    if(argc > 1)   // any value to disable "test customer performance index"
    {
        str[0] = '0';
    }
    else
    {
        str[0] = '1';
    }
    str[1] = '\0';

    ret = setenv("en_chk_p", str);
    if(ret==0)
    {
        UBOOT_TRACE("OK\n");
        saveenv();
    }
    else
    {
        UBOOT_ERROR("set en_chk_p=1 to bootargs fail\n");
    }

    return ret;
}
#endif

int do_testmode_set(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    #define STR_SIZE 20
    #define OPTION_LIST "autotest","moduletest","hsl","mdebugtest"
    char TestItem[][STR_SIZE]={OPTION_LIST};
    char *TargetItem = NULL;
    char *buffer= NULL;
    int ret = 0;
    int i=0;

    UBOOT_TRACE("IN\n");
    if (argc < 2)
    {
        return cmd_usage(cmdtp);
    }
    else
    {
        TargetItem = argv[1];
    }

    // delete All TestItem in bootargs
    for(i=0; i<(sizeof(TestItem)/STR_SIZE); i++)
    {
        del_bootargs_cfg(TestItem[i],0);
    }

    // set test TargetItem into bootargs
    if(strncmp(TargetItem,"null",4)==0)
    {
        UBOOT_INFO("clear testmode in bootargs\n");
    }
    else
    {
        buffer=malloc(STR_SIZE+5);//'=true' = 6
        if(buffer==NULL)
        {
            UBOOT_ERROR("malloc fail\n");
            return -1;
        }
        sprintf(buffer,"%s=true",TargetItem);
        UBOOT_DEBUG("buffer = %s\n",buffer);
        ret=set_bootargs_cfg(TargetItem,buffer,0);
    }
    if(ret==0)
    {
        UBOOT_TRACE("OK\n");
    }
    else
    {
        UBOOT_ERROR("set %s=ture to bootargs fail\n",TargetItem);
    }
    free(buffer);
    return ret;
}

static int set_security_cfg(void)
{
    int ret = 0;
    UBOOT_TRACE("IN\n");
    ret=set_bootargs_cfg(SECURITY_ENV_CFG_PREFIX,SECURITY_ENV_CFG,0);
    if(ret==0)
    {
        UBOOT_TRACE("OK\n");
    }
    else
    {
        UBOOT_ERROR("set SECURITY=ON/OFF to bootargs fail\n");
    }
    return ret;
}

static int set_info_exchange_cfg(void)
{
    char* pInfo = getenv(INFO_EXCHANGE_CFG);
    UBOOT_TRACE("IN\n");
    if (!pInfo || strcmp(pInfo, INFO_EXCHANGE_STORAGE) != 0)
    {
        UBOOT_DEBUG("write %s=%s\n",INFO_EXCHANGE_CFG,INFO_EXCHANGE_STORAGE);
        setenv(INFO_EXCHANGE_CFG, INFO_EXCHANGE_STORAGE);
#if CONFIG_MINIUBOOT
#else
        saveenv();
#endif
    }
    UBOOT_TRACE("OK\n");
    return 0;
}

static int set_env_cfg(void)
{
   int ret=0;
   char *buf=NULL;
   UBOOT_TRACE("IN\n");

   buf=malloc(CMD_BUF);
   if(buf==NULL)
   {
       UBOOT_ERROR("malloc fail\n");
       return -1;
   }
   memset(buf,0,CMD_BUF);
   snprintf(buf,CMD_BUF,"ENV_VAR_OFFSET=0x%x", cfg_env_offset);
   UBOOT_DEBUG("cmd=%s\n",buf);

   ret=set_bootargs_cfg("ENV_VAR_OFFSET",buf,0);
   if(ret==-1)
   {
        UBOOT_ERROR("set %s to bootargs fail\n",buf);
        free(buf);
        return -1;
   }
   memset(buf,0,CMD_BUF);
   snprintf(buf,CMD_BUF,"ENV_VAR_SIZE=0x%x", CONFIG_ENV_VAR_SIZE);
   UBOOT_DEBUG("cmd=%s\n",buf);

   ret=set_bootargs_cfg("ENV_VAR_SIZE",buf,0);
   if(ret==-1)
   {
        UBOOT_ERROR("set %s to bootargs fail\n",buf);
        free(buf);
        return -1;
   }

   ret=set_bootargs_cfg(ENV_CFG_PREFIX,ENV_CFG,0);
   if(ret==-1)
   {
        free(buf);
        UBOOT_ERROR("set %s to bootargs fail.\n",ENV_CFG);
        return -1;
   }
   free(buf);
   UBOOT_TRACE("OK\n");
   return ret;
}

#define LOGO_IN_MBOOT "BOOTLOGO_IN_MBOOT"

static int set_panelinit_cfg(void)
{
    int ret =0;
    UBOOT_TRACE("IN\n");

#if ((CONFIG_PANEL_INIT) || (CONFIG_MINIUBOOT) || (CONFIG_BOOTLOGO_LITE))
    UBOOT_DEBUG("do config panel init\n");
    ret = set_bootargs_cfg(LOGO_IN_MBOOT,LOGO_IN_MBOOT,0);
#else
    UBOOT_DEBUG("no config panel init\n");
    ret = del_bootargs_cfg(LOGO_IN_MBOOT,0);
#endif
    if(ret!=0)
    {
        UBOOT_DEBUG("set %s to bootargs fail\n",LOGO_IN_MBOOT);
    }
    UBOOT_TRACE("OK\n");
    return ret;
}

#if (CONFIG_TV_CHIP== 0 && ENABLE_ANDROID_BOOT ==1)


static int set_android_cfg(void)
{
#define  ANDROID_MAC              "androidboot.mac="
#define  ANDROID_SERIAALNO        "androidboot.serialno="
#define  ANDROID_DEVICEID         "androidboot.deviceid="

    char cfg_buf[CONFIG_SYS_CBSIZE];
    char *pEnv = NULL;
    int ret = 0;

    UBOOT_TRACE("IN\n");
    memset(cfg_buf, 0, CONFIG_SYS_CBSIZE);
    pEnv = getenv("macaddr");
    if (pEnv != NULL)
    {
        snprintf(cfg_buf, CONFIG_SYS_CBSIZE, "androidboot.mac=%s", pEnv);
        ret = set_bootargs_cfg(ANDROID_MAC, cfg_buf, TRUE);
        if (ret != 0)
        {
            printf("append macaddr to bootargs failed!\n");
            return -1;
        }
    }

    pEnv = getenv("serialno");
    if (pEnv != NULL)
    {
        snprintf(cfg_buf, CONFIG_SYS_CBSIZE, "androidboot.serialno=%s", pEnv);
        ret = set_bootargs_cfg(ANDROID_SERIAALNO, cfg_buf, TRUE);
        if (ret != 0)
        {
            printf("append serialno to bootargs failed!\n");
            return -1;
        }
    }

    pEnv = getenv("deviceid");
    if (pEnv != NULL)
    {
        snprintf(cfg_buf, CONFIG_SYS_CBSIZE, "androidboot.deviceid=%s", pEnv);
        ret = set_bootargs_cfg(ANDROID_DEVICEID, cfg_buf, TRUE);
        if (ret != 0)
        {
            printf("append deviceid to bootargs failed!\n");
            return -1;
        }
    }
    UBOOT_TRACE("OK\n");
    return 0;
}

#endif

static void if_51OnRam_set(void)
{
    char* pInfo= NULL;
    UBOOT_TRACE("IN\n");
    pInfo = getenv(ENV_51ONRAM);
    if (pInfo == NULL)
    {
        UBOOT_DEBUG("setenv %s %s\n",ENV_51ONRAM,ENV_51ONRAM_CFG);
        setenv(ENV_51ONRAM,ENV_51ONRAM_CFG);
        saveenv();
    }
    UBOOT_TRACE("OK\n");
}

int do_config2env( cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    UBOOT_TRACE("IN\n");
    char* pInfo;
    pInfo = NULL;
#if (CONFIG_OAD_IN_MBOOT)
    pInfo = getenv(OAD_IN_MBOOT);
    if (pInfo == NULL)
    {
        UBOOT_DEBUG("setenv %s 1\n",OAD_IN_MBOOT);
        setenv(OAD_IN_MBOOT, "1");
        saveenv();
    }
#endif

#if (CONFIG_ENABLE_RTPM)

    pInfo = getenv("rtpm");
    if(pInfo==NULL)
    {
        UBOOT_DEBUG("No env 'rtpm'\n");
        setenv("rtpm","1");
        saveenv();
    }
#else
    pInfo = getenv("rtpm");
    if(pInfo!=NULL)
    {
        UBOOT_DEBUG("Clean env 'rtpm'\n");
        run_command("setenv rtpm",0);
        saveenv();
    }
#endif

    if_51OnRam_set();

    UBOOT_TRACE("OK\n");
    return 0;
}


int do_set_bootargs( cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    UBOOT_TRACE("IN\n");
    UBOOT_INFO("============= set bootargs ===============\n");
    set_info_exchange_cfg();
    set_panelinit_cfg();
    set_env_cfg();
    set_security_cfg();
#if (CONFIG_TV_CHIP== 0 && ENABLE_ANDROID_BOOT ==1)
    set_android_cfg();
#endif
    UBOOT_TRACE("OK\n");
    return 0;
}

int syncMmapToEnv(void)
{
    UBOOT_TRACE("IN\n");
    delMmapInfoFromEnv();
    saveMmapInfoToEnv();
    UBOOT_TRACE("OK\n");
    return 0;
}

extern unsigned int u32UbootStart;
extern unsigned int u32UbootEnd;
int checkUbootFromEnv(void)
{
    MS_PHYADDR pEnv_addr=0;
    unsigned int pEnv_len=0;

    UBOOT_TRACE("IN\n");

    if(get_map_addr_from_env(E_MBOOT_MEM_USAGE, NO_DEFAULT_MMAP_VALUE, &pEnv_addr)==0 &&  getenv("no_compare_uboot")==NULL)
    {

        if (get_map_size_from_env(E_MBOOT_MEM_USAGE, NO_DEFAULT_MMAP_VALUE, &pEnv_len)!=0)
        {
            UBOOT_DEBUG("There is no information about MBOOT relocation LEN in mmap.ini\n");
            return -1;
        }
        pEnv_addr = PA2VA(pEnv_addr);
#if (CONFIG_BOOT_SEQUENCE_FORCE_SPINOR || CONFIG_BOOT_SEQUENCE_FORCE_BFN || CONFIG_BOOT_SEQUENCE_FORCE_BFE)
        pEnv_addr = pEnv_addr + 0x1000;
        UBOOT_DEBUG("For STB-Sboot load its header(size:0x100) and chunk-header(size:0x400) in front of E_MMAP_ID_MBOOT_MEM\n");
#endif
#if defined(__ARM__)

        if (pEnv_addr==CONFIG_UBOOT_LOADADDR)
        {
            UBOOT_DEBUG("UBOOT Memory Address and MMAP match\n");
        }
#else
        if (CONFIG_UBOOT_LOADADDR==(pEnv_addr+0x0180))
        {
            UBOOT_DEBUG("UBOOT Memory Address and MMAP match\n");
        }

#endif
        else
        {
            UBOOT_ERROR("UBOOT Memory Address and MMAP mismatch!!!\n");
            UBOOT_ERROR("UBOOT Memory Address:0x%x\n",CONFIG_UBOOT_LOADADDR);
            UBOOT_ERROR("Address in MMAP:0x%llx\n",(MS_U64)pEnv_addr);
            UBOOT_ERROR("Skip this check mechanism by:\n");
            UBOOT_ERROR("setenv no_compare_uboot 1\n");
            // Do not directly break the download once the mmap not match.
            //jump_to_console();
        }
#if defined(__ARM__)

        if (u32UbootStart<pEnv_addr || (u32UbootEnd>pEnv_addr+pEnv_len) )
        {
            UBOOT_ERROR("UBOOT relocation ERROR\n");
            UBOOT_ERROR("Address in mmap.ini:0x%llx\n",(MS_U64) pEnv_addr);
            UBOOT_ERROR("Length in mmap.ini:0x%llx\n",(MS_U64) pEnv_len);
            UBOOT_ERROR("Start address for relocation:0x%x\n",u32UbootStart);
            UBOOT_ERROR("End address for relocation:0x%x\n",u32UbootEnd);
            UBOOT_ERROR("Skip this check mechanism by:\n");
            UBOOT_ERROR("setenv no_compare_uboot 1\n");
            jump_to_console();
        }
#endif

        UBOOT_DEBUG("UBOOT relocation correct\n");
    }

    UBOOT_TRACE("OK\n");
    return 0;
}

int delMmapInfoFromEnv(void)
{
    return processMmapInfo(FALSE);
}

int saveMmapInfoToEnv(void)
{
    int ret =0;
    int i=0;

    ret = processMmapInfo(TRUE);

    // Deal with MIU Setting
    for(i=0;i<(sizeof(miuID)/MAX_MMAP_ID_LEN);i++)
    {
        setMiu2env(miuID[i]);
    }

    return ret;
}
#if 0
int processMmapInfo(BOOLEAN save2env)
{
    #define ENV_STR_LEN 512
    int i=0;
    char *ptr=NULL;
    char tmp_str[ENV_STR_LEN]="\0";
    UBOOT_TRACE("IN\n");

    // Deal with Static mmapID
    for(i=0;i<(sizeof(mmapID)/MAX_MMAP_ID_LEN);i++)
    {
        ptr=mmapID[i];
        processMmapInfoOnEnv(ptr,save2env);
    }

    // Deal with Dynamic mmapID
    ptr=getenv("DynMmapID");
    if(ptr !=NULL)
    {
        strncpy(tmp_str,ptr,ENV_STR_LEN-1);
        ptr=strtok(tmp_str," ");
        while(ptr!=NULL)
        {
            processMmapInfoOnEnv(ptr,save2env);
            ptr=strtok(NULL," ");
        }
    }
    UBOOT_TRACE("OK\n");
    return 0;
}
#else
int processMmapInfo(BOOLEAN save2env)
{
    #define ENV_STR_LEN 512
    int i=0;
    char tmp_str[ENV_STR_LEN]="\0";
    char * pEnv=NULL;
    char *ptr=NULL;
    UBOOT_TRACE("IN\n");

    pEnv = getenv("MAP_TYPE");
   if(pEnv!=NULL && strcmp(pEnv, "MI")==0) //mi3.0 format
    {
        for(i=0;i<(sizeof(staUbootMapMaping)/sizeof(MMAP_MAPING));i++)
        {
            if(staUbootMapMaping[i].miMapID!=NULL)
            {
                UBOOT_DEBUG(" MI ID=%s\n",staUbootMapMaping[i].miMapID);
                ptr=staUbootMapMaping[i].miMapID;
                processMmapInfoOnEnv(ptr,save2env);
            }
        }
    }
   else //SN AN other map format...
    {
         for(i=0;i<(sizeof(staUbootMapMaping)/sizeof(MMAP_MAPING));i++)
        {
            if(staUbootMapMaping[i].snMapID!=NULL)
            {
                UBOOT_DEBUG(" SN ID=%s\n",staUbootMapMaping[i].snMapID);
                ptr=staUbootMapMaping[i].snMapID;
                processMmapInfoOnEnv(ptr,save2env);
            }
        }
    }

    // Deal with Dynamic mmapID
    pEnv=getenv("DynMmapID");
    if(pEnv !=NULL)
    {
        strncpy(tmp_str,pEnv,ENV_STR_LEN-1);
        pEnv=strtok(tmp_str," ");
        while(pEnv!=NULL)
        {
            processMmapInfoOnEnv(ptr,save2env);
            pEnv=strtok(NULL," ");
        }
    }
    UBOOT_TRACE("OK\n");
    return 0;
}
#endif
int processMmapInfoOnEnv(char *ptr, BOOLEAN save2env)
{
    U64 addr=0xFFFFFFFF;
    U32 len=0xFFFF;

    if(ptr ==NULL)
    {
       return 0;
    }
    UBOOT_DEBUG("mmapID=%s\n",ptr);
    if(save2env==TRUE)
    {
        if(0==set_mmap2env(ptr, &addr,&len))
        {
            save_addr_to_env(ptr,addr);
            save_len_to_env(ptr,len);
        }
    }
    else
    {
        del_addr_from_env(ptr);
        del_len_from_env(ptr);
    }

    return 0;
}

int syncMmap2bootargs(void)
{
    int ret=0;
    MS_U64 lxaddr=0;
    MS_PHYADDR addr=0;
    MS_PHYADDR len=0;
    MS_PHYADDR len_demura=0;
    MS_PHYADDR len_dram0=0;
    MS_PHYADDR addr_demura=0;
    //MS_PHYADDR len_hwc_gop=0;
    //MS_PHYADDR addr_hwc_gop=0;
    char buf[CMD_BUF]="\0";
    char tee_type[10]="\0";
    char* pInfo;
    pInfo = NULL;
    UBOOT_TRACE("IN\n");
    //0. clean LX  information
    del_bootargs_cfg(ENV_LX_MEM_LEN,TRUE);
    del_bootargs_cfg(ENV_LX_MEM2,TRUE);
    del_bootargs_cfg(ENV_LX_MEM3,TRUE);
    del_bootargs_cfg(ENV_LX_MEM4,TRUE);

    //0.1 clean tee_mode
    del_bootargs_cfg(ENV_TEE_MODE,TRUE);
    del_bootargs_cfg(ENV_PM_PATCH,TRUE);
    //clean MMAP_PATH
    del_bootargs_cfg(MMAP_PATH_ONESN,TRUE);
    //clean ram log
    del_bootargs_cfg(ENV_RAM_LOG,TRUE);
    del_bootargs_cfg(ENV_RECOVER_BUFFER,TRUE);
    //clean EMAC_MEM
    del_bootargs_cfg(ENV_EMAC_MEM_LEN,TRUE);
    //clean MMAP_ID_GMAC
    del_bootargs_cfg(ENV_GMAC_MEM,TRUE);
    //clean PM51 env
    del_bootargs_cfg(ENV_PM51_ADDR,TRUE);
    del_bootargs_cfg(ENV_PM51_LEN,TRUE);
    del_bootargs_cfg(ENV_PM51_RT_ADDR,TRUE);
    del_bootargs_cfg(ENV_PM51_RT_LEN,TRUE);
    //clean SRA5
    del_bootargs_cfg(ENV_SAR5,TRUE);

    del_bootargs_cfg(ENV_RT_PM,TRUE);
    del_bootargs_cfg(ENV_RT_FRC_R2,TRUE);

    //1.1 Sync LX_MEM
    ret=_syncLen2bootargs(E_LX_MEM,ENV_LX_MEM_LEN);
    if(ret!=0)
    {
        UBOOT_ERROR("sync LX_MEM fail\n");
        return -1;
    }

    //1.2 Sync LX_MEM2
    ret=get_64lx_addr_from_env(E_LX_MEM2, NO_DEFAULT_MMAP_VALUE, &lxaddr);
    if(ret!=0)
    {
        UBOOT_INFO("No %s information \n",E_LX_MEM2_ADR);
    }
    else
    {
        ret=get_map_size_from_env(E_LX_MEM2, NO_DEFAULT_MMAP_VALUE, &len);
        if(ret!=0)
        {
            UBOOT_ERROR("get %s fail\n",E_LX_MEM2_LEN);
            return -1;
        }
#if 0 // regression on WINSTON-911
        if(Check_HD_Panel())
        {
            UBOOT_INFO("HD model\n");
        }
        else
        {
            if(get_map_addr_from_env(E_ID_HWC_GOP_BUF, NO_DEFAULT_MMAP_VALUE, &addr_hwc_gop)!=0)
            {
                UBOOT_ERROR("get %s mmap fail\n", E_MMAP_ID_HWC_GOP_BUF_ADR);
            }
            get_map_size_from_env(E_ID_HWC_GOP_BUF, NO_DEFAULT_MMAP_VALUE, &len_hwc_gop);
            len_dram0=get_specific_dram_length(0);
            lxaddr=addr_hwc_gop;
            len=len_dram0-addr_hwc_gop;
            UBOOT_INFO("Non HD model len_dram0=0x%x addr_hwc_gop=0x%x, len_hwc_gop=0x%x, len=0x%x \n", len_dram0, addr_hwc_gop, len_hwc_gop, len);
        }
#endif
#if 0
        if(Check_TCON_Enable_Fromflash())
        {
            UBOOT_INFO("Tconless model\n");
        }
        else
        {
            if(get_map_addr_from_env(E_ID_DISPOUT_DEMURA, NO_DEFAULT_MMAP_VALUE,&addr_demura)!=0)
            {
                UBOOT_ERROR("get %s mmap fail\n",E_MMAP_ID_PM51_USAGE_MEM_ADR);
            }
            get_map_size_from_env(E_ID_DISPOUT_DEMURA, NO_DEFAULT_MMAP_VALUE, &len_demura);
            len_dram0=get_specific_dram_length(0);
            lxaddr=addr_demura;
            len=len_dram0-addr_demura;
            UBOOT_INFO("Non Tconless model len_dram0=0x%x addr_demura=0x%x, len_demura=0x%x ,len=0x%x \n",len_dram0,addr_demura,len_demura,len);
        }
#endif
        memset(buf,0,sizeof(buf));
        if( NULL == getenv("LX_MEM2_ADR_CUS"))
        {   //Use E_LX_MEM2_ADR as default
            snprintf(buf,CMD_BUF,"%s=0x%llx,0x%x",ENV_LX_MEM2,PA2BA(lxaddr),len);
        }
        else
        {   //Use customized value, patch for Kano 2G case
            snprintf(buf,CMD_BUF,"%s=%s,0x%x",ENV_LX_MEM2,getenv("LX_MEM2_ADR_CUS"),len);
        }
        UBOOT_DEBUG("buf=%s\n",buf);
        ret=set_bootargs_cfg(ENV_LX_MEM2,buf,1);
        if(ret!=0)
        {
            UBOOT_ERROR("sync LX_MEM2 fail\n");
            return -1;
        }
    }

    //1.3 Sync LX_MEM3
    ret=get_64lx_addr_from_env(E_LX_MEM3, NO_DEFAULT_MMAP_VALUE, &lxaddr);
    if(ret!=0)
    {
        UBOOT_INFO("No %s information \n",E_LX_MEM3_ADR);
    }
    else
    {
        ret=get_map_size_from_env(E_LX_MEM3, NO_DEFAULT_MMAP_VALUE, &len);
        if(ret!=0)
        {
            UBOOT_ERROR("get %s fail\n",E_LX_MEM3_LEN);
            return -1;
        }
        memset(buf,0,sizeof(buf));
        if( NULL == getenv("LX_MEM3_ADR_CUS"))
        {   //Use E_LX_MEM3_ADR as default
            snprintf(buf,CMD_BUF,"%s=0x%llx,0x%x",ENV_LX_MEM3,PA2BA(lxaddr),len);
        }
        else
        {   //Use customized value, patch for Kano 2G case
            snprintf(buf,CMD_BUF,"%s=%s,0x%x",ENV_LX_MEM3,getenv("LX_MEM3_ADR_CUS"),len);
        }
        UBOOT_DEBUG("buf=%s\n",buf);
        ret=set_bootargs_cfg(ENV_LX_MEM3,buf,1);
        if(ret!=0)
        {
            UBOOT_ERROR("sync LX_MEM3 fail\n");
            return -1;
        }
    }

    //1.4 Sync LX_MEM4
    ret=get_64lx_addr_from_env(E_LX_MEM4, NO_DEFAULT_MMAP_VALUE, &lxaddr);
    if(ret!=0)
    {
        UBOOT_INFO("No %s information \n",E_LX_MEM4_ADR);
    }
    else
    {
        ret=get_map_size_from_env(E_LX_MEM4, NO_DEFAULT_MMAP_VALUE, &len);
        if(ret!=0)
        {
            UBOOT_ERROR("get %s fail\n",E_LX_MEM4_LEN);
            return -1;
        }
        memset(buf,0,sizeof(buf));
        snprintf(buf,CMD_BUF,"%s=0x%llx,0x%x",ENV_LX_MEM4,PA2BA(lxaddr),len);
        UBOOT_DEBUG("buf=%s\n",buf);
        ret=set_bootargs_cfg(ENV_LX_MEM4,buf,1);
        if(ret!=0)
        {
            UBOOT_ERROR("sync LX_MEM4 fail\n");
            return -1;
        }
    }


    //2. Sync EMAC_MEM
    ret=_syncLen2bootargs(E_EMAC_MEM,ENV_EMAC_MEM_LEN);
    if(ret!=0)
    {
        UBOOT_DEBUG("No EMAC_MEM information\n");
    }
    //3. Sync MMAP_ID_GMAC
    ret=get_map_addr_from_env(E_GMAC_MEM, NO_DEFAULT_MMAP_VALUE, &addr);
    if(ret!=0)
    {
        UBOOT_DEBUG("No ID_GMAC information \n");
    }
    else
    {
        ret=get_map_size_from_env(E_GMAC_MEM, NO_DEFAULT_MMAP_VALUE, &len);
        if(ret!=0)
        {
            UBOOT_ERROR("get %s fail\n",E_GMAC_MEM_LEN);
            return -1;
        }
        memset(buf,0,sizeof(buf));
        snprintf(buf,CMD_BUF,"%s=0x%llx,0x%x",ENV_GMAC_MEM, PA2BA(addr),len);
        UBOOT_DEBUG("buf=%s\n",buf);
        ret=set_bootargs_cfg(ENV_GMAC_MEM,buf,1);
        if(ret!=0)
        {
            UBOOT_ERROR("sync %s fail\n",ENV_GMAC_MEM);
            return -1;
        }
    }

    //4. Sync PM51_ADDR
    ret=get_map_addr_from_env(E_PM51_MEM, NO_DEFAULT_MMAP_VALUE, &addr);
    if(ret!=0)
    {
        UBOOT_DEBUG("No %s information\n",E_MMAP_ID_PM51_USAGE_MEM_ADR);
    }
    else
    {
        ret=_syncAddr2bootargs(E_PM51_MEM,ENV_PM51_ADDR);
        if(ret!=0)
        {
            UBOOT_ERROR("get PM51_ADDR fail\n");
            return -1;
        }

        //6. Sync PM51_LEN
        ret=_syncLen2bootargs(E_PM51_MEM,ENV_PM51_LEN);
        if(ret!=0)
        {
            UBOOT_ERROR("get PM51_LEN fail\n");
            return -1;
        }

    }

    //5. Sync PM51_RT_ADDR
    ret = get_map_addr_from_env(E_PM51_RT_MEM, NO_DEFAULT_MMAP_VALUE, &addr);
    if(ret != 0)
    {
        UBOOT_DEBUG("No %s information\n", E_MMAP_ID_PM51_RT_MEM_ADR);
    }
    else
    {
        ret = _syncAddr2bootargs(E_PM51_RT_MEM, ENV_PM51_RT_ADDR);
        if(ret != 0)
        {
            UBOOT_ERROR("get PM51_RT_ADDR fail\n");
            return -1;
        }

        //6. Sync PM51_RT_LEN
        ret = _syncLen2bootargs(E_PM51_RT_MEM,ENV_PM51_RT_LEN);
        if(ret != 0)
        {
            UBOOT_ERROR("get PM51_RT_LEN fail\n");
            return -1;
        }

    }

    //7. Sync tee_mode
#if defined(CONFIG_TEE)
    strcpy(tee_type,"");
#if (CONFIG_R2_BRINGUP)
    strcpy(tee_type,"nuttx");
#elif (CONFIG_SECARM_OPTEE)
    strcpy(tee_type,"secarm");
#elif defined(CONFIG_MSTAR_NS_UBOOT) || defined(CONFIG_OPTEE_TEELOADER)
    if( gOpteeFailFlag == 0 )
        strcpy(tee_type,"optee");
    else
        strcpy(tee_type,"");
#else
    UBOOT_ERROR("Unknown tee_mode set\n");
#endif // #if (CONFIG_R2_BRINGUP)

    if( strlen(tee_type) > 1)
    {
        memset(buf,0,sizeof(buf));
        snprintf(buf,CMD_BUF,"tee_mode=%s",tee_type);
        UBOOT_DEBUG("buf=%s\n",buf);
        ret=set_bootargs_cfg(ENV_TEE_MODE,buf,1);
        if(ret!=0)
        {
            UBOOT_ERROR("sync %s fail\n","tee_mode");
                return -1;
        }
    }
#endif // #if defined(CONFIG_TEE)

#if defined(CONFIG_MMC)
    memset(buf,0,sizeof(buf));
    snprintf(buf,CMD_BUF,"SAR5=ON");
    ret=set_bootargs_cfg(ENV_SAR5,buf,1);
    if(ret!=0)
    {
        UBOOT_ERROR("sync %s fail\n",ENV_SAR5);
            return -1;
    }
#endif
//. RAM LOG
    if(get_map_addr_from_env(E_TEE_RAMLOG, NO_DEFAULT_MMAP_VALUE, &addr)==0)
    {
        if(get_map_size_from_env(E_TEE_RAMLOG, NO_DEFAULT_MMAP_VALUE, &len)==0)
        {
            memset(buf,0,sizeof(buf));
            snprintf(buf,CMD_BUF,"%s=0x%llx,0x%x",ENV_RAM_LOG,PA2BA(addr),len);
            UBOOT_DEBUG("buf=%s\n",buf);
            ret=set_bootargs_cfg(ENV_RAM_LOG,buf,1);
            if(ret!=0)
            {
                UBOOT_ERROR("sync ENV_RAM_LOG fail\n");
                return -1;
            }
        }
    }
    else
    {
        UBOOT_DEBUG("No %s information\n","E_MMAP_ID_TEE_RAMLOG_ADR");
    }
#if (ENABLE_MSTAR_FAST_STR==1 || ENABLE_MSTAR_R2_STR==1)

    if(get_map_addr_from_env(E_NUTTX_STR_STATUS, NO_DEFAULT_MMAP_VALUE, &addr)==0)
    {
        if(get_map_size_from_env(E_NUTTX_STR_STATUS, NO_DEFAULT_MMAP_VALUE, &len)==0)
        {
            memset(buf,0,sizeof(buf));
            snprintf(buf,CMD_BUF,"%s=0x%llx,0x%x",FAST_STR_ENV_CFG,PA2BA(addr),len);
            UBOOT_DEBUG("buf=%s\n",buf);
            ret=set_bootargs_cfg(FAST_STR_ENV_CFG,buf,1);
            if(ret!=0)
            {
                UBOOT_ERROR("sync ENV FSTR fail\n");
                return -1;
            }
        }
    }
    else
    {
        UBOOT_DEBUG("No %s information\n","E_MMAP_ID_NUTTX_STR_STATUS_ADR");
    }
#elif (ENABLE_MSTAR_STR_SBOOT_RESUME ==1)
    memset(buf,0,sizeof(buf));
    snprintf(buf,CMD_BUF,"%s=0x00",FAST_STR_ENV_CFG);
    UBOOT_DEBUG("buf=%s\n",buf);
    ret=set_bootargs_cfg(FAST_STR_ENV_CFG,buf,1);
    if(ret!=0)
    {
        UBOOT_ERROR("sync ENV FSTR fail\n");
        return -1;
    }
#endif

    pInfo=getenv("CONFIG_PATH");
    if (pInfo!=NULL)
    {
        memset(buf,0,sizeof(buf));
        snprintf(buf,CMD_BUF,"pm_path=%s/PM.bin",pInfo);
        UBOOT_DEBUG("pm_path:%s \n",buf);
        ret=set_bootargs_cfg(ENV_PM_PATCH,buf,1);
        if(ret!=0)
        {
            UBOOT_ERROR("sync %s fail\n",ENV_PM_PATCH);
            return -1;
        }

    }
    else
    {
        UBOOT_DEBUG("skip set pm_path\n");
    }
//Add MMAP_PATH for CONFIG_ONESN_ENABLE
#if defined(CONFIG_ONESN_ENABLE)
    mstar_project_ID_t ProjectID=GetProjectID();
    snprintf(buf,CMD_BUF,"%s=onebin/model_%d/%s",MMAP_PATH_ONESN,ProjectID.Model_ID,"mmap.ini");
    ret=set_bootargs_cfg(MMAP_PATH_ONESN,buf,1);
    if(ret!=0)
    {
        UBOOT_ERROR("sync %s fail\n",MMAP_PATH_ONESN);
        return -1;
    }
#endif
// Add RTPM info
#if (ENABLE_ENABLE_RTPM==1)
    ret=get_map_addr_from_env(E_PM51_MEM, NO_DEFAULT_MMAP_VALUE, &addr);
    if(ret!=0)
    {
        UBOOT_DEBUG("No %s information\n",E_MMAP_ID_PM51_USAGE_MEM_ADR);
    }
    else
    {
        memset(buf,0,sizeof(buf));
        snprintf(buf,CMD_BUF,"%s=1,0x%llx",ENV_RT_PM,PA2BA(addr));
        ret=set_bootargs_cfg(ENV_RT_PM,buf,1);
        if(ret!=0)
        {
            UBOOT_ERROR("get PM51_ADDR fail\n");
            return -1;
        }
    }
#endif
//Add FRC_R2 infor E_FRC_R2
#if (ENABLE_URSA12_VB1_FRC_BRINGUP==1 && CONFIG_SYS_SKIP_FAST_STR==1)
    ret=get_map_addr_from_env(E_FRC_R2, NO_DEFAULT_MMAP_VALUE, &addr);
    if(ret!=0)
    {
        UBOOT_DEBUG("No E_FRC_R2 information\n");
    }
    else
    {
        memset(buf,0,sizeof(buf));
        snprintf(buf,CMD_BUF,"%s=1,0x%llx",ENV_RT_FRC_R2,PA2BA(addr));
        ret=set_bootargs_cfg(ENV_RT_FRC_R2,buf,1);
        if(ret!=0)
        {
            UBOOT_ERROR("get FRC_R2 fail\n");
            return -1;
        }
     }
#endif

    UBOOT_TRACE("OK\n");
    return 0;
}

static int _syncLen2bootargs(UBOOT_MMAP_ID id, char *envName)
{
    int ret=0;
    MS_PHYADDR len=0;
    char buf[CMD_BUF];
    UBOOT_TRACE("IN\n");
    if(id==NULL)
    {
        UBOOT_ERROR("id is a null pointer\n");
        return -1;
    }
    if(envName==NULL)
    {
        UBOOT_ERROR("envName is a null pointer\n");
        return -1;
    }

    ret=get_map_size_from_env(id,NO_DEFAULT_MMAP_VALUE, &len);
    if(ret!=0)
    {
        UBOOT_DEBUG("no %s information!!\n",envName);
        return -1;
    }
    memset(buf,0,sizeof(buf));
    snprintf(buf,CMD_BUF,"%s=0x%x",envName,len);
    UBOOT_DEBUG("buf=%s\n",buf);
    ret=set_bootargs_cfg(envName,buf,1);
    if(ret==0)
    {
        UBOOT_TRACE("OK\n");
    }
    else
    {
        UBOOT_ERROR("sync %s to bootargs fail\n",id);
    }
    return 0;
}

static int _syncAddr2bootargs(UBOOT_MMAP_ID id, char *envName)
{
    int ret=0;
    MS_PHYADDR addr=0;
    char buf[CMD_BUF]="\0";
    UBOOT_TRACE("IN\n");
    if(id==NULL)
    {
        UBOOT_ERROR("id is a null pointer\n");
        return -1;
    }
    if(envName==NULL)
    {
        UBOOT_ERROR("envName is a null pointer\n");
        return -1;
    }

    ret=get_map_addr_from_env(id,NO_DEFAULT_MMAP_VALUE, &addr);
    if(ret!=0)
    {
        UBOOT_ERROR("get %s fail\n",envName);
        return -1;
    }

    memset(buf,0,sizeof(buf));
    snprintf(buf,CMD_BUF,"%s=0x%llx",envName,PA2BA(addr));
    UBOOT_DEBUG("buf=%s\n",buf);
    ret=set_bootargs_cfg(envName,buf,1);
    if(ret==0)
    {
        UBOOT_TRACE("OK\n");
    }
    else
    {
        UBOOT_ERROR("sync %d to bootargs fail\n",id);
    }
    return 0;
}

int setMmapInfo2Env(char *id_mmap, char *env_addr, char *env_len)
{
    char *buf=NULL;
    char *pEnv_addr=NULL;
    char *pEnv_len=NULL;
    int ret=0;
    MS_PHYADDR addr=0;
    unsigned int len=0;
    UBOOT_TRACE("IN\n");

    if((id_mmap==NULL)||(env_addr==NULL)||(env_len==NULL))
    {
        UBOOT_ERROR("One of the parameters is null pointer\n");
        return -1;
    }
    UBOOT_DEBUG("id_mmap=%s\n",id_mmap);
    UBOOT_DEBUG("env_addr=%s\n",env_addr);
    UBOOT_DEBUG("env_len=%s\n",env_len);

    pEnv_addr=getenv(env_addr);
    pEnv_len=getenv(env_len);
    if((pEnv_addr==NULL)||(pEnv_len==NULL))
    {
        buf=malloc(CMD_BUF);
        if(buf==NULL)
        {
            UBOOT_ERROR("malloc for buf fail\n");
            return -1;
        }
        memset(buf,0,CMD_BUF);

        ret=get_addr_from_mmap(id_mmap, &addr);
        if(ret!=0)
        {
            free(buf);
            UBOOT_ERROR("get %s addr fail\n",id_mmap);
            return -1;
        }

        ret=get_length_from_mmap(id_mmap, &len);
        if(ret!=0)
        {
            free(buf);
            UBOOT_ERROR("get %s len fail\n",id_mmap);
            return -1;
        }
        snprintf(buf,CMD_BUF,"setenv %s 0x%x;setenv %s 0x%llx;saveenv",env_addr,(MS_U64)addr,env_len,len);
        UBOOT_DEBUG("cmd=%s\n",buf);
        ret=run_command(buf,0);
        if(ret!=0)
        {
            free(buf);
            UBOOT_ERROR("set %s=%llx to env fail\n",(MS_U64)env_addr,(MS_U64)addr);
            return -1;
        }
        free(buf);
    }
    else
    {
        UBOOT_DEBUG("%s has already existed\n",id_mmap);
    }

    UBOOT_TRACE("OK\n");
    return 0;
}

int do_mmap_to_env( cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    int ret=0;
    UBOOT_TRACE("IN\n");
    if(argc!=4)
    {
        cmd_usage(cmdtp);
        return -1;
    }
    if((argv[1]==NULL)||(argv[2]==NULL)||(argv[3]==NULL))
    {
        UBOOT_ERROR("One of the parameters is null pointer\n");
        return -1;
    }
    ret=setMmapInfo2Env(argv[1],argv[2],argv[3]);
    if(ret!=0)
    {
        UBOOT_ERROR("handle %s fail\n",argv[1]);
        return -1;
    }
    UBOOT_TRACE("OK\n");
    return 0;
}

void set_kernel_protect_information(void)
{
    char cmdBuf[CMD_BUF]="\0";
    MS_U64 dram_length0=0;
    MS_U64 dram_length1=0;
    MS_U64 dram_length2=0;
    //unsigned int dram_length3=0;

    dram_length0=get_specific_dram_length(0);
    dram_length1=get_specific_dram_length(1);
    dram_length2=get_specific_dram_length(2);
    //dram_length3=get_specific_dram_length(3);

#if ENABLE_MONACO
    snprintf(cmdBuf,CMD_BUF,"DRAM_SIZE1=0x%llx DRAM_SIZE2=0x%llx DRAM_SIZE3=0x%llx",dram_length0,dram_length1,dram_length2);
#else
    snprintf(cmdBuf,CMD_BUF,"DRAM_SIZE1=0x%llx DRAM_SIZE2=0x%llx",dram_length0,dram_length1);
#endif

    UBOOT_DEBUG("command : setenv KERNEL_PROTECT %s\n",cmdBuf);
    setenv("KERNEL_PROTECT",cmdBuf);
}

int do_sync_mmap_to_env( cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    char cmdBuf[CMD_BUF]="\0";
    char *pEnv=NULL;
    char* force_upgrade = getenv("force_upgrade");
    unsigned char sync_mmap=0;
    UBOOT_TRACE("IN\n");

    if(NULL == force_upgrade)
    {
        if(argc!=1)
        {
            cmd_usage(cmdtp);
            return -1;
        }
        pEnv=getenv(ENV_SYNC_MMAP);
        if(pEnv!=NULL)
        {
            sync_mmap=simple_strtol(pEnv, NULL, 16);
        }
        else
        {
            UBOOT_DEBUG("no env 'sync_mmap'\n");
            sync_mmap=1;
        }

       if(sync_mmap==1)
       {
            // set bIsLoadMmapSuccess TRUE, force to reload mmap from mmap.ini
            bIsLoadMmapSuccess = TRUE;

            // sync mmap.ini to uboot environment.
            syncMmapToEnv();

            //Sync bootargs
            if(syncMmap2bootargs()==-1)
            {
                UBOOT_ERROR("sync the part of mmap info to bootargs fail\n");
                return -1;
            }

            // set CMA information to bootargs
            setCMA2bootargs();
#ifdef CONFIG_UMA_WITH_NARROW
            setDMAzoneargs();
#endif
            //setting Kernel Protect
            set_kernel_protect_information();

            // check UBOOT DRAM address and length

            checkUbootFromEnv();


            if(bIsLoadMmapSuccess == TRUE)
            {
                memset(cmdBuf,0,CMD_BUF);
                snprintf(cmdBuf,CMD_BUF,"setenv %s 0;saveenv",ENV_SYNC_MMAP);
                UBOOT_DEBUG("cmdBuf=%s\n",cmdBuf);
                if(0!=run_command(cmdBuf,0))
                {
                    UBOOT_ERROR("fail\n");
                    return -1;
                }
#if CONFIG_RESCUE_ENV
                run_command("BrickTerminator backup_env_end",0);
#endif
            }
        }
        else{
            UBOOT_DEBUG("The mmap info in env is the newest\n");
        }
    }
    UBOOT_TRACE("OK\n");
    return 0;
}

MS_BOOL Get_LX_MemInfo(LX_NUM LxMem, MS_U64 *addr, unsigned int *size)
{
    int ret =FALSE;
    switch(LxMem)
    {
        case E_LX:
            if (get_map_addr_from_env(E_LX_MEM,NO_DEFAULT_MMAP_VALUE, addr)==0)
            {
                get_map_size_from_env(E_LX_MEM,NO_DEFAULT_MMAP_VALUE, size);
                UBOOT_DEBUG("Lx1 start:[0x%llx]Size[0x%x]\n",*addr,*size);
                ret=TRUE;
            }
            break;
        case E_LX2:
            if (get_map_addr_from_env(E_LX_MEM2,NO_DEFAULT_MMAP_VALUE, addr)==0)
            {
                get_map_size_from_env(E_LX_MEM2,NO_DEFAULT_MMAP_VALUE, size);
                UBOOT_DEBUG("Lx2 start:[0x%llx]Size[0x%x]\n",*addr,*size);
                ret=TRUE;
            }
            break;
        case E_LX3:
            if (get_map_addr_from_env(E_LX_MEM3,NO_DEFAULT_MMAP_VALUE, addr)==0)
            {
                get_map_size_from_env(E_LX_MEM3,NO_DEFAULT_MMAP_VALUE, size);
                UBOOT_DEBUG("Lx3 start:[0x%llx]Size[0x%x]\n",*addr,*size);
                ret=TRUE;
            }
            break;
        case E_LX4:
            if (get_map_addr_from_env(E_LX_MEM4,NO_DEFAULT_MMAP_VALUE, addr)==0)
            {
                get_map_size_from_env(E_LX_MEM4,NO_DEFAULT_MMAP_VALUE, size);
                UBOOT_DEBUG("Lx4 start:[0x%llx]Size[0x%x]\n",*addr,*size);
                ret=TRUE;
            }
            break;
        default:
            UBOOT_DEBUG("exception lxmem number %d\n",LxMem);
            break;
    }
    return ret;
}
#ifdef CONFIG_LZO
MS_U32 Set_lzo_addr_Info(unsigned int lzo_start_address, unsigned int lzo_len)
{
    extern unsigned int g_lzo_start_address;
    extern unsigned int g_lzo_len;
    g_lzo_start_address = lzo_start_address;
    g_lzo_len = lzo_len;
    return 0;
}

MS_U32 Get_lzo_addr_Info(void)
{
    MS_PHYADDR addr  =0;

    extern unsigned int g_lzo_start_address;

    if(g_lzo_start_address != 0)
    {
        UBOOT_DEBUG("[AUTO_LZO_ADDRESS] LZO_MEM_START_ADR start:[0x%x]\n", g_lzo_start_address);
        return g_lzo_start_address;
    }

    if (get_map_addr_from_env(E_LZO_MEM_USAGE,NO_DEFAULT_MMAP_VALUE, &addr)==0)
    {
        UBOOT_DEBUG("E_LZO_MEM_USAGE start:[0x%x]\n",addr);
        return PA2VA((MS_U32)addr);
    }
    // no LZO_MEM_USAGE_ADR in env
    else
    {
        // set E_MMAP_ID_LZO_MEM_USAGE from mmap.ini to env
        processMmapInfoOnEnv("E_MMAP_ID_LZO_MEM_USAGE", TRUE);

        if (get_value_from_env("E_MMAP_ID_LZO_MEM_USAGE_ADR",NO_DEFAULT_MMAP_VALUE, &addr)==0)
        {
            UBOOT_DEBUG("E_MMAP_ID_LZO_MEM_USAGE_ADR start:[0x%llx]\n",(MS_U64)addr);
            return PA2VA((MS_U32)addr);
        }
        else
        {
            UBOOT_DEBUG("E_MMAP_ID_LZO_MEM_USAGE_ADR start:[0x%x]\n", CONFIG_UNLZO_DST_ADDR);
            return CONFIG_UNLZO_DST_ADDR;
        }
    }
}
#endif
#define BOOTTIME_SBOOT_STR "BOOTTIME_SBOOT"
#define BOOTTIME_UBOOT_STR "BOOTTIME_UBOOT"
#include <stdlib.h>
#include <malloc.h>
#include "MsSystem.h"

unsigned long G_MS_BOOTTIME_SBOOT=1; // global variable for storing the boot time used in sboot (ms)
unsigned long G_MS_BOOTTIME_UBOOT=1; // global variable for storing the boot time used in sboot (ms)

void _boottime_set_to_env(void)
{
    extern int snprintf(char *str, size_t size, const char *fmt, ...);
    char *strEnv = NULL;
    const int u32EnvSize = CMD_BUF;

    G_MS_BOOTTIME_UBOOT = MsSystemGetBootTime();

    strEnv = malloc(u32EnvSize);
    if(strEnv  != NULL)
    {
        memset(strEnv , 0, u32EnvSize);
        snprintf(strEnv , u32EnvSize-1, "%s=%lu", BOOTTIME_SBOOT_STR, G_MS_BOOTTIME_SBOOT);
        if(0 != set_bootargs_cfg((char*)BOOTTIME_SBOOT_STR, strEnv, 1))
        {
            printf("%s: Error: set_bootargs_cfg failed at %d\n", __func__, __LINE__);
        }

        memset(strEnv , 0,u32EnvSize);
        snprintf(strEnv , u32EnvSize-1, "%s=%lu", BOOTTIME_UBOOT_STR, G_MS_BOOTTIME_UBOOT);
        if(0 != set_bootargs_cfg((char*)BOOTTIME_UBOOT_STR, strEnv, 1))
        {
            printf("%s: Error: set_bootargs_cfg failed at %d\n", __func__, __LINE__);
        }
        free(strEnv );
    }
}


unsigned int GetRealOffset(unsigned int u32EnvOffset)
{
    unsigned int u32Size=0;
    unsigned int u32RealOffset=0;
    int ret = 0;
#if (ENABLE_MODULE_ENV_IN_SERIAL)
    ret = getSpiSize(&u32Size);
    u32RealOffset = u32Size - (u32EnvOffset*SECTOR_SIZE);
#elif (ENABLE_MODULE_ENV_IN_NAND)
    /*char cmd_buf[CMD_BUF]="\0";
    sprintf(cmd_buf, "ubi part %s", NAND_DEFAULT_PARTITION);
    run_command(cmd_buf, 0);*/
    check_ubi_partition(NAND_DEFAULT_VOLUME,NAND_DEFAULT_PARTITION);
    ubi_get_volume_size(NAND_DEFAULT_VOLUME,&u32Size);
    UBOOT_DEBUG("u32Size : 0x%x\n",u32Size);
    UBOOT_DEBUG("ubi_get_leb_size : 0x%x\n",SECTOR_SIZE);
    u32RealOffset = u32Size - (u32EnvOffset*SECTOR_SIZE);
#elif (ENABLE_MODULE_ENV_IN_MMC)
    #include <MsMmc.h>
    ret = get_mmc_partsize(MMC_DEFAULT_PARTITION,&u32Size);
    u32RealOffset = u32Size - (u32EnvOffset*SECTOR_SIZE);
#elif (ENABLE_MODULE_ENV_IN_UFS)
    ret = get_ufs_partsize(UFS_DEFAULT_PARTITION,&u32Size);
    u32RealOffset = u32Size - (u32EnvOffset*SECTOR_SIZE);
#else
    #error "please set the correct uboot environment storage!!\n"
#endif
    if(0 != ret)
    {
        UBOOT_ERROR("Error, at %d", __LINE__);
        return 0;
    }
    return u32RealOffset;
}

#if CONFIG_RESCUE_ENV
extern uchar default_environment_rescue[];
env_t *env_ptr_rescue = NULL;
extern int default_environment_rescue_size;

void env_relocate_spec_rescue (void);
int restoreenv_from_rescue(void)
{
    //NOTE: 1. write current env to flash
//    saveenv_rescue();
    //NOTE: 2. do recovery
    ssize_t    len=0;
    char *res=NULL;
    unsigned int u32EnvOffset=0;
    UBOOT_TRACE("IN\n");

    memset (env_ptr_rescue, 0, CONFIG_ENV_SIZE);
    env_relocate_spec_rescue();

    res = (char *)&(env_ptr_rescue->data);
    len = hexport_r(&env_htab_rescue, '\0', &res, ENV_SIZE);
    if (len < 0)
    {
        UBOOT_ERROR("Cannot export environment: errno = %d\n", errno);
        return -1;
    }
    MsApiChunkHeader_GetValue(CH_UBOOT_ENVIRONMENT_ROM_OFFSET,&u32EnvOffset);
    u32EnvOffset = GetRealOffset(u32EnvOffset);
    env_ptr_rescue->crc   = crc32(0, env_ptr_rescue->data, ENV_SIZE);
    int ret = raw_write((unsigned int)env_ptr_rescue, u32EnvOffset, CONFIG_ENV_SIZE);
    int retBak = raw_write((unsigned int)env_ptr_rescue, u32EnvOffset + ENV_SECTOR_SIZE, CONFIG_ENV_SIZE);
    if( (ret != 0) || (retBak != 0) )
    {
        return -1;
    }
    UBOOT_TRACE("OK\n");
    return 0;
}

int saveenv_rescue(void)
{
    ssize_t    len=0;
    char *res=NULL;
    unsigned int u32EnvRescueOffset = 0;
    UBOOT_TRACE("IN\n");
    res = (char *)&(env_ptr_rescue->data);
    len = hexport_r(&env_htab_rescue, '\0', &res, ENV_SIZE);
    if (len < 0) {
        error("Cannot export environment: errno = %d\n", errno);
        return -1;
    }

    env_ptr_rescue->crc   = crc32(0, env_ptr_rescue->data, ENV_SIZE);
    MsApiChunkHeader_GetValue(CH_RESCURE_ENVIRONMENT_ROM_OFFSET,&u32EnvRescueOffset);
    u32EnvRescueOffset = GetRealOffset(u32EnvRescueOffset);
    int ret = raw_write((unsigned int)env_ptr_rescue,u32EnvRescueOffset , CONFIG_ENV_SIZE);
    int retBak = raw_write((unsigned int)env_ptr_rescue,u32EnvRescueOffset + ENV_SECTOR_SIZE, CONFIG_ENV_SIZE);
    if( (ret != 0) || (retBak != 0) )
    {
        return -1;
    }
    UBOOT_TRACE("OK\n");
    return 0;
}

void env_relocate_spec_rescue (void)
{
    extern void set_default_env_rescue(const char *s);
    unsigned int u32EnvRescueOffset = 0;
    void* pEnvBuf = NULL;
    void* pEnvBufBak = NULL;
    int ret=0;
    UBOOT_TRACE("IN\n");

    pEnvBuf = malloc(CONFIG_ENV_SIZE);
    if(pEnvBuf==NULL)
    {
        UBOOT_ERROR("malloc() failed\n");
        return;
    }
    memset(pEnvBuf, 0, CONFIG_ENV_SIZE);
    MsApiChunkHeader_GetValue(CH_RESCURE_ENVIRONMENT_ROM_OFFSET,&u32EnvRescueOffset);
    u32EnvRescueOffset = GetRealOffset(u32EnvRescueOffset);
    printf("u32EnvRescueOffset = 0x%x\n",u32EnvRescueOffset);

    ret = raw_read((unsigned int)pEnvBuf, u32EnvRescueOffset, CONFIG_ENV_SIZE);
    if(ret != 0)
    {
        free(pEnvBuf);
        UBOOT_ERROR("Write CONFIG_ENV_RESCUE_ADDR Fail\n");
        return;
    }

    pEnvBufBak = malloc(CONFIG_ENV_SIZE);
    if(pEnvBufBak==NULL)
    {
        free(pEnvBuf);
        UBOOT_ERROR("malloc() failed\n");
        return;
    }
    memset(pEnvBufBak, 0, CONFIG_ENV_SIZE);

    ret = raw_read((unsigned int)pEnvBufBak, u32EnvRescueOffset+ENV_SECTOR_SIZE, CONFIG_ENV_SIZE);
    if(ret != 0)
    {
        UBOOT_ERROR("Write CONFIG_ENV_RESCUE_ADDR_BAK Fail\n");
        goto Exit;
    }

    size_t offsetCrc = offsetof(env_t, crc);
    size_t offsetData = offsetof(env_t, data);

    ulong crc = *(ulong *)(pEnvBuf + offsetCrc);
    ulong crcNew = crc32(0, pEnvBuf + offsetData, ENV_SIZE);
    ulong crcBak = *(ulong *)(pEnvBufBak + offsetCrc);
    ulong crcNewBak = crc32(0, pEnvBufBak + offsetData, ENV_SIZE);

    UBOOT_DEBUG("crc:       0x%04lx\n",  crc);//DBG
    UBOOT_DEBUG("crcNew:    0x%04lx\n",  crcNew);//DBG
    UBOOT_DEBUG("crcBak:    0x%04lx\n",  crcBak);//DBG
    UBOOT_DEBUG("crcNewBak: 0x%04lx\n",  crcNewBak);//DBG


    if ( (crc != crcNew) && (crcBak != crcNewBak) )
    {
        set_default_env_rescue("!bad CRC");
        memcpy (env_ptr_rescue->data, default_environment_rescue, default_environment_rescue_size);
        env_ptr_rescue->crc = crc32(0, env_ptr_rescue->data, ENV_SIZE);
        // gd->env_valid = 1;
        goto Exit;
    }

    if (crc != crcNew)
    {
        raw_write((unsigned int)pEnvBufBak,u32EnvRescueOffset, CONFIG_ENV_SIZE);
    }
    else if (crcBak != crcNewBak)
    {
        raw_write((unsigned int)pEnvBuf,u32EnvRescueOffset+ENV_SECTOR_SIZE, CONFIG_ENV_SIZE);
    }

    memcpy(env_ptr_rescue, (crc != crcNew) ? pEnvBufBak : pEnvBuf, CONFIG_ENV_SIZE);

    ret = env_import_rescue((const char *)env_ptr_rescue, 1);
    if (ret)
    {
        // gd->env_valid = 1;
    }
    else
    {
        UBOOT_ERROR("env_import_rescue() failed\n");
    }
    UBOOT_TRACE("OK\n");
Exit:
    free(pEnvBufBak);
    free(pEnvBuf);
}
#endif

