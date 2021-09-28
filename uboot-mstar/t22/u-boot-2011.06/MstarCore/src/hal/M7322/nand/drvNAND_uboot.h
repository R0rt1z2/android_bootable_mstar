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
#ifndef __UNFD_M7322_UBOOT_H__
#define __UNFD_M7322_UBOOT_H__

#include <linux/string.h>
#include <common.h>
#include <jffs2/load_kernel.h>

#define REG_BANK_CLKGEN				(0x580)
#define REG_BANK_CHIPTOP			(0xF00)
#define REG_BANK_FCIE0				(0x8980)
#define REG_BANK_FCIE2				(0x8A00)
#define REG_BANK_FCIEPOWERSAVEMODE  (0x8A80UL)
#define REG_BANK_TIMER0				(0x1800)
#define REG_BANK_EMMCPLL				(0x11F80)
#define REG_BANK_CLKGEN2			(0x500)

#define RIU_PM_BASE					0x1F000000
#define RIU_BASE					0x1F200000

#define REG(Reg_Addr)				(*(volatile U16*)(Reg_Addr))
#define REG_OFFSET_SHIFT_BITS		2
#define GET_REG_ADDR(x, y)			(x+((y)<<REG_OFFSET_SHIFT_BITS))

#define REG_WRITE_UINT16(reg_addr, val)		REG(reg_addr) = val
#define REG_READ_UINT16(reg_addr, val)		val = REG(reg_addr)
#define HAL_WRITE_UINT16(reg_addr, val)		(REG(reg_addr) = val)
#define HAL_READ_UINT16(reg_addr, val)		val = REG(reg_addr)
#define REG_SET_BITS_UINT16(reg_addr, val)	REG(reg_addr) |= (val)
#define REG_CLR_BITS_UINT16(reg_addr, val)	REG(reg_addr) &= ~(val)
#define REG_W1C_BITS_UINT16(reg_addr, val)	REG_WRITE_UINT16(reg_addr, REG(reg_addr)&(val))

#define MPLL_CLK_REG_BASE_ADDR              GET_REG_ADDR(RIU_BASE, REG_BANK_CLKGEN)
#define CHIPTOP_REG_BASE_ADDR               GET_REG_ADDR(RIU_BASE, REG_BANK_CHIPTOP)
#define FCIE_REG_BASE_ADDR                  GET_REG_ADDR(RIU_BASE, REG_BANK_FCIE0)
#define FCIE_NC_CIFD_BASE                   GET_REG_ADDR(RIU_BASE, REG_BANK_FCIE2)
#define FCIE_NC_WBUF_CIFD_BASE 	GET_REG_ADDR(RIU_BASE, REG_BANK_FCIE2)	
#define FCIE_NC_RBUF_CIFD_BASE 	GET_REG_ADDR(RIU_BASE, REG_BANK_FCIE2 + 0x20)	
#define FCIE_REG_PWS_BASE_ADDR		(RIU_PM_BASE+(0x111500<<1))
//#define FCIE_REG_TEE_BASE_ADDR              (RIU_PM_BASE+(0x111600<<1))		to verify TEE BANK
#define FCIE_POWEER_SAVE_MODE_BASE          GET_REG_ADDR(RIU_BASE, REG_BANK_FCIEPOWERSAVEMODE)
#define MPLL_CLK2_REG_BASE_ADDR              GET_REG_ADDR(RIU_BASE, REG_BANK_CLKGEN2)


#define TIMER0_REG_BASE_ADDR                GET_REG_ADDR(RIU_PM_BASE, REG_BANK_TIMER0)
#define EMMC_PLL_REG_BASE_ADDR			GET_REG_ADDR(RIU_BASE, REG_BANK_EMMCPLL)

#define REG50_ECC_CTRL_INIT_VALUE	0

#define UNFD_ST_PLAT				0x80000000
#define IF_IP_VERIFY				0 // [CAUTION]: to verify IP and HAL code, defaut 0

// debug
#define FCIE3_DUMP_DEBUG_BUS		1

#define NC_SEL_FCIE5            1
#if NC_SEL_FCIE5
#include "drvNAND_reg_v5.h"
#else
#error "Error! no FCIE registers selected."
#endif

#define ENABLE_32BIT_MACRO			1
#define ENABLE_8BIT_MACRO				0

//FCIE5 DDR Nand
#define DDR_NAND_SUPPORT					0
#if defined(DDR_NAND_SUPPORT) && DDR_NAND_SUPPORT
#define DDR_EMMC_PLL				1
//#define FCIE4_DDR_RETRY_DQS			1
#define NAND_DELAY_CELL_PS			300 // pico-second

#define ONFI_NVDDR2_SUPPORT                 0

#endif

#define FCIE_LFSR					1

#define SPARE640B_CIFD512B_PATCH    0

#if (defined(DDR_NAND_SUPPORT) && DDR_NAND_SUPPORT)
#define NC_SET_DDR_MODE()           REG_WRITE_UINT16(NC_DDR_CTRL, pNandDrv->u16_Reg58_DDRCtrl);
#define NC_CLR_DDR_MODE()           REG_CLR_BITS_UINT16(NC_DDR_CTRL, BIT_DDR_MASM);
#else
#define NC_SET_DDR_MODE()
#define NC_CLR_DDR_MODE()
#endif

#if (defined(DDR_NAND_SUPPORT) && DDR_NAND_SUPPORT)
#define NC_TEE_SET_DDR_MODE()                   REG_WRITE_UINT16(NC_TEE_DDR_CTRL, pNandDrv->u16_Reg58_DDRCtrl);
#define NC_TEE_CLR_DDR_MODE()                   REG_CLR_BITS_UINT16(NC_TEE_DDR_CTRL, BIT_DDR_MASM);
#else
#define NC_TEE_SET_DDR_MODE()
#define NC_TEE_CLR_DDR_MODE()
#endif


