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
#include "ftlsd_api.h"

FTLSd_DEVICE FtlSdDev;

// for temporarily operations:
//   - pass to FTLSd_ APIs, and then used immediately.
//   - CAUTION: used by FTLSd_IsFreeBlk, FTLSd_IsFreePage.
U8 *pu8_FtlSdPDatBuf=NULL, *pu8_FtlSdPRddBuf=NULL;

extern U8 *pu8_FtlSdApiBuf;
extern U8 *pFtlSdUtlSectorBuf;
extern U8 *pu8_FtlSdDatBuf;

volatile U32 u32_FtlSdMemUsageByte;

#if TEMP_CHECK
U8 au8_DataReadBuf[16384];
U8 au8_SpareReadBuf[2000];
extern int nand_CheckEmptyPageFalseAlarm(U8 *main, U8 *spare);
#endif

void FTLSd_free(void)
{
}

static void *FTLSd_malloc(void)
{
    // --------------------------
    // allocate page buf
    if(0==FtlSdDev.u16_PageByteCnt)
    {
        ftl_dbg(FTLSd_DBG_LV_ERR,1,"abnormal PageByteCnt: %Xh \n", FtlSdDev.u16_PageByteCnt);
        return NULL;
    }
    if(0==FtlSdDev.u16_SpareByteCnt)
    {
        ftl_dbg(FTLSd_DBG_LV_ERR,1,"abnormal SpareByteCnt: %Xh \n", FtlSdDev.u16_SpareByteCnt);
        return NULL;
    }

    // --------------------------
    if(NULL == pu8_FtlSdPDatBuf)
    {
		#if MSFTLSd_LINUX && defined(CONFIG_MIPS)
		pu8_FtlSdPDatBuf = FDMAMALLOC(FtlSdDev.u16_PageByteCnt);
		#else
        pu8_FtlSdPDatBuf = FSdMALLOC(FtlSdDev.u16_PageByteCnt);
		#endif
        u32_FtlSdMemUsageByte += FtlSdDev.u16_PageByteCnt;
    }
    
    if(NULL == pu8_FtlSdPRddBuf)
    {
		#if MSFTLSd_LINUX && defined(CONFIG_MIPS)
		pu8_FtlSdPRddBuf = FDMAMALLOC(FtlSdDev.u16_SpareByteCnt);
		#else
        pu8_FtlSdPRddBuf = FSdMALLOC(FtlSdDev.u16_SpareByteCnt);
		#endif
        u32_FtlSdMemUsageByte += FtlSdDev.u16_SpareByteCnt;
    }

    // --------------------------
    if(NULL == pu8_FtlSdApiBuf) // in FtlSd_api.c
    {
        pu8_FtlSdApiBuf = (U8*)FSdMALLOC(FtlSdDev.u16_PageByteCnt);
        u32_FtlSdMemUsageByte += FtlSdDev.u16_PageByteCnt;
    }

    // --------------------------
    if(NULL == pu8_FtlSdDatBuf) // in FtlSd_data.c
    {
        pu8_FtlSdDatBuf = (U8*)FSdMALLOC(FtlSdDev.u16_PageByteCnt);
        u32_FtlSdMemUsageByte += FtlSdDev.u16_PageByteCnt;
    }

    // --------------------------
    if(NULL == pFtlSdUtlSectorBuf) // in FtlSd_utl.c
    {
		#if MSFTLSd_LINUX && defined(CONFIG_MIPS)
        pFtlSdUtlSectorBuf = (U8*)FDMAMALLOC(FtlSdDev.u16_SectorByteCnt);
		#else
        pFtlSdUtlSectorBuf = (U8*)FSdMALLOC(FtlSdDev.u16_SectorByteCnt);
		#endif
        u32_FtlSdMemUsageByte += FtlSdDev.u16_SectorByteCnt;
    }
    
    return pu8_FtlSdPDatBuf;
}


