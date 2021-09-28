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

#include "ftl_api.h"

FTL_DEVICE FtlDev;

// for temporarily operations:
//   - pass to HAL_ APIs, and then used immediately.
//   - CAUTION: used by NFTL_IsFreeBlk, NFTL_IsFreePage.
U8 *pu8_FtlPDatBuf=NULL, *pu8_FtlPRddBuf=NULL;

volatile U32 u32_MemUsageByte;

#if TEMP_CHECK
U8 au8_DataReadBuf[16384];
U8 au8_SpareReadBuf[2000];
extern int nand_CheckEmptyPageFalseAlarm(U8 *main, U8 *spare);
#endif

void NFTL_free(void)
{
}

static void *NFTL_malloc(void)
{
    U32 u32_tmp;
    U16 u16_i, u16_j;
    
    // --------------------------
    // allocate page buf
    if(0==FtlDev.u16_PageByteCnt)
    {
        ftl_dbg(FTL_DBG_LV_ERR,1,"abnormal PageByteCnt: %Xh \n", FtlDev.u16_PageByteCnt);
        return NULL;
    }
    if(0==FtlDev.u16_SpareByteCnt)
    {
        ftl_dbg(FTL_DBG_LV_ERR,1,"abnormal SpareByteCnt: %Xh \n", FtlDev.u16_SpareByteCnt);
        return NULL;
    }

    if(NULL == pu8_FtlPDatBuf)
    {
		#if MSFTL_LINUX && defined(CONFIG_MIPS)
		pu8_FtlPDatBuf = FDMAMALLOC(FtlDev.u16_PageByteCnt);
		#else
        pu8_FtlPDatBuf = FMALLOC(FtlDev.u16_PageByteCnt);
		#endif
        u32_MemUsageByte += FtlDev.u16_PageByteCnt;
    }
    
    if(NULL == pu8_FtlPRddBuf)
    {
		#if MSFTL_LINUX && defined(CONFIG_MIPS)
		pu8_FtlPRddBuf = FDMAMALLOC(FtlDev.u16_SpareByteCnt);
		#else
        pu8_FtlPRddBuf = FMALLOC(FtlDev.u16_SpareByteCnt);
		#endif
        u32_MemUsageByte += FtlDev.u16_SpareByteCnt;
    }

    // --------------------------
    // allocate BBT buf
    if(NULL == pu8_FtlIBBT)
    {
        pu8_FtlIBBT = (U8*)FMALLOC(BBT_BYTECNT);
        u32_MemUsageByte += BBT_BYTECNT;
    }

    if(NULL == pu8_FtlRBBT)
    {
        pu8_FtlRBBT = (U8*)FMALLOC(BBT_BYTECNT);
        u32_MemUsageByte += BBT_BYTECNT;
    }

    // --------------------------
    // allocate flag buf to check if block free or kept
    if(NULL == pu8_IfBlkKept)
    {
		#if MSFTL_LINUX
        pu8_IfBlkKept = (U8*)FVMALLOC(FtlDev.u16_PBACnt/8+1);
		#else
        pu8_IfBlkKept = (U8*)FMALLOC(FtlDev.u16_PBACnt/8+1);
		#endif
        u32_MemUsageByte += FtlDev.u16_PBACnt/8+1;
    }
    memset(pu8_IfBlkKept, 0, FtlDev.u16_PBACnt/8+1);

    if(NULL == pu8_IfBlkInFBQ)
    {
		#if MSFTL_LINUX
        pu8_IfBlkInFBQ = (U8*)FVMALLOC(FtlDev.u16_PBACnt/8+1);
		#else
        pu8_IfBlkInFBQ = (U8*)FMALLOC(FtlDev.u16_PBACnt/8+1);
		#endif
        u32_MemUsageByte += FtlDev.u16_PBACnt/8+1;
    }
    memset(pu8_IfBlkInFBQ, 0, FtlDev.u16_PBACnt/8+1);

    if(NULL == pu8_IfBlkIsDB)
    {
		#if MSFTL_LINUX
        pu8_IfBlkIsDB = (U8*)FVMALLOC(FtlDev.u16_PBACnt/8+1);
		#else
        pu8_IfBlkIsDB = (U8*)FMALLOC(FtlDev.u16_PBACnt/8+1);
		#endif
        u32_MemUsageByte += FtlDev.u16_PBACnt/8+1;
    }
    memset(pu8_IfBlkIsDB, 0, FtlDev.u16_PBACnt/8+1);

    // --------------------------
    if(NULL == pu8_PIdx2LPIdx) 
    {
		#if MSFTL_LINUX
        pu8_PIdx2LPIdx = (U8*)FVMALLOC(FtlDev.u16_BlkPageCnt);
		#else
        pu8_PIdx2LPIdx = (U8*)FMALLOC(FtlDev.u16_BlkPageCnt);
		#endif
        u32_MemUsageByte += FtlDev.u16_BlkPageCnt;
    }
    FtlDev.u16_BlkLowPCnt = 0;
    u16_j = FtlDev.u16_BlkPageCnt-1;
    for(u16_i=0; u16_i<FtlDev.u16_BlkPageCnt; u16_i++)
    {
        FtlDev.u16_BlkLowPCnt++;
        
        pu8_PIdx2LPIdx[ga_tPairedPageMap[u16_i].u16_LSB] = u16_i;
        if(u16_j==ga_tPairedPageMap[u16_i].u16_LSB)
            break;
        
        if(ga_tPairedPageMap[u16_i].u16_MSB == 0xFFFF)
            continue;        
        pu8_PIdx2LPIdx[ga_tPairedPageMap[u16_i].u16_MSB] = u16_i;
        if(u16_j==ga_tPairedPageMap[u16_i].u16_MSB)
            break;
    }

    if(NULL == pu8_IfMSBPage) 
    {
		#if MSFTL_LINUX
        pu8_IfMSBPage = (U8*)FVMALLOC(FtlDev.u16_BlkPageCnt/8+1);
		#else
        pu8_IfMSBPage = (U8*)FMALLOC(FtlDev.u16_BlkPageCnt/8+1);
		#endif
        u32_MemUsageByte += FtlDev.u16_BlkPageCnt/8+1;
    }
    memset(pu8_IfMSBPage, 0, FtlDev.u16_BlkPageCnt/8+1);
    for(u32_tmp=0; u32_tmp<FtlDev.u16_BlkLowPCnt; u32_tmp++)
    {
        if(ga_tPairedPageMap[u32_tmp].u16_MSB == 0xFFFF)
            continue;
        SET_PAGE_MSB(ga_tPairedPageMap[u32_tmp].u16_MSB);
    }

    #if 0 // debug
    for(u16_i=0; u16_i<FtlDev.u16_BlkPageCnt; u16_i++)
        ftl_dbg(0,0,"PIdx: %3Xh  FLPIdx: %3Xh \n", u16_i, NFTL_PIdx2FLPIdx(u16_i));
    #endif

    // --------------------------
    // dedicated used in ftl_data.c (NFTL_WriteData_Ex)
    if(NULL == pNANDPDataBuf)
    {
		#if MSFTL_LINUX && defined(CONFIG_MIPS)
        pNANDPDataBuf = (U8*)FDMAMALLOC(FtlDev.u16_PageByteCnt);
		#else
        pNANDPDataBuf = (U8*)FMALLOC(FtlDev.u16_PageByteCnt);
		#endif
        u32_MemUsageByte += FtlDev.u16_PageByteCnt;
    }

    if(NULL == pNANDBigBuf)
    {
		#if MSFTL_LINUX && defined(CONFIG_MIPS)
        pNANDBigBuf = (U8*)FDMAMALLOC(NFTL_BIG_BUF_BYTCNT);
		#else
        pNANDBigBuf = (U8*)FMALLOC(NFTL_BIG_BUF_BYTCNT);
		#endif
        u32_MemUsageByte += NFTL_BIG_BUF_BYTCNT;
    }
    
    if(NULL == pNANDSectorBuf)
    {
		#if MSFTL_LINUX && defined(CONFIG_MIPS)
        pNANDSectorBuf = (U8*)FDMAMALLOC(FtlDev.u16_SectorByteCnt);
		#else
        pNANDSectorBuf = (U8*)FMALLOC(FtlDev.u16_SectorByteCnt);
		#endif
        u32_MemUsageByte += FtlDev.u16_SectorByteCnt;
    }
    
    return pu8_FtlPDatBuf;
}


