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

// this file is use only for monet project

#include <config.h>
#include "drvFCIE_config.h"


#include <common.h>
//#include <stdio.h>
#include <mmc.h>
#include "drvFCIE5.h"

extern unsigned char sd_ddr_mode;
extern void HalFcie_DumpDebugBus(void);
extern unsigned char gu8TuningMode;

/**************************************
* Config definition
**************************************/
#define DBG_CR_CLK(MSG)             MSG

/**************************************
* Local function definition
**************************************/


/**************************************
* Global function definition
**************************************/

void HalSdio_ResetIP(void)
{
	U16 u16Reg, u16Cnt;

	HalSdio_SetSdioClock(48000);

	//printf("fcie reset ");
	REG_FCIE_W(FCIE_MIE_FUNC_CTL, BIT_SDIO_MOD);

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

void halFCIE_Platform_InitChiptop(void)
{

	// bring up note, chiptio related register
	FCIE_RIU_16_OF(CHIPTOP_6F, BIT07|BIT06); // reg_nand_mode = 0
	FCIE_RIU_16_OF(CHIPTOP_5A, BIT09|BIT08); // clear reg_sd_config
	FCIE_RIU_16_OF(CHIPTOP_7B, BIT05|BIT04); // clear reg_sdio_config
	FCIE_RIU_16_ON(CHIPTOP_43, BIT15|BIT14|BIT13|BIT12); // reg_sd_pe = 0xF
	FCIE_RIU_16_OF(CHIPTOP_50, BIT15); // all_pad_in

	// Bring up note: GPIO_PAD SDR mode
	FCIE_RIU_16_OF(FCIE_DDR_MODE, BIT07|BIT08|BIT12);
	FCIE_RIU_16_ON(FCIE_DDR_MODE, BIT13|BIT14);

	if( !(FCIE_RIU_R16(MIU2_79) & MIU_SELECT_BY_IP))
	{
		printf(LIGHT_RED"SdWarn: MIU select not enable for SDIO IP\n"NONE);
		FCIE_RIU_16_ON(MIU2_79, MIU_SELECT_BY_IP);	// miu select by IP
	}
	#if 1
		FCIE_RIU_W16(FCIE_MIE_FUNC_CTL, BIT_SDIO_MOD);
		FCIE_RIU_16_ON(CHIPTOP_7B, BIT04); // reg_sdio_config = 1 --> MST224A/B/C
	#else
		FCIE_RIU_W16(A_MIE_FUNC_CTL(eIP), BIT_SDIO_MOD);
		FCIE_RIU_16_ON(CHIPTOP_5A, BIT08); // reg_sd_config = 1
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

#if 0
U8 HalSdio_SetSdioClock(U32 u32Clock)
{


	//FPGA mode
	//if(u32Clock == FCIE_SLOWEST_CLK)
	//printf("set clock :%x\n",u32Clock);
	//if (u32Clock < 400000)
	//   REG_FCIE_SETBIT(reg_emmcpll_0x1d, BIT0);
	//else
	//{
	 //  printf("set fast clock\n");
	//   REG_FCIE_CLRBIT(reg_emmcpll_0x1d, BIT0);
//	}

	REG_FCIE_SETBIT(FCIE_SD_MODE,BIT_CLK_EN);			//enable clk
	//printf("SD mode:%x\n",REG_FCIE(FCIE_SD_MODE));
	//else
	//	REG_FCIE_CLRBIT(reg_emmcpll_0x1d, BIT0);

	//g_eMMCDrv.u16_ClkRegVal = (U16)u16_ClkParam;


	return 0;
        }
#endif


#if !defined FPGA_BOARD || FPGA_BOARD==0

// find clock close to target but not over

static U8 u8CurrentPadType = SDIO_MODE_GPIO_PAD_BPS;

U32 HalSdio_SetSdioClock(U32 u32Clock)
{
    U8 u8ClockSlector;
	static U32 u32_OldClock=0xFFFFFFFF;

	//printf("HalSdio_SetSdioClock(%ld)\n", u32Clock);

	if(u32_OldClock == u32Clock)
		return 0;
	else
		u32_OldClock = u32Clock;

    FCIE_RIU_16_ON(REG_CLK_SDIO, WBIT00); // turn on clock gating

    if(u32Clock>1000)
    {
        DBG_CR_CLK(printf("Set FCIE clock as %d.%d MHz, ", u32Clock/1000, (u32Clock%1000)/100 ) );
    }
    else
    {
        DBG_CR_CLK(printf("Set FCIE clock as %d KHz, ", u32Clock));
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
        DBG_CR_CLK(printf("select FCIE clock as %d.%d MHz\r\n", FCIE_CLOCK[u8ClockSlector]/1000, (FCIE_CLOCK[u8ClockSlector]%1000)/100));
    }
    else
    {
        DBG_CR_CLK(printf("select FCIE clock as %d KHz\r\n", FCIE_CLOCK[u8ClockSlector]));
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


U32 HalFcie_clock_setting(U16 u16_ClkParam)
{
    DBG_CR_CLK(printf("HalFcie_clock_setting(%Xh)\n", u16_ClkParam));

    REG_FCIE_CLRBIT(FCIE_SD_MODE, BIT_CLK_EN);

    switch(u16_ClkParam)
    {
		// emmc_pll clock

		// clock_gen fcie clock
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

    {
		printf("clkgen clock\n");

        REG_FCIE_CLRBIT(REG_CLK_SDIO, BIT_SDIO_CLK_GATING|BIT_SDIO_CLK_INVERSE|BIT_CLKGEN_SDIO_MASK);
        REG_FCIE_SETBIT(REG_CLK_SDIO, BIT_SDIO_CLK_SRC_SEL|u16_ClkParam<<2);
    }


    return 0;
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

void HalFcie_SwitchPad(unsigned char u32Mode)
{
	// chiptop

	// sdio
	REG_FCIE_CLRBIT(FCIE_DDR_MODE, BIT_FALL_LATCH|BIT_PAD_IN_SEL_SD|BIT_32BIT_MACRO_EN|BIT_DDR_EN|BIT_8BIT_MACRO_EN);

	REG_FCIE_CLRBIT(FCIE_NC_FUNC_CTL, BIT_NC_EN);

	//REG_FCIE_W(FCIE_DDR_TOGGLE_CNT, 0x0000);

	switch(u32Mode)
	{
		case SDIO_MODE_GPIO_PAD_BPS:
			//printf("[monet] SDIO_MODE_GPIO_PAD_BPS\n");
			u8CurrentPadType = SDIO_MODE_GPIO_PAD_BPS;
			break;

		case SDIO_MODE_GPIO_PAD_SDR:
			//printf("[monet] SDIO_MODE_GPIO_PAD_SDR\n");
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

#endif



U8 halFCIE_Platform_ClearEvent(U16 nReq)
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


E_IO_STS HalFcie_WaitMieEvent(U16 u16ReqVal, U32 u32WaitMs)
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
			halFCIE_Platform_ClearEvent(BIT_ERR_STS);
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

			if(!gu8SilenceTuning)
			{
				HalFcie_DumpRegister();
				HalFcie_DumpDebugBus();
				//while(1);
			}

			return(IO_TIME_OUT);
		}
	}

    ///////////////////////////////////////////////////////////////////////////////////////////
    // Clear mie event
    // Only can clear request value,
    // because read operation might enable command and data transfer at the same time

    if (halFCIE_Platform_ClearEvent(u16ReqVal))
    {
	    return (IO_TIME_OUT);
    }

    return(IO_SUCCESS);
} // wait MIE Event End

void HalFcie_SetVDD(U8 u8OnOff)
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

