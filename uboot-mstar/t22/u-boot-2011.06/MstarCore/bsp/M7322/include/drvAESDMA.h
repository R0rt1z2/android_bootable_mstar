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
/// @brief  AESDMA Driver Interface
///
////////////////////////////////////////////////////////////////////////////////////////////////////


/*! \defgroup G_AESDMA AESDMA interface
    \ingroup  G_CIPHER

    \brief
    AESDMA is a DMA ip with En/Decrypt engine. It can Encrypt/Decrypt data transmission between MIU to MIU,
    parse data header, provide random numbers, calculate SHA and RSA of data.

    <b>Features</b>

    - Encrypt/Decrypt Algorithm: AES/DES/Triple DEC + ECB/CBC
    - SHA1, SHA256
    - RSA
    - Hardware parser


    <b> AESDMA Block Diagram: </b> \n

    Data  ==>   AESDMA   ==>  En/Decrypted Data  \n
    Data  ==>   SHA      ==>  Hash Value  \n
    Data  ==>   RSA      ==>  Decrypted Value  \n
    Random Number Generatore   ==>  Random number \n


    \defgroup G_AES_INIT Initialization Task relative
    \ingroup  G_AESDMA
    \defgroup G_AES_EnDecrypt En/Decrypt Task relative
    \ingroup  G_AESDMA
    \defgroup G_AES_SHA Hash relative
    \ingroup  G_AESDMA
    \defgroup G_AES_OTHER  other relative
    \ingroup  G_AESDMA
    \defgroup G_AES_ToBeModified AESDMA api to be modified
    \ingroup  G_AESDMA
    \defgroup G_AES_ToBeRemove AESDMA api to be removed
    \ingroup  G_AESDMA
*/

#ifndef _DRVAESDMA_H_
#define _DRVAESDMA_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "MsTypes.h"
#include "MsDevice.h"
#include "MsCommon.h"
#include "drvCIPHER.h"
#include "UFO.h"

#define AESDMA_UTOPIA20       (1)
//--------------------------------------------------------------------------------------------------
//  Define
//--------------------------------------------------------------------------------------------------
#define AES_ENABLE                   0x01
#define TDES_ENABLE                  0x10
#define WORD_MASK                    0x0000000F
#define RSA_INPUT_SIZE               256
#define RSA_UNIT_SIZE                4
#define SHA_WAIT                     1
#if (SHA_WAIT==1)
#define SHA_TIMEOUT_VALUE            1200000
#endif
#define AES_MAX_TIMEOUT_VALUE        0x500000
#define RSA_MAX_TIMEOUT_VALUE        0x500000
#define RSA_DEFAULT_TIMEOUT_VALUE    0x180000

#define MSIF_AESDMA_LIB_CODE                     {'D','M','A','_'}    //Lib code
#define MSIF_AESDMA_LIBVER                       {'0','3'}            //LIB version
#define MSIF_AESDMA_BUILDNUM                     {'0','3'}            //Build Number
#define MSIF_AESDMA_CHANGELIST                   {'0','0','3','3','1','6','3','7'} //P4 ChangeList Number

#define AESDMA_DRV_VERSION                 /* Character String for DRV/API version             */  \
    MSIF_TAG,                           /* 'MSIF'                                           */  \
    MSIF_CLASS,                         /* '00'                                             */  \
    MSIF_CUS,                           /* 0x0000                                           */  \
    MSIF_MOD,                           /* 0x0000                                           */  \
    MSIF_CHIP,                                                                                  \
    MSIF_CPU,                                                                                   \
    MSIF_AESDMA_LIB_CODE,                  /* IP__                                             */  \
    MSIF_AESDMA_LIBVER,                          /* 0.0 ~ Z.Z                                        */  \
    MSIF_AESDMA_BUILDNUM,                          /* 00 ~ 99                                          */  \
    MSIF_AESDMA_CHANGELIST,  /* CL#                                              */  \
    MSIF_OS

/// AESDMA Cipher Mode
typedef enum
{
    E_DRVAESDMA_CIPHER_ECB = 0,
    E_DRVAESDMA_CIPHER_CTR,
    E_DRVAESDMA_CIPHER_CBC,
    E_DRVAESDMA_CIPHER_DES_ECB,
    E_DRVAESDMA_CIPHER_DES_CTR,
    E_DRVAESDMA_CIPHER_DES_CBC,
    E_DRVAESDMA_CIPHER_TDES_ECB,
    E_DRVAESDMA_CIPHER_TDES_CTR,
    E_DRVAESDMA_CIPHER_TDES_CBC,
    E_DRVAESDMA_CIPHER_CTS_CBC,
    E_DRVAESDMA_CIPHER_CTS_ECB,
    E_DRVAESDMA_CIPHER_DES_CTS_CBC,
    E_DRVAESDMA_CIPHER_DES_CTS_ECB,
    E_DRVAESDMA_CIPHER_TDES_CTS_CBC,
    E_DRVAESDMA_CIPHER_TDES_CTS_ECB,
    E_DRVAESDMA_CIPHER_NONE,  //memory copy
    E_DRVAESDMA_CIPHER_SM4_ECB,
    E_DRVAESDMA_CIPHER_SM4_CTR,
    E_DRVAESDMA_CIPHER_SM4_CBC,
    E_DRVAESDMA_CIPHER_GCM
} DrvAESDMA_CipherMode;

