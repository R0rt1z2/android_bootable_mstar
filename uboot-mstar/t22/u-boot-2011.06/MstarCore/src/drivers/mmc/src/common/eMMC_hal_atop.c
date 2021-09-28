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

#include "eMMC.h"
#if defined(UNIFIED_eMMC_DRIVER) && UNIFIED_eMMC_DRIVER

U32 gu32_eMMCDrvExtFlag = 0;

U32 eMMC_hw_timer_delay(U32 u32us)
{
    #if 0
   volatile int i = 0;

    for (i = 0; i < (u32us>>0); i++)
    {
        #if 1
        volatile int j = 0, tmp;
        for (j = 0; j < 0x2; j++)
        {
            tmp = j;
        }
        #endif
    }
    #else
    volatile U32 u32_i=u32us;
    //udelay((u32us>>1)+(u32us>>4)+1);
    while(u32_i>1000)
    {
        udelay(1000);
        u32_i-=1000;
    }
    udelay(u32_i);
    #endif
    return u32us;
}

U32 eMMC_hw_timer_sleep(U32 u32ms)
{
    U32 u32_us = u32ms*1000;

    while(u32_us > 1000)
    {
        eMMC_hw_timer_delay(1000); // uboot has no context switch
        u32_us -= 1000;
    }

    eMMC_hw_timer_delay(u32_us);
    return u32ms;
}

U32 eMMC_hw_timer_start(void)
{
    // Reset PIU Timer1
    REG_FCIE_W(TIMER1_MAX_LOW, 0xFFFF);
    REG_FCIE_W(TIMER1_MAX_HIGH, 0xFFFF);
    REG_FCIE_W(TIMER1_ENABLE, 0);

    // Start PIU Timer1
    REG_FCIE_SETBIT(TIMER1_ENABLE, 0x0001);
    return 0;
}

U32 eMMC_hw_timer_tick(void)
{
    U32 u32HWTimer = 0;
    volatile U16 u16TimerLow = 0;
    volatile U16 u16TimerHigh = 0;

    REG_FCIE_R(TIMER1_CAP_LOW, u16TimerLow);
    REG_FCIE_R(TIMER1_CAP_HIGH, u16TimerHigh);

    u32HWTimer = (u16TimerHigh<<16) | u16TimerLow;

    return u32HWTimer;
}

//------------------------------------
void eMMC_set_WatchDog(U8 u8_IfEnable)
{
    // do nothing
}

void eMMC_reset_WatchDog(void)
{
    // do nothing
}

//--------------------------------
U32 eMMC_translate_DMA_address_Ex(U32 u32_DMAAddr, U32 u32_ByteCnt)
{
    flush_cache(u32_DMAAddr, u32_ByteCnt);
    #if defined(CONFIG_MIPS32)

    Chip_Flush_Memory();
    return  (u32_DMAAddr & 0x1fffffff);

    #elif CONFIG_MCU_ARM
    //eMMC_debug(0,0,"MIU0[%Xh], MIU1[%Xh]\n", CONFIG_MIU0_BUSADDR, CONFIG_MIU1_BUSADDR);

    #ifdef IP_FCIE_VERSION_5
        REG_FCIE_CLRBIT(FCIE_MMA_PRI_REG, BIT_MIU_SELECT_MASK);
    #endif

	#if !(defined(FCIE_NO_MIU_SELECT) && FCIE_NO_MIU_SELECT)
    if((MsOS_VA2PA(u32_DMAAddr)& CONFIG_SYS_MIU_INTERVAL)==CONFIG_SYS_MIU_INTERVAL)//MIU1
    {
        //printf("MIU1 ");
        #ifdef IP_FCIE_VERSION_5
            REG_FCIE_SETBIT(FCIE_MMA_PRI_REG, BIT_MIU1_SELECT);
        #else
            REG_FCIE_SETBIT(FCIE_MIU_DMA_26_16,BIT_MIU1_SELECT);
        #endif
        return (MsOS_VA2PA(u32_DMAAddr) - CONFIG_SYS_MIU_INTERVAL);
    }
    else // MIU0
	#endif
    {
        //printf("MIU0 ");
        #ifdef IP_FCIE_VERSION_5
            //REG_FCIE_SETBIT(FCIE_MMA_PRI_REG, BIT_MIU1_SELECT);
        #else
            REG_FCIE_CLRBIT(FCIE_MIU_DMA_26_16,BIT_MIU1_SELECT);
        #endif
        return MsOS_VA2PA(u32_DMAAddr);
    }

    // TODO: ask namo how to handle MIU2
    /*
        #ifdef IP_FCIE_VERSION_5
            REG_FCIE_SETBIT(FCIE_MMA_PRI_REG, BIT_MIU2_SELECT);
        #else
    */

    #endif
}

void eMMC_flush_data_cache_buffer(U32 u32_DMAAddr, U32 u32_ByteCnt)
{
    //flush_cache(u32_DMAAddr, u32_ByteCnt);
}

void eMMC_Invalidate_data_cache_buffer(U32 u32_DMAAddr, U32 u32_ByteCnt)
{
    #if defined(NEED_POST_INVALIDATE_CACHE) && NEED_POST_INVALIDATE_CACHE
    invalidate_dcache_range ((unsigned long)u32_DMAAddr, (unsigned long)u32_ByteCnt);
    #endif
}

void eMMC_flush_miu_pipe(void)
{

}

void eMMC_CheckPowerCut(void)
{
}

//--------------------------------
#if defined(ENABLE_eMMC_ATOP) && ENABLE_eMMC_ATOP
extern void eMMC_FCIE_SetATopTimingReg(U8 u8_SetIdx);

#ifdef IP_FCIE_VERSION_5
U8 gau8_eMMCPLLSel_52[eMMC_FCIE_VALID_CLK_CNT] = {
    eMMC_PLL_CLK_200M,
	eMMC_PLL_CLK_160M,
	eMMC_PLL_CLK_120M
};
#else
U8 gau8_eMMCPLLSel_52[eMMC_FCIE_VALID_CLK_CNT] = {
	eMMC_PLL_CLK__52M,
	eMMC_PLL_CLK__32M,
    eMMC_PLL_CLK__27M
};
#endif

U8 gau8_eMMCPLLSel_200[eMMC_FCIE_VALID_CLK_CNT] = {
    eMMC_PLL_CLK_200M,
    eMMC_PLL_CLK_160M,
    eMMC_PLL_CLK_140M
};

static  eMMC_FCIE_ATOP_SET_t sgTSetTmp_t;

#ifdef IP_FCIE_VERSION_5
static U32 eMMC_FCIE_DetectDDRTiming_Ex(U8 u8_DQS, U8 u8_DelaySel)
{
    U32 u32_SectorAddr;

    u32_SectorAddr = eMMC_TEST_BLK_0;
    g_eMMCDrv.TimingTable_t.Set[0].u8_Skew4=u8_DQS;
    g_eMMCDrv.TimingTable_t.Set[0].u8_Cell=u8_DelaySel;

    eMMC_FCIE_SetATopTimingReg(0);
    return eMMCTest_BlkWRC_ProbeTiming(u32_SectorAddr);
}
#endif

#if (defined(ENABLE_eMMC_HS200) && ENABLE_eMMC_HS200) || \
    (defined(ENABLE_eMMC_HS400) && ENABLE_eMMC_HS400)

U32 eMMC_FCIE_DetectTiming_Ex(U32 u32_Skew4Result)
{
    U16 u16_i, u16_StartBit=0xFFFF, u16_EndBit=0xFFFF, u16_cnt;
    U32 u32_tmp;

    sgTSetTmp_t.u32_ScanResult = u32_Skew4Result;

    // ==================================================
    // case.1: all bits set
    if((u32_Skew4Result & ((1<<PLL_SKEW4_CNT*2)-1)) == ((1<<PLL_SKEW4_CNT*2)-1) )
    {
        //eMMC_debug(eMMC_DEBUG_LEVEL,0,"  case1: all\n");
        sgTSetTmp_t.u8_Reg2Ch=1;
        sgTSetTmp_t.u8_Skew4=0;
    }
    // ==================================================
    // case.2: not wrapped
    else if( 0==(u32_Skew4Result&BIT0) || 0==(u32_Skew4Result&(1<<(PLL_SKEW4_CNT*2-1))) )
    {
        for(u16_i=0; u16_i<PLL_SKEW4_CNT*2; u16_i++)
        {
            if((u32_Skew4Result&1<<u16_i) && 0xFFFF==u16_StartBit)
                u16_StartBit = u16_i;
            else if(0xFFFF!=u16_StartBit && 0==(u32_Skew4Result&1<<u16_i))
                u16_EndBit = u16_i-1;

            if(0xFFFF != u16_EndBit)
            {
                if(u16_EndBit-u16_StartBit+1<MIN_OK_SKEW_CNT)
                {   // to ignore "discontinous case"
                    u16_StartBit = 0xFFFF;
                    u16_EndBit = 0xFFFF;
                }
                else
                    break;
            }
        }
        if(0xFFFF==u16_EndBit)
        {
            if( u32_Skew4Result&1<<(u16_i-1) )
                u16_EndBit = u16_i-1;
            else
                u16_EndBit = u16_StartBit;
        }
        //eMMC_debug(eMMC_DEBUG_LEVEL,0,"  case2: not wrapped: %u %u\n", u16_StartBit, u16_EndBit);

        if(u16_EndBit-u16_StartBit+1<MIN_OK_SKEW_CNT)
        {
            eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1, "eMMC Err: Clk:%uMHz Case2 not enough skew4: %Xh %Xh\n",
                g_eMMCDrv.u32_ClkKHz/1000, u32_Skew4Result, u16_EndBit-u16_StartBit+1);
            return eMMC_ST_ERR_SKEW4;
        }

        if( (u16_StartBit+u16_EndBit)/2 < PLL_SKEW4_CNT)
        {
            sgTSetTmp_t.u8_Reg2Ch=0;
            sgTSetTmp_t.u8_Skew4=(u16_StartBit+u16_EndBit)/2;
        }
        else
        {   sgTSetTmp_t.u8_Reg2Ch=1;
            sgTSetTmp_t.u8_Skew4=(u16_StartBit+u16_EndBit)/2 -PLL_SKEW4_CNT;
        }
    }
    // ==================================================
    // case.3: wrapped
    else
    {   // --------------------------------
        // to ignore "discontinous case"
        u16_cnt = 0;
        for(u16_i=0; u16_i<PLL_SKEW4_CNT*2; u16_i++)
        {
            if(u32_Skew4Result&1<<u16_i)
                u16_cnt++;
            else
            {   u16_StartBit = u16_i-1;
                break;
            }
        }
        for(u16_i=PLL_SKEW4_CNT*2-1; u16_i>0; u16_i--)
        {
            if(u32_Skew4Result&1<<u16_i)
                u16_cnt++;
            else
            {   u16_EndBit = u16_i+1;
                break;
            }
        }
        if(u16_cnt < MIN_OK_SKEW_CNT)
        {
            //eMMC_debug(eMMC_DEBUG_LEVEL,0,"  case3: wrapped but goto case2: %u, %u %u\n",
                //u16_cnt, u16_StartBit, u16_EndBit);
            u32_Skew4Result &= ~((1<<(u16_StartBit+1))-1);
            return eMMC_FCIE_DetectTiming_Ex(u32_Skew4Result);
        }
        else
        {   u32_tmp = u32_Skew4Result;
            for(u16_i=u16_StartBit+1; u16_i<u16_EndBit; u16_i++)
                u32_Skew4Result &= ~(1<<u16_i);

            u16_StartBit = 0xFFFF;
            u16_EndBit = 0xFFFF;
        }

        // --------------------------------
        // normal judgement
        for(u16_i=0; u16_i<PLL_SKEW4_CNT*2; u16_i++)
        {
            if(0==(u32_Skew4Result&1<<u16_i) && 0xFFFF==u16_StartBit)
                u16_StartBit = u16_i-1;
            else if(0xFFFF!=u16_StartBit && (u32_Skew4Result&1<<u16_i))
                u16_EndBit = u16_i;

            if(0xFFFF != u16_EndBit)
                break;
        }
        //eMMC_debug(eMMC_DEBUG_LEVEL,0,"  case3: wrapped: %u %u, %Xh %Xh\n",
            //u16_StartBit, u16_EndBit, u32_tmp, u32_Skew4Result);

        if(u16_StartBit+1 > PLL_SKEW4_CNT*2-u16_EndBit)
        {
            sgTSetTmp_t.u8_Reg2Ch=0;
            sgTSetTmp_t.u8_Skew4 = (u16_StartBit-(PLL_SKEW4_CNT*2-u16_EndBit))/2;
        }
        else
        {   sgTSetTmp_t.u8_Reg2Ch=1;
            sgTSetTmp_t.u8_Skew4 = u16_EndBit +
                ((PLL_SKEW4_CNT*2-u16_EndBit)+u16_StartBit)/2 -
                PLL_SKEW4_CNT;
        }
    }

    //eMMC_debug(eMMC_DEBUG_LEVEL, 0, "  %Xh %Xh \n", sgTSetTmp_t.u8_Reg2Ch, sgTSetTmp_t.u8_Skew4);
    if(sgTSetTmp_t.u8_Skew4 >= PLL_SKEW4_CNT)
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: Skew4:%u > %u\n", sgTSetTmp_t.u8_Skew4, PLL_SKEW4_CNT);

    #if 0 // We should scan skew -2 ~ +2 for U02
    sgTSetTmp_t.u8_Skew4 -= 2;
    #endif

    return eMMC_ST_SUCCESS;
}
#endif

#ifdef IP_FCIE_VERSION_5
#define FCIE_DELAY_CELL_ts         300 // 0.3ns
static eMMC_FCIE_DDRT_WINDOW_t sg_DDRTWindow[2];
#endif

U32 eMMC_FCIE_DetectDDRTiming(void)
{
    #ifdef IP_FCIE_VERSION_5
    U8  u8_dqs, u8_delay_sel, u8_i;
    U8  u8_dqs_prev=0xFF, u8_delay_sel_prev=0;
    U8  au8_DQSRegVal[8]={0,1,2,3,4,5,6,7}; // 0T, 0.5T, 1T, 1.5T, 2T, 2.5T, 3T, 3.5T
    U8  au8_DQS_10T[8]={0,5,10,15,20,25,30,35}; // 0T, 0.5T, 1T, 1.5T, 2T, 2.5T, 3T, 3.5T
    U8  u8_delay_Sel_max;
    U32 u32_ts;
    U32 u32_err;
    eMMC_FCIE_DDRT_WINDOW_t *pWindow = &sg_DDRTWindow[0];

    g_eMMCDrv.u32_DrvFlag |= DRV_FLAG_TUNING_TTABLE; // just to turn off some log

    //--------------------------------------------------
    sg_DDRTWindow[0].u8_Cnt = 0;
    sg_DDRTWindow[0].aParam[0].u8_DQS = 0;
    sg_DDRTWindow[0].aParam[1].u8_DQS = 0;
    sg_DDRTWindow[1].u8_Cnt = 0;
    sg_DDRTWindow[1].aParam[0].u8_DQS = 0;
    sg_DDRTWindow[1].aParam[1].u8_DQS = 0;
    for(u8_i=1; u8_i <= BIT_DQS_MODE_MASK>>BIT_DQS_MDOE_SHIFT; u8_i++)
    {
        sg_DDRTWindow[0].au8_DQSValidCellCnt[u8_i]=0;
        sg_DDRTWindow[1].au8_DQSValidCellCnt[u8_i]=0;
    }

    //--------------------------------------------------
    // calculate delay_Sel_max
    u32_ts = 1000*1000*1000 / g_eMMCDrv.u32_ClkKHz;
    u32_ts >>= 2; // for 4X's 1T

    //--------------------------------------------------
    // no need to try DQS of no delay
    //for(u8_dqs=0; u8_dqs<=(BIT_DQS_MODE_MASK>>BIT_DQS_MDOE_SHIFT); u8_dqs++)
    for(u8_dqs=1; u8_dqs<=(BIT_DQS_MODE_MASK>>BIT_DQS_MDOE_SHIFT); u8_dqs++)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_HIGH,0,"\n---------------------------\n");

        if(u8_dqs < (BIT_DQS_MODE_MASK>>BIT_DQS_MDOE_SHIFT))
            u8_delay_Sel_max =
                (au8_DQS_10T[u8_dqs+1]-au8_DQS_10T[u8_dqs])
                *u32_ts/(FCIE_DELAY_CELL_ts*10);
        else
            u8_delay_Sel_max = (BIT_DQS_DELAY_CELL_MASK>>BIT_DQS_DELAY_CELL_SHIFT);

        if(u8_delay_Sel_max > (BIT_DQS_DELAY_CELL_MASK>>BIT_DQS_DELAY_CELL_SHIFT))
        {
            u8_delay_Sel_max = (BIT_DQS_DELAY_CELL_MASK>>BIT_DQS_DELAY_CELL_SHIFT);
            eMMC_debug(eMMC_DEBUG_LEVEL_HIGH,0,"max delay cell: %u\n", u8_delay_Sel_max);
            //eMMC_debug(eMMC_DEBUG_LEVEL_HIGH,0,"invalid, not try\n\n");
            //continue;
        }
        else
            eMMC_debug(eMMC_DEBUG_LEVEL_HIGH,0,"max delay cell: %u\n\n", u8_delay_Sel_max);

        sg_DDRTWindow[0].au8_DQSTryCellCnt[u8_dqs] = u8_delay_Sel_max;
        sg_DDRTWindow[1].au8_DQSTryCellCnt[u8_dqs] = u8_delay_Sel_max;

        //for(u8_delay_sel=0; u8_delay_sel<=(BIT_DQS_DELAY_CELL_MASK>>BIT_DQS_DELAY_CELL_SHIFT); u8_delay_sel++)
        for(u8_delay_sel=0; u8_delay_sel<=u8_delay_Sel_max; u8_delay_sel++)
        {
            u32_err = eMMC_FCIE_DetectDDRTiming_Ex(au8_DQSRegVal[u8_dqs], u8_delay_sel);
            if(eMMC_ST_SUCCESS == u32_err)
            {
                eMMC_debug(eMMC_DEBUG_LEVEL_HIGH,0,"  PASS\n\n");
                pWindow->u8_Cnt++;
                if(0 == pWindow->aParam[0].u8_DQS) // save the window head
                {
                    pWindow->aParam[0].u8_DQS = u8_dqs; // dqs uses index
                    pWindow->aParam[0].u8_Cell = u8_delay_sel;
                }
                pWindow->au8_DQSValidCellCnt[u8_dqs]++;

                u8_dqs_prev = u8_dqs;
                u8_delay_sel_prev = u8_delay_sel;
            }
            else
            {   // save the window tail
                if(0xFF != u8_dqs_prev)
                {
                    pWindow->aParam[1].u8_DQS = u8_dqs_prev; // dqs uses index
                    pWindow->aParam[1].u8_Cell = u8_delay_sel_prev;
                }
                u8_dqs_prev = 0xFF;

                // discard & re-use the window having less PASS cnt
                pWindow =
                    (sg_DDRTWindow[0].u8_Cnt < sg_DDRTWindow[1].u8_Cnt) ?
                    &sg_DDRTWindow[0] : &sg_DDRTWindow[1];
                pWindow->u8_Cnt = 0;
                pWindow->aParam[0].u8_DQS = 0;
                pWindow->aParam[1].u8_DQS = 0;
                for(u8_i=1; u8_i <= BIT_DQS_MODE_MASK>>BIT_DQS_MDOE_SHIFT; u8_i++)
                    pWindow->au8_DQSValidCellCnt[u8_i]=0;
            }
        }
    }

    // for the case of last try is ok
    if(0xFF != u8_dqs_prev)
    {
        pWindow->aParam[1].u8_DQS = u8_dqs_prev; // dqs uses index
        pWindow->aParam[1].u8_Cell = u8_delay_sel_prev;
    }

    g_eMMCDrv.u32_DrvFlag &= ~DRV_FLAG_TUNING_TTABLE;

    eMMC_debug(eMMC_DEBUG_LEVEL_HIGH,0,"\n W0, Cnt:%Xh, [%Xh %Xh], [%Xh %Xh]\n",
        sg_DDRTWindow[0].u8_Cnt,
        sg_DDRTWindow[0].aParam[0].u8_DQS, sg_DDRTWindow[0].aParam[0].u8_Cell,
        sg_DDRTWindow[0].aParam[1].u8_DQS, sg_DDRTWindow[0].aParam[1].u8_Cell);
    for(u8_i=1; u8_i <= BIT_DQS_MODE_MASK>>BIT_DQS_MDOE_SHIFT; u8_i++)
        eMMC_debug(eMMC_DEBUG_LEVEL_HIGH,0,"DQSValidCellCnt[%u]:%u \n",
            u8_i, sg_DDRTWindow[0].au8_DQSValidCellCnt[u8_i]);

    eMMC_debug(eMMC_DEBUG_LEVEL_HIGH,0,"\n W1, Cnt:%Xh, [%Xh %Xh], [%Xh %Xh]\n",
        sg_DDRTWindow[1].u8_Cnt,
        sg_DDRTWindow[1].aParam[0].u8_DQS, sg_DDRTWindow[1].aParam[0].u8_Cell,
        sg_DDRTWindow[1].aParam[1].u8_DQS, sg_DDRTWindow[1].aParam[1].u8_Cell);
    for(u8_i=1; u8_i <= BIT_DQS_MODE_MASK>>BIT_DQS_MDOE_SHIFT; u8_i++)
        eMMC_debug(eMMC_DEBUG_LEVEL_HIGH,0,"DQSValidCellCnt[%u]:%u \n",
            u8_i, sg_DDRTWindow[1].au8_DQSValidCellCnt[u8_i]);

    if(sg_DDRTWindow[0].u8_Cnt || sg_DDRTWindow[1].u8_Cnt)
        return eMMC_ST_SUCCESS;
    else
        return eMMC_ST_ERR_NO_OK_DDR_PARAM;
    #else
	U32 u32_i, u32_err, u32_Skew4Result=0;
	U16 u16_SkewCnt=0;

	g_eMMCDrv.u32_DrvFlag |= DRV_FLAG_TUNING_TTABLE; // turn on tunning flag

	REG_FCIE_CLRBIT(FCIE_SM_STS, BIT11);
	for(u32_i=0; u32_i<PLL_SKEW4_CNT; u32_i++)
	{
		// make sure a complete outside clock cycle
		REG_FCIE_CLRBIT(FCIE_SD_MODE, BIT_SD_CLK_EN);

		REG_FCIE_CLRBIT(reg_emmcpll_0x03, BIT_SKEW4_MASK);
		REG_FCIE_SETBIT(reg_emmcpll_0x03, u32_i<<12);
		u32_err = eMMCTest_BlkWRC_ProbeTiming(eMMC_TEST_BLK_0);
		if(eMMC_ST_SUCCESS==u32_err)
		{
			u16_SkewCnt++;
			u32_Skew4Result |= (1<<u32_i);
		}
	}

	REG_FCIE_SETBIT(FCIE_SM_STS, BIT11);
	for(u32_i=PLL_SKEW4_CNT; u32_i<PLL_SKEW4_CNT*2; u32_i++)
	{
		// make sure a complete outside clock cycle
		REG_FCIE_CLRBIT(FCIE_SD_MODE, BIT_SD_CLK_EN);

		REG_FCIE_CLRBIT(reg_emmcpll_0x03, BIT_SKEW4_MASK);
		REG_FCIE_SETBIT(reg_emmcpll_0x03, (u32_i-PLL_SKEW4_CNT)<<12);
		u32_err = eMMCTest_BlkWRC_ProbeTiming(eMMC_TEST_BLK_0);
		if(eMMC_ST_SUCCESS==u32_err)
		{
			u16_SkewCnt++;
			u32_Skew4Result |= (1<<u32_i);
		}
	}

	g_eMMCDrv.u32_DrvFlag &= ~DRV_FLAG_TUNING_TTABLE; // turn off tunning flag

	if(0==u32_Skew4Result || u16_SkewCnt<MIN_OK_SKEW_CNT)
	{
		eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: Clk: %uMHz SkewResult: %Xh SkewCnt: %u\n",
			g_eMMCDrv.u32_ClkKHz/1000, u32_Skew4Result, u16_SkewCnt);
		return eMMC_ST_ERR_SKEW4;
	}

    return eMMC_FCIE_DetectTiming_Ex(u32_Skew4Result);
    #endif
}


