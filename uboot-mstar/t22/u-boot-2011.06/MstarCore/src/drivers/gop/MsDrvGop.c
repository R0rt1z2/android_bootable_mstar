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
#include <config.h>
#include <MsOS.h>
#include <apiGOP.h>
#include <apiPNL.h>
#include <MsDebug.h>

#include <gop/MsDrvGop.h>
#include <uboot_mmap.h>
#include <ShareType.h>
#include <MsMmap.h>
#include <jpd/MsDrvJpd.h>

#include <bootlogo/MsPoolDB.h>
#include <MsEnvironment.h>
#include <panel/MsDrvPanel.h>
#include "MsBoot.h"

#ifndef UNUSED
#define UNUSED( var )                   ((void)(var))
#endif
U8 u8OsdLayerFbId=0xFF, u8OsdLayerGwinId=0xFF;
U8 u8OsdLayer2FbId=0xFF, u8OsdLayer2GwinId=0xFF;
U16 u16OsdW = 0,u16OsdH = 0;
U8 g_logoGopIdx = 0;
U8 g_logoMiuIdx = 0;
U8 bDelayLogo = FALSE;
U8 u8OsdLayerFbId_SC=0xFF, u8OsdLayerGwinId_SC=0xFF;
U8 g_logoGopIdx_SC = 1;

MS_BOOL _sc_is_interlace(void)
{
    return 0;
}

MS_U16 _sc_get_h_cap_start(void)
{
    return 0x60;
}
void _Sys_PQ_ReduceBW_ForOSD(MS_U8 PqWin, MS_BOOL bOSD_On)
{
    PqWin=bOSD_On=0;
    UNUSED(PqWin);
    UNUSED(bOSD_On);
}


#if ((CONFIG_TV_CHIP == 0) && (ENABLE_ANDROID_BOOT == 1))

MS_BOOL If_Reproduce_By_GWIN(st_reproduce_para *reproduce)
{
    // Gop can Not adjust Picture Position
    UBOOT_DEBUG("Reproduction rate Using GE\n");
    return FALSE;
}

MS_BOOL Load_ReproduceRate(st_reproduce_para *reproduce)
{
    char   *repro_env;
    char   *repro_mode;
    MS_U32 reproduce_val;

    UBOOT_TRACE("IN\n");
    if (isBootToRecovery())
    {
        UBOOT_DEBUG("Enter recovery, Skip Load_ReproduceRate!\n");
        UBOOT_TRACE("OK\n");
        return FALSE;
    }

    repro_env = getenv("reproducerate");
    repro_mode = getenv("reproducerate_mode");
    UBOOT_DEBUG("reproducerate      = %s\n", repro_env);
    UBOOT_DEBUG("reproducerate_mode = %s\n", repro_mode);

    if (repro_env == NULL)
    {
        UBOOT_DEBUG("reproducerate = NULL\n");
        UBOOT_TRACE("OK\n");
        return FALSE;
    }
    if ((repro_mode != NULL) && (strcmp(repro_mode, "percent") == 0))
    {
        reproduce->mode   = 1;
    }
    else
    {
        reproduce->mode   = 0;
    }

    reproduce_val         = simple_strtoul(repro_env, NULL, 0);
    reproduce->topRate    = (reproduce_val >> 24) & 0xFF;
    reproduce->bottomRate = (reproduce_val >> 16) & 0xFF;
    reproduce->leftRate   = (reproduce_val >>  8) & 0xFF;
    reproduce->rightRate  = (reproduce_val      ) & 0xFF;
    UBOOT_DEBUG("Reproduction rate : top(%d) bottom(%d) left(%d) right(%d)\n", reproduce->topRate,
                 reproduce->bottomRate, reproduce->leftRate, reproduce->rightRate);

    UBOOT_TRACE("OK\n");
    return TRUE;
}

