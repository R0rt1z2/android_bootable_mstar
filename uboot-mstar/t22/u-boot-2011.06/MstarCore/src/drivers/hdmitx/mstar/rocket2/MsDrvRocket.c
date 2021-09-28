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
/// file    mhal_lth.cpp
/// @brief  HDMITx HDCP HAL
///////////////////////////////////////////////////////////////////////////////////////////////////
#define _MHAL_LTH_C_

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------

#include <common.h>
#include <command.h>
#include "MsCommon.h"
#include <MsDebug.h>
#include <apiSWI2C.h>
#include <apiPNL.h>
#include <CusConfig.h>
#include "MsDrvRocket.h"
#include "MsBoot.h"
#include <drvGPIO.h>
#include <apiHDMITx.h>
#include <bootlogo/MsPoolDB.h>
#include <MsUtility.h>
#include <malloc.h>
#define MAILBOX_REG_BASE           (0x1200)




#define HDMITX_INTERRUPT HDMITX_INT

MS_BOOL _bSuspend = FALSE;
MS_BOOL _bHDMITx_Running = FALSE;

#if (HDMITX_IDAC_ENABLE == 1)
MS_BOOL _bIDAC_En = TRUE;
#else
MS_BOOL _bIDAC_En = FALSE;
#endif
#define SWI2C_CMD_DATA_SUM          (7)
#define SWI2C_I2C_BUS_NUM           (3)
#define SWI2C_DDC2BI_MODE_ADDR      0xA2
#define SWI2C_I2C_SUB_ADDR          (0)
#if(ENABLE_MSTAR_MASERATI == 1)
#define ROCKET2_GPIO_ACK            PAD_GPIO_PM5
#else
#define ROCKET2_GPIO_ACK            PAD_GPIO_PM2
#endif

#define SWI2C_PANEL_INIT_AC_DELAY   750
#define SWI2C_PANEL_INIT_DC_DELAY   800


#define ROCKET_I2C_WR_SLAVE_ID (SWI2C_I2C_SUB_ADDR<<8 | SWI2C_DDC2BI_MODE_ADDR)


static SWI2C_BusCfg Rocket2_I2cBusCfg[SWI2C_I2C_BUS_NUM]=
{

    // Bus-0
    {PAD_GPIO28, PAD_GPIO29, 100}, //IS_SW_I2C  /SCL_PAD /SDA_PAD /Delay
    // Bus-1
    {PAD_TGPIO2, PAD_TGPIO3, 100},   //IS_SW_I2C  /SCL_PAD /SDA_PAD /Delay
    // Bus-2
    {PAD_DDCR_CK, PAD_DDCR_DA, 100}, //IS_SW_I2C  /SCL_PAD /SDA_PAD /Delay
    // Others, add other bus here
};

#define PARSING_IN_COLOR(x)    (\
    x == E_MAPI_RAPTORS_HDMITX_COLORS_INPUT_RGB ? "RGB" : \
    x == E_MAPI_RAPTORS_HDMITX_COLORS_INPUT_YUV ? "YUV" : \
                                     "unknown")

#define PARSING_OUT_COLOR(x)    (\
    x == E_MAPI_RAPTORS_HDMITX_COLORS_OUTPUT_RGB_444 ? "RGB444" : \
    x == E_MAPI_RAPTORS_HDMITX_COLORS_OUTPUT_YUV_422 ? "YUV422" : \
    x == E_MAPI_RAPTORS_HDMITX_COLORS_OUTPUT_YUV_444 ? "YUV444" : \
    x == E_MAPI_RAPTORS_HDMITX_COLORS_OUTPUT_YUV_420? "YUV444" : \
                                     "unknown")


/// Mailbox Command Type Index
typedef enum
{
    E_MAPI_MAILBOX_CONFIQ_VIDEO        = 0x01,
    E_MAPI_MAILBOX_CONFIQ_AUDIO        = 0x02,
    E_MAPI_MAILBOX_AVMUTE_TIMING_RESET = 0x03,
    E_MAPI_MAILBOX_QUERY_EDID_INFO     = 0x04,
    E_MAPI_MAILBOX_REPORT_EDID_INFO    = 0x05,
    E_MAPI_MAILBOX_ACK_STATUS_RESPOND  = 0x06,
    E_MAPI_MAILBOX_DEBUG               = 0x07,
    E_MAPI_MAILBOX_COMBINE_COMMOND     = 0x08,
} EN_MAPI_RAPTORS_MAILBOX_COMMAND_TYPE; // mailbox command type

/// Mailbox Timing Type
typedef enum
{
    E_MAPI_RAPTORS_HDMITX_TIMING_720x576p_50Hz   = 0x00,
    E_MAPI_RAPTORS_HDMITX_TIMING_720x480p_60Hz   = 0x01,
    E_MAPI_RAPTORS_HDMITX_TIMING_1280x720p_50Hz  = 0x02,
    E_MAPI_RAPTORS_HDMITX_TIMING_1280x720p_60Hz  = 0x03,
    E_MAPI_RAPTORS_HDMITX_TIMING_1920x1080p_24Hz = 0x04,
    E_MAPI_RAPTORS_HDMITX_TIMING_1920x1080i_50Hz = 0x05,
    E_MAPI_RAPTORS_HDMITX_TIMING_1920x1080i_60Hz = 0x06,
    E_MAPI_RAPTORS_HDMITX_TIMING_1920x1080p_50Hz = 0x07,
    E_MAPI_RAPTORS_HDMITX_TIMING_1920x1080p_60Hz = 0x08,
    E_MAPI_RAPTORS_HDMITX_TIMING_4K2Kp_25Hz      = 0x09,
    E_MAPI_RAPTORS_HDMITX_TIMING_4K2Kp_30Hz      = 0x0A,
    E_MAPI_RAPTORS_HDMITX_TIMING_4K2Kp_50Hz      = 0x0B,
    E_MAPI_RAPTORS_HDMITX_TIMING_4K2Kp_60Hz      = 0x0C,
    E_MAPI_RAPTORS_HDMITX_TIMING_4096x2160_50Hz  = 0x0D,
    E_MAPI_RAPTORS_HDMITX_TIMING_4096x2160_60Hz  = 0x0E,
    E_MAPI_RAPTORS_HDMITX_TIMING_4K2Kp_24Hz      = 0x0F,
    E_MAPI_RAPTORS_HDMITX_TIMING_4096x2160p_24Hz = 0x10,
    E_MAPI_RAPTORS_HDMITX_TIMING_4096x2160p_25Hz = 0x11,
    E_MAPI_RAPTORS_HDMITX_TIMING_4096x2160p_30Hz = 0x12,
    E_MAPI_RAPTORS_HDMITX_TIMING_4K2Kp_60Hz_RB   = 0x15,
    E_MAPI_RAPTORS_HDMITX_TIMING_800x600p_60Hz   = 0x80,
    E_MAPI_RAPTORS_HDMITX_TIMING_848x480p_60Hz   = 0x81,
    E_MAPI_RAPTORS_HDMITX_TIMING_1024x768p_60Hz  = 0x82,
    E_MAPI_RAPTORS_HDMITX_TIMING_1280x768p_60Hz  = 0x83,
    E_MAPI_RAPTORS_HDMITX_TIMING_1280x800p_60Hz  = 0x84,
    E_MAPI_RAPTORS_HDMITX_TIMING_1280x960p_60Hz  = 0x85,
    E_MAPI_RAPTORS_HDMITX_TIMING_1280x1024p_60Hz = 0x86,
    E_MAPI_RAPTORS_HDMITX_TIMING_1360x768p_60Hz  = 0x87,
    E_MAPI_RAPTORS_HDMITX_TIMING_1366x768p_60Hz  = 0x88,
    E_MAPI_RAPTORS_HDMITX_TIMING_1400x1050p_60Hz = 0x89,
    E_MAPI_RAPTORS_HDMITX_TIMING_1440x900p_60Hz  = 0x8A,
    E_MAPI_RAPTORS_HDMITX_TIMING_1600x900p_60Hz  = 0x8B,
    E_MAPI_RAPTORS_HDMITX_TIMING_1600x1200p_60Hz = 0x8C,
    E_MAPI_RAPTORS_HDMITX_TIMING_1680x1050p_60Hz = 0x8D,
    E_MAPI_RAPTORS_HDMITX_TIMING_1920x1200p_60Hz = 0x8E,
    E_MAPI_RAPTORS_HDMITX_TIMING_2048x1152p_60Hz = 0x8F,
    E_MAPI_RAPTORS_HDMITX_TIMING_MAX             = 0x90,
} EN_MAPI_RAPTORS_HDMITX_VIDEO_TIMING; // raptors timing type index

/// Mailbox Color Depth Type
typedef enum
{
    E_MAPI_RAPTORS_HDMITX_COLORS_NOID   = 0,
    E_MAPI_RAPTORS_HDMITX_COLORS_8bits  = 1,
    E_MAPI_RAPTORS_HDMITX_COLORS_10bits = 2,
    E_MAPI_RAPTORS_HDMITX_COLORS_12bits = 3,
} EN_MAPI_RAPTORS_HDMITX_COLOR_DEPTH; // raptors color type index

/// Mailbox Video Output Mode
typedef enum
{
    E_MAPI_RAPTORS_HDMITX_VIDEO_OUTPUT_DVI       = 0,
    E_MAPI_RAPTORS_HDMITX_VIDEO_OUTPUT_DVI_HDCP  = 1,
    E_MAPI_RAPTORS_HDMITX_VIDEO_OUTPUT_HDMI      = 2,
    E_MAPI_RAPTORS_HDMITX_VIDEO_OUTPUT_HDMI_HDCP = 3,
} EN_MAPI_RAPTORS_HDMITX_VIDEO_OUTPUT_TYPE; // raptors video output type index

/// Mailbox Input Color Type
typedef enum
{
    E_MAPI_RAPTORS_HDMITX_COLORS_INPUT_RGB = 0,
    E_MAPI_RAPTORS_HDMITX_COLORS_INPUT_YUV = 1,
} EN_MAPI_RAPTORS_HDMITX_INPUT_COLOR_TYPE; // raptors input color type index

/// Mailbox Output Color Type
typedef enum
{
    E_MAPI_RAPTORS_HDMITX_COLORS_OUTPUT_RGB_444 = 0,
    E_MAPI_RAPTORS_HDMITX_COLORS_OUTPUT_YUV_422 = 1,
    E_MAPI_RAPTORS_HDMITX_COLORS_OUTPUT_YUV_444 = 2,
    E_MAPI_RAPTORS_HDMITX_COLORS_OUTPUT_YUV_420 = 3,
} EN_MAPI_RAPTORS_HDMITX_OUTPUT_COLOR_TYPE; // raptors output color type index

/// Mailbox 3D VS Information Type
typedef enum
{
    E_MAPI_RAPTORS_HDMITX_VIDEO_3D_FramePacking     = 0,
    E_MAPI_RAPTORS_HDMITX_VIDEO_3D_FieldAlternative = 1,
    E_MAPI_RAPTORS_HDMITX_VIDEO_3D_LineAlternative  = 2,
    E_MAPI_RAPTORS_HDMITX_VIDEO_3D_SidebySide_FULL  = 3,
    E_MAPI_RAPTORS_HDMITX_VIDEO_3D_L_Dep            = 4,
    E_MAPI_RAPTORS_HDMITX_VIDEO_3D_L_Dep_Graphic_Dep= 5,
    E_MAPI_RAPTORS_HDMITX_VIDEO_3D_TopandBottom     = 6,
    E_MAPI_RAPTORS_HDMITX_VIDEO_3D_SidebySide_Half  = 8,
    E_MAPI_RAPTORS_HDMITX_VIDEO_3D_Not_in_Use       = 15,
} EN_MAPI_RAPTORS_HDMITX_VIDEO_3D_STRUCTURE_TYPE; // raptors 3D VS info type index

/// Mailbox 4K2K VS Information Type
typedef enum
{
    E_MAPI_RAPTORS_HDMITX_VIDEO_4K2K_Reserved    = 0, // 0x00
    E_MAPI_RAPTORS_HDMITX_VIDEO_4K2K_30Hz        = 1, // 0x01
    E_MAPI_RAPTORS_HDMITX_VIDEO_4K2K_25Hz        = 2, // 0x02
    E_MAPI_RAPTORS_HDMITX_VIDEO_4K2K_24Hz        = 3, // 0x03
    E_MAPI_RAPTORS_HDMITX_VIDEO_4K2K_24Hz_SMPTE  = 4, // 0x04
} EN_MAPI_RAPTORS_HDMITX_VIDEO_4K2K_VIC;// raptors 4K2K VS info type index


/// Mailbox Audio Frequency Type
typedef enum
{
    E_MAPI_RAPTORS_HDMITX_VIDEO_3x3_MATRIX_Auto         = 0,
    E_MAPI_RAPTORS_HDMITX_VIDEO_3x3_MATRIX_Off          = 1,
    E_MAPI_RAPTORS_HDMITX_VIDEO_3x3_MATRIX_R2Y_SD_Limit = 2,
    E_MAPI_RAPTORS_HDMITX_VIDEO_3x3_MATRIX_R2Y_SD_Full  = 3,
    E_MAPI_RAPTORS_HDMITX_VIDEO_3x3_MATRIX_R2Y_HD_Limit = 4,
    E_MAPI_RAPTORS_HDMITX_VIDEO_3x3_MATRIX_R2Y_HD_Full  = 5,
} EN_MAPI_RAPTORS_HDMITX_VIDEO_3x3_MATRIX_TYPE; // raptors 3x3 matrix type index
typedef enum
{
    E_MAPI_RAPTORS_HDMITX_AUDIO_32KHz   = 0, // 0x00
    E_MAPI_RAPTORS_HDMITX_AUDIO_44KHz   = 1, // 0x01
    E_MAPI_RAPTORS_HDMITX_AUDIO_48KHz   = 2, // 0x02
    E_MAPI_RAPTORS_HDMITX_AUDIO_88KHz   = 3, // 0x03
    E_MAPI_RAPTORS_HDMITX_AUDIO_96KHz   = 4, // 0x04
    E_MAPI_RAPTORS_HDMITX_AUDIO_176KHz  = 5, // 0x05
    E_MAPI_RAPTORS_HDMITX_AUDIO_192KHz  = 6, // 0x06
} EN_MAPI_RAPTORS_HDMITX_AUDIO_FREQUENCY_TYPE;// raptors audio frequency index

/// Mailbox Audio Channel Number
typedef enum
{
    E_MAPI_RAPTORS_HDMITX_AUDIO_CH_2    = 2, // 2 channels
    E_MAPI_RAPTORS_HDMITX_AUDIO_CH_8    = 8, // 8 channels
} EN_MAPI_RAPTORS_HDMITX_AUDIO_CHANNEL_COUNT;// raptors audio channel number index

/// Mailbox Audio Code Type
typedef enum
{
    E_MAPI_RAPTORS_HDMITX_AUDIO_PCM     = 0, // PCM
    E_MAPI_RAPTORS_HDMITX_AUDIO_NONPCM  = 1, // non-PCM
} EN_MAPI_RAPTORS_HDMITX_AUDIO_CODING_TYPE;// raptors audio code type index

/// Mailbox Audio Channel Number
typedef enum
{
    E_MAPI_RAPTORS_HDMITX_AUDIO_SPDIF   = 0, // SPDIF
    E_MAPI_RAPTORS_HDMITX_AUDIO_I2S     = 1, // I2S
} EN_MAPI_RAPTORS_HDMITX_AUDIO_SOURCE_TYPE;// raptors audio source type index

/// Mailbox Audio Channel Number
typedef enum
{
    E_MAPI_RAPTORS_HDMITX_AUDIO_FORMAT_PCM   = 0, // PCM
    E_MAPI_RAPTORS_HDMITX_AUDIO_FORMAT_DSD   = 1, // DSD
    E_MAPI_RAPTORS_HDMITX_AUDIO_FORMAT_HBR   = 2, // HBR
} EN_MAPI_RAPTORS_HDMITX_AUDIO_SOURCE_FORMAT;// raptors audio source format index

/// Mailbox Video Mute
typedef enum
{
    E_MAPI_RAPTORS_HDMITX_VIDEO_MUTE_DISABLE   = 0, // disable
    E_MAPI_RAPTORS_HDMITX_VIDEO_MUTE_ENABLE    = 1, // enable
} EN_MAPI_RAPTORS_HDMITX_VIDEO_MUTE;// raptors video mute index

/// Mailbox Audio Mute
typedef enum
{
    E_MAPI_RAPTORS_HDMITX_AUDIO_MUTE_DISABLE   = 0, // disable
    E_MAPI_RAPTORS_HDMITX_AUDIO_MUTE_ENABLE    = 1, // enable
} EN_MAPI_RAPTORS_HDMITX_AUDIO_MUTE;// raptors audio mute index

/// Mailbox Audio Mute
typedef enum
{
    E_MAPI_RAPTORS_HDMITX_AVMUTE_DISABLE   = 0, // disable
    E_MAPI_RAPTORS_HDMITX_AVMUTE_ENABLE    = 1, // enable
} EN_MAPI_RAPTORS_HDMITX_AVMUTE;// raptors audio and video mute index

/// Mailbox Timing Change
typedef enum
{
    E_MAPI_RAPTORS_HDMITX_TIMING_CHANGE_DISABLE   = 0, // disable
    E_MAPI_RAPTORS_HDMITX_TIMING_CHANGE_ENABLE    = 1, // enable
} EN_MAPI_RAPTORS_HDMITX_TIMING_CHANGE;// raptors timing reset index

/// Mailbox Query Type
typedef enum
{
    E_MAPI_RAPTORS_QUERY_4K2K_VIC             = 1, // 0x01
    E_MAPI_RAPTORS_QUERY_3D_STRUCTURE         = 2, // 0x02
    E_MAPI_RAPTORS_QUERY_AUDIO_DESCRIPTOR_NUM = 3, // 0x03
    E_MAPI_RAPTORS_QUERY_AUDIO_DESCRIPTOR     = 4, // 0x04
    E_MAPI_RAPTORS_QUERY_VIDEO_DESCRIPTOR_NUM = 5, // 0x05
    E_MAPI_RAPTORS_QUERY_VIDEO_DESCRIPTOR     = 6, // 0x06
    E_MAPI_RAPTORS_QUERY_EDID_DATA            = 7, // 0x07
    E_MAPI_RAPTORS_QUERY_RX_EDID_INFO         = 0x08, // 0x08
    E_MAPI_RAPTORS_QUERY_COLOR_FORMAT         = 0x09, // 0x09
    E_MAPI_RAPTORS_QUERY_HW_INFO              = 0x0A, // 0x0A
    E_MAPI_RAPTORS_QUERY_KSV_BSTATUS          = 0x0B, // 0x0B
} EN_MAPI_RAPTORS_QUERY_TYPE;// raptors command 0x04 0x05 query type index


/// Mailbox 3D Timing Type for Command 0x04 Query
typedef enum
{
    E_MAPI_RAPTORS_HDMITX_QUERY_3DTIMING_1280x720p_50Hz  = 0x00,
    E_MAPI_RAPTORS_HDMITX_QUERY_3DTIMING_1280x720p_60Hz  = 0x01,
    E_MAPI_RAPTORS_HDMITX_QUERY_3DTIMING_1920x1080p_24Hz = 0x02,
    E_MAPI_RAPTORS_HDMITX_QUERY_3DTIMING_1920x1080i_50Hz = 0x03,
    E_MAPI_RAPTORS_HDMITX_QUERY_3DTIMING_1920x1080i_60Hz = 0x04,
    E_MAPI_RAPTORS_HDMITX_QUERY_3DTIMING_1920x1080p_25Hz = 0x05,
    E_MAPI_RAPTORS_HDMITX_QUERY_3DTIMING_1920x1080p_50Hz = 0x06,
    E_MAPI_RAPTORS_HDMITX_QUERY_3DTIMING_1920x1080p_60Hz = 0x07,
    E_MAPI_RAPTORS_HDMITX_QUERY_3DTIMING_4K2Kp_25Hz      = 0x08,
    E_MAPI_RAPTORS_HDMITX_QUERY_3DTIMING_4K2Kp_30Hz      = 0x09,
    E_MAPI_RAPTORS_HDMITX_QUERY_3DTIMING_4K2Kp_50Hz      = 0x0A,
    E_MAPI_RAPTORS_HDMITX_QUERY_3DTIMING_4K2Kp_60Hz      = 0x0B,
    E_MAPI_RAPTORS_HDMITX_QUERY_3DTIMING_MAX             = 0x0C,
} EN_MAPI_RAPTORS_HDMITX_QUERY_3DTIMING; // raptors 3D timing type index for command 0x04 query & 0x05 report

/// Mailbox 3D VS Information Type for Command 0x05 Report
typedef enum
{
    E_MAPI_RAPTORS_HDMITX_VIDEO_REPORT_3D_FramePacking               = 0x0001,
    E_MAPI_RAPTORS_HDMITX_VIDEO_REPORT_3D_FieldAlternative           = 0x0002,
    E_MAPI_RAPTORS_HDMITX_VIDEO_REPORT_3D_LineAlternative            = 0x0004,
    E_MAPI_RAPTORS_HDMITX_VIDEO_REPORT_3D_SidebySide_FULL            = 0x0008,
    E_MAPI_RAPTORS_HDMITX_VIDEO_REPORT_3D_L_Dep                      = 0x0010,
    E_MAPI_RAPTORS_HDMITX_VIDEO_REPORT_3D_L_Dep_Graphic_Dep          = 0x0020,
    E_MAPI_RAPTORS_HDMITX_VIDEO_REPORT_3D_TopandBottom               = 0x0040,
    E_MAPI_RAPTORS_HDMITX_VIDEO_REPORT_3D_SidebySide_Half_Horizontal = 0x0080,
    E_MAPI_RAPTORS_HDMITX_VIDEO_REPORT_3D_SidebySide_Half_Quincunx   = 0x8000,
} EN_MAPI_RAPTORS_HDMITX_VIDEO_REPORT_3D_STRUCTURE_TYPE; // raptors 3D VS info type index

