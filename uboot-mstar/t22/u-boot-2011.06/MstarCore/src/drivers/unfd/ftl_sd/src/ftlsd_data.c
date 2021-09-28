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

UNFD_ALIGN0 FTLSD_INFO FtlSdInfo UNFD_ALIGN1;
U8  *pu8_FtlSdDatBuf=NULL;

// ============================================
void FTLSd_DumpData(void)
{
    U16 i, cnt=0;

    #if 0//defined(CONFIG_TEE_LOADER) && defined(CONFIG_MSTAR_BUILD_TEE_LOADER_DAT)
    LDR_PUTS("PBA      :\n"); LDR_PUTDW(FtlSdInfo.PBA); LDR_PUTS("\n");
    LDR_PUTS("FreeLPIdx: \n"); LDR_PUTDW(FtlSdInfo.u16_FreeLPIdx); LDR_PUTS("\n");
    LDR_PUTS("WCounter: \n"); LDR_PUTDW(FtlSdInfo.u32_WCounter); LDR_PUTS("\n");
    LDR_PUTS("BBCnt: \n"); LDR_PUTDW(FtlSdInfo.u16_BBCnt); LDR_PUTS("\n");
    LDR_PUTS("BBIdx: \n");
    for(i=0; i<FtlSdDev.u16_PBACnt; i++)
        if(BB_CHK(i+FtlSdDev.u16_StartPBA))
        {
            cnt++;
            LDR_PUTDW(i+FtlSdDev.u16_StartPBA); LDR_PUTS("\n");
        }
        
    if(cnt != FtlSdInfo.u16_BBCnt)
    {
        LDR_PUTS("cnt != BBCnt \n");
        LDR_PUTDW(cnt); LDR_PUTS("\n");
        LDR_PUTDW(FtlSdInfo.u16_BBCnt); LDR_PUTS("\n");
        
    }
    LDR_PUTS("--------------------------\n\n");

    //=========================================================
    #else
    ftl_dbg_d2(FTLSd_DBG_LV_LOG,1,"--------------------------\n");
    ftl_dbg_d2(FTLSd_DBG_LV_LOG,0,"PBA      :       %Xh \n", FtlSdInfo.PBA);
    ftl_dbg_d2(FTLSd_DBG_LV_LOG,0,"FreeLPIdx:       %Xh \n", FtlSdInfo.u16_FreeLPIdx);
    ftl_dbg_d2(FTLSd_DBG_LV_LOG,0,"WCounter:        %Xh \n", FtlSdInfo.u32_WCounter);
    ftl_dbg_d2(FTLSd_DBG_LV_LOG,0,"BBCnt:           %Xh \n", FtlSdInfo.u16_BBCnt);
    ftl_dbg_d2(FTLSd_DBG_LV_LOG,0,"BBIdx:           \n");
    for(i=0; i<FtlSdDev.u16_PBACnt; i++)
        if(BB_CHK(i+FtlSdDev.u16_StartPBA))
        {
            cnt++;
            ftl_dbg_d2(FTLSd_DBG_LV_LOG,0,"                 %Xh ", 
                i+FtlSdDev.u16_StartPBA);
            if((i+1)%4==0)
                ftl_dbg_d2(FTLSd_DBG_LV_LOG,0,"\n");
        }

    ftl_dbg_d2(FTLSd_DBG_LV_LOG,0,"\n");
    
    if(cnt != FtlSdInfo.u16_BBCnt)
        ftl_dbg_d2(FTLSd_DBG_LV_ERR,0,"cnt: %Xh != BBCnt: %Xh \n", cnt, FtlSdInfo.u16_BBCnt);
    
    ftl_dbg_d2(FTLSd_DBG_LV_LOG,1,"--------------------------\n\n");
    #endif
}


// each sector is 512B
// always ok, or die inside.
U32 FTLSd_ReadData_Ex(U8 *pu8_DataBuf, U32 u32_PCnt)
{
    if(0==u32_PCnt)
        return FTLSd_OK;
    
    memcpy(pu8_DataBuf, pu8_FtlSdDatBuf, FtlSdDev.u16_PageByteCnt);
    return FTLSd_OK;
}


