#ifndef _ARM_INC
#define _ARM_INC

#include "assembler.h"
#include "pgtable.h"

##=============================================================================
##
##    arm.inc
##
##    ARM assembler header file
##
##=============================================================================

#ifndef ENTRY
#define ENTRY(name) \
  .globl name; \
  name:
#endif
#endif /* LINKER_SCRIPT */

#ifndef END
#define END(name) \
  .size name,.-name
#endif

#ifndef ENDPROC
#define ENDPROC(name) \
    .type name, %function; \
    END(name)  
#endif

/*
 * Address
 */
#define MIU0_VA_ADDR            0x00000000
#define MIU0_PA_ARRD            MIU0_BUS_BASE

/*
 * Stack frame.
 */
#define S_FRAME_SIZE            272
#define S_PSTATE                264
#define S_PC                    256
#define S_SP                    248
#define S_LR                    240
#define S_X29                   232
#define S_X28                   224
#define S_X27                   216
#define S_X26                   208
#define S_X25                   200
#define S_X24                   192
#define S_X23                   184
#define S_X22                   176
#define S_X21                   168
#define S_X20                   160
#define S_X19                   152
#define S_X18                   144
#define S_X17                   136
#define S_X16                   128
#define S_X15                   120
#define S_X14                   112
#define S_X13                   104
#define S_X12                   96
#define S_X11                   88
#define S_X10                   80
#define S_X9                    72
#define S_X8                    64
#define S_X7                    56
#define S_X6                    48
#define S_X5                    40
#define S_X4                    32
#define S_X3                    24
#define S_X2                    16
#define S_X1                    8
#define S_X0                    0

/*
 * GIC-400 Register Memory Map
 */
#define timer_pa                0x16000600
#define dic_pa                  0x16001000
#define cpu_if_pa               0x16002000
#define cpu_if_va               0x16002000


#-------------------------------------------------------------------------------------------------
# Macros
#-------------------------------------------------------------------------------------------------

/*
 * exception entry
 */
.macro	exception_entry
    sub	        sp, sp, #S_FRAME_SIZE - S_LR	// room for LR, SP, SPSR, ELR
    
    push	x28, x29
    push	x26, x27
    push	x24, x25
    push	x22, x23
    push	x20, x21
    push	x18, x19
    push	x16, x17
    push	x14, x15
    push	x12, x13
    push	x10, x11
    push	x8, x9
    push	x6, x7
    push	x4, x5
    push	x2, x3
    push	x0, x1

    add         x21, sp, #S_FRAME_SIZE

    mrs         x22, ELR_EL1
    mrs	        x23, SPSR_EL1
    stp	        x30, x21, [sp, #S_LR]
    stp	        x22, x23, [sp, #S_PC]
	
    /*
     * Registers that may be useful after this macro is invoked:
     *
     * x21 - aborted SP
     * x22 - aborted PC
     * x23 - aborted PSTATE
    */
.endm

/*
 * exception exit
 */
.macro	exception_exit
    ldp	        x21, x22, [sp, #S_PC]		// load ELR, SPSR
    pop         x0, x1
    pop         x2, x3				// load the rest of the registers
    pop        	x4, x5
    pop        	x6, x7
    pop        	x8, x9
    msr        	ELR_EL1, x21			// set up the return data
    msr        	SPSR_EL1, x22
    pop        	x10, x11
    pop        	x12, x13
    pop        	x14, x15
    pop        	x16, x17
    pop        	x18, x19
    pop        	x20, x21
    pop        	x22, x23
    pop        	x24, x25
    pop        	x26, x27
    pop        	x28, x29
    ldr        	x30, [sp], #S_FRAME_SIZE - S_LR	// load LR and restore SP
    
    eret					// return to kernel
.endm

/*
 * Disable mmu icache dcache
 */
.macro disable_mmu_dcache_icache
    mrs x0, SCTLR_EL1                // System Control Register (EL1)
    mov x1, xzr
    orr x1, x1, #(0x1 << 12)         // Disable instruction cache
    orr x1, x1, #(0x1 << 2)          // Disable data cache
    orr x1, x1, #(0x1 << 0)          // Disable MMU
    bic x0, x0, x1
    msr SCTLR_EL1, x0
    isb
.endm

/*
 * Macro to populate the PGD for the corresponding block entry in the next
 * level (tbl) for the given virtual address.
 *
 * Preserves:	pgd, tbl, virt
 * Corrupts:	tmp1, tmp2
 */
.macro	create_pgd_entry, pgd, tbl, virt, tmp1, tmp2
    lsr	\tmp1, \virt, #PGDIR_SHIFT
    and	\tmp1, \tmp1, #PTRS_PER_PGD - 1	// PGD index
    orr	\tmp2, \tbl, #3			// PGD entry table type
    str	\tmp2, [\pgd, \tmp1, lsl #3]
.endm

/*
 * Macro to populate block entries in the page table for the start..end
 * virtual range (inclusive).
 *
 * Preserves:	tbl, flags
 * Corrupts:	phys, start, end, pstate
 */
.macro	create_block_map, tbl, flags, phys, start, end
    lsr	\phys, \phys, #BLOCK_SHIFT
    lsr	\start, \start, #BLOCK_SHIFT
    and	\start, \start, #PTRS_PER_PTE - 1       // table index
    orr	\phys, \flags, \phys, lsl #BLOCK_SHIFT  // table entry
    .ifnc \start,\end
    lsr \end, \end, #BLOCK_SHIFT
    and \end, \end, #PTRS_PER_PTE - 1       // table end index
    .endif
99: str	\phys, [\tbl, \start, lsl #3]           // store the entry
    .ifnc \start,\end
    add \start, \start, #1                  // next entry
    add \phys, \phys, #BLOCK_SIZE           // next block
    cmp \start, \end
    b.ls 99b
    .endif
.endm

# end of arm.inc