#if (defined(DDR_NAND_SUPPORT) && DDR_NAND_SUPPORT)

#if defined(ENABLE_8BIT_MACRO) && ENABLE_8BIT_MACRO
#define DQS_MODE_0P0T		0
#define DQS_MODE_0P5T		1
#define DQS_MODE_1P0T		2
#define DQS_MODE_1P5T		3
#define DQS_MODE_2P0T		4

#define DQS_MODE_TABLE_CNT	5
#define DQS_MODE_SEARCH_TABLE	{DQS_MODE_1P5T, DQS_MODE_2P0T, DQS_MODE_0P5T, DQS_MODE_1P0T, DQS_MODE_0P0T}

#define BIT_DQS_MODE_MASK		(BIT2|BIT1|BIT0)
#define BIT_DQS_MDOE_SHIFT		0
#define BIT_DQS_DELAY_CELL_MASK	(BIT4|BIT5|BIT6|BIT7)
#define BIT_DQS_DELAY_CELL_SHIFT	4

#define SKEW_CLK_PHASE_CNT		16


#elif defined(ENABLE_32BIT_MACRO) && ENABLE_32BIT_MACRO
#define DQS_MODE_TABLE_CNT		16
#define DQS_MODE_SEARCH_TABLE 	{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}

#define SKEW_CLK_PHASE_CNT		4

#endif

#define	NC_ONFI_DEFAULT_TRR                 12
#define	NC_ONFI_DEFAULT_TCS                 7
#define NC_ONFI_DEFAULT_TWW                 8
#define NC_ONFI_DEFAULT_TWHR                5
#define NC_ONFI_DEFAULT_TADL                6
#define NC_ONFI_DEFAULT_TCWAW               4
#define	NC_ONFI_DEFAULT_RX40CMD             4
#define	NC_ONFI_DEFAULT_RX40ADR             7
#define	NC_ONFI_DEFAULT_RX56                10

#define	NC_TOGGLE_DEFAULT_TRR               8
#define	NC_TOGGLE_DEFAULT_TCS               6
#define NC_TOGGLE_DEFAULT_TWW               7
#define	NC_TOGGLE_DEFAULT_TWHR              5
#define	NC_TOGGLE_DEFAULT_TADL              7
#define	NC_TOGGLE_DEFAULT_TCWAW             2
#define	NC_TOGGLE_DEFAULT_RX40CMD           4
#define	NC_TOGGLE_DEFAULT_RX40ADR           5
#define	NC_TOGGLE_DEFAULT_RX56              15
#endif

#define	NC_SDR_DEFAULT_TRR 			7
#define	NC_SDR_DEFAULT_TCS 			6
#define NC_SDR_DEFAULT_TWW			5
#define	NC_SDR_DEFAULT_TWHR 		4
#define	NC_SDR_DEFAULT_TADL 		8
#define	NC_SDR_DEFAULT_TCWAW 		2
#define	NC_SDR_DEFAULT_RX40CMD 		4
#define	NC_SDR_DEFAULT_RX40ADR 		5
#define	NC_SDR_DEFAULT_RX56 		5

#define	NC_INST_DELAY                       1
#define	NC_HWCMD_DELAY                      1
#define	NC_TRR_TCS                          1
#define	NC_TWHR_TCLHZ                       1
#define	NC_TCWAW_TADL                       1


#if IF_IP_VERIFY
/* select a default NAND chip for IP_VERIFY or NAND_PROGRAMMER */
//#define K9GAG08U0E                          1
//#define H27UAG8T2M                          1
//#define NAND512W3A2C                        1
//#define K9F1G08X0C                          1
//#define TH58TVG7D2GBA                       1 // <- Toggle
//#define MT29F64G08CBAAB                     1 // <- ONFI
//#define H27UAG8T2A                          1
//#define H27UF081G2A                         1
//#define K511F12ACA                          1
//#define TY8A0A111162KC40                    1
//#define K522H1GACE                          1
//#define TY890A111229KC40                    1
//#define H8ACU0CE0DAR                        1
//#define H9LA25G25HAMBR                      1
//#define TY8A0A111178KC4                     1
//#define HY27UF082G2B                        1
//#define H27UCG8V5M                          1 //4k page
//#define H27U2G8F2CTR                        1
//#define H27U4G8F2DTR                        1
#define MT29F64G08CBCGB                        1	//3d NAND

#include "drvNAND_device.h"
#endif

#define IF_SPARE_AREA_DMA          0 // [CAUTION] 

#define NAND_PAD_BYPASS_MODE	    1
#define NAND_PAD_TOGGLE_MODE	    2
#define NAND_PAD_ONFI_SYNC_MODE		3

#define MACRO_TYPE_8BIT			1
#define MACRO_TYPE_32BIT			2

#define IF_FCIE_SHARE_PINS          0 // 1: need to nand_pads_switch at HAL's functions.
#define IF_FCIE_SHARE_CLK         0 // 1: need to nand_clock_setting at HAL's functions.
#define IF_FCIE_SHARE_IP          0

#define RANDOM_WR_THR_CIFD			0
#define ENABLE_NAND_INTERRUPT_MODE      0

#define NAND_DRIVER_ROM_VERSION       0 // to save code size
#define AUTO_FORMAT_FTL           0

#define ENABLE_CUS_READ_ENHANCEMENT     0

//FCIE5 Definition

#define SPARE_DMA_ADDR_AUTO_INC 		0 //spare would increase its address when dma

#undef BIT_NC_ECC_CNT_MASK
#undef BIT_NC_ECC_SEL_LOC_MASK
#undef BIT_NC_ECC_SEL_LOC_SHIFT
#define BIT_NC_ECC_CNT_MASK        (BIT8|BIT7|BIT6|BIT5|BIT4|BIT3|BIT2)
#define BIT_NC_ECC_SEL_LOC_MASK    (BIT15|BIT14|BIT13|BIT12|BIT11|BIT10|BIT9)
#define BIT_NC_ECC_SEL_LOC_SHIFT   9

