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
#include <MsCommon.h>
#include <MsTypes.h>
#include <MsDebug.h>
#include <MsMmap.h>
#include <panel/MsDrvPanel.h>
#include <apiHDMITx.h>
#include <MsEnvironment.h>
#include <malloc.h>
#include <CusConfig.h>

#define DISPOUT_3D_Max (9)
#define DISPOUT_AUDIO_TYPE_DDS_MAX (15)
#define EDID_VIC_4K2K_NUM (4)
#define EDID_VIC_4K2K420_NUM (32)
#define EDID_VIC_NUM     (107)
#define  HDMITX_EDID_NON_STATUS               ( 0 )
#define  HDMITX_EDID_VALID_BIT                (1<<0)
#define  HDMITX_EDID_4K2K_VALID_BIT           (1<<1)
#define  HDMITX_EDID_4K2K420_VALID_BIT        (1<<2)

#define PARSING_COLOR(x)    (                   \
    x == HDMITX_VIDEO_COLOR_RGB444 ? "RGB444" : \
    x == HDMITX_VIDEO_COLOR_YUV422 ? "YUV422" : \
    x == HDMITX_VIDEO_COLOR_YUV444 ? "YUV444" : \
    x == HDMITX_VIDEO_COLOR_YUV420 ? "YUV420" : \
                                     "unknown")

#define PARSING_CD(x)       (                \
    x == HDMITX_VIDEO_CD_NoID   ? "NoID"   : \
    x == HDMITX_VIDEO_CD_24Bits ? "8bits"  : \
    x == HDMITX_VIDEO_CD_30Bits ? "10bits" : \
    x == HDMITX_VIDEO_CD_36Bits ? "12bits" : \
    x == HDMITX_VIDEO_CD_48Bits ? "16bits" : \
                                  "unknown")

#define PARSING_OUTPUTTIMING(x) (                           \
    x == HDMITX_RES_640x480p        ? "640x480p" :          \
    x == HDMITX_RES_720x480i        ? "720x480i" :          \
    x == HDMITX_RES_720x576i        ? "720x576i" :          \
    x == HDMITX_RES_720x480p        ? "720x480p" :          \
    x == HDMITX_RES_720x576p        ? "720x576p" :          \
    x == HDMITX_RES_1280x720p_50Hz  ? "1280x720p_50Hz" :    \
    x == HDMITX_RES_1280x720p_60Hz  ? "1280x720p_60Hz" :    \
    x == HDMITX_RES_1920x1080i_50Hz ? "1920x1080i_50Hz" :   \
    x == HDMITX_RES_1920x1080i_60Hz ? "1920x1080i_60Hz" :   \
    x == HDMITX_RES_1920x1080p_24Hz ? "1920x1080p_24Hz" :   \
    x == HDMITX_RES_1920x1080p_25Hz ? "1920x1080p_25Hz" :   \
    x == HDMITX_RES_1920x1080p_30Hz ? "1920x1080p_30Hz" :   \
    x == HDMITX_RES_1920x1080p_50Hz ? "1920x1080p_50Hz" :   \
    x == HDMITX_RES_1920x1080p_60Hz ? "1920x1080p_60Hz" :   \
    x == HDMITX_RES_1920x2205p_24Hz ? "1920x2205p_24Hz" :   \
    x == HDMITX_RES_1280X1470p_50Hz ? "1280X1470p_50Hz" :   \
    x == HDMITX_RES_1280X1470p_60Hz ? "1280X1470p_60Hz" :   \
    x == HDMITX_RES_3840x2160p_24Hz ? "3840x2160p_24Hz" :   \
    x == HDMITX_RES_3840x2160p_25Hz ? "3840x2160p_25Hz" :   \
    x == HDMITX_RES_3840x2160p_30Hz ? "3840x2160p_30Hz" :   \
    x == HDMITX_RES_3840x2160p_50Hz ? "3840x2160p_50Hz" :   \
    x == HDMITX_RES_3840x2160p_60Hz ? "3840x2160p_60Hz" :   \
    x == HDMITX_RES_4096x2160p_24Hz ? "4096x2160p_24Hz" :   \
    x == HDMITX_RES_4096x2160p_25Hz ? "4096x2160p_25Hz" :   \
    x == HDMITX_RES_4096x2160p_30Hz ? "4096x2160p_30Hz" :   \
    x == HDMITX_RES_4096x2160p_50Hz ? "4096x2160p_50Hz" :   \
    x == HDMITX_RES_4096x2160p_60Hz ? "4096x2160p_60Hz" :   \
                                                  "unknown" )

#define PARSING_OUTPUTMODE(x) (            \
    x == HDMITX_DVI       ? "DVI"        : \
    x == HDMITX_DVI_HDCP  ? "DVI_HDCP"   : \
    x == HDMITX_HDMI      ? "HDMI"       : \
    x == HDMITX_HDMI_HDCP ? "HDMI_HDCP"  : \
                            "unknown" )

typedef enum
{
    HDMITX_NO_CHANGE_TIMING = 0,
    HDMITX_BEST_TIMING,
    HDMITX_MAX_TIMING,
} MsHDMITX_AUTO_TIMING_TYPE;

