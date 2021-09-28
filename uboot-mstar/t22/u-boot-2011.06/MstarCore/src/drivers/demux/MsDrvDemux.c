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

#if 1//(CONFIG_COMMANDS & CFG_CMD_DMX)
#include <MsTypes.h>
#include <MsDebug.h>
#include <drvMMIO.h>
#include "MsDrvDemux.h"
#include <MsSystem.h>


#define TSP_FW_SIZE             0x4000
#define TSP_SECT_BUF_SIZE     8192

#define DRV_DMX_MAX_FILE_IN_SIZE 0xF00000 //15MB

#define _SECT_WORD12(p, i)      ((((MS_U16)p[i] & 0x0f ) << 8) | p[i+1])
#define _SECT_WORD16(p, i)      (((MS_U16)p[i] << 8) | p[i+1])

///enum
typedef enum
{
    E_STREAM_TYPE_UNKNOWN             = 0x01,
    E_STREAM_TYPE_MPEG1_VID           = 0x01,
    E_STREAM_TYPE_MPEG2_VID           = 0x02,
    E_STREAM_TYPE_MPEG1_AUD           = 0x03,
    E_STREAM_TYPE_MPEG2_AUD           = 0x04,
    E_STREAM_TYPE_PRIVATE_DATA        = 0x06,
    E_STREAM_TYPE_DSMCC_DATA_TYPE_A   = 0x0A,
    E_STREAM_TYPE_DSMCC_DATA_TYPE_B   = 0x0B,
    E_STREAM_TYPE_DSMCC_DATA_TYPE_C   = 0x0C,
    E_STREAM_TYPE_DSMCC_DATA_TYPE_D   = 0x0D,
    E_STREAM_TYPE_DSMCC_DATA_TYPE_E   = 0x0E,
    E_STREAM_TYPE_AAC_AUD               = 0x0F,
    E_STREAM_TYPE_MPEG4_AUD           = 0x11,
    E_STREAM_TYPE_AVCH264_VID        = 0x1B,
    E_STREAM_TYPE_AVS                       = 0x42,
    E_STREAM_TYPE_OP_MEPG2_VID      = 0x80,
    E_STREAM_TYPE_LPCM_AUD             = 0x80,          ///< BD defined
    E_STREAM_TYPE_AC3_AUD               = 0x81,
    E_STREAM_TYPE_AC3P_AUD             = 0x84,          ///< BD defined
    E_STREAM_TYPE_EAC3_AUD             = 0x87,
    E_STREAM_TYPE_VC1_VID                = 0xEA,
} E_StreamType;


static MS_U8 u8TSFwBuf[TSP_FW_SIZE + 128] __attribute__((aligned (8)));

#if defined(CONFIG_URANUS4) || defined(CONFIG_KRONUS) || defined(CONFIG_KAISERIN) || defined(CONFIG_KENYA) || defined(CONFIG_KRIS)
#define TSP_VQ_SIZE 0x1900
static MS_U8 u8TSVQBuf[TSP_FW_SIZE + 16];
#endif

static MS_U8 u8TSPFirmware[] = {
    #include <fwTSP.dat>
};


static MS_BOOL _bDmxInited = FALSE;
static DMX_FLOW_INPUT _eCurDmxSrc = DMX_FLOW_INPUT_DEMOD;

#if (CONFIG_BOOTVIDEO == 1)
static MS_U8 _u8SecBuf[TSP_SECT_BUF_SIZE] __attribute__((aligned (8)));
static MS_BOOL _bDmxPcrEnable = FALSE;

// open av pid filter, source is the 1st parameter of MsDrv_DmxInit()
static int _MsDrv_DmxOpenAVFilter(DMX_FLOW_INPUT eDmxSrc, DMX_FILTER_TYPE u32DmxFiltType, MS_U16 u16Pid, MS_U8 *pu8DmxId)
{
    if(pu8DmxId)
        *pu8DmxId = DMX_DMXID_NULL;

    if(_bDmxInited == FALSE)
    {
        UBOOT_ERROR("DeMux>>ERROR! DMX hasn't be inited fail\n");
        return -1;
    }

    MS_U8 u8DmxId = DMX_DMXID_NULL;
    u16Pid &= 0x1FFF;

    if(_eCurDmxSrc == DMX_FLOW_INPUT_MEM)
    {
        u32DmxFiltType |= DMX_FILTER_SOURCE_TYPE_FILE;
    }
    else
    {
        u32DmxFiltType |= DMX_FILTER_SOURCE_TYPE_LIVE;
    }

    if(MApi_DMX_Open(u32DmxFiltType, &u8DmxId) != DMX_FILTER_STATUS_OK)
    {
        UBOOT_ERROR("DeMux>>ERROR! failed to MApi_DMX_Open(src = %d, filt_type = 0x%lx) fail\n", eDmxSrc, u32DmxFiltType);
        return -1;
    }

    if(MApi_DMX_Pid(u8DmxId, &u16Pid, TRUE) != DMX_FILTER_STATUS_OK)
    {
        UBOOT_ERROR("DeMux>>ERROR! failed to MApi_DMX_Pid(src = %d, , filt_type = 0x%lx, Pid = 0x%x) fail\n", eDmxSrc, u32DmxFiltType, u16Pid);
        goto OPEN_AV_FILTER_FAILED;
    }

    if(MApi_DMX_Start(u8DmxId) != DMX_FILTER_STATUS_OK)
    {
        UBOOT_ERROR("DeMux>>ERROR! failed to MApi_DMX_Start(src = %d, filt_type = 0x%lx) fail\n", eDmxSrc, u32DmxFiltType);
        goto OPEN_AV_FILTER_FAILED;
    }

    if(pu8DmxId)
        *pu8DmxId = u8DmxId;

    return 0;

OPEN_AV_FILTER_FAILED:

    if(u8DmxId != DMX_DMXID_NULL)
    {
        MApi_DMX_Stop(u8DmxId);
        MApi_DMX_Close(u8DmxId);
        u8DmxId = DMX_DMXID_NULL;
    }

    return -1;
}

