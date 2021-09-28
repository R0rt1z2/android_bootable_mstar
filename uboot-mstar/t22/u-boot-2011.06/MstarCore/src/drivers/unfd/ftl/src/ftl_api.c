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

#include "ftl_api.h"

// ============================================
U32 gu32_FtlFlag = 0;
#if MSFTL_LINUX
struct semaphore FtlFlagSem;
#endif

// ============================================
U32 NFTL_CheckIfReady(void)
{
    if(FTL_CHK_FLAG(FTL_INIT_READY))
        return FTL_OK;

    return FTL_ERR_NOT_READY;
}

void NFTL_ResetReadyFlag(void)
{    
    gu32_FtlFlag = 0;
    //NFTL_free();
}

// ============================================
#if defined(TEST_INDEX) && TEST_INDEX!=bgw_no_test
struct timeval TimeValS, TimeValE;
U32 totalms=0;

unsigned long long u64_PrevJiffies, u64_DiffJiffies;
#endif


#if defined(BKG_W_THREAD) && BKG_W_THREAD
FTLBkgW FtlBkgWD;
static U8 au8_BkgWDBuf[(BKGW_BUF_SECCNT<<9)+(UNFD_CACHE_LINE-1)];

  #if TEST_INDEX == bgw_waitevt
static DECLARE_WAIT_QUEUE_HEAD(fcie_wait);
static U32 ftl_wakeup=0;
  #elif TEST_INDEX == bgw_tasklet
struct tasklet_struct ftl_bkgw_task;
  #else
wait_queue_head_t nftl_w_wait;
  #endif  

void NFTL_DumpBkgW(P_FTLBkgW pBkgW)
{
    U16 i, pos;
    ftl_dbg(FTL_DBG_LV_LOG,0,"Buf Adr:            %Xh %Xh \n", (U32)au8_BkgWDBuf, (U32)FtlBkgWD.pBuf);
    ftl_dbg(FTL_DBG_LV_LOG,0,"WReqH WReqT WReqC:  %Xh %Xh %Xh \n", 
        FtlBkgWD.WReqHead, FtlBkgWD.WReqTail, FtlBkgWD.WReqCnt);
    ftl_dbg(FTL_DBG_LV_LOG,0,"BufSecH BufSecT:    %Xh %Xh \n", 
        FtlBkgWD.Buf512BHead, FtlBkgWD.Buf512BTail);
    ftl_dbg(FTL_DBG_LV_LOG,0,"[SAdr SCnt BAdr] \n");
    pos = FtlBkgWD.WReqHead;
    for(i=0; i<FtlBkgWD.WReqCnt; i++)
    {
        ftl_dbg(FTL_DBG_LV_LOG,0,"[%5Xh %3Xh %Xh] ", 
            FtlBkgWD.WSecAddr[pos], FtlBkgWD.WSecCnt[pos], FtlBkgWD.WBufAddr[pos]);
        pos++;
        if(pos == UNIT_CNT)
            pos = 0;
        if((i+1)%4==0)
            ftl_dbg(0,0,"\n");
    }
    ftl_dbg(0,0,"\n");
}


