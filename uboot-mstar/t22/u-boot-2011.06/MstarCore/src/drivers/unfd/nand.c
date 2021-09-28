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


#include <linux/mtd/compat.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>

#include "drvNAND.h"

#ifdef CONFIG_ENABLE_WIDESTORAGE
#include <MsDisk.h>
#endif

#define DMA_MODE                    1

U32 u32CurRow = 0;
U32 u32CurCol = 0;
U32 u32CIFDIdx = 0;
U16 u16ByteIdxofPage = 0;
U32 u32WriteLen = 0;
S32 s32_ECCStatus = 0;

int sectoridx=0;

int enable_sar5 = 1;

U8 u8MainBuf[8192*4] UNFD_ALIGN1;
U8 u8SpareBuf[640*4] UNFD_ALIGN1;

#define CONFIG_NANDDRV_DEBUG 1

#if (CONFIG_NANDDRV_DEBUG)
#define NAND_DEBUG(fmt, args...) nand_debug(UNFD_DEBUG_LEVEL_LOW, 1, fmt, ##args)
#else
#define NAND_DEBUG(fmt, args...)
#endif


/* These really don't belong here, as they are specific to the NAND Model */
static uint8_t scan_ff_pattern[] = {0xff};

/* struct nand_bbt_descr - bad block table descriptor */
static struct nand_bbt_descr _titania_nand_bbt_descr = {
    .options = NAND_BBT_2BIT | NAND_BBT_LASTBLOCK | NAND_BBT_VERSION | NAND_BBT_CREATE | NAND_BBT_WRITE,
    .offs = 5,
    .len = 1,
    .pattern = scan_ff_pattern
};

static struct nand_ecclayout unfd_nand_oob_custom;

static uint8_t bbt_pattern[] = {'B', 'b', 't', '0' };
static uint8_t mirror_pattern[] = {'1', 't', 'b', 'B' };

static struct nand_bbt_descr _titania_bbt_main_descr = {
    .options        = NAND_BBT_LASTBLOCK | NAND_BBT_CREATE | NAND_BBT_WRITE |
                      NAND_BBT_2BIT | NAND_BBT_VERSION | NAND_BBT_PERCHIP,
    .offs           = 1,
    .len            = 3,
    .veroffs        = 4,
    .maxblocks      = NAND_BBT_BLOCK_NUM,
    .pattern        = bbt_pattern
};

static struct nand_bbt_descr _titania_bbt_mirror_descr = {
    .options        = NAND_BBT_LASTBLOCK | NAND_BBT_CREATE | NAND_BBT_WRITE |
                      NAND_BBT_2BIT | NAND_BBT_VERSION | NAND_BBT_PERCHIP,
    .offs           = 1,
    .len            = 3,
    .veroffs        = 4,
    .maxblocks      = NAND_BBT_BLOCK_NUM,
    .pattern        = mirror_pattern
};

typedef struct _SAMSUNG_BBM_WA {
    U8  u8_IDByteCnt;
    U8  au8_ID[NAND_ID_BYTE_CNT];

} SAMSUNG_BBM_WA;

SAMSUNG_BBM_WA t_bbm_wa[] = 
{
    {5, {0xEC, 0xDA, 0x10, 0x95, 0x46}},
    {5, {0xEC, 0xDC, 0x10, 0x95, 0x56}},
    {5, {0xEC, 0xD3, 0x51, 0x95, 0x5A}},
    {0, {0x00}}
};

static void _titania_nand_hwcontrol(struct mtd_info *mtdinfo, int cmd, unsigned int ctrl)
{
    NAND_DEBUG("NANDDrv_HWCONTROL()\n");

    switch(cmd)
    {
        case NAND_CTL_SETNCE:
            NAND_DEBUG("NAND_CTL_SETNCE\n");
            // Let FCIE3 control the CE
            break;
        case NAND_CTL_CLRNCE:
            NAND_DEBUG("NAND_CTL_CLRNCE\n");
            // Let FCIE3 control the CE

            break;
        case NAND_CTL_SETCLE:
            NAND_DEBUG("NAND_CTL_SETCLE\n");
            // We have no way to control CLE in NFIE/FCIE2/FCIE3
            break;
        case NAND_CTL_CLRCLE:
            NAND_DEBUG("NAND_CTL_CLRCLE\n");
            // We have no way to control CLE in NFIE/FCIE2/FCIE3
            break;

        case NAND_CTL_SETALE:
            NAND_DEBUG("NAND_CTL_SETALE\n");
            // We have no way to control CLE in NFIE/FCIE2/FCIE3
            break;
        case NAND_CTL_CLRALE:
            NAND_DEBUG("NAND_CTL_CLRALE\n");
            // We have no way to control CLE in NFIE/FCIE2/FCIE3
            break;

        default:
            NAND_DEBUG("UNKNOWN CMD\n");
            break;
  }

  return;
}


static int _titania_nand_device_ready(struct mtd_info *mtdinfo)
{
    NAND_DEBUG("NANDDrv_DEVICE_READY()\n");

    #if 0 // We have wait ready in each HAL functions
    drvNAND_WaitReady(1000);
    #endif

    return 1;
}

static void _titania_nand_write_buf(struct mtd_info *mtd, const u_char *buf, int len)
{

    NAND_DEBUG("NANDDrv_WRITE_BUF() 0x%X\r\n",len);

    NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();
    U8 *pu8MainBuf;
    U8 *pu8SpareBuf;

    if( len >= pNandDrv->u16_PageByteCnt * pNandDrv->u8_PlaneCnt ) // whole page or Main area only
    {
        if( len > pNandDrv->u16_PageByteCnt * pNandDrv->u8_PlaneCnt ) // whole page
        {
            #if !defined(CONFIG_ARM)
            if( ((U32)buf) >= 0xC0000000 ) // For MIU1 (MIPS)
            {
                memcpy(u8MainBuf, buf, pNandDrv->u16_PageByteCnt * pNandDrv->u8_PlaneCnt);
                memcpy(u8SpareBuf, buf + (pNandDrv->u16_PageByteCnt * pNandDrv->u8_PlaneCnt), pNandDrv->u16_SpareByteCnt * pNandDrv->u8_PlaneCnt);
                pu8MainBuf = u8MainBuf;
                pu8SpareBuf = u8SpareBuf;
            }
            else
            #endif
            {
                pu8MainBuf = (U8*)buf;
                pu8SpareBuf = (U8*)buf + (pNandDrv->u16_PageByteCnt * pNandDrv->u8_PlaneCnt);
            }

            #if defined(CONFIG_MLC_CONFIG_FOR_MP)
            if(pNandDrv->u8_CellType)
            {
                for(sectoridx = 0; sectoridx < pNandDrv->u16_PageSectorCnt; sectoridx ++)
                {
                    pu8SpareBuf[0x5 + sectoridx * pNandDrv->u16_SectorSpareByteCnt] = 0xDE;
                    pu8SpareBuf[0x6 + sectoridx * pNandDrv->u16_SectorSpareByteCnt] = 0xAD;
                }
            }
            #endif

            #if DMA_MODE
            if(pNandDrv->u8_PlaneCnt > 1)
                NC_WritePages2P(u32CurRow, pu8MainBuf, pu8SpareBuf, 1);
            else
                NC_WritePages(u32CurRow, pu8MainBuf, pu8SpareBuf, 1);
            #else
            NC_WritePage_RIUMode(u32CurRow, pu8MainBuf, pu8SpareBuf);
            #endif
        }
        else // main area only
        {
            memcpy(u8MainBuf, buf, len);
            u32WriteLen += len;
            u16ByteIdxofPage += len;
        }
    }
    else
    {
        if((u32WriteLen==0) && (u16ByteIdxofPage>=pNandDrv->u16_PageByteCnt * pNandDrv->u8_PlaneCnt)) // mtd skip prepare main area, default all oxff
        {
            memset(u8MainBuf, 0xFF, pNandDrv->u16_PageByteCnt * pNandDrv->u8_PlaneCnt);
            memset(u8SpareBuf, 0xFF, pNandDrv->u16_SpareByteCnt * pNandDrv->u8_PlaneCnt);
        }

        memcpy(u8SpareBuf, buf, len);
        u32WriteLen += len;
        u16ByteIdxofPage += len;

        if( (u32WriteLen == (pNandDrv->u16_PageByteCnt + pNandDrv->u16_SpareByteCnt) * pNandDrv->u8_PlaneCnt) ||
        (u32WriteLen ==  pNandDrv->u16_SpareByteCnt * pNandDrv->u8_PlaneCnt))
        {   
            #if defined(CONFIG_MLC_CONFIG_FOR_MP)
            if(pNandDrv->u8_CellType)
            {
                for(sectoridx = 0; sectoridx < pNandDrv->u16_PageSectorCnt; sectoridx ++)
                {
                    u8SpareBuf[0x5 + sectoridx * pNandDrv->u16_SectorSpareByteCnt] = 0xDE;
                    u8SpareBuf[0x6 + sectoridx * pNandDrv->u16_SectorSpareByteCnt] = 0xAD;
                }
            }
            #endif

            #if DMA_MODE
            if(pNandDrv->u8_PlaneCnt > 1)
                NC_WritePages2P(u32CurRow, u8MainBuf, u8SpareBuf, 1);
            else
                NC_WritePages(u32CurRow, u8MainBuf, u8SpareBuf, 1);
            #else
            NC_WritePage_RIUMode(u32CurRow, u8MainBuf, u8SpareBuf);
            #endif
        }
    }

  return;
}

static void _titania_nand_read_buf(struct mtd_info *mtd, u_char* const buf, int len)
{
//  printf("_titania_nand_read_buf(): %08X, %d\n",buf, len );

    NAND_DEBUG("NANDDrv_READ_BUF()0x%X\n",len);

    NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();
    U8 *pu8MainBuf;
    U8 *pu8SpareBuf;
    U32 u32_Err = 0;

    u16ByteIdxofPage = len;

    if( len >= pNandDrv->u16_PageByteCnt * pNandDrv->u8_PlaneCnt)
    {
        if( len > pNandDrv->u16_PageByteCnt * pNandDrv->u8_PlaneCnt)
        {
            #if !defined(CONFIG_ARM)
            if( ((U32)buf) >= 0xC0000000 ) // For MIU1 (MIPS)
            {
                pu8MainBuf = u8MainBuf;
                pu8SpareBuf = u8SpareBuf;
            }
            else
            #endif
            {
                pu8MainBuf = buf;
                pu8SpareBuf = (U8*)buf + (pNandDrv->u16_PageByteCnt * pNandDrv->u8_PlaneCnt);
            }

            #if DMA_MODE

            memset(pu8SpareBuf, 0xFF, pNandDrv->u16_SpareByteCnt * pNandDrv->u8_PlaneCnt);
            if(pNandDrv->u8_PlaneCnt > 1)
            {
                u32_Err = NC_ReadPages(u32CurRow, pu8MainBuf, pu8SpareBuf, 1);
                if(u32_Err == 0)
                    u32_Err = NC_ReadPages(u32CurRow + pNandDrv->u16_BlkPageCnt,
                                pu8MainBuf + pNandDrv->u16_PageByteCnt, pu8SpareBuf + pNandDrv->u16_SpareByteCnt, 1);
            }
            else
                u32_Err = NC_ReadPages(u32CurRow, pu8MainBuf, pu8SpareBuf, 1);
            #else
            for(sectoridx = 0; sectoridx < pNandDrv->u16_PageSectorCnt; sectoridx ++)
                u32_Err = NC_ReadSector_RIUMode(u32CurRow, sectoridx, pu8MainBuf + sectoridx*pNandDrv->u16_SectorByteCnt,
                    pu8SpareBuf+ sectoridx*pNandDrv->u16_SectorSpareByteCnt);
            #endif
            NC_CheckECC((int*)(&s32_ECCStatus));
            if(u32_Err != UNFD_ST_SUCCESS)
                s32_ECCStatus = -1;
            else
            {                
                #if !(defined(CONFIG_MLC_CONFIG_FOR_MP) && CONFIG_MLC_CONFIG_FOR_MP)                
                if(pu8SpareBuf[0x5] == 0xDE && pu8SpareBuf[0x6] == 0xAD && pNandDrv->u8_CellType == 1)
                    s32_ECCStatus = 1;
                #endif
            }

            if( buf != pu8MainBuf ) // If MIU1, copy data from temp buffer
            {
                memcpy(buf, u8MainBuf, pNandDrv->u16_PageByteCnt * pNandDrv->u8_PlaneCnt);
                memcpy((U8*)buf + (pNandDrv->u16_PageByteCnt * pNandDrv->u8_PlaneCnt), u8SpareBuf, pNandDrv->u16_SpareByteCnt * pNandDrv->u8_PlaneCnt);
            }
        }
        else
        {
            #if !defined(CONFIG_ARM)
            if( ((U32)buf) >= 0xC0000000 ) // For MIU1 (MIPS)
            {
                pu8MainBuf = u8MainBuf;
            }
            else
            #endif
            {
                pu8MainBuf = buf;
            }
            pu8SpareBuf = u8SpareBuf; // Preserve spare data in temp buffer for next read_buf()

            #if DMA_MODE
            memset(pu8SpareBuf, 0xFF, pNandDrv->u16_SpareByteCnt * pNandDrv->u8_PlaneCnt);
            if(pNandDrv->u8_PlaneCnt > 1)
            {
                u32_Err = NC_ReadPages(u32CurRow, pu8MainBuf, pu8SpareBuf, 1);
                if(u32_Err == 0)
                    u32_Err = NC_ReadPages(u32CurRow + pNandDrv->u16_BlkPageCnt,
                                pu8MainBuf + pNandDrv->u16_PageByteCnt, pu8SpareBuf + pNandDrv->u16_SpareByteCnt, 1);
            }
            else
            u32_Err = NC_ReadPages(u32CurRow, pu8MainBuf, pu8SpareBuf, 1);
            #else
            for(sectoridx = 0; sectoridx < pNandDrv->u16_PageSectorCnt; sectoridx ++)
                u32_Err = NC_ReadSector_RIUMode(u32CurRow, sectoridx, pu8MainBuf + sectoridx*pNandDrv->u16_SectorByteCnt,
                    pu8SpareBuf+ sectoridx*pNandDrv->u16_SectorSpareByteCnt );
            #endif
            NC_CheckECC((int *)(&s32_ECCStatus));
            if(u32_Err != UNFD_ST_SUCCESS)
                s32_ECCStatus = -1;
            else
            {                
                #if !(defined(CONFIG_MLC_CONFIG_FOR_MP) && CONFIG_MLC_CONFIG_FOR_MP)                
                if(pu8SpareBuf[0x5] == 0xDE && pu8SpareBuf[0x6] == 0xAD && pNandDrv->u8_CellType == 1)
                    s32_ECCStatus = 1;
                #endif
            }

            if( buf != pu8MainBuf ) // If MIU1, copy data from temp buffer
                 memcpy(buf, u8MainBuf, pNandDrv->u16_PageByteCnt);
        }
    }
    else
    {
        memcpy(buf, u8SpareBuf, len);
    }

    return;
}

static u16 _titania_nand_read_word(struct mtd_info *mtd)
{
    NAND_DEBUG("NANDDrv_READ_WORD()\n");
    return 0;
}

static u_char _titania_nand_read_byte(struct mtd_info *mtd)
{
    U8 u8Ret = 0;
    #if defined(NC_SEL_FCIE3) && NC_SEL_FCIE3
    
    #if defined(FCIE_LFSR) && FCIE_LFSR
    NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();
    if(pNandDrv->u8_RequireRandomizer)
    {
        if(IF_SPARE_DMA())
            u8Ret = u8SpareBuf[u32CIFDIdx];
        else
            NC_GetCIFD(&u8Ret, u32CIFDIdx, 1);

        u32CIFDIdx ++;
    }
    else
    {
    #endif
        u8Ret = u8SpareBuf[u32CIFDIdx];
//        NC_GetCIFD(&u8Ret, u32CIFDIdx, 1);
        u32CIFDIdx ++;
    #if defined(FCIE_LFSR) && FCIE_LFSR
    }
    #endif

    #elif defined(NC_SEL_FCIE5) && NC_SEL_FCIE5 //only spare dma
    u8Ret = u8SpareBuf[u32CIFDIdx]; 
    u32CIFDIdx ++;
    #endif
    NAND_DEBUG("NANDDrv_READ_BYTE()=0x%X\n",u8Ret);

    return (u8Ret);
}

/**
 * nand_wait - [DEFAULT]  wait until the command is done
 * @mtd:  MTD device structure
 * @this: NAND chip structure
 * @state:  state to select the max. timeout value
 *
 * Wait for command done. This applies to erase and program only
 * Erase can take up to 400ms and program up to 20ms according to
 * general NAND and SmartMedia specs
 *
 */
static int _titania_nand_wait(struct mtd_info *mtd, struct nand_chip *this)
{
    NAND_DEBUG("NANDDrv_WAIT()\n");

    return REG(NC_ST_READ);
}

static void _titania_nand_cmdfunc(struct mtd_info *mtd, unsigned command, int column, int page_addr)
{
    NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();
    struct nand_chip *chip =(struct nand_chip*)(mtd->priv);
    int LBA, LPA;

    if(pNandDrv->u8_PlaneCnt > 1)
    {
        LBA = page_addr >> (chip->phys_erase_shift - chip->page_shift);
        LPA = page_addr & ((1<<(chip->phys_erase_shift - chip->page_shift))-1);
        page_addr = ((LBA<<1)<<pNandDrv->u8_BlkPageCntBits)+LPA;
    }


    switch (command) {
        case NAND_CMD_READ0:
            NAND_DEBUG("_titania_nand_cmdfunc: NAND_CMD_READ0, page_addr: 0x%x, column: 0x%x.\n", page_addr, (column>>1));
            u32CurRow = page_addr;
            u32CurCol = column;
            break;

        case NAND_CMD_READ1:
            NAND_DEBUG("_titania_nand_cmdfunc: NAND_CMD_READ1.\n");

            NAND_DEBUG("\033[31mUNFD not support READ1(CMD 0x01) now!!!\033[m\n");
            break;

        case NAND_CMD_READOOB:
            NAND_DEBUG("_titania_nand_cmdfunc: NAND_CMD_READOOB.\n");

            NAND_DEBUG("\033[34mReadOOB (CMD 0x50)\033[m\n");
            u32CIFDIdx = 0;
            if(page_addr < 10*pNandDrv->u16_BlkPageCnt)
                  Disable_Err_log = 1;
            NC_ReadPages(page_addr, u8MainBuf, u8SpareBuf, 1);
            Disable_Err_log = 0;
        break;

        case NAND_CMD_READID:
            NAND_DEBUG("_titania_nand_cmdfunc: NAND_CMD_READID.\n");

            u32CIFDIdx = 0;
            NC_ReadID();
            memcpy(u8SpareBuf, pNandDrv->au8_ID, NAND_ID_BYTE_CNT);
            break;

        case NAND_CMD_PAGEPROG:
            /* sent as a multicommand in NAND_CMD_SEQIN */
            NAND_DEBUG("_titania_nand_cmdfunc: NAND_CMD_PAGEPROG.\n");

            // We have done page program in write_buf()
            break;

        case NAND_CMD_ERASE1:
            NAND_DEBUG("_titania_nand_cmdfunc: NAND_CMD_ERASE1,  page_addr: 0x%x, column: 0x%x.\n", page_addr, column);
            if(pNandDrv->u8_PlaneCnt > 1)
                NC_EraseBlk2P(page_addr);
            else
                NC_EraseBlk(page_addr);

            break;

        case NAND_CMD_ERASE2:
            NAND_DEBUG("_titania_nand_cmdfunc: NAND_CMD_ERASE2.\n");
            // We do all erase function in Erase1 command.

            break;

        case NAND_CMD_SEQIN:
            /* send PAGE_PROG command(0x1080) */
            NAND_DEBUG("_titania_nand_cmdfunc: NAND_CMD_SEQIN/PAGE_PROG,  page_addr: 0x%x, column: 0x%x.\n", page_addr, column);

            u32CurRow = page_addr;
            u32CurCol = column;
            u16ByteIdxofPage = column;
            u32WriteLen = 0;
            break;

        case NAND_CMD_STATUS:
            NAND_DEBUG("_titania_nand_cmdfunc: NAND_CMD_STATUS.\n");

            u32CIFDIdx = 0;
            NC_ReadStatus();

            break;

        case NAND_CMD_RESET:
            NAND_DEBUG("_titania_nand_cmdfunc: NAND_CMD_RESET.\n");

            NC_ResetNandFlash();

            break;

        case NAND_CMD_STATUS_MULTI:
            NAND_DEBUG("_titania_nand_cmdfunc: NAND_CMD_STATUS_MULTI.\n");

            u32CIFDIdx = 0;
            NC_ReadStatus();
            break;

        case NAND_CMD_READSTART:
            NAND_DEBUG("_titania_nand_cmdfunc: NAND_CMD_READSTART.\n");

            // We have done this command in NC_ReadPages()
            break;

        case NAND_CMD_CACHEDPROG:
            NAND_DEBUG("_titania_nand_cmdfunc: NAND_CMD_CACHEDPROG.\n");

            NAND_DEBUG("\033[31mUNFD not support CACHEPROG (CMD 0x15) now!!!\033[m\n");
            break;

        default:
            NAND_DEBUG("_titania_nand_cmdfunc: error, unsupported command.\n");
            break;
    }

    return;
}

