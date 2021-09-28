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
#include "../inc/type.h"
#include "../inc/Globalvar.h"
#include "../inc/ms_drc.h"
#include "../inc/ms_dma.h"
#include "../inc/ms_otg.h"
#include "../inc/ms_function.h"
#include "../inc/ms_cpu.h"
#include "../inc/ms_usb.h"


u8 OTG_Interrupt_Polling(u8 vec, USB_VAR *gUSBStruct)
{
	drcintitem_t    d;
	//U8        index_save;
	U8        epnum;
	U16       usbreg = 0,reg;
    U16       i;
    U16       intreg[2] = {0, 0};
#ifdef DMA
    U8        dma_intr;
#endif
	u8 bHandle = 0;
	//index_save = USB_REG_READ8(M_REG_INDEX);  /* we have local copy */
#ifdef DMA
	if (0x19 == vec)
	{
#if 1//jimy patch
		//printf("polling d_i:0x%x\n",dma_intr);
		while(1)
		{
			dma_intr = USB_REG_READ8(M_REG_DMA_INTR);
			//printf("rx0:%x\n",USB_REG_READ16(M_REG_INTRRX));
			if(dma_intr)
				break;
			else
			{
				if((((USB_REG_READ16(M_REG_DMA_MODE_CTL)) & 0x8000)==0) && (USB_REG_READ8(M_REG_DMA_MODE_CTL) !=0))
				{
					//printf("rx1:%x\n",USB_REG_READ16(M_REG_INTRRX));
					//printf("C:%x\n",USB_REG_READ16(0x128*2));
					//printf("A0:%x\n",USB_REG_READ16(0x8c*2));
					//printf("D0:%x\n",USB_REG_READ16(M_REG_DMA_MODE_CTL));
					USB_REG_WRITE16(0x8c*2,USB_REG_READ16(0x8c*2) & ~0x2000);
					//printf("A1:%x\n",USB_REG_READ16(0x8c*2));
					//printf("D1:%x\n",USB_REG_READ16(M_REG_DMA_MODE_CTL));
					USB_REG_WRITE8(M_REG_DMA_MODE_CTL+1, (USB_REG_READ8(M_REG_DMA_MODE_CTL+1)) & ~0x20);
					//printf("P:%x\n",Get32BitsReg((U32 volatile*)DMA_COUNT_REGISTER(1)));
					//printf("D:%x\n",USB_REG_READ16(M_REG_DMA_MODE_CTL));
					//printf("R:%x\n",USB_REG_READ16(0x126*2));
					//printf("M:%x\n",*(DMA_CNTL_REGISTER(1)));
				}
				else
				{
					//printf("p:%x\n",Get32BitsReg((U32 volatile*)DMA_COUNT_REGISTER(1)));
					//printf("rxcsr:0x%x\n",USB_REG_READ16(M_REG_RXCSR1));
					//printf("d:%x\n",USB_REG_READ16(M_REG_DMA_MODE_CTL));
					//printf("r:%x\n",USB_REG_READ16(0x126*2));
					//printf("m:%x\n",*(DMA_CNTL_REGISTER(1)));
					MsOS_DelayTask(10);//10ms
				}
			}
			//if(Get32BitsReg((U32 volatile*)DMA_COUNT_REGISTER(1))==0)
			//	break;
		}
#else
	//printf("polling d_i:0x%x\n",dma_intr);
	while(1)
	{
		dma_intr = USB_REG_READ8(M_REG_DMA_INTR);
		printf("rx0:%x\n",USB_REG_READ16(M_REG_INTRRX));
		if(dma_intr)
			break;
		else
		{
			if((((USB_REG_READ16(M_REG_DMA_MODE_CTL)) & 0x8000)==0) && (USB_REG_READ8(M_REG_DMA_MODE_CTL) !=0))
			{
				printf("rx1:%x\n",USB_REG_READ16(M_REG_INTRRX));
				printf("C:%x\n",USB_REG_READ16(0x128*2));
				printf("A0:%x\n",USB_REG_READ16(0x8c*2));
				printf("D0:%x\n",USB_REG_READ16(M_REG_DMA_MODE_CTL));
				//USB_REG_WRITE16(0x8c*2,USB_REG_READ16(0x8c*2) & ~0x2000);
				printf("A1:%x\n",USB_REG_READ16(0x8c*2));
				printf("D1:%x\n",USB_REG_READ16(M_REG_DMA_MODE_CTL));
				//USB_REG_WRITE8(M_REG_DMA_MODE_CTL+1, (USB_REG_READ8(M_REG_DMA_MODE_CTL+1)) & ~0x20);
				printf("P:%x\n",Get32BitsReg((U32 volatile*)DMA_COUNT_REGISTER(1)));
				printf("D:%x\n",USB_REG_READ16(M_REG_DMA_MODE_CTL));
				printf("R:%x\n",USB_REG_READ16(0x126*2));
				printf("M:%x\n",*(DMA_CNTL_REGISTER(1)));
			}
			else
			{
				printf("p:%x\n",Get32BitsReg((U32 volatile*)DMA_COUNT_REGISTER(1)));
				//printf("rxcsr:0x%x\n",USB_REG_READ16(M_REG_RXCSR1));
				printf("d:%x\n",USB_REG_READ16(M_REG_DMA_MODE_CTL));
				printf("r:%x\n",USB_REG_READ16(0x126*2));
				printf("m:%x\n",*(DMA_CNTL_REGISTER(1)));
			}
		}
		//if(Get32BitsReg((U32 volatile*)DMA_COUNT_REGISTER(1))==0)
		//	break;
	}
#endif
		//printf("d_control_reg:0x%x\n",*(DMA_CNTL_REGISTER(1)));
		//printf("USB_CFG1_L:0x%x\n",USB_REG_READ16(M_REG_CFG1_L));
		//printf("rxcount:0x%x\n",USB_REG_READ16(0x128*2));
		//printf("USB_CFG3_L:0x%x\n",USB_REG_READ16(0x86*2));
		USB_REG_WRITE8(M_REG_DMA_INTR, dma_intr); // interrupt write clear
		if (dma_intr)
		{
			for (i = 0; i < MAX_DMA_CHANNEL; i++)
			{
				if (dma_intr & (1 << i))
				{
					d.IEP = i + 1;
					d.IDMA = 1;
					//				  printf("DMA int:%x",d.IEP);
					USB_DMA_IRQ_Handler(d.IEP,gUSBStruct);
					d.IDMA = 0;
					d.IEP = 0;
				}
			}
			bHandle = 0x19;
		}
	}
#endif
	if (vec == 1)
	{
		/* USB Interrupt */
		usbreg = USB_REG_READ8(M_REG_INTRUSB);
		USB_REG_WRITE8(M_REG_INTRUSB, usbreg);
		if  (usbreg)
		{
			//printf("M_REG_INTRUSB\n");
			d.IReg = usbreg;
			d.ICause = IRC_INTRUSB;
			USB_Parse_DRC_Int(&d,gUSBStruct);

			bHandle = 0x11;
		}
	}

	if (0x20 == vec)
	{
		intreg[0] = USB_REG_READ16(M_REG_INTRTX);
		USB_REG_WRITE16(M_REG_INTRTX, intreg[0]);
		/* EP0 Interrupt */
		if  (intreg[0] & 0x0001)
		{
			//gprintf("ep0_polling\n");
			//USB_REG_WRITE8(M_REG_INDEX, 0);
			reg = USB_REG_READ8(0x102*2);//USB_REG_READ8(M_REG_CSR0);
			d.IIntSrc = EP_TX;
			d.IEP = 0;
	        d.IReg = reg;
	        d.ICause = IRC_EP;
			if  (reg & M_CSR0_RXPKTRDY)
	        {
	            d.ICount = USB_REG_READ8(0x108*2);//USB_REG_READ16(M_REG_RXCOUNT);
				//printf("d.ICount:0x%x\n",d.ICount);
				//printf("d.IReg:0x%x\n",d.IReg);
	            //if (d.ICount==0)
	                //diag_printf("EP0RXCNT=0\n");
	        }
			USB_Parse_DRC_Int(&d,gUSBStruct);
			intreg[0] &= ~0x0001;
			bHandle = 0x20;
		}
	}
	d.ICause = IRC_EP;
	if (0x21 == vec) /* EPx TX Interrupt */
	{
		intreg[0] = USB_REG_READ16(M_REG_INTRTX);
		if(intreg[0]& 0x1)
		{
			intreg[0] = intreg[0] & 0xe;
		}
		USB_REG_WRITE16(M_REG_INTRTX, intreg[0]);
		epnum = 1;  i = 2;
		while (intreg[0] & 0x2)
		{
	        if (intreg[0] & i)
	        {
	            //USB_REG_WRITE8(M_REG_INDEX, epnum);
	            d.IReg = USB_REG_READ8(0x112*2);//USB_REG_READ8(M_REG_TXCSR1);
	            d.IIntSrc = EP_TX;
	            d.IEP = epnum;
	            //gprintf("EP_TX int_polling\n");
				//printf("d.IReg:0x%x\n",d.IReg);
	            USB_Parse_DRC_Int(&d,gUSBStruct);
	            intreg[0] &= ~i;

				bHandle = 0x21;
	        }
			#if 0
			if(intreg[0] == 1)
			{
				printf("[Error] break form EPx TX for EP0 INTR  \n\r");
				printf("[Error] Handle EP0 INTR \n");

				USB_REG_WRITE8(M_REG_INDEX, 0);
				reg = USB_REG_READ8(M_REG_CSR0);
				d.IIntSrc = EP_TX;
				d.IEP = 0;
		        d.IReg = reg;
		        d.ICause = IRC_EP;

				if (reg & M_CSR0_RXPKTRDY) {
					d.ICount = USB_REG_READ16(M_REG_RXCOUNT);
				}
				USB_Parse_DRC_Int(&d,gUSBStruct);
				intreg[0] &= ~0x0001;
			}
			#endif
			epnum ++;
			i = i << 1;
		}
	}
	if (vec == 0x22)
	{	/* EPx RX Interrupt */
		intreg[1] = USB_REG_READ16(M_REG_INTRRX);
		USB_REG_WRITE16(M_REG_INTRRX, intreg[1]);
		epnum = 1;  i = 2;
		while (intreg[1])
		{
	        if (intreg[1] & i)
	        {
	            USB_REG_WRITE8(M_REG_INDEX, epnum);
	            d.IReg = USB_REG_READ8(M_REG_RXCSR1);//USB_REG_READ8(0x126*2);
	            d.IIntSrc = EP_RX;
	            d.IEP = epnum;
	            d.ICount = USB_REG_READ16(M_REG_RXCOUNT);//USB_REG_READ8(0x128*2);
	            //gprintf("EP_RX int:0x%x\n",epnum);
				//printf("d.IReg:0x%x\n",d.IReg);
				//printf("d.ICount:0x%x\n",d.ICount);
	            USB_Parse_DRC_Int(&d,gUSBStruct);
	            intreg[1] &= ~i;
				bHandle = 0x22;
	        }
	        epnum ++;
	        i = i << 1;
	    }
	}
	//USB_REG_WRITE8(M_REG_INDEX, index_save);

	return bHandle;
}


