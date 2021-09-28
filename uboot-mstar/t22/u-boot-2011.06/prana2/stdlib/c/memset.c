#include <string.h>

void *memset (void *s, int c, size_t n)
{
    char *str = s;

    while (n-- > 0)
    {
        *str++ = c;
    }
    return s;
}
