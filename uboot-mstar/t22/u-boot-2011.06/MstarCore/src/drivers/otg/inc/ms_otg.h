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
#ifndef _MS_OTG_H_
#define _MS_OTG_H_

//#include "type.h"
//#include "Globalvar.h"
//#define	AB_IDLE		    0x00
//#define WAIT_VRISE      0x01

//#define A_PERIPHERAL    0x21
//#define A_WAIT_BCON     0x22
//#define A_HOST          0x23
//#define A_SUSPEND       0x24

//#define B_PERIPHERAL    0x11
//#define B_WAIT_ACON     0x12
//#define B_HOST          0x13
//#define B_SRP_INIT      0x14

//#define CID_UNKNOWN    2
//#define CID_A_DEVICE   0
//#define CID_B_DEVICE   1

#define LOW_SPEED       3
#define FULL_SPEED      1
#define HIGH_SPEED      2

#define USB_POWERED     1   /* CONNECT int in, Reset pending */
#define USB_DEFAULT     2   /* CONNECT in, Reset completed */
#define USB_ADDRESS     3   /* Above,+ peripheral SET_ADDRESS completed */
#define USB_CONFIGURED  4   /* Above,+ enumeration completed. OK for traffic*/

#define EP_EQ_TX(x) (!((x) & 0x01)) /* test for TX mode of endpoint */
#define EP_EQ_RX(x) ((x) & 0x01)    /* test for RX mode of endpoint */
#define EP_EQ_ST(x) ((x) & 0x02)    /* test for ST mode of endpoint */
#define EP_TX       0x00            /* set endpoint to TX only, no status */
#define EP_RX       0x01            /* set endpoint to RX only, no status */
#define EP_TXST     0x02            /* set status in TX direction */
#define EP_RXST     0x03            /* set status in RX direction */
#define EP_IDLE     0x02            /* peripheral only! 2nd bit set is idle */

/* INTRUSB, INTRUSBE */
#define M_INTR_VBUSERROR    0x80   /* only valid when A device */
#define M_INTR_SESSREQ      0x40   /* only valid when A device */
#define M_INTR_DISCONNECT   0x20
#define M_INTR_CONNECT      0x10   /* only valid in Host mode */
#define M_INTR_SOF          0x08
#define M_INTR_RESET        0x04
#define M_INTR_BABBLE       0x04
#define M_INTR_RESUME       0x02
#define M_INTR_SUSPEND      0x01   /* only valid in Peripheral mode */

#define	b_hnp_enable			0x03
#define	a_hnp_support			0x04
#define	a_alt_hnp_support		0x05

#define DRCPERIPHERAL(x)    (!((x) & 0x04))//M_DEVCTL_HM

//void USB_Parse_DRC_INTRUSB(U16 intrusb,USB_VAR *gUSBStruct);
//void USB_Change_OTG_State(U8 toOTG,USB_VAR *gUSBStruct);
//void USB_Reset_All_EP_IO_Vars(USB_VAR *gUSBStruct);
#endif
