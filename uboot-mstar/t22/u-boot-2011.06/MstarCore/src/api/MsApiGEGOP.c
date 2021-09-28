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

#include <MsTypes.h>
#include <common.h>
#include <command.h>
#include <config.h>
#include "stdio.h"
#include <gop/MsDrvGop.h>
#include <apiGOP.h>
#include <MsOS.h>
#include "MsApiGEGOP.h"
#include <MsApiPanel.h>
#include <MsSystem.h>
#include "uboot_mmap.h"
#include "LoaderCharset.h"
#include "LoaderCharset_Eng.h"
#include <MsDebug.h>
#include "msAPI_string.c"
#include <bootlogo/MsPoolDB.h>

#define ASC_CHAR_TABLE      LoaderEngCharacterset
#define ASC_FONT_TABLE      LoaderEngFont
#define CHAR_WIDTH_TABLE    LoaderEngWidthData

#define UNI_CHAR_TABLE      pLoaderCharacterset
#define UNI_FONT_TABLE      pLoaderFont
#define UCHAR_WIDTH_TABLE   pLoaderWidthData

#define ASC_CHAR_ARRAY      EngFont24X32
#define UNI_CHAR_ARRAY      Font24X24

#define MAX_CHAR_HEIGHT     32
#define ONE_CHAR_HEIGHT     24
#define EACH_CHAR_INTERVAL  2
#define SPACE_WIDTH         12
#define PROGRESS_NUM        32
#define PROGRESS_WIDTH      10
#define PROGRESS_INTERVAL   4
#define UNICODE             0
#define ASC                 1
#if (ENABLE_DISPLAY_PICTURE_UPDATE)
#define CANVAS_WIDTH        1280
#define CANVAS_HEIGHT       720
#else
#define CANVAS_WIDTH        720
#define CANVAS_HEIGHT       576
#endif
#define ENABLE_FRC_OSD 0

#define ID_MAGIC 0x554E


extern U8 u8FbId,u8GwinId;
extern U8 u8OsdLayerFbId,u8OsdLayerGwinId;
extern U16 u16OsdW,u16OsdH;
extern MS_BOOL gbPnlModMirrorMode;

static MS_U32 u32FontBitMask[] =
{
    0x80000000, 0x40000000, 0x20000000, 0x10000000,
    0x08000000, 0x04000000, 0x02000000, 0x01000000,
    0x00800000, 0x00400000, 0x00200000, 0x00100000,
    0x00080000, 0x00040000, 0x00020000, 0x00010000,
    0x00008000, 0x00004000, 0x00002000, 0x00001000,
    0x00000800, 0x00000400, 0x00000200, 0x00000100
};
//for new font small and large
static MS_U8 u8FontBitMask[] =
{
	0x80,
	0x40,
	0x20,
	0x10,
	0x08,
	0x04,
	0x02,
	0x01
};
static MS_U8 font_size_table[8][4] =
{
	{32,32,12,2},
	{16,16,12,2}
};
const unsigned char *CharTable[8]=
	{
		LoaderEngFont,
		LoaderEngFont_small
	};
const unsigned char *FontWidthTable[8] =
	{
		LoaderEngWidthData,
		LoaderEngWidthData_small
	};
MS_U8 using_font = font32X32;
FontInfo font_info= {0};
static unsigned char Font16X16[32] = {0};//16X16
static unsigned char Font32X32[128] = {0};//32X32
static unsigned char Font24X24[72] = {0};//24X24
//static unsigned int EngFont24X32[24] = {0};//24X32

//for CANVAS_BUFFER_ADDR buffer
static unsigned int gu32CanvasBuffer=0;
static MS_U16 lineOffset = 0;
static MS_U16 TextTotalWidth = 0;

static MS_U16 g_u16Record_OsdCreate_W;
static MS_U16 g_u16Record_OsdCreate_H;
static MS_U16 g_u16Record_OsdCreate_X;
static MS_U16 g_u16Record_OsdCreate_Y;

static size_t wstrlen(const MS_U8 * s)
{
    const MS_U8 *sc;

    for (sc = s; *sc != '\0' && *sc + 1 != '\0'; sc = sc + 2)
        /* nothing */;
    return sc - s;
}

