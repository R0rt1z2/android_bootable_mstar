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

#ifndef _BOARD_H_
#include "Board.h"
#endif

#ifndef _DRV_MIU_INIT_H_
#include "drv_miu_init_str.h"
#endif

#ifndef _MIUSEL_M7322_2MIU_H_
#include "MIUSEL_M7322_2MIU.h"
#endif
#ifdef CONFIG_ONEBIN_ENABLE
#include "drvLoadTbl.h"
#endif

    .text
    MESSAGE_DC_ON_STR:          .short  '\r', '\n', 'S', 'T', 'R', '_', 'O', 'N', '\r', '\n', 0x00

    .align      4
    .global     BootRom_InitMiu_STR
BootRom_InitMiu_STR:

    push        {lr}
    DPUTK_STR   MESSAGE_DC_ON_STR

#ifdef CONFIG_MSTAR_TITANIA_BD_FPGA

    // MIU reset & mask all
    WREG        (MS_BUS_RIU_ADDR + (0x10121e << 1)), 0x0c01
    WREG        (MS_BUS_RIU_ADDR + (0x10061e << 1)), 0x0c01
    WREG        (MS_BUS_RIU_ADDR + (0x16201e << 1)), 0x0c01
    WREG        (MS_BUS_RIU_ADDR + (0x1615E6 << 1)), 0x7ffe
    WREG        (MS_BUS_RIU_ADDR + (0x1622E6 << 1)), 0x7ffe

    // MIU0
    WREG        (MS_BUS_RIU_ADDR + (0x101202 << 1)), 0x0800
    WREG        (MS_BUS_RIU_ADDR + (0x101204 << 1)), 0x01c5
    WREG        (MS_BUS_RIU_ADDR + (0x101206 << 1)), 0x0000
    WREG        (MS_BUS_RIU_ADDR + (0x101228 << 1)), 0x0100
    WREG        (MS_BUS_RIU_ADDR + (0x152b20 << 1)), 0x1200
    WREG        (MS_BUS_RIU_ADDR + (0x152b22 << 1)), 0x1101
    WREG        (MS_BUS_RIU_ADDR + (0x152b40 << 1)), 0x0f0f
    WREG        (MS_BUS_RIU_ADDR + (0x152b42 << 1)), 0x081f
    WREG        (MS_BUS_RIU_ADDR + (0x152b44 << 1)), 0x2f08
    WREG        (MS_BUS_RIU_ADDR + (0x152b46 << 1)), 0x0f01
    WREG        (MS_BUS_RIU_ADDR + (0x152b48 << 1)), 0x070f
    WREG        (MS_BUS_RIU_ADDR + (0x152b4a << 1)), 0x0101
    WREG        (MS_BUS_RIU_ADDR + (0x152b4e << 1)), 0x000e
    WREG        (MS_BUS_RIU_ADDR + (0x152b50 << 1)), 0x000e
    WREG        (MS_BUS_RIU_ADDR + (0x152b54 << 1)), 0x0f09
    WREG        (MS_BUS_RIU_ADDR + (0x152b5e << 1)), 0x0020
    WREG        (MS_BUS_RIU_ADDR + (0x152b60 << 1)), 0x0031
    WREG        (MS_BUS_RIU_ADDR + (0x152b80 << 1)), 0x80f0
    WREG        (MS_BUS_RIU_ADDR + (0x152b86 << 1)), 0x0000
    WREG        (MS_BUS_RIU_ADDR + (0x152bde << 1)), 0x8002
    WREG        (MS_BUS_RIU_ADDR + (0x161580 << 1)), 0x0000

    //MIU1
    WREG        (MS_BUS_RIU_ADDR + (0x100602 << 1)), 0x0800
    WREG        (MS_BUS_RIU_ADDR + (0x100604 << 1)), 0x01c5
    WREG        (MS_BUS_RIU_ADDR + (0x100606 << 1)), 0x0000
    WREG        (MS_BUS_RIU_ADDR + (0x100628 << 1)), 0x0100
    WREG        (MS_BUS_RIU_ADDR + (0x152c20 << 1)), 0x1200
    WREG        (MS_BUS_RIU_ADDR + (0x152c22 << 1)), 0x1101
    WREG        (MS_BUS_RIU_ADDR + (0x152c40 << 1)), 0x0f0f
    WREG        (MS_BUS_RIU_ADDR + (0x152c42 << 1)), 0x081f
    WREG        (MS_BUS_RIU_ADDR + (0x152c44 << 1)), 0x2f08
    WREG        (MS_BUS_RIU_ADDR + (0x152c46 << 1)), 0x0f01
    WREG        (MS_BUS_RIU_ADDR + (0x152c48 << 1)), 0x070f
    WREG        (MS_BUS_RIU_ADDR + (0x152c4a << 1)), 0x0101
    WREG        (MS_BUS_RIU_ADDR + (0x152c4e << 1)), 0x000e
    WREG        (MS_BUS_RIU_ADDR + (0x152c50 << 1)), 0x000e
    WREG        (MS_BUS_RIU_ADDR + (0x152c54 << 1)), 0x0f09
    WREG        (MS_BUS_RIU_ADDR + (0x152c5e << 1)), 0x0020
    WREG        (MS_BUS_RIU_ADDR + (0x152c60 << 1)), 0x0031
    WREG        (MS_BUS_RIU_ADDR + (0x152c80 << 1)), 0x80f0
    WREG        (MS_BUS_RIU_ADDR + (0x152c86 << 1)), 0x0000
    WREG        (MS_BUS_RIU_ADDR + (0x152cde << 1)), 0x8002
    WREG        (MS_BUS_RIU_ADDR + (0x162280 << 1)), 0x0000

    //MIU2
    WREG        (MS_BUS_RIU_ADDR + (0x162002 << 1)), 0x0800
    WREG        (MS_BUS_RIU_ADDR + (0x162004 << 1)), 0x01c5
    WREG        (MS_BUS_RIU_ADDR + (0x162006 << 1)), 0x0000
    WREG        (MS_BUS_RIU_ADDR + (0x162028 << 1)), 0x0100
    WREG        (MS_BUS_RIU_ADDR + (0x152d20 << 1)), 0x1200
    WREG        (MS_BUS_RIU_ADDR + (0x152d22 << 1)), 0x1101
    WREG        (MS_BUS_RIU_ADDR + (0x152d40 << 1)), 0x0f0f
    WREG        (MS_BUS_RIU_ADDR + (0x152d42 << 1)), 0x081f
    WREG        (MS_BUS_RIU_ADDR + (0x152d44 << 1)), 0x2f08
    WREG        (MS_BUS_RIU_ADDR + (0x152d46 << 1)), 0x0f01
    WREG        (MS_BUS_RIU_ADDR + (0x152d48 << 1)), 0x070f
    WREG        (MS_BUS_RIU_ADDR + (0x152d4a << 1)), 0x0104
    WREG        (MS_BUS_RIU_ADDR + (0x152d4e << 1)), 0x000e
    WREG        (MS_BUS_RIU_ADDR + (0x152d50 << 1)), 0x000e
    WREG        (MS_BUS_RIU_ADDR + (0x152d54 << 1)), 0x0f09
    WREG        (MS_BUS_RIU_ADDR + (0x152d5e << 1)), 0x0020
    WREG        (MS_BUS_RIU_ADDR + (0x152d60 << 1)), 0x0031
    WREG        (MS_BUS_RIU_ADDR + (0x152d80 << 1)), 0x8010
    WREG        (MS_BUS_RIU_ADDR + (0x152d86 << 1)), 0x0000
    WREG        (MS_BUS_RIU_ADDR + (0x152dde << 1)), 0x8002

    //---------------------------
    //program dig general setting
    //---------------------------
    WREG        (MS_BUS_RIU_ADDR + (0x152b26 << 1)), 0x0201
    WREG        (MS_BUS_RIU_ADDR + (0x152c26 << 1)), 0x0201
    WREG        (MS_BUS_RIU_ADDR + (0x152d26 << 1)), 0x0100

    WREG        (MS_BUS_RIU_ADDR + (0x1012fe << 1)), 0xa000

    //---------------------------
    //MIU SW RESET
    //---------------------------
    WREG        (MS_BUS_RIU_ADDR + (0x10121e << 1)), 0x8c00
    WREG        (MS_BUS_RIU_ADDR + (0x10061e << 1)), 0x8c00
    WREG        (MS_BUS_RIU_ADDR + (0x16201e << 1)), 0x8c00


    //---------------------------
    //MIU DRAM INIT 1
    //---------------------------
    WREG        (MS_BUS_RIU_ADDR + (0x100600 << 1)), 0x0008
    DELAYUS     100
    WREG        (MS_BUS_RIU_ADDR + (0x100600 << 1)), 0x000c
    DELAYUS     100
    WREG        (MS_BUS_RIU_ADDR + (0x100600 << 1)), 0x000e
    DELAYUS     100
    WREG        (MS_BUS_RIU_ADDR + (0x100600 << 1)), 0x001f
    DELAYUS     100

    //---------------------------
    //MIU DRAM INIT 2
    //---------------------------
    WREG        (MS_BUS_RIU_ADDR + (0x162000 << 1)), 0x0008
    DELAYUS     100
    WREG        (MS_BUS_RIU_ADDR + (0x162000 << 1)), 0x000c
    DELAYUS     100
    WREG        (MS_BUS_RIU_ADDR + (0x162000 << 1)), 0x000e
    DELAYUS     100
    WREG        (MS_BUS_RIU_ADDR + (0x162000 << 1)), 0x001f
    DELAYUS     100

    //---------------------------
    //MIU DRAM INIT 0
    //---------------------------
    WREG        (MS_BUS_RIU_ADDR + (0x101200 << 1)), 0x0008
    DELAYUS     100
    WREG        (MS_BUS_RIU_ADDR + (0x101200 << 1)), 0x000c
    DELAYUS     100
    WREG        (MS_BUS_RIU_ADDR + (0x101200 << 1)), 0x000e
    DELAYUS     100
    WREG        (MS_BUS_RIU_ADDR + (0x101200 << 1)), 0x001f

