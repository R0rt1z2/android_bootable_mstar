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

#include <stdio.h>
#include <stddef.h>
#include <exports.h>
#include <MsCmdTable.h>
#include <config.h>
#include <mstarstr.h>
#include <MsBoot.h>
#include <ShareType.h>

#if CONFIG_RESCUE_ENV  && CONFIG_RESCUE_ENV_IR_TRIGGER
#include <MsSystem.h>
#endif

#if (CONFIG_URSA_UNION) || defined(CONFIG_URSA20_VB1)
#include <ursa/ursa_common.h>
#endif

#ifdef UFBL_FEATURE_IDME
#include <idme.h>
#endif

#include <msAPI_Power.h>

void Customer_Register_Process(void)
{
    if(is_str_resume())
    {
        #if (!(CONFIG_LOGO_STORE_IN_MBOOT) && !(CONFIG_LOGO_STORE_NOT_SET))
        Add_Command_Table ("dbtable_init" , 0, STAGE_PROCESS);
        #endif

        Add_Command_Table ("updatemiureg" , 0, STAGE_PROCESS);

        #if defined (CONFIG_SECURITY_BOOT)
        Add_Command_Table ("SecureInit" , 0, STAGE_PROCESS);  //should run before frcup
        #endif

        #if(ENABLE_URSA_8==1)
        Add_Command_Table ("InitUrsa8" , 0, STAGE_PROCESS);
        #endif

        #if !defined(CONFIG_SYS_SKIP_FAST_STR) || (CONFIG_SYS_SKIP_FAST_STR != 1)
        #if (CONFIG_URSA12_VB1_FRC_BRINGUP)
        Add_Command_Table ("frcup",0,STAGE_PROCESS);
        #endif
        #endif

        Add_Command_Table ("panel_pre_init" , 0, STAGE_PROCESS);

        #if(ENABLE_XC_Init==1)
		#if (CONFIG_URSA_UNION)
		if ((MDrv_URSA_COM_Get_URSA_Type() == URSA_COMMON_U9) || (MDrv_URSA_COM_Get_URSA_Type() == URSA_COMMON_U11) || (MDrv_URSA_COM_Get_URSA_Type() == URSA_COMMON_U20))
        #elif defined(CONFIG_URSA20_VB1)
        if (MDrv_URSA_COM_Get_URSA_Type() == URSA_COMMON_U20)
        #endif
        {
            Add_Command_Table ("xc_init" , 0, STAGE_PROCESS);
        }
        #endif

        #if(ENABLE_CVBS_OUT==1)
        Add_Command_Table ("cvbs" , 0, STAGE_PROCESS);
        #endif

        #if(ENABLE_HDMI_TX == 1)
        Add_Command_Table ("hdmi init" , 0, STAGE_PROCESS);
        #endif

        #if(ENABLE_URSA_8==1)
        Add_Command_Table ("ursa8_lvds_on" , 0, STAGE_PROCESS);
        #endif

        #if defined (CONFIG_URSA_6M40)
        Add_Command_Table("ursa_lvds_on", 0, STAGE_PROCESS);
        Add_Command_Table("ursa_osd_unmute", 0, STAGE_PROCESS);
        #endif

        #if defined (CONFIG_TEE)
        #if defined (CONFIG_ARMv8_ARM_TRUSTED_FIRMWARE)|| defined(CONFIG_ARMv7_OPTEE)
        Add_Command_Table ("readArmFw;readOptee" , 0, STAGE_TOKERNEL);
        #else
        #if !defined(CONFIG_R2TEE_TEELOADER)
        #if (ENABLE_MSTAR_R2_STR == 1)
        Add_Command_Table ("r2resume" ,0, STAGE_PROCESS);
        #else
        Add_Command_Table ("readNuttx" ,0, STAGE_PROCESS);
        Add_Command_Table ("bootNuttx" ,0, STAGE_PROCESS);
        #endif
        #endif // #if !defined(CONFIG_R2TEE_TEELOADER)
        #endif
        #endif

        #if !defined(CONFIG_SYS_SKIP_FAST_STR) || (CONFIG_SYS_SKIP_FAST_STR != 1)
        #if (CONFIG_MUNICH_PNL_EN == 0)
        #if (CONFIG_ENABLE_RTPM)
        Add_Command_Table ("runtime_pm" , 0, STAGE_PROCESS);
        #endif
        #endif
        #endif


        #if defined (CONFIG_TEE)
        #if !defined (CONFIG_ARMv8_ARM_TRUSTED_FIRMWARE) && !defined(CONFIG_ARMv7_OPTEE) && !defined(CONFIG_MSTAR_R2_STR)
        Add_Command_Table ("wait_tee_ready" ,0, STAGE_PROCESS);
        #endif
        #endif

        //ursa6
        #if defined(CONFIG_URSA6_VB1) || defined(CONFIG_URSA_UNION)
        #if defined(CONFIG_URSA_UNION)
        if (MDrv_URSA_COM_Get_URSA_Type() == URSA_COMMON_U6)
        #endif
        {
            Add_Command_Table ("send_I2C_cmd_to_ursa6" , 0, STAGE_PROCESS);
        }
        #endif

        //ursa9
        #if defined(CONFIG_URSA9_VB1) || defined(CONFIG_URSA20_VB1) || defined(CONFIG_URSA_UNION)
        #if defined(CONFIG_URSA_UNION)
        if ((MDrv_URSA_COM_Get_URSA_Type() == URSA_COMMON_U9) || (MDrv_URSA_COM_Get_URSA_Type() == URSA_COMMON_U11) || (MDrv_URSA_COM_Get_URSA_Type() == URSA_COMMON_U20))
        #elif defined(CONFIG_URSA20_VB1)
        if (MDrv_URSA_COM_Get_URSA_Type() == URSA_COMMON_U20)
        #endif
        {
            #if defined(CONFIG_URSA20_VB1)
            Add_Command_Table ("oledLGOnRFDoneGPIO" , 0, STAGE_PROCESS);
            Add_Command_Table("send_I2C_cmd_to_ursa20" , 0, STAGE_PROCESS);
            #else
            Add_Command_Table ("send_I2C_cmd_to_ursa9" , 0, STAGE_PROCESS);
            #endif
        }
        #endif

        Add_Command_Table ("panel_post_init" , 0, STAGE_PROCESS);

        #if (CONFIG_LOCAL_DIMMING)
        Add_Command_Table("local_dimming", 0,STAGE_PROCESS);
        #endif

        Add_Command_Table ("checkstr" , 0, STAGE_PROCESS);
    }


    #if defined (CONFIG_URSA_6M40)
    Add_Command_Table("ursa_lvds_off", 0, STAGE_PROCESS);
    Add_Command_Table("delay 50", 0, STAGE_PROCESS);
    #if !defined(CONFIG_ENABLE_4K2K_PANEL)
    Add_Command_Table("ursa_2k_mode", 0, STAGE_PROCESS);
    #endif
    #endif

    #if defined (CONFIG_URSA20_VB1) || defined(CONFIG_URSA_UNION)
    if((MDrv_URSA_COM_Get_URSA_Type() == URSA_COMMON_U9) || (MDrv_URSA_COM_Get_URSA_Type() == URSA_COMMON_U11) || (MDrv_URSA_COM_Get_URSA_Type() == URSA_COMMON_U20))
    {
        Add_Command_Table("ursa_Reset", 0, STAGE_PROCESS);
    }
    #endif

    #if defined(CONFIG_MTK_WIFI_7668)
    Add_Command_Table("wifi_Reset", 0, STAGE_PROCESS);
    #endif

    #if defined(CONFIG_ENABLE_MIU_SSC)
    Add_Command_Table("MIU_SSC", 0, STAGE_PROCESS);
    #endif

     //Register customter Process
    #if defined(CONFIG_LOAD_ENV_FROM_SN) || defined(CONFIG_Customer)
    Add_Command_Table ("envload" , 0, STAGE_PROCESS);
    #endif

    #if defined (CONFIG_MBOOT_VERSION)
    Add_Command_Table ("mbootver_set" , 0, STAGE_PROCESS);
    #endif

    #if(ENABLE_URSA_8==1)
    Add_Command_Table ("InitUrsa8" , 0, STAGE_PROCESS);
    #endif

    #if(ENABLE_MODULE_BOOT_IR==1)
    Add_Command_Table ("ir_delay" , 0, STAGE_PROCESS);//for IR detect delay
    #endif


    //************************************************************
    // original from App_Register_Process()
    //************************************************************
    #if CONFIG_RESCUE_ENV
    #if CONFIG_RESCUE_ENV_AUTO_TRIGGER
    Add_Command_Table ("BrickTerminator" , 0, STAGE_PROCESS);
    #else
    #if CONFIG_RESCUE_ENV_IR_TRIGGER
    {
        unsigned int u32Value = 0;
        char *pEnv = getenv(BRICK_TERMINATOR_IR_TRIGGERED_STRING);
        if(NULL != pEnv)
        {
            u32Value = (unsigned int)simple_strtoul(pEnv, NULL, 10);
            if(1 == u32Value)
            {
                Add_Command_Table ("BrickTerminator" , 0, STAGE_PROCESS);
            }
        }
    }
    #endif //CONFIG_RESCUE_ENV_IR_TRIGGER
    #endif //CONFIG_RESCUE_ENV_AUTO_TRIGGER
    #endif //CONFIG_RESCUE_ENV

    #ifdef CONFIG_ENABLE_SYNC_MMAP
    Add_Command_Table ("sync_mmap" , 0, STAGE_PROCESS);
    #endif

    #if (!(CONFIG_LOGO_STORE_IN_MBOOT) && !(CONFIG_LOGO_STORE_NOT_SET))
    Add_Command_Table ("dbtable_init" , 0, STAGE_PROCESS);
    #endif

    Add_Command_Table ("updatemiureg" , 0, STAGE_PROCESS);

    #if defined (CONFIG_SECURITY_BOOT)
    Add_Command_Table ("SecureInit" , 0, STAGE_PROCESS);
    #endif

    #if (ENABLE_UTEST == 1)
    Add_Command_Table ("utest" , 0, STAGE_PROCESS);
    #endif

    #ifdef CONFIG_DISPLAY_VERSIONINFO
    Add_Command_Table ("mversion" , 0, STAGE_PROCESS);
    #endif

    #if defined(CONFIG_BOOT2_WRITEPROTECT) && defined(CONFIG_CMD_MMC) && defined(CONFIG_TEE)
    extern unsigned long gOpteeFailFlag;
    if(gOpteeFailFlag == 0)
        Add_Command_Table ("mmc writeprotect.boot2" , 0, STAGE_PROCESS);
    #endif

    #if defined(CONFIG_USERDATA_PARTITION_SELF_ADAPTION)
    Add_Command_Table ("emmc_self_adaption" , 0, STAGE_PROCESS);
    #endif

    #if (CONFIG_OTP_READ_WRITE_DEMO == 1)
    // "checksn" needs to set env, and this env will be set to bootargs
    Add_Command_Table ("checksn" , 0, STAGE_PROCESS);
    #endif

    Add_Command_Table ("bootargs_set" , 0, STAGE_PROCESS);
    Add_Command_Table ("config2env" , 0, STAGE_PROCESS);
    Add_Command_Table ("wdt_enable 0", 0, STAGE_PROCESS);

    #if !defined(CONFIG_ANDROID_BOOT) && !defined(CONFIG_MI_CONFIG_PARTITION)
    // reload_mac(sync) mac address for SN
    Add_Command_Table ("reload_mac" , 0, STAGE_PROCESS);
    #endif
}


