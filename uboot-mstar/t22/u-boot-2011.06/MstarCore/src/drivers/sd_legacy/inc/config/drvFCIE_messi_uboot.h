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
#ifndef __SD_MUNICH_UBOOT__
#define __SD_MUNICH_UBOOT__

//#include <string.h>

#define U32  unsigned int
#define U16  unsigned short
#define U8   unsigned char
#define S32  signed int

/*#ifndef U32
#define U32  unsigned long
#endif
#ifndef U16
#define U16  unsigned short
#endif
#ifndef U8
#define U8   unsigned char
#endif
#ifndef S32
#define S32  signed long
#endif
#ifndef S16
#define S16  signed short
#endif
#ifndef S8
#define S8   signed char
#endif*/


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

#define RIU_BASE				0x1F000000

#define RIU_BASE_MIU2			(RIU_BASE+(0x1006UL<<9))
#define RIU_BASE_CLKGEN0        (RIU_BASE+(0x100BUL<<9))
#define RIU_BASE_CHIPTOP        (RIU_BASE+(0x101EUL<<9))
//#define RIU_BASE_GPIO           (RIU_BASE+(0x102BUL<<9))
#define RIU_BASE_SDIO0			(RIU_BASE+(0x120FUL<<9)) // main bank
#define RIU_BASE_SDIO1			(RIU_BASE+(0x1228UL<<9)) // CIFD
#define RIU_BASE_SDIO2			(RIU_BASE+(0x1229UL<<9)) // power save mdoe
//#define RIU_BASE_SDIO_PLL		(RIU_BASE+(0x123EUL<<9))



#define FCIE_REG_BASE_ADDR              (RIU_BASE_SDIO0)
#define FCIE_POWEER_SAVE_MODE_BASE		(RIU_BASE_SDIO2)
#define FCIE_CMDFIFO_BASE_ADDR          (RIU_BASE_SDIO0 + (0x20<<2)) // CIFC command FIFO
#define FCIE_CIFD_FIFO_W				(RIU_BASE_SDIO1)
#define FCIE_CIFD_FIFO_R				(RIU_BASE_SDIO1 + (0x20<<2))
#define FCIE_CRC_BUF					(RIU_BASE_SDIO1 + (0x40<<2))

#define SD_USE_FCIE5   1

#include "drvFCIE5_reg.h"

#define RIU_UNIT_SHIFT           2






////////////////////////////////////////////////////////////////////////////////////////////////////
// RIU_BANK_MIU2 0x1006
////////////////////////////////////////////////////////////////////////////////////////////////////
#define MIU2_79					(RIU_BASE_MIU2+ (0x79 << RIU_UNIT_SHIFT))
#define MIU_SELECT_BY_IP		BIT10


////////////////////////////////////////////////////////////////////////////////////////////////////
// RIU_BANK_CLKGEN0 0x100B
////////////////////////////////////////////////////////////////////////////////////////////////////
#define REG_CLK_SDIO            (RIU_BASE_CLKGEN0+(0x64<<RIU_UNIT_SHIFT))
//#define REG_CLK_EMMC            (RIU_BASE_CLKGEN0+(0x31<<RIU_UNIT_SHIFT))
#define REG_CLK_65				(RIU_BASE_CLKGEN0+(0x65<<RIU_UNIT_SHIFT))
#define REG_CLK_64				(RIU_BASE_CLKGEN0+(0x64<<RIU_UNIT_SHIFT))
#define REG_CLK_69				(RIU_BASE_CLKGEN0+(0x69<<RIU_UNIT_SHIFT))

#define BIT_SDIO_CLK_GATING		BIT0
#define BIT_SDIO_CLK_INVERSE	BIT1
#define BIT_CLKGEN_SDIO_MASK	(BIT5|BIT4|BIT3|BIT2)
#define BIT_SDIO_CLK_SRC_SEL	BIT6 // 0: clk_xtal 12M, 1: clk_nfie_p1


////////////////////////////////////////////////////////////////////////////////////////////////////
// MIU BANK 0x1012
////////////////////////////////////////////////////////////////////////////////////////////////////
#define MIU_79					(RIU_BASE_MIU2+ (0x79 << RIU_UNIT_SHIFT))
#define MIU_PRIORITY_FIX_SEL	BIT10 // 0: fix MIU0, 1: fix MIU1