static U32 FTLSd_CheckDevLimit(void)
{
    #if 0
    NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();
    // leave the end of NAND for MTD BBT
    if(FtlSdDev.u16_StartPBA + FTLSd_1PBlkCnt() > pNandDrv->u16_BlkCnt-10)
    {
        ftl_dbg(FTLSd_DBG_LV_ERR,1,"StartBlk + BlkCnt = %Xh > NAND total BlkCnt: %Xh - 10 \n", 
            FtlSdDev.u16_StartPBA + FtlSdDev.u16_PBACnt, pNandDrv->u16_BlkCnt);
        return FTLSd_ERR_OUT_OF_RANGE;
    }
    #endif
    // check if touch CIS reserced areas
    if(FtlSdDev.u16_StartPBA < 10)
    {
        ftl_dbg(FTLSd_DBG_LV_ERR,1,"abnormal StartPBA: %Xh < 10 \n", FtlSdDev.u16_StartPBA);
        return FTLSd_ERR_OUT_OF_RANGE;
    }
    // check basic FTL PBA count
    if(FtlSdDev.u16_PBACnt > MAX_FTLSd_BLKCNT)
    {
        ftl_dbg(FTLSd_DBG_LV_ERR,1,"abnormal PBACnt:%Xh, max is: %Xh \n", 
            FtlSdDev.u16_PBACnt, MAX_FTLSd_BLKCNT);
        return FTLSd_ERR_PART_PARAM;
    }
    // check NAND parameters
    if( FtlSdDev.u16_PageByteCnt > MAX_PDAT_BYTECNT ||
        FtlSdDev.u16_SpareByteCnt > MAX_PRDD_BYTECNT||
        FtlSdDev.u16_BlkPageCnt > MAX_BLK_PAGECNT)
    {
        ftl_dbg(FTLSd_DBG_LV_ERR,1,
            "abnormal BlkPageCnt: %Xh, PageByteCnt: %Xh, SpareByteCnt: %Xh\n", 
            FtlSdDev.u16_BlkPageCnt, FtlSdDev.u16_PageByteCnt, FtlSdDev.u16_SpareByteCnt);
        return FTLSd_ERR_NAND_PARAM;
    }
    
    return FTLSd_OK;
}


static U32 FTLSd_SyncNNIParam_Ex(void)
{    
    FtlSdDev.u8_BlkPageCntBits = drvNAND_CountBits(FtlSdDev.u16_BlkPageCnt);
    FtlSdDev.u8_PageByteCntBits = drvNAND_CountBits(FtlSdDev.u16_PageByteCnt);
    FtlSdDev.u8_SectorByteCntBits = drvNAND_CountBits(FtlSdDev.u16_SectorByteCnt);

    FtlSdDev.u16_PageSectorCnt = FtlSdDev.u16_PageByteCnt >> FtlSdDev.u8_SectorByteCntBits;
    FtlSdDev.u16_BlkSectorCnt = FtlSdDev.u16_PageSectorCnt << FtlSdDev.u8_BlkPageCntBits;
    FtlSdDev.u8_PageSectorCntBits = drvNAND_CountBits(FtlSdDev.u16_PageSectorCnt);
    FtlSdDev.u8_BlkSectorCntBits = drvNAND_CountBits(FtlSdDev.u16_BlkSectorCnt);
        
    FtlSdDev.u16_SectorSpareByteCnt = FtlSdDev.u16_SpareByteCnt >> FtlSdDev.u8_PageSectorCntBits;
    
    FtlSdDev.u16_BlkPageCntMask = FtlSdDev.u16_BlkPageCnt-1;
    FtlSdDev.u16_PageByteCntMask = FtlSdDev.u16_PageByteCnt-1;
    FtlSdDev.u16_SectorByteCntMask = FtlSdDev.u16_SectorByteCnt-1;
    
    FtlSdDev.u16_PageSectorCntMask = FtlSdDev.u16_PageSectorCnt-1;
    FtlSdDev.u16_BlkSectorCntMask = FtlSdDev.u16_BlkSectorCnt-1;
    
    // --------------------------
    FtlSdDev.u16_Blk512BCnt = FtlSdDev.u16_BlkPageCnt<<(FtlSdDev.u8_PageByteCntBits-9);
    FtlSdDev.u16_Blk512BCntMask = FtlSdDev.u16_Blk512BCnt - 1;
    FtlSdDev.u8_Blk512BCntBits = drvNAND_CountBits(FtlSdDev.u16_Blk512BCnt);
    FtlSdDev.u16_Page512BCnt = FtlSdDev.u16_PageByteCnt >> 9;
    FtlSdDev.u16_Page512BCntMask = FtlSdDev.u16_Page512BCnt - 1;
    FtlSdDev.u8_Page512BCntBits = drvNAND_CountBits(FtlSdDev.u16_Page512BCnt);
    FtlSdDev.u16_Sector512BCnt = FtlSdDev.u16_SectorByteCnt >> 9;
    FtlSdDev.u16_Sector512BCntMask = FtlSdDev.u16_Sector512BCnt - 1;
    FtlSdDev.u8_Sector512BCntBits = drvNAND_CountBits(FtlSdDev.u16_Sector512BCnt);
     
    return FTLSd_OK;      
}


