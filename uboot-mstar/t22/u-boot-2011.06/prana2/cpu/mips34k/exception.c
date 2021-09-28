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

typedef void (*ExceptionCb)(char *ps8str);

static struct
{
    mhal_isr_t  isr;
    U32         data;
} expt_table[E_EXCEPTION_MAX + 1];

void mhal_exception_attach(MHAL_EXCEPTION_TYPE expt_num, mhal_isr_t isr, U32 data)
{
    int old;

    //printf("mhal_exception_attach(expt_num=%u)\n", expt_num);

    if ((U32)expt_num <= E_EXCEPTION_MAX)
    {
        __mhal_interrupt_disable(old);
        expt_table[expt_num].isr = isr;
        expt_table[expt_num].data = data;
        __mhal_interrupt_restore(old);
    }
    else
    {
        //printf("E_EXCEPTION_MAX=%u\n", E_EXCEPTION_MAX);
    }
}

void mhal_exception_detach(MHAL_EXCEPTION_TYPE expt_num)
{
    if ((U32)expt_num <= E_EXCEPTION_MAX)
    {
        expt_table[expt_num].isr = 0;
    }
}

extern volatile ExceptionCb Exception_CB_Drawtext;

#define DEBUG_STACK_BACKTRACE(x)    //x

void mhal_exception_handler(MHAL_SavedRegisters *regs, int vector)
{
    unsigned long *addr;
    unsigned long *ra;
    unsigned long *sp;
    unsigned short depth;
    unsigned short ra_offset;
    unsigned short stack_size;
    char strbuf[256];
    unsigned int u32ExceptionNO = (vector & 0x0000007C) >> 2;
    mhal_isr_t isr = 0;


    printf("\nException: 0x%X, %u\n", (unsigned int)vector, (unsigned int)u32ExceptionNO);

    if ((U32)u32ExceptionNO <= E_EXCEPTION_MAX)
    {
        isr = expt_table[u32ExceptionNO].isr;
        if (isr)
        {
            isr(regs, expt_table[u32ExceptionNO].data);
            return;
        }
    }
    else
    {
        //printf("E_EXCEPTION_MAX=%u\n", E_EXCEPTION_MAX);
    }

    snprintf(strbuf, 255, "\nException: %u\n", u32ExceptionNO);
    printf("%s", strbuf);
    if ( Exception_CB_Drawtext != 0 )
    {
        Exception_CB_Drawtext(strbuf);
    }

    switch (u32ExceptionNO)
    {
        case 0:
            snprintf(strbuf, 255, "Int: Interrupt\n");
            break;
        case 1:
            snprintf(strbuf, 255, "Mod: Store, but page marked as read-only in the TLB\n");
            break;
        case 2:
            snprintf(strbuf, 255, "TLBL: Load or fetch, but page marked as invalid in the TLB\n");
            break;
        case 3:
            snprintf(strbuf, 255, "TLBS: Store, but page marked as invalid in the TLB\n");
            break;
        case 4:
            snprintf(strbuf, 255, "AdEL: Address error on load/fetch or store respectively. Address is either wrongly aligned, or a privilege violation\n");
            break;
        case 5:
            snprintf(strbuf, 255, "AdES: Address error on load/fetch or store respectively. Address is either wrongly aligned, or a privilege violation\n");
            break;
        case 6:
            snprintf(strbuf, 255, "IBE: Bus error signaled on instruction fetch\n");
            break;
        case 7:
            snprintf(strbuf, 255, "DBE: Bus error signaled on load/store (imprecise)\n");
            break;
        case 8:
            snprintf(strbuf, 255, "Sys: System call, ie syscall instruction executed.\n");
            break;
        case 9:
            snprintf(strbuf, 255, "Bp: Breakpoint, ie break instruction executed.\n");
            break;
        case 10:
            snprintf(strbuf, 255, "RI: Instruction code not recognized (or not legal)\n");
            break;
        case 11:
            snprintf(strbuf, 255, "CpU: Co-processor instruction encoding for co-processor which is not enabled in Status[CU3-0].\n");
            break;
        case 12:
            snprintf(strbuf, 255, "Ov: Overflow from trapping form of integer arithmetic instructions.\n");
            break;
        case 13:
            snprintf(strbuf, 255, "Tr: Condition met on one of the conditional trap instructions teq etc.\n");
            break;
        case 14:
            snprintf(strbuf, 255, "-: Reserved\n");
            break;
        case 15:
            snprintf(strbuf, 255, "FPE: Floating point unit exception - more details in FCSR.\n");
            break;
        case 16:
            snprintf(strbuf, 255, "-: Available for implementation dependent use\n");
            break;
        case 17:
            snprintf(strbuf, 255, "CeU: CorExtend instruction attempted when not enable by Status[CEE]\n");
            break;
        case 18:
            snprintf(strbuf, 255, "C2E: Reserved for precise Coprocessor 2 exceptions\n");
            break;
        case 19:
        case 20:
        case 21:
            snprintf(strbuf, 255, "-: Reserved\n");
            break;
        case 22:
            snprintf(strbuf, 255, "MDMX: Tried to run an MDMX instruction but Status[MX] wasnt set (most likely, the CPU doesnt do MDMX)\n");
            break;
        case 23:
            snprintf(strbuf, 255, "WATCH: Instruction or data reference matched a watchpoint\n");
            break;
        case 24:
            snprintf(strbuf, 255, "MCheck: Machine check - never happens in the 34K core.\n");
            break;
        case 25:
            snprintf(strbuf, 255, "Thread: Thread-related exception, as described in [MIPSMT]; theres a sub-cause field in VPEControl[EXCPT], as shown in Figure 2.1.\n");
            break;
        case 26:
            snprintf(strbuf, 255, "DSP: Tried to run an instruction from the MIPS DSP ASE, but its not enabled (that is, Status[MX] is zero).\n");
            break;
        case 27:
        case 28:
        case 29:
            snprintf(strbuf, 255, "-: Reserved\n");
            break;
        case 30:
            snprintf(strbuf, 255, "CacheErr: Parity/ECC error somewhere in the core, on either instruction fetch, load or cache refill.\n");
            break;
        case 31:
            snprintf(strbuf, 255, "-: Reserved\n");
            break;
        default:
            break;
    }
    printf("%s", strbuf);
    if ( Exception_CB_Drawtext != 0 )
    {
        Exception_CB_Drawtext(strbuf);
    }

    snprintf(strbuf, 255, "  $zero: %08lX  $at: %08lX  $v0: %08lX  $v1: %08lX\n", regs->d[0],  regs->d[1],  regs->d[2],  regs->d[3]);
    printf("%s", strbuf);
    if ( Exception_CB_Drawtext != 0 )
    {
        Exception_CB_Drawtext(strbuf);
    }
    snprintf(strbuf, 255, "  $a0: %08lX  $a1: %08lX  $a2: %08lX  $a3: %08lX\n", regs->d[4],  regs->d[5],  regs->d[6],  regs->d[7]);
    printf("%s", strbuf);
    if ( Exception_CB_Drawtext != 0 )
    {
        Exception_CB_Drawtext(strbuf);
    }
    snprintf(strbuf, 255, "  $t0: %08lX  $t1: %08lX  $t2: %08lX  $t3: %08lX\n", regs->d[8],  regs->d[9],  regs->d[10], regs->d[11]);
    printf("%s", strbuf);
    if ( Exception_CB_Drawtext != 0 )
    {
        Exception_CB_Drawtext(strbuf);
    }
    snprintf(strbuf, 255, "  $t4: %08lX  $t5: %08lX  $t6: %08lX  $t7: %08lX\n", regs->d[12], regs->d[13], regs->d[14], regs->d[15]);
    printf("%s", strbuf);
    if ( Exception_CB_Drawtext != 0 )
    {
        Exception_CB_Drawtext(strbuf);
    }
    snprintf(strbuf, 255, "  $s0: %08lX  $s1: %08lX  $s2: %08lX  $s3: %08lX\n", regs->d[16], regs->d[17], regs->d[18], regs->d[19]);
    printf("%s", strbuf);
    if ( Exception_CB_Drawtext != 0 )
    {
        Exception_CB_Drawtext(strbuf);
    }
    snprintf(strbuf, 255, "  $s4: %08lX  $s5: %08lX  $s6: %08lX  $s7: %08lX\n", regs->d[20], regs->d[21], regs->d[22], regs->d[23]);
    printf("%s", strbuf);
    if ( Exception_CB_Drawtext != 0 )
    {
        Exception_CB_Drawtext(strbuf);
    }
    snprintf(strbuf, 255, "  $t8: %08lX  $t9: %08lX  $k0: %08lX  $k1: %08lX\n", regs->d[24], regs->d[25], regs->d[26], regs->d[27]);
    printf("%s", strbuf);
    if ( Exception_CB_Drawtext != 0 )
    {
        Exception_CB_Drawtext(strbuf);
    }
    snprintf(strbuf, 255, "  $gp: %08lX  $sp: %08lX  $fp: %08lX  $ra: %08lX\n", regs->d[28], regs->d[29], regs->d[30], regs->d[31]);
    printf("%s", strbuf);
    if ( Exception_CB_Drawtext != 0 )
    {
        Exception_CB_Drawtext(strbuf);
    }

    snprintf(strbuf, 255, "   sr: %08lX   pc: %08lX  cause : %08lX  badvr : %08lX\n\n", regs->sr, regs->pc, regs->cause, regs->badvr);
    printf("%s", strbuf);
    if ( Exception_CB_Drawtext != 0 )
    {
        Exception_CB_Drawtext(strbuf);
    }

    ra = (unsigned long *)regs->d[31];
    sp = (unsigned long *)regs->d[29];

    snprintf(strbuf, 255, "stack backtrace\n");
    printf("%s", strbuf);
    if ( Exception_CB_Drawtext != 0 )
    {
        Exception_CB_Drawtext(strbuf);
    }

    snprintf(strbuf, 255, "stack frame #0: ra = %08lx, sp = %08lx\n", (unsigned long)ra, (unsigned long)sp);
    printf("%s", strbuf);
    if ( Exception_CB_Drawtext != 0 )
    {
        Exception_CB_Drawtext(strbuf);
    }

    extern U8  _readonly_start[];
    extern U8  _readonly_end[];

    if ( ((unsigned long)ra >= (unsigned long)_readonly_start) ||
         ((unsigned long)ra <= (unsigned long)_readonly_end) )
    {
        DEBUG_STACK_BACKTRACE( printf("Analyse ...\n"); );
        // Print out depth 256 call stack
        // Stop when *sp = 0xDEADBEEF (0xDEADBEEF is set in the head stack memory)
        for (depth = 0; (depth < 0xFF) && (*sp != 0xDEADBEEF); ++depth)
        {
            ra_offset = 0;
            stack_size = 0;

            DEBUG_STACK_BACKTRACE( printf("===== depth=%u\n", depth); );

            for (addr = ra; !ra_offset || !stack_size; --addr)
            {
                DEBUG_STACK_BACKTRACE( printf(" %08X : %X\n", (unsigned int)addr, (unsigned int)(*addr)); );
                switch(*addr & 0xffff0000)
                {
                    case 0x27bd0000: // 0x27bdxxxx: addiu sp, sp, xxxx
                        stack_size = ABS((short)(*addr & 0xffff));
                        DEBUG_STACK_BACKTRACE(printf("  stack_size=%u\n", stack_size););
                        break;
                    case 0xafbf0000: // 0xafbfxxxx: sw ra ,xxxx(sp)
                        ra_offset = (unsigned short)(*addr & 0xffff);
                        DEBUG_STACK_BACKTRACE(printf("  ra_offset=%u\n", ra_offset););
                        break;
                    default:
                        break;
                }
            }

            DEBUG_STACK_BACKTRACE( printf("  sp=%08X : *sp=%X\n", (unsigned int)sp, (unsigned int)(*sp)); );

            ra = *(unsigned long **)((unsigned long)sp + (unsigned long)ra_offset);
            sp = (unsigned long *)((unsigned long)sp + (unsigned long)stack_size);

            snprintf(strbuf, 255, "stack frame #%d: ra = %08lx, sp = %08lx\n", depth+1, (unsigned long)ra, (unsigned long)sp);
            printf("%s", strbuf);
            if ( Exception_CB_Drawtext != 0 )
            {
                Exception_CB_Drawtext(strbuf);
            }

            if ( ((unsigned long)ra < (unsigned long)_readonly_start) ||
                 ((unsigned long)ra > (unsigned long)_readonly_end) )
            {
                snprintf(strbuf, 255, "!!wrong ra!!\n");
                printf("%s", strbuf);
                if ( Exception_CB_Drawtext != 0 )
                {
                    Exception_CB_Drawtext(strbuf);
                }
                break;
            }
        }
    }

    while (1);

}
