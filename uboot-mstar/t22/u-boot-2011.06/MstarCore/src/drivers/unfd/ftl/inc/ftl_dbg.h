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
#ifndef _FTL_DBG_H_
#define _FTL_DBG_H_

// =============================================
// ftl_dbg.c
// =============================================
extern void NFTL_DumpInfo(void);
extern void NFTL_DumpDevInfo(void);
extern U32  NFTL_CheckDBPCnt(void);
extern U32  NFTL_Die(void);

// ---------------------
#define FTL_PROFILE_WAF    0
#if FTL_PROFILE_WAF
extern U32 u32_API512BCnt, u32_API1MBCnt, u32_HAL512BCnt, u32_HAL1MBCnt;
#endif


// ---------------------
// env test
#define TEST_HW_TIMER      0


// ---------------------
// PWROFF_TEST 
extern  void NFTL_PwrOff(U16 u16_ID, U32 u32_MaxUs);

#define PWROFF_EN          0

#define PWROFF(ID, MAX_us) if(PWROFF_EN)  NFTL_PwrOff(ID, MAX_us);
#define PWROFF_Ex0()       if(PWROFF_EN && u32_PwrCut_TrigUs)  \
                               u32_PwrCut_InitUs = HWTimer_End()/(FTL_HW_TIMER_HZ/1000000);
#define PWROFF_Ex1()       if(PWROFF_EN && u32_PwrCut_TrigUs)  \
                               if(HWTimer_End()/(FTL_HW_TIMER_HZ/1000000)-u32_PwrCut_InitUs > u32_PwrCut_TrigUs)  \
                                    nand_SysPwrOff();
//[3.3V -> 2.5V] needs 4ms
#define MAX_DELAY_E        3//us
#define MAX_DELAY_P        1//us
extern  U16 u16_PwrCut_TrigID;
extern  U32 u32_PwrCut_InitUs, u32_PwrCut_TrigUs;

#define PWROFF_ID_WRB      0
#define PWROFF_ID_WRI      1
#define PWROFF_ID_WBBT     2
#define PWROFF_ID_WCB      3
#define PWROFF_ID_MCB      4
#define PWROFF_ID_WCI      5
#define PWROFF_ID_CNT      6


// =============================================
// debug log
// =============================================
#define API_CHECK_PARAM   1
#define TEMP_CHECK        0 // can be removed

#define DBG_ON            0
#define DBG_TRACE         (1&DBG_ON)
#define DBG_API           (0&DBG_ON)
#define DBG_CTRL          (1&DBG_ON)
#define DBG_ROOT          (1&DBG_ON)
#define DBG_DATA          (0&DBG_ON)
#define DBG_DATA_2        (1&DBG_ON)
#define DBG_DBPM          (1&DBG_ON)
#define DBG_WLRD          (1&DBG_ON)

#define ftl_dbg_wlrd(dbg_lv, tag, str, ...) \
    {if(DBG_WLRD) ftl_dbg(dbg_lv, tag, str, ##__VA_ARGS__);}

#define ftl_dbg_dbpm(dbg_lv, tag, str, ...) \
    {if(DBG_DBPM) ftl_dbg(dbg_lv, tag, str, ##__VA_ARGS__);}

#define ftl_dbg_d2(dbg_lv, tag, str, ...) \
    {if(DBG_DATA_2) ftl_dbg(dbg_lv, tag, str, ##__VA_ARGS__);}
    
#define ftl_dbg_data(dbg_lv, tag, str, ...) \
    {if(DBG_DATA) ftl_dbg(dbg_lv, tag, str, ##__VA_ARGS__);}

#define ftl_dbg_ctrl(dbg_lv, tag, str, ...) \
    {if(DBG_CTRL) ftl_dbg(dbg_lv, tag, str, ##__VA_ARGS__);}

#define ftl_dbg_root(dbg_lv, tag, str, ...) \
    {if(DBG_ROOT) ftl_dbg(dbg_lv, tag, str, ##__VA_ARGS__);}

#define ftl_dbg_api(dbg_lv, tag, str, ...) \
    {if(DBG_API) ftl_dbg(dbg_lv, tag, str, ##__VA_ARGS__);}

#define ftl_trace(str, ...)  {if(DBG_TRACE) ftl_dbg(FTL_DBG_LV_LOG,1, "trace "str, ##__VA_ARGS__);}


// =============================================
// ftl_test.c
// =============================================
extern U8 *pu8_FtlTestDatBuf;// = (U8*)DMA_W_ADDR; // 4MB
extern U8 *pu8_FtlTestTmpBuf;// = (U8*)DMA_R_ADDR; // 4MB
extern U8 *pu8_FtlTestRddBuf;// = (U8*)DMA_W_SPARE_ADDR;
// -----------------------------------------
extern U32  Ftl_Test_Case(U32 u32_Flag);
extern U32  Ftl_Test(U32 u32_Flag);
extern U32  Ftl_Burn(U32 u32_LoopCnt);
extern U32  MsNAND_Test(U16 u16_PBA);
// -----------------------------------------
#define NC_TEST_Init       1
#define NC_TEST_ResetCMD   2
#define NC_TEST_ReadID     3
#define NC_TEST_ReadP      4
#define NC_TEST_EraseB     5
#define NC_TEST_WriteP     6
#define NC_TEST_Write2P    7

// -----------------------------------------
extern U32  FtlTest_HAL(U16 u16_PBA);
extern U32  FtlTest_DataWRC(U32 u32_SectorAddr, U32 u32_SectorCnt, U32 seed);
extern U32  FtlTest_DataRC(U32 u32_SectorAddr, U32 u32_SectorCnt, U32 seed);
extern U32  FtlTest_DataWO(U32 u32_SectorAddr, U32 u32_SectorCnt, U32 seed);
extern U32  Ftl_Ext4BlkCnt(U32 tick);
// -----------------------------------------
extern U32  FtlTest_Performance(void);
extern U32  FtlTest_PwrCutTestInit(void);
extern U32  FtlTest_PwrCutTestRun(U32 LoopCnt);
extern U32  FtlTest_PwrCutTestCheck(void);
extern U32  FtlTest_PwrOffWaveform(void);
#define FTL_LIFETIME_TEST_FIXED     1
#define FTL_LIFETIME_TEST_FILLED    2
#define FTL_LIFETIME_TEST_RANDOM    3
extern void FtlTest_Lifetime(U8 TestMode);

#endif // _FTL_DBG_H_
