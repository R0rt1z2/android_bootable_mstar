#define STARTUP_S

#include "mips.inc"

#define ENBALE_EXCEPTION 1

    .extern start



    .section ".reset_vector","ax"
FUNC_START(reset_vector)
    // Set status register
    li      k0, 0x10000000
    mtc0    k0, status
    // Clear count
    mtc0    z0, count
    // Clear cause
    mtc0    z0, cause
    // Initialize config
    li      k0, 0x00000002
    mtc0    k0, config

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
    li      k1, ~(1<<23)
    and     k0, k0, k1
    mtc0    k0, cause

    // 3. Set StatusBEV  = 1 and adjust EBASE coprocessor
    mfc0    k0, status
    li      k1, (1<<22)
    and     k0, k0, k1
    mtc0    k0, status

    mfc0    k0, $15, 1
    # 0x0 for default MIPS space
    li      k1, 0
    or      k0, k0, k1
    mtc0     k0, $15, 1

    // 4. Set StatusBEV & StatusEXL to be zero. page 66,
    mfc0    k0, status
    li      k1, ~((1<<22)|(1<<1))
    and     k0, k0, k1
    mtc0    k0, status
    // Now the Refill exception vector is 0x0 and
    // general exception vector is 0x0.

    // Disable all interrupts.
    mfc0    k0, status
    li      k1, 0xffff00fe
    and     k0, k0, k1
    mtc0    k0, status

    // Clear BSS
    jal     hal_zero_bss
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
    FUNC_START(hal_zero_bss)
#define STORE_OP        sw
#define BLOCK_SHIFT     5
    la      a0, __bss_start                     # start of bss
    la      a1, __bss_end                       # end of bss
    andi    a2, a0, mips_regsize-1              # is bss aligned?
    bne     a2, zero, 1f                        # skip word copy
    nop

    # loop with 8 stores per loop
    subu            a3,a1,a0                    # get length
    srl             a3,a3,BLOCK_SHIFT           # get number of blocks
    sll             a3,a3,BLOCK_SHIFT           # get length of blocks
    addu            a3,a0,a3                    # get end addr of blocks
2:  STORE_OP        zero,(mips_regsize*0)(a0)
    STORE_OP        zero,(mips_regsize*1)(a0)
    STORE_OP        zero,(mips_regsize*2)(a0)
    STORE_OP        zero,(mips_regsize*3)(a0)
    STORE_OP        zero,(mips_regsize*4)(a0)
    STORE_OP        zero,(mips_regsize*5)(a0)
    STORE_OP        zero,(mips_regsize*6)(a0)
    STORE_OP        zero,(mips_regsize*7)(a0)
    addu            a0,a0,mips_regsize*8        # next addr
    bne             a3,a0,2b                    # to next store
    nop

    # If length is a multiple of block size then we
    # are done and need to skip the byte loop
    beq		a1,a0,3f
    nop

    # finish 1 byte at a time
1:  sb      zero,0(a0)                          # zero memory
    addiu   a0,a0,1                             # next addr
    bne     a0,a1,1b                            # to next store
    nop
3:  jr      ra
    nop
FUNC_END(hal_zero_bss)


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
    andi    k0, k0, 0x7c                    # ExcCode, page 119
    beqz    k0, Process_Interrupt
    nop

#if (ENBALE_EXCEPTION)

    b       Process_Exception
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

Process_Interrupt:

    // Disable all interrupts.
    mfc0    k0, status
    li      k1, 0xfffffffe
    and     k0, k0, k1
    mtc0    k0, status

    // Save all registers -- Begin.
    # We enter here with all of the CPU state still
    # in its registers except:
    # K0 = vector index
    # K1 = address of this function

    move    k1, sp                          # K1 = original SP

    addi    sp, sp, -mips_exception_decrement
                # space for registers + safety margin

    sw    k0,mipsreg_vector(sp)             # store vector

    # store GPRs
    .set    noat
    sgpr    0, sp
    sgpr    1, sp
    sgpr    2, sp
    sgpr    3, sp
    sgpr    4, sp
    sgpr    5, sp
    sgpr    6, sp
    sgpr    7, sp
    sgpr    8, sp
    sgpr    9, sp
    sgpr    10, sp
    sgpr    11, sp
    sgpr    12, sp
    sgpr    13, sp
    sgpr    14, sp
    sgpr    15, sp
    sgpr    16, sp
    sgpr    17, sp
    sgpr    18, sp
    sgpr    19, sp
    sgpr    20, sp
    sgpr    21, sp
    sgpr    22, sp
    sgpr    23, sp
    sgpr    24, sp
    sgpr    25, sp
#    sgpr    26, sp    # == K0
#    sgpr    27, sp    # == K1
    sgpr    28, sp    # == GP
