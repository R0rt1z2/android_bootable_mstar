#ifndef _MIPS_INC
#define _MIPS_INC

##=============================================================================
##
##    mips.inc
##
##    MIPS assembler header file
##
##=============================================================================
#####ECOSGPLCOPYRIGHTBEGIN####
## -------------------------------------------
## This file is part of eCos, the Embedded Configurable Operating System.
## Copyright (C) 1998, 1999, 2000, 2001, 2002 Red Hat, Inc.
##
## eCos is free software; you can redistribute it and/or modify it under
## the terms of the GNU General Public License as published by the Free
## Software Foundation; either version 2 or (at your option) any later version.
##
## eCos is distributed in the hope that it will be useful, but WITHOUT ANY
## WARRANTY; without even the implied warranty of MERCHANTABILITY or
## FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
## for more details.
##
## You should have received a copy of the GNU General Public License along
## with eCos; if not, write to the Free Software Foundation, Inc.,
## 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
##
## As a special exception, if other files instantiate templates or use macros
## or inline functions from this file, or you compile this file and link it
## with other works to produce a work based on this file, this file does not
## by itself cause the resulting work to be covered by the GNU General Public
## License. However the source code for this file must still be made available
## in accordance with section (3) of the GNU General Public License.
##
## This exception does not invalidate any other reasons why a work based on
## this file might be covered by the GNU General Public License.
##
## Alternative licenses for eCos may be arranged by contacting Red Hat, Inc.
## at http://sources.redhat.com/ecos/ecos-license/
## -------------------------------------------
#####ECOSGPLCOPYRIGHTEND####
##=============================================================================
#######DESCRIPTIONBEGIN####
##
## Author(s):       nickg
## Contributors:    nickg
## Date:            1997-10-16
## Purpose:         MIPS definitions.
## Description:     This file contains various definitions and macros that are
##                  useful for writing assembly code for the MIPS CPU family.
## Usage:
##        #include <cyg/hal/mips.inc>
##        ...
##
##
######DESCRIPTIONEND####
##
##=============================================================================

##-----------------------------------------------------------------------------
## Standard MIPS register names:

#define MIPS_REG_0      $0
#define MIPS_REG_2      $2
#define MIPS_REG_3      $3
#define MIPS_REG_4      $4
#define MIPS_REG_5      $5
#define MIPS_REG_6      $6
#define MIPS_REG_7      $7
#define MIPS_REG_8      $8
#define MIPS_REG_9      $9
#define MIPS_REG_10      $10
#define MIPS_REG_11      $11
#define MIPS_REG_12      $12
#define MIPS_REG_13      $13
#define MIPS_REG_14      $14
#define MIPS_REG_15      $15
#define MIPS_REG_16      $16
#define MIPS_REG_17      $17
#define MIPS_REG_18      $18
#define MIPS_REG_19      $19
#define MIPS_REG_20      $20
#define MIPS_REG_21      $21
#define MIPS_REG_22      $22
#define MIPS_REG_23      $23
#define MIPS_REG_24      $24
#define MIPS_REG_25      $25
#define MIPS_REG_26      $26     /* private register 0 for kernel */
#define MIPS_REG_27      $27     /* private register 1 for kernel */
#define MIPS_REG_28      $28     /* data pointer */
#define MIPS_REG_29      $29     /* sp */
#define MIPS_REG_30      $30     /* fp */
#define MIPS_REG_31      $31     /* ra */
#define MIPS_REG_pc      $pc     /* pc */



#define zero    MIPS_REG_0
#define z0      MIPS_REG_0
#define v0      MIPS_REG_2
#define v1      MIPS_REG_3
#define a0      MIPS_REG_4
#define a1      MIPS_REG_5
#define a2      MIPS_REG_6
#define a3      MIPS_REG_7
#define t0      MIPS_REG_8
#define t1      MIPS_REG_9
#define t2      MIPS_REG_10
#define t3      MIPS_REG_11
#define t4      MIPS_REG_12
#define t5      MIPS_REG_13
#define t6      MIPS_REG_14
#define t7      MIPS_REG_15
#define s0      MIPS_REG_16
#define s1      MIPS_REG_17
#define s2      MIPS_REG_18
#define s3      MIPS_REG_19
#define s4      MIPS_REG_20
#define s5      MIPS_REG_21
#define s6      MIPS_REG_22
#define s7      MIPS_REG_23
#define t8      MIPS_REG_24
#define t9      MIPS_REG_25
#define k0      MIPS_REG_26     
#define k1      MIPS_REG_27    
#define gp      MIPS_REG_28     
#define sp      MIPS_REG_29     
#define fp      MIPS_REG_30    
#define ra      MIPS_REG_31    
#define pc      MIPS_REG_pc  

#define f0      $f0
#define f1      $f1
#define f2      $f2
#define f3      $f3
#define f4      $f4
#define f5      $f5
#define f6      $f6
#define f7      $f7
#define f8      $f8
#define f9      $f9
#define f10     $f10
#define f11     $f11
#define f12     $f12
#define f13     $f13
#define f14     $f14
#define f15     $f15
#define f16     $f16
#define f17     $f17
#define f18     $f18
#define f19     $f19
#define f20     $f20
#define f21     $f21
#define f22     $f22
#define f23     $f23
#define f24     $f24
#define f25     $f25
#define f26     $f26
#define f27     $f27
#define f28     $f28
#define f29     $f29
#define f30     $f30
#define f31     $f31