static void _titania_nand_enable_hwecc(struct mtd_info *mtd, int mode)
{
    NAND_DEBUG("enable_hwecc\r\n");
    // default enable
}

static int _titania_nand_calculate_ecc(struct mtd_info *mtd, const u_char *dat, u_char *ecc_code)
{
    NAND_DEBUG("calculate_ecc\r\n");

    // NFIE/FCIE2/FCIE3 will calculate it.

    return 0;
}

static int _titania_nand_correct_data(struct mtd_info *mtd, u_char *dat, u_char *read_ecc, u_char *calc_ecc)
{
    int s32ECCStatus = 0;
    //NC_CheckECC(&s32ECCStatus);
    s32ECCStatus = s32_ECCStatus;
    return s32ECCStatus;
}
U32 drvNAND_Init(void)
{
    U32 u32_Err = 0;
    NAND_DRIVER *pNandDrv;

    #ifdef CONFIG_ENABLE_WIDESTORAGE
    if(MSDISK_eMMC == ChkDiskType())
    {
        printf("%s: eMMC disk\n", __func__);
        return -1;
    }
    #endif 

    //-------------------------------
    pNandDrv = drvNAND_get_DrvContext_address();
    memset(pNandDrv, 0, sizeof(NAND_DRIVER));

    NC_PlatformInit();

    pNandDrv->pPartInfo = drvNAND_get_DrvContext_PartInfo();
    memset(pNandDrv->pPartInfo, 0, NAND_PARTITAION_BYTE_CNT);

    u32_Err = drvNAND_ProbeReadSeq();
    if(u32_Err != UNFD_ST_SUCCESS)
    {
        return u32_Err;
    }
    u32_Err = drvNAND_SearchCIS();

    return u32_Err;
}


int nand_unfd_init(void)
{
    NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();
    U32 u32_Err;
    U8 u8_i;

    #ifdef CONFIG_ENABLE_WIDESTORAGE
    if(MSDISK_eMMC == ChkDiskType())
    {
        printf("%s: eMMC disk\n", __func__);
        return -1;
    }
    #endif 

    //-------------------------------    
    u32_Err = drvNAND_Init();

    if(UNFD_ST_ERR_NO_CIS == u32_Err)
    {
        //printf("NAND: No CIS\n");

        #if defined(FCIE4_DDR) && FCIE4_DDR || defined(DDR_NAND_SUPPORT) && DDR_NAND_SUPPORT
        if(pNandDrv->u16_Reg58_DDRCtrl & BIT_DDR_MASM)
        {
            printf("NAND Error: Detect DDR NAND but have no CIS in NAND\n");
            printf("Please use command, \"ncisbl\", to write cis\n");
            return -1;
        }
        #endif
        {
            NC_PlatformInit();
            NC_ReConfig();
            
            #if defined(ENABLE_NAND_INTERRUPT_MODE) && ENABLE_NAND_INTERRUPT_MODE
            nand_enable_intr_mode();
            #endif

            NC_ResetNandFlash(); // Reset NAND flash
            nand_debug(UNFD_DEBUG_LEVEL_HIGH,1, "NC_ResetNandFlash done\n");

            NC_ReadID();
            nand_debug(UNFD_DEBUG_LEVEL_HIGH,1, "NC_ReadID done\n");

            if( drvNAND_CHECK_FLASH_TYPE() != 0)
            {
                printf("[%s]\tUnsupported NAND Type, Need to update CIS or modify code\n",__func__);
                return -1;
            }

            nand_debug(UNFD_DEBUG_LEVEL_HIGH,1,"drvNAND_CHECK_FLASH_TYPE done\n");

            NC_ConfigNandFlashContext();
            NC_Init();
        }
    }
    else if(UNFD_ST_SUCCESS != u32_Err)
    {
        printf("[%s]\tdrvNAND_Init Error : %Xh", __func__, u32_Err);
        return -1;
    }

    if(pNandDrv->au8_ID[0] == 0xEC)
    {
        for(u8_i=0 ; t_bbm_wa[u8_i].u8_IDByteCnt!=0 ; u8_i++)
        {
            if(memcmp(t_bbm_wa[u8_i].au8_ID, pNandDrv->au8_ID, t_bbm_wa[u8_i].u8_IDByteCnt) == 0)
            {
                pNandDrv->u16_bbm_wa = 0xBAD;
                nand_debug(UNFD_DEBUG_LEVEL,0,"Need BBM workaround!!!\n");
                break;
            }
        }
    }

    // detect CE count
    Disable_Err_log = 1;
    for(u8_i=1 ; u8_i<(pNandDrv->u8_MaxCENum+1) ; u8_i++)
    {
        REG_CLR_BITS_UINT16(NC_SIGNAL,  BIT_NC_CE_SEL_MASK);    
        REG_SET_BITS_UINT16(NC_SIGNAL,  u8_i);
        if(NC_ResetNandFlash() != UNFD_ST_SUCCESS)
        {
            pNandDrv->u8_MaxCENum = u8_i-1;
            break;
        }
    }
    Disable_Err_log = 0;
    REG_CLR_BITS_UINT16(NC_SIGNAL,  BIT_NC_CE_SEL_MASK); 
    if(pNandDrv->u8_MaxCENum > 0)
    {
        nand_debug(UNFD_DEBUG_LEVEL,1,"%d CE detect\n", pNandDrv->u8_MaxCENum+1);
    }
    else
        nand_pads_release();   
    
    /*Config clock timing either for DDR NAND or SDR NAND */
    nand_config_clock(pNandDrv->u16_tRC);

    if(u32_Err == UNFD_ST_SUCCESS)
    {
        if(nand_ReadDisturbance_BigImgRestore(NULL) != UNFD_ST_SUCCESS)
        {
            printf("[%s] NAND fatal Error, restore data fail\n", __func__);
        }
        #if defined(ENABLE_READ_DISTURBANCE_SUPPORT) && ENABLE_READ_DISTURBANCE_SUPPORT
        if(nand_ReadDisturbance() != UNFD_ST_SUCCESS)
        {
            printf("[%s] NAND Read Disturbance fail\n", __func__);
        }
        #endif
    }

    #if defined(CONFIG_RESERVED_END_OF_NAND) && defined(CONFIG_MSTAR_FTL_SD)
    FTLSd_Init(0, 0);
    #endif
    return 0;
}


void nand_CheckSAR5ON(void)
{
    #if defined(ENABLE_NAND_POWER_SAVING_MODE) && ENABLE_NAND_POWER_SAVING_MODE     
    U16 i, len;
    char *res;

    res = getenv("bootargs");
    if(res)
    {
        len = strlen(res);
        for(i=0; i<len; i++)
            if('S'==res[i] && 'A'==res[i+1] &&
               'R'==res[i+2] && '5'==res[i+3] &&
               '='==res[i+4] && 'O'==res[i+5] &&
               'F'==res[i+6] && 'F'==res[i+7])
                break;

        if(i<len)
        {
            printf("no SAR5=ON %Xh\n", len);
            enable_sar5=0;
        }
        else
        {
            //printf("SAR5=ON %Xh\n", len);
            nand_Prepare_Power_Saving_Mode_Queue();
        }
    }
    #endif
}

/*
 * Board-specific NAND initialization.
 * - hwcontrol: hardwarespecific function for accesing control-lines
 * - dev_ready: hardwarespecific function for  accesing device ready/busy line
 * - eccmode: mode of ecc, see defines
 */

int board_nand_init(struct nand_chip *nand)
{
    U16 u16_tmp, u16_oob_poi, u16_i, u16_SectorSpareByteCnt;
    NAND_DEBUG("NANDDrv_BOARD_NAND_INIT().\n");

    #ifdef CONFIG_ENABLE_WIDESTORAGE
    if(MSDISK_eMMC == ChkDiskType())
    {
        printf("%s: eMMC disk\n", __func__);
        return -1;
    }
    #endif 

    //-------------------------------    
    #if DMA_MODE
    //printf("DMA MODE\n");
    #else
    printf("RIU MODE !!!!!!\n");
    #endif

    NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();


    if( nand_unfd_init() != 0 ) return -1;

    #if defined(NC_SEL_FCIE3) && NC_SEL_FCIE3
    // Set flag to inform the kernel that only NAND driver is enabled
    REG_CLR_BITS_UINT16(NC_REG_16h, BIT_KERN_CHK_NAND_EMMC_MSK);
    REG_SET_BITS_UINT16(NC_REG_16h, BIT_KERN_NAND);
    #elif defined(NC_SEL_FCIE5) && NC_SEL_FCIE5
    //Set flag to inform the kernel that only NAND driver is enabled ????? how
    #endif

    /* please refer to include/linux/nand.h for more info. */
    nand->dev_ready = _titania_nand_device_ready;
    nand->cmd_ctrl = _titania_nand_hwcontrol;
    nand->ecc.mode  = NAND_ECC_HW;

    nand->ecc.size                      = pNandDrv->u16_PageByteCnt * pNandDrv->u8_PlaneCnt;
    nand->ecc.bytes                     = pNandDrv->u16_ECCCodeByteCnt * pNandDrv->u16_PageSectorCnt * pNandDrv->u8_PlaneCnt;
    nand->require_randomizer            = pNandDrv->u8_RequireRandomizer;
    nand->ecc_corretable_bit            = pNandDrv->u16_ECCCorretableBit;
    nand->ecc_bitflip_threshold         = pNandDrv->u16_BitflipThreshold;

    u16_SectorSpareByteCnt = (pNandDrv->u16_SpareByteCnt/pNandDrv->u16_PageSectorCnt);
    nand->ecc.sectors = pNandDrv->u16_PageSectorCnt * pNandDrv->u8_PlaneCnt;

    unfd_nand_oob_custom.eccbytes = pNandDrv->u8_PlaneCnt*(pNandDrv->u16_ECCCodeByteCnt * pNandDrv->u16_PageSectorCnt);
    for(u16_tmp=0 ; u16_tmp<pNandDrv->u8_PlaneCnt*pNandDrv->u16_PageSectorCnt ; u16_tmp++)
    {
        u16_oob_poi = ((u16_tmp+1)*u16_SectorSpareByteCnt)-pNandDrv->u16_ECCCodeByteCnt;
        for(u16_i=0 ; u16_i<pNandDrv->u16_ECCCodeByteCnt ; u16_i++)
        {
            unfd_nand_oob_custom.eccpos[(u16_tmp*pNandDrv->u16_ECCCodeByteCnt)+u16_i] = u16_oob_poi++;
        }

        if( u16_tmp == 0 || u16_tmp == pNandDrv->u16_PageSectorCnt)
        {
            unfd_nand_oob_custom.oobfree[u16_tmp].offset = u16_tmp*u16_SectorSpareByteCnt + 2;
            unfd_nand_oob_custom.oobfree[u16_tmp].length= u16_SectorSpareByteCnt - pNandDrv->u16_ECCCodeByteCnt - 2;
        }
        else
        {
            unfd_nand_oob_custom.oobfree[u16_tmp].offset = u16_tmp*u16_SectorSpareByteCnt;
            unfd_nand_oob_custom.oobfree[u16_tmp].length= u16_SectorSpareByteCnt - pNandDrv->u16_ECCCodeByteCnt;
        }
    }
    //dump_mem((unsigned char *) &unfd_nand_oob_custom, sizeof(unfd_nand_oob_custom));
    unfd_nand_oob_custom.oobavail = 0;
    for (u16_i = 0; u16_i < ARRAY_SIZE(unfd_nand_oob_custom.oobfree); u16_i++)
    {
        if(unfd_nand_oob_custom.oobfree[u16_i].length)
            unfd_nand_oob_custom.oobavail += unfd_nand_oob_custom.oobfree[u16_i].length;
    }


    nand->ecc.layout =  &unfd_nand_oob_custom;
    _titania_nand_bbt_descr.offs    = pNandDrv->u8_BadBlkMarker;

    nand->ecc.hwctl = _titania_nand_enable_hwecc;
    nand->ecc.correct = _titania_nand_correct_data;
    nand->ecc.calculate = _titania_nand_calculate_ecc;

    if( pNandDrv->u16_PageByteCnt > 512 )
        nand->options = NAND_USE_FLASH_BBT | NAND_NO_AUTOINCR;
    else
        nand->options = NAND_USE_FLASH_BBT;

    nand->options = NAND_USE_FLASH_BBT;
    nand->waitfunc = _titania_nand_wait;
    nand->read_byte = _titania_nand_read_byte;
    nand->read_word = _titania_nand_read_word;
    nand->read_buf = _titania_nand_read_buf;
    nand->write_buf = _titania_nand_write_buf;
    nand->chip_delay = 20; //@FIXME: unite: us, please refer to nand_base.c 20us is default.
    nand->cmdfunc = _titania_nand_cmdfunc;
    nand->badblock_pattern = &_titania_nand_bbt_descr; //using default badblock pattern.
    nand->bbt_td = &_titania_bbt_main_descr;
    nand->bbt_md = &_titania_bbt_mirror_descr;

    if(unfd_nand_oob_custom.oobfree[0].length < (nand->bbt_td->len +1))
    {
        nand->bbt_td->offs = 0;
        nand->bbt_td->veroffs = 3;
        nand->bbt_md->offs = 0;
        nand->bbt_md->veroffs = 3;
    }

    if(pNandDrv->u16_BBtPageCheckCount != 0 && pNandDrv->u16_BBtPageCheckCount <= NAND_BBT_PAGE_COUNT)  //using specific bbt checking
    {
        _titania_nand_bbt_descr.BBtMarkerPageCount = pNandDrv->u16_BBtPageCheckCount;
        _titania_nand_bbt_descr.options |= NAND_BBT_SCANASSIGNEDPAGES;
        for(u16_i = 0; u16_i < pNandDrv->u16_BBtPageCheckCount; u16_i ++)
        {
            _titania_nand_bbt_descr.BBtMarkerPage[u16_i] = pNandDrv->u16_BBtPageIdx[u16_i];
            _titania_nand_bbt_descr.BBtMarkerOffs[u16_i] = pNandDrv->u16_BBtMarker[u16_i];
        }
    }
    return 0;

}

