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

#include <typedef.h>
#include <hal_usb_globalvar.h>
#include <hal_usb_platform.h>
#include <hal_usb_drc.h>
#include <hal_usb_otg.h>
#include <hal_usb_function.h>
#include <hal_usb_dma.h>
#include <hal_usb.h>

/* VVVIP structure */
UsbVar_t g_tUsbStruct;

#define MAXIMUM_LENGTH_OF_USB_VENDOR_NAME    8
const char sys_UsbVendorName[MAXIMUM_LENGTH_OF_USB_VENDOR_NAME+1] = "";

void DrvUsbUtilitySetUsbVendorName(pUsbVar ptUsb)
{
	u8 u8VendorTmp[USB_VENDOR_NAME_SIZE];
	u8 i,j;
    
	if (sys_UsbVendorName[0] != 0x00) {
		MsMemSet(ptUsb->Usb_Vendor_Name,
			0x00, sizeof(ptUsb->Usb_Vendor_Name));

		if (sizeof(sys_UsbVendorName) < (USB_VENDOR_NAME_SIZE+1))
			MsMemCpy(ptUsb->Usb_Vendor_Name,
				sys_UsbVendorName, sizeof(sys_UsbVendorName));
		else
			MsMemCpy(ptUsb->Usb_Vendor_Name,
				sys_UsbVendorName, USB_VENDOR_NAME_SIZE);

		MsMemSet(u8VendorTmp, 0x00, sizeof(u8VendorTmp));

		j = 0;

		for(i = 0; i < USB_VENDOR_NAME_SIZE; i++) {
			if (ptUsb->Usb_Vendor_Name[i] == 0x00)
				u8VendorTmp[j++] = 0x20;
		}

		for(i = 0; i < USB_VENDOR_NAME_SIZE; i++) {
			if (ptUsb->Usb_Vendor_Name[i] != 0x00)
				u8VendorTmp[j++] = ptUsb->Usb_Vendor_Name[i];
		}

		for(i = 0; i < USB_VENDOR_NAME_SIZE; i++) {
			ptUsb->Usb_Vendor_Name[i] = u8VendorTmp[i];
			_USB_OTG_MSG("Usb_Vendor_Name=%x\n",
					ptUsb->Usb_Vendor_Name[i]);
		}

		ptUsb->nUsbSpVendor=1;
	} else {
		_USB_OTG_MSG("There is no vendor name.\n");
	}
}

void DrvUsbUtilityInitUsbVar(pUsbVar ptUsb)
{
	int i;
	Endpoint_t *ep;
    
	ptUsb->nUsbSpVendor = 0;
	ptUsb->nUsb20TestMode = 0;
	ptUsb->nStdReqSetAddr = 0;
	ptUsb->nStdReqCleareEndpoint = 0;
	ptUsb->nStdReqGetStatusResponse = 0;
	ptUsb->nStdReqDeviceHnpSupport = 1;
	ptUsb->nFreeDmaChannels = 0x7f;
	ptUsb->nOtgRegPower = 0;
	ptUsb->nOtgInterruptStatus = 0;
	ptUsb->nOtgDmaRxInterruptStatus = 0;
	ptUsb->nOtgDmaTxInterruptStatus = 0;
	ptUsb->nOtgDataPhaseDir = 0;
	ptUsb->nOtgReqState = 0;
	ptUsb->nOtgCurruntState = 0;
	ptUsb->nOtgSuspended = 0;
	ptUsb->nOtgResume = 0;
	ptUsb->nOtgRemoteWakeup = 0;
	ptUsb->nOtgHnpEnable = 0;
	ptUsb->nOtgHnpSupport = 0;
	ptUsb->nOtgSelfPower = 1;
	ptUsb->nStdReqConfig = 0;
	ptUsb->nStdReqInterface = 0;
	ptUsb->nOtgUsbState = 0;
	ptUsb->nOtgCid = 0;
	ptUsb->nStdReqFuncAddr = 0;
	ptUsb->nOtgRegDevCtl = 0;
	ptUsb->nOtgSpeed = 0;
	ptUsb->nOtgResetComplete = 0;
	ptUsb->tEp0Setup.nRequestType = 0;
	ptUsb->tEp0Setup.nRequest = 0;
	ptUsb->tEp0Setup.nValue = 0;
	ptUsb->tEp0Setup.nIndex = 0;
	ptUsb->tEp0Setup.nLength = 0;

	for (i =0 ; i < otgNumEPDefs; i++) {
		ep = &ptUsb->tUsbEndpointInf[i];
		ep->nEpMaxPacketSize = 0x40;
		ep->nEpBytesRequested = 0;
		ep->nEpBytesProcessed = 0;
		ep->nEpPipe = 0;
		ep->nEpBlt = 0;
		ep->nEpDrcDir = 0;
		ep->nEpLastPacket = 0;
		ep->nEpIoState = 0;
		ep->nEpHalted = 0;
		ep->nEpTransferBuffer = 0;
		ep->nEpTransferBuffer_length = 0;
		ep->nEpFifoRemain = 0;
		ep->nEpInfNum = 0;
	}
	DrvUsbUtilitySetUsbVendorName(ptUsb);
}

