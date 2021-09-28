#include <stddef.h>
#include <stdio.h>


// Define this printf() as weak for log save to USB function
int printf(const char *fmt, ...) __attribute__ ((weak));
int printf(const char *fmt, ...)
{
    int i, iret;
    char szPrint[PRINTF_CHAR_MAX];
    va_list ap;

    va_start(ap, fmt);
    iret = vsnprintf(szPrint, PRINTF_CHAR_MAX, fmt, ap);
    va_end(ap);

    for (i = 0; i < iret; i++)
    {
        putchar(szPrint[i]);
    }

    return iret;
}

int printk(const char *fmt, ...) __attribute__ ((weak, alias("printf")));