void USB_ReSet_ClrRXMode1(void)
{
        USB_REG_WRITE16(M_REG_DMA_MODE_CTL, 0); //disable set_ok2rcv[15]&ECO4NAK_en[14],wayne added
        //USB_REG_WRITE16(M_REG_EP_BULKOUT, (USB_REG_READ16(M_REG_EP_BULKOUT)&0xfff0)); //wayne added
}

void USB_Set_ClrRXMode1(void)
{
        USB_REG_WRITE16(M_REG_DMA_MODE_CTL, (USB_REG_READ16(M_REG_DMA_MODE_CTL)&~M_Mode1_P_OK2Rcv)); //disable set_ok2rcv[15]&ECO4NAK_en[14],wayne added
        USB_REG_WRITE16(M_REG_DMA_MODE_CTL, (USB_REG_READ16(M_REG_DMA_MODE_CTL)|M_Mode1_P_AllowAck)); //enable Allow ok,wayne added
}

void USB_Set_DRC_Interrupts(void)
{
    U16 reg;
    reg = 0xff;
    USB_REG_WRITE8(M_REG_INTRUSBE, 0xf7);
    USB_REG_WRITE16(M_REG_INTRTXE, reg);
    USB_REG_WRITE16(M_REG_INTRRXE, reg);
}

