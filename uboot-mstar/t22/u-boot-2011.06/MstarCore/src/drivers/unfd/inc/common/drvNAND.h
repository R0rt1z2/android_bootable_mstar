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

#ifndef NAND_DRV_V3
#define NAND_DRV_V3

#ifndef U64
#define U64  unsigned long long
#endif
#ifndef U32
#define U32  unsigned int
#endif
#ifndef U16
#define U16  unsigned short
#endif
#ifndef U8
#define U8   unsigned char
#endif
#ifndef S64
#define S64  signed long long
#endif
#ifndef S32
#define S32  signed int
#endif
#ifndef S16
#define S16  signed short
#endif
#ifndef S8
#define S8   signed char
#endif
#ifndef NULL
#define NULL (void*)0
#endif
#ifndef FALSE
#define FALSE   0
#endif
#ifndef TRUE
#define TRUE    1
#endif

#ifndef UINT16
#define UINT16 U16
#endif
#ifndef UINT32
#define UINT32 U32
#endif
#ifndef UINT64
#define UINT64 U64
#endif



#ifdef BIG_ENDIAN // project team defined macro
#define cpu2le64(x) SWAP64((x))
#define le2cpu64(x) SWAP64((x))
#define cpu2le32(x) SWAP32((x))
#define le2cpu32(x) SWAP32((x))
#define cpu2le16(x) SWAP16((x))
#define le2cpu16(x) SWAP16((x))
#define cpu2be64(x) ((UINT64)(x))
#define be2cpu64(x) ((UINT64)(x))
#define cpu2be32(x) ((UINT32)(x))
#define be2cpu32(x) ((UINT32)(x))
#define cpu2be16(x) ((UINT16)(x))
#define be2cpu16(x) ((UINT16)(x))
#else // Little_Endian
#define cpu2le64(x) ((UINT64)(x))
#define le2cpu64(x) ((UINT64)(x))
#define cpu2le32(x) ((UINT32)(x))
#define le2cpu32(x) ((UINT32)(x))
#define cpu2le16(x) ((UINT16)(x))
#define le2cpu16(x) ((UINT16)(x))
#define cpu2be64(x) SWAP64((x))
#define be2cpu64(x) SWAP64((x))
#define cpu2be32(x) SWAP32((x))
#define be2cpu32(x) SWAP32((x))
#define cpu2be16(x) SWAP16((x))
#define be2cpu16(x) SWAP16((x))
#endif

//=====================================================================================
#include "drvNAND_config.h" // [CAUTION]: edit drvNAND_config.h for your platform
//=====================================================================================
#include "drvNAND_err_codes.h"

#define NAND_DRIVER_VERSION   1 // used to sync with other SW stages (e.g. linux kernel)

//===========================================================
// debug macro
//===========================================================
#define UNFD_FTL_RD_TEST     0
#define UNFD_FTL_WL_TEST     0

#define UNFD_API_DEBUG       0
#define UNFD_FTL_RW_DEBUG    0
#define UNFD_FTL_BREAK_TEST  0

#define UNFD_PRINT_EC        0

#if UNFD_API_DEBUG
#define unfd_api_debug(x)    x
#else
#define unfd_api_debug(X)
#endif

#if UNFD_FTL_RW_DEBUG
#define ftl_rw_debug(x)      x
#else
#define ftl_rw_debug(X)
#endif

#if UNFD_FTL_BREAK_TEST
#define FTL_BREAK_PT_ID      17 // current max:17
#define ftl_break_pt(ID)     if(FTL_BREAK_PT_ID == ID)  {nand_debug(0,1,"ftl_break_pt(%u)",ID);  nand_stop();}
#else
#define FTL_BREAK_PT_ID      -1
#define ftl_break_pt(ID)
#endif

//------------------------------------
#define UNFD_FTL_PHY_GUARD      0
#define UNFD_FTL_MG_TEST        0
#define UNFD_FTL_SPEED_UP       0
#define UNFD_FTL_CHECK_ERROR    1

#if UNFD_FTL_PHY_GUARD
#define UNFD_FTL_MEM_GUARD      1
#else
#define UNFD_FTL_MEM_GUARD      0
#endif
#if UNFD_FTL_SPEED_UP
#undef  UNFD_FTL_CHECK_ERROR
#define UNFD_FTL_CHECK_ERROR    0
#endif

//------------------------------------

#define INTER_CMD		0
#define ACT_CMD			1
#define ADDR_CYCLE_CMD	2
#define ACT_DMA_CMD		3			//ACT_SER_DOUT, ACT_SER_DIN,
#define ACT_RAN_CMD		4			//ACT_RAN_DIN, ACT_RAN_DOUT
#define CUST_CMD		5
#define RAN_BYTE_CMD	6

#define NAND_BBT_PAGE_COUNT 8


//===========================================================
// Partition Info parameters
//===========================================================
#define UNFD_LOGI_PART    0x8000 // bit-or if the partition needs Wear-Leveling
#define UNFD_HIDDEN_PART  0x4000 // bit-or if this partition is hidden, normally it is set for the LOGI PARTs.