/// Mailbox Status Response
typedef enum
{
    E_MAPI_MAILBOX_REPORT_CONFIQ_VIDEO        = 0x01,
    E_MAPI_MAILBOX_REPORT_CONFIQ_AUDIO        = 0x02,
    E_MAPI_MAILBOX_REPORT_AVMUTE_TIMING_RESET = 0x03,
    E_MAPI_MAILBOX_REPORT_REPORT_EDID_INFO    = 0x04,
    E_MAPI_MAILBOX_REPORT_DEBUG               = 0x07,
} EN_MAPI_RAPTORS_MAILBOX_RECIEVED_COMMAND_TYPE; // mailbox received command type

typedef enum
{
    E_MAPI_MAILBOX_REPORT_SUCCESS           = 0x00,
    E_MAPI_MAILBOX_REPORT_FAIL              = 0x01,
    E_MAPI_MAILBOX_REPORT_PARAMETER_INVALID = 0x02,
    E_MAPI_MAILBOX_REPORT_COMMAND_INVALID   = 0x03,
} EN_MAPI_RAPTORS_MAILBOX_STATUS_RESPONSE_TYPE; // mailbox received status response type

/// Mailbox Debug Type
typedef enum
{
    E_MAPI_MAILBOX_DEBUG_TEST_PATTERN     = 0x00,
    E_MAPI_MAILBOX_DEBUG_DEBUG_MESSAGE    = 0x01,
    E_MAPI_MAILBOX_DEBUG_FREE_RUN         = 0x02,
    E_MAPI_MAILBOX_DEBUG_LOCK_STATUS_LPLL = 0x03,
    E_MAPI_MAILBOX_DEBUG_STATUS_HTT_VTT   = 0x04,
} EN_MAPI_RAPTORS_MAILBOX_DEBUG_TYPE; // mailbox received command type

typedef enum
{
    E_MAPI_HDMITX_COLOR_RGB444 = 0,
    E_MAPI_HDMITX_COLOR_YUV422 = 1,
    E_MAPI_HDMITX_COLOR_YUV444 = 2,
    E_MAPI_HDMITX_COLOR_YUV420 = 3,
    E_MAPI_HDMITX_COLOR_MAX = 3,
} EN_MAPI_HDMITX_COLOR_TYPE;

typedef enum
{
    E_MAPI_HDMITX_COLOR_DEPTH_8BITS = 0,
    E_MAPI_HDMITX_COLOR_DEPTH_10BITS,
    E_MAPI_HDMITX_COLOR_DEPTH_12BITS,
    E_MAPI_HDMITX_COLOR_DEPTH_16BITS,
    E_MAPI_HDMITX_COLOR_DEPTH_AUTO,
    E_MAPI_HDMITX_COLOR_DEPTH_MAX,
} EN_MAPI_HDMITX_COLOR_DEPTH_TYPE;

typedef enum
{
    E_MAPI_HDMITX_VIDEO_HDMI = 0,
    E_MAPI_HDMITX_VIDEO_HDMI_HDCP,
    E_MAPI_HDMITX_VIDEO_DVI,
    E_MAPI_HDMITX_VIDEO_DVI_HDCP,
    E_MAPI_HDMITX_VIDEO_MAX,
} EN_MAPI_HDMITX_OUTPUT_MODE;

//audio
typedef enum
{
    E_MAPI_HDMITX_AUDIO_FREQ_NO_SIG  = 0,
    E_MAPI_HDMITX_AUDIO_32K          = 1,
    E_MAPI_HDMITX_AUDIO_44K          = 2,
    E_MAPI_HDMITX_AUDIO_48K          = 3,
    E_MAPI_HDMITX_AUDIO_88K          = 4,
    E_MAPI_HDMITX_AUDIO_96K          = 5,
    E_MAPI_HDMITX_AUDIO_176K         = 6,
    E_MAPI_HDMITX_AUDIO_192K         = 7,
    E_MAPI_HDMITX_AUDIO_FREQ_MAX_NUM = 8,
} EN_MAPI_HDMITX_AUDIO_FREQUENCY;


typedef enum
{
    E_MAPI_HDMITX_AUDIO_CH_2  = 2, // 2 channels
    E_MAPI_HDMITX_AUDIO_CH_8  = 8, // 8 channels
} EN_MAPI_HDMITX_AUDIO_CHANNEL_COUNT;


typedef enum
{
    E_MAPI_HDMITX_AUDIO_PCM        = 0, // PCM
    E_MAPI_HDMITX_AUDIO_NONPCM     = 1, // non-PCM
} EN_MAPI_HDMITX_AUDIO_CODING_TYPE;


typedef enum
{
    E_MAPI_HDMITX_AUDIO_SPDIF,
    E_MAPI_HDMITX_AUDIO_I2S,
    E_MAPI_HDMITX_AUDIO_NONE,
} EN_MAPI_HDMITX_AUDIO_TYPE;

typedef enum
{
    E_MAPI_HDMITX_AUDIO_FORMAT_PCM   = 0,
    E_MAPI_HDMITX_AUDIO_FORMAT_DSD   = 1,
    E_MAPI_HDMITX_AUDIO_FORMAT_HBR   = 2,
    E_MAPI_HDMITX_AUDIO_FORMAT_NA    = 3,
} EN_MAPI_HDMITX_AUDIO_SOURCE_FORMAT;
//end audio

typedef enum
{
    E_MAPI_RAPTORS_HDMITX_QUERY_COLORFMTTIMING_720x576p_50Hz   = 0x00,
    E_MAPI_RAPTORS_HDMITX_QUERY_COLORFMTTIMING_720x480p_60Hz   = 0x01,
    E_MAPI_RAPTORS_HDMITX_QUERY_COLORFMTTIMING_1280x720p_50Hz  = 0x02,
    E_MAPI_RAPTORS_HDMITX_QUERY_COLORFMTTIMING_1280x720p_60Hz  = 0x03,
    E_MAPI_RAPTORS_HDMITX_QUERY_COLORFMTTIMING_1920x1080p_24Hz = 0x04,
    E_MAPI_RAPTORS_HDMITX_QUERY_COLORFMTTIMING_1920x1080i_50Hz = 0x05,
    E_MAPI_RAPTORS_HDMITX_QUERY_COLORFMTTIMING_1920x1080i_60Hz = 0x06,
    E_MAPI_RAPTORS_HDMITX_QUERY_COLORFMTTIMING_1920x1080p_50Hz = 0x07,
    E_MAPI_RAPTORS_HDMITX_QUERY_COLORFMTTIMING_1920x1080p_60Hz = 0x08,
    E_MAPI_RAPTORS_HDMITX_QUERY_COLORFMTTIMING_4K2Kp_25Hz      = 0x09,
    E_MAPI_RAPTORS_HDMITX_QUERY_COLORFMTTIMING_4K2Kp_30Hz      = 0x0A,
    E_MAPI_RAPTORS_HDMITX_QUERY_COLORFMTTIMING_4K2Kp_50Hz      = 0x0B,
    E_MAPI_RAPTORS_HDMITX_QUERY_COLORFMTTIMING_4K2Kp_60Hz      = 0x0C,
    E_MAPI_RAPTORS_HDMITX_QUERY_COLORFMTTIMING_4096x2160p_50Hz = 0x0D,
    E_MAPI_RAPTORS_HDMITX_QUERY_COLORFMTTIMING_4096x2160p_60Hz = 0x0E,
    E_MAPI_RAPTORS_HDMITX_QUERY_COLORFMTTIMING_1920x1080p_25Hz = 0x0F,
    E_MAPI_RAPTORS_HDMITX_QUERY_COLORFMTTIMING_1920x1080p_30Hz = 0x10,
    E_MAPI_RAPTORS_HDMITX_QUERY_COLORFMTTIMING_4K2Kp_24Hz      = 0x11,
    E_MAPI_RAPTORS_HDMITX_QUERY_COLORFMTTIMING_4096x2160p_24Hz = 0x12,
    E_MAPI_RAPTORS_HDMITX_QUERY_COLORFMTTIMING_4096x2160p_25Hz = 0x13,
    E_MAPI_RAPTORS_HDMITX_QUERY_COLORFMTTIMING_4096x2160p_30Hz = 0x14,
    E_MAPI_RAPTORS_HDMITX_QUERY_COLORFMTTIMING_MAX             = 0x15,
} EN_MAPI_RAPTORS_HDMITX_QUERY_COLORFMTTIMING; // raptors 3D timing type index for command 0x04 query & 0x05 report

//Global Variables
EN_MAPI_HDMITX_TIMING_TYPE genMAPI_HDMITxTming = E_MAPI_HDMITX_TIMING_1080_60P;

EN_MAPI_RAPTORS_HDMITX_VIDEO_OUTPUT_TYPE          genHDMITX_Mode =E_MAPI_RAPTORS_HDMITX_VIDEO_OUTPUT_HDMI;
EN_MAPI_RAPTORS_HDMITX_INPUT_COLOR_TYPE   genHDMITx_InColor = E_MAPI_RAPTORS_HDMITX_COLORS_INPUT_RGB;
EN_MAPI_RAPTORS_HDMITX_OUTPUT_COLOR_TYPE   genHDMITx_OutColor = E_MAPI_RAPTORS_HDMITX_COLORS_OUTPUT_YUV_422;
EN_MAPI_RAPTORS_HDMITX_COLOR_DEPTH genHDMITx_ColorDepth = E_MAPI_RAPTORS_HDMITX_COLORS_NOID;

EN_MAPI_RAPTORS_HDMITX_AUDIO_FREQUENCY_TYPE      genAudio_Freq = E_MAPI_RAPTORS_HDMITX_AUDIO_48KHz;
EN_MAPI_RAPTORS_HDMITX_AUDIO_CHANNEL_COUNT  genAudio_Ch_Cnt = E_MAPI_RAPTORS_HDMITX_AUDIO_CH_2;
EN_MAPI_RAPTORS_HDMITX_AUDIO_CODING_TYPE    genAudio_CodeType = E_MAPI_RAPTORS_HDMITX_AUDIO_PCM;
EN_MAPI_RAPTORS_HDMITX_AUDIO_SOURCE_FORMAT  genAudio_SrcFmt = E_MAPI_RAPTORS_HDMITX_AUDIO_FORMAT_PCM;
EN_MAPI_RAPTORS_HDMITX_AUDIO_SOURCE_TYPE    genAudio_SrcType =  E_MAPI_RAPTORS_HDMITX_AUDIO_I2S;
EN_MAPI_RAPTORS_HDMITX_VIDEO_TIMING         genHDMITx_VideoTiming = E_MAPI_RAPTORS_HDMITX_TIMING_1920x1080p_60Hz;
EN_MAPI_RAPTORS_HDMITX_VIDEO_3D_STRUCTURE_TYPE   genHDMITx_3D = E_MAPI_RAPTORS_HDMITX_VIDEO_3D_Not_in_Use;
EN_MAPI_RAPTORS_HDMITX_VIDEO_4K2K_VIC       genHDMITx_4K2K_VIC = E_MAPI_RAPTORS_HDMITX_VIDEO_4K2K_Reserved;
EN_MAPI_RAPTORS_HDMITX_VIDEO_3x3_MATRIX_TYPE genHDMITX_3x3Matrix = E_MAPI_RAPTORS_HDMITX_VIDEO_3x3_MATRIX_R2Y_HD_Full;



/// Mailbox Data Struct For Command 0x01 Configure Video
typedef struct ST_MAILBOX_COMMAND_CONFIQ_VIDEO
{
    EN_MAPI_RAPTORS_MAILBOX_COMMAND_TYPE command_idx; /// val = E_MAPI_MAILBOX_CONFIQ_VIDEO

    /// length of the following data
    MS_U8 data_len; /// Bytes

    /// flags for actions
    MS_BOOL timing_present;       /// flag of setting timing
    MS_BOOL color_present;        /// flag of setting color flag
    MS_BOOL VSinfo_3D_present;    /// flag of setting 3D_VS flag
    MS_BOOL analog_present;       /// flag of setting current, pren2, precon, tenpre and ten
    MS_BOOL ext_present;          ////  flag of setting extend command
    MS_BOOL sc_3x3_matrix_present; ////flag of setting 3x3 matrix

    /// data
    EN_MAPI_RAPTORS_HDMITX_VIDEO_TIMING timing_idx;        /// timing type
    EN_MAPI_RAPTORS_HDMITX_COLOR_DEPTH color_depth;               /// color depth, 8bit, 10bit, 12bit
    EN_MAPI_RAPTORS_HDMITX_VIDEO_OUTPUT_TYPE output_mode;         /// video output mode, DVI or HDMI
    EN_MAPI_RAPTORS_HDMITX_INPUT_COLOR_TYPE in_color;             /// input color format, RGB or YUV
    EN_MAPI_RAPTORS_HDMITX_OUTPUT_COLOR_TYPE out_color;           /// output color format, RGB or YUV
    EN_MAPI_RAPTORS_HDMITX_VIDEO_3D_STRUCTURE_TYPE vs_3d;  /// 3d vs information
    EN_MAPI_RAPTORS_HDMITX_VIDEO_3x3_MATRIX_TYPE sc_3x3_matrix;          //3x3 matrix
    MS_U8 current;
    MS_BOOL pren2;
    MS_U8 precon;
    MS_U8 tenpre;
    MS_U8 ten;
}ST_MAILBOX_COMMAND_CONFIQ_VIDEO;// struct of configure video

/// Mailbox Data Struct For Command 0x02 Configure Audio
typedef struct ST_MAILBOX_COMMAND_CONFIQ_AUDIO
{

    EN_MAPI_RAPTORS_MAILBOX_COMMAND_TYPE command_idx; /// val = E_MAPI_MAILBOX_CONFIQ_AUDIO

    /// length of the following data
    MS_U8 data_len; /// Bytes

    /// flags for actions
    MS_BOOL inform_present;   /// flag of timing id, frequency, channel number and code type
    MS_BOOL source_present;   /// flag of timing id, source type
    MS_BOOL fmt_present;      /// flag of timing id, source format

    /// data
    EN_MAPI_RAPTORS_HDMITX_AUDIO_FREQUENCY_TYPE frequency;  /// audio frequency
    EN_MAPI_RAPTORS_HDMITX_AUDIO_CHANNEL_COUNT channel_num; /// audio channel number
    EN_MAPI_RAPTORS_HDMITX_AUDIO_CODING_TYPE code_type;     /// audio coding type
    EN_MAPI_RAPTORS_HDMITX_AUDIO_SOURCE_TYPE source_type;   /// audio source type
    EN_MAPI_RAPTORS_HDMITX_AUDIO_SOURCE_FORMAT source_fmt;  /// audio source format
}ST_MAILBOX_COMMAND_CONFIQ_AUDIO;// struct of configure audio

/// Mailbox Data Struct For Command 0x03 Video/Audio Mute and Timing Reset
typedef struct ST_MAILBOX_COMMAND_TIMING_CHANGE_AVMUTE
{

    EN_MAPI_RAPTORS_MAILBOX_COMMAND_TYPE command_idx; /// val = E_MAPI_MAILBOX_AVMUTE_TIMING_RESET

    /// length of the following data
    MS_U8 data_len; /// Bytes

    /// flags for actions
    MS_BOOL video_flag;   /// flag of video mute
    MS_BOOL audio_flag;   /// flag of audio mute
    MS_BOOL avmute_flag;  /// flag of audio and video mute
    MS_BOOL timing_flag;  /// flag of changing timing

    /// data
    EN_MAPI_RAPTORS_HDMITX_VIDEO_MUTE enVideo_mute;
    EN_MAPI_RAPTORS_HDMITX_AUDIO_MUTE enAudio_mute;
    EN_MAPI_RAPTORS_HDMITX_AVMUTE enAV_mute;
    EN_MAPI_RAPTORS_HDMITX_TIMING_CHANGE enTiming;
}ST_MAILBOX_COMMAND_TIMING_CHANGE_AVMUTE;// struct of Video/Audio Mute and Timing Reset

/// Mailbox Data Struct For Command 0x04 Query EDID Information
typedef struct ST_MAILBOX_COMMAND_QUERY_EDID_INFO
{

    EN_MAPI_RAPTORS_MAILBOX_COMMAND_TYPE command_idx; /// val = E_MAPI_MAILBOX_QUERY_EDID_INFO

    /// length of the following data
    MS_U8 data_len; /// Bytes

    /// query type
    EN_MAPI_RAPTORS_QUERY_TYPE query_type;

    /// data
    EN_MAPI_RAPTORS_HDMITX_QUERY_3DTIMING timing_3d;
    EN_MAPI_RAPTORS_HDMITX_QUERY_COLORFMTTIMING timing_colorfmt;
    MS_U8 des_startIdx;  // audio/video descriptor start index
    MS_U8 des_endIdx;    // audio/video descriptor end index
    MS_U8 bytes64_Idx;   // 64 bytes index
    MS_U8 edid_Idx;      // EDID block index
    MS_U8 KSV_B_Idx;      // KSV_B index
}ST_MAILBOX_COMMAND_QUERY_EDID_INFO;// struct of query edid info

/// Mailbox Data Struct For Command 0x05 Report EDID Information (Different Query Type Has Different Struct)
/// Query Type = 4K2K Vic, Query_Type = E_MAPI_RAPTORS_QUERY_4K2K_VIC 0x01
typedef struct ST_MAILBOX_COMMAND_REPORT_EDID_INFO_4K2KVIC
{

    EN_MAPI_RAPTORS_MAILBOX_COMMAND_TYPE command_idx; /// val = E_MAPI_MAILBOX_REPORT_EDID_INFO

    /// length of the following data
    MS_U8 data_len; /// Bytes

    /// query type
    EN_MAPI_RAPTORS_QUERY_TYPE query_type; /// val = E_MAPI_RAPTORS_QUERY_4K2K_VIC

    /// data
    MS_U8 vic_num; /// number of 4k2k_vic
    EN_MAPI_RAPTORS_HDMITX_VIDEO_4K2K_VIC vic4k2k[8]; /// 4k2k_vic
}ST_MAILBOX_COMMAND_REPORT_EDID_INFO_4K2KVIC;// struct of report edid info 4k2k_vic

/// Mailbox Data Struct For Command 0x05 Report EDID Information (Different Query Type Has Different Struct)
/// Query Type = 3D Structure, Query_Type = E_MAPI_RAPTORS_QUERY_3D_STRUCTURE 0x02
typedef struct ST_MAILBOX_COMMAND_REPORT_EDID_INFO_3DSTRUCT
{

    EN_MAPI_RAPTORS_MAILBOX_COMMAND_TYPE command_idx; /// val = E_MAPI_MAILBOX_REPORT_EDID_INFO

    /// length of the following data
    MS_U8 data_len; /// Bytes

    /// query type
    EN_MAPI_RAPTORS_QUERY_TYPE query_type; /// val = E_MAPI_RAPTORS_QUERY_3D_STRUCTURE

    /// data
    EN_MAPI_RAPTORS_HDMITX_QUERY_3DTIMING repTiming_3d; /// 3D timing
    EN_MAPI_RAPTORS_HDMITX_VIDEO_REPORT_3D_STRUCTURE_TYPE edid_3d; /// 3D edid structure, combination of EN_MAPI_RAPTORS_HDMITX_VIDEO_REPORT_3D_STRUCTURE_TYPE
}ST_MAILBOX_COMMAND_REPORT_EDID_INFO_3DSTRUCT;// struct of report edid info 3D_structure

/// Mailbox Data Struct For Command 0x05 Report EDID Information (Different Query Type Has Different Struct)
/// Query Type = Number of Audio Descriptor, Query_Type = E_MAPI_RAPTORS_QUERY_AUDIO_DESCRIPTOR_NUM 0x03
typedef struct ST_MAILBOX_COMMAND_REPORT_EDID_INFO_AUDIODES_NUM
{

    EN_MAPI_RAPTORS_MAILBOX_COMMAND_TYPE command_idx; /// val = E_MAPI_MAILBOX_REPORT_EDID_INFO

    /// length of the following data
    MS_U8 data_len; /// Bytes

    /// query type
    EN_MAPI_RAPTORS_QUERY_TYPE query_type; /// val = E_MAPI_RAPTORS_QUERY_AUDIO_DESCRIPTOR_NUM

    /// data
    MS_U8 auddes_num; /// number of audio descriptor
}ST_MAILBOX_COMMAND_REPORT_EDID_INFO_AUDIODES_NUM;// struct of number of audio descriptor

/// Mailbox Data Struct For Command 0x05 Report EDID Information (Different Query Type Has Different Struct)
/// Query Type = Audio Descriptor, Query_Type = E_MAPI_RAPTORS_QUERY_AUDIO_DESCRIPTOR 0x04
typedef struct ST_MAILBOX_COMMAND_REPORT_EDID_INFO_AUDIODES
{

    EN_MAPI_RAPTORS_MAILBOX_COMMAND_TYPE command_idx; /// val = E_MAPI_MAILBOX_REPORT_EDID_INFO

    /// length of the following data
    MS_U8 data_len; /// Bytes

    /// query type
    EN_MAPI_RAPTORS_QUERY_TYPE query_type; /// val = E_MAPI_RAPTORS_QUERY_AUDIO_DESCRIPTOR

    /// data
    MS_U8 audstr_Idx; /// start index of audio descriptor
    MS_U8 audend_Idx; /// end index of audio descriptor
    MS_U8 aud_des[33];   /// audio descriptors
}ST_MAILBOX_COMMAND_REPORT_EDID_INFO_AUDIODES;// struct of audio descriptor