//=====================================================
#define ENABLE_READ_DISTURBANCE_SUPPORT     0
#define DEBUG_REG_ECC_STATUS                NC_RX5E // choose dummy reg from FCIE or CHIPTOP

// 2 bits ECC status
#define ECC_STATUS_NORMAL                   0
#define ECC_STATUS_BITFLIP                  BIT0
#define ECC_STATUS_ERROR                    BIT1

//---------------------------------------------------------------------
//         RESERVED          |   BL   | UBOOT | HASH2 | HASH1 | HASH0 |
//---------------------------------------------------------------------
#define BIT_ECC_STATUS_HASH0_SHIFT          0
#define BIT_ECC_STATUS_HASH1_SHIFT          2
#define BIT_ECC_STATUS_HASH2_SHIFT          4
#define BIT_ECC_STATUS_UBOOT_SHIFT          6
#define BIT_ECC_STATUS_BL_SHIFT             8

#define HASH0_SIZE  (56 * 1024)
#define HASH1_SIZE  (64 * 1024)

#if defined(CONFIG_TEE_LOADER)
#define SBOOT_SIZE  0x20000 //TL:36KB RL:36KB UBoot offset=56K+36K+36K=128K
#else
#define SBOOT_SIZE  0x16000
#endif

extern U32 nand_ReadDisturbance(void);
//=====================================================
#define ENABLE_NAND_POWER_SAVING_MODE       1
#define ENABLE_NAND_POWER_SAVING_DEGLITCH   0

#define __VER_UNFD_FTL__          0
//=====================================================
// Nand Driver configs
//=====================================================
#define NAND_ENV_FPGA             1
#define NAND_ENV_ASIC             2
#ifdef __FPGA_MODE__  // currently G1 don't care this macro, even in ROM code
#define NAND_DRIVER_ENV           NAND_ENV_FPGA
#else
#define NAND_DRIVER_ENV           NAND_ENV_ASIC
#endif

#define UNFD_CACHE_LINE           0x80
//=====================================================
// tool-chain attributes
//=====================================================
#define UNFD_PACK0
#define UNFD_PACK1              __attribute__((__packed__))
#define UNFD_ALIGN0
#define UNFD_ALIGN1             __attribute__((aligned(UNFD_CACHE_LINE)))

//=====================================================
// debug option
//=====================================================
#define NAND_TEST_IN_DESIGN         0      /* [CAUTION] */

#ifndef NAND_DEBUG_MSG
#define NAND_DEBUG_MSG            1
#endif

/* Define trace levels. */
#define UNFD_DEBUG_LEVEL_ERROR        (1)    /* Error condition debug messages. */
#define UNFD_DEBUG_LEVEL_WARNING      (2)    /* Warning condition debug messages. */
#define UNFD_DEBUG_LEVEL_HIGH       (3)    /* Debug messages (high debugging). */
#define UNFD_DEBUG_LEVEL_MEDIUM       (4)    /* Debug messages. */
#define UNFD_DEBUG_LEVEL_LOW        (5)    /* Debug messages (low debugging). */

/* Higer debug level means more verbose */
#ifndef UNFD_DEBUG_LEVEL
#define UNFD_DEBUG_LEVEL          UNFD_DEBUG_LEVEL_WARNING
#endif

