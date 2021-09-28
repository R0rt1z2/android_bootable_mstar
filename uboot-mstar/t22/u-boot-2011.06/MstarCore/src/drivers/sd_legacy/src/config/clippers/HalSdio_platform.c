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

// this file is use only for clippers project

#include <config.h>
#include "drvFCIE_config.h"


#include <common.h>
//#include <stdio.h>
#include <mmc.h>
#include "drvFCIE5.h"

#if (defined PATCH_MINUS2PLUS2) && (PATCH_MINUS2PLUS2)
U32 gu32BestSkew4sub2;
U32 gu32BestSkew4add2;
#endif

extern unsigned char sd_ddr_mode;
extern void HalSdio_DumpDebugBus(void);
extern unsigned char gu8TuningMode;

/**************************************
* Config definition
**************************************/
#define DBG_CR_CLK(MSG)             //MSG
#define DBG_CR_PAD(MSG)             //MSG

/**************************************
* Local function definition
**************************************/


/**************************************
* Global function definition
**************************************/
void HalSdio_ResetIP(void)
{
	U16 u16Reg, u16Cnt;

	HalSdio_SetClock(48000);

	//printf("sdio reset ");
	REG_FCIE_W(FCIE_MIE_FUNC_CTL, BIT_SDIO_MOD);

	REG_FCIE_CLRBIT(FCIE_RST, BIT_FCIE_SOFT_RST_n); /* active low */

	//REG_FCIE_CLRBIT(R_SDIO_PLL_0x6F, BIT1|BIT0);		//macro reset

	// SDIO reset - wait
	u16Cnt=0;

	do
	{
		udelay(HW_TIMER_DELAY_1us);

		REG_FCIE_R(FCIE_RST, u16Reg);

	  	if(0x1000 == u16Cnt++)
		{
			printf("SD Err: SDIO Reset fail!! FCIE_RST = %04Xh\n", u16Reg);
			while(1);
		}

	} while (BIT_RST_STS_MASK  != (u16Reg  & BIT_RST_STS_MASK));

	REG_FCIE_SETBIT(FCIE_RST, BIT_FCIE_SOFT_RST_n);

	//REG_FCIE_SETBIT(R_SDIO_PLL_0x6F, BIT1|BIT0);

	u16Cnt=0;

	do
	{
		REG_FCIE_R(FCIE_RST, u16Reg);
		//printf("FCIE_RST = %04Xh\n", u16Reg);

	  	if(0x1000 == u16Cnt++)
		{
			printf("SD Err: SDIO Reset fail2:h \n");
			return ;
		}

		udelay(HW_TIMER_DELAY_1us);


	}while (0  != (u16Reg  & BIT_RST_STS_MASK));

	//printf("ok\n");

}

U32 HalSdio_TranslateVirtAddr(U32 u32_DMAAddr, U32 u32_ByteCnt)
{
	flush_cache(u32_DMAAddr, u32_ByteCnt);
	#ifdef CONFIG_MCU_ARM

	//printf("MIU0[%Xh], MIU1[%Xh]\n", CONFIG_MIU0_BUSADDR, CONFIG_MIU1_BUSADDR);
	if(u32_DMAAddr < CONFIG_MIU1_BUSADDR) // MIU0
	{
		//REG_FCIE_CLRBIT(FCIE_MIU_DMA_26_16, BIT_MIU1_SELECT);
		return  (u32_DMAAddr - CONFIG_MIU0_BUSADDR);
	}
	else
	{
		// REG_FCIE_SETBIT(FCIE_MIU_DMA_26_16, BIT_MIU1_SELECT);
		printf("MIU1\n");
		return  (u32_DMAAddr - CONFIG_MIU1_BUSADDR);
	}
	#endif
}

void HalSdio_Platform_InitChiptop(void)
{
	FCIE_RIU_16_OF(CHIPTOP_50,WBIT15);
	FCIE_RIU_16_OF(CHIPTOP_5A,WBIT08|WBIT09);
	FCIE_RIU_16_ON(CHIPTOP_5A,WBIT08);

	#ifdef BY_PASS_MODE

	FCIE_RIU_16_OF(CHIPTOP_7B,BIT06|BIT07);
	FCIE_RIU_16_ON(CHIPTOP_7B,BIT06);

	FCIE_RIU_16_OF(CHIPTOP_42,0x3F); // bit 0~6 driving mask
	FCIE_RIU_16_ON(CHIPTOP_42,0x0C); // more driving power suggest by Irwin

	FCIE_RIU_16_ON(CHIPTOP_42,0x3f00); // pull enable

	FCIE_RIU_16_ON(CHIPTOP_43,0x1f);

	FCIE_RIU_16_ON(CHIPTOP_4F,BIT02);			//SDIO PAD1
	FCIE_RIU_16_OF(CHIPTOP_43,WBIT08);
	FCIE_RIU_16_OF(CHIPTOP_50,WBIT15);

	#endif
}



#if !defined FPGA_BOARD || FPGA_BOARD==0

const U8 FcieClockIdxNum = 6;

const U32 FCIE_CLOCK[] =
{
    48000, //  0
    43200, //  1
    40000, //  2
    36000, //  3
    32000, //  4
    20000, //  5
      300, //  6
};

// find clock close to target but not over

