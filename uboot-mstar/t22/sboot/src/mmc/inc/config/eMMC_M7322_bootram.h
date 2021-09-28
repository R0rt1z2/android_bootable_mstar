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
#ifndef __eMMC_M7322_UBOOT__
#define __eMMC_M7322_UBOOT__

//#include <common.h>
//#include <malloc.h>
//#include <linux/string.h>

//#include <config.h>
//#include <command.h>
//#include <mmc.h>
//#include <part.h>
//#include <malloc.h>
//#include <asm/errno.h>

#include "../../../../inc/M7322/board/Board.h"

#ifndef U32
#define U32  unsigned int
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
#endif


//=====================================================
// HW registers
//=====================================================
#define REG_OFFSET_SHIFT_BITS               2

#define REG_FCIE_U16(Reg_Addr)              (*(volatile U16*)(Reg_Addr))
#define GET_REG_ADDR(x, y)                  ((x)+((y) << REG_OFFSET_SHIFT_BITS))

#define REG_FCIE(reg_addr)                  REG_FCIE_U16(reg_addr)
#define REG_FCIE_W(reg_addr, val)           REG_FCIE(reg_addr) = (val)
#define REG_FCIE_R(reg_addr, val)           val = REG_FCIE(reg_addr)
#define REG_FCIE_SETBIT(reg_addr, val)      REG_FCIE(reg_addr) |= (val)
#define REG_FCIE_CLRBIT(reg_addr, val)      REG_FCIE(reg_addr) &= ~(val)
#define REG_FCIE_W1C(reg_addr, val)         REG_FCIE_W(reg_addr, REG_FCIE(reg_addr)&(val))

#define CONFIG_SRAM_BASE_ADDRESS            0x1FC00000
#define CONFIG_SRAM_FCIE_OFFSET             0x2C00
//------------------------------
#define RIU_BASE                            0x1F200000
#define CONFIG_RIU_BASE_ADDRESS             0x1F000000

#define REG_BANK_TIMER0                     0x1800
#define REG_BANK_FCIE0                      0x8980
#define REG_BANK_FCIE1                      (REG_BANK_FCIE0 + 0x20)
#define REG_BANK_FCIE2                      (REG_BANK_FCIE0 + 0x80)

#define FCIE0_BASE                          GET_REG_ADDR(RIU_BASE, REG_BANK_FCIE0)
#define FCIE1_BASE                          GET_REG_ADDR(RIU_BASE, REG_BANK_FCIE1)
#define FCIE2_BASE                          GET_REG_ADDR(RIU_BASE, REG_BANK_FCIE2)
#define TIMER0_REG_BASE_ADDR                GET_REG_ADDR(CONFIG_RIU_BASE_ADDRESS, REG_BANK_TIMER0)

#define FCIE_REG_BASE_ADDR                  FCIE0_BASE
#define FCIE_CMDFIFO_BASE_ADDR              FCIE1_BASE
#define FCIE_CIFD_BASE_ADDR                 FCIE2_BASE

#define FCIE_NC_WBUF_CIFD_BASE              GET_REG_ADDR(RIU_BASE, REG_BANK_FCIE2)
#define FCIE_NC_RBUF_CIFD_BASE              GET_REG_ADDR(RIU_BASE, REG_BANK_FCIE2 + 0x20)

#define REG_DEBUG                           (0x1F000000 + (0x103380 << 1)) //0x1F206700

/*#define DEBUG_REG_1                       GET_REG_ADDR(PORTSTATUS_BASE, 0x01)
#define DEBUG_REG_2                         GET_REG_ADDR(PORTSTATUS_BASE, 0x02)
#define DEBUG_REG_3                         GET_REG_ADDR(PORTSTATUS_BASE, 0x03)
*/

#include "eMMC_reg_v5.h"
// UART //(0x1F201300)
#define REG_BANK_UART     0x480

