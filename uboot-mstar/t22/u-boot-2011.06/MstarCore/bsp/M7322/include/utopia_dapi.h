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
#ifndef _UTOPIA_DAPI_H_
#define _UTOPIA_DAPI_H_

#include "MsTypes.h"
/* driver owners need not include this explicitly */
#include "utopia_driver_id.h"
#include "MsCommon.h"
#if defined(MSOS_TYPE_LINUX_KERNEL)
#include <linux/namei.h>
#endif

/*
 * for develop: block on error
 * for ut: don't block on error
 */
#define BLOCK_ON_ERROR 1
#if BLOCK_ON_ERROR
#define RET_OR_BLOCK(ret) MS_PANIC(0); return ret; /* fix compiling warning */
#else
#define RET_OR_BLOCK(ret) return ret
#endif

/* print method */
#define PRINTU_INFO   0
#if defined(ANDROID)
#include <cutils/log.h>
#define printu ALOGI
#define flagPrintu(FLAG)    if(FLAG)printu
#elif defined(MSOS_TYPE_LINUX_KERNEL)
#define printu printk
#define flagPrintu(FLAG)    if(FLAG)printu
#else
#define printu printf
#define flagPrintu(FLAG)    if(FLAG)printu
#endif

/*
 * type converter
 */
#define TO_MODULE_PTR(pTmp) ((UTOPIA_MODULE*)pTmp)
#define TO_MODULE_SHM_PTR(pTmp) ((UTOPIA_MODULE_SHM*)pTmp)
// FIXME: this macro doesn't consider synchronization problem, just for tmp use
#define MODULE_PTR_TO_RPOOL_PTR(pModule) \
	((UTOPIA_RESOURCE_POOL*)shmid2va(pModule->psModuleShm->shmid_rpool_head.ID))

#define TO_RESOURCE_PTR(pTmp) ((UTOPIA_RESOURCE*)pTmp)
// FIXME: this macro doesn't consider synchronization problem, just for tmp use
#define RESOURCE_PTR_TO_RPOOL_PTR(pResource) \
	((UTOPIA_RESOURCE_POOL*)shmid2va(pResource->shmid_rpool.ID))

#define TO_INSTANCE_PTR(pTmp) ((UTOPIA_INSTANCE*)pTmp)
#define TO_RPOOL_PTR(pTmp) ((UTOPIA_RESOURCE_POOL*)pTmp)


typedef enum{
    CMD_USIGNAL_PROCESS_KILL =(int) 0xFFFF0001,
}UTOPIA_SYSYEM_IOCTL;

typedef enum{
    MDBCMD_CMDLINE = 0x0,
    MDBCMD_GETINFO
}UTOPIA_MDBCMD;

typedef enum{
    MDBRESULT_SUCCESS_FIN = 0x0,
    MDBRESULT_SUCCESS_NOT_FIN,
    MDBRESULT_NOT_AVALIBALE
}UTOPIA_MDBRESULT;

typedef enum{
    MDBFP_NOT_USE = 0x0,
    MDBFP_IS_KERENL,
    MDBFP_IS_USER
}UTOPIA_MDBFP_TYPE;

#define MDB_NODE_NAME_MAX 40
#define MDB_CMDLINE_MAX 255
#define MDB_FPIOCTL_LIST_MAX 128
#define MDB_POLLING_TH_MAX 32
#define MDB_OUTPUT_LENGTH_MAX 4
#define MDB_OUTPUT_BUF_MAX 256
typedef struct
{
    MS_U64* u64ReqHdl;
    MS_U32 u32CmdSize;
    char pcCmdLine[MDB_CMDLINE_MAX];
    UTOPIA_MDBRESULT result;
} MDBCMD_CMDLINE_PARAMETER;

typedef struct
{
    MS_U64* u64ReqHdl;
    UTOPIA_MDBRESULT result;
} MDBCMD_GETINFO_PARAMETER;

typedef struct
{
    MS_BOOL bOutputToDram;
    void* pBufAddr;
    MS_U32 u32Pos;
    MS_U32 u32Length;
} MDBCMD_OUTPUT_PARAMETER;

/*
 * instance functions
 */
