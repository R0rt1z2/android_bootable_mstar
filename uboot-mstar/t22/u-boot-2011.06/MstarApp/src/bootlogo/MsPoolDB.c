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
#include <MsTypes.h>

#include <common.h>
#include <command.h>
#include <malloc.h>
#include <drvBDMA.h>
#include <bootlogo/MsPoolDB.h>
#include <MsVfs.h>
#include <MsUtility.h>
#include <MsDebug.h>
#include <MsSystem.h>
#include <MsRawIO.h>
#include <drvSERFLASH.h>
#include <CusConfig.h>
#include <MsMmap.h>
#include <MsEnvironment.h>
#include <MsApiPanel.h>
#include <uboot_mmap.h>
#include <bootlogo/iniparser.h>
#include <bootlogo/MsBootLogo.h>
#if defined(CONFIG_CUS_PROJECT_ID_EEPROM) && (CONFIG_CUS_PROJECT_ID_EEPROM == 1)
#if defined(CONFIG_CMD_EEPROM) && (CONFIG_MSTARI2C == 1)
#include "MsEeprom.h"
#endif
#endif

#ifdef UFBL_FEATURE_IDME
#include <idme.h>
#endif
#define DEFAULT_MODEL_NAME "/tvconfig/config/model/Customer_1.ini"

#if defined(CONFIG_VOICE_CM4)
#include <MsApiCM4.h>
#endif
#if (CONFIG_URSA_6M30)
#include <../drivers/mstar/ursa/ursa_6m30.h>
#endif

#if (ENABLE_URSA_8 ==1 )
#include <../drivers/mstar/ursa/ursa_8.h>
#endif

#if (ENABLE_URSA_6M40==1 )
#include <../drivers/mstar/ursa/ursa_6m40.h>
#endif

#include <ursa/ursa_common.h>

#if (CONFIG_DYNAMIC_IR)
#include <MsBoot.h>
#endif

#if (CONFIG_KEYPAD)
#include <keypad_type.h>
#endif
//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
char const *sDB_INDEX[]=
{
    "E_DB_LOGO",
    "E_DB_MUSIC",
    "E_DB_BOARD_PARA",
    "E_DB_PANEL_PARA",
    "E_DB_URSA_6M30",
    "E_DB_URSA",
    "E_DB_TCON",
    "E_DB_PNL_VB1",
    "E_DB_PNL_GOPMUX",
    "E_DB_PWM",
    "E_DB_MISC",
    "E_DB_MSPI",
    "E_DB_DMA",
    "E_DB_HDMITX",
    "E_DB_IR_BOOTCHECK",
    "E_DB_IR_WAKEUP",
    "E_DB_RGBW",
    "E_DB_KEYPAD",
    "E_DB_3DLUT",
#if defined(CONFIG_DATA_SEPARATION)
    "E_DB_KEYPAD_WAKEUP",
#endif
#if defined(CONFIG_VOICE_CM4)
    "E_DB_MIC_CONFIG",
#if defined(CONFIG_URSA_TXRX_SUPPORT)
    "E_DB_URSA_TXRX_PANEL_PARA",
#endif
#if defined(CONFIG_PNL_OLED_LG_SUPPORT)
    "E_DB_OLED_LG_PANEL_PARA",
#endif
#endif
    "E_DB_MAX"
};
ST_DBTABLE gstDbtable;

static char gPnlPath[BUFFER_SIZE];
static char gBoardPath[BUFFER_SIZE];
static char gLogoPath[BUFFER_SIZE];
static char gMusicPath[BUFFER_SIZE];
static char gGopMuxPath[BUFFER_SIZE];
static char gTconBinPath[BUFFER_SIZE];
static char gRGBWBinPath[BUFFER_SIZE];
static char g3DLUTBinPath[BUFFER_SIZE];
static char gLocalDimmingPath[BUFFER_SIZE];
static char gKeypadPath[BUFFER_SIZE];

#if (CONFIG_DYNAMIC_IR)
static char gIRConfigPath[BUFFER_SIZE];
#endif

#if defined(CONFIG_DATA_SEPARATION)
#define POWER_KEY_BUFFER_LENGTH   15

static char gKeypadPath[BUFFER_SIZE];

#define gModelName "model"
#define gProjectID "gProjectID"
#define gEnvProjectID "CUSDATA_PROJECT_ID"
#define gKLProjectID "cusdata_projectid"
#define gProjectID_LEN 4
#define gProjectID_Path "project_id.ini"
#endif

#define ANDROIDFBRC_PATH     "an_dfbrc_ini_path"

#if (CONFIG_URSA_UNION == 1 || CONFIG_TV_CHIP == 1)
static char gModulePath[BUFFER_SIZE];
#endif

#if defined(CONFIG_CUS_PROJECT_ID_EEPROM) && (CONFIG_CUS_PROJECT_ID_EEPROM == 1)
#if defined(CONFIG_CMD_EEPROM) && (CONFIG_MSTARI2C == 1)
#define PROJECT_ID "projectID"
#define PROJECT_ID_ADDRESS "Address"
#endif
#endif

#define MPoolVolumeOffset(addr, u32DbtableOffset) (addr-u32DbtableOffset)

#define BOOTLOGO_PRE_PATH "/tvconfig/config/bootlogo/boot"
#define BOOTLOGO_ODM_PRE_PATH "/firetv_odm/config/bootlogo/boot"

#define ODM_INI_FILE_PATH "odm.ini"
#define BOOTLOGO_DEFAULT_PATH "/tvconfig/bootlogo.jpg"
#define BOOTLOGO_INI_SECTION_NAME "BOOTLOGO"
#define BOOTLOGO_INI_KEY_NAME "BOOTLOGO_NUMBER"
#define BOOTLOGO_INI_DEFAULT_VALUE ""

#define BOARD_INI_PRE_PATH "/vendor/tvconfig/config/board"
#define INI_SUFFIX ".ini"

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
pnl_VB1ChannelOrder stVB1Channel1order;
#if defined(CONFIG_URSA_TXRX_SUPPORT)
st_PNL_UrsaRXTX_PanelParameter gstUrsaRXTX_PanelParameter;
#endif
#if defined(CONFIG_PNL_OLED_LG_SUPPORT)
st_PNL_OLED_SpecificParameter gstPNL_OLED_SpecificParameter;
#endif
st_sys_misc_setting gstSysMiscSetting;
st_pwm_setting gstPWMSetting;
#if defined(CONFIG_VOICE_CM4)
stMic_config gstMicSetting;
#endif
static U8 *pDbBufferAddr = 0;
static U32 u32Dbload_size = 0;
st_hdmitx_setting stHdmitxSetting;

#if (CONFIG_BURN_CUSTOMER_IC)
char CustomerInPath[BUFFER_SIZE];
#endif

static char gIniBootLogo[BUFFER_SIZE];;
static bool bOdmModeEnable = 0;
static bool bTconOutput = 0;
static bool bODEnable = 0;
//-------------------------------------------------------------------------------------------------
//  Extern Functions
//-------------------------------------------------------------------------------------------------
extern char * strncpy(char * dest,const char *src,size_t count);
#ifndef UFBL_FEATURE_IDME
extern int snprintf(char *str, size_t size, const char *fmt, ...);
#endif
extern unsigned int GetRealOffset(unsigned int);

//-------------------------------------------------------------------------------------------------
//  Private Functions
//-------------------------------------------------------------------------------------------------
int Load_DataToDbBuffer(U32 u32DbtableOffset, DB_INDEX db_index, U32 pu32DataAddr);
int Load_LogoToFlash(U32 u32DbtableOffset);
int Load_PanelSetting_ToFlash(U32 u32DbtableOffset);
int Load_BoardSetting_ToFlash(U32 u32DbtableOffset);
int Load_MusicToFlash(U32 u32DbtableOffset);
int Load_GopMuxToFlash(U32 u32DbtableOffset);
int Load_TConToFlash(U32 u32DbtableOffset);
int Load_PnlVb1ToFlash(U32 u32DbtableOffset);
int Load_PWMSetting_ToFlash(U32 u32DbtableOffset);
int Load_MiscSetting_ToFlash(U32 u32DbtableOffset);
int Load_LDMPara_ToFlash(U32 u32DbtableOffset);
int Load_HDMITXToFlash(U32 u32DbtableOffset);
#if(CONFIG_DYNAMIC_IR)
int Load_IRConfig_ToFlash(U32 u32DbtableOffset);
#endif
int Load_RGBWBinToFlash(U32 u32DbtableOffset);
int Load_3DLUTBinToFlash(U32 u32DbtableOffset);
int Load_KeypadToFlash(U32 u32DbtableOffset);
#if defined(CONFIG_DATA_SEPARATION)
int Load_KeypadWakeup_ToFlash(U32 u32DbtableOffset);
#endif

#if defined(CONFIG_CUS_PROJECT_ID_EEPROM) && (CONFIG_CUS_PROJECT_ID_EEPROM == 1)
#if defined(CONFIG_CMD_EEPROM) && (CONFIG_MSTARI2C == 1)
int Load_PROJECT_ID_FROM_EEPROM(char *Buffer, int BufferSize);
#endif
#endif

static void _print_dbtable_info(void);
void count_data_crc32_save(U32 addr,int size,DB_INDEX db_index);
int count_data_crc32_cmp(U32 addr,int size, DB_INDEX db_index);
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

#if(CONFIG_ENABLE_V_I_PWM == 1)
int bSafeModeEnable = 0;
#endif
#if defined(CONFIG_VOICE_CM4)
int parse_voice_setting(void)
{
    #define MIC_SECTION_NAME "MIC_CONFIG"
    #define MIC_WAKEUP_SECTION_NAME "MIC_WAKEUP_CONFIG"
    UBOOT_TRACE("IN\n");

    // init gstMicSetting
    memset(&gstMicSetting,0,sizeof(stMic_config));

    // voice on/off
    gstMicSetting.voice_onoff = Profile_GetBoolean(MIC_SECTION_NAME, "VOICE", 0);
    gstMicSetting.keyword_onoff = Profile_GetBoolean(MIC_SECTION_NAME, "KEYWORD", 0);
    gstMicSetting.seamless_onoff = Profile_GetBoolean(MIC_SECTION_NAME, "SEAMLESS", 0);
    gstMicSetting.sigen_onoff = Profile_GetBoolean(MIC_SECTION_NAME, "SIGEN", 0);
    gstMicSetting.mic_number = Profile_GetInteger(MIC_WAKEUP_SECTION_NAME, "MIC_NUM",0);
    gstMicSetting.mic_gain = Profile_GetInteger(MIC_WAKEUP_SECTION_NAME, "MIC_GAIN",0);
    gstMicSetting.mic_bitwidth = Profile_GetInteger(MIC_WAKEUP_SECTION_NAME, "MIC_BITWIDTH",0);
    gstMicSetting.sw_aec_mode = Profile_GetInteger(MIC_SECTION_NAME, "SW_AEC_MODE",0);
    gstMicSetting.hw_aec_onoff = Profile_GetBoolean(MIC_SECTION_NAME, "HW_AEC", 0);
    gstMicSetting.i2s_onoff = Profile_GetBoolean(MIC_SECTION_NAME, "I2S", 0);
    gstMicSetting.da_onoff = Profile_GetBoolean(MIC_SECTION_NAME, "DA", 0);
    gstMicSetting.hpf_onoff = Profile_GetBoolean(MIC_WAKEUP_SECTION_NAME, "HPF", 0);
    gstMicSetting.hpf_config = Profile_GetInteger(MIC_WAKEUP_SECTION_NAME, "HPF_CONFIG",0);
    gstMicSetting.uart_onoff = Profile_GetBoolean(MIC_SECTION_NAME, "UART", 0);

    UBOOT_DEBUG("Voice on/off           = %d\n",gstMicSetting.voice_onoff);
    UBOOT_DEBUG("Keyword Wake Up on/off = %d\n",gstMicSetting.keyword_onoff);
    UBOOT_DEBUG("seamless on/off        = %d\n",gstMicSetting.seamless_onoff);
    UBOOT_DEBUG("sigen                  = %d\n",gstMicSetting.sigen_onoff);
    UBOOT_DEBUG("mic num                = %d\n",gstMicSetting.mic_number);
    UBOOT_DEBUG("mic gain               = %d\n",gstMicSetting.mic_gain);
    UBOOT_DEBUG("mic bitwidth           = %d\n",gstMicSetting.mic_bitwidth);
    UBOOT_DEBUG("sw AEC mode            = %d\n",gstMicSetting.sw_aec_mode);
    UBOOT_DEBUG("HW AEC on/off          = %d\n",gstMicSetting.hw_aec_onoff );
    UBOOT_DEBUG("I2S on/off             = %d\n",gstMicSetting.i2s_onoff);
    UBOOT_DEBUG("DA on/off              = %d\n",gstMicSetting.da_onoff);
    UBOOT_DEBUG("HPF on/off             = %d\n",gstMicSetting.hpf_onoff);
    UBOOT_DEBUG("HPF config             = %d\n",gstMicSetting.hpf_config);
    UBOOT_DEBUG("UART on/off            = %d\n",gstMicSetting.uart_onoff);

    UBOOT_TRACE("OK\n");
    return 0;
}
int Read_MicParaFromFlash(stMic_config* stMicConfig_para)
{
    int ret = -1;
    UBOOT_TRACE("IN\n");
    ret = Read_FlashDataToAddr((U32)stMicConfig_para, E_DB_MIC_CONFIG);

    UBOOT_DEBUG("Voice on/off           = %d\n",stMicConfig_para->voice_onoff);
    UBOOT_DEBUG("Keyword Wake Up on/off = %d\n",stMicConfig_para->keyword_onoff);
    UBOOT_DEBUG("seamless on/off        = %d\n",stMicConfig_para->seamless_onoff);
    UBOOT_DEBUG("sigen                  = %d\n",stMicConfig_para->sigen_onoff);
    UBOOT_DEBUG("mic num                = %d\n",stMicConfig_para->mic_number);
    UBOOT_DEBUG("mic gain               = %d\n",stMicConfig_para->mic_gain);
    UBOOT_DEBUG("mic bitwidth           = %d\n",stMicConfig_para->mic_bitwidth);
    UBOOT_DEBUG("sw AEC mode            = %d\n",stMicConfig_para->sw_aec_mode);
    UBOOT_DEBUG("HW AEC on/off          = %d\n",stMicConfig_para->hw_aec_onoff );
    UBOOT_DEBUG("I2S on/off             = %d\n",stMicConfig_para->i2s_onoff);
    UBOOT_DEBUG("DA on/off              = %d\n",stMicConfig_para->da_onoff);
    UBOOT_DEBUG("HPF on/off             = %d\n",stMicConfig_para->hpf_onoff);
    UBOOT_DEBUG("HPF config             = %d\n",stMicConfig_para->hpf_config);
    UBOOT_DEBUG("UART on/off            = %d\n",stMicConfig_para->uart_onoff);

    UBOOT_TRACE("OK\n");
    return ret;
}
int Load_MicPara_ToFlash(U32 u32DbtableOffset)
{
    int ret = -1;
    UBOOT_TRACE("IN\n");
    UBOOT_DEBUG("Mic para size =: 0x%08x \n",sizeof(stMic_config));
    UBOOT_DEBUG("gstDbtable.dbdata[E_DB_MIC_CONFIG].Offset =: 0x%x\n",gstDbtable.dbdata[E_DB_MIC_CONFIG].Offset);
    ret = Load_DataToDbBuffer(u32DbtableOffset, E_DB_MIC_CONFIG, (U32)&gstMicSetting);
    UBOOT_TRACE("OK\n");
    return ret;
}
#endif
int parse_hdmitx_setting(char *path)
{
    U32 filesize = 0;
    char str[BUFFER_SIZE]="\0";
    char *script=NULL;
    #define HDMITX_SECTION_NAME "Hdmitx"

    UBOOT_TRACE("IN\n");
    memset(&stHdmitxSetting,0,sizeof(st_hdmitx_setting));

    // Set initial value from mboot
    stHdmitxSetting.enLoadDefaultFromMboot = 1;

    script = loadscript(path,&filesize);
    if(script == NULL)
    {
#if defined(CONFIG_DATA_SEPARATION)
        while (*path== '/') path++;
        path=strstr(path,"/");
        UBOOT_INFO("filter /cusdata or /config, and find file again!!\n");
        script = loadscript(path,&filesize);
        if(script == NULL)
        {
            return -1;
        }
#else
        return -1;
#endif
    }

    Profile_Init(script,filesize);
    if(Profile_GetString(HDMITX_SECTION_NAME, "enInColorFmt", "", str, sizearray(str)))
    {
        stHdmitxSetting.enInColorFmt=simple_strtoul(str,NULL,16);
        // if there are hdmi setting values in customer ini
        stHdmitxSetting.enLoadDefaultFromMboot = 0;
    }
    if(Profile_GetString(HDMITX_SECTION_NAME, "enOutColorFmt", "", str, sizearray(str)))
    {
        stHdmitxSetting.enOutColorFmt=simple_strtoul(str,NULL,16);
    }
    if(Profile_GetString(HDMITX_SECTION_NAME, "enInColorQuantRange", "", str, sizearray(str)))
    {
        stHdmitxSetting.enInColorQuantRange=simple_strtoul(str,NULL,16);
    }
    if(Profile_GetString(HDMITX_SECTION_NAME, "enOutColorQuantRange", "", str, sizearray(str)))
    {
        stHdmitxSetting.enOutColorQuantRange=simple_strtoul(str,NULL,16);
    }
    if(Profile_GetString(HDMITX_SECTION_NAME, "enOutputMode", "", str, sizearray(str)))
    {
        stHdmitxSetting.enOutputMode=simple_strtoul(str,NULL,16);

    }
    if(Profile_GetString(HDMITX_SECTION_NAME, "enColorDepth", "", str, sizearray(str)))
    {
        stHdmitxSetting.enColorDepth=simple_strtoul(str,NULL,16);

    }
    UBOOT_DEBUG("stHdmitxSetting.enInColorFmt         = %d\n", stHdmitxSetting.enInColorFmt);
    UBOOT_DEBUG("stHdmitxSetting.enOutColorFmt        = %d\n", stHdmitxSetting.enOutColorFmt);
    UBOOT_DEBUG("stHdmitxSetting.enInColorQuantRange  = %d\n", stHdmitxSetting.enInColorQuantRange);
    UBOOT_DEBUG("stHdmitxSetting.enOutColorQuantRange = %d\n", stHdmitxSetting.enOutColorQuantRange);
    UBOOT_DEBUG("stHdmitxSetting.enOutputMode         = %d\n", stHdmitxSetting.enOutputMode);
    UBOOT_DEBUG("stHdmitxSetting.enColorDepth         = %d\n", stHdmitxSetting.enColorDepth);
    UBOOT_DEBUG("stHdmitxSetting.enLoadDefaultFromMboot = %d\n", stHdmitxSetting.enLoadDefaultFromMboot);

    UBOOT_TRACE("OK\n");
    return 0;
}

#if defined(CONFIG_DATA_SEPARATION)
int parse_project_id_ini_cusdata(char* path,char* projectID)
{
    int ret = -1;
    char *script = NULL;
    char str_id[gProjectID_LEN];
    char str_name[BUFFER_SIZE];
    char *s;
    U32 filesize = 0;
    UBOOT_TRACE("IN\n");

    script = loadscript(path,&filesize);
    if(script == NULL)
    {
        goto exit;
    }
    UBOOT_INFO("sizeof( sys ini cusdata script ) --------> %u bytes\n",filesize);

    Profile_Init((char *)script,filesize);

    s = getenv (gEnvProjectID);
    if(s)
    {
        snprintf(projectID, strlen(s), "%s",s);
        UBOOT_INFO("old projectID --> %s\n",projectID);
    }

#if defined(CONFIG_CUS_PROJECT_ID_EEPROM) && (CONFIG_CUS_PROJECT_ID_EEPROM == 1) \
         && defined(CONFIG_CMD_EEPROM) && (CONFIG_MSTARI2C == 1)
    UBOOT_INFO("projectID from EEPROM!\n");
         // select project_id from eeprom
    if(0 <= Load_PROJECT_ID_FROM_EEPROM(str_id, sizearray(str_id)))
#else
    // select project_id
    if(Profile_GetString(gModelName, gProjectID, "", str_id, sizearray(str_id)))
#endif
    {
        snprintf(projectID, gProjectID_LEN, "%s",str_id);
        UBOOT_INFO("projectID --> %s\n",projectID);
        setenv(gEnvProjectID,projectID);

        char projectID_tmp[BUFFER_SIZE] = "\0";
        snprintf(projectID_tmp,BUFFER_SIZE,"%s=%s",gKLProjectID,projectID);
        // set projectid to bootargs
        if(0 == set_bootargs_cfg(gKLProjectID,projectID_tmp,0))
        {
            UBOOT_INFO("set %s to bootargs OK\n",projectID_tmp);
        }
        else
        {
            UBOOT_ERROR("set %s to bootargs fail\n",projectID_tmp);
        }
    }
    else
    {
        goto exit;
    }
    ret = 0;

exit:
    if(script)
        free(script);
    UBOOT_TRACE("OK\n");
    return ret;
}

int parse_data_ini_cusdata(char* datapath, char* PnlPath, char* KeypadPath)
{
    int ret = -1;
    char *script = NULL;
    char str[BUFFER_SIZE];
    char *str_tmp;
    U32 filesize = 0;
    int n=0;
    UBOOT_TRACE("IN\n");

    script = loadscript(datapath,&filesize);
    if(script == NULL)
    {
        goto exit;
    }
    UBOOT_INFO("sizeof( data ini script ) --------> %u bytes\n",filesize);
    Profile_Init((char *)script,filesize);

    memset(str,0,sizeof(str));
    if(Profile_GetString("panel", "m_pPanelName","", str, sizearray(str)))
    {
        UBOOT_INFO("Half panelConf.ini path --> %s\n",str);
        snprintf(PnlPath,BUFFER_SIZE,"%s",str);
        UBOOT_INFO("Full panelConf.ini  path --> %s\n",PnlPath);
    }
    else
    {
        goto exit;
    }

    memset(str,0,sizeof(str));
    if(Profile_GetString("Keypad", "m_pKeypad_File","", str, sizearray(str)))
    {
        UBOOT_INFO("Half keypadConf.ini path --> %s\n",str);
        snprintf(KeypadPath,BUFFER_SIZE,"%s",str);
        UBOOT_INFO("Full keypadConf.ini  path --> %s\n",KeypadPath);
    }
    else
    {
        goto exit;
    }
#if (CONFIG_DYNAMIC_IR)
    memset(str,0,sizeof(str));
    if(Profile_GetString("ir", "m_pIrConfig_File","", str, sizearray(str)))
    {
        UBOOT_INFO("Full irConf.ini path --> %s\n",str);
        str_tmp = str;
        if(FALSE==vfs_getsize(str_tmp))
        {
            while (*str_tmp== '/') str_tmp++;
            str_tmp=strstr(str_tmp,"/");
            UBOOT_INFO("filter /cusdata, and find file again!!\n");

            if(FALSE==vfs_getsize(str_tmp))
            {
                str_tmp=str;
            }
        }
        snprintf(gIRConfigPath,BUFFER_SIZE,"%s",str_tmp);
        UBOOT_INFO("Half irConf.ini  path --> %s\n",gIRConfigPath);
    }
    else
    {
        goto exit;
    }
#endif
    memset(str,0,sizeof(str));
    if(Profile_GetString("MISC_MIRROR_CFG", "MIRROR_OSD","", str, sizearray(str)))
    {
        UBOOT_INFO("MIRROR_OSD is  --> %s\n",str);
        if((strcmp(str, "True") == 0)||(strcmp(str, "1") == 0))
        {
            // MIRROR_OSD_TYPE   # 0:normal type.  1:Horizontal-mirror only.  2:Vertical-mirror only.  3:HV-mirror.
            if(Profile_GetString("MISC_MIRROR_CFG", "MIRROR_OSD_TYPE","", str, sizearray(str)))
            {
                UBOOT_INFO("MIRROR_OSD_TYPE is  --> %s\n",str);
                if((strcmp(str, "1") == 0))
                    gstSysMiscSetting.m_u8MirrorMode = 3;
                else if((strcmp(str, "2") == 0))
                    gstSysMiscSetting.m_u8MirrorMode = 2;
                else
                    gstSysMiscSetting.m_u8MirrorMode = 1;
            }
            else
            {
                gstSysMiscSetting.m_u8MirrorMode = 1;
            }
        }
        else if((strcmp(str, "False") == 0)||(strcmp(str, "0") == 0))
        {
            gstSysMiscSetting.m_u8MirrorMode = 0;
        }
        else
        {

            UBOOT_INFO("MIRROR_OSD is  [True] or [False], other value can not be parsed\n");
        }
    }
    else
    {
        goto exit;
    }
    n = Profile_GetInteger("MISC_MIRROR_CFG", "MIRROR_OUTPUT_CONTROL", -1);
    UBOOT_INFO("MIRROR_OUTPUT_CONTROL is  --> %d\n",n);
    if (n >= 0)
    {
        gstSysMiscSetting.m_u8MOD_H_MirrorMode = n;
    }
    else
    {
        gstSysMiscSetting.m_u8MOD_H_MirrorMode = 0;
    }

    #if (CONFIG_URSA_UNION == 1 || CONFIG_TV_CHIP==1)
    if(Profile_GetString("module", "m_pModuleName","", str, sizearray(str)))
    {
        UBOOT_INFO("Half Customer_Module.ini path --> %s\n",str);
        snprintf(gModulePath,BUFFER_SIZE,"%s",str);
        UBOOT_INFO("Full Customer_Module.ini  path --> %s\n",gModulePath);
    }
    #endif

    ret = 0;
    memset(str,0,sizeof(str));
    if(Profile_GetString("TCON_BIN", "TCON_FILE","", str, sizearray(str)))
    {
        char* tmpTconBinPath;
        tmpTconBinPath=str;
        UBOOT_INFO("Full TCON_BIN path --> %s\n",str);
        if(FALSE==vfs_getsize(tmpTconBinPath))
        {
            while (*tmpTconBinPath== '/') tmpTconBinPath++;
            tmpTconBinPath=strstr(tmpTconBinPath,"/");
            UBOOT_INFO("filter /cusdata or /config, and find file again!!\n");

            if(FALSE==vfs_getsize(tmpTconBinPath))
            {
                tmpTconBinPath=str;
            }
        }
        snprintf(gTconBinPath,BUFFER_SIZE,"%s",tmpTconBinPath);
        UBOOT_INFO("TCON path --> %s\n",gTconBinPath);
        ret = 0;
    }
    memset(str,0,sizeof(str));
    if(Profile_GetString("MISC_MIRROR_CFG", "MIRROR_VIDEO","", str, sizearray(str)))
    {
        UBOOT_INFO("MIRROR_VIDEO -> [%s]\n",str);
        if((strcmp(str, "True") == 0)||(strcmp(str, "1") == 0))
        {
            gstSysMiscSetting.m_u8VideoMirrorMode = 1;
        }
        else if((strcmp(str, "False") == 0)||(strcmp(str, "0") == 0))
        {
            gstSysMiscSetting.m_u8VideoMirrorMode = 0;
        }
        else
        {
            UBOOT_INFO("MIRROR_VIDEO is  [True] or [False], other value can not be parsed\n");
        }
    }
    else
    {
        gstSysMiscSetting.m_u8VideoMirrorMode = 0;
    }

    memset(str,0,sizeof(str));
    if(Profile_GetString("GAMMA_BIN", "PANEL_GAMMA_FILE","", str, sizearray(str)))
    {
        char* tmpPnlGammaBinPath = str;
        UBOOT_INFO("GAMMA_BIN -> [%s]\n",str);
        if(FALSE==vfs_getsize(tmpPnlGammaBinPath))
        {
            while (*tmpPnlGammaBinPath== '/') tmpPnlGammaBinPath++;
            tmpPnlGammaBinPath=strstr(tmpPnlGammaBinPath,"/");
            UBOOT_INFO("filter /cusdata or /config, and find file again!!\n");

            if(FALSE == vfs_getsize(tmpPnlGammaBinPath))
            {
                tmpPnlGammaBinPath = str;
            }
        }
        UBOOT_INFO("Half gamma.bin path --> %s\n",tmpPnlGammaBinPath);
        snprintf(gRGBWBinPath,BUFFER_SIZE,"%s",tmpPnlGammaBinPath);
        UBOOT_INFO("Full gamma.bin  path --> %s\n",gRGBWBinPath);
    }

    memset(str,0,sizeof(str));
    if(Profile_GetString("LUT_BIN", "3DLUT_BIN_FILE","", str, sizearray(str)))
    {
        char* tmp3dLutBinPath = str;
        UBOOT_INFO("LUT_BIN -> [%s]\n",str);
        if(FALSE==vfs_getsize(tmp3dLutBinPath))
        {
            while (*tmp3dLutBinPath== '/') tmp3dLutBinPath++;
            tmp3dLutBinPath=strstr(tmp3dLutBinPath,"/");
            UBOOT_INFO("filter /cusdata or /config, and find file again!!\n");

            if(FALSE == vfs_getsize(tmp3dLutBinPath))
            {
                tmp3dLutBinPath = str;
            }
        }
        UBOOT_INFO("Half 3DLUT.bin path --> %s\n",tmp3dLutBinPath);
        snprintf(g3DLUTBinPath,BUFFER_SIZE,"%s",tmp3dLutBinPath);
        UBOOT_INFO("Full 3DLUT.bin  path --> %s\n",g3DLUTBinPath);
    }

exit:
    if(script)
        free(script);
    UBOOT_TRACE("OK\n");
    return ret;
}

int parse_keypad_ini_cusdata(char* path, st_KeypadWakeup_t powerKeyConfig, int length)
{
    int ret = -1;
    char *script = NULL;
    U32 filesize = 0;
    char powerKey[POWER_KEY_BUFFER_LENGTH] = {0};
    int i;
    char * pEnv=NULL;
    UBOOT_TRACE("IN\n");

    pEnv=getenv(ENV_CUSDATA);
    if (pEnv !=NULL)
    {
        UBOOT_DEBUG("Get MAP ENV_CUSDATA from env =%s \n",pEnv);
        if(vfs_mount(pEnv)!=0)
        {
            UBOOT_ERROR("mount %s fail\n",pEnv);
            goto exit;
        }
    }
    else
    {
        if(vfs_mount(CUSDATA)!=0)  //use default PATH /cusdata
        {
            UBOOT_ERROR("mount %s fail\n",CONFIG);
            goto exit;
        }
    }

    while (*path== '/') path++;
    path=strstr(path,"/");
    UBOOT_INFO("filter /cusdata or /config, and find file again!!\n");
    script = loadscript(path,&filesize);
    if(script == NULL)
    {
        goto exit;
    }
    UBOOT_INFO("sizeof( keypad ini script ) --------> %u bytes\n",filesize);
    Profile_Init((char *)script,filesize);
    for(i = 0; i < length; i++)
    {
        memset(powerKey, 0, POWER_KEY_BUFFER_LENGTH);
        snprintf(powerKey, POWER_KEY_BUFFER_LENGTH, "WAKEUP_KEY_%d", i);
        powerKeyConfig[i] = (char)Profile_GetInteger("WAKEUP_KEY", powerKey, 0);
    }
    ret = 0;
exit:
    if(script)
        free(script);
    UBOOT_TRACE("OK\n");
    return ret;
}

int Load_KeypadWakeup_ToFlash(U32 u32DbtableOffset)
{
    int ret = -1;
    st_KeypadWakeup_t wakeup_info;
    char * pEnv=NULL;
    UBOOT_TRACE("IN\n");
    memset(&wakeup_info,0,sizeof(wakeup_info));

    pEnv=getenv(ENV_CUSDATA);
    if (pEnv !=NULL)
    {
        UBOOT_DEBUG("Get MAP ENV_CUSDATA from env =%s \n",pEnv);
        if(vfs_mount(pEnv)!=0)
        {
            UBOOT_ERROR("mount %s fail\n",pEnv);
            goto exit;
        }
    }
    else
    {
        if(vfs_mount(CUSDATA)!=0)  //use default PATH /cusdata
        {
            UBOOT_ERROR("mount %s fail\n",CONFIG);
            goto exit;
        }
    }

    if(parse_keypad_ini_cusdata(gKeypadPath, &wakeup_info, KEYPAD_MAX) != 0)
    {
        UBOOT_ERROR("parse_keypad_ini_cusdata fail !!\n");
        return ret;
    }
    UBOOT_DEBUG("wakeup_info size =: 0x%08x \n",sizeof(wakeup_info));
    ret = Load_DataToDbBuffer(u32DbtableOffset, E_DB_KEYPAD_WAKEUP, (U32)&wakeup_info);

exit:
    UBOOT_TRACE("OK\n");
    return ret;

}

int Read_KeypadWakeup_FromFlash(st_KeypadWakeup_t * wakeup_info)
{
    int ret = -1;
    unsigned char i = 0;
    ret = Read_FlashDataToAddr((U32)wakeup_info, E_DB_KEYPAD_WAKEUP);
    for(i =0;i<KEYPAD_MAX;i++)
    {
        UBOOT_DEBUG("=====================================\n");
        UBOOT_DEBUG("KEYPAD_WAKEUP_%d :\n",i);
        UBOOT_DEBUG("u16Key = 0x%x\n",(*wakeup_info)[i]);
    }
    return ret;

}