#define UNFD_PART_HWCONFIG		1
#define UNFD_PART_BOOTLOGO      2
#define UNFD_PART_BL            3
#define UNFD_PART_OS            4
#define UNFD_PART_CUS           5
#define UNFD_PART_UBOOT         6
#define UNFD_PART_SECINFO	    7
#define UNFD_PART_OTP           8
#define UNFD_PART_RECOVERY      9
#define UNFD_PART_E2PBAK        10
#define UNFD_PART_NVRAMBAK      11
#define UNFD_PART_APANIC        12
#define UNFD_PART_ENV           13
#define UNFD_PART_MISC          14
#define UNFD_PART_TBL           15
#define UNFD_PART_CTRL          16
#define UNFD_PART_FDD           17
#define UNFD_PART_TDD           18
#define UNFD_PART_UBIRO         19
#define UNFD_PART_OPTEE         20
#define UNFD_PART_ARMFW         21
#define UNFD_PART_TEEKB         22

#if defined(__VER_UNFD_FTL__)
#define UNFD_PART_PHY_POS       (1+1)

#include "drvNAND_ftl.h"
#include "drvNAND_task.h"
//Following is the Logical partition
#define UNFD_PART_FAT			(0|UNFD_LOGI_PART)
#define UNFD_PART_FTL           (1|UNFD_LOGI_PART)
#endif

typedef UNFD_PACK0 struct _PARTITION_RECORD {

	U16 u16_StartBlk;     // the start block index, reserved for UNFD internal use.
	U16 u16_BlkCnt;       // project team defined
	U16 u16_PartType;     // project team defined, e.g. UNFD_PART_XXX_0
	U16 u16_BackupBlkCnt; // reserved good blocks count for backup, UNFD internal use.
                          // e.g. u16BackupBlkCnt  = u16BlkCnt * 0.03 + 2
} UNFD_PACK1 PARTITION_RECORD_t, *P_PARTITION_RECORD_t;

typedef UNFD_PACK0 struct _PARTITION_INFO {

	U32 u32_ChkSum;
	U16	u16_SpareByteCnt;
	U16	u16_PageByteCnt;
	U16	u16_BlkPageCnt;
	U16	u16_BlkCnt;
	U16 u16_PartCnt;
	U16 u16_UnitByteCnt;
	PARTITION_RECORD_t records[];

} UNFD_PACK1 PARTITION_INFO_t;

typedef UNFD_PACK0 struct _DDR_TIMING_GROUP {

	U8	u8_ClkIdx;
	U8	u8_DqsMode;
	U8	u8_DelayCell;
	U8	u8_DdrTiming;

} UNFD_PACK1 DDR_TIMING_GROUP_t;

typedef UNFD_PACK0 struct _NAND_FLASH_INFO {

	U8  au8_Tag[16];
	U8  u8_IDByteCnt;
	U8  au8_ID[15];
	U32 u32_ChkSum;
	U16 u16_SpareByteCnt;
	U16 u16_PageByteCnt;
	U16 u16_BlkPageCnt;
	U16 u16_BlkCnt;
	U32 u32_Config;
	U16 u16_ECCType;
	U16	u16_tRC;
	U8  u8_tRP;
	U8  u8_tREH;
	U8  u8_tREA;
	U8  u8_tRR;
	U16 u16_tADL;
	U16 u16_tRHW;
	U16 u16_tWHR;
	U16 u16_InitBadBlkCnt;
	U8  u8_Vendor[16];
	U8  u8_PartNumber[16];
	U8  u8_PairPageMapLoc;
	U8  u8_PairPageMapType;
	U16 u16_tCCS;
	U8  u8_tCS;
	//for TV ROM Code
	U8  u8_BL0PBA;
	U8  u8_BL1PBA;
	U8  u8_UBOOTPBA;
	U16	u16_tWC;
	U8  u8_tWP;
	U8  u8_tWH;
	U16 u16_tCWAW;
	U8 u8_tCLHZ;
	U8 u8_AddrCycleIdx;
	DDR_TIMING_GROUP_t tDefaultDDR;
	DDR_TIMING_GROUP_t tMaxDDR;
	DDR_TIMING_GROUP_t tMinDDR;
	U8  u8_HashPBA[3][2];
	U16  u16_tWW;
	U8	u8_ReadRetryType;
	U8  u8_BitflipThreshold;
	U8	u8_Hash0PageIdx;
	U8	u8_Hash1PageIdx;
	U32	 u32_BootSize;	
	U16	u16_BBtPageCheckCount;
	U16	u16_BBtPageIdx[NAND_BBT_PAGE_COUNT];
	U16	u16_BBtMarker[NAND_BBT_PAGE_COUNT];
	U16 u16_PhySpareByteCnt;
} UNFD_PACK1 NAND_FLASH_INFO_t;


typedef UNFD_PACK0 struct _MLC_BACKUP_BLK_SPARE_INFO
{
	U8 u8_BadBlkMarker;
	U16 u16_BackupPBA;
} UNFD_PACK1 MBBS_INFO_t;


typedef UNFD_PACK0 struct _BlK_INFO {

	U8  u8_BadBlkMark;
	U8  u8_PartType;
	U16 u16_BlkAddr;
	U8  au8_Misc[2];
	U8  au8_ECC[10];

} UNFD_PACK1 BLK_INFO_t;


typedef UNFD_PACK0 struct _INIT_BBT {

	U32 u32_ChkSum;
	U16 u16_Cnt;
	U8  au8_BadBlkTbl[];

} UNFD_PACK1 INIT_BBT_t;


typedef UNFD_PACK0 struct _MIU_RECORD {

	U32 u32_RegAddr;
	U32 u32_RegValue;

} UNFD_PACK1 MIU_RECORD_t;

typedef UNFD_PACK0 struct _MIU_PART {

	U32 u32_ChkSum;
	U32 u32_ByteCnt;

	MIU_RECORD_t records[];

} UNFD_PACK1 MIU_PART_t;


