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
#ifndef __eMMC_FCIE_V5_REG_H__
#define __eMMC_FCIE_V5_REG_H__

#define REG(addr)                   (*(volatile U16*)(addr))

#define FCIE_RIU_W16(addr,value)    *((volatile U16*)(addr)) = (value)
#define FCIE_RIU_R16(addr)          *((volatile U16*)(addr))
#define FCIE_RIU_16_ON(addr,value)  FCIE_RIU_W16(addr, FCIE_RIU_R16(addr)|(value))
#define FCIE_RIU_16_OF(addr,value)  FCIE_RIU_W16(addr, FCIE_RIU_R16(addr)&(~(value)))

#ifndef NULL
#define NULL    ((void*)0)
#endif

#define BIT0 0x0001
#define BIT1 0x0002
#define BIT2 0x0004
#define BIT3 0x0008
#define BIT4 0x0010
#define BIT5 0x0020
#define BIT6 0x0040
#define BIT7 0x0080
#define BIT8 0x0100
#define BIT9 0x0200

#ifndef BIT00
#define BIT00    (1<<0)
#endif
#ifndef BIT01
#define BIT01    (1<<1)
#endif
#ifndef BIT02
#define BIT02    (1<<2)
#endif
#ifndef BIT03
#define BIT03    (1<<3)
#endif
#ifndef BIT04
#define BIT04    (1<<4)
#endif
#ifndef BIT05
#define BIT05    (1<<5)
#endif
#ifndef BIT06
#define BIT06    (1<<6)
#endif
#ifndef BIT07
#define BIT07    (1<<7)
#endif
#ifndef BIT08
#define BIT08    (1<<8)
#endif
#ifndef BIT09
#define BIT09    (1<<9)
#endif
#ifndef BIT10
#define BIT10   (1<<10)
#endif
#ifndef BIT11
#define BIT11   (1<<11)
#endif
#ifndef BIT12
#define BIT12   (1<<12)
#endif
#ifndef BIT13
#define BIT13   (1<<13)
#endif
#ifndef BIT14
#define BIT14   (1<<14)
#endif
#ifndef BIT15
#define BIT15   (1<<15)
#endif
#ifndef BIT16
#define BIT16   (1<<16)
#endif
#ifndef BIT17
#define BIT17   (1<<17)
#endif
#ifndef BIT18
#define BIT18   (1<<18)
#endif
#ifndef BIT19
#define BIT19   (1<<19)
#endif
#ifndef BIT20
#define BIT20   (1<<20)
#endif
#ifndef BIT21
#define BIT21   (1<<21)
#endif
#ifndef BIT22
#define BIT22   (1<<22)
#endif
#ifndef BIT23
#define BIT23   (1<<23)
#endif
#ifndef BIT24
#define BIT24   (1<<24)
#endif
#ifndef BIT25
#define BIT25   (1<<25)
#endif
#ifndef BIT26
#define BIT26   (1<<26)
#endif
#ifndef BIT27
#define BIT27   (1<<27)
#endif
#ifndef BIT28
#define BIT28   (1<<28)
#endif
#ifndef BIT29
#define BIT29   (1<<29)
#endif
#ifndef BIT30
#define BIT30   (1<<30)
#endif
#ifndef BIT31
#define BIT31   (1<<31)
#endif