extern int Disable_Err_log;
#if defined(NAND_DEBUG_MSG) && NAND_DEBUG_MSG
#define nand_print_tmt            //msPrintfFunc
#define nand_printf            printf
#define nand_debug(dbg_lv, tag, str, ...)       \
  do {                        \
    if (dbg_lv > UNFD_DEBUG_LEVEL || Disable_Err_log == 1)        \
      break;                  \
    else {                    \
	  if (dbg_lv == UNFD_DEBUG_LEVEL_ERROR) \
		nand_printf("NAND Err:");			\
      if (tag)                  \
      {                                       \
        nand_printf("[%s]\t",__func__);        \
        /*nand_print_tmt(__func__);*/       \
      }                                       \
                          \
      nand_printf(str, ##__VA_ARGS__);      \
      /*nand_print_tmt(str, ##__VA_ARGS__);*/     \
    }                     \
  } while(0)
#else /* NAND_DEBUG_MSG */
#define nand_printf(...)
#define nand_debug(enable, tag, str, ...) {}
#endif /* NAND_DEBUG_MSG */

static __inline void nand_assert(int condition)
{
  if (!condition) {

  }
}

#define nand_die() \
  do { \
    nand_printf(__func__); \
    nand_printf("\nUNFD Assert(%d)", __LINE__); \
    nand_assert(0); \
  } while(0);

#define nand_stop() \
  while(1)  nand_reset_WatchDog();

//=====================================================
// HW Timer for Delay
//=====================================================
#define TIMER0_ENABLE						GET_REG_ADDR(TIMER0_REG_BASE_ADDR, 0x20)
#define TIMER0_HIT							GET_REG_ADDR(TIMER0_REG_BASE_ADDR, 0x21)
#define TIMER0_MAX_LOW						GET_REG_ADDR(TIMER0_REG_BASE_ADDR, 0x22)
#define TIMER0_MAX_HIGH						GET_REG_ADDR(TIMER0_REG_BASE_ADDR, 0x23)
#define TIMER0_CAP_LOW						GET_REG_ADDR(TIMER0_REG_BASE_ADDR, 0x24)
#define TIMER0_CAP_HIGH						GET_REG_ADDR(TIMER0_REG_BASE_ADDR, 0x25)

#define HW_TIMER_DELAY_1us          1
#define HW_TIMER_DELAY_10us         10
#define HW_TIMER_DELAY_100us        100
#define HW_TIMER_DELAY_1ms          (1000 * HW_TIMER_DELAY_1us)
#define HW_TIMER_DELAY_5ms          (5    * HW_TIMER_DELAY_1ms)
#define HW_TIMER_DELAY_10ms         (10   * HW_TIMER_DELAY_1ms)
#define HW_TIMER_DELAY_100ms        (100  * HW_TIMER_DELAY_1ms)
#define HW_TIMER_DELAY_500ms        (500  * HW_TIMER_DELAY_1ms)
#define HW_TIMER_DELAY_1s         (1000 * HW_TIMER_DELAY_1ms)

#define NAND_SPEED_TEST             0

extern void delay_us( unsigned us );
extern U32  nand_hw_timer_delay(U32 u32usTick);

//=====================================================
// Pads Switch
//=====================================================
#define REG_NAND_MODE_MASK                  (BIT6|BIT7)
#define NAND_MODE1                          (BIT6)
#define NAND_MODE2							(BIT7)

#define reg_allpad_in           			GET_REG_ADDR(CHIPTOP_REG_BASE_ADDR, 0x50)
#define reg_pcm_pe_23_16           			GET_REG_ADDR(CHIPTOP_REG_BASE_ADDR, 0x0A)
#define reg_emmc_config                     GET_REG_ADDR(CHIPTOP_REG_BASE_ADDR, 0x6E)
#define reg_nf_en                           GET_REG_ADDR(CHIPTOP_REG_BASE_ADDR, 0x6F)

#define reg_emmcpll_rx03		GET_REG_ADDR(EMMC_PLL_REG_BASE_ADDR, 0x03)
#define reg_emmcpll_rx20		GET_REG_ADDR(EMMC_PLL_REG_BASE_ADDR, 0x20)

#define reg_emmcpll_rx1a		GET_REG_ADDR(EMMC_PLL_REG_BASE_ADDR, 0x1A)
#define reg_emmcpll_rx1d		GET_REG_ADDR(EMMC_PLL_REG_BASE_ADDR, 0x1D)
#define reg_emmcpll_rx45        GET_REG_ADDR(EMMC_PLL_REG_BASE_ADDR, 0x45)
#define reg_emmcpll_rx47        GET_REG_ADDR(EMMC_PLL_REG_BASE_ADDR, 0x47)
#define reg_emmcpll_rx48        GET_REG_ADDR(EMMC_PLL_REG_BASE_ADDR, 0x48)
#define reg_emmcpll_rx49        GET_REG_ADDR(EMMC_PLL_REG_BASE_ADDR, 0x49)
#define reg_emmcpll_rx4a        GET_REG_ADDR(EMMC_PLL_REG_BASE_ADDR, 0x4a)

#define reg_emmcpll_rx60		GET_REG_ADDR(EMMC_PLL_REG_BASE_ADDR, 0x60)
#define reg_emmcpll_rx61		GET_REG_ADDR(EMMC_PLL_REG_BASE_ADDR, 0x61)
#define reg_emmcpll_rx62		GET_REG_ADDR(EMMC_PLL_REG_BASE_ADDR, 0x62)
#define reg_emmcpll_rx63		GET_REG_ADDR(EMMC_PLL_REG_BASE_ADDR, 0x63)
#define reg_emmcpll_rx64		GET_REG_ADDR(EMMC_PLL_REG_BASE_ADDR, 0x64)
#define reg_emmcpll_rx69		GET_REG_ADDR(EMMC_PLL_REG_BASE_ADDR, 0x69)
#define reg_emmcpll_rx6c		GET_REG_ADDR(EMMC_PLL_REG_BASE_ADDR, 0x6c)
#define reg_emmcpll_rx6d		GET_REG_ADDR(EMMC_PLL_REG_BASE_ADDR, 0x6d)
#define reg_emmcpll_rx6f		GET_REG_ADDR(EMMC_PLL_REG_BASE_ADDR, 0x6f)

#define reg_emmcpll_rx70		GET_REG_ADDR(EMMC_PLL_REG_BASE_ADDR, 0x70)
#define reg_emmcpll_rx71		GET_REG_ADDR(EMMC_PLL_REG_BASE_ADDR, 0x71)
#define reg_emmcpll_rx72		GET_REG_ADDR(EMMC_PLL_REG_BASE_ADDR, 0x72)
#define reg_emmcpll_rx73		GET_REG_ADDR(EMMC_PLL_REG_BASE_ADDR, 0x73)
#define reg_emmcpll_rx74		GET_REG_ADDR(EMMC_PLL_REG_BASE_ADDR, 0x74)
#define reg_emmcpll_rx7f		GET_REG_ADDR(EMMC_PLL_REG_BASE_ADDR, 0x7f)


#define REG_NAND_DQS_UL_PE                  reg_emmcpll_rx47
#define REG_NAND_DQS_PS                     reg_emmcpll_rx48
#define BIT_DQS_PULL_H                      BIT8
#define NC_DQS_PULL_H()                     {REG_SET_BITS_UINT16(REG_NAND_DQS_UL_PE, BIT_DQS_PULL_H); \
                                            REG_SET_BITS_UINT16(REG_NAND_DQS_PS, BIT_DQS_PULL_H);}
#define NC_DQS_PULL_L()                     {REG_SET_BITS_UINT16(REG_NAND_DQS_UL_PE, BIT_DQS_PULL_H); \
                                            REG_CLR_BITS_UINT16(REG_NAND_DQS_PS, BIT_DQS_PULL_H);}

extern U32 nand_pads_switch(U32 u32EnableFCIE);
extern U32 	nand_check_DDR_pad(void);

//================================================================================
// Set TEE Secure Range
//================================================================================

#define FCIE_WRITE_PROTECT_SUPPORT		0

#if defined(FCIE_WRITE_PROTECT_SUPPORT) && FCIE_WRITE_PROTECT_SUPPORT
#define REG_BANK1618_ADDR					(RIU_PM_BASE+(0x161800<<1))

