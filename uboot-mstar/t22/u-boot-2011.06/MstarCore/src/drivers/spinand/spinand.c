/* SPDX-License-Identifier: GPL-2.0-only OR BSD-3-Clause */
/******************************************************************************
 *
 * This file is provided under a dual license.  When you use or
 * distribute this software, you may choose to be licensed under
 * version 2 of the GNU General Public License ("GPLv2 License")
 * or BSD License.
 *
 * GPLv2 License
 *
 * Copyright(C) 2019 MediaTek Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See http://www.gnu.org/licenses/gpl-2.0.html for more details.
 *
 * BSD LICENSE
 *
 * Copyright(C) 2019 MediaTek Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *  * Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *****************************************************************************/
#include <common.h>
#include <malloc.h>
#include <linux/err.h>
#include <linux/mtd/compat.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>
#include <linux/mtd/nand_ecc.h>
#include <asm/io.h>
#include <asm/errno.h>
#include "MsCommon.h"
#include "spinand.h"

/* SPI NAND messages */
#if 0
#define spi_nand_msg(fmt, ...) printk(KERN_NOTICE "%s: " fmt "\n", __func__, ##__VA_ARGS__)
#else
#define spi_nand_msg(fmt, ...)
#endif
#define spi_nand_warn(fmt, ...) printk(KERN_WARNING "%s:warning, " fmt "\n", __func__, ##__VA_ARGS__)
#define spi_nand_err(fmt, ...) printk(KERN_ERR "%s:error, " fmt "\n", __func__, ##__VA_ARGS__)

static U32 u32_curRow = 0;

/* These really don't belong here, as they are specific to the NAND Model */
static uint8_t scan_ff_pattern[] = {0xff};

/* struct nand_bbt_descr - bad block table descriptor */
static struct nand_bbt_descr spi_nand_bbt_descr = {
	.options = NAND_BBT_2BIT | NAND_BBT_LASTBLOCK | NAND_BBT_VERSION | NAND_BBT_CREATE | NAND_BBT_WRITE,
	.offs = 0,
	.len = 1,
	.pattern = scan_ff_pattern
};

static struct nand_ecclayout spi_nand_oobinfo = {
	.eccbytes = 32,
	.eccpos = {8, 9, 10, 11, 12, 13, 14, 15,
				24, 25, 26, 27, 28, 29, 30, 31,
				40, 41, 42, 43, 44, 45, 46, 47,
				56, 57, 58, 59, 60, 61, 62, 63},
	.oobavail = 30,
	.oobfree = {
		{2, 6},
		{16, 8},
		{32, 8},
		{48, 8},
		{0, 0}
	},
};

static uint8_t bbt_pattern[] = {'B', 'b', 't', '0' };
static uint8_t mirror_pattern[] = {'1', 't', 'b', 'B' };

static struct nand_bbt_descr spi_nand_bbt_main_descr = {
	.options		= NAND_BBT_LASTBLOCK | NAND_BBT_CREATE | NAND_BBT_WRITE |
					  NAND_BBT_2BIT | NAND_BBT_VERSION | NAND_BBT_PERCHIP,
	.offs			= 1,
	.len			= 3,
	.veroffs		= 4,
	.maxblocks		= NAND_BBT_BLOCK_NUM,
	.pattern		= bbt_pattern
};

static struct nand_bbt_descr spi_nand_bbt_mirror_descr = {
	.options		= NAND_BBT_LASTBLOCK | NAND_BBT_CREATE | NAND_BBT_WRITE |
					  NAND_BBT_2BIT | NAND_BBT_VERSION | NAND_BBT_PERCHIP,
	.offs			= 1,
	.len			= 3,
	.veroffs		= 4,
	.maxblocks		= NAND_BBT_BLOCK_NUM,
	.pattern		= mirror_pattern
};

SPI_NAND_DRIVER_t gtSpiNandDrv;

uint8_t	spi_nand_read_byte(struct mtd_info *mtd)
{
	//spi_nand_msg("");

	return gtSpiNandDrv.pu8_sparebuf[gtSpiNandDrv.u32_column++];
}

u16 spi_nand_read_word(struct mtd_info *mtd)
{
	u16 u16_word;

	//spi_nand_msg("");
	u16_word = ((u16)gtSpiNandDrv.pu8_sparebuf[gtSpiNandDrv.u32_column] | ((u16)gtSpiNandDrv.pu8_sparebuf[gtSpiNandDrv.u32_column+1]<<8));
	gtSpiNandDrv.u32_column += 2;

	return u16_word;
}

void spi_nand_write_buf(struct mtd_info *mtd, const uint8_t *buf, int len)
{
	//spi_nand_msg("not support");
}

void spi_nand_read_buf(struct mtd_info *mtd, uint8_t *buf, int len)
{
	//spi_nand_msg("not support");
}

int spi_nand_verify_buf(struct mtd_info *mtd, const uint8_t *buf, int len)
{
	//spi_nand_msg("not support");

	return 0;	
}

void spi_nand_select_chip(struct mtd_info *mtd, int chip)
{
	//spi_nand_msg("");
}
 
void spi_nand_cmd_ctrl(struct mtd_info *mtd, int dat, unsigned int ctrl)
{
	//spi_nand_msg("not support");
}

int spi_nand_dev_ready(struct mtd_info *mtd)
{
	//spi_nand_msg("not support");

	return 1;
}

void spi_nand_cmdfunc(struct mtd_info *mtd, unsigned command, int column, int page_addr)
{
	U32 ret;

	switch (command) {
		case NAND_CMD_STATUS:
			//spi_nand_msg("%02Xh", command);
			gtSpiNandDrv.pu8_sparebuf[0] = NAND_STATUS_READY|NAND_STATUS_TRUE_READY;
			gtSpiNandDrv.u32_column = 0;
			break;

		case NAND_CMD_READOOB:
			//spi_nand_msg("%02Xh", command);
			MDrv_SPINAND_Read(page_addr, (U8 *)gtSpiNandDrv.pu8_pagebuf, (U8 *)gtSpiNandDrv.pu8_sparebuf);
			gtSpiNandDrv.u32_column = column;
			break;
			

		case NAND_CMD_ERASE2:
			//spi_nand_msg("%02Xh", command);
			break;

		case NAND_CMD_ERASE1:
			spi_nand_msg("NAND_CMD_ERASE1, page_addr: 0x%X\n", page_addr);
			gtSpiNandDrv.u8_status = NAND_STATUS_READY|NAND_STATUS_TRUE_READY;
			ret = MDrv_SPINAND_BLOCK_ERASE(page_addr);
			if(ret != ERR_SPINAND_SUCCESS)
			{
				spi_nand_err("MDrv_SPINAND_Erase=%ld", ret);
				gtSpiNandDrv.u8_status |= NAND_STATUS_FAIL;
			}

			break;

		case NAND_CMD_READ0:
			//spi_nand_msg("%02Xh", command);
			u32_curRow = page_addr;
			break;

		default:
			spi_nand_err("unsupported command %02Xh", command);
			break;
	}

	return;
}

int spi_nand_waitfunc(struct mtd_info *mtd, struct nand_chip *this)
{
	//spi_nand_msg("");

	return (int)gtSpiNandDrv.u8_status;
}

int spi_nand_write_page(struct mtd_info *mtd, struct nand_chip *chip, const uint8_t *buf, int page, int cached, int raw)
{
	U32 ret;

	spi_nand_msg("0x%X", page);

	ret = MDrv_SPINAND_Write(page, (U8 *)buf, (U8 *)chip->oob_poi);
	if(ret != ERR_SPINAND_SUCCESS)
	{
		spi_nand_err("MDrv_SPINAND_Write=%ld", ret);
		return -EIO;
	}

	return 0;
}

void spi_nand_ecc_hwctl(struct mtd_info *mtd, int mode)
{
	//spi_nand_msg("not support");
}

int spi_nand_ecc_calculate(struct mtd_info *mtd, const uint8_t *dat, uint8_t *ecc_code)
{
	//spi_nand_msg("not support");

	return 0;
}

int spi_nand_ecc_correct(struct mtd_info *mtd, uint8_t *dat, uint8_t *read_ecc, uint8_t *calc_ecc)
{
	//spi_nand_msg("not support");

	return 0;	
}

int spi_nand_ecc_read_page_raw(struct mtd_info *mtd, struct nand_chip *chip, uint8_t *buf, int page)
{
	U32 ret;

	spi_nand_msg("0x%X", page);

	ret = MDrv_SPINAND_Read(page, (U8 *)buf, (U8 *)chip->oob_poi);
	if(ret != ERR_SPINAND_SUCCESS && ret != ERR_SPINAND_ECC_BITFLIP)
	{
		spi_nand_err("MDrv_SPINAND_Read=%ld", ret);
	}
	if(ret == ERR_SPINAND_ECC_BITFLIP)
	{
		mtd->ecc_stats.corrected += 1;
	}

	return 0;
}

void spi_nand_ecc_write_page_raw(struct mtd_info *mtd, struct nand_chip *chip, const uint8_t *buf)
{
	//spi_nand_msg("not support");
}

int spi_nand_ecc_read_page(struct mtd_info *mtd, struct nand_chip *chip, uint8_t *buf, int page)
{
	U32 ret;

	spi_nand_msg("0x%X", page);

	ret = MDrv_SPINAND_Read(page, (U8 *)buf, (U8 *)chip->oob_poi);
	if(ret != ERR_SPINAND_SUCCESS && ret != ERR_SPINAND_ECC_BITFLIP)
	{
		spi_nand_err("MDrv_SPINAND_Read=%ld", ret);
		mtd->ecc_stats.failed++;
	}
	if(ret == ERR_SPINAND_ECC_BITFLIP)
	{
		mtd->ecc_stats.corrected += 1;
	}

	return 0;
}

int spi_nand_ecc_read_subpage(struct mtd_info *mtd, struct nand_chip *chip, uint32_t offs, uint32_t len, uint8_t *buf)
{
	U32 ret = 0;

	if(offs + len > gtSpiNandDrv.tSpinandInfo.u16_PageByteCnt){
		spi_nand_err("read len exceeds page len\n");
		return -EINVAL;
	}

	buf += offs;

	ret = MDrv_SPINAND_Read_RandomIn(u32_curRow, offs, len, buf);

	if(ret == ERR_SPINAND_SUCCESS)
	{
		//printk("read subpage %ld success\n", u32_curRow);
	}
	else if(ret == ERR_SPINAND_ECC_ERROR)
		mtd->ecc_stats.failed++;
	else if(ret == ERR_SPINAND_ECC_BITFLIP)
		mtd->ecc_stats.corrected += 1;
	else
		return -EIO;
	
	return  0;
}

void spi_nand_ecc_write_page(struct mtd_info *mtd, struct nand_chip *chip, const uint8_t *buf)
{
	//spi_nand_msg("not support");
}

int spi_nand_ecc_read_oob(struct mtd_info *mtd, struct nand_chip *chip, int page, int sndcmd)
{
	U32 ret;
	spi_nand_msg("0x%X", page);

	ret = MDrv_SPINAND_Read(page, (U8 *)gtSpiNandDrv.pu8_pagebuf, (U8 *)chip->oob_poi);
	if(ret != ERR_SPINAND_SUCCESS)
	{
		spi_nand_err("MDrv_SPINAND_Read=%ld", ret);
	}

	return 0;
}

