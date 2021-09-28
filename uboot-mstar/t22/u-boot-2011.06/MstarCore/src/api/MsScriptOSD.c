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
#include "MsOS.h"
#include "MsTypes.h"
#include "MsScriptOSD.h"
#include "ScriptDrawRectAndJPG.h"
#include "ScriptTypes.h"
#include "ScriptXC.h"
#include "ScriptGEGOP.h"
#include "stdio.h"
#include "ScriptUpgradeJPGData.h"

extern EN_OUTPUT_TIMING_TYPE genOutputTiming;
extern MS_U32 gphyInputAddr_Upgrade;
extern int g_JpgPitch;
extern int g_JpgWidth;
extern int g_JpgHeight;
extern MS_U16 gu16Width;
extern MS_U16 gu16Height;
extern MS_U16 gu16GOPPipeDelay;
extern MS_U32 _gu32VEFrameBuff;
#define BOOT_LOGO_FLASH_BASE_ADDR 0x94000000//+0x60000
#define BOOT_LOGO_FLASH_DEFAULT_OFFSET 0x60000
#define BOOT_LOGO_DEFAULT_SIZE 0xC7C3
#define BOOT_LOGO_MAX_WIDTH 1920
#define BOOT_LOGO_MAX_HEIGHT 1080
#define BOOT_LOGO_MAX_SIZE 0x100000 // 1M

typedef struct{
	MS_U16  u16StartX;
	MS_U16  u16StartY;
	MS_U16  u16UIHeight;
	MS_U16  u16UIWidth;
}ST_UIRectInfo;


MS_BOOL Script_XC_Init(EN_OUTPUT_TIMING_TYPE enOutputTimingType)
{
    switch(enOutputTimingType)
    {
        case E_OUTPUT_TIMING_720X480_60I:
            _Script_XC_Init_720X480_60I();
            break;
        case E_OUTPUT_TIMING_720X480_60P:
            _Script_XC_Init_720X480_60P();
            break;
        case E_OUTPUT_TIMING_720X576_50I:
            _Script_XC_Init_720X576_50I();
            break;
        case E_OUTPUT_TIMING_720X576_50P:
            _Script_XC_Init_720X576_50P();
            break;
        case E_OUTPUT_TIMING_1280X720_50P:
            _Script_XC_Init_1280X720_50P();
            break;
        case E_OUTPUT_TIMING_1280X720_60P:
            _Script_XC_Init_1280X720_60P();
            break;
        case E_OUTPUT_TIMING_1920X1080_50I:
            _Script_XC_Init_1920X1080_50I();
            break;
        case E_OUTPUT_TIMING_1920X1080_60I:
            _Script_XC_Init_1920X1080_60I();
            break;
        case E_OUTPUT_TIMING_1920X1080_50P:
            _Script_XC_Init_1920X1080_50P();
            break;
        case E_OUTPUT_TIMING_1920X1080_60P:
            _Script_XC_Init_1920X1080_60P();
            break;
        default:
            break;
    }
	
    return TRUE;
}

MS_BOOL Script_VE_Init(EN_OUTPUT_TIMING_TYPE enOutputTimingType)
{



    MS_PHYADDR VEAddr = 0;
    MS_U32 u32VESize = 0;
    if(get_map_addr_from_env(E_VE, MMAP_E_MMAP_ID_VE_ADDR, &VEAddr) != 0)
    {
        UBOOT_ERROR("get %s env fail\n",E_MMAP_ID_VE_ADR);
        return FALSE;
    }
    if(get_map_size_from_env(E_VE, MMAP_E_MMAP_ID_VE_SIZE, &u32VESize) != 0)
    {
        UBOOT_ERROR("get E_MMAP_ID_VE_LEN env fail\n");
        return FALSE;
    }


    _gu32VEFrameBuff = VEAddr;//pstBufInfoVEFrm->u32PhysicalAddr;

    switch(enOutputTimingType)
    {
        case E_OUTPUT_TIMING_720X480_60I:
            _Script_VE_Init_NTSC(E_OUTPUT_RESOLUTION_720X480);
            break;
        case E_OUTPUT_TIMING_720X480_60P:
            _Script_VE_Init_NTSC(E_OUTPUT_RESOLUTION_720X480);
            break;
        case E_OUTPUT_TIMING_720X576_50I:
            _Script_VE_Init_PAL(E_OUTPUT_RESOLUTION_720X576);
            break;
        case E_OUTPUT_TIMING_720X576_50P:
            _Script_VE_Init_PAL(E_OUTPUT_RESOLUTION_720X576);
            break;
        case E_OUTPUT_TIMING_1280X720_50P:
            _Script_VE_Init_PAL(E_OUTPUT_RESOLUTION_1280X720);
            break;
        case E_OUTPUT_TIMING_1280X720_60P:
            _Script_VE_Init_NTSC(E_OUTPUT_RESOLUTION_1280X720);
            break;
        case E_OUTPUT_TIMING_1920X1080_50I:
            _Script_VE_Init_PAL(E_OUTPUT_RESOLUTION_1920X1080);
            break;
        case E_OUTPUT_TIMING_1920X1080_60I:
            _Script_VE_Init_NTSC(E_OUTPUT_RESOLUTION_1920X1080);
            break;
        case E_OUTPUT_TIMING_1920X1080_50P:
            _Script_VE_Init_PAL(E_OUTPUT_RESOLUTION_1920X1080);
            break;
        case E_OUTPUT_TIMING_1920X1080_60P:
            _Script_VE_Init_NTSC(E_OUTPUT_RESOLUTION_1920X1080);
            break;
        default:
            break;
    }

	return TRUE;

}
MS_BOOL Script_XC_HDMI_YUV2RGB(void)
{
    return _Script_XC_HDMI_YUV2RGB();
}