U32 drvNAND_WriteCIS_for_ROM(NAND_FLASH_INFO_t * pNandInfo)
{
    NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
    PARTITION_INFO_t *pPartInfo = drvNAND_get_DrvContext_PartInfo();
    U8 *au8_PageBuf = (U8*)pNandDrv->pu8_PageDataBuf;
    U8 *au8_SpareBuf = (U8*)pNandDrv->pu8_PageSpareBuf;
    BLK_INFO_t *pBlkInfo = (BLK_INFO_t*)au8_SpareBuf;
    U32 u32_Err = UNFD_ST_SUCCESS;
    U16 u16_PBA, WrittenLPIdx=0, u16_tmp, WrittenPIdx=0;
    U32 u32_BlkRow;
    U8 u8_CISIdx;
    #if defined(CONFIG_STB_BFN) && (CONFIG_STB_BFN==1)
    #if defined(CONFIG_STB_BFN_VERSION) && (CONFIG_STB_BFN_VERSION < 5)
        int ParamMIUSize = 0;
        int ProgramMiuPage = 0;
        U32 ProgramMiuAddr = 0;
    #endif
    #endif    

    NC_ConfigNandFlashContext();
    NC_RegInit();
    NC_Config();
    nand_config_clock(pNandInfo->u16_tRC);

    #if defined(FCIE4_DDR) && FCIE4_DDR || defined(DDR_NAND_SUPPORT) && DDR_NAND_SUPPORT
    memcpy((void *) &pNandInfo->tDefaultDDR, (const void *) &pNandDrv->tDefaultDDR, sizeof(DDR_TIMING_GROUP_t));
    memcpy((void *) &pNandInfo->tMaxDDR, (const void *) &pNandDrv->tMaxDDR, sizeof(DDR_TIMING_GROUP_t));
    memcpy((void *) &pNandInfo->tMinDDR, (const void *) &pNandDrv->tMinDDR, sizeof(DDR_TIMING_GROUP_t));
    #endif

    u8_CISIdx = 0;
    REG(RIU_BASE + (0x1980 << 2) + (0x41 << 2) ) = 0x0000;

    /* Search for two good blocks within the first 10 physical blocks */
    for (u16_PBA = 0; u16_PBA < 10; u16_PBA++) {

        /* Reeset NAND driver and FCIE to the original settings */
        pNandDrv->u16_SpareByteCnt = pNandInfo->u16_SpareByteCnt;
        pNandDrv->u16_PageByteCnt  = pNandInfo->u16_PageByteCnt;
        pNandDrv->u16_ECCType      = pNandInfo->u16_ECCType;
        NC_ConfigNandFlashContext();
        NC_RegInit();
        pNandDrv->u16_Reg48_Spare &= ~BIT_NC_HW_AUTO_RANDOM_CMD_DISABLE;
        //Disable Randomizer for nni Read/Write
        NC_Config();

        u32_BlkRow = u16_PBA << pNandDrv->u8_BlkPageCntBits;

        Disable_Err_log = 1;
        /* Check block is good block */
        if(!drvNAND_IsGoodBlk(u16_PBA))
            continue;
        Disable_Err_log = 0;

        u32_Err = NC_EraseBlk(u16_PBA << pNandDrv->u8_BlkPageCntBits);
        if (u32_Err != UNFD_ST_SUCCESS) {
            nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Erase blk 0x%04x failed: 0x%08X\n",
                u16_PBA, u32_Err);

            drvNAND_MarkBadBlk(u16_PBA);
            continue;
        }

        pNandDrv->u16_PageByteCnt = 2048;
        pNandDrv->u16_SpareByteCnt = 0x100;
        pNandDrv->u16_ECCType = NANDINFO_ECC_TYPE;
        #if defined(ECC_TYPE_72BIT1KB) && ECC_TYPE_72BIT1KB
        if(pNandDrv->u16_ECCType == ECC_TYPE_72BIT1KB)
            pNandDrv->u16_SpareByteCnt = 0x180;
        #endif


        NC_ConfigNandFlashContext();
        NC_RegInit();
        pNandDrv->u16_Reg48_Spare |= BIT_NC_HW_AUTO_RANDOM_CMD_DISABLE;
        NC_Config();

        memset(au8_PageBuf, '\0', pNandDrv->u16_PageByteCnt);
        memcpy(au8_PageBuf, pNandInfo, 512);
        memset(au8_SpareBuf, 0xFF, pNandDrv->u16_SpareByteCnt);

        pBlkInfo->u8_BadBlkMark = 0xFF;
        pBlkInfo->u8_PartType = 0;

        u32_Err = NC_WriteSectors(u32_BlkRow, 0, au8_PageBuf, au8_SpareBuf, 1);
        WrittenLPIdx = 0;
        if (u32_Err != UNFD_ST_SUCCESS)
        {
            nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "write nni failed: 0x%08X\n", u32_Err);
            drvNAND_MarkBadBlk(u16_PBA);
            continue;
        }

        /* Reset NAND driver and FCIE to the original settings */
        pNandDrv->u16_SpareByteCnt = pNandInfo->u16_SpareByteCnt;
        pNandDrv->u16_PageByteCnt  = pNandInfo->u16_PageByteCnt;
        pNandDrv->u16_ECCType      = pNandInfo->u16_ECCType;
        NC_ConfigNandFlashContext();
        NC_RegInit();

        pNandDrv->u16_Reg48_Spare &= ~BIT_NC_HW_AUTO_RANDOM_CMD_DISABLE;
        NC_Config();

        
        /*
        **  Write Partition Info the 2nd page
        **/
        if(pNandDrv->u8_HasPNI == 1 || pNandDrv->u8_CellType == NAND_CellType_MLC) // MLC has to fill with dummy data
        {
            if(pNandDrv->u8_SlcMode == 0)
                drvNAND_WriteDummyToPages(ga_tPairedPageMap[WrittenLPIdx].u16_LSB+1, ga_tPairedPageMap[1].u16_LSB-1, u32_BlkRow);
            
            memset(au8_PageBuf, '\0', pNandDrv->u16_PageByteCnt);
            memcpy(au8_PageBuf, pPartInfo, 512);

            if(pNandDrv->u8_SlcMode == 1 || pNandDrv->u8_CellType == NAND_CellType_SLC)
            {
                u32_Err = NC_WriteSectors(u32_BlkRow+1, 0, au8_PageBuf, au8_SpareBuf, 1);
                WrittenPIdx = 1;
            }
            else
            {
                u32_Err = NC_WriteSectors(u32_BlkRow+ga_tPairedPageMap[1].u16_LSB, 0, au8_PageBuf, au8_SpareBuf, 1);
                WrittenLPIdx = 1;
            }
            if (u32_Err != UNFD_ST_SUCCESS)
            {
                nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Write Part Info failed with EC: 0x%08X\n", u32_Err);
                drvNAND_MarkBadBlk(u16_PBA);
                continue;
            }
        }

        #if defined(CONFIG_MSTAR_UNFD_FTL) && (CONFIG_MSTAR_UNFD_FTL==1)
        {

            U8 *pu8_BBT;
            INIT_BBT_t *pInitBBT;
            
            if(pNandDrv->u8_CellType == NAND_CellType_MLC && pNandDrv->u8_SlcMode == 0)
                drvNAND_WriteDummyToPages(ga_tPairedPageMap[WrittenLPIdx].u16_LSB+1,
                    ga_tPairedPageMap[2].u16_LSB-1, u32_BlkRow);
            else
                drvNAND_WriteDummyToPages(WrittenPIdx + 1, 2 - 1, u32_BlkRow);

            
            pu8_BBT = malloc(512);
            if(!pu8_BBT)
            {
                nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "alloc memmory fail\n");
                return UNFD_ST_ERR_NULL_PTR;
            }
            
            pInitBBT = (INIT_BBT_t *)pu8_BBT;
            memset(pu8_BBT, '\0', 512);

            drvNAND_InitBBT(pInitBBT);
            /*
             *  Write Initial BBT to the 3rd page
             */
            memset(au8_PageBuf, '\0', pNandDrv->u16_PageByteCnt);
            memcpy(au8_PageBuf, pu8_BBT, 512);
            memset(au8_SpareBuf, 0xFF, pNandDrv->u16_SpareByteCnt);

            if(pNandDrv->u8_CellType == NAND_CellType_MLC && pNandDrv->u8_SlcMode == 0)
            {
                u32_Err = NC_WriteSectors(u32_BlkRow+ga_tPairedPageMap[2].u16_LSB, 0, au8_PageBuf, au8_SpareBuf, 1);
                WrittenLPIdx = 2;
            }
            else
            {
                u32_Err = NC_WriteSectors(u32_BlkRow+ 2, 0, au8_PageBuf, au8_SpareBuf, 1);
                WrittenPIdx = 2;
            }
            
            if (u32_Err != UNFD_ST_SUCCESS) {
                nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Write Initial BBT failed with EC: 0x%08x\n", (unsigned int)u32_Err);
                drvNAND_MarkBadBlk(u16_PBA);
                free(pu8_BBT);
                continue;
            }           
            free(pu8_BBT);
        }
        #else
        if(pNandDrv->u8_CellType == NAND_CellType_SLC)  // SLC write dummy to IBBT page
        {
            memset(au8_PageBuf, '\0', pNandDrv->u16_PageByteCnt);
            memset(au8_SpareBuf, 0xFF, pNandDrv->u16_SpareByteCnt);
            u32_Err = NC_WritePages(u32_BlkRow+ga_tPairedPageMap[2].u16_LSB, au8_PageBuf, au8_SpareBuf, 1);
            if (u32_Err != UNFD_ST_SUCCESS) {
                nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Write Paired-Page Map failed with EC: 0x%08x\n", u32_Err);
                drvNAND_MarkBadBlk(u16_PBA);
                continue;
            }
        }
        #endif
            
        /*
        **  Write Paired Page Map to the 4th page
        **/
        if(pNandDrv->u8_CellType == NAND_CellType_MLC)  // MLC
        {
            if(pNandDrv->u8_SlcMode == 0)
            {
                if(drvNAND_IsPageLP(pNandInfo->u8_PairPageMapLoc, &u16_tmp))
                    drvNAND_WriteDummyToPages(
                        ga_tPairedPageMap[WrittenLPIdx].u16_LSB+1, 
                        ga_tPairedPageMap[u16_tmp].u16_LSB-1, u32_BlkRow);
                else
                {   
                    nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "PairPageMapLoc: %Xh is not LSB page\n", 
                        pNandInfo->u8_PairPageMapLoc);
                    return UNFD_ST_ERR_INVALID_PARAM;
                }
            }
            else    //fill dummy page between pni and ppm
                drvNAND_WriteDummyToPages(
                    WrittenPIdx + 1,
                    pNandInfo->u8_PairPageMapLoc-1, u32_BlkRow);
            
            memset(au8_PageBuf, '\0', pNandDrv->u16_PageByteCnt);
            memcpy(au8_PageBuf, &ga_tPairedPageMap, 2048);
            u32_Err = NC_WritePages(u32_BlkRow+pNandInfo->u8_PairPageMapLoc, au8_PageBuf, au8_SpareBuf, 1);
            if(pNandDrv->u8_SlcMode == 0)
                WrittenLPIdx = u16_tmp;
            else
                WrittenPIdx = pNandInfo->u8_PairPageMapLoc;
            if (u32_Err != UNFD_ST_SUCCESS) {
                nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Write Paired-Page Map failed with EC: 0x%08X\n", u32_Err);
                drvNAND_MarkBadBlk(u16_PBA);
                continue;
            }
        }

        /*
        if(pNandDrv->u8_RequireReadRetry && pNandDrv->u8_ReadRetryLoc != 0)  // MLC
        {
            memset(au8_PageBuf, '\0', pNandDrv->u16_PageByteCnt);
            memcpy(au8_PageBuf, &gau8_ReadRetryTable, 512);
            u32_Err = NC_WritePages(u32_PageIdx+pNandInfo->u8_ReadRetryLoc, au8_PageBuf, au8_SpareBuf, 1);
            if (u32_Err != UNFD_ST_SUCCESS) {
                nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Write Customize Read-Retry Table failed with EC: 0x%08lx\n", u32_Err);
                drvNAND_MarkBadBlk(u16_PBA);
                continue;
            }
        }
        */
        #if defined(CONFIG_STB_BFN) && (CONFIG_STB_BFN==1)
        #if defined(CONFIG_STB_BFN_VERSION) && (CONFIG_STB_BFN_VERSION < 5)
        ProgramMiuPage = u32_BlkRow+10;


        if(pNandDrv->u8_CellType == NAND_CellType_MLC && pNandDrv->u8_SlcMode == 0)
        {
             if(drvNAND_IsPageLP(10, &u16_tmp))
                 drvNAND_WriteDummyToPages(
                     ga_tPairedPageMap[WrittenLPIdx].u16_LSB+1, 
                     ga_tPairedPageMap[u16_tmp].u16_LSB-1, u32_BlkRow);
             else
             {   nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "ProgramMiuPage: %Xh is not LSB page\n", 
                     ProgramMiuPage);
                 return UNFD_ST_ERR_INVALID_PARAM;
             }
        }
        else
            drvNAND_WriteDummyToPages(WrittenPIdx+1, 10-1, u32_BlkRow);
        
        
        /* Write STB BFN MIU script(8KB) in page 10 */
        //#define MIU_DRAM_ADDR (DRAM_START_ADDR+0x5000000)
        extern U32Hash0DRAMAddr;
        ProgramMiuAddr = U32Hash0DRAMAddr; //REG(RIU_BASE + (0x1980 << 2) + (0x42 << 2) ) + (REG(RIU_BASE + (0x1980 << 2) + (0x43 << 2) ) << 16);
        printf("ProgramMiuAddr:0x%x\n",ProgramMiuAddr);
        pNandDrv->u16_PageByteCnt = 2048;
        pNandDrv->u16_SpareByteCnt = 0x100;
        pNandDrv->u16_ECCType = NANDINFO_ECC_TYPE;
        #if defined(ECC_TYPE_72BIT1KB) && ECC_TYPE_72BIT1KB
        if(pNandDrv->u16_ECCType == ECC_TYPE_72BIT1KB)
            pNandDrv->u16_SpareByteCnt = 0x180;
        #endif


        NC_ConfigNandFlashContext();
        NC_RegInit();

        pNandDrv->u16_Reg48_Spare |= BIT_NC_HW_AUTO_RANDOM_CMD_DISABLE;
        NC_Config();

        ParamMIUSize = 0x2000;

        while(ParamMIUSize > 0)
        {
            //printf("Write Page, ParamMIUSize left %d \n",(U16)ParamMIUSize);
            printf("Write MIU image in page %d\n",(U16)ProgramMiuPage);
            
            u32_Err = NC_WriteSectors(ProgramMiuPage,0,(U8 *)((ProgramMiuAddr+(ProgramMiuPage-10-u32_BlkRow)*(U32)pNandDrv->u16_SectorByteCnt)), au8_SpareBuf,1);
            drvNAND_IsPageLP(ProgramMiuPage-u32_BlkRow, &WrittenLPIdx);
            WrittenPIdx = ProgramMiuPage-u32_BlkRow;    //for slc and 3d nand with slc mode
            if (u32_Err != UNFD_ST_SUCCESS)
            {
                printf("MIU bin write blk error!!\n");
            }
    
            ParamMIUSize -= pNandDrv->u16_SectorByteCnt;
            ProgramMiuPage ++;
        }
    
        /* Reset NAND driver and FCIE to the original settings */
        pNandDrv->u16_SpareByteCnt = pNandInfo->u16_SpareByteCnt;
        pNandDrv->u16_PageByteCnt  = pNandInfo->u16_PageByteCnt;
        pNandDrv->u16_ECCType      = pNandInfo->u16_ECCType;
        NC_ConfigNandFlashContext();
        NC_RegInit();

        pNandDrv->u16_Reg48_Spare &= ~BIT_NC_HW_AUTO_RANDOM_CMD_DISABLE;
        NC_Config();
        #endif
        #endif

        if(pNandDrv->u8_SlcMode == 0 && pNandDrv->u8_CellType == NAND_CellType_MLC)
            drvNAND_WriteDummyToPages(
                ga_tPairedPageMap[WrittenLPIdx].u16_LSB+1,
                pNandDrv->u16_BlkPageCnt-1, u32_BlkRow);
        else
            drvNAND_WriteDummyToPages(
                WrittenPIdx+1,
                pNandDrv->u16_BlkPageCnt-1, u32_BlkRow);


        printf("CIS%d is written to blk 0x%04x\n", u8_CISIdx, u16_PBA);

        pNandDrv->u16_CISPBA[u8_CISIdx] = u16_PBA;

        if ((++u8_CISIdx) == 2)
            break;
    }
    Disable_Err_log = 0;

    /* Reset NAND driver and FCIE to the original settings */
    pNandDrv->u16_SpareByteCnt = pNandInfo->u16_SpareByteCnt;
    pNandDrv->u16_PageByteCnt  = pNandInfo->u16_PageByteCnt;
    pNandDrv->u16_ECCType      = pNandInfo->u16_ECCType;
    NC_ConfigNandFlashContext();
    NC_RegInit();
    pNandDrv->u16_Reg48_Spare &= ~BIT_NC_HW_AUTO_RANDOM_CMD_DISABLE;
    NC_Config();

    switch (u8_CISIdx) {
        case 0:
            REG(RIU_BASE + (0x1980 << 2) + (0x41 << 2) )  = 0x0002;
            u32_Err = UNFD_ST_ERR_NO_BLK_FOR_CIS0;
            break;
        case 1:
            REG(RIU_BASE + (0x1980 << 2) + (0x41 << 2) )  = 0x0003;
            u32_Err = UNFD_ST_ERR_NO_BLK_FOR_CIS1;
            break;
        case 2:
            u32_Err = UNFD_ST_SUCCESS;
            break;
    }

    return u32_Err;
}


int search_cis_in_DRAM(U8* pu8_CISAddr, U8* pu8_PPMAddr, U8* pu8_PNIAddr, NAND_FLASH_INFO_t *pNandInfoOut)
{
    NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();
    NAND_FLASH_INFO_t * pNandInfo;
    PARTITION_INFO_t * pPartInfo;
    U32 u32_i, u32_j, u32_Err, u32_ChkSum;
    U8 u8_IsEnd = 0;

    REG(RIU_BASE + (0x1980 << 2) + (0x41 << 2) )  = 0x0000;

    NC_PlatformInit();

    u32_Err = NC_ResetNandFlash();
    if(u32_Err != UNFD_ST_SUCCESS)
    {
        nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1,"NAND Reset Fail\n");
        REG(RIU_BASE + (0x1980 << 2) + (0x41 << 2) )  = 0x0002;
        return -1;
    }

    u32_Err = NC_ReadID();
    if(u32_Err!= UNFD_ST_SUCCESS)
    {
        nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Read ID Fail\n");
        REG(RIU_BASE + (0x1980 << 2) + (0x41 << 2) )  = 0x0003;
        return -1;
    }
    printf("Begin Search CIS in DRAM\n");

    //Search 20 pieces of possiable cis memory
    //for(u32_i = 0; u32_i < 0x200 * 20; u32_i += 0x200)
    u32_i = 0;
    while(1)
    {
        pNandInfo = (NAND_FLASH_INFO_t*)(pu8_CISAddr + u32_i);

        for(u32_j=0; u32_j<pNandInfo->u8_IDByteCnt; u32_j++)
        {
            if(pNandInfo->au8_ID[u32_j] != pNandDrv->au8_ID[u32_j])
                break;
        }
        if(u32_j == pNandInfo->u8_IDByteCnt)
        {
            pNandDrv->u8_IDByteCnt = pNandInfo->u8_IDByteCnt;
            break;
        }
        
        //for(u32_k=0; u32_k<pNandInfo->u8_IDByteCnt; u32_k++)
        //    printf("%Xh ", pNandInfo->au8_ID[u32_k]);
        //printf("\n");

        // used to check the end of nni in DRAM
        u32_ChkSum = drvNAND_CheckSum(((void*)pNandInfo) + 0x24, 0x32 - 0x24);
        if (drvNAND_CompareCISTag(pNandInfo->au8_Tag) ||(u32_ChkSum != pNandInfo->u32_ChkSum))
        {
            u8_IsEnd = 1;
            break;
        }
        u32_i += 0x200;
    }
    if(u8_IsEnd)
    {
        printf("No available CIS match with current nand flash\n");
        REG(RIU_BASE + (0x1980 << 2) + (0x41 << 2))  = 0x0004;
        return -1;
    }
    
    printf("NAND ID:");
    for(u32_j=0; u32_j<pNandInfo->u8_IDByteCnt; u32_j++)
    {
        printf("0x%X ", pNandDrv->au8_ID[u32_j]);
    }
    printf("\nFound CIS in given memory\n");
    drvNAND_ParseNandInfo(pNandInfo);
    //dump_nand_info(pNandInfo);

    NC_ConfigNandFlashContext();
    /*Re-Search DDR timing again*/
    u32_Err = NC_Init();
    if(u32_Err != UNFD_ST_SUCCESS)
    {
        nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1,"NC_Init Fail ErrCode: %X\n", u32_Err);
        REG(RIU_BASE + (0x1980 << 2) + (0x41 << 2) )  = 0x0005;
        return -1;
    }
    NC_Config();
    nand_config_clock(pNandInfo->u16_tRC);
     //Read Retry
    if(pNandDrv->u8_RequireReadRetry)
    {
        NC_SetupReadRetryCmd();
        NC_GetRegDefaultValue();
    }
    //search pni
    pNandDrv->u8_HasPNI = 0;
    if(pu8_PNIAddr != NULL)
    {
        for(u32_i = 0; u32_i < NAND_PARTITAION_BYTE_CNT * 128 ; u32_i += NAND_PARTITAION_BYTE_CNT)
        {
            pPartInfo = (PARTITION_INFO_t*) (pu8_PNIAddr + u32_i);

            if(pPartInfo->u16_SpareByteCnt == pNandDrv->u16_SpareByteCnt &&
                pPartInfo->u16_PageByteCnt == pNandDrv->u16_PageByteCnt &&
                pPartInfo->u16_BlkPageCnt == pNandDrv->u16_BlkPageCnt &&
                pPartInfo->u16_BlkCnt == pNandDrv->u16_BlkCnt) // Match Partition layout
            {
                memcpy(pNandDrv->pPartInfo, pPartInfo, NAND_PARTITAION_BYTE_CNT);
                dump_part_info(pNandDrv->pPartInfo);
                pNandDrv->u8_HasPNI = 1;
                break;
            }
            //printf("%4X %4X %4X %4X \n", pPartInfo->u16_SpareByteCnt, pPartInfo->u16_PageByteCnt, 
            //    pPartInfo->u16_BlkPageCnt, pPartInfo->u16_BlkCnt);
            //printf("%4X %4X %4X %4X \n", pNandDrv->u16_SpareByteCnt, pNandDrv->u16_PageByteCnt, 
            //    pNandDrv->u16_BlkPageCnt, pNandDrv->u16_BlkCnt);
        }
    }
    
    //memcpy pair page map
    printf("\nCell Type: %s \nRandomizer: %s\n", 
        pNandDrv->u8_CellType==NAND_CellType_MLC?"MLC":"SLC", 
        pNandDrv->u8_RequireRandomizer?"Yes":"No");
    
    if(pNandDrv->u8_CellType && pu8_PPMAddr!=NULL)
        memcpy(&ga_tPairedPageMap, pu8_PPMAddr + (pNandInfo->u8_PairPageMapType * 0x800), 0x800);
    else
    {
        for(u32_i = 0; u32_i < 0x200; u32_i++)
        {
            ga_tPairedPageMap[u32_i].u16_LSB = u32_i;
            ga_tPairedPageMap[u32_i].u16_MSB = u32_i;
        }
    }
    // BlkLowPCnt
    for(u32_i=0; u32_i<pNandDrv->u16_BlkPageCnt; u32_i++)
        if(ga_tPairedPageMap[u32_i].u16_LSB==pNandDrv->u16_BlkPageCnt-1 ||
            ga_tPairedPageMap[u32_i].u16_MSB==pNandDrv->u16_BlkPageCnt-1)
            break;
    pNandDrv->u16_BlkLowPCnt = u32_i+1;
        
    printf("Read Retry: %s\n\n", pNandDrv->u8_RequireReadRetry?"Yes":"No");

    memcpy(pNandInfoOut, pNandInfo, sizeof(NAND_FLASH_INFO_t));

    return 0;
}

