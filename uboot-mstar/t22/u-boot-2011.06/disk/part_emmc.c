/*
 * See also Linux sources, fs/partitions/mac.h
 *
 * This file describes structures and values related to the standard
 * Apple SCSI disk partitioning scheme. For more information see:
 * http://developer.apple.com/techpubs/mac/Devices/Devices-126.html#MARKER-14-92
 * Refined by rui.wang@mstarsemi.com
 */
 
#include <common.h>
#include <command.h>
#include <ide.h>
#include <malloc.h>
#include "part_emmc.h"
#include <mmc.h>
#include <linux/string.h>
#include <MsTypes.h>
#include <MsSystem.h>
#include <MsMmc.h>
#include <CusRawIO.h>

#if defined(CONFIG_CMD_IDE) || \
    defined(CONFIG_CMD_MG_DISK) || \
    defined(CONFIG_CMD_SCSI) || \
    defined(CONFIG_CMD_SATA) || \
    defined(CONFIG_CMD_USB) || \
    defined(CONFIG_MMC) || \
    defined(CONFIG_MSTAR_UFS) || \
    defined(CONFIG_MSTAR_UNFD_FTL) || \
    defined(CONFIG_SD) ||\
    defined(CONFIG_SYSTEMACE)

#if (ENABLE_PART_EMMC == 1)

/* stdlib.h causes some compatibility problems; should fixe these! -- wd */
#ifndef __ldiv_t_defined
typedef struct {
        long int quot;      /* Quotient	*/
        long int rem;       /* Remainder	*/
} ldiv_t;
extern ldiv_t ldiv (long int __numer, long int __denom);
# define __ldiv_t_defined   1
#endif

static int part_emmc_read_ddb (block_dev_desc_t *dev_desc, emmc_driver_desc2_t *ddb_p);
static int part_emmc_read_pdb (block_dev_desc_t *dev_desc, int part, emmc_partition2_t *pdb_p);

static int part_emmc_write_ddb (block_dev_desc_t *dev_desc, emmc_driver_desc_t *ddb_p);
#ifdef CONFIG_DOUBLE_MBOOT
static int part_emmc_write_pdb (block_dev_desc_t *dev_desc, emmc_partition_t *pdb_p,unsigned short version);
#else
static int part_emmc_write_pdb (block_dev_desc_t *dev_desc, emmc_partition_t *pdb_p);
#endif

static int check_partition_table_version(block_dev_desc_t *dev_desc, emmc_driver_desc2_t *ddb_p);

__u16 g_emmc_reserved_for_map = EMMC_RESERVED_FOR_MAP_V2;
__u8 g_disable_nopdb_warning = 0;
emmc_driver_desc2_t  g_ddesc;
emmc_partition2_t g_mpart[EMMC_RESERVED_FOR_MAP_V2];
/*
 * Test for a valid EMMC partition
 */
int test_part_emmc (block_dev_desc_t *dev_desc)
{
        emmc_partition_t    mpart;
        ulong i, n;

        if (check_partition_table_version(dev_desc, &g_ddesc) < 0) {
                return (-1);
        }
            
        n = g_emmc_reserved_for_map;
        
        for (i=1; i<=n; ++i) {
            if ((dev_desc->block_read(dev_desc->dev, i, 1, (ulong *)&mpart) != 1) ||
                (mpart.signature != EMMC_PARTITION_MAGIC) ) {
                    return (-1);
            }
            /* update partition count */
//            n = mpart.map_count;
        }
        return (0);
}

unsigned int get_emmc_used_blockcount(block_dev_desc_t *dev_desc)
{
    int i;
    unsigned int u32UsedBlockCount = 0;
    if (check_partition_table_version(dev_desc, &g_ddesc) < 0) 
    {
            return 0;
    }

    for(i=1; i<= g_emmc_reserved_for_map; i++)
    {
        if(part_emmc_read_pdb(dev_desc, i, &g_mpart[i-1]) == 0)
        {
            u32UsedBlockCount += g_mpart[i-1].block_count ; /* assumes blk_size == 512 */
        }
        else
        {
            break;
        }
    }
    return u32UsedBlockCount;  
}

int print_info_part(block_dev_desc_t *dev_desc, char *name)
{
    ulong partnum=1;
    disk_partition_t dpt;

    for(;;)
    {
        if(get_partition_info_emmc(dev_desc, partnum, &dpt))
        {
            printf("Error >> end searching partition\n");
            return 1;
        }

        if(!strcmp(name, (const char *)dpt.name))
        	{
        	    printf("name=%s\n",(const char *)dpt.name);
                  printf("type=%s\n",(const char *)dpt.type);
                  printf("start=0x%x # the first block in partition \n",(unsigned int)dpt.start);
                  printf("size=0x%x # number of blocks \n",(unsigned int)dpt.size);
                  printf("blksz=0x%x\n",(unsigned int)dpt.blksz);
		    break;
        	}
	partnum++;
    }
    return 0;
	
}

