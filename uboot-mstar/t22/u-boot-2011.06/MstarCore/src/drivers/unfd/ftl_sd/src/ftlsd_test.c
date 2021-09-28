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

#if (defined(MSFTLSd_UBOOT)&&MSFTLSd_UBOOT) || (defined(MSFTLSd_LINUX)&&MSFTLSd_LINUX)
U8 *pu8_FtlTestDatBuf = (U8*)DMA_W_ADDR; 
U8 *pu8_FtlTestTmpBuf = (U8*)DMA_R_ADDR; 
U8 *pu8_FtlTestRddBuf = (U8*)DMA_W_SPARE_ADDR;

#define DATA_LEN   500
// =============================================
U32 FtlSdTest_DataWRC(U32 u32_TestLoop, U32 seed)
{
    volatile U32 u32_i;
    volatile U32 *pu32_1=(U32*)pu8_FtlTestDatBuf, *pu32_2=(U32*)pu8_FtlTestTmpBuf;

    for(u32_i=0; u32_i<DATA_LEN; u32_i++)
        pu32_1[u32_i] = u32_i+seed;

    for(u32_i=0; u32_i<u32_TestLoop; u32_i++)
    {
        ftl_dbg(0,1,"loop: %Xh \n", u32_i);
        
        if(FTLSd_WriteData(pu8_FtlTestDatBuf, DATA_LEN))
            return FTLSd_ERR_TEST_FAIL;
            
        if(FTLSd_ReadData(pu8_FtlTestTmpBuf, DATA_LEN))
            return FTLSd_ERR_TEST_FAIL;
            
        if(nand_CompareData(pu8_FtlTestDatBuf, pu8_FtlTestTmpBuf, DATA_LEN))
            return FTLSd_ERR_TEST_FAIL;
    }
    return FTLSd_OK;
}


U32 FtlSdTest_DataRC(U32 u32_TestLoop, U32 seed)
{
    volatile U32 u32_i;
    volatile U32 *pu32_1=(U32*)pu8_FtlTestDatBuf, *pu32_2=(U32*)pu8_FtlTestTmpBuf;

    for(u32_i=0; u32_i<DATA_LEN; u32_i++)
        pu32_1[u32_i] = u32_i+seed;

    for(u32_i=0; u32_i<u32_TestLoop; u32_i++)
    {
        ftl_dbg(0,1,"loop: %Xh \n", u32_i);
        
        //if(FTLSd_WriteData(pu8_FtlTestDatBuf, DATA_LEN))
        //    return FTLSd_ERR_TEST_FAIL;
            
        if(FTLSd_ReadData(pu8_FtlTestTmpBuf, DATA_LEN))
            return FTLSd_ERR_TEST_FAIL;
            
        if(nand_CompareData(pu8_FtlTestDatBuf, pu8_FtlTestTmpBuf, DATA_LEN))
            return FTLSd_ERR_TEST_FAIL;
    }
    return FTLSd_OK;
}


#endif // uboot || linux

