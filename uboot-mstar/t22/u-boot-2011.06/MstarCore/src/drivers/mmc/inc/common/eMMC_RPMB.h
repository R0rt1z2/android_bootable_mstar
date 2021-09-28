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
#ifndef eMMC_RPMB
#define eMMC_RPMB

#include "eMMC.h"

#define eMMC_RPMB_REQ_AUTH_KEY         0x0001
#define eMMC_RPMB_REQ_WRITE_CNT_VAL    0x0002
#define eMMC_RPMB_REQ_AUTH_DATA_W      0x0003
#define eMMC_RPMB_REQ_AUTH_DATA_R      0x0004
#define eMMC_RPMB_REQ_RESULT_R         0x0005

#define eMMC_RPMB_RSP_AUTH_KEY         0x0100
#define eMMC_RPMB_RSP_WRITE_CNT_VAL    0x0200
#define eMMC_RPMB_RSP_AUTH_DATA_W      0x0300
#define eMMC_RPMB_RSP_AUTH_DATA_R      0x0400

typedef eMMC_PACK0 struct _eMMC_RPMB_DATA
{
   U16 u16_req_rsp;
   U16 u16_result;
   U16 u16_blk_cnt;
   U16 u16_addr;
   U32 u32_writecnt;
   U8  u8_nonce[16];
   U8  u8_data[256];
   U8  u8_auth_key[32];
   U8  u8_stuff[196];
}eMMC_PACK1 eMMC_RPMB_DATA;
extern int GetHmacKey(unsigned char *u8pHmacKey);
extern U32 eMMC_RPMB_program_auth_key(void);
extern U32 eMMC_RPMB_Get_Counter_for_cmd(U32 *pu32_WCnt);
extern U32 eMMC_RPMB_Get_Counter(volatile U32 *pu32_WCnt);
extern U32 eMMC_RPMB_Read_Result(U16 *pu16_Result);
extern U32 eMMC_RPMB_NoDataReq(void);
extern U32 eMMC_RPMB_IfKeyWritten(void);
extern U32 eMMC_RPMB_Verify_Hmac(U8* pu8_DataBuf, U16 u16_BlkCnt);
extern U32 eMMC_RPMB_Write_data(U8* pu8_DataBuf, U32 u32_DataByteCnt, U16 u16_blk_addr);
extern U32 eMMC_RPMB_Read_data(U8* pu8_DataBuf, U32 u32_DataByteCnt, U16 u16_blk_addr);
extern U32 eMMC_RPMB_Read_Blk(U8 *pu8_DataBuf, U8 *pu8_nonce, U16 u16_BlkAddr);
extern U32 eMMC_RPMB_Write_Blk(U8 *pu8_DataBuf);
extern U32 eMMC_RPMB_Test(void);
#endif // eMMC_RPMB
