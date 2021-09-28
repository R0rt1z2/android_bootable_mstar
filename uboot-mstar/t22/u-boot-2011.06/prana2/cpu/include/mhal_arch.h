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

#ifndef __MHAL_ARCH_H__
#define __MHAL_ARCH_H__

///
/// @brief HAL for risc32 architecture
/// @par Example: register handler for trap exception
/// @code
/// void trap_isr(MHAL_SavedRegisters *regs, U32 data);
/// mhal_exception_attach(E_EXCEPTION_TRAP, trap_isr, 0 /* data pass to isr */);
/// @endcode
///
/// @par Example: ciritical section for interrupt
/// @code
/// // method 1 (suggested)
/// mhal_interrupt_lock();
/// // ... critical section ...
/// mhal_interrupt_unlock();
///
/// // method 2
/// int old = mhal_interrupt_disable();
/// // ... critical section ...
/// mhal_interrupt_restore(old);
/// @endcode

#ifdef LIBPRANA
#include "risc32_spr.h"
#include "risc32_util.h"
#endif

#define RegReadByte(_base_,_offset_) ((volatile unsigned char*)(_base_))[(_offset_ << 1) - (_offset_ & 1)]


#if defined (__arm__) || defined (__aarch64__)

#ifdef CHIP_FAMILY_TYPE
#include "chiptype.h"
#define NONCACHABLE(_x_)    ((_x_) | 0x00000000)
#endif

#if defined(MBOOT) || defined(CHAKRA)
    #define RIUBASE   0x1F000000
    #define PERIBASE  0x16000000
#else
    #define RIUBASE   0xFD000000
    #define PERIBASE  0xFC000000
#endif