#define REG_UART_BASE     GET_REG_ADDR(RIU_BASE, REG_BANK_UART)
#define UART_LSR_THRE     (0x20)
#define UART_LSR_TEMT     (0x40)
#define REG_UART_TX       GET_REG_ADDR(REG_UART_BASE, 0x40)
#define REG_UART_LSR      GET_REG_ADDR(REG_UART_BASE, 0x4A)


//--------------------------------clock gen------------------------------------

#define REG_BANK_CLKGEN                     0x580
#define CLKGEN_BASE                         GET_REG_ADDR(RIU_BASE, REG_BANK_CLKGEN)


#define reg_ckg_fcie                        GET_REG_ADDR(CLKGEN_BASE, 0x64)
#define BIT_FCIE_CLK_Gate                   BIT0
#define BIT_FCIE_CLK_Inverse                BIT1
#define BIT_FCIE_CLK_MASK                   (BIT5|BIT4|BIT3|BIT2)
#define BIT_FCIE_CLK_SRC_SEL                BIT6
#define BIT_FCIE_CLK_SHIFT                  2

#define reg_ckg_miu_fcie_sel               GET_REG_ADDR(CLKGEN_BASE, 0x64)
#define BIT_CKG_MIU_FCIE_SEL               BIT7

#define REG_BANK_CLKGEN2                   0x0500
#define CLKGEN2_BASE                       GET_REG_ADDR(RIU_BASE, REG_BANK_CLKGEN2)
#define reg_ckg_fcie_syn                   GET_REG_ADDR(CLKGEN2_BASE, 0x0C)
#define BIT_CKG_FCIE_SYN_MASK           (BIT1|BIT0)
#define BIT_CKG_FCIE_SYN                BIT0


#define BIT_FCIE_CLK_12M                0x0
#define BIT_FCIE_CLK_20M                0x1
#define BIT_FCIE_CLK_32M                0x2
#define BIT_FCIE_CLK_36M                0x3
#define BIT_FCIE_CLK_40M                0x4
#define BIT_FCIE_CLK_43_2M              0x5
#define BIT_FCIE_CLK_54M                0x6
#define BIT_FCIE_CLK_62M                0x7
#define BIT_FCIE_CLK_72M                0x8
#define BIT_FCIE_CLK_86M                0x9
#define BIT_FCIE_CLK_EMMC_PLL_1X            0xB
#define BIT_FCIE_CLK_EMMC_PLL_2X            0xC
#define BIT_FCIE_CLK_300K               0xD
#define BIT_CLK_XTAL_24M                0xE
#define BIT_FCIE_CLK_48M                0xF

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


//--------------------------------chiptop--------------------------------------

#define REG_BANK_CHIPTOP                0x0F00  // (0x101E - 0x1000) x 80h
#define PAD_CHIPTOP_BASE                GET_REG_ADDR(RIU_BASE, REG_BANK_CHIPTOP)

#define reg_chiptop_0x4F                GET_REG_ADDR(PAD_CHIPTOP_BASE, 0x4F)
#define BIT_reg_emmc_rstz_en            BIT2

#define reg_chiptop_0x50                GET_REG_ADDR(PAD_CHIPTOP_BASE, 0x50)
#define BIT_ALL_PAD_IN                  BIT15

#define reg_chiptop_0x6E                GET_REG_ADDR(PAD_CHIPTOP_BASE, 0x6E)
#define BIT_EMMC_CONFIG_MSK             (BIT7|BIT6)
#define BIT_EMMC_CONFIG_EMMC_MODE_1     BIT6

#define reg_chiptop_0x6F                GET_REG_ADDR(PAD_CHIPTOP_BASE, 0x6F)
#define BIT_NAND_MODE                   (BIT7|BIT6)


//--------------------------------emmc pll--------------------------------------

#define REG_BANK_EMMC_PLL                   0x11F80 // (0x123F - 0x1000) x 80h
#define EMMC_PLL_BASE                       GET_REG_ADDR(RIU_BASE, REG_BANK_EMMC_PLL)

#define REG_EMMC_PLL_RX01                   GET_REG_ADDR(EMMC_PLL_BASE, 0x01)

