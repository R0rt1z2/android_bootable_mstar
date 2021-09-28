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

/**
* @version
* @brief
*
*/

#ifndef __DRV_eMMC_H__
#define __DRV_eMMC_H__

#ifndef U32
#define U32  unsigned long
#endif
#ifndef U16
#define U16  unsigned short
#endif
#ifndef U8
#define U8   unsigned char
#endif
#ifndef S32
#define S32  signed long
#endif
#ifndef S16
#define S16  signed short
#endif
#ifndef S8
#define S8   signed char
#endif

/*=============================================================*/
// Include files
/*=============================================================*/
#include "../config/eMMC_config.h"


/*=============================================================*/
// Macro definition
/*=============================================================*/

/*=============================================================*/
// Data type definition
/*=============================================================*/

/*=============================================================*/
// Variable definition
/*=============================================================*/

/*=============================================================*/
// Global function definition
/*=============================================================*/
extern U32 eMMC_LoadImages(U32 u32_Addr, U32 u32_ByteCnt, U32 u32_BlkAddr);
//--------------------------------------------
// CAUTION: u32_DataByteCnt has to be 512B x n
//--------------------------------------------
extern U32  eMMC_ReadData_CIFD(U8* pu8_DataBuf, U32 u32_DataByteCnt, U32 u32_BlkAddr);
extern U32  eMMC_ReadData_IMI(U8* pu8_DataBuf, U32 u32_DataByteCnt, U32 u32_BlkAddr);
extern U32  eMMC_ReadData_MIU(U8* pu8_DataBuf, U32 u32_DataByteCnt, U32 u32_BlkAddr);
extern U32  eMMC_WriteData_MIU(U8* pu8_DataBuf, U32 u32_DataByteCnt, U32 u32_BlkAddr);
extern U32  eMMC_LoadTimingTable(U8 u8_PadType);
extern U32  eMMC_FCIE_EnableFastMode_Ex(U8 u8_PadType);
extern U32  eMMC_FCIE_EnableFastMode(U8 u8_PadType);
extern U32  eMMC_FCIE_ChooseSpeedMode(void);
//--------------------------------------------

extern U32 eMMC_GetExtCSD(U8* pu8_Ext_CSD);
extern U32 eMMC_SetExtCSD(U8 u8_AccessMode, U8 u8_ByteIdx, U8 u8_Value);

/*=============================================================*/
// internal function definition
/*=============================================================*/
extern U32  eMMC_ReadBootPart(U8* pu8_DataBuf, U32 u32_DataByteCnt, U32 u32_BlkAddr);
extern U32  eMMC_CheckIfReady(void);
extern void eMMC_FCIE_Apply_Reg(void);
extern void eMMC_FCIE_SetATopTimingReg(void);
extern  U32 eMMC_CMD18_RPMB_MIU(U32 u32_eMMCBlkAddr, U8 *pu8_DataBuf, U16 u16_BlkCnt);
extern  U32 eMMC_CMD23_RPMB(U16 u16_BlkCnt, U8 u8_Is_Reliable);
extern  U32 eMMC_CMD25_RPMB_MIU(U32 u32_eMMCBlkAddr, U8 *pu8_DataBuf, U16 u16_BlkCnt, U8 u8_Is_Reliable);

extern  void eMMC_SetError(U16 ErrCode, U16 Pos);
extern  void *eMMC_get_RPMBContext_address(void);
extern  void eMMC_GetCID(U8 *pu8_CID);
extern  U32  GetHmacKey(U32 u32KeyBufPA, U32 u32key_len, U8* key);
extern  void MDrv_AESDMA_Hmac_sha256(const U8 *text, U32 text_len, const U8 *key, U32 key_len, void *digest);
extern  U32  eMMC_RPMB_Get_Counter(volatile U32 *pu32_WCnt);
extern  U32  eMMC_RPMB_IfKeyWritten(void);
extern  U32  eMMC_RPMB_Read_Result(U16 *pu16_Result);
extern  void eMMC_RPMB_Copy_RESULT(U8 *u8_dest,U8 *u8_src, U32 u32_cnt);
extern  U32  eMMC_RPMB_CheckResultError(void);
extern  void eMMC_RPMB_SWAP_memcpy(U8 *u8_dest,U8 *u8_src, U32 u32_cnt);
extern  U32  eMMC_ComapreData(U8 *pu8_Buf0, U8 *pu8_Buf1, U32 u32_ByteCnt);
extern  U32  eMMC_RPMB_program_auth_key(U8* key);
extern  U32  eMMC_RPMB_Test(U8* key);
extern  U32  eMMC_RPMB_Verify_Hmac(U8* pu8_DataBuf, U16 u16_BlkCnt, U8* key);
extern  U32  eMMC_RPMB_Read_data(U8* pu8_DataBuf, U32 u32_DataByteCnt, U16 u16_BlkAddr, U8* key);
extern  U32  eMMC_RPMB_Write_data(U8* pu8_DataBuf, U32 u32_DataByteCnt, U16 u16_BlkAddr, U8* key);
extern  U32  eMMC_RPMB_IfKeyWritten(void);
extern  U32  eMMC_RPMB_NoDataReq(void);
#endif //__DRV_eMMC_H__

