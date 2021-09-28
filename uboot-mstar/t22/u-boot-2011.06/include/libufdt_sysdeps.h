#ifndef LIBDTOVERLAY_SYSDEPS_H
#define LIBDTOVERLAY_SYSDEPS_H

/* Change these includes to match your platform to bring in the
 * equivalent types available in a normal C runtime. At least things
 * like uint8_t, uint64_t, and bool (with |false|, |true| keywords)
 * must be present.
 */

#include "compiler.h"

typedef uint16_t fdt16_t;
typedef uint32_t fdt32_t;
typedef uint64_t fdt64_t;

#define FALSE 0
#define TRUE 1

#define dto_print printf

#ifdef DTO_ENABLE_DEBUG
/* Print functions, used for diagnostics.
 *
 * These have no effect unless FDT_ENABLE_DEBUG is defined.
 */
#define dto_debug(...)                   \
  do {                                   \
    printf("DEBUG: %s():", __func__); \
    printf(__VA_ARGS__);              \
  } while (0)
#else
#define dto_debug(...)
#endif

#define dto_error(...)                   \
  do {                                   \
    printf("ERROR: %s():", __func__); \
    printf(__VA_ARGS__);              \
  } while (0)

void dto_qsort(void *base, size_t nmemb, size_t size,
               int (*compar)(const void *, const void *));

void *dto_malloc(size_t size);

void dto_free(void *ptr);

char *dto_strdup(const char *s);

char *dto_strchr(const char *s, int c);

unsigned long int dto_strtoul(const char *nptr, char **endptr, int base);

size_t dto_strlen(const char *s);

int dto_memcmp(const void *lhs, const void *rhs, size_t n);

void *dto_memcpy(void *dest, const void *src, size_t n);

int dto_strcmp(const char *s1, const char *s2);

int dto_strncmp(const char *s1, const char *s2, size_t n);

void *dto_memchr(const void *s, int c, size_t n);

void *dto_memset(void *s, int c, size_t n);

#endif /* LIBDTOVERLAY_SYSDEPS_H */
