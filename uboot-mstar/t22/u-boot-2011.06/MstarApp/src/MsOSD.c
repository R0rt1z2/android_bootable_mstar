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
#include <stdio.h>
#include <malloc.h>
#include <MsTypes.h>
#include <ShareType.h>
#include <MsOSD.h>
#include <MsApiGEGOP.h>
#include <MsDebug.h>
#include <MsUboot.h>
#include <MsVfs.h>
#include <MsApiJpd.h>
#include <MsApiPanel.h>
#include <MsRawIO.h>
#include <MsSystem.h>
#include <CusConfig.h>
#include <MsMmap.h>
#include <MsEnvironment.h>
#include <apiGOP.h>
#include <MsUpgradeUtility.h>
#include <bootlogo/MsPoolDB.h>
#include <miu/MsDrvMiu.h>
#if (CONFIG_URSA_UNION) || defined(CONFIG_URSA20_VB1)
#include <ursa/ursa_common.h>
#endif

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
typedef struct
{
    U16 u16DispX;
    U16 u16DispY;
    U16 u16DispW;
    U16 u16DispH;
}BltConfigParams;

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Extern Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Private Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
#if (ENABLE_MODULE_DISPLAY_OSD==1)

int do_create_osdlayer(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    MS_PHYADDR GopBuffer=0 ;
    MS_U8 u8logoGopIdx=0 ;

    if (argc < 3)
    {
        printf("Usage:\n%s\n", cmdtp->usage);
        return 1;
    }
    MS_U16 w = simple_strtoul(argv[1], NULL, 10);
    MS_U16 h = simple_strtoul(argv[2], NULL, 10);

    GFX_Block dstBlk = {0, 0, w, h};
    MS_U32 size = w * h;

    if (0 == size)
    {
        printf("Usage:\n%s\n", cmdtp->usage);
        return 1;
    }

    if(IsPanelReady() != 0)
    {

#if (CONFIG_URSA12_VB1_FRC_BRINGUP)
        if(run_command("frcup",0) != 0)
        {
            printf("Error : frcup fail!!\n");
            //return -1;
        }
#endif

        if(run_command("panel_pre_init",0) != 0)
        {
            printf("Error : panel_pre_init fail!!\n");
            return -1;
        }
    }

#if defined(CONFIG_ENABLE_4K2K_PANEL)
#if defined(CONFIG_INX_NOVA_VB1)
	run_command("inx_panel_set_init", 0);
#endif
#endif

#if(ENABLE_HDMI_TX == 1)
    if(run_command("hdmi init",0)!=0)
    {
        printf("Error : hdmi init fail!!\n");
        return -1;
    }

#endif

#if(ENABLE_OSDC_Init==1)
#if (CONFIG_URSA_UNION)
    if ((MDrv_URSA_COM_Get_URSA_Type() == URSA_COMMON_U9) || (MDrv_URSA_COM_Get_URSA_Type() == URSA_COMMON_U11) || (MDrv_URSA_COM_Get_URSA_Type() == URSA_COMMON_U20))
#elif defined(CONFIG_URSA20_VB1)
    if (MDrv_URSA_COM_Get_URSA_Type() == URSA_COMMON_U20)
#endif
    {
        run_command("xc_init" , 0);
    }
#endif

#if(ENABLE_CVBS_OUT == 1)
    run_command("cvbs",0);
#endif

#if (CONFIG_ENABLE_RTPM)
	run_command("runtime_pm" , 0);
#endif

#if defined(CONFIG_INX_VB1)
	run_command("panel_inx_vb1_init", 0);
#endif

    // send ursa6 cmd
#if defined(CONFIG_URSA6_VB1) || defined(CONFIG_URSA_UNION)
#if (CONFIG_URSA_UNION)
    if (MDrv_URSA_COM_Get_URSA_Type() == URSA_COMMON_U6)
#endif
    {
        run_command("send_I2C_cmd_to_ursa6" , 0);
    }
#endif

    // send ursa9 cmd
#if defined(CONFIG_URSA9_VB1) || defined(CONFIG_URSA20_VB1) || defined(CONFIG_URSA_UNION)
#if (CONFIG_URSA_UNION)
    if ((MDrv_URSA_COM_Get_URSA_Type() == URSA_COMMON_U9) || (MDrv_URSA_COM_Get_URSA_Type() == URSA_COMMON_U11) || (MDrv_URSA_COM_Get_URSA_Type() == URSA_COMMON_U20))
#endif
    {
#if defined(CONFIG_URSA20_VB1)
        run_command("oledLGOnRFDoneGPIO" , 0);
        run_command("send_I2C_cmd_to_ursa20" , 0);
#else
        run_command("send_I2C_cmd_to_ursa9" , 0);
#endif
    }
#endif

    // set panel backlight ON
    if(IsPanelReady() == 0 && check_osd_active()==TRUE)
    {
        if(run_command("panel_post_init",0) != 0)
        {
            printf("Error : panel_post_init fail!!\n");
            return -1;
        }

        #if (CONFIG_LOCAL_DIMMING)
        run_command("local_dimming", 0);
        #endif
    }
    if(get_map_addr_from_env(E_BOOTLOGO_BUFFER, MMAP_E_DFB_FRAMEBUFFER_ADDR,&GopBuffer)!=0)
    {
        UBOOT_ERROR("get E_BOOTLOGO_BUFFER mmap fail\n");
        return -1;
    }
    #if 0
    if(getenv("OSD_BufferAddr")==NULL)
    {
        if(get_map_addr_from_env(E_DFB_FRAMEBUFFER, MMAP_E_DFB_FRAMEBUFFER_ADDR,&GopBuffer)!=0)
        {
            UBOOT_ERROR("get %s mmap fail\n",E_DFB_FRAMEBUFFER_ADR);
            return -1;
        }
    }

    else // for SN request, once mmap is hard to designed, we can use other buffer as OSD buffer, and SN needs to set this env in design mmap stage, not after bug happened.
    {
        if(get_value_from_env(getenv("OSD_BufferAddr"), MMAP_E_DFB_FRAMEBUFFER_ADDR,&GopBuffer)!=0)
        {
            UBOOT_ERROR("get %s mmap fail\n",E_DFB_FRAMEBUFFER_ADR);
            return -1;
        }
    }
    #endif
    extern U8 g_logoMiuIdx;
    g_logoMiuIdx=0;
    g_logoMiuIdx = MsApi_Miu_GetID(GopBuffer);
    UBOOT_DEBUG("BOOTLOGO SET GOP MIU[%d]\n",g_logoMiuIdx);

    // initilize the GOP of Subtitle instead of UI;
    // otherwise, then bootlogo will be closed unexpectedly,
    // because UI is initilalized during booting up
    char* p_str = getenv("bootlogo_gopidx");
    if(p_str==NULL)
    {
        st_sys_misc_setting misc_setting;
        Read_MiscSetting_ToFlash(&misc_setting);
        u8logoGopIdx = misc_setting.m_u8LogoGopIdx;
    }
    else
    {
        u8logoGopIdx = simple_strtoul(p_str, NULL, 10);
    }
    UBOOT_INFO("[%s] GOP Boot Logo Index %d \n",__FUNCTION__,(int)u8logoGopIdx);
    MsApiOsdCreate(u8logoGopIdx,&dstBlk,GopBuffer);
    return 0;
}
int do_destroy_osdlayer(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    MsApiOsdDestroy();
    return 0;
}
int do_flush2screen_osdlayer(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    MsApiFlushCanvas2Screen();
    return 0;
}

