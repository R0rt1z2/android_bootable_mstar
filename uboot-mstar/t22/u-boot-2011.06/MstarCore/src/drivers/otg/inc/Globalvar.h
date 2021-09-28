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
#ifndef _GOLBALVAR_H
#define _GOLBALVAR_H

#include "type.h"

#define MIN(_a_, _b_)               ((_a_) < (_b_) ? (_a_) : (_b_))

//#define SYSNUM_DMA_BUFFER_SIZE			(8 * 1024)
//#define SYSNUM_TEMP_BUFFER_SIZE			(8 * 1024)
#define DMA

typedef enum
{
    E_USB_MassStorage = 0,
    //E_USB_VirtCOM = 1,
    //E_USB_Modem = 2,
    //E_USB_PCCAM = 3,
    //E_USB_PictureBridge = 4,
    //E_USB_OTG=5
} USB_DEVICE_MODE;

struct devrequest{
	U8 bmRequestType;
	U8 bRequest;
	U16 wValue;
	U16 wIndex;
	U16 wLength;
};

typedef struct
{
    U16   FIFOSize;
    U16   MaxEPSize;
    U16   FifoRemain;
    U32   BytesRequested;
    U32   BytesProcessed;
    U8    DRCInterval;
    U8    intr_flag;
    U8    pipe;
    U8    BltEP;
    U8    DRCDir;
    U8    LastPacket;
    U8    IOState;
    U8    Halted;
    U8    Infnum;
    U32   transfer_buffer;
    U32   transfer_buffer_length;
    U8     BltEP2;
}   endpoint_t;

typedef struct vendorCmdStruct_t
{
    U32  totDwldBytes;
    U32  currDwldBytes;
    U32  currUpldBytes;
    U32  currUpldOffset;
    U32  accReadBytes;
    U32  dataBuf_CB;
    U16  PPB2Buf_CB;
    U16  errCode;
    U8   bDwldEnd;
    U8   bUpldStart;
    U8   *ucptrDataBuf;      /* Uncached pointer to data buffer */
    U8   *ptrPPB2Buf;
    U8   *ptrTmpHifBuf;
} vendorCmdStruct;

typedef struct USB_VAR_
{
    U8  volatile    otgPerMassmax_Lun;
    U8  volatile    otgSD_LUN;
    U8  volatile    otgNAND_LUN;
    U8  volatile    otgSetFaddr;
    U8  volatile    otgClearedEP;
    U8  volatile    otgMyDeviceHNPSupport;
    U8  volatile    free_dma_channels;
    U8  volatile    otgResetComplete;
    U16 volatile    otgTestMode;
    U16 volatile    otgGetStatusResponse;
    U32 volatile    otgCSW_Addr;
    U32 volatile    otgSetupCmdBuf_Addr;
    U32 volatile    otgCBWCB_Addr;
    U8  volatile    otgRegPower;
    U8  volatile    otgIntStatus;
    U8  volatile    otgDMARXIntStatus;
    U8  volatile    otgDMATXIntStatus;
    U8  volatile    otgDataPhaseDir;
    U8  volatile    otgMassCmdRevFlag;
    U8  volatile    otgMassRxDataReceived;
    U8  volatile    otgReqOTGState;
    U8  volatile    otgCurOTGState;
    U8  volatile    otgSuspended;
    U8  volatile    otgRemoteWakeup;
    U8  volatile    otgHNPEnabled;
    U8  volatile    otgHNPSupport;
    U8  volatile    otgConfig;
    U8  volatile    otgInterface;
    U8  volatile    otgUSBState;
    U8  volatile    otgcid;
    U8  volatile    otgFaddr;
    U8  volatile    otgRegDevCtl;
    U8  volatile    otgSpeed;
    U16 volatile    otgSOF_1msCount;
    U8  volatile    u8USBDeviceMode;
    U8  volatile    DeviceConnect;
    U8  volatile    u8USBDevMode;
    U8  volatile    otgFSenseKey;
    U8  volatile    otgFASC;
    U8  volatile    otgFASCQ ;
    U8  volatile    otgdataXfer_dir;
    U8  volatile    otgStop;
    S32 volatile    otgfun_residue;
    U32 volatile    otgactualXfer_len;
    endpoint_t volatile otgUSB_EP[4];
    struct devrequest volatile otgEP0Setup;
    U8  volatile    UsbSetupCmdBuf[32];
    U8  volatile    UsbCSWBuf[(13+3)];
    U8  volatile    UsbCBWBuf[(31+1)];
    U8  volatile    *ucptrDataBuf;      /* Uncached pointer to data buffer */
    struct vendorCmdStruct_t volatile   *ptrVCmdStruct;
    U8  volatile    *USB_Msdfn_Dscr;
    U32 volatile    SizeofUSB_Msdfn_Dscr;
}   USB_VAR;

#define USB_BUFSIZE (8)

S32 MDrv_USB_Read(U8 *buf, U32 len, USB_VAR *gUSBStruct);
S32 MDrv_USB_Write(U8 *buf, U32 len, USB_VAR *gUSBStruct);
void USB_Endpoint_Bulk_In_Stall(USB_VAR *gUSBStruct);
void USB_Endpoint_Bulk_Out_Stall(USB_VAR *gUSBStruct);
#endif // _GOLBALVAR_H
