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

#include <asm/byteorder.h>

#include <platform.h>
#include "inc/common.h"
#include "inc/drvhostlib.h"
#include "inc/ubootglue.h"

#ifdef USB_GLUE_DEBUG
#define USB_GLUE_PRINTF MS_MSG
#else
#define USB_GLUE_PRINTF(fmt, ...)
#endif

/* root hub port status */
static struct usb_port_status rh_status = { 0 };
extern int UsbPortSelect;

#define MAX_ROOT_PORTS 8 // depends on chip
struct usb_hcd array_usb_root_port[MAX_ROOT_PORTS] =
{
	{0, MSTAR_UTMI_BASE,MSTAR_UHC_BASE, MSTAR_USBC_BASE, MSTAR_BC_BASE, MSTAR_INIT_FLAG, 0, MSTAR_COMP0},
	{1, MSTAR_UTMI1_BASE,MSTAR_UHC1_BASE, MSTAR_USBC1_BASE, MSTAR_BC1_BASE, MSTAR_INIT1_FLAG, 0, MSTAR_COMP1},
	{2, MSTAR_UTMI2_BASE,MSTAR_UHC2_BASE, MSTAR_USBC2_BASE, MSTAR_BC2_BASE, MSTAR_INIT2_FLAG, 0, MSTAR_COMP2},
	{3, MSTAR_UTMI3_BASE,MSTAR_UHC3_BASE, MSTAR_USBC3_BASE, MSTAR_BC3_BASE, MSTAR_INIT3_FLAG, 0, MSTAR_COMP3},
	{4, MSTAR_UTMI4_BASE,MSTAR_UHC4_BASE, MSTAR_USBC4_BASE, MSTAR_BC4_BASE, MSTAR_INIT4_FLAG, 0, MSTAR_COMP4},
	{5, MSTAR_UTMI5_BASE,MSTAR_UHC5_BASE, MSTAR_USBC5_BASE, MSTAR_BC5_BASE, MSTAR_INIT5_FLAG, 0, MSTAR_COMP5},
};

int FirstBulkIn,FirstBulkOut; // for compile

static int gWaitConnectTime = USB_WAIT_TIME;

// USB_Set_WaitTime unit:ms
void USB_Set_WaitTime(unsigned int u32_ms)
{
	gWaitConnectTime = u32_ms;
}
// USB_Get_WaitTime unit:ms
unsigned int USB_Get_WaitTime(void)
{
	return gWaitConnectTime;
}


#if defined(__ARM__)
static u32 miu0_bus_base_addr;
#if defined(MIU1_BUS_BASE_ADDR)
static u32 miu1_bus_base_addr;
#endif
#if defined(MIU2_BUS_BASE_ADDR)
static u32 miu2_bus_base_addr;
#endif

unsigned int VA2PA(unsigned int u32_DMAAddr)
{
	u32_DMAAddr = MsOS_VA2PA(u32_DMAAddr);

#if defined(MIU1_BUS_BASE_ADDR)
	if(u32_DMAAddr < miu1_bus_base_addr)
	{
		u32_DMAAddr -= miu0_bus_base_addr;
		u32_DMAAddr += MIU0_PHY_BASE_ADDR;
	}
#if defined(MIU2_BUS_BASE_ADDR)
	else if(u32_DMAAddr < miu2_bus_base_addr)
	{
		u32_DMAAddr -= miu1_bus_base_addr;
		u32_DMAAddr |= MIU1_PHY_BASE_ADDR;
	}
	else
	{
		u32_DMAAddr -= miu2_bus_base_addr;
		u32_DMAAddr |= MIU2_PHY_BASE_ADDR;
	}
#else
	else
	{
		u32_DMAAddr -= miu1_bus_base_addr;
		u32_DMAAddr |= MIU1_PHY_BASE_ADDR;
	}
#endif
#else
	u32_DMAAddr -= miu0_bus_base_addr;
	u32_DMAAddr += MIU0_PHY_BASE_ADDR;
#endif
	return u32_DMAAddr;
}

