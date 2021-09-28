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

u8 OTG_Interrupt_Polling(u8 vec, pUsbVar ptUsb)
{
	drcintitem_t d;
	u8 nIndexSave;
	u8 nEpnum;
	u16 nUsbIntr = 0, nReg;
	u16 i;
	u16 aIntReg[2] = { 0, 0 };
#ifdef DMA
	u8 nDmaIntr;
#endif
	u8 bHandle = 0;
	/* we have local copy */
	nIndexSave = USB_REG_READ8(M_REG_INDEX);
#ifdef DMA
	/* DMA Interrupt */
	if (vec == 0xff || vec == 0x0) {
		/*_USB_OTG_MSG("Polling DMA INT ... %x\n",
				USB_REG_READ8(M_REG_DMA_INTR));*/
		nDmaIntr = USB_REG_READ8(M_REG_DMA_INTR);
		USB_REG_WRITE8(M_REG_DMA_INTR, nDmaIntr);

		if (nDmaIntr) {
			_USB_OTG_MSG("do DMA INT nDmaIntr: %X\n", nDmaIntr);
			for (i = 0; i < MAX_DMA_CHANNEL; i++) {
				if (nDmaIntr & (1 << i)) {
					d.IEP = i + 1;
					d.IDMA = 1;
					/* _USB_OTG_MSG("DMA int:%x\n\r",d.IEP); */
					HalUsbDmaIrqHandler(d.IEP, ptUsb);
					d.IDMA = 0;
					d.IEP = 0;
				}
			}
			_USB_OTG_MSG(".. done DMA INT\n");
			bHandle = 0x10;
		}
	}
#endif	/* DMA */
	/* USB Interrupt */
	if (vec == 0xff || vec == 0x1) {	
		/* _USB_OTG_MSG("Polling USB INT ... \n"); */
		nUsbIntr = USB_REG_READ8(M_REG_INTRUSB);
		USB_REG_WRITE8(M_REG_INTRUSB, nUsbIntr);

		if (nUsbIntr) {
			_USB_OTG_MSG("do USB INT: 0x%X\n", nUsbIntr);
			d.IReg = nUsbIntr;
			d.ICause = IRC_INTRUSB;
			HalUsbDrcParseInt(&d, ptUsb);
			_USB_OTG_MSG(".. done USB INT\n");
			bHandle = 0x11;
		}
	}

	if(vec == 0xff || vec == 0x20 || vec == 0x21) {
		aIntReg[0] = USB_REG_READ16(M_REG_INTRTX);
		USB_REG_WRITE16(M_REG_INTRTX, aIntReg[0]);
	}

	if(vec == 0xff || vec == 0x22) {
		aIntReg[1] = USB_REG_READ16(M_REG_INTRRX);
		USB_REG_WRITE16(M_REG_INTRRX, aIntReg[1]);
	}

	/* EP0 Interrupt */
	if (vec == 0xff || vec == 0x20) {
		/* _USB_OTG_MSG("Polling EP0 INT aIntReg[0]: %X\n", aIntReg[0]); */
		if (aIntReg[0] & 0x0001) {
			_USB_OTG_MSG("do EP0 INT\n");
			USB_REG_WRITE8(M_REG_INDEX, 0);
			nReg = USB_REG_READ8(M_REG_CSR0);
			d.IIntSrc = EP_TX;
			d.IEP = 0;
			d.IReg = nReg;
			d.ICause = IRC_EP;

			if (nReg & M_CSR0_RXPKTRDY) {
				d.ICount = USB_REG_READ16(M_REG_RXCOUNT);
			}

			HalUsbDrcParseInt(&d, ptUsb);
			aIntReg[0] &= ~0x0001;
			bHandle = 0x20;
			_USB_OTG_MSG(".. done EP0 INT\n");
		}
	}
	
	/* EPx TX Interrupt */
	if (vec == 0xff || vec == 0x21) {
		//_USB_OTG_MSG("Polling EPx TX INTR aIntReg[0]: %X\n", aIntReg[0]);
		nEpnum = 1;
		i = 2;
		while (aIntReg[0] & ~0x0001) {
			if (aIntReg[0] & i) {
				_USB_OTG_MSG("do EPx TX INT intreg[0]: 0x%X\n", aIntReg[0]);
				USB_REG_WRITE8(M_REG_INDEX, nEpnum);
				d.IReg = USB_REG_READ8(M_REG_TXCSR1);
				d.IIntSrc = EP_TX;
				d.IEP = nEpnum;
				d.ICause = IRC_EP;
				HalUsbDrcParseInt(&d, ptUsb);
				aIntReg[0] &= ~i;
				_USB_OTG_MSG(".. done EPx TX INT\n");
				bHandle = 0x21;
			}
			nEpnum++;
			i = i << 1;
		}
	}

	/* EPx RX Interrupt */
	if (vec == 0xff || vec == 0x22) {	
		/*_USB_OTG_MSG("Polling EPx RX INT aIntReg[1]: %X\n", aIntReg[1]);*/
		nEpnum = 1;
		i = 2;

		while (aIntReg[1]) {
			if (aIntReg[1] & i) {
				_USB_OTG_MSG("do EPx RX INT intreg[1]: 0x%X\n", aIntReg[1]);

				USB_REG_WRITE8(M_REG_INDEX, nEpnum);
				d.IReg = USB_REG_READ8(M_REG_RXCSR1);
				d.IIntSrc = EP_RX;
				d.IEP = nEpnum;
				d.ICause = IRC_EP;
				d.ICount = USB_REG_READ16(M_REG_RXCOUNT);
				HalUsbDrcParseInt(&d, ptUsb);
				aIntReg[1] &= ~i;

				_USB_OTG_MSG("done EPx RX INT\n");
				bHandle = 0x22;
			}
			nEpnum++;
			i = i << 1;
		}
	}

	USB_REG_WRITE8(M_REG_INDEX, nIndexSave);

	if(bHandle)
		_USB_OTG_MSG(".... done IRQ handle %x\n", bHandle);

	/* tony if 0 no handled */
	return bHandle;
}

