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
#ifndef _FTL_CORE_H_
#define _FTL_CORE_H_


// ============================================
// Version Control: change & auto format FTL partition
// ============================================
#define FTL_VERSION             1 // check by ftl_root.c (NFTL_ErrHandle_Root)


// =============================================
// FTL Flags
// =============================================
extern U32 gu32_FtlFlag;
#define FTL_INIT_READY     BIT0
#define FTL_MOVING_CTRL    BIT1 // to handle BB while moving CtrlBlk
#define FTL_INIT_FBQ_OK    BIT2
#define FTL_RDB_ONGOING    BIT3
#define FTL_RREQ_WAITING   BIT4 // linux ReadData_sg, make R first before W.
#define FTL_BigBuf_Kept    BIT5 // to avoid mis-reuse pNANDBigBuf (for WL, RD)
#define FTL_READ_DISTURB   BIT6

// =============================================
// Block ID:
// =============================================
#define BID_ROOT       1
#define BID_CTRL       2
#define BID_DATA       3
#define BID_DBPM       4

#define BID_FREE       0xFF

typedef UNFD_PACK0 struct _COMM_RDD
{
    U8 u8_BBMark, u8_BID;
    
}UNFD_PACK1 COMM_RDD, *P_COMM_RDD;


// =============================================
// Root
// =============================================
#define ROOT_ID_INFO   BIT0
#define ROOT_ID_IBBT   BIT1
#define ROOT_ID_RBBT   BIT2


// RootBlk Page RDD
typedef UNFD_PACK0 struct _ROOT_RDD
{
    U8 u8_BBMark, u8_BID;
       
    U8 u8_DataID, u8_tmp;
    
}UNFD_PACK1 ROOT_RDD, *P_ROOT_RDD;

// RootBlk Page Data
typedef UNFD_PACK0 struct _ROOT_INFO
{
    U16 u16_RBBTLowPIdx; // not in checksum
    U16 u16_RBBCnt; // save the time to CalcBBCnt
    
    U32 u32_ChkSum;
    
    // checksum from here to end
    U32 u32_SerialNo; // +1 @ MoveRootBlk

    U16 u16_IBBCnt; // save the time to CalcBBCnt
    
    // InitRoot: from SerialNo -> RootPBA -> to get latest valid RI & BBT.
    U16 u16_RootPBA; // assign @ MoveRootBlk / BuildRootBlk
    
    U16 u16_StartMainPBA;
    U16 u16_OldCtrlPBA, u16_NewCtrlPBA;
    
    // used to check FTL integrity
    U16 u16_FtlSWVer, u16_StartPBA, u16_PBACnt;

}UNFD_PACK1 ROOT_INFO, *P_ROOT_INFO;


typedef struct _ROOT_BLK
{
    ROOT_RDD RDD;
    ROOT_INFO Info;
    
    U16 u16_FreeLowPIdx;
    
    U8  u8_WriteTwice, u8_LastDataID;
    
}ROOT_BLK, *P_ROOT_BLK;


// --------------------------
extern ROOT_BLK FtlRoot;
extern U8 *pu8_FtlIBBT, *pu8_FtlRBBT;

// --------------------------
#define BBT_BYTECNT_MAX    0x800 // 2KB x8, map to 16K Blk
#define BBT_BYTECNT        (FtlDev.u16_PBACnt/8+1)

extern U32  NFTL_WriteRootInfo(void);
extern U32  NFTL_WriteBBT(U8* pBBT);
extern U32  NFTL_InitRoot(void);
extern void NFTL_DumpRoot(void);
extern void NFTL_DumpBBT(U8* pBBT);
extern U32  NFTL_BuildRootBlk(void); // format FTL partition

#if TEMP_CHECK
extern void NFTL_Temp(void);
extern U8 au8_DataReadBuf[16384];
extern U8 au8_SpareReadBuf[2000];
#endif

// =============================================
// Ctrl
// =============================================
#define CTRL_ID_ECNT      0
#define CTRL_ID_DBPM      1 // DB PMap Info, (init 0xFF) 

#define CTRL_ID_CNT       2 // total how many CtrlInfo Item

#define IF_CTRLINFO_INIT_0xFF(CID) \
    (CID==CTRL_ID_DBPM) // dummy in NFTL_BuildCtrlBlk so far.

