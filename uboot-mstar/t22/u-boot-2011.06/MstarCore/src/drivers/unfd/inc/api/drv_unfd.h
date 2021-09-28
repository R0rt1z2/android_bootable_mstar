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

#ifndef __DRV_UNFD_H__
#define __DRV_UNFD_H__

/*=============================================================*/
// Include files
/*=============================================================*/

/*=============================================================*/
// Extern definition
/*=============================================================*/

/*=============================================================*/
// Macro definition
/*=============================================================*/
#ifndef U32
#define U32  unsigned int
#endif
#ifndef U16
#define U16  unsigned short
#endif
#ifndef U8
#define U8   unsigned char
#endif
#ifndef S32
#define S32  signed int
#endif
#ifndef S16
#define S16  signed short
#endif
#ifndef S8
#define S8   signed char
#endif

/*=============================================================*/
// Data type definition
/*=============================================================*/
typedef struct _NAND_INFO
{
	U8	au8_Tag[16];
	U8	u8_IDByteCnt;
	U8	au8_ID[15];
	U32	u32_ChkSum;
	U16	u16_SpareByteCnt;
	U16	u16_PageByteCnt;
	U16	u16_BlkPageCnt;
	U16	u16_BlkCnt;
	U32	u32_Config;
	U16	u16_ECCType;
	U16	u16_tRC;
	U8	padding[12];
	U8	u8_Vendor[16];
	U8	u8_PartNumber[16];

	U16 u16_ECCCodeByteCnt;
	U16 u16_PageSectorCnt;
	U8  u8_WordMode;

} NAND_INFO_t;

typedef struct _PAIRED_PAGE_MAP {
    U16 u16_LSB;
    U16 u16_MSB;
} PAIRED_PAGE_MAP_t, *P_PAIRED_PAGE_MAP_t;

#define PAIRED_PAGE_COUNT  0x200
extern PAIRED_PAGE_MAP_t ga_tPairedPageMap[];

extern U8 gau8_ReadRetryTable[];
/*=============================================================*/
// Variable definition
/*=============================================================*/

/*=============================================================*/
// Global function definition
/*=============================================================*/
extern U32 drvNAND_Init(void);
extern U32 drvNAND_IPVerify_Main(void);


extern U32 drvNAND_GetNandInfo(NAND_INFO_t *pNandInfo);