/// Mailbox Data Struct For Command 0x05 Report EDID Information (Different Query Type Has Different Struct)
/// Query Type = Number of Video Descriptor, Query_Type = E_MAPI_RAPTORS_QUERY_VIDEO_DESCRIPTOR_NUM 0x05
typedef struct ST_MAILBOX_COMMAND_REPORT_EDID_INFO_VIDEODES_NUM
{

    EN_MAPI_RAPTORS_MAILBOX_COMMAND_TYPE command_idx; /// val = E_MAPI_MAILBOX_REPORT_EDID_INFO

    /// length of the following data
    MS_U8 data_len; /// Bytes

    /// query type
    EN_MAPI_RAPTORS_QUERY_TYPE query_type; /// val = E_MAPI_RAPTORS_QUERY_VIDEO_DESCRIPTOR_NUM

    /// data
    MS_U8 viddes_num; /// number of audio descriptor
}ST_MAILBOX_COMMAND_REPORT_EDID_INFO_VIDEODES_NUM;// struct of number of video descriptor

/// Mailbox Data Struct For Command 0x05 Report EDID Information (Different Query Type Has Different Struct)
/// Query Type = Video Descriptor, Query_Type = E_MAPI_RAPTORS_QUERY_VIDEO_DESCRIPTOR 0x06
typedef struct ST_MAILBOX_COMMAND_REPORT_EDID_INFO_VIDEODES
{

    EN_MAPI_RAPTORS_MAILBOX_COMMAND_TYPE command_idx; /// val = E_MAPI_MAILBOX_REPORT_EDID_INFO

    /// length of the following data
    MS_U8 data_len; /// Bytes

    /// query type
    EN_MAPI_RAPTORS_QUERY_TYPE query_type; /// val = E_MAPI_RAPTORS_QUERY_VIDEO_DESCRIPTOR

    /// data
    MS_U8 vidstr_Idx; /// start index of video descriptor
    MS_U8 vidend_Idx; /// end index of video descriptor
    MS_U8 vid_des[33];   /// video descriptors
}ST_MAILBOX_COMMAND_REPORT_EDID_INFO_VIDEODES;// struct of video descriptor

/// Mailbox Data Struct For Command 0x05 Report EDID Information (Different Query Type Has Different Struct)
/// Query Type = EDID Raw Data, Query_Type = E_MAPI_RAPTORS_QUERY_EDID_DATA 0x07
typedef struct ST_MAILBOX_COMMAND_REPORT_EDID_INFO_EDIDRAWDATA
{

    EN_MAPI_RAPTORS_MAILBOX_COMMAND_TYPE command_idx; /// val = E_MAPI_MAILBOX_REPORT_EDID_INFO

    /// length of the following data
    MS_U8 data_len; /// Bytes

    /// query type
    EN_MAPI_RAPTORS_QUERY_TYPE query_type; /// val = E_MAPI_RAPTORS_QUERY_EDID_DATA

    /// data
    MS_U8 Idx_edid;    /// index of edid
    MS_U8 Idx_64bytes; /// index of 64bytes
    MS_U8 raw_data[68];   /// raw data
}ST_MAILBOX_COMMAND_REPORT_EDID_INFO_EDIDRAWDATA;// struct of edid raw data

/// Mailbox Data Struct For Command 0x06 Status Response / ACK-Command
typedef struct ST_MAILBOX_COMMAND_ACK_STATUS_RESPONSE
{

    EN_MAPI_RAPTORS_MAILBOX_COMMAND_TYPE command_idx; /// val = E_MAPI_MAILBOX_ACK_STATUS_RESPOND

    /// length of the following data
    MS_U8 data_len; /// Bytes

    /// received command
    EN_MAPI_RAPTORS_MAILBOX_RECIEVED_COMMAND_TYPE mbx_reccom; /// received command type

    /// status
    EN_MAPI_RAPTORS_MAILBOX_STATUS_RESPONSE_TYPE mbx_status;  /// received status type

}ST_MAILBOX_COMMAND_ACK_STATUS_RESPONSE;// struct of status response or ack-command

typedef struct ST_MAILBOX_COMMAND_REPORT_EDID_INFO_COLORFMT
{

    /// command id
    EN_MAPI_RAPTORS_MAILBOX_COMMAND_TYPE command_idx; /// val = E_MAPI_MAILBOX_REPORT_EDID_INFO

    /// length of the following data
    MS_U8 data_len; /// Bytes

    /// query type
    EN_MAPI_RAPTORS_QUERY_TYPE query_type; /// val = E_MAPI_RAPTORS_QUERY_COLOR_FORMAT

    /// data
    EN_MAPI_RAPTORS_HDMITX_QUERY_COLORFMTTIMING colfmt_timing;
    MS_U8 supColorFmt;

}ST_MAILBOX_COMMAND_REPORT_EDID_INFO_COLORFMT;// struct of number of color format

void MDrv_Rocker2_SWI2C_Init(void)
{
    static MS_U8 is_first_init = FALSE;
    UBOOT_TRACE("IN\n");
    if(is_first_init == FALSE)
    {
        UBOOT_DEBUG("[Rocker2] Initialize SW I2C\n");
        MApi_SWI2C_Init(Rocket2_I2cBusCfg, SWI2C_I2C_BUS_NUM);
        is_first_init = TRUE;
    }
    else
    {
        UBOOT_ERROR("[Rocker2] SW I2C is already initialized\n");
        return;
    }
    UBOOT_TRACE("OK\n");
}

static MS_BOOL Raptors_IIC_WriteBytes( MS_U32 uAddrCnt, MS_U8 *pRegAddr, MS_U32 uSize, MS_U8 *pData)
{

    if(MApi_SWI2C_WriteBytes(ROCKET_I2C_WR_SLAVE_ID,uAddrCnt, pRegAddr, uSize, pData) == FALSE)
    {
        UBOOT_ERROR("%s, fail\n", __func__);
        return FALSE;
    }
    else
        return TRUE;

}

static MS_BOOL Raptors_IIC_ReadBytes( MS_U32 uAddrCnt, MS_U8 *pRegAddr, MS_U32 uSize, MS_U8 *pData)
{

    if(MApi_SWI2C_ReadBytes(ROCKET_I2C_WR_SLAVE_ID,uAddrCnt, pRegAddr, uSize, pData) == FALSE)
        {
        UBOOT_ERROR("%s, fail\n", __func__);
        return FALSE;
        }
    else
        return TRUE;
}


MS_BOOL Device_Hdmitx_IIC_WriteBytes(MS_U16 u16Addr, MS_U8 u8Buf)
{
        MS_U8 u8Databuf[5] = {0, 0, 0, 0, 0};
        MS_BOOL bResult = TRUE;

        u8Databuf[0] = 0x10;
        u8Databuf[3] = u8Buf;

        u8Databuf[1] = (MS_U8)(u16Addr >> 8);          //the high byte need not move left 1 bit
        u8Databuf[2] = (MS_U8)((u16Addr & 0xFF));        //the low byte moves left 1 bit, reset bit0 means data low byte
        if(Raptors_IIC_WriteBytes( 0, NULL, 4, u8Databuf) == FALSE)
        {
            UBOOT_ERROR("IIC Write fail\n");
            bResult = Raptors_IIC_WriteBytes( 0, NULL, 4, u8Databuf);
        }
        return bResult;

}

MS_BOOL Device_Hdmitx_IIC_ReadBytes(MS_U16 u16Addr, MS_U8* pBuf)
{

        MS_BOOL bResult = TRUE;
        MS_U8 u8Databuf[6]={0, 0, 0, 0, 0, 0};

        u8Databuf[0] = 0x10;

        u8Databuf[1] = (MS_U8)(u16Addr >> 8);          //the high byte need not move left 1 bit
        u8Databuf[2] = (MS_U8)((u16Addr & 0xFF));        //the low byte moves left 1 bit, reset bit0 means data low byte

        if(Raptors_IIC_WriteBytes( 0, NULL, 3, u8Databuf) == FALSE)
        {
            UBOOT_ERROR("IIC Read fail 1\n");
            bResult &= (Raptors_IIC_WriteBytes( 0, NULL, 3, u8Databuf));
        }

        if(Raptors_IIC_ReadBytes(0, NULL, 1, pBuf) == FALSE)
        {
            UBOOT_ERROR("IIC Read fail 2\n");
            bResult &= (Raptors_IIC_ReadBytes( 0, NULL, 1, pBuf));
        }

        return bResult;

}

void Raptors_w2byte(MS_U16 u16Addr, MS_U16 u16val)
{
    MS_U8 u8regval;


    UBOOT_DEBUG("w2byte: %04x, %04x\n", u16Addr, u16val);


    u8regval = (MS_U8)(u16val & 0xFF);
    Device_Hdmitx_IIC_WriteBytes(u16Addr, u8regval);

    u8regval = (MS_U8)(u16val >> 8);
    Device_Hdmitx_IIC_WriteBytes(u16Addr+1, u8regval);

}


MS_U16 Raptors_r2byte(MS_U16 u16Addr)
{
    MS_U8 u8regval;
    MS_U16 u16regval;

    UBOOT_DEBUG("Raptors_r2byte: 0x%04x\n", u16Addr);


    Device_Hdmitx_IIC_ReadBytes(u16Addr, &u8regval);
    u16regval = u8regval;

    Device_Hdmitx_IIC_ReadBytes(u16Addr+1, &u8regval);
    u16regval |= (((MS_U16)u8regval)<<8);


    return u16regval;
}


MS_BOOL Mailbox_WriteCommand(EN_MAPI_RAPTORS_MAILBOX_COMMAND_TYPE command_idx, MS_U8 data_len, MS_U8 *pBuf)
{
    int i=0;
    Device_Hdmitx_IIC_WriteBytes(MAILBOX_REG_BASE | 0x00,command_idx);
    Device_Hdmitx_IIC_WriteBytes(MAILBOX_REG_BASE | 0x01,data_len);
    for(i=0;i<data_len;i++)
        {
         Device_Hdmitx_IIC_WriteBytes(MAILBOX_REG_BASE + 0x02 + i,pBuf[i]);
         //printf("Data = %x\n",pBuf[i]);
        }
    return TRUE;
}

MS_BOOL Mailbox_ReadCommand(EN_MAPI_RAPTORS_MAILBOX_COMMAND_TYPE *command_idx, MS_U8 *data_len, MS_U8 *pBuf)
{
    MS_U8 read_command;
    int i=0;
    Device_Hdmitx_IIC_ReadBytes(MAILBOX_REG_BASE | 0x00,&read_command);
    switch(read_command)
    {
        case 0x01:
            *command_idx=E_MAPI_MAILBOX_CONFIQ_VIDEO;
            break;
        case 0x02:
            *command_idx=E_MAPI_MAILBOX_CONFIQ_AUDIO;
            break;
        case 0x03:
            *command_idx=E_MAPI_MAILBOX_AVMUTE_TIMING_RESET;
            break;
        case 0x04:
            *command_idx=E_MAPI_MAILBOX_QUERY_EDID_INFO;
            break;
        case 0x05:
            *command_idx=E_MAPI_MAILBOX_REPORT_EDID_INFO;
            break;
        case 0x06:
            *command_idx=E_MAPI_MAILBOX_ACK_STATUS_RESPOND;
            break;
        case 0xFF:
            *command_idx=E_MAPI_MAILBOX_DEBUG;
            break;
        default:
            UBOOT_ERROR("Mailbox_ReadCommand Fail!!! Wrong Command Type!!!\n");
            return FALSE;
    }
    Device_Hdmitx_IIC_ReadBytes(MAILBOX_REG_BASE | 0x01,data_len);


    for(i=0;i<*data_len;i++)
    {
        Device_Hdmitx_IIC_ReadBytes(MAILBOX_REG_BASE + 0x02 + i,&pBuf[i]);
    }

    return TRUE;
}

MS_BOOL Mailbox_WriteCommand_Config_Video(ST_MAILBOX_COMMAND_CONFIQ_VIDEO *pBuf)
{
    if (pBuf->command_idx!=E_MAPI_MAILBOX_CONFIQ_VIDEO)
        {
        UBOOT_ERROR("Wrong Command Type !!!\n");
        return FALSE;
        }
    if (pBuf->data_len>7)
        {
        UBOOT_ERROR("Wrong Data Length for Mailbox Write Command Configure Video, Maximum Data Length is 7 !!!\n");
        return FALSE;
        }
    MS_U8 pData[7];
    pData[0]=(pBuf->analog_present<<4)|(pBuf->VSinfo_3D_present<<2)|(pBuf->color_present<<1)|pBuf->timing_present;
    int dataIdx=1;
    if(pBuf->timing_present && (pBuf->data_len>dataIdx))
        {
        pData[dataIdx]=pBuf->timing_idx;
        dataIdx++;
        pData[dataIdx]=(pBuf->output_mode<<3) | pBuf->color_depth;
        if(!pBuf->color_present)
            dataIdx++;
        }
    if(pBuf->color_present && (pBuf->data_len>dataIdx))
        {
        if(pBuf->timing_present)
            pData[dataIdx]=(pBuf->out_color<<6) | (pBuf->in_color<<5) | pData[dataIdx];
        else
            pData[dataIdx]=(pBuf->out_color<<6) | (pBuf->in_color<<5);
        dataIdx++;
        }
    if(pBuf->VSinfo_3D_present && (pBuf->data_len>dataIdx))
        {
        pData[dataIdx]=pBuf->vs_3d;
        dataIdx++;
        }
    if(pBuf->analog_present && (pBuf->data_len>dataIdx))
        {
        pData[dataIdx]=(pBuf->pren2<<4) | pBuf->current;
        pData[dataIdx+1]=(pBuf->tenpre<<4) | pBuf->precon;
        }
    return Mailbox_WriteCommand(pBuf->command_idx,pBuf->data_len,pData);

}

MS_BOOL Mailbox_WriteCommand_Config_Audio(ST_MAILBOX_COMMAND_CONFIQ_AUDIO *pBuf)
{
    if (pBuf->command_idx!=E_MAPI_MAILBOX_CONFIQ_AUDIO)
        {
        UBOOT_ERROR("Wrong Command Type !!!\n");
        return FALSE;
        }
    if (pBuf->data_len>5)
        {
        UBOOT_ERROR("Wrong Data Length for Mailbox Write Command Configure Audio, Maximum Data Length is 5 !!!\n");
        return FALSE;
        }
    MS_U8 pData[5];
    pData[0]=(pBuf->fmt_present<<2)|(pBuf->source_present<<1)|pBuf->inform_present;
    int dataIdx=1;
    if(pBuf->inform_present && (pBuf->data_len>dataIdx))
        {
        pData[dataIdx]=(pBuf->channel_num<<4)|pBuf->frequency;
        pData[dataIdx+1]=pBuf->code_type;
        dataIdx=dataIdx+2;
        }
    if(pBuf->source_present && (pBuf->data_len>dataIdx))
        {
        pData[dataIdx]=pBuf->source_type;
        dataIdx++;
        }
    if(pBuf->fmt_present && (pBuf->data_len>dataIdx))
        {
        pData[dataIdx]=pBuf->source_fmt;
        }

    return Mailbox_WriteCommand(pBuf->command_idx,pBuf->data_len,pData);

}

MS_BOOL Mailbox_WriteCommand_TimingChange_AVmute(ST_MAILBOX_COMMAND_TIMING_CHANGE_AVMUTE *pBuf)
{
    if (pBuf->command_idx!=E_MAPI_MAILBOX_AVMUTE_TIMING_RESET)
        {
        UBOOT_ERROR("Wrong Command Type !!!\n");
        return FALSE;
        }
    if (pBuf->data_len>2)
        {
        UBOOT_ERROR("Wrong Data Wrong for Mailbox Write Command Timing Change & AudioVideo Mute, Maximum Data Length is 2 !!!\n");
        return FALSE;
        }
    MS_U8 pData[2];
    pData[0]=(pBuf->timing_flag<<3)|(pBuf->avmute_flag<<2)|(pBuf->audio_flag<<1)|pBuf->video_flag;
    pData[1]=(pBuf->enTiming<<3)|(pBuf->enAV_mute<<2)|(pBuf->enAudio_mute<<1)|pBuf->enVideo_mute;

    return Mailbox_WriteCommand(pBuf->command_idx,pBuf->data_len,pData);

}

MS_BOOL Mailbox_WriteCommand_Query_EDID(ST_MAILBOX_COMMAND_QUERY_EDID_INFO *pBuf)
{
    if (pBuf->command_idx!=E_MAPI_MAILBOX_QUERY_EDID_INFO)
        {
        UBOOT_ERROR("Wrong Command Type !!!\n");
        return FALSE;
        }
    if (pBuf->data_len>3)
        {
        UBOOT_ERROR("Wrong Data Length for Mailbox Write Command Query EDID Information, Maximum Data Length is 2 !!!\n");
        return FALSE;
        }
    MS_U8 pData[3];
    pData[0]=pBuf->query_type;
    if(pBuf->data_len>1)
        {

        switch(pBuf->query_type)
            {
                case E_MAPI_RAPTORS_QUERY_3D_STRUCTURE:
                    pData[1]=pBuf->timing_3d;
                    pData[2]=0;
                    break;
                case E_MAPI_RAPTORS_QUERY_AUDIO_DESCRIPTOR:
                    pData[1]=pBuf->des_startIdx;
                    pData[2]=pBuf->des_endIdx;
                    break;
                case E_MAPI_RAPTORS_QUERY_VIDEO_DESCRIPTOR:
                    pData[1]=pBuf->des_startIdx;
                    pData[2]=pBuf->des_endIdx;
                    break;
                case E_MAPI_RAPTORS_QUERY_EDID_DATA:
                    pData[1]=pBuf->edid_Idx;
                    pData[2]=pBuf->bytes64_Idx;
                    break;
                case E_MAPI_RAPTORS_QUERY_COLOR_FORMAT:
                    pData[1]=pBuf->timing_colorfmt;
                    pData[2]=0;
                    break;
                default:
                    UBOOT_ERROR("Wrong Data Length for this Query Type in Mailbox Write Command EDID Information !!!\n");
                    return FALSE;
            }

        }


    return Mailbox_WriteCommand(pBuf->command_idx,pBuf->data_len,pData);

}

MS_BOOL Mailbox_ReadCommand_Report_4K2KVic (ST_MAILBOX_COMMAND_REPORT_EDID_INFO_4K2KVIC *pBuf)
{
    MS_U8 pData[10];
    int i=0;
    if(Mailbox_ReadCommand(&pBuf->command_idx,&pBuf->data_len,pData))
        {
        if(pBuf->command_idx!=E_MAPI_MAILBOX_REPORT_EDID_INFO)
            {
            UBOOT_ERROR("Mailbox_ReadCommand_Report_4K2KVic Fail!!! Wrong Command Type!!!");
            return FALSE;
            }
        if(pData[0]!=0x01)
            {
            UBOOT_ERROR("Mailbox_ReadCommand_Report_4K2KVic Fail!!! Wrong Query Type!!!");
            return FALSE;
            }
        pBuf->vic_num=pData[1]&0x0F;
        for(i=0;i<(pBuf->data_len - 2);i++)
            {
            switch(pData[i+2])
                {
                    case 0x00:
                        pBuf->vic4k2k[i]=E_MAPI_RAPTORS_HDMITX_VIDEO_4K2K_Reserved;
                        break;
                    case 0x01:
                        pBuf->vic4k2k[i]=E_MAPI_RAPTORS_HDMITX_VIDEO_4K2K_30Hz;
                        break;
                    case 0x02:
                        pBuf->vic4k2k[i]=E_MAPI_RAPTORS_HDMITX_VIDEO_4K2K_25Hz;
                        break;
                    case 0x03:
                        pBuf->vic4k2k[i]=E_MAPI_RAPTORS_HDMITX_VIDEO_4K2K_24Hz;
                        break;
                    case 0x04:
                        pBuf->vic4k2k[i]=E_MAPI_RAPTORS_HDMITX_VIDEO_4K2K_24Hz_SMPTE;
                        break;
                    default:
                        UBOOT_ERROR("Mailbox_ReadCommand_Report_4K2KVic Fail !!! Wrong Type of 4K2KVic !!!\n");
                        pBuf->vic4k2k[i]=E_MAPI_RAPTORS_HDMITX_VIDEO_4K2K_Reserved;
                }
            }
        return TRUE;
        }
    else
        {
        UBOOT_ERROR("Mailbox_ReadCommand_Report_4K2KVic Read Fail !!!\n");
        return FALSE;
        }
}


