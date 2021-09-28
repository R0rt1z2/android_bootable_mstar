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
#include <hal_usb_drc.h>
#include <hal_usb_dma.h>
#include <hal_usb_otg.h>
#include <hal_usb.h>
#include <hal_usb_function.h>

static void _HalUsbDmaSet32BitsReg(u32 volatile* nReg, u32 nVal)
{
	*(nReg) = (u16)(nVal & 0xffff);
	*(nReg+1)=(u16)(nVal >> 16);
	return;
}

static u32 _HalUsbDmaGet32BitsReg(u32 volatile* nReg)
{
	u32 nTemp;
	nTemp = ((*(nReg)) & 0xffff) + ((*(nReg+1))<<16) ;
	return nTemp;
}

static s8 _HalUsbDmaGetDmaChannel(pUsbVar ptUsb)
{
	u8 i, nBit;

	for(i = 0, nBit = 1; i <  MAX_DMA_CHANNEL; i++, nBit <<= 1) {
		if(ptUsb->nFreeDmaChannels & nBit) {
			ptUsb->nFreeDmaChannels &= ~nBit;
			return i+1;
		}
	}
	_USB_OTG_MSG("No channel \n\r");
	return FAILURE;
}

static void _HalUsbDmaFreeDmaChannel(s8 nChannel, pUsbVar ptUsb)
{
	_USB_OTG_MSG( "Release channel:0x%X \n\r", nChannel);
	ptUsb->nFreeDmaChannels |= (1 << (nChannel - 1));
	return;
}

u16 HalUsbDmaReadDmaControl(s8 nChannel)
{
	return *((DMA_CNTL_REGISTER(nChannel)));
}

void _HalUsbDmaControlEpInterrupt(s8 nEp, u32 nMode, pUsbVar ptUsb)
{
	u8 nReg, nCurrent, nBit;
	u8 nEndpoint;

	nEndpoint = ptUsb->tUsbEndpointInf[nEp].nEpBlt;

	if(nMode & EP_IRQ_TX)
		nReg = (nEndpoint < 8) ? M_REG_INTRTX1E : M_REG_INTRTX2E;
	else
		nReg = (nEndpoint < 8) ? M_REG_INTRRX1E : M_REG_INTRRX2E;

	nCurrent = USB_REG_READ8(nReg);
	nBit = 1 << (nEndpoint % 8);

	if(nMode & EP_IRQ_ENABLE)
		USB_REG_WRITE8(nReg, (nCurrent | nBit));
	else
		USB_REG_WRITE8(nReg, (nCurrent & ~nBit));
	return;
}


