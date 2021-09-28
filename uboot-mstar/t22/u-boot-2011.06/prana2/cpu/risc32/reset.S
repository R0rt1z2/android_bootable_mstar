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
#ifndef __AEONR2__

#include "risc32_spr.h"

#define REG_BASE                0xA0000000
#define REG_BANK_RISC32         0x100F00

#define HAL_ICACHE_LINE_SIZE    16
#define HAL_DCACHE_LINE_SIZE    16

        .section ".reset","ax"

        .org 0x0

#---------------------------------------------------------------------------
# Boot From 8051
#

#ifndef __AEONR2__ //aeon booting
#if (CHIP_FAMILY_TYPE == CHIP_FAMILY_S7 || \
     CHIP_FAMILY_TYPE == CHIP_FAMILY_U3 || \
     CHIP_FAMILY_TYPE == CHIP_FAMILY_S7LD || \
     CHIP_FAMILY_TYPE == CHIP_FAMILY_S7J || \
     CHIP_FAMILY_TYPE == CHIP_FAMILY_S7L || \
     CHIP_FAMILY_TYPE == CHIP_FAMILY_A1 || \
     CHIP_FAMILY_TYPE == CHIP_FAMILY_A7 || \
     CHIP_FAMILY_TYPE == CHIP_FAMILY_AMETHYST || \
     CHIP_FAMILY_TYPE == CHIP_FAMILY_EMERALD || \
     CHIP_FAMILY_TYPE == CHIP_FAMILY_NUGGET || \
     CHIP_FAMILY_TYPE == CHIP_FAMILY_MUNICH || \
     CHIP_FAMILY_TYPE == CHIP_FAMILY_MUSTANG || \
     CHIP_FAMILY_TYPE == CHIP_FAMILY_M5321 || \
     CHIP_FAMILY_TYPE == CHIP_FAMILY_NIKON || \
     CHIP_FAMILY_TYPE == CHIP_FAMILY_MILAN || \
     CHIP_FAMILY_TYPE == CHIP_FAMILY_MARLON)
        # P2 = 0x00 (PM CH3)
        .byte   0x75, 0xA0, 0x00    # MOV   P2, 0x00
#endif
        # clear cache reset
        .byte   0x90, 0x10, 0x18    # MOV   DPTR, #0x1018
        .byte   0x74, 0x0A          # MOV   A, #0x0A
        .byte   0xF0                # MOVX  @DPTR, A
        # enable cache
        .byte   0x90, 0x2B, 0xA0    # MOV   DPTR, #0x2BA0
        .byte   0xE4                # CLR   A
        .byte   0xF0                # MOVX  @DPTR, A
#if (CHIP_FAMILY_TYPE == CHIP_FAMILY_S7 || \
     CHIP_FAMILY_TYPE == CHIP_FAMILY_S7LD || \
     CHIP_FAMILY_TYPE == CHIP_FAMILY_S7J  || \
     CHIP_FAMILY_TYPE == CHIP_FAMILY_S7L || \
     CHIP_FAMILY_TYPE == CHIP_FAMILY_A1 || \
     CHIP_FAMILY_TYPE == CHIP_FAMILY_A7 || \
     CHIP_FAMILY_TYPE == CHIP_FAMILY_AMETHYST || \
     CHIP_FAMILY_TYPE == CHIP_FAMILY_EMERALD || \
     CHIP_FAMILY_TYPE == CHIP_FAMILY_NUGGET || \
     CHIP_FAMILY_TYPE == CHIP_FAMILY_MUNICH || \
     CHIP_FAMILY_TYPE == CHIP_FAMILY_MUSTANG || \
     CHIP_FAMILY_TYPE == CHIP_FAMILY_M5321 || \
     CHIP_FAMILY_TYPE == CHIP_FAMILY_NIKON || \
     CHIP_FAMILY_TYPE == CHIP_FAMILY_MILAN || \
     CHIP_FAMILY_TYPE == CHIP_FAMILY_MARLON)

        # P2 = 0x10 (NONPM CH4)
        .byte   0x75, 0xA0, 0x10    # MOV   P2, 0x10
        # patch test
        .byte   0x90, 0x0b, 0x42    # MOV   DPTR, #0x100b42
        .byte   0x74, 0x1c          # MOV   A, #0x1c
        .byte   0xF0                # MOVX  @DPTR, A

        .byte   0x75, 0xA0, 0x00    # MOV   P2, 0x00

        .byte   0x90, 0x0e, 0x53    # MOV   DPTR, #0x000e53
        .byte   0x74, 0x80          # MOV   A, #0x80
        .byte   0xF0                # MOVX  @DPTR, A

        .byte   0x90, 0x0e, 0x26    # MOV   DPTR, #0x000e26
        .byte   0x74, 0xBE          # MOV   A, #0xbe
        .byte   0xF0                # MOVX  @DPTR, A

        .byte   0xA3                # INC   DPTR
        .byte   0x74, 0xBA          # MOV   A, #0xba
        .byte   0xF0                # MOVX  @DPTR, A

        .byte   0x90, 0x0e, 0x53    # MOV   DPTR, #0x000e53
        .byte   0x74, 0x00          # MOV   A, #0x00
        .byte   0xF0                # MOVX  @DPTR, A

        .byte   0x74, 0x80          # MOV   A, #0x80
        .byte   0xF0                # MOVX  @DPTR, A


        .byte   0x90, 0x0e, 0x26    # MOV   DPTR, #0x000e26
        .byte   0xE4                # CLR   A
        .byte   0xF0                # MOVX  @DPTR, A
        .byte   0xA3                # INC   DPTR
        .byte   0xF0                # MOVX  @DPTR, A


        .byte   0x75, 0xA0, 0x10    # MOV   P2, 0x10
        #------------------------------------------------

        # turn on CKG_AEON
        .byte   0x90, 0x0b, 0x24    # MOV   DPTR, #0x100b24
        .byte   0x74, 0x04          # MOV   A, #0x04
        .byte   0xF0                # MOVX  @DPTR, A

   #if (CHIP_FAMILY_TYPE == CHIP_FAMILY_U3)
       .byte   0x75, 0xA0, 0x10    # MOV   P2, 0x10
   #endif
        .byte   0x90, 0x1E, 0x26    # MOV   DPTR, #0x1E26
        .byte   0xF0                # MOVX  @DPTR, A
