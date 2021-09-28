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
#include "drvNAND.h"
#include "drvNAND_utl.h"

extern int search_cis_in_DRAM(U8* pu8_CISAddr, U8* pu8_PPMAddr, U8* pu8_PNIAddr, NAND_FLASH_INFO_t *pNandInfoOut);
extern U32 drvNAND_WriteCIS_for_ROM(NAND_FLASH_INFO_t * pNandInfo);

#define drvNAND_lock_sem()  
#define drvNAND_unlock_sem()    

static UNFD_ALIGN0 U8 sgau8_NandTemp512BBuf[512] UNFD_ALIGN1;

U32 drvNAND_GetPartitionCnt(void)
{
	NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
	PARTITION_INFO_t *pPartInfo = pNandDrv->pPartInfo;

    if(pNandDrv->u8_CISBlk == 0xFF)
        return 0;

    return pPartInfo->u16_PartCnt;
}

U32 drvNAND_GetPartitionInfo(PARTITION_INFO_t *pPartInfo_t)
{
	NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
	PARTITION_INFO_t *pPartInfo = pNandDrv->pPartInfo;

    if(!pPartInfo_t)
        return UNFD_ST_ERR_NULL_PTR;
    
    if(pNandDrv->u8_CISBlk == 0xFF)
        return UNFD_ST_NO_PARTINFO;

    memcpy(pPartInfo_t, pPartInfo, 512);
        
    return UNFD_ST_SUCCESS;
}

U32 drvNAND_GetNandInfo(NAND_INFO_t *pNandInfo_t)
{
	NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();

    if(!pNandInfo_t)
        return UNFD_ST_ERR_NULL_PTR;

    if(pNandDrv->u8_CISBlk == 0xFF)
        return UNFD_ST_NO_NANDINFO;

    memcpy(pNandInfo_t->au8_Tag, "MSTARSEMIUNFDCIS", 16);
    pNandInfo_t->u8_IDByteCnt = pNandDrv->u8_IDByteCnt;
    memcpy(pNandInfo_t->au8_ID, pNandDrv->au8_ID, pNandDrv->u8_IDByteCnt);
    pNandInfo_t->u32_ChkSum = 0;
    pNandInfo_t->u16_SpareByteCnt = pNandDrv->u16_SpareByteCnt;
    pNandInfo_t->u16_PageByteCnt = pNandDrv->u16_PageByteCnt;
    pNandInfo_t->u16_BlkPageCnt = pNandDrv->u16_BlkPageCnt;
    pNandInfo_t->u16_BlkCnt = pNandDrv->u16_BlkCnt;
    pNandInfo_t->u32_Config = pNandDrv->u32_Config;
    pNandInfo_t->u16_ECCType = pNandDrv->u16_ECCType;
    pNandInfo_t->u16_tRC = pNandDrv->u16_tRC;
    pNandInfo_t->u8_IDByteCnt = pNandDrv->u8_IDByteCnt;
    memcpy(pNandInfo_t->u8_Vendor, pNandDrv->u8_Vendor, 16);
    memcpy(pNandInfo_t->u8_PartNumber, pNandDrv->u8_PartNumber, 16);
    pNandInfo_t->u16_ECCCodeByteCnt = pNandDrv->u16_ECCCodeByteCnt;
    pNandInfo_t->u16_PageSectorCnt = pNandDrv->u16_PageSectorCnt;
    pNandInfo_t->u8_WordMode = pNandDrv->u8_WordMode;

    return UNFD_ST_SUCCESS;    
}