_CheckDramData:
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x101200 << 1))
    ldr         r0, [r1]
    and         r0, r0, #0x8000
    cmp         r0, #0
    beq         _CheckDramData

    //unmask client
    WREG        (MS_BUS_RIU_ADDR + (0x1615E6 << 1)), 0x0000
    WREG        (MS_BUS_RIU_ADDR + (0x1622E6 << 1)), 0x0000

    ldr         r1, =(MS_BUS_RIU_ADDR + (0x101200 << 1))
    ldr         r0, =0x8c08
    str         r0, [r1, #(0x1e << 1)]
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x100600 << 1))
    ldr         r0, =0x8c08
    str         r0, [r1, #(0x1e << 1)]
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x162000 << 1))
    ldr         r0, =0x8c08
    str         r0, [r1, #(0x1e << 1)]


    DPUTK_STR   MIU_MESSAGE_BIST0
    //BIST test
    bl          Miu_BistTest
#else

    bl          MIU_Secure_flow_str

    ldr         r0, =MIU_PreInit_Str
    bl          MDrv_WriteRegTbl32

    DELAYUS     4000

#if !defined(CONFIG_MIU0_DRAM_NONE)
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x110d00 << 1))

#ifdef CONFIG_ONEBIN_ENABLE
    bl          Get_Packge_CFG @put result to r1
    ldr         r0, =MIUSTRBuffer
    ldr         r2, =MIU0_STR_Pre
    bl          drv_LoadTbl
    ldr         r0, =MIUSTRBuffer
#else
    ldr         r0, =MIU0_DDR_PreInit_Str
#endif
    bl          MDrv_WriteRegTbl32

    DELAYUS     1000

    ldr         r1, =(MS_BUS_RIU_ADDR + (0x110d00 << 1)) //release reset VCO
    ldr         r0, =0x0000
    str         r0, [r1, #(0x8c << 1)]

#endif

#if !defined(CONFIG_MIU1_DRAM_NONE)
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x161600 << 1))

#ifdef CONFIG_ONEBIN_ENABLE
    bl          Get_Packge_CFG @put result to r1
    ldr         r0, =MIUSTRBuffer
    ldr         r2, =MIU1_STR_Pre
    bl          drv_LoadTbl
    ldr         r0, =MIUSTRBuffer
#else
    ldr         r0, =MIU1_DDR_PreInit_Str
