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
#include "../../inc/common/eMMC.h"
#if defined(UNIFIED_eMMC_DRIVER) && UNIFIED_eMMC_DRIVER

//=============================================================
U32 eMMC_hw_timer_delay(U32 u32us)
{
	#if 0
    int i = 0;

	for (i = 0; i < (u32us<<2); i++)
	{
		#if 1
		 int j = 0, tmp;
		for (j = 0; j < 0x2; j++)
		{
			tmp = j;
		}
		#endif
	}
	#else
    //udelay((u32us>>1)+(u32us>>4)+1);
    //udelay(u32us * 24);
    U32 u32HWTimer = 0;
	volatile U16 u16TimerLow = 0;
	volatile U16 u16TimerHigh = 0;

	// reset HW timer
	REG_FCIE_W(TIMER0_MAX_LOW, 0xFFFF);
	REG_FCIE_W(TIMER0_MAX_HIGH, 0xFFFF);
	REG_FCIE_W(TIMER0_ENABLE, 0);

	// start HW timer
	REG_FCIE_SETBIT(TIMER0_ENABLE, 0x0001);

	while( u32HWTimer < 12*u32us ) // wait for u32usTick micro seconds
	{
		REG_FCIE_R(TIMER0_CAP_LOW, u16TimerLow);
		REG_FCIE_R(TIMER0_CAP_HIGH, u16TimerHigh);

		u32HWTimer = (u16TimerHigh<<16) | u16TimerLow;
	}

	REG_FCIE_W(TIMER0_ENABLE, 0);
	#endif
	return u32us;
}


// use to performance test
U32 eMMC_hw_timer_start(void)
{
	return 0;
}

U32 eMMC_hw_timer_tick(void)
{
	// TIMER_FREERUN_32K  32 KHz
	// TIMER_FREERUN_XTAL 38.4 MHz,
	// counting down
	return 0;//HalTimerRead(TIMER_FREERUN_32K);
}

void *eMMC_memset (void *str, int c, unsigned int len)
{
	register char *st = str;

	while (len-- > 0)
		*st++ = c;
	return str;
}

void *eMMC_memcpy (void *destaddr, void const *srcaddr, unsigned int len)
{
	char *dest = destaddr;
	char const *src = srcaddr;

	while (len-- > 0)
		*dest++ = *src++;
	return destaddr;
}

#if defined(ENABLE_eMMC_HS400) && ENABLE_eMMC_HS400

int eMMC_memcmp(const void *cs,const void *ct,unsigned int count)
{
	const unsigned char *su1, *su2;
	int res = 0;

	for( su1 = cs, su2 = ct; 0 < count; ++su1, ++su2, count--)
		if ((res = *su1 - *su2) != 0)
			break;
	return res;
}

#endif

void *eMMC_get_RPMBContext_address(void)
{
	return (void*)(CONFIG_BOOTRAM_LOADADDR+0x20000);
}


#if (defined(eMMC_DRV_EAGLE_BOOTRAM)&&eMMC_DRV_EAGLE_BOOTRAM)
void eMMC_DumpPadClk(void)
{
	//----------------------------------------------
	eMMC_debug(0, 1, "\n[clk setting]: %uKHz \n", g_eMMCDrv.u32_ClkKHz);
	eMMC_debug(0, 1, "FCIE 1X (0x%X):0x%X\n", reg_ckg_fcie_1X, REG_FCIE_U16(reg_ckg_fcie_1X));
	eMMC_debug(0, 1, "FCIE 4X (0x%X):0x%X\n", reg_ckg_fcie_4X, REG_FCIE_U16(reg_ckg_fcie_4X));
	eMMC_debug(0, 1, "CMU (0x%X):0x%X\n", reg_ckg_CMU, REG_FCIE_U16(reg_ckg_CMU));

	//----------------------------------------------
	eMMC_debug(0, 1, "\n[pad setting]: ");
	switch(g_eMMCDrv.u32_Flag & FCIE_FLAG_PADTYPE_MASK)
	{
	case FCIE_FLAG_PADTYPE_DDR:
		eMMC_debug(0,0,"DDR\n");  break;
	case FCIE_FLAG_PADTYPE_SDR:
		eMMC_debug(0,0,"SDR\n");  break;
	case FCIE_FLAG_PADTYPE_BYPASS:
		eMMC_debug(0,0,"BYPASS\n");  break;
	default:
		eMMC_debug(0,0,"eMMC Err: Pad unknown\n");  eMMC_die("\n");
	}
	eMMC_debug(0, 1, "chiptop_0x5A (0x%X):0x%X\n", reg_chiptop_0x5A, REG_FCIE_U16(reg_chiptop_0x5A));
	eMMC_debug(0, 1, "chiptop_0x1F (0x%X):0x%X\n", reg_chiptop_0x1F, REG_FCIE_U16(reg_chiptop_0x1F));
	eMMC_debug(0, 1, "chiptop_0x70 (0x%X):0x%X\n", reg_chiptop_0x70, REG_FCIE_U16(reg_chiptop_0x70));
	eMMC_debug(0, 1, "chiptop_0x64 (0x%X):0x%X\n", reg_chiptop_0x64, REG_FCIE_U16(reg_chiptop_0x64));
	eMMC_debug(0, 1, "chiptop_0x4F (0x%X):0x%X\n", reg_chiptop_0x4F, REG_FCIE_U16(reg_chiptop_0x4F));
	eMMC_debug(0, 1, "chiptop_0x03 (0x%X):0x%X\n", reg_chiptop_0x03, REG_FCIE_U16(reg_chiptop_0x03));
	eMMC_debug(0, 1, "chiptop_0x51 (0x%X):0x%X\n", reg_chiptop_0x51, REG_FCIE_U16(reg_chiptop_0x51));
	eMMC_debug(0, 1, "chiptop_0x6F (0x%X):0x%X\n\n", reg_chiptop_0x6F, REG_FCIE_U16(reg_chiptop_0x6F));

	eMMC_debug(0, 1, "chiptop_0x43 (0x%X):0x%X\n", reg_chiptop_0x43, REG_FCIE_U16(reg_chiptop_0x43));
	eMMC_debug(0, 1, "chiptop_0x50 (0x%X):0x%X\n", reg_chiptop_0x50, REG_FCIE_U16(reg_chiptop_0x50));
	eMMC_debug(0, 1, "chiptop_0x0B (0x%X):0x%X\n", reg_chiptop_0x0B, REG_FCIE_U16(reg_chiptop_0x0B));
	eMMC_debug(0, 1, "chiptop_0x5D (0x%X):0x%X\n", reg_chiptop_0x5D, REG_FCIE_U16(reg_chiptop_0x5D));

	eMMC_debug(0, 1, "\n");
}

U32 eMMC_pads_switch(U32 u32_FCIE_IF_Type)
{
	g_eMMCDrv.u32_Flag &= ~FCIE_FLAG_PADTYPE_MASK;

	REG_FCIE_SETBIT(FCIE_BOOT_CONFIG, BIT1);

	REG_FCIE_CLRBIT(FCIE_BOOT_CONFIG,
			    BIT_MACRO_EN|BIT_SD_DDR_EN|BIT_SD_BYPASS_MODE_EN|BIT_SD_SDR_IN_BYPASS|BIT_SD_FROM_TMUX);

	switch(u32_FCIE_IF_Type){
		case FCIE_eMMC_DDR:
			eMMC_debug(eMMC_DEBUG_LEVEL_HIGH,1,"eMMC pads: DDR\n");

			REG_FCIE_SETBIT(reg_chiptop_0x43,
				BIT_PAD_EMMC_CLK_SRC|BIT_eMMC_RSTPIN_VAL);
			REG_FCIE_CLRBIT(reg_chiptop_0x43,
				BIT_EMPTY|BIT_eMMC_RSTPIN_EN|BIT_PAD_BYPASS_MACRO);

			// set DDR mode
			REG_FCIE_SETBIT(FCIE_BOOT_CONFIG, BIT_MACRO_EN|BIT_SD_DDR_EN);
			// set chiptop
			g_eMMCDrv.u32_Flag |= FCIE_FLAG_PADTYPE_DDR;
			break;

		case FCIE_eMMC_SDR:
            eMMC_debug(eMMC_DEBUG_LEVEL_HIGH,1,"eMMC pads: SDR\n");

			REG_FCIE_SETBIT(reg_chiptop_0x43,
				BIT_PAD_EMMC_CLK_SRC|BIT_eMMC_RSTPIN_VAL);
			REG_FCIE_CLRBIT(reg_chiptop_0x43,
				BIT_EMPTY|BIT_eMMC_RSTPIN_EN|BIT_PAD_BYPASS_MACRO);

			// set SDR mode
			REG_FCIE_SETBIT(FCIE_BOOT_CONFIG, BIT_MACRO_EN);
			g_eMMCDrv.u32_Flag |= FCIE_FLAG_PADTYPE_SDR;
			break;

		case FCIE_eMMC_BYPASS:
			eMMC_debug(eMMC_DEBUG_LEVEL_HIGH,1,"eMMC pads: BYPASS\n");

			REG_FCIE_SETBIT(reg_chiptop_0x43,
				BIT_PAD_EMMC_CLK_SRC|BIT_eMMC_RSTPIN_VAL|BIT_PAD_BYPASS_MACRO);
			REG_FCIE_CLRBIT(reg_chiptop_0x43,
				BIT_EMPTY|BIT_eMMC_RSTPIN_EN);

			// set bypadd mode
			REG_FCIE_SETBIT(FCIE_BOOT_CONFIG,
			    BIT_MACRO_EN|BIT_SD_BYPASS_MODE_EN|BIT_SD_SDR_IN_BYPASS);
			g_eMMCDrv.u32_Flag |= FCIE_FLAG_PADTYPE_BYPASS;
			break;

		default:
			eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: unknown interface: %X\n",u32_FCIE_IF_Type);
			return eMMC_ST_ERR_INVALID_PARAM;
	}

	// set chiptop
	REG_FCIE_CLRBIT(reg_chiptop_0x5A, BIT0|BIT1|BIT2);
	REG_FCIE_CLRBIT(reg_chiptop_0x1F, BIT0|BIT1);
	REG_FCIE_CLRBIT(reg_chiptop_0x70, BIT2);
	REG_FCIE_CLRBIT(reg_chiptop_0x64, BIT0);
	REG_FCIE_CLRBIT(reg_chiptop_0x4F, BIT13|BIT14);
	REG_FCIE_CLRBIT(reg_chiptop_0x03, BIT6|BIT7);
	REG_FCIE_CLRBIT(reg_chiptop_0x51, BIT10|BIT11);
	REG_FCIE_CLRBIT(reg_chiptop_0x6F, BIT14|BIT15);

	REG_FCIE_CLRBIT(reg_chiptop_0x50, BIT_NAND_CS1_EN|BIT_ALL_PAD_IN);
	REG_FCIE_CLRBIT(reg_chiptop_0x0B, BIT_NAND_CS2_EN|BIT_NAND_CS3_EN|
		BIT_SD0_CFG|BIT_SD0_CFG2|BIT_SD1_CFG|BIT_SD1_CFG2|BIT_NAND_CFG);


	#if (MS_BOARD_TYPE_SEL == BD_MST142A_D01A_S_EAGLE) || \
        (MS_BOARD_TYPE_SEL == BD_MST028B_10AFX_EAGLE)
	    //defined(CONFIG_MSTAR_BD_MST028B_10AFX_EAGLE)
    // eMMC Mode1: 142A, 028B
    REG_FCIE_CLRBIT(reg_chiptop_0x5D, BIT_eMMC_CFG_MASK);
    REG_FCIE_SETBIT(reg_chiptop_0x5D, BIT_eMMC_CFG_MODE1);
	#else
    // eMMC Mode2: 152A ,147A, 038B
	REG_FCIE_SETBIT(reg_chiptop_0x5D, BIT_eMMC_CFG_MODE2);
    #endif

	REG_FCIE_SETBIT(FCIE_REG_2Dh, BIT_NEW_DESIGN_ENn);

	return eMMC_ST_SUCCESS;
}


U32 eMMC_clock_setting(U32 u32ClkParam)
{
	eMMC_PlatformResetPre();
    REG_FCIE_CLRBIT(FCIE_SD_MODE, BIT_SD_CLK_EN);

	switch(u32ClkParam)	{
	case BIT_FCIE_CLK_300K:  g_eMMCDrv.u32_ClkKHz = 300;    break;
	case BIT_FCIE_CLK_20M:   g_eMMCDrv.u32_ClkKHz = 20000;  break;
    case BIT_FCIE_CLK_27M: g_eMMCDrv.u32_ClkKHz = 27000;  break;
	case BIT_FCIE_CLK_32M:   g_eMMCDrv.u32_ClkKHz = 32000; break;
    case BIT_FCIE_CLK_36M: g_eMMCDrv.u32_ClkKHz = 36000;  break;
	case BIT_FCIE_CLK_40M:   g_eMMCDrv.u32_ClkKHz = 40000; break;
    case BIT_FCIE_CLK_43_2M: g_eMMCDrv.u32_ClkKHz = 43200; break;
    case BIT_FCIE_CLK_48M:   g_eMMCDrv.u32_ClkKHz = 48000; break;
	default:
		eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: %Xh\n", eMMC_ST_ERR_INVALID_PARAM);
		return eMMC_ST_ERR_INVALID_PARAM;
	}

	REG_FCIE_CLRBIT(reg_ckg_fcie_1X, BIT_FCIE_CLK_Gate|BIT_FCIE_CLK_MASK);
	REG_FCIE_SETBIT(reg_ckg_fcie_1X, BIT_FCIE_CLK_SEL|(u32ClkParam<<BIT_FCIE_CLK_SHIFT));

	eMMC_debug(eMMC_DEBUG_LEVEL_LOW, 1, "clk:%uKHz, Param:%Xh, fcie_1X(%Xh):%Xh, fcie_4X(%Xh):%Xh\n",
		g_eMMCDrv.u32_ClkKHz, u32ClkParam,
		reg_ckg_fcie_1X, REG_FCIE_U16(reg_ckg_fcie_1X),
		reg_ckg_fcie_4X, REG_FCIE_U16(reg_ckg_fcie_4X));

    eMMC_PlatformResetPost();
	return eMMC_ST_SUCCESS;
}

#if 0

U32 eMMC_clock_gating(void)
{
	eMMC_PlatformResetPre();
	g_eMMCDrv.u32_ClkKHz = 0;
	REG_FCIE_W(reg_ckg_fcie_1X, BIT_FCIE_CLK_Gate);
	REG_FCIE_W(reg_ckg_fcie_4X, BIT_FCIE_CLK4X_Gate);
	REG_FCIE_CLRBIT(FCIE_SD_MODE, BIT_SD_CLK_EN);
	eMMC_PlatformResetPost();
	return eMMC_ST_SUCCESS;
}

#endif

U8 gau8_FCIEClkSel[eMMC_FCIE_VALID_CLK_CNT]={
	BIT_FCIE_CLK4X_48M,
	BIT_FCIE_CLK4X_40M,
	BIT_FCIE_CLK4X_36M,
	BIT_FCIE_CLK4X_27M,
	BIT_FCIE_CLK4X_20M,
};

#if defined(IF_DETECT_eMMC_DDR_TIMING) && IF_DETECT_eMMC_DDR_TIMING
static char *sgachar_string[]={"  2T","1.5T","2.5T","  0T"};

static U32 eMMC_FCIE_DetectDDRTiming_Ex(U8 u8_DQS, U8 u8_DelaySel)
{
	U32 u32_SectorAddr;
	eMMC_PNI_t *pPartInfo = (eMMC_PNI_t*)gau8_eMMC_PartInfoBuf;

	if(g_eMMCDrv.u32_Flag & FCIE_FLAG_GET_PART_INFO)
	    u32_SectorAddr = pPartInfo->u16_BlkPageCnt *
		             pPartInfo->u16_PageByteCnt *
		             eMMC_PBA2_DETECT_DDRT;
	else
		u32_SectorAddr = eMMC_DEFAULT_BLK_DETECT_DDRT;

	//eMMC_debug(0,1,"SectorAddr: %Xh\n", u32_SectorAddr);

	eMMC_debug(eMMC_DEBUG_LEVEL_HIGH,0,"\ndqs:%s[%Xh]  cell:%02Xh \n",
		sgachar_string[u8_DQS], u8_DQS, u8_DelaySel);

	eMMC_FCIE_SetDDRTimingReg(u8_DQS, u8_DelaySel);
    return eMMCTest_BlkWRC_ProbeDDR(u32_SectorAddr);
}

#define FCIE_DELAY_CELL_ts         700 // 0.7ns
static eMMC_FCIE_DDRT_WINDOW_t sg_DDRTWindow[2];

U32 eMMC_FCIE_DetectDDRTiming(void)
{
	U8  u8_dqs, u8_delay_sel, u8_i;
	U8  u8_dqs_prev=0xFF, u8_delay_sel_prev;
	U8  au8_DQSRegVal[4]={3,1,0,2}; // 0T, 1.5T, 2T, 2.5T
	U8  au8_DQS_10T[4]={0,15,20,25}; // 0T, 1.5T, 2T, 2.5T
	U8  u8_delay_Sel_max;
	U32 u32_ts;
	U32 u32_err;
	eMMC_FCIE_DDRT_WINDOW_t *pWindow = &sg_DDRTWindow[0];

	if(0 == (g_eMMCDrv.u32_Flag&FCIE_FLAG_DDR_MODE)){
		//eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC WARN: SDR mode can't detect DDR timing\n");
		u32_err = eMMC_FCIE_EnableDDRMode();
		if(eMMC_ST_SUCCESS != u32_err)
		{
		    printf("eMMC Error: set DDR IF fail: %X\n", u32_err);
		    return u32_err;
		}
	}
	g_eMMCDrv.u32_Flag |= FCIE_FLAG_DDR_TUNING; // just to turn off some log

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
			eMMC_debug(eMMC_DEBUG_LEVEL_HIGH,0,"max delay cell: %u\n", u8_delay_Sel_max);
			eMMC_debug(eMMC_DEBUG_LEVEL_HIGH,0,"invalid, not try\n\n");
			continue;
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

	g_eMMCDrv.u32_Flag &= ~FCIE_FLAG_DDR_TUNING;

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

}


U32 eMMC_FCIE_BuildDDRTimingTable(void)
{
	U8 au8_DQSRegVal[4]={3,1,0,2}; // 0T, 1.5T, 2T, 2.5T
	U8  u8_i, u8_ClkIdx, u8_SetIdx, u8_tmp, u8_DqsIdx, u8_CellBase;
	U32 u32_err, u32_ret=eMMC_ST_ERR_NO_OK_DDR_PARAM;
	eMMC_FCIE_DDRT_WINDOW_t *pWindow;

	memset((void*)&g_eMMCDrv.DDRTable, '\0', sizeof(g_eMMCDrv.DDRTable));
	u8_SetIdx = 0;

	for(u8_ClkIdx=0; u8_ClkIdx<eMMC_FCIE_VALID_CLK_CNT; u8_ClkIdx++)
	{

		eMMC_debug(eMMC_DEBUG_LEVEL,0,"=================================\n");
		eMMC_clock_setting(gau8_FCIEClkSel[u8_ClkIdx]);
		eMMC_debug(eMMC_DEBUG_LEVEL,0,"=================================\n");

		// ---------------------------
		// search and set the Windows
		u32_err = eMMC_FCIE_DetectDDRTiming();

		// ---------------------------
		// set the Table
		if(eMMC_ST_SUCCESS == u32_err)
		{
			#if 0
			// before 12MHz, should at least 2 clk is ok for DDR
			if(eMMC_FCIE_DDRT_SET_CNT-1 == u8_SetIdx &&
				BIT_FCIE_CLK_12M == gau8_FCIEClkSel[u8_ClkIdx])
				u32_ret = eMMC_ST_SUCCESS;
			#endif

			g_eMMCDrv.DDRTable.Set[u8_SetIdx].u8_Clk = gau8_FCIEClkSel[u8_ClkIdx];

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
					if(pWindow->au8_DQSValidCellCnt[u8_DqsIdx-1] >= 3 &&
						pWindow->au8_DQSValidCellCnt[u8_DqsIdx] >= 3)
					{
						g_eMMCDrv.DDRTable.Set[u8_SetIdx].Param.u8_DQS = au8_DQSRegVal[u8_DqsIdx];
						g_eMMCDrv.DDRTable.Set[u8_SetIdx].Param.u8_Cell = 0; // nice
					}
					else
					{
						u8_tmp = (pWindow->au8_DQSValidCellCnt[u8_DqsIdx] +
							pWindow->au8_DQSValidCellCnt[u8_DqsIdx-1]) / 2;


						if(u8_tmp < pWindow->au8_DQSValidCellCnt[u8_DqsIdx-1])
						{
							g_eMMCDrv.DDRTable.Set[u8_SetIdx].Param.u8_DQS =
								au8_DQSRegVal[u8_DqsIdx-1];

							u8_CellBase = pWindow->au8_DQSTryCellCnt[u8_DqsIdx-1] - pWindow->au8_DQSValidCellCnt[u8_DqsIdx-1];
							g_eMMCDrv.DDRTable.Set[u8_SetIdx].Param.u8_Cell =
								u8_CellBase + pWindow->au8_DQSValidCellCnt[u8_DqsIdx-1] +
								pWindow->au8_DQSValidCellCnt[u8_DqsIdx] - u8_tmp;
						}
						else
						{   g_eMMCDrv.DDRTable.Set[u8_SetIdx].Param.u8_DQS =
								au8_DQSRegVal[u8_DqsIdx];
						    g_eMMCDrv.DDRTable.Set[u8_SetIdx].Param.u8_Cell =
								(pWindow->au8_DQSValidCellCnt[u8_DqsIdx-1] +
								pWindow->au8_DQSValidCellCnt[u8_DqsIdx]) / 2;
						}
					}
				}
			}

			// ---------------------------
            // or, pick up the Window of large PASS Cnt
			else //if(NULL == pWindow)
			{
			    pWindow =
				    (sg_DDRTWindow[0].u8_Cnt > sg_DDRTWindow[1].u8_Cnt) ?
				    &sg_DDRTWindow[0] : &sg_DDRTWindow[1];

				g_eMMCDrv.DDRTable.Set[u8_SetIdx].Param.u8_DQS = au8_DQSRegVal[pWindow->aParam[0].u8_DQS];
				g_eMMCDrv.DDRTable.Set[u8_SetIdx].Param.u8_Cell =
					(pWindow->aParam[0].u8_Cell + pWindow->aParam[1].u8_Cell)/2;
			}

			#if 0
			// ---------------------------
			// use 12M for Set.Min
			if(eMMC_FCIE_DDRT_SET_CNT-2 == u8_SetIdx)
				u8_ClkIdx = eMMC_FCIE_VALID_CLK_CNT-2;
			#else
			if(FCIE_SLOW_CLK == g_eMMCDrv.DDRTable.Set[u8_SetIdx].u8_Clk)
				g_eMMCDrv.DDRTable.u8_SetCnt = u8_SetIdx + 1;
			#endif

			u8_SetIdx++;
		}
	}

	// dump DDRT Set
	u8_tmp = eMMC_FCIE_VALID_CLK_CNT > eMMC_FCIE_DDRT_SET_CNT ?
	    eMMC_FCIE_DDRT_SET_CNT : eMMC_FCIE_VALID_CLK_CNT;
	u8_tmp = u8_tmp > g_eMMCDrv.DDRTable.u8_SetCnt ?
		g_eMMCDrv.DDRTable.u8_SetCnt : u8_tmp;

	eMMC_DumpDDRTTable();

	// ======================================================
	// CAUTION: expect 48MHz can have valid DDRT parameter
	if(g_eMMCDrv.DDRTable.Set[eMMC_DDRT_SET_MAX].u8_Clk == gau8_FCIEClkSel[0]
		&& 0 != g_eMMCDrv.DDRTable.u8_SetCnt)
		u32_ret = eMMC_ST_SUCCESS;
	// ======================================================

	// ---------------------------
	// save DDRT Table
	if(eMMC_ST_SUCCESS == u32_ret)
	{

		g_eMMCDrv.DDRTable.u32_ChkSum =
		    eMMC_ChkSum((U8*)&g_eMMCDrv.DDRTable, sizeof(g_eMMCDrv.DDRTable)-4);
		memcpy(gau8_eMMC_SectorBuf, (U8*)&g_eMMCDrv.DDRTable, sizeof(g_eMMCDrv.DDRTable));

		eMMC_FCIE_ApplyDDRTSet(eMMC_DDRT_SET_MAX);

		u32_err = eMMC_CMD24(eMMC_DDRTABLE_BLK_0, gau8_eMMC_SectorBuf);
	    u32_ret = eMMC_CMD24(eMMC_DDRTABLE_BLK_1, gau8_eMMC_SectorBuf);
	    if(eMMC_ST_SUCCESS!=u32_err && eMMC_ST_SUCCESS!=u32_ret)
	    {
		    eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC ERROR: %Xh %Xh\n",
			    u32_err, u32_ret);
		    return eMMC_ST_ERR_SAVE_DDRT_FAIL;
	    }
	}
	else
	{
		*(U32*)&gau8_eMMC_SectorBuf[0] = ~
		    eMMC_ChkSum((U8*)&g_eMMCDrv.DDRTable, sizeof(g_eMMCDrv.DDRTable));

		eMMC_FCIE_EnableSDRMode();
	}

	return u32_ret;
}

#endif // IF_DETECT_eMMC_DDR_TIMING


void eMMC_set_WatchDog(U8 u8_IfEnable)
{
	u8_IfEnable = u8_IfEnable;
	// do nothing
}

void eMMC_reset_WatchDog(void)
{
	// do nothing
}

#define MSTAR_MIU0_BUS_BASE                      0x40000000
#define MSTAR_MIU1_BUS_BASE                      0xA0000000
U32 eMMC_translate_DMA_address_Ex(U32 u32_DMAAddr, U32 u32_ByteCnt)
{
	u32_ByteCnt = u32_ByteCnt;
	#if 0
	flush_cache(u32_DMAAddr, u32_ByteCnt);
	#endif
	if(u32_DMAAddr < MSTAR_MIU1_BUS_BASE) // MIU0
	{
		REG_FCIE_CLRBIT(FCIE_MIU_DMA_26_16, BIT_MIU1_SELECT);
	}
	else
	{	REG_FCIE_SETBIT(FCIE_MIU_DMA_26_16, BIT_MIU1_SELECT);
		u32_DMAAddr -= MSTAR_MIU1_BUS_BASE;
	}
	return u32_DMAAddr;
}

void eMMC_Invalidate_data_cache_buffer(U32 u32_addr, S32 s32_size)
{
	u32_addr = u32_addr;
	s32_size = s32_size;
//	flush_cache(u32_addr, s32_size);
}

void eMMC_flush_miu_pipe(void)
{

}

U32 eMMC_PlatformResetPre(void)
{

	return eMMC_ST_SUCCESS;
}

U32 eMMC_PlatformResetPost(void)
{

	return eMMC_ST_SUCCESS;
}

U32 eMMC_PlatformInit(void)
{
	eMMC_pads_switch(FCIE_eMMC_SDR);
	eMMC_clock_setting(FCIE_SLOWEST_CLK);

	return eMMC_ST_SUCCESS;
}

static U32 sgu32_MemGuard0 = 0xA55A;
eMMC_ALIGN0 eMMC_DRIVER g_eMMCDrv eMMC_ALIGN1;
static U32 sgu32_MemGuard1 = 0x1289;

eMMC_ALIGN0 U8 gau8_eMMC_SectorBuf[eMMC_SECTOR_BUF_16KB] eMMC_ALIGN1; // 512 bytes
eMMC_ALIGN0 U8 gau8_eMMC_PartInfoBuf[eMMC_SECTOR_512BYTE] eMMC_ALIGN1; // 512 bytes

U32 eMMC_CheckIfMemCorrupt(void)
{
	if(0xA55A != sgu32_MemGuard0 || 0x1289 != sgu32_MemGuard1)
		return eMMC_ST_ERR_MEM_CORRUPT;

	return eMMC_ST_SUCCESS;
}
#elif (defined(eMMC_DRV_EDISON_BOOTRAM)&&eMMC_DRV_EDISON_BOOTRAM)


U16 u16_reg_chiptop_0x10_val =0,u16_reg_chiptop_0x6E_val=0;

void eMMC_DumpPadClk(void)
{

}

U32 eMMC_clock_setting(U32 u32ClkParam)
{
	eMMC_PlatformResetPre();
    REG_FCIE_CLRBIT(FCIE_SD_MODE, BIT_SD_CLK_EN);

	REG_FCIE_CLRBIT(reg_ckg_fcie_1X, BIT_FCIE_CLK_Gate|BIT_FCIE_CLK_MASK|BIT_FCIE_CLK_SEL);
	if(u32ClkParam==BIT_FCIE_CLK_300K)
	{
		REG_FCIE_SETBIT(reg_ckg_fcie_1X, (u32ClkParam<<BIT_FCIE_CLK_SHIFT) | BIT_FCIE_CLK_SEL);
	}

    eMMC_PlatformResetPost();
	return eMMC_ST_SUCCESS;

}