extern U32 MsOS_VA2PA(U32 addr);
extern void Chip_Flush_Memory(void);
extern void  flush_cache (unsigned long addr, unsigned long len);
s8 HalUsbDmaSetDMA(s8 nEp, u32 nMode, pUsbVar ptUsb)
{
	s8 nCh;
	u32 count, index, csr2;
	u32 nControl;
	u32 address;
	Endpoint_t *ep = &ptUsb->tUsbEndpointInf[nEp];

	address = ep->nEpTransferBuffer + ep->nEpBytesProcessed;
	count = ep->nEpBytesRequested - ep->nEpBytesProcessed;

	_USB_OTG_MSG("###### DMA Setting ######\n\r");
	_USB_OTG_MSG("count = %x, Request = %x, mode = %x\n\r",
				count, ep->nEpBytesRequested,nMode);

	/* if packet size is too small than use FIFO mode*/
	if (count < ep->nEpMaxPacketSize) {
		if ((nMode!=DMA_RX_ZERO_IRQ)&&(nMode!=DMA_TX_ZERO_IRQ)) {
			return FAILURE;
		} else if (count < 64) {
			return FAILURE;
		}
	}

	/* no free channel */
	if((nCh = _HalUsbDmaGetDmaChannel(ptUsb)) < 0) {
		_USB_OTG_MSG( "Get DMA channel fail \n\r");
		return FAILURE;
	}

	/* for multiple Bulk packets, set Mode 1 */
	if (count > ep->nEpMaxPacketSize) {
		nMode |= DMA_MODE_ONE;
	} else {
		if(nMode & DMA_TX)
			count = MIN(ep->nEpMaxPacketSize, count);
		else
			count = MIN(ep->nEpFifoRemain, count);
	}

	sys_Clean_and_flush_data_cache_buffer(address, count);

	/* prepare DMA control register */
	nControl = DMA_ENABLE_BIT | nMode |
		(ep->nEpBlt << DMA_ENDPOINT_SHIFT) | (DMA_BurstMode<<9);

	flush_cache((unsigned long)address,count);
	Chip_Flush_Memory();
	address=MsOS_VA2PA(address);

	//address = HalUtilPHY2MIUAddr(address);

#ifdef MIU_16BIT
	_HalUsbDmaSet32BitsReg((u32 volatile*)DMA_ADDR_REGISTER(nCh), address);
	_USB_OTG_MSG("DMA set_address 0x%08X set to 0x%08X\n\r",
					DMA_ADDR_REGISTER(nCh), address);
	_HalUsbDmaSet32BitsReg((u32 volatile*)DMA_COUNT_REGISTER(nCh), count);
	_USB_OTG_MSG("DMA set_count 0x%08X set to 0x%08X\n\r",
					DMA_COUNT_REGISTER(nCh), count);
#else
	*(DMA_ADDR_REGISTER(nCh)) = address;
	_USB_OTG_MSG("DMA set_address 0x%08X set to 0x%08X\n\r",
				DMA_ADDR_REGISTER(nCh), address);
	*(DMA_COUNT_REGISTER(nCh)) = count;
	_USB_OTG_MSG("DMA set_count 0x%08X set to 0x%08X\n\r",
				DMA_COUNT_REGISTER(nCh), count);
#endif

	/* save and set index register */
	index = USB_REG_READ8(M_REG_INDEX);
	USB_REG_WRITE8(M_REG_INDEX, ep->nEpBlt);

	/* program DRC registers */
	switch(nMode & DMA_MODE_MASK) {
		case DMA_RX | DMA_MODE_ZERO:
			csr2 = USB_REG_READ8(M_REG_RXCSR2);
			Enable_RX_EP_Interrupt(nEp);

			_USB_OTG_MSG("RX_mod0 ep=%x, cnt=%x, Req=%x, ctr=0x%x\n",
				nEp, count, ep->nEpBytesRequested, nControl);

			USB_REG_WRITE8(M_REG_RXCSR2, (csr2 & ~RXCSR2_MODE1));
			break;

		case DMA_TX | DMA_MODE_ZERO:
			Enable_TX_EP_Interrupt(nEp);
			csr2 = USB_REG_READ8(M_REG_TXCSR2);

			_USB_OTG_MSG("TX_mod0 ep=%x, cnt=%x, Req=%x, ctr=0x%x\n",
				nEp, count, ep->nEpBytesRequested, nControl);

			USB_REG_WRITE8(M_REG_TXCSR2, (csr2 & ~TXCSR2_MODE1));
			break;

		case DMA_RX | DMA_MODE_ONE:
			csr2 = USB_REG_READ8(M_REG_RXCSR2);

			_USB_OTG_MSG("RX_mod1 ep=%x, cnt=%x, Req=%x, ctr=0x%x\n",	
				nEp, count, ep->nEpBytesRequested, nControl);

			*(DMA_CNTL_REGISTER(nCh)) = nControl;
			/* Set32BitsReg(DMA_CNTL_REGISTER(ch), control); */
			Enable_RX_EP_Interrupt(nEp);

			if (DRCHOST(ptUsb->nOtgRegDevCtl))
				USB_REG_WRITE8(M_REG_RXCSR2,(csr2 |
					RXCSR2_MODE1 | M_RXCSR2_H_AUTOREQ ));
			else
				USB_REG_WRITE8(M_REG_RXCSR2, (csr2 | RXCSR2_MODE1));

			USB_REG_WRITE8(M_REG_RXCSR1, 0);	/* enable RX */

			USB_REG_WRITE16(M_REG_DMA_MODE_CTL,
				USB_REG_READ16(M_REG_DMA_MODE_CTL)&M_Mode1_P_Enable);
			/* disable set_ok2rcv[15]&ECO4NAK_en[14],wayne added */
			USB_REG_WRITE16(M_REG_DMA_MODE_CTL,
				USB_REG_READ16(M_REG_DMA_MODE_CTL)|(count/ep->nEpMaxPacketSize));
			/* enable set_ok2rcv[15]&ECO4NAK_en[14],wayne added */
			USB_REG_WRITE16(M_REG_DMA_MODE_CTL,
				USB_REG_READ16(M_REG_DMA_MODE_CTL)|M_Mode1_P_OK2Rcv);

			/* restore previous index value */
			USB_REG_WRITE8(M_REG_INDEX, index);
			return SUSS;

		case DMA_TX | DMA_MODE_ONE:
			_USB_OTG_MSG("TX_mod1 ep=%x, cnt=%x, Req=%x, Proc=0x%x\n",
				nEp, count, ep->nEpBytesRequested, ep->nEpBytesProcessed);
			Enable_TX_EP_Interrupt(nEp);
			csr2 = USB_REG_READ8(M_REG_TXCSR2);
			USB_REG_WRITE8(M_REG_TXCSR2, (csr2 | TXCSR2_MODE1));
	}

	/* restore previous index value */
	USB_REG_WRITE8(M_REG_INDEX, index);
	*(DMA_CNTL_REGISTER(nCh)) = nControl;

	/* _HalUsbDmaSet32BitsReg((u32 volatile*)DMA_CNTL_REGISTER(ch), control); */
	return SUSS;
}