static U32 NFTL_CheckDevLimit(void)
{
    NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();

    // leave the end of NAND for MTD BBT
    if(FtlDev.u16_StartPBA + FTL_1PBlkCnt() > pNandDrv->u16_BlkCnt-10)
    {
        ftl_dbg(FTL_DBG_LV_ERR,1,"StartBlk + BlkCnt = %Xh > NAND total BlkCnt: %Xh - 10 \n", 
            FtlDev.u16_StartPBA + FtlDev.u16_PBACnt, pNandDrv->u16_BlkCnt);
        return FTL_ERR_OUT_OF_RANGE;
    }
    // check if touch CIS reserced areas
    if(FtlDev.u16_StartPBA < 10)
    {
        ftl_dbg(FTL_DBG_LV_ERR,1,"abnormal StartPBA: %Xh < 10 \n", FtlDev.u16_StartPBA);
        return FTL_ERR_OUT_OF_RANGE;
    }
    // check basic FTL PBA count
    if(FtlDev.u16_PBACnt > MAX_FTL_BLKCNT)
    {
        ftl_dbg(FTL_DBG_LV_ERR,1,"abnormal PBACnt:%Xh, max is: %Xh \n", 
            FtlDev.u16_PBACnt, MAX_FTL_BLKCNT);
        return FTL_ERR_PART_PARAM;
    }
    // check NAND parameters
    if( FtlDev.u16_PageByteCnt > MAX_PDAT_BYTECNT ||
        FtlDev.u16_SpareByteCnt > MAX_PRDD_BYTECNT||
        FtlDev.u16_BlkPageCnt > MAX_BLK_PAGECNT)
    {
        ftl_dbg(FTL_DBG_LV_ERR,1,
            "abnormal BlkPageCnt: %Xh, PageByteCnt: %Xh, SpareByteCnt: %Xh\n", 
            FtlDev.u16_BlkPageCnt, FtlDev.u16_PageByteCnt, FtlDev.u16_SpareByteCnt);
        return FTL_ERR_NAND_PARAM;
    }
    
    return FTL_OK;
}


U32 NFTL_SyncPNIParam(void)
{
    U16 u16_i;    
    NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();

    for(u16_i=0; u16_i<pNandDrv->pPartInfo->u16_PartCnt; u16_i++)
        if(pNandDrv->pPartInfo->records[u16_i].u16_PartType == UNFD_PART_FTL)
            break;

    if(u16_i == pNandDrv->pPartInfo->u16_PartCnt)
    {
        ftl_dbg(FTL_DBG_LV_WARN,1,"PNI has no FTL partition.\n");
        dump_part_info(pNandDrv->pPartInfo);
        return FTL_ERR_NO_FTL_PART;
    }

    FtlDev.u16_StartPBA = pNandDrv->pPartInfo->records[u16_i].u16_StartBlk;
    FtlDev.u16_PBACnt = pNandDrv->pPartInfo->records[u16_i].u16_BlkCnt +
                        pNandDrv->pPartInfo->records[u16_i].u16_BackupBlkCnt;

    return FTL_OK;
}


static U32 NFTL_SyncNNIParam_Ex(void)
{    
    FtlDev.u8_BlkPageCntBits = drvNAND_CountBits(FtlDev.u16_BlkPageCnt);
    FtlDev.u8_PageByteCntBits = drvNAND_CountBits(FtlDev.u16_PageByteCnt);
    FtlDev.u8_SectorByteCntBits = drvNAND_CountBits(FtlDev.u16_SectorByteCnt);

    FtlDev.u16_PageSectorCnt = FtlDev.u16_PageByteCnt >> FtlDev.u8_SectorByteCntBits;
    FtlDev.u16_BlkSectorCnt = FtlDev.u16_PageSectorCnt << FtlDev.u8_BlkPageCntBits;
    FtlDev.u8_PageSectorCntBits = drvNAND_CountBits(FtlDev.u16_PageSectorCnt);
    FtlDev.u8_BlkSectorCntBits = drvNAND_CountBits(FtlDev.u16_BlkSectorCnt);
        
    FtlDev.u16_SectorSpareByteCnt = FtlDev.u16_SpareByteCnt >> FtlDev.u8_PageSectorCntBits;
    
    FtlDev.u16_BlkPageCntMask = FtlDev.u16_BlkPageCnt-1;
    FtlDev.u16_PageByteCntMask = FtlDev.u16_PageByteCnt-1;
    FtlDev.u16_SectorByteCntMask = FtlDev.u16_SectorByteCnt-1;
    
    FtlDev.u16_PageSectorCntMask = FtlDev.u16_PageSectorCnt-1;
    FtlDev.u16_BlkSectorCntMask = FtlDev.u16_BlkSectorCnt-1;
    
    // --------------------------
    FtlDev.u16_Blk512BCnt = FtlDev.u16_BlkPageCnt<<(FtlDev.u8_PageByteCntBits-9);
    FtlDev.u16_Blk512BCntMask = FtlDev.u16_Blk512BCnt - 1;
    FtlDev.u8_Blk512BCntBits = drvNAND_CountBits(FtlDev.u16_Blk512BCnt);
    FtlDev.u16_Page512BCnt = FtlDev.u16_PageByteCnt >> 9;
    FtlDev.u16_Page512BCntMask = FtlDev.u16_Page512BCnt - 1;
    FtlDev.u8_Page512BCntBits = drvNAND_CountBits(FtlDev.u16_Page512BCnt);
    FtlDev.u16_Sector512BCnt = FtlDev.u16_SectorByteCnt >> 9;
    FtlDev.u16_Sector512BCntMask = FtlDev.u16_Sector512BCnt - 1;
    FtlDev.u8_Sector512BCntBits = drvNAND_CountBits(FtlDev.u16_Sector512BCnt);
     
    return FTL_OK;      
}


U32 NFTL_SyncNNIParam(void)
{
    NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();

    FtlDev.u16_BlkPageCnt = pNandDrv->u16_BlkPageCnt;
    FtlDev.u16_PageByteCnt = pNandDrv->u16_PageByteCnt;
    FtlDev.u16_SectorByteCnt = pNandDrv->u16_SectorByteCnt;
    FtlDev.u16_SpareByteCnt = pNandDrv->u16_SpareByteCnt;
    FtlDev.u16_ECCCodeByteCnt = pNandDrv->u16_ECCCodeByteCnt;
    
    FtlDev.u8_CellType = pNandDrv->u8_CellType;
    FtlDev.u8_RequireRandomizer = pNandDrv->u8_RequireRandomizer;
    //FtlDev.u16_BlkLowPCnt = FtlDev.u16_BlkPageCnt>>FtlDev.u8_CellType;

    FtlDev.u8_IDByteCnt = pNandDrv->u8_IDByteCnt;
    memcpy(FtlDev.au8_ID, pNandDrv->au8_ID, FtlDev.u8_IDByteCnt);

    // --------------------------    
    NFTL_CheckMultiPlane();

    // --------------------------
    NFTL_SyncNNIParam_Ex();

    return FTL_OK;
}


U32 NFTL_HAL_Init(U16 u16_StartPBA, U16 u16_PBACnt)
{
    U32 u32_err;

    memset(&FtlDev, 0, sizeof(FTL_DEVICE));

    if(u16_StartPBA ==0 && u16_PBACnt == 0)
    {
        u32_err = NFTL_SyncPNIParam();
        if(FTL_OK != u32_err)
        {
            FtlDev.u16_StartPBA = u16_StartPBA;
            FtlDev.u16_PBACnt = u16_PBACnt;
            ftl_dbg(FTL_DBG_LV_WARN,1,"PNI has no FTL partition, "
                "set range by parameters: %Xh~%Xh, %u blocks. \n",
                u16_StartPBA, u16_StartPBA+u16_PBACnt, u16_PBACnt);
            return u32_err;
        }
    }
    else
    {
        FtlDev.u16_StartPBA = u16_StartPBA;
        FtlDev.u16_PBACnt = u16_PBACnt;        
    }

    // init multi-plane inside (PBACnt)
    u32_err = NFTL_SyncNNIParam();
    if(FTL_OK != u32_err)
        return u32_err;

    u32_err = NFTL_CheckDevLimit();
    if(FTL_OK != u32_err)
        return u32_err;

    ftl_dbg(FTL_DBG_LV_LOG,0,"[FTL] StartPBA: %Xh, PBACnt: %Xh, %u.%u MB \n",
        FtlDev.u16_StartPBA, FtlDev.u16_PBACnt, 
        BlkCntToMB(FTL_1PBlkCnt())/FtlDev.u8_PlaneCnt, 
        (BlkCntToMB(FTL_1PBlkCnt()*10)/FtlDev.u8_PlaneCnt)%10);

    #if !(defined(NC_SEL_FCIE5) && NC_SEL_FCIE5)
    FtlDev.u16_FCIEDmaPCntLimit = 4095 >>  FtlDev.u8_PageSectorCntBits;
    #else
    FtlDev.u16_FCIEDmaPCntLimit = 0xFFF0; // no limited
    #endif
    
    if(NULL == NFTL_malloc()) // <- BlkLowPCnt init inside
    {
        ftl_dbg(FTL_DBG_LV_ERR,1,"get mem fail !\n");
        return FTL_ERR_GET_MEM_FAIL;
    }

    UpdateDevStructChkSum();

    //================================
    #if TEST_HW_TIMER // test HW timer
    for(u32_err=0; u32_err<6; u32_err++)
    {
        ftl_dbg(FTL_DBG_LV_LOG,0,"%1lu \n", u32_err);
        nand_hw_timer_delay(DELAY_1s_in_us);
    }
    #endif

    // for PowerCut test
    #if PWROFF_EN
    HWTimer_Start();
    #endif

    return FTL_OK;
}

