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

#ifndef _DRV_UART_INC_
#define _DRV_UART_INC_

//-------------------------------------------------------------------------------------------------
//UART registers
//-------------------------------------------------------------------------------------------------
UART_BASE                   = (MS_BUS_RIU_ADDR + (0x100980 << 1))
UART_LCR_DLAB               = 0x80  //Divisor latch access bit
UART_LCR_SBC                = 0x40  //Set break control
UART_LCR_SPAR               = 0x20  //Stick parity (?)
UART_LCR_EPAR               = 0x10  //Even parity select
UART_LCR_PARITY             = 0x08  //Parity Enable
UART_LCR_STOP1              = 0x00  //Stop bits: 0=1 stop bit, 1= 2 stop bits
UART_LCR_STOP2              = 0x04  //Stop bits: 0=1 stop bit, 1= 2 stop bits
UART_LCR_WLEN5              = 0x00  //Wordlength: 5 bits
UART_LCR_WLEN6              = 0x01  //Wordlength: 6 bits
UART_LCR_WLEN7              = 0x02  //Wordlength: 7 bits
UART_LCR_WLEN8              = 0x03  //Wordlength: 8 bits

#ifdef CONFIG_MSTAR_TITANIA_BD_FPGA
UART_BAUD_RATE              = 38400
UART_CLK_FREQ               = 12000000
#else
UART_BAUD_RATE              = 115200
UART_CLK_FREQ               = 123000000
#endif

CPU_CLOCK_FREQ              = (CPU_CLOCK * 1000000)

//Divisor
UART_DIVISOR                = (UART_CLK_FREQ / (16 * UART_BAUD_RATE))
UART_DIVISOR_H              = (UART_DIVISOR >> 8)
UART_DIVISOR_L              = (UART_DIVISOR & 0xff)

//-------------------------------------------------------------------------------------------------
//UART Register
//-------------------------------------------------------------------------------------------------
UART_RX                     = (0 * 4)   //In:  Receive buffer (DLAB=0)    (16-byte FIFO)
UART_TX                     = (0 * 4)   //Out: Transmit buffer (DLAB=0)   (16-byte FIFO)
UART_DLL                    = (0 * 4)   //Out: Divisor Latch Low (DLAB=1)
UART_DLM                    = (1 * 4)   //Out: Divisor Latch High (DLAB=1)
UART_IER                    = (1 * 4)   //Out: Interrupt Enable Register
UART_IIR                    = (2 * 4)   //In:  Interrupt ID Register
UART_FCR                    = (2 * 4)   //Out: FIFO Control Register
UART_LCR                    = (3 * 4)   //Out: Line Control Register
UART_MCR                    = (4 * 4)   //Out: Modem Control Register
UART_LSR                    = (5 * 4)   //In:  Line Status Register
UART_MSR                    = (6 * 4)   //In:  Modem Status Register
UART_SCR                    = (7 * 4)   //I/O: Scratch Register
//-------------------------------------------------------------------------------------------------
//FIFO Control Register
//-------------------------------------------------------------------------------------------------
UART_FCR_ENABLE_FIFO        = 0x01      //Enable the FIFO
UART_FCR_CLEAR_RCVR         = 0x02      //Clear the RCVR FIFO
UART_FCR_CLEAR_XMIT         = 0x04      //Clear the XMIT FIFO
UART_FCR_DMA_SELECT         = 0x08      //For DMA applications
UART_FCR_TRIGGER_MASK       = 0xC0      //Mask for the FIFO trigger range
UART_FCR_TRIGGER_1          = 0x00      //Mask for trigger set at 1
UART_FCR_TRIGGER_4          = 0x40      //Mask for trigger set at 4
UART_FCR_TRIGGER_8          = 0x80      //Mask for trigger set at 8
UART_FCR_TRIGGER_14         = 0xC0      //Mask for trigger set at 14

//-------------------------------------------------------------------------------------------------
//Init setup
//-------------------------------------------------------------------------------------------------
//UART 0 = 1: enable UART 0  Disable UART 2
//UART 0 = 0: enable UART 2  Disable UART 0
BOOTINFO_UART0_ENABLE       = 1
BOOTINFO_UART2_ENABLE       = 1

//-------------------------------------------------
//Macro: PUTK
//-------------------------------------------------
.macro PUTK char
//  LDR         r12, =\char
//  REG_C2M     r12, (MS_BUS_RIU_ADDR + (0x100980 << 1))
    WREG_B      (MS_BUS_RIU_ADDR + (0x100980 << 1)), \char
.endm

//-------------------------------------------------
//Macro: DPUTK
//-------------------------------------------------
.macro DPUTK char
    ldr         r0, =\char
    mov         r1, lr
    bl          BootRom_WriteUartCharactor
    mov         lr, r1
.endm

//-------------------------------------------------
//Macro: DPUTK_STR
//-------------------------------------------------
.macro DPUTK_STR MessageBuffer
    ldr         r0, =\MessageBuffer
    mov         r1, lr
    bl          BootRom_WriteUartString
    mov         lr, r1
.endm

#endif
