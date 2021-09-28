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


// ============================================
U32 gu32_FtlSdFlag = 0;
U8 *pu8_FtlSdApiBuf=NULL;

static void FTLSd_CleanStruct(void);
static U32 FTLSd_Core_Init(void);

// ============================================
U32 FTLSd_CheckIfReady(void)
{
    if(FTLSd_CHK_FLAG(FTLSd_INIT_READY))
        return FTLSd_OK;

    return FTLSd_ERR_NOT_READY;
}

void FTLSd_ResetReadyFlag(void)
{    
    gu32_FtlSdFlag = 0;
    //FTLSd_free();
}

// ----------------------------------------------------
// 1. if no pni or pni has no FTL partiton (no u16_PartType == UNFD_PART_FTL)
//     fill in the parameters with the range for FTL.
// 2. or the range determined by pni (u16_PartType == UNFD_PART_FTL), 
//     and ignore the input parameters.
// ----------------------------------------------------
U32 FTLSd_Init(U16 u16_StartPBA, U16 u16_PBACnt)
{
    U32 u32_err;
	
    if(FTLSd_OK == FTLSd_CheckIfReady())
        return FTLSd_OK;

    u32_err = drvNAND_CheckAlignPack(UNFD_CACHE_LINE);
    if(FTLSd_OK != u32_err)
        return u32_err;
    
    // --------------------------
    #if defined(CONFIG_RESERVED_END_OF_NAND)
    {
        NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();
        
        u16_PBACnt = CONFIG_RESERVED_END_OF_NAND >> 
            (pNandDrv->u8_BlkPageCntBits + pNandDrv->u8_PageByteCntBits);
        
        if(u16_PBACnt < 2)
            u16_PBACnt = 2; // need < 1 blk, assign 2 blk at least.
        else if(
            u16_PBACnt<<(pNandDrv->u8_BlkPageCntBits + pNandDrv->u8_PageByteCntBits)
            != CONFIG_RESERVED_END_OF_NAND)
            u16_PBACnt++;

        u16_StartPBA = pNandDrv->u16_BlkCnt - u16_PBACnt;
        
        u32_err = FTLSd_Hal_Init(u16_StartPBA, u16_PBACnt);

        SecureNandEndBlkCnt = u16_PBACnt;
    }
    #else
    u32_err = FTLSd_Hal_Init(u16_StartPBA, u16_PBACnt);
    #endif   
    
    if(FTLSd_OK != u32_err)
    	return u32_err;
    
    // --------------------------
    u32_err = FTLSd_Core_Init();
    if(FTLSd_OK != u32_err)
        return u32_err;
    
    // --------------------------
    // others
    FTLSd_SET_FLAG(FTLSd_INIT_READY);    
    return FTLSd_OK;
}


static void FTLSd_CleanStruct(void)
{
     memset(&FtlSdInfo, 0, sizeof(FTLSD_INFO)); 
}


static U32 FTLSd_Core_Init(void)
{
    U32 err;

    // --------------------------
    FTLSd_CleanStruct();

    // --------------------------   
    err=FTLSd_InitDB();
    if(err)  
        return err;    

        
    return FTLSd_OK;
}