// ============================================
// wrap for 
//   1. connect different HAL APIs (e.g. 2-planes)
//   2. debug: read before write, read after write.
//   3. error handle for CopyPages.
//   4. separate RDD to different sectors.
//   5. do more error handles:
//       - write/erase fail: 
//          - can not err handle here, for reasons:
//             - don't know FTL write/erase which kinds of block, e.g. ROOT or DATA.
//             - different kinds of FTL blocks, need different err handle.
//             - can't call FTL_HAL itself (to avoid re-entry).
//       - read fail: 
//          - assert since NC_HAL already retry, FTL does not need to check returned ok or not.
// ============================================
U32 HAL_ReadPages(U32 u32_PhyRowIdx, U8 *pu8_DataBuf, U8 *pu8_SpareBuf, U32 u32_PageCnt)
{
    U32 u32_err = UNFD_ST_SUCCESS, tmp;
	int ecc =0;

    #if TEMP_CHECK
    if(RowBlkAddr(u32_PhyRowIdx) < FtlDev.u16_StartPBA || 
        RowBlkAddr(u32_PhyRowIdx) >= FtlDev.u16_StartPBA+FTL_1PBlkCnt())
    {
        ftl_dbg(0,1,"PRow:%Xh  PBA: %Xh %Xh \n", u32_PhyRowIdx,
            FtlDev.u16_StartPBA, FtlDev.u16_StartPBA+FTL_1PBlkCnt());
        ftl_die();
    }
    #endif

    //ftl_dbg(FTL_DBG_LV_LOG,1,"%Xh %Xh %Xh %Xh +\n",
    //    u32_PhyRowIdx, (U32)pu8_DataBuf, (U32)pu8_SpareBuf, u32_PageCnt);
	if( u32_PageCnt <= FtlDev.u16_FCIEDmaPCntLimit)
	{
	    u32_err = NC_ReadPages(u32_PhyRowIdx, pu8_DataBuf, pu8_SpareBuf, u32_PageCnt);
	}
	else
	{
	    while(u32_PageCnt)
	    {
            tmp = u32_PageCnt > FtlDev.u16_FCIEDmaPCntLimit ?
                FtlDev.u16_FCIEDmaPCntLimit : u32_PageCnt;
            
            u32_err = NC_ReadPages(u32_PhyRowIdx, 
                pu8_DataBuf, pu8_SpareBuf, tmp);
            if(UNFD_ST_SUCCESS != u32_err)
                break;
            
			NC_CheckECC(&ecc); // CAUTION: not precisely
			if(ecc)
			{
				NFTL_RD_Set(RowBlkAddr(u32_PhyRowIdx),
					u32_PhyRowIdx & FtlDev.u16_BlkPageCntMask);
			}
            
            pu8_DataBuf += FtlDev.u16_PageByteCnt*tmp/FtlDev.u8_PlaneCnt;
	        u32_PageCnt -= tmp;
	        u32_PhyRowIdx += tmp;
	    }
	}
    if(UNFD_ST_SUCCESS == u32_err)
        return u32_err;

    ftl_dbg(FTL_DBG_LV_WARN,1,"error: %Xh, retry..\n", u32_err);

    while(u32_PageCnt)
    {
        u32_err = NC_ReadPages(u32_PhyRowIdx, 
            pu8_DataBuf, pu8_SpareBuf, 1);
        if(UNFD_ST_SUCCESS != u32_err)
            break;
		NC_CheckECC(&ecc);
		if(ecc)
		{
			NFTL_RD_Set(RowBlkAddr(u32_PhyRowIdx),
			    u32_PhyRowIdx & FtlDev.u16_BlkPageCntMask);
		}
        pu8_DataBuf += FtlDev.u16_PageByteCnt/FtlDev.u8_PlaneCnt;
        //pu8_SpareBuf +=FtlDev.u16_SpareByteCnt / FtlDev.u8_PlaneCnt; 
        u32_PageCnt--;
        u32_PhyRowIdx++;
    }

    return u32_err;
}


// CAUTION: not tested
U32 HAL_ReadPages2P(U32 u32_PhyRowIdx, U8 *pu8_DataBuf, U8 *pu8_SpareBuf, U32 u32_PageCnt)
{
    U32 u32_err = UNFD_ST_SUCCESS, u32_row, i;
	int ecc;

    u32_row = PhyRowFromMultiPlaneRow(u32_PhyRowIdx);

    #if TEMP_CHECK
    if(RowBlkAddr(u32_row) < FtlDev.u16_StartPBA ||
        RowBlkAddr(u32_row) >= FtlDev.u16_StartPBA+FTL_1PBlkCnt())
    {
        ftl_dbg(0,1,"PRow:%Xh  PBA: %Xh %Xh \n", u32_row,
            FtlDev.u16_StartPBA, FtlDev.u16_StartPBA+FTL_1PBlkCnt());
        ftl_die();
    }
    #endif
    //ftl_dbg(FTL_DBG_LV_LOG,1,"%Xh %Xh %Xh %Xh \n", 
    //    u32_row, (U32)pu8_DataBuf, (U32)pu8_SpareBuf, u32_PageCnt);
	if( ((u32_PageCnt << FtlDev.u8_PageSectorCntBits) * FtlDev.u8_PlaneCnt) <= 4095)
	{
	    u32_err = NC_ReadPages2P(u32_row, pu8_DataBuf, pu8_SpareBuf, u32_PageCnt);
	}
	else
	{
		while(u32_PageCnt)
		{
			u32_err = NC_ReadPages2P(u32_row, pu8_DataBuf, pu8_SpareBuf, 1);
			if(UNFD_ST_SUCCESS != u32_err)
				break;
			pu8_DataBuf += FtlDev.u16_PageByteCnt;
			u32_PageCnt--;
			u32_row++;
		}
	}
    if(UNFD_ST_SUCCESS == u32_err)
        return u32_err;

    ftl_dbg(FTL_DBG_LV_WARN,1,"error: %Xh, retry..\n", u32_err);

    while(u32_PageCnt)
    {
        for(i=0; i<FtlDev.u8_PlaneCnt; i++)
        {
            u32_err = NC_ReadPages(u32_row + FtlDev.u16_BlkPageCnt*i,
                pu8_DataBuf, pu8_SpareBuf, 1);
            if(UNFD_ST_SUCCESS != u32_err)
                break;
			NC_CheckECC(&ecc);
			if(ecc)
			{
				NFTL_RD_Set((u32_row + FtlDev.u16_BlkPageCnt*i) >> FtlDev.u8_BlkPageCntBits,
					(u32_row + FtlDev.u16_BlkPageCnt*i) & FtlDev.u16_BlkPageCntMask);
			}
            pu8_DataBuf += FtlDev.u16_PageByteCnt / FtlDev.u8_PlaneCnt;
            //pu8_SpareBuf +=FtlDev.u16_SpareByteCnt / FtlDev.u8_PlaneCnt; 
        }        
        u32_PageCnt--;
        u32_row++;
    }

    return u32_err;
}


U32 HAL_ReadPages2PCache(U32 u32_PhyRowIdx, U8 *pu8_DataBuf, U8 *pu8_SpareBuf, U32 u32_PageCnt)
{
    U32 u32_err =UNFD_ST_SUCCESS, u32_row, i;
	int ecc;

    u32_row = PhyRowFromMultiPlaneRow(u32_PhyRowIdx);

    #if TEMP_CHECK
    if(RowBlkAddr(u32_row) < FtlDev.u16_StartPBA || 
        RowBlkAddr(u32_row) >= FtlDev.u16_StartPBA+FTL_1PBlkCnt())
    {
        ftl_dbg(0,1,"PRow:%Xh  PBA: %Xh %Xh \n", u32_row,
            FtlDev.u16_StartPBA, FtlDev.u16_StartPBA+FTL_1PBlkCnt());
        ftl_die();
    }
    #endif
    
//    ftl_dbg(FTL_DBG_LV_LOG,1,"%lXh %lXh %lXh %lXh \n",
//        u32_row, (U32)pu8_DataBuf, (U32)pu8_SpareBuf, u32_PageCnt);
//    u32_err = NC_ReadPages2P(u32_row, pu8_DataBuf, pu8_SpareBuf, u32_PageCnt);
	u32_err = NC_ReadPages2PCache(u32_row, pu8_DataBuf, pu8_SpareBuf, u32_PageCnt);    
    if(UNFD_ST_SUCCESS == u32_err)
        return u32_err;

    ftl_dbg(FTL_DBG_LV_WARN,1,"error: %Xh, retry..\n", u32_err);

    while(u32_PageCnt)
    {
        for(i=0; i<FtlDev.u8_PlaneCnt; i++)
        {
            u32_err = NC_ReadPages(u32_row + FtlDev.u16_BlkPageCnt*i, 
                pu8_DataBuf, pu8_SpareBuf, 1);
            if(UNFD_ST_SUCCESS != u32_err)
                break;
			NC_CheckECC(&ecc);
			if(ecc)
				NFTL_RD_Set((u32_row + FtlDev.u16_BlkPageCnt*i) >> FtlDev.u8_BlkPageCntBits,
					(u32_row + FtlDev.u16_BlkPageCnt*i) & FtlDev.u16_BlkPageCntMask);

            pu8_DataBuf += FtlDev.u16_PageByteCnt / FtlDev.u8_PlaneCnt;
            //pu8_SpareBuf +=FtlDev.u16_SpareByteCnt / FtlDev.u8_PlaneCnt; 
        }        
        u32_PageCnt--;
        u32_row++;
    }

    return u32_err;
}


