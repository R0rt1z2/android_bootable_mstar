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
/// @brief  HDMI Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

/*! \defgroup MISC HDMI modules

 * \defgroup HDMI_IF HDMI interface (drvXC_HDMI_if.h)
 * \ingroup MISC

<b> Features </b>
  - HDMI Ethernet Channel
   Support Ethernet communication through HDMI
  - Audio Return Channel
  - 3D support
  - Frame packing, Side-by-Side half, Top-Buttom and etc.
  - 4Kx2K support
   3840x2160@24/25/30Hz, 4096x2160@24Hz
  - Additional Color Spaces
   sYCC601, AdobeRGB, AdobeYCC601
  - HDMI Micro Connector
   19-pin/1080p

   \image html api_HDMI_if_pic1.png

<b> Operation Flow </b>
 - Initialized and enable HDMI
     - MApi_HDMITx_SetHPDGpioPin(_st_msAPI_XC_InitData.u16HdmitxHpdPin);
     - MApi_HDMITx_Init();
     - MApi_HDMITx_TurnOnOff(TRUE);
     - MApi_HDMITx_SetVideoOnOff(TRUE)
     - MApi_HDMITx_SetAudioConfiguration(HDMITX_AUDIO_48K, HDMITX_AUDIO_CH_2, HDMITX_AUDIO_PCM);
*/
#ifndef DRVXC_HDMI_IF_H
#define DRVXC_HDMI_IF_H