/// AESDMA HW Parser mode
typedef enum
{
    E_DRVAESDMA_PARSER_TS_PKT192 = 0,
    E_DRVAESDMA_PARSER_TS_PKT192_CLEAR,
    E_DRVAESDMA_PARSER_TS_PKT188,
    E_DRVAESDMA_PARSER_TS_PKT188_CLEAR,
    E_DRVAESDMA_PARSER_HDCP20_PKT192,
    E_DRVAESDMA_PARSER_HDCP20_PKT192_CLEAR,
    E_DRVAESDMA_PARSER_HDCP20_PKT188,
    E_DRVAESDMA_PARSER_HDCP20_PKT188_CLEAR,
} DrvAESDMA_ParserMode;

typedef enum
{
    E_DRVAESDMA_PARSER_SCRMB_10 = 0,
    E_DRVAESDMA_PARSER_SCRMB_11,
    E_DRVAESDMA_PARSER_SCRMB_CLEAR,
} DrvAESDMA_ScrmbPattern;

/// SHA Mode
typedef enum
{
    E_DRVAESDMA_SHA1 = 0,
    E_DRVAESDMA_SHA256,
} DrvAESDMA_SHAMode;

/// RSA Mode
typedef enum
{
    E_DRVAESDMA_RSA1024_PUBLIC = 0,
    E_DRVAESDMA_RSA1024_PRIVATE,
    E_DRVAESDMA_RSA2048_PUBLIC,
    E_DRVAESDMA_RSA2048_PRIVATE,
    E_DRVAESDMA_RSA256_PUBLIC,
    E_DRVAESDMA_RSA256_PRIVATE,
} DrvAESDMA_RSAMode;

/// RSA HW Key Mode
typedef enum
{
    E_DRVAESDMA_RSA_HWKEY_PRIVATE = 0,
    E_DRVAESDMA_RSA_HWKEY_PUBLIC1,
    E_DRVAESDMA_RSA_HWKEY_PUBLIC2,
    E_DRVAESDMA_RSA_HWKEY_ROM_PRIVATE,
    E_DRVAESDMA_RSA_HWKEY_ROM_PUBLIC,
} DrvAESDMA_RSAHwKeyMode;

/// AESDMA notification event
typedef enum //_DrvAESDMA_Event
{
    E_DRVAESDMA_EVENT_DATA_INIT         = 0x00000000,
    /// DMA Done
    E_DRVAESDMA_EVENT_DMA_DONE          = 0x00010000,
    /// DMA Pause
    E_DRVAESDMA_EVENT_DMA_PAUSE         = 0x00020000,

} DrvAESDMA_Event;

typedef enum //_DrvTSP_DbgLevel
{
    E_DRVAESDMA_DBG_Release = 0,
    E_DRVAESDMA_DBG_L1, // display error msg
    E_DRVAESDMA_DBG_L2, // display error msg and enter while(1)
} DrvAESDMA_DbgLevel;

typedef enum //_DrvAESDMA_KEY_TYPE
{
    E_DRVAESDMA_ODD_KEY = 0,
    E_DRVAESDMA_EVEN_KEY,
} DrvAESDMA_KEY_TYPE;

typedef enum
{
    E_DRVAESDMA_SECRETKEY_1 = 1,
    E_DRVAESDMA_SECRETKEY_2 = 2,
    E_DRVAESDMA_SECRETKEY_EAEK,
} DrvAESDMA_SecretKeyType;

typedef enum //MESARATI AESDMA
{
	E_DRVAESDMA_ENG0 = 0,
	E_DRVAESDMA_ENG1 = 1,
	E_CMDQ_DRVAESDMA_ENG0 = 2,
	E_CMDQ_DRVAESDMA_ENG1 = 3
} DrvAESDMA_EngSel;

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
typedef MS_U32                         DRVAESDMA_RESULT;

typedef struct DLL_PACKED
{
    MS_U32 u32KeyN[64];
    MS_U32 u32KeyE[64];

}DrvAESDMA_RSAKey;

