/*
 * arch/arm64/include/asm/arch_timer.h
 *
 * Copyright (C) 2012 ARM Ltd.
 * Author: Marc Zyngier <marc.zyngier@arm.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef __ASM_ARCH_TIMER_H
#define __ASM_ARCH_TIMER_H

#include <barrier.h>

#define ARCH_TIMER_CTRL_ENABLE		(1 << 0)
#define ARCH_TIMER_CTRL_IT_MASK		(1 << 1)
#define ARCH_TIMER_CTRL_IT_STAT		(1 << 2)

#define ARCH_TIMER_REG_CTRL		0
#define ARCH_TIMER_REG_TVAL		1
#define ARCH_TIMER_REG_CVAL		2
#define ARCH_TIMER_REG_CT		3

#define ARCH_TIMER_PHYS_ACCESS		0
#define ARCH_TIMER_VIRT_ACCESS		1

static inline void arch_timer_reg_write(int access, int reg, unsigned long u64val)
{
	unsigned int u32val;
        u32val = (unsigned int) u64val;

	if (access == ARCH_TIMER_PHYS_ACCESS) {
		switch (reg) {
		case ARCH_TIMER_REG_CTRL:
			asm volatile("msr cntp_ctl_el0,  %0" : : "r" (u32val));
			break;
		case ARCH_TIMER_REG_TVAL:
			asm volatile("msr cntp_tval_el0, %0" : : "r" (u32val));
			break;
		case ARCH_TIMER_REG_CVAL:
			asm volatile("msr cntp_cval_el0, %0" : : "r" (u64val));
			break;
		default:
			break;
		}
	} else if (access == ARCH_TIMER_VIRT_ACCESS) {
		switch (reg) {
		case ARCH_TIMER_REG_CTRL:
			asm volatile("msr cntv_ctl_el0,  %0" : : "r" (u32val));
			break;
		case ARCH_TIMER_REG_TVAL:
			asm volatile("msr cntv_tval_el0, %0" : : "r" (u32val));
			break;
		case ARCH_TIMER_REG_CVAL:
			asm volatile("msr cntv_cval_el0, %0" : : "r" (u64val));
			break;
		default:
			break;
		}
	} else {
	}

	isb();
}

static inline unsigned long arch_timer_reg_read(int access, int reg)
{
	unsigned int u32val;
	unsigned long u64val;

	if (access == ARCH_TIMER_PHYS_ACCESS) {
		switch (reg) {
		case ARCH_TIMER_REG_CTRL:
			asm volatile("mrs %0,  cntp_ctl_el0" : "=r" (u32val));
                        u64val = (unsigned long)u32val;
			break;
		case ARCH_TIMER_REG_TVAL:
			asm volatile("mrs %0, cntp_tval_el0" : "=r" (u32val));
                        u64val = (unsigned long)u32val;
			break;
		case ARCH_TIMER_REG_CVAL:
			asm volatile("mrs %0, cntp_cval_el0" : "=r" (u64val));
			break;
		case ARCH_TIMER_REG_CT:
			asm volatile("mrs %0, cntpct_el0" : "=r" (u64val));
			break;
		default:
			break;
		}
	} else if (access == ARCH_TIMER_VIRT_ACCESS) {
		switch (reg) {
		case ARCH_TIMER_REG_CTRL:
			asm volatile("mrs %0,  cntv_ctl_el0" : "=r" (u32val));
                        u64val = (unsigned long)u32val;
			break;
		case ARCH_TIMER_REG_TVAL:
			asm volatile("mrs %0, cntv_tval_el0" : "=r" (u32val));
                        u64val = (unsigned long)u32val;
			break;
		case ARCH_TIMER_REG_CVAL:
			asm volatile("mrs %0, cntv_cval_el0" : "=r" (u64val));
			break;
		case ARCH_TIMER_REG_CT:
			asm volatile("mrs %0, cntvct_el0" : "=r" (u64val));
			break;
		default:
			break;
		}
	} else {
	}
        
	return u64val;
}

static inline unsigned int arch_timer_get_cntfrq(void)
{
	unsigned int val;
	asm volatile("mrs %0,   cntfrq_el0" : "=r" (val));
	return val;
}

static inline unsigned long arch_counter_get_cntpct(void)
{
	unsigned long cval;

	isb();
	asm volatile("mrs %0, cntpct_el0" : "=r" (cval));

	return cval;
}

static inline unsigned long arch_counter_get_cntvct(void)
{
	unsigned long cval;

	isb();
	asm volatile("mrs %0, cntvct_el0" : "=r" (cval));

	return cval;
}

int arch_timer_arch_init(void)
{
    unsigned long ctrl;
    unsigned long cntvct;    //virtual time count
    unsigned long cntv_cval; //virtual time compare value

    /*Generic timer is 12MHZ, 0x3000 means 1ms*/
    cntvct = arch_timer_reg_read(ARCH_TIMER_VIRT_ACCESS, ARCH_TIMER_REG_CT);
    cntv_cval = cntvct + 0x3000;
    arch_timer_reg_write(ARCH_TIMER_VIRT_ACCESS, ARCH_TIMER_REG_CVAL, cntv_cval);
   
    /* Enable virtual timer and unmask interrupt*/
    ctrl = 0;
    ctrl |= ARCH_TIMER_CTRL_ENABLE;
    arch_timer_reg_write(ARCH_TIMER_VIRT_ACCESS, ARCH_TIMER_REG_CTRL, ctrl);
}

#endif
