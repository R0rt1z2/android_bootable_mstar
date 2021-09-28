#include <stdio.h>
#include <assert.h>
#include "datatype.h"
#include "mhal_arch.h"

#define CREATE_MASK(msb, lsb)               (((1U << ((msb) - (lsb) + 1)) - 1) << (lsb))
#define MASK_BITS(arg, msb, lsb)            ((arg) & CREATE_MASK(msb, lsb))
#define EXTRACT_BITS(arg, msb, lsb)         (MASK_BITS(arg, msb, lsb) >> (lsb))
#define INSERT_BITS(arg, msb, lsb, value) \
    ((arg) = ((arg) & ~CREATE_MASK(msb, lsb)) | (((value) << (lsb)) & CREATE_MASK(msb, lsb)))

#define MASK_FIELD(arg, field)              MASK_BITS(arg, field##_MSB, field##_LSB)
#define EXTRACT_FIELD(arg, field)           EXTRACT_BITS(arg, field##_MSB, field##_LSB)
#define INSERT_FIELD(arg, field, value)     INSERT_BITS(arg, field##_MSB, field##_LSB, value)

#define SET_BIT(arg, bit)                   ((arg) |= (1 << (bit)))
#define CLEAR_BIT(arg, bit)                 ((arg) &= ~(1 << (bit)))
#define TEST_BIT(arg, bit)                  ((arg) & (1 << (bit)))

typedef struct
{
    unsigned int va;
    unsigned int pa;
    unsigned int size;
    int         access;
    int         attributes;
}
_cpu_memory_t;

#define MMU_AP_NONE         0
#define MMU_AP_PRIVILEGED   1
#define MMU_AP_READ         2
#define MMU_AP_FULL         3

#define MMU_BUFFERED        1
#define MMU_CACHED          2

#if (CHIP_FAMILY_TYPE == CHIP_FAMILY_M3822)

#define L2_CACHE_ENABLE     1

#if (L2_CACHE_ENABLE)

//#################################################################
//#################################################################
//#################################################################
// Invalidate/Flush Range : pass physical address
extern void   MAsm_CPU_Sync(void);
#define BIT(_bit_)                  (1 << (_bit_))

#define L2X0_CACHE_ID           0x000
#define L2X0_CACHE_TYPE         0x004
#define L2X0_CTRL               0x100
#define L2X0_AUX_CTRL           0x104
#define L310_TAG_LATENCY_CTRL   0x108
#define L310_DATA_LATENCY_CTRL  0x10C
#define L2X0_EVENT_CNT_CTRL     0x200
#define L2X0_EVENT_CNT1_CFG     0x204
#define L2X0_EVENT_CNT0_CFG     0x208
#define L2X0_EVENT_CNT1_VAL     0x20C
#define L2X0_EVENT_CNT0_VAL     0x210
#define L2X0_INTR_MASK          0x214
#define L2X0_MASKED_INTR_STAT   0x218
#define L2X0_RAW_INTR_STAT      0x21C
#define L2X0_INTR_CLEAR         0x220
#define L2X0_CACHE_SYNC         0x730
#define L2X0_DUMMY_REG          0x740
#define L2X0_INV_LINE_PA        0x770
#define L2X0_INV_WAY            0x77C
#define L2X0_CLEAN_LINE_PA      0x7B0
#define L2X0_CLEAN_LINE_IDX     0x7B8
#define L2X0_CLEAN_WAY          0x7BC
#define L2X0_CLEAN_INV_LINE_PA  0x7F0
#define L2X0_CLEAN_INV_LINE_IDX 0x7F8
#define L2X0_CLEAN_INV_WAY      0x7FC

/*
 * The lockdown registers repeat 8 times for L310, the L210 has only one
 * D and one I lockdown register at 0x0900 and 0x0904.
 */
#define L2X0_LOCKDOWN_WAY_D_BASE 0x900
#define L2X0_LOCKDOWN_WAY_I_BASE 0x904
#define L2X0_LOCKDOWN_STRIDE     0x08
#define L310_ADDR_FILTER_START   0xC00
#define L310_ADDR_FILTER_END     0xC04
#define L2X0_TEST_OPERATION      0xF00
#define L2X0_LINE_DATA           0xF10
#define L2X0_LINE_TAG            0xF30
#define L2X0_DEBUG_CTRL          0xF40
#define L310_PREFETCH_CTRL       0xF60
#define L310_POWER_CTRL          0xF80
#define L310_DYNAMIC_CLK_GATING_EN (1 << 1)
#define L310_STNDBY_MODE_EN        (1 << 0)