int nand_write_block_slcmode(U32 u32_BlkRow, U8* p_buffer, U32 u32_PageCnt, U32 (*nand_markbad)(U32))
{
    NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();

    int i, j, BlkPageCnt;
    U32 u32_TmpRow, u32_Err;
    U8 *pu8_buffer = p_buffer;
    U8 *pu8_PageDataBuf = pNandDrv->pu8_PageDataBuf;    
    U8 *pu8_PageSpareBuf = pNandDrv->pu8_PageSpareBuf;
    BlkPageCnt = pNandDrv->u16_BlkLowPCnt;

    memset(pu8_PageDataBuf, 0x5a, pNandDrv->u16_PageByteCnt);
    memset(pu8_PageSpareBuf, 0xff, pNandDrv->u16_SpareByteCnt);

    for(i = 0; i < u32_PageCnt; i ++)
    {
        u32_TmpRow = u32_BlkRow + ga_tPairedPageMap[i].u16_LSB;
        #if defined(CONFIG_MLC_CONFIG_FOR_MP)
        if(pNandDrv->u8_CellType)
        {
            for(sectoridx = 0; sectoridx < pNandDrv->u16_PageSectorCnt; sectoridx ++)
            {
                pu8_PageSpareBuf[0x5 + sectoridx * pNandDrv->u16_SectorSpareByteCnt] = 0xDE;
                pu8_PageSpareBuf[0x6 + sectoridx * pNandDrv->u16_SectorSpareByteCnt] = 0xAD;
            }
        }
        #endif        
        
        u32_Err = drvNAND_LFSRWritePhyPage(u32_TmpRow, pu8_buffer, pu8_PageSpareBuf);
        if(u32_Err)
        {
            printf ("NAND write to offset %x failed %d\n",
                u32_TmpRow, u32_Err);
            printf("Mark current block as bad block\n");
            //mark bad
            if(nand_markbad == NULL)
                drvNAND_MarkBadBlk(u32_BlkRow >> pNandDrv->u8_BlkPageCntBits);
            else
                nand_markbad(u32_BlkRow >> pNandDrv->u8_BlkPageCntBits);

            pu8_buffer -= pNandDrv->u16_PageByteCnt * i;
            u32_BlkRow += pNandDrv->u16_BlkPageCnt;

            return -1;
        }
        pu8_buffer += pNandDrv->u16_PageByteCnt;

        //Write Dummy data to MSB Page of MLC NAND
        if(i != BlkPageCnt - 1)
        {
            if(ga_tPairedPageMap[i].u16_LSB != (ga_tPairedPageMap[i + 1].u16_LSB - 1))
            {
                for(j = ga_tPairedPageMap[i].u16_LSB + 1; j < ga_tPairedPageMap[i + 1].u16_LSB; j ++)
                {
                    u32_Err = drvNAND_LFSRWritePhyPage(u32_BlkRow + j, pu8_PageDataBuf, NULL);
                    if(u32_Err)
                    {
                        printf ("NAND write to offset %x failed %d\n",
                            u32_BlkRow + j, u32_Err);
                        printf("Mark current block as bad block\n");
                        //mark bad
                        if(nand_markbad == NULL)
                            drvNAND_MarkBadBlk(u32_BlkRow >> pNandDrv->u8_BlkPageCntBits);
                        else
                            nand_markbad(u32_BlkRow >> pNandDrv->u8_BlkPageCntBits);

                        pu8_buffer -= pNandDrv->u16_PageByteCnt * i;
                        u32_BlkRow += pNandDrv->u16_BlkPageCnt;

                        return -1;
                    }
                }
            }
        }
        else{
            for(j = ga_tPairedPageMap[i].u16_LSB + 1; j < pNandDrv->u16_BlkPageCnt; j ++)
            {
                u32_Err = drvNAND_LFSRWritePhyPage(u32_BlkRow + j, pu8_PageDataBuf, NULL);
                if(u32_Err)
                {
                    printf ("NAND write to offset %xh failed %d\n",
                        u32_BlkRow + j, u32_Err);
                    printf("Mark current block as bad block\n");
                    //mark bad
                    if(nand_markbad == NULL)
                        drvNAND_MarkBadBlk(u32_BlkRow >> pNandDrv->u8_BlkPageCntBits);
                    else
                        nand_markbad(u32_BlkRow >> pNandDrv->u8_BlkPageCntBits);

                    pu8_buffer -= pNandDrv->u16_PageByteCnt * i;
                    u32_BlkRow += pNandDrv->u16_BlkPageCnt;

                    return -1;
                }
            }
        }
    }
    return 0;
}

int nand_write_bootloader(U32 u32_Row,U8 * pu8_addr, U32 u32_size, U8 u8_BootStageId)
{
    NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();

    U8 * pu8_DataBuf = pu8_addr;
    U16 u16_BlkPageCnt;
    U32 u32_pagecnt, u32_Err, u32_size_tmp = u32_size, u32_Row_Read = u32_Row;
    U8 u8_IdWritten = 0;
    
    u16_BlkPageCnt = (pNandDrv->u8_CellType && pNandDrv->u8_SlcMode == 0) ? pNandDrv->u16_BlkPageCnt/2: pNandDrv->u16_BlkPageCnt;
    u32_pagecnt = u32_size >> pNandDrv->u8_PageByteCntBits;
    
    memset(u8SpareBuf, 0xFF, pNandDrv->u16_SpareByteCnt);    
    //add register status for polling by TV Tool
    REG(RIU_BASE + (0x1980 << 2) + (0x41 << 2) )  = 0x0000;

    if(pNandDrv->u8_RequireRandomizer)
        NC_EnableLFSR();

    while(u32_pagecnt >= u16_BlkPageCnt)
    {
        while (drvNAND_IsGoodBlk(u32_Row >> pNandDrv->u8_BlkPageCntBits) == 0)
        {
            u32_Row += pNandDrv->u16_BlkPageCnt;
            //bad block jump to next block
            if(u32_Row == (pNandDrv->u16_BlkCnt << pNandDrv->u8_BlkPageCntBits))
            {
                printf("Error : There is no available GOOD block in current nand device\n");
                REG(RIU_BASE + (0x1980 << 2) + (0x41 << 2) )  = 0x0004;
                return -1;
            }
        }
        u32_Err = NC_EraseBlk(u32_Row);
        if(u32_Err != UNFD_ST_SUCCESS)
        {
            drvNAND_MarkBadBlk(u32_Row >> pNandDrv->u8_BlkPageCntBits);
            //jump to next block
            u32_Row += pNandDrv->u16_BlkPageCnt;
            continue;
        }

        if(u8_IdWritten == 0)
            u8SpareBuf[1] = u8_BootStageId;
        else
            u8SpareBuf[1] = 0xFF;

        if(pNandDrv->u8_CellType && pNandDrv->u8_SlcMode == 0)
        {
            U16 u16_i, u16_flag = 0;
            U32 u32_TmpRow;
            for(u16_i =0; u16_i < u16_BlkPageCnt; u16_i ++)
            {
                u32_TmpRow = u32_Row + ga_tPairedPageMap[u16_i].u16_LSB;
                u32_Err = NC_WritePages(u32_TmpRow, pu8_DataBuf, u8SpareBuf, 1);
                if(u32_Err != UNFD_ST_SUCCESS)
                {
                    drvNAND_MarkBadBlk(u32_Row >> pNandDrv->u8_BlkPageCntBits);
                    //jump to next block
                    u32_Row += pNandDrv->u16_BlkPageCnt;
                    u16_flag = 1;
                    break;
                }
                pu8_DataBuf += pNandDrv->u16_PageByteCnt;
            }
            if(u16_flag == 1)
            {
                pu8_DataBuf -= pNandDrv->u16_PageByteCnt * u16_i;
                continue;
            }
        }
        else
        {
            u32_Err = NC_WritePages(u32_Row, pu8_DataBuf, u8SpareBuf, pNandDrv->u16_BlkPageCnt);
            if(u32_Err != UNFD_ST_SUCCESS)
            {
                drvNAND_MarkBadBlk(u32_Row >> pNandDrv->u8_BlkPageCntBits);
                //jump to next block
                u32_Row += pNandDrv->u16_BlkPageCnt;
                continue;
            }
            pu8_DataBuf += (u16_BlkPageCnt << pNandDrv->u8_PageByteCntBits);
        }
        u8_IdWritten = 1;

        u32_pagecnt -= u16_BlkPageCnt;
        u32_size -= (u16_BlkPageCnt << pNandDrv->u8_PageByteCntBits);
        u32_Row += pNandDrv->u16_BlkPageCnt;
    }

    while(u32_size)
    {
        while (drvNAND_IsGoodBlk(u32_Row >> pNandDrv->u8_BlkPageCntBits) == 0)
        {
            u32_Row += pNandDrv->u16_BlkPageCnt;
            //bad block jump to next block
            if(u32_Row == (pNandDrv->u16_BlkCnt << pNandDrv->u8_BlkPageCntBits))
            {
                printf("Error: There is no available GOOD block in current nand device\n");
                REG(RIU_BASE + (0x1980 << 2) + (0x41 << 2) )  = 0x0004;
                return -1;
            }
        }

        u32_Err = NC_EraseBlk(u32_Row);
        if(u32_Err != UNFD_ST_SUCCESS)
        {
            drvNAND_MarkBadBlk(u32_Row >> pNandDrv->u8_BlkPageCntBits);
            //jump to next block
            u32_Row += pNandDrv->u16_BlkPageCnt;
            continue;
        }

        if(u8_IdWritten == 0)
            u8SpareBuf[1] = u8_BootStageId;
        else
            u8SpareBuf[1] = 0xFF;

        u32_pagecnt = u32_size >> pNandDrv->u8_PageByteCntBits;
        if(u32_size & pNandDrv->u16_PageByteCntMask)
             u32_pagecnt += 1;

        if(pNandDrv->u8_CellType && pNandDrv->u8_SlcMode == 0)
        {
            U16 u16_i, u16_flag = 0;
            U32 u32_TmpRow;
            for(u16_i =0; u16_i < u32_pagecnt; u16_i ++)
            {
                u32_TmpRow = u32_Row + ga_tPairedPageMap[u16_i].u16_LSB;
                u32_Err = NC_WritePages(u32_TmpRow, pu8_DataBuf, u8SpareBuf, 1);
                if(u32_Err != UNFD_ST_SUCCESS)
                {
                    drvNAND_MarkBadBlk(u32_Row >> pNandDrv->u8_BlkPageCntBits);
                    //jump to next block
                    u32_Row += pNandDrv->u16_BlkPageCnt;
                    u16_flag = 1;
                    break;
                }
                pu8_DataBuf += pNandDrv->u16_PageByteCnt;
            }
            if(u16_flag == 1)
            {
                pu8_DataBuf -= pNandDrv->u16_PageByteCnt * u16_i;
                continue;
            }
        }
        else
        {
            u32_Err = NC_WritePages(u32_Row, pu8_DataBuf, u8SpareBuf, u32_pagecnt);
            if(u32_Err != UNFD_ST_SUCCESS)
            {
                drvNAND_MarkBadBlk(u32_Row >> pNandDrv->u8_BlkPageCntBits);
                //jump to next block
                u32_Row += pNandDrv->u16_BlkPageCnt;
                continue;
            }
        }
        u8_IdWritten = 1;

        u32_size-= u32_size;
    }
    
    //finish programming
    //Add register checking
    //read back check
    u32_size = u32_size_tmp;
    u32_pagecnt = u32_size >> pNandDrv->u8_PageByteCntBits;
    pu8_DataBuf = pu8_addr;
    u32_Row = u32_Row_Read;

    while(u32_pagecnt >= u16_BlkPageCnt)
    {
        while (drvNAND_IsGoodBlk(u32_Row >> pNandDrv->u8_BlkPageCntBits) == 0)
        {
            u32_Row += pNandDrv->u16_BlkPageCnt;
            //bad block jump to next block
            if(u32_Row == (pNandDrv->u16_BlkCnt << pNandDrv->u8_BlkPageCntBits))
            {
                printf("Error : There is no available GOOD block in current nand device\n");
                REG(RIU_BASE + (0x1980 << 2) + (0x41 << 2) )  = 0x0004;
                return -1;
            }
        }

        {
            U16 u16_i;
            U32 u32_TmpRow;
            for(u16_i =0; u16_i < u16_BlkPageCnt; u16_i ++)
            {
                if(pNandDrv->u8_CellType && pNandDrv->u8_SlcMode == 0)
                    u32_TmpRow = u32_Row + ga_tPairedPageMap[u16_i].u16_LSB;
                else
                    u32_TmpRow = u32_Row + u16_i;

                u32_Err = NC_ReadPages(u32_TmpRow, u8MainBuf, u8SpareBuf, 1);
                if(u32_Err != UNFD_ST_SUCCESS)
                {
                    printf("Error: ECC fail when read back checking\n");
                    REG(RIU_BASE + (0x1980 << 2) + (0x41 << 2) )  = 0x0002;
                    return -1;
                }
                if(memcmp(u8MainBuf, pu8_DataBuf, pNandDrv->u16_PageByteCnt))
                {
                    printf("Error: compare fail when read back checking\n");
                    printf("RAM--->\n");
                    dump_mem((unsigned char *) pu8_DataBuf, pNandDrv->u16_PageByteCnt);
                    printf("NAND--->\n");
                    dump_mem((unsigned char *) u8MainBuf, pNandDrv->u16_PageByteCnt);
                    REG(RIU_BASE + (0x1980 << 2) + (0x41 << 2) )  = 0x0003;
                    return -1;
                }
                pu8_DataBuf += pNandDrv->u16_PageByteCnt;
            }
        }

        u32_pagecnt -= u16_BlkPageCnt;
        u32_size -= (u16_BlkPageCnt << pNandDrv->u8_PageByteCntBits);
        u32_Row += pNandDrv->u16_BlkPageCnt;
    }

    while(u32_size)
    {
        while (drvNAND_IsGoodBlk(u32_Row >> pNandDrv->u8_BlkPageCntBits) == 0)
        {
            u32_Row += pNandDrv->u16_BlkPageCnt;
            //bad block jump to next block
            if(u32_Row == (pNandDrv->u16_BlkCnt << pNandDrv->u8_BlkPageCntBits))
            {
                printf("Error : There is no available GOOD block in current nand device\n");
                REG(RIU_BASE + (0x1980 << 2) + (0x41 << 2) )  = 0x0004;
                return -1;
            }
        }

        u32_pagecnt = u32_size >> pNandDrv->u8_PageByteCntBits;
        if((u32_size & (pNandDrv->u16_PageByteCnt -1)))
             u32_pagecnt += 1;

        {
            U16 u16_i;
            U32 u32_TmpRow;
            for(u16_i =0; u16_i < u32_pagecnt; u16_i ++)
            {
                if(pNandDrv->u8_CellType && pNandDrv->u8_SlcMode == 0)
                    u32_TmpRow = u32_Row + ga_tPairedPageMap[u16_i].u16_LSB;
                else
                    u32_TmpRow = u32_Row + u16_i;

                u32_Err = NC_ReadPages(u32_TmpRow, u8MainBuf, u8SpareBuf, 1);
                if(u32_Err != UNFD_ST_SUCCESS)
                {
                    printf("Error: ECC fail when read back checking\n");
                    REG(RIU_BASE + (0x1980 << 2) + (0x41 << 2) )  = 0x0002;
                    return -1;
                }
                if(memcmp(u8MainBuf, pu8_DataBuf, pNandDrv->u16_PageByteCnt))
                {
                    printf("Error: compare fail when read back checking\n");
                    printf("RAM--->\n");
                    dump_mem((unsigned char *) pu8_DataBuf, pNandDrv->u16_PageByteCnt);
                    printf("NAND--->\n");
                    dump_mem((unsigned char *) u8MainBuf, pNandDrv->u16_PageByteCnt);
                    REG(RIU_BASE + (0x1980 << 2) + (0x41 << 2) )  = 0x0003;
                    return -1;
                }

                pu8_DataBuf += pNandDrv->u16_PageByteCnt;
            }
        }

        u32_size-= u32_size;
    }
    //Register status Ok
    REG(RIU_BASE + (0x1980 << 2) + (0x41 << 2) )  = 0x0001;
    return 0;
}

/*
 *For SLC, full block program is not necessary
 *Turn on this switch on MLC
 */
#if 0
#ifdef FULL_BLOCK_PROGRAM_SCRUB
static u32 empty_check(const void *buf, u32 len)
{
    int i;
    NAND_DRIVER * pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();

    //if ((!len) || (len & (meminfo->oobblock + meminfo->oobsize - 1)))
        //return -1;

    for (i = (len >> 2) - 1; i >= 0; i--)
        if (((const uint32_t *)buf)[i] != 0xFFFFFFFF)
            break;

    /* The resulting length must be aligned to the minimum flash I/O size */
    len = ALIGN((i + 1) << 2, pNandDrv->u16_PageByteCnt + pNandDrv->u16_SpareByteCnt);
    return len;
}
#endif
#endif

