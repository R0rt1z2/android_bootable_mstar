#ifndef __RISC32_UTIL_H__
#define __RISC32_UTIL_H__

#include "datatype.h"
#include "risc32_spr.h"

#if defined (__arm__)

#elif defined (__mips__)

extern U32 hal_lsbit_index(U32 mask);
extern U32 hal_msbit_index(U32 mask);

#define __mhal_lsbit_index(_value_) hal_lsbit_index(_value_)
#define __mhal_msbit_index(_value_) hal_msbit_index(_value_)

extern U32 hal_disable_interrupts(void);
#define __mhal_interrupt_disable(_old_) { (_old_) = hal_disable_interrupts(); }

extern void hal_enable_interrupts(void);
#define __mhal_interrupt_enable() hal_enable_interrupts()

extern void hal_restore_interrupts( U32 old );
#define __mhal_interrupt_restore(_old_) hal_restore_interrupts( _old_ )

#define ASM_CLEAR_EHB   "nop;nop;nop;\n"

#define __mhal_interrupt_mask( _vector_ )       \
MST_MACRO_START                                 \
    asm volatile (                              \
        "mfc0   $3,$12\n"                       \
        "la     $2,0x00000400\n"                \
        "sllv   $2,$2,%0\n"                     \
        "nor    $2,$2,$0\n"                     \
        "and    $3,$3,$2\n"                     \
        "mtc0   $3,$12\n"                       \
        ASM_CLEAR_EHB                           \
        :                                       \
        : "r"(_vector_)                         \
        : "$2", "$3"                            \
        );                                      \
MST_MACRO_END

#define __mhal_interrupt_unmask( _vector_ )     \
MST_MACRO_START                                 \
    asm volatile (                              \
        "mfc0   $3,$12\n"                       \
        "la     $2,0x00000400\n"                \
        "sllv   $2,$2,%0\n"                     \
        "or     $3,$3,$2\n"                     \
        "mtc0   $3,$12\n"                       \
        ASM_CLEAR_EHB                           \
        :                                       \
        : "r"(_vector_)                         \
        : "$2", "$3"                            \
        );                                      \
MST_MACRO_END


#else

#define __mhal_lsbit_index(_value_)                                         \
    ({                                                                      \
    unsigned long _index_;                                                  \
    __asm__ __volatile__ ("l.ff1\t\t%0,%1" : "=r" (_index_) : "r" (_value_));\
    _index_;                                                                \
    })

#define __mhal_msbit_index(_value_)                                         \
    ({                                                                      \
    unsigned long _index_;                                                  \
    __asm__ __volatile__ ("l.fl1\t\t%0,%1" : "=r" (_index_) : "r" (_value_));\
    _index_;                                                                \
    })

#define __mhal_interrupt_disable(_old_)                                     \
    MST_MACRO_START                                                         \
    _old_ = mfspr(SPR_SR);                                                  \
    mtspr(SPR_SR, (_old_) & ~(SPR_SR_IEE | SPR_SR_TEE));                    \
    MST_MACRO_END

#define __mhal_interrupt_enable()                                           \
    mtspr(SPR_SR, (SPR_SR_IEE|SPR_SR_TEE) | mfspr(SPR_SR))

#define __mhal_interrupt_restore(_old_)                                     \
    mtspr(SPR_SR, (~(SPR_SR_IEE|SPR_SR_TEE) & mfspr(SPR_SR) ) |             \
                  ( (SPR_SR_IEE|SPR_SR_TEE) & (_old_) ))

#define __mhal_interrupt_mask(_intr_)                                       \
    MST_MACRO_START                                                         \
    if (_intr_ != E_INTERRUPT_TICK_TIMER)                                   \
    {                                                                       \
        mtspr(SPR_PICMR, mfspr(SPR_PICMR) & ~(1 << (_intr_ - 1)));          \
    }                                                                       \
    MST_MACRO_END

#define __mhal_interrupt_unmask(_intr_)                                     \
    MST_MACRO_START                                                         \
    if (_intr_ != E_INTERRUPT_TICK_TIMER)                                   \
    {                                                                       \
        mtspr(SPR_PICMR, mfspr(SPR_PICMR) | (1 << (_intr_ - 1)));           \
    }                                                                       \
    MST_MACRO_END

#define __mhal_mmu_disable(_old_)                                     \
        MST_MACRO_START                                                         \
        _old_ = mfspr(SPR_SR);
#endif

#endif /* __RISC32_UTIL_H__ */
