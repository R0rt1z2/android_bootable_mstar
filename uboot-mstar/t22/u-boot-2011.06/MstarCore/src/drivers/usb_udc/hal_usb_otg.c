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

void _HalUsbOtgReadCID(pUsbVar ptUsb)
{
	/* read device control */
	HalUsbDrcReadRegDevctl(ptUsb);

	/* determine the device */
	if (ptUsb->nOtgRegDevCtl & M_DEVCTL_SESSION)
		ptUsb->nOtgCid =
			(ptUsb->nOtgRegDevCtl & M_DEVCTL_BDEVICE) >> 7;
	else
		ptUsb->nOtgCid = CID_UNKNOWN;
}

void HalUsbOtgParseIntrUsb(u16 nIntrUsb, pUsbVar ptUsb)
{
	Endpoint_t *ep_in = &ptUsb->tUsbEndpointInf[EPBULKIN];
	Endpoint_t *ep_out = &ptUsb->tUsbEndpointInf[EPBULKOUT];

	if (nIntrUsb & M_INTR_VBUSERROR) {
		_USB_OTG_MSG("[USB] M_INTR_VBUSERROR INT\n");
	}

	if (nIntrUsb & M_INTR_CONNECT) {
		/* only valid in host mode */
		_USB_OTG_MSG("[USB] M_INTR_CONNECT INT\n");
	}

	if (nIntrUsb & M_INTR_SUSPEND) {
		_USB_OTG_MSG("[USB] M_INTR_SUSPEND INT\n");
		if (ptUsb->nOtgUsbState == USB_CONFIGURED) {
			_USB_OTG_MSG("[USB] SUSPEND = %x\n",
					USB_REG_READ16(M_REG_CFG2_L));
			if ((USB_REG_READ16(M_REG_CFG2_L)&0x0008) == 0x0008) {
				ptUsb->nOtgSuspended = 1;
				_USB_OTG_MSG("USB_SUSPEND = %x\n",
							ptUsb->nOtgSuspended);
			}
		}
	}

	if ((nIntrUsb & M_INTR_RESUME)||(nIntrUsb & M_INTR_RESET)) {
		if (DRCPERIPHERAL(ptUsb->nOtgRegDevCtl)) {
			if (ptUsb->nOtgSuspended) {
				ptUsb->nOtgResume = 1;
			}
		}

		_HalUsbOtgReadCID(ptUsb);

		if (nIntrUsb & M_INTR_RESET) {
			_USB_OTG_MSG_L1("[USB] M_INTR_RESET INT\n");

			if (DRCPERIPHERAL(ptUsb->nOtgRegDevCtl)) {
				/* release all DMA channel */
				ptUsb->nFreeDmaChannels = 0x7f;
				/* Reset all epoint to idle and zero state. */
				_HalUsbOtgResetAllEpVars(ptUsb);
				ptUsb->nStdReqFuncAddr = 0;
				ptUsb->nOtgResetComplete = 1;
				HalUsbUtilChgUSBState(USB_DEFAULT,ptUsb);

				if(ptUsb->nOtgCurruntState == AB_IDLE) {
					/* change otg state to B device. */
					_HalUsbOtgChgState(B_PERIPHERAL,ptUsb);
				}

				HalUsbDrcReadRegPower(ptUsb);

				if (ptUsb->nOtgRegPower & M_POWER_HSMODE) {
					ptUsb->nOtgSpeed = HIGH_SPEED;
#ifndef USB_FPGA
					/*B2 analog parameter */
					UTMI_REG_WRITE16(0x58, 0x0230);
#endif
					ep_in->nEpMaxPacketSize = 0x200;
					ep_out->nEpMaxPacketSize = 0x200;
					_USB_OTG_MSG_L1("nOtgSpeed = HIGH_SPEED \n\r");
				} else {
					_USB_OTG_MSG_L1("nOtgSpeed = FULL_SPEED \n\r");

					ptUsb->nOtgSpeed = FULL_SPEED;
					ep_in->nEpMaxPacketSize = 0x40;
					ep_out->nEpMaxPacketSize = 0x40;
#ifndef USB_FPGA
					/* B2 analog parameter */
					UTMI_REG_WRITE16(0x58, 0x0030);
					_USB_OTG_MSG("OTG UTMI Reg[0x58] = %x\n",
								UTMI_REG_READ16(0x58));
#endif
				}

				/* reset all EP when set config/interface */
				HalUsbFunEPInitial(ptUsb);

				*(DMA_CNTL_REGISTER(MAX_DMA_CHANNEL)) =
					HalUsbDmaReadDmaControl(MAX_DMA_CHANNEL)&0xfe;
				HalUsbDrcResetClrRXMode1();

				USB_REG_WRITE16(M_REG_EP_BULKOUT,
					USB_REG_READ16(M_REG_EP_BULKOUT) & 0xfff0);
				USB_REG_WRITE16(M_REG_EP_BULKOUT,
					USB_REG_READ16(M_REG_EP_BULKOUT) | M_Mode1_P_BulkOut_EP);
				USB_REG_WRITE16(M_REG_DMA_MODE_CTL,
					USB_REG_READ16(M_REG_DMA_MODE_CTL) | M_Mode1_P_Enable);

				USB_REG_WRITE16(M_REG_DMA_MODE_CTL,
					USB_REG_READ16(M_REG_DMA_MODE_CTL) |
						(USB_BUFSIZE/ep_out->nEpMaxPacketSize));
				_USB_OTG_MSG("OTG REG:0x%X\r\n",
						USB_REG_READ16(M_REG_DMA_MODE_CTL));
 				/* USB_REG_WRITE8(M_REG_CFG8_L,
					USB_REG_READ8(M_REG_CFG8_L)|0x03); */
			}
		}
	}
}

void _HalUsbOtgChgState(u8 nToOTG, pUsbVar ptUsb)
{
	HalUsbDrcRefreshRegisters(ptUsb);
	ptUsb->nOtgReqState = nToOTG;

	switch(nToOTG) {
		case AB_IDLE:
			/* clear all pending or residue reqs */
			HalUsbDrcClearRegPower(0,ptUsb);
			HalUsbDrcClearRegDevctl(0,ptUsb);
			break;

		case A_PERIPHERAL:
		case B_PERIPHERAL:
			if (HalUsbDrcVBusStatus() < VBUS_ABOVE_AVALID)
				_HalUsbOtgChgState(AB_IDLE,ptUsb);

			HalUsbDrcReadRegPower(ptUsb);

			if (ptUsb->nOtgRegPower & M_POWER_HSMODE)
				ptUsb->nOtgSpeed = HIGH_SPEED;
			else
				ptUsb->nOtgSpeed = FULL_SPEED;
			break;

		default:
			break;
	}
}

void _HalUsbOtgResetAllEpVars(pUsbVar ptUsb)
{
	u8 i;
	Endpoint_t *ep;

	for (i = 0; i < otgNumEPDefs; i++) {
		ep = &ptUsb->tUsbEndpointInf[i];

		if DRCHOST(ptUsb->nOtgRegDevCtl)
			ep->nEpIoState = EP_TX;
		else
			ep->nEpIoState = EP_IDLE;
		ep->nEpFifoRemain = 0;
		ep->nEpBytesRequested = 0;
		ep->nEpBytesProcessed = 0;
		ep->nEpLastPacket = 0;
		ep->nEpHalted = 0;
	}
}
