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
#include <stdio.h>
#include <assert.h>
#include "datatype.h"
#include "mhal_arch.h"
#include "ptrace.h"

struct interrupt
{
    mhal_isr_t  isr;
    U32         data;
};

static struct interrupt intr_table[E_INTERRUPT_MAX + 1];

static volatile int nr_intr_locks;
static volatile int saved_intr_state;

int mhal_interrupt_disable(void)
{
    unsigned long old,temp,value;

    asm volatile ("mrs %0, cpsr\n"
                  "mrs %2, cpsr\n"
                  "orr %1, %0, #0xc0\n"
                  "msr cpsr_c, %1"
                  : "=r" (old), "=r" (temp), "=r" (value)
                  :
                  : "memory");
    value = value & 0xc0;
    return (int)value;
}

void mhal_interrupt_enable(void)
{
    /* enable IRQ/FIQ interrupts */
    unsigned long temp;

    asm volatile("mrs %0, cpsr\n"
                 "bic %0, %0, #0x80\n"
                 "bic %0, %0, #0x40\n"
                 "msr cpsr_c, %0"
                 : "=r" (temp)
                 :
                 : "memory");
}

void mhal_interrupt_restore(int old)
{
    unsigned long temp,value;
    value = (unsigned long)old;

    asm volatile("mrs %0, cpsr\n"
                 "bic %0, %0, #0x80\n"
                 "bic %0, %0, #0x40\n"
                 "orr %0, %0, %1\n"
                 "msr cpsr_c, %0"
                 : "=r" (temp), "=r" (value)
                 :
                 : "memory");
}

void mhal_interrupt_mask_all(void)
{
    mhal_interrupt_disable();
}

void mhal_interrupt_mask(MHAL_INTERRUPT_TYPE intr_num)
{
    unsigned long temp,value;

    if(intr_num == E_INTERRUPT_FIQ)
        value = 0x40;
    else if(intr_num == E_INTERRUPT_IRQ)
        value = 0x80;
    else
        return;

    asm volatile("mrs %0, cpsr\n"
                 "bic %0, %0, #0x80\n"
                 "bic %0, %0, #0x40\n"
                 "orr %0, %0, %1\n"
                 "msr cpsr_c, %0"
                 : "=r" (temp), "=r" (value)
                 :
                 : "memory");
}

void mhal_interrupt_unmask(MHAL_INTERRUPT_TYPE intr_num)
{
    unsigned long temp;

    if(intr_num == E_INTERRUPT_FIQ)
    {
         asm volatile("mrs %0, cpsr\n"
                      "bic %0, %0, #0x40\n"
                      "msr cpsr_c, %0"
                      : "=r" (temp)
                      :
                      : "memory");
    }
    else if(intr_num == E_INTERRUPT_IRQ)
    {
         asm volatile("mrs %0, cpsr\n"
                      "bic %0, %0, #0x80\n"
                      "msr cpsr_c, %0"
                      : "=r" (temp)
                      :
                      : "memory");
    }
    else
        return;
}

void mhal_interrupt_lock(void)
{
    int spr_sr;

    spr_sr = mhal_interrupt_disable();
    if (++nr_intr_locks == 1)
    {
        saved_intr_state = spr_sr;
    }

    ASSERT(nr_intr_locks > 0);
}

void mhal_interrupt_unlock(void)
{
    ASSERT(nr_intr_locks > 0);

    if (--nr_intr_locks == 0)
    {
       mhal_interrupt_restore(saved_intr_state);
    }
}

void mhal_interrupt_attach(MHAL_INTERRUPT_TYPE intr_num, mhal_isr_t isr, U32 dat)
{
    if (intr_num <= E_INTERRUPT_MAX)
    {
        intr_table[intr_num].isr = isr;
        intr_table[intr_num].data = dat;
        mhal_interrupt_unmask(intr_num);
    }
}

void mhal_interrupt_detach(MHAL_INTERRUPT_TYPE intr_num)
{
    if (intr_num <= E_INTERRUPT_MAX)
    {
        intr_table[intr_num].isr = 0;
        mhal_interrupt_mask(intr_num);
    }
}

