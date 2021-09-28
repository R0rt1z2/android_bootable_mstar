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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @brief  Panel Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

/*! \defgroup PNL_MODULE Panel Module

    PNL is used for:

    - 1.Set panel parameters base on different panel type,include VOP, MOD setting
    - 2.change panel types .
    -     Ex:VBY1 switch from 2,4,8 lanes.

    <b> Operation Code Flow: </b> \n
    check flow chart directly.
    \image html apiPNL.png

 *! \defgroup PNL_INTERFACE_INIT Panel Init Control Interface
 *  \ingroup PNL_MODULE

 *! \defgroup PNL_INTERFACE_FEATURE Panel Feature Operations Interface
 *  \ingroup PNL_MODULE

 *! \defgroup PNL_INTERFACE_INFO Panel Infomation Pool Interface
 *  \ingroup PNL_MODULE

 *! \defgroup PNL_INTERFACE_ToBeModified Panel APIs-to-be-modified Interface
 *  \ingroup PNL_MODULE

 *! \defgroup PNL_INTERFACE_ToBeRemove Panel APIs-to-be-removed Interface
 *  \ingroup PNL_MODULE
 */


#ifndef _API_XC_PANEL_H_
#define _API_XC_PANEL_H_

#include "MsDevice.h"
#include "MsVersion.h"
#include "UFO.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _API_XC_PANEL_C_
#define INTERFACE
#else
#define INTERFACE extern
#endif

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  This macro defined in mscommon.h originally, here just for avoid SN compile error
//-------------------------------------------------------------------------------------------------
#ifdef MSOS_TYPE_LINUX_KERNEL
#define SYMBOL_WEAK
#else
#define SYMBOL_WEAK __attribute__((weak))
#endif

#ifndef _MS_VERSION_H_
#define MSIF_TAG                    {'M','S','I','F'}                   // MSIF
#define MSIF_CLASS                  {'0','0'}                           // DRV/API (DDI)
#define MSIF_CUS                    0x0000                              // MStar Common library
#define MSIF_MOD                    0x0000                              // MStar Common library
#define MSIF_CHIP                   0x000B
#define MSIF_CPU                    '0'
#define MSIF_OS                     '2'
#endif

// library information
#define MSIF_PNL_LIB_CODE               {'P','N','L','_'}
#define MSIF_PNL_LIBVER                 {'0','3'}
#define MSIF_PNL_BUILDNUM               {'5','2'}
#define MSIF_PNL_CHANGELIST             {'0','0','6','1','4','4','7','7'}

#define PNL_API_VERSION                 /* Character String for DRV/API version             */  \
    MSIF_TAG,                           /* 'MSIF'    */  \
    MSIF_CLASS,                         /* '00'      */  \
    MSIF_CUS,                           /* 0x0000    */  \
    MSIF_MOD,                           /* 0x0000    */  \
    MSIF_CHIP,                                           \
    MSIF_CPU,                                            \
    MSIF_PNL_LIB_CODE  ,                /* IP__      */  \
    MSIF_PNL_LIBVER    ,                /* 0.0 ~ Z.Z */  \
    MSIF_PNL_BUILDNUM  ,                /* 00 ~ 99   */  \
    MSIF_PNL_CHANGELIST,                /* CL#       */  \
    MSIF_OS

/// ApiStatusEX version of current XC lib
#define API_PNLSTATUS_EX_VERSION                          1

///Mirror info version
#define MIRROR_INFO_VERSION                              (1)

/// ST_PNL_GAMMATBL version
#define ST_PNL_GAMMATBL_VERSION                          (1)

/// ST_PNL_PNLGAMMATBL version
#define ST_PNL_PNLGAMMATBL_VERSION                       (1)
//----------------------------
// Debug Switch
//----------------------------
#define PNL_DBGLEVEL_OFF        (0x0000)            ///< turn off debug message, this is default setting
#define PNL_DBGLEVEL_INIT       (0x0001)            ///< Initial function
#define PNL_DBGLEVEL_PANEL_EN   (0x0002)            ///< panel enable function
#define PNL_DBGLEVEL_SSC        (0x0004)            ///< panel SSC setting
#define PNL_DBGLEVEL_GAMMA      (0x0008)            ///< gamma table setting
#define PNL_DBGLEVEL_CALIBRATION (0x0010)           ///< mod calibration

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
/// Define return value of MApi_PNL
typedef enum
{
    E_APIPNL_FAIL = 0,
    E_APIPNL_OK = 1,
    E_APIPNL_GET_BASEADDR_FAIL,            ///< get base address failed when initialize panel driver
    E_APIPNL_OBTAIN_MUTEX_FAIL,            ///< obtain mutex timeout when calling this function
} APIPNL_Result;

/// Define aspect ratio
typedef enum
{
    E_PNL_ASPECT_RATIO_4_3    = 0,         ///< set aspect ratio to 4 : 3
    E_PNL_ASPECT_RATIO_WIDE,               ///< set aspect ratio to 16 : 9
    E_PNL_ASPECT_RATIO_OTHER,              ///< resvered for other aspect ratio other than 4:3/ 16:9
}E_PNL_ASPECT_RATIO;

/// Define the panel gamma precision type
typedef enum
{
    E_APIPNL_GAMMA_10BIT = 0,              ///< Gamma Type of 10bit
    E_APIPNL_GAMMA_12BIT,                  ///< Gamma Type of 12bit
    E_APIPNL_GAMMA_ALL                     ///< The library can support all mapping mode
} APIPNL_GAMMA_TYPE;

/// Define the panel gamma bin entry
typedef enum
{
    E_PNL_PNLGAMMA_256_ENTRIES = 0,              ///< Indicate PNL Gamma is 256 entrise
    E_PNL_PNLGAMMA_1024_ENTRIES,                 ///< Indicate PNL Gamma is 1024 entries
    E_PNL_PNLGAMMA_MAX_ENTRIES                   ///< Indicate PNL Gamma is MAX entries
} EN_PNL_PNLGAMMA_ENTRIES;

/// Define Gamma type
typedef enum
{
    E_APIPNL_GAMMA_8BIT_MAPPING = 0,      ///< mapping 1024 to 256 gamma entries
    E_APIPNL_GAMMA_10BIT_MAPPING,         ///< mapping 1024 to 1024 gamma entries
    E_APIPNL_GAMMA_ALL_MAPPING            ///< the library can map to any entries
} APIPNL_GAMMA_MAPPEING_MODE;             ///< samping mode for GAMMA correction

/// Define The dimming control flag. when use with setter/getter, it will set/get MIN/MAX/Current value
typedef enum
{
    E_APIPNL_DIMMING_MIN = 0,              ///< Indicate to Get/Set Min Dimming value.
    E_APIPNL_DIMMING_CURRENT  ,            ///< Indicate to Get/Set Current Dimming value.
    E_APIPNL_DIMMING_MAX    ,              ///< Indicate to Get/Set Max Dimming value.
} APIPNL_DIMMING_CTRL;

/// Define PANEL Signaling Type
typedef enum
{
    LINK_TTL,                              ///< TTL  type
    LINK_LVDS,                             ///< LVDS type
    LINK_RSDS,                             ///< RSDS type
    LINK_MINILVDS,                         ///< TCON
    LINK_ANALOG_MINILVDS,                  ///< Analog TCON
    LINK_DIGITAL_MINILVDS,                 ///< Digital TCON
    LINK_MFC,                              ///< Ursa (TTL output to Ursa)
    LINK_DAC_I,                            ///< DAC output
    LINK_DAC_P,                            ///< DAC output
    LINK_PDPLVDS,                          ///< For PDP(Vsync use Manually MODE)
    LINK_EXT,                              /// EXT LPLL TYPE
}APIPNL_LINK_TYPE;