int spi_nand_ecc_write_oob(struct mtd_info *mtd, struct nand_chip *chip, int page)
{
	U32 ret;

	spi_nand_msg("0x%X", page);

	memset((void *)gtSpiNandDrv.pu8_pagebuf, 0xFF, mtd->writesize);
	ret = MDrv_SPINAND_Write(page, (U8 *)gtSpiNandDrv.pu8_pagebuf, (U8 *)chip->oob_poi);
	if(ret != ERR_SPINAND_SUCCESS)
	{
		spi_nand_err("MDrv_SPINAND_Write=%ld", ret);
		return -EIO;
	}

	return 0;
}

static U32 CheckSum(U8 *pu8_Data, U16 u16_ByteCnt)
{
	U32 u32_Sum = 0;

	while (u16_ByteCnt--)
		u32_Sum += *pu8_Data++;

	return u32_Sum;
}

static U8 MDrv_SPINAND_CountBits(U32 u32_x)
{
	U8 u8_i = 0;

	while (u32_x) {
		u8_i++;
		u32_x >>= 1;
	}

	return u8_i-1;
}

/*
 * Board-specific NAND initialization.
 * - hwcontrol: hardwarespecific function for accesing control-lines
 * - dev_ready: hardwarespecific function for  accesing device ready/busy line
 * - eccmode: mode of ecc, see defines
 */

int board_nand_init(struct nand_chip *nand)
{
	U8 u8_i;
	U32 u32_ret;
	PARTITION_INFO_t *ptPartInfo;

	if(MDrv_SPINAND_Init(&(gtSpiNandDrv.tSpinandInfo)) != TRUE)
	{
		spi_nand_err("MDrv_SPINAND_Init fail");
		return -1;
	}

    gtSpiNandDrv.u8_SectorByteCntBits = MDrv_SPINAND_CountBits(gtSpiNandDrv.tSpinandInfo.u16_SectorByteCnt);
    gtSpiNandDrv.u16_PageSectorCnt = gtSpiNandDrv.tSpinandInfo.u16_PageByteCnt >> gtSpiNandDrv.u8_SectorByteCntBits;
    gtSpiNandDrv.u8_PageSectorCntBits = MDrv_SPINAND_CountBits(gtSpiNandDrv.u16_PageSectorCnt);
    gtSpiNandDrv.u8_PageByteCntBits = MDrv_SPINAND_CountBits(gtSpiNandDrv.tSpinandInfo.u16_PageByteCnt);
    gtSpiNandDrv.u8_BlkPageCntBits = MDrv_SPINAND_CountBits(gtSpiNandDrv.tSpinandInfo.u16_BlkPageCnt);

	gtSpiNandDrv.u8_status = NAND_STATUS_READY|NAND_STATUS_TRUE_READY;
	gtSpiNandDrv.u32_column = 0;
	gtSpiNandDrv.pu8_pagebuf = kmalloc(gtSpiNandDrv.tSpinandInfo.u16_PageByteCnt, GFP_KERNEL);
	gtSpiNandDrv.pu8_sparebuf = kmalloc(gtSpiNandDrv.tSpinandInfo.u16_SpareByteCnt, GFP_KERNEL);
	if(!gtSpiNandDrv.pu8_pagebuf || !gtSpiNandDrv.pu8_sparebuf)
	{
		spi_nand_err("Can not alloc memory for page/spare buffer");
		return -1;
	}

	ptPartInfo = (PARTITION_INFO_t *)gtSpiNandDrv.pu8_pagebuf;
	for(u8_i=0 ; u8_i<10 ; u8_i+=2)
	{
		u32_ret = MDrv_SPINAND_Read(u8_i*gtSpiNandDrv.tSpinandInfo.u16_BlkPageCnt, gtSpiNandDrv.pu8_pagebuf, gtSpiNandDrv.pu8_sparebuf);
		if(u32_ret == ERR_SPINAND_SUCCESS)
		{
			if(memcmp((const void *) gtSpiNandDrv.pu8_pagebuf, SPINAND_FLASH_INFO_TAG, 16) == 0)
			{
				u32_ret = MDrv_SPINAND_Read(u8_i*gtSpiNandDrv.tSpinandInfo.u16_BlkPageCnt+1, gtSpiNandDrv.pu8_pagebuf, gtSpiNandDrv.pu8_sparebuf);
				if(u32_ret == ERR_SPINAND_SUCCESS)
				{
					if(ptPartInfo->u32_ChkSum == CheckSum((u8*)&(ptPartInfo->u16_SpareByteCnt), 0x200 - 0x04))
						break;
				}
			}
		}
	}
	if(u8_i == 10)
	{
		printk(KERN_NOTICE"CIS doesn't contain part info\r\n");
		gtSpiNandDrv.u8_HasPNI = 0;
	}
	else
	{
		spi_nand_msg("CIS contains part info\r\n");
		gtSpiNandDrv.u8_HasPNI = 1;
		memcpy((void *)&gtSpiNandDrv.tPartInfo, (const void *) ptPartInfo, 0x200);
	}

	/* please refer to include/linux/nand.h for more info. */
	nand->read_byte = spi_nand_read_byte;
	nand->read_word = spi_nand_read_word;
	nand->write_buf = spi_nand_write_buf;
	nand->read_buf = spi_nand_read_buf;
	nand->verify_buf = spi_nand_verify_buf;
	nand->select_chip = spi_nand_select_chip;
	nand->cmd_ctrl = spi_nand_cmd_ctrl;
	nand->dev_ready = spi_nand_dev_ready;
	nand->cmdfunc = spi_nand_cmdfunc;
	nand->waitfunc = spi_nand_waitfunc;
	nand->write_page = spi_nand_write_page;

	nand->options = NAND_USE_FLASH_BBT;
	nand->chip_delay = 0;
	nand->badblock_pattern = &spi_nand_bbt_descr; //using default badblock pattern.
	nand->bbt_td = &spi_nand_bbt_main_descr;
	nand->bbt_md = &spi_nand_bbt_mirror_descr;

	nand->require_randomizer = 0;
	nand->ecc_corretable_bit = 4;
	nand->ecc_bitflip_threshold = 2;

	nand->ecc.mode = NAND_ECC_HW;
	nand->ecc.size = gtSpiNandDrv.tSpinandInfo.u16_PageByteCnt; 
	nand->ecc.bytes = (gtSpiNandDrv.tSpinandInfo.u16_SpareByteCnt>>1);
	nand->ecc.sectors = 4;
	nand->ecc.layout =  &spi_nand_oobinfo;

	nand->ecc.hwctl = spi_nand_ecc_hwctl;
	nand->ecc.calculate = spi_nand_ecc_calculate;
	nand->ecc.correct = spi_nand_ecc_correct;
	nand->ecc.read_page_raw = spi_nand_ecc_read_page_raw;
	nand->ecc.write_page_raw = spi_nand_ecc_write_page_raw;
	nand->ecc.read_page = spi_nand_ecc_read_page;
	nand->ecc.read_subpage = spi_nand_ecc_read_subpage;
	nand->ecc.write_page = spi_nand_ecc_write_page;
	nand->ecc.read_oob = spi_nand_ecc_read_oob;
	nand->ecc.write_oob = spi_nand_ecc_write_oob;

	return 0;
}

/* return 1: Good block, 0: Bad block */
U32 MDrv_SPINAND_IsGoodBlk(U16 u16_PBA)
{
	U32 u32_PageIdx;
	U32 u32_Err;

	u32_PageIdx = u16_PBA * gtSpiNandDrv.tSpinandInfo.u16_BlkPageCnt;
	u32_Err = MDrv_SPINAND_Read(u32_PageIdx, gtSpiNandDrv.pu8_pagebuf, gtSpiNandDrv.pu8_sparebuf);
	if(u32_Err != ERR_SPINAND_SUCCESS)
		spi_nand_err("MDrv_SPINAND_Read(0x%lX)=0x%lX\n", u32_PageIdx, u32_Err);
	
	if(gtSpiNandDrv.pu8_sparebuf[0] != 0xFF)
		return 0;

	return 1;
}

U32 MDrv_SPINAND_WriteCIS_for_ROM(SPINAND_FLASH_INFO_TAG_t *pSpiNandInfoTag)
{
	U32 u32_Err = ERR_SPINAND_SUCCESS;
	U16 u16_PBA;
	U32 u32_PageIdx;
	U8 u8_CISIdx;

	u8_CISIdx = 0;
	REG(RIU_BASE + (0x1980 << 2) + (0x41 << 2) ) = 0x0000;

	/* Search for two good blocks within the first 10 physical blocks */
	for (u16_PBA = 0; u16_PBA < 10; u16_PBA+=2)
	{
		/* Check first page of block */
		u32_PageIdx = u16_PBA * gtSpiNandDrv.tSpinandInfo.u16_BlkPageCnt;
		u32_Err = MDrv_SPINAND_Read(u32_PageIdx, gtSpiNandDrv.pu8_pagebuf, gtSpiNandDrv.pu8_sparebuf);
		if(u32_Err != ERR_SPINAND_SUCCESS)
			spi_nand_err("MDrv_SPINAND_Read(0x%lX)=0x%lX\n", u32_PageIdx, u32_Err);
		if(gtSpiNandDrv.pu8_sparebuf[0] != 0xFF)
		{
			spi_nand_err("Skip bad blk 0x%04x\n", u16_PBA);
			continue;
		}

		u32_Err = MDrv_SPINAND_BLOCK_ERASE(u32_PageIdx);
		if(u32_Err != ERR_SPINAND_SUCCESS)
		{
			spi_nand_err("MDrv_SPINAND_BLOCK_ERASE(0x%X)=0x%lX\n", u16_PBA, u32_Err);
			continue;
		}

		memset(gtSpiNandDrv.pu8_pagebuf, 0xFF, gtSpiNandDrv.tSpinandInfo.u16_PageByteCnt);
		memcpy(gtSpiNandDrv.pu8_pagebuf, pSpiNandInfoTag, 512);
		memset(gtSpiNandDrv.pu8_sparebuf, 0xFF, gtSpiNandDrv.tSpinandInfo.u16_SpareByteCnt);

		u32_Err = MDrv_SPINAND_Write(u32_PageIdx, gtSpiNandDrv.pu8_pagebuf, gtSpiNandDrv.pu8_sparebuf);
		if(u32_Err != ERR_SPINAND_SUCCESS)
		{
			spi_nand_err("MDrv_SPINAND_Write(0x%lX)=0x%lX\n", u32_PageIdx, u32_Err);
			continue;
		}
	
		/*
		**	Write Partition Info the 2nd page
		**/
		if(gtSpiNandDrv.u8_HasPNI == 1)
		{
			memset(gtSpiNandDrv.pu8_pagebuf, 0xFF, gtSpiNandDrv.tSpinandInfo.u16_PageByteCnt);
			memcpy(gtSpiNandDrv.pu8_pagebuf, &gtSpiNandDrv.tPartInfo, 512);
		
			u32_Err = MDrv_SPINAND_Write(u32_PageIdx+1, gtSpiNandDrv.pu8_pagebuf, gtSpiNandDrv.pu8_sparebuf);
			if(u32_Err != ERR_SPINAND_SUCCESS)
			{
				spi_nand_err("MDrv_SPINAND_Write(0x%lX)=0x%lX\n", u32_PageIdx+1, u32_Err);
				continue;
			}
		}

		printk(KERN_NOTICE"CIS%d is written to blk 0x%04x\n", u8_CISIdx, u16_PBA);

		if((++u8_CISIdx) == 2)
			break;
	}

	switch (u8_CISIdx) {
		case 0:
			REG(RIU_BASE + (0x1980 << 2) + (0x41 << 2) )  = 0x0002;
			u32_Err = ERR_SPINAND_INVALID;
			break;
		case 1:
			REG(RIU_BASE + (0x1980 << 2) + (0x41 << 2) )  = 0x0003;
			u32_Err = ERR_SPINAND_INVALID;
			break;
		case 2:
			u32_Err = ERR_SPINAND_SUCCESS;
			break;
	}

    return u32_Err;
}