static MsDrv_VIDEO_CodecType_e _MsDrv_DmxVidStrmType2CodecType(MS_U8 u8VidStrmType, MS_BOOL bHdmv)
{

    MsDrv_VIDEO_CodecType_e eVidCodecType = E_MSDRV_VIDEO_CODEC_TYPE_NONE;
    switch(u8VidStrmType)
    {
        case E_STREAM_TYPE_MPEG1_VID:
        case E_STREAM_TYPE_MPEG2_VID:
            eVidCodecType = E_MSDRV_VIDEO_CODEC_TYPE_MPEG2;
            break;

        case E_STREAM_TYPE_OP_MEPG2_VID:
            if(bHdmv == FALSE)
            {
                eVidCodecType = E_MSDRV_VIDEO_CODEC_TYPE_MPEG2;
                break;
            }

            // unknown
            UBOOT_ERROR("Error: unknown or unsupport stream_type = 0x%x, HDMV = %d !\n", u8VidStrmType, bHdmv);
            break;

        case E_STREAM_TYPE_AVCH264_VID:
            eVidCodecType = E_MSDRV_VIDEO_CODEC_TYPE_H264;
            break;

        case E_STREAM_TYPE_VC1_VID:
            eVidCodecType = E_MSDRV_VIDEO_CODEC_TYPE_VC1_ADV;
            break;

        case E_STREAM_TYPE_AVS:
            eVidCodecType = E_MSDRV_VIDEO_CODEC_TYPE_AVS;
            break;

        default:
            UBOOT_ERROR("Error: unknown or unsupport stream_type = 0x%x, HDMV = %d !\n", u8VidStrmType, bHdmv);
            break;
    }

    return eVidCodecType;
}

static MsDrv_AUDIO_CodecType_e _MsDrv_DmxAudStrmType2CodecType(MS_U8 u8AudStrmType, MS_BOOL bHdmv)
{
    MsDrv_AUDIO_CodecType_e eAudCodecType = E_MSDRV_AUDIO_CODEC_TYPE_NONE;
    switch(u8AudStrmType)
    {
        case E_STREAM_TYPE_MPEG1_AUD:
        case E_STREAM_TYPE_MPEG2_AUD:
            eAudCodecType = E_MSDRV_AUDIO_CODEC_TYPE_MPEG;
            break;

        case E_STREAM_TYPE_AC3_AUD:
            eAudCodecType = E_MSDRV_AUDIO_CODEC_TYPE_AC3;
            break;

        case E_STREAM_TYPE_AAC_AUD:
        case E_STREAM_TYPE_MPEG4_AUD:
            eAudCodecType = E_MSDRV_AUDIO_CODEC_TYPE_AAC;
            break;

        case E_STREAM_TYPE_LPCM_AUD:
            if(bHdmv)
            {
                eAudCodecType = E_MSDRV_AUDIO_CODEC_TYPE_XPCM;
                break;
            }

            // unknown
            UBOOT_ERROR("Error: unknown or unsupport stream_type = 0x%x, HDMV = %d !\n", u8AudStrmType, bHdmv);
            break;

        case E_STREAM_TYPE_AC3P_AUD:
            if(bHdmv)
            {
                eAudCodecType = E_MSDRV_AUDIO_CODEC_TYPE_AC3P;
                break;
            }

            // unknown
            UBOOT_ERROR("Error: unknown or unsupport stream_type = 0x%x, HDMV = %d !\n", u8AudStrmType, bHdmv);
            break;

        case E_STREAM_TYPE_EAC3_AUD:
            eAudCodecType = E_MSDRV_AUDIO_CODEC_TYPE_AC3P;
            break;

        default:
            UBOOT_ERROR("Error: unknown or unsupport stream_type = 0x%x, HDMV = %d !\n", u8AudStrmType, bHdmv);
            break;
    }

    return eAudCodecType;
}

static MS_BOOL _MsDrv_DmxCheckHdmvDesccriptor(MS_U8 *pu8Data, MS_U32 u32Length)
{
    if((pu8Data == NULL) || (u32Length <3))
    {
        //ERRF("Error: %s(%d) got invalid parameters(pu8Data = 0x%lx, u32Length = %ld)!!!\n", __FUNCTION__, __LINE__, (MI_U32)pu8Data, u32Length);
        return FALSE;
    }

    int nReLen = u32Length;
    MS_U8 u8DescTag;    // descriptor_tag
    MS_U8 u8DescLen;    // descriptor_length
    MS_U8 *pu8Desc = pu8Data;

    while(nReLen > 2)
    {
        u8DescTag = pu8Desc[0];
        u8DescLen = pu8Desc[1];
        if(u8DescTag == 0x05)   // registration descriptor
        {
            // registration descriptor
            // p[2], p[3], p[4], p[5] is format_identifier
            if(strncmp((const char *)&pu8Desc[2], "HDMV", 4) == 0)
            {
                // HDMV registration descriptor
                UBOOT_INFO("HDMV file format\n");
                return TRUE;
            }
        }

        nReLen -= (u8DescLen + 2);
        pu8Desc += (u8DescLen + 2);
    }

    return FALSE;
}


