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

#ifndef __HAL_USB_GLOBALVAR_H__
#define __HAL_USB_GLOBALVAR_H__

#include <typedef.h>

/* include for extend EP number from 4 to otgNumEPDefs */
#include <hal_usb_platform.h>
#include <stdio.h>

/* #define USB_SEND_TRACE */
#define USB_SEND_TRACE_L1

#ifdef USB_SEND_TRACE
	#define _USB_OTG_MSG(_x_, ...)	printf(_x_,  ##__VA_ARGS__)
#else
	#define _USB_OTG_MSG(_x_, ...)	{}
#endif

#ifdef USB_SEND_TRACE_L1
	#define _USB_OTG_MSG_L1(_x_, ...)	printf(_x_,  ##__VA_ARGS__)
#else
	#define _USB_OTG_MSG_L1(_x_, ...)	{}
#endif

#define USB_VENDOR_NAME_SIZE		8
#define USB_BUFSIZE		(64*1024)

struct _DevRequest_t
{
	u8	nRequestType;
	u8	nRequest;
	u16	nValue;
	u16	nIndex;
	u16	nLength;
};

typedef struct
{
	u16	nEpMaxPacketSize;
	u16	nEpFifoRemain;
	u32	nEpBytesRequested;
	u32	nEpBytesProcessed;
	u8	nEpPipe;
	u8	nEpBlt;
	u8	nEpDrcDir;
	u8	nEpLastPacket;
	u8	nEpIoState;
	u8	nEpHalted;
	u8	nEpInfNum;
	u32	nEpTransferBuffer;
	u32	nEpTransferBuffer_length;
	bool	bEpSendZeroPkt;
} Endpoint_t;

typedef struct USB_VAR_
{
	Ms_Flag_t	tHostAttachedflag;
	Ms_Flag_t	tUsbFlag;
	Ms_Flag_t	tCdcFlag;
	u8 volatile	nChipType;
	u8 volatile	nUsbSpVendor;
	u8 volatile	nUvcSensorId;
	u16 volatile	nUsb20TestMode;
	u8 volatile	nStdReqSetAddr;
	u8 volatile	nStdReqCleareEndpoint;
	u16 volatile	nStdReqGetStatusResponse;
	u8 volatile	nStdReqDeviceHnpSupport;
	u8 volatile	nFreeDmaChannels;
	u8 volatile	nOtgRegPower;
	u8 volatile	nOtgInterruptStatus;
	u8 volatile	nOtgDmaRxInterruptStatus;
	u8 volatile	nOtgDmaTxInterruptStatus;
	u8 volatile	nOtgDataPhaseDir;
	u8 volatile	nOtgReqState;
	u8 volatile	nOtgCurruntState;
	u8 volatile	nOtgSuspended;
	u8 volatile	nOtgResume;
	u8 volatile	nOtgRemoteWakeup;
	u8 volatile	nOtgHnpEnable;
	u8 volatile	nOtgHnpSupport;
	u8 volatile	nOtgSelfPower;
	u8 volatile	nStdReqConfig;
	u8 volatile	nStdReqInterface;
	u8 volatile	nOtgUsbState;
	u8 volatile	nOtgCid;
	u8 volatile	nStdReqFuncAddr;
	u8 volatile	nOtgRegDevCtl;
	u8 volatile	nOtgSpeed;
	u8 volatile	nOtgResetComplete;
	u8 volatile	nOtgDeviceConnect;

	/* replace tUsbEndpointInf[4] to tUsbEndpointInf[otgNumEPDefs] */
	Endpoint_t 	tUsbEndpointInf[otgNumEPDefs];
	struct _DevRequest_t volatile	tEp0Setup;
	u8 volatile	nStdReqSetupCmdBuf[32];
	u8 Usb_Vendor_Name[USB_VENDOR_NAME_SIZE];
} UsbVar_t, *pUsbVar;
#endif	/* _USB_GLOBALVAR_H_ */
