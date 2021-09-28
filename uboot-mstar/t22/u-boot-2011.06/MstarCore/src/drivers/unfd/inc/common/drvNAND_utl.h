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
#ifndef __DRVNAND_UTL_H__
#define __DRVNAND_UTL_H__

#include "drvNAND.h"

extern U32 drvNAND_ProbeReadSeq(void);
extern U32 drvNAND_CheckPartInfo(U32 u32_PageIdx);
extern U32 drvNAND_SearchCIS(void);
extern U32 drvNAND_SearchCIS1(U8 *u8_CIS1Blk);
extern U32 drvNAND_InitBBT(INIT_BBT_t *pInitBBT);
extern void drvNAND_ParseNandInfo(NAND_FLASH_INFO_t *pNandInfo);


typedef UNFD_PACK0 struct _TEST_ALIGN_PACK {

	U8	u8_0;
	U16	u16_0;
	U32	u32_0, u32_1;

} UNFD_PACK1 TEST_ALIGN_PACK_t;
extern U32  drvNAND_CheckAlignPack(U8 u8_AlignByteCnt);
extern U32  nand_CompareData(U8 *pu8_Buf0, U8 *pu8_Buf1, U32 u32_ByteCnt);
extern void dump_mem(unsigned char *buf, int cnt);
extern void dump_nand_info(NAND_FLASH_INFO_t *pNandInfo);
extern void dump_part_records(PARTITION_RECORD_t *pRecord, int cnt);
extern void dump_part_info(PARTITION_INFO_t *pPartInfo);
extern void dump_miu_records(MIU_RECORD_t *records, int cnt);
extern void dump_miu_part(MIU_PART_t *pMiuPart);
extern void dump_general_blk_info(BLK_INFO_t *pBlkInfo);
extern void dump_nand_driver(NAND_DRIVER *pNandDrv);
extern void dump_nand_PPM(void);

extern U32 drvNAND_CompareCISTag(U8 *tag);
extern U8  drvNAND_CountBits(U32 u32_x);
extern U32 drvNAND_CheckSum(U8 *pu8_Data, U32 u32_ByteCnt);

extern PARTITION_RECORD_t *drvNAND_SearchPartition(PARTITION_RECORD_t *pRecord,
												   U16 u16_PartType);

extern U32 drvNAND_ReadBlk(U8 *pu8_DestAddr, U8 u8_PartType, U16 u16_PBA,
							U16 u16_LBA, U32 u32_StartSector, 
							U32 u32_SectorCnt);
extern U32 drvNAND_WriteBlk(U8 *pu8_DestAddr, U8 u8_PartType, U16 u16_PBA,
							U16 u16_LBA, U32 u32_StartSector,
							U32 u32_SectorCnt);

extern U32 drvNAND_WriteBlkNonBackup(U8 *pu8_DestAddr, U8 u8_PartType, U16 u16_PBA,
					 U16 u16_LBA,	U32 u32_StartSector,
					 U32 u32_SectorCnt);

extern U32 drvNAND_CheckSpareSpacePattern(U8 *au8_SpareBuf, U32 u32_PageIdx);
extern U32 drvNAND_ChkGoodBlk(U16 u16_PBA, U16 IfDump);
extern U32 drvNAND_IsGoodBlk(U16 u16_BlkIdx);
extern U32 drvNAND_IsFreeBlk(U16 u16_BlkIdx);
extern U8  drvNAND_CheckAll0xFF(U8* pu8_Buf, U32 u32_ByteCnt);
extern U16 drvNAND_FindFreePage(U16 u16_BlkIdx);
extern U16 drvNAND_FindFreeLSBPageIdx(U16 u16_BlkIdx);
extern U32 drvNAND_MoveBlkData(U16 u16_DestBlk, U16 u16_SrcBlk, U16 u16_PageCnt);

extern U32 drvNAND_MarkBadBlk(U16 u16_BlkIdx);
extern U32 drvNAND_ErasePhyBlk(U16 u16_BlkIdx);
extern U32 drvNAND_ReadPhyPage(U32 u32_PageIdx, U8 *pu8_Data, U8 *pu8_Spare);
extern U32 drvNAND_WritePhyPage(U32 u32_PageIdx, U8 *pu8_Data, U8 *pu8_Spare);
extern U32 drvNAND_LFSRReadPhyPage(U32 u32_PageIdx, U8 *pu8_Data, U8 *pu8_Spare);

extern U32 drvNAND_IsPageLP(U16 PIdx, U16 *PairedPageIdx);
extern U32 drvNAND_WriteDummyToPages(U16 StartPIdx, U16 EndPIdx, U32 u32_BlkRow);
extern U32 drvNAND_LFSRWritePhyPage(U32 u32_PageIdx, U8 *pu8_Data, U8 *pu8_Spare);
extern U32 drvNAND_WriteDummyToMSBPage(U32 u32_PairPageIdx, U32 u32_BlkRow, U8* pu8_PageDataBuf, U8* pu8_SpareDataBuf, U32 (*nand_markbad)(U32));
extern int nand_CheckEmptySectorsFalseAlarm(U8 *main, U8 *spare, U16 u16_SectorCnt);
extern int nand_CheckEmptyPageFalseAlarm(U8 *main, U8 *spare);
extern int nand_ReadDisturbance_BigImg(U32  u32_BlkRow, U8 u8_torture, U32 (*nand_markbad)(U32),  U8 u8_CustSpareMarker, U8 u8_CustMarkerIndex);
extern U32 nand_ReadDisturbance_BigImgRestore(U32 (*nand_markbad)(U32));

extern U32 drvNAND_WriteBlkByteOffset(U8 *pu8_DestAddr, U8 u8_PartType, U16 u16_PBA, U16 u16_LBA, U32 u32_StartByte, U32 u32_ByteCnt);
extern U32 drvNAND_ReadBlkByteOffset(U8 *pu8_DestAddr, U8 u8_PartType, U16 u16_PBA, U16 u16_LBA, U32 u32_StartByte, U32 u32_ByteCnt);

extern U32 drvNAND_ParseAddrToADMA(U8 * pu8_DMADataBuf,  U32 u32_ByteCnt, U8 u8_Direction, ADMA_DESC_t* ADMADesc, U8 u8_IsEnd);

extern U32 drvNAND_WriteBlockECCByPass(U16 u16_PBA, U8* pu8_DataBuf, U16 u16_PageCnt, U16 u16_PhySpareByteCnt);
extern U16 drvNAND_BBT_Rev_StartBlk(void);
extern U16 drvNAND_GetBackupBlk(void);
/*
 * Return Zero Bit Count of input byte against 0xFF
 */
static inline int ZeroBitCountfor0xFFByte(U8 u8_Byte)
{
	int i;
	int sum = 0;
	if(u8_Byte ^ 0xFF)
	{
		for(i = 0; i < 8; i ++)
			sum += ((u8_Byte >> i)  & 0x1) ^ 1;
	}
	return sum;
}

#endif /* __DRVNAND_UTL_H__ */