////////////////////////////////////////////////////////////////////////////////////////////////////
// RIU_BANK_CHIPTOP 0x101E
////////////////////////////////////////////////////////////////////////////////////////////////////
#define CHIPTOP_6F				(RIU_BASE_CHIPTOP+(0x6F << RIU_UNIT_SHIFT))
#define CHIPTOP_5A				(RIU_BASE_CHIPTOP+(0x5A << RIU_UNIT_SHIFT))
#define CHIPTOP_7B				(RIU_BASE_CHIPTOP+(0x7B << RIU_UNIT_SHIFT))
#define CHIPTOP_43				(RIU_BASE_CHIPTOP+(0x43 << RIU_UNIT_SHIFT))
#define CHIPTOP_50				(RIU_BASE_CHIPTOP+(0x50 << RIU_UNIT_SHIFT))

////////////////////////////////////////////////////////////////////////////////////////////////////
// RIU_BANK_GPIO 0x102B
////////////////////////////////////////////////////////////////////////////////////////////////////
//#define CHIP_GPIO_47		(RIU_BASE_GPIO+(0x47 << RIU_UNIT_SHIFT)) // MUJI MST215A power control
//#define GPIO158_OEN 		BIT1 // low --> output
//#define GPIO158_OUT 		BIT0 // low power on, high power off

//#define CHIP_GPIO_52		(RIU_BASE_GPIO+(0x52 << RIU_UNIT_SHIFT)) // MUJI MST215A card detect
//#define GPIO87_OEN			BIT1 // high --> input
//#define GPIO87_IN			BIT2 // low --> insert, high --> remove


//--------------------------------macro define---------------------------------
#define ENABLE_8BIT_MACRO               0

#if !(defined(ENABLE_8BIT_MACRO) && ENABLE_8BIT_MACRO)
#define ENABLE_32BIT_MACRO              1
#endif

//--------------------------------clock gen------------------------------------

#define BIT_FCIE_CLK_XTAL			0x00
#define BIT_FCIE_CLK_20M			0x01
#define BIT_FCIE_CLK_32M			0x02
#define BIT_FCIE_CLK_36M			0x03
#define BIT_FCIE_CLK_40M			0x04
#define BIT_FCIE_CLK_43M			0x05
#define BIT_FCIE_CLK_54M			0x06
#define BIT_FCIE_CLK_62M			0x07
#define BIT_FCIE_CLK_72M			0x08
#define BIT_FCIE_CLK_86M			0x09
#define BIT_FCIE_CLK_EMMC_PLL_1X	0x0B // 8 bits macro & 32 bit macro HS200
#define BIT_FCIE_CLK_EMMC_PLL_2X	0x0C // 32 bit macroDDR & HS400
#define BIT_FCIE_CLK_300K			0x0D
#define BIT_FCIE_CLK_XTAL2			0x0E
#define BIT_FCIE_CLK_48M			0x0F

#define BIT_FCIE_CLK_52M                0x10 // emmc pll use only
#define BIT_FCIE_CLK_120M               0x11 // emmc pll use only
#define BIT_FCIE_CLK_140M               0x12 // emmc pll use only
#define BIT_FCIE_CLK_160M               0x13 // emmc pll use only
#define BIT_FCIE_CLK_200M               0x14 // emmc pll use only

#define eMMC_PLL_FLAG                   0x80
#define eMMC_PLL_CLK__20M               (0x01|eMMC_PLL_FLAG)
#define eMMC_PLL_CLK__27M               (0x02|eMMC_PLL_FLAG)
#define eMMC_PLL_CLK__32M               (0x03|eMMC_PLL_FLAG)
#define eMMC_PLL_CLK__36M               (0x04|eMMC_PLL_FLAG)
#define eMMC_PLL_CLK__40M               (0x05|eMMC_PLL_FLAG)
#define eMMC_PLL_CLK__48M               (0x06|eMMC_PLL_FLAG)
#define eMMC_PLL_CLK__52M               (0x07|eMMC_PLL_FLAG)
#define eMMC_PLL_CLK__62M               (0x08|eMMC_PLL_FLAG)
#define eMMC_PLL_CLK__72M               (0x09|eMMC_PLL_FLAG)
#define eMMC_PLL_CLK__80M               (0x0A|eMMC_PLL_FLAG)
#define eMMC_PLL_CLK__86M               (0x0B|eMMC_PLL_FLAG)
#define eMMC_PLL_CLK_100M               (0x0C|eMMC_PLL_FLAG)
#define eMMC_PLL_CLK_120M               (0x0D|eMMC_PLL_FLAG)
#define eMMC_PLL_CLK_140M               (0x0E|eMMC_PLL_FLAG)
#define eMMC_PLL_CLK_160M               (0x0F|eMMC_PLL_FLAG)
#define eMMC_PLL_CLK_200M               (0x10|eMMC_PLL_FLAG)