typedef enum
{
    E_HDMITX_VIDEO_SI_NoData       = 0,
    E_HDMITX_VIDEO_SI_Overscanned  = 1,
    E_HDMITX_VIDEO_SI_Underscanned = 2,
    E_HDMITX_VIDEO_SI_Reserved     = 3,
} MsHDMITX_VIDEO_SCAN_INFO;

typedef enum
{
    E_HDMITX_VIDEO_AFD_SameAsPictureAR    = 8, // 1000
    E_HDMITX_VIDEO_AFD_4_3_Center         = 9, // 1001
    E_HDMITX_VIDEO_AFD_16_9_Center        = 10, // 1010
    E_HDMITX_VIDEO_AFD_14_9_Center        = 11, // 1011
    E_HDMITx_VIDEO_AFD_Others = 15, // 0000~ 0111, 1100 ~ 1111
} MsHDMITX_VIDEO_AFD_RATIO;

typedef enum
{
    E_HDMITX_HDCP_DISABLE   = 0, // HDCP disable
    E_HDMITX_HDCP_FAIL      = 1, // HDCP fail
    E_HDMITX_HDCP_PASS      = 2, // HDCP pass
} MsHDMITX_HDCP_STATUS;

typedef struct
{
    MS_BOOL bSupportedTiming[HDMITX_RES_MAX];//get suported timing from EDID.
    MS_BOOL bSupportedAudioType[DISPOUT_AUDIO_TYPE_DDS_MAX];
    MS_BOOL bSupported3DType[DISPOUT_3D_Max];
    MS_BOOL bIsConnected;
    MS_BOOL bBestSupportedTiming[HDMITX_RES_MAX];//get the best suported timing from EDID.
    MsHDMITX_HDCP_STATUS eHdcpStatus;
} MsHDMITX_DEVICEINFO;

static char* DacPropPanelIndexTbl[] = {
    "",
    "",
    "",
    "HDMITX_BI_480_60P", //3
    "HDMITX_BI_576_50P", //4
    "HDMITX_BI_720_50P", //5
    "HDMITX_BI_720_60P", //6
    "",
    "",
    "",
    "",
    "",
    "HDMITX_BI_1080_50P", //12
    "HDMITX_BI_1080_60P", //13
    "",
    "",
    "",
    "HDMITX_BI_4K2K_24P", //17
    "HDMITX_BI_4K2K_25P", //18
    "HDMITX_BI_4K2K_30P", //19
    "HDMITX_BI_4K2K_50P", //20
    "HDMITX_BI_4K2K_60P", //21
    "",
    "",
    "",
    "",
    "",
};

static char* BootArgsResolutionIndexTbl[] = {
    "",
    "",
    "",
    "HDMITX_RES_720x480p",
    "HDMITX_RES_720x576p",
    "HDMITX_RES_1280x720p_50Hz",
    "HDMITX_RES_1280x720p_60Hz",
    "",
    "",
    "",
    "",
    "",
    "HDMITX_RES_1920x1080p_50Hz",
    "HDMITX_RES_1920x1080p_60Hz",
    "",
    "",
    "",
    "HDMITX_RES_4K2Kp_24Hz", //17
    "HDMITX_RES_4K2Kp_25Hz", //18
    "HDMITX_RES_4K2Kp_30Hz", //19
    "HDMITX_RES_4K2Kp_50Hz", //20
    "HDMITX_RES_4K2Kp_60Hz", //21
    "",
    "",
    "",
    "",
    "",
};

static MsHDMITX_AUTO_TIMING_TYPE GetAutoTimingType(void)
{
    char * pstr = NULL;
    MS_U32 value = 0;
    MsHDMITX_AUTO_TIMING_TYPE ret = HDMITX_NO_CHANGE_TIMING;

    UBOOT_TRACE("IN\n");
    pstr = getenv("auto_timing");
    UBOOT_DEBUG("auto_timing = %s\n", pstr);
    if (pstr == NULL)
    {
        UBOOT_TRACE("OK\n");
        return ret;
    }

    value = (MS_U32)simple_strtoul(pstr, NULL, 0);
    if ((value & 0xFF) != 0)
    {
        if ((value & 0xFF00) == 0)
        {
            ret = HDMITX_MAX_TIMING;
        }
        else
        {
            ret = HDMITX_BEST_TIMING;
        }
    }
    UBOOT_TRACE("OK\n");
    return ret;
}

static HDMITX_VIDEO_TIMING getHDMITxVideoTiming()
{
    HDMITX_VIDEO_TIMING mode =HDMITX_RES_640x480p;
    int u8ResolutionEnv = -1;
    char * p_str = NULL;

    p_str = getenv ("resolution");
    if(NULL != p_str)
    {
        u8ResolutionEnv = (int)simple_strtol(p_str, NULL, 10);
    }
     else
    {
        //set env normal resolution. HDMITX_RES_1920x1080p_60Hz  = 13,
        setenv("resolution", "13");
        saveenv();
    }

    if((HDMITX_RES_MAX <= u8ResolutionEnv) || (0 > u8ResolutionEnv))
    {
        printf("error, resolution env is out of [1--%d]!\n",HDMITX_RES_MAX);
        return FALSE;
    }

    return u8ResolutionEnv;
}

