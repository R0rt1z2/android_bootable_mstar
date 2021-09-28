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

//-------------------------------------------------------------------------------------------------
//  Include files
//-------------------------------------------------------------------------------------------------
//#include <linux/mii.h>
//#include <linux/delay.h>
//include <linux/netdevice.h>
//#include <linux/ethtool.h>
//#include <linux/pci.h>
#include <asm/types.h>
#include "mhal_gmac.h"

//-------------------------------------------------------------------------------------------------
//  Data structure
//-------------------------------------------------------------------------------------------------
struct _MHalBasicConfigEMAC
{
    u8                  connected;          // 0:No, 1:Yes    <== (20070515) Wait for Julian's reply
	u32					speed;				// 10:10Mbps, 100:100Mbps
	// ETH_CTL Register:
	u8                  wes;				// 0:Disable, 1:Enable (WR_ENABLE_STATISTICS_REGS)
	// ETH_CFG Register:
	u8					duplex;				// 0:Half-duplex, 1:Full-duplex
	u8					cam;                // 0:No CAM, 1:Yes
	u8 					rcv_bcast;      	// 0:No, 1:Yes
	u8					rlf;                // 0:No, 1:Yes receive long frame(1522)
	// MAC Address:
	u8					sa1[6];				// Specific Addr 1 (MAC Address)
	u8					sa2[6];				// Specific Addr 2
	u8					sa3[6];				// Specific Addr 3
	u8					sa4[6];				// Specific Addr 4
};
typedef struct _MHalBasicConfigEMAC MHalBasicConfigEMAC;

struct _MHalUtilityVarsEMAC
{
    u32                 cntChkCableConnect;
    u32                 cntChkINTCounter;
	u32 				readIdxRBQP;		// Reset = 0x00000000
	u32 				rxOneFrameAddr;		// Reset = 0x00000000 (Store the Addr of "ReadONE_RX_Frame")

	u8                  flagISR_INT_DONE;
};
typedef struct _MHalUtilityVarsEMAC MHalUtilityVarsEMAC;

MHalBasicConfigEMAC MHalThisBCE;
MHalUtilityVarsEMAC MHalThisUVE;

typedef volatile unsigned int EMAC_REG;

