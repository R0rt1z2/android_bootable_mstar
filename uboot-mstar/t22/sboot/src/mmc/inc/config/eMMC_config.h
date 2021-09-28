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
#ifndef __eMMC_CONFIG_H__
#define __eMMC_CONFIG_H__

//=====================================================
// select a HW platform:
//   - 1: enable, 0: disable.
//   - only one platform can be 1, others have to be 0.
//   - search and check all [FIXME] if need modify or not
//=====================================================
//#include <config.h>

#define UNIFIED_eMMC_DRIVER         1

#ifdef CONFIG_MSTAR_EAGLE
#define eMMC_DRV_EAGLE_BOOTRAM      1
#endif

#ifdef CONFIG_MSTAR_EDISON
#define eMMC_DRV_EDISON_BOOTRAM     1
#endif

#ifdef CONFIG_MSTAR_EIFFEL
#define eMMC_DRV_EIFFEL_BOOTRAM     1
#endif

#ifdef CONFIG_MSTAR_NIKE
#define eMMC_DRV_NIKE_BOOTRAM       1
#endif

#ifdef CONFIG_MSTAR_MADISON
#define eMMC_DRV_MADISON_BOOTRAM    1
#endif

#ifdef CONFIG_MSTAR_CLIPPERS
#define eMMC_DRV_CLIPPERS_BOOTRAM   1
#endif

#ifdef CONFIG_MSTAR_MIAMI
#define eMMC_DRV_MIAMI_BOOTRAM      1
#endif

#ifdef CONFIG_MSTAR_MONACO
#define eMMC_DRV_MONACO_BOOTRAM     1
#endif

#ifdef CONFIG_MSTAR_MUJI
#define eMMC_DRV_MUJI_BOOTRAM       1
#endif

#ifdef CONFIG_MSTAR_CELTICS
#define eMMC_DRV_CELTICS_BOOTRAM    1
#endif

#ifdef CONFIG_MSTAR_NADAL
#define eMMC_DRV_NADAL_BOOTRAM      1
#endif

#ifdef CONFIG_MSTAR_EINSTEIN
#define eMMC_DRV_EINSTEIN_BOOTRAM   1
#endif

#ifdef CONFIG_MSTAR_EINSTEIN3
#define eMMC_DRV_EINSTEIN3_BOOTRAM  1
#endif

#ifdef CONFIG_MSTAR_NAPOLI
#define eMMC_DRV_NAPOLI_BOOTRAM     1
#endif

#ifdef CONFIG_MSTAR_MONET
#define eMMC_DRV_MONET_BOOTRAM      1
#endif

#ifdef CONFIG_MSTAR_MOONEY
#define eMMC_DRV_MOONEY_BOOTRAM      1
#endif

#ifdef CONFIG_MSTAR_MESSI
#define eMMC_DRV_MESSI_BOOTRAM      1
#endif

#ifdef CONFIG_MSTAR_MUNICH
#define eMMC_DRV_MUNICH_BOOTRAM         1
#endif

#ifdef CONFIG_MSTAR_MANHATTAN
#define eMMC_DRV_MANHATTAN_BOOTRAM      1
#endif

#ifdef CONFIG_MSTAR_MASERATI
#define eMMC_DRV_MASERATI_BOOTRAM       1
#endif

#ifdef CONFIG_MSTAR_MACAN
#define eMMC_DRV_MACAN_BOOTRAM          1
#endif

#ifdef CONFIG_MSTAR_MAZDA
#define eMMC_DRV_MAZDA_BOOTRAM          1
#endif

#ifdef CONFIG_MSTAR_M3822
#define eMMC_DRV_M3822_BOOTRAM          1
#endif

#ifdef CONFIG_MSTAR_MUSTANG
#define eMMC_DRV_MUSTANG_BOOTRAM        1
#endif

#ifdef CONFIG_MSTAR_MAXIM
#define eMMC_DRV_MAXIM_BOOTRAM          1
#endif

#ifdef CONFIG_MSTAR_M7621
#define eMMC_DRV_M7621_BOOTRAM         1
#endif

#ifdef CONFIG_MSTAR_M5621
#define eMMC_DRV_M5621_BOOTRAM         1
#endif

#ifdef CONFIG_MSTAR_MATISSE
#define eMMC_DRV_MATISSE_BOOTRAM        1
#endif

#ifdef CONFIG_MSTAR_MAINZ
#define eMMC_DRV_MAINZ_BOOTRAM          1
#endif