static U32 FTLSd_FindPrevPBA(void)
{
    U16 OriPBA, PBA;

    OriPBA = FtlSdInfo.PBA;
    PBA = FtlSdInfo.PBA;

    if(OriPBA == FtlSdDev.u16_StartPBA)
        PBA = FtlSdDev.u16_StartPBA + FtlSdDev.u16_PBACnt - 1;

    while(PBA >= FtlSdDev.u16_StartPBA)
    {
        PBA--;
        if(!BB_CHK(PBA))
            break;
    }
    if(PBA == FtlSdDev.u16_StartPBA-1)
    {
        PBA = FtlSdDev.u16_StartPBA + FtlSdDev.u16_PBACnt - 1;
        
        while(PBA > OriPBA)
        {
            if(!BB_CHK(PBA))
                break;

            PBA--;
        }

        while(PBA == OriPBA)
        {
            if(BB_CHK(OriPBA))
            {
                ftl_dbg(FTLSd_DBG_LV_ERR,1,"no good PBA available.");
                ftl_die();
            }
            else
                ftl_dbg(FTLSd_DBG_LV_LOG,1,"only 1 good PBA available.");            
        }
    }

    return PBA;
}


static U32 FTLSd_FindNextPBA(void)
{
    U16 OriPBA, PBA;

    OriPBA = FtlSdInfo.PBA;
    PBA = FtlSdInfo.PBA;

    LABEL_START:
    while(PBA < FtlSdDev.u16_StartPBA + FtlSdDev.u16_PBACnt)
    {
        PBA++;
        if(!BB_CHK(PBA))
            break;
    }
    if(PBA >= FtlSdDev.u16_StartPBA + FtlSdDev.u16_PBACnt)
    {
        PBA = FtlSdDev.u16_StartPBA;
        
        while(PBA < OriPBA)
        {
            if(!BB_CHK(PBA))
                break;

            PBA++;
        }

        while(PBA >= OriPBA)
        {
            if(BB_CHK(OriPBA))
            {
                ftl_dbg(FTLSd_DBG_LV_ERR,1,"no good PBA available.");
                ftl_die();
            }
            else
                ftl_dbg(FTLSd_DBG_LV_LOG,1,"only 1 good PBA available.");
        }
    }

    while(FTLSd_OK != FTLSd_EraseBlk_HandleBB(PBA))
        goto LABEL_START;

    return PBA;
}


U32 FTLSd_WriteData_Ex(U8 *pu8_DataBuf, U32 u32_PCnt)
{
    U32 err = FTLSd_OK, row;
    
    
    // ============================================
    LABEL_NEXT_PBA:
    
    if(err != FTLSd_OK || 
        ga_tPairedPageMap[FtlSdInfo.u16_FreeLPIdx].u16_MSB == FtlSdDev.u16_BlkPageCnt-1 || 
        ga_tPairedPageMap[FtlSdInfo.u16_FreeLPIdx].u16_LSB == FtlSdDev.u16_BlkPageCnt-1)
    {        
        FtlSdInfo.PBA = FTLSd_FindNextPBA();
        ftl_dbg_data(0,1,"PBA: %Xh \n", FtlSdInfo.PBA);

        FtlSdInfo.u16_FreeLPIdx=0;
        row = BlkRowAddr(FtlSdInfo.PBA) + 
            ga_tPairedPageMap[FtlSdInfo.u16_FreeLPIdx].u16_LSB;

        FtlSdInfo.u32_WCounter++;
        FtlSdInfo.RDD.u8_DID = DID_INFO;
        
        err = (*FtlSdDev.pFnWritePages)(row, (U8*)&FtlSdInfo, (U8*)&FtlSdInfo.RDD, 1);
        if(err != FTLSd_OK)
        {
            BB_SET(FtlSdInfo.PBA);
            goto LABEL_NEXT_PBA;
        }

        FtlSdInfo.RDD.u8_DID = DID_DATA;
        FtlSdInfo.u16_FreeLPIdx++;
    }      
    
    // ============================================
    row = BlkRowAddr(FtlSdInfo.PBA) + 
            ga_tPairedPageMap[FtlSdInfo.u16_FreeLPIdx].u16_LSB;
    
    err = (*FtlSdDev.pFnWritePages)(row, pu8_DataBuf, (U8*)&FtlSdInfo.RDD, 1);
    if(err != FTLSd_OK && err != UNFD_ST_ERR_W_PROTECTED)
    {
        BB_SET(FtlSdInfo.PBA);
        goto LABEL_NEXT_PBA;
    }

    // ============================================
    if(FTLSd_OK == err)
    {
        FtlSdInfo.u16_FreeLPIdx++;
        memcpy(pu8_FtlSdDatBuf, pu8_DataBuf, FtlSdDev.u16_PageByteCnt);
    }
        
    return FTLSd_OK;
}


