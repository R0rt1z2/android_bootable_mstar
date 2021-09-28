/*
 * idme_platform.c
 *
 * Copyright 2011 - 2019 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 */

#include "idme.h"
#include <stdlib.h>
#include <string.h>
#include <debug.h>
#ifndef UFBL_TESTS
#include "fastboot.h"
#include <platform/mmc_core.h>
#endif
#include <idme.h>
/* module debug */
//#define MODULE_DEBUG
#ifndef UFBL_TESTS
//proto (from mmc_core.c)
int mmc_part_read(struct mmc_card *card, u8 partno, unsigned long blknr, u32 blkcnt, unsigned long *dst);
int mmc_part_write(struct mmc_card *card, u8 partno, unsigned long blknr, u32 blkcnt, unsigned long *src);
#endif
/* read entire idme */
__attribute__((weak)) int idme_platform_read(unsigned char *pbuf)
{
#ifndef UFBL_TESTS
	int ret = 0;
	struct mmc_card *card = mmc_get_card(0);

	ret = mmc_part_read(card,
			EXT_CSD_PART_CFG_BOOT_PART_2,
			CONFIG_USERDATA_IDME_ADDR,
			IDME_NUM_OF_EMMC_BLOCKS,
			(unsigned long *)pbuf);
#if defined(MODULE_DEBUG)
	hexdump8(pbuf, CONFIG_IDME_SIZE);
#endif
	return ret?-1:0;
#else
	return -1;
#endif
}

/* write idme */
__attribute__((weak)) int idme_platform_write(const unsigned char *pbuf)
{
#ifndef UFBL_TESTS
	int ret = 0;
	struct mmc_card *card = mmc_get_card(0);

	ret = mmc_part_write(card,
			EXT_CSD_PART_CFG_BOOT_PART_2,
			CONFIG_USERDATA_IDME_ADDR,
			IDME_NUM_OF_EMMC_BLOCKS,
			(unsigned long *) pbuf);
#if defined(MODULE_DEBUG)
	hexdump8(pbuf, CONFIG_IDME_SIZE);
#endif
	return ret?-1:0;
#else
	return -1;
#endif
}