U32 eMMC_FCIE_BuildDDRTimingTable(void)
{
    #ifdef IP_FCIE_VERSION_5
    U8  au8_DQSRegVal[8]={0,1,2,3,4,5,6,7}; // 0T, 0.5T, 1T, 1.5T, 2T, 2.5T, 3T, 3.5T
    U8  u8_i, u8_ClkIdx, u8_SetIdx, u8_tmp, u8_DqsIdx = 0, u8_CellBase;
    U32 u32_err, u32_ret;
    eMMC_FCIE_DDRT_WINDOW_t *pWindow;

    eMMC_debug(eMMC_DEBUG_LEVEL,1,"eMMC Info: building DDR table, please wait... \n");
    memset((void*)&g_eMMCDrv.TimingTable_t, '\0', sizeof(g_eMMCDrv.TimingTable_t));
    u8_SetIdx = 0;

    if(!eMMC_IF_NORMAL_SDR())
    {
        //eMMC_debug(0,1,"eMMC: re-init to SDR\n");
        g_eMMCDrv.u32_DrvFlag = 0;
        eMMC_PlatformInit();
        u32_err = eMMC_FCIE_Init();
        if(u32_err)
        {
            eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: eMMC_FCIE_Init fail, %Xh\n", u32_err);
            return u32_err;
        }
        u32_err = eMMC_Init_Device_Ex();
        if(eMMC_ST_SUCCESS != u32_err)
        {
            eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,0,"eMMC Err: eMMC_Init_Device_Ex fail: %X\n", u32_err);
            return u32_err;
        }
    }

    //eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC WARN: SDR mode can't detect DDR timing\n");
    u32_err = eMMC_FCIE_EnableFastMode_Ex(FCIE_eMMC_DDR);
    if(eMMC_ST_SUCCESS != u32_err)
    {
        eMMC_debug(0,0,"eMMC Err: set DDR IF fail: %X\n", u32_err);
        return u32_err;
    }

    for(u8_ClkIdx=0; u8_ClkIdx<eMMC_FCIE_VALID_CLK_CNT; u8_ClkIdx++)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_LOW,0,"=================================\n");
        eMMC_clock_setting(gau8_eMMCPLLSel_52[u8_ClkIdx]);
        eMMC_debug(eMMC_DEBUG_LEVEL_LOW,0,"=================================\n");

        // ---------------------------
        // search and set the Windows
        u32_err = eMMC_FCIE_DetectDDRTiming();

        // ---------------------------
        // set the Table
        if(eMMC_ST_SUCCESS == u32_err)
        {
            g_eMMCDrv.TimingTable_t.u8_SetCnt++;
            g_eMMCDrv.TimingTable_t.Set[u8_SetIdx].u8_Clk = gau8_eMMCPLLSel_52[u8_ClkIdx];
            // ---------------------------
            // select Window
            pWindow = NULL;

            // pick up the Window of Cell=0 case
            if(sg_DDRTWindow[0].aParam[0].u8_DQS != sg_DDRTWindow[0].aParam[1].u8_DQS &&
               sg_DDRTWindow[1].aParam[0].u8_DQS == sg_DDRTWindow[1].aParam[1].u8_DQS)
               pWindow = &sg_DDRTWindow[0];
            else if(sg_DDRTWindow[0].aParam[0].u8_DQS == sg_DDRTWindow[0].aParam[1].u8_DQS &&
               sg_DDRTWindow[1].aParam[0].u8_DQS != sg_DDRTWindow[1].aParam[1].u8_DQS)
               pWindow = &sg_DDRTWindow[1];
            else if(sg_DDRTWindow[0].aParam[0].u8_DQS != sg_DDRTWindow[0].aParam[1].u8_DQS &&
               sg_DDRTWindow[1].aParam[0].u8_DQS != sg_DDRTWindow[1].aParam[1].u8_DQS)
               pWindow =
                    (sg_DDRTWindow[0].u8_Cnt > sg_DDRTWindow[1].u8_Cnt) ?
                    &sg_DDRTWindow[0] : &sg_DDRTWindow[1];

            // ---------------------------
            if(NULL != pWindow)
            {
                // pick up the DQS having max valid cell
                u8_tmp = 0;
                for(u8_i=1; u8_i <= BIT_DQS_MODE_MASK>>BIT_DQS_MDOE_SHIFT; u8_i++)
                {
                    if(u8_tmp <= pWindow->au8_DQSValidCellCnt[u8_i]){
                        u8_tmp = pWindow->au8_DQSValidCellCnt[u8_i];
                        u8_DqsIdx = u8_i;
                    }
                }
                if(0 != u8_DqsIdx) // do not use 0T, this should be always TRUE
                {
                    if(pWindow->au8_DQSValidCellCnt[u8_DqsIdx-1] >= 7 &&
                        pWindow->au8_DQSValidCellCnt[u8_DqsIdx] >= 7)
                    {
                        g_eMMCDrv.TimingTable_t.Set[u8_SetIdx].u8_Skew4 = au8_DQSRegVal[u8_DqsIdx];
                        g_eMMCDrv.TimingTable_t.Set[u8_SetIdx].u8_Cell = 0; // nice
                    }
                    else
                    {
                        u8_tmp = (pWindow->au8_DQSValidCellCnt[u8_DqsIdx] +
                            pWindow->au8_DQSValidCellCnt[u8_DqsIdx-1]) / 2;


                        if(u8_tmp < pWindow->au8_DQSValidCellCnt[u8_DqsIdx-1])
                        {
                            g_eMMCDrv.TimingTable_t.Set[u8_SetIdx].u8_Skew4 =
                                au8_DQSRegVal[u8_DqsIdx-1];

                            u8_CellBase = pWindow->au8_DQSTryCellCnt[u8_DqsIdx-1] - pWindow->au8_DQSValidCellCnt[u8_DqsIdx-1];
                            g_eMMCDrv.TimingTable_t.Set[u8_SetIdx].u8_Cell =
                                u8_CellBase + pWindow->au8_DQSValidCellCnt[u8_DqsIdx-1] +
                                pWindow->au8_DQSValidCellCnt[u8_DqsIdx] - u8_tmp;
                        }
                        else
                        {   g_eMMCDrv.TimingTable_t.Set[u8_SetIdx].u8_Skew4 =
                                au8_DQSRegVal[u8_DqsIdx];
                            g_eMMCDrv.TimingTable_t.Set[u8_SetIdx].u8_Cell =
                                (pWindow->au8_DQSValidCellCnt[u8_DqsIdx-1] +
                                pWindow->au8_DQSValidCellCnt[u8_DqsIdx]) / 2;
                        }
                    }
                }
            }
            else //if(NULL == pWindow)
            {
                pWindow =
                    (sg_DDRTWindow[0].u8_Cnt > sg_DDRTWindow[1].u8_Cnt) ?
                    &sg_DDRTWindow[0] : &sg_DDRTWindow[1];

                g_eMMCDrv.TimingTable_t.Set[u8_SetIdx].u8_Skew4 = au8_DQSRegVal[pWindow->aParam[0].u8_DQS];
                g_eMMCDrv.TimingTable_t.Set[u8_SetIdx].u8_Cell =
                    (pWindow->aParam[0].u8_Cell + pWindow->aParam[1].u8_Cell)/2;
            }

            g_eMMCDrv.TimingTable_t.u32_VerNo = eMMC_TIMING_TABLE_VERSION;
            eMMC_DumpTimingTable();
            eMMC_debug(eMMC_DEBUG_LEVEL, 0, "\nOK\n");
            g_eMMCDrv.TimingTable_t.u32_ChkSum =
                eMMC_ChkSum((U8*)&g_eMMCDrv.TimingTable_t,
                sizeof(g_eMMCDrv.TimingTable_t)-eMMC_TIMING_TABLE_CHKSUM_OFFSET);
            memcpy(gau8_eMMC_SectorBuf, (U8*)&g_eMMCDrv.TimingTable_t, sizeof(g_eMMCDrv.TimingTable_t));

            eMMC_FCIE_ApplyTimingSet(eMMC_TIMING_SET_MAX);

            u32_err = eMMC_CMD24(eMMC_DDRTABLE_BLK_0, gau8_eMMC_SectorBuf);
            u32_ret = eMMC_CMD24(eMMC_DDRTABLE_BLK_1, gau8_eMMC_SectorBuf);
            if(eMMC_ST_SUCCESS!=u32_err && eMMC_ST_SUCCESS!=u32_ret)
            {
                eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: Save TTable fail: %Xh %Xh\n",
                    u32_err, u32_ret);
                return eMMC_ST_ERR_SAVE_DDRT_FAIL;
            }


            return eMMC_ST_SUCCESS;
        }
    }	
    #else
    U32 u32_err, u32_ret;
    U8  u8_SetIdx, u8_ClkIdx;

    eMMC_debug(eMMC_DEBUG_LEVEL,1,"eMMC Info: building DDR table, please wait... \n");
    memset((void*)&g_eMMCDrv.TimingTable_t, 0, sizeof(g_eMMCDrv.TimingTable_t));
    u8_SetIdx = 0;

    if(!eMMC_IF_NORMAL_SDR())
    {
        u32_err = eMMC_FCIE_EnableSDRMode();
        if(eMMC_ST_SUCCESS != u32_err)
        {
            eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,0,"eMMC Err: set SDR IF fail: %X\n", u32_err);
            return u32_err;
        }
    }

    u32_err = eMMC_FCIE_EnableFastMode_Ex(FCIE_eMMC_DDR);
    if(eMMC_ST_SUCCESS != u32_err)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,0,"eMMC Err: set DDR IF fail: %X\n", u32_err);
        return u32_err;
    }

    for(u8_ClkIdx=0; u8_ClkIdx<eMMC_FCIE_VALID_CLK_CNT; u8_ClkIdx++)
    {
        eMMC_clock_setting(gau8_eMMCPLLSel_52[u8_ClkIdx]);
        eMMC_debug(eMMC_DEBUG_LEVEL_HIGH,0,"\nclk: %uMHz\n", g_eMMCDrv.u32_ClkKHz/1000);

        // ---------------------------
        // search and set the Windows
        u32_err = eMMC_FCIE_DetectDDRTiming();

        // ---------------------------
        // set the Table
        if(eMMC_ST_SUCCESS == u32_err)
        {
            g_eMMCDrv.TimingTable_t.u8_SetCnt++;
            g_eMMCDrv.TimingTable_t.Set[u8_ClkIdx].u8_Clk = gau8_eMMCPLLSel_52[u8_ClkIdx];
            g_eMMCDrv.TimingTable_t.Set[u8_ClkIdx].u8_Reg2Ch = sgTSetTmp_t.u8_Reg2Ch;
            g_eMMCDrv.TimingTable_t.Set[u8_ClkIdx].u8_Skew4 = sgTSetTmp_t.u8_Skew4;
            g_eMMCDrv.TimingTable_t.Set[u8_ClkIdx].u32_ScanResult = sgTSetTmp_t.u32_ScanResult;
            g_eMMCDrv.TimingTable_t.u32_VerNo = eMMC_TIMING_TABLE_VERSION;
            eMMC_DumpTimingTable();
            eMMC_debug(eMMC_DEBUG_LEVEL, 0, "\nOK\n");

            g_eMMCDrv.TimingTable_t.u32_ChkSum =
                eMMC_ChkSum((U8*)&g_eMMCDrv.TimingTable_t,
                sizeof(g_eMMCDrv.TimingTable_t)-eMMC_TIMING_TABLE_CHKSUM_OFFSET);
            memcpy(gau8_eMMC_SectorBuf, (U8*)&g_eMMCDrv.TimingTable_t, sizeof(g_eMMCDrv.TimingTable_t));

            eMMC_FCIE_ErrHandler_ReInit();

            u32_err = eMMC_CMD24(eMMC_DDRTABLE_BLK_0, gau8_eMMC_SectorBuf);
            u32_ret = eMMC_CMD24(eMMC_DDRTABLE_BLK_1, gau8_eMMC_SectorBuf);
            if(eMMC_ST_SUCCESS!=u32_err && eMMC_ST_SUCCESS!=u32_ret)
            {
                eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: Save TTable fail: %Xh %Xh\n",
                    u32_err, u32_ret);
                return eMMC_ST_ERR_SAVE_DDRT_FAIL;
            }

            return eMMC_ST_SUCCESS;
        }
    }
    #endif

    eMMC_DumpTimingTable();
    eMMC_die();
    return eMMC_ST_ERR_SKEW4;
}

// trigger level
#define Trig_lvl_MASK     ((7<<6)|(7<<3)|7)

#if (defined(ENABLE_eMMC_HS200) && ENABLE_eMMC_HS200)
U32 eMMC_FCIE_ResetToHS200(U8 u8_ClkParam)
{
    U32 u32_err;

    // --------------------------------
    // reset FCIE & eMMC to normal SDR mode
    if(!eMMC_IF_NORMAL_SDR())
    {
        //eMMC_debug(0,1,"eMMC: re-init to SDR\n");
        g_eMMCDrv.u32_DrvFlag = 0;
        eMMC_PlatformInit();
        u32_err = eMMC_FCIE_Init();
        if(u32_err)
        {
            eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: eMMC_FCIE_Init fail, %Xh\n", u32_err);
            return u32_err;
        }
        u32_err = eMMC_Init_Device_Ex();
        if(eMMC_ST_SUCCESS != u32_err)
        {
            eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,0,"eMMC Err: eMMC_Init_Device_Ex fail: %X\n", u32_err);
            return u32_err;
        }
    }

    // --------------------------------
    // set eMMC to HS200 mode
    u32_err = eMMC_FCIE_EnableFastMode_Ex(FCIE_eMMC_HS200);
    if(eMMC_ST_SUCCESS != u32_err)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,0,"eMMC Err: set HS200 IF fail: %X\n", u32_err);

        return u32_err;
    }

    eMMC_clock_setting(u8_ClkParam);

    return u32_err;
}

U32 eMMC_FCIE_DetectHS200Timing(void)
{
    U32 u32_i, u32_err, u32_Skew4Result=0;
    U16 u16_SkewCnt=0;

    for(u32_i=0; u32_i<PLL_SKEW4_CNT; u32_i++)
    {
        // make sure a complete outside clock cycle
        REG_FCIE_CLRBIT(FCIE_SD_MODE, BIT_SD_CLK_EN);

        g_eMMCDrv.u32_DrvFlag |= DRV_FLAG_TUNING_TTABLE; // turn on tunning flag
        #ifndef IP_FCIE_VERSION_5
        REG_FCIE_CLRBIT(FCIE_SM_STS, BIT11);
        #else
        #ifdef REG_DIG_SKEW4_INV
        REG_FCIE_CLRBIT(REG_DIG_SKEW4_INV, BIT_DIG_SKEW4_INV);       //skew4 inverse for muji
        #endif

        #ifdef REG_ANL_SKEW4_INV
        REG_FCIE_CLRBIT(REG_ANL_SKEW4_INV, BIT_ANL_SKEW4_INV);
        #endif

        #endif
        REG_FCIE_CLRBIT(reg_emmcpll_0x03, BIT_SKEW4_MASK);
        REG_FCIE_SETBIT(reg_emmcpll_0x03, u32_i<<12);

        u32_err = eMMC_CMD21();
        if(eMMC_ST_SUCCESS==u32_err && eMMC_ST_SUCCESS==eMMCTest_BlkWRC_ProbeTiming(eMMC_TEST_BLK_0))
        {
            u16_SkewCnt++;
            u32_Skew4Result |= (1<<u32_i);
        }
        else
        {   //u32_err = eMMC_FCIE_Init();
            eMMC_debug(0,1,"failed skew4: %u\n", u32_i);
            u32_err = eMMC_FCIE_ResetToHS200((U8)g_eMMCDrv.u16_ClkRegVal);
            if(eMMC_ST_SUCCESS != u32_err)
                break;
        }
    }

    for(u32_i=PLL_SKEW4_CNT; u32_i<PLL_SKEW4_CNT*2; u32_i++)
    {
        // make sure a complete outside clock cycle
        REG_FCIE_CLRBIT(FCIE_SD_MODE, BIT_SD_CLK_EN);

        g_eMMCDrv.u32_DrvFlag |= DRV_FLAG_TUNING_TTABLE; // turn on tunning flag

        #ifndef IP_FCIE_VERSION_5
        REG_FCIE_SETBIT(FCIE_SM_STS, BIT11);
        #else
        #ifdef REG_DIG_SKEW4_INV
        REG_FCIE_SETBIT(REG_DIG_SKEW4_INV, BIT_DIG_SKEW4_INV);       //skew4 inverse for muji
        #endif

        #ifdef REG_ANL_SKEW4_INV
        REG_FCIE_SETBIT(REG_ANL_SKEW4_INV, BIT_ANL_SKEW4_INV);
        #endif

        #endif//IP_FCIE_VERSION_5
        REG_FCIE_CLRBIT(reg_emmcpll_0x03, BIT_SKEW4_MASK);
        REG_FCIE_SETBIT(reg_emmcpll_0x03, (u32_i-PLL_SKEW4_CNT)<<12);

        u32_err = eMMC_CMD21();
        if(eMMC_ST_SUCCESS==u32_err && eMMC_ST_SUCCESS==eMMCTest_BlkWRC_ProbeTiming(eMMC_TEST_BLK_0))
        {
            u16_SkewCnt++;
            u32_Skew4Result |= (1<<u32_i);
        }
        else
        {   //u32_err = eMMC_FCIE_Init();
            eMMC_debug(0,1,"failed skew4: %u\n", u32_i);
            u32_err = eMMC_FCIE_ResetToHS200((U8)g_eMMCDrv.u16_ClkRegVal);
            if(eMMC_ST_SUCCESS != u32_err)
                break;
        }
    }

    g_eMMCDrv.u32_DrvFlag &= ~DRV_FLAG_TUNING_TTABLE; // turn off tunning flag

    if(0==u32_Skew4Result || u16_SkewCnt<MIN_OK_SKEW_CNT)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: Clk: %uMHz SkewResult: %Xh SkewCnt: %u\n",
            g_eMMCDrv.u32_ClkKHz/1000, u32_Skew4Result, u16_SkewCnt);
        return eMMC_ST_ERR_SKEW4;
    }

    return eMMC_FCIE_DetectTiming_Ex(u32_Skew4Result);
}


