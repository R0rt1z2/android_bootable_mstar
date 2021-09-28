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
#include "../inc/ms_msd_global.h"
#include "../inc/ms_drc.h"
#include "../inc/Globalvar.h"

#if 1
const U8 Msdfn_Dscr[]=
{
   /* Device Descriptor */
   0x12,									/* bLength				*/
   0x01,									/* DEVICE				*/
   0x00,0x02,								/* USB 2.0				*/
   0x00,									/* CLASS				*/
   0x00,									/* Subclass 			*/
   0x00,									/* Protocol 			*/
   0x40,									/* bMaxPktSize0 		*/
   0x20,0x1B,								/* idVendor 			*/
   0x00,0x03,								/* idProduct			*/
   0x00,0x01,								/* bcdDevice			*/
   0x00,									/* iManufacturer		*/
   0x00,									/* iProduct 			*/
   0x00,									/* iSerial Number		*/
   0x01,									/* One configuration	*/
   /* Device Qualifier */
   0x0a,					  /* bLength			  */
   0x06,					  /* DEVICE Qualifier	  */
   0x00,0x02,				  /* USB 2.0			  */
   0x00,					  /* CLASS				  */
   0x00,					  /* Subclass			  */
   0x00,					  /* Protocol			  */
   0x40,					  /* bMaxPktSize0		  */
   0x01,					  /* One configuration	  */
   0x00,					  /* Reserved			  */
#if 0
   /* Other Speed Configuration Descriptor */
   0x09,					  /* bLength				   */
   0x07,					  /* Other Speed Configuration */
   0x20,					  /* length 				   */
   0x0, 					  /* length 				   */
   0x01,					  /* bNumInterfaces 		   */
   0x01,					  /* bConfigurationValue	   */
   0x00,					  /* iConfiguration 		   */
   0xC0,					  /* bmAttributes			   */
   0,						  /* power					   */
#endif
   /* Configuration Descriptor */
   0x09,					  /* bLength			  */
   0x02,					  /* CONFIGURATION		  */
   0x20,					  /* length_L			  */
   0x00,					  /* length_H			  */
   0x01,					  /* bNumInterfaces 	  */
   0x01,					  /* bConfigurationValue  */
   0x00,					  /* iConfiguration 	  */
   0xC0,					  /* bmAttributes		  */
   0x00,					  /* power 500ma		  */
   /* Interface Descriptor */
   0x09,								   /* bLength			   */
   0x04,								   /* INTERFACE 		   */
   0x0, 								   /* bInterfaceNumber	   */
   0x0, 								   /* bAlternateSetting    */
   0x02,								   /* bNumEndpoints 	   */
   0x08,								   /* bInterfaceClass	   */
   0x06,								   /* bInterfaceSubClass   */
   0x50,								   /* bInterfaceProtocol   */
   0x00,								   /* iInterface		   */
   /* Endpoint Descriptor  : Bulk-In */
   0x07,								   /* bLength			   */
   0x05,								   /* ENDPOINT			   */
   0x81,								   /* bEndpointAddress	   */
   0x02,								   /* bmAttributes		   */
   0x00, 0x02,							   /* wMaxPacketSize	   */
   0x00,								   /* bInterval 		   */
   /* Endpoint Descriptor  : Bulk-Out */
   0x07,								   /* bLength			   */
   0x05,								   /* ENDPOINT			   */
   0x02,								   /* bEndpointAddress	   */
   0x02,								   /* bmAttributes		   */
   0x00, 0x02,							  /* wMaxPacketSize 	  */
   0x00,								   /* bInterval 		   */
#if 0
   //USB_DT_OTG_SIZE,
   //USB_DT_OTG,
   /* Configuration Descriptor */
   0x09,								   /* bLength			   */
   0x02,								   /* CONFIGURATION 	   */
   0x20,								   /* length			   */
   0x0, 								   /* length			   */
   0x01,								   /* bNumInterfaces	   */
   0x01,								   /* bConfigurationValue  */
   0x00,								   /* iConfiguration	   */
   0xC0,								   /* bmAttributes		   */
   0x0, 								   /* power 			   */
   /* Interface Descriptor */
   0x09,								   /* bLength			   */
   0x04,								   /* INTERFACE 		   */
   0x0, 								   /* bInterfaceNumber	   */
   0x0, 								   /* bAlternateSetting    */
   0x02,								   /* bNumEndpoints 	   */
   0x08,								   /* bInterfaceClass	   */
   0x06,								   /* bInterfaceSubClass   */
   0x50,								   /* bInterfaceProtocol   */
   0x00,								   /* iInterface		   */
   /* Endpoint Descriptor  : Bulk-In */
   0x07,								   /* bLength			   */
   0x05,								   /* ENDPOINT			   */
   0x81,								   /* bEndpointAddress	   */
   0x02,								   /* bmAttributes		   */
   0x40, 0x00,							   /* wMaxPacketSize	   */
   0x00,								   /* bInterval 		   */
   /* Endpoint Descriptor  : Bulk-Out */
   0x07,								   /* bLength			   */
   0x05,								   /* ENDPOINT			   */
   0x02,								   /* bEndpointAddress	   */
   0x02,								   /* bmAttributes		   */
   0x40, 0x00,							   /* wMaxPacketSize	   */
   0x00,
   //USB_DT_OTG_SIZE,
   //USB_DT_OTG,
#endif
 /* string descritor*/
 /*langID*/
   0x04, 0x03, 0x09, 0x04,
/*DevID*/
  0x28, 0x03, 'M', 0x00, 'a', 0x00, 's', 0x00, 's', 0x00, 0x20, 0x00, 'S', 0x00, 't', 0x00,
  'o', 0x00, 'r', 0x00, 'a', 0x00, 'g', 0x00, 'e', 0x00, 0x20, 0x00, 'D', 0x00, 'e', 0x00,
  'v', 0x00, 'i', 0x00, 'c', 0x00, 'e', 0x00,
 /*iserID*/
  0x12, 0x03, 'B', 0x00, 'i', 0x00, 'g', 0x00,
  'B', 0x00, 'l', 0x00, 'u', 0x00, 'e', 0x00,
  '3', 0x00
};
#endif