U32 drvNAND_WritePartition(U16 u16_PartType, U32 u32_StartPhySector,
						   U32 u32_SrcPhyAddr, U32 u32_SectorCnt )
{
	NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
	PARTITION_INFO_t *pPartInfo = pNandDrv->pPartInfo;
	PARTITION_RECORD_t *pRecord = pPartInfo->records;

	U32 u32_Err = UNFD_ST_SUCCESS;
	U16 u16_BlkIdx;
	U16 u16_i;
	U16 u16_LBA;
	U32 u32_StartSector;
	U32 u32_SectorCntWritten;
	U16 u16_PageSectorCnt;
	U8 u8_PageSectorCntBits;
	//U16 u16_PageSectorCntMask;
	U8 u8_BlkSectorCntBits;
	U16 u16_BlkSectorCnt;
	U16 u16_BlkSectorCntMask;

	drvNAND_lock_sem();

	if(u32_SrcPhyAddr&(UNFD_CACHE_LINE-1))
	{
		nand_debug(UNFD_DEBUG_LEVEL, 1, "buffer is not cache line alignment:%X\n", u32_SrcPhyAddr);
		//return UNFD_ST_ERR_NOT_ALIGN;
	}

	pRecord = drvNAND_SearchPartition(pRecord, u16_PartType);
	if (!pRecord)
	{
		u32_Err = UNFD_ST_ERR_INVALID_PART;
		goto WRITEPARTITION_END;
	}

	if((u32_StartPhySector+u32_SectorCnt) >
		(((((U32)pRecord->u16_BlkCnt+(U32)pRecord->u16_BackupBlkCnt)<<(U32)pNandDrv->u8_BlkPageCntBits)<<(U32)pNandDrv->u8_PageByteCntBits)>>9))
	{
		nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "out of range\n");
		return UNFD_ST_ERR_INVALID_PARAM;
	}

	/* sector size from AP is 512 Bytes, not NAND sector size */
	u16_PageSectorCnt = pNandDrv->u16_PageByteCnt >> 9;
	u8_PageSectorCntBits = drvNAND_CountBits(u16_PageSectorCnt);
	//u16_PageSectorCntMask = (1<<u8_PageSectorCntBits)-1;
	u8_BlkSectorCntBits = pNandDrv->u8_BlkPageCntBits + u8_PageSectorCntBits;
	u16_BlkSectorCnt = 1<<u8_BlkSectorCntBits;
	u16_BlkSectorCntMask = u16_BlkSectorCnt-1;

	u16_LBA = u32_StartPhySector>>u8_BlkSectorCntBits;
	/* Find physical BlkIdx for LBA */
	u16_BlkIdx = pRecord->u16_StartBlk;
	for(u16_i=0 ; u16_i<=u16_LBA; u16_i++ )
	{
		while (!drvNAND_IsGoodBlk(u16_BlkIdx) )
		{
			nand_debug(UNFD_DEBUG_LEVEL_HIGH, 1, "Skip bad blk: 0x%04x\n", u16_BlkIdx);
			if((++u16_BlkIdx)>=(pRecord->u16_StartBlk+pRecord->u16_BlkCnt+pRecord->u16_BackupBlkCnt))
			{
				nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "too many bad block\n");
				u32_Err = UNFD_ST_ERR_LACK_BLK;
				goto WRITEPARTITION_END;
			}
		}
		u16_BlkIdx++;
	}
	u16_BlkIdx--;

	while(u32_SectorCnt>0)
	{
		while (!drvNAND_IsGoodBlk(u16_BlkIdx) )
		{
			nand_debug(UNFD_DEBUG_LEVEL_HIGH, 1, "Skip bad blk: 0x%04x\n", u16_BlkIdx);
			if((++u16_BlkIdx)>=(pRecord->u16_StartBlk+pRecord->u16_BlkCnt+pRecord->u16_BackupBlkCnt))
			{
				nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "too many bad block\n");
				u32_Err = UNFD_ST_ERR_LACK_BLK;
				goto WRITEPARTITION_END;
			}
		}

		u16_LBA = u32_StartPhySector>>u8_BlkSectorCntBits;
		u32_StartSector = u32_StartPhySector&u16_BlkSectorCntMask;
		u32_SectorCntWritten = u32_SectorCnt>(u16_BlkSectorCnt-u32_StartSector) ? (u16_BlkSectorCnt-u32_StartSector) : u32_SectorCnt;

		u32_Err = drvNAND_WriteBlk((U8*)u32_SrcPhyAddr, pRecord->u16_PartType,
								   u16_BlkIdx, u16_LBA, u32_StartSector,
								   u32_SectorCntWritten);
		if (u32_Err != UNFD_ST_SUCCESS)
		{
			nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Write Blk 0x%04x failed with EC: 0x%08x\n",
				   		(unsigned int)u16_BlkIdx, (unsigned int)u32_Err);

			goto WRITEPARTITION_END;
		}

		u32_SectorCnt -= u32_SectorCntWritten;
		u32_StartPhySector += u32_SectorCntWritten;
		u32_SrcPhyAddr += (u32_SectorCntWritten<<9);
		u16_BlkIdx++;
	}

WRITEPARTITION_END:
    drvNAND_unlock_sem();
	return u32_Err;
}

