#ifndef _PART_UFS_H_
#define _PART_UFS_H_

#include <compiler.h>
#include <linux/types.h>

#define UFS_MBOOT_PATITION_START		0x200000
#define UFS_MBOOT_PATITION_SIZE			0x500000
#define UFS_MPOOL_PATITION_START		(UFS_MBOOT_PATITION_START+UFS_MBOOT_PATITION_SIZE)
#define UFS_MPOOL_PATITION_SIZE			0x500000

#ifdef CONFIG_DOUBLE_MBOOT
#define UFS_MBOOTBAK_PATITION_START		(UFS_MPOOL_PATITION_START+UFS_MPOOL_PATITION_SIZE)
#define UFS_MBOOTBAK_PATITION_SIZE		0x500000

#define UFS_PARTITION_START				(UFS_MBOOTBAK_PATITION_START+UFS_MBOOTBAK_PATITION_SIZE)
#else
#define UFS_PARTITION_START				(UFS_MPOOL_PATITION_START+UFS_MPOOL_PATITION_SIZE)
#endif

#define UFS_RESERVED_FOR_MAP			63

#define UFS_IPVERIFY_PATITION_START		0x100000
#define UFS_IPVERIFY_PATITION_SIZE		0x100000

#define UFS_PARTITION_MAGIC	0x55465350	/* "UFSP" */

typedef struct ufs_partition {
	__be32	signature;		/* expected to be UFS_PARTITION_MAGIC   */
	__be32	map_count;		/* blocks in partition map   */
	__be32	start_block;	/* abs. starting block # of partition    */
	__be32	block_count;	/* number of blocks in partition  */
	char	name[32];		/* partition name */
	char	type[32];		/* string type description */
	__u16	part_pad[2008];	/* reserved */	
}ufs_partition_t;

#define UFS_DRIVER_MAGIC	0x55465344	/* "UFSD" */

/* Driver descriptor structure, in block 0 */
typedef struct ufs_driver_desc {
	__be32	signature;		/* expected to be UFS_DRIVER_MAGIC  */
	__be16	blk_size;		/* block size of device */
	__be16	pad;			/* reserved */
	__be32	blk_count;		/* number of blocks on device */
	__be16	dev_type;		/* device type */
	__be16	dev_id;			/* device id */
	__be32	data;			/* reserved */
	__be16	version;		/* version number of partition table */
	__be16	drvr_cnt;		/* number of blocks reserved for partition table */
	__be16	drvr_map[2036];	/* driver descriptor map		*/
}ufs_driver_desc_t;
	
#endif  /* _PART_UFS_H_ */