int search_cis_in_DRAM(U8 *pu8_CISAddr, U8 *pu8_PPMAddr, U8 *pu8_PNIAddr, SPINAND_FLASH_INFO_TAG_t *pSpiNandInfoTagOut)
{
	SPINAND_FLASH_INFO_TAG_t *pSpiNandInfoTag;
	SPINAND_FLASH_INFO_t *ptSpinandInfo;
	PARTITION_INFO_t *ptPartInfo;
	U32 u32_i, u32_j;
	U8 u8_IsEnd = 0;

	REG(RIU_BASE + (0x1980 << 2) + (0x41 << 2) )  = 0x0000;

	printk(KERN_NOTICE"Begin Search CIS in DRAM\n");
	
	//Search 20 pieces of possiable cis memory
	u32_i = 0;
	while(1)
	{
		pSpiNandInfoTag = (SPINAND_FLASH_INFO_TAG_t *)(pu8_CISAddr + u32_i);
		ptSpinandInfo = &pSpiNandInfoTag->tSpiNandInfo;

		for(u32_j=0; u32_j<gtSpiNandDrv.tSpinandInfo.u8_IDByteCnt; u32_j++)
		{
			if(gtSpiNandDrv.tSpinandInfo.au8_ID[u32_j] != ptSpinandInfo->au8_ID[u32_j])
				break;
		}
		if(u32_j == gtSpiNandDrv.tSpinandInfo.u8_IDByteCnt)
			break;

		if(memcmp((const void *) pSpiNandInfoTag->au8_Tag, (const void *) SPINAND_FLASH_INFO_TAG, 16))
		{
			u8_IsEnd = 1;
			break;
		}
		u32_i += 0x200;	
	}
	if(u8_IsEnd == 1)
	{
		printk(KERN_NOTICE"No available CIS match with current spi nand flash\n");
		REG(RIU_BASE + (0x1980 << 2) + (0x41 << 2) )  = 0x0004;
		return -1;
	}
	printk(KERN_NOTICE"Found CIS in given memory\n");
	memcpy(&gtSpiNandDrv.tSpinandInfo, ptSpinandInfo, sizeof(SPINAND_FLASH_INFO_t));
    MDrv_SPINAND_ForceInit(ptSpinandInfo);
    
	//search pni
	gtSpiNandDrv.u8_HasPNI = 0;
	if(pu8_PNIAddr != NULL)
	{	
		for(u32_i = 0; u32_i < 0x200 * 20 ; u32_i += 0x200)
		{
			ptPartInfo = (PARTITION_INFO_t*) (pu8_PNIAddr + u32_i);

			if(ptPartInfo->u16_SpareByteCnt == gtSpiNandDrv.tSpinandInfo.u16_SpareByteCnt &&
				ptPartInfo->u16_PageByteCnt == gtSpiNandDrv.tSpinandInfo.u16_PageByteCnt &&
				ptPartInfo->u16_BlkPageCnt == gtSpiNandDrv.tSpinandInfo.u16_BlkPageCnt &&
				ptPartInfo->u16_BlkCnt == gtSpiNandDrv.tSpinandInfo.u16_BlkCnt) //Match Partition layout
			{
				memcpy(&gtSpiNandDrv.tPartInfo, ptPartInfo, 0x200);
				gtSpiNandDrv.u8_HasPNI = 1;
				break;
			}
		}
	}

	memcpy(pSpiNandInfoTagOut, pSpiNandInfoTag , 0x200);

	return 0;
}

int MDrv_SPINAND_write_bootloader(U32 u32_Row,U8 * pu8_addr, U32 u32_size, U8 u8_BootStageId)
{
	U8 *pu8_DataBuf = pu8_addr;
	U16 u16_BlkPageCnt;
	U32 u32_pagecnt, u32_Err, u32_size_tmp = u32_size, u32_Row_Read = u32_Row;
	U16 u16_i;

	u16_BlkPageCnt = gtSpiNandDrv.tSpinandInfo.u16_BlkPageCnt;	
	u32_pagecnt = (u32_size+gtSpiNandDrv.tSpinandInfo.u16_PageByteCnt-1)/gtSpiNandDrv.tSpinandInfo.u16_PageByteCnt;

	//add register status for polling by TV Tool
	REG(RIU_BASE + (0x1980 << 2) + (0x41 << 2) )  = 0x0000;

	while(u32_pagecnt >= u16_BlkPageCnt)
	{	
		while (MDrv_SPINAND_IsGoodBlk(u32_Row / gtSpiNandDrv.tSpinandInfo.u16_BlkPageCnt) == 0)
		{
			u32_Row += gtSpiNandDrv.tSpinandInfo.u16_BlkPageCnt;
			//bad block jump to next block
			if(u32_Row == (gtSpiNandDrv.tSpinandInfo.u16_BlkCnt*gtSpiNandDrv.tSpinandInfo.u16_BlkPageCnt))
			{
				printk(KERN_NOTICE"Error : There is no available GOOD block in current nand device\n");
				REG(RIU_BASE + (0x1980 << 2) + (0x41 << 2) )  = 0x0004;
				return -1;
			}
		}
		u32_Err = MDrv_SPINAND_BLOCK_ERASE(u32_Row);
		if(u32_Err != ERR_SPINAND_SUCCESS)
		{
			//jump to next block
			u32_Row += gtSpiNandDrv.tSpinandInfo.u16_BlkPageCnt;
			continue;
		}

		memset(gtSpiNandDrv.pu8_sparebuf, 0xFF, gtSpiNandDrv.tSpinandInfo.u16_SpareByteCnt);
		gtSpiNandDrv.pu8_sparebuf[4] = u8_BootStageId;

		for(u16_i = 0 ; u16_i<gtSpiNandDrv.tSpinandInfo.u16_BlkPageCnt ; u16_i++)
		{
			u32_Err = MDrv_SPINAND_Write(u32_Row+u16_i, pu8_DataBuf+(u16_i*gtSpiNandDrv.tSpinandInfo.u16_PageByteCnt), gtSpiNandDrv.pu8_sparebuf);
			if(u32_Err != ERR_SPINAND_SUCCESS)
			{
				//jump to next block
				u32_Row += gtSpiNandDrv.tSpinandInfo.u16_BlkPageCnt;
				continue;
			}
		}
		pu8_DataBuf += u16_BlkPageCnt*gtSpiNandDrv.tSpinandInfo.u16_PageByteCnt;
			
		u32_pagecnt -= u16_BlkPageCnt;
		u32_size -= u16_BlkPageCnt*gtSpiNandDrv.tSpinandInfo.u16_PageByteCnt;
		u32_Row += gtSpiNandDrv.tSpinandInfo.u16_BlkPageCnt;
	}

	while(u32_size)
	{
		while (MDrv_SPINAND_IsGoodBlk(u32_Row/gtSpiNandDrv.tSpinandInfo.u16_BlkPageCnt) == 0)
		{
			u32_Row += gtSpiNandDrv.tSpinandInfo.u16_BlkPageCnt;
			//bad block jump to next block
			if(u32_Row == (gtSpiNandDrv.tSpinandInfo.u16_BlkCnt*gtSpiNandDrv.tSpinandInfo.u16_BlkPageCnt))
			{
				printk(KERN_NOTICE"Error : There is no available GOOD block in current nand device\n");
				REG(RIU_BASE + (0x1980 << 2) + (0x41 << 2) )  = 0x0004;
				return -1;
			}
		}
		u32_Err = MDrv_SPINAND_BLOCK_ERASE(u32_Row);
		if(u32_Err != ERR_SPINAND_SUCCESS)
		{
			//jump to next block
			u32_Row += gtSpiNandDrv.tSpinandInfo.u16_BlkPageCnt;
			continue;
		}

		memset(gtSpiNandDrv.pu8_sparebuf, 0xFF, gtSpiNandDrv.tSpinandInfo.u16_SpareByteCnt);
		gtSpiNandDrv.pu8_sparebuf[4] = u8_BootStageId;

		for(u16_i = 0 ; u16_i<u32_pagecnt ; u16_i++)
		{
			u32_Err = MDrv_SPINAND_Write(u32_Row+u16_i, pu8_DataBuf+(u16_i*gtSpiNandDrv.tSpinandInfo.u16_PageByteCnt), gtSpiNandDrv.pu8_sparebuf);
			if(u32_Err != ERR_SPINAND_SUCCESS)
			{
				//jump to next block
				u32_Row += gtSpiNandDrv.tSpinandInfo.u16_BlkPageCnt;
				continue;
			}
		}
		
		u32_size-= u32_size;
	}
	//finish programming
	//Add register checking
	//read back check
	// TODO
	u32_size = u32_size_tmp;
	u32_pagecnt = (u32_size+gtSpiNandDrv.tSpinandInfo.u16_PageByteCnt-1)/gtSpiNandDrv.tSpinandInfo.u16_PageByteCnt;
	pu8_DataBuf = pu8_addr;
	u32_Row = u32_Row_Read;

	while(u32_pagecnt >= u16_BlkPageCnt)
	{	
		while (MDrv_SPINAND_IsGoodBlk(u32_Row / gtSpiNandDrv.tSpinandInfo.u16_BlkPageCnt) == 0)
		{
			u32_Row += gtSpiNandDrv.tSpinandInfo.u16_BlkPageCnt;
			//bad block jump to next block
			if(u32_Row == (gtSpiNandDrv.tSpinandInfo.u16_BlkCnt*gtSpiNandDrv.tSpinandInfo.u16_BlkPageCnt))
			{
				printk(KERN_NOTICE"Error : There is no available GOOD block in current nand device\n");
				REG(RIU_BASE + (0x1980 << 2) + (0x41 << 2) )  = 0x0004;
				return -1;
			}
		}

		for(u16_i =0; u16_i < u16_BlkPageCnt; u16_i ++)
		{
			u32_Err = MDrv_SPINAND_Read(u32_Row+u16_i, gtSpiNandDrv.pu8_pagebuf, gtSpiNandDrv.pu8_sparebuf);
			if(u32_Err != ERR_SPINAND_SUCCESS)
			{
				printk(KERN_NOTICE"Error: ECC fail when read back checking\n");
				REG(RIU_BASE + (0x1980 << 2) + (0x41 << 2) )  = 0x0002;
				return -1;
			}
			if(memcmp(gtSpiNandDrv.pu8_pagebuf, pu8_DataBuf, gtSpiNandDrv.tSpinandInfo.u16_PageByteCnt))
			{
				printk(KERN_NOTICE"Error: compare fail when read back checking\n");
				printf("RAM--->\n");
				dump_mem((unsigned char *) pu8_DataBuf, gtSpiNandDrv.tSpinandInfo.u16_PageByteCnt);
				printf("NAND--->\n");
				dump_mem((unsigned char *) gtSpiNandDrv.pu8_pagebuf, gtSpiNandDrv.tSpinandInfo.u16_PageByteCnt);		
				REG(RIU_BASE + (0x1980 << 2) + (0x41 << 2) )  = 0x0003;
				return -1;
			}
			pu8_DataBuf += gtSpiNandDrv.tSpinandInfo.u16_PageByteCnt;
		}

		
		u32_pagecnt -= u16_BlkPageCnt;
		u32_size -= (u16_BlkPageCnt*gtSpiNandDrv.tSpinandInfo.u16_PageByteCnt);
		u32_Row += gtSpiNandDrv.tSpinandInfo.u16_BlkPageCnt;
	}

	while(u32_size)
	{
		while (MDrv_SPINAND_IsGoodBlk(u32_Row / gtSpiNandDrv.tSpinandInfo.u16_BlkPageCnt) == 0)
		{
			u32_Row += gtSpiNandDrv.tSpinandInfo.u16_BlkPageCnt;
			//bad block jump to next block
			if(u32_Row == (gtSpiNandDrv.tSpinandInfo.u16_BlkCnt*gtSpiNandDrv.tSpinandInfo.u16_BlkPageCnt))
			{
				printk(KERN_NOTICE"Error : There is no available GOOD block in current nand device\n");
				REG(RIU_BASE + (0x1980 << 2) + (0x41 << 2) )  = 0x0004;
				return -1;
			}
		}

		for(u16_i=0; u16_i<u32_pagecnt; u16_i++)
		{
			u32_Err = MDrv_SPINAND_Read(u32_Row+u16_i, gtSpiNandDrv.pu8_pagebuf, gtSpiNandDrv.pu8_sparebuf);
			if(u32_Err != ERR_SPINAND_SUCCESS)
			{
				printk(KERN_NOTICE"Error: ECC fail when read back checking\n");
				REG(RIU_BASE + (0x1980 << 2) + (0x41 << 2) )  = 0x0002;
				return -1;
			}			

			if(memcmp(gtSpiNandDrv.pu8_pagebuf, pu8_DataBuf, gtSpiNandDrv.tSpinandInfo.u16_PageByteCnt))
			{
				printk(KERN_NOTICE"Error: compare fail when read back checking\n");
				printf("RAM--->\n");
				dump_mem((unsigned char *) pu8_DataBuf, gtSpiNandDrv.tSpinandInfo.u16_PageByteCnt);
				printf("NAND--->\n");
				dump_mem((unsigned char *) gtSpiNandDrv.pu8_pagebuf, gtSpiNandDrv.tSpinandInfo.u16_PageByteCnt);				
				REG(RIU_BASE + (0x1980 << 2) + (0x41 << 2) )  = 0x0003;
				return -1;
			}
			pu8_DataBuf += gtSpiNandDrv.tSpinandInfo.u16_PageByteCnt;		
		}

		u32_size -= u32_size;
	}
	//Register status Ok
	REG(RIU_BASE + (0x1980 << 2) + (0x41 << 2) )  = 0x0001;
	return 0;
}

