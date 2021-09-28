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

#include <exports.h>
#include <config.h>
#include <CusSystem.h>
#include <uboot_mmap.h>
#include <ShareType.h>
#include <MsVfs.h>
#include <MsUtility.h>
#include <MsApiMiu.h>
#include <MsDebug.h>
#include <MsRawIO.h>
#include <MsEnvironment.h>
#if defined (CONFIG_SET_4K2K_MODE)
#include <apiPNL.h>
#include <apiGOP.h>
#include <MsBoot.h>
#endif
#if defined (CONFIG_URSA_6M40)
#include <ursa/ursa_6m40.h>
#endif
#if defined (CONFIG_URSA_8)
#include <ursa/ursa_8.h>
#endif
#if (ENABLE_MODULE_BOOT_IR == 1)
#include <MsOS.h>
#endif
#include <CusConfig.h>
#if defined (CONFIG_INX_VB1)
#include <panel/panel_INX_vb1.h>
#elif defined (CONFIG_INX_NOVA_VB1)
#include <panel/panel_INX_NOVA_vb1.h>
#endif
#if defined(CONFIG_URSA20_VB1)
#include <ursa/ursa_common.h>
#include <ursa/ursa_20.h>
#endif

#include <drvMIU.h>
#include <apiPNL.h>
#include <MsMmap.h>
#include <MsSystem.h>
#include <mstarstr.h>
#include <bootlogo/MsPoolDB.h>
#include <MsApiPanel.h>

/* whenever there is db/config data structure change, increase this by 1 */
#define MBOOT_DB_VERSION_NAME  "MBOOT_DB_VERSION"
#define MBOOT_DB_VERSION_VALUE "1"

extern int snprintf(char *str, size_t size, const char *fmt, ...);

#if CONFIG_RESCUE_ENV
char *CUS_BOOT_RECOVER_ENV_LIST[] =
{
    NULL
};
#endif

#if defined(CONFIG_MTK_WIFI_7668)
int do_MTK_wifi_reset (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    UBOOT_TRACE("IN\n");
    Wifi_Reset_ON();
    mdelay(2);
    Wifi_Reset_OFF();
    UBOOT_TRACE("OK\n");
    return 0;
}
#endif

#if defined (CONFIG_URSA20_VB1) || defined(CONFIG_URSA_UNION)
int do_MTK_ursa_reset (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    UBOOT_TRACE("IN\n");
    MDrv_Ursa_20_SetUrsaHardReset();
    UBOOT_TRACE("OK\n");
    return 0;
}
#endif

#if defined(CONFIG_ENABLE_MIU_SSC)
int do_MIU_SSC (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    UBOOT_TRACE("IN\n");
    MDrv_MIU_SetSscValue(E_MIU_0,(MS_U16)MIU_SSC_MODULATION, (MS_U16)MIU_SSC_DEVIATION,(MS_BOOL)MIU0_SSC_ENABLE);
    MDrv_MIU_SetSscValue(E_MIU_1,(MS_U16)MIU_SSC_MODULATION, (MS_U16)MIU_SSC_DEVIATION,(MS_BOOL)MIU0_SSC_ENABLE);
    MDrv_MIU_SetSscValue(E_MIU_2,(MS_U16)MIU_SSC_MODULATION, (MS_U16)MIU_SSC_DEVIATION,(MS_BOOL)MIU0_SSC_ENABLE);
    UBOOT_TRACE("OK\n");
    return 0;
}
#endif


