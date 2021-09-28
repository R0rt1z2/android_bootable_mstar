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

#ifndef __HAL_USB_FUNCTION_H__
#define __HAL_USB_FUNCTION_H__

#include <hal_usb_drc.h>

#define M_FTR_TESTJ		0x0100
#define M_FTR_TESTK		0x0200
#define M_FTR_TESTSE0		0x0300
#define M_FTR_TESTPKT		0x0400

/* Descriptor Length Define */
#define DEVICE_DESC_LEN			0x12
#define CONFIG_DESC_LEN			0x09
#define OTHERSPEEDCONFIG_DESC_LEN	0x09
#define INTRF_DESC_LEN	 		0x09
#define ENDP_DESC_LEN			0x07

/* ============== Android Device Class Definition ========= */
#define ANDDEV_TOTAL_CONF_LEN		\
		( CONFIG_DESC_LEN +	\
		INTRF_DESC_LEN +	\
		(ENDP_DESC_LEN*2) )

#define	ANDDEV_TOTAL_CONF_LEN_L		(ANDDEV_TOTAL_CONF_LEN & 0x00FF)
#define	ANDDEV_TOTAL_CONF_LEN_H		(ANDDEV_TOTAL_CONF_LEN >> 8)

/* Let the Length of Other Speed Configuration is Identical to Configuration */
#define	ANDDEV_TOTAL_OTHERSPEEDCONF_LEN_L	ANDDEV_TOTAL_CONF_LEN_L
#define	ANDDEV_TOTAL_OTHERSPEEDCONF_LEN_H	ANDDEV_TOTAL_CONF_LEN_H
/* From fastboot client.. */
#define FASTBOOT_INTERFACE_CLASS		0xff
#define FASTBOOT_INTERFACE_SUB_CLASS		0x42
#define FASTBOOT_INTERFACE_PROTOCOL		0x03
/* ============== Android Device Class Definition End========= */

/* Descriptor Type Definition */
#define DEVICE			0x01
#define CONFIG			0x02
#define STRING			0x03
#define INTRF			0x04
#define ENDPT			0x05
#define QUILIFIER		0x06
#define OTHERSPEEDCONFIG	0x07
#define INTERFASSOC		0x0B
#define HID			0x21
#define REPORT			0x22

#define	ENDPOINT_HALT			0x00
#define	DEVICE_REMOTE_WAKEUP		0x01
#define	USB20_TEST_MODE			0x02

#define SETLSB(x,y)	(x) = (((x) & 0xff00) | ((y) & 0xff))
#define SETMSB(x,y)	(x) = (((x) & 0xff) | (((y) & 0xff) << 8))
#define GETLSB(x)	((x) & 0xff)
#define GETMSB(x)	GETLSB((x)>>8)

typedef U8 nEpPipe_t;

void HalUsbFunParseIntPeripheral(drcintitem_t * dP, pUsbVar ptUsb);
void HalUsbFunEPInitial(pUsbVar ptUsb);
s32 HalUsbFunParseReceivedSetup(pUsbVar ptUsb);
#endif	/* _USB_FUNCTION_H_ */
