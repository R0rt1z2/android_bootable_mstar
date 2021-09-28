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

#ifdef __AEONR2__
#if ((CHIP_FAMILY_TYPE == CHIP_FAMILY_S8) || \
     (CHIP_FAMILY_TYPE == CHIP_FAMILY_A5) || \
     (CHIP_FAMILY_TYPE == CHIP_FAMILY_A5P) || \
     (CHIP_FAMILY_TYPE == CHIP_FAMILY_A3) || \
     (CHIP_FAMILY_TYPE == CHIP_FAMILY_EDISON) || \
     (CHIP_FAMILY_TYPE == CHIP_FAMILY_EIFFEL) || \
     (CHIP_FAMILY_TYPE == CHIP_FAMILY_NIKE) || \
     (CHIP_FAMILY_TYPE == CHIP_FAMILY_MADISON) || \
     (CHIP_FAMILY_TYPE == CHIP_FAMILY_MONET) || \
     (CHIP_FAMILY_TYPE == CHIP_FAMILY_MOONEY) || \
     (CHIP_FAMILY_TYPE == CHIP_FAMILY_MESSI) || \
     (CHIP_FAMILY_TYPE == CHIP_FAMILY_MACAN) || \
     (CHIP_FAMILY_TYPE == CHIP_FAMILY_M3822) || \
     (CHIP_FAMILY_TYPE == CHIP_FAMILY_CELTICS) || \
     (CHIP_FAMILY_TYPE == CHIP_FAMILY_KANO) || \
     (CHIP_FAMILY_TYPE == CHIP_FAMILY_K6) || \
     (CHIP_FAMILY_TYPE == CHIP_FAMILY_K6LITE) || \
     (CHIP_FAMILY_TYPE == CHIP_FAMILY_K7U) || \
     (CHIP_FAMILY_TYPE == CHIP_FAMILY_MUJI)  || \
     (CHIP_FAMILY_TYPE == CHIP_FAMILY_MUNICH)  || \
     (CHIP_FAMILY_TYPE == CHIP_FAMILY_MUSTANG)  || \
     (CHIP_FAMILY_TYPE == CHIP_FAMILY_MIAMI) || \
     (CHIP_FAMILY_TYPE == CHIP_FAMILY_NADAL) || \
     (CHIP_FAMILY_TYPE == CHIP_FAMILY_NUGGET) || \
     (CHIP_FAMILY_TYPE == CHIP_FAMILY_CURRY) || \
     (CHIP_FAMILY_TYPE == CHIP_FAMILY_C2P) || \
     (CHIP_FAMILY_TYPE == CHIP_FAMILY_C2S) || \
     (CHIP_FAMILY_TYPE == CHIP_FAMILY_MARVEL))
#define AEON_CLOCK_FREQ 240000000
#elif(CHIP_FAMILY_TYPE == CHIP_FAMILY_EINSTEIN || \
    CHIP_FAMILY_TYPE == CHIP_FAMILY_NAPOLI || \
    CHIP_FAMILY_TYPE == CHIP_FAMILY_CLIPPERS)
#define AEON_CLOCK_FREQ 288000000
#elif((CHIP_FAMILY_TYPE == CHIP_FAMILY_MANHATTAN) || \
      (CHIP_FAMILY_TYPE == CHIP_FAMILY_MASERATI)|| \
      (CHIP_FAMILY_TYPE == CHIP_FAMILY_MYSTIC)  || \
      (CHIP_FAMILY_TYPE == CHIP_FAMILY_MAXIM)   || \
      (CHIP_FAMILY_TYPE == CHIP_FAMILY_MCLAREN) || \
      (CHIP_FAMILY_TYPE == CHIP_FAMILY_MARCUS) || \
      (CHIP_FAMILY_TYPE == CHIP_FAMILY_M7221) || \
      (CHIP_FAMILY_TYPE == CHIP_FAMILY_M7622) || \
      (CHIP_FAMILY_TYPE == CHIP_FAMILY_M7322) || \
      (CHIP_FAMILY_TYPE == CHIP_FAMILY_M7632) || \
      (CHIP_FAMILY_TYPE == CHIP_FAMILY_M7332))
  #if (FRC_ENABLED==1)
    #define AEON_CLOCK_FREQ  345000000 //max clock speed for FRC-R2
  #else
    #define AEON_CLOCK_FREQ  240000000 //max clock speed for SEC-R2
  #endif
#else
#define AEON_CLOCK_FREQ 384000000
#endif
#else
#define AEON_CLOCK_FREQ 172800000
#endif



