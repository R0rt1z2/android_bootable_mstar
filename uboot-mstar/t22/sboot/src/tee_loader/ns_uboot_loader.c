
#include "../../../u-boot-2011.06/MstarApp/include/MsTrustZone.h"
#include "../seal/src/drvSEAL.h"
#include "../aesdma/src/drvAESDMA.h"
#if defined(CONFIG_MSTAR_ROM_BOOT_WITH_EMMC_FLASH)
#include "../mmc/inc/common/eMMC.h"
#include "../part_emmc.h"
#elif defined(CONFIG_MSTAR_ROM_BOOT_WITH_NAND_FLASH)
#include "../nand/inc/common/drvNAND.h"
#endif

#include "../MMAPInfo.h"
#include "../rpmb_config.h"

#if defined(__aarch64__)
#define UINT2PTR(x) ((void *)(MS_U64)x)
#define PTR2UINT(x) ((void *)(MS_U64)x)
#else
#define UINT2PTR(x) (x)
#define PTR2UINT(x) (x)
#endif

#define ALIGN(a,s) (((a)-((s) & ((a)-1))) & ((a)-1))


#define CONFIG_OPTEE_HEADER_LEN (sizeof(mstar_tee_t))
#define OPTEE_HEADER_MAGIC (0x4554504f)
#define EMMC_BLOCK_SIZE (512)
#define SEAL_ALIGN_BIT_WIDTH (12)

#define SEAL_ATTR_SECURE_RW (0x03)
#define SEAL_ATTR_ALL_RW (0x0F)

#ifdef CONFIG_MSTAR_M7221
#define MIU0_PA_BASE               (0x00000000UL)
#define MIU1_PA_BASE               (0x80000000UL)
#else
#define MIU0_PA_BASE               (0x00000000UL)
#define MIU1_PA_BASE               (0x80000000UL)
#endif
#define MIU2_PA_BASE               (0xC0000000UL)

#if defined(CONFIG_MSTAR_M7221) || defined(CONFIG_MSTAR_M7322)
//For UMA structures, they have low performance of memory RW, hence we use BDMA to increase copying performance
#define MEMCPY_BDMA 1
#endif

#if defined(CONFIG_MSTAR_M5621) || defined(CONFIG_MSTAR_M7322)
//This define is for temporary use, MCT can be enabled after the specified chip is functional tested
#define ENABLE_MCT 1
#endif

#define SIGNATURE_LEN               (256)
#define RSA_PUBLIC_KEY_N_LEN (256)
#define RSA_PUBLIC_KEY_E_LEN (4)
#define RSA_PUBLIC_KEY_LEN          (RSA_PUBLIC_KEY_N_LEN+RSA_PUBLIC_KEY_E_LEN)

#define TEEKEYBANK_OFFSET (0x2000)
#define REEKEYBANK_OFFSET (0x3000)

#define KEYBANK_BINARY_SIZE (0x400)
#define CUS_KEYBANK_BINARY_SIZE (512 * 3)
#define CHUNK_HEADER_SIZE (0x400)

#define AES_IV_LEN (16)
#define AES_KEY_LEN (16)
#define HMAC_KEY_LEN (32)

// Magic flag indicating anti-rollback fields in RPMB have been initialized
#define FLAG_ANTIROLLBACK_INITIALIZED (0x414e5449) //"ANTI"

#define ANTIROLLBACK_REG_ADDR (0x1f000c08)
#define ANTIROLLBACK_ENABLING_MAGIC (0xBABE)
#define ANTIROLLBACK_NOT_ENABLED_MAGIC (0xEBAB)

// Must be a positive integer less than 2^32-1
#define VERSION_NUMBER_TEELOADER (0x00000000)

struct rpmb_fs_partition {
    U32 rpmb_fs_magic;
    U32 fs_version;
    U32 write_counter;
    U32 fat_start_address;
    /* Do not use reserved[] for other purpose than partition data. */
    U8 reserved[88];
    U32 hash1_version;
    U32 sboot_version;
    U32 uboot_version;
    U32 optee_version;
    U32 armfw_version;
    U32 anti_rollback_init_flag;
};

typedef struct
{
    unsigned int ATFBase;
    unsigned int ATFSize;
    unsigned int ATFEnrty;
    unsigned int ATFFlag;
    unsigned char ATFMAC[16];
    unsigned int ATFDummy[4];
    unsigned int TLBase;
    unsigned int TLSize;
    unsigned int TLEnrty;
    unsigned int TLDummy;
} warmboot_table;

typedef struct
{
    unsigned int MultiCoreEnrty;
    unsigned int MultiCoreTrigger;
} multicore_table;

multicore_table _gMultiCoreTable __attribute__((aligned(4))) = {0};

typedef struct
{
    U32 u32Num;
    U32 u32Size;
}IMAGE_INFO;

typedef struct
{
  U8 u8SecIdentify[8];
  IMAGE_INFO info;
  U8 u8Signature[SIGNATURE_LEN];
}_SUB_SECURE_INFO;

typedef struct
{
    _SUB_SECURE_INFO customer;
    U8 u8RSABootPublicKey[RSA_PUBLIC_KEY_LEN];
    U8 u8RSAUpgradePublicKey[RSA_PUBLIC_KEY_LEN];
    U8 u8RSAImagePublicKey[RSA_PUBLIC_KEY_LEN];
    U8 u8AESBootKey[AES_KEY_LEN];
    U8 u8AESUpgradeKey[AES_KEY_LEN];
    U8 u8MagicID[16];
    U8 crc[4];
}CUSTOMER_KEY_BANK;

typedef struct
{
    _SUB_SECURE_INFO SecureInfo;
    U8 u8MSID[4];
    U32 u32TkbVersion;
    U8 u8RSATEEKey[RSA_PUBLIC_KEY_LEN];
    U8 u8AESTEEKey[AES_KEY_LEN];
} TEE_KEY_BANK;

typedef struct
{
    _SUB_SECURE_INFO SecureInfo;
    U8 u8RSAKey[RSA_PUBLIC_KEY_LEN];
    U8 u8AESKey[AES_KEY_LEN];
} REE_KEY_BANK;


typedef struct
{
    unsigned char MagicID[12];
    unsigned int NumberOfConfig;
    unsigned int TotalConfigSize; // without descriptor
    unsigned int ConfigVersion;
    unsigned char Reserved[8];
} OPTEE_CONFIG_HEADER;

typedef struct
{
    unsigned int ConfigID;
    unsigned int Offset;
    unsigned int ConfigSize;
    unsigned int Reserved;
} OPTEE_CONFIG_DESCRIPTOR;

typedef struct
{
    unsigned int NumberOfSecureGroup;
    unsigned int NumberOfMmapID;
} OPTEE_CONFIG_MMAP_HEADER;

typedef struct
{
    unsigned int addr;
    unsigned int length;
} ST_MMAP_GROUP_ITEM;

/*
typedef struct
{
    unsigned char valid;
    unsigned int addr;
    unsigned int length;
    unsigned char MiuSel;
    unsigned int index;
} ST_MMAP_DB_ITEM;
*/
typedef struct
{
    unsigned char Dummy1[10];
    unsigned short index;
    unsigned char Dummy2[2];
} ST_MMAP_DB_ITEM;

typedef struct
{
    unsigned int          eAlgo;
    unsigned int          eSrc;   // Select KL root key source //
    unsigned int          eDst;   // Select KL output Key destination //
    unsigned int  eOutsize;  // Select ouput Key size. ex: Key size of CSA and DES are E_DSCMB_KL_64_BITS//
    unsigned int       eKeyType;
    unsigned int               u32Level;
    unsigned int               u32EngID;  // Select ESA/NSA as KL destination, Dscmb engine ID //
    unsigned int               u32DscID;  // Select ESA/NSA as KL destination, Dscmb ID //
    unsigned long long   u8KeyACPU;  // Select ACPU as KL root Key, KL root key //
    unsigned long long   pu8KeyKLIn;
    unsigned char              bDecrypt;
    unsigned char              bInverse;
    unsigned int      eKLSel;   //Select KeyLadder
    unsigned int               u32CAVid; //Set CAVid
    unsigned int         stKDF[3];
    unsigned int           eFSCB; // Key-specific FSCB
} DSCMB_KLCfg_All64;

void ns_uboot_boot(unsigned int uboot_addr);
unsigned int ns_uboot_load_optee(void);
unsigned int ns_uboot_load_armfw(void);
unsigned long long __BA2PA(unsigned long long u64BusAddr);
unsigned int ns_uboot_get_optee_addr(void);
unsigned int ns_uboot_nonsecure_handler(void);