MS_BOOL Mailbox_ReadCommand_Report_3D_Structure(ST_MAILBOX_COMMAND_REPORT_EDID_INFO_3DSTRUCT *pBuf)
{
    MS_U8 data_len=0x04;
    MS_U8 pData[4];
    if(Mailbox_ReadCommand(&pBuf->command_idx,&pBuf->data_len,pData))
        {
        if(pBuf->data_len!=data_len)
            {
            UBOOT_ERROR("Mailbox_ReadCommand_Report_3D_Structure Fail !!! Wrong Data Length!!!");
            return FALSE;
            }
        if(pData[0]==0x02)
            {
            switch(pData[1])
                {
                    case 0x00:
                        pBuf->repTiming_3d=E_MAPI_RAPTORS_HDMITX_QUERY_3DTIMING_1280x720p_50Hz;
                        break;
                    case 0x01:
                        pBuf->repTiming_3d=E_MAPI_RAPTORS_HDMITX_QUERY_3DTIMING_1280x720p_60Hz;
                        break;
                    case 0x02:
                        pBuf->repTiming_3d=E_MAPI_RAPTORS_HDMITX_QUERY_3DTIMING_1920x1080i_50Hz;
                        break;
                    case 0x03:
                        pBuf->repTiming_3d=E_MAPI_RAPTORS_HDMITX_QUERY_3DTIMING_1920x1080i_60Hz;
                        break;
                    case 0x04:
                        pBuf->repTiming_3d=E_MAPI_RAPTORS_HDMITX_QUERY_3DTIMING_1920x1080p_24Hz;
                        break;
                    case 0x05:
                        pBuf->repTiming_3d=E_MAPI_RAPTORS_HDMITX_QUERY_3DTIMING_1920x1080p_25Hz;
                        break;
                    case 0x06:
                        pBuf->repTiming_3d=E_MAPI_RAPTORS_HDMITX_QUERY_3DTIMING_1920x1080p_50Hz;
                        break;
                    case 0x07:
                        pBuf->repTiming_3d=E_MAPI_RAPTORS_HDMITX_QUERY_3DTIMING_1920x1080p_60Hz;
                        break;
                    default:
                        UBOOT_ERROR("Mailbox_ReadCommand_Report_3D_Structure Fail !!! Wrong Type of 3D Timing !!!\n");
                        return FALSE;
                }
            MS_U16 Data3D;
            Data3D = (pData[3]<<8) | pData[2];
            if(Data3D== 0x0001)
                pBuf->edid_3d=E_MAPI_RAPTORS_HDMITX_VIDEO_REPORT_3D_FramePacking;
            else if(Data3D== 0x0002)
                pBuf->edid_3d=E_MAPI_RAPTORS_HDMITX_VIDEO_REPORT_3D_FieldAlternative;
            else if(Data3D== 0x0004)
                pBuf->edid_3d=E_MAPI_RAPTORS_HDMITX_VIDEO_REPORT_3D_LineAlternative;
            else if(Data3D== 0x0008)
                pBuf->edid_3d=E_MAPI_RAPTORS_HDMITX_VIDEO_REPORT_3D_SidebySide_FULL;
            else if(Data3D== 0x0010)
                pBuf->edid_3d=E_MAPI_RAPTORS_HDMITX_VIDEO_REPORT_3D_L_Dep;
            else if(Data3D== 0x0020)
                pBuf->edid_3d=E_MAPI_RAPTORS_HDMITX_VIDEO_REPORT_3D_L_Dep_Graphic_Dep;
            else if(Data3D== 0x0040)
                pBuf->edid_3d=E_MAPI_RAPTORS_HDMITX_VIDEO_REPORT_3D_TopandBottom;
            else if(Data3D== 0x0080)
                pBuf->edid_3d=E_MAPI_RAPTORS_HDMITX_VIDEO_REPORT_3D_SidebySide_Half_Horizontal;
            else if(Data3D== 0x8000)
                pBuf->edid_3d=E_MAPI_RAPTORS_HDMITX_VIDEO_REPORT_3D_SidebySide_Half_Quincunx;
            else
                {
                UBOOT_ERROR("Mailbox_ReadCommand_Report_3D_Structure Fail !!! Wrong Type of 3D Timing !!!\n");
                return FALSE;
                }
            return TRUE;
            }
        else
            {
            UBOOT_ERROR("Mailbox_ReadCommand_Report_3D_Structure Fail !!! Wrong Query Type!!!\n");
            return FALSE;
            }
        }
    else
        {
        UBOOT_ERROR("Mailbox_ReadCommand_Report_3D_Structure Read Fail !!!\n");
        return FALSE;
        }
}

MS_BOOL Mailbox_ReadCommand_Report_Num_AudioDescriptor(ST_MAILBOX_COMMAND_REPORT_EDID_INFO_AUDIODES_NUM *pBuf)
{
    MS_U8 data_len=0x02;
    MS_U8 pData[2];
    if(Mailbox_ReadCommand(&pBuf->command_idx,&pBuf->data_len,pData))
        {
        if(pBuf->command_idx!=E_MAPI_MAILBOX_REPORT_EDID_INFO)
            {
            UBOOT_ERROR("Mailbox_ReadCommand_Report_Num_AudioDescriptor Fail!!! Wrong Command Type!!!");
            return FALSE;
            }
        if(pBuf->data_len!=data_len)
            {
            UBOOT_ERROR("Mailbox_ReadCommand_Report_Num_AudioDescriptor Fail!!! Wrong Data Length!!!");
            return FALSE;
            }
        if(pData[0]!=0x03)
            {
            UBOOT_ERROR("Mailbox_ReadCommand_Report_Num_AudioDescriptor Fail!!! Wrong Query Type!!!");
            return FALSE;
            }
        pBuf->auddes_num=pData[1];
        return TRUE;
        }
    else
        {
        UBOOT_ERROR("Mailbox_ReadCommand_Report_Num_AudioDescriptor Read Fail !!!\n");
        return FALSE;
        }
}

MS_BOOL Mailbox_ReadCommand_Report_AudioDescriptor(ST_MAILBOX_COMMAND_REPORT_EDID_INFO_AUDIODES *pBuf)
{
    MS_U8 pData[68];
    int i=0;
    if(Mailbox_ReadCommand(&pBuf->command_idx,&pBuf->data_len,pData))
        {
        if(pBuf->command_idx!=E_MAPI_MAILBOX_REPORT_EDID_INFO)
            {
            UBOOT_ERROR("Mailbox_ReadCommand_Report_AudioDescriptor Fail!!! Wrong Command Type!!!");
            return FALSE;
            }
        if(pData[0]!=0x04)
            {
            UBOOT_ERROR("Mailbox_ReadCommand_Report_AudioDescriptor Fail!!! Wrong Query Type!!!");
            return FALSE;
            }
        pBuf->audstr_Idx=pData[1];
        pBuf->audend_Idx=pData[2];
        for(i=0;i<(pBuf->data_len - 3);i++)
            {
            pBuf->aud_des[i]=pData[i+3];
            }
        return TRUE;
        }
    else
        {
        UBOOT_ERROR("Mailbox_ReadCommand_Report_AudioDescriptor Read Fail !!!\n");
        return FALSE;
        }
}

MS_BOOL Mailbox_ReadCommand_Report_Num_VideoDescriptor(ST_MAILBOX_COMMAND_REPORT_EDID_INFO_VIDEODES_NUM *pBuf)
{
    MS_U8 data_len=0x02;
    MS_U8 pData[2];
    if(Mailbox_ReadCommand(&pBuf->command_idx,&pBuf->data_len,pData))
        {
        if(pBuf->command_idx!=E_MAPI_MAILBOX_REPORT_EDID_INFO)
            {
            UBOOT_ERROR("Mailbox_ReadCommand_Report_Num_VideoDescriptor Fail!!! Wrong Command Type!!!");
            return FALSE;
            }
        if(pBuf->data_len!=data_len)
            {
            UBOOT_ERROR("Mailbox_ReadCommand_Report_Num_VideoDescriptor Fail!!! Wrong Data Length!!!");
            return FALSE;
            }
        if(pData[0]!=0x05)
            {
            UBOOT_ERROR("Mailbox_ReadCommand_Report_Num_VideoDescriptor Fail!!! Wrong Query Type!!!");
            return FALSE;
            }
        pBuf->viddes_num=pData[1];
        return TRUE;
        }
    else
        {
        UBOOT_ERROR("Mailbox_ReadCommand_Report_Num_VideoDescriptor Read Fail !!!\n");
        return FALSE;
        }
}

MS_BOOL Mailbox_ReadCommand_Report_VideoDescriptor(ST_MAILBOX_COMMAND_REPORT_EDID_INFO_VIDEODES *pBuf)
{
    MS_U8 pData[68];
    int i=0;
    if(Mailbox_ReadCommand(&pBuf->command_idx,&pBuf->data_len,pData))
        {
        if(pBuf->command_idx!=E_MAPI_MAILBOX_REPORT_EDID_INFO)
            {
            UBOOT_ERROR("Mailbox_ReadCommand_Report_VideoDescriptor Fail!!! Wrong Command Type!!!");
            return FALSE;
            }
        if(pData[0]!=0x06)
            {
            UBOOT_ERROR("Mailbox_ReadCommand_Report_VideoDescriptor Fail!!! Wrong Query Type!!!");
            return FALSE;
            }
        pBuf->vidstr_Idx=pData[1];
        pBuf->vidend_Idx=pData[2];
        for(i=0;i<(pBuf->data_len - 3);i++)
            {
            pBuf->vid_des[i]=pData[i+3];
            }
        return TRUE;
        }
    else
        {
        UBOOT_ERROR("Mailbox_ReadCommand_Report_VideoDescriptor Read Fail !!!\n");
        return FALSE;
        }
}

MS_BOOL Mailbox_ReadCommand_Report_EDID_RawData(ST_MAILBOX_COMMAND_REPORT_EDID_INFO_EDIDRAWDATA *pBuf)
{
    MS_U8 pData[68];
    int i=0;
    if(Mailbox_ReadCommand(&pBuf->command_idx,&pBuf->data_len,pData))
        {
        if(pBuf->command_idx!=E_MAPI_MAILBOX_REPORT_EDID_INFO)
            {
            UBOOT_ERROR("Mailbox_ReadCommand_Report_EDID_RawData Fail!!! Wrong Command Type!!!");
            return FALSE;
            }
        if(pData[0]!=0x07)
            {
            UBOOT_ERROR("Mailbox_ReadCommand_Report_EDID_RawData Fail!!! Wrong Query Type!!!");
            return FALSE;
            }
        pBuf->Idx_edid=pData[1]&0x0F;
        pBuf->Idx_64bytes=pData[1]&0xF0;
        for(i=0;i<(pBuf->data_len - 2);i++)
            {
            pBuf->raw_data[i]=pData[i+2];
            }
        return TRUE;
        }
    else
        {
        UBOOT_ERROR("Mailbox_ReadCommand_Report_EDID_RawData Read Fail !!!\n");
        return FALSE;
        }
}


MS_BOOL Mailbox_ReadCommand_ACK_Status(ST_MAILBOX_COMMAND_ACK_STATUS_RESPONSE *pBuf)
{
    MS_U8 data_len=0x02;
    MS_U8 pData[2];
    UBOOT_TRACE("IN\n");
    if(Mailbox_ReadCommand(&pBuf->command_idx,&pBuf->data_len,pData))
        {
        if(pBuf->data_len!=data_len)
            {
            UBOOT_ERROR("Mailbox_ReadCommand_ACK_Status Fail!!! Wrong Data Length!!!");
            return FALSE;
            }
        if(pBuf->command_idx==E_MAPI_MAILBOX_ACK_STATUS_RESPOND)
            {
            switch(pData[0])
                {
                    case 0x01:
                        pBuf->mbx_reccom=E_MAPI_MAILBOX_REPORT_CONFIQ_VIDEO;
                        break;
                    case 0x02:
                        pBuf->mbx_reccom=E_MAPI_MAILBOX_REPORT_CONFIQ_AUDIO;
                        break;
                    case 0x03:
                        pBuf->mbx_reccom=E_MAPI_MAILBOX_REPORT_AVMUTE_TIMING_RESET;
                        break;
                    case 0x04:
                        pBuf->mbx_reccom=E_MAPI_MAILBOX_REPORT_REPORT_EDID_INFO;
                        break;
                    default:
                        UBOOT_ERROR("Wrong Received Command Type !!!\n");
                        return FALSE;
                }

            switch(pData[1])
                {
                    case 0x00:
                        pBuf->mbx_status=E_MAPI_MAILBOX_REPORT_SUCCESS;
                        break;
                    case 0x01:
                        pBuf->mbx_status=E_MAPI_MAILBOX_REPORT_FAIL;
                        break;
                    case 0x02:
                        pBuf->mbx_status=E_MAPI_MAILBOX_REPORT_PARAMETER_INVALID;
                        break;
                    default:
                        UBOOT_ERROR("Wrong Status Type !!!\n");
                        return FALSE;
                }
            return TRUE;
            }
        else
            {
            printf("Mailbox_ReadCommand_ACK_Status Fail !!! Wrong Command Type!!!\n");
            return FALSE;
            }
        }
    else
        {
        printf("Mailbox_ReadCommand_ACK_Status Read Fail !!!\n");
        return FALSE;
        }
    UBOOT_TRACE("OK\n");
}

MS_BOOL Mailbox_ReadCommand_Report_ColorFormat(ST_MAILBOX_COMMAND_REPORT_EDID_INFO_COLORFMT *pBuf)
{
    MS_U8 pData[3];
    memset(pData, 0, sizeof(MS_U8)*3);

    UBOOT_DEBUG("[Device Raptors] %s, %d\n",__func__,__LINE__);

    if(Mailbox_ReadCommand(&pBuf->command_idx,&pBuf->data_len,pData))
    {
        if(pBuf->command_idx!=E_MAPI_MAILBOX_REPORT_EDID_INFO)
        {
            UBOOT_DEBUG("[Device Raptors] %s Fail!!! Wrong Command Type!!!\n",__func__);
            return FALSE;
        }
        if(pData[0]!=0x09)
        {
            UBOOT_DEBUG("[Device Raptors] %s Fail!!! Wrong Query Type!!!\n",__func__);
            return FALSE;
        }
        pBuf->colfmt_timing=(EN_MAPI_RAPTORS_HDMITX_QUERY_COLORFMTTIMING)pData[1];
        pBuf->supColorFmt=(MS_U8)pData[2];

        return TRUE;

    }
    else
    {
        UBOOT_ERROR("[Device Raptors] %s Read Fail !!!\n",__func__);
        return FALSE;
    }
}

MS_BOOL EnterSerialDebugMode(MS_BOOL bEnable)
{

        MS_U8 u8DataBuf[5] = {0x53, 0x45, 0x52, 0x44, 0x42};
        MS_BOOL bResult = TRUE;
        MS_U8 u8cnt = 0;

        printf("EnterSerialDebugMode:%d ++++++++++++++++++++++++++\n", bEnable);
        if (bEnable)
        {
            u8DataBuf[0] = 0x53;

            while(Raptors_IIC_WriteBytes( 0, NULL, 5, u8DataBuf) == FALSE)
            {
                u8cnt++;
                bResult = FALSE;
                //printf("FAIL %d\n", u8cnt);
                if(u8cnt == 0x20)
                    break;
            }
        }
        else
        {
            u8DataBuf[0] = 0x45;

            while(Raptors_IIC_WriteBytes( 0, NULL, 5, u8DataBuf) == FALSE)
            {
                u8cnt++;
                bResult = FALSE;
                if(u8cnt == 0x20)
                    break;
            }
        }
        return bResult;


}

MS_BOOL IIC_Config(void)
{

    MS_U8 u8DataBuf;
    MS_BOOL bResult = TRUE;

    printf("IIC_Config ++++++++++++++++++++++++++++\n");

    // change to CH3
    u8DataBuf = 0x81;
    if(Raptors_IIC_WriteBytes(0, NULL, 1, &u8DataBuf) == FALSE)
    {
        printf("Ch3  0x81 fail\n");
        bResult &= Raptors_IIC_WriteBytes( 0, NULL, 1, &u8DataBuf);
    }

    u8DataBuf = 0x83;
    if(Raptors_IIC_WriteBytes( 0, NULL, 1, &u8DataBuf) == FALSE)
    {
        printf("Ch3  0x83 fail\n");
        bResult &= Raptors_IIC_WriteBytes( 0, NULL, 1, &u8DataBuf);
    }

    u8DataBuf = 0x84;
    if(Raptors_IIC_WriteBytes( 0, NULL, 1, &u8DataBuf) == FALSE)
    {
        printf("Ch3  0x84 fail\n");
        bResult &= Raptors_IIC_WriteBytes( 0, NULL, 1, &u8DataBuf);
    }

    // address 2 byte
    u8DataBuf = 0x51;
    if(Raptors_IIC_WriteBytes( 0, NULL, 1, &u8DataBuf) == FALSE)
    {
        printf("Ch3  0x51 fail\n");
        bResult &= Raptors_IIC_WriteBytes( 0, NULL, 1, &u8DataBuf);
    }
    // config
    u8DataBuf = 0x7F;
    if(Raptors_IIC_WriteBytes( 0, NULL, 1, &u8DataBuf) == FALSE)
    {
        printf("Ch3  0x7F fail\n");
        bResult &= Raptors_IIC_WriteBytes( 0, NULL, 1, &u8DataBuf);
    }

    return bResult;

}

MS_BOOL IICUseBus(MS_BOOL bEnable)
{

    MS_U8 u8DataBuf;
    MS_BOOL bResult = FALSE;
    UBOOT_TRACE("IN\n");

    if(bEnable)
        u8DataBuf = 0x35;
    else
        u8DataBuf = 0x34;

    printf("IICUseBus:%d ++++++++++++++++++++++++++++\n", bEnable);

    if(Raptors_IIC_WriteBytes( 0, NULL, 1, &u8DataBuf) == FALSE)
    {
        printf("IICUseBus read write fail\n");
        bResult = Raptors_IIC_WriteBytes( 0, NULL, 1, &u8DataBuf);
    }

    // shape IIC Plus
    u8DataBuf = 0x71;
    if(Raptors_IIC_WriteBytes( 0, NULL, 1, &u8DataBuf) == FALSE)
    {
        printf("IICUseBus shap IIC fail\n");
        bResult = Raptors_IIC_WriteBytes( 0, NULL, 1, &u8DataBuf);
    }
    UBOOT_TRACE("OK\n");

    return bResult;

}

void Raptors_IIC_Init(void)
{

    IICUseBus(FALSE);
    EnterSerialDebugMode(FALSE);

    //MsOS_DelayTask(10);

    EnterSerialDebugMode(TRUE);
    IIC_Config();
    IICUseBus(TRUE);
}

void Raptors_SPI_Init(void)
{
    UBOOT_TRACE("IN\n");
    Raptors_w2byte(0x1304, 0x0001);
    Raptors_w2byte(0x1E40, 0x0000);
    Raptors_w2byte(0x1E42, 0x0000);
    Raptors_w2byte(0x1E44, 0x0000);
    Raptors_w2byte(0x1E46, 0x0000);
    Raptors_w2byte(0x1E48, 0x0000);
    Raptors_w2byte(0x1E4A, 0x0000);
    Raptors_w2byte(0x1E4C, 0x0000);
    Raptors_w2byte(0x1E4E, 0x0000);
    Raptors_w2byte(0x1E60, 0x0000);
    Raptors_w2byte(0x1E62, 0x0000);
    Raptors_w2byte(0x1E64, 0x0000);
    Raptors_w2byte(0x1E66, 0x0000);
    Raptors_w2byte(0x1E68, 0x0000);
    Raptors_w2byte(0x1E74, 0x0000);
    Raptors_w2byte(0x1E76, 0x0020);
    Raptors_w2byte(0x1E78, 0x0000);
    Raptors_w2byte(0x1E7A, 0x0000);
    Raptors_w2byte(0x0B00, 0x0000); //all_pad_in = 0
    #if(ENABLE_MSTAR_MASERATI==1)
    Raptors_w2byte(0x0B80, 0x2000); //[10] reg_spi_mode = 1 [12] reg_uart_mode1 = 1 [13] Maserati patch?
    #else
    Raptors_w2byte(0x0B80, 0x1000); //[10] reg_spi_mode = 1 [12] reg_uart_mode1 = 1 [13] Maserati patch?
    #endif

    Raptors_w2byte(0x000FE4, 0x0001);

    Raptors_w2byte(0x000EC0, 0x000B); //reg_fsp_wd1[7:0], reg_fsp_wd0[7:0]
    Raptors_w2byte(0x000EC2, 0x0000); //reg_fsp_wd3[7:0], reg_fsp_wd2[7:0]
    Raptors_w2byte(0x000EC4, 0x0000); //reg_fsp_wd5[7:0], reg_fsp_wd4[7:0]
    Raptors_w2byte(0x000EC6, 0x0000); //reg_fsp_wd7[7:0], reg_fsp_wd6[7:0]
    Raptors_w2byte(0x000EC8, 0x0000); //reg_fsp_wd9[7:0], reg_fsp_wd8[7:0]
    Raptors_w2byte(0x000ED4, 0x0005); //reg_fsp_wbf_size2[3:0], reg_fsp_wbf_size1[3:0], reg_fsp_wbf_size0[3:0]
    Raptors_w2byte(0x000ED6, 0x0001); //reg_fsp_rbf_size2[3:0], reg_fsp_rbf_size1[3:0], reg_fsp_rbf_size0[3:0]

    Raptors_w2byte(0x000ED8, 0x0007); //reg_fsp_ctrl1[7:0], reg_fsp_ctrl0[7:0]

    Raptors_w2byte(0x000EDA, 0x0001); //reg_fsp_trigger_le_w
    mdelay(10);
    Raptors_w2byte(0x000EDE, 0x0001); //reg_fsp_clear_done_flag_le_w

    Raptors_w2byte(0x001E72, 0x0800);  //reg_sw_spi_clk, reg_ckg_spi[3:0]
    Raptors_w2byte(0x001E72, 0x1800);  //reg_sw_spi_clk, reg_ckg_spi[3:0]


    Raptors_w2byte(0x1E20, 0x0003);
    Raptors_w2byte(0x1C80, 0x0001);

    mdelay(1);
    Raptors_w2byte(0x1C80, 0x000F);
#if (defined(CONFIG_MSTAR_BD_MST105D_10AVPC_MASERATI) || defined(CONFIG_MSTAR_BD_MST115F_10AVPC_MASERATI))
    Raptors_w2byte(0x268a, 0x3940);
#endif
    if((getenv("raptor_box") != NULL) || (getenv("hdmitx_edid") != NULL))
    {
        Raptors_w2byte(0x001EFC, 0x0001);
    }
    UBOOT_TRACE("OK\n");
}



void Fn_Send_Interrupt_Raptors(void)
{
    Raptors_w2byte(0x2B0C, 0x0000);
    Raptors_w2byte(0x2B0C, 0x0001);
}


MS_BOOL Fn_ReadACK(void)
{
    MS_BOOL bgpio=FALSE;
    bgpio=mdrv_gpio_get_level(ROCKET2_GPIO_ACK);

    //read bank
    UBOOT_DEBUG("[Device Raptors] ACK = %x\n",(int)Raptors_r2byte(0x1250));

    //gpio
    UBOOT_DEBUG("[Device Raptors] GPIO = %x\n",bgpio);

    if(bgpio)
        return TRUE;
    else
        return FALSE;
}

