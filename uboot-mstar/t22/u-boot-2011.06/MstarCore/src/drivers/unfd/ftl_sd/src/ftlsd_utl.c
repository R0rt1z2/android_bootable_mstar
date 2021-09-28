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

U8  *pFtlSdUtlSectorBuf=NULL;

// ============================================

/* return 1: Free block, 0: not Free block */
U32 FTLSd_IsFreeBlk(U16 u16_PBA)
{
    if(IF_BB(u16_PBA))
        return 0;

    return FTLSd_IsFreePage(BlkRowAddr(u16_PBA));
}


/* return 1: Free page, 0: not Free page */
U32 FTLSd_IsFreePage(U32 u32_Row)
{
    U32 u32_err;
    P_COMM_RDD pRDD = (P_COMM_RDD)pu8_FtlSdPRddBuf;
    //ftl_dbg(0,0,"%lXh ", u32_Row);
    u32_Row = PhyRowFromMultiPlaneRow(u32_Row);
    //ftl_dbg(0,0,"%lXh \n", u32_Row);
    u32_err = FTLSd_ReadSectors(u32_Row, 0, pFtlSdUtlSectorBuf, pu8_FtlSdPRddBuf, 1);

    if(FTLSd_OK==u32_err && 0xFF==pRDD->u8_BBMark && DID_FREE==pRDD->u8_DID)
        return 1; // FP

    return 0;
}


// handle multi-plane case
// only used by ftl_root.c, ftl_test.c.
// return 1: Good block, 0: Bad block.
U32 FTLSd_IsGoodBlk(U16 u16_PBA)
{
    return drvNAND_IsGoodBlk(u16_PBA);    
}


