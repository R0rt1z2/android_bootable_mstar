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


#ifndef _API_HDMI_RX_PRIVATE_H_
#define _API_HDMI_RX_PRIVATE_H_

#include "MsTypes.h"
#include "MsDevice.h"
#include "MsCommon.h"

#ifdef __cplusplus
extern "C"
{
#endif

#include "mhal_hdmi.h"
#include "drvXC_HDMI_if.h"

#ifndef UNUSED
#define UNUSED(x) (void)(x)
#endif

#define GET_HDMI_FLAG(a, b)                 (MS_BOOL)((a &b) ?TRUE :FALSE)
#define SET_HDMI_FLAG(a, b)                 (a |= b)
#define CLR_HDMI_FLAG(a, b)                 (a &= ~b)

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define HDMI_HDCP_KEY_LENGTH                289U
#define HDMI_HDCP_BKSV_LENGTH               5U
#define HDMI_HDCP22_RIV_LENGTH              8U
#define HDMI_HDCP22_SESSION_KEY_LENGTH      16U
#define HDMI_HDCP22_MESSAGE_LENGTH          129U
#define HDMI_HDCP22_SEND_MESSAGE_LENGTH     (535U)
#ifdef MSOS_TYPE_ECOS
#define HDMI_POLLING_STACK_SIZE             2048U
#define HDMI_HDCP_INTERRUPT_STACK_SIZE      2048U
#else
#define HDMI_POLLING_STACK_SIZE             1U
#define HDMI_HDCP_INTERRUPT_STACK_SIZE      1U
#endif
#define HDMI_PACKET_RECEIVE_COUNT           (5U)    // 5 x 10ms = 50ms
#define HDMI_POLLING_INTERVAL               (10U)
#define HDMI_FRAME_RATE_OFFSET              (2U)
#define HDMI_CONTROL_FUNCTION_BUFFER_SIZE   (30U)
#define HDMI_HDCP14_RI_COUNT                550U  // 550 x 10ms = 5.5s
#define HDMI_HDCP_KSV_LIST_LENGTH           635U  // max: 127x5
#define HDMI_HDCP_VPRIME_LENGTH             20U
#define HDMI_HDCP_EVENT_POLLING_STATUS      0x100U
#define HDMI_HDCP_EVENT_RECEIVE_AKE_INIT    0x200U
#define HDMI_HDCP_EVENT_WAIT_MESSAGE        0x300U
#define HDMI_HDCP_EVENT_WRITE_DONE          0x400U
#define HDMI_HDCP_EVENT_STR_SUSPEND         0x500U
#define HDMI_HDCP_EVENT_STR_RESUME          0x600U
#define HDMI_HDCP_EVENT_INDEX_MASK          0xFFFFFF00U
#define HDMI_HDCP_EVENT_STATUS_MASK         0x000000FFU
#define HDMI_HDCP_ALL_EVENT                 0xFFFFFFFFU

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------
typedef struct _HDMI_RX_RESOURCE_PRIVATE
{
    MS_BOOL bSelfCreateTaskFlag;
    MS_BOOL bHDMITaskProcFlag;
    MS_BOOL bImmeswitchSupport;
    MS_BOOL bHDCP14KeyVaildFlag;
    MS_BOOL bHPDInvertFlag[4];
    MS_BOOL bHPDEnableFlag[4];
    MS_BOOL bDataEnableFlag[4];
    MS_BOOL bHDCPIRQAttachFlag;
    MS_BOOL bHDCP22IRQMaskFlag[4];
    MS_BOOL bHDCP22EnableFlag[4];
    MS_BOOL bHDCP14RiFlag[4];
    MS_BOOL bHDCP14RxREEFlag;
    MS_U8 ucInitialIndex;
    MS_U8 ucMHLSupportPath;
    MS_U8 ucHDCPKeyTable[HDMI_HDCP_KEY_LENGTH];
    MS_U8 ucCheckPacketState[HDMI_INFO_SOURCE_MAX];
    MS_U8 ucReceiveInterval[HDMI_INFO_SOURCE_MAX];
    MS_U8 ucReceiveIntervalCount[HDMI_INFO_SOURCE_MAX];
    MS_U8 ucHDMIPollingStack[HDMI_POLLING_STACK_SIZE];
    MS_U8 ucHDCP22RecvIDListSend[4];
    MS_U8 ucHDCPWriteDoneIndex;
    MS_U8 ucHDCPReadDoneIndex;
    MS_U8 u8PacketReceiveCount[HDMI_INFO_SOURCE_MAX];
    MS_U16 usHDCP14RiCount[4];
    MS_U32 ulPacketStatus[HDMI_INFO_SOURCE_MAX];
    MS_U32 u32PrePacketStatus[HDMI_INFO_SOURCE_MAX];
    MS_S32 slHDMIPollingTaskID;
    MS_S32 slHDMIHDCPEventID;
    EN_POWER_MODE usPrePowerState;
    E_MUX_INPUTPORT enMuxInputPort[HDMI_INFO_SOURCE_MAX];
    stHDMI_POLLING_INFO stHDMIPollingInfo[4];
}HDMI_RX_RESOURCE_PRIVATE;

typedef MS_BOOL                         (*IOCTL_HDMIRX_INITIAL)                             (void*, stHDMI_INITIAL_TABLE);
typedef void                            (*IOCTL_HDMIRX_STABLE_POLLING)                      (void*);
typedef MS_U8                           (*IOCTL_HDMIRX_GET_SOURCE_VERSION)                  (void*, E_MUX_INPUTPORT);
typedef MS_U8                           (*IOCTL_HDMIRX_CHECK_HDCP_STATE)                    (void*, E_MUX_INPUTPORT);
typedef MS_U8                           (*IOCTL_HDMIRX_CHECK_HDCP_ENC_STATE)                (void*, E_MUX_INPUTPORT);
typedef void                            (*IOCTL_HDMIRX_CHECK_HDCP14_KEY_VAILD)              (void*, MS_U8*);
typedef MS_BOOL                         (*IOCTL_HDMIRX_GET_HDCP14_KEY_VAILD_FLAG)           (void*);
typedef MS_BOOL                         (*IOCTL_HDMIRX_SET_HPD_INVERT_FLAG)                 (void*, E_MUX_INPUTPORT, MS_BOOL);
typedef MS_U32                          (*IOCTL_HDMIRX_GET_PACKET_RECEIVE)                  (void*, MS_U8);
typedef MS_BOOL                         (*IOCTL_HDMIRX_GET_HDR_INFO_FRAME)                  (void*, MS_U8, MS_U16*);
typedef void                            (*IOCTL_HDMIRX_DATA_RTERM_CONTROL)                  (void*, E_MUX_INPUTPORT, MS_BOOL, MS_BOOL);
typedef MS_U16                          (*IOCTL_HDMIRX_GET_HDCP_STATUS)                     (void*, E_MUX_INPUTPORT);
typedef MS_U8                           (*IOCTL_HDMIRX_GET_SCDC_VALUE)                      (void*, E_MUX_INPUTPORT, MS_U8);
typedef MS_U32                          (*IOCTL_HDMIRX_GET_TMDS_RATES_KHZ)                  (void*, E_MUX_INPUTPORT, E_HDMI_GET_TMDS_RATES);
typedef MS_BOOL                         (*IOCTL_HDMIRX_GET_SCDC_CABLE_DETECT)               (void*, E_MUX_INPUTPORT);
typedef MS_U8                           (*IOCTL_HDMIRX_GET_SOURCE_SELECT)                   (void*, E_MUX_INPUTPORT);
typedef MS_U32                          (*IOCTL_HDMIRX_GET_PACKET_STATUS)                   (void*, E_MUX_INPUTPORT);
typedef MS_BOOL                         (*IOCTL_HDMIRX_GET_PACKET_CONTENT)                  (void*, E_MUX_INPUTPORT, MS_HDMI_PACKET_STATE_t, MS_U8, MS_U8*);
typedef void                            (*IOCTL_HDMIRX_CREATE_PATH)                         (void*, E_MUX_INPUTPORT, MS_U8);
typedef MS_U16                          (*IOCTL_HDMIRX_GET_GC_PACKET_INFO)                  (void*, HDMI_GControl_INFO_t, MS_U8);
typedef MS_U8                           (*IOCTL_HDMIRX_ERROR_STATUS_UPDATE)                 (void*, MS_U8, MS_U8, MS_BOOL, MS_BOOL);
typedef MS_U8                           (*IOCTL_HDMIRX_AUDIO_CONTENT_PROTECT_INFO)          (void*, MS_U8, MS_BOOL);
typedef void                            (*IOCTL_HDMIRX_GET_MID_INFO_VALUE)                  (void*, MS_U8, MS_U8*);
typedef MS_BOOL                         (*IOCTL_HDMIRX_GET_PACKET_VALUE)                    (void*, MS_U8, MS_HDMI_PACKET_STATE_t, MS_U8, MS_U8*);
typedef MS_U8                           (*IOCTL_HDMIRX_AUDIO_CHANNEL_STATUS)                (void*, MS_U8, MS_U8);
typedef MS_HDMI_COLOR_FORMAT            (*IOCTL_HDMIRX_GET_COLOR_FORMAT)                    (void*, MS_U8);
typedef EN_HDMI_COLOR_RANGE             (*IOCTL_HDMIRX_GET_COLOR_RANGE)                     (void*, MS_U8);
typedef MS_HDMI_AR_TYPE                 (*IOCTL_HDMIRX_GET_ASPECT_RATIO)                    (void*, MS_U8, MS_BOOL, MS_BOOL);
typedef MS_HDMI_AR_TYPE                 (*IOCTL_HDMIRX_GET_ACTIVE_FORMAT_ASPECT_RATIO)      (void*, MS_U8, MS_BOOL);
typedef MS_HDMI_CONTENT_TYPE            (*IOCTL_HDMIRX_GET_CONTENT_TYPE)                    (void*, MS_U8);
typedef MS_U8                           (*IOCTL_HDMIRX_GET_PIXEL_REPETITION)                (void*, MS_U8);
typedef EN_AVI_INFOFRAME_VERSION        (*IOCTL_HDMIRX_GET_AVI_INFOFRAME_VERSION)           (void*, MS_U8);
typedef MS_BOOL                         (*IOCTL_HDMIRX_GET_ACTIVE_INFORMATION_PRESENT)      (void*, MS_U8);
typedef MS_HDMI_EXT_COLORIMETRY_FORMAT  (*IOCTL_HDMIRX_GET_COLORIMETRY)                     (void*, MS_U8);
typedef MS_BOOL                         (*IOCTL_HDMIRX_CHECK_VS_PACKET_4K2K)                (void*, MS_U8);
typedef void                            (*IOCTL_HDMIRX_3D_4K2K_PROCESS)                     (void*, MS_U8);
typedef E_HDMI_ADDITIONAL_VIDEO_FORMAT  (*IOCTL_HDMIRX_GET_ADDITIONAL_FORMAT)               (void*, MS_U8);
typedef E_XC_3D_INPUT_MODE              (*IOCTL_HDMIRX_GET_3D_STRUCTURE)                    (void*, MS_U8);
typedef E_HDMI_3D_EXT_DATA_T            (*IOCTL_HDMIRX_GET_3D_EXTEND_DATA)                  (void*, MS_U8);
typedef void                            (*IOCTL_HDMIRX_GET_3D_META_FIELD)                   (void*, MS_U8, sHDMI_3D_META_FIELD*);
typedef MS_U8                           (*IOCTL_HDMIRX_GET_VS_VIC_CODE)                     (void*, MS_U8);
typedef E_HDMI_VIC_4Kx2K_CODE           (*IOCTL_HDMIRX_GET_VS_4K2K_VIC_CODE)                (void*, MS_U8);
typedef void                            (*IOCTL_HDMIRX_DVI_MUX_SWITCH)                      (void*, E_MUX_INPUTPORT, MS_U8);
typedef void                            (*IOCTL_HDMIRX_AVERAGE_SCALING_DOWN)                (void*, MS_U8, MS_BOOL);
typedef MS_U16                          (*IOCTL_HDMIRX_GET_TIMING_INFO)                     (void*, E_HDMI_GET_DATA_INFO, MS_U8, MS_BOOL);
typedef void                            (*IOCTL_HDMIRX_PACKET_RESET)                        (void*, HDMI_REST_t, MS_U8);
/*************************** HDCP Repeater ***************************/
typedef void                            (*IOCTL_HDMIRX_WRITE_X74)                           (void*, E_MUX_INPUTPORT, MS_U8, MS_U8);
typedef MS_U8                           (*IOCTL_HDMIRX_READ_X74)                            (void*, E_MUX_INPUTPORT, MS_U8);
typedef void                            (*IOCTL_HDMIRX_SET_REPEATER)                        (void*, E_MUX_INPUTPORT, MS_BOOL);
typedef void                            (*IOCTL_HDMIRX_SET_BSTATUS)                         (void*, E_MUX_INPUTPORT, MS_U16);
typedef void                            (*IOCTL_HDMIRX_SET_HDMI_MODE)                       (void*, E_MUX_INPUTPORT, MS_BOOL);
typedef MS_U8                           (*IOCTL_HDMIRX_GET_INTERRUPT_STATUS)                (void*, E_MUX_INPUTPORT);
typedef void                            (*IOCTL_HDMIRX_WRITE_KSV_LIST)                      (void*, E_MUX_INPUTPORT, MS_U8*, MS_U32);
typedef void                            (*IOCTL_HDMIRX_SET_VPRIME)                          (void*, E_MUX_INPUTPORT, MS_U8*);
/*************************** HDCP Repeater ***************************/
/*************************** HDCP22 ***************************/
typedef void                            (*IOCTL_HDMIRX_FILL_CIPHER_KEY)                     (void*, MS_U8, MS_U8*, MS_U8*);
typedef void                            (*IOCTL_HDMIRX_PORT_INIT)                           (void*, MS_U8);
typedef MS_BOOL                         (*IOCTL_HDMIRX_POLLING_READ_DONE)                   (void*, MS_U8);
typedef void                            (*IOCTL_HDMIRX_ENABLE_CIPHER)                       (void*, MS_U8, MS_U8, MS_BOOL);
typedef void                            (*IOCTL_HDMIRX_SEND_MESSAGE)                        (void*, MS_U8, MS_U8, MS_U8*, MS_U32, void*);
typedef MS_BOOL                         (*IOCTL_HDMIRX_HDCP22_HANDER)                       (void*, MS_U8, MS_U8*, MS_U8*, MS_U32*, MS_BOOL);
typedef MS_U8                           (*IOCTL_HDMIRX_HDCP22_WAIT_EVENT)                   (void*);
typedef void                            (*IOCTL_HDMIRX_HDCP22_SET_EVENT)                    (void*, MS_U16);
/*************************** HDCP22 ***************************/

typedef struct _HDMI_RX_INSTANT_PRIVATE
{
    MS_U32 ulDeviceID;
    IOCTL_HDMIRX_INITIAL                        fpHDMIRxInitial;
    IOCTL_HDMIRX_STABLE_POLLING                 fpHDMIRxStablePolling;
    IOCTL_HDMIRX_GET_SOURCE_VERSION             fpHDMIRxGetSourceVersion;
    IOCTL_HDMIRX_CHECK_HDCP_STATE               fpHDMIRxCheckHDCPState;
    IOCTL_HDMIRX_CHECK_HDCP_ENC_STATE           fpHDMIRxCheckHDCPENCState;
    IOCTL_HDMIRX_CHECK_HDCP14_KEY_VAILD         fpHDMIRxCheckHDCP14KeyVaild;
    IOCTL_HDMIRX_GET_HDCP14_KEY_VAILD_FLAG      fpHDMIRxGetHDCP14KeyVaildFlag;
    IOCTL_HDMIRX_SET_HPD_INVERT_FLAG            fpHDMIRxSetHPDInvertFlag;
    IOCTL_HDMIRX_GET_PACKET_RECEIVE             fpHDMIRxGetPacketReceive;
    IOCTL_HDMIRX_GET_HDR_INFO_FRAME             fpHDMIRxGetHDRInfoFrame;
    IOCTL_HDMIRX_DATA_RTERM_CONTROL             fpHDMIRxDataRtermControl;
    IOCTL_HDMIRX_GET_HDCP_STATUS                fpHDMIRxGetHDCPStatus;
    IOCTL_HDMIRX_GET_SCDC_VALUE                 fpHDMIRxGetSCDCValue;
    IOCTL_HDMIRX_GET_TMDS_RATES_KHZ             fpHDMIRxGetTMDSRatesKHz;
    IOCTL_HDMIRX_GET_SCDC_CABLE_DETECT          fpHDMIRxGetSCDCCableDetect;
    IOCTL_HDMIRX_GET_SOURCE_SELECT              fpHDMIRxGetSourceSelect;
    IOCTL_HDMIRX_GET_PACKET_STATUS              fpHDMIRxGetPacketStatus;
    IOCTL_HDMIRX_GET_PACKET_CONTENT             fpHDMIRxGetPacketContent;
    IOCTL_HDMIRX_CREATE_PATH                    fpHDMIRxCreatePath;
    IOCTL_HDMIRX_GET_GC_PACKET_INFO             fpHDMIRxGetGCPacketInfo;
    IOCTL_HDMIRX_ERROR_STATUS_UPDATE            fpHDMIRxErrorStatusUpdate;
    IOCTL_HDMIRX_AUDIO_CONTENT_PROTECT_INFO     fpHDMIRxAudioContentProtectInfo;
    IOCTL_HDMIRX_GET_MID_INFO_VALUE             fpHDMIRxGetMIDInfoValue;
    IOCTL_HDMIRX_GET_PACKET_VALUE               fpHDMIRxGetPacketValue;
    IOCTL_HDMIRX_AUDIO_CHANNEL_STATUS           fpHDMIRxAudioChannelStatus;
    IOCTL_HDMIRX_GET_COLOR_FORMAT               fpHDMIRxGetColorFormat;
    IOCTL_HDMIRX_GET_COLOR_RANGE                fpHDMIRxGetColorRange;
    IOCTL_HDMIRX_GET_ASPECT_RATIO               fpHDMIRxGetAspectRatio;
    IOCTL_HDMIRX_GET_ACTIVE_FORMAT_ASPECT_RATIO fpHDMIRxGetActiveFormatAspectRatio;
    IOCTL_HDMIRX_GET_CONTENT_TYPE               fpHDMIRxGetContentType;
    IOCTL_HDMIRX_GET_PIXEL_REPETITION           fpHDMIRxGetPixelRepetition;
    IOCTL_HDMIRX_GET_AVI_INFOFRAME_VERSION      fpHDMIRxGetAVIInfoFrameVersion;
    IOCTL_HDMIRX_GET_ACTIVE_INFORMATION_PRESENT fpHDMIRxGetActiveInformationPresent;
    IOCTL_HDMIRX_GET_COLORIMETRY                fpHDMIRxGetColormetry;
    IOCTL_HDMIRX_CHECK_VS_PACKET_4K2K           fpHDMIRxCheckVSPacket4K2K;
    IOCTL_HDMIRX_3D_4K2K_PROCESS                fpHDMIRx3Dor4K2KProcess;
    IOCTL_HDMIRX_GET_ADDITIONAL_FORMAT          fpHDMIRxGetAdditionalFormat;
    IOCTL_HDMIRX_GET_3D_STRUCTURE               fpHDMIRxGetVS3DStructure;
    IOCTL_HDMIRX_GET_3D_EXTEND_DATA             fpHDMIRxGetVS3DExtendData;
    IOCTL_HDMIRX_GET_3D_META_FIELD              fpHDMIRxGetVS3DMetaField;
    IOCTL_HDMIRX_GET_VS_VIC_CODE                fpHDMIRxGetVSVICCode;
    IOCTL_HDMIRX_GET_VS_4K2K_VIC_CODE           fpHDMIRxGetVS4K2KVICCode;
    IOCTL_HDMIRX_DVI_MUX_SWITCH                 fpHDMIRxDVIMuxSwitch;
    IOCTL_HDMIRX_AVERAGE_SCALING_DOWN           fpHDMIRxAverageScalingDown;
    IOCTL_HDMIRX_GET_TIMING_INFO                fpHDMIRxGetTimingInfo;
    IOCTL_HDMIRX_PACKET_RESET                   fpHDMIRxPacketReset;
    /*************************** HDCP Repeater ***************************/
    IOCTL_HDMIRX_WRITE_X74                      fpHDMIRxWriteX74;
    IOCTL_HDMIRX_READ_X74                       fpHDMIRxReadX74;
    IOCTL_HDMIRX_SET_REPEATER                   fpHDMIRxSetRepeater;
    IOCTL_HDMIRX_SET_BSTATUS                    fpHDMIRxSetBstatus;
    IOCTL_HDMIRX_SET_HDMI_MODE                  fpHDMIRxSetHDMIMode;
    IOCTL_HDMIRX_GET_INTERRUPT_STATUS           fpHDMIRxGetInterruptStatus;
    IOCTL_HDMIRX_WRITE_KSV_LIST                 fpHDMIRxWriteKSVList;
    IOCTL_HDMIRX_SET_VPRIME                     fpHDMIRxSetVPrime;
    /*************************** HDCP Repeater ***************************/
    IOCTL_HDMIRX_FILL_CIPHER_KEY                fpHDMIRxFillCipherKey;
    IOCTL_HDMIRX_PORT_INIT                      fpHDMIRxPortInit;
    IOCTL_HDMIRX_POLLING_READ_DONE              fpHDMIRxPollingReadDone;
    IOCTL_HDMIRX_ENABLE_CIPHER                  fpHDMIRxEnableCipher;
    IOCTL_HDMIRX_SEND_MESSAGE                   fpHDMIRxSendMessage;
    IOCTL_HDMIRX_HDCP22_HANDER                  fpHDMIRxHDCP22Handler;
    IOCTL_HDMIRX_HDCP22_WAIT_EVENT              fpHDMIRxHDCP22WaitEvent;
    IOCTL_HDMIRX_HDCP22_SET_EVENT               fpHDMIRxHDCP22SetEvent;
}HDMI_RX_INSTANT_PRIVATE;

typedef enum
{
    HDMI_RX_POOL = 0,
    HDMI_RX_POOL_MAX,
}eHDMIRxPoolID;

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Function Prototype
//-------------------------------------------------------------------------------------------------
MS_U32 mdrv_HDMI_STREventProc(void* pModule, EN_POWER_MODE usPowerState);
MS_BOOL mdrv_HDMI_InitialSetting(void* pInstance, stHDMI_INITIAL_TABLE stInitialTable);
void mdrv_HDMI_StablePolling(void* pInstance);
MS_U8 mdrv_HDMI_GetSourceVersion(void* pInstance, E_MUX_INPUTPORT enInputPortType);
MS_U8 mdrv_HDMI_CheckHDCPState(void* pInstance, E_MUX_INPUTPORT enInputPortType);
MS_U8 mdrv_HDMI_CheckHDCPENCState(void* pInstance, E_MUX_INPUTPORT enInputPortType);
void mdrv_HDMI_CheckHDCP14KeyVaild(void* pInstance, MS_U8 *ucHDCPKeyData);
MS_BOOL mdrv_HDMI_GetHDCP14KeyVaildFlag(void* pInstance);
MS_BOOL mdrv_HDMI_SetHPDInvertFlag(void* pInstance, E_MUX_INPUTPORT enInputPortType, MS_BOOL bHPDInvertFlag);
MS_U32 mdrv_HDMI_GetPacketReceiveStatus(void* pInstance, MS_U8 ucHDMIInfoSource);
MS_BOOL mdrv_HDMI_GetHDRInfoFrame(void* pInstance, MS_U8 ucHDMIInfoSource, MS_U16 *pusHDRInfoFrame);
void mdrv_HDMI_DataRtermControl(void* pInstance, E_MUX_INPUTPORT enInputPortType, MS_BOOL bDataRtermEnable, MS_BOOL bCheckInitFlag);
MS_U16 mdrv_HDMI_GetHDCPStatusPort(void* pInstance, E_MUX_INPUTPORT enInputPortType);
MS_U8 mdrv_HDMI_GetSCDCValue(void* pInstance, E_MUX_INPUTPORT enInputPortType, MS_U8 ucOffset);
MS_U32 mdrv_HDMI_GetTMDSRatesKHz(void* pInstance, E_MUX_INPUTPORT enInputPortType, E_HDMI_GET_TMDS_RATES enType);
MS_BOOL mdrv_HDMI_GetSCDCCableDetect(void* pInstance, E_MUX_INPUTPORT enInputPortType);
MS_U8 mdrv_HDMI_GetSourceSelect(void* pInstance, E_MUX_INPUTPORT enInputPortType);
MS_U32 mdrv_HDMI_GetPacketStatus(void* pInstance, E_MUX_INPUTPORT enInputPortType);
MS_BOOL mdrv_HDMI_GetPacketContent(void* pInstance, E_MUX_INPUTPORT enInputPortType, MS_HDMI_PACKET_STATE_t enPacketType, MS_U8 u8PacketLength, MS_U8 *pu8PacketContent);
void mdrv_HDMI_CreatePath(void* pInstance, E_MUX_INPUTPORT enInputPortType, MS_U8 u8HDMIInfoSource);
MS_U16 mdrv_HDMI_GetGCPacketInfo(void* pInstance, HDMI_GControl_INFO_t enGCControlInfo, MS_U8 u8HDMIInfoSource);
MS_U8 mdrv_HDMI_ErrorStatusUpdate(void* pInstance, MS_U8 u8HDMIInfoSource, MS_U8 u8Value, MS_BOOL bReadFlag, MS_BOOL bNewPathFlag);
MS_U8 mdrv_HDMI_AudioContentProtectInfo(void* pInstance, MS_U8 u8HDMIInfoSource, MS_BOOL bNewPathFlag);
void mdrv_HDMI_GetMIDInfoValue(void* pInstance, MS_U8 u8HDMIInfoSource, MS_U8* pu8ManufactorData);
MS_BOOL mdrv_HDMI_GetPacketValue(void* pInstance, MS_U8 u8HDMIInfoSource, MS_HDMI_PACKET_STATE_t enPacketType, MS_U8 u8PacketIndex, MS_U8 *pu8PacketValue);
MS_U8 mdrv_HDMI_AudioChannelStatus(void* pInstance, MS_U8 u8HDMIInfoSource, MS_U8 u8ByteIndex);
MS_HDMI_COLOR_FORMAT mdrv_HDMI_GetColorFormat(void* pInstance, MS_U8 u8HDMIInfoSource);
EN_HDMI_COLOR_RANGE mdrv_HDMI_GetColorRange(void* pInstance, MS_U8 u8HDMIInfoSource);
MS_HDMI_AR_TYPE mdrv_HDMI_GetAspectRatio(void* pInstance, MS_U8 u8HDMIInfoSource, MS_BOOL bEn_PAR, MS_BOOL bEn_AFAR);
MS_HDMI_AR_TYPE mdrv_HDMI_GetActiveFormatAspectRatio(void* pInstance, MS_U8 u8HDMIInfoSource, MS_BOOL bEn_AFAR);
MS_HDMI_CONTENT_TYPE mdrv_HDMI_GetContentType(void* pInstance, MS_U8 u8HDMIInfoSource);
MS_U8 mdrv_HDMI_GetPixelRepetition(void* pInstance, MS_U8 u8HDMIInfoSource);
EN_AVI_INFOFRAME_VERSION mdrv_HDMI_GetAVIInfoFrameVersion(void* pInstance, MS_U8 u8HDMIInfoSource);
MS_BOOL mdrv_HDMI_GetActiveInformationPresent(void* pInstance, MS_U8 u8HDMIInfoSource);
MS_HDMI_EXT_COLORIMETRY_FORMAT mdrv_HDMI_GetColormetry(void* pInstance, MS_U8 u8HDMIInfoSource);
MS_BOOL mdrv_HDMI_CheckVSPacket4K2K(void* pInstance, MS_U8 u8HDMIInfoSource);
void mdrv_HDMI_3Dor4K2KProcess(void* pInstance, MS_U8 u8HDMIInfoSource);
E_HDMI_ADDITIONAL_VIDEO_FORMAT mdrv_HDMI_GetVSAdditionalFormat(void* pInstance, MS_U8 u8HDMIInfoSource);
E_XC_3D_INPUT_MODE mdrv_HDMI_GetVS3DStructure(void* pInstance, MS_U8 u8HDMIInfoSource);
E_HDMI_3D_EXT_DATA_T mdrv_HDMI_GetVS3DExtendData(void* pInstance, MS_U8 u8HDMIInfoSource);
void mdrv_HDMI_GetVS3DMetaField(void* pInstance, MS_U8 u8HDMIInfoSource, sHDMI_3D_META_FIELD *pst3DMetaField);
MS_U8 mdrv_HDMI_GetVSVICCode(void* pInstance, MS_U8 u8HDMIInfoSource);
E_HDMI_VIC_4Kx2K_CODE mdrv_HDMI_GetVS4K2KVICCode(void* pInstance, MS_U8 u8HDMIInfoSource);
void mdrv_HDMI_DVIMuxSwitch(void* pInstance, E_MUX_INPUTPORT enInputPortType, MS_U8 u8HDMIInfoSource);
void mdrv_HDMI_AverageScalingDown(void* pInstance, MS_U8 u8HDMIInfoSource, MS_BOOL bEnableFlag);
MS_U16 mdrv_HDMI_GetTimingInfo(void* pInstance, E_HDMI_GET_DATA_INFO enInfoType, MS_U8 u8HDMIInfoSource, MS_BOOL bNewPathFlag);
void mdrv_HDMI_PacketReset(void* pInstance, HDMI_REST_t enResetType, MS_U8 u8HDMIInfoSource);
#ifdef CONFIG_UTOPIA_PROC_DBG_SUPPORT
void mdrv_HDMI_MDCMDEchoCommand(void* pInstance, MS_U64* u64ReqHdl, char* pcCmdLine);
void mdrv_HDMI_MDCMDGetInfo(void* pInstance, MS_U64* u64ReqHdl);
#endif
/*************************** HDCP Repeater ***************************/
void mdrv_HDCP_WriteX74(void* pInstance, E_MUX_INPUTPORT enInputPortType, MS_U8 ucOffset, MS_U8 ucData);
MS_U8 mdrv_HDCP_ReadX74(void* pInstance, E_MUX_INPUTPORT enInputPortType, MS_U8 ucOffset);
void mdrv_HDCP_SetRepeater(void* pInstance, E_MUX_INPUTPORT enInputPortType, MS_BOOL bIsRepeater);
void mdrv_HDCP_SetBstatus(void* pInstance, E_MUX_INPUTPORT enInputPortType, MS_U16 usBstatus);
void mdrv_HDCP_SetHDMIMode(void* pInstance, E_MUX_INPUTPORT enInputPortType, MS_BOOL bHDMIMode);
MS_U8 mdrv_HDCP_GetInterruptStatus(void* pInstance, E_MUX_INPUTPORT enInputPortType);
void mdrv_HDCP_WriteKSVList(void* pInstance, E_MUX_INPUTPORT enInputPortType, MS_U8* pucKSV, MS_U32 ulDataLen);
void mdrv_HDCP_SetVPrime(void* pInstance, E_MUX_INPUTPORT enInputPortType, MS_U8* pucVPrime);
/*************************** HDCP Repeater ***************************/
MS_BOOL MDrv_HDMI_IOCTL(MS_U32 ulCommand, void *pBuffer, MS_U32 ulBufferSize);
void mdrv_HDCP22_FillCipherKey(void* pInstance, MS_U8 ucPortIdx, MS_U8* pucRiv, MS_U8* pucSessionKey);
void mdrv_HDCP22_PortInit(void* pInstance, MS_U8 ucPortIdx);
MS_BOOL mdrv_HDCP22_PollingReadDone(void* pInstance, MS_U8 ucPortIdx);
void mdrv_HDCP22_EnableCipher(void* pInstance, MS_U8 ucPortType, MS_U8 ucPortIdx, MS_BOOL bIsEnable);
void mdrv_HDCP22_SendMessage(void* pInstance, MS_U8 ucPortType, MS_U8 ucPortIdx, MS_U8* pucData, MS_U32 dwDataLen, void* pDummy);
MS_BOOL mdrv_HDCP22_Handler(void* pInstance, MS_U8 ucPortIdx, MS_U8* ucPortType, MS_U8* pucMsgData, MS_U32* wMsgLen, MS_BOOL bIRQModeFlag);
MS_U8 mdrv_HDCP22_WaitEvent(void* pInstance);
void mdrv_HDCP22_SetEvent(void* pInstance, MS_U16 usHDCPEventIndex);

#endif //_API_HDMI_RX_PRIVATE_H_