#if (CHIP_FAMILY_TYPE == CHIP_FAMILY_A3)
    #define MIPS_CLOCK_FREQ     720000000
    #define MIU0_BUS_BASE       0x40000000
    #define MIU1_BUS_BASE       0xA0000000
    #define MIU0_MAP_LEN        0x20000000
    #define MIU1_MAP_LEN        0x20000000
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_AGATE)
    #define MIPS_CLOCK_FREQ     720000000
    #define MIU0_BUS_BASE       0x40000000
    #define MIU1_BUS_BASE       0xA0000000
    #define MIU0_MAP_LEN        0x20000000
    #define MIU1_MAP_LEN        0x20000000
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_EAGLE)
    #define MIPS_CLOCK_FREQ     900000000
    #define MIU0_BUS_BASE       0x40000000
    #define MIU1_BUS_BASE       0xA0000000
    #define MIU0_MAP_LEN        0x20000000
    #define MIU1_MAP_LEN        0x20000000
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_EDISON)
    #define MIPS_CLOCK_FREQ     900000000
    #define MIU0_BUS_BASE       0x40000000
    #define MIU1_BUS_BASE       0xA0000000
    #define MIU0_MAP_LEN        0x20000000
    #define MIU1_MAP_LEN        0x20000000
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_KAISER)
    #define MIPS_CLOCK_FREQ     900000000
    #define MIU0_BUS_BASE       0x20000000
    #define MIU1_BUS_BASE       0xA0000000
    #define MIU0_MAP_LEN        0x20000000
    #define MIU1_MAP_LEN        0x20000000
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_NAPOLI)
    #define MIPS_CLOCK_FREQ     1008000000
    #define MIU0_BUS_BASE       0x20000000
    #define MIU1_BUS_BASE       0xA0000000
    #define MIU0_MAP_LEN        0x40000000
    #define MIU1_MAP_LEN        0x40000000
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_EINSTEIN)
    #define MIPS_CLOCK_FREQ     1008000000
    #define MIU0_BUS_BASE       0x20000000
    #define MIU1_BUS_BASE       0xA0000000
    #define MIU0_MAP_LEN        0x20000000
    #define MIU1_MAP_LEN        0x20000000
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_MUJI)
    #define MIPS_CLOCK_FREQ     1008000000
    #define MIU0_BUS_BASE       0x20000000
    #define MIU1_BUS_BASE       0xA0000000
    #define MIU0_MAP_LEN        0x40000000
    #define MIU1_MAP_LEN        0x40000000
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_MANHATTAN)
    #define MIPS_CLOCK_FREQ     1008000000
    #define MIU0_BUS_BASE       0x20000000
    #define MIU1_BUS_BASE       0xC0000000
    #define MIU0_MAP_LEN        0x40000000
    #define MIU1_MAP_LEN        0x20000000
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_MASERATI)
    #define MIPS_CLOCK_FREQ     1008000000
    #define MIU0_BUS_BASE       0x20000000
    #define MIU1_BUS_BASE       0xA0000000
    #define MIU0_MAP_LEN        0x40000000
    #define MIU1_MAP_LEN        0x20000000
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_MCLAREN)
    #define MIPS_CLOCK_FREQ     1008000000
    #define MIU0_BUS_BASE       0x20000000
    #define MIU1_BUS_BASE       0xA0000000
    #define MIU0_MAP_LEN        0x40000000
    #define MIU1_MAP_LEN        0x20000000
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_MYSTIC)
    #define MIPS_CLOCK_FREQ     1008000000
    #define MIU0_BUS_BASE       0x20000000
    #define MIU1_BUS_BASE       0xA0000000
    #define MIU0_MAP_LEN        0x40000000
    #define MIU1_MAP_LEN        0x20000000
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_MAXIM)
    #define MIPS_CLOCK_FREQ     1008000000
    #define MIU0_BUS_BASE       0x20000000
    #define MIU1_BUS_BASE       0xA0000000
    #define MIU0_MAP_LEN        0x40000000
    #define MIU1_MAP_LEN        0x20000000
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_MACAN)
    #define MIPS_CLOCK_FREQ     1008000000
    #define MIU0_BUS_BASE       0x20000000
    #define MIU1_BUS_BASE       0xA0000000
    #define MIU0_MAP_LEN        0x40000000
    #define MIU1_MAP_LEN        0x40000000
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_M3822)
    #define MIPS_CLOCK_FREQ     1008000000
    #define MIU0_BUS_BASE       0x20000000
    #define MIU1_BUS_BASE       0xA0000000
    #define MIU0_MAP_LEN        0x20000000
    #define MIU1_MAP_LEN        0x40000000
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_EIFFEL)
    #define MIPS_CLOCK_FREQ     900000000
    #define MIU0_BUS_BASE       0x20000000
    #define MIU1_BUS_BASE       0xA0000000
    #define MIU0_MAP_LEN        0x20000000
    #define MIU1_MAP_LEN        0x20000000
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_NIKE)
    #define MIPS_CLOCK_FREQ     900000000
    #define MIU0_BUS_BASE       0x20000000
    #define MIU1_BUS_BASE       0xA0000000
    #define MIU0_MAP_LEN        0x20000000
    #define MIU1_MAP_LEN        0x20000000
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_MADISON)
    #define MIPS_CLOCK_FREQ     900000000
    #define MIU0_BUS_BASE       0x20000000
    #define MIU1_BUS_BASE       0xA0000000
    #define MIU0_MAP_LEN        0x20000000
    #define MIU1_MAP_LEN        0x20000000
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_MONET)
    #define MIPS_CLOCK_FREQ     1000000000
    #define MIU0_BUS_BASE       0x20000000
    #define MIU1_BUS_BASE       0xA0000000
    #define MIU0_MAP_LEN        0x40000000
    #define MIU1_MAP_LEN        0x40000000
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_MOONEY)
    #define MIPS_CLOCK_FREQ     1000000000
    #define MIU0_BUS_BASE       0x20000000
    #define MIU1_BUS_BASE       0xA0000000
    #define MIU0_MAP_LEN        0x40000000
    #define MIU1_MAP_LEN        0x40000000
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_MESSI)
    #define MIPS_CLOCK_FREQ     1000000000
    #define MIU0_BUS_BASE       0x20000000
    #define MIU1_BUS_BASE       0xA0000000
    #define MIU0_MAP_LEN        0x20000000
    #define MIU1_MAP_LEN        0x20000000
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_CELTICS)
    #define MIPS_CLOCK_FREQ     900000000
    #define MIU0_BUS_BASE       0x20000000
    #define MIU1_BUS_BASE       0xA0000000
    #define MIU0_MAP_LEN        0x20000000
    #define MIU1_MAP_LEN        0x20000000
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_CLIPPERS)
    #define MIPS_CLOCK_FREQ     900000000
    #define MIU0_BUS_BASE       0x20000000
    #define MIU1_BUS_BASE       0xA0000000
    #define MIU0_MAP_LEN        0x40000000
    #define MIU1_MAP_LEN        0x40000000
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_CURRY)
    #define MIPS_CLOCK_FREQ     900000000
    #define MIU0_BUS_BASE       0x20000000
    #define MIU1_BUS_BASE       0xA0000000
    #define MIU0_MAP_LEN        0x20000000
    #define MIU1_MAP_LEN        0x20000000
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_C2P)
    #define MIPS_CLOCK_FREQ     900000000
    #define MIU0_BUS_BASE       0x20000000
    #define MIU1_BUS_BASE       0xA0000000
    #define MIU0_MAP_LEN        0x20000000
    #define MIU1_MAP_LEN        0x20000000
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_C2S)
    #define MIPS_CLOCK_FREQ     900000000
    #define MIU0_BUS_BASE       0x20000000
    #define MIU1_BUS_BASE       0xA0000000
    #define MIU0_MAP_LEN        0x20000000
    #define MIU1_MAP_LEN        0x20000000
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_KANO)
    #define MIPS_CLOCK_FREQ     900000000
    #define MIU0_BUS_BASE       0x20000000
    #define MIU1_BUS_BASE       0xC0000000
    #define MIU0_MAP_LEN        0x40000000
    #define MIU1_MAP_LEN        0x20000000
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_K6)
    #define MIPS_CLOCK_FREQ     900000000
    #define MIU0_BUS_BASE       0x20000000UL
    #define MIU1_BUS_BASE       0x60000000UL
    #define MIU0_MAP_LEN        0x20000000UL
    #define MIU1_MAP_LEN        0x20000000UL
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_K6LITE)
    #define MIPS_CLOCK_FREQ     900000000
    #define MIU0_BUS_BASE       0x20000000UL
    #define MIU1_BUS_BASE       0x60000000UL
    #define MIU0_MAP_LEN        0x20000000UL
    #define MIU1_MAP_LEN        0x20000000UL
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_K7U)
    #define MIPS_CLOCK_FREQ     900000000
    #define MIU0_BUS_BASE       0x20000000UL
    #define MIU1_BUS_BASE       0xa0000000UL
    #define MIU0_MAP_LEN        0x40000000UL
    #define MIU1_MAP_LEN        0x20000000UL
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_MIAMI)
    #define MIPS_CLOCK_FREQ     900000000
    #define MIU0_BUS_BASE       0x20000000
    #define MIU1_BUS_BASE       0xA0000000
    #define MIU0_MAP_LEN        0x20000000
    #define MIU1_MAP_LEN        0x20000000
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_NADAL)
    #define MIPS_CLOCK_FREQ     900000000
    #define MIU0_BUS_BASE       0x20000000
    #define MIU1_BUS_BASE       0xA0000000
    #define MIU0_MAP_LEN        0x20000000
    #define MIU1_MAP_LEN        0x20000000
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_MONACO)
    #define MIPS_CLOCK_FREQ     1008000000
    #define MIU0_BUS_BASE       0x20000000
    #define MIU1_BUS_BASE       0xA0000000
    #define MIU0_MAP_LEN        0x40000000
    #define MIU1_MAP_LEN        0x40000000
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_MUNICH)
    #define MIPS_CLOCK_FREQ     900000000
    #define MIU0_BUS_BASE       0x20000000
    #define MIU1_BUS_BASE       0xA0000000
    #define MIU0_MAP_LEN        0x40000000
    #define MIU1_MAP_LEN        0x40000000
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_MATISSE)
    #define MIPS_CLOCK_FREQ     900000000
    #define MIU0_BUS_BASE       0x20000000
    #define MIU1_BUS_BASE       0xA0000000
    #define MIU0_MAP_LEN        0x40000000
    #define MIU1_MAP_LEN        0x40000000
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_MUSTANG)
    #define MIPS_CLOCK_FREQ     900000000
    #define MIU0_BUS_BASE       0x20000000
    #define MIU1_BUS_BASE       0xA0000000
    #define MIU0_MAP_LEN        0x40000000
    #define MIU1_MAP_LEN        0x40000000
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_MARCUS)
    #define MIPS_CLOCK_FREQ     1008000000
    #define MIU0_BUS_BASE       0x20000000
    #define MIU1_BUS_BASE       0xA0000000
    #define MIU0_MAP_LEN        0x40000000
    #define MIU1_MAP_LEN        0x20000000
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_MARVEL)
    #define MIPS_CLOCK_FREQ     900000000
    #define MIU0_BUS_BASE       0x20000000
    #define MIU1_BUS_BASE       0xA0000000
    #define MIU0_MAP_LEN        0x40000000
    #define MIU1_MAP_LEN        0x40000000
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_M7221)
    #define MIPS_CLOCK_FREQ     1000000000
    #define MIU0_BUS_BASE       0x20000000
    #define MIU1_BUS_BASE       0xA0000000
    #define MIU0_MAP_LEN        0x40000000
    #define MIU1_MAP_LEN        0x40000000
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_M7622)
    #define MIPS_CLOCK_FREQ     1008000000
    #define MIU0_BUS_BASE       0x20000000
    #define MIU1_BUS_BASE       0xA0000000
    #define MIU0_MAP_LEN        0x40000000
    #define MIU1_MAP_LEN        0x20000000
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_M7322)
    #define MIPS_CLOCK_FREQ     1000000000
    #define MIU0_BUS_BASE       0x20000000
    #define MIU1_BUS_BASE       0xA0000000
    #define MIU0_MAP_LEN        0x40000000
    #define MIU1_MAP_LEN        0x40000000
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_M7632)
    #define MIPS_CLOCK_FREQ     1000000000
    #define MIU0_BUS_BASE       0x20000000
    #define MIU1_BUS_BASE       0xA0000000
    #define MIU0_MAP_LEN        0x40000000
    #define MIU1_MAP_LEN        0x40000000
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_M7332)
    #define MIPS_CLOCK_FREQ     1000000000
    #define MIU0_BUS_BASE       0x20000000
    #define MIU1_BUS_BASE       0xA0000000
    #define MIU0_MAP_LEN        0x40000000
    #define MIU1_MAP_LEN        0x40000000
