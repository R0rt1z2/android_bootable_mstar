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

/*
    1. Check whether MHL cable is plugged or not in Rx side
    2. Disable R-term and output Vbus
    3. Check whether Cbus is connected(discovery pulse completed)
    4. Sink -> Source: "SET_HPD"
    5. Sink -> Source: Write_Stat - "DCAP_RDY" and Write_INT - "DCAP_CHG"
    6. Sink -> Source: Write_Stat - "PATH_EN"
*/


#ifndef _API_CEC_PRIVATE_H_
#define _API_CEC_PRIVATE_H_

#include "MsTypes.h"
#include "MsDevice.h"
#include "MsCommon.h"
#include "apiCEC.h"


#ifdef __cplusplus
extern "C"
{
#endif

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------
typedef struct __attribute__ ((packed)) _CEC_RESOURCE_PRIVATE
{
    MS_BOOL bCECIsSource;
    MS_BOOL bRIUAddrReady;
    MS_BOOL bCecInitFinish;
    MS_BOOL bCecISRAttach;
    MS_BOOL CecDevicesExisted[CEC_DEVICE_EXIST_COUNT];
    MS_BOOL bAttachDrvISR;
    MS_U8 ucRetryCount;
    MS_U8 ucInitialIndex;
    MS_U8 CecFifoIdxS;
    MS_U8 CecFifoIdxE;
    MS_U8 bCecMsgCnt;
    MS_U8 ActiveDeviceCECVersion;
    MS_U8 MyPhysicalAddress[CEC_PHY_ADDR_COUNT];
    MS_U8 ActivePhysicalAddress[CEC_PHY_ADDR_COUNT];
    MS_U8 ucVendorID[CEC_VENDOR_ID_COUNT];
    MS_U32 ulXTAL_CLK_Hz;
    MS_VIRT ulXCRiuBaseAddress;
#if !defined (__aarch64__)
    MS_U32 ulXCRiuBaseAddressDummy;
#endif
    MS_VIRT ulPMRiuBaseAddress;
#if !defined (__aarch64__)
    MS_U32 ulPMRiuBaseAddressDummy;
#endif
    MsCEC_DEVICELA MyLogicalAddress;                         //TV related
    MsCEC_DEVICE_TYPE MyDeviceType;
    MsCEC_MSG_POWER_STATUS_PARM MyPowerStatus;
    MsCEC_DEVICE_TYPE ActiveDeviceType;
    MsCEC_DEVICELA ActiveLogicalAddress;    //the remoter controller's active device
    MsCEC_MSG_POWER_STATUS_PARM ActivePowerStatus;
    EN_POWER_MODE usPrePowerState;
    MS_CEC_RxData_Info CecRxBuf[CEC_FIFO_NUM_COUNT];   //TV CEC H/W part
    MS_CEC_RxData_Info CecRxRecBuf[CEC_REC_FIFO_NUM_COUNT];
    MS_CEC_TxData_Info CecTxRecBuf[CEC_REC_FIFO_NUM_COUNT];
    MS_U8 CecRxRecFifoIdxE;
    MS_U8 CecTxRecFifoIdxE;
    MS_U8 CecRxStartPoint;
    MS_U8 CecTxStartPoint;
#if ENABLE_CEC_MULTIPLE
    MS_U8 MyPhysicalAddress2[CEC_PHY_ADDR_COUNT];
    MsCEC_DEVICELA MyLogicalAddress2;
    MsCEC_DEVICE_TYPE MyDeviceType2;
    MsCEC_DEVICE_TYPE IniLogicalAddress;
#endif
}CEC_RESOURCE_PRIVATE;

typedef MS_BOOL           (*IOCTL_DDC2BI_GET_INFO)                    (void*, MS_CEC_INFO_LIST*);
typedef void              (*IOCTL_CEC_INIT_CHIP)                       (void*, MS_U32);
typedef void              (*IOCTL_CEC_PORT_SELECT)                    (void*, MsCEC_INPUT_PORT);
typedef void              (*IOCTL_CEC_EXIT)                             (void*);
typedef void              (*IOCTL_CEC_SET_MY_LOGICAL_ADDRESS)       (void*, MsCEC_DEVICELA);
typedef void              (*IOCTL_CEC_INIT)                             (void*, MS_U32);
typedef void              (*IOCTL_CEC_CHECK_EXIST_DEVICES)          (void*);
typedef MsCEC_DEVICELA    (*IOCTL_CEC_NEXT_DEVICE)                    (void*);
typedef void              (*IOCTL_CEC_CHK_RX_BUF)                      (void*);
typedef CEC_ERROR_CODE    (*IOCTL_CEC_TX_SEND_MSG)                    (void*, MsCEC_DEVICELA, MsCEC_MSGLIST, MS_U8*, MS_U8);
typedef CEC_ERROR_CODE    (*IOCTL_CEC_TX_SEND_MSG2)                   (void*, MsCEC_DEVICELA, MsCEC_MSGLIST, MS_U8*, MS_U8);
typedef CEC_ERROR_CODE    (*IOCTL_CEC_TX_SEND_POLLING_MSG)          (void*, MsCEC_DEVICELA);
typedef CEC_ERROR_CODE    (*IOCTL_CEC_MSG_ACTIVE_SOURCE)             (void*);
typedef CEC_ERROR_CODE    (*IOCTL_CEC_MSG_ROUTING_CHANGE)            (void*, MS_U8*, MS_U8*);
typedef CEC_ERROR_CODE    (*IOCTL_CEC_MSG_REQ_ACTIVE_SOURCE)        (void*);
typedef CEC_ERROR_CODE    (*IOCTL_CEC_MSG_SET_STREAM_PATH)           (void*, MS_U8*);
typedef CEC_ERROR_CODE    (*IOCTL_CEC_MSG_STANDBY)                     (void*, MsCEC_DEVICELA);
typedef CEC_ERROR_CODE    (*IOCTL_CEC_MSG_RECORD_OFF)                 (void*, MsCEC_DEVICELA);
typedef CEC_ERROR_CODE    (*IOCTL_CEC_MSG_RECORD_ON)                  (void*, MsCEC_DEVICELA, MsCEC_MSG_RECORD_SOURCE_TYPE);
typedef CEC_ERROR_CODE    (*IOCTL_CEC_MSG_REPORT_CEC_VERSION)       (void*, MsCEC_DEVICELA, MS_U8);
typedef CEC_ERROR_CODE    (*IOCTL_CEC_MSG_REQ_CEC_VERSION)           (void*, MsCEC_DEVICELA);
typedef CEC_ERROR_CODE    (*IOCTL_CEC_MSG_REPORT_PHYCAL_ADDRESS)    (void*);
typedef CEC_ERROR_CODE    (*IOCTL_CEC_MSG_REQ_PHYCAL_ADDRESS)        (void*, MsCEC_DEVICELA);
typedef CEC_ERROR_CODE    (*IOCTL_CEC_MSG_DECK_CONTROL)               (void*, MsCEC_DEVICELA, MsCEC_MSG_DC_DECK_CTRL_PARM);
typedef CEC_ERROR_CODE    (*IOCTL_CEC_MSG_DEC_STATUS)                 (void*, MsCEC_DEVICELA, MsCEC_MSG_DC_DECK_STATUS_PARM);
typedef CEC_ERROR_CODE    (*IOCTL_CEC_MSG_GIVE_DECK_STATUS)          (void*, MsCEC_DEVICELA, MsCEC_MSG_STATUS_REQUEST_PARM);
typedef CEC_ERROR_CODE    (*IOCTL_CEC_MSG_DC_PLAY)                     (void*, MsCEC_DEVICELA, MsCEC_MSG_DC_PLAY_PARM);
typedef CEC_ERROR_CODE    (*IOCTL_CEC_MSG_REQ_MENU_STATUS)           (void*, MsCEC_DEVICELA, MsCEC_MSG_MENU_REQ_TYPE);
typedef CEC_ERROR_CODE    (*IOCTL_CEC_MSG_USER_CTRL_PRESSED)         (void*, MS_BOOL, MsCEC_DEVICELA, MsCEC_MSG_USER_CTRL_PARM);
typedef CEC_ERROR_CODE    (*IOCTL_CEC_MSG_USER_CTRL_RELEASED)        (void*, MsCEC_DEVICELA);
typedef CEC_ERROR_CODE    (*IOCTL_CEC_MSG_GIVE_AUDIO_STATUS)         (void*, MsCEC_DEVICELA);
typedef CEC_ERROR_CODE    (*IOCTL_CEC_MSG_REPORT_POWER_STATUS)       (void*, MsCEC_DEVICELA);
typedef CEC_ERROR_CODE    (*IOCTL_CEC_MSG_REQ_POWER_STATUS)          (void*, MsCEC_DEVICELA);
typedef CEC_ERROR_CODE    (*IOCTL_CEC_MSG_FEATURE_ABORT)              (void*, MsCEC_DEVICELA, MsCEC_MSGLIST, MsCEC_MSG_ABORT_REASON);
typedef CEC_ERROR_CODE    (*IOCTL_CEC_MSG_ABORT)                       (void*, MsCEC_DEVICELA);
typedef CEC_ERROR_CODE    (*IOCTL_CEC_MSG_SEND_MENU_LANGUAGE)       (void*, MS_U8*);
typedef CEC_ERROR_CODE    (*IOCTL_CEC_MSG_REQ_ARC_INITIATION)       (void*, MsCEC_DEVICELA);
typedef CEC_ERROR_CODE    (*IOCTL_CEC_MSG_REQ_ARC_TERMINATION)      (void*, MsCEC_DEVICELA);
typedef CEC_ERROR_CODE    (*IOCTL_CEC_MSG_AUDIO_MODE_REQ)            (void*, MsCEC_DEVICELA, MS_BOOL, MS_U8*);
typedef CEC_ERROR_CODE    (*IOCTL_CEC_MSG_REPORT_PHYCAL_ADDRESS2)   (void*);
typedef MS_BOOL           (*IOCTL_CEC_CHECK_FRAME)                      (void*, MsCEC_MSG_TRANS_TYPE, MS_U8);
typedef void              (*IOCTL_CEC_CONFIG_WAKE_UP)                  (void*);
typedef void              (*IOCTL_CEC_ENABLED)                          (void*, MS_BOOL);
typedef MS_U8             (*IOCTL_CEC_GET_TX_STATUS)                   (void*);
typedef MS_BOOL           (*IOCTL_CEC_DEVICE_IS_TX)                     (void*);
typedef MS_U32            (*IOCTL_CEC_SET_POWER_STATE)                 (void*, EN_POWER_MODE);
typedef void              (*IOCTL_CEC_SET_MY_LOGICAL_ADDRESS2)       (void*, MsCEC_DEVICELA);
typedef MS_U8             (*IOCTL_CEC_GET_HEADER)                       (void*);
typedef MS_U8             (*IOCTL_CEC_GET_OPCODE)                       (void*);
typedef MS_U8             (*IOCTL_CEC_GET_PARA)                          (void*, MS_U8);
typedef MS_U8             (*IOCTL_CEC_GET_CMD_LEN)                      (void*);
typedef MS_BOOL           (*IOCTL_CEC_IS_RX_BUF_EMPTY)                 (void*);
typedef void              (*IOCTL_CEC_SET_ACTIVE_LOGICAL_ADDRESS)   (void*, MsCEC_DEVICELA);
typedef MsCEC_DEVICELA    (*IOCTL_CEC_GET_ACTIVE_LOGICAL_ADDRESS)   (void*);
typedef MsCEC_MSG_POWER_STATUS_PARM    (*IOCTL_CEC_GET_POWER_STATUS)   (void*);
typedef MS_U8             (*IOCTL_CEC_GET_FIFO_IDX)                     (void*);
typedef void              (*IOCTL_CEC_SET_FIFO_IDX)                     (void*, MS_U8);
typedef void              (*IOCTL_CEC_SET_ACTIVE_POWER_STATUS)       (void*, MsCEC_MSG_POWER_STATUS_PARM);
typedef MsCEC_MSG_POWER_STATUS_PARM    (*IOCTL_CEC_GET_ACTIVE_POWER_STATUS)       (void*);
typedef void              (*IOCTL_CEC_SET_ACTIVE_PHYSICAL_ADDRESS)   (void*, MS_U8, MS_U8);
typedef void              (*IOCTL_CEC_SET_ACTIVE_DEVICE_CEC_VERSION) (void*, MS_U8);
typedef void              (*IOCTL_CEC_SET_ACTIVE_DEVICE_TYPE)          (void*, MsCEC_DEVICE_TYPE);
typedef MS_U8             (*IOCTL_CEC_GET_MSG_CNT)                       (void*);
typedef void              (*IOCTL_CEC_SET_MSG_CNT)                       (void*, MS_U8);
typedef MS_U8             (*IOCTL_CEC_GET_RX_DATA)                       (void*, MS_U8, MS_U8);
typedef void              (*IOCTL_CEC_SET_MY_PHYSICAL_ADDRESS)        (void*, MS_U8*);
typedef void              (*IOCTL_CEC_SET_MY_PHYSICAL_ADDRESS2)       (void*, MS_U8*);
typedef void              (*IOCTL_CEC_SET_INITIATOR)                     (void*, MsCEC_DEVICELA);
typedef void              (*IOCTL_CEC_CONFIG_WAKEUP_INFO_VENDOR_ID)  (void*, MS_U8*);
typedef void              (*IOCTL_CEC_SET_RETRY_COUNT)                  (void*, MS_U8);
typedef void              (*IOCTL_CEC_ATTACH_DRIVER_ISR)               (void*, MS_BOOL);
typedef stCEC_INITIAL_CONFIG_INFO     (*IOCTL_CEC_GET_CONFIGURATION)    (void*);
typedef void              (*IOCTL_CEC_SET_CHIP_TYPE)                    (void*, MsCEC_CHIP_TYPE);

typedef struct _CEC_INSTANT_PRIVATE
{
    MS_U32                                   ulDeviceID;
    IOCTL_DDC2BI_GET_INFO                    fpDDC2BIGetInfo;
    IOCTL_CEC_INIT_CHIP                      fpCECInitChip;
    IOCTL_CEC_PORT_SELECT                    fpCECPortSelect;
    IOCTL_CEC_EXIT                           fpCECExit;
    IOCTL_CEC_SET_MY_LOGICAL_ADDRESS         fpCECSetMyLogicalAddress;
    IOCTL_CEC_INIT                           fpCECInit;
    IOCTL_CEC_CHECK_EXIST_DEVICES            fpCECCheckExistDevices;
    IOCTL_CEC_NEXT_DEVICE                    fpCECNextDevice;
    IOCTL_CEC_CHK_RX_BUF                     fpCECChkRxBuf;
    IOCTL_CEC_TX_SEND_MSG                    fpCECTxSendMsg;
    IOCTL_CEC_TX_SEND_MSG2                   fpCECTxSendMsg2;
    IOCTL_CEC_TX_SEND_POLLING_MSG            fpCECTxSendPollingMsg;
    IOCTL_CEC_MSG_ACTIVE_SOURCE              fpCECMsgActiveSource;
    IOCTL_CEC_MSG_ROUTING_CHANGE             fpCECMsgRoutingChange;
    IOCTL_CEC_MSG_REQ_ACTIVE_SOURCE          fpCECMsgReqActiveSource;
    IOCTL_CEC_MSG_SET_STREAM_PATH            fpCECMsgSetStreamPath;
    IOCTL_CEC_MSG_STANDBY                    fpCECMsgStandby;
    IOCTL_CEC_MSG_RECORD_OFF                 fpCECMsgRecordOff;
    IOCTL_CEC_MSG_RECORD_ON                  fpCECMsgRecordOn;
    IOCTL_CEC_MSG_REPORT_CEC_VERSION         fpCECMsgReportCECVersion;
    IOCTL_CEC_MSG_REQ_CEC_VERSION            fpCECMsgReqCECVersion;
    IOCTL_CEC_MSG_REPORT_PHYCAL_ADDRESS      fpCECMsgReportPhycalAddress;
    IOCTL_CEC_MSG_REQ_PHYCAL_ADDRESS         fpCECMsgReqPhycalAddress;
    IOCTL_CEC_MSG_DECK_CONTROL               fpCECMsgDeckControl;
    IOCTL_CEC_MSG_DEC_STATUS                 fpCECMsgDecStatus;
    IOCTL_CEC_MSG_GIVE_DECK_STATUS           fpCECMsgGiveDeckStatus;
    IOCTL_CEC_MSG_DC_PLAY                    fpCECMsgDCPlay;
    IOCTL_CEC_MSG_REQ_MENU_STATUS            fpCECMsgReqMenuStatus;
    IOCTL_CEC_MSG_USER_CTRL_PRESSED          fpCECMsgUserCtrlPressed;
    IOCTL_CEC_MSG_USER_CTRL_RELEASED         fpCECMsgUserCtrlReleased;
    IOCTL_CEC_MSG_GIVE_AUDIO_STATUS          fpCECMsgGiveAudioStatus;
    IOCTL_CEC_MSG_REPORT_POWER_STATUS        fpCECMsgReportPowerStatus;
    IOCTL_CEC_MSG_REQ_POWER_STATUS           fpCECMsgReqPowerStatus;
    IOCTL_CEC_MSG_FEATURE_ABORT              fpCECMsgFeatureAbort;
    IOCTL_CEC_MSG_ABORT                      fpCECMsgAbort;
    IOCTL_CEC_MSG_SEND_MENU_LANGUAGE         fpCECMsgSendMenuLanguage;
    IOCTL_CEC_MSG_REQ_ARC_INITIATION         fpCECMsgReqARCInitiation;
    IOCTL_CEC_MSG_REQ_ARC_TERMINATION        fpCECMsgReqARCTermination;
    IOCTL_CEC_MSG_AUDIO_MODE_REQ             fpCECMsgAudioModeReq;
    IOCTL_CEC_MSG_REPORT_PHYCAL_ADDRESS2     fpCECMsgReportPhycalAddress2;
    IOCTL_CEC_CHECK_FRAME                    fpCECCheckFrame;
    IOCTL_CEC_CONFIG_WAKE_UP                 fpCECConfigWakeUp;
    IOCTL_CEC_ENABLED                        fpCECEnabled;
    IOCTL_CEC_GET_TX_STATUS                  fpCECGetTxStatus;
    IOCTL_CEC_DEVICE_IS_TX                   fpCECDeviceIsTx;
    IOCTL_CEC_SET_POWER_STATE                fpCECSetPowerState;
    IOCTL_CEC_SET_MY_LOGICAL_ADDRESS2        fpCECSetMyLogicalAddress2;
    IOCTL_CEC_GET_HEADER                     fpCECGetHeader;
    IOCTL_CEC_GET_OPCODE                     fpCECGetOpCode;
    IOCTL_CEC_GET_PARA                       fpCECGetPara;
    IOCTL_CEC_GET_CMD_LEN                    fpCECGetCmdLen;
    IOCTL_CEC_IS_RX_BUF_EMPTY                fpCECIsRxBufEmpty;
    IOCTL_CEC_SET_ACTIVE_LOGICAL_ADDRESS     fpCECSetActiveLogicalAddress;
    IOCTL_CEC_GET_ACTIVE_LOGICAL_ADDRESS     fpCECGetActiveLogicalAddress;
    IOCTL_CEC_GET_POWER_STATUS               fpCECGetPowerStatus;
    IOCTL_CEC_GET_FIFO_IDX                   fpCECGetFifoIdx;
    IOCTL_CEC_SET_FIFO_IDX                   fpCECSetFifoIdx;
    IOCTL_CEC_SET_ACTIVE_POWER_STATUS        fpCECSetActivePowerStatus;
    IOCTL_CEC_GET_ACTIVE_POWER_STATUS        fpCECGetActivePowerStatus;
    IOCTL_CEC_SET_ACTIVE_PHYSICAL_ADDRESS    fpCECSetActivePhysicalAddress;
    IOCTL_CEC_SET_ACTIVE_DEVICE_CEC_VERSION  fpCECSetActiveDeviceCECVersion;
    IOCTL_CEC_SET_ACTIVE_DEVICE_TYPE         fpCECSetActiveDeviceType;
    IOCTL_CEC_GET_MSG_CNT                    fpCECGetMsgCnt;
    IOCTL_CEC_SET_MSG_CNT                    fpCECSetMsgCnt;
    IOCTL_CEC_GET_RX_DATA                    fpCECGetRxData;
    IOCTL_CEC_SET_MY_PHYSICAL_ADDRESS        fpCECSetMyPhysicalAddress;
    IOCTL_CEC_SET_MY_PHYSICAL_ADDRESS2       fpCECSetMyPhysicalAddress2;
    IOCTL_CEC_SET_INITIATOR                  fpCECSetInitiator;
    IOCTL_CEC_CONFIG_WAKEUP_INFO_VENDOR_ID   fpCECConfigWakeUpInfoVendorID;
    IOCTL_CEC_SET_RETRY_COUNT                fpCECSetRetryCount;
    IOCTL_CEC_ATTACH_DRIVER_ISR              fpCECAttachDriverISR;
    IOCTL_CEC_GET_CONFIGURATION              fpCECGetConfiguration;
    IOCTL_CEC_SET_CHIP_TYPE                  fpCECSetChipType;
}CEC_INSTANT_PRIVATE;

typedef enum
{
    CEC_POOL = 0,
    CEC_POOL_MAX,
}eCECPoolID;

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Function Prototype
//-------------------------------------------------------------------------------------------------
void CECRegisterToUtopia(FUtopiaOpen ModuleType);

#endif //_API_CEC_PRIVATE_H_
