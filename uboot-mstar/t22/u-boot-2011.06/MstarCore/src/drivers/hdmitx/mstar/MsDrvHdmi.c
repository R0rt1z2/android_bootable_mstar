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
#include <command.h>
#include <MsCommon.h>
#include <MsTypes.h>
#include <apiPNL.h>
#include <MsDebug.h>
#include <MsMmap.h>
#include <panel/MsDrvPanel.h>
#include <apiHDMITx.h>
#include <apiDAC.h>
#include <drvTVEncoder.h>
#include <MsApiPM.h>
#include <MsEnvironment.h>
#include <MsVfs.h>
#include <malloc.h>
#include <drvSYS.h>
#include <drvXC_IOPort.h>
#include <apiXC.h>
#include <CusConfig.h>
#include <mdrv_dac_tbl.h>
#include <mdrv_dac_tbl.c>
#include <mstarstr.h>
#include <bootlogo/iniparser.h>
#include <MsSystem.h>
#include <MsDrvHdmi.h>
#include <CusUpgrade.h>

#ifndef CONFIG_HDMI_OLDCHIP
#include <apiPNL_EX.h>
#include <apiXC_EX.h>
#include <apiXC_PCMonitor_EX.h>
#endif

#define VE_FRAMEBUFFER_OFFSET       (16)
#define VE_WRITE_ADDR_ADJUSTMENT    (-8)
#define VE_READ_ADDR_ADJUSTMENT     (0)

#define HDMITX_HDR_INFOFRAME_DYNAMIC_RANGE_PACKET_LENGHT (26)
#define HDMITX_HDR_INFOFRAME_DYNAMIC_RANGE_ARRAY_SIZE (3+HDMITX_HDR_INFOFRAME_DYNAMIC_RANGE_PACKET_LENGHT)

#if (CONFIG_DISPLAY_LOGO_SEAMLESS == 1)
#define HDMITX_HDR_INFOFRAME_DYNAMIC_RANGE (7)
#define HDMITX_HDR_INFOFRAME_DYNAMIC_RANGE_VERSION (1)
#define HDMITX_HDR_STATIC_METADATA_DATA_BLOCK_LENGTH (5)
#endif

extern int isBootToRecovery(void);

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

typedef struct
{
    HDMITX_OUTPUT_MODE eHdmitxMode;
    HDMITX_VIDEO_COLORDEPTH_VAL eHdmitxColorDepth;
    HDMITX_VIDEO_COLOR_FORMAT eHdmitxInColor;
    HDMITX_VIDEO_COLOR_FORMAT eHdmitxOutColor;
    HDMITX_VIDEO_TIMING eHdmitxVideoTiming;
    HDMITX_AUDIO_FREQUENCY eAudioFreq;
    HDMITX_AUDIO_CHANNEL_COUNT eAudioChCnt;
    HDMITX_AUDIO_CODING_TYPE eAudioCodeType;
    MS_BOOL bAudioOn;
    MS_U8 u8OsdSdr2HdrEnable;
    MS_U8 u8SeamlessEnable;
    MS_U8 u8CurHDRInfoFrame[HDMITX_HDR_INFOFRAME_DYNAMIC_RANGE_ARRAY_SIZE];
    MS_U8 u8ExtendedColorimetry;
    MS_U8 u8YccQuantizationRange;
}MsHDMITX_INFO;

typedef enum
{
    E_COLOR_DEPTH_8BITS = 0,
    E_COLOR_DEPTH_10BITS,
    E_COLOR_DEPTH_12BITS,
    E_COLOR_DEPTH_16BITS,
    E_COLOR_DEPTH_AUTO,
    E_COLOR_DEPTH_MAX,
} MsHDMITX_COCLOR_DEPTH;

typedef enum
{
    E_HDMITX_COLOR_YUV420_8BITS      = 0,
    E_HDMITX_COLOR_YUV420_10BITS     = 1,
    E_HDMITX_COLOR_YUV420_12BITS     = 2,
    E_HDMITX_COLOR_YUV420_16BITS     = 3,
    E_HDMITX_COLOR_YUV422_NoID       = 4,
    E_HDMITX_COLOR_RGB444_8BITS      = 5,
    E_HDMITX_COLOR_RGB444_10BITS     = 6,
    E_HDMITX_COLOR_RGB444_12BITS     = 7,
    E_HDMITX_COLOR_RGB444_16BITS     = 8,
    E_HDMITX_COLOR_YUV444_8BITS      = 9,
    E_HDMITX_COLOR_YUV444_10BITS     = 10,
    E_HDMITX_COLOR_YUV444_12BITS     = 11,
    E_HDMITX_COLOR_YUV444_16BITS     = 12,
    E_HDMITX_COLOR_FORMAT_DEPTH_AUTO = 0x80,
    E_HDMITX_COLOR_FORMAT_DEPTH_MAX,
} E_HDMITX_COLOR_FORMAT_DEPTH;


//auto-timing detect
#define DISPOUT_TIMING_MAX (33)
#define DISPOUT_3D_Max (9)
#define DISPOUT_AUDIO_TYPE_DDS_MAX (15)
#define EDID_VIC_4K2K_NUM (4)
#define EDID_VIC_4K2K420_NUM (32)
#define EDID_VIC_NUM     (107)
#define DACOUT720P50    (2)
#define DACOUT720P60    (3)
#define DACOUT1080P50   (4)
#define DACOUT1080P60   (5)
#define DACOUT1470P60   (16)
#define DACOUT3840P24   (17)
#define DACOUT3840P60   (21)
#define DACOUT3840P420_30   (27)

typedef enum
{
    E_HDMITX_HDCP_DISABLE   = 0, // HDCP disable
    E_HDMITX_HDCP_FAIL      = 1, // HDCP fail
    E_HDMITX_HDCP_PASS      = 2, // HDCP pass
} MsHDMITX_HDCP_STATUS;


#define  HDMITX_EDID_NON_STATUS               ( 0 )
#define  HDMITX_EDID_VALID_BIT                (1<<0)
#define  HDMITX_EDID_4K2K_VALID_BIT           (1<<1)
#define  HDMITX_EDID_4K2K420_VALID_BIT        (1<<2)
#define  HDMITX_AUDIO_UNMUTE                  ( 0 )
#define  HDMITX_AUDIO_MUTE                    ( 1 )

typedef enum
{
    HDMITX_NO_CHANGE_TIMING = 0,
    HDMITX_BEST_TIMING,
    HDMITX_MAX_TIMING,
} MsHDMITX_AUTO_TIMING_TYPE;


typedef struct
{
    MS_BOOL bSupportedTiming[DISPOUT_TIMING_MAX];//get suported timing from EDID.
    MS_BOOL bSupportedAudioType[DISPOUT_AUDIO_TYPE_DDS_MAX];
    MS_BOOL bSupported3DType[DISPOUT_3D_Max];
    MS_BOOL bIsConnected;
    MS_BOOL bEdidReady;
    MS_BOOL bBestSupportedTiming[DISPOUT_TIMING_MAX];//get the best suported timing from EDID.
    MsHDMITX_HDCP_STATUS eHdcpStatus;
} MsHDMITX_DEVICEINFO;

typedef struct
{
    MS_U8                          u8ResolutionEnv;
    PANEL_RESOLUTION_TYPE          enResolutionType;
    E_OUTPUT_VIDEO_TIMING_TYPE     enDACIndex;
    E_MW_Picture_Dispout_Timing_e  enMWIndex;
}HDMI_RESOLUTION_DAC_MAP;

#define RESOLUTION_COUNTER    DISPOUT_TIMING_MAX

#define HDMITX_INFO_SIZE    3


#define ENV_CVBS_STATUS       "cvbs_status"
#define ENV_HDMI_DEFAULT_RES  "resolution_default"

#if (CONFIG_DISPLAY_LOGO_SEAMLESS == 1)
static MS_U8 g_u8HDRPackage[HDMITX_HDR_INFOFRAME_DYNAMIC_RANGE_PACKET_LENGHT] = {
    #include <hdr_metedata.h>
};
#endif

static HDMITX_ANALOG_TUNING astHdmiAnalogCfg[HDMITX_INFO_SIZE] =
{
    // sd
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    // HD color depth 24bit, 30bit
    {0x01, 0x00, 0x00, 0x00, 0x00, 0x00},
    // DEEP_HD color depth 36bit
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x07},
};

/* display resolution
DACOUT_1080P_50 -> 4//resolution env via SN
DACOUT_1080P_60 -> 5
DACOUT_720P_50 -> 2
DACOUT_720P_60 -> 3
DACOUT_576P_60 -> 1
DACOUT_480P_60 -> 0
*/
static HDMI_RESOLUTION_DAC_MAP staMapTypeIndex[RESOLUTION_COUNTER] = {
    { 0, DACOUT_480P,           E_RES_720x480P_60Hz,       E_MW_PICTURE_DISPOUT_TIMING_480_60P},
    { 1, DACOUT_576P,           E_RES_720x576P_50Hz,       E_MW_PICTURE_DISPOUT_TIMING_576_50P},
    { 2, DACOUT_720P_50,        E_RES_1280x720P_50Hz,       E_MW_PICTURE_DISPOUT_TIMING_720_50P},
    { 3, DACOUT_720P_60,        E_RES_1280x720P_60Hz,       E_MW_PICTURE_DISPOUT_TIMING_720_60P},
    { 4, DACOUT_1080P_50,       E_RES_1920x1080P_50Hz,      E_MW_PICTURE_DISPOUT_TIMING_1080_50P},

    { 5, DACOUT_1080P_60,       E_RES_1920x1080P_60Hz,       E_MW_PICTURE_DISPOUT_TIMING_1080_60P},
    { 6, DACOUT_480I,           E_RES_720x480I_60Hz,        E_MW_PICTURE_DISPOUT_TIMING_480_60I},
    { 7, DACOUT_576I,           E_RES_720x576I_50Hz,        E_MW_PICTURE_DISPOUT_TIMING_576_50I},
    { 8, DACOUT_1080I_50,       E_RES_1920x1080I_50Hz,       E_MW_PICTURE_DISPOUT_TIMING_1080_50I},
    { 9, DACOUT_1080I_60,       E_RES_1920x1080I_60Hz,       E_MW_PICTURE_DISPOUT_TIMING_1080_60I},
#ifndef CONFIG_HDMI_OLDCHIP
    {10, DACOUT_1080P_30,       E_RES_1920x1080P_30Hz,       E_MW_PICTURE_DISPOUT_TIMING_1080_30P},
    {11, DACOUT_1080P_25,       E_RES_1920x1080P_25Hz,       E_MW_PICTURE_DISPOUT_TIMING_1080_25P},
    {12, DACOUT_1080P_24,       E_RES_1920x1080P_24Hz,       E_MW_PICTURE_DISPOUT_TIMING_1080_24P},
    {13, DACOUT_640x480_60,     E_RES_640x480P_60Hz,     E_MW_PICTURE_DISPOUT_TIMING_NOT_SUPPORT},
    {14, DACOUT_1920x2205P_24,  E_RES_1920x2205P_24Hz,  E_MW_PICTURE_DISPOUT_TIMING_1920x2205_24P},

    {15, DACOUT_1280x1470P_50,  E_RES_1280x1470P_50Hz,  E_MW_PICTURE_DISPOUT_TIMING_1470_50P},
    {16, DACOUT_1280x1470P_60,  E_RES_1280x1470P_60Hz,  E_MW_PICTURE_DISPOUT_TIMING_1470_60P},
    {17, DACOUT_3840x2160P_24,  E_RES_3840x2160P_24Hz,  E_MW_PICTURE_DISPOUT_TIMING_4K2K_24P},
    {18, DACOUT_3840x2160P_25,  E_RES_3840x2160P_25Hz,  E_MW_PICTURE_DISPOUT_TIMING_4K2K_25P},
    {19, DACOUT_3840x2160P_30,  E_RES_3840x2160P_30Hz,  E_MW_PICTURE_DISPOUT_TIMING_4K2K_30P},

    {20, DACOUT_3840x2160P_50,  E_RES_3840x2160P_50Hz,  E_MW_PICTURE_DISPOUT_TIMING_4K2K_50P},
    {21, DACOUT_3840x2160P_60,  E_RES_3840x2160P_60Hz,  E_MW_PICTURE_DISPOUT_TIMING_4K2K_60P},
    {22, DACOUT_4096x2160P_24,  E_RES_4096x2160P_24Hz,  E_MW_PICTURE_DISPOUT_TIMING_NOT_SUPPORT},
    {23, DACOUT_4096x2160P_25,  E_RES_4096x2160P_25Hz,  E_MW_PICTURE_DISPOUT_TIMING_NOT_SUPPORT},
    {24, DACOUT_4096x2160P_30,  E_RES_4096x2160P_30Hz,  E_MW_PICTURE_DISPOUT_TIMING_NOT_SUPPORT},
    {25, DACOUT_4096x2160P_50,  E_RES_4096x2160P_50Hz,  E_MW_PICTURE_DISPOUT_TIMING_NOT_SUPPORT},

    {26, DACOUT_4096x2160P_60,      E_RES_4096x2160P_60Hz,     E_MW_PICTURE_DISPOUT_TIMING_NOT_SUPPORT},
    {27, DACOUT_3840x2160P420_30,   E_RES_3840x2160P420_30Hz,  E_MW_PICTURE_DISPOUT_TIMING_NOT_SUPPORT},
    {28, DACOUT_3840x2160P420_50,   E_RES_3840x2160P420_50Hz,  E_MW_PICTURE_DISPOUT_TIMING_NOT_SUPPORT},
    {29, DACOUT_3840x2160P420_60,   E_RES_3840x2160P420_60Hz,  E_MW_PICTURE_DISPOUT_TIMING_NOT_SUPPORT},
    {30, DACOUT_4096x2160P420_30,   E_RES_4096x2160P420_30Hz,  E_MW_PICTURE_DISPOUT_TIMING_NOT_SUPPORT},

    {31, DACOUT_4096x2160P420_50,   E_RES_4096x2160P420_50Hz,  E_MW_PICTURE_DISPOUT_TIMING_NOT_SUPPORT},
    {32, DACOUT_4096x2160P420_60,   E_RES_4096x2160P420_60Hz,  E_MW_PICTURE_DISPOUT_TIMING_NOT_SUPPORT},

#endif
};

static char* DacPropPanelIndexTbl[] = {
    "DACOUT_480P_60",
    "DACOUT_576P_50",
    "DACOUT_720P_50",
    "DACOUT_720P_60",
    "DACOUT_1080P_50",
    "DACOUT_1080P_60",
    "DACOUT_480I_60",
    "DACOUT_576I_50",
    "DACOUT_1080I_50",
    "DACOUT_1080I_60",
    "DACOUT_1080P_30",
    "DACOUT_1080P_25",
    "DACOUT_1080P_24",
    "DACOUT_640X480_60",
    "DACOUT_1920X2205P_24",
    "DACOUT_1280X1470P_50",
    "DACOUT_1280X1470P_60",
    "DACOUT_4K2KP_24",
    "DACOUT_4K2KP_25",
    "DACOUT_4K2KP_30",
    "DACOUT_4K2KP_50",
    "DACOUT_4K2KP_60",
    "DACOUT_4096X2160P_24",
    "DACOUT_4096X2160P_25",
    "DACOUT_4096X2160P_30",
    "DACOUT_4096X2160P_50",
    "DACOUT_4096X2160P_60",

    "DACOUT_3840x2160P420_30",
    "DACOUT_3840x2160P420_50",
    "DACOUT_3840x2160P420_60",
    "DACOUT_4096x2160P420_30",
    "DACOUT_4096x2160P420_50",

    "DACOUT_4096x2160P420_60",
};