typedef struct DLL_PACKED
{
    MS_U32 u32Sig[64];

}DrvAESDMA_RSASig;

typedef struct DLL_PACKED
{
    MS_U32 u32RSAOut[64];

}DrvAESDMA_RSAOut;

/// Secure information
typedef struct DLL_PACKED
{
    MS_U32  u32SHMPhyAddr;
    MS_BOOL bSecureModeEnable;
    MS_U32  u32SHMSize;
    MS_U8   u8SHMMiuSel;

}DrvAESDMA_SecureInfo;

typedef struct DLL_PACKED
{
#if defined(UFO_PUBLIC_HEADER_500_3)||defined(UFO_PUBLIC_HEADER_700)
    MS_VIRT u32Addr;
    MS_U32  u32Size;
#else
    MS_U32 u32Addr;
    MS_U32 u32Size;
#endif
}DrvAESDMA_SHADataInfo;

typedef enum
{
    E_DRVAESDMA_HASH_STAGE_FIRST ,
    E_DRVAESDMA_HASH_STAGE_UPDATE ,
    E_DRVAESDMA_HASH_STAGE_LAST ,
    E_DRVAESDMA_HASH_STAGE_ONCE ,
}DrvAESDMA_HASH_STAGE;

typedef struct DLL_PACKED
{
    DrvAESDMA_SHAMode   eMode;
    DrvAESDMA_SHADataInfo  stInput;
    DrvAESDMA_SHADataInfo  stOutput;
}DrvAESDMA_HASHCFG;

/// @name DRVAESDMA_RESULT
/// @ref DRVAESDMA_RESULT
/// return value
/// @{
#define DRVAESDMA_OK                   0x00000000
#define DRVAESDMA_FAIL                 0x00000001
#define DRVAESDMA_INVALID_PARAM        0x00000002
#define DRVAESDMA_FUNC_ERROR           0x00000003
#define DRVAESDMA_MIU_ADDR_ERROR       0x00000004

/// @}

#define AESDMA_DMA_DONE                     0x00010000
#define AESDMA_DMA_PAUSE                    0x00020000
#define AESDMA_STATES_GROUP                 (AESDMA_DMA_DONE  | \
                                             AESDMA_DMA_PAUSE  )

#define AESDMA_PARSER_PID_MAX          0x00001FFF
/// AESDMA notification function
typedef void (*P_DrvAESDMA_EvtCallback)(DrvAESDMA_Event eEvent);

////////////////////////////////////////////////////////////////////////////////
// include utopia v2  header files here
////////////////////////////////////////////////////////////////////////////////
#include "drvAESDMA_v2.h"

//--------------------------------------------------------------------------------------------------
//  Function Prototype
//--------------------------------------------------------------------------------------------------
#ifndef MSOS_TYPE_NUTTX
//-------------------------------------------------------------------------------------------------
/// SetSecurityInfo
/// @ingroup G_AES_OTHER
/// @param  pSecureInfo         \b IN: Security information
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
DRVAESDMA_RESULT MDrv_AESDMA_SetSecurityInfo(DrvAESDMA_SecureInfo *pSecureInfo);
#endif

//-------------------------------------------------------------------------------------------------
/// Initialize AESDMA
/// @ingroup G_AES_INIT
/// @param  u32miu0addr         \b IN: MIU0 Base Address (Unused)
/// @param  u32miu1addr         \b IN: MIU1 Base Address (Unused)
/// @param  u32miunum           \b IN: Numbers of MIU
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
DRVAESDMA_RESULT MDrv_AESDMA_Init(MS_PHY u32miu0addr , MS_PHY u32miu1addr , MS_U32 u32miunum);

//-------------------------------------------------------------------------------------------------
/// Check Cihper Mode
/// @ingroup G_AES_EnDecrypt
/// @param  eMode         \b IN: Cipher Mode
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
DRVAESDMA_RESULT MDrv_AESDMA_QueryCipherMode(DrvAESDMA_CipherMode eMode);

//-------------------------------------------------------------------------------------------------
/// Set Cipher mode and En/Decrypt
/// @ingroup G_AES_EnDecrypt
/// @param  eMode         \b IN: Cipher Mode
/// @param  bDescrypt     \b IN: TRUE: Decrypt, FALSE: Encrypt
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
DRVAESDMA_RESULT MDrv_AESDMA_SelEng(DrvAESDMA_CipherMode eMode, MS_BOOL bDescrypt);