static unsigned int LoadJpg2Dram(char *SrcPath,U32 *Dram_Addr)
{
    char PathBuf[128] = "\0";
    U32 u32LogoSize=0;
    UBOOT_TRACE("IN\n");

    vfs_mount(CONFIG);
    snprintf(PathBuf, sizeof(PathBuf), "%s/%s",CONFIG_PATH,SrcPath);
    UBOOT_DEBUG("PathBuf=%s\n",PathBuf);
    u32LogoSize = vfs_getsize(PathBuf);
    *Dram_Addr=(U32)malloc(u32LogoSize);
    UBOOT_DEBUG("Dram_Addr=0x%x\n",(unsigned int)*Dram_Addr);
    UBOOT_DEBUG("u32LogoSize=0x%x\n",(unsigned int)u32LogoSize);
    vfs_read((void *)(*Dram_Addr),PathBuf,0,u32LogoSize);

    UBOOT_TRACE("OK\n");
    return u32LogoSize;
}

void draw_jpg(BltConfigParams stConfig,U32 JPD_OUTADDR)
{

    GFX_BufferInfo src_info;
    GFX_Block Blk;

    UBOOT_TRACE("IN\n");

    src_info.u32Addr = JPD_OUTADDR;
    UBOOT_DEBUG("src_info.u32Addr=0x%x\n",(unsigned int)src_info.u32Addr);

    src_info.u32ColorFmt = GFX_FMT_YUV422;
    UBOOT_DEBUG("src_info.u32ColorFmt=0x%x\n",(unsigned int)src_info.u32ColorFmt);

    src_info.u32Width = (U32)JPG_GetAlignmentWidth();
    UBOOT_DEBUG("src_info.u32Width=0x%x\n",(unsigned int)src_info.u32Width);

    src_info.u32Height = (U32)JPG_GetAlignmentHeight();
    UBOOT_DEBUG("src_info.u32Height=0x%x\n",(unsigned int)src_info.u32Height);

    src_info.u32Pitch = (U32)JPG_GetAlignmentPitch()<<1;
    UBOOT_DEBUG("src_info.u32Pitch=0x%x\n",(unsigned int)src_info.u32Pitch);

    Blk.x = (U16)((stConfig.u16DispX+15)&0xFFFFF0);
    Blk.y = stConfig.u16DispY;
    Blk.width = (U16)((stConfig.u16DispW+15)&0xFFFFF0);;
    Blk.height = stConfig.u16DispH;
    MsApiDrawJPG(src_info,Blk);
    UBOOT_TRACE("OK\n");
}

