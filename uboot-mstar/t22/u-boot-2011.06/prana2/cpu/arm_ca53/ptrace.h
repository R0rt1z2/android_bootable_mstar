/*
 * Based on arch/arm/include/asm/ptrace.h
 *
 * Copyright (C) 1996-2003 Russell King
 * Copyright (C) 2012 ARM Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
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
#ifndef __ASM_PTRACE_H
#define __ASM_PTRACE_H

/*
 * PSR bits
 */
#define PSR_MODE_EL0t	0x00000000
#define PSR_MODE_EL1t	0x00000004
#define PSR_MODE_EL1h	0x00000005
#define PSR_MODE_EL2t	0x00000008
#define PSR_MODE_EL2h	0x00000009
#define PSR_MODE_EL3t	0x0000000c
#define PSR_MODE_EL3h	0x0000000d
#define PSR_MODE_MASK	0x0000000f

#define BOOT_CPU_MODE_EL2       0x0e12b007

/* AArch32 CPSR bits */
#define PSR_MODE32_BIT		0x00000010

/* AArch64 SPSR bits */
#define PSR_F_BIT	0x00000040
#define PSR_I_BIT	0x00000080
#define PSR_A_BIT	0x00000100
#define PSR_D_BIT	0x00000200
#define PSR_Q_BIT	0x08000000
#define PSR_V_BIT	0x10000000
#define PSR_C_BIT	0x20000000
#define PSR_Z_BIT	0x40000000
#define PSR_N_BIT	0x80000000

/*
 * Groups of PSR bits
 */
#define PSR_f		0xff000000	/* Flags		*/
#define PSR_s		0x00ff0000	/* Status		*/
#define PSR_x		0x0000ff00	/* Extension		*/
#define PSR_c		0x000000ff	/* Control		*/

/* AArch32-specific ptrace requests */
#define COMPAT_PTRACE_GETREGS		12
#define COMPAT_PTRACE_SETREGS		13
#define COMPAT_PTRACE_GET_THREAD_AREA	22
#define COMPAT_PTRACE_SET_SYSCALL	23
#define COMPAT_PTRACE_GETVFPREGS	27
#define COMPAT_PTRACE_SETVFPREGS	28
#define COMPAT_PTRACE_GETHBPREGS	29
#define COMPAT_PTRACE_SETHBPREGS	30

/* AArch32 CPSR bits */
#define COMPAT_PSR_MODE_MASK	0x0000001f
#define COMPAT_PSR_MODE_USR	0x00000010
#define COMPAT_PSR_MODE_FIQ	0x00000011
#define COMPAT_PSR_MODE_IRQ	0x00000012
#define COMPAT_PSR_MODE_SVC	0x00000013
#define COMPAT_PSR_MODE_ABT	0x00000017
#define COMPAT_PSR_MODE_HYP	0x0000001a
#define COMPAT_PSR_MODE_UND	0x0000001b
#define COMPAT_PSR_MODE_SYS	0x0000001f
#define COMPAT_PSR_T_BIT	0x00000020
#define COMPAT_PSR_F_BIT	0x00000040
#define COMPAT_PSR_I_BIT	0x00000080
#define COMPAT_PSR_A_BIT	0x00000100
#define COMPAT_PSR_E_BIT	0x00000200
#define COMPAT_PSR_J_BIT	0x01000000
#define COMPAT_PSR_Q_BIT	0x08000000
#define COMPAT_PSR_V_BIT	0x10000000
#define COMPAT_PSR_C_BIT	0x20000000
#define COMPAT_PSR_Z_BIT	0x40000000
#define COMPAT_PSR_N_BIT	0x80000000
#define COMPAT_PSR_IT_MASK	0x0600fc00	/* If-Then execution state mask */
/*
 * These are 'magic' values for PTRACE_PEEKUSR that return info about where a
 * process is located in memory.
 */
#define COMPAT_PT_TEXT_ADDR		0x10000
#define COMPAT_PT_DATA_ADDR		0x10004
#define COMPAT_PT_TEXT_END_ADDR		0x10008
#ifndef __ASSEMBLY__

/* sizeof(struct user) for AArch32 */
#define COMPAT_USER_SZ	296

/* Architecturally defined mapping between AArch32 and AArch64 registers */
#define compat_usr(x)	regs[(x)]
#define compat_sp	regs[13]
#define compat_lr	regs[14]
#define compat_sp_hyp	regs[15]
#define compat_sp_irq	regs[16]
#define compat_lr_irq	regs[17]
#define compat_sp_svc	regs[18]
#define compat_lr_svc	regs[19]
#define compat_sp_abt	regs[20]
#define compat_lr_abt	regs[21]
#define compat_sp_und	regs[22]
#define compat_lr_und	regs[23]
#define compat_r8_fiq	regs[24]
#define compat_r9_fiq	regs[25]
#define compat_r10_fiq	regs[26]
#define compat_r11_fiq	regs[27]
#define compat_r12_fiq	regs[28]
#define compat_sp_fiq	regs[29]
#define compat_lr_fiq	regs[30]


#define instruction_pointer(regs)	(regs)->pc

#endif /* __ASSEMBLY__ */
#endif