/* Registers shifts and masks */
#define L2X0_CACHE_ID_PART_MASK   (0xf << 6)
#define L2X0_CACHE_ID_PART_L210   (1 << 6)
#define L2X0_CACHE_ID_PART_L220   (2 << 6)
#define L2X0_CACHE_ID_PART_L310   (3 << 6)
#define L2X0_CACHE_ID_RTL_MASK    0x3f
#define L210_CACHE_ID_RTL_R0P2_02 0x00
#define L210_CACHE_ID_RTL_R0P1    0x01
#define L210_CACHE_ID_RTL_R0P2_01 0x02
#define L210_CACHE_ID_RTL_R0P3    0x03
#define L210_CACHE_ID_RTL_R0P4    0x0b
#define L210_CACHE_ID_RTL_R0P5    0x0f
#define L220_CACHE_ID_RTL_R1P7_01REL0 0x06
#define L310_CACHE_ID_RTL_R0P0    0x00
#define L310_CACHE_ID_RTL_R1P0    0x02
#define L310_CACHE_ID_RTL_R2P0    0x04
#define L310_CACHE_ID_RTL_R3P0    0x05
#define L310_CACHE_ID_RTL_R3P1    0x06
#define L310_CACHE_ID_RTL_R3P1_50REL0 0x07
#define L310_CACHE_ID_RTL_R3P2    0x08
#define L310_CACHE_ID_RTL_R3P3    0x09
#define L2X0_EVENT_CNT_CTRL_ENABLE      BIT(0)
#define L2X0_EVENT_CNT_CFG_SRC_SHIFT    2
#define L2X0_EVENT_CNT_CFG_SRC_MASK     0xf
#define L2X0_EVENT_CNT_CFG_SRC_DISABLED 0
#define L2X0_EVENT_CNT_CFG_INT_DISABLED 0
#define L2X0_EVENT_CNT_CFG_INT_INCR     1
#define L2X0_EVENT_CNT_CFG_INT_OVERFLOW	2

/* L2C-310 specific bits */
#define L310_AUX_CTRL_FULL_LINE_ZERO    BIT(0) /* R2P0+ */
#define L310_AUX_CTRL_HIGHPRIO_SO_DEV   BIT(10) /* R2P0+ */
#define L310_AUX_CTRL_STORE_LIMITATION  BIT(11) /* R2P0+ */
#define L310_AUX_CTRL_EXCLUSIVE_CACHE   BIT(12)
#define L310_AUX_CTRL_ASSOCIATIVITY_16  BIT(16)
#define L310_AUX_CTRL_CACHE_REPLACE_RR  BIT(25) /* R2P0+ */
#define L310_AUX_CTRL_NS_LOCKDOWN       BIT(26)
#define L310_AUX_CTRL_NS_INT_CTRL       BIT(27)
#define L310_AUX_CTRL_DATA_PREFETCH     BIT(28)
#define L310_AUX_CTRL_INSTR_PREFETCH    BIT(29)
#define L310_AUX_CTRL_EARLY_BRESP       BIT(30) /* R2P0+ */

#define MP_PLATFORM_ARM     0
#define HAL_MMIO_L2CACHE_BASE 0x15000000
#define L2X0_CACHE_LINE_SIZE           32
#define L2X0_CACHE_WAY_MASK         0xFF
#define L2X0_CTRL_EN                        1
#define L2X0_WAY_SIZE_SHIFT             3