U32 eMMC_FCIE_BuildHS200TimingTable(void)
{
    U32 u32_err, u32_ret;
    U8  u8_SetIdx, u8_ClkIdx;

    eMMC_debug(eMMC_DEBUG_LEVEL,0,"eMMC Info: building HS200 table, please wait... \n");
    memset((void*)&g_eMMCDrv.TimingTable_t, 0, sizeof(g_eMMCDrv.TimingTable_t));
    memset((void*)&g_eMMCDrv.TimingTable_G_t, 0, sizeof(g_eMMCDrv.TimingTable_G_t));
    //general table setting for HS200
    if(g_eMMCDrv.u32_DrvFlag &DRV_FLAG_SPEED_HS400_DETECT)
        g_eMMCDrv.TimingTable_G_t.u8_SpeedMode = FCIE_eMMC_HS400;
    else
        g_eMMCDrv.TimingTable_G_t.u8_SpeedMode = FCIE_eMMC_HS200;

    u8_SetIdx = 0;

    // --------------------------------
    // tuning FCIE & macro for HS200
    for(u8_ClkIdx=0; u8_ClkIdx<eMMC_FCIE_VALID_CLK_CNT; u8_ClkIdx++)
    {
        u32_err = eMMC_FCIE_ResetToHS200(gau8_eMMCPLLSel_200[u8_ClkIdx]);
        if(eMMC_ST_SUCCESS != u32_err)
        {
            eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: eMMC_FCIE_ResetToHS200 fail: %Xh\n", u32_err);
            return u32_err;
        }
        eMMC_debug(eMMC_DEBUG_LEVEL,0,"\nclk: %uMHz\n", g_eMMCDrv.u32_ClkKHz/1000);

        // ---------------------------
        // search and set the Windows
        u32_err = eMMC_FCIE_DetectHS200Timing();
        // ---------------------------
        // set the Table
        if(eMMC_ST_SUCCESS == u32_err)
        {
            //eMMC_FCIE_Init();
            g_eMMCDrv.TimingTable_t.u8_SetCnt++;
            g_eMMCDrv.TimingTable_t.Set[0].u8_Clk = gau8_eMMCPLLSel_200[u8_ClkIdx];
            g_eMMCDrv.TimingTable_t.Set[0].u8_Reg2Ch = sgTSetTmp_t.u8_Reg2Ch;
            g_eMMCDrv.TimingTable_t.Set[0].u8_Skew4 = sgTSetTmp_t.u8_Skew4;
            g_eMMCDrv.TimingTable_t.Set[0].u32_ScanResult = sgTSetTmp_t.u32_ScanResult;
            g_eMMCDrv.TimingTable_t.u32_VerNo = eMMC_TIMING_TABLE_VERSION;

            #if defined(ENABLE_eMMC_HS400) && ENABLE_eMMC_HS400
            //inherit clock from hs200
            if(g_eMMCDrv.TimingTable_G_t.u8_SpeedMode == FCIE_eMMC_HS400)
            {
                g_eMMCDrv.TimingTable_G_t.u32_Clk = gau8_eMMCPLLSel_200[u8_ClkIdx];
            }
            #endif
            eMMC_FCIE_ApplyTimingSet(0);

            eMMC_DumpTimingTable();
            eMMC_debug(eMMC_DEBUG_LEVEL, 0, "\nOK\n");

            g_eMMCDrv.TimingTable_t.u32_ChkSum =
                eMMC_ChkSum((U8*)&g_eMMCDrv.TimingTable_t,
                sizeof(g_eMMCDrv.TimingTable_t)-eMMC_TIMING_TABLE_CHKSUM_OFFSET);
            memcpy(gau8_eMMC_SectorBuf, (U8*)&g_eMMCDrv.TimingTable_t, sizeof(g_eMMCDrv.TimingTable_t));

//            eMMC_FCIE_ErrHandler_ReInit();

            if(!eMMC_CHK_BKG_SCAN_HS200())
            {
                u32_err = eMMC_CMD24(eMMC_HS200TABLE_BLK_0, gau8_eMMC_SectorBuf);
                u32_ret = eMMC_CMD24(eMMC_HS200TABLE_BLK_1, gau8_eMMC_SectorBuf);
                if(eMMC_ST_SUCCESS!=u32_err && eMMC_ST_SUCCESS!=u32_ret)
                {
                    eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: Save TTable fail: %Xh %Xh\n",
                               u32_err, u32_ret);
                    return eMMC_ST_ERR_SAVE_DDRT_FAIL;
                }
            }

            memcpy(g_eMMCDrv.TimingTable_G_t.au8_CID, g_eMMCDrv.au8_CID,eMMC_MAX_RSP_BYTE_CNT - 1);
            memcpy(gau8_eMMC_SectorBuf, (U8*)&g_eMMCDrv.TimingTable_G_t, sizeof(g_eMMCDrv.TimingTable_G_t));

            u32_err = eMMC_CMD24(eMMC_HS400EXTTABLE_BLK_0, gau8_eMMC_SectorBuf);
            u32_ret = eMMC_CMD24(eMMC_HS400EXTTABLE_BLK_1, gau8_eMMC_SectorBuf);

            if(eMMC_ST_SUCCESS!=u32_err && eMMC_ST_SUCCESS!=u32_ret)
            {
                eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: Save TTable fail: %Xh %Xh\n",
                           u32_err, u32_ret);
                return eMMC_ST_ERR_SAVE_DDRT_FAIL;
            }

            return eMMC_ST_SUCCESS;
        }
    }

    eMMC_DumpTimingTable();
    eMMC_die();
    return eMMC_ST_ERR_SKEW4;
}
#ifdef IP_FCIE_VERSION_5
U32 eMMC_FCIE_DetectHS200Timing_SKEW1_SKEW4(U8 u8_enable_error_log)
{
    U32 u32_i, u32_err, u32_Skew4Result;
    U8  u8_Skew1;

    #if 1
    eMMC_DISABLE_LOG(1);
    g_eMMCDrv.u16_Reg10_Mode = BIT_SD_DEFAULT_MODE_REG;
    eMMC_PlatformInit();
    u32_err = eMMC_FCIE_Init();
    if(eMMC_ST_SUCCESS != u32_err)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"FCIE Reset Fail!!\n");
        eMMC_FCIE_ErrHandler_Stop();
    }
    u32_err = eMMC_Init_Device_Ex();
    if(eMMC_ST_SUCCESS != u32_err)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Identify Error!!\n");
        eMMC_FCIE_ErrHandler_Stop();
    }
    u32_err = eMMC_FCIE_EnableFastMode(FCIE_eMMC_HS200);
    if(eMMC_ST_SUCCESS != u32_err)
    {
        u32_err = eMMC_FCIE_BuildHS200TimingTable();//eMMC 5.1 do not have hs200 table
        if(eMMC_ST_SUCCESS != u32_err)
           eMMC_FCIE_ErrHandler_Stop();
    }
    eMMC_DISABLE_LOG(0);
    #endif
    u32_i = 0;
    eMMC_debug(0, 0, "Skew4        %02u ", u32_i);
    for(u32_i=1; u32_i<PLL_SKEW4_CNT*2; u32_i++)
        eMMC_debug(0, 0, "%02u ", u32_i);
    eMMC_debug(0, 0, "\n");

    for(u8_Skew1=0;u8_Skew1<18;u8_Skew1++)
    {
        u32_Skew4Result=0;
        g_eMMCDrv.TimingTable_t.Set[0].u8_Skew2 = 0;//HS200 SKEW2 is default

        for(u32_i=0; u32_i<PLL_SKEW4_CNT*2; u32_i++)
        {
            g_eMMCDrv.u32_DrvFlag |= DRV_FLAG_TUNING_TTABLE; // turn on tunning flag
            if(u8_enable_error_log)// disable tunning error log
                eMMC_DISABLE_LOG(1);

            if(u32_i < PLL_SKEW4_CNT)
            {
                g_eMMCDrv.TimingTable_t.Set[0].u8_Reg2Ch=0;
                g_eMMCDrv.TimingTable_t.Set[0].u8_Skew4=u32_i;
            }
            else
            {
                g_eMMCDrv.TimingTable_t.Set[0].u8_Reg2Ch=1;
                g_eMMCDrv.TimingTable_t.Set[0].u8_Skew4=(u32_i-PLL_SKEW4_CNT);
            }
            eMMC_FCIE_ApplyTimingSet(0);
            if(u8_Skew1<9)
            {
                REG_FCIE_CLRBIT(reg_emmcpll_0x69, BIT3);// reg_clk_dig_inv
                REG_FCIE_CLRBIT(reg_emmcpll_0x03, BIT_SKEW1_MASK);
                REG_FCIE_SETBIT(reg_emmcpll_0x03, u8_Skew1);
            }
            else
            {
                REG_FCIE_SETBIT(reg_emmcpll_0x69, BIT3);// reg_clk_dig_inv
                REG_FCIE_CLRBIT(reg_emmcpll_0x03, BIT_SKEW1_MASK);
                REG_FCIE_SETBIT(reg_emmcpll_0x03, u8_Skew1-9);
            }

            u32_err = eMMC_CMD21();
            if(eMMC_ST_SUCCESS==u32_err && eMMC_ST_SUCCESS==eMMCTest_BlkWRC_ProbeTiming(eMMC_TEST_BLK_0))
            {
                u32_Skew4Result |= (1<<u32_i);
            }
            else
            {
                u32_err = eMMC_FCIE_ResetToHS200((U8)g_eMMCDrv.u16_ClkRegVal);
                if(eMMC_ST_SUCCESS != u32_err)
                {
                    g_eMMCDrv.u32_DrvFlag &= ~DRV_FLAG_TUNING_TTABLE; // turn off tunning flag
                    eMMC_DISABLE_LOG(0);
                    eMMC_debug(0, 1, "eMMC Err: SKEW4: %d fail\n", u32_i);
                    break;
                }
            }
        }
        g_eMMCDrv.u32_DrvFlag &= ~DRV_FLAG_TUNING_TTABLE; // turn off tunning flag
        eMMC_DISABLE_LOG(0);
        eMMC_debug(0,0,"Skew1: %02u |  ", u8_Skew1);
        for(u32_i=0; u32_i<PLL_SKEW4_CNT*2; u32_i++)
        {
            if((u32_Skew4Result & (1<<u32_i)) == 0)
                printf(" X ");
            else
                printf(" O ");
        }
        printf("\n");
    }
    return eMMC_ST_SUCCESS;
}

U32 gu32Skew4Result = 0x3FFFF;
U32 gu32Skew4ResultLv0 = 0x3FFFF;

U32 eMMC_FCIE_DetectHS200Timing_TrigerLevel_SKEW4(U8 u8_enable_error_log)
{
    U32 u32_i = 0, u32_err = 0, u32_Skew4Result;
    U8  TrigerLevel;
    #if 1
    eMMC_DISABLE_LOG(1);
    g_eMMCDrv.u16_Reg10_Mode = BIT_SD_DEFAULT_MODE_REG;
    eMMC_PlatformInit();
    u32_err = eMMC_FCIE_Init();
    if(eMMC_ST_SUCCESS != u32_err)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"FCIE Reset Fail!!\n");
        eMMC_FCIE_ErrHandler_Stop();
    }
    u32_err = eMMC_Init_Device_Ex();
    if(eMMC_ST_SUCCESS != u32_err)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Identify Error!!\n");
        eMMC_FCIE_ErrHandler_Stop();
    }
    u32_err = eMMC_FCIE_EnableFastMode(FCIE_eMMC_HS200);
    if(eMMC_ST_SUCCESS != u32_err)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC switch to HS200 fail!!\n");
        eMMC_FCIE_ErrHandler_Stop();
    }
    eMMC_DISABLE_LOG(0);
    #endif

    u32_i = 0;
    eMMC_debug(0, 0, "Skew4              %02u ", u32_i);
    for(u32_i=1; u32_i<PLL_SKEW4_CNT*2; u32_i++)
        eMMC_debug(0, 0, "%02u ", u32_i);
    eMMC_debug(0, 0, "\n");

    // ==========================================
    for(TrigerLevel=0; TrigerLevel<8; TrigerLevel++)
    {
        if(u8_enable_error_log)// disable tunning error log
            eMMC_DISABLE_LOG(1);

        eMMC_SetTrigerLevel(TrigerLevel);
        // --------------------------------------
        u32_Skew4Result = 0;

        for(u32_i=0; u32_i<PLL_SKEW4_CNT*2; u32_i++)
        {
            g_eMMCDrv.u32_DrvFlag |= DRV_FLAG_TUNING_TTABLE; // turn on tunning flag

            if(u32_i < PLL_SKEW4_CNT)
            {
                g_eMMCDrv.TimingTable_t.Set[0].u8_Reg2Ch=0;
                g_eMMCDrv.TimingTable_t.Set[0].u8_Skew4=u32_i;
            }
            else
            {
                g_eMMCDrv.TimingTable_t.Set[0].u8_Reg2Ch=1;
                g_eMMCDrv.TimingTable_t.Set[0].u8_Skew4=(u32_i-PLL_SKEW4_CNT);
            }
            eMMC_FCIE_ApplyTimingSet(0);

            u32_err = eMMC_CMD21();
            if(eMMC_ST_SUCCESS==u32_err && eMMC_ST_SUCCESS==eMMCTest_BlkWRC_ProbeTiming(eMMC_TEST_BLK_0))
            {
                u32_Skew4Result |= (1<<u32_i);
            }
            else
            {
                u32_err = eMMC_FCIE_ResetToHS200((U8)g_eMMCDrv.u16_ClkRegVal);
                if(eMMC_ST_SUCCESS != u32_err)
                {
                    g_eMMCDrv.u32_DrvFlag &= ~DRV_FLAG_TUNING_TTABLE; // turn off tunning flag
                    eMMC_DISABLE_LOG(0);
                    eMMC_debug(0, 1, "eMMC Err: SKEW4: %d fail\n", u32_i);
                    break;
                }
            }

        }
        g_eMMCDrv.u32_DrvFlag &= ~DRV_FLAG_TUNING_TTABLE; // turn off tunning flag
        eMMC_DISABLE_LOG(0);
        eMMC_debug(0,0,"TrigerLevel: %02u |  ", TrigerLevel);
        for(u32_i=0; u32_i<PLL_SKEW4_CNT*2; u32_i++)
        {
            if((u32_Skew4Result & (1<<u32_i)) == 0)
                printf(" X ");
            else
                printf(" O ");
        }
        printf("\n");

        if( TrigerLevel == 0 )
            gu32Skew4ResultLv0 &= u32_Skew4Result;

        gu32Skew4Result &= u32_Skew4Result;
    }
    REG_FCIE_CLRBIT(reg_emmcpll_0x20, Trig_lvl_MASK);

    return eMMC_ST_SUCCESS;

}

U32 eMMC_FCIE_DetectSkew4(void)
{
    U32 u32_i, u32_err, u32_Skew4Result = 0;

    eMMC_DISABLE_LOG(1);

    g_eMMCDrv.u16_Reg10_Mode = BIT_SD_DEFAULT_MODE_REG;

    eMMC_PlatformInit();

    u32_err = eMMC_FCIE_Init();
    if(eMMC_ST_SUCCESS != u32_err)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"FCIE Reset Fail!!\n");
        eMMC_FCIE_ErrHandler_Stop();
    }

    u32_err = eMMC_Init_Device_Ex();
    if(eMMC_ST_SUCCESS != u32_err)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Identify Error!!\n");
        eMMC_FCIE_ErrHandler_Stop();
    }

    u32_err = eMMC_FCIE_EnableFastMode(FCIE_eMMC_HS200);
    if(eMMC_ST_SUCCESS != u32_err)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC switch to HS200 fail!!\n");
        eMMC_FCIE_ErrHandler_Stop();
    }

    eMMC_DISABLE_LOG(0);

    eMMC_debug(0, 0, "Skew4:\n");
    for(u32_i=0; u32_i<PLL_SKEW4_CNT*2; u32_i++)
        eMMC_debug(0, 0, "%02u ", u32_i);
    eMMC_debug(0, 0, "\n");

    for(u32_i=0; u32_i<(PLL_SKEW4_CNT*2); u32_i++)
    {
        // make sure a complete outside clock cycle
        REG_FCIE_CLRBIT(FCIE_SD_MODE, BIT_SD_CLK_EN);

        g_eMMCDrv.u32_DrvFlag |= DRV_FLAG_TUNING_TTABLE; // turn on tunning flag

        eMMC_DISABLE_LOG(1);

        if( u32_i < PLL_SKEW4_CNT )
        {
            #ifdef REG_DIG_SKEW4_INV
            REG_FCIE_CLRBIT(REG_DIG_SKEW4_INV, BIT_DIG_SKEW4_INV);
            #endif

            #ifdef REG_ANL_SKEW4_INV
            REG_FCIE_CLRBIT(REG_ANL_SKEW4_INV, BIT_ANL_SKEW4_INV);
            #endif

            REG_FCIE_CLRBIT(reg_emmcpll_0x03, BIT_SKEW4_MASK);
            REG_FCIE_SETBIT(reg_emmcpll_0x03, u32_i<<12);
        }
        else
        {
            #ifdef REG_DIG_SKEW4_INV
            REG_FCIE_SETBIT(REG_DIG_SKEW4_INV, BIT_DIG_SKEW4_INV);
            #endif

            #ifdef REG_ANL_SKEW4_INV
            REG_FCIE_SETBIT(REG_ANL_SKEW4_INV, BIT_ANL_SKEW4_INV);
            #endif

            REG_FCIE_CLRBIT(reg_emmcpll_0x03, BIT_SKEW4_MASK);
            REG_FCIE_SETBIT(reg_emmcpll_0x03, (u32_i-PLL_SKEW4_CNT)<<12);
        }

        u32_err = eMMC_CMD21();
        if((eMMC_ST_SUCCESS==u32_err) && eMMC_ST_SUCCESS==eMMCTest_BlkWRC_ProbeTiming(eMMC_TEST_BLK_0))
        {
            u32_Skew4Result |= (1<<u32_i);
        }
        else
        {
            u32_err = eMMC_FCIE_ResetToHS200((U8)g_eMMCDrv.u16_ClkRegVal);
            if(eMMC_ST_SUCCESS != u32_err)
            {
                g_eMMCDrv.u32_DrvFlag &= ~DRV_FLAG_TUNING_TTABLE; // turn off tunning flag
                eMMC_DISABLE_LOG(0);
                eMMC_debug(0, 1, "eMMC Err: SKEW4: %d fail\n", u32_i);
                break;
            }
        }
    }

    g_eMMCDrv.u32_DrvFlag &= ~DRV_FLAG_TUNING_TTABLE; // turn off tunning flag

    eMMC_DISABLE_LOG(0);

    for(u32_i=0; u32_i<PLL_SKEW4_CNT*2; u32_i++)
    {
        if((u32_Skew4Result & (1<<u32_i)) == 0)
            printf(" X ");
        else
            printf(" O ");
    }

    printf("\n");

    return eMMC_ST_SUCCESS;
}

