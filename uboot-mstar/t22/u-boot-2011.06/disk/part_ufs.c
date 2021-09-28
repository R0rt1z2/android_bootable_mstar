/*
 * See also Linux sources, fs/partitions/mac.h
 *
 * This file describes structures and values related to the standard
 * Apple SCSI disk partitioning scheme. For more information see:
 * http://developer.apple.com/techpubs/mac/Devices/Devices-126.html#MARKER-14-92
 * Refined by rui.wang@mstarsemi.com
 */
 
#include <common.h>
#include <command.h>
#include <malloc.h>
#include "part_ufs.h"

#if defined(CONFIG_MSTAR_UFS)
static int part_ufs_read_ddb (block_dev_desc_t *dev_desc, ufs_driver_desc_t *ddb_p);
static int part_ufs_read_pdb (block_dev_desc_t *dev_desc, int part, ufs_partition_t *pdb_p);
static int part_ufs_write_ddb (block_dev_desc_t *dev_desc, ufs_driver_desc_t *ddb_p);
static int part_ufs_write_pdb (block_dev_desc_t *dev_desc, ufs_partition_t *pdb_p);

#if 0
/*
 * Test for a valid UFS partition
 */
int test_part_ufs (block_dev_desc_t *dev_desc)
{
	ufs_driver_desc_t ddesc;
	ufs_partition_t mpart;
	ulong i, n;

	if (part_ufs_read_ddb (dev_desc, &ddesc)) {
		/* error reading Driver Desriptor Block, or no valid Signature */
		return (-1);
	}

	n = 0;
	for (i=1; i<=UFS_RESERVED_FOR_MAP ; i++) {
		if (dev_desc->block_read(dev_desc->dev, i, 1, (ulong *)&mpart) != 1) {
			return (-1);
		}

		if (be32_to_cpu(mpart.signature) != UFS_PARTITION_MAGIC)
			break;

		/* update partition count */
		n++;
	}

	return (0);
}
#endif

void print_part_ufs (block_dev_desc_t *dev_desc)
{
	ulong i;
	ufs_driver_desc_t ddesc;
	ufs_partition_t mpart;

	if (part_ufs_read_ddb (dev_desc, &ddesc)) {
		/* error reading Driver Desriptor Block, or no valid Signature */
		return;
	}

	printf ("   #:                 type name                   length   base       (size)\n");

	for (i=1; i<=UFS_RESERVED_FOR_MAP; ++i) {
		ulong bytes=0, bytes_tail=0;
		char c;

		if (dev_desc->block_read (dev_desc->dev, i, 1, (void *)&mpart) != 1) {
			printf ("** Can't read Partition Map on %d:%ld **\n",
				dev_desc->dev, i);
			return;
		}

		if (be32_to_cpu(mpart.signature) != UFS_PARTITION_MAGIC) {
			printf("\n");
			return;
		}

		printf ("%4ld: ", i);
        /* update partition count */

		c = 'k';
		bytes = be32_to_cpu(mpart.block_count);
		bytes *= (be16_to_cpu(ddesc.blk_size)>>10);
		if (bytes >= 1024) {
			bytes_tail = (bytes & 1023) * 1000 /1024;
			bytes >>= 10;
			c = 'M';
		}
		if (bytes >= 1024) {
			bytes_tail = (bytes & 1023) * 1000 /1024;
			bytes >>= 10;
			c = 'G';
		}
		printf("%20.32s %-18.32s %10u @ %-10u (%ld.%03ld%c)\n",
				mpart.type,
				mpart.name,
				be32_to_cpu(mpart.block_count),
				be32_to_cpu(mpart.start_block),
				bytes, bytes_tail, c);
	}

	return;
}


/*
 * Read Device Descriptor Block
 */
static int part_ufs_read_ddb (block_dev_desc_t *dev_desc, ufs_driver_desc_t *ddb_p)
{
	if (dev_desc->block_read(dev_desc->dev, 0, 1, (void *)ddb_p) != 1) {
		printf ("** Can't read Driver Desriptor Block **\n");
		return (-1);
	}

	if (be32_to_cpu(ddb_p->signature) != UFS_DRIVER_MAGIC) {
#if 0
		printf ("** Bad Signature: expected 0x%04x, got 0x%04x\n",
			UFS_DRIVER_MAGIC, ddb_p->signature);
#endif
		return (-1);
	}
	return (0);
}