int do_envload (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    uchar bReloadEnv = 0;
    char *envbuffer;
    envbuffer=NULL;

#if defined(CONFIG_ENV_IS_IN_SPIFLASH)
    extern int env_isempty(void);
    if(env_isempty())
    {
        printf("\n <env reload for CRC>\n");
        bReloadEnv = 1;
    }
#endif

#if defined(CONFIG_LOAD_ENV_FROM_SN) || defined(CONFIG_Customer)
    envbuffer = getenv ("mboot_default_env");
    if(envbuffer)
    {
        if(simple_strtoul (envbuffer, NULL, 10))
        {
            printf("\n <env reload for CHECK_IF_MBOOT_DEFAULT_ENV>\n");
            bReloadEnv = 1;
        }
    }

    if (getenv("bootargs") == NULL)
    {
        UBOOT_DEBUG("get env configs from SN\n");
        bReloadEnv = 1;
    }
#if (ENABLE_MODULE_ANDROID_BOOT == 1 )
    else if(getenv("recoverycmd") == NULL)
    {
        UBOOT_DEBUG("get env configs from SN\n");
        /* ACOS_MOD_BEGIN {amazon_common_kernel_signing_scheme} */
        /* amazon kernel signing: recoverycmd not used */
        // bReloadEnv = 1;
        /* ACOS_MOD_END {amazon_common_kernel_signing_scheme} */
    }
#endif
#endif

// check INIT_MBOOT_ENV bit in idme dev_flags
#if defined(UFBL_FEATURE_IDME)
    if (!bReloadEnv) {
        char* dev_flags_str = NULL;
        unsigned long dev_flags = 0;
        char dev_flags_buf[8] = {0};
        if ((dev_flags_str = getenv("dev_flags")) != NULL)
            dev_flags = simple_strtoul(dev_flags_str, NULL, 16);

        if ((dev_flags & DEV_FLAGS_INIT_MBOOT_ENV) == DEV_FLAGS_INIT_MBOOT_ENV) {
            UBOOT_DEBUG("reload env based on idme dev_flags\n");
            bReloadEnv = 1;
            dev_flags &= ~DEV_FLAGS_INIT_MBOOT_ENV; // clear the bit
            snprintf(dev_flags_buf, sizeof(dev_flags_buf), "%x", dev_flags);
            if (idme_update_var_ex("dev_flags", dev_flags_buf, sizeof(dev_flags_buf)))
            {
               printf("error updating dev_flags\n");
            }
        }
    }
#endif

    if (!bReloadEnv)
    {
        envbuffer = getenv (MBOOT_DB_VERSION_NAME);
        if((!envbuffer) || (strcmp(envbuffer, MBOOT_DB_VERSION_VALUE)))
        {
#if defined(CONFIG_AN_FASTBOOT_ENABLE)
            // for fastboot reboot-bootloader command
            char *sFastboot = NULL;
            sFastboot = getenv("reboot-bootloader");
            if (!(sFastboot) || (sFastboot[0] != '1')) {
                printf("\n <env reload for correct db version:%s\n",
                       MBOOT_DB_VERSION_VALUE);
                bReloadEnv = 1;
            }
#else
            printf("\n <env reload for correct db version:%s\n",
                   MBOOT_DB_VERSION_VALUE);
            bReloadEnv = 1;
#endif
        }
    }


    if(bReloadEnv)
    {
        char cmd[128]="\0";
        // load env from /system/etc/set_env
        snprintf(cmd,sizeof(cmd)-1,"loadenv %s %s", SET_ENV_PATITION, SET_ENV_FILE);
        run_command(cmd,0);
    }
#if (ENABLE_MODULE_ANDROID_BOOT == 1 )
    else
    {
        // reload env after reboot in recovery mode.
        extern int get_boot_status_from_mtd0(void);
        get_boot_status_from_mtd0();
    }
#endif

     return 0;
}

#if defined (CONFIG_MBOOT_VERSION)
int do_setmbootver (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]){
    char *s;
    if((s = getenv (USB_MBOOT_VERSION))!= NULL)
    {
        if(strcmp(s,USB_MBOOT_CURRENT_VERSION))
        {
            setenv(USB_MBOOT_VERSION, USB_MBOOT_CURRENT_VERSION);
            saveenv();
        }
    }
    else
    {
        setenv(USB_MBOOT_VERSION, USB_MBOOT_CURRENT_VERSION);
        saveenv();
    }
    return 0;
}
#endif



#define BOOT_IF_ACTION(c, b)                ((c) & (b))

int get_boot_status_from_mtd0(void)
{
    int ret = 0;
    struct bootloader_message *p_msg = NULL;
    char cmd[128];

    p_msg = (struct bootloader_message*)malloc(sizeof(struct bootloader_message));
    if(NULL == p_msg)
    {
        UBOOT_ERROR("malloc bootloader_message buffer fail!\n");
        return -1;
    }

    memset(cmd, 0, sizeof(cmd));
    memset(p_msg, 0, sizeof(struct bootloader_message));

#if (ENABLE_MODULE_MMC == 1)
    snprintf(cmd, sizeof(cmd)-1, "mmc read.p 0x%08lX misc 40", (unsigned long)p_msg);
#else
    #if !CONFIG_MSTAR_FTL
    snprintf(cmd, sizeof(cmd)-1, "nand read.e 0x%08lX misc 40", (unsigned long)p_msg);
    #else
    snprintf(cmd, sizeof(cmd)-1, "ftl read.p 0x%08lX misc 40", (unsigned long)p_msg);
    #endif
#endif
    if(-1 != run_command(cmd, 0))
    {
        UBOOT_DEBUG("read the misc partion data\n");
        memset(cmd, 0, sizeof(cmd));
        if ((strlen(p_msg->command) == strlen(BOOT_STATUS_CUSTOMER_ACTIONS_STR)) && (0==strncmp(p_msg->command, BOOT_STATUS_CUSTOMER_ACTIONS_STR, strlen(BOOT_STATUS_CUSTOMER_ACTIONS_STR))))
        {
            int reloadEnv   = 0;
            int reloadPanel = 0;
            char actionByte = p_msg->status[0];
            if (BOOT_IF_ACTION(actionByte, BOOT_STATUS_ACTION_RELOADPANEL_BIT))
            {
                reloadPanel = 1;
            }
            if (BOOT_IF_ACTION(actionByte, BOOT_STATUS_ACTION_RELOADENV_BIT))
            {
                reloadEnv = 1;
            }

            memset(p_msg->command, 0, sizeof(p_msg->command));
            memset(p_msg->status, 0, sizeof(p_msg->status));
            memset(cmd, 0, sizeof(cmd));
#if (ENABLE_MODULE_MMC == 1)
            snprintf(cmd, sizeof(cmd)-1, "mmc write.p 0x%08lX misc 40", (unsigned long)p_msg);
#else
            #if !CONFIG_MSTAR_FTL
            snprintf(cmd, sizeof(cmd)-1, "nand write.e 0x%08lX misc 40", (unsigned long)p_msg);
            #else
            snprintf(cmd, sizeof(cmd)-1, "ftl write.p 0x%08lX misc 40", (unsigned long)p_msg);
            #endif
#endif
            run_command(cmd, 0);

            if (reloadPanel)
            {
                setenv("db_table","0");
                saveenv();
            }

            if (reloadEnv)
            {
                // reload env
                char cmd[128]="\0";
                snprintf(cmd,sizeof(cmd)-1,"loadenv %s %s", SET_ENV_PATITION, SET_ENV_FILE);
                run_command(cmd,0);
            }
        }
    }
    else
    {
        UBOOT_ERROR("read misc partition data failed\n");
        ret = -1;
    }

    free(p_msg);
    return ret;
}