//------------------------------------------------------------------
#define FCIE_MIE_EVENT                  GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x00)
#define FCIE_MIE_INT_EN                 GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x01)
#define FCIE_MMA_PRI_REG                GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x02)
#define FCIE_MIU_DMA_ADDR_15_0          GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x03)
#define FCIE_MIU_DMA_ADDR_31_16         GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x04)
#define FCIE_MIU_DMA_LEN_15_0           GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x05)
#define FCIE_MIU_DMA_LEN_31_16          GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x06)
#define FCIE_MIE_FUNC_CTL               GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x07)
#define FCIE_JOB_BL_CNT                 GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x08)
#define FCIE_BLK_SIZE                   GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x09)
#define FCIE_CMD_RSP_SIZE               GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x0A)
#define FCIE_SD_MODE                    GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x0B)
#define FCIE_SD_CTRL                    GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x0C)
#define FCIE_SD_STATUS                  GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x0D)
#define FCIE_BOOT_CONFIG                GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x0E)
#define FCIE_DDR_MODE                   GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x0F)
#define FCIE_DDR_TOGGLE_CNT             GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x10)
#define FCIE_SDIO_MOD                   GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x11)
//#define FCIE_SBIT_TIMER               GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x11)
#define FCIE_RSP_SHIFT_CNT              GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x12) // GPIO PAD or 8-bit scan use
#define FCIE_RX_SHIFT_CNT               GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x13) // GPIO PAD or 8-bit scan use
#define FCIE_ZDEC_CTL0                  GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x14)
#define FCIE_TEST_MODE                  GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x15)
#define FCIE_MMA_BANK_SIZE              GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x16)
#define FCIE_WR_SBIT_TIMER              GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x17)
//#define FCIE_SDIO_MODE                GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x17)
#define FCIE_RD_SBIT_TIMER              GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x18)
//#define FCIE_DEBUG_BUS0               GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x1E)
//#define FCIE_DEBUG_BUS1               GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x1F)
#define SDIO_INTR_DET					GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x2F)
#define NC_CIFD_EVENT                   GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x30)
#define NC_CIFD_INT_EN                  GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x31)
#define FCIE_PWR_RD_MASK                GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x34)
#define FCIE_PWR_SAVE_CTL               GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x35)
#define FCIE_BIST                       GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x36)
#define FCIE_BOOT                       GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x37)
#define FCIE_EMMC_DEBUG_BUS0            GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x38)
#define FCIE_EMMC_DEBUG_BUS1            GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x39)
#define FCIE_RST                        GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x3F)
//#define NC_WIDTH                      GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x41)
#define FCIE_NC_FUNC_CTL				GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x63)

#define FCIE_CMDFIFO_ADDR(u16_pos)      GET_REG_ADDR(FCIE_CMDFIFO_BASE_ADDR, u16_pos)
#define FCIE_CMDFIFO_BYTE_CNT           0x12// 9 x 16 bits

#define NC_WBUF_CIFD_ADDR(u16_pos)      GET_REG_ADDR(FCIE_CIFD_FIFO_W, u16_pos) // 32 x 16 bits SW Read only
#define NC_RBUF_CIFD_ADDR(u16_pos)      GET_REG_ADDR(FCIE_CIFD_FIFO_R, u16_pos) // 32 x 16 bits	SW write/read
#define SDIO_CRC_ADDR(u16_pos)			GET_REG_ADDR(FCIE_CRC_BUF,     u16_pos) // 32 x 16 bits SW Read only

#define NC_CIFD_ADDR(u16_pos)           NC_RBUF_CIFD_ADDR(u16_pos)

#define NC_CIFD_WBUF_BYTE_CNT           0x40 // 32 x 16 bits
#define NC_CIFD_RBUF_BYTE_CNT           0x40 // 32 x 16 bits

#define FCIE_CIFD_BYTE_CNT              0x40 // 256 x 16 bits

//------------------------------------------------------------------
/* FCIE_MIE_EVENT  0x00 */
/* FCIE_MIE_INT_EN 0x01 */
#define BIT_DMA_END                     BIT00
#define BIT_SD_CMD_END                  BIT01
#define BIT_ERR_STS                     BIT02
//#define BIT_SD_DATA_END               BIT02
#define BIT_SDIO_INT                    BIT03
#define BIT_BUSY_END_INT                BIT04
#define BIT_R2N_RDY_INT                 BIT05
#define BIT_CARD_CHANGE                 BIT06
#define BIT_CARD2_CHANGE                BIT07
//#define BIT_PWR_SAVE_INT              BIT14
//#define BIT_BOOT_DONE_INT             BIT15
//#define BIT_ALL_CARD_INT_EVENTS         (BIT_DMA_END|BIT_SD_CMD_END|BIT_BUSY_END_INT)
#define BIT_ALL_CARD_INT_EVENTS         (BIT_DMA_END|BIT_SD_CMD_END|BIT_ERR_STS|BIT_BUSY_END_INT)