#endif
        # AEON in order
        .byte   0x90, 0x0F, 0x80    # MOV   DPTR, #0x0F80
        .byte   0x74, 0x02          # MOV   A, #0x02
        .byte   0xF0                # MOVX  @DPTR, A     //regf80=0x02   //in order

#if (CHIP_FAMILY_TYPE == CHIP_FAMILY_S7 || \
     CHIP_FAMILY_TYPE == CHIP_FAMILY_S7LD || \
     CHIP_FAMILY_TYPE == CHIP_FAMILY_S7J || \
     CHIP_FAMILY_TYPE == CHIP_FAMILY_S7L || \
     CHIP_FAMILY_TYPE == CHIP_FAMILY_A1 || \
     CHIP_FAMILY_TYPE == CHIP_FAMILY_A7 || \
     CHIP_FAMILY_TYPE == CHIP_FAMILY_AMETHYST || \
     CHIP_FAMILY_TYPE == CHIP_FAMILY_EMERALD || \
     CHIP_FAMILY_TYPE == CHIP_FAMILY_NUGGET || \
     CHIP_FAMILY_TYPE == CHIP_FAMILY_MUNICH || \
     CHIP_FAMILY_TYPE == CHIP_FAMILY_MUSTANG || \
     CHIP_FAMILY_TYPE == CHIP_FAMILY_M5321 || \
     CHIP_FAMILY_TYPE == CHIP_FAMILY_NIKON || \
     CHIP_FAMILY_TYPE == CHIP_FAMILY_MILAN || \
     CHIP_FAMILY_TYPE == CHIP_FAMILY_MARLON)

        .byte   0x90, 0x18, 0x44    # MOV   DPTR, #0x1844
        .byte   0x74, 0x01          # MOV   A, #0x01
        .byte   0xF0                # MOVX  @DPTR, A    #sw_reset MAU

        .byte   0x90, 0x0F, 0xc8    # MOV   DPTR, #0x0Fc8
        .byte   0x74, 0x06          # MOV   A, #0x06
        .byte   0xF0                # MOVX  @DPTR, A
#endif
        # AEON SPI_BOOT

        .byte   0x90, 0x0F, 0xF0    # MOV   DPTR, #0x0FF0
        .byte   0x74, 0x40          # MOV   A, #0x40
        .byte   0xF0                # MOVX  @DPTR, A
        .byte   0x74, 0x43          # MOV   A, #0x43
        .byte   0xF0                # MOVX  @DPTR, A

