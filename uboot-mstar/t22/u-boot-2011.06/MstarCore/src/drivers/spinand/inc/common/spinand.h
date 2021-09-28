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


#ifndef _SPINAND_H_
#define _SPINAND_H_

#include "drvSPINAND.h"

#define USFD_LOGI_PART			0x8000

#define USFD_PART_HWCONFIG		1
#define USFD_PART_BOOTLOGO		2
#define USFD_PART_BL			3
#define USFD_PART_OS			4
#define USFD_PART_CUS			5
#define USFD_PART_UBOOT			6
#define USFD_PART_SECINFO		7
#define USFD_PART_OTP			8
#define USFD_PART_RECOVERY		9
#define USFD_PART_E2PBAK		10
#define USFD_PART_NVRAMBAK		11
#define USFD_PART_APANIC		12
#define USFD_PART_ENV			13
#define USFD_PART_MISC			14
#define USFD_PART_TBL			15
#define USFD_PART_CTRL			16
#define USFD_PART_FDD			17
#define USFD_PART_TDD			18
#define USFD_PART_UBIRO			19

typedef struct
{
	U16 u16_StartBlk;		// the start block index
	U16 u16_BlkCnt;			// project team defined
	U16 u16_PartType;		// project team defined
	U16 u16_BackupBlkCnt;	// reserved good blocks count for backup, UNFD internal use.
							// e.g. u16BackupBlkCnt  = u16BlkCnt * 0.03 + 2
} PARTITION_RECORD_t;

typedef struct
{
	U32 u32_ChkSum;
	U16	u16_SpareByteCnt;
	U16	u16_PageByteCnt;
	U16	u16_BlkPageCnt;
	U16	u16_BlkCnt;
	U16 u16_PartCnt;
	U16 u16_UnitByteCnt;
	PARTITION_RECORD_t records[62];

} PARTITION_INFO_t;

typedef struct _SPI_NAND_DRIVER
{
	SPINAND_FLASH_INFO_t tSpinandInfo;

	PARTITION_INFO_t tPartInfo;
	U8 u8_HasPNI;

	U8 *pu8_pagebuf;
	U8 *pu8_sparebuf;
	U32 u32_column;
	U8 u8_status;

	U8  u8_SectorByteCntBits;
	U8  u8_PageSectorCntBits;
	U8  u8_PageByteCntBits;
	U8  u8_BlkPageCntBits;
	U16 u16_PageSectorCnt;
} SPI_NAND_DRIVER_t;

typedef struct
{
	U8  au8_Tag[16];
	SPINAND_FLASH_INFO_t tSpiNandInfo;
	U8 au8_Reserved[];

} SPINAND_FLASH_INFO_TAG_t;

#define SPINAND_FLASH_INFO_TAG "MSTARSEMIUSFDCIS"

// this definition is used to measure nand read/write speed
#define NAND_SPEED_TEST                     0

extern void dump_mem(unsigned char *buf, int cnt);
#endif

