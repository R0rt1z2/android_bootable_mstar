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


#ifndef __PLATFORM_H__
#define __PLATFORM_H__
#define USBHOST_LIB_VER	"2014.10.02"

#include <configs/uboot_board_config.h> // system board definition
#include "MsTypes_usb.h"

/* Select a OS Platform */
//#define SERET_ENV
#define MSTAR_UBOOT_ENV

/* General Setting for Mstar Platform USB Portting */
//#ifdef SERET_ENV
//#define CONFIG_USB_STORAGE 1
//#define USB_BAD_DEVICE_RETRY_PATCH 1
/* normal 1 + additional N */
//#define USB_BAD_DEV_MAX_RETRY 2
//#endif
#define USB_WAIT_LOOP_COUNT 0 // for super speed device

/* USB Test Mode */
// #define USB_TEST
#define USB_LEGEND_DATA_TOGGLE_METHOD

/****** Chip variable setting ******/
#if 1	/* Every Mstar chip should appley it */
	#define _USB_FLUSH_BUFFER	1
#else
	#define _USB_FLUSH_BUFFER	0
#endif

//---- 1. change override to hs_txser_en condition (DM always keep high issue)
//	#define ENABLE_HS_DM_KEEP_HIGH_ECO

//---- 2. fix pv2mi bridge mis-behavior
#if 1	/* Every Mstar chip should appley it */
	#define ENABLE_PV2MI_BRIDGE_ECO
#endif

//---- 3. 240's phase as 120's clock
#if 1	/* Every Mstar chip should appley it */
	/* bit<3> for 240's phase as 120's clock set 1, bit<4> for 240Mhz in mac 0 for faraday 1 for etron */
	#define ENABLE_UTMI_240_AS_120_PHASE_ECO
	//#define UTMI_240_AS_120_PHASE_ECO_INV
#endif

//---- 4. change to 55 interface
#if 1	/* Every Mstar chip should appley it */
	#define ENABLE_UTMI_55_INTERFACE
#endif

//---- 5. enabe PVCI i_miwcplt wait for mi2uh_last_done_z
#if 1	/* Every Mstar New chip should appley it */
#define _USB_MIU_WRITE_WAIT_LAST_DONE_Z_PATCH 1
#endif

//------ UTMI disconnect level parameters ---------------------------------
// disc: bit[7:4] 0x00: 550mv, 0x20: 575, 0x40: 600, 0x60: 625
// squelch: bit[3:0] 4'b0010 => 100mv
#define UTMI_DISCON_LEVEL_2A	(0x60 | 0x02)

//------ UTMI eye diagram parameters ---------------------------------
#if 0
	// for 40nm
	#define UTMI_EYE_SETTING_2C	(0x98)
	#define UTMI_EYE_SETTING_2D	(0x02)
	#define UTMI_EYE_SETTING_2E	(0x10)
	#define UTMI_EYE_SETTING_2F	(0x01)
	#define UTMI_ALL_EYE_SETTING	(0x01100298)
#elif 0
	// for 40nm after Agate, use 55nm setting7
	#define UTMI_EYE_SETTING_2C	(0x90)
	#define UTMI_EYE_SETTING_2D	(0x03)
	#define UTMI_EYE_SETTING_2E	(0x30)
	#define UTMI_EYE_SETTING_2F	(0x81)
	#define UTMI_ALL_EYE_SETTING	(0x81300390)
#elif 0
	// for 40nm after Agate, use 55nm setting6
	#define UTMI_EYE_SETTING_2C	(0x10)
	#define UTMI_EYE_SETTING_2D	(0x03)
	#define UTMI_EYE_SETTING_2E	(0x30)
	#define UTMI_EYE_SETTING_2F	(0x81)
	#define UTMI_ALL_EYE_SETTING	(0x81300310)
#elif 0
	// for 40nm after Agate, use 55nm setting5
	#define UTMI_EYE_SETTING_2C	(0x90)
	#define UTMI_EYE_SETTING_2D	(0x02)
	#define UTMI_EYE_SETTING_2E	(0x30)
	#define UTMI_EYE_SETTING_2F	(0x81)
	#define UTMI_ALL_EYE_SETTING	(0x81300290)