#if (CHIP_FAMILY_TYPE != CHIP_FAMILY_S7 && \
     CHIP_FAMILY_TYPE != CHIP_FAMILY_S7LD && \
     CHIP_FAMILY_TYPE != CHIP_FAMILY_S7J && \
     CHIP_FAMILY_TYPE != CHIP_FAMILY_S7L && \
     CHIP_FAMILY_TYPE != CHIP_FAMILY_A1 && \
     CHIP_FAMILY_TYPE != CHIP_FAMILY_A7 && \
     CHIP_FAMILY_TYPE != CHIP_FAMILY_AMETHYST && \
     CHIP_FAMILY_TYPE != CHIP_FAMILY_EMERALD || \
     CHIP_FAMILY_TYPE == CHIP_FAMILY_NUGGET || \
     CHIP_FAMILY_TYPE == CHIP_FAMILY_MUNICH || \
     CHIP_FAMILY_TYPE == CHIP_FAMILY_MUSTANG || \
     CHIP_FAMILY_TYPE == CHIP_FAMILY_M5321 || \
     CHIP_FAMILY_TYPE == CHIP_FAMILY_NIKON || \
     CHIP_FAMILY_TYPE == CHIP_FAMILY_MILAN || \
     CHIP_FAMILY_TYPE == CHIP_FAMILY_MARLON)


   #if (CHIP_FAMILY_TYPE == CHIP_FAMILY_U3)
        # P2 = 0x00 (PM CH3)
        .byte   0x75, 0xA0, 0x00    # MOV   P2, 0x00
   #endif
        # Release Aeon from reset
        .byte   0x90, 0x10, 0x86    # MOV   DPTR, #0x1086  //reg_0x1086=0x00
        .byte   0xE4                # CLR   A              //reg_0x1086=0x01;  //reset aeon
        .byte   0xF0                # MOVX  @DPTR, A
        .byte   0x74, 0x01          # MOV   A, #0x01
        .byte   0xF0                # MOVX  @DPTR, A
#else
        .byte   0x75, 0xA0, 0x10    # MOV   P2, 0x10
        .byte   0x90, 0x0F, 0xc8    # MOV   DPTR, #0x0Fc8
        .byte   0x74, 0x02          # MOV   A, #0x02
        .byte   0xF0                # MOVX  @DPTR, A

        .byte   0x90, 0x18, 0x44    # MOV   DPTR, #0x1844
        .byte   0x74, 0x00          # MOV   A, #0x00
        .byte   0xF0                # MOVX  @DPTR, A
        .byte   0x75, 0xA0, 0x00    # MOV   P2, 0x00
#endif
#else  //aeonR2 booting
        # Boot From 8051
        # P2 = 0x00 (PM CH3)
        .byte   0x75, 0xA0, 0x00    # MOV   P2, 0x00
        # clear cache reset
        .byte   0x90, 0x10, 0x18    # MOV   DPTR, #0x1018
        .byte   0x74, 0x0A          # MOV   A, #0x0A
        .byte   0xF0                # MOVX  @DPTR, A
        # enable cache
        .byte   0x90, 0x2B, 0xA0    # MOV   DPTR, #0x2BA0
        .byte   0xE4                # CLR   A
        .byte   0xF0                # MOVX  @DPTR, A

        # Release Beon from reset, need to do before 0x100280
        .byte   0x90, 0x2E, 0x53    # MOV   DPTR, #0x2E53
        .byte   0x74, 0x40          # MOV   A, #0x00
        .byte   0xF0                # MOVX  @DPTR, A

        # P2=0x10   >new setting in T3
        .byte   0x75,0xA0,0x10      #mov P2,0x10

        .byte   0x90, 0x02, 0x80    # MOV   DPTR, #0x100280
        .byte   0x74, 0x10          # MOV   A, #0x10 set spi boot
        .byte   0xF0                # MOVX  @DPTR, A

        .byte   0x74, 0x16          # MOV   A, #0x14 release miu domain reset
        .byte   0xF0                # MOVX  @DPTR, A

        .byte   0x74, 0x17          # MOV   A, #0x17
        .byte   0xF0                # MOVX  @DPTR, A
        .byte   0x75,0xA0,0x00      #mov P2,0x10

#endif
        # Set Xdata window for MSTV_Tool
        .byte   0x90, 0x2B, 0xC6    # MOV   DPTR, #0x2BC6
#if ((CHIP_FAMILY_TYPE == CHIP_FAMILY_S7 ) || \
     (CHIP_FAMILY_TYPE == CHIP_FAMILY_S7LD ) || \
     (CHIP_FAMILY_TYPE == CHIP_FAMILY_S7J) || \
     (CHIP_FAMILY_TYPE == CHIP_FAMILY_U3) || \
     (CHIP_FAMILY_TYPE == CHIP_FAMILY_S7L) || \
     (CHIP_FAMILY_TYPE == CHIP_FAMILY_A1) || \
     (CHIP_FAMILY_TYPE == CHIP_FAMILY_A7) || \
     (CHIP_FAMILY_TYPE == CHIP_FAMILY_AMETHYST) || \
     (CHIP_FAMILY_TYPE == CHIP_FAMILY_EMERALD) || \
     (CHIP_FAMILY_TYPE == CHIP_FAMILY_NUGGET) || \
     (CHIP_FAMILY_TYPE == CHIP_FAMILY_MUNICH) || \
     (CHIP_FAMILY_TYPE == CHIP_FAMILY_MUSTANG) || \
     (CHIP_FAMILY_TYPE == CHIP_FAMILY_M5321) || \
     (CHIP_FAMILY_TYPE == CHIP_FAMILY_NIKON) || \
     (CHIP_FAMILY_TYPE == CHIP_FAMILY_MILAN) || \
     (CHIP_FAMILY_TYPE == CHIP_FAMILY_MARLON))

        .byte   0x74, 0x11          # MOV   A, #0x11
