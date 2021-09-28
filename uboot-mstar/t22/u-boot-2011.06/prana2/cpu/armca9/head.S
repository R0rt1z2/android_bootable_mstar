#include "arm.inc"

#if defined(CHAKRA)
.macro write_vbar reg
mcr p15, 0, \reg, c12, c0, 0
.endm
#endif

.extern         start

.globl          reset
reset:

    disable_mmu_dcache
    invaliate_dcache_tlb

#if defined(CHAKRA)
    /* Set vector (VBAR) */
    ldr r0, =exception_vector
    write_vbar r0

    @//disable dcache
    mrc         p15, 0, r0, c1, c0, 0       @ read control reg
    bic r0, r0, #0x0001
    mcr         p15, 0, r0, c1, c0, 0       @ write control reg
#endif

    @ init GIC
    bl          __init_dic
    bl          __init_interrupts

    @ create page table
    bl          __create_page_tables

    @ Enable NEON and VFP
@   bl          __init_neon_vfp

    @ enable mmu
    mov         r5, #(domain_val(DOMAIN_USER, DOMAIN_MANAGER) | \
                      domain_val(DOMAIN_AP, DOMAIN_MANAGER) | \
                      domain_val(DOMAIN_BOOT, DOMAIN_MANAGER))
    mcr         p15, 0, r5, c3, c0, 0       @ load domain access register
    mcr         p15, 0, r4, c2, c0, 0       @ load page table pointer

#if defined(CHAKRA)
    bl turn_on_mmu
#endif

#ifdef ENABLE_L1_DCACHE
    bl dcache_init
#endif

    @ set SVC mode SP
    set_svc32_mode
    ldr         sp, =_ld_svc_stack_start

    @ set IRQ mode SP
    set_irq_mode                            @ change to IRQ mode
    ldr         sp, =_ld_irq_stack_start    @ set IRQ_SP
    set_svc32_mode                          @ change back to SVC mode

    @ set FIQ mode SP
    set_fiq_mode                            @ change to FIQ mode
    ldr         sp, =_ld_fiq_stack_start    @ set FIQ_SP
    set_svc32_mode                          @ change back to SVC mode

    @ clear bss
    ldr         r6, =_ld_bss_start
    ldr         r7, =__heap_end
    mov         fp, #0
1:
    cmp         r6,r7
    strcc       fp, [r6], #4
    bcc         1b

    ldr         r3, =start                  @ address to jump to after

    @ jump to C entry
    mov         r3, r3
    mov         pc, r3

/*
 * Enable NEON and VFP
 */

__init_neon_vfp:
    mrc         p15, 0, r0, c1, c0, 2
    orr         r0, r0, #(0x03 << 20)
    orr         r0, r0, #(0x03 << 22)
    mcr         p15, 0, r0, c1, c0, 2
    mov         r0, #0
    orr         r0, r0, #(0x01 << 30)
    FMXR        FPEXC, r0
    mov         pc, lr

/*
 * Create initial single-levle page table
 */

__create_page_tables:

    ldr         r4, =(_ld_ap_ram_pa_start - 0x4000)

    /*
    * Clear the 16K level 1 swapper page table
    */
    mov         r0, r4
    mov         r3, #0
    add         r6, r0, #0x4000