/*
BIT :[63:32]	|   [31: 13]	|   [12: 3]	|   [2:1]	|    [0]
     MIU_          Reserved      Page_         MIU_        End_
     Address       Count         SEL           Flag
*/
typedef  struct  _ADMA_DESC{
	U32 u32_EndFlag : 1;
	U32 u32_MIUSel  : 2;
	U32 u32_PageCnt : 10;
	U32 			: 3;
	U32 u32_SectorCnt : 16; ;
	U32 u32_Address;
} ADMA_DESC_t;


typedef  struct  _ZDEC_DESC_{
	U32 u32_EndFlag : 1;
	U32 u32_MIUSel  : 2;
	U32 u32_PageCnt : 10;
	U32 			: 3;
	U32 u32_SectorCnt : 16;    
	U32 u32_Address;
} ZDecDescriptor_t;


#define TSB_REGISTER_NUMBER		 4
#define TSB_READRETRY_TIME		 10
#define SAMSUNG_REGISTER_NUMBER  4
#define SAMSUNG_READRETRY_TIME	 15
#define MICRON_REIGSTER_NUMBER	 4
#define MCIRON_READRETRY_TIME	 8 // 8 = disable + 7 options

#define HYNIX20nm32GBMLC_REGISTER_NUMBER	8
#define HYNIX20nm32GBMLC_READRETRY_TIME		8

#define HYNIX16nm64GBMLC_FDie_READRETRY_TIME	 8
#define HYNIX16nm64GBMLC_FDie_REGISTER_NUMBER	 4

typedef UNFD_PACK0 struct _READ_RETRY {
	U8  u8_CustRegNo;
	U8* pu8_CustRegTable;
	U8  u8_DefaultValueOffset;
	U8  u8_MaxRetryTime;
	U8  u8_ByteLenPerCmd;
	U8  ***pppu8_RetryRegValue;
	U8  u8_DefaultValueRestore;
	U8  u8_SetCmdLen;
	U8* pu8_SetCmdTypeSeq;
	U8* pu8_SetCmdValueSeq;
	U8  u8_GetCmdLen;
	U8* pu8_GetCmdTypeSeq;
	U8* pu8_GetCmdValueSeq;
} UNFD_PACK1 READ_RETRY_t;


//===========================================================
// Logical Disk Info
//===========================================================
typedef struct _DISK_INFO {

	U32 u32_StartSector;
	U32 u32_SectorCnt;
} DISK_INFO_t, *P_DISK_INFO_t;

#define UNFD_MAX_DISK_NUM    4//16 // [CAUTION]: max logical partition count


//===========================================================
// driver parameters
//===========================================================
#define NAND_ID_BYTE_CNT     15

#define R_SEQUENCE_003A      1
#define R_SEQUENCE_004A      2
#define R_SEQUENCE_004A30    3
#define R_SEQUENCE_005A30    4

//bit14:13 of u32_Config, 00:Normal, 01:Cache, 10:MultiP, 11:MultiP cache
#define UNFD_RW_NORMAL				0
#define UNFD_RW_CACHE				1
#define UNFD_RW_MULTIPLANE			2
#define UNFD_RW_MULTIPLANE_CACHE	3

#define NAND_CellType_SLC    0
#define NAND_CellType_MLC    1