s32 DrvUsbUtilitySwitchOnUSB(pUsbVar ptUsb)
{
	if (ptUsb->nOtgDeviceConnect == 0) {
		_USB_OTG_MSG("==>> nOtgDeviceConnect = 1\n");
		ptUsb->nOtgDeviceConnect = 1;
		return 0;
	} else
		return -1;
}

s32 DrvUsbUtilitySwitchOffUSB(pUsbVar ptUsb)
{
	if (ptUsb->nOtgDeviceConnect) {
		ptUsb->nFreeDmaChannels = 0x7f;
		ptUsb->nOtgUsbState=0;
		ptUsb->nOtgSuspended = 0;
		ptUsb->nOtgResume = 0;
		/* benson add for disconnect */
		ptUsb->nOtgDeviceConnect = 0;
		return 0;
	}
	return -1;
}

void DrvUsbUtilityCloseUtmi(pUsbVar ptUsb)
{
	HalUsbDrcDisableUSB();
}

/* out --> in */
void udc_init(void)
{
	/* set SW connection flag */
	DrvUsbUtilitySwitchOnUSB(&g_tUsbStruct);
	/* globle var init */
	DrvUsbUtilityInitUsbVar(&g_tUsbStruct);
	/* utmi init */
	HalUsbDrcUtmiInit();
	/* soft connect */
 	HalUsbDrcInit(&g_tUsbStruct);
}

void udc_start(void)
{
	/* enable interrupt */
	HalUsbDrcSetInterrupts();
}

void service_interrupts(void)
{
	/* don't care return var */
	OTG_Interrupt_Polling(0xff, &g_tUsbStruct);
}

int usb_read(void *_buf, unsigned len)
{
	int result = -1;

	_USB_OTG_MSG("[USB] usb_read buf:%p len:%d\n", _buf, len);
	result = HalUsbDrcPC2USB(_buf, len, &g_tUsbStruct);
	_USB_OTG_MSG("[USB] read result %d\n", result);

	if(result >= 0)
		return result;
	return -1;
}

int usb_write(void *_buf, unsigned len)
{
	int result = -1;

	_USB_OTG_MSG("[USB] usb_write buf:%p len:%d\n", _buf, len);
	result = HalUsbDrcUSB2PC(_buf, len, &g_tUsbStruct);
	_USB_OTG_MSG("[USB] write result %d\n", result);

	if(result == USB_ST_NOERROR)
		return len;
	else
		return -1;
}

/* in --> out */
#include <fastboot.h>
extern unsigned fastboot_state;
void udc_online(bool on)
{
	if(on) {
		if (fastboot_state == STATE_OFFLINE) {
			_USB_OTG_MSG("USB UDC is going online\n");
		}	
		
		fastboot_state = STATE_ONLINE;

	} else {
		if (fastboot_state == STATE_ONLINE) {
			_USB_OTG_MSG("USB UDC is going offline\n");
		}	
		fastboot_state = STATE_OFFLINE;
	}	
}


int usb_speed(void)
{
	if (g_tUsbStruct.nOtgResetComplete)
		return ((int)g_tUsbStruct.nOtgSpeed);

	return 0;
}

bool is_usb_configured(void)
{
	return(g_tUsbStruct.nOtgUsbState == USB_CONFIGURED);
}