void USB_Parse_DRC_Int(drcintitem_t *dP,USB_VAR *gUSBStruct)
{
	if  (dP->ICause == IRC_INTRUSB)
		USB_Parse_DRC_INTRUSB(dP->IReg,gUSBStruct);
	else if (DRCPERIPHERAL(gUSBStruct->otgRegDevCtl))
		USB_Parse_DRC_Int_Peripheral(dP,gUSBStruct);
}

void USB_DRC_Index_Select(U8 epnum)
{
    U8    reg;
    reg = USB_REG_READ8(M_REG_INDEX);
    if  (reg != epnum)
        USB_REG_WRITE8(M_REG_INDEX, epnum);
}

void USB_Enable_EP_DRC(U8 ep,USB_VAR *gUSBStruct)
{
    //U8 reg;
    //U16 csr;

	if	(gUSBStruct->otgUSB_EP[ep].DRCDir == FIFO_RX)
	{
		USB_REG_WRITE16(0x124*2,0x400/*gUSBStruct->otgUSB_EP[ep].MaxEPSize*/);
		USB_REG_WRITE16(0x126*2,USB_REG_READ16(0x126*2) | 0x0080);
	}
	else
	{
		USB_REG_WRITE16(0x110*2,0x400/*gUSBStruct->otgUSB_EP[ep].MaxEPSize*/);
		USB_REG_WRITE16(0x112*2,0x0008 | 0x2000 | 0x0040);
	}
#if 0
    USB_DRC_Index_Select(gUSBStruct->otgUSB_EP[ep].BltEP);

	reg = (gUSBStruct->otgUSB_EP[ep].BltEP & 0x0f) | ((gUSBStruct->otgUSB_EP[ep].pipe & 0x03) << 4);
	csr = USB_REG_READ16(M_REG_RXCSR);
	if  (gUSBStruct->otgUSB_EP[ep].DRCDir == FIFO_RX)
	{
		USB_REG_WRITE8(M_REG_RXTYPE, reg);
		USB_REG_WRITE16(M_REG_RXMAXP, gUSBStruct->otgUSB_EP[ep].MaxEPSize);
		if (csr & M_RXCSR_RXPKTRDY)
		{
			gUSBStruct->otgUSB_EP[ep].FifoRemain = USB_Read_RxCount();
		}
		else
			csr = M_RXCSR_CLRDATATOG;

		//if (gUSBStruct->otgUSB_EP[ep].pipe == USB_ENDPOINT_XFER_INT)
		//	csr |= M_RXCSR_DISNYET;
        USB_REG_WRITE16(M_REG_RXCSR, csr);
        USB_REG_WRITE16(M_REG_TXCSR, 0);
		//USB_REG_WRITE16(0x124*2, gUSBStruct->otgUSB_EP[ep].MaxEPSize);
		//USB_REG_WRITE8(0x126*2, USB_REG_READ8(0x126*2) | M_RXCSR_CLRDATATOG);
	}
	else
	{
        //if (gUSBStruct->u8USBDevMode == E_USB_OTG)
         //   USB_REG_WRITE8(M_REG_TXTYPE, reg);
        USB_REG_WRITE16(M_REG_TXMAXP,gUSBStruct->otgUSB_EP[ep].MaxEPSize);
        USB_REG_WRITE16(M_REG_TXCSR, M_TXCSR_FLUSHFIFO);
        //if (gUSBStruct->otgUSB_EP[ep].pipe == PIPE_ISOCHRONOUS)
        //    csr = M_TXCSR_ISO | M_TXCSR_MODE | M_TXCSR_CLRDATATOG;
        //else
            csr = M_TXCSR_MODE | M_TXCSR_CLRDATATOG;
        USB_REG_WRITE16(M_REG_TXCSR, csr);
	//USB_REG_WRITE16(0x110*2, gUSBStruct->otgUSB_EP[ep].MaxEPSize);
	//USB_REG_WRITE8(0x112*2, USB_REG_READ8(0x112*2) | M_RXCSR_CLRDATATOG | M_TXCSR_MODE | M_TXCSR_FLUSHFIFO);
    }
#endif
}