#else
        .byte   0x74, 0x12          # MOV   A, #0x12
#endif
        .byte   0xF0                # MOVX  @DPTR, A
        .byte   0xA3                # INC   DPTR
        .byte   0x74, 0x3C          # MOV   A, #0x3C
        .byte   0xF0                # MOVX  @DPTR, A
        .byte   0xA3                # INC   DPTR
        .byte   0xE4                # CLR   A
        .byte   0xF0                # MOVX  @DPTR, A
        .byte   0xA3                # INC   DPTR
        .byte   0xF0                # MOVX  @DPTR, A
        .byte   0xA3                # INC   DPTR
        .byte   0x74, 0x3C          # MOV   A, #0x3C
        .byte   0xF0                # MOVX  @DPTR, A
        .byte   0xA3                # INC   DPTR
        .byte   0x74, 0x40          # MOV   A, #0x40
        .byte   0xF0                # MOVX  @DPTR, A
        .byte   0x90, 0x2B, 0xC4    # MOV   DPTR, #0x2BC4
        .byte   0x74, 0x04          # MOV   A, #0x04
        .byte   0xF0                # MOVX  @DPTR, A
        # forever loop
#ifdef __AEONR2__
        .byte   0x75, 0xA0, 0x10    #mov P2,0x10
        .byte   0x90, 0x02, 0xB4    # MOV   DPTR, #0x100280
        .byte   0xE0
        .byte   0x60, 0xFA

        .byte   0x90, 0x02, 0x80    # MOV   DPTR, #0x100280
        .byte   0x74, 0x00          # MOV   A, #0x24 set dram boot
        .byte   0xF0                # MOVX  @DPTR, A
        .byte   0x74, 0x24          # MOV   A, #0x24 set dram boot
        .byte   0xF0                # MOVX  @DPTR, A
        .byte   0x74, 0x27          # MOV   A, #0x27 set dram boot
        .byte   0xF0                # MOVX  @DPTR, A
#endif
        .byte   0x80, 0xFE          # SJMP  $

#ifdef __AEONR2__
.macro  hal_icache_init
        /* Disable I-Cache */
        l.ori   r4,r0,SPR_SR
        l.mfspr r3,r4,0
        l.addi  r4,r0,-1
        l.xori  r4,r4,SPR_SR_ICE
        l.and   r3,r3,r4
        l.ori   r4,r0,SPR_SR
        l.mtspr r4,r3,0

        /* Invalidate I-Cache */
        l.addi  r3,r0,0
        l.addi  r4,r0,16384
1:
        l.ori   r5,r0,SPR_ICBIR
        l.mtspr r5,r3,0
        l.sfleu r3,r4
        l.addi  r3,r3,HAL_ICACHE_LINE_SIZE
        l.bf    1b

        /* Enable I-Cache */
        // There is no "un-cached instruction area" in OpenRISC, therefore turn it off
        l.ori   r4,r0,SPR_SR
        l.mfspr r3,r4,0
        l.ori   r3,r3,SPR_SR_ICE
        l.ori   r4,r0,SPR_SR
        l.mtspr r4,r3,SPR_SR
        l.nop
        l.nop
        l.nop
        l.nop
        l.nop
.endm

.macro  hal_dcache_init
        /* Disable D-Cache */
        l.ori   r4,r0,SPR_SR
        l.mfspr r3,r4,0
        l.addi  r4,r0,-1
        l.xori  r4,r4,SPR_SR_DCE
        l.and   r3,r3,r4
        l.ori   r4,r0,SPR_SR
        l.mtspr r4,r3,0

        /* Invalidate D-Cache */
        l.addi  r3,r0,0
        l.ori  r4,r0,32768
1:
        l.invalidate_line   0(r3),0
        l.invalidate_line   0(r3),1
        l.invalidate_line   0(r3),2
        l.invalidate_line   0(r3),3
        l.addi  r3,r3,HAL_DCACHE_LINE_SIZE
        l.sfleu r3,r4
        l.bf    1b

        /* Enable DC */
        l.ori   r4,r0,SPR_SR
        l.mfspr r3,r4,0
        l.ori   r3,r3,SPR_SR_DCE
        l.ori   r4,r0,SPR_SR
        l.mtspr r4,r3,0
.endm

#else
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
        l.addi  r4,r0,4080           # (HAL_DCACHE_SIZE/HAL_DCACHE_WAYS) - HAL_DCACHE_LINE_SIZE
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
#endif

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

#ifndef __AEONR2__
        l.ori   r3,r0,0xF000
        l.sw    (0xEA * 2)(r6),r3   # 0x0FEA = 0xF000