static void writel_relaxed (U32 u32Reg, U32 u32Value);
static U32 readl_relaxed (U32 u32Reg);
static void L2C_wait_mask(U32 u32Reg, U32 u32Mask);
static void L2C_write_sec(U32 u32Reg, U32 u32Val);
static void __L2C_op_way(U32 u32Reg);
static void __L2C210_cache_sync(void);
static void __L2C210_op_pa_range(U32 u32Reg, U32 u32Start, U32 u32End);
static int L2C_enabled(void);
static void L2C_configure(void);
static void L2C_unlock(U32 num);
void L2C_inv_range(U32 u32Start, U32 u32End);
void L2C_clean_range(U32 u32Start, U32 u32End);
void L2C_flush_range(U32 u32Start, U32 u32End);
void L2C_flush_all(void);
void L2C_sync(void);
void L2C_enable(void);
void L2C_disable(void);

static void writel_relaxed (U32 u32Reg, U32 u32Value)//L2 cache write by register offset
{
    *(volatile U32 *)(HAL_MMIO_L2CACHE_BASE + u32Reg) = u32Value;
}

static U32 readl_relaxed (U32 u32Reg)//L2 cache read by register offset
{
    return (U32)(*(volatile U32 *)(HAL_MMIO_L2CACHE_BASE + u32Reg));
}

static void L2C_wait_mask(U32 u32Reg, U32 u32Mask)
{
    /* wait for cache operation by line or way to complete */
    while (readl_relaxed(u32Reg) & u32Mask)
    {
        //loop here
    }
}

static void L2C_write_sec(U32 u32Reg, U32 u32Val)
{
    if (u32Val == readl_relaxed(u32Reg))
        return;
    writel_relaxed(u32Reg,u32Val);
}

static void __L2C_op_way(U32 u32Reg)
{
    writel_relaxed(u32Reg,L2X0_CACHE_WAY_MASK);
    L2C_wait_mask(u32Reg, L2X0_CACHE_WAY_MASK);
}

static void __L2C210_cache_sync(void)
{
    writel_relaxed(L2X0_CACHE_SYNC, 0);
    #if (MP_PLATFORM_ARM == 1)
    _chip_flush_miu_pipe();
    #endif
}

static void __L2C210_op_pa_range(U32 u32Reg, U32 u32Start, U32 u32End)
{
    while (u32Start < u32End) {
        writel_relaxed(u32Reg, u32Start);
        u32Start += L2X0_CACHE_LINE_SIZE;
    }
}

static int L2C_enabled(void)
{
	return (readl_relaxed(L2X0_CTRL) & L2X0_CTRL_EN);
}

static void L2C_configure(void)
{
    ///L2C_write_sec(L2X0_AUX_CTRL, l2x0_saved_regs.aux_ctrl);
    writel_relaxed(L2X0_AUX_CTRL, 0x76420001);
    writel_relaxed(L310_TAG_LATENCY_CTRL, 0x00000121);
    writel_relaxed(L310_DATA_LATENCY_CTRL, 0x00000121);
    writel_relaxed(L310_ADDR_FILTER_END, 0xf0000000);
    writel_relaxed(L310_ADDR_FILTER_START, 0xa0000001);
    writel_relaxed(L310_PREFETCH_CTRL, 0x78000000);
    writel_relaxed(L310_POWER_CTRL, 0x00000000);
}

static void L2C_unlock(U32 num)
{
    U32 i;

    for (i = 0; i < num; i++) {
        writel_relaxed(L2X0_LOCKDOWN_WAY_D_BASE + i * L2X0_LOCKDOWN_STRIDE, 0);
        writel_relaxed(L2X0_LOCKDOWN_WAY_I_BASE + i * L2X0_LOCKDOWN_STRIDE, 0);
    }
}

void L2C_inv_range(U32 u32Start, U32 u32End)
{
    if (u32Start & (L2X0_CACHE_LINE_SIZE - 1)) {
        u32Start &= ~(L2X0_CACHE_LINE_SIZE - 1);
        writel_relaxed(L2X0_CLEAN_INV_LINE_PA, u32Start);
        u32Start += L2X0_CACHE_LINE_SIZE;
    }

    if (u32End & (L2X0_CACHE_LINE_SIZE - 1)) {
        u32End &= ~(L2X0_CACHE_LINE_SIZE - 1);
        writel_relaxed(L2X0_CLEAN_INV_LINE_PA, u32End);
    }

    __L2C210_op_pa_range(L2X0_INV_LINE_PA, u32Start, u32End);
    __L2C210_cache_sync();
}

