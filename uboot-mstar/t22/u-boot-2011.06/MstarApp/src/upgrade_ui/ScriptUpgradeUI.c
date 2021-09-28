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
#include <malloc.h>
#include <ShareType.h>
#include <MsApiJpd.h>
#include <MsApiGEGOP.h>
#include <MsVfs.h>
#include <MsSystem.h>
#include <MsUboot.h>
#include <MsDebug.h>
#include <MsRawIO.h>
#include <CusConfig.h>
#include <MsMmap.h>
#include <drvGPIO.h>
#include <MsBoot.h>
#include <miu/MsDrvMiu.h>
#include <MsEnvironment.h>
#include "MsTypes.h"
#include "MsOS.h"
#include "../bootlogo_lite/ScriptTypes.h"
#include "../bootlogo_lite/ScriptBootLogo.h"
#include "../../../MstarCore/src/hal/k5ap/bootlogo_lite/ScriptXC.h"
#include "../../../MstarCore/src/hal/k5ap/bootlogo_lite/ScriptGEGOP.h"
#include "stdio.h"
#include "../../../MstarCore/src/hal/k5ap/upgrade_ui/ScriptDrawRectAndJPG.h"



#define BOOT_LOGO_FLASH_BASE_ADDR 0x94000000//+0x60000
#define BOOT_LOGO_FLASH_DEFAULT_OFFSET 0x60000
#define BOOT_LOGO_DEFAULT_SIZE 0xC7C3
#define BOOT_LOGO_MAX_WIDTH 1920
#define BOOT_LOGO_MAX_HEIGHT 1080
#define BOOT_LOGO_MAX_SIZE 0x100000 // 1M
//*********************global*******************************//
EN_OUTPUT_TIMING_TYPE genOutputTiming;
MS_PHYADDR gphyInputAddr_Upgrade = 0;
MS_U16 gu16Width = 0;
MS_U16 gu16Height = 0;
MS_U16 gu16GOPPipeDelay = 0;





int  do_bootupgrade_init(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    UBOOT_TRACE("\nStart bootupgrade init\n");
    
    char *pEnv = NULL;
    MS_U32 u32Addr = 0;
    ST_RectInfo stRectInfoSrc = {0, 0};
    MS_U8 resolution_lite = 0;
    
    pEnv = getenv("resolution");
    if(pEnv == NULL)
    {
        resolution_lite = E_OUTPUT_TIMING_1920X1080_50P;
    }
    else
    {
        resolution_lite = (MS_U8)simple_strtoull(pEnv, NULL, 16);
    }
    UBOOT_TRACE("\n resolution 0x%x\n",resolution_lite);
    
    if (get_map_addr_from_env(E_DFB_FRAMEBUFFER, MMAP_E_DFB_FRAMEBUFFER_ADDR, &gphyInputAddr_Upgrade)!=0)
    {
        UBOOT_ERROR("get %s mmap fail\n",E_DFB_FRAMEBUFFER_ADR);
        return -1;
    }   


    
    genOutputTiming = resolution_lite;//E_OUTPUT_TIMING_1280X720_50P;//E_OUTPUT_TIMING_1920X1080_60P;//E_OUTPUT_TIMING_1920X1080_50P;//E_OUTPUT_TIMING_1920X1080_50I;//_BootLogo_GetOutputTiming();
    EN_HDMI_COLOR_FORMAT enHdmiColorFormat = E_HDMI_COLOR_FORMAT_RGB;//E_HDMI_COLOR_FORMAT_YUV;//_BootLogo_GetColorFormat();

    _Script_GEGOP_GetDstInfo(genOutputTiming, &gu16Width, &gu16Height, &gu16GOPPipeDelay);

    Script_XC_Init(genOutputTiming);
    Script_VE_Init(genOutputTiming);
    if(enHdmiColorFormat == E_HDMI_COLOR_FORMAT_RGB)
    {
        Script_XC_HDMI_YUV2RGB();
    }
    

    
    UBOOT_TRACE("\n End bootupgrade init\n");
    return 0;
}



int do_bootupgrade(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    UBOOT_TRACE("\nStart bootupgrade\n");
    
    char *pEnv = NULL;
    MS_U32 u32Addr = 0;
    ST_RectInfo stRectInfoSrc = {0, 0};
    MS_U8 resolution_lite = 0;
    
    pEnv = getenv("resolution");
    if(pEnv == NULL)
    {
        resolution_lite = E_OUTPUT_TIMING_1920X1080_50P;
    }
    else
    {
        resolution_lite = (MS_U8)simple_strtoull(pEnv, NULL, 16);
    }
    UBOOT_TRACE("\n resolution 0x%x\n",resolution_lite);
    
    if (get_map_addr_from_env(E_DFB_FRAMEBUFFER, MMAP_E_DFB_FRAMEBUFFER_ADDR, &gphyInputAddr_Upgrade)!=0)
    {
        UBOOT_ERROR("get %s mmap fail\n",E_DFB_FRAMEBUFFER_ADR);
        return -1;
    }   


    
    genOutputTiming = resolution_lite;//E_OUTPUT_TIMING_1280X720_50P;//E_OUTPUT_TIMING_1920X1080_60P;//E_OUTPUT_TIMING_1920X1080_50P;//E_OUTPUT_TIMING_1920X1080_50I;//_BootLogo_GetOutputTiming();
    EN_HDMI_COLOR_FORMAT enHdmiColorFormat = E_HDMI_COLOR_FORMAT_RGB;//E_HDMI_COLOR_FORMAT_YUV;//_BootLogo_GetColorFormat();

    _Script_GEGOP_GetDstInfo(genOutputTiming, &gu16Width, &gu16Height, &gu16GOPPipeDelay);

    Script_XC_Init(genOutputTiming);
    Script_VE_Init(genOutputTiming);
    if(enHdmiColorFormat == E_HDMI_COLOR_FORMAT_RGB)
    {
        Script_XC_HDMI_YUV2RGB();
    }
    

    Script_Upgrade_WindowClean();
    
    Script_Upgrade_ShowUI();
    Script_Upgrade_ShowProgress(1);
    Script_Upgrade_ShowProgress(2);
    Script_Upgrade_ShowProgress(3);
    //Sboot_Upgrade_ShowProgress(0);

    
    UBOOT_TRACE("\n End bootupgrade \n");
    return 0;
}

int do_bootupgrade_progress(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{

    
    MS_U8 u8Flage = 0;
    
    u8Flage = (MS_U8)simple_strtoull(argv[1], NULL, 16);
    Script_Upgrade_ShowProgress(u8Flage);
    return 0;
}
int do_bootupgrade_clean(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{

    Script_Upgrade_WindowClean();
    return 0;
}
int do_bootupgrade_ui(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{

    Script_Upgrade_ShowUI();
    return 0;

}


