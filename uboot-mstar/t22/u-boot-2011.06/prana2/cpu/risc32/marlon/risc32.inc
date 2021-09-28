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

## Register Aliases for ABI

#define sp r1
#define lr r9
#define fp r10

#define p0 r3
#define p1 r4
#define p2 r5
#define p3 r6
#define p4 r7
#define p5 r8

#define __TEXT_ALIGN      .align 4

#define ENTRY(name)                 \
        __TEXT_ALIGN;               \
        .globl _##name;             \
        .type _##name,@function;    \
_##name:

# Size of GPR regs - 4 bytes for risc32
#define RISC32_GPRSIZE    4

# Size of all other registers
#define RISC32_REGSIZE    4

# Utility macro: Load a 32-bit constant into a register
.macro  load32i reg const
    l.movhi \reg,hi(\const)
    l.ori   \reg,\reg,lo(\const)
.endm

#define RISC32REGS_GPRS   0
#ifdef __AEONR2__
#define RISC32REG_MACHI2  (RISC32REGS_GPRS + RISC32_GPRSIZE * 32)
#define RISC32REG_MACHI   (RISC32REG_MACHI2 + RISC32_GPRSIZE)
#else
#define RISC32REG_MACHI   (RISC32REGS_GPRS + RISC32_GPRSIZE * 32)
#endif
#define RISC32REG_MACLO   (RISC32REG_MACHI + RISC32_REGSIZE)
#define RISC32REG_VECTOR  (RISC32REG_MACLO + RISC32_REGSIZE)
#define RISC32REG_SR      (RISC32REG_VECTOR+ RISC32_REGSIZE)
#define RISC32REG_PC      (RISC32REG_SR    + RISC32_REGSIZE)
#define RISC32REG_EEAR    (RISC32REG_PC    + RISC32_REGSIZE)

#define LEAF_FUNC_SIZE 128
#define SIZEOF_RISC32REGS (RISC32REG_EEAR  + RISC32_REGSIZE + LEAF_FUNC_SIZE)
