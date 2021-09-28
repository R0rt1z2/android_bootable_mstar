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

// this file is use only for munich project

#include <config.h>
#include "drvFCIE_config.h"



#include <common.h>
//#include <stdio.h>
#include <mmc.h>
#include "drvFCIE5.h"

extern unsigned char sd_ddr_mode;
extern unsigned char gu8TuningMode;

/**************************************
* Config definition
**************************************/
#define DBG_CR_CLK(MSG)             //MSG
#define DBG_CR_PAD(MSG)             //MSG

/**************************************
* Local function definition
**************************************/
U32 HalSdio_SetSdioClock(U32 u32Clock);


/**************************************
* Global function definition
**************************************/

void HalSdio_ResetIP(void)
{
	U16 u16Reg, u16Cnt;

	HalSdio_SetClock(48000);

	//printf("fcie reset ");
	#if ENABLE_MUNICH // munich has FCIE only
		FCIE_RIU_W16(FCIE_MIE_FUNC_CTL, BIT_SD_EN); // enable SDIO
	#else
		FCIE_RIU_W16(FCIE_MIE_FUNC_CTL, BIT_SDIO_MOD); // enable SDIO
	#endif

	REG_FCIE_CLRBIT(FCIE_RST, BIT_FCIE_SOFT_RST_n); /* active low */


	// FCIE reset - wait
	u16Cnt=0;

	do
	{
		udelay(HW_TIMER_DELAY_1us);

		REG_FCIE_R(FCIE_RST, u16Reg);

	  	if(0x1000 == u16Cnt++)
		{
			printf("FCIE Err: FCIE Reset fail!! FCIE_RST = %04Xh\n", u16Reg);
			while(1);
		}

	} while (BIT_RST_STS_MASK  != (u16Reg  & BIT_RST_STS_MASK));

	//printf("reset finish\n");

	//[FIXME] is there any method to check that reseting FCIE is done?

	// FCIE reset - clear
	REG_FCIE_SETBIT(FCIE_RST, BIT_FCIE_SOFT_RST_n);

	// FCIE reset - check

	u16Cnt=0;
	do
	{
		REG_FCIE_R(FCIE_RST, u16Reg);
		//printf("FCIE_RST = %04Xh\n", u16Reg);

	  	if(0x1000 == u16Cnt++)
		{
			printf("eMMC Err: FCIE Reset fail2:h \n");
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
	FCIE_RIU_16_OF(CHIPTOP_50, WBIT15); // all_pad_in = 0

#if 0
	FCIE_RIU_16_OF(CHIPTOP_0B, WBIT01|WBIT02|WBIT03|WBIT04);
	FCIE_RIU_16_ON(CHIPTOP_0B, WBIT03); // reg_pcm_pe[7:4] = 4

	FCIE_RIU_16_OF(CHIPTOP_0C, WBIT00|WBIT01|WBIT02|WBIT03);
	FCIE_RIU_16_ON(CHIPTOP_0C, WBIT02); // reg_pcm_pe[19:16] = 4
#else
	//FCIE_RIU_16_ON(CHIPTOP_0B, WBIT01|WBIT02|WBIT03|WBIT04);
	FCIE_RIU_16_ON(CHIPTOP_0C, WBIT00|WBIT01|WBIT02|WBIT03);
#endif

	FCIE_RIU_16_OF(CHIPTOP_50, WBIT07|WBIT08|WBIT09); // reg_nand_mode = 0

	FCIE_RIU_W16(CHIPTOP_4F, 0x0000); // new add

#if 0
	// reg_sd_config = 2 --> MST220B
	FCIE_RIU_16_OF(CHIPTOP_57, WBIT08|WBIT10|WBIT11);
	FCIE_RIU_16_ON(CHIPTOP_57, WBIT09);
#else
	// MST085B
	// reg_sd_config = 2
	FCIE_RIU_16_OF(CHIPTOP_57, BIT08|BIT09);
	FCIE_RIU_16_ON(CHIPTOP_57, BIT09);
	// reg_sd_config2 = 0
	FCIE_RIU_16_OF(CHIPTOP_57, BIT10|BIT11);
	// reg_sd_config_eco = 0
	FCIE_RIU_16_OF(CHIPTOP_1D, BIT09);

	FCIE_RIU_16_ON(CHIPTOP_0D,BIT02);

	FCIE_RIU_16_OF(CHIPTOP_6E, BIT06|BIT07);
	FCIE_RIU_16_OF(CHIPTOP_4F,BIT13|BIT14|BIT15);

	FCIE_RIU_16_OF(CHIPTOP_64 ,BIT00);
	//----------------EMMCPLL----------------
	FCIE_RIU_16_OF(EMMCPLL_68 ,BIT00|BIT01);
	FCIE_RIU_16_OF(EMMCPLL_6D,BIT00);
	FCIE_RIU_16_OF(EMMCPLL_70, BIT08);
	
	FCIE_RIU_W16(EMMCPLL_71,0xffff);
	FCIE_RIU_W16(EMMCPLL_73,0xffff);
	
	FCIE_RIU_16_ON(EMMCPLL_74, BIT15);

	FCIE_RIU_16_ON(MIU2_79,BIT04);

	#endif

//	FCIE_RIU_W16(WDT_MAX_L, 0x0000); FCIE_RIU_W16(WDT_MAX_H, 0x0000); // prevent watch dog timeout in mboot

}


#if !defined FPGA_BOARD || FPGA_BOARD==0

// find clock close to target but not over

const U8 FcieClockIdxNum = 7;

const U32 FCIE_CLOCK[] =
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
    FCIE_RIU_16_OF(REG_CLK_SDIO, WBIT07+WBIT05+WBIT04+WBIT03+WBIT02); // mask all clock select

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

		case 6: // 12M
		    FCIE_RIU_16_ON(REG_CLK_SDIO, 0xE<<2);
			break;

		case 7: // 300K
		    FCIE_RIU_16_ON(REG_CLK_SDIO, 0xD<<2);
			break;

		default:
			printf("SDIO Err: wrong clock selector!\n");
			while(1);
			break;

	}

	DBG_CR_CLK(printf("REG_CLK_SDIO = 0x%04X\r\n", FCIE_RIU_R16(REG_CLK_SDIO)));

    FCIE_RIU_16_OF(REG_CLK_SDIO, WBIT00); // turn off clock gating

    return FCIE_CLOCK[u8ClockSlector];

}