static MS_BOOL GetFontInfo(FontInfo *pFontInfo,MS_U8 using_font )
{
	if(using_font > font_max || font_max < 0 )
		{
			printf("Error: This font is out of range !\n");
			return false;
		}
	if(pFontInfo == NULL)
		{
			printf("Error: NULL Pointer \n");
			return false;
		}

	pFontInfo->font_width= font_size_table[using_font][0];
	pFontInfo->font_height= font_size_table[using_font][1];
	pFontInfo->space_width= font_size_table[using_font][2];
	pFontInfo->interval_width= font_size_table[using_font][3];
	pFontInfo->pCharTable = (unsigned char *)CharTable[using_font];
	pFontInfo->pFontWidthTable = (unsigned char *)FontWidthTable[using_font];

	return true;


}
void MsApiSelectFont(MS_U8 font_choose)
{
	using_font = font_choose;
	return ;
}

#if (CONFIG_LOGO_STORE_IN_MBOOT)
void GetOSDMirrorMode(st_sys_misc_setting* misc_setting)
{
    char* strOSDmirrorMode = NULL;

    strOSDmirrorMode=getenv("MIRROR_OSD");
    if(strOSDmirrorMode)
    {
        UBOOT_INFO("MIRROR_OSD is  --> %s\n",strOSDmirrorMode);
        if(!strcmp(strOSDmirrorMode, "3"))
        {
            misc_setting->m_u8MirrorMode = 3;
        }
        else if(!strcmp(strOSDmirrorMode, "2"))
        {
            misc_setting->m_u8MirrorMode = 2;
        }
        else if(!strcmp(strOSDmirrorMode, "1"))
        {
            misc_setting->m_u8MirrorMode = 1;
        }
        else
        {
            misc_setting->m_u8MirrorMode = 0;
        }
    }
    else
    {
        misc_setting->m_u8MirrorMode = 0;
    }
}
#endif

#if (ENABLE_DISPLAY_LOGO) // GEGOP

void MsApiDisplay_Logo(MS_U8 u8logoGopIdx, GFX_DrawRect *rect,GFX_BufferInfo *src_info,GFX_BufferInfo *dst_info, MS_BOOL bHorStretch, MS_BOOL bVerStretch,MS_U16 u16DispX,MS_U16 u16DispY)
{

    UBOOT_TRACE("IN\n");
    U16* u16TempAddr = (U16*)MsOS_PA2KSEG1(dst_info->u32Addr);

    if(u16TempAddr == NULL)
    {
        printf("%s: Error: NULL pointer, at %d\n", __func__, __LINE__);
        return;
    }
    MsDrvGFX_Init();
    MsDrvGOP_Init(u8logoGopIdx);
    #if 0 //by jh
    U32 i=0;
    for (i=0;i<((dst_info->u32Pitch)*(rect->srcblk.height));i++)
    {
        *u16TempAddr = 0x8000;
        u16TempAddr++;
    }
    #endif

    Chip_Flush_Memory();
    MsDrvGFX_Blt(rect,src_info,dst_info);
    src_info->u32Pitch =src_info->u32Pitch>>1;
    dst_info->u32Pitch =dst_info->u32Pitch>>1;

    st_sys_misc_setting misc_setting;
#if (CONFIG_LOGO_STORE_IN_MBOOT)
    GetOSDMirrorMode(&misc_setting);
#else
    Read_MiscSetting_ToFlash(&misc_setting);
#endif

    int mirrorMode=misc_setting.m_u8MirrorMode;
    if(1==mirrorMode)
    {
        printf("<<set_mirror V & H ON!!>> \n");
        MApi_GOP_GWIN_SetVMirror(TRUE);
        if(gbPnlModMirrorMode==FALSE)
        {
            MApi_GOP_GWIN_SetHMirror(TRUE);
        }
        else
        {
          printf("<<Skip H ON!!>> \n");
        }
    }
    else if(2==mirrorMode)
    {
        printf("<<set_mirror V ON!!>> \n");
        MApi_GOP_GWIN_SetVMirror(TRUE);
    }
    else if(3==mirrorMode)
    {
        if(gbPnlModMirrorMode==FALSE)
        {
            printf("<<set_mirror H ON!!>> \n");
            MApi_GOP_GWIN_SetHMirror(TRUE);
        }
        else
        {
            printf("<<MOD H_Mirror Enable !!>> \n");
        }
    }

    MsDrvGOP_Show(u8logoGopIdx,dst_info,bHorStretch,bVerStretch,u16DispX,u16DispY);
    printf("[AT][MB][Show Logo][%lu]\n", MsSystemGetBootTime());
    UBOOT_TRACE("OK\n");

}

