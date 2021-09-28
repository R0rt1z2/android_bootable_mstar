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
#if ((CONFIG_BOOTVIDEO == 1) || (CONFIG_BOOTIFRAME == 1))

#include <MsTypes.h>
#include <MsDebug.h>
#include <MsMmap.h>
#include <apiVDEC_EX.h>
#include <apiDMX.h>
#include "MsDrvVideo.h"
#include <mdrv_dac_tbl.h>
//#include "hdmitx/mstar/MsDrvHdmi.h"
// for XC +
#include "drvXC_IOPort.h"
#include "apiXC.h"
#include "drvMVOP.h"
#include "apiVDEC.h"
#include "apiHDMITx.h"
#include "xc/msAPI_XC.h"
// for XC -

#include <MsVfs.h>
#include <CusConfig.h>
#include <MsSystem.h>

// BD_MST188A_DO1A_S.h +
#define HDMITX_ANALOG_TUNING_SD         {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
#define HDMITX_ANALOG_TUNING_HD         {0x00, 0x00, 0x02, 0x07, 0x00, 0x00}
#define HDMITX_ANALOG_TUNING_DEEP_HD    {0x02, 0x01, 0x0a, 0x07, 0x07, 0x07}
// BD_MST188A_DO1A_S.h -


/// VDEC Driver process buffer size
#define MSDRV_VIDEO_VDEC_DRV_PROC_SIZE                      (0xa000)

#define MSDRV_VIDEO_MMAP_ID_VDEC_AEON               "E_MMAP_ID_VDEC_AEON"
#define MSDRV_VIDEO_MMAP_ID_VDEC_FRAME_BUF      "E_MMAP_ID_VDEC_FRAME_BUF"
#define MSDRV_VIDEO_MMAP_ID_VDEC_BITSTREAM      "E_MMAP_ID_VDEC_BITSTREAM"
#define MSDRV_VIDEO_MMAP_ID_XC_MAIN_FB              "E_MMAP_ID_XC_MAIN_FB"
#define MSDRV_VIDEO_MMAP_ID_XC_MENULOAD_BUF     "E_MMAP_ID_XC_MENULOAD_BUF"

#define MSDRV_VIDEO_HVD_PTS_OFFSET     0xA0064     ///< HVD stores the PTS at the adress offset from E_MMAP_ID_VDEC_AEON_ADR
#define MSDRV_VIDEO_MVD_PTS_OFFSET     0xA1094     ///< MVD stores the PTS at adress offset from E_MMAP_ID_VDEC_AEON_ADR

#define MSDRV_VIDEO_HVD_FRAME_COUNT_OFFSET     0xA0030     ///< HVD stores the frame count at the adress offset from E_MMAP_ID_VDEC_AEON_ADR
#define MSDRV_VIDEO_MVD_FRAME_COUNT_OFFSET     0xA1000    ///< MVD stores the frame count at adress offset from E_MMAP_ID_VDEC_AEON_ADR

#define MSDRV_VIDEO_HVD_SKIP_FRAME_COUNT_OFFSET     0xA0034     ///< HVD stores the frame count at the adress offset from E_MMAP_ID_VDEC_AEON_ADR
#define MSDRV_VIDEO_MVD_SKIP_FRAME_COUNT_OFFSET     0xA1014    ///< MVD stores the skip frame count at adress offset from E_MMAP_ID_VDEC_AEON_ADR



#define MSDRV_VIDEO_HVD_PTS_KHZ_UNIT           1           ///< 1KHz unit
#define MSDRV_VIDEO_MVD_PTS_KHZ_UNIT               90   ///< 90kHz unit

typedef enum
{
    E_MSDRV_VDEC_SM_IDLE = 0,
    E_MSDRV_VDEC_SM_WAIT_INFO_CHANGE,
    E_MSDRV_VDEC_SM_DO_INFO_CHANGE,
    E_MSDRV_VDEC_SM_WAIT_FIRST_FRAME,
    E_MSDRV_VDEC_SM_DO_FIRST_FRAME,
    E_MSDRV_VDEC_SM_PLAYING,

}MsDrv_VdecRoutineStateMachine_e;

static VDEC_StreamId _stVdecStreamId = {0, 0};

static volatile MsDrv_VdecRoutineStateMachine_e _eVdecRoutineStateMachine = E_MSDRV_VDEC_SM_IDLE;
static volatile MS_BOOL _bVdecCallbkInfoChg = FALSE;
static volatile MS_BOOL _bVdecRecvFirstFrame = FALSE;
static volatile MS_BOOL _bVdecXcMvopCfgDone = FALSE;

static VDEC_EX_SrcMode _MsDrv_VideoGetVdecSrcType(MsDrv_VIDEO_StreamType_e eStreamType)
{
    VDEC_EX_SrcMode eVdecSrcMode = E_VDEC_EX_SRC_MODE_DTV;

    switch(eStreamType)
    {
        case E_MSDRV_VIDEO_STREAM_LIVE:
            eVdecSrcMode = E_VDEC_EX_SRC_MODE_DTV;
            break;

        case E_MSDRV_VIDEO_STREAM_TSFILE:
            eVdecSrcMode = E_VDEC_EX_SRC_MODE_TS_FILE;
            break;

        case E_MSDRV_VIDEO_STREAM_FILE:
            eVdecSrcMode = E_VDEC_EX_SRC_MODE_FILE;
            break;

        default:
            UBOOT_ERROR("Unknown stream type = %d\n", eStreamType);
            break;
    }

    return eVdecSrcMode;
}

static VDEC_EX_CodecType _MsDrv_VideoGetVdecCodecType(MsDrv_VIDEO_CodecType_e eCodecType)
{
    VDEC_EX_CodecType eVdecCodecType = E_VDEC_EX_CODEC_TYPE_NONE;
    switch(eCodecType)
    {
        ///MPEG 1/2
        case E_MSDRV_VIDEO_CODEC_TYPE_MPEG2:
            eVdecCodecType = E_VDEC_EX_CODEC_TYPE_MPEG2;
            break;

        ///H263 (short video header)
        case E_MSDRV_VIDEO_CODEC_TYPE_H263:
            eVdecCodecType = E_VDEC_EX_CODEC_TYPE_H263;
            break;

        ///MPEG4 (default)
        case E_MSDRV_VIDEO_CODEC_TYPE_MPEG4:
            eVdecCodecType = E_VDEC_EX_CODEC_TYPE_MPEG4;
            break;

        ///MPEG4 (Divx311)
        case E_MSDRV_VIDEO_CODEC_TYPE_DIVX311:
            eVdecCodecType = E_VDEC_EX_CODEC_TYPE_DIVX311;
            break;

        ///MPEG4 (Divx412)
        case E_MSDRV_VIDEO_CODEC_TYPE_DIVX412:
            eVdecCodecType = E_VDEC_EX_CODEC_TYPE_DIVX412;
            break;

        ///FLV
        case E_MSDRV_VIDEO_CODEC_TYPE_FLV:
            eVdecCodecType = E_VDEC_EX_CODEC_TYPE_FLV;
            break;

        ///VC1 advanced profile (VC1)
        case E_MSDRV_VIDEO_CODEC_TYPE_VC1_ADV:
            eVdecCodecType = E_VDEC_EX_CODEC_TYPE_VC1_ADV;
            break;

        ///VC1 main profile (RCV)
        case E_MSDRV_VIDEO_CODEC_TYPE_VC1_MAIN:
            eVdecCodecType = E_VDEC_EX_CODEC_TYPE_VC1_MAIN;
            break;

        ///Real Video version 8
        case E_MSDRV_VIDEO_CODEC_TYPE_RV8:
            eVdecCodecType = E_VDEC_EX_CODEC_TYPE_RV8;
            break;

        ///Real Video version 9 and 10
        case E_MSDRV_VIDEO_CODEC_TYPE_RV9:
            eVdecCodecType = E_VDEC_EX_CODEC_TYPE_RV9;
            break;

        ///H264
        case E_MSDRV_VIDEO_CODEC_TYPE_H264:
            eVdecCodecType = E_VDEC_EX_CODEC_TYPE_H264;
            break;

        ///AVS
        case E_MSDRV_VIDEO_CODEC_TYPE_AVS:
            eVdecCodecType = E_VDEC_EX_CODEC_TYPE_AVS;
            break;

        ///MJPEG
        case E_MSDRV_VIDEO_CODEC_TYPE_MJPEG:
            eVdecCodecType = E_VDEC_EX_CODEC_TYPE_MJPEG;
            break;

        ///MVC
        case E_MSDRV_VIDEO_CODEC_TYPE_MVC:
            eVdecCodecType = E_VDEC_EX_CODEC_TYPE_MVC;
            break;

        ///VP8
        case E_MSDRV_VIDEO_CODEC_TYPE_VP8:
            eVdecCodecType = E_VDEC_EX_CODEC_TYPE_VP8;
            break;

        ///HEVC
        case E_MSDRV_VIDEO_CODEC_TYPE_HEVC:
            eVdecCodecType = E_VDEC_EX_CODEC_TYPE_HEVC;
            break;

        default:
            UBOOT_ERROR("Unknown codec type = %d\n", eCodecType);
            break;
    }

    return eVdecCodecType;
}

static MS_BOOL _MsDrv_VideoIsHvd(MsDrv_VIDEO_CodecType_e eVdecCodecType)
{
    MS_BOOL bHvd = FALSE;

    switch (eVdecCodecType)
    {
        case E_MSDRV_VIDEO_CODEC_TYPE_MPEG2:
        case E_MSDRV_VIDEO_CODEC_TYPE_H263:
        case E_MSDRV_VIDEO_CODEC_TYPE_MPEG4:
        case E_MSDRV_VIDEO_CODEC_TYPE_DIVX311:
        case E_MSDRV_VIDEO_CODEC_TYPE_DIVX412:
        case E_MSDRV_VIDEO_CODEC_TYPE_FLV:
        case E_MSDRV_VIDEO_CODEC_TYPE_VC1_ADV:
        case E_MSDRV_VIDEO_CODEC_TYPE_VC1_MAIN:
            // MVD
            bHvd = FALSE;
            break;

        case E_MSDRV_VIDEO_CODEC_TYPE_RV8:
        case E_MSDRV_VIDEO_CODEC_TYPE_RV9:
        case E_MSDRV_VIDEO_CODEC_TYPE_H264:
        case E_MSDRV_VIDEO_CODEC_TYPE_AVS:
        case E_MSDRV_VIDEO_CODEC_TYPE_MVC:  /// SUPPORT_MVC
        case E_MSDRV_VIDEO_CODEC_TYPE_VP8:
        case E_MSDRV_VIDEO_CODEC_TYPE_HEVC:
            // HVD
            bHvd = TRUE;
            break;

        default:
            UBOOT_ERROR("Error: Unknown VDEC codec type = %d\n", eVdecCodecType);
            break;
    }

    return bHvd;
}