void eMMC_set_WatchDog(U8 u8_IfEnable)
{
	u8_IfEnable = u8_IfEnable;
	// do nothing
}

void eMMC_reset_WatchDog(void)
{
	// do nothing
}


U32 eMMC_translate_DMA_address_Ex(U32 u32_DMAAddr, U32 u32_ByteCnt)
{
	u32_ByteCnt = u32_ByteCnt;
	#if 0
	flush_cache(u32_DMAAddr, u32_ByteCnt);
	#endif
	if(u32_DMAAddr < CONFIG_MIU1_BUSADDR) // MIU0
	{
		REG_FCIE_CLRBIT(FCIE_MIU_DMA_26_16, BIT_MIU1_SELECT);
		return (u32_DMAAddr - CONFIG_MIU0_BUSADDR);
	}
	else
	{	REG_FCIE_SETBIT(FCIE_MIU_DMA_26_16, BIT_MIU1_SELECT);
		return (u32_DMAAddr - CONFIG_MIU1_BUSADDR);
	}

	return u32_DMAAddr;
}

void eMMC_Invalidate_data_cache_buffer(U32 u32_addr, S32 s32_size)
{
	u32_addr = u32_addr;
	s32_size = s32_size;
//	flush_cache(u32_addr, s32_size);
}

void eMMC_flush_miu_pipe(void)
{

}

U32 eMMC_PlatformResetPre(void)
{

	return eMMC_ST_SUCCESS;
}

U32 eMMC_PlatformResetPost(void)
{

	return eMMC_ST_SUCCESS;
}

U32 eMMC_PlatformInit(void)
{
	eMMC_clock_setting(FCIE_SLOWEST_CLK);
	return eMMC_ST_SUCCESS;
}


//eMMC_ALIGN0 eMMC_DRIVER g_eMMCDrv eMMC_ALIGN1;



#elif (defined(eMMC_DRV_EIFFEL_BOOTRAM) && eMMC_DRV_EIFFEL_BOOTRAM)
void eMMC_DumpPadClk(void)
{

}

U32 eMMC_clock_setting(U32 u32ClkParam)
{
	eMMC_PlatformResetPre();
    REG_FCIE_CLRBIT(FCIE_SD_MODE, BIT_SD_CLK_EN);
	REG_FCIE_CLRBIT(reg_ckg_fcie_1X, BIT_FCIE_CLK_Gate|BIT_FCIE_CLK_MASK|BIT_FCIE_CLK_SEL);
	if(u32ClkParam==BIT_FCIE_CLK_300K)
	{
		REG_FCIE_SETBIT(reg_ckg_fcie_1X, (u32ClkParam<<BIT_FCIE_CLK_SHIFT) | BIT_FCIE_CLK_SEL);
	}

    eMMC_PlatformResetPost();
	return eMMC_ST_SUCCESS;

}

void eMMC_set_WatchDog(U8 u8_IfEnable)
{
	u8_IfEnable = u8_IfEnable;
	// do nothing
}

void eMMC_reset_WatchDog(void)
{
	// do nothing
}


U32 eMMC_translate_DMA_address_Ex(U32 u32_DMAAddr, U32 u32_ByteCnt)
{
	u32_ByteCnt = u32_ByteCnt;

	#if 0
	flush_cache(u32_DMAAddr, u32_ByteCnt);
	#endif

	if(u32_DMAAddr < CONFIG_MIU1_BUSADDR) // MIU0
	{
		REG_FCIE_CLRBIT(FCIE_MIU_DMA_26_16, BIT_MIU1_SELECT);
		return (u32_DMAAddr - CONFIG_MIU0_BUSADDR);
	}
	else
	{	REG_FCIE_SETBIT(FCIE_MIU_DMA_26_16, BIT_MIU1_SELECT);
		return (u32_DMAAddr - CONFIG_MIU1_BUSADDR);
	}

    return u32_DMAAddr;
}

void eMMC_Invalidate_data_cache_buffer(U32 u32_addr, S32 s32_size)
{
	u32_addr = u32_addr;
	s32_size = s32_size;
//	flush_cache(u32_addr, s32_size);
}

void eMMC_flush_miu_pipe(void)
{

}

U32 eMMC_PlatformResetPre(void)
{

	return eMMC_ST_SUCCESS;
}

U32 eMMC_PlatformResetPost(void)
{

	return eMMC_ST_SUCCESS;
}

U32 eMMC_PlatformInit(void)
{
	eMMC_clock_setting(FCIE_SLOWEST_CLK);
	return eMMC_ST_SUCCESS;
}


//eMMC_ALIGN0 eMMC_DRIVER g_eMMCDrv eMMC_ALIGN1;
//eMMC_ALIGN0 U8 gau8_eMMC_SectorBuf[eMMC_SECTOR_BUF_BYTECNT] eMMC_ALIGN1; // 512 bytes
//eMMC_ALIGN0 U8 gau8_eMMC_PartInfoBuf[eMMC_SECTOR_512BYTE] eMMC_ALIGN1; // 512 bytes




#elif (defined(eMMC_DRV_NIKE_BOOTRAM)&&eMMC_DRV_NIKE_BOOTRAM)
void eMMC_DumpPadClk(void)
{

}

U32 eMMC_clock_setting(U32 u32ClkParam)
{
    eMMC_PlatformResetPre();
    REG_FCIE_CLRBIT(FCIE_SD_MODE, BIT_SD_CLK_EN);
    if(u32ClkParam==BIT_FCIE_CLK_300K)
    {
        REG_FCIE_CLRBIT(reg_ckg_fcie, BIT_FCIE_CLK_Gate|BIT_FCIE_CLK_MASK);
        REG_FCIE_SETBIT(reg_ckg_fcie, (u32ClkParam<<BIT_FCIE_CLK_SHIFT));
        REG_FCIE_SETBIT(reg_ckg_nfie_div4_en, BIT_FCIE_CLK_DIV4_EN);
    }
    else
    {
        REG_FCIE_CLRBIT(reg_ckg_fcie, BIT_FCIE_CLK_Gate|BIT_FCIE_CLK_MASK);
        REG_FCIE_CLRBIT(reg_ckg_nfie_div4_en, BIT_FCIE_CLK_DIV4_EN);
    }


    eMMC_PlatformResetPost();
	return eMMC_ST_SUCCESS;
}

void eMMC_set_WatchDog(U8 u8_IfEnable)
{
	u8_IfEnable = u8_IfEnable;
	// do nothing
}

void eMMC_reset_WatchDog(void)
{
	// do nothing
}

U32 eMMC_translate_DMA_address_Ex(U32 u32_DMAAddr, U32 u32_ByteCnt)
{
	u32_ByteCnt = u32_ByteCnt;
	#if 0
	flush_cache(u32_DMAAddr, u32_ByteCnt);
	#endif

	if(u32_DMAAddr < CONFIG_MIU1_BUSADDR) // MIU0
	{
		REG_FCIE_CLRBIT(FCIE_MIU_DMA_26_16, BIT_MIU1_SELECT);
		return (u32_DMAAddr - CONFIG_MIU0_BUSADDR);
	}
	else
	{	REG_FCIE_SETBIT(FCIE_MIU_DMA_26_16, BIT_MIU1_SELECT);
		return (u32_DMAAddr - CONFIG_MIU1_BUSADDR);
	}

	return u32_DMAAddr;
}

void eMMC_Invalidate_data_cache_buffer(U32 u32_addr, S32 s32_size)
{
	u32_addr = u32_addr;
	s32_size = s32_size;
//	flush_cache(u32_addr, s32_size);
}

void eMMC_flush_miu_pipe(void)
{

}

U32 eMMC_PlatformResetPre(void)
{

	return eMMC_ST_SUCCESS;
}

U32 eMMC_PlatformResetPost(void)
{

	return eMMC_ST_SUCCESS;
}

U32 eMMC_PlatformInit(void)
{
	eMMC_clock_setting(FCIE_SLOWEST_CLK);
	return eMMC_ST_SUCCESS;
}

//eMMC_ALIGN0 eMMC_DRIVER g_eMMCDrv eMMC_ALIGN1;
//eMMC_ALIGN0 U8 gau8_eMMC_SectorBuf[eMMC_SECTOR_BUF_16KB] eMMC_ALIGN1; // 512 bytes
//eMMC_ALIGN0 U8 gau8_eMMC_PartInfoBuf[eMMC_SECTOR_512BYTE] eMMC_ALIGN1; // 512 bytes

#elif (defined(eMMC_DRV_MADISON_BOOTRAM)&&eMMC_DRV_MADISON_BOOTRAM)
void eMMC_DumpPadClk(void)
{

}

U32 eMMC_clock_setting(U32 u32ClkParam)
{
	eMMC_PlatformResetPre();
    REG_FCIE_CLRBIT(FCIE_SD_MODE, BIT_SD_CLK_EN);
    if(u32ClkParam==BIT_FCIE_CLK_300K)
    {
	    REG_FCIE_CLRBIT(reg_ckg_nfie, BIT_XTAL_CLK_SEL);
	    REG_FCIE_CLRBIT(reg_ckg_fcie, BIT_FCIE_CLK_Gate|BIT_FCIE_CLK_MASK);
	    REG_FCIE_SETBIT(reg_ckg_fcie, (u32ClkParam<<BIT_FCIE_CLK_SHIFT));
        REG_FCIE_SETBIT(reg_ckg_nfie_div4_en, BIT_FCIE_CLK_DIV4_EN);
	    REG_FCIE_SETBIT(reg_ckg_nfie, BIT_XTAL_CLK_SEL);
    }
    else
    {
	    REG_FCIE_CLRBIT(reg_ckg_nfie, BIT_XTAL_CLK_SEL);
	    REG_FCIE_CLRBIT(reg_ckg_fcie, BIT_FCIE_CLK_Gate|BIT_FCIE_CLK_MASK);
        REG_FCIE_CLRBIT(reg_ckg_nfie_div4_en, BIT_FCIE_CLK_DIV4_EN);
    }
    eMMC_PlatformResetPost();
	return eMMC_ST_SUCCESS;
}

void eMMC_set_WatchDog(U8 u8_IfEnable)
{
	u8_IfEnable = u8_IfEnable;
	// do nothing
}

void eMMC_reset_WatchDog(void)
{
	// do nothing
}

U32 eMMC_translate_DMA_address_Ex(U32 u32_DMAAddr, U32 u32_ByteCnt)
{
	u32_ByteCnt = u32_ByteCnt;
	#if 0
	flush_cache(u32_DMAAddr, u32_ByteCnt);
	#endif

	if(u32_DMAAddr < CONFIG_MIU1_BUSADDR) // MIU0
	{
		REG_FCIE_CLRBIT(FCIE_MIU_DMA_26_16, BIT_MIU1_SELECT);
		return (u32_DMAAddr - CONFIG_MIU0_BUSADDR);
	}
	else
	{	REG_FCIE_SETBIT(FCIE_MIU_DMA_26_16, BIT_MIU1_SELECT);
		return (u32_DMAAddr - CONFIG_MIU1_BUSADDR);
	}

	return u32_DMAAddr;
}

void eMMC_Invalidate_data_cache_buffer(U32 u32_addr, S32 s32_size)
{
	u32_addr = u32_addr;
	s32_size = s32_size;
//	flush_cache(u32_addr, s32_size);
}

void eMMC_flush_miu_pipe(void)
{

}


U32 eMMC_PlatformResetPre(void)
{

	return eMMC_ST_SUCCESS;
}

U32 eMMC_PlatformResetPost(void)
{

	return eMMC_ST_SUCCESS;
}

U32 eMMC_PlatformInit(void)
{
	eMMC_clock_setting(FCIE_SLOWEST_CLK);
	return eMMC_ST_SUCCESS;
}

U32 eMMC_ROM_LoadImages_Addr(void)
{
	U32 u32_func_addr;
	u32_func_addr =(REG_FCIE(DEBUG_REG_SYS_L1)<<16) + REG_FCIE(DEBUG_REG_SYS_L2);

    return u32_func_addr;
}

//eMMC_ALIGN0 eMMC_DRIVER g_eMMCDrv eMMC_ALIGN1;
//eMMC_ALIGN0 U8 gau8_eMMC_SectorBuf[eMMC_SECTOR_BUF_16KB] eMMC_ALIGN1; // 512 bytes
//eMMC_ALIGN0 U8 gau8_eMMC_PartInfoBuf[eMMC_SECTOR_512BYTE] eMMC_ALIGN1; // 512 bytes

#elif (defined(eMMC_DRV_CLIPPERS_BOOTRAM)&&eMMC_DRV_CLIPPERS_BOOTRAM)
void eMMC_DumpPadClk(void)
{

}

void eMMC_pads_switch(void)
{
    // fcie
    REG_FCIE_CLRBIT(FCIE_DDR_MODE, BIT_FALL_LATCH|BIT_PAD_IN_SEL_SD|BIT_32BIT_MACRO_EN|BIT_DDR_EN|BIT_8BIT_MACRO_EN);
    REG_FCIE_SETBIT(FCIE_DDR_MODE,BIT_PAD_IN_SEL_SD|BIT_FALL_LATCH);
}

void eMMC_pads_switch_default(void)
{
    // fcie
    REG_FCIE_CLRBIT(FCIE_DDR_MODE, BIT_FALL_LATCH|BIT_PAD_IN_SEL_SD|BIT_32BIT_MACRO_EN|BIT_DDR_EN|BIT_8BIT_MACRO_EN);
}

U32 eMMC_clock_setting(U32 u32ClkParam)
{
	eMMC_PlatformResetPre();
	REG_FCIE_CLRBIT(FCIE_SD_MODE, BIT_SD_CLK_EN);
	REG_FCIE_CLRBIT(reg_ckg_fcie, BIT_FCIE_CLK_Gate|BIT_FCIE_CLK_MASK|BIT_FCIE_CLK_SRC_SEL);

	if(u32ClkParam==BIT_FCIE_CLK_300K)
	{
		REG_FCIE_SETBIT(reg_ckg_fcie, u32ClkParam<<BIT_FCIE_CLK_SHIFT | BIT_FCIE_CLK_SRC_SEL);
	}
	else
	{
		REG_FCIE_SETBIT(reg_ckg_fcie, u32ClkParam<<BIT_FCIE_CLK_SHIFT);
	}

    eMMC_PlatformResetPost();
	return eMMC_ST_SUCCESS;
}

void eMMC_set_WatchDog(U8 u8_IfEnable)
{
	u8_IfEnable = u8_IfEnable;
	// do nothing
}

void eMMC_reset_WatchDog(void)
{
	// do nothing
}

U32 eMMC_translate_DMA_address_Ex(U32 u32_DMAAddr, U32 u32_ByteCnt)
{
	u32_ByteCnt = u32_ByteCnt;
	#if 0
	flush_cache(u32_DMAAddr, u32_ByteCnt);
	#endif
	REG_FCIE_CLRBIT(FCIE_MMA_PRI_REG, BIT_MIU_SELECT_MASK);

	if(u32_DMAAddr < CONFIG_MIU1_BUSADDR) // MIU0
	{
		return (u32_DMAAddr - CONFIG_MIU0_BUSADDR);
	}
	else  // MIU1
	{
		REG_FCIE_SETBIT(FCIE_MMA_PRI_REG, BIT_MIU1_SELECT);
		return (u32_DMAAddr - CONFIG_MIU1_BUSADDR);
	}

	return u32_DMAAddr;
}

void eMMC_Invalidate_data_cache_buffer(U32 u32_addr, S32 s32_size)
{
	u32_addr = u32_addr;
	s32_size = s32_size;
//	flush_cache(u32_addr, s32_size);
}

void eMMC_flush_miu_pipe(void)
{

}


U32 eMMC_PlatformResetPre(void)
{

	return eMMC_ST_SUCCESS;
}

U32 eMMC_PlatformResetPost(void)
{

	return eMMC_ST_SUCCESS;
}

U32 eMMC_PlatformInit(void)
{
	eMMC_clock_setting(FCIE_SLOWEST_CLK);
    eMMC_pads_switch();
	return eMMC_ST_SUCCESS;
}


//eMMC_ALIGN0 eMMC_DRIVER g_eMMCDrv eMMC_ALIGN1;
//eMMC_ALIGN0 U8 gau8_eMMC_SectorBuf[eMMC_SECTOR_BUF_16KB] eMMC_ALIGN1; // 512 bytes
//eMMC_ALIGN0 U8 gau8_eMMC_PartInfoBuf[eMMC_SECTOR_512BYTE] eMMC_ALIGN1; // 512 bytes
#elif (defined(eMMC_DRV_MIAMI_BOOTRAM)&&eMMC_DRV_MIAMI_BOOTRAM)
void eMMC_DumpPadClk(void)
{

}


U32 eMMC_clock_setting(U32 u32ClkParam)
{
	eMMC_PlatformResetPre();
    REG_FCIE_CLRBIT(FCIE_SD_MODE, BIT_SD_CLK_EN);

	REG_FCIE_CLRBIT(reg_ckg_fcie, BIT_FCIE_CLK_Gate|BIT_FCIE_CLK_MASK|BIT_FCIE_CLK_SRC_SEL);

	if(u32ClkParam==BIT_FCIE_CLK_300K)
	{
		REG_FCIE_SETBIT(reg_ckg_fcie, u32ClkParam<<BIT_FCIE_CLK_SHIFT | BIT_FCIE_CLK_SRC_SEL);
	}
	else
	{
		REG_FCIE_SETBIT(reg_ckg_fcie, u32ClkParam<<BIT_FCIE_CLK_SHIFT);
	}

    eMMC_PlatformResetPost();
	return eMMC_ST_SUCCESS;
}

void eMMC_set_WatchDog(U8 u8_IfEnable)
{
	u8_IfEnable = u8_IfEnable;
	// do nothing
}

void eMMC_reset_WatchDog(void)
{
	// do nothing
}

U32 eMMC_translate_DMA_address_Ex(U32 u32_DMAAddr, U32 u32_ByteCnt)
{
	u32_ByteCnt = u32_ByteCnt;
	#if 0
	flush_cache(u32_DMAAddr, u32_ByteCnt);
	#endif

	// Need to set bank 0x1006 16 bits addr 0x7A bit9 on to make below code work,
	// otherwise, always DMA to MIU0

	REG_FCIE_CLRBIT(FCIE_MMA_PRI_REG, BIT_MIU_SELECT_MASK);

	if(u32_DMAAddr < CONFIG_MIU1_BUSADDR) // MIU0
	{
		return (u32_DMAAddr - CONFIG_MIU0_BUSADDR);
	}
	else // MIU1
	{
		REG_FCIE_SETBIT(FCIE_MMA_PRI_REG, BIT_MIU1_SELECT);
		return (u32_DMAAddr - CONFIG_MIU1_BUSADDR);
	}
#if 0
	else if(u32_DMAAddr < CONFIG_MIU2_BUSADDR) // MIU1
	{
		REG_FCIE_SETBIT(FCIE_MMA_PRI_REG, BIT_MIU1_SELECT);
		return (u32_DMAAddr - CONFIG_MIU1_BUSADDR);
	}
	else // MIU2
	{
		REG_FCIE_SETBIT(FCIE_MMA_PRI_REG, BIT_MIU2_SELECT);
		return (u32_DMAAddr - CONFIG_MIU2_BUSADDR);
	}
#endif

	return u32_DMAAddr;
}

void eMMC_Invalidate_data_cache_buffer(U32 u32_addr, S32 s32_size)
{
	u32_addr = u32_addr;
	s32_size = s32_size;
//	flush_cache(u32_addr, s32_size);
}

void eMMC_flush_miu_pipe(void)
{

}


U32 eMMC_PlatformResetPre(void)
{

	return eMMC_ST_SUCCESS;
}

U32 eMMC_PlatformResetPost(void)
{

	return eMMC_ST_SUCCESS;
}

U32 eMMC_PlatformInit(void)
{
	eMMC_clock_setting(FCIE_SLOWEST_CLK);

	return eMMC_ST_SUCCESS;
}

//eMMC_ALIGN0 eMMC_DRIVER g_eMMCDrv eMMC_ALIGN1;
//eMMC_ALIGN0 U8 gau8_eMMC_SectorBuf[eMMC_SECTOR_BUF_16KB] eMMC_ALIGN1; // 512 bytes
//eMMC_ALIGN0 U8 gau8_eMMC_PartInfoBuf[eMMC_SECTOR_512BYTE] eMMC_ALIGN1; // 512 bytes


#elif (defined(eMMC_DRV_MONACO_BOOTRAM)&&eMMC_DRV_MONACO_BOOTRAM)
void eMMC_DumpPadClk(void)
{

}

void eMMC_pads_switch(void)
{
    // fcie
    REG_FCIE_CLRBIT(FCIE_DDR_MODE, BIT_FALL_LATCH|BIT_PAD_IN_SEL_SD|BIT_32BIT_MACRO_EN|BIT_DDR_EN|BIT_8BIT_MACRO_EN);
    REG_FCIE_SETBIT(FCIE_DDR_MODE,BIT_PAD_IN_SEL_SD|BIT_FALL_LATCH);
}

void eMMC_pads_switch_default(void)
{
    // fcie
    REG_FCIE_CLRBIT(FCIE_DDR_MODE, BIT_FALL_LATCH|BIT_PAD_IN_SEL_SD|BIT_32BIT_MACRO_EN|BIT_DDR_EN|BIT_8BIT_MACRO_EN);
}


U32 eMMC_clock_setting(U32 u32ClkParam)
{
	eMMC_PlatformResetPre();
    REG_FCIE_CLRBIT(FCIE_SD_MODE, BIT_SD_CLK_EN);

	REG_FCIE_CLRBIT(reg_ckg_fcie, BIT_FCIE_CLK_Gate|BIT_FCIE_CLK_MASK|BIT_FCIE_CLK_SRC_SEL);

	if(u32ClkParam==BIT_FCIE_CLK_300K)
	{
		REG_FCIE_SETBIT(reg_ckg_fcie, u32ClkParam<<BIT_FCIE_CLK_SHIFT | BIT_FCIE_CLK_SRC_SEL);
	}
	else
	{
		REG_FCIE_SETBIT(reg_ckg_fcie, u32ClkParam<<BIT_FCIE_CLK_SHIFT);
	}

    eMMC_PlatformResetPost();
	return eMMC_ST_SUCCESS;
}

void eMMC_set_WatchDog(U8 u8_IfEnable)
{
	u8_IfEnable = u8_IfEnable;
	// do nothing
}

void eMMC_reset_WatchDog(void)
{
	// do nothing
}

U32 eMMC_translate_DMA_address_Ex(U32 u32_DMAAddr, U32 u32_ByteCnt)
{
	u32_ByteCnt = u32_ByteCnt;
	#if 0
	flush_cache(u32_DMAAddr, u32_ByteCnt);
	#endif

	// Need to set bank 0x1006 16 bits addr 0x7A bit9 on to make below code work,
	// otherwise, always DMA to MIU0

	REG_FCIE_CLRBIT(FCIE_MMA_PRI_REG, BIT_MIU_SELECT_MASK);

	if(u32_DMAAddr < CONFIG_MIU1_BUSADDR) // MIU0
	{
		return (u32_DMAAddr - CONFIG_MIU0_BUSADDR);
	}
	else if(u32_DMAAddr < CONFIG_MIU2_BUSADDR) // MIU1
	{
		REG_FCIE_SETBIT(FCIE_MMA_PRI_REG, BIT_MIU1_SELECT);
		return (u32_DMAAddr - CONFIG_MIU1_BUSADDR);
	}
	else // MIU2
	{
		REG_FCIE_SETBIT(FCIE_MMA_PRI_REG, BIT_MIU2_SELECT);
		return (u32_DMAAddr - CONFIG_MIU2_BUSADDR);
	}

	return u32_DMAAddr;
}

void eMMC_Invalidate_data_cache_buffer(U32 u32_addr, S32 s32_size)
{
	u32_addr = u32_addr;
	s32_size = s32_size;
//	flush_cache(u32_addr, s32_size);
}

void eMMC_flush_miu_pipe(void)
{

}


U32 eMMC_PlatformResetPre(void)
{

	return eMMC_ST_SUCCESS;
}

U32 eMMC_PlatformResetPost(void)
{

	return eMMC_ST_SUCCESS;
}

U32 eMMC_PlatformInit(void)
{
	eMMC_clock_setting(FCIE_SLOWEST_CLK);
    eMMC_pads_switch();
	return eMMC_ST_SUCCESS;
}

//eMMC_ALIGN0 eMMC_DRIVER g_eMMCDrv eMMC_ALIGN1;
//eMMC_ALIGN0 U8 gau8_eMMC_SectorBuf[eMMC_SECTOR_BUF_16KB] eMMC_ALIGN1; // 512 bytes
//eMMC_ALIGN0 U8 gau8_eMMC_PartInfoBuf[eMMC_SECTOR_512BYTE] eMMC_ALIGN1; // 512 bytes

#elif (defined(eMMC_DRV_MUJI_BOOTRAM)&&eMMC_DRV_MUJI_BOOTRAM)
void eMMC_DumpPadClk(void)
{

}

void eMMC_pads_switch(void)
{
    // fcie
    REG_FCIE_CLRBIT(FCIE_DDR_MODE, BIT_FALL_LATCH|BIT_PAD_IN_SEL_SD|BIT_32BIT_MACRO_EN|BIT_DDR_EN|BIT_8BIT_MACRO_EN);
    REG_FCIE_SETBIT(FCIE_DDR_MODE,BIT_PAD_IN_SEL_SD|BIT_FALL_LATCH);
}

void eMMC_pads_switch_default(void)
{
    // fcie
    REG_FCIE_CLRBIT(FCIE_DDR_MODE, BIT_FALL_LATCH|BIT_PAD_IN_SEL_SD|BIT_32BIT_MACRO_EN|BIT_DDR_EN|BIT_8BIT_MACRO_EN);
}

U32 eMMC_clock_setting(U32 u32ClkParam)
{
	eMMC_PlatformResetPre();
    REG_FCIE_CLRBIT(FCIE_SD_MODE, BIT_SD_CLK_EN);

	REG_FCIE_CLRBIT(reg_ckg_fcie, BIT_FCIE_CLK_Gate|BIT_FCIE_CLK_MASK|BIT_FCIE_CLK_SRC_SEL);

	if(u32ClkParam==BIT_FCIE_CLK_300K)
	{
		REG_FCIE_SETBIT(reg_ckg_fcie, u32ClkParam<<BIT_FCIE_CLK_SHIFT | BIT_FCIE_CLK_SRC_SEL);
	}
	else
	{
		REG_FCIE_SETBIT(reg_ckg_fcie, u32ClkParam<<BIT_FCIE_CLK_SHIFT);
	}

    eMMC_PlatformResetPost();
	return eMMC_ST_SUCCESS;
}

void eMMC_set_WatchDog(U8 u8_IfEnable)
{
	u8_IfEnable = u8_IfEnable;
	// do nothing
}

void eMMC_reset_WatchDog(void)
{
	// do nothing
}

U32 eMMC_translate_DMA_address_Ex(U32 u32_DMAAddr, U32 u32_ByteCnt)
{
	u32_ByteCnt = u32_ByteCnt;
	#if 0
	flush_cache(u32_DMAAddr, u32_ByteCnt);
	#endif

	// Need to set bank 0x1006 16 bits addr 0x7A bit9 on to make below code work,
	// otherwise, always DMA to MIU0

	REG_FCIE_CLRBIT(FCIE_MMA_PRI_REG, BIT_MIU_SELECT_MASK);

	if(u32_DMAAddr < CONFIG_MIU1_BUSADDR) // MIU0
	{
		return (u32_DMAAddr - CONFIG_MIU0_BUSADDR);
	}
	else if(u32_DMAAddr < CONFIG_MIU2_BUSADDR) // MIU1
	{
		REG_FCIE_SETBIT(FCIE_MMA_PRI_REG, BIT_MIU1_SELECT);
		return (u32_DMAAddr - CONFIG_MIU1_BUSADDR);
	}
	else // MIU2
	{
		REG_FCIE_SETBIT(FCIE_MMA_PRI_REG, BIT_MIU2_SELECT);
		return (u32_DMAAddr - CONFIG_MIU2_BUSADDR);
	}

	return u32_DMAAddr;
}

void eMMC_Invalidate_data_cache_buffer(U32 u32_addr, S32 s32_size)
{
	u32_addr = u32_addr;
	s32_size = s32_size;
//	flush_cache(u32_addr, s32_size);
}

void eMMC_flush_miu_pipe(void)
{

}


U32 eMMC_PlatformResetPre(void)
{

	return eMMC_ST_SUCCESS;
}

U32 eMMC_PlatformResetPost(void)
{

	return eMMC_ST_SUCCESS;
}

U32 eMMC_PlatformInit(void)
{
	eMMC_clock_setting(FCIE_SLOWEST_CLK);
    eMMC_pads_switch();
	return eMMC_ST_SUCCESS;
}

