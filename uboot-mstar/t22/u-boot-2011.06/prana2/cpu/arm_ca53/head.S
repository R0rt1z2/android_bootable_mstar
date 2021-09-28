#include <arm.inc>
#include <ptrace.h>

ENTRY(vectors)
    ventry	el1_sync_invalid		// Synchronous EL1t
    ventry	el1_irq_invalid			// IRQ EL1t
    ventry	el1_fiq_invalid			// FIQ EL1t
    ventry	el1_error_invalid		// Error EL1t

    ventry	el1_sync			// Synchronous el1h
    ventry	el1_irq				// IRQ EL1h
    ventry	el1_fiq_invalid			// FIQ EL1h
    ventry	el1_error_invalid		// Error EL1h

    ventry	el0_sync			// Synchronous 64-bit EL0
    ventry	el0_irq				// IRQ 64-bit EL0
    ventry	el0_fiq_invalid			// FIQ 64-bit EL0
    ventry	el0_error_invalid		// Error 64-bit EL0

    ventry	el0_sync_invalid		// Synchronous 32-bit EL0
    ventry	el0_irq_invalid			// IRQ 32-bit EL0
    ventry	el0_fiq_invalid			// FIQ 32-bit EL0
    ventry	el0_error_invalid		// Error 32-bit EL0
END(vectors)

.globl _ttb1_base_addr
_ttb1_base_addr:
      .quad _ld_ttb1_base_addr

.globl _ttb0_base_addr
_ttb0_base_addr:
      .quad  _ld_ttb0_base_addr

.extern         start
.globl          reset
.align          9
reset:
    /* enter el1 bypassing el2 */
    bl         el3_setup
    bl         el2_setup
    
    /* disable mmu dcache */
    disable_mmu_dcache_icache
    
    /* init GIC */
    bl          __init_dic
    bl          __init_interrupts
    
    /* set up vector base on EL1 */
    ldr         x0, =_ld_ram_vector_start
    msr         VBAR_EL1, x0

    /* create page table */
    bl          __create_page_tables
    
    /* enable mmu */
    bl          __enable_mmu

    /* set mode stack pointer on EL1 */
    ldr         x0, =_ld_stack_el1_start
    mov         sp, x0

    /* clear bss */
    ldr         x6, =_ld_bss_start
    ldr         x7, =__heap_end
1:
    cmp         x6, x7
    str         wzr, [x6], #4
    b.lo        1b
    
    ldr         x0, =start                  // address to jump to after

    /* jump to C entry */
    br          x0

/*************************************************************************
 * Setup the initial page tables. We only setup the barest amount which is
 * required to get the kernel running. The following sections are required:
 *   - identity mapping to enable the MMU (low address, TTBR0)
 *   - first few MB of the kernel linear mapping to jump to once the MMU has
 *     been enabled, including the FDT blob (TTBR1)
 *   - UART mapping if CONFIG_EARLY_PRINTK is enabled (TTBR1)
 *************************************************************************/
ENTRY(__create_page_tables)
    ldr         x26, =_ld_ttb1_base_addr                          // get level 2 traslation table base
    ldr         x25, =_ld_ttb0_base_addr                          // get level 3 traslation table base

    /*
     * Clear the idmap and swapper page tables.
     */
    mov         x0, x25
    add	        x6, x26, #TTB1_SIZE
1:  stp	        xzr, xzr, [x0], #16
    stp	        xzr, xzr, [x0], #16
    stp	        xzr, xzr, [x0], #16
    stp	        xzr, xzr, [x0], #16
    cmp	        x0, x6
    b.lo        1b


    /*
     * Map the AP bin (starting with PHYS_OFFSET).
     * VA=0x20000000 PA=0x20000000 length=0x1000000
     */
    ldr	        x7, =NORM_MMUFLAGS
    ldr         x26, =_ld_ttb1_base_addr                      // get level 2 traslation table base
    ldr         x25, =(_ld_ttb0_base_addr + TTB0_SIZE)        // get level 3 traslation table base
    ldr	        x5, =0x20000000
    create_pgd_entry x26, x25, x5, x3, x6
    ldr	        x6, =0x21000000 - 1
    ldr	        x3, =0x20000000                               // phys offset
    create_block_map x25, x7, x3, x5, x6
    
    /*
     * Map the Periphel register mmap.
     * VA=0x16000000 PA=0x16000000 length=0xa000000
     */
    ldr	        x7, =DEV_MMUFLAGS
    ldr         x26, =_ld_ttb1_base_addr                      // get level 2 traslation table base
    ldr         x25, =_ld_ttb0_base_addr                      // get level 3 traslation table base
    ldr	        x5, =0x16000000
    create_pgd_entry x26, x25, x5, x3, x6
    ldr	        x6, =0x20000000 - 1
    ldr	        x3, =0x16000000                               // phys offset
    create_block_map x25, x7, x3, x5, x6

    msr     ttbr0_el1, x26                                    // load TTBR0
    isb
    ret
