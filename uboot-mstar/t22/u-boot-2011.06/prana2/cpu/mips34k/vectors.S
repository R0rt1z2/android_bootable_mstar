#define STARTUP_S

#include "mips.inc"

#define ENBALE_EXCEPTION 1

    .extern start

	.macro	mfc0_delay
	nop
	nop
	nop
	nop
	.endm

	# Clear execution hazard, MIPS32 R1
	.macro	clear_ehb
	nop
	nop
	nop
	.endm

    .section ".reset_vector","ax"
FUNC_START(reset_vector)

#ifdef CONFIG_PM_ENABLED //only for PM runnng in icache
    // enable L1 cache
    mfc0 k0, $16
    li  k1, ~0x07    
    and k0, k0, k1
    ori k0, k0, 0x3
    mtc0 k0, $16
#endif

    // Clear cause
    mtc0    z0, cause
    mfc0_delay

    // Set status register
    li      k0, 0x10400000
    mtc0    k0, status
    mfc0_delay

    // Clear count
    mtc0    z0, count
    mfc0_delay

    // Initialize config
#    li      k0, 0x00000002
#    mtc0    k0, config
#    mtc0    k0, $16
#    mfc0_delay

    la	    v0, _start		# jump to start
    jr	    v0
    nop			        # (delay slot)


FUNC_END(reset_vector)

##-----------------------------------------------------------------------------
## Startup code

	.section ".text.start","ax"
FUNC_START(_start)
    // Set interrupt mode to compaitible mode.
    // 1. Need not to fill IntCtlVs, the default value is already 0.
    // 2. Set CauseIv = 0. Use general exception vector. page 66, 117.
    mfc0    k0, cause
    mfc0_delay

    li      k1, ~(1<<23)
    and     k0, k0, k1
    mtc0    k0, cause
    mfc0_delay


    // 3. Set StatusBEV  = 1 and adjust EBASE coprocessor
    mfc0    k0, status
    mfc0_delay

    li      k1, (1<<22)
    or     k0, k0, k1

    mtc0    k0, status
    mfc0_delay


    mfc0    k0, $15, 1
    mfc0_delay

    # 0x000000 for AEON space
    li      k1, ~(0x3FFFFF)
    and     k0, k0, k1
    li      k1, 0
    or      k0, k0, k1
    mtc0     k0, $15, 1
    mfc0_delay


    // 4. Set StatusBEV & StatusEXL to be zero. page 66,
    mfc0    k0, status
    mfc0_delay

    li      k1, ~((1<<22)|(1<<1))
    and     k0, k0, k1
    mtc0    k0, status
    mfc0_delay

    // Now the Refill exception vector is 0x80000000 and
    // general exception vector is 0x80000180.

    // Disable all interrupts.
    mfc0    k0, status
    mfc0_delay

    li      k1, 0xffff00fe
    and     k0, k0, k1
    mtc0    k0, status
    mfc0_delay


    // Clear BSS
    jal     hal_0_bss
    nop

    // Clear stack.
    li      t0, 0xDEADBEEF
    la      k0, .stack_end
    la      k1, __stack_begin
1:
    sw      t0, 0(k0)
    addi    k0, 4
    bne     k1, k0, 1b
    nop

    // Set the correct user stack pointer.
    la      sp, __stack_begin-16
    move    fp, sp
    la      a0,__stack_begin

    la      k0, start
    j       k0                      // jump to C codes!
    nop

FUNC_END(_start)

//        .section .text.hal_zero_bss
    FUNC_START(hal_0_bss)
#define STORE_OP        sw
#define BLOCK_SHIFT     5
    la      MIPS_REG_4, __bss_start                     
    la      MIPS_REG_5, __bss_end                       
    andi    MIPS_REG_6, MIPS_REG_4, mips_regsize-1              
    bne     MIPS_REG_6, MIPS_REG_0, 1f                        
    nop

    subu            MIPS_REG_7,MIPS_REG_5,MIPS_REG_4                    
    srl             MIPS_REG_7,MIPS_REG_7,BLOCK_SHIFT           
    sll             MIPS_REG_7,MIPS_REG_7,BLOCK_SHIFT          
    addu            MIPS_REG_7,MIPS_REG_4,MIPS_REG_7                 