static MS_U8 _MsDrv_DmxCheckAudioType(MS_U8 *pu8Data, MS_U32 u32Length)
{
    if((pu8Data == NULL) || (u32Length < 3))
        return 0;

    int nReLen = u32Length;
    MS_U8 u8DescTag;    // descriptor_tag
    MS_U8 u8DescLen;    // descriptor_length
    // audio_type,
    // 0x00: Undefined --> main audio
    // 0x01: Clean effects
    // 0x02: Hearing impaired
    // 0x03: Visual impaired commentary --> AD audio
    MS_U8 u8AudType = 0;    // default is main audio
    MS_U8 *pu8Desc = pu8Data;


    while(nReLen > 2)
    {
        u8DescTag = pu8Desc[0];
        u8DescLen = pu8Desc[1];
        if(u8DescTag == 0x0A)   // ISO_639_language desriptor
        {
            if(u8DescLen >= 4)
            {
                u8AudType = pu8Desc[5];
                break;  // get audio_type only in the first loop.
            }
        }

        nReLen -= (u8DescLen + 2);
        pu8Desc += (u8DescLen + 2);
    }

    return u8AudType;
}


static MS_BOOL _MsDrv_DmxCheckAudioDescriptors(MS_U8 *pu8Data, MS_U32 u32Length, MS_U8 *pu8AuxStrmType)
{
    if((pu8Data == NULL) || (u32Length < 3))
        return FALSE;

    int nReLen = u32Length;
    MS_U8 u8DescTag;    // descriptor_tag
    MS_U8 u8DescLen;    // descriptor_length
    MS_U8 u8AuxStrmType = E_STREAM_TYPE_UNKNOWN;    // auxiliary stream type for audio with stream_type private data
    MS_U8 *pu8Desc = pu8Data;

    while(nReLen > 2)
    {
        u8DescTag = pu8Desc[0];
        u8DescLen = pu8Desc[1];
        if(u8DescTag == 0x6A)   // AC-3 desriptor
        {
            u8AuxStrmType = E_STREAM_TYPE_AC3_AUD;
            break;
        }
        else if(u8DescTag == 0x7A)  // E-AC3 descriptor
        {
            u8AuxStrmType = E_STREAM_TYPE_EAC3_AUD;
            break;
        }

        nReLen -= (u8DescLen + 2);
        pu8Desc += (u8DescLen + 2);
    }

    if(pu8AuxStrmType)
        *pu8AuxStrmType = u8AuxStrmType;

    return (u8AuxStrmType != E_STREAM_TYPE_UNKNOWN);
}