void Fn_SetACK_Low(void)
{
    Raptors_w2byte(0x1250,0x0000);

    Raptors_w2byte(0x0A02, (Raptors_r2byte(0x0A02) & 0xFFDF));
    Raptors_w2byte(0x0A0A, (Raptors_r2byte(0x0A0A) | 0x0020));
}

MS_BOOL Fn_WaitACK(void)
{
    UBOOT_TRACE("IN\n");
    //read Raptors ACK
    MS_U32 bFirstTimer;
    MS_U32 bTimer;
    bFirstTimer=MsOS_GetSystemTime();
    UBOOT_DEBUG("[icebox] before set 4k2k60 System Time = %d\n",(int)bFirstTimer);
    while(!Fn_ReadACK())
    {
        mdelay(100);
        bTimer=MsOS_GetSystemTime();
        if((int)(bTimer-bFirstTimer)>5000)
        {
            UBOOT_ERROR("[Device Raptors] Read MBX ACK Fail! Time out!\n");
            return FALSE;
        }
    }
    UBOOT_TRACE("OK\n");

    return TRUE;
}

MS_BOOL TurnOnOff(MS_BOOL bEn)//Turn on bEn=1 ; Turn off bEn=0
{
    UBOOT_TRACE("IN\n");
    //set AVmute flag
    ST_MAILBOX_COMMAND_TIMING_CHANGE_AVMUTE setFlag;
    setFlag.command_idx=E_MAPI_MAILBOX_AVMUTE_TIMING_RESET;
    setFlag.data_len=0x02;
    setFlag.avmute_flag=TRUE;
    setFlag.timing_flag=TRUE;
    setFlag.video_flag=FALSE;
    setFlag.audio_flag=FALSE;
    if(!bEn)
        {
        setFlag.enAV_mute=E_MAPI_RAPTORS_HDMITX_AVMUTE_ENABLE;
        setFlag.enTiming =E_MAPI_RAPTORS_HDMITX_TIMING_CHANGE_ENABLE;
        }
    else
        {
        setFlag.enAV_mute=E_MAPI_RAPTORS_HDMITX_AVMUTE_DISABLE;
        setFlag.enTiming =E_MAPI_RAPTORS_HDMITX_TIMING_CHANGE_DISABLE;
        }
    MS_BOOL bFlag;
    bFlag=Mailbox_WriteCommand_TimingChange_AVmute(&setFlag);
    UBOOT_DEBUG("[Device Raptors] SetOutputBlack Mailbox_Write TimingChange_AVmute = %x\n",bFlag);
    Fn_Send_Interrupt_Raptors();
    //

    //read Raptors ACK
    if(!Fn_WaitACK())
        {
        UBOOT_ERROR("DEVICE_HDMITX::%s, %d , Setting Wrong!!! Read ACK Fail!!!\n",__func__,__LINE__);
        return FALSE;
        }

    Fn_SetACK_Low();//set ACK low after read it high
    UBOOT_TRACE("OK\n");
    return TRUE;
}


MS_BOOL MuteAudioFIFO(MS_BOOL bEn)
{
    UBOOT_TRACE("IN\n");
    //set AVmute flag
    ST_MAILBOX_COMMAND_TIMING_CHANGE_AVMUTE setFlag;
    setFlag.command_idx=E_MAPI_MAILBOX_AVMUTE_TIMING_RESET;
    setFlag.data_len=0x02;
    setFlag.audio_flag=TRUE;
    setFlag.command_idx=E_MAPI_MAILBOX_CONFIQ_VIDEO;
    if(bEn)
        setFlag.enAudio_mute=E_MAPI_RAPTORS_HDMITX_AUDIO_MUTE_ENABLE;
    else
        setFlag.enAudio_mute=E_MAPI_RAPTORS_HDMITX_AUDIO_MUTE_DISABLE;
    MS_BOOL bFlag;
    bFlag=Mailbox_WriteCommand_TimingChange_AVmute(&setFlag);
    UBOOT_DEBUG("[Device Raptors] SetOutputBlack Mailbox_Write TimingChange_AVmute = %x\n",bFlag);
    Fn_Send_Interrupt_Raptors();
    //

    //read Raptors ACK
    if(!Fn_WaitACK())
        {
        UBOOT_ERROR("DEVICE_HDMITX::%s, %d , Setting Wrong!!! Read ACK Fail!!!\n",__func__,__LINE__);
        return FALSE;
        }

    Fn_SetACK_Low();//set ACK low after read it high
    UBOOT_TRACE("OK\n");
    return TRUE;
}
MS_BOOL SetOutputBlack(MS_BOOL bEn)
{
    UBOOT_TRACE("IN\n");
    //set AVmute flag
    ST_MAILBOX_COMMAND_TIMING_CHANGE_AVMUTE setFlag;
    setFlag.command_idx=E_MAPI_MAILBOX_AVMUTE_TIMING_RESET;
    setFlag.data_len=0x02;
    setFlag.avmute_flag=TRUE;
    if(bEn)
        setFlag.enAV_mute=E_MAPI_RAPTORS_HDMITX_AVMUTE_ENABLE;
    else
        setFlag.enAV_mute=E_MAPI_RAPTORS_HDMITX_AVMUTE_DISABLE;
    MS_BOOL bFlag;
    bFlag=Mailbox_WriteCommand_TimingChange_AVmute(&setFlag);
    UBOOT_DEBUG("[Device Raptors] SetOutputBlack Mailbox_Write TimingChange_AVmute = %x\n",bFlag);
    Fn_Send_Interrupt_Raptors();
    //

    //read Raptors ACK
    if(!Fn_WaitACK())
        {
        UBOOT_ERROR("DEVICE_HDMITX::%s, %d , Setting Wrong!!! Read ACK Fail!!!\n",__func__,__LINE__);
        return FALSE;
        }

    Fn_SetACK_Low();//set ACK low after read it high
    UBOOT_TRACE("OK\n");
    return TRUE;
}

MS_BOOL Init(void)
{
    UBOOT_TRACE("IN\n");


    //init IIC
    Raptors_IIC_Init();

    //read ChipID
    MS_U8 chipID;
    Device_Hdmitx_IIC_ReadBytes(0x1E00,&chipID);
    printf("[Device Raptors] show ChipID 1E 00 =%x\n",chipID);
    if(chipID!=0x97)
        {
        printf("[Device HDMITX] Read Chip ID FAIL!!!\n");
        return FALSE;
        }

    //init Raptors
    Raptors_SPI_Init();

    //read Raptors CPU Status
    MS_U16 chipCPU;
    MS_U32 bFirstTimer;
    MS_U32 bTimer;

    chipCPU=Raptors_r2byte(0x1EFE);
    bFirstTimer=MsOS_GetSystemTime();
    while(chipCPU!=0xB430)
        {
        printf("[Device Raptors] show ChipCPU Status 1E FE =%x\n",chipCPU);
        bTimer=MsOS_GetSystemTime();
        if((int)(bTimer-bFirstTimer)>5000)
            {
            printf("[Device Raptors] Init Fail! CPU not ready! Time out!\n");
            return FALSE;
            }
        mdelay(100);
        chipCPU=Raptors_r2byte(0x1EFE);
        }

#if 0 // patch code
    //temp setTimingType (patch code)
    Monaco_4k2k60p_8lane();
    usleep(5000*1000);
    TurnOnOff(FALSE);
    Fn_set4K2K60Timing();
    TurnOnOff(TRUE);
    Fn_Set_Audio_2ch();
    Fn_Set_SPDIF();
    //
#endif
    UBOOT_TRACE("OK\n");

    return TRUE;
}



MS_BOOL SetTimingType(EN_MAPI_DEVICE_ROCKY_VIDEO_TIMING enTiming)
{
    UBOOT_TRACE("IN\n");

#if 0

    switch(enTiming)
        {
       case E_MAPI_ROCKY_RES_720x576p:
            genHDMITx_VideoTiming = E_MAPI_RAPTORS_HDMITX_TIMING_720x576p_50Hz;
            break;
        case E_MAPI_ROCKY_RES_720x480p:
            genHDMITx_VideoTiming = E_MAPI_RAPTORS_HDMITX_TIMING_720x480p_60Hz;
            break;
        case E_MAPI_ROCKY_RES_1280x720p_50Hz:
            genHDMITx_VideoTiming = E_MAPI_RAPTORS_HDMITX_TIMING_1280x720p_50Hz;
            break;
        case E_MAPI_ROCKY_RES_1280x720p_60Hz:
            genHDMITx_VideoTiming = E_MAPI_RAPTORS_HDMITX_TIMING_1280x720p_60Hz;
            break;
        case E_MAPI_ROCKY_RES_1920x1080p_24Hz:
            genHDMITx_VideoTiming = E_MAPI_RAPTORS_HDMITX_TIMING_1920x1080p_24Hz;
            break;
        case E_MAPI_ROCKY_RES_1920x1080i_50Hz:
            genHDMITx_VideoTiming = E_MAPI_RAPTORS_HDMITX_TIMING_1920x1080i_50Hz;
            break;
        case E_MAPI_ROCKY_RES_1920x1080i_60Hz:
            genHDMITx_VideoTiming = E_MAPI_RAPTORS_HDMITX_TIMING_1920x1080i_60Hz;
            break;
        case E_MAPI_ROCKY_RES_1920x1080p_50Hz:
            genHDMITx_VideoTiming = E_MAPI_RAPTORS_HDMITX_TIMING_1920x1080p_50Hz;
            break;
        case E_MAPI_ROCKY_RES_1920x1080p_60Hz:
            genHDMITx_VideoTiming = E_MAPI_RAPTORS_HDMITX_TIMING_1920x1080p_60Hz;
            break;
        case E_MAPI_ROCKY_RES_4K2Kp_25Hz:
            genHDMITx_VideoTiming = E_MAPI_RAPTORS_HDMITX_TIMING_4K2Kp_25Hz;
            break;
        case E_MAPI_ROCKY_RES_4K2Kp_30Hz:
            genHDMITx_VideoTiming = E_MAPI_RAPTORS_HDMITX_TIMING_4K2Kp_30Hz;
            break;

        case E_MAPI_ROCKY_RES_4K2Kp_50Hz:
            genHDMITx_VideoTiming = E_MAPI_RAPTORS_HDMITX_TIMING_4K2Kp_50Hz;
            break;

        case E_MAPI_ROCKY_RES_4K2Kp_60Hz:
            genHDMITx_VideoTiming = E_MAPI_RAPTORS_HDMITX_TIMING_4K2Kp_60Hz;
            break;

        case E_MAPI_ROCKY_RES_4K2Kp_60Hz_RB:
            genHDMITx_VideoTiming = E_MAPI_RAPTORS_HDMITX_TIMING_4K2Kp_60Hz_RB;
            break;

        case E_MAPI_ROCKY_RES_4K2Kp_24Hz:
            genHDMITx_VideoTiming = E_MAPI_RAPTORS_HDMITX_TIMING_4K2Kp_24Hz;
            break;

        default:
            UBOOT_DEBUG("Wrong Mode of EN_MAPI_HDMITX_TIMING_TYPE \n");
            return FALSE;
        }

    genMAPI_HDMITxTming = enTiming;

    UBOOT_DEBUG("+++++++   DEVICE_HDMITX:: SetTimingTYpe   +++++++\n");
    UBOOT_DEBUG("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");

    //set Raptors video
    ST_MAILBOX_COMMAND_CONFIQ_VIDEO setTiming;
    setTiming.command_idx=E_MAPI_MAILBOX_CONFIQ_VIDEO;
    setTiming.data_len=0x03;
    setTiming.timing_present=TRUE;
    setTiming.color_present=TRUE;
    setTiming.VSinfo_3D_present=FALSE;
    setTiming.analog_present=FALSE;
    setTiming.timing_idx=genHDMITx_VideoTiming;
    setTiming.color_depth=genHDMITx_ColorDepth;
    setTiming.in_color=genHDMITx_InColor;
    setTiming.output_mode=genHDMITX_Mode;
    setTiming.out_color=genHDMITx_OutColor;
    MS_BOOL bIce;
    bIce=Mailbox_WriteCommand_Config_Video(&setTiming);
    UBOOT_DEBUG("[Device Raptors] Mailbox_Write Config Video = %x\n",bIce);
    Fn_Send_Interrupt_Raptors();
    //

    //read Raptors ACK
    if(!Fn_WaitACK())
        {
        UBOOT_ERROR("DEVICE_HDMITX::%s, %d , Setting Wrong!!! Read ACK Fail!!!\n",__func__,__LINE__);
        return FALSE;
        }

    Fn_SetACK_Low();//set ACK low after read it high
    //

    //set Raptors audio
    ST_MAILBOX_COMMAND_CONFIQ_AUDIO setAudio;
    setAudio.command_idx=E_MAPI_MAILBOX_CONFIQ_AUDIO;
    setAudio.data_len=0x05;
    setAudio.inform_present=TRUE;
    setAudio.source_present=TRUE;
    setAudio.fmt_present=TRUE;
    setAudio.frequency=genAudio_Freq;
    setAudio.channel_num=genAudio_Ch_Cnt;
    setAudio.code_type=genAudio_CodeType;
    setAudio.source_type=genAudio_SrcType;
    setAudio.source_fmt=genAudio_SrcFmt;

    bIce=Mailbox_WriteCommand_Config_Audio(&setAudio);
    UBOOT_DEBUG("[Device Raptors] Mailbox_Write Config Audio = %x\n",bIce);
    Fn_Send_Interrupt_Raptors();

    //read Raptors ACK
    if(!Fn_WaitACK())
        {
        UBOOT_ERROR("DEVICE_HDMITX::%s, %d , Setting Wrong!!! Read ACK Fail!!!\n",__func__,__LINE__);
        return FALSE;
        }

    Fn_SetACK_Low();//set ACK low after read it high
    //

#if 0 //will add when mboot is ready
    if(_isFirstBoot)
    {
        MApi_Lth_Set_Disable_All_Table_Setting(FALSE);
        MApi_HDMITx_DisableRegWrite(FALSE);
        _isFirstBoot = FALSE;
    }
#endif

#endif
    UBOOT_TRACE("OK\n");

    return TRUE;
}

EN_MAPI_DEVICE_ROCKY_VIDEO_TIMING getDeviceTimingType(EN_MAPI_HDMITX_TIMING_TYPE enTiming)
{
    UBOOT_TRACE("IN\n");

    switch(enTiming)
        {
            case E_MAPI_HDMITX_TIMING_480_60I:
                return  E_MAPI_ROCKY_RES_720x480i;
                break;
            case E_MAPI_HDMITX_TIMING_480_60P:
                return  E_MAPI_ROCKY_RES_720x480p;
                break;
            case E_MAPI_HDMITX_TIMING_576_50I:
                return  E_MAPI_ROCKY_RES_720x576i;
                break;
            case E_MAPI_HDMITX_TIMING_576_50P:
                return  E_MAPI_ROCKY_RES_720x576p;
                break;
            case E_MAPI_HDMITX_TIMING_720_50P:
                return  E_MAPI_ROCKY_RES_1280x720p_50Hz;
                break;
            case E_MAPI_HDMITX_TIMING_720_60P:
                return  E_MAPI_ROCKY_RES_1280x720p_60Hz;
                break;
            case E_MAPI_HDMITX_TIMING_1080_50I:
                return  E_MAPI_ROCKY_RES_1920x1080i_50Hz;
                break;
            case E_MAPI_HDMITX_TIMING_1080_50P:
                return  E_MAPI_ROCKY_RES_1920x1080p_50Hz;
                break;
            case E_MAPI_HDMITX_TIMING_1080_60I:
                return  E_MAPI_ROCKY_RES_1920x1080i_60Hz;
                break;
            case E_MAPI_HDMITX_TIMING_1080_60P:
                return  E_MAPI_ROCKY_RES_1920x1080p_60Hz;
                break;
            case E_MAPI_HDMITX_TIMING_1080_30P:
                return  E_MAPI_ROCKY_RES_1920x1080p_30Hz;
                break;
            case E_MAPI_HDMITX_TIMING_1080_25P:
                return  E_MAPI_ROCKY_RES_1920x1080p_25Hz;
                break;
            case E_MAPI_HDMITX_TIMING_1080_24P:
                return  E_MAPI_ROCKY_RES_1920x1080p_24Hz;
                break;
            case E_MAPI_HDMITX_TIMING_4K2K_60P:
                return  E_MAPI_ROCKY_RES_4K2Kp_60Hz;
                break;
            case E_MAPI_HDMITX_TIMING_4K2K_60P_RGB10BIT:
                return E_MAPI_ROCKY_RES_4K2Kp_60Hz_RB;
                break;
            case E_MAPI_HDMITX_TIMING_4K2K_30P:
                return  E_MAPI_ROCKY_RES_4K2Kp_30Hz;
                break;
            case E_MAPI_HDMITX_TIMING_4K2K_25P:
                return  E_MAPI_ROCKY_RES_4K2Kp_25Hz;
                break;
            case E_MAPI_HDMITX_TIMING_4K2K_24P:
                return  E_MAPI_ROCKY_RES_4K2Kp_24Hz;
                break;
            case E_MAPI_HDMITX_TIMING_4096x2160_24P:
                return  E_MAPI_ROCKY_RES_4096x2160p_24Hz;
                break;
            case E_MAPI_HDMITX_TIMING_1470_50P:
                return  E_MAPI_ROCKY_RES_1280x1470p_50Hz;
                break;
            case E_MAPI_HDMITX_TIMING_1470_60P:
                return  E_MAPI_ROCKY_RES_1280x1470p_60Hz;
                break;
            case E_MAPI_HDMITX_TIMING_1470_24P:
                return  E_MAPI_ROCKY_RES_1280x1470p_24Hz;
                break;
            case E_MAPI_HDMITX_TIMING_1470_30P:
                return  E_MAPI_ROCKY_RES_1280x1470p_30Hz;
                break;
            case E_MAPI_HDMITX_TIMING_2160_24P:
                return  E_MAPI_ROCKY_RES_1920x2205p_24Hz;
                break;
            case E_MAPI_HDMITX_TIMING_2160_30P:
                return  E_MAPI_ROCKY_RES_1920x2205p_30Hz;
                break;
            default:
                UBOOT_ERROR("Wrong Mode of EN_MAPI_HDMITX_TIMING_TYPE \n");
                return E_MAPI_ROCKY_RES_1920x1080p_60Hz;
        }
    UBOOT_TRACE("OK\n");
}

EN_MAPI_HDMITX_TIMING_TYPE getMapiTimingType(EN_MAPI_DEVICE_ROCKY_VIDEO_TIMING enTiming)
{

    UBOOT_TRACE("IN\n");

    switch(enTiming)
    {
        case E_MAPI_ROCKY_RES_720x480i:
            return  E_MAPI_HDMITX_TIMING_480_60I;
            break;
        case E_MAPI_ROCKY_RES_720x480p:
            return  E_MAPI_HDMITX_TIMING_480_60P;
            break;
        case E_MAPI_ROCKY_RES_720x576i:
            return  E_MAPI_HDMITX_TIMING_576_50I;
            break;
        case E_MAPI_ROCKY_RES_720x576p:
            return  E_MAPI_HDMITX_TIMING_576_50P;
            break;
        case E_MAPI_ROCKY_RES_1280x720p_50Hz:
            return  E_MAPI_HDMITX_TIMING_720_50P;
            break;
        case E_MAPI_ROCKY_RES_1280x720p_60Hz:
            return  E_MAPI_HDMITX_TIMING_720_60P;
            break;
        case E_MAPI_ROCKY_RES_1920x1080i_50Hz:
            return  E_MAPI_HDMITX_TIMING_1080_50I;
            break;
        case E_MAPI_ROCKY_RES_1920x1080p_50Hz:
            return  E_MAPI_HDMITX_TIMING_1080_50P;
            break;
        case E_MAPI_ROCKY_RES_1920x1080i_60Hz:
            return  E_MAPI_HDMITX_TIMING_1080_60I;
            break;
        case E_MAPI_ROCKY_RES_1920x1080p_60Hz:
            return  E_MAPI_HDMITX_TIMING_1080_60P;
            break;
        case E_MAPI_ROCKY_RES_1920x1080p_30Hz:
            return  E_MAPI_HDMITX_TIMING_1080_30P;
            break;
        case E_MAPI_ROCKY_RES_1920x1080p_25Hz:
            return  E_MAPI_HDMITX_TIMING_1080_25P;
            break;
        case E_MAPI_ROCKY_RES_1920x1080p_24Hz:
            return  E_MAPI_HDMITX_TIMING_1080_24P;
            break;
        case E_MAPI_ROCKY_RES_4K2Kp_60Hz:
            return  E_MAPI_HDMITX_TIMING_4K2K_60P;
            break;
        case E_MAPI_ROCKY_RES_4K2Kp_60Hz_RB:
            return E_MAPI_HDMITX_TIMING_4K2K_60P_RGB10BIT;
            break;
        case E_MAPI_ROCKY_RES_4K2Kp_30Hz:
            return  E_MAPI_HDMITX_TIMING_4K2K_30P;
            break;
        case E_MAPI_ROCKY_RES_4K2Kp_25Hz:
            return  E_MAPI_HDMITX_TIMING_4K2K_25P;
            break;
        case E_MAPI_ROCKY_RES_1280x1470p_50Hz:
            return  E_MAPI_HDMITX_TIMING_1470_50P;
            break;
        case E_MAPI_ROCKY_RES_1280x1470p_60Hz:
            return  E_MAPI_HDMITX_TIMING_1470_60P;
            break;
        case E_MAPI_ROCKY_RES_1280x1470p_24Hz:
            return  E_MAPI_HDMITX_TIMING_1470_24P;
            break;
        case E_MAPI_ROCKY_RES_1280x1470p_30Hz:
            return  E_MAPI_HDMITX_TIMING_1470_30P;
            break;
        case E_MAPI_ROCKY_RES_1920x2205p_24Hz:
            return  E_MAPI_HDMITX_TIMING_2160_24P;
            break;
        case E_MAPI_ROCKY_RES_1920x2205p_30Hz:
            return  E_MAPI_HDMITX_TIMING_2160_30P;
            break;
        default:
            UBOOT_ERROR("Wrong Mode of EN_MAPI_DEVICE_ROCKY_VIDEO_TIMING \n");
            return E_MAPI_HDMITX_TIMING_1080_60P;
    }
    UBOOT_TRACE("OK\n");
}

