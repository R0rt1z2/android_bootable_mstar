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

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @brief  SEM Driver Interface
///
////////////////////////////////////////////////////////////////////////////////////////////////////



#ifndef _DRVSEM_PRIV_H_
#define _DRVSEM_PRIV_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "MsTypes.h"
#include "drvSEM.h"
// driver version
#define SEM_VERSION            0x00000001

typedef enum {
    MDrv_CMD_SEM_Get_Resource,
    MDrv_CMD_SEM_Free_Resource,
    MDrv_CMD_SEM_Reset_Resource,
    MDrv_CMD_SEM_Get_ResourceID,
    MDrv_CMD_SEM_Get_Num,
    MDrv_CMD_SEM_GetLibVer,
    MDrv_CMD_SEM_Lock,
    MDrv_CMD_SEM_Unlock,
    MDrv_CMD_SEM_Delete,
} eSEMIoctlOpt;

//////////////////////////////////////
// function parameter structure //////
//////////////////////////////////////
// for cmd MDrv_SEM_Get_Resource 
typedef struct _SEM_GETRESOURCE_PARAM
{
    MS_U8 u8SemID;
    MS_U16 u16ResId;
}SEM_GETRESOURCE_PARAM, *PSEM_GETRESOURCE_PARAM;

// for cmd MDrv_SEM_Free_Resource
typedef struct _SEM_FREERESOURCE_PARAM
{
    MS_U8 u8SemID;
    MS_U16 u16ResId;
}SEM_FREERESOURCE_PARAM, *PSEM_FREERESOURCE_PARAM;

// for cmd MDrv_SEM_Reset_Resource 
typedef struct _SEM_RESETRESOURCE_PARAM
{
    MS_U8 u8SemID;
}SEM_RESETRESOURCE_PARAM, *PSEM_RESETRESOURCE_PARAM;

// for cmd MDrv_SEM_Get_ResourceID
typedef struct _SEM_GETRESOURCEID_PARAM
{
    MS_U8 u8SemID;
    MS_U16* pu16ResId;
}SEM_GETRESOURCEID_PARAM, *PSEM_GETRESOURCEID_PARAM;

// for cmd MDrv_SEM_GetLibVer
typedef struct _SEM_GETLIBVER_PARAM
{
    const MSIF_Version **ppVersion;
}SEM_GETLIBVER_PARAM, *PSEM_GETLIBVER_PARAM;

// for cmd MDrv_SEM_Lock
typedef struct _SEM_LOCK_PARAM
{
    MS_U32 u32SemID;
    MS_U32 u32WaitMs;
}SEM_LOCK_PARAM, *PSEM_LOCK_PARAM;

// for cmd MDrv_SEM_Unlock
typedef struct _SEM_UNLOCK_PARAM
{
    MS_U32 u32SemID;
}SEM_UNLOCK_PARAM, *PSEM_UNLOCK_PARAM;

// for cmd MDrv_SEM_Delete
typedef struct _SEM_DELETE_PARAM
{
    MS_U32 u32SemID;
}SEM_DELETE_PARAM, *PSEM_DELETE_PARAM;

//ioctl function pointer type
typedef MS_BOOL (*IOCTL_SEM_GETRESOURCE)(MS_U8, MS_U16);
typedef MS_BOOL (*IOCTL_SEM_FREERESOURCE)(MS_U8, MS_U16);
typedef MS_BOOL (*IOCTL_SEM_RESETRESOURCE)(MS_U8);
typedef MS_BOOL (*IOCTL_SEM_GETRESOURCEID)(MS_U8, MS_U16*);
typedef MS_U32 (*IOCTL_SEM_GETNUM)(void);
typedef MS_BOOL (*IOCTL_SEM_GETLIBVER)(const MSIF_Version **);
typedef MS_BOOL (*IOCTL_SEM_LOCK)(MS_U32, MS_U32);
typedef MS_BOOL (*IOCTL_SEM_UNLOCK)(MS_U32);
typedef MS_BOOL (*IOCTL_SEM_DELETE)(MS_U32);

// INSTANCE Private(ioctl function pointer)
typedef struct _SEM_INSTANT_PRIVATE
{
    IOCTL_SEM_GETRESOURCE       fpSEMGetResource;
    IOCTL_SEM_FREERESOURCE      fpSEMFreeResource;
    IOCTL_SEM_RESETRESOURCE     fpSEMResetResource;
    IOCTL_SEM_GETRESOURCEID     fpSEMGetResourceID;
    IOCTL_SEM_GETNUM            fpSEMGetNum;
    IOCTL_SEM_GETLIBVER         fpSEMGetLibVer;
    IOCTL_SEM_LOCK              fpSEMLock;
    IOCTL_SEM_UNLOCK            fpSEMUnlock;
    IOCTL_SEM_DELETE            fpSEMDelete;
}SEM_INSTANT_PRIVATE;

//////////////////////////////////////
// function for utopia2.0 ////////////
//////////////////////////////////////
void SEMRegisterToUtopia(void);
MS_U32 SEMOpen(void** pInstance, MS_U32 u32ModuleVersion, void* pAttribute);
MS_U32 SEMClose(void* pInstance);
MS_U32 SEMIoctl(void* pInstance, MS_U32 u32Cmd, void *pArgs);

MS_BOOL MDrv_SEM_Delete_U2K(eSemId SemId);
MS_BOOL MDrv_SEM_Unlock_U2K(eSemId SemId);
MS_BOOL MDrv_SEM_Lock_U2K(eSemId SemId, MS_U32 u32WaitMs);
MS_BOOL MDrv_SEM_Get_ResourceID_U2K(MS_U8 u8SemID, MS_U16* pu16ResId);
MS_BOOL MDrv_SEM_Reset_Resource_U2K(MS_U8 u8SemID);
MS_BOOL MDrv_SEM_Free_Resource_U2K(MS_U8 u8SemID, MS_U16 u16ResId);
MS_BOOL MDrv_SEM_Get_Resource_U2K(MS_U8 u8SemID, MS_U16 u16ResId);


// Resource_Private
typedef struct _SEM_RESOURCE_PRIVATE
{
    MS_U32 Dummy;
}SEM_RESOURCE_PRIVATE;

#ifdef __cplusplus
}
#endif

#endif // _DRVSEM_PRIV_H_
