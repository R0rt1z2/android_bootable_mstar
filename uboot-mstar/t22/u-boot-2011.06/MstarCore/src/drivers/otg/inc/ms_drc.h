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
#ifndef _MS_MSD_DRC_H_
#define _MS_MSD_DRC_H_

//#include "ms_config.h"
#include <ms_config.h>


#define DRC_IN8(r)       *((U8 volatile *)(r))
#define DRC_OUT8(r,d)    (*((U8 volatile*)(r)) = (U8)d)
#define DRC_IN16(r)       *((U16 volatile*)(r))
#define DRC_OUT16(r,d)    (*((U16 volatile*)(r)) = (U16)d)


#define USB_REG_WRITE8(r,v)    (DRC_OUT8(otgRegAddress + (r),(v)))
#define USB_REG_WRITE16(r,v)    (DRC_OUT16(otgRegAddress + (r),(v)))
#define USB_REG_READ8(r)    (DRC_IN8(otgRegAddress + (r)))
#define USB_REG_READ16(r)    (DRC_IN16(otgRegAddress + (r)))
#define UTMI_REG_WRITE8(r,v)   (DRC_OUT8(UTMIBaseAddress + (r),(v)))
#define UTMI_REG_WRITE16(r,v)  (DRC_OUT16(UTMIBaseAddress + (r),(v)))
#define UTMI_REG_READ8(r)      (DRC_IN8(UTMIBaseAddress + (r)))
#define UTMI_REG_READ16(r)     (DRC_IN16(UTMIBaseAddress + (r)))
#define USBC_REG_WRITE8(r,v)   (DRC_OUT8(USBCBaseAddress + (r),(v)))
#define USBC_REG_WRITE16(r,v)  (DRC_OUT16(USBCBaseAddress + (r),(v)))
#define USBC_REG_READ8(r)      (DRC_IN8(USBCBaseAddress + (r)))
#define USBC_REG_READ16(r)     (DRC_IN16(USBCBaseAddress + (r)))


#define M_REG_FADDR        (0x00)   /* 8 bit */
#define M_REG_POWER        (0x01)   /* 8 bit */
#define M_REG_INTRTX       (0x02<<OffShift)
#define M_REG_INTRRX       (0x04<<OffShift)
#define M_REG_INTRTXE      (0x06<<OffShift)
#define M_REG_INTRRXE      (0x08<<OffShift)
#define M_REG_INTRUSB      (0x0A<<OffShift)     /* 8 bit */
#define M_REG_INTRUSBE     ((0x0A<<OffShift)+1) /* 8 bit */
#define M_REG_FRAME        (0x0C<<OffShift)
#define M_REG_INDEX        (0x0E<<OffShift)     /* 8 bit */
#define M_REG_TESTMODE     ((0x0E<<OffShift)+1) /* 8 bit */
#define M_REG_TARGET_FUNCTION_BASE     (0x80<<OffShift)   /* 8 bit */
#define M_REG_TXMAXP       (0x10<<OffShift)
#define M_REG_CSR0         (0x12<<OffShift)
#define M_REG_TXCSR        (0x12<<OffShift)
#define M_REG_RXMAXP       (0x14<<OffShift)
#define M_REG_RXCSR        (0x16<<OffShift)
#define M_REG_COUNT0       (0x18<<OffShift)
#define M_REG_RXCOUNT      (0x18<<OffShift)
#define M_REG_TXTYPE       (0x1A<<OffShift)    /* 8 bit, only valid in Host mode */
#define M_REG_TYPE0        (0x1A<<OffShift)    /* 2 bit, only valid in MDRC Host mode */
#define	M_REG_NAKLIMIT0	   ((0x1A<<OffShift)+1)    /* 8 bit, only valid in Host mode */
#define M_REG_TXINTERVAL   ((0x1A<<OffShift)+1)    /* 8 bit, only valid in Host mode */
#define M_REG_RXTYPE       (0x1C<<OffShift)    /* 8 bit, only valid in Host mode */
#define M_REG_RXINTERVAL   ((0x1C<<OffShift)+1)    /* 8 bit, only valid in Host mode */
#define M_REG_CONFIGDATA   (0x1F<<OffShift)    /* 8 bit */
#define M_REG_FIFOSIZE     (0x1F<<OffShift)    /* 8 bit */

