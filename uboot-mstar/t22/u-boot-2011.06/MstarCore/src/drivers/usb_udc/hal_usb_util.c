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
#include <hal_usb_dma.h>
#include <hal_usb_otg.h>
#include <hal_usb.h>
#include <hal_usb_function.h>

void HalUsbUtilChgUSBState(u8 toUSB,UsbVar_t *gusb)
{
	switch (toUSB) {
		/* HOST:CONNECT DRC INT;PER:exit active mode */
		case	USB_POWERED:
			gusb->nOtgSuspended = 0;
			break;

		/* HOST:Reset complete;PER: RESET DRC INT */
		case	USB_DEFAULT:
			gusb->nOtgHnpEnable = 0;
			gusb->nOtgHnpSupport = 0;
			gusb->nOtgRemoteWakeup = 0;
			gusb->nOtgSuspended = 0;
			break;

		case	USB_ADDRESS:
			gusb->nStdReqConfig = 0;
			break;

		case	USB_CONFIGURED:
			break;

		default:
			break;
	}

	gusb->nOtgUsbState = toUSB;
	/* in -> out, report device configured to fastboot function */
	if(gusb->nOtgUsbState == USB_CONFIGURED)
		udc_online(true);
	else
		udc_online(false);
}

u8 HalUsbUtilIsConfiged(pUsbVar ptUsb)
{
	if (ptUsb->nOtgUsbState == USB_CONFIGURED)
		return 1;
	else
		return 0;
}

s8 HalUsbUtilSendStall(u8 ep,UsbVar_t *gusb)
{
	u32 csr, reg, bit;

	gusb->tUsbEndpointInf[ep].nEpHalted = 1;

	if (gusb->tUsbEndpointInf[ep].nEpBlt == 0) {
		reg = M_REG_CSR0;
		bit = M_CSR0_P_SENDSTALL;
	} else {
		HalUsbDrcIndexSelect(gusb->tUsbEndpointInf[ep].nEpBlt);
		if (gusb->tUsbEndpointInf[ep].nEpDrcDir) { /* RX */
			reg = M_REG_RXCSR1;
			bit = M_RXCSR1_P_SENDSTALL;
		} else { /* TX */
			reg = M_REG_TXCSR1;
			bit = M_TXCSR1_P_SENDSTALL;
		}
	}

	csr = USB_REG_READ8(reg);
	USB_REG_WRITE8(reg, (csr | bit));

	/* _USB_OTG_MSG("HalUsbUtilSendStall ... \n"); */

	_USB_OTG_MSG("Start polling EP0 interrupt. vec: 0x%X \n", 0x20);
	while(!OTG_Interrupt_Polling(0x20, gusb));
	_USB_OTG_MSG("Start polling interrupt. vec: 0x%X \n", 0x20);

	if (gusb->tUsbEndpointInf[ep].nEpDrcDir) { /* RX */
		while((USB_REG_READ8(M_REG_RXCSR1) & M_RXCSR1_P_SENDSTALL) &&
			(gusb->nOtgDeviceConnect==1) && (!gusb->nOtgResetComplete));
	} else { /* TX */
		while((USB_REG_READ8(M_REG_TXCSR1) & M_TXCSR1_P_SENDSTALL) &&
			(gusb->nOtgDeviceConnect==1) && (!gusb->nOtgResetComplete));
	}
	return SUSS;
}

void HalUsbUtilSwopSetup(struct _DevRequest_t *sP)
{
	SWOP(sP->nValue);
	SWOP(sP->nIndex);
	SWOP(sP->nLength);
}