#if (CHIP_FAMILY_TYPE == CHIP_FAMILY_S7 || \
     CHIP_FAMILY_TYPE == CHIP_FAMILY_S7LD || \
     CHIP_FAMILY_TYPE == CHIP_FAMILY_S7J || \
     CHIP_FAMILY_TYPE == CHIP_FAMILY_U3 || \
     CHIP_FAMILY_TYPE == CHIP_FAMILY_S7L || \
     CHIP_FAMILY_TYPE == CHIP_FAMILY_A1 || \
     CHIP_FAMILY_TYPE == CHIP_FAMILY_A7 || \
     CHIP_FAMILY_TYPE == CHIP_FAMILY_AMETHYST || \
     CHIP_FAMILY_TYPE == CHIP_FAMILY_EMERALD || \
     CHIP_FAMILY_TYPE == CHIP_FAMILY_NUGGET || \
     CHIP_FAMILY_TYPE == CHIP_FAMILY_MUNICH || \
     CHIP_FAMILY_TYPE == CHIP_FAMILY_MUSTANG || \
     CHIP_FAMILY_TYPE == CHIP_FAMILY_M5321 || \
     CHIP_FAMILY_TYPE == CHIP_FAMILY_NIKON || \
     CHIP_FAMILY_TYPE == CHIP_FAMILY_MILAN || \
     CHIP_FAMILY_TYPE == CHIP_FAMILY_MARLON)

        l.ori   r3,r0,0xB000           #T3: 0xB000  T2:0xC000
                                       #T3: 0x100FEE = 0xB000    /T2: 0x0FEE = 0xC000
                                       #l.movhi r7,hi(_risc32_regbase)   #r7=0xa020_0000
#else
        l.ori   r3,r0,0xC000           # 0x0FEE = 0xC000
#endif
        l.sw    (0xEE * 2)(r6),r3
#else
        l.ori   r5, r0, 0xF000
        l.sw    (0xA0*2)(r6),r5

        l.ori   r5, r0, 0xB000
        l.sw    (0x9C*2)(r6),r5

//        l.ori   r5, r0, 0x9000
//        l.sw    (0xAA*2)(r6),r5

        l.ori   r5, r0, 0x0017
        l.sw    (0xB0*2)(r6),r5
#endif
        l.ori   r5,r7,(0x1200 * 2)
        l.lhz   r3,0(r5)
        l.andi  r3,r3,0x8000
        l.sfeqi r3,0
        l.bf    jump_to_start
        l.nop

#ifndef __AEONR2__
        l.ori   r3,r0,0xA100
        l.sw    (0xFE * 2)(r6),r3   # 0x0FFE = 0xA100
#else
        l.ori   r5, r0, 0xA100
        l.sw    (0xAC*2)(r6),r5
        l.ori   r5, r0, 0xA100
        l.sw    (0x90*2)(r6),r5
#endif
        hal_icache_init
        hal_dcache_init
jump_to_start:

        hal_bss_init

        l.movhi r1,hi(_boot_stack-4)
        l.ori   r1,r1,lo(_boot_stack-4)
        l.addi  r2,r0,-3

        l.j     _start
        l.and   r1,r1,r2            # 4 byte alignment
#else

#include "risc32_spr.h"

#if (FRC_ENABLED==1)
#define REG_BASE                0xFA000000
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_MACAN) || (CHIP_FAMILY_TYPE == CHIP_FAMILY_M3822)
#define REG_BASE                0xFA000000
#else
#define REG_BASE                0xA0000000
#endif
#ifdef __AEONR2__   //R2 boot up register base

#if (CHIP_FAMILY_TYPE == CHIP_FAMILY_S7J)
#define REG_BANK_RISC32         0x100200
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_A5) || \
      (CHIP_FAMILY_TYPE == CHIP_FAMILY_A5P) || \
      (CHIP_FAMILY_TYPE == CHIP_FAMILY_A3)
