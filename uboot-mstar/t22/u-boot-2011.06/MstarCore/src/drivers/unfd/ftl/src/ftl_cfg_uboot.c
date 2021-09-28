/*
 * Copyright 2008, Freescale Semiconductor, Inc
 * Andy Fleming
 *
 * Based vaguely on the Linux code
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


#include "ftl_api.h"
#include "../ftl.h"
#ifdef CONFIG_ENABLE_WIDESTORAGE
#include <MsDisk.h>
#endif


static struct list_head ftl_devices;
static int cur_dev_num = -1;
int FTL_STARTBLK = 0;
int FTL_BLKCNT = 0;

#define MAX_ERRNO        4095
#define IS_ERR_VALUE(x)        ((x) >= (unsigned long)-MAX_ERRNO)

#define MTD_DEV_TYPE_NOR    0x0001
#define MTD_DEV_TYPE_NAND    0x0002
#define MTD_DEV_TYPE_ONENAND    0x0004

#define MTD_DEV_TYPE(type) ((type == MTD_DEV_TYPE_NAND) ? "nand" :    \
            (type == MTD_DEV_TYPE_ONENAND) ? "onenand" : "nor")

int ftl_mtd_part_setup(void)
{
    struct mtd_device *dev=NULL;
    struct part_info *part=NULL;    
    struct mtd_info *mtd_info;
    u8 pnum=0;
    u64 size;
    int err;
    char mtd_dev[16];
    
    err = mtdparts_init();
    if(err)
    {
        return err;
    }
    err = find_dev_and_part("MSFTL", &dev,&pnum,&part);
    if(err)
    {
        return err;
    }
    
    sprintf(mtd_dev, "%s%d", MTD_DEV_TYPE(dev->id->type), dev->id->num);
    
    mtd_info = get_mtd_device_nm(mtd_dev);
    if (IS_ERR_VALUE((unsigned long)mtd_info)) {
        printf("Partition MSFTL not found on device \n");
        return 1;
    }
    
    FTL_STARTBLK = part->offset / mtd_info->erasesize;
    FTL_BLKCNT = part->size / mtd_info->erasesize;
    
    if(part->offset + part->size == dev->id->size)
    {
        size = dev->id->size;
        do_div(size, mtd_info->erasesize * 128);
        FTL_BLKCNT -= size;
    }
    printf("%s startblk %d, blkcnt %d\n", __func__, FTL_STARTBLK, FTL_BLKCNT);

    return 0;
}


struct ftl *find_ftl_device(int dev_num)
{
    struct ftl *pFtl;
    struct list_head *entry;

    list_for_each(entry, &ftl_devices) {
        pFtl = list_entry(entry, struct ftl, link);

        if (pFtl->block_dev.dev == dev_num)
            return pFtl;
    }

    printf("NAND FTL Device %d not found\n", dev_num);

    return NULL;
}
int ftl_register(struct ftl *pFtl)
{
    /* Setup the universal parts of the block interface just once */
    pFtl->block_dev.if_type = IF_TYPE_NAND;
    pFtl->block_dev.dev = cur_dev_num++;
    pFtl->block_dev.removable = 0;

    INIT_LIST_HEAD (&pFtl->link);

    list_add_tail (&pFtl->link, &ftl_devices);

    return 0;
}

block_dev_desc_t *ftl_get_dev(int dev)
{
    struct ftl *pFtl = find_ftl_device(dev);

    return pFtl ? &pFtl->block_dev : NULL;
}

void print_ftl_devices(char separator)
{
    struct ftl *pFtl;
    struct list_head *entry;

    list_for_each(entry, &ftl_devices) 
    {
        pFtl = list_entry(entry, struct ftl, link);

        printf("%s: %d, %lu blocks", pFtl->name, pFtl->block_dev.dev, pFtl->block_dev.lba);

        if (entry->next != &ftl_devices)
            printf("%c ", separator);
    }

    printf("\n");
}

int get_ftl_num(void)
{
    return cur_dev_num;
}

int ftl_initialize(void)
{
    INIT_LIST_HEAD (&ftl_devices);
    cur_dev_num = 0;
    return 0;
}

static struct ftl sg_ftl_t;

static unsigned long ftl_bread(int dev_num, ulong start, lbaint_t blkcnt, void *dst)
{
    U32 u32_Err;
    if( blkcnt == 0)
        return 0;
    u32_Err = NFTL_ReadData((U8*)dst, blkcnt, start);
    if (u32_Err == FTL_OK)
        return blkcnt;
    else
        return 0;
}

static unsigned long ftl_bwrite(int dev_num, ulong start, lbaint_t blkcnt, const void *src)
{
    U32 u32_Err;

    u32_Err = NFTL_WriteData((U8*)src, blkcnt, start);
    if (u32_Err == FTL_OK)
        return blkcnt;
    else
        return 0;
}

static unsigned long ftl_berase(int dev_num, ulong start, lbaint_t blkcnt)
{
    ftl_dbg(FTL_DBG_LV_ERR,1,"should not be here.");
    return 0;
}

int ftl_BlkDev_Init(void)
{
    #ifdef CONFIG_ENABLE_WIDESTORAGE
    if(MSDISK_eMMC == ChkDiskType())
    {
        printf("%s: eMMC disk\n", __func__);
        return -1;
    }
    #endif

    //-------------------------------
    ftl_mtd_part_setup();
    if(NFTL_Init(FTL_STARTBLK, FTL_BLKCNT))
        return 1;
    
    ftl_initialize();
    memset(&sg_ftl_t, 0, sizeof(struct ftl));
    ftl_register(&sg_ftl_t);

    sprintf(sg_ftl_t.name, "MStar-FTL");
    sg_ftl_t.capacity = (U64) (NFTL_GetDataCapacity()<<9);
    sg_ftl_t.read_bl_len = FTL_BLK_BYTECNT;
    sg_ftl_t.write_bl_len = FTL_BLK_BYTECNT;
    sg_ftl_t.block_dev.lun = 0;
    sg_ftl_t.block_dev.type = 0;
    sg_ftl_t.block_dev.blksz = FTL_BLK_BYTECNT;
    sg_ftl_t.block_dev.lba = NFTL_GetDataCapacity();
    sg_ftl_t.block_dev.part_type = PART_TYPE_NAND;
    sg_ftl_t.block_dev.block_read = ftl_bread;
    sg_ftl_t.block_dev.block_write = ftl_bwrite;
    sg_ftl_t.block_dev.block_erase = ftl_berase;
    print_ftl_devices(',');
    
    return 0;
}


//===========================================
void NFTL_Sleep(U32 u32_us)
{
    while(u32_us > 20000)
    {
        udelay(20000);
        u32_us -= 20000;
    }
    udelay(u32_us);
}

void *FMALLOC(unsigned int ByteCnt)
{
    void *p;
    p = malloc(ByteCnt+UNFD_CACHE_LINE-1);
    if(NULL==p)
    {
        ftl_dbg(FTL_DBG_LV_ERR,1,"size: %Xh \n", ByteCnt);
        NFTL_Die();
        return NULL;
    }
    p = (void*)(((U32)p+(UNFD_CACHE_LINE-1)) & ~(UNFD_CACHE_LINE-1));
    return p;
}

void FFREE(void *p)
{
}



