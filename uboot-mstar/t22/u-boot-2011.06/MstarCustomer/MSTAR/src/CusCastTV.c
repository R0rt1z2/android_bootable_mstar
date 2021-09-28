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
#include <MsDebug.h>
#include <MsMmap.h>
#include <malloc.h>
#include <MsApiJpd.h>
#include <MsApiGEGOP.h>
#include <MsOS.h>
#include <CusConfig.h>
#include <apiGFX.h>
#include <bootlogo/MsBootLogo.h>
#include <gop/MsDrvGop.h>
#include <MsSysUtility.h>
#include <apiGOP.h>
#include <MsApiPanel.h>

extern U8 u8OsdLayer2GwinId;
static U32 g_spinner_buffer=0;

static int pic_no     = 0;
static int pic_width  = 0;
static int pic_height = 0;
static int pic_size = 0;


#pragma pack(1)     // set alignment to 1 byte boundary
typedef struct
{
    U8 szMagicNumber[4];
    U8 nTableSize[4];
    U8 nBodySize[4];
    U8 snBinaryType[4];
    U8 szCompressMethod[2];
    U8 decompressSize[4];
    U8 reserved[42];
    U8 nTotalNo[2];
}ST_BinFormatHeader;

typedef struct
{
    U8 nWidth[2];
    U8 nHeight[2];
    U8 cvt_format;
}ST_BinFormatBody;
#pragma pack()


int get_spinner_bin(void)
{
    UBOOT_TRACE("IN\n");
    U32 u32BinSize=0;
    U32 u32SrcAddr;
    int ret = 0;

    char * PathBuf=(char *)malloc(CMD_BUF);
    #if 1
    if(PathBuf==NULL)
    {
        UBOOT_ERROR("\033[0;31m malloc FAIL\033[0m\n");
    }
    if(get_addr_from_mmap("E_MMAP_ID_MBOOT_GOP_SPINNER_BUFFER", &u32SrcAddr)!=0)
    {
        UBOOT_ERROR("get E_MMAP_ID_MBOOT_GOP_SPINNER_BUFFER mmap fail\n");
        return -1;
    }
    #else
    u32SrcAddr=0x18000000;
    #endif
    UBOOT_DEBUG("u32SrcAddr=0x%x u32SrcAddrVA=0x%x",(U32)u32SrcAddr,(U32)MsOS_PA2KSEG1(u32SrcAddr));

    // read spinner bin from customer partition
    vfs_mount(CONFIG);
    memset(PathBuf,0,CMD_BUF);
    snprintf(PathBuf, CMD_BUF, "%s/spinner_bitmap.bin", CONFIG_PATH);
    u32BinSize = vfs_getsize(PathBuf);
    ret = vfs_read((void*)MsOS_PA2KSEG1((MS_U32)u32SrcAddr), PathBuf, 0, u32BinSize);
    if(ret == -1)
    {
        UBOOT_ERROR("Can't read %s!!\n", PathBuf);
        return ret;
    }
    UBOOT_DUMP(MsOS_PA2KSEG1((MS_U32)u32SrcAddr), 0x100);


    // read spinner bin header
    ST_BinFormatHeader stBinFormatHeader;
    ST_BinFormatBody stBinFormatBody;
    memset(&stBinFormatHeader, 0, sizeof(ST_BinFormatHeader));
    memset(&stBinFormatBody, 0, sizeof(stBinFormatBody));
    memcpy((void*)&stBinFormatHeader, (void*)MsOS_PA2KSEG1((MS_U32)u32SrcAddr), sizeof(ST_BinFormatHeader));
    memcpy((void*)&stBinFormatBody, (void*)MsOS_PA2KSEG1((MS_U32)(u32SrcAddr + sizeof(ST_BinFormatHeader))), sizeof(stBinFormatBody));

    UBOOT_DUMP((U32)&stBinFormatHeader.szMagicNumber, 0x4);
    UBOOT_DUMP((U32)&stBinFormatHeader.nTableSize, 0x4);
    UBOOT_DUMP((U32)&stBinFormatHeader.nBodySize, 0x4);
    UBOOT_DUMP((U32)&stBinFormatHeader.snBinaryType, 0x4);
    UBOOT_DUMP((U32)&stBinFormatHeader.szCompressMethod, 0x2);
    UBOOT_DUMP((U32)&stBinFormatHeader.decompressSize, 0x4);
    UBOOT_DUMP((U32)&stBinFormatHeader.reserved[0], 0x42);

    UBOOT_DUMP((U32)&stBinFormatBody.nWidth, 0x2);
    UBOOT_DUMP((U32)&stBinFormatBody.nHeight, 0x2);
    UBOOT_DUMP((U32)&stBinFormatBody.cvt_format, 0x1);

    // get numbers of pic, width, height from header
    pic_no     = (int)(stBinFormatHeader.nTotalNo[0]<<8) + (int)stBinFormatHeader.nTotalNo[1];
    pic_width  = (int)(stBinFormatBody.nWidth[0]<<8)     + (int)stBinFormatBody.nWidth[1];
    pic_height = (int)(stBinFormatBody.nHeight[0]<<8)    + (int)stBinFormatBody.nHeight[1];
    pic_size   = pic_width * pic_height * 4 ;
    UBOOT_DEBUG("pic_no = %d\n",     pic_no);
    UBOOT_DEBUG("pic_width = %d\n",  pic_width);
    UBOOT_DEBUG("pic_height = %d\n", pic_height);

    // bitmap raw data offset
    int offset = sizeof(ST_BinFormatHeader)+sizeof(ST_BinFormatBody)*pic_no;
    // bitmap offset need 256 bytes align
    offset = ((offset>>9)+1) << 9;
    UBOOT_INFO("offset = %x\n",offset);

    //assign gop start addr
    g_spinner_buffer = u32SrcAddr + offset;

    free(PathBuf);

    UBOOT_TRACE("OK\n");
    return ret;
}

