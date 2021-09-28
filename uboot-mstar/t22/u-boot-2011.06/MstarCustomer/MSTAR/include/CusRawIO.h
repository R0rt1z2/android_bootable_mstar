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

#ifndef __CUSRAWIO_H__
#define __CUSRAWIO_H__

// MTK Loader Info
#define EMMC_MBOOTA_PAR_NAME      "MBOOTA"
#define EMMC_MPOOLA_PAR_NAME      "MPOOLA"
#define EMMC_MBOOTB_PAR_NAME      "MBOOTB"
#define EMMC_MPOOLB_PAR_NAME      "MPOOLB"

#define UNNECESSARY_PARTITION ""
#define UNNECESSARY_VOLUME ""

#define SPI_DEFAULT_PARTITION UNNECESSARY_PARTITION
#define SPI_DEFAULT_VOLUME UNNECESSARY_VOLUME

#define NAND_DEFAULT_PARTITION "UBILD"
#define NAND_DEFAULT_VOLUME "MPOOL"

#ifdef CONFIG_MTK_LOADER
#define MMC_DEFAULT_PARTITION EMMC_MPOOLA_PAR_NAME
#else
#define MMC_DEFAULT_PARTITION "MPOOL"
#endif
#define MMC_DEFAULT_VOLUME UNNECESSARY_VOLUME

#define UFS_DEFAULT_PARTITION "MPOOL"
#define UFS_DEFAULT_VOLUME UNNECESSARY_VOLUME

#if defined (CONFIG_MBOOT_IN_SPI_FLASH)
#define FLASH_MBOOT_TARGET  E_RAW_DATA_IN_SPI
#define FLASH_MBOOT_PARTITION SPI_DEFAULT_PARTITION
#define FLASH_MBOOT_VOLUME SPI_DEFAULT_VOLUME
#elif defined (CONFIG_MBOOT_IN_NAND_FLASH)
#define FLASH_MBOOT_TARGET  E_RAW_DATA_IN_NAND
#define FLASH_MBOOT_PARTITION "MBOOT"
#define FLASH_MBOOT_VOLUME UNNECESSARY_PARTITION
#elif defined  (CONFIG_MBOOT_IN_MMC_FLASH)
#define FLASH_MBOOT_TARGET  E_RAW_DATA_IN_MMC
  #ifdef CONFIG_MTK_LOADER
    #define FLASH_MBOOT_PARTITION EMMC_MBOOTA_PAR_NAME
  #else
    #define FLASH_MBOOT_PARTITION "MBOOT"
  #endif
#define FLASH_MBOOT_VOLUME MMC_DEFAULT_VOLUME
#elif defined  (CONFIG_MBOOT_IN_UFS_FLASH)
#define FLASH_MBOOT_TARGET  E_RAW_DATA_IN_UFS
#define FLASH_MBOOT_PARTITION "MBOOT"
#define FLASH_MBOOT_VOLUME UFS_DEFAULT_VOLUME
#else
#error "please set the correct Mboot storage!!\n"
#endif



#endif //__CUSRAWIO_H__