#endif
#endif
#if defined(ENABLE_eMMC_HS400) && ENABLE_eMMC_HS400
U32 eMMC_FCIE_ResetToHS400(U8 u8_ClkParam)
{
    U32 u32_err;

    //eMMC_debug(0, 0, "\033[7;31m%s\033[m\n", __FUNCTION__);

    // --------------------------------
    // reset FCIE & eMMC to normal SDR mode
    if(!eMMC_IF_NORMAL_SDR())
    {
        //eMMC_debug(0,0,"eMMC: re-init to SDR\n");
        g_eMMCDrv.u32_DrvFlag = 0;
        eMMC_PlatformInit();
        u32_err = eMMC_FCIE_Init();
        if(u32_err)
        {
            eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: eMMC_FCIE_Init fail, %Xh\n", u32_err);
            return u32_err;
        }
        u32_err = eMMC_Init_Device_Ex();
        if(eMMC_ST_SUCCESS != u32_err)
        {
            eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,0,"eMMC Err: eMMC_Init_Device_Ex fail: %X\n", u32_err);
            return u32_err;
        }
    }

    // --------------------------------
    // set eMMC to HS200 mode
    #if 0
    u32_err = eMMC_FCIE_EnableFastMode_Ex(FCIE_eMMC_HS200);
    if(eMMC_ST_SUCCESS != u32_err)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,0,"eMMC Err: set HS200 IF fail: %X\n", u32_err);

        return u32_err;
    }
    #endif

    u32_err = eMMC_FCIE_EnableFastMode_Ex(FCIE_eMMC_HS400);
    if(eMMC_ST_SUCCESS != u32_err)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,0,"eMMC Err: set HS400 IF fail: %X\n", u32_err);

        return u32_err;
    }

    eMMC_clock_setting(u8_ClkParam);

    return u32_err;
}

// 0~15
void eMMC_FCIE_SetDelayLine(U32 u32Value)
{
    //printf("\n\nSetDelayLatch(%d)\n", u32Value);

    if(u32Value>16)
    {
        printf("eMMC Err: wrong delay latch value\n");
        return;
    }

    if(u32Value%2)
        REG_FCIE_SETBIT(reg_emmcpll_0x09, BIT1);
    else
        REG_FCIE_CLRBIT(reg_emmcpll_0x09, BIT1);

    u32Value = (u32Value&0xFFFE)>>1;

    REG_FCIE_CLRBIT(reg_emmcpll_0x09, (0x000F<<4) );
    REG_FCIE_SETBIT(reg_emmcpll_0x09, (u32Value<<4) );


    //printf("emmc_pll[09] = %04X\n", REG_FCIE(reg_emmcpll_0x09));

}

U32 eMMC_FCIE_DetectHS400Timing_Ex(U32 u32_RXDLLResult)
{
    //sgTSetTmp_t.u8_Cell;
    U16 u16_i=0, u16_StartBit=0xFFFF, u16_EndBit=0xFFFF, u16_cnt=0;

    eMMC_debug(0,0,"u32_RXDLLResult:0x%x\n",u32_RXDLLResult);

    // ==================================================
    // case.1: all bits set
    if((u32_RXDLLResult & ((1<<8*2)-1)) == ((1<<8*2)-1) )
    {
        eMMC_debug(eMMC_DEBUG_LEVEL,0," all\n");
        sgTSetTmp_t.u8_Cell=7;
        u16_cnt=16;
    }
    else
    {
        do{
            if((u32_RXDLLResult&1<<u16_i) && 0xFFFF==u16_StartBit)
                u16_StartBit = u16_i;
            else if(0xFFFF!=u16_StartBit && 0==(u32_RXDLLResult&1<<u16_i))
                u16_EndBit = u16_i-1;

            if(0xFFFF != u16_EndBit)
            {
                if((u16_EndBit-u16_StartBit+1) > u16_cnt)
                {
                    sgTSetTmp_t.u8_Cell=u16_StartBit+ ((u16_EndBit - u16_StartBit)/2);
                    u16_cnt= u16_EndBit - u16_StartBit+1;
                }
                if(u16_cnt >=8)
                    break;

                u16_StartBit=0xFFFF;
                u16_EndBit  =0xFFFF;
            }
            u16_i++;
        }while(u16_i < 8*2);
    }
    sgTSetTmp_t.u8_CellCnt = u16_cnt;
    eMMC_debug(0,0,"BestDelayLine:0x%x\n",sgTSetTmp_t.u8_Cell);

    return u16_cnt;
}

U32 eMMC_FCIE_DetectHS400Timing_Skew2_Ex(U32 u32_SKew2Result)
{
    //sgTSetTmp_t.u8_Cell;
    U16 u16_i=0, u16_StartBit=0xFFFF, u16_EndBit=0xFFFF, u16_cnt=0;

    eMMC_debug(0,0,"u32_SKew2Result:0x%x\n",u32_SKew2Result);
    if((u32_SKew2Result & ((1<<10)-1)) == ((1<<10)-1) )
    {
        eMMC_debug(eMMC_DEBUG_LEVEL,0," all\n");
        sgTSetTmp_t.u8_Skew2=4;
        u16_cnt = 10;
    }
    else
    {
        do{
            if((u32_SKew2Result&1<<u16_i) && 0xFFFF==u16_StartBit)
                u16_StartBit = u16_i;
            else if(0xFFFF!=u16_StartBit && 0==(u32_SKew2Result&1<<u16_i))
                u16_EndBit = u16_i-1;
            u16_i++;
        }while(u16_i < 10);
//        eMMC_debug(0, 1, "u16_StartBit 0x%X, u16_EndBit 0x%X\n", u16_StartBit, u16_EndBit);
        if(0xFFFF != u16_EndBit)
        {
            sgTSetTmp_t.u8_Skew2=u16_StartBit+ ((u16_EndBit - u16_StartBit)/2);
            u16_cnt= u16_EndBit - u16_StartBit+1;
        }
    }
    eMMC_debug(0,0,"BestSkew2:0x%x\n",sgTSetTmp_t.u8_Skew2);
    return u16_cnt;
}