#endif
void MsApiDrawJPG(GFX_BufferInfo src_info,GFX_Block Blk)
{
    GFX_BufferInfo dst_info;
    GFX_DrawRect gfxBitbltInfo;
    GFX_Point v0;
    GFX_Point v1;

    if (0xFF == u8OsdLayerFbId)
    {
        return;
    }

    v0.x = Blk.x;
    v0.y = Blk.y;
    v1.x = Blk.x + Blk.width;
    v1.y = Blk.y + Blk.height;
    MApi_GFX_SetClip(&v0, &v1);
    MApi_GFX_SetDC_CSC_FMT(GFX_YUV_RGB2YUV_PC, GFX_YUV_OUT_PC, GFX_YUV_IN_255, GFX_YUV_YVYU,  GFX_YUV_YUYV);

    dst_info.u32ColorFmt = (GFX_Buffer_Format)E_MS_FMT_YUV422;
    dst_info.u32Addr = gu32CanvasBuffer;
    dst_info.u32Width = CANVAS_WIDTH;
    dst_info.u32Height = CANVAS_HEIGHT;
    dst_info.u32Pitch = CANVAS_WIDTH << 1;

    MApi_GFX_SetDstBufferInfo(&dst_info, 0);
    MApi_GFX_SetSrcBufferInfo(&src_info, 0);

    gfxBitbltInfo.dstblk.height = Blk.height;
    gfxBitbltInfo.dstblk.width = Blk.width;
    gfxBitbltInfo.dstblk.x = Blk.x;
    gfxBitbltInfo.dstblk.y = Blk.y;

    gfxBitbltInfo.srcblk.height = src_info.u32Height;
    gfxBitbltInfo.srcblk.width = src_info.u32Width;
    gfxBitbltInfo.srcblk.x = 0;
    gfxBitbltInfo.srcblk.y = 0;

    MApi_GFX_BitBlt(&gfxBitbltInfo, GFXDRAW_FLAG_SCALE);
    MApi_GFX_FlushQueue();
    MApi_GFX_SetDC_CSC_FMT(GFX_YUV_RGB2YUV_PC, GFX_YUV_OUT_PC, GFX_YUV_IN_255, GFX_YUV_YVYU,  GFX_YUV_YVYU);
}