U32 drvNAND_ReadPartition(U16 u16_PartType, U32 u32_StartPhySector,
						   U32 u32_SrcPhyAddr, U32 u32_SectorCnt )
{
	NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
	PARTITION_INFO_t *pPartInfo = pNandDrv->pPartInfo;
	PARTITION_RECORD_t *pRecord = pPartInfo->records;

	U32 u32_Err = UNFD_ST_SUCCESS;
	U16 u16_BlkIdx;
	U16 u16_i;
	U16 u16_LBA;
	U32 u32_StartSector;
	U32 u32_SectorCntRead;
	U16 u16_PageSectorCnt;
	U8 u8_PageSectorCntBits;
	//U16 u16_PageSectorCntMask;
	U8 u8_BlkSectorCntBits;
	U16 u16_BlkSectorCnt;
	U16 u16_BlkSectorCntMask;

    drvNAND_lock_sem();

	pRecord = drvNAND_SearchPartition(pRecord, u16_PartType);
	if (!pRecord)
	{
		u32_Err = UNFD_ST_ERR_INVALID_PART;
		goto READPARTITION_END;
	}

	if((u32_StartPhySector+u32_SectorCnt) >
		(((((U32)pRecord->u16_BlkCnt+(U32)pRecord->u16_BackupBlkCnt)<<(U32)pNandDrv->u8_BlkPageCntBits)<<(U32)pNandDrv->u8_PageByteCntBits)>>9))
	{
		nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "out of range\n");
		goto READPARTITION_END;
	}

	/* sector size from AP is 512 Bytes, not NAND sector size */
	u16_PageSectorCnt = pNandDrv->u16_PageByteCnt >> 9;
	u8_PageSectorCntBits = drvNAND_CountBits(u16_PageSectorCnt);
	//u16_PageSectorCntMask = (1<<u8_PageSectorCntBits)-1;
	u8_BlkSectorCntBits = pNandDrv->u8_BlkPageCntBits + u8_PageSectorCntBits;
	u16_BlkSectorCnt = 1<<u8_BlkSectorCntBits;
	u16_BlkSectorCntMask = u16_BlkSectorCnt-1;

	u16_LBA = u32_StartPhySector>>u8_BlkSectorCntBits;
	/* Find physical BlkIdx for LBA */
	u16_BlkIdx = pRecord->u16_StartBlk;

	for(u16_i=0 ; u16_i<=u16_LBA; u16_i++ )
	{
		while (!drvNAND_IsGoodBlk(u16_BlkIdx) )
		{
			nand_debug(UNFD_DEBUG_LEVEL_HIGH, 1, "Skip bad blk: 0x%04x\n", u16_BlkIdx);
			if((++u16_BlkIdx)>=(pRecord->u16_StartBlk+pRecord->u16_BlkCnt+pRecord->u16_BackupBlkCnt))
			{
				nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "too many bad block\n");
				u32_Err = UNFD_ST_ERR_LACK_BLK;
				goto READPARTITION_END;
			}
		}
		u16_BlkIdx++;
	}

	u16_BlkIdx--;

	while(u32_SectorCnt>0)
	{
		while (!drvNAND_IsGoodBlk(u16_BlkIdx) )
		{
			nand_debug(UNFD_DEBUG_LEVEL_HIGH, 1, "Skip bad blk: 0x%04x\n", u16_BlkIdx);
			if((++u16_BlkIdx)>=(pRecord->u16_StartBlk + pRecord->u16_BlkCnt + pRecord->u16_BackupBlkCnt))
			{
				nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "too many bad block\n");
				u32_Err = UNFD_ST_ERR_LACK_BLK;
				goto READPARTITION_END;
			}
		}

		if(u32_SrcPhyAddr&(UNFD_CACHE_LINE-1))
		{
		    u16_LBA = u32_StartPhySector>>u8_BlkSectorCntBits;
		    u32_StartSector = u32_StartPhySector&u16_BlkSectorCntMask;
		    u32_SectorCntRead = u32_SectorCnt > (u16_BlkSectorCnt - u32_StartSector) ?
			                   (u16_BlkSectorCnt - u32_StartSector) : u32_SectorCnt;

			nand_debug(UNFD_DEBUG_LEVEL, 1, "buffer is not cache line alignment:%X\n", u32_SrcPhyAddr);
    		u32_Err = drvNAND_ReadBlk(sgau8_NandTemp512BBuf, pRecord->u16_PartType,
						   u16_BlkIdx, u16_LBA, u32_StartSector,1);

            memcpy((void *)u32_SrcPhyAddr, (const void *)sgau8_NandTemp512BBuf, 512);

    		if(u32_SectorCntRead> 2)
    		{
				u16_LBA = (u32_StartPhySector+1)>>u8_BlkSectorCntBits;
				u32_StartSector = (u32_StartPhySector+1)&u16_BlkSectorCntMask;

    			u32_Err = drvNAND_ReadBlk((U8*)(u32_SrcPhyAddr+512), pRecord->u16_PartType,
						   u16_BlkIdx, u16_LBA, u32_StartSector,u32_SectorCntRead-2);

    			if( u32_Err != UNFD_ST_SUCCESS)
    			{
    				goto READPARTITION_END;
    			}
    		}

    		if(u32_SectorCntRead>1)
    		{
				u16_LBA = (u32_StartPhySector+u32_SectorCntRead-1)>>u8_BlkSectorCntBits;
				u32_StartSector = (u32_StartPhySector+u32_SectorCntRead-1)&u16_BlkSectorCntMask;

    			u32_Err = drvNAND_ReadBlk(sgau8_NandTemp512BBuf, pRecord->u16_PartType,
						   u16_BlkIdx, u16_LBA, u32_StartSector,1);

    			if( u32_Err != UNFD_ST_SUCCESS)
    			{
    				goto READPARTITION_END;
    			}
    			memcpy((void *)(u32_SrcPhyAddr+512*(u32_SectorCntRead-1)), (const void *)sgau8_NandTemp512BBuf, 512);
    		}

		}
		else
		{
			u16_LBA = u32_StartPhySector>>u8_BlkSectorCntBits;
			u32_StartSector = u32_StartPhySector&u16_BlkSectorCntMask;
			u32_SectorCntRead = u32_SectorCnt > (u16_BlkSectorCnt - u32_StartSector) ?
								(u16_BlkSectorCnt - u32_StartSector) : u32_SectorCnt;

    		u32_Err = drvNAND_ReadBlk((U8*)u32_SrcPhyAddr, pRecord->u16_PartType,
    								   u16_BlkIdx, u16_LBA, u32_StartSector,
    								   u32_SectorCntRead);
		}

		if (u32_Err != UNFD_ST_SUCCESS)
		{
			goto READPARTITION_END;
		}

		u32_SectorCnt -= u32_SectorCntRead;
		u32_StartPhySector += u32_SectorCntRead;
		u32_SrcPhyAddr += (u32_SectorCntRead<<9);
		u16_BlkIdx++;
	}