int NFTL_WriteData_Bkg(void *pData)
{
    #if TEST_INDEX == bgw_no_test
//    U16 WReqCntTmp, i, ReqHeadNext;
//    U32 SecAddr, SecCnt, BufAddr, SecCntTmp;
    #endif
    #if TEST_INDEX == bgw_waitevt
    U16 NotTO;
    ftl_wakeup = 0;
    #else
    init_waitqueue_head(&nftl_w_wait); // <- init here
    #endif
    
    LABEL_THREAD_MAIN:
    //ftl_dbg(0,1,"\n");
    // ----------------------------------------------------
    #if defined(TEST_INDEX) && TEST_INDEX==bgw_waitevt
    NotTO=wait_event_timeout(fcie_wait, ftl_wakeup, usecs_to_jiffies(1000000*5));
    ftl_wakeup = 0;
    #else
    sleep_on(&nftl_w_wait); // <- sleep here
    #endif
    
    #if defined(TEST_INDEX) && TEST_INDEX > wt_gap
    do_gettimeofday(&TimeValS);
      #if TEST_INDEX == bgw_waitevt
    if(NotTO)
      #endif
    if((U32)(TimeValS.tv_usec-TimeValE.tv_usec)/1000 < 1000*1)//skip overflow
    {
        totalms += (TimeValS.tv_usec-TimeValE.tv_usec)/1000;
        printk("BKGT: %u ms, %u ms \n", (TimeValS.tv_usec-TimeValE.tv_usec)/1000, totalms);
    }
    #endif

    // ----------------------------------------------------
    #if TEST_INDEX==bgw_no_test
    #if 1
    while(FtlBkgWD.WReqCnt)
    {
        nand_lock_fcie();
        NFTL_WriteData_Ex(
            (U8*)FtlBkgWD.WBufAddr[FtlBkgWD.WReqHead], 
            FtlBkgWD.WSecCnt[FtlBkgWD.WReqHead], 
            FtlBkgWD.WSecAddr[FtlBkgWD.WReqHead]);
        nand_unlock_fcie();

        spin_lock(&FtlBkgWD.spnlock);
        FtlBkgWD.WReqCnt--;
        FtlBkgWD.WReqHead++;
        if(FtlBkgWD.WReqHead == UNIT_CNT)
            FtlBkgWD.WReqHead = 0;

        if(0!=FtlBkgWD.WReqCnt)
            FtlBkgWD.Buf512BHead = 
                (FtlBkgWD.WBufAddr[FtlBkgWD.WReqHead]-(U32)FtlBkgWD.pBuf)>>9;
        else
            FtlBkgWD.Buf512BHead = FtlBkgWD.Buf512BTail;
        spin_unlock(&FtlBkgWD.spnlock);
    }
    #else
    while(FtlBkgWD.WReqCnt)
    {
        //spin_lock(&FtlBkgWD.spnlock);
        SecAddr = FtlBkgWD.WSecAddr[FtlBkgWD.WReqHead];
        SecCnt = FtlBkgWD.WSecCnt[FtlBkgWD.WReqHead];
        BufAddr = FtlBkgWD.WBufAddr[FtlBkgWD.WReqHead];

        SecCntTmp = SecCnt;
        WReqCntTmp = 1;
        ReqHeadNext = FtlBkgWD.WReqHead + WReqCntTmp;
        if(ReqHeadNext == UNIT_CNT)
            ReqHeadNext = 0;

        // packed WReq if possible
        while(ReqHeadNext != FtlBkgWD.WReqTail)
        {
            if(SecAddr+SecCnt==FtlBkgWD.WSecAddr[ReqHeadNext] && 
                BufAddr+(SecCnt<<9)==FtlBkgWD.WBufAddr[ReqHeadNext])
            {
                SecAddr = FtlBkgWD.WSecAddr[ReqHeadNext];
                SecCnt = FtlBkgWD.WSecCnt[ReqHeadNext];
                BufAddr = FtlBkgWD.WBufAddr[ReqHeadNext];

                WReqCntTmp++;
                SecCntTmp += SecCnt;
                
                ReqHeadNext++;
                if(ReqHeadNext == UNIT_CNT)
                    ReqHeadNext = 0;
            }
            else
                break;
        }
        //spin_unlock(&FtlBkgWD.spnlock);
        nand_lock_fcie();
        //if(FtlBkgWD.WReqHead==1)
        //  NFTL_DumpBkgW(&FtlBkgWD);
        //ftl_dbg(0,1,"WRH: %Xh  WRCT: %Xh \n", FtlBkgWD.WReqHead, WReqCntTmp);
        //spin_lock(&FtlBkgWD.spnlock);
        NFTL_WriteData_Ex((U8*)FtlBkgWD.WBufAddr[FtlBkgWD.WReqHead], 
            SecCntTmp, FtlBkgWD.WSecAddr[FtlBkgWD.WReqHead]);
        //spin_unlock(&FtlBkgWD.spnlock);
        nand_unlock_fcie();

        spin_lock(&FtlBkgWD.spnlock);
        FtlBkgWD.WReqCnt -= WReqCntTmp;
        FtlBkgWD.WReqHead += WReqCntTmp;
        if(FtlBkgWD.WReqHead >= UNIT_CNT)
            FtlBkgWD.WReqHead -= UNIT_CNT;

        if(0==FtlBkgWD.WReqCnt)
            FtlBkgWD.Buf512BHead = FtlBkgWD.Buf512BTail;
        else
            FtlBkgWD.Buf512BHead = 
                (FtlBkgWD.WBufAddr[FtlBkgWD.WReqHead]-(U32)FtlBkgWD.pBuf)>>9;        
        spin_unlock(&FtlBkgWD.spnlock);
    }
    #endif
    #endif // bgw_no_test

    goto LABEL_THREAD_MAIN;    
	return 0;
}
#endif
// ============================================