Rpmb_Config RpmbConfig = {0};

// For STR
static int _gIsStr = 0;
static int _gStrMode = COLD_BOOT;
static int _gAuthMode = 0;

//For OpteeOS consistency checking
#if defined(CONFIG_OPTEE_ANTIBRICK)
static char _gOpteeConsisStatus = 0;
#define RTC_BANK 0x1F002440
typedef enum
{
	OPTEENEVER=0,
	OPTEECHECKIN,
	OPTEECHECKOUT,
}OpteeConsisStatus;
#endif

static unsigned long long BIN0_entry_point = 0;
static unsigned long Fireware_entry = 0;
static unsigned short _gHash1Version = 0;
static char _gOpteeBootArgs[72] __attribute__((aligned(8)));
static unsigned int _gKeybankRomOffset;
static CUSTOMER_KEY_BANK _gKeybank;
static TEE_KEY_BANK _gTEEKeybank __attribute__((aligned(8)));
static REE_KEY_BANK _gREEKeybank __attribute__((aligned(8)));
const unsigned char EmbeddedTeeRsaPub[] __attribute__((aligned(16)))= {
    #include "RSAcus_pub.dat"
};
const unsigned char EmbeddedReeRsaPub[] __attribute__((aligned(16)))= {
    #include "RSAree_pub.dat"
};
unsigned char EmbeddedAESTEEUniformKey[AES_KEY_LEN] __attribute__((aligned(16)))={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
unsigned char EmbeddedAESREEUniformKey[] __attribute__((aligned(16)))= {
    #include "AES_REE_Uniform.dat"
};

#define TPMBUFFERSIZE 512*7
static const U8 u8TmpBuf[TPMBUFFERSIZE] __attribute__((aligned(16)));
static const U8 u8TmpBuf1[CUS_KEYBANK_BINARY_SIZE] __attribute__((aligned(16)));


static U8 u8ATF_MAC[16] __attribute__((aligned(16)));

//Default seed for TKB encryption with efuse key
static U8 SeedOfTeeKeybank[16] __attribute__((aligned(4))) = { 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA };

static int authFailType = 0;
#define AUTH_KEYBANK_FAIL 1
#define AUTH_ARMFW_FAIL 2
#define AUTH_OPTEE_FAIL 3

static unsigned int _gOpteeVersion = 0;
static unsigned int _gArmFwVersion = 0;
static unsigned int * _gUbootVersionAddr = NULL;
// ***** ANTI_ROLLBACK SBOOT VERSION *****
static const unsigned int _gSbootVersion = 2;

#define IS_MBOOTBAK() ((*(volatile unsigned int*)(CONFIG_RIU_BASE_ADDRESS + (0x103380<<1)))&0x0100)

#define IS_SECURE_CHIP()    ((*(volatile unsigned short*)(CONFIG_RIU_BASE_ADDRESS + (0x038E0<<1)))&0x01)

#define RREG16(addr) (*(volatile unsigned short *)(addr))
#define WREG16(addr, value) (*(volatile unsigned short *)(addr) = (value))

// UART
#define REG_UART_BASE     (0x1F201300)
#define UART_LSR_THRE     (0x20)
#define UART_LSR_TEMT     (0x40)
#define REG_UART_RX       (REG_UART_BASE + (0 * 8))
#define REG_UART_TX       (REG_UART_BASE + (0 * 8))
#define REG_UART_LSR      (REG_UART_BASE + (5 * 8))

void LDR_SHOWTIME()
{
    int i = 0,piuMs = 0;
    long long piuClk=(RREG16(0x1F006090)|(RREG16(0x1F006094)<<16));
    for(i=0;piuClk>0;piuClk-=12000)
    {
        piuMs++;
    }
    LDR_PUTDW(piuMs);
}

void LDR_PUTC(int c)
{
    while((RREG16(REG_UART_LSR) & UART_LSR_THRE) != UART_LSR_THRE)
        ;
    WREG16(REG_UART_TX, c);
    while((RREG16(REG_UART_LSR) & (UART_LSR_TEMT | UART_LSR_THRE)) != (UART_LSR_TEMT | UART_LSR_THRE))
        ;
}

void LDR_PUTX(char val)
{
    if (val > 0x9)
        LDR_PUTC(0x41+(val-10));
    else
        LDR_PUTC(0x30+val);
}

void LDR_PUTDW(unsigned int val)
{
    unsigned char value;

    value = (val>>28)&0xF;
    LDR_PUTX(value);
    value = (val>>24)&0xF;
    LDR_PUTX(value);
    value = (val>>20)&0xF;
    LDR_PUTX(value);
    value = (val>>16)&0xF;
    LDR_PUTX(value);
    value = (val>>12)&0xF;
    LDR_PUTX(value);
    value = (val>>8)&0xF;
    LDR_PUTX(value);
    value = (val>>4)&0xF;
    LDR_PUTX(value);
    value = (val)&0xF;
    LDR_PUTX(value);
}

void LDR_PUTS(const char *s)
{
    while(*s) {
        if(*s == '\n')
            LDR_PUTC('\r');
        LDR_PUTC(*s++);
    }
}

static void *_memcpy(void *d, void *s, unsigned int n)
{

#ifdef MEMCPY_BDMA
    __BDMA_MIU0Copy2MIU0((U32)__BA2PA((unsigned long long)s), (U32)__BA2PA((unsigned long long)d), n);
#else
    int *dest = (int*)d;
    int *src = (int*)s;
    char *source_char = NULL;
    char *dst_char = NULL;
    int wordnum = n >> 2;
    int slice = n & 0x03;

    if(((unsigned int)s & 0x00000003)==0 && ((unsigned int)d & 0x00000003)==0)//Check if both of d and s are aligned to 4byte
    {
        while (wordnum-- > 0)
        {
            *dest++ = *src++;
        }
    }
    else//either d or s is not aligned to 4byte
    {
        slice = n;
    }

    source_char = (char*)src;
    dst_char = (char*)dest;
    while (slice-- > 0)
    {
        *(dst_char++) = *(source_char++);
    }
#endif
    return d;
}

static int _memcmp(const void * cs,const void * ct, unsigned int count)
{
    const unsigned char *su1, *su2;
    int res = 0;

    for( su1 = cs, su2 = ct; 0 < count; ++su1, ++su2, count--)
        if ((res = *su1 - *su2) != 0)
            break;
    return res;
}

static void *_memset (void *s, int c, unsigned int n)
{
    char *str = s;

    while (n-- > 0)
    {
        *str++ = c;
    }
    return s;
}

void array_reverse(char* ary, unsigned int len)
{
    int i, j;
    char tmp;
    for (i=0, j=len-1; i<j; ++i, --j)
    {
        tmp = ary[i];
        ary[i] = ary[j];
        ary[j] = tmp;
    }
}

unsigned long long __BA2PA(unsigned long long u64BusAddr)
{
    unsigned long long u64PhyAddr = 0x0;
#ifdef CONFIG_MSTAR_MASERATI
    if( (u64BusAddr >= CONFIG_MIU0_BUSADDR) && (u64BusAddr < CONFIG_MIU1_BUSADDR) ) // MIU0
        u64PhyAddr = u64BusAddr - CONFIG_MIU0_BUSADDR + MIU0_PA_BASE;
    else if( (u64BusAddr >= CONFIG_MIU0_BUSADDR) && (u64BusAddr < CONFIG_MIU2_BUSADDR) )// MIU1
        u64PhyAddr = u64BusAddr - CONFIG_MIU1_BUSADDR + MIU1_PA_BASE;
    else                                                                        //MIU2
        u64PhyAddr = u64BusAddr - CONFIG_MIU2_BUSADDR + MIU2_PA_BASE;
#else
    if( (u64BusAddr >= CONFIG_MIU0_BUSADDR) && (u64BusAddr < CONFIG_MIU1_BUSADDR) ) // MIU0
        u64PhyAddr = u64BusAddr - CONFIG_MIU0_BUSADDR + MIU0_PA_BASE;
    else if( (u64BusAddr >= CONFIG_MIU0_BUSADDR) )// MIU1
        u64PhyAddr = u64BusAddr - CONFIG_MIU1_BUSADDR + MIU1_PA_BASE;
#endif
    return u64PhyAddr;
}

unsigned long long __PA2BA(unsigned long long u64PhyAddr)
{
    unsigned long long u64BusAddr = 0x0;
#ifdef CONFIG_MSTAR_MASERATI
    if( (u64PhyAddr >= MIU0_PA_BASE) && (u64PhyAddr < MIU1_PA_BASE) ) // MIU0
        u64BusAddr = u64PhyAddr - MIU0_PA_BASE + CONFIG_MIU0_BUSADDR;
    else if ( (u64PhyAddr >= MIU1_PA_BASE) && (u64PhyAddr < MIU2_PA_BASE) ) // MIU1
        u64BusAddr = u64PhyAddr - MIU1_PA_BASE + CONFIG_MIU1_BUSADDR;
    else
        u64BusAddr = u64PhyAddr - MIU2_PA_BASE + CONFIG_MIU2_BUSADDR;
#else
    if( (u64PhyAddr >= MIU0_PA_BASE) && (u64PhyAddr < MIU1_PA_BASE) ) // MIU0
        u64BusAddr = u64PhyAddr - MIU0_PA_BASE + CONFIG_MIU0_BUSADDR;
    else if ( (u64PhyAddr >= MIU1_PA_BASE) ) // MIU1
        u64BusAddr = u64PhyAddr - MIU1_PA_BASE + CONFIG_MIU1_BUSADDR;
#endif
    return u64BusAddr;
}

unsigned int ns_uboot_anti_rollback(void)
{
#if defined(CONFIG_PROGRAM_EMMC_RPMB_KEY)
    static U8 buf[0x100] __attribute__((aligned(16)));
    struct rpmb_fs_partition *rpmb = (struct rpmb_fs_partition*)buf;
    const U8* AESKey = RpmbConfig.select_key == 1 ? RpmbConfig.rootkey : NULL;

    if(eMMC_RPMB_Read_data(buf, 0x100, 0, AESKey) != eMMC_ST_SUCCESS)
    {
        LDR_PUTS("~eMMC_RPMB_Read_data fail!\n");
        while(1);
    }
    array_reverse(buf,0x100);

    if (rpmb->anti_rollback_init_flag != FLAG_ANTIROLLBACK_INITIALIZED)
    {
        LDR_PUTS("\nAnti-rollback not enabled\n");
        /* In kernel/reboot.c, "adb reboot rpmbp" will write the magic to the reg addr */
        if(*(volatile unsigned short *)ANTIROLLBACK_REG_ADDR == ANTIROLLBACK_ENABLING_MAGIC)
        {
            LDR_PUTS("\nEnabling anti-rollback...\n\n");
            rpmb->anti_rollback_init_flag = FLAG_ANTIROLLBACK_INITIALIZED;
            goto write_ver;
        }

        /* set the register to indicate that anti-rollback is not enabled */
        *(volatile unsigned short *)ANTIROLLBACK_REG_ADDR = ANTIROLLBACK_NOT_ENABLED_MAGIC;
        return TRUE;
    }

    LDR_PUTS("\nsboot kept;");
    LDR_PUTDW(rpmb->sboot_version);
    LDR_PUTS(", new:");
    LDR_PUTDW(_gSbootVersion);
    LDR_PUTS("\nmboot kept:");
    LDR_PUTDW(rpmb->uboot_version);
    LDR_PUTS(", new:");
    LDR_PUTDW(*_gUbootVersionAddr);
    LDR_PUTS("\narmfw kept:");
    LDR_PUTDW(rpmb->armfw_version);
    LDR_PUTS(", new:");
    LDR_PUTDW(_gArmFwVersion);
    LDR_PUTS("\noptee kept:");
    LDR_PUTDW(rpmb->optee_version);
    LDR_PUTS(", new:");
    LDR_PUTDW(_gOpteeVersion);
    LDR_PUTC('\r');
    LDR_PUTC('\n');

    /* if any kept version is larger, stop booting */
    if (rpmb->optee_version > _gOpteeVersion ||
        rpmb->armfw_version > _gArmFwVersion ||
        rpmb->uboot_version > (*_gUbootVersionAddr) ||
        rpmb->sboot_version > _gSbootVersion)
    {
        LDR_PUTS("\nAnti-rollback check failed!\n\n");
        while(1);
    }

    if (rpmb->optee_version == _gOpteeVersion &&
        rpmb->armfw_version == _gArmFwVersion &&
        rpmb->uboot_version == *_gUbootVersionAddr &&
        rpmb->sboot_version == _gSbootVersion)
    {
        /* clear the register */
        *(volatile unsigned short *)ANTIROLLBACK_REG_ADDR = 0;
        return TRUE; /* versions are all same */
    }

    LDR_PUTS("Writing new anti-rollback version...\n");

write_ver:
    rpmb->sboot_version = _gSbootVersion;
    rpmb->uboot_version = *_gUbootVersionAddr;
    rpmb->optee_version = _gOpteeVersion;
    rpmb->armfw_version = _gArmFwVersion;
    array_reverse(buf,0x100);
    if(eMMC_RPMB_Write_data(buf, 0x100, 0, AESKey) != eMMC_ST_SUCCESS)
    {
        LDR_PUTS("~eMMC_RPMB_Write_data fail!\n");
        while(1);
    }

#endif
    /* clear the register */
    *(volatile unsigned short *)ANTIROLLBACK_REG_ADDR = 0;
   return TRUE;
}

void ns_uboot_set_uboot_version(unsigned int uboot_start, unsigned int uboot_size)
{
    _gUbootVersionAddr = uboot_start + uboot_size - 0x200;
}

static unsigned int ns_uboot_is_str(void)
{
#if defined(CONFIG_MSTAR_M7221)
    return ((*(volatile unsigned short*)(0x1F000000 + (0x0E70<<1))) & 0xC000) != 0xC000;
#else
    return ((*(volatile unsigned short*)(0x1F000000 + (0x0E70<<1))) & 0xF000) != 0xF000;
#endif
}

unsigned int ns_uboot_support_faststr(void)
{
    return ns_uboot_is_str() && (_gStrMode == FAST_BOOT);
}

unsigned int ns_uboot_load_optee(void)
{
    mstar_tee_t* header = NULL;
#if defined(CONFIG_MSTAR_ROM_BOOT_WITH_EMMC_FLASH)
    emmc_partition_t mpart;
#endif
    OPTEE_CONFIG_HEADER* opteeConfigHeader = NULL;
    int encrypted = 0;
    int alignment;
    U32 tmpBuf = (U32)u8TmpBuf;
    unsigned int u32HeadSize = sizeof(mstar_tee_t) + sizeof(OPTEE_CONFIG_HEADER) + sizeof(_SUB_SECURE_INFO);
    unsigned int u32HeadeMMCAlign = ALIGN(EMMC_BLOCK_SIZE, u32HeadSize);
    unsigned int u32HeadeMMCBlockNum = (u32HeadSize + u32HeadeMMCAlign) / EMMC_BLOCK_SIZE;

#if defined(CONFIG_MSTAR_ROM_BOOT_WITH_EMMC_FLASH)
    if (ns_uboot_load_partition(5, "optee", &header, &mpart))
    {
        eMMC_ReadData_MIU((U8*)UINT2PTR(tmpBuf), u32HeadSize + u32HeadeMMCAlign, mpart.start_block);
    }
    else
    {
        // partition 'optee' not found
        authFailType = AUTH_OPTEE_FAIL;
        return FALSE;
    }
#elif defined(CONFIG_MSTAR_ROM_BOOT_WITH_NAND_FLASH)
    if (UNFD_ST_SUCCESS != drvNAND_ReadPartition(UNFD_PART_OPTEE, 0, tmpBuf, 1)) // read 512-byte to get header and signature
    {
        authFailType = AUTH_OPTEE_FAIL;
        return FALSE; // nand read fail
    }
#endif

    // verify header
    {
        if (FALSE == MDrv_AESDMA_SecureMain_v2(tmpBuf, CONFIG_OPTEE_HEADER_LEN+sizeof(OPTEE_CONFIG_HEADER), ((_SUB_SECURE_INFO*)(tmpBuf+CONFIG_OPTEE_HEADER_LEN+sizeof(OPTEE_CONFIG_HEADER)))->u8Signature, _gTEEKeybank.u8RSATEEKey))
        {
            // optee header auth fail
            header = NULL;
            authFailType = AUTH_OPTEE_FAIL;
            return FALSE;
        }
        else
        {
            header = (mstar_tee_t*)UINT2PTR(tmpBuf);
            opteeConfigHeader = (OPTEE_CONFIG_HEADER*)(tmpBuf+CONFIG_OPTEE_HEADER_LEN);
        }
    }

    // make sure binary is really optee
    if ( 0 != _memcmp( &(header->magic), "OPTE", 4) )
    {
        authFailType = AUTH_OPTEE_FAIL;
        return FALSE;
    }

    BIN0_entry_point = header->init_load_addr_lo;
    _gOpteeVersion = header->cus_version;

#if !defined(CONFIG_ARMv8_ARM_TRUSTED_FIRMWARE)
    _gStrMode = ( header->warmboot_mode & STR_MODE_MASK );
#endif

    if ( _gIsStr && _gStrMode )
    {
        MDrv_SEAL_SetSecureRange(__BA2PA(header->init_load_addr_lo), __BA2PA(header->secure_range_end), SEAL_ATTR_SECURE_RW);
        authFailType = 0; //clear authFailType
        return TRUE;
    }

    MDrv_SEAL_SetSecureRange(__BA2PA(header->init_load_addr_lo), __BA2PA(header->secure_range_end), SEAL_ATTR_SECURE_RW);

    // copy header part to load address
    _memcpy(UINT2PTR(header->init_load_addr_lo), UINT2PTR(tmpBuf), u32HeadSize + u32HeadeMMCAlign);

    // read other body data
    unsigned int u32BodyAlign = ALIGN(16,header->init_size) + ALIGN(16,opteeConfigHeader->TotalConfigSize) + opteeConfigHeader->NumberOfConfig*sizeof(OPTEE_CONFIG_DESCRIPTOR) + opteeConfigHeader->TotalConfigSize; // Body dummy data
    unsigned int u32BodyeMMCAlign = ALIGN(EMMC_BLOCK_SIZE, header->init_size + u32BodyAlign + sizeof(_SUB_SECURE_INFO) - u32HeadeMMCAlign);
    unsigned int u32BodyOtherSizeeMMC = header->init_size + u32BodyAlign + sizeof(_SUB_SECURE_INFO) - u32HeadeMMCAlign + u32BodyeMMCAlign;

#if defined(CONFIG_MSTAR_ROM_BOOT_WITH_EMMC_FLASH)
    eMMC_ReadData_MIU(UINT2PTR(header->init_load_addr_lo + u32HeadSize + u32HeadeMMCAlign), u32BodyOtherSizeeMMC, mpart.start_block + u32HeadeMMCBlockNum);
#elif defined(CONFIG_MSTAR_ROM_BOOT_WITH_NAND_FLASH)
    if (UNFD_ST_SUCCESS == drvNAND_ReadPartition(UNFD_PART_OPTEE, 0, UINT2PTR(header->init_load_addr_lo), u32BodyOtherSizeeMMC/EMMC_BLOCK_SIZE + u32HeadeMMCBlockNum)) // read header with body
    {
        // because here we have loaded header from flash again, to prevent TOCTOU issue, we first replace the header part with the previously verified copy, before verify header+body as a whole
        _memcpy(UINT2PTR(header->init_load_addr_lo), UINT2PTR(tmpBuf), u32HeadSize);
    }
    else
    {
        authFailType = AUTH_OPTEE_FAIL;
        return FALSE; // read optee body fail
    }
#endif

    //verify optee body
    {
        int auth_ok = MDrv_AESDMA_SecureMain_v2(header->init_load_addr_lo, header->init_size + u32BodyAlign + u32HeadSize, ((_SUB_SECURE_INFO*)(header->init_load_addr_lo + header->init_size + u32BodyAlign + u32HeadSize))->u8Signature, _gTEEKeybank.u8RSATEEKey);
        if (!auth_ok)
        {
            // optee body auth fail
            authFailType = AUTH_OPTEE_FAIL;
            return FALSE;
        }

        _memcpy(UINT2PTR(header->init_load_addr_lo), UINT2PTR(header->init_load_addr_lo + u32HeadSize), header->init_size + u32BodyAlign);
        if (MDrv_AESDMA_Decrypt(__BA2PA(header->init_load_addr_lo), header->init_size + u32BodyAlign, _gTEEKeybank.u8AESTEEKey, AESDMA_ENGINE_CBC, E_AESDMA_SWKEY) == FALSE)
        {
            // optee body decrypt fail
            authFailType = AUTH_OPTEE_FAIL;
            return FALSE;
        }

        // parsing config data
        OPTEE_CONFIG_DESCRIPTOR* desc = (OPTEE_CONFIG_DESCRIPTOR*)(header->init_load_addr_lo + header->init_size + ALIGN(16, header->init_size) );
        unsigned char* dataOffset = header->init_load_addr_lo + header->init_size + ALIGN(16, header->init_size) + opteeConfigHeader->NumberOfConfig*sizeof(OPTEE_CONFIG_DESCRIPTOR);

        int i;
        for (i=0; i<opteeConfigHeader->NumberOfConfig; i++, desc++)
        {
            if (desc->ConfigID==1) // mmap
            {
                OPTEE_CONFIG_MMAP_HEADER* mmapHeader = (OPTEE_CONFIG_MMAP_HEADER*)(dataOffset + desc->Offset);
                ST_MMAP_GROUP_ITEM* groupItem = (ST_MMAP_GROUP_ITEM*)(dataOffset + desc->Offset + sizeof(OPTEE_CONFIG_MMAP_HEADER));
                ST_MMAP_DB_ITEM* dbItem = (ST_MMAP_DB_ITEM*)(dataOffset + desc->Offset + sizeof(OPTEE_CONFIG_MMAP_HEADER) + mmapHeader->NumberOfSecureGroup*sizeof(ST_MMAP_GROUP_ITEM));
                int j;

                // move data to HW AES BUF for secure OS
                if (dbItem[E_MMAP_ID_HW_AES_BUF].index != 0)
                {
                    unsigned char* dst;
                    unsigned char* src;

                    // get HW AES buffer information
                    groupItem = groupItem + dbItem[E_MMAP_ID_HW_AES_BUF].index - 1;

                    MDrv_SEAL_SetSecureRange(groupItem->addr, groupItem->addr + groupItem->length, SEAL_ATTR_SECURE_RW);

                    // copy customer keybank
                    dst = (unsigned char*)UINT2PTR(__PA2BA(groupItem->addr));

                    // copy mmapDB.ini for backward-compatible
                    dst += 0x1000;
                    src = (unsigned char*)(dataOffset + desc->Offset);
                    _memcpy(dst, src, 0x1000);

                    // copy optee config header
                    dst += 0x1000;
                    src = (unsigned char*)(tmpBuf+CONFIG_OPTEE_HEADER_LEN);
                    _memcpy(dst, src, sizeof(OPTEE_CONFIG_HEADER));

                    // copy optee config decriptor and config data
                    dst += sizeof(OPTEE_CONFIG_HEADER);
                    src = (unsigned char*)(header->init_load_addr_lo + header->init_size + ALIGN(16, header->init_size));
                    _memcpy(dst, src, opteeConfigHeader->NumberOfConfig*sizeof(OPTEE_CONFIG_DESCRIPTOR) + opteeConfigHeader->TotalConfigSize);
                }
            }
            else if (desc->ConfigID==2) // secure MIU host
            {
                unsigned char* data = (unsigned char*)(dataOffset + desc->Offset);
                unsigned int size = desc->ConfigSize;
                int i, j;

                for (i=0; i<size; i++)
                {
                    for (j=0; j<8; j++)
                    {
                        if ( (*(data+i)) & (1<<j) )
                        {
                            MDrv_SEAL_SetMIUHost(i*8+j, 1);
                        }
                    }
                }
            }
            else if (desc->ConfigID==3) // secure RIU bank
            {
                unsigned char* data = (unsigned char*)(dataOffset + desc->Offset);
                unsigned int size = desc->ConfigSize;
                int i, j;

                for (i=0; i<size; i++)
                {
                    for (j=0; j<8; j++)
                    {
                        if ( (*(data+i)) & (1<<j) )
                        {
                            MDrv_SEAL_SetRIUBank(i*8+j, 1);
                        }
                    }
                }
            }
            else if (desc->ConfigID==5) // secure RIU register
            {
                unsigned char* data = (unsigned char*)(dataOffset + desc->Offset);
                unsigned int size = desc->ConfigSize;
                int i, j;

                for (i=0; i<size; i++)
                {
                    for (j=0; j<8; j++)
                    {
                        if ( (*(data+i)) & (1<<j) )
                        {
                            MDrv_SEAL_SetRIURegister(i*8+j, 1);
                        }
                    }
                }
            }
#if defined(CONFIG_PROGRAM_EMMC_RPMB_KEY)
            else if (desc->ConfigID==9) // RPMB key config
            {
                DSCMB_KLCfg_All64 cfg;
                _memset(&cfg, 0, sizeof(DSCMB_KLCfg_All64));
                _memset(&RpmbConfig, 0, sizeof(RpmbConfig));
                unsigned char* data = (unsigned char*)(dataOffset + desc->Offset);
                _memcpy(&cfg, data, sizeof(DSCMB_KLCfg_All64));
                if (desc->ConfigSize < sizeof(DSCMB_KLCfg_All64) + 16 /*ACPU key size*/ + 16*cfg.u32Level /*KL input size*/)
                {
                    LDR_PUTS("Bad RPMB key\n");
                    return FALSE;
                }
                _memcpy(&RpmbConfig, data+sizeof(DSCMB_KLCfg_All64)+16 + cfg.u32Level*16, sizeof(Rpmb_Config));
            }
#endif
        }
#if defined(CONFIG_PROGRAM_EMMC_RPMB_KEY)
        //LDR_PUTS("eMMC_RPMB_Check_Program_Key\n");
        eMMC_RPMB_Check_Program_Key(RpmbConfig.select_key == 1 ? RpmbConfig.rootkey : NULL);
#endif
    }

    authFailType = 0; //clear authFailType
    return header->init_load_addr_lo;
}

unsigned int ns_uboot_load_armfw(void)
{
    mstar_tee_t* header = NULL;
#if defined(CONFIG_MSTAR_ROM_BOOT_WITH_EMMC_FLASH)
    emmc_partition_t mpart;
#endif
    int encrypted = 0;
    int alignment;
    unsigned int u32HeadSize = CONFIG_OPTEE_HEADER_LEN + sizeof(_SUB_SECURE_INFO);
    unsigned int u32HeadeMMCAlign = ALIGN(EMMC_BLOCK_SIZE, u32HeadSize);
    unsigned int u32HeadeMMCBlockNum = (u32HeadSize + u32HeadeMMCAlign) / EMMC_BLOCK_SIZE;
    U8* tmpBuf = (U8*)u8TmpBuf;

#if defined(CONFIG_MSTAR_ROM_BOOT_WITH_EMMC_FLASH)
    if (ns_uboot_load_partition(5, "armfw", &header, &mpart))
    {
        eMMC_ReadData_MIU(tmpBuf, u32HeadSize + u32HeadeMMCAlign, mpart.start_block);
    }
    else
    {
        // load partition failed
        authFailType = AUTH_ARMFW_FAIL;
        return 0;
    }
#elif defined(CONFIG_MSTAR_ROM_BOOT_WITH_NAND_FLASH)
    if (UNFD_ST_SUCCESS != drvNAND_ReadPartition(UNFD_PART_ARMFW , 0, tmpBuf, 1)) // read 512-byte to get header and signature
    {
        authFailType = AUTH_ARMFW_FAIL;
        return FALSE; // nand read fail
    }
#endif

    // verify header
    {
        if (FALSE == MDrv_AESDMA_SecureMain_v2(tmpBuf, CONFIG_OPTEE_HEADER_LEN, ((_SUB_SECURE_INFO*)(tmpBuf+CONFIG_OPTEE_HEADER_LEN))->u8Signature, _gTEEKeybank.u8RSATEEKey))
        {
            header = NULL;
            authFailType = AUTH_ARMFW_FAIL;
            return 0;
        }
        else
        {
            header = (mstar_tee_t*)tmpBuf;
        }
    }

    // make sure binary is really armfw
    if ( 0 != _memcmp( &(header->magic), "ATFW", 4) )
    {
        authFailType = AUTH_ARMFW_FAIL;
        return 0;
    }

    Fireware_entry = header->init_load_addr_lo;
    _gArmFwVersion = header->cus_version;

    // STR mode depends on armfw header
    _gStrMode = ( header->warmboot_mode & STR_MODE_MASK );
    _gAuthMode = ( header->warmboot_mode & AUTH_MODE_MASK );


    if ( _gIsStr && _gStrMode )
    {
        MDrv_SEAL_SetSecureRange(__BA2PA(header->init_load_addr_lo), __BA2PA(header->secure_range_end), SEAL_ATTR_SECURE_RW);

        if ( _gAuthMode == AESCBC_MAC )
        {
            int i;
            unsigned int result = 0;
            warmboot_table* tbl;

            if ( MDrv_SYS_Query(E_SYS_QUERY_MOBF_KEY_SAVED, &result) == FALSE )
            {
                authFailType = AUTH_ARMFW_FAIL;
                return FALSE;
            }

            if ( result ) // support mobf
            {
                do
                {
                    if ( MDrv_SYS_Query(E_SYS_QUERY_MOBF_ENABLED, &result) == FALSE )
                    {
                        authFailType = AUTH_ARMFW_FAIL;
                        return FALSE;
                    }

                    if ( result ) // MOBF init done
                    {
                        break;
                    }
                } while ( 1 );
            }

            if ( MDrv_SYS_Query(E_SYS_QUERY_TRNG_KEY_SAVED, &result) == FALSE )
            {
                authFailType = AUTH_ARMFW_FAIL;
                return FALSE;
            }

            if ( !result ) // no TRNG key, CBC-MAC check fail
            {
                authFailType = AUTH_ARMFW_FAIL;
                return FALSE;
            }

            // calculate CBC-MAC with TRNG key
            tbl = (warmboot_table*)(header->secure_range_end - sizeof(warmboot_table));
            MDrv_AESDMA_STR_CBCMAC(__BA2PA(tbl->ATFBase), tbl->ATFSize, __BA2PA(u8ATF_MAC));

            for ( i=0 ; i<16 ; i++ )
            {
                if ( u8ATF_MAC[i] != tbl->ATFMAC[i] )
                {
                    authFailType = AUTH_ARMFW_FAIL;
                    return FALSE;
                }
            }
        }

        authFailType = 0;   //clear authFailType
        return header->init_load_addr_lo;
    }

    MDrv_SEAL_SetSecureRange(__BA2PA(header->init_load_addr_lo), __BA2PA(header->secure_range_end), SEAL_ATTR_SECURE_RW);

    // copy header part to load address
    _memcpy(UINT2PTR(header->init_load_addr_lo), UINT2PTR(tmpBuf), u32HeadSize + u32HeadeMMCAlign);

    // read other body data
    unsigned int u32BodyAlign = ALIGN(16,header->init_size); // Body dummy data

    unsigned int u32BodyeMMCAlign = ALIGN(EMMC_BLOCK_SIZE, header->init_size + u32BodyAlign + sizeof(_SUB_SECURE_INFO) - u32HeadeMMCAlign);
    unsigned int u32BodyOtherSizeeMMC = header->init_size + u32BodyAlign + sizeof(_SUB_SECURE_INFO) - u32HeadeMMCAlign + u32BodyeMMCAlign;

#if defined(CONFIG_MSTAR_ROM_BOOT_WITH_EMMC_FLASH)
    eMMC_ReadData_MIU(UINT2PTR(header->init_load_addr_lo + u32HeadSize + u32HeadeMMCAlign), u32BodyOtherSizeeMMC, mpart.start_block + u32HeadeMMCBlockNum);
#elif defined(CONFIG_MSTAR_ROM_BOOT_WITH_NAND_FLASH)
    if (UNFD_ST_SUCCESS == drvNAND_ReadPartition(UNFD_PART_ARMFW, 0, UINT2PTR(header->init_load_addr_lo), u32BodyOtherSizeeMMC/EMMC_BLOCK_SIZE + u32HeadeMMCBlockNum)) // read header with body
    {
        // because here we have loaded header from flash again, to prevent TOCTOU issue, we first replace the header part with the previously verified copy, before verify header+body as a whole
        _memcpy(UINT2PTR(header->init_load_addr_lo), UINT2PTR(tmpBuf), u32HeadSize);
    }
    else
    {
        authFailType = AUTH_ARMFW_FAIL;
        return FALSE; // read armfw body fail
    }
#endif
    // verify body
    {
        int auth_ok = MDrv_AESDMA_SecureMain_v2(header->init_load_addr_lo, header->init_size + u32BodyAlign + u32HeadSize, ((_SUB_SECURE_INFO*)(header->init_load_addr_lo + header->init_size + u32BodyAlign + u32HeadSize))->u8Signature, _gTEEKeybank.u8RSATEEKey);
        if (!auth_ok)
        {
            // armfw body auth fail
            authFailType = AUTH_ARMFW_FAIL;
            return FALSE;
        }


        _memcpy(UINT2PTR(header->init_load_addr_lo), UINT2PTR(header->init_load_addr_lo + u32HeadSize), header->init_size + u32BodyAlign);

        if (MDrv_AESDMA_Decrypt(__BA2PA(header->init_load_addr_lo), header->init_size + u32BodyAlign, _gTEEKeybank.u8AESTEEKey, AESDMA_ENGINE_CBC, E_AESDMA_SWKEY) == FALSE)
        {
            // armfw body decrypt fail
            authFailType = AUTH_ARMFW_FAIL;
            return FALSE;
        }
    }

    authFailType = 0;   //clear authFailType
    return header->init_load_addr_lo;
}

#ifdef CONFIG_DOUBLE_OPTEE
static U32 Double_OPTEE_Check(char* name, unsigned int len)
{
    U32 ret = 0;

    if (_memcmp(name,"armfw",len) == 0 && \
        (*(volatile unsigned int*)(CONFIG_RIU_BASE_ADDRESS + (0x103380<<1)))&0x0400)
    {
        ret = 1; //armfw authen failed, try armfw_bak partition
    }
    else if (_memcmp(name,"optee",len) == 0 && \
             (*(volatile unsigned int*)(CONFIG_RIU_BASE_ADDRESS + (0x103380<<1)))&0x0800)
    {
        ret = 1; //optee authen failed, try optee_bak partition
    }

    return ret;
}
#endif

#if defined(CONFIG_MSTAR_ROM_BOOT_WITH_EMMC_FLASH)
int ns_uboot_load_partition(unsigned int len, char* name, mstar_tee_t** header, emmc_partition_t* mpart)
{
    int u32_i, u32_j;
    int found;
    U8* tmpBuf = (U8*)u8TmpBuf;
    U8* part_name = (U8*)u8TmpBuf;

    for(u32_i=0; u32_i < len; u32_i++)
    {
        part_name[u32_i] = name[u32_i];
    }

#ifdef CONFIG_DOUBLE_OPTEE
    if (0 != Double_OPTEE_Check(name, len)) //get armfw_bak, optee_bak partition info
    {
        part_name[len++] = '_';
        part_name[len++] = 'b';
        part_name[len++] = 'a';
        part_name[len++] = 'k';
    }
#endif

    // iterate to find specific partition
    for(u32_i=0;u32_i<= EMMC_RESERVED_FOR_MAP_V2;u32_i++)
    {
        if(eMMC_ReadData_MIU((U8*)mpart, EMMC_BLOCK_SIZE, u32_i))
            break; // emmc read fail

        found = 1;

        for (u32_j=0; u32_j<len; u32_j++)
        {
            if (mpart->name[u32_j]!=part_name[u32_j])
            {
                found = 0;
                break;
            }
        }

        if (found)
        {
            eMMC_ReadData_MIU(tmpBuf, EMMC_BLOCK_SIZE, mpart->start_block);
            *header = (mstar_tee_t*)tmpBuf;
            return 1;
        }
    }

    return 0;
}
#endif

static char byte_to_char(char val)
{
    if (val > 0x9)
        return (0x41+(val-10));
    else
        return (0x30+val);
}

static void u32_to_string(unsigned int val, char* dst)
{
    dst[0] = byte_to_char((val>>28)&0xF);
    dst[1] = byte_to_char((val>>24)&0xF);
    dst[2] = byte_to_char((val>>20)&0xF);
    dst[3] = byte_to_char((val>>16)&0xF);
    dst[4] = byte_to_char((val>>12)&0xF);
    dst[5] = byte_to_char((val>>8)&0xF);
    dst[6] = byte_to_char((val>>4)&0xF);
    dst[7] = byte_to_char((val>>0)&0xF);
}

void to_hex_string(U8* src, char* dst, unsigned int len)
{
    static char hex_tbl[16] __attribute__((aligned(8)))= {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
    int i, j;
    U8 val;
    for (i=0, j=0; i<len; ++i, j+=2)
    {
        val = src[i];
        dst[j] = hex_tbl[((val>>4) & 0x0F)];
        dst[j+1] = hex_tbl[(val & 0x0F)];
    }
}

typedef void (*OPTEE_entry)(int _bootarg, int _ree_addr, int c, int d);

static mstar_boot_prameters_t boot  __attribute__((aligned(8)));

void ns_uboot_boot(unsigned int uboot_addr)
{
    mstar_boot_prameters_t* bootp = &boot;
    U8 emmc_CID[16];
    unsigned short RTCBank = 0;

    // Avoid REE using eFuse key
    MDrv_AESDMA_DisableHwKey();

    MDrv_SEAL_Cleanup();

    _memset(emmc_CID, 0, sizeof(emmc_CID));
#if defined(CONFIG_MSTAR_ROM_BOOT_WITH_EMMC_FLASH)
    //Copy EMMC CID prior to SRAM cleanup since EMMC CID may be stored in SRAM
    eMMC_GetCID(emmc_CID);
#endif
    //Clean SRAM
    extern unsigned int _CONFIG_SRAM_START_ADDRESS,_CONFIG_SRAM_CODE_SIZE;
    _memset(&_CONFIG_SRAM_START_ADDRESS,0,(unsigned int)(&_CONFIG_SRAM_CODE_SIZE));
    //Clean u8TmpBuf
    _memset(u8TmpBuf,0,TPMBUFFERSIZE);
    //Clean TEE/REE Keybanks
    _memset(&_gTEEKeybank,0,sizeof(TEE_KEY_BANK));
    _memset(&_gREEKeybank,0,sizeof(REE_KEY_BANK));

    switch(authFailType)
    {
        case AUTH_KEYBANK_FAIL:
            LDR_PUTS("\nAUTH_KEYBANK_FAIL\n");
            ns_uboot_nonsecure_handler();
            WREG16(0x1F206704,0xEEFF);
            return;
            break;
        case AUTH_ARMFW_FAIL:
            LDR_PUTS("\nAUTH_ARMFW_FAIL\n");
            ns_uboot_nonsecure_handler();
            WREG16(0x1F206704,0xEEFF);
            return;
            break;
        case AUTH_OPTEE_FAIL:
            LDR_PUTS("\nAUTH_OPTEE_FAIL\n");
            ns_uboot_nonsecure_handler();
            WREG16(0x1F206704,0xEEFF);
            return;
            break;
        default:
            WREG16(0x1F206704,0x0000);
            break;
    }

    if ( _gIsStr && _gStrMode )
    {
#if defined(CONFIG_ARMv8_ARM_TRUSTED_FIRMWARE)
        __asm__ __volatile__(
        ".arch_extension sec\n\t"
        "ldr  r1,[%0]  \n\t"
        "mov  r0,%1  \n\t"
        "smc #0  \n\t"
        :
        :"r"(&Fireware_entry),"r"(&bootp)
        :"r0","r1"
        );
#else
        OPTEE_entry entry = (OPTEE_entry) BIN0_entry_point;
        entry(0, bootp->BIN0_bootargs, bootp->BIN1_entry_point, 0);
#endif
    }

    bootp->BIN0_entry_point = BIN0_entry_point;
	bootp->BIN0_RW64 = MODE_RW_32;
	bootp->BIN1_entry_point = uboot_addr;
	bootp->BIN1_RW64 = MODE_RW_32;
#ifdef CONFIG_ARMv8_64BIT_KERNEL
	bootp->BIN2_RW64 = MODE_RW_64;
#else
	bootp->BIN2_RW64 = MODE_RW_32;
#endif

    { // construct OPTEE boot args

        _memset(_gOpteeBootArgs, 0, sizeof(_gOpteeBootArgs));
        _gOpteeBootArgs[0] = 'C';
        _gOpteeBootArgs[1] = 'I';
        _gOpteeBootArgs[2] = 'D';
        _gOpteeBootArgs[3] = '=';
        to_hex_string(emmc_CID, _gOpteeBootArgs+4, 15);
        _gOpteeBootArgs[34] = ' ';
        _gOpteeBootArgs[35] = 'S';
        _gOpteeBootArgs[36] = 'T';
        _gOpteeBootArgs[37] = 'R';
        _gOpteeBootArgs[38] = 'M';
        _gOpteeBootArgs[39] = 'O';
        _gOpteeBootArgs[40] = 'D';
        _gOpteeBootArgs[41] = 'E';
        _gOpteeBootArgs[42] = '=';
        _gOpteeBootArgs[43] = '0';
        _gOpteeBootArgs[44] = 'x';
        u32_to_string(_gStrMode, _gOpteeBootArgs+45);
#if defined(CONFIG_ARMv8_ARM_TRUSTED_FIRMWARE) && defined(ENABLE_MCT)
        _gOpteeBootArgs[53] = ' ';
        _gOpteeBootArgs[54] = 'M';
        _gOpteeBootArgs[55] = 'C';
        _gOpteeBootArgs[56] = 'T';
        _gOpteeBootArgs[57] = '=';
        _gOpteeBootArgs[58] = '0';
        _gOpteeBootArgs[59] = 'x';
        u32_to_string(&_gMultiCoreTable, _gOpteeBootArgs+60);
        _gOpteeBootArgs[68] = '\0';
#else
        _gOpteeBootArgs[53] = '\0';
#endif

        bootp->BIN0_bootargs = _gOpteeBootArgs;
    }
#if defined(CONFIG_OPTEE_ANTIBRICK)
    //Enable watchdog and write status flag
    WREG16(0x1F006010,0x5400);
    WREG16(0x1F006014,0x2AEA);
    //OPTEECHECKIN
    _gOpteeConsisStatus = OPTEECHECKIN;
    RTCBank = RREG16(RTC_BANK);
    RTCBank = (RTCBank & 0x00FF) | _gOpteeConsisStatus << 8;
    WREG16(RTC_BANK,RTCBank);
    //
#endif

#if defined(CONFIG_ARMv8_ARM_TRUSTED_FIRMWARE)
	__asm__ __volatile__(
        ".arch_extension sec\n\t"
		"ldr  r1,[%0]  \n\t"
		"mov  r0,%1  \n\t"
		"smc #0  \n\t"
		:
		:"r"(&Fireware_entry),"r"(bootp)
		:"r0","r1"
	);
#else
    OPTEE_entry entry = (OPTEE_entry) BIN0_entry_point;
    entry(0, bootp->BIN0_bootargs, bootp->BIN1_entry_point, 0);
#endif
}

#if defined(CONFIG_OPTEE_ANTIBRICK)
unsigned int ns_uboot_check_optee_consistency()
{
    _gOpteeConsisStatus = (unsigned char)(RREG16(RTC_BANK)>>8);
    //Check Optee consistency
    if(_gOpteeConsisStatus == OPTEECHECKIN)
    {
        authFailType = AUTH_KEYBANK_FAIL;
        return FALSE;
    }
    return TRUE;
}
#endif

unsigned int ns_uboot_read_keybank(unsigned int keybank_rom_offset)
{
#ifdef CONFIG_DOUBLE_MBOOT
    U32 mbootEmmcBase = IS_MBOOTBAK() ? 0x8000 : 0x1000;
#else
    U32 mbootEmmcBase = 0x1000;
#endif
    U8* tmpBuf = (U8*)u8TmpBuf;
    U8* tmpBuf1 = (U8*)u8TmpBuf1;

    // Get chunk header
#if defined(CONFIG_MSTAR_ROM_BOOT_WITH_EMMC_FLASH)
    if (eMMC_ReadData_MIU(tmpBuf, CHUNK_HEADER_SIZE, mbootEmmcBase ))
    {
        authFailType = AUTH_KEYBANK_FAIL;
        return FALSE;
    }
    //Authenticate chunk header
    if(FALSE ==MDrv_AESDMA_SecureMain(tmpBuf, 0x2F0, tmpBuf+0x300, 0))
    {
        authFailType = AUTH_KEYBANK_FAIL;
        return FALSE;
    }
    keybank_rom_offset =  *((unsigned int*)(&tmpBuf[0x4C]));
#elif defined(CONFIG_MSTAR_ROM_BOOT_WITH_NAND_FLASH)
    //NOT SUPPORTED
#endif

    _gKeybankRomOffset = keybank_rom_offset;

    // Copy TEEKeybank
#if defined(CONFIG_MSTAR_ROM_BOOT_WITH_EMMC_FLASH)
    if (eMMC_ReadData_MIU(tmpBuf, KEYBANK_BINARY_SIZE, ((keybank_rom_offset + TEEKEYBANK_OFFSET)>>9) + mbootEmmcBase ))
    {
        authFailType = AUTH_KEYBANK_FAIL;
        return FALSE;
    }
    // read customer keybank
    if (eMMC_ReadData_MIU((U8*)tmpBuf1, 512 * 3, (_gKeybankRomOffset>>9) + mbootEmmcBase ))
    {
        authFailType = AUTH_KEYBANK_FAIL;
        return 0; // emmc read fail
    }
#elif defined(CONFIG_MSTAR_ROM_BOOT_WITH_NAND_FLASH)
    _memcpy(tmpBuf, keybank_rom_offset + TEEKEYBANK_OFFSET, KEYBANK_BINARY_SIZE);
#endif

    _memcpy(&_gKeybank,tmpBuf1,sizeof(CUSTOMER_KEY_BANK));

    // Encrypt SeedOfTeeKeybank by eFuse key to generate a unique key for TEEKeybank
    if( MDrv_AESDMA_Encrypt(__BA2PA(SeedOfTeeKeybank), sizeof(SeedOfTeeKeybank), NULL, AESDMA_ENGINE_CBC, E_AESDMA_EFUSEKEY0) == FALSE )
    {
        authFailType = AUTH_KEYBANK_FAIL;
        return FALSE;
    }

    // Try using TEE unique key to decrypt TEEKeybank
    if (MDrv_AESDMA_Decrypt(__BA2PA(tmpBuf), KEYBANK_BINARY_SIZE, SeedOfTeeKeybank, AESDMA_ENGINE_CBC, E_AESDMA_SWKEY) == FALSE)
    {
        authFailType = AUTH_KEYBANK_FAIL;
        return FALSE;
    }

    // Auth TEEKeybank
    if (FALSE == MDrv_AESDMA_SecureMain_v2((U8*)((TEE_KEY_BANK*)(tmpBuf))->u8MSID, sizeof(TEE_KEY_BANK)-sizeof(_SUB_SECURE_INFO), (U8*)((TEE_KEY_BANK*)(tmpBuf))->SecureInfo.u8Signature, _gKeybank.u8RSABootPublicKey))
    {
        // Auth failed, TEEKeybank is not encrypted by TEE unique key. Try using offline encryption key
#if defined(CONFIG_MSTAR_ROM_BOOT_WITH_EMMC_FLASH)
        if (eMMC_ReadData_MIU(tmpBuf, KEYBANK_BINARY_SIZE, ((keybank_rom_offset + TEEKEYBANK_OFFSET)>>9) + mbootEmmcBase ))
        {
            authFailType = AUTH_KEYBANK_FAIL;
            return FALSE;
        }
        //Read EMBEDDED_TEE_AESKEY out from EMMC
        if (eMMC_ReadData_MIU(tmpBuf + KEYBANK_BINARY_SIZE, EMMC_BLOCK_SIZE, ((keybank_rom_offset + TEEKEYBANK_OFFSET + KEYBANK_BINARY_SIZE)>>9) + mbootEmmcBase ))
        {
            authFailType = AUTH_KEYBANK_FAIL;
            return FALSE;
        }
        //_memcpy(EmbeddedAESTEEUniformKey, tmpBuf + KEYBANK_BINARY_SIZE, AES_KEY_LEN);//Copy EMBEDDED_TEE_AESKEY data to EmbeddedAESTEEUniformKey
        _memcpy(EmbeddedAESTEEUniformKey, &_gKeybank.u8AESBootKey, AES_KEY_LEN);
#elif defined(CONFIG_MSTAR_ROM_BOOT_WITH_NAND_FLASH)
        _memcpy(tmpBuf, keybank_rom_offset + TEEKEYBANK_OFFSET, KEYBANK_BINARY_SIZE);
        _memcpy(EmbeddedAESTEEUniformKey, keybank_rom_offset + TEEKEYBANK_OFFSET + KEYBANK_BINARY_SIZE, AES_KEY_LEN);//Copy EMBEDDED_TEE_AESKEY data to EmbeddedAESTEEUniformKey
#endif
        array_reverse(EmbeddedAESTEEUniformKey, AES_KEY_LEN);
        if (MDrv_AESDMA_Decrypt(__BA2PA(tmpBuf), KEYBANK_BINARY_SIZE, EmbeddedAESTEEUniformKey, AESDMA_ENGINE_CBC, E_AESDMA_SWKEY) == FALSE)
        {
            authFailType = AUTH_KEYBANK_FAIL;
            return FALSE;
        }

        if (FALSE == MDrv_AESDMA_SecureMain_v2((U8*)((TEE_KEY_BANK*)(tmpBuf))->u8MSID, sizeof(TEE_KEY_BANK)-sizeof(_SUB_SECURE_INFO), (U8*)((TEE_KEY_BANK*)(tmpBuf))->SecureInfo.u8Signature, _gKeybank.u8RSABootPublicKey))
        {
            authFailType = AUTH_KEYBANK_FAIL;
            return FALSE;
        }

        _memcpy(&_gTEEKeybank,tmpBuf,sizeof(_gTEEKeybank));

        //Re-encrypt TEEKeybank using TEE unique Key and write-back to flash
        if( MDrv_AESDMA_Encrypt(__BA2PA(tmpBuf), KEYBANK_BINARY_SIZE, SeedOfTeeKeybank, AESDMA_ENGINE_CBC, E_AESDMA_SWKEY) == FALSE )
        {
            authFailType = AUTH_KEYBANK_FAIL;
            return FALSE;
        }

#if defined(CONFIG_MSTAR_ROM_BOOT_WITH_EMMC_FLASH)
        //Store EFuse re-encrypted TKB into EMMC
        if (eMMC_WriteData_MIU(tmpBuf, KEYBANK_BINARY_SIZE, ((keybank_rom_offset + TEEKEYBANK_OFFSET)>>9) + mbootEmmcBase ))
        {
            authFailType = AUTH_KEYBANK_FAIL;
            return FALSE;
        }
        //Remove EMBEDDED_TEE_AESKEY in EMMC
        _memset(tmpBuf+KEYBANK_BINARY_SIZE,0,EMMC_BLOCK_SIZE);//The buffer needs to be cleaned up for EMBEDDED_TEE_AESKEY
        if (eMMC_WriteData_MIU(tmpBuf+KEYBANK_BINARY_SIZE, EMMC_BLOCK_SIZE , ((keybank_rom_offset + TEEKEYBANK_OFFSET + KEYBANK_BINARY_SIZE)>>9) + mbootEmmcBase ))
        {
            authFailType = AUTH_KEYBANK_FAIL;
            return FALSE;
        }
#endif
    }
    else
    {
        _memcpy(&_gTEEKeybank,tmpBuf,sizeof(_gTEEKeybank));
    }

    array_reverse(_gTEEKeybank.u8AESTEEKey, AES_KEY_LEN);


#ifdef CONFIG_TEE_LOADER
    static U8 SeedOfReeKeybank[16] = { 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55 };

    //Copy REEKeybank
    if (eMMC_ReadData_MIU(tmpBuf, KEYBANK_BINARY_SIZE, ((keybank_rom_offset + REEKEYBANK_OFFSET)>>9) + mbootEmmcBase ))
    {
        authFailType = AUTH_KEYBANK_FAIL;
        return FALSE;
    }

    // Encrypt SeedOfReeKeybank by eFuse key to generate a unique key for REEKeybank
    if( MDrv_AESDMA_Encrypt(__BA2PA(SeedOfReeKeybank), sizeof(SeedOfReeKeybank), NULL, AESDMA_ENGINE_CBC, E_AESDMA_EFUSEKEY) == FALSE )
    {
        authFailType = AUTH_KEYBANK_FAIL;
        return FALSE;
    }

    // Try using REE unique key to decrypt REEKeybank
    if (MDrv_AESDMA_Decrypt(__BA2PA(tmpBuf), KEYBANK_BINARY_SIZE, SeedOfReeKeybank, AESDMA_ENGINE_CBC, E_AESDMA_SWKEY) == FALSE)
    {
        authFailType = AUTH_KEYBANK_FAIL;
        return FALSE;
    }

    // Auth REEKeybank
    if (FALSE == MDrv_AESDMA_SecureMain_v2((U8*)((REE_KEY_BANK*)(tmpBuf))->u8RSAKey, sizeof(REE_KEY_BANK)-sizeof(_SUB_SECURE_INFO), (U8*)((REE_KEY_BANK*)(tmpBuf))->SecureInfo.u8Signature, EmbeddedReeRsaPub))
    {
        // Auth failed, REEKeybank is not encrypted by REE unique key. Try using offline encryption key
        if (eMMC_ReadData_MIU(tmpBuf, KEYBANK_BINARY_SIZE, ((keybank_rom_offset + REEKEYBANK_OFFSET)>>9) + mbootEmmcBase ))
        {
            authFailType = AUTH_KEYBANK_FAIL;
            return FALSE;
        }

        array_reverse(EmbeddedAESREEUniformKey, AES_KEY_LEN);
        if (MDrv_AESDMA_Decrypt(__BA2PA(tmpBuf), KEYBANK_BINARY_SIZE, EmbeddedAESREEUniformKey, AESDMA_ENGINE_CBC, E_AESDMA_SWKEY) == FALSE)
        {
            authFailType = AUTH_KEYBANK_FAIL;
            return FALSE;
        }

        if (FALSE == MDrv_AESDMA_SecureMain_v2((U8*)((REE_KEY_BANK*)(tmpBuf))->u8RSAKey, sizeof(REE_KEY_BANK)-sizeof(_SUB_SECURE_INFO), (U8*)((REE_KEY_BANK*)(tmpBuf))->SecureInfo.u8Signature, EmbeddedReeRsaPub))
        {
            authFailType = AUTH_KEYBANK_FAIL;
            return FALSE;
        }
        _memcpy(&_gREEKeybank,tmpBuf,sizeof(_gREEKeybank));

        //Re-encrypt REEKeybank using REE unique key and write-back to flash
        if( MDrv_AESDMA_Encrypt(__BA2PA(tmpBuf), KEYBANK_BINARY_SIZE, SeedOfReeKeybank, AESDMA_ENGINE_CBC, E_AESDMA_SWKEY) == FALSE )
        {
            authFailType = AUTH_KEYBANK_FAIL;
            return FALSE;
        }

        if (eMMC_WriteData_MIU(tmpBuf, KEYBANK_BINARY_SIZE, ((keybank_rom_offset + REEKEYBANK_OFFSET)>>9) + mbootEmmcBase ))
        {
            authFailType = AUTH_KEYBANK_FAIL;
            return FALSE;
        }
    }
    else
    {
        _memcpy(&_gREEKeybank,tmpBuf,sizeof(_gREEKeybank));
    }

    array_reverse(_gREEKeybank.u8AESKey, AES_KEY_LEN);
#endif

    return TRUE;
}

unsigned int ns_uboot_get_optee_addr(void)
{
    return BIN0_entry_point;
}

unsigned int ns_uboot_nonsecure_handler(void)
{
    // set hosts of PM RIU bridge to non-secure
    // set hosts of NONPM RIU bridge to non-secure (except ARM)
    // set hosts of SEC RIU bridge to non-secure
    // set ARM to non-secure mode (must be the last step of this stage)
    MDrv_SEAL_DisableRIUBridges();

    return 1;
}

void ns_uboot_init(void)
{
    _gIsStr = ns_uboot_is_str() ? 1 : 0;

    _gHash1Version = *(volatile unsigned short*)(0x1F000000+(0x0600<<1));

    _memset(&RpmbConfig, 0, sizeof(RpmbConfig));

#if defined(CONFIG_MSTAR_ROM_BOOT_WITH_NAND_FLASH)
    nand_set_partinfo(NULL);
#endif

    MDrv_SEAL_Init();
}

#ifdef CONFIG_TEE_LOADER
unsigned int ns_uboot_SecureMain(U32 u32PlaintextAddr, U32 u32Size, U32 u32SignatureAddr, U8 KeySel)
{
    unsigned int auth_ok;

    if (KeySel == 0x0)
        auth_ok = MDrv_AESDMA_SecureMain_v2(u32PlaintextAddr, u32Size,
                                 u32SignatureAddr, _gTEEKeybank.u8RSATEEKey);
    else
        auth_ok = MDrv_AESDMA_SecureMain_v2(u32PlaintextAddr, u32Size,
                                 u32SignatureAddr, _gREEKeybank.u8RSAKey);

    return auth_ok;
}

void ns_uboot_copy_ReeKeybank(U32 addr)
{
    memcpy(addr, _gREEKeybank.u8RSAKey, RSA_PUBLIC_KEY_LEN + AES_KEY_LEN);
}
#endif