READPARTITION_END:
    drvNAND_unlock_sem();
	return u32_Err;
}


U32 drvNAND_EraseAllPartitions(void)
{
	NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
	PARTITION_INFO_t *pPartInfo = pNandDrv->pPartInfo;
	PARTITION_RECORD_t *pRecord = pPartInfo->records;
	U16 u16_i, u16_Cnt;
	U32 u32_Err;

	drvNAND_lock_sem();

	u16_Cnt=0;
	for( u16_i=pRecord->u16_StartBlk ; u16_i<pNandDrv->u16_BlkCnt ; u16_i++ )
	{
		if(drvNAND_IsGoodBlk(u16_i))
		{
			u32_Err = drvNAND_ErasePhyBlk(u16_i);
			if( u32_Err != UNFD_ST_SUCCESS )
			{
				if(u32_Err == UNFD_ST_ERR_E_FAIL)
					drvNAND_MarkBadBlk(u16_i);
			}
		}
		else
		{
			nand_debug(0,1,"bad blk: %X\n", u16_i);
			u16_Cnt++;
		}
	}
	nand_debug(0,1,"bad blk cnt: %X\n", u16_Cnt);

	drvNAND_unlock_sem();

	return UNFD_ST_SUCCESS;
}

U32 drvNAND_ErasePartition(U16 u16_PartType)
{
	NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
	PARTITION_INFO_t *pPartInfo = pNandDrv->pPartInfo;
	PARTITION_RECORD_t *pRecord = pPartInfo->records;
	U16 u16_i;
	U32 u32_Err = UNFD_ST_SUCCESS;

	nand_debug(UNFD_DEBUG_LEVEL_HIGH, 1, "+\n");

	drvNAND_lock_sem();

	pRecord = drvNAND_SearchPartition(pRecord, u16_PartType);
	if (!pRecord)
	{
		u32_Err = UNFD_ST_ERR_INVALID_PART;
		goto ERASEOS_END;
	}

	for( u16_i=pRecord->u16_StartBlk ; u16_i<(pRecord->u16_StartBlk+pRecord->u16_BlkCnt+pRecord->u16_BackupBlkCnt) ; u16_i++ )
	{
		if(drvNAND_IsGoodBlk(u16_i))
		{
			u32_Err = drvNAND_ErasePhyBlk(u16_i);
			if( u32_Err != UNFD_ST_SUCCESS )
			{
				if(u32_Err == UNFD_ST_ERR_E_FAIL)
					u32_Err = drvNAND_MarkBadBlk(u16_i);
			}
		}
	}

ERASEOS_END:
	drvNAND_unlock_sem();

	return u32_Err;
}

U32 drvNAND_GetPartitionCapacity(U16 u16_PartType)
{
	NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
	PARTITION_INFO_t *pPartInfo = pNandDrv->pPartInfo;
	PARTITION_RECORD_t *pRecord = pPartInfo->records;

	nand_debug(UNFD_DEBUG_LEVEL_HIGH, 1, "+\n");

	pRecord = drvNAND_SearchPartition(pRecord, u16_PartType);
	if (!pRecord)
		return 0;

	return (pRecord->u16_BlkCnt<<(pNandDrv->u8_BlkPageCntBits+pNandDrv->u8_PageByteCntBits));
}

#if defined(__VER_UNFD_FTL__) && __VER_UNFD_FTL__
static U8 gu8_ftl_ready = 0;
U32 drvNAND_ChkRdy(U32 u32_PartNo)
{
	return gu8_ftl_ready;
}
#endif

U32 drvNAND_WriteCIS(U8 *pu8_CISData, U8 *pu8_PNIData)
{
    U32 u32_Err;
    NAND_FLASH_INFO_t *pNandInfo;

    pNandInfo = (NAND_FLASH_INFO_t* )malloc(512);
    if (!pNandInfo)
	return UNFD_ST_NO_NANDINFO;

    u32_Err = search_cis_in_DRAM((U8*)pu8_CISData, (U8*)NULL, (U8*)pu8_PNIData, pNandInfo);
    if(u32_Err != 0) //if there is no cis for upgrading
    {
        nand_debug(UNFD_DEBUG_LEVEL, 1, "search cis in DRAM fail\n");
        u32_Err = UNFD_ST_ERR_INVALID_PARAM;
    }
    else
    {
        u32_Err = drvNAND_WriteCIS_for_ROM(pNandInfo);
        if(u32_Err != 0)
        {
            nand_debug(UNFD_DEBUG_LEVEL, 1, "Write CIS for ROM fail\n");
        }
    }

	free(pNandInfo);
    
	return u32_Err;

}

