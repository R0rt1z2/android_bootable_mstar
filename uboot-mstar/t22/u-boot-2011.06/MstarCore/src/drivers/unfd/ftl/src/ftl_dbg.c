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

U16 FTL_Disable_ErrLog=1;
U16 u16_PwrCut_TrigID=0xFFFF;
U32 u32_PwrCut_InitUs=0, u32_PwrCut_TrigUs = 0;


void NFTL_DumpDevInfo(void)
{
    U32 u32_MemByteCnt = FtlDev.u16_PageByteCnt + FtlDev.u16_SpareByteCnt;
    U32 u32_ChkSum = drvNAND_CheckSum((U8*)&FtlDev, (U32)(&FtlDev.u32_DevParamChkSum) - (U32)&FtlDev);
    
    ftl_dbg(FTL_DBG_LV_LOG,1,"--------------------------\n");
    ftl_dbg(FTL_DBG_LV_LOG,0,"Mem Usage:          %u.%u KB \n", 
        u32_MemByteCnt/1024, (u32_MemByteCnt*10/1024)%10);
    ftl_dbg(FTL_DBG_LV_LOG,0,"PDatBuf:            %Xh \n", (U32)pu8_FtlPDatBuf);
    ftl_dbg(FTL_DBG_LV_LOG,0,"PRddBuf:            %Xh \n", (U32)pu8_FtlPRddBuf);

    if(FtlDev.u32_DevParamChkSum != u32_ChkSum)
        ftl_dbg(FTL_DBG_LV_ERR,1,"ChkSum error:       %Xh %Xh \n", 
            FtlDev.u32_DevParamChkSum, u32_ChkSum);
    else
        ftl_dbg(FTL_DBG_LV_LOG,0,"Checksum:           OK \n");

    // --------------------------
    ftl_dbg(FTL_DBG_LV_LOG,0,"NAND Type:          %s  \n", FtlDev.u8_CellType==NAND_CellType_MLC?"MLC":"SLC");
    ftl_dbg(FTL_DBG_LV_LOG,0,"NAND ID:            ");
    for(u32_MemByteCnt=0; (U8)u32_MemByteCnt<FtlDev.u8_IDByteCnt; u32_MemByteCnt++)
        ftl_dbg(FTL_DBG_LV_LOG,0,"%Xh ", FtlDev.au8_ID[u32_MemByteCnt]);
    ftl_dbg(FTL_DBG_LV_LOG,0,"\n");
    ftl_dbg(FTL_DBG_LV_LOG,0,"NAND Plane Cnt:     %Xh \n", FtlDev.u8_PlaneCnt);
    
    // --------------------------
    ftl_dbg(FTL_DBG_LV_LOG,0,"BlkPageCnt:         %Xh \n", FtlDev.u16_BlkPageCnt);
    ftl_dbg(FTL_DBG_LV_LOG,0,"PageByteCnt:        %Xh \n", FtlDev.u16_PageByteCnt);
    ftl_dbg(FTL_DBG_LV_LOG,0,"SectorByteCnt:      %Xh \n", FtlDev.u16_SectorByteCnt);
    ftl_dbg(FTL_DBG_LV_LOG,0,"SpareByteCnt:       %Xh \n", FtlDev.u16_SpareByteCnt);
    ftl_dbg(FTL_DBG_LV_LOG,0,"BlkLowPCnt:         %Xh \n", FtlDev.u16_BlkLowPCnt);

    ftl_dbg(FTL_DBG_LV_LOG,0,"BlkSectorCnt:       %Xh \n", FtlDev.u16_BlkSectorCnt);
    ftl_dbg(FTL_DBG_LV_LOG,0,"Blk512BCnt:         %Xh \n", FtlDev.u16_Blk512BCnt);
    ftl_dbg(FTL_DBG_LV_LOG,0,"PageSectorCnt:      %Xh \n", FtlDev.u16_PageSectorCnt);
    ftl_dbg(FTL_DBG_LV_LOG,0,"Page512BCnt:        %Xh \n", FtlDev.u16_Page512BCnt);
    ftl_dbg(FTL_DBG_LV_LOG,0,"Sector512BCnt:      %Xh \n", FtlDev.u16_Sector512BCnt);

    ftl_dbg(FTL_DBG_LV_LOG,0,"SectorSpareByteCnt: %Xh \n", FtlDev.u16_SectorSpareByteCnt);
    ftl_dbg(FTL_DBG_LV_LOG,0,"ECCCodeByteCnt:     %Xh \n", FtlDev.u16_ECCCodeByteCnt);

    ftl_dbg(FTL_DBG_LV_LOG,0,"BlkPageCntBits:     %Xh \n", FtlDev.u8_BlkPageCntBits);
    ftl_dbg(FTL_DBG_LV_LOG,0,"BlkSectorCntBits:   %Xh \n", FtlDev.u8_BlkSectorCntBits);
    ftl_dbg(FTL_DBG_LV_LOG,0,"Blk512BCntBits:     %Xh \n", FtlDev.u8_Blk512BCntBits);
    ftl_dbg(FTL_DBG_LV_LOG,0,"PageByteCntBits:    %Xh \n", FtlDev.u8_PageByteCntBits);
    ftl_dbg(FTL_DBG_LV_LOG,0,"PageSectorCntBits:  %Xh \n", FtlDev.u8_PageSectorCntBits);
    ftl_dbg(FTL_DBG_LV_LOG,0,"Page512BCntBits:    %Xh \n", FtlDev.u8_Page512BCntBits);
    ftl_dbg(FTL_DBG_LV_LOG,0,"SectorByteCntBits:  %Xh \n", FtlDev.u8_SectorByteCntBits);
    ftl_dbg(FTL_DBG_LV_LOG,0,"Sector512BCntBits:  %Xh \n", FtlDev.u8_Sector512BCntBits);

    ftl_dbg(FTL_DBG_LV_LOG,0,"BlkPageCntMask:     %Xh \n", FtlDev.u16_BlkPageCntMask);
    ftl_dbg(FTL_DBG_LV_LOG,0,"BlkSectorCntMask:   %Xh \n", FtlDev.u16_BlkSectorCntMask);
    ftl_dbg(FTL_DBG_LV_LOG,0,"Blk512BCntMask:     %Xh \n", FtlDev.u16_Blk512BCntMask);
    ftl_dbg(FTL_DBG_LV_LOG,0,"PageByteCntMask:    %Xh \n", FtlDev.u16_PageByteCntMask);
    ftl_dbg(FTL_DBG_LV_LOG,0,"PageSectorCntMask:  %Xh \n", FtlDev.u16_PageSectorCntMask);
    ftl_dbg(FTL_DBG_LV_LOG,0,"Page512BCntMask:    %Xh \n", FtlDev.u16_Page512BCntMask);
    ftl_dbg(FTL_DBG_LV_LOG,0,"SectorByteCntMask:  %Xh \n", FtlDev.u16_SectorByteCntMask);
    ftl_dbg(FTL_DBG_LV_LOG,0,"Sector512BCntMask:  %Xh \n", FtlDev.u16_Sector512BCntMask);

    // --------------------------
    ftl_dbg(FTL_DBG_LV_LOG,0,"StartPBA:           %Xh \n", FtlDev.u16_StartPBA);
    ftl_dbg(FTL_DBG_LV_LOG,0,"PBACnt(%uP):         %Xh \n", FtlDev.u8_PlaneCnt, FtlDev.u16_PBACnt);
    ftl_dbg(FTL_DBG_LV_LOG,0,"PBACnt(1P):         %Xh \n", FTL_1PBlkCnt());
    ftl_dbg(FTL_DBG_LV_LOG,0,"PartitionSize:      %u.%u MB  \n", 
        BlkCntToMB(FTL_1PBlkCnt())/FtlDev.u8_PlaneCnt,
        (BlkCntToMB(FTL_1PBlkCnt()*10)/FtlDev.u8_PlaneCnt)%10);
    
    ftl_dbg(FTL_DBG_LV_LOG,1,"--------------------------\n\n");
}