U32 FTLSd_SyncNNIParam(void)
{
    NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();

    FtlSdDev.u16_BlkPageCnt = pNandDrv->u16_BlkPageCnt;
    FtlSdDev.u16_PageByteCnt = pNandDrv->u16_PageByteCnt;
    FtlSdDev.u16_SectorByteCnt = pNandDrv->u16_SectorByteCnt;
    FtlSdDev.u16_SpareByteCnt = pNandDrv->u16_SpareByteCnt;
    FtlSdDev.u16_ECCCodeByteCnt = pNandDrv->u16_ECCCodeByteCnt;
    
    FtlSdDev.u8_CellType = pNandDrv->u8_CellType;
    FtlSdDev.u8_RequireRandomizer = pNandDrv->u8_RequireRandomizer;
    FtlSdDev.u16_BlkLowPCnt = pNandDrv->u16_BlkLowPCnt;

    FtlSdDev.u8_IDByteCnt = pNandDrv->u8_IDByteCnt;
    memcpy(FtlSdDev.au8_ID, pNandDrv->au8_ID, FtlSdDev.u8_IDByteCnt);

    // --------------------------
    FTLSd_CheckMultiPlane();

    // --------------------------
    FTLSd_SyncNNIParam_Ex();

    return FTLSd_OK;
}


