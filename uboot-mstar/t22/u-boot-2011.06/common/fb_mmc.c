/*
 * Copyright 2014 Broadcom Corporation.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <config.h>
#include <common.h>
#include <part.h>
//#include <fastboot.h>
#include <fb_mmc.h>
#include <image-sparse.h>
#include <mmc.h>
#include <div64.h>

/*
 * FIXME: Ensure we always set these names via Kconfig once xxx_PARTITION is
 * migrated
 */

#define CONFIG_FASTBOOT_FLASH_MMC_DEV 0

struct fb_mmc_sparse {
	 block_dev_desc_t	*dev_desc;
};


static lbaint_t fb_mmc_sparse_write(struct sparse_storage *info,
		lbaint_t blk, lbaint_t blkcnt, const void *buffer)
{
	struct fb_mmc_sparse *sparse = info->priv;
	block_dev_desc_t *dev_desc = sparse->dev_desc;

	return dev_desc->block_write(dev_desc->dev, blk, blkcnt, buffer);
}

static lbaint_t fb_mmc_sparse_reserve(struct sparse_storage *info,
		lbaint_t blk, lbaint_t blkcnt)
{
	return blkcnt;
}

static void write_raw_image(block_dev_desc_t *dev_desc, disk_partition_t *info,
		const char *part_name, void *buffer,
		unsigned int download_bytes)
{
	lbaint_t blkcnt;
	lbaint_t blks;

	/* determine number of blocks to write */
	blkcnt = ((download_bytes + (info->blksz - 1)) & ~(info->blksz - 1));
	blkcnt = lldiv(blkcnt, info->blksz);

	if (blkcnt > info->size) {
		error("too large for partition: '%s'\n", part_name);
		fastboot_fail("too large for partition");
		return;
	}

	puts("Flashing Raw Image\n");

	blks = dev_desc->block_write(dev_desc->dev, info->start, blkcnt, buffer);
	if (blks != blkcnt) {
		error("failed writing to device %d\n", dev_desc->dev);
		fastboot_fail("failed writing to device");
		return;
	}

	printf("........ wrote " LBAFU " bytes to '%s'\n", blkcnt * info->blksz,
	       part_name);
	fastboot_okay("");
}

void fb_mmc_flash_write(const char *cmd, void *download_buffer,
			unsigned int download_bytes)
{
	block_dev_desc_t *dev_desc;
	disk_partition_t info;
	u32 partnum = 1;

	dev_desc = get_dev("mmc", CONFIG_FASTBOOT_FLASH_MMC_DEV);
	if (!dev_desc || dev_desc->type == DEV_TYPE_UNKNOWN) {
		error("invalid mmc device\n");
		fastboot_fail("invalid mmc device");
		return;
	}

	if (!get_partition_info(dev_desc, partnum, &info)){
		for(;;){
			if(get_partition_info(dev_desc, partnum, &info)){
				error("get partition info failed, partnum=%d\n",partnum);
				fastboot_fail("get partition info failed, partnum=%d",partnum);
				return;
			}
			if(!strcmp(cmd, (const char *)info.name))
				break;
			partnum++;
		}
       }else{
		error("get partition info failed, partnum=%d\n",partnum);
		fastboot_fail("get partition info failed, partnum=%d",partnum);
		return;
       }

	if (is_sparse_image(download_buffer)) {
		struct fb_mmc_sparse sparse_priv;
		struct sparse_storage sparse;

		sparse_priv.dev_desc = dev_desc;

		sparse.blksz = info.blksz;
		sparse.start = info.start;
		sparse.size = info.size;
		sparse.write = fb_mmc_sparse_write;
		sparse.reserve = fb_mmc_sparse_reserve;

		printf("Flashing sparse image at offset " LBAFU "\n",
		       sparse.start);

		sparse.priv = &sparse_priv;
		write_sparse_image(&sparse, cmd, download_buffer,
				   download_bytes);
	} else {
		write_raw_image(dev_desc, &info, cmd, download_buffer,
				download_bytes);
	}
}

void fb_mmc_erase(const char *cmd)
{
	int ret;
	int partnum = 1;
	block_dev_desc_t *dev_desc;
	disk_partition_t info;
	lbaint_t blks, blks_start, blks_size, grp_size;
	struct mmc *mmc = find_mmc_device(CONFIG_FASTBOOT_FLASH_MMC_DEV);

	if (mmc == NULL) {
		error("invalid mmc device");
		fastboot_fail("invalid mmc device");
		return;
	}

	dev_desc = get_dev("mmc", CONFIG_FASTBOOT_FLASH_MMC_DEV);
	if (!dev_desc || dev_desc->type == DEV_TYPE_UNKNOWN) {
		error("invalid mmc device");
		fastboot_fail("invalid mmc device");
		return;
	}

	if (!get_partition_info(dev_desc, partnum, &info)){
		for(;;){
			if(get_partition_info(dev_desc, partnum, &info)){
				error("get partition info failed, partnum=%d\n",partnum);
				fastboot_fail("get partition info failed, partnum=%d",partnum);
				return;
			}
			if(!strcmp(cmd, (const char *)info.name))
				break;
			partnum++;
		}
       }else{
		error("get partition info failed, partnum=%d\n",partnum);
		fastboot_fail("get partition info failed, partnum=%d",partnum);
		return;
       }

	/* Align blocks to erase group size to avoid erasing other partitions */
	//grp_size = mmc->erase_grp_size;
	grp_size = 512;
	printf("dev_desc->blksz = %d\n",dev_desc->blksz);
	blks_start = (info.start + grp_size - 1) & ~(grp_size - 1);
	if (info.size >= grp_size)
		blks_size = (info.size - (blks_start - info.start)) &
				(~(grp_size - 1));
	else
		blks_size = 0;

	printf("Erasing blocks " LBAFU " to " LBAFU " due to alignment\n",
	       blks_start, blks_start + blks_size);

	blks = dev_desc->block_erase(dev_desc->dev, blks_start, blks_size);
	if (blks != blks_size) {
		error("failed erasing from device %d", dev_desc->dev);
		fastboot_fail("failed erasing from device");
		return;
	}

	printf("........ erased " LBAFU " bytes from '%s'\n",
	       blks_size * info.blksz, cmd);
	fastboot_okay("");
}