#define REG_BANK_RISC32         0x100F00
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_S8)
#define REG_BANK_RISC32         0x100F00
#elif ((CHIP_FAMILY_TYPE == CHIP_FAMILY_AGATE) || \
      (CHIP_FAMILY_TYPE == CHIP_FAMILY_EAGLE) || \
      (CHIP_FAMILY_TYPE == CHIP_FAMILY_EDISON) || \
      (CHIP_FAMILY_TYPE == CHIP_FAMILY_EIFFEL) || \
      (CHIP_FAMILY_TYPE == CHIP_FAMILY_NIKE) || \
      (CHIP_FAMILY_TYPE == CHIP_FAMILY_MADISON) || \
      (CHIP_FAMILY_TYPE == CHIP_FAMILY_MONET) || \
      (CHIP_FAMILY_TYPE == CHIP_FAMILY_MOONEY) || \
      (CHIP_FAMILY_TYPE == CHIP_FAMILY_MUJI) || \
      (CHIP_FAMILY_TYPE == CHIP_FAMILY_MESSI) || \
      (CHIP_FAMILY_TYPE == CHIP_FAMILY_MACAN) || \
      (CHIP_FAMILY_TYPE == CHIP_FAMILY_M3822) || \
      (CHIP_FAMILY_TYPE == CHIP_FAMILY_CELTICS) || \
      (CHIP_FAMILY_TYPE == CHIP_FAMILY_CLIPPERS) || \
      (CHIP_FAMILY_TYPE == CHIP_FAMILY_KANO) || \
      (CHIP_FAMILY_TYPE == CHIP_FAMILY_K6) || \
      (CHIP_FAMILY_TYPE == CHIP_FAMILY_K6LITE) || \
      (CHIP_FAMILY_TYPE == CHIP_FAMILY_K7U) || \
      (CHIP_FAMILY_TYPE == CHIP_FAMILY_MIAMI) || \
      (CHIP_FAMILY_TYPE == CHIP_FAMILY_NADAL) || \
      (CHIP_FAMILY_TYPE == CHIP_FAMILY_EINSTEIN) || \
      (CHIP_FAMILY_TYPE == CHIP_FAMILY_MONACO) || \
      (CHIP_FAMILY_TYPE == CHIP_FAMILY_NUGGET) || \
      (CHIP_FAMILY_TYPE == CHIP_FAMILY_MUNICH) || \
      (CHIP_FAMILY_TYPE == CHIP_FAMILY_MUSTANG) || \
      (CHIP_FAMILY_TYPE == CHIP_FAMILY_C2P) || \
      (CHIP_FAMILY_TYPE == CHIP_FAMILY_CURRY) || \
      (CHIP_FAMILY_TYPE == CHIP_FAMILY_C2S) || \
      (CHIP_FAMILY_TYPE == CHIP_FAMILY_M5321) || \
      (CHIP_FAMILY_TYPE == CHIP_FAMILY_MARVEL))
#define REG_BANK_RISC32         0x122A00
#elif ((CHIP_FAMILY_TYPE == CHIP_FAMILY_MANHATTAN)|| \
       (CHIP_FAMILY_TYPE == CHIP_FAMILY_MASERATI) || \
       (CHIP_FAMILY_TYPE == CHIP_FAMILY_MCLAREN) || \
       (CHIP_FAMILY_TYPE == CHIP_FAMILY_MYSTIC)   || \
       (CHIP_FAMILY_TYPE == CHIP_FAMILY_MAXIM) || \
       (CHIP_FAMILY_TYPE == CHIP_FAMILY_MARCUS) || \
       (CHIP_FAMILY_TYPE == CHIP_FAMILY_M7221) || \
       (CHIP_FAMILY_TYPE == CHIP_FAMILY_M7622) || \
       (CHIP_FAMILY_TYPE == CHIP_FAMILY_M7322) || \
       (CHIP_FAMILY_TYPE == CHIP_FAMILY_M7632) || \
       (CHIP_FAMILY_TYPE == CHIP_FAMILY_M7332))
  #if (FRC_ENABLED==1)
    #define REG_BANK_RISC32         0x400700 //FRC-R2
  #else
    #define REG_BANK_RISC32         0x122A00 //SEC-R2
  #endif
#else
#error "please assign correct register base for R2\n"
#endif

#else
#define REG_BANK_RISC32         0x100F00
#endif

#define HAL_ICACHE_LINE_SIZE    16
#define HAL_DCACHE_LINE_SIZE    16

        .section ".reset","ax"

        .org 0x0

#---------------------------------------------------------------------------
# Boot From 8051
#
        # P2 = 0x00 (PM CH3)
        .byte   0x75, 0xA0, 0x00    # MOV   P2, 0x00
        # clear cache reset
        .byte   0x90, 0x10, 0x18    # MOV   DPTR, #0x1018
        .byte   0x74, 0x0A          # MOV   A, #0x0A
        .byte   0xF0                # MOVX  @DPTR, A
        # enable cache
        .byte   0x90, 0x2B, 0xA0    # MOV   DPTR, #0x2BA0
        .byte   0xE4                # CLR   A
        .byte   0xF0                # MOVX  @DPTR, A

#ifndef __AEONR2__
        # P2=0x10   >new setting in Janus
        .byte   0x75,0xA0,0x10        #mov P2,0x10

        # AEON in order
        .byte   0x90, 0x0F, 0x80    # MOV   DPTR, #0x0F80
        .byte   0x74, 0x02          # MOV   A, #0x02
        .byte   0xF0                # MOVX  @DPTR, A
        # AEON SPI_BOOT
        .byte   0x90, 0x0F, 0xF0    # MOV   DPTR, #0x0FF0
        .byte   0xE0                # MOVX  A, @DPTR
        .byte   0x44, 0x03          # ORL   A, #0x03
        .byte   0x64, 0x40          # XRL   A, #0x40
        .byte   0xF0                # MOVX  @DPTR, A


        # P2=0x00   >new setting in Janusx
        .byte   0x75,0xA0,0x00        #mov P2,0x00

        # Release Aeon from reset
        .byte   0x90, 0x10, 0x86    # MOV   DPTR, #0x1086
        .byte   0xE4                # CLR   A
        .byte   0xF0                # MOVX  @DPTR, A
        .byte   0x74, 0x01          # MOV   A, #0x01
        .byte   0xF0                # MOVX  @DPTR, A
