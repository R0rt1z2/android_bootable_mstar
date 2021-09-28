/*
 * idme_platform.c
 *
 * Copyright 2011 - 2019 Amazon.com, Inc. or its Affiliates. All rights reserved.
 */
#include <common.h>
#include <config.h>
#include <mmc.h>
#include <asm/byteorder.h>
#include <asm/unaligned.h>
#include <idme.h>

/* module debug */
//#define MODULE_DEBUG

#define CFG_FASTBOOT_MMC_NO 2
/* define how much back room to create for IDME data from top
currently we put 16KB*/
#define CFG_IDME_OFFSET_ROOM 16384
/* read entire idme */
int idme_platform_read(unsigned char *pbuf)
{
	struct mmc* mmc;
	int nread = 0;
	u64 block_offset =0;
	mmc = find_mmc_device(CFG_FASTBOOT_MMC_NO);

//	mmc_init(mmc);
	/* switch to boot partition */
	if (mmc_switch_part(CFG_FASTBOOT_MMC_NO, CONFIG_IDME_PARTITION_NUM) != 0) {
		printf("ERROR: couldn't switch to boot partition\n");
		return -1;
	}

	/* calculate offset depending on the capacity of boot partition below
	logic will choose the 1MB offset address for cards with larger than
	1 MB boot partition capacity and it will choose the down 16KB from
	upper limit for card with 1MB boot partition capacity.*/
	block_offset = mmc->capacity - CFG_IDME_OFFSET_ROOM;
	block_offset = MIN(block_offset, CONFIG_USERDATA_IDME_ADDR);

	nread = mmc->block_dev.block_read(CFG_FASTBOOT_MMC_NO,
					block_offset/CONFIG_MMC_BLOCK_SIZE,
					IDME_NUM_OF_EMMC_BLOCKS, pbuf);
	if (mmc_switch_part(CFG_FASTBOOT_MMC_NO, 0) != 0) {
		printf("ERROR: couldn't switch to user partition\n");
		return -1;
	}
#if defined(MODULE_DEBUG)
	hexdump8(pbuf, CONFIG_IDME_SIZE);
#endif
	return 0;
}

/* write idme */
int idme_platform_write(const unsigned char *pbuf)
{
	struct mmc* mmc;
	int nread = 0;
	u64 block_offset = 0;
	mmc = find_mmc_device(CFG_FASTBOOT_MMC_NO);

//	mmc_init(mmc);
	/* switch to boot partition */
	if (mmc_switch_part(CFG_FASTBOOT_MMC_NO, CONFIG_IDME_PARTITION_NUM) != 0) {
		printf("ERROR: couldn't switch to boot partition\n");
		return -1;
	}

	/* calculate offset depending on the capacity of boot partition below
	logic will choose the 1MB offset address for cards with larger than
	1 MB boot partition capacity and it will choose the down 16KB from
	upper limit for card with 1MB boot partition capacity.*/
	block_offset = mmc->capacity - CFG_IDME_OFFSET_ROOM;
	block_offset = MIN(block_offset, CONFIG_USERDATA_IDME_ADDR);

	nread = mmc->block_dev.block_write(CFG_FASTBOOT_MMC_NO,
					block_offset/CONFIG_MMC_BLOCK_SIZE,
					IDME_NUM_OF_EMMC_BLOCKS, pbuf);
	if (mmc_switch_part(CFG_FASTBOOT_MMC_NO, 0) != 0) {
		printf("ERROR: couldn't switch to user partition\n");
		return -1;
	}
#if defined(MODULE_DEBUG)
	hexdump8(pbuf, CONFIG_IDME_SIZE);
#endif
	return 0;
}

