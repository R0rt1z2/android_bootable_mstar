#include <stddef.h>
#include <stdio.h>
#include <assert.h>
#include <datatype.h>
#include <mhal_arch.h>
#include <pgtable.h>
#include <tlbflush.h>

typedef unsigned long u64;

extern u64 _ttb1_base_addr;
extern u64 _ttb0_base_addr;

#define CREATE_MASK(msb, lsb)               (((1UL << ((msb) - (lsb) + 1)) - 1) << (lsb))
#define MASK_BITS(arg, msb, lsb)            ((arg) & CREATE_MASK(msb, lsb))
#define EXTRACT_BITS(arg, msb, lsb)         (MASK_BITS(arg, msb, lsb) >> (lsb))

#define MASK_FIELD(arg, field)              MASK_BITS(arg, field##_MSB, field##_LSB)
#define EXTRACT_FIELD(arg, field)           EXTRACT_BITS(arg, field##_MSB, field##_LSB)
#define INSERT_FIELD(arg, field, value)     INSERT_BITS(arg, field##_MSB, field##_LSB, value)

#define SET_BIT(arg, bit)                   ((arg) |= (1 << (bit)))
#define CLEAR_BIT(arg, bit)                 ((arg) &= ~(1 << (bit)))
#define TEST_BIT(arg, bit)                  ((arg) & (1 << (bit)))

typedef struct
{
    u64 va;
    u64 pa;
    u64 size;
    u64 attributes;
}
_cpu_memory_t;


void mhal_dcache_enable(void)
{
    return ;
}

void mhal_dcache_disable(void)
{
    return ;
}

/* MIU pipe flush */
extern void HAL_MMIO_FlushMemory(void);

void mhal_dcache_flush(U32 u32Base, U32 u32Size)
{
}

#define LEVEL_1_ADDRESS(va,ttb) (MASK_BITS(ttb, 31, 14) | (EXTRACT_BITS(va, 31, 20) << 2))


/*! \brief  Reads the translation table base address
 *
 *  \return Translation table base address
 *
 */
static unsigned int __cpu_mmu_ttb_read (void)
{
    return 0;
}

/*! \brief  Writes the domain register
 *
 *  \param  domains The domain access control register data
 *
 */
static void __cpu_mmu_domains_write (unsigned int domains)
{
    return ;
}

/*
 *
 * Taking the virtual section address, physical address, access permissions and attributes
 * a level 1 section entry is defined in the translation table.
 *
 *  \param  va          Virtual Address
 *  \param  pa          Physical Address
 *  \param  attributes  Cacheable / Bufferable
 *
 */
static void __cpu_mmu_create_section (unsigned int va, unsigned int pa, int access, int attributes)
{
    return ;

}

void _cpu_mmu_map_memory (_cpu_memory_t *entry)
{
    u64 size;
    u64 va;
    u64 pa;
    u64 attributes;
    u64 va_end;
    u64 *ptep;
    u64 pte;
    u64 *pgdp;
    u64 pgd;
    u64 offset;

    size        = entry->size;
    va          = entry->va;
    pa          = entry->pa;
    attributes  = entry->attributes;
    va_end      = va + size;

    /* Align to PAGE_SIZE */
    if(MASK_BITS(va, PAGE_SHIFT -1, 0) != 0)
    {
        va =  (va << PAGE_SHIFT) >> PAGE_SHIFT;
    }
    
    if(MASK_BITS(pa, PAGE_SHIFT -1, 0) != 0)
    {
        pa =  (pa << PAGE_SHIFT) >> PAGE_SHIFT;
    }
    
    if(MASK_BITS(va_end, PAGE_SHIFT -1, 0) != 0)
    {
        va_end =  (va_end << PAGE_SHIFT) >> PAGE_SHIFT + PAGE_SIZE;
    }

    size = va_end - va;
    
    /* Create level 3 ttb */
    offset = 0;
    while (size > 0)
    { 
        ptep = (u64 *)(_ttb0_base_addr + (((va + offset) >> PAGE_SHIFT) << 3));
        pte = (((pa +  offset) >> PAGE_SHIFT) << PAGE_SHIFT) | attributes;
        *ptep = pte;
        
        offset += PAGE_SIZE;
        size -= PAGE_SIZE;
    }

    /* Create level 2 ttb */
    offset = 0;
    while (va_end > (va + offset))
    {
        pgdp = (u64 *)(_ttb1_base_addr + (((va + offset) >> PGDIR_SHIFT) << 3));
        pgd = (_ttb0_base_addr +  (((va + offset) >> PGDIR_SHIFT) * TTB0_SIZE)) | 0x3;
        *pgdp = pgd;
        
        offset += (1 << PGDIR_SHIFT);
    }

    flush_cache_all();
    flush_tlb_all();
}

void mhal_init_mmu(void)
{
    _cpu_memory_t entry;
   
    /* XIU SPI flash(16M) */
    entry.va            = 0x14000000;
    entry.pa            = 0x14000000;
    entry.size          = 0x01000000; 
    entry.attributes    = PTE_ATTRINDX(MT_DEVICE_nGnRnE) | PTE_FLAGS;
    _cpu_mmu_map_memory (&entry);
    
    /* ARM GIC register(16M) */
    entry.va            = 0x16000000;
    entry.pa            = 0x16000000;
    entry.size          = 0x00100000; 
    entry.attributes    = PTE_ATTRINDX(MT_DEVICE_nGnRnE) | PTE_FLAGS;
    _cpu_mmu_map_memory (&entry);
    
    /* MStar RIU(16M) */
    entry.va            = 0x1F000000;
    entry.pa            = 0x1F000000;
    entry.size          = 0x01000000; 
    entry.attributes    = PTE_ATTRINDX(MT_DEVICE_nGnRnE) | PTE_FLAGS;
    _cpu_mmu_map_memory (&entry);
    
    /* MIU0 - cached  */
    entry.va            = MIU0_BUS_BASE;
    entry.pa            = MIU0_BUS_BASE;
    entry.size          = MIU0_MAP_LEN; 
    entry.attributes    = PTE_ATTRINDX(MT_NORMAL) | PTE_FLAGS;
    _cpu_mmu_map_memory (&entry);

    /* MIU0 - uncached */
    entry.va            = MIU0_BUS_BASE + MIU0_MAP_LEN;
    entry.pa            = MIU0_BUS_BASE;
    entry.size          = MIU0_MAP_LEN; 
    entry.attributes    = PTE_ATTRINDX(MT_NORMAL_NC) | PTE_FLAGS;
    _cpu_mmu_map_memory (&entry);

    /* MIU1 - cached */
    entry.pa            = MIU1_BUS_BASE;
    entry.va            = MIU1_BUS_BASE;
    entry.size          = 0x20000000; 
    entry.attributes    = PTE_ATTRINDX(MT_NORMAL) | PTE_FLAGS;
    _cpu_mmu_map_memory (&entry);

    /* MIU1 - uncached */
    entry.va            = MIU1_BUS_BASE + MIU1_MAP_LEN;
    entry.pa            = MIU1_BUS_BASE;
    entry.size          = MIU1_MAP_LEN; 
    entry.attributes    = PTE_ATTRINDX(MT_NORMAL_NC) | PTE_FLAGS;
    _cpu_mmu_map_memory (&entry);
}

