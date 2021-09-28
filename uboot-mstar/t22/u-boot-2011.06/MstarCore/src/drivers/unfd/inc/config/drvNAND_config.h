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
#ifndef __UNFD_CONFIG_H__
#define __UNFD_CONFIG_H__

//=====================================================
// select a HW platform:
//   - 1: enable, 0: disable.
//   - only one platform can be 1, others have to be 0.
//=====================================================
#define NAND_DRV_TV_MBOOT           1


//=====================================================
// do NOT edit the following content.
//=====================================================
#include <config.h>

#if defined(NAND_DRV_TV_MBOOT) && NAND_DRV_TV_MBOOT
  #include <config.h>
  #include "drvNAND_uboot.h"
#else
  #error "Error! no platform selected."
#endif


//do NOT edit the following content, for HAL use.
//=====================================================
// misc. do NOT edit the following content.
//=====================================================
#define NAND_DMA_RACING_PATCH       1
#define NAND_DMA_PATCH_WAIT_TIME    10000 // us -> 10ms
#define NAND_DMA_RACING_PATTERN0    (((U32)'M'<<24)|((U32)0<<16)|((U32)'S'<<8)|(U32)1)
#define NAND_DMA_RACING_PATTERN1    (((U32)'T'<<24)|((U32)6<<16)|((U32)'A'<<8)|(U32)8)


//===========================================================
// Time Dalay, do NOT edit the following content, for NC_WaitComplete use.
//===========================================================
#define DELAY_100us_in_us           100
#define DELAY_500us_in_us           500
#define DELAY_1ms_in_us             1000
#define DELAY_10ms_in_us            10000
#define DELAY_100ms_in_us           100000
#define DELAY_500ms_in_us           500000
#define DELAY_1s_in_us              1000000

#define WAIT_ERASE_TIME             (DELAY_1s_in_us<<2)
#define WAIT_WRITE_TIME             (DELAY_1s_in_us<<2)
#define WAIT_READ_TIME              (DELAY_1ms_in_us<<10)  //DELAY_500us_in_us
#define WAIT_PROBE_TIME             (DELAY_100us_in_us<<2)
#define WAIT_RESET_TIME             (WAIT_ERASE_TIME)

//for system cache direction calls
#define WRITE_TO_NAND               0
#define READ_FROM_NAND              1
#define BIDIRECTIONAL			    2

extern void nand_reset_WatchDog(void);
extern U32  nand_clock_setting(U32 u32ClkParam);
extern U32  nand_config_clock(U16 u16_SeqAccessTime);
extern void nand_retry_dqs_post(void);
extern U32  nand_translate_DMA_address_Ex(U32 u32_DMAAddr, U32 u32_ByteCnt, int Mode);
extern U32  nand_translate_Spare_DMA_address_Ex(U32 u32_DMAAddr, U32 u32_ByteCnt, int Mode);
extern U32  nand_translate_DMA_MIUSel(U32 u32_DMAAddr, U32 u32_ByteCnt);
extern void nand_Invalidate_data_cache_buffer(U32 u32_addr, S32 s32_size);
extern U32  nand_WaitCompleteIntr(U16 u16_WaitEvent, U32 u32_MicroSec);
extern void nand_enable_intr_mode(void);
extern void nand_flush_miu_pipe(void);
extern void nand_check_blank_page_post_read(U8* main, U8* spare);
extern void nand_read_dma_post_flush(U32 u32_DMAAddr, U32 u32_ByteCnt);
extern U32  nand_pads_release(void);

extern int  drvNAND_CHECK_FLASH_TYPE(void);
extern void CheckPowerCut(void);

extern void HWTimer_Start(void);
extern U32  HWTimer_End(void);

#define NAND_PARTITAION_BYTE_CNT    0x200

#endif /* __UNFD_CONFIG_H__ */
