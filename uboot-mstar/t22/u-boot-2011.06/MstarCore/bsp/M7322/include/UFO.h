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
/// @brief  MStar Common Interface Header File
/// @note   utopia feature definition file
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _UFO_H_
#define _UFO_H_

/////////////////////////////////////////////////////////
/// UTOPIA CODELINE, DO NOT MODIFY
/////////////////////////////////////////////////////////
#define UFO_PUBLIC_HEADER_700

/////////////////////////////////////////////////////////
/// DMX lib feature define
/////////////////////////////////////////////////////////
#define UFO_DMX_FQ
#define UFO_DMX_TSO
//#define UFO_DMX_TSO2

/////////////////////////////////////////////////////////
/// XC lib feature define
/////////////////////////////////////////////////////////
//#define UFO_XC_FORCEWIRTE
//#define UFO_XC_FORCEWRITE
//#define UFO_XC_HDMI_5V_DETECT
#define UFO_XC_PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
#define UFO_XC_PQ_PATH

/*
** XC Pixelshift new mechanism define
*/
#define UFO_XC_SETPIXELSHIFT

/*
** XC HDR function
*/
#define UFO_XC_HDR

#ifdef UFO_XC_HDR
/*
** XC HDR version
*/
#define UFO_XC_HDR_VERSION 2   // for HW supported Dolby/Open HDR
#endif

/*
** XC test pattern function
*/
#define UFO_XC_TEST_PATTERN

/*
** XC auto download function
*/
#define UFO_XC_AUTO_DOWNLOAD

/*
** xc zorder
*/
#define UFO_XC_ZORDER

/*
** XC SUPPORT FRC
*/
#define UFO_XC_FRC_INSIDE

/*
** XC SUPPORT DS PQ relat to SUPPORT_PQ_DS
*/
#define UFO_XC_DS_PQ

#ifdef UFO_XC_DS_PQ
/*
** PQ DS version
*/
//0: For support GRule only and add cmd in MApi_XC_SetWindow()   1: For support Grule and XRule and add cmd in MDrv_XC_SetDSInfo()
#define UFO_XC_DS_PQ_VERSION 0
#endif

#define UFO_XC_SET_DSINFO_V0

#define UFO_XC_SETWINDOW_LITE

/*
** XC SUPPORT DS PQ relat to SUPPORT_PQ_DS
*/
#define UFO_XC_SWDR


/////////////////////////////////////////////////////////
/// GOP lib feature define
/////////////////////////////////////////////////////////
#define UFO_GOP_DIP_PINPON

/////////////////////////////////////////////////////////
/// DIP lib feature define
/////////////////////////////////////////////////////////
#define UFO_DIP_SELECT_TILE_BLOCK

/////////////////////////////////////////////////////////
/// Presetcontrol for TEE
/////////////////////////////////////////////////////////
#define UFO_VDEC_TEE_VPU

/////////////////////////////////////////////////////////
/// GFX function
/////////////////////////////////////////////////////////
#define UFO_GFX_TRIANGLE
#define UFO_GFX_SPAN


/////////////////////////////////////////////////////////
/// MVOP GET SUPPORTED MAX FPS
/////////////////////////////////////////////////////////
#define UFO_MVOP_GET_MAX_FPS

/////////////////////////////////////////////////////////
/// MVOP RESET SETTING
/////////////////////////////////////////////////////////
#define UFO_MVOP_RESET_SETTING



//MI support HDCP2.2
#define UFO_HDCP2_2


/////////////////////////////////////////////////////////
/// Get MVOP HSK Mode Blk screen
/////////////////////////////////////////////////////////
#define UFO_MVOP_GET_IS_MVOP_AUTO_GEN_BLACK

/////////////////////////////////////////////////////////
/// MVOP Dolby HDR
/////////////////////////////////////////////////////////
#define UFO_MVOP_DOLBY_HDR

#define IMPORT_PORTMAPPING

/// XC 3D format, FRC in scaler
/////////////////////////////////////////////////////////
#define UFO_XC_GET_3D_FORMAT

/////////////////////////////////////////////////////////
/// XC SUPPORT Dual miu
/////////////////////////////////////////////////////////
//#define UFO_XC_SUPPORT_DUAL_MIU

/////////////////////////////////////////////////////////
/// MVOP Support Stream Info Setting
/////////////////////////////////////////////////////////
#define UFO_MVOP_STREAM_INFO

/////////////////////////////////////////////////////////
/// MVOP Support FBDec Clk
/////////////////////////////////////////////////////////
#define UFO_MVOP_FB_DEC_CLK

/////////////////////////////////////////////////////////
/// DEMOD feature define
/// support VCM
/////////////////////////////////////////////////////////
#define UFO_SUPPORT_VCM
#define UFO_DEMOD_DVBS_TSCONTROL
/////////////////////////////////////////////////////////
/// MDebug feature define
/////////////////////////////////////////////////////////
#define UFO_MDEBUG_SUPPORT

/////////////////////////////////////////////////////////
/// Audio feature define
/////////////////////////////////////////////////////////
#define UFO_AUDIO_AD_SUPPORT
#define UFO_AUDIO_AC4_SUPPORT
#define UFO_AUDIO_MPEGH_SUPPORT
#define UFO_AUDIO_OPUS_SUPPORT
//#define UFO_AUDIO_CODEC_CAPABILITY_BY_LICENCE
#define UFO_AUDIO_MULTI_CHANNEL
//#define UFO_AUDIO_PCM_MIXER_SUPPORT
//#define UFO_AUDIO_SPDIF_NONPCM_PAUSE
//#define UFO_AUDIO_AVMONITOR_SYNC
//#define UFO_AUDIO_HDMI_RX_HBR_SW_CTRL
#define UFO_AUDIO_DOLBY_DAP_UI_SUPPORT
//#define UFO_AUDIO_FW_TEMPO_SUPPORT
#define UFO_AUDIO_SPDIF_NONPCM_ADDRAWPTR
#define UFO_AUDIO_ATMOS_FLUSH_EVO_METATDATA_BUFFER
#define UFO_AUDIO_EVO_SYNC_CTRL_FOR_ATMOS_ENC
#define UFO_AUDIO_ALWAYS_ENCODE

/////////////////////////////////////////////////////////
/// SYS lib feature define
/////////////////////////////////////////////////////////
#define UFO_SYS_TEE_INFO_SET_MBOOT

/////////////////////////////////////////////////////////
/// XC Support Dynamic Contrl DNR in DS Case
///
///No need this UFO,because DNR and force bob mode now is controlled in QMap DS XRule
/////////////////////////////////////////////////////////
//#define UFO_XC_SWDS_DYMAMIC_CONTROL_DNR

/////////////////////////////////////////////////////////
/// DIP support MFDec
/////////////////////////////////////////////////////////
#define UFO_DIP_SUPPORT_MFDEC

/////////////////////////////////////////////////////////
/// xc support bwd
/////////////////////////////////////////////////////////
#define SUPPORT_BWD

#endif // _UFO_H_
