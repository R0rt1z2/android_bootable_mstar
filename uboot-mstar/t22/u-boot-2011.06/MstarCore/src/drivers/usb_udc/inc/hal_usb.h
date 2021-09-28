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

#ifndef	__HAL_USB_H__
#define	__HAL_USB_H__

/* Standard requests */
#define USB_REQ_GET_STATUS		0x00
#define USB_REQ_CLEAR_FEATURE		0x01
#define USB_REQ_SET_FEATURE		0x03
#define USB_REQ_SET_ADDRESS		0x05
#define USB_REQ_GET_DESCRIPTOR		0x06
#define USB_REQ_SET_DESCRIPTOR		0x07
#define USB_REQ_GET_CONFIGURATION	0x08
#define USB_REQ_SET_CONFIGURATION	0x09
#define USB_REQ_GET_INTERFACE		0x0A
#define USB_REQ_SET_INTERFACE		0x0B
#define USB_REQ_SYNCH_FRAME		0x0C

/* USB Packet IDs (PIDs) */
#define USB_PID_OUT		0xe1
#define USB_PID_ACK		0xd2
#define USB_PID_IN		0x69
#define USB_PID_STALL		0x1e

#define USB_ENDPOINT_DIR_MASK		0x80
#define USB_ENDPOINT_XFERTYPE_MASK	0x03	/* in bmAttributes */
#define USB_ENDPOINT_XFER_BULK		2
#define USB_ENDPOINT_XFER_INT		3

#define USB_ST_NOERROR			0
#define USB_ST_NORESPONSE		0xfc
#define USB_ST_STALL			0xec

#define USB_DIR_OUT		0
#define USB_DIR_IN		0x80

#define USB_TYPE_MASK		(0x03 << 5)
#define USB_TYPE_STANDARD	(0x00 << 5)
#define USB_TYPE_CLASS		(0x01 << 5)
#define USB_TYPE_VENDOR		(0x02 << 5)
#define USB_TYPE_RESERVED	(0x03 << 5)

#define USB_RECIP_MASK		0x1f
#define USB_RECIP_DEVICE	0x00
#define USB_RECIP_INTERFACE	0x01
#define USB_RECIP_ENDPOINT	0x02
#define USB_RECIP_OTHER		0x03

#define EP0C_SET_ADDRESS		1	/* 1st enum cmd is to set dev address */
#define EP0C_GET_MAX_EP0		2	/* fetch functions max EP0, 1st enum req */
#define EP0C_GET_STD_DEV_DESC		3	/* 2nd enum req, get std descriptor */
#define EP0C_GET_CONFIG			4	/* read config descriptor only */
#define EP0C_GET_FULL_CONFIG		5	/* read entire config descriptor */
#define EP0C_SET_CONFIG			6	/* set config 0 prior to probe */
#define EP0C_SET_FEATURE		7	/* set a feature, like OTG */
#define EP0C_CLEAR_FEATURE		8	/* set a feature, like OTG */

#define USB_DT_DEVICE			0x01
#define USB_DT_CONFIG			0x02
#define USB_DT_STRING			0x03
#define USB_DT_INTERFACE		0x04
#define USB_DT_ENDPOINT			0x05
#define	USB_DT_DEVICE_QUALIFIER		0x06
#define	USB_DT_OTHER_SPEED		0X07
#define	USB_DT_INTERFACE_POWER		0x08
#define	USB_DT_OTG			0x09
#define USB_DT_DEVICE_SIZE		18
#define USB_DT_DEVICE_QUALIFIER_SIZE	10
#define USB_DT_CONFIG_SIZE		9
#define USB_DT_INTERFACE_SIZE		9
#define USB_DT_ENDPOINT_SIZE		7
#define	USB_DT_OTG_SIZE			3

#define	USB_OTG_SRP		0x01	/* bit 0 of bmAttributes */
#define	USB_OTG_HNP		0x02	/* bit 1 of bmAttributes */

#define BIT_MASK(n)	( ~(~(0UL)<<n) )

#define	ENDPOINT_HALT		0x00

#define usb_nEpPipeout(nEpPipe)		((((nEpPipe) >> PIPEDEF_DIR) & 1) ^ 1)
#define usb_nEpPipein(nEpPipe)		(((nEpPipe) >> PIPEDEF_DIR) & 1)
#define usb_nEpPipeendpoint(nEpPipe)	(((nEpPipe) >> PIPEDEF_EP) & 0xf)
#define usb_nEpPipetype(nEpPipe)	(((nEpPipe) >> PIPEDEF_ATTR) & 3)
#define usb_nEpPipeisoc(nEpPipe)	(usb_nEpPipetype((nEpPipe)) == PIPE_ISOCHRONOUS)
#define usb_nEpPipebulk(nEpPipe)	(usb_nEpPipetype((nEpPipe)) == PIPE_BULK)