MS_BOOL Set_Rerpoduce_Rect(MS_U16 *posX, MS_U16 *posY, MS_U32 *rect_width, MS_U32 *rect_height, st_reproduce_para *reproduce)
{
    UBOOT_TRACE("IN\n");
    UBOOT_DEBUG("======= Before =========\n");
    UBOOT_DEBUG("posX        = %d\n", (int)*posX);
    UBOOT_DEBUG("posY        = %d\n", (int)*posY);
    UBOOT_DEBUG("rect_width  = %d\n", (int)*rect_width);
    UBOOT_DEBUG("rect_height = %d\n", (int)*rect_height);

    MS_U8 leftRate   = reproduce->leftRate;
    MS_U8 topRate    = reproduce->topRate;
    MS_U8 rightRate  = reproduce->rightRate;
    MS_U8 bottomRate = reproduce->bottomRate;

    if (reproduce->mode == 1)
    {
        // Percent flow
        UBOOT_DEBUG("Using percent method\n");
        if ((leftRate > 100) || (topRate > 100) || (leftRate + rightRate > 100) || (topRate  + bottomRate > 100))
        {
            UBOOT_ERROR("Invalid reproduce parameter !\n");
            return FALSE;
        }
        *posX          =  (*rect_width)   * leftRate / 100;
        *posY          =  (*rect_height)  * topRate / 100;
        *rect_width   -=  ((*rect_width)  * (leftRate + rightRate) / 100);
        *rect_height  -=  ((*rect_height) * (topRate  + bottomRate) / 100);
    }
    else if (reproduce->mode == 0)
    {
        // Pixel flow
        UBOOT_DEBUG("Using pixel method\n");
        if ((leftRate > (*rect_width)) || (topRate > (*rect_height)) \
             || (leftRate + rightRate > (*rect_width)) || (topRate  + bottomRate > (*rect_height)))
        {
            UBOOT_ERROR("Invalid reproduce parameter !\n");
            return FALSE;
        }
        *posX          =  leftRate;
        *posY          =  topRate;
        *rect_width   -=  (leftRate + rightRate);
        *rect_height  -=  (topRate  + bottomRate);
    }
    UBOOT_DEBUG("======= After =========\n");
    UBOOT_DEBUG("posX        = %d\n", (int)*posX);
    UBOOT_DEBUG("posY        = %d\n", (int)*posY);
    UBOOT_DEBUG("rect_width  = %d\n", (int)*rect_width);
    UBOOT_DEBUG("rect_height = %d\n", (int)*rect_height);
    UBOOT_TRACE("OK\n");
    return TRUE;
}

#endif

void MsDrvGE_SetColorFmt(GFX_Buffer_Format *ColorFmt)
{
    #if defined(CONFIG_GFX_OUT_FMT_ABGR8888)
    *ColorFmt = GFX_FMT_ABGR8888;
    #elif defined(CONFIG_GFX_OUT_FMT_ARGB8888)
    *ColorFmt = GFX_FMT_ARGB8888;
    #else
    *ColorFmt = GFX_FMT_YUV422;
    #endif
}

int MsDrvGE_GetPixelByte(GFX_Buffer_Format ColorFmt)
{
    int bytespp;

    switch (ColorFmt)
    {
        case GFX_FMT_ABGR8888:
        case GFX_FMT_ARGB8888:
            bytespp = 4;
            break;
        case GFX_FMT_YUV422:
            bytespp = 2;
            break;
        default:
            bytespp = 0;
            break;
    }
    return bytespp;
}

MS_U32 MsDrvGE_GetDstPitch(MS_U32 src_pitch, GFX_Buffer_Format ColorFmt)
{
    MS_U32 dst_pitch;

    switch (ColorFmt)
    {
        case GFX_FMT_ABGR8888:
        case GFX_FMT_ARGB8888:
            dst_pitch = src_pitch << 2;
            break;
        case GFX_FMT_YUV422:
            dst_pitch = src_pitch << 1;
            break;
        default:
            dst_pitch = src_pitch;
            break;
    }
    return dst_pitch;
}