#if defined(CONFIG_CUS_PROJECT_ID_EEPROM) && (CONFIG_CUS_PROJECT_ID_EEPROM == 1)
#if defined(CONFIG_CMD_EEPROM) && (CONFIG_MSTARI2C == 1)
int Load_PROJECT_ID_FROM_EEPROM(char *Buffer, int BufferSize)
{
    UBOOT_TRACE("IN\n");
    int ret = -1;
    char Pathbuff [BUFFER_SIZE]="\0";
    char addressbuff[BUFFER_SIZE]="\0";
    MS_U8 productionID[gProjectID_LEN];
    char *script = NULL;
    U32 filesize = 0;
    if (Buffer == NULL || BufferSize <= 0)
    {
        return ret;
    }
    vfs_mount(CONFIG);
    snprintf(Pathbuff,sizeof(Pathbuff),"%s/eeprom/eeprom_key_setting_cus.ini",CONFIG_PATH);
    script = loadscript(Pathbuff,&filesize);
    if(script == NULL)
    {
        UBOOT_INFO("eeprom_key_setting_cus.ini not exist, please check it again!\n");
        return ret;
    }

    UBOOT_INFO("sizeof( eeprom_key_setting_cus.ini ini script ) --------> %u bytes\n",filesize);
    Profile_Init((char *)script,filesize);
    if(Profile_GetString(PROJECT_ID, PROJECT_ID_ADDRESS,"", addressbuff, sizearray(addressbuff)))
    {
        char buff2[BUFFER_SIZE]="\0";
        MS_U8 ProjectIDindex;
        sscanf(addressbuff, "0X%s", &buff2);
        MS_U16 buff3=strtol(buff2,NULL,16);
        for(ProjectIDindex = 0;ProjectIDindex< gProjectID_LEN-1 ; ProjectIDindex++)
        {
            productionID[ProjectIDindex] = eeprom_ReadByte(buff3+ProjectIDindex);
        }

        //check eeprom data validate(0~9)
        for(ProjectIDindex = 0;ProjectIDindex < gProjectID_LEN-1 ; ProjectIDindex++)
        {
             if(productionID[ProjectIDindex] > 0x09)
             {
                 UBOOT_INFO("productionID[%d] = %X is not correct!  eeprom address =%X \n",ProjectIDindex,productionID[ProjectIDindex],buff3+ProjectIDindex);
                 ret = -1;
                 break;
             }
             ret = 1;
        }

    }
    else
    {
        UBOOT_INFO("read eepromID from eeprom_key_setting_cus.ini  fail...\n");
        ret = -1;
    }

    if(1 == ret)
    {
        MS_U8 ProjectIDindex;
        MS_U8 Bufferindex = 0;

        //remove '0' in front of productionID
        for(ProjectIDindex = 0;ProjectIDindex < gProjectID_LEN-1 ; ProjectIDindex++)
        {
            if(0x00 != productionID[ProjectIDindex])
            {
                break;
            }
        }

        if((gProjectID_LEN-1) == ProjectIDindex)
        {
            //prevent case ID = 0
            UBOOT_INFO("productionID in eeprom is 0 ....  use default as  1, please check it!\n");
            snprintf(Buffer,BufferSize,"%s","1");
        }
        else
        {
            for(;ProjectIDindex < gProjectID_LEN-1 ; ProjectIDindex++)
            {
                sprintf(Buffer+Bufferindex,"%u",productionID[ProjectIDindex]);
                Bufferindex++;
            }
            Buffer[gProjectID_LEN] = '\n';
        }

    }
    else
    {
        UBOOT_INFO("productionID in eeprom not correct,  use default as 1, please check it!\n");
        snprintf(Buffer,BufferSize,"%s","1");
        ret = 1;
    }
    UBOOT_TRACE("OK\n");
    return ret;
}
#endif
#endif
#endif

int check_ini_exist(char * ModelPath)
{
    int ret = -1;
    char *script = NULL;
    U32 filesize = 0;

    UBOOT_INFO("Check ini exist...\n");
    /* uboot should use /config/XXX instead of /tvconfig/config/XXX due to there
           is no /tvconfig path in tvconfig.img */
    script = loadscript(ModelPath,&filesize);
    if(NULL == script)
    {
        char* FilterModelPath = ModelPath;
        FilterModelPath++;    // skip first "/" character
        FilterModelPath=strstr(FilterModelPath,"/");
        UBOOT_INFO("filter %s, and find file again!!\n", FilterModelPath);
        script = loadscript(FilterModelPath,&filesize);
        if(NULL == script)
        {
            return ret;
        }
    }

    return 0;
}

int parse_sys_ini(char* path,char* ModelPath, int BufferLen)
{
    int ret = -1;
    char *script = NULL;
    char str[BUFFER_SIZE];
    char platforminfo[BUFFER_SIZE]="\0";
    char *s;
    char *OdmModelPath;
    U32 filesize = 0;
    UBOOT_TRACE("IN\n");

    script = loadscript(path,&filesize);
    if(script == NULL)
    {
        goto exit;
    }
    UBOOT_INFO("sizeof( sys ini script ) --------> %u bytes\n",filesize);

    Profile_Init((char *)script,filesize);
    if(Profile_GetString("platform", "gPlatform", "", platforminfo, sizearray(platforminfo)))
    {
        if(0 == set_bootargs_cfg("platform",platforminfo,0))
        {
            UBOOT_INFO("set %s to bootargs OK\n",platforminfo);
        }
        else
        {
            UBOOT_ERROR("set %s to bootargs fail\n",platforminfo);
        }
    }
    else
    {
        UBOOT_INFO("Can't get platform info from %s\n", path);
    }

    Profile_Init((char *)script,filesize);

#ifdef UFBL_FEATURE_IDME
    //1. Get model_name from idme
    if (!idme_get_var_external("model_name", ModelPath, BufferLen-1))
    {
        if(strlen(ModelPath) > 0)
        {
            UBOOT_INFO("idme model_name value --> %s\n",ModelPath);

            if (bOdmModeEnable == 1) {
                vfs_mount(ODM);

                OdmModelPath = ModelPath;
                while (*OdmModelPath == '/') OdmModelPath++;
                OdmModelPath = strstr(OdmModelPath, "/");
                ModelPath = OdmModelPath;

            }

            ret = check_ini_exist(ModelPath);

            if(0 == ret)
            {
                UBOOT_INFO("Full model.ini  path --> %s\n",ModelPath);
            }
            vfs_mount(CONFIG);
        }
    }
#endif
    //2. get customer_ini_path from MBoot ENV
    if(0 != ret)
    {
        s = getenv ("customer_ini_path");
        if(s)
        {
            UBOOT_INFO("get customer_ini_path from env value --> %s\n",s);
            snprintf(ModelPath, BufferLen, "%s",s);
            ret = check_ini_exist(ModelPath);
            if(0 == ret)
            {
                UBOOT_INFO("Full model.ini  path --> %s\n",ModelPath);
            }
        }
    }
    //3. get customer_ini_path from sys.ini
    if(0 != ret)
    {
        if(Profile_GetString("model", "gModelName", "", str, sizearray(str)))
        {
            UBOOT_INFO("Model:gModelName in sys.ini --> %s\n",str);
            snprintf(ModelPath, BufferLen, "%s",str);
            ret = check_ini_exist(ModelPath);
            if(0 == ret)
            {
                UBOOT_INFO("Full model.ini  path --> %s\n",ModelPath);
            }
        }
        else
        {
            //4. use default path for customer_ini_path
            snprintf(ModelPath, BufferLen, "%s",DEFAULT_MODEL_NAME);
            UBOOT_INFO("customer_ini_path uses default value --> %s\n", ModelPath);
            ret = check_ini_exist(ModelPath);
            if(0 == ret)
            {
                UBOOT_INFO("Full model.ini  path --> %s\n",ModelPath);
            }
        }
    }
    /* /tvconfig/config is no longer symbol-linked to /config in Android N
     * manually append /tvonfig as prefix to model_name to make old device work.
     */
    if ((strncmp(ModelPath, "/config", 7) == 0) && (bOdmModeEnable == 0)) {
        char full_path[BUFFER_SIZE] = {0};
        snprintf(full_path, BUFFER_SIZE, "%s%s", "/tvconfig", ModelPath);
        strncpy(ModelPath, full_path, BufferLen);
        setenv("customer_ini_path",ModelPath);
        UBOOT_INFO("set customer_ini_path env value --> %s\n",ModelPath);
    } else if (bOdmModeEnable == 1) {
        char full_path[BUFFER_SIZE] = {0};
        snprintf(full_path, BUFFER_SIZE, "%s%s", "/firetv_odm", ModelPath);
        setenv("customer_ini_path", full_path);
        UBOOT_INFO("set customer_ini_path env value --> %s\n", full_path);
    }

    if(0 != ret)
    {
        U8 isFindModel = 0;
        s = getenv("model_ID");
        if(s)
        {
            char model_id[BUFFER_SIZE];
            char id[BUFFER_SIZE];
            char g_modelName[BUFFER_SIZE];
            char tmpModelName[BUFFER_SIZE];
            int i = 1;

            memset(model_id,0,BUFFER_SIZE);
            memset(id,0,BUFFER_SIZE);
            memset(g_modelName,0,BUFFER_SIZE);
            memset(tmpModelName,0,BUFFER_SIZE);
            snprintf(model_id, BUFFER_SIZE, "model_%02d",i);
            snprintf(id, BUFFER_SIZE, "id_%02d",i);
            snprintf(g_modelName, BUFFER_SIZE, "gModelName_%02d",i);
            UBOOT_DEBUG("[model_id] = %s, [g_modelName] = %s, [id] = %s\n", model_id,g_modelName, id);

            while(Profile_GetString(model_id, g_modelName,"", str, sizearray(str)))
            {
                snprintf(tmpModelName, BUFFER_SIZE, "%s", str);
                UBOOT_DEBUG("[tmpModelNmae] = %s\n", tmpModelName);
                //parsing id
                if(Profile_GetString(model_id, id,"", str, sizearray(str)))
                {
                    UBOOT_INFO("[%s] = %s\n", id,str);
                    if(strncmp(str, getenv("model_ID"), strlen(str))==0)
                    {
                        UBOOT_INFO("Half model.ini path --> %s\n",tmpModelName);
                        snprintf(ModelPath, BufferLen, "%s",tmpModelName);
                        UBOOT_INFO("Full model.ini  path --> %s\n",ModelPath);
                        setenv("customer_ini_path",ModelPath);
                        isFindModel = 1;
                        ret = 0;
                        break;
                    }
                }
                i++;
                memset(model_id,0,BUFFER_SIZE);
                memset(id,0,BUFFER_SIZE);
                memset(g_modelName,0,BUFFER_SIZE);
                snprintf(model_id, BUFFER_SIZE, "model_%02d",i);
                snprintf(id, BUFFER_SIZE, "id_%02d",i);
                snprintf(g_modelName, BUFFER_SIZE, "gModelName_%02d",i);
                UBOOT_DEBUG("[model_id] = %s, [g_modelName] = %s, [id] = %s\n", model_id,g_modelName, id);
            }

        }
        if(isFindModel==0)
        {
            if(s)
            {
                UBOOT_ERROR("can't find [id] = %s\n", getenv("model_ID"));
            }

            if(Profile_GetString("model", "gModelName", "", str, sizearray(str)))
            {
                UBOOT_INFO("Half model.ini path --> %s\n",str);
                snprintf(ModelPath, BufferLen, "%s",str);
                UBOOT_INFO("Full model.ini  path --> %s\n",ModelPath);
                ret = 0;
            }
            setenv("customer_ini_path",ModelPath);
        }
    }
    parse_hdmitx_setting(getenv("customer_ini_path"));
exit:
    if(script)
        free(script);
    UBOOT_TRACE("OK\n");
    return ret;
}

int parse_dfb_ini(char* path, st_gopmux_para *gopmux_para)
{
    int ret = 0;
    char *script = NULL;
    char str[BUFFER_SIZE];
    U32 filesize = 0;
    UBOOT_TRACE("IN\n");
    long n;

    script = loadscript(path,&filesize);
    if(script == NULL)
    {
        ret = -1;
        goto exit;
    }
    UBOOT_INFO("sizeof( dfb ini script ) --------> %u bytes\n",filesize);
    Profile_Init((char *)script,filesize);

    n = Profile_GetInteger("DirectFBRC", "DFBRC_LAYERCOUNTS", 0);
    gopmux_para->m_u16SettingCounts = n;
    UBOOT_DEBUG("gopmux_para->m_u16SettingCounts:%d\n", gopmux_para->m_u16SettingCounts);
    if (gopmux_para->m_u16SettingCounts > 0)
    {
        gopmux_para->m_u8SettingType = 1;

        n = Profile_GetInteger("DirectFBRC", "DFBRC_LAYER0_GOPINDEX", 0);
        gopmux_para->m_u16Setting0GopIndex= n;
        UBOOT_DEBUG("gopmux_para->m_u16Setting0GopIndex:%d\n", gopmux_para->m_u16Setting0GopIndex);

        n = Profile_GetInteger("DirectFBRC", "DFBRC_LAYER1_GOPINDEX", 0);
        gopmux_para->m_u16Setting1GopIndex = n;
        UBOOT_DEBUG("gopmux_para->m_u16Setting1GopIndex:%d\n", gopmux_para->m_u16Setting1GopIndex);

        n = Profile_GetInteger("DirectFBRC", "DFBRC_LAYER2_GOPINDEX", 0);
        gopmux_para->m_u16Setting2GopIndex = n;
        UBOOT_DEBUG("gopmux_para->m_u16Setting2GopIndex:%d\n", gopmux_para->m_u16Setting2GopIndex);

        n = Profile_GetInteger("DirectFBRC", "DFBRC_LAYER3_GOPINDEX", 0);
        gopmux_para->m_u16Setting3GopIndex = n;
        UBOOT_DEBUG("gopmux_para->m_u16Setting3GopIndex:%d\n", gopmux_para->m_u16Setting3GopIndex);

        n = Profile_GetInteger("DirectFBRC", "DFBRC_LAYER4_GOPINDEX", 0);
        gopmux_para->m_u16Setting4GopIndex = n;
        UBOOT_DEBUG("gopmux_para->m_u16Setting4GopIndex:%d\n", gopmux_para->m_u16Setting4GopIndex);
    }
    else
    {
        gopmux_para->m_u8SettingType = 0;

        n = Profile_GetInteger("DirectFBRC", "DFBRC_MUXCOUNTS", 0);
        gopmux_para->m_u16SettingCounts= n;
        UBOOT_DEBUG("gopmux_para->m_u16SettingCounts:%d\n", gopmux_para->m_u16SettingCounts);

        n = Profile_GetInteger("DirectFBRC", "DFBRC_MUX0_GOPINDEX", 0);
        gopmux_para->m_u16Setting0GopIndex= n;
        UBOOT_DEBUG("gopmux_para->m_u16Setting0GopIndex:%d\n", gopmux_para->m_u16Setting0GopIndex);

        n = Profile_GetInteger("DirectFBRC", "DFBRC_MUX1_GOPINDEX", 0);
        gopmux_para->m_u16Setting1GopIndex = n;
        UBOOT_DEBUG("gopmux_para->m_u16Setting1GopIndex:%d\n", gopmux_para->m_u16Setting1GopIndex);

        n = Profile_GetInteger("DirectFBRC", "DFBRC_MUX2_GOPINDEX", 0);
        gopmux_para->m_u16Setting2GopIndex = n;
        UBOOT_DEBUG("gopmux_para->m_u16Setting2GopIndex:%d\n", gopmux_para->m_u16Setting2GopIndex);

        n = Profile_GetInteger("DirectFBRC", "DFBRC_MUX3_GOPINDEX", 0);
        gopmux_para->m_u16Setting3GopIndex = n;
        UBOOT_DEBUG("gopmux_para->m_u16Setting3GopIndex:%d\n", gopmux_para->m_u16Setting3GopIndex);

        n = Profile_GetInteger("DirectFBRC", "DFBRC_MUX4_GOPINDEX", 0);
        gopmux_para->m_u16Setting4GopIndex = n;
        UBOOT_DEBUG("gopmux_para->m_u16Setting4GopIndex:%d\n", gopmux_para->m_u16Setting4GopIndex);

    }
    const U8 IP0 = 0;
    U8 gopidx = 0;
    U8 layerid = 0;
    U8 layercount = 0;
    char layername[BUFFER_SIZE];

    layercount = Profile_GetInteger("DirectFBRC", "DFBRC_MST_GOP_COUNTS", -1);
    memset(layername,0 ,sizeof(layername));

    for (layerid = 0; layerid < layercount; layerid++)
    {
        snprintf(layername, sizeof(layername), "DFBRC_MST_GOP_DSTPLANE_%d", layerid);
        if (Profile_GetInteger("DirectFBRC", layername, -1) == IP0)
        {
            snprintf(layername, sizeof(layername), "DFBRC_MST_GOP_AVAILABLE_%d", layerid);
            gopidx = Profile_GetInteger("DirectFBRC", layername, -1);

            memset(str,0 ,sizeof(str));
            snprintf(str,sizeof(str),"%u",gopidx);
            gstSysMiscSetting.m_u8LogoGopIdx = gopidx;
            UBOOT_INFO("logo -> [%s]\n",str);

            ret = 0;
            goto exit;
        }
    }

exit:
    if(script)
        free(script);
    UBOOT_TRACE("OK\n");
    return ret;
}

int Str2U8Array(char *str,unsigned char *pu8Array)
{
    unsigned char u8Number =0;
    char puTmp[16]={0};

    UBOOT_DEBUG("spilitting string : %s\n",str);
    while ((str=strstr(str,"0x"))!=NULL)
    {
        memcpy(puTmp,str,sizeof(puTmp)-1);
        u8Number=simple_strtoul(puTmp,NULL,16);
        //UBOOT_INFO("0x%x ",(unsigned int)u8Number);
        *(pu8Array++)=u8Number;
        str+=2;
    }
    UBOOT_INFO("\n");
    return 0;
}

int Str2U16Array(char *str, MS_U16 *pu16Array)
{
    MS_U16 u16Number =0;
    char puTmp[7]={0};

    UBOOT_DEBUG("spilitting string : %s\n",str);
    while ((str=strstr(str,"0x"))!=NULL)
    {
        memcpy(puTmp,str,sizeof(puTmp)-1);
        u16Number=simple_strtoul(puTmp,NULL,16);
        //UBOOT_INFO("0x%x \n",(unsigned int)u16Number);
        *(pu16Array++)=u16Number;
        str+=2;
    }
    UBOOT_INFO("\n");
    return 0;
}

void _parse_model_vb1(void)
{
    #define VB1_SECTION_NAME "VB1_Connector"
    char str[BUFFER_SIZE]={0};
    UBOOT_TRACE("IN\n");

    // init stVB1Channel1order
    memset(&stVB1Channel1order,0,sizeof(stVB1Channel1order));

    // 16V_Order
    if(Profile_GetString(VB1_SECTION_NAME, "16V_Order","", str, sizearray(str)))
    {
        Str2U8Array(str,stVB1Channel1order.u8Vb116vOrder);
    }

    // 8V_Order
    if(Profile_GetString(VB1_SECTION_NAME, "8V_Order","", str, sizearray(str)))
    {
        Str2U8Array(str,stVB1Channel1order.u8Vb18vOrder);
    }

    // 4V_Order
    if(Profile_GetString(VB1_SECTION_NAME, "4V_Order","", str, sizearray(str)))
    {
        Str2U8Array(str,stVB1Channel1order.u8Vb14vOrder);
    }

    // 2V_Order
    if(Profile_GetString(VB1_SECTION_NAME, "2V_Order","", str, sizearray(str)))
    {
        Str2U8Array(str,stVB1Channel1order.u8Vb12vOrder);
    }
    // 1V_Order
    if(Profile_GetString(VB1_SECTION_NAME, "1V_Order","", str, sizearray(str)))
    {
        Str2U8Array(str,stVB1Channel1order.u8Vb11vOrder);
    }
    // 4O_Order
    if(Profile_GetString(VB1_SECTION_NAME, "4O_Order","", str, sizearray(str)))
    {
        Str2U8Array(str,stVB1Channel1order.u8Vb14OOrder);
    }

    // 2O_Order
    if(Profile_GetString(VB1_SECTION_NAME, "2O_Order","", str, sizearray(str)))
    {
        Str2U8Array(str,stVB1Channel1order.u8Vb12OOrder);
    }

    UBOOT_TRACE("OK\n");
    return;
}

int get_tcon_enable_from_board_ini(void)
{
    int ret = -1;
    char *script = NULL;
    char str[BUFFER_SIZE];
    char board_type[5] = "\0";
    char board_rev[3] = "\0";
    char board_id[BUFFER_SIZE] = "\0";
    char ConfigName[BUFFER_SIZE] = "\0";
    unsigned int u_board_type = 0;
    unsigned int u_board_rev = 0;
    U32 filesize = 0;

    UBOOT_TRACE("IN\n");

    idme_get_var_external("board_id", board_id, 16);
    strncpy(board_type, board_id, (sizeof(board_type) - 1));
    strncpy(board_rev, (board_id + 7), (sizeof(board_rev) - 1));
    u_board_type = strtol(board_type, NULL, 16);
    u_board_rev = strtol(board_rev, NULL, 16);

    /* get board.ini path
        in case config_name in idme is null,
        we try to get board.ini path from board_id
        board_id example: 0085002000020019
        board.ini path -> board_0x85_0x20.ini
    */

    if(!idme_get_var_external("config_name", ConfigName, (BUFFER_SIZE - 1)))
    {
        if(!strcmp(ConfigName, "0"))
        {
            snprintf(str, (BUFFER_SIZE - 1), "%s_0x%x_0x%x%s", BOARD_INI_PRE_PATH, u_board_type, u_board_rev, INI_SUFFIX);
        }
        else
        {
            snprintf(str, (BUFFER_SIZE - 1), "%s_%s%s", BOARD_INI_PRE_PATH, ConfigName, INI_SUFFIX);
        }
    }
    else
    {
        snprintf(str, (BUFFER_SIZE - 1), "%s_0x%x_0x%x%s", BOARD_INI_PRE_PATH, u_board_type, u_board_rev, INI_SUFFIX);
    }

    /* load board.ini */
    if(NULL != str)
    {
        UBOOT_INFO("Half board.ini path --> %s\n",str);
        char* tmpBoardPath = str;

        /* only use tvconfig partition board.ini*/
        if (bOdmModeEnable) {
            vfs_mount(CONFIG);
        }
        script = loadscript(tmpBoardPath,&filesize);
        if(script == NULL)
        {
            while (*tmpBoardPath== '/') tmpBoardPath++;
            tmpBoardPath=strstr(tmpBoardPath,"/");
            UBOOT_INFO("filter /vendor /cusdata or /config, and find file again!!\n");
            script = loadscript(tmpBoardPath,&filesize);
            if(script == NULL)
            {
                while (*tmpBoardPath== '/') tmpBoardPath++;
                tmpBoardPath=strstr(tmpBoardPath,"/");
                UBOOT_INFO("filter /vendor/cusdata or /config, and find file again!!\n");
                script = loadscript(tmpBoardPath,&filesize);
                if(script == NULL)
                {
                    char* location = NULL;
                    location = strstr(tmpBoardPath, "_");
                    if(location != NULL)
                    {
                        *location = '\0';
                    }
                    snprintf(tmpBoardPath, (BUFFER_SIZE - 1), "%s_0x%x_%s%s", tmpBoardPath, u_board_type, "default", INI_SUFFIX);
                    script = loadscript(tmpBoardPath,&filesize);
                    if(script == NULL)
                    {
                        UBOOT_ERROR("get board.ini path error\n");
                        goto exit;
                    }
                }
            }
        }
        snprintf(gBoardPath,BUFFER_SIZE,"%s",tmpBoardPath);
        UBOOT_INFO("Full board.ini  path --> %s\n",gBoardPath);
        ret = 0;
    }
    else
    {
        UBOOT_ERROR("get board.ini path error\n");
        goto exit;
    }

    /* get Tcon_Enable & OD_Enable from board.ini */
    Profile_Init((char *)script,filesize);
    bTconOutput = Profile_GetInteger("DispoutConfig", "m_bTconOutput", 0);
    bODEnable = Profile_GetInteger("DispoutConfig", "m_bODEnable", 0);
    UBOOT_INFO("get bTconOutput --> %d, bODEnable --> %d\n", bTconOutput, bODEnable);

exit:
    if(script)
        free(script);
    UBOOT_TRACE("OK\n");
    return ret;
}

int parse_model_ini(char* path, char* PnlPath, char* BoardPath, char* TconBinPath)
{
    int ret = -1;//
    char *script = NULL;
    char str[BUFFER_SIZE];
    U32 filesize = 0;
    int n;

    UBOOT_TRACE("IN\n");
    script = loadscript(path,&filesize);
    if(script == NULL)
    {
        while (*path== '/') path++;
        path=strstr(path,"/");
        UBOOT_INFO("filter /cusdata or /config, and find file again!!\n");
        script = loadscript(path,&filesize);
        if(script == NULL)
        {
            goto exit;
        }
    }
    UBOOT_INFO("sizeof( model ini script ) --------> %u bytes\n",filesize);
    Profile_Init((char *)script,filesize);
#if !defined(CONFIG_DATA_SEPARATION)
    char *panel_name = NULL;

    panel_name = "m_pPanelName";
    if(Profile_GetString("panel", panel_name,"", str, sizearray(str)))
    {
        UBOOT_INFO("Half panel.ini path --> %s\n",str);
        char* tmpPnlPath = str;
        script = loadscript(tmpPnlPath,&filesize);
        if(script == NULL)
        {
            while (*tmpPnlPath== '/') tmpPnlPath++;
            tmpPnlPath=strstr(tmpPnlPath,"/");
            UBOOT_INFO("filter /vendor /cusdata or /config, and find file again!!\n");
            script = loadscript(tmpPnlPath,&filesize);
            if(script == NULL)
            {
                while (*tmpPnlPath== '/') tmpPnlPath++;
                tmpPnlPath=strstr(tmpPnlPath,"/");
                UBOOT_INFO("filter /vendor/cusdata or /config, and find file again!!\n");
                script = loadscript(tmpPnlPath,&filesize);
            }
        }
        snprintf(PnlPath,BUFFER_SIZE,"%s",tmpPnlPath);
        UBOOT_INFO("Full panel.ini  path --> %s\n",PnlPath);
        ret = 0;
    }
    else
    {
        goto exit;
    }
#endif

    if(Profile_GetString("GAMMA_BIN", "PANEL_GAMMA_FILE","", str, sizearray(str)))
    {
        char* tmpPnlGammaBinPath = str;
        UBOOT_INFO("Half GAMMA_BIN path --> %s\n",str);
        if(FALSE==vfs_getsize(tmpPnlGammaBinPath))
        {
            while (*tmpPnlGammaBinPath== '/') tmpPnlGammaBinPath++;
            tmpPnlGammaBinPath=strstr(tmpPnlGammaBinPath,"/");
            UBOOT_INFO("filter /cusdata or /config, and find file again!!\n");

            if(FALSE == vfs_getsize(tmpPnlGammaBinPath))
            {
                while (*tmpPnlGammaBinPath== '/') tmpPnlGammaBinPath++;
                tmpPnlGammaBinPath=strstr(tmpPnlGammaBinPath,"/");
                UBOOT_INFO("filter /vendor/cusdata or /config, and find file again!!\n");
                if(FALSE==vfs_getsize(tmpPnlGammaBinPath))
                {
                    tmpPnlGammaBinPath=str;
                }
            }
        }
        printf("Half gamma.bin path --> %s\n",tmpPnlGammaBinPath);
        snprintf(gRGBWBinPath,BUFFER_SIZE,"%s",tmpPnlGammaBinPath);
        printf("Full gamma.bin  path --> %s\n",gRGBWBinPath);
    }

    if(Profile_GetString("LUT_BIN", "3DLUT_BIN_FILE","", str, sizearray(str)))
    {
        char* tmp3dLutBinPath = str;
        UBOOT_INFO("Half LUT_BIN path --> %s\n",str);
        if(FALSE==vfs_getsize(tmp3dLutBinPath))
        {
            while (*tmp3dLutBinPath== '/') tmp3dLutBinPath++;
            tmp3dLutBinPath=strstr(tmp3dLutBinPath,"/");
            UBOOT_INFO("filter /cusdata or /config, and find file again!!\n");

            if(FALSE == vfs_getsize(tmp3dLutBinPath))
            {
                tmp3dLutBinPath = str;
            }
        }
        printf("Half 3DLUT.bin path --> %s\n",tmp3dLutBinPath);
        snprintf(g3DLUTBinPath,BUFFER_SIZE,"%s",tmp3dLutBinPath);
        printf("Full 3DLUT.bin  path --> %s\n",g3DLUTBinPath);
    }

    n = Profile_GetInteger("KEYPAD", "keypadSelect", 0);
    if(n)
    {
        if(Profile_GetString("KEYPAD", "m_pKeypadPath", "", str, sizearray(str)))
        {
            printf("Half keypad path --> %s\n", str);
            snprintf(gKeypadPath, BUFFER_SIZE, "%s", str);
            printf("Full keypad  path --> %s\n", gKeypadPath);
            snprintf(str, BUFFER_SIZE, "%d", n);
            setenv("keypad_select", str);
        }
    }
    else
    {
        setenv("keypad_select", "");
    }

    if(Profile_GetString("TCON_BIN", "TCON_FILE","", str, sizearray(str)))
    {
        char* tmpTconBinPath;
        tmpTconBinPath=str;
        UBOOT_INFO("Half TCON_BIN path --> %s\n",str);
        if(FALSE==vfs_getsize(tmpTconBinPath))
        {
            while (*tmpTconBinPath== '/') tmpTconBinPath++;
            tmpTconBinPath=strstr(tmpTconBinPath,"/");
            UBOOT_INFO("filter /vendor/cusdata or /config, and find file again!!\n");

            if(FALSE==vfs_getsize(tmpTconBinPath))
            {
                while (*tmpTconBinPath== '/') tmpTconBinPath++;
                tmpTconBinPath=strstr(tmpTconBinPath,"/");
                UBOOT_INFO("filter /vendor/cusdata or /config, and find file again!!\n");
                if(FALSE==vfs_getsize(tmpTconBinPath))
                {
                    tmpTconBinPath=str;
                }
            }
        }
        snprintf(TconBinPath,BUFFER_SIZE,"%s",tmpTconBinPath);
        UBOOT_INFO("TCON path --> %s\n",TconBinPath);
        ret = 0;
    }
    _parse_model_vb1();

#if (CONFIG_PANEL_INIT == 1)
    memset(str,0,sizeof(str));
    n = Profile_GetInteger("PANEL_SWING_LEVEL", "SWING_LEVEL", -1);

    if (n >= 0)
    {
        snprintf(str,sizeof(str),"%u",n);
        UBOOT_INFO("swing_level -> [%s]\n",str);
        gstSysMiscSetting.m_u16Panel_SwingLevel = n;
    }
    else
    {
        gstSysMiscSetting.m_u16Panel_SwingLevel = 250;
    }
#endif

#if (CONFIG_URSA_UNION == 1 || CONFIG_TV_CHIP == 1)
    memset(gModulePath,0,sizeof(gModulePath));
    if(Profile_GetString("module", "m_pModuleName","", gModulePath, sizearray(gModulePath)))
    {
        UBOOT_DEBUG("module path: %s\n", gModulePath);
    }
    else
    {
        UBOOT_ERROR("get module path error\n");
    }
#endif

#if !defined(CONFIG_DATA_SEPARATION)
    if(Profile_GetString("MISC_MIRROR_CFG", "MIRROR_OSD","", str, sizearray(str)))
    {
        UBOOT_INFO("MIRROR_OSD is  --> %s\n",str);
        if((strcmp(str, "True") == 0)||(strcmp(str, "1") == 0))
        {
            // MIRROR_OSD_TYPE   # 0:normal type.  1:Horizontal-mirror only.  2:Vertical-mirror only.  3:HV-mirror.
            if(Profile_GetString("MISC_MIRROR_CFG", "MIRROR_OSD_TYPE","", str, sizearray(str)))
            {
                UBOOT_INFO("MIRROR_OSD_TYPE is  --> %s\n",str);
                if((strcmp(str, "1") == 0))
                    gstSysMiscSetting.m_u8MirrorMode = 3;
                else if((strcmp(str, "2") == 0))
                    gstSysMiscSetting.m_u8MirrorMode = 2;
                else
                    gstSysMiscSetting.m_u8MirrorMode = 1;
            }
            else
            {
                gstSysMiscSetting.m_u8MirrorMode = 1;
            }
        }
        else if((strcmp(str, "False") == 0)||(strcmp(str, "0") == 0))
        {
            gstSysMiscSetting.m_u8MirrorMode = 0;
        }
        else
        {

            UBOOT_INFO("MIRROR_OSD is  [True] or [False], other value can not be parsed\n");
        }
    }
    else
    {
        goto exit;
    }
    n = Profile_GetInteger("MISC_MIRROR_CFG", "MIRROR_OUTPUT_CONTROL", -1);
    UBOOT_INFO("MIRROR_OUTPUT_CONTROL is  --> %d\n",n);
    if (n >= 0)
    {
        gstSysMiscSetting.m_u8MOD_H_MirrorMode = n;
    }
    else
    {
        gstSysMiscSetting.m_u8MOD_H_MirrorMode = 0;
    }
    memset(str,0,sizeof(str));
    if(Profile_GetString("MISC_MIRROR_CFG", "MIRROR_VIDEO","", str, sizearray(str)))
    {
        snprintf(str,sizeof(str),"%u",n);
        UBOOT_INFO("MIRROR_VIDEO -> [%s]\n",str);
        if((strcmp(str, "True") == 0)||(strcmp(str, "1") == 0))
        {
            gstSysMiscSetting.m_u8VideoMirrorMode = 1;
        }
        else if((strcmp(str, "False") == 0)||(strcmp(str, "0") == 0))
        {
            gstSysMiscSetting.m_u8VideoMirrorMode = 0;
        }
        else
        {
            UBOOT_INFO("MIRROR_VIDEO is  [True] or [False], other value can not be parsed\n");
        }
    }
    else
    {
        gstSysMiscSetting.m_u8VideoMirrorMode = 0;
    }
#endif
#if defined(CONFIG_VOICE_CM4)
    parse_voice_setting();
#endif
#if(CONFIG_ENABLE_V_I_PWM == 1)
    memset(str,0,sizeof(str));
    int temp_safemode = Profile_GetBoolean("SafeMode", "bSafeModeEnable", 0);
    printf("--wya-- parse_model_ini temp_safemode =%d\n",temp_safemode);
    if (1 == temp_safemode)
    {
        bSafeModeEnable = 1;
        setenv("bSafeModeEnable","true");
    }
    else
    {
        bSafeModeEnable = 0;
        setenv("bSafeModeEnable","false");
    }
    saveenv();
#endif

exit:
    if(script)
        free(script);
    UBOOT_TRACE("OK\n");
    return ret;
}