#else
    #define MIPS_CLOCK_FREQ     900000000
#endif
#define TIMER_CLOCK_FREQ     12000000

#include "datatype.h"

typedef enum {
    E_EXCEPTION_DATA_TLBERROR_ACCESS = 1,    // TLB modification exception
    E_EXCEPTION_DATA_TLBMISS_ACCESS,         // TLB miss (Load or IFetch)
    E_EXCEPTION_DATA_TLBMISS_WRITE,          // TLB miss (Store)
    E_EXCEPTION_DATA_UNALIGNED_ACCESS,       // Address error (Load or Ifetch)
    E_EXCEPTION_DATA_UNALIGNED_WRITE,        // Address error (store)
    E_EXCEPTION_CODE_ACCESS,                 // Bus error (Ifetch)
    E_EXCEPTION_DATA_ACCESS,                 // Bus error (data load or store)
    E_EXCEPTION_SYSTEM_CALL,                 // System call
    E_EXCEPTION_INSTRUCTION_BP,              // Break point
    E_EXCEPTION_ILLEGAL_INSTRUCTION,         // Reserved instruction
    E_EXCEPTION_COPROCESSOR,                 // Coprocessor unusable
    E_EXCEPTION_OVERFLOW,                    // Arithmetic overflow
    E_EXCEPTION_RESERVED_13,                 // Reserved
    E_EXCEPTION_DIV_BY_ZERO,                 // Division-by-zero [reserved vector]
    E_EXCEPTION_FPU,                         // Floating point exception
    E_EXCEPTION_MAX = E_EXCEPTION_FPU,
} MHAL_EXCEPTION_TYPE;

