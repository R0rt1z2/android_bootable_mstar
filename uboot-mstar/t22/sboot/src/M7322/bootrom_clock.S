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
    .global     BootRom_InitClock

BootRom_InitClock:

    push        {lr}

    //----------------------------------------------------------
    // MIPSPLL
    //----------------------------------------------------------
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x101d00 << 1))
    ldr         r0, =0x01
    strb        r0, [r1, #(0xf8 << 1)]                  //Set CPU clock MUX to ARMPLL clk

    //----------------------------------------------------------
    // IP: clk_mcu, uart, spi
    //----------------------------------------------------------
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x103300 << 1))
    ldr         r0, =0x10                               //Set CLK_R2_SECURE to 240MHz (Enabled)
    strb        r0, [r1, #(0x7a << 1)]

    ldr         r1, =(MS_BUS_RIU_ADDR + (0x000e00 << 1))
    ldr         r0, =0x8438                             //Set CLK_MCU_PM to 216MHz (Enabled)
    str         r0, [r1, #(0x40 << 1)]                  //Set CLK_SPI to 54MHz (Enabled)

    ldr         r1, =(MS_BUS_RIU_ADDR + (0x100b00 << 1))
    ldr         r0, =0x0001                             //Set CLK_MCU to 216MHz (Enabled)
    str         r0, [r1, #(0x20 << 1)]                  //Set MCU clock to CLK_MCU

    ldr         r0, =0x0c                               //Set CLK_UART0 to 123MHz (Enabled)
    strb        r0, [r1, #((0x26 << 1) + 1)]            //Set CLK_UART1 to 123MHz (Enabled)
    ldr         r0, =0x0c0c                             //Set CLK_UART2 to 123MHz (Enabled)
    str         r0, [r1, #(0x28 << 1)]                  //Set CLK_UART3 to 123MHz (Enabled)
    ldr         r0, =0x0c0c                             //Set CLK_UART4 to 123MHz (Enabled)
    str         r0, [r1, #(0x32 << 1)]

    ldr         r0, =0x04                               //Set CLK_SPI to 54MHz
    strb        r0, [r1, #(0x2c << 1)]
    ldr         r0, =0x44                               //Set CLK_SPI to 54MHz (Enabled)
    strb        r0, [r1, #(0x2c << 1)]
    ldr         r0, =0x04                               //Set CLK_SPI_M to 48MHz (Enabled)
    strb        r0, [r1, #(0x2e << 1)]
    ldr         r0, =0x24                               //Set CLK_SPI_M to 48MHz (Enabled)
    strb        r0, [r1, #(0x2e << 1)]

    //----------------------------------------------------------
    // IP: clk_miu
    //----------------------------------------------------------
    ldr         r0, =0x0102                             //switch clk_miu_rec clock freq
    str         r0, [r1, #(0x3c << 1)]
    ldr         r0, =0x00                               //block clk_miu_* gated
    strb        r0, [r1, #((0x3e << 1) + 1)]
    ldr         r0, =0x0808                             //switch clk_miu/clk_miu_256bus to MIU128PLL    //reg_ckg_miu[4:0]
    str         r0, [r1, #(0x3a << 1)]

    //----------------------------------------------------------
    // IP: clk_vd
    //----------------------------------------------------------
    ldr         r0, =0x0100                             //Set VD clock source to VD_ADC_CLK (Enabled)
    str         r0, [r1, #(0x40 << 1)]                  //Set CLK_VD (Disabled)
    ldr         r0, =0x0101                             //Set CLK_MCU_MAIL0/1 to CLK_MCU (Disabled)
    str         r0, [r1, #(0x44 << 1)]
    ldr         r0, =0x0101                             //Set CLK_VD2X (Disabled)
    str         r0, [r1, #(0x46 << 1)]                  //Set CLK_VD32FSC to VD_ADC_CLK (Disabled)

    //----------------------------------------------------------
    // IP: clk_vd_mcu
    //----------------------------------------------------------
    ldr         r0, =0x11                               //Set CLK_VDMCU to 108MHz (Disabled)
    strb        r0, [r1, #(0x42 << 1)]


    //----------------------------------------------------------
    // IP: clk_tsp
    //----------------------------------------------------------
    ldr         r0, =0x11                               //Set CLK_TSP to 172MHz (Disabled),Set CLK_PARSER to 192MHz (Disabled)
    strb        r0, [r1, #(0x54 << 1)]

    //----------------------------------------------------------
    // IP: clk_mheg5
    //----------------------------------------------------------
    ldr         r0, =0x01                               //Set CLK_VD_MHEG5 to 240MHz (Disabled)
    strb        r0, [r1, #(0x60 << 1)]

    //----------------------------------------------------------
    // IP: clk_jpd
    //----------------------------------------------------------
    ldr         r0, =0x01
    strb        r0, [r1, #((0x6a << 1) + 1)]            //Set CLK_NJPD to 144MHz (Disabled)

    //----------------------------------------------------------
    // IP: clk_mvd
    //----------------------------------------------------------
    ldr         r0, =0x01                               //Set CLK_MVD to 160MHz (Disabled)
    strb        r0, [r1, #(0x72 << 1)]
    ldr         r0, =0x01                               //Set CLK_MVD_LUMMA_A (Disabled)
    strb        r0, [r1, #((0x74 << 1) + 1)]
    ldr         r0, =0x0101                             //Set CLK_MVD_LUMMA_B (Disabled)
    str         r0, [r1, #(0x76 << 1)]                  //Set CLK_MVD_LUMMA_C (Disabled)
    ldr         r0, =0x0101                             //Set CLK_MVD_RMEM_C (Disabled)
    str         r0, [r1, #(0x78 << 1)]                  //Set CLK_MVD_RMEM1_C (Disabled)
    ldr         r0, =0x01                               //Set CLK_MVD_CHROMA (Disabled)
    strb        r0, [r1, #(0x7a << 1)]
    ldr         r0, =0x01                               //Set CLK_MVD_RREFDAT (Disabled)
    strb        r0, [r1, #(0x7c << 1)]

    //----------------------------------------------------------
    // IP: clk_gop
    //----------------------------------------------------------
    ldr         r0, =0x0000                             //Set CLK_GOPG 0/1 to clk_odclk_p (Enabled)
    str         r0, [r1, #(0x80 << 1)]
    ldr         r0, =0x00                               //Set CLK_GOPG 2 to clk_odclk_p (Enabled)
    strb        r0, [r1, #(0x82 << 1)]

    ldr         r0, =0x0000                             //Set CLK_PSRAM 0/1 (Enabled)
    str         r0, [r1, #(0x86 << 1)]
    ldr         r0, =0x0000                             // gop2 lb[7:4], gop1 lb[3:0], selec their own clock
    str         r0, [r1, #(0x8a << 1)]

    //----------------------------------------------------------
    // IP: clk_ge
    //----------------------------------------------------------
    ldr         r0, =0x00                               //Set CLK_GE to 256bit MIU clock (Enabled)
    strb        r0, [r1, #(0x90 << 1)]

    //----------------------------------------------------------
    // IP: clk_mvop
    //----------------------------------------------------------
    ldr         r0, =0x0101                             //Set CLK_DC0 to synchronous mode (Disabled)
    strb        r0, [r1, #(0x98 << 1)]                  //Set CLK_DC1 to synchronous mode (Disabled)
    ldr         r0, =0x0101                             //Set CLK_SUB_DC0 to synchronous mode (Disabled)
    strb        r0, [r1, #(0x9a << 1)]                  //Set CLK_SUB_DC1 to synchronous mode (Disabled)

    //----------------------------------------------------------
    // clk_sc
    //----------------------------------------------------------
    ldr         r0, =0x0808
    str         r0, [r1, #(0x9c << 1)]            //turn on clk_edclk -> 345
    ldr         r0, =0x0044
    str         r0, [r1, #(0xa6 << 1)]            //turn on clk_odclk -> lpll/2
    ldr         r0, =0x0000
    str         r0, [r1, #(0xba << 1)]            //turn on clk_fmclk
    ldr         r0, =0x0000
    str         r0, [r1, #(0xa2 << 1)]            //turn on clk_ficlk
    ldr         r0, =0x0800
    str         r0, [r1, #(0xa4 << 1)]            //turn on clk_fclk -> 345
    ldr         r0, =0x04
    strb        r0, [r1, #((0xa8 << 1) + 1)]      // idclk_f1 selection -> dvi
    ldr         r0, =0x0000
    str         r0, [r1, #(0xaa << 1)]            // idclk_f2 selection -> adc
    ldr         r0, =0x0008
    str         r0, [r1, #(0xae << 1)]            //turn on clk_fifo_mini -> lpll/2
    ldr         r0, =0x0000
    str         r0, [r1, #(0xb0 << 1)]            //turn on clk_tx_mod
    ldr         r0, =0x0804
    str         r0, [r1, #(0xb2 << 1)]            // idclk_f3 selection -> dvi
    ldr         r0, =0x0404
    str         r0, [r1, #(0xbe << 1)]            //turn on clk_pdw -> lpll/2

    //----------------------------------------------------------
    // IP: clk_emac
    //----------------------------------------------------------
    ldr         r0, =0x0000                             //Set CLK_EMAC_AHB to 123MHz (Enabled), Set CLK_EMAC_RX to CLK_EMAC_RX_in (25MHz) (Enabled)
    str         r0, [r1, #(0xc0 << 1)]
    ldr         r0, =0x0000                             //Set CLK_EMAC_TX to CLK_EMAC_TX_IN (25MHz) (Enabled), Set CLK_EMAC_TX_REF to CLK_EMAC_TX_IN (50MHz) (Enabled)
    str         r0, [r1, #(0xc2 << 1)]
    ldr         r0, =0x00                               //Set CLK_EMAC_RX_REF to CLK_EMAC_RX_IN (50MHz) (Enabled)
    strb        r0, [r1, #(0xc4 << 1)]

    //----------------------------------------------------------
    // IP: clk_sdio
    //----------------------------------------------------------
    ldr         r0, =0x41                               // CLK_SDIO gating (Disabled)
    strb        r0, [r1, #(0xd2 << 1)]

    //----------------------------------------------------------
    // IP: clk_aesdma
    //----------------------------------------------------------
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x100a00 << 1))
    ldr         r0, =0x14                               //CLK_AESDMA to 288MHz (Enabled)
    strb        r0, [r1, #(0x32 << 1)]

    //----------------------------------------------------------
    // IP: clk_spi
    //----------------------------------------------------------
#if !defined(CONFIG_MSTAR_ROM_BOOT_WITH_NAND_FLASH) && \
    !defined(CONFIG_MSTAR_ROM_BOOT_WITH_EMMC_FLASH)
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x000a00 << 1))
    ldr         r0, =0x0fff                             //Set CSZ deselect time to 16 SPI clock cycles
                                                        //Set CSZ setup time to 16 SPI clock cycles (Enabled), Set CLK_EMAC_RX to CLK_EMAC_RX_in (25MHz) (Enabled)
    str         r0, [r1, #(0xe2 << 1)]                  //Set CSZ hold time to 16 SPI clock cycles
    ldr         r0, =0x01                               //Enable fast read mode
    strb        r0, [r1, #(0xe4 << 1)]

    ldr         r1, =(MS_BUS_RIU_ADDR + (0x000e00 << 1))
    ldr         r0, =0x04                               //Set clock of clk_spi to 54MHz
    strb        r0, [r1, #((0x40 << 1) + 1)]
    ldr         r0, =0x44                               //Switch clk_spi between clk_mcu_p and clk_ext_xtali_buf
    strb        r0, [r1, #((0x40 << 1) + 1)]
    ldr         r0, =0x80                               //Set clock of MCU to 170MHz
    strb        r0, [r1, #(0x40 << 1)]
#endif

    //----------------------------------------------------------
    // IP: clk_evd_r2
    //----------------------------------------------------------
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x101e00 << 1))
    ldr         r0, =0xc0                               //EVD R2

    strb        r0, [r1, #(0x2b << 1)]

    //----------------------------------------------------------
    // CLK RTC
    //----------------------------------------------------------
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x000e00 << 1))
    ldr         r0, =0x04
    strb        r0, [r1, #(0x44 << 1)]

#if defined(CONFIG_MSTAR_GPU_MANUAL)
    bl          BootRom_SetGPUClock
#endif

    pop         {lr}
    bx          lr

#if defined(CONFIG_MSTAR_GPU_MANUAL)
//-------------------------------------------------------------------------------------------------
// BootRom_SetGPUClock
// @param
// @return None
// @note
//-------------------------------------------------------------------------------------------------
BootRom_SetGPUClock:
    //Set GPU Clock
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x110800 << 1))
    ldr         r0, =CONFIG_GPU_CLOCK
    ldr         r0, [r1, #(0x8c << 1)]
    ldr         r0, [r1, #(0xd2 << 1)]
    orr         r0, r0, #0x01
    str         r0, [r1, #(0xd2 << 1)]

    bx          lr
#endif

    .end