void print_part_emmc (block_dev_desc_t *dev_desc)
{
        ulong i, n;

#if 0        
        ldiv_t mb, gb;

        n  = g_ddesc.blk_count;

        mb = ldiv(n, ((1024 * 1024) / g_ddesc.blk_size)); /* MB */
        /* round to 1 digit */
        mb.rem *= 10 * g_ddesc.blk_size;
        mb.rem += 512 * 1024;
        mb.rem /= 1024 * 1024;

        gb = ldiv(10 * mb.quot + mb.rem, 10240);
        gb.rem += 512;
        gb.rem /= 1024;


        printf ( "Block Size=%d, Number of Blocks=%d, "
            "Total Capacity: %ld.%ld MB = %ld.%ld GB\n"
            "DeviceType=0x%x, DeviceId=0x%x\n\n" 
            "   #:                 type name"
            "                   length   base       (size)\n",
            g_ddesc.blk_size,
            g_ddesc.blk_count,
            mb.quot, mb.rem, gb.quot, gb.rem,
            g_ddesc.dev_type, g_ddesc.dev_id
            );
#endif
         if (check_partition_table_version(dev_desc, &g_ddesc) < 0) {
                return;
        }
         
        printf ("   #:                 type name"
            "                   length   base       (size)\n");
        
        n = g_emmc_reserved_for_map; 
        for (i=1; i<=n; ++i) {
                ulong bytes=0, bytes_tail=0;
                char c;
				
        if (g_mpart[i-1].signature != EMMC_PARTITION_MAGIC) {
                printf("\n");
                return;
        }

                printf ("%4ld: ", i);
        /* update partition count */

        c      = 'k';
        bytes  = g_mpart[i-1].block_count;      
        bytes /= (1024 / 512);  /* kB; assumes blk_size == 512 */
        if (bytes >= 1024) {
                bytes_tail = (bytes & 1023) * 1000 /1024;
                bytes >>= 10;
                c = 'M';
        }    
        if (bytes >= 1024) {
                bytes_tail = (bytes & 1023) * 1000 /1024;
                bytes >>= 10;
                c = 'G';
        }
        printf("%20.32s %-18.32s %10u @ %-10u (%ld.%03ld%c)\n",
                    g_mpart[i-1].type,
                    g_mpart[i-1].name,
                    g_mpart[i-1].block_count,
                    g_mpart[i-1].start_block,
                    bytes, bytes_tail, c
                );
        }

        return;
}


/*
 * Read Device Descriptor Block
 */
static int part_emmc_read_ddb (block_dev_desc_t *dev_desc, emmc_driver_desc2_t *ddb_p)
{
       int i;
	unsigned char *buf;
        if(ddb_p->signature==EMMC_DRIVER_MAGIC){
	         return(0);
        }
	buf=malloc((EMMC_RESERVED_FOR_MAP_V2 + 1) * 512);
       if(buf !=NULL)
       	{
        if (dev_desc->block_read(dev_desc->dev, 0, (EMMC_RESERVED_FOR_MAP_V2 + 1), (ulong *)buf) != (EMMC_RESERVED_FOR_MAP_V2 + 1)) {
                  printf ("** Can't read Driver Desriptor Block **\n");
		free(buf);
                return (-1);
        }
		memcpy((unsigned char *)ddb_p,buf,sizeof(emmc_driver_desc2_t));	
		for(i=1;i<=g_emmc_reserved_for_map;i++)	
	           memcpy((unsigned char *)(&g_mpart[i-1]),(buf+512*(i)),sizeof(emmc_partition2_t));	
		free(buf);		
		}
        if (ddb_p->signature != EMMC_DRIVER_MAGIC) {
#if 0
            printf ("** Bad Signature: expected 0x%04x, got 0x%04x\n",
                        AC_DRIVER_MAGIC, ddb_p->signature);
#endif
                return (-1);
        }
                return (0);
}

/*
 * Read Partition Descriptor Block
 */
static int part_emmc_read_pdb (block_dev_desc_t *dev_desc, int part, emmc_partition2_t *pdb_p)
{
                /*
                * We must always read the descritpor block for
                * partition 1 first since this is the only way to
                * know how many partitions we have.
                */
                 if ((part < 1) || (part > g_emmc_reserved_for_map)) {
                    printf ("** Invalid partition %d:%d [%d:1...%d:%d only]\n",
                                dev_desc->dev, part,
                                dev_desc->dev,
                                dev_desc->dev, pdb_p->map_count);
                    return (-1);
                }

               if (pdb_p->signature != EMMC_PARTITION_MAGIC) {
                    if(!g_disable_nopdb_warning)
                        printf ("** Bad Signature on %d:%d: "
                                    "expected 0x%04x, got 0x%04x\n",
                                    dev_desc->dev, part, EMMC_PARTITION_MAGIC, pdb_p->signature);
                    return (-1);
                }
	             return(0);
        /* NOTREACHED */
}

static int part_emmc_write_ddb (block_dev_desc_t *dev_desc, emmc_driver_desc_t *ddb_p)
{

        if(ddb_p->signature != EMMC_DRIVER_MAGIC){
                printf ("** Bad Signature: expected 0x%04x, try to write 0x%04x **\n",
                        EMMC_DRIVER_MAGIC, ddb_p->signature);
                return (-1);
        }

        if(dev_desc->block_write(dev_desc->dev, 0, 1, (ulong *)ddb_p) != 1){
                printf("** Can't write Driver Desiptor Block **\n");
                return (-1);
        }

        return (0);
}