#define reg_emmcpll_fbdiv                   GET_REG_ADDR(EMMC_PLL_BASE, 0x04)
#define reg_emmcpll_pdiv                    GET_REG_ADDR(EMMC_PLL_BASE, 0x05)
#define reg_emmc_pll_reset                  GET_REG_ADDR(EMMC_PLL_BASE, 0x06)
#define reg_emmc_pll_test                   GET_REG_ADDR(EMMC_PLL_BASE, 0x07)

#define reg_ddfset_15_00                    GET_REG_ADDR(EMMC_PLL_BASE, 0x18)
#define reg_ddfset_23_16                    GET_REG_ADDR(EMMC_PLL_BASE, 0x19)

#define reg_emmcpll_0x02                    GET_REG_ADDR(EMMC_PLL_BASE, 0x02)
#define reg_emmcpll_0x03                    GET_REG_ADDR(EMMC_PLL_BASE, 0x03)
#define BIT_SKEW1_MASK                      (BIT3|BIT2|BIT1|BIT0)
#define BIT_CLK_PH_MASK                      BIT_SKEW1_MASK
#define BIT_SKEW2_MASK                      (BIT7|BIT6|BIT5|BIT4)
#define BIT_SKEW3_MASK                      (BIT11|BIT10|BIT9|BIT8)
#define BIT_SKEW4_MASK                      (BIT15|BIT14|BIT13|BIT12)
#define BIT_DEFAULT_SKEW2                   (BIT6|BIT4)         //5

#define reg_emmcpll_0x04                    GET_REG_ADDR(EMMC_PLL_BASE, 0x04)
#define reg_emmcpll_0x09                    GET_REG_ADDR(EMMC_PLL_BASE, 0x09)
#define reg_emmc_test                       GET_REG_ADDR(EMMC_PLL_BASE, 0x1A)
#define reg_emmcpll_0x1a                    GET_REG_ADDR(EMMC_PLL_BASE, 0x1A)
#define reg_emmcpll_0x1b                    GET_REG_ADDR(EMMC_PLL_BASE, 0x1B)
#define reg_emmcpll_0x1c                    GET_REG_ADDR(EMMC_PLL_BASE, 0x1C)
#define reg_emmcpll_0x1d                    GET_REG_ADDR(EMMC_PLL_BASE, 0x1D)
#define reg_emmcpll_0x20                    GET_REG_ADDR(EMMC_PLL_BASE, 0x20)

#define REG_EMMC_PLL_RX30                   GET_REG_ADDR(EMMC_PLL_BASE, 0x30)
#define REG_EMMC_PLL_RX32                   GET_REG_ADDR(EMMC_PLL_BASE, 0x32)
#define REG_EMMC_PLL_RX33                   GET_REG_ADDR(EMMC_PLL_BASE, 0x33)
#define REG_EMMC_PLL_RX34                   GET_REG_ADDR(EMMC_PLL_BASE, 0x34)

#define reg_emmcpll_0x45                    GET_REG_ADDR(EMMC_PLL_BASE, 0x45)
#define reg_emmcpll_0x63                    GET_REG_ADDR(EMMC_PLL_BASE, 0x63)
#define reg_emmcpll_0x68                    GET_REG_ADDR(EMMC_PLL_BASE, 0x68)
#define reg_emmcpll_0x69                    GET_REG_ADDR(EMMC_PLL_BASE, 0x69)
#define reg_emmcpll_0x6a                    GET_REG_ADDR(EMMC_PLL_BASE, 0x6A)
#define reg_emmcpll_0x6b                    GET_REG_ADDR(EMMC_PLL_BASE, 0x6B)
#define reg_emmcpll_0x6c                    GET_REG_ADDR(EMMC_PLL_BASE, 0x6C)
#define BIT_DQS_DELAY_CELL_MASK             (BIT4|BIT5|BIT6|BIT7)
#define BIT_DQS_DELAY_CELL_SHIFT            4
#define BIT_DQS_MODE_MASK                   (BIT0|BIT1|BIT2)
#define BIT_DQS_MDOE_SHIFT                  0
#define BIT_DQS_MODE_2T                     (0 << BIT_DQS_MDOE_SHIFT)
#define BIT_DQS_MODE_1_5T                   (1 << BIT_DQS_MDOE_SHIFT)
#define BIT_DQS_MODE_2_5T                   (2 << BIT_DQS_MDOE_SHIFT)
#define BIT_DQS_MODE_1T                     (3 << BIT_DQS_MDOE_SHIFT)