unsigned int PA2VA(unsigned int u32_DMAAddr)
{
#if defined(MIU1_BUS_BASE_ADDR)
	if(u32_DMAAddr < MIU1_PHY_BASE_ADDR)
	{
		u32_DMAAddr -= MIU0_PHY_BASE_ADDR;
		u32_DMAAddr += miu0_bus_base_addr;
	}
#if defined(MIU2_BUS_BASE_ADDR)
	else if(u32_DMAAddr < MIU2_PHY_BASE_ADDR)
	{
		u32_DMAAddr -= MIU1_PHY_BASE_ADDR;
		u32_DMAAddr |= miu1_bus_base_addr;
	}
	else
	{
		u32_DMAAddr -= MIU2_PHY_BASE_ADDR;
		u32_DMAAddr |= miu2_bus_base_addr;
	}
#else
	else
	{
		u32_DMAAddr -= MIU1_PHY_BASE_ADDR;
		u32_DMAAddr |= miu1_bus_base_addr;
	}
#endif
#else
	u32_DMAAddr -= MIU0_PHY_BASE_ADDR;
	u32_DMAAddr += miu0_bus_base_addr;
#endif
	u32_DMAAddr=MsOS_PA2KSEG1(u32_DMAAddr); // mapping to uncache address

	return  u32_DMAAddr;
}
#endif

static void usb_bc_enable(u32 utmi_base, u32 bc_base, u8 enable)
{
	if (!bc_base)
		return;

	if (enable)
	{
		//printf("BC enable \n");
		writeb(readb((void *)(utmi_base+(0x1*2-1))) | 0x40, (void *)(utmi_base+(0x1*2-1)));  //IREF_PDN=1¡¦b1. (utmi+0x01[6] )
		writeb(readb((void *)(bc_base+(0x3*2-1))) | 0x40, (void *)(bc_base+(0x3*2-1)));  // [6]= reg_host_bc_en
		writeb(readb((void *)(bc_base+(0xc*2))) | 0x40, (void *)(bc_base+(0xc*2)));  // [6]= reg_into_host_bc_sw_tri
		writeb(0x00, (void *)(bc_base));  // [15:0] = bc_ctl_ov_en
		writeb(0x00, (void *)(bc_base+(0x1*2-1)));  // [15:0] = bc_ctl_ov_en
		writeb(readb((void *)(bc_base+(0xa*2))) | 0x80, (void *)(bc_base+(0xa*2)));  // [7]=reg_bc_switch_en
	}
	else
	{
		// disable BC
		//printf("BC disable \n");
		writeb(readb((void *)(bc_base+(0xc*2))) & (~0x40), (void *)(bc_base+(0xc*2)));  // [6]= reg_into_host_bc_sw_tri
		writeb(readb((void *)(bc_base+(0x3*2-1))) & (~0x40), (void *)(bc_base+(0x3*2-1)));  // [6]= reg_host_bc_en
		writeb(readb((void *)(utmi_base+(0x1*2-1))) & (~0x40), (void *)(utmi_base+(0x1*2-1)));  //IREF_PDN=1¡¦b1. (utmi+0x01[6] )
	}
}