U32 HalSdio_pll_setting(U16 u16_ClkParam)
{
	static U16 u16_OldClkParam=0xFFFF;
	U32 u32_value_reg_emmc_pll_pdiv;

	if(u16_ClkParam == u16_OldClkParam)
		return 0;
	else
		u16_OldClkParam = u16_ClkParam;

	// 1. reset emmc pll
	FCIE_RIU_16_ON(reg_emmc_pll_reset, BIT0);
	FCIE_RIU_16_OF(reg_emmc_pll_reset, BIT0);

	// 2. synth clock
	switch(u16_ClkParam) {
	    case eMMC_PLL_CLK_200M: // 200M
			FCIE_RIU_W16(reg_ddfset_23_16, 0x0022);
			FCIE_RIU_W16(reg_ddfset_15_00, 0x8F5C);
			// 195MHz
			//FCIE_RIU_W16(reg_ddfset_23_16, 0x0024);
			//FCIE_RIU_W16(reg_ddfset_15_00, 0x03D8);
			u32_value_reg_emmc_pll_pdiv =1;// PostDIV: 2
			break;
		case eMMC_PLL_CLK_160M:
			FCIE_RIU_W16(reg_ddfset_23_16, 0x002B);
			FCIE_RIU_W16(reg_ddfset_15_00, 0x3333);
			u32_value_reg_emmc_pll_pdiv =1;// PostDIV: 2
			break;
		case eMMC_PLL_CLK_140M:
			FCIE_RIU_W16(reg_ddfset_23_16, 0x0031);
			FCIE_RIU_W16(reg_ddfset_15_00, 0x5F15);
			u32_value_reg_emmc_pll_pdiv = 1;// PostDIV: 2
			break;
		case eMMC_PLL_CLK_120M:
			FCIE_RIU_W16(reg_ddfset_23_16, 0x0039);
			FCIE_RIU_W16(reg_ddfset_15_00, 0x9999);
			u32_value_reg_emmc_pll_pdiv = 1;// PostDIV: 2
			break;
		case eMMC_PLL_CLK_100M:
			FCIE_RIU_W16(reg_ddfset_23_16, 0x0045);
			FCIE_RIU_W16(reg_ddfset_15_00, 0x1EB8);
			u32_value_reg_emmc_pll_pdiv = 1;// PostDIV: 2
			break;
		case eMMC_PLL_CLK__86M:
			FCIE_RIU_W16(reg_ddfset_23_16, 0x0028);
			FCIE_RIU_W16(reg_ddfset_15_00, 0x2FA0);
			u32_value_reg_emmc_pll_pdiv = 2;// PostDIV: 4
			break;
		case eMMC_PLL_CLK__80M:
			FCIE_RIU_W16(reg_ddfset_23_16, 0x002B);
			FCIE_RIU_W16(reg_ddfset_15_00, 0x3333);
			u32_value_reg_emmc_pll_pdiv = 2;// PostDIV: 4
			break;
		case eMMC_PLL_CLK__72M:
			FCIE_RIU_W16(reg_ddfset_23_16, 0x0030);
			FCIE_RIU_W16(reg_ddfset_15_00, 0x0000);
			u32_value_reg_emmc_pll_pdiv = 2;// PostDIV: 4
			break;
		case eMMC_PLL_CLK__62M:
			FCIE_RIU_W16(reg_ddfset_23_16, 0x0037);
			FCIE_RIU_W16(reg_ddfset_15_00, 0xBDEF);
			u32_value_reg_emmc_pll_pdiv = 2;// PostDIV: 4
			break;
		case eMMC_PLL_CLK__52M:
			FCIE_RIU_W16(reg_ddfset_23_16, 0x0042);
			FCIE_RIU_W16(reg_ddfset_15_00, 0x7627);
			u32_value_reg_emmc_pll_pdiv = 2;// PostDIV: 4
			break;
		case eMMC_PLL_CLK__48M:
			FCIE_RIU_W16(reg_ddfset_23_16, 0x0048);
			FCIE_RIU_W16(reg_ddfset_15_00, 0x0000);
			u32_value_reg_emmc_pll_pdiv = 2;// PostDIV: 4
			break;
		case eMMC_PLL_CLK__40M:
			FCIE_RIU_W16(reg_ddfset_23_16, 0x002B);
			FCIE_RIU_W16(reg_ddfset_15_00, 0x3333);
			u32_value_reg_emmc_pll_pdiv = 4;// PostDIV: 8
			break;
		case eMMC_PLL_CLK__36M:
			FCIE_RIU_W16(reg_ddfset_23_16, 0x0030);
			FCIE_RIU_W16(reg_ddfset_15_00, 0x0000);
			u32_value_reg_emmc_pll_pdiv = 4;// PostDIV: 8
			break;
		case eMMC_PLL_CLK__32M:
			FCIE_RIU_W16(reg_ddfset_23_16, 0x0036);
			FCIE_RIU_W16(reg_ddfset_15_00, 0x0000);
			u32_value_reg_emmc_pll_pdiv = 4;// PostDIV: 8
			break;
		case eMMC_PLL_CLK__27M:
			FCIE_RIU_W16(reg_ddfset_23_16, 0x0040);
			FCIE_RIU_W16(reg_ddfset_15_00, 0x0000);
			u32_value_reg_emmc_pll_pdiv = 4;// PostDIV: 8
			break;
		case eMMC_PLL_CLK__20M:
			FCIE_RIU_W16(reg_ddfset_23_16, 0x002B);
			FCIE_RIU_W16(reg_ddfset_15_00, 0x3333);
			u32_value_reg_emmc_pll_pdiv = 7;// PostDIV: 16
			break;
		default:
			printf("SD Err: emmc PLL not configed %x\n", u16_ClkParam);
			return 0xff;
			break;
	}

	// 3. VCO clock ( loop N = 4 )
	FCIE_RIU_16_OF(reg_sdiopll_fbdiv, 0xFFFF);
	FCIE_RIU_16_ON(reg_sdiopll_fbdiv, 0x6);// PostDIV: 8

	// 4. 1X clock
	FCIE_RIU_16_OF(reg_sdiopll_pdiv, BIT2|BIT1|BIT0);
	FCIE_RIU_16_ON(reg_sdiopll_pdiv, u32_value_reg_emmc_pll_pdiv);// PostDIV: 8

	if(u16_ClkParam==eMMC_PLL_CLK__20M) {
	    FCIE_RIU_16_ON(reg_emmc_pll_test, BIT10);
	}
	else {
		FCIE_RIU_16_OF(reg_emmc_pll_test, BIT10);
	}

	mdelay(1); // asked by Irwin

	return 0;
}


