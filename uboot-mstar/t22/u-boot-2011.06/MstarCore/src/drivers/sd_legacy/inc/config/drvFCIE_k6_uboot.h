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

#ifndef  __DRVSDIO_K6_UBOOT_H__
#define  __DRVSDIO_K6_UBOOT_H__

// this file is use only for k6 project 2016.4.19

//#include <common.h> //printf()
//#include <sys/common/MsTypes.h>

int printf(const char *format, ...);


///////////////////////////////////////////////////////////////////////////////////////////////////
//
//                               PLATFORM FUNCTION DEFINITION
//
///////////////////////////////////////////////////////////////////////////////////////////////////

/* Enable D-CACHE */
#define MMC_CACHE  1

//#define USE_SD_MODE
#define SDR_MODE
///////////////////////////////////////////////////////////////////////////////////////////////////
//
//                               DATATYPE DEFINITION
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef NULL
#define NULL    ((void*)0)
#endif

#define U32  unsigned int
#define U16  unsigned short
#define U8   unsigned char
#define S32  signed int


#define BIT00 0x01
#define BIT01 0x02
#define BIT02 0x04
#define BIT03 0x08
#define BIT04 0x10
#define BIT05 0x20
#define BIT06 0x40
#define BIT07 0x80

#define WBIT00 0x0001
#define WBIT01 0x0002
#define WBIT02 0x0004
#define WBIT03 0x0008
#define WBIT04 0x0010
#define WBIT05 0x0020
#define WBIT06 0x0040
#define WBIT07 0x0080
#define WBIT08 0x0100
#define WBIT09 0x0200
#define WBIT10 0x0400
#define WBIT11 0x0800
#define WBIT12 0x1000
#define WBIT13 0x2000
#define WBIT14 0x4000
#define WBIT15 0x8000

///////////////////////////////////////////////////////////////////////////////////////////////////
#define BY_PASS_MODE  1
//#define _8BIT_MARCO  1
//#define SDR_MODE    0

//=====================================================
// HW registers
//=====================================================
#define REG_OFFSET_SHIFT_BITS           2

#define REG_FCIE_U16(Reg_Addr)          (*(volatile U16*)(Reg_Addr))
#define GET_REG_ADDR(x, y)              ((x)+((y) << REG_OFFSET_SHIFT_BITS))

#define REG_FCIE(reg_addr)              REG_FCIE_U16(reg_addr)
#define REG_FCIE_W(reg_addr, val)       REG_FCIE(reg_addr) = (val)
#define REG_FCIE_R(reg_addr, val)       val = REG_FCIE(reg_addr)
#define REG_FCIE_SETBIT(reg_addr, val)  REG_FCIE(reg_addr) |= (val)
#define REG_FCIE_CLRBIT(reg_addr, val)  REG_FCIE(reg_addr) &= ~(val)
#define REG_FCIE_W1C(reg_addr, val)     REG_FCIE_W(reg_addr, REG_FCIE(reg_addr)&(val))

#define FCIE_RIU_W16(addr,value) *((volatile U16*)(addr)) = (value)
#define FCIE_RIU_R16(addr)       *((volatile U16*)(addr))
// read modify write 16 bits register macro
#define FCIE_RIU_16_ON(addr,value) FCIE_RIU_W16(addr, FCIE_RIU_R16(addr)|(value))
#define FCIE_RIU_16_OF(addr,value) FCIE_RIU_W16(addr, FCIE_RIU_R16(addr)&(~(value)))




////////////////////////////////////////////////////////////////////////////////////////////////////
// RIU BASE ADDRESS
////////////////////////////////////////////////////////////////////////////////////////////////////

// kernel
//#define RIU_PM_BASE			0xFD000000
//#define RIU_BASE				0xFD200000

// uboot
//#define RIU_PM_BASE			0x1F000000
//#define RIU_BASE				0x1F200000

#define FCIE_IP					1
#define SDIO_IP					2
#define ALTERNATIVE_IP			SDIO_IP

#define SDIO_IP_VERIFY

#define RIU_BASE				0x1F000000

#define RIU_BANK_2_BASE(BANK)		(RIU_BASE+(BANK<<9))
// register num per bank --> 0x80 = 128
// register addr offset  --> 4