U32 HAL_WritePages(U32 u32_PhyRowIdx, U8 *pu8_DataBuf, U8 *pu8_SpareBuf, U32 u32_PageCnt)
{
    U32 u32_err = UNFD_ST_SUCCESS, tmp;

    //ftl_dbg(FTL_DBG_LV_LOG,1,"%Xh %Xh %Xh %Xh \n", 
    //    u32_PhyRowIdx, (U32)pu8_DataBuf, (U32)pu8_SpareBuf, u32_PageCnt);

    #if TEMP_CHECK
    if(RowBlkAddr(u32_PhyRowIdx) < FtlDev.u16_StartPBA || 
        RowBlkAddr(u32_PhyRowIdx) >= FtlDev.u16_StartPBA+FTL_1PBlkCnt())
    {
        ftl_dbg(FTL_DBG_LV_ERR,1,"PRow:%Xh  PBA: %Xh %Xh \n", u32_PhyRowIdx,
            FtlDev.u16_StartPBA, FtlDev.u16_StartPBA+FTL_1PBlkCnt());
        ftl_die();
    }
    {
		U32 u32_i;
		for(u32_i = 0; u32_i < u32_PageCnt; u32_i ++)
		{
			u32_err = NC_ReadPages(u32_PhyRowIdx + u32_i, au8_DataReadBuf, au8_SpareReadBuf, 1);
			if(nand_CheckEmptyPageFalseAlarm(au8_DataReadBuf, au8_SpareReadBuf) == 0)
			{
				FTL_Disable_ErrLog = 0;
				ftl_dbg(FTL_DBG_LV_ERR, 1, "Write no blank page row %Xh\n", u32_PhyRowIdx + u32_i);
                ftl_die();
				//return UNFD_ST_ERR_W_PROTECT;
			}
		}
	}
    #endif

    if( u32_PageCnt <= FtlDev.u16_FCIEDmaPCntLimit)
    {
        u32_err = NC_WritePages(u32_PhyRowIdx,
                pu8_DataBuf, pu8_SpareBuf, u32_PageCnt);
    }
    else
    {   while(u32_PageCnt)
	    {
            tmp = u32_PageCnt > FtlDev.u16_FCIEDmaPCntLimit ?
                FtlDev.u16_FCIEDmaPCntLimit : u32_PageCnt;
            
            u32_err = NC_WritePages(u32_PhyRowIdx, 
                pu8_DataBuf, pu8_SpareBuf, tmp);
            if(UNFD_ST_SUCCESS != u32_err)
                break;
            
            pu8_DataBuf += FtlDev.u16_PageByteCnt*tmp/FtlDev.u8_PlaneCnt;
	        u32_PageCnt -= tmp;
	        u32_PhyRowIdx += tmp;
	    }
    }

    #if FTL_PROFILE_WAF
    u32_HAL512BCnt += u32_PageCnt*FtlDev.u16_Page512BCnt/FtlDev.u8_PlaneCnt;
    u32_HAL1MBCnt += u32_HAL512BCnt >>11;
    u32_HAL512BCnt -=  (u32_HAL512BCnt >>11) <<11;
    #endif
    
    return u32_err;
}


// CAUTION: not tested
U32 HAL_WritePages2P(U32 u32_PhyRowIdx, U8 *pu8_DataBuf, U8 *pu8_SpareBuf, U32 u32_PageCnt)
{
    U32 u32_err = UNFD_ST_SUCCESS;

    u32_PhyRowIdx = PhyRowFromMultiPlaneRow(u32_PhyRowIdx);
    //ftl_dbg(FTL_DBG_LV_LOG,1,"%Xh %Xh %Xh %Xh \n", 
    //    u32_PhyRowIdx, (U32)pu8_DataBuf, (U32)pu8_SpareBuf, u32_PageCnt);

    #if TEMP_CHECK
    if(RowBlkAddr(u32_PhyRowIdx) < FtlDev.u16_StartPBA || 
        RowBlkAddr(u32_PhyRowIdx) >= FtlDev.u16_StartPBA+FTL_1PBlkCnt())
    {
        ftl_dbg(0,1,"PRow:%Xh  PBA: %Xh %Xh \n", u32_PhyRowIdx,
            FtlDev.u16_StartPBA, FtlDev.u16_StartPBA+FTL_1PBlkCnt());
        ftl_die();
    }
    #endif
	if( ((u32_PageCnt << FtlDev.u8_PageSectorCntBits) * FtlDev.u8_PlaneCnt) <= 4095)
	{
	    u32_err = NC_WritePages2P(u32_PhyRowIdx, pu8_DataBuf, pu8_SpareBuf, u32_PageCnt);
	}
	else
	{
		while(u32_PageCnt)
		{
			u32_err = NC_WritePages2P(u32_PhyRowIdx, pu8_DataBuf, pu8_SpareBuf, 1);
            if(UNFD_ST_SUCCESS != u32_err)
                break;
            pu8_DataBuf += FtlDev.u16_PageByteCnt;
	        u32_PageCnt--;
	        u32_PhyRowIdx++;
		}
	}

    #if FTL_PROFILE_WAF
    u32_HAL512BCnt += u32_PageCnt<<FtlDev.u8_Page512BCntBits;
    u32_HAL1MBCnt += u32_HAL512BCnt >>11;
    u32_HAL512BCnt -=  (u32_HAL512BCnt >>11) <<11;
    #endif
    
    return u32_err;
}

U32 HAL_WritePages2PCache(U32 u32_PhyRowIdx, U8 *pu8_DataBuf, U8 *pu8_SpareBuf, U32 u32_PageCnt)
{
    U32 u32_err = UNFD_ST_SUCCESS, tmp;

    u32_PhyRowIdx = PhyRowFromMultiPlaneRow(u32_PhyRowIdx);
    //ftl_dbg(FTL_DBG_LV_LOG,1,"%lXh %lXh %lXh %lXh \n", 
//      u32_PhyRowIdx, (U32)pu8_DataBuf, (U32)pu8_SpareBuf, u32_PageCnt);

    #if TEMP_CHECK
    if(RowBlkAddr(u32_PhyRowIdx) < FtlDev.u16_StartPBA || 
        RowBlkAddr(u32_PhyRowIdx) >= FtlDev.u16_StartPBA+FTL_1PBlkCnt())
    {
        ftl_dbg(FTL_DBG_LV_ERR,1,"PRow:%Xh  PBA: %Xh %Xh \n", u32_PhyRowIdx,
            FtlDev.u16_StartPBA, FtlDev.u16_StartPBA+FTL_1PBlkCnt());
        ftl_die();
    }
    {
		U32 u32_i;
		for(u32_i = 0; u32_i < u32_PageCnt; u32_i ++)
		{
			u32_err = NC_ReadPages(u32_PhyRowIdx + u32_i, au8_DataReadBuf, au8_SpareReadBuf, 1);
			if(nand_CheckEmptyPageFalseAlarm(au8_DataReadBuf, au8_SpareReadBuf) == 0)
			{
				FTL_Disable_ErrLog = 0;
				ftl_dbg(FTL_DBG_LV_ERR, 1, "Write no blank page row %Xh\n", u32_PhyRowIdx + u32_i);
                ftl_die();
				//return UNFD_ST_ERR_W_PROTECT;
			}
			u32_err = NC_ReadPages(u32_PhyRowIdx + u32_i + FtlDev.u16_BlkPageCnt, au8_DataReadBuf, au8_SpareReadBuf, 1);
			if(nand_CheckEmptyPageFalseAlarm(au8_DataReadBuf, au8_SpareReadBuf) == 0)
			{
				FTL_Disable_ErrLog = 0;
				ftl_dbg(FTL_DBG_LV_ERR, 1, "Write no blank page row %Xh\n", u32_PhyRowIdx + u32_i + FtlDev.u16_BlkPageCnt);
                ftl_die();
				//return UNFD_ST_ERR_W_PROTECT;
			}
		}
	}
	#endif   
	if(u32_PageCnt <= FtlDev.u16_FCIEDmaPCntLimit / FtlDev.u8_PlaneCnt)
	{
	    u32_err = NC_WritePages2PCache(u32_PhyRowIdx, pu8_DataBuf, pu8_SpareBuf, u32_PageCnt);
	}
	else
	{
		while(u32_PageCnt)
		{
            tmp = u32_PageCnt > FtlDev.u16_FCIEDmaPCntLimit/FtlDev.u8_PlaneCnt ?
                FtlDev.u16_FCIEDmaPCntLimit/FtlDev.u8_PlaneCnt : u32_PageCnt;
            
			u32_err = NC_WritePages2P(u32_PhyRowIdx, 
                pu8_DataBuf, pu8_SpareBuf, tmp);
            if(UNFD_ST_SUCCESS != u32_err)
                break;
            
            pu8_DataBuf += FtlDev.u16_PageByteCnt*tmp;
	        u32_PageCnt -= tmp;
	        u32_PhyRowIdx += tmp; // within a same block
		}
	}
	
	#if 0//TEMP_CHECK
    {
		U32 u32_i;
		for(u32_i = 0; u32_i < u32_PageCnt; u32_i ++)
		{
			u32_err = NC_ReadPages(u32_PhyRowIdx + u32_i, au8_DataReadBuf, au8_SpareReadBuf, 1);
			if(memcmp(au8_DataReadBuf, pu8_DataBuf, FtlDev.u16_PageByteCnt/2))
			{
				FTL_Disable_ErrLog = 0;
				ftl_dbg(FTL_DBG_LV_ERR, 1, "read back compare fails row %Xh\n", u32_PhyRowIdx + u32_i);
                ftl_die();
				//return UNFD_ST_ERR_W_PROTECT;
			}
			u32_err = NC_ReadPages(u32_PhyRowIdx + u32_i + FtlDev.u16_BlkPageCnt, au8_DataReadBuf, au8_SpareReadBuf, 1);
			if(memcmp(au8_DataReadBuf, pu8_DataBuf + FtlDev.u16_PageByteCnt/2, FtlDev.u16_PageByteCnt/2))
			{
				FTL_Disable_ErrLog = 0;
				ftl_dbg(FTL_DBG_LV_ERR, 1, "read back compare fails row %Xh\n", u32_PhyRowIdx + u32_i + FtlDev.u16_BlkPageCnt);
                ftl_die();
				//return UNFD_ST_ERR_W_PROTECT;
			}
			pu8_DataBuf =+ FtlDev.u16_PageByteCnt;
		}
	}
#endif	

    #if FTL_PROFILE_WAF
    u32_HAL512BCnt += u32_PageCnt<<FtlDev.u8_Page512BCntBits;
    u32_HAL1MBCnt += u32_HAL512BCnt >>11;
    u32_HAL512BCnt -=  (u32_HAL512BCnt >>11) <<11;
    #endif
    
    return u32_err;
}