U32 HalSdio_SetClock(U32 u32Clock)
{
    U8 u8ClockSlector;
	static U32 u32_OldClock=0xFFFFFFFF;

	//printf("HalSdio_SetClock(%ld)\n", u32Clock);

	if(u32_OldClock == u32Clock)
		return u32Clock;
	else
		u32_OldClock = u32Clock;

    FCIE_RIU_16_ON(REG_CLK_SDIO, WBIT00); // turn on clock gating

    if(u32Clock>1000)
    {
        DBG_CR_CLK(printf("Set SDIO clock as %d.%d MHz, ", u32Clock/1000, (u32Clock%1000)/100 ) );
    }
    else
    {
        DBG_CR_CLK(printf("Set SDIO clock as %d KHz, ", u32Clock));
    }

    for(u8ClockSlector=0; u8ClockSlector<=FcieClockIdxNum; u8ClockSlector++)
    {
        if( FCIE_CLOCK[u8ClockSlector] <= u32Clock )
		{
			break;
        }
    }

    if(u8ClockSlector>FcieClockIdxNum)
    {
        DBG_CR_CLK(printf("Error!!! Can not find proper clock!\r\n"));
		while(1);
        return 0x01;
    }

    if(u32Clock>1000)
    {
        DBG_CR_CLK(printf("select SDIO clock as %d.%d MHz\r\n", FCIE_CLOCK[u8ClockSlector]/1000, (FCIE_CLOCK[u8ClockSlector]%1000)/100));
    }
    else
    {
        DBG_CR_CLK(printf("select SDIO clock as %d KHz\r\n", FCIE_CLOCK[u8ClockSlector]));
    }

    FCIE_RIU_16_ON(REG_CLK_SDIO, WBIT06);
    FCIE_RIU_16_OF(REG_CLK_SDIO, WBIT05+WBIT04+WBIT03+WBIT02); // mask all clock select

	//printf("switch to clock: %d\n", u8ClockSlector);

    switch(u8ClockSlector)
    {
		case 0: // 48M
		    FCIE_RIU_16_ON(REG_CLK_SDIO, 0xF<<2);
			break;

		case 1: // 43.2M
		    FCIE_RIU_16_ON(REG_CLK_SDIO, 0x5<<2);
			break;

		case 2: // 40M
		    FCIE_RIU_16_ON(REG_CLK_SDIO, 0x4<<2);
			break;

		case 3: // 36M
		    FCIE_RIU_16_ON(REG_CLK_SDIO, 0x3<<2);
			break;

		case 4: // 32M
		    FCIE_RIU_16_ON(REG_CLK_SDIO, 0x2<<2);
			break;

		case 5: // 20M
		    FCIE_RIU_16_ON(REG_CLK_SDIO, 0x1<<2);
			break;

		case 6: // 300K
		    FCIE_RIU_16_ON(REG_CLK_SDIO, 0xD<<2);
			break;

		default:
			printf("SDIO Err: wrong clock selector!\n");
			while(1);
			break;

	}

	//DBG_CR_CLK(printf("REG_CLK_NFIE = 0x%04X\r\n", FCIE_RIU_R16(REG_CLK_SDIO)));

    FCIE_RIU_16_OF(REG_CLK_SDIO, WBIT00); // turn off clock gating

    return FCIE_CLOCK[u8ClockSlector];

}

static U32 gu32SdClock = 0;

#define BUS_SPEED_SDR12		1
#define BUS_SPEED_SDR25		2
#define BUS_SPEED_DDR50		3
#define BUS_SPEED_SDR50		4
#define BUS_SPEED_SDR104	5

//static U32 gu8BusSpeed = BUS_SPEED_SDR12;


U32 HalSdio_clock_setting(U16 u16_ClkParam)
{
    DBG_CR_CLK(printf("HalSdio_clock_setting(%Xh)\n", u16_ClkParam));

    REG_FCIE_CLRBIT(FCIE_SD_MODE, BIT_CLK_EN);

    switch(u16_ClkParam)
    {
		// emmc_pll clock
		case eMMC_PLL_CLK__20M	: gu32SdClock =  20000; break;
		case eMMC_PLL_CLK__27M	: gu32SdClock =  27000; break;
		case eMMC_PLL_CLK__32M	: gu32SdClock =  32000; break;
		case eMMC_PLL_CLK__36M	: gu32SdClock =  36000; break;
		case eMMC_PLL_CLK__40M	: gu32SdClock =  40000; break;
		case eMMC_PLL_CLK__48M	: gu32SdClock =  48000; break;
		case eMMC_PLL_CLK__52M	: gu32SdClock =  52000; break;
		case eMMC_PLL_CLK__62M	: gu32SdClock =  62000; break;
		case eMMC_PLL_CLK__72M	: gu32SdClock =  72000; break;
		case eMMC_PLL_CLK__80M	: gu32SdClock =  80000; break;
		case eMMC_PLL_CLK__86M	: gu32SdClock =  86000; break;
		case eMMC_PLL_CLK_100M	: gu32SdClock = 100000; break;
		case eMMC_PLL_CLK_120M	: gu32SdClock = 120000; break;
		case eMMC_PLL_CLK_140M	: gu32SdClock = 140000; break;
		case eMMC_PLL_CLK_160M	: gu32SdClock = 160000; break;
		case eMMC_PLL_CLK_200M	: gu32SdClock = 200000; break;

		// clock_gen sdio clock
        //case BIT_FCIE_CLK_12M	: gu32SdClock =  12000; break;
		case BIT_FCIE_CLK_20M	: gu32SdClock =  20000; break;
		case BIT_FCIE_CLK_32M	: gu32SdClock =  32000; break;
		case BIT_FCIE_CLK_36M	: gu32SdClock =  36000; break;
		case BIT_FCIE_CLK_40M	: gu32SdClock =  40000; break;
		case BIT_FCIE_CLK_43M	: gu32SdClock =  43200; break;
		case BIT_FCIE_CLK_300K	: gu32SdClock =    300; break;
        //case BIT_FCIE_CLK_24M	: gu32SdClock =  24000; break;
		case BIT_FCIE_CLK_48M	: gu32SdClock =  48000; break;

        default:
            printf("SdErr: wrong clock parameter\n");
            return 1;;
    }

	//printf("set clock to %d KHz\n", gu32SdClock);

    if(u16_ClkParam & eMMC_PLL_FLAG)
    {
		//printf("sdio pll clock\n");

        REG_FCIE_CLRBIT(REG_CLK_SDIO, BIT_SDIO_CLK_GATING|BIT_SDIO_CLK_INVERSE|BIT_CLKGEN_SDIO_MASK);

        HalSdio_pll_setting(u16_ClkParam);

        //if( gu8BusSpeed == BUS_SPEED_SDR104 ) // HS200
        {
            REG_FCIE_SETBIT(REG_CLK_SDIO, BIT_SDIO_CLK_SRC_SEL|0xB<<2);
        }
    }
    else
    {
		printf("clkgen clock\n");

        REG_FCIE_CLRBIT(REG_CLK_SDIO, BIT_SDIO_CLK_GATING|BIT_SDIO_CLK_INVERSE|BIT_CLKGEN_SDIO_MASK);
        REG_FCIE_SETBIT(REG_CLK_SDIO, BIT_SDIO_CLK_SRC_SEL|u16_ClkParam<<2);
    }


    return 0;
}