#if (CONFIG_URSA_UNION == 1 || CONFIG_TV_CHIP == 1)
int parse_module_ini(void)
{
    int ret = 0;//
    char *script = NULL;
    U32 filesize = 0;
    U32 u32Urs_Type = 0;
    MS_U8 u8PixelShift = 0;
    char* path = gModulePath;
    MS_BOOL bOSDCenableFlag = 0;
    UBOOT_TRACE("IN\n");
    char str[BUFFER_SIZE];

    script = loadscript(gModulePath,&filesize);
    if(script == NULL)
    {
        while (*path== '/') path++;
        path=strstr(path,"/");
        UBOOT_INFO("filter /vendor/cusdata or /config, and find file again!!\n");
        script = loadscript(path,&filesize);
        if(script == NULL)
        {
            while (*path== '/') path++;
            path=strstr(path,"/");
            UBOOT_INFO("filter /vendor /cusdata or /config, and find file again!!\n");
            script = loadscript(path,&filesize);
            if(script == NULL)
            {
                ret = -1;
                goto exit;
            }
        }
    }
    UBOOT_INFO("sizeof( module ini script ) --------> %u bytes\n",filesize);

    Profile_Init((char *)script,filesize);
    u32Urs_Type = Profile_GetInteger("M_URSA", "F_URSA_URSA_TYPE", 0);
    gstSysMiscSetting.m_u32ursa_type = u32Urs_Type;
    UBOOT_DEBUG("ursa_type = %d \n",u32Urs_Type);
    u8PixelShift = Profile_GetInteger("M_PIXEL_SHIFT", "F_PIXEL_SHIFT_ENABLE", 0);
    gstSysMiscSetting.m_u8PixelShiftEnable = u8PixelShift;
    UBOOT_DEBUG("u8PixelShift = %d \n",u8PixelShift);
    bOSDCenableFlag = Profile_GetInteger("M_BACKEND", "F_BACKEND_ENABLE_OSDC", 0);
    gstSysMiscSetting.m_OSDCenableFlag = bOSDCenableFlag;
    UBOOT_DEBUG("bOSDCenableFlag = %d \n",bOSDCenableFlag);
#if defined(CONFIG_URSA_TXRX_SUPPORT)
    /* Ursa TX panel parameter */
    /* VbyOne_0_ChanMap_0~3 */
    long n;

    n = Profile_GetInteger("Vby1", "u8mfc_InitOutAll5_VbyOne_0_ChanMap_0", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("u8mfc_InitOutAll5_VbyOne_0_ChanMap_0 = %s\n",str);
    gstUrsaRXTX_PanelParameter.m_u8Mfc_VbyOne_0_ChanMap_0 = n;
    n = Profile_GetInteger("Vby1", "u8mfc_InitOutAll5_VbyOne_0_ChanMap_1", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("u8mfc_InitOutAll5_VbyOne_0_ChanMap_1 = %s\n",str);
    gstUrsaRXTX_PanelParameter.m_u8Mfc_VbyOne_0_ChanMap_1 = n;
    n = Profile_GetInteger("Vby1", "u8mfc_InitOutAll5_VbyOne_0_ChanMap_2", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("u8mfc_InitOutAll5_VbyOne_0_ChanMap_2 = %s\n",str);
    gstUrsaRXTX_PanelParameter.m_u8Mfc_VbyOne_0_ChanMap_2= n;
    n = Profile_GetInteger("Vby1", "u8mfc_InitOutAll5_VbyOne_0_ChanMap_3", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("u8mfc_InitOutAll5_VbyOne_0_ChanMap_3 = %s\n",str);
    gstUrsaRXTX_PanelParameter.m_u8Mfc_VbyOne_0_ChanMap_3= n;
    /* VbyOne_1_ChanMap_0~3 */
    n = Profile_GetInteger("Vby1", "u8mfc_InitOutAll5_VbyOne_1_ChanMap_0", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("u8mfc_InitOutAll5_VbyOne_1_ChanMap_0 = %s\n",str);
    gstUrsaRXTX_PanelParameter.m_u8Mfc_VbyOne_1_ChanMap_0 = n;
    n = Profile_GetInteger("Vby1", "u8mfc_InitOutAll5_VbyOne_1_ChanMap_1", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("u8mfc_InitOutAll5_VbyOne_1_ChanMap_1 = %s\n",str);
    gstUrsaRXTX_PanelParameter.m_u8Mfc_VbyOne_1_ChanMap_1 = n;
    n = Profile_GetInteger("Vby1", "u8mfc_InitOutAll5_VbyOne_1_ChanMap_2", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("u8mfc_InitOutAll5_VbyOne_1_ChanMap_2 = %s\n",str);
    gstUrsaRXTX_PanelParameter.m_u8Mfc_VbyOne_1_ChanMap_2= n;
    n = Profile_GetInteger("Vby1", "u8mfc_InitOutAll5_VbyOne_1_ChanMap_3", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("u8mfc_InitOutAll5_VbyOne_1_ChanMap_3 = %s\n",str);
    gstUrsaRXTX_PanelParameter.m_u8Mfc_VbyOne_1_ChanMap_3= n;
    /* VbyOne_2_ChanMap_0~3 */
    n = Profile_GetInteger("Vby1", "u8mfc_InitOutAll5_VbyOne_2_ChanMap_0", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("u8mfc_InitOutAll5_VbyOne_2_ChanMap_0 = %s\n",str);
    gstUrsaRXTX_PanelParameter.m_u8Mfc_VbyOne_2_ChanMap_0 = n;
    n = Profile_GetInteger("Vby1", "u8mfc_InitOutAll5_VbyOne_2_ChanMap_1", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("u8mfc_InitOutAll5_VbyOne_2_ChanMap_1 = %s\n",str);
    gstUrsaRXTX_PanelParameter.m_u8Mfc_VbyOne_2_ChanMap_1 = n;
    n = Profile_GetInteger("Vby1", "u8mfc_InitOutAll5_VbyOne_2_ChanMap_2", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("u8mfc_InitOutAll5_VbyOne_2_ChanMap_2 = %s\n",str);
    gstUrsaRXTX_PanelParameter.m_u8Mfc_VbyOne_2_ChanMap_2= n;
    n = Profile_GetInteger("Vby1", "u8mfc_InitOutAll5_VbyOne_2_ChanMap_3", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("u8mfc_InitOutAll5_VbyOne_2_ChanMap_3 = %s\n",str);
    gstUrsaRXTX_PanelParameter.m_u8Mfc_VbyOne_2_ChanMap_3= n;
    /* VbyOne_3_ChanMap_0~3 */
    n = Profile_GetInteger("Vby1", "u8mfc_InitOutAll5_VbyOne_3_ChanMap_0", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("u8mfc_InitOutAll5_VbyOne_3_ChanMap_0 = %s\n",str);
    gstUrsaRXTX_PanelParameter.m_u8Mfc_VbyOne_3_ChanMap_0 = n;
    n = Profile_GetInteger("Vby1", "u8mfc_InitOutAll5_VbyOne_3_ChanMap_1", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("u8mfc_InitOutAll5_VbyOne_3_ChanMap_1 = %s\n",str);
    gstUrsaRXTX_PanelParameter.m_u8Mfc_VbyOne_3_ChanMap_1 = n;
    n = Profile_GetInteger("Vby1", "u8mfc_InitOutAll5_VbyOne_3_ChanMap_2", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("u8mfc_InitOutAll5_VbyOne_3_ChanMap_2 = %s\n",str);
    gstUrsaRXTX_PanelParameter.m_u8Mfc_VbyOne_3_ChanMap_2= n;
    n = Profile_GetInteger("Vby1", "u8mfc_InitOutAll5_VbyOne_3_ChanMap_3", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("u8mfc_InitOutAll5_VbyOne_3_ChanMap_3 = %s\n",str);
    gstUrsaRXTX_PanelParameter.m_u8Mfc_VbyOne_3_ChanMap_3= n;
    /* VbyOne_4_ChanMap_0~3 */
    n = Profile_GetInteger("Vby1", "u8mfc_InitOutAll5_VbyOne_4_ChanMap_0", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("u8mfc_InitOutAll5_VbyOne_4_ChanMap_0 = %s\n",str);
    gstUrsaRXTX_PanelParameter.m_u8Mfc_VbyOne_4_ChanMap_0 = n;
    n = Profile_GetInteger("Vby1", "u8mfc_InitOutAll5_VbyOne_4_ChanMap_1", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("u8mfc_InitOutAll5_VbyOne_4_ChanMap_1 = %s\n",str);
    gstUrsaRXTX_PanelParameter.m_u8Mfc_VbyOne_4_ChanMap_1 = n;
    n = Profile_GetInteger("Vby1", "u8mfc_InitOutAll5_VbyOne_4_ChanMap_2", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("u8mfc_InitOutAll5_VbyOne_4_ChanMap_2 = %s\n",str);
    gstUrsaRXTX_PanelParameter.m_u8Mfc_VbyOne_4_ChanMap_2= n;
    n = Profile_GetInteger("Vby1", "u8mfc_InitOutAll5_VbyOne_4_ChanMap_3", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("u8mfc_InitOutAll5_VbyOne_4_ChanMap_3 = %s\n",str);
    gstUrsaRXTX_PanelParameter.m_u8Mfc_VbyOne_4_ChanMap_3= n;
    /* VbyOne_5_ChanMap_0~3 */
    n = Profile_GetInteger("Vby1", "u8mfc_InitOutAll5_VbyOne_5_ChanMap_0", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("u8mfc_InitOutAll5_VbyOne_5_ChanMap_0 = %s\n",str);
    gstUrsaRXTX_PanelParameter.m_u8Mfc_VbyOne_5_ChanMap_0 = n;
    n = Profile_GetInteger("Vby1", "u8mfc_InitOutAll5_VbyOne_5_ChanMap_1", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("u8mfc_InitOutAll5_VbyOne_5_ChanMap_1 = %s\n",str);
    gstUrsaRXTX_PanelParameter.m_u8Mfc_VbyOne_5_ChanMap_1 = n;
    n = Profile_GetInteger("Vby1", "u8mfc_InitOutAll5_VbyOne_5_ChanMap_2", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("u8mfc_InitOutAll5_VbyOne_5_ChanMap_2 = %s\n",str);
    gstUrsaRXTX_PanelParameter.m_u8Mfc_VbyOne_5_ChanMap_2= n;
    n = Profile_GetInteger("Vby1", "u8mfc_InitOutAll5_VbyOne_5_ChanMap_3", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("u8mfc_InitOutAll5_VbyOne_5_ChanMap_3 = %s\n",str);
    gstUrsaRXTX_PanelParameter.m_u8Mfc_VbyOne_5_ChanMap_3= n;
#endif
exit:
    if(script)
        free(script);
    UBOOT_TRACE("OK\n");
    return ret;
}
#endif

int parse_boot_ini(char* path)
{
    int ret = 0;//
    int n = 0;
    char *script = NULL;
    char str[BUFFER_SIZE];
    U32 filesize = 0;
    char *p_vol = NULL;
#if defined(CONFIG_DATA_SEPARATION)
    char * pEnv=NULL,* pEnv2=NULL;
    char s_CusCustomer[30]={'\0'},s_CusConfig[30]={'\0'};
#endif

    UBOOT_TRACE("IN\n");
    script = loadscript(path,&filesize);
    if(script == NULL)
    {
        ret = -1;
        goto exit;
    }
    UBOOT_INFO("sizeof( boot ini script ) --------> %u bytes\n",filesize);
    Profile_Init((char *)script,filesize);

    //Music
    char* countrycode = getenv("countrycode");
    char cfgname[BUFFER_SIZE] = "\0";

#if defined(CONFIG_DATA_SEPARATION)
    pEnv=getenv(ENV_CUSDATA_CUSTOMER_PATH);
    if (pEnv !=NULL)
    {
        UBOOT_DEBUG("Get MAP ENV_CUSDATA from env =%s \n",pEnv);
        snprintf(s_CusCustomer,strlen(pEnv),pEnv);
    }
    else
    {
        UBOOT_DEBUG("Get MAP CUSDATA_CUSTOMER_PATH fail \n");  // use default "Customer"
        snprintf(s_CusCustomer,sizeof(CUSDATA_CUSTOMER_PATH)+1,CUSDATA_CUSTOMER_PATH);
    }

    pEnv2=getenv(ENV_CUSDATA_CONFIG_PATH);
    if (pEnv2 !=NULL)
    {
        UBOOT_DEBUG("Get MAP ENV_CUSDATA from env =%s \n",pEnv2);
        snprintf(s_CusConfig,strlen(pEnv2),pEnv2);
    }
    else
    {
        UBOOT_DEBUG("Get MAP CUSDATA_CONFIG_PATH fail \n");  // use default "config"
        snprintf(s_CusConfig,sizeof(CUSDATA_CONFIG_PATH)+1,CUSDATA_CONFIG_PATH);
    }
#endif

    if(countrycode != NULL && Profile_GetString("MUSIC_CFG", cfgname,"", str, sizearray(str)))
    {
        snprintf(cfgname, BUFFER_SIZE, "MUSIC_NAME_%s", countrycode);
        UBOOT_DEBUG("[MusicCfg] = %s\n", cfgname);
#if defined(CONFIG_DATA_SEPARATION)
        snprintf(gMusicPath,sizeof(gMusicPath),"%s/misc/%s",s_CusConfig,str);
#else
        snprintf(gMusicPath,sizeof(gMusicPath),"%s/%s",CUSTOMER_PATH,str);
#endif
    }
    else if(Profile_GetString("MUSIC_CFG", "MUSIC_NAME","", str, sizearray(str)))
    {
#if defined(CONFIG_DATA_SEPARATION)
        snprintf(gMusicPath,sizeof(gMusicPath),"%s/misc/%s",s_CusConfig,str);
#else
        snprintf(gMusicPath,sizeof(gMusicPath),"%s/%s",CUSTOMER_PATH,str);
#endif
    }
    else
    {
        // use default music file
        UBOOT_DEBUG("no MUSIC_NAME in %s, use default music file\n",path);
#if defined(CONFIG_DATA_SEPARATION)
        snprintf(gMusicPath,sizeof(gMusicPath),"%s/misc/boot0.mp3",s_CusCustomer);
#else
        snprintf(gMusicPath,sizeof(gMusicPath),"%s/boot0.mp3",CUSTOMER_PATH);
#endif
    }
    UBOOT_INFO("gMusicPath --> %s\n",gMusicPath);

    //Logo
    if(Profile_GetString("LOGO_CFG", "LOGO_NAME","", str, sizearray(str)))
    {
#if defined(CONFIG_DATA_SEPARATION)
        snprintf(gLogoPath,sizeof(gLogoPath),"%s/misc/%s",s_CusConfig,str);
#else
        snprintf(gLogoPath,sizeof(gLogoPath),"%s/%s",CUSTOMER_PATH,str);
#endif
    }
    else
    {
        // use default logo file
        UBOOT_DEBUG("no LOGO_NAME in %s, use default logo file\n",path);
#if defined(CONFIG_DATA_SEPARATION)
        snprintf(gLogoPath,sizeof(gLogoPath),"%s/misc/boot0.jpg",s_CusConfig);
#else
        snprintf(gLogoPath,sizeof(gLogoPath),"%s/boot0.jpg",CUSTOMER_PATH);
#endif
    }
    UBOOT_INFO("gLogoPath --> %s\n",gLogoPath);

    //Music volume
    n = Profile_GetInteger("MUSIC_VOL_CFG", "MUSIC_VOL", 0);
    if((n >= 0)&&(n <= 0x7F))
    {
        snprintf(str,sizeof(str),"0x%x",n);
        UBOOT_INFO("m_u32MusicVol -> [%s]\n",str);
        gstSysMiscSetting.m_u32MusicVol = n;

        if(NULL == (p_vol=getenv("music_vol")))  //set MUSIC_VOL to env
        {
            setenv("music_vol", str);
        }
    }
    else
    {
        gstSysMiscSetting.m_u32MusicVol = 30;
        if(NULL == (p_vol=getenv("music_vol")))  //set MUSIC_VOL to env
        {
            setenv("music_vol", "0x1E");
        }
    }

exit:
    if(script)
        free(script);
    UBOOT_TRACE("OK\n");
    return ret;
}


int parse_ini(void)
{
    int ret = -1;
    char InPath [BUFFER_SIZE]="\0";
    char OutPath[BUFFER_SIZE]="\0";
    char ModelNamePath[BUFFER_SIZE]="\0";
#if defined(CONFIG_DATA_SEPARATION)
    char * pEnv=NULL;
    char * pEnv2=NULL;
    char s_CusConfig[30]={'\0'};
#endif
    memset(gPnlPath,0,sizeof(gPnlPath));
    memset(gBoardPath,0,sizeof(gBoardPath));
    memset(gMusicPath,0,sizeof(gMusicPath));
    memset(gLogoPath,0,sizeof(gLogoPath));
    memset(gGopMuxPath,0,sizeof(gGopMuxPath));
    memset(gTconBinPath,0,sizeof(gTconBinPath));
    memset(gLocalDimmingPath,0,sizeof(gLocalDimmingPath));
    memset(gRGBWBinPath,0,sizeof(gRGBWBinPath));
    memset(g3DLUTBinPath,0,sizeof(g3DLUTBinPath));
    memset(gKeypadPath,0,sizeof(gKeypadPath));

#if (CONFIG_DYNAMIC_IR)
    memset(gIRConfigPath,0,sizeof(gIRConfigPath));
#endif
#if (CONFIG_URSA_UNION == 1 || CONFIG_TV_CHIP == 1)
    memset(gModulePath,0,sizeof(gModulePath));
#endif

    UBOOT_TRACE("IN\n");
    snprintf(gLocalDimmingPath,sizeof(gLocalDimmingPath),"%s/ldm/ldm.ini",CONFIG_PATH);

    if (!idme_get_var_external("model_name", ModelNamePath, BUFFER_SIZE-1))
    {
        if (strncmp(ModelNamePath, "/firetv_odm", 11) == 0) {
            bOdmModeEnable = 1;
        } else {
            bOdmModeEnable = 0;
        }
    }
    UBOOT_INFO("bOdmModeEnable: %d\n", bOdmModeEnable);

    vfs_mount(CONFIG);
    snprintf(InPath,sizeof(InPath),"%s/sys.ini",CONFIG_PATH);
    ret = parse_sys_ini(InPath, OutPath, BUFFER_SIZE);
    if(ret)
    {
        goto exit;
    }

    memset(InPath,0,sizeof(InPath));
    memcpy(InPath,OutPath,sizeof(OutPath));
#if (CONFIG_BURN_CUSTOMER_IC)
    memcpy(CustomerInPath,InPath,sizeof(InPath));
#endif
#if defined(CONFIG_DATA_SEPARATION)
    pEnv=getenv(ENV_CUSDATA);
    if (pEnv !=NULL)
    {
        UBOOT_DEBUG("Get ENV_CUSDATA from env =%s \n",pEnv);
        if(vfs_mount(pEnv)!=0)
        {
            UBOOT_ERROR("mount %s fail\n",pEnv);
            goto exit;
        }
    }
    else
    {
        UBOOT_DEBUG("Get ENV_CUSDATA fail, use default:cusdata \n");  // use default "cusdata"
        if(vfs_mount(CUSDATA)!=0)  //use default PATH /cusdata
        {
            UBOOT_ERROR("mount %s fail\n",CONFIG);
            goto exit;
        }
    }
#else
    if (bOdmModeEnable) {
        vfs_mount(ODM);
    } else {
        vfs_mount(CONFIG);
    }
#endif
    ret = parse_model_ini(InPath, gPnlPath, gBoardPath, gTconBinPath);
    if(ret)
    {
        goto exit;
    }

    ret = get_tcon_enable_from_board_ini();
    if(ret)
    {
        goto exit;
    }

    #if !defined(CONFIG_DATA_SEPARATION)
    #if (CONFIG_URSA_UNION == 1 || CONFIG_TV_CHIP==1)
    vfs_mount(CONFIG);
    ret = parse_module_ini();
    if(ret)
    {
        UBOOT_ERROR("parse_module_ini fail \n");
    }
    #endif
    #endif
#if defined(CONFIG_DATA_SEPARATION)
    if (pEnv !=NULL)
    {
        UBOOT_DEBUG("Get MAP ENV_CUSDATA from env =%s \n",pEnv);
        if(vfs_mount(pEnv)!=0)
        {
            UBOOT_ERROR("mount %s fail\n",pEnv);
            goto exit;
        }
    }
    else
    {
        if(vfs_mount(CUSDATA)!=0)  //use default PATH /cusdata
        {
            UBOOT_ERROR("mount %s fail\n",CONFIG);
            goto exit;
        }
    }
#else
    vfs_mount(CUSTOMER);
#endif
    memset(InPath,0,sizeof(InPath));
#if defined(CONFIG_DATA_SEPARATION)
    pEnv2=getenv(ENV_CUSDATA_CONFIG_PATH);
    if (pEnv2 !=NULL)
    {
        UBOOT_DEBUG("Get ENV_CUSDATA from env =%s \n",pEnv2);
        snprintf(s_CusConfig,strlen(pEnv2),pEnv2);
    }
    else
    {
        UBOOT_DEBUG("Get CUSDATA_CONFIG_PATH fail,use default:config \n");  // use default "config"
        snprintf(s_CusConfig,sizeof(CUSDATA_CONFIG_PATH)+1,CUSDATA_CONFIG_PATH);
    }
    char ProjectIdValue[gProjectID_LEN]="\0";
    memset(InPath,0,sizeof(InPath));
    snprintf(InPath,sizeof(InPath),"%s/%s",s_CusConfig, gProjectID_Path);

    ret = parse_project_id_ini_cusdata(InPath,ProjectIdValue);
    if(ret)
    {
        goto exit;
    }

    memset(gPnlPath,0,sizeof(gPnlPath));
    memset(gMusicPath,0,sizeof(gMusicPath));
    memset(gLogoPath,0,sizeof(gLogoPath));
    memset(gTconBinPath,0,sizeof(gTconBinPath));
#if (CONFIG_URSA_UNION == 1 || CONFIG_TV_CHIP == 1)
    memset(gModulePath, 0, sizeof(gModulePath));
#endif
    memset(InPath,0,sizeof(InPath));
    snprintf(InPath,sizeof(InPath),"%s/dataIndex/dataIndex_%s.ini",s_CusConfig,ProjectIdValue);
#else
    snprintf(InPath,sizeof(InPath),"%s/boot.ini",CUSTOMER_PATH);
#endif

#if defined(CONFIG_DATA_SEPARATION) \
        &&defined(CONFIG_CUS_PROJECT_ID_EEPROM) && (CONFIG_CUS_PROJECT_ID_EEPROM == 1) \
        &&defined(CONFIG_CMD_EEPROM) && (CONFIG_MSTARI2C == 1)
        vfs_mount(CUSDATA);
#endif

    ret = parse_boot_ini(InPath);
    if(ret)
    {
        goto exit;
    }

#if defined(CONFIG_DATA_SEPARATION)

    ret = parse_data_ini_cusdata(InPath,gPnlPath, gKeypadPath);
    if(ret)
    {
        goto exit;
    }

#endif
#if defined(CONFIG_DATA_SEPARATION)
    ret = parse_module_ini();
    if(ret)
    {
        goto exit;
    }
#endif

exit:
    UBOOT_TRACE("OK\n");
    return ret;
}

static void calculate_dbitem_offset(U32 u32DbtableOffset)
{
    //calculate gstDbtable offset
    gstDbtable.dbdata[E_DB_LOGO].Offset       = u32DbtableOffset + sizeof(gstDbtable);
    gstDbtable.dbdata[E_DB_MUSIC].Offset      = gstDbtable.dbdata[E_DB_LOGO].Offset       + gstDbtable.dbdata[E_DB_LOGO].Size;
    gstDbtable.dbdata[E_DB_BOARD_PARA].Offset = gstDbtable.dbdata[E_DB_MUSIC].Offset      + gstDbtable.dbdata[E_DB_MUSIC].Size;
    gstDbtable.dbdata[E_DB_PANEL_PARA].Offset = gstDbtable.dbdata[E_DB_BOARD_PARA].Offset + gstDbtable.dbdata[E_DB_BOARD_PARA].Size;
    gstDbtable.dbdata[E_DB_URSA_6M30].Offset  = gstDbtable.dbdata[E_DB_PANEL_PARA].Offset + gstDbtable.dbdata[E_DB_PANEL_PARA].Size;
    gstDbtable.dbdata[E_DB_URSA].Offset       = gstDbtable.dbdata[E_DB_URSA_6M30].Offset  + gstDbtable.dbdata[E_DB_URSA_6M30].Size;
    gstDbtable.dbdata[E_DB_TCON].Offset       = gstDbtable.dbdata[E_DB_URSA].Offset       + gstDbtable.dbdata[E_DB_URSA].Size;
    gstDbtable.dbdata[E_DB_PNL_VB1].Offset    = gstDbtable.dbdata[E_DB_TCON].Offset       + gstDbtable.dbdata[E_DB_TCON].Size;
    gstDbtable.dbdata[E_DB_PNL_GOPMUX].Offset = gstDbtable.dbdata[E_DB_PNL_VB1].Offset    + gstDbtable.dbdata[E_DB_PNL_VB1].Size;
    gstDbtable.dbdata[E_DB_PWM].Offset        = gstDbtable.dbdata[E_DB_PNL_GOPMUX].Offset + gstDbtable.dbdata[E_DB_PNL_GOPMUX].Size;
    gstDbtable.dbdata[E_DB_MISC].Offset       = gstDbtable.dbdata[E_DB_PWM].Offset        + gstDbtable.dbdata[E_DB_PWM].Size;
    gstDbtable.dbdata[E_DB_MSPI].Offset       = gstDbtable.dbdata[E_DB_MISC].Offset       + gstDbtable.dbdata[E_DB_MISC].Size;
    gstDbtable.dbdata[E_DB_DMA].Offset       = gstDbtable.dbdata[E_DB_MSPI].Offset       + gstDbtable.dbdata[E_DB_MSPI].Size;
    gstDbtable.dbdata[E_DB_HDMITX].Offset     = gstDbtable.dbdata[E_DB_DMA].Offset       + gstDbtable.dbdata[E_DB_DMA].Size;
    gstDbtable.dbdata[E_DB_IR_BOOTCHECK].Offset       = gstDbtable.dbdata[E_DB_HDMITX].Offset       + gstDbtable.dbdata[E_DB_HDMITX].Size;
    gstDbtable.dbdata[E_DB_IR_WAKEUP].Offset     = gstDbtable.dbdata[E_DB_IR_BOOTCHECK].Offset       + gstDbtable.dbdata[E_DB_IR_BOOTCHECK].Size;
    gstDbtable.dbdata[E_DB_RGBW].Offset       = gstDbtable.dbdata[E_DB_IR_WAKEUP].Offset       + gstDbtable.dbdata[E_DB_IR_WAKEUP].Size;
    gstDbtable.dbdata[E_DB_KEYPAD].Offset       = gstDbtable.dbdata[E_DB_RGBW].Offset       + gstDbtable.dbdata[E_DB_RGBW].Size;
    gstDbtable.dbdata[E_DB_3DLUT].Offset       = gstDbtable.dbdata[E_DB_KEYPAD].Offset       + gstDbtable.dbdata[E_DB_KEYPAD].Size;
#if defined(CONFIG_DATA_SEPARATION)
    gstDbtable.dbdata[E_DB_KEYPAD_WAKEUP].Offset = gstDbtable.dbdata[E_DB_3DLUT].Offset       + gstDbtable.dbdata[E_DB_3DLUT].Size;
#endif
#if defined(CONFIG_VOICE_CM4) && defined(CONFIG_DATA_SEPARATION)
    gstDbtable.dbdata[E_DB_MIC_CONFIG].Offset = gstDbtable.dbdata[E_DB_KEYPAD_WAKEUP].Offset       + gstDbtable.dbdata[E_DB_KEYPAD_WAKEUP].Size;
#elif defined(CONFIG_VOICE_CM4)
    gstDbtable.dbdata[E_DB_MIC_CONFIG].Offset = gstDbtable.dbdata[E_DB_3DLUT].Offset       + gstDbtable.dbdata[E_DB_3DLUT].Size;
#endif
/*************************************************************************************************
CONFIG_DATA_SEPARATION | CONFIG_VOICE_CM4 | CONFIG_URSA_TXRX_SUPPORT | E_DB_OLED_LG_PANEL_PARA |#
0                                            0                                 0                                                1                                          |1
0                                            0                                 1                                                0                                          |2
0                                            0                                 1                                                1                                          |3
0                                            1                                 0                                                1                                          |5
0                                            1                                 1                                                0                                          |6
0                                            1                                 1                                                1                                          |7

1                                            0                                 0                                                1                                          |9
1                                            0                                 1                                                0                                          |A
1                                            0                                 1                                                1                                          |C
#1                                          #1                               #0                                             #1                                        |D same as #5
#1                                          #1                               #1                                             #0                                        |E same as #6
#1                                          #1                               #1                                             #1                                        |F same as #7
***************************************************************************************************/
#if !defined(CONFIG_DATA_SEPARATION) && !defined(CONFIG_VOICE_CM4) && !defined(CONFIG_URSA_TXRX_SUPPORT) && defined(CONFIG_PNL_OLED_LG_SUPPORT)
    gstDbtable.dbdata[E_DB_OLED_LG_PANEL_PARA].Offset = gstDbtable.dbdata[E_DB_3DLUT].Offset       + gstDbtable.dbdata[E_DB_3DLUT].Size;
#elif !defined(CONFIG_DATA_SEPARATION) && !defined(CONFIG_VOICE_CM4) && defined(CONFIG_URSA_TXRX_SUPPORT) && !defined(CONFIG_PNL_OLED_LG_SUPPORT)
    gstDbtable.dbdata[E_DB_URSA_TXRX_PANEL_PARA].Offset = gstDbtable.dbdata[E_DB_3DLUT].Offset       + gstDbtable.dbdata[E_DB_3DLUT].Size;
#elif !defined(CONFIG_DATA_SEPARATION) && !defined(CONFIG_VOICE_CM4) && defined(CONFIG_URSA_TXRX_SUPPORT) && defined(CONFIG_PNL_OLED_LG_SUPPORT)
    gstDbtable.dbdata[E_DB_URSA_TXRX_PANEL_PARA].Offset = gstDbtable.dbdata[E_DB_3DLUT].Offset       + gstDbtable.dbdata[E_DB_3DLUT].Size;
    gstDbtable.dbdata[E_DB_OLED_LG_PANEL_PARA].Offset = gstDbtable.dbdata[E_DB_URSA_TXRX_PANEL_PARA].Offset       + gstDbtable.dbdata[E_DB_URSA_TXRX_PANEL_PARA].Size;
#elif !defined(CONFIG_DATA_SEPARATION) && defined(CONFIG_VOICE_CM4) && !defined(CONFIG_URSA_TXRX_SUPPORT) && defined(CONFIG_PNL_OLED_LG_SUPPORT)
    gstDbtable.dbdata[E_DB_OLED_LG_PANEL_PARA].Offset = gstDbtable.dbdata[E_DB_MIC_CONFIG].Offset       + gstDbtable.dbdata[E_DB_MIC_CONFIG].Size;
#elif !defined(CONFIG_DATA_SEPARATION) && defined(CONFIG_VOICE_CM4) && defined(CONFIG_URSA_TXRX_SUPPORT) && !defined(CONFIG_PNL_OLED_LG_SUPPORT)
    gstDbtable.dbdata[E_DB_URSA_TXRX_PANEL_PARA].Offset = gstDbtable.dbdata[E_DB_MIC_CONFIG].Offset       + gstDbtable.dbdata[E_DB_MIC_CONFIG].Size;
#elif !defined(CONFIG_DATA_SEPARATION) && defined(CONFIG_VOICE_CM4) && defined(CONFIG_URSA_TXRX_SUPPORT) && defined(CONFIG_PNL_OLED_LG_SUPPORT)
    gstDbtable.dbdata[E_DB_URSA_TXRX_PANEL_PARA].Offset = gstDbtable.dbdata[E_DB_MIC_CONFIG].Offset       + gstDbtable.dbdata[E_DB_MIC_CONFIG].Size;
    gstDbtable.dbdata[E_DB_OLED_LG_PANEL_PARA].Offset = gstDbtable.dbdata[E_DB_URSA_TXRX_PANEL_PARA].Offset       + gstDbtable.dbdata[E_DB_URSA_TXRX_PANEL_PARA].Size;
#elif defined(CONFIG_DATA_SEPARATION) && !defined(CONFIG_VOICE_CM4) && !defined(CONFIG_URSA_TXRX_SUPPORT) && defined(CONFIG_PNL_OLED_LG_SUPPORT)
    gstDbtable.dbdata[E_DB_OLED_LG_PANEL_PARA].Offset = gstDbtable.dbdata[E_DB_KEYPAD_WAKEUP].Offset       + gstDbtable.dbdata[E_DB_KEYPAD_WAKEUP].Size;
#elif defined(CONFIG_DATA_SEPARATION) && !defined(CONFIG_VOICE_CM4) && defined(CONFIG_URSA_TXRX_SUPPORT) && !defined(CONFIG_PNL_OLED_LG_SUPPORT)
    gstDbtable.dbdata[E_DB_URSA_TXRX_PANEL_PARA].Offset = gstDbtable.dbdata[E_DB_KEYPAD_WAKEUP].Offset       + gstDbtable.dbdata[E_DB_KEYPAD_WAKEUP].Size;
#elif defined(CONFIG_DATA_SEPARATION) && !defined(CONFIG_VOICE_CM4) && defined(CONFIG_URSA_TXRX_SUPPORT) && defined(CONFIG_PNL_OLED_LG_SUPPORT)
    gstDbtable.dbdata[E_DB_URSA_TXRX_PANEL_PARA].Offset = gstDbtable.dbdata[E_DB_KEYPAD_WAKEUP].Offset       + gstDbtable.dbdata[E_DB_KEYPAD_WAKEUP].Size;
    gstDbtable.dbdata[E_DB_OLED_LG_PANEL_PARA].Offset = gstDbtable.dbdata[E_DB_URSA_TXRX_PANEL_PARA].Offset       + gstDbtable.dbdata[E_DB_URSA_TXRX_PANEL_PARA].Size;
//#elif defined(CONFIG_DATA_SEPARATION) && defined(CONFIG_VOICE_CM4) && !defined(CONFIG_URSA_TXRX_SUPPORT) && defined(E_DB_OLED_LG_PANEL_PARA)
//    gstDbtable.dbdata[E_DB_OLED_LG_PANEL_PARA].Offset = gstDbtable.dbdata[E_DB_MIC_CONFIG].Offset       + gstDbtable.dbdata[E_DB_MIC_CONFIG].Size;
//#elif defined(CONFIG_DATA_SEPARATION) && defined(CONFIG_VOICE_CM4) && defined(CONFIG_URSA_TXRX_SUPPORT) && !defined(E_DB_OLED_LG_PANEL_PARA)
//    gstDbtable.dbdata[E_DB_URSA_TXRX_PANEL_PARA].Offset = gstDbtable.dbdata[E_DB_MIC_CONFIG].Offset       + gstDbtable.dbdata[E_DB_MIC_CONFIG].Size;
//#elif defined(CONFIG_DATA_SEPARATION) && defined(CONFIG_VOICE_CM4) && defined(CONFIG_URSA_TXRX_SUPPORT) && defined(E_DB_OLED_LG_PANEL_PARA)
//    gstDbtable.dbdata[E_DB_URSA_TXRX_PANEL_PARA].Offset = gstDbtable.dbdata[E_DB_MIC_CONFIG].Offset       + gstDbtable.dbdata[E_DB_MIC_CONFIG].Size;
//    gstDbtable.dbdata[E_DB_OLED_LG_PANEL_PARA].Offset = gstDbtable.dbdata[E_DB_URSA_TXRX_PANEL_PARA].Offset       + gstDbtable.dbdata[E_DB_URSA_TXRX_PANEL_PARA].Size;
#endif

}


int prepare_nand_dbtable(U32 u32DbtableOffset)
{
    int ret = -1;
    char * pEnv=NULL;
    char *script = NULL;
    char str[BUFFER_SIZE] = {0};
    char buf[BUFFER_SIZE] = {0};
    char *str_tmp;
    U32 filesize = 0;

    UBOOT_TRACE("IN\n");
    memset((void*)&gstDbtable, 0, sizeof(gstDbtable));
#if defined(CONFIG_DATA_SEPARATION)
    pEnv=getenv(ENV_CUSDATA);
    if (pEnv !=NULL)
    {
        UBOOT_DEBUG("Get MAP ENV_CUSDATA from env =%s \n",pEnv);
        if(vfs_mount(pEnv)!=0)
        {
            UBOOT_ERROR("mount %s fail\n",pEnv);
            goto exit;
        }
    }
    else
    {
        if(vfs_mount(CUSDATA)!=0)  //use default PATH /cusdata
        {
            UBOOT_ERROR("mount %s fail\n",CONFIG);
            goto exit;
        }
    }
#else
    vfs_mount(CUSTOMER);
#endif

#if(ENABLE_DISPLAY_LOGO)
    char *volName;
    char old_volName[32];
    char *new_volName = NULL;
    char subdir[CONFIG_SYS_CBSIZE];
    char *logoPath = NULL;

    // Read logo priority
    // 1. Try reading bootlogo index from /firetv_odm/odm.ini
    // 2. Try using bootlogo path in MBoot ENV (BootlogoFile)
    // 3. Try using bootlogo path in MBoot ENV (_BootlogoFile)
    // 4. Use default path.

    volName = vfs_get_volname();
    strncpy(old_volName, volName, 31);

    // 1. Try reading bootlogo index from /firetv_odm/odm.ini
    vfs_mount(ODM);
    script = loadscript(ODM_INI_FILE_PATH, &filesize);
    if (script != NULL)
    {
        Profile_Init((char *)script, filesize);

        if (Profile_GetString(BOOTLOGO_INI_SECTION_NAME, BOOTLOGO_INI_KEY_NAME, BOOTLOGO_INI_DEFAULT_VALUE, buf, sizearray(buf)))
        {
            UBOOT_DEBUG("Get bootlogo number = %s\n", buf);
            if (bOdmModeEnable == 0) {
                snprintf(str, (BUFFER_SIZE-1), "%s%s.jpg",
                    BOOTLOGO_PRE_PATH,
                    buf
                    );
            //check if bootlogo file exist.
            //remove partition part in full file path as parameter of vfs_getsize()
                vfs_mount("tvconfig");
            } else {
                snprintf(str, (BUFFER_SIZE-1), "%s%s.jpg",
                    BOOTLOGO_ODM_PRE_PATH,
                    buf
                    );
            }

            str_tmp = str;
            while (*str_tmp== '/') str_tmp++;
            str_tmp=strstr(str_tmp,"/");
            filesize = vfs_getsize(str_tmp);

            if(filesize)
            {
                UBOOT_INFO("Bootlogo Path = %s\n", str);
                logoPath = str;
            }
        }
    }
    if(script)
    {
        free(script);
    }
    vfs_umount();

    // 2. Try using bootlogo path in MBoot ENV (BootlogoFile)
    if (NULL == logoPath  || filesize <= 0)
    {
        UBOOT_INFO("no bootlogo from ini, try using bootlogo in MBoot ENV (BootlogoFile)\n");
        logoPath = getenv("BootlogoFile");
    }

    // 3. Try using bootlogo path in MBoot ENV (_BootlogoFile)
    if (NULL == logoPath)
    {
        UBOOT_INFO("No bootlogo from MBoot ENV (BootlogoFile), try using MBoot ENV (_BootlogoFile)\n");
        logoPath = getenv("_BootlogoFile");
    }

    // 4. Use default path
    if (NULL == logoPath)
    {
        UBOOT_INFO("No bootlogo from MBoot ENV (_BootlogoFile), use default path = %s\n",logoPath);
        logoPath = BOOTLOGO_DEFAULT_PATH;
    }

    if (NULL != logoPath)
    {
        vfs_mount_by_abspath(logoPath, subdir, CONFIG_SYS_CBSIZE);
        gstDbtable.dbdata[E_DB_LOGO].Size = vfs_getsize(subdir);
        UBOOT_DEBUG("logoPath = %s\n",logoPath);
        UBOOT_DEBUG("gstDbtable.dbdata[E_DB_LOGO].Size=0x%x\n", gstDbtable.dbdata[E_DB_LOGO].Size);
        snprintf(gIniBootLogo,sizeof(gIniBootLogo),"%s",logoPath);

        new_volName = vfs_get_volname();
        if ((strlen(old_volName) > 0) && (strcmp(new_volName, old_volName) != 0))
        {
            UBOOT_DEBUG("old_volNmae = %s\n", old_volName);
            vfs_mount(old_volName);
        }
    }
    else
    {
        gstDbtable.dbdata[E_DB_LOGO].Size = vfs_getsize(gLogoPath);
        UBOOT_DEBUG("gLogoPath=%s\n",gLogoPath);
        UBOOT_DEBUG("gstDbtable.dbdata[E_DB_LOGO].Size=0x%x\n", gstDbtable.dbdata[E_DB_LOGO].Size);
        snprintf(gIniBootLogo,sizeof(gIniBootLogo),"%s",gLogoPath);
    }

    if(gstDbtable.dbdata[E_DB_LOGO].Size > 0)
    {
        UBOOT_DEBUG("%s size =:0x%08x \n",gLogoPath, gstDbtable.dbdata[E_DB_LOGO].Size);
    }
    else if ((gstDbtable.dbdata[E_DB_LOGO].Size == 0) && (NULL != logoPath))
    {
        UBOOT_ERROR("%s do not exist !\n", logoPath);
    }
    else
    {
        UBOOT_ERROR(" get logo_size fail...>>>:%d\n", gstDbtable.dbdata[E_DB_LOGO].Size);
        return ret;
    }
#endif

#if ENABLE_POWER_MUSIC
    //get music_size
    gstDbtable.dbdata[E_DB_MUSIC].Size = vfs_getsize(gMusicPath);
    UBOOT_DEBUG("gMusicPath=%s\n",gMusicPath);
    UBOOT_DEBUG("gstDbtable.dbdata[E_DB_MUSIC].Size=0x%x\n",gstDbtable.dbdata[E_DB_MUSIC].Size);
    if( gstDbtable.dbdata[E_DB_MUSIC].Size > 0)
    {
        UBOOT_DEBUG("%s size =:0x%08x \n",gMusicPath,gstDbtable.dbdata[E_DB_MUSIC].Size);
    }
    else
    {
        UBOOT_ERROR(" get music_size fail...>>>:%d\n",gstDbtable.dbdata[E_DB_MUSIC].Size);
        return ret; //althought music is set off, logo can be showed if it is set on
    }
#endif


#if (CONFIG_PANEL_INIT)
    gstDbtable.dbdata[E_DB_PANEL_PARA].Size = sizeof(PanelType);
    gstDbtable.dbdata[E_DB_BOARD_PARA].Size = sizeof(st_board_para);
#endif

#if(ENABLE_ENABLE_URSA == 1)
#if(ENABLE_URSA_6M30 == 1)
    gstDbtable.dbdata[E_DB_URSA_6M30].Size = sizeof(ursa_6m30_cmd_table);
#elif (ENABLE_URSA_8 == 1)|| (ENABLE_URSA_6M40 == 1)
    gstDbtable.dbdata[E_DB_URSA].Size = sizeof(ursa_cmd_table);
#endif
#endif

    gstDbtable.dbdata[E_DB_PNL_GOPMUX].Size = sizeof(st_gopmux_para);

    if(ENABLE_MODULE_ANDROID_BOOT==0)
    {
#if defined(CONFIG_DATA_SEPARATION)
        if (pEnv !=NULL)
        {
            UBOOT_DEBUG("Get MAP ENV_CUSDATA from env =%s \n",pEnv);
            if(vfs_mount(pEnv)!=0)
            {
                UBOOT_ERROR("mount %s fail\n",pEnv);
                goto exit;
            }
        }
        else
        {
            if(vfs_mount(CUSDATA)!=0)  //use default PATH /cusdata
            {
                UBOOT_ERROR("mount %s fail\n",CONFIG);
                goto exit;
            }
        }
#else
        vfs_mount(CONFIG);
#endif
    }
    if(bTconOutput)
    {
        gstDbtable.dbdata[E_DB_TCON].Size = vfs_getsize(gTconBinPath);
        if(gstDbtable.dbdata[E_DB_TCON].Size > 0)
        {
            UBOOT_DEBUG("gTconBinPath=%s\n",gTconBinPath);
            UBOOT_DEBUG("gstDbtable.dbdata[E_DB_TCON] size =:0x%08x \n",gstDbtable.dbdata[E_DB_TCON].Size);
        }
        else
        {
            UBOOT_INFO("get tcon_size fail...>>>:%d\n", gstDbtable.dbdata[E_DB_TCON].Size);
        }
    }
    else
    {
        UBOOT_INFO("Tcon model, no need to store Tcon bin size to DB\n");
    }

    if(ENABLE_MODULE_ANDROID_BOOT==0)
    {
#if defined(CONFIG_DATA_SEPARATION)
        if (pEnv !=NULL)
        {
            UBOOT_DEBUG("Get MAP ENV_CUSDATA from env =%s \n",pEnv);
            if(vfs_mount(pEnv)!=0)
            {
                UBOOT_ERROR("mount %s fail\n",pEnv);
                goto exit;
            }
        }
        else
        {
            if(vfs_mount(CUSDATA)!=0)  //use default PATH /cusdata
            {
                UBOOT_ERROR("mount %s fail\n",CONFIG);
                goto exit;
            }
        }
#else
        vfs_mount(CONFIG);
#endif
    }
    if(bTconOutput)
    {
        gstDbtable.dbdata[E_DB_RGBW].Size = vfs_getsize(gRGBWBinPath);
        if(0 == gstDbtable.dbdata[E_DB_RGBW].Size && strlen(gRGBWBinPath)>0)
        {
            char* BinPath = gRGBWBinPath;
            BinPath++;
            BinPath=strstr(BinPath,"/");
            UBOOT_INFO("filter %s, and find file again!!\n", BinPath);

            gstDbtable.dbdata[E_DB_RGBW].Size=vfs_getsize(BinPath);
        }

        printf("gRGBWPath=%s\n",gRGBWBinPath);
        printf("gstDbtable.dbdata[E_DB_RGBW].Size=0x%x\n",gstDbtable.dbdata[E_DB_RGBW].Size);
    }
    else
    {
        UBOOT_INFO("Tcon model, no need to store Gamma bin size to DB\n");
    }

    gstDbtable.dbdata[E_DB_3DLUT].Size = vfs_getsize(g3DLUTBinPath);
    printf("g3DLUTBinPath=%s\n",g3DLUTBinPath);
    printf("gstDbtable.dbdata[E_DB_3DLUT].Size=0x%x\n",gstDbtable.dbdata[E_DB_3DLUT].Size);

    gstDbtable.dbdata[E_DB_PNL_VB1].Size = sizeof(pnl_VB1ChannelOrder);
    gstDbtable.dbdata[E_DB_PWM].Size = sizeof(st_pwm_setting);
    gstDbtable.dbdata[E_DB_MISC].Size = sizeof(st_sys_misc_setting);
#if (CONFIG_LOCAL_DIMMING)
    gstDbtable.dbdata[E_DB_MSPI].Size = sizeof(ST_DRV_MSPI_INFO);
    gstDbtable.dbdata[E_DB_DMA].Size = sizeof(ST_DRV_LD_DMA_INFO);
#endif
    gstDbtable.dbdata[E_DB_HDMITX].Size = sizeof(st_hdmitx_setting);
#if (CONFIG_DYNAMIC_IR)
    gstDbtable.dbdata[E_DB_IR_BOOTCHECK].Size = sizeof(st_IRBootCheck_t);
    gstDbtable.dbdata[E_DB_IR_WAKEUP].Size = sizeof(st_IRWakeup_t);
#endif
#if (CONFIG_KEYPAD)
    gstDbtable.dbdata[E_DB_KEYPAD].Size = sizeof(SAR_KpdRegCfg_t) * MAXKEYPADCH;
#endif
#if defined(CONFIG_DATA_SEPARATION)
    gstDbtable.dbdata[E_DB_KEYPAD_WAKEUP].Size = sizeof(st_KeypadWakeup_t);
#endif
#if defined(CONFIG_VOICE_CM4)
    gstDbtable.dbdata[E_DB_MIC_CONFIG].Size = sizeof(stMic_config);
#endif
#if defined(CONFIG_URSA_TXRX_SUPPORT)
    gstDbtable.dbdata[E_DB_URSA_TXRX_PANEL_PARA].Size = sizeof(st_PNL_UrsaRXTX_PanelParameter);
    printf("gstDbtable.dbdata[E_DB_URSA_TXRX_PANEL_PARA] size =:0x%08x \n",gstDbtable.dbdata[E_DB_URSA_TXRX_PANEL_PARA].Size);
#endif
#if defined(CONFIG_PNL_OLED_LG_SUPPORT)
    gstDbtable.dbdata[E_DB_OLED_LG_PANEL_PARA].Size = sizeof(st_PNL_OLED_SpecificParameter);
    printf("gstDbtable.dbdata[E_DB_OLED_LG_PANEL_PARA] size =:0x%08x \n",gstDbtable.dbdata[E_DB_OLED_LG_PANEL_PARA].Size);
#endif

    // check if we need to resize Dbtable item.
    {
        U32 used_size = 0;
        U32 mpool_size = 0;
        // Get MPool volume/partition size
        mpool_size = GetRealOffset(0);

        u32Dbload_size =  sizeof(gstDbtable)
                        + gstDbtable.dbdata[E_DB_LOGO].Size
                        + gstDbtable.dbdata[E_DB_MUSIC].Size
                        + gstDbtable.dbdata[E_DB_BOARD_PARA].Size
                        + gstDbtable.dbdata[E_DB_PANEL_PARA].Size
                        + gstDbtable.dbdata[E_DB_URSA_6M30].Size
                        + gstDbtable.dbdata[E_DB_URSA].Size
                        + gstDbtable.dbdata[E_DB_TCON].Size
                        + gstDbtable.dbdata[E_DB_PNL_VB1].Size
                        + gstDbtable.dbdata[E_DB_PNL_GOPMUX].Size
                        + gstDbtable.dbdata[E_DB_PWM].Size
                        + gstDbtable.dbdata[E_DB_MISC].Size
                        + gstDbtable.dbdata[E_DB_MSPI].Size
                        + gstDbtable.dbdata[E_DB_DMA].Size
                        + gstDbtable.dbdata[E_DB_HDMITX].Size
                        + gstDbtable.dbdata[E_DB_IR_BOOTCHECK].Size
                        + gstDbtable.dbdata[E_DB_IR_WAKEUP].Size
                        + gstDbtable.dbdata[E_DB_RGBW].Size
                        + gstDbtable.dbdata[E_DB_KEYPAD].Size
                        + gstDbtable.dbdata[E_DB_3DLUT].Size;
    #if defined(CONFIG_DATA_SEPARATION)
        u32Dbload_size += gstDbtable.dbdata[E_DB_KEYPAD_WAKEUP].Size;
    #endif
    #if defined(CONFIG_VOICE_CM4)
        u32Dbload_size += gstDbtable.dbdata[E_DB_MIC_CONFIG].Size;
    #endif
#if defined(CONFIG_URSA_TXRX_SUPPORT)
        u32Dbload_size += gstDbtable.dbdata[E_DB_URSA_TXRX_PANEL_PARA].Size;
#endif
#if defined(CONFIG_PNL_OLED_LG_SUPPORT)
        u32Dbload_size += gstDbtable.dbdata[E_DB_OLED_LG_PANEL_PARA].Size;
#endif

        //roughly calculate mpool data used size
        used_size = u32DbtableOffset + u32Dbload_size + env_sector_size;

        UBOOT_DEBUG("u32DbtableOffset  =: 0x%x \n", u32DbtableOffset);
        UBOOT_DEBUG("u32Dbload_size    =: 0x%x \n", u32Dbload_size);
        UBOOT_DEBUG("used_size         =: 0x%x \n", used_size);
        UBOOT_DEBUG("mpool_size        =: 0x%x \n", mpool_size);

        // mpool data is over mpool volume/partition size
        if(used_size >= mpool_size)
        {
        #if(ENABLE_DISPLAY_LOGO)
            int bootlogo_size = gstDbtable.dbdata[E_DB_LOGO].Size;
            used_size = used_size - bootlogo_size;
            if (used_size >= mpool_size)
            {
                UBOOT_ERROR("mpool data is over mpool volume/partition size\n");
                calculate_dbitem_offset(u32DbtableOffset);
                _print_dbtable_info();
                jump_to_console();
            }
            else
            {
                gstDbtable.dbdata[E_DB_LOGO].Size = 0;
                u32Dbload_size -= bootlogo_size;
                UBOOT_INFO("bootlogo is too big to store in flash, using env(BootlogoFile)\n");
                UBOOT_DEBUG("u32Dbload_size    =: 0x%x \n", u32Dbload_size);
                UBOOT_DEBUG("used_size         =: 0x%x \n", used_size);
            }
        #else
            UBOOT_ERROR("mpool data is over mpool volume/partition size\n");
            calculate_dbitem_offset(u32DbtableOffset);
            _print_dbtable_info();
            jump_to_console();
        #endif
        }
    }

    //calculate gstDbtable offset
    calculate_dbitem_offset(u32DbtableOffset);
    _print_dbtable_info();

    pDbBufferAddr = malloc(u32Dbload_size);
    if(pDbBufferAddr == NULL)
    {
        UBOOT_ERROR("malloc dbload memory error !");
        return ret;
    }

    // copy db_table to DB Pool memory buffer
    memset((void*)pDbBufferAddr,0,u32Dbload_size);
    ret = 0;

exit:
    UBOOT_TRACE("OK\n");
    return ret;
}

int write_dbtable(U32 u32DbtableOffset)
{
    int ret = -1;
    UBOOT_TRACE("IN\n");
    memset(&gstSysMiscSetting,0,sizeof(st_sys_misc_setting));
    memset(&gstPWMSetting,0,sizeof(st_pwm_setting));
#if defined(CONFIG_URSA_TXRX_SUPPORT)
    memset(&gstUrsaRXTX_PanelParameter,0,sizeof(st_sys_misc_setting));
#endif
#if defined(CONFIG_PNL_OLED_LG_SUPPORT)
    memset(&gstPNL_OLED_SpecificParameter,0,sizeof(st_PNL_OLED_SpecificParameter));
#endif

    parse_ini();
    if(prepare_nand_dbtable(u32DbtableOffset)==-1)
    {
        if(pDbBufferAddr)
        {
            free(pDbBufferAddr);
            pDbBufferAddr = NULL;
        }
        UBOOT_ERROR("flash database not ready...>>>\n");
        return ret;
    }
    else
    {

#if ENABLE_DISPLAY_LOGO
        Load_LogoToFlash(u32DbtableOffset);
#endif
#if ENABLE_POWER_MUSIC
        Load_MusicToFlash(u32DbtableOffset);
#endif
#if (CONFIG_PANEL_INIT)

        char *p_str = NULL;
        p_str = getenv ("panel_path");
        if(NULL != p_str)
        {
            memcpy(gPnlPath, p_str, sizeof(gPnlPath));
        }
        Load_PanelSetting_ToFlash(u32DbtableOffset);
        Load_BoardSetting_ToFlash(u32DbtableOffset);

    #if (ENABLE_HDMI_RESOLUTION_RESET==1)
        // set resolution info to bootargs
        Set_BootArgs_Resolution();
    #endif
#endif
#if ENABLE_ENABLE_URSA
#if ENABLE_URSA_6M30
        //Load_Ursa6m30Para_ToFlash();
#elif (ENABLE_URSA_8 == 1) || (ENABLE_URSA_6M40 == 1)
        Load_UrsaPara_ToFlash(u32DbtableOffset);
#endif
#endif
        Load_GopMuxToFlash(u32DbtableOffset);

        if (gstDbtable.dbdata[E_DB_TCON].Size > 0 && bTconOutput)
        {
            Load_TConToFlash(u32DbtableOffset);
        }

        Load_PnlVb1ToFlash(u32DbtableOffset);
        Load_PWMSetting_ToFlash(u32DbtableOffset);
        Load_MiscSetting_ToFlash(u32DbtableOffset);
#if defined(CONFIG_URSA_TXRX_SUPPORT)
        Load_UrsaRXTXPanelSetting_ToFlash(u32DbtableOffset);
#endif
#if defined(CONFIG_PNL_OLED_LG_SUPPORT)
        Load_OLEDSpecificParameter_ToFlash(u32DbtableOffset);
#endif

#if (CONFIG_LOCAL_DIMMING)
        Load_LDMPara_ToFlash(u32DbtableOffset);
#endif
        Load_HDMITXToFlash(u32DbtableOffset);
#if (CONFIG_DYNAMIC_IR)
        Load_IRConfig_ToFlash(u32DbtableOffset);
#endif
        if (gstDbtable.dbdata[E_DB_TCON].Size > 0 && bTconOutput)
        {
            Load_RGBWBinToFlash(u32DbtableOffset);
        }
#if (CONFIG_BURN_CUSTOMER_IC)
        Burn_CustomerBinToExternalFlash(CustomerInPath);
#endif
#if (CONFIG_KEYPAD)
        Load_KeypadToFlash(u32DbtableOffset);
#endif

#if defined(CONFIG_VOICE_CM4)
        Load_MicPara_ToFlash(u32DbtableOffset);
#endif
        Load_3DLUTBinToFlash(u32DbtableOffset);
#if defined(CONFIG_DATA_SEPARATION)
        Load_KeypadWakeup_ToFlash(u32DbtableOffset);
#endif
        //calculate crc32 of gstDbtable
        gstDbtable.u32CrcCheck = crc32(0, (unsigned char *)&gstDbtable, sizeof(ST_DBDATA)*E_DB_MAX);
        UBOOT_DEBUG("gstDbtable.u32CrcCheck=%08x\n",gstDbtable.u32CrcCheck);

        // copy db_table to DB Pool memory buffer
        memcpy((void*)pDbBufferAddr,(const void *)&gstDbtable,sizeof(gstDbtable));

        // save DB Pool data to flash
        ret = raw_write((unsigned int)(pDbBufferAddr), u32DbtableOffset, u32Dbload_size);
        if(ret == -1)
        {
            UBOOT_ERROR("write data to flash fail...>>>\n");
        }
        else
        {
            UBOOT_TRACE("OK\n");
        }
#if defined(CONFIG_URSA20_VB1) && defined(CONFIG_URSA20_SYSBIN)
        if(MDrv_URSA_COM_Get_URSA_Type() == URSA_COMMON_U20)
        {
            Ursa_20_Upgrade_SysBin();
        }
#endif
        if(pDbBufferAddr)
        {
            free(pDbBufferAddr);
            pDbBufferAddr = NULL;
        }
        // add gop mirror set to boot arg
        #if ENABLE_DISPLAY_LOGO
        char *buf=NULL;
        buf=malloc(64);
        if(buf==NULL)
        {
            UBOOT_ERROR("malloc for buf fail\n");
            return -1;
        }
        memset(buf,0,64);
        snprintf(buf,64,"ac MIRROR %d",gstSysMiscSetting.m_u8MirrorMode);
        UBOOT_DEBUG("cmd=%s\n",buf);
        ret=run_command(buf,0);
        if(ret!=0)
        {
            UBOOT_ERROR(" Run command :%s ERROR!!! \n",buf);
            free(buf);
            return -1;
        }
        free(buf);
        #endif
        setenv("db_table","1");
        saveenv();
        ret = 0;
    }
    UBOOT_TRACE("OK\n");
    return ret;
}

void _print_dbtable_info(void)
{
    UBOOT_DEBUG("read_dbtable success...>>>\n");
    UBOOT_DEBUG("read table >>> \n");
    UBOOT_DEBUG("logo  offset[0x%08x] size[0x%08x] end[0x%08x]\n",      gstDbtable.dbdata[E_DB_LOGO].Offset,        gstDbtable.dbdata[E_DB_LOGO].Size,       (gstDbtable.dbdata[E_DB_LOGO].Offset       + gstDbtable.dbdata[E_DB_LOGO].Size));
    UBOOT_DEBUG("music offset[0x%08x] size[0x%08x] end[0x%08x]\n",      gstDbtable.dbdata[E_DB_MUSIC].Offset,       gstDbtable.dbdata[E_DB_MUSIC].Size,      (gstDbtable.dbdata[E_DB_MUSIC].Offset      + gstDbtable.dbdata[E_DB_MUSIC].Size));
    UBOOT_DEBUG("board offset[0x%08x] size[0x%08x] end[0x%08x]\n",      gstDbtable.dbdata[E_DB_BOARD_PARA].Offset,  gstDbtable.dbdata[E_DB_BOARD_PARA].Size, (gstDbtable.dbdata[E_DB_BOARD_PARA].Offset + gstDbtable.dbdata[E_DB_BOARD_PARA].Size));
    UBOOT_DEBUG("panel offset[0x%08x] size[0x%08x] end[0x%08x]\n",      gstDbtable.dbdata[E_DB_PANEL_PARA].Offset,  gstDbtable.dbdata[E_DB_PANEL_PARA].Size, (gstDbtable.dbdata[E_DB_PANEL_PARA].Offset + gstDbtable.dbdata[E_DB_PANEL_PARA].Size));
    UBOOT_DEBUG("ursa 6m30  offset[0x%08x] size[0x%08x] end[0x%08x]\n", gstDbtable.dbdata[E_DB_URSA_6M30].Offset,   gstDbtable.dbdata[E_DB_URSA_6M30].Size,  (gstDbtable.dbdata[E_DB_URSA_6M30].Offset  + gstDbtable.dbdata[E_DB_URSA_6M30].Size));
    UBOOT_DEBUG("ursa  offset[0x%08x] size[0x%08x] end[0x%08x]\n",      gstDbtable.dbdata[E_DB_URSA].Offset,        gstDbtable.dbdata[E_DB_URSA].Size,       (gstDbtable.dbdata[E_DB_URSA].Offset       + gstDbtable.dbdata[E_DB_URSA].Size));
    UBOOT_DEBUG("tcon  offset[0x%08x] size[0x%08x] end[0x%08x]\n",      gstDbtable.dbdata[E_DB_TCON].Offset,        gstDbtable.dbdata[E_DB_TCON].Size,       (gstDbtable.dbdata[E_DB_TCON].Offset       + gstDbtable.dbdata[E_DB_TCON].Size));
    UBOOT_DEBUG("vb1   offset[0x%08x] size[0x%08x] end[0x%08x]\n",      gstDbtable.dbdata[E_DB_PNL_VB1].Offset,     gstDbtable.dbdata[E_DB_PNL_VB1].Size,    (gstDbtable.dbdata[E_DB_PNL_VB1].Offset    + gstDbtable.dbdata[E_DB_PNL_VB1].Size));
    UBOOT_DEBUG("gopmux  offset[0x%08x] size[0x%08x] end[0x%08x]\n",    gstDbtable.dbdata[E_DB_PNL_GOPMUX].Offset,  gstDbtable.dbdata[E_DB_PNL_GOPMUX].Size, (gstDbtable.dbdata[E_DB_PNL_GOPMUX].Offset + gstDbtable.dbdata[E_DB_PNL_GOPMUX].Size));
    UBOOT_DEBUG("pwm   offset[0x%08x] size[0x%08x] end[0x%08x]\n",      gstDbtable.dbdata[E_DB_PWM].Offset,         gstDbtable.dbdata[E_DB_PWM].Size,        (gstDbtable.dbdata[E_DB_PWM].Offset        + gstDbtable.dbdata[E_DB_PWM].Size));
    UBOOT_DEBUG("misc  offset[0x%08x] size[0x%08x] end[0x%08x]\n",      gstDbtable.dbdata[E_DB_MISC].Offset,        gstDbtable.dbdata[E_DB_MISC].Size,       (gstDbtable.dbdata[E_DB_MISC].Offset       + gstDbtable.dbdata[E_DB_MISC].Size));
    UBOOT_DEBUG("mspi  offset[0x%08x] size[0x%08x] end[0x%08x]\n",      gstDbtable.dbdata[E_DB_MSPI].Offset,        gstDbtable.dbdata[E_DB_MSPI].Size,       (gstDbtable.dbdata[E_DB_MSPI].Offset       + gstDbtable.dbdata[E_DB_MSPI].Size));
    UBOOT_DEBUG("dma  offset[0x%08x] size[0x%08x] end[0x%08x]\n",       gstDbtable.dbdata[E_DB_DMA].Offset,         gstDbtable.dbdata[E_DB_DMA].Size,        (gstDbtable.dbdata[E_DB_DMA].Offset        + gstDbtable.dbdata[E_DB_DMA].Size));
    UBOOT_DEBUG("bootcheck  offset[0x%08x] size[0x%08x] end[0x%08x]\n", gstDbtable.dbdata[E_DB_IR_BOOTCHECK].Offset,gstDbtable.dbdata[E_DB_IR_BOOTCHECK].Size,(gstDbtable.dbdata[E_DB_IR_BOOTCHECK].Offset + gstDbtable.dbdata[E_DB_IR_BOOTCHECK].Size));
    UBOOT_DEBUG("wakeup  offset[0x%08x] size[0x%08x] end[0x%08x]\n",       gstDbtable.dbdata[E_DB_IR_WAKEUP].Offset,   gstDbtable.dbdata[E_DB_IR_WAKEUP].Size,  (gstDbtable.dbdata[E_DB_IR_WAKEUP].Offset  + gstDbtable.dbdata[E_DB_IR_WAKEUP].Size));
    UBOOT_DEBUG("rgbw  offset[0x%08x] size[0x%08x] end[0x%08x]\n",      gstDbtable.dbdata[E_DB_RGBW].Offset,        gstDbtable.dbdata[E_DB_RGBW].Size,       (gstDbtable.dbdata[E_DB_RGBW].Offset       + gstDbtable.dbdata[E_DB_RGBW].Size));
    UBOOT_DEBUG("keypad  offset[0x%08x] size[0x%08x] end[0x%08x]\n",    gstDbtable.dbdata[E_DB_KEYPAD].Offset,      gstDbtable.dbdata[E_DB_KEYPAD].Size,     (gstDbtable.dbdata[E_DB_KEYPAD].Offset     + gstDbtable.dbdata[E_DB_KEYPAD].Size));
    UBOOT_DEBUG("3dlut  offset[0x%08x] size[0x%08x] end[0x%08x]\n",    gstDbtable.dbdata[E_DB_3DLUT].Offset,      gstDbtable.dbdata[E_DB_3DLUT].Size,     (gstDbtable.dbdata[E_DB_3DLUT].Offset     + gstDbtable.dbdata[E_DB_3DLUT].Size));
#if defined(CONFIG_DATA_SEPARATION)
    UBOOT_DEBUG("keypad wakeup  offset[0x%08x] size[0x%08x] end[0x%08x]\n",       gstDbtable.dbdata[E_DB_KEYPAD_WAKEUP].Offset,   gstDbtable.dbdata[E_DB_KEYPAD_WAKEUP].Size,  (gstDbtable.dbdata[E_DB_KEYPAD_WAKEUP].Offset  + gstDbtable.dbdata[E_DB_KEYPAD_WAKEUP].Size));
#endif
#if defined(CONFIG_VOICE_CM4)
    UBOOT_DEBUG("Mic config  offset[0x%08x] size[0x%08x] end[0x%08x]\n",       gstDbtable.dbdata[E_DB_MIC_CONFIG].Offset,   gstDbtable.dbdata[E_DB_MIC_CONFIG].Size,  (gstDbtable.dbdata[E_DB_MIC_CONFIG].Offset  + gstDbtable.dbdata[E_DB_MIC_CONFIG].Size));
#endif
}
int read_dbtable(U32 u32DbtableOffset)
{
    if(-1 == raw_read((U32)&gstDbtable,u32DbtableOffset,sizeof(gstDbtable)))
    {
        UBOOT_ERROR("ERROR: read_mmc_dbtable fail...>>>\n");
        return -1;
    }
    else
    {
        _print_dbtable_info();
    }
    U32 checksum = 0,savesum = 0;
    checksum = crc32(0, (unsigned char *)&gstDbtable, sizeof(ST_DBDATA)*E_DB_MAX);
    savesum = gstDbtable.u32CrcCheck;

    if(checksum != savesum)
    {
        UBOOT_ERROR("Error : savesum =%08x , checksum= %08x \n",savesum,checksum);
        UBOOT_ERROR("Reload the dbtable from AP !!!\n");
        return write_dbtable(u32DbtableOffset);
    }
    return 0;
}


void count_data_crc32_save(U32 addr,int size,DB_INDEX db_index)
{
    gstDbtable.dbdata[db_index].u32CrcCheck = crc32(0,(unsigned char *)addr,size);
    UBOOT_DEBUG("%s checksum =: 0x%08x\n",sDB_INDEX[db_index],gstDbtable.dbdata[db_index].u32CrcCheck);
}

int count_data_crc32_cmp(U32 addr,int size, DB_INDEX db_index)
{
    int ret = -1;
    U32 checksum,savesum;
    checksum = crc32(0,(unsigned char *)addr,size);

    savesum = gstDbtable.dbdata[db_index].u32CrcCheck;

    if(checksum == savesum)
    {
        UBOOT_DEBUG("## CRC check done, %s\n", sDB_INDEX[db_index]);
        ret = 0;
    }
    else
    {
        UBOOT_ERROR("checksum :0X%x ; savesum :0X%x\n",checksum,savesum);
        UBOOT_ERROR("## CRC check Error, %s\n", sDB_INDEX[db_index]);
        setenv("db_table","0");
        saveenv();
    }
    return ret;
}

int Read_FlashDataToAddr(U32 u32DstAddr, DB_INDEX db_index)
{
    int ret = -1;
    UBOOT_TRACE("IN\n");
    UBOOT_DEBUG("gstDbtable.dbdata[%s] Offset =:0x%x ... Size=: 0x%x\n",
                sDB_INDEX[db_index],
                gstDbtable.dbdata[db_index].Offset,
                gstDbtable.dbdata[db_index].Size);

    if(-1 == raw_read(u32DstAddr, gstDbtable.dbdata[db_index].Offset, gstDbtable.dbdata[db_index].Size))
    {
        UBOOT_ERROR("ERROR: Read_FlashDataToAddr fail...>>>\n");
    }
    else
    {
        UBOOT_DEBUG("Read_FlashDataToAddr success...>>>\n");
        //crc check
        if(count_data_crc32_cmp(u32DstAddr,gstDbtable.dbdata[db_index].Size,db_index)==-1)
        {
            UBOOT_ERROR("check FlashDataToAddr fail..>>>\n");
            return ret;
        }
        else
        {
            ret = 0;
        }
    }
    UBOOT_TRACE("OK\n");
    return ret;
}

int Load_DataToDbBuffer(U32 u32DbtableOffset, DB_INDEX db_index, U32 pu32DataAddr)
{
    UBOOT_TRACE("IN\n");
    UBOOT_DEBUG("gstDbtable.dbdata[%s] Offset =:0x%x ... Size=: 0x%x\n",
                sDB_INDEX[db_index],
                gstDbtable.dbdata[db_index].Offset,
                gstDbtable.dbdata[db_index].Size);
    // copy data to db pool memory buffer
    memcpy( (void*)(pDbBufferAddr + MPoolVolumeOffset(gstDbtable.dbdata[db_index].Offset,u32DbtableOffset)),
            (void*)pu32DataAddr,
            gstDbtable.dbdata[db_index].Size);

    count_data_crc32_save(pu32DataAddr,gstDbtable.dbdata[db_index].Size,db_index);
    UBOOT_TRACE("OK\n");
    return 0;
}

#if ENABLE_DISPLAY_LOGO
int Load_LogoToFlash(U32 u32DbtableOffset)
{
    int ret = -1;
    unsigned int *pu32LogoAddr=NULL;
    unsigned int u32fileSize   = 0;
    char *path, *logoPath = NULL;
    char old_volName[32];
    char *new_volName = NULL, *volName;
    char subdir[CONFIG_SYS_CBSIZE];
    unsigned int sync_bootlogo_ok = 0;
#if defined(CONFIG_DATA_SEPARATION)
    char * pEnv=NULL;
#endif
    UBOOT_TRACE("IN\n");

    path = gIniBootLogo;
    if (path != NULL && (strlen(path) > 0))
    {
        UBOOT_INFO("path = %s\n", path);
        volName = vfs_get_volname();
        strncpy(old_volName, volName, 31);
        ret = vfs_mount_by_abspath(path, subdir, sizeof(subdir));
        new_volName = vfs_get_volname();
        if (ret != 0)
        {
            UBOOT_ERROR("vfs_mount_by_abspath %s failed, ret = %d\n", path, ret);
            if ((strlen(old_volName) > 0) && (strcmp(old_volName, new_volName) != 0))
            {
                UBOOT_DEBUG("old_volNmae = %s\n", old_volName);
                vfs_mount(old_volName);
            }
            return -1;
        }
        u32fileSize=vfs_getsize(subdir);
        if(0 == u32fileSize)
        {
            UBOOT_ERROR("vfs_getsize error: %s !\n", subdir);
            return -1;
        }

        logoPath = subdir;
        sync_bootlogo_ok = 1;
    }
    else
    {
#if defined(CONFIG_DATA_SEPARATION)
        pEnv=getenv(ENV_CUSDATA);
        if (pEnv !=NULL)
        {
            UBOOT_DEBUG("Get MAP ENV_CUSDATA from env =%s \n",pEnv);
            if(vfs_mount(pEnv)!=0)
            {
                UBOOT_ERROR("mount %s fail\n",pEnv);
                goto exit;
            }
        }
        else
        {
            if(vfs_mount(CUSDATA)!=0)  //use default PATH /cusdata
            {
                UBOOT_ERROR("mount %s fail\n",CONFIG);
                goto exit;
            }
        }
#else
        vfs_mount(CUSTOMER);
#endif
        u32fileSize=vfs_getsize(gLogoPath);
        if(0 == u32fileSize)
        {
            UBOOT_ERROR("vfs_getsize error: %s !\n", gLogoPath);
            return -1;
        }
        logoPath = gLogoPath;
    }
    UBOOT_DEBUG("u32fileSize=0x%x;gstDbtable.dbdata[E_DB_LOGO].Size=0x%x\n",u32fileSize,gstDbtable.dbdata[E_DB_LOGO].Size);
    pu32LogoAddr=malloc(u32fileSize);
    if(pu32LogoAddr == NULL)
    {
        UBOOT_ERROR("malloc pu32LogoAddr memory error !");
        return -1;
    }
    memset((void*)pu32LogoAddr,0,u32fileSize);
    if (vfs_read((void *)pu32LogoAddr,logoPath,0,u32fileSize) == 0)
    {
        ret = Load_DataToDbBuffer(u32DbtableOffset, E_DB_LOGO, (U32)pu32LogoAddr);
    }
    else
    {
        UBOOT_ERROR("ubifsload data fail ...>>>\n");
    }

    free(pu32LogoAddr);
    pu32LogoAddr = NULL;

    if (sync_bootlogo_ok == 1)
    {
        setenv("_BootlogoFile",path);
        if (run_command("setenv BootlogoFile", 0) != 0)
        {
            UBOOT_ERROR("run command fail : setenv BootlogoFile\n");
        }
        if ((strlen(old_volName) > 0) && (strcmp(old_volName, new_volName) != 0))
        {
            UBOOT_DEBUG("old_volNmae = %s\n", old_volName);
            vfs_mount(old_volName);
        }
    }

exit:
    UBOOT_TRACE("OK\n");
    return ret;
}

int Read_LogoToDisplayAddr(U32 u32LogoAddr)
{
    int ret = -1;
    ret = Read_FlashDataToAddr(u32LogoAddr, E_DB_LOGO);
    return ret;
}
#endif

#if ENABLE_POWER_MUSIC
int Load_MusicToFlash(U32 u32DbtableOffset)
{
    int ret = -1;
    unsigned int *pu32MusicAddr=NULL;
    U32 u32fileSize=0;
#if defined(CONFIG_DATA_SEPARATION)
    char * pEnv=NULL;
#endif
    UBOOT_TRACE("IN\n");
#if defined(CONFIG_DATA_SEPARATION)
    pEnv=getenv(ENV_CUSDATA);
    if (pEnv !=NULL)
    {
        UBOOT_DEBUG("Get MAP ENV_CUSDATA from env =%s \n",pEnv);
        if(vfs_mount(pEnv)!=0)
        {
            UBOOT_ERROR("mount %s fail\n",pEnv);
            goto exit;
        }
    }
    else
    {
        if(vfs_mount(CUSDATA)!=0)  //use default PATH /cusdata
        {
            UBOOT_ERROR("mount %s fail\n",CONFIG);
            goto exit;
        }
    }
#else
    vfs_mount(CUSTOMER);
#endif
    u32fileSize=vfs_getsize(gMusicPath);
    if(0 == u32fileSize)
    {
        UBOOT_ERROR("vfs_getsize error: %s !\n", gMusicPath);
        return -1;
    }
    UBOOT_DEBUG("u32fileSize=0x%x;gstDbtable.dbdata[E_DB_MUSIC].Size=0x%x\n",u32fileSize,gstDbtable.dbdata[E_DB_MUSIC].Size);
    pu32MusicAddr = malloc(u32fileSize);
    if(pu32MusicAddr == NULL)
    {
        UBOOT_ERROR("malloc pu32MusicAddr memory error !");
        return -1;
    }
    memset((void*)pu32MusicAddr,0,u32fileSize);
    if (vfs_read((void *)pu32MusicAddr,gMusicPath,0,u32fileSize) == 0)
    {
        ret = Load_DataToDbBuffer(u32DbtableOffset, E_DB_MUSIC, (U32)pu32MusicAddr);
    }
    else
    {
        UBOOT_ERROR("load mp3 file faile>>>>>>>>>>\n");
    }
    free(pu32MusicAddr);
    pu32MusicAddr = NULL;

exit:
    UBOOT_TRACE("OK\n");
    return ret;
}

int Read_MusicToPlayAddr(MS_PHYADDR MusicDramAddr)
{
    int ret = -1;
    U32 u32MusicAddr = (U32)(PA2NVA(MusicDramAddr));
    // Clear ES1 buffer = 64K
    memset((void*)(u32MusicAddr), 0, 65536);
    ret = Read_FlashDataToAddr(u32MusicAddr, E_DB_MUSIC);
    return ret;
}

#endif

#if (CONFIG_PANEL_INIT)
int parse_pnl_ini(char *path,PanelType *p_data)
{
    long n;
    char *script = NULL;
    char str[BUFFER_SIZE];
    U32 filesize = 0;
    script = loadscript(path,&filesize);
    if(script == NULL)
    {
        while (*path== '/') path++;
        path=strstr(path,"/");
        UBOOT_INFO("filter /cusdata or /config, and find file again!!\n");
        script = loadscript(path,&filesize);
        if(script == NULL)
        {
            return -1;
        }
    }
    UBOOT_INFO("sizeof( panel ini script ) --------> %u bytes\n",filesize);
    Profile_Init((char *)script,filesize);
    n = Profile_GetInteger("panel", "m_bPanelDither", 0);
    UBOOT_DEBUG("m_bPanelDither = %ld \n",n);
    p_data->m_bPanelDither = n;
    memset(str,0,sizeof(str));
    Profile_GetString("panel", "m_pPanelName","", str, sizearray(str));

    n = Profile_GetInteger("panel", "m_ePanelLinkType", 0);
    UBOOT_DEBUG("m_ePanelLinkType = %ld \n",n);
    p_data->m_ePanelLinkType = (APIPNL_LINK_TYPE)n;
    //Profile_GetString("panel", "m_ePanelLinkExtType","", str, sizearray(str));
    n = Profile_GetInteger("panel", "m_ePanelLinkExtType", 0);
    UBOOT_DEBUG("m_ePanelLinkExtType = %ld \n",n);
//    setenv("panel_ext_type", str);
    gstSysMiscSetting.m_u16Panel_ext_type = n;
    Profile_GetString("panel", "m_u16PANEL_LVDS_CONNECT_TYPE","", str, sizearray(str));
    UBOOT_DEBUG("m_u16PANEL_LVDS_CONNECT_TYPE = %s \n",str);
    memset(gstSysMiscSetting.m_Panel_LVDS_Connect_Type, 0, sizeof(gstSysMiscSetting.m_Panel_LVDS_Connect_Type));
    strncpy(gstSysMiscSetting.m_Panel_LVDS_Connect_Type, str, sizeof(gstSysMiscSetting.m_Panel_LVDS_Connect_Type)-1);
    //saveenv();

    n = Profile_GetInteger("panel", "m_bPanelDualPort", 0);
    UBOOT_DEBUG("m_bPanelDualPort = %ld \n",n);
    p_data->m_bPanelDualPort = n;
    n = Profile_GetInteger("panel", "m_bPanelSwapPort", 0);
    UBOOT_DEBUG("m_bPanelSwapPort  = %ld \n",n);
    p_data->m_bPanelSwapPort = n;
    n = Profile_GetInteger("panel", "m_bPanelSwapOdd_ML", 0);
    UBOOT_DEBUG("m_bPanelSwapOdd_ML = %ld \n",n);
    p_data->m_bPanelSwapOdd_ML  = n;
    n = Profile_GetInteger("panel", "m_bPanelSwapEven_ML", 0);
    UBOOT_DEBUG("m_bPanelSwapEven_ML = %ld \n",n);
    p_data->m_bPanelSwapEven_ML = n;
    n = Profile_GetInteger("panel", "m_bPanelSwapOdd_RB", 0);
    UBOOT_DEBUG("m_bPanelSwapOdd_RB = %ld \n",n);
    p_data->m_bPanelSwapOdd_RB = n;
    n = Profile_GetInteger("panel", "m_bPanelSwapEven_RB", 0);
    UBOOT_DEBUG("m_bPanelSwapEven_RB = %ld \n",n);
    p_data->m_bPanelSwapEven_RB = n;
    n = Profile_GetInteger("panel", "m_bPanelSwapLVDS_POL", 0);
    UBOOT_DEBUG("m_bPanelSwapLVDS_POL = %ld \n",n);
    p_data->m_bPanelSwapLVDS_POL = n;
    n = Profile_GetInteger("panel", "m_bPanelSwapLVDS_CH", 0);
    UBOOT_DEBUG("m_bPanelSwapLVDS_CH = %ld \n",n);
    p_data->m_bPanelSwapLVDS_CH = n;
    n = Profile_GetInteger("panel", "m_bPanelPDP10BIT", 0);
    UBOOT_DEBUG("m_bPanelPDP10BIT = %ld \n",n);
    p_data->m_bPanelPDP10BIT = n;
    n = Profile_GetInteger("panel", "m_bPanelLVDS_TI_MODE", 0);
    UBOOT_DEBUG("m_bPanelLVDS_TI_MODE = %ld \n",n);
    p_data->m_bPanelLVDS_TI_MODE = n;
    n = Profile_GetInteger("panel", "m_ucPanelDCLKDelay", 0);
    UBOOT_DEBUG("m_ucPanelDCLKDelay = %ld \n",n);
    p_data->m_ucPanelDCLKDelay = n;
    n = Profile_GetInteger("panel", "m_bPanelInvDCLK", 0);
    UBOOT_DEBUG("m_bPanelInvDCLK = %ld \n",n);
    p_data->m_bPanelInvDCLK = n;
    n = Profile_GetInteger("panel", "m_bPanelInvDE", 0);
    UBOOT_DEBUG("m_bPanelInvDE = %ld \n",n);
    p_data->m_bPanelInvDE = n;
    n = Profile_GetInteger("panel", "m_bPanelInvHSync", 0);
    UBOOT_DEBUG("m_bPanelInvHSync = %ld \n",n);
    p_data->m_bPanelInvHSync = n;
    n = Profile_GetInteger("panel", "m_bPanelInvVSync", 0);
    UBOOT_DEBUG("m_bPanelInvVSync = %ld \n",n);
    p_data->m_bPanelInvVSync = n;
    n = Profile_GetInteger("panel", "m_ucPanelDCKLCurrent", 0);
    UBOOT_DEBUG("m_ucPanelDCKLCurrent = %ld \n",n);
    p_data->m_ucPanelDCKLCurrent = n;
    n = Profile_GetInteger("panel", "m_ucPanelDECurrent", 0);
    UBOOT_DEBUG("m_ucPanelDECurrent = %ld \n",n);
    p_data->m_ucPanelDECurrent = n;
    n = Profile_GetInteger("panel", "m_ucPanelODDDataCurrent", 0);
    UBOOT_DEBUG("m_ucPanelODDDataCurrent = %ld \n",n);
    p_data->m_ucPanelODDDataCurrent = n;
    n = Profile_GetInteger("panel", "m_ucPanelEvenDataCurrent", 0);
    UBOOT_DEBUG("m_ucPanelEvenDataCurrent = %ld \n",n);
    p_data->m_ucPanelEvenDataCurrent = n;
    n = Profile_GetInteger("panel", "m_wPanelOnTiming1", 0);
    UBOOT_DEBUG("m_wPanelOnTiming1 = %ld \n",n);
    p_data->m_wPanelOnTiming1 = n;
    n = Profile_GetInteger("panel", "m_wPanelOnTiming2", 0);
    UBOOT_DEBUG("m_wPanelOnTiming2 = %ld \n",n);
    p_data->m_wPanelOnTiming2 = n;

#if defined(CONFIG_PNL_OLED_LG_SUPPORT) //the panel.ini format is different
    n = Profile_GetInteger("panel", "m_wPanelOnTiming3", 0);
    UBOOT_DEBUG("m_wPanelOnTiming3 = %ld\n",n);
    gstPNL_OLED_SpecificParameter.m_u16on_timing3 = n;
#endif

    n = Profile_GetInteger("panel", "m_wPanelOffTiming1", 0);
    UBOOT_DEBUG("m_wPanelOffTiming1 = %ld \n",n);
    p_data->m_wPanelOffTiming1 = n;
    n = Profile_GetInteger("panel", "m_wPanelOffTiming2", 0);
    UBOOT_DEBUG("m_wPanelOffTiming2 = %ld \n",n);
    p_data->m_wPanelOffTiming2 = n;

#if defined(CONFIG_PNL_OLED_LG_SUPPORT) //the panel.ini format is different
    n = Profile_GetInteger("panel", "m_wPanelOffTiming3", 0);
    UBOOT_DEBUG("m_wPanelOffTiming3 = %ld\n",n);
    gstPNL_OLED_SpecificParameter.m_u16off_timing3 = n;
#endif
    n = Profile_GetInteger("panel", "m_ucPanelHSyncWidth", 0);
    UBOOT_DEBUG("m_ucPanelHSyncWidth  = %ld \n",n);
    p_data->m_ucPanelHSyncWidth = n;
    n = Profile_GetInteger("panel", "m_ucPanelHSyncBackPorch", 0);
    UBOOT_DEBUG("m_ucPanelHSyncBackPorch  = %ld \n",n);
    p_data->m_ucPanelHSyncBackPorch = n;

    n = Profile_GetInteger("panel", "m_ucPanelVSyncWidth", 0);
    UBOOT_DEBUG("m_ucPanelVSyncWidth = %ld \n",n);
    p_data->m_ucPanelVSyncWidth = n;
    n = Profile_GetInteger("panel", "m_ucPanelVBackPorch", 0);
    UBOOT_DEBUG("m_ucPanelVBackPorch = %ld \n",n);
    p_data->m_ucPanelVBackPorch = n;

    n = Profile_GetInteger("panel", "m_wPanelHStart", 0);
    UBOOT_DEBUG("m_wPanelHStart = %ld \n",n);
    p_data->m_wPanelHStart = n;
    n = Profile_GetInteger("panel", "m_wPanelVStart", 0);
    UBOOT_DEBUG("m_wPanelVStart = %ld \n",n);
    p_data->m_wPanelVStart = n;
    n = Profile_GetInteger("panel", "m_wPanelWidth", 0);
    UBOOT_DEBUG("m_wPanelWidth = %ld \n",n);
    p_data->m_wPanelWidth = n;
    n = Profile_GetInteger("panel", "m_wPanelHeight", 0);
    UBOOT_DEBUG("m_wPanelHeight = %ld \n",n);
    p_data->m_wPanelHeight = n;

    n = Profile_GetInteger("panel", "m_wPanelMaxHTotal", 0);
    UBOOT_DEBUG("m_wPanelMaxHTotal = %ld \n",n);
    p_data->m_wPanelMaxHTotal = n;
    n = Profile_GetInteger("panel", "m_wPanelHTotal", 0);
    UBOOT_DEBUG("m_wPanelHTotal = %ld \n",n);
    p_data->m_wPanelHTotal = n;
    n = Profile_GetInteger("panel", "m_wPanelMinHTotal", 0);
    UBOOT_DEBUG("m_wPanelMinHTotal = %ld \n",n);
    p_data->m_wPanelMinHTotal = n;
    n = Profile_GetInteger("panel", "m_wPanelMaxVTotal", 0);
    UBOOT_DEBUG("m_wPanelMaxVTotal = %ld \n",n);
    p_data->m_wPanelMaxVTotal = n;
    n = Profile_GetInteger("panel", "m_wPanelVTotal", 0);
    UBOOT_DEBUG("m_wPanelVTotal = %ld \n",n);
    p_data->m_wPanelVTotal = n;

    n = Profile_GetInteger("panel", "m_wPanelMinVTotal", 0);
    UBOOT_DEBUG("m_wPanelMinVTotal = %ld \n",n);
    p_data->m_wPanelMinVTotal = n;
    n = Profile_GetInteger("panel", "m_dwPanelMaxDCLK", 0);
    UBOOT_DEBUG("m_dwPanelMaxDCLK = %ld \n",n);
    p_data->m_dwPanelMaxDCLK = n;
    n = Profile_GetInteger("panel", "m_dwPanelDCLK", 0);
    UBOOT_DEBUG("m_dwPanelDCLK = %ld \n",n);
    p_data->m_dwPanelDCLK = n;
    gstSysMiscSetting.m_u16PanelDCLK = n;

    n = Profile_GetInteger("panel", "m_dwPanelMinDCLK", 0);
    UBOOT_DEBUG("m_dwPanelMinDCLK = %ld \n",n);
    p_data->m_dwPanelMinDCLK = n;
    n = Profile_GetInteger("panel", "m_wSpreadSpectrumStep", 0);
    UBOOT_DEBUG("m_wSpreadSpectrumStep = %ld \n",n);
    p_data->m_wSpreadSpectrumStep = n;
    n = Profile_GetInteger("panel", "m_wSpreadSpectrumSpan", 0);
    UBOOT_DEBUG("m_wSpreadSpectrumSpan = %ld \n",n);
    p_data->m_wSpreadSpectrumSpan = n;
    n = Profile_GetInteger("panel", "m_ucDimmingCtl", 0);
    UBOOT_DEBUG("m_ucDimmingCtl = %ld \n",n);
    p_data->m_ucDimmingCtl = n;
    n = Profile_GetInteger("panel", "m_ucMaxPWMVal", 0);
    UBOOT_DEBUG("m_ucMaxPWMVal = %ld \n",n);
    p_data->m_ucMaxPWMVal = n;
    n = Profile_GetInteger("panel", "m_ucMinPWMVal", 0);
    UBOOT_DEBUG("m_ucMinPWMVal = %ld \n",n);
    p_data->m_ucMinPWMVal = n;
    n = Profile_GetInteger("panel", "m_bPanelDeinterMode", 0);
    UBOOT_DEBUG("m_bPanelDeinterMode = %ld \n",n);
    p_data->m_bPanelDeinterMode = n;
    n = Profile_GetInteger("panel", "m_ucPanelAspectRatio", 0);
    UBOOT_DEBUG("m_ucPanelAspectRatio = %ld \n",n);
    p_data->m_ucPanelAspectRatio = (E_PNL_ASPECT_RATIO)n;

    n = Profile_GetInteger("panel", "m_u16LVDSTxSwapValue", 0);
    UBOOT_DEBUG("m_u16LVDSTxSwapValue = %ld \n",n);
    p_data->m_u16LVDSTxSwapValue = n;
    n = Profile_GetInteger("panel", "m_ucTiBitMode", 0);
    UBOOT_DEBUG("m_ucTiBitMode = %ld \n",n);
    p_data->m_ucTiBitMode = (APIPNL_TIBITMODE)n;

    n = Profile_GetInteger("panel", "m_ucOutputFormatBitMode", 0);
    UBOOT_DEBUG("m_ucOutputFormatBitMode = %ld \n",n);
    p_data->m_ucOutputFormatBitMode = (APIPNL_OUTPUTFORMAT_BITMODE)n;

    n = Profile_GetInteger("panel", "m_bPanelSwapOdd_RG", 0);
    UBOOT_DEBUG("m_bPanelSwapOdd_RG = %ld \n",n);
    p_data->m_bPanelSwapOdd_RG = n;
    n = Profile_GetInteger("panel", "m_bPanelSwapEven_RG", 0);
    UBOOT_DEBUG("m_bPanelSwapEven_RG = %ld \n",n);
    p_data->m_bPanelSwapEven_RG = n;
    n = Profile_GetInteger("panel", "m_bPanelSwapOdd_GB", 0);
    UBOOT_DEBUG("m_bPanelSwapOdd_GB = %ld \n",n);
    p_data->m_bPanelSwapOdd_GB = n;
    n = Profile_GetInteger("panel", "m_bPanelSwapEven_GB", 0);
    UBOOT_DEBUG("m_bPanelSwapEven_GB = %ld \n",n);
    p_data->m_bPanelSwapEven_GB = n;
    n = Profile_GetInteger("panel", "m_bPanelDoubleClk", 0);
    UBOOT_DEBUG("m_bPanelDoubleClk = %ld \n",n);
    p_data->m_bPanelDoubleClk = n;
    n = Profile_GetInteger("panel", "m_dwPanelMaxSET", 0);
    UBOOT_DEBUG("m_dwPanelMaxSET = %ld \n",n);
    p_data->m_dwPanelMaxSET = n;
    n = Profile_GetInteger("panel", "m_dwPanelMinSET", 0);
    UBOOT_DEBUG("m_dwPanelMinSET = %ld \n",n);
    p_data->m_dwPanelMinSET = n;
    n = Profile_GetInteger("panel", "m_ucOutTimingMode", 0);
    UBOOT_DEBUG("m_ucOutTimingMode = %ld \n",n);
    p_data->m_ucOutTimingMode = (APIPNL_OUT_TIMING_MODE)n;
#if 1//use pwm setting in panel.ini and save it to env
#if defined(CONFIG_PNL_OLED_LG_SUPPORT) //the panel.ini format is different
    n = Profile_GetInteger("panel", "m_eBacklightType", 0);
    UBOOT_DEBUG("m_eBacklightType = %ld\n",n);
    gstPNL_OLED_SpecificParameter.m_eBacklightType = n;
#endif

    n = Profile_GetInteger("panel", "u32PeriodPWM", 0);
    UBOOT_INFO("u32PeriodPWM = %ld \n",n);
    gstPWMSetting.m_u32PWMPeriod = n;


    n = Profile_GetInteger("panel", "u16DivPWM", 0);
    UBOOT_DEBUG("u16DivPWM = %ld \n",n);
    gstPWMSetting.m_u16DivPWM = n;

    memset(str,0,sizeof(str));
    n = Profile_GetInteger("panel", "u32DutyPWM", 0);// Duty use Div value,fix it later
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_INFO("u32DutyPWM = %s\n",str);
    gstPWMSetting.m_u32PWMDuty = n;

    memset(str,0,sizeof(str));
    n = Profile_GetInteger("panel", "bPolPWM", 0);// Duty use polpwm value,fix it later
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("bPolPWM = %s\n",str);
    gstPWMSetting.m_bPolPWM = n;

    memset(str,0,sizeof(str));
    n = Profile_GetInteger("panel", "u16MaxPWMvalue", 0);// Duty use u16MaxPWMvalue value,fix it later
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("u16MaxPWMvalue = %s\n",str);
    gstPWMSetting.m_u16maxPWM = n;

    memset(str,0,sizeof(str));
    n = Profile_GetInteger("panel", "u16MaxPWMvalueMsb", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("u16MaxPWMvalueMsb = %s\n",str);
    gstPWMSetting.m_u16maxPWMMsb = n;

    memset(str,0,sizeof(str));
    n = Profile_GetInteger("panel", "u16MinPWMvalue", 0);// Duty use u16MinPWMvalue value,fix it later
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("u16MinPWMvalue = %s\n",str);
    gstPWMSetting.m_u16minPWM = n;
    memset(str,0,sizeof(str));

    memset(str,0,sizeof(str));
    n = Profile_GetInteger("panel", "u16MinPWMvalueMsb", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("u16MinPWMvalueMsb = %s\n",str);
    gstPWMSetting.m_u16minPWMMsb = n;
    memset(str,0,sizeof(str));

#endif

#if(CONFIG_ENABLE_V_I_PWM == 1)
    // I_PWM
    memset(str,0,sizeof(str));
    n = Profile_GetInteger("panel", "I_PWM_ENABLE", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_INFO("I_PWM_ENABLE = %ld \n",n);
    setenv(I_PWM_ENABLE,str);

    memset(str,0,sizeof(str));
    n = Profile_GetInteger("panel", "I_u8PWM_CH", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_INFO("I_u8PWM_CH = %ld \n",n);
    setenv(I_PWM_CH,str);

    memset(str,0,sizeof(str));
    n = Profile_GetInteger("panel", "I_u32PeriodPWM", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_INFO("I_u32PeriodPWM = %ld \n",n);
    setenv(I_PWM_PERIOD,str);

    memset(str,0,sizeof(str));
    n = Profile_GetInteger("panel", "I_u16DivPWM", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("I_u16DivPWM = %ld \n",n);
    setenv(I_PWM_DIVPWM,str);

    memset(str,0,sizeof(str));
    n = Profile_GetInteger("panel", "I_u32DutyPWM", 0);// Duty use Div value,fix it later
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_INFO("I_u32DutyPWM = %s\n",str);
    setenv(I_PWM_DUTY,str);

    memset(str,0,sizeof(str));
    n = Profile_GetInteger("panel", "I_bPolPWM", 0);// Duty use polpwm value,fix it later
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("I_bPolPWM = %s\n",str);
    setenv(I_PWM_POLPWM,str);

    memset(str,0,sizeof(str));
    n = Profile_GetInteger("panel", "I_u16MaxPWMvalue", 0);// Duty use u16MaxPWMvalue value,fix it later
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("I_u16MaxPWMvalue = %s\n",str);
    setenv(I_PWM_MAXPWM,str);

    memset(str,0,sizeof(str));
    n = Profile_GetInteger("panel", "I_u16MinPWMvalue", 0);// Duty use u16MinPWMvalue value,fix it later
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("I_u16MinPWMvalue = %s\n",str);
    setenv(I_PWM_MINPWM,str);
    memset(str,0,sizeof(str));

    // V_PWM
    memset(str,0,sizeof(str));
    n = Profile_GetInteger("panel",  "V_PWM_ENABLE", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_INFO("V_PWM_ENABLE = %ld \n",n);
    setenv(V_PWM_ENABLE,str);

    memset(str,0,sizeof(str));
    n = Profile_GetInteger("panel", "V_u8PWM_CH", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_INFO("V_u8PWM_CH = %ld \n",n);
    setenv(V_PWM_CH,str);

    memset(str,0,sizeof(str));
    n = Profile_GetInteger("panel", "V_u32PeriodPWM", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_INFO("V_u32PeriodPWM = %ld \n",n);
    setenv(V_PWM_PERIOD,str);

    memset(str,0,sizeof(str));
    n = Profile_GetInteger("panel", "V_u16DivPWM", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("V_u16DivPWM = %ld \n",n);
    setenv(V_PWM_DIVPWM,str);

    memset(str,0,sizeof(str));
    n = Profile_GetInteger("panel", "V_u32DutyPWM", 0);// Duty use Div value,fix it later
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_INFO("V_u32DutyPWM = %s\n",str);
    setenv(V_PWM_DUTY,str);

    memset(str,0,sizeof(str));
    n = Profile_GetInteger("panel", "I_bPolPWM", 0);// Duty use polpwm value,fix it later
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("V_bPolPWM = %s\n",str);
    setenv(V_PWM_POLPWM,str);

    memset(str,0,sizeof(str));
    n = Profile_GetInteger("panel", "V_u16MaxPWMvalue", 0);// Duty use u16MaxPWMvalue value,fix it later
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("V_u16MaxPWMvalue = %s\n",str);
    setenv(V_PWM_MAXPWM,str);

    memset(str,0,sizeof(str));
    n = Profile_GetInteger("panel", "V_u16MinPWMvalue", 0);// Duty use u16MinPWMvalue value,fix it later
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("V_u16MinPWMvalue = %s\n",str);
    setenv(V_PWM_MINPWM,str);

    memset(str,0,sizeof(str));


    memset(str,0,sizeof(str));
    Profile_GetString("panel", "PNL_VB1_Control","false", str, sizearray(str));
    setenv("PNL_VB1_Control",str);

    memset(str,0,sizeof(str));
#endif
#if defined(CONFIG_URSA_TXRX_SUPPORT)
    memset(str,0,sizeof(str));
    /* Ursa TX panel parameter */
    n = Profile_GetInteger("FRC_Chip", "emfc_lvds_rx_channel", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("emfc_lvds_rx_channel = %s\n",str);
    gstUrsaRXTX_PanelParameter.m_eMfc_lvds_rx_channel = n;
    n = Profile_GetInteger("FRC_Chip", "emfc_lvds_rx_bit_num", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("emfc_lvds_rx_bit_num = %s\n",str);
    gstUrsaRXTX_PanelParameter.m_eMfc_lvds_rx_bit_num = n;
    n = Profile_GetInteger("FRC_Chip", "emfc_lvds_rx_ti_mode", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("emfc_lvds_rx_ti_mode = %s\n",str);
    gstUrsaRXTX_PanelParameter.m_eMfc_lvds_rx_ti_mode = n;
    n = Profile_GetInteger("FRC_Chip", "bmfc_lvds_rx_swap_msb_lsb", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("bmfc_lvds_rx_swap_msb_lsb = %s\n",str);
    gstUrsaRXTX_PanelParameter.m_bMfc_lvds_rx_swap_msb_lsb = n;
    n = Profile_GetInteger("FRC_Chip", "bmfc_lvds_rx_swap_p_n", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("bmfc_lvds_rx_swap_p_n = %s\n",str);
    gstUrsaRXTX_PanelParameter.m_bMfc_lvds_rx_swap_p_n = n;
    n = Profile_GetInteger("FRC_Chip", "bmfc_lvds_rx_swap_odd_even", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("bmfc_lvds_rx_swap_odd_even = %s\n",str);
    gstUrsaRXTX_PanelParameter.m_bMfc_lvds_rx_swap_odd_even = n;
    n = Profile_GetInteger("FRC_Chip", "u16mfc_panel_htotal", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("u16mfc_panel_htotal = %s\n",str);
    gstUrsaRXTX_PanelParameter.m_u16Mfc_panel_htotal = n;
    n = Profile_GetInteger("FRC_Chip", "u16mfc_panel_vtotal", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("u16mfc_panel_vtotal = %s\n",str);
    gstUrsaRXTX_PanelParameter.m_u16Mfc_panel_vtotal = n;
    n = Profile_GetInteger("FRC_Chip", "u16mfc_panel_vstart", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("u16mfc_panel_vstart = %s\n",str);
    gstUrsaRXTX_PanelParameter.m_u16Mfc_panel_vstart = n;
    n = Profile_GetInteger("FRC_Chip", "u16mfc_panel_hstart", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("u16mfc_panel_hstart = %s\n",str);
    gstUrsaRXTX_PanelParameter.m_u16Mfc_panel_hstart = n;
    n = Profile_GetInteger("FRC_Chip", "u16mfc_panel_height", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("u16mfc_panel_height = %s\n",str);
    gstUrsaRXTX_PanelParameter.m_u16Mfc_panel_height = n;
    n = Profile_GetInteger("FRC_Chip", "u16mfc_panel_width", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("u16mfc_panel_width = %s\n",str);
    gstUrsaRXTX_PanelParameter.m_u16Mfc_panel_width = n;
    n = Profile_GetInteger("FRC_Chip", "u8mfc_panel_vfreq", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("u8mfc_panel_vfreq = %s\n",str);
    gstUrsaRXTX_PanelParameter.m_u8Mfc_panel_vfreq = n;
    n = Profile_GetInteger("FRC_Chip", "emfc_panel_inc_vtotal_for_50hz", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("emfc_panel_inc_vtotal_for_50hz = %s\n",str);
    gstUrsaRXTX_PanelParameter.m_eMfc_panel_inc_vtotal_for_50hz = n;
    n = Profile_GetInteger("FRC_Chip", "emfc_panel_type", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("emfc_panel_type = %s\n",str);
    gstUrsaRXTX_PanelParameter.m_eMfc_panel_type = n;
    n = Profile_GetInteger("FRC_Chip", "emfc_panel_channel", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("emfc_panel_channel = %s\n",str);
    gstUrsaRXTX_PanelParameter.m_eMfc_panel_channel = n;
    n = Profile_GetInteger("FRC_Chip", "u8mfc_panel_lvds_swap_ch", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("u8mfc_panel_lvds_swap_ch = %s\n",str);
    gstUrsaRXTX_PanelParameter.m_u8Mfc_panel_lvds_swap_ch = n;
    n = Profile_GetInteger("FRC_Chip", "emfc_lvds_tx_bit_num", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("emfc_lvds_tx_bit_num = %s\n",str);
    gstUrsaRXTX_PanelParameter.m_eMfc_lvds_tx_bit_num = n;
    n = Profile_GetInteger("FRC_Chip", "bmfc_panel_lvds_tx_shift_pair", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("bmfc_panel_lvds_tx_shift_pair = %s\n",str);
    gstUrsaRXTX_PanelParameter.m_bMfc_panel_lvds_tx_shift_pair = n;
    n = Profile_GetInteger("FRC_Chip", "emfc_panel_tx_ti_mode", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("emfc_panel_tx_ti_mode = %s\n",str);
    gstUrsaRXTX_PanelParameter.m_eMfc_panel_tx_ti_mode = n;
    n = Profile_GetInteger("FRC_Chip", "bmfc_panel_lvds_tx_swap_pol", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("bmfc_panel_lvds_tx_swap_pol = %s\n",str);
    gstUrsaRXTX_PanelParameter.m_bMfc_panel_lvds_tx_swap_pol = n;
    n = Profile_GetInteger("FRC_Chip", "bmfc_panel_lvds_tx_swap_pair", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("bmfc_panel_lvds_tx_swap_pair = %s\n",str);
    gstUrsaRXTX_PanelParameter.m_bMfc_panel_lvds_tx_swap_pair = n;
    n = Profile_GetInteger("FRC_Chip", "bmfc_tx_dither_enable", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("bmfc_tx_dither_enable = %s\n",str);
    gstUrsaRXTX_PanelParameter.m_bMfc_tx_dither_enable = n;
    n = Profile_GetInteger("FRC_Chip", "bmfc_rx_ttl", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("bmfc_rx_ttl = %s\n",str);
    gstUrsaRXTX_PanelParameter.m_bMfc_rx_ttl = n;
    n = Profile_GetInteger("FRC_Chip", "emfc_panel_sub_type", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("emfc_panel_sub_type = %s\n",str);
    gstUrsaRXTX_PanelParameter.m_eMfc_panel_sub_type = n;
    n = Profile_GetInteger("FRC_Chip", "bmfc_tx_mask_HS", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("bmfc_tx_mask_HS = %s\n",str);
    gstUrsaRXTX_PanelParameter.m_bMfc_tx_mask_HS = n;
    n = Profile_GetInteger("FRC_Chip", "bmfc_tx_mask_VS", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("bmfc_tx_mask_VS = %s\n",str);
    gstUrsaRXTX_PanelParameter.m_bMfc_tx_mask_VS = n;
    n = Profile_GetInteger("FRC_Chip", "bmfc_tx_mask_DE", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("bmfc_tx_mask_DE = %s\n",str);
    gstUrsaRXTX_PanelParameter.m_bMfc_tx_mask_DE = n;
    n = Profile_GetInteger("FRC_Chip", "bmfc_tx_invert_HS", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("bmfc_tx_invert_HS = %s\n",str);
    gstUrsaRXTX_PanelParameter.m_bMfc_tx_invert_HS = n;
    n = Profile_GetInteger("FRC_Chip", "bmfc_tx_invert_VS", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("bmfc_tx_invert_VS = %s\n",str);
    gstUrsaRXTX_PanelParameter.m_bMfc_tx_invert_VS = n;
    n = Profile_GetInteger("FRC_Chip", "bmfc_tx_invert_DE", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("bmfc_tx_invert_DE = %s\n",str);
    gstUrsaRXTX_PanelParameter.m_bMfc_tx_invert_DE = n;
    n = Profile_GetInteger("FRC_Chip", "bmfc_tx_dither_8_bits", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("bmfc_tx_dither_8_bits = %s\n",str);
    gstUrsaRXTX_PanelParameter.m_bMfc_tx_dither_8_bits = n;
    n = Profile_GetInteger("FRC_Chip", "u8mfc_ctrl_swing", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("u8mfc_ctrl_swing = %s\n",str);
    gstUrsaRXTX_PanelParameter.m_u8Mfc_ctrl_swing = n;
    n = Profile_GetInteger("FRC_Chip", "u8mfc_Safe_Range", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("u8mfc_Safe_Range = %s\n",str);
    gstUrsaRXTX_PanelParameter.m_u8Mfc_Safe_Range = n;
    n = Profile_GetInteger("FRC_Chip", "emfc_panel_ursa_selection", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("emfc_panel_ursa_selection = %s\n",str);
    gstUrsaRXTX_PanelParameter.m_eMfc_panel_ursa_selection = n;
    n = Profile_GetInteger("FRC_Chip", "emfc_dither_tailcut", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("emfc_dither_tailcut = %s\n",str);
    gstUrsaRXTX_PanelParameter.m_eMfc_dither_tailcut = n;
    n = Profile_GetInteger("FRC_Chip", "bmfc_Swap_LR", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("bmfc_Swap_LR = %s\n",str);
    gstUrsaRXTX_PanelParameter.m_bMfc_Swap_LR = n;
    n = Profile_GetInteger("FRC_Chip", "emfc_Passive_First_Line", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("emfc_Passive_First_Line = %s\n",str);
    gstUrsaRXTX_PanelParameter.m_eMfc_Passive_First_Line = n;
    n = Profile_GetInteger("FRC_Chip", "emfc_Passive_Line_Order", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("emfc_Passive_Line_Order = %s\n",str);
    gstUrsaRXTX_PanelParameter.m_eMfc_Passive_Line_Order = n;
    n = Profile_GetInteger("FRC_Chip", "emfc_Sequence_Type", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("emfc_Sequence_Type = %s\n",str);
    gstUrsaRXTX_PanelParameter.m_eMfc_Sequence_Type = n;
    n = Profile_GetInteger("FRC_Chip", "emfc_Panel_3D_Type", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("emfc_Panel_3D_Type = %s\n",str);
    gstUrsaRXTX_PanelParameter.m_eMfc_Panel_3D_Type = n;
    n = Profile_GetInteger("FRC_Chip", "emfc_panel_pair_swap_type", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("emfc_panel_pair_swap_type = %s\n",str);
    gstUrsaRXTX_PanelParameter.m_eMfc_panel_pair_swap_type = n;
    n = Profile_GetInteger("FRC_Chip", "u8mfcLVDSSwapCh", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("u8mfcLVDSSwapCh = %s\n",str);
    gstUrsaRXTX_PanelParameter.m_u8MfcLVDSSwapCh = n;
    n = Profile_GetInteger("FRC_Chip", "bmfcLVDSSwapPol", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("bmfcLVDSSwapPol = %s\n",str);
    gstUrsaRXTX_PanelParameter.m_bMfcLVDSSwapPol = n;
    n = Profile_GetInteger("FRC_Chip", "bmfcPanelSwapLR", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("bmfcPanelSwapLR = %s\n",str);
    gstUrsaRXTX_PanelParameter.m_bMfcPanelSwapLR = n;
    n = Profile_GetInteger("FRC_Chip", "u8MfcUrsaType", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("u8MfcUrsaType = %s\n",str);
    gstUrsaRXTX_PanelParameter.m_u8MfcUrsaType = n;
    n = Profile_GetInteger("panel", "m_bSpreadSpectrumEnable", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("m_bSpreadSpectrumEnable = %s\n",str);
    gstUrsaRXTX_PanelParameter.m_bMfc_Spread_Spectrum_Enable = n;
#endif

#if 0 //defined(CONFIG_PNL_OLED_LG_SUPPORT) the panel.ini format is different from
    memset(str,0,sizeof(str));
    /* OLED specificl parameter */
    n = Profile_GetInteger("OLED_Specific", "eBacklightType", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("eBacklightType = %s\n",str);
    gstPNL_OLED_SpecificParameter.m_eBacklightType = n;
    n = Profile_GetInteger("OLED_Specific", "u16off_timing3", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("u16off_timing3 = %s\n",str);
    gstPNL_OLED_SpecificParameter.m_u16off_timing3 = n;
    n = Profile_GetInteger("OLED_Specific", "u16on_timing3", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("u16on_timing3 = %s\n",str);
    gstPNL_OLED_SpecificParameter.m_u16on_timing3 = n;
#endif

    if(script)
        free(script);
    return 0;
}

int Load_PanelSetting_ToFlash(U32 u32DbtableOffset)
{
    int ret = -1;
    PanelType pnl;
#if defined(CONFIG_DATA_SEPARATION)
    char * pEnv=NULL;
#endif
    UBOOT_TRACE("IN\n");
    memset(&pnl,0,sizeof(pnl));
#if defined(CONFIG_DATA_SEPARATION)
    pEnv=getenv(ENV_CUSDATA);
    if (pEnv !=NULL)
    {
        UBOOT_DEBUG("Get MAP ENV_CUSDATA from env =%s \n",pEnv);
        if(vfs_mount(pEnv)!=0)
        {
            UBOOT_ERROR("mount %s fail\n",pEnv);
            goto exit;
        }
    }
    else
    {
        if(vfs_mount(CUSDATA)!=0)  //use default PATH /cusdata
        {
            UBOOT_ERROR("mount %s fail\n",CONFIG);
            goto exit;
        }
    }
#else
    if (bOdmModeEnable == 1) {
        vfs_mount(ODM);
    } else {
        vfs_mount(CONFIG);
    }
#endif
    if(parse_pnl_ini(gPnlPath,&pnl)!=0)
    {
        UBOOT_ERROR("parse_pnl_ini fail !!\n");
        return ret;
    }

    UBOOT_DEBUG("panel para size =: 0x%08x \n",sizeof(pnl));
    ret = Load_DataToDbBuffer(u32DbtableOffset, E_DB_PANEL_PARA, (U32)&pnl);

exit:
    UBOOT_TRACE("OK\n");
    return ret;
}

int Read_PanelParaFromflash(PanelType * panel_data)
{
    int ret = -1;
    ret = Read_FlashDataToAddr((U32)panel_data, E_DB_PANEL_PARA);
    UBOOT_DEBUG("panel_data.m_wPanelVStart=: 0x%x\n",panel_data->m_wPanelVStart);
    UBOOT_DEBUG("panel_data.m_wPanelWidth=: 0x%x\n",panel_data->m_wPanelWidth);
    UBOOT_DEBUG("panel_data.m_wPanelHeight=: 0x%x\n",panel_data->m_wPanelHeight);
    return ret;
}

#if defined(CONFIG_URSA_TXRX_SUPPORT)
int Load_UrsaRXTXPanelSetting_ToFlash(U32 u32DbtableOffset)
{
    int ret = -1;
    UBOOT_TRACE("IN\n");
    ret = Load_DataToDbBuffer(u32DbtableOffset, E_DB_URSA_TXRX_PANEL_PARA, (U32)&gstUrsaRXTX_PanelParameter);
    UBOOT_DEBUG("gstUrsaRXTX_PanelParameter->m_u8MfcUrsaType                      = 0x%x\n", (unsigned int)gstUrsaRXTX_PanelParameter.m_u8MfcUrsaType);
    UBOOT_DEBUG("gstUrsaRXTX_PanelParameter->m_u16Mfc_panel_htotal              = 0x%x\n", (unsigned int)gstUrsaRXTX_PanelParameter.m_u16Mfc_panel_htotal);
    UBOOT_DEBUG("gstUrsaRXTX_PanelParameter->m_u16Mfc_panel_vtotal              = 0x%x\n", (unsigned int)gstUrsaRXTX_PanelParameter.m_u16Mfc_panel_vtotal);
    UBOOT_DEBUG("gstUrsaRXTX_PanelParameter->m_u16Mfc_panel_vstart              = 0x%x\n", (unsigned int)gstUrsaRXTX_PanelParameter.m_u16Mfc_panel_vstart);
    UBOOT_DEBUG("gstUrsaRXTX_PanelParameter->m_u16Mfc_panel_height              = 0x%x\n", (unsigned int)gstUrsaRXTX_PanelParameter.m_u16Mfc_panel_height);
    UBOOT_DEBUG("gstUrsaRXTX_PanelParameter->m_u16Mfc_panel_hstart              = 0x%x\n", (unsigned int)gstUrsaRXTX_PanelParameter.m_u16Mfc_panel_hstart);
    UBOOT_DEBUG("gstUrsaRXTX_PanelParameter->m_u16Mfc_panel_width               = 0x%x\n", (unsigned int)gstUrsaRXTX_PanelParameter.m_u16Mfc_panel_width);
    UBOOT_TRACE("OK\n");
    return ret;

}
int Read_UrsaRXTXPanelSetting_ToFlash(st_PNL_UrsaRXTX_PanelParameter * stUrsaRxTxPanel_data)
{
    int ret = -1;
    memset(stUrsaRxTxPanel_data, 0, sizeof(st_PNL_UrsaRXTX_PanelParameter));
    ret = Read_FlashDataToAddr((U32)stUrsaRxTxPanel_data, E_DB_URSA_TXRX_PANEL_PARA);
    UBOOT_DEBUG("ursaRxTxPanel_data->m_u8MfcUrsaType                      = 0x%x\n", (unsigned int)stUrsaRxTxPanel_data->m_u8MfcUrsaType);
    UBOOT_DEBUG("ursaRxTxPanel_data->m_u16Mfc_panel_htotal              = 0x%x\n", (unsigned int)stUrsaRxTxPanel_data->m_u16Mfc_panel_htotal);
    UBOOT_DEBUG("ursaRxTxPanel_data->m_u16Mfc_panel_vtotal              = 0x%x\n", (unsigned int)stUrsaRxTxPanel_data->m_u16Mfc_panel_vtotal);
    UBOOT_DEBUG("ursaRxTxPanel_data->m_u16Mfc_panel_vstart              = 0x%x\n", (unsigned int)stUrsaRxTxPanel_data->m_u16Mfc_panel_vstart);
    UBOOT_DEBUG("ursaRxTxPanel_data->m_u16Mfc_panel_height              = 0x%x\n", (unsigned int)stUrsaRxTxPanel_data->m_u16Mfc_panel_height);
    UBOOT_DEBUG("ursaRxTxPanel_data->m_u16Mfc_panel_hstart              = 0x%x\n", (unsigned int)stUrsaRxTxPanel_data->m_u16Mfc_panel_hstart);
    UBOOT_DEBUG("ursaRxTxPanel_data->m_u16Mfc_panel_width               = 0x%x\n", (unsigned int)stUrsaRxTxPanel_data->m_u16Mfc_panel_width);
    return ret;
}
#endif

#if defined(CONFIG_PNL_OLED_LG_SUPPORT)
int Load_OLEDSpecificParameter_ToFlash(U32 u32DbtableOffset)
{
    int ret = -1;
    UBOOT_TRACE("IN\n");
    ret = Load_DataToDbBuffer(u32DbtableOffset, E_DB_OLED_LG_PANEL_PARA, (U32)&gstPNL_OLED_SpecificParameter);
    UBOOT_DEBUG("gstPNL_OLED_SpecificParameter->m_eBacklightType                      = 0x%x\n", (unsigned int)gstPNL_OLED_SpecificParameter.m_eBacklightType);
    UBOOT_DEBUG("gstPNL_OLED_SpecificParameter->m_u16off_timing3                      = 0x%x\n", (unsigned int)gstPNL_OLED_SpecificParameter.m_u16off_timing3);
    UBOOT_DEBUG("gstPNL_OLED_SpecificParameter->m_u16on_timing3                      = 0x%x\n", (unsigned int)gstPNL_OLED_SpecificParameter.m_u16on_timing3);
    UBOOT_TRACE("OK\n");
    return ret;

}
int Read_OLEDSpecificParameter_ToFlash(st_PNL_OLED_SpecificParameter * stPnlOledSpecifc_data)
{
    int ret = -1;
    memset(stPnlOledSpecifc_data, 0, sizeof(st_PNL_OLED_SpecificParameter));
    ret = Read_FlashDataToAddr((U32)stPnlOledSpecifc_data, E_DB_OLED_LG_PANEL_PARA);
    UBOOT_DEBUG("PnlOledSpecifc_data->m_eBacklightType                      = 0x%x\n", (unsigned int)stPnlOledSpecifc_data->m_eBacklightType);
    UBOOT_DEBUG("PnlOledSpecifc_data->m_u16off_timing3                      = 0x%x\n", (unsigned int)stPnlOledSpecifc_data->m_u16off_timing3);
    UBOOT_DEBUG("PnlOledSpecifc_data->m_u16on_timing3                      = 0x%x\n", (unsigned int)stPnlOledSpecifc_data->m_u16on_timing3);
    return ret;
}
#endif

int parse_board_ini(char *path, st_board_para *p_data)
{
    long n;
    char *script = NULL;
    U32 filesize = 0;
    char str[BUFFER_SIZE];
    script = loadscript(path,&filesize);
    if(script == NULL)
    {
        while (*path== '/') path++;
        path=strstr(path,"/");
        UBOOT_INFO("filter /config, and find file again!!\n");
        script = loadscript(path,&filesize);
        if(script == NULL)
        {
            return -1;
        }
    }

    memset(str,0,sizeof(str));
    UBOOT_INFO("sizeof( board ini script ) --------> %u bytes\n",filesize);
    Profile_Init((char *)script,filesize);

    n = Profile_GetInteger("PanelRelativeSetting", "m_u8BOARD_PWM_PORT", 2);
    UBOOT_DEBUG("m_u8BOARD_PWM_PORT = %ld \n",n);
    p_data->m_u8BOARD_PWM_PORT = n;
    gstPWMSetting.m_u16PWMPort = n;

    n = Profile_GetInteger("PanelRelativeSetting", "m_u16BOARD_LVDS_CONNECT_TYPE", 0);
    UBOOT_DEBUG("m_u16BOARD_LVDS_CONNECT_TYPE = %ld \n",n);
    p_data->m_u16BOARD_LVDS_CONNECT_TYPE = n;

    n = Profile_GetInteger("PanelRelativeSetting", "m_bPANEL_PDP_10BIT", 0);
    UBOOT_DEBUG("m_bPANEL_PDP_10BIT = %ld \n",n);
    p_data->m_bPANEL_PDP_10BIT = n;

    n = Profile_GetInteger("PanelRelativeSetting", "m_bPANEL_SWAP_LVDS_POL", 0);
    UBOOT_DEBUG("m_bPANEL_SWAP_LVDS_POL = %ld \n",n);
    p_data->m_bPANEL_SWAP_LVDS_POL = n;

    n = Profile_GetInteger("PanelRelativeSetting", "m_bPANEL_SWAP_LVDS_CH", 0);
    UBOOT_DEBUG("m_bPANEL_SWAP_LVDS_CH = %ld \n",n);
    p_data->m_bPANEL_SWAP_LVDS_CH = n;

    n = Profile_GetInteger("PanelRelativeSetting", "m_bPANEL_CONNECTOR_SWAP_PORT", 0);
    UBOOT_DEBUG("m_bPANEL_CONNECTOR_SWAP_PORT = %ld \n",n);
    p_data->m_bPANEL_CONNECTOR_SWAP_PORT = n;

    n = Profile_GetInteger("PanelRelativeSetting", "m_u16LVDS_PN_SWAP_L", 0);
    UBOOT_DEBUG("m_u16LVDS_PN_SWAP_L = %ld \n",n);
    p_data->m_u16LVDS_PN_SWAP_L = n;

    n = Profile_GetInteger("PanelRelativeSetting", "m_u16LVDS_PN_SWAP_H", 0);
    UBOOT_DEBUG("m_u16LVDS_PN_SWAP_H = %ld \n",n);
    p_data->m_u16LVDS_PN_SWAP_H = n;

    // parsing [PanelDivisonGPIO_1]
    Profile_GetString("PanelDivisonGPIO_1", "PAD_NAME_1","", str, sizearray(str));
    UBOOT_DEBUG("PanelDivisonGPIO_1 = %s \n",str);
    strncpy(p_data->m_sGPIO1_PAD_NAME, str, sizeof(p_data->m_sGPIO1_PAD_NAME));

    n = Profile_GetInteger("PanelDivisonGPIO_1", "GPIO_INDEX_1", 0);
    UBOOT_DEBUG("GPIO_INDEX = %ld \n",n);
    p_data->m_u16GPIO1_INDEX = n;

    n = Profile_GetInteger("PanelDivisonGPIO_1", "VALUE_1", 0);
    UBOOT_DEBUG("VALUE = %ld \n",n);
    p_data->m_u8GPIO1_VALUE = n;

    // parsing [PanelDivisonGPIO_2]
    Profile_GetString("PanelDivisonGPIO_2", "PAD_NAME_2","", str, sizearray(str));
    UBOOT_DEBUG("PanelDivisonGPIO_2 = %s \n",str);
    strncpy(p_data->m_sGPIO2_PAD_NAME, str, sizeof(p_data->m_sGPIO2_PAD_NAME));

    n = Profile_GetInteger("PanelDivisonGPIO_2", "GPIO_INDEX_2", 0);
    UBOOT_DEBUG("GPIO_INDEX = %ld \n",n);
    p_data->m_u16GPIO2_INDEX = n;

    n = Profile_GetInteger("PanelDivisonGPIO_2", "VALUE_2", 0);
    UBOOT_DEBUG("VALUE = %ld \n",n);
    p_data->m_u8GPIO2_VALUE = n;

    n = Profile_GetBoolean("DispoutConfig", "m_bTconOutput", 0);
    UBOOT_DEBUG("m_bTconOutput = %ld \n",n);
    p_data->m_bTconOutput = n;

    n = Profile_GetBoolean("DispoutConfig", "m_bODEnable", 0);
    UBOOT_DEBUG("m_bODEnable = %ld \n",n);
    p_data->m_bODEnable = n;

    if(script)
        free(script);
    return 0;
}

int Load_BoardSetting_ToFlash(U32 u32DbtableOffset)
{
    int ret = -1;
    st_board_para stBoard;
    UBOOT_TRACE("IN\n");
    memset(&stBoard,0,sizeof(stBoard));
    vfs_mount(CONFIG);
    if(parse_board_ini(gBoardPath, &stBoard)!=0)
    {
        UBOOT_ERROR("parse_board_ini fail !!\n");
        return ret;
    }

    UBOOT_DEBUG("board para size =: 0x%08x \n",sizeof(stBoard));
    UBOOT_DEBUG("st_board_para.m_u8BOARD_PWM_PORT =: 0x%x \n",stBoard.m_u8BOARD_PWM_PORT);
    UBOOT_DEBUG("st_board_para.m_u16BOARD_LVDS_CONNECT_TYPE =: 0x%x \n",stBoard.m_u16BOARD_LVDS_CONNECT_TYPE);
    UBOOT_DEBUG("st_board_para.m_u8BOARD_PWM_PORT =: 0x%x \n",stBoard.m_u8BOARD_PWM_PORT);
    UBOOT_DEBUG("st_board_para.m_bPANEL_PDP_10BIT =: 0x%x \n",stBoard.m_bPANEL_PDP_10BIT);
    UBOOT_DEBUG("st_board_para.m_bPANEL_SWAP_LVDS_POL =: 0x%x \n",stBoard.m_bPANEL_SWAP_LVDS_POL);
    UBOOT_DEBUG("st_board_para.m_bPANEL_SWAP_LVDS_CH =: 0x%x \n",stBoard.m_bPANEL_SWAP_LVDS_CH);
    UBOOT_DEBUG("st_board_para.m_bPANEL_CONNECTOR_SWAP_PORT =: 0x%x \n",stBoard.m_bPANEL_CONNECTOR_SWAP_PORT);
    UBOOT_DEBUG("st_board_para.m_u16LVDS_PN_SWAP_L =: 0x%x \n",stBoard.m_u16LVDS_PN_SWAP_L);
    UBOOT_DEBUG("st_board_para.m_u16LVDS_PN_SWAP_H =: 0x%x \n",stBoard.m_u16LVDS_PN_SWAP_H);
    UBOOT_DEBUG("st_board_para.m_sGPIO1_PAD_NAME =: %s \n",stBoard.m_sGPIO1_PAD_NAME);
    UBOOT_DEBUG("st_board_para.m_u16GPIO1_INDEX =: 0x%x \n",stBoard.m_u16GPIO1_INDEX);
    UBOOT_DEBUG("st_board_para.m_u8GPIO1_VALUE =: 0x%x \n",stBoard.m_u8GPIO1_VALUE);
    UBOOT_DEBUG("st_board_para.m_sGPIO2_PAD_NAME =: %s \n",stBoard.m_sGPIO2_PAD_NAME);
    UBOOT_DEBUG("st_board_para.m_u16GPIO2_INDEX =: 0x%x \n",stBoard.m_u16GPIO2_INDEX);
    UBOOT_DEBUG("st_board_para.m_u8GPIO2_VALUE =: 0x%x \n",stBoard.m_u8GPIO2_VALUE);
    UBOOT_DEBUG("st_board_para.m_bTconOutput =: 0x%x \n",stBoard.m_bTconOutput);
    UBOOT_DEBUG("st_board_para.m_bODEnable =: 0x%x \n",stBoard.m_bODEnable);
    ret = Load_DataToDbBuffer(u32DbtableOffset, E_DB_BOARD_PARA, (U32)&stBoard);
    UBOOT_TRACE("OK\n");
    return ret;
}

int Read_BoardParaFromflash(st_board_para * board_data)
{
    int ret = -1;
    ret = Read_FlashDataToAddr((U32)board_data, E_DB_BOARD_PARA);
    return ret;
}

#endif


#if (ENABLE_ENABLE_URSA == 1)
#if (ENABLE_URSA_6M30 == 1)
int iniparser_6m30_cmd(ursa_6m30_cmd_table * cmd_table)
{
    int ret = -1;
    char * script = NULL;
    char InPath[BUFFER_SIZE]= {0};
    U32 filesize = 0;
    vfs_mount(CONFIG);
    snprintf(InPath,sizeof(InPath),"%s/6m30_mode.ini",CONFIG_PATH);
    script = loadscript(InPath,&filesize);
    if(script != NULL)
    {
        Profile_Init(script,filesize);
        cmd_table->URSA_6M30_LVDS_CHANNEL_SWAP_MODE = Profile_GetInteger("MISC_6M30_CFG", "6M30_LVDS_CHANNEL_SWAP_MODE", 0);
        cmd_table->URSA_6M30_IN_LVDS_TIMODE = Profile_GetInteger("MISC_6M30_CFG", "6M30_IN_LVDS_TIMODE", 0);
        cmd_table->URSA_6M30_IN_LVDS_TI_BITNUM = Profile_GetInteger("MISC_6M30_CFG", "6M30_IN_LVDS_TI_BITNUM", 0);
        cmd_table->URSA_6M30_OUT_LVDS_TIMODE = Profile_GetInteger("MISC_6M30_CFG", "6M30_OUT_LVDS_TIMODE", 0);
        cmd_table->URSA_6M30_OUT_LVDS_TI_BITNUM = Profile_GetInteger("MISC_6M30_CFG", "6M30_OUT_LVDS_TI_BITNUM", 0);

        cmd_table->URSA_6M30_SSC_SEL = Profile_GetInteger("MISC_6M30_CFG", "6M30_SSC_SEL", 0);
        cmd_table->URSA_6M30_SSC_ENABLE = Profile_GetInteger("MISC_6M30_CFG", "6M30_SSC_ENABLE", 0);
        cmd_table->URSA_6M30_SSC_FREQ = Profile_GetInteger("MISC_6M30_CFG", "6M30_SSC_FREQ", 0);
        cmd_table->URSA_6M30_SSC_PERCENT = Profile_GetInteger("MISC_6M30_CFG", "6M30_SSC_PERCENT", 0);

        UBOOT_DEBUG(" --->>> 6M30_LVDS_CHANNEL_SWAP_MODE is %x \n",cmd_table->URSA_6M30_LVDS_CHANNEL_SWAP_MODE);
        UBOOT_DEBUG(" --->>> 6M30_IN_LVDS_TIMODE is %x \n",cmd_table->URSA_6M30_IN_LVDS_TIMODE);
        UBOOT_DEBUG(" --->>> 6M30_IN_LVDS_TI_BITNUM is %x \n",cmd_table->URSA_6M30_IN_LVDS_TI_BITNUM);
        UBOOT_DEBUG(" --->>> 6M30_OUT_LVDS_TIMODE is %x \n",cmd_table->URSA_6M30_OUT_LVDS_TIMODE);
        UBOOT_DEBUG(" --->>> 6M30_OUT_LVDS_TI_BITNUM is %x \n",cmd_table->URSA_6M30_OUT_LVDS_TI_BITNUM);

        UBOOT_DEBUG(" --->>> 6M30_SSC_SEL is %x \n",cmd_table->URSA_6M30_SSC_SEL);
        UBOOT_DEBUG(" --->>> 6M30_SSC_ENABLE is %x \n",cmd_table->URSA_6M30_SSC_ENABLE);
        UBOOT_DEBUG(" --->>> 6M30_SSC_FREQ is %x \n",cmd_table->URSA_6M30_SSC_FREQ);
        UBOOT_DEBUG(" --->>> 6M30_SSC_PERCENT is %x \n",cmd_table->URSA_6M30_SSC_PERCENT);
        ret = 0;
    }
    else
    {
        UBOOT_ERROR("ubifsload 6m30.ini fail...>>>\n");
    }
    free(script);
    return ret;
}
int Read_Ursa_6m30_Para(ursa_6m30_cmd_table * cmd_table)
{
    int ret = -1;
    ret = Read_FlashDataToAddr((U32)cmd_table, E_DB_URSA_6M30);

    UBOOT_DEBUG(" cmd_table.6M30_LVDS_CHANNEL_SWAP_MODE is %x \n",cmd_table->URSA_6M30_LVDS_CHANNEL_SWAP_MODE);
    UBOOT_DEBUG(" cmd_table.6M30_IN_LVDS_TIMODE is %x \n",cmd_table->URSA_6M30_IN_LVDS_TIMODE);
    UBOOT_DEBUG(" cmd_table.6M30_IN_LVDS_TI_BITNUM is %x \n",cmd_table->URSA_6M30_IN_LVDS_TI_BITNUM);
    UBOOT_DEBUG(" cmd_table.6M30_OUT_LVDS_TIMODE is %x \n",cmd_table->URSA_6M30_OUT_LVDS_TIMODE);
    UBOOT_DEBUG(" cmd_table.6M30_OUT_LVDS_TI_BITNUM is %x \n",cmd_table->URSA_6M30_OUT_LVDS_TI_BITNUM);

    UBOOT_DEBUG(" cmd_table.6M30_SSC_SEL is %x \n",cmd_table->URSA_6M30_SSC_SEL);
    UBOOT_DEBUG(" cmd_table.6M30_SSC_ENABLE is %x \n",cmd_table->URSA_6M30_SSC_ENABLE);
    UBOOT_DEBUG(" cmd_table.6M30_SSC_FREQ is %x \n",cmd_table->URSA_6M30_SSC_FREQ);
    UBOOT_DEBUG(" cmd_table.6M30_SSC_PERCENT is %x \n",cmd_table->URSA_6M30_SSC_PERCENT);
    return ret;
}
int Load_Ursa6m30Para_ToFlash(U32 u32DbtableOffset)
{
    int ret = -1;
    ursa_6m30_cmd_table cmd_table= {0};
    UBOOT_TRACE("IN\n");
    if(iniparser_6m30_cmd(&cmd_table)==0)
    {
        UBOOT_DEBUG("6m30 para size =: 0x%08x \n",sizeof(cmd_table));
        UBOOT_DEBUG("gstDbtable.dbdata[E_DB_URSA_6M30].Offset =: 0x%x\n",gstDbtable.dbdata[E_DB_URSA_6M30].Offset);
        ret = Load_DataToDbBuffer(u32DbtableOffset, E_DB_URSA_6M30, (U32)&cmd_table);
    }
    else
    {
        UBOOT_ERROR("parser_6m30_cmd fail ...>>>\n");
    }
    UBOOT_TRACE("OK\n");
    return ret;
}

int Ursa_6M30_Setting(ursa_6m30_cmd_table * cmd_table)
{
    int ret = -1;
    MDrv_Ursa_6M30_ChanelSwap_Mode(cmd_table->URSA_6M30_LVDS_CHANNEL_SWAP_MODE);
    g_UrsaCMDGenSetting.g_InitInLvdsAll.bTIMode = cmd_table->URSA_6M30_IN_LVDS_TIMODE;
    g_UrsaCMDGenSetting.g_InitInLvdsAll.BitNums = cmd_table->URSA_6M30_IN_LVDS_TI_BITNUM;
    g_UrsaCMDGenSetting.g_OutLvdsMode.bTIMode = cmd_table->URSA_6M30_OUT_LVDS_TIMODE;
    g_UrsaCMDGenSetting.g_OutLvdsMode.PanelBitNums = cmd_table->URSA_6M30_OUT_LVDS_TI_BITNUM;

    MDrv_Ursa_6M30_Data_Init();

    g_UrsaCMDGenSetting.g_SscFreqPercent.SscSel = cmd_table->URSA_6M30_SSC_SEL;
    g_UrsaCMDGenSetting.g_SscFreqPercent.SscEN = cmd_table->URSA_6M30_SSC_ENABLE;
    g_UrsaCMDGenSetting.g_SscFreqPercent.SscFreq = cmd_table->URSA_6M30_SSC_FREQ;
    g_UrsaCMDGenSetting.g_SscFreqPercent.SscPercent = cmd_table->URSA_6M30_SSC_PERCENT;

    if(MDrv_Ursa_6M30_3D_Cmd(UC_SET_SSC_FREQ_PERCENT)>0)
    {
        ret = 0;
    }

    return ret;
}

#elif (ENABLE_URSA_8 == 1) || (ENABLE_URSA_6M40 == 1)

int parse_model_ini_ForUrsa(char* path, char* UrsaInIPath)
{
    int ret = -1;//
    char *script = NULL;
    char str[BUFFER_SIZE]= {0};
    U32 filesize = 0;
    script = loadscript(path,&filesize);
    if(script == NULL)
    {
        while (*path== '/') path++;
        path=strstr(path,"/");
        UBOOT_INFO("filter /config, and find file again!!\n");
        script = loadscript(path,&filesize);
        if(script == NULL)
        {
            goto exit;
        }
    }
    UBOOT_INFO("sizeof( model ini script ) --------> %u bytes\n",filesize);

    Profile_Init((char *)script,filesize);
    if(Profile_GetString("Ursa", "m_pUrsaName","", str, sizearray(str)))
    {
        UBOOT_INFO("Half Ursa.ini path --> %s\n",str);
        snprintf(UrsaInIPath,BUFFER_SIZE,"%s",str);
        UBOOT_INFO("Full Ursa.ini  path --> %s\n",UrsaInIPath);
        ret = 0;
    }
    else
    {
        goto exit;
    }

exit:
    if(script)
        free(script);
    return ret;
}

int Parser_Ursa_Ini(ursa_cmd_table * cmd_table)
{
    int ret = -1;
    char * script = NULL;
    char InPath[BUFFER_SIZE]= {0};
    char OutPath[BUFFER_SIZE]= {0};
    char UrsaBinName[BUFFER_SIZE]= {0};
    U32 filesize = 0;
    vfs_mount(CONFIG);
    memset(InPath,0,sizeof(InPath));
    snprintf(InPath,sizeof(InPath),"%s/sys.ini",CONFIG_PATH);
    ret = parse_sys_ini(InPath, OutPath, BUFFER_SIZE);
    if(ret)
    {
        goto Ursa_exit;
    }

    memset(InPath,0,sizeof(InPath));
    memcpy(InPath,OutPath,sizeof(OutPath));
    ret = parse_model_ini_ForUrsa(InPath,OutPath);
    if(ret)
    {
        goto Ursa_exit;
    }
    UBOOT_INFO("Full Ursa.ini path --> %s\n",OutPath);

    memset(InPath,0,sizeof(InPath));
    memcpy(InPath,OutPath,sizeof(OutPath));
    script = loadscript(InPath,&filesize);
    if(script != NULL)
    {
        Profile_Init(script,filesize);
        cmd_table->URSA_EXTERNAL = Profile_GetBoolean("URSA_PARA_CFG", "URSA_EXTERNAL", 1);
        cmd_table->URSA_MEMC_ENABLE = Profile_GetInteger("URSA_PARA_CFG", "URSA_MEMC_ENABLE", 0);
        cmd_table->URSA_LVDS_CHANNEL_SWAP_MODE = Profile_GetInteger("URSA_PARA_CFG", "URSA_LVDS_CHANNEL_SWAP_MODE", 0);
        cmd_table->URSA_LVDS_SWAP_POL = Profile_GetInteger("URSA_PARA_CFG", "URSA_LVDS_SWAP_POL", 0);
        cmd_table->URSA_IN_LVDS_TIMODE = Profile_GetInteger("URSA_PARA_CFG", "URSA_IN_LVDS_TIMODE", 0);
        cmd_table->URSA_IN_LVDS_TI_BITNUM = Profile_GetInteger("URSA_PARA_CFG", "URSA_IN_LVDS_TI_BITNUM", 0);
        cmd_table->URSA_OUT_LVDS_TIMODE = Profile_GetInteger("URSA_PARA_CFG", "URSA_OUT_LVDS_TIMODE", 0);
        cmd_table->URSA_OUT_LVDS_TI_BITNUM = Profile_GetInteger("URSA_PARA_CFG", "URSA_OUT_LVDS_TI_BITNUM", 0);

        cmd_table->URSA_SSC_SEL = Profile_GetInteger("URSA_PARA_CFG", "URSA_SSC_SEL", 0);
        cmd_table->URSA_SSC_ENABLE = Profile_GetInteger("URSA_PARA_CFG", "URSA_SSC_ENABLE", 0);
        cmd_table->URSA_SSC_FREQ = Profile_GetInteger("URSA_PARA_CFG", "URSA_SSC_FREQ", 0);
        cmd_table->URSA_SSC_PERCENT = Profile_GetInteger("URSA_PARA_CFG", "URSA_SSC_PERCENT", 0);
        cmd_table->URSA_OSD_PROTECT_MODE = Profile_GetInteger("URSA_PARA_CFG", "URSA_OSD_PROTECT_MODE", 0);

        UBOOT_DEBUG(" --->>> URSA_EXTERNAL is %x \n",cmd_table->URSA_EXTERNAL);
        UBOOT_DEBUG(" --->>> URSA_MEMC_ENABLE is %x \n",cmd_table->URSA_MEMC_ENABLE);
        UBOOT_DEBUG(" --->>> URSA_LVDS_CHANNEL_SWAP_MODE is %x \n",cmd_table->URSA_LVDS_CHANNEL_SWAP_MODE);
        UBOOT_DEBUG(" --->>> URSA_LVDS_SWAP_POL is %x \n",cmd_table->URSA_LVDS_SWAP_POL);
        UBOOT_DEBUG(" --->>> URSA_IN_LVDS_TIMODE is %x \n",cmd_table->URSA_IN_LVDS_TIMODE);
        UBOOT_DEBUG(" --->>> URSA_IN_LVDS_TI_BITNUM is %x \n",cmd_table->URSA_IN_LVDS_TI_BITNUM);
        UBOOT_DEBUG(" --->>> URSA_OUT_LVDS_TIMODE is %x \n",cmd_table->URSA_OUT_LVDS_TIMODE);
        UBOOT_DEBUG(" --->>> URSA_OUT_LVDS_TI_BITNUM is %x \n",cmd_table->URSA_OUT_LVDS_TI_BITNUM);

        UBOOT_DEBUG(" --->>> URSA_SSC_SEL is %x \n",cmd_table->URSA_SSC_SEL);
        UBOOT_DEBUG(" --->>> URSA_SSC_ENABLE is %x \n",cmd_table->URSA_SSC_ENABLE);
        UBOOT_DEBUG(" --->>> URSA_SSC_FREQ is %x \n",cmd_table->URSA_SSC_FREQ);
        UBOOT_DEBUG(" --->>> URSA_SSC_PERCENT is %x \n",cmd_table->URSA_SSC_PERCENT);
        UBOOT_DEBUG(" --->>> URSA_OSD_PROTECT_MODE is %x \n",cmd_table->URSA_OSD_PROTECT_MODE);

        if(cmd_table->URSA_EXTERNAL == FALSE)
        {
            if(Profile_GetString("URSA_FILE_CFG", "m_pUrsaBinName","", gstSysMiscSetting.m_Ursa_Bin_Name, sizearray(gstSysMiscSetting.m_Ursa_Bin_Name)))
            {
                UBOOT_INFO("Full Ursa Bin Name --> %s\n",UrsaBinName);
                ret = 0;
            }
            else
            {
                goto Ursa_exit;
            }
        }
        ret = 0;
    }
    else
    {
        UBOOT_ERROR("ubifsload 6m30.ini fail...>>>\n");
    }

Ursa_exit:
    if(script)
        free(script);
    return ret;
}
int Read_Ursa_Para(ursa_cmd_table * cmd_table)
{
    int ret = -1;
    memset(cmd_table, 0, sizeof(ursa_cmd_table));
    ret = Read_FlashDataToAddr((U32)cmd_table, E_DB_URSA);

    UBOOT_DEBUG(" cmd_table.URSA_EXTERNAL is %x \n",cmd_table->URSA_EXTERNAL);
    UBOOT_DEBUG(" cmd_table.URSA_MEMC_ENABLE is %x \n",cmd_table->URSA_MEMC_ENABLE);
    UBOOT_DEBUG(" cmd_table.URSA_LVDS_CHANNEL_SWAP_MODE is %x \n",cmd_table->URSA_LVDS_CHANNEL_SWAP_MODE);
    UBOOT_DEBUG(" cmd_table.URSA_LVDS_SWAP_POL is %x \n",cmd_table->URSA_LVDS_SWAP_POL);
    UBOOT_DEBUG(" cmd_table.URSA_IN_LVDS_TIMODE is %x \n",cmd_table->URSA_IN_LVDS_TIMODE);
    UBOOT_DEBUG(" cmd_table.URSA_IN_LVDS_TI_BITNUM is %x \n",cmd_table->URSA_IN_LVDS_TI_BITNUM);
    UBOOT_DEBUG(" cmd_table.URSA_OUT_LVDS_TIMODE is %x \n",cmd_table->URSA_OUT_LVDS_TIMODE);
    UBOOT_DEBUG(" cmd_table.URSA_OUT_LVDS_TI_BITNUM is %x \n",cmd_table->URSA_OUT_LVDS_TI_BITNUM);

    UBOOT_DEBUG(" cmd_table.URSA_SSC_SEL is %x \n",cmd_table->URSA_SSC_SEL);
    UBOOT_DEBUG(" cmd_table.URSA_SSC_ENABLE is %x \n",cmd_table->URSA_SSC_ENABLE);
    UBOOT_DEBUG(" cmd_table.URSA_SSC_FREQ is %x \n",cmd_table->URSA_SSC_FREQ);
    UBOOT_DEBUG(" cmd_table.URSA_SSC_PERCENT is %x \n",cmd_table->URSA_SSC_PERCENT);

    UBOOT_DEBUG(" cmd_table.URSA_OSD_PROTECT_MODE is %x \n",cmd_table->URSA_OSD_PROTECT_MODE);
    return ret;
}
int Load_UrsaPara_ToFlash(U32 u32DbtableOffset)
{
    int ret = -1;
    ursa_cmd_table cmd_table= {0};
    UBOOT_TRACE("IN\n");
    if(Parser_Ursa_Ini(&cmd_table)==0)
    {
        UBOOT_DEBUG("Ursa para size =: 0x%08x \n",sizeof(cmd_table));
        UBOOT_DEBUG("gstDbtable.dbdata[E_DB_URSA].Offset =: 0x%x\n",gstDbtable.dbdata[E_DB_URSA].Offset);

        ret = Load_DataToDbBuffer(u32DbtableOffset, E_DB_URSA, (U32)&cmd_table);
    }
    else
    {
        UBOOT_ERROR("parser_cmd fail ...>>>\n");
    }
    UBOOT_TRACE("OK\n");
    return ret;
}

#if (ENABLE_URSA_8 == 1)
int Ursa_8_Setting(ursa_cmd_table * cmd_table)
{
    int ret = -1;
    MDrv_Ursa_8_ChanelSwap_Mode(cmd_table->URSA_LVDS_CHANNEL_SWAP_MODE);
    //g_UrsaCMDGenSetting.g_InitInLvdsAll.bTIMode = cmd_table->URSA_6M30_IN_LVDS_TIMODE;
    //g_UrsaCMDGenSetting.g_InitInLvdsAll.BitNums = cmd_table->URSA_6M30_IN_LVDS_TI_BITNUM;
    g_UrsaCMDGenSetting.g_OutLvdsMode.bLvdsSwapPol = cmd_table->URSA_LVDS_SWAP_POL;
    g_UrsaCMDGenSetting.g_OutLvdsMode.bTIMode = cmd_table->URSA_OUT_LVDS_TIMODE;
    g_UrsaCMDGenSetting.g_OutLvdsMode.PanelBitNums = cmd_table->URSA_OUT_LVDS_TI_BITNUM;

    g_UrsaCMDGenSetting.g_OsdMode.protect_mode = cmd_table->URSA_OSD_PROTECT_MODE;
    //MDrv_Ursa_6M30_Data_Init();
    udelay(50*1000);
    MDrv_Ursa_8_3D_Cmd ( UC_SET_OUT_LVDS_MODE);

    g_UrsaCMDGenSetting.g_SscFreqPercent.SscSel = cmd_table->URSA_SSC_SEL;
    g_UrsaCMDGenSetting.g_SscFreqPercent.SscEN = cmd_table->URSA_SSC_ENABLE;
    g_UrsaCMDGenSetting.g_SscFreqPercent.SscFreq = cmd_table->URSA_SSC_FREQ;
    g_UrsaCMDGenSetting.g_SscFreqPercent.SscPercent = cmd_table->URSA_SSC_PERCENT;
    udelay(50*1000);
    if(MDrv_Ursa_8_3D_Cmd(UC_SET_SSC_FREQ_PERCENT)>0)
    {
        ret = 0;
    }

    return ret;
}
#elif (ENABLE_URSA_6M40 == 1)
int Ursa_6M40_Syetting(ursa_cmd_table * cmd_table)
{
    int ret = -1;
    g_UrsaCMDGenSetting.g_OutLvdsMode.PanelBitNums = cmd_table->URSA_OUT_LVDS_TI_BITNUM;
    g_UrsaCMDGenSetting.g_OsdMode.protect_mode = cmd_table->URSA_OSD_PROTECT_MODE;
    UBOOT_INFO("g_UrsaCMDGenSetting.g_OsdMode.protect_mode=%d\n",g_UrsaCMDGenSetting.g_OsdMode.protect_mode);

    g_UrsaCMDGenSetting.g_SscFreqPercent.SscSel = cmd_table->URSA_SSC_SEL;
    g_UrsaCMDGenSetting.g_SscFreqPercent.SscEN = cmd_table->URSA_SSC_ENABLE;
    g_UrsaCMDGenSetting.g_SscFreqPercent.SscFreq = cmd_table->URSA_SSC_FREQ;
    g_UrsaCMDGenSetting.g_SscFreqPercent.SscPercent = cmd_table->URSA_SSC_PERCENT;

    return ret;
}
#endif

#endif

#endif

int Load_GopMuxToFlash(U32 u32DbtableOffset)
{
    int ret = -1;
    st_gopmux_para GopMux;
    char *pEnv = NULL;
    MS_U32 fsize = 0;
    UBOOT_TRACE("IN\n");
    memset(&GopMux,0,sizeof(GopMux));

    pEnv = getenv(ANDROIDFBRC_PATH);
    UBOOT_DEBUG("env(%s) = %s\n", ANDROIDFBRC_PATH, pEnv);
    if ((pEnv != NULL) && (vfs_mount_by_abspath(pEnv, gGopMuxPath, sizeof(gGopMuxPath)) == 0))
    {
        fsize = vfs_getsize(gGopMuxPath);
        if (fsize == 0)
        {
            UBOOT_ERROR("file(%s) do not exist !\n", pEnv);
            memset(gGopMuxPath, 0, sizeof(gGopMuxPath));
        }
    }

    if (fsize == 0)
    {
        vfs_mount(CONFIG);
        snprintf(gGopMuxPath, sizeof(gGopMuxPath), "%s/dfbrc.ini", CONFIG_PATH);
    }
    ret = parse_dfb_ini(gGopMuxPath, &GopMux);
    if(ret != 0)
    {
        UBOOT_ERROR(" parse_dfb_ini fail...\n");
        //goto exit;
    }
    UBOOT_DEBUG("GopMux para size =: 0x%08x \n",sizeof(GopMux));

    ret = Load_DataToDbBuffer(u32DbtableOffset, E_DB_PNL_GOPMUX, (U32)&GopMux);
    UBOOT_TRACE("OK\n");
    return ret;
}

int Read_Gop_Mux_ParaFromFlash(st_gopmux_para *gopmux_para)
{
    int ret = -1;
    memset(gopmux_para, 0, sizeof(st_gopmux_para));
    ret = Read_FlashDataToAddr((U32)gopmux_para, E_DB_PNL_GOPMUX);

    UBOOT_DEBUG("gopmux_para.m_u16SettingCounts=: 0x%x\n",gopmux_para->m_u16SettingCounts);
    UBOOT_DEBUG("gopmux_para.m_u16Setting0GopIndex=: 0x%x\n",gopmux_para->m_u16Setting0GopIndex);
    UBOOT_DEBUG("gopmux_para.m_u16Setting1GopIndex=: 0x%x\n",gopmux_para->m_u16Setting1GopIndex);
    UBOOT_DEBUG("gopmux_para.m_u16Setting2GopIndex=: 0x%x\n",gopmux_para->m_u16Setting2GopIndex);
    UBOOT_DEBUG("gopmux_para.m_u16Setting3GopIndex=: 0x%x\n",gopmux_para->m_u16Setting3GopIndex);
    UBOOT_DEBUG("gopmux_para.m_u16Setting4GopIndex=: 0x%x\n",gopmux_para->m_u16Setting4GopIndex);
    return ret;
}

int Load_TConToFlash(U32 u32DbtableOffset)
{
    int ret = -1;
    unsigned int *pu32TConAddr=NULL;
    unsigned int u32fileSize   = 0;
    char * pEnv=NULL;
    UBOOT_TRACE("IN\n");

#if defined(CONFIG_DATA_SEPARATION)
    pEnv=getenv(ENV_CUSDATA);
    if (pEnv !=NULL)
    {
        UBOOT_DEBUG("Get MAP ENV_CUSDATA from env =%s \n",pEnv);
        if(vfs_mount(pEnv)!=0)
        {
            UBOOT_ERROR("mount %s fail\n",pEnv);
            goto exit;
        }
    }
    else
    {
        if(vfs_mount(CUSDATA)!=0)  //use default PATH /cusdata
        {
            UBOOT_ERROR("mount %s fail\n",CONFIG);
            goto exit;
        }
    }
#else
    vfs_mount(CONFIG);
#endif
    u32fileSize=vfs_getsize(gTconBinPath);
    if(0 == u32fileSize)
    {
        UBOOT_ERROR("vfs_getsize error: %s !\n", gTconBinPath);
        return -1;
    }
    UBOOT_DEBUG("u32fileSize=0x%x;gstDbtable.dbdata[E_DB_TCON].Offset=0x%x\n",u32fileSize,gstDbtable.dbdata[E_DB_TCON].Offset);
    pu32TConAddr=malloc(u32fileSize);
    if(pu32TConAddr == NULL)
    {
        UBOOT_ERROR("malloc pu32TConAddr memory error !");
        return -1;
    }
    memset((void*)pu32TConAddr,0,u32fileSize);
    if (vfs_read((void *)pu32TConAddr,gTconBinPath,0,u32fileSize) == 0)
    {
        ret = Load_DataToDbBuffer(u32DbtableOffset, E_DB_TCON, (U32)pu32TConAddr);
    }
    else
    {
        UBOOT_ERROR("vfs_read data fail ...>>>\n");
    }

    free(pu32TConAddr);
    pu32TConAddr = NULL;
    exit:
    UBOOT_TRACE("OK\n");
    return ret;
}

int Read_TConFromFlash(U32 u32TConAddr)
{
    int ret = 0;
    ret = Read_FlashDataToAddr(u32TConAddr, E_DB_TCON);
    return ret;
}

int Load_PnlVb1ToFlash(U32 u32DbtableOffset)
{
    int ret = -1;
    UBOOT_TRACE("IN\n");
    ret = Load_DataToDbBuffer(u32DbtableOffset, E_DB_PNL_VB1, (U32)&stVB1Channel1order);
    UBOOT_TRACE("OK\n");
    return ret;
}

int Read_PnlVb1FromFlash(pnl_VB1ChannelOrder * channelOrder_data)
{
    int ret = 0;
    UBOOT_TRACE("IN\n");
    memset(channelOrder_data, 0, sizeof(pnl_VB1ChannelOrder));
    ret = Read_FlashDataToAddr((U32)channelOrder_data, E_DB_PNL_VB1);
    UBOOT_DEBUG("channelOrder_data.u8Vb116vOrder 0             = 0x%x\n", channelOrder_data->u8Vb116vOrder[0]);
    UBOOT_DEBUG("channelOrder_data.u8Vb116vOrder 1             = 0x%x\n", channelOrder_data->u8Vb116vOrder[1]);
    UBOOT_DEBUG("channelOrder_data.u8Vb116vOrder 2             = 0x%x\n", channelOrder_data->u8Vb116vOrder[2]);
    UBOOT_DEBUG("channelOrder_data.u8Vb116vOrder 3             = 0x%x\n", channelOrder_data->u8Vb116vOrder[3]);
    UBOOT_DEBUG("channelOrder_data.u8Vb116vOrder 4             = 0x%x\n", channelOrder_data->u8Vb116vOrder[4]);
    UBOOT_DEBUG("channelOrder_data.u8Vb116vOrder 5             = 0x%x\n", channelOrder_data->u8Vb116vOrder[5]);
    UBOOT_DEBUG("channelOrder_data.u8Vb116vOrder 6             = 0x%x\n", channelOrder_data->u8Vb116vOrder[6]);
    UBOOT_DEBUG("channelOrder_data.u8Vb116vOrder 7             = 0x%x\n", channelOrder_data->u8Vb116vOrder[7]);
    UBOOT_DEBUG("channelOrder_data.u8Vb18vOrder 0             = 0x%x\n", channelOrder_data->u8Vb18vOrder[0]);
    UBOOT_DEBUG("channelOrder_data.u8Vb18vOrder 1             = 0x%x\n", channelOrder_data->u8Vb18vOrder[1]);
    UBOOT_DEBUG("channelOrder_data.u8Vb18vOrder 2             = 0x%x\n", channelOrder_data->u8Vb18vOrder[2]);
    UBOOT_DEBUG("channelOrder_data.u8Vb18vOrder 3             = 0x%x\n", channelOrder_data->u8Vb18vOrder[3]);
    UBOOT_DEBUG("channelOrder_data.u8Vb18vOrder 4             = 0x%x\n", channelOrder_data->u8Vb18vOrder[4]);
    UBOOT_DEBUG("channelOrder_data.u8Vb18vOrder 5             = 0x%x\n", channelOrder_data->u8Vb18vOrder[5]);
    UBOOT_DEBUG("channelOrder_data.u8Vb18vOrder 6             = 0x%x\n", channelOrder_data->u8Vb18vOrder[6]);
    UBOOT_DEBUG("channelOrder_data.u8Vb18vOrder 7             = 0x%x\n", channelOrder_data->u8Vb18vOrder[7]);
    UBOOT_DEBUG("channelOrder_data.u8Vb14vOrder 0             = 0x%x\n", channelOrder_data->u8Vb14vOrder[0]);
    UBOOT_DEBUG("channelOrder_data.u8Vb14vOrder 1             = 0x%x\n", channelOrder_data->u8Vb14vOrder[1]);
    UBOOT_DEBUG("channelOrder_data.u8Vb14vOrder 2             = 0x%x\n", channelOrder_data->u8Vb14vOrder[2]);
    UBOOT_DEBUG("channelOrder_data.u8Vb14vOrder 3             = 0x%x\n", channelOrder_data->u8Vb14vOrder[3]);
    UBOOT_DEBUG("channelOrder_data.u8Vb14vOrder 4             = 0x%x\n", channelOrder_data->u8Vb14vOrder[4]);
    UBOOT_DEBUG("channelOrder_data.u8Vb14vOrder 5             = 0x%x\n", channelOrder_data->u8Vb14vOrder[5]);
    UBOOT_DEBUG("channelOrder_data.u8Vb14vOrder 6             = 0x%x\n", channelOrder_data->u8Vb14vOrder[6]);
    UBOOT_DEBUG("channelOrder_data.u8Vb14vOrder 7             = 0x%x\n", channelOrder_data->u8Vb14vOrder[7]);
    UBOOT_DEBUG("channelOrder_data.u8Vb11vOrder 0             = 0x%x\n", channelOrder_data->u8Vb11vOrder[0]);
    UBOOT_DEBUG("channelOrder_data.u8Vb11vOrder 1             = 0x%x\n", channelOrder_data->u8Vb11vOrder[1]);
    UBOOT_DEBUG("channelOrder_data.u8Vb11vOrder 2             = 0x%x\n", channelOrder_data->u8Vb11vOrder[2]);
    UBOOT_DEBUG("channelOrder_data.u8Vb11vOrder 3             = 0x%x\n", channelOrder_data->u8Vb11vOrder[3]);
    UBOOT_DEBUG("channelOrder_data.u8Vb11vOrder 4             = 0x%x\n", channelOrder_data->u8Vb11vOrder[4]);
    UBOOT_DEBUG("channelOrder_data.u8Vb11vOrder 5             = 0x%x\n", channelOrder_data->u8Vb11vOrder[5]);
    UBOOT_DEBUG("channelOrder_data.u8Vb11vOrder 6             = 0x%x\n", channelOrder_data->u8Vb11vOrder[6]);
    UBOOT_DEBUG("channelOrder_data.u8Vb11vOrder 7             = 0x%x\n", channelOrder_data->u8Vb11vOrder[7]);
    UBOOT_TRACE("OK\n");
    return ret;
}

int Load_MiscSetting_ToFlash(U32 u32DbtableOffset)
{
    int ret = -1;
    UBOOT_TRACE("IN\n");
    ret = Load_DataToDbBuffer(u32DbtableOffset, E_DB_MISC, (U32)&gstSysMiscSetting);
    UBOOT_DEBUG("gstSysMiscSetting.m_u32MusicVol               = 0x%x\n", (unsigned int)gstSysMiscSetting.m_u32MusicVol);
    UBOOT_DEBUG("gstSysMiscSetting.m_u8LogoGopIdx              = 0x%x\n", gstSysMiscSetting.m_u8LogoGopIdx);
    UBOOT_DEBUG("gstSysMiscSetting.m_u16Panel_SwingLevel       = 0x%x\n", gstSysMiscSetting.m_u16Panel_SwingLevel);
    UBOOT_DEBUG("gstSysMiscSetting.m_u8MirrorMode              = 0x%x\n", gstSysMiscSetting.m_u8MirrorMode);
    UBOOT_DEBUG("gstSysMiscSetting.m_u16Panel_ext_type         = 0x%x\n", gstSysMiscSetting.m_u16Panel_ext_type);
    UBOOT_DEBUG("gstSysMiscSetting.m_Panel_LVDS_Connect_Type   = %s\n"  , gstSysMiscSetting.m_Panel_LVDS_Connect_Type);
    UBOOT_DEBUG("gstSysMiscSetting.m_u16PanelDCLK              = 0x%x\n", gstSysMiscSetting.m_u16PanelDCLK);
    UBOOT_DEBUG("gstSysMiscSetting.m_u32ursa_type              = 0x%x\n", (unsigned int)gstSysMiscSetting.m_u32ursa_type);
    UBOOT_DEBUG("gstSysMiscSetting.m_Ursa_Bin_Name             = %s\n"  , gstSysMiscSetting.m_Ursa_Bin_Name);
    UBOOT_DEBUG("gstSysMiscSetting.m_u8PixelShiftEnable        = 0x%x\n", gstSysMiscSetting.m_u8PixelShiftEnable);
    UBOOT_DEBUG("gstSysMiscSetting.m_u8MOD_H_MirrorMode        = 0x%x\n", gstSysMiscSetting.m_u8MOD_H_MirrorMode);
    UBOOT_DEBUG("gstSysMiscSetting.m_u8VideoMirrorMode         = 0x%x\n", gstSysMiscSetting.m_u8VideoMirrorMode);
    UBOOT_DEBUG("gstSysMiscSetting.m_OSDCenableFlag              = %d\n", gstSysMiscSetting.m_OSDCenableFlag);
    UBOOT_TRACE("OK\n");
    return ret;
}

int Read_MiscSetting_ToFlash(st_sys_misc_setting * misc_data)
{
    int ret = -1;
    memset(misc_data, 0, sizeof(st_sys_misc_setting));
    ret = Read_FlashDataToAddr((U32)misc_data, E_DB_MISC);
    UBOOT_DEBUG("misc_data->m_u32MusicVol               =: 0x%x\n", (unsigned int)misc_data->m_u32MusicVol);
    UBOOT_DEBUG("misc_data->m_u8LogoGopIdx              =: 0x%x\n", misc_data->m_u8LogoGopIdx);
    UBOOT_DEBUG("misc_data->m_u16Panel_SwingLevel       =: 0x%x\n", misc_data->m_u16Panel_SwingLevel);
    UBOOT_DEBUG("misc_data->m_u8MirrorMode              =: 0x%x\n", misc_data->m_u8MirrorMode);
    UBOOT_DEBUG("misc_data->m_u16Panel_ext_type         =: 0x%x\n", misc_data->m_u16Panel_ext_type);
    UBOOT_DEBUG("misc_data->m_Panel_LVDS_Connect_Type   =: %s\n"  , misc_data->m_Panel_LVDS_Connect_Type);
    UBOOT_DEBUG("misc_data->m_u16PanelDCLK              =: 0x%x\n", misc_data->m_u16PanelDCLK);
    UBOOT_DEBUG("misc_data->m_u32ursa_type              =: 0x%x\n", (unsigned int)misc_data->m_u32ursa_type);
    UBOOT_DEBUG("misc_data->m_Ursa_Bin_Name             =: %s\n"  , misc_data->m_Ursa_Bin_Name);
    UBOOT_DEBUG("misc_data.m_u8PixelShiftEnable         =: 0x%x\n", misc_data->m_u8PixelShiftEnable);
    UBOOT_DEBUG("misc_data->m_u8MOD_H_MirrorMode        =: 0x%x\n", misc_data->m_u8MOD_H_MirrorMode);
    UBOOT_DEBUG("misc_data->m_u8VideoMirrorMode         =: 0x%x\n", misc_data->m_u8VideoMirrorMode);
    return ret;
}

int Load_PWMSetting_ToFlash(U32 u32DbtableOffset)
{
    int ret = -1;
    UBOOT_TRACE("IN\n");
    ret = Load_DataToDbBuffer(u32DbtableOffset, E_DB_PWM, (U32)&gstPWMSetting);
    UBOOT_DEBUG("gstPWMSetting.m_u32PWMPeriod              = 0x%x\n", (unsigned int)gstPWMSetting.m_u32PWMPeriod);
    UBOOT_DEBUG("gstPWMSetting.m_u16DivPWM                 = 0x%x\n", gstPWMSetting.m_u16DivPWM);
    UBOOT_DEBUG("gstPWMSetting.m_u32PWMDuty                = 0x%x\n", (unsigned int)gstPWMSetting.m_u32PWMDuty);
    UBOOT_DEBUG("gstPWMSetting.m_bPolPWM                   = 0x%x\n", gstPWMSetting.m_bPolPWM);
    UBOOT_DEBUG("gstPWMSetting.m_u16maxPWM                 = 0x%x\n", gstPWMSetting.m_u16maxPWM);
    UBOOT_DEBUG("gstPWMSetting.m_u16maxPWMMsb              = 0x%x\n", gstPWMSetting.m_u16maxPWMMsb);
    UBOOT_DEBUG("gstPWMSetting.m_u16minPWM                 = 0x%x\n", gstPWMSetting.m_u16minPWM);
    UBOOT_DEBUG("gstPWMSetting.m_u16minPWMMsb              = 0x%x\n", gstPWMSetting.m_u16minPWMMsb);
    UBOOT_DEBUG("gstPWMSetting.m_u16PWMPort                = 0x%x\n", gstPWMSetting.m_u16PWMPort);
    UBOOT_TRACE("OK\n");
    return ret;
}

int Read_PWMSetting_ToFlash(st_pwm_setting * pwm_data)
{
    int ret = -1;
    ret = Read_FlashDataToAddr((U32)pwm_data, E_DB_PWM);
    UBOOT_DEBUG("pwm_data->m_u32PWMPeriod              = 0x%x\n", (unsigned int)pwm_data->m_u32PWMPeriod);
    UBOOT_DEBUG("pwm_data->m_u16DivPWM                 = 0x%x\n", pwm_data->m_u16DivPWM);
    UBOOT_DEBUG("pwm_data->m_u32PWMDuty                = 0x%x\n", (unsigned int)pwm_data->m_u32PWMDuty);
    UBOOT_DEBUG("pwm_data->m_bPolPWM                   = 0x%x\n", pwm_data->m_bPolPWM);
    UBOOT_DEBUG("pwm_data->m_u16maxPWM                 = 0x%x\n", pwm_data->m_u16maxPWM);
    UBOOT_DEBUG("pwm_data->m_u16maxPWMMsb              = 0x%x\n", pwm_data->m_u16maxPWMMsb);
    UBOOT_DEBUG("pwm_data->m_u16minPWM                 = 0x%x\n", pwm_data->m_u16minPWM);
    UBOOT_DEBUG("pwm_data->m_u16minPWMMsb              = 0x%x\n", pwm_data->m_u16minPWMMsb);
    UBOOT_DEBUG("pwm_data->m_u16PWMPort                = 0x%x\n", pwm_data->m_u16PWMPort);
    return ret;
}

int Load_HDMITXToFlash(U32 u32DbtableOffset)
{
    int ret = -1;
    UBOOT_TRACE("IN\n");
    ret = Load_DataToDbBuffer(u32DbtableOffset, E_DB_HDMITX, (U32)&stHdmitxSetting);
    UBOOT_TRACE("OK\n");
    return ret;
}

int Read_HDMITXFromFlash(st_hdmitx_setting *u32HDMITXAddr)
{
    int ret = 0;
    UBOOT_TRACE("IN\n");
    memset(u32HDMITXAddr, 0, sizeof(st_hdmitx_setting));
    ret = Read_FlashDataToAddr((U32)u32HDMITXAddr, E_DB_HDMITX);
    UBOOT_DEBUG("u32HDMITXAddr->enInColorFmt         = %d\n", u32HDMITXAddr->enInColorFmt);
    UBOOT_DEBUG("u32HDMITXAddr->enOutColorFmt        = %d\n", u32HDMITXAddr->enOutColorFmt);
    UBOOT_DEBUG("u32HDMITXAddr->enInColorQuantRange  = %d\n", u32HDMITXAddr->enInColorQuantRange);
    UBOOT_DEBUG("u32HDMITXAddr->enOutColorQuantRange = %d\n", u32HDMITXAddr->enOutColorQuantRange);
    UBOOT_DEBUG("u32HDMITXAddr->enOutputMode         = %d\n", u32HDMITXAddr->enOutputMode);
    UBOOT_DEBUG("u32HDMITXAddr->enColorDepth         = %d\n", u32HDMITXAddr->enColorDepth);
    UBOOT_DEBUG("u32HDMITXAddr->enLoadDefaultFromMboot  = %d\n", u32HDMITXAddr->enLoadDefaultFromMboot);

    // if there is no Hdmitx section in customer ini, load default value from mboot
    if(u32HDMITXAddr->enLoadDefaultFromMboot == 1)
    {
        ret = -1;
        UBOOT_TRACE("Load HDMI setting from mboot.\n");
    }

    UBOOT_TRACE("OK\n");
    return ret;
}

int Load_RGBWBinToFlash(U32 u32DbtableOffset)
{
    int ret = -1;
    unsigned int *pu32TConAddr=NULL;
    unsigned int u32fileSize   = 0;
    char* BinPath = gRGBWBinPath;
    char * pEnv=NULL;
    UBOOT_TRACE("IN\n");

#if defined(CONFIG_DATA_SEPARATION)
    pEnv=getenv(ENV_CUSDATA);
    if (pEnv !=NULL)
    {
        UBOOT_DEBUG("Get MAP ENV_CUSDATA from env =%s \n",pEnv);
        if(vfs_mount(pEnv)!=0)
        {
            UBOOT_ERROR("mount %s fail\n",pEnv);
            return -1;
        }
    }
    else
    {
        if(vfs_mount(CUSDATA)!=0)  //use default PATH /cusdata
        {
            UBOOT_ERROR("mount %s fail\n",CONFIG);
            return -1;
        }
    }
#else
    vfs_mount(CONFIG);
#endif

    u32fileSize=vfs_getsize(BinPath);
    if(0 == u32fileSize && strlen(BinPath)>0)
    {
        BinPath++;
        BinPath=strstr(BinPath,"/");
        UBOOT_INFO("filter %s, and find file again!!\n", BinPath);

        u32fileSize=vfs_getsize(BinPath);
        if(0 == u32fileSize)
        {
            UBOOT_ERROR("vfs_getsize error: %s !\n", BinPath);
            return -1;
        }
    }
    pu32TConAddr=malloc(u32fileSize);

    if(pu32TConAddr == NULL)
    {
        UBOOT_DEBUG("malloc pu32TConAddr memory error !");
        return -1;
    }
    memset((void*)pu32TConAddr,0,u32fileSize);

    if (vfs_read((void *)pu32TConAddr,BinPath,0,u32fileSize) == 0)
    {
        ret = Load_DataToDbBuffer(u32DbtableOffset, E_DB_RGBW, (U32)pu32TConAddr);
    }
    else
    {
        UBOOT_DEBUG("vfs_read data fail ...>>>\n");
    }

    free(pu32TConAddr);
    pu32TConAddr = NULL;
    UBOOT_TRACE("OK\n");
    return ret;

}

int Read_RGBWFromFlash(U32 u32RGBWAddr)
{
    int ret = 0;
    ret = Read_FlashDataToAddr(u32RGBWAddr, E_DB_RGBW);
    return ret;
}

#if (CONFIG_KEYPAD)
static int parse_keypad_ini(char *path, SAR_KpdRegCfg_t *pKeypadInfo, int keypad_select)
{
    int i = 0, j = 0;
    int iCh = 0;
    char *script = NULL;
    char str[BUFFER_SIZE];
    char str_ch[BUFFER_SIZE];
    char str_result[BUFFER_SIZE];
    U32 filesize = 0;
    UBOOT_TRACE("IN\n");
    script = loadscript(path, &filesize);
    if(script == NULL)
    {
        while (*path == '/') path++;
        path = strstr(path, "/");
        UBOOT_INFO("filter /config, and find file again!!\n");
        script = loadscript(path, &filesize);
        if(script == NULL)
        {
            return -1;
        }
    }
    UBOOT_INFO("sizeof( keypad ini script ) --------> %u bytes\n",filesize);
    Profile_Init((char *)script, filesize);

    for(i = 0; i < MAXKEYPADCH; i++)
    {
        iCh = i + 1;
        memset(&pKeypadInfo[i], 0, sizeof(SAR_KpdRegCfg_t));
        snprintf(str_ch, sizeof(str_ch), "Keypad_Ext_%d", keypad_select);
        snprintf(str, sizeof(str), "bEnable_%d", iCh);
        pKeypadInfo[i].bEnable = Profile_GetInteger(str_ch, str, 0);
        snprintf(str, sizeof(str), "u8SARChID_%d", iCh);
        pKeypadInfo[i].u8SARChID = Profile_GetInteger(str_ch, str, 0);
        snprintf(str, sizeof(str), "u8SARChUpBnd_%d", iCh);
        pKeypadInfo[i].tSARChBnd.u8UpBnd = Profile_GetInteger(str_ch, str, 0);
        snprintf(str, sizeof(str), "u8SARChLoBnd_%d", iCh);
        pKeypadInfo[i].tSARChBnd.u8LoBnd = Profile_GetInteger(str_ch, str, 0);
        snprintf(str, sizeof(str), "u8KeyLevelNum_%d", iCh);
        pKeypadInfo[i].u8KeyLevelNum = Profile_GetInteger(str_ch, str, 0);
        snprintf(str, sizeof(str), "u8KeyThreshold_%d", iCh);
        if(Profile_GetString(str_ch, str, "", str_result, sizearray(str_result)))
        {
            Str2U8Array(str_result, pKeypadInfo[i].u8KeyThreshold);
        }
        snprintf(str, sizeof(str), "u8KeyCode_%d", iCh);
        if(Profile_GetString(str_ch, str, "", str_result, sizearray(str_result)))
        {
            Str2U8Array(str_result, pKeypadInfo[i].u8KeyCode);
        }

        UBOOT_DEBUG("KeypadInfoSet[%d] = {%s, %u, %u, %u, %u)\n", \
               i, (pKeypadInfo[i].bEnable != TRUE) ? "Disable" : "Enable", \
               pKeypadInfo[i].u8SARChID, \
               pKeypadInfo[i].tSARChBnd.u8UpBnd, \
               pKeypadInfo[i].tSARChBnd.u8LoBnd, \
               pKeypadInfo[i].u8KeyLevelNum);

        for(j = 0; j < 8; j++)
        {
            UBOOT_DEBUG("0x%02x\n", pKeypadInfo[i].u8KeyThreshold[j]);
        }
        for(j = 0; j < 8; j++)
        {
            UBOOT_DEBUG("0x%02x\n", pKeypadInfo[i].u8KeyCode[j]);
        }
    }

    if(script)
        free(script);
    UBOOT_TRACE("OK\n");
    return 0;
}

int Load_KeypadToFlash(U32 u32DbtableOffset)
{
    int ret = -1;
    unsigned int *pu32KeypadAddr = NULL;
    char *s = NULL;
    char str[BUFFER_SIZE];
    int keypad_select = 0;
    UBOOT_TRACE("IN\n");

    s = getenv("keypad_select");
    if(s == NULL)
    {
        UBOOT_DEBUG("Use default keypad para!!\n");
        return 0;
    }
    else
    {
        keypad_select = (int)simple_strtol(s, NULL, 10);
        pu32KeypadAddr = malloc(gstDbtable.dbdata[E_DB_KEYPAD].Size);
        if(pu32KeypadAddr == NULL)
        {
            UBOOT_ERROR("pu32KeypadAddr malloc fail\n");
            setenv("keypad_select", "");
            return -1;
        }
        memset((void*)pu32KeypadAddr,0,gstDbtable.dbdata[E_DB_KEYPAD].Size);
        vfs_mount(CONFIG);
        if(parse_keypad_ini(gKeypadPath, (SAR_KpdRegCfg_t *)pu32KeypadAddr, keypad_select) == 0)
        {
            ret = Load_DataToDbBuffer(u32DbtableOffset, E_DB_KEYPAD, (U32)pu32KeypadAddr);
            snprintf(str, BUFFER_SIZE, "0x%x", gstDbtable.dbdata[E_DB_KEYPAD].Offset);
            setenv("keypad_offset", str);
            snprintf(str, BUFFER_SIZE, "0x%x", gstDbtable.dbdata[E_DB_KEYPAD].Size);
            setenv("keypad_size", str);
        }
        else
        {
            UBOOT_ERROR("parse_keypad_ini fail ...>>>\n");
            setenv("keypad_select", "");
            ret = -1;
        }
    }

    free(pu32KeypadAddr);
    pu32KeypadAddr = NULL;
    UBOOT_TRACE("OK\n");
    return ret;
}
#endif

int Load_3DLUTBinToFlash(U32 u32DbtableOffset)
{
    int ret = -1;
    unsigned int *pu32BinAddr=NULL;
    unsigned int u32fileSize   = 0;
    UBOOT_TRACE("IN\n");

    vfs_mount(CONFIG);
    u32fileSize=vfs_getsize(g3DLUTBinPath);
    if(0 == u32fileSize)
    {
        UBOOT_ERROR("vfs_getsize error: %s !\n", g3DLUTBinPath);
        return -1;
    }
    pu32BinAddr=malloc(u32fileSize);

    if(pu32BinAddr == NULL)
    {
        UBOOT_DEBUG("malloc pu32TConAddr memory error !");
        return -1;
    }
    memset((void*)pu32BinAddr,0,u32fileSize);

    if (vfs_read((void *)pu32BinAddr,g3DLUTBinPath,0,u32fileSize) == 0)
    {
        ret = Load_DataToDbBuffer(u32DbtableOffset, E_DB_3DLUT, (U32)pu32BinAddr);
    }
    else
    {
        UBOOT_DEBUG("vfs_read data fail ...>>>\n");
    }

    free(pu32BinAddr);
    pu32BinAddr = NULL;
    UBOOT_TRACE("OK\n");
    return ret;

}

int Read_3DLUTFromFlash(U32 u32RGBWAddr)
{
    int ret = 0;
    ret = Read_FlashDataToAddr(u32RGBWAddr, E_DB_3DLUT);
    return ret;
}

#if (CONFIG_DYNAMIC_IR)

#define MIR_SECTION_NAME "Mboot"
#define PIR_SECTION_NAME "PM51"

static int parse_ir_bootcheck_ini(char *path, st_IRBootCheck_t *p_data)
{
    long n;
    char str[BUFFER_SIZE];
    char section_name[BUFFER_SIZE];
    char *script = NULL;
    U32 filesize = 0;
    char i = 0;
    U16 *p = NULL;
    script = loadscript(path,&filesize);
    if(script == NULL)
    {
        while (*path== '/') path++;
        path=strstr(path,"/");
        UBOOT_INFO("filter /cusdata or /config, and find file again!!\n");
        script = loadscript(path,&filesize);
        if(script == NULL)
        {
            return -1;
        }
    }
    UBOOT_INFO("sizeof( ir_config ini script ) --------> %u bytes\n",filesize);
    Profile_Init((char *)script,filesize);
    for(i = 0;i<IR_MAX;i++)
    {
        snprintf(str,sizeof(str),"%d",i);
        if(Profile_GetString(MIR_SECTION_NAME, str,"", section_name, sizearray(section_name)))
        {
            printf("section = %s \n",section_name);
            n = Profile_GetBoolean(section_name, "Enable", 1);
            UBOOT_DEBUG("Enable = %ld \n",n);
            if(n == FALSE)
                continue;
            n = Profile_GetInteger(section_name, "Protocol", 0);
            UBOOT_DEBUG("Protocol = %lx \n",n);
            (*p_data)[i].u8Protocol = n;
            n = Profile_GetInteger(section_name, "Header", 0);
            UBOOT_DEBUG("Header = %lx \n",n);
            (*p_data)[i].u32Headcode = n;

            p = &((*p_data)[i].u16Key[0]);
            n = Profile_GetInteger(section_name, "RECOVERY_KEY", 0xFFFF);
            UBOOT_DEBUG("RECOVERY_KEY = %lx \n",n);
            p[0]= n;

            n = Profile_GetInteger(section_name, "UPGRADEUSBOTA_KEY", 0xFFFF);
            UBOOT_DEBUG("UPGRADEUSBOTA_KEY = %lx \n",n);
            p[1] = n;

            n = Profile_GetInteger(section_name, "FORCEUGRADE_KEY", 0xFFFF);
            UBOOT_DEBUG("FORCEUGRADE_KEY = %lx \n",n);
            p[2]= n;

            n = Profile_GetInteger(section_name, "UPGRADEBOOTLOADER_KEY", 0xFFFF);
            UBOOT_DEBUG("UPGRADEBOOTLOADER_KEY = %lx \n",n);
            p[3] = n;

            n = Profile_GetInteger(section_name, "RECOVERYWIPEDATA_KEY", 0xFFFF);
            UBOOT_DEBUG("RECOVERYWIPEDATA_KEY = %lx \n",n);
            p[4] = n;

            n = Profile_GetInteger(section_name, "RECOVERYWIPECACHE_KEY", 0xFFFF);
            UBOOT_DEBUG("RECOVERYWIPECACHE_KEY = %lx \n",n);
            p[5]= n;

            n = Profile_GetInteger(section_name, "SECOND_SYSTEM_KEY", 0xFFFF);
            UBOOT_DEBUG("SECOND_SYSTEM_KEY = %lx \n",n);
            p[6] = n;

            n = Profile_GetInteger(section_name, "FASTBOOT_KEY", 0xFFFF);
            UBOOT_DEBUG("FASTBOOT_KEY = %lx \n",n);
            p[7] = n;

            n = Profile_GetInteger(section_name, "BRICK_TERMINATOR_RECOVERY_KEY", 0xFFFF);
            UBOOT_DEBUG("BRICK_TERMINATOR_RECOVERY_KEY = %lx \n",n);
            p[8] = n;
        }
        else
            continue;
    }

    if(script)
        free(script);
    return 0;

}
static int parse_ir_wakeup_ini(char *path, st_IRWakeup_t *p_data)
{
    long n;
    char str[BUFFER_SIZE];
    char section_name[BUFFER_SIZE];
    char *script = NULL;
    U32 filesize = 0;
    char i = 0;
    U16 *p = NULL;
    script = loadscript(path,&filesize);
    if(script == NULL)
    {
        while (*path== '/') path++;
        path=strstr(path,"/");
        UBOOT_INFO("filter /cusdata or /config, and find file again!!\n");
        script = loadscript(path,&filesize);
        if(script == NULL)
        {
            return -1;
        }
    }
    UBOOT_INFO("sizeof( ir_config ini script ) --------> %u bytes\n",filesize);
    Profile_Init((char *)script,filesize);
    for(i = 0;i<IR_MAX;i++)
    {
        snprintf(str,sizeof(str),"%d",i);
        if(Profile_GetString(PIR_SECTION_NAME, str,"", section_name, sizearray(section_name)))
        {
            printf("section = %s \n",section_name);
            n = Profile_GetInteger(section_name, "Protocol", 0);
            UBOOT_DEBUG("Protocol = %lx \n",n);
            (*p_data)[i].u8Protocol = n;
            n = Profile_GetInteger(section_name, "Header", 0);
            UBOOT_DEBUG("Header = %lx \n",n);
            (*p_data)[i].u32Headcode = n;
            n = Profile_GetInteger(section_name, "Key", 0xFFFF);
            UBOOT_DEBUG("WAKEUP_KEY = %lx \n",n);
            (*p_data)[i].u16Key = n;
        }
        else
            continue;
    }

    if(script)
        free(script);
    return 0;

}

int Load_IRConfig_ToFlash(U32 u32DbtableOffset)
{
    int ret = -1;
    st_IRBootCheck_t bootcheck_info;
    st_IRWakeup_t wakeup_info;
#if defined(CONFIG_DATA_SEPARATION)
    char * pEnv=NULL;
#endif
    UBOOT_TRACE("IN\n");
    memset(&bootcheck_info,0,sizeof(bootcheck_info));
    memset(&wakeup_info,0,sizeof(wakeup_info));
#if defined(CONFIG_DATA_SEPARATION)
    pEnv=getenv(ENV_CUSDATA);
    if (pEnv !=NULL)
    {
        UBOOT_DEBUG("Get MAP ENV_CUSDATA from env =%s \n",pEnv);
        if(vfs_mount(pEnv)!=0)
        {
            UBOOT_ERROR("mount %s fail\n",pEnv);
            goto exit;
        }
    }
    else
    {
        if(vfs_mount(CUSDATA)!=0)  //use default PATH /cusdata
        {
            UBOOT_ERROR("mount %s fail\n",CONFIG);
            goto exit;
        }
    }
#else
    char *pIRConfigPartition = NULL;
    pIRConfigPartition = getenv("IR_CONFIG_PARTITION");
    if(NULL != pIRConfigPartition)
    {
        vfs_mount(pIRConfigPartition);
    }
    else
    {
        vfs_mount(CONFIG);
    }
#endif

#if defined(CONFIG_DATA_SEPARATION)
#else
    memset(gIRConfigPath,0,sizeof(gIRConfigPath));
    char *pIRConfigPath = NULL;
    pIRConfigPath = getenv("IR_CONFIG_PATH");
    if(NULL != pIRConfigPath)
    {
        snprintf(gIRConfigPath,sizeof(gIRConfigPath),"%s",pIRConfigPath);
    }
    else
    {
        snprintf(gIRConfigPath,sizeof(gIRConfigPath),"%s/ir_config.ini",CONFIG_PATH);
    }
#endif
    if(parse_ir_bootcheck_ini(gIRConfigPath, &bootcheck_info)!=0)
    {
        UBOOT_ERROR("parse_ir_bootcheck_ini fail !!\n");
        return ret;
    }

    if(parse_ir_wakeup_ini(gIRConfigPath, &wakeup_info)!=0)
    {
        UBOOT_ERROR("parse_ir_wakeup_ini fail !!\n");
        return ret;
    }

    UBOOT_DEBUG("bootcheck_info size =: 0x%08x \n",sizeof(bootcheck_info));
    UBOOT_DEBUG("wakeup_info size =: 0x%08x \n",sizeof(wakeup_info));
    ret = Load_DataToDbBuffer(u32DbtableOffset, E_DB_IR_BOOTCHECK, (U32)&bootcheck_info);
    ret = Load_DataToDbBuffer(u32DbtableOffset, E_DB_IR_WAKEUP, (U32)&wakeup_info);

exit:
    UBOOT_TRACE("OK\n");
    return ret;
}

int Read_IRBootcheck_FromFlash(st_IRBootCheck_t * bootcheck_info)
{
    int ret = -1;
    unsigned char i = 0;
    if(bootcheck_info == NULL)
        return ret;
    ret = Read_FlashDataToAddr((U32)bootcheck_info, E_DB_IR_BOOTCHECK);
    for(i =0;i<IR_MAX;i++)
    {
        unsigned char j = 0;
        U16 * p;
        UBOOT_DEBUG("=====================================\n");
        UBOOT_DEBUG("IR%d :\n",i);
        UBOOT_DEBUG("u8Protocol = %x\n",(*bootcheck_info)[i].u8Protocol);
        UBOOT_DEBUG("u32Headcode = %x\n",(*bootcheck_info)[i].u32Headcode);
        p = &((*bootcheck_info)[i].u16Key[0]);
        for(j = 0;j<IR_KEY_MAX;j++)
        {
            UBOOT_DEBUG("key%d = %x\n",j,p[j]);
        }
    }
    return ret;
}

int Read_IRWakeup_FromFlash(st_IRWakeup_t * wakeup_info)
{
    int ret = -1;
    unsigned char i = 0;
    ret = Read_FlashDataToAddr((U32)wakeup_info, E_DB_IR_WAKEUP);
    for(i =0;i<IR_MAX;i++)
    {
        UBOOT_DEBUG("=====================================\n");
        UBOOT_DEBUG("IR%d :\n",i);
        UBOOT_DEBUG("u8Protocol = %x\n",(*wakeup_info)[i].u8Protocol);
        UBOOT_DEBUG("u32Headcode = %x\n",(*wakeup_info)[i].u32Headcode);
        UBOOT_DEBUG("u16Key = %x\n",(*wakeup_info)[i].u16Key);
    }
    return ret;
}

#endif
#if (CONFIG_LOCAL_DIMMING)
#include <drvMSPI.h>
#include <drvLDMA.h>
#define LDM_SECTION_NAME "begin"
static int parse_ldm_mspi_ini(char *path, ST_DRV_MSPI_INFO *p_data)
{
    long n;
    char str[BUFFER_SIZE];
    char *script = NULL;
    U32 filesize = 0;
    script = loadscript(path,&filesize);
    if(script == NULL)
    {
        while (*path== '/') path++;
        path=strstr(path,"/");
        UBOOT_INFO("filter /config, and find file again!!\n");
        script = loadscript(path,&filesize);
        if(script == NULL)
        {
            return -1;
        }
    }
    UBOOT_INFO("sizeof( ldm ini script ) --------> %u bytes\n",filesize);
    Profile_Init((char *)script,filesize);

    n = Profile_GetInteger(LDM_SECTION_NAME, "u8MspiChanel", 0);
    UBOOT_DEBUG("u8MspiChanel = %ld \n",n);
    p_data->u8MspiChanel = n;

    n = Profile_GetInteger(LDM_SECTION_NAME, "u8MspiMode", 0);
    UBOOT_DEBUG("u8MspiMode = %ld \n",n);
    p_data->u8MspiMode = n;

    n = Profile_GetInteger(LDM_SECTION_NAME, "u32MspiClk", 0);
    UBOOT_DEBUG("u32MspiClk = %ld \n",n);
    p_data->u32MspiClk = n;

    if(Profile_GetString(LDM_SECTION_NAME, "u8WBitConfig[8]","", str, sizearray(str)))
    {
        Str2U8Array(str, p_data->u8WBitConfig);
    }

    if(Profile_GetString(LDM_SECTION_NAME, "u8RBitConfig[8]","", str, sizearray(str)))
    {
        Str2U8Array(str, p_data->u8RBitConfig);
    }

    n = Profile_GetInteger(LDM_SECTION_NAME, "u8TrStart", 0);
    UBOOT_DEBUG("u8TrStart = %ld \n",n);
    p_data->u8TrStart = n;

    n = Profile_GetInteger(LDM_SECTION_NAME, "u8TrEnd", 0);
    UBOOT_DEBUG("u8TrEnd = %ld \n",n);
    p_data->u8TrEnd = n;

    n = Profile_GetInteger(LDM_SECTION_NAME, "u8TB", 0);
    UBOOT_DEBUG("u8TB = %ld \n",n);
    p_data->u8TB = n;

    n = Profile_GetInteger(LDM_SECTION_NAME, "u8TRW", 0);
    UBOOT_DEBUG("u8TRW = %ld \n",n);
    p_data->u8TRW = n;

    if(script)
        free(script);
    return 0;
}

static int parse_ldm_dma_ini(char *path, ST_DRV_LD_DMA_INFO *p_data)
{
    long n;
    char *script = NULL;
    char str[BUFFER_SIZE];
    U32 filesize = 0;
    script = loadscript(path,&filesize);
    if(script == NULL)
    {
        while (*path== '/') path++;
        path=strstr(path,"/");
        UBOOT_INFO("filter /config, and find file again!!\n");
        script = loadscript(path,&filesize);
        if(script == NULL)
        {
            return -1;
        }
    }
    UBOOT_INFO("sizeof( ldm ini script ) --------> %u bytes\n",filesize);
    Profile_Init((char *)script,filesize);

    n = Profile_GetInteger(LDM_SECTION_NAME, "eLEDType", 0);
    UBOOT_DEBUG("eLEDType = %ld \n",n);
    p_data->eLEDType = n;

      n = Profile_GetInteger(LDM_SECTION_NAME, "u8LEDWidth", 0);
    UBOOT_DEBUG("u8LEDWidth = %ld \n",n);
    p_data->u8LEDWidth = n;
        n = Profile_GetInteger(LDM_SECTION_NAME, "u8LEDHeight", 0);
    UBOOT_DEBUG("u8LEDHeight = %ld \n",n);
    p_data->u8LEDHeight = n;
        n = Profile_GetInteger(LDM_SECTION_NAME, "u8LDFWidth", 0);
    UBOOT_DEBUG("u8LDFWidth = %ld \n",n);
    p_data->u8LDFWidth = n;
        n = Profile_GetInteger(LDM_SECTION_NAME, "u8LDFHeight", 0);
    UBOOT_DEBUG("u8LDFHeight = %ld \n",n);
    p_data->u8LDFHeight = n;
        n = Profile_GetInteger(LDM_SECTION_NAME, "u8LSFWidth", 0);
    UBOOT_DEBUG("u8LSFWidth = %ld \n",n);
    p_data->u8LSFWidth = n;
        n = Profile_GetInteger(LDM_SECTION_NAME, "u8LSFHeight", 0);
    UBOOT_DEBUG("u8LSFHeight = %ld \n",n);
    p_data->u8LSFHeight = n;


    n = Profile_GetInteger(LDM_SECTION_NAME, "bEdge2DEn", 0);
    UBOOT_DEBUG("bEdge2DEn = %ld \n",n);
    p_data->bEdge2DEn = n;

    n = Profile_GetInteger(LDM_SECTION_NAME, "u8LDMAchanel", 0);
    UBOOT_DEBUG("u8LDMAchanel = %ld \n",n);
    p_data->u8LDMAchanel = n;

    n = Profile_GetInteger(LDM_SECTION_NAME, "u8LDMATrimode", 0);
    UBOOT_DEBUG("u8LDMATrimode = %ld \n",n);
    p_data->u8LDMATrimode = n;

    n = Profile_GetInteger(LDM_SECTION_NAME, "u8LDMACheckSumMode", 0);
    UBOOT_DEBUG("u8LDMACheckSumMode = %ld \n",n);
    p_data->u8LDMACheckSumMode = n;

    n = Profile_GetInteger(LDM_SECTION_NAME, "u8cmdlength", 0);
    UBOOT_DEBUG("u8cmdlength = %ld \n",n);
    p_data->u8cmdlength = n;

    if(Profile_GetString(LDM_SECTION_NAME, "u16MspiHead[8]","", str, sizearray(str)))
    {
        Str2U16Array(str, p_data->u16MspiHead);
    }

    n = Profile_GetInteger(LDM_SECTION_NAME, "u16LedNum", 0);
    UBOOT_DEBUG("u16LedNum = %ld \n",n);
    p_data->u16LedNum = n;

    if(Profile_GetString(LDM_SECTION_NAME, "u16DMADelay[4]","", str, sizearray(str)))
    {
        Str2U16Array(str, p_data->u16DMADelay);
    }

    n = Profile_GetInteger(LDM_SECTION_NAME, "u8ClkHz", 0);
    UBOOT_DEBUG("u8ClkHz = %ld \n",n);
    p_data->u8ClkHz = n;

    if(script)
        free(script);
    return 0;
}

int Load_LDMPara_ToFlash(U32 u32DbtableOffset)
{
    int ret = -1;
    ST_DRV_MSPI_INFO mspi_info;
    ST_DRV_LD_DMA_INFO dma_info;
    UBOOT_TRACE("IN\n");
    memset(&mspi_info,0,sizeof(mspi_info));
    vfs_mount(CONFIG);

    if(parse_ldm_mspi_ini(gLocalDimmingPath, &mspi_info)!=0)
    {
        UBOOT_ERROR("parse_ldm_mspi_ini fail !!\n");
        return ret;
    }

    if(parse_ldm_dma_ini(gLocalDimmingPath, &dma_info)!=0)
    {
        UBOOT_ERROR("parse_ldm_dma_ini fail !!\n");
        return ret;
    }

    UBOOT_DEBUG("mspi_info size =: 0x%08x \n",sizeof(mspi_info));
    UBOOT_DEBUG("dma_info size =: 0x%08x \n",sizeof(dma_info));
    ret = Load_DataToDbBuffer(u32DbtableOffset, E_DB_MSPI, (U32)&mspi_info);
    ret = Load_DataToDbBuffer(u32DbtableOffset, E_DB_DMA, (U32)&dma_info);
    UBOOT_TRACE("OK\n");
    return ret;
}

int Read_MSPIPara_FromFash(ST_DRV_MSPI_INFO * mspi_info)
{
    int ret = -1;
    ret = Read_FlashDataToAddr((U32)mspi_info, E_DB_MSPI);

    UBOOT_DEBUG("spi_info.u8MspiChanel=: 0x%x\n",mspi_info->u8MspiChanel);
    UBOOT_DEBUG("spi_info.u8MspiMode=: 0x%x\n",mspi_info->u8MspiMode);
    UBOOT_DEBUG("spi_info.u32MspiClk=: %ld\n",mspi_info->u32MspiClk);
    UBOOT_DEBUG("spi_info.u8WBitConfig=: 0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x\n",mspi_info->u8WBitConfig[0],mspi_info->u8WBitConfig[1] \
                     ,mspi_info->u8WBitConfig[2],mspi_info->u8WBitConfig[3],mspi_info->u8WBitConfig[4],mspi_info->u8WBitConfig[5]
                     ,mspi_info->u8WBitConfig[6],mspi_info->u8WBitConfig[7]);
    UBOOT_DEBUG("spi_info.u8RBitConfig=: 0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x\n",mspi_info->u8RBitConfig[0],mspi_info->u8RBitConfig[1] \
                    ,mspi_info->u8RBitConfig[2],mspi_info->u8RBitConfig[3],mspi_info->u8RBitConfig[4],mspi_info->u8RBitConfig[5]
                    ,mspi_info->u8RBitConfig[6],mspi_info->u8RBitConfig[7]);
    UBOOT_DEBUG("spi_info.u8TrStart=: 0x%x\n",mspi_info->u8TrStart);
    UBOOT_DEBUG("spi_info.u8TrEnd=: 0x%x\n",mspi_info->u8TrEnd);
    UBOOT_DEBUG("spi_info.u8TB=: 0x%x\n",mspi_info->u8TB);
    UBOOT_DEBUG("spi_info.u8TRW=: 0x%x\n",mspi_info->u8TRW);

    return ret;
}

int Read_DMAPara_FromFash(ST_DRV_LD_DMA_INFO * dma_info)
{
    int ret = -1;
    ret = Read_FlashDataToAddr((U32)dma_info, E_DB_DMA);

    UBOOT_DEBUG("dma_info.eLEDType=: 0x%x\n", dma_info->eLEDType);
    UBOOT_DEBUG("dma_info.u8LEDWidth=: %d\n", dma_info->u8LEDWidth);
    UBOOT_DEBUG("dma_info.u8LEDHeight=: %d\n", dma_info->u8LEDHeight);
    UBOOT_DEBUG("dma_info.u8LDFWidth=: %d\n", dma_info->u8LDFWidth);
    UBOOT_DEBUG("dma_info.u8LDFHeight=: %d\n", dma_info->u8LDFHeight);
    UBOOT_DEBUG("dma_info.u8LSFWidth=: %d\n", dma_info->u8LSFWidth);
    UBOOT_DEBUG("dma_info.u8LSFHeight=: %d\n", dma_info->u8LSFHeight);
    UBOOT_DEBUG("dma_info.bEdge2DEn=: 0x%x\n", dma_info->bEdge2DEn);

    UBOOT_DEBUG("dma_info.u8LDMAchanel=: 0x%x\n", dma_info->u8LDMAchanel);
    UBOOT_DEBUG("dma_info.u8LDMATrimode=: 0x%x\n", dma_info->u8LDMATrimode);
    UBOOT_DEBUG("dma_info.u8LDMACheckSumMode=: 0x%x\n", dma_info->u8LDMACheckSumMode);
    UBOOT_DEBUG("dma_info.u16DMADelay=: 0x%x, 0x%x, 0x%x, 0x%x\n", dma_info->u16DMADelay[0],dma_info->u16DMADelay[1]  \
                , dma_info->u16DMADelay[2],dma_info->u16DMADelay[3]);
    UBOOT_DEBUG("dma_info.u8cmdlength=: 0x%x\n", dma_info->u8cmdlength);
    UBOOT_DEBUG("dma_info.u16MspiHead=: 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x\n", dma_info->u16MspiHead[0],dma_info->u16MspiHead[1] \
      , dma_info->u16MspiHead[2],dma_info->u16MspiHead[3], dma_info->u16MspiHead[4],dma_info->u16MspiHead[5]
      , dma_info->u16MspiHead[6],dma_info->u16MspiHead[7]);
    UBOOT_DEBUG("dma_info.u16LedNum=: 0x%x\n", dma_info->u16LedNum);
    UBOOT_DEBUG("dma_info.u8ClkHz=: %d\n", dma_info->u8ClkHz);
    return ret;
}
#endif