U32 HAL_WritePages_HandleBB(U32 u32_PhyRowIdx, U8 *pu8_DataBuf, U8 *pu8_SpareBuf, U32 u32_PageCnt)
{
    U32 u32_err;

    //ftl_dbg(FTL_DBG_LV_LOG,1,"%lXh %lXh %lXh %lXh \n", 
    //    u32_PhyRowIdx, (U32)pu8_DataBuf, (U32)pu8_SpareBuf, u32_PageCnt);

    u32_err = (*FtlDev.pFnWritePages)(u32_PhyRowIdx, 
        pu8_DataBuf, pu8_SpareBuf, u32_PageCnt);

    if(u32_err)
    {
        RBB_SET(RowBlkAddr(u32_PhyRowIdx));
        NFTL_WriteBBT(pu8_FtlRBBT);
    }
    
    return u32_err;
}


U32 HAL_ReadSectors(U32 u32_PhyRowIdx, U8 u8_SectorInPage, U8 *pu8_DataBuf, U8 *pu8_SpareBuf, U32 u32_SectorCnt)
{
    U32 u32_err;
	int ecc;

    #if TEMP_CHECK
    if(RowBlkAddr(u32_PhyRowIdx) < FtlDev.u16_StartPBA || 
        RowBlkAddr(u32_PhyRowIdx) >= FtlDev.u16_StartPBA+FTL_1PBlkCnt())
    {
        ftl_dbg(0,1,"PRow:%Xh  PBA: %Xh %Xh \n", u32_PhyRowIdx,
            FtlDev.u16_StartPBA, FtlDev.u16_StartPBA+FTL_1PBlkCnt());
        ftl_die();
    }
    #endif

	u32_err = NC_ReadSectors_Retry(u32_PhyRowIdx, u8_SectorInPage,
		pu8_DataBuf, pu8_SpareBuf, u32_SectorCnt);

	NC_CheckECC(&ecc);
	if(ecc)
	{
		NFTL_RD_Set(u32_PhyRowIdx >> FtlDev.u8_BlkPageCntBits,
			u32_PhyRowIdx & FtlDev.u16_BlkPageCntMask);
	}
    return u32_err;
}

#if 0
// --------------------------
// ReadSectors can NOT have multi-plane op, 
// or its data format (layout) conflicts with ReadPages' data layout.
// --------------------------
U32 HAL_ReadSectors2P(U32 u32_PhyRowIdx, U8 u8_SectorInPage, U8 *pu8_DataBuf, U8 *pu8_SpareBuf, U32 u32_SectorCnt)
{
    U32 u32_err, u32_SecCntTmp;
    U16 u16_1PlanePageSectorCnt;
    
    //ftl_dbg(FTL_DBG_LV_LOG,1,"%lXh %Xh %lXh\n", 
    //        u32_PhyRowIdx, u8_SectorInPage, u32_SectorCnt);

    u32_PhyRowIdx = PhyRowFromMultiPlaneRow(u32_PhyRowIdx);

    #if TEMP_CHECK
    if(RowBlkAddr(u32_PhyRowIdx) < FtlDev.u16_StartPBA || 
        RowBlkAddr(u32_PhyRowIdx) >= FtlDev.u16_StartPBA+FTL_1PBlkCnt())
    {
        ftl_dbg(0,1,"PRow:%Xh  PBA: %Xh %Xh \n", u32_PhyRowIdx,
            FtlDev.u16_StartPBA, FtlDev.u16_StartPBA+FTL_1PBlkCnt());
        ftl_die();
    }
    #endif

    u16_1PlanePageSectorCnt = FtlDev.u16_PageSectorCnt/FtlDev.u8_PlaneCnt;

    //ftl_dbg(FTL_DBG_LV_LOG,1,"%lXh %Xh %lXh %lXh %lXh\n", 
    //    u32_PhyRowIdx, u8_SectorInPage, (U32)pu8_DataBuf, (U32)pu8_SpareBuf, u32_SectorCnt);

    while(u32_SectorCnt)
    {
        u32_PhyRowIdx += FtlDev.u16_BlkPageCnt * 
            (u8_SectorInPage / u16_1PlanePageSectorCnt);

        if(u8_SectorInPage >= u16_1PlanePageSectorCnt)
            u8_SectorInPage -= u16_1PlanePageSectorCnt;

        u32_SecCntTmp = u16_1PlanePageSectorCnt-u8_SectorInPage;

        u32_SecCntTmp = u32_SecCntTmp > u32_SectorCnt ? 
            u32_SectorCnt : u32_SecCntTmp;

        //ftl_dbg(0,1,"%lXh %Xh %lXh\n", u32_PhyRowIdx, u8_SectorInPage, u32_SecCntTmp);

        u32_err = NC_ReadSectors_Retry(u32_PhyRowIdx, u8_SectorInPage, 
            pu8_DataBuf, pu8_SpareBuf, u32_SecCntTmp);
        if(UNFD_ST_SUCCESS != u32_err)
            return FTL_ERR_R_SECTOR;

        u32_SectorCnt -= u32_SecCntTmp;
        
        if(u32_SectorCnt)
        {
            u8_SectorInPage += u32_SecCntTmp;
            pu8_DataBuf += u32_SecCntTmp << FtlDev.u8_SectorByteCntBits;
            pu8_SpareBuf += u32_SecCntTmp * FtlDev.u16_SectorSpareByteCnt;
        }
    }

    return u32_err;
}
#else
U32 HAL_ReadSectors2P_Retry(U32 u32_PhyRowIdx, U8 u8_SectorInPage, U8 *pu8_DataBuf, U8 *pu8_SpareBuf, U32 u32_SectorCnt)
{
    U32 u32_err, u32_SecCntTmp;
    U16 u16_1PlanePageSectorCnt;
    
//  ftl_dbg(FTL_DBG_LV_LOG,1,"%lXh %Xh %lXh\n",
//          u32_PhyRowIdx, u8_SectorInPage, u32_SectorCnt);

    u32_PhyRowIdx = PhyRowFromMultiPlaneRow(u32_PhyRowIdx);

    #if TEMP_CHECK
    if(RowBlkAddr(u32_PhyRowIdx) < FtlDev.u16_StartPBA || 
        RowBlkAddr(u32_PhyRowIdx) >= FtlDev.u16_StartPBA+FTL_1PBlkCnt())
    {
        ftl_dbg(0,1,"PRow:%Xh  PBA: %Xh %Xh \n", u32_PhyRowIdx,
            FtlDev.u16_StartPBA, FtlDev.u16_StartPBA+FTL_1PBlkCnt());
        ftl_die();
    }
    #endif

    u16_1PlanePageSectorCnt = FtlDev.u16_PageSectorCnt/FtlDev.u8_PlaneCnt;

    //ftl_dbg(FTL_DBG_LV_LOG,1,"%lXh %Xh %lXh %lXh %lXh\n", 
    //    u32_PhyRowIdx, u8_SectorInPage, (U32)pu8_DataBuf, (U32)pu8_SpareBuf, u32_SectorCnt);

    while(u32_SectorCnt)
    {
        u32_PhyRowIdx += FtlDev.u16_BlkPageCnt * 
            (u8_SectorInPage / u16_1PlanePageSectorCnt);

        if(u8_SectorInPage >= u16_1PlanePageSectorCnt)
            u8_SectorInPage -= u16_1PlanePageSectorCnt;

        u32_SecCntTmp = u16_1PlanePageSectorCnt-u8_SectorInPage;

        u32_SecCntTmp = u32_SecCntTmp > u32_SectorCnt ? 
            u32_SectorCnt : u32_SecCntTmp;

//        ftl_dbg(0,1,"%lXh %Xh %lXh\n", u32_PhyRowIdx, u8_SectorInPage, u32_SecCntTmp);

        u32_err = NC_ReadSectors_Retry(u32_PhyRowIdx, u8_SectorInPage, 
            pu8_DataBuf, pu8_SpareBuf, u32_SecCntTmp);
        if(UNFD_ST_SUCCESS != u32_err)
            return FTL_ERR_R_SECTOR;

        u32_SectorCnt -= u32_SecCntTmp;
        
        if(u32_SectorCnt)
        {
            u8_SectorInPage += u32_SecCntTmp;
            pu8_DataBuf += u32_SecCntTmp << FtlDev.u8_SectorByteCntBits;
            pu8_SpareBuf += u32_SecCntTmp * FtlDev.u16_SectorSpareByteCnt;
        }
    }

    return u32_err;
}