#ifdef CONFIG_DOUBLE_MBOOT
static int part_emmc_write_pdb (block_dev_desc_t *dev_desc, emmc_partition_t *pdb_p,unsigned short version)
{
        int n = 1;
        emmc_partition_t exist_pdb;
        ulong special_start, start_block,usr_partition_start;

	if(version==EMMC_PARTITIONTABLE_VERSION2)
	{
		start_block=EMMC_PARTITION_START_V2;
		usr_partition_start=EMMC_PARTITION_START_V2;
	}
	else if(version==EMMC_PARTITIONTABLE_VERSION3)
	{
		start_block=EMMC_PARTITION_START_V3;
		usr_partition_start=EMMC_PARTITION_START_V3;
	}
	else
	{
		start_block=EMMC_PARTITION_START_V1;
		usr_partition_start=EMMC_PARTITION_START_V1;
	}
  
        if(pdb_p->signature != EMMC_PARTITION_MAGIC){
                printf ("** Bad Signature: expected 0x%04x, try to write 0x%04x **\n",
                        EMMC_PARTITION_MAGIC, pdb_p->signature);
                return (-1);
        }

        special_start = pdb_p->start_block;
            
        for (;;) {
                /*
                * We must always read the descritpor block for
                * partition 1 first since this is the only way to
                * know how many partitions we have.
                */
                if (dev_desc->block_read (dev_desc->dev, n, 1, &exist_pdb) != 1) {
                        printf ("** Can't read Partition Map on %d:%d **\n",
                                dev_desc->dev, n);
                        return (-1);
                }

                if(exist_pdb.signature == EMMC_PARTITION_MAGIC){
                        if(!strcmp((const char *)pdb_p->name, (const char *)exist_pdb.name)){
                            printf("** Try to change a exist partition %s **\n", exist_pdb.name);
                            if(pdb_p->block_count > exist_pdb.block_count){
                                printf("** The new size of the partition is too big!\n");
                                return (-1);
                            }

                            if(special_start != 0)
                                pdb_p->start_block = special_start;
                            else
                                pdb_p->start_block = exist_pdb.start_block;
                           
                            if(pdb_p->start_block + pdb_p->block_count > dev_desc->lba){
                                printf("** Partition exceed emmc capacity**\n");
                                return (-1);
                            }   
							
                            if(dev_desc->block_write(dev_desc->dev, n, 1, (ulong *)pdb_p) != 1){
                                printf("** Can't write Driver Desiptor Block **\n");
                                return (-1);
                            }
			      memcpy((unsigned char *)(&g_mpart[n-1]), (unsigned char *)(pdb_p), sizeof(emmc_partition2_t));
                            return (0);
                        }

                        if(exist_pdb.start_block + exist_pdb.block_count > usr_partition_start)
                            /* skip UBOOT partition */
                            start_block += exist_pdb.block_count;
                        
                        ++n;
                        if(n > g_emmc_reserved_for_map){
                            printf("** Exceed the max number of partition, STOP!**\n");
                            return (-1);
                        }
                }
                else{
                        if(special_start != 0)
                            pdb_p->start_block = special_start;
                        else
                            pdb_p->start_block = start_block;
                        
                        if(pdb_p->start_block + pdb_p->block_count > dev_desc->lba){
                            printf("** Partition exceed emmc capacity**\n");
                            return (-1);
                        }

                        if(dev_desc->block_write(dev_desc->dev, n, 1, (ulong *)pdb_p) != 1){
                            printf("** Can't write Driver Desiptor Block **\n");
                            return (-1);
                        }
			   memcpy((unsigned char *)(&g_mpart[n-1]), (unsigned char *)(pdb_p), sizeof(emmc_partition2_t));
                        return (0);
                }
                
        }

        /* NOTREACHED */
}
#else
static int part_emmc_write_pdb (block_dev_desc_t *dev_desc, emmc_partition_t *pdb_p)
{
        int n = 1;
        emmc_partition_t exist_pdb;
        ulong special_start, start_block = EMMC_PARTITION_START;
        
        if(pdb_p->signature != EMMC_PARTITION_MAGIC){
                printf ("** Bad Signature: expected 0x%04x, try to write 0x%04x **\n",
                        EMMC_PARTITION_MAGIC, pdb_p->signature);
                return (-1);
        }

        special_start = pdb_p->start_block;
            
        for (;;) {
                /*
                * We must always read the descritpor block for
                * partition 1 first since this is the only way to
                * know how many partitions we have.
                */
                if (dev_desc->block_read (dev_desc->dev, n, 1, &exist_pdb) != 1) {
                        printf ("** Can't read Partition Map on %d:%d **\n",
                                dev_desc->dev, n);
                        return (-1);
                }

                if(exist_pdb.signature == EMMC_PARTITION_MAGIC){
                        if(!strcmp((const char *)pdb_p->name, (const char *)exist_pdb.name)){
                            printf("** Try to change a exist partition %s **\n", exist_pdb.name);
                            if(pdb_p->block_count > exist_pdb.block_count){
                                printf("** The new size of the partition is too big!\n");
                                return (-1);
                            }

                            if(special_start != 0)
                                pdb_p->start_block = special_start;
                            else
                                pdb_p->start_block = exist_pdb.start_block;
                           
                            if(pdb_p->start_block + pdb_p->block_count > dev_desc->lba){
                                printf("** Partition exceed emmc capacity**\n");
                                return (-1);
                            }                         
                                
                            if(dev_desc->block_write(dev_desc->dev, n, 1, (ulong *)pdb_p) != 1){
                                printf("** Can't write Driver Desiptor Block **\n");
                                return (-1);
                            }
			memcpy((unsigned char *)(&g_mpart[n-1]), (unsigned char *)(pdb_p), sizeof(emmc_partition2_t));
                            return (0);
                        }

                        if(exist_pdb.start_block + exist_pdb.block_count > EMMC_PARTITION_START)
                            /* skip UBOOT partition */
                            start_block += exist_pdb.block_count;
                        
                        ++n;
                        if(n > g_emmc_reserved_for_map){
                            printf("** Exceed the max number of partition, STOP!**\n");
                            return (-1);
                        }
                }
                else{
                        if(special_start != 0)
                            pdb_p->start_block = special_start;
                        else
                            pdb_p->start_block = start_block;
                        
                        if(pdb_p->start_block + pdb_p->block_count > dev_desc->lba){
                            printf("** Partition exceed emmc capacity**\n");
                            return (-1);
                        }
                      
                        if(dev_desc->block_write(dev_desc->dev, n, 1, (ulong *)pdb_p) != 1){
                            printf("** Can't write Driver Desiptor Block **\n");
                            return (-1);
                        }
			memcpy((unsigned char *)(&g_mpart[n-1]), (unsigned char *)(pdb_p), sizeof(emmc_partition2_t));
                            return (0);
                }
                
        }

        /* NOTREACHED */
}
#endif