/* FCIE_MMA_PRI_REG 0x02 */
#define BIT_MIU_R_PRI                   BIT00
#define BIT_MIU_W_PRI                   BIT01
#define BIT_MIU_SELECT_MASK             (BIT3|BIT2)
#define BIT_MIU1_SELECT                 BIT02
#define BIT_MIU2_SELECT                 BIT03
#define BIT_MIU3_SELECT                 (BIT03|BIT02)
//#define BIT_DATA_SCRAMBLE             BIT3
#define BIT_MIU_BUS_TYPE_MASK           (BIT04|BIT05)
#define BIT_MIU_BURST1				    (~BIT_MIU_BUS_TYPE_MASK)
#define BIT_MIU_BURST2				    (BIT04)
#define BIT_MIU_BURST4				    (BIT05)
#define BIT_MIU_BURST8				    (BIT04|BIT05)

/* FCIE_FUNC_CTL 0x07 */
#define BIT_EMMC_EN                     BIT00
#define BIT_SD_EN                       BIT01
#define BIT_SDIO_MOD                    BIT02

/* FCIE_JOB_BL_CNT 0x08 */
#define BIT_SD_JOB_BLK_CNT_MASK         (BIT13-1)

/* FCIE_CMD_RSP_SIZE 0x0A */
#define BIT_RSP_SIZE_MASK               (BIT06-1)
#define BIT_CMD_SIZE_MASK               (BIT13|BIT12|BIT11|BIT10|BIT09|BIT08)
#define BIT_CMD_SIZE_SHIFT              8

/* FCIE_SD_MODE 0x0B */
#define BIT_CLK_EN                      BIT00
#define BIT_SD_DATA_WIDTH_MASK          (BIT02|BIT01)
#define BIT_SD_DATA_WIDTH_1             0
#define BIT_SD_DATA_WIDTH_4             BIT01
#define BIT_SD_DATA_WIDTH_8             BIT02
//#define BIT_CS_EN                     BIT03
#define BIT_DATA_DEST					BIT04 // 0: DMA, 1: R2N
#define BIT_DATA_SYNC                   BIT05
//#define BIT_2SD_1CLK_SRC              BIT06
#define BIT_SD_DMA_R_CLK_STOP           BIT07
#define BIT_SD_DEFAULT_MODE_REG         (BIT_DATA_SYNC|BIT_CLK_EN)

/* FCIE_SD_CTRL 0x0C */
#define BIT_SD_RSPR2_EN                 BIT00
#define BIT_SD_RSP_EN                   BIT01
#define BIT_SD_CMD_EN                   BIT02
#define BIT_SD_DTRX_EN					BIT03
#define BIT_SD_DAT_DIR_W                BIT04
#define BIT_ADMA_EN                     BIT05
#define BIT_JOB_START                   BIT06
#define BIT_CHK_CMD                     BIT07
#define BIT_BUSY_DET_ON                 BIT08
#define BIT_ERR_DET_ON                  BIT09
#define BIT_RSP_CRC_NOT_CHK				BIT10

/* FCIE_SD_STATUS 0x0D */
#define R_SD_DAT_CERR           	WBIT00 // data crc error
#define R_SD_DAT_STSERR         	WBIT01
#define R_SD_DAT_STSNEG         	WBIT02
#define R_SD_CMD_NORSP          	WBIT03 // response time out in 64 clocks
#define R_SD_CMDRSP_CERR        	WBIT04 // response crc error
#define R_SD_WR_PRO_N           	WBIT05
#define R_SD_DAT0               	WBIT08
#define R_SD_DAT1               	WBIT09
#define R_SD_DAT2               	WBIT10
#define R_SD_DAT3               	WBIT11
#define R_SD_DAT4               	WBIT12
#define R_SD_DAT5               	WBIT13
#define R_SD_DAT6               	WBIT14
#define R_SD_DAT7               	WBIT15

