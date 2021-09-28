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
#include "datatype.h"
#include "mhal_arch.h"

#define ABS(x)              (((x)<0) ? -(x) : (x))

static struct
{
    mhal_isr_t  isr;
    U32         data;
} expt_table[E_EXCEPTION_MAX + 1];

void mhal_exception_attach(MHAL_EXCEPTION_TYPE expt_num, mhal_isr_t isr, U32 data)
{
    int old;
    if ((U32)expt_num <= E_EXCEPTION_MAX)
    {
        __mhal_interrupt_disable(old);
        expt_table[expt_num].isr = isr;
        expt_table[expt_num].data = data;
        __mhal_interrupt_restore(old);
    }
}

void mhal_exception_detach(MHAL_EXCEPTION_TYPE expt_num)
{
    if ((U32)expt_num <= E_EXCEPTION_MAX)
    {
        expt_table[expt_num].isr = 0;
    }
}

void mhal_exception_handler(MHAL_SavedRegisters *regs, int vector)
{
    unsigned long *addr;
    unsigned long *ra;
    unsigned long *sp;
    unsigned short depth;
    unsigned short ra_offset;
    unsigned short stack_size;

    printf("\nException: %d\n", ((vector & 0x0000007C) >> 2));
    switch ((vector & 0x0000007C) >> 2)
    {
        case 0:
            printf("Int: Interrupt\n");
            break;
        case 1:
            printf("Mod: Store, but page marked as read-only in the TLB\n");
            break;
        case 2:
            printf("TLBL: Load or fetch, but page marked as invalid in the TLB\n");
            break;
        case 3:
            printf("TLBS: Store, but page marked as invalid in the TLB\n");
            break;
        case 4:
            printf("AdEL: Address error on load/fetch or store respectively. Address is either wrongly aligned, or a privilege violation\n");
            break;
        case 5:
            printf("AdES: Address error on load/fetch or store respectively. Address is either wrongly aligned, or a privilege violation\n");
            break;
        case 6:
            printf("IBE: Bus error signaled on instruction fetch\n");
            break;
        case 7:
            printf("DBE: Bus error signaled on load/store (imprecise)\n");
            break;
        case 8:
            printf("Sys: System call, ie syscall instruction executed.\n");
            break;
        case 9:
            printf("Bp: Breakpoint, ie break instruction executed.\n");
            break;
        case 10:
            printf("RI: Instruction code not recognized (or not legal)\n");
            break;
        case 11:
            printf("CpU: Co-processor instruction encoding for co-processor which is not enabled in Status[CU3-0].\n");
            break;
        case 12:
            printf("Ov: Overflow from trapping form of integer arithmetic instructions.\n");
            break;
        case 13:
            printf("Tr: Condition met on one of the conditional trap instructions teq etc.\n");
            break;
        case 14:
            printf("-: Reserved\n");
            break;
        case 15:
            printf("FPE: Floating point unit exception - more details in FCSR.\n");
            break;
        case 16:
            printf("-: Available for implementation dependent use\n");
            break;
        case 17:
            printf("CeU: CorExtend instruction attempted when not enable by Status[CEE]\n");
            break;
        case 18:
            printf("C2E: Reserved for precise Coprocessor 2 exceptions\n");
            break;
        case 19:
        case 20:
        case 21:
            printf("-: Reserved\n");
            break;
        case 22:
            printf("MDMX: Tried to run an MDMX instruction but Status[MX] wasn・t set (most likely, the CPU doesn・t do MDMX)\n");
            break;
        case 23:
            printf("WATCH: Instruction or data reference matched a watchpoint\n");
            break;
        case 24:
            printf("MCheck: ：Machine check； - never happens in the 34K core.\n");
            break;
        case 25:
            printf("Thread: ：Thread-related exception, as described in [MIPSMT]; there・s a sub-cause field in VPEControl[EXCPT], as shown in Figure 2.1.\n");
            break;
        case 26:
            printf("DSP: ：Tried to run an instruction from the MIPS DSP ASE, but it・s not enabled (that is, Status[MX] is zero).\n");
            break;
        case 27:
        case 28:
        case 29:
            printf("-: Reserved\n");
            break;
        case 30:
            printf("CacheErr: Parity/ECC error somewhere in the core, on either instruction fetch, load or cache refill.\n");
            break;
        case 31:
            printf("-: Reserved\n");
            break;
        default:
            break;
    }

    printf("$zero: %08lX  $at: %08lX  $v0: %08lX  $v1: %08lX\n", regs->d[0],  regs->d[1],  regs->d[2],  regs->d[3]);
    printf("  $a0: %08lX  $a1: %08lX  $a2: %08lX  $a3: %08lX\n", regs->d[4],  regs->d[5],  regs->d[6],  regs->d[7]);
    printf("  $t0: %08lX  $t1: %08lX  $t2: %08lX  $t3: %08lX\n", regs->d[8],  regs->d[9],  regs->d[10], regs->d[11]);
    printf("  $t4: %08lX  $t5: %08lX  $t6: %08lX  $t7: %08lX\n", regs->d[12], regs->d[13], regs->d[14], regs->d[15]);
    printf("  $s0: %08lX  $s1: %08lX  $s2: %08lX  $s3: %08lX\n", regs->d[16], regs->d[17], regs->d[18], regs->d[19]);
    printf("  $s4: %08lX  $s5: %08lX  $s6: %08lX  $s7: %08lX\n", regs->d[20], regs->d[21], regs->d[22], regs->d[23]);
    printf("  $t8: %08lX  $t9: %08lX  $k0: %08lX  $k1: %08lX\n", regs->d[24], regs->d[25], regs->d[26], regs->d[27]);
    printf("  $gp: %08lX  $sp: %08lX  $fp: %08lX  $ra: %08lX\n", regs->d[28], regs->d[29], regs->d[30], regs->d[31]);

    printf("   sr: %08lX   pc: %08lX  cause : %08lX  badvr : %08lX\n\n", regs->sr, regs->pc, regs->cause, regs->badvr);

    ra = (unsigned long *)regs->d[31];
    sp = (unsigned long *)regs->d[29];

    printf("stack backtrace\n");
    printf("stack frame #0: ra = %08lx, sp = %08lx\n", (unsigned long)ra, (unsigned long)sp);

    extern U8  _readonly_start[];
    extern U8  _readonly_end[];

    if ( ((unsigned long)ra >= (unsigned long)_readonly_start) ||
         ((unsigned long)ra <= (unsigned long)_readonly_end) )
    {
        // Print out depth 256 call stack
        // Stop when *sp = 0xDEADBEEF (0xDEADBEEF is set in the head stack memory)
        for (depth = 0; (depth < 0xFF) && (*sp != 0xDEADBEEF); ++depth)
        {
            ra_offset = 0;
            stack_size = 0;

            for (addr = ra; !ra_offset || !stack_size; --addr)
            {
                switch(*addr & 0xffff0000)
                {
                    case 0x27bd0000: // 0x27bdxxxx: addiu sp, sp, xxxx
                        stack_size = ABS((short)(*addr & 0xffff));
                        break;
                    case 0xafbf0000: // 0xafbfxxxx: sw ra ,xxxx(sp)
                        ra_offset = (unsigned short)(*addr & 0xffff);
                        break;
                    default:
                        break;
                }
            }

            ra = *(unsigned long **)((unsigned long)sp + (unsigned long)ra_offset);
            sp = (unsigned long *)((unsigned long)sp + (unsigned long)stack_size);

            printf("stack frame #%d: ra = %08lx, sp = %08lx\n", depth+1, (unsigned long)ra, (unsigned long)sp);

            if ( ((unsigned long)ra < (unsigned long)_readonly_start) ||
                 ((unsigned long)ra > (unsigned long)_readonly_end) )
            {
                printf("!!wrong ra!!\n");
                break;
            }
        }
    }

    while (1);

}