MS_U32 MsDrvGE_GetBlackColor(GFX_Buffer_Format ColorFmt)
{
    MS_U32 black_color;

    switch (ColorFmt)
    {
        case GFX_FMT_ABGR8888:
        case GFX_FMT_ARGB8888:
            black_color = 0;
            break;
        case GFX_FMT_YUV422:
            black_color = 0x8000;
            break;
        default:
            black_color = 0;
            break;
    }
    return black_color;
}

void MsDrvGOP_SetStretch(MS_U8 u8logoGopIdx,GFX_BufferInfo *dst_info, MS_BOOL bHorStretch, MS_BOOL bVerStretch,MS_U16 u16DispX,MS_U16 u16DispY)
{
    U32 u32GWinDispX_Max;
    U32 u32GWinDispY_Max;
    st_reproduce_para reproduce;

    u32GWinDispX_Max = g_IPanel.Width();
    u32GWinDispY_Max = g_IPanel.Height();

    MApi_GOP_GWIN_Set_HSCALE(bHorStretch, dst_info->u32Width, u32GWinDispX_Max);

    MApi_GOP_GWIN_Set_VSCALE(bVerStretch, dst_info->u32Height, u32GWinDispY_Max);

    MApi_GOP_GWIN_Set_STRETCHWIN(u8logoGopIdx, E_GOP_DST_OP0,u16DispX,u16DispY, dst_info->u32Width, dst_info->u32Height);
}


void gop_stretch(U16 graph_pitch, U16 graph_width, U16 graph_height, MS_BOOL bHorStretch, MS_BOOL bVerStretch)
{
    U32 u32GWinDispX_Max;
    U32 u32GWinDispY_Max;


    u32GWinDispX_Max = g_IPanel.Width();
    u32GWinDispY_Max = g_IPanel.Height();

    if (bHorStretch)
        MApi_GOP_GWIN_Set_HSCALE(TRUE, graph_width, u32GWinDispX_Max);
    if (bVerStretch)
        MApi_GOP_GWIN_Set_VSCALE(TRUE, graph_height, u32GWinDispY_Max);

    MApi_GOP_GWIN_Set_STRETCHWIN(g_logoGopIdx, E_GOP_DST_OP0, 0, 0, graph_pitch, graph_height);
}

