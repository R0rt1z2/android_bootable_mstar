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
#include "../inc/ms_usb.h"
#include "../inc/ms_msd_fn_xfer.h"
#include "../inc/Globalvar.h"
#include "../inc/ms_otg.h"
#include "../inc/ms_drc.h"

#ifdef RAMDISK
u8 Storage_Mem[RamDiskSize];
u32 volatile Storage_Mem_Addr;
void RamDisk_Init(void)
{
	/* tony open for init ram disk format fist 20 sectors */
	 U16 tmp,tmp1,tmp2=10;
	 u8   patern = 0x0;

     Storage_Mem_Addr =(U32)&Storage_Mem[0];

    //format(Storage_Mem_Addr);
/* tony open for init ram disk */
#if 1
	tmp=0;
	while(tmp2-->0)
	{
		for (tmp1=0;tmp1<512;tmp1++)
		{
			Storage_Mem[tmp++]=patern++;
			Storage_Mem[tmp++]=patern++;
		}
	}
	Storage_Mem[0x11] = 0x22;
#endif
}
#endif

S32 USB_Get_Nbits_Value(U8 *data,U8 p,U8 n)
{
    S32  tmp;
    S32  value;

    tmp = (S32)((S32)(*data) +
                (S32)(*(data+1)) * (POSITION_VALUE_8) +
                (S32)(*(data+2)) * (POSITION_VALUE_16) +
                (S32)(*(data+3)) * (POSITION_VALUE_24) );

    if(n == 32)
        value = ( (tmp >> (p) ) & 0xFFFFFFFF );
    else
        value = ( (tmp >> (p) ) & BIT_MASK(n) );

    return(value);
}

S32 USB_Get_Bits_From_Byte_Stream(U8 *data_ptr,U8 p,U8 n, S8 *err)
{
    S32 value = 0;

    if(data_ptr)
    {
        if ( (p + n) <= MAX_BITS_INT )
        {
            value = USB_Get_Nbits_Value(data_ptr, p, n);
            *err  = 0;
        }
        else
        {
            *err = -2;
        }
    }
    else
    {
        *err = -1;
    }
    return (value);
}

void USB_Endpoint_Bulk_In_Stall(USB_VAR *gUSBStruct)
{
    U8 ep;

    ep = USB_Ret_Blt_EP_Object(EP_RX);
    USB_Send_Stall(ep,gUSBStruct);
    return;
}

void USB_Endpoint_Bulk_Out_Stall(USB_VAR *gUSBStruct)
{
    U8 ep;

    ep = USB_Ret_Blt_EP_Object(EP_TX);
    USB_Send_Stall(ep,gUSBStruct);
    return;
}

void USB_Set_Bits_In_Byte(U8 *data_ptr, S32 value, U8 p, U8 n)
{
    *data_ptr = (*data_ptr & ~( BIT_MASK(n) << p )) |
        ( ( value & BIT_MASK(n) ) << p ) ;
    return;
}

void USB_Set_Bits_In_3Bytes(U8 *data_ptr, S32 value, U8 p, U8 n)
{
    U8 bits_in_1st_byte, bits_in_3rd_byte;

    bits_in_1st_byte = MAX_BITS_BYTE - p ;
    bits_in_3rd_byte = n - bits_in_1st_byte - MAX_BITS_BYTE ;
    *data_ptr = (*data_ptr & ~( BIT_MASK(bits_in_1st_byte) << p )) |
        ( ( value & BIT_MASK(bits_in_1st_byte) ) << p ) ;
    *(data_ptr+1) = value>>bits_in_1st_byte ;
    *(data_ptr+2) = (*(data_ptr+2) & ~( BIT_MASK(bits_in_3rd_byte) )) |
        ( ( value>>(bits_in_1st_byte + MAX_BITS_BYTE) &
            BIT_MASK(bits_in_3rd_byte) ) ) ;

    return;
}