void HalUsbDrcDisableUSB(void)
{
	_USB_OTG_MSG("[TY] HalUsbDrcDisableUSB null func\n");
}

void HalUsbDrcReadRegPower(pUsbVar ptUsb)
{
	ptUsb->nOtgRegPower = USB_REG_READ8(M_REG_POWER);
}

void HalUsbDrcReadRegDevctl(pUsbVar ptUsb)
{
	u8 reg;

	/* store index */
	reg = USB_REG_READ8(M_REG_INDEX);
	USB_REG_WRITE8(M_REG_INDEX, 0);
	ptUsb->nOtgRegDevCtl = USB_REG_READ8(M_REG_DEVCTL);
	/* restore index */
	USB_REG_WRITE8(M_REG_INDEX, reg);
}

void HalUsbDrcRefreshRegisters(pUsbVar ptUsb)
{
	HalUsbDrcReadRegPower(ptUsb);
	HalUsbDrcReadRegDevctl(ptUsb);
}

void HalUsbDrcSetRegDevctl(u8 regupdate, pUsbVar ptUsb)
{
	ptUsb->nOtgRegDevCtl |= regupdate;
	USB_REG_WRITE8(M_REG_DEVCTL, ptUsb->nOtgRegDevCtl);
	ptUsb->nOtgRegDevCtl = USB_REG_READ8(M_REG_DEVCTL);
}

void HalUsbDrcClearRegDevctl(u8 regupdate, pUsbVar ptUsb)
{
	if (!regupdate)
		ptUsb->nOtgRegDevCtl = 0;
	else
		ptUsb->nOtgRegDevCtl &= ~regupdate;

	USB_REG_WRITE8(M_REG_DEVCTL, ptUsb->nOtgRegDevCtl);
	ptUsb->nOtgRegDevCtl = USB_REG_READ8(M_REG_DEVCTL);
}

void HalUsbDrcIndexSelect(u8 epnum)
{
	u8 reg;

	reg = USB_REG_READ8(M_REG_INDEX);
	if (reg != epnum)
		USB_REG_WRITE8(M_REG_INDEX, epnum);
}

void HalUsbDrcEnableEP(u8 nEp, pUsbVar ptUsb)
{
	u8 reg;
	u16 csr;

	Endpoint_t *ep = &ptUsb->tUsbEndpointInf[nEp];

	HalUsbDrcIndexSelect(ep->nEpBlt);
	reg =(ep->nEpBlt & 0x0f) | ((ep->nEpPipe & 0x03) << 4);

	csr = USB_REG_READ16(M_REG_RXCSR);

	if (ep->nEpDrcDir == FIFO_RX) {
		USB_REG_WRITE8(M_REG_RXTYPE, reg);
		USB_REG_WRITE16(M_REG_RXMAXP, ep->nEpMaxPacketSize);

		if (csr & M_RXCSR_RXPKTRDY) {
			ep->nEpFifoRemain = USB_Read_RxCount();
		} else {
			csr = M_RXCSR_CLRDATATOG;
		}

		if (ep->nEpPipe == USB_ENDPOINT_XFER_INT)
			csr |= M_RXCSR_DISNYET;

		USB_REG_WRITE16(M_REG_RXCSR, csr);
		USB_REG_WRITE16(M_REG_TXCSR, 0);
	} else {
		USB_REG_WRITE16(M_REG_TXMAXP, ep->nEpMaxPacketSize);
		USB_REG_WRITE16(M_REG_TXCSR, M_TXCSR_FLUSHFIFO);
		if (ep->nEpPipe == PIPE_ISOCHRONOUS)
			csr = M_TXCSR_ISO | M_TXCSR_MODE | M_TXCSR_CLRDATATOG;
		else
			csr = M_TXCSR_MODE | M_TXCSR_CLRDATATOG;
		USB_REG_WRITE16(M_REG_TXCSR, csr);
	}
}