void USB_Setup_Endpoint(S8 epnum,S8 epdir,S8 eptype,S8 epinf ,S16 epmaxps,USB_VAR *gUSBStruct)
{
    gUSBStruct->otgUSB_EP[epnum].MaxEPSize = epmaxps;
    gUSBStruct->otgUSB_EP[epnum].DRCDir = epdir;
    gUSBStruct->otgUSB_EP[epnum].FifoRemain = 0;
    gUSBStruct->otgUSB_EP[epnum].BytesRequested = 0;
    gUSBStruct->otgUSB_EP[epnum].BytesProcessed = 0;
    gUSBStruct->otgUSB_EP[epnum].pipe = eptype;
    gUSBStruct->otgUSB_EP[epnum].BltEP = epnum;
    gUSBStruct->otgUSB_EP[epnum].LastPacket = 0;
    gUSBStruct->otgUSB_EP[epnum].transfer_buffer = 0;
    gUSBStruct->otgUSB_EP[epnum].transfer_buffer_length = 0;
    gUSBStruct->otgUSB_EP[epnum].IOState = EP_IDLE;
    gUSBStruct->otgUSB_EP[epnum].Halted = 0;
    gUSBStruct->otgUSB_EP[epnum].Infnum = epinf;
    //if (epnum!=0)
    //   USB_Enable_EP_DRC(epnum,gUSBStruct);
}

