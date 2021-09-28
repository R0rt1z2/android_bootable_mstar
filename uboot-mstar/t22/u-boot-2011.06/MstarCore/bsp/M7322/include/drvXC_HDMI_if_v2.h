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

#ifndef _XC_HDMI_IF_H_
#define _XC_HDMI_IF_H_

#include "MsTypes.h"
#include "UFO.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define HDMI_GET_PACKET_LENGTH              (32U)
#define HDMI_GET_MULTIVS_PACKET_LENGTH      (121U)

#if !defined(UFO_PUBLIC_HEADER_500_3)
typedef enum
{
    //HDMI Rx property
    MAPI_CMD_HDMIRX_INITIAL,
    MAPI_CMD_HDMIRX_STABLE_POLLING,
    MAPI_CMD_HDMIRX_GET_SOURCE_VERSION,
    MAPI_CMD_HDMIRX_CHECK_HDCP_STATE,
    MAPI_CMD_HDMIRX_CHECK_HDCP_ENC_STATE,
    MAPI_CMD_HDMIRX_CHECK_HDCP14_KEY,
    MAPI_CMD_HDMIRX_GET_HDCP14_KEY_VAILD,
    MAPI_CMD_HDMIRX_SET_HPD_INVERT_FLAG,
    MAPI_CMD_HDMIRX_GET_PACKET_RECEIVE,
    /*************************** HDCP Repeater ***************************/
    MAPI_CMD_HDMIRX_WRITE_X74,
    MAPI_CMD_HDMIRX_READ_X74,
    MAPI_CMD_HDMIRX_SET_REPEATER,
    MAPI_CMD_HDMIRX_SET_BSTATUS,
    MAPI_CMD_HDMIRX_SET_HDMI_MODE,
    MAPI_CMD_HDMIRX_GET_INTERRUPT_STATUS,
    MAPI_CMD_HDMIRX_WRITE_KSV_LIST,
    MAPI_CMD_HDMIRX_SET_VPRIME,
    /*************************** HDCP Repeater ***************************/
    MAPI_CMD_HDMIRX_HDCP22_FILL_CIPHER_KEY,
    MAPI_CMD_HDMIRX_HDCP22_PORT_INIT,
    MAPI_CMD_HDMIRX_HDCP22_POLLING_READ_DONE,
    MAPI_CMD_HDMIRX_HDCP22_ENABLE_CIPHER,
    MAPI_CMD_HDMIRX_HDCP22_SEND_MESSAGE,
    MAPI_CMD_HDMIRX_HDCP22_HANDLER,
    MAPI_CMD_HDMIRX_HDCP22_WAIT_EVENT,
    MAPI_CMD_HDMIRX_HDCP22_SET_EVENT,
    MAPI_CMD_HDMIRX_GET_HDR_INFO_FRAME,
    MAPI_CMD_HDMIRX_DATA_RTERM_CONTROL,
    MAPI_CMD_HDMIRX_GET_HDCP_STATUS,
    MAPI_CMD_HDMIRX_GET_SCDC_VALUE,
    MAPI_CMD_HDMIRX_GET_TMDS_RATES_KHZ,
    MAPI_CMD_HDMIRX_GET_SCDC_CABLE_DETECT,
    MAPI_CMD_HDMIRX_GET_SOURCE_SELECT,
    MAPI_CMD_HDMIRX_GET_PACKET_STATUS,
    MAPI_CMD_HDMIRX_GET_PACKET_CONTENT,
    MAPI_CMD_HDMIRX_GET_MULTIVS_PACKET_CONTENT,
    MAPI_CMD_HDMIRX_CREATE_PATH,
    MAPI_CMD_HDMIRX_GET_GC_PACKET_INFO,
    MAPI_CMD_HDMIRX_ERROR_STATUS_UPDATE,
    MAPI_CMD_HDMIRX_AUDIO_CONTENT_PROTECT_INFO,
    MAPI_CMD_HDMIRX_GET_MID_INFO_VALUE,
    MAPI_CMD_HDMIRX_GET_PACKET_VALUE,
    MAPI_CMD_HDMIRX_AUDIO_CHANNEL_STATUS,
    MAPI_CMD_HDMIRX_GET_COLOR_FORMAT,
    MAPI_CMD_HDMIRX_GET_COLOR_RANGE,
    MAPI_CMD_HDMIRX_GET_ASPECT_RATIO,
    MAPI_CMD_HDMIRX_GET_ACTIVE_FORMAT_ASPECT_RATIO,
    MAPI_CMD_HDMIRX_GET_CONTENT_TYPE,
    MAPI_CMD_HDMIRX_GET_PIXEL_REPETITION,
    MAPI_CMD_HDMIRX_GET_AVI_INFOFRAME_VERSION,
    MAPI_CMD_HDMIRX_GET_ACTIVE_INFORMATION_PRESENT,
    MAPI_CMD_HDMIRX_GET_COLORMETRY,
    MAPI_CMD_HDMIRX_CHECK_VS_PACKET_4K2K,
    MAPI_CMD_HDMIRX_3D_4K2K_PROCESS,
    MAPI_CMD_HDMIRX_GET_ADDITIONAL_FORMAT,
    MAPI_CMD_HDMIRX_GET_3D_STRUCTURE,
    MAPI_CMD_HDMIRX_GET_3D_EXTEND_DATA,
    MAPI_CMD_HDMIRX_GET_3D_META_FIELD,
    MAPI_CMD_HDMIRX_GET_VS_VIC_CODE,
    MAPI_CMD_HDMIRX_GET_VS_4K2K_VIC_CODE,
    MAPI_CMD_HDMIRX_DVI_MUX_SWITCH,
    MAPI_CMD_HDMIRX_AVERAGE_SCALING_DOWN,
    MAPI_CMD_HDMIRX_GET_TIMING_INFO,
    MAPI_CMD_HDMIRX_PACKET_RESET,
}E_HDMIRX_API_CMD_TYPE;
#endif