void MsDrvGFX_Init(void)
{
    GFX_Config tGFXcfg;
    memset(&tGFXcfg,0,sizeof(tGFXcfg));
    tGFXcfg.bIsCompt = TRUE;
    tGFXcfg.bIsHK = TRUE;
    MApi_GFX_Init(&tGFXcfg);

}
int MsDrvGFX_Blt(GFX_DrawRect *rect,GFX_BufferInfo *src_info, GFX_BufferInfo *dst_info )
{
    GFX_Point gfxPt0 = { rect->dstblk.x, rect->dstblk.y };
    GFX_Point gfxPt1 = { rect->dstblk.x+rect->dstblk.width,rect->dstblk.y+rect->dstblk.height };
    GFX_Buffer_Format colorFmt = dst_info->u32ColorFmt;

    MApi_GFX_SetSrcBufferInfo(src_info, 0);

    MApi_GFX_SetDstBufferInfo(dst_info, 0);

    MApi_GFX_SetClip(&gfxPt0, &gfxPt1);

    if ((colorFmt == GFX_FMT_ABGR8888) || (colorFmt == GFX_FMT_ARGB8888))
    {
        MApi_GFX_SetDC_CSC_FMT(GFX_YUV_RGB2YUV_255, GFX_YUV_OUT_PC, GFX_YUV_IN_255, GFX_YUV_YUYV,  GFX_YUV_YUYV);
    }
    else
    {
        MApi_GFX_SetDC_CSC_FMT(GFX_YUV_RGB2YUV_PC, GFX_YUV_OUT_PC, GFX_YUV_IN_255, GFX_YUV_YVYU,  GFX_YUV_YUYV);
    }

    if( (rect->srcblk.width!= rect->dstblk.width)||(rect->srcblk.height!= rect->dstblk.height) )
    {
        MApi_GFX_BitBlt(rect, GFXDRAW_FLAG_SCALE);
    }
    else
    {
        MApi_GFX_BitBlt(rect, 0);
    }
    MApi_GFX_FlushQueue();

    return 1;
    //BitBlt to GOP SD Path

}
void MsDrvGOP_Init(MS_U8 u8logoGopIdx)
{
    UBOOT_TRACE("IN\n");
    // MS_U8 u8Idx;
    GOP_InitInfo pGopInit;
    //MApi_GOP_RegisterFBFmtCB(( U32(*)(MS_U16 pitch,U32 addr , MS_U16 fmt ))OSD_RESOURCE_SetFBFmt);
    MApi_GOP_RegisterXCIsInterlaceCB(_sc_is_interlace);
    MApi_GOP_RegisterXCGetCapHStartCB(_sc_get_h_cap_start);
    MApi_GOP_RegisterXCReduceBWForOSDCB(_Sys_PQ_ReduceBW_ForOSD);
    pGopInit.u16PanelWidth = g_IPanel.Width();
    pGopInit.u16PanelHeight = g_IPanel.Height();
    pGopInit.u16PanelHStr = g_IPanel.HStart();
    pGopInit.u32GOPRBAdr = 0x0;//((GOP_GWIN_RB_MEMORY_TYPE & MIU1) ? (GOP_GWIN_RB_ADR | MIU_INTERVAL) : (GOP_GWIN_RB_ADR));
    pGopInit.u32GOPRBLen = 0x0;

    //there is a GOP_REGDMABASE_MIU1_ADR for MIU1
    pGopInit.u32GOPRegdmaAdr = 0;//((GOP_REGDMABASE_MEMORY_TYPE & MIU1) ? (GOP_REGDMABASE_ADR | MIU_INTERVAL) : (GOP_REGDMABASE_ADR));
    pGopInit.u32GOPRegdmaLen = 0;
    pGopInit.bEnableVsyncIntFlip = FALSE;


    MApi_GOP_GWIN_SetForceWrite(TRUE);
    MApi_GOP_InitByGOP(&pGopInit, u8logoGopIdx);
    MApi_GOP_GWIN_SwitchGOP(u8logoGopIdx);

#if (ENABLE_HDMITX_MSTAR_ROCKET==1)
    MApi_GOP_GWIN_EnableT3DMode(FALSE);
#endif

     // read the MIU in which the bootlogo stored
    UBOOT_DEBUG("BOOTLOGO SET GOP INDEX[%d]\n",u8logoGopIdx);
    UBOOT_DEBUG("BOOTLOGO SET GOP MIU[%d]\n",g_logoMiuIdx);
    MApi_GOP_MIUSel(u8logoGopIdx, g_logoMiuIdx);

#if(CONFIG_TV_CHIP == 0)
    MApi_GOP_GWIN_OutputColor(GOPOUT_YUV);	//for BOX
#else
    MApi_GOP_GWIN_OutputColor(GOPOUT_RGB);  //TV
#endif

    char * p_str = getenv("GOP_SET_MUX");
    if(p_str != NULL)
    {
        int i = 0;
        int j = 0;
        U8 au8GopMux[4] = {0};
        GOP_MuxConfig stMuxCfg;
        UBOOT_DEBUG("GET Mux from ENV  =%s\n",p_str);
        memset(&stMuxCfg, 0, sizeof(GOP_MuxConfig));

        while(p_str[i] != '\0')
        {
            if((p_str[i] != ':') && (p_str[i] >= 0x30) &&(p_str[i] <= 0x39))
            {
                au8GopMux[j] = p_str[i] - 0x30;
                stMuxCfg.GopMux[j].u8GopIndex = au8GopMux[j];
                stMuxCfg.GopMux[j].u8MuxIndex = j;
                stMuxCfg.u8MuxCounts = ++j;
            }
            i++;
        }
        MApi_GOP_GWIN_SetMux(&stMuxCfg, sizeof(GOP_MuxConfig));
    }
    else
    {
        st_gopmux_para gopSetting_para;
        Read_Gop_Mux_ParaFromFlash(&gopSetting_para);
        if (gopSetting_para.m_u8SettingType == 1)
        {
            GOP_LayerConfig layerConfig;
            layerConfig.u32LayerCounts = gopSetting_para.m_u16SettingCounts;
            layerConfig.stGopLayer[0].u32LayerIndex = 0;
            layerConfig.stGopLayer[0].u32GopIndex = gopSetting_para.m_u16Setting0GopIndex;
            layerConfig.stGopLayer[1].u32LayerIndex = 1;
            layerConfig.stGopLayer[1].u32GopIndex = gopSetting_para.m_u16Setting1GopIndex;
            layerConfig.stGopLayer[2].u32LayerIndex = 2;
            layerConfig.stGopLayer[2].u32GopIndex = gopSetting_para.m_u16Setting2GopIndex;
            layerConfig.stGopLayer[3].u32LayerIndex = 3;
            layerConfig.stGopLayer[3].u32GopIndex = gopSetting_para.m_u16Setting3GopIndex;
            layerConfig.stGopLayer[4].u32LayerIndex = 4;
            layerConfig.stGopLayer[4].u32GopIndex = gopSetting_para.m_u16Setting4GopIndex;
            MApi_GOP_GWIN_SetLayer(&layerConfig, sizeof(GOP_LayerConfig));
        }
        else
        {
            GOP_MuxConfig muxConfig;
            muxConfig.u8MuxCounts = gopSetting_para.m_u16SettingCounts;
            muxConfig.GopMux[0].u8MuxIndex = EN_GOP_MUX0;
            muxConfig.GopMux[0].u8GopIndex = gopSetting_para.m_u16Setting0GopIndex;
            muxConfig.GopMux[1].u8MuxIndex = EN_GOP_MUX1;
            muxConfig.GopMux[1].u8GopIndex = gopSetting_para.m_u16Setting1GopIndex;
            muxConfig.GopMux[2].u8MuxIndex = EN_GOP_MUX2;
            muxConfig.GopMux[2].u8GopIndex = gopSetting_para.m_u16Setting2GopIndex;
            muxConfig.GopMux[3].u8MuxIndex = EN_GOP_MUX3;
            muxConfig.GopMux[3].u8GopIndex = gopSetting_para.m_u16Setting3GopIndex;
            muxConfig.GopMux[4].u8MuxIndex = EN_GOP_MUX4;
            muxConfig.GopMux[4].u8GopIndex = gopSetting_para.m_u16Setting4GopIndex;
            MApi_GOP_GWIN_SetMux(&muxConfig,sizeof(muxConfig));
        }
    }
    /* avoiding the fail log in getwinfb func while in init stage
    for(u8Idx=0;u8Idx< MApi_GOP_GWIN_GetGwinNum(g_logoGopIdx);u8Idx++)
    {
        MApi_GOP_GWIN_DeleteWin(u8Idx);
    }
    */
    MApi_GOP_GWIN_SetForceWrite(FALSE);
    UBOOT_TRACE("OK\n");
}

