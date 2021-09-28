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
#ifndef _DRV_DEVICE_INFO_H_
#define _DRV_DEVICE_INFO_H_

#include "MsTypes.h"

#include "halSERFLASH.h"

#define FLASH_IC_MX25L1655D     0x0100UL
#define FLASH_IC_MX25L3205D     0x0101UL
#define FLASH_IC_MX25L3254E     0x0102UL
#define FLASH_IC_MX25L6406E     0x0103UL
#define FLASH_IC_MX25L1606E     0x0104UL
#define FLASH_IC_MX25L12805D     0x0105UL
#define FLASH_IC_MX25L6455E     0x0106UL
#define FLASH_IC_MX25L12855E     0x0107UL
#define FLASH_IC_MX25L4006E     0x0108UL
#define FLASH_IC_MX25L8006E     0x0109UL
#define FLASH_IC_MX25L6445E     0x0106UL
#define FLASH_IC_W25Q16         0x0200UL
#define FLASH_IC_W25Q80         0x0201UL
#define FLASH_IC_W25X32         0x0202UL
#define FLASH_IC_W25Q32         0x0204UL
#define FLASH_IC_W25X64         0x0205UL
#define FLASH_IC_W25X80         0x0206UL
#define FLASH_IC_W25Q64FV       0x0207UL
#define FLASH_IC_W25Q128FV      0x0208UL
#define FLASH_IC_W25X40         0x0209UL
#define FLASH_IC_AT26DF321      0x0300UL
#define FLASH_IC_STM25P32       0x0400UL
#define FLASH_IC_EN25B32B       0x0500UL
#define FLASH_IC_EN25B64B       0x0501UL
#define FLASH_IC_S25FL032P      0x0600UL
#define FLASH_IC_S25FL064P      0x0601UL
#define FLASH_IC_S25FL128P      0x0602UL
#define FLASH_IC_S25FL128S      0x0603UL
#define FLASH_IC_S25FL008A      0x0604UL
#define FLASH_IC_S25FL164K      0x0605UL
#define FLASH_IC_S25FL128L      0x0606UL
#define FLASH_IC_EN25F10        0x0502UL
#define FLASH_IC_EN25F16        0x0503UL
#define FLASH_IC_EN25F32        0x0504UL
#define FLASH_IC_EN25F80        0x0505UL
#define FLASH_IC_EN25P16        0x0506UL
#define FLASH_IC_EN25Q32A       0x0507UL
#define FLASH_IC_EN25Q64        0x0508UL
#define FLASH_IC_EN25QA64A      0x0509UL
#define FLASH_IC_EN25Q128       0x0510UL
#define FLASH_IC_EN25F40        0x0511UL
#define FLASH_IC_EN25QH16       0x0512UL
#define FLASH_IC_NUMONYX_M25PX16     0x0700UL
#define FLASH_IC_NUMONYX_N25Q32     0x0701UL
#define FLASH_IC_NUMONYX_N25Q64     0x0702UL
#define FLASH_IC_PM25LQ032C     0x0E00UL
#define FLASH_IC_GD25S80        0x0800UL
#define FLASH_IC_GD25Q16        0x0801UL
#define FLASH_IC_GD25Q32        0x0802UL
#define FLASH_IC_GD25Q64        0x0803UL
#define FLASH_IC_GD25Q128C      0x0804UL
#define FLASH_IC_A25LM032       0x0B00UL
#define FLASH_IC_ESMT_F25L032QA     0x0900UL
#define FLASH_IC_ESMT_F25L032PA     0x0901UL
#define FLASH_IC_IS25LP64       0x0A00UL
#define FLASH_IC_IS25CQ032      0x0A01UL
#define FLASH_IC_XM25QH64A      0x0C00UL
#define FLASH_IC_XM25QH128A     0x0C01UL
#define FLASH_IC_ZB25VQ32       0x0D00UL
#define FLASH_IC_ZB25Q64A       0x0D01UL
#define FLASH_IC_BH25Q32BS      0x0E00UL
#define FLASH_IC_BH25Q64BS      0x0E01UL

#define  MID_MXIC  0xC2UL
#define  MID_WB  0xEFUL
#define  MID_ATMEL  0x1FUL
#define  MID_ST  0x20UL
#define  MID_EON  0x1CUL
#define  MID_SPAN  0x01UL
#define  MID_PMC  0x7FUL
#define  MID_GD  0xC8UL
#define  MID_MICRON  0x20UL
#define  MID_SST  0xBFUL
#define  MID_ESMT  0x8CUL
#define  MID_ISSI  0x9DUL
#define  MID_ZB  0x5EUL
#define  MID_XM  0x20UL
#define  MID_NUMONYX  0x20UL
#define  MID_AM  0x37UL
#define  MID_BH  0x68UL

#define SIZE_1KB    0x400UL
#define SIZE_2KB    0x800UL
#define SIZE_4KB    0x1000UL
#define SIZE_8KB    0x2000UL
#define SIZE_16KB    0x4000UL
#define SIZE_32KB    0x8000UL
#define SIZE_64KB    0x10000UL
#define SIZE_128KB    0x20000UL
#define SIZE_256KB    0x40000UL
#define SIZE_512KB    0x80000UL
#define SIZE_1MB    0x100000UL
#define SIZE_2MB    0x200000UL
#define SIZE_4MB    0x400000UL
#define SIZE_8MB    0x800000UL
#define SIZE_16MB    0x1000000UL

#ifndef UNUSED
#define UNUSED(x) ((x)=(x))
#endif
#define ISP_DEV_SEL             ISP_DEV_NEXTFLASH
#define ISP_SPI_ENDIAN_SEL      ISP_SPI_ENDIAN_LITTLE
#define ISP_DEV_PMC             BITS(2:0, 0)
#define ISP_DEV_NEXTFLASH       BITS(2:0, 1)
#define ISP_DEV_ST              BITS(2:0, 2)
#define ISP_DEV_SST             BITS(2:0, 3)
#define ISP_DEV_ATMEL           BITS(2:0, 4)
#define ISP_SPI_ENDIAN_BIG      BITS(0:0, 1)
#define ISP_SPI_ENDIAN_LITTLE   BITS(0:0, 0)
#define SERFLASH_PAGE_SIZE                  256
#define SERFLASH_MAX_CHIP_WR_DONE_TIMEOUT   400 /*Experience timming*/
#define SERFLASH_WRSR_BLK_PROTECT           (_hal_SERFLASH.u8WrsrBlkProtect)
#define SERFLASH_PROTECT_TABLE              (_hal_SERFLASH.u16ProtectTbl_Size)
#define WAIT_SFSH_CS_STAT()                 {while(ISP_READ(REG_ISP_SPI_CHIP_SELE_BUSY) == SFSH_CHIP_SELE_SWITCH){}}

typedef   enum   {
     E_NORMAL_MODE,
     E_DUAL_D_MODE,
     E_DUAL_AD_MODE,
     E_QUAD_D_MODE,
     E_QUAD_AD_MODE,
}SPI_READ_MODE;

static ST_SPECIAL_BLOCKS _stSpecialBlocks_EN25F10 =
{
    .u16Start     = 0,
    .u16End       = 3,
    .au32SizeList =
    { 
        SIZE_32KB,
        SIZE_32KB,
        SIZE_32KB,
        SIZE_32KB,
     }
};

static ST_SPECIAL_BLOCKS _stSpecialBlocks_EN25B32B =
{
    .u16Start     = 0,
    .u16End       = 3,
    .au32SizeList =
    { 
        SIZE_4KB,
        SIZE_4KB,
        SIZE_8KB,
        SIZE_16KB,
        SIZE_32KB,
     }
};

static ST_SPECIAL_BLOCKS _stSpecialBlocks_EN25B64B =
{
    .u16Start     = 0,
    .u16End       = 3,
    .au32SizeList =
    { 
        SIZE_4KB,
        SIZE_4KB,
        SIZE_8KB,
        SIZE_16KB,
        SIZE_32KB,
     }
};

#define FROM_BLK(x, blk_size)   ((x) * (blk_size))
#define TO_BLK(x, blk_size)     (((x) + 1) * (blk_size) - 1)



static ST_WRITE_PROTECT   _pstWriteProtectTable_W25Q32[] =
{
    {  BITS(5:2, 1),  0x003F0000,        0x003FFFFF            },
    {  BITS(5:2, 2),  0x003E0000,        0x003FFFFF            },
    {  BITS(5:2, 3),  0x003C0000,        0x003FFFFF            },
    {  BITS(5:2, 4),  0x00380000,        0x003FFFFF            },
    {  BITS(5:2, 5),  0x00300000,        0x003FFFFF            },
    {  BITS(5:2, 6),  0x00200000,        0x003FFFFF            },
    {  BITS(5:2, 9),  0x00000000,        0x0000FFFF            },
    {  BITS(5:2, 10),  0x00000000,        0x0001FFFF            },
    {  BITS(5:2, 11),  0x00000000,        0x0003FFFF            },
    {  BITS(5:2, 12),  0x00000000,        0x0007FFFF            },
    {  BITS(5:2, 13),  0x00000000,        0x000FFFFF            },
    {  BITS(5:2, 14),  0x00000000,        0x001FFFFF            },
    {  BITS(5:2, 15),  0x00000000,        0x003FFFFF            },
    {  BITS(5:2, 0),  0xFFFFFFFF,        0xFFFFFFFF            },
};

static ST_WRITE_PROTECT   _pstWriteProtectTable_W25X32[] =
{
    {  BITS(5:2, 1),  0x003F0000,        0x003FFFFF            },
    {  BITS(5:2, 2),  0x003E0000,        0x003FFFFF            },
    {  BITS(5:2, 3),  0x003C0000,        0x003FFFFF            },
    {  BITS(5:2, 4),  0x00380000,        0x003FFFFF            },
    {  BITS(5:2, 5),  0x00300000,        0x003FFFFF            },
    {  BITS(5:2, 6),  0x00200000,        0x003FFFFF            },
    {  BITS(5:2, 9),  0x00000000,        0x0000FFFF            },
    {  BITS(5:2, 10),  0x00000000,        0x0001FFFF            },
    {  BITS(5:2, 11),  0x00000000,        0x0003FFFF            },
    {  BITS(5:2, 12),  0x00000000,        0x0007FFFF            },
    {  BITS(5:2, 13),  0x00000000,        0x000FFFFF            },
    {  BITS(5:2, 14),  0x00000000,        0x001FFFFF            },
    {  BITS(5:2, 15),  0x00000000,        0x003FFFFF            },
    {  BITS(5:2, 0),  0xFFFFFFFF,        0xFFFFFFFF            },
};