typedef struct _NAND_DRIVER
{
	//-------------
	U16 u16_mem_w0; // MEM_GUARD_W0
	//-------------

	U8  au8_ID[NAND_ID_BYTE_CNT];
	U8  u8_IDByteCnt;

	U16  u16_bbm_wa;

	U8  u8_CISBlk;
	U8  u8_CIS0_EccStatus;

	//-----------------------------
	// HAL parameters
	//-----------------------------
    #if (defined(NC_SEL_FCIE3) && NC_SEL_FCIE3) || (defined(NC_SEL_FCIE5) && NC_SEL_FCIE5)
	volatile U16 u16_Reg1B_SdioCtrl;
	volatile U16 u16_Reg40_Signal;
	volatile U16 u16_Reg48_Spare;
	volatile U16 u16_Reg49_SpareSize;
	volatile U16 u16_Reg50_EccCtrl;
	volatile U16 u16_Reg57_RELatch;

	volatile U16 u16_Reg2C_SMStatus;
	volatile U16 u16_Reg58_DDRCtrl;
	volatile U16 u16_Reg5A_tWHR_tCLHZ;
	volatile U16 u16_Reg5D_tCWAW_tADL;
	#endif
	
	#if defined(NC_SEL_FCIE5) && NC_SEL_FCIE5
	volatile U16 u16_EmmcPllReg62_LatWin;
	volatile U16 u16_EmmcPllReg6c_DqsSel;
	volatile U16 u16_EmmcPllReg6D_DdrSdrCtrl;
	volatile U16 u16_EmmcPllReg09_PhaseSel;
	#endif

	#if defined(NC_HWCMD_DELAY) && NC_HWCMD_DELAY
	volatile U16 u16_Reg56_Rand_W_Cmd;
	#endif

	#if (defined(FCIE_LFSR) && FCIE_LFSR) || (defined(NC_TRR_TCS) && NC_TRR_TCS)
	volatile U16 u16_Reg59_LFSRCtrl;
	#endif

	#if defined(FCIE4_DDR) && FCIE4_DDR || defined(DDR_NAND_SUPPORT) && DDR_NAND_SUPPORT
	DDR_TIMING_GROUP_t tDefaultDDR;
	DDR_TIMING_GROUP_t tMaxDDR;
	DDR_TIMING_GROUP_t tMinDDR;
	#endif

	#if defined(REG_ANALOG_DELAY_CELL)
	volatile U16 u16_Analog_dqs_delaycell;
	#endif

    #if defined(FCIE4_DDR_EMMC_PLL) && FCIE4_DDR_EMMC_PLL
    U16 u16_Emmc_Pll_Reg09;
    #endif

	U8	u8_PadMode;
	U8	u8_HasPNI;
	#if defined(NC_SEL_FCIE5) && NC_SEL_FCIE5
	U8	u8_MacroType;
	#endif
	U32 u32_Clk;
	U32 u32_minClk;
	//-----------------------------
	// NAND Info (listed by their init_sequence)
	//-----------------------------
	U8  u8_WordMode;
	U8  u8_OpCode_RW_AdrCycle;
	U8  u8_OpCode_Erase_AdrCycle;
	U8  u8_Flag_004A30;
	U16 u16_One_Col_Addr;

	U16 u16_BlkCnt;
	U16 u16_BlkPageCnt;
	U16 u16_PageByteCnt;
	U16 u16_SpareByteCnt;
	U16 u16_BitflipThreshold;
	U16 u16_ECCCorretableBit;
	U16 u16_ECCType;
	U32 u32_Config;
	U16	u16_tRC;
	U8  u8_tRP;
	U8  u8_tREH;
	U8  u8_tREA;
	U8  u8_tRR;
	U16 u16_tADL;
	U16 u16_tRHW;
	U16 u16_tWHR;
	U16 u16_tCCS;
	U8  u8_tCS;
	U16	u16_tWC;
	U8  u8_tWP;
	U8  u8_tWH;
	U16 u16_tCWAW;
	U8	u8_tCLHZ;
	U8 	u8_AddrCycleIdx;
	U16  u16_tWW;
	U8  u8_Vendor[16];
	U8  u8_PartNumber[16];

	U8  u8_CellType;	// bit0 of u32_Config, 0:SLC, 1:MLC
	U8  u8_BadBlkMarker;// bit3:1 of u32_Config, 0:Column0, ...
	U8  u8_PlaneCnt;	// bit6:4 of u32_Config, 0:P1, 1:P2, ...
	U8	u8_RequireRandomizer;	// bit8		of u32_Config, 0: disable, 1: enable
	U8	u8_NANDInterface;//bit10:9 of u32_Config, 0:SDR 1:Toggle 2: ONFI
	U8  u8_CacheProgram;    // bit12:11 of u32_Config, 0 normal, 1 cache, 2 multi-plane, 3 multi-plane cache
	U8  u8_CacheRead;       // bit14:13 of u32_Config, 0 normal, 1 cache, 2 multi-plane, 3 multi-plane cache
	U8  u8_RequireReadRetry;	// bit15	of u32_Config, 0: no retry 1: read retry
	U8  u8_MaxCENum;        // bit 19:18 of u32_Config, 0:CE 0   1:CE 0/1   2:CE 0/1/2   3:CE 0/1/2/3
	U8  u8_PairPageMapLoc;
	U16 u16_BlkSectorCnt;
	U16 u16_PageSectorCnt;
	U16 u16_SectorByteCnt;
	U16 u16_SectorSpareByteCnt;
	U16 u16_ECCCodeByteCnt;

	U8  u8_BlkPageCntBits;
	U8  u8_BlkSectorCntBits;
	U8  u8_PageByteCntBits;
	//U8  u8_SpareByteCntBits;
	U8  u8_PageSectorCntBits;
	U8  u8_SectorByteCntBits;
	//U8  u8_SectorSpareByteCntBits;

	U16 u16_BlkPageCntMask;
	U16 u16_BlkSectorCntMask;
	U16 u16_PageByteCntMask;
	//U16 u16_SpareByteCntMask;
	U16 u16_PageSectorCntMask;
	U16 u16_SectorByteCntMask;

	U16 u16_CISPBA[2];
	U8  u8_BL0PBA;
	U8  u8_BL1PBA;
	U8  u8_UBOOTPBA;
	U8  u8_HashPBA[3][2];
	U8	u8_Hash0PageIdx;
	U8	u8_Hash1PageIdx;
	U32	u32_BootSize;
	U16	u16_BBtPageCheckCount;
	U16	u16_BBtPageIdx[NAND_BBT_PAGE_COUNT];
	U16	u16_BBtMarker[NAND_BBT_PAGE_COUNT];
	U16 u16_PhySpareByteCnt;

    U16 u16_BlkLowPCnt;
	U8  u8_SlcMode;
	//-------------
	U16 u16_mem_w1; // MEM_GUARD_W1
	//-------------

    U8 *pu8_PageSpareBuf;
    U8 *pu8_PageDataBuf;
	//-----------------------------
	// Partition Info
	//-----------------------------
	PARTITION_INFO_t *pPartInfo;

	//-----------------------------
	// Read Retry Info
	//-----------------------------
	U8  u8_ReadRetryType;
	READ_RETRY_t ReadRetry_t;
	U8 **ppu8_ReadRetryDefault;

	#if defined(__VER_UNFD_FTL__) && __VER_UNFD_FTL__
	//-----------------------------
	// UNFD parameters
	//-----------------------------
	DISK_INFO_t DiskInfo_t[UNFD_MAX_DISK_NUM];
	U16 u16_FirstPBA_AssertDebug;
	U16 u16_FirstPBA_LogiArea, u16_LastPBA_LogiArea;
	U16 u16_RootBlkPhyIdx;
	U16 u16_LogiDataBlkCnt, u16_LogiFreeBlkCnt, u16_BadBlkCnt;

	//-------------
	U16 u16_mem_w2; // MEM_GUARD_W2
	//-------------

	// for first zone (the first 1000 logi blocks)
	U16 u16_FirstPBA_Zone0; // = u16_FirstPBA_LogiArea + UNFD_ROOT_BLK_CNT
	U16 au16_L2P_0[LUT_PBA_CNT+BACKUP_BBCNT(LUT_PBA_CNT)+UNFD_Z0_FREE_BLK_CNT];

	#if UNFD_FTL_FMAP
    U16 au16_FBIdx_0[BACKUP_BBCNT(LUT_PBA_CNT)+UNFD_Z0_FREE_BLK_CNT+1 + FMAP_MAX_LBA_CNT*FMAP_SPACE_FACTOR];
	#else
	U16 au16_FBIdx_0[BACKUP_BBCNT(LUT_PBA_CNT)+UNFD_Z0_FREE_BLK_CNT+1]; // free blk must be physical index
	#endif

	U8  u8_FBHead_0, u8_FBTail_0, u8_FBCnt_0, u8_FBMaxCnt_0;
	U16 u16_Zone0LBACnt;

	//-------------
	U16 u16_mem_w3; // MEM_GUARD_W3
	//-------------

	// for the folowing zones
	U16 u16_FirstPBA_Zone1;
	U16 au16_ActiveZoneIdx[MAX_LUT_CNT-1];
	U16 au16_L2P_1[(MAX_LUT_CNT-1)*LUT_PBA_CNT+BACKUP_BBCNT((MAX_LUT_CNT-1)*LUT_PBA_CNT)+UNFD_Z1_FREE_BLK_CNT+1];
	U16 au16_FBIdx_1[UNFD_FREE_BLK_MAX_CNT]; // free blk must be physical index
	U8  u8_FBHead_1, u8_FBTail_1, u8_FBCnt_1, u8_FBMaxCnt_1;
	U8  u8_Zone1SubZoneCnt, u8_Zone1LUTFlag;

	//-------------
	U16 u16_mem_w4; // MEM_GUARD_W4
	//-------------

	#if UNFD_FTL_WBQ
	WBQ_t aWBQ_t[MAX_WBQ_CNT];
	U16   u16_PPB, u16_PPB_Page;
	#endif

	//-------------
	U16 u16_mem_w5; // MEM_GUARD_W5
	//-------------

	#if UNFD_FTL_FMAP
	U16 u16_FM_FirstLBA, u16_FM_LBACnt, u16_FM_PBACnt;
	U8  u8_FM_PBAPageCnt;
	// R: use au16_FM_BL2P + au8_FM_PL2P
	// W: use au16_FM_BL2P + au8_FM_FreePagePos,
	//    update au16_FM_BL2P, au8_FM_PL2P, au8_FM_FreePagePos
	U16 au16_FM_BL2P[FMAP_MAX_LBA_CNT][FMAP_SPACE_FACTOR]; // value: PBA
	U8  au8_FM_FreePagePos[FMAP_MAX_LBA_CNT][FMAP_SPACE_FACTOR]; // value: 0 ~ BlkPageCnt
	U8  au8_FM_PL2P[FMAP_MAX_LBA_CNT][FMAP_MAX_BLKPAGE_CNT]; // value: 0 ~ BlkPageCnt
	U8  u8_FM_init;
	#endif

	//-------------
	U16 u16_mem_w6; // MEM_GUARD_W6
	//-------------

	#if UNFD_FTL_RD || NAND_BLD_LD_OS
	volatile U16 u16_RD_SrcPBA; // source blk
	volatile U8  u8_RD_ECCBitsCnt, u8_RD_ActiveFlag;
	U16 u16_RDB, u16_RDB_Page; // store RD info for physical area

	#if (defined(BLD_LD_OS_RD)&&BLD_LD_OS_RD)
	U16 u16_RD_FreeBlk;
	#endif

	#if UNFD_FTL_RD_TEST
	U16 u16_RD_TestCnt;
	#endif
	#endif

	//-------------
	U16 u16_mem_w7; // MEM_GUARD_W7
	//-------------

	#if UNFD_FTL_WL
	U16 au16_WL_ECntPBA[MAX_LUT_CNT];
	U16 au16_WL_ECntPBA_Page[MAX_LUT_CNT];
	U32 au32_ZoneTotalECnt[MAX_LUT_CNT];

	#if UNFD_FTL_WL_TEST
	U16 u16_WL_TestCnt;
	#endif
	#endif
	#endif
	//-----------------------------
	#if IF_IP_VERIFY
	U8 u8_IfECCTesting;
	#endif

	volatile U16 u16_Test0, u16_Test1;

	//-------------
	volatile U32 u32_mem_stack_adr,  u32_mem_stack_dat;
	volatile U32 u32_mem_stack1_adr, u32_mem_stack1_dat;

	U16 u16_mem_w8; // MEM_GUARD_W8
	//-------------
} NAND_DRIVER, *P_NAND_DRIVER;