/*
 * Read Partition Descriptor Block
 */
static int part_ufs_read_pdb (block_dev_desc_t *dev_desc, int part, ufs_partition_t *pdb_p)
{
	if ((part < 1) || (part >UFS_RESERVED_FOR_MAP)) {
		printf ("** Invalid partition %d:%d\n", dev_desc->dev, part);
		return (-1);
	}

	if (dev_desc->block_read (dev_desc->dev, part, 1, (void *)pdb_p) != 1) {
		printf ("** Can't read Partition Map on %d:%d **\n",
			dev_desc->dev, part);
		return (-1);
	}

	if (be32_to_cpu(pdb_p->signature) != UFS_PARTITION_MAGIC) {
		printf ("** Bad Signature on %d:%d: "
			"expected 0x%08x, got 0x%08x\n",
			dev_desc->dev, part, UFS_PARTITION_MAGIC, be32_to_cpu(pdb_p->signature));
		return (-1);
	}

	return 0;
}

static int part_ufs_write_ddb (block_dev_desc_t *dev_desc, ufs_driver_desc_t *ddb_p)
{
	int i;
	ufs_partition_t mpart;

	if(be32_to_cpu(ddb_p->signature) != UFS_DRIVER_MAGIC){
		printf ("** Bad Signature: expected 0x%08x, try to write 0x%08x **\n",
				UFS_DRIVER_MAGIC, be32_to_cpu(ddb_p->signature));
			return (-1);
	}

	if (dev_desc->block_write(dev_desc->dev, 0, 1, (const void *)ddb_p) != 1) {
		printf ("** Can't write Driver Desriptor Block **\n");
		return (-1);
	}

	memset(&mpart, 0x00, sizeof(ufs_partition_t));
	/* Default MBOOT Partition */
	i = 1;
	mpart.signature = cpu_to_be32(UFS_PARTITION_MAGIC);
	mpart.start_block = cpu_to_be32(UFS_MBOOT_PATITION_START/dev_desc->blksz);
	mpart.block_count= cpu_to_be32(UFS_MBOOT_PATITION_SIZE/dev_desc->blksz);
	strcpy((char *)mpart.name, "MBOOT");
	if (dev_desc->block_write(dev_desc->dev, i, 1, (const void *)&mpart) != 1) {
		printf ("** Can't write Partition Map %d **\n", i);
		return (-1);
	}

	/* Default MPOOL Partition */
	i++;
	mpart.start_block = cpu_to_be32(UFS_MPOOL_PATITION_START/dev_desc->blksz);
	mpart.block_count= cpu_to_be32(UFS_MPOOL_PATITION_SIZE/dev_desc->blksz);
	strcpy((char *)mpart.name, "MPOOL");
	if (dev_desc->block_write(dev_desc->dev, i, 1, (const void *)&mpart) != 1) {
		printf ("** Can't write Partition Map %d **\n", i);
		return (-1);
	}

	#ifdef CONFIG_DOUBLE_MBOOT
	/* Default MBOOTBAK Partition */
	i++;
	mpart.start_block = cpu_to_be32(UFS_MBOOTBAK_PATITION_START/dev_desc->blksz);
	mpart.block_count= cpu_to_be32(UFS_MBOOTBAK_PATITION_SIZE/dev_desc->blksz);
	strcpy((char *)mpart.name, "MBOOTBAK");
	if (dev_desc->block_write(dev_desc->dev, i, 1, (const void *)&mpart) != 1) {
		printf ("** Can't write Partition Map %d **\n", i);
		return (-1);
	}
	#endif

	i++;
	memset(&mpart, 0x00, sizeof(ufs_partition_t));
	for (; i<=UFS_RESERVED_FOR_MAP; ++i) {
		if (dev_desc->block_write(dev_desc->dev, i, 1, (const void *)&mpart) != 1) {
			printf ("** Can't write Partition Map %d **\n", i);
			return (-1);
		}
	}

	return (0);	
}

