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

#ifndef __ULOG_H__
#define __ULOG_H__

#ifdef __cplusplus
extern "C" {
#endif

#if (defined CONFIG_UTOPIA_TEE || defined MBOOT || defined MSOS_TYPE_NOS || defined MSOS_TYPE_OPTEE )
#define ULOGI(tag,fmt,...)      printf("[Utopia][" tag "]: " fmt, ##__VA_ARGS__)
#define ULOGW(tag,fmt,...)      printf("[Utopia][" tag "]: " fmt, ##__VA_ARGS__)
#define ULOGD(tag,fmt,...)      printf("[Utopia][" tag "]: " fmt, ##__VA_ARGS__)
#define ULOGE(tag,fmt,...)      printf("[Utopia][" tag "]: " fmt, ##__VA_ARGS__)
#define ULOGF(tag,fmt,...)      printf("[Utopia][" tag "]: " fmt, ##__VA_ARGS__)

#elif (defined MSOS_TYPE_ECOS)

#ifdef MS_DEBUG
#define ULOGI(tag,fmt,...)      printf("[Utopia][" tag "]: " fmt, ##__VA_ARGS__)
#define ULOGW(tag,fmt,...)      printf("[Utopia][" tag "]: " fmt, ##__VA_ARGS__)
#define ULOGD(tag,fmt,...)      printf("[Utopia][" tag "]: " fmt, ##__VA_ARGS__)
#define ULOGE(tag,fmt,...)      printf("[Utopia][" tag "]: " fmt, ##__VA_ARGS__)
#define ULOGF(tag,fmt,...)      printf("[Utopia][" tag "]: " fmt, ##__VA_ARGS__)
#else
#define ULOGI(tag,fmt,...)      {}
#define ULOGW(tag,fmt,...)      {}
#define ULOGD(tag,fmt,...)      {}
#define ULOGE(tag,fmt,...)      {}
#define ULOGF(tag,fmt,...)      {}
#endif

#elif (defined MSOS_TYPE_LINUX_KERNEL)

#define ULOGI(tag,fmt,...)      printk("[Utopia][" tag "]: " fmt, ##__VA_ARGS__)
#define ULOGW(tag,fmt,...)      printk("[Utopia][" tag "]: " fmt, ##__VA_ARGS__)
#define ULOGD(tag,fmt,...)      printk("[Utopia][" tag "]: " fmt, ##__VA_ARGS__)
#define ULOGE(tag,fmt,...)      printk("[Utopia][" tag "]: " fmt, ##__VA_ARGS__)
#define ULOGF(tag,fmt,...)      printk("[Utopia][" tag "]: " fmt, ##__VA_ARGS__)

#elif (defined CONFIG_MLOG)
#include "MLog.h"

#define ULOGI(tag,fmt,...)      MLOGI("Utopia", "[" tag "]" fmt, ##__VA_ARGS__)
#define ULOGW(tag,fmt,...)      MLOGW("Utopia", "[" tag "]" fmt, ##__VA_ARGS__)
#define ULOGD(tag,fmt,...)      MLOGD("Utopia", "[" tag "]" fmt, ##__VA_ARGS__)
#define ULOGE(tag,fmt,...)      MLOGE("Utopia", "[" tag "]" fmt, ##__VA_ARGS__)
#define ULOGF(tag,fmt,...)      MLOGF("Utopia", "[" tag "]" fmt, ##__VA_ARGS__)

#elif (defined ANDROID)
#include <cutils/log.h>

#ifdef ALOG
#define ULOGI(tag,fmt,...)      ALOG(LOG_INFO, "Utopia", "[" tag "]" fmt, ##__VA_ARGS__)
#define ULOGW(tag,fmt,...)      ALOG(LOG_WARN, "Utopia", "[" tag "]" fmt, ##__VA_ARGS__)
#define ULOGD(tag,fmt,...)      ALOG(LOG_DEBUG, "Utopia", "[" tag "]" fmt, ##__VA_ARGS__)
#define ULOGE(tag,fmt,...)      ALOG(LOG_ERROR, "Utopia", "[" tag "]" fmt, ##__VA_ARGS__)
#define ULOGF(tag,fmt,...)      ALOG(LOG_FATAL, "Utopia", "[" tag "]" fmt, ##__VA_ARGS__)
#else
#define ULOGI(tag,fmt,...)      LOG(LOG_INFO, "Utopia", "[" tag "]" fmt, ##__VA_ARGS__)
#define ULOGW(tag,fmt,...)      LOG(LOG_WARN, "Utopia", "[" tag "]" fmt, ##__VA_ARGS__)
#define ULOGD(tag,fmt,...)      LOG(LOG_DEBUG, "Utopia", "[" tag "]" fmt, ##__VA_ARGS__)
#define ULOGE(tag,fmt,...)      LOG(LOG_ERROR, "Utopia", "[" tag "]" fmt, ##__VA_ARGS__)
#define ULOGF(tag,fmt,...)      LOG(LOG_FATAL, "Utopia", "[" tag "]" fmt, ##__VA_ARGS__)
#endif

#else

#define ULOGI(tag,fmt,...)      printf("[Utopia][" tag "]: " fmt, ##__VA_ARGS__)
#define ULOGW(tag,fmt,...)      printf("[Utopia][" tag "]: " fmt, ##__VA_ARGS__)
#define ULOGD(tag,fmt,...)      printf("[Utopia][" tag "]: " fmt, ##__VA_ARGS__)
#define ULOGE(tag,fmt,...)      printf("[Utopia][" tag "]: " fmt, ##__VA_ARGS__)
#define ULOGF(tag,fmt,...)      printf("[Utopia][" tag "]: " fmt, ##__VA_ARGS__)

#endif

#ifdef __cplusplus
}
#endif

#endif
