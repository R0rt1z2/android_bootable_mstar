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
#ifndef eMMC_DRIVER_H
#define eMMC_DRIVER_H


//===========================================================
// debug macro
//===========================================================


//=====================================================================================
#include "../config/eMMC_config.h" // [CAUTION]: edit eMMC_config.h for your platform
//=====================================================================================
#include "eMMC_err_codes.h"


//===========================================================
// macro for Spec.
//===========================================================
#define ADDRESSING_MODE_BYTE      1 // 1 byte
#define ADDRESSING_MODE_SECTOR    2 // 512 bytes
#define ADDRESSING_MODE_4KB       3 // 4KB bytes

#define eMMC_DEVTYPE_HS400_1_8V   BIT6 // ECSD[196]
#define eMMC_DEVTYPE_HS200_1_8V   BIT4
#define eMMC_DEVTYPE_DDR          BIT2
#define eMMC_DEVTYPE_ALL          (eMMC_DEVTYPE_HS400_1_8V|eMMC_DEVTYPE_HS200_1_8V|eMMC_DEVTYPE_DDR)

#define eMMC_SPEED_OLD            0
#define eMMC_SPEED_HIGH           1
#define eMMC_SPEED_HS200          2
#define eMMC_SPEED_HS400          3

#define eMMC_FLAG_TRIM            BIT0
#define eMMC_FLAG_HPI_CMD12       BIT1
#define eMMC_FLAG_HPI_CMD13       BIT2

//-------------------------------------------------------
// Devices has to be in 512B block length mode by default 
// after power-on, or software reset.
//-------------------------------------------------------
#define eMMC_SECTOR_512BYTE       0x200
#define eMMC_SECTOR_512BYTE_BITS  9
#define eMMC_SECTOR_512BYTE_MASK  (eMMC_SECTOR_512BYTE-1)

#define eMMC_SECTOR_BUF_16KB      (eMMC_SECTOR_512BYTE * 0x20)

#define eMMC_SECTOR_BYTECNT       eMMC_SECTOR_512BYTE
#define eMMC_SECTOR_BYTECNT_BITS  eMMC_SECTOR_512BYTE_BITS
//-------------------------------------------------------

#define eMMC_ExtCSD_SetBit        1
#define eMMC_ExtCSD_ClrBit        2
#define eMMC_ExtCSD_WByte         3

#define eMMC_CMD_BYTE_CNT         5
#define eMMC_R1_BYTE_CNT          5
#define eMMC_R1b_BYTE_CNT         5
#define eMMC_R2_BYTE_CNT          16
#define eMMC_R3_BYTE_CNT          5
#define eMMC_R4_BYTE_CNT          5
#define eMMC_R5_BYTE_CNT          5
#define eMMC_MAX_RSP_BYTE_CNT     eMMC_R2_BYTE_CNT
#define eMMC_MIN_RSP_BYTE_CNT     6

//===========================================================
// driver structures
//===========================================================
#define FCIE_FLAG_GET_PART_INFO   BIT1
#define FCIE_FLAG_RSP_WAIT_D0H    BIT2 // currently only R1b
#define FCIE_FLAG_DDR_MODE        BIT3
#define FCIE_FLAG_DDR_TUNING      BIT4
#define FCIE_FLAG_SPEED_MASK      (BIT5|BIT6)
#define FCIE_FLAG_SPEED_HIGH      BIT5
#define FCIE_FLAG_SPEED_HS200     BIT6
#define FCIE_FLAG_TESTING         BIT7
#define FCIE_FLAG_PADTYPE_MASK    (BIT8|BIT9)
#define FCIE_FLAG_PADTYPE_DDR     BIT8
#define FCIE_FLAG_PADTYPE_SDR     BIT9
#define FCIE_FLAG_PADTYPE_BYPASS  (BIT8|BIT9)
#define FCIE_FLAG_SPEED_HS400     BIT10
#define FCIE_FLAG_WEAK_STRENGTH   BIT11