//-------------------------------------------------------------------------------------------------
/// Set a set of key to AESDMA
/// @ingroup G_AES_EnDecrypt
/// @param pu8Key \b IN: The key is represented as vectors of bytes with the MSB first
/// @param u32Len \b IN: The byte length of the pu8Key. 16 for AES/TDES and 8 for DES
/// @return DRVAESDMA_OK : Success
/// @return DRVAESDMA_INVALID_PARAM : Invalid key length
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
DRVAESDMA_RESULT MDrv_AESDMA_SetKey_Ex(const MS_U8 *pu8Key, MS_U32 u32Len);
//-------------------------------------------------------------------------------------------------
/// Set IV to the AESDMA
/// @ingroup G_AES_EnDecrypt
/// @param pu8IV \b IN: The IV is represented as vectors of bytes with the MSB first
/// @param u32Len \b IN: The byte length of the pu8IV. 16 for AES and 8 for TDES/DES
/// @return DRVAESDMA_OK : Success
/// @return DRVAESDMA_INVALID_PARAM : Invalid key length
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
DRVAESDMA_RESULT MDrv_AESDMA_SetIV_Ex(const MS_U8 *pu8IV, MS_U32 u32Len);

//-------------------------------------------------------------------------------------------------
/// Set a set of even key to AESDMA
/// @ingroup G_AES_EnDecrypt
/// @param pCipherKey \b IN: Key set
/// @return DRVAESDMA_OK : Success
/// @return DRVAESDMA_INVALID_PARAM : Invalid key length
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
DRVAESDMA_RESULT MDrv_AESDMA_SetKey(MS_U32 *pCipherKey);

//-------------------------------------------------------------------------------------------------
/// Set Odd/Even Key to AESDMA
/// @ingroup G_AES_EnDecrypt
/// @param pCipherKey \b IN: Key set
/// @param stKeyType \b IN: Enum: E_DRVAESDMA_ODD_KEY, E_DRVAESDMA_EVEN_KEY
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
DRVAESDMA_RESULT MDrv_AESDMA_SetKeySel(MS_U32 *pCipherKey, DrvAESDMA_KEY_TYPE stKeyType);
//-------------------------------------------------------------------------------------------------
/// Set Odd IV to AESDMA
/// @ingroup G_AES_EnDecrypt
/// @param pCipherKey \b IN: Odd IV
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
DRVAESDMA_RESULT MDrv_AESDMA_SetOddIV(MS_U32 *pInitVector);
//-------------------------------------------------------------------------------------------------
/// Enable Secure Key of AESDMA
/// @ingroup G_AES_EnDecrypt
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
DRVAESDMA_RESULT MDrv_AESDMA_SetSecureKey(void);
//-------------------------------------------------------------------------------------------------
/// Set a set of default IV (even iv) to AESDMA
/// @ingroup G_AES_EnDecrypt
/// @param pCipherKey \b IN: IV
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
DRVAESDMA_RESULT MDrv_AESDMA_SetIV(MS_U32 *pInitVector);
//-------------------------------------------------------------------------------------------------
/// Set a set of PS Pattern to AESDMA
/// @ingroup G_AES_EnDecrypt
/// @param u32PTN \b IN: PS Pattern
/// @param u32Mask \b IN: Mask
/// @param bPSin_Enable \b IN: Bit of In Enable
/// @param bPSout_Enable \b IN: Bit of Out Enable
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
DRVAESDMA_RESULT MDrv_AESDMA_SetPS(MS_U32 u32PTN, MS_U32 u32Mask, MS_BOOL bPSin_Enable, MS_BOOL bPSout_Enable);
//-------------------------------------------------------------------------------------------------
/// Release PS of AESDMA
/// @ingroup G_AES_EnDecrypt
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
DRVAESDMA_RESULT MDrv_AESDMA_PSRelease(void);
//-------------------------------------------------------------------------------------------------
/// Set File in/out address and size to AESDMA
/// @ingroup G_AES_EnDecrypt
/// @param u32FileinAddr \b IN: Source Address
/// @param u32FileInNum \b IN: Size of Data
/// @param u32FileOutSAddr \b IN: Dest Start Address
/// @param u32FileOutEAddr \b IN: Dest End Address
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
DRVAESDMA_RESULT MDrv_AESDMA_SetFileInOut(MS_PHY phyFileinAddr, MS_U32 u32FileInNum, MS_PHY phyFileOutSAddr, MS_PHY phyFileOutEAddr);

//-------------------------------------------------------------------------------------------------
/// Trigger AESDMA engine
/// @ingroup G_AES_EnDecrypt
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
DRVAESDMA_RESULT MDrv_AESDMA_Start(MS_BOOL bStart);

//-------------------------------------------------------------------------------------------------
/// Reset AESDMA engine
/// @ingroup G_AES_INIT
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
DRVAESDMA_RESULT MDrv_AESDMA_Reset(void);