MS_U32 UtopiaInstanceCreate(MS_U32 u32PrivateSize, void** ppInstance);
MS_U32 UtopiaInstanceDelete(void* pInstant);
MS_U32 UtopiaInstanceGetPrivate(void* pInstance, void** ppPrivate);
MS_U32 UtopiaInstanceGetModule(void* pInstance, void** ppModule);
/* We hope, we can support poly, ex: JPD and JPD3D as different Module */
MS_U32 UtopiaInstanceGetModuleID(void* pInstance, MS_U32* pu32ModuleID);
/* We hope, we can support poly, ex: JPD and JPD3D as different Module */
MS_U32 UtopiaInstanceGetModuleVersion(void* pInstant, MS_U32* pu32Version);
/* We hope we can support interface version mantain */
MS_U32 UtopiaInstanceGetAppRequiredModuleVersion(void* pInstance,
		MS_U32* pu32ModuleVersion);
MS_U32 UtopiaInstanceGetPid(void* pInstance);

/*
 * module functions
 */
MS_U32 UtopiaModuleCreate(MS_U32 u32ModuleID,
		MS_U32 u32PrivateSize, void** ppModule);
MS_U32 UtopiaModuleGetPrivate(void* pModule, void** ppPrivate);
MS_U32 UtopiaModuleSetSTRPrivate(void* pModule, MS_U32 u32STRPrivateSize);
MS_U32 UtopiaModuleGetSTRPrivate(void* pModule, void** ppPrivate);
MS_U32 UtopiaModuleRegister(void* pModule);
#ifdef CONFIG_UTOPIA_PROC_DBG_SUPPORT
MS_U32 UtopiaModuleRegisterMdbNode(char sMdbNodeName[MDB_NODE_NAME_MAX], FUtopiaMdbIoctl fpMdbIoctl);
MS_U32 UtopiaCreateMdbNode(void);
void* UtopiaModuleGetLocalInstantList(MS_U32 u32ModuleID, void* pLastInstance);
#endif
MS_U32 UtopiaModuleSetupFunctionPtr(void* pModule, FUtopiaOpen fpOpen,
		FUtopiaClose fpClose, FUtopiaIOctl fpIoctl);
MS_U32 UtopiaModuleSetupSTRFunctionPtr(void* pModuleTmp, FUtopiaSTR fpSTR);
MS_U32 UtopiaModuleSuspend(void);
MS_U32 UtopiaModuleResume(void);
MS_U32 UtopiaModuleSetVersion(void* pModule, MS_U32 u32Version);
MS_U32 UtopiaModuleGetDebugLevel(void* pInstance, MS_U32* pu32DebugLevel);
MS_U32 UtopiaModuleGetPtr(MS_U32 u32ModuleID, void** ppModule);

MS_U32 UtopiaModuleQueryMode(MS_U32 u32ModuleID);


/*
 * resource functions
 */
MS_U32 UtopiaResourceCreate(char* u8ResourceName,
		MS_U32 u32PrivateSize, void** ppResource);
MS_U32 UtopiaResourceGetPrivate(void* pResource, void** ppPrivate);
MS_U32 UtopiaResourceRegister(void* pModule, void* pResouce, MS_U32 u32PoolID);
MS_U32 UtopiaResourceObtain(void* pInstant,
		MS_U32 u32PoolID, void** ppResource);
MS_U32 UtopiaResourceTryObtain(void* pInstant,
		MS_U32 u32PoolID, void** ppResource);
MS_U32 UtopiaResourceRelease(void* pResource);
MS_U32 UtopiaResourceGetPid(void* pResource);
MS_U32 UtopiaResourceGetNext(void* pModTmp, void** ppResource);

MS_U32 UtopiaModuleAddResourceStart(void* psModule, MS_U32 u32PoolID);
MS_U32 UtopiaModuleAddResourceEnd(void* psModule, MS_U32 u32PoolID);
MS_U32 UtopiaModuleResetPool(void* pModuleTmp, MS_U32 u32RPoolID);

MS_U32 UtopiaStrWaitCondition(const char* u8ModuleName, MS_U32 u32Mode, MS_U32 u32Stage);
MS_U32 UtopiaStrSendCondition(const char* u8ModuleName, MS_U32 u32Mode, MS_U32 u32Stage);

MS_U32 UtopiaStrSetData(char *key, char *value);
MS_U32 UtopiaStrGetData(char *key, char *value);

#endif /* _UTOPIA_DAPI_H_ */