static MS_BOOL _MsDrv_VideoGetMmap(char *pszMmapId, MS_PHYADDR *pu32MmapAddr, U32 *pu32MmapLen)
{
    MS_PHYADDR u32MmapAddr = 0;
    U32 u32MmapLen = 0;

    if(pu32MmapAddr) *pu32MmapAddr = 0;
    if(pu32MmapLen) *pu32MmapLen = 0;

    if(pu32MmapAddr)
    {
        if(get_addr_from_mmap(pszMmapId, &u32MmapAddr) != 0)
        {
            UBOOT_ERROR("failed to get mmap addr of \"%s\" \n", pszMmapId);
            return FALSE;
        }

        *pu32MmapAddr = u32MmapAddr;
    }

    if(pu32MmapLen)
    {
        if(get_length_from_mmap(pszMmapId, &u32MmapLen) != 0)
        {
            UBOOT_ERROR("failed to get mmap length of \"%s\" \n", pszMmapId);
            return FALSE;
        }

        *pu32MmapLen = u32MmapLen;
    }

    return TRUE;
}

static MS_BOOL _MsDrv_VideoInit(VDEC_StreamId *pstVdecStreamId, VDEC_EX_SrcMode eVdecSrcMode, VDEC_EX_CodecType eVdecCodecType)
{
    if(pstVdecStreamId == NULL)
        return FALSE;

    VDEC_EX_Result eResult = E_VDEC_EX_FAIL;
    VDEC_EX_InitParam initParam;
    MS_PHYADDR u32MmapAddr;
    MS_U32 u32MmapLen;

    memset(&initParam, 0, sizeof(VDEC_EX_InitParam));
    initParam.eCodecType = eVdecCodecType;
    initParam.EnableDynaScale = FALSE;
    initParam.bDisableDropErrFrame = TRUE;
    initParam.bDisableErrConceal = FALSE;
    initParam.bRepeatLastField = TRUE;
    initParam.VideoInfo.eSrcMode = eVdecSrcMode;
    if(eVdecSrcMode == E_VDEC_EX_SRC_MODE_TS_FILE)
    {
        //initParam.VideoInfo.eSrcMode = E_VDEC_EX_SRC_MODE_TS_FILE;
        initParam.VideoInfo.eTimeStampType = E_VDEC_EX_TIME_STAMP_PTS;
        initParam.bRepeatLastField = FALSE;
        initParam.u8ErrThreshold = 0;
    }

    u32MmapAddr = u32MmapLen = 0;
    if(_MsDrv_VideoGetMmap(MSDRV_VIDEO_MMAP_ID_VDEC_AEON, &u32MmapAddr, &u32MmapLen) == FALSE)
    {
        UBOOT_ERROR("failed to _MsDrv_VideoGetMmap(MSDRV_VIDEO_MMAP_ID_VDEC_AEON)!\n");
        goto VIDEO_INIT_FAILED;
    }
    initParam.SysConfig.u32FWBinaryAddr = initParam.SysConfig.u32CodeBufAddr = u32MmapAddr;
    initParam.SysConfig.u32FWBinarySize = initParam.SysConfig.u32CodeBufSize = u32MmapLen;

    u32MmapAddr = u32MmapLen = 0;
    if(_MsDrv_VideoGetMmap(MSDRV_VIDEO_MMAP_ID_VDEC_FRAME_BUF, &u32MmapAddr, &u32MmapLen) == FALSE)
    {
        UBOOT_ERROR("failed to _MsDrv_VideoGetMmap(MSDRV_VIDEO_MMAP_ID_VDEC_AEON)!\n");
        goto VIDEO_INIT_FAILED;
    }
    initParam.SysConfig.u32FrameBufAddr = u32MmapAddr;
    initParam.SysConfig.u32FrameBufSize = u32MmapLen;

    u32MmapAddr = u32MmapLen = 0;
    if(_MsDrv_VideoGetMmap(MSDRV_VIDEO_MMAP_ID_VDEC_BITSTREAM, &u32MmapAddr, &u32MmapLen) == FALSE)
    {
        UBOOT_ERROR("failed to _MsDrv_VideoGetMmap(MSDRV_VIDEO_MMAP_ID_VDEC_AEON)!\n");
        goto VIDEO_INIT_FAILED;
    }
    initParam.SysConfig.u32BitstreamBufAddr = u32MmapAddr;
    initParam.SysConfig.u32BitstreamBufSize = u32MmapLen;

    initParam.SysConfig.u32DrvProcBufAddr = initParam.SysConfig.u32BitstreamBufAddr;
    initParam.SysConfig.u32DrvProcBufSize = MSDRV_VIDEO_VDEC_DRV_PROC_SIZE ;
    initParam.SysConfig.eFWSourceType = E_VDEC_EX_FW_SOURCE_NONE;
    initParam.SysConfig.eDbgMsgLevel = E_VDEC_EX_DBG_LEVEL_NONE;

    UBOOT_INFO("VDEC FW add = 0x%lx   Size = 0x%lx\n", initParam.SysConfig.u32CodeBufAddr, initParam.SysConfig.u32CodeBufSize);
    UBOOT_INFO("VDEC FB add = 0x%lx   Size = 0x%lx\n", initParam.SysConfig.u32FrameBufAddr, initParam.SysConfig.u32FrameBufSize);
    UBOOT_INFO("VDEC BS add = 0x%lx   Size = 0x%lx\n", initParam.SysConfig.u32BitstreamBufAddr, initParam.SysConfig.u32BitstreamBufSize);
    UBOOT_INFO("VDEC Proc add = 0x%lx   Size = 0x%lx\n", initParam.SysConfig.u32DrvProcBufAddr, initParam.SysConfig.u32DrvProcBufSize);

    MApi_VDEC_EX_EnableTurboMode(TRUE);

    //Init/Re-Init VDEC
    eResult = MApi_VDEC_EX_Init(pstVdecStreamId, &initParam);
    if(E_VDEC_EX_OK != eResult)
    {
        UBOOT_ERROR("Error 0x%x: MApi_VDEC_EX_Init return fail!!!\n",eResult);
        goto VIDEO_INIT_FAILED;
    }

    return TRUE;

VIDEO_INIT_FAILED:

    return FALSE;
}

static MVOP_InputSel _MsDrv_VideoGetMvopInputSel(VDEC_StreamId *pstVdecStreamId)
{
    if(pstVdecStreamId == NULL)
        return MVOP_INPUT_UNKNOWN;

    MVOP_InputSel eMvopInputSel = MVOP_INPUT_UNKNOWN;
    VDEC_EX_CodecType eCodecType = E_VDEC_EX_CODEC_TYPE_NONE;

    eCodecType = MApi_VDEC_EX_GetActiveCodecType(pstVdecStreamId);
    switch(eCodecType)
    {
        case E_VDEC_EX_CODEC_TYPE_MPEG2:
        case E_VDEC_EX_CODEC_TYPE_VC1_ADV:
        case E_VDEC_EX_CODEC_TYPE_VC1_MAIN:
        case E_VDEC_EX_CODEC_TYPE_MPEG4:
        case E_VDEC_EX_CODEC_TYPE_DIVX412:
        case E_VDEC_EX_CODEC_TYPE_DIVX311:
        case E_VDEC_EX_CODEC_TYPE_FLV:
        case E_VDEC_EX_CODEC_TYPE_H263:
            eMvopInputSel = MVOP_INPUT_MVD;
            break;

        case E_VDEC_EX_CODEC_TYPE_H264:
        case E_VDEC_EX_CODEC_TYPE_VP8:
        case E_VDEC_EX_CODEC_TYPE_AVS:
            eMvopInputSel = MVOP_INPUT_H264;
            break;

        case E_VDEC_EX_CODEC_TYPE_MVC:
            eMvopInputSel = MVOP_INPUT_HVD_3DLR;
            break;

        case E_VDEC_EX_CODEC_TYPE_RV8:
        case E_VDEC_EX_CODEC_TYPE_RV9:
            eMvopInputSel = MVOP_INPUT_RVD;
            break;

        case E_VDEC_EX_CODEC_TYPE_MJPEG:
            eMvopInputSel = MVOP_INPUT_JPD;
            break;

        case E_VDEC_EX_CODEC_TYPE_HEVC:
            eMvopInputSel = MVOP_INPUT_EVD;
            break;

        default:
            break;
    }

    return eMvopInputSel;
}
extern XC_Internal_TimingType msHdmitx_GetEnvOutputTiming(MS_BOOL *pbNtsc);
static E_MSAPI_XC_OUTPUT_TIMING_TYPE _MsDrv_VideoGetEnvOutputTiming(void)
{
    MS_BOOL bNtsc = FALSE;
    XC_Internal_TimingType enXcTimingType;
    E_MSAPI_XC_OUTPUT_TIMING_TYPE enMsApiXcTimingType = E_MSAPI_XC_RES_1920x1080I_50Hz;

    enXcTimingType = msHdmitx_GetEnvOutputTiming(&bNtsc);
    switch(enXcTimingType)
    {
    case E_XC_480I:
        enMsApiXcTimingType = E_MSAPI_XC_RES_720x480I_60Hz;
        break;

    case E_XC_480P:
        enMsApiXcTimingType = E_MSAPI_XC_RES_720x480P_60Hz;
        break;

    case E_XC_576I:
        enMsApiXcTimingType = E_MSAPI_XC_RES_720x576I_50Hz;
        break;

    case E_XC_576P:
        enMsApiXcTimingType = E_MSAPI_XC_RES_720x576P_50Hz;
        break;

    case E_XC_720P:
        enMsApiXcTimingType = bNtsc ? E_MSAPI_XC_RES_1280x720P_60Hz : E_MSAPI_XC_RES_1280x720P_50Hz;
        break;

    case E_XC_1080P:
        enMsApiXcTimingType = bNtsc ? E_MSAPI_XC_RES_1920x1080P_60Hz : E_MSAPI_XC_RES_1920x1080P_50Hz;
        break;

    case E_XC_1080I:
    default:
        enMsApiXcTimingType = bNtsc ? E_MSAPI_XC_RES_1920x1080I_60Hz : E_MSAPI_XC_RES_1920x1080I_50Hz;
        break;
    }

    return enMsApiXcTimingType;
}

