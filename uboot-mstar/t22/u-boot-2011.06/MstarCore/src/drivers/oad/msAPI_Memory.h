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

#ifndef MSAPI_MEMORY_H
#define MSAPI_MEMORY_H

#include <MsTypes.h>

/******************************************************************************/
/* Enum                                                                       */
/******************************************************************************/
// Please add your buffer ID when you need to allocate a buffer from buffer pool
/// buffer ID
typedef enum
{
    BUF_ID_ITC,                     ///< Buffer ID ITC
    BUF_ID_EVENT_INFO,              ///< Buffer ID Event Info
    BUF_ID_DESCRIPTOR,              ///< Buffer ID Descriptor
    BUF_ID_VCH,                     ///< Buffer ID VCH
    BUF_ID_PAT,                     ///< Buffer ID PAT
    BUF_ID_DTV_PG_INDEX,            ///< Buffer ID DTV PG Index
    BUF_ID_FLASH,                   ///< Buffer ID Flash
    BUF_ID_GLYPH_DISP_INFO,         ///< Buffer ID Glyph Display Info
    BUF_ID_DRAW_TEXT,               ///< Buffer ID Draw Text
    BUF_ID_SUBTITLE_SEC,            ///< Buffer ID Subtitle section buffer
    BUF_ID_MEM_REALLOC,             ///< Buffer ID Memory reallocate
    BUF_ID_AEON_MMAP,               ///< Buffer ID Aeon MMAP
    BUF_ID_FILEBROWER,              ///< Buffer ID Filebrowser
    BUF_ID_FILECNTL,                ///< Buffer ID File control
    BUF_ID_USB_DOWNLOAD,            ///< Buffer ID USB download
    BUF_ID_ZUI,                     ///< Buffer ID ZUI service
    BUF_ID_OAD_DOWNLOAD,            ///< Buffer ID OAD download
    BUF_ID_USB_HOST,                ///< Buffer ID USB Host
    BUF_ID_CC,                      ///< Buffer ID Closed-caption
    BUF_ID_ATSC,                    ///< Buffer ID ATSC
    BUF_ID_NIT_SCAN,                    ///< Buffer ID Netwok scan
    BUF_ID_NEW_MULTIPLEX,                   ///< Buffer ID new multiplex
    BUF_ID_MSB1210_DOWNLOAD,           ///< Buffer ID msb1210 download
} EN_BUFFER_ID;


/******************************************************************************/
/* Macro                                                                      */
/******************************************************************************/
#ifdef MSAPI_MEMORY_C
#define INTERFACE
#else
#define INTERFACE extern
#endif

/******************************************************************************/
// Function prototypes                                                        */
/******************************************************************************/
INTERFACE void msAPI_Memory_Init(void);
INTERFACE void *msAPI_Memory_Allocate(MS_U16 u16Number_of_bytes, EN_BUFFER_ID enBufID);
INTERFACE MS_U8 msAPI_Memory_Free(void *pFree, EN_BUFFER_ID enBufID);
INTERFACE void *msAPI_Memory_ReAllocate(void *pAlloc, MS_U16 newsize, EN_BUFFER_ID enBufID);

//INTERFACE void msAPI_Aeon_Disable(void);
//INTERFACE void msAPI_Aeon_ReInitial(U16 u16AeonBinID);

#define MSAPI_MEMORY_FREE(ptr,id)   \
{                                   \
    msAPI_Memory_Free(ptr, id);     \
    ptr = NULL;                     \
}
#undef INTERFACE

#endif