// ----------------------------------------------------
// 1. if no pni or pni has no FTL partiton (no u16_PartType == UNFD_PART_FTL)
//     fill in the parameters with the range for FTL.
// 2. or the range determined by pni (u16_PartType == UNFD_PART_FTL), 
//     and ignore the input parameters.
// ----------------------------------------------------
U32 NFTL_Init(U16 u16_StartPBA, U16 u16_PBACnt)
{
    U32 u32_err;
	#if MSFTL_LINUX
	init_MUTEX(&FtlFlagSem);
	#endif

    if(FTL_OK == NFTL_CheckIfReady())
        return FTL_OK;

    u32_err = drvNAND_CheckAlignPack(UNFD_CACHE_LINE);
    if(FTL_OK != u32_err)
        return u32_err;
    
    #if defined(TEST_INDEX) && TEST_INDEX == wt_gap
    u64_PrevJiffies = u64_DiffJiffies = 0;
    do_gettimeofday(&TimeValE);
    #endif
	#if MSFTL_LINUX
	nand_lock_fcie();
	#endif
    // --------------------------
    u32_err = NFTL_HAL_Init(u16_StartPBA, u16_PBACnt);
    if(FTL_OK != u32_err)
    {
		#if MSFTL_LINUX
		nand_unlock_fcie();
		#endif
        return u32_err;
    }
    
    // --------------------------
    u32_err = NFTL_Core_Init();
    if(FTL_OK != u32_err)
    {
		#if MSFTL_LINUX
		nand_unlock_fcie();
		#endif
        return u32_err;
    }
	#if MSFTL_LINUX
	nand_unlock_fcie();
	#endif
    
    // --------------------------
    // others
    FTL_SET_FLAG(FTL_INIT_READY);    

    #if TEMP_CHECK
    if(0==FTL_Disable_ErrLog)
    {
        U8 u8_i;        
        
        #if MSFTL_LINUX
		nand_lock_fcie();
		#endif
        for(u8_i=0; u8_i<pDBPMCID->PBACnt; u8_i++)
            NFTL_ChkValidDBPMCnt(u8_i); 

        NFTL_CheckDBPCnt();

        NFTL_Temp();
        
        #if MSFTL_LINUX
    	nand_unlock_fcie();
    	#endif
    }
    #endif
    
    return FTL_OK;
}


void NFTL_CleanStruct(void)
{
    memset(&FtlRoot, 0, sizeof(ROOT_BLK));
    memset(&FtlCtrl, 0, sizeof(CTRL_BLK));
    memset(&FtlDB, 0, sizeof(FTLDB));    
}


U32 NFTL_Core_Init(void)
{
    U32 err;

    // --------------------------
    NFTL_CleanStruct();
    
    // OK or die inside
    err=NFTL_InitRoot();
    if(err)  
        return err;    
    err=NFTL_InitCtrl();
    if(err)  
        return err;    
    err=NFTL_InitDBPM();
    if(err)  
        return err;    
    err=NFTL_InitDB();
    if(err)  
        return err;    
    
    // --------------------------
    // normal flow
    if(IF_PCID_FINISH(FtlCtrl.u8_LastProtID, FtlCtrl.u8_LastProtSubID))
    {
        // 1. DBPMBlk / DBPMBlkPage moves @ the end of w data,
        //     need to handle the previous pwr-cut @ the end of w.
        // 2. need to touch FBQ, so do after InitDB.
        err=NFTL_WriteDBPM_Check();
        if(err)
            return err;
        #if HW_DETECT_PWR_LOSS
        if(FtlDB.FBIdx==pDBPMBlkPage->NextFBCnt && FtlDB.PIdx==0)
        #else
        if(FtlDB.FBIdx==pDBPMBlkPage->NextFBCnt && FtlDB.LPIdx==0)
        #endif
        {
            err=NFTL_WriteDBPM();
            if(err)
                return err;
            FtlDB.FBIdx = 0;
        }

        // --------------------------
        if(PCID_RDB==FtlCtrl.u8_LastProtID && RDB_FINISH_WAIT==FtlCtrl.u8_LastProtSubID)
        {
            P_PCID_RDB_t pRDB_t = (P_PCID_RDB_t)pu8_PCIDBuf;
            //InitRDBPtr(pRDB_t);
            err=NFTL_WriteCtrlInfo_Ex(PCID_RDB, RDB_FINISH, (U8*)pRDB_t);
            if(err)
                return err;
        }
        else
        {
            err=NFTL_WriteCtrlTwice();
            if(err)
                return err;
        }
    }
    // pwr-cut flow
    else
    {
        err=NFTL_ErrHandle();
        if(err)
            return err;
    }
    
    // --------------------------
    // Power-Cut test
    #if PWROFF_EN
    do{
        u16_PwrCut_TrigID = HWTimer_End() % PWROFF_ID_CNT;       
     
    }while(u16_PwrCut_TrigID==0 || u16_PwrCut_TrigID==2);// add the IDs not be tested here

    ftl_dbg(FTL_DBG_LV_LOG,1,"[PWROFF_TEST] TrigID: %u \n", u16_PwrCut_TrigID);
    #endif

    // --------------------------
    // create background hk_thread (run NFTL_BKG_Housekeep)
    #if MSFTL_LINUX && defined(BKG_HK_THREAD) && BKG_HK_THREAD 
    err=NFTL_BKG_ThreadInit();
    if(err)
        return err;
    #endif  
        
    #if (defined(BKG_W_THREAD) && BKG_W_THREAD) && defined(TEST_INDEX) && TEST_INDEX!=bgw_tasklet
    FtlBkgWD.bkg_wdata= NULL;
    FtlBkgWD.bkg_wdata = kthread_run(NFTL_WriteData_Bkg, (void*)(&FtlBkgWD), "msftl_bkgw");
    if (IS_ERR(FtlBkgWD.bkg_wdata)) {
        ftl_dbg(FTL_DBG_LV_ERR, 1, "bkg w thread - initial fails\n");
        ftl_die();
    }
    //au8_BkgWDBuf = FMALLOC(BKGW_BUF_SECCNT<<9);
    FtlBkgWD.pBuf = (U8*)(((U32)au8_BkgWDBuf+(UNFD_CACHE_LINE-1)) & ~(UNFD_CACHE_LINE-1));
    spin_lock_init(&FtlBkgWD.spnlock);
    FtlBkgWD.WReqHead = FtlBkgWD.WReqTail = FtlBkgWD.WReqCnt = FtlBkgWD.RReqCnt = 0;
    FtlBkgWD.Buf512BHead = FtlBkgWD.Buf512BTail = 0;
    
    #elif (defined(BKG_W_THREAD) && BKG_W_THREAD) && defined(TEST_INDEX) && TEST_INDEX==bgw_tasklet
    tasklet_init(&ftl_bkgw_task, NFTL_WriteData_Bkg, 0);
    #endif

    // --------------------------
    #if HOT_DATA_EN
    err=NFTL_InitHotData();
    if(err)
        return err;
    #endif // HOT_DATA_EN
    
    return FTL_OK;
}


