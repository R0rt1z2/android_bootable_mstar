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

// this file is use only for k6 project 2016.04.19

#include <config.h>
#include "drvFCIE_config.h"


#include <common.h>
//#include <stdio.h>
#include <mmc.h>
#include "drvFCIE5.h"

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

	REG_FCIE_W(FCIE_SD_CTRL, 0); // clear job start for safety

	//printf(LIGHT_CYAN"sdio reset\n"NONE);

	#if (ALTERNATIVE_IP == FCIE_IP)
		FCIE_RIU_W16(FCIE_MIE_FUNC_CTL, BIT_SD_EN);
	#elif (ALTERNATIVE_IP == SDIO_IP)
		FCIE_RIU_W16(FCIE_MIE_FUNC_CTL, BIT_SDIO_MOD);
	#endif

	REG_FCIE_CLRBIT(FCIE_RST, BIT_FCIE_SOFT_RST_n); /* active low */

	// SDIO reset - wait
	u16Cnt=0;

	do
	{
		REG_FCIE_R(FCIE_RST, u16Reg);

		HalSdioDelayUs(1);

	  	if(0x1000 == u16Cnt++)
		{
			printf("SD Err: SDIO Reset fail!! FCIE_RST = %04Xh\n", u16Reg);
			while(1);
		}

	} while (BIT_RST_STS_MASK  != (u16Reg  & BIT_RST_STS_MASK));

	REG_FCIE_SETBIT(FCIE_RST, BIT_FCIE_SOFT_RST_n);

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

		HalSdioDelayUs(1);


	} while (0  != (u16Reg  & BIT_RST_STS_MASK));

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

#define CONFIG_SD_MODE		1
#define CONFIG_SDIO_MODE 	1

void HalSdio_Platform_InitChiptop(void)
{
	#if (ALTERNATIVE_IP == FCIE_IP)

		FCIE_RIU_16_OF(CHIPTOP_50h, REG_ALL_PAD_IN);
		FCIE_RIU_16_OF(CHIPTOP_08h, REG_NAND_MODE); // clear nand_mode
		FCIE_RIU_16_OF(CHIPTOP_08h, REG_EMMC_CONFIG); // clear emmc_config

		#if (CONFIG_SD_MODE==1)
			FCIE_RIU_16_ON(CHIPTOP_08h, REG_SD_CONFIG);
		#endif

		// MIU select by FCIE
		if(!(FCIE_RIU_R16(MIU2_7A)&MIU_SELECT_BY_FCIE))
		{
			FCIE_RIU_16_ON(MIU2_7A, MIU_SELECT_BY_FCIE);
			printf("SdErr: MIU select setting wrong for FCIE\n");
		}

	#elif (ALTERNATIVE_IP == SDIO_IP)

		FCIE_RIU_16_OF(CHIPTOP_50h, REG_ALL_PAD_IN);

		FCIE_RIU_16_OF(CHIPTOP_08h, REG_SDIO_CONFIG_MSK);

		#if (CONFIG_SD_MODE==1)
			FCIE_RIU_16_ON(CHIPTOP_08h, REG_SDIO_MODE_1);
		#endif

		// MIU select by SDIO
		FCIE_RIU_16_ON(MIU2_7A, MIU_SELECT_BY_SDIO);

	#endif

}



#if !defined FPGA_BOARD || FPGA_BOARD==0

#if (ALTERNATIVE_IP == FCIE_IP)

const U8 FcieClockIdxNum = 8;

const U32 FCIE_CLOCK[] =
{
	48000, //  0
	40000, //  1
	36000, //  2
	27000, //  3
	21500, //  4
	18000, //  5
	15500, //  6
	12000, //  7
	   75, //  8
};

// find clock close to target but not over

