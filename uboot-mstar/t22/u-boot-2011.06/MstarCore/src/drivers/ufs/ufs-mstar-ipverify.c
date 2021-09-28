/*
 * Copyright (c) 2013-2016, Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#include "ufshcd.h"
#include "ufs-mstar.h"
#include "../disk/part_ufs.h"
#include <malloc.h>

#if defined(CONFIG_MSTAR_UFS_IPVERIFY) && (CONFIG_MSTAR_UFS_IPVERIFY)
#define UFS_TEST_SUITE_INFO			1
#define UFS_TEST_SUITE_RW6_LUN		1
#define UFS_TEST_SUITE_RW10_LUN		1
#define UFS_TEST_SUITE_SPEED		1
#define UFS_TEST_SUITE_PWR_CUT		1
#define UFS_TEST_SUITE_HIBERN8		1

#define TEST_LUN					0

#define UFS_SPEED_TEST_SINGLE_BLKCNT_W		64
#define UFS_SPEED_TEST_MULTIPLE_BLKCNT_W	((u32)30*(1024*1024/4096)) // total 30MB
#define UFS_SPEED_TEST_R_MULTI				10
#define UFS_SPEED_TEST_SINGLE_BLKCNT_R		((u32)UFS_SPEED_TEST_SINGLE_BLKCNT_W*UFS_SPEED_TEST_R_MULTI)
#define UFS_SPEED_TEST_MULTIPLE_BLKCNT_R	((u32)UFS_SPEED_TEST_MULTIPLE_BLKCNT_W*UFS_SPEED_TEST_R_MULTI)
#define UFS_SECTOR_4KBYTE					4096
#define UFS_SECTOR_4KBYTE_BITS				12

#define UFS_PWRCUT_DATA0				0x55
#define UFS_PWRCUT_DATA1				0xAA
#define UFS_PWRCUT_DATA_CNT				2
#define UFS_PWRCUT_TEST_UNIT_CNT		16
#define UFS_PWRCUT_TEST_UNIT_BYTECNT	0x1000000 // 16MB
#define UFS_PWRCUT_TEST_SPACE_BYTECNT	(UFS_PWRCUT_TEST_UNIT_CNT*UFS_PWRCUT_TEST_UNIT_BYTECNT) // 16x16 = 256MB

typedef struct _UFS_PWRCUT_CTRLBLK {

	u32 u32_CheckSum;
	u8  u8_TargetUnit;
	u8  au8_TaegetData[UFS_PWRCUT_TEST_UNIT_CNT];

} UFS_PWRCUT_CTRLBLK_t;

#define TEST_BUFFER_SIZE				0x100000 // 1MB

static u8 *gau8_WBuf = 0;
static u8 *gau8_RBuf = 0;

int ufstest_info(void)
{
	int err;
	u8 desc_buf[QUERY_DESC_CONFIGURAION_MAX_SIZE];
	struct ufs_hba *hba;
	u32 bBootLunEn = 0;
	int i;

	hba = ufshcd_get_host();
	if(!hba)
	{
		dev_err(hba->dev, "ufshcd_get_host fail\n");
		return ENXIO;
	}
	
	err = ufshcd_query_attr(hba, UPIU_QUERY_OPCODE_READ_ATTR, 0, 0, 0, &bBootLunEn);
	if (err)
	{
		dev_err(hba->dev, "Failed reading bBootLunEn err = %d", err);
		return err;
	}
	pr_dbg("bBootLunEn : %d\n", bBootLunEn);

	err = ufshcd_read_device_desc(hba, desc_buf, QUERY_DESC_DEVICE_MAX_SIZE);
	if (err) 
	{
		dev_err(hba->dev, "Failed reading Device Desc. err = %d\n", err);
		return err;
	}
	pr_dbg("dump device desc : \n");
	print_hex_dump(KERN_ERR, "", DUMP_PREFIX_OFFSET, 16, 4, desc_buf, 
		QUERY_DESC_DEVICE_MAX_SIZE, false);

	for(i=0 ; i<MAX_UFS_MSTAR_LUN ; i++)
	{
		err = ufshcd_read_unit_desc_param(hba, i, 0, desc_buf, QUERY_DESC_UNIT_MAX_SIZE);
		if (err) 
		{
			dev_err(hba->dev, "Failed reading Device Desc. err = %d\n", err);
			return err;
		}
		pr_dbg("dump unit desc %d : \n", i);
		print_hex_dump(KERN_ERR, "", DUMP_PREFIX_OFFSET, 16, 4, desc_buf, 
			QUERY_DESC_UNIT_MAX_SIZE, false);
	}

	err = ufshcd_read_configuration_desc(hba, 0, desc_buf, QUERY_DESC_CONFIGURAION_MAX_SIZE);
	if (err) 
	{
		dev_err(hba->dev, "Failed reading Config Desc. err = %d\n", err);
		return err;
	}
	pr_dbg("dump config desc %d : \n", i);
	print_hex_dump(KERN_ERR, "", DUMP_PREFIX_OFFSET, 16, 4, desc_buf, 
		QUERY_DESC_CONFIGURAION_MAX_SIZE, false);

	return err;
}

static int ufstest_prepare_pattern(void * pWriteBuf, u32 u32Size)
{
	struct ufs_hba *hba;
	struct ufs_mstar_host *host;
	int i, j, n;
	u32 u32_pattern_size;
	u32 u32_total_blk;
	u16 *au16_WriteBuf = (u16 *)pWriteBuf;

	hba = ufshcd_get_host();
	if(!hba)
	{
		dev_err(hba->dev, "ufshcd_get_host fail\n");
		return ENXIO;
	}
	host = (struct ufs_mstar_host *)ufshcd_get_variant(hba);

	pr_dbg("Prepare test pattern : %Xh bytes\n", u32Size);
	u32_total_blk = u32Size/host->dLogicalBlkSize[TEST_LUN];
    u32_pattern_size = host->dLogicalBlkSize[TEST_LUN]/8/2;
	for(n=0 ; n<u32_total_blk ; n++)
	{
		au16_WriteBuf = (u16 *)(DMA_W_ADDR + n*host->dLogicalBlkSize[TEST_LUN]);
		for(i=0 ; i<8 ; i++)
		{
			switch(i)
			{
				case 0:
					for(j=0 ; j<u32_pattern_size ; j++)
						au16_WriteBuf[i*u32_pattern_size + j] = 0x0000;
					break;
				case 1:
					for(j=0 ; j<u32_pattern_size ; j++)
						au16_WriteBuf[i*u32_pattern_size + j] = 0xFFFF;
					break;
				case 2:
					for(j=0 ; j<u32_pattern_size ; j++)
						au16_WriteBuf[i*u32_pattern_size + j] = 0x0FF0;
					break;
				case 3:
					for(j=0 ; j<u32_pattern_size ; j++)
	                    au16_WriteBuf[i*u32_pattern_size + j] = 0xFF00;
	                break;
				case 4:
					for(j=0 ; j<u32_pattern_size ; j++)
						au16_WriteBuf[i*u32_pattern_size + j] = 0x55AA;
					break;
				case 5:
					for(j=0 ; j<u32_pattern_size ; j++)
						au16_WriteBuf[i*u32_pattern_size + j] = 0x5A5A;
					break;
				case 6:
					for(j=0 ; j<u32_pattern_size*2 ; j++)
						((u8 *)au16_WriteBuf)[i*u32_pattern_size*2 + j] = j;
					break;
				case 7:
					for(j=0 ; j<u32_pattern_size*2 ; j++)
						((u8 *)au16_WriteBuf)[i*u32_pattern_size*2 + j] = 0xFF-j;
					break;
				default:
					break;
			}
		}

		sprintf((char *)au16_WriteBuf, "BLK%08X", n);
		#if 0
		if(n == 0)
		{
			pr_err("dump write buffer : \n");
			print_hex_dump(KERN_ERR, "", DUMP_PREFIX_ADDRESS, 16, 4, au16_WriteBuf, 
						host->dLogicalBlkSize[TEST_LUN], false);
		}
		#endif
	}

	return 0;
}

int ufstest_rw10_lun(int loop)
{
	int err = 0;
	struct ufs_hba *hba;
	struct ufs_mstar_host *host;
	int n, iteration;
	u32 u32_total_blk;
	u16 *au16_WriteBuf = (u16 *)DMA_W_ADDR;
	u16 *au16_ReadBuf = (u16 *)DMA_R_ADDR;
	u32 dLba;

	hba = ufshcd_get_host();
	if(!hba)
	{
		dev_err(hba->dev, "ufshcd_get_host fail\n");
		return ENXIO;
	}
	host = (struct ufs_mstar_host *)ufshcd_get_variant(hba);

	ufstest_prepare_pattern(DMA_W_ADDR, UFS_IPVERIFY_PATITION_SIZE);
	u32_total_blk = UFS_IPVERIFY_PATITION_SIZE/host->dLogicalBlkSize[TEST_LUN];
	dLba = UFS_IPVERIFY_PATITION_START/host->dLogicalBlkSize[TEST_LUN];

	for(iteration=0 ; (loop==0)||(iteration<loop) ; iteration++)
	{
		pr_dbg("[ufs_test_rw10_lun] looping %d, iteration %d\n", loop, iteration+1);
		for(n=0 ; n<u32_total_blk ; n++)
		{
			pr_dbg("test lun %d, lba %Xh, %Xh blk\n", TEST_LUN, dLba, n+1);

			err = ufs_write10(TEST_LUN, (void *)au16_WriteBuf, dLba, n+1);
			if (err)
			{
				dev_err(hba->dev, "ufs_write10 err = %d\n", err);
				return err;
			}

			memset(au16_ReadBuf, 0xFF, (n+1)*host->dLogicalBlkSize[TEST_LUN]);
			err = ufs_read10(TEST_LUN, (void *)au16_ReadBuf, dLba, n+1);
			if (err)
			{
				dev_err(hba->dev, "ufs_read10 err = %d\n", err);
				return err;
			}

			if(memcmp(au16_WriteBuf, au16_ReadBuf, (n+1)*host->dLogicalBlkSize[TEST_LUN]))
			{
				pr_dbg("dump write buffer : \n");
				print_hex_dump(KERN_ERR, "", DUMP_PREFIX_ADDRESS, 16, 4, au16_WriteBuf, 
							(n+1)*host->dLogicalBlkSize[TEST_LUN], false);

				pr_dbg("dump read buffer : \n");
				print_hex_dump(KERN_ERR, "", DUMP_PREFIX_ADDRESS, 16, 4, au16_ReadBuf, 
							(n+1)*host->dLogicalBlkSize[TEST_LUN], false);

				return -EIO;
			}
		}
	}

	return err;
}


int ufstest_rw6_lun(int loop)
{
	int err = 0;
	struct ufs_hba *hba;
	struct ufs_mstar_host *host;
	int n, iteration;
	u32 u32_total_blk;
	u16 *au16_WriteBuf = (u16 *)DMA_W_ADDR;
	u16 *au16_ReadBuf = (u16 *)DMA_R_ADDR;
	u32 dLba;

	hba = ufshcd_get_host();
	if(!hba)
	{
		dev_err(hba->dev, "ufshcd_get_host fail\n");
		return ENXIO;
	}
	host = (struct ufs_mstar_host *)ufshcd_get_variant(hba);

	ufstest_prepare_pattern(DMA_W_ADDR, UFS_IPVERIFY_PATITION_SIZE);
	u32_total_blk = UFS_IPVERIFY_PATITION_SIZE/host->dLogicalBlkSize[TEST_LUN];
	dLba = UFS_IPVERIFY_PATITION_START/host->dLogicalBlkSize[TEST_LUN];

	//Max transfer length of READ6/WRITE6 is 256
	if(u32_total_blk>256)
		u32_total_blk = 256;

	for(iteration=0 ; (loop==0)||(iteration<loop) ; iteration++)
	{
		pr_dbg("[ufs_test_rw6_lun] looping %d, iteration %d\n", loop, iteration+1);
		for(n=0 ; n<u32_total_blk ; n++)
		{
			pr_dbg("test lun %d, lba %Xh, %Xh blk\n", TEST_LUN, dLba, n+1);

			err = ufs_write6(TEST_LUN, (void *)au16_WriteBuf, dLba, n+1);
			if (err)
			{
				dev_err(hba->dev, "ufs_write6 err = %d\n", err);
				return err;
			}

			memset(au16_ReadBuf, 0xFF, (n+1)*host->dLogicalBlkSize[TEST_LUN]);
			err = ufs_read6(TEST_LUN, (void *)au16_ReadBuf, dLba, n+1);
			if (err)
			{
				dev_err(hba->dev, "ufs_read6 err = %d\n", err);
				return err;
			}

			if(memcmp(au16_WriteBuf, au16_ReadBuf, (n+1)*host->dLogicalBlkSize[TEST_LUN]))
			{
				pr_dbg("dump write buffer : \n");
				print_hex_dump(KERN_ERR, "", DUMP_PREFIX_ADDRESS, 16, 4, au16_WriteBuf, 
							(n+1)*host->dLogicalBlkSize[TEST_LUN], false);

				pr_dbg("dump read buffer : \n");
				print_hex_dump(KERN_ERR, "", DUMP_PREFIX_ADDRESS, 16, 4, au16_ReadBuf, 
							(n+1)*host->dLogicalBlkSize[TEST_LUN], false);

				return -EIO;
			}
		}
	}

	return err;
}

extern u32 u32_ufs_init_t0;	// real start tick of ufs_init
extern u32 u32_ufs_init_t1;	// real end tick of ufs_init

int ufstest_speed(int testcase)
{
	int err = 0;
	struct ufs_hba *hba;
	struct ufs_mstar_host *host;
	int n, iteration;
	u32 u32_total_blk;
	u32 dLba;
	u32 u32_i, u32_SectorAddr=0, u32_SectorCnt=0, u32_t0, u32_t1, u32_t;
	u8 u8_LoopCnt=0;

	if(testcase == 0)
	{
		u32_t = u32_ufs_init_t1 > u32_ufs_init_t0 ? u32_ufs_init_t1-u32_ufs_init_t0
						: 0xFFFFFFFF-u32_ufs_init_t0+u32_ufs_init_t1;
		pr_dbg("[ufs init time]\n"
				"start : %d\n"
				"end : %d\n"
				"total : %d : %d ms\n",
				u32_ufs_init_t0, u32_ufs_init_t1, u32_t,
			    u32_t/(UFS_HW_TIMER_HZ/1000));
		return 0;
	}

	hba = ufshcd_get_host();
	if(!hba)
	{
		dev_err(hba->dev, "ufshcd_get_host fail\n");
		return ENXIO;
	}
	host = (struct ufs_mstar_host *)ufshcd_get_variant(hba);
	u32_total_blk = UFS_IPVERIFY_PATITION_SIZE/host->dLogicalBlkSize[TEST_LUN];
	dLba = UFS_IPVERIFY_PATITION_START/host->dLogicalBlkSize[TEST_LUN];

	if( !gau8_WBuf )
		gau8_WBuf = (u8*)memalign(UFS_CACHE_LINE, UFS_IPVERIFY_PATITION_SIZE);

	if( !gau8_RBuf )
		gau8_RBuf = (u8*)memalign(UFS_CACHE_LINE, UFS_IPVERIFY_PATITION_SIZE);

	if(testcase == 1)
	{
		//ufs_count_down(3);

		for(u32_i=0; u32_i<UFS_IPVERIFY_PATITION_SIZE; u32_i++)
			gau8_WBuf[u32_i] = (u8)u32_i;

		pr_dbg("Single Write, total: %u KB, %u blocks, burst size: %Xh\n",
				(UFS_SPEED_TEST_SINGLE_BLKCNT_W<<UFS_SECTOR_4KBYTE_BITS)>>10,
				UFS_SPEED_TEST_SINGLE_BLKCNT_W, 1<<UFS_SECTOR_4KBYTE_BITS);
		ufs_hw_timer_start();
		u32_t0 = ufs_hw_timer_tick();
		for(u32_i=0; u32_i<UFS_SPEED_TEST_SINGLE_BLKCNT_W; u32_i++)
		{
			u32_SectorAddr = ufs_hw_timer_tick()%u32_total_blk;
			u32_SectorAddr += dLba;

			err = ufs_write10(TEST_LUN, (void *)gau8_WBuf, u32_SectorAddr, 1);
			if (err)
			{
				dev_err(hba->dev, "ufs_write10 err = %d\n", err);
				goto LABEL_PERF_END;
			}
		}
		u32_t1 = ufs_hw_timer_tick();
		u32_t = u32_t1 > u32_t0 ? u32_t1-u32_t0 : 0xFFFFFFFF-u32_t0+u32_t1;
		#if 0
		u32_t = u32_t/(UFS_HW_TIMER_HZ/1000); // ms
		if(0!=u32_t)
			pr_dbg(" %u KB/s \n", ((UFS_SPEED_TEST_SINGLE_BLKCNT_W<<UFS_SECTOR_4KBYTE_BITS)*1000/u32_t)>>10);
		else
			pr_dbg("t=0 \n");
		#else
		//pr_dbg("%d->%d = %d", u32_t0, u32_t1, u32_t);
		pr_dbg(" %u MB/s \n", ((UFS_SPEED_TEST_SINGLE_BLKCNT_W<<UFS_SECTOR_4KBYTE_BITS)*(UFS_HW_TIMER_HZ/1000000))/u32_t);
		#endif

		// -----------------------------------------
		pr_dbg("Single Read, total: %u KB, %u blocks, burst size: %Xh\n",
				((UFS_SPEED_TEST_SINGLE_BLKCNT_R)<<UFS_SECTOR_4KBYTE_BITS)>>10,
				UFS_SPEED_TEST_SINGLE_BLKCNT_R, 1<<UFS_SECTOR_4KBYTE_BITS);
		ufs_hw_timer_start();
		u32_t0 = ufs_hw_timer_tick();
		for(u32_i=0; u32_i<UFS_SPEED_TEST_SINGLE_BLKCNT_R; u32_i++)
		{
			u32_SectorAddr = ufs_hw_timer_tick()%u32_total_blk;
			u32_SectorAddr += dLba;

			err = ufs_read10(TEST_LUN, (void *)gau8_RBuf, u32_SectorAddr, 1);
			if (err)
			{
				dev_err(hba->dev, "ufs_read10 err = %d\n", err);
				goto LABEL_PERF_END;
			}
		}
		u32_t1 = ufs_hw_timer_tick();
		u32_t = u32_t1 > u32_t0 ? u32_t1-u32_t0 : 0xFFFFFFFF-u32_t0+u32_t1;
		#if 0
		u32_t = u32_t/(UFS_HW_TIMER_HZ/1000); // ms
		if(0!=u32_t)
			pr_dbg(" %u KB/s \n", ((UFS_SPEED_TEST_SINGLE_BLKCNT_R<<UFS_SECTOR_4KBYTE_BITS)*1000/u32_t)>>10);
		else
			pr_dbg("t=0 \n");
		#else
		//pr_dbg("%d->%d = %d", u32_t0, u32_t1, u32_t);
		pr_dbg(" %u MB/s \n", ((UFS_SPEED_TEST_SINGLE_BLKCNT_R<<UFS_SECTOR_4KBYTE_BITS)*(UFS_HW_TIMER_HZ/1000000))/u32_t);
		#endif

		// ==================================
		u32_SectorCnt = 0;
		LABEL_MULTI:
		pr_dbg("------------------------------\n");
		switch(u32_SectorCnt)
		{
			case 0:
				u32_SectorCnt = 0x400*4 >> UFS_SECTOR_4KBYTE_BITS; // 4K
			    break;
			case 0x400*4 >> UFS_SECTOR_4KBYTE_BITS:
				u32_SectorCnt = 0x400*16 >> UFS_SECTOR_4KBYTE_BITS; // 16KB
			    break;
			case 0x400*16 >> UFS_SECTOR_4KBYTE_BITS:
				u32_SectorCnt = 0x400*64 >> UFS_SECTOR_4KBYTE_BITS; // 64KB
			    break;
			case 0x400*64 >> UFS_SECTOR_4KBYTE_BITS:
				u32_SectorCnt = 0x400*1024 >> UFS_SECTOR_4KBYTE_BITS; // 1024KB
			    break;
		}
		pr_dbg("Multi Write, total: %u MB, burst size: %u KB\n",
			(UFS_SPEED_TEST_MULTIPLE_BLKCNT_W<<UFS_SECTOR_4KBYTE_BITS)>>20, u32_SectorCnt<<(UFS_SECTOR_4KBYTE_BITS-10));

		u32_SectorAddr = dLba;
		ufs_hw_timer_start();
		u32_t0 = ufs_hw_timer_tick();
		for(u32_i=0; u32_i<UFS_SPEED_TEST_MULTIPLE_BLKCNT_W; u32_i += u32_SectorCnt)
		{
			err = ufs_write10(TEST_LUN, (void *)gau8_WBuf, u32_SectorAddr, u32_SectorCnt);
			if (err)
			{
				dev_err(hba->dev, "ufs_write10 err = %d\n", err);
				goto LABEL_PERF_END;
			}			
		}
		u32_t1 = ufs_hw_timer_tick();
		u32_t = u32_t1 > u32_t0 ? u32_t1-u32_t0 : 0xFFFFFFFF-u32_t0+u32_t1;
		#if 0
		u32_t = u32_t/(UFS_HW_TIMER_HZ/100); // 10ms
		if(0!=u32_t)
			pr_dbg(" %u.%u MB/s \n",
			    ((UFS_SPEED_TEST_MULTIPLE_BLKCNT_W<<UFS_SECTOR_4KBYTE_BITS)*100/u32_t)>>20,
			    (((UFS_SPEED_TEST_MULTIPLE_BLKCNT_W<<UFS_SECTOR_4KBYTE_BITS)*100/u32_t)>>10)*10/1024%10);
		else
			pr_dbg("t=0 \n");
		#else
		//pr_dbg("%d->%d = %d", u32_t0, u32_t1, u32_t);
		pr_dbg(" %u MB/s \n", ((UFS_SPEED_TEST_MULTIPLE_BLKCNT_W<<UFS_SECTOR_4KBYTE_BITS)*(UFS_HW_TIMER_HZ/1000000))/u32_t);
		#endif

		// -----------------------------------------
		pr_dbg("Multi Read, total: %uMB, burst size: %u KB \n",
			(UFS_SPEED_TEST_MULTIPLE_BLKCNT_R<<UFS_SECTOR_4KBYTE_BITS)>>20, u32_SectorCnt<<(UFS_SECTOR_4KBYTE_BITS-10));

		u32_SectorAddr = dLba;
		ufs_hw_timer_start();
		u32_t0 = ufs_hw_timer_tick();
		for(u32_i=0; u32_i<UFS_SPEED_TEST_MULTIPLE_BLKCNT_R; u32_i += u32_SectorCnt)
		{
			err = ufs_read10(TEST_LUN, (void *)gau8_RBuf, u32_SectorAddr, u32_SectorCnt);
			if (err)
			{
				dev_err(hba->dev, "ufs_read10 err = %d\n", err);
				goto LABEL_PERF_END;
			}			
		}
		u32_t1 = ufs_hw_timer_tick();
		u32_t = u32_t1 > u32_t0 ? u32_t1-u32_t0 : 0xFFFFFFFF-u32_t0+u32_t1;
		#if 0
		u32_t = u32_t/(UFS_HW_TIMER_HZ/100); // 10ms
		if(0!=u32_t)
			pr_dbg(" %u.%u MB/s \n",
			    ((UFS_SPEED_TEST_MULTIPLE_BLKCNT_R<<UFS_SECTOR_4KBYTE_BITS)*100/u32_t)>>20,
			    (((UFS_SPEED_TEST_MULTIPLE_BLKCNT_R<<UFS_SECTOR_4KBYTE_BITS)*100/u32_t)>>10)*10/1024%10);
		else
			pr_dbg("t=0 \n");
		#else
		//pr_dbg("%d->%d = %d", u32_t0, u32_t1, u32_t);
		pr_dbg(" %u MB/s \n", ((UFS_SPEED_TEST_MULTIPLE_BLKCNT_R<<UFS_SECTOR_4KBYTE_BITS)*(UFS_HW_TIMER_HZ/1000000))/u32_t);
		#endif

		if(u32_SectorCnt != 0x400*1024 >> UFS_SECTOR_4KBYTE_BITS)
			goto LABEL_MULTI;
	}
	else if(testcase == 2)
	{
		ufstest_prepare_pattern(gau8_WBuf, UFS_IPVERIFY_PATITION_SIZE);
		u32_total_blk = UFS_IPVERIFY_PATITION_SIZE/host->dLogicalBlkSize[TEST_LUN];
		dLba = UFS_IPVERIFY_PATITION_START/host->dLogicalBlkSize[TEST_LUN];

		for(n=1 ; n<=u32_total_blk ; n*=2)
		{
			ufs_hw_timer_start();
			u32_t0 = ufs_hw_timer_tick();
			err = ufs_write10(TEST_LUN, (void *)gau8_WBuf, dLba, n);
			if (err)
			{
				dev_err(hba->dev, "ufs_write10 err = %d\n", err);
				return err;
			}
			u32_t1 = ufs_hw_timer_tick();
			u32_t = u32_t1 > u32_t0 ? u32_t1-u32_t0	: 0xFFFFFFFF-u32_t0+u32_t1;
			pr_dbg("write %d blk, %d KB : %d tick, %d MB/s\n", n, n*host->dLogicalBlkSize[TEST_LUN]/1024,
					u32_t, (n*host->dLogicalBlkSize[TEST_LUN]*(UFS_HW_TIMER_HZ/1000000))/u32_t);

			memset(gau8_RBuf, 0xFF, n*host->dLogicalBlkSize[TEST_LUN]);
			ufs_hw_timer_start();
			u32_t0 = ufs_hw_timer_tick();
			err = ufs_read10(TEST_LUN, (void *)gau8_RBuf, dLba, n);
			if (err)
			{
				dev_err(hba->dev, "ufs_read10 err = %d\n", err);
				return err;
			}
			u32_t1 = ufs_hw_timer_tick();
			u32_t = u32_t1 > u32_t0 ? u32_t1-u32_t0	: 0xFFFFFFFF-u32_t0+u32_t1;
			pr_dbg("read %d blk, %d KB : %d tick, %d MB/s\n", n, n*host->dLogicalBlkSize[TEST_LUN]/1024,
					u32_t, (n*host->dLogicalBlkSize[TEST_LUN]*(UFS_HW_TIMER_HZ/1000000))/u32_t);

			if(memcmp(gau8_WBuf, gau8_RBuf, n*host->dLogicalBlkSize[TEST_LUN]))
			{
				pr_dbg("dump write buffer : \n");
				print_hex_dump(KERN_ERR, "", DUMP_PREFIX_ADDRESS, 16, 4, gau8_WBuf, 
							n*host->dLogicalBlkSize[TEST_LUN], false);

				pr_dbg("dump read buffer : \n");
				print_hex_dump(KERN_ERR, "", DUMP_PREFIX_ADDRESS, 16, 4, gau8_RBuf, 
							n*host->dLogicalBlkSize[TEST_LUN], false);

				return -EIO;
			}

			pr_dbg("\n");
		}
	}

    LABEL_PERF_END:

    if( gau8_WBuf )
    {
        free(gau8_WBuf);
        gau8_WBuf = 0;
    }

    if( gau8_RBuf )
    {
        free(gau8_RBuf);
        gau8_RBuf = 0;
    }

	return err;
}

u32 ufstest_ChkSum(u8 *pu8_Data, u32 u32_ByteCnt)
{
	volatile u32 u32_Sum=0, u32_Tmp;
	for (u32_Tmp=0; u32_Tmp < u32_ByteCnt; u32_Tmp++)
		u32_Sum += pu8_Data[u32_Tmp];
	return (u32_Sum);
}

void ufstest_DumpsPwrCutCtrlBlk(UFS_PWRCUT_CTRLBLK_t *pCtrlBlk_t)
{
	u32 u32_i;

	pr_dbg("UFS: CheckSum: %Xh\n", pCtrlBlk_t->u32_CheckSum);
	pr_dbg("UFS: TargetUnit: %Xh\n", pCtrlBlk_t->u8_TargetUnit);

	pr_dbg("  Target Data: ");
	for(u32_i=0; u32_i<UFS_PWRCUT_TEST_UNIT_CNT; u32_i++)
	{
		if(0 == (u32_i & 0xF))
			pr_dbg("\n");

		pr_dbg(" %Xh", pCtrlBlk_t->au8_TaegetData[u32_i]);
	}
	pr_dbg("\n");
}

u32 ufstest_CheckPwrCutCtrlBlk(u8* u8_DataBuf, u32 u32_BlkAddr)
{
	u32 u32_err;
	UFS_PWRCUT_CTRLBLK_t *pCtrlBlk_t;

	u32_err = ufs_read10(TEST_LUN, u8_DataBuf, u32_BlkAddr, 1);
	if(u32_err != 0)
    {
        pr_dbg("UFS Err: read CtrlBlk, Err:%x\n", u32_err);
	    return u32_err;
    }

	pCtrlBlk_t = (UFS_PWRCUT_CTRLBLK_t*)u8_DataBuf;

	if(pCtrlBlk_t->u32_CheckSum !=
		ufstest_ChkSum(u8_DataBuf+4, sizeof(UFS_PWRCUT_CTRLBLK_t)-4))
	{
		pr_dbg("UFS Err: read CtrlBlk, CheckSum fail\n");
	    return -EIO;
	}

	return 0;
}


u32 ufstest_PwrCut_InitData(u8* u8_DataBuf, u32 u32_BlkStartAddr)
{
	u32 u32_i, u32_j;
	u32 u32_err = 0;
	UFS_PWRCUT_CTRLBLK_t CtrlBlk_t;

	// ---------------------------------------
	CtrlBlk_t.u8_TargetUnit = 0xFF;

	for(u32_i=0; u32_i<UFS_PWRCUT_TEST_UNIT_CNT; u32_i++)
		CtrlBlk_t.au8_TaegetData[u32_i] = UFS_PWRCUT_DATA0;

	CtrlBlk_t.u32_CheckSum = ufstest_ChkSum((u8*)&CtrlBlk_t.u8_TargetUnit,
		sizeof(UFS_PWRCUT_CTRLBLK_t)-4);

	u32_err = ufs_write10(TEST_LUN, (void *) &CtrlBlk_t, u32_BlkStartAddr, 1);
	if(u32_err != 0)
    {
        pr_dbg("UFS Err: write CtrlBlk, Err:%x\n", u32_err);
	    return u32_err;
    }

	// ---------------------------------------
	u32_BlkStartAddr+=1; // skip Ctrl Blk

    memset(u8_DataBuf, UFS_PWRCUT_DATA0, UFS_PWRCUT_TEST_UNIT_BYTECNT);
	pr_dbg("Reliable write ...\n");

	for(u32_i=0; u32_i<UFS_PWRCUT_TEST_UNIT_CNT; u32_i++)
	{
		u32_err = ufs_write10(TEST_LUN, (void *) u8_DataBuf, 
							u32_BlkStartAddr + u32_i*(UFS_PWRCUT_TEST_UNIT_BYTECNT>>12),
							UFS_PWRCUT_TEST_UNIT_BYTECNT>>12);
	    if(u32_err != 0)
        {
            pr_dbg("UFS Err: Reliable write Fail @ %u unit, Err:%x\n", u32_i, u32_err);
	        return u32_err;
        }
		pr_dbg("%03u%% \r", (u32_i+1)*100/UFS_PWRCUT_TEST_UNIT_CNT);
	}

	// ---------------------------------------
	pr_dbg("\ncheck ...\n");

	if(ufstest_CheckPwrCutCtrlBlk(u8_DataBuf, u32_BlkStartAddr-1) != 0)
	{
		pr_dbg("UFS Err: read CtrlBlk fail\n");
	    return -EIO;
	}

	for(u32_i=0; u32_i<UFS_PWRCUT_TEST_UNIT_CNT; u32_i++)
	{
		u32_err = ufs_read10(TEST_LUN, (void *) u8_DataBuf,
							u32_BlkStartAddr + u32_i*(UFS_PWRCUT_TEST_UNIT_BYTECNT>>12),
							UFS_PWRCUT_TEST_UNIT_BYTECNT>>12);
	    if(u32_err != 0)
        {
            pr_dbg("UFS Err: Reliable read Fail @ %u unit, Err:%x\n", u32_i, u32_err);
	        return u32_err;
        }

		for (u32_j=0; u32_j < UFS_PWRCUT_TEST_UNIT_BYTECNT; u32_j++)
	    {
		    if (u8_DataBuf[u32_j] != UFS_PWRCUT_DATA0)
	 	    {
			    pr_dbg("UFS Err: Data Mismatch:  Blk:%Xh  ByteIdx:%Xh  ByteVal:%Xh \n",
					u32_BlkStartAddr+u32_i*(UFS_PWRCUT_TEST_UNIT_BYTECNT>>12)+(u32_j>>12),
					u32_j & 0x1FF, u8_DataBuf[u32_j]);
			    return -EIO;
		    }
	    }

		pr_dbg("%03u%% \r", (u32_i+1)*100/UFS_PWRCUT_TEST_UNIT_CNT);
	}

	pr_dbg("\n init success\n");
	return 0;
}


u32 ufstest_PwrCut_Test(u8* u8_DataBuf, u32 u32_BlkStartAddr)
{
	u32 u32_i, u32_j;
	u32 u32_err = 0;
	u32 u32_T0, u32_BlkAddr;
	UFS_PWRCUT_CTRLBLK_t CtrlBlk_t;

	ufs_hw_timer_start();

	// ---------------------------------------
	pr_dbg("\n UFS PwrCut Test, checking ...\n");

	// get Ctrl Blk
	if(ufstest_CheckPwrCutCtrlBlk(u8_DataBuf, u32_BlkStartAddr) != 0)
	{
		pr_dbg("UFS Err: read CtrlBlk fail\n");
		while(1);
	}
	memcpy(&CtrlBlk_t, u8_DataBuf, sizeof(UFS_PWRCUT_CTRLBLK_t));

	// check data
	u32_BlkStartAddr++;

	for(u32_i=0; u32_i<UFS_PWRCUT_TEST_UNIT_CNT; u32_i++)
	{
		u32_err = ufs_read10(TEST_LUN, (void *) u8_DataBuf,
							u32_BlkStartAddr + u32_i*(UFS_PWRCUT_TEST_UNIT_BYTECNT>>12),
							UFS_PWRCUT_TEST_UNIT_BYTECNT>>12);
	    if(u32_err != 0)
        {
            pr_dbg(	"\nUFS Err: Reliable read Fail @ %u unit, Err:%x\n", u32_i, u32_err);
	        //return u32_err;
	        while(1);
        }

		if(u32_i == CtrlBlk_t.u8_TargetUnit){
			for (u32_j=0; u32_j < UFS_PWRCUT_TEST_UNIT_BYTECNT; u32_j++)
		    {
			    if (u8_DataBuf[u32_j]!=UFS_PWRCUT_DATA0 &&
					u8_DataBuf[u32_j]!=UFS_PWRCUT_DATA1)
		 	    {
				    pr_dbg("\nUFS Err: Target Data Mismatch:  Unit:%Xh  "
						"Blk: %Xh + %Xh x %Xh + %Xh = %Xh, ByteIdx:%Xh  ByteVal:%Xh \n",
						u32_i,
						u32_BlkStartAddr, u32_i, (UFS_PWRCUT_TEST_UNIT_BYTECNT>>12), (u32_j>>12),
						u32_BlkStartAddr+u32_i*(UFS_PWRCUT_TEST_UNIT_BYTECNT>>12)+(u32_j>>12),
						u32_j & 0xFFF, u8_DataBuf[u32_j]);
					ufstest_DumpsPwrCutCtrlBlk(&CtrlBlk_t);
					print_hex_dump(KERN_ERR, "", DUMP_PREFIX_ADDRESS, 16, 4, &u8_DataBuf[u32_j]-0x10, 
								0x30, false);

				    while(1);
			    }
		    }
			// recover TargetUnit
			u32_BlkAddr = u32_BlkStartAddr +
				CtrlBlk_t.u8_TargetUnit *
				(UFS_PWRCUT_TEST_UNIT_BYTECNT>>UFS_SECTOR_4KBYTE_BITS);
			memset(u8_DataBuf, CtrlBlk_t.au8_TaegetData[CtrlBlk_t.u8_TargetUnit], UFS_PWRCUT_TEST_UNIT_BYTECNT);
			u32_err = ufs_write10(TEST_LUN, (void *) u8_DataBuf, u32_BlkAddr, UFS_PWRCUT_TEST_UNIT_BYTECNT>>12);
			if(u32_err != 0)
			{
				pr_dbg("UFS Err: write TargetUnit fail, Err:%x\n",u32_err);
				//return u32_err;
				while(1);
			}
		}
		else{
			for (u32_j=0; u32_j < UFS_PWRCUT_TEST_UNIT_BYTECNT; u32_j++)
		    {
			    if (u8_DataBuf[u32_j]!=CtrlBlk_t.au8_TaegetData[u32_i])
		 	    {
				    pr_dbg("\nUFS Err: Data Mismatch:  Unit:%Xh  "
						"Blk: %Xh + %Xh x %Xh + %Xh = %Xh, ByteIdx:%Xh  ByteVal:%Xh \n",
						u32_i,
						u32_BlkStartAddr, u32_i, (UFS_PWRCUT_TEST_UNIT_BYTECNT>>12), (u32_j>>12),
						u32_BlkStartAddr+u32_i*(UFS_PWRCUT_TEST_UNIT_BYTECNT>>12)+(u32_j>>12),
						u32_j & 0xFFF, u8_DataBuf[u32_j]);
					ufstest_DumpsPwrCutCtrlBlk(&CtrlBlk_t);
					print_hex_dump(KERN_ERR, "", DUMP_PREFIX_ADDRESS, 16, 4, &u8_DataBuf[u32_j]-0x10, 
								0x30, false);

					while(1);
			    }
		    }
		}

		pr_dbg("%03u%% \r", (u32_i+1)*100/UFS_PWRCUT_TEST_UNIT_CNT);
	}

	pr_dbg("   ok\n");

    // ---------------------------------------
	while(1)
    {
		u32_T0 = ufs_hw_timer_tick();
		CtrlBlk_t.u8_TargetUnit = u32_T0 % UFS_PWRCUT_TEST_UNIT_CNT;
		CtrlBlk_t.au8_TaegetData[CtrlBlk_t.u8_TargetUnit] ^= 0xFF;
		CtrlBlk_t.u32_CheckSum = ufstest_ChkSum((u8*)&CtrlBlk_t.u8_TargetUnit,
		    sizeof(UFS_PWRCUT_CTRLBLK_t)-4);
		u32_err = ufs_write10(TEST_LUN, (void *) &CtrlBlk_t, u32_BlkStartAddr-1, 1);
	    if(u32_err != 0)
        {
            pr_dbg("UFS Err: write CtrlBlk, Err:%x\n", u32_err);
	        return u32_err;
        }

		u32_BlkAddr = u32_BlkStartAddr +
			CtrlBlk_t.u8_TargetUnit *
			(UFS_PWRCUT_TEST_UNIT_BYTECNT>>UFS_SECTOR_4KBYTE_BITS);
		memset(u8_DataBuf, CtrlBlk_t.au8_TaegetData[CtrlBlk_t.u8_TargetUnit], UFS_PWRCUT_TEST_UNIT_BYTECNT);

		pr_dbg("writing... %02Xh %08Xh %02Xh\n",
			CtrlBlk_t.u8_TargetUnit, u32_BlkAddr, CtrlBlk_t.au8_TaegetData[CtrlBlk_t.u8_TargetUnit]);

		u32_err = ufs_write10(TEST_LUN, (void *) u8_DataBuf, u32_BlkAddr, UFS_PWRCUT_TEST_UNIT_BYTECNT>>12);
		if(u32_err != 0)
		{
			pr_dbg("UFS Err: write Fail, Err:%x\n",u32_err);
			return u32_err;
		}
    }

	return 0;
}

int ufstest_hibern8(int cmd)
{
	int err = 0;
	struct ufs_hba *hba;
	int loop = 0;

	hba = ufshcd_get_host();
	if(!hba)
	{
		dev_err(hba->dev, "ufshcd_get_host fail\n");
		return ENXIO;
	}

	if(cmd == UIC_CMD_DME_HIBER_ENTER)
	{
		pr_dbg("ufshcd_uic_hibern8_enter...\n");
		err = ufshcd_uic_hibern8_enter(hba);
		if (err)
		{
			dev_err(hba->dev, "ufshcd_uic_hibern8_enter err = %d\n", err);
			return err;
		}
		pr_dbg("OK\n");
	}
	else if(cmd == UIC_CMD_DME_HIBER_EXIT)
	{
		pr_dbg("ufshcd_uic_hibern8_exit...\n");
		err = ufshcd_uic_hibern8_exit(hba);
		if (err)
		{
			dev_err(hba->dev, "ufshcd_uic_hibern8_exit err = %d\n", err);
			return err;
		}
		pr_dbg("OK\n");
	}
	else if(cmd == 0)
	{
		while(1)
		{
			pr_dbg("[ufs_test_hibern8] iteration %d\n", ++loop);
			err = ufshcd_uic_hibern8_enter(hba);
			if(err)
			{
				pr_err("ufshcd_uic_hibern8_enter failed %d\n", err);
				return err;
			}

			udelay(3000000);

			err = ufshcd_uic_hibern8_exit(hba);
			if(err)
			{
				pr_err("ufshcd_uic_hibern8_exit failed %d\n", err);
				return err;
			}
		}
	}

	return err;
}

int ufs_mstar_ipverify(void)
{
	int err = 0;

	pr_dbg("===================================\n");
	pr_dbg("UFS IP Verification Program  \n");
	pr_dbg("===================================\n");

#if 0
	err = ufs_init();
	if(err)
	{
		pr_err("ufs_init() failed %d\n", err);
		goto out;
	}
#endif

	#if defined(UFS_TEST_SUITE_INFO) && (UFS_TEST_SUITE_INFO)
	pr_dbg("ufs_test_info()...\n");
	err = ufstest_info();
	if(err)
	{
		pr_err("ufs_test_show_lun() failed %d\n", err);
		goto out;
	}

	pr_dbg("ufs_test_show_lun() OK\n");
	#endif

	#if defined(UFS_TEST_SUITE_RW6_LUN) && (UFS_TEST_SUITE_RW6_LUN)
	pr_dbg("ufs_test_rw6_lun() ...\n");
	err = ufstest_rw6_lun(1);
	if(err)
	{
		pr_err("ufs_test_rw6_lun() failed %d\n", err);
		goto out;
	}

	pr_dbg("ufs_test_rw6_lun() OK\n");
	#endif

	#if defined(UFS_TEST_SUITE_RW10_LUN) && (UFS_TEST_SUITE_RW10_LUN)
	pr_dbg("ufs_test_rw10_lun() ...\n");
	err = ufstest_rw10_lun(1);
	if(err)
	{
		pr_err("ufs_test_rw10_lun() failed %d\n", err);
		goto out;
	}

	pr_dbg("ufs_test_rw10_lun() OK\n");
	#endif


	#if defined(UFS_TEST_SUITE_SPEED) && (UFS_TEST_SUITE_SPEED)
	pr_dbg("ufs_test_speed() ...\n");
	err = ufstest_speed(1);
	if(err)
	{
		pr_err("ufs_test_speed(1) failed %d\n", err);
		goto out;
	}

	err = ufstest_speed(0);
	if(err)
	{
		pr_err("ufs_test_speed(0) failed %d\n", err);
		goto out;
	}
	#endif

	#if defined(UFS_TEST_SUITE_PWR_CUT) && (UFS_TEST_SUITE_PWR_CUT)
	// TODO
	#endif

	#if defined(UFS_TEST_SUITE_HIBERN8) && (UFS_TEST_SUITE_HIBERN8)
	pr_dbg("ufs_test_hibern8() ...\n");
	err = ufstest_hibern8(0);
	if(err)
	{
		pr_err("ufs_test_hibern8() failed %d\n", err);
		goto out;
	}
	#endif

out:
	if(err)
		pr_err("UFS IP Verified Fail %d\n", err);
	else
		pr_dbg("UFS IP Verified OK!\n");

	return err;

}


#endif