void NFTL_DumpInfo(void)
{
    ftl_dbg(FTL_DBG_LV_LOG,0,"\n\n");
    
    NFTL_DumpDevInfo();
    NFTL_DumpRoot();
    NFTL_DumpCtrl();
    NFTL_DumpDBPM();
    NFTL_DumpData();
    NFTL_DumpDBPM_Details();

    ftl_dbg(FTL_DBG_LV_LOG,1,"Mem Usage: %u.%u MB \n", 
        u32_MemUsageByte>>20, (u32_MemUsageByte*100>>20)%100);
}


U32 NFTL_Die(void)
{
    ftl_die();
    return FTL_ERR_FATAL;
}
// ==============================================
// PowerCut test
// ==============================================
void NFTL_PwrOff(U16 u16_ID, U32 u32_MaxUs)
{
    if(u16_ID != u16_PwrCut_TrigID)
    {
        u32_PwrCut_TrigUs = 0;
        return;
    }

    u32_PwrCut_TrigUs = HWTimer_End() % u32_MaxUs;
    if(0==u32_PwrCut_TrigUs)
        u32_PwrCut_TrigUs=1;
        
    ftl_dbg(FTL_DBG_LV_LOG,0,"[PWROFF_TEST] ID: %u  Dly: %u us\n", 
        u16_PwrCut_TrigID, u32_PwrCut_TrigUs);
}