U32 HAL_ReadSectors2P(U32 u32_PhyRowIdx, U8 u8_SectorInPage, U8 *pu8_DataBuf, U8 *pu8_SpareBuf, U32 u32_SectorCnt)
{
    U32 u32_PhyRowIdxOri=u32_PhyRowIdx, err=FTL_OK;
    U16 u16_1PlanePageSectorCnt;
    
    u32_PhyRowIdx = PhyRowFromMultiPlaneRow(u32_PhyRowIdx);

    u16_1PlanePageSectorCnt = FtlDev.u16_PageSectorCnt/FtlDev.u8_PlaneCnt;

    #if TEMP_CHECK
    if(RowBlkAddr(u32_PhyRowIdx) < FtlDev.u16_StartPBA || 
        RowBlkAddr(u32_PhyRowIdx) >= FtlDev.u16_StartPBA+FTL_1PBlkCnt())
    {
        ftl_dbg(0,1,"PRow:%Xh  PBA: %Xh %Xh \n", u32_PhyRowIdx,
            FtlDev.u16_StartPBA, FtlDev.u16_StartPBA+FTL_1PBlkCnt());
        ftl_die();
    }
    #endif
    // --------------------------
    if(u8_SectorInPage < u16_1PlanePageSectorCnt && 
        u8_SectorInPage+u32_SectorCnt > u16_1PlanePageSectorCnt)
    {
        err = NC_ReadSectors2P(u32_PhyRowIdx, u8_SectorInPage,
            pu8_DataBuf, pu8_SpareBuf, u32_SectorCnt);

        if(UNFD_ST_SUCCESS != err)
            err = HAL_ReadSectors2P_Retry(u32_PhyRowIdxOri, 
                u8_SectorInPage, pu8_DataBuf, pu8_SpareBuf, u32_SectorCnt);
    }
    else
        err = HAL_ReadSectors2P_Retry(u32_PhyRowIdxOri, 
            u8_SectorInPage, pu8_DataBuf, pu8_SpareBuf, u32_SectorCnt);

    return err;    
}

#endif

#if MSFTL_LINUX
U32 HAL_ReadSendCmd2P(U32 u32_PhyRowIdx, U8 u8_Cmd)
{
    U32 err=FTL_OK;
    U16 u16_1PlanePageSectorCnt;
    
    u32_PhyRowIdx = PhyRowFromMultiPlaneRow(u32_PhyRowIdx);

    u16_1PlanePageSectorCnt = FtlDev.u16_PageSectorCnt/FtlDev.u8_PlaneCnt;

    #if TEMP_CHECK
    if(RowBlkAddr(u32_PhyRowIdx) < FtlDev.u16_StartPBA || 
        RowBlkAddr(u32_PhyRowIdx) >= FtlDev.u16_StartPBA+FTL_1PBlkCnt())
    {
        ftl_dbg(0,1,"PRow:%Xh  PBA: %Xh %Xh \n", u32_PhyRowIdx,
            FtlDev.u16_StartPBA, FtlDev.u16_StartPBA+FTL_1PBlkCnt());
        ftl_die();
    }
    #endif

    // --------------------------

    err = NC_ReadSend2P (u32_PhyRowIdx, u8_Cmd);

    return err;
}

U32 HAL_ReadSendCmd(U32 u32_PhyRowIdx, U8 u8_Cmd)
{
    U32 err=FTL_OK;

    #if TEMP_CHECK
    if(RowBlkAddr(u32_PhyRowIdx) < FtlDev.u16_StartPBA ||
        RowBlkAddr(u32_PhyRowIdx) >= FtlDev.u16_StartPBA+FTL_1PBlkCnt())
    {
        ftl_dbg(0,1,"PRow:%Xh  PBA: %Xh %Xh \n", u32_PhyRowIdx,
            FtlDev.u16_StartPBA, FtlDev.u16_StartPBA+FTL_1PBlkCnt());
        ftl_die();
    }
    #endif

    // --------------------------

    err = NC_ReadSend (u32_PhyRowIdx, u8_Cmd);

    return err;    
}

U32 HAL_SendCmd(U8 u8_Cmd, U8 u8_RBWait)
{
    // --------------------------
    //nand_debug(0, 1, "cmd %Xh\n", u8_Cmd);
    NC_SendCmd(u8_Cmd, u8_RBWait);

    return FTL_OK;    
}

U32 HAL_ReadTransfer(U32 u32_PhyRowIdx, U8 u8_SectorInPage, U8 *pu8_DataBuf, U8 *pu8_SpareBuf, U32 u32_SectorCnt)
{
    U32 u32_err, u32_SecCntTmp;
    U16 u16_1PlanePageSectorCnt;
	int ecc;
    
    //ftl_dbg(FTL_DBG_LV_LOG,1,"%lXh %Xh %lXh\n", 
    //        u32_PhyRowIdx, u8_SectorInPage, u32_SectorCnt);
	u32_PhyRowIdx = PhyRowFromMultiPlaneRow(u32_PhyRowIdx);

    #if TEMP_CHECK
    if(RowBlkAddr(u32_PhyRowIdx) < FtlDev.u16_StartPBA || 
        RowBlkAddr(u32_PhyRowIdx) >= FtlDev.u16_StartPBA+FTL_1PBlkCnt())
    {
        ftl_dbg(0,1,"PRow:%Xh  PBA: %Xh %Xh \n", u32_PhyRowIdx,
            FtlDev.u16_StartPBA, FtlDev.u16_StartPBA+FTL_1PBlkCnt());
        ftl_die();
    }
    #endif

    u16_1PlanePageSectorCnt = FtlDev.u16_PageSectorCnt/FtlDev.u8_PlaneCnt;

//    ftl_dbg(FTL_DBG_LV_LOG,1,"%Xh %Xh %Xh %Xh %Xh\n",
//        u32_PhyRowIdx, u8_SectorInPage, (U32)pu8_DataBuf, (U32)pu8_SpareBuf, u32_SectorCnt);

	if(FtlDev.u8_PlaneCnt > 1)
	{
	    while(u32_SectorCnt)
	    {
	        u32_PhyRowIdx += FtlDev.u16_BlkPageCnt *
	            (u8_SectorInPage / u16_1PlanePageSectorCnt);

	        if(u8_SectorInPage >= u16_1PlanePageSectorCnt)
	            u8_SectorInPage -= u16_1PlanePageSectorCnt;

	        u32_SecCntTmp = u16_1PlanePageSectorCnt-u8_SectorInPage;

	        u32_SecCntTmp = u32_SecCntTmp > u32_SectorCnt ?
	            u32_SectorCnt : u32_SecCntTmp;

	        //ftl_dbg(0,1,"%Xh %Xh %Xh\n", u32_PhyRowIdx, u8_SectorInPage, u32_SecCntTmp);

	        u32_err = NC_ReadSectorsWithOP_Transfer(u32_PhyRowIdx, u8_SectorInPage,
	            pu8_DataBuf, pu8_SpareBuf, u32_SecCntTmp);
	        if(UNFD_ST_SUCCESS != u32_err)
	            return FTL_ERR_R_SECTOR;
			NC_CheckECC(&ecc);
			if(ecc)
			{
				NFTL_RD_Set(u32_PhyRowIdx >> FtlDev.u8_BlkPageCntBits,
					u32_PhyRowIdx & FtlDev.u16_BlkPageCntMask);
			}

	        u32_SectorCnt -= u32_SecCntTmp;

	        if(u32_SectorCnt)
	        {
	            u8_SectorInPage += u32_SecCntTmp;
	            pu8_DataBuf += u32_SecCntTmp << FtlDev.u8_SectorByteCntBits;
	            pu8_SpareBuf += u32_SecCntTmp * FtlDev.u16_SectorSpareByteCnt;
	        }
	    }
	}
	else
	{
        //ftl_dbg(0,1,"%Xh %Xh %Xh\n", u32_PhyRowIdx, u8_SectorInPage, u32_SectorCnt);
        u32_err = NC_ReadSectorsWithOP_Transfer(u32_PhyRowIdx, u8_SectorInPage, 
            pu8_DataBuf, pu8_SpareBuf, u32_SectorCnt);
		NC_CheckECC(&ecc);
		if(ecc)
		{
			NFTL_RD_Set(u32_PhyRowIdx >> FtlDev.u8_BlkPageCntBits,
				u32_PhyRowIdx & FtlDev.u16_BlkPageCntMask);
		}		
        if(UNFD_ST_SUCCESS != u32_err)
            return FTL_ERR_R_SECTOR;
	}
    return u32_err;
}
#endif // MSFTL_LINUX