static MS_BOOL _MsDrv_VideoSetXcInit(void)
{
    static MS_BOOL _bXcInited = FALSE;

    if(_bXcInited)
        return TRUE;

    MS_BOOL bMiu1 = FALSE;
    MS_BOOL bForceHdmiOutputMode = FALSE;
    MS_BOOL bBootLogoDisplay = TRUE;
    MS_PHYADDR u32MmapAddr;
    MS_U32 u32MmapLen;
    E_MSAPI_XC_OUTPUT_TIMING_TYPE eXcOutputTiming = _MsDrv_VideoGetEnvOutputTiming();//E_MSAPI_XC_RES_1920x1080I_50Hz;
    MS_U16 u16HdmitxHpdPin = HDMITX_HPD_PM_GPIO;    // HDMITX_HPD_GPIO_NUM
    E_MSAPI_XC_HDMITX_OUTPUT_MODE eHdmitxOutputMode = E_MSAPI_XC_HDMITX_OUTPUT_HDMI_HDCP;
    E_MSAPI_XC_HDMITX_CD_TYPE eHdmitxCDType = E_MSAPI_XC_HDMITX_CD_8BITS;

    MSAPI_XC_INITDATA msAPI_XC_InitData;
    memset(&msAPI_XC_InitData, 0, sizeof(MSAPI_XC_INITDATA));
    MSAPI_XC_HDMITX_BOARD_INFO stHDMITxInfo =
    {
        HDMITX_ANALOG_TUNING_SD,
        HDMITX_ANALOG_TUNING_HD,
        HDMITX_ANALOG_TUNING_DEEP_HD,
    };

    if(_MsDrv_VideoGetMmap(MSDRV_VIDEO_MMAP_ID_XC_MAIN_FB, &u32MmapAddr, &u32MmapLen) == FALSE)
    {
        UBOOT_ERROR("failed to _MsDrv_VideoGetMmap(MSDRV_VIDEO_MMAP_ID_XC_MAIN_FB)!\n");
        return FALSE;
    }

    msAPI_XC_InitData.u32XCMemAddress = u32MmapAddr;
    msAPI_XC_InitData.u32XCMemSize = u32MmapLen;
    msAPI_XC_InitData.eTimingType = eXcOutputTiming;
    msAPI_XC_InitData.u16HdmitxHpdPin = u16HdmitxHpdPin;
    msAPI_XC_InitData.s32CachedPoolID = -1;
    msAPI_XC_InitData.bBootLogoEnable = bBootLogoDisplay;

    //For XC Menu Load, this function to switch XC mode after V-Sync
    msAPI_XC_InitData.bMenuLoadEnable = TRUE;
    if(_MsDrv_VideoGetMmap(MSDRV_VIDEO_MMAP_ID_XC_MENULOAD_BUF, &u32MmapAddr, &u32MmapLen) == FALSE)
    {
        UBOOT_ERROR("failed to _MsDrv_VideoGetMmap(MSDRV_VIDEO_MMAP_ID_XC_MAIN_FB)!\n");
        return FALSE;
    }

    msAPI_XC_InitData.u32XCMenuLoadAddress = u32MmapAddr;
    msAPI_XC_InitData.u32XCMenuLoadSize = u32MmapLen;

    //For Bandwidth Table selection and to MIU configuration
    msAPI_XC_InitData.eBWTBLType = bMiu1 ? E_MSAPI_XC_BW_TBL_MIU_128X128 : E_MSAPI_XC_BW_TBL_MIU_128;

    //For Output Control
    msAPI_XC_InitData.bDisableHDMI = FALSE;
    msAPI_XC_InitData.bDisableDacHD = FALSE;
    msAPI_XC_InitData.bDisableDacSD = FALSE;
    UBOOT_INFO("Output Control : Set HD(%d), SD(%d), HDMI(%d)]\033[m\n", msAPI_XC_InitData.bDisableDacHD, msAPI_XC_InitData.bDisableDacSD, msAPI_XC_InitData.bDisableHDMI);

    //For HDMITx Configuration
    msAPI_XC_InitData.eHdmitxOutputMode = eHdmitxOutputMode;
    msAPI_XC_InitData.eHdmitxCDType = eHdmitxCDType;
    memcpy(&msAPI_XC_InitData.stHdmitxBoardInfo, &stHDMITxInfo, sizeof(MSAPI_XC_HDMITX_BOARD_INFO));

    MApi_HDMITx_DisableRegWrite(bBootLogoDisplay);

    msAPI_XC_Init(msAPI_XC_InitData);
    msAPI_XC_SetVideoMuteByMode(TRUE,MAIN_WINDOW,E_MSAPI_XC_VIDEO_MUTE_MODE_IMMEDIATE);
   
    if(bForceHdmiOutputMode)
    {
        HDMITX_OUTPUT_MODE eHdmiOutputMode = HDMITX_DVI;
        switch(eHdmitxOutputMode)
        {
            case E_MSAPI_XC_HDMITX_OUTPUT_HDMI:
                eHdmiOutputMode = HDMITX_HDMI;
                break;
            case E_MSAPI_XC_HDMITX_OUTPUT_HDMI_HDCP:
                eHdmiOutputMode = HDMITX_HDMI_HDCP;
                break;
            case E_MSAPI_XC_HDMITX_OUTPUT_DVI:
                eHdmiOutputMode = HDMITX_DVI;
                break;
            case E_MSAPI_XC_HDMITX_OUTPUT_DVI_HDCP:
                eHdmiOutputMode = HDMITX_DVI_HDCP;
                break;
            default:
                break;

        }

        MApi_HDMITx_ForceHDMIOutputMode(TRUE, eHdmiOutputMode);
        if(!bBootLogoDisplay)
        {
            MApi_HDMITx_EdidChecking();
            MApi_HDMITx_Exhibit();
        }
    }

    //Set DTV path to Scaler
    msAPI_XC_SetConnect(INPUT_SOURCE_DTV);

    _bXcInited = TRUE;

    //msAPI_XC_SetDebugLevel(E_MSAPI_XC_DBG_LEVEL_DRV);// Eason test

    return TRUE;
}


static MS_BOOL _MsDrv_VideoSetXcVdecInfo(VDEC_StreamId *pStreamId, VDEC_EX_DispInfo *pstVidStatus)
{
    VDEC_DispInfo stVDEC_DispInfo;
    memset(&stVDEC_DispInfo, 0, sizeof(VDEC_DispInfo));

    stVDEC_DispInfo.u16HorSize        = pstVidStatus->u16HorSize;
    stVDEC_DispInfo.u16VerSize        = pstVidStatus->u16VerSize;
    stVDEC_DispInfo.u32FrameRate      = pstVidStatus->u32FrameRate;
    stVDEC_DispInfo.u8Interlace       = pstVidStatus->u8Interlace;
    stVDEC_DispInfo.u8AFD             = pstVidStatus->u8AFD;
    stVDEC_DispInfo.u16SarWidth       = pstVidStatus->u16SarWidth;
    stVDEC_DispInfo.u16SarHeight      = pstVidStatus->u16SarHeight;
    stVDEC_DispInfo.u16CropRight      = pstVidStatus->u16CropRight;
    stVDEC_DispInfo.u16CropLeft       = pstVidStatus->u16CropLeft;
    stVDEC_DispInfo.u16CropBottom     = pstVidStatus->u16CropBottom;
    stVDEC_DispInfo.u16CropTop        = pstVidStatus->u16CropTop;
    stVDEC_DispInfo.u16Pitch          = pstVidStatus->u16Pitch;
    stVDEC_DispInfo.u16PTSInterval    = pstVidStatus->u16PTSInterval;
    stVDEC_DispInfo.u8MPEG1           = pstVidStatus->u8MPEG1;
    stVDEC_DispInfo.u8PlayMode        = pstVidStatus->u8PlayMode;
    stVDEC_DispInfo.u8FrcMode         = pstVidStatus->u8FrcMode;
    stVDEC_DispInfo.u8AspectRate      = pstVidStatus->u8AspectRate;
    stVDEC_DispInfo.bWithChroma       = pstVidStatus->bWithChroma;
    stVDEC_DispInfo.bColorInXVYCC     = pstVidStatus->bColorInXVYCC;
    stVDEC_DispInfo.u32DynScalingAddr = pstVidStatus->u32DynScalingAddr;
    stVDEC_DispInfo.u32DynScalingSize = pstVidStatus->u32DynScalingSize;
    stVDEC_DispInfo.u8DynScalingDepth = pstVidStatus->u8DynScalingDepth;
    stVDEC_DispInfo.bEnableMIUSel     = pstVidStatus->bEnableMIUSel;
    stVDEC_DispInfo.u32AspectWidth    = pstVidStatus->u32AspectWidth;
    stVDEC_DispInfo.u32AspectHeight   = pstVidStatus->u32AspectHeight;

    if(msAPI_XC_SetVDECInfo(stVDEC_DispInfo) == FALSE)
    {
        UBOOT_ERROR("Error: failed to msAPI_XC_SetVDECInfo() !\n");
        return FALSE;
    }

    return TRUE;
}

static MS_BOOL _MsDrv_VideoDoInfoChg(VDEC_StreamId *pstVdecStreamId)
{
    VDEC_EX_DispInfo stDispInfo;
    VDEC_EX_Result eResult = E_VDEC_EX_FAIL;
    MVOP_InputSel eMvopInputSel = MVOP_INPUT_UNKNOWN;
    if(pstVdecStreamId == NULL)
    {
        UBOOT_ERROR("Error: NULL pstVdecStreamId!\n");
        return FALSE;
    }

    MApi_VDEC_EX_SetBlueScreen(pstVdecStreamId, FALSE);
    memset(&stDispInfo, 0, sizeof(VDEC_EX_DispInfo));
    eResult = MApi_VDEC_EX_GetDispInfo(pstVdecStreamId, &stDispInfo);
    if(eResult != E_VDEC_EX_OK)
    {
        UBOOT_ERROR("Error 0x%x: failed to MApi_VDEC_EX_GetDispInfo()!\n", eResult);
        return FALSE;
    }

    eMvopInputSel = _MsDrv_VideoGetMvopInputSel(pstVdecStreamId);
    _MsDrv_VideoSetXcVdecInfo(pstVdecStreamId, &stDispInfo);
    if(msAPI_XC_SetMVOPConfig(eMvopInputSel) != TRUE)
    {
        UBOOT_ERROR("Error: failed to msAPI_XC_SetMVOPConfig()!\n");
        return FALSE;
    }

    MsDrv_VideoPause();
    UBOOT_DEBUG("Disp cnt is %ld, decode cnt is %ld\n", MsDrv_VideoGetDispCnt(), MsDrv_VideoGetDecFrmCnt());
    msAPI_XC_SetWin(NULL, NULL, NULL, MAIN_WINDOW); // set full screen 

    MApi_VDEC_EX_AVSyncOn(pstVdecStreamId, TRUE, 120, 20);

    return TRUE;
}

static MS_BOOL _MsDrv_VideoDoFirstFrame(VDEC_StreamId *pstVdecStreamId)
{
    if(pstVdecStreamId == NULL)
    {
        UBOOT_ERROR("Error: NULL pstVdecStreamId!\n");
        return FALSE;
    }

    MApi_VDEC_EX_SetControl(pstVdecStreamId, E_VDEC_EX_USER_CMD_AVSYNC_REPEAT_TH, 0xFF);

    return TRUE;
}


static void _MsDrv_VideoCallback(MS_U32 u32Event, void *param)
{
    VDEC_StreamId *pstVdecStreamId = (VDEC_StreamId*)param;
    if(pstVdecStreamId == NULL)
    {
        UBOOT_ERROR("Error: NULL user parameter!\n");
        return;
    }

    if(u32Event & E_VDEC_EX_EVENT_DISP_INFO_CHG)
    {
        UBOOT_INFO("E_VDEC_EX_EVENT_DISP_INFO_CHG\n");
        //  TODO:
        //_MsDrv_VideoDoInfoChg(pstVdecStreamId);    // Some flow of XC/MVOP configuration call the MsOS_DelayTask() which is not allow be called in ISR.
        _bVdecCallbkInfoChg = TRUE;
    }

    if(u32Event & E_VDEC_EX_EVENT_FIRST_FRAME)
    {
        UBOOT_INFO("E_VDEC_EX_EVENT_FIRST_FRAME\n");
        //  TODO:
        // _MsDrv_VideoDoFirstFrame(pstVdecStreamId);
        _bVdecRecvFirstFrame = TRUE;
    }
}