#define R_SDIO_DAT0				WBIT00

#define BIT_DAT_RD_CERR                 BIT00
#define BIT_SD_R_CRC_ERR                BIT_DAT_RD_CERR
#define BIT_DAT_WR_CERR                 BIT01
#define BIT_SD_W_FAIL                   BIT_DAT_WR_CERR
#define BIT_DAT_WR_TOUT                 BIT02
#define BIT_SD_W_CRC_ERR                BIT_DAT_WR_TOUT
#define BIT_CMD_NO_RSP                  BIT03
#define BIT_SD_RSP_TIMEOUT              BIT_CMD_NO_RSP
#define BIT_CMD_RSP_CERR                BIT04
#define BIT_SD_RSP_CRC_ERR              BIT_CMD_RSP_CERR
#define BIT_DAT_RD_TOUT                 BIT05
//#define BIT_SD_CARD_WP                BIT05
#define BIT_SD_CARD_BUSY                BIT06
#define BIT_SD_D0                       BIT08
//#define BIT_SD_DBUS_MASK              (BIT08|BIT09|BIT10|BIT11|BIT12|BIT13|BIT14|BIT15)
//#define BIT_SD_DBUS_SHIFT             8
#define BIT_SD_FCIE_ERR_FLAGS           (BIT6-1)
#define BIT_SD_CARD_D0_ST               BIT8
#define BIT_SD_CARD_D1_ST               BIT9
#define BIT_SD_CARD_D2_ST               BIT10
#define BIT_SD_CARD_D3_ST               BIT11
#define BIT_SD_CARD_D4_ST               BIT12
#define BIT_SD_CARD_D5_ST               BIT13
#define BIT_SD_CARD_D6_ST               BIT14
#define BIT_SD_CARD_D7_ST               BIT15

#define BITS_ERROR						(BIT_SD_R_CRC_ERR|BIT_DAT_WR_CERR|BIT_DAT_WR_TOUT|BIT_CMD_NO_RSP|BIT_CMD_RSP_CERR|BIT_DAT_RD_TOUT)


/* FCIE_BOOT_CONFIG 0x0E */
#define BIT_EMMC_RSTZ                   BIT0
#define BIT_EMMC_RSTZ_EN                BIT1
#define BIT_BOOT_MODE_EN                BIT2
//#define BIT_BOOT_END                  BIT3

/* FCIE_DDR_MODE 0x0F */
#define BIT_CIF_PAD_IN_RDY_SEL			BIT1
#define BIT_CIF_PRE_FULL_SEL_MSK		(BIT2|BIT3)
#define BIT_CIF_PRE_FULL_SEL_BYPASS		(BIT2)
#define BIT_CIF_PRE_FULL_SEL_SDR		(BIT3)

#define BIT_MACRO_MODE_MASK             (BIT7|BIT8|BIT12|BIT13|BIT14|BIT15)
#define BIT_8BIT_MACRO_EN               BIT7
#define BIT_DDR_EN                      BIT8
#define BIT_FEEDBACK_CLK2				BIT10 // 2015.5.21 start from manhattan
#define BIT_PAD_CLK_SEL					BIT10
#define BIT_32BIT_MACRO_EN              BIT12
#define BIT_PAD_IN_SEL_SD               BIT13
#define BIT_PAD_IN_SEL					BIT13
#define BIT_FALL_LATCH                  BIT14
#define BIT_PAD_IN_MASK                 BIT15

/* FCIE_TOGGLE_CNT 0x10 */
#define BITS_8_MACRO32_SDR_TOGGLE_CNT   0x210
#define BITS_4_MACRO32_SDR_TOGGLE_CNT   0x410