#ifdef CONFIG_MSTAR_M7221
#define eMMC_DRV_M7221_BOOTRAM          1
#endif

#ifdef CONFIG_MSTAR_M7821
#define eMMC_DRV_M7821_BOOTRAM         1
#endif

#ifdef CONFIG_MSTAR_M7322
#define eMMC_DRV_M7322_BOOTRAM         1
#endif

#ifdef CONFIG_MSTAR_M7622
#define eMMC_DRV_M7622_BOOTRAM         1
#endif

//=====================================================
// do NOT edit the following content.
//=====================================================
#if defined(eMMC_DRV_EAGLE_BOOTRAM) && eMMC_DRV_EAGLE_BOOTRAM
  #include "eMMC_eagle_bootram.h"
#elif defined(eMMC_DRV_EDISON_BOOTRAM) && eMMC_DRV_EDISON_BOOTRAM
  #include "eMMC_edison_bootram.h"
#elif defined(eMMC_DRV_EIFFEL_BOOTRAM) && eMMC_DRV_EIFFEL_BOOTRAM
  #include "eMMC_eiffel_bootram.h"
#elif defined(eMMC_DRV_NIKE_BOOTRAM) && eMMC_DRV_NIKE_BOOTRAM
  #include "eMMC_nike_bootram.h"
#elif defined(eMMC_DRV_MADISON_BOOTRAM) && eMMC_DRV_MADISON_BOOTRAM
  #include "eMMC_madison_bootram.h"
#elif defined(eMMC_DRV_CLIPPERS_BOOTRAM) && eMMC_DRV_CLIPPERS_BOOTRAM
  #include "eMMC_clippers_bootram.h"
#elif defined(eMMC_DRV_MIAMI_BOOTRAM) && eMMC_DRV_MIAMI_BOOTRAM
  #include "eMMC_miami_bootram.h"
#elif defined(eMMC_DRV_MONACO_BOOTRAM) && eMMC_DRV_MONACO_BOOTRAM
  #include "eMMC_monaco_bootram.h"
#elif defined(eMMC_DRV_MUJI_BOOTRAM) && eMMC_DRV_MUJI_BOOTRAM
  #include "eMMC_muji_bootram.h"
#elif defined(eMMC_DRV_CELTICS_BOOTRAM) && eMMC_DRV_CELTICS_BOOTRAM
  #include "eMMC_celtics_bootram.h"
#elif defined(eMMC_DRV_NADAL_BOOTRAM) && eMMC_DRV_NADAL_BOOTRAM
    #include "eMMC_nadal_bootram.h"
#elif defined(eMMC_DRV_EINSTEIN_BOOTRAM) && eMMC_DRV_EINSTEIN_BOOTRAM
  #include "eMMC_einstein_bootram.h"
#elif defined(eMMC_DRV_EINSTEIN3_BOOTRAM) && eMMC_DRV_EINSTEIN3_BOOTRAM
  #include "eMMC_einstein3_bootram.h"
#elif defined(eMMC_DRV_NAPOLI_BOOTRAM) && eMMC_DRV_NAPOLI_BOOTRAM
  #include "eMMC_napoli_bootram.h"
#elif defined(eMMC_DRV_MONET_BOOTRAM) && eMMC_DRV_MONET_BOOTRAM
  #include "eMMC_monet_bootram.h"
#elif defined(eMMC_DRV_MOONEY_BOOTRAM) && eMMC_DRV_MOONEY_BOOTRAM
  #include "eMMC_mooney_bootram.h"
#elif defined(eMMC_DRV_MESSI_BOOTRAM) && eMMC_DRV_MESSI_BOOTRAM
  #include "eMMC_messi_bootram.h"
#elif defined(eMMC_DRV_MUNICH_BOOTRAM) && eMMC_DRV_MUNICH_BOOTRAM
  #include "eMMC_munich_bootram.h"
#elif defined(eMMC_DRV_MANHATTAN_BOOTRAM) && eMMC_DRV_MANHATTAN_BOOTRAM
  #include "eMMC_manhattan_bootram.h"
#elif defined(eMMC_DRV_MASERATI_BOOTRAM) && eMMC_DRV_MASERATI_BOOTRAM
  #include "eMMC_maserati_bootram.h"
#elif defined(eMMC_DRV_MACAN_BOOTRAM) && eMMC_DRV_MACAN_BOOTRAM
  #include "eMMC_macan_bootram.h"
#elif defined(eMMC_DRV_MAZDA_BOOTRAM) && eMMC_DRV_MAZDA_BOOTRAM
  #include "eMMC_mazda_bootram.h"
