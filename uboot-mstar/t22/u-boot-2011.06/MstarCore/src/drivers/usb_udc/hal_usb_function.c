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
#ifdef UFBL_FEATURE_IDME
#include <idme.h>
#endif

s32 HalUsbFunParseReceivedSetup(pUsbVar ptUsb);

/* FASTBOOT desc */
ALIGNED(4) u8 volatile USB_AndDev_Dscr[] =
{
	/* Standard Device Descriptor */
	DEVICE_DESC_LEN,
	DEVICE,
#if defined( USB_20 ) 
	0x00,0x02,			/* USB 2.0			*/
#else 
	0x10,0x01,			/* USB 1.1			*/
#endif
	0x00,				/* Vender define CLASS		*/
	0x00,				/* Vender define Subclass	*/
	0x00,				/* Protocol			*/
	0x40,				/* bMaxPktSize0			*/
	0x49,0x19,			/* idVendor 1949:Lab126,0BB4:HTC*/
	0xFF,0x0F,			/* idProduct			*/
	0x00,0x01,			/* bcdDevice			*/
	0x01,				/* iManufacturer		*/
	0x02,				/* iProduct			*/
	0x03,				/* iSerial Number		*/
	0x01,				/* One configuration		*/

	/* Standard Device Qualifier */
	0x0a,				/* bLength			*/
	0x06,				/* DEVICE Qualifier		*/
	0x00,0x02,			/* USB 2.0			*/
	0xff,				/* CLASS			*/
	0xff,				/* Subclass			*/
	0xff,				/* Protocol			*/
	0x40,				/* bMaxPktSize0			*/
	0x01,				/* One configuration		*/
	0x00,				/* Reserved			*/

	/* Standard Other Speed Configuration Descriptor */
	OTHERSPEEDCONFIG_DESC_LEN,	/* bLength			*/
	OTHERSPEEDCONFIG,		/* Other Speed Configuration	*/
	ANDDEV_TOTAL_OTHERSPEEDCONF_LEN_L, ANDDEV_TOTAL_OTHERSPEEDCONF_LEN_H,
	0x01,				/* bNumInterfaces		*/
	0x01,				/* bConfigurationValue		*/
	0x00,				/* iConfiguration		*/
	0xC0,				/* bmAttributes			*/
	0x32,				/* power: 100mA			*/

	/* Standard Interface Descriptor */
	INTRF_DESC_LEN,			/* Descriptor length		*/
	INTRF,				/* Interface descriptor		*/
	0x0,				/* bInterfaceNumber		*/
	0x0,				/* bAlternateSetting		*/
	0x02,				/* bNumEndpoints		*/
	FASTBOOT_INTERFACE_CLASS,	/* bInterfaceClass		*/
	FASTBOOT_INTERFACE_SUB_CLASS,	/* bInterfaceSubClass		*/
	FASTBOOT_INTERFACE_PROTOCOL,	/* bInterfaceProtocol		*/
	0x00,				/* iInterface			*/

	/* Standard Endpoint Descriptor: Bulk-In */
	ENDP_DESC_LEN,			/* bLength			*/
	ENDPT,				/* ENDPOINT			*/
	EPBULKIN | 0x80,		/* bEndpointAddress		*/
	PIPE_BULK,			/* Bulk-In			*/
	0x40, 0x00,			/* wMaxPacketSize: 64 bytes	*/
	0x00,				/* bInterval			*/

	/* Standard Endpoint Descriptor: Bulk-Out */
	ENDP_DESC_LEN,			/* bLength			*/
	ENDPT,				/* ENDPOINT			*/
	EPBULKOUT,			/* bEndpointAddress		*/
	PIPE_BULK,			/* Bulk- Out			*/
	0x40, 0x00,			/* wMaxPacketSize: 64 bytes	*/
	0x00,				/* bInterval			*/

	/* Standard Configuration Descriptor */
	CONFIG_DESC_LEN,		/* bLength			*/
	CONFIG,				/* CONFIGURATION		*/
	ANDDEV_TOTAL_CONF_LEN_L, ANDDEV_TOTAL_CONF_LEN_H,
	0x01,				/* bNumInterfaces		*/
	0x01,				/* bConfigurationValue		*/
	0x00,				/* iConfiguration		*/
	0x80,				/* bmAttributes			*/
	0x80,				/* power			*/

	/* Standard Interface Descriptor */
	INTRF_DESC_LEN,			/* Descriptor length		*/
	INTRF,				/* Interface descriptor		*/
	0x0,				/* bInterfaceNumber		*/
	0x0,				/* bAlternateSetting		*/
	0x02,				/* bNumEndpoints		*/
	FASTBOOT_INTERFACE_CLASS,	/* bInterfaceClass		*/
	FASTBOOT_INTERFACE_SUB_CLASS,	/* bInterfaceSubClass		*/
	FASTBOOT_INTERFACE_PROTOCOL,	/* bInterfaceProtocol		*/
	0x00,				/* iInterface			*/

	/* Standard Endpoint Descriptor: Bulk-In */
	ENDP_DESC_LEN,			/* bLength			*/
	ENDPT,				/* ENDPOINT			*/
	EPBULKIN | 0x80,		/* bEndpointAddress		*/
	PIPE_BULK,			/* Bulk-In			*/
	0x00, 0x02,			/* wMaxPacketSize: 512 bytes	*/
	0x00,				/* bInterval			*/

	/* Standard Endpoint Descriptor: Bulk-Out */
	ENDP_DESC_LEN,			/* bLength			*/
	ENDPT,				/* ENDPOINT			*/
	EPBULKOUT,			/* bEndpointAddress		*/
	PIPE_BULK,			/* Bulk- Out			*/
	0x00, 0x02,			/* wMaxPacketSize: 512 bytes	*/
	0x01,				/* bInterval			*/

	/* Standard String Descritor */
	/* LangID */
	0x04, 0x03, 0x09, 0x04,

	/* ManufacturerID */
	0x18, 0x03, 'M', 0x00, 's', 0x00, 't', 0x00,
	'a', 0x00, 'r', 0x00, 0x20, 0x00, 0x20, 0x00,
	0x20, 0x00, 'I', 0x00, 'n' , 0x00, 'c', 0x00,

	/* DevID */
	0x18, 0x03, 'A', 0x00, 'n', 0x00, 'd', 0x00, 'r',
	0x00, 'o', 0x00, 'i', 0x00, 'd', 0x00, ' ', 0x00,
	'1', 0x00, '.', 0x00, '0', 0x00,

#ifdef UFBL_FEATURE_IDME
	/* iserID */
	0x22, 0x03,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
#else
	/* iserID */
	0x1A, 0x03, 'H', 0x00, 'T', 0x00, '0', 0x00,
	'C', 0x00, 'E', 0x00, 'R', 0x00, 'X', 0x00,
	'2', 0x00, '2', 0x00, '7', 0x00, '5', 0x00, '8', 0x00
#endif
};