U32 eMMC_FCIE_DetectHS400Timing(void)
{
    U32 u32_i = 0, u32_j, u32_k, u32_err = 0, u32_Skew2Result = 0, u32_Skew4Result =0;
    U16 u16_Skew2Cnt = 0;
    U32 u32_ret =0;
    S8  as8_Skew4Shift[5]={-2,-1,0,1,2}, s8_Skew4Idx=0;
    S32 s32_Skew4Ori = 0, s32_Skew4Tmp = 0;
    U8 u8_SkewFailCount[18]={0};
    U8 u8_MaxCount = 0;
    u8 u8_MaxSkew = 0;
    U8 u8_TrgLvl, u8_DrvStr;
    U8 u8_TrgLvlValue[3]={0, 3, 7};
    U32 au32_RXDLLResult[5]={0};
    U8 au8_Reg2Ch[5], au8_Skew4[5];
    U8 au8_Cell[5], au8_CellCnt[5];

    memset((void*)g_eMMCDrv.TimingTable_G_t.RegSet, 0, sizeof(g_eMMCDrv.TimingTable_G_t.RegSet));

    g_eMMCDrv.TimingTable_G_t.u8_SetCnt = 5;            //initial set count for apply general register

    g_eMMCDrv.TimingTable_G_t.u8_RegisterCnt = 4;   //skew4 & its inverse register, RXDLL & its inverse & skew2
    #ifdef REG_DIG_SKEW4_INV
    g_eMMCDrv.TimingTable_G_t.u8_RegisterCnt ++; //additional skew4 for muji
    #endif
    g_eMMCDrv.TimingTable_G_t.u8_CurSetIdx = 0;

    // ==========================================
    //add skew4 for hs400 using hs200 + pad setting of HS400 to detect skew4
    for(u8_DrvStr=0; u8_DrvStr<2; u8_DrvStr++)
    {
        g_eMMCDrv.u32_LastErrCode = 0;
        if(u8_DrvStr)
            g_eMMCDrv.u32_LastErrCode |= DRV_FLAG_WEAK_STRENGTH;

        for(u8_TrgLvl = 0; u8_TrgLvl < 3; u8_TrgLvl++)
        {
        //printf("TrgLvl 0x%X, %s Driving\n", u8_TrgLvlValue[u8_TrgLvl],
            //(g_eMMCDrv.u32_LastErrCode & DRV_FLAG_WEAK_STRENGTH)? "Weakest":"Normal");
            memset(u8_SkewFailCount, 0, sizeof(u8_SkewFailCount));
            u8_MaxCount = 0;
            u8_MaxSkew = 0;
            eMMC_SetTrigerLevel(0);
            u32_err = eMMC_FCIE_ResetToHS400((U8)gau8_eMMCPLLSel_200[0]);
            if(eMMC_ST_SUCCESS != u32_err)
            {
                eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: eMMC_FCIE_ResetToHS400 fail: %Xh\n", u32_err);
                return u32_err;
            }

            #if 0
            u32_i = 0;
            eMMC_debug(0, 0, "             %02u ", u32_i);
            for(u32_i=1; u32_i<18; u32_i++)
                eMMC_debug(0, 0, "%02u ", u32_i);
            eMMC_debug(0, 0, "\n");
            #endif


            //scan skew4 for hs400 rsp using skew1 & skew4
            for(u32_j = 0; u32_j < 9*2; u32_j ++)
            {
                u32_Skew4Result = 0;

                for(u32_i = 0; u32_i < PLL_SKEW4_CNT*2; u32_i ++)
                {
                    for(u32_k = 0; u32_k < 5; u32_k ++)
                    {
                        g_eMMCDrv.u32_DrvFlag |= DRV_FLAG_TUNING_TTABLE; // turn on tunning flag
                        REG_FCIE_CLRBIT(reg_emmcpll_0x03,BIT_SKEW1_MASK);
                        if(u32_j < 9)
                        {
                            REG_FCIE_CLRBIT(reg_emmcpll_0x69, BIT3);// reg_clk_dig_inv
                            REG_FCIE_SETBIT(reg_emmcpll_0x03, u32_j);
                        }
                        else
                        {
                            REG_FCIE_SETBIT(reg_emmcpll_0x69, BIT3);// reg_clk_dig_inv
                            REG_FCIE_SETBIT(reg_emmcpll_0x03, u32_j-9);
                        }

                        if(u32_i >= PLL_SKEW4_CNT)
                        {
                            #ifdef REG_DIG_SKEW4_INV
                            REG_FCIE_SETBIT(REG_DIG_SKEW4_INV, BIT_DIG_SKEW4_INV);//skew4 inverse for muji
                            #endif

                            #ifdef REG_ANL_SKEW4_INV
                            REG_FCIE_SETBIT(REG_ANL_SKEW4_INV, BIT_ANL_SKEW4_INV);
                            #endif

                            REG_FCIE_CLRBIT(reg_emmcpll_0x03, BIT_SKEW4_MASK);
                            REG_FCIE_SETBIT(reg_emmcpll_0x03, (u32_i-PLL_SKEW4_CNT)<<12);
                        }
                        else
                        {
                            #ifdef REG_DIG_SKEW4_INV
                            REG_FCIE_CLRBIT(REG_DIG_SKEW4_INV, BIT_DIG_SKEW4_INV);//skew4 inverse for muji
                            #endif

                            #ifdef REG_ANL_SKEW4_INV
                            REG_FCIE_CLRBIT(REG_ANL_SKEW4_INV, BIT_ANL_SKEW4_INV);
                            #endif

                            REG_FCIE_CLRBIT(reg_emmcpll_0x03, BIT_SKEW4_MASK);
                            REG_FCIE_SETBIT(reg_emmcpll_0x03, (u32_i)<<12);
                        }
                        eMMC_SetTrigerLevel(u8_TrgLvlValue[u8_TrgLvl]);
                        u32_err = eMMC_CMD13(g_eMMCDrv.u16_RCA);
                        if(u32_err == eMMC_ST_SUCCESS)
                        {
                            //u32_Skew4Result |= (1<<u32_i);
                            continue;
                        }
                        else
                        {
                            eMMC_SetTrigerLevel(0);
                            u32_err = eMMC_FCIE_ResetToHS400((U8)g_eMMCDrv.u16_ClkRegVal);
                            if(eMMC_ST_SUCCESS != u32_err)
                            {
                                eMMC_debug(0,0,"scan Skew1 & skew4 fail\n");
                                g_eMMCDrv.u32_DrvFlag &= ~DRV_FLAG_TUNING_TTABLE; // turn off tunning flag
                                break;
                            }
                            break;
                        }
                    }
                    if(u32_k == 5)
                        u32_Skew4Result |= (1<<u32_i);
                }
                #if 0
                printf("Skew1: %02u |  ", u32_j);
                for(u32_i=0; u32_i<18; u32_i++)
                {
                    if((u32_Skew4Result & (1<<u32_i)) == 0)
                        printf(" X ");
                    else
                        printf(" O ");
                }
                printf("\n");
                #endif

                if(((u32_Skew4Result & ((1<<PLL_SKEW4_CNT*2)-1))!=((1<<PLL_SKEW4_CNT*2)-1)) && u32_Skew4Result != 0)
                {
                    //search the fail level
                    for(u32_i = 0; u32_i < PLL_SKEW4_CNT*2; u32_i ++)
                    {
                        if((u32_Skew4Result & (1 << u32_i)) == 0)
                        {
                            s8_Skew4Idx = (S32)u32_i - (S32)u32_j;
                            if(s8_Skew4Idx < 0)
                                s8_Skew4Idx += PLL_SKEW4_CNT*2;
                            u8_SkewFailCount[s8_Skew4Idx] ++;
                        }
                    }
                }
            }
            for(u32_i = 0; u32_i < PLL_SKEW4_CNT*2; u32_i ++)
            {
                //printf("skew4 %d: %d\n", u32_i, u8_SkewFailCount[u32_i]);
                if(u8_MaxCount < u8_SkewFailCount[u32_i])
                {
                    u8_MaxSkew = u32_i;
                    u8_MaxCount = u8_SkewFailCount[u32_i];
                }
            }

            s8_Skew4Idx = u8_MaxSkew - PLL_SKEW4_CNT;
            if(s8_Skew4Idx < 0)
                s8_Skew4Idx += PLL_SKEW4_CNT*2;

            g_eMMCDrv.u32_DrvFlag &= ~DRV_FLAG_TUNING_TTABLE; // turn off tunning flag

            s32_Skew4Ori = s8_Skew4Idx;
            s8_Skew4Idx = 0;
            if(u8_MaxCount >= 3)
                break;
        }
        if(u8_MaxCount >= 3)
        {
            g_eMMCDrv.u32_LastErrCode &= ~ DRV_FLAG_WEAK_STRENGTH;
            eMMC_SetTrigerLevel(0);
            u32_err = eMMC_FCIE_ResetToHS400((U8)gau8_eMMCPLLSel_200[0]);
            if(eMMC_ST_SUCCESS != u32_err)
            {
                eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: eMMC_FCIE_ResetToHS400 fail: %Xh\n", u32_err);
                return u32_err;
            }
            break;
        }
    }
	if(u8_MaxCount < 3)
	{
		eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1, "eMMC Err: HS400 scan skew4 fail\n");
		return eMMC_ST_ERR_SKEW4;
	}
	eMMC_debug(0, 0, "HS400 Skew4 %lXh\n", s32_Skew4Ori);
    g_eMMCDrv.u32_LastErrCode &= ~ DRV_FLAG_WEAK_STRENGTH;
    eMMC_SetTrigerLevel(0);
    u32_err = eMMC_FCIE_ResetToHS400((U8)g_eMMCDrv.u16_ClkRegVal);
    if(eMMC_ST_SUCCESS != u32_err)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: eMMC_FCIE_ResetToHS400 fail: %Xh\n", u32_err);
        return u32_err;
    }

    LABEL_SCAN_RXDLL:

    s32_Skew4Tmp = s32_Skew4Ori + as8_Skew4Shift[s8_Skew4Idx];

    if(s32_Skew4Tmp < 0)
        au8_Skew4[s8_Skew4Idx] = 2*PLL_SKEW4_CNT + s32_Skew4Tmp;
    else if(s32_Skew4Tmp >= 2*PLL_SKEW4_CNT)
        au8_Skew4[s8_Skew4Idx] = s32_Skew4Tmp - 2*PLL_SKEW4_CNT;
    else
        au8_Skew4[s8_Skew4Idx] = s32_Skew4Tmp;

    if(au8_Skew4[s8_Skew4Idx] < PLL_SKEW4_CNT)
    {
        au8_Reg2Ch[s8_Skew4Idx] = 0;
    }
    else
    {
        au8_Reg2Ch[s8_Skew4Idx] = 1;
        au8_Skew4[s8_Skew4Idx] -= PLL_SKEW4_CNT;
    }

    // setup GEN_TIMING_TABLE
    //skew4 invesre
    #ifdef REG_ANL_SKEW4_INV
    g_eMMCDrv.TimingTable_G_t.RegSet[s8_Skew4Idx *g_eMMCDrv.TimingTable_G_t.u8_RegisterCnt +GT_OFFSET_SKEW4_INV_BIT].u32_RegAddress = (REG_ANL_SKEW4_INV - RIU_BASE);
    g_eMMCDrv.TimingTable_G_t.RegSet[s8_Skew4Idx *g_eMMCDrv.TimingTable_G_t.u8_RegisterCnt +GT_OFFSET_SKEW4_INV_BIT].u16_RegValue =BIT_ANL_SKEW4_INV;
    g_eMMCDrv.TimingTable_G_t.RegSet[s8_Skew4Idx *g_eMMCDrv.TimingTable_G_t.u8_RegisterCnt +GT_OFFSET_SKEW4_INV_BIT].u16_RegMask =BIT_ANL_SKEW4_INV;
    #endif
    //skew4 value
    g_eMMCDrv.TimingTable_G_t.RegSet[s8_Skew4Idx *g_eMMCDrv.TimingTable_G_t.u8_RegisterCnt +GT_OFFSET_SKEW4_VALUE].u32_RegAddress = (reg_emmcpll_0x03 - RIU_BASE);
    g_eMMCDrv.TimingTable_G_t.RegSet[s8_Skew4Idx *g_eMMCDrv.TimingTable_G_t.u8_RegisterCnt +GT_OFFSET_SKEW4_VALUE].u16_RegValue =au8_Skew4[s8_Skew4Idx] << 12;
    g_eMMCDrv.TimingTable_G_t.RegSet[s8_Skew4Idx *g_eMMCDrv.TimingTable_G_t.u8_RegisterCnt +GT_OFFSET_SKEW4_VALUE].u16_RegMask =BIT_SKEW4_MASK;
    g_eMMCDrv.TimingTable_G_t.RegSet[s8_Skew4Idx *g_eMMCDrv.TimingTable_G_t.u8_RegisterCnt +GT_OFFSET_SKEW4_VALUE].u16_OpCode =REG_OP_SETBIT;
    #ifdef REG_DIG_SKEW4_INV
    g_eMMCDrv.TimingTable_G_t.RegSet[s8_Skew4Idx *g_eMMCDrv.TimingTable_G_t.u8_RegisterCnt +GT_OFFSET_SKEW4_INV_BIT_DIG].u32_RegAddress = (REG_DIG_SKEW4_INV - RIU_BASE);
    g_eMMCDrv.TimingTable_G_t.RegSet[s8_Skew4Idx *g_eMMCDrv.TimingTable_G_t.u8_RegisterCnt +GT_OFFSET_SKEW4_INV_BIT_DIG].u16_RegValue =BIT_DIG_SKEW4_INV;
    g_eMMCDrv.TimingTable_G_t.RegSet[s8_Skew4Idx *g_eMMCDrv.TimingTable_G_t.u8_RegisterCnt +GT_OFFSET_SKEW4_INV_BIT_DIG].u16_RegMask =BIT_DIG_SKEW4_INV;
    #endif
    if(au8_Reg2Ch[s8_Skew4Idx] == 1)
    {
        g_eMMCDrv.TimingTable_G_t.RegSet[s8_Skew4Idx *g_eMMCDrv.TimingTable_G_t.u8_RegisterCnt +GT_OFFSET_SKEW4_INV_BIT].u16_OpCode =REG_OP_SETBIT;
        #ifdef REG_DIG_SKEW4_INV
        g_eMMCDrv.TimingTable_G_t.RegSet[s8_Skew4Idx *g_eMMCDrv.TimingTable_G_t.u8_RegisterCnt +GT_OFFSET_SKEW4_INV_BIT_DIG].u16_OpCode =REG_OP_SETBIT;
        #endif
    }
    else
    {
        g_eMMCDrv.TimingTable_G_t.RegSet[s8_Skew4Idx *g_eMMCDrv.TimingTable_G_t.u8_RegisterCnt +GT_OFFSET_SKEW4_INV_BIT].u16_OpCode =REG_OP_CLRBIT;
        #ifdef REG_DIG_SKEW4_INV
        g_eMMCDrv.TimingTable_G_t.RegSet[s8_Skew4Idx *g_eMMCDrv.TimingTable_G_t.u8_RegisterCnt +GT_OFFSET_SKEW4_INV_BIT_DIG].u16_OpCode =REG_OP_CLRBIT;
        #endif
    }

    // ================================================
    // scan RXDLL for the 5 Skew4: [-2, -1, Skew4, +1, +2]
    // ================================================
    eMMC_debug(0,0,"\n  Skew4[%d]: inv: %Xh, Skew4: %Xh \n", as8_Skew4Shift[s8_Skew4Idx],
        au8_Reg2Ch[s8_Skew4Idx],au8_Skew4[s8_Skew4Idx]);

    eMMC_FCIE_Apply_Reg(s8_Skew4Idx);
    // --------------------------------------
    //reduce scan time, only scan index 0

        if(s8_Skew4Idx == 0)
        {
            eMMC_debug(0,0,"\nscan RXDLL\n");//, REG_FCIE(reg_emmcpll_0x1d));
            for(u32_i=0; u32_i<16; u32_i++)
            {
                g_eMMCDrv.u32_DrvFlag |= DRV_FLAG_TUNING_TTABLE; // turn on tunning flag
                //using default skew2 to detect RXDLL
                REG_FCIE_CLRBIT(reg_emmcpll_0x03, BIT_SKEW2_MASK);
                REG_FCIE_SETBIT(reg_emmcpll_0x03, BIT_DEFAULT_SKEW2);

                eMMC_FCIE_SetDelayLine(u32_i);

                if(eMMC_ST_SUCCESS==eMMCTest_BlkWRC_ProbeTiming(eMMC_TEST_BLK_0))
                {
                    au32_RXDLLResult[s8_Skew4Idx] |= (1<<u32_i);
                    //eMMC_debug(0, 1, "RXDLL: %d ok\n", u32_i);
                }
                else
                {	//u32_err = eMMC_FCIE_Init();
                    eMMC_debug(0, 1, "RXDLL: %d fail\n", u32_i);
                    u32_err = eMMC_FCIE_ResetToHS400((U8)g_eMMCDrv.u16_ClkRegVal);
                    if(eMMC_ST_SUCCESS != u32_err)
                        break;
                }
            }

            g_eMMCDrv.u32_DrvFlag &= ~DRV_FLAG_TUNING_TTABLE; // turn off tunning flag


            au8_CellCnt[s8_Skew4Idx] =
                eMMC_FCIE_DetectHS400Timing_Ex(au32_RXDLLResult[s8_Skew4Idx]);

	    if(au8_CellCnt[s8_Skew4Idx] < 7) {
		eMMC_debug(0, 1, "RXDLL phase are not enough, only %d!!!\n", au8_CellCnt[s8_Skew4Idx]);
		eMMC_FCIE_ErrHandler_Stop();
            }

            #if 1
            for(u32_i=0; u32_i<16; u32_i++)
            {
                printf("%02X ", u32_i);
            }
            printf("\n");

            for(u32_i=0; u32_i<16; u32_i++)
            {
                if( (au32_RXDLLResult[s8_Skew4Idx] & (1<<u32_i)) == 0 )
                    printf(" X ");
                else
                    printf(" O ");
            }
            printf("\n");
            #endif

            au8_Cell[s8_Skew4Idx] = sgTSetTmp_t.u8_Cell;


            // skew4 +1 +2 cases of even OkCnt, shift RXDLL +1 to right side.
            if(0==(au8_CellCnt[s8_Skew4Idx]&1) && as8_Skew4Shift[s8_Skew4Idx]>0)
                au8_Cell[s8_Skew4Idx] += 1;

            eMMC_debug(0,0,"             RXDLL: %Xh, RXDLLResult: %Xh, OkCnt: %u \n",
                au8_Cell[s8_Skew4Idx], au32_RXDLLResult[s8_Skew4Idx], au8_CellCnt[s8_Skew4Idx]);
        }
        else
        {
            au8_Cell[s8_Skew4Idx] = au8_Cell[0];
            au32_RXDLLResult[s8_Skew4Idx] = au32_RXDLLResult[0];
            au8_CellCnt[s8_Skew4Idx]= au8_CellCnt[0];
            eMMC_debug(0,0,"             RXDLL: %Xh, RXDLLResult: %Xh, OkCnt: %u \n",
                au8_Cell[s8_Skew4Idx], au32_RXDLLResult[s8_Skew4Idx], au8_CellCnt[s8_Skew4Idx]);
        }

    s8_Skew4Idx++;
    if(s8_Skew4Idx < 5)
        goto LABEL_SCAN_RXDLL;

    // --------------------------------------
    // setup GEN_TIMING_TABLE
    sgTSetTmp_t.u8_CellCnt = 0; // RXDLL ok cnt, used to sort out the best one.

    for(s8_Skew4Idx=0; s8_Skew4Idx<5; s8_Skew4Idx++)
    {
        g_eMMCDrv.TimingTable_G_t.RegSet[s8_Skew4Idx *g_eMMCDrv.TimingTable_G_t.u8_RegisterCnt +GT_OFFSET_RXDLL_VALUE].u32_RegAddress
            = (reg_emmcpll_0x09 - RIU_BASE);
        g_eMMCDrv.TimingTable_G_t.RegSet[s8_Skew4Idx *g_eMMCDrv.TimingTable_G_t.u8_RegisterCnt +GT_OFFSET_RXDLL_VALUE].u16_RegValue
            =(((au8_Cell[s8_Skew4Idx] & 0xFFFE) >>1) <<4) |((au8_Cell[s8_Skew4Idx] %2) ? BIT1:0);
        g_eMMCDrv.TimingTable_G_t.RegSet[s8_Skew4Idx *g_eMMCDrv.TimingTable_G_t.u8_RegisterCnt +GT_OFFSET_RXDLL_VALUE].u16_RegMask
            =((0xF<<4)|BIT1);
        g_eMMCDrv.TimingTable_G_t.RegSet[s8_Skew4Idx *g_eMMCDrv.TimingTable_G_t.u8_RegisterCnt +GT_OFFSET_RXDLL_VALUE].u16_OpCode
            =REG_OP_SETBIT;

        if( s8_Skew4Idx == 2 )
            continue;

        if(sgTSetTmp_t.u8_CellCnt < au8_CellCnt[s8_Skew4Idx])
        {
            sgTSetTmp_t.u8_CellCnt = au8_CellCnt[s8_Skew4Idx];
            g_eMMCDrv.TimingTable_G_t.u8_CurSetIdx = s8_Skew4Idx;
        }
    }

    //backward compatible with old version of kernel
    g_eMMCDrv.TimingTable_t.Set[0].u8_Reg2Ch = au8_Reg2Ch[g_eMMCDrv.TimingTable_G_t.u8_CurSetIdx];
    g_eMMCDrv.TimingTable_t.Set[0].u8_Skew4 = au8_Skew4[g_eMMCDrv.TimingTable_G_t.u8_CurSetIdx];
    g_eMMCDrv.TimingTable_t.Set[0].u8_Cell = au8_Cell[g_eMMCDrv.TimingTable_G_t.u8_CurSetIdx];
    g_eMMCDrv.TimingTable_t.Set[0].u8_CellCnt = sgTSetTmp_t.u8_CellCnt;

    eMMC_FCIE_Apply_Reg(g_eMMCDrv.TimingTable_G_t.u8_CurSetIdx);

    // ================================================
    eMMC_debug(0,0,"\nscan Skew2\n");
    // ================================================
    for(u32_i = 0; u32_i < 9; u32_i ++)
    {
        g_eMMCDrv.u32_DrvFlag |= DRV_FLAG_TUNING_TTABLE; // turn on tunning flag
        REG_FCIE_CLRBIT(reg_emmcpll_0x03, BIT_SKEW2_MASK);
        REG_FCIE_SETBIT(reg_emmcpll_0x03, u32_i<<4);

        if(eMMC_ST_SUCCESS==eMMCTest_BlkWRC_ProbeTiming(eMMC_TEST_BLK_0))
        {
            //u16_Skew2Cnt++;
            u32_Skew2Result |= (1<<u32_i);
            //eMMC_debug(0, 1, "Skew2: %d ok\n", u32_i);
        }
        else
        {   //u32_err = eMMC_FCIE_Init();
            eMMC_debug(0, 1, "Skew2: %d fail\n", u32_i);
            u32_err = eMMC_FCIE_ResetToHS400((U8)g_eMMCDrv.u16_ClkRegVal);

            eMMC_FCIE_Apply_Reg(g_eMMCDrv.TimingTable_G_t.u8_CurSetIdx);

            if(eMMC_ST_SUCCESS != u32_err)
                break;
        }
    }
    g_eMMCDrv.u32_DrvFlag &= ~DRV_FLAG_TUNING_TTABLE; // turn off tunning flag

    u16_Skew2Cnt = eMMC_FCIE_DetectHS400Timing_Skew2_Ex(u32_Skew2Result);
    if(u16_Skew2Cnt ==0)
        return eMMC_ST_ERR_NO_OK_DDR_PARAM;

    REG_FCIE_CLRBIT(reg_emmcpll_0x03, BIT_SKEW2_MASK);
    REG_FCIE_SETBIT(reg_emmcpll_0x03, sgTSetTmp_t.u8_Skew2<<4);
    g_eMMCDrv.TimingTable_t.Set[0].u8_Skew2 = sgTSetTmp_t.u8_Skew2;

    // setup GEN_TIMING_TABLE
    for(s8_Skew4Idx = 0 ; s8_Skew4Idx< 5; s8_Skew4Idx++)
    {
        g_eMMCDrv.TimingTable_G_t.RegSet[s8_Skew4Idx *g_eMMCDrv.TimingTable_G_t.u8_RegisterCnt +GT_OFFSET_SKEW2_VALUE].u32_RegAddress
            = (reg_emmcpll_0x03 - RIU_BASE);
        g_eMMCDrv.TimingTable_G_t.RegSet[s8_Skew4Idx *g_eMMCDrv.TimingTable_G_t.u8_RegisterCnt +GT_OFFSET_SKEW2_VALUE].u16_RegValue
            =sgTSetTmp_t.u8_Skew2<<4;
        g_eMMCDrv.TimingTable_G_t.RegSet[s8_Skew4Idx *g_eMMCDrv.TimingTable_G_t.u8_RegisterCnt +GT_OFFSET_SKEW2_VALUE].u16_RegMask
            =(0xF<<4);
        g_eMMCDrv.TimingTable_G_t.RegSet[s8_Skew4Idx *g_eMMCDrv.TimingTable_G_t.u8_RegisterCnt +GT_OFFSET_SKEW2_VALUE].u16_OpCode
            =REG_OP_SETBIT;
    }

    eMMC_FCIE_Apply_Reg(g_eMMCDrv.TimingTable_G_t.u8_CurSetIdx);
    // ==========================================
    g_eMMCDrv.TimingTable_t.u32_ChkSum =
        eMMC_ChkSum((U8*)&g_eMMCDrv.TimingTable_t,
            sizeof(g_eMMCDrv.TimingTable_t)-eMMC_TIMING_TABLE_CHKSUM_OFFSET);
    memcpy(gau8_eMMC_SectorBuf, (U8*)&g_eMMCDrv.TimingTable_t, sizeof(g_eMMCDrv.TimingTable_t));

    u32_err = eMMC_CMD24(eMMC_HS400TABLE_BLK_0, gau8_eMMC_SectorBuf);
    u32_ret = eMMC_CMD24(eMMC_HS400TABLE_BLK_1, gau8_eMMC_SectorBuf);
    if(eMMC_ST_SUCCESS!=u32_err && eMMC_ST_SUCCESS!=u32_ret)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: Save TTable fail: %Xh %Xh\n",
            u32_err, u32_ret);
        return eMMC_ST_ERR_SAVE_DDRT_FAIL;
    }

    g_eMMCDrv.TimingTable_G_t.u32_VerNo = eMMC_TIMING_TABLE_VERSION;
    g_eMMCDrv.TimingTable_G_t.u8_SpeedMode = FCIE_eMMC_HS400;
    memcpy(g_eMMCDrv.TimingTable_G_t.au8_CID, g_eMMCDrv.au8_CID,eMMC_MAX_RSP_BYTE_CNT - 1);
    eMMC_DumpTimingTable();

    g_eMMCDrv.TimingTable_G_t.u32_ChkSum =
        eMMC_ChkSum((U8*)&g_eMMCDrv.TimingTable_G_t.u32_VerNo,
                    sizeof(g_eMMCDrv.TimingTable_G_t)-sizeof(U32));
    memcpy(gau8_eMMC_SectorBuf, (U8*)&g_eMMCDrv.TimingTable_G_t, sizeof(g_eMMCDrv.TimingTable_G_t));

    u32_err = eMMC_CMD24(eMMC_HS400EXTTABLE_BLK_0, gau8_eMMC_SectorBuf);
    u32_ret = eMMC_CMD24(eMMC_HS400EXTTABLE_BLK_1, gau8_eMMC_SectorBuf);
    if(eMMC_ST_SUCCESS!=u32_err && eMMC_ST_SUCCESS!=u32_ret)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: Save TTable fail: %Xh %Xh\n",
            u32_err, u32_ret);
        return eMMC_ST_ERR_SAVE_DDRT_FAIL;
    }

    return eMMC_ST_SUCCESS;
}

U32 eMMC_FCIE_DetectHS400Timing_SKEW4_RX(U8 u8_enable_error_log)
{
    U32 u32_i = 0, u32_err = 0, u32_RXDLLResult;
    U8  u8_Skew4;
    #if 1
    eMMC_DISABLE_LOG(1);
    g_eMMCDrv.u16_Reg10_Mode = BIT_SD_DEFAULT_MODE_REG;
    eMMC_PlatformInit();
    u32_err = eMMC_FCIE_Init();
    if(eMMC_ST_SUCCESS != u32_err)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"FCIE Reset Fail!!\n");
        eMMC_FCIE_ErrHandler_Stop();
    }
    u32_err = eMMC_Init_Device_Ex();
    if(eMMC_ST_SUCCESS != u32_err)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Identify Error!!\n");
        eMMC_FCIE_ErrHandler_Stop();
    }
    u32_err = eMMC_FCIE_EnableFastMode(FCIE_eMMC_HS400);
    if(eMMC_ST_SUCCESS != u32_err)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC switch to HS400 fail!!\n");
        eMMC_FCIE_ErrHandler_Stop();
    }
    eMMC_DISABLE_LOG(0);
    #endif

    u32_i = 0;
    eMMC_debug(0, 0, "RX           %02u ", u32_i);
    for(u32_i=1; u32_i<16; u32_i++)
        eMMC_debug(0, 0, "%02u ", u32_i);
    eMMC_debug(0, 0, "\n");

    // ==========================================
    for(u8_Skew4=0; u8_Skew4<18; u8_Skew4++)
    {
        if(u8_Skew4 < PLL_SKEW4_CNT)
        {
            g_eMMCDrv.TimingTable_G_t.RegSet[GT_OFFSET_SKEW4_INV_BIT].u16_OpCode =REG_OP_CLRBIT;
            #ifdef REG_DIG_SKEW4_INV
            g_eMMCDrv.TimingTable_G_t.RegSet[GT_OFFSET_SKEW4_INV_BIT_DIG].u16_OpCode =REG_OP_CLRBIT;
            #endif
            g_eMMCDrv.TimingTable_G_t.RegSet[GT_OFFSET_SKEW4_VALUE].u16_RegValue = u8_Skew4 << 12;
        }
        else
        {
            g_eMMCDrv.TimingTable_G_t.RegSet[GT_OFFSET_SKEW4_INV_BIT].u16_OpCode =REG_OP_SETBIT;
            #ifdef REG_DIG_SKEW4_INV
            g_eMMCDrv.TimingTable_G_t.RegSet[GT_OFFSET_SKEW4_INV_BIT_DIG].u16_OpCode =REG_OP_SETBIT;
            #endif
            g_eMMCDrv.TimingTable_G_t.RegSet[GT_OFFSET_SKEW4_VALUE].u16_RegValue = (u8_Skew4-PLL_SKEW4_CNT) << 12;
        }

        eMMC_FCIE_ApplyTimingSet(0);

        // --------------------------------------
        u32_RXDLLResult = 0;

        for(u32_i=0; u32_i<16; u32_i++)
        {
            g_eMMCDrv.u32_DrvFlag |= DRV_FLAG_TUNING_TTABLE; // turn on tunning flag
            if(u8_enable_error_log)// disable tunning error log
                eMMC_DISABLE_LOG(1);

            eMMC_FCIE_SetDelayLine(u32_i);

            if(eMMC_ST_SUCCESS==eMMCTest_BlkWRC_ProbeTiming(eMMC_TEST_BLK_0))
            {
                u32_RXDLLResult |= (1<<u32_i);
            }
            else
            {
                u32_err = eMMC_FCIE_ResetToHS400((U8)g_eMMCDrv.u16_ClkRegVal);
                if(eMMC_ST_SUCCESS != u32_err)
                {
                    g_eMMCDrv.u32_DrvFlag &= ~DRV_FLAG_TUNING_TTABLE; // turn off tunning flag
                    eMMC_DISABLE_LOG(0);
                    eMMC_debug(0, 1, "eMMC Err: RXDLL: %d fail\n", u32_i);
                    break;
                }
            }
        }
        g_eMMCDrv.u32_DrvFlag &= ~DRV_FLAG_TUNING_TTABLE; // turn off tunning flag
        eMMC_DISABLE_LOG(0);
        eMMC_debug(0,0,"Skew4: %02u |  ", u8_Skew4);
        for(u32_i=0; u32_i<16; u32_i++)
        {
            if((u32_RXDLLResult & (1<<u32_i)) == 0)
                printf(" X ");
            else
                printf(" O ");
        }
        printf("\n");
    }
    return eMMC_ST_SUCCESS;
}