void MsDrvGOP_Show(MS_U8 u8logoGopIdx, GFX_BufferInfo *dst_info,MS_BOOL bHorStretch, MS_BOOL bVerStretch,MS_U16 u16DispX,MS_U16 u16DispY)
{
    UBOOT_TRACE("IN\n");
    MS_U32 u32Width;
    MS_U32 u32pixelmode=FALSE;
    EN_GOP_CONSALPHA_BITS enGOPConsAlphaBit;
    MApi_GOP_GWIN_SetForceWrite(TRUE);
    MApi_GOP_GWIN_SwitchGOP(u8logoGopIdx);

#if  (CONFIG_TV_CHIP==1)
    if ((LINK_VBY1_10BIT_16LANE == getLinkExtType()) ||
        (LINK_VBY1_8BIT_16LANE == getLinkExtType())
        )
    {
        MApi_GOP_GWIN_SetGOPDst(u8logoGopIdx, E_GOP_DST_OP_DUAL_RATE);
    }
    else
    {
        MApi_GOP_GWIN_SetGOPDst(u8logoGopIdx, E_GOP_DST_OP0);
    }
#else

    MApi_GOP_GWIN_SetGOPDst(u8logoGopIdx, E_GOP_DST_OP0);

#endif

    MApi_GOP_GetChipCaps(E_GOP_CAP_PIXELMODE_SUPPORT, & u32pixelmode,sizeof(u32pixelmode));
    if (u32pixelmode==FALSE)//word mode
    {
        //ARGB8888: 4
        //ARGB4444: 2
        //YUV422: 2
        MS_U32 u32WordUnit=0;
        MS_U32 aligement =0;
        MS_U32 bytespp = 0;
        MApi_GOP_GetChipCaps(E_GOP_CAP_WORD_UNIT , & u32WordUnit, sizeof(MS_U32));
        bytespp = MsDrvGE_GetPixelByte(dst_info->u32ColorFmt);
        if (bytespp == 0)
        {
            UBOOT_ERROR("MsDrvGE_GetPixelByte Error\n");
            bytespp = 2;
        }

        aligement = u32WordUnit/ bytespp;   //YUV422: 2
        u32Width=ALIGN(dst_info->u32Width, aligement);
        dst_info->u32Width=u32Width;
        UBOOT_DEBUG("word mode width 0x%x \n",(unsigned int)u32Width);
    }
    else //pixel mode
    {
        UBOOT_DEBUG("pixel mode width 0x%x \n",(unsigned int)dst_info->u32Width);
    }
    MsDrvGOP_SetStretch(u8logoGopIdx,dst_info, bHorStretch, bVerStretch,u16DispX,u16DispY);


    u8OsdLayerFbId = MApi_GOP_GWIN_GetFreeFBID();
    u8OsdLayerGwinId = MApi_GOP_GWIN_GetFreeWinID();

    MApi_GOP_GWIN_CreateFBbyStaticAddr(u8OsdLayerFbId, 0, 0, dst_info->u32Width, dst_info->u32Height, dst_info->u32ColorFmt, dst_info->u32Addr);

    MApi_GOP_GWIN_MapFB2Win(u8OsdLayerFbId, u8OsdLayerGwinId);
    //jh patch for k3 need fix after sn has right dac panel ini
    MApi_GOP_GetChipCaps(E_GOP_CAP_CONSALPHA_VALIDBITS, &enGOPConsAlphaBit,sizeof(enGOPConsAlphaBit));
    MApi_GOP_SetConfig(E_GOP_CONSALPHA_BITS, &enGOPConsAlphaBit);
    MApi_GOP_GWIN_SetBlending(u8OsdLayerGwinId, FALSE, 0xFF);

    MApi_GOP_GWIN_SetForceWrite(FALSE);

#if (CONFIG_DISPLAY_LOGO_SEAMLESS == 1)
    char* pu8str = getenv("hdr_status");
    if(pu8str != NULL)
    {
        MS_U8 u8HDRType = (int)simple_strtol(pu8str, NULL, 0);
        if(u8HDRType == 0)
        {
            UBOOT_TRACE("SDR GOP.\n");
            MApi_GOP_SetSDR2HDR(GOP_SDR2HDR_OFF);
        }
        else if(u8HDRType == 1)
        {
            UBOOT_TRACE("HDR GOP.\n");
            MApi_GOP_SetSDR2HDR(GOP_SDR2HDR_HDR10);
        }
        else
        {
            UBOOT_TRACE("[ERROR] HDR Stauts Error. \n");
        }
    }
    else
    {
        UBOOT_TRACE("[ERROR] No HDR Status. \n");
    }
#endif
    MApi_GOP_GWIN_Enable(u8OsdLayerGwinId, TRUE);
    UBOOT_TRACE("OK\n");
}