#else

U8 HalSdio_SetClock(U32 u32Clock)
{
	if (u32Clock < 400)
	{
		printf("SDIO FPGA clcok 187.5KHz\n");
		REG_FCIE_SETBIT(R_SDIO_PLL_0x1D, BIT0);
	}
	else
	{
		printf("SDIO FPGA clock 1.5MHz\n");
		REG_FCIE_CLRBIT(R_SDIO_PLL_0x1D, BIT0);
	}

	REG_FCIE_SETBIT(FCIE_SD_MODE, BIT_CLK_EN); // enable clk

	return 0;
}

#endif

static U8 u8CurrentPadType = SDIO_MODE_UNKNOWN;

U8 HalSdio_GetPadType(void)
{
	return u8CurrentPadType;
}

void HalSdio_SwitchPad(unsigned char u32Mode)
{
	//printf("switch pad %d, current pad type = %d\n", u32Mode, u8CurrentPadType);

	// chiptop

	// sdio
	REG_FCIE_CLRBIT(FCIE_DDR_MODE, BIT_FALL_LATCH|BIT_PAD_IN_SEL_SD|BIT_32BIT_MACRO_EN|BIT_DDR_EN|BIT_8BIT_MACRO_EN);
	//REG_FCIE_W(FCIE_DDR_TOGGLE_CNT, 0x0000);

	// sdio_pll
	REG_FCIE_CLRBIT(R_SDIO_PLL_0x68, BIT0|BIT1);
	//REG_FCIE_CLRBIT(R_SDIO_PLL_0x69, BIT7|BIT6|BIT5|BIT4|BIT3); // TODO: tuning after IC back
	REG_FCIE_CLRBIT(R_SDIO_PLL_0x6A, BIT0|BIT1); // 0 --> 1 bits
	REG_FCIE_W(R_SDIO_PLL_0x6B, 0x0000);

	// TODO: overcome the share register 0x6C trouble
	REG_FCIE_CLRBIT(R_SDIO_PLL_0x6C, BIT7|BIT6|BIT5|BIT2|BIT1|BIT0); // TODO: tuning for 8 bit macro DQS, delay cell

	REG_FCIE_CLRBIT(R_SDIO_PLL_0x6D, BIT0);
	REG_FCIE_CLRBIT(R_SDIO_PLL_0x70, BIT8); // reg_sel_flash_32bit_macro_interface
	REG_FCIE_W(		R_SDIO_PLL_0x71, 0xFFFF);
	REG_FCIE_W(		R_SDIO_PLL_0x73, 0xFFFF);
	REG_FCIE_SETBIT(R_SDIO_PLL_0x74, BIT15);

	switch(u32Mode)
	{
		case SDIO_MODE_GPIO_PAD_BPS:
			DBG_CR_PAD(printf(LIGHT_CYAN"SDIO_MODE_GPIO_PAD_BPS\n"NONE));
			u8CurrentPadType = SDIO_MODE_GPIO_PAD_BPS;
			break;

		/*case SDIO_MODE_GPIO_PAD_SDR:
			printf"SDIO_MODE_GPIO_PAD_SDR\n");
			// sdio
			REG_FCIE_SETBIT(FCIE_DDR_MODE, BIT_FALL_LATCH|BIT_PAD_IN_SEL_SD);
			break;*/

		////////////////////////////////////////////////////////////////////////////////////////////
		case SDIO_MODE_8BITS_MACRO_SDR:
			DBG_CR_PAD(printf(LIGHT_CYAN"SDIO_MODE_8BITS_MACRO_SDR\n"NONE));
			u8CurrentPadType = SDIO_MODE_8BITS_MACRO_SDR;
			// sdio
			REG_FCIE_SETBIT(FCIE_DDR_MODE, BIT_8BIT_MACRO_EN);
			// sdio_pll
			REG_FCIE_SETBIT(R_SDIO_PLL_0x68, BIT0);
			REG_FCIE_SETBIT(R_SDIO_PLL_0x6A, BIT0);
			REG_FCIE_W(R_SDIO_PLL_0x6B, 0x0413); // 4 bits
			break;

		case SDIO_MODE_8BITS_MACRO_DDR:
			u8CurrentPadType = SDIO_MODE_8BITS_MACRO_DDR;
			DBG_CR_PAD(printf(LIGHT_CYAN"FCIE_MODE_8BITS_MACRO_DDR52\n"NONE));
			// sdio
			REG_FCIE_SETBIT(FCIE_DDR_MODE, BIT_DDR_EN|BIT_8BIT_MACRO_EN);

			// sdio_pll
			REG_FCIE_SETBIT(R_SDIO_PLL_0x68, BIT0);
			REG_FCIE_SETBIT(R_SDIO_PLL_0x6A, BIT0);
			REG_FCIE_W(		R_SDIO_PLL_0x6B, 0x0213); // 4 bits

			//REG_FCIE_W(		R_SDIO_PLL_0x6D, 2); // DQS: 2

			REG_FCIE_SETBIT(R_SDIO_PLL_0x6D, BIT0);
			break;

		////////////////////////////////////////////////////////////////////////////////////////////
		case SDIO_MODE_32BITS_MACRO_DDR:
			u8CurrentPadType = SDIO_MODE_32BITS_MACRO_DDR;
			DBG_CR_PAD(printf(LIGHT_CYAN"SDIO_MODE_32BITS_MACRO_DDR\n"NONE));
			// sdio
			REG_FCIE_SETBIT(FCIE_DDR_MODE, BIT_32BIT_MACRO_EN|BIT_DDR_EN);

			// sdio_pll
			//REG_FCIE_SETBIT(reg_emmcpll_0x03, 0x0004);
			REG_FCIE_W(R_SDIO_PLL_0x6B, 0x0213);


			REG_FCIE_SETBIT(R_SDIO_PLL_0x68, BIT0|BIT1);

			// TODO: tuning after IC back
			REG_FCIE_CLRBIT(R_SDIO_PLL_0x69, 0xF<<4);
			REG_FCIE_SETBIT(R_SDIO_PLL_0x69, 4<<4);

			REG_FCIE_SETBIT(R_SDIO_PLL_0x69, BIT3);

			REG_FCIE_SETBIT(R_SDIO_PLL_0x6A, BIT1);
			REG_FCIE_SETBIT(R_SDIO_PLL_0x70, BIT8);
			REG_FCIE_W(R_SDIO_PLL_0x71, 0x0000);
			break;

		case SDIO_MODE_32BITS_MACRO_SDR104:
			u8CurrentPadType = SDIO_MODE_32BITS_MACRO_SDR104;
			DBG_CR_PAD(printf(LIGHT_CYAN"SDIO_MODE_32BITS_MACRO_HS200\n"NONE));
			// sdio
			REG_FCIE_SETBIT(FCIE_DDR_MODE, BIT_32BIT_MACRO_EN);
			REG_FCIE_W(FCIE_DDR_TOGGLE_CNT, 0x0410);

			// emmc_pll
			HalSdio_SetSkew(1, 0);
			HalSdio_SetSkew(2, 0);
			HalSdio_SetSkew(3, 0);
			HalSdio_SetSkew(4, 0);
			HalSdio_SetTriggerLevel(0);

			REG_FCIE_CLRBIT(R_SDIO_PLL_0x1D, WCRC_DELAY_LATCH_MSK); // clear mask
			REG_FCIE_SETBIT(R_SDIO_PLL_0x1D, BIT2); // set wcrc delay latch to 2

			REG_FCIE_W(R_SDIO_PLL_0x6B, 0x0413); // 4 bits

			REG_FCIE_SETBIT(R_SDIO_PLL_0x68, BIT0);

			// TODO: tuning after IC back
			REG_FCIE_CLRBIT(R_SDIO_PLL_0x69, 0xF<<4);
			REG_FCIE_SETBIT(R_SDIO_PLL_0x69, 4<<4);

			//REG_FCIE_SETBIT(R_SDIO_PLL_0x69, BIT3);

			REG_FCIE_SETBIT(R_SDIO_PLL_0x6A, BIT0);
			REG_FCIE_SETBIT(R_SDIO_PLL_0x70, BIT8);
			REG_FCIE_W(R_SDIO_PLL_0x71, 0x0000);


			REG_FCIE_SETBIT(R_SDIO_PLL_0x1A, BIT00); // driving

			REG_FCIE_CLRBIT(R_SDIO_PLL_0x1D, BIT04|BIT05|BIT06);
			REG_FCIE_SETBIT(R_SDIO_PLL_0x1D, BIT05);		// response latch

			REG_FCIE_SETBIT(reg_emmc_test, BIT2); // helps in voltage & temp variation

			REG_FCIE_SETBIT(reg_atop_patch, BIT8|BIT9); // ECO patch

			break;

		default:
			u8CurrentPadType = SDIO_MODE_UNKNOWN;
			DBG_CR_PAD(printf(LIGHT_CYAN"SDIO Err: wrong parameter for switch pad func\n"NONE));
			//return 0x01;
			break;
	}

	// 8 bits macro reset + 32 bits macro reset
	//REG_FCIE_CLRBIT(R_SDIO_PLL_0x6F, BIT1|BIT0);
	//REG_FCIE_SETBIT(R_SDIO_PLL_0x6F, BIT1|BIT0);

	//return 0x00;

//ErrorHandle:

	//printf("SDIO Err: set bus width before pad switch\n");
	//return 0x02;

}