#elif 0
	// for 40nm after Agate, use 55nm setting4
	#define UTMI_EYE_SETTING_2C	(0x90)
	#define UTMI_EYE_SETTING_2D	(0x03)
	#define UTMI_EYE_SETTING_2E	(0x00)
	#define UTMI_EYE_SETTING_2F	(0x81)
	#define UTMI_ALL_EYE_SETTING	(0x81000390)
#elif 0
	// for 40nm after Agate, use 55nm setting3
	#define UTMI_EYE_SETTING_2C	(0x10)
	#define UTMI_EYE_SETTING_2D	(0x03)
	#define UTMI_EYE_SETTING_2E	(0x00)
	#define UTMI_EYE_SETTING_2F	(0x81)
	#define UTMI_ALL_EYE_SETTING	(0x81000310)
#elif 0
	// for 40nm after Agate, use 55nm setting2
	#define UTMI_EYE_SETTING_2C	(0x90)
	#define UTMI_EYE_SETTING_2D	(0x02)
	#define UTMI_EYE_SETTING_2E	(0x00)
	#define UTMI_EYE_SETTING_2F	(0x81)
	#define UTMI_ALL_EYE_SETTING	(0x81000290)
#else
	// for 40nm after Agate, use 55nm setting1, the default
	#define UTMI_EYE_SETTING_2C	(0x10)
	#define UTMI_EYE_SETTING_2D	(0x02)
	#define UTMI_EYE_SETTING_2E	(0x00)
	#define UTMI_EYE_SETTING_2F	(0x81)
	#define UTMI_ALL_EYE_SETTING	(0x81000210)
#endif

/****** Porting environment setting ******/
#if defined(SERET_ENV)
	/* Delay Function */
	#include <timer.h>
	#define udelay(us)		udelay(us)

	/* buncing buffer enable for non cache flush API in cache enable system */
	// #define USB_BOUNCING_BUF	1
	#define readb(addr)		*((unsigned char volatile *)(addr))
	#define writeb(val, addr)	(*((unsigned char volatile *)(addr)) = (unsigned char)val)

	#define mdelay(ms)		{	MS_U32 i;		\
						for(i=0;i<ms;i++)	\
						udelay(1000);		\
					}

	#define USB_DELAY(ms)		mdelay(ms)
	#define wait_ms(ms)		mdelay(ms)
#endif