void MsDrvGOP_Show1(MS_U8 u8logoGopIdx,GFX_BufferInfo *dst_info , MS_U8 u8HorStretch, MS_U8 u8VerStretch,MS_U16 u16DispX,MS_U16 u16DispY,MS_U32 u32HorStretch_After_scaling , MS_U32 u32VorStretch_After_scaling )
{
    MS_U32 u32Width;
    MS_U32 u32pixelmode=FALSE;
    UBOOT_TRACE("IN\n");
    EN_GOP_CONSALPHA_BITS enGOPConsAlphaBit;
    MApi_GOP_GWIN_SetForceWrite(TRUE);
    MApi_GOP_GWIN_SwitchGOP(u8logoGopIdx);

    MApi_GOP_GWIN_Set_HSCALE(TRUE, u8HorStretch, u32HorStretch_After_scaling);
    MApi_GOP_GWIN_Set_VSCALE(TRUE, u8VerStretch, u32VorStretch_After_scaling);
    MApi_GOP_GWIN_Set_STRETCHWIN(u8logoGopIdx, E_GOP_DST_OP0,u16DispX,u16DispY, u8HorStretch, u8VerStretch);

    MApi_GOP_GWIN_SetGOPDst(u8logoGopIdx, E_GOP_DST_OP0);

    u8OsdLayer2FbId = MApi_GOP_GWIN_GetFreeFBID();
    u8OsdLayer2GwinId = MApi_GOP_GWIN_GetFreeWinID();
    MApi_GOP_GetChipCaps(E_GOP_CAP_PIXELMODE_SUPPORT, & u32pixelmode,sizeof(MS_U32));
    if (u32pixelmode==FALSE)//word mode
    {
        //ARGB8888: 4
        //ARGB4444: 2
        //YUV422: 2
        MS_U32 u32WordUnit=0;
        MS_U32 aligement =0;
        MApi_GOP_GetChipCaps(E_GOP_CAP_WORD_UNIT , & u32WordUnit, sizeof(u32WordUnit));
        aligement =u32WordUnit/4;   //ARGB8888: 4
        u32Width=ALIGN(dst_info->u32Width, aligement);
        dst_info->u32Width=u32Width;
        UBOOT_DEBUG("word mode width 0x%x \n",(unsigned int)u32Width);
    }
    else //pixel mode
    {
        UBOOT_DEBUG("pixel mode width 0x%x \n",(unsigned int)dst_info->u32Width);
    }
    MApi_GOP_GWIN_CreateFBbyStaticAddr(u8OsdLayer2FbId, 0, 0, dst_info->u32Width, dst_info->u32Height, E_MS_FMT_ARGB8888, dst_info->u32Addr);

    MApi_GOP_GWIN_MapFB2Win(u8OsdLayer2FbId, u8OsdLayer2GwinId);
    MApi_GOP_GetChipCaps(E_GOP_CAP_CONSALPHA_VALIDBITS, &enGOPConsAlphaBit,sizeof(enGOPConsAlphaBit));
    MApi_GOP_SetConfig(E_GOP_CONSALPHA_BITS, &enGOPConsAlphaBit);
    MApi_GOP_GWIN_SetBlending(u8OsdLayer2GwinId, TRUE, 0xFF);  //TV

    MApi_GOP_GWIN_SetForceWrite(FALSE);
    MApi_GOP_GWIN_Enable(u8OsdLayer2GwinId, TRUE);
    UBOOT_TRACE("OK\n");
}