//===========================================================
// exposed APIs
//===========================================================
// pub module header
#include "drv_unfd.h"

#if defined(CONFIG_MSTAR_FTL_SD)
#include "../../ftl_sd/inc/ftlsd_api.h"
#endif

extern int MDrv_Nand_init(unsigned char *nniData, unsigned char *pniData);
extern U32 drvNAND_GetPartitionCnt(void);
extern U32 drvNAND_GetPartitionInfo(PARTITION_INFO_t *pPartInfo_t);
extern U32 drvNAND_WritePartition(U16 u16_PartType, U32 u32_StartPhySector,
                U32 u32_SrcPhyAddr, U32 u32_SectorCnt);
extern U32 drvNAND_ReadPartition(U16 u16_PartType, U32 u32_StartPhySector,
                U32 u32_SrcPhyAddr, U32 u32_SectorCnt);
extern U32 drvNAND_ErasePartition(U16 u16_PartType);
extern U32 drvNAND_GetNandInfo(NAND_INFO_t *pNandInfo_t);
#if UNFD_FTL_WL
extern U32 drvNAND_WearLeveling(void);
extern U32 drvNAND_WearLeveling1(void);
extern U32 drvNAND_WL_SaveECnt(U8 u8_ECntZoneIndex);
#endif
extern U32 drvNAND_FlushAll(void);