#elif defined(eMMC_DRV_M3822_BOOTRAM) && eMMC_DRV_M3822_BOOTRAM
  #include "eMMC_m3822_bootram.h"
#elif defined(eMMC_DRV_MUSTANG_BOOTRAM) && eMMC_DRV_MUSTANG_BOOTRAM
  #include "eMMC_mustang_bootram.h"
#elif defined(eMMC_DRV_MAXIM_BOOTRAM) && eMMC_DRV_MAXIM_BOOTRAM
  #include "eMMC_maxim_bootram.h"
#elif defined(eMMC_DRV_M7621_BOOTRAM) && eMMC_DRV_M7621_BOOTRAM
  #include "eMMC_M7621_bootram.h"
#elif defined(eMMC_DRV_M5621_BOOTRAM) && eMMC_DRV_M5621_BOOTRAM
  #include "eMMC_M5621_bootram.h"
#elif defined(eMMC_DRV_MATISSE_BOOTRAM) && eMMC_DRV_MATISSE_BOOTRAM
  #include "eMMC_matisse_bootram.h"
#elif defined(eMMC_DRV_MAINZ_BOOTRAM) && eMMC_DRV_MAINZ_BOOTRAM
  #include "eMMC_mainz_bootram.h"
#elif defined(eMMC_DRV_M7221_BOOTRAM) && eMMC_DRV_M7221_BOOTRAM
  #include "eMMC_m7221_bootram.h"
#elif defined(eMMC_DRV_M7821_BOOTRAM) && eMMC_DRV_M7821_BOOTRAM
  #include "eMMC_M7821_bootram.h"
#elif defined(eMMC_DRV_M7322_BOOTRAM) && eMMC_DRV_M7322_BOOTRAM
  #include "eMMC_M7322_bootram.h"
#elif defined(eMMC_DRV_M7622_BOOTRAM) && eMMC_DRV_M7622_BOOTRAM
  #include "eMMC_M7622_bootram.h"
#else
  #error "Error! no platform selected."
#endif


//=====================================================
// misc. do NOT edit the following content.
//=====================================================
#define eMMC_DMA_RACING_PATCH        1
#define eMMC_DMA_PATCH_WAIT_TIME     DELAY_10ms_in_us
#define eMMC_DMA_RACING_PATTERN0     (((U32)'M'<<24)|((U32)0<<16)|((U32)'S'<<8)|(U32)1)
#define eMMC_DMA_RACING_PATTERN1     (((U32)'T'<<24)|((U32)6<<16)|((U32)'A'<<8)|(U32)8)

//===========================================================
// Time Dalay, do NOT edit the following content
//===========================================================
#if defined(eMMC_UPDATE_FIRMWARE) && (eMMC_UPDATE_FIRMWARE)
#define TIME_WAIT_DAT0_HIGH            (HW_TIMER_DELAY_1s*10)
#define TIME_WAIT_FCIE_RESET           (HW_TIMER_DELAY_1s*10)
#define TIME_WAIT_FCIE_RST_TOGGLE_CNT  (HW_TIMER_DELAY_1s*10)
#define TIME_WAIT_FIFOCLK_RDY          (HW_TIMER_DELAY_1s*10)
#define TIME_WAIT_CMDRSP_END           (HW_TIMER_DELAY_1s*10)
#define TIME_WAIT_1_BLK_END            (HW_TIMER_DELAY_1s*5)
#define TIME_WAIT_n_BLK_END            (HW_TIMER_DELAY_1s*10) // safe for 512 blocks
#else
#define TIME_WAIT_DAT0_HIGH            HW_TIMER_DELAY_1s
#define TIME_WAIT_ERASE_DAT0_HIGH      (HW_TIMER_DELAY_1s*60*10) //10*60 s
#define TIME_WAIT_FCIE_RESET           HW_TIMER_DELAY_10ms
#define TIME_WAIT_FCIE_RST_TOGGLE_CNT  HW_TIMER_DELAY_1us
#define TIME_WAIT_FIFOCLK_RDY          HW_TIMER_DELAY_10ms
#define TIME_WAIT_CMDRSP_END           HW_TIMER_DELAY_10ms
#define TIME_WAIT_1_BLK_END            HW_TIMER_DELAY_1s
#define TIME_WAIT_n_BLK_END            HW_TIMER_DELAY_1s // safe for 512 blocks
#endif




#endif /* __eMMC_CONFIG_H__ */
