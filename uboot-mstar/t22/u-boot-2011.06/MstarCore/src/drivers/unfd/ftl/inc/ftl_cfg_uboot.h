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

#ifndef _FTL_CFG_UBOOT_H_
#define _FTL_CFG_UBOOT_H_

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

#include "ftl_dbg.h"

struct ftl {
	struct list_head link;
	char name[32];
	void *priv;
	uint has_init;
	uint clock;
	char part_config;
	char part_num;
	uint read_bl_len;
	uint write_bl_len;
	u64 capacity; // byte count
	block_dev_desc_t block_dev;
};

extern struct ftl *find_ftl_device(int dev_num);
extern int  ftl_register(struct ftl *pFtl);
extern int  ftl_initialize(void);
extern block_dev_desc_t *ftl_get_dev(int dev);
extern void print_ftl_devices(char separator);
extern int  get_ftl_num(void);
extern int  ftl_BlkDev_Init(void);


// =========================================
// platform config
// =========================================
#define FTL_HW_TIMER_HZ             (12*1000*1000)

#define FTL_SET_FLAG(x)   gu32_FtlFlag |= (x)
#define FTL_CLR_FLAG(x)   gu32_FtlFlag &= ~(x)
#define FTL_CHK_FLAG(x)   (x == (gu32_FtlFlag & (x)))

extern void *FMALLOC(unsigned int);
extern void FFREE(void*);

#define LOCK_FTL()                  
#define UNLOCK_FTL()                

// 2 background threads:
#define BKG_HK_THREAD               1 // 0: no background hk_thread
#define BKG_W_THREAD                0 // 1: background write thread

// SW features
#define PAGE_SIZE_RAM_BUFFER    0
#define HOT_DATA_EN             0
#define BKG_ERASE_DB            0
#define STATIC_WL               1
#define READ_DISTURB            1

#define READ_DISTURB_CNT        4

extern void NFTL_Sleep(U32 u32_us);
// =========================================
// debug
// =========================================

#define FTL_DBG_LV_LOG             (0) /* just print */
#define FTL_DBG_LV_ERR             (1) /* Error condition debug messages. */
#define FTL_DBG_LV_WARN            (2) /* Warning condition debug messages. */
#define FTL_DBG_LV_HIGH            (3) /* Debug messages (high debugging). */
#define FTL_DBG_LV_MEDIUM          (4) /* Debug messages. */
#define FTL_DBG_LV_LOW             (5) /* Debug messages (low debugging). */

#define FTL_DEBUG_LEVEL            FTL_DBG_LV_LOW
#define MSFTL_DEBUG_MSG            1

#if defined(MSFTL_DEBUG_MSG) && MSFTL_DEBUG_MSG
#define ftl_printf                      printf // FIXME

#define ftl_dbg(dbg_lv, tag, str, ...) \
    do { \
        if (dbg_lv > FTL_DEBUG_LEVEL || (FTL_Disable_ErrLog==1 && dbg_lv!=FTL_DBG_LV_ERR)) \
            break; \
        else { \
	        if (tag) \
            { \
                ftl_printf("[%s %u] ", __func__, __LINE__); \
            } \
            if (dbg_lv == FTL_DBG_LV_ERR) \
		        ftl_printf("FTL Err: "); \
		    else if (dbg_lv == FTL_DBG_LV_WARN) \
		        ftl_printf("FTL Warn: "); \
            ftl_printf(str, ##__VA_ARGS__); \
        } \
    } while(0)
#else /* _FTL_CFG_UBOOT_H_ */

#define ftl_dbg(enable, tag, str, ...) {}
#endif /* MSFTL_DEBUG_MSG */


// --------------------------
#if TEMP_CHECK
#define ftl_die()   {   \
    FTL_Disable_ErrLog=0;  ftl_dbg(0,0,"\n\n");\
    ftl_dbg(FTL_DBG_LV_ERR,1,"Fatal Error, Stop.\n\n"); NFTL_DumpInfo();\
    while(1);}
#else
#define ftl_die()   {   \
    FTL_Disable_ErrLog=0;  ftl_dbg(0,0,"\n\n");\
    ftl_dbg(FTL_DBG_LV_ERR,1,"Fatal Error, Stop.\n\n"); NFTL_DumpInfo();\
    return FTL_ERR_FATAL;}
#endif

#define ftl_stop() \
	{ftl_dbg(FTL_DBG_LV_LOG,1,"Stop."); while(1)  nand_reset_WatchDog();}

#endif // _FTL_CFG_UBOOT_H_