static int part_ufs_write_pdb (block_dev_desc_t *dev_desc, ufs_partition_t *pdb_p)
{
	int n = 1;
	ufs_partition_t exist_pdb;
	ulong start_block = UFS_PARTITION_START/dev_desc->blksz;
        
	if(be32_to_cpu(pdb_p->signature) != UFS_PARTITION_MAGIC){
		printf ("** Bad Signature: expected 0x%08x, try to write 0x%08x **\n",
				UFS_PARTITION_MAGIC, be32_to_cpu(pdb_p->signature));
		return (-1);
	}

	for (;;) {
		/*
		* We must always read the descritpor block for
		* partition 1 first since this is the only way to
		* know how many partitions we have.
		*/
		if (dev_desc->block_read (dev_desc->dev, n, 1, (void *)&exist_pdb) != 1) {
			printf ("** Can't read Partition Map on %d:%d **\n",
					dev_desc->dev, n);
			return (-1);
		}

		if(be32_to_cpu(exist_pdb.signature) == UFS_PARTITION_MAGIC){
			if(!strcmp((const char *)pdb_p->name, (const char *)exist_pdb.name)){
				printf("** Try to change a exist partition %s **\n", exist_pdb.name);
				if(be32_to_cpu(pdb_p->block_count) > be32_to_cpu(exist_pdb.block_count)){
					printf("** The new size of the partition is too big!\n");
					return (-1);
				}

				pdb_p->start_block = exist_pdb.start_block;

				if(be32_to_cpu(pdb_p->start_block) + be32_to_cpu(pdb_p->block_count) > dev_desc->lba){
					printf("** Partition exceed emmc capacity**\n");
					return (-1);
				}                         

				if(dev_desc->block_write(dev_desc->dev, n, 1, (const void *)pdb_p) != 1){
					printf("** Can't write Driver Desiptor Block **\n");
					return (-1);
				}

				return (0);
			}

			if(be32_to_cpu(exist_pdb.start_block) + be32_to_cpu(exist_pdb.block_count) > UFS_PARTITION_START/dev_desc->blksz)
				/* skip UBOOT partition */
				start_block += be32_to_cpu(exist_pdb.block_count);
                
			++n;
			if(n > UFS_RESERVED_FOR_MAP){
				printf("** Exceed the max number of partition, STOP!**\n");
				return (-1);
			}
        }
		else {
			pdb_p->start_block = be32_to_cpu(start_block);

			if(be32_to_cpu(pdb_p->start_block) + be32_to_cpu(pdb_p->block_count) > dev_desc->lba) {
				printf("** Partition exceed emmc capacity**\n");
				return (-1);
			}

			if(dev_desc->block_write(dev_desc->dev, n, 1, (const void *)pdb_p) != 1){
				printf("** Can't write Driver Desiptor Block **\n");
				return (-1);
			}

			return (0);
		}       
	}
	/* NOTREACHED */
}


int get_partition_info_ufs (block_dev_desc_t *dev_desc, int part, disk_partition_t *info)
{
	ufs_driver_desc_t	ddesc;
	ufs_partition_t		mpart;

	if ((part < 1) || (part >UFS_RESERVED_FOR_MAP)) {
		return (-1);
	}

	if (part_ufs_read_ddb (dev_desc, &ddesc)) {
		return (-1);
	}

	info->blksz = be32_to_cpu(ddesc.blk_size);

	if (part_ufs_read_pdb (dev_desc, part, &mpart)) {
		return (-1);
	}

	info->start = be32_to_cpu(mpart.start_block);
	info->size  = be32_to_cpu(mpart.block_count);
	memcpy (info->type, mpart.type, sizeof(info->type));
	memcpy (info->name, mpart.name, sizeof(info->name));

	return (0);
}