static MS_BOOL IsValidOutput(int index, MS_U8* pData, MS_U8* p4kData,  MS_U8* p4k420Data, MS_U32 status, MS_BOOL *bSupport, MS_BOOL *bBestSupport)
{
    *bSupport = FALSE;
    *bBestSupport = FALSE;
    MS_U8 u8Target_16x9 = 0, u8Target_4x3 = 0;
    MS_BOOL bRet = FALSE;
    int idx = 0;

    //VSD Case
    if(status & HDMITX_EDID_VALID_BIT)
    {
        //search video tag of 4k2k edid
        switch(index)
        {
            case HDMITX_RES_720x480i:
                u8Target_16x9 = HDMITX_VIC_720x480i_60_16_9;
                u8Target_4x3 = HDMITX_VIC_720x480i_60_4_3;
                break;
            case HDMITX_RES_720x576i:
                u8Target_16x9 = HDMITX_VIC_720x576i_50_16_9;
                u8Target_4x3 = HDMITX_VIC_720x576i_50_4_3;
                break;
            case HDMITX_RES_720x480p:
                u8Target_16x9 = HDMITX_VIC_720x480p_60_16_9;
                u8Target_4x3 = HDMITX_VIC_720x480p_60_4_3;
                break;
            case HDMITX_RES_720x576p:
                u8Target_16x9 = HDMITX_VIC_720x576p_50_16_9;
                u8Target_4x3 = HDMITX_VIC_720x576p_50_4_3;
                break;
            case HDMITX_RES_1280x720p_50Hz:
                u8Target_16x9 = HDMITX_VIC_1280x720p_50_16_9;
                break;
            case HDMITX_RES_1280x720p_60Hz:
                u8Target_16x9 = HDMITX_VIC_1280x720p_60_16_9;
                break;
            case HDMITX_RES_1920x1080i_50Hz:
                u8Target_16x9 = HDMITX_VIC_1920x1080i_50_16_9;
                break;
            case HDMITX_RES_1920x1080i_60Hz:
                u8Target_16x9 = HDMITX_VIC_1920x1080i_60_16_9;
                break;
            case HDMITX_RES_1920x1080p_24Hz:
                u8Target_16x9 = HDMITX_VIC_1920x1080p_24_16_9;
                break;
            case HDMITX_RES_1920x1080p_25Hz:
                u8Target_16x9 = HDMITX_VIC_1920x1080p_25_16_9;
                break;
            case HDMITX_RES_1920x1080p_30Hz:
                u8Target_16x9 = HDMITX_VIC_1920x1080p_30_16_9;
                break;
            case HDMITX_RES_1920x1080p_50Hz:
                u8Target_16x9 = HDMITX_VIC_1920x1080p_50_16_9;
                break;
            case HDMITX_RES_1920x1080p_60Hz:
                u8Target_16x9 = HDMITX_VIC_1920x1080p_60_16_9;
                break;
            case HDMITX_RES_3840x2160p_24Hz:
                u8Target_16x9 = HDMITX_VIC_3840x2160p_24_16_9;
                u8Target_4x3 =  HDMITX_VIC_3840x2160p_24_64_27;
                break;
            case HDMITX_RES_3840x2160p_25Hz:
                u8Target_16x9 = HDMITX_VIC_3840x2160p_25_16_9;
                u8Target_4x3 =  HDMITX_VIC_3840x2160p_25_64_27;
                break;
            case HDMITX_RES_3840x2160p_30Hz:
                u8Target_16x9 = HDMITX_VIC_3840x2160p_30_16_9;
                u8Target_4x3 =  HDMITX_VIC_3840x2160p_30_64_27;
                break;
            case HDMITX_RES_3840x2160p_50Hz:
                u8Target_16x9 = HDMITX_VIC_3840x2160p_50_16_9;
                u8Target_4x3 =  HDMITX_VIC_3840x2160p_50_64_27;
                break;
            case HDMITX_RES_3840x2160p_60Hz:
                u8Target_16x9 = HDMITX_VIC_3840x2160p_60_16_9;
                u8Target_4x3 =  HDMITX_VIC_3840x2160p_60_64_27;
                break;
            case HDMITX_RES_4096x2160p_24Hz:
                u8Target_16x9 = HDMITX_VIC_4096x2160p_24_256_135;
                u8Target_4x3 =  HDMITX_VIC_4096x2160p_24_256_135;
                break;
            case HDMITX_RES_4096x2160p_25Hz:
                u8Target_16x9 = HDMITX_VIC_4096x2160p_25_256_135;
                u8Target_4x3 =  HDMITX_VIC_4096x2160p_25_256_135;
                break;
            case HDMITX_RES_4096x2160p_30Hz:
                u8Target_16x9 = HDMITX_VIC_4096x2160p_30_256_135;
                u8Target_4x3 =  HDMITX_VIC_4096x2160p_30_256_135;
                break;
            case HDMITX_RES_4096x2160p_50Hz:
                u8Target_16x9 = HDMITX_VIC_4096x2160p_50_256_135;
                u8Target_4x3 =  HDMITX_VIC_4096x2160p_50_256_135;
                break;
            case HDMITX_RES_4096x2160p_60Hz:
                u8Target_16x9 = HDMITX_VIC_4096x2160p_60_256_135;
                u8Target_4x3 =  HDMITX_VIC_4096x2160p_60_256_135;
            default:
                break;
        }

        for(idx = 0; idx < EDID_VIC_NUM; idx++)
        {
            if(pData[idx] == 0)
            {
                break;
            }
            else if(((pData[idx]&0x7F) == u8Target_16x9) || ((pData[idx]&0x7F) == u8Target_4x3))
            {
                *bSupport = TRUE;
                //is the native timing flag true?
                if(pData[idx] & 0x80)
                {
                    *bBestSupport = TRUE;
                }
            }
        }
        bRet = TRUE;
    }

    //4K2K VIC VSDB Case
    u8Target_4x3 = 0;
    if(status & HDMITX_EDID_4K2K_VALID_BIT)
    {
        //search video tag of 4k2k edid
        switch(index)
        {
            case HDMITX_RES_3840x2160p_30Hz:
                u8Target_4x3 = HDMITX_VIDEO_4k2k_30Hz;
                break;
            case HDMITX_RES_3840x2160p_25Hz:
                u8Target_4x3 = HDMITX_VIDEO_4k2k_25Hz;
                break;
            case HDMITX_RES_3840x2160p_24Hz:
                u8Target_4x3 = HDMITX_VIDEO_4k2k_24Hz;
                break;
            default:
                break;
        }

        for(idx = 0; idx < EDID_VIC_4K2K_NUM; idx++)
        {
            if(p4kData[idx] == 0)
            {
                break;
            }
            else if(p4kData[idx] == u8Target_4x3)
            {
                *bSupport = TRUE;
            }
        }
        bRet = TRUE;
    }

    //YPbPr420 Case
    u8Target_16x9 = 0;
    u8Target_4x3 = 0;
    if(status & HDMITX_EDID_4K2K420_VALID_BIT)
    {
        //search video tag of edid
        switch(index)
        {
            case HDMITX_RES_3840x2160p_30Hz:
                u8Target_16x9 = HDMITX_VIC_3840x2160p_30_16_9;
                u8Target_4x3 =  HDMITX_VIC_3840x2160p_30_64_27;
                break;
            case HDMITX_RES_3840x2160p_25Hz:
                u8Target_16x9 = HDMITX_VIC_3840x2160p_25_16_9;
                u8Target_4x3 =  HDMITX_VIC_3840x2160p_25_64_27;
                break;
            case HDMITX_RES_3840x2160p_24Hz:
                u8Target_16x9 = HDMITX_VIC_3840x2160p_24_16_9;
                u8Target_4x3 =  HDMITX_VIC_3840x2160p_24_64_27;
                break;
            case HDMITX_RES_3840x2160p_50Hz:
                u8Target_16x9 = HDMITX_VIC_3840x2160p_50_16_9;
                u8Target_4x3 =  HDMITX_VIC_3840x2160p_50_64_27;
                break;
            case HDMITX_RES_3840x2160p_60Hz:
                u8Target_16x9 = HDMITX_VIC_3840x2160p_60_16_9;
                u8Target_4x3 =  HDMITX_VIC_3840x2160p_60_64_27;
                break;
            case HDMITX_RES_4096x2160p_24Hz:
                u8Target_16x9 = HDMITX_VIC_4096x2160p_24_256_135;
                u8Target_4x3 =  HDMITX_VIC_4096x2160p_24_256_135;
                break;
            case HDMITX_RES_4096x2160p_25Hz:
                u8Target_16x9 = HDMITX_VIC_4096x2160p_25_256_135;
                u8Target_4x3 =  HDMITX_VIC_4096x2160p_25_256_135;
                break;
            case HDMITX_RES_4096x2160p_30Hz:
                u8Target_16x9 = HDMITX_VIC_4096x2160p_30_256_135;
                u8Target_4x3 =  HDMITX_VIC_4096x2160p_30_256_135;
                break;
            case HDMITX_RES_4096x2160p_50Hz:
                u8Target_16x9 = HDMITX_VIC_4096x2160p_50_256_135;
                u8Target_4x3 =  HDMITX_VIC_4096x2160p_50_256_135;
                break;
            case HDMITX_RES_4096x2160p_60Hz:
                u8Target_16x9 = HDMITX_VIC_4096x2160p_60_256_135;
                u8Target_4x3 =  HDMITX_VIC_4096x2160p_60_256_135;
                break;
            default:
                break;
        }

        for(idx = 0; idx < EDID_VIC_4K2K420_NUM; idx++)
        {
            if(p4k420Data[idx] == 0)
            {
                break;
            }
            else if(p4k420Data[idx] == u8Target_16x9 || p4k420Data[idx] == u8Target_4x3)
            {
                *bSupport = TRUE;
            }
        }
        bRet = TRUE;
    }

    return bRet;
}