void xhci_ppc(struct xhc_comp *xhci, int	bOn)
{
	u16 addr_w, bit_num;
	u32 addr, gpio_addr;
	u8  value, low_active;

	switch (xhci->port_index) {
	case 0:
		addr_w = readw((void*)(xhci->u3top_base+0xFC*2));
		addr = (u32)addr_w << 8;
		addr_w = readw((void*)(xhci->u3top_base+0xFE*2));
		addr |= addr_w & 0xFF;
		bit_num = (addr_w >> 8) & 0x7;
		low_active = (u8)((addr_w >> 11) & 0x1);
		break;
	case 1:
		addr_w = readw((void*)(xhci->u3top_base+0xE6*2));
		addr = (u32)addr_w << 8;
		addr_w = readw((void*)(xhci->u3top_base+0xE8*2));
		addr |= addr_w & 0xFF;
		bit_num = (addr_w >> 8) & 0x7;
		low_active = (u8)((addr_w >> 11) & 0x1);
		break;
	default:	/* "can't happen" */
		return;
	}

	if (addr)
	{
		printf("xhci_ppc: turn %s USB3.0 port %d power \n", (bOn) ? "on" : "off", xhci->port_index);
		printf("Addr: 0x%x bit_num: %d low_active:%d\n", addr, bit_num, low_active);

		value = (u8)(1 << bit_num);
		if (addr & 0x1)
			gpio_addr = _MSTAR_PM_BASE+addr*2-1;
		else
			gpio_addr = _MSTAR_PM_BASE+addr*2;

		if (low_active^bOn)
		{
			writeb(readb((void*)gpio_addr) | value, (void*)gpio_addr);
		}
		else
		{
			writeb(readb((void*)gpio_addr) & (u8)(~value), (void*)gpio_addr);
		}
	}
	else {
		printf("\n\n!!!! ERROR : xhci: no GPIO information for vbus port power control  !!!! \n\n");
		return;
	}


}


#if defined(ENABLE_USB_NEW_MIU_SEL)
void MIU_select_setting_ehc(unsigned int USBC_base)
{
	printf("[USB] config miu select [%x] [%x] [%x] ][%x]\n", USB_MIU_SEL0, USB_MIU_SEL1, USB_MIU_SEL2, USB_MIU_SEL3);
	writeb(USB_MIU_SEL0, (void*)(USBC_base+0x14*2));        //Setting MIU0 segment
	writeb(USB_MIU_SEL1, (void*)(USBC_base+0x16*2));        //Setting MIU1 segment
	writeb(USB_MIU_SEL2, (void*)(USBC_base+0x17*2-1));      //Setting MIU2 segment
	writeb(USB_MIU_SEL3, (void*)(USBC_base+0x18*2));        //Setting MIU3 segment
	writeb(readb((void*)(USBC_base+0x19*2-1)) | 0x01, (void*)(USBC_base+0x19*2-1)); //Enable miu partition mechanism
#if !defined(DISABLE_MIU_LOW_BOUND_ADDR_SUBTRACT_ECO)
	printf("[USB] enable miu lower bound address subtraction\n");
	writeb(readb((void*)(USBC_base+0x0F*2-1)) | 0x01, (void*)(USBC_base+0x0F*2-1));
#endif
}
#endif