/// Define PANEL Signaling Type
typedef enum
{
    // M10 New Panel Type
    LINK_EPI34_8P = LINK_EXT,              /// 10
    LINK_EPI28_8P,                         /// 11
    LINK_EPI34_6P,                         /// 12
    LINK_EPI28_6P,                         /// 13

    ///LINK_MINILVDS_6P_2L,                /// replace this with LINK_MINILVDS
    LINK_MINILVDS_5P_2L,                   /// 14
    LINK_MINILVDS_4P_2L,                   /// 15
    LINK_MINILVDS_3P_2L,                   /// 16
    LINK_MINILVDS_6P_1L,                   /// 17
    LINK_MINILVDS_5P_1L,                   /// 18
    LINK_MINILVDS_4P_1L,                   /// 19
    LINK_MINILVDS_3P_1L,                   /// 20

    LINK_HS_LVDS,                          /// 21
    LINK_HF_LVDS,                          /// 22

    LINK_TTL_TCON,                         /// 23
    LINK_MINILVDS_2CH_3P_8BIT,              //  2 channel, 3 pair, 8 bits ///24
    LINK_MINILVDS_2CH_4P_8BIT,              //  2 channel, 4 pair, 8 bits ///25
    LINK_MINILVDS_2CH_5P_8BIT,              // 2 channel, 5 pair, 8 bits ///26
    LINK_MINILVDS_2CH_6P_8BIT,              // 2 channel, 6 pair, 8 bits ///27

    LINK_MINILVDS_1CH_3P_8BIT,              // 1 channel, 3 pair, 8 bits ///28
    LINK_MINILVDS_1CH_4P_8BIT,              // 1 channel, 4 pair, 8 bits ///29
    LINK_MINILVDS_1CH_5P_8BIT,              // 1 channel, 5 pair, 8 bits ///30
    LINK_MINILVDS_1CH_6P_8BIT,              // 1 channel, 6 pair, 8 bits ///31

    LINK_MINILVDS_2CH_3P_6BIT,              // 2 channel, 3 pari, 6 bits ///32
    LINK_MINILVDS_2CH_4P_6BIT,              // 2 channel, 4 pari, 6 bits ///33
    LINK_MINILVDS_2CH_5P_6BIT,              // 2 channel, 5 pari, 6 bits ///34
    LINK_MINILVDS_2CH_6P_6BIT,              //  2 channel, 6 pari, 6 bits ///35

    LINK_MINILVDS_1CH_3P_6BIT,              // 1 channel, 3 pair, 6 bits ///36
    LINK_MINILVDS_1CH_4P_6BIT,              // 1 channel, 4 pair, 6 bits ///37
    LINK_MINILVDS_1CH_5P_6BIT,              // 1 channel, 5 pair, 6 bits ///38
    LINK_MINILVDS_1CH_6P_6BIT,              // 1 channel, 6 pair, 6 bits ///39
    LINK_HDMI_BYPASS_MODE,                   //   HDMI Bypass Mode///40

    LINK_EPI34_2P,                           /// 41
    LINK_EPI34_4P,                         /// 42
    LINK_EPI28_2P,                         /// 43
    LINK_EPI28_4P,                         /// 44

    LINK_VBY1_10BIT_4LANE,     ///45
    LINK_VBY1_10BIT_2LANE,    ///46
    LINK_VBY1_10BIT_1LANE,    ///47
    LINK_VBY1_8BIT_4LANE,     ///48
    LINK_VBY1_8BIT_2LANE,     ///49
    LINK_VBY1_8BIT_1LANE,     ///50

    LINK_VBY1_10BIT_8LANE,     ///51
    LINK_VBY1_8BIT_8LANE,     ///52

    LINK_EPI28_12P,           ///53

    LINK_HS_LVDS_2CH_BYPASS_MODE,        //54
    LINK_VBY1_8BIT_4LANE_BYPASS_MODE,    //55
    LINK_VBY1_10BIT_4LANE_BYPASS_MODE,   //56
    LINK_EPI24_12P,           ///57
    LINK_VBY1_10BIT_16LANE,   ///58
    LINK_VBY1_8BIT_16LANE,    ///59
    LINK_USI_T_8BIT_12P,      ///60
    LINK_USI_T_10BIT_12P,     ///61
    LINK_ISP_8BIT_12P,        ///62
    LINK_ISP_8BIT_6P_D,       ///63

    LINK_ISP_8BIT_8P,         ///64
    LINK_ISP_10BIT_12P,       ///65
    LINK_ISP_10BIT_6P_D,        ///66
    LINK_ISP_10BIT_8P,        ///67
    LINK_EPI24_16P,           ///68
    LINK_EPI28_16P,           ///69
    LINK_EPI28_6P_EPI3G,      ///70
    LINK_EPI28_8P_EPI3G,      ///71

    LINK_CMPI24_10BIT_12P,    ///72
    LINK_CMPI27_8BIT_12P,     ///73
    LINK_CMPI27_8BIT_8P,      ///74
    LINK_CMPI24_10BIT_8P,     ///75
    LINK_USII_8BIT_12X1_4K,   ///76
    LINK_USII_8BIT_6X1_D_4K,  ///77
    LINK_USII_8BIT_8X1_4K,    ///78
    LINK_USII_8BIT_6X1_4K,    ///79
    LINK_USII_8BIT_6X2_4K,    ///80
    LINK_USII_10BIT_12X1_4K,  ///81
    LINK_USII_10BIT_8X1_4K,   ///82

    LINK_EPI24_8P,              ///83
    LINK_USI_T_8BIT_8P,         ///84
    LINK_USI_T_10BIT_8P,        ///85
}APIPNL_LINK_EXT_TYPE;

/// Define power on and off timing order.
typedef enum
{
    E_APIPNL_POWER_TIMING_1 ,              ///< Timing order 1
    E_APIPNL_POWER_TIMING_2 ,              ///< Timing order 2
    E_APIPNL_POWER_TIMING_NA = 0xFFFF,     ///< Reserved Timing order
} APIPNL_POWER_TIMING_SEQ;

/// Define TI bit mode
typedef enum
{
    TI_10BIT_MODE = 0,
    TI_8BIT_MODE = 2,
    TI_6BIT_MODE = 3,
} APIPNL_TIBITMODE;

/// Define which panel output timing change mode is used to change VFreq for same panel
typedef enum
{
    E_PNL_CHG_DCLK   = 0,      ///<change output DClk to change Vfreq.
    E_PNL_CHG_HTOTAL = 1,      ///<change H total to change Vfreq.
    E_PNL_CHG_VTOTAL = 2,      ///<change V total to change Vfreq.
} APIPNL_OUT_TIMING_MODE;

/// Define panel output format bit mode
typedef enum
{
    OUTPUT_10BIT_MODE = 0,//default is 10bit, becasue 8bit panel can use 10bit config and 8bit config.
    OUTPUT_6BIT_MODE = 1, //but 10bit panel(like PDP panel) can only use 10bit config.
    OUTPUT_8BIT_MODE = 2, //and some PDA panel is 6bit.
} APIPNL_OUTPUTFORMAT_BITMODE;

/// Define Mirror Type
typedef enum
{
    E_PNL_MOD_H_MIRROR = 0,
    E_PNL_MIRROR_TYPE_MAX,
} EN_PNL_MIRROR_TYPE;

typedef struct DLL_PACKED
{
    MS_U32 u32Version;   ///<Version of current structure. Please always set to "MIRROR_INFO_VERSION" as input
    MS_U32 u32Length;    ///<Length of this structure, u32MirrorInfo_Length=sizeof(ST_APIPNL_MIRRORINFO)
    EN_PNL_MIRROR_TYPE eMirrorType;   ///< Mirror type
    MS_BOOL bEnable;
} ST_PNL_MIRRORINFO;

/// Panel Api information
typedef struct __attribute__((packed))
{
    APIPNL_GAMMA_TYPE eSupportGammaType;   ///< Gamma type supported by apiPNL
} PNL_ApiInfo;

/// Panel status
typedef struct __attribute__((packed))
{
    MS_BOOL bPanel_Initialized;     ///< panel initialized or not
    MS_BOOL bPanel_Enabled;         ///< panel enabled or not, if enabled, you can see OSD/Video
} PNL_ApiStatus;

/// Panel status
#define PNL_TCON_NAME_LENGTH 16
typedef struct DLL_PACKED
{
    MS_U32 u32ApiStatusEx_Version;///<Version of current structure. Please always set to "API_PNLSTATUS_EX_VERSION" as input
    MS_U16 u16ApiStatusEX_Length; ///<Length of this structure, u16PanelInfoEX_Length=sizeof(XC_PANEL_INFO_EX)

    MS_BOOL bPNLInitialize;       ///< panel initialized or not
    MS_BOOL bPNLEnable;           ///< panel enabled or not, if enabled, you can see OSD/Video
    MS_U16  u16VTotal;            ///< Output vertical total
    MS_U16  u16DEVStart;          ///< Output DE vertical start
    MS_U16  u16DEVEnd;            ///< Output DE Vertical end
    MS_U16  u16VSyncStart;        ///< Output VSync start
    MS_U16  u16VSyncEnd;          ///< Output VSync end
    MS_U16  u16HTotal;            ///< Output horizontal total
    MS_U16  u16DEHStart;          ///< Output DE horizontal start
    MS_U16  u16DEHEnd;            ///< Output DE horizontal end
    MS_U16  u16HSyncWidth;        ///< Output HSync width
    MS_BOOL bIsPanelManualVysncMode;   ///< enable manuel V sync control
    MS_BOOL bInterlaceOutput;     ///< enable Scaler Interlace output
    MS_BOOL bYUVOutput;           ///< enable Scaler YUV outputi

    MS_U8 u8TCON_MajorVersion;                        ///< TCON Major Version
    MS_U8 u8TCON_MinorVersion;                        ///< TCON Minor Version
    MS_U8 u8TCON_PanelInterface;                      ///< TCON Panel Interface
    MS_U8 u8TCON_PanelSize;                           ///< TCON Panel Size
    MS_U16 u16TCON_Year;                              ///< TCON INF Year
    MS_U8 u8TCON_Month;                               ///< TCON INF Month
    MS_U8 u8TCON_Day;                                 ///< TCON INF Day
    MS_U8 u8TCON_ProjectName[PNL_TCON_NAME_LENGTH];   ///< TCON Project Name
    MS_U8 u8TCON_CustomerName[PNL_TCON_NAME_LENGTH];  ///< TCON CustomerName
    MS_BOOL bIsTCONBinEn;                             ///< TCON EN

    MS_U32 u32TCON_PanelLinkType;                     ///< TCON PanelLinkType
    MS_U32 u32TCON_PanelLinkExtType;                  ///< TCON PanelLinkExtType
    MS_U32 u32TCON_PanelMaxSET;                       ///< TCON PanelMaxSet
    MS_U32 u32TCON_PanelMinSET;                       ///< TCON PanelMinSet

    MS_BOOL bTCONSCEnable;                            ///< TCON SSC Enable
    MS_U32 u32TCONSSCFmodulation;                    ///< TCON SSC Fmodulation
    MS_U32 u32TCONSSCPercentage;                     ///< TCON SSC Percentage
} PNL_ApiExtStatus;

