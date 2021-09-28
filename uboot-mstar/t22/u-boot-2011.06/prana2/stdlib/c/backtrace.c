#include <stddef.h>                                                                                                                                                     
#include <stdio.h>

#define get_reg(fp, offset) *(long *)((long)(fp) + offset)

void dump_backtrace(void)
{
#if defined (__aeon__)
    register long reg_fp __asm__("r10");
    register long fp;
    register long lr;

    printf("------ stack backtrace ------\n");
    for (fp = reg_fp; fp; fp = get_reg(fp, -8))
    {
        // print FP for debugging
        //printf("FP: %X\n", get_reg(fp, -8));

        lr = get_reg(fp, -4);
        if (lr == 0)
            break;
        printf("%X\n", lr);
    }
    printf("-----------------------------\n");
#endif
}