// ============================================
// ok: return 0
// u32_512BCnt: each sector is 512B
U32 NFTL_WriteData(U8* pu8_DataBuf, U32 u32_512BCnt, U32 u32_512BAddr)
{
    #if defined(TEST_INDEX) && TEST_INDEX == wt_gap
    //u64_DiffJiffies = jiffies_64-u64_PrevJiffies;
    do_gettimeofday(&TimeValS);
    if((U32)(TimeValS.tv_usec-TimeValE.tv_usec)/1000 < 100)//skip overflow
    {
        totalms += (TimeValS.tv_usec-TimeValE.tv_usec)/1000;
        printk("TG %u ms, %u ms \n", 
            (TimeValS.tv_usec-TimeValE.tv_usec)/1000, totalms);
    }
    #endif
        
    #if API_CHECK_PARAM
    if(NULL==pu8_DataBuf || 0==u32_512BCnt)
    {
        ftl_dbg(FTL_DBG_LV_ERR,1,"error input: Buf:%Xh Cnt:%Xh \n", (U32)pu8_DataBuf, u32_512BCnt);
        ftl_die();
        return FTL_ERR_PARAMETER;
    }
    if(u32_512BCnt+u32_512BAddr > NFTL_GetDataCapacity())
    {
        ftl_dbg(FTL_DBG_LV_ERR,1,"error range: Addr:%Xh + Cnt:%Xh > Size:%Xh\n", 
            u32_512BAddr, u32_512BCnt, NFTL_GetDataCapacity());
        ftl_die();
        return FTL_ERR_W_OUT_RANGE;
    }
    #endif

    ftl_dbg_api(FTL_DBG_LV_LOG,0,"\n");
    //ftl_dbg_api(FTL_DBG_LV_LOG,1,"512BAddr: %Xh  512BCnt: %Xh  buf: %Xh\n", u32_512BAddr, u32_512BCnt, (U32)pu8_DataBuf);
    ftl_dbg_api(FTL_DBG_LV_LOG,1,"512BAddr: %Xh  512BCnt: %Xh\n", u32_512BAddr, u32_512BCnt);
    
    // --------------------------
    // lock

    // --------------------------
    #if FTL_PROFILE_WAF
    u32_API512BCnt += u32_512BCnt;
    u32_API1MBCnt += u32_API512BCnt >>11;
    u32_API512BCnt -=  (u32_API512BCnt >>11) <<11;
    #endif
    
    // --------------------------
    // background write data
    #if (defined(BKG_W_THREAD) && BKG_W_THREAD) && (defined(TEST_INDEX) && TEST_INDEX==bgw_no_test)
    {volatile U16 IfQueueReady=0;
    volatile U32 Buf512BT;
     
    while(0==IfQueueReady)
    {
        spin_lock(&FtlBkgWD.spnlock);
        if( (FtlBkgWD.Buf512BTail>=FtlBkgWD.Buf512BHead && 
             FtlBkgWD.Buf512BTail+u32_512BCnt>BKGW_BUF_SECCNT &&
             FtlBkgWD.Buf512BHead<u32_512BCnt) ||
            (FtlBkgWD.Buf512BTail<FtlBkgWD.Buf512BHead && 
             FtlBkgWD.Buf512BHead-FtlBkgWD.Buf512BTail<u32_512BCnt) ||
            (FtlBkgWD.Buf512BTail==FtlBkgWD.Buf512BHead && 
             0!=FtlBkgWD.WReqCnt) ||
            UNIT_CNT==FtlBkgWD.WReqCnt ||
            FTL_CHK_FLAG(FTL_RREQ_WAITING))
            IfQueueReady = 0;
        else
        {
            IfQueueReady = 1;
            Buf512BT = FtlBkgWD.Buf512BTail;
        }
        spin_unlock(&FtlBkgWD.spnlock);

        if(0==IfQueueReady)
            //msleep(1);
            schedule();
    }
    FtlBkgWD.WSecAddr[FtlBkgWD.WReqTail] = u32_512BAddr;
    FtlBkgWD.WSecCnt[FtlBkgWD.WReqTail] = u32_512BCnt;
    if(Buf512BT+u32_512BCnt > BKGW_BUF_SECCNT)
        Buf512BT = 0;
    FtlBkgWD.WBufAddr[FtlBkgWD.WReqTail] = (U32)&FtlBkgWD.pBuf[Buf512BT<<9];
    Buf512BT += u32_512BCnt;
    if(Buf512BT == BKGW_BUF_SECCNT)
        Buf512BT = 0;
    memcpy((void*)FtlBkgWD.WBufAddr[FtlBkgWD.WReqTail], pu8_DataBuf, u32_512BCnt<<9);

    //ftl_dbg(0,1,"+ 512BAdr:%Xh  512BCnt:%Xh  %Xh\n", u32_512BAddr, u32_512BCnt,
    //    drvNAND_CheckSum(pu8_DataBuf, u32_512BCnt<<9));
    
    spin_lock(&FtlBkgWD.spnlock);
    //if(0x78==u32_512BCnt)
    //    ftl_dbg(0,1,"512BA: %Xh  512BCnt: %Xh \n", u32_512BAddr, u32_512BCnt);
    FtlBkgWD.WReqCnt++;
    FtlBkgWD.WReqTail++;
    if(FtlBkgWD.WReqTail == UNIT_CNT)
       FtlBkgWD.WReqTail = 0;
    FtlBkgWD.Buf512BTail = Buf512BT;
    spin_unlock(&FtlBkgWD.spnlock);
    wake_up(&nftl_w_wait);
    //NFTL_DumpBkgW(&FtlBkgWD);
    }
    // --------------------------
    #else
    NFTL_WriteData_Ex(pu8_DataBuf, u32_512BCnt, u32_512BAddr);
    #endif    

    // --------------------------
    // no background hk_thread
    #if MSFTL_LINUX && defined(BKG_HK_THREAD) && BKG_HK_THREAD 
    //wake_up_process(FtlCtrl.hk_thread);
    #endif

    //ftl_dbg_api(FTL_DBG_LV_LOG,1,"-\n");

    #if defined(TEST_INDEX) && TEST_INDEX==wt_gap
    u64_PrevJiffies = jiffies_64;
    do_gettimeofday(&TimeValE);
    
    #elif defined(TEST_INDEX) && TEST_INDEX>wt_gap
    do_gettimeofday(&TimeValE);
      #if TEST_INDEX == bgw_sleepon
    wake_up(&nftl_w_wait);
      #elif TEST_INDEX == bgw_waitevt
    ftl_wakeup = 1;
    wake_up(&fcie_wait);
      #elif TEST_INDEX == bgw_tasklet
    tasklet_schedule(&ftl_bkgw_task);
      #endif
    #endif // TEST_INDEX>wt_gap

    #if FTL_PROFILE_WAF
    {
        static U16 cnt=1;

        if(0==cnt%0xFFF0)
            NFTL_DumpECnt();

        cnt++;
    }
    #endif
    
    return FTL_OK;
}