MS_BOOL Script_GOP2VE(const MS_U32 u32SrcAddr, const ST_RectInfo stRectSrcInfo, const EN_OUTPUT_TIMING_TYPE enOutputTiming)
{
   return _Script_GOP2VE(u32SrcAddr,stRectSrcInfo,enOutputTiming);
}
	
MS_BOOL Script_GEGOP_Init(const ST_RectInfo stRectSrcInfo, const EN_OUTPUT_TIMING_TYPE enOutputTiming)
{
    return _Script_GEGOP_Init(stRectSrcInfo,enOutputTiming);

}

static void ShowUpgrading_GetStart(
	MS_U16* u16DstStartX,
	MS_U16* u16DstStartY,
	MS_U16* u16StretchHeight,
	MS_U16* u16StretchWidth
	)
{

	MS_U16 u16DefStartX = 0x2EE;
	MS_U16 u16DefStartY = 0x1F4;
	MS_U16 u16DefStretchHeight = 40 * 3;
	MS_U16 u16DefStretchWidth = 120 * 3;
    MS_U16 u16Width = gu16Width;
    MS_U16 u16Height = gu16Height;
	MS_U16 u16DefWidth = 1920;
	MS_U16 u16DefHeight = 1080;
	
	*u16DstStartX = (u16Width * u16DefStartX)/u16DefWidth;
	*u16DstStartY = (u16Height * u16DefStartY)/u16DefHeight;
	*u16StretchWidth =(u16Width * u16DefStretchWidth)/u16DefWidth;
	*u16StretchHeight =(u16Height * u16DefStretchHeight)/u16DefHeight;

}

static void ShowUI_GetUIRectConvert(
	const ST_UIRectInfo  stUIRectInfoDef,
	ST_UIRectInfo * stUIRectInfoGet)
{
	ST_UIRectInfo stUIRect = {0};
	MS_U16 u16DefStartX = stUIRectInfoDef.u16StartX;
	MS_U16 u16DefStartY = stUIRectInfoDef.u16StartY;
	MS_U16 u16DefHeight = stUIRectInfoDef.u16UIHeight;
	MS_U16 u16DefWidth = stUIRectInfoDef.u16UIWidth;
    MS_U16 u16Width = gu16Width;
    MS_U16 u16Height = gu16Height;
	MS_U16 u16DefGlobalWidth = 1920;
	MS_U16 u16DefGlobalHeight = 1080;	

	
	stUIRect.u16StartX = (u16DefStartX * u16Width)/u16DefGlobalWidth;
	stUIRect.u16StartY = (u16DefStartY * u16Height)/u16DefGlobalHeight;
	stUIRect.u16UIHeight = (u16DefHeight *u16Height)/u16DefGlobalHeight ;
	stUIRect.u16UIWidth = (u16DefWidth* u16Width)/u16DefGlobalWidth;
	
	*stUIRectInfoGet = stUIRect;
	UBOOT_TRACE("[J]Defu16StartX = %ld \n",u16DefStartX);
	UBOOT_TRACE("[J]Defu16StartY = %ld \n",u16DefStartY);
	UBOOT_TRACE("[J]Defu16Width = %ld \n",u16DefWidth);
	UBOOT_TRACE("[J]Defu16Height = %ld \n",u16DefHeight);
	UBOOT_TRACE("[J]Getu16StartX = %ld \n",stUIRect.u16StartX);
	UBOOT_TRACE("[J]Getu16StartY = %ld \n",stUIRect.u16StartY);
	UBOOT_TRACE("[J]Getu16Width = %ld \n",stUIRect.u16UIWidth);
	UBOOT_TRACE("[J]Getu16Height = %ld \n",stUIRect.u16UIHeight);

	
}