//-------------------------------------------------------------------------------------------------
/// Get the status of AESDMA engine
/// @ingroup G_AES_EnDecrypt
/// @return DRVAESDMA_OK : Ready
/// @return Others : Not Ready
//-------------------------------------------------------------------------------------------------
DRVAESDMA_RESULT MDrv_AESDMA_GetStatus(void);

//-------------------------------------------------------------------------------------------------
/// Check the status of AESDMA engine
/// @ingroup G_AES_EnDecrypt
/// @return DRVAESDMA_OK : Done
/// @return Others : Not yet
//-------------------------------------------------------------------------------------------------
DRVAESDMA_RESULT MDrv_AESDMA_IsFinished(void);

//-------------------------------------------------------------------------------------------------
/// Get the byte count of PS matched.
/// @ingroup G_AES_EnDecrypt
/// @return Numbers of byte counts.
//-------------------------------------------------------------------------------------------------
MS_U32 MDrv_AESDMA_GetPSMatchedByteCNT(void);

//-------------------------------------------------------------------------------------------------
/// Get the byte count of PTN matched.
/// @ingroup G_AES_EnDecrypt
/// @return Numbers of byte counts.
//-------------------------------------------------------------------------------------------------
MS_U32 MDrv_AESDMA_GetPSMatchedPTN(void);

//-------------------------------------------------------------------------------------------------
/// Call back function of AESDMA interrupt.
/// @ingroup G_AES_OTHER
/// @param eEvents \b IN: Interrupt event
/// @param pfCallback \b OUT: pointer of callback function
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
DRVAESDMA_RESULT MDrv_AESDMA_Notify(DrvAESDMA_Event eEvents, P_DrvAESDMA_EvtCallback pfCallback);

//-------------------------------------------------------------------------------------------------
/// Set debug level
/// @ingroup G_AES_OTHER
/// @param DbgLevel \b IN: Interrupt event
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
DRVAESDMA_RESULT MDrv_AESDMA_SetDbgLevel(DrvAESDMA_DbgLevel DbgLevel);

//-------------------------------------------------------------------------------------------------
/// Get library version
/// @ingroup G_AES_OTHER
/// @param ppVersion \b OUT: version
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
DRVAESDMA_RESULT MDrv_AESDMA_GetLibVer(const MSIF_Version **ppVersion);

//-------------------------------------------------------------------------------------------------
/// Get random numbers.
/// @ingroup G_AES_OTHER
/// @param u32PABuf \b OUT: an array of random numbers.
/// @param u32Size \b IN: Array size
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
DRVAESDMA_RESULT MDrv_AESDMA_Rand(MS_U32 *u32PABuf, MS_U32 u32Size);

//-------------------------------------------------------------------------------------------------
/// Enable/Disable Clock
/// @ingroup G_AES_INIT
/// @param benable \b IN: TRUE: Enable, FALSE: Disable
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
DRVAESDMA_RESULT MDrv_AESDMA_Set_Clk(MS_BOOL benable);
//-------------------------------------------------------------------------------------------------
/// Calculate SHA
/// @ingroup G_AES_SHA
/// @param eMode \b IN: SHA Mode
/// @param u32PAInBuf \b IN: Source address of data
/// @param u32Size \b IN: Size of data
/// @param u32PAOutBuf \b IN: out hash address
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
DRVAESDMA_RESULT MDrv_SHA_Calculate(DrvAESDMA_SHAMode eMode, MS_PHY phyPAInBuf, MS_U32 u32Size, MS_PHY phyPAOutBuf);

//-------------------------------------------------------------------------------------------------
/// Calculate Manual SHA
/// @ingroup G_AES_SHA
/// @param stCfg \b IN: SHA Config
/// @param eStage \b IN: SHA Stage
/// @param u32DoneBytes \b IN: Size
/// @param pu8SetIV \b IN: Init value
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
DRVAESDMA_RESULT MDrv_SHA_CalculateManual(DrvAESDMA_HASHCFG stCfg, DrvAESDMA_HASH_STAGE eStage, MS_U32 u32DoneBytes, MS_U8 *pu8SetIV);

DRVAESDMA_RESULT SYMBOL_WEAK MDrv_SHA_Accumulation(DrvAESDMA_HASH_STAGE eStage, DrvAESDMA_SHAMode eMode, MS_PHY phySrcBufAddr, MS_U32 u32SrcSize, MS_PHY phyWorkingBufAddr, MS_U32 u32WorkingSize, MS_U16* pu16ShaVal );
//-------------------------------------------------------------------------------------------------
/// MOBF
/// @ingroup G_AES_ToBeRemove
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
DRVAESDMA_RESULT MDrv_MOBF_DmaOnly(void);