#    sgpr    29, sp    # == SP
    sgpr    30, sp    # == FP
    sgpr    31, sp    # == RA
    .set    at

    mfhi    a0
    mflo    a1
    shi     a0, sp
    slo     a1, sp

    # K1 contains original SP
    ssp     k1, sp                          # store in reg dump

    mfc0    t1, status
    mfc0    t2, epc
    sw      t1, mipsreg_sr(sp)
    sw      t2, mipsreg_pc(sp)

    hal_var_save_interrupt sp

    // Save all registers -- End.
    la      gp, _gp
    move    s0, sp                          # save pointer to saved state

    subu    sp, sp, mips_stack_frame_size   # make a null frame

    move    a0, s0                          # arg0 - HAL_SavedRegisters
    mfc0    a1, cause                       # arg1 - vector index
    jal     mhal_interrupt_handler
    nop


    // Restore all registers, --Bebin
    addi    sp, sp, mips_stack_frame_size   # retrieve CPU state ptr

    hal_var_restore sp

    lhi     t1, sp
    llo     t2, sp
    mthi    t1
    mtlo    t2

    # load GPRs
    .set    noat
#    lgpr    0, sp
    lgpr    1, sp
    lgpr    2, sp
    lgpr    3, sp
    lgpr    4, sp
    lgpr    5, sp
    lgpr    6, sp
    lgpr    7, sp
    lgpr    8, sp
    lgpr    9, sp
    lgpr    10, sp
    lgpr    11, sp
    lgpr    12, sp
    lgpr    13, sp
    lgpr    14, sp
    lgpr    15, sp
    lgpr    16, sp
    lgpr    17, sp
    lgpr    18, sp
    lgpr    19, sp
    lgpr    20, sp
    lgpr    21, sp
    lgpr    22, sp
    lgpr    23, sp
    lgpr    24, sp
    lgpr    25, sp
#    lgpr    26, sp    # == K0
#    lgpr    27, sp    # == K1
    lgpr    28, sp    # == GP
#    lgpr    29, sp    # == SP
    lgpr    30, sp    # == FP
    lgpr    31, sp    # == RA
    .set    at

    lw      k1, mipsreg_sr(sp)      # K1 = saved SR
    lw      k0, mipsreg_pc(sp)      # K0 = return PC
    lsp     sp, sp                  # load SP
    // Restore all registers, --End

    // Enable all interrupts.
    ori     k1, k1, 1

    hal_cpu_eret k0, k1


Process_Exception:

    // Disable all interrupts.
    mfc0    k0, status
    li      k1, 0xfffffffe
    and     k0, k0, k1
    mtc0    k0, status

    // Save all registers -- Begin.
    # We enter here with all of the CPU state still
    # in its registers except:
    # K0 = vector index
    # K1 = address of this function

    move    k1, sp                          # K1 = original SP

    addi    sp, sp, -mips_exception_decrement
                # space for registers + safety margin

    sw    k0,mipsreg_vector(sp)             # store vector

    # store GPRs
    .set    noat
    sgpr    0, sp
    sgpr    1, sp
    sgpr    2, sp
    sgpr    3, sp
    sgpr    4, sp
    sgpr    5, sp
    sgpr    6, sp
    sgpr    7, sp
    sgpr    8, sp
    sgpr    9, sp
    sgpr    10, sp
    sgpr    11, sp
    sgpr    12, sp
    sgpr    13, sp
    sgpr    14, sp
    sgpr    15, sp
    sgpr    16, sp
    sgpr    17, sp
    sgpr    18, sp
    sgpr    19, sp
    sgpr    20, sp
    sgpr    21, sp
    sgpr    22, sp
    sgpr    23, sp
    sgpr    24, sp
    sgpr    25, sp
 #   sgpr    26, sp    # == K0
 #   sgpr    27, sp    # == K1
    sgpr    28, sp    # == GP
 #   sgpr    29, sp    # == SP
    sgpr    30, sp    # == FP
    sgpr    31, sp    # == RA
    .set    at

    mfhi    a0
    mflo    a1
    shi     a0, sp
    slo     a1, sp

    # K1 contains original SP
    ssp     k1, sp                          # store in reg dump

    mfc0    t1, status
    mfc0    t2, epc
    mfc0    t3, cause
    sw      t1, mipsreg_sr(sp)
    sw      t2, mipsreg_pc(sp)
    sw      t3, mipsreg_cause(sp)

    hal_var_save_interrupt sp

    // Save all registers -- End.
    la      gp, _gp
    move    s0, sp                          # save pointer to saved state

    subu    sp, sp, mips_stack_frame_size   # make a null frame

    move    a0, s0                          # arg0 - HAL_SavedRegisters
    mfc0    a1, cause                       # arg1 - vector index
    jal     mhal_exception_handler
    nop


FUNC_END(other_vector)

#undef STARTUP_S