typedef struct _TITANIA_EMAC {
// Constant: ----------------------------------------------------------------
// Register MAP:
  EMAC_REG   REG_EMAC_CTL_L; 	      //0x00000000 Network Control Register Low  16 bit
  EMAC_REG   REG_EMAC_CTL_H; 	      //0x00000004 Network Control Register High 16 bit
  EMAC_REG   REG_EMAC_CFG_L; 	      //0x00000008 Network Configuration Register Low  16 bit
  EMAC_REG   REG_EMAC_CFG_H; 	      //0x0000000C Network Configuration Register High 16 bit
  EMAC_REG   REG_EMAC_SR_L; 	      //0x00000010 Network Status Register Low  16 bit
  EMAC_REG   REG_EMAC_SR_H; 	      //0x00000014 Network Status Register High 16 bit
  EMAC_REG   REG_EMAC_TAR_L; 	      //0x00000018 Transmit Address Register Low  16 bit
  EMAC_REG   REG_EMAC_TAR_H; 	      //0x0000001C Transmit Address Register High 16 bit
  EMAC_REG   REG_EMAC_TCR_L; 	      //0x00000020 Transmit Control Register Low  16 bit
  EMAC_REG   REG_EMAC_TCR_H; 	      //0x00000024 Transmit Control Register High 16 bit
  EMAC_REG   REG_EMAC_TSR_L; 	      //0x00000028 Transmit Status Register Low  16 bit
  EMAC_REG   REG_EMAC_TSR_H; 	      //0x0000002C Transmit Status Register High 16 bit
  EMAC_REG   REG_EMAC_RBQP_L;         //0x00000030 Receive Buffer Queue Pointer Low  16 bit
  EMAC_REG   REG_EMAC_RBQP_H;         //0x00000034 Receive Buffer Queue Pointer High 16 bit
  EMAC_REG   REG_EMAC_RBCFG_L;        //0x00000038 Receive buffer configuration Low  16 bit
  EMAC_REG   REG_EMAC_RBCFG_H;        //0x0000003C Receive buffer configuration High 16 bit
  EMAC_REG   REG_EMAC_RSR_L; 	      //0x00000040 Receive Status Register Low  16 bit
  EMAC_REG   REG_EMAC_RSR_H; 	      //0x00000044 Receive Status Register High 16 bit
  EMAC_REG   REG_EMAC_ISR_L;          //0x00000048 Interrupt Status Register Low  16 bit
  EMAC_REG   REG_EMAC_ISR_H;          //0x0000004C Interrupt Status Register High 16 bit
  EMAC_REG   REG_EMAC_IER_L;          //0x00000050 Interrupt Enable Register Low  16 bit
  EMAC_REG   REG_EMAC_IER_H;          //0x00000054 Interrupt Enable Register High 16 bit
  EMAC_REG   REG_EMAC_IDR_L; 	      //0x00000058 Interrupt Disable Register Low  16 bit
  EMAC_REG   REG_EMAC_IDR_H; 	      //0x0000005C Interrupt Disable Register High 16 bit
  EMAC_REG   REG_EMAC_IMR_L; 	      //0x00000060 Interrupt Mask Register Low  16 bit
  EMAC_REG   REG_EMAC_IMR_H; 	      //0x00000064 Interrupt Mask Register High 16 bit
  EMAC_REG   REG_EMAC_MAN_L; 	      //0x00000068 PHY Maintenance Register Low  16 bit
  EMAC_REG   REG_EMAC_MAN_H; 	      //0x0000006C PHY Maintenance Register High 16 bit
  EMAC_REG   REG_EMAC_RBRP_L;         //0x00000070 Receive Buffer First full pointer Low  16 bit
  EMAC_REG   REG_EMAC_RBRP_H;         //0x00000074 Receive Buffer First full pointer High 16 bit
  EMAC_REG   REG_EMAC_RBWP_L;         //0x00000078 Receive Buffer Current pointer Low  16 bit
  EMAC_REG   REG_EMAC_RBWP_H;         //0x0000007C Receive Buffer Current pointer High 16 bit
  EMAC_REG   REG_EMAC_FRA_L; 	      //0x00000080 Frames Transmitted OK Register Low  16 bit
  EMAC_REG   REG_EMAC_FRA_H; 	      //0x00000084 Frames Transmitted OK Register High 16 bit
  EMAC_REG   REG_EMAC_SCOL_L;         //0x00000088 Single Collision Frame Register Low  16 bit
  EMAC_REG   REG_EMAC_SCOL_H;         //0x0000008C Single Collision Frame Register High 16 bit
  EMAC_REG   REG_EMAC_MCOL_L;         //0x00000090 Multiple Collision Frame Register Low  16 bit
  EMAC_REG   REG_EMAC_MCOL_H;         //0x00000094 Multiple Collision Frame Register High 16 bit
  EMAC_REG   REG_EMAC_OK_L; 	      //0x00000098 Frames Received OK Register Low  16 bit
  EMAC_REG   REG_EMAC_OK_H; 	      //0x0000009C Frames Received OK Register High 16 bit
  EMAC_REG   REG_EMAC_SEQE_L;         //0x000000A0 Frame Check Sequence Error Register Low  16 bit
  EMAC_REG   REG_EMAC_SEQE_H;         //0x000000A4 Frame Check Sequence Error Register High 16 bit
  EMAC_REG   REG_EMAC_ALE_L; 	      //0x000000A8 Alignment Error Register Low  16 bit
  EMAC_REG   REG_EMAC_ALE_H; 	      //0x000000AC Alignment Error Register High 16 bit
  EMAC_REG   REG_EMAC_DTE_L; 	      //0x000000B0 Deferred Transmission Frame Register Low  16 bit
  EMAC_REG   REG_EMAC_DTE_H; 	      //0x000000B4 Deferred Transmission Frame Register High 16 bit
  EMAC_REG   REG_EMAC_LCOL_L;         //0x000000B8 Late Collision Register Low  16 bit
  EMAC_REG   REG_EMAC_LCOL_H;         //0x000000BC Late Collision Register High 16 bit
  EMAC_REG   REG_EMAC_ECOL_L;         //0x000000C0 Excessive Collision Register Low  16 bit
  EMAC_REG   REG_EMAC_ECOL_H;         //0x000000C4 Excessive Collision Register High 16 bit
  EMAC_REG   REG_EMAC_TUE_L; 	      //0x000000C8 Transmit Underrun Error Register Low  16 bit
  EMAC_REG   REG_EMAC_TUE_H; 	      //0x000000CC Transmit Underrun Error Register High 16 bit
  EMAC_REG   REG_EMAC_CSE_L;          //0x000000D0 Carrier sense errors Register Low  16 bit
  EMAC_REG   REG_EMAC_CSE_H;          //0x000000D4 Carrier sense errors Register High 16 bit
  EMAC_REG   REG_EMAC_RE_L;           //0x000000D8 Receive resource error Low  16 bit
  EMAC_REG   REG_EMAC_RE_H;           //0x000000DC Receive resource error High 16 bit
  EMAC_REG   REG_EMAC_ROVR_L;         //0x000000E0 Received overrun Low  16 bit
  EMAC_REG   REG_EMAC_ROVR_H;         //0x000000E4 Received overrun High 16 bit
  EMAC_REG   REG_EMAC_SE_L;           //0x000000E8 Received symbols error Low  16 bit
  EMAC_REG   REG_EMAC_SE_H;           //0x000000EC Received symbols error High 16 bit
//  EMAC_REG	 REG_EMAC_CDE; 	      //           Code Error Register
  EMAC_REG   REG_EMAC_ELR_L; 	      //0x000000F0 Excessive Length Error Register Low  16 bit
  EMAC_REG   REG_EMAC_ELR_H; 	      //0x000000F4 Excessive Length Error Register High 16 bit

  EMAC_REG   REG_EMAC_RJB_L; 	      //0x000000F8 Receive Jabber Register Low  16 bit
  EMAC_REG   REG_EMAC_RJB_H; 	      //0x000000FC Receive Jabber Register High 16 bit
  EMAC_REG   REG_EMAC_USF_L; 	      //0x00000100 Undersize Frame Register Low  16 bit
  EMAC_REG   REG_EMAC_USF_H; 	      //0x00000104 Undersize Frame Register High 16 bit
  EMAC_REG   REG_EMAC_SQEE_L; 	      //0x00000108 SQE Test Error Register Low  16 bit
  EMAC_REG   REG_EMAC_SQEE_H; 	      //0x0000010C SQE Test Error Register High 16 bit
//  EMAC_REG	 REG_EMAC_DRFC;       //           Discarded RX Frame Register
  EMAC_REG   REG_Reserved1_L; 	      //0x00000110 Low  16 bit
  EMAC_REG   REG_Reserved1_H; 	      //0x00000114 High 16 bit
  EMAC_REG   REG_Reserved2_L; 	      //0x00000118 Low  16 bit
  EMAC_REG   REG_Reserved2_H; 	      //0x0000011C High 16 bit
  EMAC_REG   REG_EMAC_HSH_L; 	      //0x00000120 Hash Address High[63:32] Low  16 bit
  EMAC_REG   REG_EMAC_HSH_H; 	      //0x00000124 Hash Address High[63:32] High 16 bit
  EMAC_REG   REG_EMAC_HSL_L; 	      //0x00000128 Hash Address Low[31:0] Low  16 bit
  EMAC_REG   REG_EMAC_HSL_H; 	      //0x0000012C Hash Address Low[31:0] High 16 bit

  EMAC_REG   REG_EMAC_SA1L_L;         //0x00000130 Specific Address 1 Low, First 4 bytes Low  16 bit
  EMAC_REG   REG_EMAC_SA1L_H;         //0x00000134 Specific Address 1 Low, First 4 bytes High 16 bit
  EMAC_REG   REG_EMAC_SA1H_L;         //0x00000138 Specific Address 1 High, Last 2 bytes Low  16 bit
  EMAC_REG   REG_EMAC_SA1H_H;         //0x0000013C Specific Address 1 High, Last 2 bytes High 16 bit
  EMAC_REG   REG_EMAC_SA2L_L;         //0x00000140 Specific Address 2 Low, First 4 bytes Low  16 bit
  EMAC_REG   REG_EMAC_SA2L_H;         //0x00000144 Specific Address 2 Low, First 4 bytes High 16 bit
  EMAC_REG   REG_EMAC_SA2H_L;         //0x00000148 Specific Address 2 High, Last 2 bytes Low  16 bit
  EMAC_REG   REG_EMAC_SA2H_H;         //0x0000014C Specific Address 2 High, Last 2 bytes High 16 bit
  EMAC_REG   REG_EMAC_SA3L_L;         //0x00000150 Specific Address 3 Low, First 4 bytes Low  16 bit
  EMAC_REG   REG_EMAC_SA3L_H;         //0x00000154 Specific Address 3 Low, First 4 bytes High 16 bit
  EMAC_REG   REG_EMAC_SA3H_L;         //0x00000158 Specific Address 3 High, Last 2 bytes Low  16 bit
  EMAC_REG   REG_EMAC_SA3H_H;         //0x0000015C Specific Address 3 High, Last 2 bytes High 16 bit
  EMAC_REG   REG_EMAC_SA4L_L;         //0x00000160 Specific Address 4 Low, First 4 bytes Low  16 bit
  EMAC_REG   REG_EMAC_SA4L_H;         //0x00000164 Specific Address 4 Low, First 4 bytes High 16 bit
  EMAC_REG   REG_EMAC_SA4H_L;         //0x00000168 Specific Address 4 High, Last 2 bytes Low  16 bit
  EMAC_REG   REG_EMAC_SA4H_H;         //0x0000016C Specific Address 4 High, Last 2 bytes High 16 bit
  EMAC_REG   REG_TAG_TYPE_L; 	      //0x00000170 tag type of the frame Low  16 bit
  EMAC_REG   REG_TAG_TYPE_H; 	      //0x00000174 tag type of the frame High 16 bit
  EMAC_REG   REG_Reserved3[34];       //0x00000178 Low  16 bit
  EMAC_REG   REG_JULIAN_0100_L;       //0x00000200
  EMAC_REG   REG_JULIAN_0100_H;       //0x00000204
  EMAC_REG   REG_JULIAN_0104_L;       //0x00000208
  EMAC_REG   REG_JULIAN_0104_H;       //0x0000020C
  EMAC_REG   REG_JULIAN_0108_L;       //0x00000210
  EMAC_REG   REG_JULIAN_0108_H;       //0x00000214
} TITANIA_EMAC_Str, *TITANIA_EMAC;

#define MHal_MAX_INT_COUNTER    100
#define EMAC_CHECK_CNT 500000
//-------------------------------------------------------------------------------------------------
//  EMAC hardware for Titania
//-------------------------------------------------------------------------------------------------

/*8-bit RIU address*/
u8 MHal_EMAC_ReadReg8( u32 bank, u32 reg )
{
    u8 val;
    u32 address = RIU_REG_BASE + bank*0x100*2;
    address = address + (reg << 1) - (reg & 1);

    val = *( ( volatile u8* ) address );
    return val;
}