static u32 empty_check(const void *buf, u32 len)
{
	int i;

	for (i = (len >> 2) - 1; i >= 0; i--)
		if (((const uint32_t *)buf)[i] != 0xFFFFFFFF)
			break;

	/* The resulting length must be aligned to the minimum flash I/O size */
	len = ALIGN((i + 1) << 2, gtSpiNandDrv.tSpinandInfo.u16_PageByteCnt + gtSpiNandDrv.tSpinandInfo.u16_SpareByteCnt);
	return len;
}

int MDrv_SPINAND_ReadCISBlk(U32 u32_off, U8* pu8_DataBuf)
{
	U32 u32_Err, i, pglen;
	#if 0
	#ifdef FULL_BLOCK_PROGRAM_SCRUB
	U32  chk_len;
	#endif
	#endif
	U32 u32_Row = u32_off/gtSpiNandDrv.tSpinandInfo.u16_PageByteCnt;
	U32 page_off = gtSpiNandDrv.tSpinandInfo.u16_PageByteCnt + gtSpiNandDrv.tSpinandInfo.u16_SpareByteCnt;
	
	pglen = gtSpiNandDrv.tSpinandInfo.u16_PageByteCnt + gtSpiNandDrv.tSpinandInfo.u16_SpareByteCnt;
	for(i=0 ; i<gtSpiNandDrv.tSpinandInfo.u16_BlkPageCnt; i++)
	{
		u32_Err = MDrv_SPINAND_Read(u32_Row+i, pu8_DataBuf+page_off*i, pu8_DataBuf+page_off*i+gtSpiNandDrv.tSpinandInfo.u16_PageByteCnt);
		if(u32_Err != ERR_SPINAND_SUCCESS)
			return u32_Err;
			
		/*
		 *For SLC, full block program is not necessary
		 *Turn on this switch if Spinand is MLC
		 */
		#if 0
		#ifdef FULL_BLOCK_PROGRAM_SCRUB
		chk_len = empty_check(pu8_DataBuf+page_off*i, pglen);
		if (!chk_len)
		{
		    (pu8_DataBuf + page_off * i)[0] = 0xfe; // indicate empty page
		}
		#endif
		#endif
	}

	return ERR_SPINAND_SUCCESS;
}

