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
#include <MsTypes.h>
#include <common.h>
#include <stdio.h>
#include <linux/string.h>

//#define DEBUG_TEXT

#include "read_text.h"


#define  MAX_LINE_LEN   (800 * 1024 * 1024)  // Normal Demura case : the length of line can not reach it.


typedef struct
{
    MS_U8  *fbuf;
    MS_U32 bytes_read;     // the bytes of had read
    MS_U32 init_flag;      // 1 : had inited this struct
    MS_U32 eof_flag;       // 1 : end of file
    MS_U32 file_broken;    // 1 : invalid demura file.
    MS_U32 filesize;
    MS_U32 max_size;       // The max filesize
    MS_U32 brace_depth;    // the depth of symbol "{}"
    MS_U32 bracket_depth;  // the depth of symbol "[]"
}file_info;


static file_info text_info;


void init_text_info(char *file_buf, MS_U32 max_size)
{
    memset(&text_info, 0, sizeof(text_info));
    text_info.fbuf  = (unsigned char *)file_buf;
    text_info.init_flag = 1;
    text_info.max_size  = max_size;
}


#ifdef CONFIG_DEMURA_VENDOR_CSOT

// return 0 --> Do not jump out the while loop
// return 1 --> Do jump out the while loop
static inline int handle_csot_line(line_info *linfo, unsigned char *buf, int i)
{
    char ch = buf[i];
    
    if (ch == '{')
    {
        linfo->pcont_start = buf + i + 1;
        text_info.brace_depth += 1;
    }
    else if (ch == '}')
    {
        buf[i] = '\0';
        text_info.brace_depth -= 1;
        if (text_info.brace_depth == 0)    // For text file, seem it is valid way to get the length of file.
        {
            text_info.eof_flag = 1;
            printf("This file haved reached the End_Of_File position\n");
            return 1;
        }
    }
    else if (ch == ':')    // Get key string
    {
        if (i > 0)
        {
            char temp = buf[i-1];
            if ((temp == '"') && (i >= 2))
            {
                linfo->pkey_start = linfo->pcont_start + 1;
                buf[i-1] = '\0';
            }
            else
            {
                linfo->pkey_start = linfo->pcont_start;
                buf[i] = '\0';
            }
        }
        else
        {
            printf("Warning, this ini file may be damaged !\n");
        }
    }
    
    return 0;
}

#endif


MS_U32 get_line(line_info *linfo)
{
    int i;
    unsigned char ch;
    unsigned char *buf;
    buf = text_info.fbuf + text_info.bytes_read;
    memset(linfo, 0, sizeof(line_info));
    
    DBG_TEXT_PRINT("in %s : ", __FUNCTION__);
    // check if file had been read out
    if (text_info.eof_flag == 1)
    {
        return 0;
    }
    else if (text_info.init_flag != 1)
    {
        printf("text_info had not been inited !\n");
        return 0;
    }
    
    // init line_info struct
    linfo->pcont_start = buf;
    
    for (i = 0; i < MAX_LINE_LEN; i++)
    {
        ch = buf[i];
        DBG_TEXT_PRINT(" %02x", ch);
        if ( ((MS_U32)(buf + i) - (MS_U32)text_info.fbuf) > text_info.max_size )
        {
            printf("Invaild demura table file !\n");
            text_info.file_broken = 1;
            return 0;
        }
        
        if ((ch == '\r') || (ch == '\n'))    // Time to return
        {
            if ((buf[i+1] == '\n') || (buf[i+1] == '\r'))
            {
                i = i + 1;
            }
            break;
        }
    #ifdef CONFIG_DEMURA_VENDOR_CSOT
        else if (handle_csot_line(linfo, buf, i))    // handle csot line gramma
        {
            break;
        }
    #endif
    }
    
    // Update text_info
    i = i + 1;      // remeber to add
    linfo->len = i;
    text_info.bytes_read += i;
    DBG_TEXT_PRINT("  (i = %d)\n", i);  // test
    
    return i;
}


MS_U32 split_num_string(char *pstart, char *sep, int *pbuf, int maxsplit)
{
    int i = 0;
    char *tok = NULL;
    char *ptr = pstart;

    tok = strtok (ptr, sep);
    while (tok)
    {
        pbuf[i] = atoi(tok);
        tok = strtok(NULL, sep);
        i = i + 1;
        if ((maxsplit > 0) && (maxsplit == i))
        {
            break;
        }
    }
    return i;
}


MS_U32 get_filesize(void)
{
    if (text_info.eof_flag == 1)
    {
        return text_info.filesize;
    }
    else
    {
        return 0;
    }
}


MS_U32 get_file_status(void)
{
    return text_info.file_broken;
}