//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_AES_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
DRVAESDMA_RESULT MDrv_MOBF_Encrypt(MS_U32 u32Key, MS_BOOL bEnable);

//-------------------------------------------------------------------------------------------------
/// MOBF Decrypt
/// @ingroup G_AES_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
DRVAESDMA_RESULT MDrv_MOBF_Decrypt(MS_U32 u32Key, MS_BOOL bEnable);

//-------------------------------------------------------------------------------------------------
/// MOBF Oneway
/// @ingroup G_AES_ToBeRemove
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
DRVAESDMA_RESULT MDrv_MOBF_OneWay(void);

//-------------------------------------------------------------------------------------------------
/// NA
/// @ingroup G_AES_EnDecrypt
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
DRVAESDMA_RESULT MDrv_AESDMA_Parser_MaskScrmb(MS_BOOL bEnable);

//-------------------------------------------------------------------------------------------------
/// NA
/// @ingroup G_AES_EnDecrypt
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
DRVAESDMA_RESULT MDrv_AESDMA_Parser_SetScrmbPattern(DrvAESDMA_ScrmbPattern ePattern);

//-------------------------------------------------------------------------------------------------
/// NA
/// @ingroup G_AES_EnDecrypt
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
DRVAESDMA_RESULT MDrv_AESDMA_Parser_SetAddedScrmbPattern(DrvAESDMA_ScrmbPattern ePattern);

//-------------------------------------------------------------------------------------------------
/// NA
/// @ingroup G_AES_OTHER
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_U8 MDrv_AESDMA_Parser_QueryPidCount(void);

//-------------------------------------------------------------------------------------------------
/// NA
/// @ingroup G_AES_EnDecrypt
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
DRVAESDMA_RESULT MDrv_AESDMA_Parser_SetPid(MS_U8 u8Index, MS_U16 u16Pid);

//-------------------------------------------------------------------------------------------------
/// NA
/// @ingroup G_AES_EnDecrypt
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
DRVAESDMA_RESULT MDrv_AESDMA_Parser_BypassPid(MS_BOOL bEnable);

//-------------------------------------------------------------------------------------------------
/// NA
/// @ingroup G_AES_EnDecrypt
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
DRVAESDMA_RESULT MDrv_AESDMA_Parser_Encrypt(DrvAESDMA_ParserMode eMode);

//-------------------------------------------------------------------------------------------------
/// NA
/// @ingroup G_AES_EnDecrypt
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
DRVAESDMA_RESULT MDrv_AESDMA_Parser_Decrypt(DrvAESDMA_ParserMode eMode);

//-------------------------------------------------------------------------------------------------
/// NA
/// @ingroup G_AES_OTHER
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_AESDMA_Lock(void);

//-------------------------------------------------------------------------------------------------
/// NA
/// @ingroup G_AES_OTHER
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_AESDMA_Unlock(void);

//-------------------------------------------------------------------------------------------------
/// NA
/// @ingroup G_AES_EnDecrypt
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
DRVAESDMA_RESULT MDrv_RSA_Calculate(DrvAESDMA_RSASig *pstSign, DrvAESDMA_RSAKey *pstKey, DrvAESDMA_RSAMode eMode);
//-------------------------------------------------------------------------------------------------
/// NA
/// @ingroup G_AES_EnDecrypt
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
DRVAESDMA_RESULT MDrv_RSA_Calculate_Hw_Key(DrvAESDMA_RSASig *pstSign, DrvAESDMA_RSAHwKeyMode eMode);
DRVAESDMA_RESULT MDrv_RSA_IsFinished(void);

//-------------------------------------------------------------------------------------------------
/// NA
/// @ingroup G_AES_EnDecrypt
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
DRVAESDMA_RESULT MDrv_RSA_Output(DrvAESDMA_RSAMode eMode, DrvAESDMA_RSAOut *pstRSAOut);

//-------------------------------------------------------------------------------------------------
/// NA
/// @ingroup G_AES_OTHER
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_U32 MDrv_AESDMA_SetPowerState(EN_POWER_MODE u16PowerState);

//-------------------------------------------------------------------------------------------------
/// NA
/// @ingroup G_AES_OTHER
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
DRVAESDMA_RESULT MDrv_AESDMA_Exit(void);

//-------------------------------------------------------------------------------------------------
/// NA
/// @ingroup G_AES_OTHER
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
DRVAESDMA_RESULT MDrv_AESDMA_IsSecretKeyInNormalBank(void);

DRVAESDMA_RESULT MDrv_AESDMA_EnableTwoKeys(MS_BOOL bEnable);
DRVAESDMA_RESULT MDrv_AESDMA_SetOddKey(MS_U32 *pInitVector);
DRVAESDMA_RESULT MDrv_AESDMA_ParserStart(MS_BOOL bStart);
DRVAESDMA_RESULT _MDrv_AESDMA_NormalReset(void);