typedef struct
{
    MS_BOOL bCreateHDCPTaskFlag;
    stHDMI_INITIAL_TABLE stInitialTable;
} stHDMI_INITIAL_SETTING, *pstHDMI_INITIAL_SETTING;

typedef struct
{
    MS_U16 u16ReturnValue;
} stHDMI_FUNC_CAPS, *pstHDMI_FUNC_CAPS;

typedef struct
{
    HDMI_REST_t breset;
    MS_U8 u8HDMIInfoSource;
} stHDMI_PKT_RESET, *pstHDMI_PKT_RESET;

typedef struct
{
    MS_BOOL bHighLow;
    E_MUX_INPUTPORT enInputPortType;
    MS_BOOL bInverse;
} stHDMI_PULLHPD, *pstHDMI_PULLHPD;

typedef struct
{
    HDMI_GControl_INFO_t gcontrol;
    MS_U16 u16ReturnValue;
    MS_U8 ucHDMIInfoSource;
} stHDMI_GC_INFO, *pstHDMI_GC_INFO;

typedef struct
{
#if defined(UFO_PUBLIC_HEADER_212) ||defined(UFO_PUBLIC_HEADER_500_3)
    MS_BOOL bEnable;
    MS_HDMI_PACKET_VALUE_t stReturnValue;
#else
    MS_U32 ulPacketStatus;
    MS_BOOL bEnable;
    MS_U8 ucHDMIInfoSource;
#endif
} stHDMI_PACKET_RECEIVED, *pstHDMI_PACKET_RECEIVED;

typedef struct
{
    MS_HDMI_COLOR_FORMAT eReturnValue;
    MS_U8 ucHDMIInfoSource;
} stHDMI_GET_COLORFORMAT, *pstHDMI_GET_COLORFORMAT;

