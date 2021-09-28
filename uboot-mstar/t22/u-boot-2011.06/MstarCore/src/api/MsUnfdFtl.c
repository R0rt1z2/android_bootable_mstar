/*
 * (C) Copyright 2003
 * Kyle Harris, kharris@nexus-tech.net
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <config.h>
#include <common.h>
#include <command.h>
#include <malloc.h>
#include <watchdog.h>
#include <fat.h>
#include <drvWDT.h>
#include <MsMath.h>
#include <u-boot/crc.h>
#include <jffs2/load_kernel.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/partitions.h>

#include <MsMLC.h>
#include "../drivers/mstar/unfd/inc/api/drv_ftl.h"
//#include <drvMLC.h>
#include "../drivers/mstar/unfd/inc/common/drvNAND.h"
extern int nand_unfd_init(void);

#ifdef CONFIG_LZO
#include <linux/lzo.h>
#endif
#include "../disk/part_emmc.h"
#define TEST_PATTERN1 0xab
#define TEST_PATTERN2 0xcd
#define MAX_RLIABLE_TRASACTION (16*0x100000/512)

unsigned char tmp_buf[0x800];
static int curr_device = -1;
extern unsigned short g_emmc_reserved_for_map;
extern U32 drvNAND_BlkDev_Init(void);


int  ftl_get_partition_info_by_partnum(
    block_dev_desc_t *dev, u32 *partnum, disk_partition_t *dpt, 
    s32 *blk, u32 *size, u32 *cnt, u32 *cont, 
    const char *param_name, char *param_blk, char *param_size, char *cmdlasttail, char *type)
{
	for(;;)
	{
		if(get_partition_info_emmc(dev, *partnum, dpt))
		{
			//find no partition
			break;
		}
		else
		{	
			if(!strcmp(param_name, (const char *)dpt->name)) 
			{
				*blk = dpt->start;
				if(!strncmp(cmdlasttail, ".continue", 9))
				{
					*blk += simple_strtoul(param_blk, NULL, 16);
					*size = simple_strtoul(param_size, NULL, 16);

					if(!strcmp("read",type))
					{
						*cnt = ALIGN(*size, 512) / 512;
					}

					if(!strcmp("write",type))
					{
						 *cont = 1;
					}
				}
				break;
			}
		}

		*partnum=*partnum+1;
		
		if(!strcmp("write",type))
		{
			#if ENABLE_TVCONFIG_PARTITION
			if (0 == strcmp(param_name, "tvdatabase") || 0 == strcmp(param_name, "tvconfig"))
			{
				setenv("upgrade_factorydb", "TRUE");
				saveenv();
			}
			#endif
		}
	}

	return 0;
}



// =======================================================================
// MLC test cmd
// =======================================================================
int do_MsUnfdFtl(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	if (argc < 2)
		return cmd_usage(cmdtp);

	if (curr_device < 0) {
		if (get_ftl_num() > 0)
			curr_device = 0;
		else {
			//puts("No FTL device available\n");
			printf("Initial Mstar unfd ftl\n");
			if(drvNAND_BlkDev_Init())
				return 1;
			if (get_ftl_num() > 0)
				curr_device = 0;
		}
	}

	//============================
	if (strcmp(argv[1], "info") == 0)
	{

		return 0;
	}
	else if (strcmp(argv[1], "init") == 0)
	{
		/*	@FIXME
		struct mtd_device *dev;
		struct part_info *part;
		u8 pnum;

		if (argc < 3)
			return cmd_usage(cmdtp);


		if(drvNAND_ChkRdy() == 1)
		{
			printf("Mstar MLC FTL has initialized\n");
			return 0;
		}

		if (find_dev_and_part(argv[2], &dev, &pnum, &part)) {
			printf("Partition %s not found!\n", argv[2]);
			return 1;
		}
		drvNAND_MstarFTL_Init(part.offset, part.size);
		*/
	
		return 0;
	}
	else if(strncmp(argv[1], "read" ,4) == 0)
	{
		void *addr = (void *)simple_strtoul(argv[2], NULL, 16); // data buf addr
		u32 n, n2, cnt, size, tail = 0, partnum = 1,flag=0,cont=0;
		block_dev_desc_t *dev;
		disk_partition_t dpt;
		s32 blk = -1;
		char* cmdtail = strchr(argv[1], '.');
		char* cmdlasttail = strrchr(argv[1], '.');
		struct ftl *ftl = find_ftl_device(curr_device);

		if (!ftl) {
			printf("no nand ftl device at slot %x\n", curr_device);
			return 1;
		}
		
		size = simple_strtoul(argv[4], NULL, 16);
		cnt = ALIGN(size, 512) / 512;
		if((cmdtail)&&(!strncmp(cmdtail, ".p", 2)))
	    {
			//partition_flag=1;
	        dev = ftl_get_dev(curr_device);
			if ((dev == NULL) || (dev->type == DEV_TYPE_UNKNOWN)) 
			{
				printf("no blk device found!\n");
				return 1;
			}

			 flag = ftl_get_partition_info_by_partnum(dev, &partnum, &dpt, 
                &blk, &size, &cnt, &cont, 
                argv[3], argv[4], argv[5], cmdlasttail, "read");

			 if(flag==1)
			 {
				return 1;
			 }		
		}
		else
			blk = simple_strtoul(argv[3], NULL, 16);

		if(blk < 0){
			printf("[%s.%u] ERR: Please check the blk# or partiton name!\n", __func__,__LINE__);
			return 1;
		}
		/* unaligned size is allowed */
		if ((cnt << 9) > size)
		{
			cnt--;
			tail = size - (cnt << 9);
		}

		if (cnt > 0)
		{
			n = ftl->block_dev.block_read(0, blk, cnt, addr);
		}
		else if (cnt == 0)
		{
			n = 0;
		}

		if (tail)
		{
			n2 = ftl->block_dev.block_read(0, (blk + cnt), 1, tmp_buf);
			n2 = (n2 == 1) ? 1 : -1;
			memcpy(((unsigned char *)addr + (cnt << 9)), tmp_buf, tail);
			n += n2;
			cnt++;
		}
		return (n == cnt) ? 0 : 1;
	}
	//===========================================
	else if(strncmp(argv[1], "write" ,5) == 0)
	{
		void *addr = (void *)simple_strtoul(argv[2], NULL, 16);
		u32 n=0, cnt, size, partnum = 1,flag=0,cont=0, partition_flag = 0;
		block_dev_desc_t *dev;
		disk_partition_t dpt;
		s32 blk = -1;
		char* cmdtail = strchr(argv[1], '.');
		char* cmdlasttail = strrchr(argv[1], '.');
		struct ftl *ftl = find_ftl_device(curr_device);

		if (!ftl) {
			printf("no nand ftl device at slot %x\n", curr_device);
			return 1;
		}
		
		size = simple_strtoul(argv[4], NULL, 16);
		cnt = ALIGN(size, 512) / 512;
		if((cmdtail)&&(!strncmp(cmdtail, ".p", 2)))
		{
		    //partition_flag=1;
			dev = ftl_get_dev(curr_device);
			if ((dev == NULL) || (dev->type == DEV_TYPE_UNKNOWN)) 
			{
			         printf("no ftl device found!\n");
			         return 1;
			}
		
			flag = ftl_get_partition_info_by_partnum(dev, &partnum, &dpt, 
                &blk, &size, &cnt, &cont, 
                argv[3], argv[4], argv[5], cmdlasttail, "write");
			
			if(flag==1)
			{
				return 1;
			} 
		}
		else
			blk = simple_strtoul(argv[3], NULL, 16);

		if(blk < 0){
			printf("[%s.%u] ERR: Please check the blk# or partiton name!\n", __func__,__LINE__);
			return 1;
		}

        if(((blk+cnt)>(dpt.start+dpt.size)) && (partition_flag == 1))
        {
			printf("\n\n[%s.%u]***** ERROR: the write data size (%d-%d) bigger than the partition %s size (%ld-%ld)!*****\n\n",
                __func__,__LINE__, blk,blk+cnt,dpt.name,dpt.start,dpt.start+dpt.size);
			return 1;  
		}
		if (cnt > 0)
		{
			n = ftl->block_dev.block_write(0, blk, cnt, addr);
		}
		return (n == cnt) ? 0 : 1;

	}
	//===========================================
	else if(strncmp(argv[1], "create", 6) == 0)
	{
		block_dev_desc_t *dev;
        disk_partition_t dpt;
        struct ftl *ftl = find_ftl_device(curr_device);
		if (!ftl) {
		    printf("no ftl device at slot %x\n", curr_device);
		    return 1;
		}

		dev = ftl_get_dev(curr_device);
		if ((dev == NULL) ||
			(dev->type == DEV_TYPE_UNKNOWN)) {
			printf("no ftl device found!\n");
			return 1;
		}

		strcpy((char *)&dpt.name, argv[2]);

		if(simple_strtoull(argv[3], NULL, 16)%512!=0)
        {
			printf("\n\n\n[%s.%u]******** ERROR: partition (%s) size must be 512-byte aligned! ******** \n\n\n",
                __func__,__LINE__, argv[2]);
        }

		dpt.size = ALIGN(simple_strtoull(argv[3], NULL, 16), 512) / 512;

		if(argc > 4)
			dpt.start = ALIGN(simple_strtoull(argv[4], NULL, 16), 512) / 512;
		else
			dpt.start = 0;

		if (add_new_emmc_partition(dev, &dpt) == 0){
			printf("Add new partition %s success!\n", dpt.name);
			return 0;
		}
			
		return 1;
	}
	//===========================================
	else if(strncmp(argv[1], "rmgpt", 5) == 0)
	{
		struct ftl *ftl = find_ftl_device(curr_device);
		unsigned long *reset;
		int i;
		block_dev_desc_t *dev;

		if (!ftl) {
			printf("no ftl device at slot %x\n", curr_device);
			return 1;
		}
		dev = ftl_get_dev(curr_device);
		if ( (dev == NULL) || (dev->type == DEV_TYPE_UNKNOWN) )
		{
			printf("no ftl device found!\n");
			return 1;
		}

		reset = (unsigned long *)malloc(512);

		if(reset == NULL)
		{
			printf("malloc fail!\n");
			return 1;
		}

		memset(reset, 0, 512);

		for(i=1;i<= g_emmc_reserved_for_map;i++)
		{
			ftl->block_dev.block_write(curr_device, i, 1, reset);
		}

		free(reset);
		return 0;
	}
	else if(strncmp(argv[1], "part", 4) == 0)
	{
		block_dev_desc_t *dev;
		struct ftl *ftl = find_ftl_device(curr_device);

		if (!ftl) {
			printf("no Mstar FTL device at slot %x\n", curr_device);
			return 1;
		}

		dev = ftl_get_dev(curr_device);
		if (dev != NULL && dev->type != DEV_TYPE_UNKNOWN) {
			print_part(dev);
			return 0;
		}
	}
	//============================
    #if 1// defined(IF_IP_VERIFY) && IF_IP_VERIFY

	else if (strcmp(argv[1], "test") == 0)
	{
		u32 u32_LoopCnt, u32_i;

		u32_LoopCnt = simple_strtoul(argv[2], NULL, 16);
		
		for(u32_i=0; u32_i<u32_LoopCnt; u32_i++)
		{
			printf("Test: %u \n", u32_i);
			//if(MLC_ST_SUCCESS != u32_err)
		//		break;
		}

		return 0;
	}