void L2C_clean_range(U32 u32Start, U32 u32End)
{
    u32Start &= ~(L2X0_CACHE_LINE_SIZE - 1);
    __L2C210_op_pa_range(L2X0_CLEAN_LINE_PA, u32Start, u32End);
    __L2C210_cache_sync();
}

void L2C_flush_range(U32 u32Start, U32 u32End)
{
    u32Start &= ~(L2X0_CACHE_LINE_SIZE - 1);
    __L2C210_op_pa_range(L2X0_CLEAN_INV_LINE_PA, u32Start, u32End);
    __L2C210_cache_sync();
}

void L2C_flush_all(void)
{
    __L2C_op_way(L2X0_CLEAN_INV_WAY);
    __L2C210_cache_sync();
}

void L2C_sync(void)
{
    __L2C210_cache_sync();
}

/*
 * Enable the L2 cache controller.  This function must only be
 * called when the cache controller is known to be disabled.
 */
void L2C_enable(void)
{
    #define NUM_LOCK    8

    //. check if L2 enabled
    if(L2C_enabled())
    {
        //do nothing if L2 has been enabled
        return;
    }

    //1. configure
    L2C_configure();

    //2. unlock : 0x15000900 , 0x15000904 , ¡K[NUM = 8]
    if  (readl_relaxed (L2X0_AUX_CTRL) & L310_AUX_CTRL_NS_LOCKDOWN)
    {
        L2C_unlock(NUM_LOCK);
    }

    //3.invalidate : 0x1500077c
    __L2C_op_way(L2X0_INV_WAY);

    //4. do sync + Wait
    writel_relaxed (L2X0_CACHE_SYNC, 0);
    L2C_wait_mask(L2X0_CACHE_SYNC, 1);

    //5. enable: 0x15000100 ¼g¤J0x1
    L2C_write_sec(L2X0_CTRL, L2X0_CTRL_EN);

}

void L2C_disable(void)
{
    L2C_flush_all();
    L2C_write_sec(L2X0_CTRL, 0);
    MAsm_CPU_Sync();
}
//#################################################################
//#################################################################
//#################################################################
#endif
#endif

extern void v7_dma_flush_range(U32 u32Start, U32 u32End);

void mhal_dcache_enable(void)
{
    unsigned long control;

    /* Enable data cache */
    asm volatile ("mrc p15, 0, %0, c1, c0, 0\n"
                  "orr %0, %0, #0x04\n"
                  "mcr p15, 0, %0, c1, c0, 0\n"
                  : "=r" (control)
                  :
                  : "memory");
}

void mhal_dcache_disable(void)
{
    unsigned long control;

    /* Disable data cache */
    asm volatile ("mrc p15, 0, %0, c1, c0, 0 \n"
                  "bic %0, %0, #0x04 \n"
                  "mcr p15, 0, %0, c1, c0, 0 \n"
                  : "=r" (control)
                  :
                  : "memory");
}

/* MIU pipe flush */
extern void HAL_MMIO_FlushMemory(void);

void mhal_dcache_flush(U32 u32Base, U32 u32Size)
{
    v7_dma_flush_range(u32Base, u32Base + u32Size);

  #if (CHIP_FAMILY_TYPE == CHIP_FAMILY_M3822)
    #if (L2_CACHE_ENABLE)
    L2C_flush_range(u32Base, u32Base + u32Size);
    #endif
  #endif

    HAL_MMIO_FlushMemory();
}

#define LEVEL_1_ADDRESS(va,ttb) (MASK_BITS(ttb, 31, 14) | (EXTRACT_BITS(va, 31, 20) << 2))


/*! \brief  Reads the translation table base address
 *
 *  \return Translation table base address
 *
 */
static unsigned int __cpu_mmu_ttb_read (void)
{
    unsigned int ttb;

    asm volatile ("mrc p15, 0, %0, c2, c0, 0"
                   : "=r" (ttb)
                   :
                   : "memory");

    return ttb;
}

/*! \brief  Writes the domain register
 *
 *  \param  domains The domain access control register data
 *
 */