U8 HalSdio_Platform_ClearEvent(U16 nReq)
{
    U16  u16Tmp;

    u16Tmp = 0x0080;
    while((FCIE_RIU_R16(FCIE_MIE_EVENT)& nReq) != 0x00)
    {
        FCIE_RIU_W16(FCIE_MIE_EVENT, nReq); // write 1 clear register
        if (u16Tmp==0)
        {
            printf("Error!!! Can not clear MIE event.\r\n");
            return(1);
        }
        else
        {
            u16Tmp--;
        }
    }

    return 0;
}


E_IO_STS HalSdio_WaitMieEvent(U16 u16ReqVal, U32 u32WaitMs)
{
    U32 u32Count = 0;
    //u32WaitMs *= 100;
	U16 u16Event;

	while(1)
	{
		u16Event = FCIE_RIU_R16(FCIE_MIE_EVENT);

		if(u16Event&BIT_ERR_STS)
		{
			//printf("\33[1;31mSDIO detect error, SD_STATUS = %04Xh\33[m\n", FCIE_RIU_R16(FCIE_SD_STATUS));
			HalSdio_Platform_ClearEvent(BIT_ERR_STS);
			return IO_ERROR;
		}
		else if((u16Event&u16ReqVal)==u16ReqVal)
		{
			//printf(" Got event %04X ", u16Event);
			break;
		}

		udelay(1000);

		u32Count++;

		if(u32Count>u32WaitMs)
		{
			printf("u32Counter=%d\r\n", u32Count);
			printf("\r\n");
			printf("------------------------------------------\r\n");
			printf("ERROR!!! MIE EVENT TIME OUT!!!\n");
			printf("request event = %04Xh, event = %04Xh\n", u16ReqVal, u16Event);
			printf("------------------------------------------\r\n");

			if(!(gu8TuningMode&TUNE_NOW))
			{
				HalSdio_DumpRegister();
				HalSdio_DumpDebugBus();
				//while(1);
			}

			return(IO_TIME_OUT);
		}
	}

    ///////////////////////////////////////////////////////////////////////////////////////////
    // Clear mie event
    // Only can clear request value,
    // because read operation might enable command and data transfer at the same time

    if (HalSdio_Platform_ClearEvent(u16ReqVal))
    {
	    return (IO_TIME_OUT);
    }

    return(IO_SUCCESS);
} // wait MIE Event End