//===========================================================
// internal used functions
//===========================================================
#include "drvNAND_utl.h"
extern void *drvNAND_get_DrvContext_address(void);
extern void *drvNAND_get_DrvContext_PartInfo(void);
extern U32 drvNAND_ChkSum(U8 *pu8_Data, U16 u16_ByteCnt);

//---------------------------------------
// HAL functions
//---------------------------------------
extern U32  NC_Init(void);
extern U32  NC_RegInit(void);
extern void NC_ReConfig(void);
extern U32  NC_EnableLFSR(void);
extern U32  NC_DisableLFSR(void);
extern U32  NC_PlatformResetPre(void);
extern U32  NC_PlatformResetPost(void);
extern U32  NC_PlatformInit(void);
extern U32  NC_ConfigNandFlashContext(void);
extern U32  NC_ResetFCIE(void);
extern U32  NC_ReadPages(U32 u32_PhyRowIdx, U8 *pu8_DataBuf, U8 *pu8_SpareBuf, U32 u32_PageCnt);
extern U32  NC_WritePages(U32 u32_PhyRowIdx, U8 *pu8_DataBuf, U8 *pu8_SpareBuf, U32 u32_PageCnt);
extern U32  NC_PageCopy(U32 u32_SrcPhyRowIdx, U32 u32_DstPhyRowIdx, U8 *pu8_DataBuf, U8 *pu8_SpareBuf, U32 u32_PageCnt, U32 *pu32_DonePageCnt);
extern U32  NC_ReadSectors(U32 u32_PhyRowIdx, U8 u8_SectorInPage, U8 *pu8_DataBuf, U8 *pu8_SpareBuf, U32 u32_SectorCnt);
extern U32  NC_ReadSectors_Retry(U32 u32_PhyRowIdx, U8 u8_SectorInPage, U8 *pu8_DataBuf, U8 *pu8_SpareBuf, U32 u32_SectorCnt);
extern U32  NC_WriteSectors(U32 u32_PhyRowIdx, U8 u8_SectorInPage, U8 *pu8_DataBuf, U8 *pu8_SpareBuf, U32 u32_SectorCnt);
extern U32  NC_ReadID(void);
extern U32 NC_ReadUniqueID (U8 *pu8_UniqueIDBuf);
extern U32  NC_ProbeReadSeq(void);
#if defined(FCIE4_DDR) && FCIE4_DDR
extern U32  NC_ReadToggleParamPage(void);
extern U32  NC_ReadONFIParamPage(void);
extern U32  NC_SetONFISyncMode(void);
extern U32  NC_ProbeDDRNAND(void);
extern U32  NC_ProbeIfONFIDDR(void);
extern U32  NC_ProbeIfToggleDDR(void);
extern U32  NC_DetectDDRTiming(void);
extern U32  NC_FCIE4SetInterface(U8 u8_IfDDR, U8 u8_dqs_mode, U8 u8_dqs_delaycell, U8 u8_rd_ddr_timing);
extern U32  NC_FCIE4SetInterface_EMMC_PLL(U8 u8_IfDDR, U8 u8_dll_phase_sel, U8 u8_rd_ddr_timing);
#endif

#if (defined(NC_SEL_FCIE5) && NC_SEL_FCIE5) && (defined(DDR_NAND_SUPPORT) && DDR_NAND_SUPPORT)
extern U32  NC_ReadToggleParamPage(void);
extern U32  NC_ReadONFIParamPage(void);
extern U32 NC_SetONFISyncMode(U8 u8_SetSyncMode);
extern U32  NC_ProbeIfONFIDDR(void);
extern U32  NC_ProbeIfToggleDDR(void);
extern U32  NC_DetectDDRTiming(void);
extern U32  NC_FCIE5SetInterface(U8 u8_IfDDR, U8 u8_dqs_mode, U8 u8_dqs_delaycell, U8 u8_rd_ddr_timing);

#endif

extern U32  NC_EraseBlk(U32 u32PhyRowIdx);
extern U32  NC_ResetNandFlash(void);
extern void NC_SendCmdForLADebug(void);
extern U32  NC_Read_RandomIn(U32 u32PhyRow, U32 u32Col, U8 *pu8DataBuf, U32 u32DataByteCnt);
extern U32  NC_Write_RandomOut(U32 u32_PhyRow, U32 u32_Col, U8 *pu8_DataBuf, U32 u32_DataByteCnt);
extern U32  NC_Read_RandomIn_MIU(U32 u32PhyRow, U32 u32Col, U8 *pu8DataBuf, U32 u32DataByteCnt);
extern U32  NC_Write_RandomOut_MIU(U32 u32_PhyRow, U32 u32_Col, U8 *pu8_DataBuf, U32 u32_DataByteCnt);
extern U32  NC_GetECCBits(void);
extern U32  NC_WaitComplete(U16 u16_WaitEvent, U32 u32_MicroSec);
extern U32  NC_ReadStatus(void);
extern void NC_CheckECC(int *s32ECCStatus);
extern void NC_DumpRegisters(void);
extern void NC_DumpDebugBus(void);