U32 eMMC_FCIE_DetectHS400Timing_SKEW4_SKEW1(U8 u8_enable_error_log)
{
    U32 u32_i = 0, u32_j, u32_k, u32_err = 0, u32_Skew1Result =0, u32_Skew4Result =0;
    U8  u8_Skew4;
	S8  s8_Skew4Idx=0;
    U8 u8_SkewFailCount[18]={0};
    U8 u8_MaxCount = 0;
    u8 u8_MaxSkew = 0;
    U8 u8_TrgLvl, u8_DrvStr;
    U8 u8_TrgLvlValue[3]={0, 3, 7};

    #if 1
    eMMC_DISABLE_LOG(1);
    g_eMMCDrv.u16_Reg10_Mode = BIT_SD_DEFAULT_MODE_REG;
    eMMC_PlatformInit();
    u32_err = eMMC_FCIE_Init();
    if(eMMC_ST_SUCCESS != u32_err)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"FCIE Reset Fail!!\n");
        eMMC_FCIE_ErrHandler_Stop();
    }
    u32_err = eMMC_Init_Device_Ex();
    if(eMMC_ST_SUCCESS != u32_err)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Identify Error!!\n");
        eMMC_FCIE_ErrHandler_Stop();
    }
    u32_err = eMMC_FCIE_EnableFastMode(FCIE_eMMC_HS400);
    if(eMMC_ST_SUCCESS != u32_err)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC switch to HS400 fail!!\n");
        eMMC_FCIE_ErrHandler_Stop();
    }
    eMMC_DISABLE_LOG(0);
    #endif

    #if 1
    eMMC_DISABLE_LOG(1);
    // ==========================================
    //add skew4 for hs400 using hs200 + pad setting of HS400 to detect skew4
    for(u8_DrvStr=0; u8_DrvStr<2; u8_DrvStr++)
    {
   	    g_eMMCDrv.u32_LastErrCode = 0;
   	    if(u8_DrvStr)
   		    g_eMMCDrv.u32_LastErrCode |= DRV_FLAG_WEAK_STRENGTH;

   	    for(u8_TrgLvl = 0; u8_TrgLvl < 3; u8_TrgLvl++)
   	    {
     		 memset(u8_SkewFailCount, 0, sizeof(u8_SkewFailCount));
     		 u8_MaxCount = 0;
     		 u8_MaxSkew = 0;
     		 eMMC_SetTrigerLevel(0);
     		 u32_err = eMMC_FCIE_ResetToHS400((U8)gau8_eMMCPLLSel_200[0]);
     		 if(eMMC_ST_SUCCESS != u32_err)
     		 {
			 	 eMMC_DISABLE_LOG(0);
     			 eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: eMMC_FCIE_ResetToHS400 fail: %Xh\n", u32_err);
     			 return u32_err;
     		 }

     		 //scan skew4 for hs400 rsp using skew1 & skew4
     		 for(u32_j = 0; u32_j < 9*2; u32_j ++)
     		 {
     			 u32_Skew4Result = 0;

     			 for(u32_i = 0; u32_i < PLL_SKEW4_CNT*2; u32_i ++)
     			 {
     				 for(u32_k = 0; u32_k < 5; u32_k ++)
     				 {
     					 g_eMMCDrv.u32_DrvFlag |= DRV_FLAG_TUNING_TTABLE; // turn on tunning flag
     					 REG_FCIE_CLRBIT(reg_emmcpll_0x03,BIT_SKEW1_MASK);
     					 if(u32_j < 9)
     					 {
     						 REG_FCIE_CLRBIT(reg_emmcpll_0x69, BIT3);// reg_clk_dig_inv
     						 REG_FCIE_SETBIT(reg_emmcpll_0x03, u32_j);
     					 }
     					 else
     					 {
     						 REG_FCIE_SETBIT(reg_emmcpll_0x69, BIT3);// reg_clk_dig_inv
     						 REG_FCIE_SETBIT(reg_emmcpll_0x03, u32_j-9);
     					 }

     					 if(u32_i >= PLL_SKEW4_CNT)
     					 {
     					 #ifdef REG_DIG_SKEW4_INV
     						 REG_FCIE_SETBIT(REG_DIG_SKEW4_INV, BIT_DIG_SKEW4_INV);//skew4 inverse for muji
     					 #endif

     					 #ifdef REG_ANL_SKEW4_INV
     						 REG_FCIE_SETBIT(REG_ANL_SKEW4_INV, BIT_ANL_SKEW4_INV);
     					 #endif

     						 REG_FCIE_CLRBIT(reg_emmcpll_0x03, BIT_SKEW4_MASK);
     						 REG_FCIE_SETBIT(reg_emmcpll_0x03, (u32_i-PLL_SKEW4_CNT)<<12);
     					 }
     					 else
     					 {
     					 #ifdef REG_DIG_SKEW4_INV
     						 REG_FCIE_CLRBIT(REG_DIG_SKEW4_INV, BIT_DIG_SKEW4_INV);//skew4 inverse for muji
     					 #endif

     					 #ifdef REG_ANL_SKEW4_INV
     						 REG_FCIE_CLRBIT(REG_ANL_SKEW4_INV, BIT_ANL_SKEW4_INV);
     					 #endif

     						 REG_FCIE_CLRBIT(reg_emmcpll_0x03, BIT_SKEW4_MASK);
     						 REG_FCIE_SETBIT(reg_emmcpll_0x03, (u32_i)<<12);
     					 }

     					 eMMC_SetTrigerLevel(u8_TrgLvlValue[u8_TrgLvl]);
     					 u32_err = eMMC_CMD13(g_eMMCDrv.u16_RCA);
     					 if(u32_err == eMMC_ST_SUCCESS)
     					 {
     						 //u32_Skew4Result |= (1<<u32_i);
     						 continue;
     					 }
     					 else
     					 {
     						 eMMC_SetTrigerLevel(0);
     						 u32_err = eMMC_FCIE_ResetToHS400((U8)g_eMMCDrv.u16_ClkRegVal);
     						 if(eMMC_ST_SUCCESS != u32_err)
     						 {
     							 eMMC_debug(0,0,"scan Skew1 & skew4 fail\n");
     							 g_eMMCDrv.u32_DrvFlag &= ~DRV_FLAG_TUNING_TTABLE; // turn off tunning flag
     							 break;
     						 }
     						 break;
     					 }
     				 }
     				 if(u32_k == 5)
     					 u32_Skew4Result |= (1<<u32_i);
     			 }

                #if 0
                printf("Skew1: %02u |  ", u32_j);
                for(u32_i=0; u32_i<18; u32_i++)
                {
                    if((u32_Skew4Result & (1<<u32_i)) == 0)
                        printf(" X ");
                    else
                        printf(" O ");
                }
                printf("\n");
                #endif
    			 if(((u32_Skew4Result & ((1<<PLL_SKEW4_CNT*2)-1))!=((1<<PLL_SKEW4_CNT*2)-1)) && u32_Skew4Result != 0)
    			 {
    				 //search the fail level
    				 for(u32_i = 0; u32_i < PLL_SKEW4_CNT*2; u32_i ++)
    				 {
    					 if((u32_Skew4Result & (1 << u32_i)) == 0)
    					 {
    						 s8_Skew4Idx = (S32)u32_i - (S32)u32_j;
    						 if(s8_Skew4Idx < 0)
    							 s8_Skew4Idx += PLL_SKEW4_CNT*2;
    						 u8_SkewFailCount[s8_Skew4Idx] ++;
    					 }
    				 }
    			 }
    		 }
    		 for(u32_i = 0; u32_i < PLL_SKEW4_CNT*2; u32_i ++)
    		 {
    			 //printf("skew4 %d: %d\n", u32_i, u8_SkewFailCount[u32_i]);
    			 if(u8_MaxCount < u8_SkewFailCount[u32_i])
    			 {
    				 u8_MaxSkew = u32_i;
    				 u8_MaxCount = u8_SkewFailCount[u32_i];
    			 }
    		 }

    		 s8_Skew4Idx = u8_MaxSkew - PLL_SKEW4_CNT;
    		 if(s8_Skew4Idx < 0)
    			 s8_Skew4Idx += PLL_SKEW4_CNT*2;

    		 g_eMMCDrv.u32_DrvFlag &= ~DRV_FLAG_TUNING_TTABLE; // turn off tunning flag

    		 s8_Skew4Idx = 0;
    		 if(u8_MaxCount >= 3)
    			 break;
   	     }
    	 if(u8_MaxCount >= 3)
    	 {
             printf("eMMC DrvStr: 0x%X, TrgLvl: 0x%X\n", u8_DrvStr,u8_TrgLvlValue[u8_TrgLvl]);

    		 eMMC_SetTrigerLevel(u8_TrgLvlValue[u8_TrgLvl]);
    		 u32_err = eMMC_FCIE_ResetToHS400((U8)gau8_eMMCPLLSel_200[0]);
    		 if(eMMC_ST_SUCCESS != u32_err)
    		 {
			 	 eMMC_DISABLE_LOG(0);
    			 eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: eMMC_FCIE_ResetToHS400 fail: %Xh\n", u32_err);
    			 return u32_err;
    		 }
    		 break;
    	 }
    }
	if(u8_MaxCount < 3)
	{
		eMMC_DISABLE_LOG(0);
		eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1, "eMMC Err: HS400 scan skew4 fail\n");
		eMMC_FCIE_ErrHandler_Stop();
	}
	eMMC_DISABLE_LOG(0);

	#endif
    // ==========================================

    u32_i = 0;
    eMMC_debug(0, 0, "Skew1        %02u ", u32_i);
    for(u32_i=1; u32_i<18; u32_i++)
        eMMC_debug(0, 0, "%02u ", u32_i);
    eMMC_debug(0, 0, "\n");

    // ==========================================
    for(u8_Skew4=0; u8_Skew4<18; u8_Skew4++)
    {
        if(u8_Skew4 < PLL_SKEW4_CNT)
        {
            g_eMMCDrv.TimingTable_G_t.RegSet[GT_OFFSET_SKEW4_INV_BIT].u16_OpCode =REG_OP_CLRBIT;
            #ifdef REG_DIG_SKEW4_INV
            g_eMMCDrv.TimingTable_G_t.RegSet[GT_OFFSET_SKEW4_INV_BIT_DIG].u16_OpCode =REG_OP_CLRBIT;
            #endif
            g_eMMCDrv.TimingTable_G_t.RegSet[GT_OFFSET_SKEW4_VALUE].u16_RegValue = u8_Skew4 << 12;
        }
        else
        {
            g_eMMCDrv.TimingTable_G_t.RegSet[GT_OFFSET_SKEW4_INV_BIT].u16_OpCode =REG_OP_SETBIT;
            #ifdef REG_DIG_SKEW4_INV
            g_eMMCDrv.TimingTable_G_t.RegSet[GT_OFFSET_SKEW4_INV_BIT_DIG].u16_OpCode =REG_OP_SETBIT;
            #endif
            g_eMMCDrv.TimingTable_G_t.RegSet[GT_OFFSET_SKEW4_VALUE].u16_RegValue = (u8_Skew4-PLL_SKEW4_CNT) << 12;

        }
        eMMC_FCIE_ApplyTimingSet(0);
        // --------------------------------------
        u32_Skew1Result = 0;

        for(u32_i=0; u32_i<18; u32_i++)
        {
            g_eMMCDrv.u32_DrvFlag |= DRV_FLAG_TUNING_TTABLE; // turn on tunning flag
            if(u8_enable_error_log)// disable tunning error log
                eMMC_DISABLE_LOG(1);
            if(u32_i<9)
            {
                REG_FCIE_CLRBIT(reg_emmcpll_0x69, BIT3);// reg_clk_dig_inv
                REG_FCIE_CLRBIT(reg_emmcpll_0x03, BIT_SKEW1_MASK);
                REG_FCIE_SETBIT(reg_emmcpll_0x03, u32_i);
            }
            else
            {
                REG_FCIE_SETBIT(reg_emmcpll_0x69, BIT3);// reg_clk_dig_inv
                REG_FCIE_CLRBIT(reg_emmcpll_0x03, BIT_SKEW1_MASK);
                REG_FCIE_SETBIT(reg_emmcpll_0x03, u32_i-9);
            }
            if(eMMC_ST_SUCCESS==eMMC_CMD13(g_eMMCDrv.u16_RCA))
            {
                u32_Skew1Result |= (1<<u32_i);
            }
            else
            {
                u32_err = eMMC_FCIE_ResetToHS400((U8)g_eMMCDrv.u16_ClkRegVal);
                if(eMMC_ST_SUCCESS != u32_err)
                {
                    g_eMMCDrv.u32_DrvFlag &= ~DRV_FLAG_TUNING_TTABLE; // turn off tunning flag
                    eMMC_DISABLE_LOG(0);
                    eMMC_debug(0, 1, "eMMC Err: SKEW1: %d fail\n", u32_i);
                    break;
                }
            }
        }
        g_eMMCDrv.u32_DrvFlag &= ~DRV_FLAG_TUNING_TTABLE; // turn off tunning flag
        eMMC_DISABLE_LOG(0);
        eMMC_debug(0,0,"Skew4: %02u |  ", u8_Skew4);
        for(u32_i=0; u32_i<18; u32_i++)
        {
            if((u32_Skew1Result & (1<<u32_i)) == 0)
                printf(" X ");
            else
                printf(" O ");
        }
        printf("\n");
    }

    // ==========================================
	#if 1
	g_eMMCDrv.u32_LastErrCode &= ~ DRV_FLAG_WEAK_STRENGTH;
	eMMC_DISABLE_LOG(1);
	eMMC_SetTrigerLevel(0);
	eMMC_DISABLE_LOG(0);
	u32_err = eMMC_FCIE_ResetToHS400((U8)gau8_eMMCPLLSel_200[0]);
	if(eMMC_ST_SUCCESS != u32_err)
	{
		eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: eMMC_FCIE_ResetToHS400 fail: %Xh\n", u32_err);
		return u32_err;
	}
	#endif
    return eMMC_ST_SUCCESS;
}


U32 eMMC_FCIE_DetectHS400Timing_SKEW1_SKEW2(U8 u8_enable_error_log)
{
    U32 u32_i = 0, u32_err = 0, u32_Skew2Result;
    U8  u8_Skew1;
    #if 1
    eMMC_DISABLE_LOG(1);
    g_eMMCDrv.u16_Reg10_Mode = BIT_SD_DEFAULT_MODE_REG;
    eMMC_PlatformInit();
    u32_err = eMMC_FCIE_Init();
    if(eMMC_ST_SUCCESS != u32_err)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"FCIE Reset Fail!!\n");
        eMMC_FCIE_ErrHandler_Stop();
    }
    u32_err = eMMC_Init_Device_Ex();
    if(eMMC_ST_SUCCESS != u32_err)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Identify Error!!\n");
        eMMC_FCIE_ErrHandler_Stop();
    }
    u32_err = eMMC_FCIE_EnableFastMode(FCIE_eMMC_HS400);
    if(eMMC_ST_SUCCESS != u32_err)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC switch to HS400 fail!!\n");
        eMMC_FCIE_ErrHandler_Stop();
    }
    eMMC_DISABLE_LOG(0);
    #endif

    u32_i = 0;
    eMMC_debug(0, 0, "Skew2           %02u ", u32_i);
    for(u32_i=1; u32_i<9; u32_i++)
        eMMC_debug(0, 0, "%02u ", u32_i);
    eMMC_debug(0, 0, "\n");

    // ==========================================
    for(u8_Skew1=0; u8_Skew1<18; u8_Skew1++)
    {
        // --------------------------------------
        u32_Skew2Result = 0;

        for(u32_i=0; u32_i<9; u32_i++)
        {
            g_eMMCDrv.u32_DrvFlag |= DRV_FLAG_TUNING_TTABLE; // turn on tunning flag
            if(u8_enable_error_log)// disable tunning error log
                eMMC_DISABLE_LOG(1);

            g_eMMCDrv.TimingTable_G_t.RegSet[GT_OFFSET_SKEW2_VALUE].u16_RegValue =u32_i<<4;
            eMMC_FCIE_ApplyTimingSet(0);
            if(u8_Skew1<9)
            {
                REG_FCIE_CLRBIT(reg_emmcpll_0x69, BIT3);// reg_clk_dig_inv
                REG_FCIE_CLRBIT(reg_emmcpll_0x03, BIT_SKEW1_MASK);
                REG_FCIE_SETBIT(reg_emmcpll_0x03, u8_Skew1);
            }
            else
            {
                REG_FCIE_SETBIT(reg_emmcpll_0x69, BIT3);// reg_clk_dig_inv
                REG_FCIE_CLRBIT(reg_emmcpll_0x03, BIT_SKEW1_MASK);
                REG_FCIE_SETBIT(reg_emmcpll_0x03, u8_Skew1-9);
            }

			REG_FCIE_CLRBIT(reg_emmcpll_0x03, BIT_SKEW2_MASK);
			REG_FCIE_SETBIT(reg_emmcpll_0x03, u32_i<<4);

            if(eMMC_ST_SUCCESS==eMMCTest_BlkWRC_ProbeTiming(eMMC_TEST_BLK_0))
            {
                u32_Skew2Result |= (1<<u32_i);
            }
            else
            {
                u32_err = eMMC_FCIE_ResetToHS400((U8)g_eMMCDrv.u16_ClkRegVal);
                if(eMMC_ST_SUCCESS != u32_err)
                {
                    g_eMMCDrv.u32_DrvFlag &= ~DRV_FLAG_TUNING_TTABLE; // turn off tunning flag
                    eMMC_debug(0, 1, "eMMC Err: SKEW2: %d fail\n", u32_i);
                    break;
                }
            }
        }
        g_eMMCDrv.u32_DrvFlag &= ~DRV_FLAG_TUNING_TTABLE; // turn off tunning flag
        eMMC_DISABLE_LOG(0);

        eMMC_debug(0,0,"Skew1: %02u |  ", u8_Skew1);
        for(u32_i=0; u32_i<9; u32_i++)
        {
            if((u32_Skew2Result & (1<<u32_i)) == 0)
                printf(" X ");
            else
                printf(" O ");
        }
        printf("\n");
    }
    return eMMC_ST_SUCCESS;
}

U32 gu32RXDLLResult = 0xFFFF;
U32 gu32RXDLLResultLv0 = 0xFFFF;

U32 eMMC_FCIE_DetectHS400Timing_TrigerLevel_RX(U8 u8_enable_error_log)
{
    U32 u32_i = 0, u32_err = 0, u32_RXDLLResult;
    U8  TrigerLevel;
    #if 1
    eMMC_DISABLE_LOG(1);
    g_eMMCDrv.u16_Reg10_Mode = BIT_SD_DEFAULT_MODE_REG;
    eMMC_PlatformInit();
    u32_err = eMMC_FCIE_Init();
    if(eMMC_ST_SUCCESS != u32_err)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"FCIE Reset Fail!!\n");
        eMMC_FCIE_ErrHandler_Stop();
    }
    u32_err = eMMC_Init_Device_Ex();
    if(eMMC_ST_SUCCESS != u32_err)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Identify Error!!\n");
        eMMC_FCIE_ErrHandler_Stop();
    }
    u32_err = eMMC_FCIE_EnableFastMode(FCIE_eMMC_HS400);
    if(eMMC_ST_SUCCESS != u32_err)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC switch to HS400 fail!!\n");
        eMMC_FCIE_ErrHandler_Stop();
    }
    eMMC_DISABLE_LOG(0);
    #endif

    u32_i = 0;
    eMMC_debug(0, 0, "RX                 %02u ", u32_i);
    for(u32_i=1; u32_i<16; u32_i++)
        eMMC_debug(0, 0, "%02u ", u32_i);
    eMMC_debug(0, 0, "\n");

    // ==========================================
    for(TrigerLevel=0; TrigerLevel<8; TrigerLevel++)
    {
        if(u8_enable_error_log)// disable tunning error log
            eMMC_DISABLE_LOG(1);

        eMMC_SetTrigerLevel(TrigerLevel);
        // --------------------------------------
        u32_RXDLLResult = 0;

        for(u32_i=0; u32_i<16; u32_i++)
        {
            g_eMMCDrv.u32_DrvFlag |= DRV_FLAG_TUNING_TTABLE; // turn on tunning flag

            eMMC_FCIE_SetDelayLine(u32_i);

            if(eMMC_ST_SUCCESS==eMMCTest_BlkWRC_ProbeTiming(eMMC_TEST_BLK_0))
            {
                u32_RXDLLResult |= (1<<u32_i);
            }
            else
            {
                u32_err = eMMC_FCIE_ResetToHS400((U8)g_eMMCDrv.u16_ClkRegVal);
                if(eMMC_ST_SUCCESS != u32_err)
                {
                    g_eMMCDrv.u32_DrvFlag &= ~DRV_FLAG_TUNING_TTABLE; // turn off tunning flag
                    eMMC_DISABLE_LOG(0);
                    eMMC_debug(0, 1, "eMMC Err: RXDLL: %d fail\n", u32_i);
                    break;
                }
            }

        }
        g_eMMCDrv.u32_DrvFlag &= ~DRV_FLAG_TUNING_TTABLE; // turn off tunning flag
        eMMC_DISABLE_LOG(0);
        eMMC_debug(0,0,"TrigerLevel: %02u |  ", TrigerLevel);
        for(u32_i=0; u32_i<16; u32_i++)
        {
            if((u32_RXDLLResult & (1<<u32_i)) == 0)
                printf(" X ");
            else
                printf(" O ");
        }
        printf("\n");

        if( TrigerLevel == 0 )
            gu32RXDLLResultLv0 &= u32_RXDLLResult;

        gu32RXDLLResult &= u32_RXDLLResult;

    }
    REG_FCIE_CLRBIT(reg_emmcpll_0x20, Trig_lvl_MASK);

    return eMMC_ST_SUCCESS;
}