//you can type "showtb 0" in mboot console to see registered cmd.
void Customer_Register_ToKernel(void)
{
    extern unsigned long gOpteeFailFlag;
    EN_POWER_ON_MODE ePowerMode = EN_POWER_DC_BOOT;

    //Some flow must run after MBoot console entry to avoid MBoot halt.

#if defined(CONFIG_INSTALL_CHECK_ENABLE) && (CONFIG_INSTALL_CHECK_ENABLE==1)
    Add_Command_Table ("installcheck", 0, STAGE_TOKERNEL);
#endif

   /*bootcheck:check boot mode(usb/oad/net upgrade, recovery, normal).
     If it is normal mode, just go through this command.
     you can mark this to implement your customer boot flow. */
    Add_Command_Table ("bootcheck" , 0, STAGE_TOKERNEL);

#if defined (CONFIG_UPGRADE_URSA)
    Add_Command_Table ("ursa_upgrade_mmc_check_update" , 0, STAGE_TOKERNEL);
#endif
    Add_Command_Table ("wdt_enable" , 0, STAGE_TOKERNEL);// wdt_enable need to place in bootcheck's next cmd.

    Add_Command_Table ("if_boot_to_pm" , 0, STAGE_TOKERNEL);

#ifdef CONFIG_MSTAR_FAST_STR
#ifndef CONFIG_TEE
    Add_Command_Table ("strbackup" , 0, STAGE_TOKERNEL);
#endif
#endif
    // if_boot_to_pm run system into power down，we will not init panel, in order to save power.
    // so we should plase panel_pre_init after if_boot_to_pm

    #if (ENABLE_MODULE_TEE==1)
    #if defined(CONFIG_SECURE_NO_ROOT)// partial_partition
        Add_Command_Table("authenticate RFS 0", 0,STAGE_TOKERNEL);
        Add_Command_Table("authenticate APP 0", 0,STAGE_TOKERNEL);
        Add_Command_Table("authenticate MSLIB 0", 0,STAGE_TOKERNEL);
        Add_Command_Table("authenticate CONFIG 0", 0,STAGE_TOKERNEL);
        Add_Command_Table("authenticate cacert 0", 0,STAGE_TOKERNEL);
    #endif
    #endif

    #ifdef CONFIG_POWER_MUSIC
    Add_Command_Table ("audio_preinit" , 0, STAGE_TOKERNEL);
    #endif

    #if (CONFIG_URSA12_VB1_FRC_BRINGUP)
    Add_Command_Table ("frcup",0,STAGE_TOKERNEL);
    #endif
    #if (CONFIG_PANEL_INIT)
    Add_Command_Table ("panel_pre_init" , 0, STAGE_TOKERNEL);
    #endif


    //**********************************************************************
    // original from App_Register_ToKernel()
    //**********************************************************************
	#if (CONFIG_MUNICH_PNL_EN)
	#if (CONFIG_ENABLE_RTPM)
    Add_Command_Table ("runtime_pm" , 0, STAGE_TOKERNEL);
	#endif
	#endif

    #if defined(CONFIG_ENABLE_4K2K_PANEL)
    #if defined(CONFIG_INX_NOVA_VB1)
    Add_Command_Table("inx_panel_set_init", 0, STAGE_TOKERNEL);
    #endif
    #endif

    Add_Command_Table("unlockcmi", 0, STAGE_TOKERNEL);

    #if (CONFIG_DOLBY)
    Add_Command_Table ("dolby_init" , 0, STAGE_TOKERNEL);
    #endif

    #if(ENABLE_XC_Init==1)
    #if (CONFIG_URSA_UNION)
    if ((MDrv_URSA_COM_Get_URSA_Type() == URSA_COMMON_U9) || (MDrv_URSA_COM_Get_URSA_Type() == URSA_COMMON_U11))
    #elif defined(CONFIG_URSA20_VB1)
    if (MDrv_URSA_COM_Get_URSA_Type() == URSA_COMMON_U20)
    #endif
    {
        Add_Command_Table ("xc_init" , 0, STAGE_TOKERNEL);
    }
    #endif

    #if(ENABLE_CVBS_OUT==1)
    Add_Command_Table ("cvbs" , 0, STAGE_TOKERNEL);
    #endif

    #if (CONFIG_MUNICH_PNL_EN == 0)
    #if (CONFIG_ENABLE_RTPM)
    Add_Command_Table ("runtime_pm" , 0, STAGE_TOKERNEL);
    Add_Command_Table ("boot_led_init", 0, STAGE_TOKERNEL);
    #endif
    #endif

    #if (ENABLE_HDMI_TX == 1)
    Add_Command_Table ("hdmi init" , 0, STAGE_TOKERNEL);
    #endif

    #ifdef CONFIG_BOOTLOGO_LITE
    Add_Command_Table ("bootlogo_lite" , 0, STAGE_TOKERNEL);
    #endif
    #ifdef CONFIG_DISPLAY_LOGO
    char *BootlogoFile = NULL;
    char BootlogoCmd[CMD_BUF]  = {0};

    BootlogoFile = getenv("BootlogoFile");
    if( BootlogoFile==NULL )
    {
        strncpy(BootlogoCmd,"bootlogo",CMD_BUF);
    }
    else
    {
        char *p = strchr(BootlogoFile, ';');
        if (p)
            // Terminate the string to avoid injected commands
            *p = '\0';

        snprintf(BootlogoCmd, CMD_BUF, "bootlogo 0 0 1 1 %s", BootlogoFile);
    }
    char* opt_logo = NULL;
    opt_logo = getenv("logo");
    if(opt_logo != NULL)
    {
        int logo_on = 0;
        logo_on = simple_strtol(opt_logo, NULL, 10);
        if(logo_on > 0 && logo_on < 3)
        {
            Add_Command_Table (BootlogoCmd , 0, STAGE_TOKERNEL);
            #if defined (CONFIG_URSA_6M40)
            Add_Command_Table ("ursa_lvds_on" , 0, STAGE_TOKERNEL);
            #endif
			#if defined (CONFIG_URSA_8)
            Add_Command_Table ("ursa8_lvds_on" , 0, STAGE_TOKERNEL);
			#endif
        }
    }
    else
    {
        Add_Command_Table (BootlogoCmd , 0, STAGE_TOKERNEL);
        #if defined (CONFIG_URSA_6M40)
        Add_Command_Table ("ursa_lvds_on" , 0, STAGE_TOKERNEL);
        #endif
    }
    #endif

    #if ENABLE_STB_ECOS_BOOT
    Add_Command_Table ("read_boot_info" , 0, STAGE_TOKERNEL);
    #endif

    #if defined(CONFIG_INX_VB1) && defined(CONFIG_ENABLE_4K2K_PANEL)
    Add_Command_Table("panel_inx_vb1_init",0,STAGE_TOKERNEL);
    #endif

    #if (CONFIG_BOOTVIDEO == 1)
    Add_Command_Table("bootvideo 0",0,STAGE_TOKERNEL);
    #endif

    #ifdef CONFIG_POWER_MUSIC
    char *BootmusicFile = NULL;
    char BootmusicCmd[CMD_BUF]  = {0};

    BootmusicFile = getenv("BootmusicFile");
    if( BootmusicFile == NULL )
    {
        strncpy(BootmusicCmd, "bootmusic", CMD_BUF);
    }
    else
    {
        char *p = strchr(BootmusicFile, ';');
        if (p)
            // Terminate the string to avoid injected commands
            *p = '\0';

        snprintf(BootmusicCmd, CMD_BUF, "bootmusic %s", BootmusicFile);
    }

    #if (ENABLE_MODULE_TEE==0)
    Add_Command_Table (BootmusicCmd, 0, STAGE_TOKERNEL);
    #endif
    #endif

    #if (CONFIG_MSTAR_FASTBOOT_ENABLE)
    Add_Command_Table ("fastboot" , 0, STAGE_TOKERNEL);
    #endif

    #ifdef CONFIG_GENERIC_MMC
	Add_Command_Table ("mmc slcrelwrchk" , 0, STAGE_TOKERNEL);
    #endif


    #if defined(CONFIG_INX_NOVA_VB1)
    Add_Command_Table ("inx_nova_set_4k2k_2division", 0, STAGE_TOKERNEL);
    #endif

    #if defined(CONFIG_URSA_8)
    Add_Command_Table ("ursa8_set_osd_mode" , 0, STAGE_TOKERNEL);
    #endif

    #if defined(CONFIG_URSA_6M40)
    Add_Command_Table ("ursa_set_osd_mode" , 0, STAGE_TOKERNEL);
    #endif

    #if defined(CONFIG_UART_RX)
    Add_Command_Table ("uart_rx" , 0, STAGE_TOKERNEL);
    #endif

    #if defined(CONFIG_SEC_SYSTEM) // jump to non-TEE second kernel
    Add_Command_Table ("Boot2secKernel" , 0, STAGE_TOKERNEL);
    #endif

    #if (CONFIG_URSA_10)
    Add_Command_Table("send_I2C_cmd_to_ursa10" , 0,STAGE_TOKERNEL);
    #endif

    //*************** alway in bottom ****************
    #if (ENABLE_MODULE_TEE==1)
    #if defined (CONFIG_ARMv8_ARM_TRUSTED_FIRMWARE)|| defined(CONFIG_ARMv7_OPTEE)
    if(gOpteeFailFlag == 0)
    {
        Add_Command_Table ("readArmFw;readOptee" , 0, STAGE_TOKERNEL);
        Add_Command_Table ("readKL" , 0, STAGE_TOKERNEL);
    }
    #else
    #if defined(CONFIG_R2TEE_TEELOADER)
    Add_Command_Table ("uart2R2" , 0, STAGE_TOKERNEL);
    Add_Command_Table ("readKL" , 0, STAGE_TOKERNEL);
    #else
    Add_Command_Table ("readNuttx;bootNuttx" , 0, STAGE_TOKERNEL);
    Add_Command_Table ("readKL" , 0, STAGE_TOKERNEL);
    Add_Command_Table ("wait_tee_ready" , 0, STAGE_TOKERNEL);
    #endif // #if defined(CONFIG_R2TEE_TEELOADER)
    #endif
    #if defined(CONFIG_URSA6_VB1) || defined(CONFIG_URSA_UNION)
    #if (CONFIG_URSA_UNION)
    if (MDrv_URSA_COM_Get_URSA_Type() == URSA_COMMON_U6)
    #endif  // CONFIG_URSA_UNION
    {
        Add_Command_Table("send_I2C_cmd_to_ursa6" , 0,STAGE_TOKERNEL);
    }
    #endif  // CONFIG_URSA6_VB1 || CONFIG_URSA_UNION

    #if defined(CONFIG_URSA9_VB1) || defined(CONFIG_URSA20_VB1) || defined(CONFIG_URSA_UNION)
    #if (CONFIG_URSA_UNION)
    if ((MDrv_URSA_COM_Get_URSA_Type() == URSA_COMMON_U9) || (MDrv_URSA_COM_Get_URSA_Type() == URSA_COMMON_U11))
    #elif defined(CONFIG_URSA20_VB1)
    if (MDrv_URSA_COM_Get_URSA_Type() == URSA_COMMON_U20)
    #endif
    {
        #if defined(CONFIG_URSA20_VB1)
        Add_Command_Table ("oledLGOnRFDoneGPIO" , 0, STAGE_TOKERNEL);
        Add_Command_Table("send_I2C_cmd_to_ursa20" , 0, STAGE_TOKERNEL);
        #else
        Add_Command_Table ("send_I2C_cmd_to_ursa9" , 0, STAGE_TOKERNEL);
        #endif
    }
    #endif

    #if (CONFIG_PANEL_INIT)// Init Panel
    Add_Command_Table("panel_post_init", 0,STAGE_TOKERNEL);
    #endif

    #if defined(CONFIG_URSA20_VB1)
    if (MDrv_URSA_COM_Get_URSA_Type() == URSA_COMMON_U20)
    {
        // Show Ursa Version
        Add_Command_Table("getUrsa20Version", 0,STAGE_TOKERNEL);
    }
    #endif
    #if (CONFIG_LOCAL_DIMMING)
    Add_Command_Table("local_dimming", 0,STAGE_TOKERNEL);
    #endif

    #ifdef CONFIG_POWER_MUSIC
    Add_Command_Table (BootmusicCmd, 0, STAGE_TOKERNEL);
    #endif
    /* ACOS_MOD_BEGIN {amazon_common_kernel_signing_scheme} */
    //if(gOpteeFailFlag == 0)
    //    Add_Command_Table ("bootKL" , 0, STAGE_TOKERNEL);
    char* bm_buf = NULL;
    bm_buf = getenv("bootmode");
    if(bm_buf != NULL)
    {
        int bootmode = atoi(bm_buf);
        if (bootmode == IDME_BOOTMODE_DIAG) {
            Add_Command_Table("amzn_boot diag", 0, STAGE_TOKERNEL);
        }else
            Add_Command_Table ("amzn_boot normal" , 0, STAGE_TOKERNEL);
    }
    /* ACOS_MOD_END {amazon_common_kernel_signing_scheme} */
    if(gOpteeFailFlag == 0)
        Add_Command_Table ("bootKL" , 0, STAGE_TOKERNEL);
    #endif  // endif ENABLE_MODULE_TEE

    #if (ENABLE_MODULE_TEE == 0)
    #if (ENABLE_SECURITY_BOOT == 1)
    Add_Command_Table ("readKL" , 0, STAGE_TOKERNEL);
    Add_Command_Table ("bootKL" , 0, STAGE_TOKERNEL);
    #endif
    #endif
    //*********************************************


}

void Customer_Register_MsInit(void)
{
        //Register customter MsInit
}
