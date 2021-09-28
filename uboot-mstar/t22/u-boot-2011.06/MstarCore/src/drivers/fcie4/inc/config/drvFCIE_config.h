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

#ifndef __DRVFCIE_CONFIG_H__
#define __DRVFCIE_CONFIG_H__

#define FALSE           0
#define TRUE            1

#ifdef CONFIG_AMBER5
#include "drvFCIE_a5_uboot.h"
#endif

#if (ENABLE_EAGLE)
#include "drvFCIE_eagle_uboot.h"
#elif (ENABLE_EIFFEL)
#include "drvFCIE_eiffel_uboot.h"
#elif (ENABLE_NIKE)
#include "drvFCIE_nike_uboot.h"
#elif (ENABLE_MADISON)
#include "drvFCIE_madison_uboot.h"
#elif (ENABLE_CLIPPERS)
#include "drvFCIE_clippers_uboot.h"
#elif (ENABLE_MIAMI)
#include "drvFCIE_miami_uboot.h"
#elif (ENABLE_NADAL)
#include "drvFCIE_nadal_uboot.h"
#elif (ENABLE_AMBER3)
#include "drvFCIE_a3_uboot.h"
#elif (ENABLE_AGATE)
#include "drvFCIE_agate_uboot.h"
#elif (ENABLE_EINSTEIN)
#include "drvFCIE_einstein_uboot.h"
#elif (ENABLE_NAPOLI)
#include "drvFCIE_napoli_uboot.h"
#elif (ENABLE_MONACO)
#include "drvFCIE_monaco_uboot.h"
#elif (ENABLE_MONET)
#include "drvFCIE_monet_uboot.h"
#elif (ENABLE_MESSI)
#include "drvFCIE_messi_uboot.h"
#elif (ENABLE_MUJI)
#include "drvFCIE_muji_uboot.h"
#elif (ENABLE_CELTICS)
#include "drvFCIE_celtics_uboot.h"
#elif (ENABLE_KAISER)
#include "drvFCIE_kaiser_uboot.h"
#elif (ENABLE_MUNICH)
#include "drvFCIE_munich_uboot.h"
#elif (ENABLE_MANHATTAN)
#include "drvFCIE_manhattan_uboot.h"
#elif (ENABLE_KANO)
#include "drvFCIE_kano_uboot.h"
#elif (ENABLE_MACAN)
#include "drvFCIE_macan_uboot.h"
#elif (ENABLE_MAZDA)
#include "drvFCIE_mazda_uboot.h"
#elif (ENABLE_CURRY)
#include "drvFCIE_curry_uboot.h"
#elif (ENABLE_C2P)
#include "drvFCIE_c2p_uboot.h"
#elif (ENABLE_C2S)
#include "drvFCIE_c2s_uboot.h"
#elif (ENABLE_KASTOR)
#include "drvFCIE_kastor_uboot.h"
#elif (ENABLE_MASERATI)
#include "drvFCIE_maserati_uboot.h"
#elif (ENABLE_KAYLA)
#include "drvFCIE_kayla_uboot.h"
#elif (ENABLE_K5TN)
#include "drvFCIE_kentucky_uboot.h"
#else
#error "Wrong chip setting!"
#endif

#define CARDDET_CDZ         BIT00
#define CARDDET_INSERT_LOW  BIT01
#define CARDDET_INSERT_HIGH BIT02

#define NONE			"\033[m"
#define RED				"\033[0;32;31m"
#define LIGHT_RED		"\033[1;31m"
#define GREEN			"\033[0;32;32m"
#define LIGHT_GREEN		"\033[1;32m"
#define BLUE			"\033[0;32;34m"
#define LIGHT_BLUE		"\033[1;34m"
#define DARY_GRAY		"\033[1;30m"
#define CYAN			"\033[0;36m"
#define LIGHT_CYAN		"\033[1;36m"
#define PURPLE			"\033[0;35m"
#define LIGHT_PURPLE	"\033[1;35m"
#define BROWN			"\033[0;33m"
#define YELLOW			"\033[1;33m"
#define LIGHT_GRAY		"\033[0;37m"
#define WHITE			"\033[1;37m"

#define U32BEND2LEND(X) ( ((X&0x000000FF)<<24) + ((X&0x0000FF00)<<8) + ((X&0x00FF0000)>>8) + ((X&0xFF000000)>>24) )
#define U16BEND2LEND(X) ( ((X&0x00FF)<<8) + ((X&0xFF00)>>8) )

void	HalSdio_ResetIP(void);
void	HalSdio_Platform_InitChiptop(void);
U8		HalSdio_GetPadType(void);
void	HalSdio_SwitchPad(unsigned char);
U32		HalSdio_clock_setting(U16 u16_ClkParam);
void	HalSdioDelayMs(U32 u32Ms);
void	HalSdioDelayUs(U32 u32Us);
U32		HalSdio_SetClock(U32 u32Clock);
void	HalSdio_DumpDebugBus(void);

#endif // #ifndef __DRVFCIE_CONFIG_H__