//eMMC_ALIGN0 eMMC_DRIVER g_eMMCDrv eMMC_ALIGN1;
//eMMC_ALIGN0 U8 gau8_eMMC_SectorBuf[eMMC_SECTOR_BUF_16KB] eMMC_ALIGN1; // 512 bytes
//eMMC_ALIGN0 U8 gau8_eMMC_PartInfoBuf[eMMC_SECTOR_512BYTE] eMMC_ALIGN1; // 512 bytes

#elif (defined(eMMC_DRV_MANHATTAN_BOOTRAM)&&eMMC_DRV_MANHATTAN_BOOTRAM)
void eMMC_DumpPadClk(void)
{

}

void eMMC_pads_switch(void)
{
    // fcie
    REG_FCIE_CLRBIT(FCIE_DDR_MODE, BIT_FALL_LATCH|BIT_PAD_IN_SEL_SD|BIT_32BIT_MACRO_EN|BIT_DDR_EN|BIT_8BIT_MACRO_EN);
    REG_FCIE_SETBIT(FCIE_DDR_MODE,BIT_PAD_IN_SEL_SD|BIT_FALL_LATCH);
}

void eMMC_pads_switch_default(void)
{
    // fcie
    REG_FCIE_CLRBIT(FCIE_DDR_MODE, BIT_FALL_LATCH|BIT_PAD_IN_SEL_SD|BIT_32BIT_MACRO_EN|BIT_DDR_EN|BIT_8BIT_MACRO_EN);
}

U32 eMMC_clock_setting(U32 u32ClkParam)
{
	eMMC_PlatformResetPre();
    REG_FCIE_CLRBIT(FCIE_SD_MODE, BIT_SD_CLK_EN);

	REG_FCIE_CLRBIT(reg_ckg_fcie, BIT_FCIE_CLK_Gate|BIT_FCIE_CLK_MASK|BIT_FCIE_CLK_SRC_SEL);

	if(u32ClkParam==BIT_FCIE_CLK_300K)
	{
		REG_FCIE_SETBIT(reg_ckg_fcie, u32ClkParam<<BIT_FCIE_CLK_SHIFT | BIT_FCIE_CLK_SRC_SEL);
	}
	else
	{
		REG_FCIE_SETBIT(reg_ckg_fcie, u32ClkParam<<BIT_FCIE_CLK_SHIFT);
	}

    eMMC_PlatformResetPost();
	return eMMC_ST_SUCCESS;
}

void eMMC_set_WatchDog(U8 u8_IfEnable)
{
	u8_IfEnable = u8_IfEnable;
	// do nothing
}

void eMMC_reset_WatchDog(void)
{
	// do nothing
}

U32 eMMC_translate_DMA_address_Ex(U32 u32_DMAAddr, U32 u32_ByteCnt)
{
	u32_ByteCnt = u32_ByteCnt;
	#if 0
	flush_cache(u32_DMAAddr, u32_ByteCnt);
	#endif

	// Need to set bank 0x1006 16 bits addr 0x7A bit9 on to make below code work,
	// otherwise, always DMA to MIU0

	REG_FCIE_CLRBIT(FCIE_MMA_PRI_REG, BIT_MIU_SELECT_MASK);

	if(u32_DMAAddr < CONFIG_MIU1_BUSADDR) // MIU0
	{
		return (u32_DMAAddr - CONFIG_MIU0_BUSADDR);
	}
	else if(u32_DMAAddr < CONFIG_MIU2_BUSADDR) // MIU1
	{
		REG_FCIE_SETBIT(FCIE_MMA_PRI_REG, BIT_MIU1_SELECT);
		return (u32_DMAAddr - CONFIG_MIU1_BUSADDR);
	}
	else // MIU2
	{
		REG_FCIE_SETBIT(FCIE_MMA_PRI_REG, BIT_MIU2_SELECT);
		return (u32_DMAAddr - CONFIG_MIU2_BUSADDR);
	}

	return u32_DMAAddr;
}

void eMMC_Invalidate_data_cache_buffer(U32 u32_addr, S32 s32_size)
{
	u32_addr = u32_addr;
	s32_size = s32_size;
//	flush_cache(u32_addr, s32_size);
}

void eMMC_flush_miu_pipe(void)
{

}


U32 eMMC_PlatformResetPre(void)
{

	return eMMC_ST_SUCCESS;
}

U32 eMMC_PlatformResetPost(void)
{

	return eMMC_ST_SUCCESS;
}

U32 eMMC_PlatformInit(void)
{
	eMMC_clock_setting(FCIE_SLOWEST_CLK);
    eMMC_pads_switch();
	return eMMC_ST_SUCCESS;
}

//eMMC_ALIGN0 eMMC_DRIVER g_eMMCDrv eMMC_ALIGN1;
//eMMC_ALIGN0 U8 gau8_eMMC_SectorBuf[eMMC_SECTOR_BUF_16KB] eMMC_ALIGN1; // 512 bytes
//eMMC_ALIGN0 U8 gau8_eMMC_PartInfoBuf[eMMC_SECTOR_512BYTE] eMMC_ALIGN1; // 512 bytes

#elif (defined(eMMC_DRV_MASERATI_BOOTRAM)&&eMMC_DRV_MASERATI_BOOTRAM)
void eMMC_DumpPadClk(void)
{

}

void eMMC_pads_switch(void)
{
    // fcie
    REG_FCIE_CLRBIT(FCIE_DDR_MODE, BIT_FALL_LATCH|BIT_PAD_IN_SEL_SD|BIT_CLK2_SEL|BIT_32BIT_MACRO_EN|BIT_DDR_EN|BIT_8BIT_MACRO_EN);
    REG_FCIE_SETBIT(FCIE_DDR_MODE,BIT_PAD_IN_SEL_SD|BIT_FALL_LATCH|BIT_CLK2_SEL);
    REG_FCIE_SETBIT(reg_emmc_test, BIT10);
}

void eMMC_pads_switch_default(void)
{
    // fcie
    REG_FCIE_CLRBIT(FCIE_DDR_MODE, BIT_FALL_LATCH|BIT_PAD_IN_SEL_SD|BIT_CLK2_SEL|BIT_32BIT_MACRO_EN|BIT_DDR_EN|BIT_8BIT_MACRO_EN);
    REG_FCIE_CLRBIT(reg_emmc_test, BIT10);
}

U32 eMMC_clock_setting(U32 u32ClkParam)
{
	eMMC_PlatformResetPre();
    REG_FCIE_CLRBIT(FCIE_SD_MODE, BIT_SD_CLK_EN);

	REG_FCIE_CLRBIT(reg_ckg_fcie, BIT_FCIE_CLK_Gate|BIT_FCIE_CLK_MASK|BIT_FCIE_CLK_SRC_SEL);

	if(u32ClkParam==BIT_FCIE_CLK_300K)
	{
		REG_FCIE_SETBIT(reg_ckg_fcie, u32ClkParam<<BIT_FCIE_CLK_SHIFT | BIT_FCIE_CLK_SRC_SEL);
	}
	else
	{
		REG_FCIE_SETBIT(reg_ckg_fcie, u32ClkParam<<BIT_FCIE_CLK_SHIFT);
	}

    eMMC_PlatformResetPost();
	return eMMC_ST_SUCCESS;
}

void eMMC_set_WatchDog(U8 u8_IfEnable)
{
	u8_IfEnable = u8_IfEnable;
	// do nothing
}

void eMMC_reset_WatchDog(void)
{
	// do nothing
}

U32 eMMC_translate_DMA_address_Ex(U32 u32_DMAAddr, U32 u32_ByteCnt)
{
	u32_ByteCnt = u32_ByteCnt;
	#if 0
	flush_cache(u32_DMAAddr, u32_ByteCnt);
	#endif

	// Need to set bank 0x1006 16 bits addr 0x7A bit9 on to make below code work,
	// otherwise, always DMA to MIU0

	REG_FCIE_CLRBIT(FCIE_MMA_PRI_REG, BIT_MIU_SELECT_MASK);

	if(u32_DMAAddr < CONFIG_MIU1_BUSADDR) // MIU0
	{
		return (u32_DMAAddr - CONFIG_MIU0_BUSADDR);
	}
	else if(u32_DMAAddr < CONFIG_MIU2_BUSADDR) // MIU1
	{
		REG_FCIE_SETBIT(FCIE_MMA_PRI_REG, BIT_MIU1_SELECT);
		return (u32_DMAAddr - CONFIG_MIU1_BUSADDR);
	}
	else // MIU2
	{
		REG_FCIE_SETBIT(FCIE_MMA_PRI_REG, BIT_MIU2_SELECT);
		return (u32_DMAAddr - CONFIG_MIU2_BUSADDR);
	}

	return u32_DMAAddr;
}

void eMMC_Invalidate_data_cache_buffer(U32 u32_addr, S32 s32_size)
{
	u32_addr = u32_addr;
	s32_size = s32_size;
//	flush_cache(u32_addr, s32_size);
}

void eMMC_flush_miu_pipe(void)
{

}


U32 eMMC_PlatformResetPre(void)
{

	return eMMC_ST_SUCCESS;
}

U32 eMMC_PlatformResetPost(void)
{

	return eMMC_ST_SUCCESS;
}

U32 eMMC_PlatformInit(void)
{
	eMMC_clock_setting(FCIE_SLOWEST_CLK);
    eMMC_pads_switch();
	return eMMC_ST_SUCCESS;
}

//eMMC_ALIGN0 eMMC_DRIVER g_eMMCDrv eMMC_ALIGN1;
//eMMC_ALIGN0 U8 gau8_eMMC_SectorBuf[eMMC_SECTOR_BUF_16KB] eMMC_ALIGN1; // 512 bytes
//eMMC_ALIGN0 U8 gau8_eMMC_PartInfoBuf[eMMC_SECTOR_512BYTE] eMMC_ALIGN1; // 512 bytes

#elif (defined(eMMC_DRV_MACAN_BOOTRAM)&&eMMC_DRV_MACAN_BOOTRAM) || (defined(eMMC_DRV_MAZDA_BOOTRAM)&&eMMC_DRV_MAZDA_BOOTRAM)
void eMMC_DumpPadClk(void)
{

}

void eMMC_pads_switch(void)
{
    // fcie
    REG_FCIE_CLRBIT(FCIE_DDR_MODE, BIT_FALL_LATCH|BIT_PAD_IN_SEL_SD|BIT_CLK2_SEL|BIT_32BIT_MACRO_EN|BIT_DDR_EN|BIT_8BIT_MACRO_EN);
    REG_FCIE_SETBIT(FCIE_DDR_MODE,BIT_PAD_IN_SEL_SD|BIT_FALL_LATCH|BIT_CLK2_SEL);
    REG_FCIE_SETBIT(reg_emmc_test, BIT10);
}

void eMMC_pads_switch_default(void)
{
    // fcie
    REG_FCIE_CLRBIT(FCIE_DDR_MODE, BIT_FALL_LATCH|BIT_PAD_IN_SEL_SD|BIT_CLK2_SEL|BIT_32BIT_MACRO_EN|BIT_DDR_EN|BIT_8BIT_MACRO_EN);
    REG_FCIE_CLRBIT(reg_emmc_test, BIT10);
}

U32 eMMC_clock_setting(U32 u32ClkParam)
{
	eMMC_PlatformResetPre();
    REG_FCIE_CLRBIT(FCIE_SD_MODE, BIT_SD_CLK_EN);

	REG_FCIE_CLRBIT(reg_ckg_fcie, BIT_FCIE_CLK_Gate|BIT_FCIE_CLK_MASK|BIT_FCIE_CLK_SRC_SEL);

	if(u32ClkParam==BIT_FCIE_CLK_300K)
	{
		REG_FCIE_SETBIT(reg_ckg_fcie, u32ClkParam<<BIT_FCIE_CLK_SHIFT | BIT_FCIE_CLK_SRC_SEL);
	}
	else
	{
		REG_FCIE_SETBIT(reg_ckg_fcie, u32ClkParam<<BIT_FCIE_CLK_SHIFT);
	}

    eMMC_PlatformResetPost();
	return eMMC_ST_SUCCESS;
}

void eMMC_set_WatchDog(U8 u8_IfEnable)
{
	u8_IfEnable = u8_IfEnable;
	// do nothing
}

void eMMC_reset_WatchDog(void)
{
	// do nothing
}

U32 eMMC_translate_DMA_address_Ex(U32 u32_DMAAddr, U32 u32_ByteCnt)
{
	u32_ByteCnt = u32_ByteCnt;
	#if 0
	flush_cache(u32_DMAAddr, u32_ByteCnt);
	#endif

	// Need to set bank 0x1006 16 bits addr 0x7A bit9 on to make below code work,
	// otherwise, always DMA to MIU0

	REG_FCIE_CLRBIT(FCIE_MMA_PRI_REG, BIT_MIU_SELECT_MASK);

	if(u32_DMAAddr < CONFIG_MIU1_BUSADDR) // MIU0
	{
		return (u32_DMAAddr - CONFIG_MIU0_BUSADDR);
	}
	else
	{
		REG_FCIE_SETBIT(FCIE_MMA_PRI_REG, BIT_MIU1_SELECT);
		return (u32_DMAAddr - CONFIG_MIU1_BUSADDR);
	}

	return u32_DMAAddr;
}

void eMMC_Invalidate_data_cache_buffer(U32 u32_addr, S32 s32_size)
{
	u32_addr = u32_addr;
	s32_size = s32_size;
//	flush_cache(u32_addr, s32_size);
}

void eMMC_flush_miu_pipe(void)
{

}

U32 eMMC_PlatformResetPre(void)
{

	return eMMC_ST_SUCCESS;
}

U32 eMMC_PlatformResetPost(void)
{

	return eMMC_ST_SUCCESS;
}

U32 eMMC_PlatformInit(void)
{
	eMMC_clock_setting(FCIE_SLOWEST_CLK);

	return eMMC_ST_SUCCESS;
}

//eMMC_ALIGN0 eMMC_DRIVER g_eMMCDrv eMMC_ALIGN1;
//eMMC_ALIGN0 U8 gau8_eMMC_SectorBuf[eMMC_SECTOR_BUF_16KB] eMMC_ALIGN1; // 512 bytes
//eMMC_ALIGN0 U8 gau8_eMMC_PartInfoBuf[eMMC_SECTOR_512BYTE] eMMC_ALIGN1; // 512 bytes

#elif (defined(eMMC_DRV_MONET_BOOTRAM)&&eMMC_DRV_MONET_BOOTRAM)
void eMMC_DumpPadClk(void)
{

}

void eMMC_pads_switch(void)
{
    // fcie
    REG_FCIE_CLRBIT(FCIE_DDR_MODE, BIT_FALL_LATCH|BIT_PAD_IN_SEL_SD|BIT_32BIT_MACRO_EN|BIT_DDR_EN|BIT_8BIT_MACRO_EN);
    REG_FCIE_SETBIT(FCIE_DDR_MODE,BIT_PAD_IN_SEL_SD|BIT_FALL_LATCH);
}

void eMMC_pads_switch_default(void)
{
    // fcie
    REG_FCIE_CLRBIT(FCIE_DDR_MODE, BIT_FALL_LATCH|BIT_PAD_IN_SEL_SD|BIT_32BIT_MACRO_EN|BIT_DDR_EN|BIT_8BIT_MACRO_EN);
}

U32 eMMC_clock_setting(U32 u32ClkParam)
{
	eMMC_PlatformResetPre();
    REG_FCIE_CLRBIT(FCIE_SD_MODE, BIT_SD_CLK_EN);
	REG_FCIE_CLRBIT(reg_ckg_fcie, BIT_FCIE_CLK_Gate|BIT_FCIE_CLK_MASK|BIT_FCIE_CLK_SRC_SEL);

	if(u32ClkParam==BIT_FCIE_CLK_300K)
	{
		REG_FCIE_SETBIT(reg_ckg_fcie, u32ClkParam<<BIT_FCIE_CLK_SHIFT | BIT_FCIE_CLK_SRC_SEL);
	}
	else
	{
		REG_FCIE_SETBIT(reg_ckg_fcie, u32ClkParam<<BIT_FCIE_CLK_SHIFT);
	}

    eMMC_PlatformResetPost();
	return eMMC_ST_SUCCESS;
}

void eMMC_set_WatchDog(U8 u8_IfEnable)
{
	u8_IfEnable = u8_IfEnable;
	// do nothing
}

void eMMC_reset_WatchDog(void)
{
	// do nothing
}

U32 eMMC_translate_DMA_address_Ex(U32 u32_DMAAddr, U32 u32_ByteCnt)
{
	u32_ByteCnt = u32_ByteCnt;
	#if 0
	flush_cache(u32_DMAAddr, u32_ByteCnt);
	#endif

	// Need to set bank 0x1006 16 bits addr 0x7A bit9 on to make below code work,
	// otherwise, always DMA to MIU0

	REG_FCIE_CLRBIT(FCIE_MMA_PRI_REG, BIT_MIU_SELECT_MASK);

	if(u32_DMAAddr < CONFIG_MIU1_BUSADDR) // MIU0
	{
		return (u32_DMAAddr - CONFIG_MIU0_BUSADDR);
	}
	else  // MIU1
	{
		REG_FCIE_SETBIT(FCIE_MMA_PRI_REG, BIT_MIU1_SELECT);
		return (u32_DMAAddr - CONFIG_MIU1_BUSADDR);
	}

	return u32_DMAAddr;
}

void eMMC_Invalidate_data_cache_buffer(U32 u32_addr, S32 s32_size)
{
	u32_addr = u32_addr;
	s32_size = s32_size;
//	flush_cache(u32_addr, s32_size);
}

void eMMC_flush_miu_pipe(void)
{

}

U32 eMMC_PlatformResetPre(void)
{

	return eMMC_ST_SUCCESS;
}

U32 eMMC_PlatformResetPost(void)
{

	return eMMC_ST_SUCCESS;
}

U32 eMMC_PlatformInit(void)
{
	eMMC_clock_setting(FCIE_SLOWEST_CLK);
    eMMC_pads_switch();
	return eMMC_ST_SUCCESS;
}

//eMMC_ALIGN0 eMMC_DRIVER g_eMMCDrv eMMC_ALIGN1;
//eMMC_ALIGN0 U8 gau8_eMMC_SectorBuf[eMMC_SECTOR_BUF_16KB] eMMC_ALIGN1; // 512 bytes
//eMMC_ALIGN0 U8 gau8_eMMC_PartInfoBuf[eMMC_SECTOR_512BYTE] eMMC_ALIGN1; // 512 bytes

#elif (defined(eMMC_DRV_MOONEY_BOOTRAM)&&eMMC_DRV_MOONEY_BOOTRAM)
void eMMC_DumpPadClk(void)
{

}

void eMMC_pads_switch(void)
{
    // fcie
    REG_FCIE_CLRBIT(FCIE_DDR_MODE, BIT_MACRO_MODE_MASK|BIT_CLK2_SEL|BIT1|BIT2|BIT3);
    REG_FCIE_SETBIT(FCIE_DDR_MODE,BIT_PAD_IN_SEL_SD|BIT_FALL_LATCH|BIT_CLK2_SEL);
    REG_FCIE_SETBIT(reg_emmcpll_0x1a, BIT10);
}

void eMMC_pads_switch_default(void)
{
    // fcie
    REG_FCIE_CLRBIT(FCIE_DDR_MODE, BIT_MACRO_MODE_MASK|BIT_CLK2_SEL|BIT1|BIT2|BIT3);
    REG_FCIE_SETBIT(FCIE_DDR_MODE,BIT_PAD_IN_SEL_SD|BIT_FALL_LATCH|BIT_CLK2_SEL|BIT1|BIT2|BIT3);
    REG_FCIE_SETBIT(reg_emmcpll_0x1a, BIT10);
}

U32 eMMC_clock_setting(U32 u32ClkParam)
{
	eMMC_PlatformResetPre();
    REG_FCIE_CLRBIT(FCIE_SD_MODE, BIT_SD_CLK_EN);
	REG_FCIE_CLRBIT(reg_ckg_fcie, BIT_FCIE_CLK_Gate|BIT_FCIE_CLK_MASK|BIT_FCIE_CLK_SRC_SEL);

	if(u32ClkParam==BIT_FCIE_CLK_300K)
	{
		REG_FCIE_SETBIT(reg_ckg_fcie, u32ClkParam<<BIT_FCIE_CLK_SHIFT | BIT_FCIE_CLK_SRC_SEL);
	}
	else
	{
		REG_FCIE_SETBIT(reg_ckg_fcie, u32ClkParam<<BIT_FCIE_CLK_SHIFT);
	}

    eMMC_PlatformResetPost();
	return eMMC_ST_SUCCESS;
}

void eMMC_set_WatchDog(U8 u8_IfEnable)
{
	u8_IfEnable = u8_IfEnable;
	// do nothing
}

void eMMC_reset_WatchDog(void)
{
	// do nothing
}

U32 eMMC_translate_DMA_address_Ex(U32 u32_DMAAddr, U32 u32_ByteCnt)
{
	u32_ByteCnt = u32_ByteCnt;
	#if 0
	flush_cache(u32_DMAAddr, u32_ByteCnt);
	#endif

	// Need to set bank 0x1006 16 bits addr 0x7A bit9 on to make below code work,
	// otherwise, always DMA to MIU0

	REG_FCIE_CLRBIT(FCIE_MMA_PRI_REG, BIT_MIU_SELECT_MASK);

	if(u32_DMAAddr < CONFIG_MIU1_BUSADDR) // MIU0
	{
		return (u32_DMAAddr - CONFIG_MIU0_BUSADDR);
	}
	else  // MIU1
	{
		REG_FCIE_SETBIT(FCIE_MMA_PRI_REG, BIT_MIU1_SELECT);
		return (u32_DMAAddr - CONFIG_MIU1_BUSADDR);
	}

	return u32_DMAAddr;
}

void eMMC_Invalidate_data_cache_buffer(U32 u32_addr, S32 s32_size)
{
	u32_addr = u32_addr;
	s32_size = s32_size;
//	flush_cache(u32_addr, s32_size);
}

void eMMC_flush_miu_pipe(void)
{

}

U32 eMMC_PlatformResetPre(void)
{

	return eMMC_ST_SUCCESS;
}

U32 eMMC_PlatformResetPost(void)
{

	return eMMC_ST_SUCCESS;
}

U32 eMMC_PlatformInit(void)
{
	eMMC_clock_setting(FCIE_SLOWEST_CLK);
    eMMC_pads_switch();
	return eMMC_ST_SUCCESS;
}

//eMMC_ALIGN0 eMMC_DRIVER g_eMMCDrv eMMC_ALIGN1;
//eMMC_ALIGN0 U8 gau8_eMMC_SectorBuf[eMMC_SECTOR_BUF_16KB] eMMC_ALIGN1; // 512 bytes
//eMMC_ALIGN0 U8 gau8_eMMC_PartInfoBuf[eMMC_SECTOR_512BYTE] eMMC_ALIGN1; // 512 bytes



#elif (defined(eMMC_DRV_MESSI_BOOTRAM)&&eMMC_DRV_MESSI_BOOTRAM)
void eMMC_DumpPadClk(void)
{

}

void eMMC_pads_switch(void)
{
    // fcie
    REG_FCIE_CLRBIT(FCIE_DDR_MODE, BIT_FALL_LATCH|BIT_PAD_IN_SEL_SD|BIT_CLK2_SEL|BIT_32BIT_MACRO_EN|BIT_DDR_EN|BIT_8BIT_MACRO_EN);
    REG_FCIE_SETBIT(FCIE_DDR_MODE,BIT_PAD_IN_SEL_SD|BIT_FALL_LATCH|BIT_CLK2_SEL);
    REG_FCIE_SETBIT(reg_emmc_test, BIT10);
}

void eMMC_pads_switch_default(void)
{
    // fcie
    REG_FCIE_CLRBIT(FCIE_DDR_MODE, BIT_FALL_LATCH|BIT_PAD_IN_SEL_SD|BIT_CLK2_SEL|BIT_32BIT_MACRO_EN|BIT_DDR_EN|BIT_8BIT_MACRO_EN);
    REG_FCIE_CLRBIT(reg_emmc_test, BIT10);
}

U32 eMMC_clock_setting(U32 u32ClkParam)
{
	eMMC_PlatformResetPre();
    REG_FCIE_CLRBIT(FCIE_SD_MODE, BIT_SD_CLK_EN);
	REG_FCIE_CLRBIT(reg_ckg_fcie, BIT_FCIE_CLK_Gate|BIT_FCIE_CLK_MASK|BIT_FCIE_CLK_SRC_SEL);

	if(u32ClkParam==BIT_FCIE_CLK_300K)
	{
		REG_FCIE_SETBIT(reg_ckg_fcie, u32ClkParam<<BIT_FCIE_CLK_SHIFT | BIT_FCIE_CLK_SRC_SEL);
	}
	else
	{
		REG_FCIE_SETBIT(reg_ckg_fcie, u32ClkParam<<BIT_FCIE_CLK_SHIFT);
	}

    eMMC_PlatformResetPost();
	return eMMC_ST_SUCCESS;
}

void eMMC_set_WatchDog(U8 u8_IfEnable)
{
	u8_IfEnable = u8_IfEnable;
	// do nothing
}

void eMMC_reset_WatchDog(void)
{
	// do nothing
}

U32 eMMC_translate_DMA_address_Ex(U32 u32_DMAAddr, U32 u32_ByteCnt)
{
	u32_ByteCnt = u32_ByteCnt;
	#if 0
	flush_cache(u32_DMAAddr, u32_ByteCnt);
	#endif

	// Need to set bank 0x1006 16 bits addr 0x7A bit9 on to make below code work,
	// otherwise, always DMA to MIU0

	REG_FCIE_CLRBIT(FCIE_MMA_PRI_REG, BIT_MIU_SELECT_MASK);

	if(u32_DMAAddr < CONFIG_MIU1_BUSADDR) // MIU0
	{
		return (u32_DMAAddr - CONFIG_MIU0_BUSADDR);
	}
	else  // MIU1
	{
		REG_FCIE_SETBIT(FCIE_MMA_PRI_REG, BIT_MIU1_SELECT);
		return (u32_DMAAddr - CONFIG_MIU1_BUSADDR);
	}

	return u32_DMAAddr;
}

void eMMC_Invalidate_data_cache_buffer(U32 u32_addr, S32 s32_size)
{
	u32_addr = u32_addr;
	s32_size = s32_size;
//	flush_cache(u32_addr, s32_size);
}

void eMMC_flush_miu_pipe(void)
{

}

U32 eMMC_PlatformResetPre(void)
{

	return eMMC_ST_SUCCESS;
}

U32 eMMC_PlatformResetPost(void)
{

	return eMMC_ST_SUCCESS;
}

U32 eMMC_PlatformInit(void)
{
	eMMC_clock_setting(FCIE_SLOWEST_CLK);
    eMMC_pads_switch();
	return eMMC_ST_SUCCESS;
}

//eMMC_ALIGN0 eMMC_DRIVER g_eMMCDrv eMMC_ALIGN1;
//eMMC_ALIGN0 U8 gau8_eMMC_SectorBuf[eMMC_SECTOR_BUF_16KB] eMMC_ALIGN1; // 512 bytes
//eMMC_ALIGN0 U8 gau8_eMMC_PartInfoBuf[eMMC_SECTOR_512BYTE] eMMC_ALIGN1; // 512 bytes

#elif (defined(eMMC_DRV_CELTICS_BOOTRAM)&&eMMC_DRV_CELTICS_BOOTRAM)
void eMMC_DumpPadClk(void)
{

}


U32 eMMC_clock_setting(U32 u32ClkParam)
{
	eMMC_PlatformResetPre();

	REG_FCIE_CLRBIT(reg_ckg_fcie, BIT_FCIE_CLK_Gate|BIT_FCIE_CLK_MASK|BIT_FCIE_CLK_SRC_SEL);

	if(u32ClkParam==BIT_FCIE_CLK_300K)
	{
		REG_FCIE_SETBIT(reg_ckg_fcie, u32ClkParam<<BIT_FCIE_CLK_SHIFT | BIT_FCIE_CLK_SRC_SEL);
	}
	else
	{
		REG_FCIE_SETBIT(reg_ckg_fcie, u32ClkParam<<BIT_FCIE_CLK_SHIFT);
	}

    eMMC_PlatformResetPost();
	return eMMC_ST_SUCCESS;
}

void eMMC_set_WatchDog(U8 u8_IfEnable)
{
	u8_IfEnable = u8_IfEnable;
	// do nothing
}

void eMMC_reset_WatchDog(void)
{
	// do nothing
}

