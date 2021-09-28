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
#include <stdio.h>
#include <common.h>
#include <command.h>
#include <MsMmap.h>
#include <MsEnvironment.h>
#include <MsDebug.h>
#include <linux/types.h>
#include <asm/byteorder.h>
#include <miu/MsDrvMiu.h>
#include <drvBDMA.h>
#include <spi.h>
#include <MsRawIO.h>
#include <demura_config.h>
#include "ms_utils.h"
#include "crc_libs.h"


#ifdef ALIGN
#undef ALIGN
#define ALIGN(x,a)        __ALIGN_MASK((x),(typeof(x))(a)-1)
#define __ALIGN_MASK(x,mask)    (((x)+(mask))&~(mask))
#endif

#define ALIGN_1MB                (1 * 1024 * 1024)
#define DEMURA_SF_MAGIC_STRING   "Mstar CRC :\0"

struct mstar_crc_hdr {
	char magic[12];
	MS_U32  dat_offset;
	MS_U32  dat_length;
	MS_U32 crc_32;
};

#if CONFIG_SPI_FLASH

static MS_U32 sf_init_flag = FALSE;

MS_BOOL init_spi_flash(void)
{
    char cmd[256];

    if (sf_init_flag == FALSE)
    {
        memset(cmd, 0, sizeof(cmd));
        snprintf(cmd, sizeof(cmd), "sf probe 0 %d %d", SPI_CLOCLK, SPI_MODE);
        if (run_command(cmd, 0) != 0)
        {
            printf("command(%s) error!\n", cmd);
            return FALSE;
        }
        sf_init_flag = TRUE;
    }
    return TRUE;
}

MS_BOOL read_spi_flash(MS_U8 *pBuf, MS_U32 pos, MS_U32 length)
{
    char cmd[256];

    memset(cmd, 0, sizeof(cmd));
    snprintf(cmd, sizeof(cmd), "sf read %p 0x%x 0x%x", pBuf, pos, length);
    if (run_command(cmd, 0) != 0)
    {
        printf("command(%s) error\n", cmd);
        return FALSE;
    }
    return TRUE;
}

static MS_BOOL sf_write_data_safe(MS_U32 expect_addr, MS_U8 *pdat, MS_U32 dat_len, MS_U32 *extact_addr)
{
    MS_U32 i, flash_size, align_addr;
    MS_U32 wr_begin, wr_end;
    MS_U32 sector_size = ONE_SECTOR_SIZE;
    char  cmd[256];
    MS_U8 buf[ONE_SECTOR_SIZE];

    *extact_addr = -1;
    flash_size = get_spi_flash_size();
    align_addr = ALIGN(expect_addr, sector_size);
    wr_begin = align_addr;
    wr_end   = wr_begin + sector_size;

    if (wr_end > flash_size)
    {
        UBOOT_ERROR("can not find eraseable sector (0x%x < 0x%x)!\n", (uint)flash_size, (uint)wr_end);
        return FALSE;
    }
    if (dat_len > sector_size)
    {
        UBOOT_ERROR("only support little data to write (0x%x < 0x%x)!\n", (uint)sector_size, (uint)dat_len);
        return FALSE;
    }

    if (spi_flash_erase_one_sector(wr_begin) != 0)
    {
        UBOOT_ERROR("spi_flash_erase_one_sector error!\n");
        return FALSE;
    }

    // Check if erase successfully
    memset(cmd, 0, sizeof(cmd));
    snprintf(cmd, sizeof(cmd), "sf read %p 0x%x 0x%x", buf, wr_begin, sector_size);
    if (run_command(cmd, 0) != 0)
    {
        UBOOT_ERROR("command(%s) error\n", cmd);
        return FALSE;
    }
    for (i = 0; i < sector_size; i++)
    {
        if (buf[i] != 0xFF)
        {
            UBOOT_ERROR("erase sectors error !\n");
            return FALSE;
        }
    }

    memset(cmd, 0x00, sizeof(cmd));
    memcpy(buf, pdat, dat_len);
    snprintf(cmd, sizeof(cmd), "sf write %p 0x%x 0x%x", buf, wr_begin, dat_len);
    if (run_command(cmd, 0) != 0)
    {
        UBOOT_ERROR("command(%s) error\n", cmd);
        return FALSE;
    }
    *extact_addr = wr_begin;
    return TRUE;
}

