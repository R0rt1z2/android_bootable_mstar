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
#ifndef _FTL_UTL_H_
#define _FTL_UTL_H_


extern U16 FTL_Disable_ErrLog; // 1: disable all FTL log

// =========================================
#define BlkCntToKB(x)   \
    (((U32)x) << (FtlDev.u8_BlkPageCntBits+FtlDev.u8_PageByteCntBits-10))

#define BlkCntToMB(x)   (BlkCntToKB(x) >> 10)

#define BlkRowAddr(x) \
    ((U32)(x) << FtlDev.u8_BlkPageCntBits)

#define RowBlkAddr(x) \
    ((U32)(x) >> FtlDev.u8_BlkPageCntBits)

#define RowPageIdx(x) \
    ((U32)(x) & FtlDev.u16_BlkPageCntMask)

extern U32 NFTL_IsFreeBlk(U16 u16_PBA);
extern U32 NFTL_IsFreePage(U32 u32_Row);
extern U32 NFTL_IsGoodBlk(U16 u16_PBA);

// --------------------------
// Bad Block
// --------------------------
#define IBB_CLR(PBA)       \
    (pu8_FtlIBBT[((PBA) - FtlDev.u16_StartPBA) >> 3] &= \
    ~(1<<(((PBA) - FtlDev.u16_StartPBA) & 7)))
#define RBB_CLR(PBA)       \
    (pu8_FtlRBBT[((PBA) - FtlDev.u16_StartPBA) >> 3] &= \
    ~(1<<(((PBA) - FtlDev.u16_StartPBA) & 7)))

#define IBB_SET(PBA)       \
    {if(!IBB_CHK(PBA)){(pu8_FtlIBBT[((PBA) - FtlDev.u16_StartPBA) >> 3] |= \
    1<<(((PBA) - FtlDev.u16_StartPBA) & 7));  \
    FtlRoot.Info.u16_IBBCnt++;}}
#define RBB_SET(PBA)       \
    {if(!RBB_CHK(PBA)){(pu8_FtlRBBT[((PBA) - FtlDev.u16_StartPBA) >> 3] |= \
    1<<(((PBA) - FtlDev.u16_StartPBA) & 7));  \
    FtlRoot.Info.u16_RBBCnt++;}}

#define IBB_CHK(PBA)       \
    (pu8_FtlIBBT[((PBA) - FtlDev.u16_StartPBA) >> 3] & \
    1<<(((PBA) - FtlDev.u16_StartPBA) & 7))
#define RBB_CHK(PBA)       \
    (pu8_FtlRBBT[((PBA) - FtlDev.u16_StartPBA) >> 3] & \
    1<<(((PBA) - FtlDev.u16_StartPBA) & 7))

#define IF_BB(PBA)  (IBB_CHK(PBA) || RBB_CHK(PBA))


// --------------------------
// Wear-Leveling
// --------------------------
#define GET_ECNT(PBA)  pu16_PBAECnt[PBA-FtlDev.u16_StartPBA]
#define SET_ECNT(PBA)  pu16_PBAECnt[PBA-FtlDev.u16_StartPBA]++

#define ECNT_TRIG_SAVE       5  // accumulate ECnt to save
#define ECNT_TRIG_WL        30  // ECnt diff to WL
#define WL_MIN_INVALID_PCNT  5

// --------------------------
// DB (DataBlk)
// --------------------------
#define CLR_PBA_KEPT(PBA)       \
    (pu8_IfBlkKept[((PBA) - FtlDev.u16_StartPBA) >> 3] &= \
    ~(1<<(((PBA) - FtlDev.u16_StartPBA) & 7)))

#define SET_PBA_KEPT(PBA)       \
    (pu8_IfBlkKept[((PBA) - FtlDev.u16_StartPBA) >> 3] |= \
    1<<(((PBA) - FtlDev.u16_StartPBA) & 7))

#define CHK_PBA_KEPT(PBA)       \
    (pu8_IfBlkKept[((PBA) - FtlDev.u16_StartPBA) >> 3] & \
    1<<(((PBA) - FtlDev.u16_StartPBA) & 7))

// --------------------------
#define CLR_PBA_InFBQ(PBA)       \
    (pu8_IfBlkInFBQ[((PBA) - FtlDev.u16_StartPBA) >> 3] &= \
    ~(1<<(((PBA) - FtlDev.u16_StartPBA) & 7)))

#define SET_PBA_InFBQ(PBA)       \
    (pu8_IfBlkInFBQ[((PBA) - FtlDev.u16_StartPBA) >> 3] |= \
    1<<(((PBA) - FtlDev.u16_StartPBA) & 7))

#define CHK_PBA_InFBQ(PBA)       \
    (pu8_IfBlkInFBQ[((PBA) - FtlDev.u16_StartPBA) >> 3] & \
    1<<(((PBA) - FtlDev.u16_StartPBA) & 7))

// --------------------------
#define CLR_PBA_IsDB(PBA)       \
    (pu8_IfBlkIsDB[((PBA) - FtlDev.u16_StartPBA) >> 3] &= \
    ~(1<<(((PBA) - FtlDev.u16_StartPBA) & 7)))

#define SET_PBA_IsDB(PBA)       \
    (pu8_IfBlkIsDB[((PBA) - FtlDev.u16_StartPBA) >> 3] |= \
    1<<(((PBA) - FtlDev.u16_StartPBA) & 7))

#define CHK_PBA_IsDB(PBA)       \
    (pu8_IfBlkIsDB[((PBA) - FtlDev.u16_StartPBA) >> 3] & \
    1<<(((PBA) - FtlDev.u16_StartPBA) & 7))


#endif // _FTL_UTL_H_