void HalUsbDrcSetupEP(s8 nEp, s8 nDir, s8 nType, s8 nInf, s16 nMaxps, pUsbVar ptUsb)
{
	Endpoint_t *ep = &ptUsb->tUsbEndpointInf[nEp];

	ep->nEpMaxPacketSize = nMaxps;
	ep->nEpDrcDir = nDir;
	ep->nEpFifoRemain = 0;
	ep->nEpBytesRequested = 0;
	ep->nEpBytesProcessed = 0;
	ep->nEpPipe = nType;
	ep->nEpBlt = nEp;
	ep->nEpLastPacket = 0;
	ep->nEpTransferBuffer = 0;
	ep->nEpTransferBuffer_length = 0;
	ep->nEpIoState = EP_IDLE;
	ep->nEpHalted = 0;
	ep->nEpInfNum = nInf;

	if (nEp != 0)
		HalUsbDrcEnableEP(nEp, ptUsb);
}

void HalUsbDrcFifoRead(u8 * pDst, u8 nEp, pUsbVar ptUsb)
{
	s32 nReadCount;
	u32 nFifo;

	Endpoint_t *ep = &ptUsb->tUsbEndpointInf[nEp];

	nReadCount = MIN((ep->nEpBytesRequested -
		ep->nEpBytesProcessed), ep->nEpFifoRemain);

	if (nReadCount <= 0)
		return;

	pDst += ep->nEpBytesProcessed;
	ep->nEpBytesProcessed += nReadCount;
	ep->nEpFifoRemain -= nReadCount;
	nFifo = FIFO_ADDRESS(ep->nEpBlt);

	_USB_OTG_MSG("HalUsbDrcFifoRead ==> nFifo 0x%08X - nReadCount %d\n", nFifo, nReadCount);

	if ((nReadCount > 0) && ((u32) pDst & 1)) {
		while (nReadCount > 0) {
			_USB_OTG_MSG("## Ouch, buf addr is strang - %p\n", (u32) pDst);
			*((u8 volatile *) pDst) = *((u8 volatile *) nFifo);
			pDst++;
			nReadCount--;
		}
	} else {
		while ((nReadCount >= 2) && !((u32) pDst & 1)) {
			*((u16 volatile *) pDst) = *((u16 volatile *) nFifo);
			pDst = pDst + 2;
			nReadCount = nReadCount - 2;
		}
 
		while (nReadCount > 0) {
			*((u8 volatile *) pDst) = *((u8 volatile *) nFifo);
			pDst++;
			nReadCount--;
		}
	}
}

void HalUsbDrcFifoWrite(u8 * pSrc, u8 nEp, pUsbVar ptUsb)
{
	s32 nWriteCount;
	u32 nFifo;

	Endpoint_t *ep = &ptUsb->tUsbEndpointInf[nEp];

	if (ep->nEpMaxPacketSize != 0)
		nWriteCount = MIN(ep->nEpMaxPacketSize,
			(ep->nEpBytesRequested - ep->nEpBytesProcessed));
	else
		nWriteCount = ep->nEpBytesRequested - ep->nEpBytesProcessed;

	if (!nWriteCount) {
		ep->nEpLastPacket = 1;
		return;
	}

	pSrc += ep->nEpBytesProcessed;
	ep->nEpBytesProcessed += nWriteCount;

	if (ep->nEpBytesProcessed == ep->nEpBytesRequested)
		ep->nEpLastPacket = 1;

	nFifo = FIFO_ADDRESS(ep->nEpBlt);

	_USB_OTG_MSG("Write FIFO: 0x%X -- Addr: 0x%X\n", nEp, nFifo);
	_USB_OTG_MSG("nWriteCount: 0x%X\n", nWriteCount);

	/* byte access for unaligned */
	if ((nWriteCount > 0) && ((u32) pSrc & 1)) {
		while (nWriteCount) {
			_USB_OTG_MSG("## Ouch, buf addr is strang - 0x%08X\n", (u32)pSrc);
			*((u8 volatile *) nFifo) = *((u8 volatile *) pSrc);
			pSrc++;
			nWriteCount--;
		}
	} else {/* word access if aligned */
		while ((nWriteCount >= 2) && !((u32) pSrc & 1)) {
			*((u16 volatile *) nFifo) = *((u16 volatile *) pSrc);
			pSrc = pSrc + 2;
			nWriteCount = nWriteCount - 2;
		}
		while (nWriteCount > 0) {
			*((u8 volatile *) nFifo) = *((u8 volatile *) pSrc);
			pSrc++;
			nWriteCount--;
		}
	}
}

