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
#include <config.h>
#include <common.h>
#include <command.h>
#include <malloc.h>
#include <watchdog.h>
#include <fat.h>
#include <MsMath.h>
#include <MsUfs.h>
#include <u-boot/crc.h>
#include <MsDebug.h>

#ifdef CONFIG_LZO
#include <linux/lzo.h>
#include <MsEnvironment.h>
#endif

#include "../disk/part_ufs.h"
#include <CusRawIO.h>
#include <sparse_format.h>
#include "ufs-mstar.h"
#include "ufshcd.h"

extern int cur_ufs_dev_num;

static void print_ufsinfo(struct ufs_mstar_host *host)
{
	int i;

	printf("Device: %s\n", "ufs");
	printf("Manufacturer ID: %x\n", host->hba->wmanufacturerid);
	printf("Model: %s\n", host->hba->model);
	printf("version %02x.%02x\n", (host->hba->ufs_version>>8)&0xff, host->hba->ufs_version&0xff);

	for(i=0 ; i<MAX_UFS_MSTAR_LUN ; i++)
	{
		printf("LUN %d :\n", i);
		printf("\tBlk Size : %d\n", host->dLogicalBlkSize[i]);
		printf("\tBlk Cnt : %lld\n", host->qLogicalBlkCnt[i]);
		print_size((unsigned long long)host->dLogicalBlkSize[i]*(unsigned long long)host->qLogicalBlkCnt[i], "\n");
	}
}

int do_ufsinfo (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	struct ufs_hba *hba;
	struct ufs_mstar_host *host;

	if(cur_ufs_dev_num < 0)
	{
		puts("No UFS device available\n");
		return 1;
	}

	hba = ufshcd_get_host();
	host = (struct ufs_mstar_host *)ufshcd_get_variant(hba);
	if(host == NULL)
	{
		printf("no ufs device at slot %x\n", cur_ufs_dev_num);
		return 1;
	}

	print_ufsinfo(host);

	return 0;
}

static int cmd_part(cmd_tbl_t *cmdtp_t, int flag_t, int argc_t, char * const argv_t[])
{
	block_dev_desc_t *ufs_dev;

	if(cur_ufs_dev_num < 0)
	{
		puts("No UFS device available\n");
		return 1;
	}

	ufs_dev = ufs_get_dev(cur_ufs_dev_num);
	if(ufs_dev != NULL && ufs_dev->type != DEV_TYPE_UNKNOWN)
	{
		print_part(ufs_dev);
		return 0;
	}
 
	puts("get ufs type error!\n");
	return 1;
}

