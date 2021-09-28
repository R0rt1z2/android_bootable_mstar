#include <stddef.h>
#include <stdio.h>
#include <stdarg.h>
#include <limits.h>

#ifndef ASSERT
#define ASSERT(expr)    do {} while (0)
#endif

#define to_digit(c)     ((c) - '0')
#define is_digit(c)     ((unsigned) to_digit(c) <= 9)

static const char hex16[] = { '0', '1', '2', '3', '4', '5', '6', '7',
                              '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

/*
 * %[-^][digits][.digits][scd]
 */

#define ZEROPAD     (1 << 0)
#define LADJUST     (1 << 1)
#define CADJUST     (1 << 2)

/*
 * Length
 */
#define LEN_DEFAULT     0
#define LEN_CHAR        1
#define LEN_SHORT       2
#define LEN_LONG        3
#define LEN_LONGLONG    4

#define NUM(str, n, base)                   \
    do {                                    \
        if (n < base)                       \
            *--str = hex16[n];              \
        else                                \
            do {                            \
                *--s = hex16[n % base];     \
            } while ( (n /= base) );        \
    } while (0)

int vsnprintf(char *str, size_t size, const char *fmt, va_list ap)
{
    signed int          sInt;
    unsigned int        uInt;
    signed long long    sLongLong;
    unsigned long long  uLongLong;

    int flags, width, prec, length;
    char *s, *p, *dst, *strMax;
    char digit[24];

    ASSERT(str != NULL && fmt != NULL);

    dst = str;
    strMax = str + size;

    while ( (uInt = *fmt++) )
    {

        if (uInt != '%')
        {
            if (dst < strMax)
                *dst++ = uInt;
            continue;
        }

        /* % */

        width = 0;
        length = LEN_DEFAULT;
        flags = 0;
        prec = -1;
    
    rflag:                /* read flag */
        uInt = *fmt++;

    reswitch:

        switch (uInt)
        {

        case 's':
            s = va_arg(ap, char *);
            if (width != 0 || prec >= 0)
            {
                p = s;
                if (prec < 0)
                    while (*p)
                        p++;
                else
                    while (*p && prec-- > 0)
                        p++;
                prec = p - s;  
                break;
            }

            while ( (dst < strMax) && (*dst = *s) )
            {
                s++;
                dst++;
            }
            continue;

        case 'd':
        case 'u': 
            p = s = digit + sizeof(digit);
            if (length == LEN_LONGLONG)
            {
                uLongLong = va_arg(ap, unsigned long long);
                if (uInt == 'd')
                {
                    sLongLong = uLongLong;
                    if (sLongLong < 0)
                        uLongLong = -sLongLong;
                    NUM(s, uLongLong, 10);
                    if (sLongLong < 0)
                        *--s = '-';
                }
                else
                {
                    NUM(s, uLongLong, 10);
                }
            }
            else
            {
                if (uInt == 'd')
                {
                    sInt = uInt = va_arg(ap, int);
                    if (sInt < 0)
                        uInt = -sInt;
                    NUM(s, uInt, 10);
                    if (sInt < 0)
                        *--s = '-';
                }
                else
                {
                    uInt = va_arg(ap, int);
                    NUM(s, uInt, 10);
                }
            }

            ASSERT( (p - s) <= (int) sizeof(digit));

            uInt = p - s;
            if (prec < 0 || prec > (int) uInt)
                prec = uInt;
            break;

        case 'p':
        case 'x': case 'X':
            p = s = digit + sizeof(digit);
            if (length == LEN_LONGLONG)
            {
                uLongLong = va_arg(ap, unsigned long long);
                NUM(s, uLongLong, 16);
            }
            else
            {
                uInt = va_arg(ap, int);
                NUM(s, uInt, 16);
            }

            ASSERT( (p - s) <= (int) sizeof(digit));

            uInt = p - s;
            if (prec < 0 || prec > (int) uInt)
                prec = uInt;
            break;

        case 'c':
            *(s = digit) = va_arg(ap, int);
            prec = 1;
            break;

        case '*':  /* must be postive value */
            width = va_arg(ap, int);
            goto rflag;

        case '0':
            flags |= ZEROPAD;
            goto rflag;

        case '1': case '2': case '3': case '4': case '5':
        case '6': case '7': case '8': case '9':
            width = to_digit(uInt);

            while (is_digit( (uInt = *fmt++) ) )
            {
                /* width = 10 * width + to_digit(uInt); */
                width <<= 1;
                width += (width << 2) + to_digit(uInt);
            }
            goto reswitch;

        case '.':
            uInt = *fmt++;
            if (uInt == '*')
            {
                prec = va_arg(ap, int);
                goto rflag;
            }

            ASSERT(is_digit(uInt));

            if (is_digit(uInt))
            {
                prec = to_digit(uInt);

                while ( is_digit( (uInt = *fmt++) ) )
                    prec = 10 * prec + to_digit(uInt);
            }
            goto reswitch;

        case '-':
            flags |= LADJUST;
            goto rflag;

        case '^':           /* center */
            flags |= CADJUST;
            goto rflag;

        case 'b': case 'B': /* 8051 compatibility */
            length = LEN_CHAR;
            goto rflag;
        case 'h':
            length = (length == LEN_SHORT) ? LEN_CHAR : LEN_SHORT;
            goto rflag;
        case 'l': case 'L':
            length = (length == LEN_LONG) ? LEN_LONGLONG : LEN_LONG;
            goto rflag;        

        default:
            ASSERT(uInt == '%');
            if ((dst < strMax) && !(*dst++ = uInt))
                return (dst - str);
            continue;
        } /* switch */


        /* format */

        /* prec now is length of visual part of string s */
        if (width > prec)
        {
            uInt = width - prec;

            if ( !(flags & (LADJUST | CADJUST)) )
            {
                if (uInt > 0)
                {
                    flags = (flags & ZEROPAD) ?  '0' : ' ';
                    do {
                        if (dst < strMax)
                            *dst++ = flags;
                    } while (--uInt > 0);
                }
                while (prec-- > 0)
                {
                    if (dst < strMax)
                        *dst++ = *s++;
                }

            }
            else if (flags & LADJUST)
            {
                while (prec-- > 0 && dst < strMax)
                    *dst++ = *s++;
                while (uInt-- > 0 && dst < strMax)
                    *dst++ = ' ';

            }
            else    /* CADJUST */
            {
                width = uInt / 2;
                uInt -= width;

                while (width-- > 0 && dst < strMax)
                    *dst++ = ' ';
                while (prec-- > 0 && dst < strMax)
                    *dst++ = *s++;
                while (uInt-- > 0 && dst < strMax)
                    *dst++ = ' ';
            }
        }
        else
        {
            while (prec-- > 0 && dst < strMax)
                *dst++ = *s++;
        }
    } /* while */   

    *dst = '\0';

    return (dst - str);
}

#if defined(TEST_VSNPRINTF)

#include <string.h>

int snprintf(char *str, size_t size, const char *fmt, ...)
{
    int ret;
    va_list ap;

    va_start(ap, fmt);
    ret = vsnprintf(str, size, fmt, ap);
    va_end(ap);

    return ret;
}

int main(int argc, char *argv[])
{
    char buf[256];
    int i;

    snprintf(buf, sizeof(buf), "%bd %s %c \t%s ip:%-20llx\n", 99, "id", 0, "lala", 0x12345678ABCDEFULL);

    printf("%s%s", buf , buf + strlen(buf) + 1);
    return 0;
}

#endif
