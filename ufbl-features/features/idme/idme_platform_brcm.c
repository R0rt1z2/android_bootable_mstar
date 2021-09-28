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

/* read entire idme */
int idme_platform_read(unsigned char *pbuf)
{
	struct mmc* mmc;
	int nread = 0;

	mmc = find_mmc_device(CFG_FASTBOOT_MMC_NO);

//	mmc_init(mmc);
	/* switch to boot partition */
	if (mmc_switch_part(CFG_FASTBOOT_MMC_NO, CONFIG_IDME_PARTITION_NUM) != 0) {
		printf("ERROR: couldn't switch to boot partition\n");
		return -1;
	}

	nread = mmc->block_dev.block_read(CFG_FASTBOOT_MMC_NO,
					CONFIG_USERDATA_IDME_ADDR,
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

	mmc = find_mmc_device(CFG_FASTBOOT_MMC_NO);

//	mmc_init(mmc);
	/* switch to boot partition */
	if (mmc_switch_part(CFG_FASTBOOT_MMC_NO, CONFIG_IDME_PARTITION_NUM) != 0) {
		printf("ERROR: couldn't switch to boot partition\n");
		return -1;
	}

	nread = mmc->block_dev.block_write(CFG_FASTBOOT_MMC_NO,
					CONFIG_USERDATA_IDME_ADDR,
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