static void __cpu_mmu_domains_write (unsigned int domains)
{
    asm volatile ("mcr p15, 0, %0, c3, c0, 0"
                   :
                   : "r" (domains)
                   : "memory");
}
/*! \brief  Creates a section translation table entry.
 *
 * Taking the virtual section address, physical address, access permissions and attributes
 * a level 1 section entry is defined in the translation table.
 *
 *  \param  va          Virtual Address
 *  \param  pa          Physical Address
 *  \param  access      Access Permissions
 *  \param  attributes  Cacheable / Bufferable
 *
 */
static void __cpu_mmu_create_section (unsigned int va, unsigned int pa, int access, int attributes)
{
    unsigned int ttb;
    unsigned int *ptr;

    ttb  = __cpu_mmu_ttb_read ();
    ptr  = (unsigned int *)LEVEL_1_ADDRESS(va,ttb);
    *ptr = pa | (access << 10) | (attributes << 2) | 2;

}

/*
** TO BE IMPLEMENTED
*/
/*
static void __cpu_mmu_create_large_page (unsigned int va, unsigned int pa, int access, int attributes)
{
    va = va;
    pa = pa;
    access = access;
    attributes = attributes;
}
*/
/*
** TO BE IMPLEMENTED
*/
/*
static void __cpu_mmu_create_small_page (unsigned int va, unsigned int pa, int access, int attributes)
{
    va = va;
    pa = pa;
    access = access;
    attributes = attributes;
}
*/
/*! \brief  Creates a MMU table entry
 *
 * Takes a pointer to a memory descriptor (size, virtual address, physical address,
 * access permissions and attributes) and creates the required translation table
 * entries.
 *
 *  \param  entry   Pointer to the table entry
 *
 */
void _cpu_mmu_map_memory (_cpu_memory_t *entry)
{
    unsigned int size;
    unsigned int va;
    unsigned int pa;
    int access;
    int attributes;

    size        = entry->size;
    va          = entry->va;
    pa          = entry->pa;
    access      = entry->access;
    attributes  = entry->attributes;

#if 1
    /*
    ** Round up to MB so that we only use sections
    */
    size += 0x000FFFFF;
    size &= ~0x000FFFFF;
#endif

    while (size > 0)
    {   // Generate the page tables
        int temp = 0;

        __cpu_mmu_create_section (va, pa, access, attributes);
        temp = 0x00100000;

        va += temp;
        pa += temp;
        size -= temp;
    }
}

/*! \brief  Initializes the MMU
 *
 * Initializes the MMU, generating a background 4GB flat mapped full access,
 * non cacheable non bufferable page table then overlay that with cached and
 * buffered SDRAM entries.
 *
 */