U32 HAL_WriteSectors(U32 u32_PhyRowIdx, U8 u8_SectorInPage, U8 *pu8_DataBuf, U8 *pu8_SpareBuf, U32 u32_SectorCnt)
{
    U32 u32_err;

    #if TEMP_CHECK
    if(RowBlkAddr(u32_PhyRowIdx) < FtlDev.u16_StartPBA || 
        RowBlkAddr(u32_PhyRowIdx) >= FtlDev.u16_StartPBA+FTL_1PBlkCnt())
    {
        ftl_dbg(FTL_DBG_LV_ERR,1,"PRow:%Xh  PBA: %Xh %Xh \n", u32_PhyRowIdx,
            FtlDev.u16_StartPBA, FtlDev.u16_StartPBA+FTL_1PBlkCnt());
        ftl_die();
    }
    {
		U32 u32_i=0;
		//for(u32_i = 0; u32_i < u32_PageCnt; u32_i ++)
		{
			u32_err = NC_ReadPages(u32_PhyRowIdx + u32_i, au8_DataReadBuf, au8_SpareReadBuf, 1);
			if(0==nand_CheckEmptyPageFalseAlarm(au8_DataReadBuf, au8_SpareReadBuf))
			{
				FTL_Disable_ErrLog = 0;
				ftl_dbg(FTL_DBG_LV_ERR, 1, "Write no blank page row %Xh\n", u32_PhyRowIdx + u32_i);
                ftl_die();
				//return UNFD_ST_ERR_W_PROTECT;
			}
		}
	}
    #endif

    #if defined(FOR_FCIE5_BUG_WS) && FOR_FCIE5_BUG_WS
    u32_err = NC_WritePages(u32_PhyRowIdx,  
        pu8_DataBuf, pu8_SpareBuf, 1);
    #else
    u32_err = NC_WriteSectors(u32_PhyRowIdx, u8_SectorInPage, 
        pu8_DataBuf, pu8_SpareBuf, u32_SectorCnt);
    #endif

    #if FTL_PROFILE_WAF
    u32_HAL512BCnt += u32_SectorCnt*FtlDev.u16_Sector512BCnt/FtlDev.u8_PlaneCnt;
    u32_HAL1MBCnt += u32_HAL512BCnt >>11;
    u32_HAL512BCnt -=  (u32_HAL512BCnt >>11) <<11;
    #endif
    
    return u32_err;
}


// [CAUTION]: currently not used, need to modifiy.
U32 HAL_WriteSectors2P(U32 u32_PhyRowIdx, U8 u8_SectorInPage, U8 *pu8_DataBuf, U8 *pu8_SpareBuf, U32 u32_SectorCnt)
{
    U32 u32_err=FTL_OK, u32_SecCntTmp;

    u32_PhyRowIdx = PhyRowFromMultiPlaneRow(u32_PhyRowIdx);

    #if TEMP_CHECK
    if(RowBlkAddr(u32_PhyRowIdx) < FtlDev.u16_StartPBA || 
        RowBlkAddr(u32_PhyRowIdx) >= FtlDev.u16_StartPBA+FTL_1PBlkCnt())
    {
        ftl_dbg(0,1,"PRow:%Xh  PBA: %Xh %Xh \n", u32_PhyRowIdx,
            FtlDev.u16_StartPBA, FtlDev.u16_StartPBA+FTL_1PBlkCnt());
        ftl_die();
    }
    {
		U32 u32_i=0;
		//for(u32_i = 0; u32_i < u32_PageCnt; u32_i ++)
		{
			u32_err = NC_ReadPages(u32_PhyRowIdx + u32_i, au8_DataReadBuf, au8_SpareReadBuf, 1);
			if(0==nand_CheckEmptyPageFalseAlarm(au8_DataReadBuf, au8_SpareReadBuf))
			{
				FTL_Disable_ErrLog = 0;
				ftl_dbg(FTL_DBG_LV_ERR, 1, "Write no blank page row %Xh\n", u32_PhyRowIdx + u32_i);
                ftl_die();
				//return UNFD_ST_ERR_W_PROTECT;
			}
		}
	}
    #endif

//    ftl_dbg(FTL_DBG_LV_LOG,1,"%lXh %Xh %lXh %lXh %lXh\n", 
//        u32_PhyRowIdx, u8_SectorInPage, (U32)pu8_DataBuf, (U32)pu8_SpareBuf, u32_SectorCnt);

    while(u32_SectorCnt)
    {
        if(u8_SectorInPage < FtlDev.u16_PageSectorCnt/2)
        {
            u32_SecCntTmp = FtlDev.u16_PageSectorCnt/2 - u8_SectorInPage;
            u32_SecCntTmp = u32_SecCntTmp > u32_SectorCnt ?
                u32_SectorCnt : u32_SecCntTmp;
        }
        else
        {   u32_PhyRowIdx += FtlDev.u16_BlkPageCnt;
            u32_SecCntTmp = u32_SectorCnt;
            u8_SectorInPage = 0;
        }
        #if defined(FOR_FCIE5_BUG_WS) && FOR_FCIE5_BUG_WS
        u32_err = NC_WritePages(u32_PhyRowIdx,  
            pu8_DataBuf, pu8_SpareBuf, 1);
        #else
        u32_err = NC_WriteSectors(u32_PhyRowIdx, u8_SectorInPage, 
            pu8_DataBuf, pu8_SpareBuf, u32_SecCntTmp);
        #endif

        u32_SectorCnt -= u32_SecCntTmp;
        if(u32_SectorCnt)
        {
            u8_SectorInPage += u32_SecCntTmp;
            pu8_DataBuf += u32_SecCntTmp << FtlDev.u8_SectorByteCntBits;
            pu8_SpareBuf += u32_SecCntTmp * FtlDev.u16_SectorSpareByteCnt;
        }        
    }    

    #if FTL_PROFILE_WAF
    u32_HAL512BCnt += u32_SectorCnt<<FtlDev.u8_Sector512BCntBits;
    u32_HAL1MBCnt += u32_HAL512BCnt >>11;
    u32_HAL512BCnt -=  (u32_HAL512BCnt >>11) <<11;
    #endif
    
    return u32_err;
}



