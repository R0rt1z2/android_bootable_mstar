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

#ifndef _BOARD_H_
#define _BOARD_H_

#ifndef _ENUM_DEVICE_H_
#include "Enum_Device.h"
#endif

#ifndef __MEMORY_MAP_SEL_H__
#include "mmap/MMap.h"
#endif

//------------------------------------------------------------------------------
#define ENABLE                          1
#define DISABLE                         0

//------------------------------UNUSED MACROS-----------------------------------
#define _FUNC_NOT_USED()                do {} while ( 0 )

//------------------------------MS_BOARD_TYPE_SEL-------------------------------
// Demo board made in Taiwan

#define BD_FPGA_M7322                   0xC000
#define BD_MST303A_D02A_S_M7322         0xC001
#define BD_MST303B_D01A_S_M7322         0xC002
#define BD_MST148B_10NQAT_18115_M7322   0xC003
#define BD_MST150B_10ANQHT_18151_M7322  0xC004
#define BD_MST148B_10NQAT_18115_M7322_ARMANI    0xC005
#define BD_MST148B_10NQAT_18115_M7322_TOPAZ     0xC006
#define BD_MST148B_10NQAT_18115_M7322_CHIEF     0xC007
#define BD_M7322_MB175 0xC008
#define BD_M7322_MB170 0xC009
#define BD_M7322_G22 0xC00A
#define BD_M7322_G22_abc123 0xC00B
#define BD_M7322_abc123 0xC00C


#define BD_UNKNOWN                      0xFFFF

#ifndef MS_BOARD_TYPE_SEL
#if defined (CONFIG_MSTAR_TITANIA_BD_FPGA)
#define MS_BOARD_TYPE_SEL               BD_FPGA_M7322
#include "BD_FPGA_M7322.h"

#elif defined(CONFIG_MSTAR_BD_MST303A_D02A_S_M7322)
#define MS_BOARD_TYPE_SEL               BD_MST303A_D02A_S_M7322
#include "BD_MST303A_D02A_S_M7322.h"

#elif defined(CONFIG_MSTAR_BD_MST303B_D01A_S_M7322)
#define MS_BOARD_TYPE_SEL               BD_MST303B_D01A_S_M7322
#include "BD_MST303B_D01A_S_M7322.h"

#elif defined(CONFIG_MSTAR_BD_MST148B_10NQAT_18115_M7322)
#define MS_BOARD_TYPE_SEL               BD_MST148B_10NQAT_18115_M7322
#include "BD_MST148B_10NQAT_18115_M7322.h"

#elif defined(CONFIG_MSTAR_BD_MST148B_10NQAT_18115_M7322_ARMANI)
#define MS_BOARD_TYPE_SEL               BD_MST148B_10NQAT_18115_M7322_ARMANI
#include "BD_MST148B_10NQAT_18115_M7322_ARMANI.h"

#elif defined(CONFIG_MSTAR_BD_MST148B_10NQAT_18115_M7322_TOPAZ)
#define MS_BOARD_TYPE_SEL               BD_MST148B_10NQAT_18115_M7322_TOPAZ
#include "BD_MST148B_10NQAT_18115_M7322_TOPAZ.h"

#elif defined(CONFIG_MSTAR_BD_MST148B_10NQAT_18115_M7322_CHIEF)
#define MS_BOARD_TYPE_SEL               BD_MST148B_10NQAT_18115_M7322_CHIEF
#include "BD_MST148B_10NQAT_18115_M7322_CHIEF.h"

#elif defined(CONFIG_MSTAR_BD_MST150B_10ANQHT_18151_M7322)
#define MS_BOARD_TYPE_SEL               BD_MST150B_10ANQHT_18151_M7322
#include "BD_MST150B_10ANQHT_18151_M7322.h"

#elif defined(CONFIG_MSTAR_BD_M7322_MB175)
#define MS_BOARD_TYPE_SEL               BD_M7322_MB175
#include "BD_M7322_MB175.h"

#elif defined(CONFIG_MSTAR_BD_M7322_MB170)
#define MS_BOARD_TYPE_SEL               BD_M7322_MB170
#include "BD_M7322_MB170.h"

#elif defined(CONFIG_MSTAR_BD_M7322_G22)
#define MS_BOARD_TYPE_SEL               BD_M7322_G22
#include "BD_M7322_G22.h"

#elif defined(CONFIG_MSTAR_BD_M7322_G22_abc123)
#define MS_BOARD_TYPE_SEL               BD_M7322_G22_abc123
#include "BD_M7322_G22_abc123.h"

#elif defined(CONFIG_MSTAR_BD_M7322_abc123)
#define MS_BOARD_TYPE_SEL               BD_M7322_abc123
#include "BD_M7322_abc123.h"

#else
#error "Unknown Board Selection"

#endif
#endif

#endif // _BOARD_H_
