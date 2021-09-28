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

//-------------------------------------------------------------------------------------------------
//  Include files
//-------------------------------------------------------------------------------------------------
#include <common.h>

#include <net.h>
#include "usb.h"
#include "usb_lan.h"

extern int rtl8150_init(struct eth_device *nic);
extern int pegasus_init(struct eth_device *nic);
extern int ax88772a_init(struct eth_device *nic);

static struct eth_device *gnic = NULL;
typedef int  (*__entry) (struct eth_device*);

struct init_info {
	unsigned short idVendor;
	unsigned short idProduct;
	__entry init;
};

struct init_info ctrlTbl[] =
{
	{VENDOR_ID_REALTEK, PRODUCT_ID_RTL8150, rtl8150_init}
	,{VENDOR_ID_ADMTEK, PRODUCT_ID_ADM8515, pegasus_init}
	,{VENDOR_ID_ASIX, PRODUCT_ID_AX88772A, ax88772a_init}
	,{VENDOR_ID_ASIX, PRODUCT_ID_AX88772B, ax88772a_init}
	,{VENDOR_ID_ASIX, PRODUCT_ID_AX887720, ax88772a_init}
};

#define USBLAN_MAX_INFOTBL  (sizeof(ctrlTbl)/sizeof(struct init_info))

static int lookup_ctrlTbl(u16 idVendor, u16 idProduct)
{
	u8 idx = 0;
	for (idx = 0; idx < USBLAN_MAX_INFOTBL; idx++) {
		if (idVendor == ctrlTbl[idx].idVendor
				&& idProduct == ctrlTbl[idx].idProduct) {
			break;
		}
	}
	return idx;
}

void usb_lan_release(void)
{
	if (gnic) {
		if (gnic->priv) {
			free(gnic->priv);
		}
		free(gnic);
	}
}

void dump_pkt(u8 *pkt, u32 len)
{
	u32 i = 0;
	for (i = 0; i < len; i++) {
		if (i % 0x10 == 0) {
			printf("\n%x: ", (u32)pkt);
		}
		if (*pkt < 0x10) {
			printf("0%x ", *pkt);
		} else {
			printf("%x ", *pkt);
		}
		pkt++;
	}
	printf("\n");
}

int usb_lan_initialize(struct usb_device *udev)
{
	struct usb_lan_hw *hw = NULL;
	struct eth_device *nic = NULL;
	u8 ret = 1;

	nic = (struct eth_device *)malloc(sizeof(struct eth_device));
	if (!nic) {
		printf("[USB] Failed to malloc eth_device\n");
		return -ENOMEM;
	}
	hw = (struct usb_lan_hw *)malloc(sizeof(struct usb_lan_hw));
	if (!hw) {
		printf("[USB] Failed to malloc usb_lan_hw\n");
		free(nic);
		return -ENOMEM;
	}
	nic->udev = udev;
	nic->priv = hw;

	/* Match USB LAN driver */
	printf("[USB] search USB LAN driver for (VID: 0x%x, PID: 0x%x)...",
			nic->udev->descriptor.idVendor,
			nic->udev->descriptor.idProduct);
	hw->tblIdx = lookup_ctrlTbl(nic->udev->descriptor.idVendor,
			nic->udev->descriptor.idProduct);
	if (hw->tblIdx == USBLAN_MAX_INFOTBL) {
		printf("Not found\n");
		free(hw);
		free(nic);
		return -ENXIO;
	}
	printf("Found\n");

	/* Init USB LAN driver */
	if (hw->tblIdx == 0) {
		ret = rtl8150_init(nic);
	} else if (hw->tblIdx == 1) {
		ret = pegasus_init(nic);
	} else {
		ret = ax88772a_init(nic);
	}
	if (!ret) {
		printf("[USB] Failed to init LAN driver\n");
		free(hw);
		free(nic);
		return 0;
	}

	/* Register USB LAN driver */
	eth_register(nic);
	uEMAC_start = 0;
	uUSB_LAN_start = 1;

	gnic = nic; /* For usb_lan_release */
	return 1;
}