#if defined(MSTAR_UBOOT_ENV)
	extern MS_U32 MsOS_VA2PA(MS_U32 addr);
	extern MS_U32 MsOS_PA2KSEG0(MS_U32 addr);
	extern MS_U32 MsOS_PA2KSEG1(MS_U32 addr);
	extern void Chip_Read_Memory(void);
	extern void Chip_Flush_Memory(void);
	extern void   flush_cache   (unsigned long, unsigned long);

	/* Delay Function */
	void udelay (unsigned long);

	#define readb(addr)			*((unsigned char volatile *)(addr))
	#define writeb(val, addr)	(*((unsigned char volatile *)(addr)) = (unsigned char)val)
	#define readw(addr) 		*((volatile MS_UINT16 *)(addr))
	#define writew(val, addr)	(*((volatile MS_UINT16 *)(addr)) = (MS_UINT16)val)
	#define readl(addr) 		*((volatile MS_UINT32 *)(addr))
	#define writel(val, addr)	(*((volatile MS_UINT32 *)(addr)) = (MS_UINT32)val)

	#define mdelay(ms)		{	MS_U32 i;		\
						for(i=0;i<ms;i++)	\
						udelay(1000);		\
					}

	#define USB_DELAY(ms)		mdelay(ms)

	#if defined(__ARM__)
		#define KSEG02KSEG1(addr)	MsOS_PA2KSEG1(MsOS_VA2PA((MS_U32)addr))
		#define KSEG12KSEG0(addr)	MsOS_PA2KSEG0(MsOS_VA2PA((MS_U32)addr))
	#else
		#define KSEG02KSEG1(addr)	((void *)((MS_U32)(addr)|0x20000000))  //cached -> unchched
		#define KSEG12KSEG0(addr)	((void *)((MS_U32)(addr)&~0x20000000)) //unchched -> cached
	#endif

	/* virtual address to physical address translation */
	#if defined(__ARM__)
		#if (ENABLE_MONACO) || (ENABLE_MASERATI) /* Monaco2 */
			#define ENABLE_USB_NEW_MIU_SEL	1
			#define USB_MIU_SEL0	0x70
			#define USB_MIU_SEL1	0xB8
			#define USB_MIU_SEL2	0xDC
			#define USB_MIU_SEL3	0xFF
			#define MIU0_BUS_BASE_ADDR	CONFIG_MIU0_BUSADDR
			#define MIU0_PHY_BASE_ADDR	0x00000000UL
			#define MIU1_BUS_BASE_ADDR	CONFIG_MIU1_BUSADDR
			#define MIU1_PHY_BASE_ADDR	0x80000000UL
			#if defined(CONFIG_MIU2_BUSADDR)
				#define MIU2_BUS_BASE_ADDR	CONFIG_MIU2_BUSADDR
				#define MIU2_PHY_BASE_ADDR	0xC0000000UL
			#endif
		#elif (ENABLE_M7221)
			#define ENABLE_USB_NEW_MIU_SEL	1
			#define USB_MIU_SEL0	0x70	/* MIU0: 2GB */
			#define USB_MIU_SEL1	0xF8	/* MIU1: 2GB */
			#define USB_MIU_SEL2	0xEF
			#define USB_MIU_SEL3	0xEF
			#define MIU0_BUS_BASE_ADDR	CONFIG_MIU0_BUSADDR
			#define MIU0_PHY_BASE_ADDR	0x00000000UL
			#define MIU1_BUS_BASE_ADDR	CONFIG_MIU1_BUSADDR
			#define MIU1_PHY_BASE_ADDR	0x80000000UL
		#elif (ENABLE_MIAMI)  //Only MIU0/MIU1
		        #define ENABLE_USB_NEW_MIU_SEL	1
			#define USB_MIU_SEL0	0x70
			#define USB_MIU_SEL1	0xB8
			#define USB_MIU_SEL2	0xFF
			#define USB_MIU_SEL3	0xFF
			#define MIU0_BUS_BASE_ADDR	CONFIG_MIU0_BUSADDR
			#define MIU0_PHY_BASE_ADDR	0x00000000UL
			#define MIU1_BUS_BASE_ADDR	CONFIG_MIU1_BUSADDR
			#define MIU1_PHY_BASE_ADDR	0x80000000UL
		#elif (ENABLE_M7322)
			#define ENABLE_USB_NEW_MIU_SEL	1
			#define USB_MIU_SEL0	0x70	/* MIU0: 2GB */
			#define USB_MIU_SEL1	0xF8	/* MIU1: 2GB */
			#define USB_MIU_SEL2	0xEF
			#define USB_MIU_SEL3	0xEF
			#define MIU0_BUS_BASE_ADDR	CONFIG_MIU0_BUSADDR
			#define MIU0_PHY_BASE_ADDR	0x00000000UL
			#define MIU1_BUS_BASE_ADDR	CONFIG_MIU1_BUSADDR
			#define MIU1_PHY_BASE_ADDR	0x80000000UL
		#elif (ENABLE_MONET)  //Only MIU0/MIU1
		        #define ENABLE_USB_NEW_MIU_SEL	1
			#define USB_MIU_SEL0	0x70
			#define USB_MIU_SEL1	0xB8
			#define USB_MIU_SEL2	0xFF
			#define USB_MIU_SEL3	0xFF
			#define MIU0_BUS_BASE_ADDR	CONFIG_MIU0_BUSADDR
			#define MIU0_PHY_BASE_ADDR	0x00000000UL
			#define MIU1_BUS_BASE_ADDR	CONFIG_MIU1_BUSADDR
			#define MIU1_PHY_BASE_ADDR	0x80000000UL
		#elif (ENABLE_MUJI)  //Only MIU0/MIU1
		        #define ENABLE_USB_NEW_MIU_SEL	1
			#define USB_MIU_SEL0	0x70
			#define USB_MIU_SEL1	0xB8
			#define USB_MIU_SEL2	0xFF
			#define USB_MIU_SEL3	0xFF
			#define MIU0_BUS_BASE_ADDR	CONFIG_MIU0_BUSADDR
			#define MIU0_PHY_BASE_ADDR	0x00000000UL
			#define MIU1_BUS_BASE_ADDR	CONFIG_MIU1_BUSADDR
			#define MIU1_PHY_BASE_ADDR	0x80000000UL
		#elif (ENABLE_CELTICS) || (ENABLE_KANO)
		    #define ENABLE_USB_NEW_MIU_SEL	1
			#define USB_MIU_SEL0	0x70
			#define USB_MIU_SEL1	0xB8
			#define USB_MIU_SEL2	0xFF
			#define USB_MIU_SEL3	0xFF
			#define MIU0_BUS_BASE_ADDR	CONFIG_MIU0_BUSADDR
			#define MIU0_PHY_BASE_ADDR	0x00000000UL
			#define MIU1_BUS_BASE_ADDR	CONFIG_MIU1_BUSADDR
			#define MIU1_PHY_BASE_ADDR	0x80000000UL
		#elif (ENABLE_CURRY)
			#define ENABLE_USB_NEW_MIU_SEL      1
			#define USB_MIU_SEL0    0x70
			#define USB_MIU_SEL1    0x0F
			#define USB_MIU_SEL2    0x0F
			#define USB_MIU_SEL3    0x0F
			#define MIU0_BUS_BASE_ADDR      CONFIG_MIU0_BUSADDR
			#define MIU0_PHY_BASE_ADDR      0x00000000UL
			#define MIU1_BUS_BASE_ADDR      CONFIG_MIU1_BUSADDR
			#define MIU1_PHY_BASE_ADDR      0x80000000UL
		#elif (ENABLE_MUNICH)  //Only MIU0/MIU1
		    #define ENABLE_USB_NEW_MIU_SEL	1
			#define USB_MIU_SEL0	0x70
			#define USB_MIU_SEL1	0xB8
			#define USB_MIU_SEL2	0xFF
			#define USB_MIU_SEL3	0xFF
			#define MIU0_BUS_BASE_ADDR	CONFIG_MIU0_BUSADDR
			#define MIU0_PHY_BASE_ADDR	0x00000000UL
			#define MIU1_BUS_BASE_ADDR	CONFIG_MIU1_BUSADDR
			#define MIU1_PHY_BASE_ADDR	0x80000000UL
		#elif (ENABLE_MANHATTAN)  //Only MIU0/MIU1
		    #define ENABLE_USB_NEW_MIU_SEL	1
			#define USB_MIU_SEL0	0x70
			#define USB_MIU_SEL1	0xB8
			#define USB_MIU_SEL2	0xFF
			#define USB_MIU_SEL3	0xFF
			#define MIU0_BUS_BASE_ADDR	CONFIG_MIU0_BUSADDR
			#define MIU0_PHY_BASE_ADDR	0x00000000UL
			#define MIU1_BUS_BASE_ADDR	CONFIG_MIU1_BUSADDR
			#define MIU1_PHY_BASE_ADDR	0x80000000UL
		#elif (ENABLE_MESSI) || (ENABLE_MOONEY)
			#define ENABLE_USB_NEW_MIU_SEL	1	/* Unit: 4GB / 16 = 256MB */
			#define USB_MIU_SEL0	0x30	/* MIU0: 1GB */
			#define USB_MIU_SEL1	0x74	/* MIU1: 1GB */
			#define USB_MIU_SEL2	0xEF
			#define USB_MIU_SEL3	0xEF
			#define MIU0_BUS_BASE_ADDR	CONFIG_MIU0_BUSADDR
			#define MIU0_PHY_BASE_ADDR	0x00000000UL
			#define MIU1_BUS_BASE_ADDR	CONFIG_MIU1_BUSADDR
			#define MIU1_PHY_BASE_ADDR	0x40000000UL
		#elif (ENABLE_MACAN)
			#define ENABLE_USB_NEW_MIU_SEL	1	/* Unit: 4GB / 16 = 256MB */
			#define USB_MIU_SEL0	0x70	/* MIU0: 2GB */
			#define USB_MIU_SEL1	0xB8	/* MIU1: 1GB */
			#define USB_MIU_SEL2	0xFF
			#define USB_MIU_SEL3	0xFF
			#define MIU0_BUS_BASE_ADDR	CONFIG_MIU0_BUSADDR
			#define MIU0_PHY_BASE_ADDR	0x00000000UL
			#define MIU1_BUS_BASE_ADDR	CONFIG_MIU1_BUSADDR
			#define MIU1_PHY_BASE_ADDR	0x80000000UL
		#elif (ENABLE_KASTOR)
			#define ENABLE_USB_NEW_MIU_SEL	1	/* Unit: 4GB / 16 = 256MB */
			#define USB_MIU_SEL0	0x70	/* MIU0: 2GB */
			#define USB_MIU_SEL1	0xB8	/* MIU1: 1GB */
			#define USB_MIU_SEL2	0xEF
			#define USB_MIU_SEL3	0xEF
			#define MIU0_BUS_BASE_ADDR	CONFIG_MIU0_BUSADDR
			#define MIU0_PHY_BASE_ADDR	0x00000000UL
			#define MIU1_BUS_BASE_ADDR	CONFIG_MIU1_BUSADDR
			#define MIU1_PHY_BASE_ADDR	0x80000000UL
		#else
			#define MIU0_BUS_BASE_ADDR	CONFIG_MIU0_BUSADDR
			#define MIU0_PHY_BASE_ADDR	0x00000000UL
			#define MIU1_BUS_BASE_ADDR	CONFIG_MIU1_BUSADDR
			#define MIU1_PHY_BASE_ADDR	0x80000000UL
		#endif

		/* transmit between BUS and USB PHY Addr */
		extern unsigned int VA2PA(unsigned int u32_DMAAddr);
		extern unsigned int PA2VA(unsigned int u32_DMAAddr);
	#elif defined(__MIPS__)
		#define VA2PA(a)  (a)
		#define PA2VA(a)  (a)
	#else // ??? which CPU
		#define VA2PA(a)  ((a) & 0x1FFFFFFF)
		#define PA2VA(a)  ((a) | 0xA0000000) //// mapping to uncache address
	#endif