static void ShowUI_GetUIRectConvert_Points(
	const ST_UIRectInfo  stUIRectInfoDef,
	ST_UIRectInfo * stUIRectInfoGet)
{
	ST_UIRectInfo stUIRect = {0};
	MS_U16 u16DefStartX = stUIRectInfoDef.u16StartX;
	MS_U16 u16DefStartY = stUIRectInfoDef.u16StartY;
	MS_U16 u16DefHeight = stUIRectInfoDef.u16UIHeight;
	MS_U16 u16DefWidth = stUIRectInfoDef.u16UIWidth;
    MS_U16 u16Width = gu16Width;
    MS_U16 u16Height = gu16Height;
	MS_U16 u16DefGlobalWidth = 1920;
	MS_U16 u16DefGlobalHeight = 1080;	

	//stUIRect.u16StartX = (u16Width/2)+(190*u16Width)/u16DefGlobalWidth;
	//stUIRect.u16StartY = (u16Height/2);/*+(30*u16Height)/u16DefGlobalHeight;*/
	stUIRect.u16StartX = (u16DefStartX * u16Width)/u16DefGlobalWidth;
	stUIRect.u16StartY = (u16DefStartY * u16Height)/u16DefGlobalHeight;
	stUIRect.u16UIHeight = (u16DefHeight *u16Height)/u16DefGlobalHeight ;
	stUIRect.u16UIWidth = (u16DefWidth* u16Width)/u16DefGlobalWidth;
	
	*stUIRectInfoGet = stUIRect;
	
}


void Script_Upgrade_WindowClean(void)
{
	//show the window backgroud black
    MS_U16 u16Width = gu16Width;
    MS_U16 u16Height = gu16Height;
	ST_FillRectInfo stFillRectInfoClean = {0,0,u16Width,u16Height,0,0x80,0,0x80,(MS_U32)gphyInputAddr_Upgrade};
	_Script_GEGOP_FillRect(stFillRectInfoClean);

}

void Script_Upgrade_ShowUpgrading(void)
{

		//show upgrading
		MS_U32 u32LogoSize = 120*40;
		MS_U16  u16DstStartX = 0;
		MS_U16  u16DstStartY = 0;
		MS_U16  u16StretchHeight = 0;
		MS_U16  u16StretchWidth = 0;
		
		ShowUpgrading_GetStart(&u16DstStartX,&u16DstStartY,&u16StretchHeight,&u16StretchWidth);
		
		ST_BitBlitInfo stBitBlitInfoUpgrade;
		memset(&stBitBlitInfoUpgrade,0,sizeof(stBitBlitInfoUpgrade));
		stBitBlitInfoUpgrade.u32JpdOutputAddr = MsApi_JPD_Decode((MS_U32)upgrading_jpg, u32LogoSize);
		UBOOT_TRACE("stBitBlitInfoUpgrade.u32JpdOutputAddr = %ld \n",stBitBlitInfoUpgrade.u32JpdOutputAddr);
		stBitBlitInfoUpgrade.u16SrcWidth = g_JpgWidth;
		stBitBlitInfoUpgrade.u16SrcHeight = g_JpgHeight;
		stBitBlitInfoUpgrade.u16SrcPitch = g_JpgPitch;
		stBitBlitInfoUpgrade.u16DstStartX = u16DstStartX;
		stBitBlitInfoUpgrade.u16DstStartY = u16DstStartY;
		stBitBlitInfoUpgrade.u16StretchHeight = u16StretchHeight;//g_JpgHeight*3;
		stBitBlitInfoUpgrade.u16StretchWidth = u16StretchWidth; //g_JpgWidth*3 ;
		stBitBlitInfoUpgrade.u32GeOutputAddr = (MS_U32)gphyInputAddr_Upgrade;
		
		
		if(g_JpgWidth > BOOT_LOGO_MAX_WIDTH ||g_JpgHeight > BOOT_LOGO_MAX_HEIGHT ||u32LogoSize > BOOT_LOGO_MAX_SIZE)
		{
			return FALSE;
		}	
		_Script_GEGOP_DrawJpeg(stBitBlitInfoUpgrade);
		
}