ENDPROC(__create_page_tables)

/**************************************************************************
 *Initialise the processor for turning the MMU on.
 *************************************************************************/
ENTRY(__enable_mmu)
    /*
     * Preserve the link register across the function call.
     */
    mov         x28, lr
    bl	        __flush_dcache_all
    mov         lr, x28
    ic	        IALLU				// I+BTB cache invalidate
    dsb	        sy
    
    mov	        x0, #3 << 20
    msr	        CPACR_el1, x0			// Enable FP/ASIMD
    msr	        MDSCR_el1, xzr			// Reset mdscr_el1
    tlbi        VMALLE1IS			// invalidate I + D TLBs
	
    /*
     * Memory region attributes for LPAE:
     *
     *   n = AttrIndx[2:0]
     *			n	MAIR
     *   DEVICE_nGnRnE	000	00000000
     *   DEVICE_nGnRE	001	00000100
     *   DEVICE_GRE	010	00001100
     *   NORMAL_NC	011	01000100
     *   NORMAL		100	11111111
     */
    ldr         x5, =MAIR(0x00, MT_DEVICE_nGnRnE) | \
		     MAIR(0x04, MT_DEVICE_nGnRE) | \
		     MAIR(0x0c, MT_DEVICE_GRE) | \
		     MAIR(0x44, MT_NORMAL_NC) | \
		     MAIR(0xff, MT_NORMAL)
    msr	        MAIR_EL1, x5
    
    /*
     * Prepare SCTLR
     */
    adr	        x5, crval
    ldp	        w5, w6, [x5]
    mrs	        x0, SCTLR_EL1
    bic	        x0, x0, x5			// clear bits
    orr	        x0, x0, x6			// set bits
    
    /*
     * Set/prepare TCR and TTBR. We use 512GB (39-bit) address range for
     * both user and kernel.
     */
    ldr	        x10, =TCR_TxSZ(VA_BITS) | TCR_FLAGS | TCR_IPS_40BIT | \
    	              TCR_ASID16 | (1 << 31)
    
    orr	        x10, x10, TCR_TG0_64K
    orr	        x10, x10, TCR_TG1_64K
    msr	        TCR_EL1, x10
    
    msr         SCTLR_EL1, x0
    isb
    ret

/*
 *                 n n            T
 *       U E      WT T UD     US IHBS
 *       CE0      XWHW CZ     ME TEEA S
 * .... .IEE .... NEAI TE.I ..AD DEN0 ACAM
 * 0011 0... 1101 ..0. ..0. 10.. .... .... < hardware reserved
 * .... .100 .... 01.1 11.1 ..01 0001 1101 < software settings
 */
	.type	crval, #object
crval:
	.word	0x030802e2			// clear
	.word	0x0405d11d			// set

ENDPROC(__enable_mmu)

/*************************************************************************
 * If we're fortunate enough to boot at EL3, ensure that the world is
 * sane before dropping to EL2.
 *************************************************************************/
ENTRY(el3_setup)
    mrs	        x0, CurrentEL
    cmp	        x0, #PSR_MODE_EL3t
    ccmp	x0, #PSR_MODE_EL3h, #0x4, ne
    b.eq	1f
    ret

1:  msr         ELR_EL3, lr
    msr         SCTLR_EL2, XZR
    mov         x0, #0x3c9
    msr         SPSR_EL3, x0
    mov         x0, #0x401
    msr         SCR_EL3, x0
    eret
ENDPROC(el3_setup)