u8 HalUsbDrcMiu2UsbBulk(u32 nTxAddr, u32 nTxSize, pUsbVar ptUsb)
{
	u8 nInReg, nOutReg = 0;

	Endpoint_t *ep = &ptUsb->tUsbEndpointInf[EPBULKIN];

	ptUsb->nOtgInterruptStatus = 0xff;
	/* _USB_OTG_MSG("[Info] txsize:%x\n\r", txsize ); */
	/* ep->nEpIoState = EP_TX; */
	ep->nEpBytesRequested = nTxSize;
	ep->nEpBytesProcessed = 0;
	ep->nEpLastPacket = 0;
	ep->nEpTransferBuffer = nTxAddr;
	ptUsb->nOtgDataPhaseDir = usb_sndbulknEpPipe(ep->nEpBlt);

	HalUsbDrcIndexSelect(ep->nEpBlt);

	nInReg = USB_REG_READ8(M_REG_TXCSR);

	if (nInReg & M_TXCSR1_P_SENTSTALL)
		nOutReg |= M_TXCSR1_CLRDATATOG;

	if (!(nInReg & M_TXCSR1_FIFONOTEMPTY) || (nOutReg & M_TXCSR1_CLRDATATOG)) {
		HalUsbDrcFifoWrite((u8 *) ep->nEpTransferBuffer, EPBULKIN, ptUsb);
		nOutReg |= M_TXCSR1_TXPKTRDY;
	}

	if (nOutReg) {
		USB_REG_WRITE8(M_REG_TXCSR1, nOutReg);
	}

	_USB_OTG_MSG("Start polling EPx TX interrupt. vec: 0x%X \n\r", 0x21);
	while (!OTG_Interrupt_Polling(0x21, ptUsb)) ;
	_USB_OTG_MSG("End polling interrupt. vec: 0x%X \n\r", 0x21);

	ep->nEpBytesRequested = 0;
	return USB_ST_NOERROR;
}

u8 HalUsbDrcMiu2UsbBulkDMA(u32 nTxAddr, u32 nTxSize, pUsbVar ptUsb)
{
	u8 nInReg, nOutReg = 0;

	Endpoint_t *ep = &ptUsb->tUsbEndpointInf[EPBULKIN];

	ptUsb->nOtgDmaTxInterruptStatus = 0xff;
	/* _USB_OTG_MSG("txsize:%x\n\r", nTxSize ); */
	/* ep->nEpIoState = EP_TX; */
	ep->nEpBytesRequested = nTxSize;
	ep->nEpBytesProcessed = 0;
	ep->nEpLastPacket = 0;
	ep->nEpTransferBuffer = nTxAddr;
	ptUsb->nOtgDataPhaseDir = usb_sndbulknEpPipe(ep->nEpBlt);

	HalUsbDrcIndexSelect(ep->nEpBlt);
	nInReg = USB_REG_READ8(M_REG_TXCSR1);

	if (nInReg & M_TXCSR1_P_SENTSTALL)
		nOutReg |= M_TXCSR1_CLRDATATOG;

	if (!(nInReg & M_TXCSR1_FIFONOTEMPTY) || (nOutReg & M_TXCSR1_CLRDATATOG)) {
		HalUsbDmaSetDMA(EPBULKIN, DMA_TX_ONE_IRQ, ptUsb);
	}

	_USB_OTG_MSG("Start polling DMA M2U interrupt. vec: 0x%X\n", 0x0);
	while (!OTG_Interrupt_Polling(0, ptUsb)) ;
	_USB_OTG_MSG("End polling interrupt. vec: 0x%X\n", 0x0);
	return USB_ST_NOERROR;
}

