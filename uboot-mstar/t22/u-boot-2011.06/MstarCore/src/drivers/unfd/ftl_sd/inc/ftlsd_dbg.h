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
#ifndef _FTLSd_DBG_H_
#define _FTLSd_DBG_H_

// =============================================
// ftlsd_dbg.c
// =============================================
extern void FTLSd_DumpInfo(void);
extern void FTLSd_DumpDevInfo(void);
extern U32  FTLSd_Die(void);

// ---------------------
#define FTLSd_PROFILE_WAF    0
#if FTLSd_PROFILE_WAF
extern U32 u32_API512BCnt, u32_API1MBCnt, u32_HAL512BCnt, u32_HAL1MBCnt;
#endif


// =============================================
// debug log
// =============================================
#define API_CHECK_PARAM   1
#define TEMP_CHECK        0 // can be removed

#define DBG_ON            0
#define DBG_TRACE         (1&DBG_ON)
#define DBG_API           (1&DBG_ON)
#define DBG_DATA          (1&DBG_ON)
#define DBG_DATA_2        (1&DBG_ON)

#define ftl_dbg_d2(dbg_lv, tag, str, ...) \
    {if(DBG_DATA_2) ftl_dbg(dbg_lv, tag, str, ##__VA_ARGS__);}

#define ftl_dbg_data(dbg_lv, tag, str, ...) \
    {if(DBG_DATA) ftl_dbg(dbg_lv, tag, str, ##__VA_ARGS__);}

#define ftl_dbg_api(dbg_lv, tag, str, ...) \
    {if(DBG_API) ftl_dbg(dbg_lv, tag, str, ##__VA_ARGS__);}

#define ftl_trace(str, ...)  {if(DBG_TRACE) ftl_dbg(FTLSd_DBG_LV_LOG,1, "trace "str, ##__VA_ARGS__);}


extern U32 FtlSdTest_DataWRC(U32 u32_TestLoop, U32 seed);
extern U32 FtlSdTest_DataRC(U32 u32_TestLoop, U32 seed);
#endif // _FTLSd_DBG_H_