typedef enum {
    E_INTERRUPT_02 = 0,
    E_INTERRUPT_03,
    E_INTERRUPT_04,
    E_INTERRUPT_05,
    E_INTERRUPT_06,
    E_INTERRUPT_07,
} MHAL_INTERRUPT_TYPE;

// Aliases for interrupt number
#define E_INTERRUPT_FIQ         E_INTERRUPT_03
#define E_INTERRUPT_IRQ         E_INTERRUPT_02
#define E_INTERRUPT_TICK_TIMER  E_INTERRUPT_07
#define E_INTERRUPT_MAX         E_INTERRUPT_07

typedef struct
{
    // These are common to all saved states
    unsigned long    d[32];          /* Data regs                    */
    unsigned long    hi;             /* hi word of mpy/div reg       */
    unsigned long    lo;             /* lo word of mpy/div reg       */

    // The status register contains the interrupt-enable bit which needs
    // to be preserved across context switches.
    unsigned long    sr;             /* Status Reg                   */

    // These are only saved for exceptions and interrupts
    unsigned long    vector;         /* Vector number                */
    unsigned long    pc;             /* Program Counter              */

    // These are only saved for exceptions, and are not restored
    // when continued.
    unsigned long    cause;          /* Exception cause register     */
    unsigned long    badvr;          /* Bad virtual address reg      */

} MHAL_SavedRegisters;

/// interrupt service routine prototype
typedef void (*mhal_isr_t)(void);

#elif defined (__mips__)

#ifdef CHIP_FAMILY_TYPE
#include "chiptype.h"
#if (CHIP_FAMILY_TYPE == CHIP_FAMILY_S7)
#define NONCACHABLE(_x_)    ((_x_) | 0xC0000000)
#else
/// convert address to corresponding noncacheable address
#define NONCACHABLE(_x_)    ((_x_) | 0xA0000000)
#endif
#endif