typedef struct
{
    EN_HDMI_COLOR_RANGE eReturnValue;
    MS_U8 ucHDMIInfoSource;
} stHDMI_GET_COLORRANGE, *pstHDMI_GET_COLORRANGE;

typedef struct
{
    MS_HDMI_EQ enEq;
    MS_U8 u8EQValue;
} stHDMI_SET_EQ, *pstHDMI_SET_EQ;

typedef struct
{
    MS_HDMI_EQ enEq;
    MS_U8 u8EQValue;
    E_MUX_INPUTPORT enInputPortType;
    MS_BOOL bReturnValue;
} stHDMI_SET_EQ_TO_PORT, *pstHDMI_SET_EQ_TO_PORT;

typedef struct
{
    MS_U16 u16MuteEvent;
    MS_U16 u16MuteMask;
} stHDMI_AUDIO_MUTE_ENABLE, *pstHDMI_AUDIO_MUTE_ENABLE;

typedef struct
{
    MS_BOOL bEn_PAR;
    MS_BOOL bEn_AFAR;
    MS_HDMI_AR_TYPE eReturnValue;
    MS_U8 ucHDMIInfoSource;
} stHDMI_GET_ASPECTRATIO, *pstHDMI_GET_ASPECTRATIO;

typedef struct
{
    MS_BOOL bEn_AFAR;
    MS_HDMI_AR_TYPE eReturnValue;
    MS_U8 ucHDMIInfoSource;
} stHDMI_GET_ACTIVEFORMAT_ASPECTRATIO, *pstHDMI_GET_ACTIVEFORMAT_ASPECTRATIO;

typedef struct
{
    EN_AVI_INFOFRAME_VERSION eReturnValue;
    MS_U8 ucHDMIInfoSource;
} stHDMI_GET_AVIINFOFRAMEVER, *pstHDMI_GET_AVIINFOFRAMEVER;

typedef struct
{
    MS_U8 u8value;
    MS_BOOL bread;
    MS_U8 u8ReturnValue;
    MS_U8 ucHDMIInfoSource;
    MS_BOOL bNewPathFlag;
} stHDMI_ERR_STATUS_UPDATE, *pstHDMI_ERR_STATUS_UPDATE;

typedef struct
{
    MS_U16 u8ReturnValue;
} stHDMI_GET_TMDS_FREQ, *pstHDMI_GET_TMDS_FREQ;

typedef struct
{
    HDMI_POLLING_STATUS_t **pStatus;
} stHDMI_GET_POLLINGSTATUS, *pstHDMI_GET_POLLINGSTATUS;

typedef struct
{
    MS_BOOL bReturnValue;
} stHDMI_ISHDMI_MODE, *pstHDMI_ISHDMI_MODE;

typedef struct DLL_PACKED
{
    MS_U8 * pu8ManufactorData;
    MS_U8 ucHDMIInfoSource;
} stHDMI_GET_MID_INFO, *pstHDMI_GET_MID_INFO;

typedef struct DLL_PACKED
{
    MS_HDMI_PACKET_STATE_t u8state;
    MS_U8 u8byte_idx;
    MS_U8 *pValue;
    MS_BOOL bReturnValue;
    MS_U8 ucHDMIInfoSource;
} stHDMI_GET_PACKET_VALUE, *pstHDMI_GET_PACKET_VALUE;

typedef struct
{
    MS_DVI_CHANNEL_TYPE u8Channel;
    MS_U16 u16ReturnValue;
} stDVI_CHANNELPHASESTATUS, *pstDVI_CHANNELPHASESTATUS;

typedef struct
{
    MS_U16 u16Reset;
} stDVI_SOFTWARERESET, *pstDVI_SOFTWARERESET;

typedef struct
{
    MS_BOOL bReturnValue;
    MS_U8 ucHDMIInfoSource;
} stHDMI_GET_AVIIINFO_ACTIVEINFOPRESENT, *pstHDMI_GET_AVIIINFO_ACTIVEINFOPRESENT;