#else
        # Release Beon from reset, need to do before 0x100280
        .byte   0x90, 0x2E, 0x53    # MOV   DPTR, #0x2E53
        .byte   0x74, 0x40          # MOV   A, #0x00
        .byte   0xF0                # MOVX  @DPTR, A

        # P2=0x10   >new setting in T3
        .byte   0x75,0xA0,0x10      #mov P2,0x10

        .byte   0x90, 0x02, 0x80    # MOV   DPTR, #0x100280
        .byte   0x74, 0x10          # MOV   A, #0x10 set spi boot
        .byte   0xF0                # MOVX  @DPTR, A

        .byte   0x74, 0x16          # MOV   A, #0x14 release miu domain reset
        .byte   0xF0                # MOVX  @DPTR, A

        .byte   0x74, 0x17          # MOV   A, #0x17
        .byte   0xF0                # MOVX  @DPTR, A
        .byte   0x75,0xA0,0x00      #mov P2,0x10
#endif
        # Set Xdata window for MSTV_Tool
        .byte   0x90, 0x2B, 0xC6    # MOV   DPTR, #0x2BC6
        .byte   0x74, 0x11          # MOV   A, #0x11
        .byte   0xF0                # MOVX  @DPTR, A
        .byte   0xA3                # INC   DPTR
        .byte   0x74, 0x3C          # MOV   A, #0x3C
        .byte   0xF0                # MOVX  @DPTR, A
        .byte   0xA3                # INC   DPTR
        .byte   0xE4                # CLR   A
        .byte   0xF0                # MOVX  @DPTR, A
        .byte   0xA3                # INC   DPTR
        .byte   0xF0                # MOVX  @DPTR, A
        .byte   0xA3                # INC   DPTR
        .byte   0x74, 0x3C          # MOV   A, #0x3C
        .byte   0xF0                # MOVX  @DPTR, A
        .byte   0xA3                # INC   DPTR
        .byte   0x74, 0x40          # MOV   A, #0x40
        .byte   0xF0                # MOVX  @DPTR, A
        .byte   0x90, 0x2B, 0xC4    # MOV   DPTR, #0x2BC4
        .byte   0x74, 0x04          # MOV   A, #0x04
        .byte   0xF0                # MOVX  @DPTR, A
        # forever loop
#ifdef __AEONR2__
        .byte   0x75, 0xA0, 0x10    #mov P2,0x10
        .byte   0x90, 0x02, 0xB4    # MOV   DPTR, #0x100280
        .byte   0xE0
        .byte   0x60, 0xFA

        .byte   0x90, 0x02, 0x80    # MOV   DPTR, #0x100280
        .byte   0x74, 0x00          # MOV   A, #0x24 set dram boot
        .byte   0xF0                # MOVX  @DPTR, A
        .byte   0x74, 0x24          # MOV   A, #0x24 set dram boot
        .byte   0xF0                # MOVX  @DPTR, A
        .byte   0x74, 0x27          # MOV   A, #0x27 set dram boot
        .byte   0xF0                # MOVX  @DPTR, A
#endif
        .byte   0x80, 0xFE          # SJMP  $

.macro  hal_icache_init
    /* Invalidate I-Cache */
    l.addi  r3,r0,0
#ifdef __AEONR2__
    #if ((CHIP_FAMILY_TYPE == CHIP_FAMILY_MANHATTAN) && (FRC_ENABLED==1))
    l.ori  r4,r0,8192// HAL_ICACHE_SIZE
    #else
    l.ori  r4,r0,16384// HAL_ICACHE_SIZE
    #endif
#else
    l.addi  r4,r0,8192// HAL_ICACHE_SIZE
#endif
1:
#ifdef __AEONR2__
    l.ori   r5,r0,SPR_ICBIR
    l.mtspr r5,r3,0
#else
    l.mtspr r0,r3,SPR_ICBIR
#endif
    l.sfne  r3,r4
#ifdef __AEONR2__
    l.addi  r3,r3,HAL_ICACHE_LINE_SIZE
    l.bf    1b
#else
    l.bf    1b
    l.addi  r3,r3,HAL_ICACHE_LINE_SIZE
#endif

                /* Enable I-Cache */
