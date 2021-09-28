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

#ifndef _FTL_HAL_H_
#define _FTL_HAL_H_

//================================
// can not eat the NAND_ID_BYTE_CNT in drvNAND.h
#define NAND_ID_BYTE_CNT    15

// 2-plane MLC ID:



//================================
typedef struct _FTL_DEVICE
{
    U16 u16_BlkPageCnt, u16_BlkLowPCnt;
	U16 u16_PageByteCnt; // cover possible 2-planes
	U16 u16_SectorByteCnt; // depends on ECC, normally 1KB
	U16 u16_SpareByteCnt; // cover possible 2-planes

    U16 u16_BlkSectorCnt;
    U16 u16_Blk512BCnt;
    U16 u16_PageSectorCnt;    
    U16 u16_Page512BCnt;
    U16 u16_Sector512BCnt;
    
	U16 u16_SectorSpareByteCnt;
	U16 u16_ECCCodeByteCnt;

    U8  u8_BlkPageCntBits;
	U8  u8_BlkSectorCntBits;
    U8  u8_Blk512BCntBits;
	U8  u8_PageByteCntBits;
    U8  u8_PageSectorCntBits;
    U8  u8_Page512BCntBits;
	U8  u8_SectorByteCntBits;
    U8  u8_Sector512BCntBits;

    U16 u16_BlkPageCntMask;
	U16 u16_BlkSectorCntMask;
    U16 u16_Blk512BCntMask;
	U16 u16_PageByteCntMask;
    U16 u16_PageSectorCntMask;
    U16 u16_Page512BCntMask;
	U16 u16_SectorByteCntMask;
    U16 u16_Sector512BCntMask;

    U8  u8_CellType, u8_PlaneCnt, u8_RequireRandomizer;
    U8  au8_ID[NAND_ID_BYTE_CNT];
	U8  u8_IDByteCnt;

    #if 1//!(defined(NC_SEL_FCIE5) && NC_SEL_FCIE5)
    U16 u16_FCIEDmaPCntLimit;
    #endif

    // --------------------------
    U32 (*pFnReadPages)(U32 u32_PhyRowIdx, U8 *pu8_DataBuf, U8 *pu8_SpareBuf, U32 u32_PageCnt);    
    U32 (*pFnWritePages)(U32 u32_PhyRowIdx, U8 *pu8_DataBuf, U8 *pu8_SpareBuf, U32 u32_PageCnt);
    U32 (*pFnReadSectors)(U32 u32_PhyRowIdx, U8 u8_SectorInPage, U8 *pu8_DataBuf, U8 *pu8_SpareBuf, U32 u32_SectorCnt);
    U32 (*pFnWriteSectors)(U32 u32_PhyRowIdx, U8 u8_SectorInPage, U8 *pu8_DataBuf, U8 *pu8_SpareBuf, U32 u32_SectorCnts);
    U32 (*pFnEraseBlk)(U16 u16_PBA);
    U32 (*pFnNC_PageCopy)(U32 u32_SrcPhyRowIdx, U32 u32_DstPhyRowIdx, U8 *pu8_DataBuf, U8 *pu8_SpareBuf, U32 u32_PageCnt, U32 *pu32_DonePageCnt);
    #if MSFTL_LINUX
    U32 (*pFnReadSendCmd)(U32 u32_PhyRowIdx, U8 u8_Cmd);
	#endif
    // --------------------------
    U16 u16_StartPBA, u16_PBACnt; // the blocks managed by FTL
    
    // --------------------------
    U32 u32_DevParamChkSum; // to check run-time memory corrupt.

    // --------------------------
    
} FTL_DEVICE, *P_FTL_DEVICE;

extern FTL_DEVICE FtlDev;

extern U8 *pu8_FtlPDatBuf, *pu8_FtlPRddBuf;  // for temporarily operations

extern volatile U32 u32_MemUsageByte;

// ===================================

#define UpdateDevStructChkSum()  \
    FtlDev.u32_DevParamChkSum = drvNAND_CheckSum((U8*)&FtlDev, \
        (U32)(&FtlDev.u32_DevParamChkSum) - (U32)&FtlDev);