/// Panel output control, must be called before g_IPanel.Enable(), otherwise will output after called g_IPanel.Enable()
typedef enum
{
    E_APIPNL_OUTPUT_NO_OUTPUT = 0,     ///< even called g_IPanel.Enable(TRUE), still no physical output
    E_APIPNL_OUTPUT_CLK_ONLY,          ///< after called g_IPanel.Enable(TRUE), will output clock only
    E_APIPNL_OUTPUT_DATA_ONLY,         ///< after called g_IPanel.Enable(TRUE), will output data only
    E_APIPNL_OUTPUT_CLK_DATA,          ///< after called g_IPanel.Enable(TRUE), will output clock and data
} APIPNL_OUTPUT_MODE;

/// Define Panel MISC control index
/// please enum use BIT0 = 0x01, BIT1 = 0x02, BIT2 = 0x04, BIT3 = 0x08, BIT4 = 0x10,
typedef enum
{
    E_APIPNL_MISC_CTRL_OFF   = 0x0000,
    E_APIPNL_MISC_MFC_ENABLE = 0x0001,
    E_APIPNL_MISC_SKIP_CALIBRATION = 0x0002,
    E_APIPNL_MISC_GET_OUTPUT_CONFIG = 0x0004,
    E_APIPNL_MISC_SKIP_ICONVALUE = 0x0008,

    E_APIPNL_MISC_MFC_MCP    = 0x0010, // bit 4
    E_APIPNL_MISC_MFC_ABChannel = 0x0020,  // bit5
    E_APIPNL_MISC_MFC_ACChannel = 0x0040,  // bit 6
    E_APIPNL_MISC_MFC_ENABLE_60HZ = 0x0080, // bit 7, for 60Hz Panel
    E_APIPNL_MISC_MFC_ENABLE_240HZ = 0x0100, // bit 8, for 240Hz Panel
    E_APIPNL_MISC_4K2K_ENABLE_60HZ = 0x0200, // bit 9, for 4k2K 60Hz Panel
    E_APIPNL_MISC_SKIP_T3D_CONTROL = 0x0400, // bit 10, for T3D control
    E_APIPNL_MISC_PIXELSHIFT_ENABLE = 0x0800,// bit 11, enable pixel shift
    E_APIPNL_MISC_ENABLE_MANUAL_VSYNC_CTRL = 0x8000, // enable manual V sync control
} APIPNL_MISC;

typedef enum
{
    E_APIPNL_TCON_TAB_TYPE_GENERAL,
    E_APIPNL_TCON_TAB_TYPE_GPIO,
    E_APIPNL_TCON_TAB_TYPE_SCALER,
    E_APIPNL_TCON_TAB_TYPE_MOD,
    E_APIPNL_TCON_TAB_TYPE_GAMMA,
    E_APIPNL_TCON_TAB_TYPE_POWER_SEQUENCE_ON,
    E_APIPNL_TCON_TAB_TYPE_POWER_SEQUENCE_OFF,
    E_APIPNL_TCON_TAB_TYPE_PANEL_INFO,
    E_APIPNL_TCON_TAB_TYPE_OVERDRIVER,
    E_APIPNL_TCON_TAB_TYPE_PCID,
}APIPNL_TCON_TAB_TYPE;

typedef struct
{
#if !defined(CONFIG_MBOOT)
    MS_U8  u8SubTableType;
    MS_U16 u16RegCount;
    MS_U8  u8RegType;
    MS_U16 u16RegListOffset;
#endif
    MS_U8* pData;
}ST_PNL_TCON_PANEL;

typedef enum
{
    APIPNL_OUTPUT_CHANNEL_ORDER_DEFAULT,
    APIPNL_OUTPUT_CHANNEL_ORDER_USER,
}APIPNL_OUTPUT_CHANNEL_ORDER;

/// Define PNL local dimming type
typedef enum
{
    E_STATUS_SUCCESS = 0,
    E_STATUS_FAIL = 1,
    E_STATUS_NOT_SUPPORTED,
    E_STATUS_PARAMETER_ERROR,
} E_PNL_LOCALDIMMING_RETURN_TYPE;

typedef enum
{
    E_LED_DEVICE_SEC_75INCH_DIRECT_180 = 0,
    E_LED_DEVICE_SEC_85INCH_DIRECT_240 = 1,
    E_LED_DEVICE_AMT_65INCH_DIRECT_120,
    E_PNL_LOCALDIMMING_TYPE_NUM,
    E_PNL_LOCALDIMMING_TYPE_MAX = E_PNL_LOCALDIMMING_TYPE_NUM,
} E_PNL_LOCALDIMMING_TYPE;

typedef enum
{
    /// BIN
    E_PNL_FILE_TCON_BIN_PATH,
    /// The max support number of paths
    E_PNL_FILE_PATH_MAX,
}E_PNL_FILE_PATH;

typedef struct _stPNL_Init_LocalDimming_Parameters
{
    E_PNL_LOCALDIMMING_RETURN_TYPE ePnlLDType;
    MS_U32 u32Adrress;
    MS_U16 u16Size;
    MS_U16 u16PnlWidth;
    MS_U16 u16PnlHeight;
} stPNL_Init_LocalDimming_Parameters, *pstPNL_Init_LocalDimming_Parameters;

/**
* Represent a panel interface.
*
* Provide panel attributes, and some panel basic functions
*/
typedef struct
{
    //
    //  Data
    //
    const char*        ( * const Name          ) ( void ); // /< Panel name
    MS_U16             ( * const HStart        ) ( void ); // /< DE H start
    MS_U16             ( * const VStart        ) ( void ); // /< DE V start
    MS_U16             ( * const Width         ) ( void ); // /< DE H width
    MS_U16             ( * const Height        ) ( void ); // /< DE V height
    MS_U16             ( * const HTotal        ) ( void ); // /< Htotal
    MS_U16             ( * const VTotal        ) ( void ); // /< Vtotal
    MS_U8              ( * const HSynWidth     ) ( void ); // /< H sync width
    MS_U8              ( * const HSynBackPorch ) ( void ); // /< H sync back porch
    MS_U8              ( * const VSynBackPorch ) ( void ); // /< V sync back porch
    MS_U16             ( * const DefaultVFreq  ) ( void ); // /< deault V Freq
    MS_U8              ( * const LPLL_Mode     ) ( void ); // /< 0: single,      1: dual mode
    MS_U8              ( * const LPLL_Type     ) ( void ); // /< 0: LVDS,        1: RSDS
    E_PNL_ASPECT_RATIO ( * const AspectRatio   ) ( void ); // /< please refer to E_PNL_ASPECT_RATIO
    MS_U32             ( * const MinSET        ) ( void ); // / < MinSET
    MS_U32             ( * const MaxSET        ) ( void );     // / < MaxSET

    //
    //  Manipulation
    //
    /// @brief Set Span-Spectrum-Control
    /// @param u16Fmodulation  IN:SSC_SPAN_PERIOD
    /// @param u16Rdeviation   IN:SSC_STEP_PERCENT
    /// @param bEnable         IN:Enable / Disable
    ///
    void    ( * const SetSSC      ) ( MS_U16 u16Fmodulation, MS_U16 u16Rdeviation, MS_BOOL bEnable ) ;
    void    ( * const SetOSDSSC   ) ( MS_U16 u16Fmodulation, MS_U16 u16Rdeviation, MS_BOOL bEnable ) ;

    /// @brief Enable panel's output, but "not include the function to turn VCC on".
    /// @param bEnable          IN:Enable / Disable
    MS_BOOL ( * const Enable      ) ( MS_BOOL bEnable ) ;

    /// @brief Set Gamma correction table.
    /// @param eGammaType       Resolution of gamma table
    /// @param pu8GammaTab      gamma table
    /// @param u16NumOfLevel    T2: 256, T3: can be 256 / 1024 levels
    MS_BOOL ( * const SetGammaTbl ) ( APIPNL_GAMMA_TYPE eGammaType,
                                      MS_U8* pu8GammaTab[3],
                                      APIPNL_GAMMA_MAPPEING_MODE Gamma_Map_Mode ) ;

    /// @brief Get Gamma correction table.
    /// @return A Gamma table used currently.
    MS_U8** ( * const GammaTab    ) ( void ) ;

    /// @brief printout panel data, width, height, htt, vtt etc.
    void    ( * const Dump        ) ( void ) ;

    /// @brief Get Min/Max/Current Dimming Value according to the given flag.
    /// @param max_min_setting     Flag of Min / Max / Current Dimming Value.s
    MS_U8   ( * const DimCtrl     ) ( APIPNL_DIMMING_CTRL max_min_setting ) ;

    /// @brief Query Power On Timing with given power on timing order.\n
    /// @param power_on_sequence_timing order
    MS_U16  ( * const OnTiming    ) ( APIPNL_POWER_TIMING_SEQ power_on_sequence_timing  ) ;

    /// @brief Query Power Off Timing with given power on timing order.\n
    /// @param power_off_sequence_timing order
    MS_U16  ( * const OffTiming   ) ( APIPNL_POWER_TIMING_SEQ power_off_sequence_timing ) ;

    //
    // Custimized methods, can be provided by clinets.
    //
    void   ( *TurnBackLightOn     ) ( MS_BOOL bEnable ) ;
    APIPNL_OUT_TIMING_MODE
          ( * const OutTimingMode )( void ); ///<output timing mode

    ///@brief Set Gamma value
    ///@param u8Channel     R/G/B channel, 0->R, 1->G, 2->B
    ///@param u16Offset     The address of Gamma value
    ///@param u16GammaValue Gamma value
    MS_BOOL (* const SetGammaValue)(MS_U8 u8Channel, MS_U16 u16Offset, MS_U16 u16GammaValue);

    /// @brief Get Gamma correction table.
    /// @param eGammaType       Resolution of gamma table
    /// @param pu8GammaTab      gamma table
    /// @param Gamma_Map_Mode   8Bit mapping or 10Bit mapping
    MS_BOOL ( * const GetGammaTbl ) ( APIPNL_GAMMA_TYPE eGammaType,
                                      MS_U8* pu8GammaTab[3],
                                      APIPNL_GAMMA_MAPPEING_MODE Gamma_Map_Mode ) ;
}XC_PNL_OBJ;