int drvNAND_ReadCISBlk(U32 u32_off, U8* pu8_DataBuf, u16 u16_phySpareByteCnt)
{
    NAND_DRIVER * pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();
    U32 u32_Ret, i;
    U32 u32_Row = u32_off >> pNandDrv->u8_PageByteCntBits;
    U16 u16_PageByteCntTmp, u16_SpareByteCntTmp, u16_ECCTypeTmp;
    U32 page_off = pNandDrv->u16_PageByteCnt + u16_phySpareByteCnt;
    int ProgramMiuPage = 10;
    U32 u32_ChkSum;

    //set 0xff for nni page
    memset((void*)pu8_DataBuf, 0xff, page_off);
    u16_PageByteCntTmp = pNandDrv->u16_PageByteCnt;
    u16_SpareByteCntTmp = pNandDrv->u16_SpareByteCnt;
    u16_ECCTypeTmp = pNandDrv->u16_ECCType;

    pNandDrv->u16_PageByteCnt = 0x800;
    pNandDrv->u16_SpareByteCnt = 0x100;
    pNandDrv->u16_ECCType = NANDINFO_ECC_TYPE;
    #if defined(ECC_TYPE_72BIT1KB) && ECC_TYPE_72BIT1KB
    if(pNandDrv->u16_ECCType == ECC_TYPE_72BIT1KB)
        pNandDrv->u16_SpareByteCnt = 0x180;
    #endif

    NC_ConfigNandFlashContext();
    NC_RegInit();
    pNandDrv->u16_Reg48_Spare |= (1 << 12);
    NC_Config();

    u32_Ret = NC_ReadSectors_Retry(u32_Row, 0, pu8_DataBuf, (U8*)((U32)pu8_DataBuf + pNandDrv->u16_SectorByteCnt), 1);


    //restore original setting
    pNandDrv->u16_PageByteCnt = u16_PageByteCntTmp;
    pNandDrv->u16_SpareByteCnt = u16_SpareByteCntTmp;
    pNandDrv->u16_ECCType = u16_ECCTypeTmp;
    //pglen = pNandDrv->u16_PageByteCnt + pNandDrv->u16_SpareByteCnt;

    NC_ConfigNandFlashContext();
    NC_RegInit();
    pNandDrv->u16_Reg48_Spare &= ~(1 << 12);
    NC_Config();

    if(u32_Ret != 0)
        return u32_Ret;

    if(pNandDrv->u8_HasPNI == 1)
        u32_Ret = NC_ReadSectors_Retry(u32_Row+1, 0, (U8*) ((U32)pu8_DataBuf + page_off),
        (U8*) ((U32)pu8_DataBuf + page_off + pNandDrv->u16_PageByteCnt), 1);
    else
        u32_Ret = NC_ReadPages(u32_Row + 1, (U8*) ((U32)pu8_DataBuf + page_off),
                            (U8*) ((U32)pu8_DataBuf + page_off + pNandDrv->u16_PageByteCnt), 1);
    if(u32_Ret != 0)
        return u32_Ret;

    //Read page2~page9 with normal ecc type
    for(i = 2 ; i < ProgramMiuPage; i ++)
    {
        u32_Ret = NC_ReadPages(u32_Row + i, (U8*) ((U32)pu8_DataBuf + page_off * i),
                            (U8*) ((U32)pu8_DataBuf + (page_off * i) + pNandDrv->u16_PageByteCnt), 1);
        if(u32_Ret)
            return u32_Ret;
        if(i == 2 && pNandDrv->u8_CellType == 0) //slc check initial bbt for unfd ftl
        {
            INIT_BBT_t *pInitBBT = (INIT_BBT_t *)(pu8_DataBuf+ page_off * i);
            u32_ChkSum = drvNAND_CheckSum((U8*)(&(pInitBBT->u16_Cnt)), (512-4));
            if (u32_ChkSum == pInitBBT->u32_ChkSum) //if IBBT is found, don't dump the IBBT, else dump the page data
            {
                memset((pu8_DataBuf+ page_off * i), 0xff, pNandDrv->u16_PageByteCnt);
                memset((pu8_DataBuf+ page_off * i)+ pNandDrv->u16_PageByteCnt, 0xff, pNandDrv->u16_SpareByteCnt);
            }
        }
        /*
         *For SLC, full block program is not necessary
         *Turn on this switch on MLC
         */
        #if 0
        #ifdef FULL_BLOCK_PROGRAM_SCRUB
        {
            U32 chk_len;
            chk_len = empty_check((pu8_DataBuf + page_off * i), pglen);
            if (!chk_len)
            {
                (pu8_DataBuf + page_off * i)[0] = 0xfe; // indicate empty page
            }
        }
        #endif
        #endif
    }

    //If BFN board read the left pages(MIU) with special ecc type
    #if (defined(CONFIG_STB_BFN) && (CONFIG_STB_BFN==1)) && \
        (defined(CONFIG_STB_BFN_VERSION) && (CONFIG_STB_BFN_VERSION < 5))
    pNandDrv->u16_PageByteCnt = 2048;
    pNandDrv->u16_SpareByteCnt = 256;
    pNandDrv->u16_ECCType = NANDINFO_ECC_TYPE;
    #if defined(ECC_TYPE_72BIT1KB) && ECC_TYPE_72BIT1KB
    if(pNandDrv->u16_ECCType == ECC_TYPE_72BIT1KB)
        pNandDrv->u16_SpareByteCnt = 0x180;
    #endif
    NC_ConfigNandFlashContext();
    NC_RegInit();
    pNandDrv->u16_Reg48_Spare |= (1 << 12);
    NC_Config();

    for(; i < pNandDrv->u16_BlkPageCnt; i++){
        u32_Ret = NC_ReadSectors_Retry(u32_Row + i, 0, (U8*) ((U32)pu8_DataBuf + page_off * i),
                  (U8*) ((U32)pu8_DataBuf + (page_off * i) + pNandDrv->u16_SectorByteCnt), 1);
        if(u32_Ret != 0)
            return u32_Ret;
    }

    /* Reset NAND driver and FCIE to the original settings */
    //restore original setting
    pNandDrv->u16_PageByteCnt = u16_PageByteCntTmp;
    pNandDrv->u16_SpareByteCnt = u16_SpareByteCntTmp;
    pNandDrv->u16_ECCType = u16_ECCTypeTmp;
    //pglen = pNandDrv->u16_PageByteCnt + pNandDrv->u16_SpareByteCnt;

    NC_ConfigNandFlashContext();
    NC_RegInit();
    pNandDrv->u16_Reg48_Spare &= ~(1 << 12);
    NC_Config();

    #else

    for(; i < pNandDrv->u16_BlkPageCnt; i++){
        u32_Ret = NC_ReadPages(u32_Row + i, (U8*) ((U32)pu8_DataBuf + page_off * i),
                            (U8*) ((U32)pu8_DataBuf + (page_off * i) + pNandDrv->u16_PageByteCnt), 1);
        if(u32_Ret)
            return u32_Ret;
    }

    #endif

    return UNFD_ST_SUCCESS;
}