MS_BOOL mark_sf_by_mstar(MS_U32 sf_dat_oft, MS_U8 *pdat, MS_U32 dat_len)
{
    char buf[64];
    MS_U32 extact_addr, begin, end;
    struct mstar_crc_hdr hdr;

    memset(&hdr, 0, sizeof(hdr));
    memcpy(hdr.magic, DEMURA_SF_MAGIC_STRING, sizeof(hdr.magic));
    hdr.dat_offset = sf_dat_oft;
    hdr.dat_length = dat_len;
    hdr.crc_32 = MDrv_CRC32_Cal_DeMura((MS_U8 *)pdat, dat_len);

    begin = sf_dat_oft;
    end   = begin + dat_len;
    if (sf_write_data_safe(end, (MS_U8 *)&hdr, sizeof(hdr), &extact_addr) != TRUE)
    {
        UBOOT_ERROR("sf_write_data_safe error\n");
        return FALSE;
    }

    sprintf(buf, "0x%X", (uint)hdr.crc_32);
    setenv(ENV_DEMURA_MSTAR_CRC, buf);
    sprintf(buf, "0x%X", (uint)extact_addr);
    setenv(ENV_DEMURA_MSTAR_CRC_POS, buf);
    saveenv();
    return TRUE;
}

static MS_U32 get_sf_crc(MS_U32 crc_pos)
{
    MS_U8 buf[256];

    memset(buf, 0, sizeof(buf));
    if (read_spi_flash(buf, crc_pos, 256) != TRUE)
    {
        UBOOT_ERROR("read_spi_flash error\n");
        return 0xFFFFffff;
    }

    struct mstar_crc_hdr *phdr = (struct mstar_crc_hdr *)buf;
    if (strncmp(phdr->magic, DEMURA_SF_MAGIC_STRING, sizeof(phdr->magic)) != 0)
    {
        UBOOT_ERROR("Invalid demura sf header !\n");
        return 0xFFFFffff;
    }
    return phdr->crc_32;
}

MS_BOOL sf_match_by_mstar(void)
{
    char *crc_str, *cpos_str;
    MS_U32 crc_board, crc_sf, crc_pos;

    if (init_spi_flash() != TRUE)
    {
        UBOOT_ERROR("init_spi_flash error, Assume that data is match !\n");
        return TRUE;
    }

    crc_str  = getenv(ENV_DEMURA_MSTAR_CRC);
    cpos_str = getenv(ENV_DEMURA_MSTAR_CRC_POS);
    if ((crc_str == NULL) || (cpos_str == NULL))
    {
        UBOOT_ERROR("Can not find env(%s and %s), Don't match !\n", ENV_DEMURA_MSTAR_CRC, ENV_DEMURA_MSTAR_CRC_POS);
        return FALSE;
    }
    crc_pos = simple_strtoul(cpos_str, NULL, 0);
    crc_board = simple_strtoul(crc_str, NULL, 0);

    crc_sf = get_sf_crc(crc_pos);
    if (crc_sf == 0xFFFFffff)
    {
        return FALSE;
    }
    if (crc_board != crc_sf)
    {
        UBOOT_ERROR("crc_board(0x%x) != crc_sf(0x%x)\n", (uint)crc_board, (uint)crc_sf);
        return FALSE;
    }
    return TRUE;
}
#endif

MS_BOOL write_raw_data(MS_U8 *pdat, MS_U32 partition_oft, MS_U32 dat_len)
{
    MS_U32 result;
    raw_io_config_push();
    #if defined (CONFIG_NAND_FLASH) || defined(CONFIG_SPI_NAND_FLASH)
    // dynpart edb64M-nand:0x800000(KL),0x700000(UBIRO),0x100000(RTPM),-(UBI)
    // dynpart edb64M-nand:0x800000(KL),0x700000(UBIRO),0x300000(demura),0x100000(RTPM),-(UBI)
    result = raw_io_config(E_RAW_DATA_IN_NAND, DEMURA_PARTITION_NAME, UNNECESSARY_PARTITION);
    #elif defined (CONFIG_MMC)
    result = raw_io_config(E_RAW_DATA_IN_MMC,  DEMURA_PARTITION_NAME, MMC_DEFAULT_VOLUME);
    #else
    UBOOT_ERROR("Unknow storage\n");
    result = -1;
    #endif
    if (result != 0)
    {
        UBOOT_ERROR("raw_io_config Failed\n");
        raw_io_config_pop();
        return FALSE;
    }

    result = raw_write((MS_U32)pdat, partition_oft, dat_len);
    if (result != 0)
    {
        UBOOT_ERROR("raw write demura partition Failed\n");
        raw_io_config_pop();
        return FALSE;
    }
    raw_io_config_pop();
    return TRUE;
}