#define reg_emmcpll_0x6d                    GET_REG_ADDR(EMMC_PLL_BASE, 0x6D)
#define reg_emmcpll_0x6f                    GET_REG_ADDR(EMMC_PLL_BASE, 0x6F)
#define reg_emmcpll_0x70                    GET_REG_ADDR(EMMC_PLL_BASE, 0x70)
#define reg_emmcpll_0x71                    GET_REG_ADDR(EMMC_PLL_BASE, 0x71)
#define reg_emmcpll_0x73                    GET_REG_ADDR(EMMC_PLL_BASE, 0x73)
#define reg_emmcpll_0x7f                    GET_REG_ADDR(EMMC_PLL_BASE, 0x7F)



#define BIT_TUNE_SHOT_OFFSET_MASK       (BIT4|BIT5|BIT6|BIT7)
#define BIT_TUNE_SHOT_OFFSET_SHIFT      4


#define GT_OFFSET_SKEW4_INV_BIT       0
#define GT_OFFSET_SKEW4_VALUE         1
#define GT_OFFSET_RXDLL_VALUE         2
#define GT_OFFSET_SKEW2_VALUE         3

#define GT_OFFSET_SKEW4_INV_BIT_DIG   4


#define eMMC_RST_L()                    {REG_FCIE_SETBIT(reg_chiptop_0x4F, BIT_reg_emmc_rstz_en);\
                                         REG_FCIE_SETBIT(FCIE_BOOT_CONFIG, BIT1);\
                                         REG_FCIE_CLRBIT(FCIE_BOOT_CONFIG, BIT0);}
#define eMMC_RST_H()                    {REG_FCIE_SETBIT(reg_chiptop_0x4F, BIT_reg_emmc_rstz_en);\
                                         REG_FCIE_SETBIT(FCIE_BOOT_CONFIG, BIT1);\
                                         REG_FCIE_SETBIT(FCIE_BOOT_CONFIG, BIT0);}

////////////////////////////////////////////////////////////////////////////////////////////////////

// define what latch method (mode) fcie has
// implement switch pad function with below cases

#define FCIE_MODE_GPIO_PAD_DEFO_SPEED       0
#define FCIE_eMMC_BYPASS                    FCIE_MODE_GPIO_PAD_DEFO_SPEED

#define FCIE_MODE_8BITS_MACRO_HIGH_SPEED    2
#define FCIE_eMMC_SDR                       FCIE_MODE_8BITS_MACRO_HIGH_SPEED

#define FCIE_MODE_8BITS_MACRO_DDR52         3
#define FCIE_eMMC_DDR                       FCIE_MODE_8BITS_MACRO_DDR52
#define FCIE_eMMC_DDR_8BIT_MACRO            FCIE_MODE_8BITS_MACRO_DDR52

#define FCIE_MODE_32BITS_MACRO_HS200        5
#define FCIE_eMMC_HS200                     FCIE_MODE_32BITS_MACRO_HS200

#define FCIE_MODE_32BITS_MACRO_HS400_DS     6 // data strobe
#define FCIE_eMMC_HS400                     FCIE_MODE_32BITS_MACRO_HS400_DS
#define FCIE_eMMC_HS400_DS                  FCIE_eMMC_HS400

#define FCIE_MODE_32BITS_MACRO_HS400_AIFO_5_1    7 // eMMC 5.1
#define FCIE_eMMC_HS400_5_1                 FCIE_MODE_32BITS_MACRO_HS400_AIFO_5_1
#define FCIE_eMMC_HS400_AIFO_5_1            FCIE_eMMC_HS400_5_1

