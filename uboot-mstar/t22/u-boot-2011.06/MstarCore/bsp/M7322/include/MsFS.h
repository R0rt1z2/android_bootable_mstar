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

/// @brief  FileSystem wrapper
/// @note   Most of the APIs follows POSIX Specification including: \n
///                  - Files and Directories [POSIX Section 5] \n
///                  - Input and Output [POSIX Section 6]
/// @note   With some limitations: \n
///                  - Doesn't support symolic link, user/group/other ID, ...
/// @note   Plz reference Open Group's Single Unix Specification V3 for detailed POSIX API descriptions: \n
///                  - http://www.opengroup.org/onlinepubs/009695399/
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __MSFS_H__
#define __MSFS_H__

//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#ifdef __cplusplus
extern "C"
{
#endif

#include "MsTypes.h"

#if defined(MSOS_TYPE_LINUX_KERNEL)

#define FILE struct file

FILE * MsFS_Fopen( const char *filename, const char *mode );
int MsFS_Fclose( FILE *file );
size_t MsFS_Fread( void *data, size_t itemsize, size_t nitems, FILE *file );
size_t MsFS_Fwrite( const void *data, size_t itemsize, size_t nitems, FILE *file );
int MsFS_Fseek( FILE *file, MS_U64 pos, int whence );

#else

#include "stdlib.h"
#include "stdio.h"
#include "dirent.h"

//-------------------------------------------------------------------------------------------------
// Defines
//------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Type and Structure Declaration
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Extern Functions
//-------------------------------------------------------------------------------------------------
// int MsFS_Init( int bUnicode);
int MsFS_Init(int bUnitcode);

//
// Mount/Umount operations
//
int MsFS_Mount( const char *devname, const char *dir, const char *fsname, unsigned long mountflags, const void* data );
int MsFS_Umount( const char *dir);

//
// File/Directory operations
//
int MsFS_Open( const char *path, int oflag, ... );
int MsFS_Unlink( const char *path );
int MsFS_MkDir( const char *path, mode_t mode );
int MsFS_RmDir( const char *path );
int MsFS_ChDir(const char *path);
int MsFS_Rename( const char *path1, const char *path2 );
int MsFS_Link( const char *path1, const char *path2 );
int MsFS_Stat( const char *path, struct stat *buf );
int MsFS_FStat( int fd, struct stat *buf );
long MsFS_PathConf( const char *path, int name );
long MsFS_FPathConf( int fd, int name );
int MsFS_Access( const char *path, int amode );
char* MsFS_GetCwd( char *buf, size_t size );
DIR* MsFS_OpenDir( const char *dirname );
struct dirent* MsFS_ReadDir( DIR *dirp );
void MsFS_RewindDir( DIR *dirp );
void MsFS_CloseDir( DIR *dirp );


//
// File IO operations
//
ssize_t MsFS_Read( int fd, void *buf, size_t len );
ssize_t MsFS_Write( int fd, const void *buf, size_t len );
int MsFS_Close( int fd );
MS_U64 MsFS_Lseek( int fd, MS_U64 pos, int whence );
int MsFS_FCntl( int fd, int cmd, ... );
int MsFS_FSync( int fd );
void MsFS_Sync(void);


//
// Standard C Library's counterparts
//
int MsFS_Setvbuf(FILE *stream, char *buffer, int mode, size_t size);
FILE * MsFS_Fopen( const char *filename, const char *mode );
int MsFS_Fclose( FILE *stream );
size_t MsFS_Fread( void *data, size_t itemsize, size_t nitems, FILE *stream );
size_t MsFS_Fwrite( const void *data, size_t itemsize, size_t nitems, FILE *stream );
int MsFS_Fftruncate( FILE *stream, MS_U64 length);
int MsFS_Fseek( FILE *stream, MS_U64 pos, int whence );
void MsFS_Rewind( FILE *stream );
MS_U64 MsFS_Ftell( FILE *stream );
int MsFS_Fflush(FILE *stream);

//
// File system information query
//
typedef struct
{
    MS_U32      u32ClusTotal;
    MS_U32      u32ClusFree;
    MS_U32      u32ClusSize;
} MApi_FsInfo;

MS_BOOL MApi_FS_Info(const char *path, MS_BOOL bUnicode, MApi_FsInfo* pFsInfo);

#endif //#if defined(MSOS_TYPE_LINUX_KERNEL)

#ifdef __cplusplus
}
#endif


#endif // #ifndef __MSFS_H__
