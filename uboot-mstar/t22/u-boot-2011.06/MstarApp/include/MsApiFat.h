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

#ifndef __CMD_MS_API_FAT__
#define __CMD_MS_API_FAT__

#define ENV_PARTNO                  "partno"
#define MAX_FAT_PARTNO      10 // support 10 partitions
#define CMD_BUF_SIZE           20

#define MAX_FILE_NAME_LENGTH 256


typedef struct{
    char name[MAX_FILE_NAME_LENGTH];
    unsigned int idx;
}ST_FILE_INFO;


int do_fat_fswrite (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_fat_fspartload (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_fat_fsfilesize (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_fat_fileList_test(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_dump_fileList(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);

//-------------------------------------------------------------------------------------------------    
/// Creat a file list by the input path. (Not include folder)
/// @param  path                           \b IN: path
/// @param  cur                             \b IN/OUT: return the first file information
/// @return  int                              \b OUT: 0: successful. -1: fail
//-------------------------------------------------------------------------------------------------
int createfileList(char *path,ST_FILE_INFO *cur);

//-------------------------------------------------------------------------------------------------    
/// Return the next file information by the current file information
/// @param  cur                             \b IN/OUT: input the current file information/return the next file information
/// @return  int                              \b OUT: 0: successful. -1: fail
//-------------------------------------------------------------------------------------------------
int getNextFile(ST_FILE_INFO *cur);

//-------------------------------------------------------------------------------------------------    
/// Return the prev file information by the current file information
/// @param  cur                             \b IN/OUT: input the current file information/return the prev file information
/// @return  int                              \b OUT: 0: successful. -1: fail
//-------------------------------------------------------------------------------------------------
int getPrevFile(ST_FILE_INFO *cur);

//-------------------------------------------------------------------------------------------------    
/// Return the the first file information in file list
/// @param  cur                             \b IN/OUT: return the first file information
/// @return  int                              \b OUT: 0: successful. -1: fail
//-------------------------------------------------------------------------------------------------
int getFirstFile(ST_FILE_INFO *cur);

//-------------------------------------------------------------------------------------------------    
/// Return the the last file information in file list
/// @param  cur                             \b IN/OUT: return the last file information
/// @return  int                              \b OUT: 0: successful. -1: fail
//-------------------------------------------------------------------------------------------------
int getLastFile(ST_FILE_INFO *cur);

//-------------------------------------------------------------------------------------------------    
/// Delete the internal file list
/// @return  int                              \b OUT: 0: successful. -1: fail
//-------------------------------------------------------------------------------------------------
int DeleteFileList(void);



#endif
