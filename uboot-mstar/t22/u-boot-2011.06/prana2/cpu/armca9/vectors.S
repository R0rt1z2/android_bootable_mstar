#include "arm.inc"

.globl          exception_vector
exception_vector :
    b           reset
    ldr         pc, _undefined_instruction
    ldr         pc, _software_interrupt
    ldr         pc, _prefetch_abort
    ldr         pc, _data_abort
    ldr         pc, _not_used
    ldr         pc, _irq
    ldr         pc, _fiq

_undefined_instruction:
    .word       undefined_handler

_software_interrupt:
    .word       swi_handler

_prefetch_abort:
    .word       prefetch_handler

_data_abort:
    .word       abort_handler

_not_used:
    .word       not_used

_irq:
    .word       irq_handler

_fiq:
    .word       fiq_handler

_pad:
    .word       0x12345678    @///* now 16*4=64 */

    .align      5
undefined_handler:
    ldr         sp, =_ld_fiq_stack_start    @//use fiq stack pointer as temprory stack
    sub         sp, sp, #S_FRAME_SIZE
    stmia       sp, {r0 - r15}              @ copy r0-r15
    add         r1, sp, #S_PSR
    mrs         r2, cpsr
    str         r2, [r1]
    mov         r0, sp
    bl          do_undefined_instruction

undefined_loop:
    b           undefined_loop

    .align      5
swi_handler:
    ldr         sp, =_ld_fiq_stack_start    @ use fiq stack pointer as temprory stack
    sub         sp, sp, #S_FRAME_SIZE
    stmia       sp, {r0 - r15}              @ copy r0-r15
    add         r1, sp, #S_PSR
    mrs         r2, cpsr
    str         r2, [r1]
    mov         r0, sp
    bl          do_software_interrupt

swi_loop:
    b           swi_loop

    .align      5
prefetch_handler:
    ldr         sp, =_ld_fiq_stack_start    @ use fiq stack pointer as temprory stack
    sub         sp, sp, #S_FRAME_SIZE
    stmia       sp, {r0 - r15}              @ copy r0-r15
    add         r1, sp, #S_PSR
    mrs         r2, cpsr
    str         r2, [r1]
    mov         r0, sp
    bl          do_prefetch_abort

prefetch_loop:
    b           prefetch_loop

    .align      5
abort_handler:
    ldr         sp, =_ld_fiq_stack_start    @ use fiq stack pointer as temprory stack
    sub         sp, sp, #S_FRAME_SIZE
    stmia       sp, {r0 - r15}              @ copy r0-r15
    add         r1, sp, #S_PSR
    mrs         r2, cpsr
    str         r2, [r1]
    mov         r0, sp
    bl          do_data_abort
abort_loop:
    b           abort_loop

    .align      5
not_used:
    ldr         sp, =_ld_fiq_stack_start    @ use fiq stack pointer as temprory stack
    sub         sp, sp, #S_FRAME_SIZE
    stmia       sp, {r0 - r15}              @ copy r0-r15
    add         r1, sp, #S_PSR
    mrs         r2, cpsr
    str         r2, [r1]
    mov         r0, sp
    bl          do_not_used

not_used_loop:
    b           not_used_loop

    .align      5
irq_handler:
    ldr         sp, =_ld_irq_stack_start
    sub         sp, sp, #S_FRAME_SIZE
    stmia       sp, {r0 - r12}          @ Calling r0-r12
    add         r8, sp, #S_PC           @ !! R8 NEEDS to be saved !!
                                        @ a reserved stack spot would be good.
    stmdb       r8, {sp, lr}^           @ Calling SP, LR
    str         lr, [r8, #0]            @ Save calling PC
    mrs         r6, spsr
    str         r6, [r8, #4]            @ Save CPSR
    str         r0, [r8, #8]            @ Save OLD_R0
    mov         r0, sp

    @Acknowledge Interrupt
    ldr         r0, =cpu_if_va
    ldr         r1, =0xc @ Interrupt Acknowledge Register offset
    ldr         r2, [r0, r1]
    ldr         r3, =0x3F @ ACK_INTID mask
    and         r4, r2, r3 @ INTID - we need this at irq_handler_end
    cmp         r4, #29 @Private timer ID
    bleq        timer_interrupt_handler
    blne        do_irq

irq_handler_end:
    @Signal End of Interrupt
    ldr         r0, =cpu_if_va
    ldr         r1, =0x10 @ End Of Interrupt Register offset
    str         r4, [r0, r1]

    ldmia       sp, {r0 - lr}^          @ Calling r0 - lr
    mov         r0, r0
    ldr         lr, [sp, #S_PC]         @ Get PC
    add         sp, sp, #S_FRAME_SIZE
    subs        pc, lr, #4              @ return & move spsr_svc into cpsr

    .align      5
fiq_handler:
    ldr         sp, =_ld_fiq_stack_start
    sub         sp, sp, #S_FRAME_SIZE
    stmia       sp, {r0 - r12}          @ Calling r0-r12
    add         r8, sp, #S_PC           @ !! R8 NEEDS to be saved !!
                                        @ a reserved stack spot would be good.
    stmdb       r8, {sp, lr}^           @ Calling SP, LR
    str         lr, [r8, #0]            @ Save calling PC
    mrs         r6, spsr
    str         r6, [r8, #4]            @ Save CPSR
    str         r0, [r8, #8]            @ Save OLD_R0
    mov         r0, sp

    @Acknowledge Interrup
    ldr         r0, =cpu_if_va
    ldr         r1, =0xc                @ Interrupt Acknowledge Register offset
    ldr         r2, [r0, r1]
    ldr         r3, =0x3F               @ ACK_INTID mask
    and         r4, r2, r3              @ INTID - we need this at fiq_handler_end

    bl          do_fiq

fiq_handler_end:
    @Signal End of Interrupt
    ldr         r0, =cpu_if_va
    ldr         r1, =0x10 @ End Of Interrupt Register offset
    str         r4, [r0, r1]

    ldmia       sp, {r0 - lr}^          @ Calling r0 - lr
    mov         r0, r0
    ldr         lr, [sp, #S_PC]         @ Get PC
    add         sp, sp, #S_FRAME_SIZE
    subs        pc, lr, #4              @ return & move spsr_svc into cpsr