/// A panel struct type used to specify the panel attributes, and settings from Board layout
typedef struct __attribute__((packed))
{
    const char *m_pPanelName;                ///<  PanelName
#if !(defined(UFO_PUBLIC_HEADER_212) || defined(UFO_PUBLIC_HEADER_300))
#if !defined (__aarch64__)
    MS_U32 u32AlignmentDummy0;
#endif
#endif
    //
    //  Panel output
    //
    MS_U8 m_bPanelDither :1;                 ///<  PANEL_DITHER, keep the setting
    APIPNL_LINK_TYPE m_ePanelLinkType   :4;  ///<  PANEL_LINK

    ///////////////////////////////////////////////
    // Board related setting
    ///////////////////////////////////////////////
    MS_U8 m_bPanelDualPort  :1;              ///<  VOP_21[8], MOD_4A[1],    PANEL_DUAL_PORT, refer to m_bPanelDoubleClk
    MS_U8 m_bPanelSwapPort  :1;              ///<  MOD_4A[0],               PANEL_SWAP_PORT, refer to "LVDS output app note" A/B channel swap
    MS_U8 m_bPanelSwapOdd_ML    :1;          ///<  PANEL_SWAP_ODD_ML
    MS_U8 m_bPanelSwapEven_ML   :1;          ///<  PANEL_SWAP_EVEN_ML
    MS_U8 m_bPanelSwapOdd_RB    :1;          ///<  PANEL_SWAP_ODD_RB
    MS_U8 m_bPanelSwapEven_RB   :1;          ///<  PANEL_SWAP_EVEN_RB

    MS_U8 m_bPanelSwapLVDS_POL  :1;          ///<  MOD_40[5], PANEL_SWAP_LVDS_POL, for differential P/N swap
    MS_U8 m_bPanelSwapLVDS_CH   :1;          ///<  MOD_40[6], PANEL_SWAP_LVDS_CH, for pair swap
    MS_U8 m_bPanelPDP10BIT      :1;          ///<  MOD_40[3], PANEL_PDP_10BIT ,for pair swap
    MS_U8 m_bPanelLVDS_TI_MODE  :1;          ///<  MOD_40[2], PANEL_LVDS_TI_MODE, refer to "LVDS output app note"

    ///////////////////////////////////////////////
    // For TTL Only
    ///////////////////////////////////////////////
    MS_U8 m_ucPanelDCLKDelay;                ///<  PANEL_DCLK_DELAY
    MS_U8 m_bPanelInvDCLK   :1;              ///<  MOD_4A[4],                   PANEL_INV_DCLK
    MS_U8 m_bPanelInvDE     :1;              ///<  MOD_4A[2],                   PANEL_INV_DE
    MS_U8 m_bPanelInvHSync  :1;              ///<  MOD_4A[12],                  PANEL_INV_HSYNC
    MS_U8 m_bPanelInvVSync  :1;              ///<  MOD_4A[3],                   PANEL_INV_VSYNC

    ///////////////////////////////////////////////
    // Output driving current setting
    ///////////////////////////////////////////////
    // driving current setting (0x00=4mA, 0x01=6mA, 0x02=8mA, 0x03=12mA)
    MS_U8 m_ucPanelDCKLCurrent;              ///<  define PANEL_DCLK_CURRENT
    MS_U8 m_ucPanelDECurrent;                ///<  define PANEL_DE_CURRENT
    MS_U8 m_ucPanelODDDataCurrent;           ///<  define PANEL_ODD_DATA_CURRENT
    MS_U8 m_ucPanelEvenDataCurrent;          ///<  define PANEL_EVEN_DATA_CURRENT

    ///////////////////////////////////////////////
    // panel on/off timing
    ///////////////////////////////////////////////
    MS_U16 m_wPanelOnTiming1;                ///<  time between panel & data while turn on power
    MS_U16 m_wPanelOnTiming2;                ///<  time between data & back light while turn on power
    MS_U16 m_wPanelOffTiming1;               ///<  time between back light & data while turn off power
    MS_U16 m_wPanelOffTiming2;               ///<  time between data & panel while turn off power

    ///////////////////////////////////////////////
    // panel timing spec.
    ///////////////////////////////////////////////
    // sync related
    MS_U8 m_ucPanelHSyncWidth;               ///<  VOP_01[7:0], PANEL_HSYNC_WIDTH
    MS_U8 m_ucPanelHSyncBackPorch;           ///<  PANEL_HSYNC_BACK_PORCH, no register setting, provide value for query only

                                             ///<  not support Manuel VSync Start/End now
                                             ///<  VOP_02[10:0] VSync start = Vtt - VBackPorch - VSyncWidth
                                             ///<  VOP_03[10:0] VSync end = Vtt - VBackPorch
    MS_U8 m_ucPanelVSyncWidth;               ///<  define PANEL_VSYNC_WIDTH
    MS_U8 m_ucPanelVBackPorch;               ///<  define PANEL_VSYNC_BACK_PORCH

    // DE related
    MS_U16 m_wPanelHStart;                   ///<  VOP_04[11:0], PANEL_HSTART, DE H Start (PANEL_HSYNC_WIDTH + PANEL_HSYNC_BACK_PORCH)
    MS_U16 m_wPanelVStart;                   ///<  VOP_06[11:0], PANEL_VSTART, DE V Start
    MS_U16 m_wPanelWidth;                    ///< PANEL_WIDTH, DE width (VOP_05[11:0] = HEnd = HStart + Width - 1)
    MS_U16 m_wPanelHeight;                   ///< PANEL_HEIGHT, DE height (VOP_07[11:0], = Vend = VStart + Height - 1)

    // DClk related
    MS_U16 m_wPanelMaxHTotal;                ///<  PANEL_MAX_HTOTAL. Reserved for future using.
    MS_U16 m_wPanelHTotal;                   ///<  VOP_0C[11:0], PANEL_HTOTAL
    MS_U16 m_wPanelMinHTotal;                ///<  PANEL_MIN_HTOTAL. Reserved for future using.

    MS_U16 m_wPanelMaxVTotal;                ///<  PANEL_MAX_VTOTAL. Reserved for future using.
    MS_U16 m_wPanelVTotal;                   ///<  VOP_0D[11:0], PANEL_VTOTAL
    MS_U16 m_wPanelMinVTotal;                ///<  PANEL_MIN_VTOTAL. Reserved for future using.

    MS_U8 m_dwPanelMaxDCLK;                  ///<  PANEL_MAX_DCLK. Reserved for future using.
    MS_U8 m_dwPanelDCLK;                     ///<  LPLL_0F[23:0], PANEL_DCLK          ,{0x3100_10[7:0], 0x3100_0F[15:0]}
    MS_U8 m_dwPanelMinDCLK;                  ///<  PANEL_MIN_DCLK. Reserved for future using.

                                             ///<  spread spectrum
    MS_U16 m_wSpreadSpectrumStep;            ///<  move to board define, no use now.
    MS_U16 m_wSpreadSpectrumSpan;            ///<  move to board define, no use now.

    MS_U8 m_ucDimmingCtl;                    ///<  Initial Dimming Value
    MS_U8 m_ucMaxPWMVal;                     ///<  Max Dimming Value
    MS_U8 m_ucMinPWMVal;                     ///<  Min Dimming Value

    MS_U8 m_bPanelDeinterMode   :1;          ///<  define PANEL_DEINTER_MODE,  no use now
    E_PNL_ASPECT_RATIO m_ucPanelAspectRatio; ///<  Panel Aspect Ratio, provide information to upper layer application for aspect ratio setting.
  /*
    *
    * Board related params
    *
    *  If a board ( like BD_MST064C_D01A_S ) swap LVDS TX polarity
    *    : This polarity swap value =
    *      (LVDS_PN_SWAP_H<<8) | LVDS_PN_SWAP_L from board define,
    *  Otherwise
    *    : The value shall set to 0.
    */
    MS_U16 m_u16LVDSTxSwapValue;
    APIPNL_TIBITMODE m_ucTiBitMode;                         ///< MOD_4B[1:0], refer to "LVDS output app note"
    APIPNL_OUTPUTFORMAT_BITMODE m_ucOutputFormatBitMode;

    MS_U8 m_bPanelSwapOdd_RG    :1;          ///<  define PANEL_SWAP_ODD_RG
    MS_U8 m_bPanelSwapEven_RG   :1;          ///<  define PANEL_SWAP_EVEN_RG
    MS_U8 m_bPanelSwapOdd_GB    :1;          ///<  define PANEL_SWAP_ODD_GB
    MS_U8 m_bPanelSwapEven_GB   :1;          ///<  define PANEL_SWAP_EVEN_GB

    /**
    *  Others
    */
    MS_U8 m_bPanelDoubleClk     :1;             ///<  LPLL_03[7], define Double Clock ,LVDS dual mode
    MS_U32 m_dwPanelMaxSET;                     ///<  define PANEL_MAX_SET
    MS_U32 m_dwPanelMinSET;                     ///<  define PANEL_MIN_SET
    APIPNL_OUT_TIMING_MODE m_ucOutTimingMode;   ///<Define which panel output timing change mode is used to change VFreq for same panel
    MS_U8 m_bPanelNoiseDith     :1;             ///<  PAFRC mixed with noise dither disable
} PanelType;