U32 eMMC_translate_DMA_address_Ex(U32 u32_DMAAddr, U32 u32_ByteCnt)
{
	u32_ByteCnt = u32_ByteCnt;
	#if 0
	flush_cache(u32_DMAAddr, u32_ByteCnt);
	#endif

	// Need to set bank 0x1006 16 bits addr 0x7A bit9 on to make below code work,
	// otherwise, always DMA to MIU0

	REG_FCIE_CLRBIT(FCIE_MMA_PRI_REG, BIT_MIU_SELECT_MASK);

	if(u32_DMAAddr < CONFIG_MIU1_BUSADDR) // MIU0
	{
		return (u32_DMAAddr - CONFIG_MIU0_BUSADDR);
	}
	else if(u32_DMAAddr < CONFIG_MIU2_BUSADDR) // MIU1
	{
		REG_FCIE_SETBIT(FCIE_MMA_PRI_REG, BIT_MIU1_SELECT);
		return (u32_DMAAddr - CONFIG_MIU1_BUSADDR);
	}
	else // MIU2
	{
		REG_FCIE_SETBIT(FCIE_MMA_PRI_REG, BIT_MIU2_SELECT);
		return (u32_DMAAddr - CONFIG_MIU2_BUSADDR);
	}

	return u32_DMAAddr;
}

void eMMC_Invalidate_data_cache_buffer(U32 u32_addr, S32 s32_size)
{
	u32_addr = u32_addr;
	s32_size = s32_size;
//	flush_cache(u32_addr, s32_size);
}

void eMMC_flush_miu_pipe(void)
{

}

U32 eMMC_PlatformResetPre(void)
{

	return eMMC_ST_SUCCESS;
}

U32 eMMC_PlatformResetPost(void)
{

	return eMMC_ST_SUCCESS;
}

U32 eMMC_PlatformInit(void)
{
	eMMC_clock_setting(FCIE_SLOWEST_CLK);

	return eMMC_ST_SUCCESS;
}

//eMMC_ALIGN0 eMMC_DRIVER g_eMMCDrv eMMC_ALIGN1;
//eMMC_ALIGN0 U8 gau8_eMMC_SectorBuf[eMMC_SECTOR_BUF_16KB] eMMC_ALIGN1; // 512 bytes
//eMMC_ALIGN0 U8 gau8_eMMC_PartInfoBuf[eMMC_SECTOR_512BYTE] eMMC_ALIGN1; // 512 bytes
#elif (defined(eMMC_DRV_NADAL_BOOTRAM)&&eMMC_DRV_NADAL_BOOTRAM)
void eMMC_DumpPadClk(void)
{

}


U32 eMMC_clock_setting(U32 u32ClkParam)
{
	eMMC_PlatformResetPre();

	REG_FCIE_CLRBIT(reg_ckg_fcie, BIT_FCIE_CLK_Gate|BIT_FCIE_CLK_MASK);
	REG_FCIE_SETBIT(reg_ckg_fcie, u32ClkParam<<BIT_FCIE_CLK_SHIFT);
    REG_FCIE_SETBIT(reg_ckg_nfie_div4_en, BIT_FCIE_CLK_DIV4_EN);

    eMMC_PlatformResetPost();
	return eMMC_ST_SUCCESS;
}

void eMMC_set_WatchDog(U8 u8_IfEnable)
{
	u8_IfEnable = u8_IfEnable;
	// do nothing
}

void eMMC_reset_WatchDog(void)
{
	// do nothing
}

U32 eMMC_translate_DMA_address_Ex(U32 u32_DMAAddr, U32 u32_ByteCnt)
{
	u32_ByteCnt = u32_ByteCnt;
	#if 0
	flush_cache(u32_DMAAddr, u32_ByteCnt);
	#endif

	if(u32_DMAAddr < CONFIG_MIU1_BUSADDR) // MIU0
	{
		REG_FCIE_CLRBIT(FCIE_MIU_DMA_26_16, BIT_MIU1_SELECT);
		return (u32_DMAAddr - CONFIG_MIU0_BUSADDR);
	}
	else
	{	REG_FCIE_SETBIT(FCIE_MIU_DMA_26_16, BIT_MIU1_SELECT);
		return (u32_DMAAddr - CONFIG_MIU1_BUSADDR);
	}

	return u32_DMAAddr;
}

void eMMC_Invalidate_data_cache_buffer(U32 u32_addr, S32 s32_size)
{
	u32_addr = u32_addr;
	s32_size = s32_size;
//	flush_cache(u32_addr, s32_size);
}

void eMMC_flush_miu_pipe(void)
{

}

U32 eMMC_PlatformResetPre(void)
{

	return eMMC_ST_SUCCESS;
}

U32 eMMC_PlatformResetPost(void)
{

	return eMMC_ST_SUCCESS;
}

U32 eMMC_PlatformInit(void)
{
	eMMC_clock_setting(FCIE_SLOWEST_CLK);

	return eMMC_ST_SUCCESS;
}

//eMMC_ALIGN0 eMMC_DRIVER g_eMMCDrv eMMC_ALIGN1;
//eMMC_ALIGN0 U8 gau8_eMMC_SectorBuf[eMMC_SECTOR_BUF_16KB] eMMC_ALIGN1; // 512 bytes
//eMMC_ALIGN0 U8 gau8_eMMC_PartInfoBuf[eMMC_SECTOR_512BYTE] eMMC_ALIGN1; // 512 bytes

#elif (defined(eMMC_DRV_EINSTEIN_BOOTRAM)&&eMMC_DRV_EINSTEIN_BOOTRAM)
void eMMC_DumpPadClk(void)
{

}


U32 eMMC_clock_setting(U32 u32ClkParam)
{
	eMMC_PlatformResetPre();

	REG_FCIE_CLRBIT(reg_ckg_fcie, BIT_FCIE_CLK_Gate|BIT_FCIE_CLK_MASK);
	REG_FCIE_SETBIT(reg_ckg_fcie, u32ClkParam<<BIT_FCIE_CLK_SHIFT);


    eMMC_PlatformResetPost();
	return eMMC_ST_SUCCESS;
}

void eMMC_set_WatchDog(U8 u8_IfEnable)
{
	u8_IfEnable = u8_IfEnable;
	// do nothing
}

void eMMC_reset_WatchDog(void)
{
	// do nothing
}

U32 eMMC_translate_DMA_address_Ex(U32 u32_DMAAddr, U32 u32_ByteCnt)
{
	u32_ByteCnt = u32_ByteCnt;
	#if 0
	flush_cache(u32_DMAAddr, u32_ByteCnt);
	#endif

	if(u32_DMAAddr < CONFIG_MIU1_BUSADDR) // MIU0
	{
		REG_FCIE_CLRBIT(FCIE_MIU_DMA_26_16, BIT_MIU1_SELECT);
		return (u32_DMAAddr - CONFIG_MIU0_BUSADDR);
	}
	else
	{	REG_FCIE_SETBIT(FCIE_MIU_DMA_26_16, BIT_MIU1_SELECT);
		return (u32_DMAAddr - CONFIG_MIU1_BUSADDR);
	}

	return u32_DMAAddr;
}

void eMMC_Invalidate_data_cache_buffer(U32 u32_addr, S32 s32_size)
{
	u32_addr = u32_addr;
	s32_size = s32_size;
//	flush_cache(u32_addr, s32_size);
}

void eMMC_flush_miu_pipe(void)
{

}

U32 eMMC_PlatformResetPre(void)
{

	return eMMC_ST_SUCCESS;
}

U32 eMMC_PlatformResetPost(void)
{

	return eMMC_ST_SUCCESS;
}

U32 eMMC_PlatformInit(void)
{
	eMMC_clock_setting(FCIE_SLOWEST_CLK);

	return eMMC_ST_SUCCESS;
}

//eMMC_ALIGN0 eMMC_DRIVER g_eMMCDrv eMMC_ALIGN1;
//eMMC_ALIGN0 U8 gau8_eMMC_SectorBuf[eMMC_SECTOR_BUF_16KB] eMMC_ALIGN1; // 512 bytes
//eMMC_ALIGN0 U8 gau8_eMMC_PartInfoBuf[eMMC_SECTOR_512BYTE] eMMC_ALIGN1; // 512 bytes

#elif (defined(eMMC_DRV_NAPOLI_BOOTRAM)&&eMMC_DRV_NAPOLI_BOOTRAM)
void eMMC_DumpPadClk(void)
{

}

U32 eMMC_clock_setting(U32 u32ClkParam)
{
	eMMC_PlatformResetPre();
    REG_FCIE_CLRBIT(FCIE_SD_MODE, BIT_SD_CLK_EN);
    REG_FCIE_CLRBIT(reg_ckg_fcie, BIT_FCIE_CLK_Gate|BIT_FCIE_CLK_MASK|BIT_FCIE_CLK_SRC_SEL);
    if(u32ClkParam==BIT_FCIE_CLK_300K)
    {
        REG_FCIE_SETBIT(reg_ckg_fcie, BIT_FCIE_CLK_SRC_SEL|(u32ClkParam<<BIT_FCIE_CLK_SHIFT));
    }

    eMMC_PlatformResetPost();
	return eMMC_ST_SUCCESS;
}

void eMMC_set_WatchDog(U8 u8_IfEnable)
{
	u8_IfEnable = u8_IfEnable;
	// do nothing
}

void eMMC_reset_WatchDog(void)
{
	// do nothing
}

U32 eMMC_translate_DMA_address_Ex(U32 u32_DMAAddr, U32 u32_ByteCnt)
{
	u32_ByteCnt = u32_ByteCnt;
	#if 0
	flush_cache(u32_DMAAddr, u32_ByteCnt);
	#endif

	if(u32_DMAAddr < CONFIG_MIU1_BUSADDR) // MIU0
	{
		REG_FCIE_CLRBIT(FCIE_MIU_DMA_26_16, BIT_MIU1_SELECT);
		return (u32_DMAAddr - CONFIG_MIU0_BUSADDR);
	}
	else
	{	REG_FCIE_SETBIT(FCIE_MIU_DMA_26_16, BIT_MIU1_SELECT);
		return (u32_DMAAddr - CONFIG_MIU1_BUSADDR);
	}

	return u32_DMAAddr;
}

void eMMC_Invalidate_data_cache_buffer(U32 u32_addr, S32 s32_size)
{
	u32_addr = u32_addr;
	s32_size = s32_size;
//	flush_cache(u32_addr, s32_size);
}

void eMMC_flush_miu_pipe(void)
{

}

U32 eMMC_PlatformResetPre(void)
{

	return eMMC_ST_SUCCESS;
}

U32 eMMC_PlatformResetPost(void)
{

	return eMMC_ST_SUCCESS;
}

U32 eMMC_PlatformInit(void)
{
	eMMC_clock_setting(FCIE_SLOWEST_CLK);
	return eMMC_ST_SUCCESS;
}

//eMMC_ALIGN0 eMMC_DRIVER g_eMMCDrv eMMC_ALIGN1;
//eMMC_ALIGN0 U8 gau8_eMMC_SectorBuf[eMMC_SECTOR_BUF_16KB] eMMC_ALIGN1; // 512 bytes
//eMMC_ALIGN0 U8 gau8_eMMC_PartInfoBuf[eMMC_SECTOR_512BYTE] eMMC_ALIGN1; // 512 bytes

#elif (defined(eMMC_DRV_EINSTEIN3_BOOTRAM)&&eMMC_DRV_EINSTEIN3_BOOTRAM)
void eMMC_DumpPadClk(void)
{

}


U32 eMMC_clock_setting(U32 u32ClkParam)
{
	eMMC_PlatformResetPre();

	REG_FCIE_CLRBIT(reg_ckg_fcie, BIT_FCIE_CLK_Gate|BIT_FCIE_CLK_MASK);
	REG_FCIE_SETBIT(reg_ckg_fcie, u32ClkParam<<BIT_FCIE_CLK_SHIFT);


    eMMC_PlatformResetPost();
	return eMMC_ST_SUCCESS;
}

void eMMC_set_WatchDog(U8 u8_IfEnable)
{
	u8_IfEnable = u8_IfEnable;
	// do nothing
}

void eMMC_reset_WatchDog(void)
{
	// do nothing
}

U32 eMMC_translate_DMA_address_Ex(U32 u32_DMAAddr, U32 u32_ByteCnt)
{
	u32_ByteCnt = u32_ByteCnt;
	#if 0
	flush_cache(u32_DMAAddr, u32_ByteCnt);
	#endif

	if(u32_DMAAddr < CONFIG_MIU1_BUSADDR) // MIU0
	{
		REG_FCIE_CLRBIT(FCIE_MIU_DMA_26_16, BIT_MIU1_SELECT);
		return (u32_DMAAddr - CONFIG_MIU0_BUSADDR);
	}
	else
	{	REG_FCIE_SETBIT(FCIE_MIU_DMA_26_16, BIT_MIU1_SELECT);
		return (u32_DMAAddr - CONFIG_MIU1_BUSADDR);
	}

	return u32_DMAAddr;
}

void eMMC_Invalidate_data_cache_buffer(U32 u32_addr, S32 s32_size)
{
	u32_addr = u32_addr;
	s32_size = s32_size;
//	flush_cache(u32_addr, s32_size);
}

void eMMC_flush_miu_pipe(void)
{

}

U32 eMMC_PlatformResetPre(void)
{

	return eMMC_ST_SUCCESS;
}

U32 eMMC_PlatformResetPost(void)
{

	return eMMC_ST_SUCCESS;
}

U32 eMMC_PlatformInit(void)
{
	eMMC_clock_setting(FCIE_SLOWEST_CLK);

	return eMMC_ST_SUCCESS;
}

//eMMC_ALIGN0 eMMC_DRIVER g_eMMCDrv eMMC_ALIGN1;
//eMMC_ALIGN0 U8 gau8_eMMC_SectorBuf[eMMC_SECTOR_BUF_16KB] eMMC_ALIGN1; // 512 bytes
//eMMC_ALIGN0 U8 gau8_eMMC_PartInfoBuf[eMMC_SECTOR_512BYTE] eMMC_ALIGN1; // 512 bytes
#elif (defined(eMMC_DRV_MUNICH_BOOTRAM)&&eMMC_DRV_MUNICH_BOOTRAM)
void eMMC_DumpPadClk(void)
{

}

void eMMC_pads_switch(void)
{
    // fcie
    REG_FCIE_CLRBIT(FCIE_DDR_MODE, BIT_FALL_LATCH|BIT_PAD_IN_SEL_SD|BIT_32BIT_MACRO_EN|BIT_DDR_EN|BIT_8BIT_MACRO_EN);
    REG_FCIE_SETBIT(FCIE_DDR_MODE,BIT_PAD_IN_SEL_SD|BIT_FALL_LATCH);
}

void eMMC_pads_switch_default(void)
{
    // fcie
    REG_FCIE_CLRBIT(FCIE_DDR_MODE, BIT_FALL_LATCH|BIT_PAD_IN_SEL_SD|BIT_32BIT_MACRO_EN|BIT_DDR_EN|BIT_8BIT_MACRO_EN);
}

U32 eMMC_clock_setting(U32 u32ClkParam)
{
	eMMC_PlatformResetPre();
    REG_FCIE_CLRBIT(FCIE_SD_MODE, BIT_SD_CLK_EN);
	REG_FCIE_CLRBIT(reg_ckg_fcie, BIT_FCIE_CLK_Gate|BIT_FCIE_CLK_MASK|BIT_FCIE_CLK_SRC_SEL);

	if(u32ClkParam==BIT_FCIE_CLK_300K)
	{
		REG_FCIE_SETBIT(reg_ckg_fcie, u32ClkParam<<BIT_FCIE_CLK_SHIFT | BIT_FCIE_CLK_SRC_SEL);
	}
	else
	{
		REG_FCIE_SETBIT(reg_ckg_fcie, u32ClkParam<<BIT_FCIE_CLK_SHIFT);
	}


    eMMC_PlatformResetPost();
	return eMMC_ST_SUCCESS;
}

void eMMC_set_WatchDog(U8 u8_IfEnable)
{
	u8_IfEnable = u8_IfEnable;
	// do nothing
}

void eMMC_reset_WatchDog(void)
{
	// do nothing
}

U32 eMMC_translate_DMA_address_Ex(U32 u32_DMAAddr, U32 u32_ByteCnt)
{
	u32_ByteCnt = u32_ByteCnt;
	#if 0
	flush_cache(u32_DMAAddr, u32_ByteCnt);
	#endif

	// Need to set bank 0x1006 16 bits addr 0x7A bit9 on to make below code work,
	// otherwise, always DMA to MIU0

	REG_FCIE_CLRBIT(FCIE_MMA_PRI_REG, BIT_MIU_SELECT_MASK);

	if(u32_DMAAddr < CONFIG_MIU1_BUSADDR) // MIU0
	{
		return (u32_DMAAddr - CONFIG_MIU0_BUSADDR);
	}
	else  // MIU1
	{
		REG_FCIE_SETBIT(FCIE_MMA_PRI_REG, BIT_MIU1_SELECT);
		return (u32_DMAAddr - CONFIG_MIU1_BUSADDR);
	}

	return u32_DMAAddr;
}

void eMMC_Invalidate_data_cache_buffer(U32 u32_addr, S32 s32_size)
{
	u32_addr = u32_addr;
	s32_size = s32_size;
//	flush_cache(u32_addr, s32_size);
}

void eMMC_flush_miu_pipe(void)
{

}

U32 eMMC_PlatformResetPre(void)
{

	return eMMC_ST_SUCCESS;
}

U32 eMMC_PlatformResetPost(void)
{

	return eMMC_ST_SUCCESS;
}

U32 eMMC_PlatformInit(void)
{
	eMMC_clock_setting(FCIE_SLOWEST_CLK);
    eMMC_pads_switch();
	return eMMC_ST_SUCCESS;
}


#elif (defined(eMMC_DRV_MUSTANG_BOOTRAM)&&eMMC_DRV_MUSTANG_BOOTRAM)
void eMMC_DumpPadClk(void)
{

}

void eMMC_pads_switch(void)
{
    // fcie
    REG_FCIE_CLRBIT(FCIE_DDR_MODE, BIT_MACRO_MODE_MASK|BIT_CLK2_SEL|BIT1|BIT2|BIT3);
    REG_FCIE_SETBIT(FCIE_DDR_MODE,BIT_PAD_IN_SEL_SD|BIT_FALL_LATCH|BIT_CLK2_SEL);
    REG_FCIE_SETBIT(reg_emmcpll_0x1a, BIT10);
}

void eMMC_pads_switch_default(void)
{
    // fcie
    REG_FCIE_CLRBIT(FCIE_DDR_MODE, BIT_MACRO_MODE_MASK|BIT_CLK2_SEL|BIT1|BIT2|BIT3);
    REG_FCIE_SETBIT(FCIE_DDR_MODE,BIT_PAD_IN_SEL_SD|BIT_FALL_LATCH|BIT_CLK2_SEL|BIT1|BIT2|BIT3);
    REG_FCIE_SETBIT(reg_emmcpll_0x1a, BIT10);
}

U32 eMMC_clock_setting(U32 u32ClkParam)
{
	eMMC_PlatformResetPre();
    REG_FCIE_CLRBIT(FCIE_SD_MODE, BIT_SD_CLK_EN);
	REG_FCIE_CLRBIT(reg_ckg_fcie, BIT_FCIE_CLK_Gate|BIT_FCIE_CLK_MASK|BIT_FCIE_CLK_SRC_SEL);

	if(u32ClkParam==BIT_FCIE_CLK_300K)
	{
		REG_FCIE_SETBIT(reg_ckg_fcie, u32ClkParam<<BIT_FCIE_CLK_SHIFT | BIT_FCIE_CLK_SRC_SEL);
	}
	else
	{
		REG_FCIE_SETBIT(reg_ckg_fcie, u32ClkParam<<BIT_FCIE_CLK_SHIFT);
	}


    eMMC_PlatformResetPost();
	return eMMC_ST_SUCCESS;
}

void eMMC_set_WatchDog(U8 u8_IfEnable)
{
	u8_IfEnable = u8_IfEnable;
	// do nothing
}

void eMMC_reset_WatchDog(void)
{
	// do nothing
}

U32 eMMC_translate_DMA_address_Ex(U32 u32_DMAAddr, U32 u32_ByteCnt)
{
	u32_ByteCnt = u32_ByteCnt;
	#if 0
	flush_cache(u32_DMAAddr, u32_ByteCnt);
	#endif

	// Need to set bank 0x1006 16 bits addr 0x7A bit9 on to make below code work,
	// otherwise, always DMA to MIU0

	REG_FCIE_CLRBIT(FCIE_MMA_PRI_REG, BIT_MIU_SELECT_MASK);

	if(u32_DMAAddr < CONFIG_MIU1_BUSADDR) // MIU0
	{
		return (u32_DMAAddr - CONFIG_MIU0_BUSADDR);
	}
	else  // MIU1
	{
		REG_FCIE_SETBIT(FCIE_MMA_PRI_REG, BIT_MIU1_SELECT);
		return (u32_DMAAddr - CONFIG_MIU1_BUSADDR);
	}

	return u32_DMAAddr;
}

void eMMC_Invalidate_data_cache_buffer(U32 u32_addr, S32 s32_size)
{
	u32_addr = u32_addr;
	s32_size = s32_size;
//	flush_cache(u32_addr, s32_size);
}

void eMMC_flush_miu_pipe(void)
{

}

U32 eMMC_PlatformResetPre(void)
{

	return eMMC_ST_SUCCESS;
}

U32 eMMC_PlatformResetPost(void)
{

	return eMMC_ST_SUCCESS;
}

U32 eMMC_PlatformInit(void)
{
	eMMC_clock_setting(FCIE_SLOWEST_CLK);
    eMMC_pads_switch();
	return eMMC_ST_SUCCESS;
}

//eMMC_ALIGN0 eMMC_DRIVER g_eMMCDrv eMMC_ALIGN1;
//eMMC_ALIGN0 U8 gau8_eMMC_SectorBuf[eMMC_SECTOR_BUF_16KB] eMMC_ALIGN1; // 512 bytes
//eMMC_ALIGN0 U8 gau8_eMMC_PartInfoBuf[eMMC_SECTOR_512BYTE] eMMC_ALIGN1; // 512 bytes

#elif (defined(eMMC_DRV_MAXIM_BOOTRAM)&&eMMC_DRV_MAXIM_BOOTRAM)
void eMMC_DumpPadClk(void)
{

}

void eMMC_pads_switch(void)
{
    // fcie
    REG_FCIE_CLRBIT(FCIE_DDR_MODE, BIT_FALL_LATCH|BIT_PAD_IN_SEL_SD|BIT_CLK2_SEL|BIT_32BIT_MACRO_EN|BIT_DDR_EN|BIT_8BIT_MACRO_EN);
    REG_FCIE_SETBIT(FCIE_DDR_MODE,BIT_PAD_IN_SEL_SD|BIT_FALL_LATCH|BIT_CLK2_SEL);
    REG_FCIE_SETBIT(reg_emmc_test, BIT10);
}

void eMMC_pads_switch_default(void)
{
    // fcie
    REG_FCIE_CLRBIT(FCIE_DDR_MODE, BIT_FALL_LATCH|BIT_PAD_IN_SEL_SD|BIT_CLK2_SEL|BIT_32BIT_MACRO_EN|BIT_DDR_EN|BIT_8BIT_MACRO_EN);
    REG_FCIE_CLRBIT(reg_emmc_test, BIT10);
}

U32 eMMC_clock_setting(U32 u32ClkParam)
{
    U16 u16_Tmp = 0;

    eMMC_PlatformResetPre();

    REG_FCIE_CLRBIT(FCIE_SD_MODE, BIT_SD_CLK_EN);

    REG_FCIE_CLRBIT(reg_ckg_fcie, BIT_FCIE_CLK_SRC_SEL);
    REG_FCIE_R(reg_ckg_fcie, u16_Tmp);
    eMMC_hw_timer_delay(7);
    REG_FCIE_CLRBIT(reg_ckg_fcie, BIT_FCIE_CLK_Gate|BIT_FCIE_CLK_MASK);

    if(u32ClkParam==BIT_FCIE_CLK_300K)
        REG_FCIE_SETBIT(reg_ckg_fcie, u32ClkParam<<BIT_FCIE_CLK_SHIFT);

    REG_FCIE_SETBIT(reg_ckg_fcie, BIT_FCIE_CLK_SRC_SEL);

    eMMC_PlatformResetPost();

    return eMMC_ST_SUCCESS;
}

void eMMC_set_WatchDog(U8 u8_IfEnable)
{
	u8_IfEnable = u8_IfEnable;
	// do nothing
}

void eMMC_reset_WatchDog(void)
{
	// do nothing
}

U32 eMMC_translate_DMA_address_Ex(U32 u32_DMAAddr, U32 u32_ByteCnt)
{
	u32_ByteCnt = u32_ByteCnt;
	#if 0
	flush_cache(u32_DMAAddr, u32_ByteCnt);
	#endif

	// Need to set bank 0x1006 16 bits addr 0x7A bit9 on to make below code work,
	// otherwise, always DMA to MIU0

	REG_FCIE_CLRBIT(FCIE_MMA_PRI_REG, BIT_MIU_SELECT_MASK);

	if(u32_DMAAddr < CONFIG_MIU1_BUSADDR) // MIU0
	{
		return (u32_DMAAddr - CONFIG_MIU0_BUSADDR);
	}
	else  // MIU1
	{
		REG_FCIE_SETBIT(FCIE_MMA_PRI_REG, BIT_MIU1_SELECT);
		return (u32_DMAAddr - CONFIG_MIU1_BUSADDR);
	}

	return u32_DMAAddr;
}

void eMMC_Invalidate_data_cache_buffer(U32 u32_addr, S32 s32_size)
{
	u32_addr = u32_addr;
	s32_size = s32_size;
//	flush_cache(u32_addr, s32_size);
}

void eMMC_flush_miu_pipe(void)
{

}


U32 eMMC_PlatformResetPre(void)
{

	return eMMC_ST_SUCCESS;
}

U32 eMMC_PlatformResetPost(void)
{

	return eMMC_ST_SUCCESS;
}

U32 eMMC_PlatformInit(void)
{
	eMMC_clock_setting(FCIE_SLOWEST_CLK);
    eMMC_pads_switch();
	return eMMC_ST_SUCCESS;
}

#elif (defined(eMMC_DRV_M7621_BOOTRAM) && eMMC_DRV_M7621_BOOTRAM)
void putk_emmc(char c)
{
	while((REG_FCIE(REG_UART_LSR) & UART_LSR_THRE) != UART_LSR_THRE);
	REG_FCIE_W(REG_UART_TX, c);
	while((REG_FCIE(REG_UART_LSR) & (UART_LSR_TEMT | UART_LSR_THRE)) != (UART_LSR_TEMT | UART_LSR_THRE));
}