void MsDrv_VideoRoutinePolling(void)
{
    VDEC_StreamId *pstVdecStreamId = &_stVdecStreamId;
    VDEC_EX_DispInfo _pDispinfo;
    if(pstVdecStreamId == NULL)
        return;

    switch(_eVdecRoutineStateMachine)
    {
        case E_MSDRV_VDEC_SM_IDLE:
            break;

        case E_MSDRV_VDEC_SM_WAIT_INFO_CHANGE:
            if (MApi_VDEC_EX_IsSeqChg(pstVdecStreamId)!= E_VDEC_EX_OK)
                break;

            MApi_VDEC_EX_GetDispInfo(pstVdecStreamId,&_pDispinfo);
            printf("H %d V %d \n",_pDispinfo.u16HorSize,_pDispinfo.u16VerSize);
            _bVdecCallbkInfoChg = FALSE;
            _eVdecRoutineStateMachine = E_MSDRV_VDEC_SM_DO_INFO_CHANGE;
            break;

        case E_MSDRV_VDEC_SM_DO_INFO_CHANGE:
            if(_MsDrv_VideoDoInfoChg(pstVdecStreamId) == FALSE)
            {
                UBOOT_ERROR("Error: failed to _MsDrv_VideoDoInfoChg()\n");
                _eVdecRoutineStateMachine = E_MSDRV_VDEC_SM_IDLE;
                break;
            }

            _eVdecRoutineStateMachine = E_MSDRV_VDEC_SM_WAIT_FIRST_FRAME;
            break;

        case E_MSDRV_VDEC_SM_WAIT_FIRST_FRAME:
            if(MsDrv_VideoGetDecFrmCnt() != 0)
            {
                _eVdecRoutineStateMachine = E_MSDRV_VDEC_SM_DO_FIRST_FRAME;
            }
            break;

        case E_MSDRV_VDEC_SM_DO_FIRST_FRAME:
            if(_MsDrv_VideoDoFirstFrame(pstVdecStreamId) == FALSE)
            {
                UBOOT_ERROR("Error: failed to _MsDrv_VideoDoFirstFrame()\n");
                _eVdecRoutineStateMachine = E_MSDRV_VDEC_SM_IDLE;
                break;
            }

            _eVdecRoutineStateMachine = E_MSDRV_VDEC_SM_PLAYING;
            break;

        case E_MSDRV_VDEC_SM_PLAYING:
            if(_bVdecCallbkInfoChg)
            {
                _eVdecRoutineStateMachine = E_MSDRV_VDEC_SM_DO_INFO_CHANGE;
                break;
            }
            break;

        default:
            break;
    }
}

MS_BOOL MsDrv_VideoSetXcMvopCfg(void)
{
    VDEC_StreamId *pstVdecStreamId = &_stVdecStreamId;

    if(pstVdecStreamId == NULL)
        return 0;

    VDEC_EX_Result eResult = E_VDEC_EX_FAIL;
    VDEC_EX_DispInfo stDispInfo;
    memset(&stDispInfo, 0, sizeof(VDEC_EX_DispInfo));

    eResult = MApi_VDEC_EX_GetDispInfo(pstVdecStreamId, &stDispInfo);
    if (E_VDEC_EX_OK != eResult)
    {
        UBOOT_ERROR("Error 0x%x: failed to MApi_VDEC_EX_GetDispInfo(...)!\n", eResult);
        return FALSE;
    }
    else
    {
        // PATCH : some 1080 stream don't have crop information,
        if((stDispInfo.u16VerSize == 1088) && \
                (stDispInfo.u16CropBottom == 0) && \
                (stDispInfo.u16CropTop == 0))
        {
            stDispInfo.u16CropBottom = 8;
        }

    }

    if((stDispInfo.u16HorSize == 0) || (stDispInfo.u16VerSize == 0))
    {
        memset(&stDispInfo, 0, sizeof(VDEC_EX_DispInfo));
        UBOOT_ERROR("_MI_VIDEO_GetInfo failed\n");
        return FALSE;
    }

    msAPI_XC_SetVDECInfo_EX(pstVdecStreamId, &stDispInfo);

    if(TRUE != msAPI_XC_SetMVOPConfig(MVOP_INPUT_DRAM))
    {
        UBOOT_ERROR(" Video Set Output Config Failed!!\n");
        return FALSE;
    }

    return TRUE;
}


MS_U32 MsDrv_VideoGetDecFrmCnt(void)
{
    VDEC_StreamId *pstVdecStreamId = &_stVdecStreamId;

    if(pstVdecStreamId == NULL)
        return 0;

    if(MApi_VDEC_EX_IsFrameRdy(pstVdecStreamId) != E_VDEC_EX_OK)
        return 0;

    return MApi_VDEC_EX_GetFrameCnt(pstVdecStreamId);
}

MS_BOOL MsDrv_VideoIsSyncOn(void)
{
    VDEC_StreamId *pstVdecStreamId = &_stVdecStreamId;

    if(pstVdecStreamId == NULL)
        return FALSE;

    return (MApi_VDEC_EX_IsAVSyncOn(pstVdecStreamId) == E_VDEC_EX_OK);
}


MS_BOOL MsDrv_VideoIsReachSync(MS_BOOL bWaitSync)
{
    VDEC_StreamId *pstVdecStreamId = &_stVdecStreamId;

    if(pstVdecStreamId == NULL)
        return FALSE;

    if(MApi_VDEC_EX_IsFrameRdy(pstVdecStreamId) != E_VDEC_EX_OK)
        return FALSE;

    if(TRUE == bWaitSync)
    {
        return (MApi_VDEC_EX_IsReachSync(pstVdecStreamId) == E_VDEC_EX_OK);
    }
    else
    {
        return TRUE;
    }
}

void MsDrv_VideoUnMute(void)
{
    MApi_XC_WaitOutputVSync(5, 140, MAIN_WINDOW);
    msAPI_XC_SetVideoMuteByMode(FALSE, MAIN_WINDOW, E_MSAPI_XC_VIDEO_MUTE_MODE_IMMEDIATE);
}

void MsDrv_VideoMute(void)
{
    msAPI_XC_SetVideoMuteByMode(TRUE, MAIN_WINDOW, E_MSAPI_XC_VIDEO_MUTE_MODE_IMMEDIATE);
}


MS_PHYADDR MsDrv_VideoGetPtsAddr(MsDrv_VIDEO_CodecType_e eCodecType, MS_U32 *pu32PtsKhzUnit)
{
    MS_PHYADDR u32PtsAddr = 0;

    if(_MsDrv_VideoGetMmap(MSDRV_VIDEO_MMAP_ID_VDEC_AEON, &u32PtsAddr, NULL) == FALSE)
    {
        UBOOT_ERROR("failed to _MsDrv_VideoGetMmap(MSDRV_VIDEO_MMAP_ID_VDEC_AEON)!\n");
        return 0;
    }

    if(_MsDrv_VideoIsHvd(eCodecType))
    {
        // HVD
        u32PtsAddr += MSDRV_VIDEO_HVD_PTS_OFFSET;
        if(pu32PtsKhzUnit)
            *pu32PtsKhzUnit = MSDRV_VIDEO_HVD_PTS_KHZ_UNIT;
    }
    else
    {
        // MVD
        u32PtsAddr += MSDRV_VIDEO_MVD_PTS_OFFSET;
        if(pu32PtsKhzUnit)
            *pu32PtsKhzUnit = MSDRV_VIDEO_MVD_PTS_KHZ_UNIT;
    }

    return u32PtsAddr;
}


MS_BOOL MsDrv_VideoGetFrameCountAddr(MsDrv_VIDEO_CodecType_e eCodecType,MS_PHYADDR *pu32FrameCntAddr, MS_PHYADDR *pu32SkipFrameCntAddr)
{
    MS_PHYADDR u32BaseAddr = 0;
    MS_PHYADDR u32FrameCntAddr = 0;
    MS_PHYADDR u32SkipFrameCntAddr = 0;
    if(_MsDrv_VideoGetMmap(MSDRV_VIDEO_MMAP_ID_VDEC_AEON, &u32BaseAddr, NULL) == FALSE)
    {
        UBOOT_ERROR("failed to _MsDrv_VideoGetMmap(MSDRV_VIDEO_MMAP_ID_VDEC_AEON)!\n");
        return 0;
    }

    if(_MsDrv_VideoIsHvd(eCodecType))
    {
        // HVD
        u32FrameCntAddr     = u32BaseAddr + MSDRV_VIDEO_HVD_FRAME_COUNT_OFFSET;
        u32SkipFrameCntAddr = u32BaseAddr + MSDRV_VIDEO_HVD_SKIP_FRAME_COUNT_OFFSET;
    }
    else
    {
        // MVD
        u32FrameCntAddr     = u32BaseAddr + MSDRV_VIDEO_MVD_FRAME_COUNT_OFFSET;
        u32SkipFrameCntAddr = u32BaseAddr + MSDRV_VIDEO_MVD_SKIP_FRAME_COUNT_OFFSET;
    }
    if (pu32FrameCntAddr != NULL)
    {
        *pu32FrameCntAddr = u32FrameCntAddr;
    }

    if (pu32SkipFrameCntAddr != NULL)
    {
        *pu32SkipFrameCntAddr = u32SkipFrameCntAddr;
    }

    return TRUE;
}


