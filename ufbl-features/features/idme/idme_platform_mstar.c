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
#include "eMMC.h"
#include "part_emmc.h"
#include <malloc.h>
/* module debug */
//#define MODULE_DEBUG

#define CFG_FASTBOOT_MMC_NO 0

/* read entire idme */
int idme_platform_read(unsigned char *pbuf)
{
	U8 *readBuf = NULL;
	U32 u32_err = eMMC_ST_SUCCESS;

	readBuf = (U8*)memalign(eMMC_CACHE_LINE, CONFIG_IDME_SIZE);
	u32_err = eMMC_Init();
	if(eMMC_ST_SUCCESS != u32_err)
	{
		eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: init fail: %Xh \n", u32_err);
		goto exit_idme_platform_read;
	}
        /* read from boot partition*/
	u32_err = eMMC_ReadBootPart(readBuf, CONFIG_IDME_SIZE, CONFIG_USERDATA_IDME_ADDR/CONFIG_MMC_BLOCK_SIZE, 2);
	if(eMMC_ST_SUCCESS != u32_err)
        {
		eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: read BootPart 2 fail: %Xh \n", u32_err);
		goto exit_idme_platform_read;
        }
        memcpy(pbuf,readBuf,CONFIG_IDME_SIZE);
#if defined(MODULE_DEBUG)
	hexdump8(pbuf, CONFIG_IDME_SIZE);
#endif
	exit_idme_platform_read:
		if(readBuf){
			free(readBuf);
			readBuf = NULL;
		}
	return 0;
}

/* write idme */
int idme_platform_write(const unsigned char *pbuf)
{
	U8 *writeBuf = NULL;
	U32 u32_err = eMMC_ST_SUCCESS;

	writeBuf = (U8*)memalign(eMMC_CACHE_LINE, CONFIG_IDME_SIZE);
	u32_err = eMMC_Init();
	if(eMMC_ST_SUCCESS != u32_err)
	{
		eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: init fail: %Xh \n", u32_err);
		goto exit_idme_platform_write;
	}
	/* write to boot partition */
	memcpy(writeBuf,pbuf,CONFIG_IDME_SIZE);
	u32_err = eMMC_WriteBootPart(writeBuf, CONFIG_IDME_SIZE, CONFIG_USERDATA_IDME_ADDR/CONFIG_MMC_BLOCK_SIZE, 2);
	if(eMMC_ST_SUCCESS != u32_err)
	{
		eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: read BootPart 2 fail: %Xh \n", u32_err);
		goto exit_idme_platform_write;
	}

#if defined(MODULE_DEBUG)
	hexdump8(pbuf, CONFIG_IDME_SIZE);
#endif
	exit_idme_platform_write:
		if(writeBuf){
			free(writeBuf);
			writeBuf = NULL;
		}
	return 0;
}

