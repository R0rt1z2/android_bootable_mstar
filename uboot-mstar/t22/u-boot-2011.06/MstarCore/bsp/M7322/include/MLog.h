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
#ifndef __MLOG_H__
#define __MLOG_H__

#ifdef __cplusplus
extern "C" {
#endif
typedef enum
{
    EN_LOG_PRIORITY_UNKNOWN = 0,
    EN_LOG_PRIORITY_DEFAULT,    /* only for SetMinPriority() */
    EN_LOG_PRIORITY_VERBOSE,
    EN_LOG_PRIORITY_INFO,
    EN_LOG_PRIORITY_WARN,
    EN_LOG_PRIORITY_DEBUG,
    EN_LOG_PRIORITY_ERROR,
    EN_LOG_PRIORITY_FATAL,
    EN_LOG_PRIORITY_SILENT,
} EN_LOG_PRIORITY;

void filter_log(EN_LOG_PRIORITY pri,const char* TAG,const char* format, ...);

#define MLOGV(tag,fmt,...)      filter_log(EN_LOG_PRIORITY_VERBOSE, tag, fmt, ##__VA_ARGS__)
#define MLOGI(tag,fmt,...)      filter_log(EN_LOG_PRIORITY_INFO, tag, fmt, ##__VA_ARGS__)
#define MLOGW(tag,fmt,...)      filter_log(EN_LOG_PRIORITY_WARN, tag, fmt, ##__VA_ARGS__)
#define MLOGD(tag,fmt,...)      filter_log(EN_LOG_PRIORITY_DEBUG, tag, fmt, ##__VA_ARGS__)
#define MLOGE(tag,fmt,...)      filter_log(EN_LOG_PRIORITY_ERROR, tag, fmt, ##__VA_ARGS__)
#define MLOGF(tag,fmt,...)      filter_log(EN_LOG_PRIORITY_FATAL, tag, fmt, ##__VA_ARGS__)
#define TARGET_USES_LOGD        1    // Support Client–Server model
//-------------------------------------------------------------------------------------------------
/// show TAG
/// @param  NONE
/// @return  NONE
//-------------------------------------------------------------------------------------------------
void mlog_showTags(void);
//-------------------------------------------------------------------------------------------------
/// add TAG
/// @param  TAG
/// @return  NONE
//-------------------------------------------------------------------------------------------------
void mlog_addTAG(char* TAG);
//-------------------------------------------------------------------------------------------------
/// remove TAG
/// @param  TAG
/// @return  NONE
//-------------------------------------------------------------------------------------------------
void mlog_removeTAG(char* TAG);
//-------------------------------------------------------------------------------------------------
/// get Priority
/// @param  NONE
/// @return  EN_LOG_PRIORITY
//-------------------------------------------------------------------------------------------------
EN_LOG_PRIORITY mlog_getPriority(void);
//-------------------------------------------------------------------------------------------------
/// set Priority
/// @param  pri                             \b set up priority
/// @return  NONE
//-------------------------------------------------------------------------------------------------
void mlog_setPriority(EN_LOG_PRIORITY pri);


#ifdef __cplusplus
}
#endif

#endif