void Script_Upgrade_ShowUI(void)
{
	
	//draw upgrade backgroud rect 
	ST_UIRectInfo  stUIRectInfoDef = {250,200,680,1420};
	ST_UIRectInfo  stUIRectInfoGet = {0}; 

	ShowUI_GetUIRectConvert(stUIRectInfoDef,&stUIRectInfoGet);	
	ST_FillRectInfo stFillRectInfoUI = {stUIRectInfoGet.u16StartX,\
		stUIRectInfoGet.u16StartY,stUIRectInfoGet.u16UIWidth,\
		stUIRectInfoGet.u16UIHeight,0x0,0xFF,0x1D,0x6B,(MS_U32)gphyInputAddr_Upgrade};//blue 
	_Script_GEGOP_FillRect(stFillRectInfoUI);
	Script_Upgrade_ShowUpgrading();
}



MS_BOOL Script_Upgrade_ShowProgress(MS_U8 u8Flage)
{


	UBOOT_TRACE("u8Flage %d !\n",u8Flage);

	if(u8Flage == 1)//show 1st  points
	{
		ST_UIRectInfo  stUIRectInfoDef = {0x460,0x23A,10,10};
		ST_UIRectInfo  stUIRectInfoGet = {0}; 		
		ShowUI_GetUIRectConvert_Points(stUIRectInfoDef,&stUIRectInfoGet);
		ST_FillRectInfo stFillRectInfoIng  = {stUIRectInfoGet.u16StartX,\
		stUIRectInfoGet.u16StartY,stUIRectInfoGet.u16UIWidth,stUIRectInfoGet.u16UIWidth,\
		0x0,0x80,0xFF,0x80,(MS_U32)gphyInputAddr_Upgrade};
		_Script_GEGOP_FillRect(stFillRectInfoIng);
	}
	else if(u8Flage == 2)//show 2nd points
	{
		ST_UIRectInfo  stUIRectInfoDef = {0x460+30,0x23A,10,10};
		ST_UIRectInfo  stUIRectInfoGet = {0}; 		
		ShowUI_GetUIRectConvert_Points(stUIRectInfoDef,&stUIRectInfoGet);
		ST_FillRectInfo stFillRectInfoIng  = {stUIRectInfoGet.u16StartX,\
		stUIRectInfoGet.u16StartY,stUIRectInfoGet.u16UIWidth,stUIRectInfoGet.u16UIWidth,\
		0x0,0x80,0xFF,0x80,(MS_U32)gphyInputAddr_Upgrade};
		_Script_GEGOP_FillRect(stFillRectInfoIng);
	}
	else if(u8Flage == 3)//show 3rd  points
	{
		ST_UIRectInfo  stUIRectInfoDef = {0x460+60,0x23A,10,10};
		ST_UIRectInfo  stUIRectInfoGet = {0}; 		
		ShowUI_GetUIRectConvert_Points(stUIRectInfoDef,&stUIRectInfoGet);
		ST_FillRectInfo stFillRectInfoIng  = {stUIRectInfoGet.u16StartX,\
		stUIRectInfoGet.u16StartY,stUIRectInfoGet.u16UIWidth,stUIRectInfoGet.u16UIWidth,\
		0x0,0x80,0xFF,0x80,(MS_U32)gphyInputAddr_Upgrade};
		_Script_GEGOP_FillRect(stFillRectInfoIng);
	}
	else if(u8Flage == 0)//clean progress points
	{
		ST_UIRectInfo  stUIRectInfoDef = {0x460,0x23A,10,90};
		ST_UIRectInfo  stUIRectInfoGet = {0}; 		
		ShowUI_GetUIRectConvert_Points(stUIRectInfoDef,&stUIRectInfoGet);
		ST_FillRectInfo stFillRectInfoRes  = {stUIRectInfoGet.u16StartX,\
		stUIRectInfoGet.u16StartY,stUIRectInfoGet.u16UIWidth,stUIRectInfoGet.u16UIHeight,\
		0x0,0xFF,0x1D,0x6B,(MS_U32)gphyInputAddr_Upgrade};
		_Script_GEGOP_FillRect(stFillRectInfoRes);
	}
	else
	{
		UBOOT_ERROR("ERR parameter %d!\n",u8Flage);
		return FALSE;
	}
	return TRUE;	

}