#define eMMC_FCIE_VALID_CLK_CNT         1// FIXME

#define PLL_SKEW4_CNT               9
#define MIN_OK_SKEW_CNT             5



#define REG_BANK_TIMER1                 0x1800
#define TIMER1_BASE                     GET_REG_ADDR(RIU_PM_BASE, REG_BANK_TIMER1)

#define TIMER1_ENABLE                   GET_REG_ADDR(TIMER1_BASE, 0x20)
#define TIMER1_HIT                      GET_REG_ADDR(TIMER1_BASE, 0x21)
#define TIMER1_MAX_LOW                  GET_REG_ADDR(TIMER1_BASE, 0x22)
#define TIMER1_MAX_HIGH                 GET_REG_ADDR(TIMER1_BASE, 0x23)
#define TIMER1_CAP_LOW                  GET_REG_ADDR(TIMER1_BASE, 0x24)
#define TIMER1_CAP_HIGH                 GET_REG_ADDR(TIMER1_BASE, 0x25)


//=====================================================
// API declarations
//=====================================================
#define eMMC_HW_TIMER_MHZ               12000000//12MHz  [FIXME]

#define FCIE_eMMC_MACRO_8BIT            0
#define FCIE_eMMC_MACRO_32BIT           1

#define FCIE_eMMC_DISABLE               0
#define FCIE_eMMC_DDR                   1
#define FCIE_eMMC_SDR                   2
#define FCIE_eMMC_BYPASS                3 // never use this
#define FCIE_eMMC_TMUX                  4
#define FCIE_eMMC_HS200                 5
#define FCIE_eMMC_HS400                 6
#define FCIE_eMMC_SDR_GPIO              7
#define FCIE_eMMC_HS400_DS              8

#define eMMC_BOOT_PART_W                BIT0
#define eMMC_BOOT_PART_R                BIT1




//=====================================================
// Driver configs
//=====================================================
// [CAUTION]: to verify IP and HAL code, defaut 0
#define IF_IP_VERIFY                    1 // [FIXME] -->

#define FCIE_EMMC_BOOT_MODE             0

#if defined(FCIE_EMMC_BOOT_MODE) && FCIE_EMMC_BOOT_MODE
#define FCIE_EMMC_BOOT_TO_SRAM          0
#endif

#define WRITE_TEST_BOOT_CODE            0

#define DELAY_FOR_DEBUGGING_BUS         0
#define DELAY_FOR_BRIAN                 0

#define FCIE_GPIO_PAD_VERIFY            1
#define FCIE_SDR_FLASH_MACRO_VERIFY     1
#define FCIE_DDR52_VERIFY               1
#define FCIE_HS200_VERIFY               1
#define FCIE_HS400_VERIFY               1
#define FCIE_REGRESSION_TEST            0

// [CAUTION]: to detect DDR timiing parameters, only for DL
#define IF_DETECT_eMMC_DDR_TIMING       1
#define eMMC_IF_DDRT_TUNING()           (g_eMMCDrv.u32_DrvFlag&DRV_FLAG_DDR_TUNING)

// need to eMMC_pads_switch
// need to eMMC_clock_setting
#define IF_FCIE_SHARE_IP                1

