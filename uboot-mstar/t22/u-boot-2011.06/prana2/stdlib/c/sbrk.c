#include <sys/types.h>  /* For size_t */
#include <stddef.h>
#include <stdio.h>

#define MFAIL       ((void *) -1)

// please define these symbols in linker script
extern unsigned char __heap[];
extern unsigned char __heap_end[];

static unsigned char * sbrk_top = __heap;

#if( defined(CHAKRA) )
void sbrk_Reset(void)
{
    //printf("sbrk_Reset\n");
    sbrk_top = __heap;
}

unsigned char * sbrk_Get_TOP(void)
{
    return sbrk_top;
}
#endif

void *sbrk(unsigned int size)
{
    void *ptr;
    //printf("sbrk(%u)\n", size);
    // NOTE: we assume all allocated address are 4 byte-aligned!!!
    size = (size + 3) & (~3);

    if (size > 0)
    {
        if ((sbrk_top + size) <= __heap_end)
        {
            ptr = sbrk_top;
            sbrk_top += size;
            return ptr;
        }
    }
    else if (size == 0)
    {
        return sbrk_top;
    }
    else
    {
        printf("sbrk: error\n");
        // we don't currently support shrink behavior
    }
    return MFAIL;
}

void *_sbrk(unsigned int size) __attribute__ ((weak, alias("sbrk")));

////////////////////////////////////////////////////////////////////////////////////////////////
#define TTF_MFAIL       ((void *) -1)
#define TTF_MEMORY_POOL_SIZE        (1024*300)

unsigned char *__ttf_heap = NULL;
unsigned char *__ttf_heap_end = NULL;

static unsigned char * ttf_sbrk_top = NULL;
static unsigned long u32TTF_Memory_Pool_Size = TTF_MEMORY_POOL_SIZE;
static unsigned long u32TTF_Memory_Pool_Addr = 0;

extern void* malloc(size_t);

void ttf_sbrk_reset(void)
{
    __ttf_heap = NULL;
}

void *_ttf_sbrk_reset(void) __attribute__ ((weak, alias("ttf_sbrk_reset")));

void *ttf_sbrk(unsigned int size)
{
    void *ptr;

    // NOTE: we assume all allocated address are 4 byte-aligned!!!
    size = (size + 3) & (~3);

    if ( __ttf_heap == NULL )
    {
        //initial memory space
        if ( u32TTF_Memory_Pool_Addr == 0 )
        {
            __ttf_heap = (unsigned char *)malloc(u32TTF_Memory_Pool_Size);
            u32TTF_Memory_Pool_Addr = (unsigned long)__ttf_heap;
        }
        else
        {
            __ttf_heap = (unsigned char *)(u32TTF_Memory_Pool_Addr);
        }
        if ( __ttf_heap == NULL )
            return TTF_MFAIL;

        __ttf_heap_end = __ttf_heap + u32TTF_Memory_Pool_Size;
        ttf_sbrk_top = __ttf_heap;
        //printf("__ttf_heap = 0x%x, __ttf_heap_end = 0x%x, ttf_sbrk_top = 0x%x\n", __ttf_heap, __ttf_heap_end, ttf_sbrk_top);
    }

    if (size > 0)
    {
        if ((ttf_sbrk_top + size) <= __ttf_heap_end)
        {
            ptr = ttf_sbrk_top;
            ttf_sbrk_top += size;
            return ptr;
        }
    }
    else if (size == 0)
    {
        return ttf_sbrk_top;
    }
    else
    {
        printf("ttf_sbrk: error\n");
        // we don't currently support shrink behavior
    }

    //printf("Memory Not Enough\n");
    return TTF_MFAIL;
}

unsigned long ttf_sbrk_size(void)
{
    return ((unsigned long)(__ttf_heap_end - __ttf_heap));
}

void ttf_sbrk_SetMemPoolBaseAddr(unsigned long u32Addr)
{
    u32TTF_Memory_Pool_Addr = u32Addr;
}

void ttf_sbrk_SetMemPoolSize(unsigned long u32Size)
{
    u32TTF_Memory_Pool_Size = u32Size;
}

void *_ttf_sbrk(unsigned int size) __attribute__ ((weak, alias("ttf_sbrk")));

////////////////////////////////////////////////////////////////////////////////////////////////
#define HASH_MFAIL       ((void *) -1)
#define HASH_MEMORY_POOL_SIZE        (1024*32)

unsigned char *__hash_heap = NULL;
unsigned char *__hash_heap_end = NULL;

static unsigned char * hash_sbrk_top = NULL;
static unsigned long u32HASH_Memory_Pool_Size = HASH_MEMORY_POOL_SIZE;
static unsigned long u32HASH_Memory_Pool_Addr = 0;

//extern void* malloc(size_t);

void *hash_sbrk(unsigned int size)
{
    void *ptr;

    // NOTE: we assume all allocated address are 4 byte-aligned!!!
    size = (size + 3) & (~3);

    if ( __hash_heap == NULL )
    {
        //initial memory space
        if ( u32HASH_Memory_Pool_Addr == 0 )
        {
            __hash_heap = (unsigned char *)malloc(u32HASH_Memory_Pool_Size);
            u32HASH_Memory_Pool_Addr = (unsigned long)__hash_heap;
        }
        else
        {
            __hash_heap = (unsigned char *)(u32HASH_Memory_Pool_Addr);
        }
        if ( __hash_heap == NULL )
            return HASH_MFAIL;

        __hash_heap_end = __hash_heap + u32HASH_Memory_Pool_Size;
        hash_sbrk_top = __hash_heap;
        //printf("__hash_heap = 0x%x, __hash_heap_end = 0x%x, hash_sbrk_top = 0x%x\n", __hash_heap, __hash_heap_end, hash_sbrk_top);
    }

    if (size > 0)
    {
        if ((hash_sbrk_top + size) <= __hash_heap_end)
        {
            ptr = hash_sbrk_top;
            hash_sbrk_top += size;
            return ptr;
        }
    }
    else if (size == 0)
    {
        return hash_sbrk_top;
    }
    else
    {
        printf("hash_sbrk: error\n");
        // we don't currently support shrink behavior
    }

    //printf("Memory Not Enough\n");
    return HASH_MFAIL;
}

unsigned long hash_sbrk_size(void)
{
    return ((unsigned long)(__hash_heap_end - __hash_heap));
}

void hash_sbrk_SetMemPoolBaseAddr(unsigned long u32Addr)
{
    u32HASH_Memory_Pool_Addr = u32Addr;
}

void hash_sbrk_SetMemPoolSize(unsigned long u32Size)
{
    u32HASH_Memory_Pool_Size = u32Size;
}

void *_hash_sbrk(unsigned int size) __attribute__ ((weak, alias("hash_sbrk")));