void USB_DRC_Fifo_Write(U8 *srcP,U8 ep,USB_VAR *gUSBStruct)
{
    S32 writecount;
    U32 fifo;
    //U8 i;
    if (gUSBStruct->otgUSB_EP[ep].MaxEPSize!=0)
        writecount = MIN(gUSBStruct->otgUSB_EP[ep].MaxEPSize, (gUSBStruct->otgUSB_EP[ep].BytesRequested - gUSBStruct->otgUSB_EP[ep].BytesProcessed));
    else
        writecount = gUSBStruct->otgUSB_EP[ep].BytesRequested - gUSBStruct->otgUSB_EP[ep].BytesProcessed;
    //gprintf("write count:%x\n",writecount);
    if  (!writecount)
    {
        gUSBStruct->otgUSB_EP[ep].LastPacket = 1;
        return;
    }
    srcP += gUSBStruct->otgUSB_EP[ep].BytesProcessed;
    gUSBStruct->otgUSB_EP[ep].BytesProcessed += writecount;
    if  (gUSBStruct->otgUSB_EP[ep].BytesProcessed == gUSBStruct->otgUSB_EP[ep].BytesRequested)
        gUSBStruct->otgUSB_EP[ep].LastPacket = 1;
    fifo=FIFO_ADDRESS(gUSBStruct->otgUSB_EP[ep].BltEP);
    //printf("write EP:%x\n",ep);
    //gprintf("FIFO address:%x \n",fifo);
    //for (i=0 ; i < writecount ; i++)
    //    gprintf("%x ",srcP[i]);
	//gprintf("\n");
	//*(volatile unsigned int *)(0x1f206704)=(srcP[7]);
    /* byte access for unaligned */
    if ((writecount > 0) && ((U32) srcP & 1))
    {
              while(writecount)
              {
                  *((U8 volatile *)fifo) = *((U8 volatile *)srcP);
                  srcP++;
                  writecount--;
              }
    }
    else     /* word access if aligned */
    {
              while ((writecount >= 2) && !((U32) srcP & 1) )
              {
                  *((U16 volatile *)fifo) = *((U16 volatile *)srcP);
                  srcP = srcP + 2;
                  writecount = writecount - 2;
              }
              while (writecount > 0)
              {
                  *((U8 volatile *)fifo) = *((U8 volatile *)srcP);
                  srcP++;
                  writecount--;
              }
    }
}

void USB_DRC_Fifo_Read(U8 *dstP,U8 ep,USB_VAR *gUSBStruct)
{
	S32 readcount;
	U32 fifo;

	readcount = MIN((gUSBStruct->otgUSB_EP[ep].BytesRequested - gUSBStruct->otgUSB_EP[ep].BytesProcessed),gUSBStruct->otgUSB_EP[ep].FifoRemain);


	dstP += gUSBStruct->otgUSB_EP[ep].BytesProcessed;
	gUSBStruct->otgUSB_EP[ep].BytesProcessed += readcount;
	gUSBStruct->otgUSB_EP[ep].FifoRemain -= readcount;
	fifo = FIFO_ADDRESS(gUSBStruct->otgUSB_EP[ep].BltEP);

	//gprintf("readcount:0x%x, ep:0x%x\n",readcount,ep);
	//printf("BytesProcessed:0x%x\n",gUSBStruct->otgUSB_EP[ep].BytesProcessed);
	if ((readcount > 0)&& ((U32) dstP & 1))
	{
		while (readcount > 0)
		{
			*((U8 volatile *)dstP)= *((U8 volatile  *)fifo); //set address//wayne add
			//*((U8 volatile *)dstP) = *((U8 volatile *)FIFO_DATA_PORT);
			dstP++;
			readcount--;
		}
	}
	else
	{
		while ((readcount  >= 2)&& !((U32) dstP & 1))
		{
			*((U16 volatile *)dstP)=*((U16 volatile *)fifo); //set address//wayne add
			//*((U16 volatile *)dstP) = *((U16 volatile *)FIFO_DATA_PORT);
			dstP = dstP + 2;
			readcount = readcount - 2;
		}
		while (readcount > 0)
		{
			*((U8 volatile *)dstP)=*((U8 volatile *)fifo); //set address//wayne add
			//*((U8 volatile *)dstP) = *((U8 volatile *)FIFO_DATA_PORT);
			dstP++;
			readcount--;
		}
	}
}