// --------------------------
// Protective CtrlInfo: each defines its SubIDs
#define CTRL_ID_PROTECTIVE  0x80
#define PCID_RDB            (1|CTRL_ID_PROTECTIVE) // recycle DB
#define PCID_RDBPM          (2|CTRL_ID_PROTECTIVE) // recycle DBPM
#define PCID_DBB            (3|CTRL_ID_PROTECTIVE) // DB -> BB

#define PCID_WL             (4|CTRL_ID_PROTECTIVE) // wear-leveling
#define PCID_RD             (5|CTRL_ID_PROTECTIVE) // read-disturbance
#define PCID_RHPI           (6|CTRL_ID_PROTECTIVE) // Read HPI
#define PCID_CIMG           (7|CTRL_ID_PROTECTIVE) // clone FTL image
#define PCID_DB2FB          (8|CTRL_ID_PROTECTIVE) // Gain a FB from DB(s)

#define IF_PCID_FINISH(ID, SID)  \
    ( ((ID)==0 && (SID)==0) ||   \
    IF_PCID_RDB_FINISH(ID, SID)||\
    IF_PCID_RDBPM_FINISH(ID, SID) ||\
    IF_PCID_DBB_FINISH(ID, SID))

  #define IF_PCID_RDB_FINISH(ID, SID) \
    ((ID)==PCID_RDB && ((SID)==RDB_FINISH || (SID)==RDB_FINISH_WAIT))

  #define IF_PCID_RDBPM_FINISH(ID, SID) \
    ((ID)==PCID_RDBPM && (SID)==RDBPM_FINISH)

  #define IF_PCID_DBB_FINISH(ID, SID) \
    ((ID)==PCID_DBB && (SID)==DBB_FINISH)

// --------------------------
  typedef struct _CTRL_INFO
  {
      void *pBuf; // CtrlBlk Page Data
      U32 u32_ByteCnt;
      U8  u8_PageCnt;
  
  }CTRL_INFO, *P_CTRL_INFO;
  

// CtrlBlk Page RDD
typedef UNFD_PACK0 struct _CTRL_RDD
{
    U8 u8_BBMark, u8_BID;
    
    U8 u8_DataID, u8_SubID;
    
}UNFD_PACK1 CTRL_RDD, *P_CTRL_RDD;


typedef struct _CTRL_BLK
{
    CTRL_RDD RDD;
    
    U16 u16_MainTotalBlkCnt; // total PBA (PBACnt) - ROOT_BLK_CNT
    U16 u16_MainBBCnt, u16_MainIBBCnt;
    U16 u16_DataBlkCnt; // <- this is a fixed number
    
    U16 u16_SysBlkCnt, u16_FB4DBCnt, u16_FB4BBCnt;
    
    U16 u16_CtrlPBA, u16_FreeLowPIdx;
    
    // for PwrCut handle
    U16 u16_LastValidLowPIdx, u16_LastProtLowPIdx;
    U8  u8_LastDataID, u8_LastSubID, u8_LastProtID, u8_LastProtSubID;

    // Ctrl Info
    CTRL_INFO a_CtrlInfo[CTRL_ID_CNT];

    // ECnt
    U32 u32_AccuECnt; // accumulated ECnt

    #if STATIC_WL
    U16 ValidPCntRatio;
    #endif

    #if READ_DISTURB
    U16 a_RD_PBA[READ_DISTURB_CNT], a_RD_PIdx[READ_DISTURB_CNT], RD_Cnt;
    #endif

    #if MSFTL_LINUX && defined(BKG_HK_THREAD) && BKG_HK_THREAD 
	struct task_struct	*hk_thread;    
	struct semaphore	hk_thread_sem;
    #endif
    
}CTRL_BLK, *P_CTRL_BLK;

#define CheckFBForBB() {\
    if(FtlCtrl.u16_FB4BBCnt)  FtlCtrl.u16_FB4BBCnt--; \
    else  {ftl_dbg(FTL_DBG_LV_ERR,1,"no FB for BB.\n");  ftl_die();}}

// --------------------------
extern CTRL_BLK FtlCtrl;
extern U16 *pu16_PBAECnt; // start from FtlDev.u16_StartPBA
#define ByteCNT_ECnt      (FtlDev.u16_PBACnt*2)

// ====================
// used for ErrHandle @ init
extern  U8 *pu8_PCIDBuf; 
#define MAX_PCID_BYTECNT   FtlDev.u16_PageByteCnt
// ====================