void MsApiDrawRect(GFX_Block* pBlk, GFX_RgbColor color)
{
    GFX_Point v0 = {0, 0};
    GFX_Point v1 = {CANVAS_WIDTH, CANVAS_HEIGHT};
    MApi_GFX_SetClip(&v0, &v1);
    GFX_RgbColor tmpcolor;

    GFX_BufferInfo gfxDstBuf;
    gfxDstBuf.u32ColorFmt = (GFX_Buffer_Format)E_MS_FMT_YUV422;
    gfxDstBuf.u32Addr = gu32CanvasBuffer;
    gfxDstBuf.u32Width = CANVAS_WIDTH;
    gfxDstBuf.u32Height = CANVAS_HEIGHT;
    gfxDstBuf.u32Pitch = CANVAS_WIDTH << 1;
    MApi_GFX_SetDstBufferInfo(&gfxDstBuf, 0);

    GFX_RectFillInfo gfxFillBlock;
    gfxFillBlock.dstBlock.x = pBlk->x;
    gfxFillBlock.dstBlock.y = pBlk->y;
    gfxFillBlock.dstBlock.height = pBlk->height;
    gfxFillBlock.dstBlock.width = pBlk->width;

    gfxFillBlock.fmt = (GFX_Buffer_Format)E_MS_FMT_YUV422;

    if(gfxDstBuf.u32ColorFmt == GFX_FMT_YUV422)
    {
        tmpcolor.r = (MS_U8)( 0.439*color.r-0.368*color.g-0.071*color.b+128);
        tmpcolor.g = (MS_U8)( 0.257*color.r+0.504*color.g+0.098*color.b+16);
        tmpcolor.b = (MS_U8)(-0.148*color.r-0.291*color.g+0.439*color.b+128);
		tmpcolor.a = 0xFF;
    }
    else
    {
        tmpcolor.r = color.r;
        tmpcolor.g = color.g;
        tmpcolor.b = color.b;
		tmpcolor.a = 0xFF;
    }

    memcpy(&gfxFillBlock.colorRange.color_s,&tmpcolor,sizeof(tmpcolor));
    memcpy(&gfxFillBlock.colorRange.color_e,&tmpcolor,sizeof(tmpcolor));

    gfxFillBlock.flag = GFXRECT_FLAG_COLOR_CONSTANT;

    MApi_GFX_BeginDraw();
    MApi_GFX_RectFill(&gfxFillBlock);
    MApi_GFX_EndDraw();

    return;
}
void MsApiDrawPixel(GFX_Point p, GFX_RgbColor color)
{
    GFX_Block dstBlk = {p.x, p.y, 1, 1};
    MsApiDrawRect(&dstBlk, color);
    return ;
}
static MS_BOOL drawASCChar(MS_U16 X, MS_U16 Y, MS_U16 W, GFX_RgbColor color)
{
    MS_U16 h = 0;
    MS_U16 i = 0;
    unsigned char castFont[8]={0};
    Y+=(MAX_CHAR_HEIGHT-font_info.font_height);
    for (; h < font_info.font_height; ++h)
    {

        GFX_Point point;
        point.y = h + Y;

        MS_U16 w = 0;
		for(i=0;i<font_info.font_height/8;i++)
		{
			if(using_font == font32X32)
				{
					castFont[i] = Font32X32[h*4+i];
				}
			if(using_font  ==font16X16)
				{
					castFont[i] = Font16X16[h*2+i];
				}
		}
	     i=0;
        for (; w < W; ++w)
        {
            //Draw one ponit
            i = w/8;
	        castFont[0] = castFont[i];
            if (castFont[0]  & u8FontBitMask[w-i*8])
            {
                point.x = w + X;
                MsApiDrawPixel(point, color);
            }
        }
    }

    return TRUE;
}
static MS_BOOL drawUniChar(MS_U16 X, MS_U16 Y, MS_U16 W, GFX_RgbColor color)
{
    MS_U16 h = 0;
    Y+=(MAX_CHAR_HEIGHT-ONE_CHAR_HEIGHT);
    for (; h < ONE_CHAR_HEIGHT; ++h)
    {
        MS_U16 u16Idx = 3 * h;
        MS_U32 u32Line = (UNI_CHAR_ARRAY[u16Idx] << 24) | (UNI_CHAR_ARRAY[u16Idx + 1] << 16) | (UNI_CHAR_ARRAY[u16Idx + 2] << 8);

        if (0 == u32Line)
        {
            continue;
        }

        GFX_Point point;
        point.y = h + Y;

        MS_U16 w = 0;
        for (; w < W; ++w)
        {
            //Draw one ponit
            if (u32Line & u32FontBitMask[w])
            {
                point.x = w + X;
                MsApiDrawPixel(point, color);
            }
        }
    }

    return TRUE;
}

static MS_S16 findASCCharIndex(const char* pStrText)
{
    MS_U16 u16Len = strlen(ASC_CHAR_TABLE);
    MS_S16 idx = 0;
    for (; idx < u16Len; ++idx)
    {
        if (ASC_CHAR_TABLE[idx] == pStrText[0])
        {
            return idx;
        }
    }

    return -1;
}

static MS_S16 findUniCharIndex(const MS_U8* pStrText)
{
    MS_U16 u16Len = wstrlen(UNI_CHAR_TABLE);
    MS_S16 idx = 0;
    for (; idx < u16Len; idx=idx+2)
    {
        if (UNI_CHAR_TABLE[idx] == pStrText[0] && UNI_CHAR_TABLE[idx+1] == pStrText[1])
        {
            return idx >> 1;
        }
    }

    return -1;
}

