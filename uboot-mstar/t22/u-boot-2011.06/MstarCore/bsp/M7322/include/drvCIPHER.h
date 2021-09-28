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
/// @brief  CIPHER Driver Interface
///
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __DRVCIPHER_H__
#define __DRVCIPHER_H__

#include "MsTypes.h"
#include "MsDevice.h"
#include "MsCommon.h"

#ifdef __cplusplus
extern "C"
{
#endif


//--------------------------------------------------------------------------------------------------
//  Define
//--------------------------------------------------------------------------------------------------
#define HASH_TIMEOUT_VALUE      (5000UL)

typedef enum
{
    CIPHER_DBG_NONE,
    CIPHER_DBG_ERR,
    CIPHER_DBG_WARN,
    CIPHER_DBG_INFO,
}CIPHER_DBGMSG_LEVEL;

#define MSIF_CIPHER_LIB_CODE                     {'D','M','A','_'}    //Lib code
#define MSIF_CIPHER_LIBVER                       {'0','1'}            //LIB version
#define MSIF_CIPHER_BUILDNUM                     {'0','1'}            //Build Number
#define MSIF_CIPHER_CHANGELIST                   {'0','0','8','2','9','9','2','7'} //P4 ChangeList Number

#define CIPHER_DRV_VERSION                 /* Character String for DRV/API version             */  \
    MSIF_TAG,                           /* 'MSIF'                                           */  \
    MSIF_CLASS,                         /* '00'                                             */  \
    MSIF_CUS,                           /* 0x0000                                           */  \
    MSIF_MOD,                           /* 0x0000                                           */  \
    MSIF_CHIP,                                                                                  \
    MSIF_CPU,                                                                                   \
    MSIF_CIPHER_LIB_CODE,                  /* IP__                                             */  \
    MSIF_CIPHER_LIBVER,                          /* 0.0 ~ Z.Z                                        */  \
    MSIF_CIPHER_BUILDNUM,                          /* 00 ~ 99                                          */  \
    MSIF_CIPHER_CHANGELIST,  /* CL#                                              */  \
    MSIF_OS


//--------------------------------------------------------------------------------------------------
//  Driver Capability
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//  Local variable
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//  Type and Structure
//--------------------------------------------------------------------------------------------------
/// AESDMA DDI return value
typedef MS_U32                         DRV_CIPHER_RET;

typedef void (*P_DrvCIPHER_EvtCallback)(MS_U32 u32CmdId, MS_BOOL bSuccess, MS_U32 u32Exception);

typedef enum
{
    E_CIPHER_MAIN_NONE,
    E_CIPHER_MAIN_AES,
    E_CIPHER_MAIN_DES,
    E_CIPHER_MAIN_TDES,
    E_CIPHER_MAIN_M6_S56_CCBC,
    E_CIPHER_MAIN_M6_S56,
    E_CIPHER_MAIN_M6_KE56,
    E_CIPHER_MAIN_RC4,
    E_CIPHER_MAIN_RC4_128,
    E_CIPHER_MAIN_SM4,
}CIPHER_MAIN_ALGO;

typedef enum
{
    E_CIPHER_SUB_NONE,
    E_CIPHER_SUB_ECB,
    E_CIPHER_SUB_CBC,
    E_CIPHER_SUB_CTR,
    E_CIPHER_SUB_CBC_MAC,
    E_CIPHER_SUB_CTR_64,
    E_CIPHER_SUB_CMAC_Key,
    E_CIPHER_SUB_CMAC_Algo,
    E_CIPHER_SUB_PCBC_ADD,
    E_CIPHER_SUB_PCBC_XOR,
    E_CIPHER_SUB_OTPHASH,
    E_CIPHER_SUB_GCM,
}CIPHER_SUB_ALGO;

typedef enum
{
    E_CIPHER_RES_NONE,
    E_CIPHER_RES_CLR,
    E_CIPHER_RES_CTS,
    E_CIPHER_RES_SCTE52,
}CIPHER_RES_ALGO;

typedef enum
{
    E_CIPHER_SB_NONE,
    E_CIPHER_SB_CLR,
    E_CIPHER_SB_IV1,
    E_CIPHER_SB_IV2,
}CIPHER_SB_ALGO;

typedef enum
{
    E_CIPHER_KSRC_CPU,
    E_CIPHER_KSRC_KL,
    E_CIPHER_KSRC_OTP,
    E_CIPHER_KSRC_STR,
    E_CIPHER_KSRC_KEYSLOT,
    E_CIPHER_KSRC_SECUREKEY, //for TV
    E_CIPHER_KSRC_CAIP = 9,
}CIPHER_KEY_SRC;


typedef struct
{
    CIPHER_MAIN_ALGO    eMainAlgo;
    CIPHER_SUB_ALGO     eSubAlgo;
    CIPHER_RES_ALGO     eResAlgo;
    CIPHER_SB_ALGO      eSBAlgo;
}DRV_CIPHER_ALGO;


typedef enum
{
	E_CIPHER_DRAM = 1,
	E_CIPHER_DQMEM,
	E_CIPHER_IQMEM,
	E_CIPHER_DIRECT,
	E_CIPHER_OTP,
	E_CIPHER_KL_LUT,
	E_CIPHER_KL_M,
	E_CIPHER_KL_BC,
}CIPHER_MEM_TYPE;

typedef struct
{
    MS_PHY u32Addr;
    MS_U32 u32Size;
	CIPHER_MEM_TYPE    eMemoryType;
}DRV_CIPHER_DATA;

typedef struct
{
    CIPHER_KEY_SRC  eKeySrc;
    MS_U8           u8KeyIdx;
    MS_U8           u8KeyLen;
    MS_U8           *pu8KeyData;
    MS_U8           u8IVLen;
    MS_U8           *pu8IVData;
    DRV_CIPHER_DATA stGCMIV;
}DRV_CIPHER_KEY;

typedef struct
{
    DRV_CIPHER_ALGO stAlgo;
    DRV_CIPHER_KEY  stKey;
    DRV_CIPHER_DATA stInput;
    DRV_CIPHER_DATA stOutput;
    MS_BOOL         bDecrypt;
	MS_U32          u32CAVid;
    P_DrvCIPHER_EvtCallback pfCallback;
    DRV_CIPHER_DATA  stAAD;
}DRV_CIPHER_DMACFG;

typedef enum
{
    E_CIPHER_HASH_ALGO_SHA1,
    E_CIPHER_HASH_ALGO_SHA256 ,
    E_CIPHER_HASH_ALGO_MD5 ,
    E_CIPHER_HASH_ALGO_SM3 ,
}CIPHER_HASH_ALGO;

typedef enum
{
    E_CIPHER_HASH_IV_FIPS,
    E_CIPHER_HASH_IV_CMD,
    E_CIPHER_HASH_IV_PRV,
}CIPHER_HASH_IV;

typedef enum
{
    E_CIPHER_HASH_STAGE_FIRST ,
    E_CIPHER_HASH_STAGE_UPDATE ,
    E_CIPHER_HASH_STAGE_LAST ,
}CIPHER_HASH_STAGE;

typedef enum
{
    E_CIPHER_HMAC_IKPAD,
    E_CIPHER_HMAC_OKPAD,
}CIPHER_HMAC_KPAD;

typedef struct
{
    CIPHER_KEY_SRC  eKeySrc;
    MS_U8           u8KeyIdx;
    MS_U8           u8KeyLen;
    MS_U8           *pu8KeyData;
}DRV_CIPHER_HMAC_KEY;

typedef struct
{
    CIPHER_HASH_ALGO        eAlgo;
    DRV_CIPHER_DATA         stInput;
    MS_U8                   *pu8Digest_Buf;
    MS_U32                  u32Digest_Buf_Size;
    MS_U32                  u32CAVid;
    P_DrvCIPHER_EvtCallback pfCallback;
}DRV_CIPHER_HASHCFG;

typedef struct
{
    CIPHER_HASH_ALGO      eAlgo;
    DRV_CIPHER_DATA       stInput;
    MS_U8                 *pu8Digest_Buf;
    MS_U32                u32Digest_Buf_Size;
    MS_U32                u32CAVid;
    MS_U32                u32StartByte;
    MS_U8                 *pu8SetIV;
    CIPHER_HASH_IV        eIVMode;
    CIPHER_HASH_STAGE     eStage;
    CIPHER_HMAC_KPAD      eHMACKPad;
    DRV_CIPHER_HMAC_KEY   stHMACKey;
    MS_U8                 *pu8Handle;
}DRV_CIPHER_HASHCFG_MANUAL;

typedef struct
{
    CIPHER_HASH_ALGO    eAlgo;
    DRV_CIPHER_DATA     stInput;
    MS_U8               *pu8Digest_Buf;
    MS_U32              u32Digest_Buf_Size;
    MS_U32              u32CAVid;
    DRV_CIPHER_HMAC_KEY stHMACKey;
    P_DrvCIPHER_EvtCallback pfCallback;
}DRV_CIPHER_HMACCFG;

typedef enum
{
    E_CIPHER_KS_KEY_TYPE_0 = 0, // Even Key
    E_CIPHER_KS_KEY_TYPE_1 = 1, // Odd Key
}CIPHER_KS_KEY_TYPE;

typedef enum
{
    E_CIPHER_KS_ALLOW_NONSECURE_HOST, // The KeySlot key is allowed to be used by all hosts
    E_CIPHER_KS_ALLOW_SECURE_HOST,    // The KeySlot key is only allowed to be used by secure hosts
}CIPHER_KS_ALLOW_HOST;

typedef struct
{
    MS_U32               u32CAVid;
    MS_U8               *pu8Key;
    MS_U32               u32KeyLen;
    MS_U32               u32KeyIdx;
    CIPHER_KS_KEY_TYPE   eKeyType;
    CIPHER_KS_ALLOW_HOST eKeyAllowHost;
}DRV_CIPHER_KS_CFG; // Configuration for KeySlot setting


#define DRV_CIPHER_OK                   (0x00000000UL)
#define DRV_CIPHER_FAIL                 (0x00000001UL)
#define DRV_CIPHER_BAD_PARAM            (0x00000002UL)

//Exception Message
#define DRV_CIPHER_EXCEPT_CAVID         (0x00000001UL)
#define DRV_CIPHER_EXCEPT_DATA          (0x00000002UL)
#define DRV_CIPHER_EXCEPT_ALGO          (0x00000004UL)
#define DRV_CIPHER_EXCEPT_DMA_KEY       (0x00000008UL)
#define DRV_CIPHER_EXCEPT_HMAC_KEY      (0x00000010UL)


//--------------------------------------------------------------------------------------------------
//  Function Prototype
//--------------------------------------------------------------------------------------------------
DLL_PUBLIC DRV_CIPHER_RET MDrv_CIPHER_Init(void);
DLL_PUBLIC DRV_CIPHER_RET MDrv_CIPHER_Reset(void);
DLL_PUBLIC DRV_CIPHER_RET MDrv_CIPHER_ResetKey(MS_U32 u32CAVid, MS_U32 u32KeyIdx);

DLL_PUBLIC DRV_CIPHER_RET MDrv_CIPHER_DMACalc(DRV_CIPHER_DMACFG stCfg, MS_U32 *pu32CmdId);
DLL_PUBLIC DRV_CIPHER_RET MDrv_CIPHER_OTPHASHCalc(DRV_CIPHER_DMACFG stCfg, MS_U32 *pu32CmdId);
DLL_PUBLIC DRV_CIPHER_RET MDrv_CIPHER_HASH(DRV_CIPHER_HASHCFG stCfg, MS_U32 *pu32CmdId);
DLL_PUBLIC DRV_CIPHER_RET MDrv_CIPHER_HASHManual(DRV_CIPHER_HASHCFG_MANUAL stCfg, MS_U32 *pu32CmdId);
DLL_PUBLIC DRV_CIPHER_RET MDrv_CIPHER_HMAC(DRV_CIPHER_HMACCFG stCfg, MS_U32 *pu32CmdId);

DLL_PUBLIC MS_BOOL MDrv_CIPHER_IsDMADone(MS_U32 u32CmdId, MS_U32 *pu32Exception);
DLL_PUBLIC MS_BOOL MDrv_CIPHER_IsHASHDone(MS_U32 u32CmdId, MS_U32 *pu32Exception);

DLL_PUBLIC DRV_CIPHER_RET MDrv_CIPHER_SetDbgLevel(CIPHER_DBGMSG_LEVEL eDBGMsgLevel);
DLL_PUBLIC DRV_CIPHER_RET MDrv_CIPHER_GetLibVer(const MSIF_Version **ppVersion);

//Key Control API
DLL_PUBLIC DRV_CIPHER_RET MDrv_CIPHER_KeyCtrl(MS_BOOL bEnable);
DLL_PUBLIC DRV_CIPHER_RET MDrv_CIPHER_Alloc(MS_U32 *pu32CipherId);
DLL_PUBLIC DRV_CIPHER_RET MDrv_CIPHER_Free(MS_U32 u32CipherId);
DLL_PUBLIC DRV_CIPHER_RET MDrv_CIPHER_DMAConfigure(MS_U32 u32CipherId, DRV_CIPHER_DMACFG stCfg);
DLL_PUBLIC DRV_CIPHER_RET MDrv_CIPHER_DMAStart(MS_U32 u32CipherId, MS_U32 *pu32CmdId);

DLL_PUBLIC DRV_CIPHER_RET MDrv_CIPHER_KeySlot_Alloc(MS_U32 *pu32KeySlotIdx, CIPHER_KS_ALLOW_HOST stKeyAllowHost);
DLL_PUBLIC DRV_CIPHER_RET MDrv_CIPHER_KeySlot_Free(MS_U32 u32KeySlotIdx);
DLL_PUBLIC DRV_CIPHER_RET MDrv_CIPHER_KeySlot_SetKey(DRV_CIPHER_KS_CFG stSKCfg);

DLL_PUBLIC DRV_CIPHER_RET MDrv_CIPHER_SetPowerState(EN_POWER_MODE u16PowerState);

DLL_PUBLIC MS_U32 MDrv_CIPHER_GCM_GetTag(MS_U8 *pu8OutputBuff, MS_U32 u32OutputBuffSize);

#ifdef ENABLE_BGC
DLL_PUBLIC DRV_CIPHER_RET MDrv_CIPHER_BGC_Enable(MS_U32 u32EngId,MS_U32 u32Algo, MS_U8 *pu8Golden, MS_PHY u32SAddr, MS_U32 u32Size);
DLL_PUBLIC DRV_CIPHER_RET MDrv_CIPHER_BGC_Check(MS_U32 u32EngId, MS_BOOL *bOK);
#endif

#ifdef __cplusplus
}
#endif

//////// Old Interface /////////////////
//--------------------------------------------------------------------------------------------------
//  Function Prototype
//--------------------------------------------------------------------------------------------------
#include "drvAESDMA.h"

#endif