#define REG_FCIE_WP_START_ROW_ADDR_L	GET_REG_ADDR(REG_BANK1618_ADDR, 0x60)
#define REG_FCIE_WP_START_ROW_ADDR_H	GET_REG_ADDR(REG_BANK1618_ADDR, 0x61)
#define REG_FCIE_WP_END_ROW_ADDR_L		GET_REG_ADDR(REG_BANK1618_ADDR, 0x62)
#define REG_FCIE_WP_END_ROW_ADDR_H		GET_REG_ADDR(REG_BANK1618_ADDR, 0x63)

#define REG_FCIE_WP_ENABLE				GET_REG_ADDR(REG_BANK1618_ADDR, 0x64)
#define BIT_FCIE_WP_ENABLE				BIT0

#endif

//=====================================================
// set FCIE clock
//=====================================================
#define REG_EMMC_PLL_RX01                   GET_REG_ADDR(EMMC_PLL_REG_BASE_ADDR, 0x01)
#define REG_EMMC_PLL_RX02                   GET_REG_ADDR(EMMC_PLL_REG_BASE_ADDR, 0x02)
#define REG_EMMC_PLL_RX03                   GET_REG_ADDR(EMMC_PLL_REG_BASE_ADDR, 0x03)
#define REG_EMMC_PLL_RX04                   GET_REG_ADDR(EMMC_PLL_REG_BASE_ADDR, 0x04)
#define REG_EMMC_PLL_RX05                   GET_REG_ADDR(EMMC_PLL_REG_BASE_ADDR, 0x05)
#define REG_EMMC_PLL_RX06                   GET_REG_ADDR(EMMC_PLL_REG_BASE_ADDR, 0x06)
#define REG_EMMC_PLL_RX07                   GET_REG_ADDR(EMMC_PLL_REG_BASE_ADDR, 0x07)

#define REG_EMMC_PLL_RX09                   GET_REG_ADDR(EMMC_PLL_REG_BASE_ADDR, 0x09)
#define BIT_EMMC_RXDLL_PHASE_SEL_MASK       (BIT7|BIT6|BIT5|BIT4)
#define BIT_EMMC_RXDLL_PHASE_SEL_SHIFT      4

#define ENABLE_DELAY_CELL					0

#if defined(ENABLE_DELAY_CELL) && ENABLE_DELAY_CELL
#undef DQS_MODE_SEARCH_TABLE
#define DQS_MODE_SEARCH_TABLE               {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}
#endif

#define BIT_EMMC_RXDELAY_CELL_MASK          (BIT7|BIT6|BIT5|BIT4)
#define BIT_EMMC_RXDELAY_CELL_SHIFT         4

#define REG_EMMC_PLL_RX18                   GET_REG_ADDR(EMMC_PLL_REG_BASE_ADDR, 0x18)
#define REG_EMMC_PLL_RX19                   GET_REG_ADDR(EMMC_PLL_REG_BASE_ADDR, 0x19)
#define REG_EMMC_PLL_RX1B                   GET_REG_ADDR(EMMC_PLL_REG_BASE_ADDR, 0x1B)
#define REG_EMMC_PLL_RX30                   GET_REG_ADDR(EMMC_PLL_REG_BASE_ADDR, 0x30)
#define REG_EMMC_PLL_RX32                   GET_REG_ADDR(EMMC_PLL_REG_BASE_ADDR, 0x32)
#define REG_EMMC_PLL_RX33                   GET_REG_ADDR(EMMC_PLL_REG_BASE_ADDR, 0x33)
#define REG_EMMC_PLL_RX34                   GET_REG_ADDR(EMMC_PLL_REG_BASE_ADDR, 0x34)


#define DECIDE_CLOCK_BY_NAND			1

#define NFIE_CLK_12M                         (0<<2)
#define NFIE_CLK_20M                        (1<<2)
#define NFIE_CLK_32M                      (2<<2)
#define NFIE_CLK_36M                        (3<<2)
#define NFIE_CLK_40M                      (4<<2)
#define NFIE_CLK_43M                        (5<<2)
#define NFIE_CLK_54M                        (6<<2)
#define NFIE_CLK_62M                        (7<<2)
#define NFIE_CLK_72M                        (8<<2)
#define NFIE_CLK_86M                        (9<<2)
#define NFIE_CLK_1X_P_SDR                        (11<<2)
#define NFIE_CLK_2X_P_DDR                     	(12<<2)
#define NFIE_CLK_300K                     	(13<<2)
#define NFIE_CLK_48M                        (15<<2)

#define NFIE_CLK_100M                        (16<<2)
#define NFIE_CLK_120M                        (17<<2)
#define NFIE_CLK_140M                        (18<<2)
#define NFIE_CLK_160M                        (19<<2)
#define NFIE_CLK_200M                        (20<<2)
#define NFIE_CLK_250M                        (21<<2)
#define NFIE_CLK_266M                        (22<<2)

#define NFIE_CLK_TABLE_CNT	11
#define NFIE_CLK_TABLE	{	 NFIE_CLK_12M, NFIE_CLK_20M, NFIE_CLK_32M, \
							NFIE_CLK_36M, NFIE_CLK_40M, NFIE_CLK_43M, NFIE_CLK_48M, \
							NFIE_CLK_54M, NFIE_CLK_62M, NFIE_CLK_72M, NFIE_CLK_86M }

#define NFIE_CLK_TABLE_STR	{	"12M", "20M", "32M",\
							"36M", "40M", "43M", "48M", \
							"54M", "62M", "72M", "86M" }

