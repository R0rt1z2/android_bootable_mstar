#include <stddef.h>
#include <stdio.h>
#include <stdarg.h>

int vsprintf(char *str, const char *fmt, va_list ap)
{
    return vsnprintf(str, 0x10000, fmt, ap);     // support max 64KB buffer
}