//Display information
typedef struct  DLL_PACKED
{
    MS_U32 VDTOT; //Output vertical total
    MS_U32 DEVST; //Output DE vertical start
    MS_U32 DEVEND;//Output DE Vertical end
    MS_U32 HDTOT;// Output horizontal total
    MS_U32 DEHST; //Output DE horizontal start
    MS_U32 DEHEND;// Output DE horizontal end
    MS_BOOL bInterlaceMode;
    MS_BOOL bYUVOutput;
} MS_PNL_DST_DispInfo;

//HW LVDS Reserved Bit to L/R flag Info
typedef struct
{
    MS_U32 u32pair; // pair 0: BIT0, pair 1: BIT1, pair 2: BIT2, pair 3: BIT3, pair 4: BIT4, etc ...
    MS_U16 u16channel; // channel A: BIT0, channel B: BIT1,
    MS_BOOL bEnable;
} MS_PNL_HW_LVDSResInfo;

/// Define the initial OverDrive for XC
typedef struct  DLL_PACKED
{
    MS_U8 u8ODTbl[1056];
    MS_U32 u32PNL_version;                  ///<Version of current structure.
    // OD frame buffer related
#if defined(UFO_PUBLIC_HEADER_700)
    MS_PHY u32OD_MSB_Addr;              ///<OverDrive MSB frame buffer start address, absolute without any alignment
#else
    MS_PHYADDR u32OD_MSB_Addr;              ///<OverDrive MSB frame buffer start address, absolute without any alignment
#endif
    MS_U32 u32OD_MSB_Size;                  ///<OverDrive MSB frame buffer size, the unit is BYTES
#if defined(UFO_PUBLIC_HEADER_700)
    MS_PHY u32OD_LSB_Addr;              ///<OverDrive LSB frame buffer start address, absolute without any alignment
#else
    MS_PHYADDR u32OD_LSB_Addr;              ///<OverDrive LSB frame buffer start address, absolute without any alignment
#endif
    MS_U32 u32OD_LSB_Size;                  ///<OverDrive MSB frame buffer size, the unit is BYTES
} MS_PNL_OD_INITDATA;

typedef struct  DLL_PACKED
{
    MS_U16* pu16ODTbl;
    MS_U32 u32PNL_version;                  ///<Version of current structure.
    // OD frame buffer related
    MS_PHY u32OD_MSB_Addr;              ///<OverDrive MSB frame buffer start address, absolute without any alignment
    MS_U32 u32OD_MSB_Size;                  ///<OverDrive MSB frame buffer size, the unit is BYTES
    MS_PHY u32OD_LSB_Addr;              ///<OverDrive LSB frame buffer start address, absolute without any alignment
    MS_U32 u32OD_LSB_Size;                  ///<OverDrive MSB frame buffer size, the unit is BYTES
    MS_U32 u32ODTbl_Size;                   ///<OverDrive Table buffer size>
} MS_PNL_OD_SETTING;


typedef struct  DLL_PACKED
{
    MS_U16 m_u16ExpectSwingLevel;
    MS_U8 m_u8ModCaliPairSel;
    MS_U8 m_u8ModCaliTarget;
    MS_S8 m_s8ModCaliOffset;
    MS_BOOL m_bPVDD_2V5;
}MS_PNL_ModCaliInfo;

//-------------------------------------------------------------------------------------------------
//MApi_PNL_Setting enum of cmd
//-------------------------------------------------------------------------------------------------
typedef enum
{
    E_PNL_MOD_PECURRENT_SETTING,
    E_PNL_CONTROL_OUT_SWING,
}E_PNL_SETTING;
//-------------------------------------------------------------------------------------------------
//MApi_PNL_Setting struct of cmd
//-------------------------------------------------------------------------------------------------
typedef struct
{
    MS_U16 u16Current_Level;
    MS_U16 u16Channel_Select;
}ST_PNL_MOD_PECURRENT_SETTING;

typedef struct
{
    MS_U16 u16Swing_Level;
}ST_PNL_CONTROL_OUT_SWING;

typedef enum
{
    E_PNL_PNLGAMMA_TBL_FROM_BIN,
    E_PNL_PNLGAMMA_TBL_FROM_REG,
    E_PNL_PNLGAMMA_TBL_MAX,
}EN_PNL_PNLGAMMA_TBL;

typedef struct  DLL_PACKED
{
    MS_U32 u32Version;                 /// Struct version
    MS_U32 u32Length;                  /// Sturct length
    EN_PNL_PNLGAMMA_TBL  enPnlGammaTbl; /// IN: select Get gamma value from
    MS_U8*  pu8GammaTbl;                /// IN: Bin file address
#if !defined (__aarch64__)
    void *pDummy0;                     /// Dummy parameter
#endif
    MS_U32  u32TblSize;                /// IN: PNL Gamma bin size
    MS_U16* pu16RChannel0;             /// IN/OUT: R channel 0 data
#if !defined (__aarch64__)
    void *pDummy1;                     /// Dummy parameter
#endif
    MS_U16* pu16GChannel0;             /// IN/OUT: G channel 0 data
#if !defined (__aarch64__)
    void *pDumm2;                     /// Dummy parameter
#endif
    MS_U16* pu16BChannel0;             /// IN/OUT: B channel 0 data
#if !defined (__aarch64__)
    void *pDummy3;                     /// Dummy parameter
#endif
    MS_U16* pu16WChannel0;             /// IN/OUT: W channel 0 data
#if !defined (__aarch64__)
    void *pDummy4;                     /// Dummy parameter
#endif
    MS_U16* pu16RChannel1;             /// IN/OUT: R channel 1 data
#if !defined (__aarch64__)
    void *pDummy5;                     /// Dummy parameter
#endif
    MS_U16* pu16GChannel1;             /// IN/OUT: G channel 1 data
#if !defined (__aarch64__)
    void *pDummy6;                     /// Dummy parameter
#endif
    MS_U16* pu16BChannel1;             /// IN/OUT: B channel 1 data
#if !defined (__aarch64__)
    void *pDummy7;                     /// Dummy parameter
#endif
    MS_U16* pu16WChannel1;             /// IN/OUT: W channel 1 data
#if !defined (__aarch64__)
    void *pDummy8;                     /// Dummy parameter
#endif
    MS_U16 u16Index;                   /// IN: the index of the data want to get
    MS_U16 u16Size;                    /// IN: every channel data number
    MS_BOOL bSupportChannel1;          /// OUT: Support PNL gamma Channel 1
    EN_PNL_PNLGAMMA_ENTRIES enGammaEntries;// OUT: PNL gamma bin entries
    APIPNL_GAMMA_TYPE enGammaType;     // OUT: PNL gamma bin type
    MS_U16 u162DTableNum;              // OUT: PNL gamma bin 2D table number
    MS_U16 u163DTableNum;              // OUT: PNL gamma bin 3D table number
}ST_PNL_PNLGAMMATBL;

typedef struct  DLL_PACKED
{
    MS_U32 u32Version;                 ///Struct version
    MS_U32 u32Length;                  ///Sturct length
    APIPNL_GAMMA_TYPE  enGammaType;    ///IN: select gamma type
    MS_U16*  pu16RGammaTab;            ///R channel Gamma data
#if !defined (__aarch64__)
    void *pDummy0;                     /// Dummy parameter
#endif
    MS_U16*  pu16GGammaTab;            ///G channel Gamma data
#if !defined (__aarch64__)
    void *pDummy1;                     /// Dummy parameter
#endif
    MS_U16*  pu16BGammaTab;            ///B Gamma data
#if !defined (__aarch64__)
    void *pDummy2;                     /// Dummy parameter
#endif
    APIPNL_GAMMA_MAPPEING_MODE  enGamma_Map_Mode;                /// IN: Gamma mapping mode
    MS_U16 u16Size;                    /// IN: every channel data number
}ST_PNL_GAMMATBL;
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

/******************************************************************************/
/*                     Variable                                            */
/* ****************************************************************************/
/**
*
*  The global interface for panel manipulation.
*
*  @attention <b>Call "MApi_PNL_Init()" first before using this obj</b>
*/
extern XC_PNL_OBJ g_IPanel;
INTERFACE void* pu32PNLInst;
typedef enum
{
    E_PNL_NO_OUTPUT,
    E_PNL_CLK_ONLY,
    E_PNL_CLK_DATA,
    E_PNL_MAX,
} E_PNL_PREINIT_OPTIONS;
//------------------------------------------------------------------------------
/// Description : Show the PNL driver version
/// @ingroup PNL_INTERFACE_INFO
/// @param  ppVersion \b OUT: output PNL driver version
/// @return @ref APIPNL_Result
/// @return E_APIPNL_OK : succeed
/// @return E_APIPNL_FAIL : fail before timeout or illegal parameters
//------------------------------------------------------------------------------
APIPNL_Result       MApi_PNL_GetLibVer(const MSIF_Version **ppVersion);