U32 HalSdio_FindSkewer(U32 u32_Candidate, SKEWER * st_skewer)
{
	U32 u32_i;
	st_skewer->u32LatchOKStart = 0xBEEF;
	st_skewer->u32LatchOKEnd = 0xBEEF;
	st_skewer->u8_edge = 0;

	//printf("%s(%05Xh)\n", __FUNCTION__, u32_Candidate);

	for(u32_i=0; u32_i<20; u32_i++) {
		if( (u32_Candidate&(1<<u32_i)) ^ ((u32_Candidate>>1)&(1<<u32_i)) ) { // use XOR to find 0b01/0b10 edge
			st_skewer->u8_edge++;
			//printf("%d [%04Xh ^ %04Xh] = %04Xh, ", u32_i, (u32_Candidate&(1<<u32_i)), ((u32_Candidate>>1)&(1<<u32_i)),
			//	( (u32_Candidate&(1<<u32_i)) ^ ((u32_Candidate>>1)&(1<<u32_i)) ) );
			if(st_skewer->u32LatchOKStart==0xBEEF) {
				st_skewer->u32LatchOKStart = u32_i;
			} else if(st_skewer->u32LatchOKEnd==0xBEEF) {
				st_skewer->u32LatchOKEnd = u32_i;
			}
		}
	}

	st_skewer->u32LatchOKEnd--;

	//eMMC_printf("u8_edge = %d, ", st_skewer->u8_edge);
	//eMMC_printf("start = %d, ", st_skewer->u32LatchOKStart);
	//eMMC_printf("end = %d\n", st_skewer->u32LatchOKEnd);

	return 0;
}

#if (defined PATCH_MINUS2PLUS2) && (PATCH_MINUS2PLUS2)

U32 HalSdio_Skew4MetaPatch(U32 u32BestSkew4)
{
	if(u32BestSkew4 >= 2)
	{
		gu32BestSkew4sub2 = u32BestSkew4 - 2;
	}
	else // 0 --> 16, 1 --> 17
	{
		gu32BestSkew4sub2 = u32BestSkew4 + 16;
	}

	if(u32BestSkew4 <= 15)
	{
		gu32BestSkew4add2 = u32BestSkew4 + 2;
	}
	else // 16 --> 0, 17 --> 1
	{
		gu32BestSkew4add2 = u32BestSkew4 - 16;
	}

	printf("Best %d, -2 --> %d, +2 --> %d\n", u32BestSkew4, gu32BestSkew4sub2, gu32BestSkew4add2);

	return gu32BestSkew4sub2;

}

#endif

U32 HalSdio_SlectBestPhase(U32 u32_Candidate, SKEWER * pSkewer)
{
	SKEWER st_skewer1 = {0};
	SKEWER st_skewer2 = {0};
	U32 u32_LatchBestSelect = 0;
	U32 u32_Temp;
	U32 u32Ret = 0;

	//printf("\n%s(%05Xh)\n", __FUNCTION__, u32_Candidate);

	if(!u32_Candidate) {
		printf("Scan fail: no workable phase\n");
		u32Ret = 1;
		goto ErrorHandle;
	}

	u32_Candidate<<=1; // bit 0~17 --> bit 1~18, add 0 former and later
	HalSdio_FindSkewer(u32_Candidate, &st_skewer1);

	u32_Candidate |= 0x00180001; // add 1 former and later
	HalSdio_FindSkewer(u32_Candidate, &st_skewer2);

	u32_Candidate &= ~0x00180001; // bit 1~18
	u32_Candidate>>=1; // bit 1~18 --> bit 0~17, shift back

	if( (st_skewer1.u8_edge==2) || (st_skewer2.u8_edge==2) ) { // only 1 continuous workable skew4 group

		if(st_skewer1.u8_edge==2) {
			// select best skew4
			if((st_skewer1.u32LatchOKStart + st_skewer1.u32LatchOKEnd)%2) { // odd

				u32_LatchBestSelect = (st_skewer1.u32LatchOKStart + st_skewer1.u32LatchOKEnd) >> 1;
				//u32_LatchBestSelect++; // select later
			}
			else { // even

				u32_LatchBestSelect = (st_skewer1.u32LatchOKStart + st_skewer1.u32LatchOKEnd) >> 1;
			}
		}
		else {
			memcpy(&st_skewer1, &st_skewer2, sizeof(SKEWER));
			//eMMC_printf("ring type:\n");
			st_skewer1.u32LatchOKStart--;
			st_skewer1.u32LatchOKEnd++;

			//eMMC_printf("start = %d, ", st_skewer1.u32LatchOKStart);
			//eMMC_printf("end = %d\n", st_skewer1.u32LatchOKEnd);

			u32_Temp = ((st_skewer1.u32LatchOKStart+1)+(18-st_skewer1.u32LatchOKEnd)) >> 1;
			if(st_skewer1.u32LatchOKStart>=u32_Temp) {
				//printf("enough\n");
				u32_LatchBestSelect = st_skewer1.u32LatchOKStart - u32_Temp;
			}
			else {
				u32_LatchBestSelect = st_skewer1.u32LatchOKEnd + u32_Temp;
			}
			if(u32_LatchBestSelect==18) u32_LatchBestSelect = 0;
			// swap start & end
			u32_Temp = st_skewer1.u32LatchOKStart; st_skewer1.u32LatchOKStart = st_skewer1.u32LatchOKEnd; st_skewer1.u32LatchOKEnd = u32_Temp;
		}

		if( st_skewer1.u32LatchOKStart == st_skewer1.u32LatchOKEnd ) {
			printf("Scan warn: %05Xh (", u32_Candidate);
			printf("%d ~ ", st_skewer1.u32LatchOKStart);
			printf("%d)\n", st_skewer1.u32LatchOKEnd);
			u32Ret = 2;
			goto ErrorHandle;
		}
		else {
			printf("Scan success: %05Xh (", u32_Candidate);
			printf("%d ~ ", st_skewer1.u32LatchOKStart);
			printf("%d)", st_skewer1.u32LatchOKEnd);
		}
	}
	else {
		printf("Scan fail: not continuous: %05Xh, u8_edge: ", u32_Candidate);
		printf("%d (", st_skewer1.u8_edge);
		printf("%d ~ ", st_skewer1.u32LatchOKStart);
		printf("%d)\n", st_skewer1.u32LatchOKEnd);
		u32Ret = 3;
		goto ErrorHandle;
	}

	printf(" --> %d\n", u32_LatchBestSelect);

	pSkewer->u32LatchOKStart = st_skewer1.u32LatchOKStart;
	pSkewer->u32LatchOKEnd = st_skewer1.u32LatchOKEnd;
	pSkewer->u32LatchBest = u32_LatchBestSelect;

ErrorHandle:

	return u32Ret;
}