U32 eMMC_pads_switch(U8 u8Mode)
{
	eMMC_DRIVER *g_eMMCDrv = (eMMC_DRIVER *)g_eMMCDrv_array;

	g_eMMCDrv->u8_PadType = u8Mode;

	// chiptop
	REG_FCIE_CLRBIT(reg_chiptop_0x5A, BIT_SD_CONFIG);
	REG_FCIE_CLRBIT(reg_chiptop_0x6F, BIT_NAND_MODE);
	REG_FCIE_CLRBIT(reg_chiptop_0x6E, BIT_EMMC_CONFIG_MSK);
	REG_FCIE_SETBIT(reg_chiptop_0x6E, BIT_EMMC_CONFIG_EMMC_MODE_1);
	REG_FCIE_CLRBIT(reg_chiptop_0x50, BIT_ALL_PAD_IN);

	// fcie
	REG_FCIE_CLRBIT(FCIE_DDR_MODE, BIT_FALL_LATCH|BIT_PAD_IN_SEL_SD|BIT_CLK2_SEL|BIT_32BIT_MACRO_EN|BIT_DDR_EN|BIT_8BIT_MACRO_EN);

	// emmc_pll
	REG_FCIE_CLRBIT(reg_emmcpll_0x09, BIT0);
	REG_FCIE_CLRBIT(reg_emmcpll_0x1a, BIT10|BIT5|BIT4);
	REG_FCIE_CLRBIT(reg_emmcpll_0x1c, BIT8|BIT9);
	REG_FCIE_CLRBIT(reg_emmcpll_0x20, BIT9|BIT10);
	REG_FCIE_CLRBIT(reg_emmcpll_0x63, BIT0);
	REG_FCIE_CLRBIT(reg_emmcpll_0x68, BIT0|BIT1);
	REG_FCIE_CLRBIT(reg_emmcpll_0x69, BIT7|BIT6|BIT5|BIT4|BIT3);
	REG_FCIE_CLRBIT(reg_emmcpll_0x6a, BIT0|BIT1);
	REG_FCIE_W(reg_emmcpll_0x6b, 0x0000);
	REG_FCIE_CLRBIT(reg_emmcpll_0x6d, BIT0);
	REG_FCIE_CLRBIT(reg_emmcpll_0x70, BIT11|BIT10|BIT8);
	REG_FCIE_CLRBIT(reg_emmcpll_0x7f, BIT10|BIT9|BIT8|BIT3|BIT2|BIT1);

	switch (u8Mode) {

	case FCIE_eMMC_BYPASS:

		// fcie
		REG_FCIE_SETBIT(FCIE_DDR_MODE,BIT_PAD_IN_SEL_SD|BIT_FALL_LATCH|BIT_CLK2_SEL);

		// emmc_pll
		REG_FCIE_SETBIT(reg_emmcpll_0x1a, BIT10);
		REG_FCIE_W(reg_emmcpll_0x71, 0xFFFF);
		REG_FCIE_W(reg_emmcpll_0x73, 0xFFFF);

		break;

	case FCIE_eMMC_HS200:

		// fcie
		REG_FCIE_SETBIT(FCIE_DDR_MODE, BIT_32BIT_MACRO_EN);

		// emmc_pll
		REG_FCIE_CLRBIT(reg_emmcpll_0x03, 0x0FFF);
		REG_FCIE_SETBIT(reg_emmcpll_0x1a, BIT5|BIT4);
		REG_FCIE_SETBIT(reg_emmcpll_0x1c, BIT8);
		REG_FCIE_SETBIT(reg_emmcpll_0x20, BIT9|BIT10);
		REG_FCIE_SETBIT(reg_emmcpll_0x68, BIT0);
		REG_FCIE_SETBIT(reg_emmcpll_0x69, 4<<4);
		REG_FCIE_SETBIT(reg_emmcpll_0x6a, 2<<0);
		REG_FCIE_W(reg_emmcpll_0x6b, 0x0213);
		REG_FCIE_SETBIT(reg_emmcpll_0x70, BIT8);
		REG_FCIE_W(reg_emmcpll_0x71, 0xF800);
		REG_FCIE_W(reg_emmcpll_0x73, 0xFD00);
		#if defined(ENABLE_eMMC_AFIFO) && ENABLE_eMMC_AFIFO
		REG_FCIE_SETBIT(reg_emmcpll_0x70, BIT10|BIT11);
		#endif

		break;

	case FCIE_eMMC_HS400:

		// fcie
		REG_FCIE_SETBIT(FCIE_DDR_MODE, BIT_32BIT_MACRO_EN|BIT_DDR_EN);

		// emmc_pll
		REG_FCIE_CLRBIT(reg_emmcpll_0x03, 0x0F0F);
		REG_FCIE_SETBIT(reg_emmcpll_0x09, BIT0);
		REG_FCIE_SETBIT(reg_emmcpll_0x1a, BIT5|BIT4);
		REG_FCIE_SETBIT(reg_emmcpll_0x1c, BIT8);
		REG_FCIE_SETBIT(reg_emmcpll_0x20, BIT10);
		REG_FCIE_SETBIT(reg_emmcpll_0x63, BIT0);
		REG_FCIE_SETBIT(reg_emmcpll_0x68, BIT0|BIT1);
		REG_FCIE_SETBIT(reg_emmcpll_0x69, 6<<4);
		REG_FCIE_SETBIT(reg_emmcpll_0x6a, 2<<0);
		REG_FCIE_W(reg_emmcpll_0x6b, 0x0113);
		REG_FCIE_SETBIT(reg_emmcpll_0x70, BIT8);
		REG_FCIE_W(reg_emmcpll_0x71, 0xF800);
		REG_FCIE_W(reg_emmcpll_0x73, 0xFD00);
		#if defined(ENABLE_eMMC_AFIFO) && ENABLE_eMMC_AFIFO
		REG_FCIE_SETBIT(reg_emmcpll_0x70, BIT10|BIT11);
		REG_FCIE_SETBIT(reg_emmcpll_0x7f, BIT2);
		#endif

		break;

	default:
		return eMMC_ST_ERR_PARAMETER;
		break;
	}

	// 8 bits macro reset + 32 bits macro reset
	REG_FCIE_CLRBIT(reg_emmcpll_0x6f, BIT0|BIT1);
	REG_FCIE_SETBIT(reg_emmcpll_0x6f, BIT0|BIT1); // 8 bits macro reset + 32 bits macro reset

	return eMMC_ST_SUCCESS;
}

void eMMC_pads_switch_default(void)
{
    // fcie
    REG_FCIE_SETBIT(FCIE_DDR_MODE, BIT1|BIT2|BIT3);
}



U32 eMMC_pll_setting(U16 u16_ClkParam)
{
    U32 u32_value_reg_emmc_pll_pdiv;


    // 1. reset emmc pll
    REG_FCIE_SETBIT(reg_emmc_pll_reset,BIT0);
    REG_FCIE_CLRBIT(reg_emmc_pll_reset,BIT0);

    // 2. synth clock
    switch(u16_ClkParam)
    {
        case eMMC_PLL_CLK_200M: // 200M
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
        #if 0
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x22); // 20xMHz
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x8F5C);
        #else
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x24); // 195MHz
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x03D8);
        #endif
            u32_value_reg_emmc_pll_pdiv =1;// PostDIV: 2
            break;

        case eMMC_PLL_CLK_160M:
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x2B);
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x3333);
            u32_value_reg_emmc_pll_pdiv =1;
            break;

        case eMMC_PLL_CLK_140M:
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x31);
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x5F15);
            u32_value_reg_emmc_pll_pdiv = 1;
            break;

        case eMMC_PLL_CLK_120M:
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x39);
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x9999);
            u32_value_reg_emmc_pll_pdiv = 1;
            break;

        case eMMC_PLL_CLK_100M:
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x45);
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x1EB8);
            u32_value_reg_emmc_pll_pdiv = 1;
            break;

        case eMMC_PLL_CLK__86M:
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x28);
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x2FA0);
            u32_value_reg_emmc_pll_pdiv = 2;
            break;

        case eMMC_PLL_CLK__80M:
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x2B);
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x3333);
            u32_value_reg_emmc_pll_pdiv = 2;
            break;

        case eMMC_PLL_CLK__72M:
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x30);
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x0000);
            u32_value_reg_emmc_pll_pdiv = 2;
            break;

        case eMMC_PLL_CLK__62M:
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x37);
            REG_FCIE_SETBIT(reg_ddfset_15_00,0xBDEF);
            u32_value_reg_emmc_pll_pdiv = 2;
            break;

        case eMMC_PLL_CLK__52M:
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x42);
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x7627);
            u32_value_reg_emmc_pll_pdiv = 2;
            break;

        case eMMC_PLL_CLK__48M:
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x48);
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x0000);
            u32_value_reg_emmc_pll_pdiv = 2;
            break;

        case eMMC_PLL_CLK__40M:
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x2B);
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x3333);
            u32_value_reg_emmc_pll_pdiv = 4;
            break;

        case eMMC_PLL_CLK__36M:
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x30);
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x0000);
            u32_value_reg_emmc_pll_pdiv = 4;
            break;

        case eMMC_PLL_CLK__32M:
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x36);
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x0000);
            u32_value_reg_emmc_pll_pdiv = 4;
            break;

        case eMMC_PLL_CLK__27M:
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x40);
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x0000);
            u32_value_reg_emmc_pll_pdiv = 4;
            break;

        case eMMC_PLL_CLK__20M:
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x2B);
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x3333);
            u32_value_reg_emmc_pll_pdiv = 7;
            break;

        default:
            return eMMC_ST_ERR_UNKNOWN_CLK;
            break;
    }

    // 3. VCO clock ( loop N = 4 )
    REG_FCIE_CLRBIT(reg_emmcpll_fbdiv,0xffff);
    REG_FCIE_SETBIT(reg_emmcpll_fbdiv,0x6);

    // 4. 1X clock
    REG_FCIE_CLRBIT(reg_emmcpll_pdiv,BIT2|BIT1|BIT0);
    REG_FCIE_SETBIT(reg_emmcpll_pdiv,u32_value_reg_emmc_pll_pdiv);

    if(u16_ClkParam==eMMC_PLL_CLK__20M)
    {
        REG_FCIE_SETBIT(reg_emmc_pll_test, BIT10);
    }
    else
    {
        REG_FCIE_CLRBIT(reg_emmc_pll_test, BIT10);
    }

    eMMC_hw_timer_delay(HW_TIMER_DELAY_100us);

    return eMMC_ST_SUCCESS;
}



void HalEmmcPll_dll_setting(void)
{
    volatile U16 u16_reg;


    REG_FCIE_CLRBIT(reg_emmcpll_0x09, BIT0);

    // Reset eMMC_DLL
    REG_FCIE_SETBIT(REG_EMMC_PLL_RX30, BIT2);
    REG_FCIE_CLRBIT(REG_EMMC_PLL_RX30, BIT2);

    //DLL pulse width and phase
    REG_FCIE_W(REG_EMMC_PLL_RX01, 0x7F72);

    // DLL code
    REG_FCIE_W(REG_EMMC_PLL_RX32, 0xF200);

    // DLL calibration
    REG_FCIE_W(REG_EMMC_PLL_RX30, 0x3378);
    REG_FCIE_SETBIT(REG_EMMC_PLL_RX33, BIT15);

    // Wait 100us
    eMMC_hw_timer_delay(250);

    // Get hw dll0 code
    REG_FCIE_R(REG_EMMC_PLL_RX33, u16_reg);

    REG_FCIE_CLRBIT(REG_EMMC_PLL_RX34, (BIT10 - 1));
    // Set dw dll0 code
    REG_FCIE_SETBIT(REG_EMMC_PLL_RX34, u16_reg & 0x03FF);

    // Disable reg_hw_upcode_en
    REG_FCIE_CLRBIT(REG_EMMC_PLL_RX30, BIT9);

    // Clear reg_emmc_dll_test[7]
    REG_FCIE_CLRBIT(reg_emmcpll_0x02, BIT15);

    // Enable reg_rxdll_dline_en
    REG_FCIE_SETBIT(reg_emmcpll_0x09, BIT0);

}


U32 eMMC_clock_setting(U32 u32_ClkParam)
{
	U16 u16_Tmp;
	eMMC_DRIVER *g_eMMCDrv = (eMMC_DRIVER *)g_eMMCDrv_array;

	eMMC_PlatformResetPre();

	REG_FCIE_CLRBIT(FCIE_SD_MODE, BIT_SD_CLK_EN);

	REG_FCIE_CLRBIT(reg_ckg_fcie, BIT_FCIE_CLK_SRC_SEL);
	REG_FCIE_R(reg_ckg_fcie, u16_Tmp);
	eMMC_hw_timer_delay(7);
	REG_FCIE_CLRBIT(reg_ckg_fcie, BIT_FCIE_CLK_Gate|BIT_FCIE_CLK_MASK);

	if (u32_ClkParam & eMMC_PLL_FLAG) {

		eMMC_pll_setting(u32_ClkParam);

		if((g_eMMCDrv->u32_Flag & (FCIE_FLAG_SPEED_HS400 | FCIE_FLAG_DDR_MODE)) ==
			(FCIE_FLAG_SPEED_HS400 | FCIE_FLAG_DDR_MODE)) {

			REG_FCIE_SETBIT(reg_ckg_fcie, BIT_FCIE_CLK_EMMC_PLL_2X<<2);
			REG_FCIE_SETBIT(reg_ckg_fcie, BIT_FCIE_CLK_SRC_SEL);

			HalEmmcPll_dll_setting(); // tuning DLL setting
		} else {
			REG_FCIE_SETBIT(reg_ckg_fcie, BIT_FCIE_CLK_EMMC_PLL_1X<<2);
			REG_FCIE_SETBIT(reg_ckg_fcie, BIT_FCIE_CLK_SRC_SEL);
		}
	} else {
		REG_FCIE_SETBIT(reg_ckg_fcie, u32_ClkParam<<2);
		REG_FCIE_SETBIT(reg_ckg_fcie, BIT_FCIE_CLK_SRC_SEL);
	}

	eMMC_PlatformResetPost();

	return eMMC_ST_SUCCESS;
}

void eMMC_set_WatchDog(U8 u8_IfEnable)
{
	u8_IfEnable = u8_IfEnable;
	// do nothing
}

void eMMC_reset_WatchDog(void)
{
	// do nothing
}

U32 eMMC_translate_DMA_address_Ex(U32 u32_DMAAddr, U32 u32_ByteCnt)
{
	u32_ByteCnt = u32_ByteCnt;
	#if 0
	flush_cache(u32_DMAAddr, u32_ByteCnt);
	#endif

	// Need to set bank 0x1006 16 bits addr 0x7A bit9 on to make below code work,
	// otherwise, always DMA to MIU0

	REG_FCIE_CLRBIT(FCIE_MMA_PRI_REG, BIT_MIU_SELECT_MASK);
	if (u32_DMAAddr < CONFIG_MIU0_BUSADDR)
		return (u32_DMAAddr & 0xFFFF);
	else if (u32_DMAAddr < CONFIG_MIU1_BUSADDR)
		return (u32_DMAAddr - CONFIG_MIU0_BUSADDR);
	else {
		REG_FCIE_SETBIT(FCIE_MMA_PRI_REG, BIT_MIU1_SELECT);
		return (u32_DMAAddr - CONFIG_MIU1_BUSADDR);
	}

	return u32_DMAAddr;
}

void eMMC_Invalidate_data_cache_buffer(U32 u32_addr, S32 s32_size)
{
	u32_addr = u32_addr;
	s32_size = s32_size;
//	flush_cache(u32_addr, s32_size);
}

void eMMC_flush_miu_pipe(void)
{
}

U32 eMMC_PlatformResetPre(void)
{
	if ((REG_FCIE(reg_ckg_fcie_syn) & 0x3) != BIT0 )
	{
		REG_FCIE_CLRBIT(reg_ckg_fcie_syn, 0x3);
		REG_FCIE_SETBIT(reg_ckg_fcie_syn, BIT0);
	}

	return eMMC_ST_SUCCESS;
}

U32 eMMC_PlatformResetPost(void)
{
	return eMMC_ST_SUCCESS;
}


U32 eMMC_PlatformInit(void)
{
	if ((REG_FCIE(reg_emmcpll_0x1a) & BIT0) == 0) {
		//eMMC_debug(0, 0, "eMMC Err: not 1.8V IO setting\n");
		REG_FCIE_SETBIT(reg_emmcpll_0x1a, BIT0);// 1.8V must set this bit
		REG_FCIE_SETBIT(reg_emmcpll_0x1a, BIT2); // atop patch
		REG_FCIE_CLRBIT(reg_emmcpll_0x45, 0xffff);// 1.8V must set this bit
		REG_FCIE_SETBIT(reg_emmcpll_0x45, 0xf<<8);// 1.8V must set this bit
	}

	eMMC_clock_setting(FCIE_SLOWEST_CLK);
	eMMC_pads_switch(FCIE_eMMC_BYPASS);

	return eMMC_ST_SUCCESS;
}

#elif (defined(eMMC_DRV_M7821_BOOTRAM) && eMMC_DRV_M7821_BOOTRAM)
void eMMC_DumpPadClk(void)
{

}

void eMMC_pads_switch(void)
{
    // fcie
    REG_FCIE_CLRBIT(FCIE_DDR_MODE, BIT_FALL_LATCH|BIT_PAD_IN_SEL_SD|BIT_CLK2_SEL|BIT_32BIT_MACRO_EN|BIT_DDR_EN|BIT_8BIT_MACRO_EN|BIT1|BIT2|BIT3);
    REG_FCIE_SETBIT(FCIE_DDR_MODE,BIT_PAD_IN_SEL_SD|BIT_FALL_LATCH|BIT_CLK2_SEL);
    REG_FCIE_SETBIT(reg_emmc_test, BIT10);
}

void eMMC_pads_switch_default(void)
{
    // fcie
    REG_FCIE_SETBIT(FCIE_DDR_MODE, BIT1|BIT2|BIT3);
}

U32 eMMC_clock_setting(U32 u32ClkParam)
{
    U16 u16_Tmp = 0;

    eMMC_PlatformResetPre();

    REG_FCIE_CLRBIT(FCIE_SD_MODE, BIT_SD_CLK_EN);

    REG_FCIE_CLRBIT(reg_ckg_fcie, BIT_FCIE_CLK_SRC_SEL);
    REG_FCIE_R(reg_ckg_fcie, u16_Tmp);
    eMMC_hw_timer_delay(7);
    REG_FCIE_CLRBIT(reg_ckg_fcie, BIT_FCIE_CLK_Gate|BIT_FCIE_CLK_MASK);

    if (u32ClkParam == BIT_FCIE_CLK_300K)
        REG_FCIE_SETBIT(reg_ckg_fcie, u32ClkParam<<BIT_FCIE_CLK_SHIFT);

    REG_FCIE_SETBIT(reg_ckg_fcie, BIT_FCIE_CLK_SRC_SEL);

    eMMC_PlatformResetPost();

    return eMMC_ST_SUCCESS;
}

void eMMC_set_WatchDog(U8 u8_IfEnable)
{
	u8_IfEnable = u8_IfEnable;
	// do nothing
}

void eMMC_reset_WatchDog(void)
{
	// do nothing
}

U32 eMMC_translate_DMA_address_Ex(U32 u32_DMAAddr, U32 u32_ByteCnt)
{
	u32_ByteCnt = u32_ByteCnt;
	#if 0
	flush_cache(u32_DMAAddr, u32_ByteCnt);
	#endif

	// Need to set bank 0x1006 16 bits addr 0x7A bit9 on to make below code work,
	// otherwise, always DMA to MIU0

	REG_FCIE_CLRBIT(FCIE_MMA_PRI_REG, BIT_MIU_SELECT_MASK);

	if(u32_DMAAddr < CONFIG_MIU1_BUSADDR) // MIU0
	{
		return (u32_DMAAddr - CONFIG_MIU0_BUSADDR);
	}
	else  // MIU1
	{
		REG_FCIE_SETBIT(FCIE_MMA_PRI_REG, BIT_MIU1_SELECT);
		return (u32_DMAAddr - CONFIG_MIU1_BUSADDR);
	}

	return u32_DMAAddr;
}

void eMMC_Invalidate_data_cache_buffer(U32 u32_addr, S32 s32_size)
{
	u32_addr = u32_addr;
	s32_size = s32_size;
//	flush_cache(u32_addr, s32_size);
}

void eMMC_flush_miu_pipe(void)
{
}

U32 eMMC_PlatformResetPre(void)
{
	return eMMC_ST_SUCCESS;
}

U32 eMMC_PlatformResetPost(void)
{
	return eMMC_ST_SUCCESS;
}


U32 eMMC_PlatformInit(void)
{
	eMMC_clock_setting(FCIE_SLOWEST_CLK);
	eMMC_pads_switch();
	return eMMC_ST_SUCCESS;
}


#elif (defined(eMMC_DRV_M5621_BOOTRAM)&&eMMC_DRV_M5621_BOOTRAM)
void putk_emmc(char c)
{
	while((REG_FCIE(REG_UART_LSR) & UART_LSR_THRE) != UART_LSR_THRE);
	REG_FCIE_W(REG_UART_TX, c);
	while((REG_FCIE(REG_UART_LSR) & (UART_LSR_TEMT | UART_LSR_THRE)) != (UART_LSR_TEMT | UART_LSR_THRE));

}

void eMMC_DumpPadClk(void)
{

}

U32 eMMC_pads_switch(U8 u8Mode)
{
    eMMC_DRIVER *g_eMMCDrv = (eMMC_DRIVER *)g_eMMCDrv_array;

    g_eMMCDrv->u8_PadType = u8Mode;

    // chiptop
    REG_FCIE_CLRBIT(reg_chiptop_0x11, BIT_SD1_CONFIG_MSK);
    REG_FCIE_CLRBIT(reg_chiptop_0x57, BIT_SD2_CONFIG_MSK);

    REG_FCIE_CLRBIT(reg_chiptop_0x50, BIT_ALL_PAD_IN);

    REG_FCIE_CLRBIT(reg_emmc_config2, BIT9);


    // fcie
    REG_FCIE_CLRBIT(FCIE_DDR_MODE, BIT_FALL_LATCH|BIT_PAD_IN_SEL_SD|BIT_32BIT_MACRO_EN|BIT_CLK2_SEL|BIT_DDR_EN|BIT_8BIT_MACRO_EN);
    // emmc_pll
    REG_FCIE_CLRBIT(reg_emmcpll_0x09, BIT0);
    REG_FCIE_CLRBIT(reg_emmcpll_0x1a, BIT4|BIT5|BIT10);
    REG_FCIE_CLRBIT(reg_emmcpll_0x1c, BIT8|BIT9);               //meta issue of clk4x and skew
    REG_FCIE_CLRBIT(reg_emmcpll_0x20, BIT9|BIT10);                // reg_sel_internal & CMD

    REG_FCIE_CLRBIT(reg_emmcpll_0x63, BIT0);                // reg_use_rxdll

    REG_FCIE_CLRBIT(reg_emmcpll_0x68, BIT0|BIT1);           // reg_emmc_en | reg_emmc_ddr_en

    REG_FCIE_CLRBIT(reg_emmcpll_0x69, BIT7|BIT6|BIT5|BIT4); // reg_tune_shot_offset

    REG_FCIE_CLRBIT(reg_emmcpll_0x6a, BIT0|BIT1);           // reg_io_bus_wid

    REG_FCIE_W(reg_emmcpll_0x6b, 0x0000);                   // reg_dqs_page_no

    REG_FCIE_CLRBIT(reg_emmcpll_0x6d, BIT0);                // reg_ddr_io_mode

    REG_FCIE_CLRBIT(reg_emmcpll_0x70, BIT8);                // reg_sel_flash_32bif
    REG_FCIE_CLRBIT(reg_emmcpll_0x70, BIT10|BIT11);         // reg_afifo_enable


    //REG_FCIE_SETBIT(reg_emmcpll_0x74, BIT15);               // reg_atop_byp_rx_en
    REG_FCIE_CLRBIT(reg_emmcpll_0x7f, BIT1|BIT2|BIT3|BIT8|BIT9|BIT10|BIT11);      //reg_afifo_wen
    if((REG_FCIE_U16(reg_stat_bond_sw)& BIT_BOND_BGA)== BIT_BOND_BGA)
    {
        // Set emmc pad mode 1
        REG_FCIE_SETBIT(reg_emmc_config, BIT_EMMC_CONFIG_EMMC_MODE_1);

        REG_FCIE_SETBIT(reg_chiptop_0x4F, BIT_reg_emmc_rstz_en);
    }
    else
    {
        // Set emmc pad mode pcm_a
        REG_FCIE_SETBIT(reg_emmc_config2, BIT_EMMC_MODE_PCM_A);    //bit9 for sd config eco
        //Set pe for eMMC IO
        REG_FCIE_SETBIT(reg_pcm_pe_23_16,0xff);
        //Set ps for eMMC GPIO
        REG_FCIE_CLRBIT(reg_emmcpll_0x49, BIT2);

        REG_FCIE_CLRBIT(reg_chiptop_0x4F, BIT_reg_emmc_rstz_en);
    }

    switch (u8Mode) {

    case FCIE_eMMC_BYPASS:

        // fcie
        REG_FCIE_SETBIT(FCIE_DDR_MODE,BIT_PAD_IN_SEL_SD|BIT_FALL_LATCH|BIT_CLK2_SEL);

        // emmc_pll
        REG_FCIE_SETBIT(reg_emmcpll_0x1a, BIT10);
        REG_FCIE_W(reg_emmcpll_0x71, 0xFFFF);
        REG_FCIE_W(reg_emmcpll_0x73, 0xFFFF);
        break;

    case FCIE_eMMC_HS200:

        // fcie
        REG_FCIE_SETBIT(FCIE_DDR_MODE, BIT_32BIT_MACRO_EN);

        // emmc_pll
        REG_FCIE_CLRBIT(reg_emmcpll_0x03, 0x0FFF);
        REG_FCIE_SETBIT(reg_emmcpll_0x1a, BIT5|BIT4);
        REG_FCIE_SETBIT(reg_emmcpll_0x1c, BIT8);
        REG_FCIE_SETBIT(reg_emmcpll_0x20, BIT9|BIT10);
        REG_FCIE_SETBIT(reg_emmcpll_0x68, BIT0);
        REG_FCIE_SETBIT(reg_emmcpll_0x69, 4<<4);
		REG_FCIE_CLRBIT(reg_emmcpll_0x69, BIT3);
        REG_FCIE_SETBIT(reg_emmcpll_0x6a, 2<<0);
        REG_FCIE_W(reg_emmcpll_0x6b, 0x0213);
        REG_FCIE_SETBIT(reg_emmcpll_0x70, BIT8);
        REG_FCIE_W(reg_emmcpll_0x71, 0xF800);
        REG_FCIE_W(reg_emmcpll_0x73, 0xFD00);
        break;

    case FCIE_eMMC_HS400:

        // fcie
        REG_FCIE_SETBIT(FCIE_DDR_MODE, BIT_32BIT_MACRO_EN|BIT_DDR_EN);

        // emmc_pll
        REG_FCIE_CLRBIT(reg_emmcpll_0x03, 0x0F0F);
        REG_FCIE_SETBIT(reg_emmcpll_0x09, BIT0);
        REG_FCIE_SETBIT(reg_emmcpll_0x1a, BIT5|BIT4);
        REG_FCIE_SETBIT(reg_emmcpll_0x1c, BIT8);
        REG_FCIE_SETBIT(reg_emmcpll_0x20, BIT10);
        REG_FCIE_SETBIT(reg_emmcpll_0x63, BIT0);
        REG_FCIE_SETBIT(reg_emmcpll_0x68, BIT0|BIT1);
		REG_FCIE_CLRBIT(reg_emmcpll_0x69, BIT3);
        REG_FCIE_SETBIT(reg_emmcpll_0x69, 6<<4);
        REG_FCIE_SETBIT(reg_emmcpll_0x6a, 2<<0);
        REG_FCIE_W(reg_emmcpll_0x6b, 0x0113);
        REG_FCIE_SETBIT(reg_emmcpll_0x70, BIT8);
        REG_FCIE_W(reg_emmcpll_0x71, 0xF800);
        REG_FCIE_W(reg_emmcpll_0x73, 0xFD00);
        break;

    default:
        return eMMC_ST_ERR_PARAMETER;
        break;
    }
    // 8 bits macro reset + 32 bits macro reset
    REG_FCIE_CLRBIT(reg_emmcpll_0x6f, BIT0|BIT1);
    REG_FCIE_SETBIT(reg_emmcpll_0x6f, BIT0|BIT1); // 8 bits macro reset + 32 bits macro reset
    return eMMC_ST_SUCCESS;

}

void eMMC_pads_switch_default(void)
{
    // fcie
    REG_FCIE_SETBIT(FCIE_DDR_MODE, BIT1|BIT2|BIT3);
}

U32 eMMC_pll_setting(U16 u16_ClkParam)
{
    U32 u32_value_reg_emmc_pll_pdiv;


    // 1. reset emmc pll
    REG_FCIE_SETBIT(reg_emmc_pll_reset,BIT0);
    REG_FCIE_CLRBIT(reg_emmc_pll_reset,BIT0);

    // 2. synth clock
    switch(u16_ClkParam)
    {
        case eMMC_PLL_CLK_200M: // 200M
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
        #if 0
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x22); // 20xMHz
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x8F5C);
        #else
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x24); // 195MHz
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x03D8);
        #endif
            u32_value_reg_emmc_pll_pdiv =1;// PostDIV: 2
            break;

        case eMMC_PLL_CLK_160M:
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x2B);
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x3333);
            u32_value_reg_emmc_pll_pdiv =1;
            break;

        case eMMC_PLL_CLK_140M:
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x31);
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x5F15);
            u32_value_reg_emmc_pll_pdiv = 1;
            break;

        case eMMC_PLL_CLK_120M:
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x39);
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x9999);
            u32_value_reg_emmc_pll_pdiv = 1;
            break;

        case eMMC_PLL_CLK_100M:
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x45);
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x1EB8);
            u32_value_reg_emmc_pll_pdiv = 1;
            break;

        case eMMC_PLL_CLK__86M:
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x28);
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x2FA0);
            u32_value_reg_emmc_pll_pdiv = 2;
            break;

        case eMMC_PLL_CLK__80M:
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x2B);
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x3333);
            u32_value_reg_emmc_pll_pdiv = 2;
            break;

        case eMMC_PLL_CLK__72M:
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x30);
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x0000);
            u32_value_reg_emmc_pll_pdiv = 2;
            break;

        case eMMC_PLL_CLK__62M:
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x37);
            REG_FCIE_SETBIT(reg_ddfset_15_00,0xBDEF);
            u32_value_reg_emmc_pll_pdiv = 2;
            break;

        case eMMC_PLL_CLK__52M:
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x42);
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x7627);
            u32_value_reg_emmc_pll_pdiv = 2;
            break;

        case eMMC_PLL_CLK__48M:
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x48);
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x0000);
            u32_value_reg_emmc_pll_pdiv = 2;
            break;

        case eMMC_PLL_CLK__40M:
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x2B);
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x3333);
            u32_value_reg_emmc_pll_pdiv = 4;
            break;

        case eMMC_PLL_CLK__36M:
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x30);
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x0000);
            u32_value_reg_emmc_pll_pdiv = 4;
            break;

        case eMMC_PLL_CLK__32M:
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x36);
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x0000);
            u32_value_reg_emmc_pll_pdiv = 4;
            break;

        case eMMC_PLL_CLK__27M:
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x40);
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x0000);
            u32_value_reg_emmc_pll_pdiv = 4;
            break;

        case eMMC_PLL_CLK__20M:
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x2B);
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x3333);
            u32_value_reg_emmc_pll_pdiv = 7;
            break;

        default:
            return eMMC_ST_ERR_UNKNOWN_CLK;
            break;
    }

    // 3. VCO clock ( loop N = 4 )
    REG_FCIE_CLRBIT(reg_emmcpll_fbdiv,0xffff);
    REG_FCIE_SETBIT(reg_emmcpll_fbdiv,0x6);

    // 4. 1X clock
    REG_FCIE_CLRBIT(reg_emmcpll_pdiv,BIT2|BIT1|BIT0);
    REG_FCIE_SETBIT(reg_emmcpll_pdiv,u32_value_reg_emmc_pll_pdiv);

    if(u16_ClkParam==eMMC_PLL_CLK__20M)
    {
        REG_FCIE_SETBIT(reg_emmc_pll_test, BIT10);
    }
    else
    {
        REG_FCIE_CLRBIT(reg_emmc_pll_test, BIT10);
    }

    eMMC_hw_timer_delay(HW_TIMER_DELAY_100us);

    return eMMC_ST_SUCCESS;
}