void drvNAND_GetMtdParts(char *buf)
{
    NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
    PARTITION_INFO_t *pPartInfo = pNandDrv->pPartInfo;
    U8 u8_i, u8_Unknown = 0;
    int len, Maxlen = 512;
    u32 PartSize = 0, PartSize1 = 0, PartSize2 = 0;
    U32 u32_Err;
    u32 virtual_boot;
    U32 u32_BlkIdx;
    U8  u8_MbootCnt= 0;
    U32 u32_MbootBlkIdx[2];
    char *mtd_buf=buf, tmp[128], PartName[16];
    //U16 u16_LastPartType;

    #if defined(CONFIG_MSTAR_TOOL_ROM_PROGRAM_NAND_BIN)
        goto cleanup;
    #endif
    
    if(pNandDrv->u8_HasPNI == 0)
        goto cleanup;

    sprintf(mtd_buf, "mtdparts=edb64M-nand:");
    //printf("%s\r\n",buf);
    mtd_buf += 21;
    if (Maxlen < 21)
        goto cleanup;
    Maxlen -= 21;

    //u16_LastPartType = 0;
    for(u8_i = 0; u8_i < pPartInfo->u16_PartCnt; u8_i ++)
    {
        if( (pPartInfo->records[u8_i].u16_PartType & UNFD_LOGI_PART) == UNFD_LOGI_PART)
            break;

        PartSize = 0;
        if( u8_i )
        {
            memcpy(mtd_buf, ",", 1);
            mtd_buf ++;
            Maxlen --;
        }

        PartSize = pNandDrv->u16_BlkPageCnt * pNandDrv->u16_PageByteCnt *
            (pPartInfo->records[u8_i].u16_BackupBlkCnt + pPartInfo->records[u8_i].u16_BlkCnt);

        //u8_PartNo = u16_LastPartType != pPartInfo->records[u8_i].u16_PartType ? 0:1;

        //u16_LastPartType = pPartInfo->records[u8_i].u16_PartType;

        //partition name should sync with nand_SyncMtdPartFor1stBoot
        switch(pPartInfo->records[u8_i].u16_PartType)
        {
            case UNFD_PART_BOOTLOGO:
                sprintf(PartName,"UBILD" );
                break;
            case UNFD_PART_UBOOT:
                sprintf(PartName,"MBOOT" );
                break;
            case UNFD_PART_OPTEE:
                sprintf(PartName,"OPTEE" );
                break;
            case UNFD_PART_ARMFW:
                sprintf(PartName,"ARMFW" );
                break;
            case UNFD_PART_TEEKB:
                sprintf(PartName,"TEEKB" );
                break;
             default:
                sprintf(PartName,"CUS%d", u8_Unknown++);
                break;
        }
        if (u8_i)
            sprintf(tmp, "%dk(%s)", PartSize/1024, PartName); //,pPartInfo->records[u8_i].u16_PartType);
        else
        {
            if(pPartInfo->records[u8_i].u16_PartType == UNFD_PART_UBOOT)
            {
                u8_MbootCnt = 0;
                if(pNandDrv->u8_BL1PBA != 0)
                {
                    //search vid chunk header for mboot partition beginning
                    u32_BlkIdx = 1;
                    while(1)
                    {
                        u32_Err = NC_ReadPages((pNandDrv->u8_BL1PBA + u32_BlkIdx) << pNandDrv->u8_BlkPageCntBits, u8MainBuf, u8SpareBuf, 1);
                        //binary ID for chunk header
                        if( ((U32 *)u8MainBuf)[0x7] == 0x0000B007 && u32_Err == UNFD_ST_SUCCESS)
                        {
                            u32_MbootBlkIdx[u8_MbootCnt] = (pNandDrv->u8_BL1PBA + u32_BlkIdx);
                            u8_MbootCnt ++;
                        }
                        #if (ENABLE_MODULE_DISABLE_MBOOTBAK == 1)
                        if(u8_MbootCnt == 1)
                            break;
                        #else
                        if(u8_MbootCnt == 2)
                            break;
                        #endif

                        if((pNandDrv->u8_BL1PBA + u32_BlkIdx) == 0xFF)
                            break;

                        u32_BlkIdx ++;
                    }
                    if((pNandDrv->u8_BL1PBA + u32_BlkIdx) == 0xFF)
                    {
                        //only one mboot or upgrade power cut
                        //goto cleanup;
                        if(u8_MbootCnt == 1)        //one mboot ->powercut during upgrade->layout ubild
                        {
                            virtual_boot = u32_MbootBlkIdx[0] * (pNandDrv->u16_BlkPageCnt * pNandDrv->u16_PageByteCnt) / 1024;
                            PartSize = PartSize - (u32_MbootBlkIdx[0] - 10) * (pNandDrv->u16_BlkPageCnt * pNandDrv->u16_PageByteCnt) ;
                            sprintf(tmp, "%dk@%dk(%s)", PartSize/1024, virtual_boot, PartName);
                            #if defined(UBOOT_RECOVERY)
                            REG_WRITE_UINT16(UBOOT_RECOVERY, 0x1);
                            #endif
                            printf("Warning: Only One Mboot found, Please re-upgrade your mboot, in case of read ecc fail\n");
                        }
                        else    //no mboot  using default for ubild partition, impossible in normal case or tool programming case
                            goto cleanup;
                    }
                    else
                    {
                         if(u8_MbootCnt == 1)
                        {
                            virtual_boot = u32_MbootBlkIdx[0] * (pNandDrv->u16_BlkPageCnt * pNandDrv->u16_PageByteCnt) / 1024;
                            PartSize = PartSize - (u32_MbootBlkIdx[0] - 10) * (pNandDrv->u16_BlkPageCnt * pNandDrv->u16_PageByteCnt) ;
                            sprintf(tmp, "%dk@%dk(%s)", PartSize/1024, virtual_boot, PartName);
                        }
                        else if(u8_MbootCnt == 2)
                        {
                            virtual_boot = u32_MbootBlkIdx[0] * (pNandDrv->u16_BlkPageCnt * pNandDrv->u16_PageByteCnt) / 1024;
                            PartSize1 = (u32_MbootBlkIdx[1] - u32_MbootBlkIdx[0]) * (pNandDrv->u16_BlkPageCnt * pNandDrv->u16_PageByteCnt) ;
                            PartSize2 = PartSize - (u32_MbootBlkIdx[1] - 10) * (pNandDrv->u16_BlkPageCnt * pNandDrv->u16_PageByteCnt) ;
                            sprintf(tmp, "%dk@%dk(%s),%dk(%sBAK)", PartSize1/1024, virtual_boot, PartName,PartSize2/1024, PartName);
                        }
                        else
                            goto cleanup;

                        //virtual_boot = (pNandDrv->u8_BL1PBA + u32_BlkIdx) * (pNandDrv->u16_BlkPageCnt * pNandDrv->u16_PageByteCnt) / 1024;
                        //PartSize = PartSize - ((pNandDrv->u8_BL1PBA + u32_BlkIdx - 10) * (pNandDrv->u16_BlkPageCnt * pNandDrv->u16_PageByteCnt));
                    }
                }
                else if(pNandDrv->u8_HashPBA[1][1] != 0)
                {
                    u32_BlkIdx = 1;
                    while(1)
                    {
                        u32_Err = NC_ReadPages((pNandDrv->u8_HashPBA[1][1] + u32_BlkIdx) << pNandDrv->u8_BlkPageCntBits, u8MainBuf, u8SpareBuf, 1);
                        //binary ID for chunk header
                        if( ((U32 *)u8MainBuf)[0x7] == 0x0000B007 && u32_Err == UNFD_ST_SUCCESS)
                        {
                            u32_MbootBlkIdx[u8_MbootCnt] = (pNandDrv->u8_HashPBA[1][1] + u32_BlkIdx);
                            u8_MbootCnt ++;
                        }
                        if(u8_MbootCnt == 2)
                            break;
                        if((pNandDrv->u8_HashPBA[1][1] + u32_BlkIdx) == 0xFF)
                            break;

                        u32_BlkIdx ++;
                    }
                    if((pNandDrv->u8_HashPBA[1][1] + u32_BlkIdx) == 0xFF)
                    {
                        //only one mboot or upgrade power cut
                        //goto cleanup;
                        if(u8_MbootCnt == 1)        //one mboot ->powercut during upgrade->layout ubild
                        {
                            virtual_boot = u32_MbootBlkIdx[0] * (pNandDrv->u16_BlkPageCnt * pNandDrv->u16_PageByteCnt) / 1024;
                            PartSize = PartSize - (u32_MbootBlkIdx[0] - 10) * (pNandDrv->u16_BlkPageCnt * pNandDrv->u16_PageByteCnt) ;
                            sprintf(tmp, "%dk@%dk(%s)", PartSize/1024, virtual_boot, PartName);
                            #if defined(UBOOT_RECOVERY)
                            REG_WRITE_UINT16(UBOOT_RECOVERY, 0x1);
                            #endif
                            printf("Warning: Only One Mboot found, Please re-upgrade your mboot, in case of read ecc fail.\n");
                        }
                        else    //no mboot  using default for ubild partition impossible in normal case or tool programming case
                            goto cleanup;
                    }
                    else
                    {
                         if(u8_MbootCnt == 1)
                        {
                            virtual_boot = u32_MbootBlkIdx[0] * (pNandDrv->u16_BlkPageCnt * pNandDrv->u16_PageByteCnt) / 1024;
                            PartSize = PartSize - (u32_MbootBlkIdx[0] - 10) * (pNandDrv->u16_BlkPageCnt * pNandDrv->u16_PageByteCnt) ;
                            sprintf(tmp, "%dk@%dk(%s)", PartSize/1024, virtual_boot, PartName);
                        }
                        else if(u8_MbootCnt == 2)
                        {
                            virtual_boot = u32_MbootBlkIdx[0] * (pNandDrv->u16_BlkPageCnt * pNandDrv->u16_PageByteCnt) / 1024;
                            //PartSize = PartSize - (u32_MbootBlkIdx[0] - 10) * (pNandDrv->u16_BlkPageCnt * pNandDrv->u16_PageByteCnt) ;
                            PartSize1 = (u32_MbootBlkIdx[1] - u32_MbootBlkIdx[0]) * (pNandDrv->u16_BlkPageCnt * pNandDrv->u16_PageByteCnt) ;
                            PartSize2 = PartSize - (u32_MbootBlkIdx[1] - 10) * (pNandDrv->u16_BlkPageCnt * pNandDrv->u16_PageByteCnt) ;
                            sprintf(tmp, "%dk@%dk(%s),%dk(%sBAK)", PartSize1/1024, virtual_boot, PartName,PartSize2/1024, PartName);
                        }
                        else
                            goto cleanup;
                        //virtual_boot = (pNandDrv->u8_HashPBA[1][1] + u32_BlkIdx) * (pNandDrv->u16_BlkPageCnt * pNandDrv->u16_PageByteCnt) / 1024;
                        //PartSize = PartSize - (pNandDrv->u8_HashPBA[1][1] + u32_BlkIdx - 10) * (pNandDrv->u16_BlkPageCnt * pNandDrv->u16_PageByteCnt) ;
                    }
                }
                else // BFN case
                {
                    //search vid chunk header for mboot partition beginning
                    u32_BlkIdx = 1;
                    while(1)
                    {
                        u32_Err = NC_ReadPages((10 + u32_BlkIdx) << pNandDrv->u8_BlkPageCntBits, u8MainBuf, u8SpareBuf, 1);
                        //binary ID for chunk header
                        if( memcmp((const void *)u8MainBuf, "BIN0", 4) == 0 && u32_Err == UNFD_ST_SUCCESS)
                        {
                            u32_MbootBlkIdx[u8_MbootCnt] = (10 + u32_BlkIdx);
                            u8_MbootCnt ++;
                        }
                        if(u8_MbootCnt == 2)
                            break;

                        if((10 + u32_BlkIdx) == 0xFF)
                            break;

                        u32_BlkIdx ++;
                    }
                    if((10 + u32_BlkIdx) == 0xFF)
                    {
                        //only one mboot or upgrade power cut
                        //goto cleanup;
                        if(u8_MbootCnt == 1)        //one mboot ->powercut during upgrade->layout ubild
                        {
                            virtual_boot = u32_MbootBlkIdx[0] * (pNandDrv->u16_BlkPageCnt * pNandDrv->u16_PageByteCnt) / 1024;
                            PartSize = PartSize - (u32_MbootBlkIdx[0] - 10) * (pNandDrv->u16_BlkPageCnt * pNandDrv->u16_PageByteCnt) ;
                            sprintf(tmp, "%dk@%dk(%s)", PartSize/1024, virtual_boot, PartName);
                            #if defined(UBOOT_RECOVERY)
                            REG_WRITE_UINT16(UBOOT_RECOVERY, 0x1);
                            #endif
                            printf("Warning: Only One Mboot found, Please re-upgrade your mboot, in case of read ecc fail\n");
                        }
                        else    //no mboot  using default for ubild partition, impossible in normal case or tool programming case
                            goto cleanup;
                    }
                    else
                    {
                         if(u8_MbootCnt == 1)
                        {
                            virtual_boot = u32_MbootBlkIdx[0] * (pNandDrv->u16_BlkPageCnt * pNandDrv->u16_PageByteCnt) / 1024;
                            PartSize = PartSize - (u32_MbootBlkIdx[0] - 10) * (pNandDrv->u16_BlkPageCnt * pNandDrv->u16_PageByteCnt) ;
                            sprintf(tmp, "%dk@%dk(%s)", PartSize/1024, virtual_boot, PartName);
                        }
                        else if(u8_MbootCnt == 2)
                        {
                            virtual_boot = u32_MbootBlkIdx[0] * (pNandDrv->u16_BlkPageCnt * pNandDrv->u16_PageByteCnt) / 1024;
                            PartSize1 = (u32_MbootBlkIdx[1] - u32_MbootBlkIdx[0]) * (pNandDrv->u16_BlkPageCnt * pNandDrv->u16_PageByteCnt) ;
                            PartSize2 = PartSize - (u32_MbootBlkIdx[1] - 10) * (pNandDrv->u16_BlkPageCnt * pNandDrv->u16_PageByteCnt) ;
                            sprintf(tmp, "%dk@%dk(%s),%dk(%sBAK)", PartSize1/1024, virtual_boot, PartName,PartSize2/1024, PartName);
                        }
                        else
                            goto cleanup;

                        //virtual_boot = (10 + u32_BlkIdx) * (pNandDrv->u16_BlkPageCnt * pNandDrv->u16_PageByteCnt) / 1024;
                        //PartSize = PartSize - ((10 + u32_BlkIdx - 10) * (pNandDrv->u16_BlkPageCnt * pNandDrv->u16_PageByteCnt));
                    }
                }
            }
            else
            {
                sprintf(tmp, "%dk@%dk(%s)", PartSize/1024, pPartInfo->records[u8_i].u16_StartBlk*(pNandDrv->u16_BlkPageCnt * pNandDrv->u16_PageByteCnt) / 1024, PartName);
            }
        }
        len = strlen(tmp);
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

int nand_RefreshAllBlock(int refresh, int force)
{    
    NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();
    PARTITION_INFO_t *pPartInfo = pNandDrv->pPartInfo;    
    U8 *pu8_BlkPageBuf, *pu8_BlkSpareBuf;
    U16 u16_PBA, u16_Row, u16_MbootPartEndPBA = 0;
    U32 u32_Err = 0;
    int ret = 0;
    U8  u8_i;
    int s32_ECCsts;
    
    if(pNandDrv->u8_CellType == 0)
        return 0;

    pu8_BlkPageBuf = malloc(pNandDrv->u16_BlkPageCnt << pNandDrv->u8_PageByteCntBits);
    pu8_BlkSpareBuf = malloc(pNandDrv->u16_BlkPageCnt * pNandDrv->u16_SpareByteCnt);

    if(pu8_BlkPageBuf== NULL || pu8_BlkSpareBuf==NULL)
    {
        nand_debug(0, 1,"Allocate block buffer fails\n");
        free(pu8_BlkPageBuf);
        free(pu8_BlkSpareBuf);
        return 1;
    }

    //find end block of mboot partition
    for(u8_i = 0; u8_i < pPartInfo->u16_PartCnt; u8_i ++)
    {
        if(pPartInfo->records[u8_i].u16_PartType == UNFD_PART_UBOOT)
        {
            u16_MbootPartEndPBA = pPartInfo->records[u8_i].u16_StartBlk +
                                  pPartInfo->records[u8_i].u16_BlkCnt +
                                  pPartInfo->records[u8_i].u16_BackupBlkCnt;
            break;
        }
    }
    if(u8_i == pPartInfo->u16_PartCnt)
    {
        nand_debug(0,1,"no uboot in PNI\n");
        ret = 0;
        goto out;
    }
    
    for(u16_PBA = 0xA; u16_PBA < pNandDrv->u16_BlkCnt; u16_PBA ++)
    {
        if(drvNAND_IsGoodBlk(u16_PBA) == 0)
            continue;

//        nand_debug(0, 1, "Read Blk 0x%X\n", u16_PBA);
        //read all block first
        for(u16_Row = 0; u16_Row < pNandDrv->u16_BlkPageCnt; u16_Row ++)
        {
            u32_Err = drvNAND_LFSRReadPhyPage(u16_Row + (u16_PBA << pNandDrv->u8_BlkPageCntBits),
                                        pu8_BlkPageBuf + u16_Row * pNandDrv->u16_PageByteCnt,
                                        pu8_BlkSpareBuf + u16_Row * pNandDrv->u16_SpareByteCnt);
            if(u32_Err != UNFD_ST_SUCCESS)
            {
                nand_debug(0, 1,"Read blk 0x%X, page 0x%X Err\n", u16_PBA, u16_Row);
                ret = -1;
                goto out;
            }
            
            if(force == 0)
            {
                NC_CheckECC(&s32_ECCsts);
                if(s32_ECCsts)
                {
                    nand_debug(0, 1,"bit flip %d or no all 0xff page\n",s32_ECCsts);                
                    ret = -1;
                    goto out;
                }
            }
            
            if(drvNAND_CheckAll0xFF(pu8_BlkSpareBuf + u16_Row * pNandDrv->u16_SpareByteCnt, pNandDrv->u16_SpareByteCnt) && 
                u16_PBA >= u16_MbootPartEndPBA)
                break;
        }
    }

    //refreshing all block   
//    nand_debug(0, 1, "Begin Refresh all Blk \n");
    if(refresh)
    {
        for(u16_PBA = 0xA; u16_PBA < pNandDrv->u16_BlkCnt; u16_PBA ++)
        {
            if(drvNAND_IsGoodBlk(u16_PBA) == 0)
                continue;

    //        nand_debug(0, 1, "Read Blk 0x%X\n", u16_PBA);

            memset(pu8_BlkPageBuf , 0xff, (pNandDrv->u16_BlkPageCnt << pNandDrv->u8_PageByteCntBits));
            memset(pu8_BlkSpareBuf, 0xff, (pNandDrv->u16_BlkPageCnt * pNandDrv->u16_SpareByteCnt));
            for(u16_Row = 0; u16_Row < pNandDrv->u16_BlkPageCnt; u16_Row ++)
            {
                u32_Err = drvNAND_LFSRReadPhyPage(u16_Row + (u16_PBA << pNandDrv->u8_BlkPageCntBits),
                                            pu8_BlkPageBuf + u16_Row * pNandDrv->u16_PageByteCnt,
                                            pu8_BlkSpareBuf + u16_Row * pNandDrv->u16_SpareByteCnt);
                if(u32_Err != UNFD_ST_SUCCESS)
                {
                    nand_debug(0, 1,"Read blk 0x%X, page 0x%X Err\n", u16_PBA, u16_Row);
                    ret = -1;                
                    goto out;
                }
                if(drvNAND_CheckAll0xFF(pu8_BlkSpareBuf + u16_Row * pNandDrv->u16_SpareByteCnt, pNandDrv->u16_SpareByteCnt) && 
                    u16_PBA >= u16_MbootPartEndPBA)
                    break;            
            }
    //        nand_debug(0, 1, "Erase Blk 0x%X\n", u16_PBA);
            u32_Err = NC_EraseBlk(u16_PBA << pNandDrv->u8_BlkPageCntBits);
            if(u32_Err != UNFD_ST_SUCCESS)
            {
                nand_debug(0, 1, "Erase blk 0x%X Err\n", u16_PBA);
                ret = -1;
                goto out;
            }
    //        nand_debug(0, 1, "Write Blk 0x%X\n", u16_PBA);
            for(u16_Row = 0; u16_Row < pNandDrv->u16_BlkPageCnt; u16_Row ++)
            {
                if(drvNAND_CheckAll0xFF(pu8_BlkSpareBuf + u16_Row * pNandDrv->u16_SpareByteCnt, pNandDrv->u16_SpareByteCnt))
                    continue;
                
                u32_Err = drvNAND_LFSRWritePhyPage(u16_Row + (u16_PBA << pNandDrv->u8_BlkPageCntBits),
                                            pu8_BlkPageBuf + u16_Row * pNandDrv->u16_PageByteCnt,
                                            pu8_BlkSpareBuf + u16_Row * pNandDrv->u16_SpareByteCnt);
                if(u32_Err != UNFD_ST_SUCCESS)
                {
                    nand_debug(0, 1,"Write blk 0x%X, page 0x%X Err\n", u16_PBA, u16_Row);
                    ret = -1;                
                    goto out;
                }            
            }
        }
    }

out:
    free(pu8_BlkPageBuf);
    free(pu8_BlkSpareBuf);
    return ret;
}

void nand_SyncMtdPartFor1stBoot(void)
{
    const char *parts;
    char mtd_buf[512];
    char buffer[1024];
    char *pch;
    NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
    PARTITION_INFO_t *pPartInfo = pNandDrv->pPartInfo;
    char PartName[16]="UNKNOWN";
    U8 u8_i;
    U8 u8_Unknown = 0;

    // partition name should sync with drvNAND_GetMtdParts
    for(u8_i = 0; u8_i < pPartInfo->u16_PartCnt; u8_i ++)
    {
        if( (pPartInfo->records[u8_i].u16_PartType & UNFD_LOGI_PART) == UNFD_LOGI_PART)
            break;

        switch(pPartInfo->records[u8_i].u16_PartType)
        {
            case UNFD_PART_BOOTLOGO:
                sprintf(PartName,"UBILD" );
                break;
            case UNFD_PART_UBOOT:
                sprintf(PartName,"MBOOT" );
                break;
            case UNFD_PART_OPTEE:
                sprintf(PartName,"OPTEE" );
                break;
            case UNFD_PART_ARMFW:
                sprintf(PartName,"ARMFW" );
                break;
            case UNFD_PART_TEEKB:
                sprintf(PartName,"TEEKB" );
                break;
             default:
                sprintf(PartName,"CUS%d", u8_Unknown++);
                break;
        }
    }

    parts = getenv("mtdparts");
    if(parts == NULL)
        return;

    pch = strstr(parts, PartName);

    if(pch != NULL)
    {
        pch += (strlen(PartName)+1); // ABC)

        if(strlen(pch) != 0)
        {
            pch += 1;       //skip ","
            sprintf(mtd_buf, "edb64M-nand:%s", pch);
            sprintf(buffer,"dynpart %s", mtd_buf);
            run_command(buffer,0);
        }
    }
}


U32 drvNAND_RefreshCIS(void)
{
    NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();
    NAND_FLASH_INFO_t * pNandInfo = (NAND_FLASH_INFO_t* )malloc(512);
    PARTITION_INFO_t *pPartInfo = pNandDrv->pPartInfo;
    U32 u32_BlkIdx, u32_MBootBegin = 0, u32_MBootEnd = 0;
    U32 u32_Err;
    U8  u8_i;
    int bl_count = 0;

    if(!pNandInfo)
    {
        printf("Memory Allocate fail for pNandInfo\n");
        return -1;
    }


    //setup pNandInfo for CIS
    memset(pNandInfo, 0, 512);
    memcpy(pNandInfo->au8_Tag, "MSTARSEMIUNFDCIS", 16);
    pNandInfo->u8_IDByteCnt = pNandDrv->u8_IDByteCnt;
    memset(pNandInfo->au8_ID, 0, NAND_ID_BYTE_CNT);
    memcpy(pNandInfo->au8_ID, pNandDrv->au8_ID, pNandDrv->u8_IDByteCnt);
    pNandInfo->u16_SpareByteCnt = pNandDrv->u16_SpareByteCnt;
    pNandInfo->u16_PageByteCnt = pNandDrv->u16_PageByteCnt;
    pNandInfo->u16_BlkPageCnt = pNandDrv->u16_BlkPageCnt;
    if(pNandDrv->u8_SlcMode == 1)
        pNandInfo->u16_BlkPageCnt <<= 1;
    pNandInfo->u16_BlkCnt = pNandDrv->u16_BlkCnt;
    pNandInfo->u32_Config = pNandDrv->u32_Config;

    pNandInfo->u16_ECCType = pNandDrv->u16_ECCType;
    pNandInfo->u16_tRC          = pNandDrv->u16_tRC;
    pNandInfo->u8_tRP           = pNandDrv->u8_tRP;
    pNandInfo->u8_tREH          = pNandDrv->u8_tREH;
    pNandInfo->u8_tREA          = pNandDrv->u8_tREA;
    pNandInfo->u8_tRR           = pNandDrv->u8_tRR;
    pNandInfo->u16_tADL         = pNandDrv->u16_tADL;
    pNandInfo->u16_tRHW         = pNandDrv->u16_tRHW;
    pNandInfo->u16_tWHR         = pNandDrv->u16_tWHR;
    pNandInfo->u16_tCCS         = pNandDrv->u16_tCCS;
    pNandInfo->u8_tCS           = pNandDrv->u8_tCS;
    pNandInfo->u16_tWC          = pNandDrv->u16_tWC;
    pNandInfo->u8_tWP           = pNandDrv->u8_tWP;
    pNandInfo->u8_tWH           = pNandDrv->u8_tWH;
    pNandInfo->u16_tCWAW        = pNandDrv->u16_tCWAW;
    pNandInfo->u8_tCLHZ         = pNandDrv->u8_tCLHZ;
    pNandInfo->u8_AddrCycleIdx  = pNandDrv->u8_AddrCycleIdx;
    pNandInfo->u16_tWW          = pNandDrv->u16_tWW;

    pNandInfo->u8_PairPageMapLoc = pNandDrv->u8_PairPageMapLoc;
    pNandInfo->u8_ReadRetryType =   pNandDrv->u8_ReadRetryType;
    pNandInfo->u8_BitflipThreshold = pNandDrv->u16_BitflipThreshold;

    pNandInfo->u32_ChkSum       = drvNAND_CheckSum((U8*) (pNandInfo) + 0x24, 0x32 - 0x24);

    memcpy(pNandInfo->u8_Vendor, pNandDrv->u8_Vendor, 16);
    memcpy(pNandInfo->u8_PartNumber, pNandDrv->u8_PartNumber, 16);

    pNandInfo->u8_Hash0PageIdx = pNandDrv->u8_Hash0PageIdx;
    pNandInfo->u8_Hash1PageIdx = pNandDrv->u8_Hash1PageIdx;
    pNandInfo->u32_BootSize = pNandDrv->u32_BootSize;

    pNandInfo->u16_BBtPageCheckCount = pNandDrv->u16_BBtPageCheckCount;
    memcpy(pNandInfo->u16_BBtPageIdx, pNandDrv->u16_BBtPageIdx, NAND_BBT_PAGE_COUNT*sizeof(U16));
    memcpy(pNandInfo->u16_BBtMarker, pNandDrv->u16_BBtMarker, NAND_BBT_PAGE_COUNT*sizeof(U16));

    pNandInfo->u16_PhySpareByteCnt = pNandDrv->u16_PhySpareByteCnt;

    //search MBOOT partition in partinfo

    if(pNandDrv->u8_HasPNI == 1)
    {
        for(u8_i = 0; u8_i < pPartInfo->u16_PartCnt; u8_i ++)
        {
            if(pPartInfo->records[u8_i].u16_PartType == UNFD_PART_UBOOT)
            {
                u32_MBootBegin = pPartInfo->records[u8_i].u16_StartBlk;
                u32_MBootEnd = pPartInfo->records[u8_i].u16_StartBlk + pPartInfo->records[u8_i].u16_BlkCnt + pPartInfo->records[u8_i].u16_BackupBlkCnt;
                break;
            }
        }
        if(u8_i == pPartInfo->u16_PartCnt)
        {
            printf("ERROR: Partition info does not contain MBOOT partition\n");
            free(pNandInfo);            
            return -1;
        }

        //search sboot uboot/ HashX location for update nni infomation

        if(pNandDrv->u8_BL0PBA != 0)    //for bl uboot
        {
            bl_count = 0;
            //search bl location in MBOOT PARTITION
            for(u32_BlkIdx = u32_MBootBegin; u32_BlkIdx < u32_MBootEnd; u32_BlkIdx ++)
            {
                u32_Err = NC_ReadPages(u32_BlkIdx << pNandDrv->u8_BlkPageCntBits, u8MainBuf, u8SpareBuf, 1);
                if(u32_Err != UNFD_ST_SUCCESS)
                    continue;
                if(!pNandDrv->u16_BBtPageCheckCount || pNandDrv->u16_BBtPageCheckCount > NAND_BBT_PAGE_COUNT){
                    if(u8SpareBuf[0] != 0xFF)
                        continue;
                }else{
                    if(!drvNAND_CheckSpareSpacePattern(u8SpareBuf, u32_BlkIdx << pNandDrv->u8_BlkPageCntBits))
                        continue;
                }
                if(!drvNAND_CheckAll0xFF(u8MainBuf, pNandDrv->u16_PageByteCnt))
                {
                    if(bl_count == 0)
                    {
                        pNandInfo->u8_BL0PBA = pNandDrv->u8_BL0PBA = (U8)u32_BlkIdx;
                        bl_count ++;
                        
                        #if (ENABLE_MODULE_DISABLE_MBOOTBAK == 1)
                        pNandInfo->u8_BL1PBA = pNandDrv->u8_BL1PBA = (U8)u32_BlkIdx;
                        bl_count ++;
                        break;
                        #endif
                    }
                    else if(bl_count == 1)
                    {
                        pNandInfo->u8_BL1PBA = pNandDrv->u8_BL1PBA = (U8)u32_BlkIdx;
                        bl_count ++;
                        break;
                    }
                    bl_count ++;
                }
            }

            printf("BL0_PBA %X, BL1_PBA %X\n", pNandDrv->u8_BL0PBA, pNandDrv->u8_BL1PBA);
            if(bl_count != 2)
            {
                printf("WARNING: there is no two sboots in NAND Flash, Please Reupgrade Sboot\n");
                free(pNandInfo);                
                return -1;
            }

            if(pNandDrv->u8_UBOOTPBA != 0)
            {
                bl_count = 0;
                for(u32_BlkIdx = u32_MBootBegin; u32_BlkIdx < u32_MBootEnd; u32_BlkIdx ++)
                {
                    u32_Err = NC_ReadPages(u32_BlkIdx << pNandDrv->u8_BlkPageCntBits, u8MainBuf, u8SpareBuf, 1);
                    if(u32_Err != UNFD_ST_SUCCESS)
                        continue;
                    if(!pNandDrv->u16_BBtPageCheckCount || pNandDrv->u16_BBtPageCheckCount > NAND_BBT_PAGE_COUNT){
                        if(u8SpareBuf[0] != 0xFF)
                            continue;
                    }else{
                        if(!drvNAND_CheckSpareSpacePattern(u8SpareBuf, u32_BlkIdx << pNandDrv->u8_BlkPageCntBits))
                            continue;
                    }
                    if(((U32 *)u8MainBuf)[0x7] == 0x0000B007)
                    {
                        if(bl_count == 1)
                        {
                            pNandInfo->u8_UBOOTPBA = pNandDrv->u8_UBOOTPBA = (U8)u32_BlkIdx;
                            bl_count ++;
                            break;
                        }
                        bl_count ++;
                    }
                }
                if(bl_count != 2)
                {
                    printf("WARNING: there is no two Mboots in NAND Flash, Please Reupgrade Mboot\n");
                    free(pNandInfo);                    
                    return -1;
                }

                printf("UBOOTPBA %X\n", pNandDrv->u8_UBOOTPBA);
            }
        }
        else if(pNandDrv->u8_HashPBA[0][0] != 0)    //for hash
        {
            bl_count = 0;
            //search bl location in MBOOT PARTITION
            for(u32_BlkIdx = u32_MBootBegin; u32_BlkIdx < u32_MBootEnd; u32_BlkIdx ++)
            {
                u32_Err = NC_ReadPages(u32_BlkIdx << pNandDrv->u8_BlkPageCntBits, u8MainBuf, u8SpareBuf, 1);
                if(u32_Err != UNFD_ST_SUCCESS)
                    continue;
                if(!pNandDrv->u16_BBtPageCheckCount || pNandDrv->u16_BBtPageCheckCount > NAND_BBT_PAGE_COUNT){
                    if(u8SpareBuf[0] != 0xFF)
                        continue;
                }else{
                    if(!drvNAND_CheckSpareSpacePattern(u8SpareBuf, u32_BlkIdx << pNandDrv->u8_BlkPageCntBits))
                        continue;
                }

                if(!drvNAND_CheckAll0xFF(u8MainBuf, pNandDrv->u16_PageByteCnt))
                {
                    pNandInfo->u8_HashPBA[bl_count>>1][bl_count&1] = pNandDrv->u8_HashPBA[bl_count>>1][bl_count&1] = (U8)u32_BlkIdx;
                    if(++bl_count == 4)
                        break;
                }
            }

            printf("HASH00_PBA %X, HASH01_PBA %X\n", pNandInfo->u8_HashPBA[0][0], pNandInfo->u8_HashPBA[0][1]);
            printf("HASH10_PBA %X, HASH11_PBA %X\n", pNandInfo->u8_HashPBA[1][0], pNandInfo->u8_HashPBA[1][1]);
            //printf("HASH00_PBA %X, HASH01_PBA %X\n", pNandInfo->u8_HashPBA[0][0], pNandInfo->u8_HashPBA[0][1]);
            if(bl_count != 4)
            {
                printf("WARNING: there is no two sboots in NAND Flash, Please Reupgrade Sboot\n");
                free(pNandInfo);                
                return -1;
            }

            bl_count = 0;
            for(u32_BlkIdx = pNandDrv->u8_HashPBA[1][1]+1; u32_BlkIdx < u32_MBootEnd; u32_BlkIdx ++)
            {
                u32_Err = NC_ReadPages(u32_BlkIdx << pNandDrv->u8_BlkPageCntBits, u8MainBuf, u8SpareBuf, 1);
                if(u32_Err != UNFD_ST_SUCCESS)
                    continue;
                if(!pNandDrv->u16_BBtPageCheckCount || pNandDrv->u16_BBtPageCheckCount > NAND_BBT_PAGE_COUNT){
                    if(u8SpareBuf[0] != 0xFF)
                        continue;
                }else{
                    if(!drvNAND_CheckSpareSpacePattern(u8SpareBuf, u32_BlkIdx << pNandDrv->u8_BlkPageCntBits))
                        continue;
                }
                if(((U32 *)u8MainBuf)[0x7] == 0x0000B007)
                {
                    pNandInfo->u8_HashPBA[2][bl_count] = pNandDrv->u8_HashPBA[2][bl_count] = (U8)u32_BlkIdx;
                    if(++bl_count == 2)
                        break;
                }
            }
            if(bl_count != 2)
            {
                printf("WARNING: there is no two Mboots in NAND Flash, Please Reupgrade Mboot\n");                
                free(pNandInfo);
                return -1;
            }

            printf("HASH20_PBA %X, HASH21_PBA %X\n", pNandInfo->u8_HashPBA[2][0], pNandInfo->u8_HashPBA[2][1]);
        }
    }

    NC_PlatformInit();

    u32_Err = NC_ResetNandFlash();
    if(u32_Err != UNFD_ST_SUCCESS)
    {
        nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1,"NAND Reset Fail\n");
        free(pNandInfo);        
        return -1;
    }
    #if defined(FCIE4_DDR) && FCIE4_DDR
    memset((void *) &pNandDrv->tDefaultDDR, 0, sizeof(DDR_TIMING_GROUP_t));
    memset((void *) &pNandDrv->tMaxDDR, 0, sizeof(DDR_TIMING_GROUP_t));
    memset((void *) &pNandDrv->tMinDDR, 0, sizeof(DDR_TIMING_GROUP_t));
    #endif
    NC_ConfigNandFlashContext();
    /*Re-Search DDR timing again*/
    u32_Err = NC_Init();
    if(u32_Err != UNFD_ST_SUCCESS)
    {
        nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1,"NC_Init Fail ErrCode: %X\n", u32_Err);
        free(pNandInfo);        
        return -1;
    }
    NC_Config();

    nand_config_clock(pNandInfo->u16_tRC);

    u32_Err = drvNAND_WriteCIS_for_ROM(pNandInfo);

    nand_SyncMtdPartFor1stBoot();

    free(pNandInfo);
    return u32_Err;
}