extern U32 drvNAND_ChkRdy(U32 u32_PartNo);
extern int drv_unfd_init(void);
extern U32 drvNAND_LoadBootLoader(U8 *pu8_DestAddr, U32 u32_StartSector, U32 u32_SectorCnt, U32 u32_LogicIdx);
extern U32 drvNAND_InitProgrammer(void);
extern U32 drvNAND_GetNandID(U8 *pu8IDByteCnt, U8 *pu8ID);
extern U32 drvNAND_CheckCIS(U8 *pu8_CISData);
extern U32 drvNAND_WriteCIS(U8 *pu8_CISData, U8 *pu8_PNIData);
extern U32 drvNAND_ReadCIS(U8 *pu8_CISData);
extern U32 drvNAND_WriteMiuParam(U32 u32_SrcPhyAddr, U32 u32_StartSector, U32 u32_SectorCnt);
extern U32 drvNAND_LoadMiuParam(U32 u32_SrcPhyAddr, U32 u32_StartSector, U32 u32_SectorCnt);
extern U32 drvNAND_EraseMiuParam(void);
extern U32 drvNAND_GetMiuParamCapacity(void);
extern U32 drvNAND_WriteBootLoader(U32 u32_SrcPhyAddr, U32 u32_StartSector, U32 u32_SectorCnt);
extern U32 drvNAND_EraseBootLoader(void);
extern U32 drvNAND_GetBootLoaderCapacity(U32 u32_LogicIdx);
extern U32 drvNAND_WriteOS(U32 u32_SrcPhyAddr, U32 u32_StartSector, U32 u32_SectorCnt);
extern U32 drvNAND_LoadOS(U32 u32_SrcPhyAddr, U32 u32_StartSector, U32 u32_SectorCnt, void (*pfCB)(U32 XferSize));
extern U32 drvNAND_EraseOS(void);
extern U32 drvNAND_GetOSCapacity(void);
extern U32 drvNAND_WriteCUS(U32 u32_SrcPhyAddr, U32 u32_StartSector, U32 u32_SectorCnt);
extern U32 drvNAND_LoadCUS(U32 u32_SrcPhyAddr, U32 u32_StartSector, U32 u32_SectorCnt);
extern U32 drvNAND_EraseCUS(void);
extern U32 drvNAND_GetCUSCapacity(void);
extern U32 drvNAND_EraseAllPartitions(void);
extern U32 drvNAND_WriteNRAI(U32 u32_SrcPhyAddr, U32 u32_StartSector, U32 u32_SectorCnt);
extern U32 drvNAND_ReadNRAI(U32 u32_SrcPhyAddr, U32 u32_StartSector, U32 u32_SectorCnt);
extern U32 drvNAND_GetNRAICapacity(void);
extern U32 drvNAND_WriteE2P0(U32 u32_SrcPhyAddr, U32 u32_StartSector, U32 u32_SectorCnt);
extern U32 drvNAND_ReadE2P0(U32 u32_SrcPhyAddr, U32 u32_StartSector, U32 u32_SectorCnt);
extern U32 drvNAND_GetE2P0Capacity(void);
extern U32 drvNAND_WriteE2P1(U32 u32_SrcPhyAddr, U32 u32_StartSector, U32 u32_SectorCnt);
extern U32 drvNAND_ReadE2P1(U32 u32_SrcPhyAddr, U32 u32_StartSector, U32 u32_SectorCnt);
extern U32 drvNAND_GetE2P1Capacity(void);
extern U32 drvNAND_WriteBacktrace(U32 u32_SrcPhyAddr, U32 u32_StartSector, U32 u32_SectorCnt);
extern U32 drvNAND_ReadBacktrace(U32 u32_SrcPhyAddr, U32 u32_StartSector, U32 u32_SectorCnt);
extern U32 drvNAND_GetBacktraceCapacity(void);
extern U32 drvNAND_WriteFAT(U32 u32_SrcPhyAddr, U32 u32_StartSector, U32 u32_SectorCnt);
extern U32 drvNAND_ReadFAT(U32 u32_SrcPhyAddr, U32 u32_StartSector, U32 u32_SectorCnt);
extern U32 drvNAND_GetFATCapacity(void);
extern U32 drvNAND_WriteNVRAM0(U32 u32_SrcPhyAddr, U32 u32_StartSector, U32 u32_SectorCnt);
extern U32 drvNAND_ReadNVRAM0(U32 u32_SrcPhyAddr, U32 u32_StartSector, U32 u32_SectorCnt);
extern U32 drvNAND_GetNVRAM0Capacity(void);
extern U32 drvNAND_WriteNVRAM1(U32 u32_SrcPhyAddr, U32 u32_StartSector, U32 u32_SectorCnt);
extern U32 drvNAND_ReadNVRAM1(U32 u32_SrcPhyAddr, U32 u32_StartSector, U32 u32_SectorCnt);
extern U32 drvNAND_GetNVRAM1Capacity(void);
extern U32 drvNAND_FormatFTL(void);
extern U32 drvNAND_GetPhyCapacity(void);

extern U32 drvNAND_EraseAllPhyBlk(void);
extern U32 drvNAND_EraseAllGoodPhyBlk(U16 *pu16_GoodBlkCnt, U16 *pu16_BadBlkIdxList, U16 *pu16_BadBlkCnt);
extern U32 drvNAND_ErasePhysicalBlk(U16 u16_BlkIdx);
extern U32 drvNAND_ReadPhysicalPage(U32 u32_PageIdx, U8 *pu8_Data, U8 *pu8_Spare);
extern U32 drvNAND_WritePhysicalPage(U32 u32_PageIdx, U8 *pu8_Data, U8 *pu8_Spare);
extern U32 drvNAND_CheckGoodBlk(U16 u16_BlkIdx);
extern U32 drvNAND_ResetNandFlash(void);
extern U32 drvNAND_FlushCache(void);

extern U32 drvNAND_OTP_Entry(void);
extern U32 drvNAND_OTP_Exit(void);
extern U32 drvNAND_OTP_Lock(void);
extern U32 drvNAND_OTP_WritePages(U32 u32_PhyRowIdx, U8 *pu8_DataBuf, U32 u32_PageCnt);
extern U32 drvNAND_OTP_ReadPages(U32 u32_PhyRowIdx, U8 *pu8_DataBuf, U32 u32_PageCnt);
extern U32 drvNAND_OTP_Access_Size(void);
extern U32 drvNAND_OTP_Total_Size(void);

#endif //__DRV_UNFD_H__