static MS_BOOL GetDeviceInfo(MsHDMITX_DEVICEINFO *pstDeviceInfo)
{

    MS_BOOL bSupport = FALSE;
    MS_BOOL bBestSupport = FALSE;
    MS_U8 i=0;
    MS_U8 u8HDMITx[EDID_VIC_NUM];
    MS_U8 u8HDMITx4K2K[EDID_VIC_4K2K_NUM];
    MS_U8 u8HDMITx4K2K420[EDID_VIC_4K2K420_NUM];
    MS_U32 edidStatus = HDMITX_EDID_NON_STATUS;
    UBOOT_TRACE("IN\n");

    if(pstDeviceInfo ==  NULL)
    {
        UBOOT_ERROR("ERROR: *pstDeviceInfo is NULL!\n");
        return FALSE;
    }

    //step 1. check hdmi whether connected
    pstDeviceInfo->bIsConnected = MApi_HDMITx_GetRxStatus();
    if(!pstDeviceInfo->bIsConnected)
    {
        UBOOT_DEBUG("Hdmi is not connected!\n");
        return FALSE;
    }

    //step 2. get edid timing and 4k2k edid timing list
    //VSD Case
    memset(u8HDMITx, 0, EDID_VIC_NUM * sizeof(MS_U8));
    if(TRUE == MApi_HDMITx_GetRxVideoFormatFromEDID(u8HDMITx, EDID_VIC_NUM))
    {
        edidStatus |= HDMITX_EDID_VALID_BIT;
    }

    //VSDB Case
    memset(u8HDMITx4K2K, 0, EDID_VIC_4K2K_NUM * sizeof(MS_U8));
    if(TRUE == MApi_HDMITx_GetVICListFromEDID(u8HDMITx4K2K, EDID_VIC_4K2K_NUM))
    {
        edidStatus |= HDMITX_EDID_4K2K_VALID_BIT;
    }

   //YPbPr420 Case
    MS_U32 u32RealLen = 0;
    memset(u8HDMITx4K2K420, 0, EDID_VIC_4K2K420_NUM * sizeof(MS_U8));
    if(TRUE == MApi_HDMITx_GetEdidDataBlocks(E_CEA_TAG_CODE_EXT_TAG, E_CEA_EXT_TAG_420VDB, u8HDMITx4K2K420, EDID_VIC_4K2K420_NUM, &u32RealLen))
    {
        edidStatus |= HDMITX_EDID_4K2K420_VALID_BIT;
    }

    if(HDMITX_EDID_NON_STATUS == edidStatus)
    {
        UBOOT_DEBUG("Get edid or 4K edid or 4K420 fail\n");
        return FALSE;
    }

    //step 3. get support Timing and bestTiming info for device
    memset((void *)(pstDeviceInfo->bSupportedTiming), 0, HDMITX_RES_MAX);
    memset((void *)(pstDeviceInfo->bBestSupportedTiming), 0, HDMITX_RES_MAX);
    for(i=0; i<HDMITX_RES_MAX; i++)
    {
        if(IsValidOutput(i, u8HDMITx, u8HDMITx4K2K, u8HDMITx4K2K420, edidStatus, &bSupport, &bBestSupport) == TRUE)
        {
            pstDeviceInfo->bSupportedTiming[i] = bSupport;
            pstDeviceInfo->bBestSupportedTiming[i] = bBestSupport;
        }
    }

    //step 4. reserved
    //get support audio format info for device func
    //get hdcp status info for device func
    //get support 3D format info for device func
    UBOOT_TRACE("OK\n");
    return TRUE;
}