extern void NC_Config(void);
extern void NC_SetCIFD(U8 *pu8_Buf, U32 u32_CIFDPos, U32 u32_ByteCnt);
extern void NC_GetCIFD(U8 *pu8_Buf, U32 u32_CIFDPos, U32 u32_ByteCnt);
extern void NC_SetCIFD_Const(U8 u8_Data, U32 u32_CIFDPos, U32 u32_ByteCnt);
extern void NC_GetRBufCIFD(U8 *pu8_Buf, U32 u32_CIFDPos, U32 u32_ByteCnt);


extern U32  NC_WritePage_RIUMode(U32 u32_PhyRowIdx, U8 *pu8_DataBuf, U8 *pu8_SpareBuf);
extern U32  NC_ReadSector_RIUMode(U32 u32_PhyRowIdx, U8 u8_SectorInPage, U8 *pu8_DataBuf, U8 *pu8_SpareBuf);

extern U32 NC_PageCopy2P(U32 u32_SrcPhyRowIdx, U32 u32_DstPhyRowIdx, U8 *pu8_DataBuf, U8 *pu8_SpareBuf, U32 u32_PageCnt , U32 *pu32_DonePageCnt);
extern U32 NC_ReadSectors2P(U32 u32_PhyRowIdx, U8 u8_SectorInPage, U8 *pu8_DataBuf, U8 *pu8_SpareBuf, U32 u32_SectorCnt);
extern U32 NC_ReadPages2P(U32 u32_PlaneRowIdx, U8 *pu8_DataBuf, U8 *pu8_SpareBuf, U32 u32_PageCnt);
extern U32 NC_ReadPages2PCache(U32 u32_PlaneRowIdx, U8 *pu8_DataBuf, U8 *pu8_SpareBuf, U32 u32_PageCnt);
extern U32 NC_WritePages2P(U32 u32_PlaneRowIdx, U8 *pu8_DataBuf, U8 *pu8_SpareBuf, U32 u32_PageCnt);
extern U32 NC_WritePages2PCache(U32 u32_PlaneRowIdx, U8 *pu8_DataBuf, U8 *pu8_SpareBuf, U32 u32_PageCnt);
extern U32 NC_EraseBlk2P(U32 u32_PlaneRowIdx);

extern U32 NC_ReadSend2P(U32 u32_PlaneRowIdx, U8 u8_Cmd);
extern U32 NC_ReadSend(U32 u32_PlaneRowIdx, U8 u8_Cmd);

extern U32 drvNAND_init_sem(void);
extern U32 drvNAND_lock_sem(void);
extern U32 drvNAND_unlock_sem(void);

extern void drvNAND_SetAssertPBA(void);
extern U32 drvNAND_IPVerify_Main(void);
extern U32 NCTEST_ReadLoopInitData(U32 u32_StartRowIdx);
extern U32 NCTEST_ReadLoop(U32 u32_StartRowIdx);


//Read Retry

extern void NC_SetupReadRetryCmd(void);
extern U32 NC_GetRegDefaultValue(void);

extern U32 NC_GetReadRetryRegValue(U8** ppu8_RegisterValue);


extern U32  NC_ADMAReadPages(U32 u32_PhyRowIdx, U8 *pu8_DataBuf, U8 *pu8_SpareBuf, U32 u32_PageCnt, U8 u8_Zdec);
extern U32  NC_ADMAWritePages(U32 u32_PhyRowIdx, U8 *pu8_DataBuf, U8 *pu8_SpareBuf, U32 u32_PageCnt);

#if  defined(NAND_BLOCK_LOCK) && NAND_BLOCK_LOCK
extern U32 NC_SetBlkLock_Ex(U32 u32_UBAddr, U32 u32_LBAddr);
#endif //NAND_BLOCK_LOCK

#if  defined(NAND_OTP_ENABLE) && NAND_OTP_ENABLE
extern U32 NC_NAND_OTP_Entry(void);
extern U32 NC_NAND_OTP_Exit(void);
extern U32 NC_NAND_OTP_Status(U8 u8_PageIdx);
extern U32 NC_NAND_OTP_Lock(void);
#endif

#if defined(ENABLE_NAND_POWER_SAVING_MODE) && ENABLE_NAND_POWER_SAVING_MODE
extern void nand_CheckPowerCut(void);
extern void nand_Prepare_Power_Saving_Mode_Queue(void);
#endif // ENABLE_NAND_POWER_SAVING_MODE

