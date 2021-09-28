/*
 * See also Linux sources, fs/partitions/mac.h
 *
 * This file describes structures and values related to the standard
 * Apple SCSI disk partitioning scheme. For more information see:
 * http://developer.apple.com/techpubs/mac/Devices/Devices-126.html#MARKER-14-92
 * Refined by rui.wang@mstarsemi.com
 */
#include <mmc.h>

#ifndef _DISK_PART_EMMC_H
#define _DISK_PART_EMMC_H

/*      0 ~ 32KB for partition table. 
    32KB ~ 64KB reserved for partition table, but not used
    64KB ~   2MB reserved for emmc driver test
      2MB ~  7MB  reserved for chunk header, pm, uboot and env
      7MB ~          ordinary partition data start
*/

//#define CONFIG_DOUBLE_MBOOT

#ifdef CONFIG_DOUBLE_MBOOT
#define EMMC_MBOOT_PATITION_START_V2 0x200000
#define EMMC_MBOOT_PATITION_SIZE_V2 0x300000
#define EMMC_MPOOL_PATITION_START_V2 0x500000
#define EMMC_MPOOL_PATITION_SIZE_V2 0x200000
#define EMMC_MBOOT_PATITION_START_V3 0x200000
#define EMMC_MBOOT_PATITION_SIZE_V3 0x300000
#define EMMC_MPOOL_PATITION_START_V3 0x500000
#define EMMC_MPOOL_PATITION_SIZE_V3 0x200000
#define EMMC_MBOOTBAK_PATITION_START_V3 0x1000000
#define EMMC_MBOOTBAK_PATITION_SIZE_V3 0x300000
#else
#define EMMC_MBOOT_PATITION_START 0x200000
#define EMMC_MBOOT_PATITION_SIZE 0x300000
#define EMMC_MPOOL_PATITION_START 0x500000
#define EMMC_MPOOL_PATITION_SIZE 0x200000
#endif

#define EMMC_MBOOTA_PAR_START     0x200000
#define EMMC_MBOOTA_PAR_SIZE      0xF00000
#define EMMC_MPOOLA_PAR_START     (EMMC_MBOOTA_PAR_START + EMMC_MBOOTA_PAR_SIZE)
#define EMMC_MPOOLA_PAR_SIZE      0xA00000
#define EMMC_MBOOTB_PAR_START     (EMMC_MPOOLA_PAR_START + EMMC_MPOOLA_PAR_SIZE)
#define EMMC_MBOOTB_PAR_SIZE      0xF00000
#define EMMC_MPOOLB_PAR_START     (EMMC_MBOOTB_PAR_START + EMMC_MBOOTB_PAR_SIZE)
#define EMMC_MPOOLB_PAR_SIZE      0xA00000
#define EMMC_PARTITION_SAVE_NUM     4

#define EMMC_RESERVED_FOR_MAP 10         
#define EMMC_RESERVED_FOR_MAP_V2 63   //0~32kb for partition table. 32kb~64kb reserved for partition table, but not used
#ifdef CONFIG_DOUBLE_MBOOT
#define EMMC_RESERVED_FOR_MAP_V3 EMMC_RESERVED_FOR_MAP_V2   //0~32kb for partition table. 32kb~64kb reserved for partition table, but not used
#endif
#define EMMC_CHUNK_HEAD_START 4096        // chunk head start addr

#ifdef CONFIG_DOUBLE_MBOOT
#define EMMC_PARTITION_START_V1 12       // ordinary partition data start addr
#define EMMC_PARTITION_START_V2 14336       // ordinary partition data start addr
#define EMMC_PARTITION_START_V3 65536       // ordinary partition data start addr
#elif defined(CONFIG_MTK_LOADER)
// ordinary partition data start addr
#define EMMC_PARTITION_START ((EMMC_MPOOLB_PAR_START + EMMC_MPOOLB_PAR_SIZE)/512)
#else
#define EMMC_PARTITION_START 14336       // ordinary partition data start addr
#endif

#define EMMC_DRIVER_MAGIC   0x1630

/*
 * Driver Descriptor Structure, in block 0.
 * This block is (and shall remain) 512 bytes long.
 * Note that there is an alignment problem for the driver descriptor map!
 */
typedef struct emmc_driver_desc {
        __u16   signature;           /* expected to be EMMC_DRIVER_MAGIC  */
        __u16   blk_size;             /* block size of device */
        __u32   blk_count;           /* number of blocks on device */
        __u16   dev_type;            /* device type */
        __u16   dev_id;                /* device id */
        __u32   data;                   /* reserved */
        __u16   version;               /* version number of partition table */
        __u16   drvr_cnt;             /* number of blocks reserved for partition table */
        __u16   drvr_map[246];    /* driver descriptor map */
} emmc_driver_desc_t;

