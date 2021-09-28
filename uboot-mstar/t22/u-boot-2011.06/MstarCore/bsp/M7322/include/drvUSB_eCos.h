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
#ifndef _DRVUSB_H
#define _DRVUSB_H

#include "drvUsbcommon.h"

//
// Function support
//
#define USBC_IP_SUPPORT 1 // over current detection support

#define USB_IF_EHSET_SUPPORT 1 // embeded host hisg speed electrical test procedure

#define USB_CDC_SUPPORT 0 // 1 to turn on cdc support

#define USB_MASS_STORAGE_SUPPORT 1

#define USB_HID_SUPPORT                     0

//
// Extended Function support
//

#define USB_SYSTEM_STR_SUPPORT

//#define USB_NOT_SUPPORT_EX_HUB

//
// Parameter definition
//
#define USBC_OVER_CURRENT 1
#define USBC_NON_OVER_CURRENT 2

#define  USB_PLUG_IN	1
#define  USB_PLUG_OUT	2

#define USB_EVENT_DEV_TYPE_UNKNOW    0
#define USB_EVENT_DEV_TYPE_STOR         1
#define USB_EVENT_DEV_TYPE_HID            2
#define USB_EVENT_DEV_TYPE_CDC          3
#define USB_EVENT_DEV_NO_RESPONSE          4
#define USB_EVENT_DEV_TYPE_VENDOR         5

#define USB_NOT_RESPONSE		 0xff
#define USB_INTERFACE_CLASS_NONE 0
#define USB_INTERFACE_CLASS_MSD  0x08
#define USB_INTERFACE_CLASS_HUB  0x09
#define USB_INTERFACE_CLASS_HID  0x03

typedef void ( *USBCallback ) (MS_U8 u8Event, MS_U32 u32Para, char *s);
// for MDrv_USB_RegisterCallBack()
//u8Event: 1=plug, 2=unplug¡K can extend later);
//u32Para: bit 0 ~ 3, device type (0: unknow, 1: Mass Storage, 2: HID)
//             bit 4 ~ 31, reserved
// for MDrv_OverCurrentDetect_RegisterCallBack()
//u8Event: 1=over current,¡K can extend later);
//u32Para: port number

extern USBCallback _DrvUSB_CBFun;

void MDrv_OverCurrentDetect_RegisterCallBack (USBCallback pCallbackFn, MS_U8 port_mask);
void MDrv_USB_RegisterCallBack (USBCallback pCallbackFn);

void MDrv_Usb_Init(
    mem_Alloc     pfn_Cachedmem_Alloc,
    mem_Free      pfn_Cachedmem_Free,
    mem_Alloc     pfn_NonCachedmem_Alloc,
    mem_Free      pfn_NonCachedmem_Free,
    mem_VA2PA     pfn_mem_VA2PA,
    mem_PA2VA     pfn_mem_PA2VA,
    mem_Cached2Noncached pfn_mem_Cached2Noncached,
    mem_NonCached2Cached pfn_mem_NonCached2Cached
);
MS_BOOL MDrv_USB_Port_Init(MS_U8 u8Hostid);
void MDrv_UsbClose(void);
int MDrv_UsbDeviceConnectBitEx(MS_U8 u8Hostid);
MS_BOOL MDrv_USB_Port_Close(MS_U8 u8Hostid);
#ifdef USB_SYSTEM_STR_SUPPORT
void MDrv_Usb_STR_Off(MS_U8 u8Hostid);
void MDrv_Usb_STR_On(MS_U8 u8Hostid);
#endif

/* Mass Storage class API */
MS_BOOL MDrv_UsbBlockReadToMIU(MS_U8 lun, MS_U32 u32BlockAddr, MS_U32 u32BlockNum, MS_U32 u32MIUAddr);
MS_BOOL MDrv_UsbBlockWriteFromMIU(MS_U8 lun, MS_U32 u32BlockAddr, MS_U32 u32BlockNum, MS_U32 u32MIUAddr);
MS_BOOL MDrv_UsbBlockReadToMIUEx(MS_U8 uPort,MS_U8 lun, MS_U32 u32BlockAddr, MS_U32 u32BlockNum, MS_U32 u32MIUAddr);
MS_BOOL MDrv_UsbBlockWriteFromMIUEx(MS_U8 uPort,MS_U8 lun, MS_U32 u32BlockAddr, MS_U32 u32BlockNum, MS_U32 u32MIUAddr);
MS_U32 MDrv_GetUsbBlockSize(MS_U8 lun);
MS_U32 MDrv_GetUsbBlockSizeEx(MS_U8 uPort, MS_U8 lun);
MS_U32 MDrv_GetUsbBlockNumEx(MS_U8 uPort, MS_U8 lun);
MS_U32 MDrv_GetUsbBlockNum(MS_U8 lun);
MS_U8 MDrv_UsbGetMaxLUNCount(void);
MS_U8 MDrv_UsbGetMaxLUNCountEx(MS_U8 uPort);
MS_BOOL MDrv_UsbIsLunConnected(MS_U8 uPort, MS_U8 lun);

MS_BOOL ChkUsbReady(void); // Obsolete
MS_BOOL ChkUsbReadyEx(MS_U8 uPort); // Obsolete

/* Lookup USB information by disk path */
MS_U8 MDrv_USB_MscLookupHostID(char *str);
char *MDrv_USB_MscLookupManufacturerString(char *str);
char *MDrv_USB_MscLookupProductString(char *str);
char *MDrv_USB_MscLookupSerialNumberString(char *str);
MS_U32 MDrv_USB_MscLookupVidPid(char *str);
MS_BOOL MDrv_USB_Force_HDD_Standby(char *str, int attr);
int MDrv_USB_Quirk_Add_3G(U32 vid_pid);

/* MSC IOCTL implement ver:001 20151023 */
enum usb_ioctl_cmd {
    USB_IOCTL_CTRL,
    USB_IOCTL_BULK
};

enum usb_ioctl_bulk_dir {
    USB_IOCTL_BULK_IN,
    USB_IOCTL_BULK_OUT
};

struct usb_ioctl_ctrl_xfer{
    U8 bRequestType;
    U8 bRequest;
    U16 wValue;
    U16 wIndex;
    U16 wLength;
    U32 timeout;  /* in ms */
    void *pDataBuf;
};

struct usb_ioctl_bulk_xfer {
    enum usb_ioctl_bulk_dir dir;
    U32 len;
    void *pDataBuf;
};

int MDrv_USB_IOCTL_Cmd(char *strDevPath, enum usb_ioctl_cmd cmd_type, void *pdata);
#endif
