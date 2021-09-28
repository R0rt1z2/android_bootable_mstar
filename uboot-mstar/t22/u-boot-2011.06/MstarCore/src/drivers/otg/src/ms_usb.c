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
#include "../inc/ms_msd_global.h"
#include "../inc/ms_cpu.h"
#include "../inc/ms_drc.h"

S8 USB_Send_Stall(U8 ep,USB_VAR *gUSBStruct)
{
    U32 csr, reg, bit;
    U32 timeout = 0;

    if (gUSBStruct->otgUSB_EP[ep].BltEP == 0)
    {
        reg = 0x102*2;//M_REG_CSR0;
          bit = M_CSR0_P_SENDSTALL;
    }
    else
    {
        //USB_DRC_Index_Select(gUSBStruct->otgUSB_EP[ep].BltEP);

        if (gUSBStruct->otgUSB_EP[ep].DRCDir)   /* RX */
        {
            reg = 0x126*2;//M_REG_RXCSR1;
            bit = M_RXCSR1_P_SENDSTALL;
        }
        else             /* TX */
        {
            reg = 0x112*2;//M_REG_TXCSR1;
            bit = M_TXCSR1_P_SENDSTALL;
        }
    }

    csr = USB_REG_READ8(reg);
    USB_REG_WRITE8(reg, (csr | bit));
    if (gUSBStruct->otgUSB_EP[ep].DRCDir)   /* RX */
    {
//        printf("R:%x \n",USB_REG_READ8(reg));
        while((USB_REG_READ8(0x126*2/*M_REG_RXCSR1*/)&M_RXCSR1_P_SENDSTALL)&&(gUSBStruct->u8USBDeviceMode==1))
        {
            /* wait 1 second timeout */
            if(timeout > 100000)
                break;
            timeout++;
            //hal_delay_us(10);
        };
    }
    else             /* TX */
    {
//        printf("T:%x \n",USB_REG_READ8(reg));
		while (!OTG_Interrupt_Polling(0x20, gUSBStruct)) ;//polling ep0 interrupt
        while((USB_REG_READ8(0x112*2/*M_REG_TXCSR1*/)&M_TXCSR1_P_SENDSTALL)&&(gUSBStruct->u8USBDeviceMode==1))
        {
            /* wait 1 second timeout */
            if(timeout > 100000)
                break;
            timeout++;
            //hal_delay_us(10);
        };
    }
    return 1;
}

void USB_SWOP_Setup(struct devrequest *sP)
{
    SWOP(sP->wValue);
    SWOP(sP->wIndex);
    SWOP(sP->wLength);
}

U8 otgIsUSBConfiged(USB_VAR *gUSBStruct)
{
      if (gUSBStruct->otgUSBState==USB_CONFIGURED)
          return 1;
      else
          return 0;
}

void USB_Change_USB_State(U8 toUSB,USB_VAR *gUSBStruct)
{
    switch  (toUSB)
    {
        case USB_POWERED:   /* HOST:CONNECT DRC INT;PER:exit active mode*/
            gUSBStruct->otgSuspended = 0;
            break;
        case USB_DEFAULT:   /* HOST:Reset complete;PER: RESET DRC INT */
            gUSBStruct->otgHNPEnabled = 0;
            gUSBStruct->otgHNPSupport = 0;
            gUSBStruct->otgRemoteWakeup = 0;
            gUSBStruct->otgSuspended = 0;
            gUSBStruct->u8USBDeviceMode = 0;	// 20110310
            break;
        case USB_ADDRESS:
            gUSBStruct->otgConfig = 0;
            break;
        case USB_CONFIGURED:
         gUSBStruct->u8USBDeviceMode = 1;
            //DBG_MSG("USB Configured\n");
            break;
        default:
            break;
    }
    gUSBStruct->otgUSBState = toUSB;
}
