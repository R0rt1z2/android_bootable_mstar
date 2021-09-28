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


#ifndef _BOOT_INC_
#include "boot.inc"
#endif

    .text
    .globl      _vector

_vector:

#ifndef CONFIG_MSTAR_TITANIA_BD_FPGA
#if defined(CONFIG_MBOOT_IN_SPI_FLASH)

    .word 0xd503201f  // nop
    .word 0xd503201f  // nop
    .word 0x5800089a  // ldr  x26, 0x110
    .word 0xd51ec01a  // msr  vbar_el3, x26

    //init cpu register
    .word 0xd53e1007  // mrs  x7, sctlr_el3
    .word 0xd2820008  // mov  x8, #0x1000
    .word 0xb27e0108  // orr  x8, x8, #0x4
    .word 0xb2400108  // orr  x8, x8, #0x1
    .word 0xb2750108  // orr  x8, x8, #0x800
    .word 0x8a2800e7  // bic  x7, x7, x8
    .word 0xb27b00e7  // orr  x7, x7, #0x20
    .word 0xb27d00e7  // orr  x7, x7, #0x8
    .word 0xd51e1007  // msr  sctlr_el3, x7
    .word 0xd503201f  // nop
    .word 0xd503201f  // nop
    .word 0xd503201f  // nop

    //set 0x123964=0x1051 to switch FIQ/IRQ merge bit
    .word 0x5800065a  // ldr  x26, 0xc8
    .word 0x18000679  // ldr  w25, 0xcc
    .word 0x39000359  // strb w25, [x26]
    .word 0xd503201f  // nop

    //set 0x101e27=0x02 to enable ejtag
    .word 0x5800051a  // ldr  x26, 0xa0
    .word 0x18000539  // ldr  w25, 0xa4
    .word 0x39000359  // strb w25, [x26]

    //switch from aarch64 to aarch32
    .word 0x5800053e  // ldr  x30, 0xa4
    .word 0xd503201f  // nop
    .word 0xd51cc01f  // msr  vbar_el2, xzr
    .word 0xd51c101f  // msr  sctlr_el2, xzr
    .word 0xd518101f  // msr  sctlr_el1, xzr
    .word 0xd51e115f  // msr  cptr_el3, xzr
    .word 0xd51c115f  // msr  cptr_el2, xzr
    .word 0xd518105f  // msr  cpacr_el1, xzr
    .word 0xd51c11ff  // msr  hacr_el2, xzr
    .word 0xd51c117f  // msr  hstr_el2, xzr
    .word 0xd51ce07f  // msr  cntvoff_el2, xzr
    .word 0xd53800a0  // mrs  x0, mpidr_el1
    .word 0xd51c00a0  // msr  vmpidr_el2, x0
    .word 0xd5380000  // mrs  x0, midr_el1
    .word 0xd51c0000  // msr  vpidr_el2, x0
    .word 0xd53e1021  // mrs  x1, actlr_el3
    .word 0xb2400021  // orr  x1, x1, #0x1
    .word 0xb27f0021  // orr  x1, x1, #0x2
    .word 0xb27c0021  // orr  x1, x1, #0x10
    .word 0xb27b0021  // orr  x1, x1, #0x20
    .word 0xb27a0021  // orr  x1, x1, #0x40
    .word 0xd51e1021  // msr  actlr_el3, x1
    .word 0xd51c111f  // msr  hcr_el2, xzr
    .word 0xd51e403e  // msr  elr_el3, x30
    .word 0xd51c101f  // msr  sctlr_el2, xzr
    .word 0xd2807920  // msr  elr_el3, x30
    .word 0xd51e4000  // msr  sctlr_el2, xzr
    .word 0xd51e403e  // msr  elr_el3, x30
    .word 0xd53e4001  // mrs  x1, spsr_el3
    .word 0x928043e2  // mov  x2, #0xfffffffffffffde0
    .word 0x8a020021  // and  x1, x1, x2
    .word 0xb2400421  // orr  x1, x1, #0x3
    .word 0xb27c0021  // orr  x1, x1, #0x10
    .word 0x9276f821  // and  x1, x1, #0xfffffffffffffdff
    .word 0xd51e4001  // msr  spsr_el3, x1
    .word 0xd69f03e0  // eret

    //data section
    .word 0x00000000  // dada=0x00000000
    .word 0x1f203c4d  // dada=0x1f203c4d
    .word 0x00000000  // data=0x00000000
    .word 0x00000002  // dada=0x00000002
    .word 0x00000000  // dada=0x00000000
    .word Boot_Entry
    .word 0x00000000  // data=0x00000000
    .word 0x1f2472c8  // data=0x00000000
    .word 0x00000000  // data=0x00000000
    .word 0x00001051  // data=0x00000000
    .word 0x00000000  // data=0x00000000
    .word CONFIG_BOOTRAM_LOADADDR // data=CONFIG_BOOTRAM_LOADADDR
    .word 0x00000000  // data=0x00000000
#endif
#endif  /*CONFIG_MSTAR_TITANIA_BD_FPGA*/

//-------------------------------------------------------------------------------------------------
// Boot_Entry:
// @param  None
// @return None
// @note   Entry point of Boot code
//-------------------------------------------------------------------------------------------------
    .globl      Boot_Entry

Boot_Entry:

#ifdef CONFIG_MSTAR_FAST_STR
.globl resume
resume:
    .word 0x0
#endif

#if !defined(CONFIG_MSTAR_ROM_BOOT_WITH_NAND_FLASH) && \
    !defined(CONFIG_MSTAR_ROM_BOOT_WITH_EMMC_FLASH)

    //Set bonding overwrite to ARM bootup without EJTAG