int MsDrv_IFrameStart(MsDrv_VIDEO_StreamType_e eStreamType, MsDrv_VIDEO_CodecType_e eCodecType, char * FileName)
{
    VDEC_EX_Result eResult = E_VDEC_EX_FAIL;
    VDEC_StreamId *pstVdecStreamId = &_stVdecStreamId;
    VDEC_EX_SrcMode eVdecSrcMode = _MsDrv_VideoGetVdecSrcType(eStreamType);
    VDEC_EX_CodecType eVdecCodecType = _MsDrv_VideoGetVdecCodecType(eCodecType);
    //MS_U32 u32VdecCbEvent = E_VDEC_EX_EVENT_DISP_INFO_CHG | E_VDEC_EX_EVENT_FIRST_FRAME;

    if(_MsDrv_VideoSetXcInit() == FALSE)    // XC initialization
    {
        UBOOT_ERROR("Error: failed to _MsDrv_VideoSetXcInit()!\n");
        return -1;
    }
    MsDrv_VideoMute();
    memset(pstVdecStreamId, 0, sizeof(VDEC_StreamId));
    eResult =  MApi_VDEC_EX_GetFreeStream((void*)pstVdecStreamId, sizeof(VDEC_StreamId), E_VDEC_EX_MAIN_STREAM, eVdecCodecType);
    if(eResult != E_VDEC_EX_OK)
    {
        UBOOT_ERROR("Error[%d]: failed to MApi_VDEC_EX_GetFreeStream()!\n", eResult);
        return -1;
    }

    if(_MsDrv_VideoInit(pstVdecStreamId, eVdecSrcMode, eVdecCodecType) == FALSE)
    {
        UBOOT_ERROR("Error: failed to _MsDrv_VideoInit()!\n");
        return -1;
    }

    MS_PHYADDR u32BitAddr = 0;
    MS_U32 u32BitLen = 0;
    if(_MsDrv_VideoGetMmap(MSDRV_VIDEO_MMAP_ID_VDEC_BITSTREAM, &u32BitAddr, &u32BitLen) == FALSE)
    {
        UBOOT_ERROR("failed to _MsDrv_VideoGetMmap(MSDRV_VIDEO_MMAP_ID_VDEC_BITSTREAM)!\n");
        goto VIDEO_START_FAILED;
    }

    MS_PHYADDR u32FrameAddr = 0;
    MS_U32 u32FrameLen = 0;
    if(_MsDrv_VideoGetMmap(MSDRV_VIDEO_MMAP_ID_VDEC_FRAME_BUF, &u32FrameAddr, &u32FrameLen) == FALSE)
    {
        UBOOT_ERROR("failed to _MsDrv_VideoGetMmap(MSDRV_VIDEO_MMAP_ID_VDEC_BITSTREAM)!\n");
        goto VIDEO_START_FAILED;
    }

    char PathBuf[128] = "\0";
    vfs_mount(CUSTOMER);
    snprintf(PathBuf, sizeof(PathBuf), "%s/%s",CUSTOMER_PATH,/*"iframe.mpg"*/FileName);
    UBOOT_DEBUG("PathBuf=%s\n",PathBuf);
    U32 u32LogoSize = vfs_getsize(PathBuf);
    UBOOT_DEBUG("u32LogoSize=0x%x\n",(unsigned int)u32LogoSize);
    vfs_read((void *)PA2NVA(u32BitAddr),PathBuf,0,u32LogoSize);
    if(u32LogoSize == 0)
    {
        UBOOT_ERROR("Fail: no found iframe file. return size is 0 !!\n");
        return -1;
    }

    //MApi_VDEC_EX_SetControl(pstVdecStreamId, E_VDEC_EX_USER_CMD_FD_MASK_DELAY_COUNT, 4);
    //MApi_VDEC_EX_SetControl(pstVdecStreamId, E_VDEC_EX_USER_CMD_SHOW_FIRST_FRAME_DIRECT, 1);
    //MApi_VDEC_EX_SetControl(pstVdecStreamId, E_VDEC_EX_USER_CMD_PUSH_DISPQ_WITH_REF_NUM, TRUE);
    //MApi_VDEC_EX_SetAVSyncFreerunThreshold(pstVdecStreamId, 495000);
    //MApi_VDEC_EX_SetEvent(pstVdecStreamId, u32VdecCbEvent, _MsDrv_VideoCallback, (void*)pstVdecStreamId);

    //_eVdecRoutineStateMachine = E_MSDRV_VDEC_SM_WAIT_INFO_CHANGE;
    //eResult = MApi_VDEC_EX_Play(pstVdecStreamId);
    eResult = MApi_VDEC_EX_MHEG_DecodeIFrame(pstVdecStreamId, u32FrameAddr, u32BitAddr, u32BitAddr+u32LogoSize);
    if(E_VDEC_EX_OK != eResult)
    {
        UBOOT_ERROR("Error[%d]: failed to MApi_VDEC_EX_MHEG_DecodeIFrame(...)!\n", eResult);
        goto VIDEO_START_FAILED;
    }

    UBOOT_INFO("pstVdecStreamId = 0x%x, Video stream type = %d and codec type = %d start succeeded!\n", (unsigned int)pstVdecStreamId, eStreamType, eCodecType);

    return 0;

VIDEO_START_FAILED:

    return -1;
}


int MsDrv_VideoStart(MsDrv_VIDEO_StreamType_e eStreamType, MsDrv_VIDEO_CodecType_e eCodecType)
{
    VDEC_EX_Result eResult = E_VDEC_EX_FAIL;
    VDEC_StreamId *pstVdecStreamId = &_stVdecStreamId;
    VDEC_EX_SrcMode eVdecSrcMode = _MsDrv_VideoGetVdecSrcType(eStreamType);
    VDEC_EX_CodecType eVdecCodecType = _MsDrv_VideoGetVdecCodecType(eCodecType);
    MS_U32 u32VdecCbEvent = E_VDEC_EX_EVENT_DISP_INFO_CHG | E_VDEC_EX_EVENT_FIRST_FRAME;

    if(_MsDrv_VideoSetXcInit() == FALSE)    // XC initialization
    {
        UBOOT_ERROR("Error: failed to _MsDrv_VideoSetXcInit()!\n");
        return -1;
    }

    memset(pstVdecStreamId, 0, sizeof(VDEC_StreamId));
    eResult =  MApi_VDEC_EX_GetFreeStream((void*)pstVdecStreamId, sizeof(VDEC_StreamId), E_VDEC_EX_MAIN_STREAM, eVdecCodecType);
    if(eResult != E_VDEC_EX_OK)
    {
        UBOOT_ERROR("Error[%d]: failed to MApi_VDEC_EX_GetFreeStream()!\n", eResult);
        return -1;
    }

    if(_MsDrv_VideoInit(pstVdecStreamId, eVdecSrcMode, eVdecCodecType) == FALSE)
    {
        UBOOT_ERROR("Error: failed to _MsDrv_VideoInit()!\n");
        return -1;
    }

    MApi_VDEC_EX_SetControl(pstVdecStreamId, E_VDEC_EX_USER_CMD_FD_MASK_DELAY_COUNT, 4);
    MApi_VDEC_EX_SetControl(pstVdecStreamId, E_VDEC_EX_USER_CMD_SHOW_FIRST_FRAME_DIRECT, 1);
    MApi_VDEC_EX_SetControl(pstVdecStreamId, E_VDEC_EX_USER_CMD_PUSH_DISPQ_WITH_REF_NUM, TRUE);
    MApi_VDEC_EX_SetAVSyncFreerunThreshold(pstVdecStreamId, 495000);
    //MApi_VDEC_EX_SetEvent(pstVdecStreamId, u32VdecCbEvent, _MsDrv_VideoCallback, (void*)pstVdecStreamId);

    MDrv_MVOP_Enable(DISABLE);
    _eVdecRoutineStateMachine = E_MSDRV_VDEC_SM_WAIT_INFO_CHANGE;
    eResult = MApi_VDEC_EX_Play(pstVdecStreamId);
    if(E_VDEC_EX_OK != eResult)
    {
        UBOOT_ERROR("Error[%d]: failed to MApi_VDEC_EX_Play(...)!\n", eResult);
        goto VIDEO_START_FAILED;
    }

    //MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE_VIDEO, FALSE);

    UBOOT_INFO("pstVdecStreamId = 0x%x, Video stream type = %d and codec type = %d start succeeded!\n", (unsigned int)pstVdecStreamId, eStreamType, eCodecType);

    return 0;

VIDEO_START_FAILED:

    return -1;
}

int MsDrv_VideoStop(void)
{
    VDEC_EX_Result eResult = E_VDEC_EX_FAIL;
    VDEC_StreamId *pstVdecStreamId = &_stVdecStreamId;

    MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE_VIDEO, TRUE);

    eResult = MApi_VDEC_EX_Exit(pstVdecStreamId);
    if ((eResult != E_VDEC_EX_OK) && (eResult != E_VDEC_EX_RET_NOT_INIT))
    {
        UBOOT_ERROR("Error 0x%x: failed to MApi_VDEC_EX_Exit(...)!\n", eResult);

        goto VIDEO_STOP_FAILED;
    }

    _eVdecRoutineStateMachine = E_MSDRV_VDEC_SM_IDLE;

    UBOOT_INFO("Video stop succeeded!\n");

    return 0;

VIDEO_STOP_FAILED:

    return -1;
}

#include <apiDMX.h>
#include <apiAUDIO.h>
MS_U32 MsDrv_VideoShowDecInfo(void)
{
//    //====================================
//    printf("fd_mask=%d \n", MApi_XC_Get_FD_Mask());
//    //====================================

    MS_U32 u32SystemSTC = 0, u32SystemSTC_H = 0;
    MS_U32 u32ReadPtr = 0, u32WritePtr = 0, u32Vacany = 0;
    MS_U32 u32VPTS = 0, u32APTS = 0;
    MS_S32 s32Adelta = 0;
    MS_U32 u32preFrameCount=0;
    MS_U32 u32preTime=0;
    MS_U32 u32Rate=0;
    MS_U32 u32ADecCnt = 0;

    VDEC_StreamId stStreamId, stGetStreamId;
    MApi_VDEC_EX_GetControl(&stStreamId, E_VDEC_EX_USER_CMD_GET_MAIN_STREAM_ID, (MS_U32 *)&stGetStreamId);

    u32ReadPtr = MApi_VDEC_EX_GetESReadPtr(&stGetStreamId);
    u32WritePtr = MApi_VDEC_EX_GetESWritePtr(&stGetStreamId);
    u32Vacany = MApi_VDEC_EX_GetESBuffVacancy(&stGetStreamId, NULL);
    MApi_DMX_Stc_Get(&u32SystemSTC_H, &u32SystemSTC);
    u32SystemSTC /= 90;
    u32VPTS = MApi_VDEC_EX_GetPTS(&stGetStreamId);

    u32APTS = (MApi_AUDIO_GetCommAudioInfo((Audio_COMM_infoType)Audio_Comm_infoType_ADEC1_33bit_PTS) & 0xFFFFFFFF) / 90;
    s32Adelta = (MApi_AUDIO_GetCommAudioInfo((Audio_COMM_infoType)Audio_Comm_infoType_ADEC1_33bit_STCPTS_DIFF) & 0xFFFFFFFF) / 90;
    u32ADecCnt = MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_ADEC1_okFrmCnt);

    if((MsOS_GetSystemTime() - u32preTime) > 0)
        u32Rate = ((MApi_VDEC_EX_GetFrameCnt(&stGetStreamId) - u32preFrameCount) * 1000) / (MsOS_GetSystemTime() - u32preTime);

    printf("RATE:%ld STC:%lx PTS(V:%lx A:%lx)(I:%ld 1stF:%ld)AV(on:%ld S:%ld R:%ld)(Dec:%lu Err:%lu Skp:%lu Drp:%lu Disp:%lu)ES(R:%6lx W:%6lx V:%6lx)DELTA(V:%4d A:%4d) ADEC(Dec:%lu)\n",
            u32Rate, u32SystemSTC, u32VPTS, u32APTS,
            (MS_U32)MApi_VDEC_EX_IsIFrameFound(&stGetStreamId), (MS_U32)MApi_VDEC_EX_IsFrameRdy(&stGetStreamId),
            (MS_U32)MApi_VDEC_EX_IsAVSyncOn(&stGetStreamId), (MS_U32)MApi_VDEC_EX_IsStartSync(&stGetStreamId), (MS_U32)MApi_VDEC_EX_IsReachSync(&stGetStreamId),
            MApi_VDEC_EX_GetFrameCnt(&stGetStreamId), MApi_VDEC_EX_GetErrCnt(&stGetStreamId), MApi_VDEC_EX_GetSkipCnt(&stGetStreamId), MApi_VDEC_EX_GetDropCnt(&stGetStreamId),MApi_VDEC_EX_GetDispCnt(&stGetStreamId),
            u32ReadPtr, u32WritePtr, u32Vacany,
            (int)(u32VPTS-u32SystemSTC), (int)s32Adelta, u32ADecCnt);

    u32preFrameCount = MApi_VDEC_EX_GetFrameCnt(&stGetStreamId);
    u32preTime = MsOS_GetSystemTime();

    return 0;
}

MS_U32 MsDrv_VideoGetCurPts(void)
{
    MS_U32 u32Vfpts = 0;
    VDEC_StreamId stStreamId, stGetStreamId;

    MApi_VDEC_EX_GetControl(&stStreamId, E_VDEC_EX_USER_CMD_GET_MAIN_STREAM_ID, (MS_U32 *)&stGetStreamId);
    u32Vfpts = MApi_VDEC_EX_GetPTS(&stGetStreamId);
    return u32Vfpts;
}

