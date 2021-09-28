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
#ifndef _DRV_NSK2TYPE_H_
#define _DRV_NSK2TYPE_H_

////////////////////////////////////////////////////////////////////////////////
/// @brief
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Define & data type
////////////////////////////////////////////////////////////////////////////////

#define NSK2_DBGLV_EMERG     0  //Used for emergency messages, usually those that precede a crash.
#define NSK2_DBGLV_ALERT     1  //A situation requiring immediate action.
#define NSK2_DBGLV_CRIT      2  //Critical conditions, often related to serious hardware or software failures.
#define NSK2_DBGLV_ERR       3  //Used to report error conditions; device drivers often use KERN_ERR to report hardware difficulties.
#define NSK2_DBGLV_WARNING   4  //Warnings about problematic situations that do not, in themselves, create serious problems with the system.
#define NSK2_DBGLV_NOTICE    5  //Situations that are normal, but still worthy of note. A number of security-related conditions are reported at this level.
#define NSK2_DBGLV_INFO      6  //Informational messages. Many drivers print information about the hardware they find at startup time at this level.
#define NSK2_DBGLV_DEBUG     7  //Used for debugging messages.
#define NSK2_DBGLV_ARRAY     8  //Uesd for array data.


#define EMM_DBGLV_EMERG     0  //Used for emergency messages, usually those that precede a crash.
#define EMM_DBGLV_ALERT     1  //A situation requiring immediate action.
#define EMM_DBGLV_CRIT      2  //Critical conditions, often related to serious hardware or software failures.
#define EMM_DBGLV_ERR       3  //Used to report error conditions; device drivers often use KERN_ERR to report hardware difficulties.
#define EMM_DBGLV_WARNING   4  //Warnings about problematic situations that do not, in themselves, create serious problems with the system.
#define EMM_DBGLV_NOTICE    5  //Situations that are normal, but still worthy of note. A number of security-related conditions are reported at this level.
#define EMM_DBGLV_INFO      6  //Informational messages. Many drivers print information about the hardware they find at startup time at this level.
#define EMM_DBGLV_DEBUG     7  //Used for debugging messages.
#define EMM_DBGLV_ARRAY     8  //Uesd for array data.


#endif