void MHal_EMAC_WritReg8( u32 bank, u32 reg, u8 val )
{
    u32 address = RIU_REG_BASE + bank*0x100*2;
    address = address + (reg << 1) - (reg & 1);

    *( ( volatile u8* ) address ) = val;
}

void MHal_EMAC_WritRam32(u32 uRamAddr, u32 xoffset,u32 xval)
{
	*((u32*)((char*)uRamAddr + xoffset)) = xval;
}

u32 MHal_EMAC_ReadReg32( u32 xoffset )
{
    volatile u32 *ptrs = (u32*)REG_ADDR_BASE;
    volatile u32 val_l = *(ptrs + (xoffset >> 1)) & 0x0000FFFF;
    volatile u32 val_h = (*(ptrs+ (xoffset >> 1) + 1) & 0x0000FFFF) << 0x10;
    return( val_l | val_h );
}

void MHal_EMAC_WritReg32( u32 xoffset, u32 xval )
{
    u32 address = REG_ADDR_BASE + xoffset*2;

    *( ( volatile u32 * ) address ) = ( u32 ) ( xval & 0x0000FFFF );
    *( ( volatile u32 * ) ( address + 4 ) ) = ( u32 ) ( xval >> 0x10 );
}

void MHal_EMAC_Write_SA1_MAC_Address(u8 m0,u8 m1,u8 m2,u8 m3,u8 m4,u8 m5)
{
	u32 w0 = (u32)m3 << 24 | m2 << 16 | m1 << 8 | m0;
	u32 w1 = (u32)m5 <<  8 | m4;
	TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;

	regs->REG_EMAC_SA1L_L= w0&0x0000FFFF;
    regs->REG_EMAC_SA1L_H= w0>>16;
	regs->REG_EMAC_SA1H_L= w1&0x0000FFFF;
    regs->REG_EMAC_SA1H_H= w1>>16;
}

void MHal_EMAC_Write_SA2_MAC_Address(u8 m0,u8 m1,u8 m2,u8 m3,u8 m4,u8 m5)
{
	u32 w0 = (u32)m3 << 24 | m2 << 16 | m1 << 8 | m0;
	u32 w1 = (u32)m5 <<  8 | m4;
	TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;

	regs->REG_EMAC_SA2L_L= w0&0x0000FFFF;
    regs->REG_EMAC_SA2L_H= w0>>16;
	regs->REG_EMAC_SA2H_L= w1&0x0000FFFF;
    regs->REG_EMAC_SA2H_H= w1>>16;
}

void MHal_EMAC_Write_SA3_MAC_Address(u8 m0,u8 m1,u8 m2,u8 m3,u8 m4,u8 m5)
{
	u32 w0 = (u32)m3 << 24 | m2 << 16 | m1 << 8 | m0;
	u32 w1 = (u32)m5 <<  8 | m4;
	TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;

	regs->REG_EMAC_SA3L_L= w0&0x0000FFFF;
    regs->REG_EMAC_SA3L_H= w0>>16;
	regs->REG_EMAC_SA3H_L= w1&0x0000FFFF;
    regs->REG_EMAC_SA3H_H= w1>>16;
}

void MHal_EMAC_Write_SA4_MAC_Address(u8 m0,u8 m1,u8 m2,u8 m3,u8 m4,u8 m5)
{
	u32 w0 = (u32)m3 << 24 | m2 << 16 | m1 << 8 | m0;
	u32 w1 = (u32)m5 <<  8 | m4;
    TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;

	regs->REG_EMAC_SA4L_L= w0&0x0000FFFF;
    regs->REG_EMAC_SA4L_H= w0>>16;
	regs->REG_EMAC_SA4H_L= w1&0x0000FFFF;
    regs->REG_EMAC_SA4H_H= w1>>16;
}

//-------------------------------------------------------------------------------------------------
//  R/W EMAC register for Titania
//-------------------------------------------------------------------------------------------------

void MHal_EMAC_update_HSH(u8 mc1, u8 mc0)
{
	TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
	regs->REG_EMAC_HSL_L= mc1&0x0000FFFF;
    regs->REG_EMAC_HSL_H= mc1>>16;
	regs->REG_EMAC_HSH_L= mc0&0x0000FFFF;
    regs->REG_EMAC_HSH_H= mc0>>16;
}

//-------------------------------------------------------------------------------------------------
//  Read control register
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_CTL(void)
{
    u32 xval;
    TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
    xval = ((regs->REG_EMAC_CTL_L)&0x0000FFFF)+((regs->REG_EMAC_CTL_H)<<0x10);
	return xval;
}

//-------------------------------------------------------------------------------------------------
//  Write Network control register
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_Write_CTL(u32 xval)
{
	TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
	regs->REG_EMAC_CTL_L= xval&0x0000FFFF;
    regs->REG_EMAC_CTL_H= xval>>16;
}

//-------------------------------------------------------------------------------------------------
//  Read Network configuration register
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_CFG(void)
{
    u32 xval;
    TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
    xval = ((regs->REG_EMAC_CFG_L)&0x0000FFFF)+((regs->REG_EMAC_CFG_H)<<0x10);
	return xval;
}

//-------------------------------------------------------------------------------------------------
//  Write Network configuration register
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_Write_CFG(u32 xval)
{
	TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
	regs->REG_EMAC_CFG_L= xval&0x0000FFFF;
    regs->REG_EMAC_CFG_H= xval>>16;
}

//-------------------------------------------------------------------------------------------------
//  Read RBQP
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_RBQP(void)
{
    u32 xval;
    TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
    xval = ((regs->REG_EMAC_RBQP_L)&0x0000FFFF)+((regs->REG_EMAC_RBQP_H)<<0x10);
	return xval;
}

//-------------------------------------------------------------------------------------------------
//  Write RBQP
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_Write_RBQP(u32 xval)
{
	TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
	regs->REG_EMAC_RBQP_L= xval&0x0000FFFF;
    regs->REG_EMAC_RBQP_H= xval>>16;
}

//-------------------------------------------------------------------------------------------------
//  Write Transmit Address register
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_Write_TAR(u32 xval)
{
	TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
	regs->REG_EMAC_TAR_L= xval&0x0000FFFF;
    regs->REG_EMAC_TAR_H= xval>>16;
}

//-------------------------------------------------------------------------------------------------
//  Write Transmit Control register
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_Write_TCR(u32 xval)
{
	TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
	regs->REG_EMAC_TCR_L= xval&0x0000FFFF;
    regs->REG_EMAC_TCR_H= xval>>16;
}

//-------------------------------------------------------------------------------------------------
//  Transmit Status Register
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_RSR(void)
{
    u32 xval;
    TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
    xval = ((regs->REG_EMAC_RSR_L)&0x0000FFFF)+((regs->REG_EMAC_RSR_H)<<0x10);
	return xval;
}

//-------------------------------------------------------------------------------------------------
//  Transmit Status Register
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_TSR(void)
{
    u32 xval;
    TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
    xval = ((regs->REG_EMAC_TSR_L)&0x0000FFFF)+((regs->REG_EMAC_TSR_H)<<0x10);
	return xval;
}

//-------------------------------------------------------------------------------------------------
//  Read Interrupt status register
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_ISR(void)
{
    u32 xval;
    TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
    xval = ((regs->REG_EMAC_ISR_L)&0x0000FFFF)+((regs->REG_EMAC_ISR_H)<<0x10);
	return xval;
}

//-------------------------------------------------------------------------------------------------
//  Read Interrupt enable register
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_IER(void)
{
    u32 xval;
    TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
    xval = ((regs->REG_EMAC_IER_L)&0x0000FFFF)+((regs->REG_EMAC_IER_H)<<0x10);
	return xval;
}

