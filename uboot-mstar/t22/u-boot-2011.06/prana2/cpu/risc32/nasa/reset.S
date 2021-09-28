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

#include "chiptype.h"
#ifndef __AEONR2__	//AEON

#include "risc32_spr.h"

#define REG_BASE           0xA0000000
#define REG_BANK_RISC32    0x100F00
#define REG_BANK_DBG       0x103300

#define HAL_ICACHE_LINE_SIZE    16
#define HAL_DCACHE_LINE_SIZE    16

        .section ".reset","ax"

.macro  hal_icache_init
        /* Invalidate I-Cache */
        l.addi  r3,r0,0
        l.addi  r4,r0,8192// HAL_ICACHE_SIZE
    1:
        l.mtspr r0,r3,SPR_ICBIR
        l.sfne  r3,r4
        l.bf    1b
        l.addi  r3,r3,HAL_ICACHE_LINE_SIZE

        /* Enable I-Cache */
        l.mfspr r3,r0,SPR_SR
        l.ori   r3,r3,SPR_SR_ICE
        l.mtspr r0,r3,SPR_SR

        /* Flush instructions out of instruction buffer */
        l.nop
        l.nop
        l.nop
        l.nop
        l.nop
.endm

.macro  hal_dcache_init
        # Invalidate DC
        l.addi  r3,r0,0
        l.addi  r4,r0,2032           # (HAL_DCACHE_SIZE/HAL_DCACHE_WAYS) - HAL_DCACHE_LINE_SIZE
    1:
        l.invalidate_line   0(r3),0  # invalidate line index=r3, way=0
        l.invalidate_line   0(r3),1  # invalidate line index=r3, way=1
        l.sfne  r3,r4
        l.bf    1b
        l.addi  r3,r3,HAL_DCACHE_LINE_SIZE

        # Enable D-Cache
        l.mfspr r3,r0,SPR_SR
        l.ori   r3,r3,SPR_SR_DCE
        l.mtspr r0,r3,SPR_SR
.endm

.macro  hal_bss_init
        l.movhi r3,hi(__bss_start)
        l.ori   r3,r3,lo(__bss_start)
        l.movhi r4,hi(__bss_end)
        l.ori   r4,r4,lo(__bss_end)

1:      l.sfgeu r3,r4
        l.bf    2f
        l.nop
        l.sw    0(r3),r0
        l.j     1b
        l.addi  r3,r3,4
2:
.endm

        .extern _risc32_regbase
        .extern _start
        .extern _boot_stack

        .org 0x100

        .globl  __reset
__reset:
        # Initialize Supervision Register:
        # Supervisor mode on, all interrupts off, caches off
        l.ori   r3,r0,SPR_SR_SM
        l.mtspr r0,r3,SPR_SR

        #   org:map SRAM to 0xc000_0000~0xc000_0fff
        #   T3:map SRAM to 0xB000_0000~0xB000_0fff
        # qmem_dmask[7:0]
        # qmem_daddr[7:0]
        l.movhi r7,hi(_risc32_regbase)
        l.ori   r6,r7,lo(_risc32_regbase)

        l.ori   r3,r0,0xF000
        l.sw    (0xEA * 2)(r6),r3   # 0x0FEA = 0xF000

#if (CHIP_FAMILY_TYPE == CHIP_FAMILY_NASA)
        l.ori   r3,r0,0xB000           #T3: 0xB000  T2:0xC000
                                       #T3: 0x100FEE = 0xB000    /T2: 0x0FEE = 0xC000
                                       #l.movhi r7,hi(_risc32_regbase)   #r7=0xa020_0000
#else
        l.ori   r3,r0,0xC000           # 0x0FEE = 0xC000
#endif
        l.sw    (0xEE * 2)(r6),r3
        l.ori   r5,r7,(0x1200 * 2)
        l.lhz   r3,0(r5)
        l.andi  r3,r3,0x8000
        l.sfeqi r3,0
        l.bf    jump_to_start
        l.nop

        l.ori   r3,r0,0xA100
        l.sw    (0xFE * 2)(r6),r3   # 0x0FFE = 0xA100

	hal_icache_init
        hal_dcache_init
jump_to_start:

        hal_bss_init

        ### debugging @@@
        #l.movhi r7,hi(REG_BASE+REG_BANK_DBG*2)
        #l.ori   r6,r7,lo(REG_BASE+REG_BANK_DBG*2)
        #l.ori   r5, r0, 0x7777
        #l.sw    (0x80*2)(r6),r5
        
        l.movhi r1,hi(_boot_stack-4)
        l.ori   r1,r1,lo(_boot_stack-4)
        l.addi  r2,r0,-3

        l.j     _start
        l.and   r1,r1,r2            # 4 byte alignment