//------------------------------
#define FICE_BYTE_MODE_ENABLE           1 // always 1
#define ENABLE_eMMC_INTERRUPT_MODE      0
#define ENABLE_eMMC_RIU_MODE            0 // for debug cache issue
#define ENABLE_eMMC_ATOP                0 // after einstein 2013-JUN-14 might has this feature
#define ENABLE_eMMC_HS200               0 // after einstein 2013-JUN-14 might has this feature
#define ENABLE_eMMC_HS400               1

#if !FCIE_GPIO_PAD_VERIFY
#undef ENABLE_eMMC_RIU_MODE
#define ENABLE_eMMC_RIU_MODE            0
#endif

#if ENABLE_eMMC_RIU_MODE
#undef IF_DETECT_eMMC_DDR_TIMING
#define IF_DETECT_eMMC_DDR_TIMING       0 // RIU mode can NOT use DDR
#endif
// <-- [FIXME]

//------------------------------


//=====================================================
// unit for HW Timer delay (unit of us)
//=====================================================

/*
#define HW_TIMER_DELAY_1us              1
#define HW_TIMER_DELAY_5us              1
#define HW_TIMER_DELAY_10us             1
#define HW_TIMER_DELAY_100us            1
#define HW_TIMER_DELAY_500us            1
#define HW_TIMER_DELAY_1ms              (1 * HW_TIMER_DELAY_1us)
#define HW_TIMER_DELAY_5ms              (5    * HW_TIMER_DELAY_1ms)
#define HW_TIMER_DELAY_10ms             (10   * HW_TIMER_DELAY_1ms)
#define HW_TIMER_DELAY_100ms            (100  * HW_TIMER_DELAY_1ms)
#define HW_TIMER_DELAY_500ms            (500  * HW_TIMER_DELAY_1ms)
#define HW_TIMER_DELAY_1s               (1000 * HW_TIMER_DELAY_1ms)
*/

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


//=====================================================
// set FCIE clock
//=====================================================
// [FIXME] -->
#define FCIE_SLOWEST_CLK                BIT_FCIE_CLK_300K
#define FCIE_SLOW_CLK                   BIT_FCIE_CLK_48M
#define FCIE_DEFAULT_CLK                BIT_FCIE_CLK_48M // BIT_FCIE_CLK_36M
#define FCIE_HS200_CLK                  BIT_FCIE_CLK_200M //BIT_FCIE_CLK_52M
// <-- [FIXME]
//=====================================================
// transfer DMA Address
//=====================================================
#define MIU_BUS_WIDTH_BITS              4// 8 bytes width [FIXME]
/*
 * Important:
 * The following buffers should be large enough for a whole eMMC block
 */
// FIXME, this is only for verifing IP
#define DMA_W_ADDR                      0x00300000
#define DMA_R_ADDR                      0x00600000
#define DMA_W_SPARE_ADDR                0x00900000
#define DMA_R_SPARE_ADDR                0x00A80000
#define DMA_BAD_BLK_BUF                 0x00C00000

int printf(const char *format, ...);


//=====================================================
// misc
//=====================================================
//#define BIG_ENDIAN
#define LITTLE_ENDIAN

#define BIT_DQS_MODE_MASK               (BIT0|BIT1|BIT2)
#define BIT_DQS_MODE_SHIFT              0


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
#define SDIO_PAD_SDR25			SDIO_MODE_GPIO_PAD_SDR
#define SDIO_PAD_SDR12			SDIO_MODE_GPIO_PAD_SDR

//void FCIE_HWTimer_Start(void);
//U32 FCIE_HWTimer_End(void);
extern unsigned char halFCIE_Platform_ClearEvent(unsigned short nReq);
extern void halFCIE_Platform_InitChiptop(void);
//extern E_IO_STS halFCIE_Platform_WaitMieEvent(U16 u16ReqVal, U32 u32WaitMs);
extern void HalSdio_ResetIP(void);

typedef struct _SKEWER {

	unsigned int u32LatchOKStart;
	unsigned int u32LatchOKEnd;
	unsigned int u32LatchBest;
	unsigned char u8_edge;

} SKEWER;

U32 HalSdio_SlectBestSkew4(U32 u32_Candidate, SKEWER * pSkewer);
void HalSdio_SetTuning(U8 u8Type, U8 u8Count);
void HalSdio_SetTriggerLevel(U8 u8STrigLevel);

#endif /* __SD_MUNICH_UBOOT__ */