#define NFIE_12M_VALUE		12000000
#define NFIE_20M_VALUE		20000000
#define NFIE_32M_VALUE		32000000
#define NFIE_36M_VALUE		36000000
#define NFIE_40M_VALUE		40000000
#define NFIE_43M_VALUE		43000000
#define NFIE_48M_VALUE		48000000
#define NFIE_54M_VALUE		54000000
#define NFIE_62M_VALUE		62000000
#define NFIE_72M_VALUE		72000000
#define NFIE_86M_VALUE		86000000
#define NFIE_100M_VALUE		100000000
#define NFIE_120M_VALUE		120000000
#define NFIE_140M_VALUE		140000000
#define NFIE_160M_VALUE		160000000
#define NFIE_200M_VALUE		200000000
#define NFIE_250M_VALUE		250000000
#define NFIE_266M_VALUE		266000000

#define NFIE_CLK_VALUE_TABLE	{	NFIE_12M_VALUE, NFIE_20M_VALUE, NFIE_32M_VALUE, NFIE_36M_VALUE, \
							NFIE_40M_VALUE, NFIE_43M_VALUE, NFIE_48M_VALUE, NFIE_54M_VALUE, \
							NFIE_62M_VALUE, NFIE_72M_VALUE, NFIE_86M_VALUE }


/*Define 1 cycle Time for each clock note: define value must be the (real value -1)*/
#define NFIE_1T_12M			83
#define NFIE_1T_20M			50
#define NFIE_1T_32M			30
#define NFIE_1T_36M			27
#define NFIE_1T_40M			25
#define NFIE_1T_43M			23
#define NFIE_1T_48M			20
#define NFIE_1T_54M			18
#define NFIE_1T_62M			16
#define NFIE_1T_72M			13
#define NFIE_1T_86M			11
#define NFIE_1T_100M		10
#define NFIE_1T_120M		8
#define NFIE_1T_140M		7
#define NFIE_1T_160M		6
#define NFIE_1T_200M		5
#define NFIE_1T_250M		4
#define NFIE_1T_266M		3

#define NFIE_1T_TABLE		{	NFIE_1T_12M, NFIE_1T_20M, NFIE_1T_32M, \
							NFIE_1T_36M, NFIE_1T_40M, NFIE_1T_43M, NFIE_1T_48M, \
							NFIE_1T_54M, NFIE_1T_62M, NFIE_1T_72M, NFIE_1T_86M }


#define NFIE_4CLK_TABLE_CNT	18

#define EMMC_PLL_1XCLK_TABLE_CNT		NFIE_4CLK_TABLE_CNT

#define EMMC_PLL_1XCLK_RX05_MASK            (BIT2|BIT1|BIT0)

#define EMMC_PLL_1XCLK_12M_IDX              0
#define EMMC_PLL_1XCLK_12M_RX05             7
#define EMMC_PLL_1XCLK_12M_RX18             (0x0000)
#define EMMC_PLL_1XCLK_12M_RX19             (0x0048)

#define EMMC_PLL_1XCLK_20M_IDX              1
#define EMMC_PLL_1XCLK_20M_RX05             7
#define EMMC_PLL_1XCLK_20M_RX18             (0x3333)
#define EMMC_PLL_1XCLK_20M_RX19             (0x002B)

#define EMMC_PLL_1XCLK_32M_IDX              2
#define EMMC_PLL_1XCLK_32M_RX05             4
#define EMMC_PLL_1XCLK_32M_RX18             (0x0000)
#define EMMC_PLL_1XCLK_32M_RX19             (0x0036)

#define EMMC_PLL_1XCLK_36M_IDX              3
#define EMMC_PLL_1XCLK_36M_RX05             4
#define EMMC_PLL_1XCLK_36M_RX18             (0x0000)
#define EMMC_PLL_1XCLK_36M_RX19             (0x0030)

#define EMMC_PLL_1XCLK_40M_IDX              4
#define EMMC_PLL_1XCLK_40M_RX05             4
#define EMMC_PLL_1XCLK_40M_RX18             (0x3333)
#define EMMC_PLL_1XCLK_40M_RX19             (0x002B)

#define EMMC_PLL_1XCLK_43M_IDX              5
#define EMMC_PLL_1XCLK_43M_RX05             4
#define EMMC_PLL_1XCLK_43M_RX18             (0x2FA0)
#define EMMC_PLL_1XCLK_43M_RX19             (0x0028)

#define EMMC_PLL_1XCLK_48M_IDX              6
#define EMMC_PLL_1XCLK_48M_RX05             2
#define EMMC_PLL_1XCLK_48M_RX18             (0x0000)
#define EMMC_PLL_1XCLK_48M_RX19             (0x0048)

#define EMMC_PLL_1XCLK_54M_IDX              7
#define EMMC_PLL_1XCLK_54M_RX05             2
#define EMMC_PLL_1XCLK_54M_RX18             (0x0000)
#define EMMC_PLL_1XCLK_54M_RX19             (0x0040)

#define EMMC_PLL_1XCLK_62M_IDX              8
#define EMMC_PLL_1XCLK_62M_RX05             2
#define EMMC_PLL_1XCLK_62M_RX18             (0xBDEF)
#define EMMC_PLL_1XCLK_62M_RX19             (0x0037)

#define EMMC_PLL_1XCLK_72M_IDX              9
#define EMMC_PLL_1XCLK_72M_RX05             2
#define EMMC_PLL_1XCLK_72M_RX18             (0x0000)
#define EMMC_PLL_1XCLK_72M_RX19             (0x0030)

#define EMMC_PLL_1XCLK_86M_IDX              10
#define EMMC_PLL_1XCLK_86M_RX05             2
#define EMMC_PLL_1XCLK_86M_RX18             (0x2FA0)
#define EMMC_PLL_1XCLK_86M_RX19             (0x0028)

#define EMMC_PLL_1XCLK_100M_IDX              11
#define EMMC_PLL_1XCLK_100M_RX05             1
#define EMMC_PLL_1XCLK_100M_RX18             (0x1EB8)
#define EMMC_PLL_1XCLK_100M_RX19             (0x0045)