void HalEmmcPll_dll_setting(void)
{
    volatile U16 u16_reg;


    REG_FCIE_CLRBIT(reg_emmcpll_0x09, BIT0);

    // Reset eMMC_DLL
    REG_FCIE_SETBIT(REG_EMMC_PLL_RX30, BIT2);
    REG_FCIE_CLRBIT(REG_EMMC_PLL_RX30, BIT2);

    //DLL pulse width and phase
    REG_FCIE_W(REG_EMMC_PLL_RX01, 0x7F72);

    // DLL code
    REG_FCIE_W(REG_EMMC_PLL_RX32, 0xF200);

    // DLL calibration
    REG_FCIE_W(REG_EMMC_PLL_RX30, 0x3378);
    REG_FCIE_SETBIT(REG_EMMC_PLL_RX33, BIT15);

    // Wait 100us
    eMMC_hw_timer_delay(250);

    // Get hw dll0 code
    REG_FCIE_R(REG_EMMC_PLL_RX33, u16_reg);

    REG_FCIE_CLRBIT(REG_EMMC_PLL_RX34, (BIT10 - 1));
    // Set dw dll0 code
    REG_FCIE_SETBIT(REG_EMMC_PLL_RX34, u16_reg & 0x03FF);

    // Disable reg_hw_upcode_en
    REG_FCIE_CLRBIT(REG_EMMC_PLL_RX30, BIT9);

    // Clear reg_emmc_dll_test[7]
    REG_FCIE_CLRBIT(reg_emmcpll_0x02, BIT15);

    // Enable reg_rxdll_dline_en
    REG_FCIE_SETBIT(reg_emmcpll_0x09, BIT0);

}


U32 eMMC_clock_setting(U16 u16_ClkParam)
{
    U16 u16_Tmp;
    eMMC_DRIVER *g_eMMCDrv = (eMMC_DRIVER *)g_eMMCDrv_array;

    eMMC_PlatformResetPre();

    REG_FCIE_CLRBIT(FCIE_SD_MODE, BIT_SD_CLK_EN);

    REG_FCIE_CLRBIT(reg_ckg_fcie, BIT_FCIE_CLK_SRC_SEL);
    REG_FCIE_R(reg_ckg_fcie, u16_Tmp);
    eMMC_hw_timer_delay(7);
    REG_FCIE_CLRBIT(reg_ckg_fcie, BIT_FCIE_CLK_Gate|BIT_FCIE_CLK_MASK);

    if (u16_ClkParam & eMMC_PLL_FLAG) {

        eMMC_pll_setting(u16_ClkParam);

        if((g_eMMCDrv->u32_Flag & (FCIE_FLAG_SPEED_HS400 | FCIE_FLAG_DDR_MODE)) ==
            (FCIE_FLAG_SPEED_HS400 | FCIE_FLAG_DDR_MODE)) {

            REG_FCIE_SETBIT(reg_ckg_fcie, BIT_FCIE_CLK_EMMC_PLL_2X<<2);
            REG_FCIE_SETBIT(reg_ckg_fcie, BIT_FCIE_CLK_SRC_SEL);

            HalEmmcPll_dll_setting(); // tuning DLL setting
        } else {
            REG_FCIE_SETBIT(reg_ckg_fcie, BIT_FCIE_CLK_EMMC_PLL_1X<<2);
            REG_FCIE_SETBIT(reg_ckg_fcie, BIT_FCIE_CLK_SRC_SEL);
        }
    } else {
        REG_FCIE_SETBIT(reg_ckg_fcie, u16_ClkParam<<2);
        REG_FCIE_SETBIT(reg_ckg_fcie, BIT_FCIE_CLK_SRC_SEL);
    }

    eMMC_PlatformResetPost();

	return eMMC_ST_SUCCESS;
}

void eMMC_set_WatchDog(U8 u8_IfEnable)
{
	u8_IfEnable = u8_IfEnable;
	// do nothing
}

void eMMC_reset_WatchDog(void)
{
	// do nothing
}

U32 eMMC_translate_DMA_address_Ex(U32 u32_DMAAddr, U32 u32_ByteCnt)
{
	u32_ByteCnt = u32_ByteCnt;
	#if 0
	flush_cache(u32_DMAAddr, u32_ByteCnt);
	#endif

	// Need to set bank 0x1006 16 bits addr 0x7A bit9 on to make below code work,
	// otherwise, always DMA to MIU0

	REG_FCIE_CLRBIT(FCIE_MMA_PRI_REG, BIT_MIU_SELECT_MASK);

	if(u32_DMAAddr < CONFIG_MIU1_BUSADDR) // MIU0
	{
		return (u32_DMAAddr - CONFIG_MIU0_BUSADDR);
	}
	else  // MIU1
	{
		REG_FCIE_SETBIT(FCIE_MMA_PRI_REG, BIT_MIU1_SELECT);
		return (u32_DMAAddr - CONFIG_MIU1_BUSADDR);
	}

	return u32_DMAAddr;
}

void eMMC_Invalidate_data_cache_buffer(U32 u32_addr, S32 s32_size)
{
	u32_addr = u32_addr;
	s32_size = s32_size;
//	flush_cache(u32_addr, s32_size);
}

void eMMC_flush_miu_pipe(void)
{
}

U32 eMMC_PlatformResetPre(void)
{
    if ((REG_FCIE(reg_ckg_fcie_syn) & BIT_CKG_FCIE_SYN_MASK) != BIT_CKG_FCIE_SYN )
    {
        REG_FCIE_CLRBIT(reg_ckg_fcie_syn, BIT_CKG_FCIE_SYN_MASK);
        REG_FCIE_SETBIT(reg_ckg_fcie_syn, BIT_CKG_FCIE_SYN);
    }

	return eMMC_ST_SUCCESS;
}

U32 eMMC_PlatformResetPost(void)
{
	return eMMC_ST_SUCCESS;
}


U32 eMMC_PlatformInit(void)
{
    if ((REG_FCIE(reg_emmcpll_0x1a) & BIT0) == 0) {
        //eMMC_debug(0, 0, "eMMC Err: not 1.8V IO setting\n");
        REG_FCIE_SETBIT(reg_emmcpll_0x1a, BIT0);// 1.8V must set this bit
        REG_FCIE_SETBIT(reg_emmcpll_0x1a, BIT2); // atop patch
        REG_FCIE_CLRBIT(reg_emmcpll_0x45, 0xffff);// 1.8V must set this bit
        REG_FCIE_SETBIT(reg_emmcpll_0x45, 0xf<<8);// 1.8V must set this bit
    }

    eMMC_clock_setting(FCIE_SLOWEST_CLK);
    eMMC_pads_switch(FCIE_eMMC_BYPASS);

    return eMMC_ST_SUCCESS;
}

#elif (defined(eMMC_DRV_MAINZ_BOOTRAM)&&eMMC_DRV_MAINZ_BOOTRAM)
void eMMC_DumpPadClk(void)
{

}

void eMMC_pads_switch(void)
{
    // fcie
    REG_FCIE_CLRBIT(FCIE_DDR_MODE, BIT_FALL_LATCH|BIT_PAD_IN_SEL_SD|BIT_CLK2_SEL|BIT_32BIT_MACRO_EN|BIT_DDR_EN|BIT_8BIT_MACRO_EN);
    REG_FCIE_SETBIT(FCIE_DDR_MODE, BIT_PAD_IN_SEL_SD|BIT_FALL_LATCH|BIT_CLK2_SEL);
    REG_FCIE_SETBIT(reg_emmc_test, BIT10);
}

void eMMC_pads_switch_default(void)
{
    // fcie
    #if 0
    REG_FCIE_CLRBIT(FCIE_DDR_MODE, BIT_FALL_LATCH|BIT_PAD_IN_SEL_SD|BIT_CLK2_SEL|BIT_32BIT_MACRO_EN|BIT_DDR_EN|BIT_8BIT_MACRO_EN);
    REG_FCIE_SETBIT(FCIE_DDR_MODE,BIT_PAD_IN_SEL_SD|BIT_FALL_LATCH|BIT_CLK2_SEL|BIT1|BIT2|BIT3);
    REG_FCIE_SETBIT(reg_emmc_test, BIT10);
    #else
    eMMC_pads_switch();
    REG_FCIE_SETBIT(FCIE_DDR_MODE, BIT1|BIT2|BIT3);
    #endif
}

U32 eMMC_clock_setting(U32 u32ClkParam)
{
	eMMC_PlatformResetPre();
    REG_FCIE_CLRBIT(FCIE_SD_MODE, BIT_SD_CLK_EN);

	REG_FCIE_CLRBIT(reg_ckg_fcie, BIT_FCIE_CLK_Gate|BIT_FCIE_CLK_MASK|BIT_FCIE_CLK_SRC_SEL);

	if(u32ClkParam==BIT_FCIE_CLK_300K)
	{
		REG_FCIE_SETBIT(reg_ckg_fcie, u32ClkParam<<BIT_FCIE_CLK_SHIFT | BIT_FCIE_CLK_SRC_SEL);
	}
	else
	{
		REG_FCIE_SETBIT(reg_ckg_fcie, u32ClkParam<<BIT_FCIE_CLK_SHIFT);
	}

    eMMC_PlatformResetPost();
	return eMMC_ST_SUCCESS;
}

void eMMC_set_WatchDog(U8 u8_IfEnable)
{
	u8_IfEnable = u8_IfEnable;
	// do nothing
}

void eMMC_reset_WatchDog(void)
{
	// do nothing
}

U32 eMMC_translate_DMA_address_Ex(U32 u32_DMAAddr, U32 u32_ByteCnt)
{
	u32_ByteCnt = u32_ByteCnt;
	#if 0
	flush_cache(u32_DMAAddr, u32_ByteCnt);
	#endif

	// Need to set bank 0x1006 16 bits addr 0x7A bit9 on to make below code work,
	// otherwise, always DMA to MIU0

	REG_FCIE_CLRBIT(FCIE_MMA_PRI_REG, BIT_MIU_SELECT_MASK);

	if(u32_DMAAddr < CONFIG_MIU1_BUSADDR) // MIU0
	{
		return (u32_DMAAddr - CONFIG_MIU0_BUSADDR);
	}
	else  // MIU1
	{
		REG_FCIE_SETBIT(FCIE_MMA_PRI_REG, BIT_MIU1_SELECT);
		return (u32_DMAAddr - CONFIG_MIU1_BUSADDR);
	}

	return u32_DMAAddr;
}

void eMMC_Invalidate_data_cache_buffer(U32 u32_addr, S32 s32_size)
{
	u32_addr = u32_addr;
	s32_size = s32_size;
//	flush_cache(u32_addr, s32_size);
}

void eMMC_flush_miu_pipe(void)
{

}

U32 eMMC_PlatformResetPre(void)
{

	return eMMC_ST_SUCCESS;
}

U32 eMMC_PlatformResetPost(void)
{

	return eMMC_ST_SUCCESS;
}

U32 eMMC_PlatformInit(void)
{
	eMMC_clock_setting(FCIE_SLOWEST_CLK);
    eMMC_pads_switch();
	return eMMC_ST_SUCCESS;
}

#elif (defined(eMMC_DRV_M7221_BOOTRAM) && eMMC_DRV_M7221_BOOTRAM)
void putk_emmc(char c)
{
	while((REG_FCIE(REG_UART_LSR) & UART_LSR_THRE) != UART_LSR_THRE);
	REG_FCIE_W(REG_UART_TX, c);
	while((REG_FCIE(REG_UART_LSR) & (UART_LSR_TEMT | UART_LSR_THRE)) != (UART_LSR_TEMT | UART_LSR_THRE));
}

void eMMC_DumpPadClk(void)
{

}

void eMMC_pads_switch(U8 u8_Mode)
{
	eMMC_DRIVER *g_eMMCDrv = (eMMC_DRIVER *)g_eMMCDrv_array;

    g_eMMCDrv->u8_PadType = u8_Mode;

    // chiptop well configured in rom code
    REG_FCIE_CLRBIT(reg_nand_config, BIT_NAND_CFG_MASK);
    REG_FCIE_CLRBIT(reg_emmc_config, BIT_eMMC_CFG_MASK);
    REG_FCIE_SETBIT(reg_emmc_config, BIT_eMMC_CFG_MODE1);
    REG_FCIE_CLRBIT(reg_chiptop_0x50, BIT_ALL_PAD_IN);

    // fcie
    REG_FCIE_CLRBIT(FCIE_DDR_MODE, BIT_FALL_LATCH|BIT_PAD_IN_SEL_SD|BIT_CLK2_SEL|BIT_32BIT_MACRO_EN|BIT_DDR_EN|BIT_8BIT_MACRO_EN|BIT3|BIT2|BIT1);
    // emmc_pll

    REG_FCIE_CLRBIT(reg_emmcpll_0x09, BIT0);
    REG_FCIE_CLRBIT(reg_emmcpll_0x1a, BIT10|BIT5|BIT4);
    REG_FCIE_CLRBIT(reg_emmcpll_0x1c, BIT8|BIT9);
    REG_FCIE_CLRBIT(reg_emmcpll_0x20, BIT9|BIT10);

    REG_FCIE_CLRBIT(reg_emmcpll_0x63, BIT0);

    REG_FCIE_CLRBIT(reg_emmcpll_0x68, BIT0|BIT1);

    REG_FCIE_CLRBIT(reg_emmcpll_0x69, BIT7|BIT6|BIT5|BIT4|BIT3);

    REG_FCIE_CLRBIT(reg_emmcpll_0x6a, BIT0|BIT1);

    REG_FCIE_W(reg_emmcpll_0x6b, 0x0000);

    REG_FCIE_CLRBIT(reg_emmcpll_0x6d, BIT0);

    REG_FCIE_CLRBIT(reg_emmcpll_0x70, BIT11|BIT10|BIT8);

    REG_FCIE_CLRBIT(reg_emmcpll_0x7f, BIT10|BIT9|BIT8|BIT3|BIT2|BIT1);

    switch(u8_Mode)
    {
        case FCIE_eMMC_BYPASS:
//            eMMC_debug(eMMC_DEBUG_LEVEL_LOW, 0, "Bypass\n");

            // fcie
            REG_FCIE_SETBIT(FCIE_DDR_MODE,BIT_PAD_IN_SEL_SD|BIT_FALL_LATCH|BIT_CLK2_SEL);

            // emmc_pll
            REG_FCIE_SETBIT(reg_emmcpll_0x1a, BIT10);

            REG_FCIE_W(reg_emmcpll_0x71, 0xFFFF);

            REG_FCIE_W(reg_emmcpll_0x73, 0xFFFF);

            break;
        case FCIE_eMMC_HS200:
//            eMMC_debug(eMMC_DEBUG_LEVEL_LOW, 0, "HS200\n");

            // fcie
            REG_FCIE_SETBIT(FCIE_DDR_MODE, BIT_32BIT_MACRO_EN);

            // emmc_pll
            REG_FCIE_CLRBIT(reg_emmcpll_0x03, 0x0FFF);

            REG_FCIE_SETBIT(reg_emmcpll_0x1a, BIT5|BIT4);

            REG_FCIE_SETBIT(reg_emmcpll_0x1c, BIT8);

            REG_FCIE_SETBIT(reg_emmcpll_0x20, BIT9|BIT10);

            REG_FCIE_SETBIT(reg_emmcpll_0x68, BIT0);

            REG_FCIE_SETBIT(reg_emmcpll_0x69, 4<<4);

            REG_FCIE_SETBIT(reg_emmcpll_0x6a, 2<<0);
            REG_FCIE_W(reg_emmcpll_0x6b, 0x0213);

            REG_FCIE_SETBIT(reg_emmcpll_0x70, BIT8);

            REG_FCIE_W(reg_emmcpll_0x71, 0xF800);

            REG_FCIE_W(reg_emmcpll_0x73, 0xFD00);

            #if defined(ENABLE_eMMC_AFIFO) && ENABLE_eMMC_AFIFO

            REG_FCIE_SETBIT(reg_emmcpll_0x70, BIT10|BIT11);

            #endif

            break;

        case FCIE_eMMC_HS400:
//            eMMC_debug(eMMC_DEBUG_LEVEL_LOW, 0, "HS400\n");

            // fcie
            REG_FCIE_SETBIT(FCIE_DDR_MODE, BIT_32BIT_MACRO_EN|BIT_DDR_EN);

            // emmc_pll
            REG_FCIE_CLRBIT(reg_emmcpll_0x03, 0x0F0F);

            REG_FCIE_SETBIT(reg_emmcpll_0x09, BIT0);

            REG_FCIE_SETBIT(reg_emmcpll_0x1a, BIT5|BIT4);

            REG_FCIE_SETBIT(reg_emmcpll_0x1c, BIT8);

            REG_FCIE_SETBIT(reg_emmcpll_0x20, BIT10);
            REG_FCIE_SETBIT(reg_emmcpll_0x63, BIT0);

            REG_FCIE_SETBIT(reg_emmcpll_0x68, BIT0|BIT1);

            REG_FCIE_SETBIT(reg_emmcpll_0x69, 6<<4);

            REG_FCIE_SETBIT(reg_emmcpll_0x6a, 2<<0);
            REG_FCIE_W(reg_emmcpll_0x6b, 0x0113);

            REG_FCIE_SETBIT(reg_emmcpll_0x70, BIT8);

            REG_FCIE_W(reg_emmcpll_0x71, 0xF800);

            REG_FCIE_W(reg_emmcpll_0x73, 0xFD00);

            #if defined(ENABLE_eMMC_AFIFO) && ENABLE_eMMC_AFIFO

            REG_FCIE_SETBIT(reg_emmcpll_0x70, BIT10|BIT11);
            REG_FCIE_SETBIT(reg_emmcpll_0x7f, BIT2);

            #endif

            break;
        default:
//            eMMC_debug(1, 1, "eMMC Err: wrong parameter for switch pad func\n");
            return eMMC_ST_ERR_PARAMETER;
            break;
    }
    // 8 bits macro reset + 32 bits macro reset
    REG_FCIE_CLRBIT(reg_emmcpll_0x6f, BIT0|BIT1);
    REG_FCIE_SETBIT(reg_emmcpll_0x6f, BIT0|BIT1); // 8 bits macro reset + 32 bits macro reset
    return eMMC_ST_SUCCESS;

}

void eMMC_pads_switch_default(void)
{
    // fcie
    REG_FCIE_SETBIT(FCIE_DDR_MODE, BIT1|BIT2|BIT3);
}


U32 eMMC_pll_setting_OvertonePatch(void)
{
	U32 u32_value_reg_emmc_pll_pdiv;

    // 1. reset emmc pll
    REG_FCIE_SETBIT(reg_emmc_pll_reset,BIT0);
    REG_FCIE_CLRBIT(reg_emmc_pll_reset,BIT0);

	REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
	REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
    #if 0
    REG_FCIE_SETBIT(reg_ddfset_23_16,0x22); // 20xMHz
    REG_FCIE_SETBIT(reg_ddfset_15_00,0x8F5C);
    #else
    REG_FCIE_SETBIT(reg_ddfset_23_16,0x24); // 195MHz
    REG_FCIE_SETBIT(reg_ddfset_15_00,0x03D8);
    #endif
	u32_value_reg_emmc_pll_pdiv =1;// PostDIV: 2

	//overtone patch	
	//putk_emmc('P');
	//REG_FCIE_R(reg_emmcpll_0x03, u16_Tmp);
	REG_FCIE_W(reg_emmcpll_0x03, 0xffff);
	REG_FCIE_SETBIT(reg_emmcpll_0x1b, BIT0);
	REG_FCIE_CLRBIT(reg_emmcpll_0x04, BIT8-1);
	REG_FCIE_SETBIT(reg_emmcpll_0x04, 0x7);
//	eMMC_hw_timer_delay(500);
//	REG_FCIE_CLRBIT(reg_emmcpll_0x04, BIT0);
//	eMMC_hw_timer_delay(500);
//	REG_FCIE_CLRBIT(reg_emmcpll_0x1b, BIT0);
	//REG_FCIE_W(reg_emmcpll_0x03, u16_Tmp);
//	REG_FCIE_CLRBIT(reg_emmcpll_0x03, 0x0F0F);
	//putk_emmc('T');
	//putk_emmc('\n');
 
    // 3. VCO clock ( loop N = 4 )
//    REG_FCIE_CLRBIT(reg_emmcpll_fbdiv,0xffff);
//    REG_FCIE_SETBIT(reg_emmcpll_fbdiv,0x6);

    // 4. 1X clock
//    REG_FCIE_CLRBIT(reg_emmcpll_pdiv,BIT2|BIT1|BIT0);
//    REG_FCIE_SETBIT(reg_emmcpll_pdiv,u32_value_reg_emmc_pll_pdiv);

//    REG_FCIE_CLRBIT(reg_emmc_pll_test, BIT10);

    //eMMC_hw_timer_delay(HW_TIMER_DELAY_100us);

	//while(1);
	return eMMC_ST_SUCCESS;
}

U32 eMMC_pll_setting(U16 u16_ClkParam)
{
    U32 u32_value_reg_emmc_pll_pdiv;


    // 1. reset emmc pll
    REG_FCIE_SETBIT(reg_emmc_pll_reset,BIT0);
    REG_FCIE_CLRBIT(reg_emmc_pll_reset,BIT0);

    // 2. synth clock
    switch(u16_ClkParam)
    {
        case eMMC_PLL_CLK_200M: // 200M
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
        #if 0
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x22); // 20xMHz
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x8F5C);
        #else
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x24); // 195MHz
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x03D8);
        #endif
            u32_value_reg_emmc_pll_pdiv =1;// PostDIV: 2
            break;

        case eMMC_PLL_CLK_160M:
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x2B);
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x3333);
            u32_value_reg_emmc_pll_pdiv =1;
            break;

        case eMMC_PLL_CLK_140M:
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x31);
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x5F15);
            u32_value_reg_emmc_pll_pdiv = 1;
            break;

        case eMMC_PLL_CLK_120M:
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x39);
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x9999);
            u32_value_reg_emmc_pll_pdiv = 1;
            break;

        case eMMC_PLL_CLK_100M:
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x45);
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x1EB8);
            u32_value_reg_emmc_pll_pdiv = 1;
            break;

        case eMMC_PLL_CLK__86M:
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x28);
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x2FA0);
            u32_value_reg_emmc_pll_pdiv = 2;
            break;

        case eMMC_PLL_CLK__80M:
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x2B);
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x3333);
            u32_value_reg_emmc_pll_pdiv = 2;
            break;

        case eMMC_PLL_CLK__72M:
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x30);
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x0000);
            u32_value_reg_emmc_pll_pdiv = 2;
            break;

        case eMMC_PLL_CLK__62M:
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x37);
            REG_FCIE_SETBIT(reg_ddfset_15_00,0xBDEF);
            u32_value_reg_emmc_pll_pdiv = 2;
            break;

        case eMMC_PLL_CLK__52M:
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x42);
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x7627);
            u32_value_reg_emmc_pll_pdiv = 2;
            break;

        case eMMC_PLL_CLK__48M:
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x48);
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x0000);
            u32_value_reg_emmc_pll_pdiv = 2;
            break;

        case eMMC_PLL_CLK__40M:
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x2B);
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x3333);
            u32_value_reg_emmc_pll_pdiv = 4;
            break;

        case eMMC_PLL_CLK__36M:
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x30);
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x0000);
            u32_value_reg_emmc_pll_pdiv = 4;
            break;

        case eMMC_PLL_CLK__32M:
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x36);
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x0000);
            u32_value_reg_emmc_pll_pdiv = 4;
            break;

        case eMMC_PLL_CLK__27M:
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x40);
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x0000);
            u32_value_reg_emmc_pll_pdiv = 4;
            break;

        case eMMC_PLL_CLK__20M:
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x2B);
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x3333);
            u32_value_reg_emmc_pll_pdiv = 7;
            break;

        default:
            return eMMC_ST_ERR_UNKNOWN_CLK;
            break;
    }

    // 3. VCO clock ( loop N = 4 )
    REG_FCIE_CLRBIT(reg_emmcpll_fbdiv,0xffff);
    REG_FCIE_SETBIT(reg_emmcpll_fbdiv,0x6);

    // 4. 1X clock
    REG_FCIE_CLRBIT(reg_emmcpll_pdiv,BIT2|BIT1|BIT0);
    REG_FCIE_SETBIT(reg_emmcpll_pdiv,u32_value_reg_emmc_pll_pdiv);

    if(u16_ClkParam==eMMC_PLL_CLK__20M)
    {
        REG_FCIE_SETBIT(reg_emmc_pll_test, BIT10);
    }
    else
    {
        REG_FCIE_CLRBIT(reg_emmc_pll_test, BIT10);
    }

    eMMC_hw_timer_delay(HW_TIMER_DELAY_100us);

    return eMMC_ST_SUCCESS;
}

void HalEmmcPll_dll_setting(void)
{
    volatile U16 u16_reg;


    REG_FCIE_CLRBIT(reg_emmcpll_0x09, BIT0);

    // Reset eMMC_DLL
    REG_FCIE_SETBIT(REG_EMMC_PLL_RX30, BIT2);
    REG_FCIE_CLRBIT(REG_EMMC_PLL_RX30, BIT2);

    //DLL pulse width and phase
    REG_FCIE_W(REG_EMMC_PLL_RX01, 0x7F72);

    // DLL code
    REG_FCIE_W(REG_EMMC_PLL_RX32, 0xF200);

    // DLL calibration
    REG_FCIE_W(REG_EMMC_PLL_RX30, 0x3378);
    REG_FCIE_SETBIT(REG_EMMC_PLL_RX33, BIT15);

    // Wait 100us
    eMMC_hw_timer_delay(250);

    // Get hw dll0 code
    REG_FCIE_R(REG_EMMC_PLL_RX33, u16_reg);

    REG_FCIE_CLRBIT(REG_EMMC_PLL_RX34, (BIT10 - 1));
    // Set dw dll0 code
    REG_FCIE_SETBIT(REG_EMMC_PLL_RX34, u16_reg & 0x03FF);

    // Disable reg_hw_upcode_en
    REG_FCIE_CLRBIT(REG_EMMC_PLL_RX30, BIT9);

    // Clear reg_emmc_dll_test[7]
    REG_FCIE_CLRBIT(reg_emmcpll_0x02, BIT15);

    // Enable reg_rxdll_dline_en
    REG_FCIE_SETBIT(reg_emmcpll_0x09, BIT0);

}

U32 eMMC_clock_setting(U32 u32_ClkParam)
{
    U16 u16_Tmp = 0;
	eMMC_DRIVER *g_eMMCDrv = (eMMC_DRIVER *)g_eMMCDrv_array;

    eMMC_PlatformResetPre();

    REG_FCIE_CLRBIT(FCIE_SD_MODE, BIT_SD_CLK_EN);

    REG_FCIE_CLRBIT(reg_ckg_fcie, BIT_FCIE_CLK_SRC_SEL);
    REG_FCIE_R(reg_ckg_fcie, u16_Tmp);
    eMMC_hw_timer_delay(7);
    REG_FCIE_CLRBIT(reg_ckg_fcie, BIT_FCIE_CLK_Gate|BIT_FCIE_CLK_MASK);

	if(u32_ClkParam & eMMC_PLL_FLAG)
	{
	    REG_FCIE_R(reg_ckg_fcie_syn, u16_Tmp);
	    if((u16_Tmp&(BIT0|BIT1)) != BIT0)
	    {
	        REG_FCIE_CLRBIT(reg_ckg_fcie_syn, BIT0|BIT1);
	        REG_FCIE_SETBIT(reg_ckg_fcie_syn, BIT0);
	    }

		eMMC_pll_setting(u32_ClkParam);

        if((g_eMMCDrv->u32_Flag & (FCIE_FLAG_SPEED_HS400 | FCIE_FLAG_DDR_MODE)) ==
            (FCIE_FLAG_SPEED_HS400 | FCIE_FLAG_DDR_MODE)) {

            REG_FCIE_SETBIT(reg_ckg_fcie, BIT_FCIE_CLK_EMMC_PLL_2X<<2);
            REG_FCIE_SETBIT(reg_ckg_fcie, BIT_FCIE_CLK_SRC_SEL);

            HalEmmcPll_dll_setting(); // tuning DLL setting
        } else {
            REG_FCIE_SETBIT(reg_ckg_fcie, BIT_FCIE_CLK_EMMC_PLL_1X<<2);
            REG_FCIE_SETBIT(reg_ckg_fcie, BIT_FCIE_CLK_SRC_SEL);
        }
	}
	else
	{
		REG_FCIE_CLRBIT(reg_emmcpll_0x1d, BIT0);	//FPGA mode no harm for ASIC
	    if (u32_ClkParam == BIT_FCIE_CLK_300K)
		{
			REG_FCIE_SETBIT(reg_emmcpll_0x1d, BIT0);	//FPGA mode no harm for ASIC
		}
        REG_FCIE_SETBIT(reg_ckg_fcie, u32_ClkParam<<BIT_FCIE_CLK_SHIFT);
        REG_FCIE_SETBIT(reg_ckg_fcie, BIT_FCIE_CLK_SRC_SEL);
	}

    eMMC_PlatformResetPost();

    return eMMC_ST_SUCCESS;
}