U32 HalSdio_SetClock(U32 u32Clock)
{
	U8 u8ClockSlector;
	/*static U32 u32_OldClock=0xFFFFFFFF;

	//printf("HalFcie_SetClock(%ld)\n", u32Clock);

	if(u32_OldClock == u32Clock)
		return 0;
	else
		u32_OldClock = u32Clock;*/

#ifdef CONFIG_MSTAR_CLKM
	S32 fcie_clk, fcie_gate;

	fcie_clk = get_handle("g_clk_nfie_p");
	fcie_gate = get_handle("g_clk_nfie");

	set_clk_source(fcie_gate, "g_nfie_clk_xtal"); // clock gated
#else
    FCIE_RIU_16_ON(REG_CLK_SDIO, BIT_SDIO_CLK_GATING); // turn on clock gating
#endif

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

	#ifndef CONFIG_MSTAR_CLKM
		FCIE_RIU_16_ON(REG_CLK_SDIO, BIT_SDIO_CLK_SRC_SEL);
		FCIE_RIU_16_OF(REG_CLK_SDIO, BIT_CLKGEN_SDIO_MASK); // mask all clock select
	#endif

	//printf("switch to clock: %d\n", u8ClockSlector);

	switch(u8ClockSlector)
	{
		case 0: // 48M
			#ifdef CONFIG_MSTAR_CLKM
				set_clk_source(fcie_clk, "g_nfie_clk_48");
			#else
				FCIE_RIU_16_ON(REG_CLK_SDIO, 0x8<<BIT_CLKGEN_SDIO_SHIFT);
			#endif
			break;

		case 1: // 40M
			#ifdef CONFIG_MSTAR_CLKM
				set_clk_source(fcie_clk, "g_nfie_clk_40");
			#else
				FCIE_RIU_16_ON(REG_CLK_SDIO, 0x7<<BIT_CLKGEN_SDIO_SHIFT);
			#endif
			break;

		case 2: // 36M
			#ifdef CONFIG_MSTAR_CLKM
				set_clk_source(fcie_clk, "g_nfie_clk_36");
			#else
				FCIE_RIU_16_ON(REG_CLK_SDIO, 0x6<<BIT_CLKGEN_SDIO_SHIFT);
			#endif
			break;

		case 3: // 27M
			#ifdef CONFIG_MSTAR_CLKM
				set_clk_source(fcie_clk, "g_nfie_clk_27");
			#else
				FCIE_RIU_16_ON(REG_CLK_SDIO, 0x5<<BIT_CLKGEN_SDIO_SHIFT);
			#endif
			break;

		case 4: // 21.5M
			#ifdef CONFIG_MSTAR_CLKM
				set_clk_source(fcie_clk, "g_nfie_clk_21");
			#else
				FCIE_RIU_16_ON(REG_CLK_SDIO, 0x4<<BIT_CLKGEN_SDIO_SHIFT);
			#endif
			break;

		case 5: // 18M
			#ifdef CONFIG_MSTAR_CLKM
				set_clk_source(fcie_clk, "g_nfie_clk_18");
			#else
				FCIE_RIU_16_ON(REG_CLK_SDIO, 0x3<<BIT_CLKGEN_SDIO_SHIFT);
			#endif
			break;

		case 6: // 15.5M
			#ifdef CONFIG_MSTAR_CLKM
				set_clk_source(fcie_clk, "g_nfie_clk_15");
			#else
				FCIE_RIU_16_ON(REG_CLK_SDIO, 0x2<<BIT_CLKGEN_SDIO_SHIFT);
			#endif
			break;

		case 7: // 12M
			#ifdef CONFIG_MSTAR_CLKM
				set_clk_source(fcie_clk, "g_nfie_clk_12");
			#else
				FCIE_RIU_16_ON(REG_CLK_SDIO, 0x1<<BIT_CLKGEN_SDIO_SHIFT);
			#endif
			break;

		case 8: // 75K
			#ifdef CONFIG_MSTAR_CLKM
				set_clk_source(fcie_clk, "g_nfie_clk_75K");
			#else
				FCIE_RIU_16_ON(REG_CLK_SDIO, 0xC<<BIT_CLKGEN_SDIO_SHIFT);
			#endif
			break;

		default:
			printf("FCIE Err: wrong clock selector!\n");
			while(1);
			break;

	}

	DBG_CR_CLK(printf("REG_CLK_SDIO = 0x%04X\r\n", FCIE_RIU_R16(REG_CLK_SDIO)));

	#ifdef CONFIG_MSTAR_CLKM
		set_clk_source(fcie_gate, "g_nfie_clk_p"); // clock enable
	#else
		FCIE_RIU_16_OF(REG_CLK_SDIO, BIT_SDIO_CLK_GATING); // turn off clock gating
	#endif

	return FCIE_CLOCK[u8ClockSlector];

}