void mhal_init_mmu(void)
{

    _cpu_memory_t entry;
#if defined(MBOOT)
    entry.pa            = 0x1F000000;
    entry.va            = 0x1F000000;
    entry.size          = 0x01000000; // RIU(16M)
    entry.access        = MMU_AP_FULL;
    entry.attributes    = 0;

    _cpu_mmu_map_memory (&entry);

    entry.pa            = 0x15000000;
    entry.va            = 0x15000000;
    entry.size          = 0x00100000; // ARM RIU(16M)
    entry.access        = MMU_AP_FULL;
    entry.attributes    = 0;

    _cpu_mmu_map_memory (&entry);

    entry.pa            = 0x16000000;
    entry.va            = 0x16000000;
    entry.size          = 0x00100000; // ARM RIU(16M)
    entry.access        = MMU_AP_FULL;
    entry.attributes    = 0;

    _cpu_mmu_map_memory (&entry);

    entry.pa            = 0x14000000;
    entry.va            = 0x14000000;
    entry.size          = 0x01000000; // XIU SPI flash(16M)
    entry.access        = MMU_AP_FULL;
    entry.attributes    = MMU_CACHED | MMU_BUFFERED;

    _cpu_mmu_map_memory (&entry);

#if (CHIP_FAMILY_TYPE == CHIP_FAMILY_M3822)
    entry.pa            = MIU0_BUS_BASE;
    entry.va            = MIU0_BUS_BASE;
    entry.size          = MIU0_MAP_LEN; // MIU0 - cached
    entry.access        = MMU_AP_FULL;
    entry.attributes    = MMU_CACHED | MMU_BUFFERED;

    _cpu_mmu_map_memory (&entry);


    entry.pa            = MIU0_BUS_BASE;
    entry.va            = MIU0_BUS_BASE +MIU0_MAP_LEN;
    entry.size          = MIU0_MAP_LEN; // MIU0 - uncached
    entry.access        = MMU_AP_FULL;
    entry.attributes    = MMU_BUFFERED;

    _cpu_mmu_map_memory (&entry);
#else
    entry.pa            = MIU0_BUS_BASE;
    entry.va            = MIU0_BUS_BASE;
    entry.size          = MIU0_MAP_LEN; // MIU0 - cached
    entry.access        = MMU_AP_FULL;
    entry.attributes    = MMU_CACHED | MMU_BUFFERED;

    _cpu_mmu_map_memory (&entry);


    entry.pa            = MIU0_BUS_BASE;
    entry.va            = MIU0_BUS_BASE +MIU0_MAP_LEN;
    entry.size          = MIU0_MAP_LEN; // MIU0 - uncached
    entry.access        = MMU_AP_FULL;
    entry.attributes    = MMU_BUFFERED;

    _cpu_mmu_map_memory (&entry);


    entry.pa            = MIU1_BUS_BASE;
    entry.va            = MIU1_BUS_BASE;
    entry.size          = 0x20000000; // MIU1 - cached
    entry.access        = MMU_AP_FULL;
    entry.attributes    = MMU_CACHED | MMU_BUFFERED;

    _cpu_mmu_map_memory (&entry);

    entry.pa            = MIU1_BUS_BASE;
    entry.va            = MIU1_BUS_BASE + 0x20000000;
    entry.size          = MIU1_MAP_LEN; // MIU1 - uncached
    entry.access        = MMU_AP_FULL;
    entry.attributes    = MMU_BUFFERED;
#endif
#else //#ifdef MBOOT
    entry.pa            = 0x1F000000;
    entry.va            = 0xFD000000;
    entry.size          = 0x01000000; // RIU(16M)
    entry.access        = MMU_AP_FULL;
    entry.attributes    = 0;

    _cpu_mmu_map_memory (&entry);

    entry.pa            = 0x15000000;
    entry.va            = 0xFB000000;
    entry.size          = 0x00100000; // ARM RIU(16M)
    entry.access        = MMU_AP_FULL;
    entry.attributes    = 0;

    _cpu_mmu_map_memory (&entry);

    entry.pa            = 0x16000000;
    entry.va            = 0xFC000000;
    entry.size          = 0x00100000; // ARM RIU(16M)
    entry.access        = MMU_AP_FULL;
    entry.attributes    = 0;

    _cpu_mmu_map_memory (&entry);

    entry.pa            = 0x14000000;
    entry.va            = 0xFE000000;
    entry.size          = 0x01000000; // XIU SPI flash(16M)
    entry.access        = MMU_AP_FULL;
    entry.attributes    = MMU_CACHED | MMU_BUFFERED;

    _cpu_mmu_map_memory (&entry);

    entry.pa            = MIU0_BUS_BASE + 0x100000;
    entry.va            = 0x00100000;
    entry.size          = 0x1FF00000; // MIU0 - cached
    entry.access        = MMU_AP_FULL;
    entry.attributes    = MMU_CACHED | MMU_BUFFERED;

    _cpu_mmu_map_memory (&entry);

    entry.pa            = MIU0_BUS_BASE;
    entry.va            = 0x20000000;
    entry.size          = 0x1FF00000; // MIU0 - uncached
    entry.access        = MMU_AP_FULL;
    entry.attributes    = MMU_BUFFERED;

    _cpu_mmu_map_memory (&entry);

    entry.pa            = MIU1_BUS_BASE;
    entry.va            = 0x40000000;
    entry.size          = 0x20000000; // MIU1 - cached
    entry.access        = MMU_AP_FULL;
    entry.attributes    = MMU_CACHED | MMU_BUFFERED;

    _cpu_mmu_map_memory (&entry);

    entry.pa            = MIU1_BUS_BASE;
    entry.va            = 0x60000000;
    entry.size          = 0x20000000; // MIU1 - uncached
    entry.access        = MMU_AP_FULL;
    entry.attributes    = MMU_BUFFERED;
#endif

    _cpu_mmu_map_memory (&entry);

    /*
    ** We are only using domain 0.
    */
    __cpu_mmu_domains_write (1);
}