static int _MsDrv_DmxParsePmt(MS_U8 *pu8Data, MsDrv_DmxProg_t *pstProg)
{
    MS_U8 u8TableId;
    MS_S32 s32LoopLen;
    MS_S16 u16ProgNum;
    MS_U32 u32ProgInfoLen;
    MS_U8 *pu8Loop;
    MS_U8 u8StrmType;
    MS_U16 u16Pid;
    MS_U32 u32TempLoopLen;
    MS_U8 u8AudioType;
    MS_BOOL bHdmv;

    if(pstProg == NULL)
        return -1;

    memset(pstProg, 0, sizeof(MsDrv_DmxProg_t));

    u8TableId = pu8Data[0];
    if(u8TableId != 0x02)
    {
        UBOOT_ERROR("DeMux>>ERROR! Wrong table ID = 0x%x for PMT!\n", u8TableId);
        return -1;
    }

    s32LoopLen = _SECT_WORD12(pu8Data, 1);
    if (12 > s32LoopLen)
    {
        UBOOT_ERROR("DeMux>>ERROR! got too small section length = %ld!!!\n", s32LoopLen);
        return -1;
    }

    u16ProgNum = (MS_U16)_SECT_WORD16(pu8Data, 3);
    pstProg->u16PcrPid = ((MS_U16)_SECT_WORD16(pu8Data, 8)) & 0x1FFF;

    u32ProgInfoLen = _SECT_WORD12(pu8Data, 10);
    if ((13 + u32ProgInfoLen)> s32LoopLen)
    {
        UBOOT_ERROR("DeMux>>ERROR! illegal program_info_length = %ld, section length = %ld\n", u32ProgInfoLen, s32LoopLen);
        return -1;
    }

    bHdmv = _MsDrv_DmxCheckHdmvDesccriptor(&pu8Data[12], u32ProgInfoLen);

    s32LoopLen -= 9; // PMT header
    s32LoopLen -= u32ProgInfoLen; // program_info_length
    s32LoopLen -= 4; // CRC

    pu8Loop = pu8Data + 12 + u32ProgInfoLen;

    while (5 <= s32LoopLen)
    {
        u8StrmType = pu8Loop[0];
        u16Pid = ((MS_U16)_SECT_WORD16(pu8Loop, 1)) & 0x1FFF;
        u32TempLoopLen = (MS_U32)_SECT_WORD12(pu8Loop, 3);

        switch (u8StrmType) // stream type
        {
        // video
        case E_STREAM_TYPE_AVCH264_VID:
        case E_STREAM_TYPE_MPEG1_VID:
        case E_STREAM_TYPE_MPEG2_VID:
        case E_STREAM_TYPE_AVS:
            if(pstProg->u16VideoPid == 0)
            {
                pstProg->u16VideoPid = u16Pid;
                pstProg->eVdecCodecType = _MsDrv_DmxVidStrmType2CodecType(u8StrmType, bHdmv);
            }

            break;
        case E_STREAM_TYPE_PRIVATE_DATA:
            // need to check descriptor
            if(_MsDrv_DmxCheckAudioDescriptors(pu8Loop + 5, u32TempLoopLen, &u8StrmType))// auxiliary stream type for audio with stream_type private data
            {
                // fall through to audio stream type checking flow
                goto FALL_THROUGH_AUDIO;
            }

            break;
FALL_THROUGH_AUDIO:
        // Audio
        case E_STREAM_TYPE_MPEG1_AUD:
            // MPEG-1 Audio
        case E_STREAM_TYPE_MPEG2_AUD:
            // MPEG-2 Audio
        case E_STREAM_TYPE_AC3_AUD:
            // 13818-7 AAC
        case E_STREAM_TYPE_AAC_AUD:
            // 14496-3 MPEG4
        case E_STREAM_TYPE_MPEG4_AUD:
            u8AudioType = _MsDrv_DmxCheckAudioType(pu8Loop + 5, u32TempLoopLen);
            if(u8AudioType == 0x03) // Visual impaired commentary
            {
                // AD
            }
            else if(pstProg->u16AudioPid == 0)
            {
                // main audio
                pstProg->u16AudioPid = u16Pid;
                pstProg->eAdecCodecType = _MsDrv_DmxAudStrmType2CodecType(u8StrmType, bHdmv);
            }

            break;

        default:
            break;
        }
        pu8Loop += (5+ u32TempLoopLen);
        s32LoopLen -= (5+ u32TempLoopLen);
    }

    return 0;
}

static int _MsDrv_DmxGet1stProgInfo(MS_BOOL bLive, MS_U16 u16PmtPid, MS_U16 u16ProgNum, MsDrv_DmxProg_t *pstProg)
{
#define PMT_PATTERN_SIZE        5

    int ret = -1;
    MS_U32 u32FilterSrcType = DMX_FILTER_TYPE_SECTION;
    MS_U8 u8DmxId = DMX_DMXID_NULL;
    DMX_EVENT u32Event;
    DMX_Flt_info stFlterInfo;
    int nSectTimeout = 0;
    MS_PHYADDR phyAddr;
    MS_U8 *pu8Data = NULL;
    MS_U8 data[PMT_PATTERN_SIZE], mask[PMT_PATTERN_SIZE], nmask[PMT_PATTERN_SIZE];

    if(bLive)
        u32FilterSrcType |= DMX_FILTER_SOURCE_TYPE_LIVE;
    else
        u32FilterSrcType |= DMX_FILTER_SOURCE_TYPE_FILE;

    if(MApi_DMX_Open(u32FilterSrcType, &u8DmxId) != DMX_FILTER_STATUS_OK)
    {
        UBOOT_ERROR("DeMux>>ERROR! failed to MApi_DMX_Open(type = 0x%lx) \n", u32FilterSrcType);
        return -1;
    }

    memset(_u8SecBuf, 0, TSP_SECT_BUF_SIZE);
    memset(&stFlterInfo, 0, sizeof(DMX_Flt_info));
    stFlterInfo.Info.SectInfo.SectMode = DMX_SECT_MODE_ONESHOT;
    stFlterInfo.Info.SectInfo.SectMode |= DMX_SECT_MODE_CRCCHK;
    stFlterInfo.Info.SectInfo.Event = DMX_EVENT_DATA_READY | DMX_EVENT_CB_POLL;
    stFlterInfo.Info.SectInfo.SectBufAddr = MS_VA2PA((MS_U32)_u8SecBuf);
    stFlterInfo.Info.SectInfo.SectBufSize = TSP_SECT_BUF_SIZE;
    if(MApi_DMX_Info(u8DmxId, &stFlterInfo, NULL, TRUE) != DMX_FILTER_STATUS_OK)
    {
        UBOOT_ERROR("DeMux>>ERROR! failed to MApi_DMX_Info() \n");
        goto EXIT;
    }

    memset(data, 0, PMT_PATTERN_SIZE);
    memset(mask, 0, PMT_PATTERN_SIZE);
    memset(nmask, 0, PMT_PATTERN_SIZE);
    data[0] = 0x02;
    data[3] = (u16ProgNum>>8) & 0xFF;
    data[4] = (u16ProgNum & 0xFF);
    mask[0] = 0xFF;
    mask[3] = 0xFF;
    mask[4] = 0xFF;
    if(MApi_DMX_SectPatternSet(u8DmxId, data, mask, nmask, PMT_PATTERN_SIZE) != DMX_FILTER_STATUS_OK)
    {
        UBOOT_ERROR("DeMux>>ERROR! failed to MApi_DMX_SectPatternSet() \n");
        goto EXIT;
    }

    if(MApi_DMX_Pid(u8DmxId, &u16PmtPid, TRUE) != DMX_FILTER_STATUS_OK)
    {
        UBOOT_ERROR("DeMux>>ERROR! failed to MApi_DMX_Pid() \n");
        goto EXIT;
    }

    if(MApi_DMX_Start(u8DmxId) != DMX_FILTER_STATUS_OK)
    {
        UBOOT_ERROR("DeMux>>ERROR! failed to MApi_DMX_Start() \n");
        goto EXIT;
    }

    nSectTimeout = 300;
    while(--nSectTimeout)
    {
        u32Event = 0;
        if(MApi_DMX_Proc(u8DmxId, &u32Event) != DMX_FILTER_STATUS_OK)
            goto WHILE_CONTINUE;

        if((u32Event & DMX_EVENT_DATA_READY) == 0)
            goto WHILE_CONTINUE;

        phyAddr = 0;
        if(MApi_DMX_SectReadGet(u8DmxId, &phyAddr) != DMX_FILTER_STATUS_OK)
        {
            UBOOT_ERROR("DeMux>>ERROR! failed to MApi_DMX_SectReadGet()\n");
            goto EXIT;
        }

        pu8Data = (MS_U8*)MS_PA2KSEG1(phyAddr);
        if(pu8Data == NULL)
        {
            UBOOT_ERROR("DeMux>>ERROR! failed to MS_PA2KSEG1(0x%lx)\n", phyAddr);
            goto EXIT;
        }

        if(_MsDrv_DmxParsePmt(pu8Data, pstProg) != 0)
        {
            UBOOT_ERROR("DeMux>>ERROR! failed to _MsDrv_DmxParsePmt()\n");
            goto EXIT;
        }

        break;

WHILE_CONTINUE:
        MsOS_DelayTask(10);
    }

    if(nSectTimeout <= 0)
    {
        UBOOT_ERROR("DeMux>>ERROR! Timeout for receving PMT\n");
        goto EXIT;
    }

    ret = 0;

EXIT:


    if(u8DmxId != DMX_DMXID_NULL)
    {
        MApi_DMX_Stop(u8DmxId);
        MApi_DMX_Close(u8DmxId);
        u8DmxId = DMX_DMXID_NULL;
    }

    return ret;
}