#elif (ALTERNATIVE_IP == SDIO_IP)

const U8 SdioClockIdxNum = 7;

const U32 SDIO_CLOCK[] =
{
    48000, //  0
    43200, //  1
    40000, //  2
    36000, //  3
    32000, //  4
    20000, //  5
    12000, //  6
      300, //  7
};

// find clock close to target but not over

U32 HalSdio_SetClock(U32 u32Clock)
{
    U8 u8ClockSlector;
	/*static U32 u32_OldClock=0xFFFFFFFF;

	//printf("HalSdio_SetClock(%ld)\n", u32Clock);

	if(u32_OldClock == u32Clock)
		return 0;
	else
		u32_OldClock = u32Clock;*/

#ifdef CONFIG_MSTAR_CLKM
	S32 sdio_clk, sdio_gate;

	sdio_clk = get_handle("g_clk_sdio_p");
	sdio_gate = get_handle("g_clk_sdio");
	set_clk_source(sdio_gate, "g_sdio_clk_xtal"); // clock gated
#else
    FCIE_RIU_16_ON(REG_CLK_SDIO, BIT00); // turn on clock gating
#endif

	if(u32Clock>1000)
	{
		DBG_CR_CLK(printf("Set SDIO clock as %d.%d MHz, ", u32Clock/1000, (u32Clock%1000)/100 ) );
	}
	else
	{
		DBG_CR_CLK(printf("Set SDIO clock as %d KHz, ", u32Clock));
	}

	for(u8ClockSlector=0; u8ClockSlector<=SdioClockIdxNum; u8ClockSlector++)
	{
		if( SDIO_CLOCK[u8ClockSlector] <= u32Clock )
		{
			break;
		}
	}

	if(u8ClockSlector>SdioClockIdxNum)
	{
		DBG_CR_CLK(printf("Error!!! Can not find proper clock!\r\n"));
		while(1);
		return 0x01;
	}

	if(u32Clock>1000)
	{
		DBG_CR_CLK(printf("select SDIO clock as %d.%d MHz\r\n", SDIO_CLOCK[u8ClockSlector]/1000, (SDIO_CLOCK[u8ClockSlector]%1000)/100));
	}
	else
	{
	DBG_CR_CLK(printf("select SDIO clock as %d KHz\r\n", SDIO_CLOCK[u8ClockSlector]));
	}

	#ifndef CONFIG_MSTAR_CLKM
		FCIE_RIU_16_ON(REG_CLK_SDIO, BIT06);
		FCIE_RIU_16_OF(REG_CLK_SDIO, BIT05+BIT04+BIT03+BIT02); // mask all clock select
	#endif

	//printf("switch to clock: %d\n", u8ClockSlector);

	switch(u8ClockSlector)
	{
		case 0: // 48M
			#ifdef CONFIG_MSTAR_CLKM
				set_clk_source(sdio_clk, "g_sdio_clk_48");
			#else
				FCIE_RIU_16_ON(REG_CLK_SDIO, 0xF<<BIT_CLKGEN_SDIO_SHIFT);
			#endif
			break;

		case 1: // 43.2M
			#ifdef CONFIG_MSTAR_CLKM
				set_clk_source(sdio_clk, "g_sdio_clk_43");
			#else
				FCIE_RIU_16_ON(REG_CLK_SDIO, 0x5<<BIT_CLKGEN_SDIO_SHIFT);
			#endif
			break;

		case 2: // 40M
			#ifdef CONFIG_MSTAR_CLKM
				set_clk_source(sdio_clk, "g_sdio_clk_40");
			#else
				FCIE_RIU_16_ON(REG_CLK_SDIO, 0x4<<BIT_CLKGEN_SDIO_SHIFT);
			#endif
			break;

		case 3: // 36M
			#ifdef CONFIG_MSTAR_CLKM
				set_clk_source(sdio_clk, "g_sdio_clk_36");
			#else
				FCIE_RIU_16_ON(REG_CLK_SDIO, 0x3<<BIT_CLKGEN_SDIO_SHIFT);
			#endif
			break;

		case 4: // 32M
			#ifdef CONFIG_MSTAR_CLKM
				set_clk_source(sdio_clk, "g_sdio_clk_32");
			#else
				FCIE_RIU_16_ON(REG_CLK_SDIO, 0x2<<BIT_CLKGEN_SDIO_SHIFT);
			#endif
			break;

		case 5: // 20M
			#ifdef CONFIG_MSTAR_CLKM
				set_clk_source(sdio_clk, "g_sdio_clk_20");
			#else
				FCIE_RIU_16_ON(REG_CLK_SDIO, 0x1<<BIT_CLKGEN_SDIO_SHIFT);
			#endif
			break;

		case 6: // 12M
			#ifdef CONFIG_MSTAR_CLKM
				set_clk_source(sdio_clk, "g_sdio_clk_xtal");
			#else
				FCIE_RIU_16_ON(REG_CLK_SDIO, 0xE<<BIT_CLKGEN_SDIO_SHIFT);
			#endif
			break;

		case 7: // 300K
			#ifdef CONFIG_MSTAR_CLKM
				set_clk_source(sdio_clk, "g_sdio_clk_300k");
			#else
				FCIE_RIU_16_ON(REG_CLK_SDIO, 0xD<<BIT_CLKGEN_SDIO_SHIFT);
			#endif
			break;

		default:
			printf("SDIO Err: wrong clock selector!\n");
			while(1);
			break;

	}

	DBG_CR_CLK(printf("REG_CLK_SDIO = 0x%04X\r\n", FCIE_RIU_R16(REG_CLK_SDIO)));

	#ifdef CONFIG_MSTAR_CLKM
		set_clk_source(sdio_gate, "g_sdio_clk_p"); // clock enable
	#else
		FCIE_RIU_16_OF(REG_CLK_SDIO, BIT00); // turn off clock gating
	#endif

	return SDIO_CLOCK[u8ClockSlector];

}