int do_initenv(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{

    UBOOT_TRACE("IN\n");

    run_command("mmc erase.p MPOOL", 0);
    run_command("cleanallenv", 0);
    setenv("bootargs", "init=/init CORE_DUMP_PATH=/data/core_dump.%%p.gz KDebug=1 delaylogo=true platform=sn SD_CONFIG=2 tee_mode=optee pm_path=/tvconfig/config/PM.bin loop.max_part=7");
    setenv("bootlogo_gopidx","2");
    setenv("GOP_SET_MUX", "1:0:2:3");
    //setenv("bootlogo_buffer", "E_MMAP_ID_BOOTLOGO_BUFFER");
    //setenv("OSD_BufferAddr", "E_MMAP_ID_JPD_WRITE_ADR");
    setenv("str_crc", "2");
    setenv("db_table", "0");
    setenv("verify", "n");
    setenv("WDT_ENABLE", "1");
    setenv("WDT_TIMEOUT", "100");
    setenv("sync_mmap", "1");
    setenv("CONFIG_PATH", "/vendor/tvconfig/config");
    //setenv("mboot_default_env", "0"); // move behind
    setenv("MAP_TYPE", "MI");
    setenv("MI_MAP_PARTITION", "tvconfig");
    setenv("MI_MAP_PATH", "config/MMAP_MI.h");
    setenv("customer_ini_path", "/tvconfig/config/model/Customer_1.ini");
    setenv("_BootlogoFile", "/tvconfig/bootlogo.jpg");
    setenv("unlock_cmi", "1");
    setenv("kernel_loglevel", "6");
    setenv("bt_usb_port", "1");
    setenv(MBOOT_DB_VERSION_NAME, MBOOT_DB_VERSION_VALUE);

    run_command("dbtable_init 1", 0);
    run_command("sync_mmap", 0);

    //Prevent HD_panel() going wrong flow when AC off while initenv
    setenv("mboot_default_env", "0");
    saveenv();

#if defined (CONFIG_URSA20_VB1) || defined(CONFIG_URSA_UNION)
    run_command("ursa_upgrade_mmc_check_update", 0);
#endif

    UBOOT_TRACE("OK\n");

    return 0;
}

int do_loadenv(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    UBOOT_TRACE("IN\n");

    run_command("initenv", 0);
    run_command("reset",0);

#if 0 // run initenv command instead of loading from emmc
    if(argc < 3)
    {
        cmd_usage(cmdtp);
        return -1;
    }

    if(argv[1] == NULL)
    {
        cmd_usage(cmdtp);
        return -1;
    }

    if(argv[2] == NULL)
    {
        cmd_usage(cmdtp);
        return -1;
    }
    UBOOT_DEBUG("load env from partition -> %s path -> %s\n",argv[1],argv[2]);
#if ENABLE_MODULE_LOAD_ENV_FROM_SN
    vfs_mount(argv[1]);
    runscript_linebyline(argv[2]);
    vfs_umount();
#endif

#endif // run initenv command instead of loading from emmc
    UBOOT_TRACE("OK\n");

    return 0;

}

#if defined (CONFIG_SET_4K2K_MODE)
extern U8 g_logoGopIdx;

MS_BOOL __Sc_is_interlace(void)
{
    return 0;
}

MS_U16 __Sc_get_h_cap_start(void)
{
    return 0x60;
}

void __Sys_PQ_ReduceBW_ForOSD(MS_U8 PqWin, MS_BOOL bOSD_On)
{

}

int do_setFRC(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    GOP_InitInfo pGopInit;

    if(get_poweroff_flag())
    {
        memset((void *)&pGopInit, 0, sizeof(pGopInit));
        MApi_GOP_RegisterXCIsInterlaceCB(__Sc_is_interlace);
        MApi_GOP_RegisterXCGetCapHStartCB(__Sc_get_h_cap_start);
        MApi_GOP_RegisterXCReduceBWForOSDCB(__Sys_PQ_ReduceBW_ForOSD);
        pGopInit.u16PanelWidth = g_IPanel.Width();
        pGopInit.u16PanelHeight = g_IPanel.Height();
        pGopInit.u16PanelHStr = g_IPanel.HStart();
        pGopInit.u32GOPRBAdr = 0x0;
        pGopInit.u32GOPRBLen = 0x0;

        pGopInit.u32GOPRegdmaAdr = 0;
        pGopInit.u32GOPRegdmaLen = 0;
        pGopInit.bEnableVsyncIntFlip = FALSE;

        MApi_GOP_InitByGOP(&pGopInit, g_logoGopIdx);
        MApi_GOP_GWIN_SetGOPDst(g_logoGopIdx, E_GOP_DST_FRC);
    }
    return 0;
}

int Set_4K2K_OP0(void)
{
    GOP_InitInfo pGopInit;

    //close lvds
    MDrv_Ursa_6M40_Set_Lvds_Off();

    //disable osd
    MDrv_Ursa_6M40_Set_Osd_Off();

    //set OP0
    memset((void *)&pGopInit, 0, sizeof(pGopInit));
    MApi_GOP_RegisterXCIsInterlaceCB(__Sc_is_interlace);
    MApi_GOP_RegisterXCGetCapHStartCB(__Sc_get_h_cap_start);
    MApi_GOP_RegisterXCReduceBWForOSDCB(__Sys_PQ_ReduceBW_ForOSD);
    pGopInit.u16PanelWidth = g_IPanel.Width();
    pGopInit.u16PanelHeight = g_IPanel.Height();
    pGopInit.u16PanelHStr = g_IPanel.HStart();
    pGopInit.u32GOPRBAdr = 0x0;
    pGopInit.u32GOPRBLen = 0x0;

    pGopInit.u32GOPRegdmaAdr = 0;
    pGopInit.u32GOPRegdmaLen = 0;
    pGopInit.bEnableVsyncIntFlip = FALSE;

    MApi_GOP_InitByGOP(&pGopInit, g_logoGopIdx);
    MApi_GOP_GWIN_SetGOPDst(g_logoGopIdx, E_GOP_DST_OP0);

    //open lvds
    MDrv_Ursa_6M40_Set_Lvds_On();

    return 0;
}
#endif

#if defined (CONFIG_URSA_6M40)
int do_ursa_lvds_on (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    MS_BOOL bRet = FALSE;
    bRet = MDrv_Ursa_6M40_Set_Lvds_On();
    return 0;
}
int do_ursa_lvds_off(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    MS_BOOL bRet = FALSE;
    bRet = MDrv_Ursa_6M40_Set_Lvds_Off();
    return 0;
}

int do_ursa_osd_unmute (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    MS_BOOL bRet = FALSE;
    bRet = MDrv_Ursa_6M40_Set_Osd_Unmute();
    return 0;
}
int do_ursa_2k_mode_on (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    MS_BOOL bRet = FALSE;
    bRet = MDrv_Ursa_6M40_Set_2K_Mode_On();
    return 0;
}

int do_ursa_set_osd_mode(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    u16 protect_mode =0;
    int ret =0;

    protect_mode = g_UrsaCMDGenSetting.g_OsdMode.protect_mode;
    if(protect_mode>=0 && protect_mode < 3)
    {

        printf("osd_protect_mode = %d\n",protect_mode);
            switch(protect_mode)
                {
                case URSA_6M40_OSD_PROTECT_OFF:
                    {
                        MDrv_Ursa_6M40_SendCmd(CMD_6M40_OSD_PROTECT_OFF);
                    }
                    break;
                case URSA_6M40_OSD_PROTECT_ON:
                    {
                        MDrv_Ursa_6M40_SendCmd(CMD_6M40_OSD_PROTECT_ON);
                    }
                    break;
                case URSA_6M40_OSD_PROTECT_ON_EMMC:
                    {
                        MDrv_Ursa_6M40_SendCmd(CMD_6M40_OSD_PROTECT_ON_EMMC);
                    }
                    break;
                default:
                         break;
                }
        }
        else
		{
            UBOOT_ERROR("OSD protect Mode setting not correct\n");
            ret = -1;
        }
    return ret;
}

#endif

#if defined (CONFIG_ENABLE_4K2K_PANEL)
int do_inx_panel_set_init (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
#if defined (CONFIG_INX_VB1)
    MDrv_Panel_INX_VB1_Set_Pre_Init();
#elif defined(CONFIG_INX_NOVA_VB1)
    MDrv_Panel_INX_NOVA_VB1_Unlock_AHB();
#endif
    return 0;
}

int do_inx_panel_set_fhd (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
#if defined (CONFIG_INX_VB1)
    MDrv_Panel_INX_VB1_Set_FHD();
#elif defined (CONFIG_INX_LVDS)
    MDrv_PANEL_INX_LVDS_Set_FHD();
#endif
    return 0;
}

int do_inx_panel_set_4k2k (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
#if defined (CONFIG_INX_VB1)
    MDrv_Panel_INX_VB1_Set_4K2K();
#elif defined (CONFIG_INX_LVDS)
    MDrv_PANEL_INX_LVDS_Set_4K2K();
#elif defined (CONFIG_INX_NOVA_VB1)
    MDrv_Panel_INX_NOVA_VB1_Set_UHD_DIVISION(UHD_2_DIVISION);
#endif
    return 0;
}

#if defined (CONFIG_INX_NOVA_VB1)
int do_inx_nova_set_4k2k_2division (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    MDrv_Panel_INX_NOVA_VB1_Set_UHD_DIVISION(UHD_2_DIVISION);
    return 0;
}
#endif

#if defined (CONFIG_INX_VB1)
int do_panel_inx_vb1_init (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
#if defined(CONFIG_ENABLE_4K2K_PANEL)
    MDrv_Panel_INX_VB1_RX_INIT();
#endif
    return 0;
}
#endif
#endif

#if defined (CONFIG_URSA_8)
int do_ursa8_lvds_on (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    MS_BOOL bRet = FALSE;
    bRet = MDrv_Ursa_8_Set_Lvds_On();
    return 0;
}


int do_ursa8_set_osd_mode(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    u16 protect_mode =0;
    int ret =0;

    protect_mode = g_UrsaCMDGenSetting.g_OsdMode.protect_mode;
    if(protect_mode>=0 && protect_mode < 3)
    {

        printf("osd_protect_mode = %d\n",protect_mode);
            switch(protect_mode)
                {
                case URSA_8_OSD_PROTECT_OFF:
                    {
                        MDrv_Ursa_8_SendCmd(URSA_8_CMD_OSD_PROTECT_OFF);
                    }
                    break;
                case URSA_8_OSD_PROTECT_ON:
                    {
                        MDrv_Ursa_8_SendCmd(URSA_8_CMD_OSD_PROTECT_ON);
                    }
                    break;
                case URSA_8_OSD_PROTECT_ON_EMMC:
                    {
                        MDrv_Ursa_8_SendCmd(URSA_8_CMD_OSD_PROTECT_ON_EMMC);
                    }
                    break;
                default:
                         break;
                }
        }
        else{
            UBOOT_ERROR("OSD protect Mode setting not correct\n");
            ret = -1;
            }


    return ret;

}

#endif
#if (ENABLE_MODULE_BOOT_IR == 1)
int do_ir_delay(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    char *pEnv = getenv("ir_delay");
    int delay_time = 0;

    if (pEnv != NULL)
    {
        delay_time = (int)simple_strtol(pEnv, NULL, 10);
        if (delay_time <= 0)
        {
            printf("ir_delay = %d, Skip ir delay, for performance\n", delay_time);
        }
        else
        {
            mdelay(delay_time); //sleep
        }
    }

    return 0;
}
#endif

#if(CONFIG_ENABLE_V_I_PWM == 1)
#define BUFFER_SIZE 128
int do_check_safemode(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
   U8 eep_safemode = 0xff;
   MS_BOOL change_flag = FALSE;
    char str[BUFFER_SIZE];
   char * env_safemode = getenv("safemodeset");
   printf("---wya--- do_check_safemode\n");

   memset(str, 0 , BUFFER_SIZE);
   snprintf(str, BUFFER_SIZE, "eeprom read 0x%x 0x001E 1", (MS_PHYADDR)&eep_safemode);
   run_command(str, 0);

   printf("---wya--- env_safemode =%s\n",env_safemode);
   printf("---wya--- eep_safemode =%d\n",eep_safemode);

   if(env_safemode==NULL && eep_safemode==0xff)
   {
       printf("---wya--- smt progress!! set safemode to off \n");
       setenv("safemodeset","off");
       eep_safemode = 0;
       memset(str, 0 , BUFFER_SIZE);
       snprintf(str, BUFFER_SIZE, "eeprom write 0x%x 0x001E 1", (MS_PHYADDR)&eep_safemode);
       run_command(str, 0);
       return 1;
   }
   if(strcmp(env_safemode, "on")!= 0 && strcmp(env_safemode, "off")!= 0)
   {
       printf("---wya--- other case,we need to set safemode on \n");
       setenv("safemodeset","on");
       eep_safemode = 1;
       memset(str, 0 , BUFFER_SIZE);
       snprintf(str, BUFFER_SIZE, "eeprom write 0x%x 0x001E 1", (MS_PHYADDR)&eep_safemode);
       run_command(str, 0);
       return 1;
   }


   printf("---wya--- before change eep_safemode =%d , env_safemode=%s\n",eep_safemode,env_safemode);
   if(strcmp(env_safemode, "on")== 0 && eep_safemode!= 1)
   {
       eep_safemode = 1;
       memset(str, 0 , BUFFER_SIZE);
       snprintf(str, BUFFER_SIZE, "eeprom write 0x%x 0x001E 1", (MS_PHYADDR)&eep_safemode);
       run_command(str, 0);
       //printf("---wya--- run eeprom wb 0x001E 1 \n");
       //run_command("eeprom wb 0x001E 1", 0);
       change_flag = TRUE;
   }
   if(strcmp(env_safemode, "off")== 0 && eep_safemode!= 0)
   {
       eep_safemode = 0;

       memset(str, 0 , BUFFER_SIZE);
       snprintf(str, BUFFER_SIZE, "eeprom write 0x%x 0x001E 1", (MS_PHYADDR)&eep_safemode);
       run_command(str, 0);
       //printf("---wya--- run eeprom wb 0x001E 0 \n");
       //run_command("eeprom wb 0x001E 0", 0);
       change_flag = TRUE;
   }
   if(change_flag == TRUE)
   {
       memset(str, 0 , BUFFER_SIZE);
       snprintf(str, BUFFER_SIZE, "eeprom read 0x%x 0x001E 1", (MS_PHYADDR)&eep_safemode);
       run_command(str, 0);
       printf("---wya--- after eep_safemode =%d , env_safemode=%s\n",eep_safemode,env_safemode);
       return 1;
   }
   return 0;
}

int do_safemode(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
   int ret = 0;
    char str[BUFFER_SIZE];
   int eep_safemode = -1;
   if (argc < 2)
       return cmd_usage(cmdtp);
   if (strcmp(argv[1], "get") == 0) {
       char * val = getenv("safemodeset");
       printf("---wya--- safemode get =%s\n",val);
   }else if(strcmp(argv[1], "set") == 0) {
       printf("---wya--- safemode get arg[2]=%s\n",argv[2]);
       if(strcmp(argv[2], "off") == 0)
       {
           eep_safemode = 0;
           setenv("safemodeset","off");
           saveenv();
       }
       else
       {
           eep_safemode = 1;
           setenv("safemodeset","on");
           saveenv();
       }

       memset(str, 0 , BUFFER_SIZE);
       snprintf(str, BUFFER_SIZE, "eeprom write 0x%x 0x001E 1", (MS_PHYADDR)&eep_safemode);
       run_command(str, 0);
       //if(eep_safemode==1)
       //  run_command("eeprom wb 0x001E 1", 0);
       //else
       //  run_command("eeprom wb 0x001E 0", 0);

       memset(str, 0 , BUFFER_SIZE);
       snprintf(str, BUFFER_SIZE, "eeprom read 0x%x 0x001E 1", (MS_PHYADDR)&eep_safemode);
       run_command(str, 0);
       printf("---wya--- eeprom read index=%d\n",eep_safemode);
       run_command("reset",0);
   }
    return ret;
}
#endif

#if (CONFIG_URSA12_VB1_FRC_BRINGUP)//alex_tung###

//################################
#include <MsSystem.h>
#include <MsSysUtility.h>
#include "drvCPU.h"
#include "drvMBX.h"
//################################
#define FRCR2_MBX_QUEUESIZE     8
#define FRCR2_MBX_TIMEOUT       5000
//################################
static MS_BOOL FRCR2_MBX_Init(void)
{
    MBX_CPU_ID eHKCPU;
    MS_U32 u32TimeoutMillSecs = 1500;

    MDrv_MBX_SetDbgLevel(MBX_DBG_LEVEL_ALL);

    //Initialize Mailbox
    eHKCPU = E_MBX_CPU_MIPS;
    if( E_MBX_SUCCESS != MDrv_MBX_Init(eHKCPU, E_MBX_ROLE_HK, u32TimeoutMillSecs))
    {
        printf("[HKCPU] FRCR2_MBX_Init - Fail !!!!\n");
        while(1);
    }
    else
    {
        MDrv_MBX_Enable(TRUE);
        //MDrv_MBX_SetCallDrvFlag(TRUE);
    }

    //Register class for R2
    if( E_MBX_SUCCESS != MDrv_MBX_RegisterMSG(E_MBX_CLASS_FRC, FRCR2_MBX_QUEUESIZE))
    {
        printf("[HKCPU] FRCR2_MBX_Init - register msg error !!!\n");
        return FALSE;
    }

    return TRUE;
}

static MS_BOOL FRCR2_MBX_SendMsg(U8 u8CmdIdx,U32 u32Data)
{
    MBX_Msg stMB_Command;
    MBX_Result enMbxResult = 0;

    //Write2Byte(0x101EA6,0xFFF2);//alex_tung###

    memset((void*)&stMB_Command, 0x00, sizeof(MBX_Msg));
    //(1) send to CB
    stMB_Command.eRoleID = E_MBX_ROLE_FRC;
    stMB_Command.eMsgType = E_MBX_MSG_TYPE_INSTANT;
    stMB_Command.u8Ctrl = 0;
    stMB_Command.u8MsgClass = E_MBX_CLASS_FRC;
    stMB_Command.u8Index = u8CmdIdx;
    stMB_Command.u8ParameterCount = 4;
    stMB_Command.u8Parameters[0] =  (U8)((u32Data>>24)&0xFF);
    stMB_Command.u8Parameters[1] =  (U8)((u32Data>>16)&0xFF);
    stMB_Command.u8Parameters[2] =  (U8)((u32Data>>8)&0xFF);
    stMB_Command.u8Parameters[3] =  (U8)((u32Data>>0)&0xFF);
    enMbxResult = MDrv_MBX_SendMsg(&stMB_Command);

    return (enMbxResult==E_MBX_SUCCESS)? TRUE : FALSE;
}

static MS_BOOL FRCR2_MBX_RecvMsg(void)
{
    MBX_Msg stMB_Command;
    MBX_Result enMbxResult = 0;

    Write2Byte(0x101EA6,0xFFF4);//alex_tung###
    do
    {
        memset((void*)&stMB_Command, 0x00, sizeof(MBX_Msg));
        enMbxResult = MDrv_MBX_RecvMsg(E_MBX_CLASS_FRC, &stMB_Command, FRCR2_MBX_TIMEOUT, MBX_CHECK_INSTANT_MSG);
    }while((enMbxResult  !=  E_MBX_SUCCESS) && (enMbxResult  !=  E_MBX_ERR_TIME_OUT));


    //(3) check result
    if(enMbxResult == E_MBX_ERR_TIME_OUT)
    {
        printf("[HK] do_frc_send : FAIL to receive message from FRCR2\n");
    }
    else
    {
        printf("[HK] do_frc_send : PASS to receive from FRCR2\n");
        printf("\n[%s] : %s =========> (%d): stMB_Command.eRoleID   =%d\n",__FILE__,__FUNCTION__,__LINE__,stMB_Command.eRoleID);//alex_tung###
        printf("\n[%s] : %s =========> (%d): stMB_Command.eMsgType  =%d\n",__FILE__,__FUNCTION__,__LINE__,stMB_Command.eMsgType);//alex_tung###
        printf("\n[%s] : %s =========> (%d): stMB_Command.u8Ctrl    =0x%02X\n",__FILE__,__FUNCTION__,__LINE__,stMB_Command.u8Ctrl);//alex_tung###
        printf("\n[%s] : %s =========> (%d): stMB_Command.u8MsgClass=%d\n",__FILE__,__FUNCTION__,__LINE__,stMB_Command.u8MsgClass);//alex_tung###
        printf("\n[%s] : %s =========> (%d): stMB_Command.u8Index   =%d\n",__FILE__,__FUNCTION__,__LINE__,stMB_Command.u8Index);//alex_tung###
        printf("\n[%s] : %s =========> (%d): stMB_Command.u8ParameterCount=%d\n",__FILE__,__FUNCTION__,__LINE__,stMB_Command.u8ParameterCount );//alex_tung###
        printf("\n[%s] : %s =========> (%d): stMB_Command.u8Parameters[0]=0x%02X\n",__FILE__,__FUNCTION__,__LINE__,stMB_Command.u8Parameters[0] );//alex_tung###
        printf("\n[%s] : %s =========> (%d): stMB_Command.u8Parameters[1]=0x%02X\n",__FILE__,__FUNCTION__,__LINE__,stMB_Command.u8Parameters[1] );//alex_tung###
        printf("\n[%s] : %s =========> (%d): stMB_Command.u8Parameters[2]=0x%02X\n",__FILE__,__FUNCTION__,__LINE__,stMB_Command.u8Parameters[2] );//alex_tung###
        printf("\n[%s] : %s =========> (%d): stMB_Command.u8Parameters[3]=0x%02X\n",__FILE__,__FUNCTION__,__LINE__,stMB_Command.u8Parameters[3] );//alex_tung###
    }
    return (enMbxResult==E_MBX_SUCCESS)? TRUE : FALSE;

}

//#################################################################
int do_frc_bringup(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    char cmd[128];
    MS_U8 u8MIUProtectkernel_ID[16] ={0};

    MS_PHYADDR FRCR2BootAddr;
    U32 u32FRCR2BootLen;

    if (get_map_addr_from_env(E_FRC_R2, NO_DEFAULT_MMAP_VALUE, &FRCR2BootAddr) != 0)
    {
        UBOOT_ERROR("get E_MMAP_ID_FRC_R2 mmap fail\n");
        return -1;
    }
    UBOOT_DEBUG("E_MMAP_ID_FRC_R2 = 0x%llx\n", FRCR2BootAddr);
    UBOOT_DEBUG("(U32)(PA2NVA(u32PanelConfigsAddr)) = 0x%x\n", (U32)(PA2NVA(FRCR2BootAddr)));

    memset(cmd, 0, sizeof(cmd));
#if (CONFIG_MBOOT_IN_NAND_FLASH)
    snprintf(cmd, sizeof(cmd)-1, "nand read.e 0x%08lX frc 0x100000", (U32)(PA2NVA(FRCR2BootAddr)));
#elif (CONFIG_MBOOT_IN_MMC_FLASH)
    snprintf(cmd, sizeof(cmd)-1, "mmc read.p 0x%08lX frc 0x100000", (U32)(PA2NVA(FRCR2BootAddr)));
#elif (CONFIG_MBOOT_IN_UFS_FLASH)
    snprintf(cmd, sizeof(cmd)-1, "ufs read.p 0x%08lX frc 0x100000", (U32)(PA2NVA(u32FRCR2BootAddr)));
#endif

    if (-1 != run_command(cmd, 0))
    {
        UBOOT_DEBUG("\n[%s: %d] read the frc partion data@%llx:\n",__func__,__LINE__,FRCR2BootAddr);
    }
    else
    {
        UBOOT_ERROR("%s: '%s' fails, at %d\n", __func__, cmd, __LINE__);
        return -1;
    }

    FRCR2_MBX_Init();
    MDrv_FRCR2_Init_Front();
    MDrv_FRCR2_Init_End(FRCR2BootAddr);

    if (get_map_size_from_env(E_FRC_R2, NO_DEFAULT_MMAP_VALUE, &u32FRCR2BootLen)!=0)
    {
        UBOOT_ERROR("get E_MMAP_ID_FRC_R2_LENGTH mmap fail\n");
        return -1;
    }
    UBOOT_DEBUG("E_MMAP_ID_FRC_R2 = 0x%llx 0x%x\n", FRCR2BootAddr, u32FRCR2BootLen);
    u8MIUProtectkernel_ID[0] = MIU_CLIENT_FRC_R2; //CPU. Ref: drvMIU.h

    return 0;
}

//#################################################################
int do_frc_send(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    MS_U8 u8CmdIdx;
    MS_U32 u32Data;
    MS_BOOL bResult;
    MS_U8 k=0;

    u8CmdIdx=(U8)atoi(argv[1]);
    u32Data = (U32)atoi(argv[2]);

    printf("\n[HK] do_frc_send : u8CmdIdx=%d, u32Data=0x%X\n",u8CmdIdx,u32Data);
    #if 0
    while(k--)
    {
        bResult = FRCR2_MBX_SendMsg(u8CmdIdx++,u32Data+k);
        mdelay(1000);
    }
    #else
    k=100;
    while(k--)
    {
        mdelay(100);
        bResult = FRCR2_MBX_SendMsg(u8CmdIdx++,u32Data+k);
        if(bResult)
        {
            mdelay(100);
            FRCR2_MBX_RecvMsg();

        }
        mdelay(1000);
    }
    #endif
    Write2Byte(0x101EA6,0xFFF4);//alex_tung###

    return 0;
}

int do_frc_recv(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    MS_U8 u8CmdIdx;
    MS_U32 u32Data;
    MS_BOOL bResult;
    MS_U8 k=5;

    u8CmdIdx=(U8)atoi(argv[1]);
    u32Data = (U32)atoi(argv[2]);

    while(k)
    {
        bResult = FRCR2_MBX_RecvMsg();
        if(bResult)
        {
            k--;
        }
        mdelay(100);
    }
    return 0;

}

#endif

#if defined(CONFIG_URSA20_VB1) && defined(CONFIG_PNL_OLED_LG_SUPPORT)
int do_oledLGOnRFDoneGPIO(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    if (MDrv_URSA_COM_Get_URSA_Type() == URSA_COMMON_U20)
    {
        if (IsTVOled())
        {
            OLED_LG_OnRFDone_GPIOSetting();
        }
    }
    return 0;
}
#endif


