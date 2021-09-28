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
#include <environment.h>
#include <search.h>
#include <MsFtl.h>
#include "../drivers/mstar/unfd/ftl/ftl.h"

extern int nand_unfd_init(void);

#ifdef CONFIG_LZO
#include <linux/lzo.h>
#endif
#include "../disk/part_emmc.h"

#ifdef CONFIG_ENABLE_WIDESTORAGE
#include <MsDisk.h>
#endif

//----------------------------------
UNFD_ALIGN0 static unsigned char tmp_buf[0x200] UNFD_ALIGN1; // used inside this file
static int curr_device = -1;


// =======================================================================
// partitions
// =======================================================================
extern unsigned short g_emmc_reserved_for_map;

int  ftl_get_partition_info_by_partnum(
    block_dev_desc_t *dev, u32 *partnum, disk_partition_t *dpt, 
    s32 *SecAddr, u32 *size, u32 *SecCnt, u32 *cont, 
    const char *part_Name, char *part_SecAddr, char *part_SecCnt, char *cmdlasttail, char *type)
{
	for(;;)
	{
        // search from 1st partition
		if(get_partition_info_emmc(dev, *partnum, dpt))
		{
			//find no partition
			break;
		}
		else
		{	
			if(!strcmp(part_Name, (const char *)dpt->name)) 
			{
				*SecAddr = dpt->start;
                
				if(!strncmp(cmdlasttail, ".continue", 9))
				{
					*SecAddr += simple_strtoul(part_SecAddr, NULL, 16);
					*SecCnt = simple_strtoul(part_SecCnt, NULL, 16);

                    #if 0
					if(!strcmp("read",type))
					{
						*SecCnt = ALIGN(*size, 512) / 512;
					}

					if(!strcmp("write",type))
					{
						 *cont = 1;
					}
                    #endif
				}
				return 0; // ok
			}
		}

		*partnum=*partnum+1;

        // ------------------------------
		if(!strcmp("write",type))
		{
			#if ENABLE_TVCONFIG_PARTITION
			if (0 == strcmp(part_Name, "tvdatabase") || 0 == strcmp(part_Name, "tvconfig"))
			{
				setenv("upgrade_factorydb", "TRUE");
				saveenv();
			}
			#endif
		}
	}

	return -1; // fail
}

#ifdef CONFIG_LZO
static u32 unlzo_partition_blkcnt = 0;