1:
    str         r3, [r0], #4
    str         r3, [r0], #4
    str         r3, [r0], #4
    str         r3, [r0], #4
    teq         r0, r6
    bne         1b

    ldr         r7, =mm_flags

    /*
    * Create identity mapping for first MB of kernel to
    * cater for the MMU enable.  This identity mapping
    * will be removed by paging_init().  We use our current program
    * counter to determine corresponding section base address.
    * VA=PA , length = 1MB
    */

    mov         r6, pc
    mov         r6, r6, lsr #20             @ start of Head section
    orr         r3, r7, r6, lsl #20         @ flags + Head base
    str         r3, [r4, r6, lsl #2]        @ identity mapping

    /*
    * Setup the pagetables vector mapped region.
    * VA=0x00000000 PA=0x40010000 , lengtg = 1MB
    */

    ldr         r6, =_ld_vector_ram_va_start
    mov         r6, r6, lsr #20             @ start of vector section
    ldr         r3, =_ld_vector_ram_pa_start
    mov         r3, r3, lsr #20             @ start of vector section
    orr         r3, r7, r3, lsl #20         @ flags + ebase
    str         r3, [r4, r6, lsl #2]        @ identity mapping

    /*
    * Setup the pagetables for our AP direct
    * mapped region.
    * VA=0x00100000 PA=0x40100000 , AP length
    */
    ldr         r3, =_ld_ap_ram_pa_start
    mov         r3, r3, lsr #20             @ start of AP section
    orr         r3, r7, r3, lsl #20         @ flags + AP_base
    ldr         r1, =_ld_ap_ram_va_start
    and         r1, r1, #0xff000000
    mov         r1, r1, lsr #18
    add         r0, r4, r1
    ldr         r1, =_ld_ap_ram_va_start
    and         r1, r1, #0x00f00000
    mov         r1, r1, lsr #18
    str         r3, [r0, r1]!
    ldr         r6, =(_ld_ap_ram_va_end - 1)
    add         r0, r0, #4
    add         r6, r4, r6, lsr #18
1:
    cmp         r0, r6
    add         r3, r3, #1 << 20
    strls       r3, [r0], #4
    bls         1b

    /* other TLB mapping example RIU....*/

    mov         pc, lr

/*
 * Init Distributed Interrupt Controler
 */

__init_dic:

    @ disable interrupts in DIC
    ldr         r0, =dic_pa
    ldr         r1, =0x0                    @ DICControl offset
    mov         r2, #0
    str         r2, [r0, r1]

    @ disable all interrupt sources ID0-ID63
    ldr         r1, =0x180                  @ Interrupt Clear-enable Register ID0-ID31 offset
    ldr         r2, =0xFFFFFFFF             @ Disable All Interrupts. Does not affect SW interrupts (ID0-ID15)
    str         r2, [r0, r1]
    ldr         r1, =0x184                  @ Interrupt Clear-enable Register ID32-ID63 offset
    str         r2, [r0, r1]

    @ clear all pending interrupts
    ldr         r1, =0x280                  @ Interrupt Clear-pending Register ID0-ID31 offset
    ldr         r2, =0xFFFFFFFF             @ Clear All Pending Interrupts
    str         r2, [r0, r1]
    ldr         r1, =0x284                  @ Interrupt Clear-pending Register ID32-ID63 offset
    str         r2, [r0, r1]

    @ enable interrupts in DIC
    ldr         r0, =dic_pa
    ldr         r1, =0x0                    @ DICControl offset
    ldr         r2, [r0, r1]
    orr         r2, r2, #0x1
    str         r2, [r0, r1]

    bx          lr

/*
 * Init CPU Interrupt Interface
 */

__init_interrupts:

    @ disable interrupts in CPU Interface
    ldr         r0, =cpu_if_pa
    ldr         r1, =0x0                    @ CPUControl offset
    mov         r2, #0
    str         r2, [r0, r1]

    @ enable interrupt sources in DIC we care about
    @ ID29 = Timer counter wrap interrupt
    ldr         r0, =dic_pa
    ldr         r1, =0x100                  @ Interrupt Set-enable Register ID0-ID31 offset
    ldr         r2, [r0, r1]
    orr         r2, r2, #1 << 29
    orr         r2, r2, #1 << 31
    str         r2, [r0, r1]
    @ set ID29 interrupt priority to highest
    ldr         r1, =0x41C                  @ Priority level Register ID28-ID31 offset
    ldr         r2, [r0, r1]
    bic         r2, r2, #0xFF00             @ Set ID29 priority to 0 (highest priority)
    str         r2, [r0, r1]

    @ set priority mask to 1 (allowing interrupts with priority of 0 through)
    @ i.e. ID29 Timer Interrupt
    ldr         r0, =cpu_if_pa
    ldr         r1, =0x4                    @ Priority Mask Register offset
    ldr         r2, =1 << 3
    str         r2, [r0, r1]

    @ enable interrupts in CPU interface
    ldr         r1, =0x0                    @ CPUControl offset
    ldr         r2, [r0, r1]
    orr         r2, r2, #1
    str         r2, [r0, r1]

    bx          lr

/*************************************************************************
 *
 * Init data catche
 *
 *************************************************************************/
.globl dcache_init
dcache_init:

    @ Invalidate D-cache by set/way
    @ Note: for Cortex-A9, there is no instruction for invalidating
    @ the whole D-cache. Need to invalidate line by line.
    @ Read cache size from the Cache Size Identification Register
    
    mrc p15, 1, r3, c0, c0, 0           @ Read current Cache Size Identification Register
    mov r1, #0x200
    sub r1, r1, #1
    and r3, r1, r3, LSR #13             @ r3 = (number of sets - 1)
    mov r0, #0                          @ r0 -> way counter
way_loop1:
    mov r1, #0                          @ r1 -> set counter
set_loop1:
    mov r2, r0, LSL #30                 @
    orr r2, r1, LSL #5                  @ r2 -> set/way cache-op format
    mcr p15, 0, r2, c7, c6, 2           @ Invalidate line described by r2
    add r1, r1, #1                      @ Increment set counter
    cmp r1, r3                          @ Check if the last set is reached...
    ble set_loop1                        @ ...if not, continue the set_loop...
    add r0, r0, #1                      @ ...else, Increment way counter
    cmp r0, #4                          @ Check if the last way is reached...
    blt way_loop1                        @ ...if not, continue the way_loop

    @ Enable dcaches
    @ D-cache is controlled by bit 2
    mrc p15, 0, r0, c1, c0, 0           @ read CP15 register 1
    orr r0, r0, #(0x1 <<2)              @ enable D Cache
    mcr p15, 0, r0, c1, c0, 0           @ write CP15 register 1

    @ Enable Program Flow Prediction
    @ Branch prediction is controlled by the System Control Register:
    @ Set Bit 11 to enable branch prediciton and return stack
    @ Turning on branch prediction requires a general enable
    @ CP15, c1. Control Register
    @ Bit 11 [Z] bit Program flow prediction:
    @ 0 = Program flow prediction disabled
    @ 1 = Program flow prediction enabled.
    mrc p15, 0, r0, c1, c0, 0           @ Read System Control Register
    orr r0, r0, #(0x1 <<11)
    mcr p15, 0, r0, c1, c0, 0           @ Write System Control Register

    @ Enable D-side prefetch
    @ Bit 2 [DP] Dside prefetch:
    @ 0 = Dside prefetch disabled
    @ 1 = Dside prefetch enabled.
    mrc 15, 0, r0, c1, c0, 1            @ Read Auxiliary Control Register
    orr r0, r0, #(0x1 <<2)              @ Enable Dside prefetch
    mcr p15, 0, r0, c1, c0, 1           @ Write Auxiliary Control Register

#if defined(CHAKRA)
    @invaliate tlb
    mov         r0, #0
    mcr         p15, 0, r0, c8, c7, 0
    isb
    dsb
#endif

    mov pc, lr


/*************************************************************************
 *
 * Turn On MMU
 *
 *************************************************************************/
.globl turn_on_mmu
turn_on_mmu:

    @ turn on MMU
    mrc         p15, 0, r0, c1, c0, 0       @ read control reg
    ldr         r1, =0x00001001
    orr         r0, r0, r1
    mov         r0, r0
    mcr         p15, 0, r0, c1, c0, 0       @ write control reg
    mrc         p15, 0, r3, c0, c0, 0       @ read id reg

    mov pc, lr