int add_new_ufs_partition(block_dev_desc_t *dev_desc, disk_partition_t *info)
{
	int ret;
	ufs_driver_desc_t ddb_base;
	ufs_partition_t pdb_base;

	if(part_ufs_read_ddb(dev_desc, &ddb_base)){
		printf("Need write new driver description table!\n");

		memset(&ddb_base, 0, sizeof(ufs_driver_desc_t));
		ddb_base.signature = cpu_to_be32(UFS_DRIVER_MAGIC);
		ddb_base.blk_size = cpu_to_be16(dev_desc->blksz);
		ddb_base.blk_count = cpu_to_be32(dev_desc->lba);
		ddb_base.dev_type = cpu_to_be16(dev_desc->type);
		ddb_base.dev_id = cpu_to_be16(dev_desc->target);
		ddb_base.drvr_cnt = cpu_to_be16(UFS_RESERVED_FOR_MAP);
		ret = part_ufs_write_ddb(dev_desc, &ddb_base);
		if(ret){
			printf("Error during write new driver description table!\n");
			return ret;
		}
	}

	if(info) {
		memset(&pdb_base, 0, sizeof(ufs_partition_t));
		pdb_base.signature = cpu_to_be32(UFS_PARTITION_MAGIC);
		pdb_base.start_block = cpu_to_be32(info->start);
		pdb_base.block_count = cpu_to_be32(info->size);
		strcpy((char *)pdb_base.name, (const char *)info->name);
		//strcpy((char *)pdb_base.type, (const char *)info->type);
		ret = part_ufs_write_pdb(dev_desc, &pdb_base);
		if(ret){
			printf("Error during write new partition description table!\n");
			return ret;
		}
	}

	return 0;        
}

int remove_ufs_partition(block_dev_desc_t *dev_desc, disk_partition_t *info)
{
	int n = 0;
	ufs_partition_t exist_pdb, pdb;        

	pdb.signature = 0;
       
	for (;;) 
	{
		/*
		* We must always read the descritpor block for
		* partition 1 first since this is the only way to
		* know how many partitions we have.
		*/
		if(n > UFS_RESERVED_FOR_MAP)
		{
			printf("** Cannot remove the partition, not found **\n");
			return (-1);
		}

		if (dev_desc->block_read (dev_desc->dev, n, 1, (ulong *)&exist_pdb) != 1) 
		{
			printf ("** Can't read Partition Map on %d:%d **\n", dev_desc->dev, n);
			return (-1);
		}

		if(be32_to_cpu(exist_pdb.signature) == UFS_PARTITION_MAGIC)
		{
			if(!strcmp((const char *)info->name, (const char *)exist_pdb.name))
			{
				if(dev_desc->block_write(dev_desc->dev, n, 1, (ulong *)&pdb) != 1)
				{
					printf("** Can't remove the partition %s **\n", exist_pdb.name);
					return (-1);
				}
				return (0);
			}
		}
		++n;                                
	}
        
	return (-1);        
}

int _get_ufs_partsize (block_dev_desc_t *dev_desc, char *puPartName, unsigned int *u32PartSize)
{
	int i;
	ufs_driver_desc_t ddb_base;
	ufs_partition_t pdb_base;

	if(part_ufs_read_ddb(dev_desc, &ddb_base)) {
		return (-1);
	}
   
    for(i=1 ; i<= UFS_RESERVED_FOR_MAP ; i++) {
		if(part_ufs_read_pdb(dev_desc, i, &pdb_base) == 0) {
			if(strcmp(puPartName, (const char *)pdb_base.name) == 0) {
				*u32PartSize = be32_to_cpu(pdb_base.block_count)*dev_desc->blksz;
				printf("%s size : 0x%x\n", puPartName, *u32PartSize);
				break;
			}
		}
	}

	return 0;
}

int rmgpt_ufs_partition(block_dev_desc_t *dev_desc)
{
	int ret;
	ufs_driver_desc_t ddb_base;

	memset(&ddb_base, 0, sizeof(ufs_driver_desc_t));
	ddb_base.signature = cpu_to_be32(UFS_DRIVER_MAGIC);
	ddb_base.blk_size = cpu_to_be16(dev_desc->blksz);
	ddb_base.blk_count = cpu_to_be32(dev_desc->lba);
	ddb_base.dev_type = cpu_to_be16(dev_desc->type);
	ddb_base.dev_id = cpu_to_be16(dev_desc->target);
	ddb_base.drvr_cnt = cpu_to_be16(UFS_RESERVED_FOR_MAP);
	ret = part_ufs_write_ddb(dev_desc, &ddb_base);
	if(ret){
		printf("Error during write new driver description table!\n");
		return ret;
	}

	return 0;
}
#endif