#if 0==FOR_FCIE5_BUG_CP
// CAUTION: not tested
U32 HAL_PageCopy( // [CAUTION]: did not support Multi-Plane yet.
    U32 u32_SrcPhyRowIdx, U32 u32_DstPhyRowIdx, U8 *pu8_DataBuf, U8 *pu8_SpareBuf, U32 u32_PageCnt
)
{
    U32 u32_err, u32_DonePageCnt;
    U16 FinishPCnt; // [CAUTION]: this API is not verified yet.

    #if TEMP_CHECK
    if(RowBlkAddr(u32_SrcPhyRowIdx) < FtlDev.u16_StartPBA || 
        RowBlkAddr(u32_DstPhyRowIdx) < FtlDev.u16_StartPBA)
        ftl_die();
    #endif

    u32_err = NC_PageCopy(u32_SrcPhyRowIdx, u32_DstPhyRowIdx,
        pu8_DataBuf, pu8_SpareBuf, u32_PageCnt, &u32_DonePageCnt);

    // --------------------------
    // handle read error
    while(UNFD_ST_ERR_ECC_FAIL==u32_err || UNFD_ST_ERR_R_TIMEOUT==u32_err)
    {
        ftl_dbg(FTL_DBG_LV_LOG,1,"warn: NC_PageCopy R Err..\n");

        FinishPCnt = u32_DonePageCnt; // total pages = 1 + RptCnt
        
        #if 0
        else if(0==REG(NC_TR_RPT_CNT) && 0==NFTL_IsFreePage(u32_DstPhyRowIdx))
        {
            ftl_dbg(FTL_DBG_LV_ERR,1,"Dest Blk:%Xh Page:%Xh is not empty.\n",
                RowBlkAddr(u32_DstPhyRowIdx), RowPageIdx(u32_DstPhyRowIdx));
            ftl_die();
        }
        #endif

        pu8_DataBuf += FinishPCnt<<FtlDev.u8_BlkPageCntBits;
        u32_SrcPhyRowIdx += FinishPCnt;
        u32_DstPhyRowIdx += FinishPCnt;
                    
        u32_err = NC_ReadPages(u32_SrcPhyRowIdx,
            pu8_DataBuf, pu8_SpareBuf, 1);
        if(u32_err)
        {
            ftl_dbg(FTL_DBG_LV_ERR,1,"ReadPages fail: %Xh\n", u32_err);
            ftl_die();
        }
        u32_err = NC_WritePages(u32_DstPhyRowIdx,
            pu8_DataBuf, pu8_SpareBuf, 1);
        if(u32_err)
        {
            ftl_dbg(FTL_DBG_LV_ERR,1,"WritePages fail: %Xh\n", u32_err);
            return u32_err; // w error left for FTL core
        }

        u32_PageCnt -= (++FinishPCnt);
        pu8_DataBuf += 1<<FtlDev.u8_BlkPageCntBits;
        u32_SrcPhyRowIdx += 1;
        u32_DstPhyRowIdx += 1;
        
        u32_err = NC_PageCopy(u32_SrcPhyRowIdx, u32_DstPhyRowIdx,
            pu8_DataBuf, pu8_SpareBuf, u32_PageCnt,&u32_DonePageCnt);
    }

    return u32_err;
}
#else
// caller has to handle possible BB.
// CAUTION: not tested
U32 HAL_PageCopy( // support Multi-Plane
    U32 u32_SrcPhyRowIdx, U32 u32_DstPhyRowIdx, U8 *pu8_DataBuf, U8 *pu8_SpareBuf, U32 u32_PageCnt
)
{
    U32 u32_err=FTL_OK, u32_i=0, u32_tmp, u32_cnt;

    u32_SrcPhyRowIdx = PhyRowFromMultiPlaneRow(u32_SrcPhyRowIdx);
    u32_DstPhyRowIdx = PhyRowFromMultiPlaneRow(u32_DstPhyRowIdx);

    #if TEMP_CHECK
    if(RowBlkAddr(u32_SrcPhyRowIdx) < FtlDev.u16_StartPBA || 
        RowBlkAddr(u32_SrcPhyRowIdx) >= FtlDev.u16_StartPBA+FTL_1PBlkCnt())
    {
        ftl_dbg(0,1,"SrcPRow:%Xh  PBA: %Xh %Xh \n", u32_SrcPhyRowIdx,
            FtlDev.u16_StartPBA, FtlDev.u16_StartPBA+FTL_1PBlkCnt());
        ftl_die();
    }
    if(RowBlkAddr(u32_DstPhyRowIdx) < FtlDev.u16_StartPBA || 
        RowBlkAddr(u32_DstPhyRowIdx) >= FtlDev.u16_StartPBA+FTL_1PBlkCnt())
    {
        ftl_dbg(0,1,"DestPRow:%Xh  PBA: %Xh %Xh \n", u32_DstPhyRowIdx,
            FtlDev.u16_StartPBA, FtlDev.u16_StartPBA+FTL_1PBlkCnt());
        ftl_die();
    }
    #endif

    while(u32_i<u32_PageCnt)
    {
        u32_tmp = u32_PageCnt-u32_i;
        u32_err = (*FtlDev.pFnNC_PageCopy)(
            u32_SrcPhyRowIdx, u32_DstPhyRowIdx, pu8_DataBuf, pu8_SpareBuf, u32_tmp, &u32_cnt);

        if(u32_err==UNFD_ST_ERR_W_FAIL || u32_err==UNFD_ST_ERR_W_BUSY ||
            u32_err==UNFD_ST_ERR_W_PROTECTED)
        {
            ftl_dbg(FTL_DBG_LV_ERR, 1, "NC_PageCopy2P fail: %Xh\n", u32_err);
            return u32_err;
        }
        //ftl_dbg(0,1,"Src:%lXh Dst:%lXh Err:%lXh PCnt:%lXh i:%lXh tmp:%lXh cnt:%lXh \n", 
        //    u32_SrcPhyRowIdx, u32_DstPhyRowIdx, u32_err, u32_PageCnt, u32_i, u32_tmp, u32_cnt);
        u32_i += u32_cnt;
        u32_SrcPhyRowIdx += u32_cnt;
        u32_DstPhyRowIdx += u32_cnt;
            
        #if FTL_PROFILE_WAF
        u32_HAL512BCnt += u32_cnt<<FtlDev.u8_Page512BCntBits;
        u32_HAL1MBCnt += u32_HAL512BCnt >>11;
        u32_HAL512BCnt -=  (u32_HAL512BCnt >>11) <<11;
        #endif
        
        if(u32_cnt != u32_tmp)
        {
            u32_err = HAL_PageCopy_P2P(u32_SrcPhyRowIdx, u32_DstPhyRowIdx, 
                pu8_DataBuf, pu8_SpareBuf, 1);
            if(UNFD_ST_SUCCESS != u32_err)
            {
                ftl_dbg(FTL_DBG_LV_ERR, 1, "HAL_PageCopy_P2P fail: %Xh\n", u32_err);
                return u32_err;
            }
            u32_SrcPhyRowIdx += 1;
            u32_DstPhyRowIdx += 1;
            u32_i++;
        }
    }  
    
    return FTL_OK;
}
#endif
U32 HAL_PageCopy_P2P( // page to page
    U32 u32_SrcPhyRowIdx, U32 u32_DstPhyRowIdx, U8 *pu8_DataBuf, U8 *pu8_SpareBuf, U32 u32_PageCnt
)
{
	U32 u32_err=FTL_OK, u32_i;

	ftl_dbg(0,1,"Src:%Xh Dst:%Xh Err:%Xh PCnt:%Xh \n", 
		u32_SrcPhyRowIdx, u32_DstPhyRowIdx, u32_err, u32_PageCnt);

	for(u32_i=0; u32_i<u32_PageCnt; u32_i++)
	{
		u32_err = (*FtlDev.pFnReadPages)(u32_SrcPhyRowIdx+u32_i, 
			pu8_DataBuf, pu8_SpareBuf, 1);
		if(u32_err)
		{
			ftl_dbg(FTL_DBG_LV_ERR,1,"ReadPages %Xh fail: %Xh \n",
				u32_SrcPhyRowIdx+u32_i, u32_err);
			ftl_die();
		}

		u32_err = (*FtlDev.pFnWritePages)(u32_DstPhyRowIdx+u32_i,
			pu8_DataBuf, pu8_SpareBuf, 1);
		if(u32_err)
		{
			ftl_dbg(FTL_DBG_LV_ERR,1,"WritePages %Xh fail: %Xh\n", 
				u32_DstPhyRowIdx+u32_i, u32_err);
			return u32_err; // w error left for FTL core
		}
	}	 
	return u32_err;
}


U32 HAL_EraseBlk(U16 u16_PBA)
{
    U32 u32_err;

    #if TEMP_CHECK
    if(u16_PBA < FtlDev.u16_StartPBA || 
        u16_PBA >= FtlDev.u16_StartPBA+FTL_1PBlkCnt())
    {
        ftl_dbg(FTL_DBG_LV_ERR,1,"PBA:%Xh, %Xh %Xh \n", u16_PBA,
            FtlDev.u16_StartPBA, FtlDev.u16_StartPBA+FTL_1PBlkCnt());
        ftl_die();
    }
    #endif

    // ftl_dbg(0,1,"%Xh \n", u16_PBA);
    u32_err = NC_EraseBlk(BlkRowAddr(u16_PBA));
    
    if(UNFD_ST_SUCCESS == u32_err)
    {   // handle ECnt
        if(pu16_PBAECnt)
        {
            SET_ECNT(u16_PBA);    FtlCtrl.u32_AccuECnt++;
        }

        if(pu8_IfBlkKept)
            CLR_PBA_KEPT(u16_PBA);
    }
    return u32_err;
}

U32 HAL_EraseBlk2P(U16 u16_PBA)
{
    U32 u32_err;

    #if TEMP_CHECK
    if(u16_PBA < FtlDev.u16_StartPBA || 
        u16_PBA >= FtlDev.u16_StartPBA+FTL_1PBlkCnt())
    {
        ftl_dbg(FTL_DBG_LV_ERR,1,"PBA:%Xh, %Xh %Xh \n", u16_PBA,
            FtlDev.u16_StartPBA, FtlDev.u16_StartPBA+FTL_1PBlkCnt());
        ftl_die();
    }
    #endif

    // --------------------------
    // handle ECnt
    if(pu16_PBAECnt)
    {
        SET_ECNT(u16_PBA);    FtlCtrl.u32_AccuECnt++;
    }
    
    // don't care when called by RootBlk or Test APIs 
    if(pu8_IfBlkKept)
        CLR_PBA_KEPT(u16_PBA);

    // --------------------------
    u16_PBA = PhyPBAFromMultiPlanePBA(u16_PBA);

    #if TEMP_CHECK
    if(u16_PBA < FtlDev.u16_StartPBA || 
        u16_PBA >= FtlDev.u16_StartPBA+FTL_1PBlkCnt())
    {
        ftl_dbg(0,1,"PBA:%Xh, %Xh %Xh \n", u16_PBA,
            FtlDev.u16_StartPBA, FtlDev.u16_StartPBA+FTL_1PBlkCnt());
        ftl_die();
    }
    #endif
    
    //ftl_dbg(0,1,"%Xh \n", u16_PBA);    
    u32_err = NC_EraseBlk2P(BlkRowAddr(u16_PBA));

    return u32_err;
}


U32 HAL_EraseBlk_HandleBB(U16 u16_PBA)
{
    U32 u32_err;

    u32_err = (*FtlDev.pFnEraseBlk)(u16_PBA);
    if(u32_err)
    {
        RBB_SET(u16_PBA);
        NFTL_WriteBBT(pu8_FtlRBBT);
    }
    return u32_err;
}


U32 HAL_EraseBlk_HandleBB_SetFB(U16 u16_PBA)
{
    U32 u32_err;

    u32_err = HAL_EraseBlk_HandleBB(u16_PBA);
    
    if(FTL_OK == u32_err)
        NFTL_SetFB(u16_PBA);
    
    return u32_err;
}