#define EMMC_PLL_1XCLK_120M_IDX              12
#define EMMC_PLL_1XCLK_120M_RX05             1
#define EMMC_PLL_1XCLK_120M_RX18             (0x9999)
#define EMMC_PLL_1XCLK_120M_RX19             (0x0039)

#define EMMC_PLL_1XCLK_140M_IDX              13
#define EMMC_PLL_1XCLK_140M_RX05             1
#define EMMC_PLL_1XCLK_140M_RX18             (0x5F15)
#define EMMC_PLL_1XCLK_140M_RX19             (0x0031)

#define EMMC_PLL_1XCLK_160M_IDX              14
#define EMMC_PLL_1XCLK_160M_RX05             1
#define EMMC_PLL_1XCLK_160M_RX18             (0x3333)
#define EMMC_PLL_1XCLK_160M_RX19             (0x002B)

#define EMMC_PLL_1XCLK_200M_IDX              15
#define EMMC_PLL_1XCLK_200M_RX05             1
#define EMMC_PLL_1XCLK_200M_RX18             (0x8F5C)
#define EMMC_PLL_1XCLK_200M_RX19             (0x0022)

#define EMMC_PLL_1XCLK_250M_IDX              16
#define EMMC_PLL_1XCLK_250M_RX05             1
#define EMMC_PLL_1XCLK_250M_RX18             (0xA5E3)	//250
#define EMMC_PLL_1XCLK_250M_RX19             (0x001B)

#define EMMC_PLL_1XCLK_266M_IDX              17
#define EMMC_PLL_1XCLK_266M_RX05             1
#define EMMC_PLL_1XCLK_266M_RX18             (0xFC26)	//266
#define EMMC_PLL_1XCLK_266M_RX19             (0x0019)


#define NFIE_4CLK_TABLE	{	 NFIE_CLK_12M, NFIE_CLK_20M, NFIE_CLK_32M, \
							NFIE_CLK_36M, NFIE_CLK_40M, NFIE_CLK_43M, NFIE_CLK_48M, \
							NFIE_CLK_54M, NFIE_CLK_62M, NFIE_CLK_72M, NFIE_CLK_86M, NFIE_CLK_100M, \
							NFIE_CLK_120M, NFIE_CLK_140M, NFIE_CLK_160M, NFIE_CLK_200M, NFIE_CLK_250M, NFIE_CLK_266M }

#define NFIE_4CLK_TABLE_STR	{	"12M", "20M", "32M", "36M",\
							"40M", "43", "48M", "54M", \
							"62M", "72M", "86M", "100M", "120M", "140M", "160M", "200M", "250M", "266M" }

#define NFIE_4CLK_VALUE_TABLE	{	NFIE_12M_VALUE, NFIE_20M_VALUE, NFIE_32M_VALUE, NFIE_36M_VALUE, NFIE_40M_VALUE, \
							NFIE_43M_VALUE, NFIE_48M_VALUE, NFIE_54M_VALUE, NFIE_62M_VALUE, \
							NFIE_72M_VALUE, NFIE_86M_VALUE, NFIE_100M_VALUE, NFIE_120M_VALUE, NFIE_140M_VALUE, \
							NFIE_160M_VALUE, NFIE_200M_VALUE, NFIE_250M_VALUE, NFIE_266M_VALUE}	

#define NFIE_4CLK_1T_TABLE		{	NFIE_1T_12M, NFIE_1T_20M, NFIE_1T_32M, \
							NFIE_1T_36M, NFIE_1T_40M, NFIE_1T_43M, NFIE_1T_48M, \
							NFIE_1T_54M, NFIE_1T_62M, NFIE_1T_72M, NFIE_1T_86M, \
							NFIE_1T_100M, NFIE_1T_120M, NFIE_1T_140M, NFIE_1T_160M, \
							NFIE_1T_200M, NFIE_1T_250M ,NFIE_1T_266M}

typedef struct _EMMC_PLL_SETTINGS
{
    U16 emmc_pll_1xclk_rx05;
    U16 emmc_pll_1xclk_rx18;
    U16 emmc_pll_1xclk_rx19;
} EMMC_PLL_SETTINGS;

