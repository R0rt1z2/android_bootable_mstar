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
#ifndef _FTLSd_CORE_H_
#define _FTLSd_CORE_H_


// ============================================
// Version Control: change & auto format FTL partition
// ============================================
#define FTLSd_VERSION             1 // check by ftl_root.c (FTLSd_ErrHandle_Root)


// =============================================
// FTL Flags
// =============================================
extern U32 gu32_FtlSdFlag;
#define FTLSd_INIT_READY     BIT0

// =============================================
// Data ID:
// =============================================
#define DID_INFO       0
#define DID_DATA       1
#define DID_FREE       0xFF

typedef UNFD_PACK0 struct _COMM_RDD
{
    U8 u8_BBMark, u8_DID;
    
}UNFD_PACK1 COMM_RDD, *P_COMM_RDD;


// =============================================
#define BBT_BYTECNT_MAX    (64/8) // 8 blocks, or may be less.

typedef struct _FTLSDINFO
{
    COMM_RDD RDD;
    
    volatile U16 u16_FreeLPIdx, PBA;

    U16 u16_BBCnt;
    U8  au8_BBT[BBT_BYTECNT_MAX];

    U32 u32_WCounter;
    
}FTLSD_INFO, *P_FTLSD_INFO;


extern FTLSD_INFO FtlSdInfo;

// =============================================
#if TEMP_CHECK
extern void FTLSd_Temp(void);
extern U8 au8_DataReadBuf[16384];
extern U8 au8_SpareReadBuf[2000];
#endif


extern U32  FTLSd_InitDB(void);
extern U32  FTLSd_ReadData_Ex(U8 *pu8_DataBuf, U32 u32_PCnt);
extern U32  FTLSd_WriteData_Ex(U8 *pu8_DataBuf, U32 u32_PCnt);
extern U32  FTLSd_InitDB(void);
#endif // _FTLSd_CORE_H_