void MDrv_SPINAND_GetMtdParts(char *buf)
{
	PARTITION_INFO_t *pPartInfo = &gtSpiNandDrv.tPartInfo;
	U8 u8_i,u8_PartNo, u8_Unknown = 0;
	int len, Maxlen = 512;
	u32 PartSize = 0, PartSize1 = 0, PartSize2 = 0;
	U32 u32_Err;
	u32 virtual_boot;
	U32 u32_BlkIdx;
	U8	u8_MbootCnt= 0;
	U32 u32_MbootBlkIdx[2];
	char *mtd_buf=buf, tmp[128], PartName[16];
	U16 u16_LastPartType;

	if(gtSpiNandDrv.u8_HasPNI == 0)
		goto cleanup;

	sprintf(mtd_buf, "mtdparts=edb64M-nand:");
	//printf("%s\r\n",buf);
	mtd_buf += 21;
	if (Maxlen < 21)
		goto cleanup;
	Maxlen -= 21;
		
	u16_LastPartType = 0;
	for(u8_i = 0; u8_i < pPartInfo->u16_PartCnt; u8_i ++)
	{
		if( (pPartInfo->records[u8_i].u16_PartType & USFD_LOGI_PART) == USFD_LOGI_PART)
			break;
		
		PartSize = 0;
		if( u8_i )
		{
			memcpy(mtd_buf, ",", 1);
			mtd_buf ++;
			Maxlen --;
		}
		
		PartSize = gtSpiNandDrv.tSpinandInfo.u16_BlkPageCnt * gtSpiNandDrv.tSpinandInfo.u16_PageByteCnt *
			(pPartInfo->records[u8_i].u16_BackupBlkCnt + pPartInfo->records[u8_i].u16_BlkCnt);
		
		u8_PartNo = u16_LastPartType != pPartInfo->records[u8_i].u16_PartType ? 0:1;

		u16_LastPartType = pPartInfo->records[u8_i].u16_PartType;

		switch(pPartInfo->records[u8_i].u16_PartType)
		{
			case USFD_PART_BOOTLOGO:
				sprintf(PartName,"UBILD" );
				break;				
			case USFD_PART_UBOOT:
				sprintf(PartName,"MBOOT" );
				break;
			case USFD_PART_OS:
				sprintf(PartName,"KL" );
				break;
			case USFD_PART_SECINFO:
				sprintf(PartName,"KL_BP" );
				break;
			case USFD_PART_OTP:
				sprintf(PartName,"NTP" );
				break;
			case USFD_PART_RECOVERY:
				sprintf(PartName,"RECOVERY" );
				break;
			case USFD_PART_MISC:
				sprintf(PartName,"MISC" );
				break;	
			case USFD_PART_TBL:
				sprintf(PartName,"TBL" );
				break;	
			case USFD_PART_CTRL:
				sprintf(PartName,"CTRL" );
				break;	
			case USFD_PART_UBIRO:
				sprintf(PartName,"UBIRO" );
				break;	
			default:
				sprintf(PartName,"UNKNOWN%d", u8_Unknown++);
				break;	
		}
		if (u8_i)
			sprintf(tmp, "%dk(%s)", PartSize/1024, PartName); //,pPartInfo->records[u8_i].u16_PartType);
		else 
		{
			u8_MbootCnt = 0;
			if(gtSpiNandDrv.tSpinandInfo.u8_BL1PBA != 0)
			{
				//search vid chunk header for mboot partition beginning
				u32_BlkIdx = 1;
				while(1)
				{
					u32_Err = MDrv_SPINAND_Read((gtSpiNandDrv.tSpinandInfo.u8_BL1PBA + u32_BlkIdx)*gtSpiNandDrv.tSpinandInfo.u16_BlkPageCnt,
												gtSpiNandDrv.pu8_pagebuf, gtSpiNandDrv.pu8_sparebuf);
					//binary ID for chunk header
					if( ((U32 *)gtSpiNandDrv.pu8_pagebuf)[0x7] == 0x0000B007 && u32_Err == ERR_SPINAND_SUCCESS)
					{
						u32_MbootBlkIdx[u8_MbootCnt] = (gtSpiNandDrv.tSpinandInfo.u8_BL1PBA + u32_BlkIdx);
						u8_MbootCnt ++;
					}
					#if (ENABLE_MODULE_DISABLE_MBOOTBAK == 1)
					if(u8_MbootCnt == 1)
						break;
					#else					
					if(u8_MbootCnt == 2)
						break;
					#endif

					if((gtSpiNandDrv.tSpinandInfo.u8_BL1PBA + u32_BlkIdx) == 0xFF)
						break;

					u32_BlkIdx ++;
				}
				if((gtSpiNandDrv.tSpinandInfo.u8_BL1PBA + u32_BlkIdx) == 0xFF)
				{
					goto cleanup;
				}
				else
				{
					if(u8_MbootCnt == 1)
					{
						virtual_boot = u32_MbootBlkIdx[0] * (gtSpiNandDrv.tSpinandInfo.u16_BlkPageCnt * gtSpiNandDrv.tSpinandInfo.u16_PageByteCnt) / 1024;
						PartSize = PartSize - (u32_MbootBlkIdx[0] - 10) * (gtSpiNandDrv.tSpinandInfo.u16_BlkPageCnt * gtSpiNandDrv.tSpinandInfo.u16_PageByteCnt) ;
						sprintf(tmp, "%dk@%dk(%s)", PartSize/1024, virtual_boot, PartName);
					}
					else if(u8_MbootCnt == 2)
					{
						virtual_boot = u32_MbootBlkIdx[0] * (gtSpiNandDrv.tSpinandInfo.u16_BlkPageCnt * gtSpiNandDrv.tSpinandInfo.u16_PageByteCnt) / 1024;
						PartSize1 = (u32_MbootBlkIdx[1] - u32_MbootBlkIdx[0]) * (gtSpiNandDrv.tSpinandInfo.u16_BlkPageCnt * gtSpiNandDrv.tSpinandInfo.u16_PageByteCnt) ;
						PartSize2 = PartSize - (u32_MbootBlkIdx[1] - 10) * (gtSpiNandDrv.tSpinandInfo.u16_BlkPageCnt * gtSpiNandDrv.tSpinandInfo.u16_PageByteCnt) ;
						sprintf(tmp, "%dk@%dk(%s),%dk(%sBAK)", PartSize1/1024, virtual_boot, PartName,PartSize2/1024, PartName);
					}
					else
						goto cleanup;

					//virtual_boot = (gtSpiNandDrv.tSpinandInfo.u8_BL1PBA + u32_BlkIdx) * (gtSpiNandDrv.tSpinandInfo.u16_BlkPageCnt * gtSpiNandDrv.tSpinandInfo.u16_PageByteCnt) / 1024;
					//PartSize = PartSize - ((gtSpiNandDrv.tSpinandInfo.u8_BL1PBA + u32_BlkIdx - 10) * (gtSpiNandDrv.tSpinandInfo.u16_BlkPageCnt *gtSpiNandDrv.tSpinandInfo.u16_PageByteCnt));
				}
			}
			else if(gtSpiNandDrv.tSpinandInfo.u8_HashPBA[1][1] != 0)
			{
				u32_BlkIdx = 1;
				while(1)
				{
					u32_Err = MDrv_SPINAND_Read((gtSpiNandDrv.tSpinandInfo.u8_HashPBA[1][1] + u32_BlkIdx)*gtSpiNandDrv.tSpinandInfo.u16_BlkPageCnt,
												gtSpiNandDrv.pu8_pagebuf, gtSpiNandDrv.pu8_sparebuf);					
					//binary ID for chunk header
					if( ((U32 *)gtSpiNandDrv.pu8_pagebuf)[0x7] == 0x0000B007 && u32_Err == ERR_SPINAND_SUCCESS)
					{
						u32_MbootBlkIdx[u8_MbootCnt] = (gtSpiNandDrv.tSpinandInfo.u8_HashPBA[1][1] + u32_BlkIdx);
						u8_MbootCnt ++;
					}
					if(u8_MbootCnt == 2)
						break;
					if((gtSpiNandDrv.tSpinandInfo.u8_HashPBA[1][1] + u32_BlkIdx) == 0xFF)
						break;

					u32_BlkIdx ++;
				}
				if((gtSpiNandDrv.tSpinandInfo.u8_HashPBA[1][1] + u32_BlkIdx) == 0xFF)
				{
					goto cleanup;
				}
				else
				{
					if(u8_MbootCnt == 1)
					{
						virtual_boot = u32_MbootBlkIdx[0] * (gtSpiNandDrv.tSpinandInfo.u16_BlkPageCnt * gtSpiNandDrv.tSpinandInfo.u16_PageByteCnt) / 1024;
						PartSize = PartSize - (u32_MbootBlkIdx[0] - 10) * (gtSpiNandDrv.tSpinandInfo.u16_BlkPageCnt * gtSpiNandDrv.tSpinandInfo.u16_PageByteCnt) ;
						sprintf(tmp, "%dk@%dk(%s)", PartSize/1024, virtual_boot, PartName);
					}
					else if(u8_MbootCnt == 2)
					{
						virtual_boot = u32_MbootBlkIdx[0] * (gtSpiNandDrv.tSpinandInfo.u16_BlkPageCnt * gtSpiNandDrv.tSpinandInfo.u16_PageByteCnt) / 1024;
						//PartSize = PartSize - (u32_MbootBlkIdx[0] - 10) * (gtSpiNandDrv.tSpinandInfo.u16_BlkPageCnt * gtSpiNandDrv.tSpinandInfo.u16_PageByteCnt) ;
						PartSize1 = (u32_MbootBlkIdx[1] - u32_MbootBlkIdx[0]) * (gtSpiNandDrv.tSpinandInfo.u16_BlkPageCnt * gtSpiNandDrv.tSpinandInfo.u16_PageByteCnt) ;
						PartSize2 = PartSize - (u32_MbootBlkIdx[1] - 10) * (gtSpiNandDrv.tSpinandInfo.u16_BlkPageCnt * gtSpiNandDrv.tSpinandInfo.u16_PageByteCnt) ;
						sprintf(tmp, "%dk@%dk(%s),%dk(%sBAK)", PartSize1/1024, virtual_boot, PartName,PartSize2/1024, PartName);
					}
					else
						goto cleanup;
					//virtual_boot = (gtSpiNandDrv.tSpinandInfo.u8_HashPBA[1][1] + u32_BlkIdx) * (gtSpiNandDrv.tSpinandInfo.u16_BlkPageCnt * gtSpiNandDrv.tSpinandInfo.u16_PageByteCnt) / 1024;
					//PartSize = PartSize - (gtSpiNandDrv.tSpinandInfo.u8_HashPBA[1][1] + u32_BlkIdx - 10) * (gtSpiNandDrv.tSpinandInfo.u16_BlkPageCnt * gtSpiNandDrv.tSpinandInfo.u16_PageByteCnt) ;
				}
			}
			else
				goto cleanup;

		}
		len = strlen(tmp);
		memcpy(mtd_buf, tmp, len);
		mtd_buf += len;
		if (Maxlen < len)
			goto cleanup;
		Maxlen -= len;
		
	}
	if( (pPartInfo->records[u8_i].u16_PartType & USFD_LOGI_PART) == USFD_LOGI_PART)
	{
		sprintf(tmp,",-(UBI)");
		len= strlen(tmp);
		memcpy(mtd_buf, tmp, len);
		mtd_buf += len;
		if (Maxlen < len)
			goto cleanup;
		Maxlen -= len;
	}
	*mtd_buf = '\0';
	return ;
	cleanup:
	buf[0] = '\0';
	return ;
}

static U8 MDrv_SPINAND_CheckAll0xFF(U8* pu8_Buf, U32 u32_ByteCnt)
{
	register U32 u32_i;

	for(u32_i=0; u32_i<u32_ByteCnt; u32_i++)
		if(0xFF != pu8_Buf[u32_i])
			return 0;

	return 0xFF;
}