#define BITS_8_MACRO32_DDR_TOGGLE_CNT   0x110
#define BITS_4_MACRO32_DDR_TOGGLE_CNT   0x210

/* FCIE_SDIO_MOD 0x11 */
#define BIT_REG_SDIO_MOD_MASK           (BIT01|BIT00)
#define SDIO_INT_MOD_1					BIT00
#define SDIO_INT_MOD_2					BIT01 // CMD52 use
#define SDIO_INT_MOD_3					(BIT01|BIT00) // CMD53 use

#define BIT_SDIO_INT_SW_MODE			BIT02
#define BIT_SDIO_DET_INT_SRC            BIT03 // 0: edge, 1: level trigger

#define BITS_SDIO_INT_TUNE				BIT06|BIT05|BIT04
#define BITS_SDIO_INT_TUNE_CLR			BIT09|BIT08|BIT07

#define BIT_RD_WAIT_EN					BIT11
#define BIT_BLK_GAP_DIS					BIT12
#define BIT_INT_STOP_DMA				BIT13 // not detect after DMA finish
#define BIT_INT_TUNE_SW					BIT14
#define BIT_INT_ASYNC_EN				BIT15

/* FCIE_RSP_SHIFT_CNT 0x12 */
#define BIT_RSP_SHIFT_TUNE_MASK         (BIT0|BIT1|BIT2|BIT3)
#define BIT_RSP_SHIFT_SEL               BIT4		//	0: HW, 1: SW

/* FCIE_RX_SHIFT_CNT 0x13 */
#define BIT_RSTOP_SHIFT_TUNE_MASK       (BIT0|BIT1|BIT2|BIT3)
#define BIT_RSTOP_SHIFT_SEL             BIT4
#define BIT_WRSTS_SHIFT_TUNE_MASK       (BIT8|BIT9|BIT10|BIT11)
#define BIT_WRSTS_SHIFT_SEL             BIT12

/* FCIE_ZDEC_CTL0 0x14 */
#define BIT_ZDEC_EN                     BIT0
#define BIT_SD2ZDEC_PTR_CLR             BIT1

/* FCIE_TEST_MODE 0x15 */
#define BIT_SDDR1                       BIT0
#define BIT_DEBUG_MODE_MASK             (BIT3|BIT2|BIT1)
#define BIT_DEBUG_MODE_SHIFT            1
#define BIT_BIST_MODE                   BIT4
//#define BIT_DS_TESTEN                 BIT1
//#define BIT_TEST_MODE                 BIT2
//#define BIT_DEBUG_MODE_MASK           BIT3|BIT4|BIT5
//#define BIT_DEBUG_MODE_SHIFT          3
//#define BIT_TEST_MIU                  BIT6
//#define BIT_TEST_MIE                  BIT7
//#define BIT_TEST_MIU_STS              BIT8
//#define BIT_TEST_MIE_STS              BIT9
//#define BIT_BIST_MODE                 BIT10

/* FCIE_WR_SBIT_TIMER 0x17 */
#define BIT_WR_SBIT_TIMER_MASK          (BIT15-1)
#define BIT_WR_SBIT_TIMER_EN            BIT15

/* FCIE_RD_SBIT_TIMER 0x18 */
#define BIT_RD_SBIT_TIMER_MASK          (BIT15-1)
#define BIT_RD_SBIT_TIMER_EN            BIT15

/* 0x2F */
#define BIT_SDIO_DET_ON                 BIT0 // after detect, auto clear. New reg from celtics


/* NC_CIFD_EVENT 0x30 */
#define BIT_WBUF_FULL                   BIT0
#define BIT_WBUF_EMPTY_TRI              BIT1
#define BIT_RBUF_FULL_TRI               BIT2
#define BIT_RBUF_EMPTY                  BIT3