/*
* To reduce the memery of ddb struct when read the partition table.
*/
typedef struct emmc_driver_desc2 {
        __u16   signature;           /* expected to be EMMC_DRIVER_MAGIC  */
        __u16   blk_size;             /* block size of device */
        __u32   blk_count;           /* number of blocks on device */
	 __u16   dev_type;            /* device type */
        __u16   dev_id;                /* device id */
        __u32   data;                   /* reserved */
        __u16   version;               /* version number of partition table */
        __u16   drvr_cnt;             /* number of blocks reserved for partition table */
} emmc_driver_desc2_t;

/*
 * Device Driver Entry
 * (Cannot be included in emmc_driver_desc because of alignment problems)
 */
typedef struct emmc_driver_entry {
        __u32   block;          /* block number of starting block */
        __u16   size;            /* size of driver, in 512 byte blocks */
        __u16   type;           /* OS Type */
} emmc_driver_entry_t;


#define EMMC_PARTITION_MAGIC    0x5840
#define EMMC_PARTITIONTABLE_VERSION2 0x2000
#ifdef CONFIG_DOUBLE_MBOOT
#define EMMC_PARTITIONTABLE_VERSION3 0x4000
#endif

/* type field value for A/UX or other Unix partitions */
#define APPLE_AUX_TYPE  "Apple_UNIX_SVR2"

/*
 * Each Partition Map entry (in blocks 1 ... N) has this format:
 */
typedef struct EMMC_partition {
        __u16   signature;          /* expected to be EMMC_PARTITION_MAGIC   */
        __u16   sig_pad;            /* reserved */
        __u32   map_count;       /* blocks in partition map   */
        __u32   start_block;       /* abs. starting block # of partition    */
        __u32   block_count;      /* number of blocks in partition  */
        uchar    name[32];          /* partition name   */
        uchar    type[32];           /* string type description */
        __u32   data_start;        /* rel block # of first data block   */
        __u32   data_count;       /* number of data blocks  */
        __u32   status;               /* partition status bits  */
        __u32   boot_start;         /* first block of boot code */
        __u32   boot_size;          /* size of boot code, in bytes  */
        __u32   boot_load;          /* boot code load address   */
        __u32   boot_load2;        /* reserved  */
        __u32   boot_entry;        /* boot code entry point */
        __u32   boot_entry2;      /* reserved   */
        __u32   boot_cksum;      /* boot code checksum  */
        uchar    processor[16];    /* Type of Processor */
        __u16   part_pad[188];   /* reserved    */
#ifdef CONFIG_ISO_PARTITION
	uchar   iso_dummy[2048];/* Reservere enough room for an ISO partition block to fit */
#endif
} emmc_partition_t;

/*
 * To reduce the memery of pdb struct when read the partition table.
 */
typedef struct EMMC_partition2{
__u16   signature;          /* expected to be EMMC_PARTITION_MAGIC   */
__u16   sig_pad;            /* reserved */
__u32   map_count;       /* blocks in partition map   */
__u32   start_block;       /* abs. starting block # of partition    */
__u32   block_count;      /* number of blocks in partition  */
uchar    name[32];          /* partition name   */
uchar    type[32];           /* string type description */
}emmc_partition2_t;

#define EMMC_STATUS_BOOTABLE    8   /* partition is bootable */

int rmgpt_emmc_partition(int curr_device, char *symbol);
int clone_part(int curr_device, char * const source, char * const destination);
void erase_customer_partition(int curr_device, struct mmc *mmc, s32 *start, u32 *cnt);
int _rmgpt_sync(int i);
int _erase_sync(block_dev_desc_t *dev_desc, int start, int cnt);
int print_info_part(block_dev_desc_t *dev_desc, char *name);
void get_version_emmc(block_dev_desc_t *dev_desc);
void Configure_Boot(block_dev_desc_t *dev_desc, disk_partition_t *info, int tag);
ulong Define_Boot_size(block_dev_desc_t *dev_desc, int tag);
ulong Define_Boot_start(block_dev_desc_t *dev_desc, int tag);

#ifdef CONFIG_DOUBLE_MBOOT
int Configure_MBOOTBAK(block_dev_desc_t *dev_desc, disk_partition_t *info);
ulong Define_MBOOTBAK(block_dev_desc_t *dev_desc, int tag);
#endif
int check_ca_partition_emmc(void);
int add_ca_partition_emmc(void);
int access_ca_partition_emmc(char *partition_name);

#endif  /* _DISK_PART_EMMC_H */
