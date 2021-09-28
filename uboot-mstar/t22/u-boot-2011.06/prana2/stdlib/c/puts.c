#include <stddef.h>
#include <stdio.h>


// Define this puts() as weak for log save to USB function
int puts(const char *s) __attribute__ ((weak));

int puts(const char *s)
{
    int i;

    for (; (i = *s); s++)
    {
        putchar(i);
    }
    putchar('\n');

    return 0;
}