//-------------------------------------------------------------------------------------------------
//  Write Interrupt enable register
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_Write_IER(u32 xval)
{
	TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
	regs->REG_EMAC_IER_L= xval&0x0000FFFF;
    regs->REG_EMAC_IER_H= xval>>16;
}

//-------------------------------------------------------------------------------------------------
//  Read Interrupt disable register
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_IDR(void)
{
    u32 xval;
    TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
    xval = ((regs->REG_EMAC_IDR_L)&0x0000FFFF)+((regs->REG_EMAC_IDR_H)<<0x10);
	return xval;
}

//-------------------------------------------------------------------------------------------------
//  Write Interrupt disable register
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_Write_IDR(u32 xval)
{
	TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
	regs->REG_EMAC_IDR_L= xval&0x0000FFFF;
    regs->REG_EMAC_IDR_H= xval>>16;
}

//-------------------------------------------------------------------------------------------------
//  Read Interrupt mask register
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_IMR(void)
{
    u32 xval;
    TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
    xval = ((regs->REG_EMAC_IMR_L)&0x0000FFFF)+((regs->REG_EMAC_IMR_H)<<0x10);
	return xval;
}

//-------------------------------------------------------------------------------------------------
//  Read PHY maintenance register
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_MAN(void)
{
    u32 xval;
    TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
    xval = ((regs->REG_EMAC_MAN_L)&0x0000FFFF)+((regs->REG_EMAC_MAN_H)<<0x10);
	return xval;
}

//-------------------------------------------------------------------------------------------------
//  Write PHY maintenance register
//-------------------------------------------------------------------------------------------------
extern unsigned char phy_id;
void MHal_EMAC_Write_MAN(u32 xval)
{
	TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;

	xval = (xval & 0xF07FFFFF) | (phy_id << 23); // <-@@@ specify PHY ID

	regs->REG_EMAC_MAN_L= xval&0x0000FFFF;
    regs->REG_EMAC_MAN_H= xval>>16;
}

//-------------------------------------------------------------------------------------------------
//  Write Receive Buffer Configuration
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_Write_BUFF(u32 xval)
{
	TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
	regs->REG_EMAC_RBCFG_L= xval&0x0000FFFF;
    regs->REG_EMAC_RBCFG_H= xval>>16;
}

//-------------------------------------------------------------------------------------------------
//  Read Receive Buffer Configuration
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_BUFF(void)
{
    u32 xval;
    TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
    xval = ((regs->REG_EMAC_RBCFG_L)&0x0000FFFF)+((regs->REG_EMAC_RBCFG_H)<<0x10);
	return xval;
}

//-------------------------------------------------------------------------------------------------
//  Read Receive First Full Pointer
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_RDPTR(void)
{
    u32 xval;
    TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
    xval = ((regs->REG_EMAC_RBRP_L)&0x0000FFFF)+((regs->REG_EMAC_RBRP_H)<<0x10);
	return xval;
}

//-------------------------------------------------------------------------------------------------
//  Write Receive First Full Pointer
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_Write_RDPTR(u32 xval)
{
	TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
	regs->REG_EMAC_RBRP_L= xval&0x0000FFFF;
    regs->REG_EMAC_RBRP_H= xval>>16;
}

//-------------------------------------------------------------------------------------------------
//  Write Receive First Full Pointer
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_Write_WRPTR(u32 xval)
{
	TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
	regs->REG_EMAC_RBWP_L= xval&0x0000FFFF;
    regs->REG_EMAC_RBWP_H= xval>>16;
}

//-------------------------------------------------------------------------------------------------
//  Frames transmitted OK
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_FRA(void)
{
    //return MHal_EMAC_ReadReg32(REG_ETH_FRA);
    u32 xval;
    TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
    xval = ((regs->REG_EMAC_FRA_L)&0x0000FFFF)+((regs->REG_EMAC_FRA_H)<<0x10);
	return xval;
}

//-------------------------------------------------------------------------------------------------
//  Single collision frames
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_SCOL(void)
{
    //return MHal_EMAC_ReadReg32(REG_ETH_SCOL);
    u32 xval;
    TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
    xval = ((regs->REG_EMAC_SCOL_L)&0x0000FFFF)+((regs->REG_EMAC_SCOL_H)<<0x10);
	return xval;
}

//-------------------------------------------------------------------------------------------------
//  Multiple collision frames
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_MCOL(void)
{
    //return MHal_EMAC_ReadReg32(REG_ETH_MCOL);
    u32 xval;
    TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
    xval = ((regs->REG_EMAC_MCOL_L)&0x0000FFFF)+((regs->REG_EMAC_MCOL_H)<<0x10);
	return xval;
}

//-------------------------------------------------------------------------------------------------
//  Frames received OK
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_OK(void)
{
    //return MHal_EMAC_ReadReg32(REG_ETH_OK);
    u32 xval;
    TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
    xval = ((regs->REG_EMAC_OK_L)&0x0000FFFF)+((regs->REG_EMAC_OK_H)<<0x10);
	return xval;
}

//-------------------------------------------------------------------------------------------------
//  Frame check sequence errors
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_SEQE(void)
{
    //return MHal_EMAC_ReadReg32(REG_ETH_SEQE);
    u32 xval;
    TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
    xval = ((regs->REG_EMAC_SEQE_L)&0x0000FFFF)+((regs->REG_EMAC_SEQE_H)<<0x10);
	return xval;
}

//-------------------------------------------------------------------------------------------------
//  Alignment errors
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_ALE(void)
{
    //return MHal_EMAC_ReadReg32(REG_ETH_ALE);
    u32 xval;
    TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
    xval = ((regs->REG_EMAC_ALE_L)&0x0000FFFF)+((regs->REG_EMAC_ALE_H)<<0x10);
	return xval;
}

//-------------------------------------------------------------------------------------------------
//  Late collisions
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_LCOL(void)
{
    //return MHal_EMAC_ReadReg32(REG_ETH_LCOL);
    u32 xval;
    TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
    xval = ((regs->REG_EMAC_LCOL_L)&0x0000FFFF)+((regs->REG_EMAC_LCOL_H)<<0x10);
	return xval;
}

//-------------------------------------------------------------------------------------------------
//  Excessive collisions
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_ECOL(void)
{
    //return MHal_EMAC_ReadReg32(REG_ETH_ECOL);
    u32 xval;
    TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
    xval = ((regs->REG_EMAC_ECOL_L)&0x0000FFFF)+((regs->REG_EMAC_ECOL_H)<<0x10);
	return xval;
}

//-------------------------------------------------------------------------------------------------
//  Transmit under-run errors
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_TUE(void)
{
    //return MHal_EMAC_ReadReg32(REG_ETH_TUE);
    u32 xval;
    TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
    xval = ((regs->REG_EMAC_TUE_L)&0x0000FFFF)+((regs->REG_EMAC_TUE_H)<<0x10);
	return xval;
}

//-------------------------------------------------------------------------------------------------
//  Carrier sense errors
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_CSE(void)
{
    //return MHal_EMAC_ReadReg32(REG_ETH_CSE);
    u32 xval;
    TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
    xval = ((regs->REG_EMAC_CSE_L)&0x0000FFFF)+((regs->REG_EMAC_CSE_H)<<0x10);
	return xval;
}

//-------------------------------------------------------------------------------------------------
//  Receive resource error
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_RE(void)
{
    //return MHal_EMAC_ReadReg32(REG_ETH_RE);
    u32 xval;
    TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
    xval = ((regs->REG_EMAC_RE_L)&0x0000FFFF)+((regs->REG_EMAC_RE_H)<<0x10);
	return xval;
}