// define what latch method (mode) use for latch eMMC data
// switch FCIE mode when driver (kernel) change eMMC speed

#define EMMC_DEFO_SPEED_MODE            FCIE_MODE_GPIO_PAD_DEFO_SPEED
#define EMMC_HIGH_SPEED_MODE            FCIE_MODE_8BITS_MACRO_HIGH_SPEED
#define EMMC_DDR52_MODE                 FCIE_MODE_8BITS_MACRO_DDR52
#define EMMC_HS200_MODE                 FCIE_MODE_32BITS_MACRO_HS200
#define EMMC_HS400_MODE                 FCIE_MODE_32BITS_MACRO_HS400_DS // FCIE_MODE_32BITS_MACRO_HS400_SKEW4
#define EMMC_HS400_5_1_MODE             FCIE_MODE_32BITS_MACRO_HS400_AIFO_5_1 // FCIE_MODE_32BITS_MACRO_HS400_SKEW4

//=====================================================
// partitions config
//=====================================================
#define eMMC_CIS_BLK_0                  (64*1024/512) // from 64KB
#define eMMC_NNI_BLK_0                  (eMMC_CIS_BLK_0+0)
#define eMMC_NNI_BLK_1                  (eMMC_CIS_BLK_0+1)
#define eMMC_PNI_BLK_0                  (eMMC_CIS_BLK_0+2)
#define eMMC_PNI_BLK_1                  (eMMC_CIS_BLK_0+3)
#define eMMC_DDRTABLE_BLK_0             (eMMC_CIS_BLK_0+4)
#define eMMC_DDRTABLE_BLK_1             (eMMC_CIS_BLK_0+5)
#define eMMC_HS200TABLE_BLK_0           (eMMC_CIS_BLK_0+6)
#define eMMC_HS200TABLE_BLK_1           (eMMC_CIS_BLK_0+7)
#define eMMC_HS400TABLE_BLK_0           (eMMC_CIS_BLK_0+8)
#define eMMC_HS400TABLE_BLK_1           (eMMC_CIS_BLK_0+9)
#define eMMC_HS400EXTTABLE_BLK_0        (eMMC_CIS_BLK_0+10)
#define eMMC_HS400EXTTABLE_BLK_1        (eMMC_CIS_BLK_0+11)
#define eMMC_ALLRSP_BLK_0               (eMMC_CIS_BLK_0+12)
#define eMMC_ALLRSP_BLK_1               (eMMC_CIS_BLK_0+13)
#define eMMC_BURST_LEN_BLK_0            (eMMC_CIS_BLK_0+14)
#define eMMC_LIFE_TEST_BYTE_CNT_BLK     (eMMC_CIS_BLK_0+15)

#define eMMC_CIS_BLK_END                eMMC_LIFE_TEST_BYTE_CNT_BLK

//=====================================================
// API declarations
//=====================================================
extern  U32 eMMC_hw_timer_delay(U32 u32us);
extern  U32 eMMC_pll_setting(U16 u16_ClkParam);
extern  void HalEmmcPll_dll_setting(void);
extern  U32 eMMC_clock_setting(U16 u16_ClkParam);
extern void eMMC_set_WatchDog(U8 u8_IfEnable);
extern void eMMC_reset_WatchDog(void);
extern  U32 eMMC_translate_DMA_address_Ex(U32 u32_DMAAddr, U32 u32_ByteCnt);
extern void eMMC_Invalidate_data_cache_buffer(U32 u32_addr, S32 s32_size);
extern void eMMC_flush_miu_pipe(void);
extern  U32 eMMC_PlatformResetPre(void);
extern  U32 eMMC_PlatformResetPost(void);
extern  U32 eMMC_PlatformInit(void);
extern void eMMC_DumpPadClk(void);
extern  U32 eMMC_pads_switch(U8 u8Mode);
extern void eMMC_pads_switch_default(void);
extern  U32 eMMC_hw_timer_start(void);
extern  U32 eMMC_hw_timer_tick(void);

