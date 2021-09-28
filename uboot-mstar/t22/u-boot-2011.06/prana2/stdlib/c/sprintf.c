#include <stddef.h>
#include <stdio.h>
#include <stdarg.h>

int sprintf(char *str, const char *fmt, ...)
{
    int ret;
    va_list ap;

    va_start(ap, fmt);
    ret = vsnprintf(str, 0x10000, fmt, ap);     // support max 64KB buffer
    va_end(ap);

    return ret;
}