void TV_usb_init(struct usb_hcd *hcd)
{
	USB_GLUE_PRINTF("[USB] TV_usb_init (UTMI Init) ++\n");
	USB_GLUE_PRINTF("[USB] UTMI Base %8x\n", hcd->utmi_base);
	USB_GLUE_PRINTF("[USB] UHC Base %8x\n", hcd->ehc_base);
	USB_GLUE_PRINTF("[USB] USBC Base %8x\n", hcd->usbc_base);
	USB_GLUE_PRINTF("[USB] BC Base %8x\n", hcd->bc_base);

#if defined(ENABLE_USB_NEW_MIU_SEL)
	MIU_select_setting_ehc(hcd->usbc_base);
#endif
	/* force DP/DM pull down while HC reset: set */
	writeb(readb(hcd->utmi_base) | 0xc2, hcd->utmi_base);
	/* new HW term overwrite: on */
	writeb(readb(hcd->utmi_base+0x52*2) | (MS_BIT5|MS_BIT4|
		MS_BIT3|MS_BIT2|MS_BIT1|MS_BIT0), hcd->utmi_base+0x52*2);

	writeb(0x0A, hcd->usbc_base);	/* Disable MAC initial suspend, Reset UHC */
	writeb(0x28, hcd->usbc_base);	/* Release UHC reset, enable UHC XIU function */

	/* Init UTMI squelch level setting before CA */
	if(UTMI_DISCON_LEVEL_2A & (MS_BIT3|MS_BIT2|MS_BIT1|MS_BIT0))
	{
		writeb((UTMI_DISCON_LEVEL_2A & (MS_BIT3|MS_BIT2|MS_BIT1|MS_BIT0)),
			(void*)(hcd->utmi_base+0x2a*2));
		USB_GLUE_PRINTF("[USB] init squelch level 0x%x\n", readb((void*)(hcd->utmi_base+0x2a*2)));
	}

	writeb(readb(hcd->utmi_base+0x3C*2) | 0x01, hcd->utmi_base+0x3C*2);	/* set CA_START as 1 */

	mdelay(1); // 10->1

	writeb(readb(hcd->utmi_base+0x3C*2) & ~0x01, hcd->utmi_base+0x3C*2);	/* release CA_START */
#if defined(ENABLE_HS_DM_KEEP_HIGH_ECO)
	writeb(readb((void*)hcd->utmi_base+0x10*2) | 0x40, (void*) (hcd->utmi_base+0x10*2)); // bit<6> for monkey test and HS current
#endif
	while (((unsigned int)(readb(hcd->utmi_base+0x3C*2)) & 0x02) == 0);	/* polling bit <1> (CA_END) */

	if (hcd->init_flag & EHCFLAG_DPDM_SWAP)
		writeb(readb(hcd->utmi_base+0x0b*2-1) |0x20, hcd->utmi_base+0x0b*2-1);	/* dp dm swap */

#if defined(ENABLE_UTMI_DPDM_SWAP_P1_FOR_MARLON_P2P_MILAN)
	if (hcd->port_index == 1) {
		/* Marlon may share the same HAL as Milan, so we need to check chip ID register */
		if (MDrv_SYS_GetChipID() == 0x00C8) {
			/* Marlon need to swap DP/DM for P2P Milan 216/256-pin */
			if ((readb((void*)(_MSTAR_BONDING_OPTION_BASE+0xC1*2-1)) & 0xC0) == 0xC0) {
				writeb(readb(hcd->utmi_base+0x0b*2-1) |0x20, hcd->utmi_base+0x0b*2-1);	/* dp dm swap */
			}
		}
	}
#endif
#if defined(ENABLE_UTMI_DPDM_SWAP_P1_FOR_MARLON_NEW_216PIN_TCON)
	if (hcd->port_index == 1) {
		/* Marlon may share the same HAL as Milan, so we need to check chip ID register */
		if (MDrv_SYS_GetChipID() == 0x00C8) {
			/* Marlon need to swap DP/DM for new 216-pin (TCON) */
			if ((readb((void*)(_MSTAR_BONDING_OPTION_BASE+0xC1*2-1)) & 0xC0) == 0x00) {
				writeb(readb(hcd->utmi_base+0x0b*2-1) |0x20, hcd->utmi_base+0x0b*2-1);	/* dp dm swap */
			}
		}
	}
#endif
	writeb(readb(hcd->usbc_base+0x02*2) & ~0x03, hcd->usbc_base+0x02*2);	/* UHC select enable */
	writeb(readb(hcd->usbc_base+0x02*2) | 0x01, hcd->usbc_base+0x02*2);	/* UHC select enable */

	writeb(readb(hcd->ehc_base+0x40*2) & ~0x10, hcd->ehc_base+0x40*2);	/* 0: VBUS On. */
	udelay(1);	/* delay 1us */
	writeb(readb(hcd->ehc_base+0x40*2) | 0x08, hcd->ehc_base+0x40*2);	/* Active HIGH */

	/* force DP/DM pull down while HC reset: release */
	writeb(readb(hcd->utmi_base) & ~0xc2, hcd->utmi_base);
	/* new HW term overwrite: off */
	writeb(readb(hcd->utmi_base+0x52*2) & ~(MS_BIT5|MS_BIT4|
		MS_BIT3|MS_BIT2|MS_BIT1|MS_BIT0), hcd->utmi_base+0x52*2);

	writeb((readb(hcd->utmi_base+0x06*2) & 0x9F) | 0x40, hcd->utmi_base+0x06*2);	/* reg_tx_force_hs_current_enable */

	writeb(readb(hcd->utmi_base+0x03*2-1) | 0x28, hcd->utmi_base+0x03*2-1);	/* Disconnect window select */
	writeb(readb(hcd->utmi_base+0x03*2-1) & 0xef, hcd->utmi_base+0x03*2-1);	/* Disconnect window select */
	writeb(readb(hcd->utmi_base+0x07*2-1) & 0xfd, hcd->utmi_base+0x07*2-1);	/* Disable improved CDR */

#if defined(ENABLE_UTMI_240_AS_120_PHASE_ECO)
	#if defined(UTMI_240_AS_120_PHASE_ECO_INV)
	writeb(readb((void*)hcd->utmi_base+0x08*2) & ~0x08, (void*) (hcd->utmi_base+0x08*2)); // bit<3> special for Eiffel analog LIB issue
	#elif defined(UTMI_240_AS_120_PHASE_ECO_INV_IF_REV_0)
	if (MDrv_SYS_GetChipRev() == 0x0) {
		writeb(readb((void*)hcd->utmi_base+0x08*2) & ~0x08, (void*) (hcd->utmi_base+0x08*2)); // bit<3> special for Eiffel analog LIB issue
	}
	#else
	writeb(readb(hcd->utmi_base+0x08*2) | 0x08, hcd->utmi_base+0x08*2);	/* bit<3> for 240's phase as 120's clock set 1, bit<4> for 240Mhz in mac 0 for faraday 1 for etron */
	#endif
#endif

	writeb(readb(hcd->utmi_base+0x09*2-1) | 0x81, hcd->utmi_base+0x09*2-1);	/* UTMI RX anti-dead-loc, ISI effect improvement */
#if defined(UTMI_TX_TIMING_SEL_LATCH_PATH_INV_IF_REV_0)
	if (MDrv_SYS_GetChipRev() == 0x0) {
		writeb(readb(hcd->utmi_base+0x0b*2-1) & ~0x80, hcd->utmi_base+0x0b*2-1);	/* TX timing select latch path */
	}
#else
	writeb(readb(hcd->utmi_base+0x0b*2-1) | 0x80, hcd->utmi_base+0x0b*2-1);	/* TX timing select latch path */
#endif
	writeb(readb(hcd->utmi_base+0x15*2-1) | 0x20, hcd->utmi_base+0x15*2-1);	/* Chirp signal source select */
#if defined(ENABLE_UTMI_55_INTERFACE)
	writeb(readb(hcd->utmi_base+0x15*2-1) | 0x40, hcd->utmi_base+0x15*2-1);	/* change to 55 interface */
#endif

#if defined(ENABLE_PV2MI_BRIDGE_ECO)
	writeb(readb((hcd->usbc_base+0x0a*2)) | 0x40, hcd->usbc_base+0x0a*2);
#endif
	/* Init UTMI eye diagram parameter setting */
	writeb(readb(hcd->utmi_base+0x2c*2) | UTMI_EYE_SETTING_2C, hcd->utmi_base+0x2c*2);
	writeb(readb(hcd->utmi_base+0x2d*2-1) | UTMI_EYE_SETTING_2D, hcd->utmi_base+0x2d*2-1);
	writeb(readb(hcd->utmi_base+0x2e*2) | UTMI_EYE_SETTING_2E, hcd->utmi_base+0x2e*2);
	writeb(readb(hcd->utmi_base+0x2f*2-1) | UTMI_EYE_SETTING_2F, hcd->utmi_base+0x2f*2-1);

#if _USB_MIU_WRITE_WAIT_LAST_DONE_Z_PATCH
	/* Enabe PVCI i_miwcplt wait for mi2uh_last_done_z */
	writeb(readb(hcd->ehc_base+0x83*2-1) | MS_BIT4, hcd->ehc_base+0x83*2-1);
#endif

	usb_bc_enable(hcd->utmi_base, hcd->bc_base, FALSE);

	USB_GLUE_PRINTF ("[USB] TV_usb_init--\n");
}