#endif
    bl          MDrv_WriteRegTbl32

    DELAYUS     1000

    ldr         r1, =(MS_BUS_RIU_ADDR + (0x161600 << 1)) //release reset VCO
    ldr         r0, =0x0000
    str         r0, [r1, #(0x8c << 1)]

#endif

    ldr         r1, =(MS_BUS_RIU_ADDR + (0x110d00 << 1)) //mclk_ldo_pd=0
    ldr         r0, =0x0000
    str         r0, [r1, #(0x84 << 1)]

    DELAYUS     10000

#if !defined(CONFIG_MIU0_DRAM_NONE)
#if defined(CONFIG_ENABLE_MIU_SSC)
    ldr         r0, =MIU0_EnableSSC
    bl          MDrv_WriteRegTbl32
#endif

    ldr         r1, =(MS_BUS_RIU_ADDR + (0x110d00 << 1))
    ldr         r0, =0x0000
    str         r0, [r1, #(0x22 << 1)]
    ldr         r0, =0x0114
    str         r0, [r1, #(0x2c << 1)]

#endif /* CONFIG_MIU0_DRAM_NONE */

#if !defined(CONFIG_MIU1_DRAM_NONE)
#if defined(CONFIG_ENABLE_MIU_SSC)
    ldr         r0, =MIU1_EnableSSC
    bl          MDrv_WriteRegTbl32
#endif

    DELAYUS     100

    ldr         r1, =(MS_BUS_RIU_ADDR + (0x161600 << 1))
    ldr         r0, =0x0000
    str         r0, [r1, #(0x22 << 1)]
    ldr         r0, =0x0114
    str         r0, [r1, #(0x2c << 1)]
#endif

    DELAYUS     1000

#if !defined(CONFIG_MIU0_DRAM_NONE)

#ifdef CONFIG_ONEBIN_ENABLE
    bl          Get_Packge_CFG @put result to r1
    ldr         r0, =MIUSTRBuffer
    ldr         r2, =MIU0_STR
    bl          drv_LoadTbl
    ldr         r0, =MIUSTRBuffer
#else
    ldr         r0, =MIU0_DDR_Init_Str
#endif
    bl          MDrv_WriteRegTbl32

#if defined(CONFIG_MSTAR_BD_MST148B_10NQAT_18115_M7322) \
 || defined(CONFIG_MSTAR_BD_MST148B_10NQAT_18115_M7322_ARMANI) \
 || defined(CONFIG_MSTAR_BD_MST148B_10NQAT_18115_M7322_TOPOAZ) \
 || defined(CONFIG_MSTAR_BD_MST148B_10NQAT_18115_M7322_CHIEF) \
 || defined(CONFIG_MSTAR_BD_M7322_MB175) \
 || defined(CONFIG_MSTAR_BD_M7322_MB170) \
 || defined(CONFIG_MSTAR_BD_M7322_G22) \
 || defined(CONFIG_MSTAR_BD_M7322_G22_abc123) \
 || defined(CONFIG_MSTAR_BD_M7322_abc123)
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x101e00 << 1))
    ldr         r0, [r1, #(0xc0 << 1)]
    lsr         r0, r0, #0x0a
    and         r0, r0, #0x01
    cmp         r0, #0x00
    beq         MIU0_MCP_512M_STR

    // Set for MCP 1G setting (default use MCP 512M setting)
    ldr         r0, =MIU0_DDR_Init_MCP_1G_Str
    bl          MDrv_WriteRegTbl32

    ldr         r1, =(MS_BUS_RIU_ADDR + (0x101e00 << 1))
    ldr         r0, [r1, #(0xc0 << 1)]
    lsr         r0, r0, #0x09
    and         r0, r0, #0x01
    cmp         r0, #0x01
    beq         MIU0_SAMSUNG_STR

    // Set for MCP 1G Nanya setting
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x110d00 << 1))
    ldr         r0, =MIU0_DDR_Init_Str_MCP_1G_2nd
    bl          MDrv_WriteRegTbl32
    b           MIU0_SAMSUNG_STR

MIU0_MCP_512M_STR:

    ldr         r1, =(MS_BUS_RIU_ADDR + (0x101e00 << 1))
    ldr         r0, [r1, #(0xc0 << 1)]
    lsr         r0, r0, #0x09
    and         r0, r0, #0x01
    cmp         r0, #0x01
    beq         MIU0_SAMSUNG_STR

    ldr         r1, =(MS_BUS_RIU_ADDR + (0x110d00 << 1))
    ldr         r0, =MIU0_DDR_Init_Str_2nd
    bl          MDrv_WriteRegTbl32

MIU0_SAMSUNG_STR:
#endif
#endif /* CONFIG_MIU0_DRAM_NONE */

#if !defined(CONFIG_MIU1_DRAM_NONE)


#ifdef CONFIG_ONEBIN_ENABLE
    bl          Get_Packge_CFG @put result to r1
    ldr         r0, =MIUSTRBuffer
    ldr         r2, =MIU1_STR
    bl          drv_LoadTbl
    ldr         r0, =MIUSTRBuffer
#else
    ldr         r0, =MIU1_DDR_Init_Str
#endif
    bl          MDrv_WriteRegTbl32

#endif /* CONFIG_MIU1_DRAM_NONE */

    bl          MIU_Arbiter_Setting_Str
    bl          Release_Sw_Reset_Str
    bl          MIU_Post_inivec_Str

    DELAYUS     200                         //delay 200us

#if !defined(CONFIG_MIU1_DRAM_NONE)
    //MIU1 INIT Cycle
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x100600 << 1))
    ldr         r0, =0x0000                 //DRAM reset
    str         r0, [r1, #(0x00 << 1)]

    DELAYUS     200

    ldr         r0, =0x0008                 //DRAM reset
    str         r0, [r1, #(0x00 << 1)]
    ldr         r0, =0x000c                 //DRAM reset+CS
    str         r0, [r1, #(0x00 << 1)]

    DELAYUS     1000

    ldr         r0, =0x000e                 //DRAM reset+CS+CKE
    str         r0, [r1, #(0x00 << 1)]

    DELAYUS     1                           //delay 1us

    ldr         r0, =0x001f                 //DRAM reset+CS+CKE+auto cycle+ODE
    str         r0, [r1, #(0x00 << 1)]

#endif /* CONFIG_MIU1_DRAM_NONE */

#if !defined(CONFIG_MIU0_DRAM_NONE)
    //MIU0 INIT Cycle
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x101200 << 1))
    ldr         r0, =0x0000                 //DRAM reset
    str         r0, [r1, #(0x00 << 1)]

    DELAYUS     200

    ldr         r0, =0x0008                 //DRAM reset
    str         r0, [r1, #(0x00 << 1)]
    ldr         r0, =0x000c                 //DRAM reset+CS
    str         r0, [r1, #(0x00 << 1)]

    DELAYUS     1000

    ldr         r0, =0x000e                 //DRAM reset+CS+CKE
    str         r0, [r1, #(0x00 << 1)]

    DELAYUS     1                           //delay 1us

    ldr         r0, =0x001f                 //DRAM reset+CS+CKE+auto cycle+ODE
    str         r0, [r1, #(0x00 << 1)]
#endif /* CONFIG_MIU0_DRAM_NONE */

#if !defined(CONFIG_MIU0_DRAM_NONE)
    // wait for miu0 init done only (for m7322), miu1 is no need
CheckMiu0Init:
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x101200 << 1))
    ldr         r0, [r1, #(0x00 << 1)]
    and         r0, r0, #0x8000
    cmp         r0, #0
    beq         CheckMiu0Init

    WREG        (MS_BUS_RIU_ADDR + (0x152b36 << 1)), 0x0000
    WREG        (MS_BUS_RIU_ADDR + (0x110d14 << 1)), 0x0e00
#endif

#if !defined(CONFIG_MIU1_DRAM_NONE)
    WREG        (MS_BUS_RIU_ADDR + (0x152c36 << 1)), 0x0000
    WREG        (MS_BUS_RIU_ADDR + (0x161614 << 1)), 0x0e00
#endif

// Set DRAM size for OPTEE tsp used
#if !defined(CONFIG_MIU0_DRAM_NONE)
#if defined(CONFIG_UMA)
    WREG        (MS_BUS_RIU_ADDR + (0x1012d2 << 1)), 0xB000
#else
    WREG        (MS_BUS_RIU_ADDR + (0x1012d2 << 1)), 0x9000
#if !defined(CONFIG_MIU1_DRAM_NONE)
    WREG        (MS_BUS_RIU_ADDR + (0x1006d2 << 1)), 0xA000
#endif
#endif
#endif

//-----------------------------------STR--------------------------------------

    // enter refreseh
    bl          Enter_DDR_RefreshMode

    DELAYUS     1000

    // disable oenz
    bl          Miu_str_oenz_disable

    DELAYUS     1

    // ATOP_PD on
#ifndef CONFIG_MIU0_DRAM_NONE
    WREG        (MS_BUS_RIU_ADDR + (0x110d54 <<1 )), 0xC000 //ATOP_PD off
#endif
#ifndef CONFIG_MIU1_DRAM_NONE
    WREG        (MS_BUS_RIU_ADDR + (0x161654 <<1 )), 0xC000 //ATOP_PD off
#endif

    // rx enable
#ifndef CONFIG_MIU0_DRAM_NONE
#if defined(CONFIG_MIU0_DDR4_2133) || defined(CONFIG_MIU0_DDR4_2400) || defined(CONFIG_MIU0_DDR4_2666) || defined(CONFIG_MIU0_DDR4_3000)
    WREG        (MS_BUS_RIU_ADDR + (0x110d08 << 1)), 0x007f
#else
    WREG        (MS_BUS_RIU_ADDR + (0x110d08 << 1)), 0x003f
#endif
#endif
#ifndef CONFIG_MIU1_DRAM_NONE
#if defined(CONFIG_MIU0_DDR4_2133) || defined(CONFIG_MIU0_DDR4_2400) || defined(CONFIG_MIU0_DDR4_2666) || defined(CONFIG_MIU0_DDR4_3000)
   WREG        (MS_BUS_RIU_ADDR + (0x161608 << 1)), 0x007f
#else
   WREG        (MS_BUS_RIU_ADDR + (0x161608 << 1)), 0x003f
#endif
#endif

    // DQSM RST
#if !defined(CONFIG_MIU0_DRAM_NONE)
    WREG        (MS_BUS_RIU_ADDR + (0x110d1e << 1)), 0x0005
    WREG        (MS_BUS_RIU_ADDR + (0x110d1e << 1)), 0x000f
    WREG        (MS_BUS_RIU_ADDR + (0x110d1e << 1)), 0x0005

#endif

#if !defined(CONFIG_MIU1_DRAM_NONE)
    WREG        (MS_BUS_RIU_ADDR + (0x16161e << 1)), 0x0005
    WREG        (MS_BUS_RIU_ADDR + (0x16161e << 1)), 0x000f
    WREG        (MS_BUS_RIU_ADDR + (0x16161e << 1)), 0x0005
#endif

    // exit refresh
    bl          Exit_DDR_RefreshMode

    DELAYUS     300

    // push dram
    // miu0
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x110D00 << 1))
    ldr         r2, [r1, #(0x1E << 1)]
    orr         r2, r2, #0x05   //set bit1 & bit3 = 1
    str         r2, [r1, #(0x1E << 1)]

    ldr         r2, [r1, #(0x1E << 1)]
    orr         r2, r2, #0x0A   //set bit0 & bit2 = 1
    str         r2, [r1, #(0x1E << 1)]

    ldr         r2, [r1, #(0x1E << 1)]
    bic         r2, r2, #0x0A   // clear bit1 & bit3
    str         r2, [r1, #(0x1E << 1)]

    ldr         r2, [r1, #(0x1E << 1)]
    orr         r2, r2, #0x0A   //set bit0 & bit2 = 1
    str         r2, [r1, #(0x1E << 1)]

    ldr         r2, [r1, #(0x1E << 1)]
    bic         r2, r2, #0x0A   // clear bit1 & bit3
    str         r2, [r1, #(0x1E << 1)]

    // miu1
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x161600 << 1))
    ldr         r2, [r1, #(0x1E << 1)]
    orr         r2, r2, #0x05   //set bit1 & bit3 = 1
    str         r2, [r1, #(0x1E << 1)]

    ldr         r2, [r1, #(0x1E << 1)]
    orr         r2, r2, #0x0A   //set bit0 & bit2 = 1
    str         r2, [r1, #(0x1E << 1)]

    ldr         r2, [r1, #(0x1E << 1)]
    bic         r2, r2, #0x0A   // clear bit1 & bit3
    str         r2, [r1, #(0x1E << 1)]

    ldr         r2, [r1, #(0x1E << 1)]
    orr         r2, r2, #0x0A   //set bit0 & bit2 = 1
    str         r2, [r1, #(0x1E << 1)]

    ldr         r2, [r1, #(0x1E << 1)]
    bic         r2, r2, #0x0A   // clear bit1 & bit3
    str         r2, [r1, #(0x1E << 1)]


//-----------------------------------STR--------------------------------------

    ldr         r0, =MIU_PostInit_Str
    bl          MDrv_WriteRegTbl32

#endif

#ifdef CONFIG_MSTAR_MOBF_ENABLE
    bl          BootRom_InitMobf_MiuStr
#endif

    pop         {lr}
    bx          lr
    .size       BootRom_InitMiu_STR,.-BootRom_InitMiu_STR


//-------------------------------------------------------------------------------------------------
// Miu_str_oenz_disable
// @param  None
// @return None
// @note   Miu_str_oenz_disable
//-------------------------------------------------------------------------------------------------
Miu_str_oenz_disable:

#ifndef CONFIG_MIU0_DRAM_NONE
    ldr         r3, =(MS_BUS_RIU_ADDR + (0x101200 << 1))
    ldrb        r4, [r3, #((0x02 << 1) + 1)]
    bic         r4, r4, #0xf0
    orr         r4, r4, #0x00
    strb        r4, [r3, #((0x02 << 1) + 1)]

    ldr         r3, =(MS_BUS_RIU_ADDR + (0x110d00 << 1))
    ldr         r4, [r3, #(0x0 << 1)]
    bic         r4, r4, #0x10
    orr         r4, r4, #0x00
    str         r4, [r3, #(0x0 << 1)]

#endif /* CONFIG_MIU0_DRAM_NONE */

#ifndef CONFIG_MIU1_DRAM_NONE
    ldr         r3, =(MS_BUS_RIU_ADDR + (0x100600 << 1))
    ldrb        r4, [r3, #((0x02 << 1) + 1)]
    bic         r4, r4, #0xf0
    orr         r4, r4, #0x00
    strb        r4, [r3, #((0x02 << 1) + 1)]

    ldr         r3, =(MS_BUS_RIU_ADDR + (0x161600 << 1))
    ldr         r4, [r3, #(0x0 << 1)]
    bic         r4, r4, #0x10
    orr         r4, r4, #0x00
    str         r4, [r3, #(0x0 << 1)]

#endif /* CONFIG_MIU1_DRAM_NONE */

    bx          lr

//-------------------------------------------------------------------------------------------------
// Enter_DDR_RefreshMode
// @param  None
// @return None
// @note   Enter_DDR_RefreshMode
//-------------------------------------------------------------------------------------------------
Enter_DDR_RefreshMode:
    WREG        (MS_BUS_RIU_ADDR + (0x1012e0 << 1)), 0x0000
    WREG        (MS_BUS_RIU_ADDR + (0x1006e0 << 1)), 0x0000

#ifndef CONFIG_MIU0_DRAM_NONE
    // [DDR3][enter] <MIU0>
    ldr         r3, =(MS_BUS_RIU_ADDR + (0x101200 << 1))
    ldr         r4, =0x8c00
    str         r4, [r3, #(0x1e << 1)]

    //arb4, client 1 = bist
    ldr         r3, =(MS_BUS_RIU_ADDR + (0x152000 << 1))
    ldr         r4, =0xfffd
    str         r4, [r3, #(0x06 << 1)]

    //ctrl arb, client 0 = miu_cmd, client f = BIST
    ldr         r3, =(MS_BUS_RIU_ADDR + (0x161500 << 1))
    ldr         r4, =0x7ffe
    str         r4, [r3, #(0xe6 << 1)]
    DELAYUS     2000

    //---------------------------
    //Pre-charge all
    //---------------------------
    ldr         r3, =(MS_BUS_RIU_ADDR + (0x152b00 << 1))
    ldr         r4, [r3, #(0x1a << 1)]                            //[1]     : reg_individual_mr_set_off = 1
    bic         r4, r4, #0x02
    orr         r4, r4, #0x02
    str         r4, [r3, #(0x1a << 1)]
    ldr         r4, =0x0400
    str         r4, [r3, #(0x1c << 1)]                            //          reg_mrx = 0400 (precharege all)
    ldr         r4, [r3, #(0x14 << 1)]                            //[0]     : reg_auto_ref_off          = 1
    bic         r4, r4, #0x01
    orr         r4, r4, #0x01
    str         r4, [r3, #(0x14 << 1)]
    ldr         r4, [r3, #(0x1a << 1)]                            //[0]     : reg_single_cmd_old_mode   = 1
    bic         r4, r4, #0x01
    orr         r4, r4, #0x01
    str         r4, [r3, #(0x1a << 1)]
    ldr         r4, [r3, #(0x1e << 1)]                            //[3:0]   : reg_single_cmd            = 2 (precharege)
    bic         r4, r4, #0x0f
    orr         r4, r4, #0x02
    str         r4, [r3, #(0x1e << 1)]
    ldrb        r4, [r3, #((0x1e << 1) + 1)]                        //[14]    : reg_single_cmd_en         = 1
    bic         r4, r4, #0x40
    orr         r4, r4, #0x40
    strb        r4, [r3, #((0x1e << 1) + 1)]
    ldr         r4, [r3, #(0x1e << 1)]                            //[3:0]   : reg_single_cmd            = 1 (refresh)
    bic         r4, r4, #0x0f
    orr         r4, r4, #0x01
    str         r4, [r3, #(0x1e << 1)]
    ldrb        r4, [r3, #((0x1e << 1) + 1)]                           //[14]    : reg_single_cmd_en         = 1
    bic         r4, r4, #0x40
    orr         r4, r4, #0x40
    strb         r4, [r3, #((0x1e << 1) + 1)]
    ldr         r4, =0x0000
    str         r4, [r3, #(0x1e << 1)]                            //[15:0]  : clear single cmd

    ldr         r3, =(MS_BUS_RIU_ADDR + (0x161500 << 1))
    ldr         r4, =0xffff
    str         r4, [r3, #(0xe6 << 1)]                            //[15:0]  : turn off cmd request

    DELAYUS     1

    //---------------------------
    //Enter self-refresh
    //---------------------------
    ldr         r3, =(MS_BUS_RIU_ADDR + (0x152b00 << 1))
    ldr         r4, [r3, #(0x14 << 1)]                            //[1]     : reg_self_refresh          = 1
    bic         r4, r4, #0x02
    orr         r4, r4, #0x02
    str         r4, [r3, #(0x14 << 1)]
#endif /* CONFIG_MIU0_DRAM_NONE */

#ifndef CONFIG_MIU1_DRAM_NONE
    // [DDR3][enter] <MIU1>
    ldr         r3, =(MS_BUS_RIU_ADDR + (0x100600 << 1))
    ldr         r4, =0x8c00
    str         r4, [r3, #(0x1e << 1)]

    ldr         r3, =(MS_BUS_RIU_ADDR + (0x152100 << 1))
    ldr         r4, =0xfffd
    str         r4, [r3, #(0x06 << 1)]

    ldr         r3, =(MS_BUS_RIU_ADDR + (0x162200 << 1))
    ldr         r4, =0x7ffe
    str         r4, [r3, #(0xe6 << 1)]

    DELAYUS     2000
    //---------------------------
    //Pre-charge all
    //---------------------------
    ldr         r3, =(MS_BUS_RIU_ADDR + (0x152c00 << 1))
    ldr         r4, [r3, #(0x1a << 1)]                            //[1]     : reg_individual_mr_set_off = 1
    bic         r4, r4, #0x02
    orr         r4, r4, #0x02
    str         r4, [r3, #(0x1a << 1)]

    ldr         r4, =0x0400
    str         r4, [r3, #(0x1c << 1)]                            //          reg_mrx = 0400 (precharege all)

    ldr         r4, [r3, #(0x14 << 1)]                            //[0]     : reg_auto_ref_off          = 1
    bic         r4, r4, #0x01
    orr         r4, r4, #0x01
    str         r4, [r3, #(0x14 << 1)]

    ldr         r4, [r3, #(0x1a << 1)]                            //[0]     : reg_single_cmd_old_mode   = 1
    bic         r4, r4, #0x01
    orr         r4, r4, #0x01
    str         r4, [r3, #(0x1a << 1)]

    ldr         r4, [r3, #(0x1e << 1)]                            //[3:0]   : reg_single_cmd            = 2 (precharege)
    bic         r4, r4, #0x0f
    orr         r4, r4, #0x02
    str         r4, [r3, #(0x1e << 1)]

    ldrb        r4, [r3, #((0x1e << 1) + 1)]                           //[14]    : reg_single_cmd_en         = 1
    bic         r4, r4, #0x40
    orr         r4, r4, #0x40
    strb        r4, [r3, #((0x1e << 1) + 1)]

    ldr         r4, [r3, #(0x1e << 1)]                            //[3:0]   : reg_single_cmd            = 1 (refresh)
    bic         r4, r4, #0x0f
    orr         r4, r4, #0x01
    str         r4, [r3, #(0x1e << 1)]

    ldrb        r4, [r3, #((0x1e << 1) + 1)]                            //[14]    : reg_single_cmd_en         = 1
    bic         r4, r4, #0x40
    orr         r4, r4, #0x40
    strb        r4, [r3, #((0x1e << 1) + 1)]

    ldr         r4, =0x0000
    str         r4, [r3, #(0x1e << 1)]                            //[15:0]  : clear single cmd

    ldr         r3, =(MS_BUS_RIU_ADDR + (0x162200 << 1))
    ldr         r4, =0xffff
    str         r4, [r3, #(0xe6 << 1)]                            //[15:0]  : turn off cmd request

    DELAYUS     1

    //---------------------------
    //Enter self-refresh
    //---------------------------
    ldr         r3, =(MS_BUS_RIU_ADDR + (0x152c00 << 1))
    ldr         r4, [r3, #(0x14 << 1)]                            //[1]     : reg_self_refresh          = 1
    bic         r4, r4, #0x02
    orr         r4, r4, #0x02
    str         r4, [r3, #(0x14 << 1)]
#endif /* CONFIG_MIU1_DRAM_NONE */

    bx          lr

//-------------------------------------------------------------------------------------------------
// Exit_DDR_RefreshMode
// @param  None
// @return None
// @note   Exit_DDR_RefreshMode
//-------------------------------------------------------------------------------------------------
Exit_DDR_RefreshMode:

#ifndef CONFIG_MIU0_DRAM_NONE
    // [DDR4][DDR3][Exit Self-Refresh Start]
    ldr         r3, =(MS_BUS_RIU_ADDR + (0x152b00 << 1))
    ldr         r4, [r3, #(0x14 << 1)]                            //[1]    : reg_self_refresh          = 0
    bic         r4, r4, #0x02
    str         r4, [r3, #(0x14 << 1)]

    ldr         r3, =(MS_BUS_RIU_ADDR + (0x161500 << 1))           // turn on miu_cmd client
    ldr         r4, =0xfffe
    str         r4, [r3, #(0xe6 << 1)]
#endif /* CONFIG_MIU0_DRAM_NONE */

#ifndef CONFIG_MIU1_DRAM_NONE
    // [DDR4][DDR3][Exit Self-Refresh Start]
    ldr         r3, =(MS_BUS_RIU_ADDR + (0x152c00 << 1))
    ldr         r4, [r3, #(0x14 << 1)]                            //[1]    : reg_self_refresh          = 0
    bic         r4, r4, #0x02
    str         r4, [r3, #(0x14 << 1)]

    ldr         r3, =(MS_BUS_RIU_ADDR + (0x162200 << 1))           // turn on miu_cmd client
    ldr         r4, =0xfffe
    str         r4, [r3, #(0xe6 << 1)]
#endif /* CONFIG_MIU1_DRAM_NONE */

    DELAYUS     1

#ifndef CONFIG_MIU0_DRAM_NONE
    //------------------------------------------
    //Insert Refresh cmd & Trun on auto-refresh
    //------------------------------------------
    ldr         r3, =(MS_BUS_RIU_ADDR + (0x152b00 << 1))
    ldr         r4, [r3, #(0x1e << 1)]                            //[ 3: 0]    : reg_single_cmd            = 1 (refresh)
    bic         r4, r4, #0x0f
    orr         r4, r4, #0x01
    str         r4, [r3, #(0x1e << 1)]

    ldrb        r4, [r3, #((0x1e << 1) + 1)]                            //[14]    : reg_single_cmd_en         = 1
    bic         r4, r4, #0x40
    orr         r4, r4, #0x40
    strb        r4, [r3, #((0x1e << 1) + 1)]

    ldr         r4, =0x0000
    str         r4, [r3, #(0x1e << 1)]                            //[15: 0]    : clear single cmd

    ldr         r4, [r3, #(0x14 << 1)]                            //[    0]    : reg_auto_ref_off          = 0
    bic         r4, r4, #0x01
    str         r4, [r3, #(0x14 << 1)]

    str         r4, [r3, #(0x1e << 1)]                            //[15:0]  : clear single cmd

    ldr         r3, =(MS_BUS_RIU_ADDR + (0x101200 << 1))
    ldr         r4, [r3]
    bic         r4, r4, #0x10
    str         r4, [r3]                                          //[    4]    : reg_odt                   = 0

    DELAYUS     50

    //---------------------------
    //Enable ODT
    //---------------------------
    ldr         r3, =(MS_BUS_RIU_ADDR + (0x101200 << 1))
    ldr         r4, [r3]
    bic         r4, r4, #0x10
    orr         r4, r4, #0x10
    str         r4, [r3]

    DELAYUS     2000

    ldr         r3, =(MS_BUS_RIU_ADDR + (0x101200 << 1))
    ldr         r4, =0x8c08
    str         r4, [r3, #(0x1e << 1)]

    ldr         r3, =(MS_BUS_RIU_ADDR + (0x152000 << 1))
    ldr         r4, =0x0000
    str         r4, [r3, #(0x06 << 1)]                            //arb4, client 1 = bist

    ldr         r3, =(MS_BUS_RIU_ADDR + (0x161500 << 1))
    ldr         r4, =0x0000
    str         r4, [r3, #(0xe6 << 1)]                            //ctrl arb, client 0 = miu_cmd, client f = BIST
#endif /* CONFIG_MIU0_DRAM_NONE */

#ifndef CONFIG_MIU1_DRAM_NONE
    //------------------------------------------
    //Insert Refresh cmd & Trun on auto-refresh
    //------------------------------------------
    ldr         r3, =(MS_BUS_RIU_ADDR + (0x152c00 << 1))
    ldr         r4, [r3, #(0x1e << 1)]                            //[ 3: 0]    : reg_single_cmd            = 1 (refresh)
    bic         r4, r4, #0x0f
    orr         r4, r4, #0x01
    str         r4, [r3, #(0x1e << 1)]

    ldrb        r4, [r3, #((0x1e << 1) + 1)]                            //[14]    : reg_single_cmd_en         = 1
    bic         r4, r4, #0x40
    orr         r4, r4, #0x40
    strb        r4, [r3, #((0x1e << 1) + 1)]

    ldr         r4, =0x0000
    str         r4, [r3, #(0x1e << 1)]                            //[15: 0]    : clear single cmd

    ldr         r4, [r3, #(0x14 << 1)]                            //[    0]    : reg_auto_ref_off          = 0
    bic         r4, r4, #0x01
    str         r4, [r3, #(0x14 << 1)]

    str         r4, [r3, #(0x1e << 1)]                            //[15:0]  : clear single cmd

    ldr         r3, =(MS_BUS_RIU_ADDR + (0x100600 << 1))
    ldr         r4, [r3]
    bic         r4, r4, #0x10
    str         r4, [r3]                                          //[    4]    : reg_odt                   = 0

    DELAYUS     50

    //---------------------------
    //Enable ODT
    //---------------------------
    ldr         r3, =(MS_BUS_RIU_ADDR + (0x100600 << 1))
    ldr         r4, [r3]
    bic         r4, r4, #0x10
    orr         r4, r4, #0x10
    str         r4, [r3]

    DELAYUS     2000

    ldr         r3, =(MS_BUS_RIU_ADDR + (0x100600 << 1))
    ldr         r4, =0x8c08
    str         r4, [r3, #(0x1e << 1)]

    ldr         r3, =(MS_BUS_RIU_ADDR + (0x152100 << 1))
    ldr         r4, =0x0000
    str         r4, [r3, #(0x06 << 1)]                            //arb4, client 1 = bist

    ldr         r3, =(MS_BUS_RIU_ADDR + (0x162200 << 1))
    ldr         r4, =0x0000
    str         r4, [r3, #(0xe6 << 1)]                            //ctrl arb, client 0 = miu_cmd, client f = BIST
#endif /* CONFIG_MIU1_DRAM_NONE */

    bx          lr

//-------------------------------------------------------------------------------------------------
// MIU_Arbiter_Setting_Str
// param
// return None
// note
//-------------------------------------------------------------------------------------------------
MIU_Arbiter_Setting_Str:

    //Block Arbiter setting
    WREG        (MS_BUS_RIU_ADDR + (0x310212 << 1)), 0xffff
    WREG        (MS_BUS_RIU_ADDR + (0x310312 << 1)), 0xffff
    WREG        (MS_BUS_RIU_ADDR + (0x310412 << 1)), 0xffff
    WREG        (MS_BUS_RIU_ADDR + (0x310512 << 1)), 0xffff
    WREG        (MS_BUS_RIU_ADDR + (0x310612 << 1)), 0xffff
    WREG        (MS_BUS_RIU_ADDR + (0x310712 << 1)), 0xffff
    WREG        (MS_BUS_RIU_ADDR + (0x310812 << 1)), 0xffff
    WREG        (MS_BUS_RIU_ADDR + (0x310912 << 1)), 0xffff
    WREG        (MS_BUS_RIU_ADDR + (0x311012 << 1)), 0xffff
    WREG        (MS_BUS_RIU_ADDR + (0x310230 << 1)), 0x0011
    WREG        (MS_BUS_RIU_ADDR + (0x310200 << 1)), 0x0303
    WREG        (MS_BUS_RIU_ADDR + (0x310330 << 1)), 0x0011
    WREG        (MS_BUS_RIU_ADDR + (0x310300 << 1)), 0x0303
    WREG        (MS_BUS_RIU_ADDR + (0x310430 << 1)), 0x0011
    WREG        (MS_BUS_RIU_ADDR + (0x310400 << 1)), 0x0303
    WREG        (MS_BUS_RIU_ADDR + (0x310530 << 1)), 0x0011
    WREG        (MS_BUS_RIU_ADDR + (0x310500 << 1)), 0x0303
    WREG        (MS_BUS_RIU_ADDR + (0x310630 << 1)), 0x0011
    WREG        (MS_BUS_RIU_ADDR + (0x310600 << 1)), 0x0303
    WREG        (MS_BUS_RIU_ADDR + (0x310730 << 1)), 0x0011
    WREG        (MS_BUS_RIU_ADDR + (0x310700 << 1)), 0x0303
    WREG        (MS_BUS_RIU_ADDR + (0x310830 << 1)), 0x0011
    WREG        (MS_BUS_RIU_ADDR + (0x310844 << 1)), 0x0003
    WREG        (MS_BUS_RIU_ADDR + (0x310800 << 1)), 0x0303
    WREG        (MS_BUS_RIU_ADDR + (0x310930 << 1)), 0x0011
    WREG        (MS_BUS_RIU_ADDR + (0x310900 << 1)), 0x0303
    WREG        (MS_BUS_RIU_ADDR + (0x311030 << 1)), 0x0011
    WREG        (MS_BUS_RIU_ADDR + (0x311000 << 1)), 0x0303

    //CTRL Arbiter setting
    WREG        (MS_BUS_RIU_ADDR + (0x161534 << 1)), 0x03fe
    WREG        (MS_BUS_RIU_ADDR + (0x161502 << 1)), 0x0320
    WREG        (MS_BUS_RIU_ADDR + (0x161504 << 1)), 0x0000
    WREG        (MS_BUS_RIU_ADDR + (0x161506 << 1)), 0x7ffe
    WREG        (MS_BUS_RIU_ADDR + (0x161508 << 1)), 0x0404
    WREG        (MS_BUS_RIU_ADDR + (0x16150a << 1)), 0xffff

#if !defined(CONFIG_UMA)
    WREG        (MS_BUS_RIU_ADDR + (0x162234 << 1)), 0x03fe
    WREG        (MS_BUS_RIU_ADDR + (0x162202 << 1)), 0x0320
    WREG        (MS_BUS_RIU_ADDR + (0x162204 << 1)), 0x0000
    WREG        (MS_BUS_RIU_ADDR + (0x162206 << 1)), 0x7ffe
    WREG        (MS_BUS_RIU_ADDR + (0x162208 << 1)), 0x0404
    WREG        (MS_BUS_RIU_ADDR + (0x16220a << 1)), 0xffff
#endif

    bx          lr

//-------------------------------------------------------------------------------------------------
// Release_Sw_Reset_Str
// param
// return None
// note
//-------------------------------------------------------------------------------------------------
Release_Sw_Reset_Str:

#if !defined(CONFIG_MIU1_DRAM_NONE)
    //MIU1
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x100600 << 1))
    ldr         r0, =0x8c00
    str         r0, [r1, #(0x1e << 1)]
#endif

#if !defined(CONFIG_MIU0_DRAM_NONE)
    //MIU0
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x101200 << 1))
    ldr         r0, =0x8c00
    str         r0, [r1, #(0x1e << 1)]
#endif

    bx          lr

//-------------------------------------------------------------------------------------------------
// MIU_Post_inivec_Str
// param
// return None
// note
//-------------------------------------------------------------------------------------------------
MIU_Post_inivec_Str:

#if !defined(CONFIG_MIU0_DRAM_NONE)

    ldr         r1, =(MS_BUS_RIU_ADDR + (0x110d00 << 1))
    ldr         r0, [r1, #(0x00 << 1)]
    ldr         r2, =0xFFF8             // BIT[2:0] (mapping setting need keep original value)
    bic         r0, r0, r2
    ldr         r2, =0x2018
    orr         r3, r0, r2
    str         r3, [r1, #(0x00 << 1)]
    ldr         r2, =0x0008
    orr         r3, r0, r2
    str         r3, [r1, #(0x00 << 1)]

    WREG        (MS_BUS_RIU_ADDR + (0x110d18 << 1)), 0x0000
    WREG        (MS_BUS_RIU_ADDR + (0x110d06 << 1)), 0x0000
    WREG        (MS_BUS_RIU_ADDR + (0x110d7c << 1)), 0x0005
    WREG        (MS_BUS_RIU_ADDR + (0x110d54 << 1)), 0xc070
    WREG        (MS_BUS_RIU_ADDR + (0x110d1e << 1)), 0x0005
    WREG        (MS_BUS_RIU_ADDR + (0x110d1e << 1)), 0x000f
    WREG        (MS_BUS_RIU_ADDR + (0x110d1e << 1)), 0x0005

#endif

#if !defined(CONFIG_MIU1_DRAM_NONE)

    ldr         r1, =(MS_BUS_RIU_ADDR + (0x161600 << 1))
    ldr         r0, [r1, #(0x00 << 1)]
    ldr         r2, =0xFFF8             // BIT[2:0] (mapping setting need keep original value)
    bic         r0, r0, r2
    ldr         r2, =0x2018
    orr         r3, r0, r2
    str         r3, [r1, #(0x00 << 1)]
    ldr         r2, =0x0008
    orr         r3, r0, r2
    str         r3, [r1, #(0x00 << 1)]

    WREG        (MS_BUS_RIU_ADDR + (0x161618 << 1)), 0x0000
    WREG        (MS_BUS_RIU_ADDR + (0x161606 << 1)), 0x0000
    WREG        (MS_BUS_RIU_ADDR + (0x16167c << 1)), 0x0005
    WREG        (MS_BUS_RIU_ADDR + (0x161654 << 1)), 0xc070
    WREG        (MS_BUS_RIU_ADDR + (0x16161e << 1)), 0x0005
    WREG        (MS_BUS_RIU_ADDR + (0x16161e << 1)), 0x000f
    WREG        (MS_BUS_RIU_ADDR + (0x16161e << 1)), 0x0005

#endif
    bx          lr

//-------------------------------------------------------------------------------------------------
// MIU_Secure_flow_str
// @param
// @return None
// @note
//-------------------------------------------------------------------------------------------------
MIU_Secure_flow_str:

    //1. Set all DRAM in secure range
    //MIU0
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x123700 << 1))
    ldr         r0, =0x0
    str         r0, [r1]                    // start address low
    str         r0, [r1, #(0x02 << 1)]      // start address high
    str         r0, [r1, #(0x04 << 1)]      // end address low
    ldr         r0, =0x08
    orr         r0, #0x300
    str         r0, [r1, #(0x06 << 1)]      // end address high
    ldr         r0, =0x8308
    str         r0, [r1, #(0x06 << 1)]      // end address high
    ldr         r0, =0x1
    str         r0, [r1, #(0xec << 1)]      // secure range 0 det en

    //MIU1
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x123800 << 1))
    ldr         r0, =0x0
    str         r0, [r1]                    // start address low
    str         r0, [r1, #(0x02 << 1)]      // start address high
    str         r0, [r1, #(0x04 << 1)]      // end address low
    ldr         r0, =0x08
    orr         r0, #0x300
    str         r0, [r1, #(0x06 << 1)]      // end address high
    ldr         r0, =0x8308
    str         r0, [r1, #(0x06 << 1)]      // end address high
    ldr         r0, =0x1
    str         r0, [r1, #(0xec << 1)]      // secure range 0 det en

    //2. Set all RIU to secure, except MIU bank
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x123900 << 1))
    ldr         r0, [r1, #(0x02 << 1)]
    orr         r0, #(0x01 << 2)
    str         r0, [r1, #(0x02 << 1)]
    ldr         r0, =(0x01 << 8)        //0x1006(8)
    str         r0, [r1, #(0x20 << 1)]
    ldr         r0, =(0x01 << 12)        //0x1012(28)
    str         r0, [r1, #(0x22 << 1)]
    ldr         r0, =0xffff
    str         r0, [r1, #(0x24 << 1)]
    str         r0, [r1, #(0x26 << 1)]
    str         r0, [r1, #(0x28 << 1)]
    str         r0, [r1, #(0x2a << 1)]
    ldr         r0, =(0x01 << 6)       	//0x110d(102)
    str         r0, [r1, #(0x2c << 1)]
    ldr         r0, =0x0
    str         r0, [r1, #(0x2e << 1)]
    str         r0, [r1, #(0x30 << 1)]
    str         r0, [r1, #(0x32 << 1)]
    str         r0, [r1, #(0x34 << 1)]
    str         r0, [r1, #(0x36 << 1)]
    str         r0, [r1, #(0x38 << 1)]
    ldr         r0, =(0x03 << 9)       //0x1515(217) 0x1516(218)
    str         r0, [r1, #(0x3a << 1)]
    ldr         r0, =0x0
    str         r0, [r1, #(0x3c << 1)]
    str         r0, [r1, #(0x3e << 1)]
    ldr         r0, =(0x01 << 8)       //0x1616(264)
    str         r0, [r1, #(0x40 << 1)]
    ldr         r0, =0x0
    str         r0, [r1, #(0x42 << 1)]
    str         r0, [r1, #(0x44 << 1)]
    str         r0, [r1, #(0x46 << 1)]
    str         r0, [r1, #(0x48 << 1)]
    str         r0, [r1, #(0x4a << 1)]
    str         r0, [r1, #(0x4c << 1)]
    str         r0, [r1, #(0x4e << 1)]
    str         r0, [r1, #(0x50 << 1)]
    str         r0, [r1, #(0x52 << 1)]
    str         r0, [r1, #(0x54 << 1)]

    //3. enable block 51 patch
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x3900 << 1))
    ldr         r0, =0x01
    str         r0, [r1, #(0x64 << 1)]

    ldr         r1, =(MS_BUS_RIU_ADDR + (0x123900 << 1))
    ldr         r0, [r1, #(0x60 << 1)]
    orr         r0, #(0x01 << 2)
    str         r0, [r1, #(0x60 << 1)]

    ldr         r1, =(MS_BUS_RIU_ADDR + (0x0e00 << 1))
    ldr         r0, =0xbabe
    str         r0, [r1, #(0xaa << 1)]

    //4. announce 51 to access die domain
MIU_SECURE_LOOP:
    ldr         r0, [r1, #(0xaa << 1)]
    ldr         r2, =0xdead
    cmp         r0, r2
    bne         MIU_SECURE_LOOP

#ifndef CONFIG_BOOTING_FROM_EXT_SPI_WITH_PM51
    //5. ARM kill PM51, pull PM51 reset
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x2e00 << 1))
    ldr         r3, [r1, #(0x52 << 1)]
    bic         r3, r3, #0x00001000         @//Reset PM51
    str         r3, [r1, #(0x52 << 1)]
#endif

    //6. ARM Release all DRAM in secure range
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x123700 << 1))
    ldr         r0, =0x0
    str         r0, [r1, #(0x06 << 1)]      // end address high
    str         r0, [r1, #(0x06 << 1)]      // end address high
    str         r0, [r1, #(0xec << 1)]      // secure range 0 det en

    ldr         r1, =(MS_BUS_RIU_ADDR + (0x123800 << 1))
    ldr         r0, =0x0
    str         r0, [r1, #(0x06 << 1)]      // end address high
    str         r0, [r1, #(0x06 << 1)]      // end address high
    str         r0, [r1, #(0xec << 1)]      // secure range 0 det en

    ldr         r1, =(MS_BUS_RIU_ADDR + (0x123900 << 1))
    ldr         r0, =0xffff
    str         r0, [r1, #(0x20 << 1)]
    str         r0, [r1, #(0x22 << 1)]
    str         r0, [r1, #(0x24 << 1)]
    str         r0, [r1, #(0x26 << 1)]
    str         r0, [r1, #(0x28 << 1)]
    str         r0, [r1, #(0x2a << 1)]
    str         r0, [r1, #(0x2c << 1)]
    str         r0, [r1, #(0x2e << 1)]
    str         r0, [r1, #(0x30 << 1)]
    str         r0, [r1, #(0x32 << 1)]
    str         r0, [r1, #(0x34 << 1)]
    str         r0, [r1, #(0x36 << 1)]
    str         r0, [r1, #(0x38 << 1)]
    str         r0, [r1, #(0x3a << 1)]
    str         r0, [r1, #(0x3c << 1)]
    str         r0, [r1, #(0x3e << 1)]
    str         r0, [r1, #(0x40 << 1)]
    str         r0, [r1, #(0x42 << 1)]
    str         r0, [r1, #(0x44 << 1)]
    str         r0, [r1, #(0x46 << 1)]
    str         r0, [r1, #(0x48 << 1)]
    str         r0, [r1, #(0x4a << 1)]
    str         r0, [r1, #(0x4c << 1)]
    str         r0, [r1, #(0x4e << 1)]
    str         r0, [r1, #(0x50 << 1)]
    str         r0, [r1, #(0x52 << 1)]
    str         r0, [r1, #(0x54 << 1)]

    bx          lr

#ifdef CONFIG_MSTAR_MOBF_ENABLE
//-------------------------------------------------------------------------------------------------
// BootRom_InitMobf_MiuStr
// @param
// @return None
// @note
//-------------------------------------------------------------------------------------------------
    .global     BootRom_InitMobf_MiuStr
BootRom_InitMobf_MiuStr:

Obf_CheckTRNG_str_Loop:
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x113D00 << 1))
    ldr         r2, [r1, #(0x06 << 1)]
    and         r2, r2, #0x0002
    cmp         r0, #0
    beq         Obf_CheckTRNG_str_Loop

    ldr         r1, =(MS_BUS_RIU_ADDR + (0x113B00 << 1))
    ldr         r2, [r1, #(0xEC << 1)]
    bic         r2, r2, #0x0060
    str         r2, [r1, #(0xEC << 1)]

    ldr         r3, =0xC000
    orr         r2, r2, r3
    str         r2, [r1, #(0xEC << 1)]

    ldr         r1, =(MS_BUS_RIU_ADDR + (0x113B00 << 1))
    ldr         r2, [r1, #(0xEC << 1)]
    ldr         r3, =0x1000
    orr         r2, r2, r3
    str         r2, [r1, #(0xEC << 1)]

    // Lock
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x113B00 << 1))
    ldr         r2, [r1, #(0xEC << 1)]
    ldr         r3, =0x0400
    orr         r2, r2, r3
    str         r2, [r1, #(0xEC << 1)]

Obf_CheckMiu0_str_Loop:
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x101200 << 1))
    ldr         r2, [r1, #(0x2A << 1)]
    and         r2, r2, #0x8000
    cmp         r0, #0
    beq         Obf_CheckMiu0_str_Loop

Obf_CheckMiu1_str_Loop:
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x100600 << 1))
    ldr         r2, [r1, #(0x2A << 1)]
    and         r2, r2, #0x8000
    cmp         r0, #0
    beq         Obf_CheckMiu1_str_Loop

    bx          lr
#endif