MS_BOOL SetColorType(EN_MAPI_HDMITX_COLOR_TYPE enInputColor, EN_MAPI_HDMITX_COLOR_TYPE enOutputColor)
{
    UBOOT_TRACE("OK\n");

    EN_MAPI_RAPTORS_HDMITX_OUTPUT_COLOR_TYPE enOutColor;
    EN_MAPI_RAPTORS_HDMITX_INPUT_COLOR_TYPE enInColor;

    switch(enOutputColor)
    {
        case E_MAPI_HDMITX_COLOR_RGB444:
            enOutColor = E_MAPI_RAPTORS_HDMITX_COLORS_OUTPUT_RGB_444;
            break;
        case E_MAPI_HDMITX_COLOR_YUV422:
            enOutColor = E_MAPI_RAPTORS_HDMITX_COLORS_OUTPUT_YUV_422;
            break;
        case E_MAPI_HDMITX_COLOR_YUV444:
            enOutColor = E_MAPI_RAPTORS_HDMITX_COLORS_OUTPUT_YUV_444;
            break;
        case E_MAPI_HDMITX_COLOR_YUV420:
            enOutColor = E_MAPI_RAPTORS_HDMITX_COLORS_OUTPUT_YUV_420;
            break;
        default:
            UBOOT_DEBUG("Wrong Mode of EN_MAPI_HDMITX_COLOR_TYPE for output color\n");
            return FALSE;
    }

    switch(enInputColor)
    {
        case E_MAPI_HDMITX_COLOR_RGB444:
            enInColor = E_MAPI_RAPTORS_HDMITX_COLORS_INPUT_RGB;
            break;
        case E_MAPI_HDMITX_COLOR_YUV422:
            enInColor = E_MAPI_RAPTORS_HDMITX_COLORS_INPUT_YUV;
            break;
        case E_MAPI_HDMITX_COLOR_YUV444:
            enInColor = E_MAPI_RAPTORS_HDMITX_COLORS_INPUT_YUV;
            break;
        default:
            UBOOT_DEBUG("Wrong Mode of EN_MAPI_HDMITX_COLOR_TYPE for input color\n");
            return FALSE;
    }

    if(enOutColor == genHDMITx_OutColor &&
       enInColor  == genHDMITx_InColor)
    {
        UBOOT_DEBUG("ColorSpace don't change. IN%s, OUT:%s\n", PARSING_IN_COLOR(enInColor), PARSING_OUT_COLOR(enOutColor));
        return TRUE;
    }

    printf("IN:%s, OUT:%s \n", PARSING_IN_COLOR(enInColor), PARSING_OUT_COLOR(enOutColor));


    //set AVmute
    SetOutputBlack(1);
    mdelay(100);
    //

    //set colortype
    ST_MAILBOX_COMMAND_CONFIQ_VIDEO setTiming;
    setTiming.command_idx=E_MAPI_MAILBOX_CONFIQ_VIDEO;
    setTiming.data_len=0x02;
    setTiming.color_present=TRUE;
    setTiming.in_color=enInColor;
    setTiming.out_color=enOutColor;
    MS_BOOL bColor;
    bColor=Mailbox_WriteCommand_Config_Video(&setTiming);
    printf("[Device Raptors] Mailbox_Write Config Video = %x\n",bColor);
    Fn_Send_Interrupt_Raptors();
    //

    //read Raptors ACK
    if(!Fn_WaitACK())
        {
        UBOOT_ERROR("DEVICE_HDMITX::%s, %d , Setting Wrong!!! Read ACK Fail!!!\n",__func__,__LINE__);
        return FALSE;
        }

    Fn_SetACK_Low();//set ACK low after read it high
    //

    //disable AVmute
    mdelay(100);
    SetOutputBlack(0);
    //

    genHDMITx_InColor  = enInColor;
    genHDMITx_OutColor = enOutColor;
    UBOOT_TRACE("OK\n");

    return TRUE;
}


MS_BOOL SetColorDepthType(EN_MAPI_HDMITX_COLOR_DEPTH_TYPE enColorDepth)
{
    UBOOT_TRACE("IN\n");


    genHDMITx_ColorDepth = enColorDepth == E_MAPI_HDMITX_COLOR_DEPTH_8BITS ?  E_MAPI_RAPTORS_HDMITX_COLORS_8bits :
                           enColorDepth == E_MAPI_HDMITX_COLOR_DEPTH_10BITS ? E_MAPI_RAPTORS_HDMITX_COLORS_10bits :
                           enColorDepth == E_MAPI_HDMITX_COLOR_DEPTH_12BITS ? E_MAPI_RAPTORS_HDMITX_COLORS_12bits :
                                                                              E_MAPI_RAPTORS_HDMITX_COLORS_NOID;

    if(_bHDMITx_Running)
    {
        TurnOnOff(FALSE);

        SetTimingType(genMAPI_HDMITxTming);

        TurnOnOff(TRUE);
    }
    UBOOT_TRACE("OK\n");
    return TRUE;
}

MS_BOOL SetOutputMode(EN_MAPI_HDMITX_OUTPUT_MODE enOutputMode)
{
    UBOOT_TRACE("IN\n");

    switch(enOutputMode)
    {
        case E_MAPI_HDMITX_VIDEO_HDMI:
            genHDMITX_Mode = E_MAPI_RAPTORS_HDMITX_VIDEO_OUTPUT_HDMI;
            break;
        case E_MAPI_HDMITX_VIDEO_HDMI_HDCP:
            genHDMITX_Mode = E_MAPI_RAPTORS_HDMITX_VIDEO_OUTPUT_HDMI_HDCP;
            break;
        case E_MAPI_HDMITX_VIDEO_DVI:
            genHDMITX_Mode = E_MAPI_RAPTORS_HDMITX_VIDEO_OUTPUT_DVI;
            break;
        case E_MAPI_HDMITX_VIDEO_DVI_HDCP:
            genHDMITX_Mode = E_MAPI_RAPTORS_HDMITX_VIDEO_OUTPUT_DVI_HDCP;
            break;
        default:
            UBOOT_DEBUG("Wrong Mode of EN_MAPI_HDMITX_OUTPUT_MODE \n");
            return FALSE;
    }

    if(_bHDMITx_Running == FALSE)
    {
        //set setTiming flag
        ST_MAILBOX_COMMAND_TIMING_CHANGE_AVMUTE setFlag;
        setFlag.command_idx=E_MAPI_MAILBOX_AVMUTE_TIMING_RESET;
        setFlag.data_len=0x02;
        setFlag.timing_flag=TRUE;
        setFlag.enTiming=E_MAPI_RAPTORS_HDMITX_TIMING_CHANGE_ENABLE;
        MS_BOOL bFlag;
        bFlag=Mailbox_WriteCommand_TimingChange_AVmute(&setFlag);
        UBOOT_DEBUG("[Device Raptors] SetOutputMode Mailbox_Write TimingChange_AVmute = %x\n",bFlag);
        Fn_Send_Interrupt_Raptors();
        //

        //read Raptors ACK
        if(!Fn_WaitACK())
            {
            UBOOT_ERROR("DEVICE_HDMITX::%s, %d , Setting Wrong!!! Read ACK Fail!!!\n",__func__,__LINE__);
            return FALSE;
            }

        Fn_SetACK_Low();//set ACK low after read it high
        //

        //set timing at configure video
        ST_MAILBOX_COMMAND_CONFIQ_VIDEO setTiming;
        setTiming.command_idx=E_MAPI_MAILBOX_CONFIQ_VIDEO;
        setTiming.data_len=0x03;
        setTiming.timing_present=TRUE;
        setTiming.timing_idx=genHDMITx_VideoTiming;
        setTiming.color_depth=genHDMITx_ColorDepth;
        setTiming.output_mode=genHDMITX_Mode;
        MS_BOOL bIce;
        bIce=Mailbox_WriteCommand_Config_Video(&setTiming);
        UBOOT_DEBUG("[Device Raptors] Mailbox_Write Config Video = %x\n",bIce);
        Fn_Send_Interrupt_Raptors();
        //

        //read Raptors ACK
        if(!Fn_WaitACK())
            {
            UBOOT_ERROR("DEVICE_HDMITX::%s, %d , Setting Wrong!!! Read ACK Fail!!!\n",__func__,__LINE__);
            return FALSE;
            }

        Fn_SetACK_Low();//set ACK low after read it high
        //

        //set setTiming flag
        ST_MAILBOX_COMMAND_TIMING_CHANGE_AVMUTE setFlag2;
        setFlag2.command_idx=E_MAPI_MAILBOX_AVMUTE_TIMING_RESET;
        setFlag2.data_len=0x02;
        setFlag2.timing_flag=TRUE;
        setFlag2.enTiming=E_MAPI_RAPTORS_HDMITX_TIMING_CHANGE_DISABLE;
        MS_BOOL bFlag2;
        bFlag2=Mailbox_WriteCommand_TimingChange_AVmute(&setFlag2);
        UBOOT_DEBUG("[Device Raptors] Mailbox_Write TimingChange_AVmute = %x\n",bFlag2);
        Fn_Send_Interrupt_Raptors();
        //

        //read Raptors ACK
        if(!Fn_WaitACK())
            {
            UBOOT_ERROR("DEVICE_HDMITX::%s, %d , Setting Wrong!!! Read ACK Fail!!!\n",__func__,__LINE__);
            return FALSE;
            }

        Fn_SetACK_Low();//set ACK low after read it high
        //

    }
    else
    {
        UBOOT_ERROR("DEVICE_HDMITX::%s, %d , Setting Wrong!!! Rocket reset Fail!!!\n",__func__,__LINE__);
    }
    UBOOT_TRACE("OK\n");
    return TRUE;
}







MS_BOOL SetAudioConfiguration(EN_MAPI_HDMITX_AUDIO_FREQUENCY freq, EN_MAPI_HDMITX_AUDIO_CHANNEL_COUNT ch, EN_MAPI_HDMITX_AUDIO_CODING_TYPE type)
{
    UBOOT_TRACE("IN\n");

    MS_BOOL bRet=TRUE;

    switch(freq)
    {
    case E_MAPI_HDMITX_AUDIO_32K:
        genAudio_Freq = E_MAPI_RAPTORS_HDMITX_AUDIO_32KHz;
    break;

    case E_MAPI_HDMITX_AUDIO_44K:
        genAudio_Freq = E_MAPI_RAPTORS_HDMITX_AUDIO_44KHz;
    break;

    case E_MAPI_HDMITX_AUDIO_48K:
        genAudio_Freq = E_MAPI_RAPTORS_HDMITX_AUDIO_48KHz;
    break;

    case E_MAPI_HDMITX_AUDIO_88K:
        genAudio_Freq = E_MAPI_RAPTORS_HDMITX_AUDIO_88KHz;
    break;

    case E_MAPI_HDMITX_AUDIO_96K:
        genAudio_Freq = E_MAPI_RAPTORS_HDMITX_AUDIO_96KHz;
    break;

    case E_MAPI_HDMITX_AUDIO_176K:
        genAudio_Freq = E_MAPI_RAPTORS_HDMITX_AUDIO_176KHz;
    break;

    case E_MAPI_HDMITX_AUDIO_192K:
        genAudio_Freq = E_MAPI_RAPTORS_HDMITX_AUDIO_192KHz;
    break;
    default:
    bRet=FALSE;
    }


    switch(ch)
    {
    case E_MAPI_HDMITX_AUDIO_CH_2:
        genAudio_Ch_Cnt = E_MAPI_RAPTORS_HDMITX_AUDIO_CH_2;
    break;

    case E_MAPI_HDMITX_AUDIO_CH_8:
        genAudio_Ch_Cnt = E_MAPI_RAPTORS_HDMITX_AUDIO_CH_8;
    break;

    default:
    bRet=FALSE;
    }


    switch(type)
    {
    case E_MAPI_HDMITX_AUDIO_PCM:
        genAudio_CodeType = E_MAPI_RAPTORS_HDMITX_AUDIO_PCM;
    break;

    case E_MAPI_HDMITX_AUDIO_NONPCM:
        genAudio_CodeType = E_MAPI_RAPTORS_HDMITX_AUDIO_NONPCM;
    break;

    default:
    bRet=FALSE;
    }

    if(bRet)
    {
        ST_MAILBOX_COMMAND_CONFIQ_AUDIO setAudio;
        setAudio.command_idx=E_MAPI_MAILBOX_CONFIQ_AUDIO;
        setAudio.data_len=0x03;
        setAudio.inform_present=TRUE;
        setAudio.frequency=genAudio_Freq;
        setAudio.channel_num=genAudio_Ch_Cnt;
        setAudio.code_type=genAudio_CodeType;

        MS_BOOL bIce;
        bIce=Mailbox_WriteCommand_Config_Audio(&setAudio);
        printf("[Device Raptors] Mailbox_Write Config Audio = %x\n",bIce);
        Fn_Send_Interrupt_Raptors();
        //

        //read Raptors ACK
        if(!Fn_WaitACK())
            {
            UBOOT_ERROR("DEVICE_HDMITX::%s, %d , Setting Wrong!!! Read ACK Fail!!!\n",__func__,__LINE__);
            return FALSE;
            }

        Fn_SetACK_Low();//set ACK low after read it high
        //
    }
    UBOOT_TRACE("OK\n");

    return bRet;

}

MS_BOOL SetAudioSourceFormat(EN_MAPI_HDMITX_AUDIO_SOURCE_FORMAT fmt)
{
    UBOOT_TRACE("IN\n");
    MS_BOOL bRet=TRUE;

    switch(fmt)
    {
    case E_MAPI_HDMITX_AUDIO_FORMAT_PCM:
        genAudio_SrcFmt = E_MAPI_RAPTORS_HDMITX_AUDIO_FORMAT_PCM;
    break;

    case E_MAPI_HDMITX_AUDIO_FORMAT_DSD:
        genAudio_SrcFmt = E_MAPI_RAPTORS_HDMITX_AUDIO_FORMAT_DSD;
    break;

    case E_MAPI_HDMITX_AUDIO_FORMAT_HBR:
        genAudio_SrcFmt = E_MAPI_RAPTORS_HDMITX_AUDIO_FORMAT_HBR;
    break;

    default:
     bRet=FALSE;
    }

    if(bRet)
    {
        ST_MAILBOX_COMMAND_CONFIQ_AUDIO setAudio;
        setAudio.command_idx=E_MAPI_MAILBOX_CONFIQ_AUDIO;
        setAudio.data_len=0x02;
        setAudio.fmt_present=TRUE;
        setAudio.source_fmt=genAudio_SrcFmt;

        MS_BOOL bIce;
        bIce=Mailbox_WriteCommand_Config_Audio(&setAudio);
        printf("[Device Raptors] Mailbox_Write Config Audio = %x\n",bIce);
        Fn_Send_Interrupt_Raptors();
        //

        //read Raptors ACK
        if(!Fn_WaitACK())
            {
            UBOOT_ERROR("DEVICE_HDMITX::%s, %d , Setting Wrong!!! Read ACK Fail!!!\n",__func__,__LINE__);
            return FALSE;
            }

        Fn_SetACK_Low();//set ACK low after read it high
        //
    }
    UBOOT_TRACE("OK\n");

    return bRet;
}


MS_BOOL Set_Audio_SourceType(EN_MAPI_HDMITX_AUDIO_TYPE enAudioType)
{
    UBOOT_TRACE("IN\n");
    MS_BOOL bRet = TRUE;

    switch(enAudioType)
    {
    case E_MAPI_HDMITX_AUDIO_SPDIF:
        genAudio_SrcType = E_MAPI_RAPTORS_HDMITX_AUDIO_SPDIF;
    break;

    case E_MAPI_HDMITX_AUDIO_I2S:
        genAudio_SrcType = E_MAPI_RAPTORS_HDMITX_AUDIO_I2S;
    break;

    default:
    bRet=FALSE;
    }

    if(bRet)
    {
        ST_MAILBOX_COMMAND_CONFIQ_AUDIO setAudio;
        setAudio.command_idx=E_MAPI_MAILBOX_CONFIQ_AUDIO;
        setAudio.data_len=0x02;
        setAudio.source_present=TRUE;
        setAudio.source_type=genAudio_SrcType;

        MS_BOOL bIce;
        bIce=Mailbox_WriteCommand_Config_Audio(&setAudio);
        printf("[Device Raptors] Mailbox_Write Config Audio = %x\n",bIce);
        Fn_Send_Interrupt_Raptors();
        //

        //read Raptors ACK
        if(!Fn_WaitACK())
            {
            UBOOT_ERROR("DEVICE_HDMITX::%s, %d , Setting Wrong!!! Read ACK Fail!!!\n",__func__,__LINE__);
            return FALSE;
            }

        Fn_SetACK_Low();//set ACK low after read it high
        //
    }

    UBOOT_TRACE("OK\n");

    return bRet;

}

MS_BOOL GetRxStatus(void)
{
    MS_BOOL bRet;
    bRet=(Raptors_r2byte(0x1252) & 0x0001);
    if(bRet)
        return TRUE;
    else
        return FALSE;
}

extern MS_BOOL MApi_SetEnv2BootArgs(const char* name,const char* value);

#define EDID_VIC_4K2K_NUM (8)
#define EDID_VIC_NUM     (32)
static MS_U8 g_u8HDMITx4K2K[EDID_VIC_4K2K_NUM];
static MS_U8 g_u8HDMITx[EDID_VIC_NUM];
static MS_BOOL g_bEDIDReady = FALSE;
static MS_BOOL g_bEDID4K2KReady = FALSE;

static char* HdmiTxPanelIndexTable[] = {
    "",//0
    "",//1
    "",//2
    "HDMITX_VB1_480_60P",//3/
    "HDMITX_VB1_576_50P",//4/
    "HDMITX_VB1_720_50P",//5
    "HDMITX_VB1_720_60P",//6
    "",//7
    "",//8
    "",//9
    "",//10
    "",//11
    "HDMITX_VB1_1080_50P",//12
    "HDMITX_VB1_1080_60P",//13
    "HDMITX_VB1_4K2K_30P",//14
    "",//15
    "",//16
    "",//17
    "",//18
    "",//19
    "",//20
    "HDMITX_VB1_4K2K_25P",//21
    "HDMITX_VB1_4K2K_24P",//22
    "HDMITX_VB1_4K2K_50P",//23
    "HDMITX_VB1_4K2K_60P",//24
    "",//25
    "",//26
    "",//27
    "HDMITX_VB1_4096_24P",//28
    "HDMITX_VB1_4K2K_60P_REDUCE_BLANK",//29
};

static char* HdmiTxTimingIndexTable[] = {
    "",//0
    "",//1
    "",//2
    "HDMITX_RES_720x480p",//3
    "HDMITX_RES_720x576p",//4
    "HDMITX_RES_1280x720p_50Hz",//5
    "HDMITX_RES_1280x720p_60Hz",//6
    "",//7
    "",//8
    "",//9
    "",//10
    "",//11
    "HDMITX_RES_1920x1080p_50Hz",//12
    "HDMITX_RES_1920x1080p_60Hz",//13
    "HDMITX_RES_4K2Kp_30Hz",//14
    "",//15
    "",//16
    "",//17
    "",//18
    "",//19
    "",//20
    "HDMITX_RES_4K2Kp_25Hz",//21
    "HDMITX_RES_4K2Kp_24Hz",//22
    "HDMITX_RES_4K2Kp_50Hz",//23
    "HDMITX_RES_4K2Kp_60Hz",//24
    "",//25
    "",//26
    "",//27
    "HDMITX_RES_4096_24P",//28
    "HDMITX_RES_4K2Kp_60Hz_RB",//29
};

static MS_U8 HdmiTxTimgList[] = {
    E_MAPI_ROCKY_RES_4K2Kp_60Hz,//24
    E_MAPI_ROCKY_RES_4K2Kp_50Hz,//23
    E_MAPI_ROCKY_RES_4K2Kp_30Hz,//14
    E_MAPI_ROCKY_RES_4K2Kp_25Hz,//21
    E_MAPI_ROCKY_RES_4K2Kp_24Hz,//22

    E_MAPI_ROCKY_RES_1920x1080p_60Hz,//13
    E_MAPI_ROCKY_RES_1920x1080p_50Hz,//12

    E_MAPI_ROCKY_RES_1280x720p_60Hz,//6
    E_MAPI_ROCKY_RES_1280x720p_50Hz,//5
    E_MAPI_ROCKY_RES_720x576p,//04
    E_MAPI_ROCKY_RES_720x480p //03
};

static MS_BOOL Fn_WaitReport(void)
{
    //read Raptors ACK
    MS_U32 bFirstTimer = 0;
    MS_U32 bTimer = 0;

    bFirstTimer=MsOS_GetSystemTime();
    while((Raptors_r2byte(0x1200) & 0xFF)!=0x05)
    {
        //udelay(100*1000);
        mdelay(100);
        //mdelay(100);
        bTimer=MsOS_GetSystemTime();
        if((int)(bTimer-bFirstTimer)>5000)
        {
            UBOOT_ERROR("[Device Raptors] Wait MBX Report Fail! Time out!\n");
            return FALSE;
        }
    }

    return TRUE;
}