U32 MDrv_SPINAND_RefreshCIS(void)
{
	SPINAND_FLASH_INFO_t *ptSpinandInfo;
	SPINAND_FLASH_INFO_TAG_t *pSpiNandInfoTag;
	PARTITION_INFO_t *pPartInfo = &gtSpiNandDrv.tPartInfo;
	U32 u32_BlkIdx, u32_MBootBegin = 0, u32_MBootEnd = 0;
	U32 u32_Err;
	U8 	u8_i;
	int bl_count = 0;

	pSpiNandInfoTag = (SPINAND_FLASH_INFO_TAG_t *)malloc(512);
	if(!pSpiNandInfoTag)
	{
		printk(KERN_NOTICE"Memory Allocate fail for pSpiNandInfoTag\n");
		return -1;
	}

	memset(pSpiNandInfoTag, 0, 512);
	memcpy(pSpiNandInfoTag->au8_Tag, SPINAND_FLASH_INFO_TAG, 16);
	ptSpinandInfo = &pSpiNandInfoTag->tSpiNandInfo;
	memcpy(ptSpinandInfo, &gtSpiNandDrv.tSpinandInfo, sizeof(SPINAND_FLASH_INFO_t));

	//search MBOOT partition in partinfo

	if(gtSpiNandDrv.u8_HasPNI == 1)
	{
		for(u8_i=0 ; u8_i<pPartInfo->u16_PartCnt ; u8_i++)
		{
			if(pPartInfo->records[u8_i].u16_PartType == USFD_PART_UBOOT)
			{
				u32_MBootBegin = pPartInfo->records[u8_i].u16_StartBlk;
				u32_MBootEnd = pPartInfo->records[u8_i].u16_StartBlk 
							+ pPartInfo->records[u8_i].u16_BlkCnt
							+ pPartInfo->records[u8_i].u16_BackupBlkCnt;
				break;
			}
		}
		if(u8_i == pPartInfo->u16_PartCnt)
		{
			printk(KERN_NOTICE"ERROR: Partition info does not contain MBOOT partition\n");
			return -1;
		}

		//search sboot uboot/ HashX location for update nni infomation
		
		if(gtSpiNandDrv.tSpinandInfo.u8_BL0PBA != 0)	//for bl uboot
		{
			bl_count = 0;
			//search bl location in MBOOT PARTITION
			for(u32_BlkIdx=u32_MBootBegin ; u32_BlkIdx<u32_MBootEnd ; u32_BlkIdx++)
			{
				u32_Err = MDrv_SPINAND_Read(u32_BlkIdx*gtSpiNandDrv.tSpinandInfo.u16_BlkPageCnt,
											gtSpiNandDrv.pu8_pagebuf, gtSpiNandDrv.pu8_sparebuf);
				if(u32_Err!=ERR_SPINAND_SUCCESS || gtSpiNandDrv.pu8_sparebuf[0]!=0xFF)
					continue;

				if(!MDrv_SPINAND_CheckAll0xFF(gtSpiNandDrv.pu8_pagebuf, gtSpiNandDrv.tSpinandInfo.u16_PageByteCnt))
				{
					if(bl_count == 0)
					{
						ptSpinandInfo->u8_BL0PBA = gtSpiNandDrv.tSpinandInfo.u8_BL0PBA = (U8)u32_BlkIdx;
						bl_count ++;
						
						#if (ENABLE_MODULE_DISABLE_MBOOTBAK == 1)
						ptSpinandInfo->u8_BL1PBA = gtSpiNandDrv.tSpinandInfo.u8_BL1PBA = (U8)u32_BlkIdx;
						bl_count ++;
						break;
						#endif						
					}
					else if(bl_count == 1)
					{
						ptSpinandInfo->u8_BL1PBA = gtSpiNandDrv.tSpinandInfo.u8_BL1PBA = (U8)u32_BlkIdx;
						bl_count ++;
						break;
					}
					bl_count ++;
				}
			}

			printk(KERN_NOTICE"BL0_PBA %X, BL1_PBA %X\n", gtSpiNandDrv.tSpinandInfo.u8_BL0PBA, gtSpiNandDrv.tSpinandInfo.u8_BL1PBA);
			if(bl_count != 2)
			{
				printk(KERN_NOTICE"WARNING: there is no two sboots in SPI NAND Flash, Please Reupgrade Sboot\n");
				return -1;
			}

			if(gtSpiNandDrv.tSpinandInfo.u8_UBOOTPBA != 0)
			{
				bl_count = 0;
				for(/*u32_BlkIdx = u32_MBootBegin*/; u32_BlkIdx < u32_MBootEnd; u32_BlkIdx ++)
				{
					u32_Err = MDrv_SPINAND_Read(u32_BlkIdx*gtSpiNandDrv.tSpinandInfo.u16_BlkPageCnt,
												gtSpiNandDrv.pu8_pagebuf, gtSpiNandDrv.pu8_sparebuf);
					if(u32_Err!=ERR_SPINAND_SUCCESS || gtSpiNandDrv.pu8_sparebuf[0]!=0xFF)
						continue;

					if(((U32 *)gtSpiNandDrv.pu8_pagebuf)[0x7] == 0x0000B007)
					{
						if(bl_count == 1)
						{
							ptSpinandInfo->u8_UBOOTPBA = gtSpiNandDrv.tSpinandInfo.u8_UBOOTPBA = (U8)u32_BlkIdx;
							bl_count ++;
							break;
						}
						bl_count ++;
					}
				}
				if(bl_count != 2)
				{
					printk(KERN_NOTICE"WARNING: there is no two Mboots in SPI NAND Flash, Please Reupgrade Mboot\n");
					return -1;
				}

				printk(KERN_NOTICE"UBOOTPBA %X\n", gtSpiNandDrv.tSpinandInfo.u8_UBOOTPBA);
			}
		}
		else if(gtSpiNandDrv.tSpinandInfo.u8_HashPBA[0][0] != 0)	//for hash
		{
			bl_count = 0;
			//search bl location in MBOOT PARTITION
			for(u32_BlkIdx = u32_MBootBegin; u32_BlkIdx < u32_MBootEnd; u32_BlkIdx ++)
			{
				u32_Err = MDrv_SPINAND_Read(u32_BlkIdx*gtSpiNandDrv.tSpinandInfo.u16_BlkPageCnt,
											gtSpiNandDrv.pu8_pagebuf, gtSpiNandDrv.pu8_sparebuf);
				if(u32_Err!=ERR_SPINAND_SUCCESS || gtSpiNandDrv.pu8_sparebuf[0]!=0xFF)
					continue;
				if(!MDrv_SPINAND_CheckAll0xFF(gtSpiNandDrv.pu8_pagebuf, gtSpiNandDrv.tSpinandInfo.u16_PageByteCnt))
				{
					ptSpinandInfo->u8_HashPBA[bl_count>>1][bl_count&1] = gtSpiNandDrv.tSpinandInfo.u8_HashPBA[bl_count>>1][bl_count&1] = (U8)u32_BlkIdx;
					if(++bl_count == 4)
						break;
				}
			}

			printk(KERN_NOTICE"HASH00_PBA %X, HASH01_PBA %X\n", gtSpiNandDrv.tSpinandInfo.u8_HashPBA[0][0], gtSpiNandDrv.tSpinandInfo.u8_HashPBA[0][1]);
			printk(KERN_NOTICE"HASH10_PBA %X, HASH11_PBA %X\n", gtSpiNandDrv.tSpinandInfo.u8_HashPBA[1][0], gtSpiNandDrv.tSpinandInfo.u8_HashPBA[1][1]);
			if(bl_count != 4)
			{
				printk(KERN_NOTICE"WARNING: there is no two sboots in SPI NAND Flash, Please Reupgrade Sboot\n");
				return -1;
			}

			bl_count = 0;
			for(u32_BlkIdx = gtSpiNandDrv.tSpinandInfo.u8_HashPBA[1][1]+1; u32_BlkIdx < u32_MBootEnd; u32_BlkIdx ++)
			{
				u32_Err = MDrv_SPINAND_Read(u32_BlkIdx*gtSpiNandDrv.tSpinandInfo.u16_BlkPageCnt,
											gtSpiNandDrv.pu8_pagebuf, gtSpiNandDrv.pu8_sparebuf);
				if(u32_Err!=ERR_SPINAND_SUCCESS || gtSpiNandDrv.pu8_sparebuf[0]!=0xFF)
					continue;

				if(((U32 *)gtSpiNandDrv.pu8_pagebuf)[0x7] == 0x0000B007)
				{
					ptSpinandInfo->u8_HashPBA[2][bl_count] = gtSpiNandDrv.tSpinandInfo.u8_HashPBA[2][bl_count] = (U8)u32_BlkIdx;
					if(++bl_count == 2)
						break;
				}
			}
			if(bl_count != 2)
			{
				printk(KERN_NOTICE"WARNING: there is no two Mboots in SPI NAND Flash, Please Reupgrade Mboot\n");
				return -1;
			}

			printk(KERN_NOTICE"HASH20_PBA %X, HASH21_PBA %X\n", gtSpiNandDrv.tSpinandInfo.u8_HashPBA[2][0], gtSpiNandDrv.tSpinandInfo.u8_HashPBA[2][1]);		
		}
	}

	u32_Err = MDrv_SPINAND_WriteCIS_for_ROM(pSpiNandInfoTag);

	free(pSpiNandInfoTag);

	return u32_Err;
}

static __inline void dump_mem_line(unsigned char *buf, int cnt)
{
	int i;

	printk(KERN_NOTICE" 0x%08lx: ", (U32)buf);
	for (i= 0; i < cnt; i++)
		printk(KERN_NOTICE"%02X ", buf[i]);

	printk(KERN_NOTICE" | ");

	for (i = 0; i < cnt; i++)
		printk(KERN_NOTICE"%c", (buf[i] >= 32 && buf[i] < 128) ? buf[i] : '.');

	printk(KERN_NOTICE"\n");
}

void dump_mem(unsigned char *buf, int cnt)
{
	int i;

	for (i= 0; i < cnt; i+= 16)
		dump_mem_line(buf + i, 16);
}

void HWTimer_Start(void)
{
    // reset HW timer
    (*(volatile U16*)(TIMER1_MAX_LOW)) = 0xFFFF;
    (*(volatile U16*)(TIMER1_MAX_HIGH)) = 0xFFFF;
    (*(volatile U16*)(TIMER1_ENABLE)) = 0;

    // start HW timer
    (*(volatile U16*)(TIMER1_ENABLE)) |= 0x0001;
}

U32 HWTimer_End(void)
{
    U32 u32HWTimer = 0;
    U16 u16TimerLow = 0;
    U16 u16TimerHigh = 0;

    // Get HW timer
    u16TimerLow = (*(volatile U16*)(TIMER1_CAP_LOW));
    u16TimerHigh = (*(volatile U16*)(TIMER1_CAP_HIGH));	

    u32HWTimer = (u16TimerHigh<<16) | u16TimerLow;

    return u32HWTimer;
}

U32 MDrv_SPINAND_WriteBlkByteOffset(U8 *pu8_DestAddr, U16 u16_PBA, U32 u32_StartByte, U32 u32_ByteCnt)
{
#if 1
    return ERR_SPINAND_W_FAIL;
#else
	NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
	U8 *au8_PageBuf = (U8*)pNandDrv->PlatCtx_t.pu8_PageDataBuf;
	U8 *au8_SpareBuf = (U8*)pNandDrv->PlatCtx_t.pu8_PageSpareBuf;
	U32 u32_Err;
	//MBBS_INFO_t* SPAREINFO =(MBBS_INFO_t*) pNandDrv->PlatCtx_t.pu8_PageSpareBuf;
	U32 u32_SrcPageIdx, u32_BakPageIdx;
	U16 u16_PageCnt;
	U16 u16_ByteCntRead;
	U16 u16_ByteIdxInPage;
	U16 u16_i;
	U16 u16_LastWrittenPage = 0;
	U16 u16_BakBlkIdx;
	U8 u8_GoodBlkCnt;
	U16 u16_BlkPageCnt;
	U16 *pu16_BlkPBA = (U16 *)(pNandDrv->PlatCtx_t.pu8_PageSpareBuf + pNandDrv->u16_SectorSpareByteCnt);

	#if defined(FCIE_LFSR) && FCIE_LFSR
	if(pNandDrv->u8_RequireRandomizer)
	{
		NC_DisableLFSR();
	}
	#endif

	u16_BlkPageCnt = pNandDrv->u16_BlkPageCnt;
	if(pNandDrv->u8_CellType == 1)	// MLC
		u16_BlkPageCnt = u16_BlkPageCnt>>1;	

	/* sector size from AP is 512 Bytes, not NAND sector size */
	nand_debug(UNFD_DEBUG_LEVEL_HIGH, 1, "BlkIdx:%d, BlkNo:%d, StartByte:0x%X, ByteCnt:0x%X\n", (int)u16_PBA, (int)u16_LBA, (int)u32_StartByte, (int)u32_ByteCnt);

	/* Find physical Backup BlkIdx in CIS remain block */
	u16_BakBlkIdx = 9;
	u8_GoodBlkCnt = 0;
	while(1)
	{
		if(!drvNAND_IsGoodBlk(u16_BakBlkIdx))
		{
			nand_debug(UNFD_DEBUG_LEVEL_WARNING, 1, "Skip bad blk: 0x%04x\n", u16_BakBlkIdx);
		}
		else
		{
			if(++u8_GoodBlkCnt == 1)
				break;
		}

		if((--u16_BakBlkIdx) < 5)
		{
			nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "too many bad block\n");
			return UNFD_ST_ERR_LACK_BLK;
		}
	}
	nand_debug(UNFD_DEBUG_LEVEL_HIGH, 1, "u16_BakBlkIdx:%d\n", u16_BakBlkIdx);

	//nand_debug(UNFD_DEBUG_LEVEL_WARNING, 1, "erase u16_BakBlkIdx:%d\n", u16_BakBlkIdx);

	// erase bak block
	u32_Err = drvNAND_ErasePhyBlk(u16_BakBlkIdx);
	if (u32_Err != UNFD_ST_SUCCESS) {
		nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Erase Blk 0x%04x failed with EC: 0x%08lx\n",
				   u16_BakBlkIdx, u32_Err);
		drvNAND_MarkBadBlk(u16_BakBlkIdx);
		return u32_Err;
	}

	//copy data from src block to backup block before the startbyte
	for(u16_i=0 ; u16_i<(u32_StartByte>>pNandDrv->u8_PageByteCntBits) ; u16_i++)
	{
		u32_SrcPageIdx = (u16_PBA << pNandDrv->u8_BlkPageCntBits) + ga_tPairedPageMap[u16_i].u16_LSB;
		
		//nand_debug(UNFD_DEBUG_LEVEL_WARNING, 1, "copy data before start byte read  page :0x%X\n", u32_SrcPageIdx);
		u32_Err = NC_ReadPages(u32_SrcPageIdx, au8_PageBuf, au8_SpareBuf, 1);
		if (u32_Err != UNFD_ST_SUCCESS)
		{
			nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Reading page 0x%04lx failed with EC: 0x%04lx\n",
					   u32_SrcPageIdx, u32_Err);

			return u32_Err;
		}