//-------------------------------------------------------------------------------------------------
//  Received overrun
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_ROVR(void)
{
    //return MHal_EMAC_ReadReg32(REG_ETH_ROVR);
    u32 xval;
    TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
    xval = ((regs->REG_EMAC_ROVR_L)&0x0000FFFF)+((regs->REG_EMAC_ROVR_H)<<0x10);
	return xval;
}

//-------------------------------------------------------------------------------------------------
//  Received symbols error
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_SE(void)
{
    //return MHal_EMAC_ReadReg32(REG_ETH_SE);
    u32 xval;
    TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
    xval = ((regs->REG_EMAC_SE_L)&0x0000FFFF)+((regs->REG_EMAC_SE_H)<<0x10);
	return xval;
}

//-------------------------------------------------------------------------------------------------
//  Excessive length errors
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_ELR(void)
{
    //return MHal_EMAC_ReadReg32(REG_ETH_ELR);
    u32 xval;
    TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
    xval = ((regs->REG_EMAC_ELR_L)&0x0000FFFF)+((regs->REG_EMAC_ELR_H)<<0x10);
	return xval;
}

//-------------------------------------------------------------------------------------------------
//  Receive jabbers
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_RJB(void)
{
    //return MHal_EMAC_ReadReg32(REG_ETH_RJB);
    u32 xval;
    TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
    xval = ((regs->REG_EMAC_RJB_L)&0x0000FFFF)+((regs->REG_EMAC_RJB_H)<<0x10);
	return xval;
}

//-------------------------------------------------------------------------------------------------
//  Undersize frames
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_USF(void)
{
    //return MHal_EMAC_ReadReg32(REG_ETH_USF);
    u32 xval;
    TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
    xval = ((regs->REG_EMAC_USF_L)&0x0000FFFF)+((regs->REG_EMAC_USF_H)<<0x10);
	return xval;
}

//-------------------------------------------------------------------------------------------------
//  SQE test errors
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_SQEE(void)
{
    //return MHal_EMAC_ReadReg32(REG_ETH_SQEE);
    u32 xval;
    TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
    xval = ((regs->REG_EMAC_SQEE_L)&0x0000FFFF)+((regs->REG_EMAC_SQEE_H)<<0x10);
	return xval;
}

//-------------------------------------------------------------------------------------------------
//  Write Julian 100
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_Write_JULIAN_0100(u32 xval)
{
	TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
	//regs->REG_JULIAN_0100= xval&0x0000FFFF;
	regs->REG_JULIAN_0100_L= xval&0x0000FFFF;
    regs->REG_JULIAN_0100_H= xval>>16;
}

//-------------------------------------------------------------------------------------------------
//  Read Julian 104
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_JULIAN_0104(void)
{
    TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
	return ((regs->REG_JULIAN_0104_L)&0x0000FFFF)+((regs->REG_JULIAN_0104_H)<<0x10);
}

//-------------------------------------------------------------------------------------------------
//  Write Julian 104
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_Write_JULIAN_0104(u32 xval)
{
	TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
	regs->REG_JULIAN_0104_L= xval&0x0000FFFF;
    regs->REG_JULIAN_0104_H= xval>>16;
}

//-------------------------------------------------------------------------------------------------
//  Read Julian 108
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_JULIAN_0108(void)
{
    TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
	return ((regs->REG_JULIAN_0108_L)&0x0000FFFF)+((regs->REG_JULIAN_0108_H)<<0x10);
}

//-------------------------------------------------------------------------------------------------
//  Write Julian 108
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_Write_JULIAN_0108(u32 xval)
{
	TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
	regs->REG_JULIAN_0108_L= xval&0x0000FFFF;
    regs->REG_JULIAN_0108_H= xval>>16;
}

//-------------------------------------------------------------------------------------------------
//  PHY INTERFACE
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Enable the MDIO bit in MAC control register
// When not called from an interrupt-handler, access to the PHY must be
// protected by a spinlock.
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_enable_mdi(void)
{
   TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
   regs->REG_EMAC_CTL_L |= EMAC_MPE; //enable management port //
   regs->REG_EMAC_CTL_H &= EMAC_ALLFF;
}

//-------------------------------------------------------------------------------------------------
//  Disable the MDIO bit in the MAC control register
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_disable_mdi(void)
{
   TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
   regs->REG_EMAC_CTL_L &= ~EMAC_MPE;    // disable management port //
   regs->REG_EMAC_CTL_H &= EMAC_ALLFF;
}

//-------------------------------------------------------------------------------------------------
// Write value to the a PHY register
// Note: MDI interface is assumed to already have been enabled.
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_write_phy (unsigned char phy_addr, unsigned char address, u32 value)
{
   u32 uRegVal, uCTL = 0;
   TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;

   uRegVal =(EMAC_HIGH | EMAC_CODE_802_3 | EMAC_RW_W)
            | ((phy_addr & 0x1f) << PHY_ADDR_SHIFT) | (address << PHY_REG_SHIFT) | (value & 0xFFFF) ;

    uCTL = MHal_EMAC_Read_CTL();
    MHal_EMAC_enable_mdi();
    MHal_EMAC_Write_MAN(uRegVal);

   // Wait until IDLE bit in Network Status register is cleared //
   uRegVal = ((regs->REG_EMAC_SR_L)&0x0000FFFF)+((regs->REG_EMAC_SR_H)<<0x10);

   while (!(uRegVal& EMAC_IDLE))
   {
       uRegVal = ((regs->REG_EMAC_SR_L)&0x0000FFFF)+((regs->REG_EMAC_SR_H)<<0x10);
       barrier ();
   }

    MHal_EMAC_Write_CTL(uCTL);
}
//-------------------------------------------------------------------------------------------------
// Read value stored in a PHY register.
// Note: MDI interface is assumed to already have been enabled.
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_read_phy(unsigned char phy_addr, unsigned char address,u32 *value)
{
   u32 uRegVal, uCTL = 0;
   TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;

   uRegVal = (EMAC_HIGH | EMAC_CODE_802_3 | EMAC_RW_R)
           | ((phy_addr & 0x1f) << PHY_ADDR_SHIFT) | (address << PHY_REG_SHIFT) | (0) ;

   uCTL = MHal_EMAC_Read_CTL();
   MHal_EMAC_enable_mdi();
   MHal_EMAC_Write_MAN(uRegVal);

   //Wait until IDLE bit in Network Status register is cleared //
   uRegVal = ((regs->REG_EMAC_SR_L)&0x0000FFFF)+((regs->REG_EMAC_SR_H)<<0x10);

   while (!(uRegVal & EMAC_IDLE))
   {
       uRegVal = ((regs->REG_EMAC_SR_L)&0x0000FFFF)+((regs->REG_EMAC_SR_H)<<0x10);
       barrier ();
   }

    *value = (MHal_EMAC_Read_MAN() & 0x0000ffff);
    MHal_EMAC_Write_CTL(uCTL);
}

//-------------------------------------------------------------------------------------------------
// Update MAC speed and H/F duplex
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_update_speed_duplex(u32 uspeed, u32 uduplex)
{
   u32 mac_cfg_L/*, mac_cfg_H*/;
   TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;

   mac_cfg_L = regs->REG_EMAC_CFG_L & ~(EMAC_SPD | EMAC_FD);
  // mac_cfg_H = regs->REG_EMAC_CFG_H & ~(EMAC_SPD | EMAC_FD);

   if (uspeed == SPEED_100)
   {

       if (uduplex == DUPLEX_FULL)    // 100 Full Duplex //
       {
           regs->REG_EMAC_CFG_L = mac_cfg_L | EMAC_SPD | EMAC_FD;
       }
       else                           // 100 Half Duplex ///
       {
		   regs->REG_EMAC_CFG_L = mac_cfg_L | EMAC_SPD;
       }
   }
   else
   {
       if (uduplex == DUPLEX_FULL)    //10 Full Duplex //
       {
           regs->REG_EMAC_CFG_L = mac_cfg_L |EMAC_FD;
       }
       else                           // 10 Half Duplex //
       {
           regs->REG_EMAC_CFG_L = mac_cfg_L;
       }
   }
   regs->REG_EMAC_CFG_H &= EMAC_ALLFF;//Write to CFG
}