typedef struct
{
    MS_BOOL bPullLow;
    E_MUX_INPUTPORT enInputPortType;
} stDVI_CLKPULLLOW, *pstDVI_CLKPULLLOW;

typedef struct
{
    E_MUX_INPUTPORT enInputPortType;
    MS_U8 ucHDMIInfoSource;
} stDVI_SWITCHSRC, *pstDVI_SWITCHSRC;

typedef struct
{
    E_MUX_INPUTPORT enInputPortType;
    MS_BOOL bReturnValue;
} stHDMI_SETFORCLOCKLESSTHAN70MHZ, *pstHDMI_SETFORCLOCKLESSTHAN70MHZ;

typedef struct
{
    MS_BOOL bClockLessThan70MHz;
} stHDMI_DVI_ADJUST, *pstHDMI_DVI_ADJUST;

typedef struct
{
    INPUT_SOURCE_TYPE_t enInputSourceType;
} stHDMI_SETUPDATEPHASELINECOUNT, *pstHDMI_SETUPDATEPHASELINECOUNT;

typedef struct
{
    MS_BOOL bEnable;
} stHDCP_ENABLE, *pstHDCP_ENABLE;

typedef struct
{
    E_MUX_INPUTPORT enInputPortType;
    MS_BOOL bEnable;
    MS_BOOL bReturnValue;
} stHDMI_SET_HDCPENABLE, *pstHDMI_SET_HDCPENABLE;

typedef struct DLL_PACKED
{
    MS_U8 * pu8HdcpKeyData;
} stHDCP_INITPRODUCTIONKEY, *pstHDCP_INITPRODUCTIONKEY;

typedef struct DLL_PACKED
{
#if defined(UFO_PUBLIC_HEADER_212)
    MS_HDCP_STATUS_INFO_t* stReturnValue;
#else
    MS_U16 usHDCPStatus;
#endif
} stHDCP_GETSTATUS, *pstHDCP_GETSTATUS;

typedef struct
{
    MS_BOOL bStable;
} stHDCP_VSYNC_END_EN, *pstHDCP_VSYNC_END_EN;

typedef struct
{
    MS_BOOL bEnable;
} stHDMI_AUDIO_OUTPUT, *pstHDMI_AUDIO_OUTPUT;

typedef struct
{
    MS_U8 u8ReturnValue;
    MS_U8 ucHDMIInfoSource;
    MS_BOOL bNewPathFlag;
} stHDMI_AUDIO_CP_HDR_INFO, *pstHDMI_AUDIO_CP_HDR_INFO;

typedef struct
{
    MS_U8 u8byte;
    MS_U8 u8ReturnValue;
    MS_U8 ucHDMIInfoSource;
} stHDMI_AUDIO_CHANNEL_STATUS, *pstHDMI_AUDIO_CHANNEL_STATUS;

typedef struct DLL_PACKED
{
    const MSIF_Version **ppVersion;
    HDMI_Ret_Value eReturnValue;
} stHDMI_GETLIBVER, *pstHDMI_GETLIBVER;

typedef struct DLL_PACKED
{
    MS_HDMI_INFO *pInfo;
    HDMI_Ret_Value eReturnValue;
} stHDMI_GETINFO, *pstHDMI_GETINFO;

typedef struct DLL_PACKED
{
    MS_HDMI_Status *pStatus;
    HDMI_Ret_Value eReturnValue;
} stHDMI_GETSTATUS, *pstHDMI_GETSTATUS;

typedef struct DLL_PACKED
{
    XC_DDCRAM_PROG_INFO *pstDDCRam_Info;
    MS_U32 u32SizeOfInfo;
} stHDMI_READ_DDCRAM, *pstHDMI_READ_DDCRAM;

typedef struct DLL_PACKED
{
    XC_DDCRAM_PROG_INFO *pstDDCRam_Info;
    MS_U32 u32SizeOfInfo;
} stHDMI_PROG_DDCRAM, *pstHDMI_PROG_DDCRAM;

