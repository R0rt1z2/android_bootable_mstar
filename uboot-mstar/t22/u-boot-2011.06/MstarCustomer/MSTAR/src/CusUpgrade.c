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
#include <MsTypes.h>
#include <CusUpgrade.h>
#include <CusUpgradeUtility.h>
#include <MsUpgrade.h>
#include <MsUpgradeUtility.h>
#include <MsSystem.h>
#include <MsUtility.h>
#include <MsVfs.h>
#include <MsDebug.h>
#include <fat.h>
#include <MsBoot.h>
#include <MsAppCB.h>
#include <MsOS.h>
#include <apiSWI2C.h>
#include <CusCastTV.h>
#include"mstar_string_emnu.h"
#include <ursa/ursa_common.h>
#if defined (CONFIG_URSA_6M60)
#include <ursa/ursa_6m60.h>
#endif
#if defined (CONFIG_URSA20_VB1)
#include <ursa/ursa_20.h>
#endif

#if (ENABLE_MODULE_USB == 1)
#include <usb.h>
#endif
#if (ENABLE_MODULE_DISPLAY_OSD==1)
int display_osd = 0;
#endif

#if defined(CONFIG_CMD_WRITE_DATA_TO_RAW_PARTITION_ENABLE) && (CONFIG_CMD_WRITE_DATA_TO_RAW_PARTITION_ENABLE==1)
#include <MsRawIO.h>
#endif
#include <bootlogo/iniparser.h>

#ifdef UFBL_FEATURE_IDME
#include <idme.h>
#endif
#define PATH_BUFFER_SIZE 128
#define URSA_BIN_SECTION_NAME "URSA_BIN"
#define URSA_BIN_KEY_NAME "URSA_BIN_FILE_PATH"
#define URSA_UPGRADE_ODM_PARTITION "odm"

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define GWIN_WIDTH              720
#define GWIN_HEIGHT             576
#define GRAPHIC_WIDTH           600
#define GRAPHIC_HEIGHT          400
#define GRAPHIC_X               60
#define GRAPHIC_Y               88
#define LINE_HEIGHT             50
#define RECT_LEFT_INTERVAL      50
#define UPGRADE_BUFFER_SIZE 0x4000000
#if ENABLE_DISPLAY_PICTURE_UPDATE
#define GWIN_PIC_WIDTH              1280
#define GWIN_PIC_HEIGHT             720
#define GRAPHIC_PIC_WIDTH           1280
#define GRAPHIC_PIC_HEIGHT          720
#define GRAPHIC_PIC_X               0
#define GRAPHIC_PIC_Y               0
#define EN_PIC_BASE_PATH            "updating_en_base.jpg"
#define EN_PIC_ERROR_PATH           "updating_en_unsuccessful.jpg"
#define EN_PIC_PROGRESS_X           560
#define EN_PIC_PROGRESS_Y           220
#define EN_PIC_PROGRESS_WIDTH       80
#define EN_PIC_PROGRESS_HEIGHT      60
#define EN_PIC_ERROR_X              570
#define EN_PIC_ERROR_Y              200
#define EN_PIC_ERROR_WIDTH          200
#define EN_PIC_ERROR_HEIGHT         112
#endif

//config update led Enable
#define CONFIG_GPIO_LED_ENABLE 0
#define CONFIG_PWM_LED_ENABLE  0

#define AP_BIN_PATH             getenv("ForceUpgradePath")==NULL?"/MstarUpgrade.bin":getenv("ForceUpgradePath")
#define UL_BIN_PATH             getenv("UpdateLogicUpgradeFile")==NULL?"MstarUpgrade.bin":getenv("UpdateLogicUpgradeFile")
#define OAD_BIN_PATH            getenv("OadUpgradeFile")==NULL?"MstarUpgrade.bin":getenv("OadUpgradeFile")
#define NET_BIN_PATH            AP_BIN_PATH
#define MBOOT_BIN_PATH          "/MbootUpgrade.bin"
#define SBOOT_BIN_PATH          "/SbootUpgrade.bin"
#define URSA_BIN_PATH           getenv("UrsaUpgradePath")==NULL?"/UrsaUpgrade.bin":getenv("UrsaUpgradePath")
#define URSA_UPGRADE_TVCONFIG_PARTITION        "tvconfig"
#define URSA_UPGRADE_URSA_BIN_PATH             "config/pq/ursa_upgrade.bin"
#define URSA_VERSION_MAGIC_0                   0x78
#define URSA_VERSION_MAGIC_1                   0x56
#define URSA_VERSION_MAGIC_2                   0xAA
#define URSA_VERSION_MAGIC_3                   0x55
#define URSA_VERSION_MAGIC_OFFSET              0x18
#define URSA_COMPRESS_LENGTH_OFFSET            (0x9C)
#define URSA_VERSION_BUFFER                    16
#define DEFAULT_CUSTOMER_INI_PATH        "/config/model/Customer_1.ini" // default path: /vendor/tvconfig/config/model/Customer_1.ini
#define URSA_IDME_COLUMN                       "memc"
#define OTA_ZIP_PATH            "/update_signed.zip"


#define UPGRADE_STATE_URSA_BIN            (BIT0)
#define UPGRADE_STATE_SBOOT_BIN           (BIT1)
#define UPGRADE_STATE_MBOOT_BIN           (BIT2)
#define UPGRADE_STATE_AP_BIN              (BIT3)
#if defined (CONFIG_URSA_6M60)
#define UPGRADE_STATE_TERRA2_BIN          (BIT4)
#endif
#define FORCE_UPDATE_URSA_ENABLE "URSA_UPDATE_ENABLE"

//============for ISP Upgrade============
#if defined (CONFIG_UPGRADE_URSA)
#define URSA_UPGRADE_BIN_PATH  getenv("UrsaUpgradePath")==NULL?"/UrsaUpgrade.bin":getenv("UrsaUpgradePath")

#define URSA_ISP_I2C_BUS_SEL 0x0
#define BLOCK_ERASE_ERROR_MAX      2000

#define BANKSIZE    0x10000
#define BLOCKNUM    0xff
#define BLOCKSIZE   0x100
#define SYSBIN_SIZE 1024 // 1KB

#define ENTER_ISP_ERROR_MAX     20
#define GET_ID_ERROR_MAX        10

#define URSA_I2C_BUS_NUM        (1)

#define URSA_ISP_I2C_BUS_SEL 0x0
#define SPI_WRITE           0x10
#define SPI_READ            0x11
#define SPI_STOP            0x12
#define CMD_DATA_WRITE      0x10

#define  SW_IIC_READ             0
#define  SW_IIC_WRITE            1

MS_U8 g_ucFlashType;
MS_U8 g_ucBankDst;
MS_U8 g_ucBankNumber;
EN_MFC_UPGRADE_SEL_t g_eUrsaUpgradeSel = E_MFC_FIRMWARE; //upgrade MFC FW by default
SFlashType sFlash;

MS_U8 g_u8CurPercent;
MS_U32 g_u32ISPSrcAddr;
MS_U32 g_u32ISPCodeSize;
FlashProgErrorType ErrorFlag;
FlashProgStateType g_eFlashProgramState;

#endif
//========ISP END=================

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
#if (CONFIG_DISPLAY_OSD)
static const U32 progress_max_value = 1000;

#if ((ENABLE_MODULE_USB == 1) || \
     (ENABLE_MODULE_NETUPDATE == 1) || \
     (ENABLE_MODULE_OAD == 1))
static U32 progress_cnt = 0;
static U32 progress_start_load_value = 10;
static U32 progress_finish_load_value = 200;
#endif
#if (ENABLE_DISPLAY_PICTURE_UPDATE)
static U32 progress_cnt_last = 0;
#endif
#endif

extern MS_U8 m_UrsaBootFlag;
static MS_BOOL m_bFullUpgrade = TRUE;
static MS_U8 upgrade_percentage = 0;
extern MS_U8 m_UrsaFastUpgradeEnable;

enum
{
    I2cUseBusN = 0x34,
    I2cUseBus = 0x35,
    CpuWaitN = 0x36,
    CpuWait = 0x37,

    SerialDebugEnter = 0x44,
    SerialDebugExit = 0x45,

    MFC_SetAddrFmt2Byte = 0x51,
    MFC_DisableNewCfg = 0x7E,
    MFC_EnableNewCfg = 0x7F,
    MFC_ChNumBit0Disable = 0x80,
    MFC_ChNumBit0Enable = 0x81,
    MFC_ChNumBit1Disable = 0x82,
    MFC_ChNumBit1Enable = 0x83,
    MFC_ChNumBit2Disable = 0x84,
    MFC_ChNumBit2Enable = 0x85,
};

//-------------------------------------------------------------------------------------------------
//  Extern Functions
//-------------------------------------------------------------------------------------------------
#if defined(CONFIG_MMC)
extern int get_mmc_part_size_and_offset (char *puPartName, unsigned int *u32PartSize, unsigned int *u32PartOffset);
#endif

extern int idme_get_var_external(const char *name, char *buf, unsigned int length);

//-------------------------------------------------------------------------------------------------
//  Private Functions
//-------------------------------------------------------------------------------------------------
#if (ENABLE_MODULE_USB == 1)
static int check_bin_flag(void);
#endif

#if (((ENABLE_MODULE_USB == 1)||(ENABLE_MODULE_NETUPDATE == 1)||(ENABLE_MODULE_OAD == 1))&&(ENABLE_MODULE_DISPLAY_OSD==1))
static int show_LoadData(int var);
static int show_StartUpgrading(int var);
static int show_Upgrading(int current_cnt, int total_cnt);
static int show_Error(int var);
static int show_Finish(int var);
static int show_test_start(int var);
static int show_test(int var, char * string1, char * string2, int current_cnt, int total_cnt);
#endif

//-------------------------------------------------------------------------------------------------
//  Public Functions
//-------------------------------------------------------------------------------------------------
#if (((ENABLE_MODULE_USB == 1)||(ENABLE_MODULE_NETUPDATE == 1)||(ENABLE_MODULE_OAD == 1))&&(ENABLE_MODULE_DISPLAY_OSD==1))
static int show_LoadData(int var)
{
    if (-1 == display_osd)
        return -1;
    char buffer[CMD_BUF]="\0";
    int ret = 0;
    UBOOT_TRACE("IN\n");

    if (1 == CONFIG_GPIO_LED_ENABLE)
    {
        run_command("led time 100",0); //GPIO led Frequency
    }
    else if (1 == CONFIG_PWM_LED_ENABLE)
    {
        run_command("led pwm_start",0);
    }

    progress_cnt = 0;
    progress_start_load_value = 10;
    progress_finish_load_value = 200;
#if (ENABLE_DISPLAY_PICTURE_UPDATE)
    snprintf(buffer, CMD_BUF, "osd_create %d %d", GWIN_PIC_WIDTH, GWIN_PIC_HEIGHT);
#else
    snprintf(buffer, CMD_BUF, "osd_create %d %d", GWIN_WIDTH, GWIN_HEIGHT);
#endif
    UBOOT_DEBUG("cmd=%s\n",buffer);
    ret = run_command(buffer, 0);
    if (-1 == ret)
    {
        display_osd = -1;
        return -1;
    }
#if (ENABLE_DISPLAY_PICTURE_UPDATE)
    memset(buffer, 0 , CMD_BUF);
    snprintf(buffer, CMD_BUF, "draw_jpg -fs %d %d %d %d %s",GRAPHIC_PIC_X, GRAPHIC_PIC_Y, GRAPHIC_PIC_WIDTH, GRAPHIC_PIC_HEIGHT, EN_PIC_BASE_PATH);
    UBOOT_DEBUG("cmd=%s\n",buffer);
    run_command(buffer, 0);
#else
    memset(buffer, 0 , CMD_BUF);
    snprintf(buffer, CMD_BUF, "draw_rect %d %d %d %d 0x800000ff", GRAPHIC_X, GRAPHIC_Y, GRAPHIC_WIDTH, GRAPHIC_HEIGHT);
    UBOOT_DEBUG("cmd=%s\n",buffer);
    run_command(buffer, 0);
    memset(buffer, 0 , CMD_BUF);
    snprintf(buffer, CMD_BUF, "draw_string %d %d 0x3fffffff 1 %x", GRAPHIC_X, GRAPHIC_Y + LINE_HEIGHT * 2,en_load_data);
    UBOOT_DEBUG("cmd=%s\n",buffer);
    run_command(buffer, 0);
    memset(buffer, 0 , CMD_BUF);
    snprintf(buffer, CMD_BUF, "draw_progress %d %d 0x3fffffff %d", GRAPHIC_X + RECT_LEFT_INTERVAL, GRAPHIC_Y + LINE_HEIGHT * 5, progress_start_load_value/10);
    UBOOT_DEBUG("cmd=%s\n",buffer);
    run_command(buffer, 0);
#endif
    memset(buffer, 0 , CMD_BUF);
    snprintf(buffer, CMD_BUF, "osd_flush");
    UBOOT_DEBUG("cmd=%s\n",buffer);
    run_command(buffer, 0);
    UBOOT_TRACE("OK\n");
    return 0;
}

static int show_StartUpgrading(int var)
{
    if (-1 == display_osd)
        return -1;
#if (ENABLE_DISPLAY_PICTURE_UPDATE)
    Spinner_Timer_ISR_Register();
#else
    char buffer[CMD_BUF]="\0";
    UBOOT_TRACE("IN\n");
    snprintf(buffer, CMD_BUF, "draw_rect %d %d %d %d 0x800000ff", GRAPHIC_X, GRAPHIC_Y, GRAPHIC_WIDTH, GRAPHIC_HEIGHT);
    UBOOT_DEBUG("cmd=%s\n",buffer);
    run_command(buffer, 0);
    memset(buffer, 0 , CMD_BUF);
    snprintf(buffer, CMD_BUF, "draw_string %d %d 0x3fffffff 1 %x", GRAPHIC_X, GRAPHIC_Y + LINE_HEIGHT * 2,en_upgrading_software);
    UBOOT_DEBUG("cmd=%s\n",buffer);
    run_command(buffer, 0);
    memset(buffer, 0 , CMD_BUF);
    snprintf(buffer, CMD_BUF, "draw_string %d %d 0x3fffffff 1 %x", GRAPHIC_X,  GRAPHIC_Y + LINE_HEIGHT * 3,en_not_turn_off);
    UBOOT_DEBUG("cmd=%s\n",buffer);
    run_command(buffer, 0);
    memset(buffer, 0 , CMD_BUF);
    snprintf(buffer, CMD_BUF, "draw_progress %d %d 0x3fffffff %d", GRAPHIC_X + RECT_LEFT_INTERVAL, GRAPHIC_Y + LINE_HEIGHT * 5, progress_finish_load_value/10);
    UBOOT_DEBUG("cmd=%s\n",buffer);
    run_command(buffer, 0);
    memset(buffer, 0 , CMD_BUF);
    snprintf(buffer, CMD_BUF, "osd_flush");
    run_command(buffer, 0);
#endif
    UBOOT_TRACE("OK\n");
    return 0;
}

static int show_Upgrading(int current_cnt, int total_cnt)
{
    if (-1 == display_osd)
        return -1;

    char buffer[CMD_BUF]="\0";
    UBOOT_TRACE("IN\n");
    progress_cnt = (((progress_max_value - progress_finish_load_value)*1000)/total_cnt); //(1000-200)/cmd_cnt
    progress_cnt = (progress_cnt *current_cnt+progress_finish_load_value*1000)/(10*1000);
    printf("--%d %%\n", progress_cnt);
#if (ENABLE_DISPLAY_PICTURE_UPDATE)
    if( progress_cnt_last == progress_cnt)
    {
        return 0;
    }
    progress_cnt_last = progress_cnt;
    memset(buffer, 0 , CMD_BUF);
    snprintf(buffer, CMD_BUF, "draw_jpg -fs %d %d %d %d updating_%d.jpg", EN_PIC_PROGRESS_X, EN_PIC_PROGRESS_Y, EN_PIC_PROGRESS_WIDTH, EN_PIC_PROGRESS_HEIGHT, progress_cnt);
    UBOOT_DEBUG("cmd=%s\n",buffer);
    run_command(buffer, 0);
#else
    memset(buffer, 0 , CMD_BUF);
    snprintf(buffer, CMD_BUF, "draw_rect %d %d %d %d 0x800000ff", GRAPHIC_X, GRAPHIC_Y + LINE_HEIGHT * 5, GRAPHIC_WIDTH, LINE_HEIGHT);
    UBOOT_DEBUG("cmd=%s\n",buffer);
    run_command(buffer, 0);
    memset(buffer, 0 , CMD_BUF);
    snprintf(buffer, CMD_BUF, "draw_progress %d %d 0x3fffffff %d", GRAPHIC_X + RECT_LEFT_INTERVAL,GRAPHIC_Y + LINE_HEIGHT * 5, progress_cnt);
    UBOOT_DEBUG("cmd=%s\n",buffer);
    run_command(buffer, 0);
#endif
    memset(buffer, 0 , CMD_BUF);
    snprintf(buffer, CMD_BUF, "osd_flush");
    UBOOT_DEBUG("cmd=%s\n",buffer);
    run_command(buffer, 0);
    UBOOT_TRACE("OK\n");

    return 0;
}

static int show_Error(int var)
{
    if (-1 == display_osd)
        return -1;
    char buffer[CMD_BUF]="\0";
    UBOOT_TRACE("IN\n");
#if (ENABLE_DISPLAY_PICTURE_UPDATE)
    stop_Spinner_Timer_ISR();
    memset(buffer, 0 , CMD_BUF);
    snprintf(buffer, CMD_BUF, "draw_jpg -fs %d %d %d %d %s", EN_PIC_ERROR_X, EN_PIC_ERROR_Y, EN_PIC_ERROR_WIDTH, EN_PIC_ERROR_HEIGHT, EN_PIC_ERROR_PATH);
    UBOOT_DEBUG("cmd=%s\n",buffer);
    run_command(buffer, 0);
#else
    memset(buffer, 0 , CMD_BUF);
    snprintf(buffer, CMD_BUF, "draw_rect %d %d %d %d 0x800000ff", GRAPHIC_X, GRAPHIC_Y, GRAPHIC_WIDTH, GRAPHIC_HEIGHT);
    UBOOT_DEBUG("cmd=%s\n",buffer);
    run_command(buffer, 0);
    memset(buffer, 0 , CMD_BUF);
    snprintf(buffer, CMD_BUF, "draw_string %d %d 0x3fffffff 1 %x", GRAPHIC_X, GRAPHIC_Y + LINE_HEIGHT * 3,en_upgrade_error);
    UBOOT_DEBUG("cmd=%s\n",buffer);
    run_command(buffer, 0);
#endif
    memset(buffer, 0 , CMD_BUF);
    snprintf(buffer, CMD_BUF, "osd_flush");
    UBOOT_DEBUG("cmd=%s\n",buffer);
    run_command(buffer, 0);
    if (1 == CONFIG_GPIO_LED_ENABLE)
    {
        run_command("led stop",0);
    }
    UBOOT_TRACE("OK\n");
    return 0;
}

static int show_Finish(int var)
{
    if (-1 == display_osd)
        return -1;

    char buffer[CMD_BUF]="\0";
    UBOOT_TRACE("IN\n");
#if (ENABLE_DISPLAY_PICTURE_UPDATE)
    memset(buffer, 0 , CMD_BUF);
    snprintf(buffer, CMD_BUF, "draw_jpg -fs %d %d %d %d updating_%d.jpg", EN_PIC_PROGRESS_X, EN_PIC_PROGRESS_Y, EN_PIC_PROGRESS_WIDTH, EN_PIC_PROGRESS_HEIGHT, progress_cnt);
    UBOOT_DEBUG("cmd=%s\n",buffer);
    run_command(buffer, 0);
#else
    memset(buffer, 0 , CMD_BUF);
    snprintf(buffer, CMD_BUF, "draw_rect %d %d %d %d 0x800000ff", GRAPHIC_X, GRAPHIC_Y, GRAPHIC_WIDTH, GRAPHIC_HEIGHT);
    UBOOT_DEBUG("cmd=%s\n",buffer);
    run_command(buffer, 0);
    memset(buffer, 0 , CMD_BUF);
    snprintf(buffer, CMD_BUF, "draw_string %d %d 0x3fffffff 1 %x", GRAPHIC_X, GRAPHIC_Y + LINE_HEIGHT * 2,en_upgrade_complete);
    UBOOT_DEBUG("cmd=%s\n",buffer);
    run_command(buffer, 0);
    memset(buffer, 0 , CMD_BUF);
    snprintf(buffer, CMD_BUF, "draw_progress %d %d 0x3fffffff %d", GRAPHIC_X + RECT_LEFT_INTERVAL, GRAPHIC_Y + LINE_HEIGHT * 5, 100);
    UBOOT_DEBUG("cmd=%s\n",buffer);
    run_command(buffer, 0);
#endif
    memset(buffer, 0 , CMD_BUF);
    snprintf(buffer, CMD_BUF, "osd_flush");
    UBOOT_DEBUG("cmd=%s\n",buffer);
    run_command(buffer, 0);
    udelay(2000000);

    if (1 == CONFIG_GPIO_LED_ENABLE)
    {
        run_command("led stop",0);
    }
    else if (1 == CONFIG_PWM_LED_ENABLE)
    {
        run_command("led pwm_stop",0);
    }

    UBOOT_DEBUG("cmd=%s\n",buffer);
    UBOOT_TRACE("OK\n");
    return 0;
}