void gop_init_spinner(MS_U32 XPos, MS_U32 YPos)
{
    GFX_BufferInfo srcbuf, dstbuf;
    GFX_DrawRect rect;
    LogoDispConfigParams stConfig1;
    U8 u8logoGopIdx =1;
    PanelType panel_data;

    //map size
    MS_U32 u32MapWidth=0;
    MS_U32 u32MapHeight =0;
    //GOP stretch window size
    MS_U32 u32StretchHSize=0;
    MS_U32 u32StretchVSize =0;
    //GOP stretch window position
    MS_U32 u32Stretch_XPos=0;
    MS_U32 u32Stretch_YPos =0;
    //GOP stretch window after stretch size
    MS_U32 u32StretchH_After_scaling =0;
    MS_U32 u32StretchV_After_scaling =0;

    // FIXED, wait agv
    u32MapWidth     = pic_width;
    u32MapHeight    = pic_height;
    u32Stretch_XPos = XPos;
    u32Stretch_YPos = YPos;
    u32StretchHSize = u32MapWidth;
    u32StretchVSize = u32MapHeight;
    u32StretchH_After_scaling = 250;
    u32StretchV_After_scaling = 250;

    MsDrvGOP_Init(u8logoGopIdx);
    Chip_Flush_Memory();

    //GE
    srcbuf.u32ColorFmt = E_MS_FMT_ARGB8888;
    srcbuf.u32Addr     = g_spinner_buffer;
    srcbuf.u32Width    = pic_width;
    srcbuf.u32Height   = pic_height;
    srcbuf.u32Pitch    = pic_width;
    rect.srcblk.x      = 0;
    rect.srcblk.y      = 0;
    rect.srcblk.width  = pic_width;
    rect.srcblk.height = pic_height;
    rect.dstblk.x      = 0;
    rect.dstblk.y      = 0;
    rect.dstblk.width  = pic_width;
    rect.dstblk.height = pic_height;

    //stretch window
    stConfig1.u16DispX=u32Stretch_XPos;
    stConfig1.u16DispY=u32Stretch_YPos;
    stConfig1.bHorStretch=u32StretchHSize;
    stConfig1.bVerStretch=u32StretchVSize;

    //GWIN
    dstbuf.u32Addr= g_spinner_buffer;
    dstbuf.u32Width =u32MapWidth;
    dstbuf.u32Height = u32MapHeight;
    dstbuf.u32Pitch = u32MapWidth;
    dstbuf.u32ColorFmt= E_MS_FMT_ARGB8888;

    Read_PanelParaFromflash(&panel_data);

    MsDrvGFX_Blt(&rect,&srcbuf,&dstbuf);
    if (panel_data.m_wPanelWidth==1920 && panel_data.m_wPanelHeight==1080)
        MsDrvGOP_Show1(u8logoGopIdx,&dstbuf,stConfig1.bHorStretch,stConfig1.bVerStretch,stConfig1.u16DispX/2,stConfig1.u16DispY/2,u32StretchH_After_scaling/2,u32StretchV_After_scaling/2);
    else
        MsDrvGOP_Show1(u8logoGopIdx,&dstbuf,stConfig1.bHorStretch,stConfig1.bVerStretch,stConfig1.u16DispX,stConfig1.u16DispY,u32StretchH_After_scaling,u32StretchV_After_scaling);
}

void show_spinner(void)
{
    static  unsigned int clk = 0;
    MApi_GOP_Switch_GWIN_2_FB_BY_ADDR(u8OsdLayer2GwinId,(g_spinner_buffer+clk*pic_size),0,0);
    if(clk == 159){
        clk=0;
    }else{
        clk++;
    }
}

MS_U32 g_check_timer_id;

MS_U32 Spinner_Timer_ISR_Register(void)
{
    MS_U32 x_pos = 0x6AD;
    MS_U32 y_pos = 0x274;
    if(get_spinner_bin()==0)
    {
        gop_init_spinner(x_pos, y_pos);
        g_check_timer_id=0;
        g_check_timer_id = MsOS_CreateTimer( (TimerCb)show_spinner,
                                                      0,
                                                      33,
                                                      TRUE,
                                                      "show spinner");
        printf("timer id =%d\n",(int)g_check_timer_id);
    }
    else
    {
        UBOOT_ERROR("get_spinner_bin fail\n");
        return -1;
    }
    return g_check_timer_id;
}

void stop_Spinner_Timer_ISR(void)
{
     MsOS_StopTimer(g_check_timer_id);
     MsOS_DeleteTimer(g_check_timer_id);
     MApi_GOP_GWIN_Enable(u8OsdLayer2GwinId, FALSE);
     printf("Stop timer !!\n");
}
int do_show_spinner(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    Spinner_Timer_ISR_Register();
    return 0;
}