#endif

/* RIU Base */
#if defined(__ARM__)
	#define HW_BASE		0x1f200000
#elif defined(__MIPS__)
	#define HW_BASE		0xbf200000
#else
	#error Unknow CPU core type
#endif

#if defined(CONFIG_ARM)
#define _MSTAR_PM_BASE         0x1F000000
#else
#define _MSTAR_PM_BASE         0xBF000000
#endif

#define EHCFLAG_NONE		0x0
#define EHCFLAG_DPDM_SWAP	0x1
#define EHCFLAG_XHC_COMP	0x2

/* USB Disk upgrade IMI/EMI switch */
#define ENABLE_USB_DISK_UPGRADE_EMI_SWITCH
#define usb_port0_emi_switch() \
{ \
    writeb(readb(HW_BASE+0xb18*2) | 0x1, HW_BASE+0xb18*2); \
    writeb(readb(HW_BASE+0x1e5b*2-1) & ~0x1, HW_BASE+0x1e5b*2-1); \
}

/* xhc definition */
#if (ENABLE_MONACO) || (ENABLE_MIAMI) || (ENABLE_MUJI) || (ENABLE_CELTICS) || \
	(ENABLE_KANO) || (ENABLE_MASERATI)
#define XHCI_2PORTS
#endif

#if (ENABLE_MONACO) || (ENABLE_MIAMI) || (ENABLE_MUJI) || (ENABLE_CELTICS) || \
	(ENABLE_MASERATI)