extern void *eMMC_memset (void *str, int c, unsigned int len);
extern void *eMMC_memcpy (void *destaddr, void const *srcaddr, unsigned int len);
extern int eMMC_memcmp(const void * cs,const void * ct,unsigned int count);
extern void putk_emmc(char c);
extern void UART_PUTS(char *str , U8 u8_CmdIdx, U16 u16_err);

#define memset(str, c, len) eMMC_memset(str, c, len)
#define memcpy(destaddr, srcaddr, len) eMMC_memcpy(destaddr, srcaddr, len)

//=====================================================
// partitions config
//=====================================================


//=====================================================
// Driver configs
//=====================================================
#define eMMC_UPDATE_FIRMWARE                0

#define eMMC_ST_PLAT                        0
// [CAUTION]: to verify IP and HAL code, defaut 0
#define IF_IP_VERIFY                        0 // [FIXME] -->

#define IF_FCIE_SHARE_PINS                  0 // 1: need to eMMC_pads_switch
#define IF_FCIE_SHARE_CLK                   0 // 1: need to eMMC_clock_setting
#define IF_FCIE_SHARE_IP                    0

//------------------------------
#define FICE_BYTE_MODE_ENABLE               1 // always 1
#define ENABLE_eMMC_INTERRUPT_MODE          0
#define ENABLE_eMMC_RIU_MODE                0 // for debug cache issue
#define ENABLE_DMA_TO_IMI                   1

#define ENABLE_eMMC_ATOP                1
#if defined(ENABLE_eMMC_ATOP) && ENABLE_eMMC_ATOP
#define ENABLE_eMMC_HS400               1
#endif

#if defined(ENABLE_eMMC_HS400) && ENABLE_eMMC_HS400
#define ENABLE_eMMC_AFIFO               1
#define ENABLE_HS400_ENHANCED_STORBE    1
#endif


//------------------------------
// [CAUTION]: to detect DDR timiing parameters, only for DL
#define IF_DETECT_eMMC_DDR_TIMING           0

#if ENABLE_eMMC_RIU_MODE
#undef IF_DETECT_eMMC_DDR_TIMING
#define IF_DETECT_eMMC_DDR_TIMING           0 // RIU mode can NOT use DDR
#endif
// <-- [FIXME]

//------------------------------
#define eMMC_FEATURE_RELIABLE_WRITE         1

//------------------------------
#if defined(ENABLE_eMMC_HS400) && ENABLE_eMMC_HS400
extern U8 gau8_eMMC_SectorBuf[];
#endif

#define eMMC_CACHE_LINE                     0x20 // [FIXME]

//=====================================================
// tool-chain attributes
//===================================================== [FIXME] -->
#define eMMC_PACK0
#define eMMC_PACK1                          __attribute__((__packed__))
#define eMMC_ALIGN0
#define eMMC_ALIGN1                         __attribute__((aligned(eMMC_CACHE_LINE)))
// <-- [FIXME]

//=====================================================
// debug option
//=====================================================
#define eMMC_TEST_IN_DESIGN                 0 // [FIXME]: set 1 to verify HW timer

#ifndef eMMC_DEBUG_MSG
#define eMMC_DEBUG_MSG                      0
#endif

/* Define trace levels. */
#define eMMC_DEBUG_LEVEL_ERROR              (1)    /* Error condition debug messages. */
#define eMMC_DEBUG_LEVEL_WARNING            (2)    /* Warning condition debug messages. */
#define eMMC_DEBUG_LEVEL_HIGH               (3)    /* Debug messages (high debugging). */
#define eMMC_DEBUG_LEVEL_MEDIUM             (4)    /* Debug messages. */
#define eMMC_DEBUG_LEVEL_LOW                (5)    /* Debug messages (low debugging). */