#include "MsDevice.h"
#include "MsCommon.h"
#ifdef __cplusplus
extern "C"
{
#endif

//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------

#ifdef MDRV_HDMI_C
#define INTERFACE
#else
#define INTERFACE extern
#endif

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define _BYTE_0  0x00
#define _BYTE_1  0x01
#define _BYTE_2  0x02
#define _BYTE_3  0x03
#define _BYTE_4  0x04
#define _BYTE_5  0x05
#define _BYTE_6  0x06
#define _BYTE_7  0x07
#define _BYTE_8  0x08
#define _BYTE_9  0x09
#define _BYTE_10  0x0A
#define _BYTE_11  0x0B
#define _BYTE_12  0x0C
#define _BYTE_13  0x0D
// library information
#define MSIF_HDMI_LIB_CODE               {'H','D','M','I'}
#define MSIF_HDMI_LIBVER                 {'0','1'}
#define MSIF_HDMI_BUILDNUM               {'0','7'}
#define MSIF_HDMI_CHANGELIST             {'0','0','3','5','1','6','6','3'}
#define HDMI_DRV_VERSION                  /* Character String for DRV/API version             */  \
    MSIF_TAG,                           /* 'MSIF'                                           */  \
    MSIF_CLASS,                         /* '00'                                             */  \
    MSIF_CUS,                           /* 0x0000                                           */  \
    MSIF_MOD,                           /* 0x0000                                           */  \
    MSIF_CHIP,                                                                                  \
    MSIF_CPU,                                                                                   \
    MSIF_HDMI_LIB_CODE  ,                  /* IP__                                             */  \
    MSIF_HDMI_LIBVER    ,                          /* 0.0 ~ Z.Z                                        */  \
    MSIF_HDMI_BUILDNUM  ,                          /* 00 ~ 99                                          */  \
    MSIF_HDMI_CHANGELIST,  /* CL#                                              */  \
    MSIF_OS

#define HDMI_VSDB_PACKET_SIZE 14
#define HDMI_HDR_PACKET_SIZE HDMI_VSDB_PACKET_SIZE

/// Extend packet receive structure version
#define HDMI_EXTEND_PACKET_RECEIVE_VERSION          4U
#define HDMI_EXTEND_PACKET_RECEIVE_VERSION_1_SIZE   30U

/// HDR metadata structure version
#define HDMI_HDR_METADATA_VERSION                   2U
#define HDMI_HDR_METADATA_VERSION_1_SIZE            32U

/// AVI parsing info structure version
#define HDMI_AVI_PARSING_INFO_VERSION               1U
#define HDMI_AVI_PARSING_INFO_VERSION_1_SIZE        24U

/// VS parsing info structure version
#define HDMI_VS_PARSING_INFO_VERSION                1U
#define HDMI_VS_PARSING_INFO_VERSION_1_SIZE         13U

/// GC parsing info structure version
#define HDMI_GC_PARSING_INFO_VERSION                1U
#define HDMI_GC_PARSING_INFO_VERSION_1_SIZE         11U

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
// for HDCP 22 callback function
typedef void (*HDCP22_Recv_CBF)(MS_U8, MS_U8, MS_U8*, MS_U32, void*); //type, portIdx, msg, msg length, context

typedef enum
{
    E_HDMI_SWITCH_VSDB_TO_HDR_PACKET, // XC resource
    E_HDMI_HDR_INFOFRAME_GET,
    E_HDMI_EXTEND_PACKET_RECEIVE_GET,
    E_HDMI_EXTEND_XC_RESOURCE_CMD_END = 0x500,

    E_HDMI_INTERFACE_CMD_GET_HDCP_STATE = E_HDMI_EXTEND_XC_RESOURCE_CMD_END, // HDMIRX resource
    /*************************** HDCP Repeater ***************************/
    E_HDMI_INTERFACE_CMD_WRITE_X74,
    E_HDMI_INTERFACE_CMD_READ_X74,
    E_HDMI_INTERFACE_CMD_SET_REPEATER,
    E_HDMI_INTERFACE_CMD_SET_BSTATUS,
    E_HDMI_INTERFACE_CMD_SET_HDMI_MODE,
    E_HDMI_INTERFACE_CMD_GET_INTERRUPT_STATUS,
    E_HDMI_INTERFACE_CMD_WRITE_KSV_LIST,
    E_HDMI_INTERFACE_CMD_SET_VPRIME,
    /*************************** HDCP Repeater ***************************/
    E_HDMI_INTERFACE_CMD_GET_HDCP_ENC_STATE,
    E_HDMI_INTERFACE_CMD_HDR_INFOFRAME_GET,
    E_HDMI_INTERFACE_CMD_EXTEND_PACKET_RECEIVE_GET,
    E_HDMI_INTERFACE_CMD_DATA_RTERM_CONTROL,
    E_HDMI_INTERFACE_CMD_GET_HDCP_STATUS,
    E_HDMI_INTERFACE_CMD_GET_SCDC_VALUE,
    E_HDMI_INTERFACE_CMD_GET_TMDS_RATES_KHZ,
    E_HDMI_INTERFACE_CMD_GET_SCDC_CABLE_DETECT,
    E_HDMI_INTERFACE_CMD_ASSIGN_SOURCE_SELECT,
    E_HDMI_INTERFACE_CMD_GET_PACKET_STATUS,
    E_HDMI_INTERFACE_CMD_GET_PACKET_CONTENT,
    E_HDMI_INTERFACE_CMD_GET_HDR_METADATA,
    E_HDMI_INTERFACE_CMD_GET_AVI_PARSING_INFO,
    E_HDMI_INTERFACE_CMD_GET_VS_PARSING_INFO,
    E_HDMI_INTERFACE_CMD_GET_GC_PARSING_INFO,
    E_HDMI_INTERFACE_CMD_GET_TIMING_INFO,
} E_HDMI_CTRL_ID;

/// HDMI reset type
typedef enum
{
    REST_AVMUTE = _BIT(8),    ///<HDMI Reset bit[8]
    REST_Y_COLOR = _BIT(9),    ///<HDMI Reset bit[9]
    REST_PIXEL_REPETITION  = _BIT(10),    ///<HDMI Reset bit[10]
    REST_FRAME_REPETITION  = _BIT(11),    ///<HDMI Reset bit[11]
    REST_GCP_PKT  = _BIT(12),    ///<HDMI Reset bit[12]
    REST_DEEP_COLOR_FIFO  = _BIT(13),    ///<HDMI Reset bit[13]
    REST_RESERVE = _BIT(14),    ///<HDMI Reset bit[14]
    REST_HDMI_STATUS = _BIT(15),    ///<HDMI Reset bit[15]
}HDMI_REST_t;

/*!
    HDMI Aspect Ratio TYPE
*/
typedef enum
{
    // Active Format Aspect Ratio - AFAR
//    HDMI_AF_AR_Not_Present   = -1,    ///< IF0[11..8] AFD not present, or AFD not yet found
    HDMI_AF_AR_Reserve_0  = 0x00,    ///< IF0[11..8] 0000, Reserved
    HDMI_AF_AR_Reserve_1  = 0x01,    ///< IF0[11..8] 0001, Reserved
    HDMI_AF_AR_16x9_Top = 0x02,    ///< IF0[11..8] 0010, box 16:9 (top).
    HDMI_AF_AR_14x9_Top = 0x03,    ///< IF0[11..8] 0011, box 14:9 (top).
    HDMI_AF_AR_GT_16x9 = 0x04,    ///< IF0[11..8] 0100, box >16:9 (centre)
    HDMI_AF_AR_Reserve_5  = 0x05,    ///< IF0[11..8] 0101, Reserved
    HDMI_AF_AR_Reserve_6  = 0x06,    ///< IF0[11..8] 0110, Reserved
    HDMI_AF_AR_Reserve_7  = 0x07,    ///< IF0[11..8] 0111, Reserved
    HDMI_AF_AR_SAME   = 0x08,    ///< IF0[11..8] 1000, same as picture
    HDMI_AF_AR_4x3_C  = 0x09,    ///< IF0[11..8] 1001, 4:3 Center
    HDMI_AF_AR_16x9_C = 0x0A,    ///< IF0[11..8] 1010, 16:9 Center
    HDMI_AF_AR_14x9_C = 0x0B,    ///< IF0[11..8] 1011, 14:9 Center
    HDMI_AF_AR_Reserve_12   = 0x0C,    ///< IF0[11..8] 1100, Reserved.
    HDMI_AF_AR_4x3_with_14x9_C  = 0x0D,    ///< IF0[11..8] 1101, 4:3 with shoot and protect 14:9 centre.
    HDMI_AF_AR_16x9_with_14x9_C = 0x0E,    ///< IF0[11..8] 1110, 16:9 with shoot and protect 14:9 centre.
    HDMI_AF_AR_16x9_with_4x3_C = 0x0F,    ///< IF0[11..8] 1111, 16:9 with shoot and protect 4:3 centre.

    // Picture Aspect Ratio - PAR
    HDMI_Pic_AR_NODATA = 0x00,     ///< IF0[13..12] 00
    HDMI_Pic_AR_4x3    = 0x10,     ///< IF0[13..12] 01, 4:3
    HDMI_Pic_AR_16x9   = 0x20,     ///< IF0[13..12] 10, 16:9
    HDMI_Pic_AR_RSV    = 0x30,     ///< IF0[13..12] 11, reserved
} MS_HDMI_AR_TYPE;

//-------------------------------------------------------------------------------------------------
/// AVI infomation frame version
//-------------------------------------------------------------------------------------------------
typedef enum
{
    E_AVI_INFOFRAME_VERSION_NON,    //invalid version
    E_AVI_INFOFRAME_VERSION1,       //AVI infomation frame version 1
    E_AVI_INFOFRAME_VERSION2,       //AVI infomation frame version 2
    E_AVI_INFOFRAME_VERSION3        //AVI infomation frame version 3
}EN_AVI_INFOFRAME_VERSION;

#define HDMI_AR_INIT_VALUE          (HDMI_Pic_AR_RSV)
#define HDMI_AR_REG_MASK            (0x3F)
#define HDMI_AFAR_MASK              (0x0F)      // mask to get the AFAR
#define HDMI_AR_MASK                (0x30)      // mask to get the PAR

/// HDMI GControl package type.
typedef enum
{
    G_CTRL_AVMUTE = 0,    ///< bit[0]: AVMUTE
    G_Ctrl_DEFAULT_PHASE,    ///< bit[1]: Default_Phase
    G_Ctrl_LAST_PP,    ///< bit[4:2]: LAST_PP[2:0]
    G_Ctrl_PRE_LPP,    ///< bit[7:5]: PRE_LPP[2:0]
    G_Ctrl_CD_VAL,    ///< bit[11:8]: CD_VAL[3:0]
    G_Ctrl_PP_VAL,    ///< bit[15:12]: PP_VAL[3:0]
    G_Ctrl_ALL
}HDMI_GControl_INFO_t;

/// HDMI PLL control type
typedef enum
{
    PLL_CTRL_L=0,    ///< HDMI_PLL_CTRL1_L  7:0
    PLL_CTRL_MANUAL_DIV,    ///< MANUAL_PLL_DIV    7
    PLL_CTRL_PORST,    ///< PLL_PORST 6
    PLL_CTRL_RESET_TP,    ///< RESET_TP 5
    PLL_CTRL_RESET_TF,    ///< RESET_TF 4
    PLL_CTRL_RESET_TI,    ///< RESET_TI 3
    PLL_CTRL_VCO_OFFSET,    ///< VCO_OFFSET    2
    PLL_CTRL_RESET,    ///< PLL_RESET 1
    PLL_CTRL_PWRDOWN,    ///< PWRDOWN  0

    PLL_CTRL_H,    ///< HDMI_PLL_CTRL1_H 7:0
    PLL_CTRL_KN,    ///< KN[1:0]    7:6
    PLL_CTRL_RCTRL,    ///< RCTRL 5:3
    PLL_CTRL_ICTRL,    ///< ICTRL 2:0
}HDMI_PLL_CTRL_t;

/// HDMI PLL control type
typedef enum
{
    PLL_CTRL2_L,    ///< HDMI_PLL_CTRL2_L 7:0
    PLL_CTRL2_KP,    ///< KP[3:0]   7:4
    PLL_CTRL2_KM,    ///< KM[3:0]   3:0

    PLL_CTRL2_H,    ///< HDMI_PLL_CTRL2_H  7:0
    PLL_CTRL2_DDIV,    ///< DDIV[3:0] 7:4
    PLL_CTRL2_FBDIV,    ///< FBDIV[3:0]    3:0
}HDMI_PLL_CTRL2_t;

////===========================>>HDMI
typedef struct
{
    MS_BOOL bIsHDMIMode;
    MS_BOOL bMuteHDMIVideo;

    MS_BOOL bMuteHDMIAudio;
    MS_BOOL bEnableNonPCM;
    MS_U8      u8AudioStbCnt;      // If stable more than HDMI_AUDIO_STABLE_CNT, enable audio
    // In 199M case, audio on->off->on->off->on, so, we wait 4 * HDMI_AUDIO_STABLE_CNT, if unstable,
    // enable audio still to prevent mute audio always because of un-stable
    MS_U8      u8AudioForceEnableCnt;

    MS_U8      u8ColorFormat;
    MS_U8      u8LostHDMICounter;
    MS_BOOL bColorFMTChange;
    MS_U8      u8PreClolrFormat;
    MS_U8      u8AspectRatio;
} HDMI_POLLING_STATUS_t;

typedef enum
{
    MS_DVI_CHANNEL_R,
    MS_DVI_CHANNEL_G,
    MS_DVI_CHANNEL_B,
} MS_DVI_CHANNEL_TYPE;

/// HDMI COLOR FORMAT
typedef enum
{
    MS_HDMI_COLOR_RGB,    ///< HDMI RGB 444 Color Format
    MS_HDMI_COLOR_YUV_422,    ///< HDMI YUV 422 Color Format
    MS_HDMI_COLOR_YUV_444,    ///< HDMI YUV 444 Color Format
    MS_HDMI_COLOR_YUV_420,    ///< HDMI YUV 420 Color Format
    MS_HDMI_COLOR_RESERVED,    ///< Reserve
    MS_HDMI_COLOR_DEFAULT = MS_HDMI_COLOR_RGB,    ///< Default setting
    MS_HDMI_COLOR_UNKNOWN = 7,    ///< Unknow Color Format
} MS_HDMI_COLOR_FORMAT;

/// HDMI Extended COLORIMETRY
typedef enum
{
    MS_HDMI_EXT_COLOR_XVYCC601,        ///< xvycc 601
    MS_HDMI_EXT_COLOR_XVYCC709,        ///< xvycc 709
    MS_HDMI_EXT_COLOR_SYCC601,         ///< sYCC 601
    MS_HDMI_EXT_COLOR_ADOBEYCC601,     ///< Adobe YCC 601
    MS_HDMI_EXT_COLOR_ADOBERGB,        ///< Adobe RGB
    MS_HDMI_EXT_COLOR_BT2020YcCbcCrc,  /// ITU-F BT.2020 YcCbcCrc
    MS_HDMI_EXT_COLOR_BT2020RGBYCbCr,  /// ITU-R BT.2020 RGB or YCbCr
    MS_HDMI_EXT_COLOR_DEFAULT = MS_HDMI_EXT_COLOR_XVYCC601,    ///< Default setting
    MS_HDMI_EXT_COLOR_UNKNOWN = 7,    ///< Unknow
} MS_HDMI_EXT_COLORIMETRY_FORMAT;

/// HDMI Content Type
typedef enum
{
    MS_HDMI_CONTENT_NoData = 0,    // No Data.
    MS_HDMI_CONTENT_Graphics,  // Graphics(text).
    MS_HDMI_CONTENT_Photo,     // Photo type.
    MS_HDMI_CONTENT_Cinema,    // Cinema type.
    MS_HDMI_CONTENT_Game,      // Game type.
} MS_HDMI_CONTENT_TYPE;

///HDMI COLOR RANGE
typedef enum
{
    E_HDMI_COLOR_RANGE_DEFAULT, //
    E_HDMI_COLOR_RANGE_LIMIT,   //HDMI RGB Limited Range (16-235)
    E_HDMI_COLOR_RANGE_FULL,    //HDMI Full Range (0-255)
    E_HDMI_COLOR_RANGE_RESERVED
} EN_HDMI_COLOR_RANGE;

typedef enum
{
    MS_HDMI_COLOR_DEPTH_6_BIT = 0,
    MS_HDMI_COLOR_DEPTH_8_BIT = 1,
    MS_HDMI_COLOR_DEPTH_10_BIT = 2,
    MS_HDMI_COLOR_DEPTH_12_BIT = 3,
    MS_HDMI_COLOR_DEPTH_16_BIT = 4,
    MS_HDMI_COLOR_DEPTH_UNKNOWN = 5,
}MS_HDMI_COLOR_DEPTH;

typedef enum
{
    MS_HDMI_PIXEL_REPETITION_1X = 0,
    MS_HDMI_PIXEL_REPETITION_2X,
    MS_HDMI_PIXEL_REPETITION_3X,
    MS_HDMI_PIXEL_REPETITION_4X,
    MS_HDMI_PIXEL_REPETITION_5X,
    MS_HDMI_PIXEL_REPETITION_6X,
    MS_HDMI_PIXEL_REPETITION_7X,
    MS_HDMI_PIXEL_REPETITION_8X,
    MS_HDMI_PIXEL_REPETITION_9X,
    MS_HDMI_PIXEL_REPETITION_10X,
    MS_HDMI_PIXEL_REPETITION_RESERVED,
}MS_HDMI_PIXEL_REPETITION;

/// HDMI Equalize Level
typedef enum
{
    MS_HDMI_EQ_2M,    ///< HDMI EQ 2M
    MS_HDMI_EQ_10M,    ///< HDMI EQ 10M
    MS_HDMI_EQ_15M,    ///< HDMI EQ 15M
    MS_HDMI_EQ_MAX,    ///< HDMI EQ MAX
    MS_HDMI_EQ_USER_DEFINE,    ///< HDMI EQ User define
} MS_HDMI_EQ;

/// HDMI Packet nauber
typedef enum
{
    PKT_MPEG  = _BIT(0),    ///< Pactet[0]
    PKT_AUI  = _BIT(1),    ///< Pactet[1]
    PKT_SPD  = _BIT(2),    ///< Pactet[2]
    PKT_AVI  = _BIT(3),    ///< Pactet[3]
    PKT_GC  = _BIT(4),    ///< Pactet[4]
    PKT_ASAMPLE  = _BIT(5),    ///< Pactet[5]
    PKT_ACR  = _BIT(6),    ///< Pactet[6]
    PKT_VS  = _BIT(7),    ///< Pactet[7]
    PKT_NULL  = _BIT(8),    ///< Pactet[8]
    PKT_ISRC2  = _BIT(9),    ///< Pactet[9]
    PKT_ISRC1  = _BIT(10),    ///< Pactet[10]
    PKT_ACP  = _BIT(11),    ///< Pactet[11]
    PKT_ONEBIT_AUD  = _BIT(12),    ///< Pactet[12]
    PKT_GM =  _BIT(13),    ///< Pactet[13]
    PKT_HBR =  _BIT(14),    ///< Pactet[14]
    PKT_VBI =  _BIT(15),    ///< Pactet[15]
    PKT_HDR  = _BIT(16),    ///< Pactet[16]
    PKT_RSV  = _BIT(17),    ///< Pactet[17]
    PKT_EDR  = _BIT(18),	///< Pactet[18]
    PKT_CHANNEL_STATUS = _BIT(19),
    PKT_MULTI_VS = _BIT(20),
}MS_HDMI_PACKET_STATE_t;

typedef enum
{
    CAPS_DVI_ADJ_70M = _BIT(0),
    CAPS_VSYNC_END_EN = _BIT(1),
}MS_HDMI_FUNC_CAPS;

/// HDMI PACKET VALUE structure
typedef struct __attribute__((packed))
{
    MS_U16 PKT_MPEG_VALUE;    ///< MPEG packet content
    MS_U16 PKT_AUI_VALUE;    ///< AUI packet content
    MS_U16 PKT_SPD_VALUE;    ///< SPD packet content
    MS_U16 PKT_AVI_VALUE;    ///< AVI packet content
    MS_U16 PKT_GC_VALUE;    ///< GC packet content
    MS_U16 PKT_ASAMPLE_VALUE;    ///< ASAMPLE packet content
    MS_U16 PKT_ACR_VALUE;    ///< ACR packet content
    MS_U16 PKT_VS_VALUE;    ///< VS packet content
    MS_U16 PKT_NULL_VALUE;    ///< NULL packet content
    MS_U16 PKT_ISRC2_VALUE;    ///< ISRC2 packet content
    MS_U16 PKT_ISRC1_VALUE;    ///< ISRC1 packet content
    MS_U16 PKT_ACP_VALUE;    ///< ACP packet content
    MS_U16 PKT_ONEBIT_AUD_VALUE;    ///< ONE bit AUD packet content
}MS_HDMI_PACKET_VALUE_t;

/// Received HDMI packet flag.
typedef struct __attribute__((packed))
{
    MS_U16 u16Version;                     /// Version.
    MS_U16 u16Size;                        /// Structure size.
    MS_BOOL bPKT_MPEG_RECEIVE;       ///< MPEG packet received
    MS_BOOL bPKT_AUI_RECEIVE;         ///< AUI packet received
    MS_BOOL bPKT_SPD_RECEIVE;         ///< SPD packet received
    MS_BOOL bPKT_AVI_RECEIVE;         ///< AVI packet received
    MS_BOOL bPKT_GC_RECEIVE;         ///< GC packet received
    MS_BOOL bPKT_ASAMPLE_RECEIVE;    ///< ASAMPLE packet received
    MS_BOOL bPKT_ACR_RECEIVE;        ///< ACR packet received
    MS_BOOL bPKT_VS_RECEIVE;          ///< VS packet received
    MS_BOOL bPKT_NULL_RECEIVE;       ///< NULL packet received
    MS_BOOL bPKT_ISRC2_RECEIVE;      ///< ISRC2 packet received
    MS_BOOL bPKT_ISRC1_RECEIVE;      ///< ISRC1 packet received
    MS_BOOL bPKT_ACP_RECEIVE;         ///< ACP packet received
    MS_BOOL bPKT_ONEBIT_AUD_RECEIVE;    ///< ONE bit AUD packet received
    MS_BOOL bPKT_GM_RECEIVE;     /// <GM packet received
    MS_BOOL bPKT_HBR_RECEIVE;    /// <HBR packet received
    MS_BOOL bPKT_VBI_RECEIVE;     /// <VBIpacket received
    MS_BOOL bPKT_HDR_RECEIVE;    ///< HDR packet received
    MS_BOOL bPKT_RSV_RECEIVE;    ///< RSV packet received
    MS_BOOL bPKT_EDR_VALID;      ///< Dolby EDR packet valid
    MS_BOOL bPKT_AUDIO_DST_RECEIVE; // DST
    MS_BOOL bPKT_AUDIO_3D_ASP_RECEIVE; // 3D L-PCM
    MS_BOOL bPKT_AUDIO_3D_DSD_RECEIVE; // One bit 3D audio
    MS_BOOL bPKT_AUDIO_METADATA_RECEIVE; // Audio metadata
    MS_BOOL bPKT_AUDIO_MULTI_ASP_RECEIVE; // Multi-stream
    MS_BOOL bPKT_AUDIO_MULTI_DSD_RECEIVE; // Multi-stream one bit audio
    MS_BOOL bPKT_EM_RECEIVE;    // EM packet received
}MS_HDMI_EXTEND_PACKET_RECEIVE_t;



typedef struct
{
    MS_U8 AVMuteStatus;
    MS_U8 AudioNotPCM;
    MS_U8 PreAudiostatus;
    MS_U8 ChecksumErrOrBCHParityErr;
    MS_U8 PacketColorFormat;
    MS_U8 AspectRatio;
    MS_U8 AudioPacketErr;
    MS_U8 ACPType;
} MS_HDMI_PACKET_STATUS_t;


typedef struct
{
    MS_HDMI_PACKET_VALUE_t enPKT_Value;
    MS_HDMI_PACKET_STATUS_t enPKT_Status;
} HDMI_PACKET_INFO_t, *P_HDMI_PACKET_INFO_t;

/// HDCP STATUS INFO structure
typedef struct __attribute__((packed))
{
    MS_BOOL b_St_HDMI_Mode;     ///> HDCP_STATUS[0] & BIT0
    MS_BOOL b_St_Reserve_1;     ///> (HDCP_STATUS[0] & BIT1)>>1
    MS_BOOL b_St_HDCP_Ver;      ///> (HDCP_STATUS[0] & BIT2)>>2
    MS_BOOL b_St_AVMute;        ///> (HDCP_STATUS[0] & BIT3)>>3
    MS_BOOL b_St_ENC_En;        ///> (HDCP_STATUS[0] & BIT4)>>4
    MS_BOOL b_St_AKsv_Rcv;      ///> (HDCP_STATUS[0] & BIT5)>>5
    MS_BOOL b_St_ENC_Dis;       ///> (HDCP_STATUS[0] & BIT6)>>6
    MS_BOOL b_St_Reserve_7;     ///> (HDCP_STATUS[0] & BIT7)>>7
}MS_HDCP_STATUS_INFO_t;

/// HDMI Black Level
typedef enum
{
    E_BLACK_LEVEL_LOW = 0,    ///> HDMI black level low
    E_BLACK_LEVEL_HIGH,    ///> HDMI black level high
} BLACK_LEVEL;

/// VGA/DVI/HDMI EDID
typedef enum
{
    E_XC_PROG_DVI0_EDID = 0x00,
    E_XC_PROG_DVI1_EDID = 0x01,
    E_XC_PROG_DVI2_EDID = 0x02,
    E_XC_PROG_DVI3_EDID = 0x03,
    E_XC_PROG_VGA_EDID  = 0x10,
} E_XC_DDCRAM_PROG_TYPE;

typedef struct DLL_PACKED
{
    MS_U8 *EDID;                            ///< customer EDID
    MS_U16 u16EDIDSize;                       ///< 128 for VGA/DVI , 256 for HDMI
    E_XC_DDCRAM_PROG_TYPE eDDCProgType;     ///< what kind of DDCRam to program

    MS_U16 u16CECPhyAddr;                   ///< if only 1 Digital DDCRam (get this information from MApi_XC_GetInfo()), you need to specify CEC physical address here
    MS_U8  u8CECPhyAddrIdxL;                ///< start point of CEC physical address (low byte) in EDID
    MS_U8  u8CECPhyAddrIdxH;                ///< start point of CEC physical address (high byte) in EDID
} XC_DDCRAM_PROG_INFO;

////===========================>>HDCP

///HDCP polling status
typedef struct
{
    MS_BOOL bHPD_OK;    ///< HPD OK
    MS_U16     u16HPD_Counter;    ///< HPD times counter
    MS_U16     u16HDCP_KeyCounter;    /// > HDCP key number
    MS_U16     u16HDCP_KeyChkSum;    ///> HDCP key check sum
    MS_BOOL bIsPullHighHPD;    ///> HPD is high or not
    MS_U8      u8BKSV[5];    ///> BKSV

} HDCP_POLLING_STATUS_t;

/**
 *  The return value for HDMI fuction.
 */
typedef enum
{
    /// fail
    E_HDMI_RET_FAIL = 0,
    /// success
    E_HDMI_RET_OK,
} HDMI_Ret_Value;


// HDMI v1.4 new feature - T8 and T9
typedef enum
{
    E_HDMI_NO_ADDITIONAL_FORMAT, // 000
    E_HDMI_4Kx2K_FORMAT,                 // 001
    E_HDMI_3D_FORMAT,                      // 010
    E_HDMI_RESERVED,                         // 011 ~ 111
    E_HDMI_NA                                     // Not available
} E_HDMI_ADDITIONAL_VIDEO_FORMAT;

typedef enum
{
    E_VIC_4Kx2K_30Hz = 0x01, // 1920(x2)x1080(x2) @ 29.97/30Hz
    E_VIC_4Kx2K_25Hz = 0x02, // 1920(x2)x1080(x2) @ 25Hz
    E_VIC_4Kx2K_24Hz = 0x03, // 1920(x2)x1080(x2) @ 23.976/24Hz
    E_VIC_4Kx2K_24Hz_SMPTE = 0x04, // 2048(x2)x1080(x2) @ 24Hz
    E_VIC_RESERVED,                // 0x00, 0x05 ~ 0xFF
    E_VIC_NA                             // Not available
} E_HDMI_VIC_4Kx2K_CODE;

typedef enum
{
    E_3D_EXT_DATA_HOR_SUB_SAMPL_0,      //0000
    E_3D_EXT_DATA_HOR_SUB_SAMPL_1,      //0001
    E_3D_EXT_DATA_HOR_SUB_SAMPL_2,      //0010
    E_3D_EXT_DATA_HOR_SUB_SAMPL_3,      //0011
    E_3D_EXT_DATA_QUINCUNX_MATRIX_0,    //0100
    E_3D_EXT_DATA_QUINCUNX_MATRIX_1,    //0101
    E_3D_EXT_DATA_QUINCUNX_MATRIX_2,    //0110
    E_3D_EXT_DATA_QUINCUNX_MATRIX_3,    //0111
    E_3D_EXT_DATA_RESERVE,              //1000 ~ 1111
    E_3D_EXT_DATA_MODE_MAX,
} E_HDMI_3D_EXT_DATA_T;

typedef enum
{
    E_3D_META_DATA_PARALLAX_ISO23002_3,      //0000
    E_3D_META_DATA_RESERVE,              //001 ~ 1111
    E_3D_META_DATA_MAX,
} E_HDMI_3D_META_DATA_TYPE;

typedef struct
{
    MS_BOOL bImmeswitchSupport;
    MS_U8 ucMHLSupportPath;
} stHDMI_INITIAL_TABLE;

typedef struct DLL_PACKED
{
    MS_BOOL b3D_Meta_Present;
    E_HDMI_3D_META_DATA_TYPE t3D_Metadata_Type;
    MS_U8 u83D_Metadata_Length;
    MS_U8 u83D_Metadata[23]; // Maximum length of VS is 31 - 8 = 23
} sHDMI_3D_META_FIELD;

/////////// HDCP22 /////////////////
typedef enum
{
	E_HDCP22_IF_HDMI,		//0x00
	E_HDCP22_IF_MIRACAST,	//0x01
	//TBD
} E_HDCP22_PORT_TYPE;

typedef enum
{
    E_HDCP_NO_ENCRYPTION = 0, //0x00
    E_HDCP_1_4,               //0x01
    E_HDCP_2_2,               //0x02
}E_HDMI_HDCP_STATE;

typedef enum
{
    E_HDCP_NOT_ENCRYPTION = 0, //0x00
    E_HDCP_1_4_ENCRYPTION,     //0x01
    E_HDCP_2_2_ENCRYPTION,     //0x02
}E_HDMI_HDCP_ENCRYPTION_STATE;

/////////// HDR metadata /////////////////
typedef struct __attribute__((packed))
{
    MS_U16      u16Version;                     /// Version.
    MS_U16      u16Size;                        /// Structure size.
    MS_U8       u8EOTF; // 0:SDR gamma,  1:HDR gamma,  2:SMPTE ST2084,  3:Future EOTF,  4-7:Reserved
    MS_U8       u8Static_Metadata_ID; // 0:Static Metadata Type 1,  1-7:Reserved for future use
    MS_U16      u16Display_Primaries_X[3]; // display_primaries_x
    MS_U16      u16Display_Primaries_Y[3]; // display_primaries_x
    MS_U16      u16White_Point_X; // display primaries Wx
    MS_U16      u16White_Point_Y; // display primaries Wy
    MS_U16      u16Max_Display_Mastering_Luminance; // max display mastering luminance
    MS_U16      u16Min_Display_Mastering_Luminance; // min display mastering luminance
    MS_U16      u16Maximum_Content_Light_Level; // maximum content light level
    MS_U16      u16Maximum_Frame_Average_Light_Level; // maximum frame-average light level
    MS_U8       u8Version;
    MS_U8       u8Length;
} sHDR_METADATA;

typedef struct DLL_PACKED
{
    MS_U16 u16StructVersion;     // Version.
    MS_U16 u16StructSize;        // Structure size.
    MS_U8 u8Version;
    MS_U8 u8Length;
    MS_U8 u8S10Value;
    MS_U8 u8B10Value;
    MS_U8 u8A0Value;
    MS_U8 u8Y210Value;
    MS_U8 u8R3210Value;
    MS_U8 u8M10Value;
    MS_U8 u8C10Value;
    MS_U8 u8SC10Value;
    MS_U8 u8Q10Value;
    MS_U8 u8EC210Value;
    MS_U8 u8ITCValue;
    MS_U8 u8VICValue;
    MS_U8 u8PR3210Value;
    MS_U8 u8CN10Value;
    MS_U8 u8YQ10Value;
    MS_HDMI_COLOR_FORMAT enColorForamt;
    EN_HDMI_COLOR_RANGE enColorRange;
    MS_HDMI_EXT_COLORIMETRY_FORMAT enColormetry;
    MS_HDMI_PIXEL_REPETITION enPixelRepetition;
    MS_HDMI_AR_TYPE enAspectRatio;
    MS_HDMI_AR_TYPE enActiveFormatAspectRatio;
}stAVI_PARSING_INFO;

typedef struct DLL_PACKED
{
    MS_U16 u16StructVersion;     // Version.
    MS_U16 u16StructSize;        // Structure size.
    MS_U8 u8Version;
    MS_U8 u8Length;
    MS_U8 u8HDMIVIC;
    MS_U8 u83DStructure;
    MS_U8 u83DExtData;
    E_HDMI_ADDITIONAL_VIDEO_FORMAT enHDMIVideoFormat;
    E_HDMI_VIC_4Kx2K_CODE en4Kx2KVICCode;
    E_XC_3D_INPUT_MODE en3DStructure;
    E_HDMI_3D_EXT_DATA_T en3DExtendedData;
}stVS_PARSING_INFO;

typedef struct DLL_PACKED
{
    MS_U16 u16StructVersion;     // Version.
    MS_U16 u16StructSize;        // Structure size.
    MS_U8 u8ControlAVMute;
    MS_U8 u8DefaultPhase;
    MS_U8 u8CDValue;
    MS_U8 u8PPValue;
    MS_U8 u8LastPPValue;
    MS_U8 u8PreLastPPValue;
    MS_HDMI_COLOR_DEPTH enColorDepth;
}stGC_PARSING_INFO;

// HDMI Control structure
/**
*   Switch VSDB to HDR packet control parameters.
*/
typedef struct __attribute__((packed))
{
    MS_BOOL bEnableHDR;
} MS_HDMI_Switch_VSDB_to_HDR_Parameters;

/**
*   HDR info-frame get control parameters.
*/
typedef struct __attribute__((packed))
{
    sHDR_METADATA *pstHDRMetadata;
} MS_HDMI_HDR_InfoFrame_Get_Parameters;

/**
*   Extend packet receive get control parameters.
*/
typedef struct __attribute__((packed))
{
    MS_BOOL bEnable;
    MS_HDMI_EXTEND_PACKET_RECEIVE_t *pstPacketReceive;
} MS_HDMI_Extend_Packet_Receive_Get_Parameters;

typedef struct
{
    E_MUX_INPUTPORT enInputPortType;
    MS_U8 ucHDCPState;
} stCMD_HDMI_CHECK_HDCP_STATE;

/************************ HDCP Repeater ************************/
typedef struct
{
    E_MUX_INPUTPORT enInputPortType;
    MS_U8 ucOffset;
    MS_U8 ucData;
} stCMD_HDCP_WRITE_X74;

typedef struct
{
    E_MUX_INPUTPORT enInputPortType;
    MS_U8 ucOffset;
    MS_U8 ucRetData;
} stCMD_HDCP_READ_X74;

typedef struct
{
    E_MUX_INPUTPORT enInputPortType;
    MS_BOOL bIsRepeater;
} stCMD_HDCP_SET_REPEATER;

typedef struct
{
    E_MUX_INPUTPORT enInputPortType;
    MS_U16 usBstatus;
} stCMD_HDCP_SET_BSTATUS;

typedef struct
{
    E_MUX_INPUTPORT enInputPortType;
    MS_BOOL bHDMIMode;
} stCMD_HDCP_SET_HDMI_MODE;

typedef struct
{
    E_MUX_INPUTPORT enInputPortType;
    MS_U8 ucRetIntStatus;
} stCMD_HDCP_GET_INTERRUPT_STATUS;

typedef struct
{
    E_MUX_INPUTPORT enInputPortType;
    MS_U8* pucKSV;
    MS_U32 ulLen;
} stCMD_HDCP_WRITE_KSV_LIST;

typedef struct
{
    E_MUX_INPUTPORT enInputPortType;
    MS_U8* pucVPrime;
} stCMD_HDCP_SET_VPRIME;
/************************ HDCP Repeater ************************/

typedef struct
{
    E_MUX_INPUTPORT enInputPortType;
    MS_U8 ucHDCPENCState;
} stCMD_HDMI_CHECK_HDCP_ENC_STATE;

typedef struct
{
    E_MUX_INPUTPORT enInputPortType;
    MS_BOOL bDataRtermEnable;
} stCMD_HDMI_DATA_RTERM_CONTROL;

typedef struct
{
    E_MUX_INPUTPORT enInputPortType;
    MS_HDCP_STATUS_INFO_t stHDCPStatusInfo;
} stCMD_HDMI_GET_HDCP_STATUS;

typedef struct
{
    E_MUX_INPUTPORT enInputPortType;
    MS_U8 ucOffset;
    MS_U8 ucRetData;
} stCMD_HDMI_GET_SCDC_VALUE;

typedef enum
{
    E_HDMI_GET_HDE,
    E_HDMI_GET_VDE,
    E_HDMI_GET_HTT,
    E_HDMI_GET_VTT,
    E_HDMI_GET_AUDIO_PROTECT_INFO,
    E_HDMI_GET_SUPPORT_EDID_SIZE,
    E_HDMI_GET_ERROR_STATUS,
    E_HDMI_GET_MULTIVS_COUNT,
} E_HDMI_GET_DATA_INFO;

typedef enum
{
    E_HDMI_GET_TMDS_BIT_RATES,
    E_HDMI_GET_TMDS_CHARACTER_RATES,
    E_HDMI_GET_TMDS_CLOCK_RATES,
} E_HDMI_GET_TMDS_RATES;

typedef struct
{
    E_MUX_INPUTPORT enInputPortType;
    E_HDMI_GET_TMDS_RATES enType;
    MS_U32 ulRetRates;
} stCMD_HDMI_GET_TMDS_RATES_KHZ;

typedef struct
{
    E_MUX_INPUTPORT enInputPortType;
    MS_BOOL bCableDetectFlag;
} stCMD_HDMI_GET_SCDC_CABLE_DETECT;

typedef struct
{
    E_MUX_INPUTPORT enInputPortType;
} stCMD_HDMI_ASSIGN_SOURCE_SELECT;

typedef struct DLL_PACKED
{
    E_MUX_INPUTPORT enInputPortType;
    MS_HDMI_EXTEND_PACKET_RECEIVE_t *pExtendPacketReceive;
} stCMD_HDMI_GET_PACKET_STATUS;

typedef struct DLL_PACKED
{
    E_MUX_INPUTPORT enInputPortType;
    MS_HDMI_PACKET_STATE_t enPacketType;
    MS_U8 u8PacketLength;
    MS_U8 *pu8PacketContent;
} stCMD_HDMI_GET_PACKET_CONTENT;

typedef struct DLL_PACKED
{
    E_MUX_INPUTPORT enInputPortType;
    sHDR_METADATA *pstHDRMetadata;
} stCMD_HDMI_GET_HDR_METADATA;

typedef struct DLL_PACKED
{
    E_MUX_INPUTPORT enInputPortType;
    stAVI_PARSING_INFO *pstAVIParsingInfo;
} stCMD_HDMI_GET_AVI_PARSING_INFO;

typedef struct DLL_PACKED
{
    E_MUX_INPUTPORT enInputPortType;
    stVS_PARSING_INFO *pstVSParsingInfo;
} stCMD_HDMI_GET_VS_PARSING_INFO;

typedef struct DLL_PACKED
{
    E_MUX_INPUTPORT enInputPortType;
    stGC_PARSING_INFO *pstGCParsingInfo;
} stCMD_HDMI_GET_GC_PARSING_INFO;

typedef struct DLL_PACKED
{
    E_MUX_INPUTPORT enInputPortType;
    E_HDMI_GET_DATA_INFO enInfoType;
    MS_U16 u16TimingInfo;
} stCMD_HDMI_GET_TIMING_INFO;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

//HDMI/DVI
INTERFACE HDMI_POLLING_STATUS_t    g_HdmiPollingStatus;

//HDCP
INTERFACE HDCP_POLLING_STATUS_t    g_HdcpPollingStatus;

INTERFACE MS_U16 g_AVMUTEMissingCounter;
INTERFACE MS_U16 g_AVIMissingCounter;
INTERFACE MS_U16 g_ACPMissingCounter;

#ifdef DVI_NEW_Patch
    // 20081226 - DVI+HDCP snow noise patch - start ...
INTERFACE MS_U16 g_DVI70MhzPollingCounter;
INTERFACE MS_U8 g_DVI70MhzUnstableCounter;
INTERFACE MS_BOOL g_bDVILess70MHz;
    // 20081226 - DVI+HDCP snow noise patch - end ...
#endif


//HDMI
INTERFACE MS_U16 MDrv_HDMI_Func_Caps(void);
INTERFACE void MDrv_HDMI_init( void );
INTERFACE void MDrv_HDMI_Exit( void );
INTERFACE void MDrv_HDMI_pkt_reset( HDMI_REST_t breset );
INTERFACE void MDrv_HDMI_pullhpd( MS_BOOL bHighLow, E_MUX_INPUTPORT enInputPortType, MS_BOOL bInverse);
INTERFACE MS_U16 MDrv_HDMI_GC_Info(HDMI_GControl_INFO_t gcontrol);
INTERFACE MS_HDMI_PACKET_VALUE_t MDrv_HDMI_Packet_Received(MS_BOOL bEnable);
INTERFACE MS_HDMI_COLOR_FORMAT MDrv_HDMI_Get_ColorFormat(void);
INTERFACE EN_HDMI_COLOR_RANGE MDrv_HDMI_Get_ColorRange(void);
INTERFACE MS_HDMI_CONTENT_TYPE MDrv_HDMI_Get_Content_Type(void);
INTERFACE MS_HDMI_EXT_COLORIMETRY_FORMAT MDrv_HDMI_Get_ExtColorimetry(void);

INTERFACE void MDrv_HDMI_Set_EQ(MS_HDMI_EQ enEq, MS_U8 u8EQValue);
INTERFACE MS_BOOL MDrv_HDMI_Set_EQ_To_Port(MS_HDMI_EQ enEq, MS_U8 u8EQValue, E_MUX_INPUTPORT enInputPortType);
INTERFACE void MDrv_HDMI_Audio_MUTE_Enable(MS_U16 u16MuteEvent, MS_U16 u16MuteMask);
INTERFACE void MDrv_HDMI_Audio_Status_Clear(void);

INTERFACE MS_HDMI_AR_TYPE MDrv_HDMI_Get_AspectRatio(MS_BOOL bEn_PAR, MS_BOOL bEn_AFAR);
INTERFACE MS_HDMI_AR_TYPE MDrv_HDMI_Get_ActiveFormat_AspectRatio(MS_BOOL bEn_AFAR);
INTERFACE EN_AVI_INFOFRAME_VERSION MDrv_HDMI_Get_AVIInfoFrameVer(void);

INTERFACE MS_U8 MDrv_HDMI_err_status_update(MS_U8 u8value, MS_BOOL bread);
INTERFACE void MDrv_HDMI_Get_PollingStatus(HDMI_POLLING_STATUS_t **pStatus);
INTERFACE MS_BOOL MDrv_HDMI_IsHDMI_Mode(void);

INTERFACE void MDrv_HDMI_Get_MID_info(MS_U8 * pu8ManufactorData);
INTERFACE MS_BOOL MDrv_HDMI_get_packet_value(MS_HDMI_PACKET_STATE_t u8state, MS_U8 u8byte_idx, MS_U8 *pValue);

//DVI
INTERFACE MS_U16 MDrv_DVI_ChannelPhaseStatus( MS_DVI_CHANNEL_TYPE u8Channel);
INTERFACE void MDrv_DVI_HF_adjust( void );
INTERFACE void MDrv_DVI_SoftwareReset( MS_U16 u16Reset );
INTERFACE void mdrv_dvi_reset(void);
INTERFACE void MDrv_DVI_ClkPullLow(MS_BOOL bPullLow, E_MUX_INPUTPORT enInputPortType);
INTERFACE void MDrv_DVI_SwitchSrc(E_MUX_INPUTPORT enInputPortType);
INTERFACE MS_BOOL MDrv_HDMI_SetForClockLessThan70Mhz( E_MUX_INPUTPORT InputSourceType  );
INTERFACE void MDrv_HDMI_dvi_adjust( MS_BOOL bClockLessThan70MHz);
INTERFACE void MDrv_HDMI_SetUpdatePhaseLineCount(INPUT_SOURCE_TYPE_t enInputSourceType);
INTERFACE MS_U16 MDrv_HDMI_GetTMDSFreq(void);


//HDCP
INTERFACE void MDrv_HDCP_Enable(MS_BOOL bEnable);
INTERFACE MS_BOOL MDrv_HDMI_SetHdcpEnable(E_MUX_INPUTPORT enInputPortType,MS_BOOL bEnable);
INTERFACE void MDrv_HDCP_ClearStatus( void );
INTERFACE void MDrv_HDCP_initproductionkey( MS_U8 * pu8HdcpKeyData );
INTERFACE MS_HDCP_STATUS_INFO_t* MDrv_HDCP_GetStatus(void );
INTERFACE void MDrv_HDCP_Vsync_end_en(MS_BOOL bStable);

//Audio
INTERFACE void MDrv_HDMI_audio_output( MS_BOOL bEnable );
INTERFACE MS_U8 MDrv_HDMI_audio_cp_hdr_info(void);
INTERFACE MS_U8 MDrv_HDMI_audio_channel_status(MS_U8 u8byte);


// Debug
/**
*   HDMI Info
*/
typedef struct __attribute__((packed))
{
    MS_U8 u8SupportPorts;
}MS_HDMI_INFO;

/**
*   HDMI Status
*/
typedef struct __attribute__((packed))
{
    MS_BOOL bIsInitialized;
    MS_BOOL bIsRunning;
}MS_HDMI_Status;

INTERFACE HDMI_Ret_Value  MDrv_HDMI_GetLibVer(const MSIF_Version **ppVersion);      ///< Get version (without Mutex protect)
INTERFACE HDMI_Ret_Value  MDrv_HDMI_GetInfo(MS_HDMI_INFO *pInfo);
INTERFACE HDMI_Ret_Value  MDrv_HDMI_GetStatus(MS_HDMI_Status *pStatus);
INTERFACE MS_BOOL MDrv_HDMI_Get_AVIInfoActiveInfoPresent(void);

// DDC Ram
INTERFACE void MDrv_HDMI_READ_DDCRAM(XC_DDCRAM_PROG_INFO *pstDDCRam_Info, MS_U32 u32SizeOfInfo);
INTERFACE void MDrv_HDMI_PROG_DDCRAM(XC_DDCRAM_PROG_INFO *pstDDCRam_Info, MS_U32 u32SizeOfInfo);
INTERFACE MS_U8 MDrv_HDMI_Get_Pixel_Repetition(void);

//#if defined(CHIP_T8) || defined(CHIP_J2) // HDMI 1.4 new feature
INTERFACE void MDrv_HDMI_3D_4Kx2K_Process(void);
INTERFACE void MDrv_HDMI_AVG_ScaleringDown(MS_BOOL bTrue);
INTERFACE MS_BOOL MDrv_HDMI_Check4K2K(void);
INTERFACE E_HDMI_ADDITIONAL_VIDEO_FORMAT MDrv_HDMI_Check_Additional_Format(void);
INTERFACE E_XC_3D_INPUT_MODE MDrv_HDMI_Get_3D_Structure(void);
INTERFACE E_HDMI_3D_EXT_DATA_T MDrv_HDMI_Get_3D_Ext_Data(void);
INTERFACE void MDrv_HDMI_Get_3D_Meta_Field(sHDMI_3D_META_FIELD *pdata);
INTERFACE MS_U8 MDrv_HDMI_Get_VIC_Code(void);
INTERFACE E_HDMI_VIC_4Kx2K_CODE MDrv_HDMI_Get_4Kx2K_VIC_Code(void);
// HDMI ARC pin control
INTERFACE void MDrv_HDMI_ARC_PINControl(E_MUX_INPUTPORT enInputPortType, MS_BOOL bEnable, MS_BOOL bDrivingHigh);
INTERFACE void MDrv_DVI_Software_Reset(E_MUX_INPUTPORT enInputPortType, MS_U16 u16Reset);
INTERFACE MS_BOOL MDrv_HDMI_CheckHDMI20_Setting(E_MUX_INPUTPORT enInputPortType);
INTERFACE void MDrv_HDMI_StablePolling(void);
INTERFACE MS_U8 MDrv_HDMI_GetSourceVersion(E_MUX_INPUTPORT enInputPortType);
INTERFACE void SYMBOL_WEAK MDrv_HDMI_Set5VDetectGPIOSelect(MS_U32 ul5VDetectGPIOIndex);
INTERFACE MS_BOOL SYMBOL_WEAK MDrv_HDMI_GetDEStableStatus(E_MUX_INPUTPORT enInputPortType);
INTERFACE MS_BOOL SYMBOL_WEAK MDrv_HDMI_CheckHDCP14(void);
INTERFACE E_HDMI_HDCP_STATE MDrv_HDMI_CheckHDCPState(E_MUX_INPUTPORT enInputPortType);
// HDMI common function. If you want to add new function, you shold modify MDrv_HDMI_Ctrl to support new feature.
INTERFACE MS_BOOL SYMBOL_WEAK MDrv_HDMI_Ctrl(MS_U32 u32Cmd, void *pBuf, MS_U32 u32BufSize);
INTERFACE MS_U16 MDrv_HDMI_GetDataInfo(E_HDMI_GET_DATA_INFO enInfo);

//#endif

// fast boot
INTERFACE MS_U32 MDrv_HDMI_SetPowerState(EN_POWER_MODE enPowerState);

/*************************** HDCP22 ***************************/
INTERFACE void MDrv_HDCP22_FillCipherKey(MS_U8 pucPortIdx, MS_U8 * pucRiv,MS_U8 * pucSessionKey);
INTERFACE void MDrv_HDCP22_InitCBFunc(HDCP22_Recv_CBF pCBFunc, void* pContext);
INTERFACE void MDrv_HDCP22_PortInit(MS_U8 ucPortIdx);
INTERFACE MS_BOOL MDrv_HDCP22_PollingReadDone(MS_U8 ucPortIdx);
INTERFACE void MDrv_HDCP22_EnableCipher(MS_U8 ucPortType, MS_U8 ucPortIdx, MS_BOOL bIsEnable);
INTERFACE void MDrv_HDCP22_SendMsg(MS_U8 ucPortType, MS_U8 ucPortIdx, MS_U8* pucData, MS_U32 dwDataLen, void* pDummy);
INTERFACE void MDrv_HDCP22_Handler(MS_U8 ucPortIdx);
/*************************** HDCP22 ***************************/

#if 0
INTERFACE MS_U8 u8PSDVIStableCount[4];
void MDrv_HDMI_ImmeswitchPS_Handler(void);
void MDrv_DVI_ForceAllPortsEnterPS(void);
#endif
#undef INTERFACE


#ifdef __cplusplus
}
#endif

#endif /* MDRV_HDMI_H */

