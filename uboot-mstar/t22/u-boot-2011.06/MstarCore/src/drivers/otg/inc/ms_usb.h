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
#ifndef _MSUSB_H_
#define _MSUSB_H_

/*
 * Standard requests
 */
#define USB_REQ_GET_STATUS                  0x00
#define USB_REQ_CLEAR_FEATURE           0x01
#define USB_REQ_SET_FEATURE                 0x03
#define USB_REQ_SET_ADDRESS                 0x05
#define USB_REQ_GET_DESCRIPTOR          0x06
#define USB_REQ_SET_DESCRIPTOR          0x07
#define USB_REQ_GET_CONFIGURATION       0x08
#define USB_REQ_SET_CONFIGURATION       0x09
#define USB_REQ_GET_INTERFACE           0x0A
#define USB_REQ_SET_INTERFACE           0x0B
#define USB_REQ_SYNCH_FRAME                 0x0C

#define usb_pipeout(pipe)       ((((pipe) >> PIPEDEF_DIR) & 1) ^ 1)
#define usb_pipein(pipe)        (((pipe) >> PIPEDEF_DIR) & 1)
#define usb_pipeendpoint(pipe)  (((pipe) >> PIPEDEF_EP) & 0xf)
#define usb_pipetype(pipe)      (((pipe) >> PIPEDEF_ATTR) & 3)
//#define usb_pipeisoc(pipe)      (usb_pipetype((pipe)) == PIPE_ISOCHRONOUS)
#define usb_pipebulk(pipe)      (usb_pipetype((pipe)) == PIPE_BULK)

#define usb_sndctrlpipe(endpoint)       ((PIPE_CONTROL << PIPEDEF_ATTR) | \
        (PIPE_OUT << PIPEDEF_DIR) | (((endpoint) << PIPEDEF_EP) &0xf))

#define usb_rcvctrlpipe(endpoint)       ((PIPE_CONTROL << PIPEDEF_ATTR) | \
        (PIPE_IN  << PIPEDEF_DIR) | (((endpoint) << PIPEDEF_EP) &0xf))

#define usb_sndbulkpipe(endpoint)       ((PIPE_BULK << PIPEDEF_ATTR) | \
        (PIPE_OUT << PIPEDEF_DIR) | (((endpoint) << PIPEDEF_EP) &0xf))

#define usb_rcvbulkpipe(endpoint)       ((PIPE_BULK << PIPEDEF_ATTR) | \
        (PIPE_IN << PIPEDEF_DIR) | (((endpoint) << PIPEDEF_EP) &0xf))

#define MAX_BITS_BYTE    (8)
#define MAX_BITS_SHORT   (16)
#define MAX_BITS_3BYTE   (24)
#define MAX_BITS_INT     (32)

#define POSITION_VALUE_8    (0x100)
#define POSITION_VALUE_16   (U32)(0x10000)
#define POSITION_VALUE_24   (U32)(0x1000000)

#define BIT_MASK(n)    ( ~(~(0L)<<n) )

#define USB_PID_OUT                 0xe1
#define USB_PID_ACK                 0xd2
#define USB_PID_IN                  0x69
#define USB_PID_STALL               0x1e

#define USB_ENDPOINT_DIR_MASK           0x80
#define USB_ENDPOINT_XFERTYPE_MASK      0x03    /* in bmAttributes */
#define USB_ENDPOINT_XFER_BULK          2
//#define USB_ENDPOINT_XFER_INT           3

#define USB_ST_NOERROR                  0
#define USB_ST_NORESPONSE               0xfc
#define USB_ST_STALL                    0xec

#define USB_DIR_OUT                         0
#define USB_DIR_IN                          0x80

#define USB_TYPE_MASK                   (0x03 << 5)
#define USB_TYPE_STANDARD               (0x00 << 5)
#define USB_TYPE_CLASS                  (0x01 << 5)
#define USB_TYPE_VENDOR                 (0x02 << 5)
#define USB_TYPE_RESERVED               (0x03 << 5)

#define USB_RECIP_MASK                  0x1f
#define USB_RECIP_DEVICE                0x00
#define USB_RECIP_INTERFACE             0x01
#define USB_RECIP_ENDPOINT              0x02
#define USB_RECIP_OTHER                 0x03

#define EP0C_SET_ADDRESS        1   /* 1st enum cmd is to set dev address */
#define EP0C_GET_MAX_EP0        2   /* fetch functions max EP0, 1st enum req */
#define EP0C_GET_STD_DEV_DESC   3   /* 2nd enum req, get std descriptor */
#define EP0C_GET_CONFIG         4   /* read config descriptor only */
#define EP0C_GET_FULL_CONFIG    5   /* read entire config descriptor */
#define EP0C_SET_CONFIG         6   /* set config 0 prior to probe */
#define EP0C_SET_FEATURE        7   /* set a feature, like OTG */
#define EP0C_CLEAR_FEATURE        8   /* set a feature, like OTG */

#define USB_DT_DEVICE                   0x01
#define USB_DT_CONFIG                   0x02
#define USB_DT_STRING                   0x03
#define USB_DT_INTERFACE                0x04
#define USB_DT_ENDPOINT                 0x05
#define USB_DT_DEVICE_QUALIFIER         0x06
#define USB_DT_OTHER_SPEED                  0X07
#define USB_DT_INTERFACE_POWER          0x08
#define USB_DT_OTG                              0x09
#define USB_DT_DEVICE_SIZE                   18
#define USB_DT_DEVICE_QUALIFIER_SIZE 10
#define USB_DT_CONFIG_SIZE                    9
#define USB_DT_INTERFACE_SIZE             9
#define USB_DT_ENDPOINT_SIZE              7
#define USB_DT_OTG_SIZE                       3
#endif
