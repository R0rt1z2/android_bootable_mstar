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

#ifndef __MHAL_MIU_H__
#define __MHAL_MIU_H__

#include <common.h>
#include <malloc.h>
#include <asm/io.h>
#include <configs/uboot_board_config.h>
#include <ShareType.h>
#include <MsTypes.h>

#define SUPPORT_MIU_NEW_PROTECT_API

#define MIUREG_BASE		            (0x1F200000)
#define	HAL_MIU_GP_NUM		        (7)         // MIU Group number
#define HAL_MIU_GP_PRIORITY_NUM     (4)         // MIU Group Priority number
#define	REG_MIU_GP_PRIORITY		    (0x7F << 1) //MIU group priority
#define PM_TOP_BASE                 0x1F000000
#define CHIP_ID                     0x001E00
#define CHIP_VER   					0x001E03


///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// Register Map
///
///////////////////////////////////////////////////////////////////////////////////////////////////
#define MIU0_REG_BASE				    (0x1200)
#define MIU1_REG_BASE				    (0x0600)
#define MIU2_REG_BASE				    (0x62000)

#define MIU0_ARBB_REG_BASE			    (0x52000)
#define MIU1_ARBB_REG_BASE			    (0x52100)
#define MIU2_ARBB_REG_BASE			    (0x52200)

//MIU select group 0
#define		REG_MIUSEL_GP0				(0x78 << 1)
//MIU select group 1
#define		REG_MIUSEL_GP1				(0x79 << 1)
//MIU select group 2
#define		REG_MIUSEL_GP2				(0x7A << 1)
//MIU select group 3
#define		REG_MIUSEL_GP3				(0x7B << 1)

//MIU select group 4
#define		REG_MIUSEL_GP4				(0x7C << 1)
//MIU select group 5
#define		REG_MIUSEL_GP5				(0x7D << 1)
//MIU select group 6, Bank 0x1520
#define		REG_MIUSEL_GP6				(0x78 << 1)
//MIU select group 7, Bank 0x1520
#define		REG_MIUSEL_GP7				(0x79 << 1)


////
U8 HAL_MIU_SelectMIU_UBoot(U8 bMIU1, U16 *au16SelMiu, U8 u8GpNum);

MS_U8* HAL_MIU_GetKernelProtectClients(void);
MS_U32* HAL_MIU_GetKernelProtectClientsEx(void);

MS_U64 HAL_MIU_PA2BA(MS_U64 u64PhyAddr);

MS_U64 HAL_MIU_BA2PA(MS_U64 u64BusAddr);

void HAL_MIU_ProtectDummyDRAM(void);

#endif //__MHAL_MIU_H__
