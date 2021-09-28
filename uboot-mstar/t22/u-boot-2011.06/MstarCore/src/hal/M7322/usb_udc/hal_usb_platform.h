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

#ifndef __HAL_USB_PLATFORM_H__
#define __HAL_USB_PLATFORM_H__


#define VERSION "mainline_20180823_v1.0"

//#define ENABLE_FASTBOOT

/* USB Platform Enviroment Configurations */
/******************[ HARDWARE ]**********************/
#define M7322_PLATFORM

#define ENABLE_RIU_2BYTES_READ					/* CPU access FIFO w/ 2bytes mode */
#define FIFOCOUNT_MINIUS_01 		0x7FF		/* FIFO count -1 */

/****************************************************/

/*****************[ COMPILER ] **************************/
/* It is independent from OS & hardwar platform */
#define GCC_COMPILER

/************************************************************/

/* USB Other Enviroment Configurations */
/******************[ OTHER ]*************************/

/* High Speed/Full Speed Mode */
#define USB_20

/* Polling/Interrupt Mode */
#define POLLING_MODE

/* DMA/FIFO Mode */
#define DMA

/* Interactive */
/* #define FASTBOOT_INTERACTIVE */

/* EP layout */
#define EPCONTROL               0
#define EPBULKIN                1
#define EPBULKOUT               2
#define EPINTIN                 3
#define EPISO                   3

/********************[ OS View ]*****************************/
#define INCLUDE_LINUX_TYPE

/* functions must define for OS platfoms */
/* Sync functions */
#define USB_UVC_WAIT_ENCODE_SEM

#define Ms_flag_value_t s32
#define Ms_Flag_t u32

#define MsFlagInit(x)
#define MsFlagSetbits(x, y)
#define MsFlagWait(x, y, z)
#define MsInitSem(x, y)
#define MsConsumeSem(x)
#define MsProduceSem(x)
#define MsFlagDestroy(x)
#define MsFlagTimedWait(x, y, z, a) 0

#define _TRACE(var)
#define MsSleep(x)      udelay(x*1000);

/*******************[ PLATFORM(hardware) View ]**************/
#if defined( M7322_PLATFORM )

	#define RIU_BASE        0x1f200000

	#define UTMIBaseAddress (RIU_BASE + (0x03A80 << 1))
	#define USBCBaseAddress (RIU_BASE + (0x00700 << 1))
	#define OTGBaseAddress  (RIU_BASE + (0x21500 << 1))

	#define otgNumEPDefs            4

	/* #define MAGIC_DMA_ADDRESS_BASE               0xA0000000 */
	#define HalUtilPHY2MIUAddr(addr)        addr
	#define HalUtilMIU2PHYAddr(addr)        addr

	//------ UTMI disconnect level parameters -------------
	// 0x00: 550mv, 0x20: 575, 0x40: 600, 0x60: 625
	#define UTMI_DISCON_LEVEL_2A    (0x60 | 0x02)

	// for 40nm after Agate, use 55nm setting1, the default
	#define UTMI_EYE_SETTING_2C     (0x10)
	#define UTMI_EYE_SETTING_2D     (0x02)
	#define UTMI_EYE_SETTING_2E     (0x00)
	#define UTMI_EYE_SETTING_2F     (0x81)

	#define sys_Clean_and_flush_data_cache_buffer(addr, size)  flush_cache(addr, size)
#else
	#error # to do #
#endif

/*******************[ COMPILER View]*************************/
#if defined( GCC_COMPILER)

	#define ALIGNED(x) __attribute__ ((aligned (x)))

	/* tony include for fondemantal type define */
	#include <linux/types.h>
	#include <command.h>

	#include <linux/string.h>
	#define MsMemCpy(x, y, z) memcpy(x, y, z)
	#define MsMemSet(x, y, z) memset(x, y, z)

#else
	#define	ALIGNED(x) __align (x)
#endif

/******************[ Other Settings ]************************/
#define MIU_16BIT
#define otgOffShift     1

#define EVB_Board
#define MASS_BUFFER_SIZE	(4 * 1024)
#define MAX_DMA_CHANNEL		1

#define MASS_BUFFER_SIZE2	0x10000
#define MASS_TRN_LEN		8
#define Enable_Read_Write_Test
#define UVC_BULK_MODE

#define DRC_IN8(r)		*((u8 volatile *)(r))
#define DRC_OUT8(r,d)		(*((u8 volatile*)(r)) = (u8)d)
#define DRC_IN16(r)		*((u16 volatile*)(r))
#define DRC_OUT16(r,d)		(*((u16 volatile*)(r)) = (u16)d)

#define readb(r)	DRC_IN8(r)
#define writeb(v,a)	DRC_OUT8(a,v)
#define readw(r)	DRC_IN16(r)
#define writew(v,a)	DRC_IN16(a,v)

#define FIFO_ADDRESS(e)		(OTGBaseAddress + (e<<3) + M_FIFO_EP0)
#define FIFO_DATA_PORT		(OTGBaseAddress + M_REG_FIFO_DATA_PORT)

#ifdef BIG_ENDIAN
	#define SWOP(X)		((X) = (((X)<<8)+((X)>>8)))
	#define SWAP4(X)	((X) = ((X)<<24) + ((X)>>24) + (((X)>>8)&0x0000FF00) + (((X)<<8)&0x00FF0000) )
#else
	#define SWAP4(X)	(X = X)
	#define SWOP(X)		(X = X)
#endif
#endif	/* _HAL_USB_PLATFORM_H_ */