U8  USB_Ret_Blt_EP_Object(U8 drcdir)
{
    U8 i=0;

	if (drcdir==PIPE_IN)
        i=1;
    else if (drcdir==PIPE_OUT)
        i=2;
    return i;
}

#ifdef DMA
U8 USB2SDRAM_BulkDMA(U32 rxaddr,U32 rxsize,USB_VAR *gUSBStruct)
{
	U8  i = 0;
    gUSBStruct->otgDMARXIntStatus=0xff;
    gUSBStruct->otgUSB_EP[2].BytesRequested=rxsize;
    gUSBStruct->otgUSB_EP[2].BytesProcessed=0;
    gUSBStruct->otgUSB_EP[2].transfer_buffer=rxaddr;
    gUSBStruct->otgUSB_EP[2].transfer_buffer_length=0;
    gUSBStruct->otgUSB_EP[2].FifoRemain = rxsize;
    gUSBStruct->otgDataPhaseDir=usb_rcvbulkpipe(gUSBStruct->otgUSB_EP[2].BltEP);   //PIPE_In

	USB_REG_WRITE8(M_REG_USB_AHB,(USB_REG_READ8(M_REG_USB_AHB)|0x10)); //AHB bridge Write FIFO flush begin
	USB_Set_DMA(2, DMA_RX_ONE_IRQ, 0,gUSBStruct);


	//while(!(USB_REG_READ8(M_REG_USB_AHB)&0x20)&&(gUSBStruct->u8USBDeviceMode==1))
	//{
	//	/* Polling AHB bridge Write FIFO flush, counter value 0x80 in experienced */
    //    if (++i > 0x80)
    //        break;
	//}
	//USB_REG_WRITE8(M_REG_USB_AHB,USB_REG_READ8(M_REG_USB_AHB)); //AHB bridge Write FIFO flush clear
	while (!OTG_Interrupt_Polling(0x19, gUSBStruct)) ;
	//printf("Status:0x%x\n",gUSBStruct->otgDMARXIntStatus);
	while((gUSBStruct->otgDMARXIntStatus==0xff)&&(gUSBStruct->u8USBDeviceMode==1));
	//while(!(USB_REG_READ8(M_REG_USB_AHB)&0x20)&&(gUSBStruct->u8USBDeviceMode==1));

	return USB_ST_NOERROR;
}
#endif