static MS_U16 getASCStrTextTotalWidth(const char* pStrText)
{
    MS_U16 u16Total = 0;
    MS_U16 u16Len = strlen(pStrText);
    MS_U16 i = 0;
    for (; i < u16Len; ++i)
    {
        MS_S16 s16Idx = findASCCharIndex(&pStrText[i]);
        u16Total += ((s16Idx < 0) ? font_info.space_width: (font_info.pFontWidthTable[s16Idx] + font_info.interval_width));
    }

    return (u16Total > font_info.interval_width)?(u16Total - font_info.interval_width):0;
}

static MS_U16 getUniStrTextTotalWidth(const MS_U8* pStrText)
{
    MS_U16 u16Total = 0;
    MS_U16 u16Len = wstrlen(pStrText) >> 1;
    MS_U16 i = 0;
    for (; i < u16Len; ++i)
    {
        MS_S16 s16Idx = findUniCharIndex(&pStrText[i]);
        u16Total += ((s16Idx < 0) ? SPACE_WIDTH : (UCHAR_WIDTH_TABLE[s16Idx] + EACH_CHAR_INTERVAL));
    }

    return u16Total - EACH_CHAR_INTERVAL;
}

static MS_BOOL drawOneASCCharByIdx(MS_S16 s16Idx, MS_U16 X, MS_U16 Y, GFX_RgbColor color)
{
    if (s16Idx < 0)
    {
        return FALSE;
    }
    if(using_font == 0)
    {
        memcpy(Font32X32, &(font_info.pCharTable[s16Idx * 128]), 128);
        drawASCChar(X, Y, font_info.pFontWidthTable[s16Idx], color);
    }
    if(using_font == 1)
    {
            memcpy(Font16X16, &(font_info.pCharTable[s16Idx * 32]), 32);
            drawASCChar(X, Y,font_info.pFontWidthTable[s16Idx], color);
    }
    return TRUE;
}
static MS_BOOL drawOneUniCharByIdx(MS_S16 s16Idx, MS_U16 X, MS_U16 Y, GFX_RgbColor color)
{
    if (s16Idx < 0)
    {
        return FALSE;
    }

    memcpy(UNI_CHAR_ARRAY, &UNI_FONT_TABLE[s16Idx * 72], 72);
    drawUniChar(X, Y, UCHAR_WIDTH_TABLE[s16Idx], color);
    return TRUE;
}

static MS_BOOL drawASCStrText(const char* pStrText, MS_U16 X, MS_U16 Y, GFX_RgbColor color, TextAttrib eTextAttrib)
{
    switch (eTextAttrib)
    {
        case eTextAlignMiddle:
        {
            GOP_GwinFBAttr sFBInfo;
            MApi_GOP_GWIN_GetFBInfo(u8OsdLayerFbId, &sFBInfo);
            X=(g_u16Record_OsdCreate_W-TextTotalWidth)>>1;
            X=X+g_u16Record_OsdCreate_X;
            break;
        }

        case eTextAlignRight:
             X -= TextTotalWidth;
            break;

        case eTextAlignLeft:
        default:
            break;
    }

    MS_U16 u16Offset = lineOffset;
    MS_U16 u16Len = strlen(pStrText);
    MS_U16 i = 0;

    for(; i < u16Len; ++i)
    {
        X += u16Offset;
        MS_S16 s16Idx = findASCCharIndex(&pStrText[i]);
        drawOneASCCharByIdx(s16Idx, X, Y, color);
        u16Offset = (s16Idx < 0) ? font_info.space_width: (font_info.pFontWidthTable[s16Idx] + font_info.interval_width);
        lineOffset += u16Offset;

    }

    return TRUE;
}

static MS_BOOL drawUniStrText(const MS_U8* pStrText, MS_U16 X, MS_U16 Y, GFX_RgbColor color, TextAttrib eTextAttrib)
{
    switch (eTextAttrib)
    {
        case eTextAlignMiddle:
        {
            GOP_GwinFBAttr sFBInfo;
            MApi_GOP_GWIN_GetFBInfo(u8OsdLayerFbId, &sFBInfo);
            X=(g_u16Record_OsdCreate_W-TextTotalWidth)>>1;
            X=X+g_u16Record_OsdCreate_X;
            break;
        }

        case eTextAlignRight:
            X -= TextTotalWidth;
            break;

        case eTextAlignLeft:
        default:
            break;
    }

    MS_U16 u16Offset = lineOffset;
    MS_U16 u16Len = wstrlen(pStrText);
    MS_U16 i = 0;

    for(; i < u16Len; i += 2)
    {
        X += u16Offset;
        MS_S16 s16Idx = findUniCharIndex(&pStrText[i]);
        drawOneUniCharByIdx(s16Idx, X, Y, color);
        u16Offset = (s16Idx < 0) ? SPACE_WIDTH : (UCHAR_WIDTH_TABLE[s16Idx] + EACH_CHAR_INTERVAL);
        lineOffset += u16Offset;
    }

    return TRUE;
}

