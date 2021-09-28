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
#ifndef _FTLSd_CFG_UBOOT_H_
#define _FTLSd_CFG_UBOOT_H_

// =========================================
// the code connected to system
// =========================================

#include <config.h>
#include <common.h>
#include <command.h>
#include <malloc.h>
#include <watchdog.h>
#include <fat.h>
#include <drvWDT.h>
#include <MsMath.h>
#include <u-boot/crc.h>
#include <jffs2/load_kernel.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/partitions.h>

#include <linux/list.h>
#include <linux/string.h>
#include <jffs2/load_kernel.h>
#include <part.h>
#include <linux/list.h>
#include <div64.h>

#include "ftlsd_dbg.h"

extern U16 SecureNandEndBlkCnt;

// =========================================
// platform config
// =========================================
#define FTLSd_HW_TIMER_HZ             (12*1000*1000)

#define FTLSd_SET_FLAG(x)   gu32_FtlSdFlag |= (x)
#define FTLSd_CLR_FLAG(x)   gu32_FtlSdFlag &= ~(x)
#define FTLSd_CHK_FLAG(x)   (x == (gu32_FtlSdFlag & (x)))

extern int   FTLSd_BlkDev_Init(void);
extern void *FSdMALLOC(unsigned int);
extern void  FSdFREE(void*);

#define LOCK_FTL()                  
#define UNLOCK_FTL()                

extern void FTLSd_Sleep(U32 u32_us);
// =========================================
// debug
// =========================================

#define FTLSd_DBG_LV_LOG             (0) /* just print */
#define FTLSd_DBG_LV_ERR             (1) /* Error condition debug messages. */
#define FTLSd_DBG_LV_WARN            (2) /* Warning condition debug messages. */
#define FTLSd_DBG_LV_HIGH            (3) /* Debug messages (high debugging). */
#define FTLSd_DBG_LV_MEDIUM          (4) /* Debug messages. */
#define FTLSd_DBG_LV_LOW             (5) /* Debug messages (low debugging). */

#define FTLSd_DEBUG_LEVEL            FTLSd_DBG_LV_LOW
#define MSFTLSd_DEBUG_MSG            1

#if defined(MSFTLSd_DEBUG_MSG) && MSFTLSd_DEBUG_MSG
#define ftl_printf                      printf // FIXME

#define ftl_dbg(dbg_lv, tag, str, ...) \
    do { \
        if (dbg_lv > FTLSd_DEBUG_LEVEL || (FTLSd_Disable_ErrLog==1 && dbg_lv!=FTLSd_DBG_LV_ERR)) \
            break; \
        else { \
	        if (tag) \
            { \
                ftl_printf("[%s %u] ", __func__, __LINE__); \
            } \
            if (dbg_lv == FTLSd_DBG_LV_ERR) \
		        ftl_printf("FTLSd Err: "); \
		    else if (dbg_lv == FTLSd_DBG_LV_WARN) \
		        ftl_printf("FTLSd Warn: "); \
            ftl_printf(str, ##__VA_ARGS__); \
        } \
    } while(0)
#else /* _FTLSd_CFG_UBOOT_H_ */

#define ftl_dbg(enable, tag, str, ...) {}
#endif /* MSFTLSd_DEBUG_MSG */


// --------------------------
#if TEMP_CHECK
#define ftl_die()   {   \
    FTLSd_Disable_ErrLog=0;  ftl_dbg(0,0,"\n\n");\
    ftl_dbg(FTLSd_DBG_LV_ERR,1,"Fatal Error, Stop.\n\n"); FTLSd_DumpInfo();\
    while(1);}
#else
#define ftl_die()   {   \
    FTLSd_Disable_ErrLog=0;  ftl_dbg(0,0,"\n\n");\
    ftl_dbg(FTLSd_DBG_LV_ERR,1,"Fatal Error, Stop.\n\n"); FTLSd_DumpInfo();\
    return FTLSd_ERR_FATAL;}
#endif

#define ftl_stop() \
	{ftl_dbg(FTLSd_DBG_LV_LOG,1,"Stop."); while(1)  nand_reset_WatchDog();}

#endif // _FTLSd_CFG_UBOOT_H_