//------------------------------------------------------------------------------
/// Description : Show the PNL info
/// @ingroup PNL_INTERFACE_INFO
/// @return @ref APIPNL_Result
/// @return E_APIPNL_OK : succeed
/// @return E_APIPNL_FAIL : fail before timeout or illegal parameters
//------------------------------------------------------------------------------
const PNL_ApiInfo * MApi_PNL_GetInfo(void);

//------------------------------------------------------------------------------
/// Description : Show the PNL Status
/// @ingroup PNL_INTERFACE_ToBeModified
/// @param  pPnlStatus               \b IN: point of panel status
/// @return @ref MS_BOOL
//------------------------------------------------------------------------------
MS_BOOL             MApi_PNL_GetStatus(PNL_ApiStatus *pPnlStatus);

//------------------------------------------------------------------------------
/// Description : Show the PNL Status EX
/// @ingroup PNL_INTERFACE_ToBeModified
/// @param  pPnlExtStatus               \b IN: point of panel status
/// @return @ref MS_BOOL
//------------------------------------------------------------------------------
MS_BOOL             MApi_PNL_GetStatusEx(PNL_ApiExtStatus *pPnlExtStatus);

//------------------------------------------------------------------------------
/// Description : Set the PNL debug level
/// @ingroup PNL_INTERFACE_INFO
/// @param  u16DbgSwitch               \b IN: debug level switch
/// @return @ref MS_BOOL
//------------------------------------------------------------------------------
MS_BOOL             MApi_PNL_SetDbgLevel(MS_U16 u16DbgSwitch);

//------------------------------------------------------------------------------
/// Description : Init the PNL IOMAP base
/// @ingroup PNL_INTERFACE_INIT
/// @return @ref MS_BOOL
//------------------------------------------------------------------------------
MS_BOOL             MApi_PNL_IOMapBaseInit(void);
//------------------------------------------------------------------------------
/// Description : Pre Init the PNL
/// @ingroup PNL_INTERFACE_INIT
/// @param  eInitParam               \b IN: Init Parameter
/// @return @ref MS_BOOL
//------------------------------------------------------------------------------
MS_BOOL             MApi_PNL_PreInit(E_PNL_PREINIT_OPTIONS eInitParam);

//------------------------------------------------------------------------------
/// Description :
/// This is a wrapper for \link MApi_PNL_Init_Ex \endlink.
/// For more information, please check MApi_PNL_Init_Ex( ).
/// @ingroup PNL_INTERFACE_INIT
//------------------------------------------------------------------------------
#ifndef _API_XC_PANEL_C_
#define             MApi_PNL_Init(x) MApi_PNL_Init_Ex(x, (MSIF_Version){{ PNL_API_VERSION },});
#endif
//------------------------------------------------------------------------------
/// \b Description : \n
/// In order to make panel inited and working properly according to panel spec, such as Htotal, Vtotal..etc \n
/// We have to pass these specific panel specs to Utopia panel module by this API.
/// @ingroup PNL_INTERFACE_INIT
/// @param[in]  pSelPanelType A panel struct type used to specify the panel attributes, and settings from PCB board layout
/// @param[in]  LIBVER                      lib version
/// @return @ref MS_BOOL returns @ref TRUE for success, @ref FALSE for failure.
/// @par Example Code
/// \code
/// // MApi_PNL_Init_Ex example : how to pass initial panel data to panel driver.
/// #include <apiPNL.h>
///
/// void main()
/// {
/// 	MS_BOOL bSuccess = FALSE;
///
/// 	// This setting is purely virtual
/// 	// it should be adopted from panel vendors
/// 	PanelType panelSetting = {
/// 		.m_pPanelName     = "MStar_Panel,
/// 		.m_bPanelDither   = 1,
/// 		.m_ePanelLinkType = LINK_LVDS,
/// 		...
/// 		...
/// 	};
///
/// 	bSuccess = MApi_PNL_Init_Ex( &panelSetting,
/// 					             (MSIF_Version){{ PNL_API_VERSION },}
/// 				               );
/// }
/// \endcode
//------------------------------------------------------------------------------
MS_BOOL             MApi_PNL_Init_Ex(PanelType *pSelPanelType, MSIF_Version LIBVER);

//------------------------------------------------------------------------------
/// Description : Panel Get Config
/// @ingroup PNL_INTERFACE_FEATURE
/// @param[in]  pSelPanelType A panel struct type used to specify the panel attributes, and settings from PCB board layout
/// @return @ref APIPNL_Result
//------------------------------------------------------------------------------
APIPNL_Result             MApi_PNL_GetConfig(PanelType *pSelPanelType);

//------------------------------------------------------------------------------
/// Description : Setup the PNL output type
/// @ingroup PNL_INTERFACE_FEATURE
/// @param  eOutputMode               \b IN: setup output mode
//------------------------------------------------------------------------------
void                MApi_PNL_SetOutput(APIPNL_OUTPUT_MODE eOutputMode);

//------------------------------------------------------------------------------
/// Description : Change the PNL type
/// @ingroup PNL_INTERFACE_FEATURE
/// @param  pSelPanelType               \b IN: change panel type
/// @return @ref MS_BOOL
//------------------------------------------------------------------------------
MS_BOOL             MApi_PNL_ChangePanelType(PanelType *pSelPanelType);

//------------------------------------------------------------------------------
/// Description : Dump TCON Table
/// @ingroup PNL_INTERFACE_INIT
/// @param pTCONTable              \b IN: Table
/// @param u8Tcontype               \b IN: use APIPNL_TCON_TAB_TYPE ad input
/// @return @ref MS_BOOL
//------------------------------------------------------------------------------
MS_BOOL             MApi_PNL_TCONMAP_DumpTable(MS_U8 *pTCONTable, MS_U8 u8Tcontype);

//------------------------------------------------------------------------------
/// Description : Control the TCON power sequence
/// @ingroup PNL_INTERFACE_INIT
/// @param pTCONTable              \b IN: Table
/// @param bEnable                    \b IN: Enable Power sequence
/// @return @ref MS_BOOL
//------------------------------------------------------------------------------
MS_BOOL             MApi_PNL_TCONMAP_Power_Sequence(MS_U8 *pTCONTable, MS_BOOL bEnable);
//------------------------------------------------------------------------------
/// Description : Reset the TCON counter
/// @ingroup PNL_INTERFACE_INIT
/// @param bEnable                    \b IN: Enable Power sequence
/// @return @ref MS_BOOL
//------------------------------------------------------------------------------
void                MApi_PNL_TCON_Count_Reset ( MS_BOOL bEnable );
//------------------------------------------------------------------------------
/// Description : Init the TCON
/// @ingroup PNL_INTERFACE_INIT
//------------------------------------------------------------------------------

void                MApi_PNL_TCON_Init( void );
//------------------------------------------------------------------------------
/// Description : Show the PNL Destnation info
/// @ingroup PNL_INTERFACE_INFO
/// @param pDstInfo              \b IN: get destanation onfo
/// @param u32SizeofDstInfo                    \b IN: size of stucture
/// @return @ref MS_BOOL
//------------------------------------------------------------------------------
MS_BOOL             MApi_PNL_GetDstInfo(MS_PNL_DST_DispInfo *pDstInfo, MS_U32 u32SizeofDstInfo);

//------------------------------------------------------------------------------
/// Description : Setup the PNL output swing
/// @ingroup PNL_INTERFACE_FEATURE
/// @param u16Swing_Level              \b IN: setup swing value
/// @return @ref MS_BOOL
//------------------------------------------------------------------------------
MS_BOOL             MApi_PNL_Control_Out_Swing(MS_U16 u16Swing_Level);
//------------------------------------------------------------------------------
/// Description : Setup the PNL output DCLK
/// @ingroup PNL_INTERFACE_ToBeModified
/// @param u16PanelDCLK              \b IN: setup DCLK
/// @param bSetDCLKEnable           \b IN: enable this setting
/// @return @ref MS_BOOL
//------------------------------------------------------------------------------
MS_BOOL             MApi_PNL_ForceSetPanelDCLK(MS_U16 u16PanelDCLK ,MS_BOOL bSetDCLKEnable );
//------------------------------------------------------------------------------
/// Description : Setup the PNL Horizontal start
/// @ingroup PNL_INTERFACE_INIT
/// @param u16PanelHStart              \b IN: setup H start
/// @param bSetHStartEnable           \b IN: enable this setting
/// @return @ref MS_BOOL
//------------------------------------------------------------------------------
MS_BOOL             MApi_PNL_ForceSetPanelHStart(MS_U16 u16PanelHStart ,MS_BOOL bSetHStartEnable);
//------------------------------------------------------------------------------
/// Description : Setup the PNL Output pattern
/// @ingroup PNL_INTERFACE_FEATURE
/// @param bEnable              \b IN: Enable output pattern
/// @param u16Red              \b IN: Red color
/// @param u16Green           \b IN: Green color
/// @param u16Blue             \b IN: Blue color
//------------------------------------------------------------------------------
void                MApi_PNL_SetOutputPattern(MS_BOOL bEnable, MS_U16 u16Red , MS_U16 u16Green, MS_U16 u16Blue);
//------------------------------------------------------------------------------
/// Description : Init the MOD calibration parameter
/// @ingroup PNL_INTERFACE_INIT
/// @param pstModCaliInfo              \b IN: setup mod calibration parameter
/// @return @ref MS_BOOL
//------------------------------------------------------------------------------
MS_BOOL             MApi_Mod_Calibration_Setting(MS_PNL_ModCaliInfo *pstModCaliInfo);

//------------------------------------------------------------------------------
/// Description : Start the MOD calibration
/// @ingroup PNL_INTERFACE_FEATURE
/// @return @ref MS_BOOL
//------------------------------------------------------------------------------
MS_BOOL             MApi_Mod_Do_Calibration(void);