//-------------------------------------------------------------------------------------------------
//Initialize and enable the PHY interrupt when link-state changes
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_enable_phyirq (void)
{

}

//-------------------------------------------------------------------------------------------------
// Disable the PHY interrupt
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_disable_phyirq (void)
{

}
//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------

u32 MHal_EMAC_get_SA1H_addr(void)
{
   TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
   return (regs->REG_EMAC_SA1H_L&0x0000FFFF) + (regs->REG_EMAC_SA1H_H<<16);
}

u32 MHal_EMAC_get_SA1L_addr(void)
{
   TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
   return (regs->REG_EMAC_SA1L_L&0x0000FFFF) + (regs->REG_EMAC_SA1L_H<<16);
}

u32 MHal_EMAC_get_SA2H_addr(void)
{
   TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
   return (regs->REG_EMAC_SA2H_L&0x0000FFFF) + (regs->REG_EMAC_SA2H_H<<16);
}

u32 MHal_EMAC_get_SA2L_addr(void)
{
   TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
   return (regs->REG_EMAC_SA2L_L&0x0000FFFF) + (regs->REG_EMAC_SA2L_H<<16);
}

void MHal_EMAC_Write_SA1H(u32 xval)
{
	TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
	regs->REG_EMAC_SA1H_L= xval&0x0000FFFF;
    regs->REG_EMAC_SA1H_H= xval>>16;
}

void MHal_EMAC_Write_SA1L(u32 xval)
{
	TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
	regs->REG_EMAC_SA1L_L= xval&0x0000FFFF;
    regs->REG_EMAC_SA1L_H= xval>>16;
}

void MHal_EMAC_Write_SA2H(u32 xval)
{
	TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
	regs->REG_EMAC_SA2H_L= xval&0x0000FFFF;
    regs->REG_EMAC_SA2H_H= xval>>16;
}

void MHal_EMAC_Write_SA2L(u32 xval)
{
	TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
	regs->REG_EMAC_SA2L_L= xval&0x0000FFFF;
    regs->REG_EMAC_SA2L_H= xval>>16;
}

void * MDev_memset(void * s,int c,unsigned long count)
{
	char *xs = (char *) s;

	while (count--)
		*xs++ = c;

	return s;
}

//-------------------------------------------------------------------------------------------------
// Check INT Done
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_CheckINTDone(void)
{
   u32 retIntStatus;
   retIntStatus = MHal_EMAC_Read_ISR();
   MHalThisUVE.cntChkINTCounter = (MHalThisUVE.cntChkINTCounter%MHal_MAX_INT_COUNTER);
   MHalThisUVE.cntChkINTCounter ++;
   if((retIntStatus&EMAC_INT_DONE)||(MHalThisUVE.cntChkINTCounter==(MHal_MAX_INT_COUNTER-1)))
   {
      MHalThisUVE.flagISR_INT_DONE = 0x01;
	  return TRUE;
   }
   return FALSE;
}

void MHal_EMAC_WritCAM_Address(u32 addr,u8 m0,u8 m1,u8 m2,u8 m3,u8 m4,u8 m5)
{
	u32 w0 = (u32)m1 << 24 | m0 << 16;
	u32 w1 = (u32)m5 << 24 | m4 << 16 | m3 << 8 | m2;
	MHal_EMAC_WritReg32(addr    ,w0);
    MHal_EMAC_WritReg32(addr + 4,w1);
}

void MHal_EMAC_ReadCAM_Address(u32 addr,u32 *w0,u32 *w1)
{
	*w0 = MHal_EMAC_ReadReg32(addr);
	*w1 = MHal_EMAC_ReadReg32(addr + 4);
}

extern unsigned char phy_id;
//-------------------------------------------------------------------------------------------------
// MAC cable connection detection
//-------------------------------------------------------------------------------------------------
s32 MHal_EMAC_CableConnection(void)
{
    u32 value, retValue, counter;
    u8 uRegVal;
    u32 speed, duplex;

    //check status

    counter = 0;
    MHal_EMAC_read_phy(phy_id, PHY_REG_STATUS, &value);
    while(!(value & PHY_AN_DONE))
    {
        //wait 4 secs
        counter++;
        if(counter > 20)
        {
            printf("AN fail");
            return 0;
        }

        mdelay(200);
        MHal_EMAC_read_phy(phy_id, PHY_REG_STATUS, &value);
    }

    counter = 0;
    MHal_EMAC_read_phy(phy_id, PHY_REG_STATUS, &value);
    while(!(value & PHY_LINK_UP))
    {
        //wait 4 secs
        counter++;
        if(counter > 20)
        {
            printf("Link up fail");
            return 0;
        }

        mdelay(200);
        MHal_EMAC_read_phy(phy_id, PHY_REG_STATUS, &value);
    }

	#ifndef CONFIG_GMAC_ETHERNET_ALBANY
    MHal_EMAC_read_phy(phy_id, PHY_REG_PHYSR, &value);
    speed = value & PHYSR_SPEED_MASK;
    duplex = value & PHYSR_DUPLEX_MASK;
	#else
    MHal_EMAC_read_phy(phy_id, PHY_REG_BASIC, &value);
    speed = value & BMCR_SPEED_MASK;
    duplex = value & BMCR_DUPLEX_MASK;
	#endif


	#ifndef CONFIG_GMAC_ETHERNET_ALBANY
    //1000MB Full
    if( speed == PHYSR_1000)
    {
        /* Disable reg_rgmii_slow */
        uRegVal = MHal_EMAC_ReadReg8(0x0033, 0xb4);
        uRegVal &= 0xfe;
        MHal_EMAC_WritReg8(0x0033, 0xb4, uRegVal);

        /* Set reg_xmii_type as 1G */
        uRegVal = MHal_EMAC_ReadReg8(0x1224, 0x60);
        uRegVal &= 0xfc;
        uRegVal |= 0x00;

        MHal_EMAC_WritReg8(0x1224, 0x60, uRegVal);

        MHalThisBCE.speed = SPEED_1000;
        MHalThisBCE.duplex = 1;

        retValue = 5;
    }
    //100MB Full/HALF
    else if (speed == PHYSR_100)
    {
        /* Enable reg_rgmii_slow */
        uRegVal = MHal_EMAC_ReadReg8(0x0033, 0xb4);
        uRegVal |= 0x01;
        MHal_EMAC_WritReg8(0x0033, 0xb4, uRegVal);

        /* Set reg_rgmii10_100 as 100M*/
        uRegVal = MHal_EMAC_ReadReg8(0x0033, 0xb4);
        uRegVal |= 0x02;
        MHal_EMAC_WritReg8(0x0033, 0xb4, uRegVal);

        /* Set reg_xmii_type as 10M/100M */
        uRegVal = MHal_EMAC_ReadReg8(0x1224, 0x60);
        uRegVal &= 0xfc;
        uRegVal |= 0x01;

        MHal_EMAC_WritReg8(0x1224, 0x60, uRegVal);

        MHalThisBCE.speed = SPEED_100;

        if(duplex == PHYSR_FULL_DUPLEX)
        {
            MHalThisBCE.duplex = 1;
        }
        else
        {
            MHalThisBCE.duplex = 0;
        }

        retValue = 4;
    }
    //10MB Full/Half
    else
    {
        /* Enable reg_rgmii_slow */
        uRegVal = MHal_EMAC_ReadReg8(0x0033, 0xb4);
        uRegVal |= 0x01;
        MHal_EMAC_WritReg8(0x0033, 0xb4, uRegVal);

        /* Enable reg_rgmii10_100 as 10M*/
        uRegVal = MHal_EMAC_ReadReg8(0x0033, 0xb4);
        uRegVal &= 0xfd;
        MHal_EMAC_WritReg8(0x0033, 0xb4, uRegVal);

        /* Set reg_xmii_type as 10M/100M */
        uRegVal = MHal_EMAC_ReadReg8(0x1224, 0x60);
        uRegVal &= 0xfc;
        uRegVal |= 0x01;

        MHal_EMAC_WritReg8(0x1224, 0x60, uRegVal);

        MHalThisBCE.speed = SPEED_10;

        if(duplex == PHYSR_FULL_DUPLEX)
        {
            MHalThisBCE.duplex = 1;
            retValue = 2;
        }
        else
        {
            MHalThisBCE.duplex = 0;
            retValue = 1;
        }

       retValue = 3;
    }
	#else
    //1000MB Full
    if( speed == BMCR_1000)
    {
        /* Disable reg_rgmii_slow */
        uRegVal = MHal_EMAC_ReadReg8(0x0033, 0xb4);
        uRegVal &= 0xfe;
        MHal_EMAC_WritReg8(0x0033, 0xb4, uRegVal);

        MHalThisBCE.speed = SPEED_1000;
        MHalThisBCE.duplex = 1;

        retValue = 5;
    }
    //100MB Full/HALF
    else if (speed == BMCR_100)
    {
        /* Enable reg_rgmii_slow */
        uRegVal = MHal_EMAC_ReadReg8(0x0033, 0xb4);
        uRegVal |= 0x01;
        MHal_EMAC_WritReg8(0x0033, 0xb4, uRegVal);

        /* Set reg_rgmii10_100 as 100M*/
        uRegVal = MHal_EMAC_ReadReg8(0x0033, 0xb4);
        uRegVal |= 0x02;
        MHal_EMAC_WritReg8(0x0033, 0xb4, uRegVal);

        MHalThisBCE.speed = SPEED_100;

        if(duplex == BMCR_FULL_DUPLEX)
        {
            MHalThisBCE.duplex = 1;
        }
        else
        {
            MHalThisBCE.duplex = 0;
        }

        retValue = 4;
    }
    //10MB Full/Half
    else
    {
        /* Enable reg_rgmii_slow */
        uRegVal = MHal_EMAC_ReadReg8(0x0033, 0xb4);
        uRegVal |= 0x01;
        MHal_EMAC_WritReg8(0x0033, 0xb4, uRegVal);

        /* Enable reg_rgmii10_100 as 10M*/
        uRegVal = MHal_EMAC_ReadReg8(0x0033, 0xb4);
        uRegVal &= 0xfd;
        MHal_EMAC_WritReg8(0x0033, 0xb4, uRegVal);

        MHalThisBCE.speed = SPEED_10;

        if(duplex == BMCR_FULL_DUPLEX)
        {
            MHalThisBCE.duplex = 1;
            retValue = 2;
        }
        else
        {
            MHalThisBCE.duplex = 0;
            retValue = 1;
        }

       retValue = 3;
    }
	#endif

    return(retValue);
}