static int cmd_read(cmd_tbl_t *cmdtp_t, int flag_t, int argc_t, char * const argv_t[])
{
	void *addr;
	u32 n, cnt, size, partnum = 1, bootpart = -1;
	block_dev_desc_t *ufs_dev;
	disk_partition_t dpt;
	s32 blk = -1, temp;
	char* cmdtail = strchr(argv_t[1], '.');
	char* cmdlasttail = strrchr(argv_t[1], '.');
	struct ufs_hba *hba;
	struct ufs_mstar_host *host;

	if(cur_ufs_dev_num < 0)
	{
		puts("No UFS device available\n");
		return 1;
	}

	hba = ufshcd_get_host();
	host = (struct ufs_mstar_host *)ufshcd_get_variant(hba);
	if(host == NULL)
	{
		printf("no ufs device at slot %x\n", cur_ufs_dev_num);
		return 1;
	}

	ufs_dev = ufs_get_dev(cur_ufs_dev_num);
	if ((ufs_dev == NULL) || ufs_dev->type == DEV_TYPE_UNKNOWN)
	{
		printf("no ufs device found!\n");
		return 1;
	}

	if(cmdtail == NULL) // read
	{
		addr = (void *)simple_strtoul(argv_t[2], NULL, 16);
		blk = simple_strtoul(argv_t[3], NULL, 16);
		size = simple_strtoul(argv_t[4], NULL, 16);
		cnt = ALIGN(size, 4096) / 4096;
		if(blk+cnt > host->qLogicalBlkCnt[0])
		{
			printf("out of range, read cnt %x > actual cnt %llx\n", cnt, host->qLogicalBlkCnt[0]);
			return 1;
		}

		n = ufs_dev->block_read(cur_ufs_dev_num, blk, cnt, addr);
	}
	else if(!strcmp(cmdtail, ".boot")) // read.boot
	{
		if(argc_t == 5)
		{
			bootpart = 1;
			addr = (void *)simple_strtoul(argv_t[2], NULL, 16);
			blk = simple_strtoul(argv_t[3], NULL, 16);
			size = simple_strtoul(argv_t[4], NULL, 16);
			cnt = ALIGN(size, 4096) / 4096;
		}
		else
		{
			bootpart = simple_strtoul(argv_t[2], NULL, 16);
			if(bootpart > 2)
			{
				printf("ufs access boot partition No. %d is invalid!\n", bootpart);
				return 1;
			}
			addr = (void *)simple_strtoul(argv_t[3], NULL, 16);
			blk = simple_strtoul(argv_t[4], NULL, 16);
			size = simple_strtoul(argv_t[5], NULL, 16);
			cnt = ALIGN(size, 4096) / 4096;
		}

		if(blk+cnt > host->qLogicalBlkCnt[bootpart])
		{
			printf("out of range, start blk %x read cnt %x > actual cnt %llx\n", blk, cnt, host->qLogicalBlkCnt[bootpart]);
			return 1;
		}

		if(ufs_read10(bootpart, addr, blk, cnt) == 0)
			n = cnt;
		else
			n = 0;
	}
	else if(!strncmp(cmdtail, ".p", 2)) // read.p or read.p.continue
	{
		for(;;)
		{
			// search from 1st partition
			if(get_partition_info_ufs(ufs_dev, partnum, &dpt))
			{
                printf("no part:%s found!\n", argv_t[3]);
				return 1;
			}
			else
			{	
				if(!strcmp(argv_t[3], (const char *)dpt.name)) 
				{
					blk = dpt.start;
					break;
				}
			}

			partnum++;
		}
	
		if(!strcmp(cmdlasttail, ".continue"))
		{
			addr = (void *)simple_strtoul(argv_t[2], NULL, 16);
			temp = simple_strtoul(argv_t[4], NULL, 16);
			blk += temp;
			size = simple_strtoul(argv_t[5], NULL, 16);
			cnt = ALIGN(size, 4096) / 4096;
			if(temp+cnt > dpt.size)
			{
				printf("out of range, start blk %x read cnt %x > actual cnt %llx\n", temp, cnt, dpt.size);
				return 1;
			}
		}
		else
		{
			addr = (void *)simple_strtoul(argv_t[2], NULL, 16);
			size = simple_strtoul(argv_t[4], NULL, 16);
			cnt = ALIGN(size, 4096) / 4096;
			if(cnt > dpt.size)
			{
				printf("out of range, read cnt %x > actual cnt %llx\n", cnt, dpt.size);
				return 1;
			}
		}

		n = ufs_dev->block_read(cur_ufs_dev_num, blk, cnt, addr);
	}
	else
		return cmd_usage(cmdtp_t);

	return (n == cnt) ? 0 : 1;
}