typedef struct
{
    MS_HDMI_CONTENT_TYPE enReturnValue;
    MS_U8 ucHDMIInfoSource;
} stHDMI_GET_CONTENT_TYPE, *pstHDMI_GET_CONTENT_TYPE;

typedef struct
{
    MS_HDMI_EXT_COLORIMETRY_FORMAT enReturnValue;
    MS_U8 ucHDMIInfoSource;
} stHDMI_GET_EXT_COLORIMETRY, *pstHDMI_GET_EXT_COLORIMETRY;

typedef struct
{
    MS_U8 u8ReturnValue;
    MS_U8 ucHDMIInfoSource;
} stHDMI_GET_PIXEL_REPETITION, *pstHDMI_GET_PIXEL_REPETITION;

typedef struct
{
    MS_BOOL bReturnValue;
    MS_U8 ucHDMIInfoSource;
} stHDMI_CHECK_4K2K, *pstHDMI_CHECK_4K2K;

typedef struct
{
    MS_U8 ucHDMIInfoSource;
} stHDMI_3D_4K2K_PROCESS, *pstHDMI_3D_4K2K_PROCESS;

typedef struct
{
    MS_BOOL bTrue;
    MS_U8 ucHDMIInfoSource;
} stHDMI_AVG_SCALERINGDOWN, *pstHDMI_AVG_SCALERINGDOWN;

typedef struct
{
    E_HDMI_ADDITIONAL_VIDEO_FORMAT eReturnValue;
    MS_U8 ucHDMIInfoSource;
} stHDMI_CHECK_ADDITIONAL_FORMAT, *pstHDMI_CHECK_ADDITIONAL_FORMAT;

typedef struct
{
    E_XC_3D_INPUT_MODE eReturnValue;
    MS_U8 ucHDMIInfoSource;
} stHDMI_GET_3D_STRUCTURE, *pstHDMI_GET_3D_STRUCTURE;

typedef struct
{
    E_HDMI_3D_EXT_DATA_T eReturnValue;
    MS_U8 ucHDMIInfoSource;
} stHDMI_GET_3D_EXT_DATA, *pstHDMI_GET_3D_EXT_DATA;

typedef struct DLL_PACKED
{
    sHDMI_3D_META_FIELD *pdata;
    MS_U8 ucHDMIInfoSource;
} stHDMI_GET_3D_META_FIELD, *pstHDMI_GET_3D_META_FIELD;

typedef struct
{
    MS_U8 u8ReturnValue;
    MS_U8 ucHDMIInfoSource;
} stHDMI_GET_VIC_CODE, *pstHDMI_GET_VIC_CODE;

typedef struct
{
    E_HDMI_VIC_4Kx2K_CODE eReturnValue;
    MS_U8 ucHDMIInfoSource;
} stHDMI_GET_4KX2K_VIC_CODE, *pstHDMI_GET_4KX2K_VIC_CODE;

typedef struct
{
    E_MUX_INPUTPORT enInputPortType;
    MS_BOOL bEnable;
    MS_BOOL bDrivingHigh;
} stHDMI_ARC_PINCONTROL, *pstHDMI_ARC_PINCONTROL;

typedef struct
{
    E_MUX_INPUTPORT enInputPortType;
    MS_U16 u16Reset;
} stDVI_SOFTWARE_RESET, *pstDVI_SOFTWARE_RESET;

typedef struct
{
    EN_POWER_MODE enPowerState;
    MS_U32 u32ReturnValue;
} stHDMI_SET_POWERSTATE, *pstHDMI_SET_POWERSTATE;

typedef struct
{
    E_MUX_INPUTPORT enInputPortType;
    MS_BOOL bReturnValue;
} stHDMI_CheckHDMI20_Setting, *pstHDMI_CheckHDMI20_Setting;

typedef struct
{
    E_MUX_INPUTPORT enInputPortType;
    MS_BOOL bReturnValue;
} stHDMI_CHECK_HDMI20_SETTING, *pstHDMI_CHECK_HDMI20_SETTING;