MS_BOOL MsDrv_VideoPause(void)
{
    VDEC_StreamId *pstVdecStreamId = &_stVdecStreamId;
    if (MApi_VDEC_EX_Pause(pstVdecStreamId) != E_VDEC_EX_OK)
    {
            printf("MApi_VDEC_EX_Pause fail\n");
            return FALSE;
    }
    return TRUE;
}

MS_BOOL MsDrv_VideoResume(void)
{
    VDEC_StreamId *pstVdecStreamId = &_stVdecStreamId;
    UBOOT_DEBUG("IN");
    if (MApi_VDEC_EX_Play(pstVdecStreamId) != E_VDEC_EX_OK)
    {
        printf("MApi_VDEC_EX_Play Main fail\n");
        return FALSE;
    }
    return TRUE;
}

MS_U32 MsDrv_VideoGetDispCnt(void)
{
    VDEC_StreamId *pstVdecStreamId = &_stVdecStreamId;

    if(pstVdecStreamId == NULL)
        return 0;

    return MApi_VDEC_EX_GetDispCnt(pstVdecStreamId);
}



#endif  // #if (CONFIG_BOOTVIDEO == 1)

#if ENABLE_MBOOT_FT_VIDEO
#include <MsTypes.h>
#include <MsDebug.h>
#include <MsOS.h>
#include <apiVDEC_EX.h>
#include <drvMVOP.h>
#include <miu/MsDrvMiu.h>
#include "MsDrvVideo.h"

#include <drvSYS.h>
#include <drvIPAUTH.h>

// for XC +
#include "drvXC_IOPort.h"
#include "apiXC.h"
#include "drvMVOP.h"
#include "apiVDEC.h"
#include "apiHDMITx.h"
#include "xc/msAPI_XC.h"
// for XC -

static VDEC_StreamId _stVdecStreamId = {0, 0};

#define FT_VIDEO_LUMA_CRC               0x008e0f36
#define FT_VIDEO_CHROMA_CRC             0x00412106


#define FT_MMAP_ID_VDEC_AEON_ADR        0x00100000  // 0x00100000  //Alignment 0x10000
#define FT_MMAP_ID_VDEC_AEON_LEN        0x00100000

#define FT_MMAP_ID_VDEC_FRAME_BUF_ADR   (FT_MMAP_ID_VDEC_AEON_ADR + FT_MMAP_ID_VDEC_AEON_LEN)  //Alignment 0x00200
#define FT_MMAP_ID_VDEC_FRAME_BUF_LEN   0x01800000

#define FT_MMAP_ID_VDEC_BITSTREAM_ADR   (FT_MMAP_ID_VDEC_FRAME_BUF_ADR + FT_MMAP_ID_VDEC_FRAME_BUF_LEN)  //Alignment 0x00008
#define FT_MMAP_ID_VDEC_BITSTREAM_LEN   0x00400000

#define FT_MMAP_ID_XC_MAIN_FB_ADR       (FT_MMAP_ID_VDEC_BITSTREAM_ADR + FT_MMAP_ID_VDEC_BITSTREAM_LEN)  //Alignment 0x00008
#define FT_MMAP_ID_XC_MAIN_FB_LEN       0x00A00000

#define FT_MMAP_ID_XC_MENULOAD_BUF      (FT_MMAP_ID_XC_MAIN_FB_ADR + FT_MMAP_ID_XC_MAIN_FB_LEN)
#define FT_MMAP_ID_XC_MENULOAD_LEN      0x00100000

// BD_MST188A_DO1A_S.h +
#define HDMITX_ANALOG_TUNING_SD         {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
#define HDMITX_ANALOG_TUNING_HD         {0x00, 0x00, 0x02, 0x07, 0x00, 0x00}
#define HDMITX_ANALOG_TUNING_DEEP_HD    {0x02, 0x01, 0x0a, 0x07, 0x07, 0x07}
// BD_MST188A_DO1A_S.h -

static const MS_U8 _u8PatternData[] =
{
#include "IPBsdFrame.dat"
};

#if (CONFIG_BOOTVIDEO == 0)
#include <Customer_Info.h>
#else
extern MS_U8 CID_Buf[32];
extern MS_U8 IP_Cntrol_Mapping_1[];
extern MS_U8 IP_Cntrol_Mapping_2[];
extern MS_U8 IP_Cntrol_Mapping_3[];
extern MS_U8 IP_Cntrol_Mapping_4[];
extern MS_U8 Customer_hash[];
#endif

static unsigned char _aucCustomerInfo[49];