void *otg_memset(void *s, int c, unsigned int n)
{
    unsigned char* p=s;
    while(n--)
        *p++ = (unsigned char)c;
    return s;
}

void Init_vendorCmd_Var(vendorCmdStruct *pCmdStruct)
{
    otg_memset(pCmdStruct, 0x00, sizeof(vendorCmdStruct));
}

void otgInitMassBuffer(USB_VAR *gUSBStruct)
{
#ifdef DMA
	USB_REG_WRITE16(0x80*2,0x1401);
	USB_REG_WRITE16(0x82*2,0xc000);
	USB_REG_WRITE16(0x84*2,0x000c);
	USB_REG_WRITE16(0x86*2,0xa002);
	USB_REG_WRITE16(0x88*2,0x1400);
	USB_REG_WRITE16(0x8a*2,0x2000);
	USB_REG_WRITE16(0x8c*2,0x7900);
	USB_REG_WRITE16(0x8e*2,0x8611);
	USB_REG_WRITE16(0x124*2, 0x200);
#endif
    gUSBStruct->otgCSW_Addr = (U32)&gUSBStruct->UsbCSWBuf[0];
    gUSBStruct->otgSetupCmdBuf_Addr = (U32)&gUSBStruct->UsbSetupCmdBuf[0];
    gUSBStruct->otgCBWCB_Addr = (U32)&gUSBStruct->UsbCBWBuf[0];
}

void Init_gUSB_Var(USB_VAR *gUSBStruct, vendorCmdStruct *pCmdStruct)
{
	otg_memset(gUSBStruct, 0x00, sizeof(USB_VAR));
	gUSBStruct->otgMyDeviceHNPSupport = 1;
	gUSBStruct->free_dma_channels = 0x7F;
	gUSBStruct->USB_Msdfn_Dscr = (U8 *)(Msdfn_Dscr);
	gUSBStruct->SizeofUSB_Msdfn_Dscr = (U32)sizeof(Msdfn_Dscr);
	gUSBStruct->ptrVCmdStruct = pCmdStruct;
}