/*************************************************************************
 * If we're fortunate enough to boot at EL2, ensure that the world is
 * sane before dropping to EL1.
 *************************************************************************/
ENTRY(el2_setup)
    mrs	        x0, CurrentEL
    cmp	        x0, #PSR_MODE_EL2t
    ccmp	x0, #PSR_MODE_EL2h, #0x4, ne
    b.eq	1f
    ret

    /* Hyp configuration. */
1:  ldr	        w1, =BOOT_CPU_MODE_EL2
    str	        w1, [x0, #4]			// This CPU has EL2
    mov	        x0, #(1 << 31)			// 64-bit EL1
    msr	        HCR_EL2, x0

    /* Generic timers. */
    mrs	        x0, CNTHCTL_EL2
    orr	        x0, x0, #3			// Enable EL1 physical timers
    msr	        CNTHCTL_EL2, x0
    msr	        CNTVOFF_EL2, xzr		// Clear virtual offset
    
    /* Populate ID registers. */
    mrs	        x0, MIDR_EL1
    mrs         x1, MPIDR_EL1
    msr        	VPIDR_EL2, x0
    msr        	VMPIDR_EL2, x1
    
    /* sctlr_el1 */
    mov	        x0, #0x0800			// Set/clear RES{1,0} bits
    movk        x0, #0x30d0, lsl #16
    msr	        SCTLR_EL1, x0
    
    /* Coprocessor traps. */
    mov	        x0, #0x33ff
    msr	        CPTR_EL2, x0			// Disable copro. traps to EL2
    
    /* Stage-2 translation */
    msr	       VTTBR_EL2, xzr
    
    /* spsr */
    mov	       x0, #(PSR_F_BIT | PSR_I_BIT | PSR_A_BIT | PSR_D_BIT |\
    	             PSR_MODE_EL1h)
    msr	       SPSR_EL2, x0
    msr	       ELR_EL2, lr
    eret
ENDPROC(el2_setup)

/*************************************************************************
 * Init Distributed Interrupt Controler
 *************************************************************************/
ENTRY(__init_dic)
    # disable interrupts in DIC
    ldr         x0, =dic_pa
    ldr         x1, =0x0                    // DICControl offset
    str         wzr, [x0, x1]

    # disable all interrupt sources ID0-ID63
    ldr         w1, =0x180                  // Interrupt Clear-enable Register ID0-ID31 offset
    ldr         w2, =0xFFFFFFFF             // Disable All Interrupts. Does not affect SW interrupts (ID0-ID15)
    str         w2, [x0, x1]
    ldr         w1, =0x184                  // Interrupt Clear-enable Register ID32-ID63 offset
    str         w2, [x0, x1]

    # clear all pending interrupts
    ldr         w1, =0x280                  // Interrupt Clear-pending Register ID0-ID31 offset
    ldr         w2, =0xFFFFFFFF             // Clear All Pending Interrupts
    str         w2, [x0, x1]
    ldr         w1, =0x284                  // Interrupt Clear-pending Register ID32-ID63 offset
    str         w2, [x0, x1]

    # enable interrupts in DIC
    ldr         x0, =dic_pa
    ldr         x1, =0x0                    // DICControl offset
    ldr         w2, [x0, x1]
    orr         w2, w2, #0x1
    str         w2, [x0, x1]
    ret
ENDPROC(__init_dic)

/*************************************************************************
 * Init CPU Interrupt Interface
 *************************************************************************/
ENTRY(__init_interrupts)
    # disable interrupts in CPU Interface
    ldr         x0, =cpu_if_pa
    ldr         x1, =0x0                    // CPUControl offset
    str         wzr, [x0, x1]

    # enable interrupt sources in DIC we care about
    # ID29 = Timer counter wrap interrupt
    ldr         x0, =dic_pa
    ldr         x1, =0x100                  // Interrupt Set-enable Register ID0-ID31 offset
    ldr         w2, [x0, x1]
    orr         w2, w2, #1 << 27            // Enable virtual timer event with ID 27
    orr         w2, w2, #1 << 31            // Enable Legacy IRQ with ID 31
    str         w2, [x0, x1]
    
    # set ID27 interrupt priority to highest
    ldr         x1, =0x418                  // Priority level Register ID26-ID27 offset
    ldr         w2, [x0, x1]
    bic         w2, w2, #0xFF000000         // Set ID27 priority to 0 (highest priority)
    str         w2, [x0, x1]

    # set priority mask to 1 (allowing interrupts with priority of 0 through)
    # i.e. ID27 virtual timer event
    ldr         x0, =cpu_if_pa
    ldr         x1, =0x4                    // Priority Mask Register offset
    ldr         w2, =1 << 3
    str         w2, [x0, x1]

    # enable interrupts in CPU interface
    ldr         x1, =0x0                    // CPUControl offset
    ldr         w2, [x0, x1]
    orr         w2, w2, #1
    str         w2, [x0, x1]
    ret