int HalUsbDrcUsb2MiuBulk(u32 nRxAddr, u32 nRxSize, pUsbVar ptUsb)
{
	Endpoint_t *ep = &ptUsb->tUsbEndpointInf[EPBULKOUT];

	ep->nEpBytesRequested = nRxSize;
	ep->nEpBytesProcessed = 0;
	ep->nEpTransferBuffer = nRxAddr;
	ep->nEpTransferBuffer_length = 0;
	HalUsbDrcIndexSelect(ep->nEpBlt);

	while ((nRxSize > 0) && (ptUsb->nOtgDeviceConnect == 1)) {
		HalUsbDrcSetClrRXMode1();
		//?? 0xff
		while(OTG_Interrupt_Polling(0xFF, ptUsb) != 0x22);
		if(ep->nEpFifoRemain == 0)
			continue;

		nRxSize = nRxSize - ep->nEpFifoRemain;
		ptUsb->nOtgDataPhaseDir = usb_rcvbulknEpPipe(ep->nEpBlt);

		if (ep->nEpFifoRemain < ep->nEpMaxPacketSize)
			ep->nEpLastPacket = 1;
		else {
			if ((ep->nEpBytesProcessed + ep->nEpFifoRemain) >=
							 ep->nEpBytesRequested)
				ep->nEpLastPacket = 1;
			else
				ep->nEpLastPacket = 0;
		}

		HalUsbDrcFifoRead((u8*)ep->nEpTransferBuffer, EPBULKOUT, ptUsb);
		ep->nEpFifoRemain = USB_Read_RxCount();

#ifdef ENABLE_RIU_2BYTES_READ
		if (!ep->nEpFifoRemain || (ep->nEpFifoRemain == FIFOCOUNT_MINIUS_01)) {
			USB_REG_WRITE8(M_REG_RXCSR1, 0);
		}
#else
		if (!ep->nEpFifoRemain) {
			USB_REG_WRITE8(M_REG_RXCSR1, 0);
		}
#endif

		if(ep->nEpLastPacket == 1)
			break;
	}

	if(ptUsb->nOtgDeviceConnect == 0)
		return -1;

	return ep->nEpBytesProcessed;
}

int HalUsbDrcUsb2MiuBulkDMA(u32 nRxAddr, u32 nRxSize, pUsbVar ptUsb)
{
	Endpoint_t *ep;
	ep = &ptUsb->tUsbEndpointInf[EPBULKOUT];

	ptUsb->nOtgDmaRxInterruptStatus = 0xff;
	ep->nEpBytesRequested = nRxSize;
	ep->nEpBytesProcessed = 0;
	ep->nEpTransferBuffer = nRxAddr;
	ep->nEpTransferBuffer_length = 0;
	ep->nEpFifoRemain = nRxSize;
	ptUsb->nOtgDataPhaseDir = usb_rcvbulknEpPipe(ep->nEpBlt);

	/* ??? AHB bridge Write FIFO flush begin */
	//USB_REG_WRITE8(M_REG_USB_AHB, (USB_REG_READ8(M_REG_USB_AHB) | 0x10));
	HalUsbDmaSetDMA(EPBULKOUT, DMA_RX_ONE_IRQ, ptUsb);

	_USB_OTG_MSG("[TY] Start polling DMA U2M interrupt. vec: 0x%X\n", 0x0);
	while (!OTG_Interrupt_Polling(0, ptUsb));
	_USB_OTG_MSG("End polling DMA interrupt. vec: 0x%X\n", 0x0);
	return nRxSize;
}

u8 HalUsbDrcUSB2PC(u32 txaddr, u32 txsize, pUsbVar ptUsb)
{
	u32 mmaxcnt;
	u16 rem;

	Endpoint_t *ep = &ptUsb->tUsbEndpointInf[EPBULKIN];

	mmaxcnt = txsize;
	rem = mmaxcnt % ep->nEpMaxPacketSize;
	mmaxcnt = mmaxcnt - rem;

	if (mmaxcnt > 0) {
#ifdef DMA
		if (HalUsbDrcMiu2UsbBulkDMA(txaddr, mmaxcnt, ptUsb) != 0)
			return USB_ST_NORESPONSE;
#else
		if (HalUsbDrcMiu2UsbBulk(txaddr, mmaxcnt, ptUsb) != 0)
			return USB_ST_NORESPONSE;
#endif
	}

	if (rem > 0) {
		if (HalUsbDrcMiu2UsbBulk(txaddr + mmaxcnt, rem, ptUsb))
			return USB_ST_NORESPONSE;
	}
	return USB_ST_NOERROR;
}


#define MAX_2MB		(2 * 1024 * 1024)	