/* Higer debug level means more verbose */
#ifndef eMMC_DEBUG_LEVEL
#define eMMC_DEBUG_LEVEL                    eMMC_DEBUG_LEVEL_WARNING
#endif

#if defined(eMMC_DEBUG_MSG) && eMMC_DEBUG_MSG
#define eMMC_printf    printf
#define eMMC_debug(dbg_lv, tag, str, ...)   \
    do {    \
        if (dbg_lv > eMMC_DEBUG_LEVEL)              \
            break;                                  \
        else {                                      \
            if (tag)                                \
                eMMC_printf("[ %s() ] ", __func__); \
                                                    \
            eMMC_printf(str, ##__VA_ARGS__);        \
        } \
    } while(0)
#else /* eMMC_DEBUG_MSG */
#define eMMC_printf(...)
#define eMMC_debug(enable, tag, str, ...)
#endif /* eMMC_DEBUG_MSG */

#define eMMC_die(msg) while(1);//SYS_FAIL(""msg);

#define eMMC_stop() \
    while(1)  eMMC_reset_WatchDog();

//=====================================================
// unit for HW Timer delay
//=====================================================
#define TIMER0_ENABLE                       GET_REG_ADDR(TIMER0_REG_BASE_ADDR, 0x10)
#define TIMER0_HIT                          GET_REG_ADDR(TIMER0_REG_BASE_ADDR, 0x11)
#define TIMER0_MAX_LOW                      GET_REG_ADDR(TIMER0_REG_BASE_ADDR, 0x12)
#define TIMER0_MAX_HIGH                     GET_REG_ADDR(TIMER0_REG_BASE_ADDR, 0x13)
#define TIMER0_CAP_LOW                      GET_REG_ADDR(TIMER0_REG_BASE_ADDR, 0x14)
#define TIMER0_CAP_HIGH                     GET_REG_ADDR(TIMER0_REG_BASE_ADDR, 0x15)

#define HW_TIMER_DELAY_1us                  1
#define HW_TIMER_DELAY_5us                  5
#define HW_TIMER_DELAY_10us                 10
#define HW_TIMER_DELAY_100us                100
#define HW_TIMER_DELAY_1ms                  (1000 * HW_TIMER_DELAY_1us)
#define HW_TIMER_DELAY_5ms                  (5    * HW_TIMER_DELAY_1ms)
#define HW_TIMER_DELAY_10ms                 (10   * HW_TIMER_DELAY_1ms)
#define HW_TIMER_DELAY_100ms                (100  * HW_TIMER_DELAY_1ms)
#define HW_TIMER_DELAY_500ms                (500  * HW_TIMER_DELAY_1ms)
#define HW_TIMER_DELAY_1s                   (1000 * HW_TIMER_DELAY_1ms)

//=====================================================
// set FCIE clock
//=====================================================
#define FCIE_SLOWEST_CLK           BIT_FCIE_CLK_300K
#define FCIE_SLOW_CLK              BIT_FCIE_CLK_12M
#define FCIE_DEFAULT_CLK           eMMC_PLL_CLK_200M



//=====================================================
// misc
//=====================================================
//#define BIG_ENDIAN
#define LITTLE_ENDIAN

#define __ATTR_DRAM_CODE__  __attribute__((section ("EMMCCODE")))

typedef eMMC_PACK0 struct _eMMC_FCIE_ATOP_SET {
    U32 u32_ScanResult;
    U8  u8_Clk;
    U8  u8_Reg2Ch, u8_Skew4;
    U8  u8_Cell;
    U8  u8_Skew2, u8_CellCnt;
} eMMC_PACK1 eMMC_FCIE_ATOP_SET_t;



//--------------------------------INV----------------------------
#define REG_ANL_SKEW4_INV                reg_emmcpll_0x6c

#define BIT_ANL_SKEW4_INV                BIT7


#define eMMC_PUTS(str , u8_CmdIdx, u16_err) UART_PUTS(str , u8_CmdIdx, u16_err)
#endif /* __eMMC_M7322_UBOOT__ */
