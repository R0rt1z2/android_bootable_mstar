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

#ifndef __DRV_RIU_H__
#define __DRV_RIU_H__

typedef unsigned char MS_U8;
typedef MS_U8 MS_REG_INIT;

#define _RV1(addr, value)           (((addr) >> 8) & 0x3F), (MS_U8)(addr), (MS_U8)(value)
#define _RV2(addr, value)           0x40 + _RV1(addr, value), (MS_U8)((value) >> 8)
#define _RV3(addr, value)           0x40 + _RV2(addr, value), (MS_U8)((value) >> 16)
#define _RV4(addr, value)           0x40 + _RV3(addr, value), (MS_U8)((value) >> 24)

#define _RVM1(addr, value, mask)    (((addr) >> 8) & 0x3F), (MS_U8)(addr), (MS_U8)(value), (MS_U8)(mask)
#define _RVM2(addr, value, mask)    0x40 + _RVM1(addr, value, mask), (MS_U8)((value) >> 8), (MS_U8)((mask) >> 8)
#define _RVM3(addr, value, mask)    0x40 + _RVM2(addr, value, mask), (MS_U8)((value) >> 16), (MS_U8)((mask) >> 16)
#define _RVM4(addr, value, mask)    0x40 + _RVM3(addr, value, mask), (MS_U8)((value) >> 24), (MS_U8)((mask) >> 24)
#define _END_OF_TBL_                0xFF, 0xFF

#if defined(__arm__) || defined(__arm64__)
#define _RV32_1(addr, value)        (MS_U8)(((addr) >> 16) & 0xFF), (MS_U8)(((addr) >> 8) & 0xFF), (MS_U8)(addr), (MS_U8)(value)
#define _RV32_2(addr, value)        0x40 + _RV32_1(addr, value), (MS_U8)((value) >> 8)
#define _RV32_3(addr, value)        0x40 + _RV32_2(addr, value), (MS_U8)((value) >> 16)
#define _RV32_4(addr, value)        0x40 + _RV32_3(addr, value), (MS_U8)((value) >> 24)
#else
#define _RV32_1(addr, value)        (MS_U8)(((addr) >> 16) & 0xFF), (MS_U8)(((addr) >> 8) & 0xFF), (MS_U8)(addr), (MS_U8)(value)
#define _RV32_2(addr, value)        0x20 + _RV32_1(addr, value), (MS_U8)((value) >> 8)
#define _RV32_3(addr, value)        0x20 + _RV32_2(addr, value), (MS_U8)((value) >> 16)
#define _RV32_4(addr, value)        0x20 + _RV32_3(addr, value), (MS_U8)((value) >> 24)
#endif

#define _RVM32_1(addr, value, mask) (MS_U8)(((addr) >> 16) & 0xFF), (MS_U8)(((addr) >> 8) & 0xFF), (MS_U8)(addr), (MS_U8)(value), (MS_U8)(mask)
#define _RVM32_2(addr, value, mask) 0x20 + _RVM32_1(addr, value, mask), (MS_U8)((value) >> 8), (MS_U8)((mask) >> 8)
#define _RVM32_3(addr, value, mask) 0x20 + _RVM32_2(addr, value, mask), (MS_U8)((value) >> 16), (MS_U8)((mask) >> 16)
#define _RVM32_4(addr, value, mask) 0x20 + _RVM32_3(addr, value, mask), (MS_U8)((value) >> 24), (MS_U8)((mask) >> 24)
#define _END_OF_TBL32_              0xFF, 0xFF, 0xFF

#ifdef __cplusplus
extern "C" {
#endif

void MDrv_WriteRegMaskTbl(const MS_REG_INIT *pRegMaskTbl);
void MDrv_WriteRegTbl32(const MS_REG_INIT *pRegTbl32);
void MDrv_WriteRegTbl32Off(const MS_REG_INIT *pRegTbl32, int iRegOffset);

#ifdef __cplusplus
}
#endif

#endif /* __DRV_RIU_H__ */