/**************************************************************
 * Local function definition                                  *
 **************************************************************/

void HalUsbFunEPInitial(pUsbVar ptUsb)
{
	if (ptUsb->nOtgSpeed==HIGH_SPEED) {
		HalUsbDrcSetupEP(EPCONTROL,	FIFO_DUAL,	PIPE_CONTROL,	0,	0x40,	ptUsb);
		HalUsbDrcSetupEP(EPBULKIN,	EP_TX,		PIPE_BULK,	0,	0x200,	ptUsb);
		HalUsbDrcSetupEP(EPBULKOUT,	EP_RX,		PIPE_BULK,	0,	0x200,	ptUsb);
	} else if (ptUsb->nOtgSpeed == FULL_SPEED) {
		HalUsbDrcSetupEP(EPCONTROL,	FIFO_DUAL,	PIPE_CONTROL,	0,	0x40,	ptUsb);
		HalUsbDrcSetupEP(EPBULKIN,	EP_TX,		PIPE_BULK,	0,	0x40,	ptUsb);
		HalUsbDrcSetupEP(EPBULKOUT,	EP_RX,		PIPE_BULK,	0,	0x40,	ptUsb);
	}
}

u8 HalUsbFunLoadFifo(s8 nEp, pUsbVar ptUsb)
{
	u8 nOutReg;
	Endpoint_t *ep = &ptUsb->tUsbEndpointInf[nEp];

	nOutReg = M_CSR0_TXPKTRDY;
	HalUsbDrcFifoWrite((u8 *)ep->nEpTransferBuffer, nEp, ptUsb);

	if (ep->nEpLastPacket) {
		nOutReg |= M_CSR0_P_DATAEND;
		ep->nEpIoState = EP_IDLE;
	}
	return nOutReg;
}