static int _MsDrv_DmxParsePat(MS_U8 *pu8Data, MS_U16 *pu16PmtPid, MS_U16 *pu16ProgNum)
{
    if(pu8Data == NULL)
        return -1;

    int i;
    int ret = -1;
    MS_U8 u8TableId;
    MS_S32 s32LoopLen;
    MS_U8* pu8Loop;
    int nPmtLoopCnt;
    MS_U32 u16ProgNum;

    u8TableId = pu8Data[0];
    if(u8TableId != 0x00)
    {

        UBOOT_ERROR("DeMux>>ERROR! Wrong table ID = 0x%x for PAT!\n", u8TableId);
        return -1;
    }

    s32LoopLen = (MS_U32)_SECT_WORD12(pu8Data, 1);
    if (9 > s32LoopLen)
    {
        UBOOT_ERROR("DeMux>>ERROR! got too small section length = %ld!\n", s32LoopLen);
        return -1;
    }

    s32LoopLen -= 5; // PAT header
    s32LoopLen -= 4; // CRC
    if (0x3 & s32LoopLen)
    {
        UBOOT_ERROR("DeMux>>ERROR! illegal loop length = %ld for PMTs\n", s32LoopLen);
        return FALSE;
    }

    pu8Loop = pu8Data + 8;
    nPmtLoopCnt = (s32LoopLen>>2);
    for(i = 0; i < nPmtLoopCnt; i++)
    {
        u16ProgNum = _SECT_WORD16(pu8Loop, 0);
        if (0 == u16ProgNum)
        {
            // network_PID
            goto NEXT_LOOP;
        }

        if(pu16ProgNum)
            *pu16ProgNum = u16ProgNum;

        if(pu16PmtPid)
            *pu16PmtPid = ((MS_U16)(((MS_U8*)(pu8Loop))[2]<<8)|(((MS_U8*)(pu8Loop))[3])) & 0x1FFF;

        ret = 0;
        break;

NEXT_LOOP:
        pu8Loop+= 4;
    }

    return ret;
}