#if (CHIP_FAMILY_TYPE == CHIP_FAMILY_S7)
    #define MIPS_CLOCK_FREQ     504000000
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_S7LD)
    #define MIPS_CLOCK_FREQ     312000000
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_S7LA)
    #define MIPS_CLOCK_FREQ     312000000
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_A5)
    #define NON_PM_RIU_BASE     0xBF200000
    #define CPU_CLOCK_REG       0x10C26
    #define MIPS_CLOCK_FREQ     (RegReadByte(NON_PM_RIU_BASE, CPU_CLOCK_REG)*12)*1000000
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_A5P)
    #define MIPS_CLOCK_FREQ     720000000
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_S8)
    #define MIPS_CLOCK_FREQ     720000000
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_S7L)
    #define MIPS_CLOCK_FREQ     504000000
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_U4)
    #define MIPS_CLOCK_FREQ     504000000
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_K1)
    #define NON_PM_RIU_BASE     0xBF200000
    #define CPU_CLOCK_REG       0x10C34
    #define MIPS_CLOCK_FREQ     ((RegReadByte(NON_PM_RIU_BASE, CPU_CLOCK_REG)*24))*1000000
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_KAPPA)
    #define NON_PM_RIU_BASE     0xBF200000
    #define CPU_CLOCK_REG       0x10C34
    #define MIPS_CLOCK_FREQ     ((RegReadByte(NON_PM_RIU_BASE, CPU_CLOCK_REG)*6))*1000000
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_KENYA)
    #define MIPS_CLOCK_FREQ     936000000
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_KELTIC)
    #define NON_PM_RIU_BASE     0xBF200000
    #define CPU_CLOCK_REG       0x10C34
    #define CPU_VCO_MUX         0x10C2A  // Bit 5
    #define MIPS_CLOCK_FREQ     ((RegReadByte(NON_PM_RIU_BASE, CPU_CLOCK_REG)*12*(RegReadByte(NON_PM_RIU_BASE, CPU_VCO_MUX)>>5)) \
                                  + (RegReadByte(NON_PM_RIU_BASE, CPU_CLOCK_REG)*6*((!(RegReadByte(NON_PM_RIU_BASE, CPU_VCO_MUX)>>5))&0x1)))*1000000
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_KERES) || (CHIP_FAMILY_TYPE == CHIP_FAMILY_KRIS) || (CHIP_FAMILY_TYPE == CHIP_FAMILY_K1P) || (CHIP_FAMILY_TYPE == CHIP_FAMILY_K5AP)
    #define NPM_REG_ANAMISC        0x10A00
    #define NON_PM_RIU_BASE     0xBF200000
    #define NPM_REG_MIPS_PLLCLK_L  (NON_PM_RIU_BASE + (NPM_REG_ANAMISC+0x00C0)*2)
    #define NPM_REG_MIPS_PLLCLK_H  (NON_PM_RIU_BASE + (NPM_REG_ANAMISC+0x00C2)*2)
    #define SPEED_L                (*(volatile unsigned short*)NPM_REG_MIPS_PLLCLK_L)
    #define SPEED_H                (*(volatile unsigned short*)NPM_REG_MIPS_PLLCLK_H)
    #define MIPS_CLOCK_FREQ        (((4320000 / ((( ( (SPEED_H & 0x000000ff) << 16)|(SPEED_L & 0x0000ffff)) * 100) / 0x80000)) * 24) * 10000)
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_KRITI)
    #define NON_PM_RIU_BASE     0xBF200000
    #define CPU_CLOCK_REG       0x10C34
    #define MIPS_CLOCK_FREQ     ((RegReadByte(NON_PM_RIU_BASE, CPU_CLOCK_REG)*6))*1000000
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_KRATOS) || (CHIP_FAMILY_TYPE == CHIP_FAMILY_KIWI)
    #define NON_PM_RIU_BASE     0xBF200000
    #define MIPSPLL_DIV1_REG    0x10C32
    #define MIPSPLL_DIV2_REG    0x10C34
    #define MIPSPLL_INPUT_DIV1  ((RegReadByte(NON_PM_RIU_BASE, MIPSPLL_DIV1_REG))&0x03)
    #define MIPSPLL_LOOP_DIV1   ((RegReadByte(NON_PM_RIU_BASE, MIPSPLL_DIV1_REG)>>2)&0x03)
    #define MIPSPLL_OUTPUT_DIV1 ((RegReadByte(NON_PM_RIU_BASE, MIPSPLL_DIV1_REG)>>4)&0x03)
    #define MIPSPLL_LOOP_DIV2   (RegReadByte(NON_PM_RIU_BASE, MIPSPLL_DIV2_REG))
    #define MIPS_CLOCK_FREQ     (MIPSPLL_LOOP_DIV2*6*(1<<MIPSPLL_LOOP_DIV1))/2/(1<<MIPSPLL_OUTPUT_DIV1)*1000000
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_K2)
    #define MIPS_CLOCK_FREQ     792000000
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_J2)
    #define MIPS_CLOCK_FREQ     600000000
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_T11)
    #define MIPS_CLOCK_FREQ     550000000
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_A1)
    #define NON_PM_RIU_BASE     0xBF200000
    #define CPU_CLOCK_REG       0x10C26
    #define MIPS_CLOCK_FREQ     ((RegReadByte(NON_PM_RIU_BASE, CPU_CLOCK_REG)*12)>>1)*1000000
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_A6)
    #define NON_PM_RIU_BASE     0xBF200000
    #define CPU_CLOCK_REG       0x10C26
    #define MIPS_CLOCK_FREQ     (RegReadByte(NON_PM_RIU_BASE, CPU_CLOCK_REG)*12)*1000000
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_A7)
    #define NON_PM_RIU_BASE     0xBF200000
    #define CPU_CLOCK_REG       0x10C26
    #define MIPS_CLOCK_FREQ     ((RegReadByte(NON_PM_RIU_BASE, CPU_CLOCK_REG)*12)>>1)*1000000
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_AMETHYST)
    #define NON_PM_RIU_BASE     0xBF200000
    #define CPU_CLOCK_REG       0x10C26
    #define MIPS_CLOCK_FREQ     ((RegReadByte(NON_PM_RIU_BASE, CPU_CLOCK_REG)*12)>>1)*1000000
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_AGATE_MIPS)
    #define MIPS_CLOCK_FREQ     504000000
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_EMERALD)
    #define NON_PM_RIU_BASE     0xBF200000
    #define CPU_CLOCK_REG       0x10C26
    #define MIPS_CLOCK_FREQ     ((RegReadByte(NON_PM_RIU_BASE, CPU_CLOCK_REG)*12)>>1)*1000000
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_NUGGET)
    #define NON_PM_RIU_BASE     0xBF200000
    #define CPU_CLOCK_REG       0x10C26
    #define MIPS_CLOCK_FREQ     (RegReadByte(NON_PM_RIU_BASE, CPU_CLOCK_REG)*12)*1000000
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_NIKON)
    #define NON_PM_RIU_BASE     0xBF200000
    #define CPU_CLOCK_REG       0x10C26
    #define MIPS_CLOCK_FREQ     ((RegReadByte(NON_PM_RIU_BASE, CPU_CLOCK_REG)*12)>>1)*1000000
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_MILAN)
    #define NON_PM_RIU_BASE     0xBF200000
    #define CPU_CLOCK_REG       0x10C26
    #define MIPS_CLOCK_FREQ     (RegReadByte(NON_PM_RIU_BASE, CPU_CLOCK_REG)*6)*1000000
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_MARLON)
    #define NON_PM_RIU_BASE     0xBF200000
    #define NPM_REG_MIPS_PLLCLK_L  (NON_PM_RIU_BASE + (0x10CC0<<1))
    #define NPM_REG_MIPS_PLLCLK_H  (NON_PM_RIU_BASE + (0x10CC2<<1))
    #define SPEED_L                (*(volatile unsigned short*)NPM_REG_MIPS_PLLCLK_L)
    #define SPEED_H                (*(volatile unsigned short*)NPM_REG_MIPS_PLLCLK_H)
    #define MIPS_CLOCK_FREQ       ((0xA2000000/(SPEED_H<<16|SPEED_L))*1000000)
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_M5321)
    #define NON_PM_RIU_BASE     0xBF200000
    #define NPM_REG_MIPS_PLLCLK_L  (NON_PM_RIU_BASE + (0x10CC0<<1))
    #define NPM_REG_MIPS_PLLCLK_H  (NON_PM_RIU_BASE + (0x10CC2<<1))
    #define SPEED_L                (*(volatile unsigned short*)NPM_REG_MIPS_PLLCLK_L)
    #define SPEED_H                (*(volatile unsigned short*)NPM_REG_MIPS_PLLCLK_H)
    #define MIPS_CLOCK_FREQ       ((0xA2000000/(SPEED_H<<16|SPEED_L))*1000000)