extern U32  NFTL_MoveCtrlBlk(void);
extern U32  NFTL_WriteCtrlInfo_AllSubID(U8 u8_DataID);
extern U32  NFTL_WriteCtrlInfo(U8 u8_DataID, U8 u8_SubID);
// ProtCID go through NFTL_WriteCtrlInfo_Ex
extern U32  NFTL_WriteCtrlInfo_Ex(U8 u8_DataID, U8 u8_SubID, U8 *pBuf);
extern U32  NFTL_InitCtrl(void);
extern void NFTL_DumpCtrl(void);
extern void NFTL_DumpCtrl_Ex(void);
extern U32  NFTL_ErrHandle(void);
extern U32  NFTL_ErrHandle_ClearFB(void);

#if defined(BKG_HK_THREAD) && BKG_HK_THREAD
extern U32 NFTL_BKG_ThreadInit(void);

  #if MSFTL_LINUX
  extern int  NFTL_BKG_Housekeep(void* p);
  #elif MSFTL_UBOOT
  extern U32  NFTL_BKG_Housekeep(void);
  #endif
#endif

extern void NFTL_DumpECnt(void);
extern U32  NFTL_WriteCtrlTwice(void);

// Static Wear-Leveling
extern U16  NFTL_WL_PickupColdDB(void);
extern U32  NFTL_WL_PinchStone(void);

// read-disturbance
extern void NFTL_RD_Set(U16 PBA, U16 PIdx);
extern void NFTL_RD_Move(void);

// =============================================
// DBPM
// =============================================
typedef UNFD_PACK0 struct _FTLDBPM_BlkRdd
{
    U8 u8_BBMark, u8_BID;
    
}UNFD_PACK1 DBPM_BlkRdd, *P_DBPM_BlkRdd;

// update at the time its pu16_NextFBPBA used up.
// update into pDBPMCID->pu16_PBA[pDBPMCID->PBACnt-1]'s LowPage.
typedef struct _FTLDBPM_BlkPage
{
    U8   NextFBCnt, TheseDBCnt;
    U16 *pu16_NextFBPBA, *pu16_TheseDBPBA;

    // when write into DBPMBlk: pu32_DBPMs for the PBAs in pu16_TheseDBPBA,
    // when run-time: pu32_DBPMs for the PBAs in pu16_NextFBPBA.
    U32 *pu32_DBPMs; // pu32_DBPMs[PhyPIdx] = LogiRow
    
}DBPM_BlkPage, *P_DBPM_BlkPage;


extern DBPM_BlkPage *pDBPMBlkPage;
extern DBPM_BlkRdd  DBPMBlkRdd;

#define GetDBPMBuf(pDBPMPage, BlkIdx)  \
    &pDBPMPage->pu32_DBPMs[pDBPMCID->DBPMUnitBCnt*(BlkIdx)/4]

extern void NFTL_InitDBPM_PagePtr(P_DBPM_BlkPage pDBPMPage);

// --------------------------
// some status for pu8_PBA2DBPM_BIdx
#define NotInDBPMBlk_FB_SB  0xFF // FB or SysBlk
#define NotInDBPMBlk_DDB    0xFE // DDB: Dirty DB, the DB with 0 PBAValidPCnt.
// update at the time of its pu16_PBA changed.
// update by NFTL_WriteCtrlInfo_AllSubID(CTRL_ID_DBPM).
typedef struct _FTLDBPM_CID
{
    U16 *pu16_PBA; // based on pDBPMCID->MaxPBACnt
    U16 FreeLowPIdx;
    U8  PBACnt, MaxPBACnt, MinPBACnt;

    // kept to save calc time
    U16 DBPMUnitBCnt, PageDBPMCnt;

    // --------------------------
    // update based on DBPM_BlkPage::pu16_TheseDBPBA @ NFTL_WriteDBPM.
    // size based on FtlCtrl.u16_MainTotalBlkCnt
    U8  *pu8_PBA2DBPM_BIdx; // init to all 0xFF
    U8  *pu8_PBA2DBPM_LPIdx;

}DBPM_CID, *P_DBPM_CID;

extern DBPM_CID *pDBPMCID;

// pRowL2P[LogiRow] = PhyRow
// not saved directly, build from loading DBPMs from DBPMBlks.
// size based on FtlCtrl.u16_DataBlkCnt.
extern  U32 *pRowL2P; // init to all 0