#endif // ALTERNATIVE_IP

#else // FPGA_BOARD

U8 HalSdio_SetSdioClock(U32 u32Clock)
{
	if (u32Clock < 400)
	{
		printf("SDIO FPGA clock 187.5KHz\n");
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
	HalSdio_Platform_InitChiptop();

	// sdio
	REG_FCIE_CLRBIT(FCIE_DDR_MODE, BIT_FALL_LATCH|BIT_PAD_IN_SEL_SD|BIT_32BIT_MACRO_EN|BIT_DDR_EN|BIT_8BIT_MACRO_EN);

	switch(u32Mode)
	{
		///////////////////////////////////////////////////////////////////////////////////////////
		case SDIO_MODE_GPIO_PAD_BPS:
			DBG_CR_PAD(printf(LIGHT_CYAN"SDIO_MODE_GPIO_PAD_BPS\n"NONE));
			u8CurrentPadType = SDIO_MODE_GPIO_PAD_BPS;
			break;

		///////////////////////////////////////////////////////////////////////////////////////////
		case SDIO_MODE_GPIO_PAD_SDR:
			DBG_CR_PAD(printf(LIGHT_CYAN"SDIO_MODE_GPIO_PAD_SDR\n"NONE));
			u8CurrentPadType = SDIO_MODE_GPIO_PAD_SDR;
			FCIE_RIU_16_ON(FCIE_DDR_MODE, BIT_FALL_LATCH|BIT_PAD_IN_SEL_SD);
			// New patch for feedback clock reflex
			FCIE_RIU_16_ON(FCIE_DDR_MODE, BIT_FEEDBACK_CLK2);
			break;

		default:
			u8CurrentPadType = SDIO_MODE_UNKNOWN;
			DBG_CR_PAD(printf(LIGHT_CYAN"SdErr: wrong parameter for switch pad func\n"NONE));
			while(1);
			//return 0x01;
			break;
	}

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
			printf("Error!!! Can not clear MIE event %04Xh (%04Xh)\n", FCIE_RIU_R16(FCIE_MIE_EVENT), nReq);
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
			/*if(!(gu8TuningMode&TUNE_NOW))
			{
				printf("\33[1;31mSDIO ErrDet, SD_STS = %04Xh\33[m\n", FCIE_RIU_R16(FCIE_SD_STATUS));
			}*/
			HalSdio_Platform_ClearEvent(BIT_ERR_STS);
			return IO_ERROR_DETECT;
		}
		else if((u16Event&u16ReqVal)==u16ReqVal)
		{
			//printf("Got event %04X\n", u16Event);
			break;
		}

		HalSdioDelayUs(1000);

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

void HalSdio_SetVDD(U8 u8OnOff)
{
	#if (ALTERNATIVE_IP == FCIE_IP)
	// MST257B, Q43, need to rework add R882
	// GPIO index 65
	/*
	reg_allpad_in		reg[101ea1]#7      = 0b
	reg_test_out_mode	reg[101e24]#5 ~ #4 = 00b
	reg_spdif_in_mode	reg[101e0a]#1 ~ #0 = 00b
	reg_i2cm1_mode		reg[101e12]#3 ~ #2 = 00b
	*/

	FCIE_RIU_16_OF(REG_PADTOP0_61h, REG_GPIO9_OEN);
	if(u8OnOff)
	{
		FCIE_RIU_16_OF(REG_PADTOP0_61h, REG_GPIO9_OUT);
	}
	else
	{
		FCIE_RIU_16_ON(REG_PADTOP0_61h, REG_GPIO9_OUT);
	}
	#elif (ALTERNATIVE_IP == SDIO_IP)
	// CON25 always has power
	#endif
}

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

U32 HalSdio_SlectBestSkew4(U32 u32_Candidate, SKEWER * pSkewer)
{
	SKEWER st_skewer1/* = {0}*/;
	SKEWER st_skewer2/* = {0}*/;
	U32 u32_LatchBestSelect = 0;
	U32 u32_Temp;
	U32 u32Ret = 0;

	//printf("\n%s(%05Xh)\n", __FUNCTION__, u32_Candidate);

	if(!u32_Candidate) {
		printf("ATOP fail: no workable skew4\n");
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
			printf("ATOP warn: %05Xh (", u32_Candidate);
			printf("%d ~ ", st_skewer1.u32LatchOKStart);
			printf("%d)\n", st_skewer1.u32LatchOKEnd);
			u32Ret = 2;
			goto ErrorHandle;
		}
		else {
			printf("ATOP success: %05Xh (", u32_Candidate);
			printf("%d ~ ", st_skewer1.u32LatchOKStart);
			printf("%d)", st_skewer1.u32LatchOKEnd);
		}
	}
	else {
		printf("ATOP fail: not continuous: %05Xh, u8_edge: ", u32_Candidate);
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

void HalSdio_SetDrivingStrength(U16 u16DrivingLevel)
{
	printf("Please porting me...\n");
}


void HalSdio_MaskMiuReq(U8 u8Mask)
{
	#if (ALTERNATIVE_IP == FCIE_IP)
		if(u8Mask)
		{
			FCIE_RIU_16_ON(MIU_GROUP2_REQ_MSK, MIU_FCIE_MSK);
			printf("mask MIU request from FCIE IP to simulate MIU hang up\n");

		}
		else
		{
			FCIE_RIU_16_OF(MIU_GROUP2_REQ_MSK, MIU_FCIE_MSK);
		}
	#elif (ALTERNATIVE_IP == SDIO_IP)
		if(u8Mask)
		{
			FCIE_RIU_16_ON(MIU_GROUP2_REQ_MSK, MIU_SDIO_MSK);
			printf("mask MIU request from SDIO IP to simulate MIU hang up\n");

		}
		else
		{
			FCIE_RIU_16_OF(MIU_GROUP2_REQ_MSK, MIU_SDIO_MSK);
		}

	#endif
}

void HalSdio_PatchCheckR2nWriteState(void)
{
	U16 u16WRstate = 0;
	// add patch to wait write state become 0xC (w4_wbk1)

	// debug mode = 5 (Card)
	REG_FCIE_CLRBIT(FCIE_EMMC_DEBUG_BUS1, BIT_DEBUG_MODE_MSK);
	REG_FCIE_SETBIT(FCIE_EMMC_DEBUG_BUS1, BIT_DEBUG_MODE_SET);
	//printf("Debug mode = %d (5: Card)\n", (FCIE_RIU_R16(FCIE_EMMC_DEBUG_BUS1)&BIT_DEBUG_MODE_MSK)>>BIT_SHIFT_DEBUG_MODE);

	REG_FCIE_CLRBIT(FCIE_TEST_MODE, BIT_DEBUG_MODE_MASK);
	REG_FCIE_SETBIT(FCIE_TEST_MODE, 2<<BIT_DEBUG_MODE_SHIFT); // card debug mode = 2
	//printf("card debug mode = %d\n", REG_FCIE(FCIE_TEST_MODE)>>BIT_DEBUG_MODE_SHIFT);

	while(1)
	{
		u16WRstate = (FCIE_RIU_R16(FCIE_EMMC_DEBUG_BUS0)&BIT_STATE_MSK_WR)>>BIT_SHIFT_WR;
		//printf("WR = 0x%X\n", u16WRstate);
		if(u16WRstate==0xC) {
			break;
		}
	}
}

void HalSdio_McmSlowDownRatio(U16 u16Ratio)
{
	if( u16Ratio>16 || u16Ratio==1)
	{
		printf("SdErr, wrong ratio for mcm slow down! Use 2~16\n");
		return;
	}

	FCIE_RIU_16_OF(REG_SDIO_MCM, BIT_SDIO_RATIO_MSK);
	if(u16Ratio)
	{
		FCIE_RIU_16_ON(REG_SDIO_MCM, (u16Ratio-1)<<BIT_SDIO_RATIO_SHIFT);
	}
	printf("REG_SDIO_MCM = %04Xh\n", FCIE_RIU_R16(REG_SDIO_MCM));
}

#if 0
void FCIE_HWTimer_Start(void)
{
    // reset HW timer
    FCIE_RIU_W16(TIMER0_MAX_LOW, 0xFFFF);
    FCIE_RIU_W16(TIMER0_MAX_HIGH, 0xFFFF);
    FCIE_RIU_W16(TIMER0_ENABLE, 0);

    // start HW timer
    FCIE_RIU_16_ON(TIMER0_ENABLE, 0x0001);

}

U32 FCIE_HWTimer_End(void)
{
    U32 u32HWTimer = 0;
    U16 u16TimerLow = 0;
    U16 u16TimerHigh = 0;

    // Get HW timer
    u16TimerLow = FCIE_RIU_R16(TIMER0_CAP_LOW);
    u16TimerHigh = FCIE_RIU_R16(TIMER0_CAP_HIGH);

    u32HWTimer = (u16TimerHigh<<16) | u16TimerLow;

    return u32HWTimer;
}
#endif

extern void MsOS_DelayTaskUs_Poll(U32 u32Us); // MsOS.h
extern void MsOS_DelayTaskUs(U32 u32Us);


void HalSdioDelayUs(U32 u32Us)
{
	U32 u32_i, u32Ms;

	if(u32Us<1000)
	{
		udelay(u32Us);
	}
	else
	{
		u32Ms = u32Us/1000;
		for(u32_i=0; u32_i<u32Ms; u32_i++)
			udelay(1000);
	}
}

void HalSdioDelayMs(U32 u32Ms)
{
	U32 u32_i;

	for(u32_i=0; u32_i<u32Ms; u32_i++)
		udelay(1000);
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