static MS_BOOL getResolutionTypeAndDACIndex(MS_U16 *pPanelType, MS_U16 *pDACIndex)
{
    int u8ResolutionEnv = -1;
    char * p_str = NULL;

    if((NULL == pPanelType) || (NULL == pDACIndex))
    {
        printf("error, paremetre is invalid!\n");
        return FALSE;
    }

    p_str = getenv ("resolution");
    if(NULL != p_str)
    {
        u8ResolutionEnv = (int)simple_strtol(p_str, NULL, 10);
    }
     else
    {
        //set env normal resolution.
        setenv("resolution", "5");
        saveenv();
    }
    //printf("getResolutionTypeAndDACIndex,line:%d£¬ u8ResolutionEnv:%d\n",__LINE__,u8ResolutionEnv);

    if((RESOLUTION_COUNTER <= u8ResolutionEnv) || (0 > u8ResolutionEnv))
    {
        printf("error, resolution env is out of [0--9]!\n");
        return FALSE;
    }

    *pPanelType = staMapTypeIndex[u8ResolutionEnv].enResolutionType;
    *pDACIndex  = staMapTypeIndex[u8ResolutionEnv].enDACIndex;
    //printf("getResolutionTypeAndDACIndex,line:%d£¬ enResolutionType:%d, enDACIndex:%d\n",__LINE__,*pPanelType,*pDACIndex);

    return TRUE;
}

#if ENABLE_STB_ECOS_BOOT
static MS_BOOL getColorFormat(HDMITX_VIDEO_COLOR_FORMAT *pColorFormat)
{
    int u8ColorFormatEnv = -1;
    char * p_str = NULL;

    p_str = getenv ("colorformat");
    if(NULL != p_str)
    {
        u8ColorFormatEnv = (int)simple_strtol(p_str, NULL, 10);
    }
    else
    {
        //set env normal colorformat.
        u8ColorFormatEnv = HDMITX_VIDEO_COLOR_YUV444;
        setenv("colorformat", "2"); // 2, HDMITX_VIDEO_COLOR_YUV444
        saveenv();
    }
    //printf("getColorFormat,line:%d, u8ColorFormatEnv:%d\n",__LINE__,u8ColorFormatEnv);

    if((u8ColorFormatEnv < HDMITX_VIDEO_COLOR_RGB444) || (u8ColorFormatEnv > HDMITX_VIDEO_COLOR_YUV420))
    {
        printf("error, colorformat env is out of [0--3]!\n");
        return FALSE;
    }

    *pColorFormat = u8ColorFormatEnv;

    return TRUE;
}
#endif

MS_BOOL setHDMITxAnalogTuning(PANEL_RESOLUTION_TYPE enResolutionType)
{
    MS_BOOL bRet = FALSE;
    HDMITX_VIDEO_COLORDEPTH_VAL Val = 0;
    HDMITX_ANALOG_TUNING stHDMITxTun;
    memset(&stHDMITxTun, 0x00 , sizeof(HDMITX_ANALOG_TUNING));

    bRet = MApi_HDMITx_GetRxDCInfoFromEDID(&Val);
    if(FALSE == bRet)
    {
        printf("%s: Error: MApi_HDMITx_GetRxDCInfoFromEDID EDID is not ready, at %d\n", __func__, __LINE__);
        return FALSE;
    }

    //HD:1080P50/60£¬deep HD:1080P50/60&36 bit£¬others SD
/*    if(DACOUT_1080P_50 > enResolutionType)
    {
        memcpy(&stHDMITxTun, (HDMITX_ANALOG_TUNING*)&(astHdmiAnalogCfg[0]), sizeof(HDMITX_ANALOG_TUNING));
    }
    else */if(((DACOUT_1080P_50 == enResolutionType) || (DACOUT_1080P_60 == enResolutionType)) &&
        (HDMITX_VIDEO_CD_36Bits == Val))
    {
        memcpy(&stHDMITxTun, (HDMITX_ANALOG_TUNING*)&(astHdmiAnalogCfg[2]), sizeof(HDMITX_ANALOG_TUNING));
    }
    else if((DACOUT_1080P_50 == enResolutionType) || (DACOUT_1080P_60 == enResolutionType))
    {
        memcpy(&stHDMITxTun, (HDMITX_ANALOG_TUNING*)&(astHdmiAnalogCfg[1]), sizeof(HDMITX_ANALOG_TUNING));
    }
    else
    {
       // printf("%s: warning: setHDMITxAnalogTuning params are out of scope now, at %d\n", __func__, __LINE__);
        memcpy(&stHDMITxTun, (HDMITX_ANALOG_TUNING*)&(astHdmiAnalogCfg[0]), sizeof(HDMITX_ANALOG_TUNING));
    }

    MApi_HDMITx_AnalogTuning(&stHDMITxTun);
    return TRUE;
}

static MS_VE_VIDEOSYS getVEVideoSys(PANEL_RESOLUTION_TYPE enPanelType)
{
    switch (enPanelType)
    {
        case DACOUT_480I:
        case DACOUT_480P:
        case DACOUT_720P_60:
        case DACOUT_1080I_60:
        case DACOUT_1080P_60:
        case DACOUT_1080P_30:
        case DACOUT_1080P_24:
        case DACOUT_640x480_60:
        case DACOUT_1920x2205P_24:
        case DACOUT_1280x1470P_60:
        case DACOUT_3840x2160P_24:
        case DACOUT_3840x2160P_30:
        case DACOUT_3840x2160P_60:
        case DACOUT_4096x2160P_24:
        case DACOUT_4096x2160P_30:
        case DACOUT_4096x2160P_60:

            return MS_VE_NTSC;
        case DACOUT_576I:
        case DACOUT_576P:
        case DACOUT_720P_50:
        case DACOUT_1080I_50:
        case DACOUT_1080P_50:
        case DACOUT_1080P_25:
        case DACOUT_1280x1470P_50:
        case DACOUT_3840x2160P_50:
        case DACOUT_4096x2160P_25:
        case DACOUT_4096x2160P_50:
            return MS_VE_PAL;
        default:
            UBOOT_ERROR("ERROR: Not support current panel resolution!\n");
            while(1);
            break;
    }

    return MS_VE_VIDEOSYS_NUM;
}

static MS_U16 getFrameRates(PANEL_RESOLUTION_TYPE enPanelType)
{
    switch (enPanelType)
    {
        case DACOUT_480I:
        case DACOUT_480P:
        case DACOUT_720P_60:
        case DACOUT_1080I_60:
        case DACOUT_1080P_60:
        case DACOUT_640x480_60:
        case DACOUT_1920x2205P_24:
        case DACOUT_1280x1470P_60:
        case DACOUT_3840x2160P_24:
        case DACOUT_3840x2160P_30:
        case DACOUT_3840x2160P_60:
        case DACOUT_4096x2160P_24:
        case DACOUT_4096x2160P_30:
        case DACOUT_4096x2160P_60:
            return 6000 / 2;
        case DACOUT_576I:
        case DACOUT_576P:
        case DACOUT_720P_50:
        case DACOUT_1080I_50:
        case DACOUT_1080P_50:
        case DACOUT_1080P_25:
        case DACOUT_1280x1470P_50:
        case DACOUT_3840x2160P_50:
        case DACOUT_4096x2160P_25:
        case DACOUT_4096x2160P_50:
        default:
            break;
    }

    return 5000 / 2;
}

static HDMITX_VIDEO_TIMING getHDMITxVideoTiming(PANEL_RESOLUTION_TYPE enPanelType)
{
    switch (enPanelType)
    {
        case DACOUT_480I:
            return HDMITX_RES_720x480i;
        case DACOUT_480P:
            return HDMITX_RES_720x480p;
        case DACOUT_576I:
            return HDMITX_RES_720x576i;
        case DACOUT_576P:
            return HDMITX_RES_720x576p;
           case DACOUT_720P_50:
            return HDMITX_RES_1280x720p_50Hz;
        case DACOUT_720P_60:
            return HDMITX_RES_1280x720p_60Hz;
        case DACOUT_1080I_50:
            return HDMITX_RES_1920x1080i_50Hz;
        case DACOUT_1080I_60:
            return HDMITX_RES_1920x1080i_60Hz;
        case DACOUT_1080P_50:
            return HDMITX_RES_1920x1080p_50Hz;
        case DACOUT_1080P_60:
            return HDMITX_RES_1920x1080p_60Hz;
        case DACOUT_1080P_30:
            return HDMITX_RES_1920x1080p_30Hz;

        case DACOUT_1080P_25:
            return HDMITX_RES_1920x1080p_25Hz;

        case DACOUT_1080P_24:
            return HDMITX_RES_1920x1080p_24Hz;

        case DACOUT_1280x1470P_50:
            return HDMITX_RES_1280X1470p_50Hz;

        case DACOUT_1280x1470P_60:
            return HDMITX_RES_1280X1470p_60Hz;

        case DACOUT_3840x2160P_24:
            return HDMITX_RES_3840x2160p_24Hz;

        case DACOUT_3840x2160P_25:
            return HDMITX_RES_3840x2160p_25Hz;

        case DACOUT_3840x2160P_30:
            return HDMITX_RES_3840x2160p_30Hz;

        case DACOUT_3840x2160P_50:
            return HDMITX_RES_3840x2160p_50Hz;

        case DACOUT_3840x2160P_60:
            return HDMITX_RES_3840x2160p_60Hz;

        case DACOUT_4096x2160P_24:
            return HDMITX_RES_4096x2160p_24Hz;

        case DACOUT_4096x2160P_25:
            return HDMITX_RES_4096x2160p_25Hz;

        case DACOUT_4096x2160P_30:
            return HDMITX_RES_4096x2160p_30Hz;

        case DACOUT_4096x2160P_50:
            return HDMITX_RES_4096x2160p_50Hz;

        case DACOUT_4096x2160P_60:
            return HDMITX_RES_4096x2160p_60Hz;

        default:
            break;
    }

    return HDMITX_RES_1280x720p_50Hz;
}

#ifndef CONFIG_HDMI_OLDCHIP

void _XC_InputSource_InputPort_Mapping(XC_EX_INPUT_SOURCE_TYPE src_ids , E_MUX_INPUTPORT* port_ids , MS_U8* u8port_count)
{
    switch (src_ids)
    {
        default:
        case E_XC_EX_INPUT_SOURCE_SCALER_OP:
            *u8port_count = 1;
            port_ids[0] = INPUT_PORT_SCALER_OP;
            break;
    }
}

PANEL_RESOLUTION_TYPE InitializeEx(E_OUTPUT_VIDEO_TIMING_TYPE enDisplayRes)
{

    PANEL_RESOLUTION_TYPE enPanelTimingEx = PNL_RES_MAX_NUM;

    switch(enDisplayRes)
    {
        case E_RES_720x480I_60Hz:
        case E_RES_720x480P_60Hz:
        case E_RES_1280x720P_60Hz:
        case E_RES_1920x1080I_60Hz:
        case E_RES_1920x1080P_60Hz:
        case E_RES_1920x1080P_30Hz:
        case E_RES_640x480P_60Hz:
        case E_RES_1280x1470P_60Hz:
        case E_RES_3840x2160P_30Hz:
        case E_RES_3840x2160P_60Hz:
        case E_RES_4096x2160P_30Hz:
        case E_RES_4096x2160P_60Hz:
        case E_RES_1920x1080P_24Hz:
        case E_RES_1920x2205P_24Hz:
        case E_RES_3840x2160P_24Hz:
        case E_RES_4096x2160P_24Hz:
        case E_RES_3840x2160P420_30Hz:
        case E_RES_3840x2160P420_60Hz:
        case E_RES_4096x2160P420_30Hz:
        case E_RES_4096x2160P420_60Hz:
        case E_RES_1600x1200P_60Hz:
        case E_RES_1440x900P_60Hz:
        case E_RES_1280x1024P_60Hz:
        case E_RES_1024x768P_60Hz:
            printf("DACOUT_480I  ----\n");
            enPanelTimingEx = DACOUT_480I;
            break;

        case E_RES_720x576I_50Hz:
        case E_RES_720x576P_50Hz:
        case E_RES_1280x720P_50Hz:
        case E_RES_1920x1080I_50Hz:
        case E_RES_1920x1080P_50Hz:
        case E_RES_1920x1080P_25Hz:
        case E_RES_1280x1470P_50Hz:
        case E_RES_3840x2160P_25Hz:
        case E_RES_3840x2160P_50Hz:
        case E_RES_4096x2160P_50Hz:
        case E_RES_3840x2160P420_50Hz:
        case E_RES_4096x2160P_25Hz:
            printf("DACOUT_576I  ----\n");
            enPanelTimingEx = DACOUT_576I;
            break;

        default:
            printf("default DACOUT_576I  ----\n");
            enPanelTimingEx = DACOUT_576I;
            break;

    }
    return enPanelTimingEx;
}
#endif