extern U32 NFTL_SyncNNIParam(void);
extern U32 NFTL_SyncPNIParam(void);
extern U32 NFTL_HAL_Init(U16 u16_StartPBA, U16 u16_PBACnt);

extern U32 HAL_ReadPages(U32 u32_PhyRowIdx, U8 *pu8_DataBuf, U8 *pu8_SpareBuf, U32 u32_PageCnt);
extern U32 HAL_WritePages(U32 u32_PhyRowIdx, U8 *pu8_DataBuf, U8 *pu8_SpareBuf, U32 u32_PageCnt);
extern U32 HAL_WritePages_HandleBB(U32 u32_PhyRowIdx, U8 *pu8_DataBuf, U8 *pu8_SpareBuf, U32 u32_PageCnt);
extern U32 HAL_ReadSectors(U32 u32_PhyRowIdx, U8 u8_SectorInPage, U8 *pu8_DataBuf, U8 *pu8_SpareBuf, U32 u32_SectorCnt);
extern U32 HAL_WriteSectors(U32 u32_PhyRowIdx, U8 u8_SectorInPage, U8 *pu8_DataBuf, U8 *pu8_SpareBuf, U32 u32_SectorCnt);
extern U32 HAL_PageCopy(U32 u32_SrcPhyRowIdx, U32 u32_DstPhyRowIdx, U8 *pu8_DataBuf, U8 *pu8_SpareBuf, U32 u32_PageCnt);
extern U32 HAL_PageCopy_P2P(U32 u32_SrcPhyRowIdx, U32 u32_DstPhyRowIdx, U8 *pu8_DataBuf, U8 *pu8_SpareBuf, U32 u32_PageCnt);
extern U32 HAL_EraseBlk(U16 u16_PBA);
extern U32 HAL_EraseBlk_HandleBB(U16 u16_PBA);
extern U32 HAL_EraseBlk_HandleBB_SetFB(U16 u16_PBA);

extern U32 HAL_ReadPages2P(U32 u32_PhyRowIdx, U8 *pu8_DataBuf, U8 *pu8_SpareBuf, U32 u32_PageCnt);
extern U32 HAL_WritePages2P(U32 u32_PhyRowIdx, U8 *pu8_DataBuf, U8 *pu8_SpareBuf, U32 u32_PageCnt);
extern U32 HAL_ReadPages2PCache(U32 u32_PhyRowIdx, U8 *pu8_DataBuf, U8 *pu8_SpareBuf, U32 u32_PageCnt);
extern U32 HAL_WritePages2PCache(U32 u32_PhyRowIdx, U8 *pu8_DataBuf, U8 *pu8_SpareBuf, U32 u32_PageCnt);
extern U32 HAL_ReadSectors2P(U32 u32_PhyRowIdx, U8 u8_SectorInPage, U8 *pu8_DataBuf, U8 *pu8_SpareBuf, U32 u32_SectorCnt);
extern U32 HAL_WriteSectors2P(U32 u32_PhyRowIdx, U8 u8_SectorInPage, U8 *pu8_DataBuf, U8 *pu8_SpareBuf, U32 u32_SectorCnt);
extern U32 HAL_EraseBlk2P(U16 u16_PBA);
extern U32 HAL_PageCopy2P(U32 u32_SrcPhyRowIdx, U32 u32_DstPhyRowIdx, U8 *pu8_DataBuf, U8 *pu8_SpareBuf, U32 u32_PageCnt);

extern U32 HAL_ReadSendCmd2P(U32 u32_PhyRowIdx, U8 u8_Cmd);
extern U32 HAL_ReadSendCmd(U32 u32_PhyRowIdx, U8 u8_Cmd);
extern U32 HAL_ReadTransfer(U32 u32_PhyRowIdx, U8 u8_SectorInPage, U8 *pu8_DataBuf, U8 *pu8_SpareBuf, U32 u32_SectorCnt);
extern U32 HAL_SendCmd(U8 u8_Cmd, U8 u8_RBWait);

#endif // _FTL_HAL_H_