static MS_BOOL GetEDID_4K2K_Inform(MS_U8 u8Num, MS_U8 *penEDID4K2KVic)
{
    MS_U8 i = 0;
    MS_U8 u8MaxNum = u8Num > 8 ? 8 : u8Num;
    MS_BOOL bRet = FALSE;
    ST_MAILBOX_COMMAND_QUERY_EDID_INFO set4K2K;
    MS_BOOL bIce = FALSE;
    ST_MAILBOX_COMMAND_REPORT_EDID_INFO_4K2KVIC get4k2k;

    UBOOT_DEBUG("[Device Raptors] %s, %d\n",__func__,__LINE__);

    memset(penEDID4K2KVic, 0, sizeof(MS_U8)*u8MaxNum);
    set4K2K.data_len=0x01;
    set4K2K.query_type=E_MAPI_RAPTORS_QUERY_4K2K_VIC;
    set4K2K.command_idx=E_MAPI_MAILBOX_QUERY_EDID_INFO;
    bIce=Mailbox_WriteCommand_Query_EDID(&set4K2K);
    if(!bIce)
        UBOOT_ERROR("[Device Raptors] Mailbox_Write Query EDID = %x\n",bIce);

    Fn_Send_Interrupt_Raptors();

    if(!Fn_WaitACK())//read Raptors ACK
    {
        UBOOT_ERROR("[Device Raptors] %s, %d , Setting Wrong!!! Read ACK Fail!!!\n",__func__,__LINE__);
        return FALSE;
    }
    Fn_SetACK_Low();//set ACK low after read it high

    if(Fn_WaitReport())
    {
        get4k2k.command_idx=E_MAPI_MAILBOX_REPORT_EDID_INFO;
        Mailbox_ReadCommand_Report_4K2KVic(&get4k2k);
        if(FALSE == get4k2k.vic_num)
        {
            UBOOT_ERROR("[Device Raptors] Can't get EDID_4K2K Inform !!\n");
            return FALSE;
        }

        if(u8MaxNum>get4k2k.vic_num)
            u8MaxNum=get4k2k.vic_num;

        for(i=0;i<u8MaxNum;i++)
        {
            switch(get4k2k.vic4k2k[i])
            {
            case E_MAPI_RAPTORS_HDMITX_VIDEO_4K2K_Reserved:
                penEDID4K2KVic[i] = E_MAPI_RAPTORS_HDMITX_VIDEO_4K2K_Reserved;
                break;
            case E_MAPI_RAPTORS_HDMITX_VIDEO_4K2K_30Hz:
                penEDID4K2KVic[i] = E_MAPI_RAPTORS_HDMITX_VIDEO_4K2K_30Hz;
                break;
            case E_MAPI_RAPTORS_HDMITX_VIDEO_4K2K_25Hz:
                penEDID4K2KVic[i] = E_MAPI_RAPTORS_HDMITX_VIDEO_4K2K_25Hz;
                break;
            case E_MAPI_RAPTORS_HDMITX_VIDEO_4K2K_24Hz:
                penEDID4K2KVic[i] = E_MAPI_RAPTORS_HDMITX_VIDEO_4K2K_24Hz;
                break;
            case E_MAPI_RAPTORS_HDMITX_VIDEO_4K2K_24Hz_SMPTE:
                penEDID4K2KVic[i] = E_MAPI_RAPTORS_HDMITX_VIDEO_4K2K_24Hz_SMPTE;
                break;
            default:
                break;
            }

        }
        bRet = TRUE;
    }
    else
    {
        UBOOT_ERROR("[Device Raptors] GetEDID_4K2K_Inform Fail! Wait no 4K2K Info!\n");
        return FALSE;
    }

    return bRet;
}


static MS_BOOL GetEDID_Inform(MS_U8 u8Num, MS_U8 *penEDIDVic)
{
    MS_U8 i = 0;
    MS_U8 u8MaxNum = u8Num > 32 ? 32 : u8Num;
    MS_BOOL bRet = FALSE;
    MS_BOOL bIce = FALSE;
    ST_MAILBOX_COMMAND_QUERY_EDID_INFO getVideoNum;
    ST_MAILBOX_COMMAND_REPORT_EDID_INFO_VIDEODES_NUM getNum;
    ST_MAILBOX_COMMAND_QUERY_EDID_INFO setVideoEdid;
    ST_MAILBOX_COMMAND_REPORT_EDID_INFO_VIDEODES getVideoEdid;

    UBOOT_DEBUG("[Device Raptors] %s, %d\n",__func__,__LINE__);

    memset(penEDIDVic, 0 , sizeof(MS_U8)*u8Num);

    //get the number of video descriptors
    getVideoNum.data_len=0x01;
    getVideoNum.query_type=E_MAPI_RAPTORS_QUERY_VIDEO_DESCRIPTOR_NUM;
    getVideoNum.command_idx=E_MAPI_MAILBOX_QUERY_EDID_INFO;

    bIce=Mailbox_WriteCommand_Query_EDID(&getVideoNum);
    if(!bIce)
        UBOOT_ERROR("[Device Raptors] Mailbox_Write Get Num of Video Descriptor = %x\n",bIce);

    Fn_Send_Interrupt_Raptors();

    if(!Fn_WaitACK())//read Raptors ACK
    {
        UBOOT_ERROR("[Device Raptors] %s, %d , Read ACK Fail!!!\n",__func__,__LINE__);
        return FALSE;
    }
    Fn_SetACK_Low();//set ACK low after read it high

    if(Fn_WaitReport())
    {
        getNum.command_idx = E_MAPI_MAILBOX_REPORT_EDID_INFO;
        Mailbox_ReadCommand_Report_Num_VideoDescriptor(&getNum);
        if(FALSE == getNum.viddes_num)
        {
            UBOOT_ERROR("[Device Raptors] Can't get EDID Inform !!\n");
            return FALSE;
        }

        if(u8MaxNum>getNum.viddes_num)
            u8MaxNum=getNum.viddes_num;
    }
    else
    {
        UBOOT_ERROR("[Device Raptors] GetEDID_Inform Fail! Get Number Fail!\n");
        return FALSE;
    }

    //get video descriptor
    setVideoEdid.data_len=0x03;
    setVideoEdid.query_type=E_MAPI_RAPTORS_QUERY_VIDEO_DESCRIPTOR;
    setVideoEdid.des_startIdx=0x00;
    setVideoEdid.des_endIdx= u8MaxNum - 1;
    setVideoEdid.command_idx=E_MAPI_MAILBOX_QUERY_EDID_INFO;

    bIce=Mailbox_WriteCommand_Query_EDID(&setVideoEdid);
    if(!bIce)
        UBOOT_ERROR("[Device Raptors] Mailbox_Write Query EDID Video Descriptor = %x\n",bIce);

    Fn_Send_Interrupt_Raptors();

    if(!Fn_WaitACK())//read Raptors ACK
    {
        UBOOT_ERROR("[Device Raptors] %s, %d , Read ACK Fail!!!\n",__func__,__LINE__);
        return FALSE;
    }
    Fn_SetACK_Low();//set ACK low after read it high

    if(Fn_WaitReport())
    {
        getVideoEdid.command_idx=E_MAPI_MAILBOX_REPORT_EDID_INFO;
        Mailbox_ReadCommand_Report_VideoDescriptor(&getVideoEdid);
        for(i=0;i<u8MaxNum;i++)
        {
             penEDIDVic[i] = getVideoEdid.vid_des[i];
        }

        bRet = TRUE;
    }
    else
    {
        UBOOT_ERROR("[Device Raptors] GetEDID_Inform Fail! Wait no 4K2K Info!\n");
        bRet = FALSE;
    }

    return bRet;
}

static MS_BOOL GetEDID_ColorFmt_Inform(EN_MAPI_HDMITX_TIMING_TYPE enTiming, MS_U8 *pbColorFmt)
{
    MS_BOOL bRet = TRUE;
    MS_BOOL bIce = TRUE;
    ST_MAILBOX_COMMAND_QUERY_EDID_INFO setColorfmt;
    ST_MAILBOX_COMMAND_REPORT_EDID_INFO_COLORFMT getColorfmt;

    //get RX EDID Info
    setColorfmt.data_len=0x02;
    setColorfmt.query_type=E_MAPI_RAPTORS_QUERY_COLOR_FORMAT;
    setColorfmt.command_idx=E_MAPI_MAILBOX_QUERY_EDID_INFO;

    switch(enTiming)
        {
             case E_MAPI_HDMITX_TIMING_576_50P:
                 setColorfmt.timing_colorfmt=E_MAPI_RAPTORS_HDMITX_QUERY_COLORFMTTIMING_720x576p_50Hz;
                 break;

             case E_MAPI_HDMITX_TIMING_480_60P:
                 setColorfmt.timing_colorfmt=E_MAPI_RAPTORS_HDMITX_QUERY_COLORFMTTIMING_720x480p_60Hz;
                 break;

             case E_MAPI_HDMITX_TIMING_720_50P:
                 setColorfmt.timing_colorfmt=E_MAPI_RAPTORS_HDMITX_QUERY_COLORFMTTIMING_1280x720p_50Hz;
                 break;

             case E_MAPI_HDMITX_TIMING_720_60P:
                 setColorfmt.timing_colorfmt=E_MAPI_RAPTORS_HDMITX_QUERY_COLORFMTTIMING_1280x720p_60Hz;
                 break;

             case E_MAPI_HDMITX_TIMING_1080_24P:
                 setColorfmt.timing_colorfmt=E_MAPI_RAPTORS_HDMITX_QUERY_COLORFMTTIMING_1920x1080p_24Hz;
                 break;

             case E_MAPI_HDMITX_TIMING_1080_50I:
                 setColorfmt.timing_colorfmt=E_MAPI_RAPTORS_HDMITX_QUERY_COLORFMTTIMING_1920x1080i_50Hz;
                 break;

             case E_MAPI_HDMITX_TIMING_1080_60I:
                 setColorfmt.timing_colorfmt=E_MAPI_RAPTORS_HDMITX_QUERY_COLORFMTTIMING_1920x1080i_60Hz;
                 break;

             case E_MAPI_HDMITX_TIMING_1080_50P:
                 setColorfmt.timing_colorfmt=E_MAPI_RAPTORS_HDMITX_QUERY_COLORFMTTIMING_1920x1080p_50Hz;
                 break;

             case E_MAPI_HDMITX_TIMING_1080_60P:
                 setColorfmt.timing_colorfmt=E_MAPI_RAPTORS_HDMITX_QUERY_COLORFMTTIMING_1920x1080p_60Hz;
                 break;

             case E_MAPI_HDMITX_TIMING_4K2K_25P:
                 setColorfmt.timing_colorfmt=E_MAPI_RAPTORS_HDMITX_QUERY_COLORFMTTIMING_4K2Kp_25Hz;
                 break;

             case E_MAPI_HDMITX_TIMING_4K2K_30P:
                 setColorfmt.timing_colorfmt=E_MAPI_RAPTORS_HDMITX_QUERY_COLORFMTTIMING_4K2Kp_30Hz;
                 break;

             case E_MAPI_HDMITX_TIMING_4K2K_50P:
                 setColorfmt.timing_colorfmt=E_MAPI_RAPTORS_HDMITX_QUERY_COLORFMTTIMING_4K2Kp_50Hz;
                 break;

             case E_MAPI_HDMITX_TIMING_4K2K_60P:
                 setColorfmt.timing_colorfmt=E_MAPI_RAPTORS_HDMITX_QUERY_COLORFMTTIMING_4K2Kp_60Hz;
                 break;

             case E_MAPI_HDMITX_TIMING_1080_25P:
                 setColorfmt.timing_colorfmt=E_MAPI_RAPTORS_HDMITX_QUERY_COLORFMTTIMING_1920x1080p_25Hz;
                 break;

             case E_MAPI_HDMITX_TIMING_1080_30P:
                 setColorfmt.timing_colorfmt=E_MAPI_RAPTORS_HDMITX_QUERY_COLORFMTTIMING_1920x1080p_30Hz;
                 break;

             case E_MAPI_HDMITX_TIMING_4K2K_24P:
                 setColorfmt.timing_colorfmt=E_MAPI_RAPTORS_HDMITX_QUERY_COLORFMTTIMING_4K2Kp_24Hz;
                 break;

             case E_MAPI_HDMITX_TIMING_4096x2160_24P:
                 setColorfmt.timing_colorfmt=E_MAPI_RAPTORS_HDMITX_QUERY_COLORFMTTIMING_4096x2160p_24Hz;
                 break;

             default:
                 UBOOT_ERROR("[Device Raptors] Wrong Timing for GetEDID_ColorFmt_Inform \n");
                 return FALSE;
        }

    bIce=Mailbox_WriteCommand_Query_EDID(&setColorfmt);
    if(!bIce)
        UBOOT_ERROR("[Device Raptors] GetEDID_ColorDepth_Inform = %x\n",bIce);

    Fn_Send_Interrupt_Raptors();

    if(!Fn_WaitACK())//read Raptors ACK
    {
        UBOOT_ERROR("[Device Raptors] %s, %d , Read ACK Fail!!!\n",__func__,__LINE__);
        return FALSE;
    }
    Fn_SetACK_Low();//set ACK low after read it high

    if(Fn_WaitReport())
    {
        Mailbox_ReadCommand_Report_ColorFormat(&getColorfmt);

        pbColorFmt[E_MAPI_HDMITX_COLOR_RGB444]=(MS_U8)(getColorfmt.supColorFmt & 0x01);
        pbColorFmt[E_MAPI_HDMITX_COLOR_YUV422]=(MS_U8)((getColorfmt.supColorFmt & 0x02)>>1);
        pbColorFmt[E_MAPI_HDMITX_COLOR_YUV444]=(MS_U8)((getColorfmt.supColorFmt & 0x04)>>2);
        pbColorFmt[E_MAPI_HDMITX_COLOR_YUV420]=(MS_U8)((getColorfmt.supColorFmt & 0x08)>>3);

        bRet = TRUE;
    }
    else
    {
        UBOOT_DEBUG("[Device Raptors] GetEDID_ColorFmt_Inform fail! Wait no color format Info!\n");
        bRet = FALSE;
    }

    return bRet;
}

static MS_BOOL checkPanelIniSupport(MS_U8 enTiming, char* customer_ini_path)
{
    char *script = NULL;
    char *s = NULL;
    MS_BOOL ret = FALSE;
    char as8PanelFilePath[BUFFER_SIZE] = "\0";
    U32 filesize = 0;
    UBOOT_TRACE("IN\n");

    vfs_mount(CONFIG);
    script = loadscript(customer_ini_path, &filesize);
    if(script == NULL)
    {
        return FALSE;
    }

    snprintf(as8PanelFilePath, sizeof(as8PanelFilePath),"/config/panel/%s.ini", HdmiTxPanelIndexTable[enTiming]);
    if((s = strstr(script, as8PanelFilePath)) != NULL)
    {
        UBOOT_DEBUG("Panel ini: %s is supported.\n", as8PanelFilePath);
        ret = TRUE;
    }

    if(script)
    {
        free(script);
    }
    vfs_umount();
    UBOOT_TRACE("OK\n");
    return ret;
}

static E_MAPI_RAPTORS_SUPPORT_FORMAT checkReciverSupportFormat(MS_U8 enTiming)
{
    int idx = 0;
    E_MAPI_RAPTORS_SUPPORT_FORMAT result = E_MAPI_RAPTORS_RESOLUTION_NOT_SUPPORT;
    MS_U8 u8Target_4x3 = HDMITX_VIC_NOT_AVAILABLE;
    MS_U8 u8Target_16x9 = HDMITX_VIC_NOT_AVAILABLE;
    EN_MAPI_RAPTORS_HDMITX_VIDEO_4K2K_VIC enEdid4k2kVIC = E_MAPI_RAPTORS_HDMITX_VIDEO_4K2K_Reserved;

    if(FALSE == g_bEDID4K2KReady && FALSE == g_bEDIDReady)
    {
        UBOOT_ERROR("Error: Get EDID Failed\n");
        return E_MAPI_RAPTORS_GET_EDID_FAILED;
    }

    //VSDB Case
    if(TRUE == g_bEDID4K2KReady)
    {
        enEdid4k2kVIC = E_MAPI_RAPTORS_HDMITX_VIDEO_4K2K_Reserved;
        //search video tag of 4k2k edid
        switch (enTiming)
        {
            case E_MAPI_ROCKY_RES_4K2Kp_30Hz:
                enEdid4k2kVIC = E_MAPI_RAPTORS_HDMITX_VIDEO_4K2K_30Hz;
                break;
            case E_MAPI_ROCKY_RES_4K2Kp_25Hz:
                enEdid4k2kVIC = E_MAPI_RAPTORS_HDMITX_VIDEO_4K2K_25Hz;
                break;
            case E_MAPI_ROCKY_RES_4K2Kp_24Hz:
                enEdid4k2kVIC = E_MAPI_RAPTORS_HDMITX_VIDEO_4K2K_24Hz;
                break;
            default:
                UBOOT_DEBUG("Invalid VIC 4K2K Timing Type(%d)\n", enTiming);
                break;
        }
        //VSDB Debug Msg
        if(enEdid4k2kVIC != E_MAPI_RAPTORS_HDMITX_VIDEO_4K2K_Reserved)
        {
            for(idx = 0; idx < EDID_VIC_4K2K_NUM; idx++)
            {
                if(g_u8HDMITx4K2K[idx] == enEdid4k2kVIC)
                {
                    result = E_MAPI_RAPTORS_RESOLUTION_SUPPORT_SUCCESS;
                }
            }
        }
    }

    //VSD Case
    if(TRUE == g_bEDIDReady)
    {
        u8Target_4x3 = HDMITX_VIC_NOT_AVAILABLE;
        u8Target_16x9 = HDMITX_VIC_NOT_AVAILABLE;
        //search video tag of normal edid
        switch(enTiming)
        {
            case E_MAPI_ROCKY_RES_720x480i:
                u8Target_16x9 = HDMITX_VIC_720x480i_60_16_9;
                u8Target_4x3 = HDMITX_VIC_720x480i_60_4_3;
                break;
            case E_MAPI_ROCKY_RES_640x480p:
                u8Target_16x9 = HDMITX_VIC_720x480p_60_16_9;
                u8Target_4x3 = HDMITX_VIC_720x480p_60_4_3;
                break;
            case E_MAPI_ROCKY_RES_720x576i:
                u8Target_16x9 = HDMITX_VIC_720x576i_50_16_9;
                u8Target_4x3 = HDMITX_VIC_720x576i_50_4_3;
                break;
            case E_MAPI_ROCKY_RES_720x576p:
                u8Target_16x9 = HDMITX_VIC_720x576p_50_16_9;
                u8Target_4x3 = HDMITX_VIC_720x576p_50_4_3;
                break;
            case E_MAPI_ROCKY_RES_1280x720p_60Hz:
                u8Target_16x9 = HDMITX_VIC_1280x720p_60_16_9;
                break;
            case E_MAPI_ROCKY_RES_1280x720p_50Hz:
                u8Target_16x9 = HDMITX_VIC_1280x720p_50_16_9;
                break;
            case E_MAPI_ROCKY_RES_1920x1080i_60Hz:
                u8Target_16x9 = HDMITX_VIC_1920x1080i_60_16_9;
                break;
            case E_MAPI_ROCKY_RES_1920x1080p_30Hz:
                u8Target_16x9 = HDMITX_VIC_1920x1080p_30_16_9;
                break;
            case E_MAPI_ROCKY_RES_1920x1080p_60Hz:
                u8Target_16x9 = HDMITX_VIC_1920x1080p_60_16_9;
                break;
            case E_MAPI_ROCKY_RES_1920x1080i_50Hz:
                u8Target_16x9 = HDMITX_VIC_1920x1080i_50_16_9;
                break;
            case E_MAPI_ROCKY_RES_1920x1080p_24Hz:
                u8Target_16x9 = HDMITX_VIC_1920x1080p_24_16_9;
                break;
            case E_MAPI_ROCKY_RES_1920x1080p_25Hz:
                u8Target_16x9 = HDMITX_VIC_1920x1080p_25_16_9;
                break;
            case E_MAPI_ROCKY_RES_1920x1080p_50Hz:
                u8Target_16x9 = HDMITX_VIC_1920x1080p_50_16_9;
                break;
            case E_MAPI_ROCKY_RES_4K2Kp_30Hz:
                u8Target_16x9 = HDMITX_VIC_3840x2160p_30_16_9;
                u8Target_4x3 =  HDMITX_VIC_3840x2160p_30_64_27;
                break;
            case E_MAPI_ROCKY_RES_4K2Kp_25Hz:
                u8Target_16x9 = HDMITX_VIC_3840x2160p_25_16_9;
                u8Target_4x3 =  HDMITX_VIC_3840x2160p_25_64_27;
                break;
            case E_MAPI_ROCKY_RES_4K2Kp_24Hz:
                u8Target_16x9 = HDMITX_VIC_3840x2160p_24_16_9;
                u8Target_4x3 =  HDMITX_VIC_3840x2160p_24_64_27;
                break;
            case E_MAPI_ROCKY_RES_4K2Kp_50Hz:
                u8Target_16x9 = HDMITX_VIC_3840x2160p_50_16_9;
                u8Target_4x3 =  HDMITX_VIC_3840x2160p_50_64_27;
                break;
            case E_MAPI_ROCKY_RES_4K2Kp_60Hz:
            case E_MAPI_ROCKY_RES_4K2Kp_60Hz_RB:
                u8Target_16x9 = HDMITX_VIC_3840x2160p_60_16_9;
                u8Target_4x3 =  HDMITX_VIC_3840x2160p_60_64_27;
                break;
            default:
                UBOOT_DEBUG("Invalid VIC Timing Type(%d)\n", enTiming);
                break;
        }

        //VSD Debug Msg
        for(idx = 0; idx < EDID_VIC_NUM; idx++)
        {
            //doesn't consider native timing flag
            if(g_u8HDMITx[idx] == 0)
            {
                break;
            }
            else if(((g_u8HDMITx[idx]&0x7F) == u8Target_16x9) || ((g_u8HDMITx[idx]&0x7F) == u8Target_4x3))
            {
                result = E_MAPI_RAPTORS_RESOLUTION_SUPPORT_SUCCESS;
                if((g_u8HDMITx[idx] & 0x80) && (!strcmp(getenv("hdmitx_edid"), "2")))
                {
                    result = E_MAPI_RAPTORS_RESOLUTION_BESTSUPPORT_SUCCESS;
                }
            }
        }
    }

    if((enTiming == E_MAPI_ROCKY_RES_4K2Kp_60Hz_RB)
      && (result == E_MAPI_RAPTORS_RESOLUTION_SUPPORT_SUCCESS))
    {
        result = E_MAPI_RAPTORS_RESOLUTION_NOT_SUPPORT;
        char *s;
        s = getenv ("customer_ini_path");
        if(s)
        {
            if((checkPanelIniSupport(E_MAPI_ROCKY_RES_4K2Kp_60Hz_RB, s) == TRUE)
              && (checkPanelIniSupport(E_MAPI_ROCKY_RES_4K2Kp_60Hz, s) == FALSE))
            {
                result = E_MAPI_RAPTORS_RESOLUTION_SUPPORT_SUCCESS;
            }
        }
    }
    return result;
}