typedef struct
{
    E_MUX_INPUTPORT enInputPortType;
    MS_U8 ucSourceVersion;
} stHDMI_Get_Source_Version, *pstHDMI_Get_Source_Version;

typedef struct
{
    E_MUX_INPUTPORT enInputPortType;
    MS_U8 ucSourceVersion;
} stHDMI_GET_SOURCE_VERSION, *pstHDMI_GET_SOURCE_VERSION;

typedef struct
{
    MS_U32 ul5VDetectGPIOSelect;
} stHDMI_Set5VDetectGPIOSelect, *pstHDMI_Set5VDetectGPIOSelect;

typedef struct
{
    MS_U32 ul5VDetectGPIOSelect;
} stHDMI_SET_5V_DETECT_GPIO_SELECT, *pstHDMI_SET_5V_DETECT_GPIO_SELECT;

typedef struct
{
    E_MUX_INPUTPORT enInputPortType;
    MS_BOOL bStatus;
} stHDMI_Get_De_Stable_Status, *pstHDMI_Get_De_Stable_Status;

typedef struct
{
    E_MUX_INPUTPORT enInputPortType;
    MS_BOOL bStatus;
} stHDMI_GET_DE_STABLE_STATUS, *pstHDMI_GET_DE_STABLE_STATUS;

typedef struct
{
    MS_BOOL ucHDCP14_flag;
} stHDMI_Check_HDCP14, *pstHDMI_Check_HDCP14;

typedef struct
{
    MS_BOOL bHDCP14KeyVaildFlag;
} stHDMI_GET_HDCP14_KEY_VAILD, *pstHDMI_GET_HDCP14_KEY_VAILD;

typedef struct
{
    E_MUX_INPUTPORT enInputPortType;
    MS_U8 ucHDCPState;
} stHDMI_CHECK_HDCP_STATE, *pstHDMI_CHECK_HDCP_STATE;

typedef struct
{
    E_MUX_INPUTPORT enInputPortType;
    MS_U8 ucHDCPENCState;
} stHDMI_CHECK_HDCP_ENC_STATE, *pstHDMI_CHECK_HDCP_ENC_STATE;

typedef struct
{
    MS_BOOL bInverse;
    E_MUX_INPUTPORT enInputPortType;
} stHDMI_SET_HPD_INVERT_FLAG, *pstHDMI_SET_HPD_INVERT_FLAG;

typedef struct
{
    MS_U16 usReturnValue;
    E_HDMI_GET_DATA_INFO enInfo;
    MS_U8 ucHDMIInfoSource;
    MS_BOOL bNewPathFlag;
} stHDMI_GET_DATA_INFO, *pstHDMI_GET_DATA_INFO;

/*************************** HDCP22 ***************************/
typedef struct
{
	MS_U8 ucPortIdx;
	MS_U8* pucRiv;
	MS_U8* pucSessionKey;
} stHDCP22_FillCipherKey, *pstHDCP22_FillCipherKey;

typedef struct DLL_PACKED
{
    MS_U8 ucPortIdx;
    MS_U8* pucRiv;
    MS_U8* pucSessionKey;
} stHDCP22_FILL_CIPHER_KEY, *pstHDCP22_FILL_CIPHER_KEY;

typedef struct DLL_PACKED
{
    MS_BOOL bHDRReceiveFlag;
    MS_U8 ucHDMIInfoSource;
    MS_U16* pusHDRInfoFrame;
} stHDMI_GET_HDR_INFOFRAME, *pstHDMI_GET_HDR_INFOFRAME;

typedef struct
{
    E_MUX_INPUTPORT enInputPortType;
    MS_BOOL bDataRtermEnable;
    MS_BOOL bCheckInitFlag;
} stHDMI_DATA_RTERM_CONTROL, *pstHDMI_DATA_RTERM_CONTROL;

