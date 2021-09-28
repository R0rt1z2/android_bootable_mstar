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
#include "../inc/Globalvar.h"
#include "../inc/ms_otg.h"
#include "../inc/ms_drc.h"
#include "../inc/ms_dma.h"

U16 USB_Read_DMA_Control(S8 channel)
{
   return *((DMA_CNTL_REGISTER(channel)));
}

void  USB_Reset_All_EP_IO_Vars(USB_VAR *gUSBStruct)
{
    U8 i;

    for (i=0; i<otgNumEPDefs; i++)
    {
        gUSBStruct->otgUSB_EP[i].IOState = EP_IDLE;
        gUSBStruct->otgUSB_EP[i].FifoRemain = 0;
        gUSBStruct->otgUSB_EP[i].BytesRequested = 0;
        gUSBStruct->otgUSB_EP[i].BytesProcessed = 0;
        gUSBStruct->otgUSB_EP[i].LastPacket = 0;
        gUSBStruct->otgUSB_EP[i].Halted = 0;
    }
}

void USB_Parse_DRC_INTRUSB(U16 intrusb,USB_VAR *gUSBStruct)
{
	//if  (intrusb & M_INTR_VBUSERROR)
	//{
	//	USB_REG_WRITE8(M_REG_DEVCTL, USB_REG_READ8(M_REG_DEVCTL) | 0x01); //enable session
	//}
	//if  (intrusb & M_INTR_DISCONNECT)
	//{
	//	gUSBStruct->otgSuspended = 0;
	//	gUSBStruct->otgUSBState = 0;
	//	gUSBStruct->u8USBDeviceMode = 0;
	//	gUSBStruct->DeviceConnect = 0;
	//	gUSBStruct->free_dma_channels = 0x7f;
	//	gUSBStruct->otgStop = 0;
	//}
	if  (intrusb & M_INTR_RESET)
	{
		//printf("Reset\n");
		if  (DRCPERIPHERAL(gUSBStruct->otgRegDevCtl))
		{
			USB_Reset_All_EP_IO_Vars(gUSBStruct);//Reset all epoint to idle and zero state.
			gUSBStruct->free_dma_channels = 0x7f;	// 20110310 free all DMA channel
			gUSBStruct->otgFaddr = 0;
			USB_Change_USB_State(USB_DEFAULT,gUSBStruct);
			//if (gUSBStruct->otgRegPower & M_POWER_HSMODE)
			{
				gUSBStruct->otgSpeed = HIGH_SPEED;
			}
			//else
			{
				//gUSBStruct->otgSpeed = FULL_SPEED;
			}
			gUSBStruct->otgResetComplete=1; // 20100813 G1 U02
		}
		if (1)//(gUSBStruct->u8USBDevMode == E_USB_MassStorage)
		{
			gUSBStruct->otgMassCmdRevFlag = 0; // 20100809 G1 U02
			//************DMA setting**************
			//************************************
			*(DMA_CNTL_REGISTER(MAX_DMA_CHANNEL))=USB_Read_DMA_Control(MAX_DMA_CHANNEL)&0xfe;
			USB_ReSet_ClrRXMode1();
			USB_REG_WRITE16(M_REG_EP_BULKOUT, (USB_REG_READ16(M_REG_EP_BULKOUT)&0xfff0)); //wayne added
			USB_REG_WRITE16(M_REG_EP_BULKOUT, (USB_REG_READ16(M_REG_EP_BULKOUT)|M_Mode1_P_BulkOut_EP)); //wayne added
			USB_REG_WRITE16(M_REG_DMA_MODE_CTL, (USB_REG_READ16(M_REG_DMA_MODE_CTL)|M_Mode1_P_Enable|M_Mode1_P_AllowAck)); //allow ack,wayne added
			//USB_REG_WRITE16(M_REG_DMA_MODE_CTL, (USB_REG_READ16(M_REG_DMA_MODE_CTL)|(USB_BUFSIZE/512))); //disable set_ok2rcv[15]&ECO4NAK_en[14],wayne added
		}
	}
}