int HalUsbDrcPC2USB(u32 rxaddr, u32 rxsize, pUsbVar ptUsb)
{
	u32 mmaxcnt;
	u16 rem;
#ifdef DMA
	int result_tmp = 0;
	int result_dma = 0;
#endif
	int result_fifo = 0;
	int Cnt;

	Endpoint_t *ep = &ptUsb->tUsbEndpointInf[EPBULKOUT];

	_USB_OTG_MSG_L1("Rxsize %x\n", rxsize);
	Cnt = 0;
#ifdef DMA
	/* fastboot note: command 64 bytes wiil go to fifo diretly */

	while (rxsize > MAX_2MB) {
		Cnt++;
		mmaxcnt = MAX_2MB;
		_USB_OTG_MSG_L1("loop %d, dest:%08X len:%x\n", Cnt, rxaddr, MAX_2MB);
		
		if ((result_tmp = HalUsbDrcUsb2MiuBulkDMA(rxaddr, mmaxcnt, ptUsb)) < 0)
			return -1;

		result_dma += result_tmp;
		rxaddr += MAX_2MB;
		rxsize -= MAX_2MB;

	}	


	if (rxsize > 512) {
		Cnt++;
		mmaxcnt = (rxsize / ep->nEpMaxPacketSize) * ep->nEpMaxPacketSize;
		rem = rxsize % ep->nEpMaxPacketSize;
		_USB_OTG_MSG_L1("loop %d:dest:%08X len:%x+%x\n",Cnt, rxaddr, mmaxcnt, rem);		
		
		if ((result_tmp = HalUsbDrcUsb2MiuBulkDMA(rxaddr, mmaxcnt, ptUsb)) < 0)
			return -1;
		result_dma += result_tmp;
		//mmaxcnt = mmaxcnt - rem;
	} else {
		Cnt++;
		_USB_OTG_MSG_L1("byte mode:dest:%08x len:%x\n", rxaddr, rxsize);
		mmaxcnt = 0;
		rem = rxsize;
	}
		
#else
	/* FIFO mode */
	mmaxcnt = 0;
	rem = rxsize;	
#endif
	
	if (rem > 0) {
          	_USB_OTG_MSG_L1("FIFO mode go: %x %x\n", (rxaddr + mmaxcnt), rem);
		if ((result_fifo += HalUsbDrcUsb2MiuBulk(rxaddr + mmaxcnt, rem, ptUsb)) < 0)
			return -1;
	}
	
	return result_dma + result_fifo;
}


void HalUsbDrcParseInt(drcintitem_t * ptD, pUsbVar ptUsb)
{
	HalUsbDrcRefreshRegisters(ptUsb);

	if (ptD->ICause == IRC_INTRUSB)
		HalUsbOtgParseIntrUsb(ptD->IReg, ptUsb);
	else if DRCPERIPHERAL
		(ptUsb->nOtgRegDevCtl) HalUsbFunParseIntPeripheral(ptD, ptUsb);
}

void HalUsbDrcSetInterrupts(void)
{
	u16 reg;

	reg = 0xff;
	USB_REG_WRITE8(M_REG_INTRUSBE, 0xf7);
	USB_REG_WRITE16(M_REG_INTRTXE, reg);
	USB_REG_WRITE16(M_REG_INTRRXE, reg);
}

void HalUsbDrcClearInterrupts(void)
{
	u16 reg;

	reg = USB_REG_READ8(M_REG_INTRUSB);
	reg = USB_REG_READ16(M_REG_INTRTX);
	reg = USB_REG_READ16(M_REG_INTRRX);
	_USB_OTG_MSG("Clear reg:0x%x \n\r", reg);
}

s32 HalUsbDrcVBusStatus(void)
{
	u8 devctl;

	devctl = USB_REG_READ8(M_REG_DEVCTL);

	switch ((devctl & VBUS_MASK) >> 3) {
		case 0:
			return VBUS_BELOW_SESSION_END;
		case 1:
			return VBUS_ABOVE_SESSION_END;
		case 2:
			return VBUS_ABOVE_AVALID;
		case 3:
			return VBUS_ABOVE_VBUS_VALID;
	}
	return FAILURE;
}

u8 HalUsbDrcRetBltEpObject(u8 drcdir)
{
	u8 pipe = 0;

	if (drcdir == FIFO_DUAL)
		pipe = EPCONTROL;
	else if (drcdir == PIPE_IN)
		pipe = EPBULKIN;
	else if (drcdir == PIPE_OUT)
		pipe = EPBULKOUT;

	return pipe;
}

void HalUsbDrcSetClrRXMode1(void)
{
	/* disable set_ok2rcv[15]&ECO4NAK_en[14],wayne added */
	USB_REG_WRITE16(M_REG_DMA_MODE_CTL,
		(USB_REG_READ16(M_REG_DMA_MODE_CTL) & ~M_Mode1_P_OK2Rcv));
	/* enable Allow ok,wayne added */
	USB_REG_WRITE16(M_REG_DMA_MODE_CTL,
		(USB_REG_READ16(M_REG_DMA_MODE_CTL) | M_Mode1_P_AllowAck));
}

void HalUsbDrcResetClrRXMode1(void)
{
	/* disable set_ok2rcv[15]&ECO4NAK_en[14],wayne added */
	USB_REG_WRITE16(M_REG_DMA_MODE_CTL, 0);
}

void HalUsbDrcClearRegPower(u8 regupdate, pUsbVar ptUsb)
{
	if (!regupdate)
		ptUsb->nOtgRegPower = M_POWER_SOFTCONN | M_POWER_HSENAB;
	else
		ptUsb->nOtgRegPower &= ~regupdate;

	USB_REG_WRITE8(M_REG_POWER, ptUsb->nOtgRegPower);
	ptUsb->nOtgRegPower = USB_REG_READ8(M_REG_POWER);
}