void show_regs (struct pt_regs *regs)
{
    unsigned long flags;
    const char *processor_modes[] = {
    "USER_26",  "FIQ_26",   "IRQ_26",   "SVC_26",
    "UK4_26",   "UK5_26",   "UK6_26",   "UK7_26",
    "UK8_26",   "UK9_26",   "UK10_26",  "UK11_26",
    "UK12_26",  "UK13_26",  "UK14_26",  "UK15_26",
    "USER_32",  "FIQ_32",   "IRQ_32",   "SVC_32",
    "UK4_32",   "UK5_32",   "UK6_32",   "ABT_32",
    "UK8_32",   "UK9_32",   "UK10_32",  "UND_32",
    "UK12_32",  "UK13_32",  "UK14_32",  "SYS_32",
    };

    flags = condition_codes (regs);

    printf ("\npc : [<%08lx>]      lr : [<%08lx>]\n"
        "sp : %08lx  ip : %08lx  fp : %08lx\n",
        instruction_pointer (regs),
        regs->ARM_lr, regs->ARM_sp, regs->ARM_ip, regs->ARM_fp);
    printf ("r10: %08lx  r9 : %08lx  r8 : %08lx\n",
        regs->ARM_r10, regs->ARM_r9, regs->ARM_r8);
    printf ("r7 : %08lx  r6 : %08lx  r5 : %08lx  r4 : %08lx\n",
        regs->ARM_r7, regs->ARM_r6, regs->ARM_r5, regs->ARM_r4);
    printf ("r3 : %08lx  r2 : %08lx  r1 : %08lx  r0 : %08lx\n",
        regs->ARM_r3, regs->ARM_r2, regs->ARM_r1, regs->ARM_r0);
    printf ("Flags: %c%c%c%c",
        flags & CC_N_BIT ? 'N' : 'n',
        flags & CC_Z_BIT ? 'Z' : 'z',
        flags & CC_C_BIT ? 'C' : 'c', flags & CC_V_BIT ? 'V' : 'v');
    printf ("  IRQs %s  FIQs %s  Mode %s%s\n",
        interrupts_enabled (regs) ? "on" : "off",
        fast_interrupts_enabled (regs) ? "on" : "off",
        processor_modes[processor_mode (regs)],
        thumb_mode (regs) ? " (T)" : "");
}

void do_undefined_instruction (struct pt_regs *pt_regs)
{
    show_regs (pt_regs);
    printf ("\nUndefined instruction......\n");
}

void do_software_interrupt (struct pt_regs *pt_regs)
{
    show_regs (pt_regs);
    printf ("\nSoftware interrupt.....\n");
}

void do_prefetch_abort (struct pt_regs *pt_regs)
{
    show_regs (pt_regs);
    printf ("\nPrefetch abort.....\n");
}

void do_data_abort (struct pt_regs *pt_regs)
{
    show_regs (pt_regs);
    printf ("\nData abort.....\n");
}

void do_not_used (struct pt_regs *pt_regs)
{
    show_regs (pt_regs);
    printf ("\nNot used.....\n");
}

void do_fiq (void)
{
    unsigned long picvec;
    mhal_isr_t    isr;

    picvec = E_INTERRUPT_FIQ;

    isr = intr_table[picvec].isr;
    if (isr)
    {
       isr();
    }
}

void do_irq (void)
{
    unsigned long picvec;
    mhal_isr_t    isr;

    picvec = E_INTERRUPT_IRQ;

    isr = intr_table[picvec].isr;
    if (isr)
    {
        isr();
    }

    //Merge Mstar FIQ handler into IRQ
    picvec = E_INTERRUPT_FIQ;

    isr = intr_table[picvec].isr;
    if (isr)
    {
       isr();
    }
}

void timer_interrupt_handler (void)
{
    unsigned long   picvec;
    mhal_isr_t      isr;
    unsigned int    dwRegisterValue = 0;

    asm volatile("mrc p15, 0, %0, c14, c2, 1" : "=r" (dwRegisterValue));
    if((dwRegisterValue & 0x04) != 0)
    {
        //Disable Physical Timer
        asm volatile("mcr p15, 0, %0, c14, c2, 1" : : "r" (0));

        picvec = E_INTERRUPT_TICK_TIMER;

        isr = intr_table[picvec].isr;
        if (isr)
        {
            isr();
        }

        //Reload Initial Value of Physical Timer
        asm volatile("mcr p15, 0, %0, c14, c2, 0" : : "r" (TIMER_CLOCK_FREQ / 1000));

        //Enable Physical Timer
        asm volatile("mcr p15, 0, %0, c14, c2, 1" : : "r" (1));
    }
}

void mhal_fiq_merge (void)
{
#if (CHIP_FAMILY_TYPE != CHIP_FAMILY_KANO)
#if ((CHIP_FAMILY_TYPE == CHIP_FAMILY_NIKE) 	 || \
     (CHIP_FAMILY_TYPE == CHIP_FAMILY_MADISON) 	 || \
     (CHIP_FAMILY_TYPE == CHIP_FAMILY_MONET)     || \
     (CHIP_FAMILY_TYPE == CHIP_FAMILY_MESSI)     || \
     (CHIP_FAMILY_TYPE == CHIP_FAMILY_CELTICS)   || \
     (CHIP_FAMILY_TYPE == CHIP_FAMILY_CLIPPERS)  || \
     (CHIP_FAMILY_TYPE == CHIP_FAMILY_MUJI)      || \
     (CHIP_FAMILY_TYPE == CHIP_FAMILY_MANHATTAN) || \
     (CHIP_FAMILY_TYPE == CHIP_FAMILY_NADAL))
    unsigned int    dwRegisterData = 0;

    dwRegisterData = *(volatile unsigned int*)(RIUBASE + (0x123964 << 1));
    dwRegisterData &= 0xFFDF;
    dwRegisterData |= 0x0050;
    *(volatile unsigned int*)(RIUBASE + (0x123964 << 1)) = dwRegisterData;

#else
    U32 u32Tmp;

    u32Tmp = *(volatile U32*)(RIUBASE + 0x203ADC);
    u32Tmp |= 0x20;
    *(volatile U32*)(RIUBASE + 0x203ADC) = u32Tmp;
#endif
#endif
}