// ============================================
// ok: return 0
// u32_512BCnt: each sector is 512B
U32 FTLSd_WriteData(U8* pu8_DataBuf, U32 u32_ByteCnt)
{
    U16 PCnt;
    
    #if API_CHECK_PARAM
    if(FTLSd_OK != FTLSd_CheckIfReady())
    {
        ftl_dbg(FTLSd_DBG_LV_ERR,1,"did not init yet \n");
        return FTLSd_ERR_NOT_READY;
    }
    
    if(NULL==pu8_DataBuf || 0==u32_ByteCnt)
    {
        ftl_dbg(FTLSd_DBG_LV_ERR,1,"error input: Buf:%Xh BCnt:%Xh \n", 
            (U32)pu8_DataBuf, u32_ByteCnt);
        ftl_die();
        return FTLSd_ERR_PARAMETER;
    }

    // temp: Small-data length is 5xx bytes.
    if(u32_ByteCnt > FtlSdDev.u16_PageByteCnt)
    {
        ftl_dbg(FTLSd_DBG_LV_ERR,1,"error ByteCnt: %Xh > PageByteCnt: %Xh\n", 
            u32_ByteCnt, FtlSdDev.u16_PageByteCnt);
        ftl_die();
        return FTLSd_ERR_W_OUT_RANGE;
    }
    #endif

    PCnt = u32_ByteCnt >> FtlSdDev.u8_PageByteCntBits;
    PCnt += (u32_ByteCnt & FtlSdDev.u16_PageByteCntMask) ? 1 : 0;

    ftl_dbg_api(FTLSd_DBG_LV_LOG,0,"\n");
    //ftl_dbg_api(FTLSd_DBG_LV_LOG,1,"512BAddr: %Xh  512BCnt: %Xh  buf: %Xh\n", u32_512BAddr, u32_512BCnt, (U32)pu8_DataBuf);
    ftl_dbg_api(FTLSd_DBG_LV_LOG,1,"BufAddr: %Xh  ByteCnt: %Xh  PCnt: %Xh\n", 
        (U32)pu8_DataBuf, u32_ByteCnt, PCnt);

    FTLSd_WriteData_Ex(pu8_DataBuf, PCnt);

    //ftl_dbg_api(FTLSd_DBG_LV_LOG,1,"-\n");
    return FTLSd_OK;
}


// ok: return 0
// u32_512BCnt: each sector is 512B
U32 FTLSd_ReadData(U8* pu8_DataBuf, U32 u32_ByteCnt)
{
    U16 PCnt;
    
    #if API_CHECK_PARAM
    if(NULL==pu8_DataBuf || 0==u32_ByteCnt)
    {
        ftl_dbg(FTLSd_DBG_LV_ERR,1,"error input: Buf:%Xh BCnt:%Xh \n", 
            (U32)pu8_DataBuf, u32_ByteCnt);
        ftl_die();
        return FTLSd_ERR_PARAMETER;
    }

    // [CAUTION]: Small-data length is 5xx bytes.
    if(u32_ByteCnt > FtlSdDev.u16_PageByteCnt)
    {
        ftl_dbg(FTLSd_DBG_LV_ERR,1,"error ByteCnt: %Xh > PageByteCnt: %Xh\n", 
            u32_ByteCnt, FtlSdDev.u16_PageByteCnt);
        ftl_die();
        return FTLSd_ERR_W_OUT_RANGE;
    }
    #endif

    ftl_dbg_api(FTLSd_DBG_LV_LOG,0,"\n");
    //ftl_dbg_api(FTLSd_DBG_LV_LOG,1,"512BAddr: %Xh  512BCnt: %Xh  buf: %Xh\n", u32_512BAddr, u32_512BCnt, (U32)pu8_DataBuf);
    ftl_dbg_api(FTLSd_DBG_LV_LOG,1,"BufAddr: %Xh  ByteCnt: %Xh\n", 
        (U32)pu8_DataBuf, u32_ByteCnt);

    PCnt = u32_ByteCnt >> FtlSdDev.u8_PageByteCntBits;
    FTLSd_ReadData_Ex(pu8_DataBuf, PCnt);

    if(u32_ByteCnt & FtlSdDev.u16_PageByteCntMask)
    {
        //ftl_dbg_api(FTLSd_DBG_LV_LOG,0,"partial: %Xh \n", u32_ByteCnt & FtlSdDev.u16_PageByteCntMask);
        
        FTLSd_ReadData_Ex(pu8_FtlSdApiBuf, 1);
        memcpy(pu8_DataBuf + (PCnt << FtlSdDev.u8_PageByteCntBits), 
            pu8_FtlSdApiBuf,
            u32_ByteCnt & FtlSdDev.u16_PageByteCntMask);        
    }

    //ftl_dbg_api(FTLSd_DBG_LV_LOG,1,"-\n");
    return FTLSd_OK;
}



// return sector count (1 sector = 512B)
U32  FTLSd_GetDataCapacity(void)
{

    return 0;
}

U32  FTLSd_GetUID(U8 *pBuf)
{

    return FTLSd_OK;
}