typedef struct
{
    E_MUX_INPUTPORT enInputPortType;
    MS_U16 usHDCPStatus;
} stHDMI_GET_HDCP_STATUS, *pstHDMI_GET_HDCP_STATUS;

typedef struct
{
    E_MUX_INPUTPORT enInputPortType;
    MS_U8 ucOffset;
    MS_U8 ucRetData;
} stHDMI_GET_SCDC_VALUE, *pstHDMI_GET_SCDC_VALUE;

typedef struct
{
    E_MUX_INPUTPORT enInputPortType;
    E_HDMI_GET_TMDS_RATES enType;
    MS_U32 ulRetRates;
} stHDMI_GET_TMDS_RATES_KHZ, *pstHDMI_GET_TMDS_RATES_KHZ;

typedef struct
{
    E_MUX_INPUTPORT enInputPortType;
    MS_BOOL bCableDetectFlag;
} stHDMI_GET_SCDC_CABLE_DETECT, *pstHDMI_GET_SCDC_CABLE_DETECT;

typedef struct
{
    E_MUX_INPUTPORT enInputPortType;
    MS_U8 u8HDMIInfoSource;
} stHDMI_GET_SOURCE_SELECT, *pstHDMI_GET_SOURCE_SELECT;

typedef struct
{
    E_MUX_INPUTPORT enInputPortType;
    MS_U32 u32PacketStatus;
} stHDMI_GET_PACKET_STATUS, *pstHDMI_GET_PACKET_STATUS;

typedef struct
{
    E_MUX_INPUTPORT enInputPortType;
    MS_BOOL bGetContentFlag;
    MS_U8 u8PacketLength;
    MS_U8 u8PacketContent[HDMI_GET_PACKET_LENGTH];
    MS_HDMI_PACKET_STATE_t enPacketType;
} stHDMI_GET_PACKET_CONTENT, *pstHDMI_GET_PACKET_CONTENT;

typedef struct
{
    E_MUX_INPUTPORT enInputPortType;
    MS_BOOL bGetContentFlag;
    MS_U8 u8PacketLength;
    MS_U8 u8PacketContent[HDMI_GET_MULTIVS_PACKET_LENGTH];
    MS_HDMI_PACKET_STATE_t enPacketType;
} stHDMI_GET_MULTIVS_PACKET_CONTENT, *pstHDMI_GET_MULTIVS_PACKET_CONTENT;

typedef struct
{
    E_MUX_INPUTPORT enInputPortType;
    MS_U8 u8HDMIInfoSource;
} stHDMI_CREATE_PATH, *pstHDMI_CREATE_PATH;

typedef struct DLL_PACKED
{
    HDCP22_Recv_CBF pCBFunc;
    void* pContext;
} stHDCP22_InitCBFunc, *pstHDCP22_InitCBFunc;

typedef struct
{
	MS_U8	ucPortIdx;
} stHDCP22_PortInit, *pstHDCP22_PortInit;

typedef struct
{
    MS_U8    ucPortIdx;
} stHDCP22_PORT_INIT, *pstHDCP22_PORT_INIT;

typedef struct
{
    MS_U8    ucPortIdx;
    MS_BOOL bReturnValue;
} stHDCP22_PollingReadDone, *pstHDCP22_PollingReadDone;

typedef struct
{
    MS_U8    ucPortIdx;
    MS_BOOL bReturnValue;
} stHDCP22_POLLING_READ_DONE, *pstHDCP22_POLLING_READ_DONE;

typedef struct
{
    MS_U8    ucPortType;
    MS_U8    ucPortIdx;
    MS_BOOL bIsEnable;
} stHDCP22_EnableCipher, *pstHDCP22_EnableCipher;

typedef struct
{
    MS_U8    ucPortType;
    MS_U8    ucPortIdx;
    MS_BOOL bIsEnable;
} stHDCP22_ENABLE_CIPHER, *pstHDCP22_ENABLE_CIPHER;