// to speed-up recycle DB, 
// size based on FtlCtrl.u16_MainTotalBlkCnt
extern  U16 *pu16_PBAValidPCnt; // init to all 0

// used to speed up flush DBPMBlk
// size based on pDBPMCID->MaxPBACnt
// each DBPMBlk, keeps how many valid DBs' PMap.
// + or - depends on the corresponding pu16_PBAValidPCnt[DB_MainIdx] 0 or not 0.
#define DBPMBLK_CNT_FACTOR     1 // to diminish the overhead of flushing DBPMBlk.
#define MAX_DBPM_BLK_CNT       8
extern U16 pu16_ValidDBPMCnt[MAX_DBPM_BLK_CNT]; // init to 0 @ NFTL_DBPM_GetBlkFBs

#define MainAreaPBAIdx(PBA)  ((PBA) - FtlRoot.Info.u16_StartMainPBA)

// --------------------------
extern U32  NFTL_InitDBPM(void);
extern U32  NFTL_BuildDBPM_CID(void);
extern U32  NFTL_LoadDBPM(void);
extern U32  NFTL_WriteDBPM(void);
extern U32  NFTL_MoveDBPM(void);
extern U32  NFTL_MoveDBPM_BB(void);
extern void NFTL_ChkValidDBPMCnt(U8 Idx);
extern void NFTL_DumpDBPM(void);
extern void NFTL_DumpDBPM_Details(void);
extern void NFTL_DumpRowL2P(void);
extern void NFTL_DumpPBAValidPCnt(void);
extern U32  NFTL_WriteDBPM_Check(void);
extern void NFTL_DumpBlkSt(void);
extern void NFTL_DumpDBPM_LRow(U32 *pDBPM);
// --------------------------
// RDBPM
// --------------------------
#define RDBPM_START          1
#define RDBPM_MOVE_DONE      2
#define RDBPM_FINISH         3

#define RDBPM_ST_NORMAL      BIT0
#define RDBPM_ST_BB          BIT1
#define RDBPM_ST_HIT_PCID    BIT2

typedef struct _PCID_RDBPM
{
    //PCID_RDB_t RDB_t;
    
    U8  SrcIdx, StFlag;
    U16 SrcPBA, FreeLowPIdx; // for BB case, the write-failed LPIdx.
    U16 DestPBA, ValidDBPMCnt;
#if 0
    U8  TheseDBCnt;
    U16 *pu16_TheseDBPBA;
    U32 *pu32_DBPM; // for RDBPM_ST_BB_RDB case
#else
    U8  PrevPCID, PrevSID;
    U8  *pPrevPCIDBuf;
#endif
}PCID_RDBPM_t, *P_PCID_RDBPM_t;

#if 0
#define InitRDBPMPtr(pRDBPM)  { \
    pRDBPM->pu16_TheseDBPBA = (U16*)((U32)pRDBPM + sizeof(PCID_RDBPM_t)); \
    pRDBPM->pu32_DBPM = (U32*)((U32)pRDBPM->pu16_TheseDBPBA + pDBPMCID->PageDBPMCnt*sizeof(U16));}
#else
#define InitRDBPMPtr(pRDBPM)  { \
    pRDBPM->pPrevPCIDBuf = (U8*)((U32)pRDBPM + sizeof(PCID_RDBPM_t));}
#endif
extern U32  NFTL_ErrHandle_RDBPMBlk(void);
extern U32  NFTL_ErrHandle_RDBPMBlk_MoveFromBB(void);
extern U32  NFTL_MoveDBPM_Ex(P_PCID_RDBPM_t pRDBPM_t);

// --------------------------
// RDB
// --------------------------
#define RDB_INIT             0
#define RDB_START            1
#define RDB_MOVE_DONE        2
#define RDB_FINISH           3
#define RDB_FINISH_WAIT      4

#define RDB_ST_TheseDB       3
#define RDB_ST_DBPMBlkPage   2
#define RDB_ST_DBPMBlkCID    1

typedef struct _PCID_RDB
{
    U16 DB_PBA, FB_PBA, PCnt;
    U8  RDBSt, NextFBCnt;
    U16 DBPMBlkLPIdx;
    U32 *pu32_DBPM;
    U16 *pu16_NextFBPBA;

}PCID_RDB_t, *P_PCID_RDB_t;

