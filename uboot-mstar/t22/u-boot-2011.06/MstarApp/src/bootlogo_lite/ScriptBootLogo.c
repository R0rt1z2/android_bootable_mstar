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
#include "ScriptTypes.h"
#include "ScriptBootLogo.h"
#include "MsScriptOSD.h"
#include "stdio.h"


MS_U32 gu32JPD_InputADDR=0;
MS_U32 gu32JPD_OUTADDR=0;
MS_PHYADDR gphyInputAddr = 0;

#define BOOT_LOGO_FLASH_BASE_ADDR 0x94000000//+0x60000
#define BOOT_LOGO_FLASH_DEFAULT_OFFSET 0x60000
extern int g_JpgPitch;
extern int g_JpgWidth;
extern int g_JpgHeight;

static int LogoInMboot2Dram(MS_U32 *Dram_Addr)
{
    int ret = -1;
    MS_U32 u32LogoSize = 0;
    MS_U32 u32LogoOffset = 0;
    UBOOT_TRACE("IN\n");

    ret = mboot_raw_io_Config();
    if(ret != 0)
    {
        UBOOT_ERROR("raw_io_config setting fail!\n");
        return -1;
    }

    ret=MsApiChunkHeader_GetValue(CH_LOGO_ROM_OFFSET,&u32LogoOffset);
    if(ret!=0)
    {
        UBOOT_ERROR("get u32LogoOffset fail\n");
        return -1;
    }
    ret=MsApiChunkHeader_GetValue(CH_LOGO_ROM_SIZE,&u32LogoSize);
    if(ret!=0)
    {
        UBOOT_ERROR("get u32LogoSize fail\n");
        return -1;
    }
    
    *Dram_Addr=(MS_U32)malloc(u32LogoSize);
    if(Dram_Addr!=NULL)
    {
        if(raw_read(*Dram_Addr,u32LogoOffset,u32LogoSize)!=0)
        {
            UBOOT_ERROR("LogoInMboot2Dram Fail!!\n");
            return -1;
        }
    }

    UBOOT_TRACE("OK\n");
    return u32LogoSize;
}

#define BOOT_LOGO_DEFAULT_SIZE 0xC7C3
#define BOOT_LOGO_MAX_WIDTH 1920
#define BOOT_LOGO_MAX_HEIGHT 1080
#define BOOT_LOGO_MAX_SIZE 0x100000 // 1M
MS_BOOL do_bootlogo_lite(void)
{
    UBOOT_TRACE("\nStart bootlogo\n");
    
    char *pEnv=NULL;
    MS_U32 u32Addr = 0;
    ST_RectInfo stRectInfoSrc = {0, 0};
    MS_U8 resolution_lite = 0;
    MS_U32 u32LogoSize = 0;
    
    #if 0
    pEnv = getenv("upgrade_mode");
    if(strcmp(pEnv,"usb") == 0 || strcmp(pEnv,"oad") == 0)//when oad or usb upgrade we don't show logo first
    {   
        return FALSE;
    }
    #endif
    
    pEnv = getenv("resolution");
    if(pEnv == NULL)
    {
        resolution_lite = E_OUTPUT_TIMING_1920X1080_50I;// 8
        setenv("resolution", "8");//E_OUTPUT_TIMING_1920X1080_50I
        saveenv();
    }
    else
    {
        resolution_lite = (MS_U8)simple_strtoull(pEnv, NULL, 16);
    }
    UBOOT_TRACE("\nresolution 0x%x\n",resolution_lite);
    


    
    EN_OUTPUT_TIMING_TYPE enOutputTiming = resolution_lite;//E_OUTPUT_TIMING_1280X720_50P;//E_OUTPUT_TIMING_1920X1080_60P;//E_OUTPUT_TIMING_1920X1080_50P;//E_OUTPUT_TIMING_1920X1080_50I;//_BootLogo_GetOutputTiming();
    EN_HDMI_COLOR_FORMAT enHdmiColorFormat = E_HDMI_COLOR_FORMAT_YUV;//_BootLogo_GetColorFormat();

    Script_XC_Init(enOutputTiming);
    Script_VE_Init(enOutputTiming);

    if(enHdmiColorFormat == E_HDMI_COLOR_FORMAT_RGB)
    {
        Script_XC_HDMI_YUV2RGB();
    }

   
    if (get_map_addr_from_env(E_DFB_FRAMEBUFFER, MMAP_E_DFB_FRAMEBUFFER_ADDR, &gphyInputAddr)!=0)
    {
        UBOOT_ERROR("get %s mmap fail\n",E_DFB_FRAMEBUFFER_ADR);
        return FALSE;
     }

    u32LogoSize = LogoInMboot2Dram(&gu32JPD_InputADDR);
    gu32JPD_OUTADDR=MsApi_JPD_Decode((MS_U32)gu32JPD_InputADDR, u32LogoSize);

    stRectInfoSrc.u16Height = g_JpgHeight;
    stRectInfoSrc.u16Width = g_JpgWidth;
    stRectInfoSrc.u16Pitch = g_JpgPitch;
    
    if(g_JpgWidth > BOOT_LOGO_MAX_WIDTH ||g_JpgHeight > BOOT_LOGO_MAX_HEIGHT ||u32LogoSize > BOOT_LOGO_MAX_SIZE)
    {
        return FALSE;
    }
    
    Script_GEGOP_Init(stRectInfoSrc, enOutputTiming);
    
    UBOOT_TRACE("\nEnd bootlogo\n");
    return TRUE;
}

//stub funcs
typedef MS_U32 (*FUtopiaOpen)(void** ppInstance, const void* const pAttribute);
void PNLRegisterToUtopia(FUtopiaOpen ModuleType)
{
}
void GOPRegisterToUtopia(FUtopiaOpen ModuleType)
{
}
void GFXRegisterToUtopia(FUtopiaOpen ModuleType)
{
}
void XCRegisterToUtopia(FUtopiaOpen ModuleType)
{
}
void TVENCODERRegisterToUtopia(FUtopiaOpen ModuleType)
{
}
void HDMITXRegisterToUtopia(FUtopiaOpen ModuleType)
{
}
void DACRegisterToUtopia(FUtopiaOpen ModuleType)
{
}
/*
void CECRegisterToUtopia(FUtopiaOpen ModuleType)
{
}*/