int MDrv_Nand_init(unsigned char *nniData, unsigned char *pniData)
{
    NAND_DRIVER *pNandDrv;
    U32 u32_Ret;
    U32 u32_i;
    PARTITION_INFO_t * pPartInfo;
    U8 u8_RewriteCIS = 0;
    
    drvNAND_lock_sem();

    u32_Ret = drvNAND_Init();
    if (u32_Ret == UNFD_ST_SUCCESS || (u32_Ret == UNFD_ST_ERR_NO_CIS))
    {
        if(u32_Ret == UNFD_ST_SUCCESS)
        {
            if(pniData != NULL)
            {
                pNandDrv = drvNAND_get_DrvContext_address();
                for(u32_i = 0; u32_i < NAND_PARTITAION_BYTE_CNT * 20 ; u32_i += NAND_PARTITAION_BYTE_CNT)
                {
                    pPartInfo = (PARTITION_INFO_t*) (pniData + u32_i);

                    if(pPartInfo->u16_SpareByteCnt == pNandDrv->u16_SpareByteCnt &&
                        pPartInfo->u16_PageByteCnt == pNandDrv->u16_PageByteCnt &&
                        pPartInfo->u16_BlkPageCnt == pNandDrv->u16_BlkPageCnt &&
                        pPartInfo->u16_BlkCnt == pNandDrv->u16_BlkCnt) // Match Partition layout
                    {
                        if(memcmp((const void *)pNandDrv->pPartInfo, (const void *)pPartInfo, NAND_PARTITAION_BYTE_CNT) != 0)
                            u8_RewriteCIS = 1;
                        
                        break;
                    }
                }
            }
        }
        else
            u8_RewriteCIS = 1;

        if(u8_RewriteCIS == 0)
            nand_debug(UNFD_DEBUG_LEVEL, 1, "Find CIS\n");
        else
        {
            if(!nniData || !pniData)
            {
                nand_debug(UNFD_DEBUG_LEVEL, 1, "No available nni or pni\n");
                drvNAND_unlock_sem();
                return FALSE;
            }
            
            nand_debug(UNFD_DEBUG_LEVEL, 1, "Write CIS\n");
            u32_Ret = drvNAND_WriteCIS(nniData, pniData);
            if (u32_Ret != UNFD_ST_SUCCESS)
            {
                nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "ERROR, write cis fail: %X\n", u32_Ret);
                drvNAND_unlock_sem();
                return FALSE;
            }
            else
            {
                u32_Ret = drvNAND_Init();
                if (u32_Ret != UNFD_ST_SUCCESS)
                {
                    nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "ERROR, init fail: %X\n", u32_Ret);
                    drvNAND_unlock_sem();
                    return FALSE;
                }
            }
        }
    }
    else
    {
        nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "ERROR, init fail: %X\n", u32_Ret);
        drvNAND_unlock_sem();
        return FALSE;
    }

	#if defined(__VER_UNFD_FTL__) && __VER_UNFD_FTL__
	// init
	u32_Ret = nand_Init_FTL();
	if(UNFD_ST_SUCCESS != u32_Ret)
	{
	    nand_Low_Level_Format();
	    drvNAND_Init();
        nand_Init_FTL();
	}

	gu8_ftl_ready = 1; // UNFD ready
	#endif

	drvNAND_unlock_sem();
    
    return TRUE;
}


#if (defined(__VER_UNFD_FTL__)&&__VER_UNFD_FTL__)
#define NAND_READ_TIMEOUT_MAX_RETRY_CNT  3

U32 drvNAND_WriteFAT(U32 u32_SrcPhyAddr, U32 u32_StartSector, U32 u32_SectorCnt)
{
	NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
	U32 u32_Err = UNFD_ST_SUCCESS;
	U8 u8_DiskNum;
	U32 u32_LogiStartSector;


	if(u32_SrcPhyAddr&(UNFD_CACHE_LINE-1))
	{
		nand_debug(UNFD_DEBUG_LEVEL_HIGH, 1, "buffer is not cache line alignment:%lX\n", u32_SrcPhyAddr);
		//return UNFD_ST_ERR_NOT_ALIGN;
	}

	u8_DiskNum = UNFD_PART_FAT & (~UNFD_LOGI_PART);

	if(u32_StartSector >= pNandDrv->DiskInfo_t[u8_DiskNum].u32_SectorCnt)
	{
		nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "ERROR, NAND Invalid parameter 0\n");
		return UNFD_ST_ERR_INVALID_PARAM;
	}
	if(u32_SectorCnt > pNandDrv->DiskInfo_t[u8_DiskNum].u32_SectorCnt)
	{
		nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "ERROR, NAND Invalid parameter 1\n");
		return UNFD_ST_ERR_INVALID_PARAM;
	}
	if((u32_StartSector+u32_SectorCnt) > pNandDrv->DiskInfo_t[u8_DiskNum].u32_SectorCnt)
	{
		nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "ERROR, NAND Invalid parameter 2\n");
		return UNFD_ST_ERR_INVALID_PARAM;
	}
	u32_LogiStartSector = pNandDrv->DiskInfo_t[u8_DiskNum].u32_StartSector + u32_StartSector;

	drvNAND_lock_sem();

	LABEL_WRITE_FLOW:
	unfd_api_debug(nand_debug(0,1," SecAdr:%X, %X, SecCnt:%X +\n",
		u32_LogiStartSector-pNandDrv->DiskInfo_t[u8_DiskNum].u32_StartSector, u32_LogiStartSector, u32_SectorCnt));



	u32_Err = nand_WriteFlow(u32_LogiStartSector, u32_SectorCnt, u32_SrcPhyAddr);
	if(UNFD_ST_SUCCESS != u32_Err)
	{
		nand_debug(UNFD_DEBUG_LEVEL_ERROR,1,"w fail!!\n");
		goto LABEL_WRITE_FLOW;
	}

    unfd_api_debug(nand_debug(0,1,"-\n\n"));
	drvNAND_unlock_sem();

	if(UNFD_ST_SUCCESS != u32_Err)
		nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "ERROR, NAND fail: %lX \n", u32_Err);
	return u32_Err;
}