// WREG        (MS_BUS_RIU_ADDR + (0x0e3e << 1)), 0x1f
#endif

#ifdef CONFIG_MSTAR_SBOOT_WDT_ENABLE
    RREG        r0, (MS_BUS_RIU_ADDR + (0x0E70 << 1))   //Disable Divisor Latch Access
    lsr         r0, r0, #14
    ldr         r1, =0x0003
    cmp         r0, r1
    beq         Enable_WDT
    bl          Disable_WDT
Enable_WDT:
    WREG        (MS_BUS_RIU_ADDR + (0x300a << 1)), ((CONFIG_SBOOT_WDT_VALUE >> 16) & 0xFFFF)
    WREG        (MS_BUS_RIU_ADDR + (0x3008 << 1)), (CONFIG_SBOOT_WDT_VALUE & 0xFFFF)
    WREG        (MS_BUS_RIU_ADDR + (0x3000 << 1)), 0x0001   //Re-start WDT
    b           Boot_Start
#endif
Disable_WDT:
    WREG        (MS_BUS_RIU_ADDR + (0x3008 << 1)), 0x0000
    WREG        (MS_BUS_RIU_ADDR + (0x300a << 1)), 0x0000

Boot_Start:

#if 0
    //----------------------------------------------------------
    // CPU setting
    //----------------------------------------------------------
    // Reorder fifo
    WREG        (MS_BUS_RIU_ADDR + (0x313900 << 1)), 0x0000
    WREG        (MS_BUS_RIU_ADDR + (0x313A00 << 1)), 0x0000

    ldr         r1, =(MS_BUS_RIU_ADDR + (0x313400 << 1))
    ldr         r0, [r1, #(0x00 << 1)]
    orr         r0, r0, #0x01
    strb        r0, [r1, #(0x00 << 1)]

    //Read prefetch, Write compress
    WREG        (MS_BUS_RIU_ADDR + (0x101D1E << 1)), 0x0101
#endif

    //Initalize the PIU timer to measure boot time
    bl          BootRom_InitTimer

    RREG        r3, (MS_BUS_RIU_ADDR + (0x002e52 << 1))
    orr         r3, r3, #0x0000003f         //Set deglitch period for vdd2low_rst and chip_top_powergood
    REG_C2M     r3, (MS_BUS_RIU_ADDR + (0x002e52 << 1))

    bl          Boot_InitCpuRegister        //Initialize CPU Registers

#ifdef CONFIG_MSTAR_FAST_STR
    //check STR magic number
    RREG        r0, (MS_BUS_RIU_ADDR + (0x0E70 << 1))   //Disable Divisor Latch Access
    bic         r0, r0, #0x0FFF
    ldr         r1, =0xF000
    ldr         r2, =resume                                          //resume flag address
    mov         r3, #0                                               //set default resume = 0
    cmp         r0, r1
    movne       r3, #1                                               //set resume = 1, if resume mode
    str         r3, [r2]                                             //
#endif

    DBGREG      0x3697

    mrc         p15, 0, r0, c0, c0, 5
    and         r0, r0, #0x300              //Single Core
    cmp         r0, #0
    beq         Boot_PrimaryCore

    bl          Boot_SecondaryCore

Boot_PrimaryCore:

    //----------------------------------------------------------
    // switch FIQ / IRQ merge bit
    //----------------------------------------------------------
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x123900 << 1))
    ldr         r0, [r1, #(0x64 << 1)]
    orr         r0, r0, #0x50
    strb        r0, [r1, #(0x64 << 1)]

    // Set 0x1239fc bit 3 to 1
    ldr         r0, [r1, #(0xfc << 1)]
    orr         r0, r0, #(0x1 << 3)
    strb        r0, [r1, #(0xfc << 1)]

    // Set 0x1239fc bit 2 to 1
    ldr         r0, [r1, #(0xfc << 1)]
    orr         r0, r0, #(0x1 << 2)
    strb        r0, [r1, #(0xfc << 1)]

    // Set 0x1239fc bit 1 to 1
    ldr         r0, [r1, #(0xfc << 1)]
    orr         r0, r0, #(0x1 << 1)
    strb        r0, [r1, #(0xfc << 1)]

    ldr         pc, =BootRom_Entry          //Jump to Entry Point of BootRom

Boot_SecondaryCore:
                                            //Enable software interrupt
    //Set Priority Mask Register in CPU Interface
    WREG        (REG_GIC_CPU_BASE_ADDRESS + 0x04), 0xf0

    //Set Control Register in CPU Interface to Enable Software Interrupt
    WREG        (REG_GIC_CPU_BASE_ADDRESS + 0x00), 0x01

                                            //SECOND_START_ADDR_HI = 0x1f206740
                                            //SECOND_START_ADDR_LO = 0x1f206744
                                            //set LSB[1:0] of LO = 2'b11, then wait soft interrupt to wake up
    WREG        (MS_BUS_RIU_ADDR + (0x1033A2 << 1)), 0xff

    wfi

    //Core 1 must jump to DRAM space to avoid SPI flash hang issue
    ldr         pc, =Bootram_SecondaryCore

Boot_SecondaryCoreLoop:
    b           Boot_SecondaryCoreLoop

//-------------------------------------------------------------------------------------------------
// Boot_InitCpuRegister
// @param  None
// @return None
// @note   ARM internal register init setting
//-------------------------------------------------------------------------------------------------
Boot_InitCpuRegister:

    mrc         p15, 0, r0, c1, c0, 0
    bic         r0, r0, #0x01               //Disable MMU
    bic         r0, r0, #0x04               //Disable D-Cache
    bic         r0, r0, #0x1000             //Disable I-Cache
    mcr         p15, 0, r0, c1, c0, 0

    bx          lr