int Usb_host_Init(struct usb_hcd *hcd, unsigned int delay)
{
	int usb_retry_cnt = USB_WAIT_LOOP_COUNT;

	USB_GLUE_PRINTF ("[USB] Usb_host_Init++\n");

	hcd->ever_inited = 1;

	/* Data length byte alignment issue */
	flib_Host20_Fix_DataLength_ByteAlign(hcd);

usb_device_detect_retry:
	if (flib_Host20_Init(hcd, 0, delay)==0)
	{
		if (usb_retry_cnt > 0)
		{
			usb_retry_cnt--;
			MS_MSG("[USB] waiting for connecting...%d\n", usb_retry_cnt+1);
			goto usb_device_detect_retry;
		}
		MS_MSG("[USB] USB init failed\n");
		return -1;
	}

	USB_GLUE_PRINTF ("[USB] Usb_host_Init--\n");
	return hcd->bSpeed;
}

int _usb_lowlevel_init(int entry)
{
	int speed = -1;
	int ret = 0;
	struct usb_hcd *hcd = &array_usb_root_port[UsbPortSelect];

#if defined(__ARM__)
	miu0_bus_base_addr = MsOS_VA2PA(MIU0_BUS_BASE_ADDR);
	#if defined(MIU1_BUS_BASE_ADDR)
	miu1_bus_base_addr = MsOS_VA2PA(MIU1_BUS_BASE_ADDR);
	#endif
	#if defined(MIU2_BUS_BASE_ADDR)
	miu2_bus_base_addr = MsOS_VA2PA(MIU2_BUS_BASE_ADDR);
	#endif
#endif

	USB_GLUE_PRINTF ("[USB] usb_lowlevel_init++\n");

	MS_MSG ("[USB] USB EHCI LIB VER: %s\n", USBHOST_LIB_VER);
	MS_MSG ("[USB] Port %d is Enabled\n", UsbPortSelect);

#if defined(CONFIG_USB_DISK_UPGRADE_LOADER) && defined(ENABLE_USB_DISK_UPGRADE_EMI_SWITCH)
	if (UsbPortSelect == 0)
		usb_port0_emi_switch();
#endif

	TV_usb_init(hcd);

	if (hcd->init_flag & EHCFLAG_XHC_COMP)
	{
		//enable xHCI clock
		xhci_enable_clock();
		//disable port
		xhci_ssport_set_state(&hcd->xhci, false);
		//turn on power
		xhci_ppc(&hcd->xhci, true);
	}

	if (entry == 1)
		goto port_status;

	speed = (entry == 2) ? Usb_host_Init(hcd, 0) :
		Usb_host_Init(hcd, USB_Get_WaitTime()) ;
port_status:
	if (speed == 1)
	{
		rh_status.wPortStatus |= USB_PORT_STAT_CONNECTION | USB_PORT_STAT_LOW_SPEED;
	}
	else
	{
		rh_status.wPortStatus |= USB_PORT_STAT_CONNECTION;
		rh_status.wPortStatus &= ~USB_PORT_STAT_LOW_SPEED;

		if(speed == -1)
			ret = -1;
	}

	rh_status.wPortChange |= USB_PORT_STAT_C_CONNECTION;
	USB_GLUE_PRINTF ("[USB] usb_lowlevel_init--\n");
	return ret;
}