void HalSdio_SetTriggerLevel(U8 u8STrigLevel)
{
	REG_FCIE_CLRBIT(R_SDIO_PLL_0x20, BIT_TRIG_LEVEL_MASK);
	switch(u8STrigLevel)
	{
		case 0:
			break;
		case 1:
			REG_FCIE_SETBIT(R_SDIO_PLL_0x20, BIT_TRIG_LEVEL_1);
			break;
		case 2:
			REG_FCIE_SETBIT(R_SDIO_PLL_0x20, BIT_TRIG_LEVEL_2);
			break;
		case 3:
			REG_FCIE_SETBIT(R_SDIO_PLL_0x20, BIT_TRIG_LEVEL_3);
			break;
		case 4:
			REG_FCIE_SETBIT(R_SDIO_PLL_0x20, BIT_TRIG_LEVEL_4);
			break;
		case 5:
			REG_FCIE_SETBIT(R_SDIO_PLL_0x20, BIT_TRIG_LEVEL_5);
			break;
		case 6:
			REG_FCIE_SETBIT(R_SDIO_PLL_0x20, BIT_TRIG_LEVEL_6);
			break;
		case 7:
			REG_FCIE_SETBIT(R_SDIO_PLL_0x20, BIT_TRIG_LEVEL_7);
			break;
		default:
			printf("SdErr: wrong parameter for set trigger level\n");
			break;
	}
}

void HalSdio_SetVDD(U8 u8OnOff)
{
	REG_FCIE_CLRBIT(CHIPGPIO_01, BIT3); // oen = 0, output

	if(u8OnOff)
	{
		REG_FCIE_CLRBIT(CHIPGPIO_00, BIT3); // set GPIO low to get 3.3V VDD
	}
	else
	{
		REG_FCIE_SETBIT(CHIPGPIO_00, BIT3); // set GPIO high to get 3.3V VDD
	}
}


#define VOLT_00V 0
#define VOLT_33V 1
#define VOLT_18V 2

// must set 3.3V fisrt, then switch to 1.8V

void HalSdio_SetIOVoltage(U8 u8Volt)
{
	U32 u32_i = 0;
	static U8 only_need_run_once = 0;
	static U8 ldo_on_off_status = 0;

	if(!only_need_run_once)
	{
		// 1. disable OSP (short protection curcuit related)
		REG_FCIE_SETBIT(R_SDIO_PLL_0x1D, BIT15);

		// 2. OSP count = 0x30
		REG_FCIE_CLRBIT(R_SDIO_PLL_0x36, 0xFF00);
		REG_FCIE_SETBIT(R_SDIO_PLL_0x36, 0x3000);

		only_need_run_once = 1;
	}

	if(u8Volt==VOLT_00V)
	{
		ldo_on_off_status = 0;
		REG_FCIE_SETBIT(R_SDIO_PLL_0x36, BIT05);		// Turning off LDO
	}
	else
	{
		if(!ldo_on_off_status) // LDO is off
		{
			// 3. tunning on LDO
			REG_FCIE_SETBIT(R_SDIO_PLL_0x36, BIT05);		// Turning off LDO
			udelay(10000);							// delay 10ms
			REG_FCIE_CLRBIT(R_SDIO_PLL_0x36, BIT05);		// Turning on LDO
			ldo_on_off_status = 1;
		}

		// 4. check if 3.3V ready
		if(u8Volt==VOLT_33V)
		{
			REG_FCIE_CLRBIT(R_SDIO_PLL_0x36, BIT02|BIT04);		// set to 3.3V

			//printf("wait 3.3V ready\n");

			while(1)
			{
				if (FCIE_RIU_R16(R_SDIO_PLL_0x10) & BIT12) break;
				//printf(".");

				udelay(1000); // delay 1ms
				if(u32_i++>1000)
				{
					printf("timeout to set 3.3V\n");
					while(1);
				}
			}
		}
		// 5. check if 1.8V ready
		else if(u8Volt==VOLT_18V)
		{
			REG_FCIE_SETBIT(R_SDIO_PLL_0x36, BIT02|BIT04);		// set to 1.8V

			//printf("wait 1.8V ready");

			while(1)
			{
				if (FCIE_RIU_R16(R_SDIO_PLL_0x10) & BIT13) break;
				//printf(".");

				udelay(1000); // delay 1ms
				if(u32_i++>1000)
				{
					printf("timeout to set 1.8V\n");
					while(1);
				}
			}

			//printf("1.8V is ready, R_SDIO_PLL_0x10 = %04Xh\n", FCIE_RIU_R16(R_SDIO_PLL_0x10));
		}
	}


}

//
// skew1: 0~17 (18 steps)
// skew2: 0~8  ( 9 steps)
// skew3: 0~8  ( 9 steps)
// skew4: 0~17 (18 steps)