void HalUsbDrcSwitchOffUsb(void)
{
	USB_REG_WRITE8(M_REG_POWER, USB_REG_READ8(M_REG_POWER) & ~M_POWER_SOFTCONN);

#ifndef USB_FPGA
	UTMI_REG_WRITE16(0x50, UTMI_REG_READ16(0x50) & 0x7FFF); /* disable DM-Pull high */
#endif

	USB_REG_WRITE8(0x100, USB_REG_READ8(0x100) & 0xFE);	/*  Reset OTG */
	USB_REG_WRITE8(0x100, USB_REG_READ8(0x100) | 0x01);
	HalUsbDrcDisableUSB();
}

void HalUsbDrcInit(pUsbVar ptUsb)
{
	USB_REG_WRITE8(M_REG_CFG0_L, USB_REG_READ8(M_REG_CFG0_L)&0xFE); // Reset OTG
	USB_REG_WRITE8(M_REG_CFG0_L, USB_REG_READ8(M_REG_CFG0_L)|0x01);


	/* USBC_REG_WRITE8(0, USBC_REG_READ8(0)|BIT6); */
	USB_REG_WRITE8(M_REG_FADDR, 0);
	// Set Index to 0
	USB_REG_WRITE8(M_REG_INDEX, 0);
	
	/* Enable write clear */
	USB_REG_WRITE8(M_REG_CFG6_H, USB_REG_READ8(M_REG_CFG6_H) | 0x08);

	USB_REG_WRITE8(M_REG_CFG6_H, USB_REG_READ8(M_REG_CFG6_H) | 0x40);
#ifdef USB_20
	/* High Speed Mode */
	USB_REG_WRITE8(M_REG_POWER, M_POWER_SOFTCONN | M_POWER_HSENAB);
#else
	/* Full Speed Mode */
	USB_REG_WRITE8(M_REG_POWER, M_POWER_SOFTCONN);
#endif
	USB_REG_WRITE8(M_REG_DEVCTL, 0);

	// Flush the next packet to be transmitted/ read from the endpoint 0 FIFO
	USB_REG_WRITE16(M_REG_CSR0, USB_REG_READ16(M_REG_CSR0) | M_CSR0_FLUSHFIFO);
	// Flush the latest packet from the endpoint Tx FIFO
	USB_REG_WRITE8(M_REG_INDEX, 1);
	USB_REG_WRITE16(M_REG_TXCSR, USB_REG_READ16(M_REG_TXCSR) | M_TXCSR_FLUSHFIFO);

	// Flush the next packet to be read from the endpoint Rx FIFO
	USB_REG_WRITE8(M_REG_INDEX, 2);
	USB_REG_WRITE16(M_REG_RXCSR, USB_REG_READ16(M_REG_RXCSR) | M_RXCSR_FLUSHFIFO);

	USB_REG_WRITE8(M_REG_INDEX, 0);

	_USB_OTG_MSG("[Info] M_REG_POWER reg:0x%x \n\r", USB_REG_READ8(M_REG_POWER));
}

void HalUsbClearRxStatus(u8 nEp)
{
	u8 nIndex;

	nIndex = USB_REG_READ8(M_REG_INDEX);
	USB_REG_WRITE8(M_REG_INDEX, nEp);
	USB_REG_WRITE8(M_REG_RXCSR1, 0);
	/* _USB_OTG_MSG("After Clear Ep: %d, 0x%02X \n\r",
			nEp, USB_REG_READ8(M_REG_RXCSR1)); */
	/* _USB_OTG_MSG("M_REG_INTRRXE: 0x%04X \n\r",
			USB_REG_READ16(M_REG_INTRRXE)); */
	USB_REG_WRITE8(M_REG_INDEX, nIndex);
}