inline int usb_lowlevel_init(void)
{
	return _usb_lowlevel_init(0);
}

inline int usb_lowlevel_preinit(void)
{
	return _usb_lowlevel_init(1);
}

inline int usb_lowlevel_postinit(void)
{
	return _usb_lowlevel_init(2);
}

int usb_lowlevel_stop(int p)
{
	struct usb_hcd *hcd = &array_usb_root_port[p];

	if(hcd->ever_inited == 0)
		return 0;

	flib_Host20_StopRun_Setting(hcd, HOST20_Disable);

	/* reset HC to release 45ohm HS term */
	flib_Host20_Reset_HC(hcd);

	if (hcd->init_flag & EHCFLAG_XHC_COMP)
	{
		//turn off power
		xhci_ppc(&hcd->xhci, false);
		//enable port
		xhci_ssport_set_state(&hcd->xhci, true);
		mdelay(1000);
	}

	if (hcd->ever_inited == 2)
	{
		MS_MSG("[USB] Long time stable device. Waiting .5 second...\n");
		mdelay(500);
	}

	return 0;
}

inline void Usb_host_SetEverInited(int val)
{
	struct usb_hcd *hcd = &array_usb_root_port[UsbPortSelect];

	hcd->ever_inited = val;
}

inline void SetControl1MaxPacketEx(MS_UINT8 max, int port)
{
	struct usb_hcd *hcd = &array_usb_root_port[UsbPortSelect];

	flib_Host20_SetControl1MaxPacket(hcd, max);
}