#else
    #define MIPS_CLOCK_FREQ     576000000
#endif

#include "datatype.h"

typedef enum {
    E_EXCEPTION_DATA_TLBERROR_ACCESS = 1,    // TLB modification exception
    E_EXCEPTION_DATA_TLBMISS_ACCESS,         // TLB miss (Load or IFetch)
    E_EXCEPTION_DATA_TLBMISS_WRITE,          // TLB miss (Store)
    E_EXCEPTION_DATA_UNALIGNED_ACCESS,       // Address error (Load or Ifetch)
    E_EXCEPTION_DATA_UNALIGNED_WRITE,        // Address error (store)
    E_EXCEPTION_CODE_ACCESS,                 // Bus error (Ifetch)
    E_EXCEPTION_DATA_ACCESS,                 // Bus error (data load or store)
    E_EXCEPTION_SYSTEM_CALL,                 // System call
    E_EXCEPTION_INSTRUCTION_BP,              // Break point
    E_EXCEPTION_ILLEGAL_INSTRUCTION,         // Reserved instruction
    E_EXCEPTION_COPROCESSOR,                 // Coprocessor unusable
    E_EXCEPTION_OVERFLOW,                    // Arithmetic overflow
    E_EXCEPTION_RESERVED_13,                 // Reserved
    E_EXCEPTION_DIV_BY_ZERO,                 // Division-by-zero [reserved vector]
    E_EXCEPTION_FPU,                         // Floating point exception
    E_EXCEPTION_MAX = E_EXCEPTION_FPU,
} MHAL_EXCEPTION_TYPE;

typedef enum {
    E_INTERRUPT_02 = 0,
    E_INTERRUPT_03,
    E_INTERRUPT_04,
    E_INTERRUPT_05,
    E_INTERRUPT_06,
    E_INTERRUPT_07,
} MHAL_INTERRUPT_TYPE;

// Aliases for interrupt number
#define E_INTERRUPT_FIQ         E_INTERRUPT_03
#define E_INTERRUPT_IRQ         E_INTERRUPT_02
#define E_INTERRUPT_TICK_TIMER  E_INTERRUPT_07
#define E_INTERRUPT_MAX         E_INTERRUPT_07

typedef struct
{
    // These are common to all saved states
    unsigned long    d[32];          /* Data regs                    */
    unsigned long    hi;             /* hi word of mpy/div reg       */
    unsigned long    lo;             /* lo word of mpy/div reg       */

    // The status register contains the interrupt-enable bit which needs
    // to be preserved across context switches.
    unsigned long    sr;             /* Status Reg                   */

    // These are only saved for exceptions and interrupts
    unsigned long    vector;         /* Vector number                */
    unsigned long    pc;             /* Program Counter              */

    // These are only saved for exceptions, and are not restored
    // when continued.
    unsigned long    cause;          /* Exception cause register     */
    unsigned long    badvr;          /* Bad virtual address reg      */

} MHAL_SavedRegisters;

/// interrupt service routine prototype
typedef void (*mhal_isr_t)(MHAL_SavedRegisters *regs, U32 vector);

#else

#ifdef CHIP_FAMILY_TYPE
#include "chiptype.h"
#if (CHIP_FAMILY_TYPE == CHIP_FAMILY_S7)
#define NONCACHABLE(_x_)    ((_x_) | 0xC0000000)
#else
/// convert address to corresponding noncacheable address
#define NONCACHABLE(_x_)    ((_x_) | 0x80000000)
#endif
#endif

#include "datatype.h"