U32 drvNAND_ReadFAT(U32 u32_SrcPhyAddr, U32 u32_StartSector, U32 u32_SectorCnt)
{
	NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
	U32 u32_Err = UNFD_ST_SUCCESS;
	U8 u8_DiskNum, u8_RetryCnt;
	U32 u32_LogiStartSector;

	u8_DiskNum = UNFD_PART_FAT & (~UNFD_LOGI_PART);

	if(u32_StartSector >= pNandDrv->DiskInfo_t[u8_DiskNum].u32_SectorCnt)
	{
		nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "ERROR, NAND Invalid parameter 0\n");
		return UNFD_ST_ERR_INVALID_PARAM;
	}
	if(u32_SectorCnt > pNandDrv->DiskInfo_t[u8_DiskNum].u32_SectorCnt)
	{
		nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "ERROR, NAND Invalid parameter 1\n");
		return UNFD_ST_ERR_INVALID_PARAM;
	}
	if((u32_StartSector+u32_SectorCnt) > pNandDrv->DiskInfo_t[u8_DiskNum].u32_SectorCnt)
	{
		nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "ERROR, NAND Invalid parameter 2\n");
		return UNFD_ST_ERR_INVALID_PARAM;
	}
	u32_LogiStartSector = pNandDrv->DiskInfo_t[u8_DiskNum].u32_StartSector + u32_StartSector;

	drvNAND_lock_sem();

	unfd_api_debug(nand_debug(0,1," SecAdr:%X, %X, SecCnt:%X +\n",
		u32_LogiStartSector-pNandDrv->DiskInfo_t[u8_DiskNum].u32_StartSector, u32_LogiStartSector, u32_SectorCnt));

	if(u32_SrcPhyAddr&(UNFD_CACHE_LINE-1))
	{
		nand_debug(UNFD_DEBUG_LEVEL_HIGH, 1, "buffer is not cache line alignment:%lX\n", u32_SrcPhyAddr);

	    u8_RetryCnt = 0;
		do
		{
			u32_Err = nand_ReadFlow(u32_LogiStartSector, 1, (U32)sgau8_NandTemp512BBuf);
			if( u32_Err == UNFD_ST_SUCCESS)
			{
				memcpy((void*)u32_SrcPhyAddr, (const void*)sgau8_NandTemp512BBuf, 512);
			}
			else if( u32_Err == UNFD_ST_ERR_R_TIMEOUT)
			{
				if(++u8_RetryCnt>NAND_READ_TIMEOUT_MAX_RETRY_CNT)
					goto LABEL_ENDREADFAT;

				nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "ERROR, NAND, retry: %X \n", u8_RetryCnt);
			}
			else
				goto LABEL_ENDREADFAT;
		}while(u32_Err != UNFD_ST_SUCCESS);

		if(u32_SectorCnt>2)
		{
		    u8_RetryCnt = 0;
			do
			{
				u32_Err = nand_ReadFlow(u32_LogiStartSector+1, u32_SectorCnt-2, (u32_SrcPhyAddr+512));
				if( u32_Err == UNFD_ST_SUCCESS)
				{
				}
				else if( u32_Err == UNFD_ST_ERR_R_TIMEOUT)
				{
					if(++u8_RetryCnt>NAND_READ_TIMEOUT_MAX_RETRY_CNT)
						goto LABEL_ENDREADFAT;

					nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "ERROR, NAND, retry: %X \n", u8_RetryCnt);
				}
				else
					goto LABEL_ENDREADFAT;
			}while(u32_Err != UNFD_ST_SUCCESS);
		}

		if(u32_SectorCnt>1)
		{
		    u8_RetryCnt = 0;
			do
			{
				u32_Err = nand_ReadFlow(u32_LogiStartSector+u32_SectorCnt-1, 1, (U32)sgau8_NandTemp512BBuf);
				if( u32_Err == UNFD_ST_SUCCESS)
				{
					memcpy((void*)(u32_SrcPhyAddr+512*(u32_SectorCnt-1)), (const void*)sgau8_NandTemp512BBuf, 512);
				}
				else if( u32_Err == UNFD_ST_ERR_R_TIMEOUT)
				{
					if(++u8_RetryCnt>NAND_READ_TIMEOUT_MAX_RETRY_CNT)
						goto LABEL_ENDREADFAT;

					nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "ERROR, NAND, retry: %X \n", u8_RetryCnt);
				}
				else
					goto LABEL_ENDREADFAT;
			}while(u32_Err != UNFD_ST_SUCCESS);
		}
	}
	else
	{
	    u8_RetryCnt = 0;
		do
		{
			u32_Err = nand_ReadFlow(u32_LogiStartSector, u32_SectorCnt, u32_SrcPhyAddr);
			if( u32_Err == UNFD_ST_SUCCESS)
			{
			}
			else if( u32_Err == UNFD_ST_ERR_R_TIMEOUT)
			{
				if(++u8_RetryCnt>NAND_READ_TIMEOUT_MAX_RETRY_CNT)
					goto LABEL_ENDREADFAT;

				nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "ERROR, NAND, retry: %X \n", u8_RetryCnt);
			}
			else
				goto LABEL_ENDREADFAT;
		}while(u32_Err != UNFD_ST_SUCCESS);
	}