inline void USB_Bulk_InitEx(struct usb_device *dev, int port)
{
	struct usb_hcd *hcd = &array_usb_root_port[UsbPortSelect];

	hcd->FirstBulkIn = 1;
	hcd->FirstBulkOut = 1;
#if defined(USB_LEGEND_DATA_TOGGLE_METHOD) && !defined(ENABLE_DYNAMIC_BULK_QH_MOUNT_PATCH)
	FirstBulkIn = 1;
	FirstBulkOut = 1;
#endif
	/* report device speed */
	dev->speed = hcd->bSpeed;
#if defined(ENABLE_DYNAMIC_BULK_QH_MOUNT_PATCH)
	printf("[USB] init dynamic bulk QH\n");
	hcd->ehci->cur_pipe = ~0x0;
	hcd->ehci->FirstBulkInMap = ~0x0;
	hcd->ehci->FirstBulkOutMap = ~0x0;
#else
	flib_Host20_Bulk_Init(dev, hcd);
#endif
}

inline void USB_Interrupt_InitEx(struct usb_device *dev, int port)
{
	struct usb_hcd *hcd = &array_usb_root_port[UsbPortSelect];

	hcd->FirstIntIn = 0;
	hcd->IntrIn_Complete = 0;
	/* report device speed */
	dev->speed = hcd->bSpeed;
	flib_Host20_Interrupt_Init(dev, hcd);
}

int submit_bulk_msg(struct usb_device *dev, unsigned long pipe, void *buffer, int len)
{
	int dir_out = usb_pipeout(pipe);
	//int done = 0;
	struct usb_hcd *hcd = &array_usb_root_port[UsbPortSelect];

#if defined(ENABLE_DYNAMIC_BULK_QH_MOUNT_PATCH)
	/* QH switch update FirstBulkIn, data toggle .. */
	flib_Host20_Bulk_Update(dev, hcd, pipe);
#endif

	/* deal with bulk endpoint data toggle */
#if defined(USB_LEGEND_DATA_TOGGLE_METHOD) && !defined(ENABLE_DYNAMIC_BULK_QH_MOUNT_PATCH)
	hcd->FirstBulkIn = (FirstBulkIn != hcd->FirstBulkIn) ? FirstBulkIn : hcd->FirstBulkIn;
	hcd->FirstBulkOut = (FirstBulkOut != hcd->FirstBulkOut) ? FirstBulkOut : hcd->FirstBulkOut;
#endif
	dev->status = 0;

	if (flib_Host20_Send_Receive_Bulk_Data(hcd, buffer,len,&dev->act_len,dir_out)>0)
	{
		dev->status = hcd->urb_status;
		return -1;
	}

	//dev->act_len = done;

	/* deal with bulk endpoint data toggle */
#if defined(USB_LEGEND_DATA_TOGGLE_METHOD) && !defined(ENABLE_DYNAMIC_BULK_QH_MOUNT_PATCH)
	FirstBulkIn = (FirstBulkIn == hcd->FirstBulkIn) ? FirstBulkIn : hcd->FirstBulkIn;
	FirstBulkOut = (FirstBulkOut == hcd->FirstBulkOut) ? FirstBulkOut : hcd->FirstBulkOut;
#endif
	return 0;
}