////////////////////////////////////////////////////////////////////////////////////////////////////
// RIU BANK DEFINITION
////////////////////////////////////////////////////////////////////////////////////////////////////
#define RIU_BANK_TIMER1			0x0030UL
#define RIU_BANK_MIU2			0x1006UL
#define RIU_BANK_CLKGEN2		0x100AUL
#define RIU_BANK_CLKGEN0		0x100BUL
#define RIU_BANK_MIU			0x1012UL
#define RIU_BANK_CHIPTOP		0x101EUL
#if (ALTERNATIVE_IP == FCIE_IP)
#define RIU_BANK_SDIO0			0x1113UL
#define RIU_BANK_SDIO1			0x1114UL
#define RIU_BANK_SDIO2			0x1115UL
#elif (ALTERNATIVE_IP == SDIO_IP)
#define RIU_BANK_SDIO0			0x111FUL
#define RIU_BANK_SDIO1			0x1120UL
#define RIU_BANK_SDIO2			0x1121UL
#endif
#define RIU_BANK_PADTOP0		0x1025UL
#define RIU_BANK_CHIP_GPIO		0x102BUL
//#define RIU_BANK_SDIO_PLL		0x123EUL // these is no SDIO PLL in maserati
#define RIU_BANK_EMMC_PLL		0x123FUL
#define RIU_BANK_CODEC_BLOCK	0x1712UL


#define RIU_BASE_TIMER1			RIU_BANK_2_BASE(RIU_BANK_TIMER1)
#define RIU_BASE_MIU2			RIU_BANK_2_BASE(RIU_BANK_MIU2)
#define RIU_BASE_CLKGEN2		RIU_BANK_2_BASE(RIU_BANK_CLKGEN2)
#define RIU_BASE_CLKGEN0		RIU_BANK_2_BASE(RIU_BANK_CLKGEN0)
#define RIU_BASE_MIU			RIU_BANK_2_BASE(RIU_BANK_MIU)
#define RIU_BASE_CHIPTOP		RIU_BANK_2_BASE(RIU_BANK_CHIPTOP)
#define RIU_BASE_SDIO0			RIU_BANK_2_BASE(RIU_BANK_SDIO0) // main bank
#define RIU_BASE_SDIO1			RIU_BANK_2_BASE(RIU_BANK_SDIO1) // CIFD + CRC
#define RIU_BASE_SDIO2			RIU_BANK_2_BASE(RIU_BANK_SDIO2) // power save bank
#define RIU_BASE_PADTOP0		RIU_BANK_2_BASE(RIU_BANK_PADTOP0)
#define RIU_BASE_CHIP_GPIO		RIU_BANK_2_BASE(RIU_BANK_CHIP_GPIO)
#define RIU_BASE_EMMC_PLL		RIU_BANK_2_BASE(RIU_BANK_EMMC_PLL)
#define RIU_BASE_CODEC_BLOCK	RIU_BANK_2_BASE(RIU_BANK_CODEC_BLOCK)

////////////////////////////////////////////////////////////////////////////////////////////////////

#define FCIE_REG_BASE_ADDR              (RIU_BASE_SDIO0)
#define FCIE_POWEER_SAVE_MODE_BASE		(RIU_BASE_SDIO2)
#define FCIE_CMDFIFO_BASE_ADDR          (RIU_BASE_SDIO0 + (0x20<<2)) // CIFC command FIFO
#define FCIE_CIFD_FIFO_W				(RIU_BASE_SDIO1)
#define FCIE_CIFD_FIFO_R				(RIU_BASE_SDIO1 + (0x20<<2))
#define FCIE_CRC_BUF					(RIU_BASE_SDIO1 + (0x40<<2))

#define SD_USE_FCIE5		1
#define SDIO_D1_INTR_VER	2

#include "drvFCIE5_reg.h"

#define RIU_UNIT_SHIFT           2


////////////////////////////////////////////////////////////////////////////////////////////////////
// RIU_BANK_TIMER1 0x0030
////////////////////////////////////////////////////////////////////////////////////////////////////
#define TIMER1_ENABLE                   (RIU_BASE_TIMER1+(0x20<<RIU_UNIT_SHIFT))
#define TIMER1_HIT                      (RIU_BASE_TIMER1+(0x21<<RIU_UNIT_SHIFT))
#define TIMER1_MAX_LOW                  (RIU_BASE_TIMER1+(0x22<<RIU_UNIT_SHIFT))
#define TIMER1_MAX_HIGH                 (RIU_BASE_TIMER1+(0x23<<RIU_UNIT_SHIFT))
#define TIMER1_CAP_LOW                  (RIU_BASE_TIMER1+(0x24<<RIU_UNIT_SHIFT))
#define TIMER1_CAP_HIGH                 (RIU_BASE_TIMER1+(0x25<<RIU_UNIT_SHIFT))