static MS_U8 GetSupportTimingFrList(MS_U8 *timing_list, MS_U8 timing_cnt, MS_BOOL check_ini_exist)
{
    UBOOT_DEBUG("IN\n");
    char cmd_buf[CMD_BUF] = {0};
    int i = 0;
    MS_U8 u8SupportTimingIdx = HDMITX_RES_MAX;
    MS_U32 filesize = 0;

    if(timing_cnt == 0)
    {
        return HDMITX_RES_MAX;
    }

    if (check_ini_exist == TRUE)
    {
        if(vfs_mount(CONFIG)!= 0)
        {
            UBOOT_ERROR("vfs_mount fail\n");
            return HDMITX_RES_MAX;
        }

        for (i = timing_cnt - 1; i >= 0; i--)
        {
            u8SupportTimingIdx = timing_list[i];
            snprintf(cmd_buf, sizeof(cmd_buf),"/config/panel/%s.ini", DacPropPanelIndexTbl[u8SupportTimingIdx]);
            filesize = vfs_getsize(cmd_buf);
            if( (filesize > 0x10000) || (filesize == 0) )
            {
                UBOOT_DEBUG("%s does not exist, move to next resolution\n", cmd_buf);
            }
            else
            {
                return u8SupportTimingIdx;
            }
        }
    }
    else
    {
        return timing_list[timing_cnt - 1];
    }

    UBOOT_DEBUG("OK\n");
    return HDMITX_RES_MAX;
}