//-------------------------------------------------------------------------------------------------
/// NA
/// @ingroup G_AES_INIT
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
DRVAESDMA_RESULT MDrv_AESDMA_NormalReset(void);

DRVAESDMA_RESULT _MDrv_AESDMA_SelEng(DrvAESDMA_CipherMode eMode, MS_BOOL bDescrypt);
DRVAESDMA_RESULT _MDrv_AESDMA_SetKey(MS_U32 *pCipherKey);
DRVAESDMA_RESULT _MDrv_AESDMA_SetIV(MS_U32 *pInitVector);
DRVAESDMA_RESULT _MDrv_AESDMA_SetFileInOut(MS_PHY phy64FileinAddr, MS_U32 u32FileInNum, MS_PHY phy64FileOutSAddr, MS_PHY phy64FileOutEAddr);
DRVAESDMA_RESULT _MDrv_AESDMA_Start(MS_BOOL bStart);
DRVAESDMA_RESULT _MDrv_RSA_Calculate(DrvAESDMA_RSASig *pstSign, DrvAESDMA_RSAKey *pstKey, DrvAESDMA_RSAMode eMode);
DRVAESDMA_RESULT _MDrv_SHA_Calculate(DrvAESDMA_SHAMode eMode, MS_PHY phy64PAInBuf, MS_U32 u32Size, MS_PHY phy64PAOutBuf);
DRVAESDMA_RESULT _MDrv_RSA_IsFinished(void);
DRVAESDMA_RESULT _MDrv_RSA_Output(DrvAESDMA_RSAMode eMode, DrvAESDMA_RSAOut *pstRSAOut);
DRVAESDMA_RESULT _MDrv_AESDMA_Init(MS_PHY phy64miu0addr , MS_PHY phy64miu1addr , MS_U32 u32miunum);
DRVAESDMA_RESULT _MDrv_AESDMA_Reset(void);
DRVAESDMA_RESULT _MDrv_AESDMA_Set_Clk(MS_BOOL benable);
DRVAESDMA_RESULT _MDrv_AESDMA_SetPS(MS_U32 u32PTN, MS_U32 u32Mask, MS_BOOL bPSin_Enable, MS_BOOL bPSout_Enable);
DRVAESDMA_RESULT _MDrv_AESDMA_PSRelease(void);
#if defined(UFO_PUBLIC_HEADER_700)
DRVAESDMA_RESULT _MDrv_AESDMA_IsFinished(MS_U32 *u32RetVal);
DRVAESDMA_RESULT _MDrv_AESDMA_GetStatus(MS_U32 *u32RetVal);
MS_U32 _MDrv_AESDMA_GetPSMatchedByteCNT(MS_U32 *u32RetVal);
#endif


MS_U32 _MDrv_AESDMA_GetPSMatchedPTN(void);
DRVAESDMA_RESULT _MDrv_AESDMA_Notify(DrvAESDMA_Event eEvents, P_DrvAESDMA_EvtCallback pfCallback);
DRVAESDMA_RESULT _MDrv_AESDMA_Get_Rand_Num(MS_U8 *pu8Num);
DRVAESDMA_RESULT _MDrv_AESDMA_Parser_MaskScrmb(MS_BOOL bEnable);
DRVAESDMA_RESULT _MDrv_AESDMA_Parser_SetScrmbPattern(DrvAESDMA_ScrmbPattern ePattern);
DRVAESDMA_RESULT _MDrv_AESDMA_Parser_BypassPid(MS_BOOL bEnable);
DRVAESDMA_RESULT _MDrv_AESDMA_Parser_SetPid(MS_U8 u8Index, MS_U16 u16Pid);
DRVAESDMA_RESULT _MDrv_AESDMA_Rand(MS_U32 *u32PABuf, MS_U32 u32Size);

//-------------------------------------------------------------------------------------------------
/// NA
/// @ingroup G_AES_OTHER
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
DRVAESDMA_RESULT MDrv_HDCP_ProcessCipher(MS_U8 u8Idx, MS_U8* pu8Riv, MS_U8 *pu8ContentKey);

DRVAESDMA_RESULT MDrv_AESDMA_SetDefaultCAVid(MS_U32 u32CAVid);
//-------------------------------------------------------------------------------------------------
/// Set index of Secure Key
/// @ingroup G_AES_EnDecrypt
/// @param u8KeyIdx \b IN: Index of efuse Key  (1:efuse Key1, 2: efuse key2)
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
DRVAESDMA_RESULT SYMBOL_WEAK MDrv_AESDMA_SetSecuredKeyIndex (MS_U8 u8KeyIdx);