//	/*
	else if(strcmp(argv[1],"reliable_test") == 0)
	{
		block_dev_desc_t *dev;
        struct ftl *ftl = find_ftl_device(curr_device);
		int n, i;
		U32 idx, startblk =0, previous_startblk = 0x999;
		const char *sign = "MSTARSEMI";
		unsigned char * src = 0x80400000;
		char* reliable_test_char;
		int reliable_test_value;
		int reset = 0;
		char buf[4];


        if (!ftl) {
            printf("no Mstar FTL device at slot %x\n", curr_device);
            return 1;
        }

		if(argc == 3)
			reset = 1;

		if(reset)
		{	
			sprintf(buf, "%d", 1);
			setenv("unfdftl_reliabletest", buf);
			saveenv();
		}

	    reliable_test_char = getenv("unfdftl_reliabletest");
	    if(reliable_test_char == NULL)
	    {
			printf("no unfdftl_reliabletest env\n");
	        return 0;
    	}
    	reliable_test_value = (int)simple_strtol(reliable_test_char, NULL, 10);
    	if(reliable_test_value == 0)
    	{
			printf("reliable test value is 0\n");
        	return 0;
	    }		

		//test from offset 0x40 block
		
		//check block 0x40

		n = ftl->block_dev.block_read(curr_device, 0x40, 1, tmp_buf);
		
		for(i = 0; i < 9; i ++)
		{
			if(tmp_buf[i] != sign[i])
				break;
		}
		if(i== 9 && reset == 0)
		{
			n = ftl->block_dev.block_read(curr_device, 0x41, 1, tmp_buf);
			memcpy(&previous_startblk, tmp_buf, sizeof(U32));
			printf("check last block address beginning with 0x%X\n", previous_startblk);
			
			for(idx = 0x42; idx < MAX_RLIABLE_TRASACTION*2/*ftl->block_dev.lba*/; idx++)
			{
				n=ftl->block_dev.block_read(curr_device, idx, 1, tmp_buf);
				
				for(i = 0 ; i < 0x200; i ++)
				{
					if(idx < previous_startblk || idx > previous_startblk + MAX_RLIABLE_TRASACTION)
					{	
						if(tmp_buf[i] != TEST_PATTERN1)
						{
							printf("damage other block @ 0x%lX idx 0x%x, value 0x%X\n", idx, i, tmp_buf[i]);
							sprintf(buf, "%d", 0);
							setenv("unfdftl_reliabletest", buf);
							saveenv();
							return 1;
						}
					}
				}
			}
		}

		/*
		memset(tmp_buf, TEST_PATTERN1, 0x800);
		for(idx = 0x42; idx < ftl->block_dev.lba; idx += 4)
		{
			int length = (ftl->block_dev.lba - idx > 4) ? 4: (ftl->block_dev.lba - idx);
			n = ftl->block_dev.block_write(curr_device, idx, length, tmp_buf);
			if(n == 0)
				return 1;
		}
		*/

		memset(src, TEST_PATTERN1, MAX_RLIABLE_TRASACTION*2*512);

		n = ftl->block_dev.block_write(curr_device, 0x42, MAX_RLIABLE_TRASACTION*2, src);
		if(n == 0)
			return 1;
		
		startblk = rRand(previous_startblk, previous_startblk+333);
		if(startblk == previous_startblk)
			startblk += 763;
		
		if(startblk + MAX_RLIABLE_TRASACTION > (0x42 + MAX_RLIABLE_TRASACTION*2))
		{
			startblk = (startblk + MAX_RLIABLE_TRASACTION) % (0x42 + MAX_RLIABLE_TRASACTION* 2);
		}
		if(startblk < 0x42)
			startblk += 0x42;
		memset(tmp_buf, 0, 0x200);
		memcpy(tmp_buf, &startblk, sizeof(U32));
		n = ftl->block_dev.block_write(curr_device, 0x41, 1, tmp_buf);
		
		memset(tmp_buf, 0, 0x200);
		memcpy(tmp_buf, sign, 0x9);
		n = ftl->block_dev.block_write(curr_device, 0x40, 1, tmp_buf);

		printf("beginning reliable write test for power cut @ 0x%X\n", startblk);
		
		while(1)
		{
			for(idx = startblk; idx < startblk + MAX_RLIABLE_TRASACTION; idx ++)
			{
				n = ftl->block_dev.block_write(curr_device, idx, 1, tmp_buf);
				if(n == 0)
				{
					printf("write block fail @ 0x%X\n", idx);
					sprintf(buf, "%d", 0);
					setenv("unfdftl_reliabletest", buf);
					saveenv();
					return 0;
				}
			}
		}

	}


//	*/
    #endif
	
	//============================
	

	return cmd_usage(cmdtp);
}