int submit_control_msg(struct usb_device *dev, unsigned long pipe, void *buffer,
		int len,struct devrequest *setup)
{
	int done = 0;
	struct usb_hcd *hcd = &array_usb_root_port[UsbPortSelect];

	dev->status = 0;

	if ( flib_Host20_Issue_Control (hcd, dev->devnum,(unsigned char*) setup,len,(unsigned char*) buffer) > 0)
	{
		dev->status = hcd->urb_status;
		done=-1;
	}

	/* status phase */
	dev->act_len = len;

	/* deal with bulk endpoint data toggle */
#if defined(ENABLE_DYNAMIC_BULK_QH_MOUNT_PATCH)
	/* clear feature: endpoint stall */
	if ((setup->request == USB_REQ_CLEAR_FEATURE) && (setup->requesttype == USB_RECIP_ENDPOINT))
	{
		if (le16_to_cpu(setup->index) & USB_DIR_IN) {
			hcd->ehci->FirstBulkInMap =
				hcd->ehci->FirstBulkInMap | (0x1 << (le16_to_cpu(setup->index) & 0xf));
		} else {
			hcd->ehci->FirstBulkOutMap =
				hcd->ehci->FirstBulkOutMap | (0x1 << (le16_to_cpu(setup->index) & 0xf));
		}
	}
#elif !defined(USB_LEGEND_DATA_TOGGLE_METHOD)
	/* clear feature: endpoint stall */
	if ((setup->request == USB_REQ_CLEAR_FEATURE) && (setup->requesttype == USB_RECIP_ENDPOINT))
	{
		if (pipe & USB_DIR_IN)
			hcd->FirstBulkIn = 1;
		else
			hcd->FirstBulkOut = 1;
	}
#endif
	return done;
}

u8 usb_bulk_transfer_in_ex(struct usb_device *dev, unsigned int pipe,
            MS_U32 buf, MS_U32 length, MS_U32 *actual_len, int timeout)
{
	struct usb_hcd *hcd = &array_usb_root_port[UsbPortSelect];

	USB_GLUE_PRINTF("usb_bulk_transfer_in_ex: pipe:%x\n", pipe);
	if (flib_Host20_Get_BulkIn_Data(hcd, dev, pipe, buf, length, actual_len, timeout)>0)
	{
		return -1;
	}

	if(dev->status==0)
		return 0;
	else
		return -1;
}

int submit_int_msg(struct usb_device *dev, unsigned long pipe, void *buffer, int len, int interval)
{
	MS_UINT32 buf, act_len=0;
	struct usb_hcd *hcd = &array_usb_root_port[UsbPortSelect];
#if 0
	if (flib_Host20_Issue_Interrupt(hcd, buf /*(MS_UINT32)pUsbIntrData*/, 8)>0)
	   return -1;

	return 0;
#else
	dev->status=0;
	dev->act_len = 0;
	buf=(MS_UINT32)buffer;
	do {
		  if (flib_Host20_Issue_Interrupt_NonBlock(hcd,  buf, len, &act_len)>0)
		  {
			printf("Send INTR fail\n");
			dev->status = hcd->urb_status;
			return -1;
		  }
		  dev->status = hcd->urb_status;
		  if (hcd->IntrIn_Complete)
		  {
			//USB_GLUE_PRINTF("Get INTR success:%d, %d\n", len, act_len);
			dev->act_len = act_len;
			return 0;
		  }
		  wait_ms(1);
		  interval--;
	}while(interval);
#endif
	USB_GLUE_PRINTF("submit_int_msg time out\n");
	return 0;
}