#define XHCI_PORT0_ADDR		(HW_BASE+0x90440)
#define XHCI_PORT1_ADDR		(HW_BASE+0x90450)
#define MSTAR_U3TOP_BASE	(HW_BASE+(0x22500 << 1))
#elif (ENABLE_KANO)
#define XHCI_PORT0_ADDR		(HW_BASE+0xC0440)
#define XHCI_PORT1_ADDR		(HW_BASE+0xC0450)
#define MSTAR_U3TOP_BASE	(HW_BASE+(0x40200 << 1))
#elif (ENABLE_MANHATTAN)
#define XHCI_PORT0_ADDR		(HW_BASE+0x90430)
#define MSTAR_U3TOP_BASE	(HW_BASE+(0x22500 << 1))
#elif (ENABLE_KASTOR)
#define XHCI_PORT0_ADDR		(HW_BASE+0xF0430)
#define MSTAR_U3TOP_BASE	(HW_BASE+(0x40200 << 1))
#endif

/* struct xhc_comp */
#define XHC_COMP_NONE		{0}
#define XHC_COMP_PORT0		{0, XHCI_PORT0_ADDR, MSTAR_U3TOP_BASE}
#define XHC_COMP_PORT1		{1, XHCI_PORT1_ADDR, MSTAR_U3TOP_BASE}