static ST_WRITE_PROTECT   _pstWriteProtectTable_W25X64[] =
{
    {  BITS(5:2, 1),  0x007E0000,        0x007FFFFF            },
    {  BITS(5:2, 2),  0x007C0000,        0x007FFFFF            },
    {  BITS(5:2, 3),  0x00780000,        0x007FFFFF            },
    {  BITS(5:2, 4),  0x00700000,        0x007FFFFF            },
    {  BITS(5:2, 5),  0x00600000,        0x007FFFFF            },
    {  BITS(5:2, 6),  0x00400000,        0x007FFFFF            },
    {  BITS(5:2, 9),  0x00000000,        0x0001FFFF            },
    {  BITS(5:2, 10),  0x00000000,        0x0003FFFF            },
    {  BITS(5:2, 11),  0x00000000,        0x0007FFFF            },
    {  BITS(5:2, 12),  0x00000000,        0x000FFFFF            },
    {  BITS(5:2, 13),  0x00000000,        0x001FFFFF            },
    {  BITS(5:2, 14),  0x00000000,        0x003FFFFF            },
    {  BITS(5:2, 15),  0x00000000,        0x007FFFFF            },
    {  BITS(5:2, 0),  0xFFFFFFFF,        0xFFFFFFFF            },
};

static ST_WRITE_PROTECT   _pstWriteProtectTable_W25Q128FV[] =
{
    {  BITS(5:2, 1),  0x00FC0000,        0x00FFFFFF            },
    {  BITS(5:2, 2),  0x00F80000,        0x00FFFFFF            },
    {  BITS(5:2, 3),  0x00F00000,        0x00FFFFFF            },
    {  BITS(5:2, 4),  0x00E00000,        0x00FFFFFF            },
    {  BITS(5:2, 5),  0x00C00000,        0x00FFFFFF            },
    {  BITS(5:2, 6),  0x00800000,        0x00FFFFFF            },
    {  BITS(5:2, 9),  0x00000000,        0x0003FFFF            },
    {  BITS(5:2, 10),  0x00000000,        0x0007FFFF            },
    {  BITS(5:2, 11),  0x00000000,        0x000FFFFF            },
    {  BITS(5:2, 12),  0x00000000,        0x001FFFFF            },
    {  BITS(5:2, 13),  0x00000000,        0x003FFFFF            },
    {  BITS(5:2, 14),  0x00000000,        0x007FFFFF            },
    {  BITS(5:2, 15),  0x00000000,        0x00FFFFFF            },
    {  BITS(5:2, 0),  0xFFFFFFFF,        0xFFFFFFFF            },
};

static ST_WRITE_PROTECT   _pstWriteProtectTable_W25Q64FV[] =
{
    {  BITS(5:2, 1),  0x007E0000,        0x007FFFFF            },
    {  BITS(5:2, 2),  0x007C0000,        0x007FFFFF            },
    {  BITS(5:2, 3),  0x00780000,        0x007FFFFF            },
    {  BITS(5:2, 4),  0x00700000,        0x007FFFFF            },
    {  BITS(5:2, 5),  0x00600000,        0x007FFFFF            },
    {  BITS(5:2, 6),  0x00400000,        0x007FFFFF            },
    {  BITS(5:2, 9),  0x00000000,        0x0001FFFF            },
    {  BITS(5:2, 10),  0x00000000,        0x0003FFFF            },
    {  BITS(5:2, 11),  0x00000000,        0x0007FFFF            },
    {  BITS(5:2, 12),  0x00000000,        0x000FFFFF            },
    {  BITS(5:2, 13),  0x00000000,        0x001FFFFF            },
    {  BITS(5:2, 14),  0x00000000,        0x003FFFFF            },
    {  BITS(5:2, 15),  0x00000000,        0x007FFFFF            },
    {  BITS(5:2, 0),  0xFFFFFFFF,        0xFFFFFFFF            },
};