void HalSdio_SetSkew(U8 u8SkewIdx, U32 u32SkewValue)
{
	//if( u8SkewIdx==1 || u8SkewIdx==2 )
		//printf(LIGHT_RED"HalSdio_SetSkew%d --> %d\n"NONE, u8SkewIdx, u32SkewValue);

	switch(u8SkewIdx)
	{
		case 1:
			if(u32SkewValue>17) goto ErrorHandle;

			REG_FCIE_CLRBIT(R_SDIO_PLL_0x03, BIT_SKEW1_MASK);
			if(u32SkewValue<9)
			{
				REG_FCIE_CLRBIT(R_SDIO_PLL_0x69, BIT3);
				REG_FCIE_SETBIT(R_SDIO_PLL_0x03, u32SkewValue);
			}
			else
			{
				REG_FCIE_SETBIT(R_SDIO_PLL_0x69, BIT3);
				REG_FCIE_SETBIT(R_SDIO_PLL_0x03, u32SkewValue-9);
			}
			break;

		case 2:
			if(u32SkewValue>8) goto ErrorHandle;

			REG_FCIE_CLRBIT(R_SDIO_PLL_0x03, BIT_SKEW2_MASK);
			REG_FCIE_SETBIT(R_SDIO_PLL_0x03, u32SkewValue<<4);
			break;

		case 3:
			if(u32SkewValue>8) goto ErrorHandle;

			REG_FCIE_CLRBIT(R_SDIO_PLL_0x03, BIT_SKEW3_MASK);
			REG_FCIE_SETBIT(R_SDIO_PLL_0x03, u32SkewValue<<8);
			break;

		case 4:
			if(u32SkewValue>17) goto ErrorHandle;

			REG_FCIE_CLRBIT(R_SDIO_PLL_0x03, BIT_SKEW4_MASK);
			if(u32SkewValue<9)
			{
				REG_FCIE_CLRBIT(R_SDIO_PLL_0x6C, BIT7); // inverse of CLK_SKEW_LAT
				REG_FCIE_SETBIT(R_SDIO_PLL_0x03, u32SkewValue<<12);
			}
			else
			{
				REG_FCIE_SETBIT(R_SDIO_PLL_0x6C, BIT7);
				REG_FCIE_SETBIT(R_SDIO_PLL_0x03, (u32SkewValue-9)<<12);
			}
			break;

		default:
			goto ErrorHandle;
			break;
	}

	return;

ErrorHandle:

	printf("SDErr: wrong skew index (%d) or wrong skew value (%d)\n", u8SkewIdx, u32SkewValue);

}

void HalSdio_SetDqs(U32 u32DqsValue)
{
	if(u32DqsValue>7) goto ErrorHandle;

	REG_FCIE_W(R_SDIO_PLL_0x6C, u32DqsValue);

	return;

ErrorHandle:

	printf("SdErr: wrong dqs value %d\n", u32DqsValue);
}

void HalSdio_SetDrivingStrength(U16 u16DrivingLevel)
{
	unsigned short u16RegValue[8] = {0x00, 0x03, 0x0C, 0x0F, 0x30, 0x33, 0x3C, 0x3F};

	FCIE_RIU_16_OF(CHIPTOP_42, 0x3F); // bit 0~6 driving mask
	FCIE_RIU_16_ON(CHIPTOP_42, u16RegValue[u16DrivingLevel]);
	printf("Set driving level to %04Xh\n", FCIE_RIU_R16(CHIPTOP_42));
}

void HalSdio_MaskMiuReq(U8 u8Mask)
{
	printf("Please porting me...\n");
}

void HalSdio_TimerEnable(void)
{
    // reset HW timer
    FCIE_RIU_W16(TIMER1_MAX_LOW, 0xFFFF);
    FCIE_RIU_W16(TIMER1_MAX_HIGH, 0xFFFF);
    FCIE_RIU_W16(TIMER1_ENABLE, 0);

    // start HW timer
    FCIE_RIU_16_ON(TIMER1_ENABLE, 0x0001);

	// 0xFFFFFFFF = 4,294,967,295 tick
	// divide 12 --> 357,913,941 us --> 357 sec --> 6 min
}


U32 HalSdio_TimerGetTick(void)
{
    U32 u32HWTimer = 0;
    U16 u16TimerLow = 0;
    U16 u16TimerHigh = 0;

    // Get HW timer
    u16TimerLow = FCIE_RIU_R16(TIMER1_CAP_LOW);
    u16TimerHigh = FCIE_RIU_R16(TIMER1_CAP_HIGH);

    u32HWTimer = (u16TimerHigh<<16) | u16TimerLow;

    return u32HWTimer;
}

// max: 357,913,941 = 0x15555555
U32 HalSdio_TimerGetMicroSec(void)
{
    return (HalSdio_TimerGetTick()/12);
}

void HalSdio_TimerTest(void)
{
	unsigned int sec = 0;

	HalSdio_TimerEnable();
	printf("count to 3 then start test: ");
	while(1)
	{
		if (HalSdio_TimerGetMicroSec() >= (1000000+sec*1000000))
		{
			printf("%d ", ++sec);
		}
		if(sec==3)
		{
			printf("Go!\n");
			break;
		}
	}
}

void HalSdio_DelaySecond(unsigned int how_many_sec)
{
	unsigned int sec = 0;

	if(how_many_sec>4293)
	{
		printf("Sorry, this function do not support long delay\n");
		return;
	}

	HalSdio_TimerEnable();
	while(1)
	{
		if (HalSdio_TimerGetMicroSec() >= (1000000+sec*1000000))
		{
			++sec;
		}
		if(sec==how_many_sec)
		{
			break;
		}
	}
}


static unsigned int tick_start;
static unsigned int tick_stop;

void HalSdio_TimerStart(void)
{
	HalSdio_TimerEnable();
	tick_start = HalSdio_TimerGetTick();
}

U32 HalSdio_TimerStop(void)
{
	tick_stop = HalSdio_TimerGetTick();
	return ((tick_stop - tick_start) / 12);
}