/* Register base mapping */
/* Port0 */
#define MSTAR_UTMI_BASE  (HW_BASE+(0x3A80 << 1))
#define MSTAR_USBC_BASE  (HW_BASE+(0x0700 << 1))
#define MSTAR_UHC_BASE   (HW_BASE+(0x2400 << 1))

#if (ENABLE_MONACO) || (ENABLE_MIAMI) || (ENABLE_MUJI) || (ENABLE_CELTICS) || \
	(ENABLE_MONET) || (ENABLE_MUNICH) || (ENABLE_MULAN) || \
	(ENABLE_MANHATTAN) || (ENABLE_MESSI) || (ENABLE_MASERATI) || (ENABLE_MACAN) || \
	(ENABLE_MOONEY) || (ENABLE_M7221) || (ENABLE_M7322)
	#define MSTAR_BC_BASE	(HW_BASE+(0x23600 << 1))
#elif (ENABLE_KRATOS) || (ENABLE_KIWI)
	#define MSTAR_BC_BASE	(HW_BASE+(0x20500 << 1))
#elif (ENABLE_KANO) || (ENABLE_CURRY) || (ENABLE_KASTOR)
	#define MSTAR_BC_BASE	(HW_BASE+(0x13700 << 1))
#else
	#define MSTAR_BC_BASE    0
#endif

#if 0
	#define MSTAR_INIT_FLAG     EHCFLAG_DPDM_SWAP
#else
	#define MSTAR_INIT_FLAG     EHCFLAG_NONE
#endif

#define MSTAR_COMP0			XHC_COMP_NONE

/* Port1 */
#if (ENABLE_MULAN)
	#define MSTAR_UTMI1_BASE  (HW_BASE+(0x10E80 << 1))
	#define MSTAR_USBC1_BASE  (HW_BASE+(0x13800 << 1))
	#define MSTAR_UHC1_BASE   (HW_BASE+(0x13900 << 1))
#else
	#define MSTAR_UTMI1_BASE  (HW_BASE+(0x3A00 << 1))
	#define MSTAR_USBC1_BASE  (HW_BASE+(0x0780 << 1))
	#define MSTAR_UHC1_BASE   (HW_BASE+(0x0D00 << 1))
#endif

#if (ENABLE_MONACO) || (ENABLE_MIAMI) || (ENABLE_MUJI) || (ENABLE_CELTICS) || \
	(ENABLE_MONET) || (ENABLE_MUNICH) || (ENABLE_MANHATTAN) || (ENABLE_MESSI) || (ENABLE_MASERATI) || \
	(ENABLE_MACAN) || (ENABLE_MOONEY) || (ENABLE_M7221) || (ENABLE_M7322)
	#define MSTAR_BC1_BASE	(HW_BASE+(0x23620 << 1))
#elif (ENABLE_MULAN)
	#define MSTAR_BC1_BASE	(HW_BASE+(0x23640 << 1))
#elif (ENABLE_KRATOS) || (ENABLE_KIWI)
	#define MSTAR_BC1_BASE	(HW_BASE+(0x20540 << 1))
#elif (ENABLE_KANO) || (ENABLE_CURRY)
	#define MSTAR_BC1_BASE	(HW_BASE+(0x13720 << 1))
#elif (ENABLE_KASTOR)
	#define MSTAR_BC1_BASE	(HW_BASE+(0x13740 << 1))
#else
	#define MSTAR_BC1_BASE	0
#endif

#if 0
	#define MSTAR_INIT1_FLAG	EHCFLAG_DPDM_SWAP
#else
	#define MSTAR_INIT1_FLAG	EHCFLAG_NONE
#endif

#define MSTAR_COMP1			XHC_COMP_NONE

/* Port2 */
#if (ENABLE_MUNICH)
	#define MSTAR_UTMI2_BASE  (HW_BASE+(0x10E80 << 1))
#elif (ENABLE_KASTOR)
	#define MSTAR_UTMI2_BASE  (HW_BASE+(0x3800 << 1))
#else
	#define MSTAR_UTMI2_BASE  (HW_BASE+(0x3900 << 1))
#endif

#if (ENABLE_KANO) || (ENABLE_CURRY)
	#define MSTAR_USBC2_BASE  (HW_BASE+(0x10200 << 1))
	#define MSTAR_UHC2_BASE   (HW_BASE+(0x10300 << 1))
