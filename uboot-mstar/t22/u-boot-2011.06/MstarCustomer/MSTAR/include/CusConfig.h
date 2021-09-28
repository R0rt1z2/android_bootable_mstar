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

#ifndef __CUS_CONFIG_H__
#define __CUS_CONFIG_H__

//******** This part is for boot ********
#if(ENABLE_MODULE_ANDROID_BOOT == 1)
#define KL_PARTITION "boot"
#else
#define KL_PARTITION "KL"
#endif
#define TEE_PARTITION "tee"
#define SECKL_PARTITION "secKL"
//***************************************

#if (ENABLE_MODULE_ANDROID_BOOT == 1)
#define UBIPART  "ubi"
/*******************************************************************************
* 1,if CONFIG_PATH is null, you should put "boot0.mp3, boot0.jpg, boot.ini" to 
*   /tvconfig; otherwise, you should put these files to /tvconfig/CONFIG_PATH.
*
* 2,if CUSTOMER_PATH is null, you should put "sys.ini, mmap.ini" to /tvconfig
*   otherwise, you should put these files to /tvconfig/CUSTOMER_PATH.
*******************************************************************************/
#define ODM "odm"
#define CONFIG   "tvconfig"
#define CUSTOMER "tvconfig"
//#define CONFIG_PATH   ""
#define CONFIG_PATH   "/config"
#define CUSTOMER_PATH  ""
#define CHECK_FLASH_EMPTY "tvconfig"
#define SET_ENV_PATITION "system"
#define SET_ENV_FILE "/etc/set_env"
//#define CUSTOMER_PATH  "/Customer"
#elif(ENABLE_STB_CONFIG_PARTITION==1)	// STB Partition Cfg.
  #define UBIPART               "UBI"
  #define CONFIG                "CON"
  #define CUSTOMER              "CON"
  #define CONFIG_PATH           ""
  #define CUSTOMER_PATH         "customer"
  #define CHECK_FLASH_EMPTY     "CON"
  #define SET_ENV_PATITION      "system"
  #define SET_ENV_FILE          "/etc/set_env"
#elif(CONFIG_MI_CONFIG_PARTITION==1)  //MI pure linux cfg
  #define UBIPART  "UBI"
  #define CONFIG   "tvconfig"
  #define CUSTOMER "tvconfig"
  #define CONFIG_PATH   "/config"
  #define CUSTOMER_PATH  ""
  #define CHECK_FLASH_EMPTY "customerbackup"
  #define SET_ENV_PATITION "tvconfig"
  #define SET_ENV_FILE "/set_env"
#elif(ENABLE_CAST_CONFIG_PARTITION==1)  //Cast TV Partition Cfg
  #define UBIPART  "UBI"
  #define CONFIG   "CONFIG"
  #define CUSTOMER "CONFIG"
  #define CONFIG_PATH   ""
  #define CUSTOMER_PATH  ""
  #define CHECK_FLASH_EMPTY "customerbackup"
  #define SET_ENV_PATITION "CONFIG"
  #define SET_ENV_FILE "/set_env"
#else
#define UBIPART  "UBI"
#define CONFIG   "CONFIG"
#define CUSTOMER "customer"
#define CERTIFICATE "certificate"
#define CONFIG_PATH   ""
#define CUSTOMER_PATH  ""
#define CHECK_FLASH_EMPTY "customerbackup"
#define SET_ENV_PATITION "CONFIG"
#define SET_ENV_FILE "/set_env"
#define CUSDATA   "cusdata"
#define CUSDATA_CUSTOMER_PATH  "Customer"
#define CUSDATA_CONFIG_PATH  "config"
#define ENV_CUSDATA  "E_CUSDATA_PATH"
#define ENV_CUSDATA_CUSTOMER_PATH  "E_CUSDATA_CUSTOMER_PATH"
#define ENV_CUSDATA_CONFIG_PATH  "E_CUSDATA_CONFIG_PATH"

#endif
#define OAD            "oad"
#endif