/* NC_CIFD_INT_EN 0x31 */
#define BIT_WBUF_FULL_INT_EN            BIT0
#define BIT_RBUF_EMPTY_INT_EN           BIT1
#define BIT_F_WBUF_FULL_INT             BIT2
#define BIT_F_RBUF_EMPTY_INT            BIT3

/* FCIE_PWR_SAVE_CTL 0x35 */
#define BIT_POWER_SAVE_MODE             BIT0
#define BIT_SD_POWER_SAVE_RIU           BIT1
#define BIT_POWER_SAVE_MODE_INT_EN      BIT2
#define BIT_SD_POWER_SAVE_RST           BIT3
#define BIT_POWER_SAVE_INT_FORCE        BIT4
#define BIT_RIU_SAVE_EVENT              BIT5
#define BIT_RST_SAVE_EVENT              BIT6
#define BIT_BAT_SAVE_EVENT              BIT7
#define BIT_BAT_SD_POWER_SAVE_MASK      BIT8
#define BIT_RST_SD_POWER_SAVE_MASK      BIT9
#define BIT_POWER_SAVE_MODE_INT         BIT15

/* FCIE_BIST 0x36 */

/* FCIE_BOOT 0x37 */
#define BIT_NAND_BOOT_EN                BIT0
#define BIT_BOOTSRAM_ACCESS_SEL         BIT1

// FCIE 0x38
#define BIT_STATE_MSK_CMD				(BIT03|BIT02|BIT01|BIT00)
#define BIT_STATE_MSK_RD				(BIT03|BIT02|BIT01|BIT00)
#define BIT_STATE_MSK_WR				(BIT07|BIT06|BIT05|BIT04)
#define BIT_STATE_MSK_FIFO				(BIT11|BIT10|BIT09|BIT08)

#define BIT_SHIFT_CMD					0
#define BIT_SHIFT_RD					0
#define BIT_SHIFT_WR					4
#define BIT_SHIFT_FIFO					8

/* FCIE_BOOT 0x39 */
#define BIT_DEBUG_MODE_MSK				(BIT11|BIT10|BIT9|BIT8)
#define BIT_DEBUG_MODE_SET				(BIT10|BIT8)
#define BIT_SHIFT_DEBUG_MODE			8

/*FCIE_RST 0x3F*/

#define BIT_FCIE_SOFT_RST_n             BIT0
#define BIT_RST_MIU_STS                 BIT1
#define BIT_RST_MIE_STS                 BIT2
#define BIT_RST_MCU_STS                 BIT3
#define BIT_RST_STS_MASK                (BIT_RST_MIU_STS | BIT_RST_MIE_STS |BIT_RST_MCU_STS)
#define BIT_NC_DEB_SEL_SHIFT            12
#define BIT_NC_DEB_SEL_MASK             (BIT15|BIT14|BIT13|BIT12)

/*FCIE_NC_FUNC_CTL 0x63*/
#define BIT_NC_EN						BIT0

//------------------------------------------------------------------
/*
 * Power Save FIFO Cmd*
 */
#define PWR_BAT_CLASS    (0x1 << 13)  /* Battery lost class */
#define PWR_RST_CLASS    (0x1 << 12)  /* Reset Class */

/* Command Type */
#define PWR_CMD_WREG     (0x0 << 9)   /* Write data */
#define PWR_CMD_RDCP     (0x1 << 9)   /* Read and cmp data. If mismatch, HW retry */
#define PWR_CMD_WAIT     (0x2 << 9)   /* Wait idle, max. 128T */
#define PWR_CMD_WINT     (0x3 << 9)   /* Wait interrupt */
#define PWR_CMD_STOP     (0x7 << 9)   /* Stop */

/* RIU Bank */
#define PWR_CMD_BK0      (0x0 << 7)
#define PWR_CMD_BK1      (0x1 << 7)
#define PWR_CMD_BK2      (0x2 << 7)
#define PWR_CMD_BK3      (0x3 << 7)

#define PWR_RIU_ADDR     (0x0 << 0)



#endif /* __eMMC_FCIE_V5_REG_H__ */