static int cmd_write(cmd_tbl_t *cmdtp_t, int flag_t, int argc_t, char * const argv_t[])
{
	void *addr;
	u32 n, cnt, size, partnum = 1, bootpart = -1;
	block_dev_desc_t *ufs_dev;
	disk_partition_t dpt;
	s32 blk = -1, temp;
	char* cmdtail = strchr(argv_t[1], '.');
	char* cmdlasttail = strrchr(argv_t[1], '.');
	struct ufs_hba *hba;
	struct ufs_mstar_host *host;

	if(cur_ufs_dev_num < 0)
	{
		puts("No UFS device available\n");
		return 1;
	}

	hba = ufshcd_get_host();
	host = (struct ufs_mstar_host *)ufshcd_get_variant(hba);
	if(host == NULL)
	{
		printf("no ufs device at slot %x\n", cur_ufs_dev_num);
		return 1;
	}

	ufs_dev = ufs_get_dev(cur_ufs_dev_num);
	if ((ufs_dev == NULL) || ufs_dev->type == DEV_TYPE_UNKNOWN)
	{
		printf("no ufs device found!\n");
		return 1;
	}

	if(cmdtail == NULL) // write
	{
		addr = (void *)simple_strtoul(argv_t[2], NULL, 16);
		blk = simple_strtoul(argv_t[3], NULL, 16);
		size = simple_strtoul(argv_t[4], NULL, 16);
		cnt = ALIGN(size, 4096) / 4096;
		if(blk+cnt > host->qLogicalBlkCnt[0])
		{
			printf("out of range, write cnt %x > actual cnt %llx\n", cnt, host->qLogicalBlkCnt[0]);
			return 1;
		}

		n = ufs_dev->block_write(cur_ufs_dev_num, blk, cnt, addr);
	}
	else if(!strcmp(cmdtail, ".boot")) // write.boot
	{
		if(argc_t == 5)
		{
			bootpart = 1;
			addr = (void *)simple_strtoul(argv_t[2], NULL, 16);
			blk = simple_strtoul(argv_t[3], NULL, 16);
			size = simple_strtoul(argv_t[4], NULL, 16);
			cnt = ALIGN(size, 4096) / 4096;
		}
		else
		{
			bootpart = simple_strtoul(argv_t[2], NULL, 16);
			if(bootpart > 2)
			{
				printf("ufs access boot partition No. %d is invalid!\n", bootpart);
				return 1;
			}
			addr = (void *)simple_strtoul(argv_t[3], NULL, 16);
			blk = simple_strtoul(argv_t[4], NULL, 16);
			size = simple_strtoul(argv_t[5], NULL, 16);
			cnt = ALIGN(size, 4096) / 4096;
		}

		if(blk+cnt > host->qLogicalBlkCnt[bootpart])
		{
			printf("out of range, start blk %x write cnt %x > actual cnt %llx\n", blk, cnt, host->qLogicalBlkCnt[bootpart]);
			return 1;
		}

		if(ufs_write10(bootpart, addr, blk, cnt) == 0)
			n = cnt;
		else
			n = 0;
	}
	else if(!strncmp(cmdtail, ".p", 2)) // write.p or write.p.continue
	{
		for(;;)
		{
			// search from 1st partition
			if(get_partition_info_ufs(ufs_dev, partnum, &dpt))
			{
                printf("no part:%s found!\n", argv_t[3]);
				return 1;
			}
			else
			{	
				if(!strcmp(argv_t[3], (const char *)dpt.name)) 
				{
					blk = dpt.start;
					break;
				}
			}

			partnum++;
		}
	
		if(!strcmp(cmdlasttail, ".continue"))
		{
			addr = (void *)simple_strtoul(argv_t[2], NULL, 16);
			temp = simple_strtoul(argv_t[4], NULL, 16);
			blk += temp;
			size = simple_strtoul(argv_t[5], NULL, 16);
			cnt = ALIGN(size, 4096) / 4096;
			if(temp+cnt > dpt.size)
			{
				printf("out of range, start blk %x write cnt %x > actual cnt %llx\n", temp, cnt, dpt.size);
				return 1;
			}
		}
		else
		{
			addr = (void *)simple_strtoul(argv_t[2], NULL, 16);
			size = simple_strtoul(argv_t[4], NULL, 16);
			cnt = ALIGN(size, 4096) / 4096;
			if(cnt > dpt.size)
			{
				printf("out of range, write cnt %x > actual cnt %llx\n", cnt, dpt.size);
				return 1;
			}
		}

		n = ufs_dev->block_write(cur_ufs_dev_num, blk, cnt, addr);
	}
	else
		return cmd_usage(cmdtp_t);

	return (n == cnt) ? 0 : 1;
}

static int cmd_create(cmd_tbl_t *cmdtp_t, int flag_t, int argc_t, char * const argv_t[])
{
	block_dev_desc_t *ufs_dev;
	disk_partition_t dpt;

	if(argc_t != 4)
	{
		puts("Wrong argument number\n");
		return 1;
	}

	if(cur_ufs_dev_num < 0)
	{
		puts("No UFS device available\n");
		return 1;
	}

	ufs_dev = ufs_get_dev(cur_ufs_dev_num);
	if ((ufs_dev == NULL) || ufs_dev->type == DEV_TYPE_UNKNOWN)
	{
		printf("no ufs device found!\n");
		return 1;
	}

	if(simple_strtoull(argv_t[3], NULL, 16)%4096!=0)
	{
		printf("\n\n\n******** ERROR:partition (%s) size must be 4096-byte aligned! ******** \n\n\n",argv_t[2]);
	}

	dpt.size = ALIGN(simple_strtoull(argv_t[3], NULL, 16), 4096) / 4096;
	dpt.start = 0;
	strcpy((char *)&dpt.name, argv_t[2]);
	if (add_new_ufs_partition(ufs_dev, &dpt) == 0){
		printf("Add new partition %s success!\n", dpt.name);
		return 0;
	}

	return 1;
}