#elif (ENABLE_KASTOR)
	#define MSTAR_USBC2_BASE  (HW_BASE+(0x40600 << 1))
	#define MSTAR_UHC2_BASE   (HW_BASE+(0x40400 << 1))
#else
	#define MSTAR_USBC2_BASE  (HW_BASE+(0x13800 << 1))
	#define MSTAR_UHC2_BASE   (HW_BASE+(0x13900 << 1))
#endif

#if (ENABLE_MONACO) || (ENABLE_MIAMI) || (ENABLE_MUJI) || (ENABLE_CELTICS) || \
	(ENABLE_MONET) || (ENABLE_MUNICH) || (ENABLE_MANHATTAN) || (ENABLE_MESSI) || (ENABLE_MASERATI) || \
	(ENABLE_MACAN) || (ENABLE_MOONEY) || (ENABLE_M7221) || (ENABLE_M7322)
	#define MSTAR_BC2_BASE	(HW_BASE+(0x23640 << 1))
#elif (ENABLE_KANO) || (ENABLE_CURRY)
	#define MSTAR_BC2_BASE	(HW_BASE+(0x13740 << 1))
#elif (ENABLE_KASTOR)
	#define MSTAR_BC2_BASE	(HW_BASE+(0x13780 << 1))
#else
	#define MSTAR_BC2_BASE	0
#endif

#if 0
	#define MSTAR_INIT2_FLAG     EHCFLAG_DPDM_SWAP
#elif (ENABLE_KASTOR)
	#define MSTAR_INIT2_FLAG     EHCFLAG_XHC_COMP
#else
	#define MSTAR_INIT2_FLAG     EHCFLAG_NONE
#endif

#if (ENABLE_KASTOR)
	#define MSTAR_COMP2     	XHC_COMP_PORT0
#else
#define MSTAR_COMP2			XHC_COMP_NONE
#endif

/* Port3 */
#if (ENABLE_MONACO) || (ENABLE_MIAMI) || (ENABLE_MUJI) || (ENABLE_CELTICS) || \
	(ENABLE_MASERATI)
	#define MSTAR_UTMI3_BASE	(HW_BASE+(0x22300 << 1))
	#define MSTAR_USBC3_BASE	(HW_BASE+(0x53200 << 1))
	#define MSTAR_UHC3_BASE		(HW_BASE+(0x53000 << 1))
	#define MSTAR_BC3_BASE		(HW_BASE+(0x23680 << 1))
#elif (ENABLE_MUNICH) // EHCI only
	#define MSTAR_UTMI3_BASE	(HW_BASE+(0x10E00 << 1))
	#define MSTAR_USBC3_BASE	(HW_BASE+(0x13880 << 1))
	#define MSTAR_UHC3_BASE		(HW_BASE+(0x10F00 << 1))
	#define MSTAR_BC3_BASE		(HW_BASE+(0x23660 << 1))
#elif (ENABLE_MANHATTAN) || (ENABLE_MACAN) || (ENABLE_M7221) || (ENABLE_M7322)
	#define MSTAR_UTMI3_BASE	(HW_BASE+(0x22080 << 1))
	#define MSTAR_USBC3_BASE	(HW_BASE+(0x13880 << 1))
	#define MSTAR_UHC3_BASE	(HW_BASE+(0x22600 << 1))
	#define MSTAR_BC3_BASE		(HW_BASE+(0x23660 << 1))
#elif (ENABLE_KANO)
	#define MSTAR_UTMI3_BASE	(HW_BASE+(0x03800 << 1))
	#define MSTAR_USBC3_BASE	(HW_BASE+(0x40600 << 1))
	#define MSTAR_UHC3_BASE		(HW_BASE+(0x40400 << 1))
	#define MSTAR_BC3_BASE		(HW_BASE+(0x13780 << 1))
#else
	/* deault: null Port3 base */
	#define MSTAR_UTMI3_BASE	(0x00000000)
	#define MSTAR_USBC3_BASE	(0x00000000)
	#define MSTAR_UHC3_BASE		(0x00000000)
	#define MSTAR_BC3_BASE		(0x00000000)
#endif

#if 0
	#define MSTAR_INIT3_FLAG     EHCFLAG_DPDM_SWAP
#elif (ENABLE_MONACO) || (ENABLE_MIAMI) || (ENABLE_MUJI) || (ENABLE_CELTICS) || \
	(ENABLE_KANO) || (ENABLE_MASERATI)
	#define MSTAR_INIT3_FLAG     EHCFLAG_XHC_COMP
