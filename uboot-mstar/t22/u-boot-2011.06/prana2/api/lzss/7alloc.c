/* Alloc.c -- Memory allocation functions
2008-09-24
Igor Pavlov
Public domain */

#include <stdlib.h>

#include "7alloc.h"

// #define _SZ_ALLOC_DEBUG

/* use _SZ_ALLOC_DEBUG to debug alloc/free operations */
#ifdef _SZ_ALLOC_DEBUG
#include <stdio.h>
int g_allocCount = 0;
#endif

extern void *zalloc(void *ptr, unsigned long nNum, unsigned long nSize);
extern void zfree(void *x, void *address, unsigned nbytes);

void *MyAlloc(size_t size)
{
  void *ptr = NULL;
  
  if (size == 0)
    return 0;
  #ifdef _SZ_ALLOC_DEBUG
  {
    void *p = zalloc(ptr, 1, size);
	printf("\nAlloc %10d bytes, count = %10d,  addr = %8X", (int)size, g_allocCount++, (unsigned)p);
    return p;
  }
  #else
  return zalloc(ptr, 1, size);
  #endif
}

void MyFree(void *address)
{
  #ifdef _SZ_ALLOC_DEBUG
  if (address != 0)
	printf("\nFree; count = %10d,  addr = %8X", --g_allocCount, (unsigned)address);
  #endif
  zfree(address, address, 4);
}