#if (ENABLE_UTEST == 1)
static int show_test_start(int var)
{
    if (-1 == display_osd)
    {
        return -1;
    }
    char buffer[CMD_BUF]="\0";
    int ret = 0;
    UBOOT_TRACE("IN\n");
    progress_cnt = 0;
    progress_start_load_value = 10;
    progress_finish_load_value = 200;
    snprintf(buffer, CMD_BUF, "osd_create %d %d", GWIN_WIDTH, GWIN_HEIGHT);
    UBOOT_DEBUG("cmd=%s\n",buffer);
    ret = run_command(buffer, 0);
    if (-1 == ret)
    {
        display_osd = -1;
        return -1;
    }
    memset(buffer, 0 , CMD_BUF);
    snprintf(buffer, CMD_BUF, "draw_rect %d %d %d %d 0x800000ff", GRAPHIC_X, GRAPHIC_Y, GRAPHIC_WIDTH, GRAPHIC_HEIGHT);
    UBOOT_DEBUG("cmd=%s\n",buffer);
    run_command(buffer, 0);
    memset(buffer, 0 , CMD_BUF);
    snprintf(buffer, CMD_BUF, "draw_string %d %d 0x3fffffff 1 USB TEST START...", GRAPHIC_X, GRAPHIC_Y + LINE_HEIGHT * 2);
    UBOOT_DEBUG("cmd=%s\n",buffer);
    run_command(buffer, 0);

    memset(buffer, 0 , CMD_BUF);
    snprintf(buffer, CMD_BUF, "draw_string %d %d 0x3fffffff 1 Remove usb device after test", GRAPHIC_X,  GRAPHIC_Y + LINE_HEIGHT * 4);
    UBOOT_DEBUG("cmd=%s\n",buffer);
    run_command(buffer, 0);

    memset(buffer, 0 , CMD_BUF);
    snprintf(buffer, CMD_BUF, "osd_flush");
    UBOOT_DEBUG("cmd=%s\n",buffer);
    run_command(buffer, 0);
    UBOOT_TRACE("OK\n");
    return 0;
}

static int show_test(int var, char * string1, char * string2, int current_cnt, int total_cnt)
{
    if (-1 == display_osd){
        return -1;
    }

    char buffer[CMD_BUF]="\0";
    UBOOT_TRACE("IN\n");
    progress_cnt = (((progress_max_value - progress_finish_load_value)*1000)/total_cnt);   //(1000-200)/cmd_cnt
    progress_cnt = (progress_cnt *current_cnt+progress_finish_load_value*1000)/(10*1000);
    printf("--%d %%\n", progress_cnt);

    memset(buffer, 0 , CMD_BUF);
    snprintf(buffer, CMD_BUF, "draw_rect %d %d %d %d 0x800000ff", GRAPHIC_X, GRAPHIC_Y, GRAPHIC_WIDTH, GRAPHIC_HEIGHT);
    UBOOT_DEBUG("cmd=%s\n",buffer);
    run_command(buffer, 0);

    if(string1 != NULL)
    {
        memset(buffer, 0 , CMD_BUF);
        snprintf(buffer, CMD_BUF, "draw_string %d %d 0x3fffffff 1 %s", GRAPHIC_X, GRAPHIC_Y + LINE_HEIGHT * 2, string1);
        UBOOT_DEBUG("cmd=%s\n",buffer);
        run_command(buffer, 0);
    }

    if(string2 != NULL)
    {
        memset(buffer, 0 , CMD_BUF);
        snprintf(buffer, CMD_BUF, "draw_string %d %d 0x3fffffff 1 %s", GRAPHIC_X,  GRAPHIC_Y + LINE_HEIGHT * 3, string2);
        UBOOT_DEBUG("cmd=%s\n",buffer);
        run_command(buffer, 0);
    }

    memset(buffer, 0 , CMD_BUF);
    snprintf(buffer, CMD_BUF, "draw_string %d %d 0x3fffffff 1 Remove usb device after test", GRAPHIC_X,  GRAPHIC_Y + LINE_HEIGHT * 4);
    UBOOT_DEBUG("cmd=%s\n",buffer);
    run_command(buffer, 0);

    memset(buffer, 0 , CMD_BUF);
    snprintf(buffer, CMD_BUF, "draw_progress %d %d 0x3fffffff %d", GRAPHIC_X + RECT_LEFT_INTERVAL, GRAPHIC_Y + LINE_HEIGHT * 5, progress_cnt);
    UBOOT_DEBUG("cmd=%s\n",buffer);
    run_command(buffer, 0);
    memset(buffer, 0 , CMD_BUF);
    snprintf(buffer, CMD_BUF, "osd_flush");
    UBOOT_DEBUG("cmd=%s\n",buffer);
    run_command(buffer, 0);
    UBOOT_TRACE("OK\n");

    return 0;
}
#endif


#endif


#if (ENABLE_MODULE_USB == 1)
static int check_bin_flag(void)
{
    int stage = 0;
    UBOOT_TRACE("IN\n");

    if(check_file_partition(AP_BIN_PATH,NULL,NULL)==0)//check Supernova file
    {
      stage |= UPGRADE_STATE_AP_BIN;
      UBOOT_DEBUG("has app upgrade bin\n");
    }

    if(check_file_partition(SBOOT_BIN_PATH,NULL,NULL)==0)//check sboot file
    {
      stage |= UPGRADE_STATE_SBOOT_BIN;
      UBOOT_DEBUG("has sboot upgrade bin\n");
    }

    if(check_file_partition(MBOOT_BIN_PATH,NULL,NULL)==0)//check mboot file
    {
      stage |= UPGRADE_STATE_MBOOT_BIN;
      UBOOT_DEBUG("has mboot upgrade bin\n");
    }
#if defined (CONFIG_UPGRADE_URSA)
    if(check_file_partition(URSA_UPGRADE_BIN_PATH,NULL,NULL)==0)//check mboot file
    {
        stage |= UPGRADE_STATE_URSA_BIN;
        UBOOT_DEBUG("has mboot upgrade bin\n");
    }
#else
    if(check_file_partition(URSA_BIN_PATH,NULL,NULL)==0)//check ursa file
    {
      stage |= UPGRADE_STATE_URSA_BIN;
      UBOOT_DEBUG("has ursa upgrade bin\n");
    }
#endif

#if defined (CONFIG_URSA_6M60)
    if(check_file_partition(TERRA2_UPGRADE_BIN_PATH, NULL, NULL)==0)//check terra2 file
    {
        stage |= UPGRADE_STATE_TERRA2_BIN;
        UBOOT_DEBUG("has terra2 upgrade bin\n");
    }
#endif

    UBOOT_TRACE("OK\n");

    return stage;
}