void HalUsbFunParseIntPeripheral(drcintitem_t *dP, pUsbVar ptUsb)
{
	s8 nEp;
	u8 parsestatus;		/* Setup parse result */
	u8 outreg;		/* Reset EP0 DRC regs with this */
	static u8 aTestPacket[53] = {
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
		0xAA, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE,
		0xEE, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0xBF, 0xDF,
		0xEF, 0xF7, 0xFB, 0xFD, 0xFC, 0x7E, 0xBF, 0xDF,
		0xEF, 0xF7, 0xFB, 0xFD, 0x7E
	};
	Endpoint_t *ep;

	outreg = 0;
	nEp = dP->IEP;
	ep = &ptUsb->tUsbEndpointInf[nEp];

	HalUsbDrcIndexSelect(nEp);	/* get our idx in window */

	do { /* while INDEX held ... */
		if (!nEp) { /* commence EP0 analysis */
			/* _USB_OTG_MSG("EP0=%x,%x \n\r",dP->IReg, ep->nEpIoState); */
			if (ptUsb->nUsb20TestMode) {
				if (ptUsb->nUsb20TestMode == M_FTR_TESTJ)
					USB_REG_WRITE8(M_REG_TESTMODE,M_TEST_J);

				if (ptUsb->nUsb20TestMode == M_FTR_TESTK)
					USB_REG_WRITE8(M_REG_TESTMODE,M_TEST_K);

				if (ptUsb->nUsb20TestMode == M_FTR_TESTSE0)
					USB_REG_WRITE8(M_REG_TESTMODE,M_TEST_SE0_NAK);

				if (ptUsb->nUsb20TestMode == M_FTR_TESTPKT) {
					u8 i;
					u32 nFifo;		/* FIFO Address */

					nFifo=FIFO_ADDRESS(ep->nEpBlt);

					for (i=0; i < 53; i++)
						*((u8 volatile *)nFifo) = aTestPacket[i];

					USB_REG_WRITE8(M_REG_CSR0, M_CSR0_INPKTRDY);
					USB_REG_WRITE8(M_REG_TESTMODE,M_TEST_PACKET);
				}
				ptUsb->nUsb20TestMode = 0;   /* Reset */
			}

			if (dP->IReg & M_CSR0_P_SENTSTALL) {
				ep->nEpIoState = EP_IDLE;
				USB_REG_WRITE8(M_REG_CSR0, 0);
			}

			if (dP->IReg & M_CSR0_P_SETUPEND) {
				outreg =  M_CSR0_P_SVDSETUPEND;
				ep->nEpIoState = EP_IDLE;
				USB_REG_WRITE8(M_REG_CSR0, outreg);
			}

			if (ep->nEpIoState == EP_IDLE) {
				if (dP->IReg & M_CSR0_RXPKTRDY) {
					outreg = M_CSR0_P_SVDRXPKTRDY;
					ep->nEpFifoRemain = dP->ICount;
					ep->nEpBytesRequested = 8;	/* Setup Data Size */
					ep->nEpBytesProcessed = 0;
					HalUsbDrcFifoRead((u8 *)&ptUsb->tEp0Setup, EPCONTROL,ptUsb);

					HalUsbUtilSwopSetup((struct _DevRequest_t *) &ptUsb->tEp0Setup);

					 _USB_OTG_MSG_L1("EP0:%02X%02X-I%04X-V%04X-%04x\n",					\
						 ptUsb->tEp0Setup.nRequestType, ptUsb->tEp0Setup.nRequest,	\
						 ptUsb->tEp0Setup.nValue, ptUsb->tEp0Setup.nIndex,			\
						 ptUsb->tEp0Setup.nLength);

					parsestatus = HalUsbFunParseReceivedSetup(ptUsb);
					/* _USB_OTG_MSG("HalUsbFunParseReceivedSetup Sts: 0x%x\n", parsestatus); */
					USB_REG_WRITE8(M_REG_INDEX, 0);

					if (parsestatus == USB_PID_ACK) {
						outreg |= M_CSR0_P_DATAEND;

						if ((!ptUsb->nStdReqSetAddr) && (ptUsb->nStdReqFuncAddr)
								&& (ptUsb->nOtgUsbState < USB_ADDRESS)) {
							HalUsbUtilChgUSBState(USB_ADDRESS,ptUsb);
							ptUsb->nStdReqSetAddr = 1;
						}
						/* _USB_OTG_MSG("USB_PID_ACK \n\r"); */
					} else if (parsestatus == USB_PID_OUT) {
						USB_REG_WRITE8(M_REG_CSR0, outreg);
						outreg = 0;
						ep->nEpIoState = EP_RX;
						ep->nEpBytesProcessed = 0;
						ep->nEpBytesRequested = ptUsb->tEp0Setup.nLength;
						/* _USB_OTG_MSG("USB_PID_OUT \n\r"); */
						break;	/* Benson add */
					} else if (parsestatus == USB_PID_IN) {
						USB_REG_WRITE8(M_REG_CSR0, outreg);
						outreg = 0;
						ep->nEpIoState = EP_TX;
						ep->nEpLastPacket = 0;
						ep->nEpBytesProcessed = 0;
						ep->nEpBytesRequested = ep->nEpTransferBuffer_length;
						/* _USB_OTG_MSG("USB_PID_IN \n\r"); */
					} else {
						outreg = M_CSR0_P_SENDSTALL | M_CSR0_P_SVDRXPKTRDY;
					}
				} else {
					/* [Trick] Set New address When the incoming of next "SETUP"
					 * after Set_Address_Request */
					if (ptUsb->nStdReqSetAddr) {
						USB_REG_WRITE8(M_REG_FADDR, ptUsb->nStdReqFuncAddr);
						_USB_OTG_MSG_L1("Set Address %d\n", ptUsb->nStdReqFuncAddr);
					}

					ptUsb->nStdReqSetAddr = 0;

					if (ptUsb->nStdReqCleareEndpoint) {
						ptUsb->nStdReqCleareEndpoint = 0;
					}
				}
			}

			if (ep->nEpIoState == EP_TX) {
		             outreg = HalUsbFunLoadFifo(0,ptUsb);
			}

			if (ep->nEpIoState == EP_RX) { /* wayne added */
				if (dP->IReg & M_CSR0_RXPKTRDY) {
					ep->nEpFifoRemain = dP->ICount;
					HalUsbDrcFifoRead((u8 *)ep->nEpTransferBuffer, EPCONTROL, ptUsb);
					outreg = 0;

					if (ep->nEpBytesProcessed >= ep->nEpBytesRequested) {
						outreg |= M_CSR0_P_DATAEND;
						ep->nEpIoState = EP_IDLE;
						USB_REG_WRITE8(M_REG_INDEX, 0);
						USB_REG_WRITE8(M_REG_CSR0, outreg);
					}

					outreg = M_CSR0_P_SVDRXPKTRDY;
					USB_REG_WRITE8(M_REG_CSR0, outreg);
					outreg = 0;
					break;
				}
			}

			if (outreg) {
				/* _USB_OTG_MSG("EPREG=%x \n\r",outreg); */
				USB_REG_WRITE8(M_REG_CSR0, outreg);

				if (ptUsb->nOtgHnpEnable) {
					/* _USB_OTG_MSG("EPHNP=%x \n\r",outreg); */
					HalUsbDrcSetRegDevctl((M_DEVCTL_HR + M_DEVCTL_SESSION),ptUsb);
				}
			}
			break;
		} else { /* for Other En Interrupt*/
			if (dP->IIntSrc == EP_RX) { /* RX Interrupt */
				if ((dP->IReg & M_RXCSR1_P_SENTSTALL)||!(dP->IReg & M_RXCSR1_RXPKTRDY)) {
					ptUsb->nOtgInterruptStatus = USB_ST_STALL;
					outreg = 0;
					USB_REG_WRITE8(M_REG_RXCSR1, outreg);
					break;
				} else if (ep->nEpHalted) {
					outreg = M_RXCSR1_P_SENDSTALL;
					USB_REG_WRITE8(M_REG_RXCSR1, outreg);
					break;
				} else { /* normal condition */
					_USB_OTG_MSG("Recive: 0x%08X, 0x%08X\n", dP->ICount);
					ep->nEpFifoRemain = dP->ICount;
					break;
				}
			} else { /* TX Interrupt */
				if (dP->IReg & M_TXCSR1_P_SENTSTALL) {
					ptUsb->nOtgInterruptStatus = USB_ST_STALL;
					USB_REG_WRITE8(M_REG_TXCSR1, 0);
					return;
				}

				if (ep->nEpLastPacket) {
					ptUsb->nOtgInterruptStatus = USB_ST_NOERROR;
					break;
				}

				if (ep->nEpHalted) {
					USB_REG_WRITE8(M_REG_TXCSR1, M_TXCSR1_P_SENDSTALL);
					break;
				}
				/* Trnsmit next remaining packet continuously if Any */
				HalUsbDrcFifoWrite((u8 *)ep->nEpTransferBuffer,nEp,ptUsb);
				USB_REG_WRITE8(M_REG_TXCSR1, M_TXCSR1_TXPKTRDY);
			}
		}
	} while(0);
	return;
}