void HalUsbDrcUtmiInit(void)
{
	unsigned int utmi_base = UTMIBaseAddress;
	unsigned int usbc_base = USBCBaseAddress;
	unsigned int otg_base = OTGBaseAddress;

	_USB_OTG_MSG_L1("[USB] TV_usb_init (UTMI Init)\n");
	_USB_OTG_MSG_L1("[USB] Ver. %s\n", VERSION);
	_USB_OTG_MSG_L1("[USB] UTMI Base %8x\n", utmi_base);
	_USB_OTG_MSG_L1("[USB] USBC Base %8x\n", usbc_base);
	_USB_OTG_MSG_L1("[USB] OTG Base %8x\n", otg_base);
	//_USB_OTG_MSG("[USB] BC Base %8x\n", hcd->bc_base);

	writeb((readb(usbc_base+0x2*2)&~(BIT0|BIT1))|BIT1, usbc_base+0x2*2);

	/* Disable MAC initial suspend, Reset UHC */
	writeb(0x0c, usbc_base);
	/* Release HW reset, enable OTG XIU function */
	writeb(0x48, usbc_base);	

	/* Init UTMI squelch level setting before CA */
	if(UTMI_DISCON_LEVEL_2A & (BIT3|BIT2|BIT1|BIT0))
	{
		writeb((UTMI_DISCON_LEVEL_2A & (BIT3|BIT2|BIT1|BIT0)),
			(void*)(utmi_base+0x2a*2));
		_USB_OTG_MSG("[USB] init squelch level 0x%x\n",
				readb((void*)(utmi_base+0x2a*2)));
	}

	writeb(readb(utmi_base+0x3C*2) | 0x01, utmi_base+0x3C*2); 	/* set CA_START as 1 */
	MsSleep(1); // 10->1

	writeb(readb(utmi_base+0x3C*2) & ~0x01, utmi_base+0x3C*2); 	/* release CA_START */
	/* polling bit <1> (CA_END) */
	while (((unsigned int)(readb(utmi_base+0x3C*2)) & 0x02) == 0);

	// Reset OTG controllers
	writeb(0x0c, usbc_base);
	// Unlock Register R/W functions  (RST_CTRL[6] = 1)
	// Enter suspend  (RST_CTRL[3] = 1)
	writeb(0x48, usbc_base);

	writeb((readb(utmi_base+0x06*2) & 0x9F) | 0x40, utmi_base+0x06*2); /* reg_tx_force_hs_current_enable */

	writeb(readb(utmi_base+0x03*2-1) | 0x28, utmi_base+0x03*2-1); 	/* Disconnect window select */
	writeb(readb(utmi_base+0x03*2-1) & 0xef, utmi_base+0x03*2-1);
	/* Disable improved CDR */
	writeb(readb(utmi_base+0x07*2-1) & 0xfd, utmi_base+0x07*2-1);

#if defined(ENABLE_UTMI_240_AS_120_PHASE_ECO)
	#if defined(UTMI_240_AS_120_PHASE_ECO_INV)
	// bit<3> special for Eiffel analog LIB issue
	writeb(readb((void*)utmi_base+0x08*2) & ~0x08,
				(void*) (utmi_base+0x08*2));
	#elif defined(UTMI_240_AS_120_PHASE_ECO_INV_IF_REV_0)
	if (MDrv_SYS_GetChipRev() == 0x0) {
		// bit<3> special for Eiffel analog LIB issue
		writeb(readb((void*)utmi_base+0x08*2) & ~0x08,
					(void*) (utmi_base+0x08*2));
	}
	#else
	/* bit<3> for 240's phase as 120's clock set 1,
	 * bit<4> for 240Mhz in mac 0 for faraday 1 for etron */
	writeb(readb(utmi_base+0x08*2) | 0x08, utmi_base+0x08*2);
	#endif
#endif
	/* UTMI RX anti-dead-loc, ISI effect improvement */
	writeb(readb(utmi_base+0x09*2-1) | 0x81, utmi_base+0x09*2-1);
#if defined(UTMI_TX_TIMING_SEL_LATCH_PATH_INV_IF_REV_0)
	if (MDrv_SYS_GetChipRev() == 0x0) {
		/* TX timing select latch path */
		writeb(readb(utmi_base+0x0b*2-1) & ~0x80, utmi_base+0x0b*2-1);
	}
#else
	/* TX timing select latch path */
	writeb(readb(utmi_base+0x0b*2-1) | 0x80, utmi_base+0x0b*2-1);
#endif
	/* Chirp signal source select */
	writeb(readb(utmi_base+0x15*2-1) | 0x20, utmi_base+0x15*2-1);
#if defined(ENABLE_UTMI_55_INTERFACE)
	/* change to 55 interface */
	writeb(readb(utmi_base+0x15*2-1) | 0x40, utmi_base+0x15*2-1);
#endif

#if defined(ENABLE_PV2MI_BRIDGE_ECO)
	writeb(readb((usbc_base+0x0a*2)) | 0x40, usbc_base+0x0a*2);
#endif
	/* Init UTMI eye diagram parameter setting */
	writeb(readb(utmi_base+0x2c*2) | UTMI_EYE_SETTING_2C, utmi_base+0x2c*2);
	writeb(readb(utmi_base+0x2d*2-1) | UTMI_EYE_SETTING_2D, utmi_base+0x2d*2-1);
	writeb(readb(utmi_base+0x2e*2) | UTMI_EYE_SETTING_2E, utmi_base+0x2e*2);
	writeb(readb(utmi_base+0x2f*2-1) | UTMI_EYE_SETTING_2F, utmi_base+0x2f*2-1);

	//usb_bc_enable(hcd->utmi_base, hcd->bc_base, FALSE);
	_USB_OTG_MSG("[USB] TV_usb_init--\n");
}