#if ENABLE_MODULE_BFN_WRITE_NAND
#define CHIPCFG_CMD_BASE       0xFE00
#define CHIPCFG_CMD_END        0xFFF0
#define CHIPCFG_CMD_WAIT       0xFF00
#define CHIPCFG_CMD_DBG        0xFFE0

unsigned char *nand_buffer;
static unsigned char nand_buffer2[16384] __attribute((aligned(64)));
static unsigned char nand_buffer_spare[16384] __attribute((aligned(64)));

#define MIUIDTAG    (0x4D495530)      // 'MIU0'
#define LDRIDTAG    (0x4C445230)      // 'LDR0'
#define BINIDTAG    (0x42494E30)      // 'BIN0'

static U32 BFN_WritePage_MIU(U32 u32Row, U8 *u8Buf, U8 *u8Spare, U16 u16Repeat, U16 autodis)
{
    NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();
    U32 u32_Err = UNFD_ST_SUCCESS;


    if(autodis)
    {
        pNandDrv->u16_Reg48_Spare |= BIT_NC_HW_AUTO_RANDOM_CMD_DISABLE;
        NC_Config();
    }

    u32_Err = NC_WritePages( u32Row, u8Buf, u8Spare, u16Repeat);
    if( u32_Err != UNFD_ST_SUCCESS)
    {
        printf("NC_WritePages error\n");
    }

    if(autodis)
    {
        pNandDrv->u16_Reg48_Spare &= ~BIT_NC_HW_AUTO_RANDOM_CMD_DISABLE;
        NC_Config();
    }

    return u32_Err;
}

static U32 BFN_ReadPage_MIU(U32 u32Row, U8 *u8Buf, U8 *u8Spare, U16 u16Repeat, U16 autodis)
{
    NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();
    U32 u32_Err = UNFD_ST_SUCCESS;

    if(autodis)
    {
        pNandDrv->u16_Reg48_Spare |= BIT_NC_HW_AUTO_RANDOM_CMD_DISABLE;
        NC_Config();
    }

    u32_Err = NC_ReadPages( u32Row, u8Buf, u8Spare, u16Repeat);
    if( u32_Err != UNFD_ST_SUCCESS)
    {
        printf("NC_ReadPages error\n");
    }

    if(autodis)
    {
        pNandDrv->u16_Reg48_Spare &= ~BIT_NC_HW_AUTO_RANDOM_CMD_DISABLE;
        NC_Config();
    }

    return u32_Err;
}

static U32 BFN_WriteSector(U32 u32PageIndex, U8 *u8Buf, U8 *u8Spare, U16 autodis)
{
    NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();
    U32 u32_Err = UNFD_ST_SUCCESS;


    if(autodis)
    {
        pNandDrv->u16_Reg48_Spare |= BIT_NC_HW_AUTO_RANDOM_CMD_DISABLE;
        NC_Config();
    }

    u32_Err = NC_WriteSectors( u32PageIndex, 0, u8Buf, u8Spare, 1);
    if( u32_Err != UNFD_ST_SUCCESS)
    {
        printf("NC_WriteSectors error\n");
    }

    NC_Write_RandomOut(u32PageIndex,0,u8Buf,16);

    if(autodis)
    {
        pNandDrv->u16_Reg48_Spare &= ~BIT_NC_HW_AUTO_RANDOM_CMD_DISABLE;
        NC_Config();
    }

    return u32_Err;
}

static U32 BFN_ReadSector_RIU(U32 u32Row, U32 u32Col, U8 *u8Buf, U8 *u8SprBuf ,U16 autodis)
{
    NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();
    U32 u32_Err = UNFD_ST_SUCCESS;

    if(autodis)
    {
        pNandDrv->u16_Reg48_Spare |= BIT_NC_HW_AUTO_RANDOM_CMD_DISABLE;
        NC_Config();
    }

    u32_Err = NC_ReadSector_RIUMode(u32Row, u32Col, u8Buf, u8SprBuf);

    if(autodis)
    {
        pNandDrv->u16_Reg48_Spare &= ~BIT_NC_HW_AUTO_RANDOM_CMD_DISABLE;
        NC_Config();
    }

    return u32_Err;
}

int drvNAND_ReadMIUParam(U32 u32MIUPos)
{
    /*
    int              retlen=0;
    while (1)
    {
        U16 reg = *(U16*)u32MIUPos;

        retlen += 4;

        if (reg == CHIPCFG_CMD_END)
            return retlen;

        u32MIUPos += 4;
    }
    return 0;
    */
     return 0x2000;
}


U32 drvNAND_WriteMIUParam(int MIUBLOCK, U32 MIUPOS)
{
    NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();

    int ParamMIUSize;
    int ProgramPage=0;
    U32 u32_Err = UNFD_ST_SUCCESS;

    u32_Err = NC_EraseBlk( MIUBLOCK * pNandDrv->u16_BlkPageCnt );//use first block as test item
    if (u32_Err != UNFD_ST_SUCCESS)
    {
        printf("MIU write blk error!!\n");
        drvNAND_MarkBadBlk(MIUBLOCK);
        return 0;
    }

    //==============write MIU position==============
    // read MIU parameter

    ParamMIUSize = drvNAND_ReadMIUParam(MIUPOS);

    *((U32*)(nand_buffer2+0x00)) = MIUIDTAG;//0x4D495530
    *((U32*)(nand_buffer2+0x04)) = pNandDrv->u16_PageByteCnt;
    *((U32*)(nand_buffer2+0x10)) = ParamMIUSize>>9; // number of MIU page (in 512 bytes size)
    *((U32*)(nand_buffer2+0x14)) = pNandDrv->u16_BlkPageCnt;

    memset(nand_buffer_spare, -1, sizeof(nand_buffer_spare));

    printf("MIU parameter size %d\n",ParamMIUSize);
    BFN_WritePage_MIU(MIUBLOCK * pNandDrv->u16_BlkPageCnt + ProgramPage, (U8*)nand_buffer2,(U8*)nand_buffer_spare,1,1);
    ProgramPage++;
    printf("Write First Page, ParamMIUSize left %d program page number %d\n",ParamMIUSize,ProgramPage);

    while(ParamMIUSize > 0)
    {
        BFN_WritePage_MIU(MIUBLOCK * pNandDrv->u16_BlkPageCnt+ProgramPage,(U8*)nand_buffer+((ProgramPage-1)*(512)), (U8*)nand_buffer_spare,1,1);
        ParamMIUSize -= 512;
        ProgramPage ++;
        printf("Write next Page, ParamMIUSize left %d program page number %d\n",ParamMIUSize,ProgramPage);
    }

    return 1;
}

U32 drvNAND_WriteCISMIUParam(int MIUBLOCK, U32 MIUPOS, NAND_FLASH_INFO_t * pNandInfo)
{
    NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();
    PARTITION_INFO_t *pPartInfo = drvNAND_get_DrvContext_PartInfo();
    BLK_INFO_t *pBlkInfo = (BLK_INFO_t*)nand_buffer_spare;

    int ParamMIUSize;
    int ProgramPage=0;
    U32 u32_Err = UNFD_ST_SUCCESS;
    U32 u32_BlkRow;
    U16 u16_CISIdx;

    /* Check first page of block */
    u32_BlkRow = MIUBLOCK << pNandDrv->u8_BlkPageCntBits;
    u32_Err = BFN_ReadPage_MIU(u32_BlkRow,nand_buffer2,nand_buffer_spare,1,1);
    if (u32_Err != UNFD_ST_SUCCESS)
    {
        printf("MIU write blk error!!%s %x\n",__FUNCTION__,__LINE__);
    }
    if (nand_buffer_spare[0] != 0xFF)
    {
        printf("MIU write blk error!!%s %x\n",__FUNCTION__,__LINE__);
        return 0;
    }

    u32_Err = NC_EraseBlk( MIUBLOCK * pNandDrv->u16_BlkPageCnt );//use first block as test item
    if (u32_Err != UNFD_ST_SUCCESS)
    {
        printf("MIU write blk error!!%s %x\n",__FUNCTION__,__LINE__);
        drvNAND_MarkBadBlk(MIUBLOCK);
        return 0;
    }

    //==============write MIU position==============
    // read MIU parameter

    ParamMIUSize = drvNAND_ReadMIUParam(MIUPOS);

    /* original drvNAND_WriteCIS_for_ROM*/
    NC_ConfigNandFlashContext();
    NC_RegInit();
    NC_Config();

    nand_config_clock(pNandInfo->u16_tRC);

    #if defined(FCIE4_DDR) && FCIE4_DDR
    memcpy((void *) &pNandInfo->tDefaultDDR, (const void *) &pNandDrv->tDefaultDDR, sizeof(DDR_TIMING_GROUP_t));
    memcpy((void *) &pNandInfo->tMaxDDR, (const void *) &pNandDrv->tMaxDDR, sizeof(DDR_TIMING_GROUP_t));
    memcpy((void *) &pNandInfo->tMinDDR, (const void *) &pNandDrv->tMinDDR, sizeof(DDR_TIMING_GROUP_t));
    #endif

    u16_CISIdx = REG(RIU_BASE + (0x1980 << 2) + (0x41 << 2) );
    switch (u16_CISIdx) {
        case 2:
            printf("for first cis index\n");
            u16_CISIdx = 0;
            break;
        case 3:
            printf("for second cis index");
            u16_CISIdx = 1;
            break;
        default:
            printf("for first cis index");
            REG(RIU_BASE + (0x1980 << 2) + (0x41 << 2) ) = 0;
            u16_CISIdx = 0;
            break;
    }

    /* Reeset NAND driver and FCIE to the original settings */
    pNandDrv->u16_SpareByteCnt = pNandInfo->u16_SpareByteCnt;
    pNandDrv->u16_PageByteCnt  = pNandInfo->u16_PageByteCnt;
    pNandDrv->u16_ECCType      = pNandInfo->u16_ECCType;
    NC_ConfigNandFlashContext();
    NC_RegInit();
    pNandDrv->u16_Reg48_Spare &= ~(1 << 12);
    NC_Config();

    pNandDrv->u16_PageByteCnt = 2048;
    pNandDrv->u16_SpareByteCnt = 256;
    pNandDrv->u16_ECCType = NANDINFO_ECC_TYPE;
    #if defined(ECC_TYPE_72BIT1KB) && ECC_TYPE_72BIT1KB
    if(pNandDrv->u16_ECCType == ECC_TYPE_72BIT1KB)
        pNandDrv->u16_SpareByteCnt = 0x180;
    #endif

    NC_ConfigNandFlashContext();
    NC_RegInit();

    // copy CIS to the buffer offset 0x20
    memset(nand_buffer2, '\0', 512);
    memcpy(nand_buffer2,pNandInfo,512);
    memset(nand_buffer_spare, -1, sizeof(nand_buffer_spare));

    pBlkInfo->u8_BadBlkMark = 0xFF;
    pBlkInfo->u8_PartType = 0;

    u32_Err = BFN_WriteSector(MIUBLOCK * pNandDrv->u16_BlkPageCnt + ProgramPage, (U8*)nand_buffer2,(U8*)nand_buffer_spare,1);

    if (u32_Err != UNFD_ST_SUCCESS)
    {
        printf("MIU write blk error!!%s %x\n",__FUNCTION__,__LINE__);
        drvNAND_MarkBadBlk(MIUBLOCK);
        return 0;
    }

    /*
        **  Write Partition Info the 2nd page
        **/
    if(pNandDrv->u8_HasPNI == 1)
    {
        memset(nand_buffer2, '\0', pNandDrv->u16_PageByteCnt);
        memcpy(nand_buffer2, pPartInfo, 512);

        u32_Err = BFN_WriteSector(MIUBLOCK * pNandDrv->u16_BlkPageCnt + ga_tPairedPageMap[1].u16_LSB, (U8*)nand_buffer2,(U8*)nand_buffer_spare,0);
        if (u32_Err != UNFD_ST_SUCCESS)
        {
            printf("MIU write blk error!!%s %x\n",__FUNCTION__,__LINE__);
            drvNAND_MarkBadBlk(MIUBLOCK);
            return 0;
        }
    }

    /*
    **  Write Paired Page Map to the 4th page
    **/
    if(pNandDrv->u8_CellType == NAND_CellType_MLC)  // MLC
    {
        memset(nand_buffer2, '\0', pNandDrv->u16_PageByteCnt);
        memcpy(nand_buffer2, &ga_tPairedPageMap, 2048);

        u32_Err = BFN_WriteSector(MIUBLOCK * pNandDrv->u16_BlkPageCnt + pNandInfo->u8_PairPageMapLoc, (U8*)nand_buffer2,(U8*)nand_buffer_spare,1);
        if (u32_Err != UNFD_ST_SUCCESS) {
            printf("MIU write blk error!!%s %x\n",__FUNCTION__,__LINE__);
            drvNAND_MarkBadBlk(MIUBLOCK);
            return 0;
        }
    }
    u16_CISIdx++;

    printf("CIS is written to blk 0x%04x\n", MIUBLOCK);

    // chk cis index
    switch (u16_CISIdx) {
        case 0:
            REG(RIU_BASE + (0x1980 << 2) + (0x41 << 2) )  = 0x0002;
            u32_Err = UNFD_ST_ERR_NO_BLK_FOR_CIS0;
            break;
        case 1:
            REG(RIU_BASE + (0x1980 << 2) + (0x41 << 2) )  = 0x0003;
            u32_Err = UNFD_ST_ERR_NO_BLK_FOR_CIS1;
            break;
        case 2:
            u32_Err = UNFD_ST_SUCCESS;
            break;
    }

    printf("MIU parameter size %d, sector byte count %d\n",ParamMIUSize, pNandDrv->u16_SectorByteCnt);

    ProgramPage = 10;


    while(ParamMIUSize > 0)
    {
        printf("Write Page, ParamMIUSize left %d program page number %d\n",ParamMIUSize,ProgramPage);
        u32_Err = BFN_WriteSector(MIUBLOCK * pNandDrv->u16_BlkPageCnt+ProgramPage,(U8*)nand_buffer+((ProgramPage-10)*(pNandDrv->u16_SectorByteCnt)), (U8*)nand_buffer_spare,1);
        if (u32_Err != UNFD_ST_SUCCESS)
        {
            printf("MIU write blk error!!%s %x\n",__FUNCTION__,__LINE__);
        }

        ParamMIUSize -= pNandDrv->u16_SectorByteCnt;
        ProgramPage ++;
    }
    printf("totally write %x\n",ProgramPage-10);

    /* Reset NAND driver and FCIE to the original settings */
    pNandDrv->u16_SpareByteCnt = pNandInfo->u16_SpareByteCnt;
    pNandDrv->u16_PageByteCnt  = pNandInfo->u16_PageByteCnt;
    pNandDrv->u16_ECCType      = pNandInfo->u16_ECCType;
    NC_ConfigNandFlashContext();
    NC_RegInit();
    pNandDrv->u16_Reg48_Spare &= ~(1 << 12);
    NC_Config();

    return 1;
}