static int check_partition_table_version(block_dev_desc_t *dev_desc, emmc_driver_desc2_t *ddb_p)
{     
        if (part_emmc_read_ddb (dev_desc, ddb_p) < 0) {
                return (-1);
        }
		#ifdef CONFIG_DOUBLE_MBOOT
        if (ddb_p->version == EMMC_PARTITIONTABLE_VERSION2 || ddb_p->version == EMMC_PARTITIONTABLE_VERSION3){
		#else
        if (ddb_p->version == EMMC_PARTITIONTABLE_VERSION2){
		#endif
            g_emmc_reserved_for_map = ddb_p->drvr_cnt;
            return 1;
        }
        else{            
            g_emmc_reserved_for_map = EMMC_RESERVED_FOR_MAP;
            return 0;
        }
}


int get_partition_info_emmc (block_dev_desc_t *dev_desc, int part, disk_partition_t *info)
{

        if (check_partition_table_version(dev_desc, &g_ddesc) < 0) {
                return (-1);
        }
        
        if ((part > g_emmc_reserved_for_map) || (part < 1)){
                return (-1);
        }

        info->blksz = g_ddesc.blk_size;

        if (part_emmc_read_pdb (dev_desc, part, &(g_mpart[part-1]))) {
                return (-1);
        }

        info->start = g_mpart[part-1].start_block;
        info->size  = g_mpart[part-1].block_count;
        info->blksz  =   512;
        memcpy (info->type, g_mpart[part-1].type, sizeof(info->type));
        memcpy (info->name, g_mpart[part-1].name, sizeof(info->name));

        return (0);
}

int Search_Mboot_partition_emmc (block_dev_desc_t *dev_desc)
{
        int i;
		
        g_disable_nopdb_warning = 1;
        for(i=1;i<= EMMC_RESERVED_FOR_MAP_V2;i++)
        {
            if(part_emmc_read_pdb(dev_desc, i, &g_mpart[i-1]) == 0)
            {
                if(strcmp("MBOOT", (const char *)g_mpart[i-1].name)==0)
                {
                    printf("** Find MBOOT **\n");
                    g_disable_nopdb_warning = 0;
                    return i;
                }
            }
        }
        g_disable_nopdb_warning = 0;
        return 0;
}

#ifdef CONFIG_DOUBLE_MBOOT
int Search_Mbootbak_partition_emmc (block_dev_desc_t *dev_desc)
{
        int i;
		
        g_disable_nopdb_warning = 1;
        for(i=1;i<= EMMC_RESERVED_FOR_MAP_V2;i++)
        {
            if(part_emmc_read_pdb(dev_desc, i, &g_mpart[i-1]) == 0)
            {
                if(strcmp("MBOOTBAK", (const char *)g_mpart[i-1].name)==0)
                {
                    printf("** Find MBOOTBAK **\n");
                    g_disable_nopdb_warning = 0;
                    return i;
                }
            }
        }
        g_disable_nopdb_warning = 0;
        return 0;
}
#endif
int Search_Mpool_partition_emmc (block_dev_desc_t *dev_desc)
{
        int i;
    
        g_disable_nopdb_warning = 1;
        for(i=1;i<= EMMC_RESERVED_FOR_MAP_V2;i++)
        {
            if(part_emmc_read_pdb(dev_desc, i, &g_mpart[i-1]) == 0)
            {
                if(strcmp("MPOOL", (const char *)g_mpart[i-1].name)==0)
                {
                    printf("** Find MPOOL **\n");
                    g_disable_nopdb_warning = 0;
                    return i;
                }
            }
        }
        g_disable_nopdb_warning = 0;
        return 0;
}

#ifdef CONFIG_DOUBLE_MBOOT
int add_new_emmc_partition(block_dev_desc_t *dev_desc, disk_partition_t *info)
{
        int ret;
        int flag = 0;
        emmc_driver_desc_t ddb_base;
        emmc_partition_t pdb_base; 
		
        memset(&pdb_base, 0, sizeof(emmc_partition_t));
        ddb_base.signature = EMMC_DRIVER_MAGIC;
        ddb_base.version = EMMC_PARTITIONTABLE_VERSION3;
        ddb_base.drvr_cnt = EMMC_RESERVED_FOR_MAP_V3;
        pdb_base.signature = EMMC_PARTITION_MAGIC;

	 flag = part_emmc_read_ddb(dev_desc, &g_ddesc);	
	  
        if(flag){
            printf("Need write new driver description table!\n");
            ret = part_emmc_write_ddb(dev_desc, &ddb_base);
            if(ret){
                printf("Error during write new driver description table!\n");
                return ret;
            }
            g_emmc_reserved_for_map= ddb_base.drvr_cnt;
        }else if(g_ddesc.version == EMMC_PARTITIONTABLE_VERSION2 ||g_ddesc.version == EMMC_PARTITIONTABLE_VERSION3)
                g_emmc_reserved_for_map = ddb_base.drvr_cnt;
            else
                g_emmc_reserved_for_map = EMMC_RESERVED_FOR_MAP;

        strcpy((char *)pdb_base.name, (const char *)info->name);
        pdb_base.block_count = info->size;
        pdb_base.start_block = info->start;
		
        ret = part_emmc_write_pdb(dev_desc, &pdb_base,flag==0?g_ddesc.version:ddb_base.version);
        if(ret){
            printf("Error during write new partition description table!\n");
            return ret;
        }

        return 0;        
}
#else
int add_new_emmc_partition(block_dev_desc_t *dev_desc, disk_partition_t *info)
{
        int ret;
	 emmc_driver_desc_t ddb_base;
        emmc_partition_t pdb_base;        

        memset(&pdb_base, 0, sizeof(emmc_partition_t));
        ddb_base.signature = EMMC_DRIVER_MAGIC;
        ddb_base.version = EMMC_PARTITIONTABLE_VERSION2;
        ddb_base.drvr_cnt = EMMC_RESERVED_FOR_MAP_V2;
        pdb_base.signature = EMMC_PARTITION_MAGIC;
        
        if(part_emmc_read_ddb(dev_desc, &g_ddesc)){
            printf("Need write new driver description table!\n");
            ret = part_emmc_write_ddb(dev_desc, &ddb_base);
            if(ret){
                printf("Error during write new driver description table!\n");
                return ret;
            }
            g_emmc_reserved_for_map= ddb_base.drvr_cnt;
        }else if(g_ddesc.version == EMMC_PARTITIONTABLE_VERSION2)
                g_emmc_reserved_for_map = ddb_base.drvr_cnt;
            else
                g_emmc_reserved_for_map = EMMC_RESERVED_FOR_MAP;

        strcpy((char *)pdb_base.name, (const char *)info->name);
        pdb_base.block_count = info->size;
        pdb_base.start_block = info->start;
        
        ret = part_emmc_write_pdb(dev_desc, &pdb_base);
        if(ret){
            printf("Error during write new partition description table!\n");
            return ret;
        }

        return 0;        
}
#endif

int remove_emmc_partition(block_dev_desc_t *dev_desc, disk_partition_t *info)
{
        int n = 0;
        emmc_partition_t exist_pdb, pdb;        

        pdb.signature = 0;

        if (check_partition_table_version(dev_desc, &g_ddesc) < 0) {
            printf("Haven't driver description table!\n");
            return (-1);
        }
        
        for (;;) {
                /*
                * We must always read the descritpor block for
                * partition 1 first since this is the only way to
                * know how many partitions we have.
                */
                if(n > g_emmc_reserved_for_map){
                        printf("** Cannot remove the partition, not found **\n");
                        return (0);
                }
                if (dev_desc->block_read (dev_desc->dev, n, 1, (ulong *)&exist_pdb) != 1) {
                        printf ("** Can't read Partition Map on %d:%d **\n",
                                dev_desc->dev, n);
                        return (-1);
                }

                if(exist_pdb.signature == EMMC_PARTITION_MAGIC){
                        if(!strcmp((const char *)info->name, (const char *)exist_pdb.name)){
                            if(dev_desc->block_write(dev_desc->dev, n, 1, (ulong *)&pdb) != 1){
                                printf("** Can't remove the partition %s **\n", exist_pdb.name);
                                return (-1);
                            }
		  g_mpart[n-1].signature = 0x0;
                            return (0);
                        }
                }
                ++n;                                
        }
        
        return (-1);        
}

int _get_mmc_partsize (block_dev_desc_t *dev_desc, char *puPartName, unsigned int *u32PartSize)
{
    int i;
    
    if (check_partition_table_version(dev_desc, &g_ddesc) < 0) {
            return (-1);
    }
    
    for(i=1;i<= g_emmc_reserved_for_map;i++)
    {
        if(part_emmc_read_pdb(dev_desc, i, &g_mpart[i-1]) == 0)
        {
            if(strcmp(puPartName, (const char *)g_mpart[i-1].name)==0)
            {
                *u32PartSize = g_mpart[i-1].block_count * 512; /* assumes blk_size == 512 */
                printf("%s size : 0x%x\n",puPartName,*u32PartSize);
                break;
            }
        }
    }

    return 0;
}

int _rmgpt_sync(int i)
{
   g_mpart[i-1].signature = 0x0;
   
   return 0;
}

int rmgpt_emmc_partition(int curr_device, char *symbol)
{
    struct mmc *mmc = find_mmc_device(curr_device);
    unsigned long *reset = NULL;
    int i;
    block_dev_desc_t *mmc_dev;
    unsigned char force_rmgpt = 0;

    if (!mmc)
    {
        printf("no mmc device at slot %x\n", curr_device);
        return 1;
    }
    if (!mmc->has_init)
    {
        printf("Do mmc init first!\n");
        mmc_init(mmc);
        mmc->has_init = 1;
    }

    mmc_dev = mmc_get_dev(curr_device);
    if ( (mmc_dev == NULL) || (mmc_dev->type == DEV_TYPE_UNKNOWN) )
    {
        printf("no mmc device found!\n");
        return 1;
    }

    if ((symbol) && (!strncmp(symbol, ".all", 4)))
    {
         force_rmgpt = 1;
         printf("remove all partition info\n");
    }
    reset = (unsigned long *)malloc(512);
    if(reset == NULL)
    {
        printf("malloc fail!\n");
        return 1;
    }
    memset(reset, 0, 512);

    #ifdef CONFIG_MTK_LOADER
    for (i = 1; i <= g_emmc_reserved_for_map; i++)
    {
        mmc->block_dev.block_write(curr_device, i, 1, reset);
        _rmgpt_sync(i);
    }
    if (force_rmgpt == 0)
    {
        if (add_ca_partition_emmc() != 0)
        {
            printf("add_ca_partition_emmc failed\n");
            free(reset);
            return 1;
        }
    }
    #else
    unsigned char u8_uboot_partnum = Search_Mboot_partition_emmc(mmc_dev);
    unsigned char u8_mpool_partnum = Search_Mpool_partition_emmc(mmc_dev);
    #ifdef CONFIG_DOUBLE_MBOOT
    unsigned char u8_mbootbak_partnum = Search_Mbootbak_partition_emmc(mmc_dev);
    #endif
      
        if ((u8_uboot_partnum > 0)||(u8_mpool_partnum > 0))
        {
            for (i = 1; i <= g_emmc_reserved_for_map; i++)
            {
                #ifdef CONFIG_DOUBLE_MBOOT
                if((force_rmgpt==0)&&(((i == u8_uboot_partnum)&&(u8_uboot_partnum >0))
                    ||((i == u8_mpool_partnum)&&(u8_mpool_partnum >0))
                    || ((i == u8_mbootbak_partnum)&&(u8_mbootbak_partnum >0))))
                #else
                if((force_rmgpt==0)&&(((i == u8_uboot_partnum)&&(u8_uboot_partnum >0))
                    ||((i == u8_mpool_partnum)&&(u8_mpool_partnum >0))))
                #endif
                {
                    //printf("test is %d\n" , i);
                    continue; // keep data for ROM boot
                }
                else
                {
                    mmc->block_dev.block_write(curr_device, i, 1, reset);
                    _rmgpt_sync(i);
                }
            }
        }
        else
        {
            printf("no UBOOT or MPOOL Partition found!\n");

            for(i=1;i<= g_emmc_reserved_for_map;i++)
            {
                mmc->block_dev.block_write(curr_device, i, 1, reset);
                _rmgpt_sync(i);
            }
        }
    #endif

    free(reset);
    return 0;
}

int clone_part(int curr_device, char * const source, char * const destination)
{
    struct mmc *mmc = find_mmc_device(curr_device);
    block_dev_desc_t *mmc_dev;
    emmc_partition_t mpart[EMMC_RESERVED_FOR_MAP_V2];
    int i;

    u32 src_start_blk = 0, src_blks = 0;
    u32 dst_start_blk = 0, dst_blks = 0;
    u32 cur_blks =0;

    u8 buf[65536] = {0};

    if( source == NULL )
    {
        printf("No source partition name\n");
        return 1;
    }
    else if( destination == NULL )
    {
        printf("No destination partition name\n");
        return 1;
    }

    if(!mmc)
    {
        printf("no mmc device at slot %x\n", curr_device);
        return 1;
    }

    if(!mmc->has_init)
    {
        printf("Do mmc init first!\n");
        mmc_init(mmc);
        mmc->has_init = 1;
    }

    mmc_dev = mmc_get_dev(curr_device);

    for (i=1; i<=EMMC_RESERVED_FOR_MAP_V2; i++)
    {
        if (mmc_dev->block_read (mmc_dev->dev, i, 1, (ulong *)&mpart[i-1])!= 1)
        {
            printf ("** Can't read Partition Map on %d:%d **\n", mmc_dev->dev, i);
            return -1;
        }

        if( strcmp((const char *)mpart[i-1].name, source) == 0 )
        {
            src_start_blk = mpart[i-1].start_block;
            src_blks = mpart[i-1].block_count;
        }

        if( strcmp((const char *)mpart[i-1].name, destination) == 0 )
        {
            dst_start_blk = mpart[i-1].start_block;
            dst_blks = mpart[i-1].block_count;
        }
    }

    if( src_start_blk == 0 )
    {
        printf("No source partition name matches %s\n", source);
        return 1;
    }
    else if( dst_start_blk == 0 )
    {
        printf("No destinantion partition name matches %s\n", destination);
        return 1;
    }

    if( dst_blks < src_blks )
    {
        printf("Destination partition is smaller than source\n");
        return 1;
    }

    cur_blks = src_blks;
    i=0;

    do
    {
        mmc->block_dev.block_read(curr_device, src_start_blk+i*128, (cur_blks >= 128) ? 128 : cur_blks, buf);
        mmc->block_dev.block_write(curr_device, dst_start_blk+i*128, (cur_blks >= 128) ? 128 : cur_blks, buf);
        if( cur_blks >= 128 )
        {
            cur_blks -= 128;
            i++;
        }
     } while (cur_blks >= 128);

    return 0;
}

void erase_customer_partition(int curr_device, struct mmc *mmc, s32 *start, u32 *cnt)
{
    unsigned long *reset = (unsigned long *)malloc(512);
    block_dev_desc_t *mmc_dev;
    disk_partition_t dpt;
    int i;
    u32 partnum = 1;

    if(reset == NULL)
    {
        printf("mmc erase malloc fail!\n");
        //return 1;
    }

     memset(reset, 0, 512);

     for(i=3;i<= g_emmc_reserved_for_map;i++)
     {
         mmc->block_dev.block_write(curr_device, i, 1, reset);
	  g_mpart[i -1].signature = 0x0;
     }

     free(reset);

     mmc_dev = mmc_get_dev(curr_device);
     if ((mmc_dev == NULL) ||
         (mmc_dev->type == DEV_TYPE_UNKNOWN))
         {
             printf("no mmc device found!\n");
	      //return 1;
         }

     for(i=1;i<= g_emmc_reserved_for_map;i++)
         {
             if(get_partition_info_emmc(mmc_dev, partnum, &dpt))
	      break;
	      if(!strcmp("MPOOL", (const char *)dpt.name)){
		  	(*start) = dpt.start+dpt.size;
		       (*cnt) = mmc->block_dev.lba-(*start); //block number
		       break;
			   }
      partnum++;
	  }
}

int _erase_sync(block_dev_desc_t *dev_desc, int start, int cnt)
{
    int j;
  
    if(start == 0 && (cnt == dev_desc->lba))
    {
	   for(j = 1; j <= g_emmc_reserved_for_map; j++)
	   	{
	           g_mpart[j-1].signature = 0x0;
	   	}
	       g_ddesc.signature = 0x0;
    }
	
    return 0;
}

void get_version_emmc(block_dev_desc_t *dev_desc)
{
    #ifdef CONFIG_DOUBLE_MBOOT
	 dev_desc->version_flag = EMMC_PARTITIONTABLE_VERSION3;
        emmc_driver_desc_t ddb;
        int ret=dev_desc->block_read(dev_desc->dev, 0, 1, &ddb);
		
        if(ret != 1 || ddb.signature != EMMC_DRIVER_MAGIC)
        {
            dev_desc->version_flag=  EMMC_PARTITIONTABLE_VERSION3;
         }
        else if(ddb.version == EMMC_PARTITIONTABLE_VERSION2)
        {
            dev_desc->version_flag=  EMMC_PARTITIONTABLE_VERSION2;
        }
         else if(ddb.version == EMMC_PARTITIONTABLE_VERSION3)
        {
            dev_desc->version_flag=  EMMC_PARTITIONTABLE_VERSION3;
        }
    #endif
}	

/*tag==0 Represent Configure the size and start of MPOOL.  
   tag==1 Represent Configure the size and start of MBOOT.*/		
void Configure_Boot(block_dev_desc_t *dev_desc, disk_partition_t *info, int tag)
{
    if(tag == 0)
    {
        #ifdef CONFIG_DOUBLE_MBOOT
        if(dev_desc->version_flag == EMMC_PARTITIONTABLE_VERSION2)
        {
            info->size = EMMC_MPOOL_PATITION_SIZE_V2  / 512;
            info->start = EMMC_MPOOL_PATITION_START_V2  / 512;
        }
        else if(dev_desc->version_flag == EMMC_PARTITIONTABLE_VERSION3)
        {
            info->size = EMMC_MPOOL_PATITION_SIZE_V3  / 512;
            info->start = EMMC_MPOOL_PATITION_START_V3  / 512;
        }
        #else
        info->size = EMMC_MPOOL_PATITION_SIZE  / 512;
        info->start = EMMC_MPOOL_PATITION_START  / 512;
        #endif
        strcpy((char *)info->name, "MPOOL");
    }
    else if(tag == 1)
    {
        #ifdef CONFIG_DOUBLE_MBOOT
        if(dev_desc->version_flag == EMMC_PARTITIONTABLE_VERSION2)
        {
            info->size = EMMC_MBOOT_PATITION_SIZE_V2  / 512;
            info->start = EMMC_MBOOT_PATITION_START_V2  / 512;
        }
        else if(dev_desc->version_flag == EMMC_PARTITIONTABLE_VERSION3)
        {
            info->size = EMMC_MBOOT_PATITION_SIZE_V3  / 512;
            info->start = EMMC_MBOOT_PATITION_START_V3  / 512;
        }
        #else
        info->size = EMMC_MBOOT_PATITION_SIZE  / 512;
        info->start = EMMC_MBOOT_PATITION_START  / 512;
        #endif
        strcpy((char *)info->name, "MBOOT");
    }
}

/*tag==0 Represent define the size of mpool.  
   tag==1 Represent define the size of mboot.    */
ulong Define_Boot_size(block_dev_desc_t *dev_desc, int tag)
{
    ulong mboot_size_t;
    ulong mpool_size_t;

    if(tag == 0)
    	{
    	    #ifdef CONFIG_DOUBLE_MBOOT
           if(dev_desc->version_flag == EMMC_PARTITIONTABLE_VERSION3)
	        {
	            mpool_size_t=EMMC_MPOOL_PATITION_SIZE_V3 / 512; 
               }
	     else
		 {
		     mpool_size_t=EMMC_MPOOL_PATITION_SIZE_V2 / 512;
		  }
	     #else
		      mpool_size_t=EMMC_MPOOL_PATITION_SIZE / 512;
            #endif
	     return mpool_size_t;
    	}
    else if(tag == 1)
    	{
    	    #ifdef CONFIG_DOUBLE_MBOOT
           if(dev_desc->version_flag ==  EMMC_PARTITIONTABLE_VERSION3)
	        {
	            mboot_size_t=EMMC_MBOOT_PATITION_SIZE_V3 / 512; 
		  }
           else
		  {
		      mboot_size_t=EMMC_MBOOT_PATITION_SIZE_V2 / 512;
                 }
	     #else
		      mboot_size_t=EMMC_MBOOT_PATITION_SIZE / 512;
	     #endif
	     return mboot_size_t;
	 }

	return 0;
}

/*tag==0 Represent define the start of mpool.  
   tag==1 Represent define the start of mboot.   */
ulong Define_Boot_start(block_dev_desc_t *dev_desc, int tag)
{
    ulong mboot_start_t;
    ulong mpool_start_t;

    if(tag == 0)
    	{
    	    #ifdef CONFIG_DOUBLE_MBOOT
           if(dev_desc->version_flag == EMMC_PARTITIONTABLE_VERSION3)
	        {
	            mpool_start_t=EMMC_MPOOL_PATITION_START_V3 / 512;
               }
	     else
		 {
		     mpool_start_t=EMMC_MPOOL_PATITION_START_V2 / 512;
		  }
	     #else
		     mpool_start_t=EMMC_MPOOL_PATITION_START / 512;
            #endif
	     return mpool_start_t;
    	}
    else if(tag == 1)
    	{
    	    #ifdef CONFIG_DOUBLE_MBOOT
           if(dev_desc->version_flag ==  EMMC_PARTITIONTABLE_VERSION3)
	        {
	            mboot_start_t=EMMC_MBOOT_PATITION_START_V3  / 512;
		  }
           else
		  {
		      mboot_start_t=EMMC_MBOOT_PATITION_START_V2  / 512;
                 }
	     #else
		      mboot_start_t=EMMC_MBOOT_PATITION_START  / 512;
	     #endif
 	     return mboot_start_t;
	 }

        return 0;
}

#ifdef CONFIG_DOUBLE_MBOOT 
/*Configure the size and start of MBOOTBAK.
    when CONFIG_DOUBLE_MBOOT. MBOOTBAK use to recovery*/
int Configure_MBOOTBAK(block_dev_desc_t *dev_desc, disk_partition_t *info)
{
        if(dev_desc->version_flag == EMMC_PARTITIONTABLE_VERSION3)
	    {
	        info->size = EMMC_MBOOTBAK_PATITION_SIZE_V3 / 512;
               info->start = EMMC_MBOOTBAK_PATITION_START_V3 / 512;;
               strcpy((char *)info->name, "MBOOTBAK");
               return -1;
	    }
	else
		 return 0;

}

/*tag==0 Represent define the size of mbootbak.  
   tag==1 Represent define the start of mbootbak.   */
ulong Define_MBOOTBAK(block_dev_desc_t *dev_desc, int tag)
{
    ulong mbootbak_size_t;
    ulong mbootbak_start_t;
	
    if(dev_desc->version_flag == EMMC_PARTITIONTABLE_VERSION3)
        {
        if(tag == 0)
            {
            mbootbak_size_t=EMMC_MBOOTBAK_PATITION_SIZE_V3 / 512;
	     return mbootbak_size_t;
             }
	 else if(tag == 1)
            {
            mbootbak_start_t=EMMC_MBOOTBAK_PATITION_START_V3 / 512;
	     return mbootbak_start_t;
	     }
	  }
     else
         return 0;

}
#endif
#endif
#endif


static int check_single_ca_part(emmc_partition2_t *mpart, char *name, u32 start_addr, u32 size)
{
    if (mpart->signature != EMMC_PARTITION_MAGIC)
    {
        printf("%s : Invalid Partition Signature : 0x%x\n", __FUNCTION__,mpart->signature);
        return -1;
    }

    if (strncmp((const char *)(mpart->name), name, sizeof(mpart->name)) != 0)
    {
        printf("%s : Partition Name do Not Match\n", __FUNCTION__);
        return -1;
    }
    if ((mpart->start_block != (start_addr/512)) || (mpart->block_count != (size/512)))
    {
        printf("%s : Partition Info do Not Match\n", __FUNCTION__);
        return -1;
    }
    return 0;
}

int check_ca_partition_emmc(void)
{
#ifdef CONFIG_MTK_LOADER
    block_dev_desc_t *mmc_dev;

    mmc_dev = mmc_get_cur_dev();
    if (mmc_dev == NULL)
    {
        printf("no mmc device found!\n");
        return -1;
    }

    if (check_partition_table_version(mmc_dev, &g_ddesc) < 0)
    {
       printf("%s : check_partition_table_version failed !\n", __FUNCTION__);
       return -1;
    }

    if (check_single_ca_part(&g_mpart[0], EMMC_MBOOTA_PAR_NAME, EMMC_MBOOTA_PAR_START, EMMC_MBOOTA_PAR_SIZE) != 0)
        return -1;
    if (check_single_ca_part(&g_mpart[1], EMMC_MPOOLA_PAR_NAME, EMMC_MPOOLA_PAR_START, EMMC_MPOOLA_PAR_SIZE) != 0)
        return -1;
    if (check_single_ca_part(&g_mpart[2], EMMC_MBOOTB_PAR_NAME, EMMC_MBOOTB_PAR_START, EMMC_MBOOTB_PAR_SIZE) != 0)
        return -1;
    if (check_single_ca_part(&g_mpart[3], EMMC_MPOOLB_PAR_NAME, EMMC_MPOOLB_PAR_START, EMMC_MPOOLB_PAR_SIZE) != 0)
        return -1;
#endif
    return 0;
}

int access_ca_partition_emmc(char *partition_name)
{
#ifdef CONFIG_MTK_LOADER
    if ((strcmp(partition_name, EMMC_MBOOTA_PAR_NAME) == 0)      \
        || (strcmp(partition_name, EMMC_MPOOLA_PAR_NAME) == 0)   \
        || (strcmp(partition_name, EMMC_MBOOTB_PAR_NAME) == 0)   \
        || (strcmp(partition_name, EMMC_MPOOLB_PAR_NAME) == 0))
    {
        return 1;
    }
#endif
    return 0;
}


static int add_single_ca_part(block_dev_desc_t *dev_desc, char *name, ulong start, ulong size)
{
    disk_partition_t dpt;

    memset(&dpt, 0, sizeof(dpt));
    strncpy((char *)&dpt.name, name, sizeof(dpt.name)-1);
    dpt.start = start / dev_desc->blksz;
    dpt.size  = size  / dev_desc->blksz;
    dpt.blksz = dev_desc->blksz;

    if (add_new_emmc_partition(dev_desc, &dpt) != 0)
    {
        printf("Add new partition %s failed!\n", name);
        return -1;
    }
    return 0;
}

int add_ca_partition_emmc(void)
{
#ifdef CONFIG_MTK_LOADER
    block_dev_desc_t *mmc_dev;

    mmc_dev = mmc_get_cur_dev();
    if (mmc_dev == NULL)
    {
        printf("no mmc device found!\n");
        return -1;
    }

    if (add_single_ca_part(mmc_dev, EMMC_MBOOTA_PAR_NAME, EMMC_MBOOTA_PAR_START, EMMC_MBOOTA_PAR_SIZE) != 0)
        return -1;
    if (add_single_ca_part(mmc_dev, EMMC_MPOOLA_PAR_NAME, EMMC_MPOOLA_PAR_START, EMMC_MPOOLA_PAR_SIZE) != 0)
        return -1;
    if (add_single_ca_part(mmc_dev, EMMC_MBOOTB_PAR_NAME, EMMC_MBOOTB_PAR_START, EMMC_MBOOTB_PAR_SIZE) != 0)
        return -1;
    if (add_single_ca_part(mmc_dev, EMMC_MPOOLB_PAR_NAME, EMMC_MPOOLB_PAR_START, EMMC_MPOOLB_PAR_SIZE) != 0)
        return -1;

    printf("add_ca_partition_emmc(MTK) success !\n");
#endif
    return 0;
}