#ifdef __AEONR2__
    l.ori   r5,r0,SPR_SR
    l.mfspr r3,r5,0
    l.ori   r3,r3,SPR_SR_ICE
    l.mtspr r5,r3,0
#else
    l.mfspr r3,r0,SPR_SR
    l.ori   r3,r3,SPR_SR_ICE
    l.mtspr r0,r3,SPR_SR
#endif

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
#ifdef __AEONR2__
    l.addi  r4,r0,8176          # (HAL_DCACHE_SIZE/HAL_DCACHE_WAYS) - HAL_DCACHE_LINE_SIZE
#else
    l.addi  r4,r0,4080          # (HAL_DCACHE_SIZE/HAL_DCACHE_WAYS) - HAL_DCACHE_LINE_SIZE
#endif
1:
    l.invalidate_line   0(r3),0  # invalidate line index=r3, way=0
    l.invalidate_line   0(r3),1  # invalidate line index=r3, way=1
#ifdef __AEONR2__
    l.invalidate_line   0(r3),2  # invalidate line index=r3, way=1
    l.invalidate_line   0(r3),3  # invalidate line index=r3, way=1
#endif
    l.sfne  r3,r4
#ifdef __AEONR2__
    l.addi  r3,r3,HAL_DCACHE_LINE_SIZE
    l.bf    1b
#else
    l.bf    1b
    l.addi  r3,r3,HAL_DCACHE_LINE_SIZE
#endif

# Enable D-Cache
#ifdef __AEONR2__
    l.mfspr r3,r0,SPR_SR
    l.ori   r3,r3,SPR_SR_DCE
    l.mtspr r0,r3,SPR_SR
#else
    l.mfspr r3,r0,SPR_SR
    l.ori   r3,r3,SPR_SR_DCE
    l.mtspr r0,r3,SPR_SR
#endif
        .endm


.macro  hal_bss_init
        l.movhi r3,hi(__bss_start)
        l.ori   r3,r3,lo(__bss_start)
        l.movhi r4,hi(__bss_end)
        l.ori   r4,r4,lo(__bss_end)

1:      l.sfgeu r3,r4
        l.bf    2f
#ifndef __AEONR2__
        l.nop
#endif
        l.sw    0(r3),r0
#ifdef __AEONR2__
        l.addi  r3,r3,4
#endif
        l.j     1b
#ifndef __AEONR2__
        l.addi  r3,r3,4
#endif
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
#ifndef __AEONR2__
        l.ori   r3,r0,0xF000
        l.sw    (0xEA * 2)(r6),r3   # 0x0FEA = 0xF000

        l.ori   r3,r0,0xB000        #T3: 0xB000  T2:0xC000
        l.sw    (0xEE * 2)(r6),r3
#else
    #if ((CHIP_FAMILY_TYPE != CHIP_FAMILY_MACAN)) && ((CHIP_FAMILY_TYPE != CHIP_FAMILY_M3822))
        l.ori   r5, r0, 0xF000
        l.sw    (0xA0*2)(r6),r5

        l.ori   r5, r0, 0xB000
        l.sw    (0x9C*2)(r6),r5
    #endif

        //l.ori   r5, r0, 0x9000
        //l.sw    (0xAA*2)(r6),r5

        l.ori   r5, r0, 0x0017
        l.sw    (0xB0*2)(r6),r5
#endif

#if (CHIP_FAMILY_TYPE == CHIP_FAMILY_MANHATTAN) && (FRC_ENABLED==1)
#else
        l.movhi r7,hi(REG_BASE+0x101200*2)
        l.ori   r5,r7,lo(REG_BASE+0x101200*2)
        l.lhz   r3,0(r5)
        l.andi  r3,r3,0x8000
        l.sfeqi r3,0
        l.bf    jump_to_start
        l.nop
#endif

#ifndef __AEONR2__
        l.ori   r3,r0,0xA100
        l.sw    (0xFE * 2)(r6),r3   # 0x0FFE = 0xA100
#else
    #if ((CHIP_FAMILY_TYPE != CHIP_FAMILY_MACAN)) && ((CHIP_FAMILY_TYPE != CHIP_FAMILY_M3822))
        l.ori   r5, r0, 0xA100
        l.sw    (0xAC*2)(r6),r5
        l.ori   r5, r0, 0xA100
        l.sw    (0x90*2)(r6),r5
    #endif
#endif
        hal_icache_init
        #if ((CHIP_FAMILY_TYPE != CHIP_FAMILY_AGATE) && (CHIP_FAMILY_TYPE != CHIP_FAMILY_EDISON))
        hal_dcache_init
        #endif

jump_to_start:
        hal_bss_init

        l.movhi r1,hi(_boot_stack-4)
        l.ori   r1,r1,lo(_boot_stack-4)
        l.addi  r2,r0,-3

        l.j     _start
        l.and   r1,r1,r2            # 4 byte alignment
#endif