LABEL_ENDREADFAT:
	unfd_api_debug(nand_debug(0,1,"-\n\n"));
	drvNAND_unlock_sem();

	if(UNFD_ST_SUCCESS != u32_Err)
		nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "ERROR, NAND fail: %lX \n", u32_Err);
	return u32_Err;
}

U32 drvNAND_GetFATCapacity(void)
{
	U8 u8_DiskNum;

	u8_DiskNum = UNFD_PART_FAT& (~UNFD_LOGI_PART);

	return (nand_get_partition_sector_cnt(u8_DiskNum)<<9);
}

U32 drvNAND_FormatFTL(void)
{
	U32 u32_Err = UNFD_ST_SUCCESS;

	drvNAND_lock_sem();
	nand_Low_Level_Format();
	u32_Err = nand_Init_FTL();
	drvNAND_unlock_sem();

	if(UNFD_ST_SUCCESS != u32_Err)
		nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "ERROR, NAND fail: %lX \n", u32_Err);
	return u32_Err;
}


U32 drvNAND_FlushCache(void)
{
	U8 u8_i;
	U32 u32_Ret;

	#if UNFD_FTL_WBQ
	drvNAND_lock_sem();

	if(0 == drvNAND_ChkRdy(0))
	{
		drvNAND_unlock_sem();
		return UNFD_ST_ERR_NOT_READY;
	}

	unfd_api_debug(nand_debug(0,1,"+\n"));
	for(u8_i=0; u8_i<MAX_WBQ_CNT; u8_i++)
	{
		u32_Ret = nand_FlushWBQ(u8_i);
		if(UNFD_ST_SUCCESS != u32_Ret)
		{
			drvNAND_unlock_sem();
			return u32_Ret;
		}
	}
    unfd_api_debug(nand_debug(0,1,"-\n"));
	drvNAND_unlock_sem();
	#endif

	return UNFD_ST_SUCCESS;
}

U32 drvNAND_FlushAll(void)
{
	// Flush write-back queue and erase counter
	NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
	U8 u8_i;
	U32 u32_Ret;

	drvNAND_lock_sem();

	if(0 == drvNAND_ChkRdy(0))
	{
		drvNAND_unlock_sem();
		return UNFD_ST_ERR_NOT_READY;
	}

	unfd_api_debug(nand_debug(0,1,"+\n"));
#if UNFD_FTL_WL
	for(u8_i=0 ; u8_i<(pNandDrv->u8_Zone1SubZoneCnt+1); u8_i++)
	{
			nand_SaveEraseCounter(u8_i);
	}
#endif

#if UNFD_FTL_WBQ
	for(u8_i=0; u8_i<MAX_WBQ_CNT; u8_i++)
	{
		u32_Ret = nand_FlushWBQ(u8_i);
		if(UNFD_ST_SUCCESS != u32_Ret)
		{
			drvNAND_unlock_sem();
			return u32_Ret;
		}
	}
#endif
    unfd_api_debug(nand_debug(0,1,"-\n"));
	drvNAND_unlock_sem();
	return UNFD_ST_SUCCESS;
}

U32 drvNAND_WearLeveling(void)
{
#if UNFD_FTL_WL
	drvNAND_lock_sem();

	if(0 == drvNAND_ChkRdy(0))
	{
		drvNAND_unlock_sem();
		return UNFD_ST_ERR_NOT_READY;
	}

	unfd_api_debug(nand_debug(0,1,"+\n"));
	nand_Wear_Leveling();
    unfd_api_debug(nand_debug(0,1,"-\n"));

	drvNAND_unlock_sem();
#endif
	return UNFD_ST_SUCCESS;
}

U32 drvNAND_WearLeveling1(void)
{
#if UNFD_FTL_WL
	drvNAND_lock_sem();

	if(0 == drvNAND_ChkRdy(0))
	{
		drvNAND_unlock_sem();
		return UNFD_ST_ERR_NOT_READY;
	}

	unfd_api_debug(nand_debug(0,1,"+\n"));
	nand_Wear_Leveling1();
    unfd_api_debug(nand_debug(0,1,"-\n"));

	drvNAND_unlock_sem();
#endif
	return UNFD_ST_SUCCESS;
}