void MHal_EMAC_CheckTSR(void)
{
    u32 check;
    u32 tsrval = 0;

    for (check = 0; check < EMAC_CHECK_CNT; check++)
    {
        tsrval = MHal_EMAC_Read_TSR();
        if ((tsrval & EMAC_IDLETSR) || (tsrval & EMAC_BNQ))
            return;
    }

    printf("Err CheckTSR:0x%x\n", tsrval);
}

//-------------------------------------------------------------------------------------------------
// EMAC Negotiation PHY
//-------------------------------------------------------------------------------------------------
s32 MHal_EMAC_NegotiationPHY(void)
{
    s32 retValue;

    // Set default as 10Mb half-duplex if negotiation fails.
    MHalThisBCE.duplex = 0;
    MHalThisBCE.speed = SPEED_10;
    //Auto-Negotiation
    printf("Auto-Negotiation...\n");
    MHal_EMAC_write_phy(phy_id, PHY_REG_BASIC, 0x1000);
    retValue = MHal_EMAC_CableConnection();

    printf("Link Status Speed:%u Full-duplex:%u\n", MHalThisBCE.speed, MHalThisBCE.duplex);
    return(retValue);
}

//-------------------------------------------------------------------------------------------------
// EMAC Hardware register set
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
// EMAC Timer set for Receive function
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_timer_callback(unsigned long value)
{
	TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
    regs->REG_EMAC_IER_L |= (EMAC_INT_RCOM );
    regs->REG_EMAC_IER_H &= EMAC_ALLFF;
}

//-------------------------------------------------------------------------------------------------
// EMAC Timer set for Receive function
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_WriteProtect(u8 bEnable, u32 u32AddrUB, u32 u32AddrLB)
{
    u32 val = MHal_EMAC_ReadReg32(0x100), addr = 0;;

    val |= (bEnable) ? EMAC_MIU_WP_EN : 0;
    MHal_EMAC_WritReg32(0x100, val);

    //0x11E
    addr = u32AddrUB / MIU_ADDR_UNIT;
    printf("Upper addr:%ux\n", addr);
    MHal_EMAC_WritReg32(0x11E, (addr & 0x0000FFFF));
    MHal_EMAC_WritReg32(0x120, (addr & 0x00FF0000) >> 16);
    //0x122
    addr = u32AddrLB / MIU_ADDR_UNIT;
    printf("Lower addr:%ux\n", addr);
    MHal_EMAC_WritReg32(0x122, (u32AddrLB/MIU_ADDR_UNIT) & 0x0000FFFF);
    MHal_EMAC_WritReg32(0x124, ((u32AddrLB/MIU_ADDR_UNIT) & 0x00FF0000) >> 16);
}

#if (SUPPORT_ALBANY)
void MHal_EMAC_Albany_I2C(u8 enable)
{
    //I2C [6:0]
    *( (volatile u8 * ) ( ( char * ) 0xA0203C00 +  0x58*2) ) &= 0x80;
    if (enable)
        *( (volatile u8 * ) ( ( char * ) 0xA0203C00 +  0x58*2) ) |= 0x47;
    else
        *( (volatile u8 * ) ( ( char * ) 0xA0203C00 +  0x58*2) ) |= 0x17;
}