static int do_ftl_unlzo (struct ftl * ftl_blkdev, int argc, char * const argv[])
{
    int ret=0, cnt, cnt_total=0;
    unsigned char *AddrSrc=NULL, *AddrDst=NULL;
    size_t LengthSrc=0,  LengthDst=0;
    size_t LenTail=0, LenSpl=0, LenSrcAlign=0;
    disk_partition_t dpt;
    block_dev_desc_t *dev;
    s32 blk = -1, partnum = 1, n;
    u32 empty_skip = 0, partition_blkcnt = 0;
    char* cmdtail = strchr(argv[1], '.');

    AddrSrc = (unsigned char *) simple_strtol(argv[2], NULL, 16);
    LengthSrc = (size_t) simple_strtol(argv[3], NULL, 16);

    dev = ftl_get_dev(curr_device);
    if ((dev == NULL) ||
            (dev->type == DEV_TYPE_UNKNOWN)) {
        printf("no ftl device found!\n");
        return 1;
    }

    //Get the partition offset from partition name
    for(;;){

        if(get_partition_info_emmc(dev, partnum, &dpt))
            break;
        if(!strcmp(argv[4], (const char *)dpt.name)){
            blk = dpt.start;
            partition_blkcnt = dpt.size;
            break;
        }
        partnum++;
    }

    if(blk < 0){
        printf("ERR:Please check the partition name!\n");
        return 1;
    }

    if ((!cmdtail) || (strncmp(cmdtail, ".cont", 5)))
        unlzo_partition_blkcnt = 0;

    if(unlzo_partition_blkcnt >= partition_blkcnt){
        printf("ERR: The partition unlzo write has been done!\n");
        return 1;
    }

    blk += unlzo_partition_blkcnt;

    AddrDst = (unsigned char *) CONFIG_UNLZO_DST_ADDR;

    printf ("   Uncompressing ... \n");

    ret = lzop_decompress_part ((const unsigned char *)AddrSrc, LengthSrc,
                (unsigned char *)AddrDst, &LengthDst, &LenSrcAlign, 0);


    if (ret) {
        printf("LZO: uncompress, out-of-mem or overwrite error %d\n", ret);
        return 1;
    }

    if (argc == 6)
    {
        empty_skip = simple_strtoul(argv[5], NULL, 16);
    }

    /* We assume that the decompressed file is aligned to 512 byte
        when complete decompressing */
    cnt = LengthDst >> 9;

    n = ftl_blkdev->block_dev.block_write(curr_device, blk, cnt, AddrDst);

    if(n == cnt)
        cnt_total += cnt;
    else{
        printf("%d blocks written error at %d\n", cnt, blk);
        return 1;
    }

    /* If the decompressed file is not aligned to 512byte, we should
        split the not aligned tail and write it in the next loop */
    LenTail = LengthDst & (512 - 1);

    if(LenTail){
        memcpy((unsigned char *) CONFIG_UNLZO_DST_ADDR,
                    (const unsigned char *) (AddrDst + LengthDst - LenTail), LenTail);
        AddrDst = (unsigned char *) (CONFIG_UNLZO_DST_ADDR + LenTail);
    }else
        AddrDst = (unsigned char *) CONFIG_UNLZO_DST_ADDR;

    if(LenSrcAlign == LengthSrc)
        goto done;

    //Move the source address to the right offset
    AddrSrc += LenSrcAlign;

    printf("    Continue uncompressing and writing ...\n");

    for(;;){

        LengthDst = 0;
        ret = lzop_decompress_part ((const unsigned char *)AddrSrc, LengthSrc,
                (unsigned char *)AddrDst, &LengthDst, &LenSrcAlign, 1);
        if (ret) {
            printf("LZO: uncompress, out-of-mem or overwrite error %d\n", ret);
            return 1;
        }

        LenSpl = LenTail + LengthDst;
        cnt = LenSpl >> 9;

        if(cnt){
            n = ftl_blkdev->block_dev.block_write(curr_device, (blk+cnt_total), cnt, (const unsigned char *)CONFIG_UNLZO_DST_ADDR);

            if(n == cnt)
                cnt_total += cnt;
            else{
                printf("%d blocks written error at %d\n", cnt, (blk+cnt_total));
                return 1;
            }
        }

        LenTail = LenSpl & (512 - 1);
        if(LenTail){
            memcpy((unsigned char *) CONFIG_UNLZO_DST_ADDR,
                        (const unsigned char *) (AddrDst + LengthDst - LenTail), LenTail);
            AddrDst = (unsigned char *) (CONFIG_UNLZO_DST_ADDR + LenTail);
        }else
            AddrDst = (unsigned char *) CONFIG_UNLZO_DST_ADDR;

        if(LenSrcAlign == LengthSrc)
            break;

        AddrSrc += LenSrcAlign;
    }

done:


    if(LenTail){
        if(1 != ftl_blkdev->block_dev.block_write(0, (blk + cnt_total),
                    1, (const unsigned char *)CONFIG_UNLZO_DST_ADDR))
        {
            printf("%d blocks written error at %d\n", cnt, blk);
            return 1;
        }
        cnt_total++;
    }

    unlzo_partition_blkcnt += cnt_total;

    printf("    Depressed OK! Write to %s partition OK!\nTotal write size: 0x%0x\n",
            argv[4], cnt_total << 9);

    return 0;
}
#endif


