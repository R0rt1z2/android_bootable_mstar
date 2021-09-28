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

#ifndef _FTL_DEV_H_
#define _FTL_DEV_H_

extern U32 NFTL_CheckMultiPlane(void);

#define PhyPBAFromMultiPlanePBA(PBA)  \
        (((PBA)-FtlRoot.Info.u16_StartMainPBA) * FtlDev.u8_PlaneCnt \
         + FtlRoot.Info.u16_StartMainPBA)
     
#define PhyRowFromMultiPlaneRow(Row)  \
        ((Row) + \
        BlkRowAddr((RowBlkAddr(Row)-FtlRoot.Info.u16_StartMainPBA)* \
        (FtlDev.u8_PlaneCnt-1)))
    

//================================
// 2-plane MLC ID:
#define GET_ID_SetCnt(Vendor)         Vendor##SetCnt

#define GET_ID_ByteStr(Vendor, Idx)   Vendor##IDByteStr(Idx)
#define GET_ID_ByteCnt(Vendor, Idx)   Vendor##IDByteCnt(Idx)
#define GET_ID_PlaneCnt(Vendor, Idx)  Vendor##IDPlaneCnt(Idx)


// --------------------------
#define MicronSetCnt            3 // add Micron MLC, edit this value, and ftl_hal_plane.c
extern U8 MicronIDArray[MicronSetCnt][NAND_ID_BYTE_CNT];
extern U8 MicronIDByteCnt[MicronSetCnt];
extern U8 MicronIDPlaneCnt[MicronSetCnt];

#define MicronIDByteStr(Idx)   &MicronIDArray[Idx][0]    
#define MicronIDByteCnt(Idx)    MicronIDByteCnt[Idx]
#define MicronIDPlaneCnt(Idx)   MicronIDPlaneCnt[Idx]


#define ToshibaSetCnt            1 // add Toshbia MLC, edit this value, and ftl_hal_plane.c
extern U8 ToshibaIDArray[ToshibaSetCnt][NAND_ID_BYTE_CNT];
extern U8 ToshibaIDByteCnt[ToshibaSetCnt];
extern U8 ToshibaIDPlaneCnt[ToshibaSetCnt];

#define ToshibaIDByteStr(Idx)   &ToshibaIDArray[Idx][0]
#define ToshibaIDByteCnt(Idx)    ToshibaIDByteCnt[Idx]
#define ToshibaIDPlaneCnt(Idx)   ToshibaIDPlaneCnt[Idx]

#define HynixSetCnt            1 // add Hynix MLC, edit this value, and ftl_hal_plane.c
extern U8 HynixIDArray[HynixSetCnt][NAND_ID_BYTE_CNT];
extern U8 HynixIDByteCnt[HynixSetCnt];
extern U8 HynixIDPlaneCnt[HynixSetCnt];

#define HynixIDByteStr(Idx)   &HynixIDArray[Idx][0]
#define HynixIDByteCnt(Idx)    HynixIDByteCnt[Idx]
#define HynixIDPlaneCnt(Idx)   HynixIDPlaneCnt[Idx]


//================================
#define FTL_1PBlkCnt()    ((FtlDev.u16_PBACnt-ROOT_BLK_CNT)*FtlDev.u8_PlaneCnt+ROOT_BLK_CNT)

#endif // _FTL_DEV_H_