MS_BOOL read_raw_data(MS_U8 *pdat, MS_U32 partition_oft, MS_U32 dat_len)
{
    MS_U32 result;
    raw_io_config_push();
    #if defined (CONFIG_NAND_FLASH) || defined(CONFIG_SPI_NAND_FLASH)
    // dynpart edb64M-nand:0x800000(KL),0x700000(UBIRO),0x100000(RTPM),-(UBI)
    // dynpart edb64M-nand:0x800000(KL),0x700000(UBIRO),0x300000(demura),0x100000(RTPM),-(UBI)
    result = raw_io_config(E_RAW_DATA_IN_NAND, DEMURA_PARTITION_NAME, UNNECESSARY_PARTITION);
    #elif defined (CONFIG_MMC)
    result = raw_io_config(E_RAW_DATA_IN_MMC,  DEMURA_PARTITION_NAME, MMC_DEFAULT_VOLUME);
    #else
    UBOOT_ERROR("Unknow storage\n");
    result = -1;
    #endif
    if (result != 0)
    {
        UBOOT_ERROR("raw_io_config Failed\n");
        raw_io_config_pop();
        return FALSE;
    }

    result = raw_read((MS_U32)pdat, partition_oft, dat_len);
    if (result != 0)
    {
        UBOOT_ERROR("raw read demura partition Failed\n");
        raw_io_config_pop();
        return FALSE;
    }
    raw_io_config_pop();
    return TRUE;
}


static MS_BOOL adjust_memory(MS_PHYADDR *pmem_start, MS_PHYADDR mem_len, MS_U32 min_distance)
{
    // Get MBoot self memory infomation
    static MS_U32 has_inited = FALSE;
    static MS_PHYADDR mboot_start = 0;
    static MS_PHYADDR mboot_end   = 0;
    static MS_PHYADDR mboot_len = 0;

    UBOOT_TRACE("IN\n");
    if (has_inited == FALSE)
    {
        if (get_map_size_from_env(E_MBOOT_MEM_USAGE, NO_DEFAULT_MMAP_VALUE, &mboot_len) != 0)
            return FALSE;
        if (get_map_addr_from_env(E_MBOOT_MEM_USAGE, NO_DEFAULT_MMAP_VALUE, &mboot_start) != 0)
            return FALSE;
        mboot_end  = mboot_start + mboot_len;
        has_inited = TRUE;
    }

    MS_PHYADDR mem_start = *pmem_start;
    MS_PHYADDR mem_end   = mem_start + mem_len;

    if (mem_start <= mboot_start)
    {
        if ((mboot_start - mem_start) >= min_distance)
        {
            return TRUE;
        }
        if ((mem_end > (mboot_end + (ALIGN_1MB*2))) && ((mem_end - mboot_end - (ALIGN_1MB*2)) >= min_distance))
        {
            *pmem_start = ALIGN((mboot_end + ALIGN_1MB), ALIGN_1MB);
            UBOOT_DEBUG("Change memory_addr from 0x%lx to 0x%lx\n", mem_start, *pmem_start);
            return TRUE;
        }
    }
    else
    {
        if (mem_start < mboot_end)
        {
            if ((mem_end > (mboot_end + (ALIGN_1MB*2))) && ((mem_end - mboot_end - (ALIGN_1MB*2)) >= min_distance))
            {
                *pmem_start = ALIGN((mboot_end + ALIGN_1MB), ALIGN_1MB);
                UBOOT_DEBUG("Change memory_addr from 0x%lx to 0x%lx\n", mem_start, *pmem_start);
                return TRUE;
            }
        }
        if (mem_len >= min_distance)
        {
            return TRUE;
        }
    }

    UBOOT_TRACE("OK\n");
    return FALSE;
}


MS_BOOL get_large_memory(MS_U32 *pmem_addr, MS_U32 req_len)
{
    unsigned int i;
    MS_PHYADDR len  = 0;
    MS_PHYADDR addr = 0;
    UBOOT_MMAP_ID mpool[] = {E_LX_MEM, E_LX_MEM2, E_LX_MEM3, E_LX_MEM4 };

    UBOOT_TRACE("IN\n");
    for (i = 0; i < ARRAY_SIZE(mpool); i++)
    {
        if (get_map_size_from_env(mpool[i], NO_DEFAULT_MMAP_VALUE, &len) != 0)
        {
            // get_lenght of mpool[i] failed
            continue;
        }
        if (len >= req_len)
        {
            if (get_map_addr_from_env(mpool[i], NO_DEFAULT_MMAP_VALUE, &addr) != 0)
            {
                // get_addr of mpool[i] failed
                continue;
            }
            if ((addr < 0xFFF00000) && (adjust_memory(&addr, len, req_len) == TRUE))
            {
                UBOOT_DEBUG("Select E_LX_MEM_%d as Large memory Buffer, request length :0x%X\n", i, (uint)req_len);
                *pmem_addr = (MS_U32)PA2BA(addr);
                UBOOT_TRACE("OK\n");
                return TRUE;
            }
        }
    }
    *pmem_addr = NULL;
    UBOOT_TRACE("OK\n");
    return FALSE;
}