2:  STORE_OP        MIPS_REG_0,(mips_regsize*0)(MIPS_REG_4)
    STORE_OP        MIPS_REG_0,(mips_regsize*1)(MIPS_REG_4)
    STORE_OP        MIPS_REG_0,(mips_regsize*2)(MIPS_REG_4)
    STORE_OP        MIPS_REG_0,(mips_regsize*3)(MIPS_REG_4)
    STORE_OP        MIPS_REG_0,(mips_regsize*4)(MIPS_REG_4)
    STORE_OP        MIPS_REG_0,(mips_regsize*5)(MIPS_REG_4)
    STORE_OP        MIPS_REG_0,(mips_regsize*6)(MIPS_REG_4)
    STORE_OP        MIPS_REG_0,(mips_regsize*7)(MIPS_REG_4)
    addu            MIPS_REG_4,MIPS_REG_4,mips_regsize*8        
    bne             MIPS_REG_7,MIPS_REG_4,2b                    
    nop

    beq		a1,a0,3f
    nop

    # finish 1 byte at a time
1:  sb      zero,0(a0)                          # zero memory
    addiu   a0,a0,1                             # next addr
    bne     a0,a1,1b                            # to next store
    nop
3:  jr      ra
    nop
FUNC_END(hal_0_bss)


// define TLB refill excpetion service routine.
    .section ".tlb_refill_vector","ax"
FUNC_START(tlb_refill_vector)

#if (ENBALE_EXCEPTION)
    jal     mhal_exception_handler
    nop
1:
    b       1b
    nop

#else
    // just let EPC = EPC + 4
    mfc0    k0, epc
    addi    k0, 4
    mtc0    k0, epc
    nop
    nop
    nop
    eret
    nop
#endif

FUNC_END(tlb_refill_vector)


// define excpetion service routine.
    .section ".other_vector","ax"
FUNC_START(other_vector)

    // Judge if it is an exception.
    mfc0    k0, cause
    mfc0_delay
    andi    k0, k0, 0x7c                    # ExcCode, page 119
    beqz    k0, Process_Interrupt
    nop

#if (ENBALE_EXCEPTION)

    b       Process_Exception
    nop

#else
    // just let EPC = EPC + 4
    mfc0    k0, epc
    mfc0_delay
    addi    k0, 4
    mtc0    k0, epc
    nop
    nop
    nop
    eret
    nop
#endif


Process_Interrupt:

    // Disable all interrupts.
    mfc0    MIPS_REG_26, status
    mfc0_delay
    li      MIPS_REG_27, 0xfffffffe
    and     MIPS_REG_26, MIPS_REG_26, MIPS_REG_27
    mtc0    MIPS_REG_26, status
    mfc0_delay

    // Save all registers -- Begin.
    move    MIPS_REG_27, MIPS_REG_29                         
    addi    sp, sp, -mips_exception_decrement
    sw    k0,mipsreg_vector(MIPS_REG_29)            

    .set    noat
    sgpr    0, MIPS_REG_29
    sgpr    1, MIPS_REG_29
    sgpr    2, MIPS_REG_29
    sgpr    3, MIPS_REG_29
    sgpr    4, MIPS_REG_29
    sgpr    5, MIPS_REG_29
    sgpr    6, MIPS_REG_29
    sgpr    7, MIPS_REG_29
    sgpr    8, MIPS_REG_29
    sgpr    9, MIPS_REG_29
    sgpr    10, MIPS_REG_29
    sgpr    11, MIPS_REG_29
    sgpr    12, MIPS_REG_29
    sgpr    13, MIPS_REG_29
    sgpr    14, MIPS_REG_29
    sgpr    15, MIPS_REG_29
    sgpr    16, MIPS_REG_29
    sgpr    17, MIPS_REG_29
    sgpr    18, MIPS_REG_29
    sgpr    19, MIPS_REG_29
    sgpr    20, MIPS_REG_29
    sgpr    21, MIPS_REG_29
    sgpr    22, MIPS_REG_29
    sgpr    23, MIPS_REG_29
    sgpr    24, MIPS_REG_29
    sgpr    25, MIPS_REG_29
    sgpr    28, MIPS_REG_29
    sgpr    30, MIPS_REG_29    
    sgpr    31, MIPS_REG_29
    .set    at

    mfhi    MIPS_REG_4
    mflo    MIPS_REG_5
    shi     MIPS_REG_4, MIPS_REG_29
    slo     MIPS_REG_5, MIPS_REG_29
    ssp     MIPS_REG_27, MIPS_REG_29                

    mfc0    MIPS_REG_9, status
    mfc0_delay
    mfc0    MIPS_REG_10, epc
    mfc0_delay
    sw      MIPS_REG_9, mipsreg_sr(MIPS_REG_29)
    sw      MIPS_REG_10, mipsreg_pc(MIPS_REG_29)

    hal_var_save_interrupt sp

    // Save all registers -- End.
    la      gp, _gp
    move    s0, MIPS_REG_29                          

    subu    MIPS_REG_29, MIPS_REG_29, mips_stack_frame_size  

    move    a0, s0                         
    mfc0    a1, cause                       
    mfc0_delay
    jal     mhal_interrupt_handler
    nop


    // Restore all registers, --Bebin
    addi    MIPS_REG_29, MIPS_REG_29, mips_stack_frame_size 

    hal_var_restore sp

    lhi     t1, MIPS_REG_29
    llo     t2, MIPS_REG_29
    mthi    t1
    mtlo    t2

    # load GPRs
    .set    noat
    lgpr    1, MIPS_REG_29
    lgpr    2, MIPS_REG_29
    lgpr    3, MIPS_REG_29
    lgpr    4, MIPS_REG_29
    lgpr    5, MIPS_REG_29
    lgpr    6, MIPS_REG_29
    lgpr    7, MIPS_REG_29
    lgpr    8, MIPS_REG_29
    lgpr    9, MIPS_REG_29
    lgpr    10, MIPS_REG_29
    lgpr    11, MIPS_REG_29
    lgpr    12, MIPS_REG_29
    lgpr    13, MIPS_REG_29
    lgpr    14, MIPS_REG_29
    lgpr    15, MIPS_REG_29
    lgpr    16, MIPS_REG_29
    lgpr    17, MIPS_REG_29
    lgpr    18, MIPS_REG_29
    lgpr    19, MIPS_REG_29
    lgpr    20, MIPS_REG_29
    lgpr    21, MIPS_REG_29
    lgpr    22, MIPS_REG_29
    lgpr    23, MIPS_REG_29
    lgpr    24, MIPS_REG_29
    lgpr    25, MIPS_REG_29 
    lgpr    28, MIPS_REG_29      
    lgpr    30, MIPS_REG_29    
    lgpr    31, MIPS_REG_29    
    .set    at

    lw      k1, mipsreg_sr(sp)      
    lw      k0, mipsreg_pc(sp)      
    lsp     MIPS_REG_29, MIPS_REG_29                  
    // Restore all registers, --End

    // Enable all interrupts.
    ori     k1, k1, 1

    hal_cpu_eret k0, k1