void eMMC_set_WatchDog(U8 u8_IfEnable)
{
	u8_IfEnable = u8_IfEnable;
	// do nothing
}

void eMMC_reset_WatchDog(void)
{
	// do nothing
}

U32 eMMC_translate_DMA_address_Ex(U32 u32_DMAAddr, U32 u32_ByteCnt)
{
	u32_ByteCnt = u32_ByteCnt;
	#if 0
	flush_cache(u32_DMAAddr, u32_ByteCnt);
	#endif

	// Need to set bank 0x1006 16 bits addr 0x7A bit9 on to make below code work,
	// otherwise, always DMA to MIU0

	REG_FCIE_CLRBIT(FCIE_MMA_PRI_REG, BIT_MIU_SELECT_MASK);

	if ((u32_DMAAddr >> 16) == ((CONFIG_SRAM_BASE_ADDRESS) >> 16))
		return (u32_DMAAddr - CONFIG_SRAM_BASE_ADDRESS);

	if(u32_DMAAddr < CONFIG_MIU1_BUSADDR) // MIU0
	{
		return (u32_DMAAddr - CONFIG_MIU0_BUSADDR);
	}
	else  // MIU1
	{
		REG_FCIE_SETBIT(FCIE_MMA_PRI_REG, BIT_MIU1_SELECT);
		return (u32_DMAAddr - CONFIG_MIU1_BUSADDR);
	}

	return u32_DMAAddr;
}

void eMMC_Invalidate_data_cache_buffer(U32 u32_addr, S32 s32_size)
{
	u32_addr = u32_addr;
	s32_size = s32_size;
//	flush_cache(u32_addr, s32_size);
}

void eMMC_flush_miu_pipe(void)
{
}

U32 eMMC_PlatformResetPre(void)
{
	return eMMC_ST_SUCCESS;
}

U32 eMMC_PlatformResetPost(void)
{
	return eMMC_ST_SUCCESS;
}


U32 eMMC_PlatformInit(void)
{
	eMMC_pads_switch(FCIE_eMMC_BYPASS);
	eMMC_clock_setting(FCIE_SLOWEST_CLK);
	return eMMC_ST_SUCCESS;
}

#elif (defined(eMMC_DRV_M7322_BOOTRAM) && eMMC_DRV_M7322_BOOTRAM)
void putk_emmc(char c)
{
	while((REG_FCIE(REG_UART_LSR) & UART_LSR_THRE) != UART_LSR_THRE);
	REG_FCIE_W(REG_UART_TX, c);
	while((REG_FCIE(REG_UART_LSR) & (UART_LSR_TEMT | UART_LSR_THRE)) != (UART_LSR_TEMT | UART_LSR_THRE));

}

void eMMC_DumpPadClk(void)
{

}

U32 eMMC_pads_switch(U8 u8Mode)
{
    eMMC_DRIVER *g_eMMCDrv = (eMMC_DRIVER *)g_eMMCDrv_array;

    g_eMMCDrv->u8_PadType = u8Mode;

    // fcie
    REG_FCIE_CLRBIT(FCIE_DDR_MODE, BIT_FALL_LATCH|BIT_PAD_IN_SEL_SD|BIT_CLK2_SEL|BIT_32BIT_MACRO_EN|BIT_DDR_EN|BIT_8BIT_MACRO_EN);

    // emmc_pll
    REG_FCIE_CLRBIT(reg_emmcpll_0x09, BIT0);
    REG_FCIE_CLRBIT(reg_emmcpll_0x1a, BIT10|BIT5|BIT4);
    REG_FCIE_CLRBIT(reg_emmcpll_0x1c, BIT8|BIT9);
    REG_FCIE_CLRBIT(reg_emmcpll_0x20, BIT9|BIT10);
    REG_FCIE_CLRBIT(reg_emmcpll_0x63, BIT0);
    REG_FCIE_CLRBIT(reg_emmcpll_0x68, BIT0|BIT1);
    REG_FCIE_CLRBIT(reg_emmcpll_0x69, BIT7|BIT6|BIT5|BIT4|BIT3);
    REG_FCIE_CLRBIT(reg_emmcpll_0x6a, BIT0|BIT1);
    REG_FCIE_W(reg_emmcpll_0x6b, 0x0000);
    REG_FCIE_CLRBIT(reg_emmcpll_0x6d, BIT0);
    REG_FCIE_CLRBIT(reg_emmcpll_0x70, BIT11|BIT10|BIT8);
    REG_FCIE_CLRBIT(reg_emmcpll_0x7f, BIT10|BIT9|BIT8|BIT3|BIT2|BIT1);

    switch (u8Mode) {

    case FCIE_eMMC_BYPASS:

        // fcie
        REG_FCIE_SETBIT(FCIE_DDR_MODE,BIT_PAD_IN_SEL_SD|BIT_FALL_LATCH|BIT_CLK2_SEL);

        // emmc_pll
        REG_FCIE_SETBIT(reg_emmcpll_0x1a, BIT10);
        REG_FCIE_W(reg_emmcpll_0x71, 0xFFFF);
        REG_FCIE_W(reg_emmcpll_0x73, 0xFFFF);

        break;

    case FCIE_eMMC_HS200:

        // fcie
        REG_FCIE_SETBIT(FCIE_DDR_MODE, BIT_32BIT_MACRO_EN);

        // emmc_pll
        REG_FCIE_CLRBIT(reg_emmcpll_0x03, 0x0FFF);
        REG_FCIE_SETBIT(reg_emmcpll_0x1a, BIT5|BIT4);
        REG_FCIE_SETBIT(reg_emmcpll_0x1c, BIT8);
        REG_FCIE_SETBIT(reg_emmcpll_0x20, BIT9|BIT10);
        REG_FCIE_SETBIT(reg_emmcpll_0x68, BIT0);
        REG_FCIE_SETBIT(reg_emmcpll_0x69, 4<<4);
        REG_FCIE_SETBIT(reg_emmcpll_0x6a, 2<<0);
        REG_FCIE_W(reg_emmcpll_0x6b, 0x0213);
        REG_FCIE_SETBIT(reg_emmcpll_0x70, BIT8);
        REG_FCIE_W(reg_emmcpll_0x71, 0xF800);
        REG_FCIE_W(reg_emmcpll_0x73, 0xFD00);
        #if defined(ENABLE_eMMC_AFIFO) && ENABLE_eMMC_AFIFO
        REG_FCIE_SETBIT(reg_emmcpll_0x70, BIT10|BIT11);
        #endif

        break;

    case FCIE_eMMC_HS400:

        // fcie
        REG_FCIE_SETBIT(FCIE_DDR_MODE, BIT_32BIT_MACRO_EN|BIT_DDR_EN);

        // emmc_pll
        REG_FCIE_CLRBIT(reg_emmcpll_0x03, 0x0F0F);
        REG_FCIE_SETBIT(reg_emmcpll_0x09, BIT0);
        REG_FCIE_SETBIT(reg_emmcpll_0x1a, BIT5|BIT4);
        REG_FCIE_SETBIT(reg_emmcpll_0x1c, BIT8);
        REG_FCIE_SETBIT(reg_emmcpll_0x20, BIT10);
        REG_FCIE_SETBIT(reg_emmcpll_0x63, BIT0);
        REG_FCIE_SETBIT(reg_emmcpll_0x68, BIT0|BIT1);
        REG_FCIE_SETBIT(reg_emmcpll_0x69, 6<<4);
        REG_FCIE_SETBIT(reg_emmcpll_0x6a, 2<<0);
        REG_FCIE_W(reg_emmcpll_0x6b, 0x0113);
        REG_FCIE_SETBIT(reg_emmcpll_0x70, BIT8);
        REG_FCIE_W(reg_emmcpll_0x71, 0xF800);
        REG_FCIE_W(reg_emmcpll_0x73, 0xFD00);
        #if defined(ENABLE_eMMC_AFIFO) && ENABLE_eMMC_AFIFO
        REG_FCIE_SETBIT(reg_emmcpll_0x70, BIT10|BIT11);
        REG_FCIE_SETBIT(reg_emmcpll_0x7f, BIT2);
        #if defined(ENABLE_HS400_ENHANCED_STORBE) && ENABLE_HS400_ENHANCED_STORBE
        if (g_eMMCDrv->u8_ECSD184_Stroe_Support)
        {
            REG_FCIE_CLRBIT(reg_emmcpll_0x20, BIT10);
            REG_FCIE_SETBIT(reg_emmcpll_0x7f, BIT8);
        }
        #endif
        #endif

        break;

    default:
        return eMMC_ST_ERR_PARAMETER;
        break;
    }

    // 8 bits macro reset + 32 bits macro reset
    REG_FCIE_CLRBIT(reg_emmcpll_0x6f, BIT0|BIT1);
    REG_FCIE_SETBIT(reg_emmcpll_0x6f, BIT0|BIT1); // 8 bits macro reset + 32 bits macro reset

    return eMMC_ST_SUCCESS;
}

void eMMC_pads_switch_default(void)
{
    // fcie
    REG_FCIE_SETBIT(FCIE_DDR_MODE, BIT1|BIT2|BIT3);
}

U32 eMMC_pll_setting(U16 u16_ClkParam)
{
    U32 u32_value_reg_emmc_pll_pdiv;


    // 1. reset emmc pll
    REG_FCIE_SETBIT(reg_emmc_pll_reset,BIT0);
    REG_FCIE_CLRBIT(reg_emmc_pll_reset,BIT0);

    // 2. synth clock
    switch(u16_ClkParam)
    {
        case eMMC_PLL_CLK_200M: // 200M
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            #if 0
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x22); // 20xMHz
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x8F5C);
            #else
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x24); // 195MHz
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x03D8);
            #endif
            u32_value_reg_emmc_pll_pdiv =1;// PostDIV: 2
            break;

        case eMMC_PLL_CLK_160M:
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x2B);
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x3333);
            u32_value_reg_emmc_pll_pdiv =1;
            break;

        case eMMC_PLL_CLK_140M:
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x31);
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x5F15);
            u32_value_reg_emmc_pll_pdiv = 1;
            break;

        case eMMC_PLL_CLK_120M:
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x39);
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x9999);
            u32_value_reg_emmc_pll_pdiv = 1;
            break;

        default:
            return eMMC_ST_ERR_UNKNOWN_CLK;
            break;
    }

    // 3. VCO clock ( loop N = 4 )
    REG_FCIE_CLRBIT(reg_emmcpll_fbdiv,0xffff);
    REG_FCIE_SETBIT(reg_emmcpll_fbdiv,0x6);

    // 4. 1X clock
    REG_FCIE_CLRBIT(reg_emmcpll_pdiv,BIT2|BIT1|BIT0);
    REG_FCIE_SETBIT(reg_emmcpll_pdiv,u32_value_reg_emmc_pll_pdiv);

    if(u16_ClkParam==eMMC_PLL_CLK__20M)
    {
        REG_FCIE_SETBIT(reg_emmc_pll_test, BIT10);
    }
    else
    {
        REG_FCIE_CLRBIT(reg_emmc_pll_test, BIT10);
    }

    eMMC_hw_timer_delay(HW_TIMER_DELAY_100us);

    return eMMC_ST_SUCCESS;
}

void HalEmmcPll_dll_setting(void)
{
    volatile U16 u16_reg;


    REG_FCIE_CLRBIT(reg_emmcpll_0x09, BIT0);

    // Reset eMMC_DLL
    REG_FCIE_SETBIT(REG_EMMC_PLL_RX30, BIT2);
    REG_FCIE_CLRBIT(REG_EMMC_PLL_RX30, BIT2);

    //DLL pulse width and phase
    REG_FCIE_W(REG_EMMC_PLL_RX01, 0x7F72);

    // DLL code
    REG_FCIE_W(REG_EMMC_PLL_RX32, 0xF200);

    // DLL calibration
    REG_FCIE_W(REG_EMMC_PLL_RX30, 0x3378);
    REG_FCIE_SETBIT(REG_EMMC_PLL_RX33, BIT15);

    // Wait 100us
    eMMC_hw_timer_delay(250);

    // Get hw dll0 code
    REG_FCIE_R(REG_EMMC_PLL_RX33, u16_reg);

    REG_FCIE_CLRBIT(REG_EMMC_PLL_RX34, (BIT10 - 1));
    // Set dw dll0 code
    REG_FCIE_SETBIT(REG_EMMC_PLL_RX34, u16_reg & 0x03FF);

    // Disable reg_hw_upcode_en
    REG_FCIE_CLRBIT(REG_EMMC_PLL_RX30, BIT9);

    // Clear reg_emmc_dll_test[7]
    REG_FCIE_CLRBIT(reg_emmcpll_0x02, BIT15);

    // Enable reg_rxdll_dline_en
    REG_FCIE_SETBIT(reg_emmcpll_0x09, BIT0);

}


U32 eMMC_pll_setting_OvertonePatch(void)
{
	U32 u32_value_reg_emmc_pll_pdiv;

	// 1. reset emmc pll
	REG_FCIE_SETBIT(reg_emmc_pll_reset,BIT0);
	REG_FCIE_CLRBIT(reg_emmc_pll_reset,BIT0);

	REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
	REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
    #if 0
	REG_FCIE_SETBIT(reg_ddfset_23_16,0x22); // 20xMHz
	REG_FCIE_SETBIT(reg_ddfset_15_00,0x8F5C);
    #else
	REG_FCIE_SETBIT(reg_ddfset_23_16,0x24); // 195MHz
	REG_FCIE_SETBIT(reg_ddfset_15_00,0x03D8);
    #endif
	u32_value_reg_emmc_pll_pdiv =1;// PostDIV: 2

	// 2. overtone patch
	REG_FCIE_W(reg_emmcpll_0x03, 0xffff);
	REG_FCIE_SETBIT(reg_emmcpll_0x1b, BIT0);
	REG_FCIE_CLRBIT(reg_emmcpll_0x04, BIT8-1);
	REG_FCIE_SETBIT(reg_emmcpll_0x04, 0x7);
	eMMC_hw_timer_delay(100);
	REG_FCIE_CLRBIT(reg_emmcpll_0x04, BIT0);

	// 3. VCO clock ( loop N = 4 )
	REG_FCIE_CLRBIT(reg_emmcpll_fbdiv,0xffff);
	REG_FCIE_SETBIT(reg_emmcpll_fbdiv,0x6);

	// 4. 1X clock
	REG_FCIE_CLRBIT(reg_emmcpll_pdiv,BIT2|BIT1|BIT0);
	REG_FCIE_SETBIT(reg_emmcpll_pdiv,u32_value_reg_emmc_pll_pdiv);

	REG_FCIE_CLRBIT(reg_emmc_pll_test, BIT10);
	eMMC_hw_timer_delay(100);
	REG_FCIE_CLRBIT(reg_emmcpll_0x1b, BIT0);

	return eMMC_ST_SUCCESS;
}


U32 eMMC_clock_setting(U16 u16_ClkParam)
{
    U16 u16_Tmp;
    eMMC_DRIVER *g_eMMCDrv = (eMMC_DRIVER *)g_eMMCDrv_array;

    eMMC_PlatformResetPre();

    REG_FCIE_CLRBIT(FCIE_SD_MODE, BIT_SD_CLK_EN);

    REG_FCIE_CLRBIT(reg_ckg_fcie, BIT_FCIE_CLK_SRC_SEL);
    REG_FCIE_R(reg_ckg_fcie, u16_Tmp);
    eMMC_hw_timer_delay(7);
    REG_FCIE_CLRBIT(reg_ckg_fcie, BIT_FCIE_CLK_Gate|BIT_FCIE_CLK_MASK);

    if (u16_ClkParam & eMMC_PLL_FLAG) {

        eMMC_pll_setting(u16_ClkParam);

        if((g_eMMCDrv->u32_Flag & (FCIE_FLAG_SPEED_HS400 | FCIE_FLAG_DDR_MODE)) ==
            (FCIE_FLAG_SPEED_HS400 | FCIE_FLAG_DDR_MODE)) {

            REG_FCIE_SETBIT(reg_ckg_fcie, BIT_FCIE_CLK_EMMC_PLL_2X<<2);
            REG_FCIE_SETBIT(reg_ckg_fcie, BIT_FCIE_CLK_SRC_SEL);

            HalEmmcPll_dll_setting(); // tuning DLL setting
        } else {
            REG_FCIE_SETBIT(reg_ckg_fcie, BIT_FCIE_CLK_EMMC_PLL_1X<<2);
            REG_FCIE_SETBIT(reg_ckg_fcie, BIT_FCIE_CLK_SRC_SEL);
        }
    } else {
        REG_FCIE_SETBIT(reg_ckg_fcie, u16_ClkParam<<2);
        REG_FCIE_SETBIT(reg_ckg_fcie, BIT_FCIE_CLK_SRC_SEL);
    }

    eMMC_PlatformResetPost();


    return eMMC_ST_SUCCESS;

}

void eMMC_set_WatchDog(U8 u8_IfEnable)
{
	u8_IfEnable = u8_IfEnable;
	// do nothing
}

void eMMC_reset_WatchDog(void)
{
	// do nothing
}

U32 eMMC_translate_DMA_address_Ex(U32 u32_DMAAddr, U32 u32_ByteCnt)
{
	u32_ByteCnt = u32_ByteCnt;
	#if 0
	flush_cache(u32_DMAAddr, u32_ByteCnt);
	#endif

	// Need to set bank 0x1006 16 bits addr 0x7A bit9 on to make below code work,
	// otherwise, always DMA to MIU0

	REG_FCIE_CLRBIT(FCIE_MMA_PRI_REG, BIT_MIU_SELECT_MASK);

	if ((u32_DMAAddr >> 16) == ((CONFIG_SRAM_BASE_ADDRESS) >> 16))
		return (u32_DMAAddr - CONFIG_SRAM_BASE_ADDRESS);

	if(u32_DMAAddr < CONFIG_MIU1_BUSADDR) // MIU0
	{
		return (u32_DMAAddr - CONFIG_MIU0_BUSADDR);
	}
	else  // MIU1
	{
		REG_FCIE_SETBIT(FCIE_MMA_PRI_REG, BIT_MIU1_SELECT);
		return (u32_DMAAddr - CONFIG_MIU1_BUSADDR);
	}

	return u32_DMAAddr;
}

void eMMC_Invalidate_data_cache_buffer(U32 u32_addr, S32 s32_size)
{
	u32_addr = u32_addr;
	s32_size = s32_size;
//	flush_cache(u32_addr, s32_size);
}

void eMMC_flush_miu_pipe(void)
{
}

U32 eMMC_PlatformResetPre(void)
{
    if ((REG_FCIE(reg_ckg_fcie_syn) & BIT_CKG_FCIE_SYN_MASK) != BIT_CKG_FCIE_SYN )
    {
        REG_FCIE_CLRBIT(reg_ckg_fcie_syn, BIT_CKG_FCIE_SYN_MASK);
        REG_FCIE_SETBIT(reg_ckg_fcie_syn, BIT_CKG_FCIE_SYN);
    }

    return eMMC_ST_SUCCESS;
}

U32 eMMC_PlatformResetPost(void)
{
	return eMMC_ST_SUCCESS;
}


U32 eMMC_PlatformInit(void)
{
    if ((REG_FCIE(reg_emmcpll_0x1a) & BIT0) == 0) {
        //eMMC_debug(0, 0, "eMMC Err: not 1.8V IO setting\n");
        REG_FCIE_SETBIT(reg_emmcpll_0x1a, BIT0);// 1.8V must set this bit
        REG_FCIE_SETBIT(reg_emmcpll_0x1a, BIT2); // atop patch
        REG_FCIE_CLRBIT(reg_emmcpll_0x45, 0xffff);// 1.8V must set this bit
        REG_FCIE_SETBIT(reg_emmcpll_0x45, 0xf);// 1.8V must set this bit
    }

    eMMC_clock_setting(FCIE_SLOWEST_CLK);
    eMMC_pads_switch(FCIE_eMMC_BYPASS);

    return eMMC_ST_SUCCESS;
}

#elif (defined(eMMC_DRV_M7622_BOOTRAM) && eMMC_DRV_M7622_BOOTRAM)
void putk_emmc(char c)
{
	while((REG_FCIE(REG_UART_LSR) & UART_LSR_THRE) != UART_LSR_THRE);
	REG_FCIE_W(REG_UART_TX, c);
	while((REG_FCIE(REG_UART_LSR) & (UART_LSR_TEMT | UART_LSR_THRE)) != (UART_LSR_TEMT | UART_LSR_THRE));
}

U32 eMMC_pads_switch(U8 u8Mode)
{
	eMMC_DRIVER *g_eMMCDrv = (eMMC_DRIVER *)g_eMMCDrv_array;

	g_eMMCDrv->u8_PadType = u8Mode;

	// chiptop
	REG_FCIE_CLRBIT(reg_chiptop_0x5A, BIT_SD_CONFIG);
	REG_FCIE_CLRBIT(reg_chiptop_0x6F, BIT_NAND_MODE);
	REG_FCIE_CLRBIT(reg_chiptop_0x6E, BIT_EMMC_CONFIG_MSK);
	REG_FCIE_SETBIT(reg_chiptop_0x6E, BIT_EMMC_CONFIG_EMMC_MODE_1);
	REG_FCIE_CLRBIT(reg_chiptop_0x50, BIT_ALL_PAD_IN);

	// fcie
	REG_FCIE_CLRBIT(FCIE_DDR_MODE, BIT_FALL_LATCH|BIT_PAD_IN_SEL_SD|BIT_CLK2_SEL|BIT_32BIT_MACRO_EN|BIT_DDR_EN|BIT_8BIT_MACRO_EN);

	// emmc_pll
	REG_FCIE_CLRBIT(reg_emmcpll_0x09, BIT0);
	REG_FCIE_CLRBIT(reg_emmcpll_0x1a, BIT10|BIT5|BIT4);
	REG_FCIE_CLRBIT(reg_emmcpll_0x1c, BIT8|BIT9);
	REG_FCIE_CLRBIT(reg_emmcpll_0x20, BIT9|BIT10);
	REG_FCIE_CLRBIT(reg_emmcpll_0x63, BIT0);
	REG_FCIE_CLRBIT(reg_emmcpll_0x68, BIT0|BIT1);
	REG_FCIE_CLRBIT(reg_emmcpll_0x69, BIT7|BIT6|BIT5|BIT4|BIT3);
	REG_FCIE_CLRBIT(reg_emmcpll_0x6a, BIT0|BIT1);
	REG_FCIE_W(reg_emmcpll_0x6b, 0x0000);
	REG_FCIE_CLRBIT(reg_emmcpll_0x6d, BIT0);
	REG_FCIE_CLRBIT(reg_emmcpll_0x70, BIT11|BIT10|BIT8);
	REG_FCIE_CLRBIT(reg_emmcpll_0x7f, BIT10|BIT9|BIT8|BIT3|BIT2|BIT1);

	switch (u8Mode) {

	case FCIE_eMMC_BYPASS:

		// fcie
		REG_FCIE_SETBIT(FCIE_DDR_MODE,BIT_PAD_IN_SEL_SD|BIT_FALL_LATCH|BIT_CLK2_SEL);

		// emmc_pll
		REG_FCIE_SETBIT(reg_emmcpll_0x1a, BIT10);
		REG_FCIE_W(reg_emmcpll_0x71, 0xFFFF);
		REG_FCIE_W(reg_emmcpll_0x73, 0xFFFF);

		break;

	case FCIE_eMMC_HS200:

		// fcie
		REG_FCIE_SETBIT(FCIE_DDR_MODE, BIT_32BIT_MACRO_EN);

		// emmc_pll
		REG_FCIE_CLRBIT(reg_emmcpll_0x03, 0x0FFF);
		REG_FCIE_SETBIT(reg_emmcpll_0x1a, BIT5|BIT4);
		REG_FCIE_SETBIT(reg_emmcpll_0x1c, BIT8);
		REG_FCIE_SETBIT(reg_emmcpll_0x20, BIT9|BIT10);
		REG_FCIE_SETBIT(reg_emmcpll_0x68, BIT0);
		REG_FCIE_SETBIT(reg_emmcpll_0x69, 4<<4);
		REG_FCIE_SETBIT(reg_emmcpll_0x6a, 2<<0);
		REG_FCIE_W(reg_emmcpll_0x6b, 0x0213);
		REG_FCIE_SETBIT(reg_emmcpll_0x70, BIT8);
		REG_FCIE_W(reg_emmcpll_0x71, 0xF800);
		REG_FCIE_W(reg_emmcpll_0x73, 0xFD00);
		REG_FCIE_SETBIT(reg_emmcpll_0x70, BIT10|BIT11);
		break;

	case FCIE_eMMC_HS400:

		// fcie
		REG_FCIE_SETBIT(FCIE_DDR_MODE, BIT_32BIT_MACRO_EN|BIT_DDR_EN);

		// emmc_pll
		REG_FCIE_CLRBIT(reg_emmcpll_0x03, 0x0F0F);
		REG_FCIE_SETBIT(reg_emmcpll_0x09, BIT0);
		REG_FCIE_SETBIT(reg_emmcpll_0x1a, BIT5|BIT4);
		REG_FCIE_SETBIT(reg_emmcpll_0x1c, BIT8);
		REG_FCIE_SETBIT(reg_emmcpll_0x20, BIT10);
		REG_FCIE_SETBIT(reg_emmcpll_0x63, BIT0);
		REG_FCIE_SETBIT(reg_emmcpll_0x68, BIT0|BIT1);
		REG_FCIE_SETBIT(reg_emmcpll_0x69, 6<<4);
		REG_FCIE_SETBIT(reg_emmcpll_0x6a, 2<<0);
		REG_FCIE_W(reg_emmcpll_0x6b, 0x0113);
		REG_FCIE_SETBIT(reg_emmcpll_0x70, BIT8);
		REG_FCIE_W(reg_emmcpll_0x71, 0xF800);
		REG_FCIE_W(reg_emmcpll_0x73, 0xFD00);
		REG_FCIE_SETBIT(reg_emmcpll_0x70, BIT10|BIT11);
		REG_FCIE_SETBIT(reg_emmcpll_0x7f, BIT2);
        #if defined(ENABLE_HS400_ENHANCED_STORBE) && ENABLE_HS400_ENHANCED_STORBE
        if (g_eMMCDrv->u8_ECSD184_Stroe_Support)
        {
            REG_FCIE_CLRBIT(reg_emmcpll_0x20, BIT10);
        REG_FCIE_SETBIT(reg_emmcpll_0x7f, BIT8);
        }
        #endif

		break;

	default:
		return eMMC_ST_ERR_PARAMETER;
		break;
	}

	// 8 bits macro reset + 32 bits macro reset
	REG_FCIE_CLRBIT(reg_emmcpll_0x6f, BIT0|BIT1);
	REG_FCIE_SETBIT(reg_emmcpll_0x6f, BIT0|BIT1); // 8 bits macro reset + 32 bits macro reset

	return eMMC_ST_SUCCESS;
}

void eMMC_pads_switch_default(void)
{
    // fcie
    REG_FCIE_SETBIT(FCIE_DDR_MODE, BIT1|BIT2|BIT3);
}