void USB_Set_Bits_In_Int(U8 *data_ptr, S32 value, U8 p, U8 n)
{
    U8 bits_in_1st_byte, bits_in_4th_byte;

    bits_in_1st_byte = MAX_BITS_BYTE - p ;
    bits_in_4th_byte = n - bits_in_1st_byte - MAX_BITS_SHORT ;
    *data_ptr = (*data_ptr & ~( BIT_MASK(bits_in_1st_byte) << p )) |
        ( ( value & BIT_MASK(bits_in_1st_byte) ) << p ) ;
    *(data_ptr+1) = value>>bits_in_1st_byte ;
    *(data_ptr+2) = value>>(bits_in_1st_byte + MAX_BITS_BYTE) ;
    *(data_ptr+3) = (*(data_ptr+3) & ~( BIT_MASK(bits_in_4th_byte) )) |
        ( ( value>>(bits_in_1st_byte + MAX_BITS_SHORT) &
            BIT_MASK(bits_in_4th_byte) ) ) ;
    return;

}


void USB_Set_Bits_In_Short(U8 *data_ptr, S32 value, U8 p, U8 n)
{
    U8 bits_in_1st_byte, bits_in_2nd_byte;

    bits_in_1st_byte = MAX_BITS_BYTE - p ;
    bits_in_2nd_byte = n - bits_in_1st_byte;

    *data_ptr = (*data_ptr & ~( BIT_MASK(bits_in_1st_byte) << p )) |
        ( (value & BIT_MASK(bits_in_1st_byte) ) << p ) ;
    *(data_ptr+1) = (*(data_ptr+1) & ~( BIT_MASK(bits_in_2nd_byte) )) |
        (( value>>bits_in_1st_byte & BIT_MASK(bits_in_2nd_byte) ));
    return;
}

S32 USB_Set_Bits_In_Byte_Stream(U8 *data_ptr, S32 value, U8 p, U8 n)
{
    S32 ret_value = 0;

    if(data_ptr)
    {
        if ( (p + n) <= MAX_BITS_BYTE )
        {
            USB_Set_Bits_In_Byte(data_ptr, value, p, n) ;
        }
        else if ( (p + n) <= MAX_BITS_SHORT)
        {
            USB_Set_Bits_In_Short(data_ptr, value, p, n);
        }
        else if ( (p + n) <= MAX_BITS_3BYTE)
        {
            USB_Set_Bits_In_3Bytes(data_ptr, value, p, n);
        }
        else if( (p + n) <= MAX_BITS_INT)
        {
            USB_Set_Bits_In_Int(data_ptr, value, p, n);
        }
        else
        {
            ret_value = -2;
        }
    }
    else
    {
        ret_value = -1;
    }
    return(ret_value);
}

void USB_MSDFN_Encode_CSW(MSDFN_BOT_CSW_STRU *csw, U8 *cswP)
{
    USB_Set_Bits_In_Byte_Stream(cswP,   csw->signature, 0, 32);
    USB_Set_Bits_In_Byte_Stream(cswP+4, csw->tag,       0, 32);
    USB_Set_Bits_In_Byte_Stream(cswP+8, csw->residue, 0, 32);
    cswP[12] = csw->status;
    return;
}

void USB_MSDFN_Decode_CBW(U8 *cbwP, MSDFN_BOT_CBW_STRU *cbw)
{
    S8 err;
    U8 i;

    cbw->signature       = USB_Get_Bits_From_Byte_Stream(cbwP,   0, 32, &err);
    cbw->tag             = USB_Get_Bits_From_Byte_Stream(cbwP+4, 0, 32, &err);
    cbw->dxfer_length    = USB_Get_Bits_From_Byte_Stream(cbwP+8, 0, 32, &err);
    cbw->dir_flag        = cbwP[12];
    cbw->max_lun         = cbwP[13];
    cbw->cmd_length      = cbwP[14];
	//printf("dir_flag:0x%x\n",cbwP[12]);
	//printf("cmd_length:0x%x\n",cbwP[14]);
	//for(i=0;i<32;i++)
	//	printf("%x ",cbwP[i]);
	//printf("\n");
    for (i=0;i<16;i++)
        cbw->cmd_bytes[i]=cbwP[15+i];
    return;
}