#define	usb_sndctrlnEpPipe(endpoint)	((PIPE_CONTROL << PIPEDEF_ATTR) | \
	(PIPE_OUT << PIPEDEF_DIR) | (((endpoint) << PIPEDEF_EP) &0xf))

#define	usb_rcvctrlnEpPipe(endpoint)	((PIPE_CONTROL << PIPEDEF_ATTR) | \
	(PIPE_IN  << PIPEDEF_DIR) | (((endpoint) << PIPEDEF_EP) &0xf))

#define	usb_sndisonEpPipe(endpoint)	((PIPE_ISOCHRONOUS<< PIPEDEF_ATTR) | \
	(PIPE_OUT << PIPEDEF_DIR) | (((endpoint) << PIPEDEF_EP) &0xf))

#define	usb_sndbulknEpPipe(endpoint)	((PIPE_BULK << PIPEDEF_ATTR) | \
	(PIPE_OUT << PIPEDEF_DIR) | (((endpoint) << PIPEDEF_EP) &0xf))

#define	usb_rcvbulknEpPipe(endpoint)	((PIPE_BULK << PIPEDEF_ATTR) | \
	(PIPE_IN << PIPEDEF_DIR) | (((endpoint) << PIPEDEF_EP) &0xf))

/* Device descriptor */
struct usb_device_descriptor
{
	u8	bLength;
	u8	bDescriptorType;
	u16	bcdUSB;
	u8	bDeviceClass;
	u8	bDeviceSubClass;
	u8	bDeviceProtocol;
	u8	bMaxPacketSize0;
	u16	idVendor;
	u16	idProduct;
	u16	bcdDevice;
	u8	iManufacturer;
	u8	iProduct;
	u8	iSerialNumber;
	u8	bNumConfigurations;
};

/* Endpoint descriptor */
typedef struct
{
	u8	bLength;
	u8	bDescriptorType;
	u8	bEndpointAddress;
	u8	bmAttributes;
	u16	wMaxPacketSize;
	u8	bInterval;
	u8	bRefresh;
	u8	bSynchAddress;
	u8*	extra;	/* Extra descriptors */
	s32	extralen;
} usb_endpoint_descriptor;

/* Interface descriptor */
typedef struct
{
	u8	bLength;
	u8	bDescriptorType;
	u8	bInterfaceNumber;
	u8	bAlternateSetting;
	u8	bNumEndpoints;
	u8	bInterfaceClass;
	u8	bInterfaceSubClass;
	u8	bInterfaceProtocol;
	u8	iInterface;
	usb_endpoint_descriptor*	endpoint;
	u8*	extra;	/* Extra descriptors */
	s32	extralen;
} usb_interface_descriptor;

typedef struct
{
	usb_interface_descriptor*	altsetting;
	s32 act_altsetting;			/* active alternate setting */
	s32 num_altsetting;			/* number of alternate settings */
	s32 max_altsetting;			/* total memory allocated */
	struct usb_driver*	driver;	/* driver */
	void*	private_data;
} usb_interface;

/* Configuration descriptor information.. */
typedef struct
{
	u8	bLength;
	u8	bDescriptorType;
	u16	wTotalLength;
	u8	bNumInterfaces;
	u8	bConfigurationValue;
	u8	iConfiguration;
	u8	bmAttributes;
	u8	MaxPower;
	usb_interface*	interface;
	u8*	extra;	/* Extra descriptors */
	s32	extralen;
} usb_config_descriptor;

struct usb_device
{
	u8	core;
	struct usb_device_descriptor	descriptor;	/* Descriptor */
	usb_config_descriptor*	config;			/* All of the configs */
	usb_config_descriptor*	actconfig;		/* the active configuration */
	s8 **rawdescriptors;	/* Raw descriptors for each config */
};

void HalUsbUtilChgUSBState(u8 toUSB, UsbVar_t * g_tUsbStruct);
s8 HalUsbUtilSendStall(u8 ep, UsbVar_t * g_tUsbStruct);
u8 HalUsbUtilIsConfiged(pUsbVar ptUsb);
void HalUsbUtilSwopSetup(struct _DevRequest_t *sP);
#endif	/* _MSUSB_H_ */