#define M_FIFO_EP0         (0x20<<OffShift)
#define	M_REG_DEVCTL       (0x60<<OffShift)	   /* 8 bit */
#define M_REG_TXFIFOSZ     (0x62<<OffShift)    /* 8 bit, TxFIFO size */
#define M_REG_RXFIFOSZ     ((0x62<<OffShift)+1)    /* 8 bit, RxFIFO size */
#define M_REG_TXFIFOADD    (0x64<<OffShift)    /* 16 bit, TxFIFO address */
#define M_REG_RXFIFOADD    (0x66<<OffShift)    /* 16 bit, RxFIFO address */
#define M_REG_EP_BULKOUT   (0x86 << OffShift)
#define M_REG_FIFO_DATA_PORT  (0x88 << OffShift)
#define M_REG_DMA_MODE_CTL    (0x8A << OffShift)
//#ifdef  CYGHWR_MSTAR_PLATFORM_BIG2
#define M_REG_CFG6_H        ((0x8C << OffShift)+1)


#define M_REG_USB_AHB      ((0x8E << OffShift)+1)
#define M_REG_CFG1_L       (0x82 << OffShift)
#define M_REG_CFG2_L       (0x84 << OffShift)
#define M_REG_INTRTX1      (0x02<<OffShift)   /* 8 bit */
#define M_REG_INTRTX2      (0x03<<OffShift)   /* 8 bit */
#define M_REG_INTRRX1      (0x04<<OffShift)   /* 8 bit */
#define M_REG_INTRRX2      (0x05<<OffShift)   /* 8 bit */
#define M_REG_INTRTX1E     (0x06<<OffShift)   /* 8 bit */
#define M_REG_INTRTX2E     ((0x06<<OffShift)+1)   /* 8 bit */
#define M_REG_INTRRX1E     (0x08<<OffShift)   /* 8 bit */
#define M_REG_INTRRX2E     ((0x08<<OffShift)+1)   /* 8 bit */
#define M_REG_TXCSR1       (0x12<<OffShift)
#define M_REG_TXCSR2       ((0x12<<OffShift)+1)
#define M_REG_RXCSR1       (0x126<<OffShift)
#define M_REG_RXCSR2       ((0x126<<OffShift)+1)


/* POWER */
#define M_POWER_ISOUPDATE   0x80
#define	M_POWER_SOFTCONN    0x40
#define	M_POWER_HSENAB	    0x20
#define	M_POWER_HSMODE	    0x10
#define M_POWER_RESET       0x08
#define M_POWER_RESUME      0x04
#define M_POWER_SUSPENDM    0x02
#define M_POWER_ENSUSPEND   0x01

/* TESTMODE */
#define M_TEST_FIFOACCESS   0x40
#define M_TEST_FORCEFS      0x20
#define M_TEST_FORCEHS      0x10
#define M_TEST_PACKET       0x08
#define M_TEST_K            0x04
#define M_TEST_J            0x02
#define M_TEST_SE0_NAK      0x01

/* DEVCTL */
#define M_DEVCTL_BDEVICE    0x80
#define M_DEVCTL_FSDEV      0x40
#define M_DEVCTL_LSDEV      0x20
#define M_DEVCTL_HM         0x04
#define M_DEVCTL_HR         0x02
#define M_DEVCTL_SESSION    0x01

/* CSR0 in Peripheral and Host mode */
#define	M_CSR0_FLUSHFIFO      0x0100
#define M_CSR0_TXPKTRDY       0x0002
#define M_CSR0_RXPKTRDY       0x0001

/* CSR0 in HSFC */
#define M_CSR0_INPKTRDY       0x02
#define M_CSR0_OUTPKTRDY      0x01

/* CSR0 in Peripheral mode */
#define M_CSR0_P_SVDSETUPEND  0x0080
#define M_CSR0_P_SVDRXPKTRDY  0x0040
#define M_CSR0_P_SENDSTALL    0x0020
#define M_CSR0_P_SETUPEND     0x0010
#define M_CSR0_P_DATAEND      0x0008
#define M_CSR0_P_SENTSTALL    0x0004

/* CSR0 in Host mode */
#define	M_CSR0_H_NAKTIMEOUT   0x0080
#define M_CSR0_H_STATUSPKT    0x0040
#define M_CSR0_H_REQPKT       0x0020
#define M_CSR0_H_ERROR        0x0010
#define M_CSR0_H_SETUPPKT     0x0008
#define M_CSR0_H_RXSTALL      0x0004