// ok: return 0
// u32_512BCnt: each sector is 512B
U32 NFTL_ReadData(U8* pu8_DataBuf, U32 u32_512BCnt, U32 u32_512BAddr)
{
    #if API_CHECK_PARAM
    if(NULL==pu8_DataBuf || 0==u32_512BCnt)
    {
        ftl_dbg(FTL_DBG_LV_ERR,1,"error input: Buf:%Xh Cnt:%Xh \n", (U32)pu8_DataBuf, u32_512BCnt);
        ftl_die();
        return FTL_ERR_PARAMETER;
    }
    if(u32_512BCnt+u32_512BAddr > NFTL_GetDataCapacity())
    {
        ftl_dbg(FTL_DBG_LV_ERR,1,"error range: Addr:%Xh + Cnt:%Xh > Size:%Xh\n", 
            u32_512BAddr, u32_512BCnt, NFTL_GetDataCapacity());
        ftl_die();
        return FTL_ERR_R_OUT_RANGE;
    }
    #endif

    ftl_dbg_api(FTL_DBG_LV_LOG,0,"\n");
    ftl_dbg_api(FTL_DBG_LV_LOG,1,"+ 512BAddr: %Xh  512BCnt: %Xh  buf: %Xh\n", u32_512BAddr, u32_512BCnt, (U32)pu8_DataBuf);
    
    // --------------------------
    #if HOT_DATA_EN
    if(u32_512BAddr>>FtlDev.u8_Page512BCntBits == 
        (u32_512BAddr+u32_512BCnt-1) >> FtlDev.u8_Page512BCntBits)
    {
        U16 i, PSecIdx;
        U32 LRow = u32_512BAddr >> FtlDev.u8_Page512BCntBits;
        for(i=0; i<HDAT_TOTAL_PCNT; i++)
        {
            if(LRow!=FtlHotData.pHotLRow[i])
                continue;

            PSecIdx = u32_512BAddr & FtlDev.u16_Page512BCntMask;
            memcpy(pu8_DataBuf, 
                &FtlHotData.pHotDBuf[(i<<FtlDev.u8_PageByteCntBits)+(PSecIdx<<9)], 
                u32_512BCnt<<9);
            ftl_dbg(0,1,"hit: %Xh\n", LRow);
            goto LABEL_END;
        }
    }
      #if TEMP_CHECK
    NFTL_ChkHotData(u32_512BCnt, u32_512BAddr);
      #endif
    #endif // HOT_DATA_EN
    
    // check and set R_HPI here
    #if (defined(BKG_W_THREAD) && BKG_W_THREAD) && (defined(TEST_INDEX) && TEST_INDEX==bgw_no_test)
    {
    volatile U16 i, j, k, WReqIdx;
    volatile U32 WEndSecAdr, REndSecAdr, SecAdr1, SecAdr2;

    if(0==FtlBkgWD.WReqCnt)
    {
        nand_lock_fcie();
        NFTL_ReadData_Ex(pu8_DataBuf, u32_512BCnt, u32_512BAddr);
        nand_unlock_fcie();
        goto LABEL_END;
    }

    spin_lock(&FtlBkgWD.spnlock);    
    FtlBkgWD.RReqCnt = 1;
    FtlBkgWD.RSecAddr[0] = u32_512BAddr;
    FtlBkgWD.RSecCnt[0] = u32_512BCnt;
    FtlBkgWD.RBufAddr[0] = (U32)pu8_DataBuf;
    
    if(FtlBkgWD.WReqTail)
        WReqIdx = FtlBkgWD.WReqTail-1;
    else
        WReqIdx = UNIT_CNT-1;
        
    for(i=0; i<FtlBkgWD.WReqCnt && FtlBkgWD.RReqCnt; i++)
    {
        WEndSecAdr = FtlBkgWD.WSecAddr[WReqIdx] + FtlBkgWD.WSecCnt[WReqIdx];
        
        for(j=0; j<FtlBkgWD.RReqCnt; j++)
        {
            REndSecAdr = FtlBkgWD.RSecAddr[j] + FtlBkgWD.RSecCnt[j];

            // no overlap
            if(REndSecAdr <= FtlBkgWD.WSecAddr[WReqIdx] || 
                WEndSecAdr <= FtlBkgWD.RSecAddr[j])
                continue;

            // w-range covers r-range
            if(FtlBkgWD.WSecAddr[WReqIdx] <= FtlBkgWD.RSecAddr[j] && 
                WEndSecAdr >= REndSecAdr)
            {
                memcpy((void*)FtlBkgWD.RBufAddr[j], 
                    (void*)(FtlBkgWD.WBufAddr[WReqIdx]+
                      ((FtlBkgWD.RSecAddr[j]-FtlBkgWD.WSecAddr[WReqIdx])<<9)),
                    FtlBkgWD.RSecCnt[j]<<9);

                FtlBkgWD.RReqCnt -= 1;
                for(k=j; k<FtlBkgWD.RReqCnt; k++)
                {                    
                    FtlBkgWD.RSecAddr[k] = FtlBkgWD.RSecAddr[k+1];
                    FtlBkgWD.RSecCnt[k] = FtlBkgWD.RSecCnt[k+1];
                    FtlBkgWD.RBufAddr[k] = FtlBkgWD.RBufAddr[k+1];
                }
                j--;
            }
            // r-range covers w-range
            else if(FtlBkgWD.WSecAddr[WReqIdx] > FtlBkgWD.RSecAddr[j] && 
                WEndSecAdr < REndSecAdr)
            {
                FtlBkgWD.RSecCnt[j] = FtlBkgWD.WSecAddr[WReqIdx]-FtlBkgWD.RSecAddr[j];
                
                memcpy((void*)(FtlBkgWD.RBufAddr[j]+
                      ((FtlBkgWD.WSecAddr[WReqIdx]-FtlBkgWD.RSecAddr[j])<<9)), 
                    (void*)FtlBkgWD.WBufAddr[WReqIdx],
                    FtlBkgWD.WSecCnt[WReqIdx]<<9);

                FtlBkgWD.RSecAddr[FtlBkgWD.RReqCnt] = 
                    FtlBkgWD.RSecAddr[j]+FtlBkgWD.RSecCnt[j]+FtlBkgWD.WSecCnt[WReqIdx];
                FtlBkgWD.RSecCnt[FtlBkgWD.RReqCnt] = 
                    REndSecAdr-FtlBkgWD.RSecAddr[FtlBkgWD.RReqCnt];
                FtlBkgWD.RBufAddr[FtlBkgWD.RReqCnt] = FtlBkgWD.RBufAddr[j]+
                    ((FtlBkgWD.RSecCnt[j]+FtlBkgWD.WSecCnt[WReqIdx])<<9);
                FtlBkgWD.RReqCnt++;                
            }
            else // overlap (partial covered)
            {
                SecAdr1 = FtlBkgWD.WSecAddr[WReqIdx] > FtlBkgWD.RSecAddr[j] ?
                    FtlBkgWD.WSecAddr[WReqIdx] : FtlBkgWD.RSecAddr[j];
                SecAdr2 = WEndSecAdr < REndSecAdr ? WEndSecAdr : REndSecAdr;
                
                memcpy((void*)(FtlBkgWD.RBufAddr[j]+((SecAdr1-FtlBkgWD.RSecAddr[j])<<9)), 
                    (void*)(FtlBkgWD.WBufAddr[WReqIdx]+((SecAdr1-FtlBkgWD.WSecAddr[WReqIdx])<<9)),
                    (SecAdr2-SecAdr1)<<9);

                if(SecAdr1 == FtlBkgWD.RSecAddr[j])
                {
                    FtlBkgWD.RSecAddr[j] = SecAdr2;
                    FtlBkgWD.RBufAddr[j] = FtlBkgWD.RBufAddr[j] + ((SecAdr2-SecAdr1)<<9);
                }
                FtlBkgWD.RSecCnt[j] = FtlBkgWD.RSecCnt[j]-(SecAdr2-SecAdr1);
            }
        }

        if(WReqIdx)
            WReqIdx--;
        else
            WReqIdx = UNIT_CNT-1;
    }
    spin_unlock(&FtlBkgWD.spnlock);

    nand_lock_fcie();
    for(j=0; j<FtlBkgWD.RReqCnt; j++)
        NFTL_ReadData_Ex((U8*)FtlBkgWD.RBufAddr[j], FtlBkgWD.RSecCnt[j], FtlBkgWD.RSecAddr[j]);
    //ftl_dbg(0,0,"rc: %Xh\n", drvNAND_CheckSum(pu8_DataBuf, u32_512BCnt<<9));
    nand_unlock_fcie();
    }
    #else
    NFTL_ReadData_Ex(pu8_DataBuf, u32_512BCnt, u32_512BAddr);
    //ftl_dbg(0,0,"rc: %Xh\n", drvNAND_CheckSum(pu8_DataBuf, u32_512BCnt<<9));
    #endif


	#if (defined(BKG_W_THREAD) && BKG_W_THREAD) || HOT_DATA_EN
    LABEL_END:;
    #endif
    // --------------------------
    // background hk_thread
    #if READ_DISTURB

	#if MSFTL_LINUX
	nand_lock_fcie();
	NFTL_RD_Move();
	nand_unlock_fcie();
	#else
	NFTL_RD_Move();
	#endif

    #endif // READ_DISTURB
    
    ftl_dbg_api(FTL_DBG_LV_LOG,1,"-\n");
    return FTL_OK;
}