#else	//AEON R2

#include "risc32_spr.h"

#define REG_BASE                0xA0000000
#if (CHIP_FAMILY_TYPE == CHIP_FAMILY_NASA)
#define REG_BANK_RISC32			0x100200
#else
#error "please assign correct register base for R2\n"
#endif
#define HAL_ICACHE_LINE_SIZE    32
#define HAL_DCACHE_LINE_SIZE    16

        .section ".reset","ax"

.macro  hal_icache_init
    /* Invalidate I-Cache */
    l.addi  r3,r0,0
    l.ori  r4,r0,8192// HAL_ICACHE_SIZE
1:
    l.ori   r5,r0,SPR_ICBIR
    l.mtspr r5,r3,0
    l.sfne  r3,r4
    l.addi  r3,r3,HAL_ICACHE_LINE_SIZE
    l.bf    1b

                /* Enable I-Cache */
    l.ori   r5,r0,SPR_SR
    l.mfspr r3,r5,0
    l.ori   r3,r3,SPR_SR_ICE
    l.mtspr r5,r3,0

    /* Flush instructions out of instruction buffer */
    l.nop
    l.nop
    l.nop
    l.nop
    l.nop
.endm

.macro  hal_dcache_init
# Invalidate DC
    l.addi  r3,r0,0
    l.addi  r4,r0,2032          # (HAL_DCACHE_SIZE/HAL_DCACHE_WAYS) - HAL_DCACHE_LINE_SIZE
1:
    l.invalidate_line   0(r3),0  # invalidate line index=r3, way=0
    l.invalidate_line   0(r3),1  # invalidate line index=r3, way=1
    l.sfne  r3,r4
    l.addi  r3,r3,HAL_DCACHE_LINE_SIZE
    l.bf    1b

# Enable D-Cache
    l.mfspr r3,r0,SPR_SR
    l.ori   r3,r3,SPR_SR_DCE
    l.mtspr r0,r3,SPR_SR
        .endm


.macro  hal_bss_init
        l.movhi r3,hi(__bss_start)
        l.ori   r3,r3,lo(__bss_start)
        l.movhi r4,hi(__bss_end)
        l.ori   r4,r4,lo(__bss_end)

1:      l.sfgeu r3,r4
        l.bf    2f
        l.sw    0(r3),r0
        l.addi  r3,r3,4
        l.j     1b
2:
.endm

        .extern _start
        .extern _boot_stack

        .org 0x100

        .globl  __reset
__reset:
        # Initialize Supervision Register:
        # Supervisor mode on, all interrupts off, caches off
        l.ori   r3,r0,SPR_SR_SM
        l.mtspr r0,r3,SPR_SR

        #   org:map SRAM to 0xc000_0000~0xc000_0fff
        #   T3:map SRAM to 0xB000_0000~0xB000_0fff
        # qmem_dmask[7:0]
        # qmem_daddr[7:0]
        l.movhi r7,hi(REG_BASE+REG_BANK_RISC32*2)
        l.ori   r6,r7,lo(REG_BASE+REG_BANK_RISC32*2)

        l.ori   r5, r0, 0xF000
        l.sw    (0xA0*2)(r6),r5

        l.ori   r5, r0, 0xB000
        l.sw    (0x9C*2)(r6),r5

        l.ori   r5, r0, 0x9000
        l.sw    (0xAA*2)(r6),r5

        l.ori   r5, r0, 0x0017
        l.sw    (0xB0*2)(r6),r5
        l.ori   r5,r7,(0x1200 * 2)
        l.lhz   r3,0(r5)
        l.andi  r3,r3,0x8000
        l.sfeqi r3,0
        l.bf    jump_to_start
        l.nop

        l.ori   r5, r0, 0xA100
        l.sw    (0xAC*2)(r6),r5
        l.ori   r5, r0, 0xA100
        l.sw    (0x90*2)(r6),r5

        hal_icache_init
        hal_dcache_init
jump_to_start:
        hal_bss_init

        l.movhi r1,hi(_boot_stack-4)
        l.ori   r1,r1,lo(_boot_stack-4)
        l.addi  r2,r0,-3

        l.j     _start
        l.and   r1,r1,r2            # 4 byte alignment
#endif