static void _MDrv_VideoInitCustomerInfo(MS_U8 *pu8CustomerInfo)
{
    unsigned char * pu8Temp;
    MS_U8 i,u8Data;
    MS_U8 u8ASCII_Mapping[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
    MS_U32 dolbycontrol=0, chiptype=0, reverse=0;

    if(pu8CustomerInfo == NULL)
        return;

#if (DEBUG_CUSTOMER_INFO_ENABLE)
    printf("****** _aucCustomerInfo:Before ******");
    for (i=0;i<48;i++)
    {
        if (i%8 ==0)
            printf("\n");
        printf("0x%bx,",pu8CustomerInfo[i]);

    }

    printf("\n abcdef => 0x%bx,0x%bx,0x%bx,0x%bx,0x%bx,0x%bx,",'a','b','c','d','e','f');
    printf("\n ABCDEF => 0x%bx,0x%bx,0x%bx,0x%bx,0x%bx,0x%bx,",'A','B','C','D','E','F');
    printf("\n\n");
#endif

    pu8Temp = &CID_Buf[6];//(Byte6,7: Customer ID)(Byte8,9: Model ID)(Byte1011: Chip ID)

    //<1>.To Prepare the Customer-ID + Model-ID + Chip-ID
    for (i=0;i<3;i++)
    { //Get hight byte
        u8Data = ((*(pu8Temp+1))>> 4);
        pu8CustomerInfo[0+i*4] = u8ASCII_Mapping[u8Data];
        u8Data = (*(pu8Temp+1) & 0x0F);
        pu8CustomerInfo[1+i*4] =  u8ASCII_Mapping[u8Data];
        //Get Low byte
        u8Data = ((*(pu8Temp))>>4) ;
        pu8CustomerInfo[2+i*4] = u8ASCII_Mapping[u8Data];
        u8Data = (*(pu8Temp) & 0x0F);
        pu8CustomerInfo[3+i*4] = u8ASCII_Mapping[u8Data];
        pu8Temp +=2;
    }

    //support Hash Key v2.0
    dolbycontrol = MDrv_SYS_QueryDolbyHashInfo(E_SYS_DOLBY_CONTROL_BIT);
    chiptype = MDrv_SYS_GetChipType();
    reverse = MDrv_SYS_QueryDolbyHashInfo(E_SYS_DOLBY_REVERSE_BIT);
    if(dolbycontrol == 1 && chiptype == 1 && reverse == 0)
    {
        printf("\033[1;32m *** hash key v2.0, use _aucCustomerInfo_V2.h to replace _aucCustomerInfo.h *** \033[0m \r\n");
        //Get hight byte
        u8Data = ((*(pu8Temp + 1)) >> 4);
        pu8CustomerInfo[0 + 12] = u8ASCII_Mapping[u8Data];
        u8Data = (*(pu8Temp + 1) & 0x0F);
        pu8CustomerInfo[1 + 12] =  u8ASCII_Mapping[u8Data];
        //Get Low byte
        u8Data = ((*(pu8Temp)) >> 4) ;
        pu8CustomerInfo[2 + 12] = u8ASCII_Mapping[u8Data];
        u8Data = (*(pu8Temp) & 0x0F);
        pu8CustomerInfo[3 + 12] = u8ASCII_Mapping[u8Data];
    }
    else
    {
        printf("\033[1;32m *** hash key v1.0, use _aucCustomerInfo.h directly *** \033[0m \r\n");
        for (i=0;i<4;i++)
            pu8CustomerInfo[12+i] = 0x30;
    }

//<2>.To prepare the Customer IP
      pu8Temp = &IP_Cntrol_Mapping_1[0];
      memcpy(&pu8CustomerInfo[16], pu8Temp, 8);
      pu8Temp = &IP_Cntrol_Mapping_2[0];
      memcpy(&pu8CustomerInfo[16+8], pu8Temp, 8);
      pu8Temp = &IP_Cntrol_Mapping_3[0];
      memcpy(&pu8CustomerInfo[16+16], pu8Temp, 8);
      pu8Temp = &IP_Cntrol_Mapping_4[0];
      memcpy(&pu8CustomerInfo[16+24], pu8Temp, 8);

#if (DEBUG_CUSTOMER_INFO_ENABLE)
    printf("****** _aucCustomerInfo ******");
    for (i=0;i<48;i++)
    {
        if (i%8 ==0)
            printf("\n");
        printf("0x%bx,",pu8CustomerInfo[i]);
    }
#endif

    pu8CustomerInfo[48] = 0;  //Important, string end symbol
}

static void _MsDrv_VideoSetSecurityAuthentication(void)
{
    memset(_aucCustomerInfo, 0, sizeof(_aucCustomerInfo));
    _MDrv_VideoInitCustomerInfo(_aucCustomerInfo);

    MApi_AUTH_Process(_aucCustomerInfo, Customer_hash);
}

static int _MsDrv_VideoDoFrameCheckSum(VDEC_StreamId *pstVdecStrmId)
{
    int i, j;
    MS_U8 *pu8Data = NULL;
    MS_U32 u32LumaCheckSum = 0;
    MS_U32 u32ChromaCheckSum = 0;
    MS_U32 u32Width = 0;
    MS_U32 u32Height = 0;
    MS_U32 u32PitchOffset = 0;

    VDEC_EX_FrameInfo stFrameInfo;
    memset(&stFrameInfo, 0, sizeof(stFrameInfo));

    printf("\033[33m Decoded = %ld, Error = %ld, Drop = %ld, Skip = %ld\033[m\n",
        MApi_VDEC_EX_GetFrameCnt(pstVdecStrmId), MApi_VDEC_EX_GetErrCnt(pstVdecStrmId),
        MApi_VDEC_EX_GetDropCnt(pstVdecStrmId), MApi_VDEC_EX_GetSkipCnt(pstVdecStrmId));

//    if(MApi_VDEC_EX_GetDispFrameInfo(pstVdecStrmId, &stFrameInfo) != E_VDEC_EX_OK)
    if(MApi_VDEC_EX_GetDecFrameInfo(pstVdecStrmId, &stFrameInfo) != E_VDEC_EX_OK)
    {
        UBOOT_ERROR("Error: failed to MApi_VDEC_EX_GetDecFrameInfo()\n");
        return -1;
    }

    printf("\033[33m FrameType = %d, width = %d, height = %d, pitch = %d\033[m\n",
        stFrameInfo.eFrameType, stFrameInfo.u16Width, stFrameInfo.u16Height, stFrameInfo.u16Pitch);
    printf("\033[33m LUMA   addr = 0x%lx, size = 0x%x\033[m\n",  stFrameInfo.u32LumaAddr, ((unsigned int)stFrameInfo.u16Pitch * stFrameInfo.u16Height));
    printf("\033[33m CHROMA addr = 0x%lx, size = 0x%x\033[m\n", stFrameInfo.u32ChromaAddr, ((unsigned int)stFrameInfo.u16Pitch * stFrameInfo.u16Height / 2));

    // check luma
    pu8Data = (MS_U8*)MS_PA2KSEG1(stFrameInfo.u32LumaAddr);
    if(pu8Data == NULL)
    {
        UBOOT_ERROR("Error: failed to MS_PA2KSEG1(0x%lx) for LUMA \n", stFrameInfo.u32LumaAddr);
        return -1;
    }

    u32Width = ((MS_U32)stFrameInfo.u16Width << 5); // width x 32
    u32PitchOffset = (((MS_U32)stFrameInfo.u16Pitch - stFrameInfo.u16Width) << 5); // pitch x 32 - width x 32
    u32Height = ((MS_U32)stFrameInfo.u16Height >> 5);   // height / 32
    u32LumaCheckSum = 0;
    for(i = 0; i < u32Height; i++)
    {
        for(j = 0; j < u32Width; j++)
            u32LumaCheckSum += *pu8Data++;

        pu8Data += u32PitchOffset;
    }

    printf("\033[33m LUMA   check sum = 0x%08x, reslut = 0x%08lx\033[m\n", FT_VIDEO_LUMA_CRC, u32LumaCheckSum);

    // check chroma
    pu8Data = (MS_U8*)MS_PA2KSEG1(stFrameInfo.u32ChromaAddr);
    if(pu8Data == NULL)
    {
        UBOOT_ERROR("Error: failed to MS_PA2KSEG1(0x%lx) for CHROMA \n", stFrameInfo.u32ChromaAddr);
        return -1;
    }

    u32Height = (u32Height >> 1);   // chroma size is half of luma size
    u32ChromaCheckSum = 0;
    for(i = 0; i < u32Height; i++)
    {
        for(j = 0; j < u32Width; j++)
            u32ChromaCheckSum += *pu8Data++;

        pu8Data += u32PitchOffset;
    }

    printf("\033[33m CHROMA check sum = 0x%08x, reslut = 0x%08lx\033[m\n", FT_VIDEO_CHROMA_CRC, u32ChromaCheckSum);

    if((u32LumaCheckSum != FT_VIDEO_LUMA_CRC) || (u32ChromaCheckSum != FT_VIDEO_CHROMA_CRC))
    {
        printf("\033[31m Check sum is wrong!\033[m\n");
        return -1;
    }

    return 0;
}

static MS_BOOL _MsDrv_VideoSetXcInit(void)
{
    static MS_BOOL _bXcInited = FALSE;

    if(_bXcInited)
        return TRUE;
        
    MS_BOOL bForceHdmiOutputMode = FALSE;
    MS_BOOL bBootLogoDisplay = FALSE;
    E_MSAPI_XC_OUTPUT_TIMING_TYPE eXcOutputTiming = E_MSAPI_XC_RES_1920x1080P_60Hz;  
    MS_U16 u16HdmitxHpdPin = HDMITX_HPD_PM_GPIO;    // HDMITX_HPD_GPIO_NUM
    E_MSAPI_XC_HDMITX_OUTPUT_MODE eHdmitxOutputMode = E_MSAPI_XC_HDMITX_OUTPUT_HDMI;
    E_MSAPI_XC_HDMITX_CD_TYPE eHdmitxCDType = E_MSAPI_XC_HDMITX_CD_8BITS;

    MSAPI_XC_INITDATA msAPI_XC_InitData;
    memset(&msAPI_XC_InitData, 0, sizeof(MSAPI_XC_INITDATA));
    MSAPI_XC_HDMITX_BOARD_INFO stHDMITxInfo =
    {
        HDMITX_ANALOG_TUNING_SD,
        HDMITX_ANALOG_TUNING_HD,
        HDMITX_ANALOG_TUNING_DEEP_HD,
    };

    msAPI_XC_InitData.u32XCMemAddress = FT_MMAP_ID_XC_MAIN_FB_ADR;
    msAPI_XC_InitData.u32XCMemSize = FT_MMAP_ID_XC_MAIN_FB_LEN;
    msAPI_XC_InitData.eTimingType = eXcOutputTiming;
    msAPI_XC_InitData.u16HdmitxHpdPin = u16HdmitxHpdPin;
    msAPI_XC_InitData.s32CachedPoolID = -1;
    msAPI_XC_InitData.bBootLogoEnable = bBootLogoDisplay;

    //For XC Menu Load, this function to switch XC mode after V-Sync
    msAPI_XC_InitData.bMenuLoadEnable = FALSE;

    msAPI_XC_InitData.u32XCMenuLoadAddress = FT_MMAP_ID_XC_MENULOAD_BUF;
    msAPI_XC_InitData.u32XCMenuLoadSize = FT_MMAP_ID_XC_MENULOAD_LEN;

    //For Bandwidth Table selection and to MIU configuration
    msAPI_XC_InitData.eBWTBLType = E_MSAPI_XC_BW_TBL_MIU_128;

    //For Output Control
    msAPI_XC_InitData.bDisableHDMI = FALSE;
    msAPI_XC_InitData.bDisableDacHD = FALSE;
    msAPI_XC_InitData.bDisableDacSD = FALSE;
    UBOOT_INFO("Output Control : Set HD(%d), SD(%d), HDMI(%d)]\033[m\n", msAPI_XC_InitData.bDisableDacHD, msAPI_XC_InitData.bDisableDacSD, msAPI_XC_InitData.bDisableHDMI);

    //For HDMITx Configuration
    msAPI_XC_InitData.eHdmitxOutputMode = eHdmitxOutputMode;
    msAPI_XC_InitData.eHdmitxCDType = eHdmitxCDType;
    memcpy(&msAPI_XC_InitData.stHdmitxBoardInfo, &stHDMITxInfo, sizeof(MSAPI_XC_HDMITX_BOARD_INFO));
    printf("%s() done\n", __FUNCTION__);

    MApi_HDMITx_DisableRegWrite(bBootLogoDisplay);

    msAPI_XC_Init(msAPI_XC_InitData);
    MsDrv_VideoMute();
    if(bForceHdmiOutputMode)
    {
        HDMITX_OUTPUT_MODE eHdmiOutputMode = HDMITX_DVI;
        switch(eHdmitxOutputMode)
        {
            case E_MSAPI_XC_HDMITX_OUTPUT_HDMI:
                eHdmiOutputMode = HDMITX_HDMI;
                break;
            case E_MSAPI_XC_HDMITX_OUTPUT_HDMI_HDCP:
                eHdmiOutputMode = HDMITX_HDMI_HDCP;
                break;
            case E_MSAPI_XC_HDMITX_OUTPUT_DVI:
                eHdmiOutputMode = HDMITX_DVI;
                break;
            case E_MSAPI_XC_HDMITX_OUTPUT_DVI_HDCP:
                eHdmiOutputMode = HDMITX_DVI_HDCP;
                break;
            default:
                break;

        }

        MApi_HDMITx_ForceHDMIOutputMode(TRUE, eHdmiOutputMode);
        if(!bBootLogoDisplay)
        {
            MApi_HDMITx_EdidChecking();
            MApi_HDMITx_Exhibit();
        }
    }

    //Set DTV path to Scaler
    msAPI_XC_SetConnect(INPUT_SOURCE_DTV);

    _bXcInited = TRUE;

    msAPI_XC_SetDebugLevel(E_MSAPI_XC_DBG_LEVEL_PQ);// Eason test

    return TRUE;
}


static MS_BOOL _MsDrv_VideoSetXcMvopCfg(MVOP_InputSel eMvopInputSel)
{
    VDEC_StreamId *pstVdecStreamId = &_stVdecStreamId;

    if(pstVdecStreamId == NULL)
        return FALSE;

    VDEC_EX_Result eResult = E_VDEC_EX_FAIL;
    VDEC_EX_DispInfo stDispInfo;
    memset(&stDispInfo, 0, sizeof(VDEC_EX_DispInfo));

    eResult = MApi_VDEC_EX_GetDispInfo(pstVdecStreamId, &stDispInfo);
    if (E_VDEC_EX_OK != eResult)
    {
        UBOOT_ERROR("Error 0x%x: failed to MApi_VDEC_EX_GetDispInfo(...)!\n", eResult);
        return FALSE;
    }
    else
    {
        // PATCH : some 1080 stream don't have crop information,
        if((stDispInfo.u16VerSize == 1088) && \
                (stDispInfo.u16CropBottom == 0) && \
                (stDispInfo.u16CropTop == 0))
        {
            stDispInfo.u16CropBottom = 8;
        }

    }

    if((stDispInfo.u16HorSize == 0) || (stDispInfo.u16VerSize == 0))
    {
        memset(&stDispInfo, 0, sizeof(VDEC_EX_DispInfo));
        UBOOT_ERROR("_MI_VIDEO_GetInfo failed\n");
        return FALSE;
    }

    msAPI_XC_SetVDECInfo_EX(pstVdecStreamId, &stDispInfo);
    if(TRUE != msAPI_XC_SetMVOPConfig(eMvopInputSel))
    {
        UBOOT_ERROR(" Video Set Output Config Failed!!\n");
        return FALSE;
    }


    msAPI_XC_SetWin(NULL, NULL, NULL, MAIN_WINDOW); // set full screen
    MApi_VDEC_EX_AVSyncOn(pstVdecStreamId, FALSE, 120, 20);

    MApi_XC_WaitOutputVSync(5, 140, MAIN_WINDOW);
    msAPI_XC_SetVideoMuteByMode(FALSE, MAIN_WINDOW, E_MSAPI_XC_VIDEO_MUTE_MODE_IMMEDIATE);

    return TRUE;
}

static MS_BOOL _MsDrv_VideoSetXcVdecInfo(VDEC_StreamId *pStreamId, VDEC_EX_DispInfo *pstVidStatus)
{
    VDEC_DispInfo stVDEC_DispInfo;
    memset(&stVDEC_DispInfo, 0, sizeof(VDEC_DispInfo));

    stVDEC_DispInfo.u16HorSize        = pstVidStatus->u16HorSize;
    stVDEC_DispInfo.u16VerSize        = pstVidStatus->u16VerSize;
    stVDEC_DispInfo.u32FrameRate      = pstVidStatus->u32FrameRate;
    stVDEC_DispInfo.u8Interlace       = pstVidStatus->u8Interlace;
    stVDEC_DispInfo.u8AFD             = pstVidStatus->u8AFD;
    stVDEC_DispInfo.u16SarWidth       = pstVidStatus->u16SarWidth;
    stVDEC_DispInfo.u16SarHeight      = pstVidStatus->u16SarHeight;
    stVDEC_DispInfo.u16CropRight      = pstVidStatus->u16CropRight;
    stVDEC_DispInfo.u16CropLeft       = pstVidStatus->u16CropLeft;
    stVDEC_DispInfo.u16CropBottom     = pstVidStatus->u16CropBottom;
    stVDEC_DispInfo.u16CropTop        = pstVidStatus->u16CropTop;
    stVDEC_DispInfo.u16Pitch          = pstVidStatus->u16Pitch;
    stVDEC_DispInfo.u16PTSInterval    = pstVidStatus->u16PTSInterval;
    stVDEC_DispInfo.u8MPEG1           = pstVidStatus->u8MPEG1;
    stVDEC_DispInfo.u8PlayMode        = pstVidStatus->u8PlayMode;
    stVDEC_DispInfo.u8FrcMode         = pstVidStatus->u8FrcMode;
    stVDEC_DispInfo.u8AspectRate      = pstVidStatus->u8AspectRate;
    stVDEC_DispInfo.bWithChroma       = pstVidStatus->bWithChroma;
    stVDEC_DispInfo.bColorInXVYCC     = pstVidStatus->bColorInXVYCC;
    stVDEC_DispInfo.u32DynScalingAddr = pstVidStatus->u32DynScalingAddr;
    stVDEC_DispInfo.u32DynScalingSize = pstVidStatus->u32DynScalingSize;
    stVDEC_DispInfo.u8DynScalingDepth = pstVidStatus->u8DynScalingDepth;
    stVDEC_DispInfo.bEnableMIUSel     = pstVidStatus->bEnableMIUSel;
    stVDEC_DispInfo.u32AspectWidth    = pstVidStatus->u32AspectWidth;
    stVDEC_DispInfo.u32AspectHeight   = pstVidStatus->u32AspectHeight;

    if(msAPI_XC_SetVDECInfo(stVDEC_DispInfo) == FALSE)
    {
        UBOOT_ERROR("Error: failed to msAPI_XC_SetVDECInfo() !\n");
        return FALSE;
    }

    return TRUE;
}

static MS_BOOL _MsDrv_VideoDoInfoChg(VDEC_StreamId *pstVdecStreamId)
{
    VDEC_EX_DispInfo stDispInfo;
    VDEC_EX_Result eResult = E_VDEC_EX_FAIL;
    MVOP_InputSel eMvopInputSel = MVOP_INPUT_DRAM;
    if(pstVdecStreamId == NULL)
    {
        UBOOT_ERROR("Error: NULL pstVdecStreamId!\n");
        return FALSE;
    }

    MApi_VDEC_EX_SetBlueScreen(pstVdecStreamId, FALSE);
    memset(&stDispInfo, 0, sizeof(VDEC_EX_DispInfo));
    eResult = MApi_VDEC_EX_GetDispInfo(pstVdecStreamId, &stDispInfo);
    if(eResult != E_VDEC_EX_OK)
    {
        UBOOT_ERROR("Error 0x%x: failed to MApi_VDEC_EX_GetDispInfo()!\n", eResult);
        return FALSE;
    }

    _MsDrv_VideoSetXcVdecInfo(pstVdecStreamId, &stDispInfo);
    if(msAPI_XC_SetMVOPConfig(eMvopInputSel) != TRUE)
    {
        UBOOT_ERROR("Error: failed to msAPI_XC_SetMVOPConfig()!\n");
        return FALSE;
    }

    msAPI_XC_SetWin(NULL, NULL, NULL, MAIN_WINDOW); // set full screen
    MsDrv_VideoPause();
    UBOOT_DEBUG("Disp cnt is %ld, decode cnt is %ld\n", MsDrv_VideoGetDispCnt(), MsDrv_VideoGetDecFrmCnt());

    MApi_VDEC_EX_AVSyncOn(pstVdecStreamId, FALSE, 120, 20);

    MApi_XC_WaitOutputVSync(5, 140, MAIN_WINDOW);
    msAPI_XC_SetVideoMuteByMode(FALSE, MAIN_WINDOW, E_MSAPI_XC_VIDEO_MUTE_MODE_IMMEDIATE);

    return TRUE;
}


int MsDrv_VideoFT(void)
{
    VDEC_EX_Result eResult = E_VDEC_EX_FAIL;
    VDEC_EX_InitParam initParam;
    VDEC_EX_SrcMode eVdecSrcMode = E_VDEC_EX_SRC_MODE_FILE;
    VDEC_EX_CodecType eVdecCodecType = E_VDEC_EX_CODEC_TYPE_MPEG2;
    MS_U32 u32BitstreamBufAddr = 0;
    MS_U32 u32FrameBufAddr = 0;
    VDEC_EX_DecCmd stCmd;
    MS_U8 *pu8Virt = NULL;

    // MIU init
    MsDrv_MiuInit();
    MDrv_MVOP_Init();
    _MsDrv_VideoSetSecurityAuthentication();

    _MsDrv_VideoSetXcInit();
    eResult = MApi_VDEC_EX_GetFreeStream((void*)&_stVdecStreamId, sizeof(VDEC_StreamId), E_VDEC_EX_MAIN_STREAM, eVdecCodecType);
    if(eResult != E_VDEC_EX_OK)
    {
        UBOOT_ERROR("Error[%d]: failed to MApi_VDEC_EX_GetFreeStream()!\n", eResult);
        return -1;
    }

    memset(&initParam, 0, sizeof(VDEC_EX_InitParam));
    initParam.eCodecType = eVdecCodecType;
    initParam.EnableDynaScale = FALSE;
    initParam.bDisableDropErrFrame = TRUE;
    initParam.bDisableErrConceal = FALSE;
    initParam.bRepeatLastField = TRUE;
    initParam.VideoInfo.eSrcMode = eVdecSrcMode;
    initParam.VideoInfo.eTimeStampType = E_VDEC_EX_TIME_STAMP_NONE;
    initParam.SysConfig.u32FWBinaryAddr = initParam.SysConfig.u32CodeBufAddr = FT_MMAP_ID_VDEC_AEON_ADR;
    initParam.SysConfig.u32FWBinarySize = initParam.SysConfig.u32CodeBufSize = FT_MMAP_ID_VDEC_AEON_LEN;
    initParam.SysConfig.u32BitstreamBufAddr = u32BitstreamBufAddr = FT_MMAP_ID_VDEC_BITSTREAM_ADR;
    initParam.SysConfig.u32BitstreamBufSize = FT_MMAP_ID_VDEC_BITSTREAM_LEN;
    initParam.SysConfig.u32FrameBufAddr = u32FrameBufAddr = FT_MMAP_ID_VDEC_FRAME_BUF_ADR;
    initParam.SysConfig.u32FrameBufSize = FT_MMAP_ID_VDEC_FRAME_BUF_LEN;
    initParam.SysConfig.u32DrvProcBufAddr = initParam.SysConfig.u32BitstreamBufAddr;
    initParam.SysConfig.u32DrvProcBufSize = 0xa000;
    initParam.SysConfig.eFWSourceType = E_VDEC_EX_FW_SOURCE_NONE;
    initParam.SysConfig.eDbgMsgLevel = E_VDEC_EX_DBG_LEVEL_NONE;

    UBOOT_INFO("VDEC FW add = 0x%lx   Size = 0x%lx\n", initParam.SysConfig.u32CodeBufAddr, initParam.SysConfig.u32CodeBufSize);
    UBOOT_INFO("VDEC FB add = 0x%lx   Size = 0x%lx\n", initParam.SysConfig.u32FrameBufAddr, initParam.SysConfig.u32FrameBufSize);
    UBOOT_INFO("VDEC BS add = 0x%lx   Size = 0x%lx\n", initParam.SysConfig.u32BitstreamBufAddr, initParam.SysConfig.u32BitstreamBufSize);
    UBOOT_INFO("VDEC Proc add = 0x%lx   Size = 0x%lx\n", initParam.SysConfig.u32DrvProcBufAddr, initParam.SysConfig.u32DrvProcBufSize);

    MApi_VDEC_EX_EnableTurboMode(TRUE);

    //Init/Re-Init VDEC
    eResult = MApi_VDEC_EX_Init(&_stVdecStreamId, &initParam);
    if(eResult != E_VDEC_EX_OK)
    {
        UBOOT_ERROR("Error[%d]: failed to MApi_VDEC_EX_Init()!\n", eResult);
        return -1;
    }

    MApi_VDEC_EX_SetControl(&_stVdecStreamId, E_VDEC_EX_USER_CMD_SHOW_FIRST_FRAME_DIRECT, 1);
    MApi_VDEC_EX_SetControl(&_stVdecStreamId, E_VDEC_EX_USER_CMD_SHOW_DECODE_ORDER, 1);
    MApi_VDEC_EX_SetControl(&_stVdecStreamId, E_VDEC_EX_USER_CMD_AVC_MIN_FRM_GAP, 0xFFFFFFFF);
    MApi_VDEC_EX_SetControl(&_stVdecStreamId, E_VDEC_EX_USER_CMD_SHOW_DECODE_ORDER, 1);

    eResult = MApi_VDEC_EX_StepDecode(&_stVdecStreamId);
    if(eResult != E_VDEC_EX_OK)
    {
        UBOOT_ERROR("Error[%d]: failed to MApi_VDEC_EX_StepDecode()!\n", eResult);
        return -1;
    }

    MsOS_DelayTask(50); // wait play ready(must be, otherwise push dec Q too fast  will be failed)

    pu8Virt = (MS_U8*)MS_PA2KSEG1((u32BitstreamBufAddr+0xa000));
    memcpy(pu8Virt, _u8PatternData, sizeof(_u8PatternData));
    MsOS_FlushMemory();
    MsOS_ReadMemory();

    memset(&stCmd, 0, sizeof(VDEC_EX_DecCmd));
    stCmd.u32StAddr = 0xa000;    // offset from bitstream buffer
    stCmd.u32Size = sizeof(_u8PatternData);
    stCmd.u32ID_H = 0;
    stCmd.u32ID_L = stCmd.u32Timestamp = 0xFFFFFFFF;

    eResult = MApi_VDEC_EX_PushDecQ(&_stVdecStreamId, &stCmd);
    if(eResult != E_VDEC_EX_OK)
    {
        UBOOT_ERROR("Error[%d]: failed to MApi_VDEC_EX_PushDecQ()!\n", eResult);
        return -1;
    }

    eResult = MApi_VDEC_EX_FireDecCmd(&_stVdecStreamId);
    if(eResult != E_VDEC_EX_OK)
    {
        UBOOT_ERROR("Error[%d]: failed to MApi_VDEC_EX_FireDecCmd()!\n", eResult);
        return -1;
    }

    // wait I-frame decode done
    while(MApi_VDEC_EX_IsStepDecodeDone(&_stVdecStreamId) != E_VDEC_EX_OK);

    // Trigger next frame(P frame) decoding
    eResult = MApi_VDEC_EX_StepDecode(&_stVdecStreamId);
    if(eResult != E_VDEC_EX_OK)
    {
        UBOOT_ERROR("Error[%d]: failed to MApi_VDEC_EX_StepDecode()!\n", eResult);
        return -1;
    }

    // wait P-frame decode done
    while(MApi_VDEC_EX_IsStepDecodeDone(&_stVdecStreamId) != E_VDEC_EX_OK);

    if(0)
    {
        _MsDrv_VideoSetXcMvopCfg(MVOP_INPUT_DRAM);
    }
    else
    {
        _MsDrv_VideoDoInfoChg(&_stVdecStreamId);
    }
    // check_sum
    if(_MsDrv_VideoDoFrameCheckSum(&_stVdecStreamId) != 0)
    {
        // check sum failed
        return -1;
    }

    printf("%s() done\n", __FUNCTION__);

    return 0;
}
#endif  // #if ENABLE_MBOOT_FT_VIDEO