/// exception type for risc32
typedef enum {
    E_EXCEPTION_BUS_ERROR = 2,
    E_EXCEPTION_DATA_PAGE_FAULT,
    E_EXCEPTION_INSTRUCTION_PAGE_FAULT,
    E_EXCEPTION_TICK_TIMER,             ///< tick timer, do not use directly
    E_EXCEPTION_UNALIGNED_ACCESS,
    E_EXCEPTION_ILLEGAL_INSTRUCTION,
    E_EXCEPTION_EXTERNAL_INTERRUPT,     ///< external interrupt, do not use directly
    E_EXCEPTION_DTLB_MISS,
    E_EXCEPTION_ITLB_MISS,
    E_EXCEPTION_RANGE,
    E_EXCEPTION_SYSCALL,                ///< caused by l.sys
    E_EXCEPTION_RESERVED,
    E_EXCEPTION_TRAP,                   ///< caused by l.trap
    E_EXCEPTION_MAX = E_EXCEPTION_TRAP,
} MHAL_EXCEPTION_TYPE;

/// interrupt type for PIC on external interrupt exception including tick_timer
typedef enum {
    E_INTERRUPT_TICK_TIMER, //< risc32 builtin tick timer
    E_INTERRUPT_00 = 1,     //< PIC interrupt start from 1 for handler performance
    E_INTERRUPT_01,
    E_INTERRUPT_02,
    E_INTERRUPT_03,
    E_INTERRUPT_04,
    E_INTERRUPT_05,
    E_INTERRUPT_06,
    E_INTERRUPT_07,
    E_INTERRUPT_08,
    E_INTERRUPT_09,
    E_INTERRUPT_10,
    E_INTERRUPT_11,
    E_INTERRUPT_12,
    E_INTERRUPT_13,
    E_INTERRUPT_14,
    E_INTERRUPT_15,
    E_INTERRUPT_16,
    E_INTERRUPT_17,
    E_INTERRUPT_18,
    E_INTERRUPT_19,
    E_INTERRUPT_20,
    E_INTERRUPT_21,
    E_INTERRUPT_22,
    E_INTERRUPT_23,
    E_INTERRUPT_24,
    E_INTERRUPT_25,
    E_INTERRUPT_26,
    E_INTERRUPT_27,
    E_INTERRUPT_28,
    E_INTERRUPT_29,
    E_INTERRUPT_30,
    E_INTERRUPT_31,
} MHAL_INTERRUPT_TYPE;

// Aliases for interrupt number
#define E_INTERRUPT_FIQ         E_INTERRUPT_02
#define E_INTERRUPT_IRQ         E_INTERRUPT_03
#define E_INTERRUPT_UART        E_INTERRUPT_19
#define E_INTERRUPT_MAX         E_INTERRUPT_31

/// task context used for context switch
typedef struct
{
    unsigned long   r1;         ///< stack pointer
    unsigned long   r2;
    unsigned long   r3;         ///< parameter 0
    unsigned long   r9;         ///< link register
    unsigned long   r10;        ///< frame pointer
    unsigned long   r12;
    unsigned long   r14;
    unsigned long   r16;
    unsigned long   r18;
    unsigned long   r20;
    unsigned long   r22;
    unsigned long   r24;
    unsigned long   r26;
    unsigned long   r28;
    unsigned long   r30;
    unsigned long   sr;
} MHAL_Context;

/// interrupt/exception context
typedef struct
{
    unsigned long   r[32];          ///< GPR registers
#ifdef __AEONR2__
    unsigned long   machi2;         // Highest 32-bits of new 32x32=64 multiplier
#endif
    unsigned long   machi;          // High and low words of
    unsigned long   maclo;          //   multiply/accumulate reg

    // These are only saved for exceptions and interrupts
    int             vector;         ///< vector number
    int             sr;             ///< status register
    unsigned long   pc;             ///< program counter

    // Saved only for exceptions, and not restored when continued:
    // Effective address of instruction/data access that caused exception
    unsigned long   eear;           ///< exception effective address
} MHAL_SavedRegisters;

/// interrupt service routine prototype
typedef void (*mhal_isr_t)(MHAL_SavedRegisters *regs, U32 vector);
#endif


MST_BEGIN_DECL

/////////////////////////////////////////////////////////////////////////////
/// @brief register ISR of exception
/// @param <IN>     \b expt_num     : exception type
/// @param <IN>     \b isr          : exception interrupt service routine
/// @param <IN>     \b data         : user data will be passed to ISR
/// @see   mhal_exception_detach
/////////////////////////////////////////////////////////////////////////////
void mhal_exception_attach(MHAL_EXCEPTION_TYPE expt_num, mhal_isr_t isr, U32 data);

/////////////////////////////////////////////////////////////////////////////
/// @brief unregister ISR of exception
/// @param <IN>     \b expt_num     : exception type
/// @see   mhal_exception_attach
/////////////////////////////////////////////////////////////////////////////
void mhal_exception_detach(MHAL_EXCEPTION_TYPE expt_num);

/////////////////////////////////////////////////////////////////////////////
/// @brief disable interrupt controller and timer interrupt
/// @return old status
/// @see   mhal_interrupt_restore
/// @see   mhal_interrupt_enable
/////////////////////////////////////////////////////////////////////////////
int  mhal_interrupt_disable(void);

/////////////////////////////////////////////////////////////////////////////
/// @brief enable interrupt controller and timer interrupt
/// @see   mhal_interrupt_disable
/////////////////////////////////////////////////////////////////////////////
void mhal_interrupt_enable(void);

/////////////////////////////////////////////////////////////////////////////
/// @brief enable interrupt controller and timer interrupt
/// @see   mhal_interrupt_disable
/////////////////////////////////////////////////////////////////////////////
void mhal_interrupt_restore(int old);