#else
	#define MSTAR_INIT3_FLAG     EHCFLAG_NONE
#endif

#if (ENABLE_MONACO) || (ENABLE_MIAMI) || (ENABLE_MUJI) || (ENABLE_CELTICS) || \
	(ENABLE_KANO) || (ENABLE_MASERATI)
	#define MSTAR_COMP3     	XHC_COMP_PORT0
#else
	#define MSTAR_COMP3     	XHC_COMP_NONE
#endif


/* Port4 */
#if (ENABLE_MONACO) || (ENABLE_MIAMI) || (ENABLE_MUJI) || (ENABLE_CELTICS) || \
	(ENABLE_MASERATI)
	#define MSTAR_UTMI4_BASE	(HW_BASE+(0x22380 << 1))
	#define MSTAR_USBC4_BASE	(HW_BASE+(0x53280 << 1))
	#define MSTAR_UHC4_BASE		(HW_BASE+(0x53100 << 1))
	#define MSTAR_BC4_BASE		(HW_BASE+(0x236A0 << 1))
#elif (ENABLE_MANHATTAN)
	#define MSTAR_UTMI4_BASE	(HW_BASE+(0x22300 << 1))
	#define MSTAR_USBC4_BASE	(HW_BASE+(0x53200 << 1))
	#define MSTAR_UHC4_BASE		(HW_BASE+(0x53000 << 1))
	#define MSTAR_BC4_BASE		(HW_BASE+(0x23680 << 1))
#elif (ENABLE_KANO)
	#define MSTAR_UTMI4_BASE	(HW_BASE+(0x03880 << 1))
	#define MSTAR_USBC4_BASE	(HW_BASE+(0x40680 << 1))
	#define MSTAR_UHC4_BASE		(HW_BASE+(0x40500 << 1))
	#define MSTAR_BC4_BASE		(HW_BASE+(0x137A0 << 1))
#else
	/* deault: null Port3 base */
	#define MSTAR_UTMI4_BASE	(0x00000000)
	#define MSTAR_USBC4_BASE	(0x00000000)
	#define MSTAR_UHC4_BASE		(0x00000000)
	#define MSTAR_BC4_BASE		(0x00000000)
#endif

#if 0
	#define MSTAR_INIT4_FLAG     EHCFLAG_DPDM_SWAP
#elif (ENABLE_MONACO) || (ENABLE_MIAMI) || (ENABLE_MUJI) || (ENABLE_CELTICS) ||\
	(ENABLE_MANHATTAN) || (ENABLE_KANO) || (ENABLE_MASERATI)
	#define MSTAR_INIT4_FLAG     EHCFLAG_XHC_COMP
#else
	#define MSTAR_INIT4_FLAG     EHCFLAG_NONE
#endif

#if (ENABLE_MONACO) || (ENABLE_MIAMI) || (ENABLE_MUJI) || (ENABLE_CELTICS) || \
	(ENABLE_KANO) || (ENABLE_MASERATI)
	#define MSTAR_COMP4     	XHC_COMP_PORT1
#elif (ENABLE_MANHATTAN)
	#define MSTAR_COMP4     	XHC_COMP_PORT0
#else
	#define MSTAR_COMP4     	XHC_COMP_NONE
#endif


/* Port5 */
#if 0
	#define MSTAR_UTMI5_BASE	(HW_BASE+(0x << 1))
	#define MSTAR_USBC5_BASE	(HW_BASE+(0x << 1))
	#define MSTAR_UHC5_BASE		(HW_BASE+(0x << 1))
	#define MSTAR_BC5_BASE		(HW_BASE+(0x << 1))
#else
	/* deault: null Port5 base */
	#define MSTAR_UTMI5_BASE	(0x00000000)
	#define MSTAR_USBC5_BASE	(0x00000000)
	#define MSTAR_UHC5_BASE		(0x00000000)
	#define MSTAR_BC5_BASE		(0x00000000)
#endif

#if 0
	#define MSTAR_INIT5_FLAG     EHCFLAG_DPDM_SWAP
#else
	#define MSTAR_INIT5_FLAG     EHCFLAG_NONE
#endif

#define MSTAR_COMP5 		XHC_COMP_NONE


#endif /* __PLATFORM_H__*/