U32 eMMC_FCIE_DetectRXDLL(void)
{
    U32 u32_i = 0, u32_err = 0, u32_RXDLLResult = 0;

    eMMC_DISABLE_LOG(1);

    g_eMMCDrv.u16_Reg10_Mode = BIT_SD_DEFAULT_MODE_REG;

    eMMC_PlatformInit();

    u32_err = eMMC_FCIE_Init();
    if(eMMC_ST_SUCCESS != u32_err)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"FCIE Reset Fail!!\n");
        eMMC_FCIE_ErrHandler_Stop();
    }

    u32_err = eMMC_Init_Device_Ex();
    if(eMMC_ST_SUCCESS != u32_err)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Identify Error!!\n");
        eMMC_FCIE_ErrHandler_Stop();
    }
    u32_err = eMMC_FCIE_EnableFastMode(FCIE_eMMC_HS400);
    if(eMMC_ST_SUCCESS != u32_err)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC switch to HS400 fail!!\n");
        eMMC_FCIE_ErrHandler_Stop();
    }

    eMMC_DISABLE_LOG(0);

    eMMC_debug(0, 0, "RXDLL:\n");
    for(u32_i=0; u32_i<16; u32_i++)
        eMMC_debug(0, 0, "%02u ", u32_i);
    eMMC_debug(0, 0, "\n");

    eMMC_FCIE_ApplyTimingSet(0);

    for(u32_i=0; u32_i<16; u32_i++)
    {
        g_eMMCDrv.u32_DrvFlag |= DRV_FLAG_TUNING_TTABLE;

        eMMC_DISABLE_LOG(1);

        eMMC_FCIE_SetDelayLine(u32_i);

        if(eMMC_ST_SUCCESS==eMMCTest_BlkWRC_ProbeTiming(eMMC_TEST_BLK_0))
        {
            u32_RXDLLResult |= (1<<u32_i);
        }
        else
        {
            u32_err = eMMC_FCIE_ResetToHS400((U8)g_eMMCDrv.u16_ClkRegVal);
            if(eMMC_ST_SUCCESS != u32_err)
            {
                g_eMMCDrv.u32_DrvFlag &= ~DRV_FLAG_TUNING_TTABLE; // turn off tunning flag
                eMMC_DISABLE_LOG(0);
                eMMC_debug(0, 1, "eMMC Err: RXDLL: %d fail\n", u32_i);
                break;
            }
        }
    }

    g_eMMCDrv.u32_DrvFlag &= ~DRV_FLAG_TUNING_TTABLE; // turn off tunning flag

    eMMC_DISABLE_LOG(0);

    for(u32_i=0; u32_i<16; u32_i++)
    {
        if((u32_RXDLLResult & (1<<u32_i)) == 0)
            printf(" X ");
        else
            printf(" O ");
    }
    printf("\n");

    return eMMC_ST_SUCCESS;
}


#endif

#if defined(ENABLE_eMMC_HS400_5_1)&&ENABLE_eMMC_HS400_5_1

U32 eMMC_FCIE_ResetToHS400_5_1(U8 u8_ClkParam)
{
    U32 u32_err;

    //eMMC_debug(0, 0, "\033[7;31m%s\033[m\n", __FUNCTION__);

    // --------------------------------
    // reset FCIE & eMMC to normal SDR mode
    if(!eMMC_IF_NORMAL_SDR())
    {
        //eMMC_debug(0,0,"eMMC: re-init to SDR\n");
        g_eMMCDrv.u32_DrvFlag = 0;
        eMMC_PlatformInit();
        u32_err = eMMC_FCIE_Init();
        if(u32_err)
        {
            eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: eMMC_FCIE_Init fail, %Xh\n", u32_err);
            return u32_err;
        }
        u32_err = eMMC_Init_Device_Ex();
        if(eMMC_ST_SUCCESS != u32_err)
        {
            eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,0,"eMMC Err: eMMC_Init_Device_Ex fail: %X\n", u32_err);
            return u32_err;
        }
    }

    // --------------------------------


    u32_err = eMMC_FCIE_EnableFastMode_Ex(FCIE_eMMC_HS400_5_1);
    if(eMMC_ST_SUCCESS != u32_err)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,0,"eMMC Err: set HS400 5.1 IF fail: %X\n", u32_err);

        return u32_err;
    }

    eMMC_clock_setting(u8_ClkParam);

    return u32_err;
}

U32 eMMC_FCIE_DetectHS400_5_1_Timing(void)
{
    U32 u32_i = 0, u32_j, u32_k, u32_err = 0, u32_Skew2Result = 0;
    U16 u16_Skew2Cnt = 0;
    U32 u32_ret =0;
    U32 au32_RXDLLResult=0;
    U8 au8_Cell, au8_CellCnt;

    memset((void*)&g_eMMCDrv.TimingTable_t, 0, sizeof(g_eMMCDrv.TimingTable_t));
    memset((void*)&g_eMMCDrv.TimingTable_G_t, 0, sizeof(g_eMMCDrv.TimingTable_G_t));

    g_eMMCDrv.TimingTable_G_t.u8_SetCnt = 1;        //initial set count for apply general register

    g_eMMCDrv.TimingTable_G_t.u8_RegisterCnt = 4;   //skew4 & its inverse register, RXDLL & its inverse & skew2
    #ifdef REG_DIG_SKEW4_INV
    g_eMMCDrv.TimingTable_G_t.u8_RegisterCnt ++;    //additional skew4 for muji
    #endif
    g_eMMCDrv.TimingTable_G_t.u8_CurSetIdx = 0;

	g_eMMCDrv.TimingTable_G_t.u32_Clk = gau8_eMMCPLLSel_200[0];



    // setup GEN_TIMING_TABLE
    //skew4 invesre
    #ifdef REG_ANL_SKEW4_INV
    g_eMMCDrv.TimingTable_G_t.RegSet[GT_OFFSET_SKEW4_INV_BIT].u32_RegAddress = (REG_ANL_SKEW4_INV - RIU_BASE);
    g_eMMCDrv.TimingTable_G_t.RegSet[GT_OFFSET_SKEW4_INV_BIT].u16_RegValue   = BIT_ANL_SKEW4_INV;
    g_eMMCDrv.TimingTable_G_t.RegSet[GT_OFFSET_SKEW4_INV_BIT].u16_RegMask    = BIT_ANL_SKEW4_INV;
    g_eMMCDrv.TimingTable_G_t.RegSet[GT_OFFSET_SKEW4_INV_BIT].u16_OpCode     = REG_OP_CLRBIT;
    #endif
    //skew4 value
    g_eMMCDrv.TimingTable_G_t.RegSet[GT_OFFSET_SKEW4_VALUE].u32_RegAddress = (reg_emmcpll_0x03 - RIU_BASE);
    g_eMMCDrv.TimingTable_G_t.RegSet[GT_OFFSET_SKEW4_VALUE].u16_RegValue   = BIT_SKEW4_MASK;
    g_eMMCDrv.TimingTable_G_t.RegSet[GT_OFFSET_SKEW4_VALUE].u16_RegMask    = BIT_SKEW4_MASK;
    g_eMMCDrv.TimingTable_G_t.RegSet[GT_OFFSET_SKEW4_VALUE].u16_OpCode     = REG_OP_CLRBIT;
    #ifdef REG_DIG_SKEW4_INV
    g_eMMCDrv.TimingTable_G_t.RegSet[GT_OFFSET_SKEW4_INV_BIT_DIG].u32_RegAddress = (REG_DIG_SKEW4_INV - RIU_BASE);
    g_eMMCDrv.TimingTable_G_t.RegSet[GT_OFFSET_SKEW4_INV_BIT_DIG].u16_RegValue   = BIT_DIG_SKEW4_INV;
    g_eMMCDrv.TimingTable_G_t.RegSet[GT_OFFSET_SKEW4_INV_BIT_DIG].u16_RegMask    = BIT_DIG_SKEW4_INV;
    g_eMMCDrv.TimingTable_G_t.RegSet[GT_OFFSET_SKEW4_INV_BIT_DIG].u16_OpCode     = REG_OP_CLRBIT;
	#endif

    // ================================================
    // scan RXDLL
    // ================================================

	u32_err = eMMC_FCIE_ResetToHS400_5_1((U8)gau8_eMMCPLLSel_200[0]);
	if(eMMC_ST_SUCCESS != u32_err)
	{
		eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: eMMC_FCIE_ResetToHS400 5.1 fail: %Xh\n", u32_err);
		return u32_err;
	}

    eMMC_debug(0,0,"\nscan RXDLL\n");
    for(u32_i=0; u32_i<16; u32_i++)
    {
        g_eMMCDrv.u32_DrvFlag |= DRV_FLAG_TUNING_TTABLE; // turn on tunning flag
        //using default skew2 to detect RXDLL
        REG_FCIE_CLRBIT(reg_emmcpll_0x03, BIT_SKEW2_MASK);
        REG_FCIE_SETBIT(reg_emmcpll_0x03, BIT_DEFAULT_SKEW2);

        eMMC_FCIE_SetDelayLine(u32_i);

        if(eMMC_ST_SUCCESS==eMMCTest_BlkWRC_ProbeTiming(eMMC_TEST_BLK_0))
        {
            au32_RXDLLResult |= (1<<u32_i);
            //eMMC_debug(0, 1, "RXDLL: %d ok\n", u32_i);
        }
        else
        {
            eMMC_debug(0, 1, "RXDLL: %d fail\n", u32_i);
            u32_err = eMMC_FCIE_ResetToHS400_5_1((U8)gau8_eMMCPLLSel_200[0]);
            if(eMMC_ST_SUCCESS != u32_err)
                break;
        }
    }

    g_eMMCDrv.u32_DrvFlag &= ~DRV_FLAG_TUNING_TTABLE; // turn off tunning flag

    au8_CellCnt = eMMC_FCIE_DetectHS400Timing_Ex(au32_RXDLLResult);

    if(au8_CellCnt < 7)
	    eMMC_FCIE_ErrHandler_Stop();

    #if 1
    for(u32_i=0; u32_i<16; u32_i++)
    {
        printf("%02X ", u32_i);
    }
    printf("\n");

    for(u32_i=0; u32_i<16; u32_i++)
    {
        if( (au32_RXDLLResult & (1<<u32_i)) == 0 )
            printf(" X ");
        else
            printf(" O ");
    }
    printf("\n");
    #endif

    au8_Cell = sgTSetTmp_t.u8_Cell;


    // --------------------------------------
    // setup GEN_TIMING_TABLE
    g_eMMCDrv.TimingTable_G_t.RegSet[GT_OFFSET_RXDLL_VALUE].u32_RegAddress
        = (reg_emmcpll_0x09 - RIU_BASE);
    g_eMMCDrv.TimingTable_G_t.RegSet[GT_OFFSET_RXDLL_VALUE].u16_RegValue
        =(((au8_Cell & 0xFFFE) >>1) <<4) |((au8_Cell %2) ? BIT1:0);
    g_eMMCDrv.TimingTable_G_t.RegSet[GT_OFFSET_RXDLL_VALUE].u16_RegMask
        =((0xF<<4)|BIT1);
    g_eMMCDrv.TimingTable_G_t.RegSet[GT_OFFSET_RXDLL_VALUE].u16_OpCode
        =REG_OP_SETBIT;


    //backward compatible with old version of kernel
    g_eMMCDrv.TimingTable_t.u8_SetCnt++;
	g_eMMCDrv.TimingTable_t.Set[0].u8_Clk = gau8_eMMCPLLSel_200[0];
    g_eMMCDrv.TimingTable_t.Set[0].u8_Reg2Ch = 0;
    g_eMMCDrv.TimingTable_t.Set[0].u8_Skew4 = 0;
	g_eMMCDrv.TimingTable_t.Set[0].u32_ScanResult = 0;
    g_eMMCDrv.TimingTable_t.Set[0].u8_Cell = au8_Cell;
    g_eMMCDrv.TimingTable_t.Set[0].u8_CellCnt = sgTSetTmp_t.u8_CellCnt;
	g_eMMCDrv.TimingTable_t.u32_VerNo = eMMC_TIMING_TABLE_VERSION;

    eMMC_FCIE_Apply_Reg(g_eMMCDrv.TimingTable_G_t.u8_CurSetIdx);

    // ================================================
    eMMC_debug(0,0,"\nscan Skew2\n");
    // ================================================
    for(u32_i = 0; u32_i < 9; u32_i ++)
    {
        g_eMMCDrv.u32_DrvFlag |= DRV_FLAG_TUNING_TTABLE; // turn on tunning flag
        REG_FCIE_CLRBIT(reg_emmcpll_0x03, BIT_SKEW2_MASK);
        REG_FCIE_SETBIT(reg_emmcpll_0x03, u32_i<<4);

        if(eMMC_ST_SUCCESS==eMMCTest_BlkWRC_ProbeTiming(eMMC_TEST_BLK_0))
        {
            //u16_Skew2Cnt++;
            u32_Skew2Result |= (1<<u32_i);
            //eMMC_debug(0, 1, "Skew2: %d ok\n", u32_i);
        }
        else
        {   //u32_err = eMMC_FCIE_Init();
            eMMC_debug(0, 1, "Skew2: %d fail\n", u32_i);
            u32_err = eMMC_FCIE_ResetToHS400_5_1((U8)gau8_eMMCPLLSel_200[0]);

            eMMC_FCIE_Apply_Reg(g_eMMCDrv.TimingTable_G_t.u8_CurSetIdx);

            if(eMMC_ST_SUCCESS != u32_err)
                break;
        }
    }

    g_eMMCDrv.u32_DrvFlag &= ~DRV_FLAG_TUNING_TTABLE; // turn off tunning flag

    u16_Skew2Cnt = eMMC_FCIE_DetectHS400Timing_Skew2_Ex(u32_Skew2Result);
    if(u16_Skew2Cnt ==0)
        return eMMC_ST_ERR_NO_OK_DDR_PARAM;

    REG_FCIE_CLRBIT(reg_emmcpll_0x03, BIT_SKEW2_MASK);
    REG_FCIE_SETBIT(reg_emmcpll_0x03, sgTSetTmp_t.u8_Skew2<<4);
    g_eMMCDrv.TimingTable_t.Set[0].u8_Skew2 = sgTSetTmp_t.u8_Skew2;

    // setup GEN_TIMING_TABLE
    g_eMMCDrv.TimingTable_G_t.RegSet[GT_OFFSET_SKEW2_VALUE].u32_RegAddress
        = (reg_emmcpll_0x03 - RIU_BASE);
    g_eMMCDrv.TimingTable_G_t.RegSet[GT_OFFSET_SKEW2_VALUE].u16_RegValue
        =sgTSetTmp_t.u8_Skew2<<4;
    g_eMMCDrv.TimingTable_G_t.RegSet[GT_OFFSET_SKEW2_VALUE].u16_RegMask
        =(0xF<<4);
    g_eMMCDrv.TimingTable_G_t.RegSet[GT_OFFSET_SKEW2_VALUE].u16_OpCode
        =REG_OP_SETBIT;

    eMMC_FCIE_Apply_Reg(g_eMMCDrv.TimingTable_G_t.u8_CurSetIdx);

    // ==========================================
    g_eMMCDrv.TimingTable_t.u32_ChkSum =
        eMMC_ChkSum((U8*)&g_eMMCDrv.TimingTable_t,
            sizeof(g_eMMCDrv.TimingTable_t)-eMMC_TIMING_TABLE_CHKSUM_OFFSET);
    memcpy(gau8_eMMC_SectorBuf, (U8*)&g_eMMCDrv.TimingTable_t, sizeof(g_eMMCDrv.TimingTable_t));

    u32_err = eMMC_CMD24(eMMC_HS400TABLE_BLK_0, gau8_eMMC_SectorBuf);
    u32_ret = eMMC_CMD24(eMMC_HS400TABLE_BLK_1, gau8_eMMC_SectorBuf);
    if(eMMC_ST_SUCCESS!=u32_err && eMMC_ST_SUCCESS!=u32_ret)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: Save TTable fail: %Xh %Xh\n",
            u32_err, u32_ret);
        return eMMC_ST_ERR_SAVE_DDRT_FAIL;
    }

    g_eMMCDrv.TimingTable_G_t.u32_VerNo = eMMC_TIMING_TABLE_VERSION;
    g_eMMCDrv.TimingTable_G_t.u8_SpeedMode = FCIE_eMMC_HS400_5_1;
    memcpy(g_eMMCDrv.TimingTable_G_t.au8_CID, g_eMMCDrv.au8_CID,eMMC_MAX_RSP_BYTE_CNT - 1);
    eMMC_DumpTimingTable();

    g_eMMCDrv.TimingTable_G_t.u32_ChkSum =
        eMMC_ChkSum((U8*)&g_eMMCDrv.TimingTable_G_t.u32_VerNo,
                    sizeof(g_eMMCDrv.TimingTable_G_t)-sizeof(U32));
    memcpy(gau8_eMMC_SectorBuf, (U8*)&g_eMMCDrv.TimingTable_G_t, sizeof(g_eMMCDrv.TimingTable_G_t));

    u32_err = eMMC_CMD24(eMMC_HS400EXTTABLE_BLK_0, gau8_eMMC_SectorBuf);
    u32_ret = eMMC_CMD24(eMMC_HS400EXTTABLE_BLK_1, gau8_eMMC_SectorBuf);
    if(eMMC_ST_SUCCESS!=u32_err && eMMC_ST_SUCCESS!=u32_ret)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: Save TTable fail: %Xh %Xh\n",
            u32_err, u32_ret);
        return eMMC_ST_ERR_SAVE_DDRT_FAIL;
    }

    return eMMC_ST_SUCCESS;
}

U32 eMMC_FCIE_DetectHS400_5_1_Timing_RX(U8 u8_enable_error_log)
{
    U32 u32_i = 0, u32_err = 0, u32_RXDLLResult;

    #if 1
    eMMC_DISABLE_LOG(1);
    g_eMMCDrv.u16_Reg10_Mode = BIT_SD_DEFAULT_MODE_REG;
    eMMC_PlatformInit();
    u32_err = eMMC_FCIE_Init();
    if(eMMC_ST_SUCCESS != u32_err)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"FCIE Reset Fail!!\n");
        eMMC_FCIE_ErrHandler_Stop();
    }
    u32_err = eMMC_Init_Device_Ex();
    if(eMMC_ST_SUCCESS != u32_err)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Identify Error!!\n");
        eMMC_FCIE_ErrHandler_Stop();
    }
    u32_err = eMMC_FCIE_EnableFastMode(FCIE_eMMC_HS400_5_1);
    if(eMMC_ST_SUCCESS != u32_err)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC 5.1 switch to HS400 fail!!\n");
        eMMC_FCIE_ErrHandler_Stop();
    }

    eMMC_DISABLE_LOG(0);
    #endif
    eMMC_debug(0, 0, "RX:\n");
    u32_i = 0;
    eMMC_debug(0, 0, " %02u ", u32_i);
    for(u32_i=1; u32_i<16; u32_i++)
        eMMC_debug(0, 0, "%02u ", u32_i);
    eMMC_debug(0, 0, "\n");

    // ==========================================
    u32_RXDLLResult = 0;

    for(u32_i=0; u32_i<16; u32_i++)
    {
        g_eMMCDrv.u32_DrvFlag |= DRV_FLAG_TUNING_TTABLE; // turn on tunning flag
        if(u8_enable_error_log)// disable tunning error log
            eMMC_DISABLE_LOG(1);

        eMMC_FCIE_SetDelayLine(u32_i);

        if(eMMC_ST_SUCCESS==eMMCTest_BlkWRC_ProbeTiming(eMMC_TEST_BLK_0))
        {
            u32_RXDLLResult |= (1<<u32_i);
        }
        else
        {
            u32_err = eMMC_FCIE_ResetToHS400_5_1((U8)g_eMMCDrv.u16_ClkRegVal);
            if(eMMC_ST_SUCCESS != u32_err)
            {
                g_eMMCDrv.u32_DrvFlag &= ~DRV_FLAG_TUNING_TTABLE; // turn off tunning flag
                eMMC_DISABLE_LOG(0);
                eMMC_debug(0, 1, "eMMC Err: RXDLL: %d fail\n", u32_i);
                break;
            }
        }
    }
    g_eMMCDrv.u32_DrvFlag &= ~DRV_FLAG_TUNING_TTABLE; // turn off tunning flag
    eMMC_DISABLE_LOG(0);

    for(u32_i=0; u32_i<16; u32_i++)
    {
        if((u32_RXDLLResult & (1<<u32_i)) == 0)
            printf(" X ");
        else
            printf(" O ");
    }
    printf("\n");

    return eMMC_ST_SUCCESS;
}