//		SPAREINFO->u8_BadBlkMarker = 0xFF;
//		SPAREINFO->u16_BackupPBA = u16_PBA;
//		au8_SpareBuf[3] = 0x36;
//		au8_SpareBuf[4] = 0x97;
		
		au8_SpareBuf[0] = 0xFF;
		*pu16_BlkPBA = u16_PBA;
		au8_SpareBuf[pNandDrv->u16_SectorSpareByteCnt * 2] = 0x36;
		au8_SpareBuf[pNandDrv->u16_SectorSpareByteCnt * 2 + 1] = 0x97;
		au8_SpareBuf[pNandDrv->u16_SectorSpareByteCnt * 3] = 0xA1;
		au8_SpareBuf[pNandDrv->u16_SectorSpareByteCnt * 3 + 1] = 1;		
		u32_BakPageIdx = (u16_BakBlkIdx << pNandDrv->u8_BlkPageCntBits) + ga_tPairedPageMap[u16_i].u16_LSB;
		//nand_debug(UNFD_DEBUG_LEVEL_WARNING, 1, "write  page :0x%X\n", u32_BakPageIdx);
		
		u32_Err = NC_WritePages(u32_BakPageIdx, au8_PageBuf, au8_SpareBuf, 1);
		if (u32_Err != UNFD_ST_SUCCESS)
		{
			nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Writing page 0x%04lx failed with EC: 0x%04lx\n",
					   u32_BakPageIdx, u32_Err);
			drvNAND_MarkBadBlk(u16_BakBlkIdx);
			return u32_Err;
		}
		memset(au8_PageBuf, 0xFF, pNandDrv->u16_PageByteCnt);
		
		u32_Err = drvNAND_WriteDummyToMSBPage(u16_i, (u16_BakBlkIdx << pNandDrv->u8_BlkPageCntBits), au8_PageBuf, au8_SpareBuf, NULL);
		if(u32_Err != UNFD_ST_SUCCESS)
		{
			nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Writing page 0x%04lx failed with EC: 0x%04lx\n",
					   u32_BakPageIdx, u32_Err);
			return u32_Err;		
		}
		u16_LastWrittenPage = u16_i;
	}

	/* Read unaligned Byte first */
	if( u32_StartByte & pNandDrv->u16_PageByteCntMask)
	{
		u32_SrcPageIdx = (u16_PBA << pNandDrv->u8_BlkPageCntBits) + ga_tPairedPageMap[(u32_StartByte>>pNandDrv->u8_PageByteCntBits)].u16_LSB;

		//nand_debug(UNFD_DEBUG_LEVEL_WARNING, 1, "unaligned byte first read  page :0x%X\n", u32_SrcPageIdx);		
		u32_Err = NC_ReadPages(u32_SrcPageIdx, au8_PageBuf, au8_SpareBuf, 1);
		if (u32_Err != UNFD_ST_SUCCESS) {

			nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Reading page 0x%04lx failed with EC: 0x%04lx\n",
					   u32_SrcPageIdx, u32_Err);

			return u32_Err;
		}
		u16_ByteIdxInPage = u32_StartByte & pNandDrv->u16_PageByteCntMask;
		u16_ByteCntRead = (u32_ByteCnt > ((U32)pNandDrv->u16_PageByteCnt-(U32)u16_ByteIdxInPage)) ? ((U32)pNandDrv->u16_PageByteCnt-(U32)u16_ByteIdxInPage) : (u32_ByteCnt);

		memcpy((au8_PageBuf+u16_ByteIdxInPage), pu8_DestAddr, u16_ByteCntRead);
		u32_BakPageIdx = (u16_BakBlkIdx << pNandDrv->u8_BlkPageCntBits) + ga_tPairedPageMap[(u32_StartByte>>pNandDrv->u8_PageByteCntBits)].u16_LSB;
		
//		SPAREINFO->u8_BadBlkMarker = 0xFF;
//		SPAREINFO->u16_BackupPBA = u16_PBA;
//		au8_SpareBuf[3] = 0x36;
//		au8_SpareBuf[4] = 0x97;

		au8_SpareBuf[0] = 0xFF;
		*pu16_BlkPBA = u16_PBA;
		au8_SpareBuf[pNandDrv->u16_SectorSpareByteCnt * 2] = 0x36;
		au8_SpareBuf[pNandDrv->u16_SectorSpareByteCnt * 2 + 1] = 0x97;
		au8_SpareBuf[pNandDrv->u16_SectorSpareByteCnt * 3] = 0xA1;
		au8_SpareBuf[pNandDrv->u16_SectorSpareByteCnt * 3 + 1] = 1;			
		//nand_debug(UNFD_DEBUG_LEVEL_WARNING, 1, "write  page :0x%X\n", u32_BakPageIdx);
		u32_Err = NC_WritePages(u32_BakPageIdx, au8_PageBuf, au8_SpareBuf, 1);
		if (u32_Err != UNFD_ST_SUCCESS) {

			nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Writing page 0x%04lx failed with EC: 0x%04lx\n",
					   u32_BakPageIdx, u32_Err);
			drvNAND_MarkBadBlk(u16_BakBlkIdx);
			return u32_Err;
		}

		memset(au8_PageBuf, 0xFF, pNandDrv->u16_PageByteCnt);
		u32_Err = drvNAND_WriteDummyToMSBPage((u32_StartByte>>pNandDrv->u8_PageByteCntBits), (u16_BakBlkIdx << pNandDrv->u8_BlkPageCntBits), au8_PageBuf, au8_SpareBuf, NULL);
		if(u32_Err != UNFD_ST_SUCCESS)
		{
			nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Writing page 0x%04lx failed with EC: 0x%04lx\n",
					   u32_BakPageIdx, u32_Err);
			return u32_Err;		
		}

		pu8_DestAddr += u16_ByteCntRead;
		u32_StartByte += u16_ByteCntRead;
		u32_ByteCnt -= u16_ByteCntRead;
		u16_LastWrittenPage = (u32_StartByte>>pNandDrv->u8_PageByteCntBits);
	}

	/* Read aligned sectors then */
//	SPAREINFO->u8_BadBlkMarker = 0xFF;
//	SPAREINFO->u16_BackupPBA = u16_PBA;
//	au8_SpareBuf[3] = 0x36;
//	au8_SpareBuf[4] = 0x97;

	au8_SpareBuf[0] = 0xFF;
	*pu16_BlkPBA = u16_PBA;
	au8_SpareBuf[pNandDrv->u16_SectorSpareByteCnt * 2] = 0x36;
	au8_SpareBuf[pNandDrv->u16_SectorSpareByteCnt * 2 + 1] = 0x97;	
	au8_SpareBuf[pNandDrv->u16_SectorSpareByteCnt * 3] = 0xA1;
	au8_SpareBuf[pNandDrv->u16_SectorSpareByteCnt * 3 + 1] = 1; 

	u32_BakPageIdx = (u16_BakBlkIdx << pNandDrv->u8_BlkPageCntBits) + ga_tPairedPageMap[(u32_StartByte>>pNandDrv->u8_PageByteCntBits)].u16_LSB;
	u16_PageCnt = (u32_ByteCnt>>pNandDrv->u8_PageByteCntBits);
	while (u16_PageCnt)
	{
	
		//nand_debug(UNFD_DEBUG_LEVEL_WARNING, 1, "aligned write page :0x%X\n", u32_BakPageIdx);
		u32_Err = NC_WritePages(u32_BakPageIdx, pu8_DestAddr, au8_SpareBuf, 1);
		if (u32_Err != UNFD_ST_SUCCESS) {

			nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Writing page 0x%04lx failed with EC: 0x%04lx\n",
					   u32_BakPageIdx, u32_Err);
			drvNAND_MarkBadBlk(u16_BakBlkIdx);
			return u32_Err;
		}

		memset(au8_PageBuf, 0xFF, pNandDrv->u16_PageByteCnt);
		u32_Err = drvNAND_WriteDummyToMSBPage((u32_StartByte>>pNandDrv->u8_PageByteCntBits), (u16_BakBlkIdx << pNandDrv->u8_BlkPageCntBits), au8_PageBuf, au8_SpareBuf, NULL);
		if(u32_Err != UNFD_ST_SUCCESS)
		{
			nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Writing page 0x%04lx failed with EC: 0x%04lx\n",
					   u32_BakPageIdx, u32_Err);
			return u32_Err;		
		}

		u16_LastWrittenPage = u32_BakPageIdx -(u16_BakBlkIdx << pNandDrv->u8_BlkPageCntBits);
		pu8_DestAddr += pNandDrv->u16_PageByteCnt;
		u16_PageCnt--;
		u32_StartByte += pNandDrv->u16_PageByteCnt;
		u32_BakPageIdx = (u16_BakBlkIdx << pNandDrv->u8_BlkPageCntBits) + ga_tPairedPageMap[(u32_StartByte>>pNandDrv->u8_PageByteCntBits)].u16_LSB;		
		u32_ByteCnt -= pNandDrv->u16_PageByteCnt;
	}

	/* Read remaining unaligned sectors finally */
	if( u32_ByteCnt)
	{
		u32_SrcPageIdx = (u16_PBA << pNandDrv->u8_BlkPageCntBits) + ga_tPairedPageMap[(u32_StartByte>>pNandDrv->u8_PageByteCntBits)].u16_LSB;
		//nand_debug(UNFD_DEBUG_LEVEL_WARNING, 1, "remaining unaligned read  page :0x%X\n", u32_SrcPageIdx);
		
		u32_Err = NC_ReadPages(u32_SrcPageIdx, au8_PageBuf, au8_SpareBuf, 1);
		if (u32_Err != UNFD_ST_SUCCESS) {

			nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Reading page 0x%04lx failed with EC: 0x%04lx\n",
					   u32_SrcPageIdx, u32_Err);

			return u32_Err;
		}