MS_S8 parseArgv(char* argv, MS_U8* outputbuffer)
{
    MS_S8 isASC = -1;
    MS_U16 id;
    MS_U32 hexmagic;
    MS_U32 decmagic;
    MS_U8 *cStr = NULL;
    MS_U16 k, j, u16Len;
    if (argv == NULL || outputbuffer == NULL)
    {
        return -1;
    }
    hexmagic = simple_strtoul(argv, NULL, 16);
    decmagic = simple_strtoul(argv, NULL, 10);
    if ((hexmagic & 0xffff) == ID_MAGIC || (decmagic & 0xffff) == ID_MAGIC)
    {
        if ((hexmagic & 0xffff) == ID_MAGIC)
        {
            id = (hexmagic >> 16) & 0xffff;
        }
        else
        {
            id = (decmagic >> 16) & 0xffff;
        }
        cStr = OSD_GetString(id);
        if (cStr == NULL)
        {
            return -1;
        }
        if (get_languageID() == ENGLISH)
        {
            k = 1;
            j = 0;
            while (((cStr[j] | (cStr[j + 1] << 8)) < 0xa0) && ((cStr[j] | (cStr[j + 1] << 8)) > 0x00))
            {
                cStr[k] = cStr[2 * k];
                k += 1;
                j += 2;
            }
            cStr[k - 1] = '\0';
            sprintf((char*)outputbuffer, "%s", cStr);
            isASC = ASC;
        }
        else
        {
            u16Len = wstrlen(cStr);
            for (k = 0; k < u16Len; k++)
            {
                outputbuffer[k] = cStr[k];
            }
            isASC = UNICODE;
        }
    }
    else
    {
        sprintf((char*)outputbuffer, "%s ", argv);
        isASC = ASC;
    }
    return isASC;

}