#if 0
static int cmd_remove(cmd_tbl_t *cmdtp_t, int flag_t, int argc_t, char * const argv_t[])
{
	block_dev_desc_t *ufs_dev;
	disk_partition_t dpt;

	if(cur_ufs_dev_num < 0)
	{
		puts("No UFS device available\n");
		return 1;
	}

	ufs_dev = ufs_get_dev(cur_ufs_dev_num);
	if ((ufs_dev == NULL) || ufs_dev->type == DEV_TYPE_UNKNOWN)
	{
		printf("no ufs device found!\n");
		return 1;
	}

	strcpy((char *)&dpt.name, (const char *)argv_t[2]);

	if (!strcmp((char *)&dpt.name,"MBOOT") || !strcmp((char*)&dpt.name,"MPOOL")) 
	{
		printf("\nCan't remove MBOOT or MPOOL partitions\n");
		return 1;
	}

	if (remove_ufs_partition(ufs_dev, &dpt) == 0){
		printf("Remove partition %s success!\n", dpt.name);
		return 0;
	}

	return 1;
}
#endif

static int cmd_rmgpt(cmd_tbl_t *cmdtp_t, int flag_t, int argc_t, char * const argv_t[])
{
	block_dev_desc_t *ufs_dev;

	if(cur_ufs_dev_num < 0)
	{
		puts("No UFS device available\n");
		return 1;
	}

	ufs_dev = ufs_get_dev(cur_ufs_dev_num);
	if ((ufs_dev == NULL) || ufs_dev->type == DEV_TYPE_UNKNOWN)
	{
		printf("no ufs device found!\n");
		return 1;
	}

	return rmgpt_ufs_partition(ufs_dev);
}