U32 eMMC_FCIE_DetectHS400_5_1_Timing_SKEW1_SKEW2(U8 u8_enable_error_log)
{
    U32 u32_i = 0, u32_err = 0, u32_Skew2Result;
    U8  u8_Skew1;
    #if 1
    eMMC_DISABLE_LOG(1);
    g_eMMCDrv.u16_Reg10_Mode = BIT_SD_DEFAULT_MODE_REG;
    eMMC_PlatformInit();
    u32_err = eMMC_FCIE_Init();
    if(eMMC_ST_SUCCESS != u32_err)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"FCIE Reset Fail!!\n");
        eMMC_FCIE_ErrHandler_Stop();
    }
    u32_err = eMMC_Init_Device_Ex();
    if(eMMC_ST_SUCCESS != u32_err)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Identify Error!!\n");
        eMMC_FCIE_ErrHandler_Stop();
    }
    u32_err = eMMC_FCIE_EnableFastMode(FCIE_eMMC_HS400_5_1);
    if(eMMC_ST_SUCCESS != u32_err)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC 5.1 switch to HS400 fail!!\n");
        eMMC_FCIE_ErrHandler_Stop();
    }
    eMMC_DISABLE_LOG(0);
    #endif

    u32_i = 0;
    eMMC_debug(0, 0, "Skew2           %02u ", u32_i);
    for(u32_i=1; u32_i<9; u32_i++)
        eMMC_debug(0, 0, "%02u ", u32_i);
    eMMC_debug(0, 0, "\n");

    // ==========================================
    for(u8_Skew1=0; u8_Skew1<18; u8_Skew1++)
    {
        // --------------------------------------
        u32_Skew2Result = 0;

        for(u32_i=0; u32_i<9; u32_i++)
        {
            g_eMMCDrv.u32_DrvFlag |= DRV_FLAG_TUNING_TTABLE; // turn on tunning flag
            if(u8_enable_error_log)// disable tunning error log
                eMMC_DISABLE_LOG(1);

            g_eMMCDrv.TimingTable_G_t.RegSet[GT_OFFSET_SKEW2_VALUE].u16_RegValue =u32_i<<4;
            eMMC_FCIE_ApplyTimingSet(0);
            if(u8_Skew1<9)
            {
                REG_FCIE_CLRBIT(reg_emmcpll_0x69, BIT3);// reg_clk_dig_inv
                REG_FCIE_CLRBIT(reg_emmcpll_0x03, BIT_SKEW1_MASK);
                REG_FCIE_SETBIT(reg_emmcpll_0x03, u8_Skew1);
            }
            else
            {
                REG_FCIE_SETBIT(reg_emmcpll_0x69, BIT3);// reg_clk_dig_inv
                REG_FCIE_CLRBIT(reg_emmcpll_0x03, BIT_SKEW1_MASK);
                REG_FCIE_SETBIT(reg_emmcpll_0x03, u8_Skew1-9);
            }

            REG_FCIE_CLRBIT(reg_emmcpll_0x03, BIT_SKEW2_MASK);
            REG_FCIE_SETBIT(reg_emmcpll_0x03, u32_i<<4);

            if(eMMC_ST_SUCCESS==eMMCTest_BlkWRC_ProbeTiming(eMMC_TEST_BLK_0))
            {
                u32_Skew2Result |= (1<<u32_i);
            }
            else
            {
                u32_err = eMMC_FCIE_ResetToHS400_5_1((U8)g_eMMCDrv.u16_ClkRegVal);
                if(eMMC_ST_SUCCESS != u32_err)
                {
                    g_eMMCDrv.u32_DrvFlag &= ~DRV_FLAG_TUNING_TTABLE; // turn off tunning flag
                    eMMC_debug(0, 1, "eMMC Err: SKEW2: %d fail\n", u32_i);
                    break;
                }
            }
        }
        g_eMMCDrv.u32_DrvFlag &= ~DRV_FLAG_TUNING_TTABLE; // turn off tunning flag
        eMMC_DISABLE_LOG(0);

        eMMC_debug(0,0,"Skew1: %02u |  ", u8_Skew1);
        for(u32_i=0; u32_i<9; u32_i++)
        {
            if((u32_Skew2Result & (1<<u32_i)) == 0)
                printf(" X ");
            else
                printf(" O ");
        }
        printf("\n");
    }
    return eMMC_ST_SUCCESS;
}

U32 eMMC_FCIE_DetectHS400_5_1_Timing_TrigerLevel_RX(U8 u8_enable_error_log)
{
    U32 u32_i = 0, u32_err = 0, u32_RXDLLResult;
    U8  TrigerLevel;
    #if 1
    eMMC_DISABLE_LOG(1);
    g_eMMCDrv.u16_Reg10_Mode = BIT_SD_DEFAULT_MODE_REG;
    eMMC_PlatformInit();
    u32_err = eMMC_FCIE_Init();
    if(eMMC_ST_SUCCESS != u32_err)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"FCIE Reset Fail!!\n");
        eMMC_FCIE_ErrHandler_Stop();
    }
    u32_err = eMMC_Init_Device_Ex();
    if(eMMC_ST_SUCCESS != u32_err)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Identify Error!!\n");
        eMMC_FCIE_ErrHandler_Stop();
    }
    u32_err = eMMC_FCIE_EnableFastMode(FCIE_eMMC_HS400_5_1);
    if(eMMC_ST_SUCCESS != u32_err)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC 5.1 switch to HS400 fail!!\n");
        eMMC_FCIE_ErrHandler_Stop();
    }
    eMMC_DISABLE_LOG(0);
    #endif

    u32_i = 0;
    eMMC_debug(0, 0, "RX                 %02u ", u32_i);
    for(u32_i=1; u32_i<16; u32_i++)
        eMMC_debug(0, 0, "%02u ", u32_i);
    eMMC_debug(0, 0, "\n");

    // ==========================================
    for(TrigerLevel=0; TrigerLevel<8; TrigerLevel++)
    {
        if(u8_enable_error_log)// disable tunning error log
            eMMC_DISABLE_LOG(1);

        eMMC_SetTrigerLevel(TrigerLevel);
        // --------------------------------------
        u32_RXDLLResult = 0;

        for(u32_i=0; u32_i<16; u32_i++)
        {
            g_eMMCDrv.u32_DrvFlag |= DRV_FLAG_TUNING_TTABLE; // turn on tunning flag

            eMMC_FCIE_SetDelayLine(u32_i);

            if(eMMC_ST_SUCCESS==eMMCTest_BlkWRC_ProbeTiming(eMMC_TEST_BLK_0))
            {
                u32_RXDLLResult |= (1<<u32_i);
            }
            else
            {
                u32_err = eMMC_FCIE_ResetToHS400_5_1((U8)g_eMMCDrv.u16_ClkRegVal);
                if(eMMC_ST_SUCCESS != u32_err)
                {
                    g_eMMCDrv.u32_DrvFlag &= ~DRV_FLAG_TUNING_TTABLE; // turn off tunning flag
                    eMMC_DISABLE_LOG(0);
                    eMMC_debug(0, 1, "eMMC Err: RXDLL: %d fail\n", u32_i);
                    break;
                }
            }

        }
        g_eMMCDrv.u32_DrvFlag &= ~DRV_FLAG_TUNING_TTABLE; // turn off tunning flag
        eMMC_DISABLE_LOG(0);
        eMMC_debug(0,0,"TrigerLevel: %02u |  ", TrigerLevel);
        for(u32_i=0; u32_i<16; u32_i++)
        {
            if((u32_RXDLLResult & (1<<u32_i)) == 0)
                printf(" X ");
            else
                printf(" O ");
        }
        printf("\n");

        if( TrigerLevel == 0 )
            gu32RXDLLResultLv0 &= u32_RXDLLResult;

        gu32RXDLLResult &= u32_RXDLLResult;

    }
    REG_FCIE_CLRBIT(reg_emmcpll_0x20, Trig_lvl_MASK);

    return eMMC_ST_SUCCESS;
}

#endif

#endif // IF_DETECT_eMMC_DDR_TIMING

void eMMC_Check_SAR5ON(void)
{
    #if defined(ENABLE_EMMC_POWER_SAVING_MODE) && ENABLE_EMMC_POWER_SAVING_MODE

    U16 i, len;
    char *res;

    res = getenv("bootargs");
    if (res)
    {
        len = strlen(res);
        for(i = 0; i < len; i++)
        {
            if ('S' == res[i] && 'A' == res[i+1] &&
                'R' == res[i+2] && '5' == res[i+3] &&
                '=' == res[i+4] && 'O' == res[i+5] &&
                'F' == res[i+6] && 'F' == res[i+7])
                break;
        }

        if (i < len)
        {
            //eMMC_debug(0, 0, "SAR5=OFF\n");
        }
        else
        {
            eMMC_debug(0, 0, "SAR5=ON\n");
            eMMC_Prepare_Power_Saving_Mode_Queue();
        }
    }

    #endif
}


static U32 sgu32_MemGuard0 = 0xA55A;
eMMC_ALIGN0 eMMC_DRIVER g_eMMCDrv eMMC_ALIGN1;
static U32 sgu32_MemGuard1 = 0x1289;

eMMC_ALIGN0 U8 gau8_eMMC_SectorBuf[eMMC_SECTOR_BUF_16KB] eMMC_ALIGN1;
eMMC_ALIGN0 U8 au8_ExtCSD[eMMC_SECTOR_512BYTE] eMMC_ALIGN1;

U32 eMMC_CheckIfMemCorrupt(void)
{
    if(0xA55A != sgu32_MemGuard0 || 0x1289 != sgu32_MemGuard1)
        return eMMC_ST_ERR_MEM_CORRUPT;

    return eMMC_ST_SUCCESS;
}


#if (defined(ENABLE_eMMC_HS200) && ENABLE_eMMC_HS200) || \
    (defined(ENABLE_eMMC_HS400) && ENABLE_eMMC_HS400)

// ======================================================
// 0~7, 8 steps
void eMMC_SetTrigerLevel(U8 u8Level)
{
    const U16 TRIG_LEVEL[8] = {0x0000, 0x0007, 0x0038, 0x003f, 0x01c0, 0x01c7, 0x01f8, 0x01ff};

    if(u8Level>7)
    {
        eMMC_debug(1, 0, "wrong parameter for %s()\n", __func__);
        return;
    }

    eMMC_debug(1, 0, "set trigger level to %d, ", u8Level);
    REG_FCIE_CLRBIT(reg_emmcpll_0x20, Trig_lvl_MASK);
    REG_FCIE_SETBIT(reg_emmcpll_0x20, TRIG_LEVEL[u8Level]);
    eMMC_debug(1, 0, "emmc_pll[0x20] = %04Xh\n\n", REG_FCIE(reg_emmcpll_0x20));
}

// 0~8, 9 steps
void eMMC_SetWritePhase(U8 u8CmdData, U8 u8Phase)
{
    const U8 SKEW1 = 1;
    const U8 SKEW2 = 2; // data DQ
    const U8 SKEW3 = 3; // CMD
    const U8 SKEW4 = 4;

    if( (u8Phase>8)  )
    {
        eMMC_debug(1, 0, "wrong parameter for %s() %d %d\n", __func__, u8CmdData, u8Phase);
    }

    if(u8CmdData==SKEW3)
    {
        eMMC_debug(1, 0, "\nset skew3 cmd phase to %d, ", u8Phase);
        REG_FCIE_CLRBIT(reg_emmcpll_0x03, BIT11|BIT10|BIT9|BIT8);
        REG_FCIE_SETBIT(reg_emmcpll_0x03, u8Phase<<8);
        eMMC_debug(1, 0, "emmc_pll[0x03] = %04Xh\n", REG_FCIE(reg_emmcpll_0x03));
    }
    else if(u8CmdData==SKEW2)
    {
        eMMC_debug(1, 0, "\nset skew2 data phase to %d, ", u8Phase);
        REG_FCIE_CLRBIT(reg_emmcpll_0x03, BIT7|BIT6|BIT5|BIT4);
        REG_FCIE_SETBIT(reg_emmcpll_0x03, u8Phase<<4);
        eMMC_debug(1, 0, "emmc_pll[0x03] = %04Xh\n", REG_FCIE(reg_emmcpll_0x03));
    }
    else if(u8CmdData==SKEW1)
    {
        eMMC_debug(1, 0, "\nset skew1 phase to %d, ", u8Phase);
        REG_FCIE_CLRBIT(reg_emmcpll_0x03, BIT3|BIT2|BIT1|BIT0);
        REG_FCIE_SETBIT(reg_emmcpll_0x03, u8Phase);
        eMMC_debug(1, 0, "emmc_pll[0x03] = %04Xh\n", REG_FCIE(reg_emmcpll_0x03));
    }
    else if(u8CmdData==SKEW4)
    {
        eMMC_debug(1, 0, "\nset skew4 phase to %d, ", u8Phase);
        REG_FCIE_CLRBIT(reg_emmcpll_0x03, BIT15|BIT14|BIT13|BIT12);
        if(u8Phase < PLL_SKEW4_CNT)
        {
            REG_FCIE_CLRBIT(reg_emmcpll_0x02, BIT14);
            REG_FCIE_CLRBIT(reg_emmcpll_0x6c, BIT7);
            REG_FCIE_SETBIT(reg_emmcpll_0x03, u8Phase<<12);
        }
        else
        {
            REG_FCIE_SETBIT(reg_emmcpll_0x02, BIT14);
            REG_FCIE_SETBIT(reg_emmcpll_0x6c, BIT7);
            REG_FCIE_SETBIT(reg_emmcpll_0x03, (u8Phase-PLL_SKEW4_CNT)<<12);
        }

        eMMC_debug(1, 0, "emmc_pll[0x02] = %04Xh\n", REG_FCIE(reg_emmcpll_0x02));
        eMMC_debug(1, 0, "emmc_pll[0x03] = %04Xh\n", REG_FCIE(reg_emmcpll_0x03));
    }
}

#endif

#if defined(ENABLE_eMMC_INTERRUPT_MODE) && ENABLE_eMMC_INTERRUPT_MODE
//static DECLARE_WAIT_QUEUE_HEAD(fcie_wait);
#if defined(MSOS_TYPE_ECOS)
static U16 sgu16_CurNCMIEEvent = 0;      // Used to store current IRQ state
#endif

static void eMMC_FCIE_IRQ(void)
{
    #if defined(MSOS_TYPE_ECOS)
    sgu16_CurNCMIEEvent |= REG_FCIE(FCIE_MIE_EVENT);
    REG_FCIE_CLRBIT(FCIE_MIE_INT_EN, sgu16_CurNCMIEEvent);

    MsOS_SetEvent(gs32_eMMCIntEvent, sgu16_CurNCMIEEvent);
    MsOS_EnableInterrupt(E_INT_IRQ_NFIE);
    #endif
}

U32 eMMC_WaitCompleteIntr(U32 u32_RegAddr, U16 u16_WaitEvent, U32 u32_MicroSec)
{
    U32 u32_i, u32_err, u32_event;

    u32_MicroSec = u32_MicroSec/100 +1; // unit: 100 us

    #if defined(MSOS_TYPE_ECOS)
    if(0==MsOS_WaitEvent(gs32_eMMCIntEvent, u16_WaitEvent, (MS_U32*)&u32_event,
        E_AND_CLEAR, u32_MicroSec/10+1))
    {
        u32_i = u32_MicroSec; // timeout
        eMMC_debug(0,0,"Evt:%X, %X %X\n", sgu16_CurNCMIEEvent, REG_FCIE(FCIE_JOB_BL_CNT), REG_FCIE(FCIE_TR_BK_CNT));
    }
    #else
    for(u32_i=0; u32_i<u32_MicroSec; u32_i++)
    {
        if((sgu16_CurNCMIEEvent & u16_WaitEvent) == u16_WaitEvent)
            break;

        eMMC_hw_timer_delay(HW_TIMER_DELAY_100us);
    }
    #endif

    if(u32_i == u32_MicroSec)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,
            "eMMC Warn: int timeout, reg.0:%Xh reg.1:%Xh  St:%Xh\n",
            REG_FCIE(FCIE_MIE_EVENT), REG_FCIE(FCIE_MIE_INT_EN), sgu16_CurNCMIEEvent);

        u32_err = eMMC_FCIE_PollingEvents(u32_RegAddr, u16_WaitEvent, u32_MicroSec*100);
        if(eMMC_ST_SUCCESS == u32_err)
            eMMC_debug(0,1,"eMMC: but polling ok: %Xh\n", REG_FCIE(u32_RegAddr));
        else
        {
            eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: events lose, WaitEvent: %Xh \n", u16_WaitEvent);
            eMMC_DumpDriverStatus();  eMMC_DumpPadClk();
            eMMC_FCIE_DumpRegisters();eMMC_FCIE_DumpDebugBus();
            return eMMC_ST_ERR_INT_TO;
        }
    }

    REG_FCIE_W(FCIE_MIE_EVENT, sgu16_CurNCMIEEvent);
    sgu16_CurNCMIEEvent = 0;

    return eMMC_ST_SUCCESS;
}

#if defined(MSOS_TYPE_ECOS)
S32 gs32_eMMCIntEvent=0;
#endif

void eMMC_enable_intr_mode(void)
{
    #if defined(MSOS_TYPE_ECOS)
    if(0==gs32_eMMCIntEvent)
        gs32_eMMCIntEvent = MsOS_CreateEventGroup("eMMC_EVENT");

    // HW ISR initial for ecos system
    MsOS_AttachInterrupt(E_INT_IRQ_NFIE, (InterruptCb)eMMC_FCIE_IRQ);
    MsOS_EnableInterrupt(E_INT_IRQ_NFIE);
    #endif
}
#endif


void eMMC_LockFCIE(U8 *pu8_str)
{
    //eMMC_debug(0,1,"%s 1\n", pu8_str);
    #if defined(MSOS_TYPE_ECOS)
    if(-1 == _s32UnfdTaskMutexId)
        _s32UnfdTaskMutexId = MsOS_CreateMutex(E_MSOS_FIFO, "UnfdTaskMutex", MSOS_PROCESS_PRIVATE);
    MsOS_ObtainMutex(_s32UnfdTaskMutexId, MSOS_WAIT_FOREVER);
    #endif
    //eMMC_debug(0,1,"%s 2\n", pu8_str);

    #if 0 // for SD Card enabled
    #if IF_FCIE_SHARE_IP && defined(CONFIG_MSTAR_SDMMC)
    eMMC_clock_setting(g_eMMCDrv.u16_ClkRegVal);
    eMMC_pads_switch(g_eMMCDrv.u8_PadType);
    #endif
    #endif

    REG_FCIE_SETBIT(FCIE_SD_MODE, BIT_SD_CLK_EN); // output clock

    #ifdef IP_FCIE_VERSION_5
        REG_FCIE_SETBIT(FCIE_MIE_FUNC_CTL, BIT_EMMC_ACTIVE);
    #else
        REG_FCIE_SETBIT(FCIE_REG16h, BIT_EMMC_ACTIVE);
    #endif

}


void eMMC_UnlockFCIE(U8 *pu8_str)
{
    //eMMC_debug(0,1,"%s 1\n", pu8_str);
    REG_FCIE_CLRBIT(FCIE_SD_MODE, BIT_SD_CLK_EN); // not output clock

    #ifdef IP_FCIE_VERSION_5
        REG_FCIE_CLRBIT(FCIE_MIE_FUNC_CTL, BIT_EMMC_ACTIVE);
    #else
        REG_FCIE_CLRBIT(FCIE_REG16h, BIT_EMMC_ACTIVE);
    #endif

    #if defined(MSOS_TYPE_ECOS)
    MsOS_ReleaseMutex(_s32UnfdTaskMutexId);
    #endif
    //eMMC_debug(0,1,"%s 2\n", pu8_str);
}

#endif