// Coprocessor 0 registers

#define index       $0      // TLB entry index register
#define random      $1      // TLB random index register
#define tlblo0      $2      // TLB even page entry register
#define tlblo1      $3      // TLB odd page entry register
#define config      $3      // Configuration register (TX39 only)
#define context     $4      // TLB context register
#define pagemask    $5      // TLB page size mask
#define wired       $6      // TLB wired boundary
#define cachectrl   $7      // Cache control
#define badvr       $8      // Bad virtual address
#define count       $9      // Timer cycle count register
#define tlbhi       $10     // TLB virtual address match register
#define compare     $11     // Timer compare register
#define status      $12     // Status register
#define cause       $13     // Exception cause
#define epc         $14     // Exception pc value
#define prid        $15     // processor ID
#define config0     $16     // Config register 0
#define lladdr      $17     // LLAdddr
#define xcontext    $20     // XContext register
#define debug       $23     // Debug register
#define ecc         $26     // Error Checking and Correcting
#define cache_err   $27     // Cache Error and Status
#define taglo       $28     // TagLo
#define taghi       $29     // TagHi
#define error_epc   $30     // Error exception pc value

#define mips_regsize    4
#define mips_regsize32  4

#define mipsreg_regs    0
#define mipsreg_hi      (mips_regsize*32)
#if 1
#define mipsreg_lo      (mipsreg_hi+1*mips_regsize)
#define mipsreg_sr      (mipsreg_hi+2*mips_regsize)
#define mipsreg_vector  (mipsreg_hi+3*mips_regsize)
#define mipsreg_pc      (mipsreg_hi+4*mips_regsize)
#define mipsreg_cause   (mipsreg_hi+5*mips_regsize)
#define mipsreg_badvr   (mipsreg_hi+6*mips_regsize)
#else
#define mipsreg_lo      (mipsreg_hi+mips_regsize)
#define mipsreg_sr      (mipsreg_lo+mips_regsize)
#define mipsreg_vector  (mipsreg_sr+mips_regsize32)
#define mipsreg_pc      (mipsreg_vector+mips_regsize32)
#define mipsreg_cause   (mipsreg_pc+mips_regsize)
#define mipsreg_badvr   (mipsreg_cause+mips_regsize32)
#endif

# The variant HAL should specify which additional registers it needs,
# but older HALs don't do this.

#ifndef HAL_MIPS_VAR_MIPSREG_DEFINED
#define mipsreg_cachectrl   (mipsreg_badvr+mips_regsize)
#define mipsreg_prid        (mipsreg_cachectrl+mips_regsize32)
#define mipsreg_config      (mipsreg_prid+mips_regsize32)
#define mipsreg_size        (mipsreg_config+mips_regsize32)
#else
#define mipsreg_size        (mipsreg_badvr+mips_regsize32+mipsreg_var_size)
#endif

# The following expression ensures that the decrement is always a
# multiple of 16 bytes. This is a requirement of the MEABI used in
# MIPS32/64 targets.

#define mips_exception_decrement    ((mipsreg_size*2)&~0xF)

##-----------------------------------------------------------------------------
## Minimal stack frame size uses to call functions from asm.

#define mips_stack_frame_size       32    // 4 (64 bit) args worth

#------------------------------------------------------------------------------
#define FUNC_START(name)        \
        .type name,@function;   \
        .set push ;             \
        .globl name;            \
        .ent   name;            \
        .set noreorder ;        \
name:

#define FUNC_END(name)          \
name##_end:                     \
        .set pop;               \
        .end name               \

.macro sgpr reg,ptr
sw      $\reg,(mipsreg_regs+\reg*mips_regsize)(\ptr)
.endm

.macro lgpr reg,ptr
lw     $\reg,(mipsreg_regs+\reg*mips_regsize)(\ptr)
.endm

.macro  hal_var_save_interrupt regs
mfc0    t3,cachectrl
sw      t3,mipsreg_cachectrl(\regs)
.endm

.macro  hal_var_restore    regs
lw      t0,mipsreg_cachectrl(sp)
mtc0    t0,cachectrl
.endm

.macro ssp reg,ptr
sw     \reg,(mipsreg_regs+29*mips_regsize)(\ptr)
.endm

.macro lsp reg,ptr
lw     \reg,(mipsreg_regs+29*mips_regsize)(\ptr)
.endm

.macro llo reg,ptr
lw     \reg,(mipsreg_lo)(\ptr)
.endm

.macro lhi reg,ptr
lw     \reg,(mipsreg_hi)(\ptr)
.endm

.macro slo reg,ptr
sw     \reg,(mipsreg_lo)(\ptr)
.endm

.macro shi reg,ptr
sw     \reg,(mipsreg_hi)(\ptr)
.endm

.macro hal_cpu_eret pc,sr
    .set mips3
ori     \sr,\sr,2           # prevent interrupts until eret
mtc0	\sr,status	        # put SR back
nop
nop
nop
mtc0    \pc,epc	            # put PC in EPC
nop
nop
nop
sync	                    # settle things down
eret	                    # return
nop	                    # just to be safe
    .set mips0
.endm

#------------------------------------------------------------------------------
#endif // ifndef CYGONCE_HAL_MIPS_INC
# end of mips.inc

