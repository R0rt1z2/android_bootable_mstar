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

#ifndef  __DRVFCIE_MADISON_UBOOT_H__
#define  __DRVFCIE_MADISON_UBOOT_H__

//#include <common.h> //printf()
//#include <sys/common/MsTypes.h>


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
//
//                               RIU BANK INFOMATION
//
///////////////////////////////////////////////////////////////////////////////////////////////////
#define FCIE_OffShift           2
#define RIU_BASE                0x1F200000U
#define RIU_BASE_CLKGEN0        (RIU_BASE+(0x0580U<<FCIE_OffShift))
#define RIU_BASE_CHIPTOP        (RIU_BASE+(0x0F00U<<FCIE_OffShift))
#ifdef USE_SD_MODE
#define RIU_BASE_FCIE           (RIU_BASE+(0x8980U<<FCIE_OffShift))
#define RIU_BASE_FCIE1          (RIU_BASE+(0x89E0U<<FCIE_OffShift))
#define RIU_BASE_FCIE2          (RIU_BASE+(0x8A80U<<FCIE_OffShift))
#else
#define RIU_BASE_FCIE           (RIU_BASE+(0x10780U<<FCIE_OffShift))
#define RIU_BASE_FCIE1          (RIU_BASE+(0x107E0U<<FCIE_OffShift))
#define RIU_BASE_FCIE2          (RIU_BASE+(0x11480U<<FCIE_OffShift))

#endif
#define CIF_C_ADDR              RIU_BASE_FCIE1

//=====================================================
// HW Timer for Delay
//=====================================================
#define RIU_PM_BASE             0x1F000000
#define RIU_BASE_TIMER0  		(RIU_PM_BASE+(0x1800<<FCIE_OffShift))
#define TIMER0_ENABLE			(RIU_BASE_TIMER0+(0x10<<FCIE_OffShift))
#define TIMER0_HIT				(RIU_BASE_TIMER0+(0x11<<FCIE_OffShift))
#define TIMER0_MAX_LOW			(RIU_BASE_TIMER0+(0x12<<FCIE_OffShift))
#define TIMER0_MAX_HIGH			(RIU_BASE_TIMER0+(0x13<<FCIE_OffShift))
#define TIMER0_CAP_LOW			(RIU_BASE_TIMER0+(0x14<<FCIE_OffShift))
#define TIMER0_CAP_HIGH			(RIU_BASE_TIMER0+(0x15<<FCIE_OffShift))

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//                               PLATFORM MACRO DEFINITION
//
///////////////////////////////////////////////////////////////////////////////////////////////////
#define halFCIE_PHY2MIUAddr(addr) addr

#if MMC_CACHE
#define halFCIE_Flush_Cache(base,size)  flush_cache(base, size)
#else
#define halFCIE_Flush_Cache(base,size)
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//                               CHIPTOP
//
///////////////////////////////////////////////////////////////////////////////////////////////////
#define ALL_PAD_IN              (RIU_BASE_CHIPTOP+(0x50<<FCIE_OffShift))

#define SD_TOP_CFG              (RIU_BASE_CHIPTOP+(0x5A<<FCIE_OffShift))
#define CFG_SD_PAD              WBIT05

#define EMMC_TOP_CFG            (RIU_BASE_CHIPTOP+(0x6E<<FCIE_OffShift))
#define CFG_EMMC_PAD            WBIT06
#define CFG_EMMC_DDR_PAD        WBIT07

#define NAND_TOP_CFG            (RIU_BASE_CHIPTOP+(0x6F<<FCIE_OffShift))
#define PCMCIA_TOP_CFG          (RIU_BASE_CHIPTOP+(0x64<<FCIE_OffShift))

#define EMMC_BOOT_CFG          (RIU_BASE_CHIPTOP+(0x10<<FCIE_OffShift))
#define FORCE_BOOT_FROM_EMMC2   WBIT01

#define FCIE_MACRO_BYPASS       (RIU_BASE_CHIPTOP+(0x43<<FCIE_OffShift))
#define SD_USE_BYPASS           WBIT00
#define FCIE2MACRO_SD_BYPASS    WBIT01

#define EMMC_CONFIG             (RIU_BASE_CHIPTOP+(0x5D<<FCIE_OffShift))
#define EMMC_MODE_1             WBIT14
#define EMMC_MODE_2             WBIT14 | WBIT15

#define NAND_CONFIG             (RIU_BASE_CHIPTOP+(0x0B<<FCIE_OffShift))
#define NAND_CONFIG_MASK        0x7FFF

#define CHIPTOP_0C   		   (RIU_BASE_CHIPTOP+ (0x0c << FCIE_OffShift))
#define CHIPTOP_12   		   (RIU_BASE_CHIPTOP+ (0x12 << FCIE_OffShift))
#define CHIPTOP_1F   		   (RIU_BASE_CHIPTOP+ (0x1F << FCIE_OffShift))

#define CHIPTOP_42   		   (RIU_BASE_CHIPTOP+ (0x42 << FCIE_OffShift))
#define CHIPTOP_43   		   (RIU_BASE_CHIPTOP+ (0x43 << FCIE_OffShift))
#define CHIPTOP_4F   		   (RIU_BASE_CHIPTOP+ (0x4F << FCIE_OffShift))
#define CHIPTOP_50   		   (RIU_BASE_CHIPTOP+ (0x50 << FCIE_OffShift))
#define CHIPTOP_5B   		   (RIU_BASE_CHIPTOP+ (0x5B << FCIE_OffShift))
#define CHIPTOP_64   		   (RIU_BASE_CHIPTOP+ (0x64 << FCIE_OffShift))
#define CHIPTOP_6F   		   (RIU_BASE_CHIPTOP+ (0x6F << FCIE_OffShift))
#define CHIPTOP_70   		   (RIU_BASE_CHIPTOP+ (0x70 << FCIE_OffShift))

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//                               CLKGEN
//
///////////////////////////////////////////////////////////////////////////////////////////////////
#define REG_CLK_NFIE            (RIU_BASE_CLKGEN0+(0x69<<FCIE_OffShift))
#define REG_CLK_EMMC            (RIU_BASE_CLKGEN0+(0x31<<FCIE_OffShift))
#define REG_CLK_65			(RIU_BASE_CLKGEN0+(0x65<<FCIE_OffShift))
#define REG_CLK_64			(RIU_BASE_CLKGEN0+(0x64<<FCIE_OffShift))

///////////////////////////////////////////////////////////////////////////////////////////////////
// Timer
///////////////////////////////////////////////////////////////////////////////////////////////////
extern void udelay (unsigned long usec);
#define FCIE_DELAY_MS(MS) udelay(MS*1000)
#define FCIE_DELAY_US(US) udelay(US)

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//                               EXTERN GLOBAL FUNCTION
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//void FCIE_HWTimer_Start(void);
//U32 FCIE_HWTimer_End(void);
extern U8 halFCIE_Platform_ClearEvent(U16 nReq);
extern void halFCIE_Platform_InitChiptop(void);
extern E_IO_STS halFCIE_Platform_WaitMieEvent(U16 u16ReqVal, U32 u32WaitMs);
extern void halFCIE_Platform_Reset_Fcie(void);
extern U8 halFCIE_Platform_SetFcieClock(U32 u32Clock);
#endif  //__FCIE_PLAT_A5_UBOOT_H__