#if (CONFIG_BOOTVIDEO == 1)
static MS_BOOL _MsDrvGOP_EnableDwinCap(MS_BOOL bCfgOnly)
{
    MMapInfo_s stMmapInfo;
    GOP_DwinProperty stDwinProperty;
    E_GOP_API_Result eDwinRet;

    memset(&stMmapInfo, 0, sizeof(MMapInfo_s));
    if(get_mmap("E_DFB_FRAMEBUFFER", &stMmapInfo)!=0)
    {
        UBOOT_ERROR("get E_DFB_FRAMEBUFFER fail\n");
        return FALSE;
    }

    // GOP DWIN init
    eDwinRet = MApi_GOP_DWIN_Init();
    if(eDwinRet != GOP_API_SUCCESS)
    {
        UBOOT_ERROR("Error 0x%x: failed to MApi_GOP_DWIN_Init()\n", eDwinRet);
        return FALSE;
    }

    //Source can only from Scaler OP
    eDwinRet = MApi_GOP_DWIN_SetSourceSel(DWIN_SRC_OP);
    if(eDwinRet != GOP_API_SUCCESS)
    {
        UBOOT_ERROR("Error 0x%x: failed to MApi_GOP_DWIN_SetSourceSel(DWIN_SRC_OP)\n", eDwinRet);
        return FALSE;
    }

    //OP scan type is progressive
    eDwinRet = MApi_GOP_DWIN_SelectSourceScanType(DWIN_SCAN_MODE_PROGRESSIVE);
    if(eDwinRet != GOP_API_SUCCESS)
    {
        UBOOT_ERROR("Error 0x%x: failed to MApi_GOP_DWIN_SelectSourceScanType(DWIN_SCAN_MODE_PROGRESSIVE)\n", eDwinRet);
        return FALSE;
    }

    //Data format only YUV; Set data format to YUV422
    eDwinRet = MApi_GOP_DWIN_SetDataFmt(DWIN_DATA_FMT_UV8Y8);
    if(eDwinRet != GOP_API_SUCCESS)
    {
        UBOOT_ERROR("Error 0x%x: failed to MApi_GOP_DWIN_SetDataFmt(DWIN_DATA_FMT_UV8Y8)\n", eDwinRet);
        return FALSE;
    }

    memset(&stDwinProperty, 0, sizeof(GOP_DwinProperty));
    // Set DWin property
    stDwinProperty.u16x = 0;
    stDwinProperty.u16y = 0;
    stDwinProperty.u16fbw = g_IPanel.Width();
    stDwinProperty.u16w = g_IPanel.Width();
    stDwinProperty.u16h = g_IPanel.Height();
    stDwinProperty.u32fbaddr0 = (MS_U32)stMmapInfo.u64Addr;
    stDwinProperty.u32fbaddr1 = (MS_U32)stMmapInfo.u64Addr + stMmapInfo.u32Size;
    eDwinRet = MApi_GOP_DWIN_SetWinProperty(&stDwinProperty);
    if(eDwinRet != GOP_API_SUCCESS)
    {
        UBOOT_ERROR("Error 0x%x: failed to MApi_GOP_DWIN_SetWinProperty()!\n", eDwinRet);
        return FALSE;
    }

    if(bCfgOnly)
        return TRUE;

    // start DWIN capture
    eDwinRet = MApi_GOP_DWIN_Enable(TRUE);
    if(eDwinRet != GOP_API_SUCCESS)
    {
        UBOOT_ERROR("Error 0x%x: failed to MApi_GOP_DWIN_Enable(TRUE)!\n", eDwinRet);
        return FALSE;
    }

    return TRUE;
}