#ifdef CONFIG_LZO
static u32 unlzo_partition_blkcnt = 0;
static int do_unlzo (int argc, char * const argv[])
{
	int ret=0, cnt, cnt_total=0;
	unsigned char *AddrSrc=NULL, *AddrDst=NULL;
	size_t LengthSrc=0,  LengthDst=0;
	size_t LenTail=0, LenSpl=0, LenSrcAlign=0;
	disk_partition_t dpt;
	block_dev_desc_t *ufs_dev;
	s32 blk = -1, partnum = 1, n;
	u32 empty_skip = 0, partition_blkcnt = 0;
	char* cmdtail = strchr(argv[1], '.');

	AddrSrc = (unsigned char *) simple_strtol(argv[2], NULL, 16);
	LengthSrc = (size_t) simple_strtol(argv[3], NULL, 16);

	if(cur_ufs_dev_num < 0)
	{
		puts("No UFS device available\n");
		return 1;
	}

	ufs_dev = ufs_get_dev(cur_ufs_dev_num);
	if ((ufs_dev == NULL) || ufs_dev->type == DEV_TYPE_UNKNOWN)
	{
		printf("no ufs device found!\n");
		return 1;
	}

	//Get the partition offset from partition name
	for(;;)
	{
		if(get_partition_info_ufs(ufs_dev, partnum, &dpt))
			break;
		if(!strcmp(argv[4], (const char *)dpt.name))
		{
			blk = dpt.start;
			partition_blkcnt = dpt.size;
			break;
		}
		partnum++;
	}

	if(blk < 0)
	{
		printf("ERR:Please check the partition name!\n");
		return 1;
	}

	if ((!cmdtail) || (strncmp(cmdtail, ".cont", 5)))
		unlzo_partition_blkcnt = 0;

	if(unlzo_partition_blkcnt >= partition_blkcnt)
	{
		printf("ERR: The partition unlzo write has been done!\n");
		return 1;
	}

	blk += unlzo_partition_blkcnt;

	AddrDst = (unsigned char *) Get_lzo_addr_Info();

	printf ("   Uncompressing ... \n");

	ret = lzop_decompress_part ((const unsigned char *)AddrSrc, LengthSrc,
				(unsigned char *)AddrDst, &LengthDst, &LenSrcAlign, 0);

	if (ret) 
	{
		printf("LZO: uncompress, out-of-mem or overwrite error %d\n", ret);
		return 1;
	}

	if (argc == 6)
    {
		empty_skip = simple_strtoul(argv[5], NULL, 16);
	}

	/* We assume that the decompressed file is aligned to mmc block size
	when complete decompressing */
	cnt = LengthDst >> 12;

	n = ufs_dev->block_write(cur_ufs_dev_num, blk, cnt, AddrDst);

	if(n == cnt)
		cnt_total += cnt;
	else
	{
		printf("%d blocks written error at %d\n", cnt, blk);
		return 1;
    }

	/* If the decompressed file is not aligned to mmc block size, we should
	split the not aligned tail and write it in the next loop */
	LenTail = LengthDst & (4096 - 1);

	if(LenTail)
	{
		memcpy((unsigned char *) Get_lzo_addr_Info(),
				(const unsigned char *) (AddrDst + LengthDst - LenTail), LenTail);
		AddrDst = (unsigned char *) (Get_lzo_addr_Info() + LenTail);
	}
	else
		AddrDst = (unsigned char *) Get_lzo_addr_Info();

	if(LenSrcAlign == LengthSrc)
		goto done;

	//Move the source address to the right offset
	AddrSrc += LenSrcAlign;

	printf("    Continue uncompressing and writing ufs...\n");

	for(;;)
	{
		LengthDst = 0;
		ret = lzop_decompress_part ((const unsigned char *)AddrSrc, LengthSrc,
				(unsigned char *)AddrDst, &LengthDst, &LenSrcAlign, 1);
		if (ret) 
		{
			printf("LZO: uncompress, out-of-mem or overwrite error %d\n", ret);
			return 1;
		}

		LenSpl = LenTail + LengthDst;
		cnt = LenSpl >> 12;

		if(cnt)
		{
			n = ufs_dev->block_write(cur_ufs_dev_num, (blk+cnt_total), cnt, (const unsigned char *)CONFIG_UNLZO_DST_ADDR);

			if(n == cnt)
				cnt_total += cnt;
			else
			{
				printf("%d blocks written error at %d\n", cnt, (blk+cnt_total));
				return 1;
			}
		}

		LenTail = LenSpl & (4096 - 1);
		if(LenTail)
		{
			memcpy((unsigned char *) Get_lzo_addr_Info(),
					(const unsigned char *) (AddrDst + LengthDst - LenTail), LenTail);
			AddrDst = (unsigned char *) (Get_lzo_addr_Info() + LenTail);
		}
		else
			AddrDst = (unsigned char *) Get_lzo_addr_Info();

		if(LenSrcAlign == LengthSrc)
			break;

		AddrSrc += LenSrcAlign;
	}

done:

	if(LenTail)
	{
		if(1 != ufs_dev->block_write(cur_ufs_dev_num, (blk + cnt_total),
					1, (const unsigned char *)Get_lzo_addr_Info()))
		{
			printf("%d blocks written error at %d\n", cnt, blk);
			return 1;
		}
		cnt_total++;
	}

	unlzo_partition_blkcnt += cnt_total;

	printf("    Depressed OK! Write to %s partition OK!\nTotal write size: 0x%0x\n",
			argv[4], cnt_total << 12);

    return 0;
}
#endif

static int cmd_unlzo(cmd_tbl_t *cmdtp_t, int flag_t, int argc_t, char * const argv_t[])
{
#ifdef CONFIG_LZO
	int ret=0;

	if(cur_ufs_dev_num < 0)
	{
		puts("No UFS device available\n");
		return 1;
	}

	if (argc_t < 5) {
		printf ("Usage:\n%s\n", cmdtp_t->usage);
		return 1;
	}

	ret = do_unlzo(argc_t, argv_t);
	return ret;
#else
	UBOOT_ERROR("Do not enable CONFIG_LZO\n");
	printf("Please check the LZO config\n");
	return -1;
#endif
}