////////////////////////////////////////////////////////////////////////////////////////////////////
// RIU_BANK_MIU2 0x1006
////////////////////////////////////////////////////////////////////////////////////////////////////
#define MIU2_7A							(RIU_BASE_MIU2+(0x7A << RIU_UNIT_SHIFT))
#define MIU_SELECT_BY_FCIE				(BIT09)
#define MIU_SELECT_BY_SDIO				(BIT00)


////////////////////////////////////////////////////////////////////////////////////////////////////
// RIU_BANK_CLKGEN0 0x100B
////////////////////////////////////////////////////////////////////////////////////////////////////
#if (ALTERNATIVE_IP == FCIE_IP)
#define REG_CLK_SDIO					(RIU_BASE_CLKGEN0+(0x64<<RIU_UNIT_SHIFT))

#define BIT_SDIO_CLK_GATING				(BIT08)
#define BIT_SDIO_CLK_INVERSE			(BIT09)
#define BIT_CLKGEN_SDIO_SHIFT			10
#define BIT_CLKGEN_SDIO_MASK			(BIT13|BIT12|BIT11|BIT10)
#define BIT_SDIO_CLK_SRC_SEL			(BIT14) // 0: clk_xtal 12M, 1: clk_nfie_p1
#elif (ALTERNATIVE_IP == SDIO_IP)
#define REG_CLK_SDIO					(RIU_BASE_CLKGEN2+(0x3E<<RIU_UNIT_SHIFT))

#define BIT_SDIO_CLK_GATING				(BIT00)
#define BIT_SDIO_CLK_INVERSE			(BIT01)
#define BIT_CLKGEN_SDIO_SHIFT			2
#define BIT_CLKGEN_SDIO_MASK			(BIT05|BIT04|BIT03|BIT02)
#define BIT_SDIO_CLK_SRC_SEL			(BIT06) // 0: clk_xtal 12M, 1: clk_nfie_p1
#endif



////////////////////////////////////////////////////////////////////////////////////////////////////
// RIU_BANK_MIU 0x1012
////////////////////////////////////////////////////////////////////////////////////////////////////
#define MIU_GROUP2_REQ_MSK		(RIU_BASE_MIU+(0x43 << RIU_UNIT_SHIFT))
#define MIU_SDIO_MSK			BIT10 // 1: mask request from SDIO
#define MIU_FCIE_MSK			BIT09 // 0: mask request from FCIE


////////////////////////////////////////////////////////////////////////////////////////////////////
// RIU_BANK_CHIPTOP 0x101E
////////////////////////////////////////////////////////////////////////////////////////////////////
#define CHIPTOP_08h						(RIU_BASE_CHIPTOP+(0x08 << RIU_UNIT_SHIFT))
#define CHIPTOP_50h						(RIU_BASE_CHIPTOP+(0x50 << RIU_UNIT_SHIFT))

// CHIPTOP_08h
#define REG_SDIO_CONFIG_MSK				(BIT11)
#define REG_SDIO_MODE_1					(BIT11)
#define REG_SD_CONFIG					(BIT08)
#define REG_NAND_MODE					(BIT03)
#define REG_EMMC_CONFIG					(BIT00)

// CHIPTOP_50h
#define REG_ALL_PAD_IN					(BIT15)


////////////////////////////////////////////////////////////////////////////////////////////////////
// RIU_BANK_PADTOP0 0x1025
////////////////////////////////////////////////////////////////////////////////////////////////////
#define REG_PADTOP0_61h				(RIU_BASE_PADTOP0+(0x61 << RIU_UNIT_SHIFT))

// 0x61
// this GPIO 9 is SD_PWR_CTRL dedicate pin
#define REG_GPIO9_OEN					(BIT05)
#define REG_GPIO9_OUT					(BIT04)


////////////////////////////////////////////////////////////////////////////////////////////////////
// RIU_BANK_CHIP_GPIO 0x102B
////////////////////////////////////////////////////////////////////////////////////////////////////
#define CHIP_GPIO_47		(RIU_BASE_CHIP_GPIO+(0x47 << RIU_UNIT_SHIFT)) // maserati MST244A
#define TGPIO1_OEN 		BIT1 // low --> output
#define TGPIO1_OUT 		BIT0 // low power on, high power off


////////////////////////////////////////////////////////////////////////////////////////////////////
// RIU_BANK_CODEC_BLOCK 0x1712
////////////////////////////////////////////////////////////////////////////////////////////////////
#define REG_FCIE_MCM				(RIU_BASE_CODEC_BLOCK+ (0x02 << RIU_UNIT_SHIFT))
#define REG_SDIO_MCM				(RIU_BASE_CODEC_BLOCK+ (0x03 << RIU_UNIT_SHIFT))