static ST_WRITE_PROTECT   _pstWriteProtectTable_MX25L3205D[] =
{
    {  BITS(5:2, 1),  FROM_BLK(63, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(5:2, 2),  FROM_BLK(62, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(5:2, 3),  FROM_BLK(60, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(5:2, 4),  FROM_BLK(56, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(5:2, 5),  FROM_BLK(48, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(5:2, 6),  FROM_BLK(32, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(5:2, 9),  FROM_BLK(0, SIZE_64KB),  TO_BLK(31, SIZE_64KB)  },
    {  BITS(5:2, 10),  FROM_BLK(0, SIZE_64KB),  TO_BLK(47, SIZE_64KB)  },
    {  BITS(5:2, 11),  FROM_BLK(0, SIZE_64KB),  TO_BLK(55, SIZE_64KB)  },
    {  BITS(5:2, 12),  FROM_BLK(0, SIZE_64KB),  TO_BLK(59, SIZE_64KB)  },
    {  BITS(5:2, 13),  FROM_BLK(0, SIZE_64KB),  TO_BLK(61, SIZE_64KB)  },
    {  BITS(5:2, 14),  FROM_BLK(0, SIZE_64KB),  TO_BLK(62, SIZE_64KB)  },
    {  BITS(5:2, 15),  FROM_BLK(0, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(5:2, 0),  0xFFFFFFFF,        0xFFFFFFFF            },
};
static ST_WRITE_PROTECT   _pstWriteProtectTable_MX25L3254E[] =
{
    {  BITS(5:2, 1),  FROM_BLK(63, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(5:2, 2),  FROM_BLK(62, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(5:2, 3),  FROM_BLK(60, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(5:2, 4),  FROM_BLK(56, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(5:2, 5),  FROM_BLK(48, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(5:2, 6),  FROM_BLK(32, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(5:2, 7),  FROM_BLK(0, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(5:2, 8),  FROM_BLK(0, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(5:2, 9),  FROM_BLK(0, SIZE_64KB),  TO_BLK(31, SIZE_64KB)  },
    {  BITS(5:2, 10),  FROM_BLK(0, SIZE_64KB),  TO_BLK(47, SIZE_64KB)  },
    {  BITS(5:2, 11),  FROM_BLK(0, SIZE_64KB),  TO_BLK(55, SIZE_64KB)  },
    {  BITS(5:2, 12),  FROM_BLK(0, SIZE_64KB),  TO_BLK(59, SIZE_64KB)  },
    {  BITS(5:2, 13),  FROM_BLK(0, SIZE_64KB),  TO_BLK(61, SIZE_64KB)  },
    {  BITS(5:2, 14),  FROM_BLK(0, SIZE_64KB),  TO_BLK(62, SIZE_64KB)  },
    {  BITS(5:2, 15),  FROM_BLK(0, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(5:2, 0),  0xFFFFFFFF,        0xFFFFFFFF            },
};
static ST_WRITE_PROTECT   _pstWriteProtectTable_GD25Q16[] =
{
    {  BITS(6:2, 1),  0x001F0000,        0x001FFFFF            },
    {  BITS(6:2, 2),  0x001E0000,        0x001FFFFF            },
    {  BITS(6:2, 3),  0x001C0000,        0x001FFFFF            },
    {  BITS(6:2, 4),  0x00180000,        0x001FFFFF            },
    {  BITS(6:2, 5),  0x00100000,        0x001FFFFF            },
    {  BITS(6:2, 9),  0x00000000,        0x0000FFFF            },
    {  BITS(6:2, 10),  0x00000000,        0x0001FFFF            },
    {  BITS(6:2, 11),  0x00000000,        0x0003FFFF            },
    {  BITS(6:2, 12),  0x00000000,        0x0007FFFF            },
    {  BITS(6:2, 13),  0x00000000,        0x000FFFFF            },
    {  BITS(6:2, 14),  0x00000000,        0x001FFFFF            },
    {  BITS(6:2, 17),  0x1FF000,        0x1FFFFF            },
    {  BITS(6:2, 18),  0x1FE000,        0x1FFFFF            },
    {  BITS(6:2, 19),  0x1FC000,        0x1FFFFF            },
    {  BITS(6:2, 20),  0x1F8000,        0x1FFFFF            },
    {  BITS(6:2, 21),  0x1F8000,        0x1FFFFF            },
    {  BITS(6:2, 25),  0x000000,        0x001FFF            },
    {  BITS(6:2, 26),  0x000000,        0x001FFF            },
    {  BITS(6:2, 27),  0x000000,        0x003FFF            },
    {  BITS(6:2, 28),  0x000000,        0x007FFF            },
    {  BITS(6:2, 29),  0x000000,        0x007FFF            },
    {  BITS(6:2, 0),  0xFFFFFFFF,        0xFFFFFFFF            },
};

static ST_WRITE_PROTECT   _pstWriteProtectTable_GD25Q128C[] =
{
    {  BITS(6:2, 1),  0x00FC0000,        0x00FFFFFF            },
    {  BITS(6:2, 2),  0x00F80000,        0x00FFFFFF            },
    {  BITS(6:2, 3),  0x00F00000,        0x00FFFFFF            },
    {  BITS(6:2, 4),  0x00E00000,        0x00FFFFFF            },
    {  BITS(6:2, 5),  0x00C00000,        0x00FFFFFF            },
    {  BITS(6:2, 6),  0x00800000,        0x0000FFFF            },
    {  BITS(6:2, 9),  0x00000000,        0x0003FFFF            },
    {  BITS(6:2, 10),  0x00000000,        0x0007FFFF            },
    {  BITS(6:2, 11),  0x00000000,        0x000FFFFF            },
    {  BITS(6:2, 12),  0x00000000,        0x001FFFFF            },
    {  BITS(6:2, 13),  0x00000000,        0x003FFFFF            },
    {  BITS(6:2, 14),  0x00000000,        0x007FFFFF            },
    {  BITS(6:2, 15),  0x00000000,        0x00FFFFFF            },
    {  BITS(6:2, 17),  0xFFF000,        0xFFFFFF            },
    {  BITS(6:2, 18),  0xFFE000,        0xFFFFFF            },
    {  BITS(6:2, 19),  0xFFC000,        0xFFFFFF            },
    {  BITS(6:2, 20),  0xFF8000,        0xFFFFFF            },
    {  BITS(6:2, 21),  0xFF8000,        0xFFFFFF            },
    {  BITS(6:2, 22),  0xFF8000,        0xFFFFFF            },
    {  BITS(6:2, 25),  0x000000,        0x000FFF            },
    {  BITS(6:2, 26),  0x000000,        0x001FFF            },
    {  BITS(6:2, 27),  0x000000,        0x003FFF            },
    {  BITS(6:2, 28),  0x000000,        0x007FFF            },
    {  BITS(6:2, 29),  0x000000,        0x007FFF            },
    {  BITS(6:2, 30),  0x000000,        0x007FFF            },
    {  BITS(6:2, 0),  0xFFFFFFFF,        0xFFFFFFFF            },
};

static ST_WRITE_PROTECT   _pstWriteProtectTable_MX25L6406E[] =
{
    {  BITS(5:2, 1),  FROM_BLK(126, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 2),  FROM_BLK(124, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 3),  FROM_BLK(120, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 4),  FROM_BLK(112, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 5),  FROM_BLK(96, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 6),  FROM_BLK(64, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 9),  FROM_BLK(0, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(5:2, 10),  FROM_BLK(0, SIZE_64KB),  TO_BLK(95, SIZE_64KB)  },
    {  BITS(5:2, 11),  FROM_BLK(0, SIZE_64KB),  TO_BLK(111, SIZE_64KB)  },
    {  BITS(5:2, 12),  FROM_BLK(0, SIZE_64KB),  TO_BLK(119, SIZE_64KB)  },
    {  BITS(5:2, 13),  FROM_BLK(0, SIZE_64KB),  TO_BLK(123, SIZE_64KB)  },
    {  BITS(5:2, 14),  FROM_BLK(0, SIZE_64KB),  TO_BLK(125, SIZE_64KB)  },
    {  BITS(5:2, 15),  FROM_BLK(0, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 0),  0xFFFFFFFF,        0xFFFFFFFF            },
};
static ST_WRITE_PROTECT   _pstWriteProtectTable_MX25L1606E[] =
{
    {  BITS(5:2, 1),  FROM_BLK(31, SIZE_64KB),  TO_BLK(31, SIZE_64KB)  },
    {  BITS(5:2, 2),  FROM_BLK(30, SIZE_64KB),  TO_BLK(31, SIZE_64KB)  },
    {  BITS(5:2, 3),  FROM_BLK(28, SIZE_64KB),  TO_BLK(31, SIZE_64KB)  },
    {  BITS(5:2, 4),  FROM_BLK(24, SIZE_64KB),  TO_BLK(31, SIZE_64KB)  },
    {  BITS(5:2, 5),  FROM_BLK(16, SIZE_64KB),  TO_BLK(31, SIZE_64KB)  },
    {  BITS(5:2, 6),  FROM_BLK(0, SIZE_64KB),  TO_BLK(31, SIZE_64KB)  },
    {  BITS(5:2, 7),  FROM_BLK(0, SIZE_64KB),  TO_BLK(31, SIZE_64KB)  },
    {  BITS(5:2, 8),  FROM_BLK(0, SIZE_64KB),  TO_BLK(31, SIZE_64KB)  },
    {  BITS(5:2, 9),  FROM_BLK(0, SIZE_64KB),  TO_BLK(31, SIZE_64KB)  },
    {  BITS(5:2, 10),  FROM_BLK(0, SIZE_64KB),  TO_BLK(15, SIZE_64KB)  },
    {  BITS(5:2, 11),  FROM_BLK(0, SIZE_64KB),  TO_BLK(23, SIZE_64KB)  },
    {  BITS(5:2, 12),  FROM_BLK(0, SIZE_64KB),  TO_BLK(27, SIZE_64KB)  },
    {  BITS(5:2, 13),  FROM_BLK(0, SIZE_64KB),  TO_BLK(29, SIZE_64KB)  },
    {  BITS(5:2, 14),  FROM_BLK(0, SIZE_64KB),  TO_BLK(30, SIZE_64KB)  },
    {  BITS(5:2, 15),  FROM_BLK(0, SIZE_64KB),  TO_BLK(31, SIZE_64KB)  },
    {  BITS(5:2, 0),  0xFFFFFFFF,        0xFFFFFFFF            },
};
static ST_WRITE_PROTECT   _pstWriteProtectTable_MX25L12805D[] =
{
    {  BITS(5:2, 1),  FROM_BLK(255, SIZE_64KB),  TO_BLK(255, SIZE_64KB)  },
    {  BITS(5:2, 2),  FROM_BLK(254, SIZE_64KB),  TO_BLK(255, SIZE_64KB)  },
    {  BITS(5:2, 3),  FROM_BLK(252, SIZE_64KB),  TO_BLK(255, SIZE_64KB)  },
    {  BITS(5:2, 4),  FROM_BLK(248, SIZE_64KB),  TO_BLK(255, SIZE_64KB)  },
    {  BITS(5:2, 5),  FROM_BLK(240, SIZE_64KB),  TO_BLK(255, SIZE_64KB)  },
    {  BITS(5:2, 6),  FROM_BLK(224, SIZE_64KB),  TO_BLK(255, SIZE_64KB)  },
    {  BITS(5:2, 7),  FROM_BLK(192, SIZE_64KB),  TO_BLK(255, SIZE_64KB)  },
    {  BITS(5:2, 8),  FROM_BLK(128, SIZE_64KB),  TO_BLK(255, SIZE_64KB)  },
    {  BITS(5:2, 15),  FROM_BLK(0, SIZE_64KB),  TO_BLK(255, SIZE_64KB)  },
    {  BITS(5:2, 0),  0xFFFFFFFF,        0xFFFFFFFF            },
};
static ST_WRITE_PROTECT   _pstWriteProtectTable_MX25L12855E[] =
{
    {  BITS(5:2, 1),  FROM_BLK(254, SIZE_64KB),  TO_BLK(255, SIZE_64KB)  },
    {  BITS(5:2, 2),  FROM_BLK(252, SIZE_64KB),  TO_BLK(255, SIZE_64KB)  },
    {  BITS(5:2, 3),  FROM_BLK(248, SIZE_64KB),  TO_BLK(255, SIZE_64KB)  },
    {  BITS(5:2, 4),  FROM_BLK(240, SIZE_64KB),  TO_BLK(255, SIZE_64KB)  },
    {  BITS(5:2, 5),  FROM_BLK(192, SIZE_64KB),  TO_BLK(255, SIZE_64KB)  },
    {  BITS(5:2, 6),  FROM_BLK(128, SIZE_64KB),  TO_BLK(255, SIZE_64KB)  },
    {  BITS(5:2, 7),  FROM_BLK(0, SIZE_64KB),  TO_BLK(255, SIZE_64KB)  },
    {  BITS(5:2, 8),  FROM_BLK(0, SIZE_64KB),  TO_BLK(255, SIZE_64KB)  },
    {  BITS(5:2, 9),  FROM_BLK(0, SIZE_64KB),  TO_BLK(255, SIZE_64KB)  },
    {  BITS(5:2, 10),  FROM_BLK(0, SIZE_64KB),  TO_BLK(255, SIZE_64KB)  },
    {  BITS(5:2, 11),  FROM_BLK(0, SIZE_64KB),  TO_BLK(255, SIZE_64KB)  },
    {  BITS(5:2, 12),  FROM_BLK(0, SIZE_64KB),  TO_BLK(255, SIZE_64KB)  },
    {  BITS(5:2, 13),  FROM_BLK(0, SIZE_64KB),  TO_BLK(255, SIZE_64KB)  },
    {  BITS(5:2, 14),  FROM_BLK(0, SIZE_64KB),  TO_BLK(255, SIZE_64KB)  },
    {  BITS(5:2, 15),  FROM_BLK(0, SIZE_64KB),  TO_BLK(255, SIZE_64KB)  },
    {  BITS(5:2, 0),  0xFFFFFFFF,        0xFFFFFFFF            },
};
static ST_WRITE_PROTECT   _pstWriteProtectTable_MX25L6455E[] =
{
    {  BITS(5:2, 1),  FROM_BLK(126, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 2),  FROM_BLK(124, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 3),  FROM_BLK(120, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 4),  FROM_BLK(112, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 5),  FROM_BLK(96, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 6),  FROM_BLK(64, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 7),  FROM_BLK(0, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 8),  FROM_BLK(0, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 9),  FROM_BLK(0, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 10),  FROM_BLK(0, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 11),  FROM_BLK(0, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 12),  FROM_BLK(0, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 13),  FROM_BLK(0, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 14),  FROM_BLK(0, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 15),  FROM_BLK(0, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 0),  0xFFFFFFFF,        0xFFFFFFFF            },
};
static ST_WRITE_PROTECT   _pstWriteProtectTable_IS25LP64[] =
{
    {  BITS(5:2, 1),  FROM_BLK(127, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 2),  FROM_BLK(126, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 3),  FROM_BLK(124, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 4),  FROM_BLK(120, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 5),  FROM_BLK(112, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 6),  FROM_BLK(96, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 7),  FROM_BLK(64, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 8),  FROM_BLK(0, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 9),  FROM_BLK(0, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 10),  FROM_BLK(0, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 11),  FROM_BLK(0, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 12),  FROM_BLK(0, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 13),  FROM_BLK(0, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 14),  FROM_BLK(0, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 15),  FROM_BLK(0, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 0),  0xFFFFFFFF,        0xFFFFFFFF            },
};
static ST_WRITE_PROTECT   _pstWriteProtectTable_AT26DF321[] =
{
    {  BITS(5:2, 1),  FROM_BLK(63, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(5:2, 2),  FROM_BLK(62, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(5:2, 3),  FROM_BLK(60, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(5:2, 4),  FROM_BLK(56, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(5:2, 5),  FROM_BLK(48, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(5:2, 6),  FROM_BLK(32, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(5:2, 9),  FROM_BLK(0, SIZE_64KB),  TO_BLK(31, SIZE_64KB)  },
    {  BITS(5:2, 10),  FROM_BLK(0, SIZE_64KB),  TO_BLK(47, SIZE_64KB)  },
    {  BITS(5:2, 11),  FROM_BLK(0, SIZE_64KB),  TO_BLK(55, SIZE_64KB)  },
    {  BITS(5:2, 12),  FROM_BLK(0, SIZE_64KB),  TO_BLK(59, SIZE_64KB)  },
    {  BITS(5:2, 13),  FROM_BLK(0, SIZE_64KB),  TO_BLK(61, SIZE_64KB)  },
    {  BITS(5:2, 14),  FROM_BLK(0, SIZE_64KB),  TO_BLK(62, SIZE_64KB)  },
    {  BITS(5:2, 15),  FROM_BLK(0, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(5:2, 0),  0xFFFFFFFF,        0xFFFFFFFF            },
};
static ST_WRITE_PROTECT   _pstWriteProtectTable_S25FL032P[] =
{
    {  BITS(4:2, 1),  FROM_BLK(63, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(4:2, 2),  FROM_BLK(62, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(4:2, 3),  FROM_BLK(60, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(4:2, 4),  FROM_BLK(56, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(4:2, 5),  FROM_BLK(48, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(4:2, 6),  FROM_BLK(32, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(4:2, 7),  FROM_BLK(0, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(4:2, 0),  0xFFFFFFFF,        0xFFFFFFFF            },
};
static ST_WRITE_PROTECT   _pstWriteProtectTable_S25FL064P[] =
{
    {  BITS(4:2, 1),  FROM_BLK(64, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(4:2, 2),  FROM_BLK(96, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(4:2, 3),  FROM_BLK(112, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(4:2, 4),  FROM_BLK(120, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(4:2, 5),  FROM_BLK(124, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(4:2, 6),  FROM_BLK(126, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(4:2, 7),  FROM_BLK(0, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(4:2, 0),  0xFFFFFFFF,        0xFFFFFFFF            },
};
static ST_WRITE_PROTECT   _pstWriteProtectTable_S25FL128P[] =
{
    {  BITS(5:2, 1),  FROM_BLK(254, SIZE_64KB),  TO_BLK(255, SIZE_64KB)  },
    {  BITS(5:2, 2),  FROM_BLK(252, SIZE_64KB),  TO_BLK(255, SIZE_64KB)  },
    {  BITS(5:2, 3),  FROM_BLK(248, SIZE_64KB),  TO_BLK(255, SIZE_64KB)  },
    {  BITS(5:2, 4),  FROM_BLK(240, SIZE_64KB),  TO_BLK(255, SIZE_64KB)  },
    {  BITS(5:2, 5),  FROM_BLK(224, SIZE_64KB),  TO_BLK(255, SIZE_64KB)  },
    {  BITS(5:2, 6),  FROM_BLK(192, SIZE_64KB),  TO_BLK(255, SIZE_64KB)  },
    {  BITS(5:2, 7),  FROM_BLK(128, SIZE_64KB),  TO_BLK(255, SIZE_64KB)  },
    {  BITS(5:2, 15),  FROM_BLK(0, SIZE_64KB),  TO_BLK(255, SIZE_64KB)  },
    {  BITS(5:2, 0),  0xFFFFFFFF,        0xFFFFFFFF            },
};
static ST_WRITE_PROTECT   _pstWriteProtectTable_S25FL128L[] =
{
    {  BITS(5:2, 1),  FROM_BLK(252, SIZE_64KB),  TO_BLK(255, SIZE_64KB)  },
    {  BITS(5:2, 2),  FROM_BLK(248, SIZE_64KB),  TO_BLK(255, SIZE_64KB)  },
    {  BITS(5:2, 3),  FROM_BLK(240, SIZE_64KB),  TO_BLK(255, SIZE_64KB)  },
    {  BITS(5:2, 4),  FROM_BLK(224, SIZE_64KB),  TO_BLK(255, SIZE_64KB)  },
    {  BITS(5:2, 5),  FROM_BLK(192, SIZE_64KB),  TO_BLK(255, SIZE_64KB)  },
    {  BITS(5:2, 6),  FROM_BLK(128, SIZE_64KB),  TO_BLK(255, SIZE_64KB)  },
    {  BITS(5:2, 7),  FROM_BLK(0, SIZE_64KB),  TO_BLK(255, SIZE_64KB)  },
    {  BITS(5:2, 15),  FROM_BLK(0, SIZE_64KB),  TO_BLK(255, SIZE_64KB)  },
    {  BITS(5:2, 0),  0xFFFFFFFF,        0xFFFFFFFF            },
};
static ST_WRITE_PROTECT   _pstWriteProtectTable_EN25P16[] =
{
    {  BITS(4:2, 1),  0x001F0000,        0x001FFFFF            },
    {  BITS(4:2, 2),  0x001E0000,        0x001FFFFF            },
    {  BITS(4:2, 3),  0x001C0000,        0x001FFFFF            },
    {  BITS(4:2, 4),  0x00180000,        0x001FFFFF            },
    {  BITS(4:2, 5),  0x00100000,        0x001FFFFF            },
    {  BITS(4:2, 6),  0x00000000,        0x001FFFFF            },
    {  BITS(4:2, 7),  0x00000000,        0x001FFFFF            },
    {  BITS(4:2, 0),  0xFFFFFFFF,        0xFFFFFFFF            },
};

static ST_WRITE_PROTECT   _pstWriteProtectTable_EN25F16[] =
{
    {  BITS(4:2, 1),  0x001F0000,        0x001FFFFF            },
    {  BITS(4:2, 2),  0x001E0000,        0x001FFFFF            },
    {  BITS(4:2, 3),  0x001C0000,        0x001FFFFF            },
    {  BITS(4:2, 4),  0x00180000,        0x001FFFFF            },
    {  BITS(4:2, 5),  0x00100000,        0x001FFFFF            },
    {  BITS(4:2, 6),  0x00000000,        0x001FFFFF            },
    {  BITS(4:2, 7),  0x00000000,        0x001FFFFF            },
    {  BITS(4:2, 0),  0xFFFFFFFF,        0xFFFFFFFF            },
};

static ST_WRITE_PROTECT   _pstWriteProtectTable_EN25F80[] =
{
    {  BITS(4:2, 1),  0x000F0000,        0x000FFFFF            },
    {  BITS(4:2, 2),  0x000E0000,        0x000FFFFF            },
    {  BITS(4:2, 3),  0x000C0000,        0x000FFFFF            },
    {  BITS(4:2, 4),  0x00080000,        0x000FFFFF            },
    {  BITS(4:2, 5),  0x00000000,        0x000FFFFF            },
    {  BITS(4:2, 6),  0x00000000,        0x000FFFFF            },
    {  BITS(4:2, 7),  0x00000000,        0x000FFFFF            },
    {  BITS(4:2, 0),  0xFFFFFFFF,        0xFFFFFFFF            },
};

static ST_WRITE_PROTECT   _pstWriteProtectTable_EN25F32[] =
{
    {  BITS(5:2, 1),  0x00000000,        0x003EFFFF            },
    {  BITS(5:2, 2),  0x00000000,        0x003DFFFF            },
    {  BITS(5:2, 3),  0x00000000,        0x003BFFFF            },
    {  BITS(5:2, 4),  0x00000000,        0x0037FFFF            },
    {  BITS(5:2, 5),  0x00000000,        0x002FFFFF            },
    {  BITS(5:2, 6),  0x00000000,        0x001FFFFF            },
    {  BITS(5:2, 7),  0x00000000,        0x003FFFFF            },
    {  BITS(5:2, 9),  0x003FFFFF,        0x00010000            },
    {  BITS(5:2, 10),  0x003FFFFF,        0x00020000            },
    {  BITS(5:2, 11),  0x003FFFFF,        0x00040000            },
    {  BITS(5:2, 12),  0x003FFFFF,        0x00080000            },
    {  BITS(5:2, 13),  0x003FFFFF,        0x00100000            },
    {  BITS(5:2, 14),  0x003FFFFF,        0x00200000            },
    {  BITS(5:2, 15),  0x00000000,        0x003FFFFF            },
    {  BITS(5:2, 0),  0xFFFFFFFF,        0xFFFFFFFF            },
};

static ST_WRITE_PROTECT   _pstWriteProtectTable_EN25Q32A[] =
{
    {  BITS(5:2, 1),  FROM_BLK(0, SIZE_64KB),  TO_BLK(62, SIZE_64KB)  },
    {  BITS(5:2, 2),  FROM_BLK(0, SIZE_64KB),  TO_BLK(61, SIZE_64KB)  },
    {  BITS(5:2, 3),  FROM_BLK(0, SIZE_64KB),  TO_BLK(59, SIZE_64KB)  },
    {  BITS(5:2, 4),  FROM_BLK(0, SIZE_64KB),  TO_BLK(55, SIZE_64KB)  },
    {  BITS(5:2, 5),  FROM_BLK(0, SIZE_64KB),  TO_BLK(47, SIZE_64KB)  },
    {  BITS(5:2, 6),  FROM_BLK(0, SIZE_64KB),  TO_BLK(31, SIZE_64KB)  },
    {  BITS(5:2, 7),  FROM_BLK(0, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(5:2, 8),  FROM_BLK(0, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(5:2, 9),  FROM_BLK(1, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(5:2, 10),  FROM_BLK(2, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(5:2, 11),  FROM_BLK(4, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(5:2, 12),  FROM_BLK(8, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(5:2, 13),  FROM_BLK(16, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(5:2, 14),  FROM_BLK(32, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(5:2, 15),  FROM_BLK(0, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(5:2, 0),  0xFFFFFFFF,        0xFFFFFFFF            },
};
static ST_WRITE_PROTECT   _pstWriteProtectTable_EN25Q64[] =
{
    {  BITS(5:2, 1),  FROM_BLK(0, SIZE_64KB),  TO_BLK(126, SIZE_64KB)  },
    {  BITS(5:2, 2),  FROM_BLK(0, SIZE_64KB),  TO_BLK(125, SIZE_64KB)  },
    {  BITS(5:2, 3),  FROM_BLK(0, SIZE_64KB),  TO_BLK(123, SIZE_64KB)  },
    {  BITS(5:2, 4),  FROM_BLK(0, SIZE_64KB),  TO_BLK(119, SIZE_64KB)  },
    {  BITS(5:2, 5),  FROM_BLK(0, SIZE_64KB),  TO_BLK(111, SIZE_64KB)  },
    {  BITS(5:2, 6),  FROM_BLK(0, SIZE_64KB),  TO_BLK(95, SIZE_64KB)  },
    {  BITS(5:2, 7),  FROM_BLK(0, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 9),  FROM_BLK(1, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 10),  FROM_BLK(2, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 11),  FROM_BLK(4, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 12),  FROM_BLK(8, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 13),  FROM_BLK(16, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 14),  FROM_BLK(32, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 15),  FROM_BLK(0, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 0),  0xFFFFFFFF,        0xFFFFFFFF            },
};
static ST_WRITE_PROTECT   _pstWriteProtectTable_EN25QA64A[] =
{
    {  BITS(5:2, 1),  FROM_BLK(0, SIZE_64KB),  TO_BLK(126, SIZE_64KB)  },
    {  BITS(5:2, 2),  FROM_BLK(0, SIZE_64KB),  TO_BLK(125, SIZE_64KB)  },
    {  BITS(5:2, 3),  FROM_BLK(0, SIZE_64KB),  TO_BLK(123, SIZE_64KB)  },
    {  BITS(5:2, 4),  FROM_BLK(0, SIZE_64KB),  TO_BLK(119, SIZE_64KB)  },
    {  BITS(5:2, 5),  FROM_BLK(0, SIZE_64KB),  TO_BLK(111, SIZE_64KB)  },
    {  BITS(5:2, 6),  FROM_BLK(0, SIZE_64KB),  TO_BLK(95, SIZE_64KB)  },
    {  BITS(5:2, 7),  FROM_BLK(0, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 9),  FROM_BLK(1, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 10),  FROM_BLK(2, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 11),  FROM_BLK(4, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 12),  FROM_BLK(8, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 13),  FROM_BLK(16, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 14),  FROM_BLK(32, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 15),  FROM_BLK(0, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 0),  0xFFFFFFFF,        0xFFFFFFFF            },
};
static ST_WRITE_PROTECT   _pstWriteProtectTable_EN25Q128[] =
{
    {  BITS(5:2, 1),  FROM_BLK(0, SIZE_64KB),  TO_BLK(254, SIZE_64KB)  },
    {  BITS(5:2, 2),  FROM_BLK(0, SIZE_64KB),  TO_BLK(253, SIZE_64KB)  },
    {  BITS(5:2, 3),  FROM_BLK(0, SIZE_64KB),  TO_BLK(251, SIZE_64KB)  },
    {  BITS(5:2, 4),  FROM_BLK(0, SIZE_64KB),  TO_BLK(247, SIZE_64KB)  },
    {  BITS(5:2, 5),  FROM_BLK(0, SIZE_64KB),  TO_BLK(239, SIZE_64KB)  },
    {  BITS(5:2, 6),  FROM_BLK(0, SIZE_64KB),  TO_BLK(223, SIZE_64KB)  },
    {  BITS(5:2, 7),  FROM_BLK(0, SIZE_64KB),  TO_BLK(255, SIZE_64KB)  },
    {  BITS(5:2, 8),  FROM_BLK(0, SIZE_64KB),  TO_BLK(255, SIZE_64KB)  },
    {  BITS(5:2, 9),  FROM_BLK(1, SIZE_64KB),  TO_BLK(255, SIZE_64KB)  },
    {  BITS(5:2, 10),  FROM_BLK(2, SIZE_64KB),  TO_BLK(255, SIZE_64KB)  },
    {  BITS(5:2, 11),  FROM_BLK(4, SIZE_64KB),  TO_BLK(255, SIZE_64KB)  },
    {  BITS(5:2, 12),  FROM_BLK(8, SIZE_64KB),  TO_BLK(255, SIZE_64KB)  },
    {  BITS(5:2, 13),  FROM_BLK(16, SIZE_64KB),  TO_BLK(255, SIZE_64KB)  },
    {  BITS(5:2, 14),  FROM_BLK(32, SIZE_64KB),  TO_BLK(255, SIZE_64KB)  },
    {  BITS(5:2, 15),  FROM_BLK(0, SIZE_64KB),  TO_BLK(255, SIZE_64KB)  },
    {  BITS(5:2, 0),  0xFFFFFFFF,        0xFFFFFFFF            },
};
static ST_WRITE_PROTECT   _pstWriteProtectTable_W25Q80[] =
{
    {  BITS(5:2, 1),  0x000F0000,        0x000FFFFF            },
    {  BITS(5:2, 2),  0x000E0000,        0x000FFFFF            },
    {  BITS(5:2, 3),  0x000C0000,        0x000FFFFF            },
    {  BITS(5:2, 4),  0x00080000,        0x000FFFFF            },
    {  BITS(5:2, 9),  0x00000000,        0x0000FFFF            },
    {  BITS(5:2, 10),  0x00000000,        0x0001FFFF            },
    {  BITS(5:2, 11),  0x00000000,        0x0003FFFF            },
    {  BITS(5:2, 12),  0x00000000,        0x0007FFFF            },
    {  BITS(5:2, 15),  0x00000000,        0x000FFFFF            },
    {  BITS(5:2, 0),  0xFFFFFFFF,        0xFFFFFFFF            },
};

static ST_WRITE_PROTECT   _pstWriteProtectTable_MX25L4006E[] =
{
    {  BITS(4:2, 1),  FROM_BLK(7, SIZE_64KB),  TO_BLK(7, SIZE_64KB)  },
    {  BITS(4:2, 2),  FROM_BLK(6, SIZE_64KB),  TO_BLK(7, SIZE_64KB)  },
    {  BITS(4:2, 3),  FROM_BLK(4, SIZE_64KB),  TO_BLK(7, SIZE_64KB)  },
    {  BITS(4:2, 4),  FROM_BLK(0, SIZE_64KB),  TO_BLK(7, SIZE_64KB)  },
    {  BITS(4:2, 5),  FROM_BLK(0, SIZE_64KB),  TO_BLK(7, SIZE_64KB)  },
    {  BITS(4:2, 6),  FROM_BLK(0, SIZE_64KB),  TO_BLK(7, SIZE_64KB)  },
    {  BITS(4:2, 7),  FROM_BLK(0, SIZE_64KB),  TO_BLK(7, SIZE_64KB)  },
    {  BITS(4:2, 0),  0xFFFFFFFF,        0xFFFFFFFF            },
};
static ST_WRITE_PROTECT   _pstWriteProtectTable_W25X80[] =
{
    {  BITS(5:2, 1),  0x000F0000,        0x000FFFFF            },
    {  BITS(5:2, 2),  0x000E0000,        0x000FFFFF            },
    {  BITS(5:2, 3),  0x000C0000,        0x000FFFFF            },
    {  BITS(5:2, 4),  0x00080000,        0x000FFFFF            },
    {  BITS(5:2, 9),  0x00000000,        0x0000FFFF            },
    {  BITS(5:2, 10),  0x00000000,        0x0001FFFF            },
    {  BITS(5:2, 11),  0x00000000,        0x0003FFFF            },
    {  BITS(5:2, 12),  0x00000000,        0x0007FFFF            },
    {  BITS(5:2, 13),  0x00000000,        0x000FFFFF            },
    {  BITS(5:2, 15),  0x00000000,        0x000FFFFF            },
    {  BITS(5:2, 0),  0xFFFFFFFF,        0xFFFFFFFF            },
};

static ST_WRITE_PROTECT   _pstWriteProtectTable_EN25QH16[] =
{
    {  BITS(5:2, 1),   0x00000000,        0x001EFFFF            },
    {  BITS(5:2, 2),  0x00000000,        0x001DFFFF            },
    {  BITS(5:2, 3),  0x00000000,        0x001BFFFF            },
    {  BITS(5:2, 4),  0x00000000,        0x0017FFFF            },
    {  BITS(5:2, 5),  0x00000000,        0x000FFFFF            },
    {  BITS(5:2, 6),  0x00000000,        0x001FFFFF            },
    {  BITS(5:2, 7),  0x00000000,         0x001FFFFF            },
    {  BITS(5:2, 8),  0xFFFFFFFF,        0xFFFFFFFF            },
    {  BITS(5:2, 9),  0x00010000,        0x001FFFFF            },
    {  BITS(5:2, 10),  0x00020000,        0x001FFFFF            },
    {  BITS(5:2, 11),  0x00040000,        0x001FFFFF            },
    {  BITS(5:2, 12),  0x00080000,        0x001FFFFF            },
    {  BITS(5:2, 13),  0x00100000,        0x001FFFFF            },
    {  BITS(5:2, 14),  0x00000000,        0x001FFFFF            },
    {  BITS(5:2, 15),  0x00000000,        0x001FFFFF            },
    {  BITS(5:2, 0),  0xFFFFFFFF,        0xFFFFFFFF            },
};

static ST_WRITE_PROTECT   _pstWriteProtectTable_W25X40[] =
{
    {  BITS(5:2, 1),  FROM_BLK(7, SIZE_64KB),  TO_BLK(7, SIZE_64KB)  },
    {  BITS(5:2, 2),  FROM_BLK(6, SIZE_64KB),  TO_BLK(7, SIZE_64KB)  },
    {  BITS(5:2, 3),  FROM_BLK(4, SIZE_64KB),  TO_BLK(7, SIZE_64KB)  },
    {  BITS(5:2, 4),  FROM_BLK(0, SIZE_64KB),  TO_BLK(7, SIZE_64KB)  },
    {  BITS(5:2, 5),  FROM_BLK(0, SIZE_64KB),  TO_BLK(7, SIZE_64KB)  },
    {  BITS(5:2, 6),  FROM_BLK(0, SIZE_64KB),  TO_BLK(7, SIZE_64KB)  },
    {  BITS(5:2, 7),  FROM_BLK(0, SIZE_64KB),  TO_BLK(7, SIZE_64KB)  },
    {  BITS(5:2, 10),  FROM_BLK(0, SIZE_64KB),  TO_BLK(1, SIZE_64KB)  },
    {  BITS(5:2, 11),  FROM_BLK(0, SIZE_64KB),  TO_BLK(3, SIZE_64KB)  },
    {  BITS(5:2, 15),  FROM_BLK(0, SIZE_64KB),  TO_BLK(7, SIZE_64KB)  },
    {  BITS(5:2, 0),  0xFFFFFFFF,        0xFFFFFFFF            },
};
static ST_WRITE_PROTECT   _pstWriteProtectTable_EN25F40[] =
{
    {  BITS(4:2, 1),  FROM_BLK(7, SIZE_64KB),  TO_BLK(7, SIZE_64KB)  },
    {  BITS(4:2, 2),  FROM_BLK(6, SIZE_64KB),  TO_BLK(7, SIZE_64KB)  },
    {  BITS(4:2, 3),  FROM_BLK(4, SIZE_64KB),  TO_BLK(7, SIZE_64KB)  },
    {  BITS(4:2, 4),  FROM_BLK(0, SIZE_64KB),  TO_BLK(7, SIZE_64KB)  },
    {  BITS(4:2, 5),  FROM_BLK(0, SIZE_64KB),  TO_BLK(7, SIZE_64KB)  },
    {  BITS(4:2, 6),  FROM_BLK(0, SIZE_64KB),  TO_BLK(7, SIZE_64KB)  },
    {  BITS(4:2, 7),  FROM_BLK(0, SIZE_64KB),  TO_BLK(7, SIZE_64KB)  },
    {  BITS(4:2, 0),  0xFFFFFFFF,        0xFFFFFFFF            },
};
static ST_WRITE_PROTECT   _pstWriteProtectTable_S25FL008A[] =
{
    {  BITS(5:2, 1),  FROM_BLK(15, SIZE_64KB),  TO_BLK(15, SIZE_64KB)  },
    {  BITS(5:2, 2),  FROM_BLK(14, SIZE_64KB),  TO_BLK(15, SIZE_64KB)  },
    {  BITS(5:2, 3),  FROM_BLK(12, SIZE_64KB),  TO_BLK(15, SIZE_64KB)  },
    {  BITS(5:2, 4),  FROM_BLK(8, SIZE_64KB),  TO_BLK(15, SIZE_64KB)  },
    {  BITS(5:2, 5),  FROM_BLK(0, SIZE_64KB),  TO_BLK(15, SIZE_64KB)  },
    {  BITS(5:2, 6),  FROM_BLK(0, SIZE_64KB),  TO_BLK(15, SIZE_64KB)  },
    {  BITS(5:2, 7),  FROM_BLK(0, SIZE_64KB),  TO_BLK(15, SIZE_64KB)  },
    {  BITS(5:2, 0),  0xFFFFFFFF,        0xFFFFFFFF            },
};
static ST_WRITE_PROTECT   _pstWriteProtectTable_PM25LQ032C[] =
{
    {  BITS(5:2, 1),  FROM_BLK(63, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(5:2, 2),  FROM_BLK(62, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(5:2, 3),  FROM_BLK(60, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(5:2, 4),  FROM_BLK(56, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(5:2, 5),  FROM_BLK(48, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(5:2, 6),  FROM_BLK(32, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(5:2, 7),  FROM_BLK(0, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(5:2, 9),  FROM_BLK(0, SIZE_64KB),  TO_BLK(0, SIZE_64KB)  },
    {  BITS(5:2, 10),  FROM_BLK(0, SIZE_64KB),  TO_BLK(1, SIZE_64KB)  },
    {  BITS(5:2, 11),  FROM_BLK(0, SIZE_64KB),  TO_BLK(3, SIZE_64KB)  },
    {  BITS(5:2, 12),  FROM_BLK(0, SIZE_64KB),  TO_BLK(7, SIZE_64KB)  },
    {  BITS(5:2, 13),  FROM_BLK(0, SIZE_64KB),  TO_BLK(15, SIZE_64KB)  },
    {  BITS(5:2, 14),  FROM_BLK(0, SIZE_64KB),  TO_BLK(31, SIZE_64KB)  },
    {  BITS(5:2, 15),  FROM_BLK(0, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(5:2, 0),  0xFFFFFFFF,        0xFFFFFFFF            },
};
static ST_WRITE_PROTECT   _pstWriteProtectTable_NUMONYX_M25PX16[] =
{
    {  BITS(5:2, 1),  FROM_BLK(31, SIZE_64KB),  TO_BLK(31, SIZE_64KB)  },
    {  BITS(5:2, 2),  FROM_BLK(30, SIZE_64KB),  TO_BLK(31, SIZE_64KB)  },
    {  BITS(5:2, 3),  FROM_BLK(28, SIZE_64KB),  TO_BLK(31, SIZE_64KB)  },
    {  BITS(5:2, 4),  FROM_BLK(24, SIZE_64KB),  TO_BLK(31, SIZE_64KB)  },
    {  BITS(5:2, 5),  FROM_BLK(16, SIZE_64KB),  TO_BLK(31, SIZE_64KB)  },
    {  BITS(5:2, 6),  FROM_BLK(0, SIZE_64KB),  TO_BLK(31, SIZE_64KB)  },
    {  BITS(5:2, 7),  FROM_BLK(0, SIZE_64KB),  TO_BLK(31, SIZE_64KB)  },
    {  BITS(5:2, 9),  FROM_BLK(0, SIZE_64KB),  TO_BLK(0, SIZE_64KB)  },
    {  BITS(5:2, 10),  FROM_BLK(0, SIZE_64KB),  TO_BLK(1, SIZE_64KB)  },
    {  BITS(5:2, 11),  FROM_BLK(0, SIZE_64KB),  TO_BLK(3, SIZE_64KB)  },
    {  BITS(5:2, 12),  FROM_BLK(0, SIZE_64KB),  TO_BLK(7, SIZE_64KB)  },
    {  BITS(5:2, 13),  FROM_BLK(0, SIZE_64KB),  TO_BLK(15, SIZE_64KB)  },
    {  BITS(5:2, 14),  FROM_BLK(0, SIZE_64KB),  TO_BLK(31, SIZE_64KB)  },
    {  BITS(5:2, 15),  FROM_BLK(0, SIZE_64KB),  TO_BLK(31, SIZE_64KB)  },
    {  BITS(5:2, 0),  0xFFFFFFFF,        0xFFFFFFFF            },
};
static ST_WRITE_PROTECT   _pstWriteProtectTable_NUMONYX_N25Q32[] =
{
    {  BITS(5:2, 1),  FROM_BLK(63, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(5:2, 2),  FROM_BLK(62, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(5:2, 3),  FROM_BLK(60, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(5:2, 4),  FROM_BLK(56, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(5:2, 5),  FROM_BLK(48, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(5:2, 6),  FROM_BLK(32, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(5:2, 7),  FROM_BLK(0, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(5:2, 9),  FROM_BLK(0, SIZE_64KB),  TO_BLK(0, SIZE_64KB)  },
    {  BITS(5:2, 10),  FROM_BLK(0, SIZE_64KB),  TO_BLK(1, SIZE_64KB)  },
    {  BITS(5:2, 11),  FROM_BLK(0, SIZE_64KB),  TO_BLK(3, SIZE_64KB)  },
    {  BITS(5:2, 12),  FROM_BLK(0, SIZE_64KB),  TO_BLK(7, SIZE_64KB)  },
    {  BITS(5:2, 13),  FROM_BLK(0, SIZE_64KB),  TO_BLK(15, SIZE_64KB)  },
    {  BITS(5:2, 14),  FROM_BLK(0, SIZE_64KB),  TO_BLK(31, SIZE_64KB)  },
    {  BITS(5:2, 15),  FROM_BLK(0, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(5:2, 0),  0xFFFFFFFF,        0xFFFFFFFF            },
};
static ST_WRITE_PROTECT   _pstWriteProtectTable_NUMONYX_N25Q64[] =
{
    {  BITS(5:2, 1),  FROM_BLK(127, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 2),  FROM_BLK(126, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 3),  FROM_BLK(124, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 4),  FROM_BLK(120, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 5),  FROM_BLK(112, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 6),  FROM_BLK(96, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 7),  FROM_BLK(64, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 9),  FROM_BLK(0, SIZE_64KB),  TO_BLK(0, SIZE_64KB)  },
    {  BITS(5:2, 10),  FROM_BLK(0, SIZE_64KB),  TO_BLK(1, SIZE_64KB)  },
    {  BITS(5:2, 11),  FROM_BLK(0, SIZE_64KB),  TO_BLK(3, SIZE_64KB)  },
    {  BITS(5:2, 12),  FROM_BLK(0, SIZE_64KB),  TO_BLK(7, SIZE_64KB)  },
    {  BITS(5:2, 13),  FROM_BLK(0, SIZE_64KB),  TO_BLK(15, SIZE_64KB)  },
    {  BITS(5:2, 14),  FROM_BLK(0, SIZE_64KB),  TO_BLK(31, SIZE_64KB)  },
    {  BITS(5:2, 15),  FROM_BLK(0, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(5:2, 0),  0xFFFFFFFF,        0xFFFFFFFF            },
};
static ST_WRITE_PROTECT   _pstWriteProtectTable_ESMT_F25L032QA[] =
{
    {  BITS(5:2, 1),  FROM_BLK(63, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(5:2, 2),  FROM_BLK(62, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(5:2, 3),  FROM_BLK(60, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(5:2, 4),  FROM_BLK(56, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(5:2, 5),  FROM_BLK(48, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(5:2, 6),  FROM_BLK(32, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(5:2, 7),  FROM_BLK(0, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(5:2, 8),  FROM_BLK(0, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(5:2, 9),  FROM_BLK(0, SIZE_64KB),  TO_BLK(31, SIZE_64KB)  },
    {  BITS(5:2, 10),  FROM_BLK(0, SIZE_64KB),  TO_BLK(47, SIZE_64KB)  },
    {  BITS(5:2, 11),  FROM_BLK(0, SIZE_64KB),  TO_BLK(55, SIZE_64KB)  },
    {  BITS(5:2, 12),  FROM_BLK(0, SIZE_64KB),  TO_BLK(59, SIZE_64KB)  },
    {  BITS(5:2, 13),  FROM_BLK(0, SIZE_64KB),  TO_BLK(61, SIZE_64KB)  },
    {  BITS(5:2, 14),  FROM_BLK(0, SIZE_64KB),  TO_BLK(62, SIZE_64KB)  },
    {  BITS(5:2, 15),  FROM_BLK(0, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(5:2, 0),  0xFFFFFFFF,        0xFFFFFFFF            },
};
static ST_WRITE_PROTECT   _pstWriteProtectTable_ESMT_F25L032PA[] =
{
    {  BITS(4:2, 1),  FROM_BLK(63, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(4:2, 2),  FROM_BLK(62, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(4:2, 3),  FROM_BLK(60, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(4:2, 4),  FROM_BLK(56, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(4:2, 5),  FROM_BLK(48, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(4:2, 6),  FROM_BLK(32, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(4:2, 7),  FROM_BLK(0, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(4:2, 0),  0xFFFFFFFF,        0xFFFFFFFF            },
};
static ST_WRITE_PROTECT   _pstWriteProtectTable_S25FL164K[] =
{
    {  BITS(5:2, 1),  FROM_BLK(126, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 2),  FROM_BLK(124, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 3),  FROM_BLK(120, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 4),  FROM_BLK(112, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 5),  FROM_BLK(96, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 6),  FROM_BLK(64, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 7),  FROM_BLK(0, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 9),  FROM_BLK(0, SIZE_64KB),  TO_BLK(1, SIZE_64KB)  },
    {  BITS(5:2, 10),  FROM_BLK(0, SIZE_64KB),  TO_BLK(3, SIZE_64KB)  },
    {  BITS(5:2, 11),  FROM_BLK(0, SIZE_64KB),  TO_BLK(7, SIZE_64KB)  },
    {  BITS(5:2, 12),  FROM_BLK(0, SIZE_64KB),  TO_BLK(15, SIZE_64KB)  },
    {  BITS(5:2, 13),  FROM_BLK(0, SIZE_64KB),  TO_BLK(31, SIZE_64KB)  },
    {  BITS(5:2, 14),  FROM_BLK(0, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(5:2, 15),  FROM_BLK(0, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 0),  0xFFFFFFFF,        0xFFFFFFFF            },
};
static ST_WRITE_PROTECT   _pstWriteProtectTable_IS25CQ032[] =
{
    {  BITS(5:2, 1),  FROM_BLK(63, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(5:2, 2),  FROM_BLK(62, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(5:2, 3),  FROM_BLK(60, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(5:2, 4),  FROM_BLK(56, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(5:2, 5),  FROM_BLK(48, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(5:2, 6),  FROM_BLK(32, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(5:2, 7),  FROM_BLK(0, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(5:2, 9),  FROM_BLK(0, SIZE_64KB),  TO_BLK(0, SIZE_64KB)  },
    {  BITS(5:2, 10),  FROM_BLK(0, SIZE_64KB),  TO_BLK(1, SIZE_64KB)  },
    {  BITS(5:2, 11),  FROM_BLK(0, SIZE_64KB),  TO_BLK(3, SIZE_64KB)  },
    {  BITS(5:2, 12),  FROM_BLK(0, SIZE_64KB),  TO_BLK(7, SIZE_64KB)  },
    {  BITS(5:2, 13),  FROM_BLK(0, SIZE_64KB),  TO_BLK(15, SIZE_64KB)  },
    {  BITS(5:2, 14),  FROM_BLK(0, SIZE_64KB),  TO_BLK(31, SIZE_64KB)  },
    {  BITS(5:2, 15),  FROM_BLK(0, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(5:2, 0),  0xFFFFFFFF,        0xFFFFFFFF            },
};
static ST_WRITE_PROTECT   _pstWriteProtectTable_XM25QH64A[] =
{
    {  BITS(5:2, 1),  FROM_BLK(127, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 2),  FROM_BLK(126, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 3),  FROM_BLK(124, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 4),  FROM_BLK(120, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 5),  FROM_BLK(112, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 6),  FROM_BLK(96, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 7),  FROM_BLK(64, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 8),  FROM_BLK(32, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 9),  FROM_BLK(16, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 10),  FROM_BLK(8, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 11),  FROM_BLK(4, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 12),  FROM_BLK(2, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 13),  FROM_BLK(1, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 14),  FROM_BLK(0, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 15),  FROM_BLK(0, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 0),  0xFFFFFFFF,        0xFFFFFFFF            },
};
static ST_WRITE_PROTECT   _pstWriteProtectTable_XM25QH128A[] =
{
    {  BITS(5:2, 1),  FROM_BLK(252, SIZE_64KB),  TO_BLK(255, SIZE_64KB)  },
    {  BITS(5:2, 2),  FROM_BLK(248, SIZE_64KB),  TO_BLK(255, SIZE_64KB)  },
    {  BITS(5:2, 3),  FROM_BLK(240, SIZE_64KB),  TO_BLK(255, SIZE_64KB)  },
    {  BITS(5:2, 4),  FROM_BLK(224, SIZE_64KB),  TO_BLK(255, SIZE_64KB)  },
    {  BITS(5:2, 5),  FROM_BLK(192, SIZE_64KB),  TO_BLK(255, SIZE_64KB)  },
    {  BITS(5:2, 6),  FROM_BLK(128, SIZE_64KB),  TO_BLK(255, SIZE_64KB)  },
    {  BITS(5:2, 7),  FROM_BLK(0, SIZE_64KB),  TO_BLK(255, SIZE_64KB)  },
    {  BITS(5:2, 9),  FROM_BLK(0, SIZE_64KB),  TO_BLK(3, SIZE_64KB)  },
    {  BITS(5:2, 10),  FROM_BLK(0, SIZE_64KB),  TO_BLK(7, SIZE_64KB)  },
    {  BITS(5:2, 11),  FROM_BLK(0, SIZE_64KB),  TO_BLK(15, SIZE_64KB)  },
    {  BITS(5:2, 12),  FROM_BLK(0, SIZE_64KB),  TO_BLK(31, SIZE_64KB)  },
    {  BITS(5:2, 13),  FROM_BLK(0, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(5:2, 14),  FROM_BLK(0, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 15),  FROM_BLK(0, SIZE_64KB),  TO_BLK(255, SIZE_64KB)  },
    {  BITS(5:2, 0),  0xFFFFFFFF,        0xFFFFFFFF            },
};
static ST_WRITE_PROTECT   _pstWriteProtectTable_ZB25VQ32[] =
{
    {  BITS(5:2, 1),  FROM_BLK(63, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(5:2, 2),  FROM_BLK(62, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(5:2, 3),  FROM_BLK(60, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(5:2, 4),  FROM_BLK(56, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(5:2, 5),  FROM_BLK(48, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(5:2, 6),  FROM_BLK(32, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(5:2, 7),  FROM_BLK(0, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(5:2, 0),  0xFFFFFFFF,        0xFFFFFFFF            },
};
static ST_WRITE_PROTECT   _pstWriteProtectTable_ZB25Q64A[] =
{
    {  BITS(5:2, 1),  FROM_BLK(127, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 2),  FROM_BLK(126, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 3),  FROM_BLK(124, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 4),  FROM_BLK(120, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 5),  FROM_BLK(112, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 6),  FROM_BLK(96, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 7),  FROM_BLK(64, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 8),  FROM_BLK(32, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 9),  FROM_BLK(16, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 10),  FROM_BLK(8, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 11),  FROM_BLK(4, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 12),  FROM_BLK(2, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 13),  FROM_BLK(1, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 14),  FROM_BLK(0, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 15),  FROM_BLK(0, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 0),  0xFFFFFFFF,        0xFFFFFFFF            },
};
static ST_WRITE_PROTECT   _pstWriteProtectTable_BH25Q32BS[] =
{
    {  BITS(4:2, 1),  FROM_BLK(63, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(4:2, 2),  FROM_BLK(62, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(4:2, 3),  FROM_BLK(60, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(4:2, 4),  FROM_BLK(56, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(4:2, 5),  FROM_BLK(48, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(4:2, 6),  FROM_BLK(32, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(4:2, 7),  FROM_BLK(0, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(4:2, 0),  0xFFFFFFFF,        0xFFFFFFFF            },
};
static ST_WRITE_PROTECT   _pstWriteProtectTable_BH25Q64BS[] =
{
    {  BITS(5:2, 1),  FROM_BLK(126, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 2),  FROM_BLK(124, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 3),  FROM_BLK(120, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 4),  FROM_BLK(112, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 5),  FROM_BLK(96, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 6),  FROM_BLK(64, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 7),  FROM_BLK(0, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 9),  FROM_BLK(0, SIZE_64KB),  TO_BLK(1, SIZE_64KB)  },
    {  BITS(5:2, 10),  FROM_BLK(0, SIZE_64KB),  TO_BLK(3, SIZE_64KB)  },
    {  BITS(5:2, 11),  FROM_BLK(0, SIZE_64KB),  TO_BLK(7, SIZE_64KB)  },
    {  BITS(5:2, 12),  FROM_BLK(0, SIZE_64KB),  TO_BLK(15, SIZE_64KB)  },
    {  BITS(5:2, 13),  FROM_BLK(0, SIZE_64KB),  TO_BLK(31, SIZE_64KB)  },
    {  BITS(5:2, 14),  FROM_BLK(0, SIZE_64KB),  TO_BLK(63, SIZE_64KB)  },
    {  BITS(5:2, 15),  FROM_BLK(0, SIZE_64KB),  TO_BLK(127, SIZE_64KB)  },
    {  BITS(5:2, 0),  0xFFFFFFFF,        0xFFFFFFFF            },
};






hal_SERFLASH_t _hal_SERFLASH_table[] =
{ 
     { FLASH_IC_MX25L1655D,          MID_MXIC,        0x26,     0x15,   NULL,                                  NULL,                       SIZE_2MB,       32,           BITS(5:2,  0x0F),      0,                                            1},
     { FLASH_IC_MX25L3205D,          MID_MXIC,        0x20,     0x16,   _pstWriteProtectTable_MX25L3205D,      NULL,                       SIZE_4MB,       64,           BITS(5:2,  0x0F),      sizeof(_pstWriteProtectTable_MX25L3205D),     1},
     { FLASH_IC_MX25L3254E,          MID_MXIC,        0x9E,     0x16,   _pstWriteProtectTable_MX25L3254E,      NULL,                       SIZE_4MB,       64,           BITS(5:2,  0x0F),      sizeof(_pstWriteProtectTable_MX25L3254E),     1},
     { FLASH_IC_MX25L6406E,          MID_MXIC,        0x20,     0x17,   _pstWriteProtectTable_MX25L6406E,      NULL,                       SIZE_8MB,       128,          BITS(5:2,  0x0F),      sizeof(_pstWriteProtectTable_MX25L6406E),     1},
     { FLASH_IC_MX25L1606E,          MID_MXIC,        0x20,     0x15,   _pstWriteProtectTable_MX25L1606E,      NULL,                       SIZE_2MB,       32,           BITS(5:2,  0x0F),      sizeof(_pstWriteProtectTable_MX25L1606E),     1},
     { FLASH_IC_MX25L12805D,         MID_MXIC,        0x20,     0x18,   _pstWriteProtectTable_MX25L12805D,     NULL,                       SIZE_16MB,      256,          BITS(5:2,  0x0F),      sizeof(_pstWriteProtectTable_MX25L12805D),    1},
     { FLASH_IC_MX25L6455E,          MID_MXIC,        0x26,     0x17,   _pstWriteProtectTable_MX25L6455E,      NULL,                       SIZE_8MB,       128,          BITS(5:2,  0x0F),      sizeof(_pstWriteProtectTable_MX25L6455E),     1},
     { FLASH_IC_MX25L12855E,         MID_MXIC,        0x26,     0x18,   _pstWriteProtectTable_MX25L12855E,     NULL,                       SIZE_16MB,      256,          BITS(5:2,  0x0F),      sizeof(_pstWriteProtectTable_MX25L12855E),    1},
     { FLASH_IC_MX25L4006E,          MID_MXIC,        0x20,     0x13,   _pstWriteProtectTable_MX25L4006E,      NULL,                       SIZE_512KB,     8,            BITS(4:2,  0x07),      sizeof(_pstWriteProtectTable_MX25L4006E),     1},
     { FLASH_IC_MX25L8006E,          MID_MXIC,        0x20,     0x14,   NULL,                                  NULL,                       SIZE_1MB,       16,           BITS(5:2,  0x0F),      0,                                            1},
     { FLASH_IC_W25Q16,              MID_WB,          0x40,     0x15,   NULL,                                  NULL,                       SIZE_2MB,       32,           BITS(4:2,  0x07),      0,                                            2},
     { FLASH_IC_W25Q80,              MID_WB,          0x40,     0x14,   _pstWriteProtectTable_W25Q80,          NULL,                       SIZE_1MB,       16,           BITS(5:2,  0x0F),      sizeof(_pstWriteProtectTable_W25Q80),         1},
     { FLASH_IC_W25X32,              MID_WB,          0x30,     0x16,   _pstWriteProtectTable_W25X32,          NULL,                       SIZE_4MB,       64,           BITS(5:2,  0x0F),      sizeof(_pstWriteProtectTable_W25X32),         2},
     { FLASH_IC_W25Q32,              MID_WB,          0x40,     0x16,   _pstWriteProtectTable_W25Q32,          NULL,                       SIZE_4MB,       64,           BITS(5:2,  0x0F),      sizeof(_pstWriteProtectTable_W25Q32),         2},
     { FLASH_IC_W25X64,              MID_WB,          0x70,     0x17,   _pstWriteProtectTable_W25X64,          NULL,                       SIZE_8MB,       128,          BITS(5:2,  0x0F),      sizeof(_pstWriteProtectTable_W25X64),         2},
     { FLASH_IC_W25X80,              MID_WB,          0x30,     0x14,   _pstWriteProtectTable_W25X80,          NULL,                       SIZE_1MB,       16,           BITS(5:2,  0x0F),      sizeof(_pstWriteProtectTable_W25X80),         1},
     { FLASH_IC_W25Q64FV,            MID_WB,          0x40,     0x17,   _pstWriteProtectTable_W25Q64FV,        NULL,                       SIZE_8MB,       128,          BITS(5:2,  0x0F),      sizeof(_pstWriteProtectTable_W25Q64FV),       1},
     { FLASH_IC_W25Q128FV,           MID_WB,          0x40,     0x18,   _pstWriteProtectTable_W25Q128FV,       NULL,                       SIZE_16MB,      256,          BITS(5:2,  0x0F),      sizeof(_pstWriteProtectTable_W25Q128FV),      1},
     { FLASH_IC_W25X40,              MID_WB,          0x30,     0x13,   _pstWriteProtectTable_W25X40,          NULL,                       SIZE_512KB,     8,            BITS(5:2,  0x0F),      sizeof(_pstWriteProtectTable_W25X40),         1},
     { FLASH_IC_AT26DF321,           MID_ATMEL,       0x47,     0x00,   _pstWriteProtectTable_AT26DF321,       NULL,                       SIZE_4MB,       64,           BITS(5:2,  0x0F),      sizeof(_pstWriteProtectTable_AT26DF321),      1},
     { FLASH_IC_STM25P32,            MID_ST,          0x20,     0x16,   NULL,                                  NULL,                       SIZE_512KB,     8,            BITS(4:2,  0x07),      0,                                            1},
     { FLASH_IC_EN25B32B,            MID_EON,         0x20,     0x16,   NULL,                                  &_stSpecialBlocks_EN25B32B, SIZE_4MB,       64,           BITS(4:2,  0x07),      0,                                            1},
     { FLASH_IC_EN25B64B,            MID_EON,         0x20,     0x17,   NULL,                                  &_stSpecialBlocks_EN25B64B, SIZE_8MB,       128,          BITS(4:2,  0x07),      0,                                            1},
     { FLASH_IC_S25FL032P,           MID_SPAN,        0x02,     0x15,   _pstWriteProtectTable_S25FL032P,       NULL,                       SIZE_4MB,       64,           BITS(4:2,  0x07),      sizeof(_pstWriteProtectTable_S25FL032P),      1},
     { FLASH_IC_S25FL064P,           MID_SPAN,        0x02,     0x16,   _pstWriteProtectTable_S25FL064P,       NULL,                       SIZE_8MB,       128,          BITS(4:2,  0x07),      sizeof(_pstWriteProtectTable_S25FL064P),      1},
     { FLASH_IC_S25FL128P,           MID_SPAN,        0x20,     0x18,   _pstWriteProtectTable_S25FL128P,       NULL,                       SIZE_16MB,      256,          BITS(5:2,  0x0F),      sizeof(_pstWriteProtectTable_S25FL128P),      1},
     { FLASH_IC_S25FL008A,           MID_SPAN,        0x20,     0x13,   _pstWriteProtectTable_S25FL008A,       NULL,                       SIZE_1MB,       16,           BITS(5:2,  0x0F),      sizeof(_pstWriteProtectTable_S25FL008A),      1},
     { FLASH_IC_S25FL164K,           MID_SPAN,        0x40,     0x17,   _pstWriteProtectTable_S25FL164K,       NULL,                       SIZE_16MB,      256,          BITS(5:2,  0x0F),      sizeof(_pstWriteProtectTable_S25FL164K),      1},
     { FLASH_IC_S25FL128L,           MID_SPAN,        0x60,     0x18,   _pstWriteProtectTable_S25FL128L,       NULL,                       SIZE_8MB,       128,          BITS(4:2,  0x07),      sizeof(_pstWriteProtectTable_S25FL128L),      1},
     { FLASH_IC_EN25F10,             MID_EON,         0x31,     0x11,   NULL,                                   &_stSpecialBlocks_EN25F10, SIZE_256KB,     4,            BITS(4:2,  0x07),      0,                                            1},
     { FLASH_IC_EN25F16,             MID_EON,         0x31,     0x15,   _pstWriteProtectTable_EN25F16,         NULL,                       SIZE_8MB,       128,          BITS(4:2,  0x07),      sizeof(_pstWriteProtectTable_EN25F16),        1},
     { FLASH_IC_EN25F32,             MID_EON,         0x31,     0x16,   _pstWriteProtectTable_EN25F32,         NULL,                       SIZE_16MB,      256,          BITS(5:2,  0x0F),      sizeof(_pstWriteProtectTable_EN25F32),        1},
     { FLASH_IC_EN25F80,             MID_EON,         0x31,     0x14,   _pstWriteProtectTable_EN25F80,         NULL,                       SIZE_256KB,     4,            BITS(4:2,  0x07),      sizeof(_pstWriteProtectTable_EN25F80),        1},
     { FLASH_IC_EN25P16,             MID_EON,         0x20,     0x15,   _pstWriteProtectTable_EN25P16,         NULL,                       SIZE_2MB,       32,           BITS(4:2,  0x07),      sizeof(_pstWriteProtectTable_EN25P16),        1},
     { FLASH_IC_EN25Q32A,            MID_EON,         0x30,     0x16,   _pstWriteProtectTable_EN25Q32A,        NULL,                       SIZE_4MB,       64,           BITS(5:2,  0x0F),      sizeof(_pstWriteProtectTable_EN25Q32A),       1},
     { FLASH_IC_EN25Q64,             MID_EON,         0x30,     0x17,   _pstWriteProtectTable_EN25Q64,         NULL,                       SIZE_8MB,       128,          BITS(5:2,  0x0F),      sizeof(_pstWriteProtectTable_EN25Q64),        1},
     { FLASH_IC_EN25QA64A,           MID_EON,         0x60,     0x17,   _pstWriteProtectTable_EN25QA64A,       NULL,                       SIZE_8MB,       128,          BITS(5:2,  0x0F),      sizeof(_pstWriteProtectTable_EN25QA64A),      1},
     { FLASH_IC_EN25Q128,            MID_EON,         0x30,     0x18,   _pstWriteProtectTable_EN25Q128,        NULL,                       SIZE_16MB,      256,          BITS(5:2,  0x0F),      sizeof(_pstWriteProtectTable_EN25Q128),       1},
     { FLASH_IC_EN25F40,             MID_EON,         0x31,     0x13,   _pstWriteProtectTable_EN25F40,         NULL,                       SIZE_512KB,     8,            BITS(4:2,  0x07),      sizeof(_pstWriteProtectTable_EN25F40),        1},
     { FLASH_IC_EN25QH16,            MID_EON,         0x70,     0x15,   _pstWriteProtectTable_EN25QH16,        NULL,                       SIZE_2MB,       32,           BITS(4:2,  0x07),      sizeof(_pstWriteProtectTable_EN25QH16),       1},
     { FLASH_IC_NUMONYX_M25PX16,     MID_NUMONYX,     0x73,     0x15,   _pstWriteProtectTable_NUMONYX_M25PX16, NULL,                       SIZE_2MB,       32,           BITS(5:2,  0x0F),      sizeof(_pstWriteProtectTable_NUMONYX_M25PX16),1},
     { FLASH_IC_NUMONYX_N25Q32,      MID_NUMONYX,     0xBA,     0x16,   _pstWriteProtectTable_NUMONYX_N25Q32,  NULL,                       SIZE_4MB,       64,           BITS(4:2,  0x0F),      sizeof(_pstWriteProtectTable_NUMONYX_N25Q32), 1},
     { FLASH_IC_NUMONYX_N25Q64,      MID_NUMONYX,     0xBA,     0x17,   _pstWriteProtectTable_NUMONYX_N25Q64,  NULL,                       SIZE_8MB,       128,          BITS(4:2,  0x07),      sizeof(_pstWriteProtectTable_NUMONYX_N25Q64), 1},
     { FLASH_IC_PM25LQ032C,          MID_PMC,         0x9D,     0x46,   _pstWriteProtectTable_PM25LQ032C,      NULL,                       SIZE_4MB,       64,           BITS(5:2,  0x0F),      sizeof(_pstWriteProtectTable_PM25LQ032C),     1},
     { FLASH_IC_GD25S80,             MID_GD,          0x40,     0x14,   NULL,                                  NULL,                       SIZE_1MB,       16,           BITS(5:2,  0x0F),      0,                                            2},
     { FLASH_IC_GD25Q16,             MID_GD,          0x40,     0x15,   _pstWriteProtectTable_GD25Q16,         NULL,                       SIZE_2MB,       32,           BITS(6:2,  0x1F),      sizeof(_pstWriteProtectTable_GD25Q16),        1},
     { FLASH_IC_GD25Q32,             MID_GD,          0x40,     0x16,   NULL,                                  NULL,                       SIZE_4MB,       64,           BITS(5:2,  0x0F),      0,                                            1},
     { FLASH_IC_GD25Q64,             MID_GD,          0x40,     0x17,   NULL,                                  NULL,                       SIZE_8MB,       128,          BITS(5:2,  0x0F),      0,                                            1},
     { FLASH_IC_GD25Q128C,           MID_GD,          0x40,     0x18,   _pstWriteProtectTable_GD25Q128C,       NULL,                       SIZE_16MB,      256,          BITS(6:2,  0x1F),      sizeof(_pstWriteProtectTable_GD25Q128C),      1},
     { FLASH_IC_A25LM032,            MID_AM,          0x30,     0x16,   NULL,                                  NULL,                       SIZE_4MB,       64,           BITS(5:2,  0x0F),      0,                                            1},
     { FLASH_IC_ESMT_F25L032QA,      MID_ESMT,        0x41,     0x16,   _pstWriteProtectTable_ESMT_F25L032QA,  NULL,                       SIZE_4MB,       64,           BITS(5:2,  0x07),      sizeof(_pstWriteProtectTable_ESMT_F25L032QA), 1},
     { FLASH_IC_ESMT_F25L032PA,      MID_ESMT,        0x20,     0x16,   _pstWriteProtectTable_ESMT_F25L032PA,  NULL,                       SIZE_4MB,       64,           BITS(4:2,  0x07),      sizeof(_pstWriteProtectTable_ESMT_F25L032PA), 1},
     { FLASH_IC_IS25LP64,            MID_ISSI,        0x60,     0x17,   _pstWriteProtectTable_IS25LP64,        NULL,                       SIZE_8MB,       128,          BITS(5:2,  0x0F),      sizeof(_pstWriteProtectTable_IS25LP64),       1},
     { FLASH_IC_IS25CQ032,           MID_ISSI,        0x15,     0x46,   _pstWriteProtectTable_IS25CQ032,       NULL,                       SIZE_4MB,       64,           BITS(5:2,  0x0F),      sizeof(_pstWriteProtectTable_IS25CQ032),      1},
     { FLASH_IC_XM25QH64A,           MID_XM,          0x70,     0x17,   _pstWriteProtectTable_XM25QH64A,       NULL,                       SIZE_8MB,       128,          BITS(5:2,  0x0F),      sizeof(_pstWriteProtectTable_XM25QH64A),      2},
     { FLASH_IC_XM25QH128A,          MID_XM,          0x70,     0x18,   _pstWriteProtectTable_XM25QH128A,      NULL,                       SIZE_16MB,      256,          BITS(5:2,  0x0F),      sizeof(_pstWriteProtectTable_XM25QH128A),     2},
     { FLASH_IC_ZB25VQ32,            MID_ZB,          0x40,     0x16,   _pstWriteProtectTable_ZB25VQ32,        NULL,                       SIZE_4MB,       64,           BITS(4:2,  0x07),      sizeof(_pstWriteProtectTable_ZB25VQ32),       1},
     { FLASH_IC_ZB25Q64A,            MID_ZB,          0x70,     0x17,   _pstWriteProtectTable_ZB25Q64A,        NULL,                       SIZE_8MB,       128,          BITS(5:2,  0x0F),      sizeof(_pstWriteProtectTable_ZB25Q64A),       1},
     { FLASH_IC_BH25Q32BS,           MID_BH,          0x40,     0x16,   _pstWriteProtectTable_BH25Q32BS,       NULL,                       SIZE_4MB,       64,           BITS(4:2,  0x07),      sizeof(_pstWriteProtectTable_BH25Q32BS),      1},
     { FLASH_IC_BH25Q64BS,           MID_BH,          0x40,     0x17,   _pstWriteProtectTable_BH25Q64BS,       NULL,                       SIZE_8MB,       128,          BITS(5:2,  0x0F),      sizeof(_pstWriteProtectTable_BH25Q64BS),      1},
};
#endif