static MS_U8 CheckSupportTiming(MS_U8 current_timing)
{
    MsHDMITX_DEVICEINFO stDeviceInfo;
    MS_U8 index = 0;
    MS_U8 u8BestTimingCnt = 0;
    MS_U8 u8SupportTimingCnt = 0;
    MS_U8 u8BestTimingList[HDMITX_RES_MAX]={0};
    MS_U8 u8SupportTimingList[HDMITX_RES_MAX]={0};
    MS_U8 prop_timing = current_timing;
    UBOOT_TRACE("IN\n");

    UBOOT_DEBUG("current_timing = %d\n", current_timing);
    if(GetDeviceInfo((MsHDMITX_DEVICEINFO *)&stDeviceInfo) == TRUE)
    {
        // no-auto_timing and current timing is support
        if((GetAutoTimingType() == HDMITX_NO_CHANGE_TIMING) && (stDeviceInfo.bSupportedTiming[current_timing] == TRUE))
        {
            UBOOT_TRACE("OK\n");
            return current_timing;
        }

        //restore best timing and highest timing
        for(index=0; index<HDMITX_RES_MAX; index++)
        {
            if(stDeviceInfo.bBestSupportedTiming[index] == TRUE)
            {
                if(!strcmp(DacPropPanelIndexTbl[index], ""))
                {
                    continue;
                }
                UBOOT_DEBUG("--Support Native Timing is: %s\n", DacPropPanelIndexTbl[index]);
                u8BestTimingList[u8BestTimingCnt++] = index;
            }
            if(stDeviceInfo.bSupportedTiming[index] == TRUE)
            {
                if(!strcmp(DacPropPanelIndexTbl[index], ""))
                {
                    continue;
                }
                UBOOT_DEBUG("--Support Timing is: %s\n", DacPropPanelIndexTbl[index]);
                u8SupportTimingList[u8SupportTimingCnt++] = index;
            }
        }

        // Select the required timing
        if(GetAutoTimingType() == HDMITX_BEST_TIMING)        // use best timing
        {
            prop_timing = GetSupportTimingFrList(u8BestTimingList, u8BestTimingCnt, FALSE);
            if(prop_timing >= HDMITX_RES_MAX)
            {
                prop_timing = GetSupportTimingFrList(u8SupportTimingList, u8SupportTimingCnt, FALSE);
                if(prop_timing >= HDMITX_RES_MAX)
                {
                    UBOOT_DEBUG("Set the max_timing to default(%d)\n", HDMITX_RES_1280x720p_50Hz);
                    prop_timing = HDMITX_RES_1280x720p_50Hz;
                }
            }
            UBOOT_INFO("\nThe best_timing is %d\n", prop_timing);
        }
        else if(GetAutoTimingType() == HDMITX_MAX_TIMING)    // use max timing
        {
            prop_timing = GetSupportTimingFrList(u8SupportTimingList, u8SupportTimingCnt, FALSE);
            if(prop_timing >= HDMITX_RES_MAX)
            {
                UBOOT_DEBUG("Set the max_timing to default(%d)\n", HDMITX_RES_1280x720p_50Hz);
                prop_timing = HDMITX_RES_1280x720p_50Hz;
            }
            UBOOT_INFO("\nThe max_timing is %d\n", prop_timing);
        }
        else    // no-auto_timing and current timing is NOT support
        {
            prop_timing = GetSupportTimingFrList(u8SupportTimingList, u8SupportTimingCnt, TRUE);
            if (prop_timing >=  HDMITX_RES_MAX)
            {
                // case : can NOT find the required timing
                prop_timing = HDMITX_RES_1280x720p_50Hz;
            }
            else
            {
                // case : can find the required timing
                if((prop_timing < HDMITX_RES_3840x2160p_24Hz) && (stDeviceInfo.bSupportedTiming[HDMITX_RES_1920x1080p_50Hz] == TRUE))
                {
                    prop_timing = HDMITX_RES_1920x1080p_50Hz;
                }
                else if((prop_timing < HDMITX_RES_3840x2160p_24Hz) && (stDeviceInfo.bSupportedTiming[HDMITX_RES_1280x720p_50Hz] == TRUE))
                {
                    prop_timing = HDMITX_RES_1280x720p_50Hz;
                }
            }
        }
    }
    else
    {
        if(getenv("auto_timing") != NULL) //auto timing use default timing
        {
            UBOOT_DEBUG("auto default timing is %d\n", HDMITX_RES_1280x720p_50Hz);
            prop_timing = HDMITX_RES_1280x720p_50Hz;
        }
        else if((current_timing >= HDMITX_RES_3840x2160p_24Hz) && MApi_HDMITx_GetRxStatus()) //no-auto 4k timing and edid is not ready change to 1080P50
        {
            UBOOT_DEBUG("4K timing is  not support, change to 1080P50\n");
            prop_timing = HDMITX_RES_1920x1080p_50Hz;
        }
    }

    UBOOT_TRACE("OK\n");
    return prop_timing;
}