int do_draw_jpg (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    U32 u32LogoAddr=0;
    U32 u32LogoSize=0;
    U32 JPD_InputADDR=0;
    U32 JPD_OUTADDR=0;
    char *PhotoPath=NULL;
    BltConfigParams stConfig;
    UBOOT_TRACE("IN\n");

    if(argc < 6)
    {
        cmd_usage(cmdtp);
        return 1;
    }
    stConfig.u16DispX = simple_strtoul(argv[2], NULL, 10);
    stConfig.u16DispY = simple_strtoul(argv[3], NULL, 10);
    stConfig.u16DispW = simple_strtoul(argv[4], NULL, 10);
    stConfig.u16DispH = simple_strtoul(argv[5], NULL, 10);
    UBOOT_DEBUG("u16DispX=0x%x,u16DispY=0x%x,u16DispW=0x%x,u16DispH=0x%x\n",stConfig.u16DispX,stConfig.u16DispY,stConfig.u16DispW,stConfig.u16DispH);

    if(IsPanelReady()!=0)
    {
         UBOOT_ERROR("Panle is not ready\n");
        return -1;
    }

    if(strncmp(argv[1], "-fs", 3)==0) // jpd from filesystem
    {
        PhotoPath = argv[6];
        UBOOT_DEBUG("PhotoPath=%s\n",PhotoPath);
        u32LogoSize = LoadJpg2Dram(PhotoPath, &JPD_InputADDR);
        UBOOT_DEBUG("u32LogoSize : 0x%x \n",u32LogoSize);
        if(u32LogoSize == 0)
        {
            free((void*)JPD_InputADDR);
            UBOOT_ERROR("Fail: Load_Photo2Dram return size is 0 !!\n");
            return -1;
        }
    }
    else if(strncmp(argv[1], "-s", 2)==0)
    {
        mboot_raw_io_Config();
        u32LogoAddr= simple_strtoul(argv[6],NULL,16);
        u32LogoSize= simple_strtoul(argv[7],NULL,16);;
        JPD_InputADDR=(U32)malloc(u32LogoSize);
        raw_read(JPD_InputADDR,u32LogoAddr,u32LogoSize);
    }
    else
    {
        cmd_usage(cmdtp);
        return -1;
    }
    flush_cache(JPD_InputADDR, u32LogoSize);
    JPD_OUTADDR=MsApi_JPD_Decode(JPD_InputADDR, u32LogoSize);
    draw_jpg(stConfig,JPD_OUTADDR);
    free((void*)JPD_InputADDR);
    UBOOT_TRACE("OK\n");
    return 0;
}