void MsApiDrawStrText(const char* pStrText, MS_U16 X, MS_U16 Y, GFX_RgbColor color, TextAttrib eTextAttrib)
{
    MS_S8 ret = -1;
    MS_U16 i;
    MS_U8 Strtext[100] = {0};
    char word[100] = {0};
    const char* pStr=pStrText;

    lineOffset = 0;
    TextTotalWidth = 0;
    GetFontInfo(&font_info,  using_font);
    while (*pStr != '\0')
    {
        for (i = 0; (*pStr != ' ' && *pStr != '\0'); i++)
        {
            word[i] = *pStr++;
        }
        word[i] = '\0';
        pStr++;

        ret = parseArgv(word, Strtext);
        if (ASC == ret)
        {
            TextTotalWidth += getASCStrTextTotalWidth((const char*)Strtext);
        }
        else if (UNICODE == ret)
        {
            TextTotalWidth += getUniStrTextTotalWidth(Strtext);
        }
        else
        {
            return;
        }
    }

    pStr = pStrText;
    while (*pStr != '\0')
    {
        for (i = 0; (*pStr != ' ' && *pStr != '\0'); i++)
        {
            word[i] = *pStr++;
        }
        word[i] = '\0';
        pStr++;

        ret = parseArgv(word, Strtext);
        if (ASC == ret)
        {
            drawASCStrText((const char*)Strtext, X, Y, color, eTextAttrib);
        }
        else if (UNICODE == ret)
        {
            drawUniStrText(Strtext, X, Y, color, eTextAttrib);
        }
        else
        {
            return;
        }
    }


}
void MsApiFlushCanvas2Screen(void)
{
    if (0xFF == u8OsdLayerFbId)
    {
        return;
    }

    GOP_GwinFBAttr sFBInfo;
    MApi_GOP_GWIN_GetFBInfo(u8OsdLayerFbId, &sFBInfo);

    GFX_Point v0 = {sFBInfo.x0, sFBInfo.y0};
    GFX_Point v1 = {sFBInfo.x1, sFBInfo.y1};
    MApi_GFX_SetClip(&v0, &v1);

    GFX_BufferInfo gfxDstBuf;
    gfxDstBuf.u32ColorFmt = (GFX_Buffer_Format)(sFBInfo.fbFmt & 0xFF);
    gfxDstBuf.u32Addr = sFBInfo.addr;
    gfxDstBuf.u32Width = sFBInfo.width;
    gfxDstBuf.u32Height = sFBInfo.height;
    gfxDstBuf.u32Pitch = sFBInfo.pitch;
    MApi_GFX_SetDstBufferInfo(&gfxDstBuf, 0);

    GFX_BufferInfo gfxSrcBuf;
    gfxSrcBuf.u32ColorFmt = (GFX_Buffer_Format)E_MS_FMT_YUV422;
    gfxSrcBuf.u32Addr = gu32CanvasBuffer;
    gfxSrcBuf.u32Width = CANVAS_WIDTH;
    gfxSrcBuf.u32Height = CANVAS_HEIGHT;
    gfxSrcBuf.u32Pitch = CANVAS_WIDTH << 1;
    MApi_GFX_SetSrcBufferInfo(&gfxSrcBuf, 0);

    GFX_DrawRect gfxBitbltInfo;
    gfxBitbltInfo.dstblk.height = sFBInfo.height;
    gfxBitbltInfo.dstblk.width = sFBInfo.width;
    gfxBitbltInfo.dstblk.x = 0;
    gfxBitbltInfo.dstblk.y = 0;

    gfxBitbltInfo.srcblk.height = CANVAS_HEIGHT;
    gfxBitbltInfo.srcblk.width = CANVAS_WIDTH;
    gfxBitbltInfo.srcblk.x = 0;
    gfxBitbltInfo.srcblk.y = 0;

    MApi_GFX_BitBlt(&gfxBitbltInfo, GFXDRAW_FLAG_SCALE);
    MApi_GFX_FlushQueue();

}

void MsApiOsdCreate(MS_U8 u8logoGopIdx, GFX_Block* pBlk, MS_PHYADDR GopBuffer)
{
	U32 i=0;
    U16* u16TempAddr = NULL;
    GFX_BufferInfo dst_info;

    MsDrvGFX_Init();
    MsDrvGOP_Init(u8logoGopIdx);

    g_u16Record_OsdCreate_W = pBlk->width;
    g_u16Record_OsdCreate_H = pBlk->height;
    g_u16Record_OsdCreate_X = pBlk->x;
    g_u16Record_OsdCreate_Y = pBlk->y;
    pBlk->width = g_IPanel.Width();
    pBlk->height = g_IPanel.Height();

    //set gop canvas address
    gu32CanvasBuffer=GopBuffer+(pBlk->width* g_IPanel.Height()*2);

    //init  gop canvas address
    u16TempAddr = (U16*)PA2NVA(gu32CanvasBuffer);
    for (i = 0; i < CANVAS_WIDTH * CANVAS_HEIGHT; ++i)
    {
        *(u16TempAddr++) = 0x8000;
    }
    //init gop display address
    u16TempAddr = (U16*)PA2NVA(GopBuffer);
    for (i = 0; i < pBlk->width *  pBlk->height; ++i)
    {
            *(u16TempAddr++) = 0x8000;
    }

    dst_info.u32Addr = GopBuffer;
    dst_info.u32Width = pBlk->width;
    dst_info.u32Height =  pBlk->height;
    dst_info.u32ColorFmt = GFX_FMT_YUV422;
    dst_info.u32Pitch = (pBlk->width<<1);

    u16OsdW = pBlk->width;
    u16OsdH = pBlk->height;

    st_sys_misc_setting misc_setting;
#if (CONFIG_LOGO_STORE_IN_MBOOT)
    GetOSDMirrorMode(&misc_setting);
#else
    Read_MiscSetting_ToFlash(&misc_setting);
#endif

    int mirrorMode=misc_setting.m_u8MirrorMode;
    if(1==mirrorMode)
    {
        printf("<<set_mirror V & H ON!!>> \n");
        MApi_GOP_GWIN_SetVMirror(TRUE);
        if(gbPnlModMirrorMode==FALSE)
        {
            MApi_GOP_GWIN_SetHMirror(TRUE);
        }
        else
        {
          printf("<<Skip H ON!!>> \n");
        }
    }
    else if(2==mirrorMode)
    {
        printf("<<set_mirror V ON!!>> \n");
        MApi_GOP_GWIN_SetVMirror(TRUE);
    }
    else if(3==mirrorMode)
    {
        if(gbPnlModMirrorMode==TRUE)
        {
            printf("<<set_mirror H ON!!>> \n");
            MApi_GOP_GWIN_SetHMirror(TRUE);
        }
        else
        {
            printf("<<MOD H_Mirror Enable !!>> \n");
        }
    }

    MsDrvGOP_Show(u8logoGopIdx, &dst_info,FALSE,FALSE,pBlk->x,pBlk->y);
}