// 0x02
#define BIT_FCIE_RATIO_MSK			(BIT15|BIT14|BIT13|BIT12)
#define BIT_FCIE_RATIO_SHIFT		12

// 0x03
#define BIT_SDIO_RATIO_MSK			(BIT07|BIT06|BIT05|BIT04)
#define BIT_SDIO_RATIO_SHIFT		4


////////////////////////////////////////////////////////////////////////////////////////////////////


#define HW_TIMER_DELAY_1us              1
#define HW_TIMER_DELAY_5us              5
#define HW_TIMER_DELAY_10us             10
#define HW_TIMER_DELAY_100us            100
#define HW_TIMER_DELAY_500us            500
#define HW_TIMER_DELAY_1ms              (1000 * HW_TIMER_DELAY_1us)
#define HW_TIMER_DELAY_5ms              (5    * HW_TIMER_DELAY_1ms)
#define HW_TIMER_DELAY_10ms             (10   * HW_TIMER_DELAY_1ms)
#define HW_TIMER_DELAY_100ms            (100  * HW_TIMER_DELAY_1ms)
#define HW_TIMER_DELAY_500ms            (500  * HW_TIMER_DELAY_1ms)
#define HW_TIMER_DELAY_1s               (1000 * HW_TIMER_DELAY_1ms)



int printf(const char *format, ...);




///////////////////////////////////////////////////////////////////////////////////////////////////
//
//                               EXTERN GLOBAL FUNCTION
//
///////////////////////////////////////////////////////////////////////////////////////////////////


#define SDIO_MODE_GPIO_PAD_BPS			1
#define SDIO_MODE_GPIO_PAD_SDR			2
#define SDIO_MODE_8BITS_MACRO_SDR		3
#define SDIO_MODE_8BITS_MACRO_DDR		4
#define SDIO_MODE_32BITS_MACRO_DDR		5
#define SDIO_MODE_32BITS_MACRO_SDR104	6
#define SDIO_MODE_UNKNOWN				7

#define SDIO_PAD_SDR104			SDIO_MODE_32BITS_MACRO_SDR104
#define SDIO_PAD_SDR50			SDIO_MODE_32BITS_MACRO_SDR104
#define SDIO_PAD_DDR50			SDIO_MODE_8BITS_MACRO_DDR
#if 0
#define SDIO_PAD_SDR25			SDIO_MODE_GPIO_PAD_BPS
#define SDIO_PAD_SDR12			SDIO_MODE_GPIO_PAD_BPS
#else
#define SDIO_PAD_SDR25			SDIO_MODE_GPIO_PAD_SDR
#define SDIO_PAD_SDR12			SDIO_MODE_GPIO_PAD_SDR
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////
// new feature or patch different with other project
///////////////////////////////////////////////////////////////////////////////////////////////////
#define SDIO_NEW_R2N_PATCH // R2N mode new patch from maserati 2015.06.09
#if (ALTERNATIVE_IP == SDIO_IP)
#define SDIO_R2N_WRSTATE_PATCH // FCIE use R2N ready evnet bit, SDIO need this patch to prevent FIFO overwrite issue
#endif

#define SDIO_MCM_TEST			1

#define SDIO_ERRDET 			1

#if (defined SDIO_ERRDET) && (SDIO_ERRDET==1)
#define SDIO_ERRDET_RSP_TO		1
#define SDIO_ERRDET_RSP_CRC_ERR	1
#define SDIO_ERRDET_R_CRC_ERR	1
#define SDIO_ERRDET_W_CRC_STS	1
#define SDIO_ERRDET_R_SBIT_TO	1 // maserati fix counter 2 issue
#define SDIO_ERRDET_W_MIU_TO	1 //
#endif

typedef struct _SKEWER {

	unsigned int u32LatchOKStart;
	unsigned int u32LatchOKEnd;
	unsigned int u32LatchBest;
	unsigned char u8_edge;

} SKEWER;

//#define CONFIG_MIU0_BUSADDR 0x20000000
//#define CONFIG_MIU1_BUSADDR 0xA0000000

U32		HalSdio_SlectBestSkew4(U32 u32_Candidate, SKEWER * pSkewer);
void	HalSdio_SetTuning(U8 u8Type, U8 u8Count);
void	HalSdio_SetTriggerLevel(U8 u8STrigLevel);
void	HalSdio_SetDrivingStrength(U16 u16DrivingLevel);
void	HalSdio_PatchCheckR2nWriteState(void);

#endif // __DRVSDIO_K6_UBOOT_H__