static int _MsDrv_DmxGet1stProg(MS_BOOL bLive, MS_U16 *pu16PmtPid, MS_U16 *pu16ProgNum)
{
    int ret = -1;
    MS_U32 u32FilterSrcType = DMX_FILTER_TYPE_SECTION;
    MS_U8 u8DmxId = DMX_DMXID_NULL;
    DMX_EVENT u32Event;
    DMX_Flt_info stFlterInfo;
    int nSectTimeout = 0;
    MS_PHYADDR phyAddr;
    MS_U8 *pu8Data = NULL;
    MS_U8 data, mask, nmask;
    MS_U16 u16PatPid = 0x00; // PAT pid

    if(bLive)
        u32FilterSrcType |= DMX_FILTER_SOURCE_TYPE_LIVE;
    else
        u32FilterSrcType |= DMX_FILTER_SOURCE_TYPE_FILE;

    if(MApi_DMX_Open(u32FilterSrcType, &u8DmxId) != DMX_FILTER_STATUS_OK)
    {
        UBOOT_ERROR("DeMux>>ERROR! failed to MApi_DMX_Open(type = 0x%lx) \n", u32FilterSrcType);
        return -1;
    }

    memset(_u8SecBuf, 0, TSP_SECT_BUF_SIZE);
    memset(&stFlterInfo, 0, sizeof(DMX_Flt_info));
    stFlterInfo.Info.SectInfo.SectMode = DMX_SECT_MODE_ONESHOT;
    stFlterInfo.Info.SectInfo.SectMode |= DMX_SECT_MODE_CRCCHK;
    stFlterInfo.Info.SectInfo.Event = DMX_EVENT_DATA_READY | DMX_EVENT_CB_POLL;
    stFlterInfo.Info.SectInfo.SectBufAddr = MS_VA2PA((MS_U32)_u8SecBuf);
    stFlterInfo.Info.SectInfo.SectBufSize = TSP_SECT_BUF_SIZE;
    if(MApi_DMX_Info(u8DmxId, &stFlterInfo, NULL, TRUE) != DMX_FILTER_STATUS_OK)
    {
        UBOOT_ERROR("DeMux>>ERROR! failed to MApi_DMX_Info() \n");
        goto EXIT;
    }

    data = 0x00;    // PAT table_id
    mask = 0xFF;
    nmask = 0;
    if(MApi_DMX_SectPatternSet(u8DmxId, &data, &mask, &nmask, 1) != DMX_FILTER_STATUS_OK)
    {
        UBOOT_ERROR("DeMux>>ERROR! failed to MApi_DMX_SectPatternSet() \n");
        goto EXIT;
    }

    if(MApi_DMX_Pid(u8DmxId, &u16PatPid, TRUE) != DMX_FILTER_STATUS_OK)
    {
        UBOOT_ERROR("DeMux>>ERROR! failed to MApi_DMX_Pid() \n");
        goto EXIT;
    }

    if(MApi_DMX_Start(u8DmxId) != DMX_FILTER_STATUS_OK)
    {
        UBOOT_ERROR("DeMux>>ERROR! failed to MApi_DMX_Start() \n");
        goto EXIT;
    }

    nSectTimeout = 300;
    while(--nSectTimeout)
    {
        u32Event = 0;
        if(MApi_DMX_Proc(u8DmxId, &u32Event) != DMX_FILTER_STATUS_OK)
        {
            goto WHILE_CONTINUE;
        }

        if((u32Event & DMX_EVENT_DATA_READY) == 0)
        {
            goto WHILE_CONTINUE;
        }

        phyAddr = 0;
        if(MApi_DMX_SectReadGet(u8DmxId, &phyAddr) != DMX_FILTER_STATUS_OK)
        {
            UBOOT_ERROR("DeMux>>ERROR! failed to MApi_DMX_SectReadGet()\n");
            goto EXIT;
        }

        pu8Data = (MS_U8*)MS_PA2KSEG1(phyAddr);
        if(pu8Data == NULL)
        {
            UBOOT_ERROR("DeMux>>ERROR! failed to MS_PA2KSEG1(0x%lx)\n", phyAddr);
            goto EXIT;
        }

        if(_MsDrv_DmxParsePat(pu8Data, pu16PmtPid, pu16ProgNum) != 0)
        {
            UBOOT_ERROR("DeMux>>ERROR! failed to _MsDrv_DmxParsePat()\n");
            goto EXIT;
        }

        break;

WHILE_CONTINUE:
        MsOS_DelayTask(10);
    }

    if(nSectTimeout <= 0)
    {
        UBOOT_ERROR("DeMux>>ERROR! Timeout for receving PAT\n");
        goto EXIT;
    }

    ret = 0;

EXIT:


    if(u8DmxId != DMX_DMXID_NULL)
    {
        MApi_DMX_Stop(u8DmxId);
        MApi_DMX_Close(u8DmxId);
        u8DmxId = DMX_DMXID_NULL;
    }

    return ret;
}
#endif  // #if (CONFIG_BOOTVIDEO == 1)

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
int MsDrv_DmxInit(DMX_FLOW_INPUT inSrc)
{
    MS_PHYADDR      phyFW;
    MS_U8*          pu8FW;
    if(_bDmxInited)
    {
        UBOOT_INFO("Demux>>INFO: TSP is already be inited!\n");
        goto DMX_FLOW_SET;
    }

    MDrv_MMIO_Init(); // must init MMI before init DMX

    pu8FW = (MS_U8*)(((int)u8TSFwBuf + 0xFF) & (~0xFF));

    pu8FW = (MS_U8*)MsOS_VA2PA((MS_U32)pu8FW);

    pu8FW = (MS_U8*)MsOS_PA2KSEG1((MS_U32)pu8FW);

    memcpy(pu8FW, u8TSPFirmware, sizeof(u8TSPFirmware));
    MsOS_FlushMemory();

    phyFW = (MS_PHYADDR)MsOS_VA2PA((MS_U32)pu8FW);

    if (DMX_FILTER_STATUS_ERROR == MApi_DMX_SetFW(phyFW, sizeof(u8TSPFirmware)))
    {
        UBOOT_ERROR("DeMux>>ERROR! SetFW fail\n");
        return -1;
    }

    MApi_DMX_SetHK(TRUE);

    if (DMX_FILTER_STATUS_ERROR == MApi_DMX_Init())
    {
        UBOOT_ERROR("DeMux>>ERROR! TSP init fail\n");
        return -1;
    }

#if defined(TSP_FW_SIZE) && defined(TSP_VQ_SIZE)
    // set VQ for TSP, set FW/VQ for TSP2
    MS_U8*          pu8VQ;
    MS_PHYADDR      phyVQ;
    DMX_TSPParam    param;

    memset(&param,0,sizeof(param));
    pu8VQ = (MS_U8*)(((int)u8TSVQBuf + 0x7) & (~0x7));
    phyVQ = (MS_PHYADDR)MsOS_VA2PA((MS_U32)pu8VQ);

    // printf("[%s][%d] (fw, vq) = (0x%08x 0x%08x)\n", __FUNCTION__, __LINE__,
    // phyFW, phyVQ);
    param.phyFWAddr = (MS_PHYADDR)phyFW;
    param.u32FWSize = TSP_FW_SIZE;
    param.phyVQAddr = (MS_PHYADDR)phyVQ;
    param.u32VQSize = TSP_VQ_SIZE;//(200*16 + 200*8 + 200*8);
    UBOOT_INFO("param.phyFWAddr = 0x%x\n", param.phyFWAddr);
    UBOOT_INFO("param.u32FWSize = 0x%x\n", param.u32FWSize);
    UBOOT_INFO("param.phyVQAddr = 0x%x\n", param.phyVQAddr);
    UBOOT_INFO("param.u32VQSize = 0x%x\n", param.u32VQSize);
//    while(1);
    if (DMX_FILTER_STATUS_ERROR == MApi_DMX_TSPInit(&param))
    {
        UBOOT_ERROR("DeMux>>ERROR! TSP set fw/vq fail\n");
        return -1;
    }
#endif  // #if defined(TSP_FW_SIZE) && defined(TSP_VQ_SIZE)
    MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE_VIDEO, TRUE);

    MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE_AUDIO, TRUE);

    MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE_AUDIO2, TRUE);



    _bDmxInited = TRUE;