void MsDrvGOP_Hide(void)
{
    if(u8OsdLayerGwinId == 0xFF)
        return;

    // Prepare DWIN capture
    //_MsDrvGOP_EnableDwinCap(TRUE);

    // hide bootlogo
    MApi_GOP_GWIN_Enable(u8OsdLayerGwinId, FALSE);

}
#endif  // #if (CONFIG_BOOTVIDEO == 1)

#if CONFIG_MINIUBOOT
#else
#if (ENABLE_DISPLAY_LOGO) // GEGOP
int do_destroy_logo (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    if (u8OsdLayerGwinId != 0xFF)
    {
        MApi_GOP_GWIN_Set_HSCALE(FALSE, 0, 0);
        MApi_GOP_GWIN_Set_VSCALE(FALSE, 0, 0);
        MApi_GOP_GWIN_Enable(u8OsdLayerGwinId, FALSE);
        MApi_GOP_GWIN_DestroyWin(u8OsdLayerGwinId);
        u8OsdLayerGwinId = 0xFF;
    }

    if (u8OsdLayerFbId != 0xFF)
    {
        MApi_GOP_GWIN_DestroyFB(u8OsdLayerFbId);
        u8OsdLayerFbId = 0xFF;
    }
    return 0;
}

U_BOOT_CMD(
    destroy_logo,    1,   1,  do_destroy_logo,
    "destroy_logo - Stop logo display \n",
    "Stop logo display\n"
);
#endif
#endif