MS_BOOL setHdcpKey(HDMITX_OUTPUT_MODE *enHdmiTxMode)
{
    #define HDCP_KEY_PATITION    "tvcustomer"
    #define HDCP_KEY_FILE        "/Customer/hdcpkey.bin"
    U32 HdcpKey_size = 0;
    MS_U8* HdcpKey_Buffer = NULL;

    //check hdcp key
    char * p_str = getenv("hdcp_key");
    if((NULL != p_str) && (0 != simple_strtol(p_str, NULL, 10))) //hdcp key enabled
    {
        if(vfs_mount(HDCP_KEY_PATITION)!=0)
        {
            UBOOT_ERROR("mount %s failed\n", HDCP_KEY_PATITION);
            return FALSE;
        }

        HdcpKey_size=vfs_getsize(HDCP_KEY_FILE);
        if(HdcpKey_size==0)
        {
            UBOOT_ERROR("get the file size of %s fail\n", HDCP_KEY_FILE);
            return FALSE;
        }

        HdcpKey_Buffer=(MS_U8*)malloc(HdcpKey_size);
        UBOOT_DEBUG("HdcpKey_Buffer at 0x%x\n",(unsigned int)HdcpKey_Buffer);
        if(HdcpKey_Buffer==NULL)
        {
            UBOOT_ERROR("malloc for HdcpKey_Buffer fail\n");
            return FALSE;
        }

        if(vfs_read((char *)HdcpKey_Buffer, HDCP_KEY_FILE, 0, HdcpKey_size)!=0)
        {
            free(HdcpKey_Buffer);
            UBOOT_ERROR("read %s fail\n",HDCP_KEY_FILE);
            return FALSE;
        }

        MApi_HDMITx_GetHdcpKey(FALSE, HdcpKey_Buffer);  //set hdcp key
        *enHdmiTxMode = HDMITX_HDMI_HDCP;

        vfs_umount();
        free(HdcpKey_Buffer);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
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
        switch(staMapTypeIndex[index].enResolutionType)
        {
            case DACOUT_480P:
                u8Target_16x9 = HDMITX_VIC_720x480p_60_16_9;
                u8Target_4x3 = HDMITX_VIC_720x480p_60_4_3;
                break;
            case DACOUT_576P:
                u8Target_16x9 = HDMITX_VIC_720x576p_50_16_9;
                u8Target_4x3 = HDMITX_VIC_720x576p_50_4_3;
                break;
            case DACOUT_720P_50:
                u8Target_16x9 = HDMITX_VIC_1280x720p_50_16_9;
                break;
            case DACOUT_720P_60:
                u8Target_16x9 = HDMITX_VIC_1280x720p_60_16_9;
                break;
            case DACOUT_1080P_50:
                u8Target_16x9 = HDMITX_VIC_1920x1080p_50_16_9;
                break;
            case DACOUT_1080P_60:
                u8Target_16x9 = HDMITX_VIC_1920x1080p_60_16_9;
                break;
            case DACOUT_480I:
                u8Target_16x9 = HDMITX_VIC_720x480i_60_16_9;
                u8Target_4x3 = HDMITX_VIC_720x480i_60_4_3;
                break;
            case DACOUT_576I:
                u8Target_16x9 = HDMITX_VIC_720x576i_50_16_9;
                u8Target_4x3 = HDMITX_VIC_720x576i_50_4_3;
                break;
            case DACOUT_1080I_50:
                u8Target_16x9 = HDMITX_VIC_1920x1080i_50_16_9;
                break;
            case DACOUT_1080I_60:
                u8Target_16x9 = HDMITX_VIC_1920x1080i_60_16_9;
                break;
            case DACOUT_1080P_30:
                u8Target_16x9 = HDMITX_VIC_1920x1080p_30_16_9;
                break;
            case DACOUT_1080P_25:
                u8Target_16x9 = HDMITX_VIC_1920x1080p_25_16_9;
                break;
            case DACOUT_1080P_24:
                u8Target_16x9 = HDMITX_VIC_1920x1080p_24_16_9;
                break;
            case DACOUT_3840x2160P_24:
                u8Target_16x9 = HDMITX_VIC_3840x2160p_24_16_9;
                u8Target_4x3 =  HDMITX_VIC_3840x2160p_24_64_27;
                break;
            case DACOUT_3840x2160P_25:
                u8Target_16x9 = HDMITX_VIC_3840x2160p_25_16_9;
                u8Target_4x3 =  HDMITX_VIC_3840x2160p_25_64_27;
                break;
            case DACOUT_3840x2160P_30:
                u8Target_16x9 = HDMITX_VIC_3840x2160p_30_16_9;
                u8Target_4x3 =  HDMITX_VIC_3840x2160p_30_64_27;
                break;
            case DACOUT_3840x2160P_50:
                u8Target_16x9 = HDMITX_VIC_3840x2160p_50_16_9;
                u8Target_4x3 =  HDMITX_VIC_3840x2160p_50_64_27;
                break;
            case DACOUT_3840x2160P_60:
                u8Target_16x9 = HDMITX_VIC_3840x2160p_60_16_9;
                u8Target_4x3 =  HDMITX_VIC_3840x2160p_60_64_27;
                break;
        #if (! ((ENABLE_MSTAR_CURRY == 1)&&(ENABLE_SZMI_PLATFORM == 1)) )
            // Curry SZ_MI_Platform not support now
            case DACOUT_4096x2160P_24:
                u8Target_16x9 = HDMITX_VIC_4096x2160p_24_256_135;
                u8Target_4x3 =  HDMITX_VIC_4096x2160p_24_256_135;
                break;
            case DACOUT_4096x2160P_25:
                u8Target_16x9 = HDMITX_VIC_4096x2160p_25_256_135;
                u8Target_4x3 =  HDMITX_VIC_4096x2160p_25_256_135;
                break;
            case DACOUT_4096x2160P_30:
                u8Target_16x9 = HDMITX_VIC_4096x2160p_30_256_135;
                u8Target_4x3 =  HDMITX_VIC_4096x2160p_30_256_135;
                break;
            case DACOUT_4096x2160P_50:
                u8Target_16x9 = HDMITX_VIC_4096x2160p_50_256_135;
                u8Target_4x3 =  HDMITX_VIC_4096x2160p_50_256_135;
                break;
            case DACOUT_4096x2160P_60:
                u8Target_16x9 = HDMITX_VIC_4096x2160p_60_256_135;
                u8Target_4x3 =  HDMITX_VIC_4096x2160p_60_256_135;
        #endif
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
        switch(staMapTypeIndex[index].enResolutionType)
        {
            case DACOUT_3840x2160P_30:
                u8Target_4x3 = HDMITX_VIDEO_4k2k_30Hz;
                break;
            case DACOUT_3840x2160P_25:
                u8Target_4x3 = HDMITX_VIDEO_4k2k_25Hz;
                break;
            case DACOUT_3840x2160P_24:
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
        switch(staMapTypeIndex[index].enResolutionType)
        {
            case DACOUT_3840x2160P_30:
                u8Target_16x9 = HDMITX_VIC_3840x2160p_30_16_9;
                u8Target_4x3 =  HDMITX_VIC_3840x2160p_30_64_27;
                break;
            case DACOUT_3840x2160P_25:
                u8Target_16x9 = HDMITX_VIC_3840x2160p_25_16_9;
                u8Target_4x3 =  HDMITX_VIC_3840x2160p_25_64_27;
                break;
            case DACOUT_3840x2160P_24:
                u8Target_16x9 = HDMITX_VIC_3840x2160p_24_16_9;
                u8Target_4x3 =  HDMITX_VIC_3840x2160p_24_64_27;
                break;
            case DACOUT_3840x2160P_50:
                u8Target_16x9 = HDMITX_VIC_3840x2160p_50_16_9;
                u8Target_4x3 =  HDMITX_VIC_3840x2160p_50_64_27;
                break;
            case DACOUT_3840x2160P_60:
                u8Target_16x9 = HDMITX_VIC_3840x2160p_60_16_9;
                u8Target_4x3 =  HDMITX_VIC_3840x2160p_60_64_27;
                break;
        #if (! ((ENABLE_MSTAR_CURRY == 1)&&(ENABLE_SZMI_PLATFORM == 1)) )
            // Curry SZ_MI_Platform not support now
            case DACOUT_4096x2160P_24:
                u8Target_16x9 = HDMITX_VIC_4096x2160p_24_256_135;
                u8Target_4x3 =  HDMITX_VIC_4096x2160p_24_256_135;
                break;
            case DACOUT_4096x2160P_25:
                u8Target_16x9 = HDMITX_VIC_4096x2160p_25_256_135;
                u8Target_4x3 =  HDMITX_VIC_4096x2160p_25_256_135;
                break;
            case DACOUT_4096x2160P_30:
                u8Target_16x9 = HDMITX_VIC_4096x2160p_30_256_135;
                u8Target_4x3 =  HDMITX_VIC_4096x2160p_30_256_135;
                break;
            case DACOUT_4096x2160P_50:
                u8Target_16x9 = HDMITX_VIC_4096x2160p_50_256_135;
                u8Target_4x3 =  HDMITX_VIC_4096x2160p_50_256_135;
                break;
            case DACOUT_4096x2160P_60:
                u8Target_16x9 = HDMITX_VIC_4096x2160p_60_256_135;
                u8Target_4x3 =  HDMITX_VIC_4096x2160p_60_256_135;
                break;
        #endif
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
    MS_U8 u8HDMITx[EDID_VIC_NUM] = {0};
    MS_U8 u8HDMITx4K2K[EDID_VIC_4K2K_NUM] = {0};
    MS_U8 u8HDMITx4K2K420[EDID_VIC_4K2K420_NUM] = {0};
    MS_U32 edidStatus = HDMITX_EDID_NON_STATUS;
    MS_U32 u32EdidCheckTime=0;
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

    pstDeviceInfo->bEdidReady  = TRUE;
    while((MApi_HDMITx_EdidChecking() != TRUE) && ((u32EdidCheckTime) < 6))
    {
        UBOOT_ERROR("CHeck EDID fail %d times!\n", ++u32EdidCheckTime);
    }

    if(u32EdidCheckTime >= 6)
    {
        pstDeviceInfo->bEdidReady  = FALSE;
        UBOOT_ERROR("EDID NOT READY! \n");
        return FALSE;
    }

    //step 2. get edid timing and 4k2k edid timing list
    //VSD Case
    memset(u8HDMITx, 0, EDID_VIC_NUM * sizeof(MS_U8));
    if(TRUE == MApi_HDMITx_GetRxVideoFormatFromEDID(u8HDMITx, EDID_VIC_NUM))
    {
        edidStatus |= HDMITX_EDID_VALID_BIT;
    }
#ifndef CONFIG_HDMI_OLDCHIP
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
#endif

    if(HDMITX_EDID_NON_STATUS == edidStatus)
    {
        UBOOT_DEBUG("Get edid or 4K edid or 4K420 fail\n");
        return FALSE;
    }

    //step 3. get support Timing and bestTiming info for device
    memset((void *)(pstDeviceInfo->bSupportedTiming), 0, DISPOUT_TIMING_MAX);
    memset((void *)(pstDeviceInfo->bBestSupportedTiming), 0, DISPOUT_TIMING_MAX);
    for(i=0; i<DISPOUT_TIMING_MAX; i++)
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

static MS_U8 GetDefaultTiming(void)
{
    int i = 0;
    MS_U8 index = DACOUT1080P50;
    MS_U8 skip_timing[] = {
        14,    // DACOUT_1920x2205P_24
        15,    // DACOUT_1280x1470P_50
        16,    // DACOUT_1280x1470P_60
    };

    UBOOT_TRACE("IN\n");
    char *env_default_res = getenv(ENV_HDMI_DEFAULT_RES);
    if (env_default_res == NULL)
    {
        UBOOT_DEBUG("env(%s) is empty, using default : %s\n", ENV_HDMI_DEFAULT_RES, DacPropPanelIndexTbl[index]);
        UBOOT_TRACE("OK\n");
        return index;
    }

    MS_U32 default_res = simple_strtoul(env_default_res, NULL, 0);
    UBOOT_DEBUG("%s = %d\n", ENV_HDMI_DEFAULT_RES, (int)default_res);
    if (default_res >= DACOUT3840P420_30)
    {
        UBOOT_DEBUG("invalid timing(%d) > DACOUT3840P420_30(%d)\n", (int)default_res, DACOUT3840P420_30);
        UBOOT_DEBUG("using default : %s\n", DacPropPanelIndexTbl[index]);
        return index;
    }

    index = default_res;
    for (i = 0; i < (sizeof(skip_timing)/sizeof(skip_timing[0])); i++)
    {
        if (skip_timing[i] == default_res)
        {
            index = DACOUT1080P50;
            UBOOT_DEBUG("Do not use %s as default timing\n", DacPropPanelIndexTbl[default_res]);
            break;
        }
    }

    UBOOT_TRACE("OK\n");
    return index;
}


static MsHDMITX_AUTO_TIMING_TYPE GetAutoTimingType(void)
{
    char * pstr = NULL;
    MS_U32 value = 0;
    MsHDMITX_AUTO_TIMING_TYPE ret = HDMITX_NO_CHANGE_TIMING;

    UBOOT_TRACE("IN\n");
    pstr = getenv("auto_timing");
    if (pstr == NULL)
    {
        UBOOT_TRACE("OK\n");
        return ret;
    }
    UBOOT_DEBUG("auto_timing = %s\n", pstr);

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

static MS_U8 GetMaxTimingFrList(MS_U8 *timing_list, MS_U8 timing_cnt, MS_BOOL check_ini_exist)
{
    UBOOT_DEBUG("IN\n");
    MS_U8  max_timing;
    MS_U8  mw_max_index;
    MS_U8  i, index, mw_index;
    MS_U32 filesize;
    char   cmd_buf[CMD_BUF];

    // init
    mw_max_index = 0;
    max_timing   = DISPOUT_TIMING_MAX;
    if (check_ini_exist == TRUE)
    {
        if(vfs_mount(CONFIG)!= 0)
        {
            UBOOT_ERROR("vfs_mount fail\n");
            return max_timing;
        }
    }

    for (i = 0; i < timing_cnt; i++)
    {
        index = timing_list[i];
        mw_index = staMapTypeIndex[index].enMWIndex;
        if (mw_index == E_MW_PICTURE_DISPOUT_TIMING_NOT_SUPPORT)
        {
            UBOOT_DEBUG("MW do not support this timing(%d), skip it\n", index);
            continue;
        }
        else if (mw_index >= mw_max_index)
        {
            if (check_ini_exist == TRUE)
            {
                snprintf(cmd_buf, sizeof(cmd_buf),"/config/panel/%s.ini", DacPropPanelIndexTbl[index]);
                filesize = vfs_getsize(cmd_buf);
                if( (filesize > 0x10000) || (filesize == 0) )
                {
                    UBOOT_DEBUG("%s does not exist, move to next resolution\n", cmd_buf);
                    continue;
                }
            }
            mw_max_index = mw_index;
            max_timing = index;
        }
    }
    UBOOT_DEBUG("OK\n");
    return max_timing;
}

static MS_U8 CheckSupportTiming(MS_U8 current_timing, MsHDMITX_DEVICEINFO *pstDeviceInfo)
{
    MS_U8 index = 0;
    MS_U8 u8BestTimingCnt = 0;
    MS_U8 u8SupportTimingCnt = 0;
    MS_U8 u8BestTimingList[DISPOUT_TIMING_MAX]={0};
    MS_U8 u8SupportTimingList[DISPOUT_TIMING_MAX]={0};
    MS_U8 prop_timing = 0;
    UBOOT_TRACE("IN\n");

    UBOOT_DEBUG("current_timing = %d\n", current_timing);
    if(GetDeviceInfo(pstDeviceInfo) == TRUE)
    {
        // no-auto_timing and current timing is support
        if((GetAutoTimingType() == HDMITX_NO_CHANGE_TIMING) && ((pstDeviceInfo->bSupportedTiming)[current_timing] == TRUE))
        {
            UBOOT_TRACE("OK\n");
            if (get_first_boot_status() == 1)
            {
                clear_first_boot_status();
            }
            return current_timing;
        }

        //restore best timing and highest timing
        for(index=0; index<DISPOUT_TIMING_MAX; index++)
        {
            if((pstDeviceInfo->bBestSupportedTiming)[index] == TRUE)
            {
                UBOOT_DEBUG("--Support Native Timing is: %s\n", DacPropPanelIndexTbl[index]);
                u8BestTimingList[u8BestTimingCnt++] = index;
            }
            if((pstDeviceInfo->bSupportedTiming)[index] == TRUE)
            {
                UBOOT_DEBUG("--Support Timing is: %s\n", DacPropPanelIndexTbl[index]);
                u8SupportTimingList[u8SupportTimingCnt++] = index;
            }
        }

        // Select the required timing
        if(GetAutoTimingType() == HDMITX_BEST_TIMING)        // use best timing
        {
            prop_timing = GetMaxTimingFrList(u8BestTimingList, u8BestTimingCnt, FALSE);
            if(prop_timing >= DISPOUT_TIMING_MAX)
            {
                // can not find the MW_support timing.
                prop_timing = GetMaxTimingFrList(u8SupportTimingList, u8SupportTimingCnt, FALSE);
                if ((prop_timing >= DACOUT3840P24)&&((pstDeviceInfo->bSupportedTiming)[DACOUT1080P60] == TRUE))
                {
                    prop_timing = DACOUT1080P60;
                }
                else if ((prop_timing >= DACOUT1080P50)&&((pstDeviceInfo->bSupportedTiming)[DACOUT720P60] == TRUE))
                {
                    prop_timing = DACOUT720P60;
                }
                else
                {
                    prop_timing = DACOUT720P50;
                }
            }
            UBOOT_INFO("\nthe best_timing is %s\n", DacPropPanelIndexTbl[prop_timing]);
        }
        else if(GetAutoTimingType() == HDMITX_MAX_TIMING)    // use max timing
        {
            prop_timing = GetMaxTimingFrList(u8SupportTimingList, u8SupportTimingCnt, FALSE);
            if(prop_timing >= DISPOUT_TIMING_MAX)
            {
                // can NOT find the MW_support timing, so use the default timing
                UBOOT_DEBUG("set the max_timing to default(%d)\n", DACOUT720P50);
                prop_timing = DACOUT720P50;
            }
            UBOOT_INFO("\nthe max_timing is %s\n", DacPropPanelIndexTbl[prop_timing]);
        }
        else    // no-auto_timing and current timing is NOT support
        {
            UBOOT_DEBUG("\nu8SupportTimingCnt = %d\n", u8SupportTimingCnt);
            prop_timing = GetMaxTimingFrList(u8SupportTimingList, u8SupportTimingCnt, TRUE);
            if (prop_timing >=  DISPOUT_TIMING_MAX)
            {
                // case : can NOT find the required timing
                UBOOT_DEBUG("can NOT find the required timing\n");
                prop_timing = DACOUT720P50;
            }
            else
            {
                if (current_timing >= DACOUT3840P24)
                {
                    UBOOT_DEBUG("No-Auto timing(4K/4K+) case : select max timing = %s\n", DacPropPanelIndexTbl[prop_timing]);
                }
                else
                {
                    if (get_first_boot_status() == 1)
                    {
                        UBOOT_DEBUG("No-Auto timing(4K-), After factory_reset/ota_upgrade/write_image, ");
                        UBOOT_DEBUG("select max timing = %s\n", DacPropPanelIndexTbl[prop_timing]);
                    }
                    else
                    {
                        prop_timing = current_timing;
                        UBOOT_DEBUG("No-Auto timing(4K-) case : no change timing(%s)\n", DacPropPanelIndexTbl[prop_timing]);
                    }
                }
            }
        }
    }
    else
    {
        if (MApi_HDMITx_GetRxStatus() == TRUE)
        {
            if (GetAutoTimingType() != HDMITX_NO_CHANGE_TIMING)
            {
                prop_timing = GetDefaultTiming();
                UBOOT_DEBUG("No-Edid Auto timing case : default timing = %s\n", DacPropPanelIndexTbl[prop_timing]);
            }
            else
            {
                if (current_timing >= DACOUT3840P24)
                {
                    prop_timing = GetDefaultTiming();
                    UBOOT_DEBUG("No-Edid No-Auto timing(4K/4K+) case : default timing = %s\n", DacPropPanelIndexTbl[prop_timing]);
                }
                else
                {
                    prop_timing = current_timing;
                    UBOOT_DEBUG("No-Edid No-Auto timing(4K-) case : no change timing(%s)\n", DacPropPanelIndexTbl[prop_timing]);
                }
            }
        }
    }

    if (get_first_boot_status() == 1)
    {
        clear_first_boot_status();
    }
    UBOOT_TRACE("OK\n");
    return prop_timing;
}

static MS_BOOL GetHdmitxStatus(MsHDMITX_INFO *pstHdmitxInfo)
{
    char *pstPath, *pstBuffer;
    int ret = -1;
    int i = 0;
    MS_BOOL bRet = TRUE;
    MS_U32 u32fileSize = 0;
    MsHDMITX_INFO stHdmitxInfo;
    char str[512]="\0";
    char tmp[32]="\0";
    char index[4]="\0";

    ret = vfs_mount("tvdatabase");
    if(ret != 0)
    {
        UBOOT_ERROR("Mount tvocnfig fail.\n");
        return FALSE;
    }

    pstPath = "/HdmitxInfo.ini";
    u32fileSize = vfs_getsize(pstPath);

    pstBuffer = (char *)malloc(u32fileSize);
    if(pstBuffer == NULL)
    {
        UBOOT_ERROR("No enough buffer %s %s\n",__FILE__,__FUNCTION__);
        return FALSE;
    }

    if(vfs_read((void *)pstBuffer, pstPath, 0, u32fileSize)==-1)
    {
        free(pstBuffer);
        UBOOT_ERROR("Error: vfs_read Fail \n");
        return FALSE;
    }

    Profile_Init(pstBuffer, u32fileSize);
    if(Profile_GetString("HdmitxInfo", "HDMITXMODE", "", str, sizearray(str)))
        stHdmitxInfo.eHdmitxMode = atoi(str);
    else
        bRet = FALSE;

    if(Profile_GetString("HdmitxInfo", "HDMITXINCOLOR", "", str, sizearray(str)))
        stHdmitxInfo.eHdmitxInColor = atoi(str);
    else
        bRet = FALSE;

    if(Profile_GetString("HdmitxInfo", "HDMITXOUTCOLOR", "", str, sizearray(str)))
        stHdmitxInfo.eHdmitxOutColor = atoi(str);
    else
        bRet = FALSE;

    if(Profile_GetString("HdmitxInfo", "HDMITXCOLORDEPTH", "", str, sizearray(str)))
        stHdmitxInfo.eHdmitxColorDepth = atoi(str);
    else
        bRet = FALSE;

    if(Profile_GetString("HdmitxInfo", "HDMITXVIDEOTIMING", "", str, sizearray(str)))
        stHdmitxInfo.eHdmitxVideoTiming = atoi(str);
    else
        bRet = FALSE;

    if(Profile_GetString("HdmitxInfo", "AUDIOFREQ", "", str, sizearray(str)))
        stHdmitxInfo.eAudioFreq = atoi(str);
    else
        bRet = FALSE;

    if(Profile_GetString("HdmitxInfo", "AUDIOCHCNT", "", str, sizearray(str)))
        stHdmitxInfo.eAudioChCnt = atoi(str);
    else
        bRet = FALSE;

    if(Profile_GetString("HdmitxInfo", "AUDIOCODETYPE", "", str, sizearray(str)))
        stHdmitxInfo.eAudioCodeType = atoi(str);
    else
        bRet = FALSE;

    if(Profile_GetString("HdmitxInfo", "AUDIOMUTE", "", str, sizearray(str)))
    {
        if(HDMITX_AUDIO_UNMUTE == atoi(str))
            stHdmitxInfo.bAudioOn = TRUE;
        else
            stHdmitxInfo.bAudioOn = FALSE;
    }
    else
        bRet = FALSE;

    if(Profile_GetString("HdmitxInfo", "OSD_SDR2HDR_EN", "", str, sizearray(str)))
        stHdmitxInfo.u8OsdSdr2HdrEnable = atoi(str);
    else
        bRet = FALSE;

    if(Profile_GetString("HdmitxInfo", "SEAMLESS_EN", "", str, sizearray(str)))
        stHdmitxInfo.u8SeamlessEnable = atoi(str);
    else
        bRet = FALSE;

    for(i=0; i<HDMITX_HDR_INFOFRAME_DYNAMIC_RANGE_ARRAY_SIZE; i++)
    {
        memcpy(tmp, "CurHDRInfoFrame", sizeof("CurHDRInfoFrame"));
        sprintf(index, "%02d", i);
        strcat(tmp, index); // tmp = "CurHDRInfoFrameXX"

        if(Profile_GetString("HdmitxInfo", tmp, "", str, sizearray(str)))
            stHdmitxInfo.u8CurHDRInfoFrame[i] = atoi(str);
    }

    if(Profile_GetString("HdmitxInfo", "ExtendedColorimetry", "", str, sizearray(str)))
        stHdmitxInfo.u8ExtendedColorimetry = atoi(str);
    else
        bRet = FALSE;

    if(Profile_GetString("HdmitxInfo", "YccQuantizationRange", "", str, sizearray(str)))
        stHdmitxInfo.u8YccQuantizationRange = atoi(str);
    else
        bRet = FALSE;

    if(FALSE == bRet)
    {
        UBOOT_ERROR("Get profile string fail. \n");
        free(pstBuffer);
        return FALSE;
    }

    memcpy(pstHdmitxInfo, &stHdmitxInfo, sizeof(MsHDMITX_INFO));

    free(pstBuffer);
    return TRUE;
}

static MS_BOOL _CheckRxSupportColorDepthFormatForSpecificTiming(HDMITX_OUTPUT_MODE eOutMode, HDMITX_VIDEO_TIMING eVideoTiming, HDMITX_VIDEO_COLOR_FORMAT eOutputColor, HDMITX_VIDEO_COLORDEPTH_VAL eColorDepth)
{
    HDMITX_CHECK_LEGAL_TIMING HDMITXArgs= {0,eOutMode,eVideoTiming,HDMITX_VIDEO_COLOR_YUV444,eOutputColor,eColorDepth,HDMITX_TIMING_ERR_NONE};

    if(!MApi_HDMITx_GetRxStatus() || !MApi_HDMITx_EdidChecking())
    {
        UBOOT_DEBUG("DEVICE_HDMITX EDID not ready!\n");
        return FALSE;
    }

    switch(eOutputColor)
    {
        case HDMITX_VIDEO_COLOR_RGB444:
            HDMITXArgs.eInColor = HDMITX_VIDEO_COLOR_YUV444;
            break;
        case HDMITX_VIDEO_COLOR_YUV422:
            HDMITXArgs.eInColor = HDMITX_VIDEO_COLOR_YUV444;
            break;
        case HDMITX_VIDEO_COLOR_YUV444:
            HDMITXArgs.eInColor = HDMITX_VIDEO_COLOR_YUV444;
            break;
        case HDMITX_VIDEO_COLOR_YUV420:
            HDMITXArgs.eInColor = HDMITX_VIDEO_COLOR_YUV420;
            break;
    }

    MApi_HDMITx_GeneralCtrl(HDMITX_CMD_TIMING_CAPABILITY_CHECK, &HDMITXArgs, sizeof(HDMITX_CHECK_LEGAL_TIMING));
    UBOOT_DEBUG("InColor:%d, OutColor:%d, OutputMode:%d, ColorDepth:%d, OutputTiming:%d --> %s\n",
                      HDMITXArgs.eInColor, eOutputColor, eOutMode, eColorDepth, eVideoTiming, (HDMITXArgs.ubRet)?"not support":"support");

    if(HDMITXArgs.ubRet == HDMITX_TIMING_ERR_NONE)
        return TRUE;

    return FALSE;
}

//This function is used to get hdmi video color capacity,
//You can set the value in menuconfig
//-> Module Options                                                                                                                                    .
//  .       -> HDMI_TX (HDMI_TX [=y])                                                                                                                          .
//  .         -> HDMI TX Out Video color cap
//If you did not set this value through menuconfig, the default value is YUV420

static HDMITX_VIDEO_COLOR_FORMAT GetHdmitxOutVideoColorCap(void)
{
#if defined(CONFIG_HDMITX_VIDEO_COLOR_RGB444) && (CONFIG_HDMITX_VIDEO_COLOR_RGB444 == 1)
    return HDMITX_VIDEO_COLOR_RGB444;
#elif defined (CONFIG_HDMITX_VIDEO_COLOR_YUV422) && (CONFIG_HDMITX_VIDEO_COLOR_YUV422 == 1)
    return HDMITX_VIDEO_COLOR_YUV422;
#elif defined (CONFIG_HDMITX_VIDEO_COLOR_YUV444) && (CONFIG_HDMITX_VIDEO_COLOR_YUV444 == 1)
    return HDMITX_VIDEO_COLOR_YUV444;
#elif defined (CONFIG_HDMITX_VIDEO_COLOR_YUV420) && (CONFIG_HDMITX_VIDEO_COLOR_YUV420 == 1)
    return HDMITX_VIDEO_COLOR_YUV420;
#else
    return HDMITX_VIDEO_COLOR_YUV420;
#endif
}

//This function is used to get hdmi video color capacity,
//You can set the value in menuconfig
//-> Module Options                                                                                                                                    .
//     -> Module Options                                                                                                                                    .
//  .       -> HDMI_TX (HDMI_TX [=y])                                                                                                                          .
//  .         -> HDMI color depth cap
//If you did not set this value through menuconfig, the default value is HDMITX_VIDEO_CD_48Bits
static HDMITX_VIDEO_COLORDEPTH_VAL GetHdmitxColorDepthCap(void)
{
#if defined (CONFIG_HDMITX_VIDEO_CD_24Bits) && (CONFIG_HDMITX_VIDEO_CD_24Bits == 1)
    return HDMITX_VIDEO_CD_24Bits;
#elif defined (CONFIG_HDMITX_VIDEO_CD_30Bits) && (CONFIG_HDMITX_VIDEO_CD_30Bits == 1)
    return HDMITX_VIDEO_CD_30Bits;
#elif defined (CONFIG_HDMITX_VIDEO_CD_36Bits) && (CONFIG_HDMITX_VIDEO_CD_36Bits == 1)
    return HDMITX_VIDEO_CD_36Bits;
#elif defined (CONFIG_HDMITX_VIDEO_CD_48Bits) && (CONFIG_HDMITX_VIDEO_CD_48Bits == 1)
    return HDMITX_VIDEO_CD_48Bits;
#else
    return HDMITX_VIDEO_CD_48Bits;
#endif
}

static MS_BOOL GetHdmitxColorInfo(HDMITX_OUTPUT_MODE eOutMode, MS_U8 *pColorDepth, MS_U8 *pColorFormat, HDMITX_VIDEO_TIMING eVideoTiming)
{
    E_HDMITX_COLOR_FORMAT_DEPTH enHdmiColor = E_HDMITX_COLOR_FORMAT_DEPTH_MAX;
    char* p_str = NULL;
    MS_U8 u8AutoColorDepth = HDMITX_VIDEO_CD_24Bits;
    MS_U8 u8AutoColorFormat = HDMITX_VIDEO_COLOR_YUV444;
    HDMITX_VIDEO_COLORDEPTH_VAL eMaxColorDepth[HDMITX_VIDEO_COLOR_YUV420 + 1] = {HDMITX_VIDEO_CD_NoID};
    MS_BOOL bCusHdmiColor = FALSE;

    if((NULL == pColorDepth) || (NULL == pColorFormat))
    {
        UBOOT_ERROR("error, paremetre is invalid!\n");
        return FALSE;
    }

    p_str = getenv("hdmicolor");
    if(NULL != p_str)
    {
        bCusHdmiColor = TRUE;
        enHdmiColor = (E_HDMITX_COLOR_FORMAT_DEPTH)simple_strtol(p_str, NULL, 10);
        UBOOT_DEBUG("hdmicolor: 0x%x\n", enHdmiColor);
        switch(enHdmiColor)
        {
            case E_HDMITX_COLOR_YUV420_8BITS:
                *pColorFormat = HDMITX_VIDEO_COLOR_YUV420;
                *pColorDepth = HDMITX_VIDEO_CD_24Bits;
            break;
            case E_HDMITX_COLOR_YUV420_10BITS:
                *pColorFormat = HDMITX_VIDEO_COLOR_YUV420;
                *pColorDepth = HDMITX_VIDEO_CD_30Bits;
            break;
            case E_HDMITX_COLOR_YUV420_12BITS:
                *pColorFormat = HDMITX_VIDEO_COLOR_YUV420;
                *pColorDepth = HDMITX_VIDEO_CD_36Bits;
            break;
            case E_HDMITX_COLOR_YUV420_16BITS:
                *pColorFormat = HDMITX_VIDEO_COLOR_YUV420;
                *pColorDepth = HDMITX_VIDEO_CD_48Bits;
            break;
            case E_HDMITX_COLOR_YUV422_NoID:
                *pColorFormat = HDMITX_VIDEO_COLOR_YUV422;
                *pColorDepth = HDMITX_VIDEO_CD_NoID;
            break;
            case E_HDMITX_COLOR_RGB444_8BITS:
                *pColorFormat = HDMITX_VIDEO_COLOR_RGB444;
                *pColorDepth = HDMITX_VIDEO_CD_24Bits;
            break;
            case E_HDMITX_COLOR_RGB444_10BITS:
                *pColorFormat = HDMITX_VIDEO_COLOR_RGB444;
                *pColorDepth = HDMITX_VIDEO_CD_30Bits;
            break;
            case E_HDMITX_COLOR_RGB444_12BITS:
                *pColorFormat = HDMITX_VIDEO_COLOR_RGB444;
                *pColorDepth = HDMITX_VIDEO_CD_36Bits;
            break;
            case E_HDMITX_COLOR_RGB444_16BITS:
                *pColorFormat = HDMITX_VIDEO_COLOR_RGB444;
                *pColorDepth = HDMITX_VIDEO_CD_48Bits;
            break;
            case E_HDMITX_COLOR_YUV444_8BITS:
                *pColorFormat = HDMITX_VIDEO_COLOR_YUV444;
                *pColorDepth = HDMITX_VIDEO_CD_24Bits;
            break;
            case E_HDMITX_COLOR_YUV444_10BITS:
                *pColorFormat = HDMITX_VIDEO_COLOR_YUV444;
                *pColorDepth = HDMITX_VIDEO_CD_30Bits;
            break;
            case E_HDMITX_COLOR_YUV444_12BITS:
                *pColorFormat = HDMITX_VIDEO_COLOR_YUV444;
                *pColorDepth = HDMITX_VIDEO_CD_36Bits;
            break;
            case E_HDMITX_COLOR_YUV444_16BITS:
                *pColorFormat = HDMITX_VIDEO_COLOR_YUV444;
                *pColorDepth = HDMITX_VIDEO_CD_48Bits;
            break;
            default:
                UBOOT_DEBUG("env hdmicolor value not in list, use auto color depth&fmt\n");
                bCusHdmiColor = FALSE;
            break;
        }
    }

    if(MApi_HDMITx_GetRxStatus() && MApi_HDMITx_EdidChecking() && eOutMode == HDMITX_HDMI)
    {
        HDMITX_VIDEO_COLOR_FORMAT eOutputColor;
        HDMITX_VIDEO_COLORDEPTH_VAL eEDIDColorDepth;
        MS_BOOL bSupported[HDMITX_VIDEO_COLOR_YUV420 + 1][HDMITX_VIDEO_CD_48Bits + 1] = {{FALSE}};
        bSupported[HDMITX_VIDEO_COLOR_YUV422][HDMITX_VIDEO_CD_NoID] = _CheckRxSupportColorDepthFormatForSpecificTiming(eOutMode, eVideoTiming, HDMITX_VIDEO_COLOR_YUV422, HDMITX_VIDEO_CD_NoID);
        for(eOutputColor=0; eOutputColor<=GetHdmitxOutVideoColorCap(); eOutputColor++)
        {
            for(eEDIDColorDepth=HDMITX_VIDEO_CD_24Bits; eEDIDColorDepth<=GetHdmitxColorDepthCap(); eEDIDColorDepth++)
            {
                if(eOutputColor == HDMITX_VIDEO_COLOR_YUV422)
                {
                    continue;
                }
                bSupported[eOutputColor][eEDIDColorDepth] = _CheckRxSupportColorDepthFormatForSpecificTiming(eOutMode, eVideoTiming, eOutputColor, eEDIDColorDepth);
                //only support max to 12bits
                if((bSupported[eOutputColor][eEDIDColorDepth] == TRUE) && (eEDIDColorDepth != HDMITX_VIDEO_CD_48Bits))
                {
                    eMaxColorDepth[eOutputColor] = eEDIDColorDepth;
                }
            }
            if(u8AutoColorDepth <= eMaxColorDepth[eOutputColor])
            {
                u8AutoColorDepth = eMaxColorDepth[eOutputColor];
                u8AutoColorFormat = eOutputColor;
            }
        }

        if(bCusHdmiColor == FALSE || bSupported[*pColorFormat][*pColorDepth] == FALSE)
        {
            if((eVideoTiming == HDMITX_RES_3840x2160p_50Hz) || (eVideoTiming == HDMITX_RES_3840x2160p_60Hz) ||
               (eVideoTiming == HDMITX_RES_4096x2160p_50Hz) || (eVideoTiming == HDMITX_RES_4096x2160p_60Hz))
            {
                *pColorFormat = HDMITX_VIDEO_COLOR_YUV420;
                *pColorDepth = eMaxColorDepth[HDMITX_VIDEO_COLOR_YUV420];
            }
            else
            {
                *pColorFormat = u8AutoColorFormat;
                *pColorDepth = u8AutoColorDepth;
            }
            bCusHdmiColor = FALSE;
        }
    }
    else
    {
        p_str = getenv ("colorformat");
        if(NULL != p_str)
        {
            MS_U8 u8ColorFormatEnv = (MS_U8)simple_strtol(p_str, NULL, 10);
            if((u8ColorFormatEnv < HDMITX_VIDEO_COLOR_RGB444) || (u8ColorFormatEnv > HDMITX_VIDEO_COLOR_YUV420))
            {
                printf("error, colorformat env is out of [0--3]!\n");
                u8ColorFormatEnv = 2;
            }
            *pColorFormat = (HDMITX_VIDEO_COLOR_FORMAT)u8ColorFormatEnv;
            if(eOutMode == HDMITX_DVI)
            {
                *pColorFormat = HDMITX_VIDEO_COLOR_RGB444;
            }
            bCusHdmiColor = TRUE;
        }
        else
        {
            if((eVideoTiming == HDMITX_RES_3840x2160p_50Hz) || (eVideoTiming == HDMITX_RES_3840x2160p_60Hz) ||
               (eVideoTiming == HDMITX_RES_4096x2160p_50Hz) || (eVideoTiming == HDMITX_RES_4096x2160p_60Hz))
            {
                *pColorFormat = HDMITX_VIDEO_COLOR_YUV420;
                MApi_HDMITx_ForceHDMIOutputMode(TRUE, HDMITX_HDMI);
                MApi_HDMITx_ForceHDMIOutputColorFormat(TRUE, HDMITX_VIDEO_COLOR_YUV420);
                printf("force 420 10bit out when 4K50/60! \n");
                bCusHdmiColor = FALSE;
            }
        }
    }

#if ENABLE_STB_ECOS_BOOT
    char cmd_buf[CMD_BUF] = "\0";
    snprintf(cmd_buf, sizeof(cmd_buf),"%d", *pColorFormat);
    setenv("colorformat", cmd_buf);
#endif
    if(*pColorDepth == HDMITX_VIDEO_CD_24Bits)
    {
        *pColorDepth = HDMITX_VIDEO_CD_NoID;
    }
    UBOOT_DEBUG("pColorFormat: %d, pColorDepth: %d\n", *pColorFormat, *pColorDepth);
    return bCusHdmiColor;
}

static void GetInColorFmt(HDMITX_VIDEO_COLOR_FORMAT *pInColorFmt, HDMITX_VIDEO_COLOR_FORMAT *pOutColorFmt)
{
    switch(*pOutColorFmt)
    {
        case HDMITX_VIDEO_COLOR_RGB444:
            *pInColorFmt = HDMITX_VIDEO_COLOR_YUV444;
            break;
        case HDMITX_VIDEO_COLOR_YUV422:
            *pInColorFmt = HDMITX_VIDEO_COLOR_YUV444;
            break;
        case HDMITX_VIDEO_COLOR_YUV444:
            *pInColorFmt = HDMITX_VIDEO_COLOR_YUV444;
            break;
        case HDMITX_VIDEO_COLOR_YUV420:
            *pInColorFmt = HDMITX_VIDEO_COLOR_YUV420;
            break;
    }
}

HDMITX_OUTPUT_MODE GetHdmiMode(void)
{
    MS_U32 u32StartTime = 0;
    MS_BOOL bHDMISupport = FALSE;
    HDMITX_OUTPUT_MODE    eOutMode = HDMITX_HDMI;

    u32StartTime = MsOS_GetSystemTime();
    while(MsOS_Timer_DiffTimeFromNow(u32StartTime) < 10)
    {
        MApi_HDMITx_EDID_HDMISupport(&bHDMISupport);  // always return TRUE;
    }
    if (bHDMISupport == FALSE)
    {
        UBOOT_ERROR("EDID NOT SUPPORT HDMI! \n");
        // if this env is not exist, default is force hdmi mode
        MS_BOOL bForceHdmiMode = TRUE;
        char *p_str = NULL;
        p_str = getenv("force_hdmi");
        if(NULL != p_str)
        {
            bForceHdmiMode = (int)simple_strtol(p_str, NULL, 10);
            UBOOT_DEBUG("bForceHdmiMode = %d\n", bForceHdmiMode);
        }

        if(bForceHdmiMode)
        {
            eOutMode = HDMITX_HDMI;
            printf("Force to HDMI mode! \n");
        }
        else
        {
            eOutMode = HDMITX_DVI;
            printf("Set to DVI mode! \n");
        }
    }
    return eOutMode;
}

#if (CONFIG_DISPLAY_LOGO_SEAMLESS == 1)
static void SendTxHDRInfo(MS_BOOL bSendHDRInfoFrame)
{
    int idx = 0;
    MS_U8 u8HDRInfoFrame[HDMITX_HDR_INFOFRAME_DYNAMIC_RANGE_ARRAY_SIZE] = {0};
    if(bSendHDRInfoFrame == TRUE)
    {
        u8HDRInfoFrame[0] = HDMITX_HDR_INFOFRAME_DYNAMIC_RANGE;                //InfoFrame type
        u8HDRInfoFrame[1] = HDMITX_HDR_INFOFRAME_DYNAMIC_RANGE_VERSION;        //Version
        u8HDRInfoFrame[2] = HDMITX_HDR_INFOFRAME_DYNAMIC_RANGE_PACKET_LENGHT;  //Length of the following HDR InfoFrame
        for(idx=0; idx<HDMITX_HDR_INFOFRAME_DYNAMIC_RANGE_PACKET_LENGHT; idx++)
        {
            u8HDRInfoFrame[3+idx] = g_u8HDRPackage[idx];
        }

        UBOOT_INFO("\n---------------HDR PACKAGE:------------\n");
        for (idx=0; idx<28; idx++)
        {
            UBOOT_INFO("0x%x ", u8HDRInfoFrame[idx]);
            if(idx%8 == 7)
                UBOOT_INFO("\n");
        }
        UBOOT_INFO("\n----------------------------------------\n");

        MApi_HDMITx_PKT_Content_Define(HDMITX_HDR_INFOFRMAE, u8HDRInfoFrame, sizeof(u8HDRInfoFrame));
        MApi_HDMITx_PKT_User_Define(HDMITX_HDR_INFOFRMAE, TRUE, HDMITX_CYCLIC_PACKET, 1);
    }
    else
    {
        MApi_HDMITx_PKT_User_Define(HDMITX_HDR_INFOFRMAE, FALSE, HDMITX_STOP_PACKET, 1);
    }
}

static void HDRInfo_Init(HDMITX_VIDEO_COLOR_FORMAT eOutColorFmt)
{
    char * p_str = NULL;
    MS_U8 u8HSMDB[HDMITX_HDR_STATIC_METADATA_DATA_BLOCK_LENGTH] = {0};
    MS_U32 u32HSMDBLength = HDMITX_HDR_STATIC_METADATA_DATA_BLOCK_LENGTH;
    MS_U16 u16HDRType = 1;

    if((MApi_HDMITx_GetRxStatus() == TRUE) && (!isBootToRecovery()))
    {
        MApi_HDMITx_GetEdidDataBlocks(E_CEA_TAG_CODE_EXT_TAG, E_CEA_EXT_TAG_HSMDB, &u8HSMDB[0], u32HSMDBLength, &u32HSMDBLength);

        if (u8HSMDB[0] != 0)
        {
            UBOOT_DEBUG("Sink support HDR! \n");
            p_str = getenv ("HDR_UI_STATUS"); // MW gived. 0 = off, 1 = auto, 2 = on
            if(NULL != p_str)
            {
                u16HDRType = (((MS_U32)simple_strtol(p_str, NULL, 10)) >> 16) & 0xFFFF;
            }

            if(u16HDRType == 1)
            {
                UBOOT_DEBUG("Set HDR Info and AVI Info for seamless bootlogo \n");
                setenv("hdr_status", "1");
                saveenv();
                SendTxHDRInfo(TRUE);
                if(eOutColorFmt == HDMITX_VIDEO_COLOR_RGB444)
                {
                    MApi_HDMITx_SetAVIInfoExtColorimetry(HDMITX_EXT_COLORIMETRY_BT2020RGB, HDMITX_YCC_QUANT_LIMIT);
                }
                else
                {
                    MApi_HDMITx_SetAVIInfoExtColorimetry(HDMITX_EXT_COLORIMETRY_BT2020YCC, HDMITX_YCC_QUANT_LIMIT);
                }
                MApi_HDMITx_PKT_User_Define(HDMITX_AVI_INFOFRAME, FALSE, HDMITX_CYCLIC_PACKET, 1);
                return;
            }
        }

        UBOOT_DEBUG("Sink NOT support HDR! \n");
        setenv("hdr_status","0");
        saveenv();
    }
    else
    {
        UBOOT_DEBUG("HDMI no connect, clear env hdr_stauts!\n");
        setenv("hdr_status","");
        saveenv();
    }
}
#endif

void DacTbl_Init(E_OUTPUT_VIDEO_TIMING_TYPE enDACIndex, HDMITX_VIDEO_COLORDEPTH_VAL eEDIDColorDepth)
{
    MApi_DAC_SetYPbPrOutputTiming(enDACIndex);

#if !defined(CONFIG_HDMI_OLDCHIP) && !defined(CONFIG_K6LITE)
    MS_U8 *DacTbl=NULL;
    // dump DAC general/scaler/HDGEN register tables
    MApi_DAC_DumpTable(DacTbl, DAC_TABTYPE_INIT);
    MApi_DAC_DumpTable(DacTbl, DAC_TABTYPE_INIT_SC);
    MApi_DAC_DumpTable(DacTbl, DAC_TABTYPE_INIT_HDGEN);

    if(eEDIDColorDepth == HDMITX_VIDEO_CD_30Bits)
    {
        MApi_DAC_DumpTable(DacTbl, DAC_TABTYPE_INIT_HDMITX_10BIT_Divider);
        MApi_DAC_DumpTable(DacTbl, DAC_TABTYPE_INIT_HDMITX_10BIT);
    }
    else if(eEDIDColorDepth == HDMITX_VIDEO_CD_36Bits)
    {
        MApi_DAC_DumpTable(DacTbl, DAC_TABTYPE_INIT_HDMITX_12BIT_Divider);
        MApi_DAC_DumpTable(DacTbl, DAC_TABTYPE_INIT_HDMITX_12BIT);
    }
    else if(eEDIDColorDepth == HDMITX_VIDEO_CD_48Bits)
    {
        MApi_DAC_DumpTable(DacTbl, DAC_TABTYPE_INIT_HDMITX_16BIT_Divider);
        MApi_DAC_DumpTable(DacTbl, DAC_TABTYPE_INIT_HDMITX_16BIT);
    }
    else
    {
        //Default HDMI TX color depth : 8-bits mode
        MApi_DAC_DumpTable(DacTbl, DAC_TABTYPE_INIT_HDMITX_8BIT_Divider);
        MApi_DAC_DumpTable(DacTbl, DAC_TABTYPE_INIT_HDMITX_8BIT);
    }
#else  // old chip
    DAC_TAB_INFO DacTbl;
    memset(&DacTbl, 0, sizeof(DAC_TAB_INFO));
    DacTbl = DACMAP_Main[enDACIndex];
    MApi_DAC_DumpTable(DacTbl.pDacINITTab, DacTbl.eINITtype);
    MApi_DAC_DumpTable(DacTbl.pDacINIT_SCTab, DacTbl.eINIT_SCtype);
    MApi_DAC_DumpTable(DacTbl.pDacINIT_HDGENTab, DacTbl.eINIT_HDGENtype);
    if(eEDIDColorDepth == HDMITX_VIDEO_CD_30Bits)
    {
        MApi_DAC_DumpTable(DacTbl.pDacINIT_HDMITX_10BIT_DividerTab, DacTbl.eINIT_HDMITX_10BIT_Dividertype);
        MApi_DAC_DumpTable(DacTbl.pDacINIT_HDMITX_10BITTab, DacTbl.eINIT_HDMITX_10BITtype);
    }
    else if(eEDIDColorDepth == HDMITX_VIDEO_CD_36Bits)
    {
        MApi_DAC_DumpTable(DacTbl.pDacINIT_HDMITX_12BIT_DividerTab, DacTbl.eINIT_HDMITX_12BIT_Dividertype);
        MApi_DAC_DumpTable(DacTbl.pDacINIT_HDMITX_12BITTab, DacTbl.eINIT_HDMITX_12BITtype);
    }
    else if(eEDIDColorDepth == HDMITX_VIDEO_CD_48Bits)
    {
        MApi_DAC_DumpTable(DacTbl.pDacINIT_HDMITX_16BIT_DividerTab, DacTbl.eINIT_HDMITX_16BIT_Dividertype);
        MApi_DAC_DumpTable(DacTbl.pDacINIT_HDMITX_16BITTab, DacTbl.eINIT_HDMITX_16BITtype);
    }
    else
    {
        MApi_DAC_DumpTable(DacTbl.pDacINIT_HDMITX_8BIT_DividerTab, DacTbl.eINIT_HDMITX_8BIT_Dividertype);
        MApi_DAC_DumpTable(DacTbl.pDacINIT_HDMITX_8BITTab, DacTbl.eINIT_HDMITX_8BITtype);
    }
#endif
}

void XC_Init(PANEL_RESOLUTION_TYPE enPanelType, E_OUTPUT_VIDEO_TIMING_TYPE enDACIndex)
{
    UBOOT_TRACE("IN\n");

    //1. XC Init
#ifdef CONFIG_HDMI_OLDCHIP
    #if defined(__ARM__)
        *(volatile unsigned int*)0x1F205E00 = 0x000F;
        *(volatile unsigned int*)0x1F205F18 = 0x0020;
    #else
        *(volatile unsigned int*)0xBF205E00 = 0x000F;
        *(volatile unsigned int*)0xBF205F18 = 0x0020;
    #endif
#else
    MApi_XC_SetOSD2VEMode(E_VOP_SEL_OSD_LAST);
#endif

    //2. XC Ex Init
#ifndef CONFIG_HDMI_OLDCHIP
    MS_PHYADDR XC1Addr = 0;
    MS_PHYADDR XC1Size = 0;
    if(get_map_addr_from_env(E_XC1_MAIN_FB, NO_DEFAULT_MMAP_VALUE, &XC1Addr) != 0)
    {
        UBOOT_ERROR("get %s env fail\n",E_MMAP_ID_XC1_MAIN_FB_ADR);
        return;
    }
    if(get_map_size_from_env(E_XC1_MAIN_FB, NO_DEFAULT_MMAP_VALUE, &XC1Size) != 0)
    {
        UBOOT_ERROR("get E_MMAP_ID_XC1_MAIN_FB_ADR env fail\n");
        return;
    }

    E_OUTPUT_VIDEO_TIMING_TYPE enXc1DACIndex = E_RES_720x480I_60Hz;
    XC_DEVICE_ID stXC_DeviceId = {0, 1};
    PNL_DeviceId stPNL_DeviceId = {0, 1};
    PNL_DeviceId stPNL_DeviceId_SC0 = {0, 0};
    XC_EX_INITDATA stXC_InitData;
    MS_S16 s16PathId = 0;
    PANEL_RESOLUTION_TYPE stExPanelType;
    XC_EX_MUX_PATH_INFO PathInfo;
    XC_EX_SETWIN_INFO stXC_SetWin_Info;

    stExPanelType = InitializeEx(enDACIndex);
    if(stExPanelType == DACOUT_480I)
    {
        enXc1DACIndex  = E_RES_720x480I_60Hz;
        UBOOT_DEBUG("---------XC1 DAC = DAC_PANEL_480I_60 \n");
    }
    else
    {
        enXc1DACIndex  = E_RES_720x576I_50Hz;
        UBOOT_DEBUG("---------XC1 DAC = DAC_PANEL_576I_50 \n");
    }
    MApi_PNL_EX_Init(&stPNL_DeviceId, (PNL_EX_PanelType *) MApi_GetPanelSpec(stExPanelType) ); // NEED MODIFY
    UBOOT_DEBUG("---------Width= 0x%x \n",g_IPanelEx.Width(&stPNL_DeviceId));
    UBOOT_DEBUG("---------Height= 0x%x \n",g_IPanelEx.Height(&stPNL_DeviceId));

    MApi_DAC_SetYPbPrOutputTiming(enXc1DACIndex);
#if !defined(CONFIG_K6LITE)
    MS_U8 *DacTbl=NULL;
    MApi_DAC_DumpTable(DacTbl, DAC_TABTYPE_SC1_INIT);
    MApi_DAC_DumpTable(DacTbl, DAC_TABTYPE_SC1_INIT_SC);
#else  // old chip
    DAC_TAB_INFO DacTbl;
    memset(&DacTbl, 0, sizeof(DAC_TAB_INFO));
    MApi_DAC_DumpTable(DacTbl.pDacSC1_INITTab, DacTbl.eSC1_INITtype);
    MApi_DAC_DumpTable(DacTbl.pDacSC1_INIT_SCTab, DacTbl.eSC1_INIT_SCtype);
#endif

    memset(&stXC_InitData, 0, sizeof(XC_EX_INITDATA));
    stXC_InitData.u32XTAL_Clock = 12000000UL;
    stXC_InitData.u32Main_FB_Size = XC1Size; // NEED MODIFY
    stXC_InitData.u32Main_FB_Start_Addr = XC1Addr; // NEED MODIFY
    stXC_InitData.u32Sub_FB_Size = 0;
    stXC_InitData.u32Sub_FB_Start_Addr = 0;
    stXC_InitData.bIsShareGround = FALSE;
    stXC_InitData.bCEC_Use_Interrupt = FALSE;
    stXC_InitData.bEnableIPAutoCoast = XC_PCMONITOR_EX_ENABLE_IP_AUTO_COAST;
    stXC_InitData.bMirror = FALSE;
    stXC_InitData.stPanelInfo.u16HStart = g_IPanelEx.HStart(&stPNL_DeviceId);
    stXC_InitData.stPanelInfo.u16VStart = g_IPanelEx.VStart(&stPNL_DeviceId);
    stXC_InitData.stPanelInfo.u16Width  = g_IPanelEx.Width(&stPNL_DeviceId);
    stXC_InitData.stPanelInfo.u16Height = g_IPanelEx.Height(&stPNL_DeviceId);
    stXC_InitData.stPanelInfo.u16HTotal = g_IPanelEx.HTotal(&stPNL_DeviceId);
    stXC_InitData.stPanelInfo.u16VTotal = g_IPanelEx.VTotal(&stPNL_DeviceId);
    stXC_InitData.stPanelInfo.u16DefaultVFreq = g_IPanelEx.DefaultVFreq(&stPNL_DeviceId);
    stXC_InitData.stPanelInfo.u8LPLL_Mode = g_IPanelEx.LPLL_Mode(&stPNL_DeviceId);
    stXC_InitData.stPanelInfo.enPnl_Out_Timing_Mode = E_XC_EX_PNL_CHG_VTOTAL;
    stXC_InitData.stPanelInfo.u16DefaultHTotal = g_IPanelEx.HTotal(&stPNL_DeviceId);
    stXC_InitData.stPanelInfo.u16DefaultVTotal = g_IPanelEx.VTotal(&stPNL_DeviceId);
    stXC_InitData.stPanelInfo.u32MinSET = g_IPanelEx.MinSET(&stPNL_DeviceId);
    stXC_InitData.stPanelInfo.u32MaxSET = g_IPanelEx.MaxSET(&stPNL_DeviceId);
    stXC_InitData.stPanelInfo.eLPLL_Type = (XC_EX_PNL_LPLL_TYPE)g_IPanelEx.LPLL_Type(&stPNL_DeviceId);
    stXC_InitData.bDLC_Histogram_From_VBlank = FALSE;
    //  MApi_XC_EX_SkipSWReset(&stXC_DeviceId, FALSE);
    if(MApi_XC_EX_Init(&stXC_DeviceId, &stXC_InitData, sizeof(XC_EX_INITDATA)) == FALSE)
    {
        UBOOT_ERROR("XC_Init failed!\n");
    }
    MApi_XC_EX_SkipSWReset(&stXC_DeviceId, TRUE);
    MApi_XC_EX_SetFrameColor(&stXC_DeviceId, 0x00);
    MApi_XC_EX_SetDispWindowColor(&stXC_DeviceId, 0x82, E_XC_EX_MAIN_WINDOW);
    //MApi_XC_EX_GenerateBlackVideo(&stXC_DeviceId, TRUE, E_XC_EX_MAIN_WINDOW);
    //MApi_XC_EX_set_FD_Mask_byWin(&stXC_DeviceId, TRUE, E_XC_EX_MAIN_WINDOW);
    //MApi_XC_EX_EnableFrameBufferLess(&stXC_DeviceId, FALSE);
    MApi_XC_EX_Mux_Init(&stXC_DeviceId, _XC_InputSource_InputPort_Mapping);

    memset(&PathInfo, 0, sizeof(XC_EX_MUX_PATH_INFO));
    PathInfo.Path_Type = E_XC_EX_PATH_TYPE_SYNCHRONOUS;
    PathInfo.src = E_XC_EX_INPUT_SOURCE_SCALER_OP;
    PathInfo.dest = E_XC_EX_OUTPUT_SCALER_MAIN_WINDOW;
    PathInfo.SyncEventHandler = NULL;
    PathInfo.DestOnOff_Event_Handler = NULL;
    PathInfo.path_thread = NULL;
    PathInfo.dest_periodic_handler = NULL;
    s16PathId = MApi_XC_EX_Mux_CreatePath(&stXC_DeviceId, &PathInfo, sizeof(XC_EX_MUX_PATH_INFO));
    if (s16PathId == -1)
    {
        UBOOT_ERROR("Create path fail!\n");
    }
    else
    {
        MApi_XC_EX_Mux_EnablePath(&stXC_DeviceId, (MS_U16)s16PathId);
    }
    MApi_XC_EX_SetInputSource(&stXC_DeviceId, E_XC_EX_INPUT_SOURCE_SCALER_OP, E_XC_EX_MAIN_WINDOW);
    MApi_XC_EX_DisableInputSource(&stXC_DeviceId, FALSE, E_XC_EX_MAIN_WINDOW);

    PanelType* pPNL_PanelType = MApi_GetPanelSpec(enPanelType);
    switch(enPanelType)
    {
        case DACOUT_480I:
            pPNL_PanelType = MApi_GetPanelSpec(DACOUT_480P);
            break;
        case DACOUT_576I:
            pPNL_PanelType = MApi_GetPanelSpec(DACOUT_576P);
            break;
        case DACOUT_1080I_50:
            pPNL_PanelType = MApi_GetPanelSpec(DACOUT_1080P_50);
            break;
        case DACOUT_1080I_60:
            pPNL_PanelType = MApi_GetPanelSpec(DACOUT_1080P_60);
            break;
        default:
            break;
    }

    memset(&stXC_SetWin_Info, 0, sizeof(XC_EX_SETWIN_INFO));
    stXC_SetWin_Info.enInputSourceType = E_XC_EX_INPUT_SOURCE_SCALER_OP;
    stXC_SetWin_Info.stCapWin.y = pPNL_PanelType->m_ucPanelVBackPorch - 2;
    stXC_SetWin_Info.stCapWin.x = (g_IPanelEx.HStart(&stPNL_DeviceId_SC0) - g_IPanelEx.HSynWidth(&stPNL_DeviceId_SC0)) - 1;
    stXC_SetWin_Info.stCapWin.width = g_IPanelEx.Width(&stPNL_DeviceId_SC0);
    stXC_SetWin_Info.stCapWin.height = g_IPanelEx.Height(&stPNL_DeviceId_SC0);
    stXC_SetWin_Info.bHDuplicate = FALSE;
    stXC_SetWin_Info.u16InputVTotal = g_IPanelEx.HTotal(&stPNL_DeviceId_SC0);
    stXC_SetWin_Info.u16DefaultHtotal = g_IPanelEx.VTotal(&stPNL_DeviceId_SC0);
    stXC_SetWin_Info.bInterlace = FALSE;
    stXC_SetWin_Info.u16InputVFreq = g_IPanelEx.DefaultVFreq(&stPNL_DeviceId_SC0);
    stXC_SetWin_Info.stCropWin.x = stXC_SetWin_Info.stCapWin.x;
    stXC_SetWin_Info.stCropWin.y = stXC_SetWin_Info.stCapWin.y;
    stXC_SetWin_Info.stCropWin.width = stXC_SetWin_Info.stCapWin.width;
    stXC_SetWin_Info.stCropWin.height= stXC_SetWin_Info.stCapWin.height;
    stXC_SetWin_Info.stDispWin.x = 0;
    stXC_SetWin_Info.stDispWin.y = 0;
    stXC_SetWin_Info.stDispWin.width = g_IPanelEx.Width(&stPNL_DeviceId);
    stXC_SetWin_Info.stDispWin.height = g_IPanelEx.Height(&stPNL_DeviceId);
    MApi_XC_EX_SkipWaitVsync(&stXC_DeviceId, E_XC_EX_MAIN_WINDOW, TRUE);
    //MApi_XC_SetDbgLevel(0x81);
    if(MApi_XC_EX_SetWindow(&stXC_DeviceId, &(stXC_SetWin_Info), sizeof(XC_EX_SETWIN_INFO), E_XC_EX_MAIN_WINDOW) == FALSE)
    {
        UBOOT_ERROR("Set window failed!\n");
    }
    MApi_XC_EX_SkipWaitVsync(&stXC_DeviceId, E_XC_EX_MAIN_WINDOW, FALSE);
    //MApi_XC_EX_WaitOutputVSync(&stXC_DeviceId, 2, 50, E_XC_EX_MAIN_WINDOW);
    MApi_XC_EX_GenerateBlackVideo(&stXC_DeviceId, FALSE, E_XC_EX_MAIN_WINDOW);
#endif
    UBOOT_TRACE("OK\n");
}

void VE_Init(PANEL_RESOLUTION_TYPE enPanelType, E_OUTPUT_VIDEO_TIMING_TYPE enDACIndex)
{
    UBOOT_TRACE("IN\n");
    MS_VE_WINDOW_TYPE stVEWin = {0, 0, 720, 576};
    MS_VE_VIDEOSYS eVEStd = MS_VE_PAL;
#ifdef CONFIG_HDMI_OLDCHIP
    MS_U8 u8TvFormat;
    char * p_tvstr = NULL;
    p_tvstr = getenv ("tv_format");
    if(NULL != p_tvstr)
    {
        u8TvFormat = (int)simple_strtol(p_tvstr, NULL, 10);
        if(u8TvFormat==0)
        {
            eVEStd = MS_VE_NTSC;
        }
        else
        {
            eVEStd = MS_VE_PAL;
        }
    }
    else
    {
        switch(enPanelType)
        {
            case DACOUT_480I:
            case DACOUT_480P:
            case DACOUT_720P_60:
            case DACOUT_1080I_60:
            case DACOUT_1080P_60:
            {
                eVEStd = MS_VE_NTSC;
            }
            break;
            default:
            {
                eVEStd = MS_VE_PAL;
            }
            break;
        }
    }
#else
    PANEL_RESOLUTION_TYPE stExPanelType;
    stExPanelType = InitializeEx(enDACIndex);
    if(stExPanelType == DACOUT_480I)
    {
        eVEStd = MS_VE_NTSC;
    }
    else
    {
        eVEStd = MS_VE_PAL;
    }
#endif

    MS_PHYADDR VEAddr = 0;
    MS_PHYADDR VESize = 0;
    if(get_map_addr_from_env(E_VE, MMAP_E_MMAP_ID_VE_ADDR + VE_FRAMEBUFFER_OFFSET, &VEAddr) != 0)
    {
        UBOOT_ERROR("get %s env fail\n",E_MMAP_ID_VE_ADR);
        return;
    }
    if(get_map_size_from_env(E_VE, MMAP_E_MMAP_ID_VE_SIZE, &VESize) != 0)
    {
        UBOOT_ERROR("get E_MMAP_ID_VE_LEN env fail\n");
        return;
    }

    if(eVEStd >= MS_VE_PAL_N)
    {
        //shift 8 pixels in PAL mode
        UBOOT_INFO("shift %d pixels in PAL mode \n", VE_WRITE_ADDR_ADJUSTMENT);
        MDrv_VE_AdjustPositionBase(VE_WRITE_ADDR_ADJUSTMENT, VE_READ_ADDR_ADJUSTMENT);
    }
    else
    {
        //shift 0 pixels in NTSC mode
        UBOOT_INFO("shift 0 pixels in NTSC mode \n");
        MDrv_VE_AdjustPositionBase(0, VE_READ_ADDR_ADJUSTMENT);
    }

    MDrv_VE_Init((MS_PHYADDR) (VEAddr + VE_FRAMEBUFFER_OFFSET));

    MDrv_VE_SetOSD(TRUE);
    MDrv_VE_SetOutputVideoStd(eVEStd);
    MDrv_VE_set_display_window(stVEWin);

    //1. VE configuration
    MS_Switch_VE_Src_Info SwitchInputSrc;
    SwitchInputSrc.InputSrcType = MS_VE_SRC_SCALER;
    MDrv_VE_SwitchInputSource(&SwitchInputSrc);

    MS_Switch_VE_Dest_Info SwitchOutputDest;
    SwitchOutputDest.OutputDstType = MS_VE_DEST_CVBS;
    MDrv_VE_SwitchOuputDest(&SwitchOutputDest);

    MS_VE_Set_Mode_Type SetModeType;
    SetModeType.u16H_CapSize     = g_IPanel.Width();
    SetModeType.u16V_CapSize     = g_IPanel.Height();
    SetModeType.u16H_CapStart    = g_IPanel.HStart();
    SetModeType.u16V_CapStart    = g_IPanel.VStart();
    SetModeType.u16H_SC_CapSize  = g_IPanel.Width();
    SetModeType.u16V_SC_CapSize  = g_IPanel.Height();
    SetModeType.u16H_SC_CapStart = g_IPanel.HStart();
    SetModeType.u16V_SC_CapStart = g_IPanel.VStart();
    SetModeType.u16InputVFreq = 2500*2/10;
    SetModeType.bSrcInterlace = FALSE;
    MDrv_VE_SetMode(&SetModeType);

    MS_VE_Output_Ctrl OutputCtrl;
    OutputCtrl.bEnable    = TRUE;
    OutputCtrl.OutputType = MS_VE_OUT_TVENCODER;
    MDrv_VE_SetOutputCtrl(&OutputCtrl);
    //printf("[%s][%d] VE setting done, system time = %lu \n", __FUNCTION__, __LINE__, MsOS_GetSystemTime());
    UBOOT_TRACE("OK\n");
}

void msHdmitx_Post_Init(PANEL_RESOLUTION_TYPE enPanelType, E_OUTPUT_VIDEO_TIMING_TYPE* pDACIndex)
{
    UBOOT_TRACE("IN\n");
    //1. Get hdmi Mode
    HDMITX_OUTPUT_MODE    eOutMode = GetHdmiMode();

    //2. Set color fmt&depth
    HDMITX_VIDEO_COLOR_FORMAT eOutColorFmt = HDMITX_VIDEO_COLOR_RGB444;
    HDMITX_VIDEO_COLOR_FORMAT eInColorFmt  = HDMITX_VIDEO_COLOR_YUV444;
    HDMITX_VIDEO_COLORDEPTH_VAL eEDIDColorDepth = HDMITX_VIDEO_CD_NoID;
    if(TRUE == GetHdmitxColorInfo(eOutMode, (MS_U8 *)&eEDIDColorDepth, (MS_U8 *)&eOutColorFmt, getHDMITxVideoTiming(enPanelType)))
    {
        UBOOT_DEBUG("Get hdmitx colorfmt&colordepth from env.\n");
    }
    else
    {
        UBOOT_DEBUG("Use auto setting colorfmt&colordepth.\n");
    }

    GetInColorFmt(&eInColorFmt, &eOutColorFmt);
    UBOOT_DEBUG("InColor:%d, OutColor:%d, ColorDepth:%d\n", eInColorFmt, eOutColorFmt, eEDIDColorDepth);
    MApi_HDMITx_SetHDMITxMode_CD(eOutMode, eEDIDColorDepth);

    //3. Set HDCP
    if(setHdcpKey(&eOutMode) == TRUE)
    {
        printf("HDMITxMode = %d\n", eOutMode);
        MApi_HDMITx_SetHDMITxMode(eOutMode);
    }

    MApi_HDMITx_SetColorFormat(eInColorFmt, eOutColorFmt);

    //4. Set Audio
    MApi_HDMITx_SetAudioOnOff(TRUE);
    MApi_HDMITx_SetAudioConfiguration(HDMITX_AUDIO_48K, HDMITX_AUDIO_CH_2, HDMITX_AUDIO_PCM);

    //5. Dactbl Init
    if(eOutColorFmt == HDMITX_VIDEO_COLOR_YUV420)
    {
        switch(enPanelType)
        {
            case DACOUT_3840x2160P_30:
                *pDACIndex = E_RES_3840x2160P420_30Hz;
                break;
            case DACOUT_3840x2160P_50:
                *pDACIndex = E_RES_3840x2160P420_50Hz;
                break;
            case DACOUT_3840x2160P_60:
                *pDACIndex = E_RES_3840x2160P420_60Hz;
                break;
            case DACOUT_4096x2160P_30:
                *pDACIndex = E_RES_4096x2160P420_30Hz;
                break;
            case DACOUT_4096x2160P_50:
                *pDACIndex = E_RES_4096x2160P420_50Hz;
                break;
            case DACOUT_4096x2160P_60:
                *pDACIndex = E_RES_4096x2160P420_60Hz;
                break;
            default:
                break;
        }
    }
    DacTbl_Init(*pDACIndex, eEDIDColorDepth);

    //6. Set timing
    HDMITX_VIDEO_TIMING enHDMITxVideoTiming = getHDMITxVideoTiming(enPanelType);
    MApi_HDMITx_SetVideoOutputTiming(enHDMITxVideoTiming);
    setHDMITxAnalogTuning(enPanelType);

    if(enHDMITxVideoTiming <= HDMITX_RES_720x576p)
    {
        MApi_HDMITx_SetVideoOutputOverscan_AFD_II(ENABLE, E_HDMITX_VIDEO_SI_NoData, E_HDMITX_VIDEO_AFD_4_3_Center, 0x1);
    }
    else
    {
        MApi_HDMITx_SetVideoOutputOverscan_AFD_II(ENABLE, E_HDMITX_VIDEO_SI_NoData, E_HDMITX_VIDEO_AFD_16_9_Center, 0x1);
    }

    //7. Send Av Info Pkt.
    switch(enHDMITxVideoTiming)
    {
        case HDMITX_RES_3840x2160p_24Hz:
            MApi_HDMITx_PKT_User_Define(HDMITX_VS_INFOFRAME, FALSE, HDMITX_CYCLIC_PACKET, 0x0);
            MApi_HDMITx_Set_VS_InfoFrame(HDMITX_VIDEO_VS_4k_2k, HDMITx_VIDEO_3D_Not_in_Use, HDMITX_VIDEO_4k2k_24Hz);
            UBOOT_DEBUG("Set_VS_InfoFrame HDMITX_VIDEO_4k2k_24Hz \n");
            break;
        case HDMITX_RES_3840x2160p_25Hz:
            MApi_HDMITx_PKT_User_Define(HDMITX_VS_INFOFRAME, FALSE, HDMITX_CYCLIC_PACKET, 0x0);
            MApi_HDMITx_Set_VS_InfoFrame(HDMITX_VIDEO_VS_4k_2k, HDMITx_VIDEO_3D_Not_in_Use, HDMITX_VIDEO_4k2k_25Hz);
            UBOOT_DEBUG("Set_VS_InfoFrame HDMITX_VIDEO_4k2k_25Hz \n");
            break;
        case HDMITX_RES_3840x2160p_30Hz:
            MApi_HDMITx_PKT_User_Define(HDMITX_VS_INFOFRAME, FALSE, HDMITX_CYCLIC_PACKET, 0x0);
            MApi_HDMITx_Set_VS_InfoFrame(HDMITX_VIDEO_VS_4k_2k, HDMITx_VIDEO_3D_Not_in_Use, HDMITX_VIDEO_4k2k_30Hz);
            UBOOT_DEBUG("Set_VS_InfoFrame HDMITX_VIDEO_4k2k_30Hz \n");
            break;
        case HDMITX_RES_4096x2160p_24Hz:
            MApi_HDMITx_PKT_User_Define(HDMITX_VS_INFOFRAME, FALSE, HDMITX_CYCLIC_PACKET, 0x0);
            MApi_HDMITx_Set_VS_InfoFrame(HDMITX_VIDEO_VS_4k_2k, HDMITx_VIDEO_3D_Not_in_Use, HDMITX_VIDEO_4k2k_24Hz);
            UBOOT_DEBUG("Set_VS_InfoFrame HDMITX_VIDEO_4k2k_24Hz \n");
            break;
        case HDMITX_RES_4096x2160p_25Hz:
            MApi_HDMITx_PKT_User_Define(HDMITX_VS_INFOFRAME, FALSE, HDMITX_CYCLIC_PACKET, 0x0);
            MApi_HDMITx_Set_VS_InfoFrame(HDMITX_VIDEO_VS_4k_2k, HDMITx_VIDEO_3D_Not_in_Use, HDMITX_VIDEO_4k2k_25Hz);
            UBOOT_DEBUG("Set_VS_InfoFrame HDMITX_VIDEO_4k2k_25Hz \n");
            break;
        case HDMITX_RES_4096x2160p_30Hz:
            MApi_HDMITx_PKT_User_Define(HDMITX_VS_INFOFRAME, FALSE, HDMITX_CYCLIC_PACKET, 0x0);
            MApi_HDMITx_Set_VS_InfoFrame(HDMITX_VIDEO_VS_4k_2k, HDMITx_VIDEO_3D_Not_in_Use, HDMITX_VIDEO_4k2k_30Hz);
            UBOOT_DEBUG("Set_VS_InfoFrame HDMITX_VIDEO_4k2k_30Hz \n");
            break;
        default:
            MApi_HDMITx_PKT_User_Define(HDMITX_VS_INFOFRAME, FALSE, HDMITX_STOP_PACKET, 0x0);
            break;
    }

    //8. Send Hdr Info Pkt.
#if (CONFIG_DISPLAY_LOGO_SEAMLESS == 1)
    HDRInfo_Init(eOutColorFmt);
#endif
UBOOT_TRACE("OK\n");
}

void msHdmitx_Str_Init(PANEL_RESOLUTION_TYPE enPanelType, E_OUTPUT_VIDEO_TIMING_TYPE enDACIndex)
{
    UBOOT_TRACE("IN\n");
    #ifdef CONFIG_HDMITX_REG_RESET
    MsHDMITX_INFO stHdmitxInfo;
    memset(&stHdmitxInfo, 0, sizeof(MsHDMITX_INFO));

    if(TRUE == GetHdmitxStatus(&stHdmitxInfo))
    {
        //restore hdmitx suspend reg when str resume and light screen.
        UBOOT_DEBUG("[HDMITX resume parameters]HdmitxMode:%d\n", stHdmitxInfo.eHdmitxMode);
        UBOOT_DEBUG("[HDMITX resume parameters]HdmitxColorDepth:%d\n", stHdmitxInfo.eHdmitxColorDepth);
        UBOOT_DEBUG("[HDMITX resume parameters]HdmitxInColor:%d\n", stHdmitxInfo.eHdmitxOutColor);
        UBOOT_DEBUG("[HDMITX resume parameters]HdmitxOutColor:%d\n", stHdmitxInfo.eHdmitxOutColor);
        UBOOT_DEBUG("[HDMITX resume parameters]HdmitxVideoTiming:%d\n", stHdmitxInfo.eHdmitxVideoTiming);
        UBOOT_DEBUG("[HDMITX resume parameters]AudioChCnt:%d\n", stHdmitxInfo.eAudioChCnt);
        UBOOT_DEBUG("[HDMITX resume parameters]AudioCodeType:%d\n", stHdmitxInfo.eAudioCodeType);
        UBOOT_DEBUG("[HDMITX resume parameters]AudioFreq:%d\n", stHdmitxInfo.eAudioFreq);
        UBOOT_DEBUG("[HDMITX resume parameters]AudioMute:%d\n", stHdmitxInfo.bAudioOn);
        UBOOT_DEBUG("[HDMITX resume parameters]OsdSdr2HdrEnable:%d\n", stHdmitxInfo.u8OsdSdr2HdrEnable);
        UBOOT_DEBUG("[HDMITX resume parameters]SeamlessEnable:%d\n", stHdmitxInfo.u8SeamlessEnable);
        UBOOT_DEBUG("[HDMITX resume parameters]u8ExtendedColorimetry:%d\n", stHdmitxInfo.u8ExtendedColorimetry);
        UBOOT_DEBUG("[HDMITX resume parameters]u8YccQuantizationRange:%d\n", stHdmitxInfo.u8YccQuantizationRange);

        HDMITX_GET_COLOR_FMT stColorFormat;
        HDMITX_GET_COLOR_DEPTH stColorDepth;
        memset(&stColorFormat, 0, sizeof(stColorFormat));
        memset(&stColorDepth, 0, sizeof(stColorDepth));

        //Get ColorFormat and ColorDepth from register set by mboot
        MApi_HDMITx_GeneralCtrl(HDMITX_CMD_GET_COLOR_FORMAT, &stColorFormat, sizeof(stColorFormat));
        MApi_HDMITx_GeneralCtrl(HDMITX_CMD_GET_COLOR_DEPTH, &stColorDepth, sizeof(stColorDepth));

        HDMITX_VIDEO_COLORDEPTH_VAL eEDIDColorDepth = stColorDepth.enColorDepth;
        HDMITX_VIDEO_COLOR_FORMAT eOutColorFmt = stColorFormat.enColorFmt;

        MApi_HDMITx_SetAVMUTE(TRUE);

        if(stHdmitxInfo.eHdmitxOutColor != eOutColorFmt || stHdmitxInfo.eHdmitxColorDepth != eEDIDColorDepth)
        {
            DacTbl_Init(enDACIndex, stHdmitxInfo.eHdmitxColorDepth);
        }

        MApi_HDMITx_SetHDMITxMode_CD(stHdmitxInfo.eHdmitxMode, stHdmitxInfo.eHdmitxColorDepth);
        MApi_HDMITx_SetColorFormat(stHdmitxInfo.eHdmitxInColor, stHdmitxInfo.eHdmitxOutColor);
        MApi_HDMITx_SetVideoOutputTiming(stHdmitxInfo.eHdmitxVideoTiming);
        MApi_HDMITx_SetVideoOnOff(TRUE);
        MApi_HDMITx_SetAudioConfiguration(stHdmitxInfo.eAudioFreq, stHdmitxInfo.eAudioChCnt, stHdmitxInfo.eAudioCodeType);
        //MApi_HDMITx_SetAudioOnOff(stHdmitxInfo.bAudioOn);

        if(stHdmitxInfo.u8OsdSdr2HdrEnable && stHdmitxInfo.u8SeamlessEnable)
        {
            MApi_HDMITx_PKT_Content_Define(HDMITX_HDR_INFOFRMAE, stHdmitxInfo.u8CurHDRInfoFrame, sizeof(stHdmitxInfo.u8CurHDRInfoFrame));
            MApi_HDMITx_PKT_User_Define(HDMITX_HDR_INFOFRMAE, TRUE, HDMITX_CYCLIC_PACKET, 1);
        }

        if((stHdmitxInfo.u8ExtendedColorimetry >= HDMITX_EXT_COLORIMETRY_BT2020CYCC) \
            && (stHdmitxInfo.u8ExtendedColorimetry <= HDMITX_EXT_COLORIMETRY_BT2020RGB))
        {
            MApi_HDMITx_SetAVIInfoExtColorimetry(stHdmitxInfo.u8ExtendedColorimetry, stHdmitxInfo.u8YccQuantizationRange);
            MApi_HDMITx_PKT_User_Define(HDMITX_AVI_INFOFRAME, FALSE, HDMITX_CYCLIC_PACKET, 0);
        }

        MApi_HDMITx_Exhibit();

        MApi_HDMITx_SetTMDSOnOff(TRUE);
        MsOS_DelayTask(20);
        MApi_HDMITx_SetAVMUTE(FALSE);
    }
    else
    {
        UBOOT_ERROR("Get hdmitx suspend status failed.\n");
    }
    #else
        MApi_HDMITx_SetAVMUTE(TRUE);
        MApi_HDMITx_SetTMDSOnOff(TRUE);
    #endif
    UBOOT_TRACE("OK\n");
}

void AutoTimingCheck(MsHDMITX_DEVICEINFO *pstDeviceInfo)
{
    UBOOT_TRACE("IN\n");
    char *p_str;
    char cmd_buf[CMD_BUF] = "\0";
    int resolution_index = DACOUT1080P50;
    int prop_index = DACOUT1080P50;

    if(!isBootToRecovery())
    {
        p_str = getenv ("resolution");
        if(p_str != NULL)
        {
            resolution_index = (int)simple_strtol(p_str, NULL, 10);
        }
        prop_index = CheckSupportTiming(resolution_index, pstDeviceInfo);
        if((prop_index != resolution_index) && (prop_index != 0))
        {
            resolution_index = prop_index;
            printf("select proper resolution=%d from EDID\n", resolution_index);
            printf("select proper panel_name: %s.ini\n", DacPropPanelIndexTbl[resolution_index]);
            snprintf(cmd_buf, sizeof(cmd_buf),"%d", resolution_index);
            setenv("resolution", cmd_buf);
            setenv("resolution_reset", cmd_buf);

            MApi_SetEnv2BootArgs("resolution=", DacPropPanelIndexTbl[resolution_index]);
            snprintf(cmd_buf, sizeof(cmd_buf),"/config/panel/%s.ini", DacPropPanelIndexTbl[resolution_index]);
            setenv("panel_path", cmd_buf);
            setenv("panel_name", DacPropPanelIndexTbl[resolution_index]);

            #if (ENABLE_AUTO_USB_UPGRADE == 1)
            setenv("MstarUpgrade_complete","1");
            #endif

            setenv("db_table", "0");
            saveenv();
            run_command("reset", 0);
        }
    }
}

int msHdmitx_Disp_PreInit(void)
{
    //PreInitialization - DAC and HDMI initialization
    MsHDMITX_DEVICEINFO stDeviceInfo;
    UBOOT_TRACE("IN\n");
    memset(&stDeviceInfo, 0, sizeof(stDeviceInfo));
    MDrv_SYS_Init();
    MApi_HDMITx_SetDbgLevel(0);
    MApi_HDMITx_SetHPDGpioPin(HDMITX_HPD_PM_GPIO);
    MApi_HDMITx_Init();

    // add here for hdmitx ssc enable
#ifdef CONFIG_HDMITX_SSC
    HDMITX_SSCENABLE stSSCEn = {0, 1, 0};
    MApi_HDMITx_GeneralCtrl(HDMITX_CMD_SSC_ENABLE, (void*)&stSSCEn, sizeof(stSSCEn));
#endif

    MApi_DAC_Init();
    MApi_DAC_SetClkInv(TRUE, TRUE);

    char *cvbs_status = getenv(ENV_CVBS_STATUS);
    if (((cvbs_status == NULL) || (strcmp(cvbs_status, "0") == 0)) && (MApi_HDMITx_GetRxStatus() == TRUE))
    {
        MApi_DAC_Enable(FALSE, TRUE);
        MApi_DAC_Enable(FALSE, FALSE);
        UBOOT_DEBUG("Hdmi is connected!\n");
        UBOOT_DEBUG("%s = 0 , Do not enable CVBS\n", ENV_CVBS_STATUS);
    }
    else
    {
        MApi_DAC_Enable(TRUE, TRUE);
        MApi_DAC_Enable(TRUE, FALSE);
    }
    MApi_HDMITx_TurnOnOff(TRUE);
    MApi_HDMITx_SetVideoOnOff(TRUE);

    AutoTimingCheck(&stDeviceInfo);
    UBOOT_TRACE("OK\n");
    return 0;
}

int msHdmitx_Disp_Init(void)
{
    MS_BOOL bRet = FALSE;
    PANEL_RESOLUTION_TYPE enPanelType = DACOUT_1080P_60;
    E_OUTPUT_VIDEO_TIMING_TYPE enDACIndex      = E_RES_1920x1080P_60Hz;
    //XC_INITDATA sXC_InitData;

    UBOOT_TRACE("IN\n");
    printf("msHdmitx_Disp_Init\n");

    //1. hdmi pre init
    msHdmitx_Disp_PreInit();

    //2. XC Init && Get panel type && dacindex from env
    XC_INITDATA sXC_InitData;
    memset(&sXC_InitData, 0, sizeof(XC_INITDATA));
    MApi_XC_Init(&sXC_InitData, sizeof(XC_INITDATA));

    bRet = getResolutionTypeAndDACIndex((MS_U16 *)&enPanelType, (MS_U16 *)&enDACIndex);
    UBOOT_DEBUG("[%s %d] (%u, %u)\n",__FUNCTION__,__LINE__,enPanelType,enDACIndex);

    //3. Post HDMI setting
    msHdmitx_Post_Init(enPanelType, &enDACIndex);

    //4. XC Init
    XC_Init(enPanelType, enDACIndex);

    //5. VE initialization
    VE_Init(enPanelType, enDACIndex);

    //6. HDMI STR setting
    if(is_str_resume())
    {
        msHdmitx_Str_Init(enPanelType, enDACIndex);
    }
    else
    {
        if((FALSE == pm_check_back_ground_active()) && (MApi_HDMITx_GetRxStatus() == TRUE))
        {
            MApi_HDMITx_SetTMDSOnOff(TRUE);
            MsOS_DelayTask(20);
            MApi_HDMITx_SetAVMUTE(FALSE);
        }
        else
        {
            MApi_HDMITx_SetTMDSOnOff(FALSE);
        }
    }

    //7. VE SW Reset here
    MDrv_VE_ReloadSetting();
    UBOOT_TRACE("ON\n");
    //printf("[%s][%d] Function exit, system time = %lu \n", __FUNCTION__, __LINE__, MsOS_GetSystemTime());
    return 0;
}


#if (CONFIG_BOOTVIDEO == 1)
XC_Internal_TimingType msHdmitx_GetEnvOutputTiming(MS_BOOL *pbNtsc)
{
    MS_BOOL bRet;
    MS_BOOL bNtsc = FALSE;
    MS_U16 enPanelType = DACOUT_1080I_50;
    E_OUTPUT_VIDEO_TIMING_TYPE enDACIndex = E_RES_1920x1080I_50Hz;
    XC_Internal_TimingType enXcTimingType = E_XC_1080I;
    bRet = getResolutionTypeAndDACIndex((MS_U16 *)&enPanelType, (MS_U16 *)&enDACIndex);

    if(FALSE == bRet)
    {
        printf("%s: Error: getResolutionTypeAndDACIndex FAILE! use 1080p default, at %d\n", __func__, __LINE__);
        enPanelType = DACOUT_1080I_50;
        enDACIndex  = E_RES_1920x1080I_50Hz;
    }

    switch(enDACIndex)
    {
        case DAC_PANEL_480I_60:
            enXcTimingType = E_XC_480I;
            bNtsc = TRUE;
            break;
        case DAC_PANEL_480P_60:
            enXcTimingType = E_XC_480P;
            bNtsc = TRUE;
            break;
        case DAC_PANEL_576I_50:
            enXcTimingType = E_XC_576I;
            bNtsc = FALSE;
            break;
        case DAC_PANEL_576P_50:
            enXcTimingType = E_XC_576P;
            bNtsc = FALSE;
            break;
        case DAC_PANEL_720P_50:
            enXcTimingType = E_XC_720P;
            bNtsc = FALSE;
            break;
        case DAC_PANEL_720P_60:
            enXcTimingType = E_XC_720I;
            bNtsc = TRUE;
            break;
        case DAC_PANEL_1080I_50:
            enXcTimingType = E_XC_1080I;
            bNtsc = FALSE;
            break;
        case DAC_PANEL_1080I_60:
            enXcTimingType = E_XC_1080I;
            bNtsc = TRUE;
            break;
        case DAC_PANEL_1080P_50:
            enXcTimingType = E_XC_1080P;
            bNtsc = FALSE;
            break;
        case DAC_PANEL_1080P_60:
            enXcTimingType = E_XC_1080P;
            bNtsc = TRUE;
            break;
        default:
            // 1080 50I
            enXcTimingType = E_XC_1080I;
            bNtsc = FALSE;
            break;
    }

    if(pbNtsc)
        *pbNtsc = bNtsc;

    return enXcTimingType;
}
#endif  // #if (CONFIG_BOOTVIDEO == 1)
