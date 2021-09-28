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
#include <mhal_sys.h>
#include <MsTypes.h>
#include <MsUboot.h>
#include <command.h>
#include <common.h>
#include <MsSysUtility.h>

#if defined(CONFIG_MSTAR_ROM_BOOT_WITH_EMMC_FLASH)
#include "../mmc/inc/common/eMMC.h"
#endif
void Mhal_console_init(void)
{
    //config PIU_UART1 1106 bank
    *(volatile MS_U32*)(MS_RIU_MAP+(0x11060C<<1)) |= 0x0080; //Divisor Latch Access enable
    *(volatile MS_U32*)(MS_RIU_MAP+(0x110600<<1)) = 0x0046; // Divisor Latch LSB = 0x5D
    *(volatile MS_U32*)(MS_RIU_MAP+(0x110604<<1)) = 0x0000; //Divisor Latch MSB =0
    *(volatile MS_U32*)(MS_RIU_MAP+(0x11060C<<1)) &= 0xFF7F; //Divisor Latch Access disable

    *(volatile MS_U32*)(MS_RIU_MAP+(0x110608<<1)) = 0x0000;
    *(volatile MS_U32*)(MS_RIU_MAP+(0x110608<<1)) = 0x0007; //enable fifo & clear fifo

    *(volatile MS_U32*)(MS_RIU_MAP+(0x11060C<<1)) = 0x0000;
    *(volatile MS_U32*)(MS_RIU_MAP+(0x11060C<<1)) = 0x0003; //8-bit Data / 1-bit Stop / No Parity Bit

    *(volatile MS_U32*)(MS_RIU_MAP+(0x110610<<1)) = 0x0000; //Set DTS/RTS to 1

    *(volatile MS_U32*)(MS_RIU_MAP+(0x110604<<1)) = 0x0000; //Disable Interrupt
    //config PIU_FUART0 1106 bank
    *(volatile MS_U32*)(MS_RIU_MAP+(0x11068C<<1)) |= 0x0080; //Divisor Latch Access enable
    *(volatile MS_U32*)(MS_RIU_MAP+(0x110680<<1)) = 0x0068; // Divisor Latch LSB = 0x5D
    *(volatile MS_U32*)(MS_RIU_MAP+(0x110684<<1)) = 0x0000; //Divisor Latch MSB =0
    *(volatile MS_U32*)(MS_RIU_MAP+(0x11068C<<1)) &= 0xFF7F; //Divisor Latch Access disable

    *(volatile MS_U32*)(MS_RIU_MAP+(0x110688<<1)) = 0x0000;
    *(volatile MS_U32*)(MS_RIU_MAP+(0x110688<<1)) = 0x0007; //enable fifo & clear fifo

    *(volatile MS_U32*)(MS_RIU_MAP+(0x11068C<<1)) = 0x0000;
    *(volatile MS_U32*)(MS_RIU_MAP+(0x11068C<<1)) = 0x0003; //8-bit Data / 1-bit Stop / No Parity Bit

    *(volatile MS_U32*)(MS_RIU_MAP+(0x110690<<1)) = 0x0000; //Set DTS/RTS to 1

    *(volatile MS_U32*)(MS_RIU_MAP+(0x110684<<1)) = 0x0000; //Disable Interrupt

    //bit 12 UART Enable, bit 11 UART RX Enable
    *(volatile MS_U32*)(MS_RIU_MAP+(0x0E12<<1)) &= ~0x1800;    //don't enable uart (decided by mboot env)

    // Switch UART0 to PIU UART 0
    // Clear UART_SEL0
    *(volatile MS_U32*)(MS_RIU_MAP+(0x101EA6<<1)) &= ~0x000F;
#if (EARLY_DISABLE_UART)
#else
    // bit 11 UART RX Enable
    *(volatile MS_U32*)(MS_RIU_MAP+(0x0E12<<1)) |= 0x0800;

    // UART_SEL0 --> PIU UART0


#if defined(CONFIG_MSTAR_BD_MST095D_10AVPC_MASERATI) && defined(CONFIG_HDMITX_MSTAR_ROCKET2)
    //*(volatile MS_U32*)(MS_RIU_MAP+(0x101EA6<<1)) = 0x0004; // uart_sel0 = PIU UART0
    *(volatile MS_U32*)(MS_RIU_MAP+(0x101EA6<<1)) = 0x0064; // uart_sel0 = PIU UART0
    *(volatile MS_U32*)(MS_RIU_MAP+(0x101E04<<1)) = 0x0300; // uart_sel0 = PIU UART0
    *(volatile MS_U32*)(MS_RIU_MAP+(0x101EA8<<1)) = 0x0050; // uart_sel5 = PIU UART1
    *(volatile MS_U32*)(MS_RIU_MAP+(0x100B28<<1)) = 0x0c0c; // uart 1 clk
#elif defined(CONFIG_MSTAR_BD_MST098D_10AVPC_MASERATI) && defined(CONFIG_HDMITX_MSTAR_ROCKET2)
    *(volatile MS_U32*)(MS_RIU_MAP+(0x101EA6<<1)) = 0x0564; // uart_sel0 = PIU UART0
    *(volatile MS_U32*)(MS_RIU_MAP+(0x100B28<<1)) = 0x0c0c; // uart 1 clk
#else
    *(volatile MS_U32*)(MS_RIU_MAP+(0x100B28<<1)) = 0x0c0c; // PIU_UART0 / PIU_UART1--> clk 144
    *(volatile MS_U32*)(MS_RIU_MAP+(0x100B2A<<1)) &= 0xFFE0; // Clean fuart clk
    *(volatile MS_U32*)(MS_RIU_MAP+(0x100B2A<<1)) |= 0x000c; // Config PIU_FUART--> clk 144
    *(volatile MS_U32*)(MS_RIU_MAP+(0x101EA6<<1)) = 0x0654;
    *(volatile MS_U32*)(MS_RIU_MAP+(0x101EA8<<1)) = 0x0000;

#endif
#endif
    // <-------

    // UART mode

    //enable reg_uart_rx_enable;
    *(volatile MS_U32*)(MS_RIU_MAP+(0x101EA2<<1)) |= (1<<10);

   
}

void Mhal_Sys_WholeChipReset( void )
{

    #if defined(CONFIG_MSTAR_ROM_BOOT_WITH_EMMC_FLASH)
	eMMC_RST_L();
	eMMC_hw_timer_delay(HW_TIMER_DELAY_1ms);
	eMMC_RST_H();
    #endif
    WriteByte( 0x2E5C, 0x79 ); //0x2E for T12, J2, A5, A3
    //MDrv_WriteByte( 0xE5C, 0x79 ); //0xE for T13, A1, A6, A7
    while(1);

}


int MHal_MSPI_CFG_PIN(int port_num)
{
    if (port_num == 0)
    {
        WriteRegBit(0x101e9c, (1<<4), TRUE);
    }
    else
    {
        return -1;
    }
    return 0;
}