DRVAESDMA_RESULT _MDrv_AESDMA_Parser_Encrypt(DrvAESDMA_ParserMode eMode);
#if defined(UFO_PUBLIC_HEADER_700)||defined(UFO_PUBLIC_HEADER_500)
MS_U8 _MDrv_AESDMA_Parser_QueryPidCount(MS_U8* retCount);
#endif
DRVAESDMA_RESULT _MDrv_AESDMA_Parser_Decrypt(DrvAESDMA_ParserMode eMode);
DRVAESDMA_RESULT _MDrv_AESDMA_Parser_SetAddedScrmbPattern(DrvAESDMA_ScrmbPattern ePattern);
DRVAESDMA_RESULT _MDrv_AESDMA_SetKeySel(MS_U32 *pCipherKey, DrvAESDMA_KEY_TYPE stKeyType);
DRVAESDMA_RESULT _MDrv_AESDMA_SetOddIV(MS_U32 *pInitVector);
DRVAESDMA_RESULT _MDrv_AESDMA_IsSecretKeyInNormalBank(MS_U8 *retVal);
DRVAESDMA_RESULT _MDrv_AESDMA_EnableTwoKeys(MS_BOOL bEnable);
DRVAESDMA_RESULT _MDrv_AESDMA_SetSecureKey(void);

//-------------------------------------------------------------------------------------------------
/// NA
/// @ingroup G_AES_OTHER
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
DRVAESDMA_RESULT MDrv_HDCP_GetHdcpCipherState(MS_U8 u8Idx, MS_U8 *pu8State);

DRVAESDMA_RESULT MDrv_HDMI_GetM0(MS_U32 u32InputPortType, MS_U8 *pu8Data);
//-------------------------------------------------------------------------------------------------
/// Set index of kslot
/// @ingroup G_AES_EnDecrypt
/// @param u32Index \b IN: Index of kslot
/// @param bIsOddKey \b IN: Key type (True: odd key, False: even key)
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
DRVAESDMA_RESULT SYMBOL_WEAK MDrv_AESDMA_SetKeyIndex (MS_U32 u32Index, MS_BOOL bIsOddKey);

//-------------------------------------------------------------------------------------------------
/// Enable AES CTR64
/// @ingroup G_AES_EnDecrypt
/// @param bCtr64En \b IN: True: Support CTR64 Algo
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
DRVAESDMA_RESULT SYMBOL_WEAK MDrv_AESDMA_SetAesCtr64 (MS_BOOL bCtr64En);

//-------------------------------------------------------------------------------------------------
/// Allocate empty key slot
/// @ingroup G_AES_EnDecrypt
/// @param u32Index \b OUT: Index of key slot
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
DRVAESDMA_RESULT SYMBOL_WEAK MDrv_AESDMA_AllocKeySlot (MS_U32 *pu32Index);

//-------------------------------------------------------------------------------------------------
/// Free key slot
/// @ingroup G_AES_EnDecrypt
/// @param u32Index \b IN: Index of key slot
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
DRVAESDMA_RESULT SYMBOL_WEAK MDrv_AESDMA_FreeKeySlot (MS_U32 u32Index);

MS_BOOL MDrv_AESDMA_GetConfig(PAESDMA_INIT sAesdmaInit);

//reserved for secure protect function
/*
DRVAESDMA_RESULT MDrv_AESDMA_SecureSetKey(MS_U32 *pCipherKey);
DRVAESDMA_RESULT MDrv_AESDMA_SecureProtect(MS_BOOL bEnable);
DRVAESDMA_RESULT MDrv_AESDMA_SecureSetIV(MS_U32 *pInitVector);
DRVAESDMA_RESULT MDrv_AESDMA_SetFileOutEnable(MS_BOOL bEnable);
DRVAESDMA_RESULT MDrv_AESDMA_SecureStart(MS_BOOL bStart);
DRVAESDMA_RESULT MDrv_AESDMA_SecureIsFinished(void);
*/

//--------------------------------------------------------------------------------------------------
//  CIPHER Function Prototype
//--------------------------------------------------------------------------------------------------
#if (defined(CONFIG_NUTTX) || defined(CONFIG_OPTEE))
DRV_CIPHER_RET MDrv_CIPHER_DMACalcList(DRV_CIPHER_DMACFG* pstCfg, MS_U32 u32NumOfCmds, MS_U32 *pu32CmdId, MS_PHY BufferAddr, MS_PHY BufferLen,  MS_U32 CMDQ_MIU_SELECT);
#endif


#ifdef __cplusplus
}
#endif


#endif // _DRVAESDMA_H_