extern U32 MsOS_PA2KSEG0(U32 addr);
void HalUsbDmaIrqHandler(u8 nCh, pUsbVar ptUsb)
{
	u8  index, endpoint;
	u32 csr, mode, bytesleft, addr, bytesdone, control;
	s8  nEp;
	u8  csr2;
	Endpoint_t *ep;

	/* get DMA Mode, address and byte counts from DMA registers */
	control =*(DMA_CNTL_REGISTER(nCh));
	mode = *(DMA_CNTL_REGISTER(nCh)) & 0xf;
	addr = _HalUsbDmaGet32BitsReg((u32 volatile*)DMA_ADDR_REGISTER(nCh));

	bytesleft = _HalUsbDmaGet32BitsReg((u32 volatile*)DMA_COUNT_REGISTER(nCh));

	/* get endpoint, URB pointer */
	endpoint = (*(DMA_CNTL_REGISTER(nCh)) & 0xf0) >> DMA_ENDPOINT_SHIFT;
	/* ep = HalUsbDrcRetBltEpObject(endpoint); */
	nEp = endpoint;
	ep = &ptUsb->tUsbEndpointInf[nEp];

	/* how many bytes were processed? */
	addr=MsOS_PA2KSEG0(addr);
	//addr = HalUtilMIU2PHYAddr(addr);

	/****************** must be fixed ************/
	/* bytesdone = addr - (((u32)(ep->nEpTransferBuffer +
			ep->nEpBytesProcessed)) & 0x0fffffff); */
	bytesdone = addr - ((u32)(ep->nEpTransferBuffer + ep->nEpBytesProcessed));

	_USB_OTG_MSG("nEpTransferBuff: 0x%X -- addr: 0x%X\n",
					ep->nEpTransferBuffer,  addr);

	ep->nEpBytesProcessed += bytesdone;

	/* save and set index register */
	index = USB_REG_READ8(M_REG_INDEX);
	USB_REG_WRITE8(M_REG_INDEX, ep->nEpBlt);

	/* release DMA channel */
	_HalUsbDmaFreeDmaChannel(nCh, ptUsb);

	/* clean DMA setup in CSR  */
	if (mode & DMA_TX) {
		csr2 = USB_REG_READ8(M_REG_TXCSR2);
		USB_REG_WRITE8(M_REG_TXCSR2, (csr2 & ~TXCSR2_MODE1));
	} else {
		csr2 = USB_REG_READ8(M_REG_RXCSR2);
		if (DRCHOST(ptUsb->nOtgRegDevCtl))
			USB_REG_WRITE8(M_REG_RXCSR2,(csr2 & ~(RXCSR2_MODE1 | M_RXCSR2_H_AUTOREQ)));
		else
			USB_REG_WRITE8(M_REG_RXCSR2, (csr2 & ~RXCSR2_MODE1));
	}

	/* Bus Error */
	if (control & DMA_BUSERROR_BIT) {
		_USB_OTG_MSG("DMA Bus ERR\n");
		if (mode & DMA_TX)
			ptUsb->nOtgDmaTxInterruptStatus=USB_ST_STALL;
		else
			ptUsb->nOtgDmaRxInterruptStatus=USB_ST_STALL;
		return;
	}

	if (mode & DMA_TX) {
		if (ep->nEpBytesProcessed == ep->nEpBytesRequested) {
			/* short packet */
			if (ep->nEpBytesProcessed % ep->nEpMaxPacketSize) {
				/* need to set TXPKTRDY manually */
				ep->nEpLastPacket = 1;
			} else { /* the last packet size is equal to nEpMaxPacketSize */
				if (mode & (DMA_MODE_ONE)) {
					ep->nEpLastPacket = 1;
					ptUsb->nOtgDmaTxInterruptStatus = USB_ST_NOERROR;
					return;
				} else {
					if (usb_nEpPipebulk(ptUsb->nOtgDataPhaseDir))
						ep->nEpLastPacket = 1;
					_USB_OTG_MSG("DMA TXONE1\n\r");
				}
			}
		}
	} else {
		Enable_RX_EP_Interrupt(nEp);
		ep->nEpFifoRemain = bytesleft;

		if (ep->nEpFifoRemain == 0) {
			/* short packet */
			if (ep->nEpBytesProcessed % ep->nEpMaxPacketSize) {
				ep->nEpLastPacket = 1;
			} else { /* the last packet size is equal to nEpMaxPacketSize */
				if (mode & DMA_MODE_ONE) {
					ep->nEpLastPacket = 1;
					//ptUsb->nOtgInterruptStatus=USB_ST_NOERROR;
					ptUsb->nOtgDmaRxInterruptStatus=USB_ST_NOERROR;
					//_USB_OTG_MSG("DMARXIntSts : %x \n\r",
					//	ptUsb->nOtgDmaRxInterruptStatus);
					return;
				}
			}
		}
	}

	/*  for short packet, CPU needs to handle TXPKTRDY/RXPKTRDY bit  */
	if (mode & DMA_TX) {
		USB_REG_WRITE8(M_REG_TXCSR1, M_TXCSR1_TXPKTRDY);
		ptUsb->nOtgDmaTxInterruptStatus=USB_ST_NOERROR;
		/* _USB_OTG_MSG("DMA TXONE2\n\r"); */
	} else {
		csr = USB_REG_READ8(M_REG_RXCSR1);
		/* _USB_OTG_MSG("DMARXCSR : %x\n",csr); */
		USB_REG_WRITE8(M_REG_RXCSR1, (csr & ~M_RXCSR1_RXPKTRDY));
	}

	/* restore the index */
	USB_REG_WRITE8(M_REG_INDEX, index);

	if (mode & DMA_TX) {
		/* _USB_OTG_MSG("DMA TXONE3\n\r"); */
		return;
	} else {
		if (((!ep->nEpFifoRemain) && (ep->nEpLastPacket)) ||
			(ep->nEpBytesProcessed >= ep->nEpBytesRequested)) {
			/* INT or BULK */
			if (!(usb_nEpPipeisoc(ptUsb->nOtgDataPhaseDir))) {
				/* ptUsb->nOtgInterruptStatus=USB_ST_NOERROR; */
				ptUsb->nOtgDmaRxInterruptStatus=USB_ST_NOERROR;
				/* _USB_OTG_MSG("DMARXIntSts1 : %x\n",
					ptUsb->nOtgDmaRxInterruptStatus); */
			}
		}
	}
}