void MHal_EMAC_Power_On_Albany(void)
{
    printf("Albany's PHY setting\n");
    //Chip config [6][4]= 0
    *( (volatile u8 * ) ( ( char * ) 0xA0203C00 +  0x40*2+1) ) &= 0xAF;
    //Reset output mode, bit[3] = 0
    *( (volatile u8 * ) ( ( char * ) 0xA0203C00 +  0x98*2) ) &= 0xF7;
    //Reset -> High, bit[3] = 1
    *( (volatile u8 * ) ( ( char * ) 0xA0203C00 +  0x96*2) ) |= 0x08;

    //clk enable
    *( (volatile u8 * ) ( ( char * ) 0xA0203C00 +  0xF0*2) ) &= 0xFC;
    *( (volatile u8 * ) ( ( char * ) 0xA0203C00 +  0xF0*2) ) |= 0x2;
    //BIT[2] = 0
    *( (volatile u8 * ) ( ( char * ) 0xA0203C00 +  0x38*2) ) &= 0xFB;

    //Chip config [6:0]= 0
    *( (volatile u8 * ) ( ( char * ) 0xA0203C00 +  0x5A*2) ) &= 0x80;
    //I2C enable [6:0]
    MHal_EMAC_Albany_I2C(TRUE);
    //Reset -> low, bit[3] = 0
    *( (volatile u8 * ) ( ( char * ) 0xA0203C00 +  0x96*2) ) &= 0xF7;
    udelay(10000);
    //Reset -> High, bit[3] = 1
    *( (volatile u8 * ) ( ( char * ) 0xA0203C00 +  0x96*2) ) |= 0x08;
    udelay(5000);
}
#endif
//-------------------------------------------------------------------------------------------------
// EMAC clock on/off
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_ClkGen(void)
{
    u8 uRegVal;

#ifdef CONFIG_GMAC_ETHERNET_ALBANY

    /* LED light on */
    uRegVal = MHal_EMAC_ReadReg8(0x000e, 0x50);
    uRegVal &= 0xcf;
    uRegVal |= 0x10;
    MHal_EMAC_WritReg8(0x000e, 0x50, uRegVal);

    //printk("internal ephy flow\n");
    /* Set reg_xmii_type as int PHY mode */
    uRegVal = MHal_EMAC_ReadReg8(0x1224, 0x60);
    uRegVal &= 0xfc;
    uRegVal |= 0x02;
    MHal_EMAC_WritReg8(0x1224, 0x60, uRegVal);

    /* Set gmac ahb clock to 172MHZ */
    MHal_EMAC_WritReg8(0x1033, 0x64, 0x04);

    /* Enable gmac tx clock */
    MHal_EMAC_WritReg8(0x1224, 0x23, 0x08);

    /* Enable gmac rx clock */
    MHal_EMAC_WritReg8(0x1224, 0x24, 0x08);
    MHal_EMAC_WritReg8(0x1224, 0x25, 0x00);

    /* Init ePHY */
    //gain shift
    MHal_EMAC_WritReg8(0x0032, 0xb4, 0x02);

    //det max
    MHal_EMAC_WritReg8(0x0032, 0x4f, 0x02);

    //det min
    MHal_EMAC_WritReg8(0x0032, 0x51, 0x01);

    //snr len (emc noise)
    MHal_EMAC_WritReg8(0x0032, 0x77, 0x18);

    //lpbk_enable set to 0
    MHal_EMAC_WritReg8(0x0031, 0x72, 0xa0);

    MHal_EMAC_WritReg8(0x0032, 0xfc, 0x00);
    MHal_EMAC_WritReg8(0x0032, 0xfd, 0x00);
    MHal_EMAC_WritReg8(0x0033, 0xa1, 0x80);
    MHal_EMAC_WritReg8(0x0032, 0xcc, 0x40);
    MHal_EMAC_WritReg8(0x0032, 0xbb, 0x04);
    MHal_EMAC_WritReg8(0x0033, 0x3a, 0x00);
    MHal_EMAC_WritReg8(0x0033, 0xf1, 0x00);
    MHal_EMAC_WritReg8(0x0033, 0x8a, 0x01);
    MHal_EMAC_WritReg8(0x0032, 0x3b, 0x01);
    MHal_EMAC_WritReg8(0x0032, 0xc4, 0x44);
    MHal_EMAC_WritReg8(0x0033, 0x80, 0x30);

    //100 gat
    MHal_EMAC_WritReg8(0x0033, 0xc5, 0x00);

    //200 gat
    MHal_EMAC_WritReg8(0x0033, 0x30, 0x43);

    //en_100t_phase
    MHal_EMAC_WritReg8(0x0033, 0x39, 0x41);

    //Low power mode
    MHal_EMAC_WritReg8(0x0033, 0xf2, 0xf5);
    MHal_EMAC_WritReg8(0x0033, 0xf3, 0x0d);

    // Prevent packet drop by inverted waveform
    MHal_EMAC_WritReg8(0x0031, 0x79, 0xd0);
    MHal_EMAC_WritReg8(0x0031, 0x77, 0x5a);

    //10T waveform
    MHal_EMAC_WritReg8(0x0033, 0xe8, 0x06);
    MHal_EMAC_WritReg8(0x0031, 0x2b, 0x00);
    MHal_EMAC_WritReg8(0x0033, 0xe8, 0x00);
    MHal_EMAC_WritReg8(0x0031, 0x2b, 0x00);
    MHal_EMAC_WritReg8(0x0033, 0xe8, 0x06);
    MHal_EMAC_WritReg8(0x0031, 0xaa, 0x19);
    MHal_EMAC_WritReg8(0x0031, 0xac, 0x19);
    MHal_EMAC_WritReg8(0x0031, 0xad, 0x19);
    MHal_EMAC_WritReg8(0x0031, 0xae, 0x19);
    MHal_EMAC_WritReg8(0x0031, 0xaf, 0x19);
    MHal_EMAC_WritReg8(0x0033, 0xe8, 0x00);
    MHal_EMAC_WritReg8(0x0031, 0xab, 0x28);
    MHal_EMAC_WritReg8(0x0031, 0xaa, 0x19);

    //Disable eee
    MHal_EMAC_WritReg8(0x0031, 0x2d, 0x7c);

    //speed up timing recovery
    MHal_EMAC_WritReg8(0x0032, 0xf5, 0x02);

    //signal_det_k
    MHal_EMAC_WritReg8(0x0032, 0x0f, 0xc9);

    //snr_h
    MHal_EMAC_WritReg8(0x0032, 0x89, 0x50);
    MHal_EMAC_WritReg8(0x0032, 0x8b, 0x80);
    MHal_EMAC_WritReg8(0x0032, 0x8e, 0x0e);
	MHal_EMAC_WritReg8(0x0032, 0x90, 0x04);

#else
    /* Set GMAC ahb clock to 172MHZ */
    MHal_EMAC_WritReg8(0x1033, 0x64, 0x04);

    /* Enable GMAC tx clock */
    MHal_EMAC_WritReg8(0x1224, 0x23, 0x00);

    /* Enable gmac rx clock */
    MHal_EMAC_WritReg8(0x1224, 0x24, 0x00);
    MHal_EMAC_WritReg8(0x1224, 0x25, 0x00);

    /* IO setting, enable TX delay */
    MHal_EMAC_WritReg8(0x0033, 0xb2, 0x08);
    MHal_EMAC_WritReg8(0x0033, 0xb4, 0x18);
    MHal_EMAC_WritReg8(0x0033, 0xb0, 0xac);

    /* enable RX delay */
    MHal_EMAC_WritReg8(0x0033, 0xbb, 0x0f);

    /* Enable GT1 */
    uRegVal = MHal_EMAC_ReadReg8(0x0e, 0x39);
    uRegVal &= ~(0x01);
    MHal_EMAC_WritReg8(0x0e, 0x39, uRegVal);

    /* Digital synthesizer */
    MHal_EMAC_WritReg8(0x100b, 0xc6, 0x00);
    MHal_EMAC_WritReg8(0x110c, 0xd2, 0x1b);
    MHal_EMAC_WritReg8(0x110c, 0xc6, 0x00);


    /* Enable HW patch */
    //uRegVal = MHal_GMAC_ReadReg8(0x121c, 0x2d);
    //uRegVal |= 0x02;
    //MHal_GMAC_WritReg8(0x121c, 0x2d, uRegVal);
#endif
}

void MHal_EMAC_PadMux(void)
{
    //chiptop [15] allpad_in
    *( ( u32 * ) ( ( char * ) 0x1F203C00 + 0x50*4 ) ) &= 0x7FFF;
}

void MHal_EMAC_Power_On_Clk(void)
{
    MHal_EMAC_ClkGen();
    MHal_EMAC_PadMux();
}

void MHal_EMAC_Power_Off_Clk(void)
{

}
