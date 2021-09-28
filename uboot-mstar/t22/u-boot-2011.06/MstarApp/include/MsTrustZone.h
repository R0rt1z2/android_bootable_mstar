#ifndef __MSTAR_TRUSTZONE_H__
#define __MSTAR_TRUSTZONE_H__

#ifndef __ASSEMBLY__

#define MSTAR_BOOT_VERSION_MAJOR 1
#define MSTAR_BOOT_VERSION_MINOR 0
#define MSTAR_BOOT_VERSION (MSTAR_BOOT_VERSION_MAJOR << 8) | (MSTAR_BOOT_VERSION_MINOR)

/*
 * version will be 0~7bit(MINOR) 8~15bit(MAJOR) 16~31bit(struct size check in byte)
 * 31  sizeof(mstar_boot_prameters) 15   MAJOR       7     MINOR      0
 * |--------------------------------|----------------|----------------|
 */

typedef struct mstar_boot_prameters
{
	unsigned int version;
	unsigned long long FW_bootargs;
		//OPTEE
		//OPTEE
		unsigned long long BIN0_entry_point;
		unsigned long long BIN0_bootargs;
		unsigned long long BIN0_RW64;
		unsigned long long BIN0_r0;
		unsigned long long BIN0_r1;
		unsigned long long BIN0_r2;
		unsigned long long BIN0_r3;
		unsigned long long BIN0_r4;
		unsigned long long BIN0_Reserve[16];
		//KERN
		unsigned long long BIN1_entry_point;
		unsigned long long BIN1_bootargs;
		unsigned long long BIN1_RW64;
		unsigned long long BIN1_r0;
		unsigned long long BIN1_r1;
		unsigned long long BIN1_r2;
		unsigned long long BIN1_r3;
		unsigned long long BIN1_r4;
		unsigned long long BIN1_Reserve[16];
		//Reserve1
		unsigned long long BIN2_entry_point;
		unsigned long long BIN2_bootargs;
		unsigned long long BIN2_RW64;
		unsigned long long BIN2_r0;
		unsigned long long BIN2_r1;
		unsigned long long BIN2_r2;
		unsigned long long BIN2_r3;
		unsigned long long BIN2_r4;
		unsigned long long BIN2_Reserve[16];
		//Reserve2
		unsigned long long BIN3_entry_point;
		unsigned long long BIN3_bootargs;
		unsigned long long BIN3_RW64;
		unsigned long long BIN3_r0;
		unsigned long long BIN3_r1;
		unsigned long long BIN3_r2;
		unsigned long long BIN3_r3;
		unsigned long long BIN3_r4;
		unsigned long long BIN3_Reserve[16];
} mstar_boot_prameters_t;


#define SECURE		0x0
#define NON_SECURE	0x1

#define MODE_SP_SHIFT	0x0
#define MODE_SP_MASK	0x1
#define MODE_SP_EL0		0x0
#define MODE_SP_ELX		0x1

#define MODE_RW_SHIFT	0x4
#define MODE_RW_MASK	0x1
#define MODE_RW_64		0x0
#define MODE_RW_32		0x1

#define MODE_EL_SHIFT	0x2
#define MODE_EL_MASK	0x3
#define MODE_EL3		0x3
#define MODE_EL2		0x2
#define MODE_EL1		0x1
#define MODE_EL0		0x0

#define MODE32_SHIFT	0
#define MODE32_MASK		0xf
#define MODE32_usr		0x0
#define MODE32_fiq		0x1
#define MODE32_irq		0x2
#define MODE32_svc		0x3
#define MODE32_mon		0x6
#define MODE32_abt		0x7
#define MODE32_hyp		0xa
#define MODE32_und		0xb
#define MODE32_sys		0xf

/* CPSR/SPSR definitions */
#define DAIF_FIQ_BIT	(1 << 0)
#define DAIF_IRQ_BIT	(1 << 1)
#define DAIF_ABT_BIT	(1 << 2)
#define DAIF_DBG_BIT	(1 << 3)
#define SPSR_DAIF_SHIFT	6
#define SPSR_DAIF_MASK	0xf

#define SPSR_AIF_SHIFT	6
#define SPSR_AIF_MASK	0x7

#define SPSR_E_SHIFT	9
#define SPSR_E_MASK		0x1
#define SPSR_E_LITTLE	0x0
#define SPSR_E_BIG		0x1

#define SPSR_T_SHIFT	5
#define SPSR_T_MASK		0x1
#define SPSR_T_ARM		0x0
#define SPSR_T_THUMB	0x1

#define DISABLE_ALL_EXCEPTIONS \
		(DAIF_FIQ_BIT | DAIF_IRQ_BIT | DAIF_ABT_BIT | DAIF_DBG_BIT)

#define SPSR_64(el, sp, daif)				\
	(MODE_RW_64 << MODE_RW_SHIFT |			\
	((el) & MODE_EL_MASK) << MODE_EL_SHIFT |	\
	((sp) & MODE_SP_MASK) << MODE_SP_SHIFT |	\
	((daif) & SPSR_DAIF_MASK) << SPSR_DAIF_SHIFT)

#define SPSR_MODE32(mode, isa, endian, aif)		\
	(MODE_RW_32 << MODE_RW_SHIFT |			\
	((mode) & MODE32_MASK) << MODE32_SHIFT |	\
	((isa) & SPSR_T_MASK) << SPSR_T_SHIFT |		\
	((endian) & SPSR_E_MASK) << SPSR_E_SHIFT |	\
	((aif) & SPSR_AIF_MASK) << SPSR_AIF_SHIFT)

typedef struct mstar_magic_key
{
	unsigned int version;
	unsigned long long entry_point;
	unsigned int RW64;					//Public aarch64 or aarch32
	unsigned int Exceptions;		//Public irq fiq data
	unsigned int IsNONSecure;			//Public
	unsigned int Mode;			//Public EL3/EL2/EL3 user/irq/fiq ...
	unsigned int SP_ELX;			//Private only aarch64
	unsigned int ISA;					//Private only aarch32
	unsigned int BigLittle;				//Private only aarch32
	unsigned int Reserve[20];
} mstar_magic_key_t;

#define STR_MODE_MASK 0x7 /*bit 0~2*/
#define AUTH_MODE_MASK 0x38 /*bit 3~6*/

#define COLD_BOOT 0x0
#define STR_BOOT 0x1
#define FAST_BOOT 0x2

#define AESCBC_MAC 0x8

typedef struct __attribute__((packed)) mstar_tee
{
	unsigned int cus_version;
	unsigned int reserve[2];
	unsigned int tee_encryptor_mode;
	unsigned int group_size;
	unsigned int init_group_addr_hi;
	unsigned int init_group_addr_lo;
	unsigned int warmboot_mode;
	unsigned int magic;
	unsigned char version;
	unsigned char arch_id;
	unsigned short flags;
	unsigned int init_size;
	unsigned int init_load_addr_hi;
	unsigned int init_load_addr_lo;
	unsigned int init_mem_usage;
	unsigned int paged_size;
	unsigned int secure_range_end;
} mstar_tee_t;
#endif /*__ASSEMBLY__*/

#endif /* __MSTAR_TRUSTZONE_H__ */
