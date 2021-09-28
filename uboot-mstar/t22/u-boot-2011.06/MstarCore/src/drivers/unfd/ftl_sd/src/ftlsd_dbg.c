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

U16 FTLSd_Disable_ErrLog=0;


void FTLSd_DumpDevInfo(void)
{
    U32 u32_MemByteCnt = FtlSdDev.u16_PageByteCnt + FtlSdDev.u16_SpareByteCnt;
    U32 u32_ChkSum = drvNAND_CheckSum((U8*)&FtlSdDev, (U32)(&FtlSdDev.u32_DevParamChkSum) - (U32)&FtlSdDev);

    #if 0//defined(CONFIG_TEE_LOADER) && defined(CONFIG_MSTAR_BUILD_TEE_LOADER_DAT)
    LDR_PUTS("NAND ID: \n");
    LDR_PUTDW((FtlSdDev.au8_ID[0]<<24)|(FtlSdDev.au8_ID[1]<<16)|(FtlSdDev.au8_ID[2]<<8)|FtlSdDev.au8_ID[3]);  
    LDR_PUTDW((FtlSdDev.au8_ID[4]<<8)|FtlSdDev.au8_ID[5]);  
    LDR_PUTS("\n");
    LDR_PUTS("Plane Cnt: \n"); LDR_PUTDW(FtlSdDev.u8_PlaneCnt); LDR_PUTS("\n");
    LDR_PUTS("BlkPageCnt: \n"); LDR_PUTDW(FtlSdDev.u16_BlkPageCnt); LDR_PUTS("\n");
    LDR_PUTS("PageByteCnt: \n"); LDR_PUTDW(FtlSdDev.u16_PageByteCnt); LDR_PUTS("\n");

    LDR_PUTS("SectorByteCnt: \n"); LDR_PUTDW(FtlSdDev.u16_SectorByteCnt); LDR_PUTS("\n");
    LDR_PUTS("SpareByteCnt: \n"); LDR_PUTDW(FtlSdDev.u16_SpareByteCnt); LDR_PUTS("\n");
    LDR_PUTS("BlkLowPCnt: \n"); LDR_PUTDW(FtlSdDev.u16_BlkLowPCnt); LDR_PUTS("\n");

    LDR_PUTS("StartPBA: \n"); LDR_PUTDW(FtlSdDev.u16_StartPBA); LDR_PUTS("\n");
    LDR_PUTS("PBACnt: \n"); LDR_PUTDW(FtlSdDev.u16_PBACnt); LDR_PUTS("\n");

    //=========================================================
    #else
    ftl_dbg(FTLSd_DBG_LV_LOG,1,"--------------------------\n");
    ftl_dbg(FTLSd_DBG_LV_LOG,0,"Mem Usage:          %u.%u KB \n", 
        u32_MemByteCnt/1024, (u32_MemByteCnt*10/1024)%10);
    ftl_dbg(FTLSd_DBG_LV_LOG,0,"PDatBuf:            %Xh \n", (U32)pu8_FtlSdPDatBuf);
    ftl_dbg(FTLSd_DBG_LV_LOG,0,"PRddBuf:            %Xh \n", (U32)pu8_FtlSdPRddBuf);

    if(FtlSdDev.u32_DevParamChkSum != u32_ChkSum)
        ftl_dbg(FTLSd_DBG_LV_ERR,1,"ChkSum error:       %Xh %Xh \n", 
            FtlSdDev.u32_DevParamChkSum, u32_ChkSum);
    else
        ftl_dbg(FTLSd_DBG_LV_LOG,0,"Checksum:           OK \n");

    // --------------------------
    ftl_dbg(FTLSd_DBG_LV_LOG,0,"NAND Type:          %s  \n", FtlSdDev.u8_CellType==NAND_CellType_MLC?"MLC":"SLC");
    ftl_dbg(FTLSd_DBG_LV_LOG,0,"NAND ID:            ");
    for(u32_MemByteCnt=0; (U8)u32_MemByteCnt<FtlSdDev.u8_IDByteCnt; u32_MemByteCnt++)
        ftl_dbg(FTLSd_DBG_LV_LOG,0,"%Xh ", FtlSdDev.au8_ID[u32_MemByteCnt]);
    ftl_dbg(FTLSd_DBG_LV_LOG,0,"\n");
    ftl_dbg(FTLSd_DBG_LV_LOG,0,"NAND Plane Cnt:     %Xh \n", FtlSdDev.u8_PlaneCnt);
    
    // --------------------------
    ftl_dbg(FTLSd_DBG_LV_LOG,0,"BlkPageCnt:         %Xh \n", FtlSdDev.u16_BlkPageCnt);
    ftl_dbg(FTLSd_DBG_LV_LOG,0,"PageByteCnt:        %Xh \n", FtlSdDev.u16_PageByteCnt);
    ftl_dbg(FTLSd_DBG_LV_LOG,0,"SectorByteCnt:      %Xh \n", FtlSdDev.u16_SectorByteCnt);
    ftl_dbg(FTLSd_DBG_LV_LOG,0,"SpareByteCnt:       %Xh \n", FtlSdDev.u16_SpareByteCnt);
    ftl_dbg(FTLSd_DBG_LV_LOG,0,"BlkLowPCnt:         %Xh \n", FtlSdDev.u16_BlkLowPCnt);

    ftl_dbg(FTLSd_DBG_LV_LOG,0,"BlkSectorCnt:       %Xh \n", FtlSdDev.u16_BlkSectorCnt);
    ftl_dbg(FTLSd_DBG_LV_LOG,0,"Blk512BCnt:         %Xh \n", FtlSdDev.u16_Blk512BCnt);
    ftl_dbg(FTLSd_DBG_LV_LOG,0,"PageSectorCnt:      %Xh \n", FtlSdDev.u16_PageSectorCnt);
    ftl_dbg(FTLSd_DBG_LV_LOG,0,"Page512BCnt:        %Xh \n", FtlSdDev.u16_Page512BCnt);
    ftl_dbg(FTLSd_DBG_LV_LOG,0,"Sector512BCnt:      %Xh \n", FtlSdDev.u16_Sector512BCnt);

    ftl_dbg(FTLSd_DBG_LV_LOG,0,"SectorSpareByteCnt: %Xh \n", FtlSdDev.u16_SectorSpareByteCnt);
    ftl_dbg(FTLSd_DBG_LV_LOG,0,"ECCCodeByteCnt:     %Xh \n", FtlSdDev.u16_ECCCodeByteCnt);

    ftl_dbg(FTLSd_DBG_LV_LOG,0,"BlkPageCntBits:     %Xh \n", FtlSdDev.u8_BlkPageCntBits);
    ftl_dbg(FTLSd_DBG_LV_LOG,0,"BlkSectorCntBits:   %Xh \n", FtlSdDev.u8_BlkSectorCntBits);
    ftl_dbg(FTLSd_DBG_LV_LOG,0,"Blk512BCntBits:     %Xh \n", FtlSdDev.u8_Blk512BCntBits);
    ftl_dbg(FTLSd_DBG_LV_LOG,0,"PageByteCntBits:    %Xh \n", FtlSdDev.u8_PageByteCntBits);
    ftl_dbg(FTLSd_DBG_LV_LOG,0,"PageSectorCntBits:  %Xh \n", FtlSdDev.u8_PageSectorCntBits);
    ftl_dbg(FTLSd_DBG_LV_LOG,0,"Page512BCntBits:    %Xh \n", FtlSdDev.u8_Page512BCntBits);
    ftl_dbg(FTLSd_DBG_LV_LOG,0,"SectorByteCntBits:  %Xh \n", FtlSdDev.u8_SectorByteCntBits);
    ftl_dbg(FTLSd_DBG_LV_LOG,0,"Sector512BCntBits:  %Xh \n", FtlSdDev.u8_Sector512BCntBits);

    ftl_dbg(FTLSd_DBG_LV_LOG,0,"BlkPageCntMask:     %Xh \n", FtlSdDev.u16_BlkPageCntMask);
    ftl_dbg(FTLSd_DBG_LV_LOG,0,"BlkSectorCntMask:   %Xh \n", FtlSdDev.u16_BlkSectorCntMask);
    ftl_dbg(FTLSd_DBG_LV_LOG,0,"Blk512BCntMask:     %Xh \n", FtlSdDev.u16_Blk512BCntMask);
    ftl_dbg(FTLSd_DBG_LV_LOG,0,"PageByteCntMask:    %Xh \n", FtlSdDev.u16_PageByteCntMask);
    ftl_dbg(FTLSd_DBG_LV_LOG,0,"PageSectorCntMask:  %Xh \n", FtlSdDev.u16_PageSectorCntMask);
    ftl_dbg(FTLSd_DBG_LV_LOG,0,"Page512BCntMask:    %Xh \n", FtlSdDev.u16_Page512BCntMask);
    ftl_dbg(FTLSd_DBG_LV_LOG,0,"SectorByteCntMask:  %Xh \n", FtlSdDev.u16_SectorByteCntMask);
    ftl_dbg(FTLSd_DBG_LV_LOG,0,"Sector512BCntMask:  %Xh \n", FtlSdDev.u16_Sector512BCntMask);

    // --------------------------
    ftl_dbg(FTLSd_DBG_LV_LOG,0,"StartPBA:           %Xh \n", FtlSdDev.u16_StartPBA);
    ftl_dbg(FTLSd_DBG_LV_LOG,0,"PBACnt(%uP):         %Xh \n", FtlSdDev.u8_PlaneCnt, FtlSdDev.u16_PBACnt);
    ftl_dbg(FTLSd_DBG_LV_LOG,0,"PBACnt(1P):         %Xh \n", FTLSd_1PBlkCnt());
    ftl_dbg(FTLSd_DBG_LV_LOG,0,"PartitionSize:      %u.%u MB  \n", 
        BlkCntToMB(FTLSd_1PBlkCnt())/FtlSdDev.u8_PlaneCnt,
        (BlkCntToMB(FTLSd_1PBlkCnt()*10)/FtlSdDev.u8_PlaneCnt)%10);
    #endif    
    
    ftl_dbg(FTLSd_DBG_LV_LOG,1,"--------------------------\n\n");
}


void FTLSd_DumpInfo(void)
{
    ftl_dbg(FTLSd_DBG_LV_LOG,0,"\n\n");
    
    FTLSd_DumpDevInfo();
    FTLSd_DumpData();
    
    ftl_dbg(FTLSd_DBG_LV_LOG,1,"Mem Usage: %u.%u MB \n", 
        u32_FtlSdMemUsageByte>>20, (u32_FtlSdMemUsageByte*100>>20)%100);
}


U32 FTLSd_Die(void)
{
    ftl_die();
    return FTLSd_ERR_FATAL;
}