// ----------------------------------------------
typedef eMMC_PACK0 struct _eMMC_FCIE_DDRT_TABLE {
	U8 u8_SetCnt, u8_CurSetIdx;
	#if defined(ENABLE_eMMC_ATOP) && ENABLE_eMMC_ATOP
	eMMC_FCIE_ATOP_SET_t Set[1];
	#endif
	U32 u32_ChkSum; // put in the last
	U32 u32_VerNo; // for auto update
} eMMC_PACK1 eMMC_FCIE_TIMING_TABLE_t;

#define REG_OP_W    1
#define REG_OP_CLRBIT  2
#define REG_OP_SETBIT  3

typedef eMMC_PACK0 struct _eMMC_FCIE_REG_SET {      //total 10 bytes
	U32 u32_RegAddress;
	U16 u16_RegValue;
	U16 u16_RegMask;
	U16 u16_OpCode;
} eMMC_PACK1 eMMC_FCIE_REG_SET_t;

typedef eMMC_PACK0 struct _eMMC_FCIE_GEN_TIMING_TABLE {
	U32 u32_ChkSum;
	U32 u32_VerNo; // for auto update
	U32 u32_Clk;
	U8 u8_SpeedMode;
	U8 u8_CurSetIdx;
	U8 u8_RegisterCnt;
	U8 u8_SetCnt;
	U8 au8_CID[eMMC_MAX_RSP_BYTE_CNT];
	U32 u32_Dummy[6];      //for extension
	eMMC_FCIE_REG_SET_t RegSet[45]; //at most 45 register set
} eMMC_PACK1 eMMC_FCIE_GEN_TIMING_TABLE_t;

#define eMMC_TIMING_TABLE_CHKSUM_OFFSET  8

typedef struct _eMMC_DRIVER
{
	// ----------------------------------------
    // FCIE
    // ----------------------------------------
	U16 u16_RCA;
	U32 u32_Flag;
	U8  au8_Rsp[eMMC_MIN_RSP_BYTE_CNT];
    U8  au8_CID[eMMC_MAX_RSP_BYTE_CNT];
	U16 u16_Reg10_Mode;

    // ----------------------------------------
    // eMMC
    // ----------------------------------------
	U8  u8_IfSectorMode;
    #if defined(ENABLE_HS400_ENHANCED_STORBE) && ENABLE_HS400_ENHANCED_STORBE
    U8  u8_ECSD184_Stroe_Support;
    #endif

	// ExtCSD
	U8  u8_BUS_WIDTH;
	U8  u8_ECSD196_DevType;
	U8  u8_PadType;

	eMMC_FCIE_TIMING_TABLE_t TimingTable_t;
	eMMC_FCIE_GEN_TIMING_TABLE_t TimingTable_G_t;
} eMMC_DRIVER, *P_eMMC_DRIVER;

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

typedef eMMC_PACK0 struct _eMMC_TEST_ALIGN_PACK {
	U8	u8_0;
	U16	u16_0;
	U32	u32_0, u32_1;
} eMMC_PACK1 eMMC_TEST_ALIGN_PACK_t;

extern U8 g_eMMCDrv_array[];
#if defined(ENABLE_DMA_TO_IMI) && ENABLE_DMA_TO_IMI
extern char gu8_eMMC_str[];
#endif
//===========================================================
// exposed APIs
//===========================================================
#include "../api/drv_eMMC.h"

//===========================================================
// internal used functions
//===========================================================
#include "eMMC_utl.h"
#include "eMMC_hal.h"

#define eMMC_RPMB_REQ_AUTH_KEY         0x01
#define eMMC_RPMB_REQ_WRITE_CNT_VAL    0x02
#define eMMC_RPMB_REQ_AUTH_DATA_W      0x03
#define eMMC_RPMB_REQ_AUTH_DATA_R      0x04
#define eMMC_RPMB_REQ_RESULT_R         0x05

#endif // eMMC_DRIVER_H