//------------------------------------------------------------------------------
/// Description : Type: This type means package. Different package maybe have different type id.
///                    Check board define or system configure for type id.
/// @ingroup PNL_INTERFACE_INIT
/// @param Type              \b IN: setup LVDS output type for different board layout
//------------------------------------------------------------------------------
void                MApi_BD_LVDS_Output_Type(MS_U16 Type);

//------------------------------------------------------------------------------
/// Description : Setup the LPLL type EX
/// @ingroup PNL_INTERFACE_ToBeModified
/// @param eLPLL_TypeExt              \b IN: setup LPLL Ext type
//------------------------------------------------------------------------------
void                MApi_PNL_SetLPLLTypeExt(APIPNL_LINK_EXT_TYPE eLPLL_TypeExt);

//------------------------------------------------------------------------------
/// Description : Init the PNL MISC config
/// @ingroup PNL_INTERFACE_INIT
/// @param ePNL_MISC              \b IN: setup MISC control
//------------------------------------------------------------------------------
void                MApi_PNL_Init_MISC(APIPNL_MISC ePNL_MISC);

//------------------------------------------------------------------------------
/// Description : Show the PNL MISC config
/// @ingroup PNL_INTERFACE_INIT
/// @return @ref MS_U32
//------------------------------------------------------------------------------
MS_U32              MApi_PNL_GetMiscStatus(void);

//------------------------------------------------------------------------------
/// Description : Setup the PNL output config by user
/// @ingroup PNL_INTERFACE_INIT
/// @param u32OutputCFG0_7                 \b IN: setup output config channel 00~07
/// @param u32OutputCFG8_15               \b IN: setup output config channel 08~15
/// @param u32OutputCFG16_21             \b IN: setup output config channel 16~23
//------------------------------------------------------------------------------
void                MApi_PNL_MOD_OutputConfig_User(MS_U32 u32OutputCFG0_7, MS_U32 u32OutputCFG8_15, MS_U32 u32OutputCFG16_21);

//------------------------------------------------------------------------------
/// Description : Set channel output order
/// @ingroup PNL_INTERFACE_FEATURE
/// @param u8OutputOrderType            \b IN: use enum of APIPNL_OUTPUT_CHANNEL_ORDER
/// @param u16OutputOrder0_3            \b IN: output order of channel 0 to 3
/// @param u16OutputOrder4_7            \b IN: output order of channel 4 to 7
/// @param u16OutputOrder8_11           \b IN: output order of channel 8 to 11
/// @param u16OutputOrder12_13          \b IN: output order of channel 12 to 13
//------------------------------------------------------------------------------
void                MApi_PNL_MOD_OutputChannelOrder(MS_U8  u8OutputOrderType,
                                                    MS_U16 u16OutputOrder0_3,
                                                    MS_U16 u16OutputOrder4_7,
                                                    MS_U16 u16OutputOrder8_11,
                                                    MS_U16 u16OutputOrder12_13);

//------------------------------------------------------------------------------
/// Description : Enable Mirror
/// @ingroup PNL_INTERFACE_FEATURE
/// @param pstPnlMirrorInfo              \b IN: setup Mirror parameter
/// @return @ref APIPNL_Result
//------------------------------------------------------------------------------
APIPNL_Result SYMBOL_WEAK MApi_PNL_SetMirrorMode(ST_PNL_MIRRORINFO *pstPnlMirrorInfo);

//------------------------------------------------------------------------------
/// Description : Get Mirror Status
/// @ingroup PNL_INTERFACE_INFO
/// @param pstPnlMirrorInfo              \b IN: get Mirror parameter
/// @return @ref APIPNL_Result
//------------------------------------------------------------------------------
APIPNL_Result SYMBOL_WEAK MApi_PNL_GetMirrorStatus(ST_PNL_MIRRORINFO *pstPnlMirrorInfo);

//------------------------------------------------------------------------------
/// Description : Setup the PNL 3D LR falg
/// @ingroup PNL_INTERFACE_INIT
/// @param lvdsresinfo                 \b IN: setup LVDS reserved bit to be LR flag pin
//------------------------------------------------------------------------------
void                MApi_PNL_HWLVDSReservedtoLRFlag(MS_PNL_HW_LVDSResInfo lvdsresinfo);

//------------------------------------------------------------------------------
/// Description : Setup the PNL PVDD level
/// @ingroup PNL_INTERFACE_INIT
/// @param bIs2p5              \b IN: setup PVDD voltage is 2.5V or 3.3V
//------------------------------------------------------------------------------
void                MApi_MOD_PVDD_Power_Setting(MS_BOOL bIs2p5);


//------------------------------------------------------------------------------
/// Description : Enable the PNL Video path SSC
/// @ingroup PNL_INTERFACE_FEATURE
/// @param bEnable              \b IN: Enable SSC
/// @return @ref APIPNL_Result
//------------------------------------------------------------------------------
APIPNL_Result       MApi_PNL_SetSSC_En(MS_BOOL bEnable);

//------------------------------------------------------------------------------
/// Description : Set panel SSC Fmodulation
/// @ingroup PNL_INTERFACE_FEATURE
/// @param u16Fmodulation              \b IN:Fmodulation, Unit:0.1Khz
/// @return @ref APIPNL_Result
//------------------------------------------------------------------------------
APIPNL_Result       MApi_PNL_SetSSC_Fmodulation(MS_U16 u16Fmodulation);
//------------------------------------------------------------------------------
/// Description : Set panel SSC Rdeviation
/// @ingroup PNL_INTERFACE_FEATURE
/// @param u16Rdeviation              \b IN: u16Rdeviation, Unit:1%%(1/10000)
/// @return @ref APIPNL_Result
//------------------------------------------------------------------------------
APIPNL_Result       MApi_PNL_SetSSC_Rdeviation(MS_U16 u16Rdeviation);

//------------------------------------------------------------------------------
/// Description : Enable the PNL OSD path SSC
/// @ingroup PNL_INTERFACE_FEATURE
/// @param bEnable              \b IN: Enable OSD SSC
/// @return @ref APIPNL_Result
//------------------------------------------------------------------------------
APIPNL_Result       MApi_PNL_SetOSDSSC_En(MS_BOOL bEnable);

//------------------------------------------------------------------------------
/// Description : Set panel OSD SSC Fmodulation
/// @ingroup PNL_INTERFACE_FEATURE
/// @param u16Fmodulation              \b IN:Fmodulation, Unit:0.1Khz
/// @return @ref APIPNL_Result
//------------------------------------------------------------------------------
APIPNL_Result       MApi_PNL_SetOSDSSC_Fmodulation(MS_U16 u16Fmodulation);

//------------------------------------------------------------------------------
/// Description : Set panel OSD SSC Rdeviation
/// @ingroup PNL_INTERFACE_FEATURE
/// @param u16Rdeviation              \b IN: u16Rdeviation, Unit:1%%(1/10000)
/// @return TRUE --OK   FALSE
//------------------------------------------------------------------------------
APIPNL_Result       MApi_PNL_SetOSDSSC_Rdeviation(MS_U16 u16Rdeviation);

//-------------------------------------------------------------------------------------------------
/// Description : skip the timing change
/// @ingroup PNL_INTERFACE_INIT
/// @param  bSetModeOn               \b IN: TRUE: when set mode on  t; FALSE: when set mode off
/// @return E_APIPNL_OK or E_APIPNL_FAIL
//-------------------------------------------------------------------------------------------------

APIPNL_Result       MApi_PNL_SkipTimingChange(MS_BOOL bFlag);

//-------------------------------------------------------------------------------------------------
/// Description : Set Pre Set Mode On
/// @ingroup PNL_INTERFACE_INIT
/// @param  bSetModeOn               \b IN: TRUE: when set mode on  t; FALSE: when set mode off
/// @return E_APIPNL_OK or E_APIPNL_FAIL
//-------------------------------------------------------------------------------------------------
APIPNL_Result MApi_PNL_PreSetModeOn(MS_BOOL bSetModeOn);

//-------------------------------------------------------------------------------------------------
/// Description : Initialize OverDrive
/// @ingroup PNL_INTERFACE_INIT
/// @param  pPNL_ODInitData                 \b IN: the Initialized Data
/// @param  u32ODInitDataLen                \b IN: the length of the initialized data
/// @return E_APIPNL_OK or E_APIPNL_FAIL
//-------------------------------------------------------------------------------------------------
APIPNL_Result       MApi_PNL_OverDriver_Init(MS_PNL_OD_INITDATA *pPNL_ODInitData, MS_U32 u32ODInitDataLen);

//-------------------------------------------------------------------------------------------------
/// Description : Initialize OverDrive
/// @ingroup PNL_INTERFACE_INIT
/// @param  pPNL_ODSetting                   \b IN: the Initialized Data
/// @param  u32ODInitDataLen                \b IN: the length of the initialized data
/// @return E_APIPNL_OK or E_APIPNL_FAIL
//-------------------------------------------------------------------------------------------------
APIPNL_Result SYMBOL_WEAK MApi_PNL_OverDriver_Setting(MS_PNL_OD_SETTING *pPNL_ODSetting, MS_U32 u32ODInitDataLen);
//-------------------------------------------------------------------------------------------------
/// Description : OverDrive Enable
/// @ingroup PNL_INTERFACE_FEATURE
/// @param  bEnable               \b IN: TRUE: Enable OverDrive; FALSE: Disable OverDrive
/// @return E_APIPNL_OK or E_APIPNL_FAIL
//-------------------------------------------------------------------------------------------------
APIPNL_Result       MApi_PNL_OverDriver_Enable(MS_BOOL bEnable);