#define InitRDBPtr(pRDB)  { \
    pRDB->pu32_DBPM = (U32*)((U32)pRDB + sizeof(PCID_RDB_t)); \
    pRDB->pu16_NextFBPBA = (U16*)((U32)pRDB->pu32_DBPM + pDBPMCID->DBPMUnitBCnt); }

// should be less than PageByteCnt
#define PCID_RDB_BCNT \
    (sizeof(PCID_RDB_t)+pDBPMCID->DBPMUnitBCnt+pDBPMCID->PageDBPMCnt*sizeof(U16))

// current FB Cnt + reservedBlk for Sys - Blk already from FB to DBPMBlk - Blk already taken from FB to CtrlBlk.
#define FB_CNT_NotForDB    (FtlCtrl.u16_FB4BBCnt+FtlCtrl.u16_SysBlkCnt-pDBPMCID->PBACnt-1)

extern U32  NFTL_RecycleDB(U8 PCID);
extern void NFTL_DumpRDB(P_PCID_RDB_t pRDB_t);
extern U32  NFTL_ErrHandle_PresetRDB(void);
extern U32  NFTL_ErrHandle_FinishRDB(void); // after Core_Init

extern U32  NFTL_MoveDB(U16 DestPBA, U16 DestPIdx, U16 SrcPBA, U32 *pDestDBPM, U32 *pSrcDBPM);
extern U32* NFTL_GetPBADBPM(U16 PBA, P_DBPM_BlkPage pDBPMPage);


// --------------------------
// DB -> BB
// --------------------------
#define DBB_START          1
#define DBB_MOVE_DONE      2
#define DBB_FINISH         3

typedef struct _PCID_DBB
{
    U16 DB_PBA, FB_PBA, PCnt;
    U32 *pu32_DBPM;
    
}PCID_DBB_t, *P_PCID_DBB_t;

#define InitDBBPtr(pDBB)  { \
    pDBB->pu32_DBPM = (U32*)((U32)pDBB + sizeof(PCID_DBB_t)); }

// =============================================
// Data
// =============================================
// DataBlk Page RDD
#define CONTINUOUS_W   (U32)(1<<31)
#define LOGI_ROW(LogiRow)  ((LogiRow)&~CONTINUOUS_W)

typedef UNFD_PACK0 struct _FTLDB_RDD
{
    U8 u8_BBMark, u8_BID;
    
    U32 LogiRow; // bit.31: continuous-write flag
                 // thus support max 2G x PageByteCnt
    
}UNFD_PACK1 DB_RDD, *P_DB_RDD;


typedef struct _FTLDB
{
    DB_RDD RDD;

    // FB
    U16 FBQBlkCnt, FBQSlotCnt;

    // now used DB, @ DBPM_BlkPage::pu16_NextFBPBA
    #if HW_DETECT_PWR_LOSS
    U16 FBIdx, PIdx;
    #else
    U16 FBIdx, LPIdx;
    #endif

    #if PAGE_SIZE_RAM_BUFFER
    U32 RamBuf512BAddr, RamBuf512BCnt;
    #endif
    
}FTLDB, *P_FTLDB;


// --------------------------
extern FTLDB FtlDB;
extern  U16 *pu16_FreePBA; // free blk queue
// BlkKept flag buf, start from FtlDev.u16_StartPBA.
extern  U8  *pu8_IfBlkKept;  // init to all 0.
extern  U8  *pu8_IfBlkInFBQ; // init to all 0.
extern  U8  *pu8_IfBlkIsDB;  // init to all 0.

extern  U8  *pu8_IfMSBPage, *pu8_PIdx2LPIdx;
#define CHK_PAGE_MSB(BlkPIdx)  (pu8_IfMSBPage[(BlkPIdx) >> 3] & 1<<((BlkPIdx) & 7))
#define SET_PAGE_MSB(BlkPIdx)  (pu8_IfMSBPage[(BlkPIdx) >> 3] |= 1<<((BlkPIdx) & 7))

// internal data R/W, RamBuf, LoadDBPM_FromBlkPage, LoadDBPM_FromCID.
extern  U8  *pNANDPDataBuf; 