U32 drvNAND_WL_SaveECnt(U8 u8_ECntZoneIndex)
{
    #if UNFD_FTL_WL
	drvNAND_lock_sem();

	if(0 == drvNAND_ChkRdy(0))
	{
		drvNAND_unlock_sem();
		return UNFD_ST_ERR_NOT_READY;
	}

	unfd_api_debug(nand_debug(0,1,"+\n"));
	nand_SaveEraseCounter(u8_ECntZoneIndex);
	unfd_api_debug(nand_debug(0,1,"-\n"));

	drvNAND_unlock_sem();
    #endif
	return UNFD_ST_SUCCESS;
}


/*
uboot ftl device registration
*/

#if defined(CONFIG_MSTAR_UNFD_FTL) && CONFIG_MSTAR_UNFD_FTL

#include <config.h>
#include <common.h>
#include <command.h>
#include "drv_ftl.h"
#include <part.h>
#include <malloc.h>
#include <linux/list.h>
#include <div64.h>
//ftl include file @FIXME

static struct list_head ftl_devices;
static int cur_dev_num = -1;

struct ftl *find_ftl_device(int dev_num)
{
	struct ftl *m;
	struct list_head *entry;

	list_for_each(entry, &ftl_devices) {
		m = list_entry(entry, struct ftl, link);

		if (m->block_dev.dev == dev_num)
			return m;
	}

	printf("NAND FTL Device %d not found\n", dev_num);

	return NULL;
}
int ftl_register(struct ftl *ftl)
{
	/* Setup the universal parts of the block interface just once */
	ftl->block_dev.if_type = IF_TYPE_NAND;
	ftl->block_dev.dev = cur_dev_num++;
	ftl->block_dev.removable = 0;

	INIT_LIST_HEAD (&ftl->link);

	list_add_tail (&ftl->link, &ftl_devices);

	return 0;
}

block_dev_desc_t *ftl_get_dev(int dev)
{
	struct ftl *ftl = find_ftl_device(dev);

	return ftl ? &ftl->block_dev : NULL;
}

void print_ftl_devices(char separator)
{
	struct ftl *m;
	struct list_head *entry;

	list_for_each(entry, &ftl_devices) {
		m = list_entry(entry, struct ftl, link);

		printf("%s: %d, %lu blocks", m->name, m->block_dev.dev, m->block_dev.lba);

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

	print_ftl_devices(',');

	return 0;
}

static struct ftl ftl;

static unsigned long drvNAND_bread(int dev_num, ulong start, lbaint_t blkcnt, void *dst)
{
    U32 u32_Err;
    if( blkcnt == 0)
        return 0;
    u32_Err = nand_ReadFlow(start , blkcnt, dst);
    if (u32_Err == UNFD_ST_SUCCESS)
        return blkcnt;
    else
        return 0;
}

static unsigned long drvNAND_bwrite(int dev_num, ulong start, lbaint_t blkcnt, const void *src)
{
    U32 u32_Err;
    void* p_src = (U8*) src;

    u32_Err = nand_WriteFlow(start, blkcnt, p_src);
    if (u32_Err == UNFD_ST_SUCCESS)
        return blkcnt;
    else
        return 0;
}

static unsigned long drvNAND_berase(int dev_num, ulong start, lbaint_t blkcnt)
{
    return 0;
}


U32 drvNAND_BlkDev_Init(void)
{
    NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();
    U32 u32_Err;
    ftl_initialize();
    memset(&ftl, 0, sizeof(struct ftl));
    ftl_register(&ftl);

    if(pNandDrv->u8_HasPNI)
    {
        u32_Err = nand_Init_FTL();
		if(u32_Err != UNFD_ST_SUCCESS)
		{
		    u32_Err = nand_Low_Level_Format();
		    if(u32_Err != UNFD_ST_SUCCESS)
		    {
		        nand_debug(0, 1, "UNFD nand_Low_Level_Format FTL initial fail: 0x%X\n", u32_Err);
                return u32_Err;
		    }
		    memset(pNandDrv->DiskInfo_t, 0, sizeof(NAND_DRIVER) - ((U32)(pNandDrv->DiskInfo_t) - (U32)(pNandDrv)));
		    u32_Err = nand_Init_FTL();
		    if(u32_Err != UNFD_ST_SUCCESS)
		    {
		        nand_debug(0, 1, "UNFD FTL initial fail: 0x%X\n", u32_Err);
		        return u32_Err;
		    }
		}
		drvNAND_BGThread_Init();     //nothing return
		gu8_ftl_ready = 1;
	}
	else
	{
		nand_debug(0, 1, "No partition info(pni) in NAND\n");
		return 1;
	}

    sprintf(ftl.name, "Mstar-FTL");
    ftl.capacity = (U64) (pNandDrv->DiskInfo_t[0].u32_SectorCnt << 9);
    ftl.read_bl_len = 512;
    ftl.write_bl_len = 512;
    ftl.block_dev.lun = 0;
    ftl.block_dev.type = 0;
    ftl.block_dev.blksz = 512;
    ftl.block_dev.lba = pNandDrv->DiskInfo_t[0].u32_SectorCnt;
    ftl.block_dev.part_type = PART_TYPE_NAND;
    ftl.block_dev.block_read = drvNAND_bread;
    ftl.block_dev.block_write = drvNAND_bwrite;
    ftl.block_dev.block_erase = drvNAND_berase;
	print_ftl_devices(",");
    return 0;
}

#endif

#endif