//		SPAREINFO->u8_BadBlkMarker = 0xFF;
//		SPAREINFO->u16_BackupPBA = u16_PBA;
//		au8_SpareBuf[3] = 0x36;
//		au8_SpareBuf[4] = 0x97;

		au8_SpareBuf[0] = 0xFF;
		*pu16_BlkPBA = u16_PBA;
		au8_SpareBuf[pNandDrv->u16_SectorSpareByteCnt * 2] = 0x36;
		au8_SpareBuf[pNandDrv->u16_SectorSpareByteCnt * 2 + 1] = 0x97;
		au8_SpareBuf[pNandDrv->u16_SectorSpareByteCnt * 3] = 0xA1;
		au8_SpareBuf[pNandDrv->u16_SectorSpareByteCnt * 3 + 1] = 1;	

		memcpy(au8_PageBuf, pu8_DestAddr, u32_ByteCnt);
		u32_BakPageIdx = (u16_BakBlkIdx << pNandDrv->u8_BlkPageCntBits) + ga_tPairedPageMap[(u32_StartByte>>pNandDrv->u8_PageByteCntBits)].u16_LSB;		
		//nand_debug(UNFD_DEBUG_LEVEL_WARNING, 1, "write  page :0x%X\n", u32_BakPageIdx);
		
		u32_Err = NC_WritePages(u32_BakPageIdx, au8_PageBuf, au8_SpareBuf, 1);
		if (u32_Err != UNFD_ST_SUCCESS) {

			nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Writing page 0x%04lx failed with EC: 0x%04lx\n",
					   u32_BakPageIdx, u32_Err);
			drvNAND_MarkBadBlk(u16_BakBlkIdx);
			return u32_Err;
		}
		
		memset(au8_PageBuf, 0xFF, pNandDrv->u16_PageByteCnt);
		u32_Err = drvNAND_WriteDummyToMSBPage((u32_StartByte>>pNandDrv->u8_PageByteCntBits), (u16_BakBlkIdx << pNandDrv->u8_BlkPageCntBits), au8_PageBuf, au8_SpareBuf, NULL);
		if(u32_Err != UNFD_ST_SUCCESS)
		{
			nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Writing page 0x%04lx failed with EC: 0x%04lx\n",
					   u32_BakPageIdx, u32_Err);
			return u32_Err;		
		}
		
		u16_LastWrittenPage = (u32_StartByte>>pNandDrv->u8_PageByteCntBits);
	}

	//copy remain data from PBA to Backup block
	for(u16_i=(u16_LastWrittenPage+1) ; u16_i<u16_BlkPageCnt; u16_i++)
	{
		u32_SrcPageIdx = (u16_PBA << pNandDrv->u8_BlkPageCntBits) + ga_tPairedPageMap[u16_i].u16_LSB;
		//nand_debug(UNFD_DEBUG_LEVEL_WARNING, 1, "remain data read page :0x%X\n", u32_SrcPageIdx);
		
		u32_Err = NC_ReadPages(u32_SrcPageIdx, au8_PageBuf, au8_SpareBuf, 1);
		if (u32_Err != UNFD_ST_SUCCESS)
		{
			nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Reading page 0x%04lx failed with EC: 0x%04lx\n",
					   u32_SrcPageIdx, u32_Err);

			return u32_Err;
		}
//		SPAREINFO->u8_BadBlkMarker = 0xFF;
//		SPAREINFO->u16_BackupPBA = u16_PBA;
//		au8_SpareBuf[3] = 0x36;
//		au8_SpareBuf[4] = 0x97;
		
		au8_SpareBuf[0] = 0xFF;
		*pu16_BlkPBA = u16_PBA;
		au8_SpareBuf[pNandDrv->u16_SectorSpareByteCnt * 2] = 0x36;
		au8_SpareBuf[pNandDrv->u16_SectorSpareByteCnt * 2 + 1] = 0x97;	
		au8_SpareBuf[pNandDrv->u16_SectorSpareByteCnt * 3] = 0xA1;
		au8_SpareBuf[pNandDrv->u16_SectorSpareByteCnt * 3 + 1] = 1;	

		u32_BakPageIdx = (u16_BakBlkIdx << pNandDrv->u8_BlkPageCntBits) + ga_tPairedPageMap[u16_i].u16_LSB;
		
		//nand_debug(UNFD_DEBUG_LEVEL_WARNING, 1, "write  page :0x%X\n", u32_BakPageIdx);
		u32_Err = NC_WritePages(u32_BakPageIdx, au8_PageBuf, au8_SpareBuf, 1);
		if (u32_Err != UNFD_ST_SUCCESS)
		{
			nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Writing page 0x%04lx failed with EC: 0x%04lx\n",
					   u32_BakPageIdx, u32_Err);
			drvNAND_MarkBadBlk(u16_BakBlkIdx);
			return u32_Err;
		}
		
		memset(au8_PageBuf, 0xFF, pNandDrv->u16_PageByteCnt);
		u32_Err = drvNAND_WriteDummyToMSBPage(u16_i, (u16_BakBlkIdx << pNandDrv->u8_BlkPageCntBits), au8_PageBuf, au8_SpareBuf, NULL);
		if(u32_Err != UNFD_ST_SUCCESS)
		{
			nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Writing page 0x%04lx failed with EC: 0x%04lx\n",
					   u32_BakPageIdx, u32_Err);
			return u32_Err;		
		}
		
	}
	
	// erase src block
	
	//nand_debug(UNFD_DEBUG_LEVEL_WARNING, 1, "erase src block :0x%X\n", u16_PBA);
	u32_Err = drvNAND_ErasePhyBlk(u16_PBA);
	if (u32_Err != UNFD_ST_SUCCESS) {
        nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Erase Blk 0x%04x failed with EC: 0x%08x\n",
            (unsigned int)u16_PBA, (unsigned int)u32_Err);
		drvNAND_MarkBadBlk(u16_PBA);
		return u32_Err;
	}

	// copy bak to src
	for(u16_i=0 ; u16_i<u16_BlkPageCnt; u16_i++)
	{
		u32_BakPageIdx = (u16_BakBlkIdx << pNandDrv->u8_BlkPageCntBits) + ga_tPairedPageMap[u16_i].u16_LSB;
		//nand_debug(UNFD_DEBUG_LEVEL_WARNING, 1, "read page :0x%X\n", u32_BakPageIdx);
		
		u32_Err = NC_ReadPages(u32_BakPageIdx, au8_PageBuf, au8_SpareBuf, 1);
		if (u32_Err != UNFD_ST_SUCCESS)
		{
			nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Reading page 0x%04lx failed with EC: 0x%04lx\n",
					   u32_BakPageIdx, u32_Err);

			return u32_Err;
		}

		u32_SrcPageIdx = (u16_PBA << pNandDrv->u8_BlkPageCntBits) + ga_tPairedPageMap[u16_i].u16_LSB;
		//nand_debug(UNFD_DEBUG_LEVEL_WARNING, 1, "write  page :0x%X\n", u32_SrcPageIdx);
		
		memset(au8_SpareBuf, 0xFF, pNandDrv->u16_SpareByteCnt);
		u32_Err = NC_WritePages(u32_SrcPageIdx, au8_PageBuf, au8_SpareBuf, 1);
		if (u32_Err != UNFD_ST_SUCCESS)
		{
			nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Reading page 0x%04lx failed with EC: 0x%04lx\n",
					   u32_SrcPageIdx, u32_Err);
			drvNAND_MarkBadBlk(u16_PBA);
			return u32_Err;
		}

		memset(au8_PageBuf, 0xFF, pNandDrv->u16_PageByteCnt);
		u32_Err = drvNAND_WriteDummyToMSBPage(u16_i, (u16_PBA << pNandDrv->u8_BlkPageCntBits), au8_PageBuf, au8_SpareBuf, NULL);
		if(u32_Err != UNFD_ST_SUCCESS)
		{
			nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Writing page 0x%04lx failed with EC: 0x%04lx\n",
					   u32_BakPageIdx, u32_Err);
			return u32_Err;		
		}		
	}

	// erase bak block
	u32_Err = drvNAND_ErasePhyBlk(u16_BakBlkIdx);
	if (u32_Err != UNFD_ST_SUCCESS) {
		nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Erase Blk 0x%04x failed with EC: 0x%08lx\n",
				   u16_BakBlkIdx, u32_Err);
		drvNAND_MarkBadBlk(u16_BakBlkIdx);
		return u32_Err;
	}

	return UNFD_ST_SUCCESS;
#endif    
}

U32 MDrv_SPINAND_ReadBlkByteOffset(U8 *pu8_DestAddr, U16 u16_PBA, U32 u32_StartByte, U32 u32_ByteCnt)
{
	U8 *au8_PageBuf = (U8*)gtSpiNandDrv.pu8_pagebuf;
	U8 *au8_SpareBuf = (U8*)gtSpiNandDrv.pu8_sparebuf;
	U32 u32_Err;
	U32 u32_PageIdx;
	U16 u16_PageCnt;
	U16 u16_ByteCntRead;
	U16 u16_ByteInPage;

	/* Read unaligned sectors first */
	if( u32_StartByte & (gtSpiNandDrv.tSpinandInfo.u16_PageByteCnt-1))
	{
		u16_ByteInPage = u32_StartByte & (gtSpiNandDrv.tSpinandInfo.u16_PageByteCnt-1);
		u16_ByteCntRead = u32_ByteCnt > ((U32)gtSpiNandDrv.tSpinandInfo.u16_PageByteCnt-(U32)u16_ByteInPage) ? ((U32)gtSpiNandDrv.tSpinandInfo.u16_PageByteCnt-(U32)u16_ByteInPage) : (u32_ByteCnt);
		u32_PageIdx = (u16_PBA << gtSpiNandDrv.u8_BlkPageCntBits) + (u32_StartByte >> gtSpiNandDrv.u8_PageByteCntBits);
		
        u32_Err = MDrv_SPINAND_Read(u32_PageIdx, au8_PageBuf, au8_SpareBuf);
		if (u32_Err != ERR_SPINAND_SUCCESS) {

            spi_nand_err("Reading page 0x%04x failed with EC: 0x%04x\n",
                    (unsigned int)u32_PageIdx, (unsigned int)u32_Err);
			return u32_Err;
		}

		memcpy(pu8_DestAddr, au8_PageBuf + u16_ByteInPage, u16_ByteCntRead);
		pu8_DestAddr += u16_ByteCntRead;
		u32_StartByte += u16_ByteCntRead;
		u32_ByteCnt -= u16_ByteCntRead;
	}

	/* Read aligned sectors then */
	u32_PageIdx = (u16_PBA << gtSpiNandDrv.u8_BlkPageCntBits) + (u32_StartByte >> gtSpiNandDrv.u8_PageByteCntBits);
	u16_PageCnt = (u32_ByteCnt >> gtSpiNandDrv.u8_PageByteCntBits);

	while (u16_PageCnt)
	{
        u32_Err = MDrv_SPINAND_Read(u32_PageIdx, pu8_DestAddr, au8_SpareBuf);
		if (u32_Err != ERR_SPINAND_SUCCESS) {

            spi_nand_err("Reading page 0x%04x failed with EC: 0x%04x\n",
            (unsigned int)u32_PageIdx, (unsigned int)u32_Err);
			return u32_Err;
		}

		pu8_DestAddr += gtSpiNandDrv.tSpinandInfo.u16_PageByteCnt;
		u16_PageCnt--;
		u32_StartByte += gtSpiNandDrv.tSpinandInfo.u16_PageByteCnt;
		u32_ByteCnt -= gtSpiNandDrv.tSpinandInfo.u16_PageByteCnt;
		u32_PageIdx = (u16_PBA << gtSpiNandDrv.u8_BlkPageCntBits) + (u32_StartByte >> gtSpiNandDrv.u8_PageByteCntBits);
		
	}

	/* Read remaining unaligned sectors finally */
	if( u32_ByteCnt )
	{
		u32_PageIdx = (u16_PBA << gtSpiNandDrv.u8_BlkPageCntBits) +  (u32_StartByte >> gtSpiNandDrv.u8_PageByteCntBits);
        u32_Err = MDrv_SPINAND_Read(u32_PageIdx, au8_PageBuf, au8_SpareBuf);
		if (u32_Err != ERR_SPINAND_SUCCESS) {

            spi_nand_err("Reading page 0x%04x failed with EC: 0x%04x\n",
	                    (unsigned int)u32_PageIdx, (unsigned int)u32_Err); 
			return u32_Err;
		}
		memcpy(pu8_DestAddr, au8_PageBuf, u32_ByteCnt);
	}
	return ERR_SPINAND_SUCCESS;
}