/////////////////////////////////////////////////////////////////////////////
/// mask a source on interrupt controller
/////////////////////////////////////////////////////////////////////////////
void mhal_interrupt_mask(MHAL_INTERRUPT_TYPE intr_num);

/////////////////////////////////////////////////////////////////////////////
/// unmask a source on interrupt controller
/////////////////////////////////////////////////////////////////////////////
void mhal_interrupt_unmask(MHAL_INTERRUPT_TYPE intr_num);

/////////////////////////////////////////////////////////////////////////////
/// mask all interrupt sources on interrupt controller
/////////////////////////////////////////////////////////////////////////////
void mhal_interrupt_mask_all(void);

/////////////////////////////////////////////////////////////////////////////
/// lock interrupt, mhal_interrupt_lock() and mhal_interrupt_unlock()
/// should be used in pair, and can be nested.
/////////////////////////////////////////////////////////////////////////////
void mhal_interrupt_lock(void);

/////////////////////////////////////////////////////////////////////////////
/// unlock interrupt
/////////////////////////////////////////////////////////////////////////////
void mhal_interrupt_unlock(void);

/////////////////////////////////////////////////////////////////////////////
/// @brief register ISR of interrupt contoller
/// @param <IN>     \b expt_num     : interrupt type
/// @param <IN>     \b isr          : interrupt service routine
/// @param <IN>     \b data         : user data will be passed to ISR
/// @see   mhal_interrupt_detach
/////////////////////////////////////////////////////////////////////////////
void mhal_interrupt_attach(MHAL_INTERRUPT_TYPE intr_num, mhal_isr_t isr, U32 data);

/////////////////////////////////////////////////////////////////////////////
/// @brief unregister ISR of interrupt controller
/// @param <IN>     \b expt_num     : interrupt type
/// @see   mhal_interrupt_attach
/////////////////////////////////////////////////////////////////////////////
void mhal_interrupt_detach(MHAL_INTERRUPT_TYPE intr_num);

/////////////////////////////////////////////////////////////////////////////
/// please note original variables or parameters in current stack frame will be lost
/// @brief set stack of current context
/// @param <IN>     \b stack:   stack pointer (high address)
/////////////////////////////////////////////////////////////////////////////
void mhal_stack_init(void *stack);

/////////////////////////////////////////////////////////////////////////////
/// @brief flush dcache
/// @param <IN>     \b u32Base: memory region base address
/// @param <IN>     \b u32Size: memory region size
/////////////////////////////////////////////////////////////////////////////
void mhal_dcache_flush(U32 u32Base, U32 u32Size);

#if (CHIP_FAMILY_TYPE == CHIP_FAMILY_A3 || CHIP_FAMILY_TYPE == CHIP_FAMILY_EDISON || CHIP_FAMILY_TYPE == CHIP_FAMILY_A5 || CHIP_FAMILY_TYPE == CHIP_FAMILY_A5P || CHIP_FAMILY_TYPE == CHIP_FAMILY_S8 || CHIP_FAMILY_TYPE == CHIP_FAMILY_J2 || CHIP_FAMILY_TYPE == CHIP_FAMILY_T11)
/////////////////////////////////////////////////////////////////////////////
/// @brief L2 flush dcache
/// @param <IN>     \b u32Base: memory region base address
/// @param <IN>     \b u32Size: memory region size
/////////////////////////////////////////////////////////////////////////////
void mhal_L2Cache_Flush(void);

/////////////////////////////////////////////////////////////////////////////
/// @brief L2 flush Read
/// @param <IN>     \b u32Base: memory region base address
/// @param <IN>     \b u32Size: memory region size
/////////////////////////////////////////////////////////////////////////////
void mhal_L2Cache_Read(void);
#endif

/// task main function prototype
typedef int (mhal_task_func)(int arg);

#if defined (__aeon__)

/////////////////////////////////////////////////////////////////////////////
/// @brief setup context structure for task creation
/// @param <IN>     \b stkptr:  stack pointer (bottom: low address)
/// @param <IN>     \b stklen:  stack size
/// @param <IN>     \b start:   task main function
/// @param <IN>     \b arg:     argument for task
/////////////////////////////////////////////////////////////////////////////
MHAL_Context *mhal_context_setup(unsigned long *stkptr, int stklen, mhal_task_func start, int arg);

/////////////////////////////////////////////////////////////////////////////
/// @brief switch to new context and save current context to stack
/// @param <IN>     \b pNextCtx:        new context's stack pointer to context
/// @param <IN>     \b pSavedCtxPtr:    pointer to variable that is used to store
///                                     the pointer of current tack context
/////////////////////////////////////////////////////////////////////////////
void mhal_context_switch(MHAL_Context *pNextCtx, MHAL_Context **pSaveCtxPtr);

/////////////////////////////////////////////////////////////////////////////
/// @brief switch to new context
/// @param <IN>     \b pNextCtx:        new context's stack pointer to context
/////////////////////////////////////////////////////////////////////////////
void mhal_context_load(MHAL_Context *pNextCtx);

#endif

MST_END_DECL

#endif /* __MHAL_ARCH_H__ */