#if MSFTL_LINUX
U32 NFTL_ReadData_sg(struct msftl_sg *sg_buf, U32 u32_512BCnt, U32 u32_512BAddr, U32 u32_SgCnt)
{
    #if API_CHECK_PARAM
    if(u32_512BCnt+u32_512BAddr > NFTL_GetDataCapacity())
    {
        ftl_dbg(FTL_DBG_LV_ERR,1,"error range: Addr:%Xh + Cnt:%Xh > Size:%Xh\n", 
            u32_512BAddr, u32_512BCnt, NFTL_GetDataCapacity());
        ftl_die();
        return FTL_ERR_R_OUT_RANGE;
    }
    #endif

    // --------------------------    
    #if HOT_DATA_EN
    if(u32_512BAddr>>FtlDev.u8_Page512BCntBits == 
        (u32_512BAddr+u32_512BCnt-1) >> FtlDev.u8_Page512BCntBits)
    {
        U16 i, PSecIdx, sg_i;
        U32 LRow = u32_512BAddr >> FtlDev.u8_Page512BCntBits, SecCntTmp;
        U8 *pBuf;
        
        for(i=0; i<HDAT_TOTAL_PCNT; i++)
        {
            if(LRow!=FtlHotData.pHotLRow[i])
                continue;

            PSecIdx = u32_512BAddr & FtlDev.u16_Page512BCntMask;
            pBuf = &FtlHotData.pHotDBuf[(i<<FtlDev.u8_PageByteCntBits)+(PSecIdx<<9)];
            SecCntTmp = 0;            
            for(sg_i=0; sg_i<u32_SgCnt; sg_i++)
            {
                memcpy(sg_buf->buf[sg_i], pBuf+(SecCntTmp<<9), sg_buf->u32_512SectorCnt[sg_i]<<9);
                SecCntTmp += sg_buf->u32_512SectorCnt[sg_i];
            }            
            ftl_dbg(0,1,"hit: %Xh\n", LRow);
            goto LABEL_END;
        }
    }
      #if TEMP_CHECK
    NFTL_ChkHotData(u32_512BCnt, u32_512BAddr);
      #endif
    #endif // HOT_DATA_EN    

    // --------------------------
    // check and set R_HPI here
    #if (defined(BKG_W_THREAD) && BKG_W_THREAD) && (defined(TEST_INDEX) && TEST_INDEX==bgw_no_test)
    {
    #if 0
    volatile U16 IfQueueEmpty=0;

    while(0==IfQueueEmpty)
    {
        spin_lock(&FtlBkgWD.spnlock);
        if(0==FtlBkgWD.WReqCnt)
            IfQueueEmpty = 1;
        else
            IfQueueEmpty = 0;
        spin_unlock(&FtlBkgWD.spnlock);

        if(0==IfQueueEmpty)
        {
            FTL_SET_FLAG(FTL_RREQ_WAITING);
            schedule();
        }
    }
    #else
    while(FtlBkgWD.WReqCnt)
    {
        FTL_SET_FLAG(FTL_RREQ_WAITING);
        schedule();
    }
    #endif
    nand_lock_fcie();
    FTL_CLR_FLAG(FTL_RREQ_WAITING);
    NFTL_ReadData_sg_Ex(sg_buf, u32_512BCnt, u32_512BAddr,u32_SgCnt);
    nand_unlock_fcie();
    }
    #else
    NFTL_ReadData_sg_Ex(sg_buf, u32_512BCnt, u32_512BAddr,u32_SgCnt);
    //ftl_dbg(0,0,"rc: %Xh\n", drvNAND_CheckSum(pu8_DataBuf, u32_512BCnt<<9));
    #endif

    #if HOT_DATA_EN
    LABEL_END:;
    #endif
    // --------------------------
    // no background thread
    #if READ_DISTURB
	nand_lock_fcie();
	NFTL_RD_Move();
	nand_unlock_fcie();
    #endif // READ_DISTURB

    ftl_dbg_api(FTL_DBG_LV_LOG,1,"-\n");
    return FTL_OK;
}
#endif // MSFTL_LINUX

// return sector count (1 sector = 512B)
U32  NFTL_GetDataCapacity(void)
{
    // 1 sector = 512B
    return FtlCtrl.u16_DataBlkCnt << 
        (FtlDev.u8_BlkPageCntBits + FtlDev.u8_PageByteCntBits - 9);
}