U8 USB2SDRAM_Bulk(U32 rxaddr,U32 rxsize,USB_VAR *gUSBStruct)
{
    gUSBStruct->otgUSB_EP[2].BytesRequested=rxsize;
    gUSBStruct->otgUSB_EP[2].BytesProcessed=0;
    gUSBStruct->otgUSB_EP[2].transfer_buffer=rxaddr;
    gUSBStruct->otgUSB_EP[2].transfer_buffer_length=0;
    //USB_DRC_Index_Select(gUSBStruct->otgUSB_EP[2].BltEP);
    //gprintf("rxsize:0x%x\n",rxsize);
    while (rxsize>0)
    {
		USB_Set_ClrRXMode1();
		while (!OTG_Interrupt_Polling(0x22, gUSBStruct)) ;//polling data phase rx interrupt
		if (gUSBStruct->otgMassRxDataReceived==1)
		{
			gUSBStruct->otgMassRxDataReceived=0;
			rxsize=rxsize-gUSBStruct->otgUSB_EP[2].FifoRemain;
			gUSBStruct->otgDataPhaseDir=usb_rcvbulkpipe(gUSBStruct->otgUSB_EP[2].BltEP);   //PIPE_In
			if (gUSBStruct->otgUSB_EP[2].FifoRemain < gUSBStruct->otgUSB_EP[2].MaxEPSize)
				gUSBStruct->otgUSB_EP[2].LastPacket = 1;
			else
			{
                if ((gUSBStruct->otgUSB_EP[2].BytesProcessed + gUSBStruct->otgUSB_EP[2].FifoRemain) >= gUSBStruct->otgUSB_EP[2].BytesRequested)
                    gUSBStruct->otgUSB_EP[2].LastPacket = 1;
                else
                    gUSBStruct->otgUSB_EP[2].LastPacket = 0;
			}
			USB_DRC_Fifo_Read((U8 *)gUSBStruct->otgUSB_EP[2].transfer_buffer, 2, gUSBStruct);
			gUSBStruct->otgUSB_EP[2].FifoRemain = USB_Read_RxCount();
			if (!gUSBStruct->otgUSB_EP[2].FifoRemain)
				USB_REG_WRITE8(0x126*2, 0);//USB_REG_WRITE8(M_REG_RXCSR1, 0);
		}
    }

	//while (!OTG_Interrupt_Polling(0x22, gUSBStruct)) ;

    while((gUSBStruct->otgUSB_EP[2].BytesRequested!=gUSBStruct->otgUSB_EP[2].BytesProcessed) && (gUSBStruct->u8USBDeviceMode == 1));	// 20110310
    while((!gUSBStruct->otgUSB_EP[2].LastPacket)&&(gUSBStruct->u8USBDeviceMode==1));
    gUSBStruct->otgUSB_EP[2].BytesRequested=0;
    return USB_ST_NOERROR;
}

U8 SDRAM2USB_Bulk(U32 txaddr,U32 txsize,USB_VAR *gUSBStruct)
{
	U8 inreg,outreg=0;
	gUSBStruct->otgIntStatus=0xff;
    gUSBStruct->otgUSB_EP[1].IOState = EP_TX;
    gUSBStruct->otgUSB_EP[1].BytesRequested = txsize;
    gUSBStruct->otgUSB_EP[1].BytesProcessed = 0;
    gUSBStruct->otgUSB_EP[1].LastPacket     = 0;
    gUSBStruct->otgUSB_EP[1].transfer_buffer=txaddr;
    gUSBStruct->otgDataPhaseDir=usb_sndbulkpipe(gUSBStruct->otgUSB_EP[1].BltEP);   //PIPE_OUT

    //USB_DRC_Index_Select(gUSBStruct->otgUSB_EP[1].BltEP);
	inreg = USB_REG_READ8(0x112*2);//USB_REG_READ8(M_REG_TXCSR);
    if  (inreg & M_TXCSR1_P_SENTSTALL)
         outreg |= M_TXCSR1_CLRDATATOG;
	if  (!(inreg & M_TXCSR1_FIFONOTEMPTY)||(outreg & M_TXCSR1_CLRDATATOG))
	{
		//if(USB_Set_DMA(1, DMA_TX_ONE_IRQ, 0,gUSBStruct) < 0)
		{
			USB_DRC_Fifo_Write((U8 *)gUSBStruct->otgUSB_EP[1].transfer_buffer, 1,gUSBStruct);
			outreg |= M_TXCSR1_TXPKTRDY;
		}
		//else
		//	outreg = 0;
	}
	 if  (outreg)
		USB_REG_WRITE8(0x112*2, outreg);//USB_REG_WRITE8(M_REG_TXCSR1, outreg);

	while (!OTG_Interrupt_Polling(0x21, gUSBStruct)) ;
    while((gUSBStruct->otgIntStatus==0xff)&&(gUSBStruct->u8USBDeviceMode==1));
    while((!gUSBStruct->otgUSB_EP[1].LastPacket)&&(gUSBStruct->u8USBDeviceMode==1));
	gUSBStruct->otgUSB_EP[1].BytesRequested=0;
	return USB_ST_NOERROR;
}