int do_ftl(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    #ifdef CONFIG_ENABLE_WIDESTORAGE
    if(MSDISK_eMMC == ChkDiskType())
    {
        printf("%s: eMMC disk\n", __func__);
        return -1;
    }
    #endif

    //-------------------------------
	if (argc < 2)
		return cmd_usage(cmdtp);

	if (curr_device < 0) {
		if (get_ftl_num() > 0)
			curr_device = 0; // support only 1 FTL device (more is nonsense)
		else {
            //printf("initial Mstar FTL\n");
            if(ftl_BlkDev_Init())
            {
                printf("FTL initialization fails\n");
                return 1;
            }
          if (get_ftl_num() > 0)
              curr_device = 0;
		}
	}
	if(strncmp(argv[1], "init",4) == 0)	//force reinit
	{
		struct ftl *pFtl;

		NFTL_ResetReadyFlag();
		//printf("initial Mstar FTL\n");
		if(ftl_BlkDev_Init())
		{
		    printf("FTL initialization fails\n");
		    return 1;
		}
        if (get_ftl_num() > 0)
            curr_device = 0;

		pFtl = find_ftl_device(curr_device);

		_erase_sync(&(pFtl->block_dev), 0, pFtl->block_dev.lba);
		return 0;
	}
    
	//===========================================
	if(strncmp(argv[1], "read" ,4) == 0)
	{
        void *addr = (void *)simple_strtoul(argv[2], NULL, 16); // data buf addr
		u32 n, n2, SecCnt, size, tail = 0, partnum = 1,cont=0, partition_flag=0;
		block_dev_desc_t *dev;
		disk_partition_t dpt;
		s32 SecAddr = -1;
		char* cmdtail = strchr(argv[1], '.');
		char* cmdlasttail = strrchr(argv[1], '.');
		struct ftl *pFtl = find_ftl_device(curr_device);

		if (!pFtl) {
			printf("no NAND FTL device at slot %x\n", curr_device);
			return 1;
		}		
		// ------------------------------------------------------------
        // 1 sector = 512 bytes
        // ftl read            DRAM SecAddr  DataByteCnt 
        // ftl read.p          DRAM PartName DataByteCnt  // read from the partition's beginning
        // ftl read.p.continue DRAM PartName PartSecOffset SecCnt 
        // ------------------------------------------------------------
		size = simple_strtoul(argv[4], NULL, 16);
		SecCnt = ALIGN(size, 512) / 512;

        if((cmdtail)&&(!strncmp(cmdtail, ".p", 2)))
	    {
			//partition_flag=1;
	        dev = ftl_get_dev(curr_device);
			if ((dev == NULL) || (dev->type == DEV_TYPE_UNKNOWN)) 
			{
				printf("no blk device found!\n");
				return 1;
			}

			 if(ftl_get_partition_info_by_partnum(dev, &partnum, &dpt, 
                &SecAddr, &size, &SecCnt, &cont, 
                argv[3], argv[4], argv[5], cmdlasttail, "read"))
			 {
                printf("no part:%s found!\n", argv[3]);
				return 1;
			 }
             partition_flag = 1;
		}
		else
			SecAddr = simple_strtoul(argv[3], NULL, 16);


        // --------------------------------
        if((partition_flag == 1) && ((SecAddr+SecCnt)>(dpt.start+dpt.size)))
        {
			printf("\n\n[%s.%u]***** ERROR: the read data size (%d-%d) bigger than the partition %s size (%lld-%lld)!*****\n\n",
                __func__,__LINE__, SecAddr,SecAddr+SecCnt,dpt.name,dpt.start,dpt.start+dpt.size);
			return 1;  
		}
		if(SecAddr < 0){
			printf("[%s.%u] ERR: Please check the blk# or partiton name!\n", __func__,__LINE__);
			return 1;
		}
		/* unaligned size is allowed */
		if ((SecCnt << 9) > size)
		{
			SecCnt--;
			tail = size - (SecCnt << 9);
		}

		if (SecCnt > 0)
			n = pFtl->block_dev.block_read(0, SecAddr, SecCnt, addr);
		else if (SecCnt == 0)
			n = 0;
		
		if (tail)
		{
			n2 = pFtl->block_dev.block_read(0, (SecAddr + SecCnt), 1, tmp_buf);
			n2 = (n2 == 1) ? 1 : -1;
			memcpy(((unsigned char *)addr + (SecCnt << 9)), tmp_buf, tail);
			n += n2;
			SecCnt++;
		}
		return (n == SecCnt) ? 0 : 1;
	}
	//===========================================
	else if(strncmp(argv[1], "write" ,5) == 0)
	{
		void *addr = (void *)simple_strtoul(argv[2], NULL, 16);
		u32 n=0, SecCnt, size, partnum = 1,cont=0, partition_flag = 0;
		block_dev_desc_t *dev;
		disk_partition_t dpt;
		s32 SecAddr = -1;
		char* cmdtail = strchr(argv[1], '.');
		char* cmdlasttail = strrchr(argv[1], '.');
		struct ftl *pFtl = find_ftl_device(curr_device);

		if (!pFtl) {
			printf("no NAND FTL device at slot %x\n", curr_device);
			return 1;
		}		
		// ------------------------------------------------------------
        // 1 sector = 512 bytes
        // ftl write            DRAM SecAddr  DataByteCnt 
        // ftl write.p          DRAM PartName DataByteCnt  // write from the partition's beginning
        // ftl write.p.continue DRAM PartName PartSecOffset SecCnt 
        // ------------------------------------------------------------
		size = simple_strtoul(argv[4], NULL, 16);
		SecCnt = ALIGN(size, 512) / 512;
		if((cmdtail)&&(!strncmp(cmdtail, ".p", 2)))
		{
		    dev = ftl_get_dev(curr_device);
			if ((dev == NULL) || (dev->type == DEV_TYPE_UNKNOWN)) 
			{
			     printf("no FTL device found!\n");
			     return 1;
			}
		
			if(ftl_get_partition_info_by_partnum(dev, &partnum, &dpt, 
                &SecAddr, &size, &SecCnt, &cont, 
                argv[3], argv[4], argv[5], cmdlasttail, "write"))
            {
                printf("no part:%s found!\n", argv[3]);
				return 1;
			 }
            partition_flag = 1;
		}
		else
			SecAddr = simple_strtoul(argv[3], NULL, 16);

		if(SecAddr < 0){
			printf("[%s.%u] ERR: Please check the blk# or partiton name!\n", __func__,__LINE__);
			return 1;
		}

        if((partition_flag == 1) && ((SecAddr+SecCnt)>(dpt.start+dpt.size)))
        {
			printf("\n\n[%s.%u]***** ERROR: the write data size (%d-%d) bigger than the partition %s size (%lld-%lld)!*****\n\n",
                __func__,__LINE__, SecAddr,SecAddr+SecCnt,dpt.name,dpt.start,dpt.start+dpt.size);
			return 1;  
		}
		if (SecCnt > 0)
		{
			n = pFtl->block_dev.block_write(0, SecAddr, SecCnt, addr);
		}
		return (n == SecCnt) ? 0 : 1;

	}
	//===========================================
	else if(strncmp(argv[1], "create", 6) == 0)
	{
		block_dev_desc_t *dev;
        disk_partition_t dpt;
        struct ftl *pFtl = find_ftl_device(curr_device);
		if (!pFtl) {
		    printf("no FTL device at slot %x\n", curr_device);
		    return 1;
		}

		dev = ftl_get_dev(curr_device);
		if ((dev == NULL) ||
			(dev->type == DEV_TYPE_UNKNOWN)) {
			printf("no FTL device found!\n");
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
		struct ftl *pFtl = find_ftl_device(curr_device);
		unsigned long *reset;
		int i;
		block_dev_desc_t *dev;

		if (!pFtl) {
			printf("no FTL device at slot %x\n", curr_device);
			return 1;
		}
		dev = ftl_get_dev(curr_device);
		if ( (dev == NULL) || (dev->type == DEV_TYPE_UNKNOWN) )
		{
			printf("no FTL device found!\n");
			return 1;
		}

		reset = (unsigned long *)malloc(512);
		if(reset == NULL)
		{
			printf("malloc fail!\n");
			return 1;
		}
		memset(reset, 0, 512);

		for(i=1; i<= g_emmc_reserved_for_map; i++)
		{
			pFtl->block_dev.block_write(curr_device, i, 1, reset);
		}

		_erase_sync(&(pFtl->block_dev), 0, pFtl->block_dev.lba);

		free(reset);
		return 0;
	}
	//===========================================
	else if(strncmp(argv[1], "part", 4) == 0)
	{
		block_dev_desc_t *dev;
		struct ftl *pFtl = find_ftl_device(curr_device);

		if (!pFtl) {
			printf("no Mstar FTL device at slot %x\n", curr_device);
			return 1;
		}

		dev = ftl_get_dev(curr_device);
		if (dev != NULL && dev->type != DEV_TYPE_UNKNOWN) {
			print_part(dev);
			return 0;
		}

        return 0;
	}
    else if(strncmp(argv[1], "unlzo", 5) == 0) {
    
#ifdef CONFIG_LZO
                struct ftl *pFtl = find_ftl_device(curr_device);
                int ret=0;
    
                if (!pFtl) {
                    printf("no Mstar FTL device at slot %x\n", curr_device);
                    return 1;
                }   
    
                if (argc < 5) {
                    printf ("Usage:\n%s\n", cmdtp->usage);
                    return 1;
                }
    
                ret = do_ftl_unlzo(pFtl, argc, argv);
                return ret;
#else
                printf("Please check the LZO config\n");
                return 0;
#endif
    }        
	return cmd_usage(cmdtp);
}


// =======================================================================
// FTL / NAND test cmd
// =======================================================================
int do_ftltest(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    #ifdef CONFIG_ENABLE_WIDESTORAGE
    if(MSDISK_eMMC == ChkDiskType())
    {
        printf("%s: eMMC disk\n", __func__);
        return -1;
    }
    #endif 

    //-------------------------------
	if (argc < 2)
		return cmd_usage(cmdtp);
    
	//===========================================
	if (strcmp(argv[1], "dbglog") == 0)
	{
        if(simple_strtoul(argv[2], NULL, 16))
        {
            FTL_Disable_ErrLog = 0;
            ftl_dbg(0,0,"enable dbg log \n");
        }
        else
        {   ftl_dbg(0,0,"disable dbg log \n");
            FTL_Disable_ErrLog = 1;        
        }
        return 0;
	}
	else if (strcmp(argv[1], "info") == 0)
	{
        if (argc == 2)
        {
            NFTL_DumpInfo();
        }
        else if(strcmp(argv[2],"l2p") == 0)
        {
            NFTL_DumpRowL2P();
            }
        else if(strcmp(argv[2],"blk") == 0)
        {
            NFTL_DumpBlkSt();
        }
            
        return 0;        
	}
    //===========================================
	else if (strcmp(argv[1], "init")==0)
	{
        u32 u32_LoopCnt, u32_i, u32_t0, u32_t1;
        U16 OriDisableErrLog = FTL_Disable_ErrLog;

        if (argc == 2)
        {
            FTL_Disable_ErrLog=0; 
            NFTL_ResetReadyFlag();
            NFTL_Init(0,0);
            //ftl_BlkDev_Init();
            FTL_Disable_ErrLog = OriDisableErrLog;
            return 0;
        }

        u32_LoopCnt = simple_strtoul(argv[2], NULL, 16);
        HWTimer_Start();
        
        for(u32_i=0; u32_i<u32_LoopCnt; u32_i++)
        {
            NFTL_ResetReadyFlag();
            u32_t0 = HWTimer_End();
            FTL_Disable_ErrLog=1;
            NFTL_Init(0,0);
            u32_t1 = HWTimer_End();
            FTL_Disable_ErrLog=0;            
            if(u32_t0 > u32_t1)
                printf("timer overflow\n");
            else
                printf("init time: %u ms\n",
                    (u32_t1-u32_t0)/(FTL_HW_TIMER_HZ/1000));
        }
        FTL_Disable_ErrLog = OriDisableErrLog;
        return 0;
        
	}
    //===========================================
    else if(strcmp(argv[1],"format") == 0)
    {
        NFTL_CleanStruct();
        
        if(NFTL_HAL_Init(0,0))
        {
            printf("HAL init fail \n");
            return 0;
        }
        if(NFTL_BuildRootBlk())
        {
            printf("BuildRootBlk fail \n");
            return 0;
        }
        return 0;
    }
    //===========================================
    else if(strcmp(argv[1],"hal") == 0)
    {
        U16 u16_PBA = simple_strtoul(argv[2], NULL, 16);
        FtlTest_HAL(u16_PBA);
        return 0;
    }
	//===========================================
	else if(strcmp(argv[1],"speed") == 0)
    {
        #if 0
        extern U32 u32_TestSecAddr;
        for(u32_TestSecAddr=3; u32_TestSecAddr<4/*FtlDev.u16_Page512BCnt*/; u32_TestSecAddr++)
        {
            printf("*** %Xh ***\n", u32_TestSecAddr);
        #endif
        FtlTest_Performance();
        //}
        return 0;
    }
    //===========================================
    else if (strcmp(argv[1], "ftlsize")==0)
	{
        NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();
        U32 dataMB;
        U16 DBCnt, RsvBCnt;
        
		if(argc < 3)
		{
            ftl_dbg(0, 0, "\n please enter: msnand ftlsize MB \n\n");
            return 0;            
		}
        dataMB = simple_strtoul(argv[2], NULL, 10);
        if(0==dataMB)
            return 0;

        if(dataMB > (pNandDrv->u16_BlkCnt/1024)*pNandDrv->u16_BlkPageCnt*(pNandDrv->u16_PageByteCnt/1024))
        {
            ftl_dbg(0, 0, "\n data %u MB > nand size %u MB \n\n", dataMB,
                (pNandDrv->u16_BlkCnt/1024)*pNandDrv->u16_BlkPageCnt*(pNandDrv->u16_PageByteCnt/1024));
            return 0;
        }
        
        // --------------------------
        DBCnt = dataMB*0x400 / (pNandDrv->u16_BlkPageCnt*pNandDrv->u16_PageByteCnt/0x400);
        if((dataMB*0x400) % (pNandDrv->u16_BlkPageCnt*pNandDrv->u16_PageByteCnt/0x400))
            DBCnt++;

        ftl_dbg(0,0,"data %u MB = %u blocks\n", dataMB, DBCnt);

        RsvBCnt = DBCnt/BaseBlkCnt;
        if(DBCnt%BaseBlkCnt)
            RsvBCnt++;
        RsvBCnt *= ReservedBlkPerBase;
        
        ftl_dbg(0,0,"please set %u + %u = %u blocks in pni FTL partition\n", 
            DBCnt, RsvBCnt, DBCnt+RsvBCnt);        
        return 0;
	}
    //===========================================
    else if(strcmp(argv[1],"life") == 0)
    {
        if(0 == strcmp(argv[2], "fixed"))
            FtlTest_Lifetime(FTL_LIFETIME_TEST_FIXED);
        if(0 == strcmp(argv[2], "filled"))
            FtlTest_Lifetime(FTL_LIFETIME_TEST_FILLED);
        if(0 == strcmp(argv[2], "random"))
            FtlTest_Lifetime(FTL_LIFETIME_TEST_RANDOM);
    }
    //===========================================
    else if(strcmp(argv[1],"pwr_cut") == 0)
    {
        U32 cnt, i;

        cnt = simple_strtoul(argv[3], NULL, 10);

        // --------------------------
        // for auto test:
        //   1. ftltest pwr_cut init 1
        //   2. @ board.c / board_init_r
        //       - after nand_init(), call FtlTest_PwrCutTestCheck(), 
        //       - and FtlTest_PwrCutTestRun(0x3697);
        //   3. then auto random power_cut.
        // --------------------------
        
        if(strcmp(argv[2], "init") == 0)
            for(i=0; i<cnt; i++)
                FtlTest_PwrCutTestInit();
        else if(strcmp(argv[2], "w") == 0)
            FtlTest_PwrCutTestRun(cnt);
		else if(strcmp(argv[2], "rc") == 0)
            for(i=0; i<cnt; i++)
                FtlTest_PwrCutTestCheck();
        else if(strcmp(argv[2], "wrc") == 0)
            for(i=0; i<cnt; i++)
    		{
    			FtlTest_PwrCutTestRun(1);
                FtlTest_PwrCutTestCheck();
    		}
        return 0;
    }
    //===========================================
    else if(strcmp(argv[1],"temp") == 0)
    {
        U32 u32_TestCnt;

        u32_TestCnt = simple_strtoul(argv[2], NULL, 10); // 0 is 0xFFFFFFFF
        Ftl_Test(u32_TestCnt);
        return 0;
    }    
    else if(strcmp(argv[1],"case") == 0)
    {
        U32 u32_TestCnt;

        u32_TestCnt = simple_strtoul(argv[2], NULL, 10); // 0 is 0xFFFFFFFF
        Ftl_Test_Case(u32_TestCnt);
        return 0;
    }
    else if(strcmp(argv[1],"burn") == 0)
    {
        U32 u32_TestCnt;

        u32_TestCnt = simple_strtoul(argv[2], NULL, 10); // 0 is 0xFFFFFFFF
        Ftl_Burn(u32_TestCnt);
        return 0;
    }
    else if(strcmp(argv[1],"fill") == 0)
    {
        FtlTest_DataWRC(0, NFTL_GetDataCapacity(), 0);
        return 0;
    }    
    else
    {   // -----------------------------------------
        // ftltest  512BAddr  512BCnt  LoopCnt  [rc/w]          
        U32 u32_512BAddr, u32_512BCnt, u32_LoopCnt, u32_i, u32_err=0;

        if (argc < 4)
            return cmd_usage(cmdtp);
        
        printf("BIST: %s \n", 
            (strcmp(argv[4],"rc")==0) ? "RC" : 
            (strcmp(argv[4],"w")==0) ? "W" : "WRC");
        
        u32_512BAddr = simple_strtoul(argv[1], NULL, 10);
        u32_512BCnt = simple_strtoul(argv[2], NULL, 10);
        
        u32_LoopCnt = simple_strtoul(argv[3], NULL, 10);
        printf("SectorAddr: %Xh  SectorCnt: %Xh  LoopCnt: %u\n", 
            u32_512BAddr, u32_512BCnt, u32_LoopCnt);
        
        for(u32_i=0; u32_i<u32_LoopCnt; u32_i++)
        {
            if(strcmp(argv[4],"rc") == 0)
            {
                u32_err = FtlTest_DataRC(u32_512BAddr, u32_512BCnt, 0);
                if(FTL_OK != u32_err)
                    break;
            }else if(strcmp(argv[4],"w") == 0)
            {
                u32_err = FtlTest_DataWO(u32_512BAddr, u32_512BCnt, 0);
                if(FTL_OK != u32_err)
                    break;
            }
            else
            {   u32_err = FtlTest_DataWRC(u32_512BAddr, u32_512BCnt, 0);
                if(FTL_OK != u32_err)
                    break;
            }
            
            if(3697 == u32_LoopCnt) // infinite loop
                u32_i=0;
        }

        if(0==u32_err)
            printf("\n OK \n");

        return 0;
    }

    return cmd_usage(cmdtp);
}