Process_Exception:

    // Disable all interrupts.
    mfc0    k0, status
    mfc0_delay
    li      k1, 0xfffffffe
    and     k0, k0, k1
    mtc0    k0, status
    mfc0_delay


    move    k1, MIPS_REG_29                          # K1 = original SP

    addi    MIPS_REG_29, MIPS_REG_29, -mips_exception_decrement
                

    sw    k0,mipsreg_vector(MIPS_REG_29)             # store vector

    .set    noat
    sgpr    0, MIPS_REG_29
    sgpr    1, MIPS_REG_29
    sgpr    2, MIPS_REG_29
    sgpr    3, MIPS_REG_29
    sgpr    4, MIPS_REG_29
    sgpr    5, MIPS_REG_29
    sgpr    6, MIPS_REG_29
    sgpr    7, MIPS_REG_29
    sgpr    8, MIPS_REG_29
    sgpr    9, MIPS_REG_29
    sgpr    10, MIPS_REG_29
    sgpr    11, MIPS_REG_29
    sgpr    12, MIPS_REG_29
    sgpr    13, MIPS_REG_29
    sgpr    14, MIPS_REG_29
    sgpr    15, MIPS_REG_29
    sgpr    16, MIPS_REG_29
    sgpr    17, MIPS_REG_29
    sgpr    18, MIPS_REG_29
    sgpr    19, MIPS_REG_29
    sgpr    20, MIPS_REG_29
    sgpr    21, MIPS_REG_29
    sgpr    22, MIPS_REG_29
    sgpr    23, MIPS_REG_29
    sgpr    24, MIPS_REG_29
    sgpr    25, MIPS_REG_29
 
    sgpr    28, MIPS_REG_29    # == GP
 
    sgpr    30, MIPS_REG_29    # == FP
    sgpr    31, MIPS_REG_29    # == RA
    .set    at

    mfhi    a0
    mflo    a1
    shi     a0, MIPS_REG_29
    slo     a1, MIPS_REG_29

    ssp     k1, MIPS_REG_29                          # store in reg dump

    mfc0    t1, status
    mfc0_delay
    mfc0    t2, epc
    mfc0_delay
    mfc0    t3, cause
    mfc0_delay
    sw      t1, mipsreg_sr(MIPS_REG_29)
    sw      t2, mipsreg_pc(MIPS_REG_29)
    sw      t3, mipsreg_cause(MIPS_REG_29)

    hal_var_save_interrupt sp

    // Save all registers -- End.
    la      gp, _gp
    move    s0, sp                          # save pointer to saved state

    subu    sp, sp, mips_stack_frame_size   # make a null frame

    move    a0, s0                          # arg0 - HAL_SavedRegisters
    mfc0    a1, cause                       # arg1 - vector index
    mfc0_delay
    jal     mhal_exception_handler
    nop



FUNC_END(other_vector)

#undef STARTUP_S