U32 FTLSd_Hal_Init(U16 u16_StartPBA, U16 u16_PBACnt)
{
    U32 u32_err;

    memset(&FtlSdDev, 0, sizeof(FTLSd_DEVICE));

    if(u16_StartPBA ==0 || u16_PBACnt == 0)
    {
        ftl_dbg(FTLSd_DBG_LV_WARN,1,"PNI has no FTL partition, "
            "set range by parameters: %Xh~%Xh, %u blocks. \n",
            u16_StartPBA, u16_StartPBA+u16_PBACnt, u16_PBACnt);
        return FTLSd_ERR_FATAL;
    }
    else
    {
        FtlSdDev.u16_StartPBA = u16_StartPBA;
        FtlSdDev.u16_PBACnt = u16_PBACnt;        
    }

    // init multi-plane inside (PBACnt)
    u32_err = FTLSd_SyncNNIParam();
    if(FTLSd_OK != u32_err)
        return u32_err;

    u32_err = FTLSd_CheckDevLimit();
    if(FTLSd_OK != u32_err)
        return u32_err;

    ftl_dbg(FTLSd_DBG_LV_LOG,0,"[FTLSd] StartPBA: %Xh, PBACnt: %Xh, %u.%u MB \n",
        FtlSdDev.u16_StartPBA, FtlSdDev.u16_PBACnt, 
        BlkCntToMB(FTLSd_1PBlkCnt())/FtlSdDev.u8_PlaneCnt, 
        (BlkCntToMB(FTLSd_1PBlkCnt()*10)/FtlSdDev.u8_PlaneCnt)%10);

    #if !(defined(NC_SEL_FCIE5) && NC_SEL_FCIE5)
    FtlSdDev.u16_FCIEDmaPCntLimit = 4095 >>  FtlSdDev.u8_PageSectorCntBits;
    #else
    FtlSdDev.u16_FCIEDmaPCntLimit = 0xFFF0; // no limited
    #endif
    
    if(NULL == FTLSd_malloc()) // <- BlkLowPCnt init inside
    {
        ftl_dbg(FTLSd_DBG_LV_ERR,1,"get mem fail !\n");
        return FTLSd_ERR_GET_MEM_FAIL;
    }

    UpdateDevStructChkSum();
    return FTLSd_OK;
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
//             - can't call FTLSd_HAL itself (to avoid re-entry).
//       - read fail: 
//          - assert since NC_HAL already retry, FTL does not need to check returned ok or not.
// ============================================
U32 FTLSd_ReadPages(U32 u32_PhyRowIdx, U8 *pu8_DataBuf, U8 *pu8_SpareBuf, U32 u32_PageCnt)
{
    U32 u32_err = UNFD_ST_SUCCESS, tmp;
	int ecc =0;
    #if defined(MSFTLSd_SBOOT) && MSFTLSd_SBOOT
    NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
    #endif

    #if TEMP_CHECK
    if(RowBlkAddr(u32_PhyRowIdx) < FtlSdDev.u16_StartPBA || 
        RowBlkAddr(u32_PhyRowIdx) >= FtlSdDev.u16_StartPBA+FTLSd_1PBlkCnt())
    {
        ftl_dbg(0,1,"PRow:%Xh  PBA: %Xh %Xh \n", u32_PhyRowIdx,
            FtlSdDev.u16_StartPBA, FtlSdDev.u16_StartPBA+FTLSd_1PBlkCnt());
        ftl_die();
    }
    #endif

    //ftl_dbg(FTLSd_DBG_LV_LOG,1,"%Xh %Xh %Xh %Xh +\n",
    //    u32_PhyRowIdx, (U32)pu8_DataBuf, (U32)pu8_SpareBuf, u32_PageCnt);
	if( u32_PageCnt <= FtlSdDev.u16_FCIEDmaPCntLimit)
	{
        #if defined(MSFTLSd_SBOOT) && MSFTLSd_SBOOT
        u32_err = NC_ReadPages(pNandDrv, u32_PhyRowIdx, pu8_DataBuf, pu8_SpareBuf, u32_PageCnt);
        #else
	    u32_err = NC_ReadPages(u32_PhyRowIdx, pu8_DataBuf, pu8_SpareBuf, u32_PageCnt);
        #endif
	}
	else
	{
	    while(u32_PageCnt)
	    {
            tmp = u32_PageCnt > FtlSdDev.u16_FCIEDmaPCntLimit ?
                FtlSdDev.u16_FCIEDmaPCntLimit : u32_PageCnt;

            #if defined(MSFTLSd_SBOOT) && MSFTLSd_SBOOT
            u32_err = NC_ReadPages(pNandDrv, u32_PhyRowIdx, 
                pu8_DataBuf, pu8_SpareBuf, tmp);
            #else
            u32_err = NC_ReadPages(u32_PhyRowIdx, 
                pu8_DataBuf, pu8_SpareBuf, tmp);
            #endif
            if(UNFD_ST_SUCCESS != u32_err)
                break;
            
			NC_CheckECC(&ecc); // CAUTION: not precisely
			if(ecc)
			{
				//FTLSd_RD_Set(RowBlkAddr(u32_PhyRowIdx),
				//	u32_PhyRowIdx & FtlSdDev.u16_BlkPageCntMask);
			}
            
            pu8_DataBuf += FtlSdDev.u16_PageByteCnt*tmp/FtlSdDev.u8_PlaneCnt;
	        u32_PageCnt -= tmp;
	        u32_PhyRowIdx += tmp;
	    }
	}
    if(UNFD_ST_SUCCESS == u32_err)
        return u32_err;

    ftl_dbg(FTLSd_DBG_LV_WARN,1,"error: %Xh, retry..\n", u32_err);

    while(u32_PageCnt)
    {
        #if defined(MSFTLSd_SBOOT) && MSFTLSd_SBOOT
        u32_err = NC_ReadPages(pNandDrv, u32_PhyRowIdx, 
            pu8_DataBuf, pu8_SpareBuf, 1);
        #else
        u32_err = NC_ReadPages(u32_PhyRowIdx, 
            pu8_DataBuf, pu8_SpareBuf, 1);
        #endif
        if(UNFD_ST_SUCCESS != u32_err)
            break;
		NC_CheckECC(&ecc);
		if(ecc)
		{
			//FTLSd_RD_Set(RowBlkAddr(u32_PhyRowIdx),
			//    u32_PhyRowIdx & FtlSdDev.u16_BlkPageCntMask);
		}
        pu8_DataBuf += FtlSdDev.u16_PageByteCnt/FtlSdDev.u8_PlaneCnt;
        //pu8_SpareBuf +=FtlSdDev.u16_SpareByteCnt / FtlSdDev.u8_PlaneCnt; 
        u32_PageCnt--;
        u32_PhyRowIdx++;
    }

    return u32_err;
}



U32 FTLSd_WritePages(U32 u32_PhyRowIdx, U8 *pu8_DataBuf, U8 *pu8_SpareBuf, U32 u32_PageCnt)
{
    U32 u32_err = UNFD_ST_SUCCESS, tmp;    
    #if defined(MSFTLSd_SBOOT) && MSFTLSd_SBOOT
    NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
    #endif

    //ftl_dbg(FTLSd_DBG_LV_LOG,1,"%Xh %Xh %Xh %Xh \n", 
    //    u32_PhyRowIdx, (U32)pu8_DataBuf, (U32)pu8_SpareBuf, u32_PageCnt);

    #if TEMP_CHECK
    if(RowBlkAddr(u32_PhyRowIdx) < FtlSdDev.u16_StartPBA || 
        RowBlkAddr(u32_PhyRowIdx) >= FtlSdDev.u16_StartPBA+FTLSd_1PBlkCnt())
    {
        ftl_dbg(FTLSd_DBG_LV_ERR,1,"PRow:%Xh  PBA: %Xh %Xh \n", u32_PhyRowIdx,
            FtlSdDev.u16_StartPBA, FtlSdDev.u16_StartPBA+FTLSd_1PBlkCnt());
        ftl_die();
    }
    {
		U32 u32_i;
		for(u32_i = 0; u32_i < u32_PageCnt; u32_i ++)
		{
            #if defined(MSFTLSd_SBOOT) && MSFTLSd_SBOOT
			u32_err = NC_ReadPages(pNandDrv, u32_PhyRowIdx + u32_i, au8_DataReadBuf, au8_SpareReadBuf, 1);
            #else
            u32_err = NC_ReadPages(u32_PhyRowIdx + u32_i, au8_DataReadBuf, au8_SpareReadBuf, 1);
            #endif
			if(nand_CheckEmptyPageFalseAlarm(au8_DataReadBuf, au8_SpareReadBuf) == 0)
			{
				FTLSd_Disable_ErrLog = 0;
				ftl_dbg(FTLSd_DBG_LV_ERR, 1, "Write no blank page row %Xh\n", u32_PhyRowIdx + u32_i);
                ftl_die();
				//return UNFD_ST_ERR_W_PROTECT;
			}
		}
	}
    #endif

    if( u32_PageCnt <= FtlSdDev.u16_FCIEDmaPCntLimit)
    {
        u32_err = NC_WritePages(u32_PhyRowIdx,
                pu8_DataBuf, pu8_SpareBuf, u32_PageCnt);
    }
    else
    {   while(u32_PageCnt)
	    {
            tmp = u32_PageCnt > FtlSdDev.u16_FCIEDmaPCntLimit ?
                FtlSdDev.u16_FCIEDmaPCntLimit : u32_PageCnt;
            
            u32_err = NC_WritePages(u32_PhyRowIdx, 
                pu8_DataBuf, pu8_SpareBuf, tmp);
            if(UNFD_ST_SUCCESS != u32_err)
                break;
            
            pu8_DataBuf += FtlSdDev.u16_PageByteCnt*tmp/FtlSdDev.u8_PlaneCnt;
	        u32_PageCnt -= tmp;
	        u32_PhyRowIdx += tmp;
	    }
    }
    
    return u32_err;
}


U32 FTLSd_WritePages_HandleBB(U32 u32_PhyRowIdx, U8 *pu8_DataBuf, U8 *pu8_SpareBuf, U32 u32_PageCnt)
{
    U32 u32_err;

    //ftl_dbg(FTLSd_DBG_LV_LOG,1,"%lXh %lXh %lXh %lXh \n", 
    //    u32_PhyRowIdx, (U32)pu8_DataBuf, (U32)pu8_SpareBuf, u32_PageCnt);

    u32_err = (*FtlSdDev.pFnWritePages)(u32_PhyRowIdx, 
        pu8_DataBuf, pu8_SpareBuf, u32_PageCnt);

    if(u32_err)
    {
        BB_SET(RowBlkAddr(u32_PhyRowIdx));
    }
    
    return u32_err;
}


U32 FTLSd_ReadSectors(U32 u32_PhyRowIdx, U8 u8_SectorInPage, U8 *pu8_DataBuf, U8 *pu8_SpareBuf, U32 u32_SectorCnt)
{
    U32 u32_err;
	int ecc;
    #if defined(MSFTLSd_SBOOT) && MSFTLSd_SBOOT
    U32 i;
    NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
    #endif

    #if TEMP_CHECK
    if(RowBlkAddr(u32_PhyRowIdx) < FtlSdDev.u16_StartPBA || 
        RowBlkAddr(u32_PhyRowIdx) >= FtlSdDev.u16_StartPBA+FTLSd_1PBlkCnt())
    {
        ftl_dbg(0,1,"PRow:%Xh  PBA: %Xh %Xh \n", u32_PhyRowIdx,
            FtlSdDev.u16_StartPBA, FtlSdDev.u16_StartPBA+FTLSd_1PBlkCnt());
        ftl_die();
    }
    #endif

    #if defined(MSFTLSd_SBOOT) && MSFTLSd_SBOOT
    for(i=0; i<u32_SectorCnt; i++)
    {        
        u32_err = NC_ReadSector_DMAMode(pNandDrv, u32_PhyRowIdx, u8_SectorInPage+i,
		    pu8_DataBuf + (i<<FtlSdDev.u8_SectorByteCntBits), pu8_SpareBuf);
    }
    #else    
	u32_err = NC_ReadSectors_Retry(u32_PhyRowIdx, u8_SectorInPage,
		pu8_DataBuf, pu8_SpareBuf, u32_SectorCnt);
    #endif

	NC_CheckECC(&ecc);
	if(ecc)
	{
		//FTLSd_RD_Set(u32_PhyRowIdx >> FtlSdDev.u8_BlkPageCntBits,
		//	u32_PhyRowIdx & FtlSdDev.u16_BlkPageCntMask);
	}
    return u32_err;
}



U32 FTLSd_WriteSectors(U32 u32_PhyRowIdx, U8 u8_SectorInPage, U8 *pu8_DataBuf, U8 *pu8_SpareBuf, U32 u32_SectorCnt)
{
    U32 u32_err;
    #if defined(MSFTLSd_SBOOT) && MSFTLSd_SBOOT
    NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
    #endif

    #if TEMP_CHECK
    if(RowBlkAddr(u32_PhyRowIdx) < FtlSdDev.u16_StartPBA || 
        RowBlkAddr(u32_PhyRowIdx) >= FtlSdDev.u16_StartPBA+FTLSd_1PBlkCnt())
    {
        ftl_dbg(FTLSd_DBG_LV_ERR,1,"PRow:%Xh  PBA: %Xh %Xh \n", u32_PhyRowIdx,
            FtlSdDev.u16_StartPBA, FtlSdDev.u16_StartPBA+FTLSd_1PBlkCnt());
        ftl_die();
    }
    {
		U32 u32_i=0;
		//for(u32_i = 0; u32_i < u32_PageCnt; u32_i ++)
		{
            #if defined(MSFTLSd_SBOOT) && MSFTLSd_SBOOT
			u32_err = NC_ReadPages(pNandDrv, u32_PhyRowIdx + u32_i, au8_DataReadBuf, au8_SpareReadBuf, 1);
            #else
            u32_err = NC_ReadPages(u32_PhyRowIdx + u32_i, au8_DataReadBuf, au8_SpareReadBuf, 1);
            #endif
			if(0==nand_CheckEmptyPageFalseAlarm(au8_DataReadBuf, au8_SpareReadBuf))
			{
				FTLSd_Disable_ErrLog = 0;
				ftl_dbg(FTLSd_DBG_LV_ERR, 1, "Write no blank page row %Xh\n", u32_PhyRowIdx + u32_i);
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

    #if FTLSd_PROFILE_WAF
    u32_HAL512BCnt += u32_SectorCnt*FtlSdDev.u16_Sector512BCnt/FtlSdDev.u8_PlaneCnt;
    u32_HAL1MBCnt += u32_HAL512BCnt >>11;
    u32_HAL512BCnt -=  (u32_HAL512BCnt >>11) <<11;
    #endif
    
    return u32_err;
}


U32 FTLSd_EraseBlk(U16 u16_PBA)
{
    U32 u32_err;

    #if TEMP_CHECK
    if(u16_PBA < FtlSdDev.u16_StartPBA || 
        u16_PBA >= FtlSdDev.u16_StartPBA+FTLSd_1PBlkCnt())
    {
        ftl_dbg(FTLSd_DBG_LV_ERR,1,"PBA:%Xh, %Xh %Xh \n", u16_PBA,
            FtlSdDev.u16_StartPBA, FtlSdDev.u16_StartPBA+FTLSd_1PBlkCnt());
        ftl_die();
    }
    #endif

    // ftl_dbg(0,1,"%Xh \n", u16_PBA);
    u32_err = NC_EraseBlk(BlkRowAddr(u16_PBA));
    
    return u32_err;
}


U32 FTLSd_EraseBlk_HandleBB(U16 u16_PBA)
{
    U32 u32_err;

    u32_err = (*FtlSdDev.pFnEraseBlk)(u16_PBA);
    if(u32_err)
    {
        BB_SET(u16_PBA);
        //FTLSd_WriteBBT(pu8_FtlRBBT);
    }
    return u32_err;
}