#if defined(CHAKRA)
void mhal_init_mmu_cfgmiusize(U32 u32SizeMiu0, U32 u32SizeMiu1)
{
    #define entrysize_mask     0x00000003 //minimum page table size: 4 bytes
    #define entrysize_max0     0x40000000 //limit miu0 maximum as 1024MB bytes
    #define entrysize_max1     0x20000000 //limit miu1 maximum as 512MB bytes
    _cpu_memory_t entry;
    U32 u32EntrySize0=(((u32SizeMiu0 & entrysize_mask)==0)? u32SizeMiu0: (((u32SizeMiu0>>2)+1)<<2));
    U32 u32EntrySize1=(((u32SizeMiu1 & entrysize_mask)==0)? u32SizeMiu1: (((u32SizeMiu1>>2)+1)<<2));

    if(u32EntrySize0 > entrysize_max0) u32EntrySize0=entrysize_max0;
    if(u32EntrySize1 > entrysize_max1) u32EntrySize1=entrysize_max1;

    entry.pa            = 0x1F000000;
    entry.va            = 0x1F000000;
    entry.size          = 0x01000000; // RIU(16M)
    entry.access        = MMU_AP_FULL;
    entry.attributes    = 0;
    _cpu_mmu_map_memory (&entry);

    entry.pa            = 0x15000000;
    entry.va            = 0x15000000;
    entry.size          = 0x00100000; // ARM RIU(16M)
    entry.access        = MMU_AP_FULL;
    entry.attributes    = 0;
    _cpu_mmu_map_memory (&entry);

    entry.pa            = 0x16000000;
    entry.va            = 0x16000000;
    entry.size          = 0x00100000; // ARM RIU(16M)
    entry.access        = MMU_AP_FULL;
    entry.attributes    = 0;
    _cpu_mmu_map_memory (&entry);

    entry.pa            = 0x14000000;
    entry.va            = 0x14000000;
    entry.size          = 0x01000000; // XIU SPI flash(16M)
    entry.access        = MMU_AP_FULL;
    entry.attributes    = MMU_CACHED | MMU_BUFFERED;
    _cpu_mmu_map_memory (&entry);

    entry.pa            = MIU0_BUS_BASE;
    entry.va            = MIU0_BUS_BASE;
    entry.size          = u32EntrySize0; // MIU0 - cached
    entry.access        = MMU_AP_FULL;
    entry.attributes    = MMU_CACHED | MMU_BUFFERED;
    _cpu_mmu_map_memory (&entry);

    entry.pa            = MIU0_BUS_BASE;
    entry.va            = MIU0_BUS_BASE +MIU0_MAP_LEN;
    entry.size          = u32EntrySize0; // MIU0 - uncached
    entry.access        = MMU_AP_FULL;
    entry.attributes    = MMU_BUFFERED;
    _cpu_mmu_map_memory (&entry);

#if 0
    entry.pa            = MIU1_BUS_BASE;
    entry.va            = MIU1_BUS_BASE;
    entry.size          = u32EntrySize1; // MIU1 - cached
    entry.access        = MMU_AP_FULL;
    entry.attributes    = MMU_CACHED | MMU_BUFFERED;
    _cpu_mmu_map_memory (&entry);

    entry.pa            = MIU1_BUS_BASE;
    entry.va            = MIU1_BUS_BASE + 0x20000000;
    entry.size          = u32EntrySize1; // MIU1 - uncached
    entry.access        = MMU_AP_FULL;
    entry.attributes    = MMU_BUFFERED;
    _cpu_mmu_map_memory (&entry);
#endif

    /*
    ** We are only using domain 0.
    */
    __cpu_mmu_domains_write (1);

  #if (CHIP_FAMILY_TYPE == CHIP_FAMILY_M3822)
   #if 0//(L2_CACHE_ENABLE)
    L2C_enable();
   #endif
  #endif

}
#endif

void mhal_L2Cache_Flush(void)
{
}

void mhal_L2Cache_Read(void)
{
}