#if defined(FCIE_REG_TEE_BASE_ADDR) && FCIE_REG_TEE_BASE_ADDR
extern U32  NC_TEE_Init(void);
extern U32  NC_TEE_RegInit(void);
extern void NC_TEE_ReConfig(void);
extern U32  NC_TEE_EnableLFSR(void);
extern U32  NC_TEE_DisableLFSR(void);
extern U32  NC_TEE_ConfigNandFlashContext(void);
extern U32  NC_TEE_ResetFCIE(void);
extern U32  NC_TEE_ReadPages(U32 u32_PhyRowIdx, U8 *pu8_DataBuf, U8 *pu8_SpareBuf, U32 u32_PageCnt);
extern U32  NC_TEE_WritePages(U32 u32_PhyRowIdx, U8 *pu8_DataBuf, U8 *pu8_SpareBuf, U32 u32_PageCnt);
extern U32  NC_TEE_PageCopy(U32 u32_SrcPhyRowIdx, U32 u32_DstPhyRowIdx, U8 *pu8_DataBuf, U8 *pu8_SpareBuf, U32 u32_PageCnt);
extern U32  NC_TEE_ReadSectors(U32 u32_PhyRowIdx, U8 u8_SectorInPage, U8 *pu8_DataBuf, U8 *pu8_SpareBuf, U32 u32_SectorCnt);
extern U32  NC_TEE_ReadSectors_Retry(U32 u32_PhyRowIdx, U8 u8_SectorInPage, U8 *pu8_DataBuf, U8 *pu8_SpareBuf, U32 u32_SectorCnt);
extern U32  NC_TEE_WriteSectors(U32 u32_PhyRowIdx, U8 u8_SectorInPage, U8 *pu8_DataBuf, U8 *pu8_SpareBuf, U32 u32_SectorCnt);
extern U32  NC_TEE_ReadID(void);
extern U32 NC_TEE_ReadUniqueID (U8 *pu8_UniqueIDBuf);
extern U32  NC_TEE_ProbeReadSeq(void);

#if (defined(NC_SEL_FCIE5) && NC_SEL_FCIE5) && (defined(DDR_NAND_SUPPORT) && DDR_NAND_SUPPORT)
extern U32  NC_TEE_ReadToggleParamPage(void);
extern U32  NC_TEE_ReadONFIParamPage(void);
extern U32 NC_TEE_SetONFISyncMode(U8 u8_SetSyncMode);
extern U32  NC_TEE_ProbeIfONFIDDR(void);
extern U32  NC_TEE_ProbeIfToggleDDR(void);
extern U32  NC_TEE_DetectDDRTiming(void);
extern U32  NC_TEE_FCIE5SetInterface(U8 u8_IfDDR, U8 u8_dqs_mode, U8 u8_dqs_delaycell, U8 u8_rd_ddr_timing);

#endif

extern U32  NC_TEE_EraseBlk(U32 u32PhyRowIdx);
extern U32  NC_TEE_ResetNandFlash(void);
extern U32  NC_TEE_Read_RandomIn(U32 u32PhyRow, U32 u32Col, U8 *pu8DataBuf, U32 u32DataByteCnt);
extern U32  NC_TEE_Write_RandomOut(U32 u32_PhyRow, U32 u32_Col, U8 *pu8_DataBuf, U32 u32_DataByteCnt);
extern U32  NC_TEE_Read_RandomIn_MIU(U32 u32PhyRow, U32 u32Col, U8 *pu8DataBuf, U32 u32DataByteCnt);
extern U32  NC_TEE_Write_RandomOut_MIU(U32 u32_PhyRow, U32 u32_Col, U8 *pu8_DataBuf, U32 u32_DataByteCnt);
extern U32  NC_TEE_GetECCBits(void);
extern U32  NC_TEE_WaitComplete(U16 u16_WaitEvent, U32 u32_MicroSec);
extern U32  NC_TEE_ReadStatus(void);
extern void NC_TEE_CheckECC(int *s32ECCStatus);
extern void NC_TEE_DumpRegisters(void);
extern void NC_TEE_DumpDebugBus(void);

extern void NC_TEE_Config(void);
extern void NC_TEE_SetCIFD(U8 *pu8_Buf, U32 u32_CIFDPos, U32 u32_ByteCnt);
extern void NC_TEE_GetCIFD(U8 *pu8_Buf, U32 u32_CIFDPos, U32 u32_ByteCnt);
extern void NC_TEE_SetCIFD_Const(U8 u8_Data, U32 u32_CIFDPos, U32 u32_ByteCnt);
extern void NC_TEE_GetRBufCIFD(U8 *pu8_Buf, U32 u32_CIFDPos, U32 u32_ByteCnt);


extern U32  NC_TEE_WritePage_RIUMode(U32 u32_PhyRowIdx, U8 *pu8_DataBuf, U8 *pu8_SpareBuf);
extern U32  NC_TEE_ReadSector_RIUMode(U32 u32_PhyRowIdx, U8 u8_SectorInPage, U8 *pu8_DataBuf, U8 *pu8_SpareBuf);

extern U32 NC_TEE_WritePages2P(U32 u32_PlaneRowIdx, U8 *pu8_DataBuf, U8 *pu8_SpareBuf, U32 u32_PageCnt);
extern U32 NC_TEE_EraseBlk2P(U32 u32_PlaneRowIdx);
#endif


#if defined (CONFIG_NAND_SECURITY_GEN) && CONFIG_NAND_SECURITY_GEN==1
extern void drvNAND_SetSecureRange(U32 StartRow, U32 EndRow, U8 PortNo);
extern void drvNAND_EnSecureRange(U8 PortNo);
extern void drvNAND_DisSecureRange(U8 PortNo);
extern void drvNAND_LockSecureRange(U8 PortNo);
#endif

#if defined(ONFI_NVDDR2_SUPPORT) && ONFI_NVDDR2_SUPPORT
extern U32 NC_WritePages_fake (U32 u32_PhyRowIdx, U8 *pu8_DataBuf, U8 *pu8_SpareBuf, U32 u32_PageCnt);
#endif

#endif // NAND_DRV_V3