ENDPROC(__init_interrupts)

/*************************************************************************
 * Exception handlers
 *************************************************************************/
el1_sync_invalid:
    exception_entry                          //Push general purpose register
    # Dump registers
    mov         x0, sp
    bl          do_el1_sync_invalid
el1_sync_invalid_loop:
    b           el1_sync_invalid_loop

el1_irq_invalid:
    exception_entry                          //Push general purpose register
    # Dump registers
    mov         x0, sp
    bl          do_el1_irq_invalid
el1_irq_invalid_loop:
    b           el1_irq_invalid_loop

el1_fiq_invalid:
    exception_entry                          //Push general purpose register
    # Dump registers
    mov         x0, sp
    bl          do_el1_fiq_invalid
el1_fiq_invalid_loop:
    b           el1_fiq_invalid_loop

el1_error_invalid:
    exception_entry                          //Push general purpose register
    # Dump registers
    mov         x0, sp
    bl          do_el1_error_invalid
el1_error_invalid_loop:
    b           el1_error_invalid_loop

el1_sync:
    exception_entry                          //Push general purpose register
    # Dump registers
    mov         x0, sp
    bl          do_el1_sync
el1_sync_loop:
    b           el1_sync_loop

el1_irq:
    exception_entry                          //Push general purpose register
    # Acknowledge Interrupt
    ldr         x0, =cpu_if_va
    ldr         x1, =0xc                     // Interrupt Acknowledge Register offset
    ldr         w2, [x0, x1]
    ldr         w3, =0x3F                    // ACK_INTID mask
    and         w4, w2, w3                   // INTID - we need this at irq_handler_end
    push	x4, x3                       // Push w4 to backup interrupt ID
    cmp         w4, #27                      // Physical timer ID
    b.eq        timer 
ppi:
    bl          do_irq
    b           irq_handler_end
timer:
    bl          timer_interrupt_handler
irq_handler_end:
    # Signal End of Interrupt
    pop        	x4, x5                       // Pop w4 to restore interrupt ID
    ldr         x0, =cpu_if_va
    ldr         x1, =0x10                    // End Of Interrupt Register offset
    str         w4, [x0, x1]
    exception_exit                           // Pop general purpose register and return

el0_sync:
    exception_entry                          //Push general purpose register
    # Dump registers
    mov         x0, sp
    bl          do_el0_sync
el0_sync_loop:
    b           el0_sync_loop

el0_irq:
    exception_entry                          //Push general purpose register
    # Dump registers
    mov         x0, sp
    bl          do_el0_irq
el0_irq_loop:
    b           el0_irq_loop

el0_fiq_invalid:
    exception_entry                          //Push general purpose register
    # Dump registers
    mov         x0, sp
    bl          do_el0_fiq_invalid
el0_fiq_invalid_loop:
    b           el0_fiq_invalid_loop

el0_error_invalid:
    exception_entry                          //Push general purpose register
    # Dump registers
    mov         x0, sp
    bl          do_el0_error_invalid
el0_error_invalid_loop:
    b           el0_error_invalid_loop

el0_sync_invalid:
    exception_entry                          //Push general purpose register
    # Dump registers
    mov         x0, sp
    bl          do_el0_sync_invalid
el0_sync_invalid_loop:
    b           el0_sync_invalid_loop

el0_irq_invalid:
    exception_entry                          //Push general purpose register
    # Dump registers
    mov         x0, sp
    bl          do_el0_irq_invalid
el0_irq_invalid_loop:
    b           el0_irq_invalid_loop