#ifdef __AEONR2__
  #if(CHIP_FAMILY_TYPE == CHIP_FAMILY_MACAN) || (CHIP_FAMILY_TYPE == CHIP_FAMILY_M3822)
    #ifdef AEON_CLOCK_FREQ
    #undef AEON_CLOCK_FREQ
    #define NON_PM_RIU_BASE  0xFA000000
    #define CPU_CLOCK_REG    0x10337A //Secure-R2 clock
    #define AEON_CLOCK_FREQ  ((RegReadByte(NON_PM_RIU_BASE, CPU_CLOCK_REG) == 0x00)? 288000000 : \
                              (RegReadByte(NON_PM_RIU_BASE, CPU_CLOCK_REG) == 0x04)? 240000000 : \
                              (RegReadByte(NON_PM_RIU_BASE, CPU_CLOCK_REG) == 0x08)? 216000000 : 12000000)
    #endif
  #endif
#endif

struct interrupt
{
    mhal_isr_t  isr;
    U32         data;
};

static void __mhal_default_isr(MHAL_SavedRegisters *regs, U32 vector)
{
    printf("\nmhal_default_isr vector: %d regs: %p\n", vector, regs);
    dump_backtrace();
    while(1);
}

void mhal_default_isr(MHAL_SavedRegisters *regs, U32 vector) __attribute__ ((weak, alias("__mhal_default_isr")));

static struct interrupt intr_table[E_INTERRUPT_MAX + 1];

static volatile int nr_intr_locks;
static volatile int saved_intr_state;

int  mhal_interrupt_disable(void)
{
    int spr_sr;
    __mhal_interrupt_disable(spr_sr);
    return spr_sr;
}

void mhal_interrupt_enable(void)
{
    __mhal_interrupt_enable();
}

void mhal_interrupt_restore(int old)
{
    __mhal_interrupt_restore(old);
}

void mhal_interrupt_mask_all(void)
{
    mtspr(SPR_PICMR, 0x00000000);
}

void mhal_interrupt_mask(MHAL_INTERRUPT_TYPE intr_num)
{
    __mhal_interrupt_mask(intr_num);
}

void mhal_interrupt_unmask(MHAL_INTERRUPT_TYPE intr_num)
{
    __mhal_interrupt_unmask(intr_num);
}

void mhal_interrupt_lock(void)
{
    int spr_sr;

    __mhal_interrupt_disable(spr_sr);
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
        __mhal_interrupt_restore(saved_intr_state);
    }
}

void mhal_interrupt_attach(MHAL_INTERRUPT_TYPE intr_num, mhal_isr_t isr, U32 dat)
{
    if (intr_num <= E_INTERRUPT_MAX)
    {
        intr_table[intr_num].isr = isr;
        intr_table[intr_num].data = dat;
    }
}

void mhal_interrupt_detach(MHAL_INTERRUPT_TYPE intr_num)
{
    if (intr_num <= E_INTERRUPT_MAX)
    {
        intr_table[intr_num].isr = 0;
    }
}

#define ISR_TIMER0_PERIOD_US        1000UL  // : 1000us, unit:us, Timer 0 Interrupt period

void mhal_interrupt_handler(MHAL_SavedRegisters *regs, int vector)
{
    unsigned long spr;
    unsigned long picvec;
    mhal_isr_t    isr;

    UNUSED(vector);

    spr = mfspr(SPR_PICSR);
    picvec = __mhal_lsbit_index(spr);
    isr = intr_table[picvec].isr;
    if (isr)
    {
       isr(regs, intr_table[picvec].data);
    }
    else
    {
       mhal_default_isr(regs, picvec);
    }
    if (picvec)
    {
        picvec--;
        spr = mfspr(SPR_PICMR);
        mtspr(SPR_PICMR, spr & ~(1 << picvec));
        mtspr(SPR_PICSR, mfspr(SPR_PICSR) & ~(1 << picvec));
        mtspr(SPR_PICMR, spr);
    }

    if(picvec==0)
    {
        #if (ISR_TIMER0_PERIOD_US>1000000)
        mtspr(SPR_TTMR, 0x60000000 | (AEON_CLOCK_FREQ * (ISR_TIMER0_PERIOD_US / 1000000)));
        #elif (ISR_TIMER0_PERIOD_US>0)
        mtspr(SPR_TTMR, 0x60000000 | (AEON_CLOCK_FREQ / (1000000 / ISR_TIMER0_PERIOD_US)));
        #else //(ISR_TIMER0_PERIOD_US==0)
            #error ISR_TIMER0_PERIOD_US is 0
        #endif
    }
}

void mhal_interrupt_dumpvector()
{
    unsigned int i;
    for(i=0;i< E_INTERRUPT_MAX;i++)
    {
        printf("vector %d addr:%lx data:%lx\n",i,intr_table[i].isr,intr_table[i].data);
    }
}