s32 HalUsbFunParseReceivedSetup(pUsbVar ptUsb)
{
	s32 retstatus;
	s32 bytesrem;
	s32 length;
	u8 *scanP;
	u8 i, descindex;
	u8 desctype;
	s32 nEp;
	nEpPipe_t nEpPipe_l;
	void *sp;
	u16 l,k;

	Endpoint_t *ep_ctrl= &ptUsb->tUsbEndpointInf[EPCONTROL];

	retstatus = USB_PID_STALL;

	if ((ptUsb->tEp0Setup.nRequestType &
		USB_TYPE_MASK) != USB_TYPE_STANDARD) {
		if ((ptUsb->tEp0Setup.nRequestType &
			USB_TYPE_MASK) == USB_TYPE_CLASS) {
			_USB_OTG_MSG("[TY] Get Class Control\n");
		}
		return(retstatus);
	}

	/* Standard Request Type */
	switch (ptUsb->tEp0Setup.nRequest) {
		case	USB_REQ_GET_STATUS:

			ptUsb->nStdReqGetStatusResponse = 0;
			ep_ctrl->nEpTransferBuffer_length=ptUsb->tEp0Setup.nLength;
			ep_ctrl->nEpTransferBuffer=(u32)&ptUsb->nStdReqGetStatusResponse;

			if ((ptUsb->tEp0Setup.nRequestType &
				USB_RECIP_MASK) == USB_RECIP_DEVICE) {
				if (ptUsb->nOtgRemoteWakeup)
					ptUsb->nStdReqGetStatusResponse = 0x02;

				if (ptUsb->nOtgSelfPower)
					ptUsb->nStdReqGetStatusResponse |= 0x01;

				retstatus = USB_PID_IN;
				break;
			}

			if ((ptUsb->tEp0Setup.nRequestType &
				USB_RECIP_MASK) == USB_RECIP_INTERFACE) {
				retstatus = USB_PID_IN;
				break;
			}

			if ((ptUsb->tEp0Setup.nRequestType &
					USB_RECIP_MASK) == USB_RECIP_ENDPOINT) {
				nEpPipe_l = GETLSB(ptUsb->tEp0Setup.nIndex) & 0xff;
				nEp = usb_nEpPipeendpoint(nEpPipe_l);

				if(nEp == 0)
					break;

				if  (ptUsb->tUsbEndpointInf[nEp].nEpHalted)
					ptUsb->nStdReqGetStatusResponse = 0x01;

				retstatus = USB_PID_IN;
				break;
			}
			break;

		case	USB_REQ_CLEAR_FEATURE:
			if (ptUsb->nOtgUsbState < USB_ADDRESS)
				break;	/* STALL */

			if ((ptUsb->tEp0Setup.nRequestType &
					USB_RECIP_MASK) == USB_RECIP_DEVICE) {
				if (GETLSB(ptUsb->tEp0Setup.nValue) == DEVICE_REMOTE_WAKEUP) {
					ptUsb->nOtgRemoteWakeup = 0;
					retstatus = USB_PID_ACK;
				}
			} else if ((ptUsb->tEp0Setup.nRequestType &
					USB_RECIP_MASK) == USB_RECIP_INTERFACE) {
				if  (ptUsb->nOtgUsbState != USB_CONFIGURED )
					break;	/* STALL */

				retstatus = USB_PID_ACK;
			} else if ((ptUsb->tEp0Setup.nRequestType &
					USB_RECIP_MASK) == USB_RECIP_ENDPOINT) {
				if ((ptUsb->nOtgUsbState == USB_ADDRESS) &&
						(GETLSB(ptUsb->tEp0Setup.nIndex) != 0))
					break;	/* STALL */

				if (GETLSB(ptUsb->tEp0Setup.nValue) == ENDPOINT_HALT) {
					nEpPipe_l = GETLSB(ptUsb->tEp0Setup.nIndex) & 0xff;
					nEp = usb_nEpPipeendpoint(nEpPipe_l);

					if (nEp) {
						ptUsb->tUsbEndpointInf[nEp].nEpHalted = 0;
						HalUsbDrcIndexSelect(ptUsb->tUsbEndpointInf[nEp].nEpBlt);

						if (ptUsb->tUsbEndpointInf[nEp].nEpDrcDir) {
							USB_REG_WRITE8(M_REG_RXCSR1, M_RXCSR_CLRDATATOG);
							USB_REG_WRITE8(M_REG_RXCSR1, 0);
						} else {
							USB_REG_WRITE8(M_REG_TXCSR1, M_TXCSR_CLRDATATOG);
							USB_REG_WRITE8(M_REG_TXCSR1, 0);
						}

						HalUsbDrcIndexSelect(0);	/* Back to EP0. */
						ptUsb->nStdReqCleareEndpoint=1;
					}
					retstatus = USB_PID_ACK;
				}
			}
			break;

		case	USB_REQ_SET_FEATURE:
			if (ptUsb->nOtgUsbState < USB_DEFAULT)
				break;

			if ((ptUsb->tEp0Setup.nRequestType &
					USB_RECIP_MASK) == USB_RECIP_DEVICE) {
				if (GETLSB(ptUsb->tEp0Setup.nValue) == DEVICE_REMOTE_WAKEUP) {
					ptUsb->nOtgRemoteWakeup = 1;
					retstatus = USB_PID_ACK;
				} else if (GETLSB(ptUsb->tEp0Setup.nValue) == USB20_TEST_MODE) {
					ptUsb->nUsb20TestMode = ptUsb->tEp0Setup.nIndex;
				} else if (GETLSB(ptUsb->tEp0Setup.nValue) == b_hnp_enable) {
					if (ptUsb->nStdReqDeviceHnpSupport)
						ptUsb->nOtgHnpEnable = 1;	/* we can become host now */
					else
						break;
				} else if (GETLSB(ptUsb->tEp0Setup.nValue) == a_hnp_support) {
					if (ptUsb->nStdReqDeviceHnpSupport)
						ptUsb->nOtgHnpSupport = 1;
					else
						break;
				} else {
					retstatus = USB_PID_STALL;
					break;
				}
				retstatus = USB_PID_ACK;
				break;
			}

			if ((ptUsb->tEp0Setup.nRequestType &
					USB_RECIP_MASK) == USB_RECIP_INTERFACE) {
				if (ptUsb->nOtgUsbState != USB_CONFIGURED )
					break;

				if ((GETLSB(ptUsb->tEp0Setup.nValue) == b_hnp_enable) ||
					(GETLSB(ptUsb->tEp0Setup.nValue) == a_hnp_support) ||
					(GETLSB(ptUsb->tEp0Setup.nValue) == a_alt_hnp_support))
					retstatus = USB_PID_ACK;

				break;
			}

			if ((ptUsb->tEp0Setup.nRequestType &
					USB_RECIP_MASK) == USB_RECIP_ENDPOINT)
			{
				if ((ptUsb->nOtgUsbState == USB_ADDRESS) &&
						(GETLSB(ptUsb->tEp0Setup.nIndex) != 0))
					break;

				if (GETLSB(ptUsb->tEp0Setup.nValue) == ENDPOINT_HALT) {
					nEpPipe_l = GETLSB(ptUsb->tEp0Setup.nIndex) & 0xff;
					nEp = usb_nEpPipeendpoint(nEpPipe_l);

					if(nEp) {
						ptUsb->tUsbEndpointInf[nEp].nEpHalted = 1;
						HalUsbDrcIndexSelect(ptUsb->tUsbEndpointInf[nEp].nEpBlt);

						if (ptUsb->tUsbEndpointInf[nEp].nEpDrcDir)
							USB_REG_WRITE8(M_REG_RXCSR1, M_RXCSR1_P_SENDSTALL);
						else
							USB_REG_WRITE8(M_REG_TXCSR1, M_TXCSR1_P_SENDSTALL);

						HalUsbDrcIndexSelect(0);
					}
					retstatus = USB_PID_ACK;
					break;
				}
			}
			break;

		case    USB_REQ_SET_ADDRESS:

			if(GETLSB(ptUsb->tEp0Setup.nValue) <= 127) {
				ptUsb->nStdReqFuncAddr = GETLSB(ptUsb->tEp0Setup.nValue);

				if ((ptUsb->nOtgUsbState == USB_DEFAULT) &&
							(ptUsb->nStdReqFuncAddr != 0))
					ptUsb->nStdReqSetAddr = 0;

				retstatus = USB_PID_ACK;
			}
			break;

		case	USB_REQ_GET_DESCRIPTOR:

			desctype = GETMSB(ptUsb->tEp0Setup.nValue);		
		  
			if (!((desctype == USB_DT_DEVICE) || (desctype == USB_DT_CONFIG)||
				(desctype == USB_DT_STRING)||(desctype == USB_DT_DEVICE_QUALIFIER) ||
				(desctype == USB_DT_OTHER_SPEED)))
				return(retstatus);	/* STALL if not valid request */

			/* must to modify for spliting fill Dev DESC */
			scanP = (u8 *)USB_AndDev_Dscr;
			bytesrem = (u32)sizeof(USB_AndDev_Dscr);

			/* ptUsb->nOtgSpeed depends on ptUsb->nOtgRegPower set
			 * when HalUsbOtgParseIntrUsb */
			if (ptUsb->nOtgSpeed == HIGH_SPEED) {
				*(scanP+82) = 0x00;	/* Bulk-In	mMaxPacketSize: 512 bytes */
				*(scanP+83) = 0x02;
				*(scanP+89) = 0x00;	/* Bulk-Out	mMaxPacketSize: 512 bytes */
				*(scanP+90) = 0x02;
			} else {
				*(scanP+82)=0x40;	/* Bulk-In	mMaxPacketSize: 64 bytes */
				*(scanP+83)=0x0;
				*(scanP+89)=0x40;	/* Bulk-Out	mMaxPacketSize: 64 bytes */
				*(scanP+90)=0x0;
			}

			if (ptUsb->nUsbSpVendor == 1) {
				/* l=98; //manufactor */
				l = 96;
				for(k = 0; k < USB_VENDOR_NAME_SIZE; k++) {
					if (ptUsb->Usb_Vendor_Name[k]==0x00)
						ptUsb->Usb_Vendor_Name[k]=0x20;

					*(scanP+l)=ptUsb->Usb_Vendor_Name[k];
					l = l + 2;
				}
			}

			if  ((!scanP) || (bytesrem <=0))
				return(retstatus);

#ifdef UFBL_FEATURE_IDME
			{
				char serialno[18] = {0};
				idme_get_var_external("serial", serialno, sizeof(serialno)-1);
				/* _USB_OTG_MSG_L1("serialno:%s \n\r",serialno); */
				l = 146; /* iserID + 2*/
				for(k = 0; k < sizeof(serialno)-2; k++) {
					*(scanP+l) = serialno[k];
					l = l + 2;
				}
			}
#endif
			/* !=0 for string,config */
			descindex = GETLSB(ptUsb->tEp0Setup.nValue);
			i = descindex;

			while (bytesrem > 0) {
				if (*(scanP+1) == desctype) {
					if (i == 0)
						break;
					i--;
				}
				bytesrem -= *scanP;
				scanP += *scanP;
			}

			if (bytesrem <= 0)
				return(retstatus);

			if ((desctype == USB_DT_CONFIG) ||
					(desctype == USB_DT_OTHER_SPEED)) {
				length = (*(scanP+3) << 8) + *(scanP+2);
			}
			else
				length = *scanP;

			ep_ctrl->nEpTransferBuffer_length=MIN(ptUsb->tEp0Setup.nLength,length);
			ep_ctrl->nEpTransferBuffer=(u32)scanP;

			/* _USB_OTG_MSG("scanP:0x%x \n\r",*scanP); */
			retstatus = USB_PID_IN;
			break;

		case	USB_REQ_SET_DESCRIPTOR:
			sp=(void*)ptUsb->nStdReqSetupCmdBuf;

			ep_ctrl->nEpTransferBuffer_length=ptUsb->tEp0Setup.nLength;
			ep_ctrl->nEpTransferBuffer=(u32)sp;

			retstatus = USB_PID_OUT;
			break;

		case	USB_REQ_GET_CONFIGURATION:
			ep_ctrl->nEpTransferBuffer_length=ptUsb->tEp0Setup.nLength;
			ep_ctrl->nEpTransferBuffer=(u32)&ptUsb->nStdReqConfig;

			retstatus = USB_PID_IN;
			break;

		case	USB_REQ_SET_CONFIGURATION:
			if (ptUsb->nOtgUsbState < USB_ADDRESS)
				break;

			if (!GETLSB(ptUsb->tEp0Setup.nValue)) {
				HalUsbUtilChgUSBState(USB_ADDRESS,ptUsb);
				retstatus = USB_PID_ACK;
				break;
			}

			if (GETLSB(ptUsb->tEp0Setup.nValue)) {
				ptUsb->nStdReqConfig = GETLSB(ptUsb->tEp0Setup.nValue);
				HalUsbUtilChgUSBState(USB_CONFIGURED,ptUsb);
				/* reset all EP when set configuration/interface */
				HalUsbFunEPInitial(ptUsb);

				retstatus = USB_PID_ACK;
				break;
			}
			
			break;

		case	USB_REQ_GET_INTERFACE:

			if (ptUsb->nOtgUsbState != USB_CONFIGURED)
				break;

			ep_ctrl->nEpTransferBuffer_length = ptUsb->tEp0Setup.nLength;
			ep_ctrl->nEpTransferBuffer = (u32)&ptUsb->nStdReqInterface;
			retstatus = USB_PID_IN;
			break;

		case	USB_REQ_SET_INTERFACE:

			if (ptUsb->nOtgUsbState != USB_CONFIGURED)
				break;
			/* reset all EP when set configuration/interface */
			HalUsbFunEPInitial(ptUsb);
			retstatus = USB_PID_ACK;
			break;

		case	USB_REQ_SYNCH_FRAME:
			break;

		default:
			break;
	}

	return(retstatus);
}
