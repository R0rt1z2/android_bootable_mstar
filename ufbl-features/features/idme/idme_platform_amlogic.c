/*
 * idme_platform_amlogic.c
 *
 * Copyright 2011 - 2019 Amazon.com, Inc. or its Affiliates. All rights reserved.
 */
#include <common.h>
#include <config.h>
#include <mmc.h>
#include <asm/byteorder.h>
#include <asm/unaligned.h>
#include "ufbl_debug.h"
#include <idme.h>

#define CFG_FASTBOOT_MMC_NO    (1)

/*
 * read entire idme
 * returns -1 when fails
 */
int idme_platform_read(unsigned char *pbuf)
{
	struct mmc* mmc;
	int nread = 0;
	u64 block_offset =0;

	if (!pbuf) {
		dprintf(CRITICAL, "Null pbuf used in %s\n", __FUNCTION__);
		return -1;
	}

	mmc = find_mmc_device(CFG_FASTBOOT_MMC_NO);
	if (!mmc) {
		dprintf(CRITICAL, "no mmc devices available\n");
		return -1;
	}

	/* switch to boot partition */
	/*
	 * Amlogic upated uboot v2019 for Hoya, which has different API
	 * definition
	 */
#if defined(UFBL_PROJ_HOYA)
	if (mmc_switch_part(mmc, CONFIG_IDME_PARTITION_NUM) != 0) {
#else
	if (mmc_switch_part(CFG_FASTBOOT_MMC_NO, CONFIG_IDME_PARTITION_NUM) != 0) {
#endif
		dprintf(CRITICAL, "ERROR: couldn't switch to boot partition\n");
		return -1;
	}

	/* Always use the ending part of boot partition since uboot is backed up from beginning
	   capacity is usually 4M */
	block_offset = mmc->capacity - IDME_NUM_OF_EMMC_BLOCKS * CONFIG_MMC_BLOCK_SIZE;
	dprintf(INFO, "%s block_offset=%lx, capacity=%lx\n",  __FUNCTION__, (long)block_offset, (long)mmc->capacity);

/*
 * Uboot V2009 introduces CONFIG_BLK for block access APIs.
 * mmc->block_dev only exists when CONFIG_BLK is not defined.
 * For older version of uboot, use old block device APIs.
 */
#ifndef CONFIG_BLK
	nread = mmc->block_dev.block_read(CFG_FASTBOOT_MMC_NO,
					block_offset/CONFIG_MMC_BLOCK_SIZE,
					IDME_NUM_OF_EMMC_BLOCKS, pbuf);
#else
	nread = blk_dread(mmc_get_blk_desc(mmc),
					block_offset/CONFIG_MMC_BLOCK_SIZE,
					IDME_NUM_OF_EMMC_BLOCKS, pbuf);
#endif

#if defined(UFBL_PROJ_HOYA)
	if (mmc_switch_part(mmc, 0) != 0) {
#else
	if (mmc_switch_part(CFG_FASTBOOT_MMC_NO, 0) != 0) {
#endif
		dprintf(CRITICAL, "ERROR: couldn't switch to user partition\n");
		return -1;
	}

	if (nread < 0) {
		dprintf(CRITICAL, "ERROR: idme read failure, nread %d\n", nread);
		return -1;
	}

	return 0;
}

/*
 * write idme
 * returns -1 when fails
 */
int idme_platform_write(const unsigned char *pbuf)
{
	struct mmc* mmc;
	int nwrite = 0;
	u64 block_offset = 0;

	if (!pbuf) {
		dprintf(CRITICAL, "Null pbuf used in %s\n", __FUNCTION__);
		return -1;
	}

	mmc = find_mmc_device(CFG_FASTBOOT_MMC_NO);
	if (!mmc) {
		dprintf(CRITICAL, "no mmc devices available\n");
		return -1;
	}

	/* switch to boot partition */
	/*
	 * Amlogic upated uboot v2019 for Hoya, which has different API
	 * definition
	 */
#if defined(UFBL_PROJ_HOYA)
	if (mmc_switch_part(mmc, CONFIG_IDME_PARTITION_NUM) != 0) {
#else
	if (mmc_switch_part(CFG_FASTBOOT_MMC_NO, CONFIG_IDME_PARTITION_NUM) != 0) {
#endif
		dprintf(CRITICAL, "ERROR: couldn't switch to boot partition\n");
		return -1;
	}

	/*
	 * Always use the ending part of boot partition since uboot is backed up from beginning
	 * capacity is usually 4M
	 */
	block_offset = mmc->capacity - IDME_NUM_OF_EMMC_BLOCKS * CONFIG_MMC_BLOCK_SIZE;
	dprintf(INFO, "%s block_offset=%lx, capacity=%lx\n",  __FUNCTION__, (long)block_offset, (long)mmc->capacity);

/*
 * Uboot V2009 introduces CONFIG_BLK for block access APIs.
 * mmc->block_dev only exists when CONFIG_BLK is not defined.
 * For older version of uboot, use old block device APIs.
 */
#ifndef CONFIG_BLK
	nwrite = mmc->block_dev.block_write(CFG_FASTBOOT_MMC_NO,
					block_offset/CONFIG_MMC_BLOCK_SIZE,
					IDME_NUM_OF_EMMC_BLOCKS, pbuf);
#else
	nwrite = blk_dwrite(mmc_get_blk_desc(mmc),
					block_offset/CONFIG_MMC_BLOCK_SIZE,
					IDME_NUM_OF_EMMC_BLOCKS, pbuf);
#endif

#if defined(UFBL_PROJ_HOYA)
	if (mmc_switch_part(mmc, 0) != 0) {
#else
	if (mmc_switch_part(CFG_FASTBOOT_MMC_NO, 0) != 0) {
#endif
		dprintf(CRITICAL, "ERROR: couldn't switch to user partition\n");
		return -1;
	}

	if (nwrite < 0) {
		dprintf(CRITICAL, "ERROR: idme write failure, nwrite %s\n", nwrite);
		return -1;
	}

	return 0;
}