static MS_BOOL SelectProperTimingFromEDID(const MS_U8 *pTimingList, const MS_U8 TimingListSize, MS_U8 *enSelectedTiming)
{
    MS_U8 enTiming = 0;
    MS_U8 idx = 0;

    if(pTimingList == NULL || TimingListSize == 0)
    {
        UBOOT_ERROR("!!!!! Error: Timing list is empty.\n");
        return FALSE;
    }

    for(idx = 0; idx < TimingListSize; idx++)
    {
        enTiming = pTimingList[idx];
        UBOOT_DEBUG("[%s:%d] enTiming=0x%X\n", __FUNCTION__, __LINE__, enTiming);
        if(checkReciverSupportFormat(enTiming) == E_MAPI_RAPTORS_RESOLUTION_SUPPORT_SUCCESS)
        {
            *enSelectedTiming = enTiming;
            return TRUE;
        }
    }

    return FALSE;
}

MS_BOOL msHdmitx_Rocket_Init(EN_MAPI_DEVICE_ROCKY_VIDEO_TIMING outPutType )
{
    MS_U8 Data_len_total;
    MS_U8 pData[22];
    char *pEnv = NULL;
    E_MAPI_RAPTORS_SUPPORT_FORMAT status;
    MS_U8 u8_RxStatus = 0;
    UBOOT_TRACE("IN\n");
#ifdef EXT_RAPTORS_RET
    mdrv_gpio_set_low(EXT_RAPTORS_RET);
#endif
    MDrv_Rocker2_SWI2C_Init();
    Init();
    pEnv = getenv("hdmitx_edid");
    u8_RxStatus = (MS_U8)( Raptors_r2byte(0x2E66) & 0x1);
    printf("[Device Raptors]RxStatus:%d, Reg2E_66:%x\n", u8_RxStatus, Raptors_r2byte(0x2E66));

    if((pEnv != NULL) && (u8_RxStatus))
    {
        MS_U8 SelectedTiming = E_MAPI_ROCKY_RES_1920x1080p_60Hz;
        MS_U8 BestTiming = E_MAPI_ROCKY_RES_1920x1080p_60Hz;
        MS_BOOL bResetTiming = FALSE;
        MS_BOOL bBestSupportTiming = FALSE;
        char p_str[] = "\0";
        char as8PanelFilePath[BUFFER_SIZE] = "\0";

        //step 1. get edid info
        //case: non-4K2K VIC
        memset(g_u8HDMITx, 0, EDID_VIC_NUM * sizeof(MS_U8));
        g_bEDIDReady = GetEDID_Inform(EDID_VIC_NUM, g_u8HDMITx);

        //case: 4K2K VIC
        memset(g_u8HDMITx4K2K, 0, EDID_VIC_4K2K_NUM * sizeof(MS_U8));
        g_bEDID4K2KReady = GetEDID_4K2K_Inform(EDID_VIC_4K2K_NUM, g_u8HDMITx4K2K);

        //step 2. check bestTiming support  if env hdmitx_edid == 2
        if(!(strcmp(pEnv, "2")))
        {
            UBOOT_DEBUG("Search EDID support best timing... \n");
            for(SelectedTiming = 0; SelectedTiming < E_MAPI_ROCKY_RES_MAX; SelectedTiming++)
            {
                if( checkReciverSupportFormat(SelectedTiming) == E_MAPI_RAPTORS_RESOLUTION_BESTSUPPORT_SUCCESS)
                {
                    UBOOT_DEBUG("EDID support best timing is %d\n", SelectedTiming);
                    BestTiming = SelectedTiming;
                    bBestSupportTiming = TRUE;
                }
            }

            if((bBestSupportTiming == TRUE) && (BestTiming != outPutType))
            {
                printf("select best resolution=%d from EDID\n", BestTiming);
                printf("panel_name: %s.ini\n", HdmiTxPanelIndexTable[BestTiming]);
                SelectedTiming = BestTiming;
                bResetTiming = TRUE;
            }
        }
        //step 3. check highest Timing support if env hdmitx_edid == 3
        else if(!(strcmp(pEnv, "3")))
        {
            UBOOT_DEBUG("Search EDID support highest timing... \n");
            for(SelectedTiming = E_MAPI_ROCKY_RES_MAX; SelectedTiming > 0; SelectedTiming--)
            {
                if(checkReciverSupportFormat(SelectedTiming) == E_MAPI_RAPTORS_RESOLUTION_SUPPORT_SUCCESS)
                {
                    if((getMapiTimingType((EN_MAPI_DEVICE_ROCKY_VIDEO_TIMING) SelectedTiming) != E_MAPI_HDMITX_TIMING_1080_60P) || (SelectedTiming == E_MAPI_ROCKY_RES_1920x1080p_60Hz))
                    {
                        if(SelectedTiming != outPutType)
                        {
                            printf("select highest resolution=%d from EDID\n", SelectedTiming);
                            printf("panel_name: %s.ini\n", HdmiTxPanelIndexTable[SelectedTiming]);
                            bResetTiming = TRUE;
                        }
                         break;
                    }
                }
            }
        }

        //step 4. check Current Timing support  if env hdmitx_edid == 1 or bestTimingSupport not exsits
        if(!(strcmp(pEnv, "1")) || ((bBestSupportTiming == FALSE) && !(strcmp(pEnv, "2"))))
        {
            SelectedTiming = E_MAPI_ROCKY_RES_1920x1080p_60Hz;
            status = checkReciverSupportFormat((MS_U8)outPutType);
            if(E_MAPI_RAPTORS_RESOLUTION_NOT_SUPPORT == status)
            {
                UBOOT_DEBUG("Reciver not support resolution:%d, select proper timng from EDID \n", outPutType);
                if(TRUE == SelectProperTimingFromEDID(HdmiTxTimgList, sizeof(HdmiTxTimgList), &SelectedTiming))
                {
                    printf("select proper resolution=%d from EDID\n", SelectedTiming);
                    printf("panel_name: %s.ini\n", HdmiTxPanelIndexTable[SelectedTiming]);
                    bResetTiming = TRUE;
                }
            }
            else if(E_MAPI_RAPTORS_GET_EDID_FAILED == status)
            {
                UBOOT_DEBUG("Get EDID failed, resolution:%d. \n", outPutType);
            }
            else if((E_MAPI_RAPTORS_RESOLUTION_SUPPORT_SUCCESS == status) || (E_MAPI_RAPTORS_RESOLUTION_BESTSUPPORT_SUCCESS == status))
            {
                UBOOT_DEBUG("Reciver support resolution:%d. \n", outPutType);
            }
        }

        //step 5. reset Current Timing if Current Timing is not support in EDID
        if(bResetTiming == TRUE)
        {
            snprintf(p_str, sizeof(p_str), "%ld", SelectedTiming);
            setenv("resolution", p_str);
            setenv("resolution_reset", p_str);

            MApi_SetEnv2BootArgs("resolution=", HdmiTxTimingIndexTable[SelectedTiming]);
            snprintf(as8PanelFilePath, sizeof(as8PanelFilePath),"/config/panel/%s.ini", HdmiTxPanelIndexTable[SelectedTiming]);
            MApi_SetEnv2BootArgs("panel_path=", as8PanelFilePath);
            setenv("panel_path", as8PanelFilePath);
            setenv("panel_name", HdmiTxPanelIndexTable[SelectedTiming]);

            setenv("db_table", "0");
            saveenv();
            run_command("reset", 0);
        }
    }
    else
    {
        printf("Check Timing From EDID OFF !\n");
    }
    //use no combine combo
    //TurnOnOff(FALSE);
    //SetTimingType(outPutType);
    //TurnOnOff(TRUE);

    //use combine command
    //set AVmute
    ST_MAILBOX_COMMAND_TIMING_CHANGE_AVMUTE setOff;
    setOff.command_idx=E_MAPI_MAILBOX_AVMUTE_TIMING_RESET;
    setOff.data_len=0x02;
    setOff.avmute_flag=TRUE;
    setOff.timing_flag=TRUE;
    setOff.video_flag=FALSE;
    setOff.audio_flag=FALSE;
    setOff.enAV_mute=E_MAPI_RAPTORS_HDMITX_AVMUTE_ENABLE;
    setOff.enTiming =E_MAPI_RAPTORS_HDMITX_TIMING_CHANGE_ENABLE;

    switch(outPutType)
    {
    case E_MAPI_ROCKY_RES_720x576p:
        genHDMITx_VideoTiming = E_MAPI_RAPTORS_HDMITX_TIMING_720x576p_50Hz;
        break;

    case E_MAPI_ROCKY_RES_720x480p:
        genHDMITx_VideoTiming = E_MAPI_RAPTORS_HDMITX_TIMING_720x480p_60Hz;
        break;

    case E_MAPI_ROCKY_RES_1280x720p_50Hz:
        genHDMITx_VideoTiming = E_MAPI_RAPTORS_HDMITX_TIMING_1280x720p_50Hz;
        break;

    case E_MAPI_ROCKY_RES_1280x720p_60Hz:
        genHDMITx_VideoTiming = E_MAPI_RAPTORS_HDMITX_TIMING_1280x720p_60Hz;
        break;

    case E_MAPI_ROCKY_RES_1920x1080p_24Hz:
        genHDMITx_VideoTiming = E_MAPI_RAPTORS_HDMITX_TIMING_1920x1080p_24Hz;
        break;

    case E_MAPI_ROCKY_RES_1920x1080i_50Hz:
        genHDMITx_VideoTiming = E_MAPI_RAPTORS_HDMITX_TIMING_1920x1080i_50Hz;
        break;

    case E_MAPI_ROCKY_RES_1920x1080i_60Hz:
        genHDMITx_VideoTiming = E_MAPI_RAPTORS_HDMITX_TIMING_1920x1080i_60Hz;
        break;

    case E_MAPI_ROCKY_RES_1920x1080p_50Hz:
        genHDMITx_VideoTiming = E_MAPI_RAPTORS_HDMITX_TIMING_1920x1080p_50Hz;
        break;

    case E_MAPI_ROCKY_RES_1920x1080p_60Hz:
        genHDMITx_VideoTiming = E_MAPI_RAPTORS_HDMITX_TIMING_1920x1080p_60Hz;
        break;

    case E_MAPI_ROCKY_RES_4K2Kp_25Hz:
        genHDMITx_VideoTiming = E_MAPI_RAPTORS_HDMITX_TIMING_4K2Kp_25Hz;
        break;

    case E_MAPI_ROCKY_RES_4K2Kp_30Hz:
        genHDMITx_VideoTiming = E_MAPI_RAPTORS_HDMITX_TIMING_4K2Kp_30Hz;
        break;

    case E_MAPI_ROCKY_RES_4K2Kp_50Hz:
        genHDMITx_VideoTiming = E_MAPI_RAPTORS_HDMITX_TIMING_4K2Kp_50Hz;
        break;

    case E_MAPI_ROCKY_RES_4K2Kp_60Hz:
        genHDMITx_VideoTiming = E_MAPI_RAPTORS_HDMITX_TIMING_4K2Kp_60Hz;
        break;

    case E_MAPI_ROCKY_RES_4K2Kp_60Hz_RB:
        genHDMITx_VideoTiming = E_MAPI_RAPTORS_HDMITX_TIMING_4K2Kp_60Hz_RB;
        break;

    case E_MAPI_ROCKY_RES_4K2Kp_24Hz:
        genHDMITx_VideoTiming = E_MAPI_RAPTORS_HDMITX_TIMING_4K2Kp_24Hz;
        break;

    case E_MAPI_ROCKY_RES_4096x2160p_24Hz:
        genHDMITx_VideoTiming =E_MAPI_RAPTORS_HDMITX_TIMING_4096x2160p_24Hz;
        break;

    default:
        UBOOT_DEBUG("Wrong Mode of EN_MAPI_HDMITX_TIMING_TYPE \n");
        return FALSE;
    }

    if((outPutType == E_MAPI_ROCKY_RES_4K2Kp_50Hz) || (outPutType == E_MAPI_ROCKY_RES_4K2Kp_60Hz))
    {
        MS_U8 color_fmt[4] = {0};
        if(GetEDID_ColorFmt_Inform((EN_MAPI_HDMITX_TIMING_TYPE) getMapiTimingType(outPutType), &color_fmt) == TRUE)
        {
            MS_U8 color_index = 0;
            for(color_index=0; color_index<4; color_index++)
                UBOOT_DEBUG("support color_fmt[%d] = %d\n", color_index, color_fmt[color_index]);
            if((color_fmt[E_MAPI_HDMITX_COLOR_YUV420] == TRUE) && (color_fmt[E_MAPI_HDMITX_COLOR_YUV422] != TRUE))
                genHDMITx_OutColor = E_MAPI_RAPTORS_HDMITX_COLORS_OUTPUT_YUV_420;
        }
    }
    genMAPI_HDMITxTming = outPutType;

    //set Raptors video
    ST_MAILBOX_COMMAND_CONFIQ_VIDEO setTiming;
    setTiming.command_idx=E_MAPI_MAILBOX_CONFIQ_VIDEO;
    setTiming.data_len=0x05;
    setTiming.timing_present=TRUE;
    setTiming.color_present=TRUE;
    setTiming.VSinfo_3D_present=FALSE;
    setTiming.analog_present=FALSE;
    setTiming.timing_idx=genHDMITx_VideoTiming;
    st_hdmitx_setting u32HDMITXAddr;
    pEnv = getenv("repeater");
    if(pEnv != NULL && !strcmp(pEnv, "1"))
    {
        UBOOT_DEBUG("Output mode is HDMI HDCP\n");
        genHDMITX_Mode = E_MAPI_RAPTORS_HDMITX_VIDEO_OUTPUT_HDMI_HDCP;
    }
    else
    {
        UBOOT_DEBUG("Output mode is HDMI\n");
        genHDMITX_Mode = E_MAPI_RAPTORS_HDMITX_VIDEO_OUTPUT_HDMI;
    }

    if(Read_HDMITXFromFlash(&u32HDMITXAddr)==0)
    {
        setTiming.color_depth=(EN_MAPI_RAPTORS_HDMITX_COLOR_DEPTH) u32HDMITXAddr.enColorDepth;
        setTiming.in_color=(EN_MAPI_RAPTORS_HDMITX_INPUT_COLOR_TYPE) u32HDMITXAddr.enInColorFmt;
        setTiming.output_mode=(EN_MAPI_RAPTORS_HDMITX_VIDEO_OUTPUT_TYPE) u32HDMITXAddr.enOutputMode;
        setTiming.out_color=(EN_MAPI_RAPTORS_HDMITX_OUTPUT_COLOR_TYPE) u32HDMITXAddr.enOutColorFmt;
        if(pEnv != NULL && !strcmp(pEnv, "1"))
        {
            setTiming.output_mode=E_MAPI_RAPTORS_HDMITX_VIDEO_OUTPUT_HDMI_HDCP;
        }
        else
        {
            setTiming.output_mode = E_MAPI_RAPTORS_HDMITX_VIDEO_OUTPUT_HDMI;
        }
    }
    else
   {
        setTiming.color_depth=genHDMITx_ColorDepth;
        setTiming.in_color=genHDMITx_InColor;
        setTiming.output_mode=genHDMITX_Mode;
        setTiming.out_color=genHDMITx_OutColor;
        UBOOT_DEBUG("%s: Read_HDMITXFromFlash() failed, at %d\n", __func__, __LINE__);
   }
    UBOOT_DEBUG("Reciver support genHDMITx_VideoTiming :%d. \n", genHDMITx_VideoTiming);
    UBOOT_DEBUG("Reciver support genMAPI_HDMITxTming :%d. \n", genMAPI_HDMITxTming);
    UBOOT_DEBUG("Reciver support genHDMITX_Mode :%d. \n", genHDMITX_Mode);

    if(setTiming.timing_idx == E_MAPI_RAPTORS_HDMITX_TIMING_4K2Kp_60Hz_RB)
    {
        UBOOT_DEBUG("HDMI Mode: 4K2K60_Reduce, Change color depth to 10bit, RGB444\n");
        setTiming.color_depth = E_MAPI_RAPTORS_HDMITX_COLORS_10bits;
        setTiming.out_color = E_MAPI_RAPTORS_HDMITX_COLORS_OUTPUT_RGB_444;
    }

    if(setTiming.in_color == E_MAPI_RAPTORS_HDMITX_COLORS_INPUT_RGB && setTiming.out_color != E_MAPI_RAPTORS_HDMITX_COLORS_OUTPUT_RGB_444)
    {
        setTiming.ext_present = TRUE;
        setTiming.sc_3x3_matrix_present = TRUE;
        setTiming.sc_3x3_matrix = genHDMITX_3x3Matrix;
    }
    //set Raptors audio
    ST_MAILBOX_COMMAND_CONFIQ_AUDIO setAudio;
    setAudio.command_idx=E_MAPI_MAILBOX_CONFIQ_AUDIO;
    setAudio.data_len=0x05;
    setAudio.inform_present=TRUE;
    setAudio.source_present=TRUE;
    setAudio.fmt_present=TRUE;
    setAudio.frequency=genAudio_Freq;
    setAudio.channel_num=genAudio_Ch_Cnt;
    setAudio.code_type=genAudio_CodeType;
    setAudio.source_type=genAudio_SrcType;
    setAudio.source_fmt=genAudio_SrcFmt;

    //set AVmute off
    ST_MAILBOX_COMMAND_TIMING_CHANGE_AVMUTE setOn;
    setOn.command_idx=E_MAPI_MAILBOX_AVMUTE_TIMING_RESET;
    setOn.data_len=0x02;
    setOn.avmute_flag=TRUE;
    setOn.timing_flag=TRUE;
    setOn.video_flag=FALSE;
    setOn.audio_flag=FALSE;
    setOn.enAV_mute=E_MAPI_RAPTORS_HDMITX_AVMUTE_DISABLE;
    setOn.enTiming =E_MAPI_RAPTORS_HDMITX_TIMING_CHANGE_DISABLE;

    //total data_length
    Data_len_total=setOff.data_len+setTiming.data_len+setAudio.data_len+setOn.data_len+0x08;

    //combine data
    //setAVmute
    pData[0]=(MS_U8)setOff.command_idx;
    pData[1]=(MS_U8)setOff.data_len;
    pData[2]=(setOff.timing_flag<<3)|(setOff.avmute_flag<<2)|(setOff.audio_flag<<1)|setOff.video_flag;
    pData[3]=(setOff.enTiming<<3)|(setOff.enAV_mute<<2)|(setOff.enAudio_mute<<1)|setOff.enVideo_mute;
    //setTiming
    pData[4]=(MS_U8)setTiming.command_idx;
    pData[5]=(MS_U8)setTiming.data_len;
    pData[6]=(setTiming.ext_present<<7)|(setTiming.analog_present<<4)|(setTiming.VSinfo_3D_present<<2)|(setTiming.color_present<<1)|setTiming.timing_present;
    pData[7]=(setTiming.sc_3x3_matrix_present<<1);
    pData[8]=(MS_U8)setTiming.timing_idx;
    pData[9]=(setTiming.out_color<<6)|(setTiming.in_color<<5)|(setTiming.output_mode<<3)|setTiming.color_depth;
    pData[10]=(setTiming.sc_3x3_matrix<<4);
    //setAudio
    pData[11]=(MS_U8)setAudio.command_idx;
    pData[12]=(MS_U8)setAudio.data_len;
    pData[13]=(setAudio.fmt_present<<2)|(setAudio.source_present<<1)|setAudio.inform_present;
    pData[14]=(setAudio.channel_num<<4)|setAudio.frequency;
    pData[15]=(MS_U8)setAudio.code_type;
    pData[16]=(MS_U8)setAudio.source_type;
    pData[17]=(MS_U8)setAudio.source_fmt;
    //setAVmute off
    pData[18]=(MS_U8)setOn.command_idx;
    pData[19]=(MS_U8)setOn.data_len;
    pData[20]=(setOn.timing_flag<<3)|(setOn.avmute_flag<<2)|(setOn.audio_flag<<1)|setOn.video_flag;
    pData[21]=(setOn.enTiming<<3)|(setOn.enAV_mute<<2)|(setOn.enAudio_mute<<1)|setOn.enVideo_mute;

    Mailbox_WriteCommand(E_MAPI_MAILBOX_COMBINE_COMMOND,Data_len_total,pData);
    Fn_Send_Interrupt_Raptors();

    UBOOT_TRACE("OK\n");
    return TRUE;

}