//-------------------------------------------------------------------------------------------------
/// Description : Set PNL file path
/// @ingroup PNL_INTERFACE_INIT
/// @param  ePNLFilePath                   \b IN: the file type
/// @param  b_PNLFilePath                  \b IN: the file path
/// @return E_APIPNL_OK or E_APIPNL_FAIL
//-------------------------------------------------------------------------------------------------
APIPNL_Result SYMBOL_WEAK MApi_PNL_SetFilePath(E_PNL_FILE_PATH enPNLFilePath,char* const pPNLFilePath);

//-------------------------------------------------------------------------------------------------
/// Description : Get TCON capability
/// @ingroup PNL_INTERFACE_INFO
/// @return MS_BOOL
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_PNL_Get_TCON_Capability(void);


//-------------------------------------------------------------------------------------------------
/// Description : Set FRC MOD pair swap
/// @ingroup PNL_INTERFACE_FEATURE
/// @param  u32Polarity               \b IN: u32Polarity, (d:c:b:a)=([15:14],[13:12],[11:10],[9:8]) => (10,00,11,01), => (2,0,3,1)
//-------------------------------------------------------------------------------------------------
void MApi_PNL_SetPairSwap(MS_U32 u32Polarity);

//-------------------------------------------------------------------------------------------------
/// Description : Set Ext LPLL type
/// @ingroup PNL_INTERFACE_ToBeModified
/// @param  u16Ext_lpll_type               \b IN: ldHz = Htt*Vtt*Vfreq
//-------------------------------------------------------------------------------------------------
void MApi_PNL_SetExt_LPLL_Type(MS_U16 u16Ext_lpll_type);

//-------------------------------------------------------------------------------------------------
/// Description : Cal Ext LPLL Set by DCLK
/// @ingroup PNL_INTERFACE_FEATURE
/// @param  ldHz               \b IN: ldHz = Htt*Vtt*Vfreq
//-------------------------------------------------------------------------------------------------
void MApi_PNL_CalExtLPLLSETbyDClk(MS_U32 ldHz);

//-------------------------------------------------------------------------------------------------
/// Description : Enable Internal Termination for Urania, disable it for others case
/// @ingroup PNL_INTERFACE_FEATURE
/// @param  bEnable               \b IN: Enable or Disable
/// @return TRUE or FALSE
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_PNL_EnableInternalTermination(MS_BOOL bEnable);


//-------------------------------------------------------------------------------------------------
/// Description : Set power state
/// @ingroup PNL_INTERFACE_FEATURE
/// @param  ePowerState                 \b IN: power state
/// @return MS_U32
//-------------------------------------------------------------------------------------------------
MS_U32 MApi_PNL_SetPowerState(EN_POWER_MODE ePowerState);


//////////////////////////////////////////////
// Below functions are obosolted ! Please do not use them if you do not use them yet.
//////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
/// Description : Set power state
/// @ingroup PNL_INTERFACE_ToBeRemove
/// @param  ePowerState                 \b IN: power state
/// @return MS_U32
//-------------------------------------------------------------------------------------------------
MS_BOOL             MApi_PNL_SetDiffSwingLevel(MS_U8 u8Swing_Level);

//-------------------------------------------------------------------------------------------------
/// Do handshake for special output device, ex. VB1
/// @ingroup PNL_INTERFACE_FEATURE
/// @return TRUE or FALSE
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_PNL_OutputDeviceHandshake(void);

//-------------------------------------------------------------------------------------------------
/// Do OC handshake for special output device, ex. VB1
/// @ingroup PNL_INTERFACE_FEATURE
/// @return TRUE or FALSE
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_PNL_OutputDeviceOCHandshake(void);

//-------------------------------------------------------------------------------------------------
/// set necessary setting for outputing interlace timing to rear
/// @ingroup PNL_INTERFACE_FEATURE
/// @return APIPNL_Result
//-------------------------------------------------------------------------------------------------
APIPNL_Result MApi_PNL_SetOutputInterlaceTiming(MS_BOOL bEnable);

//-------------------------------------------------------------------------------------------------
/// Get Support Max output Dclk
/// @ingroup PNL_INTERFACE_FEATURE
/// @return MS_U16
//-------------------------------------------------------------------------------------------------
MS_U16 SYMBOL_WEAK MApi_PNL_GetSupportMaxDclk(void);

//-------------------------------------------------------------------------------------------------
/// Panel Pol Invert for special panel usecase
/// @ingroup PNL_INTERFACE_FEATURE
/// @return TRUE or FALSE
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_PNL_PolInvert(void);

//-------------------------------------------------------------------------------------------------
/// Set Pol Invert threshold (osd threshold, video threshold, motion threshold)
/// @ingroup PNL_INTERFACE_FEATURE
/// @param  u8OsdThreshold                 \b IN: osd threshold: 0~100(0~100%)
/// @param  u8VideoThreshold               \b IN: video threshold: 0~100(0~100%)
/// @param  u8MotionThreshold              \b IN: motion threshold: 0~100(0~100%)
/// @return TRUE or FALSE
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_PNL_SetPolInvertThreshold(MS_U8 u8OsdThreshold, MS_U8 u8VideoThreshold, MS_U8 u8MotionThreshold);

//-------------------------------------------------------------------------------------------------
///  Description : Get Gamma table ex.
/// @ingroup PNL_INTERFACE_FEATURE
/// @param  pstGetGammaTbl                 \b IN\OUT: GAMMA TABLE
/// @return TRUE or FALSE
//-------------------------------------------------------------------------------------------------
MS_BOOL SYMBOL_WEAK MApi_PNL_GetGammaTblEx(ST_PNL_GAMMATBL *pstGetGammaTbl);

//-------------------------------------------------------------------------------------------------
///  Description : Get Panel Gamma table.
/// @ingroup PNL_INTERFACE_FEATURE
/// @param  pstGetPNLGammaTbl                 \b OUT: Panel GAMMA TABLE
/// @return TRUE or FALSE
//-------------------------------------------------------------------------------------------------
MS_BOOL SYMBOL_WEAK MApi_PNL_GetPanelGammaTbl(ST_PNL_PNLGAMMATBL *pstGetPNLGammaTbl);

//-------------------------------------------------------------------------------------------------
///  Description : Set Panel Gamma table.
/// @ingroup PNL_INTERFACE_FEATURE
/// @param  pstSetPNLGammaTbl                 \b IN: Panel GAMMA TABLE
/// @return TRUE or FALSE
//-------------------------------------------------------------------------------------------------
MS_BOOL SYMBOL_WEAK MApi_PNL_SetPanelGammaTbl(ST_PNL_PNLGAMMATBL *pstSetPNLGammaTbl);

void MApi_PNL_GetPanelData(PanelType* pstPNLData);
void MApi_PNL_DumpPanelData(void);
void MApi_PNL_SetSSC(MS_U16 u16Fmodulation, MS_U16 u16Rdeviation, MS_BOOL bEnable);
MS_U16 MApi_PNL_GetPanelOnTiming(APIPNL_POWER_TIMING_SEQ seq);
MS_U16 MApi_PNL_GetPanelOffTiming(APIPNL_POWER_TIMING_SEQ seq);
MS_U8 MApi_PNL_GetPanelDimCtrl(APIPNL_DIMMING_CTRL dim_type);
MS_U8** MApi_PNL_GetAllGammaTbl(void);
MS_BOOL MApi_PNL_EnablePanel(MS_BOOL bPanelOn);
MS_BOOL SYMBOL_WEAK MApi_PNL_SetPanelGamma(MS_U8* pu8GammaTab, MS_U8 u8Index);
MS_BOOL MApi_PNL_SetGammaTbl(APIPNL_GAMMA_TYPE eGammaType, MS_U8* pu8GammaTab[3], APIPNL_GAMMA_MAPPEING_MODE Gamma_Map_Mode);
MS_BOOL MApi_PNL_GetGammaTbl(APIPNL_GAMMA_TYPE eGammaType, MS_U8* pu8GammaTab[3], APIPNL_GAMMA_MAPPEING_MODE Gamma_Map_Mode);
MS_BOOL MApi_PNL_SetGammaValue(MS_U8 u8Channel, MS_U16 u16Offset, MS_U16 u16GammaValue);
MS_U16 MApi_PNL_InitLocalDimming(pstPNL_Init_LocalDimming_Parameters pstPnlLDArgs, MS_U16 u16DataLen);
MS_BOOL MApi_PNL_Check_VBY1_Handshake_Status(void);
void MApi_PNL_SetVideoHWTraining(MS_BOOL bEnable);
void MApi_PNL_SetOSDHWTraining(MS_BOOL bEnable);
MS_BOOL MApi_PNL_GetVideoHWTraining_Status(void);
MS_BOOL MApi_PNL_GetOSDHWTraining_Status(void);
MS_BOOL SYMBOL_WEAK MApi_PNL_GetOutputInterlaceTiming(void);
APIPNL_Result MApi_PNL_Setting(MS_U32 u32Cmd,void *pCmdArgs,MS_U32 u32CmdArgsSize);
MS_BOOL SYMBOL_WEAK MApi_PNL_SetPanelGammaBIN(MS_U8* pu8GammaTab,MS_U32 u32Size,MS_U8 u8Index);
#undef INTERFACE

#ifdef __cplusplus
}
#endif

#endif
