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
#ifndef _FTL_CFG_H_
#define _FTL_CFG_H_

#include "../../inc/common/drvNAND.h" // MStar FTL bases on UNFD
//#include <math.h>


// ============================================
// [CAUTION]: must check with FCIE designers
// ============================================
#define FOR_FCIE5_BUG_WS    1 // WriteSector then ReadPage UECC
#define FOR_FCIE5_BUG_CP    1 // CopyPage would increase DataBuf

// ============================================
// [CAUTION]: select platform, only one can be 1 
// ============================================
#define MSFTL_UBOOT          1
#define MSFTL_LINUX          0
#define MSFTL_eCos           0

#if MSFTL_UBOOT
    #define FTL_ST_PLAT      0x80000000
    #include "ftl_cfg_uboot.h"
	
#elif MSFTL_LINUX
    #define FTL_ST_PLAT      0x40000000
    #include "ftl_cfg_linux.h"

#elif MSFTL_eCos
    #define FTL_ST_PLAT      0x20000000
    #include "ftl_cfg_eCos.h"
	
#else
    #error "[ftl_cfg.h] Err: no platform selected for MStar FTL."
#endif


// ============================================
// limitations
// ============================================
// [NAND]
#define MAX_PDAT_BYTECNT        (16*1024*MAX_PLANE_NUM) // limited by drvNandPageBuf (32KB).
#define MAX_PRDD_BYTECNT        (128*16*MAX_PLANE_NUM)   // limited by drvNandSpareBuf (4KB).

//------------------------------------
// limited by ga_tPairedPageMap (PPM array)
//#define MAX_BLK_PAGECNT         1024 

// limited by DBPM_CID::pu8_PBA2DBPM_LPIdx, 
// avoid the DBPM_CID keeps more than 1 page (2KB) for SLC case.
// beside, Micron 3D 128Gb TLC, 1 blk has 512 pages, no need 1024 pages so far.
#define MAX_BLK_PAGECNT         512 

//------------------------------------
#define MAX_PLANE_NUM           2
#define MAX_FTL_BLKCNT          ((U32)BBT_BYTECNT_MAX*8) // 16K, limited by FTL BBT (2KB bit-map, may enlarge for MLC)
// [FTL]
#define ROOT_BLK_CNT            3 // 3 good PBAs
#define MIN_CTRL_BLK_CNT        1

// [RDB, Ctrl] or [RDBPM, Ctrl] GetFB at a same flow.
// before RDB, do RDBPM eraly if necessary, RDB can not get DB then hit RDBPM. 
#define MIN_FREE_BLK_CNT        2 

//------------------------------------
// [CAUTION]: for SLC NAND, may need more blocks assigned to run by FTL partition.
//------------------------------------
#define ReservedBlkPerBase      26
#define BaseBlkCnt           0x100 

// ============================================
// Important options
// HW has power-loss detection, FTL does not need to do related handling.
// ============================================
#define HW_DETECT_PWR_LOSS      1


#endif // _FTL_CFG_H_