U32 eMMC_pll_setting(U16 u16_ClkParam)
{
    U32 u32_value_reg_emmc_pll_pdiv;


    // 1. reset emmc pll
    REG_FCIE_SETBIT(reg_emmc_pll_reset,BIT0);
    REG_FCIE_CLRBIT(reg_emmc_pll_reset,BIT0);

    // 2. synth clock
    switch(u16_ClkParam)
    {
        case eMMC_PLL_CLK_200M: // 200M
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
        #if 0
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x22); // 20xMHz
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x8F5C);
        #else
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x24); // 195MHz
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x03D8);
        #endif
            u32_value_reg_emmc_pll_pdiv =1;// PostDIV: 2
            break;

        case eMMC_PLL_CLK_160M:
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x2B);
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x3333);
            u32_value_reg_emmc_pll_pdiv =1;
            break;

        case eMMC_PLL_CLK_140M:
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x31);
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x5F15);
            u32_value_reg_emmc_pll_pdiv = 1;
            break;

        case eMMC_PLL_CLK_120M:
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x39);
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x9999);
            u32_value_reg_emmc_pll_pdiv = 1;
            break;

        case eMMC_PLL_CLK_100M:
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x45);
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x1EB8);
            u32_value_reg_emmc_pll_pdiv = 1;
            break;

        case eMMC_PLL_CLK__86M:
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x28);
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x2FA0);
            u32_value_reg_emmc_pll_pdiv = 2;
            break;

        case eMMC_PLL_CLK__80M:
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x2B);
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x3333);
            u32_value_reg_emmc_pll_pdiv = 2;
            break;

        case eMMC_PLL_CLK__72M:
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x30);
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x0000);
            u32_value_reg_emmc_pll_pdiv = 2;
            break;

        case eMMC_PLL_CLK__62M:
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x37);
            REG_FCIE_SETBIT(reg_ddfset_15_00,0xBDEF);
            u32_value_reg_emmc_pll_pdiv = 2;
            break;

        case eMMC_PLL_CLK__52M:
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x42);
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x7627);
            u32_value_reg_emmc_pll_pdiv = 2;
            break;

        case eMMC_PLL_CLK__48M:
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x48);
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x0000);
            u32_value_reg_emmc_pll_pdiv = 2;
            break;

        case eMMC_PLL_CLK__40M:
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x2B);
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x3333);
            u32_value_reg_emmc_pll_pdiv = 4;
            break;

        case eMMC_PLL_CLK__36M:
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x30);
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x0000);
            u32_value_reg_emmc_pll_pdiv = 4;
            break;

        case eMMC_PLL_CLK__32M:
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x36);
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x0000);
            u32_value_reg_emmc_pll_pdiv = 4;
            break;

        case eMMC_PLL_CLK__27M:
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x40);
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x0000);
            u32_value_reg_emmc_pll_pdiv = 4;
            break;

        case eMMC_PLL_CLK__20M:
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x2B);
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x3333);
            u32_value_reg_emmc_pll_pdiv = 7;
            break;

        default:
            return eMMC_ST_ERR_UNKNOWN_CLK;
            break;
    }

    // 3. VCO clock ( loop N = 4 )
    REG_FCIE_CLRBIT(reg_emmcpll_fbdiv,0xffff);
    REG_FCIE_SETBIT(reg_emmcpll_fbdiv,0x6);

    // 4. 1X clock
    REG_FCIE_CLRBIT(reg_emmcpll_pdiv,BIT2|BIT1|BIT0);
    REG_FCIE_SETBIT(reg_emmcpll_pdiv,u32_value_reg_emmc_pll_pdiv);

    if(u16_ClkParam==eMMC_PLL_CLK__20M)
    {
        REG_FCIE_SETBIT(reg_emmc_pll_test, BIT10);
    }
    else
    {
        REG_FCIE_CLRBIT(reg_emmc_pll_test, BIT10);
    }

    eMMC_hw_timer_delay(HW_TIMER_DELAY_100us);

    return eMMC_ST_SUCCESS;
}



void HalEmmcPll_dll_setting(void)
{
    volatile U16 u16_reg;


    REG_FCIE_CLRBIT(reg_emmcpll_0x09, BIT0);

    // Reset eMMC_DLL
    REG_FCIE_SETBIT(REG_EMMC_PLL_RX30, BIT2);
    REG_FCIE_CLRBIT(REG_EMMC_PLL_RX30, BIT2);

    //DLL pulse width and phase
    REG_FCIE_W(REG_EMMC_PLL_RX01, 0x7F72);

    // DLL code
    REG_FCIE_W(REG_EMMC_PLL_RX32, 0xF200);

    // DLL calibration
    REG_FCIE_W(REG_EMMC_PLL_RX30, 0x3378);
    REG_FCIE_SETBIT(REG_EMMC_PLL_RX33, BIT15);

    // Wait 100us
    eMMC_hw_timer_delay(250);

    // Get hw dll0 code
    REG_FCIE_R(REG_EMMC_PLL_RX33, u16_reg);

    REG_FCIE_CLRBIT(REG_EMMC_PLL_RX34, (BIT10 - 1));
    // Set dw dll0 code
    REG_FCIE_SETBIT(REG_EMMC_PLL_RX34, u16_reg & 0x03FF);

    // Disable reg_hw_upcode_en
    REG_FCIE_CLRBIT(REG_EMMC_PLL_RX30, BIT9);

    // Clear reg_emmc_dll_test[7]
    REG_FCIE_CLRBIT(reg_emmcpll_0x02, BIT15);

    // Enable reg_rxdll_dline_en
    REG_FCIE_SETBIT(reg_emmcpll_0x09, BIT0);

}


U32 eMMC_clock_setting(U32 u32_ClkParam)
{
	U16 u16_Tmp;
	eMMC_DRIVER *g_eMMCDrv = (eMMC_DRIVER *)g_eMMCDrv_array;

	eMMC_PlatformResetPre();

	REG_FCIE_CLRBIT(FCIE_SD_MODE, BIT_SD_CLK_EN);

	REG_FCIE_CLRBIT(reg_ckg_fcie, BIT_FCIE_CLK_SRC_SEL);
	REG_FCIE_R(reg_ckg_fcie, u16_Tmp);
	eMMC_hw_timer_delay(7);
	REG_FCIE_CLRBIT(reg_ckg_fcie, BIT_FCIE_CLK_Gate|BIT_FCIE_CLK_MASK);

	if (u32_ClkParam & eMMC_PLL_FLAG) {

        if ((REG_FCIE(reg_ckg_fcie_syn) & 0x3) != BIT0) {
            REG_FCIE_CLRBIT(reg_ckg_fcie_syn, BIT1|BIT0);
            REG_FCIE_SETBIT(reg_ckg_fcie_syn, BIT0);
        }

		eMMC_pll_setting(u32_ClkParam);

		if((g_eMMCDrv->u32_Flag & (FCIE_FLAG_SPEED_HS400 | FCIE_FLAG_DDR_MODE)) ==
			(FCIE_FLAG_SPEED_HS400 | FCIE_FLAG_DDR_MODE)) {

			REG_FCIE_SETBIT(reg_ckg_fcie, BIT_FCIE_CLK_EMMC_PLL_2X<<2);
			REG_FCIE_SETBIT(reg_ckg_fcie, BIT_FCIE_CLK_SRC_SEL);

			HalEmmcPll_dll_setting(); // tuning DLL setting
		} else {
			REG_FCIE_SETBIT(reg_ckg_fcie, BIT_FCIE_CLK_EMMC_PLL_1X<<2);
			REG_FCIE_SETBIT(reg_ckg_fcie, BIT_FCIE_CLK_SRC_SEL);
		}
	} else {
		REG_FCIE_SETBIT(reg_ckg_fcie, u32_ClkParam<<2);
		REG_FCIE_SETBIT(reg_ckg_fcie, BIT_FCIE_CLK_SRC_SEL);
	}

	eMMC_PlatformResetPost();

	return eMMC_ST_SUCCESS;
}

void eMMC_set_WatchDog(U8 u8_IfEnable)
{
	u8_IfEnable = u8_IfEnable;
	// do nothing
}

void eMMC_reset_WatchDog(void)
{
	// do nothing
}

U32 eMMC_translate_DMA_address_Ex(U32 u32_DMAAddr, U32 u32_ByteCnt)
{
	u32_ByteCnt = u32_ByteCnt;
	#if 0
	flush_cache(u32_DMAAddr, u32_ByteCnt);
	#endif

	// Need to set bank 0x1006 16 bits addr 0x7A bit9 on to make below code work,
	// otherwise, always DMA to MIU0

	REG_FCIE_CLRBIT(FCIE_MMA_PRI_REG, BIT_MIU_SELECT_MASK);
	if (u32_DMAAddr < CONFIG_MIU0_BUSADDR)
		return (u32_DMAAddr & 0xFFFF);
	else if (u32_DMAAddr < CONFIG_MIU1_BUSADDR)
		return (u32_DMAAddr - CONFIG_MIU0_BUSADDR);
	else {
		REG_FCIE_SETBIT(FCIE_MMA_PRI_REG, BIT_MIU1_SELECT);
		return (u32_DMAAddr - CONFIG_MIU1_BUSADDR);
	}

	return u32_DMAAddr;
}

void eMMC_Invalidate_data_cache_buffer(U32 u32_addr, S32 s32_size)
{
	u32_addr = u32_addr;
	s32_size = s32_size;
//	flush_cache(u32_addr, s32_size);
}

void eMMC_flush_miu_pipe(void)
{
}

U32 eMMC_PlatformResetPre(void)
{
	if ((REG_FCIE(reg_ckg_fcie_syn) & 0x3) != BIT0 )
	{
		REG_FCIE_CLRBIT(reg_ckg_fcie_syn, 0x3);
		REG_FCIE_SETBIT(reg_ckg_fcie_syn, BIT0);
	}

	return eMMC_ST_SUCCESS;
}

U32 eMMC_PlatformResetPost(void)
{
	return eMMC_ST_SUCCESS;
}


U32 eMMC_PlatformInit(void)
{
	if ((REG_FCIE(reg_emmcpll_0x1a) & BIT0) == 0) {
		//eMMC_debug(0, 0, "eMMC Err: not 1.8V IO setting\n");
		REG_FCIE_SETBIT(reg_emmcpll_0x1a, BIT0);// 1.8V must set this bit
		REG_FCIE_SETBIT(reg_emmcpll_0x1a, BIT2); // atop patch
		REG_FCIE_CLRBIT(reg_emmcpll_0x45, 0xffff);// 1.8V must set this bit
		REG_FCIE_SETBIT(reg_emmcpll_0x45, 0xf<<8);// 1.8V must set this bit
	}

	eMMC_clock_setting(FCIE_SLOWEST_CLK);
	eMMC_pads_switch(FCIE_eMMC_BYPASS);

	return eMMC_ST_SUCCESS;
}

#elif (defined(eMMC_DRV_M7632_BOOTRAM) && eMMC_DRV_M7632_BOOTRAM)
void putk_emmc(char c)
{
	while((REG_FCIE(REG_UART_LSR) & UART_LSR_THRE) != UART_LSR_THRE);
	REG_FCIE_W(REG_UART_TX, c);
	while((REG_FCIE(REG_UART_LSR) & (UART_LSR_TEMT | UART_LSR_THRE)) != (UART_LSR_TEMT | UART_LSR_THRE));

}

void eMMC_DumpPadClk(void)
{

}

U32 eMMC_pads_switch(U8 u8Mode)
{
    eMMC_DRIVER *g_eMMCDrv = (eMMC_DRIVER *)g_eMMCDrv_array;

    g_eMMCDrv->u8_PadType = u8Mode;
    // gpio function mux
    REG_FCIE_CLRBIT(reg_nand_mode, BIT_NAND_MODE);
    REG_FCIE_SETBIT(reg_gpio_mux_0x30, BIT_EMMC_CONFIG_EMMC_MODE_1);
    REG_FCIE_CLRBIT(reg_gpio_mux_0x70, BIT_ALL_PAD_IN);

    // fcie
    REG_FCIE_CLRBIT(FCIE_DDR_MODE, BIT_FALL_LATCH|BIT_PAD_IN_SEL_SD|BIT_CLK2_SEL|BIT_32BIT_MACRO_EN|BIT_DDR_EN|BIT_8BIT_MACRO_EN);

    // emmc_pll
    REG_FCIE_CLRBIT(reg_emmcpll_0x09, BIT0);
    REG_FCIE_CLRBIT(reg_emmcpll_0x1a, BIT10|BIT5|BIT4);
    REG_FCIE_CLRBIT(reg_emmcpll_0x1c, BIT8|BIT9);
    REG_FCIE_CLRBIT(reg_emmcpll_0x20, BIT9|BIT10);
    REG_FCIE_CLRBIT(reg_emmcpll_0x63, BIT0);
    REG_FCIE_CLRBIT(reg_emmcpll_0x68, BIT0|BIT1);
    REG_FCIE_CLRBIT(reg_emmcpll_0x69, BIT7|BIT6|BIT5|BIT4|BIT3);
    REG_FCIE_CLRBIT(reg_emmcpll_0x6a, BIT0|BIT1);
    REG_FCIE_W(reg_emmcpll_0x6b, 0x0000);
    REG_FCIE_CLRBIT(reg_emmcpll_0x6d, BIT0);
    REG_FCIE_CLRBIT(reg_emmcpll_0x70, BIT11|BIT10|BIT8);
    REG_FCIE_CLRBIT(reg_emmcpll_0x7f, BIT10|BIT9|BIT8|BIT3|BIT2|BIT1);

    switch (u8Mode) {

    case FCIE_eMMC_BYPASS:

        // fcie
        REG_FCIE_SETBIT(FCIE_DDR_MODE,BIT_PAD_IN_SEL_SD|BIT_FALL_LATCH|BIT_CLK2_SEL);

        // emmc_pll
        REG_FCIE_SETBIT(reg_emmcpll_0x1a, BIT10);
        REG_FCIE_W(reg_emmcpll_0x71, 0xFFFF);
        REG_FCIE_W(reg_emmcpll_0x73, 0xFFFF);

        break;

    case FCIE_eMMC_HS200:

        // fcie
        REG_FCIE_SETBIT(FCIE_DDR_MODE, BIT_32BIT_MACRO_EN);

        // emmc_pll
        REG_FCIE_CLRBIT(reg_emmcpll_0x03, 0x0FFF);
        REG_FCIE_SETBIT(reg_emmcpll_0x1a, BIT5|BIT4);
        REG_FCIE_SETBIT(reg_emmcpll_0x1c, BIT8);
        REG_FCIE_SETBIT(reg_emmcpll_0x20, BIT9|BIT10);
        REG_FCIE_SETBIT(reg_emmcpll_0x68, BIT0);
        REG_FCIE_SETBIT(reg_emmcpll_0x69, 4<<4);
        REG_FCIE_SETBIT(reg_emmcpll_0x6a, 2<<0);
        REG_FCIE_W(reg_emmcpll_0x6b, 0x0213);
        REG_FCIE_SETBIT(reg_emmcpll_0x70, BIT8);
        REG_FCIE_W(reg_emmcpll_0x71, 0xF800);
        REG_FCIE_W(reg_emmcpll_0x73, 0xFD00);
        #if defined(ENABLE_eMMC_AFIFO) && ENABLE_eMMC_AFIFO
        REG_FCIE_SETBIT(reg_emmcpll_0x70, BIT10|BIT11);
        #endif

        break;

    case FCIE_eMMC_HS400:

        // fcie
        REG_FCIE_SETBIT(FCIE_DDR_MODE, BIT_32BIT_MACRO_EN|BIT_DDR_EN);

        // emmc_pll
        REG_FCIE_CLRBIT(reg_emmcpll_0x03, 0x0F0F);
        REG_FCIE_SETBIT(reg_emmcpll_0x09, BIT0);
        REG_FCIE_SETBIT(reg_emmcpll_0x1a, BIT5|BIT4);
        REG_FCIE_SETBIT(reg_emmcpll_0x1c, BIT8);
        REG_FCIE_SETBIT(reg_emmcpll_0x20, BIT10);
        REG_FCIE_SETBIT(reg_emmcpll_0x63, BIT0);
        REG_FCIE_SETBIT(reg_emmcpll_0x68, BIT0|BIT1);
        REG_FCIE_SETBIT(reg_emmcpll_0x69, 6<<4);
        REG_FCIE_SETBIT(reg_emmcpll_0x6a, 2<<0);
        REG_FCIE_W(reg_emmcpll_0x6b, 0x0113);
        REG_FCIE_SETBIT(reg_emmcpll_0x70, BIT8);
        REG_FCIE_W(reg_emmcpll_0x71, 0xF800);
        REG_FCIE_W(reg_emmcpll_0x73, 0xFD00);
        #if defined(ENABLE_eMMC_AFIFO) && ENABLE_eMMC_AFIFO
        REG_FCIE_SETBIT(reg_emmcpll_0x70, BIT10|BIT11);
        REG_FCIE_SETBIT(reg_emmcpll_0x7f, BIT2);
        #if defined(ENABLE_HS400_ENHANCED_STORBE) && ENABLE_HS400_ENHANCED_STORBE
        if (g_eMMCDrv->u8_ECSD184_Stroe_Support)
        {
            REG_FCIE_CLRBIT(reg_emmcpll_0x20, BIT10);
            REG_FCIE_SETBIT(reg_emmcpll_0x7f, BIT8);
        }
        #endif
        #endif

        break;

    default:
        return eMMC_ST_ERR_PARAMETER;
        break;
    }

    // 8 bits macro reset + 32 bits macro reset
    REG_FCIE_CLRBIT(reg_emmcpll_0x6f, BIT0|BIT1);
    REG_FCIE_SETBIT(reg_emmcpll_0x6f, BIT0|BIT1); // 8 bits macro reset + 32 bits macro reset

    return eMMC_ST_SUCCESS;
}

void eMMC_pads_switch_default(void)
{
    // fcie
    REG_FCIE_SETBIT(FCIE_DDR_MODE, BIT1|BIT2|BIT3);
}

U32 eMMC_pll_setting(U16 u16_ClkParam)
{
    U32 u32_value_reg_emmc_pll_pdiv;


    // 1. reset emmc pll
    REG_FCIE_SETBIT(reg_emmc_pll_reset,BIT0);
    REG_FCIE_CLRBIT(reg_emmc_pll_reset,BIT0);

    // 2. synth clock
    switch(u16_ClkParam)
    {
        case eMMC_PLL_CLK_200M: // 200M
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            #if 0
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x22); // 20xMHz
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x8F5C);
            #else
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x24); // 195MHz
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x03D8);
            #endif
            u32_value_reg_emmc_pll_pdiv =1;// PostDIV: 2
            break;

        case eMMC_PLL_CLK_160M:
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x2B);
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x3333);
            u32_value_reg_emmc_pll_pdiv =1;
            break;

        case eMMC_PLL_CLK_140M:
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x31);
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x5F15);
            u32_value_reg_emmc_pll_pdiv = 1;
            break;

        case eMMC_PLL_CLK_120M:
            REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
            REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
            REG_FCIE_SETBIT(reg_ddfset_23_16,0x39);
            REG_FCIE_SETBIT(reg_ddfset_15_00,0x9999);
            u32_value_reg_emmc_pll_pdiv = 1;
            break;

        default:
            return eMMC_ST_ERR_UNKNOWN_CLK;
            break;
    }

    // 3. VCO clock ( loop N = 4 )
    REG_FCIE_CLRBIT(reg_emmcpll_fbdiv,0xffff);
    REG_FCIE_SETBIT(reg_emmcpll_fbdiv,0x6);

    // 4. 1X clock
    REG_FCIE_CLRBIT(reg_emmcpll_pdiv,BIT2|BIT1|BIT0);
    REG_FCIE_SETBIT(reg_emmcpll_pdiv,u32_value_reg_emmc_pll_pdiv);

    if(u16_ClkParam==eMMC_PLL_CLK__20M)
    {
        REG_FCIE_SETBIT(reg_emmc_pll_test, BIT10);
    }
    else
    {
        REG_FCIE_CLRBIT(reg_emmc_pll_test, BIT10);
    }

    eMMC_hw_timer_delay(HW_TIMER_DELAY_100us);

    return eMMC_ST_SUCCESS;
}

void HalEmmcPll_dll_setting(void)
{
    volatile U16 u16_reg;


    REG_FCIE_CLRBIT(reg_emmcpll_0x09, BIT0);

    // Reset eMMC_DLL
    REG_FCIE_SETBIT(REG_EMMC_PLL_RX30, BIT2);
    REG_FCIE_CLRBIT(REG_EMMC_PLL_RX30, BIT2);

    //DLL pulse width and phase
    REG_FCIE_W(REG_EMMC_PLL_RX01, 0x7F72);

    // DLL code
    REG_FCIE_W(REG_EMMC_PLL_RX32, 0xF200);

    // DLL calibration
    REG_FCIE_W(REG_EMMC_PLL_RX30, 0x3378);
    REG_FCIE_SETBIT(REG_EMMC_PLL_RX33, BIT15);

    // Wait 100us
    eMMC_hw_timer_delay(250);

    // Get hw dll0 code
    REG_FCIE_R(REG_EMMC_PLL_RX33, u16_reg);

    REG_FCIE_CLRBIT(REG_EMMC_PLL_RX34, (BIT10 - 1));
    // Set dw dll0 code
    REG_FCIE_SETBIT(REG_EMMC_PLL_RX34, u16_reg & 0x03FF);

    // Disable reg_hw_upcode_en
    REG_FCIE_CLRBIT(REG_EMMC_PLL_RX30, BIT9);

    // Clear reg_emmc_dll_test[7]
    REG_FCIE_CLRBIT(reg_emmcpll_0x02, BIT15);

    // Enable reg_rxdll_dline_en
    REG_FCIE_SETBIT(reg_emmcpll_0x09, BIT0);

}


U32 eMMC_pll_setting_OvertonePatch(void)
{
    U32 u32_value_reg_emmc_pll_pdiv;

    // 1. reset emmc pll
    REG_FCIE_SETBIT(reg_emmc_pll_reset,BIT0);
    REG_FCIE_CLRBIT(reg_emmc_pll_reset,BIT0);

    REG_FCIE_CLRBIT(reg_ddfset_23_16,0xffff);
    REG_FCIE_CLRBIT(reg_ddfset_15_00,0xffff);
    #if 0
    REG_FCIE_SETBIT(reg_ddfset_23_16,0x22); // 20xMHz
    REG_FCIE_SETBIT(reg_ddfset_15_00,0x8F5C);
    #else
    REG_FCIE_SETBIT(reg_ddfset_23_16,0x24); // 195MHz
    REG_FCIE_SETBIT(reg_ddfset_15_00,0x03D8);
    #endif
    u32_value_reg_emmc_pll_pdiv =1;// PostDIV: 2

    // 2. overtone patch
    REG_FCIE_W(reg_emmcpll_0x03, 0xffff);
    REG_FCIE_SETBIT(reg_emmcpll_0x1b, BIT0);
    REG_FCIE_CLRBIT(reg_emmcpll_0x04, BIT8-1);
    REG_FCIE_SETBIT(reg_emmcpll_0x04, 0x7);
    eMMC_hw_timer_delay(100);
    REG_FCIE_CLRBIT(reg_emmcpll_0x04, BIT0);

    // 3. VCO clock ( loop N = 4 )
    REG_FCIE_CLRBIT(reg_emmcpll_fbdiv,0xffff);
    REG_FCIE_SETBIT(reg_emmcpll_fbdiv,0x6);

    // 4. 1X clock
    REG_FCIE_CLRBIT(reg_emmcpll_pdiv,BIT2|BIT1|BIT0);
    REG_FCIE_SETBIT(reg_emmcpll_pdiv,u32_value_reg_emmc_pll_pdiv);

    REG_FCIE_CLRBIT(reg_emmc_pll_test, BIT10);
    eMMC_hw_timer_delay(100);
    REG_FCIE_CLRBIT(reg_emmcpll_0x1b, BIT0);

    return eMMC_ST_SUCCESS;
}


U32 eMMC_clock_setting(U16 u16_ClkParam)
{
    #if defined(CONFIG_MSTAR_TITANIA_BD_FPGA) && CONFIG_MSTAR_TITANIA_BD_FPGA
    if(u16_ClkParam == BIT_FCIE_CLK_300K)
        REG_FCIE_SETBIT(reg_emmcpll_0x1d, BIT0);
    else
        REG_FCIE_CLRBIT(reg_emmcpll_0x1d, BIT0);
    #else
    U16 u16_Tmp;
    eMMC_DRIVER *g_eMMCDrv = (eMMC_DRIVER *)g_eMMCDrv_array;

    eMMC_PlatformResetPre();

    REG_FCIE_CLRBIT(FCIE_SD_MODE, BIT_SD_CLK_EN);

    REG_FCIE_CLRBIT(reg_ckg_fcie, BIT_FCIE_CLK_SRC_SEL);
    REG_FCIE_R(reg_ckg_fcie, u16_Tmp);
    eMMC_hw_timer_delay(7);
    REG_FCIE_CLRBIT(reg_ckg_fcie, BIT_FCIE_CLK_Gate|BIT_FCIE_CLK_MASK);

    if (u16_ClkParam & eMMC_PLL_FLAG) {

        eMMC_pll_setting(u16_ClkParam);

        if((g_eMMCDrv->u32_Flag & (FCIE_FLAG_SPEED_HS400 | FCIE_FLAG_DDR_MODE)) ==
            (FCIE_FLAG_SPEED_HS400 | FCIE_FLAG_DDR_MODE)) {

            REG_FCIE_SETBIT(reg_ckg_fcie, BIT_FCIE_CLK_EMMC_PLL_2X<<2);
            REG_FCIE_SETBIT(reg_ckg_fcie, BIT_FCIE_CLK_SRC_SEL);

            HalEmmcPll_dll_setting(); // tuning DLL setting
        } else {
            REG_FCIE_SETBIT(reg_ckg_fcie, BIT_FCIE_CLK_EMMC_PLL_1X<<2);
            REG_FCIE_SETBIT(reg_ckg_fcie, BIT_FCIE_CLK_SRC_SEL);
        }
    } else {
        REG_FCIE_SETBIT(reg_ckg_fcie, u16_ClkParam<<2);
        REG_FCIE_SETBIT(reg_ckg_fcie, BIT_FCIE_CLK_SRC_SEL);
    }

    eMMC_PlatformResetPost();
    #endif
    return eMMC_ST_SUCCESS;
}

void eMMC_set_WatchDog(U8 u8_IfEnable)
{
	u8_IfEnable = u8_IfEnable;
	// do nothing
}

void eMMC_reset_WatchDog(void)
{
	// do nothing
}

U32 eMMC_translate_DMA_address_Ex(U32 u32_DMAAddr, U32 u32_ByteCnt)
{
	u32_ByteCnt = u32_ByteCnt;
	#if 0
	flush_cache(u32_DMAAddr, u32_ByteCnt);
	#endif

	// Need to set bank 0x1006 16 bits addr 0x7A bit9 on to make below code work,
	// otherwise, always DMA to MIU0

	REG_FCIE_CLRBIT(FCIE_MMA_PRI_REG, BIT_MIU_SELECT_MASK);

	if ((u32_DMAAddr >> 16) == ((CONFIG_SRAM_BASE_ADDRESS) >> 16))
		return (u32_DMAAddr - CONFIG_SRAM_BASE_ADDRESS);

	if(u32_DMAAddr < CONFIG_MIU1_BUSADDR) // MIU0
	{
		return (u32_DMAAddr - CONFIG_MIU0_BUSADDR);
	}
	else  // MIU1
	{
		REG_FCIE_SETBIT(FCIE_MMA_PRI_REG, BIT_MIU1_SELECT);
		return (u32_DMAAddr - CONFIG_MIU1_BUSADDR);
	}

	return u32_DMAAddr;
}

void eMMC_Invalidate_data_cache_buffer(U32 u32_addr, S32 s32_size)
{
	u32_addr = u32_addr;
	s32_size = s32_size;
//	flush_cache(u32_addr, s32_size);
}

void eMMC_flush_miu_pipe(void)
{
}

U32 eMMC_PlatformResetPre(void)
{
    if ((REG_FCIE(reg_ckg_fcie_syn) & BIT_CKG_FCIE_SYN_MASK) != BIT_CKG_FCIE_SYN )
    {
        REG_FCIE_CLRBIT(reg_ckg_fcie_syn, BIT_CKG_FCIE_SYN_MASK);
        REG_FCIE_SETBIT(reg_ckg_fcie_syn, BIT_CKG_FCIE_SYN);
    }

    return eMMC_ST_SUCCESS;
}

U32 eMMC_PlatformResetPost(void)
{
    return eMMC_ST_SUCCESS;
}


U32 eMMC_PlatformInit(void)
{
    if ((REG_FCIE(reg_emmcpll_0x1a) & BIT0) == 0) {
        //eMMC_debug(0, 0, "eMMC Err: not 1.8V IO setting\n");
        REG_FCIE_SETBIT(reg_emmcpll_0x1a, BIT0);// 1.8V must set this bit
        REG_FCIE_SETBIT(reg_emmcpll_0x1a, BIT2); // atop patch
        REG_FCIE_CLRBIT(reg_emmcpll_0x45, 0xffff);// 1.8V must set this bit
        REG_FCIE_SETBIT(reg_emmcpll_0x45, 0xf);// 1.8V must set this bit
    }

    eMMC_clock_setting(FCIE_SLOWEST_CLK);
    eMMC_pads_switch(FCIE_eMMC_BYPASS);

    return eMMC_ST_SUCCESS;
}

#else
  #error "Error! no platform functions."
#endif
#endif
