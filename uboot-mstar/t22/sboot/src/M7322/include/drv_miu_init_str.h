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

#ifndef _DRV_MIU_INIT_STR_H_
#define _DRV_MIU_INIT_STR_H_

#ifndef _BOARD_H_
#include "Board.h"
#endif

#ifndef _C_RIUBASE_H_
#include "c_riubase.h"
#endif

#define MIU_VER                         'M','I','U','_','M','7','3','2','2',' ','V','e','r',':','1','_','0','_','0'
#define REG_ADDR_BASE                   0x1f000000

#ifndef __ASSEMBLER__

#ifndef __DRV_RIU_H__
#include "drvRIU.h"
#endif

#if (ENABLE_MSTAR_TITANIA_BD_FPGA == 1)
#ifndef _MIU_FPGA_M7322_H_
#include "MIU_FPGA_M7322.h"
#endif
#endif

#if (ENABLE_MSTAR_BD_MST303A_D02A_S_M7322 == 1)
#ifndef _MIU_MST303A_D02A_S_M7322_STR_H_
#include "MIU_MST303A_D02A_S_M7322_STR.h"
#endif
#endif

#if (ENABLE_MSTAR_BD_MST303B_D01A_S_M7322 == 1)
#ifndef _MIU_MST303B_D01A_S_M7322_STR_H_
#include "MIU_MST303B_D01A_S_M7322_STR.h"
#endif
#endif

#if (ENABLE_MSTAR_BD_MST148B_10NQAT_18115_M7322 == 1)
#ifndef _MIU_MST148B_10NQAT_18115_M7322_STR_H_
#include "MIU_MST148B_10NQAT_18115_M7322_STR.h"
#endif
#endif

#if (ENABLE_MSTAR_BD_M7322_MB175 == 1)
#ifndef _MIU_M7322_MB175_STR_H_
#include "MIU_M7322_MB175_STR.h"
#endif
#endif

#if (ENABLE_MSTAR_BD_M7322_MB170 == 1)
#ifndef _MIU_M7322_MB170_STR_H_
#include "MIU_M7322_MB170_STR.h"
#endif
#endif

#if (ENABLE_MSTAR_BD_M7322_G22 == 1)
#ifndef _MIU_M7322_G22_STR_H_
#include "MIU_M7322_G22_STR.h"
#endif
#endif

#if (ENABLE_MSTAR_BD_M7322_G22_abc123 == 1)
#ifndef _MIU_M7322_G22_abc123_STR_H_
#include "MIU_M7322_G22_abc123_STR.h"
#endif
#endif

#if (ENABLE_MSTAR_BD_M7322_abc123 == 1)
#ifndef _MIU_M7322_abc123_STR_H_
#include "MIU_M7322_abc123_STR.h"
#endif
#endif


#if (ENABLE_MSTAR_BD_MST148B_10NQAT_18115_M7322_ARMANI == 1)
#ifndef _MIU_MST148B_10NQAT_18115_M7322_STR_ARMANI_H_
#include "MIU_MST148B_10NQAT_18115_M7322_STR_ARMANI.h"
#endif
#endif

#if (ENABLE_MSTAR_BD_MST148B_10NQAT_18115_M7322_TOPAZ == 1)
#ifndef _MIU_MST148B_10NQAT_18115_M7322_STR_TOPAZ_H_
#include "MIU_MST148B_10NQAT_18115_M7322_STR_TOPAZ.h"
#endif
#endif

#if (ENABLE_MSTAR_BD_MST148B_10NQAT_18115_M7322_CHIEF == 1)
#ifndef _MIU_MST148B_10NQAT_18115_M7322_STR_CHIEF_H_
#include "MIU_MST148B_10NQAT_18115_M7322_STR_CHIEF.h"
#endif
#endif

#if (ENABLE_MSTAR_BD_MST150B_10ANQHT_18151_M7322 == 1)
#ifndef _MIU_MST150B_10ANQHT_18151_M7322_STR_H_
#include "MIU_MST150B_10ANQHT_18151_M7322_STR.h"
#endif
#endif

const MS_REG_INIT MIU_PreInit_Str[] =
{
//MIU Channel Config Setting
#if defined(CONFIG_UMA)
    _RV32_2(0x152b26, 0x0002),
    _RV32_2(0x152b28, 0x0000),
    _RV32_2(0x152c28, 0x0001),
#else
    _RV32_2(0x152b26, 0x4c00), // 4'hc:2G for MIU0
    _RV32_2(0x152c26, 0x4000),
    _RV32_2(0x152b28, 0x0000),
    _RV32_2(0x152c28, 0x0002),
#endif

//MIU Reset & Mask all
    _RV32_2(0x10121e, 0x0c00),
    _RV32_2(0x10121e, 0x0c00),
    _RV32_2(0x10121e, 0x0c00),
    _RV32_2(0x10121e, 0x0c01),
    _RV32_2(0x1615e6, 0xfffe),

//MIU Reset & Mask all
    _RV32_2(0x10061e, 0x0c00),
    _RV32_2(0x10061e, 0x0c00),
    _RV32_2(0x10061e, 0x0c00),
    _RV32_2(0x10061e, 0x0c01),
    _RV32_2(0x1622e6, 0xfffe),

    _RV32_2(0x110d78, 0x0101),
    _RV32_2(0x110d36, 0x0000),
    _RV32_2(0x110d34, 0x0102),

    _END_OF_TBL32_,
    MIU_VER
};

//-----------------------------------------------
const MS_REG_INIT MIU_PostInit_Str[] =
{
//Unmask clients
    _RV32_2(0x310206, 0x0000),
    _RV32_2(0x310306, 0x0000),
    _RV32_2(0x310406, 0x0000),
    _RV32_2(0x310506, 0x0000),
    _RV32_2(0x310606, 0x0000),
    _RV32_2(0x310706, 0x0000),
    _RV32_2(0x310806, 0x0000),
    _RV32_2(0x310906, 0x0000),
    _RV32_2(0x311006, 0x0000),
    _RV32_2(0x1615e6, 0x0000),
    _RV32_2(0x310206, 0x0000),
    _RV32_2(0x310306, 0x0000),
    _RV32_2(0x310406, 0x0000),
    _RV32_2(0x310506, 0x0000),
    _RV32_2(0x310606, 0x0000),
    _RV32_2(0x310706, 0x0000),
    _RV32_2(0x310806, 0x0000),
    _RV32_2(0x310906, 0x0000),
    _RV32_2(0x311006, 0x0000),
    _RV32_2(0x1622e6, 0x0000),
    _RV32_2(0x310644, 0x0003),
    _RV32_2(0x310644, 0x0003),
    _RV32_2(0x310644, 0x0003),
    _RV32_2(0x1012fe, 0xa000),
    _RV32_2(0x10121e, 0x8008),
#if !defined(CONFIG_UMA)
    _RV32_2(0x10061e, 0x8008),
#endif

    _END_OF_TBL32_,
    MIU_VER
};


#endif /* !__ASSEMBLER__ */

#endif /* _DRV_MIU_INIT_H_ */