/* this is sec flashing command */
/* it uses usb upgrade to flash two packages from usb disk */
int do_uupdate(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    /* sec flashing has to be enabled */
    /* quietly return in these conditions */
#if defined(UFBL_FEATURE_SECURE_FLASHING)
    if ( (!amzn_device_is_sec_flashing_enabled()) ||
#else
    if (
#endif
         (2 > argc) || (3 < argc) ||
         (-1 == appInitUsbDisk()) ||
         (check_file_partition(argv[1],NULL,NULL)) ||
         ((argv[2]) && (check_file_partition(argv[2],NULL,NULL))) )
        return 0;

    #if (ENABLE_MODULE_DISPLAY_OSD==1)
    UnRegisterShowCb();
    RegisterCBShowError((show_Error_cb)show_Error);
    RegisterCBShowFinish((show_Finish_cb)show_Finish);
    RegisterCBShowLoadData((show_LoadData_cb)show_LoadData);
    RegisterCBShowStartUpgrading((show_StartUpgrading_cb)show_StartUpgrading);
    RegisterCBShowUpgrading((show_Upgrading_cb)show_Upgrading);
    #endif

    char cmd[128];
    memset(cmd, 0, 128);
    snprintf(cmd, sizeof(cmd)-1, "ustar %s", argv[1]);
    UBOOT_DEBUG("cmd=%s\n",cmd);
    if(-1 == run_command(cmd, 0))
    {
       UBOOT_ERROR("Upgrade %s error!\n", argv[1]);
       /* halt */
       while (1);
    }

    if (argv[2]) { // second package presents
       MsOS_DelayTask(2000);
       memset(cmd, 0, 128);
       snprintf(cmd, sizeof(cmd)-1, "ustar %s", argv[2]);
       UBOOT_DEBUG("cmd=%s\n",cmd);
       if(-1 == run_command(cmd, 0))
       {
          UBOOT_ERROR("Upgrade %s error!\n", argv[2]);
          /* halt */
          while (1);
       }
    }

    setenv ("sync_mmap", "1");
    setenv("upgrade_status", NULL);
    saveenv();
    run_command("reset",0);
    return 0;
}

int do_custar(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{

    char cmd[128];
    char upgrade_state_flag[] = "0x00";
    char* env_state = NULL;
    int state=0;
    char *cUpgradeStatus = NULL;
    UBOOT_TRACE("IN\n");

    //clean the env  upgrade_status
    setenv("upgrade_status", NULL);
    saveenv();
    //usb init
    if (-1 == appInitUsbDisk())
    {
        printf("FAIL : can not init usb!! \n");
        run_command("setenv force_upgrade", 0);
        setenv("upgrade_status", "ERROR");
        saveenv();
        return -1;
    }

    #if (ENABLE_MODULE_DISPLAY_OSD==1)
    UnRegisterShowCb();
    RegisterCBShowError((show_Error_cb)show_Error);
    RegisterCBShowFinish((show_Finish_cb)show_Finish);
    RegisterCBShowLoadData((show_LoadData_cb)show_LoadData);
    RegisterCBShowStartUpgrading((show_StartUpgrading_cb)show_StartUpgrading);
    RegisterCBShowUpgrading((show_Upgrading_cb)show_Upgrading);
    #endif

    //check upgrade file content
    env_state = getenv(ENV_FORCEUPGRADE);
    if(env_state != NULL )
    {
         state = (int)simple_strtoul(env_state, NULL, 16);
    }
    else
    {
        state=check_bin_flag();
        if(state == 0 )
        {
            UBOOT_INFO("warn!! no upgrade file in usb. \n");
            UBOOT_TRACE("OK\n");
            run_command("setenv force_upgrade", 0);
            setenv("upgrade_status", "ERROR");
            saveenv();
            return -1;
        }

        if(state & 0x0F)
        {
            snprintf(upgrade_state_flag, sizeof(upgrade_state_flag)-1, "0x%02x",state);
            setenv(ENV_FORCEUPGRADE,upgrade_state_flag);
            saveenv();
        }
    }

    UBOOT_INFO("Current state ->[0x%x]\n",state);
#if defined (CONFIG_UPGRADE_URSA)
    if(state&UPGRADE_STATE_URSA_BIN) // upgrade Ursa
    {
        UBOOT_DEBUG("cmd= ursa_upgrade \n");
        if(-1 == run_command("ursa_upgrade", 0))
        {
            UBOOT_ERROR("Upgrade Ursa error!\n");
            setenv(ENV_FORCEUPGRADE,NULL);
            setenv("upgrade_status", "ERROR");
            saveenv();
            return -1;
        }
        state = state&(~UPGRADE_STATE_URSA_BIN);
        UBOOT_INFO("Ursa upgrade OK!~\n");
    }
#else
    if(state&UPGRADE_STATE_URSA_BIN) // upgrade Ursa
    {
        memset(cmd, 0, 128);
        snprintf(cmd, sizeof(cmd)-1, "ustar %s",URSA_BIN_PATH);
        UBOOT_DEBUG("cmd=%s\n",cmd);
        if(-1 == run_command(cmd, 0))
        {
            UBOOT_ERROR("Upgrade Ursa error!\n");
            setenv(ENV_FORCEUPGRADE,NULL);
            setenv("upgrade_status", "ERROR");
            saveenv();
            return -1;
        }
        state = state&(~UPGRADE_STATE_URSA_BIN);
        UBOOT_INFO("Ursa upgrade OK!~\n");
    }
#endif
    else if(state&UPGRADE_STATE_SBOOT_BIN)//upgrade Sboot
    {
        memset(cmd, 0, 128);
        snprintf(cmd, sizeof(cmd)-1, "ustar %s",SBOOT_BIN_PATH);
        UBOOT_DEBUG("cmd=%s\n",cmd);
        if(-1 == run_command(cmd, 0))
        {
            UBOOT_ERROR("Upgrade Sboot error!\n");
            setenv(ENV_FORCEUPGRADE,NULL);
            setenv("upgrade_status", "ERROR");
            saveenv();
            //return -1;
        }
        state = state&(~UPGRADE_STATE_SBOOT_BIN);
        UBOOT_INFO("Sboot upgrade OK!~\n");
    }

    else if(state&UPGRADE_STATE_MBOOT_BIN)//upgrade Mboot
    {
        memset(cmd, 0, 128);
        snprintf(cmd, sizeof(cmd)-1, "ustar %s",MBOOT_BIN_PATH);
        UBOOT_DEBUG("cmd=%s\n",cmd);
        if(-1 == run_command(cmd, 0))
        {
            UBOOT_ERROR("Upgrade Mboot error!\n");
            setenv(ENV_FORCEUPGRADE,NULL);
            setenv("upgrade_status", "ERROR");
            saveenv();
            //return -1;
        }
        state = state&(~UPGRADE_STATE_MBOOT_BIN);
        UBOOT_INFO("Mboot upgrade OK!~\n");
    }
    else if(state&UPGRADE_STATE_AP_BIN)//upgrade Supernova
    {
        memset(cmd, 0, 128);
        snprintf(cmd, sizeof(cmd)-1, "ustar %s",AP_BIN_PATH);
        UBOOT_DEBUG("cmd=%s\n",cmd);
        if(-1 == run_command(cmd, 0))
        {
            UBOOT_ERROR("Upgrade Application error!\n");
            setenv(ENV_FORCEUPGRADE,NULL);
            setenv("upgrade_status", "ERROR");
            saveenv();
            //return -1;
        }
        state = state&(~UPGRADE_STATE_AP_BIN);
        UBOOT_INFO("AP upgrade OK!~\n");
    }

#if defined (CONFIG_URSA_6M60)
    if(state & UPGRADE_STATE_TERRA2_BIN) // upgrade Terra2
    {
        UBOOT_DEBUG("cmd= terra2_fast_upgrade \n");
        if(-1 == run_command("terra2_fast_upgrade", 0))
        {
            UBOOT_ERROR("Upgrade terra2 fail!\n");
            //setenv(ENV_FORCEUPGRADE,NULL);
            setenv("upgrade_status", "ERROR");
            saveenv();
        }
        else
        {
            UBOOT_INFO("terra2 upgrade OK!~\n");
        }
        state = state&(~UPGRADE_STATE_TERRA2_BIN);
    }
#endif

    cUpgradeStatus = getenv("upgrade_status");
    if( cUpgradeStatus != NULL )
    {
        if( strncmp(cUpgradeStatus, "ERROR", 5) == 0 )
        {
          MsOS_DelayTask(5000);
          run_command("reset", 0);
        }
    }

    if(state == 0)
    {
        UBOOT_INFO("All Upgrade File Had Finished !!\n");
        setenv(ENV_FORCEUPGRADE,NULL);
    }
    else
    {
        snprintf(upgrade_state_flag, sizeof(upgrade_state_flag)-1, "0x%02x",state);
        UBOOT_DEBUG("upgrade_state_flag=%s\n",upgrade_state_flag);
        setenv(ENV_FORCEUPGRADE,upgrade_state_flag);
    }
#ifdef CONFIG_BOOTCOUNT_LIMIT
    setenv ("bootcount", "0");
#endif
    setenv ("sync_mmap", "1");
    setenv("upgrade_status", NULL);
    saveenv();
    UBOOT_TRACE("OK\n");
    run_command("reset",0);
    return 0;
}

#if defined(CONFIG_MMC)
int do_usb_partial_upgrade_to_emmc(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
   char cmd[CMD_BUF] = "\0";
   char *env =NULL, *path;
   int partition = 0;
   int device = 0;
   MS_U8 u8Percent= 0;
   long unsigned int u32FileSize = 0, offset = 0, remain = 0, partial_size = 0;
   unsigned int u32PartSize, u32PartOffset;

   UBOOT_DEBUG("cyli argv[1]=%s,argv[2]=%s,argv[3]=%s,argv[4]=%s,argv[5]=%s\n",argv[1],argv[2],argv[3],argv[4],argv[5]);
   if (argc < 3) {
       UBOOT_ERROR ("usage: %s <filename> <partition_name>\n", argv[0]);
       return 1;
   }
#if 0
   if (-1 == appInitUsbDisk()) {
       UBOOT_ERROR("FAIL : can not init usb!! \n");
       return -1;
   }
#endif

   path = argv[1];
   if (!check_file_partition(path ,&device,&partition)) {
       if (get_mmc_part_size_and_offset(argv[2], &u32PartSize, &u32PartOffset)) {
           UBOOT_ERROR("get emmc part size and offset fail\n");
           return -1;
       }
       UBOOT_DEBUG("Get %s partition: size = 0x%x , offset = 0x%x\n", argv[2], u32PartSize, u32PartOffset);

       UBOOT_DEBUG("Geting usb file size of %s\n", path);
       snprintf(cmd, CMD_BUF, "fatfilesize usb %d:%d %s", device, partition, path);
       UBOOT_DEBUG("cmd => %s\n",cmd);
       if (run_command(cmd, 0)) {
           UBOOT_ERROR("get script file's size fail\n");
           return -1;
       } else {
           env = getenv("filesize");
           if (NULL == env) {
               UBOOT_ERROR("get env 'filesize' fail\n");
               return -1;
           }
           u32FileSize = simple_strtol(env, NULL, 16);
           printf("Size 0x%lx\n", u32FileSize);
           remain = u32FileSize;
       }

       if (u32FileSize > (u32PartSize << 9)) {
           UBOOT_ERROR("file size (0x%x) is bigger than partition size (0x%x)\n", u32FileSize, u32PartSize << 9);
           return -1;
       }

       //      snprintf(cmd, CMD_BUF, "fatload usb %d:%d %X %s %X", device, partition, UPGRADE_BUFFER_ADDR, path, u32FileSize);
       while (remain > 0) {
           if (remain > UPGRADE_BUFFER_SIZE)
               partial_size = UPGRADE_BUFFER_SIZE;
           else
               partial_size = remain;

           snprintf(cmd, CMD_BUF, "fatpartload usb %d:%d 0x%X %s 0x%X 0x%X", device, partition, UPGRADE_BUFFER_ADDR, path, offset, partial_size);
           UBOOT_DEBUG("cmd => %s\n",cmd);
           if (run_command(cmd, 0)) {
               UBOOT_ERROR("Load Upgrade File fail!\n");
               return -1;
           }

           flush_cache(UPGRADE_BUFFER_ADDR, partial_size);

           snprintf(cmd, CMD_BUF, "mmc write 0x%X 0x%X 0x%X", UPGRADE_BUFFER_ADDR, u32PartOffset + (offset >> 9), partial_size);
           UBOOT_DEBUG("cmd => %s\n",cmd);
           if (run_command(cmd, 0)) {
               UBOOT_ERROR("Write Upgrade File fail!\n");
               return -1;
           }

           remain -= partial_size;
           offset += partial_size;
       }

   } else {
       UBOOT_DEBUG("no %s in usb disk\n", path);
       return -1;
   }

   return 0;
}
#endif

#if defined(CONFIG_CMD_CUSTOMER_UDSTAR) && (CONFIG_CMD_CUSTOMER_UDSTAR == 1)
#define DEFAULT_DEBUG_SCRIPT_PATH             "/MstarDebug.script"
#define ARG_NUM_SCRIPT_FILE         1
#define IS_ARG_AVAILABLE_SCRIPT_FILE(x) ((x) > ARG_NUM_SCRIPT_FILE)
#define ARG_SCRIPT_FILE(x)              (x)[ARG_NUM_SCRIPT_FILE]
#define MAX_SCRIPT_SIZE 0x20000

int do_cudstar (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
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

    while ((next_line = get_script_next_line(&script_buf)) != NULL)
    {
        UBOOT_INFO ("\n>>1 %s \n", next_line);

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

#endif
#endif

#if (ENABLE_MODULE_OAD == 1)
int do_costar(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    char buffer[CMD_BUF] = "\0";
    int ret=0;

    UBOOT_TRACE("IN\n");
    #if (ENABLE_MODULE_DISPLAY_OSD==1)
        UnRegisterShowCb();
        RegisterCBShowError((show_Error_cb)show_Error);
        RegisterCBShowFinish((show_Finish_cb)show_Finish);
        RegisterCBShowLoadData((show_LoadData_cb)show_LoadData);
        RegisterCBShowStartUpgrading((show_StartUpgrading_cb)show_StartUpgrading);
        RegisterCBShowUpgrading((show_Upgrading_cb)show_Upgrading);
    #endif

    if(argc == 2)
    {
        if(strcmp(argv[1], "updatelogic")==0)
            snprintf(buffer,CMD_BUF,"ostar %s",UL_BIN_PATH);
        else
            return -1;
    }
    else
    {
        snprintf(buffer,CMD_BUF,"ostar %s",OAD_BIN_PATH);
    }

    UBOOT_DEBUG("cmd=%s\n",buffer);
    ret=run_command(buffer,0);
    if(ret==-1)
    {
        UBOOT_ERROR("do ostar fail\n");
        #if (ENABLE_MODULE_DISPLAY_OSD==1)
        show_Error(0);
        #endif
        MsOS_DelayTask(3000);
        setenv("upgrade_mode","null");
        setenv("MstarUpgrade_complete","1");
        saveenv();
        run_command("reset", 0);
        return -1;
    }

    UBOOT_TRACE("OK\n");
    return 0;
}
#endif

#if (ENABLE_MODULE_NETUPDATE == 1)
int do_cnstar(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    char buffer[CMD_BUF] = "\0";
    int ret=0;

    UBOOT_TRACE("IN\n");

    #if (ENABLE_MODULE_DISPLAY_OSD==1)
        UnRegisterShowCb();
        RegisterCBShowError((show_Error_cb)show_Error);
        RegisterCBShowFinish((show_Finish_cb)show_Finish);
        RegisterCBShowLoadData((show_LoadData_cb)show_LoadData);
        RegisterCBShowStartUpgrading((show_StartUpgrading_cb)show_StartUpgrading);
        RegisterCBShowUpgrading((show_Upgrading_cb)show_Upgrading);
    #endif
    snprintf(buffer,CMD_BUF,"nstar %s",NET_BIN_PATH);
    UBOOT_DEBUG("cmd=%s\n",buffer);
    ret=run_command(buffer,0);
    if(ret==-1)
    {
        UBOOT_ERROR("do nstar fail\n");
        return -1;
    }

    UBOOT_TRACE("OK\n");
    return 0;
}


#endif

#if (ENABLE_MODULE_USB == 1)
#if (ENABLE_MINIUBOOT==0)
int do_usb_bin_check(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    char cmd[128];
    int ret = -1;
    UBOOT_TRACE("IN\n");
    //usb init
    if (-1 == appInitUsbDisk())
    {
       printf("FAIL : can not init usb!! \n");
       return -1;
    }
    //we try usb upgrade first
    ret = check_file_partition(AP_BIN_PATH,NULL,NULL);
    if(ret == 0)
    {
        UBOOT_DEBUG("has app upgrade bin\n");
        memset(cmd, 0, 128);
        snprintf(cmd, sizeof(cmd)-1, "ustar %s",AP_BIN_PATH);
        UBOOT_DEBUG("cmd=%s\n",cmd);
        if(-1 == run_command(cmd, 0))
        {
            UBOOT_ERROR("Upgrade Application error!\n");
            return -1;
        }
        else
        {
            UBOOT_INFO("AP upgrade OK!~\n");
            UBOOT_TRACE("OK\n");
            run_command("reset",0);
            return 0;
        }
    }
    else
    {
        UBOOT_DEBUG("no %s in usb disk\n",AP_BIN_PATH);
    }
    return -1;
}


static int ota_zip_check_usb(void)
{
    int ret;
    char cmd[128];

    UBOOT_TRACE("IN\n");
    //usb init
    if (-1 == appInitUsbDisk())
    {
       printf("FAIL : can not init usb!! \n");
       return -1;
    }
    char label[128] = "\0";
    char raw_uuid[128] = "\0";
    char uuid[128] = "\0";
    struct bootloader_message p_msg;

    ret = check_file_partition(OTA_ZIP_PATH,NULL,NULL);
    if(ret == 0)//OTA upgrade
    {
        getFatVolumeLabel(label,raw_uuid);
        snprintf(uuid, sizeof(uuid)-1, "%02x%02x-%02x%02x", raw_uuid[3], raw_uuid[2], raw_uuid[1], raw_uuid[0]);
        UBOOT_DEBUG("label:%s\nuuid:%s\n",label,uuid);
        memset(cmd,0,128);
        memset(&p_msg,0,sizeof(struct bootloader_message));
        strcpy(p_msg.command,BOOT_MODE_RECOVERY_STR);
        strcpy(p_msg.recovery,"recovery\n");
        strcat(p_msg.recovery,"--update_package=/mnt/usb/sda1/update_signed.zip\n");
        strcat(p_msg.recovery,"--label=");
        strcat(p_msg.recovery,label);
        strcat(p_msg.recovery,"\n");
        strcat(p_msg.recovery,"--uuid=");
        strcat(p_msg.recovery,uuid);
        strcat(p_msg.recovery,"\n");
        UBOOT_DEBUG("command:%s\n",p_msg.recovery);
        #if (ENABLE_MODULE_MMC == 1)
        snprintf(cmd, sizeof(cmd)-1, "mmc write.p 0x%X misc 0x%x", &p_msg,sizeof(struct bootloader_message));
        #else
            #if !CONFIG_MSTAR_FTL
        snprintf(cmd, sizeof(cmd)-1, "nand write.e 0x%X misc 0x%x", &p_msg,sizeof(struct bootloader_message));
            #else
        snprintf(cmd, sizeof(cmd)-1, "ftl write.p 0x%X misc 0x%x", &p_msg,sizeof(struct bootloader_message));
            #endif
        #endif
        if(-1 == run_command(cmd,0))
        {
            UBOOT_ERROR("cmd:%s error\n",cmd);
            return -1;
        }
        UBOOT_INFO("OTA upgrade OK!~\n");
        UBOOT_TRACE("OK\n");
        return 0;
    }
    else
    {
        UBOOT_DEBUG("no %s in usb disk\n",OTA_ZIP_PATH);
        UBOOT_TRACE("OK\n");
        return -1;
    }
}
#endif
 #if defined(CONFIG_MMC)
static int ota_zip_check_emmc(char *abspath)
{
    int ret;
    U32 fsize;
    char cmd[256];
    char subdir[512];
    struct bootloader_message p_msg;

    UBOOT_TRACE("IN\n");
    ret = vfs_mount_by_abspath(abspath, subdir, sizeof(subdir));
    if (ret != 0)
    {
        UBOOT_ERROR("vfs_mount_by_abspath failed\n");
        return -1;
    }
    fsize = vfs_getsize(subdir);
    if (fsize <= 0)
    {
        UBOOT_DEBUG("file(%s) do not exist !\n", abspath);
        return -1;
    }

    memset(&p_msg,0,sizeof(struct bootloader_message));
    strcpy(p_msg.command,BOOT_MODE_RECOVERY_STR);
    strcpy(p_msg.recovery,"recovery\n");

    memset(cmd, 0, 128);
    snprintf(cmd, sizeof(cmd), "--update_package=%s\n", abspath);
    strcat(p_msg.recovery, cmd);
    UBOOT_DEBUG("command:%s\n", p_msg.recovery);
    memset(cmd, 0, 128);
    #if (ENABLE_MODULE_MMC == 1)
    snprintf(cmd, sizeof(cmd)-1, "mmc write.p 0x%X misc 0x%x", &p_msg,sizeof(struct bootloader_message));
    #else
        #if !CONFIG_MSTAR_FTL
    snprintf(cmd, sizeof(cmd)-1, "nand write.e 0x%X misc 0x%x", &p_msg,sizeof(struct bootloader_message));
        #else
    snprintf(cmd, sizeof(cmd)-1, "ftl write.p 0x%X misc 0x%x", &p_msg,sizeof(struct bootloader_message));
        #endif
    #endif
    if(-1 == run_command(cmd,0))
    {
        UBOOT_ERROR("cmd:%s error\n",cmd);
        return -1;
    }

    UBOOT_TRACE("OK\n");
    return 0;
}
#endif

int do_ota_zip_check(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    int ret = -1;
    UBOOT_TRACE("IN\n");
 #if defined(CONFIG_MMC)
    if ((argc >= 2) && (strcmp(argv[1], "emmc") == 0))
    {
        char *abspath = argv[2];
        UBOOT_DEBUG("check otapackage(%s) in emmc\n", abspath);
        ret = ota_zip_check_emmc(abspath);
    }
    else
#endif
    {
        UBOOT_DEBUG("check otapackage(update_signed.zip) in usb_disk\n");
        ret = ota_zip_check_usb();
    }
    UBOOT_TRACE("OK\n");
    return ret;
}


#if (ENABLE_UTEST == 1)

static char* load_script_by_path(char *script_path, char *script_buf, unsigned int script_max_size)
{
    char cmd[CONFIG_SYS_CBSIZE];
    unsigned int iFilesize = 0;

    memset(script_buf, 0, script_max_size);
    sprintf(cmd, "fatload usb 0 %X %s %X", (unsigned int)script_buf, script_path, script_max_size);
    UBOOT_INFO("%s\n", cmd);
    if (run_command (cmd, 0) != 0)
    {
        UBOOT_ERROR("cmd %s failed\n", cmd);
        return NULL;
    }

    iFilesize = (unsigned int)simple_strtoul(getenv("filesize"), NULL, 16);
    UBOOT_DEBUG("iFilesize=0x%x\n", iFilesize);
    if (iFilesize >= (script_max_size-2))
    {
        UBOOT_ERROR("script_max_size(0x%x) is too small !\n", script_max_size);
        UBOOT_INFO("Filesize = 0x%x\n", iFilesize);
        return NULL;
    }

    script_buf[iFilesize]='\n';
    script_buf[iFilesize+1]='%';
    return script_buf;
}


int do_utest(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    #define USB_TEST_PATH "/auto_test.txt"
    #define DEFAULT_SCRIPT_SIZE 0x4000

    char *next_line = NULL;
    char *penv = NULL;
    char *script_buf = NULL;
    char *tmp_script_buf = NULL;
    char *script_path = NULL;
    char * test_cmd = NULL;
    unsigned int total_cnt_num=0;
    unsigned int current_cnt_num=0;
    UBOOT_TRACE("IN\n");
    if (((penv = getenv("enable_utest")) == NULL) || (strcmp(penv,"1") != 0))
    {
        UBOOT_INFO("env(enable_utest) != 1, skip utest ...\n");
        UBOOT_TRACE("OK\n");
        return 0;
    }
    tmp_script_buf = (char *)malloc(DEFAULT_SCRIPT_SIZE);
    if (tmp_script_buf == NULL)
    {
        UBOOT_ERROR("malloc 0x%x byte failed\n", DEFAULT_SCRIPT_SIZE);
        return -1;
    }

    printf("[AT][MB][do_bootm][%lu]\n", MsSystemGetBootTime());
    //usb init
    if (-1 == appInitUsbDisk_Single(CONFIG_UTEST_SCAN_USB_PORT))
    {
        printf("FAIL : can not init usb %d!! \n", CONFIG_UTEST_SCAN_USB_PORT);
        printf("[AT][MB][do_bootm][%lu]\n", MsSystemGetBootTime());
        free(tmp_script_buf);
        return -1;
    }

    show_test_start(0);
    if(check_file_partition(USB_TEST_PATH,NULL,NULL) != 0)
    {
        UBOOT_ERROR("Can not find %s in Udisk\n", USB_TEST_PATH);
        free(tmp_script_buf);
        return -1;
    }

    script_buf = load_script_by_path(USB_TEST_PATH, tmp_script_buf, DEFAULT_SCRIPT_SIZE);
    if(script_buf == NULL)
    {
        UBOOT_ERROR("Load %s failed\n", USB_TEST_PATH);
        free(tmp_script_buf);
        return -1;
    }

    next_line = get_script_next_line(&script_buf);
    test_cmd = (char *)malloc(strlen(next_line));
    if (test_cmd == NULL)
    {
        UBOOT_ERROR("malloc 0x%x bytes failed !\n", strlen(next_line));
        goto utest_error;
    }

    strcpy(test_cmd, next_line);
    if ((script_path = strstr(next_line, "utest ")) != NULL)
    {
        UBOOT_INFO("\n>> %s \n", next_line);
        script_path = malloc(strlen(next_line) - strlen("utest "));
        if (script_path == NULL)
        {
            UBOOT_ERROR("malloc 0x%x bytes failed !\n", strlen(next_line));
            goto utest_error;
        }

        strcpy(script_path, next_line + strlen("utest "));
        script_buf = load_script_by_path(script_path, tmp_script_buf, DEFAULT_SCRIPT_SIZE);
        if(script_buf == NULL)
        {
            goto utest_error;
        }
        total_cnt_num = check_script_cmd_count(script_buf);
        UBOOT_INFO("total_cnt_num = %d \n", total_cnt_num);

        script_buf = load_script_by_path(script_path, tmp_script_buf,  DEFAULT_SCRIPT_SIZE);
        if(script_buf == NULL)
        {
            goto utest_error;
        }
        while ((next_line = get_script_next_line(&script_buf)) != NULL)
        {
            UBOOT_INFO("\n>> %s \n", next_line);
            if(-1 == run_command(next_line, 0))
            {
                const char * write_cmd = "mmc write.p";
                UBOOT_ERROR("command error!\n");
                printf("write_cmd = %s\n", write_cmd);
                printf("next_line = %s\n", next_line);
                if(strncmp(next_line, write_cmd, 11) == 0)
                {
                    show_test(0, "TEST ABORT", get_utest_err_msg(), 1, 1);
                }
                else
                {
                    show_test(0, "TEST ABORT", "Unknown command, check script!", 1, 1);
                }
                goto utest_error;
            }
            else
            {
                current_cnt_num ++;
                show_test(0, "TESTING...", test_cmd, current_cnt_num, total_cnt_num);
            }
        }
        show_test(0, "TEST FINISH", test_cmd, 1, 1);
    } else{
        UBOOT_ERROR("can not find command 'utest' in %s\n", USB_TEST_PATH);
        goto utest_error;
    }
    UBOOT_TRACE("OK\n");
    free(tmp_script_buf);
    free(script_path);
    free(test_cmd);
    test_cmd = NULL;
    script_path = NULL;
    setenv ("sync_mmap", "1");
    jump_to_console();
    return 0;
utest_error:
    free(tmp_script_buf);
    free(script_path);
    free(test_cmd);
    test_cmd = NULL;
    script_path = NULL;
    jump_to_console();
    return -1;
}
#endif


#endif

int do_wipe_datacache_check(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    char cmd[128];
    UBOOT_TRACE("IN\n");

    struct bootloader_message p_msg;
    memset(cmd,0,128);
    memset(&p_msg,0,sizeof(struct bootloader_message));
    strcpy(p_msg.command,BOOT_MODE_RECOVERY_STR);
    strcpy(p_msg.recovery,"recovery\n");
    if(0 == strcmp(argv[1],"data"))
    {
        strcat(p_msg.recovery,"--wipe_data\n");
    }
    if(0 == strcmp(argv[1],"cache"))
    {
        strcat(p_msg.recovery,"--wipe_cache\n");
    }
    UBOOT_DEBUG("command:%s\n",p_msg.recovery);
    #if (ENABLE_MODULE_MMC == 1)
        snprintf(cmd, sizeof(cmd)-1, "mmc write.p 0x%X misc 0x%x", &p_msg,sizeof(struct bootloader_message));
    #else
        #if !CONFIG_MSTAR_FTL
        snprintf(cmd, sizeof(cmd)-1, "nand write.e 0x%X misc 0x%x", &p_msg,sizeof(struct bootloader_message));
        #else
		snprintf(cmd, sizeof(cmd)-1, "ftl write.p 0x%X misc 0x%x", &p_msg,sizeof(struct bootloader_message));
        #endif
    #endif
    if(-1 == run_command(cmd,0))
    {
        UBOOT_ERROR("cmd:%s error\n",cmd);
        return -1;
    }
    UBOOT_INFO(" OK!~\n");
    UBOOT_TRACE("OK\n");
    return 0;
 }

#if (ENABLE_MODULE_SYSTEM_RESTORE == 1)
int do_systemrestore(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    char cmd[128];
    UBOOT_TRACE("IN\n");

    struct bootloader_message p_msg;
    memset(cmd,0,128);
    memset(&p_msg,0,sizeof(struct bootloader_message));
    strcpy(p_msg.command,BOOT_MODE_RECOVERY_STR);
    strcpy(p_msg.recovery,"recovery\n");
    strcat(p_msg.recovery,"--update_package=/systembackup/update_signed.zip\n");
    UBOOT_DEBUG("command:%s\n",p_msg.recovery);
    #if (ENABLE_MODULE_MMC == 1)
    snprintf(cmd, sizeof(cmd)-1, "mmc write.p 0x%X misc 0x%x", &p_msg,sizeof(struct bootloader_message));
    #else
        #if !CONFIG_MSTAR_FTL
    snprintf(cmd, sizeof(cmd)-1, "nand write.e 0x%X misc 0x%x", &p_msg,sizeof(struct bootloader_message));
        #else
    snprintf(cmd, sizeof(cmd)-1, "ftl write.p 0x%X misc 0x%x", &p_msg,sizeof(struct bootloader_message));
        #endif
    #endif
    if(-1 == run_command(cmd,0))
    {
       UBOOT_ERROR("cmd:%s error\n",cmd);
       return -1;
    }
    UBOOT_INFO(" OK!~\n");
    UBOOT_TRACE("OK\n");
    return 0;
}
#endif
//==============ISP functions for Upgrade===================
#if defined (CONFIG_UPGRADE_URSA)
//------------------------------------------------------------------------------
SFlashType SupportSFlash[Flash_NUMS]=
{
// 1-4:size ,5:manufacturer_id,6:device_id,7:WREN,8:WRDI,9:RDSR,10:WRSR,11:READ,12:FAST_READ,
// 13:PG_PROG,14:SECTOR_ERASE,15:BLOCK_ERASE,16:CHIP_ERASE,17:RDID,18:LOCK,19-20: ChipEraseMaxTime

//                    1-4    ,5   ,6   ,7   ,8   ,9   ,10  ,11  ,12  ,13  ,14  ,15  ,16  ,17  ,18  ,19-20
//PMC
{/*"PMC25LV512A", */  0x20000,0x9D,0x7B,0x06,0x04,0x05,0x01,0x03,NULL,0x02,0xD7,0xD8,0xC7,0xAB,0x0C,40  },
{/*" PMC25LV010", */  0x20000,0x9D,0x7C,0x06,0x04,0x05,0x01,0x03,NULL,0x02,0xD7,0xD8,0xC7,0xAB,0x0C,40  },
{/*" PMC25LV020", */  0x40000,0x9D,0x7D,0x06,0x04,0x05,0x01,0x03,NULL,0x02,0xD7,0xD8,0xC7,0xAB,0x0C,40  },
{/*" PMC25LV040", */  0x80000,0x9D,0x7E,0x06,0x04,0x05,0x01,0x03,NULL,0x02,0xD7,0xD8,0xC7,0xAB,0x1C,40  },
//EON
{/*"    EN25P10", */  0x20000,0x1C,0x10,0x06,0x04,0x05,0x01,0x03,NULL,0x02,0xD8,0xD8,0xC7,0xAB,0x0C,80  },
{/*"    EN25P20", */  0x40000,0x1C,0x11,0x06,0x04,0x05,0x01,0x03,NULL,0x02,0xD8,0xD8,0xC7,0xAB,0x0C,80  },
{/*"    EN25P40", */  0x80000,0x1C,0x12,0x06,0x04,0x05,0x01,0x03,NULL,0x02,0xD8,0xD8,0xC7,0xAB,0x1C,80  },
{/*"    EN25P80", */  0x100000,0x1C,0x13,0x06,0x04,0x05,0x01,0x03,NULL,0x02,0xD8,0xD8,0xC7,0xAB,0x1C,80  },
{/*"    EN25F40", */  0x80000,0x1C,0x12,0x06,0x04,0x05,0x01,0x03,NULL,0x02,0xD8,0xD8,0xC7,0xAB,0x1C,80  },
{/*"    EN25F80", */  0x100000,0x1C,0x13,0x06,0x04,0x05,0x01,0x03,NULL,0x02,0xD8,0xD8,0xC7,0xAB,0x1C,220  },
{/*"    EN25F16", */  0x200000,0x1C,0x14,0x06,0x04,0x05,0x01,0x03,NULL,0x02,0xD8,0xD8,0xC7,0xAB,0x1C,440  },
{/*"    EN25P16", */  0x80000,0x1C,0x14,0x06,0x04,0x05,0x01,0x03,NULL,0x02,0xD8,0xD8,0xC7,0xAB,0x1C,220 },
{/*"    EN25B20", */  0x40000,0x1C,0x41,0x06,0x04,0x05,0x01,0x03,NULL,0x02,0xD8,0xD8,0xC7,0xAB,0x0C,80  },
{/*"    EN25B40", */  0x80000,0x1C,0x42,0x06,0x04,0x05,0x01,0x03,NULL,0x02,0xD8,0xD8,0xC7,0xAB,0x1C,80  },
{/*"   EN25B40T", */  0x80000,0x1C,0x42,0x06,0x04,0x05,0x01,0x03,NULL,0x02,0xD8,0xD8,0xC7,0xAB,0x1C,80  },
{/*"    EN25B16", */  0x80000,0x1C,0x34,0x06,0x04,0x05,0x01,0x03,NULL,0x02,0xD8,0xD8,0xC7,0xAB,0x1C,220 },
{/*"   EN25B16T", */  0x80000,0x1C,0x34,0x06,0x04,0x05,0x01,0x03,NULL,0x02,0xD8,0xD8,0xC7,0xAB,0x1C,220 },
{/*"    EN25B32", */  0x80000,0x1C,0x35,0x06,0x04,0x05,0x01,0x03,NULL,0x02,0xD8,0xD8,0xC7,0xAB,0x1C,440 },
//                    1-4    ,5   ,6   ,7   ,8   ,9   ,10  ,11  ,12  ,13  ,14  ,15  ,16  ,17  ,18  ,19-20
//SPANSION
{/*"  S25FL004A", */  0x80000,0x01,0x12,0x06,0x04,0x05,0x01,0x03,NULL,0x02,0xD8,0xD8,0xC7,0xAB,0x1C,260 },
{/*"  S25FL016A", */0x1000000,0x01,0x14,0x06,0x04,0x05,0x01,0x03,NULL,0x02,0xD8,0xD8,0xC7,0xAB,0x1C,1000},
{/*"  S25FL040A", */  0x80000,0x01,0x25,0x06,0x04,0x05,0x01,0x03,NULL,0x02,0xD8,0xD8,0xC7,0xAB,0x1C,260 },
//Winbond & NX
{/*"    NX25P16", */0x1000000,0xEF,0x15,0x06,0x04,0x05,0x01,0x03,NULL,0x02,0xD8,0xD8,0xC7,0xAB,0x1C,250 },
{/*"     W25X10", */  0x20000,0xEF,0x10,0x06,0x04,0x05,0x01,0x03,NULL,0x02,0xD8,0xD8,0xC7,0xAB,0x0C,70  },
{/*"     W25X20", */  0x40000,0xEF,0x51,0x06,0x04,0x05,0x01,0x03,NULL,0x02,0xD8,0xD8,0xC7,0xAB,0x1C,250 },
{/*"     W25X40", */  0x80000,0xEF,0x52,0x06,0x04,0x05,0x01,0x03,NULL,0x02,0xD8,0xD8,0xC7,0xAB,0x1C,250 },
{/*"     W25P20", */  0x40000,0xEF,0x11,0x06,0x04,0x05,0x01,0x03,NULL,0x02,0xD8,0xD8,0xC7,0xAB,0x1C,250 },
{/*"     W25P40", */  0x80000,0xEF,0x12,0x06,0x04,0x05,0x01,0x03,NULL,0x02,0xD8,0xD8,0xC7,0xAB,0x1C,250 },
{/*"     W25X16", */  0x20000,0xEF,0x14,0x06,0x04,0x05,0x01,0x03,NULL,0x02,0xD8,0xD8,0xC7,0xAB,0x0C,256  },
{/*"     W25X32", */  0x40000,0xEF,0x15,0x06,0x04,0x05,0x01,0x03,NULL,0x02,0xD8,0xD8,0xC7,0xAB,0x1C,512 },
{/*"     W25X64", */  0x80000,0xEF,0x16,0x06,0x04,0x05,0x01,0x03,NULL,0x02,0xD8,0xD8,0xC7,0xAB,0x1C,1000 },
{/*"     W25Q32", */  0x40000,0xEF,0x16,0x06,0x04,0x05,0x01,0x03,NULL,0x02,0xD8,0xD8,0xC7,0xAB,0x1C,512 },
//SST
{/*"SST25VF016B", */0x1000000,0xBF,0x41,0x06,0x04,0x05,0x01,0x03,NULL,0xAD,0xD8,0xD8,0xC7,0xAB,0x1C,40  },
{/*"SST25VF040B", */  0x80000,0xBF,0x8D,0x06,0x04,0x05,0x01,0x03,NULL,0xAD,0xD8,0xD8,0xC7,0xAB,0x1C,40  },
//MX
{/*" MX25L4005A", */  0x80000,0xC2,0x12,0x06,0x04,0x05,0x01,0x03,0x0B,0x02,0x20,0xD8,0xC7,0xAB,0x1C,80  },
{/*" MX25L8005", */  0x100000,0xC2,0x13,0x06,0x04,0x05,0x01,0x03,0x0B,0x02,0x20,0xD8,0xC7,0xAB,0x1C,160  },
{/*"  MX25L2005", */  0x40000,0xC2,0x11,0x06,0x04,0x05,0x01,0x03,0x0B,0x02,0x20,0xD8,0xC7,0xAB,0x1C,50  },
{/*"  MX25L1605", */0x200000,0xC2,0x14,0x06,0x04,0x05,0x01,0x03,0x0B,0x02,0x20,0xD8,0xC7,0xAB,0x1C,550 },
{/*"  MX25L3205", */0x400000,0xC2,0x15,0x06,0x04,0x05,0x01,0x03,0x0B,0x02,0x20,0xD8,0xC7,0xAB,0x1C,750 },
{/*"  MX25L6405", */0x800000,0xC2,0x16,0x06,0x04,0x05,0x01,0x03,0x0B,0x02,0x20,0xD8,0xC7,0xAB,0x1C,1000 },
//INTEL
{/*"QB25F160S33B8", */  0x20000,0x89,0x11,0x06,0x04,0x05,0x01,0x03,NULL,0x02,0xD7,0xD8,0xC7,0xAB,0x0C,128  },
{/*"QB25F320S33B8", */  0x40000,0x89,0x12,0x06,0x04,0x05,0x01,0x03,NULL,0x02,0xD7,0xD8,0xC7,0xAB,0x0C,256  },
{/*"QB25F640S33B8", */  0x80000,0x89,0x13,0x06,0x04,0x05,0x01,0x03,NULL,0x02,0xD7,0xD8,0xC7,0xAB,0x1C,512  },
//AMIC
{/*" A25L40P", */  0x80000,0x37,0x13,0x06,0x04,0x05,0x01,0x03,0x0B,0x02,0x20,0xD8,0xC7,0xAB,0x1C,128  },
{/*" GD25Q32", */  0x40000,0xC8,0x16,0x06,0x04,0x05,0x01,0x03,0x0B,0x02,0x20,0xD8,0xC7,0xAB,0x1C,512  },
{/*" EN25Q32A", */  0x40000,0x1C,0x15,0x06,0x04,0x05,0x01,0x03,NULL,0x02,0xD8,0xD8,0xC7,0xAB,0x1C,256  },
};

static SWI2C_BusCfg Ursa_I2cBusCfg[URSA_I2C_BUS_NUM]=
{
    // Bus-0
    {PAD_DDCR_CK, PAD_DDCR_DA, 100}, //IS_SW_I2C  /SCL_PAD /SDA_PAD /Delay

    // Bus-1
    //{PAD_GPIO28, PAD_GPIO29, 100}, //IS_SW_I2C  /SCL_PAD /SDA_PAD /Delay

    // Others, add other bus here
};
//default ursa20 i2c addr
MS_U8 g_slave_addr = 0x98;
MS_U8 g_serial_debug_addr = 0xB4;
static void Ursa_SWI2C_Init(void)
{
    char * p_slave_addr = NULL;
    char * p_serial_debug_addr = NULL;
    char * p_pad_scl = NULL;
    char * p_pad_sda = NULL;

    //get slave addr from "slave_addr" & "serial_debug_addr" env
    if((p_slave_addr = getenv ("slave_addr")) != NULL)
    {
        g_slave_addr = (int)simple_strtol(p_slave_addr, NULL, 16);
    }
    if((p_serial_debug_addr = getenv ("serial_debug_addr")) != NULL)
    {
        g_serial_debug_addr = (int)simple_strtol(p_serial_debug_addr, NULL, 16);
    }
    //get i2c bus pad form "pad_scl" & "pad_sda" env
    if((p_pad_scl = getenv ("pad_scl")) != NULL)
    {
        Ursa_I2cBusCfg[0].padSCL = (int)simple_strtol(p_pad_scl, NULL, 10);
    }
    if((p_pad_sda = getenv ("pad_sda")) != NULL)
    {
        Ursa_I2cBusCfg[0].padSDA = (int)simple_strtol(p_pad_sda, NULL, 10);
    }

    UBOOT_INFO("Ursa_SWI2C_Init() - g_slave_addr=0x%x, g_serial_debug_addr=0x%x \n", g_slave_addr, g_serial_debug_addr);
    UBOOT_INFO("Ursa_I2cBusCfg: padSCL=%d, padSDA=%d \n", Ursa_I2cBusCfg[0].padSCL, Ursa_I2cBusCfg[0].padSDA );
    MApi_SWI2C_Init(Ursa_I2cBusCfg, URSA_I2C_BUS_NUM);
}


static void Ursa_SWI2C_ReadDatas(MS_U8  * paddr, MS_U8 addrSize, MS_U8 * pdatabuf, MS_U8 dataSize)
{
    MS_U8 k;
    MApi_SWI2C_UseBus(URSA_ISP_I2C_BUS_SEL);

    if(MApi_SWI2C_AccessStart(g_slave_addr, SW_IIC_WRITE)==FALSE)
    {
    printf("\n I2cReadDatas->IIC_AccessStart Error");
    }

    for(k=0;k<addrSize;k++)
    {
        if(MApi_SWI2C_SendByte(paddr[k])==FALSE)
        {
            printf("\n I2cReadDatas paddr Error");
        }
    }

    if(MApi_SWI2C_AccessStart(g_slave_addr, SW_IIC_READ)==FALSE)
    {
        printf("\n I2cReadDatas->IIC_AccessReStart Error");
    }

    for(k=0;k<dataSize-1;k++)
    {
        pdatabuf[k]=MApi_SWI2C_GetByte(1);
    }
    //last byte
    pdatabuf[k]=MApi_SWI2C_GetByte(0);

    MApi_SWI2C_Stop();
}

static void Ursa_SWI2C_WriteDatas(MS_U8 * paddr, MS_U8 addrSize, MS_U8  * pdatabuf, MS_U8 dataSize)
{
     MS_U8 k;
     MApi_SWI2C_UseBus(URSA_ISP_I2C_BUS_SEL);

    if(MApi_SWI2C_AccessStart(g_slave_addr, SW_IIC_WRITE)==FALSE)
    {
        printf("\n I2cWriteDatas->IIC_AccessStart Error");
    }

    for(k=0;k<addrSize;k++)
    {
        if(MApi_SWI2C_SendByte(paddr[k])==FALSE)
        {
            printf("\n paddr Error");
        }
    }

    for(k=0;k<dataSize;k++)
    {
        if(MApi_SWI2C_SendByte(pdatabuf[k])==FALSE)
        {
            printf("\n pdatabuf Error");
        }
    }

    MApi_SWI2C_Stop();
}

static void Ursa_SWI2C_WriteDatas2(MS_U8 * paddr, MS_U8 addrSize, MS_U8 * pdatabuf, MS_U16 dataSize)
{
    MS_U16 k;
    MApi_SWI2C_UseBus(URSA_ISP_I2C_BUS_SEL);
    MApi_SWI2C_AccessStart(g_serial_debug_addr, SW_IIC_WRITE);

    for(k=0;k<addrSize;k++)
    {
        MApi_SWI2C_SendByte(paddr[k]);
    }
    for(k=0;k<dataSize;k++)
    {
        MApi_SWI2C_SendByte(pdatabuf[k]);
    }

    MApi_SWI2C_Stop();
}
static void Ursa_SerialFlash_WriteReg(MS_U8 bBank, MS_U8 bAddr, MS_U8 bData)
{
    MS_U8 bWriteData[4];

    bWriteData[0] = CMD_DATA_WRITE;
    bWriteData[1] = bBank;
    bWriteData[2] = bAddr;
    bWriteData[3] = bData;

    Ursa_SWI2C_WriteDatas2(bWriteData, sizeof(bWriteData),NULL,0);
}

static void Ursa_SWI2C_WriteStop(void)
{
    MApi_SWI2C_UseBus(URSA_ISP_I2C_BUS_SEL);

    if(MApi_SWI2C_AccessStart(g_slave_addr, SW_IIC_WRITE)==FALSE)
    {
        printf("\n I2cWriteStop->IIC_AccessStart Error");
    }

    if(MApi_SWI2C_SendByte(SPI_STOP)==FALSE)
    {
        printf("\n I2cWriteStop-> Error");
    }

    MApi_SWI2C_Stop();
}
static void Ursa_SWI2C_CommandRead(void)
{
    MApi_SWI2C_UseBus(URSA_ISP_I2C_BUS_SEL);

    if(MApi_SWI2C_AccessStart(g_slave_addr, SW_IIC_WRITE)==FALSE)
    {
        printf("\n I2cCommondRead Error");
    }
    if(MApi_SWI2C_SendByte(SPI_READ)==FALSE)
    {
        printf("\n I2cCommondRead-> SPI_READ");
    }
}

static MS_U8 Ursa_ReadChipID(void)
{
    MS_U8 ReadIDCommand[]={SPI_WRITE,0xAB,0x00,0x00,0x00};
    MS_U8 buf;

    Ursa_SWI2C_WriteDatas(ReadIDCommand,sizeof(ReadIDCommand),NULL,0);

    buf=SPI_READ; //Manufacture ID
    Ursa_SWI2C_ReadDatas(&buf,1,&buf,1);

    buf=SPI_READ; //Device ID1
    Ursa_SWI2C_ReadDatas(&buf,1,&buf,1);

    buf=SPI_READ; //Device ID2
    Ursa_SWI2C_ReadDatas(&buf,1,&buf,1);

    Ursa_SWI2C_WriteStop();
    return buf;
}

static MS_U8 Ursa__ReadChipJEDEC_ID(MS_U8 *ReadIDBuf)
{
    MS_U8 ReadIDCommand[]={SPI_WRITE,0x9f};

    MS_U8 buf;

    Ursa_SWI2C_WriteDatas(ReadIDCommand,sizeof(ReadIDCommand),NULL,0);
    buf=SPI_READ;
    Ursa_SWI2C_ReadDatas(&buf, 1, &ReadIDBuf[0], 3); //Manufacture ID, Device ID1[15-8], Device ID2[7-0]
    Ursa_SWI2C_WriteStop();
    return buf;
}

static MS_U8 Ursa_ReadChipIDBuffer(MS_U8 *ReadIDBuf)
{
    MS_U8 ReadIDCommand[]={SPI_WRITE,0xAB,0x00,0x00,0x00};
    MS_U8 buf;

    Ursa_SWI2C_WriteDatas(ReadIDCommand,sizeof(ReadIDCommand),NULL,0);
    buf=SPI_READ; //Manufacture ID
    Ursa_SWI2C_ReadDatas(&buf,1,&ReadIDBuf[0],1);

    buf=SPI_READ; //Device ID1
    Ursa_SWI2C_ReadDatas(&buf,1,&ReadIDBuf[1],1);

    buf=SPI_READ; //Device ID2
    Ursa_SWI2C_ReadDatas(&buf,1,&ReadIDBuf[2],1);

    Ursa_SWI2C_WriteStop();
    return buf;
}
static MS_BOOL Ursa_EnterIspMode(void)
{
    // 0x4D,0x53,0x54,0x41,0x52
    MS_U8 enterISPdata[]="MSTAR";
    MS_U8 addr=0x20;

    // enter isp mode

    Ursa_SWI2C_WriteDatas(enterISPdata,sizeof(enterISPdata)-1,NULL,0);
    Ursa_SWI2C_ReadDatas(&addr,1,&addr,1);

    if(addr==0xC0)
    {
        printf("\nEnterIspMode->OK\n");
        return TRUE; // enter ok
    }
    else
    {
        printf("\nEnterIspMode->fail\n");
        return FALSE; // enter fail
    }
}
static void Ursa_ExitIspMode(void)
{
    MS_U8 tdata=0x24;

    // exit isp mode
    printf("\n1.ExitIspMode");
    Ursa_SWI2C_WriteDatas(&tdata,1,NULL,0);
}

static void Ursa_EnterSerialDebug(void)
{
    MS_U8 EnterSerialDebug[]={0x53, 0x45, 0x52, 0x44, 0x42};

    Ursa_SWI2C_WriteDatas2(EnterSerialDebug,sizeof(EnterSerialDebug),NULL,0);
}

static void Ursa_EnterSingleStep(void)
{
    MS_U8 EnterSingleStep[]={CMD_DATA_WRITE, 0xC0, 0xC1, 0x53};

    Ursa_SWI2C_WriteDatas2(EnterSingleStep,sizeof(EnterSingleStep),NULL,0);
}

static void Ursa_ExitSingleStep(void)
{
    MS_U8 ExitSingleStep[]={CMD_DATA_WRITE, 0xC0, 0xC1, 0xFF};

    Ursa_SWI2C_WriteDatas2(ExitSingleStep,sizeof(ExitSingleStep),NULL,0);
}

static void Ursa_ExitSerialDebug(void)
{
    MS_U8 ExitSerialDebug[]={0x45};

    Ursa_SWI2C_WriteDatas2(ExitSerialDebug,sizeof(ExitSerialDebug),NULL,0);
}

static void Ursa_WP_PullHigh(void)
{
    Ursa_EnterSerialDebug();
    Ursa_EnterSingleStep();

    // Set chip to power up for register setting
    Ursa_SerialFlash_WriteReg(0x00, 0, 0);
    Ursa_SerialFlash_WriteReg(0x00, 0xF0, 0x02);

    // Only for GIPO P2.4
    Ursa_SerialFlash_WriteReg(0x00, 0x00, 0x01);    // BK1_50[7] = 1
    Ursa_SerialFlash_WriteReg(0x00, 0x50, 0x80);    // Enable register clock for gaultier

    Ursa_SerialFlash_WriteReg(0x00, 0x00, 0x00);
    Ursa_SerialFlash_WriteReg(0x00, 0x63, 0x3D);    // Adjust P2.4 and P2.7
                                               // TSUM chip p2.4 at 0x63[4], p2.7 at 0x63[3]
                                               // TSUMO chip p2.4 at 0x63[5],p2.7 at 0x63[2]
    Ursa_SerialFlash_WriteReg(0x00, 0xc5, 0xFF);    // PWM 1 set to high (P2.7)
    Ursa_SerialFlash_WriteReg(0x00, 0xc7, 0xFF);    // PWM 2 set to high (P2.4)

    // Paulo series chip
    // MCU config pin PWM0~3 default resistor setting = 1001
    Ursa_SerialFlash_WriteReg(0x00, 0x00, 0x0A);
    Ursa_SerialFlash_WriteReg(0x00, 0xED, 0x3F);    // PWM1 pin set to PWM BK0A_ED[7:6]=00
                                               // the other pin set to input mode
    // Lopez series chip
    // MCU config pin PWM2
    Ursa_SerialFlash_WriteReg(0x1E, 0xE5, 0xC8);
    Ursa_SerialFlash_WriteReg(0x1E, 0x8A, 0xC0);

    Ursa_ExitSingleStep();
    Ursa_ExitSerialDebug();
}

MS_BOOL GetsFlashType(void)
{
    MS_U8 temp;
    MS_U8 ucTmpBuf[4];

    Ursa__ReadChipJEDEC_ID(ucTmpBuf);
    printf("\n***ReadChipJEDEC_ID[%x]",ucTmpBuf[0]);
    printf("\n***ReadChipJEDEC_ID[%x]",ucTmpBuf[1]);
    printf("\n***ReadChipJEDEC_ID[%x]",ucTmpBuf[2]);
    switch(ucTmpBuf[0])
    {
         //AL
        case 0x37:
            printf("\n  AL or AMIC");
            temp=Ursa_ReadChipID();
            printf("\n  AL or AMIC ChipID=%x",temp);
            if (temp == 0x12)
            {
                g_ucFlashType=Flash_A25L40P;
                g_ucBankNumber=8;
                printf("\n   A25L40P");
            }
            else
            {
              return FALSE;
            }
            break;
    ///////////////////////////////////////////////

        //PMC
        case 0x7F:
            printf("\n  PMC or AMIC");
            temp=Ursa_ReadChipID();
            printf("\n  PMC or AMIC ChipID=%x",temp);
            if (temp == 0x7C)
            {
                g_ucFlashType=Flash_PMC010;
                g_ucBankNumber=2;
            }
            else if (temp == 0x7D)
            {
                g_ucFlashType=Flash_PMC020;
                g_ucBankNumber=4;
            }
            else if (temp == 0x7E)
            {
                g_ucFlashType=Flash_PMC040;
                g_ucBankNumber=8;
            }
            else if (temp == 0x12)
            {
                g_ucFlashType=Flash_A25L40P;
                g_ucBankNumber=8;
                printf("\n   A25L40P");
            }
            else
            {
                return FALSE;
            }
            break;
        //EON
        case 0x1C:
            printf("\n  EON");
            temp=Ursa_ReadChipID();
            printf("\n  EON ChipID=%x",temp);
            if (ucTmpBuf[2] == 0x20)
            {
                if (ucTmpBuf[1] == 0x11 && temp == 0x10)
                {
                    g_ucFlashType = Flash_EN25P10;
                    g_ucBankNumber=2;
                    printf("\n   EN25P10");
                }
                else if (ucTmpBuf[1] == 0x12 && temp == 0x11)
                {
                    g_ucFlashType = Flash_EN25P20;
                    g_ucBankNumber=4;
                    printf("\n   EN25P20");
                }
                else if (ucTmpBuf[1] == 0x13 && temp == 0x12)
                {
                    g_ucFlashType = Flash_EN25P40;
                    g_ucBankNumber=8;
                    printf("\n   EN25P40");
                }
                else if (ucTmpBuf[1] == 0x14 && temp == 0x13)
                {
                    g_ucFlashType = Flash_EN25P80;
                    g_ucBankNumber=16;
                    printf("\n   EN25P80");
                }
                else if (ucTmpBuf[1] == 0x12 && temp == 0x41)
                {
                    g_ucFlashType = Flash_EN25B20;
                    g_ucBankNumber=4;
                    printf("\n   EN25B20");
                }
                else if (ucTmpBuf[1] == 0x13 && temp == 0x32)
                {
                    g_ucFlashType = Flash_EN25B40;
                    g_ucBankNumber=8;
                    printf("\n   EN25B40");
                }
                else if (ucTmpBuf[1] == 0x13 && temp == 0x42)
                {
                    g_ucFlashType = Flash_EN25B40T;
                    g_ucBankNumber=8;
                    printf("\n   EN25B40T");
                }
                else if (ucTmpBuf[1] == 0x15 && temp == 0x14)
                {
                    g_ucFlashType = Flash_EN25P16;
                    g_ucBankNumber = 32;
                    printf("\n   EN25P16");
                }
                else if (ucTmpBuf[1] == 0x15 && temp == 0x34)
                {
                    g_ucFlashType = Flash_EN25B16;
                    g_ucBankNumber = 32;
                    printf("\n   EN25B16");
                }
                else if (ucTmpBuf[1] == 0x15 && temp == 0x44)
                {
                    g_ucFlashType = Flash_EN25B16T;
                    g_ucBankNumber = 32;
                    printf("\n   EN25B16T");
                }
                else if (ucTmpBuf[1] == 0x16 && temp == 0x35)
                {
                    g_ucFlashType = Flash_EN25B32;
                    g_ucBankNumber = 64;
                    printf("\n   EN25B32");
                }
                else
                {
                    return FALSE;
                }
            }
            else if (ucTmpBuf[2] == 0x31)
            {
                if (ucTmpBuf[1] == 0x13 && temp == 0x12)
                {
                    g_ucFlashType = Flash_EN25F40;
                    g_ucBankNumber=8;
                    printf("\n   EN25F40");
                }
                else if (ucTmpBuf[1] == 0x14 && temp == 0x13)
                {
                    g_ucFlashType = Flash_EN25F80;
                    g_ucBankNumber=16;
                    printf("\n   EN25F80");
                }
                else if (ucTmpBuf[1] == 0x15 && temp == 0x14)
                {
                    g_ucFlashType = Flash_EN25F16;
                    g_ucBankNumber=32;
                    printf("\n   EN25F16");
                }
            }
            else if (ucTmpBuf[2] == 0x30)
            {
                if (ucTmpBuf[1] == 0x16 && temp == 0x15)
                {
                    g_ucFlashType = Flash_EN25Q32A;
                    g_ucBankNumber = 64;
                    printf("\n   EN25Q32A");
                }
            }
            else
                {
                    return FALSE;
                }
            break;
        case 0x01:
            printf("\n  SPANSION");
            printf("\n   chip ID=%x",ucTmpBuf[2]);
            if (ucTmpBuf[2] == 0x14)
            {
                g_ucFlashType = Flash_S25FL016A;
                g_ucBankNumber = 32;
                printf("\n   S25FL016A");
            }
            else if (ucTmpBuf[2] == 0x12)
            {
                g_ucFlashType = Flash_S25FL004A;
                g_ucBankNumber = 8;
                printf("\n   S25FL004A");
            }
            else if (ucTmpBuf[2] == 0x25)
            {
                g_ucFlashType = Flash_S25FL040A;
                g_ucBankNumber = 8;
                printf("\n   S25FL040A");
            }
            else
            {
                return FALSE;
            }
            break;
        case 0xEF:
            printf("\n  WINBOND & NX");
            if (ucTmpBuf[1] == 0x20 && ucTmpBuf[2] == 0x15)
            {
                g_ucFlashType = Flash_NX25P16;
                g_ucBankNumber = 32;
                printf("\n   NX25P16 or W25P16");
            }
            else if (ucTmpBuf[1] == 0x30 && ucTmpBuf[2] == 0x11)
            {
                g_ucFlashType = Flash_W25X10;
                g_ucBankNumber = 2;
                printf("\n   W25X10");
            }
            else if (ucTmpBuf[1] == 0x30 && ucTmpBuf[2] == 0x12)
            {
                g_ucFlashType = Flash_W25X20;
                g_ucBankNumber = 4;
                printf("\n   W25X20");
            }
            else if (ucTmpBuf[1] == 0x30 && ucTmpBuf[2] == 0x13)
            {
                g_ucFlashType = Flash_W25X40;
                g_ucBankNumber = 8;
                printf("\n   W25X40");
            }
            else if (ucTmpBuf[1] == 0x00 && ucTmpBuf[2] == 0x15)
            {
                g_ucFlashType = Flash_W25X16;
                g_ucBankNumber = 32;
                printf("\n   W25X16");
            }
            else if (ucTmpBuf[1] == 0x00 && ucTmpBuf[2] == 0x16)
            {
                g_ucFlashType = Flash_W25X32;
                g_ucBankNumber = 64;
                printf("\n   W25X32");
            }
            else if (ucTmpBuf[1] == 0x00 && ucTmpBuf[2] == 0x17)
            {
                g_ucFlashType = Flash_W25X64;
                g_ucBankNumber = 128;
                printf("\n   W25X64");
            }
            else if (ucTmpBuf[1] == 0x40 && ucTmpBuf[2] == 0x16)
            {
                g_ucFlashType = Flash_W25Q32;
                g_ucBankNumber = 64;
                printf("\n   W25Q32");
            }
            else
            {
                temp=Ursa_ReadChipID();
                printf("\n ID 2:%x",temp);
                if (temp == 0x51)
                {//because some W25X20 have no ucTmpBuf[2](=0x00)
                    g_ucFlashType = Flash_W25X20;
                    g_ucBankNumber = 4;
                    printf("\n   W25X20-0");
                }
                else if(temp == 0x10)
                {
                    g_ucFlashType = Flash_W25X10;
                    g_ucBankNumber = 2;
                    printf("\n   W25X10-0");
                }
                else if(temp == 0x52)
                {//because some W25X40 have no ucTmpBuf[2](=0x00)
                    g_ucFlashType = Flash_W25X40;
                    g_ucBankNumber = 8;
                    printf("\n   W25X40-0");
                }
                else if(temp == 0x11)
                {//because some W25X40 have no ucTmpBuf[2](=0x00)
                    g_ucFlashType = Flash_W25P20;
                    g_ucBankNumber = 4;
                    printf("\n   W25P20");
                }
                else if(temp == 0x12)
                {//because some W25X40 have no ucTmpBuf[2](=0x00)
                    g_ucFlashType = Flash_W25P40;
                    g_ucBankNumber = 8;
                    printf("\n   W25P40");
                }
                else if(temp == 0x14)
                {
                    g_ucFlashType = Flash_NX25P16;
                    g_ucBankNumber = 32;
                    printf("\n   W25P16");
                }
                else
                {
                   return FALSE;
                }
            }
            break;
        case 0xBF:
            printf("\n  SST");
            if ((ucTmpBuf[1] == 0x25) & (ucTmpBuf[2] == 0x41))
            {
                g_ucFlashType = Flash_SST25VF016B;
                g_ucBankNumber = 32;
                printf("\n   SST25VF016B");
            }
            else if ((ucTmpBuf[1] == 0x25) & (ucTmpBuf[2] == 0x8D))
            {
                g_ucFlashType = Flash_SST25VF040B;
                g_ucBankNumber = 8;
                printf("\n   SST25VF040B");
            }
            else
            {
                 return FALSE;
            }
            break;
        case 0xC2:
            printf("\n MX");

            printf("\n JEDEC_ID1 0:%x",ucTmpBuf[0]);
            printf("\n JEDEC_ID1 1:%x",ucTmpBuf[1]);
            printf("\n JEDEC_ID1 2:%x",ucTmpBuf[2]);
            if ((ucTmpBuf[1] == 0x20) & (ucTmpBuf[2] == 0x13))
            {
                g_ucFlashType = Flash_MX25L4005A;
                g_ucBankNumber = 8;
                printf("\n   MX25L4005A");
            }
            else if ((ucTmpBuf[1] == 0x20) & (ucTmpBuf[2] == 0x14))
            {
                g_ucFlashType = Flash_MX25L8005;
                g_ucBankNumber = 16;
                printf("\n   MX25L8005");
            }
            else if ((ucTmpBuf[1] == 0x20) & (ucTmpBuf[2] == 0x12))
            {
                g_ucFlashType = Flash_MX25L2005;
                g_ucBankNumber = 4;
                printf("\n   MX25L2005");
            }
            else if((ucTmpBuf[1] == 0x20) & (ucTmpBuf[2] == 0x15))
            {
                g_ucFlashType = Flash_MX25L1605;
                g_ucBankNumber = 32;
                printf("\n   MX25L1605");
            }
            else if((ucTmpBuf[1] == 0x20) & (ucTmpBuf[2] == 0x16))
            {
                g_ucFlashType = Flash_MX25L3205;
                g_ucBankNumber = 32;
                printf("\n   MX25L3205");
            }
            else if((ucTmpBuf[1] == 0x20) & (ucTmpBuf[2] == 0x17))
            {
                g_ucFlashType = Flash_MX25L6405;
                g_ucBankNumber = 128;
                printf("\n   MX25L6405");
            }
            else
            {
                 return FALSE;
            }
            break;
        //intel
        case 0x89:
            printf("\n  INTEL");
            temp=Ursa_ReadChipID();
            printf("\n  INTEL ChipID=%x",temp);
            if (ucTmpBuf[1] == 0x11)
            {
                g_ucFlashType=Flash_QB25F160S;
                g_ucBankNumber=32;
                printf("\n   INTS33_16");
            }
            else if (ucTmpBuf[1] == 0x12)
            {
                g_ucFlashType=Flash_QB25F320S;
                g_ucBankNumber=64;
                printf("\n   INTS33_32");
            }
            else if (ucTmpBuf[1] == 0x13)
            {
                g_ucFlashType=Flash_QB25F640S;
                g_ucBankNumber=128;
                printf("\n   INTS33_64");
            }
            else
            {
                return FALSE;
            }
            break;
            //GD
        case 0xC8:
            printf("\n  GD");
            if ((ucTmpBuf[1] == 0x40) && (ucTmpBuf[2] == 0x16))
            {
                g_ucFlashType = Flash_GD25Q32;
                g_ucBankNumber = 32;
                printf("\n  GD25Q32");
            }
            else
            {
                 return FALSE;
            }
            break;
        default:
            printf("\n JEDEC_ID 0:%x",ucTmpBuf[0]);
            printf("\n JEDEC_ID 1:%x",ucTmpBuf[1]);
            printf("\n JEDEC_ID 2:%x",ucTmpBuf[2]);
            Ursa_ReadChipIDBuffer(ucTmpBuf);
            printf("\n RDID 0:%x",ucTmpBuf[0]);
            printf("\n RDID 1:%x",ucTmpBuf[1]);
            printf("\n RDID 2:%x",ucTmpBuf[2]);
            if ((ucTmpBuf[0] == 0x9D) && (ucTmpBuf[1] == 0x7F) && (ucTmpBuf[2] == 0x7B))
            {
                printf("\n  PMC-RDID");
                printf("\n   PM25LV512A/PM25LV512");
                g_ucFlashType=Flash_PMC512;
                g_ucBankNumber=1;
            }
            else
            {
                 return FALSE;
            }
            break;
    }
    Ursa_ReadChipIDBuffer(ucTmpBuf);
    printf("\n RDID1 0:%x",ucTmpBuf[0]);
    printf("\n RDID1 1:%x",ucTmpBuf[1]);
    printf("\n RDID1 2:%x",ucTmpBuf[2]);
    return TRUE;
}

MS_U8 SrcReadFlash(MS_U16 pcount , MS_U8 BankNum)
{
    MS_U8 u8Data;
    if(g_eUrsaUpgradeSel == E_MFC_SYSTEM_BIN)
    {
        memcpy(&u8Data,(void *)(g_u32ISPSrcAddr+(MS_U32)pcount + (MS_U32)(BankNum - g_ucBankDst)*0x10000), 1);
    }
    else // Upgrade MFC FW by default
    {
        memcpy(&u8Data,(void *)(g_u32ISPSrcAddr+(MS_U32)pcount + (MS_U32)BankNum*0x10000), 1);
    }

    return u8Data;
}


MS_U8 SFlashRdsr(void)
{
    MS_U8 buffer[2];

    buffer[0]=SPI_WRITE;
    buffer[1]=sFlash.RDSR;
    Ursa_SWI2C_WriteDatas(buffer,2,NULL,0);

    buffer[0]=SPI_READ;
    Ursa_SWI2C_ReadDatas(buffer,1,&buffer[1],1);

    Ursa_SWI2C_WriteStop();

    return buffer[1];
}

MS_U16 SFlashRdsr_1_2(void)
{
    MS_U8 buffer[2];
    MS_U16 u16value = 0x00;
    /* read status register 1 */
    buffer[0]=SPI_WRITE;
    buffer[1]=sFlash.RDSR;
    Ursa_SWI2C_WriteDatas(buffer,2,NULL,0);
    buffer[0]=SPI_READ;
    Ursa_SWI2C_ReadDatas(buffer,1,&buffer[1],1);
    Ursa_SWI2C_WriteStop();
    u16value = (MS_U16)buffer[1];
    /* read status register 2 */
    buffer[0]=SPI_WRITE;
    buffer[1]=0x35;
    Ursa_SWI2C_WriteDatas(buffer,2,NULL,0);
    buffer[0]=SPI_READ;
    Ursa_SWI2C_ReadDatas(buffer,1,&buffer[1],1);
    Ursa_SWI2C_WriteStop();
    u16value = ((buffer[1] << 8) | u16value);
    return u16value;
}

MS_BOOL SFlashChecksr(void)
{
    MS_U8 ucdelay = 10;
    MS_U32 count=1000;

    while(count--)
    {
        if((SFlashRdsr()&0x01)==0x00)
            return TRUE;
        while(ucdelay--);
    }
    return FALSE;
}


void SFlashWrsr(MS_U8 tdata)
{
    MS_U8 buffer[3];

    SFlashChecksr();

    // for sst sflash (sst manufacturer id=0xBF )
    if(sFlash.manufacturer_id==0xBF)
    {
        buffer[0]=SPI_WRITE;
        buffer[1]=0x50;
        Ursa_SWI2C_WriteDatas(buffer,2,NULL,0);

        Ursa_SWI2C_WriteStop();
    }

    buffer[0]=SPI_WRITE;
    buffer[1]=sFlash.WRSR;
    buffer[2]=tdata;
    Ursa_SWI2C_WriteDatas(buffer,sizeof(buffer),NULL,0);

    Ursa_SWI2C_WriteStop();
}

void SFlashWrsr_1_2(MS_U16 tdata)
{
    MS_U8 buffer[4];
    SFlashChecksr();
    buffer[0]=SPI_WRITE;
    buffer[1]=sFlash.WRSR;
    buffer[2]=(tdata & 0x00FF); //status register 1
    buffer[3]=((tdata & 0xFF00) >> 8); //status register 2
    Ursa_SWI2C_WriteDatas(buffer,sizeof(buffer),NULL,0);
    Ursa_SWI2C_WriteStop();
}

void SFlashWren(void)
{
    MS_U8 buffer[2];

    buffer[0]=SPI_WRITE;
    buffer[1]=sFlash.WREN;

    SFlashChecksr();
    Ursa_SWI2C_WriteDatas(buffer,2,NULL,0);

    Ursa_SWI2C_WriteStop();
}
void SFlashWrdi(void)
{
    MS_U8 buffer[2];

    buffer[0]=SPI_WRITE;
    buffer[1]=sFlash.WRDI;

    SFlashChecksr();
    Ursa_SWI2C_WriteDatas(buffer,2,NULL,0);

    buffer[0]=SPI_STOP;
    Ursa_SWI2C_WriteDatas(buffer,1,NULL,0);
}

MS_BOOL VerifyPMC64K(U8 BankNum)
{
    MS_U8 tdata[]={SPI_WRITE,0x03,0x00,0x00,0x00};
    MS_BOOL VerifyOk=TRUE;
    MS_U16 PcCounter=0;
    MS_U8 FlashReadData;
    MS_U16 u16PcCounterEnd = 0;

    tdata[1]=sFlash.READS;
    tdata[2]=BankNum;
    tdata[3]=0;
    tdata[4]=0;

    Ursa_SWI2C_WriteDatas(tdata,sizeof(tdata),NULL,0);
    udelay(60*1000);
    {
        Ursa_SWI2C_CommandRead();
           udelay(60*1000);
        MApi_SWI2C_UseBus(URSA_ISP_I2C_BUS_SEL);

    if(MApi_SWI2C_AccessStart(g_slave_addr, SW_IIC_READ)==FALSE)
    {
        printf("\n VerifyPMC64K Error");
    }

        if(g_eUrsaUpgradeSel == E_MFC_SYSTEM_BIN)
        {
            u16PcCounterEnd = SYSBIN_SIZE - 2; // system bin only 1KB needed (0 ~ 3).
        }
        else // Upgrade MFC FW by default
        {
            u16PcCounterEnd = BANKSIZE - 2;
        }

        for(PcCounter=0;PcCounter<=u16PcCounterEnd;PcCounter++)
        {
            FlashReadData=MApi_SWI2C_GetByte(1);
            if(FlashReadData!=SrcReadFlash(PcCounter,BankNum))
            {
                VerifyOk=FALSE;
                printf("\n flash read date error[%x],[%x],[%x]",PcCounter,FlashReadData,SrcReadFlash(PcCounter,BankNum));
                break;
            }
        }

        //last byte
        {
            FlashReadData=MApi_SWI2C_GetByte(0);
            //PcCounter++;

            if(FlashReadData!=SrcReadFlash(PcCounter,BankNum))
                VerifyOk=FALSE;
        }

        // 1 bank program end
        MApi_SWI2C_Stop();
    }

    Ursa_SWI2C_WriteStop();

    return VerifyOk;
}

void ProgSST64K(MS_U8 BankNum)
{
    MS_U8 k;
    MS_U8 tdata[7];
    MS_U16 PcCounter=0;

    SFlashWrsr(0x00);

    SFlashWren();
    SFlashChecksr();

    //AAI send the address
    tdata[0]=SPI_WRITE;
    tdata[1]=sFlash.PG_PROG;
    tdata[2]=BankNum;
    tdata[3]=0;
    tdata[4]=0;
    tdata[5]=SrcReadFlash(PcCounter,BankNum);
    PcCounter++;
    tdata[6]=SrcReadFlash(PcCounter,BankNum);
    PcCounter++;

    MApi_SWI2C_UseBus(URSA_ISP_I2C_BUS_SEL);
    MApi_SWI2C_AccessStart(g_slave_addr, SW_IIC_WRITE);

    for(k=0;k<sizeof(tdata);k++)
        MApi_SWI2C_SendByte(tdata[k]);
    Ursa_SWI2C_WriteStop();
    SFlashChecksr();

    //AAI send the datas
    tdata[0]=SPI_WRITE;
    tdata[1]=sFlash.PG_PROG;
    for (;PcCounter<0xFFFE;)
    {
        tdata[2]=SrcReadFlash(PcCounter,BankNum);
        PcCounter++;
        tdata[3]=SrcReadFlash(PcCounter,BankNum);
        PcCounter++;

        MApi_SWI2C_UseBus(URSA_ISP_I2C_BUS_SEL);
        MApi_SWI2C_AccessStart(g_slave_addr, SW_IIC_WRITE);
        for(k=0;k<4;k++)
            MApi_SWI2C_SendByte(tdata[k]);
        Ursa_SWI2C_WriteStop();
        //SFlashChecksr();
    }
    //AAI send the remain 2 bytes
    tdata[2]=SrcReadFlash(PcCounter,BankNum);
    PcCounter++;
    tdata[3]=SrcReadFlash(PcCounter,BankNum);

    MApi_SWI2C_UseBus(URSA_ISP_I2C_BUS_SEL);
    MApi_SWI2C_AccessStart(g_slave_addr, SW_IIC_WRITE);
    for(k=0;k<4;k++)
        MApi_SWI2C_SendByte(tdata[k]);
    Ursa_SWI2C_WriteStop();
    SFlashChecksr();
    //AAI stop
    SFlashWrdi();
    SFlashChecksr();
}

void ProgPMC64K(MS_U8 BankNum)
{
    MS_U8 k;
    MS_U16 i;
    MS_U8 tdata[5];
    MS_U8 count;
    MS_U16 PcCounter=0;
    MS_U16 u16BlockEnd = 0;

    if(sFlash.manufacturer_id==0xEF) /* WINBOND & NX */
    {
        MS_U16 u16StatusReg = SFlashRdsr_1_2();
        SFlashWrsr_1_2(u16StatusReg & ~(BIT14 | BIT4 | BIT3 | BIT2)); // unprotected => CMP(BIT14) = 0; BP2(BIT4) = 0, BP1(BIT3) = 0, and BP0(BIT2) = 0.
    }
    else
    {
        SFlashWrsr(0x00);
    }

    if(g_eUrsaUpgradeSel == E_MFC_SYSTEM_BIN)
    {
        u16BlockEnd = (SYSBIN_SIZE / BLOCKSIZE) - 1; // system bin only 1KB needed (0 ~ 3).
    }
    else // Upgrade MFC FW by default
    {
        u16BlockEnd = BLOCKNUM;
    }
    for(i=0;i<=u16BlockEnd;i++)
    {
        SFlashWren();
        SFlashChecksr();

        count=BLOCKSIZE-1;

        tdata[0]=SPI_WRITE;
        tdata[1]=sFlash.PG_PROG;
        tdata[2]=BankNum;
        tdata[3]=i;
        tdata[4]=0;

        MApi_SWI2C_UseBus(URSA_ISP_I2C_BUS_SEL);
        MApi_SWI2C_AccessStart(g_slave_addr, SW_IIC_WRITE);

        for(k=0;k<sizeof(tdata);k++)
            MApi_SWI2C_SendByte(tdata[k]);

        while(count--)
        {
            MApi_SWI2C_SendByte(SrcReadFlash(PcCounter,BankNum));

            PcCounter++;
        }

        // 1 byte more
        MApi_SWI2C_SendByte(SrcReadFlash(PcCounter,BankNum));
        PcCounter++;

        //block program end
        Ursa_SWI2C_WriteStop();
        //printf("*");
    }

    Ursa_SWI2C_WriteDatas(tdata,2,NULL,0);
    SFlashWrdi();
    SFlashChecksr();
}

void SFlashChip_Erase(void)
{
    //U8 tdata[5]={SPI_WRITE,0x00,0x00,0x00,0x00};
    MS_U8 addr[2];

    SFlashWrsr(0x00);
    SFlashWren();
    SFlashChecksr();

    addr[0]=SPI_WRITE;
    addr[1]=sFlash.CHIP_ERASE;
    Ursa_SWI2C_WriteDatas(addr,2,NULL,0);

    Ursa_SWI2C_WriteStop();

    SFlashChecksr();//Vick Add
}

void SFlashBlock_Erase(MS_U32 bank_num)
{
    MS_U8 addr[5] = {0};

    SFlashWren();
    if(sFlash.manufacturer_id==0xEF) /* WINBOND & NX */
    {
        MS_U16 u16StatusReg = SFlashRdsr_1_2();
        SFlashWrsr_1_2(u16StatusReg & ~(BIT14 | BIT4 | BIT3 | BIT2)); // unprotected => CMP(BIT14) = 0; BP2(BIT4) = 0, BP1(BIT3) = 0, and BP0(BIT2) = 0.
    }
    else
    {
        SFlashWrsr(0x00);
    }
    SFlashWren();
    SFlashChecksr();

    addr[0]=SPI_WRITE;
    addr[1]=sFlash.BLOCK_ERASE;
    addr[2] = bank_num;
    addr[3] = bank_num << 8;
    addr[4] = bank_num << 16;

    Ursa_SWI2C_WriteDatas(addr, sizeof(addr), NULL, 0);

    Ursa_SWI2C_WriteStop();

    SFlashChecksr();
}

void SFlashProgram(void)
{
    MS_U16    num;
    MS_U8 u8BankStart;
    MS_U8 u8BankEnd;

    if(g_eUrsaUpgradeSel == E_MFC_SYSTEM_BIN)
    {
        u8BankStart = g_ucBankDst;
        u8BankEnd =  g_ucBankDst + g_ucBankNumber;
    }
    else // Upgrade MFC FW by default
    {
        u8BankStart = 0;
        u8BankEnd = g_ucBankNumber;
    }

    UBOOT_INFO("u8BankStart %x\n", u8BankStart);
    UBOOT_INFO("u8BankEnd %x\n", u8BankEnd);

    for(num = u8BankStart; num < u8BankEnd; num++)
    {
        printf("\n Write Bank %x",num);
        if (g_ucFlashType==Flash_SST25VF016B||g_ucFlashType==Flash_SST25VF040B)
            ProgSST64K(num);
        else
            ProgPMC64K(num);
        g_u8CurPercent = 80 - 40*(num+1)/u8BankEnd;
        if(g_eUrsaUpgradeSel == E_MFC_FIRMWARE)
        {
            show_Upgrading((100 - g_u8CurPercent), 100);
        }
        else
        {
            printf("\n g_eUrsaUpgradeSel != E_MFC_FIRMWARE\n");
        }
    }
}
MS_BOOL SFlashVerify(void)
{
    MS_U8    num;
    MS_U8 u8BankStart;
    MS_U8 u8BankEnd;


    if(g_eUrsaUpgradeSel == E_MFC_SYSTEM_BIN)
    {
        u8BankStart = g_ucBankDst;
        u8BankEnd =  g_ucBankDst + g_ucBankNumber;
    }
    else // Upgrade MFC FW by default
    {
        u8BankStart = 0;
        u8BankEnd = g_ucBankNumber;
    }

    UBOOT_INFO("u8BankStart %x\n", u8BankStart);
    UBOOT_INFO("u8BankEnd %x\n", u8BankEnd);

    for(num = u8BankStart; num < u8BankEnd; num++)
    {
        printf("\n Verify Bank %x",num);
        if(VerifyPMC64K(num)==FALSE)
            return FALSE;
        g_u8CurPercent = 40 - 40*(num+1)/u8BankEnd;
        if(g_eUrsaUpgradeSel == E_MFC_FIRMWARE)
        {
            show_Upgrading((100 - g_u8CurPercent), 100);
        }
        else
        {
            printf("\n g_eUrsaUpgradeSel != E_MFC_FIRMWARE\n");
        }
    }
    return TRUE;
}
void Ursa_ISP_Init(MS_U32 u32Addr, MS_U32 u32Size, MS_U8 u8UpgradeSel)
{
    ErrorFlag = FlashProgOK;
    g_eFlashProgramState = FlashProgStateInit;
    g_u32ISPSrcAddr = u32Addr;
    g_u32ISPCodeSize = u32Size;
    g_eUrsaUpgradeSel = (EN_MFC_UPGRADE_SEL_t)u8UpgradeSel;
}

int Ursa_ISP_Program(void)
{
    MS_U16 wErrCounter = 0;
    MS_U8 Bank_Num = 0;

    if (ErrorFlag != FlashProgOK)
    {
        return (0xF0+ErrorFlag);
    }

    switch (g_eFlashProgramState)
    {
        case FlashProgStateInit:
            g_u8CurPercent = 100;
            g_eFlashProgramState = FlashProgStateEnterIsp;
            break;

        case FlashProgStateEnterIsp:
            printf("\n1:ISP Enter\n");

            for(wErrCounter = 0;wErrCounter<ENTER_ISP_ERROR_MAX;)
            {//for arcing
                Ursa_WP_PullHigh();
                if (!Ursa_EnterIspMode())
                {
                    wErrCounter++;
                    udelay(10*1000);
                    Ursa_ExitIspMode();
                    udelay(80*1000);
                }
                else
                    break;
            }

            if (wErrCounter>=ENTER_ISP_ERROR_MAX)
            {
                ErrorFlag=FlashEnterIspModeErr;
                printf("\nCan not enter ISP\n");
                goto ProgEnd;
            }
            else
            {
                printf("\nEnter ISP Successful[%x]\n",wErrCounter);
            }
            udelay(50*1000);//Wait the Protect pin to hight.
            for(wErrCounter=0;wErrCounter<GET_ID_ERROR_MAX;)
            {
                if (!GetsFlashType())
                {
                    wErrCounter++;
                    udelay(50*1000);
                }
                else
                    break;
            }

            if (wErrCounter>=GET_ID_ERROR_MAX)
            {
                printf("\n Detect SPI Flash Failed");
                g_ucFlashType=Flash_A25L40P;
                g_ucBankNumber=8;
            }
            sFlash=SupportSFlash[g_ucFlashType];

            printf("\n Bank Number=%d",g_ucBankNumber);
            //-------------------------
            g_ucBankNumber = g_u32ISPCodeSize/0x10000;
            if (0 != (g_u32ISPCodeSize%0x10000))
            {
                g_ucBankNumber = g_ucBankNumber + 1;
            }
            //-------------------------
            printf("\n Program Banks=%d",g_ucBankNumber);

            //------------------------
            //disable write protect and set status 0x00
            SFlashWren();
            SFlashWrsr(0x00);
            g_u8CurPercent = 95;
            g_eFlashProgramState = FlashProgStateErase;
            break;

        case FlashProgStateErase:
            //Step2:Block Erase
            printf("\n2:Block Erase \n");
            MS_U8 u8BankStart;
            MS_U8 u8BankEnd;

            if(g_eUrsaUpgradeSel == E_MFC_SYSTEM_BIN)
            {
                u8BankStart = g_ucBankDst;
                u8BankEnd =  g_ucBankDst + g_ucBankNumber;
            }
            else // Upgrade MFC FW by default
            {
                u8BankStart = 0;
                u8BankEnd = g_ucBankNumber;
            }

            UBOOT_INFO("u8BankStart %x\n", u8BankStart);
            UBOOT_INFO("u8BankEnd %x\n", u8BankEnd);


            for (Bank_Num = u8BankStart; Bank_Num < u8BankEnd; Bank_Num++)
            {
                printf("Erase Bank %x\n", Bank_Num);
                SFlashBlock_Erase(Bank_Num);

                 //wait the flash erase finished.
                 for (wErrCounter = 0; wErrCounter < BLOCK_ERASE_ERROR_MAX; wErrCounter++)
                 {
                    if (SFlashRdsr()&0x01)
                    {
                         udelay(100*1000);
                         wErrCounter++;
                         udelay(100*1000);
                         wErrCounter++;
                         udelay(100*1000);
                         wErrCounter++;
                         udelay(100*1000);
                         wErrCounter++;
                         udelay(100*1000);
                         wErrCounter++;
                         udelay(100*1000);
                         wErrCounter++;
                         udelay(100*1000);
                         wErrCounter++;
                         udelay(100*1000);
                         wErrCounter++;
                         udelay(100*1000);
                         wErrCounter++;
                         udelay(100*1000);
                     }
                     else
                     {
                         break;
                     }
                 }
             }

            if (wErrCounter >= BLOCK_ERASE_ERROR_MAX)
            {
                printf("Block Erase fail!!\n");
                goto ProgEnd;
            }
            else
            {
                printf("Block Erase Success\n");
            }

            printf("\n Wait Count:%d",wErrCounter);
            g_u8CurPercent = 80;
            g_eFlashProgramState = FlashProgStateBlank;
            break;

        case FlashProgStateBlank:
        #if 0
                //Step3:Blanking Check
                printf("\n3:Blanking");
                if(!SFlashBlanking(0,sFlash.size))
                {
                    ErrorFlag=FlashBlankingErr;
                    goto ProgEnd;
                }
        #endif
            g_u8CurPercent = 40;
            g_eFlashProgramState = FlashProgStateProgram;
            break;

        case FlashProgStateProgram:
            //Step4:Prog Flash
            printf("\n4:Prog... \n");
            SFlashProgram();
            g_u8CurPercent = 20;
            g_eFlashProgramState = FlashProgStateVerify;
            break;

        case FlashProgStateVerify:
            //Step5:Verify
            printf("\n5:Verify... \n");
            SFlashWren();
            SFlashWrsr(sFlash.LOCK);
            SFlashWrdi();

            if(!SFlashVerify())
            {
                ErrorFlag=FlashVerifyErr;
                goto ProgEnd;
            }
            printf("\nVerify->OK!");
            g_u8CurPercent = 10;
            g_eFlashProgramState = FlashProgStateExit;
            break;

        case FlashProgStateExit:
            //Step6:Exit ISP
            printf("\n6:Exit ISP");
            Ursa_ExitIspMode();

            g_u8CurPercent = 0;
            g_eFlashProgramState = FlashProgStateIdle;
            break;

       case FlashProgStateIdle:
        break;
    }

ProgEnd:
    if (ErrorFlag != FlashProgOK)
    {
        Ursa_ExitIspMode();
#if (ENABLE_WATCH_DOG == ENABLE)
        MDrv_Sys_EnableWatchDog();
#endif
        return (0xF0+ErrorFlag);
    }
    else
    {
        return g_u8CurPercent;
    }
    return 0;
}

static void _mfcSerialDebugMode(MS_U8 ucCmd)
{
    MS_BOOL bRet = 0;
    MS_U8 u8MsData[5] = {0};

    if (ucCmd == SerialDebugEnter)
    {
        u8MsData[0] = 0x53;
        u8MsData[1] = 0x45;
        u8MsData[2] = 0x52;
        u8MsData[3] = 0x44;
        u8MsData[4] = 0x42;
        Ursa_SWI2C_WriteDatas2(u8MsData,5,NULL,0);
    }
    else
    {
        u8MsData[0] = ucCmd;
        Ursa_SWI2C_WriteDatas2(u8MsData,1,NULL,0);
    }
}

static void MFCEnterSerialDebugMode(void)
{
    _mfcSerialDebugMode(SerialDebugEnter);
    _mfcSerialDebugMode(MFC_ChNumBit0Disable);
    _mfcSerialDebugMode(MFC_ChNumBit1Disable);
    _mfcSerialDebugMode(MFC_ChNumBit2Disable);
    _mfcSerialDebugMode(MFC_SetAddrFmt2Byte);
    _mfcSerialDebugMode(MFC_EnableNewCfg);
    _mfcSerialDebugMode(I2cUseBus);
}

static void MFCResetSerialDebugMode(void)
{
    _mfcSerialDebugMode(I2cUseBusN);
    _mfcSerialDebugMode(MFC_ChNumBit0Disable);
    _mfcSerialDebugMode(MFC_ChNumBit1Disable);
    _mfcSerialDebugMode(MFC_ChNumBit2Disable);
    _mfcSerialDebugMode(MFC_SetAddrFmt2Byte);
    _mfcSerialDebugMode(MFC_DisableNewCfg);
    _mfcSerialDebugMode(SerialDebugExit);
}

MS_BOOL DoFastUpgradeProgramToUrsaDRAM(MS_U32 u32DramAddr)
{
    MS_BOOL bRet = FALSE;
    MS_U32 u32Count = 0, u32RestData = 0;
    MS_U32 u32DramRegBank = 0x2B;
    MS_U32 i = 0;


    if (g_u32ISPCodeSize % 0x1000 == 0)
        u32Count = g_u32ISPCodeSize/0x1000;
    else
    {
        u32Count = (g_u32ISPCodeSize/0x1000) + 1;
        u32RestData = g_u32ISPCodeSize - ((u32Count-1)*0x1000);
    }

    MS_U8 tdata[3] = {0};
    MS_U8 pBuf[0x1000] = {0};


    // enter serial debug mode and switch to CH0
    MFCEnterSerialDebugMode();

    for(i = 0; i < (int)u32Count; i++)
    {
        memset(tdata, 0, sizeof(tdata));
        memset(pBuf, 0, sizeof(pBuf));
        MS_U32 u32BaseAddr = 0;
        MS_U8 AlignedAddr[2] = {0};
        u32BaseAddr = (u32DramAddr/0x1000) + i;
        Ursa_SerialFlash_WriteReg(u32DramRegBank, 0xc6, 0x30);
        Ursa_SerialFlash_WriteReg(u32DramRegBank, 0xc7, 0x34);
        Ursa_SerialFlash_WriteReg(u32DramRegBank, 0xca, 0x3c);
        Ursa_SerialFlash_WriteReg(u32DramRegBank, 0xcb, 0x40);
        Ursa_SerialFlash_WriteReg(u32DramRegBank, 0xc4, 0x04);

        AlignedAddr[0] = u32BaseAddr;
        AlignedAddr[1] = u32BaseAddr >> 8;

        Ursa_SerialFlash_WriteReg(u32DramRegBank, 0xcc, AlignedAddr[0]);
        Ursa_SerialFlash_WriteReg(u32DramRegBank, 0xcd, AlignedAddr[1]);
        //Ursa_SerialFlash_WriteReg(u32DramRegBank, 0xce, 0x0);

        tdata[0] = SPI_WRITE;
        tdata[1] = (0xf0)|((u32DramAddr&(0xf00)) >> 8);
        tdata[2] = (u32DramAddr&(0xff));

        // arrange last one block
        if ((u32RestData > 0) && ((MS_U32)i == (u32Count-1)))
            memcpy(pBuf, g_u32ISPSrcAddr + (MS_U32)i*0x1000, u32RestData);
        else
            memcpy(pBuf, g_u32ISPSrcAddr + (MS_U32)i*0x1000, 0x1000);
        UBOOT_DEBUG("tdata: 0x%x, 0x%x, 0x%x\n", tdata[0], tdata[1], tdata[2]);
        UBOOT_DEBUG("pBuf = 0x%x, 0x%x, 0x%x, 0x%x\n", pBuf[0], pBuf[1],pBuf[2],pBuf[3]);

        Ursa_SWI2C_WriteDatas2(tdata, sizeof(tdata), pBuf, sizeof(pBuf));//IIC_CH
        upgrade_percentage = i*100/u32Count;
        UBOOT_DEBUG("Ursa upgrading progress = %d%%\n", upgrade_percentage);
        show_Upgrading(upgrade_percentage, 100);
        mdelay(30);
    }

    //reset serial debug
    MFCResetSerialDebugMode();

    return TRUE;
}

MS_BOOL SetSWUpgradeInit(void)
{
    MS_U8 cmd[7] = {0x05, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00};

    if (m_bFullUpgrade == true) {
        cmd[2] = 0x0;   // All bank
    } else {
        if (m_UrsaBootFlag == 0 || m_UrsaBootFlag == 0xff) {
            cmd[2] = 0x2;   // upgrade AP2
        } else {
            cmd[2] = 0x1;   // upgrade AP1
        }
    }

    cmd[4] = g_u32ISPCodeSize;
    cmd[5] = g_u32ISPCodeSize >> 8;
    cmd[6] = g_u32ISPCodeSize >> 16;
    return MDrv_Ursa_20_Write_Cmd(cmd);
}

MS_BOOL SetSWUPgradeUpdateBootFlag(void)
{
    MS_U8 cmd[7] = {0x05, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00};

    if (m_UrsaFastUpgradeEnable == false)
    {
        printf("[ERROR][%s][%d] m_UrsaFastUpgradeEnable is FALSE\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    if (m_bFullUpgrade == true) {
        cmd[2] = 0x0;   // All bank
    } else {
        if (m_UrsaBootFlag == 0 || m_UrsaBootFlag == 0xff) {
            cmd[2] = 0x2;   // boot from  AP2
        } else {
            cmd[2] = 0x1;   // boot from AP1
        }
    }
    printf("SetSWUPgradeUpdateBootFlag boot flag(0x%x)..\n", cmd[2]);
    return MDrv_Ursa_20_Write_Cmd(cmd);
}

int Ursa_Fast_Upgrade_Program(void)
{
    MS_U32 u32FlashDramAddress = 0;
    MS_U8 retryTimes = 3;
    MS_BOOL bProgramSuccess = false;
    MS_U8 m_UrsaUpgradeAddr[4] = {0};

    // program from SoC dram -> ursa dram -> dram flash with maximum 3 times retry in case data corruption during transfer via I2C
    do
    {
        if (SetSWUpgradeInit() == false)
        {
            printf("FastUpgradeStatusInitFail, retryTimes = %d\n", retryTimes);
            if (retryTimes == 1)
                return 0;
            else
                continue;
        }
        memset(m_UrsaUpgradeAddr, 0, sizeof(m_UrsaUpgradeAddr));
        if(GetSWUpgradeDumpAddr(m_UrsaUpgradeAddr) == false)
        {
            printf("FastUpgradeStatusGetDumpAddrFail, retryTimes = %d\n", retryTimes);
            if (retryTimes == 1)
                return 0;
            else
                continue;
        }


        u32FlashDramAddress = m_UrsaUpgradeAddr[0] + (m_UrsaUpgradeAddr[1] << 8) + (m_UrsaUpgradeAddr[2] << 16) + (m_UrsaUpgradeAddr[3] << 24);
        printf("[%d]%s u32FlashDramAddress = 0x%x \n", __LINE__, __PRETTY_FUNCTION__, u32FlashDramAddress);

        UBOOT_DEBUG("[%d]%s retryTimes = %d \n", __LINE__, __PRETTY_FUNCTION__, retryTimes);

        if (DoFastUpgradeProgramToUrsaDRAM(u32FlashDramAddress) == false)
        {
            printf("FastUpgradeStatusWriteDRAMFail, retryTimes = %d\n", retryTimes);
           if (retryTimes == 1)
                return 0;
            else
                continue;
        }

        // check dram crc32
        if (GetSWUpgradeChkCrc32(CRC32_TYPE_DRAM) == false)
        {
            printf("FastUpgradeStatusChkDRAMFail, retryTimes = %d\n", retryTimes);
            if (retryTimes == 1)
                return 0;
            else
                continue;
        }

        mdelay(30);

        // invoke write data from register to dram
        if (SetSWUpgradeWriteFlash() == false)
        {
            printf("FastUpgradeStatusWriteFlashFail, retryTimes = %d\n", retryTimes);
            if (retryTimes == 1)
                return 0;
            else
                continue;
        }
        printf("wait 30 seconds for URSA to finish the writting\n");

        mdelay(30000);

        // check flash crc32
        if (GetSWUpgradeChkCrc32(CRC32_TYPE_FLASH) == false)
        {
            printf("FastUpgradeStatusChkFLASHFail, retryTimes = %d\n", retryTimes);
            if (retryTimes == 1)
                return 0;
            else
                continue;
        }
        bProgramSuccess = true;
    }while(((--retryTimes) > 0) && (bProgramSuccess == false));

    if (SetSWUPgradeUpdateBootFlag() == false)
    {
        printf("[ERROR][%s][%d] SetSWUPgradeUpdateBootFlag is FALSE\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    upgrade_percentage = 100;

    return TRUE;
}

MS_BOOL remove_prefix_from_path(char *oriPath)
{
    MS_BOOL ret = false;
    char *newPath;

    if (oriPath == NULL)
        ret = false;

    newPath = oriPath;
    if (strncmp(newPath, "/vendor/tvconfig", strlen("/vendor/tvconfig")) == 0)
    {
        newPath = strstr(newPath, "/vendor/tvconfig");
        if (newPath != NULL)
        {
            newPath += strlen("/vendor/tvconfig");
        }
    }
    else if (strncmp(newPath, "/firetv_odm", strlen("/firetv_odm")) == 0)
    {
        newPath = strstr(newPath, "/firetv_odm");
        if (newPath != NULL)
        {
            newPath += strlen("/firetv_odm");
        }
    }

    if (newPath != NULL)
    {
        strncpy(oriPath, newPath, PATH_BUFFER_SIZE);
        ret = true;
    }
    else
    {
        ret = false;
    }

    return ret;
}

int load_ursa_bin_from_eMMC(U32 * const pu32FileSize)
{
    int ret = 0;
    const int device = 0;
    int partition_num = 0;
    char cmd[CMD_BUF] = "\0";
    U32 u32FileSize = 0;
    char * script = NULL;
    char ModelNamePath[PATH_BUFFER_SIZE] = "\0";
    char UrsaBinPath[PATH_BUFFER_SIZE] = URSA_UPGRADE_URSA_BIN_PATH;
    char UrsaBinPath_compress[PATH_BUFFER_SIZE] = "\0";
    MS_BOOL bIsODMPartition = false;

    UBOOT_TRACE("IN\n");
    if(pu32FileSize == NULL)
    {
        UBOOT_ERROR("pu32FileSize == NULL\n");
        goto load_ursa_bin_from_eMMC_end;
    }

    // 1. check where ursa bin file is
    //Get model_name(Customer_XXX.ini) from idme
    if (!idme_get_var_external("model_name", ModelNamePath, PATH_BUFFER_SIZE-1))
    {
        if(strlen(ModelNamePath) > 0)
        {
            //check if Customer_XXX.ini is in odm partition
            if (strncmp(ModelNamePath, "/firetv_odm", 11) == 0)
            {
                // 2. mount partition
                UBOOT_INFO("Mounting %s\n", URSA_UPGRADE_ODM_PARTITION);
                bIsODMPartition = true;
                vfs_mount(URSA_UPGRADE_ODM_PARTITION);
            }
            else
            {
                // 2. mount partition
                UBOOT_INFO("Mounting %s\n", URSA_UPGRADE_TVCONFIG_PARTITION);
                vfs_mount(URSA_UPGRADE_TVCONFIG_PARTITION);
            }
            UBOOT_INFO("idme model_name value --> %s\n", ModelNamePath);
        }
    }

    if (remove_prefix_from_path(ModelNamePath) != true)
    {
        strncpy(ModelNamePath, DEFAULT_CUSTOMER_INI_PATH, PATH_BUFFER_SIZE);
    }

    UBOOT_INFO("Final model name (customer ini) path --> %s\n", ModelNamePath);

    // Get Ursa bin path from Customer_XXX.ini
    script = loadscript(ModelNamePath, &u32FileSize);
    if (NULL != script)
    {
        Profile_Init((char *)script, u32FileSize);
        if(Profile_GetString(URSA_BIN_SECTION_NAME, URSA_BIN_KEY_NAME, URSA_UPGRADE_URSA_BIN_PATH, UrsaBinPath, sizearray(UrsaBinPath)))
        {
            UBOOT_INFO("Get ursa upgrade bin file path %s from %s\n", UrsaBinPath, ModelNamePath);
        }
        else
        {
            UBOOT_INFO("Can't get ursa upgrade bin file path from %s\n", ModelNamePath);
        }
    }
    else
    {
        UBOOT_INFO("Can't get ursa upgrade bin file path from %s\n", ModelNamePath);
    }

    if(remove_prefix_from_path(UrsaBinPath) != true)
    {
        // Use Ursa Default Bin Path
        strncpy(UrsaBinPath, URSA_UPGRADE_URSA_BIN_PATH, PATH_BUFFER_SIZE);
    }

    //Find compress file first
    strcpy (UrsaBinPath_compress,UrsaBinPath);
    strcat (UrsaBinPath_compress,"c");
    u32FileSize = vfs_getsize(UrsaBinPath_compress);
    if (u32FileSize <= 0)
    {
        UBOOT_INFO("file(%s) do not exist or size = 0!, find uncompress one\n", UrsaBinPath_compress);
        u32FileSize = vfs_getsize(UrsaBinPath);
        if (u32FileSize <= 0)
        {
            UBOOT_INFO("file(%s) do not exist or size = 0!, EXIT\n", UrsaBinPath);
            ret = -1;
            goto load_ursa_bin_from_eMMC_unmount;
        }
    }
    else
    {
        memset(UrsaBinPath, 0, sizeof(UrsaBinPath));
        strcpy (UrsaBinPath,UrsaBinPath_compress);
    }
    UBOOT_INFO("Final ursa upgrade bin path --> %s\n", UrsaBinPath);
    UBOOT_INFO("Ursa upgrade bin size = 0x%x\n", u32FileSize);

    // 3. Load ursa bin from eMMC to dram
    if (bIsODMPartition)
    {
        if (-1 == get_mmc_part_number(URSA_UPGRADE_ODM_PARTITION, &partition_num))
        {
            UBOOT_DEBUG("Cannot find parition number of %s\n", URSA_UPGRADE_ODM_PARTITION);
            ret = -1;
            goto load_ursa_bin_from_eMMC_unmount;
        }
    }
    else
    {
        if (-1 == get_mmc_part_number(URSA_UPGRADE_TVCONFIG_PARTITION, &partition_num))
        {
            UBOOT_DEBUG("Cannot find parition number of %s\n", URSA_UPGRADE_TVCONFIG_PARTITION);
            ret = -1;
            goto load_ursa_bin_from_eMMC_unmount;
        }
    }

    snprintf(cmd, CMD_BUF, "ext4load mmc %d:%d %X %s %ul", device, partition_num, UPGRADE_BUFFER_ADDR, UrsaBinPath, u32FileSize);
    UBOOT_DEBUG("cmd=%s\n",cmd);
    if (-1 == run_command(cmd, 0))
    {
       UBOOT_ERROR("Load ursa upgrade bin path = %s failed\n", UrsaBinPath);
       ret = -1;
       goto load_ursa_bin_from_eMMC_unmount;
    }
    *pu32FileSize = u32FileSize;
    UBOOT_INFO("Ursa upgrade bin loaded to 0x%x OK, size = 0x%x\n", UPGRADE_BUFFER_ADDR, u32FileSize);

load_ursa_bin_from_eMMC_unmount:
    vfs_umount();

load_ursa_bin_from_eMMC_end:
    UBOOT_TRACE("OUT\n");

    if(script != NULL)
    {
        free(script);
    }

    return ret;
}

int get_ursa_version_from_bin(const U8 * const pAddr, const U32 u32FileSize, char * const pVersion, const U32 u32VersionSize, MS_BOOL* pcompressed )
{
    int ret = 0;
    U32 offset = 0;
    MS_U32 u32CompressLength = 0;

    UBOOT_TRACE("IN\n");
    if ((pAddr == NULL) || (pVersion == NULL) || (u32FileSize < (URSA_VERSION_MAGIC_OFFSET + 3)) || (pcompressed == NULL))
    {
        UBOOT_ERROR("Wrong parameter, pAddr = %p pVersion = %p u32FileSize = %d pcompressed=%d\n", pAddr, pVersion, u32FileSize, pcompressed);
        ret = -1;
        goto get_ursa_version_from_bin_end;
    }

    // 4. Get version info from binary
    do {
        if ((pAddr[offset]     == URSA_VERSION_MAGIC_0) &&
            (pAddr[offset + 1] == URSA_VERSION_MAGIC_1) &&
            (pAddr[offset + 2] == URSA_VERSION_MAGIC_2) &&
            (pAddr[offset + 3] == URSA_VERSION_MAGIC_3))
        {
            snprintf(pVersion, (u32VersionSize - 1), "%02x%02x%02x",
                pAddr[offset + URSA_VERSION_MAGIC_OFFSET],
                pAddr[offset + URSA_VERSION_MAGIC_OFFSET + 1],
                pAddr[offset + URSA_VERSION_MAGIC_OFFSET + 2]);

            UBOOT_INFO("Ursa version in bin is = %s\n", pVersion);
            u32CompressLength = pAddr[offset+URSA_COMPRESS_LENGTH_OFFSET] + (pAddr[offset+URSA_COMPRESS_LENGTH_OFFSET+1] << 8) + (pAddr[offset+URSA_COMPRESS_LENGTH_OFFSET+2] << 16) + (pAddr[offset+URSA_COMPRESS_LENGTH_OFFSET+3] << 24);
            UBOOT_INFO("u32CompressLength = 0x%x\n", u32CompressLength);
            if (u32CompressLength != 0)
                *pcompressed = TRUE;
            else
                *pcompressed = FALSE;
            goto get_ursa_version_from_bin_end;
        }
        offset++;
    } while(offset < (u32FileSize - URSA_VERSION_MAGIC_OFFSET - 3));
    UBOOT_ERROR("Ursa version pattern not found\n");
    // Pattern not found.
    ret = -1;

get_ursa_version_from_bin_end:
    UBOOT_TRACE("OUT\n");
    return ret;
}

int get_ursa_version_from_ursa(char * pVersion, const U32 u32Size)
{
    int ret = 0;
    U32 u32Version = 0;
    UBOOT_TRACE("IN\n");
    if (pVersion == NULL)
    {
        UBOOT_ERROR("pVersion == NULL\n");
        ret = -1;
        goto get_ursa_version_from_ursa_end;
    }

    u32Version = GetUrsa20VersionInfo();
    if (u32Version == 0)
    {
        UBOOT_ERROR("Get ursa version from ursa failed\n");
        ret = -1;
        goto get_ursa_version_from_ursa_end;
    }
    snprintf(pVersion, u32Size,"%06x", u32Version);
    UBOOT_INFO("Ursa version in ursa is = %s\n", pVersion);

get_ursa_version_from_ursa_end:
    UBOOT_TRACE("OUT\n");
    return ret;
}
int do_ursa_upgrade_mmc_check_update(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    int ret = 0;
    char *pUrsaVersionEnv = NULL;
    char buffer[URSA_VERSION_BUFFER] = {'\0'};
    char currentVersion[URSA_VERSION_BUFFER] = {0};
    char newVersion[URSA_VERSION_BUFFER] = {0};
    MS_BOOL bFlashVersionFound = FALSE;
    MS_BOOL bBinVersionFound = FALSE;
    MS_U8 u8Percent = 0;
    long unsigned  int u32FileSize = 0;
    MS_BOOL bMfcFwUpdated = FALSE;
    MS_BOOL bIsCompressed = FALSE;
    char *pEnvUrsaUpdate = NULL;


    UBOOT_TRACE("IN\n");

    pEnvUrsaUpdate = getenv(FORCE_UPDATE_URSA_ENABLE);

    // 1. Check if memc is supported (by idme)
    if (0 != idme_get_var_external("memc", buffer, URSA_VERSION_BUFFER))
    {
        UBOOT_INFO("idme memc column not found, skip\n");
        goto do_ursa_upgrade_mmc_check_update_exit;
    }
    if (!strncmp(buffer, "0", 1))
    {
        UBOOT_INFO("A non-memc model defined in idme memc, skip\n");
        goto do_ursa_upgrade_mmc_check_update_exit;
    }

    // 2. Case 1: URSA_VERSION_ENV exists (version was checked after OTA)
    pUrsaVersionEnv = getenv(URSA_VERSION_ENV);
    if (pUrsaVersionEnv != NULL)
    {
        if(pEnvUrsaUpdate != NULL && !(strcmp(pEnvUrsaUpdate, "1")))
        {
            UBOOT_INFO("ENV Ursa version found, but env is set for frocing update\n");
        }
        else
        {
            UBOOT_INFO("ENV found %s=%s: no need to update ursa.\n", URSA_VERSION_ENV, pUrsaVersionEnv);
            goto do_ursa_upgrade_mmc_check_update_exit;
        }
    }

    // 3. Case 2: URSA_VERSION_ENV does not exists (ex: after OTA or full flash), try to get from SPI flash
    mdelay(1500);
    if (0 == get_ursa_version_from_ursa(currentVersion, URSA_VERSION_BUFFER))
    {
        UBOOT_INFO("Ursa version from flash found, version = %s\n", currentVersion);
        bFlashVersionFound = TRUE;
    }
    else
    {
        UBOOT_ERROR("Ursa version from flash not found: SPI image may be corrupted\n");
    }

    // 4. Get version info from ursa bin (ex: file not exist)
    if (0 == load_ursa_bin_from_eMMC(&u32FileSize))
    {
        if (0 == get_ursa_version_from_bin(UPGRADE_BUFFER_ADDR, u32FileSize, newVersion, URSA_VERSION_BUFFER,&bIsCompressed))
        {
            UBOOT_INFO("Ursa version from bin found, version = %s\n", newVersion);
            bBinVersionFound = TRUE;
        }
        else
        {
            UBOOT_INFO("Ursa version from bin not found\n");
        }
    }
    else
    {
        UBOOT_INFO("Load ursa bin failed\n");
    }

    if (bBinVersionFound)
    {
        if (bFlashVersionFound)
        {
            if (0 == strncmp(currentVersion, newVersion, URSA_VERSION_BUFFER))
            {
                if(pEnvUrsaUpdate != NULL && !(strcmp(pEnvUrsaUpdate, "1")))
                {
                    UBOOT_INFO("Ursa version are same, but env is set for frocing update\n");
                }
                else
                {
                    // Case A: Both version are same, save env and skip ursa update if version same
                    UBOOT_INFO("Ursa version are same, version = %s\n", currentVersion);
                    goto do_ursa_upgrade_mmc_checkversion_save_version;
                }
            }
        }
        // Case B: Version are different or SPI flash corrupt
        // 5. Start flashing ursa
        UBOOT_INFO("Start Upgrade Ursa!~\n");
#if (ENABLE_MODULE_ANDROID_BOOT == 1 )
        check_boot_mode_from_mtd0_Ursa_Update();
#endif
        show_LoadData(0);
        show_StartUpgrading(0);
        Ursa_ISP_Init(UPGRADE_BUFFER_ADDR, u32FileSize, (MS_U8)E_MFC_FIRMWARE);
        Ursa_SWI2C_Init();
        if(bIsCompressed == TRUE)
        {
            printf("\033[0;32;32m***** FAST Ursa Update ****\033[0m\n");
            SFlashHeader m_sFlashHeader = {0};
            if (IsSupportGetSPIHeaderbyDDC() == TRUE)
            {
                if (GetSWUpgradeSPIHeader(&m_sFlashHeader) == TRUE)
                {
                    if ((m_sFlashHeader.is_double_bin_enable == TRUE) && (m_UrsaFastUpgradeEnable == TRUE))
                    {
                        if (Ursa_Fast_Upgrade_Program())
                        {
                            printf("Usra Fast Upgrading success!!!\n");
                        }
                        else
                        {
                            UBOOT_ERROR("Usra Fast Upgrading fail!!!\n");
                            goto do_ursa_upgrade_mmc_check_update_exit;
                        }
                    }
                    else
                    {
                        UBOOT_ERROR("isDoubleBinEnable FALSE\n");
                        goto do_ursa_upgrade_mmc_check_update_exit;
                    }
                }
                else
                {
                    UBOOT_ERROR("GetSWUpgradeSPIHeader failed!!!\n");
                    goto do_ursa_upgrade_mmc_check_update_exit;
                }
            }
            MDrv_Ursa_20_SetUrsaHardReset();
            mdelay(1000);
        }
        else
        {
            printf("\033[41;4m***** SLOW Ursa Update ****\033[0m\n");
            Ursa_ISP_Program();
            do
            {
                u8Percent = Ursa_ISP_Program();
                UBOOT_INFO("Ursa upgrading progress = %d%%\n", u8Percent);
                show_Upgrading((100 - u8Percent), 100);
            } while ((u8Percent < 0xF0) && (u8Percent != 0));
            if (0 != u8Percent)
            {
                UBOOT_ERROR("Ursa_ISP_Program failed\n");
                show_Error(0);
                ret = -1;
                goto do_ursa_upgrade_mmc_check_update_exit;
            }
        }
        bMfcFwUpdated = TRUE;
        show_Finish(0);
        UBOOT_INFO("Ursa upgrade success\n");
        strncpy(currentVersion, newVersion, URSA_VERSION_BUFFER);
        goto do_ursa_upgrade_mmc_checkversion_save_version;

    }
    else // if bBinVersionFound == false
    {
        UBOOT_INFO("No ursa bin found\n");
        if (bFlashVersionFound)
        {
            // Case C: No ursa update bin found, should keep original version
            goto do_ursa_upgrade_mmc_checkversion_save_version;
        }
        else
        {
            // Case D: SPI corrupt and no ursa bin file exist
            UBOOT_ERROR("Ursa fail and no ursa bin!!!!\n");
            ret = -1;
            goto do_ursa_upgrade_mmc_check_update_exit;
        }
    }

do_ursa_upgrade_mmc_checkversion_save_version:
    UBOOT_INFO("Set the env version = %s\n", currentVersion);
    setenv(FORCE_UPDATE_URSA_ENABLE, "0");
    setenv(URSA_VERSION_ENV, currentVersion);
    saveenv();
    if(bMfcFwUpdated)
    {
        Ursa_20_Upgrade_SysBin();
    }
do_ursa_upgrade_mmc_check_update_exit:
    UBOOT_TRACE("OUT\n");
    return ret;
}


int do_ursa_upgrade(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    char cmd[CMD_BUF] = "\0";
    char *env =NULL;
    int partition = 0;
    int device = 0;
    int ret = 0;
    MS_U8 u8Percent= 0;
    long unsigned  int u32FileSize = 0;
    if (-1 == appInitUsbDisk())
    {
        UBOOT_ERROR("FAIL : can not init usb!! \n");
        return -1;
    }
    ret = check_file_partition(URSA_UPGRADE_BIN_PATH,&device,&partition);
    if(ret == 0)
    {
           UBOOT_DEBUG("has Ursa7 upgrade bin\n");


           UBOOT_DEBUG("Geting file size\n");
           snprintf(cmd, CMD_BUF, "fatfilesize usb %d:%d %s", device,partition, URSA_UPGRADE_BIN_PATH);
           UBOOT_DEBUG("cmd=%s\n",cmd);
           ret = run_command(cmd, 0);
           if(ret!=0)
           {
               UBOOT_ERROR("get script file's size fail\n");
               return -1;
           }
           else
           {
               env=getenv("filesize");
               if(env==NULL)
               {
                   UBOOT_ERROR("get env 'filesize' fail\n");
               }
               u32FileSize = simple_strtol(env, NULL, 16);
               printf("Size 0x%lx \n",u32FileSize);

           }

           snprintf(cmd, CMD_BUF, "fatload usb %d:%d  %X %s %ul",device, partition, UPGRADE_BUFFER_ADDR, URSA_UPGRADE_BIN_PATH,u32FileSize);

           UBOOT_DEBUG("cmd=%s\n",cmd);
           if(-1 == run_command(cmd, 0))
           {
               UBOOT_ERROR("Load Upgrade File fail!\n");
               return -1;
           }
           else
           {
              UBOOT_INFO("Load upgrade file OK!~\n");
              UBOOT_INFO("Start Upgrade Ursa!~\n");
              show_LoadData(0);
              show_StartUpgrading(0);
              Ursa_ISP_Init(UPGRADE_BUFFER_ADDR,u32FileSize, (MS_U8)E_MFC_FIRMWARE);
              Ursa_SWI2C_Init();
              Ursa_ISP_Program();
              do
              {
                  u8Percent = Ursa_ISP_Program();
                  UBOOT_INFO("Ursa upgrading progress = %d%%\n", u8Percent);
                  show_Upgrading((100 - u8Percent), 100);
              }while ((u8Percent < 0xF0) && (u8Percent != 0));

              UBOOT_TRACE("OK\n");
              if(0 == u8Percent)
              {
                  show_Finish(0);
                  return 0;
              }
              show_Error(0);
          }
    }
    else
    {
        UBOOT_DEBUG("no %s in usb disk\n",URSA_UPGRADE_BIN_PATH);
    }
    return -1;
}

#if defined(CONFIG_URSA20_SYSBIN)
int Ursa_20_Upgrade_SysBin()
{
    MS_U16 u32FileSize = 0;
    MS_U32 u32FileAddr = 0;
    MS_U8 u8Percent= 0;
    MS_U32 u32SysBinAddr = 0;
    MS_U8 *pData = NULL;

    u32FileSize = MDrv_Ursa_20_GetMFCSysBinLen();

    if(u32FileSize > BANKSIZE)
    {
        UBOOT_ERROR("the size of system bin shouldn't exceed a flash block size\n");
        return -1;
    }

    pData = (MS_U8*)malloc(sizeof(MS_U8) * SYSBIN_SIZE); // align to 1kB size
    if(pData == NULL)
    {
        UBOOT_ERROR("malloc Failed\n");
        return -1;
    }

    memset(pData, 0, SYSBIN_SIZE);
    MDrv_Ursa_20_GenMFCSysBin(pData);

    // Get sys bin address in Ursa FW
    u32SysBinAddr = MDrv_Ursa_20_GetMFCBinAddress(0x02); // 0x02:System Bin
    UBOOT_INFO("sys bin address in Ursa FW = 0x%lx\n", u32SysBinAddr);
    if((u32SysBinAddr == 0)||(u32SysBinAddr > 0x400000))
    {
        UBOOT_ERROR("system.bin is not exist in MFC SPI flash\n");
        free(pData);
        return -1;
    }

    g_ucBankDst = (MS_U8)(u32SysBinAddr / BANKSIZE); // one MFC bank size : 64KB

    u32FileAddr = (MS_U32)pData;

    UBOOT_INFO("Load Ursa System Bin OK!~\n");
    UBOOT_INFO("Start Upgrade Ursa System Bin!~\n");
    Ursa_ISP_Init(u32FileAddr,u32FileSize, (MS_U8)E_MFC_SYSTEM_BIN);
    Ursa_SWI2C_Init();
    Ursa_ISP_Program();
    do
    {
        u8Percent =  Ursa_ISP_Program();
    }while ((u8Percent < 0xF0) && (u8Percent != 0));

    free(pData);
    UBOOT_TRACE("OK\n");

    if(0 == u8Percent)
    {
        return 0;
    }

    return -1;
}

int do_Ursa_20_Upgrade_SysBin(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    Ursa_20_Upgrade_SysBin();
    return 0;
}

U_BOOT_CMD(Ursa_20_Upgrade_SysBin, CONFIG_SYS_MAXARGS, 1, do_Ursa_20_Upgrade_SysBin,
    "Ursa_20_Upgrade_SysBin\n",    NULL);

#endif

int do_ursa_reset(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
   MS_U16 wErrCounter;
   Ursa_SWI2C_Init();
   for(wErrCounter = 0;wErrCounter<ENTER_ISP_ERROR_MAX;)
   {
       if (!Ursa_EnterIspMode())
       {
           wErrCounter++;
           udelay(10*1000);
           Ursa_ExitIspMode();
           udelay(80*1000);
       }
       else
           break;
   }
   Ursa_ExitIspMode();
   return 0;
}
#endif

#if defined(CONFIG_CMD_WRITE_DATA_TO_RAW_PARTITION_ENABLE) && (CONFIG_CMD_WRITE_DATA_TO_RAW_PARTITION_ENABLE==1)
int do_crc32check(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    MS_U32 u32DataStartAddr=0, u32CrcStartAddr=0, u32CrcCalResult=0, u32DataSize=0;
    char *pStrDataStartAddr=NULL, *pStrCrcStartAddr=NULL, *pStrDataSize=NULL;
    void *pDataBuffer=NULL;
    MS_U32 u32CrcLoadedResult=0;
    U8 *pu8CrcValue=NULL, *pu8DataStartAddr=NULL;

    UBOOT_TRACE("IN\n");
    pStrDataStartAddr = getenv("dataStartAddr");
    pStrCrcStartAddr = getenv("crcStartAddr");
    pStrDataSize = getenv("filesize");

    if ((pStrDataStartAddr == NULL) || (pStrCrcStartAddr == NULL) || (pStrDataSize==NULL))
    {
        UBOOT_ERROR("The environment variables: dataStartAddr, crcStartAddr or filesize is not configured yet. \n");
        return (-1);
    }

    u32DataStartAddr = simple_strtol(pStrDataStartAddr, NULL, 16);
    u32CrcStartAddr = simple_strtol(pStrCrcStartAddr, NULL, 16);
    u32DataSize = simple_strtol(pStrDataSize, NULL, 16);

    UBOOT_DEBUG("u32DataStartAddr=0x%lx, u32CrcStartAddr=0x%lx, u32DataSize=%lu \n", u32DataStartAddr, u32CrcStartAddr, u32DataSize);

    pu8CrcValue = (U8*) (u32CrcStartAddr);
    u32CrcLoadedResult = ((pu8CrcValue[0] << 24) |  (pu8CrcValue[1] << 16) |  (pu8CrcValue[2] << 8) | (pu8CrcValue[3]));
    printf("u32CrcLoadedResult=0x%lx \n", u32CrcLoadedResult);

    pDataBuffer = malloc(u32DataSize);
    if (pDataBuffer == NULL)
    {
        UBOOT_ERROR("pDataBuffer is null. \n");
        return (-1);
    }

    pu8DataStartAddr = ((U8 *) u32DataStartAddr);
    memcpy(pDataBuffer, pu8DataStartAddr, u32DataSize);

    u32CrcCalResult = crc32(u32CrcCalResult, (U8 *)pDataBuffer, u32DataSize);
    printf("u32CrcCalResult=0x%lx \n", u32CrcCalResult);

    if (u32CrcCalResult == u32CrcLoadedResult)
    {
        setenv ("crcTestResult", "1");
    }
    else
    {
        setenv ("crcTestResult", "0");
    }
    free(pDataBuffer);

    UBOOT_TRACE("OUT\n");

    return 0;
}

int do_writeDataToRawPartition(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
#define __ENV_BUF_SIZE__ (128)
    static MS_U32 u32TotalSize=0;
    MS_U32 u32DataStartAddr=0, u32DataSize=0, u32MainIdx=0, u32SubIdx=0, u32BlockSize=0, u32UpgradeBitmap=0;
    char *pStrDataStartAddr=NULL, *pStrDataSize=NULL, *pStrPartName=NULL, *pStrVolName=NULL, *pStrUpgradeBitmap=NULL;
    U8 *pu8DataStartAddr=NULL;
    char strEnvUpgradeBitmap[(__ENV_BUF_SIZE__)]={};
    int s32RtnVal=0;

    UBOOT_TRACE("IN\n");
    setenv("writeResult", "-1");
    /***************************************************************
     *  Check and set the arguments
     ***************************************************************/
    if (argc < 4)
    {
        UBOOT_ERROR("Command Usage: writeDataToRawPartition <main_idx> <sub_idx> <block_size> <partition_name> [volume_name] \n");
        UBOOT_ERROR("main_idx - The installation main_idx-index to the raw partition. \n");
        UBOOT_ERROR("sub_idx - The installation sub-index to the raw partition. \n");
        UBOOT_ERROR("block_size - The block sizeo of the raw partition. \n");
        UBOOT_ERROR("partition_name - The name of raw partition. \n");
        UBOOT_ERROR("volume_name - The volume name of raw partition for NAND flash.\n");
        return (-1);
    }
    UBOOT_DEBUG(" argv[1]=%s,argv[2]=%s,argv[3]=%s,argv[4]=%s, argv[5]=%s\n",argv[1],argv[2],argv[3],argv[4], argv[5]);
    u32MainIdx = simple_strtol(argv[1], NULL, 16);
    u32SubIdx = simple_strtol(argv[2], NULL, 16);
    u32BlockSize = simple_strtol(argv[3], NULL, 16);
    pStrPartName = argv[4];
    pStrVolName  = argv[5];

    UBOOT_DEBUG("u32MainIdx=0x%lx, u32SubIdx=0x%lx, u32BlockSize=0x%lx, pStrPartName=%s, pStrVolName=%s \n",
                 u32MainIdx, u32SubIdx, u32BlockSize, pStrPartName, pStrVolName);
    /***************************************************************
    * Check and set the mboot environment variables.
    ****************************************************************/
    pStrDataStartAddr = getenv("dataStartAddr");
    pStrDataSize = getenv("filesize");
    pStrUpgradeBitmap = getenv("FactoryUpgradeBitmap");

    if ((pStrDataStartAddr==NULL) || (pStrDataSize==NULL) || (pStrPartName==NULL) )
    {
        UBOOT_ERROR("The environment variables: dataStartAddr, filesize or partition_name is not configured yet. \n");
        return (-1);
    }
    UBOOT_DEBUG("pStrDataStartAddr=0x%s, pStrDataSize=0x%s, pStrUpgradeBitmap=%s \n", pStrDataStartAddr, pStrDataSize, pStrUpgradeBitmap);
    u32DataStartAddr = simple_strtol(pStrDataStartAddr, NULL, 16);
    u32DataSize = simple_strtol(pStrDataSize, NULL, 16);
    u32UpgradeBitmap = simple_strtol(pStrUpgradeBitmap, NULL, 16);

    UBOOT_DEBUG("u32DataStartAddr=0x%lx, u32DataSize=0x%lx, u32UpgradeBitmap=0x%lx\n", u32DataStartAddr, u32DataSize, u32UpgradeBitmap);
    if (u32DataSize > u32BlockSize)
    {
        UBOOT_ERROR("u32DataSize(%lu) is greater than u32BlockSize(%lu). \n", u32DataSize, u32BlockSize);
        return (-1);
    }

    /* Reset the total size. */
    if (u32SubIdx == 0)
    {
        u32TotalSize = 0;
    }

    pu8DataStartAddr = ((U8 *) u32DataStartAddr);
    if (pu8DataStartAddr != NULL)
    {
#if CONFIG_MSTAR_ROM_BOOT_WITH_EMMC_FLASH
        if(raw_io_config(E_RAW_DATA_IN_MMC, pStrPartName ,pStrVolName) == 0)
#else
        if(raw_io_config(E_RAW_DATA_IN_NAND, pStrPartName, pStrVolName) == 0)
#endif
        {
            UBOOT_DEBUG("RAW IO config is OK for key transfer.\n");
        }
        else
        {
            UBOOT_ERROR("RAW IO config is NOK for key transfer. \n");
            return -1;
        }

        UBOOT_DEBUG("pu8DataStartAddr=0x%lx \n", pu8DataStartAddr);

        s32RtnVal = raw_write((unsigned int) pu8DataStartAddr, (u32MainIdx * u32BlockSize) + (u32TotalSize), u32DataSize);
        if (s32RtnVal == 0)
        {
            u32TotalSize += u32DataSize;
            u32UpgradeBitmap |= (1<<(u32MainIdx));
            memset(strEnvUpgradeBitmap, 0x0, (sizeof(char)*(__ENV_BUF_SIZE__)));
            sprintf(strEnvUpgradeBitmap, "0x%x", u32UpgradeBitmap);
            UBOOT_DEBUG("u32UpgradeBitmap=0x%lx, strEnvUpgradeBitmap=%s", u32UpgradeBitmap, strEnvUpgradeBitmap);
            setenv("FactoryUpgradeBitmap", strEnvUpgradeBitmap);
            setenv("writeResult", "0");
            saveenv();
        }
        else
        {
            UBOOT_ERROR("Failed to writhe data the raw partition. \n");
        }
    }

    UBOOT_TRACE("OUT\n");
    return 0;
}
#endif

#if defined(CONFIG_INSTALL_CHECK_ENABLE) && (CONFIG_INSTALL_CHECK_ENABLE==1)
int do_installcheck(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    int s32GpioStartInstall=0, s32GpioInstallType=0, s32ReadCnt=0, idx=0;
    char *pStrInstallationDone=NULL;
    unsigned int u32InstallationDone=0;

    UBOOT_TRACE("IN\n");

    s32GpioStartInstall = mdrv_gpio_get_level(GPIO_START_INSTALLATION);
    if (s32GpioStartInstall == 0)
    {
        for(idx = 0; idx < 4; idx++)
        {
            s32GpioStartInstall = mdrv_gpio_get_level(GPIO_START_INSTALLATION);
            if(s32GpioStartInstall == 0)
            {
                s32ReadCnt++;
            }
            MsOS_DelayTask(100); //100ms
        }

        s32GpioInstallType = mdrv_gpio_get_level(GPIO_INSTALLATION_TYPE);
        UBOOT_DEBUG("\n BEFORE START INSTALL KEYS: GPIO_INSTALLATION_TYPE LEVEL = %d \n", mdrv_gpio_get_level(GPIO_INSTALLATION_TYPE));

        if ((s32GpioStartInstall == 0) && (s32ReadCnt > 3) && (s32GpioInstallType == 1))
        {
            if ( -1 == appInitUsbDisk() )
            {
                UBOOT_ERROR("FAIL : can not init usb!! \n");
            }
            else
            {
                printf("Start USB installation................ \n");

                /* Set GPIO GPIO_INSTALLATION_RESULT high to indicate the installation will be started. */
                mdrv_gpio_set_high(GPIO_INSTALLATION_RESULT);

                run_command("setenv installation_done 0", 0);
                mdelay(500);
                run_command("cudstar /usb_scripts/factory_install.txt", 0);

                pStrInstallationDone = getenv("installation_done");
                u32InstallationDone = simple_strtol(pStrInstallationDone, NULL, 16);
                if (u32InstallationDone == 1)
                {
                    /* Set GPIO GPIO_INSTALLATION_RESULT low to indicate the installation is done. */
                    printf("USB installation is done................ \n");
                    mdrv_gpio_set_low(GPIO_INSTALLATION_RESULT);
                    mdelay(4000);
                    run_command("reset", 0);
                }
                else
                {
                    UBOOT_ERROR("\n The installation process is failed!!!!!\n");
                }
            }
        }

        else if ((s32GpioStartInstall == 0) && (s32ReadCnt > 3) && (s32GpioInstallType == 0))
        {
            printf("Start TFTP installation................ \n");

            /* Set GPIO GPIO_INSTALLATION_RESULT high to indicate the installation will be started. */
            mdrv_gpio_set_high(GPIO_INSTALLATION_RESULT);

            run_command("setenv installation_done 0", 0);
            mdelay(500);
            run_command("mstar /tftp_scripts/factory_install.txt", 0);

            pStrInstallationDone = getenv("installation_done");
            u32InstallationDone = simple_strtol(pStrInstallationDone, NULL, 16);
            if (u32InstallationDone == 1)
            {
                /* Set GPIO GPIO_INSTALLATION_RESULT low to indicate the installation is done. */
                printf("TFTP installation is done................ \n");
                mdrv_gpio_set_low(GPIO_INSTALLATION_RESULT);
                mdelay(4000);
                run_command("reset", 0);
            }
            else
            {
                UBOOT_ERROR("\n The installation process is failed!!!!!\n");
            }

        }
    }

    UBOOT_TRACE("OUT\n");
    return 0;
}
#endif

#define ENV_FIRST_BOOT_UP  "first_bootup"

int save_first_boot_status(void)
{
    int ret = 0;
    UBOOT_TRACE("IN\n");

    char *sync_mmap = getenv("sync_mmap");
    char *db_table  = getenv("db_table");

    if ((sync_mmap != NULL) && (strcmp(sync_mmap, "1") == 0)
         && (db_table != NULL) && (strcmp(db_table, "0") == 0))
    {
        UBOOT_DEBUG("It is the first boot up case\n");
        setenv(ENV_FIRST_BOOT_UP, "1");
        saveenv();
        ret = 1;
    }

    UBOOT_TRACE("OK\n");
    return ret;
}


int get_first_boot_status(void)
{
    UBOOT_TRACE("IN\n");
    char *status = getenv(ENV_FIRST_BOOT_UP);
    if ((status != NULL) && (strcmp(status, "1") == 0))
    {
        UBOOT_TRACE("OK\n");
        return 1;
    }
    UBOOT_TRACE("OK\n");
    return 0;
}


void clear_first_boot_status(void)
{
    char cmdBuf[256];
    UBOOT_TRACE("IN\n");

    memset(cmdBuf, 0, 256);
    snprintf(cmdBuf,CMD_BUF,"setenv %s;saveenv", ENV_FIRST_BOOT_UP);
    if (run_command(cmdBuf, 0) != 0)
    {
        UBOOT_ERROR("run command error : %s\n", cmdBuf);
    }

    UBOOT_TRACE("OK\n");
}
