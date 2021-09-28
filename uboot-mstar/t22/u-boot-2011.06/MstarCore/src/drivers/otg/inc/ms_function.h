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
#ifndef _USB_FUNCTION_H_
#define _USB_FUNCTION_H_

//#include "type.h"

#define M_FTR_TESTJ                     0x0100
#define M_FTR_TESTK                     0x0200
#define M_FTR_TESTSE0                   0x0300
#define M_FTR_TESTPKT                   0x0400

/* MSD BOT Functional Characteristics */
#define MSDFN_BOT_RESET                 0xFF
#define MSDFN_BOT_GET_MAXLUN            0xFE

#define DEV_DESC_EP0_SIZE_OFFSET    8   /* where to find max ep0 in dscrptor*/
#define SETLSB(x,y) (x) = (((x) & 0xff00) | ((y) & 0xff))
#define SETMSB(x,y) (x) = (((x) & 0xff) | (((y) & 0xff) << 8))
#define GETLSB(x)   ((x) & 0xff)
#define GETMSB(x)   GETLSB((x)>>8)

/*
 * USB Set Features
 */
#define	ENDPOINT_HALT			        0x00
#define	DEVICE_REMOTE_WAKEUP		    0x01
#define	TEST_MODE			            0x02

#define USB_SUCCESS                         0x00
#define USB_FAILURE                         -1

typedef	U8	pipe_t;

void USB_Parse_DRC_Int_Peripheral(drcintitem_t *dP,USB_VAR *gUSBStruct);
S32 USB_Parse_Received_Setup(USB_VAR *gUSBStruct);
#endif