DMX_FLOW_SET:

    //if (DMX_FILTER_STATUS_OK!= MApi_DMX_FlowSet(DMX_FLOW_PLAYBACK, inSrc, TS_CLK_INV, TRUE, TS_PARALLEL_OUTPUT))
    if (DMX_FILTER_STATUS_OK!= MApi_DMX_FlowSet(DMX_FLOW_PLAYBACK, inSrc, FALSE, FALSE, TRUE))
    {
        UBOOT_ERROR("DeMux>>ERROR! failed to MApi_DMX_FlowSet(inSrc = %d) fail\n", inSrc);
        return -1;
    }

    _eCurDmxSrc = inSrc;

    return 0;
}

int MsDrv_DmxFileInInit(MS_U32 u32FileinAddr, MS_U32 u32FileSize, MS_U32 u32Repeat)
{
    DMX_Filein_info stFileinInfo;
    MS_U32 u32Idx = 0;
    MS_U32 u32MaxLoop = 0;
    MS_U32 u32FileinPhyAddr;
   //   u32FileinPhyAddr=PA2NVA(u32FileinAddr);
   u32FileinPhyAddr=u32FileinAddr;
    // File in Setting
    // Set Packet size 188 = TS packet size(188/192/204)
    stFileinInfo.PKT_Mode = DMX_PKTMODE_188;
    // Set filein input delay 0x0 ~ 0xFFFFFF
    stFileinInfo.Rate = 0x0A;//0x0000ffff;
    // set filein parameter
    MApi_DMX_Filein_Info(&stFileinInfo);

    if (DRV_DMX_MAX_FILE_IN_SIZE < u32FileSize)
    {
        u32FileSize = DRV_DMX_MAX_FILE_IN_SIZE;
        UBOOT_ERROR("The file size is larger than DMX buffer, resize to 0x%lx\n",u32FileSize);
    }

    if (u32Repeat > 3)
    {
        u32MaxLoop = u32Repeat;
        //u32MaxLoop = 3;
    }
    else
    {
        u32MaxLoop = u32Repeat;
    }

    for (u32Idx = 0; u32Idx < u32MaxLoop; u32Idx++)
    {
        //UBOOT_DEBUG("@@ MApi_DMX_Filein_Start u32FileinPhyAddr=0x%x, u32FileSize = 0x%x\n",u32FileinPhyAddr, u32FileSize);
        if (DMX_FILTER_STATUS_ERROR == MApi_DMX_Filein_Start(DMX_PES_NO_BYPASS_FIFO,  u32FileinPhyAddr, u32FileSize))
        {
            return -1;
        }
        UBOOT_DEBUG("@@ MApi_DMX_Filein_Start u32FileinPhyAddr=0x%x, u32FileSize = 0x%x\n",u32FileinPhyAddr, u32FileSize);
    }

    return 0;
}