// used @ 
// - LoadDBPM: PBA_Visited, 
// - MoveDB 
// - MoveDBPM_Ex: [WBuf,PBA_Visited].
// - MoveDBPM_BackupPrevPCID: temp for PrevPCID.
// - WL_PinchStone: temp data buf
extern  U8  *pNANDBigBuf; 
#define NFTL_BIG_BUF_BYTCNT  (U32)0x10000 // 64KB
// for ftl_utl.c
extern  U8  *pNANDSectorBuf; 

// based on FtlCtrl.u16_MainTotalBlkCnt
#define SET_PBA_Visited(PBA)       \
    (pNANDBigBuf[(PBA) >> 3] |= 1<<((PBA) & 7))

#define CHK_PBA_Visited(PBA)       \
    (pNANDBigBuf[(PBA) >> 3] & 1<<((PBA) & 7))


// --------------------------
extern U32  NFTL_ReadData_Ex(U8* pu8_DataBuf, U32 u32_512BCnt, U32 u32_512BAddr);
#if MSFTL_LINUX
extern U32  NFTL_ReadData_sg_Ex(struct msftl_sg * sg_buf, U32 u32_512BCnt, U32 u32_512BAddr, U32 u32_SgCnt);
#endif
extern U32  NFTL_WriteData_Ex(U8* pu8_DataBuf, U32 u32_512BCnt, U32 u32_SectorAddr);
extern U32  NFTL_PIdx2FLPIdx(U16 PIdx);
extern U32  NFTL_InitDB(void);
extern U32  NFTL_MakeupFBQ(void);
extern U32  NFTL_SetFB(U16 FreePBA);
extern U32  NFTL_GetFB(U16 *pFreePBA);
extern U32  NFTL_GetFB_B4Init(U16 *pFreePBA);
extern U32  NFTL_ChkFB(void);
extern void NFTL_DumpData(void);

#if PAGE_SIZE_RAM_BUFFER
extern U8 *pPageRamBuf;
extern void NFTL_SetPageRamBuf(U8 *pu8_DataBuf, U32 u32_512BCnt, U32 u32_512BAddr);
extern void NFTL_GetPageRamBuf(U8 *pu8_DataBuf, U32 *p512BCnt, U32 *p512BAddr);
#endif

// =====================================
#if BKG_W_THREAD
extern wait_queue_head_t nftl_w_wait;
extern int NFTL_WriteData_Bkg(void *pData);
#define BKGW_BUF_SECCNT  (0x100000/0x200) // 1MB
#define UNIT_CNT         0x400 // 1K w req

typedef struct _FTLBKGW
{
    struct task_struct *bkg_wdata;
    spinlock_t spnlock;
    
    U8  *pBuf; // data buffer
    U16 WReqHead, WReqTail, WReqCnt, RReqCnt;
    U32 Buf512BHead, Buf512BTail;
    U32 WSecAddr[UNIT_CNT], WSecCnt[UNIT_CNT], WBufAddr[UNIT_CNT];
    U32 RSecAddr[UNIT_CNT+1], RSecCnt[UNIT_CNT+1], RBufAddr[UNIT_CNT+1];
    
}FTLBkgW, *P_FTLBkgW;

extern FTLBkgW FtlBkgWD;
#endif // BKG_W_THREAD


// =====================================
#if HOT_DATA_EN

#define HDB_TEST       0
  #if HDB_TEST
#define SecAdr_0       0x8   //LRow: 0
#define SecAdr_1       0x710 //LRow: 38h
#define SecAdr_2       0x790 //LRow: 3Ch
#define SecAdr_3       0x6A8 //LRow: 35h
  #endif

#define HDAT_PCNT_PerBlk      4 
#define HDAT_BLKCNT           1 
#define HDAT_TOTAL_PCNT       (HDAT_PCNT_PerBlk*HDAT_BLKCNT)

typedef struct _FTLHD
{
    U32  pHotLRow[HDAT_TOTAL_PCNT];
    U8  *pHotDBuf;
    U16  pHotPBA[HDAT_BLKCNT];
    U16  pFLPIdx[HDAT_BLKCNT];
      
}FTLHotData, *P_FTLHotData;

extern FTLHotData FtlHotData;


extern U32  NFTL_InitHotData(void);
extern U32  NFTL_ChkHotData(U32 u32_512BCnt, U32 u32_512BAddr);
extern void NFTL_DumpHotData(P_FTLHotData pHotData);
extern U32  NFTL_MoveHotDB(U16 HotDBIdx);

#endif // HOT_DATA_EN



#endif // _FTL_CORE_H_

