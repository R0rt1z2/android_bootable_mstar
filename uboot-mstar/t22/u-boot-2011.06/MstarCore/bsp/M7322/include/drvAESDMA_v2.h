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

//////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @brief  AESDMA Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_AESDMA_V2_H_
#define _DRV_AESDMA_V2_H_

#include "UFO.h"

#ifdef __cplusplus
extern "C"
{
#endif

//-------------------------------------------------------------------------------------------------
// Defines
//-------------------------------------------------------------------------------------------------
// below for utopia20 structure


typedef enum {
    MDrv_CMD_AESDMA_Init,
    MDrv_CMD_AESDMA_SetIV,
    MDrv_CMD_AESDMA_Reset,
    MDrv_CMD_AESDMA_SetFileInOut,
    MDrv_CMD_AESDMA_SetKey,
    MDrv_CMD_AESDMA_SelEng,
    MDrv_CMD_AESDMA_Start,
    MDrv_CMD_AESDMA_GetStatus,
    MDrv_CMD_AESDMA_IsFinished,
    MDrv_CMD_AESDMA_SetClk,
    MDrv_CMD_AESDMA_SetSecureKey,
    MDrv_CMD_AESDMA_SetPS,
    MDrv_CMD_AESDMA_PSRelease,
    MDrv_CMD_AESDMA_GetPSMatchedByteCNT,
    MDrv_CMD_AESDMA_GetPSMatchedPTN,
    MDrv_CMD_AESDMA_Notify,
    MDrv_CMD_AESDMA_Rand,
    MDrv_CMD_RSA_Calculate,
    MDrv_CMD_RSA_IsFinished,
    MDrv_CMD_RSA_Output,
    MDrv_CMD_SHA_Calculate,
    MDrv_CMD_AESDMA_Memcpy,
    MDrv_CMD_SHA_CalculateManual,
    MDrv_CMD_AESDMA_PARSER_MASKSCRMB,
    MDrv_CMD_AESDMA_PARSER_SETSCRMBPATTERN,
    MDrv_CMD_AESDMA_PARSER_BYPASSPID,
    MDrv_CMD_AESDMA_PARSER_SETPID,
    MDrv_CMD_AESDMA_PARSER_ENCRYPT,
    MDrv_CMD_AESDMA_PARSER_DECRYPT,
    MDrv_CMD_AESDMA_PARSER_Start,
    MDrv_CMD_AESDMA_PARSER_SETADDEDSCRMBPATTERN,
    MDrv_CMD_AESDMA_PARSER_QUERYPIDCOUNT,
    MDrv_CMD_AESDMA_ENABLE_Two_Key,
    MDrv_CMD_AESDMA_SetOddKey,
    MDrv_CMD_AESDMA_SetOddIV,
    MDrv_CMD_IS_SECRETKEY_IN_NORMAL_BLANK,
    MDrv_CMD_AESDMA_SetKey_Ex,
    MDrv_CMD_AESDMA_SetIV_Ex,
    MDrv_CMD_HDCP_MDrv_PROCESSCIPHER,
    MDrv_CMD_AESDMA_Set_Efuse_Key_Index,
    MDrv_CMD_HDCP_GetHdcpCipherState,
    MDrv_CMD_AESDMA_Set_Key_Index,
    MDrv_CMD_AESDMA_Set_AES_CTR64_En,
    MDrv_CMD_AESDMA_SetDefaultCAVid,
    MDrv_CMD_AESDMA_GetConfig,
    MDrv_CMD_AESDMA_AllocKeySlot,
    MDrv_CMD_AESDMA_FreeKeySlot,
    MDrv_CMD_CIPHER_DMACALC,
    MDrv_CMD_CIPHER_ISDMADONE,
    MDrv_CMD_CIPHER_DMACALCLIST,
    MDrv_CMD_AESDMA_SetPowerState,
    MDrv_CMD_AESDMA_QueryCipherMode,
    MDrv_CMD_CIPHER_GCM_GetTag
} eAesdmaIoctlOpt;

//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
// Type and Structure Declaration
//-------------------------------------------------------------------------------------------------

#ifdef BIFROST_32BIT_MODE
typedef struct DLL_PACKED _AESDMA_INIT
{
    MS_U32 u32miu0addr;
    MS_U32 u32miu1addr;
    MS_U32 u32miunum;
}AESDMA_INIT, *PAESDMA_INIT;

typedef struct DLL_PACKED _AESDMA_FILE_INOUT
{
    MS_U32 u32FileinAddr;
    MS_U32 u32FileInNum;
    MS_U32 u32FileOutSAddr;
    MS_U32 u32FileOutEAddr;
}AESDMA_FILE_INOUT, *PAESDMA_FILE_INOUT;
#else
typedef struct DLL_PACKED _AESDMA_INIT
{
    MS_PHY u32miu0addr;
    MS_PHY u32miu1addr;
    MS_U32 u32miunum;
}AESDMA_INIT, *PAESDMA_INIT;

typedef struct DLL_PACKED _AESDMA_FILE_INOUT
{
    MS_PHY u32FileinAddr;
    MS_U32 u32FileInNum;
    MS_PHY u32FileOutSAddr;
    MS_PHY u32FileOutEAddr;
}AESDMA_FILE_INOUT, *PAESDMA_FILE_INOUT;
#endif

typedef struct DLL_PACKED _AESDMA_SEL_ENG
{
    DrvAESDMA_CipherMode eMode;
    MS_BOOL bDescrypt;
}AESDMA_SEL_ENG, *PAESDMA_SEL_ENG;

typedef struct DLL_PACKED _AESDMA_SET_IV
{
    MS_U32 *pInitVector;
}AESDMA_SET_IV, *PAESDMA_SET_IV;

typedef struct DLL_PACKED _AESDMA_SET_KEY
{
    MS_U32 *pCipherKey;
}AESDMA_SET_KEY, *PAESDMA_SET_KEY;

typedef struct DLL_PACKED _AESDMA_SET_KEY_EX
{
    const MS_U8 *pu8Key;
    MS_U32 u32Len;
}AESDMA_SET_KEY_EX, *PAESDMA_SET_KEY_EX;

typedef struct DLL_PACKED _AESDMA_SET_IV_EX
{
    const MS_U8 *pu8IV;
    MS_U32 u32Len;
}AESDMA_SET_IV_EX, *PAESDMA_SET_IV_EX;


typedef struct DLL_PACKED _AESDMA_SET_PS
{
    MS_U32 u32PTN;
    MS_U32 u32Mask;
    MS_BOOL bPSin_Enable;
    MS_BOOL bPSout_Enable;
}AESDMA_SET_PS, *PAESDMA_SET_PS;

typedef struct DLL_PACKED _AESDMA_START
{
    MS_BOOL bStart;
}AESDMA_START, *PAESDMA_START;


typedef struct DLL_PACKED _AESDMA_NOTIFY
{
    DrvAESDMA_Event eEvents;
    P_DrvAESDMA_EvtCallback pfCallback;
}AESDMA_NOTIFY, *PAESDMA_NOTIFY;

typedef struct DLL_PACKED _AESDMA_RAND
{
    MS_U32 *u32PABuf;
    MS_U32 u32Size;
}AESDMA_RAND, *PAESDMA_RAND;

typedef struct DLL_PACKED _RSA_CALCULATE
{
    DrvAESDMA_RSASig *pstSign;
    DrvAESDMA_RSAKey *pstKey;
    DrvAESDMA_RSAMode eMode;
}RSA_CALCULATE, *PRSA_CALCULATE;

typedef struct DLL_PACKED _RSA_OUTPUT
{
    DrvAESDMA_RSAMode eMode;
    DrvAESDMA_RSAOut *pstRSAOut;
}RSA_OUTPUT, *PRSA_OUTPUT;

#ifdef BIFROST_32BIT_MODE
typedef struct DLL_PACKED _SHA_CALCULATE
{
    DrvAESDMA_SHAMode eMode;
    MS_U32 u32PAInBuf;
    MS_U32 u32Size;
    MS_U32 u32PAOutBuf;
}SHA_CALCULATE, *PSHA_CALCULATE;
#else
typedef struct DLL_PACKED _SHA_CALCULATE
{
    DrvAESDMA_SHAMode eMode;
    MS_PHY u32PAInBuf;
    MS_U32 u32Size;
    MS_PHY u32PAOutBuf;
}SHA_CALCULATE, *PSHA_CALCULATE;
#endif

typedef struct DLL_PACKED _SHA_CALCULATE_MANUAL
{
    DrvAESDMA_HASHCFG stCfg;
    DrvAESDMA_HASH_STAGE eStage;
    MS_U32 u32DoneBytes;
    MS_U8 *pu8SetIV;
}SHA_CALCULATE_MANUAL, *PSHA_CALCULATE_MANUAL;

typedef struct DLL_PACKED _AESDMA_PARSER_CONFIGURATIONS
{
    DrvAESDMA_ScrmbPattern eScrmbPattern;
    DrvAESDMA_ParserMode   eParserMode;
    DrvAESDMA_ScrmbPattern eAddedScrmbPattern;
}AESDMA_PARSER_CONFIGURATIONS, *PAESDMA_PARSER_CONFIGURATIONS;

typedef struct DLL_PACKED _AESDMA_PARSER_PID_CONFIGURATIONS
{
    //Don't care PidIndex and Pid if BypassPid is true;
    MS_BOOL bBypassPid;
    MS_U8   u8PidIndex;
    MS_U16  u16Pid;
}AESDMA_PARSER_PID_CONFIGURATIONS, *PAESDMA_PARSER_PID_CONFIGURATIONS;

typedef struct DLL_PACKED _AESDMA_CAVID
{
    MS_U32 u32CAVid;
}AESDMA_CAVID, *PAESDMA_CAVID;

typedef struct DLL_PACKED _HDCP_CIPHER
{
    MS_U8 u8Idx;
    MS_U8 pu8Riv[8];
    MS_U8 pu8ContentKey[16];
}HDCP_CIPHER, *PHDCP_CIPHER;

typedef struct DLL_PACKED _HDCP_CIPHER_STATE
{
    MS_U8 u8Idx;         //Port Index.
    MS_U8 *pu8State;    //For return Status.
}HDCP_CIPHER_STATE, *PHDCP_CIPHER_STATE;

typedef struct DLL_PACKED _AESDMA_KEY_INDEX
{
    MS_BOOL bOddKeyEn;     //FALSE: Default/Even Key, TRUE: Odd Key
    MS_U32  u32KeyIndex;   //Key Index
}AESDMA_KEY_INDEX, *PAESDMA_KEY_INDEX;

typedef struct DLL_PACKED _AESDMA_ALLOC_KEY_SLOT
{
    MS_U32  *pu32KeyIndex; //For return Key Index
}AESDMA_ALLOC_KEY_SLOT, *PAESDMA_ALLOC_KEY_SLOT;

typedef struct DLL_PACKED _CIPHER_DMACALCLIST
{
    DRV_CIPHER_DMACFG* pstCfg;
    MS_U32 CMDQ_MIU_SELECT;
    MS_U32 u32NumOfCmds;
    MS_U32 *pu32CmdId;
    MS_PHY BufferAddr;
    MS_PHY BufferLen;
}CIPHER_DMACALCLIST, *PCIPHER_DMACALCLIST;

typedef struct DLL_PACKED _AESDMA_RAND_TEE
{
    AESDMA_RAND    stCfg;
}AESDMA_RAND_TEE;

//-------------------------------------------------------------------------------------------------
// Extern Global Variabls
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Extern Functions
//-------------------------------------------------------------------------------------------------

//////////////////////////////////////////////////////////////

#ifdef __cplusplus
}
#endif

#endif // _DRV_AESDMA_V2_H_