typedef struct
{
	MS_U8	ucPortType;
	MS_U8	ucPortIdx;
	MS_U8*	pucData;
	MS_U32	dwDataLen;
	void*	pDummy;
} stHDCP22_SendMsg, *pstHDCP22_SendMsg;

typedef struct DLL_PACKED
{
    MS_U8    ucPortType;
    MS_U8    ucPortIdx;
    MS_U8*    pucData;
    MS_U32    dwDataLen;
    void*    pDummy;
} stHDCP22_SEND_MESSAGE, *pstHDCP22_SEND_MESSAGE;

typedef struct
{
	MS_U8	ucPortIdx;
} stHDCP22_Handler, *pstHDCP22_Handler;

typedef struct
{
    MS_BOOL bHdcp22RecvMsgFlag;
    MS_BOOL bIRQModeFlag;
    MS_U8 ucPortIdx;
    MS_U8 ucHdcp22PortType;
    MS_U8 pucHdcp22MsgData[129];
    MS_U32 ucHdcp22MsgLen;
} stHDCP22_HANDLER, *pstHDCP22_HANDLER;

typedef struct
{
    MS_U8 ucHDCPWriteDoneIndex;
} stHDCP22_WAIT_EVENT, *pstHDCP22_WAIT_EVENT;

typedef struct
{
    MS_U16 usHDCPEventIndex;
} stHDCP22_SET_EVENT, *pstHDCP22_SET_EVENT;

typedef struct
{
    MS_U32  u32Cmd;
    void    *pBuf;
    MS_U32  u32BufSize;
    MS_BOOL bRet;
} stHDMI_Ctrl, *pstHDMI_Ctrl;

/*************************** HDCP22 ***************************/
typedef struct DLL_PACKED
{
    MS_U32 u32Cmd;
    void *pBuf;
    MS_U32 u32BufSize;
    MS_BOOL bRet;
    MS_U8 ucHDMIInfoSource;
} stHDMI_CONTROL, *pstHDMI_CONTROL;

/************************ HDCP Repeater ************************/
typedef struct
{
    E_MUX_INPUTPORT enInputPortType;
    MS_U8 ucOffset;
    MS_U8 ucData;
} stHDCP_WRITE_X74, *pstHDCP_WRITE_X74;

typedef struct
{
    E_MUX_INPUTPORT enInputPortType;
    MS_U8 ucOffset;
    MS_U8 ucRetData;
} stHDCP_READ_X74, *pstHDCP_READ_X74;

typedef struct
{
    E_MUX_INPUTPORT enInputPortType;
    MS_BOOL bIsRepeater;
} stHDCP_SET_REPEATER, *pstHDCP_SET_REPEATER;

typedef struct
{
    E_MUX_INPUTPORT enInputPortType;
    MS_U16 usBstatus;
} stHDCP_SET_BSTATUS, *pstHDCP_SET_BSTATUS;

typedef struct
{
    E_MUX_INPUTPORT enInputPortType;
    MS_BOOL bHDMIMode;
} stHDCP_SET_HDMI_MODE, *pstHDCP_SET_HDMI_MODE;

typedef struct
{
    E_MUX_INPUTPORT enInputPortType;
    MS_U8 ucRetIntStatus;
} stHDCP_GET_INTERRUPT_STATUS, *pstHDCP_GET_INTERRUPT_STATUS;

typedef struct DLL_PACKED
{
    E_MUX_INPUTPORT enInputPortType;
    MS_U8* pucKSV;
    MS_U32 ulLen;
} stHDCP_WRITE_KSV_LIST, *pstHDCP_WRITE_KSV_LIST;

typedef struct DLL_PACKED
{
    E_MUX_INPUTPORT enInputPortType;
    MS_U8* pucVPrime;
} stHDCP_SET_VPRIME, *pstHDCP_SET_VPRIME;
/************************ HDCP Repeater ************************/

#ifdef __cplusplus
}
#endif

#endif //_XC_HDMI_IF_H_