void USB_DRC_Interrupt(USB_VAR *gUSBStruct)
{
	drcintitem_t    d;
	//U8        index_save;
	U8        epnum;
	U16       usbreg = 0,reg;
    U16       i;
    U16       intreg[2] = {0, 0};
#ifdef DMA
    U8        dma_intr;
#endif
	u8 bHandle = 0;
	//index_save = USB_REG_READ8(M_REG_INDEX);  /* we have local copy */
#ifdef DMA
	dma_intr = USB_REG_READ8(M_REG_DMA_INTR);
	USB_REG_WRITE8(M_REG_DMA_INTR, dma_intr); // interrupt write clear
	if (dma_intr)
	{
		printf("d_i:%x\n",dma_intr);
		for (i = 0; i < MAX_DMA_CHANNEL; i++)
		{
			if (dma_intr & (1 << i))
			{
				d.IEP = i + 1;
				d.IDMA = 1;
		//				  printf("DMA int:%x",d.IEP);
				USB_DMA_IRQ_Handler(d.IEP,gUSBStruct);
				d.IDMA = 0;
				d.IEP = 0;
			}
		}
	}
#endif
	/* USB Interrupt */
	usbreg = USB_REG_READ8(M_REG_INTRUSB);

	USB_REG_WRITE8(M_REG_INTRUSB, usbreg);
	if  (usbreg)
	{
		//printf("M_REG_INTRUSB\n");
		d.IReg = usbreg;
		d.ICause = IRC_INTRUSB;
		USB_Parse_DRC_Int(&d,gUSBStruct);

		bHandle = 0x11;
	}

	intreg[0] = USB_REG_READ16(M_REG_INTRTX);
	intreg[1] = USB_REG_READ16(M_REG_INTRRX);
	USB_REG_WRITE16(M_REG_INTRTX, intreg[0]);
	USB_REG_WRITE16(M_REG_INTRRX, intreg[1]);
	/* EP0 Interrupt */
	if  (intreg[0] & 0x0001)
	{
		//gprintf("ep0\n");
        //USB_REG_WRITE8(M_REG_INDEX, 0);
        reg = USB_REG_READ8(0x102*2);//USB_REG_READ8(M_REG_CSR0);
		d.IIntSrc = EP_TX;
		d.IEP = 0;
        d.IReg = reg;
        d.ICause = IRC_EP;
		if  (reg & M_CSR0_RXPKTRDY)
        {
            d.ICount = USB_REG_READ8(0x108*2);//USB_REG_READ16(M_REG_RXCOUNT);
			//printf("d.ICount:0x%x\n",d.ICount);
			//printf("d.IReg:0x%x\n",d.IReg);
            //if (d.ICount==0)
                //diag_printf("EP0RXCNT=0\n");
        }
		USB_Parse_DRC_Int(&d,gUSBStruct);
		intreg[0] &= ~0x0001;
		bHandle = 0x20;
	}

	d.ICause = IRC_EP;
	/* EPx TX Interrupt */
	epnum = 1;  i = 2;
	while (intreg[0])
	{
        if (intreg[0] & i)
        {
            //USB_REG_WRITE8(M_REG_INDEX, epnum);
            d.IReg = USB_REG_READ8(0x112*2);//USB_REG_READ8(M_REG_TXCSR1);
            d.IIntSrc = EP_TX;
            d.IEP = epnum;
            //gprintf("EP_TX int_isr\n");
			//printf("d.IReg:0x%x\n",d.IReg);
            USB_Parse_DRC_Int(&d,gUSBStruct);
            intreg[0] &= ~i;

			bHandle = 0x21;
        }
		epnum ++;
		i = i << 1;
	}


	/* EPx RX Interrupt */
	epnum = 1;  i = 2;

	while (intreg[1])
	{
		if (intreg[1] & i)
		{
			//USB_REG_WRITE8(M_REG_INDEX, epnum);
			d.IReg = USB_REG_READ8(0x126*2);//USB_REG_READ8(M_REG_RXCSR1);
			d.IIntSrc = EP_RX;
			d.IEP = epnum;
			d.ICount = USB_REG_READ16(0x128*2);//USB_REG_READ16(M_REG_RXCOUNT);
			//gprintf("EP_RX int\n");
			//printf("d.IReg:0x%x\n",d.IReg);
			//printf("d.ICount:0x%x\n",d.ICount);
			USB_Parse_DRC_Int(&d,gUSBStruct);
			intreg[1] &= ~i;
			bHandle = 0x22;
		}
		epnum ++;
		i = i << 1;
	}

	//USB_REG_WRITE8(M_REG_INDEX, index_save);

	//return bHandle;
}