#define EMMC_PLL_CLK_TABLE {\
  {EMMC_PLL_1XCLK_12M_RX05, EMMC_PLL_1XCLK_12M_RX18, EMMC_PLL_1XCLK_12M_RX19},    \
  {EMMC_PLL_1XCLK_20M_RX05, EMMC_PLL_1XCLK_20M_RX18, EMMC_PLL_1XCLK_20M_RX19},    \
  {EMMC_PLL_1XCLK_32M_RX05, EMMC_PLL_1XCLK_32M_RX18, EMMC_PLL_1XCLK_32M_RX19},    \
  {EMMC_PLL_1XCLK_36M_RX05, EMMC_PLL_1XCLK_36M_RX18, EMMC_PLL_1XCLK_36M_RX19},    \
  {EMMC_PLL_1XCLK_40M_RX05, EMMC_PLL_1XCLK_40M_RX18, EMMC_PLL_1XCLK_40M_RX19},    \
  {EMMC_PLL_1XCLK_43M_RX05, EMMC_PLL_1XCLK_43M_RX18, EMMC_PLL_1XCLK_43M_RX19},    \
  {EMMC_PLL_1XCLK_48M_RX05, EMMC_PLL_1XCLK_48M_RX18, EMMC_PLL_1XCLK_48M_RX19},    \
  {EMMC_PLL_1XCLK_54M_RX05, EMMC_PLL_1XCLK_54M_RX18, EMMC_PLL_1XCLK_54M_RX19},    \
  {EMMC_PLL_1XCLK_62M_RX05, EMMC_PLL_1XCLK_62M_RX18, EMMC_PLL_1XCLK_62M_RX19},    \
  {EMMC_PLL_1XCLK_72M_RX05, EMMC_PLL_1XCLK_72M_RX18, EMMC_PLL_1XCLK_72M_RX19},    \
  {EMMC_PLL_1XCLK_86M_RX05, EMMC_PLL_1XCLK_86M_RX18, EMMC_PLL_1XCLK_86M_RX19},    \
  {EMMC_PLL_1XCLK_100M_RX05, EMMC_PLL_1XCLK_100M_RX18, EMMC_PLL_1XCLK_100M_RX19},    \
  {EMMC_PLL_1XCLK_120M_RX05, EMMC_PLL_1XCLK_120M_RX18, EMMC_PLL_1XCLK_120M_RX19},    \
  {EMMC_PLL_1XCLK_140M_RX05, EMMC_PLL_1XCLK_140M_RX18, EMMC_PLL_1XCLK_140M_RX19},    \
  {EMMC_PLL_1XCLK_160M_RX05, EMMC_PLL_1XCLK_160M_RX18, EMMC_PLL_1XCLK_160M_RX19},    \
  {EMMC_PLL_1XCLK_200M_RX05, EMMC_PLL_1XCLK_200M_RX18, EMMC_PLL_1XCLK_200M_RX19},    \
  {EMMC_PLL_1XCLK_250M_RX05, EMMC_PLL_1XCLK_250M_RX18, EMMC_PLL_1XCLK_250M_RX19},    \
  {EMMC_PLL_1XCLK_266M_RX05, EMMC_PLL_1XCLK_266M_RX18, EMMC_PLL_1XCLK_266M_RX19},    \
}


#define DUTY_CYCLE_PATCH          0 // 1: to enlarge low width for tREA's worst case of 25ns
#if DUTY_CYCLE_PATCH
#define FCIE3_SW_DEFAULT_CLK        NFIE_CLK_86M
#define FCIE_REG41_VAL            ((2<<9)|(2<<3)) // RE,WR pulse, Low:High=3:1
#define REG57_ECO_FIX_INIT_VALUE      0
#else
#define FCIE3_SW_DEFAULT_CLK        NFIE_CLK_54M
#define FCIE_REG41_VAL            0               // RE,WR pulse, Low:High=1:1
#define REG57_ECO_FIX_INIT_VALUE            BIT_NC_LATCH_DATA_2_0_T|BIT_NC_LATCH_STS_1_0_T
#endif
#define FCIE3_SW_SLOWEST_CLK                NFIE_CLK_12M

#define NAND_SEQ_ACC_TIME_TOL       16 //in unit of ns

#define reg_ckg_fcie				GET_REG_ADDR(MPLL_CLK_REG_BASE_ADDR, 0x64)
#define BIT_CLK_ENABLE				BIT6

//=====================================================
// set ECC clock
//=====================================================
#define ECC_CLK_GATING              (BIT0)
#define ECC_CLK_INVERSE             (BIT1)
#define ECC_CLK_MASK                (BIT4|BIT3|BIT2)
#define ECC_CLK_12M                 (0<<2)
#define ECC_CLK_54M                 (1<<2)
#define ECC_CLK_108M                (2<<2)
#define ECC_CLK_160M                (3<<2)
#define ECC_CLK_216M                (4<<2)
#define ECC_SW_DEFAULT_CLK          ECC_CLK_216M
#define reg_ckg_ecc                 GET_REG_ADDR(MPLL_CLK_REG_BASE_ADDR, 0x67)
//=====================================================
// set fcie sync clock
//=====================================================
#define NFIE_CLK_SYNC_MASK          (BIT1|BIT0)
#define NFIE_CLK_SYNC_216M          (0<<0)
#define NFIE_CLK_SYNC_432M          (1<<0)
#define NFIE_CLK_SYNC_DEFAULT       NFIE_CLK_SYNC_432M
#define reg_ckg_fcie_syn			GET_REG_ADDR(MPLL_CLK2_REG_BASE_ADDR, 0x0C)
//=====================================================

extern U32  nand_clock_setting(U32 u32ClkParam);

extern void nand_DumpPadClk(void);

//=====================================================
// transfer DMA Address
//=====================================================
/*
 * Important:
 * The following buffers should be large enough for a whole NAND block
 */
// FIXME, this is only for verifing IP
#define DMA_W_ADDR                          0x21000000
#define DMA_R_ADDR                          0x21400000
#define DMA_W_SPARE_ADDR                    0x21800000
#define DMA_R_SPARE_ADDR                    0x21900000
#define DMA_BAD_BLK_BUF                     0x21A00000

#define DMA_W_SRAM_ADDR                     0xFDC00000
#define DMA_R_SRAM_ADDR                     0xFDC00800
#define DMA_W_SRAM_SPARE_ADDR               0xFDC01000
#define DMA_R_SRAM_SPARE_ADDR               0xFDC01800


#define MIU_CHECK_LAST_DONE         1

//=====================================================
// misc
//=====================================================
#define NC_REG_MIU_LAST_DONE        NC_MIE_EVENT

//extern void flush_cache(U32 start_addr, U32 size);
extern void Chip_Flush_Memory(void);
extern void Chip_Read_Memory(void);

extern int memcmp(const void *,const void *, unsigned int);
extern void *malloc(unsigned int);
extern void free(void*);

int printf(const char *format, ...);

extern U32 nand_Crazy_WriteOnly(U16 *u16_pattern_test, U16 u16_pattern_cnt);
extern U32 nand_tortureblk(U16 u16_PBA, U32 u32_loop);
extern U32 nand_skew1_skew2_scanning(U8 u8_Skew1);
extern U32 nand_skew1_RXDLL(U8 u8_mode);


#define NANDINFO_ECC_TYPE ECC_TYPE_72BIT1KB

#endif //__UNFD_M7322_UBOOT_H__