#else

U8 HalSdio_SetSdioClock(U32 u32Clock)
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
	// chiptop

	// sdio
	REG_FCIE_CLRBIT(FCIE_DDR_MODE, BIT_FALL_LATCH|BIT_PAD_IN_SEL_SD|BIT_32BIT_MACRO_EN|BIT_DDR_EN|BIT_8BIT_MACRO_EN);

	REG_FCIE_CLRBIT(FCIE_NC_FUNC_CTL, BIT_NC_EN);


	REG_FCIE_SETBIT(FCIE_DDR_MODE, BIT10);


	//REG_FCIE_W(FCIE_DDR_TOGGLE_CNT, 0x0000);

	switch(u32Mode)
	{
		case SDIO_MODE_GPIO_PAD_BPS:
			DBG_CR_PAD(printf("SDIO_MODE_GPIO_PAD_BPS\n"));
			u8CurrentPadType = SDIO_MODE_GPIO_PAD_BPS;
			break;

		case SDIO_MODE_GPIO_PAD_SDR:
			DBG_CR_PAD(printf("SDIO_MODE_GPIO_PAD_SDR\n"));
			u8CurrentPadType = SDIO_MODE_GPIO_PAD_SDR;
			// fcie
			REG_FCIE_SETBIT(FCIE_DDR_MODE, BIT_FALL_LATCH|BIT_PAD_IN_SEL_SD);
			break;

		default:
			u8CurrentPadType = SDIO_MODE_UNKNOWN;
			printf("SDIO Err: wrong parameter for switch pad func\n");
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
			printf("\33[1;31mSDIO detect error, SD_STATUS = %04Xh\33[m\n", FCIE_RIU_R16(FCIE_SD_STATUS));
			HalSdio_Platform_ClearEvent(BIT_ERR_STS);
			return IO_ERROR;
		}
		else if((u16Event&u16ReqVal)==u16ReqVal)
		{
			//printf(" Got event %04X ", u16Event);
			break;
		}

		udelay(HW_TIMER_DELAY_1ms);

		u32Count++;

		if(u32Count>u32WaitMs)
		{
			printf("u32Counter=%d\r\n", u32Count);
			printf("\r\n");
			printf("------------------------------------------\r\n");
			printf("ERROR!!! MIE EVENT TIME OUT!!!\n");
			printf("request event = %04Xh, event = %04Xh\n", u16ReqVal, u16Event);
			printf("------------------------------------------\r\n");

//			if(!gu8SilenceTuning)
			//{
				//HalSdio_DumpRegister();
				//HalSdio_DumpDebugBus();
				//while(1);
			//}

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
	#if 1
		printf("please porting me...\n");
	#else
	FCIE_RIU_16_OF(CHIP_GPIO_47, GPIO158_OEN); // active low

	if(u8OnOff)
	{
		FCIE_RIU_16_OF(CHIP_GPIO_47, GPIO158_OUT); // power on
		printf(LIGHT_RED"turn on card power\n"NONE);
	}
	else
	{
		FCIE_RIU_16_ON(CHIP_GPIO_47, GPIO158_OEN); // power off
		printf(LIGHT_RED"turn off card power\n"NONE);
	}
	#endif
}

void HalSdio_SetDrivingStrength(U16 u16DrivingLevel)
{
	printf("Please porting me...\n");
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




