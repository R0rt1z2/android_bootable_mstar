#include <stdio.h>
#include <assert.h>
#include "datatype.h"
#include "mhal_arch.h"

struct interrupt
{
    mhal_isr_t  isr;
    U32         data;
};

static void __mhal_default_isr(MHAL_SavedRegisters *regs, U32 vector)
{
    printf("\nmhal_default_isr vector: %lu regs: %p\n", vector, regs);
    dump_backtrace();
    while(1);
}

void mhal_default_isr(MHAL_SavedRegisters *regs, U32 vector) __attribute__ ((weak, alias("__mhal_default_isr")));

static struct interrupt intr_table[E_INTERRUPT_MAX + 1];

static volatile int nr_intr_locks;
static volatile int saved_intr_state;

U32 hal_lsbit_index(U32 u32Mask)
{
    U32 in = u32Mask;

    static const signed char table[64] =
    { 
      -1,  0, 1, 12, 2,  6, 0, 13, 3,  0,  7,  0,  0,  0,  0, 14, 
      10,  4, 0,  0, 8,  0, 0, 25, 0,  0,  0,  0,  0, 21, 27, 15, 
      31, 11, 5,  0, 0,  0, 0,  0, 9,  0,  0, 24,  0,  0, 20, 26, 
      30,  0, 0,  0, 0, 23, 0, 19, 29, 0, 22, 18, 28, 17, 16, 0
    };

    in &= ~(in-1UL);
    in = (in<<16)-in;
    in = (in<<6)+in;
    in = (in<<4)+in;

    return table[in>>26];
}

U32 hal_msbit_index(U32 u32Mask)
{
    U32 i = u32Mask;
    U32 j;
    U32 pattern1=0x55555555;
	U32 pattern2=0x33333333;
	U32 pattern3=0x000F000F;
	
    /* 
        *  Step 1: Make word with all ones from that one to the right 
        */ 
    i |= i >> 16;
    i |= i >> 8;
    i |= i >> 4;
    i |= i >> 2;
    i |= i >> 1;

    /* 
        * Step 2: calculate number of "1" bits in the word        
        */
    j = (i & pattern1) + ((i >> 1) & pattern1);
    j = (j & pattern2) + ((j >> 2) & pattern2);
    j = j + (j >> 4);
    j = (j & pattern3) + ((j >> 8) & pattern3);
	
    return (U32)((j + (j >> 16)) & 0xFF) - 1;

}


int  mhal_interrupt_disable(void)
{
    int spr_sr;
    __mhal_interrupt_disable(spr_sr);
    return spr_sr;
}

void mhal_interrupt_enable(void)
{
    __mhal_interrupt_enable();
}

void mhal_interrupt_restore(int old)
{
    __mhal_interrupt_restore(old);
}


void mhal_interrupt_mask_all(void)
{
    unsigned long sr;

    sr = mfsr();
    sr &= ~(0x0000ff00); //SR_IMASK
    mtsr(sr);
}

void mhal_interrupt_mask(MHAL_INTERRUPT_TYPE intr_num)
{
    __mhal_interrupt_mask(intr_num);
}

void mhal_interrupt_unmask(MHAL_INTERRUPT_TYPE intr_num)
{
    __mhal_interrupt_unmask(intr_num);
}

void mhal_interrupt_lock(void)
{
    int spr_sr;

    __mhal_interrupt_disable(spr_sr);
    if (++nr_intr_locks == 1)
    {
        saved_intr_state = spr_sr;
    }

    ASSERT(nr_intr_locks > 0);
}

void mhal_interrupt_unlock(void)
{
    ASSERT(nr_intr_locks > 0);

    if (--nr_intr_locks == 0)
    {
        __mhal_interrupt_restore(saved_intr_state);
    }
}

void mhal_interrupt_attach(MHAL_INTERRUPT_TYPE intr_num, mhal_isr_t isr, U32 dat)
{
    if (intr_num <= E_INTERRUPT_MAX)
    {
        intr_table[intr_num].isr = isr;
        intr_table[intr_num].data = dat;
        mhal_interrupt_unmask(intr_num);
    }
}

void mhal_interrupt_detach(MHAL_INTERRUPT_TYPE intr_num)
{
    if (intr_num <= E_INTERRUPT_MAX)
    {
        intr_table[intr_num].isr = 0;
        mhal_interrupt_mask(intr_num);
    }
}

extern void hal_interrupt_acknowledge_im( unsigned long vector );
extern void hal_clock_reset( unsigned long step );
void mhal_interrupt_handler(MHAL_SavedRegisters *regs, int vector)
{
    unsigned long cause;
    unsigned long picvec;
    mhal_isr_t    isr;

    cause = mfcause();

    if (cause & 0x00000400) //CAUSE_IP3 (intr0)
    {
        picvec = E_INTERRUPT_IRQ;

        isr = intr_table[picvec].isr;
        if (isr)
        {
           isr(regs, intr_table[picvec].data);
        }
    }

    if (cause & 0x00000800) //CAUSE_IP4 (intr1)
    {
        picvec = E_INTERRUPT_FIQ;

        isr = intr_table[picvec].isr;
        if (isr)
        {
           isr(regs, intr_table[picvec].data);
        }
    }

    if (cause & 0x00008000) //CAUSE_IP8 (intr5)
    {
        picvec = E_INTERRUPT_TICK_TIMER;

        isr = intr_table[picvec].isr;
        if (isr)
        {
           isr(regs, intr_table[picvec].data);
        }

        hal_clock_reset(MIPS_CLOCK_FREQ/2000);

        hal_interrupt_acknowledge_im(picvec);
    }

}