void MsApiOsdDestroy(void)
{
    //Set OSD to black before destroy
    if (u16OsdW != 0 && u16OsdH != 0)
    {
        GFX_RgbColor color = {0, 0, 0, 0};
        GFX_Block dstBlk = {0, 0, u16OsdW, u16OsdH};
        MsApiDrawRect(&dstBlk, color);
        MsApiFlushCanvas2Screen();
        u16OsdW = 0;
        u16OsdH = 0;
    }

    if (u8OsdLayerGwinId != 0xFF)
    {
        MApi_GOP_GWIN_Enable(u8OsdLayerGwinId, FALSE);
        u8OsdLayerGwinId = 0xFF;
    }

    if (u8OsdLayerFbId != 0xFF)
    {
        MApi_GOP_GWIN_DestroyFB(u8OsdLayerFbId);
        u8OsdLayerFbId = 0xFF;
    }
}
void MsApiDrawProgress(MS_U16 X, MS_U16 Y, GFX_RgbColor fcolor, MS_U8 per)
{
    if (per > 100)
    {
        per = 100;
    }

    MS_U16 u16StartX = X;
    MS_U16 u16Total = (PROGRESS_NUM * PROGRESS_WIDTH) * per / 100;
    MS_U16 u16Count = u16Total / PROGRESS_WIDTH;

    //memset(ASC_CHAR_ARRAY, 0xFF, 96);
	memset(Font32X32, 0xFF, 128);
#if defined (CONFIG_URSA20_VB1) && defined(CONFIG_PNL_OLED_LG_SUPPORT)
    if(!IsTVOled())
#endif
    {

    MS_U16 i = 0;
    for (; i < u16Count; ++i)
    {
        drawASCChar(u16StartX, Y, PROGRESS_WIDTH, fcolor);
        u16StartX += (PROGRESS_WIDTH + PROGRESS_INTERVAL);
    }

    MS_U16 u16Left = u16Total % PROGRESS_WIDTH;
    if (u16Left != 0)
    {
        drawASCChar(u16StartX, Y, u16Left, fcolor);
        u16StartX += (u16Left + PROGRESS_INTERVAL);
    }

    char cStr[5] = {0};
    memset(cStr, 0, sizeof(cStr));
    snprintf(cStr, sizeof(cStr), "%d%%", per);
    lineOffset = 0;
    TextTotalWidth = 0;
    TextTotalWidth = getASCStrTextTotalWidth(cStr);
    drawASCStrText(cStr, X + PROGRESS_NUM * (PROGRESS_WIDTH + PROGRESS_INTERVAL), Y, fcolor, eTextAlignLeft);
    }
#if defined (CONFIG_URSA20_VB1) && defined(CONFIG_PNL_OLED_LG_SUPPORT)
    else //OLED
    {
            char cStr[5] = {0};
            memset(cStr, 0, sizeof(cStr));
            snprintf(cStr, sizeof(cStr), "%d%%", per);
            lineOffset = 0;
            TextTotalWidth = 0;
            TextTotalWidth = getASCStrTextTotalWidth(cStr);
            drawASCStrText(cStr, X + PROGRESS_NUM * (PROGRESS_WIDTH + PROGRESS_INTERVAL), Y, fcolor, eTextAlignMiddle);
    }
#endif

}