/* TXCSR in Peripheral and Host mode */
#define M_TXCSR_AUTOSET       0x8000
#define M_TXCSR_ISO           0x4000
#define M_TXCSR_MODE          0x2000
#define M_TXCSR_DMAENAB       0x1000
#define M_TXCSR_FRCDATATOG    0x0800
#define M_TXCSR_DMAMODE       0x0400
#define M_TXCSR_CLRDATATOG    0x0040
#define M_TXCSR_FLUSHFIFO     0x0008
#define M_TXCSR_FIFONOTEMPTY  0x0002
#define M_TXCSR_TXPKTRDY      0x0001

/* TXCSR in Peripheral mode */
#define M_TXCSR_P_INCOMPTX    0x0080
#define M_TXCSR_P_SENTSTALL   0x0020
#define M_TXCSR_P_SENDSTALL   0x0010
#define M_TXCSR_P_UNDERRUN    0x0004

/* TXCSR in Host mode */
#define M_TXCSR_H_NAKTIMEOUT  0x0080
#define M_TXCSR_H_RXSTALL     0x0020
#define M_TXCSR_H_ERROR       0x0004

/* RXCSR in Peripheral and Host mode */
#define M_RXCSR_AUTOCLEAR     0x8000
#define M_RXCSR_DMAENAB       0x2000
#define M_RXCSR_DISNYET       0x1000
#define M_RXCSR_DMAMODE       0x0800
#define M_RXCSR_INCOMPRX      0x0100
#define M_RXCSR_CLRDATATOG    0x0080
#define M_RXCSR_FLUSHFIFO     0x0010
#define M_RXCSR_DATAERROR     0x0008
#define M_RXCSR_FIFOFULL      0x0002
#define M_RXCSR_RXPKTRDY      0x0001

/* RXCSR in Peripheral mode */
#define M_RXCSR_P_ISO         0x4000
#define M_RXCSR_P_SENTSTALL   0x0040
#define M_RXCSR_P_SENDSTALL   0x0020
#define M_RXCSR_P_OVERRUN     0x0004

/* TXCSR in Peripheral and Host mode */
#define M_TXCSR2_AUTOSET       0x80
#define M_TXCSR2_ISO           0x40
#define M_TXCSR2_MODE          0x20
#define M_TXCSR2_DMAENAB       0x10
#define M_TXCSR2_FRCDATATOG    0x08
#define M_TXCSR2_DMAMODE       0x04
#define M_TXCSR1_CLRDATATOG    0x40
#define M_TXCSR1_FLUSHFIFO     0x08
#define M_TXCSR1_FIFONOTEMPTY  0x02
#define M_TXCSR1_TXPKTRDY      0x01

/* TXCSR in Peripheral mode */
#define M_TXCSR1_P_INCOMPTX    0x80
#define M_TXCSR1_P_SENTSTALL   0x20
#define M_TXCSR1_P_SENDSTALL   0x10
#define M_TXCSR1_P_UNDERRUN    0x04

/* TXCSR in Host mode */
#define M_TXCSR1_H_NAKTIMEOUT  0x80
#define M_TXCSR1_H_RXSTALL     0x20
#define M_TXCSR1_H_ERROR       0x04

/* RXCSR in Peripheral and Host mode */
#define M_RXCSR2_AUTOCLEAR     0x80
#define M_RXCSR2_DMAENAB       0x20
#define M_RXCSR2_DISNYET       0x10
#define M_RXCSR2_DMAMODE       0x08
#define M_RXCSR2_INCOMPRX      0x01
#define M_RXCSR1_CLRDATATOG    0x80
#define M_RXCSR1_FLUSHFIFO     0x10
#define M_RXCSR1_DATAERROR     0x08
#define M_RXCSR1_FIFOFULL      0x02
#define M_RXCSR1_RXPKTRDY      0x01

/* RXCSR in Peripheral mode */
#define M_RXCSR2_P_ISO         0x40
#define M_RXCSR1_P_SENTSTALL   0x40
#define M_RXCSR1_P_SENDSTALL   0x20
#define M_RXCSR1_P_OVERRUN     0x04

/* RXCSR in Host mode */
#define M_RXCSR2_H_AUTOREQ     0x40
#define M_RXCSR1_H_RXSTALL     0x40
#define M_RXCSR1_H_REQPKT      0x20
#define M_RXCSR1_H_ERROR       0x04