static MS_BOOL IsReciverSupportYPbPr(void)
{
    #define BLOCK_INDEX1    1

    MS_U8 u8BlockData[128] = {0};

    MApi_HDMITx_GetEDIDData(u8BlockData, BLOCK_INDEX1);

    if ( ( u8BlockData[0x03]& 0x30) == 0x00)
    {
        UBOOT_TRACE("Rx Not Support YCbCr\n");
        return FALSE;
    }
    else
    {
        UBOOT_TRACE("Rx Support YUV444 or YUV422 \n");
        return TRUE;
    }
}

int MsDrvTvHdmiTxInit(void)
{
    MS_BOOL _bModeNTSC = FALSE;
    MS_BOOL bRet = FALSE;
    HDMITX_VIDEO_TIMING mode =HDMITX_RES_640x480p;

    UBOOT_TRACE("IN\n");


    MApi_HDMITx_SetDbgLevel(0);
    MApi_HDMITx_SetHPDGpioPin(HDMITX_HPD_PM_GPIO);
    MApi_HDMITx_Init();
    MApi_HDMITx_TurnOnOff(TRUE);
    MApi_HDMITx_SetVideoOnOff(TRUE);

    //For tv chip Incolorformat should be RGB444
    HDMITX_VIDEO_COLOR_FORMAT eInColorFmt  = HDMITX_VIDEO_COLOR_RGB444;
    HDMITX_OUTPUT_MODE    eOutMode = HDMITX_HDMI;

    HDMITX_VIDEO_COLORDEPTH_VAL eEDIDColorDepth = HDMITX_VIDEO_CD_24Bits;
    HDMITX_VIDEO_COLOR_FORMAT eOutColorFmt = HDMITX_VIDEO_COLOR_RGB444;

    MS_U32 u32EdidCheckTime=0;
    int resolution_index = HDMITX_RES_1920x1080p_50Hz;
    int prop_index = HDMITX_RES_1920x1080p_50Hz;
    char *p_str = NULL;
    MS_U32 u32StartTime = 0;
    MS_BOOL bHDMISupport = FALSE;
    while((MApi_HDMITx_EdidChecking() != TRUE) && ((u32EdidCheckTime) < 6))
    {
        UBOOT_ERROR("CHeck EDID fail %d times!\n", ++u32EdidCheckTime);
    }

    u32StartTime = MsOS_GetSystemTime();
    while((MApi_HDMITx_EDID_HDMISupport(&bHDMISupport) != TRUE) && (MsOS_Timer_DiffTimeFromNow(u32StartTime) < 10))
    {
        UBOOT_ERROR("EDID NOT READY! \n");
    }

    p_str = getenv ("resolution");
    if(p_str != NULL)
    {
        resolution_index = (int)simple_strtol(p_str, NULL, 10);
    }

    prop_index = CheckSupportTiming(resolution_index);
    if(prop_index != resolution_index)
    {
        char cmd_buf[CMD_BUF] = "\0";
        resolution_index = prop_index;
        printf("select proper resolution=%d from EDID\n", resolution_index);
        printf("select proper panel_name: %s.ini\n", DacPropPanelIndexTbl[resolution_index]);
        snprintf(cmd_buf, sizeof(cmd_buf),"%d", resolution_index);
        setenv("resolution", cmd_buf);
        setenv("resolution_reset", cmd_buf);

        del_bootargs_cfg("panel_path", FALSE);
        MApi_SetEnv2BootArgs("resolution=", BootArgsResolutionIndexTbl[resolution_index]);
        snprintf(cmd_buf, sizeof(cmd_buf),"/config/panel/%s.ini", DacPropPanelIndexTbl[resolution_index]);
        setenv("panel_path", cmd_buf);
        setenv("panel_name", DacPropPanelIndexTbl[resolution_index]);

        setenv("db_table", "0");
        saveenv();
        run_command("reset", 0);
    }
    if(bHDMISupport)
    {
        if(HDMITX_RES_3840x2160p_60Hz == prop_index)
        {
            HDMITX_EDID_COLOR_FORMAT ColorFmt = HDMITX_EDID_Color_YCbCr_444;
            MApi_HDMITx_GetColorFormatFromEDID(HDMITX_RES_3840x2160p_60Hz, &ColorFmt);

            if(ColorFmt != HDMITX_EDID_Color_YCbCr_420)
            {
                // support 4k2k@60Hz YUV444
                eInColorFmt = HDMITX_VIDEO_COLOR_RGB444;
                eOutColorFmt = HDMITX_VIDEO_COLOR_YUV444;
            }
            else
            {
                // only support 4k2k@60Hz YUV420
                eInColorFmt = HDMITX_VIDEO_COLOR_RGB444;
                eOutColorFmt = HDMITX_VIDEO_COLOR_YUV420;
            }
        }
        else if(HDMITX_RES_3840x2160p_50Hz == prop_index)
        {
            HDMITX_EDID_COLOR_FORMAT ColorFmt = HDMITX_EDID_Color_YCbCr_444;
            MApi_HDMITx_GetColorFormatFromEDID(HDMITX_RES_3840x2160p_50Hz, &ColorFmt);

            if(ColorFmt != HDMITX_EDID_Color_YCbCr_420)
            {
                // support 4k2k@50Hz YUV444
                eInColorFmt = HDMITX_VIDEO_COLOR_RGB444;
                eOutColorFmt = HDMITX_VIDEO_COLOR_YUV444;
            }
            else
            {
                // only support 4k2k@50Hz YUV420
                eInColorFmt = HDMITX_VIDEO_COLOR_RGB444;
                eOutColorFmt = HDMITX_VIDEO_COLOR_YUV420;
            }
        }
        //check Timing except 4k60/50 whether RX supoort YUV colorformat,if support set YUV colorformat
        else if(IsReciverSupportYPbPr() == TRUE)
        {
            eInColorFmt = HDMITX_VIDEO_COLOR_RGB444;
            eOutColorFmt = HDMITX_VIDEO_COLOR_YUV444;
        }
        else//RX not supoort YUV colorformat,so set colorformat as RGB444
        {
            eInColorFmt = HDMITX_VIDEO_COLOR_RGB444;
            eOutColorFmt = HDMITX_VIDEO_COLOR_RGB444;
        }

        eOutMode = HDMITX_HDMI;
        printf("Rx Support HDMI mode! \n");
    }
    else //Rx not support hdmi,force hdmi mode
    {
        eOutMode = HDMITX_HDMI;
        eInColorFmt = HDMITX_VIDEO_COLOR_RGB444;
        eOutColorFmt = HDMITX_VIDEO_COLOR_RGB444;
        eEDIDColorDepth = HDMITX_VIDEO_CD_24Bits;

        if(prop_index == DACOUT_3840x2160P_50 || prop_index == DACOUT_3840x2160P_60)
        {
            eInColorFmt = HDMITX_VIDEO_COLOR_RGB444;
            eOutColorFmt = HDMITX_VIDEO_COLOR_YUV420;

            MApi_HDMITx_ForceHDMIOutputMode(TRUE, HDMITX_HDMI);
            MApi_HDMITx_ForceHDMIOutputColorFormat(TRUE, HDMITX_VIDEO_COLOR_YUV420);
            printf("force 420 8bit out when 4K50/60! \n");
        }
        printf("force to HDMI mode! \n");
    }

    printf("Final eInColorFmt:%s, eOutColorFmt:%s, eOutMode:%s, eEDIDColorDepth:%s, OutputTiming:%s \n",
                          PARSING_COLOR(eInColorFmt), PARSING_COLOR(eOutColorFmt),
                          PARSING_OUTPUTMODE(eOutMode), PARSING_CD(eEDIDColorDepth), PARSING_OUTPUTTIMING(prop_index));

    MApi_HDMITx_SetHDMITxMode_CD(eOutMode, eEDIDColorDepth);
    MApi_HDMITx_SetHDMITxMode(eOutMode);
    MApi_HDMITx_SetColorFormat(eInColorFmt, eOutColorFmt);
#if(ENABLE_POWER_MUSIC==1)
    MApi_HDMITx_SetAudioOnOff(TRUE);
    MApi_HDMITx_SetAudioConfiguration(HDMITX_AUDIO_48K, HDMITX_AUDIO_CH_2, HDMITX_AUDIO_PCM);
#endif

    //6. Post HDMI setting
    MApi_HDMITx_SetVideoOutputTiming(getHDMITxVideoTiming());
    //setHDMITxAnalogTuning(enPanelType);
    // MApi_HDMITx_Exhibit();
    if(getHDMITxVideoTiming() <= HDMITX_RES_720x576p)
    {
        //MApi_HDMITx_SetVideoOutputOverscan_AFD(ENABLE, E_HDMITX_VIDEO_SI_NoData, E_HDMITX_VIDEO_AFD_4_3_Center);
        MApi_HDMITx_SetVideoOutputOverscan_AFD_II(ENABLE, E_HDMITX_VIDEO_SI_NoData, E_HDMITX_VIDEO_AFD_4_3_Center, 0x1);
    }
    else
    {
        //MApi_HDMITx_SetVideoOutputOverscan_AFD(ENABLE, E_HDMITX_VIDEO_SI_NoData, E_HDMITX_VIDEO_AFD_16_9_Center);
        MApi_HDMITx_SetVideoOutputOverscan_AFD_II(ENABLE, E_HDMITX_VIDEO_SI_NoData, E_HDMITX_VIDEO_AFD_16_9_Center, 0x1);
    }
    //back ground active need disable output (video and audio)
    if(FALSE == pm_check_back_ground_active())
    {
        MApi_HDMITx_SetTMDSOnOff(TRUE);
        MsOS_DelayTask(20);
        MApi_HDMITx_SetAVMUTE(FALSE);
    }
    else
    {
        MApi_HDMITx_SetTMDSOnOff(FALSE);
    }
    UBOOT_TRACE("ON\n");
    return 0;
}