U32 drvNAND_RefreshCISMIUParam(int MIUBLOCK, U32 MIUPOS)
{
    NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();
    NAND_FLASH_INFO_t * pNandInfo = (NAND_FLASH_INFO_t* )malloc(512);
    PARTITION_INFO_t *pPartInfo = pNandDrv->pPartInfo;
    U32 u32_BlkIdx, u32_MBootBegin = 0, u32_MBootEnd = 0;
    U32 u32_Err;
    U8  u8_i;
    int bl_count = 0;

    if(!pNandInfo)
    {
        printf("Memory Allocate fail for pNandInfo\n");
        return -1;
    }

    //setup pNandInfo for CIS
    memset(pNandInfo, 0, 512);
    memcpy(pNandInfo->au8_Tag, "MSTARSEMIUNFDCIS", 16);
    pNandInfo->u8_IDByteCnt = pNandDrv->u8_IDByteCnt;
    memset(pNandInfo->au8_ID, 0, NAND_ID_BYTE_CNT);
    memcpy(pNandInfo->au8_ID, pNandDrv->au8_ID, pNandDrv->u8_IDByteCnt);
    pNandInfo->u16_SpareByteCnt = pNandDrv->u16_SpareByteCnt;
    pNandInfo->u16_PageByteCnt = pNandDrv->u16_PageByteCnt;
    pNandInfo->u16_BlkPageCnt = pNandDrv->u16_BlkPageCnt;
    pNandInfo->u16_BlkCnt = pNandDrv->u16_BlkCnt;
    pNandInfo->u32_Config = pNandDrv->u32_Config;

    pNandInfo->u16_ECCType = pNandDrv->u16_ECCType;
    pNandInfo->u16_tRC          = pNandDrv->u16_tRC;
    pNandInfo->u8_tRP           = pNandDrv->u8_tRP;
    pNandInfo->u8_tREH          = pNandDrv->u8_tREH;
    pNandInfo->u8_tREA          = pNandDrv->u8_tREA;
    pNandInfo->u8_tRR           = pNandDrv->u8_tRR;
    pNandInfo->u16_tADL         = pNandDrv->u16_tADL;
    pNandInfo->u16_tRHW         = pNandDrv->u16_tRHW;
    pNandInfo->u16_tWHR         = pNandDrv->u16_tWHR;
    pNandInfo->u16_tCCS         = pNandDrv->u16_tCCS;
    pNandInfo->u8_tCS           = pNandDrv->u8_tCS;
    pNandInfo->u16_tWC          = pNandDrv->u16_tWC;
    pNandInfo->u8_tWP           = pNandDrv->u8_tWP;
    pNandInfo->u8_tWH           = pNandDrv->u8_tWH;
    pNandInfo->u16_tCWAW        = pNandDrv->u16_tCWAW;
    pNandInfo->u8_tCLHZ         = pNandDrv->u8_tCLHZ;
    pNandInfo->u8_AddrCycleIdx  = pNandDrv->u8_AddrCycleIdx;
    pNandInfo->u16_tWW          = pNandDrv->u16_tWW;

    pNandInfo->u8_PairPageMapLoc = pNandDrv->u8_PairPageMapLoc;
    pNandInfo->u8_ReadRetryType =   pNandDrv->u8_ReadRetryType;
    pNandInfo->u8_BitflipThreshold = pNandDrv->u16_BitflipThreshold;

    pNandInfo->u32_ChkSum       = drvNAND_CheckSum((U8*) (pNandInfo) + 0x24, 0x32 - 0x24);

    memcpy(pNandInfo->u8_Vendor, pNandDrv->u8_Vendor, 16);
    memcpy(pNandInfo->u8_PartNumber, pNandDrv->u8_PartNumber, 16);

    pNandInfo->u8_Hash0PageIdx = pNandDrv->u8_Hash0PageIdx;
    pNandInfo->u8_Hash1PageIdx = pNandDrv->u8_Hash1PageIdx;
    pNandInfo->u32_BootSize = pNandDrv->u32_BootSize;

    //search MBOOT partition in partinfo

    if(pNandDrv->u8_HasPNI == 1)
    {
        for(u8_i = 0; u8_i < pPartInfo->u16_PartCnt; u8_i ++)
        {
            if(pPartInfo->records[u8_i].u16_PartType == UNFD_PART_UBOOT)
            {
                u32_MBootBegin = pPartInfo->records[u8_i].u16_StartBlk;
                u32_MBootEnd = pPartInfo->records[u8_i].u16_StartBlk + pPartInfo->records[u8_i].u16_BlkCnt + pPartInfo->records[u8_i].u16_BackupBlkCnt;
                break;
            }
        }
        if(u8_i == pPartInfo->u16_PartCnt)
        {
            printf("ERROR: Partition info does not contain MBOOT partition\n");
            free(pNandInfo);
            return -1;
        }

        //search sboot uboot/ HashX location for update nni infomation

        if(pNandDrv->u8_BL0PBA != 0)    //for bl uboot
        {
            bl_count = 0;
            //search bl location in MBOOT PARTITION
            for(u32_BlkIdx = u32_MBootBegin; u32_BlkIdx < u32_MBootEnd; u32_BlkIdx ++)
            {
                u32_Err = NC_ReadPages(u32_BlkIdx << pNandDrv->u8_BlkPageCntBits, u8MainBuf, u8SpareBuf, 1);
                if(u32_Err != UNFD_ST_SUCCESS || u8SpareBuf[0] !=0xFF)
                    continue;
                if(!drvNAND_CheckAll0xFF(u8MainBuf, pNandDrv->u16_PageByteCnt))
                {
                    if(bl_count == 0)
                        pNandInfo->u8_BL0PBA = pNandDrv->u8_BL0PBA = (U8)u32_BlkIdx;
                    else if(bl_count == 1)
                    {
                        pNandInfo->u8_BL1PBA = pNandDrv->u8_BL1PBA = (U8)u32_BlkIdx;
                        bl_count ++;
                        break;
                    }
                    bl_count ++;
                }
            }

            printf("BL0_PBA %X, BL1_PBA %X\n", pNandDrv->u8_BL0PBA, pNandDrv->u8_BL1PBA);
            if(bl_count != 2)
            {
                printf("WARNING: there is no two sboots in NAND Flash, Please Reupgrade Sboot\n");
                free(pNandInfo);
                return -1;
            }

            if(pNandDrv->u8_UBOOTPBA != 0)
            {
                bl_count = 0;
                for(u32_BlkIdx = u32_MBootBegin; u32_BlkIdx < u32_MBootEnd; u32_BlkIdx ++)
                {
                    u32_Err = NC_ReadPages(u32_BlkIdx << pNandDrv->u8_BlkPageCntBits, u8MainBuf, u8SpareBuf, 1);
                    if(u32_Err != UNFD_ST_SUCCESS || u8SpareBuf[0] !=0xFF)
                        continue;
                    if(((U32 *)u8MainBuf)[0x7] == 0x0000B007)
                    {
                        if(bl_count == 1)
                        {
                            pNandInfo->u8_UBOOTPBA = pNandDrv->u8_UBOOTPBA = (U8)u32_BlkIdx;
                            bl_count ++;
                            break;
                        }
                        bl_count ++;
                    }
                }
                if(bl_count != 2)
                {
                    printf("WARNING: there is no two Mboots in NAND Flash, Please Reupgrade Mboot\n");
                    free(pNandInfo);
                    return -1;
                }

                printf("UBOOTPBA %X\n", pNandDrv->u8_UBOOTPBA);
            }
        }
        else if(pNandDrv->u8_HashPBA[0][0] != 0)    //for hash
        {
            bl_count = 0;
            //search bl location in MBOOT PARTITION
            for(u32_BlkIdx = u32_MBootBegin; u32_BlkIdx < u32_MBootEnd; u32_BlkIdx ++)
            {
                u32_Err = NC_ReadPages(u32_BlkIdx << pNandDrv->u8_BlkPageCntBits, u8MainBuf, u8SpareBuf, 1);
                if(u32_Err != UNFD_ST_SUCCESS || u8SpareBuf[0] !=0xFF)
                    continue;
                if(!drvNAND_CheckAll0xFF(u8MainBuf, pNandDrv->u16_PageByteCnt))
                {
                    pNandInfo->u8_HashPBA[bl_count>>1][bl_count&1] = pNandDrv->u8_HashPBA[bl_count>>1][bl_count&1] = (U8)u32_BlkIdx;
                    if(++bl_count == 4)
                        break;
                }
            }

            printf("HASH00_PBA %X, HASH01_PBA %X\n", pNandInfo->u8_HashPBA[0][0], pNandInfo->u8_HashPBA[0][1]);
            printf("HASH10_PBA %X, HASH11_PBA %X\n", pNandInfo->u8_HashPBA[1][0], pNandInfo->u8_HashPBA[1][1]);
            //printf("HASH00_PBA %X, HASH01_PBA %X\n", pNandInfo->u8_HashPBA[0][0], pNandInfo->u8_HashPBA[0][1]);
            if(bl_count != 4)
            {
                printf("WARNING: there is no two sboots in NAND Flash, Please Reupgrade Sboot\n");
                free(pNandInfo);
                return -1;
            }

            bl_count = 0;
            for(u32_BlkIdx = pNandDrv->u8_HashPBA[1][1]+1; u32_BlkIdx < u32_MBootEnd; u32_BlkIdx ++)
            {
                u32_Err = NC_ReadPages(u32_BlkIdx << pNandDrv->u8_BlkPageCntBits, u8MainBuf, u8SpareBuf, 1);
                if(u32_Err != UNFD_ST_SUCCESS || u8SpareBuf[0] !=0xFF)
                    continue;
                if(((U32 *)u8MainBuf)[0x7] == 0x0000B007)
                {
                    pNandInfo->u8_HashPBA[2][bl_count] = pNandDrv->u8_HashPBA[2][bl_count] = (U8)u32_BlkIdx;
                    if(++bl_count == 2)
                        break;
                }
            }
            if(bl_count != 2)
            {
                printf("WARNING: there is no two Mboots in NAND Flash, Please Reupgrade Mboot\n");
                free(pNandInfo);
                return -1;
            }

            printf("HASH20_PBA %X, HASH21_PBA %X\n", pNandInfo->u8_HashPBA[2][0], pNandInfo->u8_HashPBA[2][1]);
        }
        else
        {
            printf("loop\n");
        }
    }

    dump_nand_info(pNandInfo);

    NC_PlatformInit();

    u32_Err = NC_ResetNandFlash();
    if(u32_Err != UNFD_ST_SUCCESS)
    {
        nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1,"NAND Reset Fail\n");
        free(pNandInfo);
        return -1;
    }
#if defined(FCIE4_DDR) && FCIE4_DDR
    memset((void *) &pNandDrv->tDefaultDDR, 0, sizeof(DDR_TIMING_GROUP_t));
    memset((void *) &pNandDrv->tMaxDDR, 0, sizeof(DDR_TIMING_GROUP_t));
    memset((void *) &pNandDrv->tMinDDR, 0, sizeof(DDR_TIMING_GROUP_t));
#endif
    NC_ConfigNandFlashContext();
    /*Re-Search DDR timing again*/
    u32_Err = NC_Init();
    if(u32_Err != UNFD_ST_SUCCESS)
    {
        nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1,"NC_Init Fail ErrCode: %X\n", u32_Err);
        free(pNandInfo);
        return -1;
    }
    NC_Config();

    nand_config_clock(pNandInfo->u16_tRC);


    drvNAND_WriteCISMIUParam(MIUBLOCK,MIUPOS,pNandInfo);
    free(pNandInfo);
  
    printf("sizeof CIS is %x\n",sizeof(NAND_FLASH_INFO_t));
    return 0;
}

U32 drvNAND_SearchCISMIUParam(void)
{
    U32 u32_Err;

    u32_Err = drvNAND_SearchCIS();

    return u32_Err;
}

U32 drvNAND_WriteLoader(int LOADERBLOCK, U8* u8LDRPos, int size)
{
    NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();
    U32 u32Row = LOADERBLOCK * pNandDrv->u16_BlkPageCnt;
    U32 u8Spare[256 / sizeof(U32)];
    U32 U32Ret = UNFD_ST_SUCCESS;
    
    memset(u8Spare, 0xff, sizeof(u8Spare));
    u8Spare[1] = 0x42000000;
    while(size > 0) {
        printf("[LDR] u32Row=%d u8LDRPos=0x%08X u8Spare[1]=0x%08X\n", u32Row, (U32)u8LDRPos, u8Spare[1]);
        if(u32Row % pNandDrv->u16_BlkPageCnt == 0 && u32Row != LOADERBLOCK * pNandDrv->u16_BlkPageCnt)
            u8Spare[1]++;
        U32 v = NC_WritePages(u32Row, u8LDRPos, (U8 *)u8Spare, 1);
        U32Ret|=v;
        if (v!=UNFD_ST_SUCCESS)
        {
            printf("[LDR] NC_WritePages failed v=%u,u32Row=%d u8APPPos=0x%08X \n", v, u32Row, (U32)u8LDRPos);
        }
        size -= pNandDrv->u16_PageByteCnt;
        u8LDRPos += pNandDrv->u16_PageByteCnt;
        u32Row++;
    }


    printf("[LDR]end blk:%d written \n",u32Row/pNandDrv->u16_BlkPageCnt);
    if (U32Ret!=UNFD_ST_SUCCESS)
    {
        printf("!!!!May occurring NC_WritePages failed\n ");
    }
    return 0;
}





U32 drvNAND_WriteBINParam(int BINBLOCK,U8* u8BINPos)
{
#if ! (defined(CONFIG_KAISER) && (CONFIG_KAISER ==1))
    NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();
    U32 u32Row = BINBLOCK * pNandDrv->u16_BlkPageCnt;
    U32 u8Spare[256 / sizeof(U32)];
    int size = *((U32*)(u8BINPos+12));
    U32 U32Ret = UNFD_ST_SUCCESS;
    
    printf("Bin size %d",size);
    memset(u8Spare, 0xff, sizeof(u8Spare));
    u8Spare[1] = 0x42010000;
    while(size > 0) {
        
        if(u32Row % pNandDrv->u16_BlkPageCnt == 0 && u32Row != BINBLOCK * pNandDrv->u16_BlkPageCnt)
        {
            u8Spare[1]++;
            printf("[APP] u32Row=%d u8APPPos=0x%08X u8Spare[1]=0x%08X size=0x%08x data=0x%08X\n", u32Row, (U32)u8BINPos, u8Spare[1],size,*(U32*)u8BINPos);
        }
        U32 v = NC_WritePages(u32Row, u8BINPos, (U8 *)u8Spare, 1);
        U32Ret|=v;
        if (v!=UNFD_ST_SUCCESS)
        {
            printf("[APP] NC_WritePages failed v=%u,u32Row=%d u8APPPos=0x%08X \n", v, u32Row, (U32)u8BINPos);
        }
        size -= pNandDrv->u16_PageByteCnt;
        u8BINPos += pNandDrv->u16_PageByteCnt;
        u32Row++;
    }


    printf("[APP]end blk:%d written \n",u32Row /pNandDrv->u16_BlkPageCnt);
    if (U32Ret!=UNFD_ST_SUCCESS)
    {
        printf("!!!!May occurring NC_WritePages failed\n ");
    }
    return 0;

#else
    NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();
    static U16 HEADER_OFFSET = 0x100;
    U32 u32Row = BINBLOCK * pNandDrv->u16_BlkPageCnt;
    U32 u32Spare[256 / sizeof(U32)];

    U32 Bin2MIUPos = 0;
    U32 BinSize = 0;
    U32 APPEntryOffset = 0; // means the actually position offset from u8BINPos for bin file to store
    U32 u32ret = 0;

    memset(nand_buffer2, 0, 0x100);
    memset(u32Spare, 0xff, sizeof(u32Spare));
    
    Bin2MIUPos = *((U32*)(u8BINPos));
    BinSize = *((U32*)(u8BINPos+8));
    APPEntryOffset = *((U32*)(u8BINPos+4));
    Bin2MIUPos += HEADER_OFFSET; //APPEntryOffset;
    u8BINPos += HEADER_OFFSET;//APPEntryOffset;
    BinSize -= HEADER_OFFSET;//APPEntryOffset;
    APPEntryOffset -= HEADER_OFFSET;
    *((U32*)(nand_buffer2+0x00)) = BINIDTAG;
    *((U32*)(nand_buffer2+0x04)) = Bin2MIUPos;
    *((U32*)(nand_buffer2+0x10)) = BINIDTAG; // the position we are going to put to MIU and jump
    *((U32*)(nand_buffer2+0x14)) = BinSize;
    *((U32*)(nand_buffer2+0x20)) = BINIDTAG; // the position we are going to put to MIU and jump
    *((U32*)(nand_buffer2+0x24)) = APPEntryOffset;

    u32Spare[1] = 0x42010000;

    //write first page
    if(NC_WritePages(u32Row,(U8*)nand_buffer2,(U8*)u32Spare,1)!=UNFD_ST_SUCCESS)
    {
        printf("[APP]Write first page failed\n");
        return 0;
    }
    
    u32Row ++;
    //u32Spare[1] ++;

    while(BinSize > 0) {
        //printf("[APP] u32Row=%d u8LDRPos=0x%08x u8Spare[0]=0x%08x\n", u32Row, u8BINPos, u32Spare[1]);
        
        if(u32Row % pNandDrv->u16_BlkPageCnt == 0)
        {
            u32Spare[1]++;
            printf("[APP]blk:%d written, u32Spare[1]:0x%x\n",u32Row/pNandDrv->u16_BlkPageCnt-1,u32Spare[1]);
        }
                    
        if (NC_WritePages(u32Row, u8BINPos, u32Spare, 1)!=UNFD_ST_SUCCESS)
        {
            printf("[APP] write page failed\n");
        }

        BinSize -= ((BinSize < pNandDrv->u16_PageByteCnt ) ? BinSize : pNandDrv->u16_PageByteCnt);
        u8BINPos += pNandDrv->u16_PageByteCnt;

        u32Row++;
    }

    return 1;
   
#endif
}


U32 drvNAND_WriteBIN_NonContinues(int BINBLOCK,U8* u8BINPos)
{
    NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();
    U32 u32Row = BINBLOCK * pNandDrv->u16_BlkPageCnt;
    U32 u8Spare[256 / sizeof(U32)];
    int size = *((U32*)(u8BINPos+12));

    printf("Bin size %d",size);
    memset(u8Spare, 0xff, sizeof(u8Spare));
    u8Spare[1] = 0x42010000;
    while(size > 0) {
        if(u32Row % pNandDrv->u16_BlkPageCnt == 0 && u32Row != BINBLOCK * pNandDrv->u16_BlkPageCnt)
        {
            u8Spare[1]++;
            if (u8Spare[1]==0x42010002)
            {
                u32Row+=pNandDrv->u16_BlkPageCnt;
                printf("u32Row mvoe to:%d\n",u32Row);
            }
            
        }
        printf("[APP] u32Row=%d u8APPPos=0x%08X u8Spare[1]=0x%08X size=0x%08x data=0x%08X\n", u32Row,(U32) u8BINPos, (U32)u8Spare[1],size,*(U32*)u8BINPos);
        
        U32 v = NC_WritePages(u32Row, u8BINPos, (U8 *)u8Spare, 1);
        if (v!=UNFD_ST_SUCCESS)
        {
            printf("[APP] NC_WritePages failed v=%u,u32Row=%d u8APPPos=0x%08X \n", v, u32Row, (U32)u8BINPos);
        }
        size -= pNandDrv->u16_PageByteCnt;
        u8BINPos += pNandDrv->u16_PageByteCnt;
        u32Row++;
    }


    printf("[APP]end blk:%d written \n",u32Row /pNandDrv->u16_BlkPageCnt);
    return 0;

}


U32 drvNAND_ReadNANDMIUParam(void)
{
    U32 row_pos = 0;
    NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();

    while(1) {
        printf("next row position %x\n",row_pos);
        BFN_ReadSector_RIU(row_pos, 0,nand_buffer,nand_buffer_spare,1);

        printf("%x\n",*((U32*)(nand_buffer)));
        if (*((U32*)(nand_buffer)) == MIUIDTAG)
        {
            printf("tag correct\n");
            printf("read next block\n");

            break;
        }

        row_pos += pNandDrv->u16_BlkPageCnt;
        if (row_pos > (0x10*pNandDrv->u16_BlkPageCnt ))
            return 1;

    }
U32 BINSize=0;
    while (1)
    {
        printf("2next row position %x\n",row_pos);
        BFN_ReadPage_MIU(row_pos, nand_buffer,nand_buffer_spare,1,0);
        //_PUTDW(*((MS_U32*)(MIUPos)));
        printf("%x\n",*((U32*)(nand_buffer)));
        if (*((U32*)(nand_buffer)) == LDRIDTAG)
        {
            printf("tag correct\n");
            printf("size %x miu pos %x\n",*((U32*)(nand_buffer+20)),*((U32*)(nand_buffer+4)));
            BINSize=*((U32*)(nand_buffer+20));
            break;
        }
        row_pos += pNandDrv->u16_BlkPageCnt;
    }

    U32 i=0;

    while(BINSize)
    {
        //_PUTDW(row_pos);
        printf("3next row position %x %x\n",row_pos,i);
        BFN_ReadPage_MIU(row_pos, nand_buffer,nand_buffer_spare,1,0);
        if (i==0)
        {
            if (*((U32*)(nand_buffer)) != LDRIDTAG)
            {
                row_pos += pNandDrv->u16_BlkPageCnt;
            }
            else
            {
                i++;
            }
            continue;
        }

        //_PUTC('E');
        i++;
        nand_buffer += pNandDrv->u16_PageByteCnt;
        BINSize -= ((BINSize < pNandDrv->u16_PageByteCnt)?BINSize:pNandDrv->u16_PageByteCnt);
        if (i==pNandDrv->u16_BlkPageCnt)
        {
            row_pos += pNandDrv->u16_BlkPageCnt;
            i=0;
        }
        //_PUTDW(BINSize);
    }
    return 0;
}




U32 drvNAND_BFN_FLASH_INFO(U32 bfn_type,U32 bfn_blk,U32 bfn_addr, int size)
{
    NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();

    printf("byte per page %d page per block %d total block %d\n", pNandDrv->u16_PageByteCnt, pNandDrv->u16_BlkPageCnt, pNandDrv->u16_BlkCnt);

    nand_buffer = (unsigned char *)bfn_addr;

    switch (bfn_type)
    {
    case 1://miu
        // if Partial Mode (if not 512B/page, set Partial Mode)
        drvNAND_WriteMIUParam(bfn_blk,bfn_addr);
        //drvNAND_ReadNANDMIUParam(); /* for Test */
        break;
    case 2://ldr
        drvNAND_WriteLoader(bfn_blk,(U8*)bfn_addr, size);
        break;
    case 3://app
        printf("bfn block %x bfn address %x\n",bfn_blk,bfn_addr);
        drvNAND_WriteBINParam(bfn_blk,(U8*)bfn_addr);
        break;
    case 4://chk
        printf("gogogogog\n");
        drvNAND_ReadNANDMIUParam();
        break;
    case 5://cis for box
        printf("refreshCIS for box use\n");
        drvNAND_RefreshCISMIUParam(bfn_blk,bfn_addr);
        break;
    case 6:
        printf("check for CIS correctness\n");
        drvNAND_SearchCISMIUParam();
    case 7:
        printf("Test APP bin in non continues blk\n");
        drvNAND_WriteBIN_NonContinues(bfn_blk,(U8*)bfn_addr);

    default:
        return 0;
    }

    return 1;
}
//=====================
#endif