int do_draw_rect(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    if (argc < 6)
    {
        printf("Usage:\n%s\n", cmdtp->usage);
        return 1;
    }
    MS_U16 x = simple_strtoul(argv[1], NULL, 10);
    MS_U16 y = simple_strtoul(argv[2], NULL, 10);
    MS_U16 w = simple_strtoul(argv[3], NULL, 10);
    MS_U16 h = simple_strtoul(argv[4], NULL, 10);
    MS_U32 tColor = simple_strtoul(argv[5], NULL, 16);

    GFX_RgbColor color;
    color.a = tColor >> 24;
    color.r = (tColor >> 16) & 0xFF;
    color.g = (tColor >> 8) & 0xFF;
    color.b = tColor & 0xFF;
    GFX_Block dstBlk = {x, y, w, h};
    MsApiDrawRect(&dstBlk, color);
    return 0;
}

int do_draw_pixel(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    if (argc < 4)
    {
        printf("Usage:\n%s\n", cmdtp->usage);
        return 1;
    }

    MS_U16 x = simple_strtoul(argv[1], NULL, 10);
    MS_U16 y = simple_strtoul(argv[2], NULL, 10);
    MS_U32 tColor = simple_strtoul(argv[3], NULL, 16);

    GFX_RgbColor color;
    color.a = tColor >> 24;
    color.r = (tColor >> 16) & 0xFF;
    color.g = (tColor >> 8) & 0xFF;
    color.b = tColor & 0xFF;
    GFX_Point p = {x, y};
    MsApiDrawPixel(p, color);
    return 0;
}
int do_draw_string(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    MS_U8 font;
    MS_U16 x, y, attrib;
    MS_U32 tColor;
    GFX_RgbColor color;

    if (argc < 6)
    {
        printf("Usage:\n%s\n", cmdtp->usage);
        return 1;
    }
    x = simple_strtoul(argv[1], NULL, 10);
    y = simple_strtoul(argv[2], NULL, 10);
    tColor = simple_strtoul(argv[3], NULL, 16);
    attrib = simple_strtoul(argv[4], NULL, 10)%10;
    font = simple_strtoul(argv[4], NULL, 10)/10;

    color.a = tColor >> 24;
    color.r = (tColor >> 16) & 0xFF;
    color.g = (tColor >> 8) & 0xFF;
    color.b = tColor & 0xFF;


	MsApiSelectFont(font);
    char cStr[202] = {0};
    MS_U16 u16Total = 0;
    MS_S16 i = 5;

    for (; i < argc; ++i)
    {
        MS_U16 u16Len = strlen(argv[i]);
        if (u16Total + u16Len > 200)
        {
            break;
        }

        memset(cStr + u16Total, 0 , (sizeof(cStr)-u16Total));
        snprintf(cStr + u16Total, (sizeof(cStr)-u16Total), "%s", argv[i]);
        u16Total += u16Len;
        cStr[u16Total++] = ' ';
    }

    cStr[u16Total] = 0;

    MsApiDrawStrText(cStr, x, y, color, (TextAttrib)attrib);
    return 0;
}
int do_draw_Progress(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    if (argc < 5)
    {
        printf("Usage:\n%s\n", cmdtp->usage);
        return 1;
    }

    MS_U16 x = simple_strtoul(argv[1], NULL, 10);
    MS_U16 y = simple_strtoul(argv[2], NULL, 10);
    MS_U8 per = simple_strtoul(argv[4], NULL, 10);
    MS_U32 tColor = simple_strtoul(argv[3], NULL, 16);

    GFX_RgbColor color;
    color.a = tColor >> 24;
    color.r = (tColor >> 16) & 0xFF;
    color.g = (tColor >> 8) & 0xFF;
    color.b = tColor & 0xFF;
    MsApiDrawProgress(x, y, color, per);
    return 0;
}
#endif