#if (CONFIG_BOOTVIDEO == 1)
// open video pid filter, source is the 1st parameter of MsDrv_DmxInit()
int MsDrv_DmxOpenVideoFilter(MS_U16 u16VideoPid, MS_U8 *pu8DmxId)
{
    if(_bDmxInited == FALSE)
    {
        UBOOT_ERROR("DeMux>>ERROR! DMX hasn't been inited()\n");
        return -1;
    }

    return _MsDrv_DmxOpenAVFilter(_eCurDmxSrc, DMX_FILTER_TYPE_VIDEO, u16VideoPid, pu8DmxId);
}


// open audio pid filter, source is the 1st parameter of MsDrv_DmxInit()
int MsDrv_DmxOpenAudioFilter(MS_U16 u16AudioPid, MS_U8 *pu8DmxId)
{
    if(_bDmxInited == FALSE)
    {
        UBOOT_ERROR("DeMux>>ERROR! DMX hasn't been inited()\n");
        return -1;
    }

    return _MsDrv_DmxOpenAVFilter(_eCurDmxSrc, DMX_FILTER_TYPE_AUDIO, u16AudioPid, pu8DmxId);
}

// open pcr pid filter, source is the 1st parameter of MsDrv_DmxInit()
int MsDrv_DmxOpenPcrFilter(MS_U16 u16PcrPid, MS_U8 *pu8DmxId)
{
    if(_bDmxInited == FALSE)
    {
        UBOOT_ERROR("DeMux>>ERROR! DMX hasn't been inited()\n");
        return -1;
    }

    int nRet = _MsDrv_DmxOpenAVFilter(_eCurDmxSrc, DMX_FILTER_TYPE_PCR, u16PcrPid, pu8DmxId);
    if(nRet == 0)
    {
        // enable PCR update
        _bDmxPcrEnable = TRUE;
    }

    return nRet;
}

void MsDrv_DmxCloseFilter(MS_U8 u8DmxId)
{
    if(_bDmxInited == FALSE)
    {
        UBOOT_ERROR("DeMux>>ERROR! DMX hasn't been inited()\n");
        return;
    }

    if(u8DmxId == DMX_DMXID_NULL)
        return;

    DMX_FILTER_TYPE u32FiltType;
    DMX_Flt_info stDmxFiltInfo;
    memset(&stDmxFiltInfo, 0, sizeof(DMX_Flt_info));
    if(MApi_DMX_Info(u8DmxId, &stDmxFiltInfo, &u32FiltType, FALSE))
    {
        if(u32FiltType == DMX_FILTER_TYPE_PCR)
            _bDmxPcrEnable = FALSE;
    }

    MApi_DMX_Stop(u8DmxId);
    MApi_DMX_Close(u8DmxId);
}

void MsDrv_DmxRoutinePolling(void)
{
    if(_bDmxInited == FALSE)
    {
        UBOOT_ERROR("DeMux>>ERROR! DMX hasn't been inited()\n");
        return;
    }

    // Get PCR to update to STC
    if(_bDmxPcrEnable)
    {
        static MS_U32 u32PrevPcr32 = 0;
        static MS_U32 u32PrevPcr = 0;
        MS_U32 u32Pcr32, u32Pcr;
        if(MApi_DMX_Pcr_Get(&u32Pcr32, &u32Pcr) == DMX_FILTER_STATUS_OK)
        {
            if((u32PrevPcr32 != u32Pcr32) || (u32PrevPcr != u32Pcr))
            {
                // update to STC
                MApi_DMX_Stc_Set(u32Pcr32, u32Pcr);

                u32PrevPcr32 = u32Pcr32;
                u32PrevPcr = u32Pcr;
            }
        }
    }
}

int MsDrv_DmxScan(MS_BOOL bLive, MsDrv_DmxProg_t *pstProg1st)
{
    if(_bDmxInited == FALSE)
    {
        UBOOT_ERROR("DeMux>>ERROR! DMX hasn't been inited()\n");
        return -1;
    }

    if(pstProg1st == NULL)
        return -1;

    MS_U16 u16PmtPid = 0;
    MS_U16 u16ProgNum = 0;

    if(_MsDrv_DmxGet1stProg(bLive, &u16PmtPid, &u16ProgNum) != 0)
    {
        UBOOT_ERROR("DeMux>>ERROR! failed to _MsDrv_DmxGet1stProg()\n");
        return -1;
    }

    memset(pstProg1st, 0, sizeof(MsDrv_DmxProg_t));
    if(_MsDrv_DmxGet1stProgInfo(bLive, u16PmtPid, u16ProgNum, pstProg1st) != 0)
    {
        UBOOT_ERROR("DeMux>>ERROR! failed to _MsDrv_DmxGet1stProgInfo()\n");
        return -1;
    }

    UBOOT_INFO("\nScan result of 1st program is:\n");
    UBOOT_INFO("Video pid = %d\n", pstProg1st->u16VideoPid);
    UBOOT_INFO("Video codec = %d\n", pstProg1st->eVdecCodecType);
    UBOOT_INFO("Audio pid = %d\n", pstProg1st->u16AudioPid);
    UBOOT_INFO("Audio codec = %d\n", pstProg1st->eAdecCodecType);
    UBOOT_INFO("PCR pid = %d\n", pstProg1st->u16PcrPid);

    return 0;
}

void MsDrv_DmxEnableAvFifo(void)
{
    MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE_VIDEO, FALSE);
    //MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE_AUDIO, FALSE);
}
#endif  // #if (CONFIG_BOOTVIDEO == 1)

#endif