U32  FTLSd_SetupDB(void)
{
    U32 err, row;

    ftl_trace("\n");

    FtlSdInfo.PBA = FtlSdDev.u16_StartPBA - 1;
    FtlSdInfo.PBA = FTLSd_FindNextPBA();
    FtlSdInfo.u16_FreeLPIdx = 0;
    FtlSdInfo.u32_WCounter = 1;
    FtlSdInfo.RDD.u8_DID = DID_INFO;

    do{        
        row = BlkRowAddr(FtlSdInfo.PBA) + 
                ga_tPairedPageMap[FtlSdInfo.u16_FreeLPIdx].u16_LSB;
        
        err = (*FtlSdDev.pFnWritePages)
            (row, (U8*)&FtlSdInfo, (U8*)&FtlSdInfo.RDD, 1);

        if(err != FTLSd_OK)
        {
            ftl_dbg(0,1,"w fail: %Xh, PBA: %Xh \n", err, FtlSdInfo.PBA);
            BB_SET(FtlSdInfo.PBA);
            FtlSdInfo.PBA = FTLSd_FindNextPBA();
        }
    }
    while(err != FTLSd_OK);

    memset(pu8_FtlSdDatBuf, 0xFF, FtlSdDev.u16_PageByteCnt);
    FtlSdInfo.RDD.u8_DID = DID_DATA;
    FtlSdInfo.u16_FreeLPIdx++;
    
    return err;
}


U32  FTLSd_InitDB(void)
{
    U32 err=FTLSd_OK, row, WCnt=0;
    U16 i, PBA=0, LPIdx;
    P_FTLSD_INFO pSdInfo = (P_FTLSD_INFO)pu8_FtlSdPDatBuf;
    
    ftl_trace("\n");

    FtlSdInfo.RDD.u8_BBMark=0xFF;

    // ============================================
    // find the latest PBA
    for(i=FtlSdDev.u16_StartPBA; i<FtlSdDev.u16_StartPBA+FtlSdDev.u16_PBACnt; i++)
    {
        row = BlkRowAddr(i) + ga_tPairedPageMap[0].u16_LSB;
        err = (*FtlSdDev.pFnReadPages)(
            row, pu8_FtlSdPDatBuf, pu8_FtlSdPRddBuf, 1);

        if(0==pu8_FtlSdPRddBuf[0])
        {
            BB_SET(i);  FtlSdInfo.u16_BBCnt++;
            ftl_dbg(0,0,"bb: %Xh \n", i);
            continue;
        }
        
        if(FTLSd_OK != err) // may be w @ pwr_cut
            continue;
        
        if(WCnt < pSdInfo->u32_WCounter)
        {
            if(0xFFFFFFFF != pSdInfo->u32_WCounter)
                PBA = i;
            
            WCnt = pSdInfo->u32_WCounter;
        }
    }

    //FTLSd_DumpData();
    // ============================================
    if(0==PBA) // empty
        FTLSd_SetupDB();
    else
    {
        LPIdx = drvNAND_FindFreeLSBPageIdx(PBA);
        //ftl_dbg(0,1,"PBA: %Xh  LPIdx: %Xh \n", PBA, LPIdx);
        
        row = BlkRowAddr(PBA) + ga_tPairedPageMap[0].u16_LSB;
        err = (*FtlSdDev.pFnReadPages)(
            row, pu8_FtlSdPDatBuf, pu8_FtlSdPRddBuf, 1);
        if(FTLSd_OK != err) // should not be UECC
        {
            ftl_dbg(FTLSd_DBG_LV_ERR,1,"read BlkInfo error: %Xh \n", err);
            ftl_die();
        }
        memcpy(&FtlSdInfo, pu8_FtlSdPDatBuf, sizeof(FTLSD_INFO));
        FtlSdInfo.u16_FreeLPIdx = LPIdx;

        // --------------------------
        LABEL_LOAD_DATA: // load data

        for(i=LPIdx; i>1; i--)
        {
            row = BlkRowAddr(PBA) + ga_tPairedPageMap[i-1].u16_LSB;
            err = (*FtlSdDev.pFnReadPages)(
                row, pu8_FtlSdDatBuf, pu8_FtlSdPRddBuf, 1);
            if(err != FTLSd_OK)
                continue;
            else 
                break;
        }        
        
        if(0==i-1)
        {
            PBA = FTLSd_FindPrevPBA();
            LPIdx = FtlSdDev.u16_BlkLowPCnt;
            goto LABEL_LOAD_DATA;
        }
            
    }
    
    return err;
}