static int cmd_erase(cmd_tbl_t *cmdtp_t, int flag_t, int argc_t, char * const argv_t[])
{
{
	u32 n, cnt, size, partnum = 1, bootpart = -1;
	block_dev_desc_t *ufs_dev;
	disk_partition_t dpt;
	s32 blk = -1, temp;
	char* cmdtail = strchr(argv_t[1], '.');
	struct ufs_hba *hba;
	struct ufs_mstar_host *host;

	if(cur_ufs_dev_num < 0)
	{
		puts("No UFS device available\n");
		return 1;
	}

	hba = ufshcd_get_host();
	host = (struct ufs_mstar_host *)ufshcd_get_variant(hba);
	if(host == NULL)
	{
		printf("no ufs device at slot %x\n", cur_ufs_dev_num);
		return 1;
	}

	ufs_dev = ufs_get_dev(cur_ufs_dev_num);
	if ((ufs_dev == NULL) || ufs_dev->type == DEV_TYPE_UNKNOWN)
	{
		printf("no ufs device found!\n");
		return 1;
	}

	if(cmdtail == NULL) // erase
	{
		if(argc_t == 2)
		{
			blk = 0;
			cnt = host->qLogicalBlkCnt[cur_ufs_dev_num];
		}
		else
		{
			blk = simple_strtoul(argv_t[2], NULL, 16);
			size = simple_strtoul(argv_t[3], NULL, 16);
			cnt = ALIGN(size, 4096) / 4096;
			if(blk+cnt > host->qLogicalBlkCnt[0])
			{
				printf("out of range, write cnt %x > actual cnt %llx\n", cnt, host->qLogicalBlkCnt[0]);
				return 1;
			}
		}
		n = ufs_dev->block_erase(cur_ufs_dev_num, blk, cnt);
	}
	else if(!strcmp(cmdtail, ".boot")) // write.boot
	{
		if(argc_t == 4)
		{
			bootpart = 1;
			blk = simple_strtoul(argv_t[2], NULL, 16);
			size = simple_strtoul(argv_t[3], NULL, 16);
			cnt = ALIGN(size, 4096) / 4096;
		}
		else
		{
			bootpart = simple_strtoul(argv_t[2], NULL, 16);
			if(bootpart > 2)
			{
				printf("ufs access boot partition No. %d is invalid!\n", bootpart);
				return 1;
			}
			blk = simple_strtoul(argv_t[3], NULL, 16);
			size = simple_strtoul(argv_t[4], NULL, 16);
			cnt = ALIGN(size, 4096) / 4096;
		}

		if(blk+cnt > host->qLogicalBlkCnt[bootpart])
		{
			printf("out of range, start blk %x write cnt %x > actual cnt %llx\n", blk, cnt, host->qLogicalBlkCnt[bootpart]);
			return 1;
		}

		if(ufs_unmap(bootpart, blk, cnt) == 0)
			n = cnt;
		else
			n = 0;
	}
	else if(!strncmp(cmdtail, ".p", 2)) // write.p or write.p.continue
	{
		for(;;)
		{
			// search from 1st partition
			if(get_partition_info_ufs(ufs_dev, partnum, &dpt))
			{
                printf("no part:%s found!\n", argv_t[2]);
				return 1;
			}
			else
			{	
				if(!strcmp(argv_t[2], (const char *)dpt.name)) 
				{
					break;
				}
			}

			partnum++;
		}
		cnt = dpt.size;

		n = ufs_dev->block_erase(cur_ufs_dev_num, dpt.start, cnt);
	}
	else
		return cmd_usage(cmdtp_t);

	return (n == cnt) ? 0 : 1;
}
}

static int cmd_ipverify(cmd_tbl_t *cmdtp_t, int flag_t, int argc_t, char * const argv_t[])
{
	int ret=0;

	#if defined(CONFIG_MSTAR_UFS_IPVERIFY) && (CONFIG_MSTAR_UFS_IPVERIFY)
	ret = ufs_mstar_ipverify();
	#endif

	return ret;
}

struct ufs_ops ufs_option[] = {
	{"part",cmd_part},
	{"read",cmd_read},
	{"read.p",cmd_read},
	{"read.p.continue",cmd_read},
	{"read.boot",cmd_read},
	{"write",cmd_write},
	{"write.p",cmd_write},
	{"write.p.continue",cmd_write},
	{"write.boot",cmd_write},
	{"create",cmd_create},
	//{"rmgpt",cmd_remove},
	{"rmgpt",cmd_rmgpt},
	{"unlzo",cmd_unlzo},
	{"unlzo.cont",cmd_unlzo},
	{"erase",cmd_erase},
	{"erase.p",cmd_erase},
	{"erase.boot",cmd_erase},
	{"ipverify",cmd_ipverify}
};