/* new mode1 in Peripheral mode */
#define M_Mode1_P_BulkOut_EP   0x0002
#define M_Mode1_P_OK2Rcv   0x8000
#define M_Mode1_P_AllowAck   0x4000
#define M_Mode1_P_Enable    0x2000
#define PIPE_CONTROL			0
//#define PIPE_ISOCHRONOUS		1
#define PIPE_BULK			    2
//#define PIPE_INTERRUPT			3
#define	PIPE_IN				1	/* direction */
#define	PIPE_OUT			0
#define	PIPEDEF_DIR	        7	/* 1 is IN, 0 is OUT */
#define	PIPEDEF_ATTR	    4	/* 00=Control,01=ISO,10=BULK,11=INT */
#define	PIPEDEF_EP	        0	/* endpoints 1 - 15 */

#define VBUS_MASK            0x18    /* DevCtl D4 - D3 */

#define VBUS_BELOW_SESSION_END  0
#define VBUS_ABOVE_SESSION_END  1
#define VBUS_ABOVE_AVALID       2
#define VBUS_ABOVE_VBUS_VALID   3
#define VBUS_ERROR              256

#define USB_Read_RxCount()   USB_REG_READ16(0x128*2);//USB_REG_READ16(M_REG_RXCOUNT);
//#define A_DEVICE()           (gUSBStruct->otgcid == CID_A_DEVICE)
//#define B_DEVICE()           (gUSBStruct->otgcid == CID_B_DEVICE)
//#define VBUS_MASK            0x18    /* DevCtl D4 - D3 */
//#define MIN(x,y)    (((x) < (y)) ? (x) : (y))

#define     FIFO_TX             0           /* endpoint fifo is TX only */
#define     FIFO_RX             1           /* endpoint fifo is RX only */
#define     FIFO_DUAL           2           /* endpoint fifo is TX or RX */
#define     FIFO_DPB            16          /* double packet buffering */
#define     FIFO_ERROR          256

typedef    struct
{
    U16  IReg;     /* holds content of INTRUSB|CSR0|TXCSR1|RXCSR1 */
    U16  ICount;   /* 8-bit COUNT holds bits 0-7 for RX EP */
    U8   IIntSrc;  /* 0=TXInt,1=Rxint for EP interrupts */
    U8   ICause;   /* 0=INTRUSB, 1=EP */
    U8   InUse;    /* 0=free,1=holds interrupt data */
    U8   IDMA;     /* set if DMA interrupt */
    U8   IDummy;   /* unclaimed,use at will */
    U8   IEP;      /* identifies the EP when EP is source of int */
}   drcintitem_t;

#define IRC_INTRUSB 0   /* 2 possible cause codes:INTRUSB ... */
#define IRC_EP      1   /* or an endpoint */


void USB_DRC_Index_Select(U8 epnum);
void USB_DRC_Fifo_Read(U8 *dstP,U8 ep,USB_VAR *gUSBStruct);
u8 OTG_Interrupt_Polling(u8 vec, USB_VAR *gUSBStruct);
void USB_ReSet_ClrRXMode1(void);
void USB_Change_USB_State(U8 toUSB,USB_VAR *gUSBStruct);
void USB_SWOP_Setup(struct devrequest *sP);
void USB_Enable_EP_DRC(U8 ep,USB_VAR *gUSBStruct);
void USB_DRC_Fifo_Write(U8 *srcP,U8 ep,USB_VAR *gUSBStruct);
void USB_Setup_Endpoint(S8 epnum,S8 epdir,S8 eptype,S8 epinf ,S16 epmaxps,USB_VAR *gUSBStruct);
void USB_Parse_DRC_Int(drcintitem_t *dP,USB_VAR *gUSBStruct);
U8  USB_Ret_Blt_EP_Object(U8 drcdir);
S8 USB_Send_Stall(U8 ep,USB_VAR *gUSBStruct);
U8 USB2SDRAM_Bulk(U32 rxaddr,U32 rxsize,USB_VAR *gUSBStruct);
void USB_Set_DRC_Interrupts(void);
U8 otgIsUSBConfiged(USB_VAR *gUSBStruct);
void Init_vendorCmd_Var(vendorCmdStruct *pCmdStruct);
void Init_gUSB_Var(USB_VAR *gUSBStruct, vendorCmdStruct *pCmdStruct);
void otgInitMassBuffer(USB_VAR *gUSBStruct);
void USB_DRC_Interrupt(USB_VAR *gUSBStruct);
void USB_Set_ClrRXMode1(void);
U8 SDRAM2USB_Bulk(U32 txaddr,U32 txsize,USB_VAR *gUSBStruct);
void USB_Parse_DRC_INTRUSB(U16 intrusb,USB_VAR *gUSBStruct);
#endif