int do_ufsops(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int i = 0;

	if (argc < 2)
	{
		return cmd_usage(cmdtp);
	}

	do
	{
		// printf("%s %s %s %s %s\n",argv[1],argv[2],argv[3],argv[4],argv[5]);

		if(strcmp(argv[1], ufs_option[i].name) == 0)
		{
			return (*(ufs_option[i].op))(cmdtp, flag, argc, argv);
		}
		i++;
	}while(i != sizeof(ufs_option)/sizeof(ufs_option[0]));

	return cmd_usage(cmdtp);

}

int get_ufs_partsize (char *puPartName, unsigned int *u32PartSize)
{
	block_dev_desc_t *dev_desc = ufs_get_dev(0);

	if (!dev_desc) 
	{
		printf("no ufs device at slot\n");
		return -1;
	}

	return _get_ufs_partsize (dev_desc, puPartName, u32PartSize);
}

int get_ufs_part_size_and_offset (char *puPartName, unsigned int *u32PartSize, unsigned int *u32PartOffset)
{
	disk_partition_t dpt;
	u32 partnum = 1;
	block_dev_desc_t *dev_desc = ufs_get_dev(0);

	if (!dev_desc) 
	{
		printf("no ufs device at slot\n");
		return -1;
	}

	for(;;)
	{
		if(get_partition_info_ufs(dev_desc, partnum, &dpt))
			break;
		if(!strcmp(puPartName, (const char *)dpt.name)){
			*u32PartOffset = dpt.start;
			*u32PartSize = dpt.size; //block number
			break;
		}
		partnum++;
	}
	return 0;
}

int do_ufstestops(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	char *cmd;
	int ret;
	int loop;
	int init;
	int testcase;
	u32 u32_buf,u32_BlkStartAddr;

#if defined(CONFIG_MSTAR_UFS_IPVERIFY) && (CONFIG_MSTAR_UFS_IPVERIFY)
	if(cur_ufs_dev_num < 0)
	{
		puts("No UFS device available\n");
		return 1;
	}

	cmd = argv[1];

	if (strcmp(cmd, "info") == 0) {
		ret = ufstest_info();
		return ret;
	}

	if (strcmp(cmd, "rw") == 0) {
		if(argc == 2)
			loop = 0;
		else
			loop = (int)simple_strtoul(argv[2], NULL, 10);

		ret = ufstest_rw10_lun(loop);
		return ret;
	}

	if (strcmp(cmd, "speed") == 0) {
		testcase = 1;
		if(argc == 3)
			testcase = (int)simple_strtoul(argv[2], NULL, 10);

		ret = ufstest_speed(testcase);
		return ret;
	}

	if (strcmp(cmd, "pwr_cut") == 0) {
		if(argc != 5)
        {
            printf("too few argument!\n");
            return 1;
        }

		if(strcmp(argv[2], "init") == 0) {
			u32_buf = simple_strtoul(argv[3], NULL, 16);
			u32_BlkStartAddr = simple_strtoul(argv[4], NULL, 16);

			if(ufstest_PwrCut_InitData((u8*)u32_buf,u32_BlkStartAddr) == 0)
				printf("UFS Info: UFS Power Cut Init OK\n");
			else
			{
				printf("UFS Err: UFS Power Cut Init Fail\n");
				return 1;
			}
		}
		else if(strcmp(argv[2], "test") == 0) {
			u32_buf = simple_strtoul(argv[3], NULL, 16);
			u32_BlkStartAddr = simple_strtoul(argv[4], NULL, 16);

			if(ufstest_PwrCut_Test((u8*)u32_buf,u32_BlkStartAddr) == 0)
				printf("UFS Info: UFS Power Cut Test OK\n");
			else
			{
				printf("UFS Err: UFS Power Cut Test Fail\n");
				return 1;
			}
		}

		return 0;
	}

	if (strcmp(cmd, "hibern") == 0) {
		if(argc == 2)
			ret = ufstest_hibern8(0);
		else if(strcmp(argv[2], "enter") == 0)
			ret = ufstest_hibern8(UIC_CMD_DME_HIBER_ENTER);
		else if(strcmp(argv[2], "exit") == 0)
			ret = ufstest_hibern8(UIC_CMD_DME_HIBER_EXIT);
		else
			return 1;

		return ret;
	}
#else
	puts("UFS IP verification doesn't enable\n");
	return 1;
#endif

	return cmd_usage(cmdtp);
}

